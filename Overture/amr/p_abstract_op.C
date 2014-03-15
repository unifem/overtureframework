#define COMPILE_PPP

extern bool automaticCommunication;  // *wdh* 

// kkc 061031 include mpi here to avoid mpich/bsparti error
#include <mpi.h> 
#include"A++.h"
// Inline functions specific to use by the abstract operators
#include<inline_support.h>

#if !defined(PPP)
#error PPP should be defined for P++
#endif


#define USE_TEMPORARY_DELETE_FUNCTIONS TRUE
#define USE_FIXUP_BASE_FUNCTION TRUE



// **************************************************************************
// Support function adjusts the base of the A++ temporary (which has default
// base of ZERO (typically)) so that it reflects the global indexspace required
// of the partitioned P++ array.
// **************************************************************************
#if !defined(GNU)
// inline
#endif
void
Array_Domain_Type::fixupLocalBase ( 
   Array_Domain_Type& New_Parallel_Domain,
   SerialArray_Domain_Type& New_Serial_Domain, 
   const Array_Domain_Type& Old_Parallel_Domain, 
   const SerialArray_Domain_Type& Old_Serial_Domain ) 
   {
  // This function sets up the base of the local A++ array inside the P++ array 
  // object to be consistant with the global indexspace of the distributed P++ 
  // array object.

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of Array_Domain_Type::fixupLocalBase \n");

     if (APP_DEBUG > 6)
        {
          New_Parallel_Domain.display
	    ("New_Parallel_Domain (PARALLEL DESCRIPTOR - TO BE FIXED UP)");
          New_Serial_Domain.display  
	    ("New_Serial_Domain   (SERIAL DESCRIPTOR   - TO BE FIXED UP)");
          Old_Parallel_Domain.display
	    ("Old_Parallel_Domain (PARALLEL DESCRIPTOR - REQUIRED INFORMATION)");
          Old_Serial_Domain.display  
	    ("Old_Serial_Domain   (SERIAL DESCRIPTOR   - REQUIRED INFORMATION)");

          printf ("******************************************************** \n");
          printf ("BEFORE for loop in Array_Domain_Type::fixupLocalBase \n");
          printf ("******************************************************** \n");
        }
     if (APP_DEBUG > 9)
        {
          New_Parallel_Domain.display ("New_Parallel_Domain (PARALLEL DESCRIPTOR - BEFORE BEING FIXED UP)");
        }
#endif

     int Axis;
     for (Axis=0; Axis < MAX_ARRAY_DIMENSION; Axis++)
        {
	  int Difference = 
	     (Old_Serial_Domain.Data_Base[Axis] - 
	      Old_Parallel_Domain.Data_Base[Axis]) + 
              New_Parallel_Domain.Data_Base[Axis] -
              New_Serial_Domain.Data_Base[Axis];

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 0)
               printf ("Axis = %d 1st Difference = %d \n",Axis,Difference);
#endif

          if (!New_Parallel_Domain.Uses_Indirect_Addressing)
	  {
             New_Serial_Domain.Data_Base[Axis] +=Difference;
             New_Serial_Domain.User_Base[Axis] +=Difference;

             Difference = New_Serial_Domain.Data_Base[Axis] - 
                          Old_Serial_Domain.Data_Base[Axis];
	  }
	  else
	  {

	     // ... if indirect addressing is used by the parallel array
	     //  don't modify the serial array base because this will undo
	     //  earlier fix in Fix_SerialArray.  Instead compute new
	     //  Difference the same way without modifying Data_Base ...

             Difference = New_Serial_Domain.Data_Base[Axis] + Difference - 
                          Old_Serial_Domain.Data_Base[Axis];
	  }

/*
// ... (4/28/98, kdb) turn back on, this section was turned off by Dan, 
//   I don't know why ...
*/
#if 1
       // Difference = New_Serial_Domain.Data_Base[Axis] - 
       //              Old_Serial_Domain.Data_Base[Axis];

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 0)
               printf ("Axis = %d  2nd Difference = %d \n",Axis,Difference);
#endif
       // This is not adjusted in setBase so it need not be done here (I think - hope)
        New_Parallel_Domain.Global_Index     [Axis] += Difference;
        New_Parallel_Domain.Local_Mask_Index [Axis] += Difference;

       // Some error checking specific to one processor only!
          if (Communication_Manager::Number_Of_Processors == 1)
             {
               APP_ASSERT (New_Parallel_Domain.Global_Index     [Axis].getBase() == New_Parallel_Domain.getRawBase(Axis));
               APP_ASSERT (New_Parallel_Domain.Local_Mask_Index [Axis].getBase() == New_Parallel_Domain.getRawBase(Axis));
             }

       // We can't always APP_ASSERT this to be true (why not?)
       // APP_ASSERT (New_Parallel_Domain.Array_Conformability_Info == NULL);
       // if (New_Parallel_Domain.Array_Conformability_Info != NULL)
       //   New_Parallel_Domain.Array_Conformability_Info->
       //      Lhs_Lvalue_Assignment_Index[Axis] += Difference;
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 6)
        {
          printf ("******************************************************** \n");
          printf ("AFTER for loop in Array_Domain_Type::fixupLocalBase \n");
          printf ("******************************************************** \n");

          New_Parallel_Domain.display
	    ("New_Parallel_Domain (PARALLEL DESCRIPTOR - AFTER BEING FIXED UP)");
          New_Serial_Domain.display  
	    ("New_Serial_Domain   (SERIAL DESCRIPTOR   - AFTER BEING FIXED UP)");

          printf ("******************************************************** \n");
        }
#endif

  // The domain is not sufficiently setup to pass the Test_Consistency function!
  // New_Serial_Domain.Test_Consistency   
  //("New_Serial_Domain.Test_Consistency called from Array_Domain_Type::fixupLocalBase");
  // New_Parallel_Domain.Test_Consistency 
  //("Old_Parallel_Domain.Test_Consistency called from Array_Domain_Type::fixupLocalBase");

#if defined(PPP)
  // printf ("Exiting in Array_Domain_Type::fixupLocalBase \n");
  // APP_ABORT();
#endif
   }


#define DOUBLEARRAY
doubleArray *Last_Lhs_doubleArray_Operand = NULL;

#if 0

#if !defined(GNU)
// inline
#endif
void
doubleArray::Modify_Reference_Counts_And_Manage_Temporaries ( 
   Array_Conformability_Info_Type *Array_Set, 
   doubleArray *Temporary, 
   doubleSerialArray* Lhs_Serial_PCE_Array, 
   doubleSerialArray* Rhs_Serial_PCE_Array, 
   doubleSerialArray *Data )
   {
  // The following is a confusing point of P++ reference count management.  It was (is)
  // the source of many memory leaks that were (are) difficult to track down.

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of doubleArray::Modify_Reference_Counts_And_Manage_Temporaries (Lhs,Rhs) \n");
#endif
  // This means that the Lhs_Serial_PCE_Array and Rhs_Serial_PCE_Array 
  // are both views built from Build_Pointer_To_View_Of_Array
  // bool Lhs_Is_A_View = TRUE;
  // bool Rhs_Is_A_View = TRUE;

     bool Lhs_Serial_PCE_Array_Reused = (Data == Lhs_Serial_PCE_Array);
     bool Rhs_Serial_PCE_Array_Reused = (Data == Rhs_Serial_PCE_Array);

     bool New_Array_Used = (Lhs_Serial_PCE_Array_Reused == FALSE) &&
                              (Rhs_Serial_PCE_Array_Reused == FALSE);

     if (New_Array_Used)
        {
       // We don't have to do anything the reference count of the Lhs_Serial_PCE_Array
       // and Rhs_Serial_PCE_Array should be 1 (or greater).
#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 0)
               printf ("New_Array_Used == TRUE \n");
#endif
       // In this case the serial binary operation build a new temporary to return (Lhs was not reused)
       // We delete the raw data because the serial array shell is deleted elsewhere?
#if 1
       // Test (11/10/2000) trying to fix reference counting mechanism
       // Temporary->Array_Descriptor.SerialArray->Delete_Array_Data();
          APP_ASSERT (Temporary != NULL);
          APP_ASSERT (Temporary->Array_Descriptor.SerialArray != NULL);
          APP_ASSERT (Temporary->Array_Descriptor.SerialArray->getReferenceCount() >= doubleArray::getReferenceCountBase());
          Temporary->Array_Descriptor.SerialArray->decrementReferenceCount();
          if (Temporary->Array_Descriptor.SerialArray->getReferenceCount() < doubleArray::getReferenceCountBase())
               delete Temporary->Array_Descriptor.SerialArray;
#endif
       // APP_ASSERT (Lhs_Serial_PCE_Array->getRawDataReferenceCount() > 0);
       // APP_ASSERT (Rhs_Serial_PCE_Array->getRawDataReferenceCount() > 0);
        }
       else
        {
#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 0)
               printf ("New_Array_Used == FALSE \n");
#endif
       // While we have to increment the SerialArray's reference count so that the delete in the 
       // binary operator will not destroy the SerialArray (this is because the Lhs_SerialArray and the 
       // result of the Serail array binary operation are the same array -- because the Lhs 
       // was resused) --- we have to decrement the RawDataReferenceCount
       // because we want to use the raw data obtained though the view taken in PCE and
       // we will not be using the original array from which that view was taken.
       // Data->decrementRawDataReferenceCount();

       // This code construction concerns me since it still does not seem that we should
       // be checking for the Data->getRawDataReferenceCount() > 0.

          printf ("##### Questionable decrement of the reference count if it is greater than the DataReferenceCountBase \n");
          if (Data->getRawDataReferenceCount() > getRawDataReferenceCountBase())
               Data->decrementRawDataReferenceCount();
          Data->incrementReferenceCount();
        }

  // Copy the input serialArray into the temporary (the return value for the calling (parent) functions)
     Temporary->Array_Descriptor.SerialArray = Data;
   }

#endif

// **************************************************************************
// **************************************************************************
//                   ABSTRACT OPERATORS SPECIFIC TO P++
//           (follows same logic as in A++ abstract operators)
//     (where statement logic is handled at the lower Serial_A++ level)
// **************************************************************************
// **************************************************************************
     
// **********************************************************
// The equals operator taking a scalar
// **********************************************************
doubleArray &
doubleArray::operator= ( double x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of doubleArray::operator=(double x)! \n");

  // This is the only test we can do on the input!
     Test_Consistency ("Test in doubleArray::operator= (double x)");
#endif

     APP_ASSERT(Array_Descriptor.SerialArray != NULL);
  // APP_ASSERT(Array_Descriptor.SerialArray->Array_Descriptor != NULL);

  // The SerialArray is a view of the correct portion of the raw data
  // so we can just call the SerialArray (A++) assignment operator.
  /* ...(5/5/98, kdb) WRONG: if there is a where mask this doesn't work ... */
  //   (*Array_Descriptor.SerialArray) = x;

     doubleSerialArray *This_SerialArray = NULL;
     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing)
               Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement (*this, This_SerialArray);
            else
               Array_Set = doubleArray::Parallel_Conformability_Enforcement (*this, This_SerialArray);
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->Array_Descriptor.SerialArray;

          if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE)
               Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement
                    (*this, This_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
            else
               Array_Set = doubleArray::Parallel_Conformability_Enforcement
                                (*this, This_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
          Where_Statement_Support::Where_Statement_Mask->Array_Descriptor.SerialArray = Mask_SerialArray;
        }

  // if (Array_Set == Null)
  //      Array_Set = new Array_Conformability_Info_Type();

  // APP_ASSERT(Array_Set != NULL);
     APP_ASSERT(This_SerialArray != NULL);

     *This_SerialArray = x;

     if (Where_Statement_Support::Where_Statement_Mask != NULL)
          Where_Statement_Support::Where_Statement_Mask->Array_Descriptor.SerialArray = Old_Mask_SerialArray;

#if defined(PPP) && defined(USE_PADRE)
     setLocalDomainInPADRE_Descriptor(NULL);
#endif

     APP_ASSERT (This_SerialArray->getReferenceCount() >= doubleArray::getReferenceCountBase());
     This_SerialArray->decrementReferenceCount();
     if (This_SerialArray->getReferenceCount() < doubleArray::getReferenceCountBase())
          delete This_SerialArray;
 
     if (Mask_SerialArray != NULL)
        {
#if defined(PPP) && defined(USE_PADRE)
          setLocalDomainInPADRE_Descriptor(NULL);
#endif
          APP_ASSERT (Mask_SerialArray->getReferenceCount() >= intArray::getReferenceCountBase());
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intArray::getReferenceCountBase())
               delete Mask_SerialArray;
        }

     This_SerialArray = NULL;
     Mask_SerialArray = NULL;

#if 1
     if (Communication_Manager::Number_Of_Processors > 1)
        {
          if (Array_Set != NULL)
             {
               if (Array_Set->Full_VSG_Update_Required == FALSE)
                  {
                    int update_any = FALSE;
                    int i;
                    for (i=0; i < MAX_ARRAY_DIMENSION; i++)
                         if ((Array_Set->Update_Left_Ghost_Boundary_Width [i] >0) ||
                             (Array_Set->Update_Right_Ghost_Boundary_Width[i] >0) )
                              update_any = TRUE;
                    if (update_any)
                       {
#if COMPILE_DEBUG_STATEMENTS
                         for (i =0; i < MAX_ARRAY_DIMENSION; i++)
                            {
                              if ((Array_Set->Update_Left_Ghost_Boundary_Width [i] >0) ||
                                  (Array_Set->Update_Right_Ghost_Boundary_Width[i] >0) )
                                 {
                                   if (Array_Descriptor.Array_Domain.InternalGhostCellWidth[i] == 0)
                                      {
                                        printf("ERROR: No ghost cells along axis %d to ", i);
                                        printf("support message passing \n");
                                        APP_ABORT();
                                      }
                                 }
                              if (Array_Descriptor.isLeftPartition(i))
                                 {
                                   if (Array_Set->Update_Left_Ghost_Boundary_Width [i] >0) 
                                      {
                                        printf("ERROR: Left processor shouldn't try to update ");
                                        printf("left ghost boundary on dim %d.\n",i);
                                      }
                                   APP_ASSERT (Array_Set->Update_Left_Ghost_Boundary_Width[i] == 0);
                                 }
                              if (Array_Descriptor.isRightPartition(i))
                                 {
                                   if (Array_Set->Update_Right_Ghost_Boundary_Width [i] >0) 
                                      {
                                        printf("ERROR: Right processor shouldn't try to update ");
                                        printf("right ghost boundary on dim %d.\n",i);
                                      }
                                   APP_ASSERT (Array_Set->Update_Right_Ghost_Boundary_Width[i] == 0);
                                 }
                            }
#endif
                         if (Array_Descriptor.Array_Domain.Array_Conformability_Info != NULL)
                            {
#if COMPILE_DEBUG_STATEMENTS
                              if (APP_DEBUG > 3)
                                 {
                                   printf ("Inside of operator=(scalar): Array_Conformability_Info->getReferenceCount() = %d \n",
                                        Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount());
                                 }
#endif
                              APP_ASSERT (Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount() >= 
                                   Array_Conformability_Info_Type::getReferenceCountBase());
                              Array_Descriptor.Array_Domain.Array_Conformability_Info->decrementReferenceCount();
                              if (Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount() <
                                  Array_Conformability_Info_Type::getReferenceCountBase())
                                 {
                                   delete Array_Descriptor.Array_Domain.Array_Conformability_Info;
                                 }
                            }
                         Array_Descriptor.Array_Domain.Array_Conformability_Info = Array_Set;

                      // Test added (11/26/2000)
                         APP_ASSERT (Array_Descriptor.Array_Domain.Array_Conformability_Info != NULL);

                      // We should modify this so that the ghost boundaries are set properly without communication
                         if( automaticCommunication ) updateGhostBoundaries();
                         Array_Descriptor.Array_Domain.Array_Conformability_Info = NULL;
                       }
                  }
                 else
                  {
                 // Full VSG_Update is required 
                    if( automaticCommunication ) updateGhostBoundaries();
                  }
             }
            else
             {
            // Array_Set is null because indirect addressing is used;
               if( automaticCommunication ) updateGhostBoundaries();
             }
        } // end of (Number_Of_Processors > 1)
#endif

     if (Array_Set != NULL)
        {
#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 3)
             {
               printf ("Inside of ~Array_Domain_Type: Array_Set->getReferenceCount() = %d \n",
                    Array_Set->getReferenceCount());
             }
#endif
          APP_ASSERT (Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
          Array_Set->decrementReferenceCount();
          if (Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
               delete Array_Set;
          Array_Set = NULL;
        }

#if COMPILE_DEBUG_STATEMENTS
  // APP_ASSERT(Array_Descriptor.Array_Domain.Is_A_Temporary == FALSE);
     Test_Consistency ("Test (on return) in doubleArray::operator= (double x)");
#endif

#if USE_TEMPORARY_DELETE_FUNCTIONS
  // This would be unlikely but we have to make sure
     Delete_Lhs_If_Temporary ( *this );
#endif

     return *this;
   }

// **********************************************************
// The other equals operator taking an array object
// **********************************************************
doubleArray &
doubleArray::operator= ( const doubleArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if ( (APP_DEBUG > 1) || (Diagnostic_Manager::getReferenceCountingReport() > 0) )
        {
          printf ("\n\n\n@@@@@ Inside of doubleArray::operator=(const doubleArray & Rhs) (id=%d) = (id=%d) \n",Array_ID(),Rhs.Array_ID());
          this->displayReferenceCounts("Lhs in doubleArray & operator=(doubleArray,doubleArray)");
          Rhs.displayReferenceCounts("Rhs in doubleArray & operator=(doubleArray,doubleArray)");
       }

  // printf ("In doubleArray::operator=(Rhs): Rhs.getReferenceCount() = %d \n",Rhs.getReferenceCount());
  // printf ("In doubleArray::operator=(Rhs): Rhs.Array_Descriptor.SerialArray->getReferenceCount() = %d \n",
  //      Rhs.Array_Descriptor.SerialArray->getReferenceCount());

  // Temp code to debug reference count problem!
  // Rhs.view ("view Rhs in doubleArray::operator= (const doubleArray & Rhs)");

  // This is the only test we can do on the input!
  // printf ("Test_Consistency the input array objects \n");
     Test_Consistency ("Test Lhs in doubleArray::operator= (const doubleArray & Rhs)");
     Rhs.Test_Consistency ("Test Rhs in doubleArray::operator= (const doubleArray & Rhs)");

#if 0
#if COMPILE_DEBUG_STATEMENTS
     if (Communication_Manager::localProcessNumber() == 0)
        {
          Array_Descriptor.Array_Domain.display("Lhs in operator=");
        }
#endif
#endif

     if (APP_DEBUG > 3)
        {
          printf ("View the input array objects \n");
          view ("view Lhs in doubleArray::operator= (const doubleArray & Rhs)");
          Rhs.view ("view Rhs in doubleArray::operator= (const doubleArray & Rhs)");
       // printf ("Exiting at TOP of doubleArray::operator= ( const doubleArray & Rhs ) ... \n");
       // APP_ABORT();
        }
#endif

     if (Index::Index_Bounds_Checking)
          Test_Conformability (Rhs);

  // view("THIS inside of operator=");
  // Rhs.view("Rhs inside of operator=");

     doubleSerialArray** Lhs_SerialArray_Pointer = &Array_Descriptor.SerialArray;
     doubleSerialArray** Rhs_SerialArray_Pointer = &(((doubleArray &)Rhs).Array_Descriptor.SerialArray);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          this->displayReferenceCounts("THIS in doubleArray::operator= (doubleArray)");
          Rhs.displayReferenceCounts("Rhs in doubleArray::operator= (doubleArray)");
        }
#endif

  // reorder these tests for better efficiency (can it be improved?)
  // if ( (Array_Descriptor.Array_Reference_Count_Array[Array_Descriptor.Array_ID()] == 0) &&
  //       Binary_Conformable (Rhs) &&
  //       Rhs.Array_Descriptor.Array_Domain.Is_A_Temporary &&
  //      !Rhs.Array_Descriptor.Array_Domain.Is_A_View )
  /* bool Case_1 = (Array_Descriptor.Array_Reference_Count_Array[Array_Descriptor.Array_ID()] == 0) && */
  // ... (5/8/97, kdb) make sure this isn't a view also ...
     bool Case_1 = (getRawDataReferenceCount() == getRawDataReferenceCountBase()) &&
                       !isView() &&
                       Binary_Conformable (Rhs) &&
                       Rhs.isTemporary() &&
                      !Rhs.isView();
  // this case is specific to P++ because temporaries are not condensed in the assignment to the
  // Lhs if that Lhs is a Null Array.  This is an internal difference between A++ and P++.
  // It is done to permit the distribution to be identical for the Lhs instead of being remapped.
  // ... (5/30/98, kdb) this cann't be a null array and a view without problems ...
     bool Case_2 = (getRawDataReferenceCount() == getRawDataReferenceCountBase()) && 
                       Rhs.isTemporary() && isNullArray() && !isView(); 
     if (Case_1 || Case_2)
        {
       // Give back the original Array_Data memory (but only if it exists)!
       // But it should always exist so we don't really have the check!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
               printf ("Skip the assignment by stealing the data (delete the Lhs's data if not a NullArray)! \n");
#endif

       // Handle the case of assignment to a NULL array (i.e. undimensioned array object
       // or an array object dimensioned to length zero).
          if (isNullArray())
             {
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Lhs is a Null Array so copy descriptor before assignment! \n");
#endif

            // Bug Fix (8/17/94) Null array must have a properly computed array descriptor
            // APP_ASSERT( Array_Descriptor != NULL );
            // Array_Descriptor.ReferenceCountedDelete();
            // delete Array_Descriptor;

            // We can't steal the descriptor because the descriptor contains the Array_ID and it
            // would have to be different for each of the Lhs and Rhs array objects.
            // The Array_ID should be in the array objects.
               Array_Descriptor.Build_Temporary_By_Example ( Rhs.Array_Descriptor ); 
               Array_Descriptor.Array_Domain.Is_A_Temporary = FALSE;
             }
            else
             {
            // Bug fix for nonzero Lhs base -- the Rhs is a temporary and so it has a zero base and the A++
            // assignment is never called so the serial array we will substitute has zero base.  
            // So we have to fix the base of the rhs we will steal and place on the Lhs.
	    // ... (9/27/96, kdb) other values need to be set correctly also ...
            // BTNG APP_ASSERT ( MAX_ARRAY_DIMENSION == 4);
	       int temp;
               for (temp=0; temp < MAX_ARRAY_DIMENSION; temp++)
                  {
                    Rhs.Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain.Data_Base[temp] = 
                         Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain.Data_Base[temp];
                    Rhs.Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain.User_Base[temp] = 
                         Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain.User_Base[temp];
                    Rhs.Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain.Scalar_Offset[temp] = 
                         Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain.Scalar_Offset[temp];
                  }
             }

       // Why is this a warning!
          APP_ASSERT ( getRawDataReferenceCount() == getRawDataReferenceCountBase() );
          if ( getRawDataReferenceCount() > getRawDataReferenceCountBase() )
             {
               printf ("WARNING: Array_ID = %d getRawDataReferenceCount() = %d \n",
                    Array_Descriptor.Array_ID(),getRawDataReferenceCount() );
             }
          APP_ASSERT(getRawDataReferenceCount() == getRawDataReferenceCountBase());

       // printf ("Before Delete_Array_Data -- Skip in doubleArray::operator=(Rhs): Rhs.getReferenceCount() = %d \n",Rhs.getReferenceCount());
       // printf ("Before Delete_Array_Data -- Skip in doubleArray::operator=(Rhs): Rhs.Array_Descriptor.SerialArray->getReferenceCount() = %d \n",
       //      Rhs.Array_Descriptor.SerialArray->getReferenceCount());

       // view ("View of THIS from operator=");
          Delete_Array_Data ();
          *Lhs_SerialArray_Pointer = NULL;

       // APP_ASSERT(Array_Descriptor.Array_Domain.Array_Reference_Count_Array[Array_Descriptor.Array_ID()] == 
       //   doubleArray_Descriptor_Type
       //   ::Array_Reference_Count_Array[Array_Descriptor.Array_ID()] );

       // Since we stold the data we can zero the reference count (part of breaking the reference)
       // APP_ASSERT( doubleArray_Descriptor_Type::Array_Reference_Count_Array[Array_Descriptor.Array_ID()] >= 0 );
       // if ( doubleArray_Descriptor_Type::Array_Reference_Count_Array[Array_Descriptor.Array_ID()] >= 0 )
       // Array_Descriptor.Array_Reference_Count_Array[Array_Descriptor.Array_ID()] = 0;

          APP_ASSERT(*Lhs_SerialArray_Pointer == NULL);
       // Call the Serial_A++ assignment operator
          *Lhs_SerialArray_Pointer = *Rhs_SerialArray_Pointer;
          APP_ASSERT(*Lhs_SerialArray_Pointer != NULL);
          (*Lhs_SerialArray_Pointer)->Array_Descriptor.Array_Domain.Is_A_Temporary = FALSE;

       // increment the reference count so that after the Rhs is deleted we will still have the
       // serial array around on the Lhs.
          incrementRawDataReferenceCount();
          APP_ASSERT( getRawDataReferenceCount() > getRawDataReferenceCountBase());

#if 0
          printf ("After incrementRawDataReferenceCount() -- Skip in doubleArray::operator=(Rhs): Rhs.getReferenceCount() = %d \n",Rhs.getReferenceCount());
          printf ("After incrementRawDataReferenceCount() -- Skip in doubleArray::operator=(Rhs): Rhs.Array_Descriptor.SerialArray->getReferenceCount() = %d \n",Rhs.Array_Descriptor.SerialArray->getReferenceCount());
          printf ("After incrementRawDataReferenceCount() -- Skip in doubleArray::operator=(Rhs): Rhs.getRawDataReferenceCount() = %d \n",Rhs.getRawDataReferenceCount());
#endif

       // We want the Rhs to have a serial array when the Rhs is deleted as a temporary
       // if the reference counts are greater then 1 then the Lhs will be left with the data
       // after the Rhs is deleted!
       // *Rhs_SerialArray_Pointer = NULL;

       // Bugfix (12/19/94) Force base of SerialArray equal to the value in the P++ descriptor
          APP_ASSERT(Array_Descriptor.SerialArray != NULL);

       // (4/30/97,kdb) Reinitialize the Serial Array_View_Pointers because 
       // SerialArray has been copied from Rhs

	  SERIAL_POINTER_LIST_INITIALIZATION_MACRO;
        }
       else 
        {
       // Do the assignment the hard way (not so hard 
       // since we just call the A++ assignment operator)
#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
               printf ("Do the assignment by calling the Serial_A++ assignment function! \n");
#endif

       // The more complex logic contained in the Serial_A++ implementation can be skipped
       // since we are just calling the Serial_A++ assignment operator and so it is included
       // this means that cases such as A(I) = A(I+1) are properly handled
          APP_ASSERT(Array_Descriptor.SerialArray != NULL);
          APP_ASSERT(Rhs.Array_Descriptor.SerialArray != NULL);

       // We want to allow NULL operations to avoid building new Null arrays for the Lhs
       // So don't allow the Lhs to be rebuilt if the Rhs is a Null array!
       // if (Array_Descriptor.Array_Domain.Is_A_Null_Array)
       // if (Array_Descriptor.Array_Domain.Is_A_Null_Array && !Rhs.Array_Descriptor.Array_Domain.Is_A_Null_Array)
          if (isNullArray() && !Rhs.isNullArray())
             {
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Lhs is a Null Array! \n");
#endif

            // printf ("################################################################## \n");
            // printf ("Case of Lhs being a NULL ARRAY and Rhs is a VIEW -- commented out! \n");
            // printf ("################################################################## \n");
            // The problem here is that the view Lhs is not being build to be the correct size
            // to work well in the parallel environment.  The lhs in this case should be built
            // to be the size of the rhs array data assuming no view.  I.e. the lhs should be
            // the same size as what the rhs is a view of so that the partitioning of the 
            // lhs and the rhs will be alligned (when we get to the serial operator= below).
               bool HANDLE_CASE_OF_VIEW = FALSE;
               if ( Rhs.isView() && HANDLE_CASE_OF_VIEW )
                  {
                 // Force Lhs to be the size of the referenced part of the Rhs
                 // This implies a default partitioning when used with P++.
                 // printf ("Rhs.Array_Descriptor.Array_Domain.Is_A_View == TRUE -- Building a new descriptor from scratch! \n");
                 // int i = Rhs.Array_Descriptor.getLength(0);
                 // int j = Rhs.Array_Descriptor.getLength(1);
                 // int k = Rhs.Array_Descriptor.getLength(2);
                 // int l = Rhs.Array_Descriptor.getLength(3);

                    Integer_Array_MAX_ARRAY_DIMENSION_Type Integer_List;
		    int temp;
                    for (temp=0; temp < MAX_ARRAY_DIMENSION; temp++)
                         Integer_List[temp] = Rhs.Array_Descriptor.getLength(temp);

                 // This is NOT EFFICIENT -- but I gather that this case does not happen
                 // sense HANDLE_CASE_OF_VIEW is FALSE!
                 // Array_Descriptor = new doubleArray_Descriptor_Type(i,j,k,l);
                 // Array_Descriptor = 
                 //  new doubleArray_Descriptor_Type (Integer_List);
                    Array_Descriptor = doubleArray_Descriptor_Type
		                          (MAX_ARRAY_DIMENSION,Integer_List);

                    Array_Descriptor.display("Exiting in case of null Lhs of operator= ");
                    printf ("Exiting in case of null Lhs of operator= \n");
                    APP_ABORT();
                  }
                 else
                  {
                 // This is just calling the Array_Descriptor_Type's copy constructor so we have to
                 // modify the descriptor in the case it is a view.
                 // printf ("Rhs.Array_Descriptor.Array_Domain.Is_A_View == FALSE -- Building a copy of the Rhs descriptor! \n");

                 // If the Rhs is a Null Array then we should never have gotten to this point in the code!
                    APP_ASSERT( Rhs.isNullArray() == FALSE );

                 // This copies the domain in Rhs into the domain in the Lhs except that
                 // subsequently the bases are set to the default A++/P++ base (typically zero).
                    Array_Descriptor.Build_Temporary_By_Example ( Rhs.Array_Descriptor );

                 // ... change (10/29/96,kdb) set bases to Rhs bases because 
                 // Build_Temporary_By_Example sets the bases to zero 
                 // after copying Rhs and in the case of a null Lhs we now 
                 // want the bases of Lhs and Rhs to be the same ...

                 // ... set bases to Rhs bases ...
                 // Now we reset the base to be that of the Rhs (perhaps we should have just copied the
                 // domain explicitly -- since we copied the Rhs domain reset the base and then set the
                 // base again).
	            int nd = 0; 
                    for (nd=0;nd<MAX_ARRAY_DIMENSION;nd++)                    
		         setBase(Rhs.Array_Descriptor.getRawBase(nd),nd); 

                    setTemporary(FALSE);

#if COMPILE_DEBUG_STATEMENTS
                    if (APP_DEBUG > 5)
                       {
                         Rhs.Array_Descriptor.display("In operator= display (Rhs.Array_Descriptor)");
                         Array_Descriptor.display("In operator= display (Array_Descriptor)");
                       }
#endif

                    if (Array_Descriptor.Array_Domain.Array_Conformability_Info != NULL)
                       {
#if COMPILE_DEBUG_STATEMENTS
                         if (APP_DEBUG > 3)
                            {
                              printf ("Inside of operator=(Rhs): Array_Conformability_Info->getReferenceCount() = %d \n",
                                   Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount());
                            }
#endif
                         APP_ASSERT (Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount() >= 
                                     Array_Conformability_Info_Type::getReferenceCountBase());
                         Array_Descriptor.Array_Domain.Array_Conformability_Info->decrementReferenceCount();
                         if (Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount() 
                              < Array_Conformability_Info_Type::getReferenceCountBase())
                            {
                              delete Array_Descriptor.Array_Domain.Array_Conformability_Info;
                            }
                         Array_Descriptor.Array_Domain.Array_Conformability_Info = NULL;
                       }

                    APP_ASSERT( Array_Descriptor.Array_Domain.Array_Conformability_Info == NULL );
                 // Array_Descriptor.display ("CASE OF NULL LHS OF OPERATOR=");
                  }

               if (HANDLE_CASE_OF_VIEW)
                  {
                    APP_ASSERT( isView() == FALSE );
                  }

#if 0
#if !defined(USE_PADRE)
            // attempt at bug fix (6/10/2000)
               APP_ASSERT (Array_Descriptor.Array_Domain.BlockPartiArrayDomain == NULL);
               APP_ASSERT (Array_Descriptor.Array_Domain.BlockPartiArrayDecomposition == NULL);
#endif
#endif

            // Now allocate the data for the Lhs (so we can copy the Rhs into the Lhs)
            // printf ("Allocating data for local array on Lhs (Lhs was a null array)! \n");
               Allocate_Array_Data(TRUE);

            // printf ("Exiting inside of operator= \n");
            // APP_ABORT();
             }

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
               printf ("Now call the Serial_A++ assignment operator! \n");
       // Communication_Manager::Sync();
#endif

          APP_ASSERT(isTemporary() == FALSE);
          APP_ASSERT(Array_Descriptor.SerialArray != NULL);
          APP_ASSERT(Array_Descriptor.SerialArray->isTemporary() == FALSE);

       // Figure out the parallel communication required and form the conformable 
       // views of the SerialArray objects then call the SerialArray assignment 
       // operator.

          doubleSerialArray  *This_SerialArray     = NULL;
          doubleSerialArray  *Rhs_SerialArray      = NULL;
	  intSerialArray *Mask_SerialArray     = NULL;
	  intSerialArray *Old_Mask_SerialArray = NULL;

          Array_Conformability_Info_Type *Array_Set = NULL;
          if (Where_Statement_Support::Where_Statement_Mask == NULL)
             {
            // printf ("No Where Mask: Calling doubleArray::Parallel_Indirect_Conformability_Enforcement \n");
            // printf ("Exiting BEFORE doubleArray::Parallel_Conformability_Enforcement in operator= \n");
            // APP_ABORT();

               if ((usesIndirectAddressing() == TRUE) || (Rhs.usesIndirectAddressing() == TRUE))
                  {
                    Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( *this, This_SerialArray, Rhs, Rhs_SerialArray );
                  }
                 else
                  {
                    Array_Set = doubleArray::Parallel_Conformability_Enforcement ( *this, This_SerialArray, Rhs, Rhs_SerialArray );
                  }
             }
            else
             {
            // ... (4/10/97, kdb) save old serial where mask because
            // Serial_Where_Statement_Mask has to be temporarily 
            // reset to make where work ...

               Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->Array_Descriptor.SerialArray;

               if ((usesIndirectAddressing() == TRUE) || (Rhs.usesIndirectAddressing() == TRUE))
                  {
                    printf ("Sorry, not implemented: can't mix indirect addressing (case of where and 2 array objects).\n");
                    APP_ABORT();
                  }
                 else
                  {
                    Array_Set = doubleArray::Parallel_Conformability_Enforcement
                         (*this, This_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray, Rhs, Rhs_SerialArray );
                  }

            // ...  this will be reset later ...
               Where_Statement_Support::Where_Statement_Mask->Array_Descriptor.SerialArray = Mask_SerialArray; 
             }

       // printf ("DONE: Check Where_Statement_Support::Where_Statement_Mask \n");

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 5)
             {
               Rhs_SerialArray->Array_Descriptor.display("In operator= AFTER ASSIGNMENT Rhs_SerialArray");
               This_SerialArray->Array_Descriptor.display("In operator= AFTER ASSIGNMENT This_SerialArray");
             }
#endif

       // printf ("Check Array_Set \n");

          if (Array_Set == NULL)
               Array_Set = new Array_Conformability_Info_Type();

       // Communication_Manager::Sync();
          APP_ASSERT(Array_Set        != NULL);
          APP_ASSERT(This_SerialArray != NULL);
          APP_ASSERT(Rhs_SerialArray  != NULL);

       // APP_ASSERT(Rhs_SerialArray->Array_Descriptor != NULL);

       // Call the A++ assignment operator!
#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("Call the A++ assignment operator from the P++ assignment operator ... \n");
            // This_SerialArray->view("In doubleArray::operator= This_SerialArray");
            // Rhs_SerialArray ->view("In doubleArray::operator= Rhs_SerialArray");
             }
#endif

          *This_SerialArray = *Rhs_SerialArray;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("DONE: Call the A++ assignment operator from the P++ assignment operator ... \n");
               this->displayReferenceCounts("After serial array assignment Lhs in doubleArray & operator=(doubleArray,doubleArray)");
               This_SerialArray->displayReferenceCounts("After serial array assignment This_SerialArray");
               Rhs_SerialArray->displayReferenceCounts("After serial array assignment Rhs_SerialArray");
             }
#endif

       // Replace the where mask
	  if (Where_Statement_Support::Where_Statement_Mask != NULL)
               Where_Statement_Support::Where_Statement_Mask->Array_Descriptor.SerialArray = Old_Mask_SerialArray;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 6)
             {
               Rhs_SerialArray->view("In operator= AFTER ASSIGNMENT Rhs_SerialArray");
               This_SerialArray->view("In operator= AFTER ASSIGNMENT This_SerialArray");
               view("In operator= AFTER ASSIGNMENT *this");
             }
#endif

       // Bugfix (6/15/95) Memory in use reported by Markus (a similar problem 
       // should exist in all the binary operaotrs (so we will fix that next))
       // Now delete the This_SerialArray and Rhs_SerialArray obtained from
       // the doubleArray::Parallel_Conformability_Enforcement

#if defined(PPP) && defined(USE_PADRE)
       // We have to remove the references in PADRE to the Serial_Array object
       // which is being deleted.  This is a consequence of P++ using PADRE in a way
       // so as to prevent the redundent storage of Array_Domain objects
       // (specifically we use PADRE in a way so that only references are stored).
          setLocalDomainInPADRE_Descriptor(NULL);
          Rhs.setLocalDomainInPADRE_Descriptor(NULL);
#endif

       // printf ("Now delete the This_SerialArray in the P++ operator= (doubleArray) \n");

          This_SerialArray->decrementReferenceCount();
          if (This_SerialArray->getReferenceCount() < doubleArray::getReferenceCountBase())
               delete This_SerialArray;

       // printf ("Now delete the Rhs_SerialArray in the P++ operator= (doubleArray) \n");

       // (12/09/2000) Check for reuse of serialArray object in return value (do not delete it if it was reused)
          if (Rhs_SerialArray->isTemporary() == FALSE)
             {
               Rhs_SerialArray->decrementReferenceCount();
               if (Rhs_SerialArray->getReferenceCount() < doubleArray::getReferenceCountBase())
                    delete Rhs_SerialArray;
             }

       // printf ("Now delete the Mask_SerialArray in the P++ operator= (doubleArray) \n");

	  if (Mask_SerialArray != NULL) 
             {
#if defined(PPP) && defined(USE_PADRE)
               Where_Statement_Support::Where_Statement_Mask->setLocalDomainInPADRE_Descriptor(NULL);
#endif
               Mask_SerialArray->decrementReferenceCount();
               if (Mask_SerialArray->getReferenceCount() < intArray::getReferenceCountBase())
                    delete Mask_SerialArray;
             }

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 5)
             {
               view("In operator= AFTER ASSIGNMENT (AND CALL TO DELETE) *this");
             }
#endif
       // Now set these pointers to NULL since we don't use them any more
          This_SerialArray = NULL;
          Rhs_SerialArray  = NULL;
	  Mask_SerialArray = NULL;

       // printf ("Now do whatever message passing that is required in P++ operator= (doubleArray) \n");

       // Now we need to do the required message passing to fixup the "this" array
       // first check to see if just updating the ghost boundaries is enough

          if (Communication_Manager::Number_Of_Processors > 1)
             {
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
	          {
                    printf ("All ghost boundaries updated together (not seperate treatment for each edge) \n");
                  }
#endif
               if (Array_Set != NULL)
	          {
                    if (Array_Set->Full_VSG_Update_Required == FALSE)
                       {
                         int update_any = FALSE;
                         int i;
                         for (i=0; i < MAX_ARRAY_DIMENSION; i++)
                            {
                              if ((Array_Set->Update_Left_Ghost_Boundary_Width [i] > 0) || 
		                  (Array_Set->Update_Right_Ghost_Boundary_Width[i] > 0)) 
	                           update_any = TRUE;		 
                            }

                         if (update_any)
                            {
#if COMPILE_DEBUG_STATEMENTS
                              if (APP_DEBUG > 0)
                                   printf ("UPDATING GHOST BOUNDARIES (Overlap Update)! \n");

                           // error checking
                              for (i=0; i < MAX_ARRAY_DIMENSION; i++)
                                 {
                                   if ((Array_Set->Update_Left_Ghost_Boundary_Width[i] > 0) || 
                                       (Array_Set->Update_Right_Ghost_Boundary_Width[i] > 0))
                                      {
                                        if (Array_Descriptor.Array_Domain.InternalGhostCellWidth[i] == 0)
                                           {
                                             printf ("ERROR: No ghost cells along axis %d to ",i);
                                             printf ("support message passing ");
                                             printf ("(Full_VSG_Update_Required should be TRUE ");
                                             printf ("but it is FALSE) \n");
                                             APP_ABORT();
                                           }
                                      }

                                // If we are the leftmost processor then it is an error to 
                                // pass boundary info the the left
                                   if (Array_Descriptor.isLeftPartition(i))
                                      {
                                        if (Array_Set->Update_Left_Ghost_Boundary_Width[i] != 0)
                                           {
                                             printf ("ERROR: Left side -- Axis i = %d \n",i);
                                             Array_Set->display("ERROR: Left processor should not pass left -- Array_Set->Update_Left_Ghost_Boundary_Width[i] == 0");
                                             view ("ERROR: Left processor should not pass left -- Array_Set->Update_Left_Ghost_Boundary_Width[i] == 0");
                                           }
                                        APP_ASSERT (Array_Set->Update_Left_Ghost_Boundary_Width[i] == 0);
                                      }

                                // If we are the rightmost processor then it is an error to 
                                // pass boundary info the the right
                                   if (Array_Descriptor.isRightPartition(i))
                                      {
#if COMPILE_DEBUG_STATEMENTS
                                        if (Array_Set->Update_Right_Ghost_Boundary_Width[i] != 0)
                                           {
                                             printf ("ERROR: Right side -- Axis i = %d \n",i);
                                             Array_Set->display("ERROR: Right processor should not pass right -- Array_Set->Update_Right_Ghost_Boundary_Width[i] == 0");
                                             view ("ERROR: Right processor should not pass right -- Array_Set->Update_Right_Ghost_Boundary_Width[i] == 0");
                                           }
#endif
                                        APP_ASSERT (Array_Set->Update_Right_Ghost_Boundary_Width[i] == 0);
                                      }
                                 }
#endif
                           // For now we update all ghost boundaries but later we can be 
                           // more efficient and selective
                           /*
                           // ... (12/11/96,kdb) now only the necessary ghost cells
                           // are updated.  The updateGhostBoundaries code needs
                           // Array_Set to do this.  To avoid passing in an
                           // extra parameter, temporarily attach this to
                           // Array_Descriptor since the Array_Conformability_Info
                           // will be deleted anyways right after this. ...
                           */
                              if (Array_Descriptor.Array_Domain.Array_Conformability_Info != NULL)
                                 {
                                   Array_Descriptor.Array_Domain.Array_Conformability_Info->decrementReferenceCount();
                                   if (Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount() < 
                                       intArray::getReferenceCountBase())
                                        delete Array_Descriptor.Array_Domain.Array_Conformability_Info;
                                 }

                           // ... don't need reference counting ...
                              Array_Descriptor.Array_Domain.Array_Conformability_Info = Array_Set;
                              if( automaticCommunication ) updateGhostBoundaries();
                              Array_Descriptor.Array_Domain.Array_Conformability_Info = NULL;
                            }
                           else
                            {
                           // No updates specified 
#if COMPILE_DEBUG_STATEMENTS
                              if (APP_DEBUG > 0)
                                   printf ("NO UPDATE TO GHOST BOUNDARIES REQUIRED! \n");
#endif
                            }
                       }
                      else
                       {
                      // Full_VSG_Update_Required == TRUE

#if COMPILE_DEBUG_STATEMENTS
                         if (APP_DEBUG > 0)
                            {
                              printf ("FULL VSG update required (already done) (but now we ");
                              printf ("have to update the ghost boundaries) \n");
                            }
#endif
                      // Since the Regular Section transfer used in PARTI does not update
                      // the ghost boundaries we have to force the update here!
                         if( automaticCommunication ) updateGhostBoundaries();
                       }
                  }
                 else
                  {
                 // Array_Set is null because indirect addressing was used. 
                 // Update ghost boundaries.
                 // printf ("Calling updateGhostBoundaries() for 1 processor \n");
                    if( automaticCommunication ) updateGhostBoundaries();
                  }
             } // end of (Communication_Manager::Number_Of_Processors > 1)

       // printf ("DONE with communication for operator= \n");

       // Need to delete the Array_Set to avoid a memory leak
       // Array_Set can be null now if indirect addressing
       // APP_ASSERT (Array_Set != NULL);
          if (Array_Set != NULL);
	     {
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 3)
                  {
                    printf ("Inside of operator=(Rhs): Array_Set->getReferenceCount() = %d \n",
                         Array_Set->getReferenceCount());
                  }
#endif

#if 1
               APP_ASSERT (Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
               Array_Set->decrementReferenceCount();
               if (Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
                    delete Array_Set;
               Array_Set = NULL;
#endif
	     }
        }

#if COMPILE_DEBUG_STATEMENTS
     APP_ASSERT(Array_Descriptor.Array_Domain.Is_A_Temporary == FALSE);
  // printf ("Calling Test_Consistency for Lhs \n");
     Test_Consistency("Test (on return) in doubleArray::operator= (const doubleArray & Rhs)");
#endif

#if USE_TEMPORARY_DELETE_FUNCTIONS
  // Manage temporary if it appears on the Rhs or Lhs
  // printf ("In operator= (before Delete_If_Temporary): Rhs.getReferenceCount() = %d \n",Rhs.getReferenceCount());
  // printf ("In operator= (before Delete_If_Temporary): Rhs.Array_Descriptor.SerialArray->getReferenceCount() = %d \n",
  //      Rhs.Array_Descriptor.SerialArray->getReferenceCount());
     Delete_If_Temporary ( Rhs );

  // printf ("In doubleArray::operator= Array_ID on the stack is %d \n",SerialArray_Domain_Type::queryNextArrayID());

  // printf ("In operator= (before Delete_Lhs_If_Temporary): Lhs.getReferenceCount() = %d \n",this->getReferenceCount());
     Delete_Lhs_If_Temporary ( *this );
#endif

     if (Array_Descriptor.Array_Domain.Array_Conformability_Info != NULL)
        {
          Array_Descriptor.Array_Domain.Array_Conformability_Info->decrementReferenceCount();
          if (Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount() <
              Array_Conformability_Info_Type::getReferenceCountBase())
               delete Array_Descriptor.Array_Domain.Array_Conformability_Info;
          Array_Descriptor.Array_Domain.Array_Conformability_Info = NULL;
        }

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          this->displayReferenceCounts("THIS at BASE of doubleArray::operator= (doubleArray)");
        }
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Return from doubleArray::operator= \n");

  // Commented out (12/14/2000)
  // Communication_Manager::Sync();
#endif

     return *this;
   }

// ************************************************************************************
// This is the abstract operator used for reduction operators
// ************************************************************************************
double
doubleArray::Abstract_Operator (
   Array_Conformability_Info_Type *Array_Set, const doubleArray & X, 
   doubleSerialArray* X_Serial_PCE_Array, double x, int Operation_Type )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of #1 doubleArray::Abstract_Operator(Array_Conformability_Info_Type,doubleArray,double,int) \n");

  // This is the only test we can do on the input!
     X.Test_Consistency ("Test in #1 doubleArray::Abstract_Operator(Array_Conformability_Info_Type,doubleArray,double,int)");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X at TOP of #1 doubleArray::Abstract_Operator()");
       // X_Serial_PCE_Array->displayReferenceCounts("X_Serial_PCE_Array at TOP of #1 doubleArray::Abstract_Operator()");
        }
#endif

  // error checking
     APP_ASSERT(Array_Set != NULL);

  // We have to do a communication on the group of procesors owning 
  // this P++ array object we skip the the communication for now.

#if defined(USE_PADRE)
  // What PADRE function should we call?
  // printf ("NEED TO CALL PADRE \n"); APP_ABORT();
     APP_ASSERT (X.Array_Descriptor.Array_Domain.parallelPADRE_DescriptorPointer != NULL);
#else
     APP_ASSERT (X.Array_Descriptor.Array_Domain.BlockPartiArrayDecomposition != NULL);
#endif
  // The local reduction has already been done and passed in as the scalar "x" so we only have
  // further reduction to do if there are more than 1 processors in the multiprocesor system
     if ( Communication_Manager::Number_Of_Processors > 1 )
        {
          Reduction_Operation ( Operation_Type , x );
        }

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X at BOTTOM of #1 doubleArray::Abstract_Operator()");
        }
#endif

#if USE_TEMPORARY_DELETE_FUNCTIONS
     Delete_If_Temporary (X);
#endif

     return x;
   }
 

// ************************************************************************
// usual binary operators: operator+ operator- operator* operator/ operator% 
// for array to scalar operations
// ************************************************************************
doubleArray &
doubleArray::Abstract_Operator ( 
   Array_Conformability_Info_Type *Array_Set, 
   const doubleArray & X_ParallelArray, 
   doubleSerialArray* X_Serial_PCE_Array, 
   doubleSerialArray & X_SerialArray )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Inside of #2 doubleArray::Abstract_Operator(Array_Conformability_Info_Type,doubleArray,doubleSerialArray) \n");

  // This is the only test we can do on the input!
     X_ParallelArray.Test_Consistency ("Test X_ParallelArray in #2 doubleArray::Abstract_Operator(Array_Conformability_Info_Type,doubleArray,doubleSerialArray)");
     X_SerialArray.Test_Consistency   ("Test X_SerialArray in #2 doubleArray::Abstract_Operator(Array_Conformability_Info_Type,doubleArray,doubleSerialArray)");

  // X_ParallelArray.view("X_ParallelArray in #2 doubleArray::Abstract_Operator(Array_Conformability_Info_Type,doubleArray,doubleSerialArray)");
  // printf ("X_ParallelArray.Array_Descriptor.SerialArray->getBase(0) = %d \n",X_ParallelArray.Array_Descriptor.SerialArray->getBase(0));
  // printf ("X_ParallelArray.Array_Descriptor.SerialArray->getBase(1) = %d \n",X_ParallelArray.Array_Descriptor.SerialArray->getBase(1));
  // printf ("X_ParallelArray.Array_Descriptor.SerialArray->getBase(2) = %d \n",X_ParallelArray.Array_Descriptor.SerialArray->getBase(2));
  // printf ("X_ParallelArray.Array_Descriptor.SerialArray->getBase(3) = %d \n",X_ParallelArray.Array_Descriptor.SerialArray->getBase(3));
  // X_SerialArray.view("X_SerialArray in #2 doubleArray::Abstract_Operator(Array_Conformability_Info_Type,doubleArray,doubleSerialArray)");
#endif

  // error checking
     APP_ASSERT(Array_Set != NULL);

  // The Partitioning array is the array object (usually the Lhs operand of a binary operation) which the 
  // resulting temporary will be partitioned similar to (if it is required to build the temporary).
  // If a scalar was used on the Lhs then we have to use the Rhs as a Partitioning_doubleArray
  // doubleArray* Partitioning_Array = &((doubleArray &) X_ParallelArray);
  // APP_ASSERT (Partitioning_Array != NULL);

  // The size of the temporary must be the same as that of the Lhs (Lhs based owners compute rule)
     doubleSerialArray *Data = &((doubleSerialArray &) X_SerialArray);
     APP_ASSERT(Data != NULL);

  // Data->view ("view Data (as X_SerialArray AT TOP) in #2 doubleArray::Abstract_Operator(Array_Conformability_Info_Type,doubleArray,doubleSerialArray)");

  // The size of the temporary must be the same as that of the Lhs (Lhs based owners compute rule)
     doubleArray* Temporary = NULL;

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X_SerialArray.displayReferenceCounts("X_SerialArray at TOP of #2 doubleArray::Abstract_Operator()");
        }
#endif

     if ( X_ParallelArray.isTemporary() )
        {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Inside of #2 doubleArray::Abstract_Operator(): Reuse X_ParallelArray \n");
#endif

       // Temporary reuse of serial array already handled at Serial_A++ level
          Temporary = &((doubleArray &) X_ParallelArray);

       // Temporary->view ("view temporary (as X_ParallelArray BEFORE fixupLocalBase) in #2 doubleArray::Abstract_Operator(Array_Conformability_Info_Type,doubleArray,doubleSerialArray)");

          Array_Domain_Type::fixupLocalBase 
	     ( (Array_Domain_Type&)X_ParallelArray.Array_Descriptor.Array_Domain, 
	       Data->Array_Descriptor.Array_Domain , 
               X_ParallelArray.Array_Descriptor.Array_Domain , 
               X_ParallelArray.Array_Descriptor.SerialArray->
	       Array_Descriptor.Array_Domain );

       // Bugfix (12/3/2000) we have to place the X_SerialArray into the Temporary
          Delete_If_Temporary (*X_ParallelArray.Array_Descriptor.SerialArray);
          ((doubleArray &) X_ParallelArray).Array_Descriptor.SerialArray = Data;

       // Temporary->view ("view temporary (as X_ParallelArray BEFORE Modify_Reference_Counts_And_Manage_Temporaries) in #2 doubleArray::Abstract_Operator(Array_Conformability_Info_Type,doubleArray,doubleSerialArray)");

       // Modify_Reference_Counts_And_Manage_Temporaries ( Array_Set, Temporary, X_Serial_PCE_Array, Data );
          APP_ASSERT (Data->isTemporary() == TRUE);

#if COMPILE_DEBUG_STATEMENTS
          if (Diagnostic_Manager::getReferenceCountingReport() > 0)
             {
            // This mechanism outputs reports which allow us to trace the reference counts
               Temporary->displayReferenceCounts("Temporary (after Modify_Reference_Counts_And_Manage_Temporaries) of #2 doubleArray::Abstract_Operator()");
               X_SerialArray.displayReferenceCounts("X_SerialArray (after Modify_Reference_Counts_And_Manage_Temporaries) of #2 doubleArray::Abstract_Operator()");
             }
#endif

       // Temporary->view ("view temporary (as X_ParallelArray AFTER MRCAMT) in #2 doubleArray::Abstract_Operator(Array_Conformability_Info_Type,doubleArray,doubleSerialArray)");
       // APP_ABORT();

#if COMPILE_DEBUG_STATEMENTS
          X_ParallelArray.Test_Consistency ("Test X_ParallelArray (temporary) in #2 doubleArray::Abstract_Operator(Array_Conformability_Info_Type,doubleArray,doubleSerialArray)");
#endif
       // Data->view("Data (temporary) in #2 doubleArray::Abstract_Operator(Array_Conformability_Info_Type,doubleArray,doubleSerialArray)");
       // X_ParallelArray.view("X_ParallelArray in Abstract_Operator");
       // APP_ABORT();
        }
       else
        {
       // No temporary to reuse so we have to build one!

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Inside of #2 doubleArray::Abstract_Operator(): Build a new array to be the temporary \n");
#endif

       // This is more efficient than the previous version which build a descriptor and
       // then forced 2 copies of the descriptor before ending up with a temporary to use.
          bool AvoidBuildingIndirectAddressingView = TRUE;
       // Data->view("In abstract_op #2: Data");
       // X_ParallelArray.Array_Descriptor.Array_Domain.display("In abstract_op #2: X_ParallelArray: Array_Domain");
          Temporary = new doubleArray ( Data , 
                                    &(X_ParallelArray.Array_Descriptor.Array_Domain),
                                    AvoidBuildingIndirectAddressingView );
          APP_ASSERT (Temporary != NULL);
          APP_ASSERT (Temporary->Array_Descriptor.SerialArray != NULL);

       // printf ("In #2 doubleArray::Abstract_Operator: Temporary->Array_Descriptor.SerialArray->getReferenceCount() = %d \n",
       //      Temporary->Array_Descriptor.SerialArray->getReferenceCount());
       // APP_ASSERT (Temporary->Array_Descriptor.SerialArray->getReferenceCount() > 1);

       // The base now needs to be reset since temporaries always have APP_Global_Array_Base as a base
       // ... don't reset this if indirect addressing is used ...
	  if (!Temporary->Array_Descriptor.Array_Domain.Uses_Indirect_Addressing)
             Temporary->setBase(APP_Global_Array_Base);
          
       // Temporary->view ("view temporary in #2 doubleArray::Abstract_Operator(Array_Conformability_Info_Type,doubleArray,doubleSerialArray)");
       // APP_ABORT();

          Temporary->setTemporary(TRUE);

       /*
       // ... (4/28/98, kdb) added this function call, don't know what 
       //   happened to it ...
       // ... (4/29/98, kdb) not sure this is needed ...
       */

       /*
          Array_Domain_Type::fixupLocalBase
	     (Descriptor->Array_Domain, Data->Array_Descriptor.Array_Domain,
	      X_ParallelArray.Array_Descriptor.Array_Domain,
	      X_ParallelArray.Array_Descriptor.SerialArray->
	      Array_Descriptor.Array_Domain);
       */

       // (11/4/2000) I think this is always NULL for the temporary just built (verify)!
          APP_ASSERT (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info == NULL);

       // This is how we acumulate the information used to determine message passing 
       // when the operator= is executed (to terminate the expression statement)!
          if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info != NULL)
             {
            // printf ("Commented out Array_Conformability_Info_Type::delete \n");
               Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->decrementReferenceCount();
               if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount() < 
                   Array_Conformability_Info_Type::getReferenceCountBase())
                    delete Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info;
             }
          Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info = Array_Set;

       // Bugfix (11/27/2000)
          Array_Set->incrementReferenceCount();
        }

#if COMPILE_DEBUG_STATEMENTS
     APP_ASSERT(Temporary->Array_Descriptor.Array_Domain.Is_A_Temporary == TRUE);
     Temporary->Test_Consistency ("Test (on return) in #2 doubleArray::Abstract_Operator(ACIT,doubleArray,doubleSerialArray)");
     if (APP_DEBUG > 5)
        {
          Temporary->view ("view temporary (on return) in #2 doubleArray::Abstract_Operator(ACIT,doubleArray,doubleSerialArray)");
        }

     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Temporary->displayReferenceCounts("Temporary at BASE of #2 doubleArray::Abstract_Operator()");
          X_SerialArray.displayReferenceCounts("X_SerialArray at BASE of #2 doubleArray::Abstract_Operator()");
        }
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving #2 doubleArray::Abstract_Operator(Array_Conformability_Info_Type,doubleArray,doubleSerialArray) \n");
#endif

     return *Temporary;
   }





#if !defined(INTARRAY)
// if !defined(INTARRAY)
// ************************************************************************
// usual binary operators: operator+ operator- operator* operator/ operator% 
// for array to scalar operations
// ************************************************************************
intArray & 
doubleArray::Abstract_Operator (
     Array_Conformability_Info_Type *Array_Set, 
     const doubleArray & X_ParallelArray, 
     doubleSerialArray* X_Serial_PCE_Array,
     intSerialArray & X_SerialArray )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of #3 doubleArray::Abstract_Operator(Array_Conformability_Info_Type,doubleArray,intSerialArray) \n");

  // This is the only test we can do on the input!
     X_ParallelArray.Test_Consistency ("Test X_ParallelArray in #3 doubleArray::Abstract_Operator(Array_Conformability_Info_Type,doubleArray,intSerialArray) ");
     X_SerialArray.Test_Consistency ("Test X_SerialArray in #3 doubleArray::Abstract_Operator(Array_Conformability_Info_Type,doubleArray,intSerialArray) ");
#endif

  // error checking
     APP_ASSERT(Array_Set != NULL);

  // The size of the temporary must be the same as that of the Lhs (Lhs based owners compute rule)
     intSerialArray *Data = &((intSerialArray &) X_SerialArray);
     APP_ASSERT(Data != NULL);

     intArray *Temporary = NULL;
  // This is more efficient than the previous version which build a descriptor and
  // then forced 2 copies of the descriptor before ending up with a temporary to use.
     bool AvoidBuildingIndirectAddressingView = TRUE;
  // Data->view("In abstract_op #2: Data");
  // X_ParallelArray.Array_Descriptor.Array_Domain.display("In abstract_op #2: X_ParallelArray: Array_Domain");
     Temporary = new intArray ( Data , 
                               &(X_ParallelArray.Array_Descriptor.Array_Domain),
                               AvoidBuildingIndirectAddressingView );

     APP_ASSERT (Temporary != NULL);
     APP_ASSERT (Temporary->Array_Descriptor.SerialArray != NULL);
  // printf ("In #3 doubleArray::Abstract_Operator: Temporary->Array_Descriptor.SerialArray->getReferenceCount() = %d \n",
  //      Temporary->Array_Descriptor.SerialArray->getReferenceCount());
  // APP_ASSERT (Temporary->Array_Descriptor.SerialArray->getReferenceCount() > 1);

  // The base now needs to be reset since temporaries always have APP_Global_Array_Base as a base
  // ... don't reset this if indirect addressing is used ...
     if (!Temporary->Array_Descriptor.Array_Domain.Uses_Indirect_Addressing)
        Temporary->setBase(APP_Global_Array_Base);
          
  // Temporary->view ("view temporary in #3 doubleArray::Abstract_Operator(Array_Conformability_Info_Type,doubleArray,doubleSerialArray)");
  // APP_ABORT();

     Temporary->setTemporary(TRUE);

  // ... maybe this isn't needed with the new constructor ...

     /*
     Array_Domain_Type::fixupLocalBase
	( Descriptor->Array_Domain, Data->Array_descriptor.Array_Domain,
	  X_ParallelArray.Array_Descriptor.Array_Domain,
	  X_ParallelArray.Array_Descriptor.SerialArray->
	  Array_Descriptor.Array_Domain);
     */

  // Test (12/4/2000) I think this might be true!
     APP_ASSERT (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info == NULL);

  // This is now we acumulate the information used to determine message passing 
  // when the operator= is executed!
     if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info != NULL)
        {
       // printf ("Commented out Array_Conformability_Info_Type::delete \n");
          Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->decrementReferenceCount();
          if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount() < 
              Array_Conformability_Info_Type::getReferenceCountBase())
               delete Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info;
        }

     Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info = Array_Set;
  // Bugfix (11/27/2000)
     Array_Set->incrementReferenceCount();

#if COMPILE_DEBUG_STATEMENTS
     APP_ASSERT(Temporary->isTemporary() == TRUE);
     Temporary->Test_Consistency ("Test (on return) in #3 doubleArray::Abstract_Operator(Array_Domain.Array_Conformability_Info_Type,doubleArray,intSerialArray)");

  // Temporary->displayReferenceCounts("Called inside of #3 doubleArray::Abstract_Operator");
  // X_SerialArray.displayReferenceCounts("X_SerialArray at BASE of #3 doubleArray::Abstract_Operator()");
#endif

     return *Temporary;
   }
// endif



#endif



// if !defined(INTARRAY)
// ************************************************************************
// usual binary operators: operator+ operator- operator* operator/ operator% 
// for array to scalar operations
// ************************************************************************
intArray & 
doubleArray::Abstract_int_Conversion_Operator (
     Array_Conformability_Info_Type *Array_Set, 
     const doubleArray & X_ParallelArray, 
     doubleSerialArray* X_Serial_PCE_Array,
     intSerialArray & X_SerialArray )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of #3 doubleArray::Abstract_Operator(Array_Conformability_Info_Type,doubleArray,intSerialArray) \n");

  // This is the only test we can do on the input!
     X_ParallelArray.Test_Consistency ("Test X_ParallelArray in #3 doubleArray::Abstract_Operator(Array_Conformability_Info_Type,doubleArray,intSerialArray) ");
     X_SerialArray.Test_Consistency ("Test X_SerialArray in #3 doubleArray::Abstract_Operator(Array_Conformability_Info_Type,doubleArray,intSerialArray) ");
#endif

  // error checking
     APP_ASSERT(Array_Set != NULL);

  // The size of the temporary must be the same as that of the Lhs (Lhs based owners compute rule)
     intSerialArray *Data = &((intSerialArray &) X_SerialArray);
     APP_ASSERT(Data != NULL);

     intArray *Temporary = NULL;
  // This is more efficient than the previous version which build a descriptor and
  // then forced 2 copies of the descriptor before ending up with a temporary to use.
     bool AvoidBuildingIndirectAddressingView = TRUE;
  // Data->view("In abstract_op #2: Data");
  // X_ParallelArray.Array_Descriptor.Array_Domain.display("In abstract_op #2: X_ParallelArray: Array_Domain");
     Temporary = new intArray ( Data , 
                               &(X_ParallelArray.Array_Descriptor.Array_Domain),
                               AvoidBuildingIndirectAddressingView );

     APP_ASSERT (Temporary != NULL);
     APP_ASSERT (Temporary->Array_Descriptor.SerialArray != NULL);
  // printf ("In #3 doubleArray::Abstract_Operator: Temporary->Array_Descriptor.SerialArray->getReferenceCount() = %d \n",
  //      Temporary->Array_Descriptor.SerialArray->getReferenceCount());
  // APP_ASSERT (Temporary->Array_Descriptor.SerialArray->getReferenceCount() > 1);

  // The base now needs to be reset since temporaries always have APP_Global_Array_Base as a base
  // ... don't reset this if indirect addressing is used ...
     if (!Temporary->Array_Descriptor.Array_Domain.Uses_Indirect_Addressing)
        Temporary->setBase(APP_Global_Array_Base);
          
  // Temporary->view ("view temporary in #3 doubleArray::Abstract_Operator(Array_Conformability_Info_Type,doubleArray,doubleSerialArray)");
  // APP_ABORT();

     Temporary->setTemporary(TRUE);

  // ... maybe this isn't needed with the new constructor ...

     /*
     Array_Domain_Type::fixupLocalBase
	( Descriptor->Array_Domain, Data->Array_descriptor.Array_Domain,
	  X_ParallelArray.Array_Descriptor.Array_Domain,
	  X_ParallelArray.Array_Descriptor.SerialArray->
	  Array_Descriptor.Array_Domain);
     */

  // Test (12/4/2000) I think this might be true!
     APP_ASSERT (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info == NULL);

  // This is now we acumulate the information used to determine message passing 
  // when the operator= is executed!
     if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info != NULL)
        {
       // printf ("Commented out Array_Conformability_Info_Type::delete \n");
          Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->decrementReferenceCount();
          if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount() < 
              Array_Conformability_Info_Type::getReferenceCountBase())
               delete Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info;
        }

     Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info = Array_Set;
  // Bugfix (11/27/2000)
     Array_Set->incrementReferenceCount();

#if COMPILE_DEBUG_STATEMENTS
     APP_ASSERT(Temporary->isTemporary() == TRUE);
     Temporary->Test_Consistency ("Test (on return) in #3 doubleArray::Abstract_Operator(Array_Domain.Array_Conformability_Info_Type,doubleArray,intSerialArray)");

  // Temporary->displayReferenceCounts("Called inside of #3 doubleArray::Abstract_Operator");
  // X_SerialArray.displayReferenceCounts("X_SerialArray at BASE of #3 doubleArray::Abstract_Operator()");
#endif

     return *Temporary;
   }
// endif



// if !defined(INTARRAY)
// ************************************************************************
// usual binary operators: operator+ operator- operator* operator/ operator% 
// for array to scalar operations
// ************************************************************************
floatArray & 
doubleArray::Abstract_float_Conversion_Operator (
     Array_Conformability_Info_Type *Array_Set, 
     const doubleArray & X_ParallelArray, 
     doubleSerialArray* X_Serial_PCE_Array,
     floatSerialArray & X_SerialArray )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of #3 doubleArray::Abstract_Operator(Array_Conformability_Info_Type,doubleArray,intSerialArray) \n");

  // This is the only test we can do on the input!
     X_ParallelArray.Test_Consistency ("Test X_ParallelArray in #3 doubleArray::Abstract_Operator(Array_Conformability_Info_Type,doubleArray,intSerialArray) ");
     X_SerialArray.Test_Consistency ("Test X_SerialArray in #3 doubleArray::Abstract_Operator(Array_Conformability_Info_Type,doubleArray,floatSerialArray) ");
#endif

  // error checking
     APP_ASSERT(Array_Set != NULL);

  // The size of the temporary must be the same as that of the Lhs (Lhs based owners compute rule)
     floatSerialArray *Data = &((floatSerialArray &) X_SerialArray);
     APP_ASSERT(Data != NULL);

     floatArray *Temporary = NULL;
  // This is more efficient than the previous version which build a descriptor and
  // then forced 2 copies of the descriptor before ending up with a temporary to use.
     bool AvoidBuildingIndirectAddressingView = TRUE;
  // Data->view("In abstract_op #2: Data");
  // X_ParallelArray.Array_Descriptor.Array_Domain.display("In abstract_op #2: X_ParallelArray: Array_Domain");
     Temporary = new floatArray ( Data , 
                               &(X_ParallelArray.Array_Descriptor.Array_Domain),
                               AvoidBuildingIndirectAddressingView );

     APP_ASSERT (Temporary != NULL);
     APP_ASSERT (Temporary->Array_Descriptor.SerialArray != NULL);
  // printf ("In #3 doubleArray::Abstract_Operator: Temporary->Array_Descriptor.SerialArray->getReferenceCount() = %d \n",
  //      Temporary->Array_Descriptor.SerialArray->getReferenceCount());
  // APP_ASSERT (Temporary->Array_Descriptor.SerialArray->getReferenceCount() > 1);

  // The base now needs to be reset since temporaries always have APP_Global_Array_Base as a base
  // ... don't reset this if indirect addressing is used ...
     if (!Temporary->Array_Descriptor.Array_Domain.Uses_Indirect_Addressing)
        Temporary->setBase(APP_Global_Array_Base);
          
  // Temporary->view ("view temporary in #3 doubleArray::Abstract_Operator(Array_Conformability_Info_Type,doubleArray,doubleSerialArray)");
  // APP_ABORT();

     Temporary->setTemporary(TRUE);

  // ... maybe this isn't needed with the new constructor ...

     /*
     Array_Domain_Type::fixupLocalBase
	( Descriptor->Array_Domain, Data->Array_descriptor.Array_Domain,
	  X_ParallelArray.Array_Descriptor.Array_Domain,
	  X_ParallelArray.Array_Descriptor.SerialArray->
	  Array_Descriptor.Array_Domain);
     */

  // Test (12/4/2000) I think this might be true!
     APP_ASSERT (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info == NULL);

  // This is now we acumulate the information used to determine message passing 
  // when the operator= is executed!
     if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info != NULL)
        {
       // printf ("Commented out Array_Conformability_Info_Type::delete \n");
          Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->decrementReferenceCount();
          if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount() < 
              Array_Conformability_Info_Type::getReferenceCountBase())
               delete Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info;
        }

     Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info = Array_Set;
  // Bugfix (11/27/2000)
     Array_Set->incrementReferenceCount();

#if COMPILE_DEBUG_STATEMENTS
     APP_ASSERT(Temporary->isTemporary() == TRUE);
     Temporary->Test_Consistency ("Test (on return) in #3 doubleArray::Abstract_Operator(Array_Domain.Array_Conformability_Info_Type,doubleArray,floatSerialArray)");

  // Temporary->displayReferenceCounts("Called inside of #3 doubleArray::Abstract_Operator");
  // X_SerialArray.displayReferenceCounts("X_SerialArray at BASE of #3 doubleArray::Abstract_Operator()");
#endif

     return *Temporary;
   }
// endif



// if !defined(INTARRAY)
// ************************************************************************
// usual binary operators: operator+ operator- operator* operator/ operator% 
// for array to scalar operations
// ************************************************************************
doubleArray & 
doubleArray::Abstract_double_Conversion_Operator (
     Array_Conformability_Info_Type *Array_Set, 
     const doubleArray & X_ParallelArray, 
     doubleSerialArray* X_Serial_PCE_Array,
     doubleSerialArray & X_SerialArray )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of #3 doubleArray::Abstract_Operator(Array_Conformability_Info_Type,doubleArray,intSerialArray) \n");

  // This is the only test we can do on the input!
     X_ParallelArray.Test_Consistency ("Test X_ParallelArray in #3 doubleArray::Abstract_Operator(Array_Conformability_Info_Type,doubleArray,intSerialArray) ");
     X_SerialArray.Test_Consistency ("Test X_SerialArray in #3 doubleArray::Abstract_Operator(Array_Conformability_Info_Type,doubleArray,doubleSerialArray) ");
#endif

  // error checking
     APP_ASSERT(Array_Set != NULL);

  // The size of the temporary must be the same as that of the Lhs (Lhs based owners compute rule)
     doubleSerialArray *Data = &((doubleSerialArray &) X_SerialArray);
     APP_ASSERT(Data != NULL);

     doubleArray *Temporary = NULL;
  // This is more efficient than the previous version which build a descriptor and
  // then forced 2 copies of the descriptor before ending up with a temporary to use.
     bool AvoidBuildingIndirectAddressingView = TRUE;
  // Data->view("In abstract_op #2: Data");
  // X_ParallelArray.Array_Descriptor.Array_Domain.display("In abstract_op #2: X_ParallelArray: Array_Domain");
     Temporary = new doubleArray ( Data , 
                               &(X_ParallelArray.Array_Descriptor.Array_Domain),
                               AvoidBuildingIndirectAddressingView );

     APP_ASSERT (Temporary != NULL);
     APP_ASSERT (Temporary->Array_Descriptor.SerialArray != NULL);
  // printf ("In #3 doubleArray::Abstract_Operator: Temporary->Array_Descriptor.SerialArray->getReferenceCount() = %d \n",
  //      Temporary->Array_Descriptor.SerialArray->getReferenceCount());
  // APP_ASSERT (Temporary->Array_Descriptor.SerialArray->getReferenceCount() > 1);

  // The base now needs to be reset since temporaries always have APP_Global_Array_Base as a base
  // ... don't reset this if indirect addressing is used ...
     if (!Temporary->Array_Descriptor.Array_Domain.Uses_Indirect_Addressing)
        Temporary->setBase(APP_Global_Array_Base);
          
  // Temporary->view ("view temporary in #3 doubleArray::Abstract_Operator(Array_Conformability_Info_Type,doubleArray,doubleSerialArray)");
  // APP_ABORT();

     Temporary->setTemporary(TRUE);

  // ... maybe this isn't needed with the new constructor ...

     /*
     Array_Domain_Type::fixupLocalBase
	( Descriptor->Array_Domain, Data->Array_descriptor.Array_Domain,
	  X_ParallelArray.Array_Descriptor.Array_Domain,
	  X_ParallelArray.Array_Descriptor.SerialArray->
	  Array_Descriptor.Array_Domain);
     */

  // Test (12/4/2000) I think this might be true!
     APP_ASSERT (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info == NULL);

  // This is now we acumulate the information used to determine message passing 
  // when the operator= is executed!
     if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info != NULL)
        {
       // printf ("Commented out Array_Conformability_Info_Type::delete \n");
          Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->decrementReferenceCount();
          if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount() < 
              Array_Conformability_Info_Type::getReferenceCountBase())
               delete Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info;
        }

     Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info = Array_Set;
  // Bugfix (11/27/2000)
     Array_Set->incrementReferenceCount();

#if COMPILE_DEBUG_STATEMENTS
     APP_ASSERT(Temporary->isTemporary() == TRUE);
     Temporary->Test_Consistency ("Test (on return) in #3 doubleArray::Abstract_Operator(Array_Domain.Array_Conformability_Info_Type,doubleArray,doubleSerialArray)");

  // Temporary->displayReferenceCounts("Called inside of #3 doubleArray::Abstract_Operator");
  // X_SerialArray.displayReferenceCounts("X_SerialArray at BASE of #3 doubleArray::Abstract_Operator()");
#endif

     return *Temporary;
   }
// endif




// ************************************************************************
// usual binary operators: operator+ operator- operator* operator/ operator% 
// for array to array operations
// ************************************************************************
doubleArray &
doubleArray::Abstract_Operator (
     Array_Conformability_Info_Type *Array_Set, 
     const doubleArray & Lhs_ParallelArray, const doubleArray & Rhs_ParallelArray, 
     doubleSerialArray* Lhs_Serial_PCE_Array, doubleSerialArray* Rhs_Serial_PCE_Array, 
     doubleSerialArray & X_SerialArray )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of #4 doubleArray::Abstract_Operator(Array_Conformability_Info_Type,doubleArray,doubleArray,doubleSerialArray) \n");
#endif

  // error checking
     APP_ASSERT(Array_Set != NULL);

#if COMPILE_DEBUG_STATEMENTS
  // printf ("doubleArray::Abstract_Operator for binary operators! \n");
  // These are the only tests we can do on the input!
     Array_Set->Test_Consistency("Test Array_Set in #4 doubleArray::Abstract_Operator(Array_Conformability_Info_Type,doubleArray,doubleArray,doubleSerialArray)");
     Lhs_ParallelArray.Test_Consistency ("Test LHS in #4 doubleArray::Abstract_Operator(Array_Conformability_Info_Type,doubleArray,doubleArray,doubleSerialArray)");
     Rhs_ParallelArray.Test_Consistency ("Test RHS in #4 doubleArray::Abstract_Operator(Array_Conformability_Info_Type,doubleArray,doubleArray,doubleSerialArray)");
     X_SerialArray.Test_Consistency     ("Test X_SerialArray in #4 doubleArray::Abstract_Operator(Array_Conformability_Info_Type,doubleArray,doubleArray,doubleSerialArray)");
#endif

  // Used for loop indexing
  // int i = 0;

     if (Index::Index_Bounds_Checking)
          Lhs_ParallelArray.Test_Conformability (Rhs_ParallelArray);

  // The Partitioning array is the array object (usually the Lhs operand) which the 
  // resulting temporary will be partitioned similar to (if it is required to build the temporary).
  // If a scalar was used on the Lhs then we have to use the Rhs as a Partitioning_doubleArray
  // doubleArray* Partitioning_Array = &((doubleArray &) Lhs_ParallelArray);
  // APP_ASSERT (Partitioning_Array != NULL);

  // The size of the temporary must be the same as that of the Lhs (Lhs based owners compute rule)
     doubleSerialArray *Data = &((doubleSerialArray &) X_SerialArray);
     APP_ASSERT(Data != NULL);

#if COMPILE_DEBUG_STATEMENTS
  // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
     if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
        {
          printf ("SerialArray input to doubleArray::Abstract_Operator X_SerialArray: local range (%d,%d,%d) \n",
               X_SerialArray.getBase(0),
               X_SerialArray.getBound(0),
               X_SerialArray.getStride(0));
        }
#endif

     doubleArray* Temporary = NULL;
     Memory_Source_Type Result_Is_Lhs_Or_Rhs_Or_New_Memory = Uninitialized_Source;
     if ( Lhs_ParallelArray.isTemporary() == TRUE )
        {
       // Temporary reuse of serial array already handled at Serial_A++ level
#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
               printf ("Reuse the Lhs since it is a temporary! \n");
#endif
          Temporary = &((doubleArray &) Lhs_ParallelArray);
          Result_Is_Lhs_Or_Rhs_Or_New_Memory = Memory_From_Lhs;

       // Delete the old Array_Conformability_Info object so that the new one can be inserted
          if (Lhs_ParallelArray.Array_Descriptor.Array_Domain.Array_Conformability_Info != NULL)
             {
               Lhs_ParallelArray.Array_Descriptor.Array_Domain.Array_Conformability_Info->decrementReferenceCount();
               if (Lhs_ParallelArray.Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount() < 
                   Array_Conformability_Info_Type::getReferenceCountBase())
                    delete Lhs_ParallelArray.Array_Descriptor.Array_Domain.Array_Conformability_Info;
             }

#if COMPILE_DEBUG_STATEMENTS
          Array_Set->Test_Consistency("Test Array_Set in #4 doubleArray::Abstract_Operator (after delete Lhs_ParallelArray.Array_Descriptor.Array_Domain.Array_Conformability_Info)");
#endif
       // ... a new Array_Set is computed now even if Lhs has one ...
       // APP_ASSERT( Lhs_ParallelArray.Array_Descriptor.Array_Domain.Array_Conformability_Info == Array_Set );
          ((doubleArray&)Lhs_ParallelArray).Array_Descriptor.Array_Domain.Array_Conformability_Info = Array_Set;
       // Bugfix (11/27/2000)
          Array_Set->incrementReferenceCount();

#if COMPILE_DEBUG_STATEMENTS
          Array_Set->Test_Consistency("Lhs_ParallelArray.Array_Descriptor.Array_Domain.Array_Conformability_Info == Array_Set at MIPOINT of #4 doubleArray::Abstract_Operator(Array_Conformability_Info_Type,doubleArray,doubleArray,doubleSerialArray)");
#endif

       // Temporary->displayReferenceCounts("Temporary at MIPOINT of #4 doubleArray::Abstract_Operator");

          Array_Domain_Type::fixupLocalBase 
	     ( (Array_Domain_Type&)Lhs_ParallelArray.Array_Descriptor.Array_Domain , 
	       Data->Array_Descriptor.Array_Domain , 
               Lhs_ParallelArray.Array_Descriptor.Array_Domain , 
               Lhs_ParallelArray.Array_Descriptor.SerialArray->
	       Array_Descriptor.Array_Domain );

       // Lhs_ParallelArray.view("In P++ Abstract_Operator() Lhs_ParallelArray");

          APP_ASSERT (Temporary != NULL);
          APP_ASSERT (Temporary->Array_Descriptor.SerialArray != NULL);
          APP_ASSERT (Temporary->Array_Descriptor.SerialArray->getReferenceCount() >= doubleArray::getReferenceCountBase());
       // Delete the serial array in the Temporary that will be reused 
       // (so it can be replaced by the input serialArray!)
          Temporary->Array_Descriptor.SerialArray->decrementReferenceCount();
          if (Temporary->Array_Descriptor.SerialArray->getReferenceCount() < doubleArray::getReferenceCountBase())
               delete Temporary->Array_Descriptor.SerialArray;

       // Copy the input serialArray into the temporary (the return value)
          Temporary->Array_Descriptor.SerialArray = Data;

       // Temporary->displayReferenceCounts("Temporary after insertion of X_SerialArray in #4 doubleArray::Abstract_Operator");

          APP_ASSERT (Data->getRawDataReferenceCount() >= getRawDataReferenceCountBase());
       // printf ("Exiting in reuse of Lhs as a test ... \n");
       // APP_ABORT();
        }
       else
        {
       // In the case of a Full_VSG_Update only the Lhs can be reused the Rhs may not be reused
       // This is because the Rhs is reconstructed for each binary operation and the data for the
       // Rhs is assembled into the newly constructed Rhs (which is the size of the local Lhs)
       // from all the processors owning that part of the global indexspace required.
       // This detail was unexpected and might be handled more efficiently in the future.
       // if ( Rhs_ParallelArray.Array_Descriptor.Array_Domain.Is_A_Temporary == TRUE )
          if ( (Rhs_ParallelArray.Array_Descriptor.Array_Domain.Is_A_Temporary == TRUE) && 
               (Array_Set->Full_VSG_Update_Required == FALSE) )
             {
            // Temporary reuse of serial array already handled at Serial_A++ level
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Reuse the Rhs since it is a temporary! \n");
#endif
            // printf ("Reuse the Rhs since it is a temporary! Exiting ... \n");
            // APP_ABORT();

               Temporary = &((doubleArray &) Rhs_ParallelArray);
               Result_Is_Lhs_Or_Rhs_Or_New_Memory = Memory_From_Rhs;

               if (Rhs_ParallelArray.Array_Descriptor.Array_Domain.Array_Conformability_Info != NULL)
                  {
#if COMPILE_DEBUG_STATEMENTS
                    if (APP_DEBUG > 0)
                         printf ("delete for Array_Conformability_Info_Type called in doubleArray::Abstract_Operator (Array_Conformability_Info_Type,doubleArray,doubleArray,doubleSerialArray)");
#endif
                    Rhs_ParallelArray.Array_Descriptor.Array_Domain.Array_Conformability_Info->decrementReferenceCount();
                    if (Rhs_ParallelArray.Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount() < 
                        Array_Conformability_Info_Type::getReferenceCountBase())
                         delete Rhs_ParallelArray.Array_Descriptor.Array_Domain.Array_Conformability_Info;
                  }

#if COMPILE_DEBUG_STATEMENTS
               Array_Set->Test_Consistency("Test Array_Set in #4 doubleArray::Abstract_Operator (after delete Rhs_ParallelArray.Array_Descriptor.Array_Domain.Array_Conformability_Info)");
#endif
               // ... TEMP TEST:  there is no reason this should br
	       //   true now ...
               //APP_ASSERT( Rhs_ParallelArray.Array_Descriptor.Array_Domain.Array_Conformability_Info == Array_Set );

               ((doubleArray&)Rhs_ParallelArray).Array_Descriptor.Array_Domain.Array_Conformability_Info = Array_Set;
            // Bugfix (11/27/2000)
               Array_Set->incrementReferenceCount();

            // Data->view("Data BEFORE setBase");

               Array_Domain_Type::fixupLocalBase 
		  ((Array_Domain_Type&)Rhs_ParallelArray.Array_Descriptor.Array_Domain , 
		   Data->Array_Descriptor.Array_Domain , 
                   Rhs_ParallelArray.Array_Descriptor.Array_Domain , 
                   Rhs_ParallelArray.Array_Descriptor.SerialArray->
		   Array_Descriptor.Array_Domain );

	       // ... (10/11/96,kdb) correct Array_Set because it is valid
	       //  only for Lhs ...

               // ... temporary is consistent with Rhs so reverse offsets ...
               bool reverse_offset = TRUE;

               Array_Domain_Type:: Fix_Array_Conformability 
		  ( *Array_Set, Rhs_ParallelArray.Array_Descriptor.Array_Domain , 
		     Data->Array_Descriptor.Array_Domain, reverse_offset );

#if 0
               Modify_Reference_Counts_And_Manage_Temporaries ( Array_Set, Temporary, Lhs_Serial_PCE_Array, Rhs_Serial_PCE_Array, Data );
#endif

            // Temporary->Array_Descriptor.SerialArray = Data;

            // Bugfix (2/5/96) Must increment reference count since the operator.C code
            // will delete the serial arrays that the PCE returns.
            // printf ("Increment reference count on array reused (LHS)! \n");
            // While we have to increment the SerialArray's reference count so that the delete in the 
            // binary operator will not destroy the SerialArray --- we have to decrement the RawDataReferenceCount
            // because we want to use the raw data obtained though the view taken in PCE and
            // we will not be using the original array from which that view was taken.

            // To complicate issues the relational operators return a newly built temporary and so 
            // in this case the reference count of the data is ZERO and we can't (and should not) 
            // decrement it.
            // if (Data->getRawDataReferenceCount() > 0)
            //      Data->decrementRawDataReferenceCount();
            // Data->incrementReferenceCount();
               APP_ASSERT (Data->getRawDataReferenceCount() >= getRawDataReferenceCountBase());

            // printf ("Exiting in reuse of Rhs as a test ... \n");
            // APP_ABORT();

            // Temporary->view("Temporary");
             }
            else
             {
            // No temporary to reuse so we have to build one!
            // later we can avoid the construction of a new descriptor

#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Build a temporary since neither Lhs nor Rhs is a temporary! \n");
#endif
            // printf ("Build a temporary since neither Lhs nor Rhs is a temporary! \n");
            // APP_ABORT();
            // We should better call the doubleArray_Descriptor_Type::Build_Temporary_By_Example
            // Now we have a descriptor based on the descriptor from the Lhs operand.  But we
            // have to fix it up in order to use it. Or mabe we should change the design so 
            // we don't have to change it (i.e. no re-centering would then be required) 

            // printf ("ERROR: In Abstract_Operator we are doing redundent copying (exit until this is fixed) \n");
            // APP_ABORT();
            // This is more efficient than the previous version which build a descriptor and
            // then forced 2 copies of the descriptor before ending up with a temporary to use.
               bool AvoidBuildingIndirectAddressingView = TRUE;
            // Data->view("In abstract_op #2: Data");
            // X_ParallelArray.Array_Descriptor.Array_Domain.display("In abstract_op #2: X_ParallelArray: Array_Domain");
#if COMPILE_DEBUG_STATEMENTS
            // Temporary->view ("view temporary in #4 doubleArray::Abstract_Operator(): AFTER Temporary is built!");
               if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
                  {
                    printf ("(Abstract_Operator()) - Lhs_ParallelArray: global range (%d,%d,%d) local range (%d,%d,%d) \n",
                         Lhs_ParallelArray.getGlobalMaskIndex(0).getBase(),
                         Lhs_ParallelArray.getGlobalMaskIndex(0).getBound(),
                         Lhs_ParallelArray.getGlobalMaskIndex(0).getStride(),
                         Lhs_ParallelArray.getLocalMaskIndex(0).getBase(),
                         Lhs_ParallelArray.getLocalMaskIndex(0).getBound(),
                         Lhs_ParallelArray.getLocalMaskIndex(0).getStride());
                  }
#endif

               Temporary = new doubleArray ( Data ,
                                         &(Lhs_ParallelArray.Array_Descriptor.Array_Domain),
                                         AvoidBuildingIndirectAddressingView );

#if COMPILE_DEBUG_STATEMENTS
            // Temporary->view ("view temporary in #4 doubleArray::Abstract_Operator(): AFTER Temporary is built!");
               if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
                  {
                    printf ("(Abstract_Operator()) - Build the parallel array as a return value (BEFORE setBase()) - Temporary: global range (%d,%d,%d) local range (%d,%d,%d) \n",
                         Temporary->getGlobalMaskIndex(0).getBase(),
                         Temporary->getGlobalMaskIndex(0).getBound(),
                         Temporary->getGlobalMaskIndex(0).getStride(),
                         Temporary->getLocalMaskIndex(0).getBase(),
                         Temporary->getLocalMaskIndex(0).getBound(),
                         Temporary->getLocalMaskIndex(0).getStride());
        }
#endif

            // The base now needs to be reset since temporaries always have APP_Global_Array_Base as a base
	    // ... only reset if no indirect addressing ...
	       if (!Temporary->Array_Descriptor.Array_Domain.Uses_Indirect_Addressing)
                    Temporary->setBase(APP_Global_Array_Base);

#if 0
               Temporary->view ("view temporary in #4 doubleArray::Abstract_Operator(Array_Conformability_Info_Type,doubleArray,doubleSerialArray)");
            // APP_ABORT();
#endif

#if COMPILE_DEBUG_STATEMENTS
               if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
                  {
                    printf ("(Abstract_Operator()) - Build the parallel array as a return value (AFTER setBase()) - Temporary: global range (%d,%d,%d) local range (%d,%d,%d) \n",
                         Temporary->getGlobalMaskIndex(0).getBase(),
                         Temporary->getGlobalMaskIndex(0).getBound(),
                         Temporary->getGlobalMaskIndex(0).getStride(),
                         Temporary->getLocalMaskIndex(0).getBase(),
                         Temporary->getLocalMaskIndex(0).getBound(),
                         Temporary->getLocalMaskIndex(0).getStride());
        }
#endif

               Temporary->setTemporary(TRUE);
 
	    // ... might not need this now ...

            /*
	    Array_Domain_Type::fixupLocalBase
	       (Descriptor->Array_Domain,
	        Data->Array_Descriptor.Array_Domain,
		Lhs_ParallelArray.Array_Descriptor.Array_Domain,
		Lhs_ParallelArray.Array_Descriptor.SerialArray->
		Array_Descriptor.Array_Domain);
            */

            // This is now we acumulate the information used to determine message passing
            // when the operator= is executed!
               APP_ASSERT (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info == NULL);
               Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info = Array_Set;
            // Bugfix (11/27/2000)
               Array_Set->incrementReferenceCount();

            // ... temporary is consistent with Lhs so don't reverse offsets ...
               bool reverse_offset = FALSE;

               Array_Domain_Type:: Fix_Array_Conformability ( 
		    *Array_Set, Temporary->Array_Descriptor.Array_Domain , 
		    Data->Array_Descriptor.Array_Domain, reverse_offset);

            // Bugfix (9/14/95) This should fix the problem related to the Rhs being a temporary
            // and thus the Rhs serial array being reused in the A++ class libaray such that it
            // is referenced in the Temporary which the function returns and the Rhs.
            // The problem shows up when B is a null array and we have B = B + B * B;
            // I would guess that this is because the Null arrays objects in this case
            // are multibily referenced rather than redundently built.
            // But if so --- is this the best place to increment the reference count!
               APP_ASSERT ( Data != NULL );
               if ( (Rhs_ParallelArray.isTemporary() == TRUE) && (Data->isNullArray() == TRUE) )
                  {
#if COMPILE_DEBUG_STATEMENTS
                    if (APP_DEBUG > 1)
                         printf ("Special case of Rhs temporary and reuse of Rhs SerialArray (B = B + B * B) reference count incremented in #4 doubleArray::Abstract_Operator \n");
#endif
                 // printf ("Special Case Exiting ... \n");
                 // APP_ABORT();
                    Data->incrementReferenceCount();
                  }

            // Temporary->view("Temporary at MIPOINT of doubleArray::Abstract_Operator(Array_Conformability_Info_Type,doubleArray,doubleArray,doubleSerialArray)");
            // Temporary->Test_Consistency ("Test TEMPORARY at BASE of #4 doubleArray::Abstract_Operator(Array_Conformability_Info_Type,doubleArray,doubleArray,doubleSerialArray)");

            // printf ("Exiting in #4 doubleArray::Abstract_Operator ... \n");
            // APP_ABORT();

               Result_Is_Lhs_Or_Rhs_Or_New_Memory = Newly_Allocated_Memory;
             }
        }

     APP_ASSERT(Temporary != NULL);
     APP_ASSERT(Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info != NULL);
     APP_ASSERT(Temporary->Array_Descriptor.SerialArray != NULL);

  // printf ("In #4 doubleArray::Abstract_Operator: Temporary->Array_Descriptor.SerialArray->Array_ID = %d \n",
  //      Temporary->Array_Descriptor.SerialArray->Array_ID());

#if COMPILE_DEBUG_STATEMENTS
     APP_ASSERT(Temporary->Array_Descriptor.Array_Domain.Is_A_Temporary == TRUE);
  // Temporary->Test_Consistency ("Test Temporary after construction in #4 doubleArray::Abstract_Operator");

     if (APP_DEBUG > 1)
          printf ("Now delete the temporaries handed in as input if they were not reused! \n");
#endif

  // If the Lhs was a temporary then we used it so we don't have to call
  // the "Delete_If_Temporary ( Lhs_ParallelArray );" function for the LHS
  // We want to delete the RHS if it is a temporary unless we used it
     if (Result_Is_Lhs_Or_Rhs_Or_New_Memory != Memory_From_Rhs)
          Delete_If_Temporary ( Rhs_ParallelArray );

  // printf ("(AFTER RHS TEST) Temporary->Array_Descriptor.SerialArray->Array_Descriptor = %p \n",Temporary->Array_Descriptor.SerialArray->Array_Descriptor);
  // printf ("Set Temporary->Array_Descriptor.Array_Set = Array_Set in doubleArray::Abstract_Binary_Operator \n");

#if COMPILE_DEBUG_STATEMENTS
     APP_ASSERT(Temporary->Array_Descriptor.Array_Domain.Is_A_Temporary == TRUE);
     Temporary->Test_Consistency ("Test TEMPORARY at BASE of #4 doubleArray::Abstract_Operator(Array_Conformability_Info_Type,doubleArray,doubleArray,doubleSerialArray)");

     if (APP_DEBUG > 1)
        {
          if (APP_DEBUG > 2)
             {
               Temporary->view ("view TEMPORARY at BASE of #4 doubleArray::Abstract_Operator(Array_Conformability_Info_Type,doubleArray,doubleArray,doubleSerialArray)");
            // Lhs_Serial_PCE_Array->view ("Lhs_Serial_PCE_Array at BASE of #4 doubleArray::Abstract_Operator(Array_Conformability_Info_Type,doubleArray,doubleArray,doubleSerialArray)");
            // Rhs_Serial_PCE_Array->view ("Rhs_Serial_PCE_Array at BASE of #4 doubleArray::Abstract_Operator(Array_Conformability_Info_Type,doubleArray,doubleArray,doubleSerialArray)");
             }
          printf ("************************************************************************************************* \n");
          printf ("************************************************************************************************* \n");
          printf ("Leaving #4 doubleArray::Abstract_Operator (Array_Conformability_Info_Type,doubleArray,doubleArray,doubleSerialArray) \n");
          printf ("************************************************************************************************* \n");
          printf ("************************************************************************************************* \n");
        }

     if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
        {
          printf ("Build the parallel array as a return value (#4 doubleArray::Abstract_Operator) - Temporary: global range (%d,%d,%d) local range (%d,%d,%d) \n",
               Temporary->getGlobalMaskIndex(0).getBase(),
               Temporary->getGlobalMaskIndex(0).getBound(),
               Temporary->getGlobalMaskIndex(0).getStride(),
               Temporary->getLocalMaskIndex(0).getBase(),
               Temporary->getLocalMaskIndex(0).getBound(),
               Temporary->getLocalMaskIndex(0).getStride());
        }

  // Commented out (12/14/2000)
  // Communication_Manager::Sync();
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
        {
          APP_ASSERT (Temporary != NULL);
          APP_ASSERT (Temporary->Array_Descriptor.SerialArray != NULL);
          printf ("In #4 doubleArray::Abstract_Operator(): Temporary->Array_Descriptor.Array_ID() = %d Temporary->SerialArray->Array_ID() = %d \n",
               Temporary->Array_ID(),Temporary->Array_Descriptor.SerialArray->Array_ID());
          printf ("************************************************************************************************* \n");
          printf ("Leaving #4 doubleArray::Abstract_Operator(Array_Conformability_Info_Type,doubleArray,doubleArray,doubleSerialArray) (Temporary SerialArray pointer = %p) \n",Temporary->Array_Descriptor.SerialArray);
          printf ("************************************************************************************************* \n");
        }
#endif

     return *Temporary;
   }

#if !defined(INTARRAY)
// replace operators for array to array
intArray &
doubleArray::Abstract_Operator ( 
     Array_Conformability_Info_Type *Array_Set, 
     const doubleArray & Lhs_ParallelArray, const doubleArray & Rhs_ParallelArray, 
     doubleSerialArray* Lhs_Serial_PCE_Array, doubleSerialArray* Rhs_Serial_PCE_Array, 
     intSerialArray & X_SerialArray )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of #5 doubleArray::Abstract_Operator(Array_Conformability_Info_Type,doubleArray,doubleArray,intSerialArray) \n");

  // This is the only test we can do on the input!
     Lhs_ParallelArray.Test_Consistency ("Test LHS in #5 doubleArray::Abstract_Operator(Array_Conformability_Info_Type,doubleArray,doubleArray,intSerialArray)");
     Rhs_ParallelArray.Test_Consistency ("Test RHS in #5 doubleArray::Abstract_Operator(Array_Conformability_Info_Type,doubleArray,doubleArray,intSerialArray)");
     X_SerialArray.Test_Consistency ("Test X_SerialArray in #5 doubleArray::Abstract_Operator(Array_Conformability_Info_Type,doubleArray,doubleArray,intSerialArray)");
#endif

     if (Index::Index_Bounds_Checking)
          Lhs_ParallelArray.Test_Conformability (Rhs_ParallelArray);

  // error checking
     APP_ASSERT(Array_Set != NULL);

  // The Partitioning array is the array object (usually the Lhs operand) which the
  // resulting temporary will be partitioned similar to (if it is required to build the temporary).
  // If a scalar was used on the Lhs then we have to use the Rhs as a Partitioning_doubleArray
  // doubleArray* Partitioning_Array = &((doubleArray &) Lhs_ParallelArray);
  // APP_ASSERT (Partitioning_Array != NULL);

  // The size of the temporary must be the same as that of the Lhs (Lhs based owners compute rule)
     intSerialArray *Data = &((intSerialArray &) X_SerialArray);
     APP_ASSERT(Data != NULL);

     intArray *Temporary = NULL;

  // printf ("ERROR: In Abstract_Operator we are doing redundent copying (exit until this is fixed) \n");
  // APP_ABORT();
  // This is more efficient than the previous version which build a descriptor and
  // then forced 2 copies of the descriptor before ending up with a temporary to use.
     bool AvoidBuildingIndirectAddressingView = TRUE;
  // Data->view("In abstract_op #2: Data");
  // X_ParallelArray.Array_Descriptor.Array_Domain.display("In abstract_op #2: X_ParallelArray: Array_Domain");
     Temporary = new intArray ( Data ,
                               &(Lhs_ParallelArray.Array_Descriptor.Array_Domain),
                               AvoidBuildingIndirectAddressingView );
 
  // The base now needs to be reset since temporaries always have APP_Global_Array_Base as a base
     Temporary->setBase(APP_Global_Array_Base);
 
  // Temporary->view ("view temporary in #5 doubleArray::Abstract_Operator(Array_Conformability_Info_Type,doubleArray,doubleSerialArray)");
  // APP_ABORT();
 
     Temporary->setTemporary(TRUE);
 
  // This is now we acumulate the information used to determine message passing
  // when the operator= is executed!
  // APP_ASSERT (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info == NULL);
  // Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info = Array_Set;

#if USE_TEMPORARY_DELETE_FUNCTIONS
     Delete_If_Temporary ( Lhs_ParallelArray );
     Delete_If_Temporary ( Rhs_ParallelArray );
#endif

  // This is how we acumulate the information used to determine message passing 
  // when the operator= is executed!
     if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info != NULL)
        {
          Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->decrementReferenceCount();
          if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount() < 
              Array_Conformability_Info_Type::getReferenceCountBase())
          delete Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info;
        }

     Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info = Array_Set;
  // Bugfix (11/27/2000)
     Array_Set->incrementReferenceCount();

#if COMPILE_DEBUG_STATEMENTS
     APP_ASSERT(Temporary->Array_Descriptor.Array_Domain.Is_A_Temporary == TRUE);
     Temporary->Test_Consistency ("Test (on return) in #5 doubleArray::Abstract_Operator(Array_Conformability_Info_Type,doubleArray,doubleArray,intSerialArray)");
#endif

     return *Temporary;
   }
#endif

// ******************************************************************************
// These Abstract_Modification_Operator operators are special because they don't
// return a temporary.  This function is used for the array to scalar relational operators 
// ******************************************************************************
doubleArray &
doubleArray::Abstract_Modification_Operator (
     Array_Conformability_Info_Type *Array_Set, 
     const doubleArray & X_ParallelArray, 
     doubleSerialArray* X_Serial_PCE_Array, 
     const doubleSerialArray & X_SerialArray )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of #6 doubleArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,doubleArray,doubleSerialArray) \n");

  // This is the only test we can do on the input!
     X_ParallelArray.Test_Consistency ("Test A in #6 doubleArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,doubleArray,doubleSerialArray)");
     X_SerialArray.Test_Consistency ("Test B in #6 doubleArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,doubleArray,doubleSerialArray)");
#endif

#if USE_TEMPORARY_DELETE_FUNCTIONS
  // Delete_If_Temporary ( X_ParallelArray );
#endif

     doubleArray* Temporary = &((doubleArray &) X_ParallelArray);

  // Modify_Reference_Counts_And_Manage_Temporaries ( Temporary , Data );

  // This is now we acumulate the information used to determine message passing 
  // when the operator= is executed!
     if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info != NULL)
        {
          Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->decrementReferenceCount();
          if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount() < 
              Array_Conformability_Info_Type::getReferenceCountBase())
          delete Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info;
        }
     Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info = NULL;

#if 0
  // Bugfix (11/27/2000)
     Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info = Array_Set;
  // Bugfix (11/27/2000)
     Array_Set->incrementReferenceCount();
#endif

#if 0
  // Need to delete the Array_Set to avoid a memory leak
     Array_Set->decrementReferenceCount();
     if (Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
          delete Array_Set;
     Array_Set = NULL;
#endif

#if COMPILE_DEBUG_STATEMENTS
  // APP_ASSERT(Temporary->Array_Descriptor.Array_Domain.Is_A_Temporary == TRUE);
     Temporary->Test_Consistency ("Test (on return) in #6 doubleArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,doubleArray,doubleSerialArray)");
#endif

     return *Temporary;
   }

// ******************************************************************************
// These Abstract_Modification_Operator operators are special because they don't
// return a temporary.  This function is used for the array to array replace operators 
// ******************************************************************************
doubleArray &
doubleArray::Abstract_Modification_Operator (
     Array_Conformability_Info_Type *Array_Set, 
     const doubleArray & This_ParallelArray, const doubleArray & Lhs_ParallelArray, 
     doubleSerialArray* This_Serial_PCE_Array, doubleSerialArray* Lhs_Serial_PCE_Array, 
     const doubleSerialArray & This_SerialArray )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of #7 doubleArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,doubleArray,doubleArray,doubleSerialArray) \n");

  // This is the only test we can do on the input!
     This_ParallelArray.Test_Consistency ("Test THIS in #7 doubleArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,doubleArray,doubleArray,doubleSerialArray)");
     Lhs_ParallelArray.Test_Consistency ("Test LHS in #7 doubleArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,doubleArray,doubleArray,doubleSerialArray)");
     This_SerialArray.Test_Consistency ("Test This_SerialArray in #7 doubleArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,doubleArray,doubleArray,doubleSerialArray)");
#endif

     if (Index::Index_Bounds_Checking)
          This_ParallelArray.Test_Conformability (Lhs_ParallelArray);

#if USE_TEMPORARY_DELETE_FUNCTIONS
  // Delete_If_Temporary ( This_ParallelArray );
     Delete_If_Temporary ( Lhs_ParallelArray );
#endif

     doubleArray* Temporary = &((doubleArray &) This_ParallelArray);

  // Modify_Reference_Counts_And_Manage_Temporaries ( Temporary , Data );

  // This is how we acumulate the information used to determine message passing 
  // when the operator= is executed!
     if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info != NULL)
        {
       // printf ("Commented out Array_Conformability_Info_Type::delete \n");
          Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->decrementReferenceCount();
          if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount() < 
              Array_Conformability_Info_Type::getReferenceCountBase())
               delete Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info;
        }
     Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info = NULL;

#if 0
  // Bugfix (11/27/2000)
     Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info = Array_Set;
  // Bugfix (11/27/2000)
     Array_Set->incrementReferenceCount();
#endif

#if 0
  // Need to delete the Array_Set to avoid a memory leak
     Array_Set->decrementReferenceCount();
     if (Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
          delete Array_Set;
     Array_Set = NULL;
#endif

#if COMPILE_DEBUG_STATEMENTS
  // APP_ASSERT(Temporary->Array_Descriptor.Array_Domain.Is_A_Temporary == TRUE);
     Temporary->Test_Consistency ("Test (on return) in #7 doubleArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,doubleArray,doubleSerialArray)");
#endif

     return *Temporary;
   }

#if !defined(INTARRAY)
// ******************************************************************************
// These Abstract_Modification_Operator operators are special because they don't
// return a temporary.  This function is used for the array to array replace operators 
// ******************************************************************************
doubleArray &
doubleArray::Abstract_Modification_Operator (
     Array_Conformability_Info_Type *Array_Set, 
     const doubleArray & This_ParallelArray, const intArray & Lhs_ParallelArray, 
     doubleSerialArray* This_Serial_PCE_Array, intSerialArray* Lhs_Serial_PCE_Array, 
     const doubleSerialArray & This_SerialArray )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of #8 doubleArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,intArray,doubleArray,doubleSerialArray) \n");

  // This is the only test we can do on the input!
     This_ParallelArray.Test_Consistency ("Test THIS in #8 doubleArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,intArray,doubleArray,doubleSerialArray)");
     Lhs_ParallelArray.Test_Consistency ("Test LHS in #8 doubleArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,intArray,doubleArray,doubleSerialArray)");
     This_SerialArray.Test_Consistency ("Test This_SerialArray in #8 doubleArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,intArray,doubleArray,doubleSerialArray)");
#endif

     if (Index::Index_Bounds_Checking)
          This_ParallelArray.Test_Conformability (Lhs_ParallelArray);

     APP_ASSERT(Array_Set != NULL);

#if USE_TEMPORARY_DELETE_FUNCTIONS
  // Delete_If_Temporary ( This_ParallelArray );
     Delete_If_Temporary ( Lhs_ParallelArray );
#endif

     doubleArray* Temporary = &((doubleArray &) This_ParallelArray);

  // Modify_Reference_Counts_And_Manage_Temporaries ( Temporary , Data );

  // This is now we acumulate the information used to determine message passing 
  // when the operator= is executed!
     if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info != NULL)
        {
       // printf ("Commented out Array_Conformability_Info_Type::delete \n");
          Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->decrementReferenceCount();
          if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount() < 
              Array_Conformability_Info_Type::getReferenceCountBase())
               delete Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info;
        }
     Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info = NULL;

#if 0
  // Bugfix (11/27/2000)
     Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info = Array_Set;
  // Bugfix (11/27/2000)
     Array_Set->incrementReferenceCount();
#endif

#if 0
  // Need to delete the Array_Set to avoid a memory leak
     Array_Set->decrementReferenceCount();
     if (Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
          delete Array_Set;
     Array_Set = NULL;
#endif

#if COMPILE_DEBUG_STATEMENTS
  // APP_ASSERT(Temporary->Array_Descriptor.Array_Domain.Is_A_Temporary == TRUE);
     Temporary->Test_Consistency ("Test (on return) in #8 doubleArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,intArray,doubleArray,doubleSerialArray)");
#endif

     return *Temporary;
   }

// ******************************************************************************
// These Abstract_Modification_Operator operators are special because they don't
// return a temporary.  This function is used for the array to scalar relational operators 
// ******************************************************************************
doubleArray &
doubleArray::Abstract_Modification_Operator (
     Array_Conformability_Info_Type *Array_Set, 
     const doubleArray & This_ParallelArray, 
     const intArray & Lhs_ParallelArray, 
     const doubleArray & Rhs_ParallelArray, 
     doubleSerialArray* This_Serial_PCE_Array, 
     intSerialArray* Lhs_Serial_PCE_Array, 
     doubleSerialArray* Rhs_Serial_PCE_Array, 
     const doubleSerialArray & This_SerialArray )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of #9 doubleArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,doubleArray,intArray,doubleArray,doubleSerialArray) \n");

  // This is the only test we can do on the input!
     This_ParallelArray.Test_Consistency ("Test in #9 doubleArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,doubleArray,intArray,doubleArray,doubleSerialArray)");
     Lhs_ParallelArray.Test_Consistency ("Test in #9 doubleArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,doubleArray,intArray,doubleArray,doubleSerialArray)");
     Rhs_ParallelArray.Test_Consistency ("Test in #9 doubleArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,doubleArray,intArray,doubleArray,doubleSerialArray)");
     This_SerialArray.Test_Consistency ("Test in #9 doubleArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,doubleArray,intArray,doubleArray,doubleSerialArray)");
#endif

     if (Index::Index_Bounds_Checking)
        {
          This_ParallelArray.Test_Conformability (Lhs_ParallelArray);
          This_ParallelArray.Test_Conformability (Rhs_ParallelArray);
#if EXTRA_ERROR_CHECKING
       // Redundent test
          Lhs_ParallelArray.Test_Conformability (Rhs_ParallelArray);
#endif
        }

#if USE_TEMPORARY_DELETE_FUNCTIONS
     Delete_If_Temporary ( Lhs_ParallelArray );
     Delete_If_Temporary ( Rhs_ParallelArray );
#endif

     doubleArray* Temporary = &((doubleArray &) This_ParallelArray);

  // Modify_Reference_Counts_And_Manage_Temporaries ( Temporary , Data );

  // This is now we acumulate the information used to determine message passing 
  // when the operator= is executed!
     if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info != NULL)
        {
          Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->decrementReferenceCount();
          if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount() < 
              Array_Conformability_Info_Type::getReferenceCountBase())
               delete Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info;
        }
     Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info = NULL;

#if 0
  // Bugfix (11/27/2000)
     Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info = Array_Set;
  // Bugfix (11/27/2000)
     Array_Set->incrementReferenceCount();
#endif

#if 0
  // Need to delete the Array_Set to avoid a memory leak
     Array_Set->decrementReferenceCount();
     if (Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
          delete Array_Set;
     Array_Set = NULL;
#endif

#if COMPILE_DEBUG_STATEMENTS
  // APP_ASSERT(Temporary->Array_Descriptor.Array_Domain.Is_A_Temporary == TRUE);
     Temporary->Test_Consistency ("Test (on return) in #9 doubleArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,doubleArray,intArray,doubleArray,doubleSerialArray)");
#endif

     return *Temporary;
   }

#endif

#undef DOUBLEARRAY

#define FLOATARRAY
floatArray *Last_Lhs_floatArray_Operand = NULL;

#if 0

#if !defined(GNU)
// inline
#endif
void
floatArray::Modify_Reference_Counts_And_Manage_Temporaries ( 
   Array_Conformability_Info_Type *Array_Set, 
   floatArray *Temporary, 
   floatSerialArray* Lhs_Serial_PCE_Array, 
   floatSerialArray* Rhs_Serial_PCE_Array, 
   floatSerialArray *Data )
   {
  // The following is a confusing point of P++ reference count management.  It was (is)
  // the source of many memory leaks that were (are) difficult to track down.

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of floatArray::Modify_Reference_Counts_And_Manage_Temporaries (Lhs,Rhs) \n");
#endif
  // This means that the Lhs_Serial_PCE_Array and Rhs_Serial_PCE_Array 
  // are both views built from Build_Pointer_To_View_Of_Array
  // bool Lhs_Is_A_View = TRUE;
  // bool Rhs_Is_A_View = TRUE;

     bool Lhs_Serial_PCE_Array_Reused = (Data == Lhs_Serial_PCE_Array);
     bool Rhs_Serial_PCE_Array_Reused = (Data == Rhs_Serial_PCE_Array);

     bool New_Array_Used = (Lhs_Serial_PCE_Array_Reused == FALSE) &&
                              (Rhs_Serial_PCE_Array_Reused == FALSE);

     if (New_Array_Used)
        {
       // We don't have to do anything the reference count of the Lhs_Serial_PCE_Array
       // and Rhs_Serial_PCE_Array should be 1 (or greater).
#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 0)
               printf ("New_Array_Used == TRUE \n");
#endif
       // In this case the serial binary operation build a new temporary to return (Lhs was not reused)
       // We delete the raw data because the serial array shell is deleted elsewhere?
#if 1
       // Test (11/10/2000) trying to fix reference counting mechanism
       // Temporary->Array_Descriptor.SerialArray->Delete_Array_Data();
          APP_ASSERT (Temporary != NULL);
          APP_ASSERT (Temporary->Array_Descriptor.SerialArray != NULL);
          APP_ASSERT (Temporary->Array_Descriptor.SerialArray->getReferenceCount() >= floatArray::getReferenceCountBase());
          Temporary->Array_Descriptor.SerialArray->decrementReferenceCount();
          if (Temporary->Array_Descriptor.SerialArray->getReferenceCount() < floatArray::getReferenceCountBase())
               delete Temporary->Array_Descriptor.SerialArray;
#endif
       // APP_ASSERT (Lhs_Serial_PCE_Array->getRawDataReferenceCount() > 0);
       // APP_ASSERT (Rhs_Serial_PCE_Array->getRawDataReferenceCount() > 0);
        }
       else
        {
#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 0)
               printf ("New_Array_Used == FALSE \n");
#endif
       // While we have to increment the SerialArray's reference count so that the delete in the 
       // binary operator will not destroy the SerialArray (this is because the Lhs_SerialArray and the 
       // result of the Serail array binary operation are the same array -- because the Lhs 
       // was resused) --- we have to decrement the RawDataReferenceCount
       // because we want to use the raw data obtained though the view taken in PCE and
       // we will not be using the original array from which that view was taken.
       // Data->decrementRawDataReferenceCount();

       // This code construction concerns me since it still does not seem that we should
       // be checking for the Data->getRawDataReferenceCount() > 0.

          printf ("##### Questionable decrement of the reference count if it is greater than the DataReferenceCountBase \n");
          if (Data->getRawDataReferenceCount() > getRawDataReferenceCountBase())
               Data->decrementRawDataReferenceCount();
          Data->incrementReferenceCount();
        }

  // Copy the input serialArray into the temporary (the return value for the calling (parent) functions)
     Temporary->Array_Descriptor.SerialArray = Data;
   }

#endif

// **************************************************************************
// **************************************************************************
//                   ABSTRACT OPERATORS SPECIFIC TO P++
//           (follows same logic as in A++ abstract operators)
//     (where statement logic is handled at the lower Serial_A++ level)
// **************************************************************************
// **************************************************************************
     
// **********************************************************
// The equals operator taking a scalar
// **********************************************************
floatArray &
floatArray::operator= ( float x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of floatArray::operator=(float x)! \n");

  // This is the only test we can do on the input!
     Test_Consistency ("Test in floatArray::operator= (float x)");
#endif

     APP_ASSERT(Array_Descriptor.SerialArray != NULL);
  // APP_ASSERT(Array_Descriptor.SerialArray->Array_Descriptor != NULL);

  // The SerialArray is a view of the correct portion of the raw data
  // so we can just call the SerialArray (A++) assignment operator.
  /* ...(5/5/98, kdb) WRONG: if there is a where mask this doesn't work ... */
  //   (*Array_Descriptor.SerialArray) = x;

     floatSerialArray *This_SerialArray = NULL;
     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing)
               Array_Set = floatArray::Parallel_Indirect_Conformability_Enforcement (*this, This_SerialArray);
            else
               Array_Set = floatArray::Parallel_Conformability_Enforcement (*this, This_SerialArray);
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->Array_Descriptor.SerialArray;

          if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE)
               Array_Set = floatArray::Parallel_Indirect_Conformability_Enforcement
                    (*this, This_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
            else
               Array_Set = floatArray::Parallel_Conformability_Enforcement
                                (*this, This_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
          Where_Statement_Support::Where_Statement_Mask->Array_Descriptor.SerialArray = Mask_SerialArray;
        }

  // if (Array_Set == Null)
  //      Array_Set = new Array_Conformability_Info_Type();

  // APP_ASSERT(Array_Set != NULL);
     APP_ASSERT(This_SerialArray != NULL);

     *This_SerialArray = x;

     if (Where_Statement_Support::Where_Statement_Mask != NULL)
          Where_Statement_Support::Where_Statement_Mask->Array_Descriptor.SerialArray = Old_Mask_SerialArray;

#if defined(PPP) && defined(USE_PADRE)
     setLocalDomainInPADRE_Descriptor(NULL);
#endif

     APP_ASSERT (This_SerialArray->getReferenceCount() >= floatArray::getReferenceCountBase());
     This_SerialArray->decrementReferenceCount();
     if (This_SerialArray->getReferenceCount() < floatArray::getReferenceCountBase())
          delete This_SerialArray;
 
     if (Mask_SerialArray != NULL)
        {
#if defined(PPP) && defined(USE_PADRE)
          setLocalDomainInPADRE_Descriptor(NULL);
#endif
          APP_ASSERT (Mask_SerialArray->getReferenceCount() >= intArray::getReferenceCountBase());
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intArray::getReferenceCountBase())
               delete Mask_SerialArray;
        }

     This_SerialArray = NULL;
     Mask_SerialArray = NULL;

#if 1
     if (Communication_Manager::Number_Of_Processors > 1)
        {
          if (Array_Set != NULL)
             {
               if (Array_Set->Full_VSG_Update_Required == FALSE)
                  {
                    int update_any = FALSE;
                    int i;
                    for (i=0; i < MAX_ARRAY_DIMENSION; i++)
                         if ((Array_Set->Update_Left_Ghost_Boundary_Width [i] >0) ||
                             (Array_Set->Update_Right_Ghost_Boundary_Width[i] >0) )
                              update_any = TRUE;
                    if (update_any)
                       {
#if COMPILE_DEBUG_STATEMENTS
                         for (i =0; i < MAX_ARRAY_DIMENSION; i++)
                            {
                              if ((Array_Set->Update_Left_Ghost_Boundary_Width [i] >0) ||
                                  (Array_Set->Update_Right_Ghost_Boundary_Width[i] >0) )
                                 {
                                   if (Array_Descriptor.Array_Domain.InternalGhostCellWidth[i] == 0)
                                      {
                                        printf("ERROR: No ghost cells along axis %d to ", i);
                                        printf("support message passing \n");
                                        APP_ABORT();
                                      }
                                 }
                              if (Array_Descriptor.isLeftPartition(i))
                                 {
                                   if (Array_Set->Update_Left_Ghost_Boundary_Width [i] >0) 
                                      {
                                        printf("ERROR: Left processor shouldn't try to update ");
                                        printf("left ghost boundary on dim %d.\n",i);
                                      }
                                   APP_ASSERT (Array_Set->Update_Left_Ghost_Boundary_Width[i] == 0);
                                 }
                              if (Array_Descriptor.isRightPartition(i))
                                 {
                                   if (Array_Set->Update_Right_Ghost_Boundary_Width [i] >0) 
                                      {
                                        printf("ERROR: Right processor shouldn't try to update ");
                                        printf("right ghost boundary on dim %d.\n",i);
                                      }
                                   APP_ASSERT (Array_Set->Update_Right_Ghost_Boundary_Width[i] == 0);
                                 }
                            }
#endif
                         if (Array_Descriptor.Array_Domain.Array_Conformability_Info != NULL)
                            {
#if COMPILE_DEBUG_STATEMENTS
                              if (APP_DEBUG > 3)
                                 {
                                   printf ("Inside of operator=(scalar): Array_Conformability_Info->getReferenceCount() = %d \n",
                                        Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount());
                                 }
#endif
                              APP_ASSERT (Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount() >= 
                                   Array_Conformability_Info_Type::getReferenceCountBase());
                              Array_Descriptor.Array_Domain.Array_Conformability_Info->decrementReferenceCount();
                              if (Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount() <
                                  Array_Conformability_Info_Type::getReferenceCountBase())
                                 {
                                   delete Array_Descriptor.Array_Domain.Array_Conformability_Info;
                                 }
                            }
                         Array_Descriptor.Array_Domain.Array_Conformability_Info = Array_Set;

                      // Test added (11/26/2000)
                         APP_ASSERT (Array_Descriptor.Array_Domain.Array_Conformability_Info != NULL);

                      // We should modify this so that the ghost boundaries are set properly without communication
                         if( automaticCommunication ) updateGhostBoundaries();
                         Array_Descriptor.Array_Domain.Array_Conformability_Info = NULL;
                       }
                  }
                 else
                  {
                 // Full VSG_Update is required 
                    if( automaticCommunication ) updateGhostBoundaries();
                  }
             }
            else
             {
            // Array_Set is null because indirect addressing is used;
               if( automaticCommunication ) updateGhostBoundaries();
             }
        } // end of (Number_Of_Processors > 1)
#endif

     if (Array_Set != NULL)
        {
#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 3)
             {
               printf ("Inside of ~Array_Domain_Type: Array_Set->getReferenceCount() = %d \n",
                    Array_Set->getReferenceCount());
             }
#endif
          APP_ASSERT (Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
          Array_Set->decrementReferenceCount();
          if (Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
               delete Array_Set;
          Array_Set = NULL;
        }

#if COMPILE_DEBUG_STATEMENTS
  // APP_ASSERT(Array_Descriptor.Array_Domain.Is_A_Temporary == FALSE);
     Test_Consistency ("Test (on return) in floatArray::operator= (float x)");
#endif

#if USE_TEMPORARY_DELETE_FUNCTIONS
  // This would be unlikely but we have to make sure
     Delete_Lhs_If_Temporary ( *this );
#endif

     return *this;
   }

// **********************************************************
// The other equals operator taking an array object
// **********************************************************
floatArray &
floatArray::operator= ( const floatArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if ( (APP_DEBUG > 1) || (Diagnostic_Manager::getReferenceCountingReport() > 0) )
        {
          printf ("\n\n\n@@@@@ Inside of floatArray::operator=(const floatArray & Rhs) (id=%d) = (id=%d) \n",Array_ID(),Rhs.Array_ID());
          this->displayReferenceCounts("Lhs in floatArray & operator=(floatArray,floatArray)");
          Rhs.displayReferenceCounts("Rhs in floatArray & operator=(floatArray,floatArray)");
       }

  // printf ("In floatArray::operator=(Rhs): Rhs.getReferenceCount() = %d \n",Rhs.getReferenceCount());
  // printf ("In floatArray::operator=(Rhs): Rhs.Array_Descriptor.SerialArray->getReferenceCount() = %d \n",
  //      Rhs.Array_Descriptor.SerialArray->getReferenceCount());

  // Temp code to debug reference count problem!
  // Rhs.view ("view Rhs in floatArray::operator= (const floatArray & Rhs)");

  // This is the only test we can do on the input!
  // printf ("Test_Consistency the input array objects \n");
     Test_Consistency ("Test Lhs in floatArray::operator= (const floatArray & Rhs)");
     Rhs.Test_Consistency ("Test Rhs in floatArray::operator= (const floatArray & Rhs)");

#if 0
#if COMPILE_DEBUG_STATEMENTS
     if (Communication_Manager::localProcessNumber() == 0)
        {
          Array_Descriptor.Array_Domain.display("Lhs in operator=");
        }
#endif
#endif

     if (APP_DEBUG > 3)
        {
          printf ("View the input array objects \n");
          view ("view Lhs in floatArray::operator= (const floatArray & Rhs)");
          Rhs.view ("view Rhs in floatArray::operator= (const floatArray & Rhs)");
       // printf ("Exiting at TOP of floatArray::operator= ( const floatArray & Rhs ) ... \n");
       // APP_ABORT();
        }
#endif

     if (Index::Index_Bounds_Checking)
          Test_Conformability (Rhs);

  // view("THIS inside of operator=");
  // Rhs.view("Rhs inside of operator=");

     floatSerialArray** Lhs_SerialArray_Pointer = &Array_Descriptor.SerialArray;
     floatSerialArray** Rhs_SerialArray_Pointer = &(((floatArray &)Rhs).Array_Descriptor.SerialArray);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          this->displayReferenceCounts("THIS in floatArray::operator= (floatArray)");
          Rhs.displayReferenceCounts("Rhs in floatArray::operator= (floatArray)");
        }
#endif

  // reorder these tests for better efficiency (can it be improved?)
  // if ( (Array_Descriptor.Array_Reference_Count_Array[Array_Descriptor.Array_ID()] == 0) &&
  //       Binary_Conformable (Rhs) &&
  //       Rhs.Array_Descriptor.Array_Domain.Is_A_Temporary &&
  //      !Rhs.Array_Descriptor.Array_Domain.Is_A_View )
  /* bool Case_1 = (Array_Descriptor.Array_Reference_Count_Array[Array_Descriptor.Array_ID()] == 0) && */
  // ... (5/8/97, kdb) make sure this isn't a view also ...
     bool Case_1 = (getRawDataReferenceCount() == getRawDataReferenceCountBase()) &&
                       !isView() &&
                       Binary_Conformable (Rhs) &&
                       Rhs.isTemporary() &&
                      !Rhs.isView();
  // this case is specific to P++ because temporaries are not condensed in the assignment to the
  // Lhs if that Lhs is a Null Array.  This is an internal difference between A++ and P++.
  // It is done to permit the distribution to be identical for the Lhs instead of being remapped.
  // ... (5/30/98, kdb) this cann't be a null array and a view without problems ...
     bool Case_2 = (getRawDataReferenceCount() == getRawDataReferenceCountBase()) && 
                       Rhs.isTemporary() && isNullArray() && !isView(); 
     if (Case_1 || Case_2)
        {
       // Give back the original Array_Data memory (but only if it exists)!
       // But it should always exist so we don't really have the check!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
               printf ("Skip the assignment by stealing the data (delete the Lhs's data if not a NullArray)! \n");
#endif

       // Handle the case of assignment to a NULL array (i.e. undimensioned array object
       // or an array object dimensioned to length zero).
          if (isNullArray())
             {
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Lhs is a Null Array so copy descriptor before assignment! \n");
#endif

            // Bug Fix (8/17/94) Null array must have a properly computed array descriptor
            // APP_ASSERT( Array_Descriptor != NULL );
            // Array_Descriptor.ReferenceCountedDelete();
            // delete Array_Descriptor;

            // We can't steal the descriptor because the descriptor contains the Array_ID and it
            // would have to be different for each of the Lhs and Rhs array objects.
            // The Array_ID should be in the array objects.
               Array_Descriptor.Build_Temporary_By_Example ( Rhs.Array_Descriptor ); 
               Array_Descriptor.Array_Domain.Is_A_Temporary = FALSE;
             }
            else
             {
            // Bug fix for nonzero Lhs base -- the Rhs is a temporary and so it has a zero base and the A++
            // assignment is never called so the serial array we will substitute has zero base.  
            // So we have to fix the base of the rhs we will steal and place on the Lhs.
	    // ... (9/27/96, kdb) other values need to be set correctly also ...
            // BTNG APP_ASSERT ( MAX_ARRAY_DIMENSION == 4);
	       int temp;
               for (temp=0; temp < MAX_ARRAY_DIMENSION; temp++)
                  {
                    Rhs.Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain.Data_Base[temp] = 
                         Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain.Data_Base[temp];
                    Rhs.Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain.User_Base[temp] = 
                         Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain.User_Base[temp];
                    Rhs.Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain.Scalar_Offset[temp] = 
                         Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain.Scalar_Offset[temp];
                  }
             }

       // Why is this a warning!
          APP_ASSERT ( getRawDataReferenceCount() == getRawDataReferenceCountBase() );
          if ( getRawDataReferenceCount() > getRawDataReferenceCountBase() )
             {
               printf ("WARNING: Array_ID = %d getRawDataReferenceCount() = %d \n",
                    Array_Descriptor.Array_ID(),getRawDataReferenceCount() );
             }
          APP_ASSERT(getRawDataReferenceCount() == getRawDataReferenceCountBase());

       // printf ("Before Delete_Array_Data -- Skip in floatArray::operator=(Rhs): Rhs.getReferenceCount() = %d \n",Rhs.getReferenceCount());
       // printf ("Before Delete_Array_Data -- Skip in floatArray::operator=(Rhs): Rhs.Array_Descriptor.SerialArray->getReferenceCount() = %d \n",
       //      Rhs.Array_Descriptor.SerialArray->getReferenceCount());

       // view ("View of THIS from operator=");
          Delete_Array_Data ();
          *Lhs_SerialArray_Pointer = NULL;

       // APP_ASSERT(Array_Descriptor.Array_Domain.Array_Reference_Count_Array[Array_Descriptor.Array_ID()] == 
       //   floatArray_Descriptor_Type
       //   ::Array_Reference_Count_Array[Array_Descriptor.Array_ID()] );

       // Since we stold the data we can zero the reference count (part of breaking the reference)
       // APP_ASSERT( floatArray_Descriptor_Type::Array_Reference_Count_Array[Array_Descriptor.Array_ID()] >= 0 );
       // if ( floatArray_Descriptor_Type::Array_Reference_Count_Array[Array_Descriptor.Array_ID()] >= 0 )
       // Array_Descriptor.Array_Reference_Count_Array[Array_Descriptor.Array_ID()] = 0;

          APP_ASSERT(*Lhs_SerialArray_Pointer == NULL);
       // Call the Serial_A++ assignment operator
          *Lhs_SerialArray_Pointer = *Rhs_SerialArray_Pointer;
          APP_ASSERT(*Lhs_SerialArray_Pointer != NULL);
          (*Lhs_SerialArray_Pointer)->Array_Descriptor.Array_Domain.Is_A_Temporary = FALSE;

       // increment the reference count so that after the Rhs is deleted we will still have the
       // serial array around on the Lhs.
          incrementRawDataReferenceCount();
          APP_ASSERT( getRawDataReferenceCount() > getRawDataReferenceCountBase());

#if 0
          printf ("After incrementRawDataReferenceCount() -- Skip in floatArray::operator=(Rhs): Rhs.getReferenceCount() = %d \n",Rhs.getReferenceCount());
          printf ("After incrementRawDataReferenceCount() -- Skip in floatArray::operator=(Rhs): Rhs.Array_Descriptor.SerialArray->getReferenceCount() = %d \n",Rhs.Array_Descriptor.SerialArray->getReferenceCount());
          printf ("After incrementRawDataReferenceCount() -- Skip in floatArray::operator=(Rhs): Rhs.getRawDataReferenceCount() = %d \n",Rhs.getRawDataReferenceCount());
#endif

       // We want the Rhs to have a serial array when the Rhs is deleted as a temporary
       // if the reference counts are greater then 1 then the Lhs will be left with the data
       // after the Rhs is deleted!
       // *Rhs_SerialArray_Pointer = NULL;

       // Bugfix (12/19/94) Force base of SerialArray equal to the value in the P++ descriptor
          APP_ASSERT(Array_Descriptor.SerialArray != NULL);

       // (4/30/97,kdb) Reinitialize the Serial Array_View_Pointers because 
       // SerialArray has been copied from Rhs

	  SERIAL_POINTER_LIST_INITIALIZATION_MACRO;
        }
       else 
        {
       // Do the assignment the hard way (not so hard 
       // since we just call the A++ assignment operator)
#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
               printf ("Do the assignment by calling the Serial_A++ assignment function! \n");
#endif

       // The more complex logic contained in the Serial_A++ implementation can be skipped
       // since we are just calling the Serial_A++ assignment operator and so it is included
       // this means that cases such as A(I) = A(I+1) are properly handled
          APP_ASSERT(Array_Descriptor.SerialArray != NULL);
          APP_ASSERT(Rhs.Array_Descriptor.SerialArray != NULL);

       // We want to allow NULL operations to avoid building new Null arrays for the Lhs
       // So don't allow the Lhs to be rebuilt if the Rhs is a Null array!
       // if (Array_Descriptor.Array_Domain.Is_A_Null_Array)
       // if (Array_Descriptor.Array_Domain.Is_A_Null_Array && !Rhs.Array_Descriptor.Array_Domain.Is_A_Null_Array)
          if (isNullArray() && !Rhs.isNullArray())
             {
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Lhs is a Null Array! \n");
#endif

            // printf ("################################################################## \n");
            // printf ("Case of Lhs being a NULL ARRAY and Rhs is a VIEW -- commented out! \n");
            // printf ("################################################################## \n");
            // The problem here is that the view Lhs is not being build to be the correct size
            // to work well in the parallel environment.  The lhs in this case should be built
            // to be the size of the rhs array data assuming no view.  I.e. the lhs should be
            // the same size as what the rhs is a view of so that the partitioning of the 
            // lhs and the rhs will be alligned (when we get to the serial operator= below).
               bool HANDLE_CASE_OF_VIEW = FALSE;
               if ( Rhs.isView() && HANDLE_CASE_OF_VIEW )
                  {
                 // Force Lhs to be the size of the referenced part of the Rhs
                 // This implies a default partitioning when used with P++.
                 // printf ("Rhs.Array_Descriptor.Array_Domain.Is_A_View == TRUE -- Building a new descriptor from scratch! \n");
                 // int i = Rhs.Array_Descriptor.getLength(0);
                 // int j = Rhs.Array_Descriptor.getLength(1);
                 // int k = Rhs.Array_Descriptor.getLength(2);
                 // int l = Rhs.Array_Descriptor.getLength(3);

                    Integer_Array_MAX_ARRAY_DIMENSION_Type Integer_List;
		    int temp;
                    for (temp=0; temp < MAX_ARRAY_DIMENSION; temp++)
                         Integer_List[temp] = Rhs.Array_Descriptor.getLength(temp);

                 // This is NOT EFFICIENT -- but I gather that this case does not happen
                 // sense HANDLE_CASE_OF_VIEW is FALSE!
                 // Array_Descriptor = new floatArray_Descriptor_Type(i,j,k,l);
                 // Array_Descriptor = 
                 //  new floatArray_Descriptor_Type (Integer_List);
                    Array_Descriptor = floatArray_Descriptor_Type
		                          (MAX_ARRAY_DIMENSION,Integer_List);

                    Array_Descriptor.display("Exiting in case of null Lhs of operator= ");
                    printf ("Exiting in case of null Lhs of operator= \n");
                    APP_ABORT();
                  }
                 else
                  {
                 // This is just calling the Array_Descriptor_Type's copy constructor so we have to
                 // modify the descriptor in the case it is a view.
                 // printf ("Rhs.Array_Descriptor.Array_Domain.Is_A_View == FALSE -- Building a copy of the Rhs descriptor! \n");

                 // If the Rhs is a Null Array then we should never have gotten to this point in the code!
                    APP_ASSERT( Rhs.isNullArray() == FALSE );

                 // This copies the domain in Rhs into the domain in the Lhs except that
                 // subsequently the bases are set to the default A++/P++ base (typically zero).
                    Array_Descriptor.Build_Temporary_By_Example ( Rhs.Array_Descriptor );

                 // ... change (10/29/96,kdb) set bases to Rhs bases because 
                 // Build_Temporary_By_Example sets the bases to zero 
                 // after copying Rhs and in the case of a null Lhs we now 
                 // want the bases of Lhs and Rhs to be the same ...

                 // ... set bases to Rhs bases ...
                 // Now we reset the base to be that of the Rhs (perhaps we should have just copied the
                 // domain explicitly -- since we copied the Rhs domain reset the base and then set the
                 // base again).
	            int nd = 0; 
                    for (nd=0;nd<MAX_ARRAY_DIMENSION;nd++)                    
		         setBase(Rhs.Array_Descriptor.getRawBase(nd),nd); 

                    setTemporary(FALSE);

#if COMPILE_DEBUG_STATEMENTS
                    if (APP_DEBUG > 5)
                       {
                         Rhs.Array_Descriptor.display("In operator= display (Rhs.Array_Descriptor)");
                         Array_Descriptor.display("In operator= display (Array_Descriptor)");
                       }
#endif

                    if (Array_Descriptor.Array_Domain.Array_Conformability_Info != NULL)
                       {
#if COMPILE_DEBUG_STATEMENTS
                         if (APP_DEBUG > 3)
                            {
                              printf ("Inside of operator=(Rhs): Array_Conformability_Info->getReferenceCount() = %d \n",
                                   Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount());
                            }
#endif
                         APP_ASSERT (Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount() >= 
                                     Array_Conformability_Info_Type::getReferenceCountBase());
                         Array_Descriptor.Array_Domain.Array_Conformability_Info->decrementReferenceCount();
                         if (Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount() 
                              < Array_Conformability_Info_Type::getReferenceCountBase())
                            {
                              delete Array_Descriptor.Array_Domain.Array_Conformability_Info;
                            }
                         Array_Descriptor.Array_Domain.Array_Conformability_Info = NULL;
                       }

                    APP_ASSERT( Array_Descriptor.Array_Domain.Array_Conformability_Info == NULL );
                 // Array_Descriptor.display ("CASE OF NULL LHS OF OPERATOR=");
                  }

               if (HANDLE_CASE_OF_VIEW)
                  {
                    APP_ASSERT( isView() == FALSE );
                  }

#if 0
#if !defined(USE_PADRE)
            // attempt at bug fix (6/10/2000)
               APP_ASSERT (Array_Descriptor.Array_Domain.BlockPartiArrayDomain == NULL);
               APP_ASSERT (Array_Descriptor.Array_Domain.BlockPartiArrayDecomposition == NULL);
#endif
#endif

            // Now allocate the data for the Lhs (so we can copy the Rhs into the Lhs)
            // printf ("Allocating data for local array on Lhs (Lhs was a null array)! \n");
               Allocate_Array_Data(TRUE);

            // printf ("Exiting inside of operator= \n");
            // APP_ABORT();
             }

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
               printf ("Now call the Serial_A++ assignment operator! \n");
       // Communication_Manager::Sync();
#endif

          APP_ASSERT(isTemporary() == FALSE);
          APP_ASSERT(Array_Descriptor.SerialArray != NULL);
          APP_ASSERT(Array_Descriptor.SerialArray->isTemporary() == FALSE);

       // Figure out the parallel communication required and form the conformable 
       // views of the SerialArray objects then call the SerialArray assignment 
       // operator.

          floatSerialArray  *This_SerialArray     = NULL;
          floatSerialArray  *Rhs_SerialArray      = NULL;
	  intSerialArray *Mask_SerialArray     = NULL;
	  intSerialArray *Old_Mask_SerialArray = NULL;

          Array_Conformability_Info_Type *Array_Set = NULL;
          if (Where_Statement_Support::Where_Statement_Mask == NULL)
             {
            // printf ("No Where Mask: Calling floatArray::Parallel_Indirect_Conformability_Enforcement \n");
            // printf ("Exiting BEFORE floatArray::Parallel_Conformability_Enforcement in operator= \n");
            // APP_ABORT();

               if ((usesIndirectAddressing() == TRUE) || (Rhs.usesIndirectAddressing() == TRUE))
                  {
                    Array_Set = floatArray::Parallel_Indirect_Conformability_Enforcement ( *this, This_SerialArray, Rhs, Rhs_SerialArray );
                  }
                 else
                  {
                    Array_Set = floatArray::Parallel_Conformability_Enforcement ( *this, This_SerialArray, Rhs, Rhs_SerialArray );
                  }
             }
            else
             {
            // ... (4/10/97, kdb) save old serial where mask because
            // Serial_Where_Statement_Mask has to be temporarily 
            // reset to make where work ...

               Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->Array_Descriptor.SerialArray;

               if ((usesIndirectAddressing() == TRUE) || (Rhs.usesIndirectAddressing() == TRUE))
                  {
                    printf ("Sorry, not implemented: can't mix indirect addressing (case of where and 2 array objects).\n");
                    APP_ABORT();
                  }
                 else
                  {
                    Array_Set = floatArray::Parallel_Conformability_Enforcement
                         (*this, This_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray, Rhs, Rhs_SerialArray );
                  }

            // ...  this will be reset later ...
               Where_Statement_Support::Where_Statement_Mask->Array_Descriptor.SerialArray = Mask_SerialArray; 
             }

       // printf ("DONE: Check Where_Statement_Support::Where_Statement_Mask \n");

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 5)
             {
               Rhs_SerialArray->Array_Descriptor.display("In operator= AFTER ASSIGNMENT Rhs_SerialArray");
               This_SerialArray->Array_Descriptor.display("In operator= AFTER ASSIGNMENT This_SerialArray");
             }
#endif

       // printf ("Check Array_Set \n");

          if (Array_Set == NULL)
               Array_Set = new Array_Conformability_Info_Type();

       // Communication_Manager::Sync();
          APP_ASSERT(Array_Set        != NULL);
          APP_ASSERT(This_SerialArray != NULL);
          APP_ASSERT(Rhs_SerialArray  != NULL);

       // APP_ASSERT(Rhs_SerialArray->Array_Descriptor != NULL);

       // Call the A++ assignment operator!
#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("Call the A++ assignment operator from the P++ assignment operator ... \n");
            // This_SerialArray->view("In floatArray::operator= This_SerialArray");
            // Rhs_SerialArray ->view("In floatArray::operator= Rhs_SerialArray");
             }
#endif

          *This_SerialArray = *Rhs_SerialArray;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("DONE: Call the A++ assignment operator from the P++ assignment operator ... \n");
               this->displayReferenceCounts("After serial array assignment Lhs in floatArray & operator=(floatArray,floatArray)");
               This_SerialArray->displayReferenceCounts("After serial array assignment This_SerialArray");
               Rhs_SerialArray->displayReferenceCounts("After serial array assignment Rhs_SerialArray");
             }
#endif

       // Replace the where mask
	  if (Where_Statement_Support::Where_Statement_Mask != NULL)
               Where_Statement_Support::Where_Statement_Mask->Array_Descriptor.SerialArray = Old_Mask_SerialArray;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 6)
             {
               Rhs_SerialArray->view("In operator= AFTER ASSIGNMENT Rhs_SerialArray");
               This_SerialArray->view("In operator= AFTER ASSIGNMENT This_SerialArray");
               view("In operator= AFTER ASSIGNMENT *this");
             }
#endif

       // Bugfix (6/15/95) Memory in use reported by Markus (a similar problem 
       // should exist in all the binary operaotrs (so we will fix that next))
       // Now delete the This_SerialArray and Rhs_SerialArray obtained from
       // the floatArray::Parallel_Conformability_Enforcement

#if defined(PPP) && defined(USE_PADRE)
       // We have to remove the references in PADRE to the Serial_Array object
       // which is being deleted.  This is a consequence of P++ using PADRE in a way
       // so as to prevent the redundent storage of Array_Domain objects
       // (specifically we use PADRE in a way so that only references are stored).
          setLocalDomainInPADRE_Descriptor(NULL);
          Rhs.setLocalDomainInPADRE_Descriptor(NULL);
#endif

       // printf ("Now delete the This_SerialArray in the P++ operator= (floatArray) \n");

          This_SerialArray->decrementReferenceCount();
          if (This_SerialArray->getReferenceCount() < floatArray::getReferenceCountBase())
               delete This_SerialArray;

       // printf ("Now delete the Rhs_SerialArray in the P++ operator= (floatArray) \n");

       // (12/09/2000) Check for reuse of serialArray object in return value (do not delete it if it was reused)
          if (Rhs_SerialArray->isTemporary() == FALSE)
             {
               Rhs_SerialArray->decrementReferenceCount();
               if (Rhs_SerialArray->getReferenceCount() < floatArray::getReferenceCountBase())
                    delete Rhs_SerialArray;
             }

       // printf ("Now delete the Mask_SerialArray in the P++ operator= (floatArray) \n");

	  if (Mask_SerialArray != NULL) 
             {
#if defined(PPP) && defined(USE_PADRE)
               Where_Statement_Support::Where_Statement_Mask->setLocalDomainInPADRE_Descriptor(NULL);
#endif
               Mask_SerialArray->decrementReferenceCount();
               if (Mask_SerialArray->getReferenceCount() < intArray::getReferenceCountBase())
                    delete Mask_SerialArray;
             }

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 5)
             {
               view("In operator= AFTER ASSIGNMENT (AND CALL TO DELETE) *this");
             }
#endif
       // Now set these pointers to NULL since we don't use them any more
          This_SerialArray = NULL;
          Rhs_SerialArray  = NULL;
	  Mask_SerialArray = NULL;

       // printf ("Now do whatever message passing that is required in P++ operator= (floatArray) \n");

       // Now we need to do the required message passing to fixup the "this" array
       // first check to see if just updating the ghost boundaries is enough

          if (Communication_Manager::Number_Of_Processors > 1)
             {
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
	          {
                    printf ("All ghost boundaries updated together (not seperate treatment for each edge) \n");
                  }
#endif
               if (Array_Set != NULL)
	          {
                    if (Array_Set->Full_VSG_Update_Required == FALSE)
                       {
                         int update_any = FALSE;
                         int i;
                         for (i=0; i < MAX_ARRAY_DIMENSION; i++)
                            {
                              if ((Array_Set->Update_Left_Ghost_Boundary_Width [i] > 0) || 
		                  (Array_Set->Update_Right_Ghost_Boundary_Width[i] > 0)) 
	                           update_any = TRUE;		 
                            }

                         if (update_any)
                            {
#if COMPILE_DEBUG_STATEMENTS
                              if (APP_DEBUG > 0)
                                   printf ("UPDATING GHOST BOUNDARIES (Overlap Update)! \n");

                           // error checking
                              for (i=0; i < MAX_ARRAY_DIMENSION; i++)
                                 {
                                   if ((Array_Set->Update_Left_Ghost_Boundary_Width[i] > 0) || 
                                       (Array_Set->Update_Right_Ghost_Boundary_Width[i] > 0))
                                      {
                                        if (Array_Descriptor.Array_Domain.InternalGhostCellWidth[i] == 0)
                                           {
                                             printf ("ERROR: No ghost cells along axis %d to ",i);
                                             printf ("support message passing ");
                                             printf ("(Full_VSG_Update_Required should be TRUE ");
                                             printf ("but it is FALSE) \n");
                                             APP_ABORT();
                                           }
                                      }

                                // If we are the leftmost processor then it is an error to 
                                // pass boundary info the the left
                                   if (Array_Descriptor.isLeftPartition(i))
                                      {
                                        if (Array_Set->Update_Left_Ghost_Boundary_Width[i] != 0)
                                           {
                                             printf ("ERROR: Left side -- Axis i = %d \n",i);
                                             Array_Set->display("ERROR: Left processor should not pass left -- Array_Set->Update_Left_Ghost_Boundary_Width[i] == 0");
                                             view ("ERROR: Left processor should not pass left -- Array_Set->Update_Left_Ghost_Boundary_Width[i] == 0");
                                           }
                                        APP_ASSERT (Array_Set->Update_Left_Ghost_Boundary_Width[i] == 0);
                                      }

                                // If we are the rightmost processor then it is an error to 
                                // pass boundary info the the right
                                   if (Array_Descriptor.isRightPartition(i))
                                      {
#if COMPILE_DEBUG_STATEMENTS
                                        if (Array_Set->Update_Right_Ghost_Boundary_Width[i] != 0)
                                           {
                                             printf ("ERROR: Right side -- Axis i = %d \n",i);
                                             Array_Set->display("ERROR: Right processor should not pass right -- Array_Set->Update_Right_Ghost_Boundary_Width[i] == 0");
                                             view ("ERROR: Right processor should not pass right -- Array_Set->Update_Right_Ghost_Boundary_Width[i] == 0");
                                           }
#endif
                                        APP_ASSERT (Array_Set->Update_Right_Ghost_Boundary_Width[i] == 0);
                                      }
                                 }
#endif
                           // For now we update all ghost boundaries but later we can be 
                           // more efficient and selective
                           /*
                           // ... (12/11/96,kdb) now only the necessary ghost cells
                           // are updated.  The updateGhostBoundaries code needs
                           // Array_Set to do this.  To avoid passing in an
                           // extra parameter, temporarily attach this to
                           // Array_Descriptor since the Array_Conformability_Info
                           // will be deleted anyways right after this. ...
                           */
                              if (Array_Descriptor.Array_Domain.Array_Conformability_Info != NULL)
                                 {
                                   Array_Descriptor.Array_Domain.Array_Conformability_Info->decrementReferenceCount();
                                   if (Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount() < 
                                       intArray::getReferenceCountBase())
                                        delete Array_Descriptor.Array_Domain.Array_Conformability_Info;
                                 }

                           // ... don't need reference counting ...
                              Array_Descriptor.Array_Domain.Array_Conformability_Info = Array_Set;
                              updateGhostBoundaries();
                              Array_Descriptor.Array_Domain.Array_Conformability_Info = NULL;
                            }
                           else
                            {
                           // No updates specified 
#if COMPILE_DEBUG_STATEMENTS
                              if (APP_DEBUG > 0)
                                   printf ("NO UPDATE TO GHOST BOUNDARIES REQUIRED! \n");
#endif
                            }
                       }
                      else
                       {
                      // Full_VSG_Update_Required == TRUE

#if COMPILE_DEBUG_STATEMENTS
                         if (APP_DEBUG > 0)
                            {
                              printf ("FULL VSG update required (already done) (but now we ");
                              printf ("have to update the ghost boundaries) \n");
                            }
#endif
                      // Since the Regular Section transfer used in PARTI does not update
                      // the ghost boundaries we have to force the update here!
                         updateGhostBoundaries();
                       }
                  }
                 else
                  {
                 // Array_Set is null because indirect addressing was used. 
                 // Update ghost boundaries.
                 // printf ("Calling updateGhostBoundaries() for 1 processor \n");
                    updateGhostBoundaries();
                  }
             } // end of (Communication_Manager::Number_Of_Processors > 1)

       // printf ("DONE with communication for operator= \n");

       // Need to delete the Array_Set to avoid a memory leak
       // Array_Set can be null now if indirect addressing
       // APP_ASSERT (Array_Set != NULL);
          if (Array_Set != NULL);
	     {
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 3)
                  {
                    printf ("Inside of operator=(Rhs): Array_Set->getReferenceCount() = %d \n",
                         Array_Set->getReferenceCount());
                  }
#endif

#if 1
               APP_ASSERT (Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
               Array_Set->decrementReferenceCount();
               if (Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
                    delete Array_Set;
               Array_Set = NULL;
#endif
	     }
        }

#if COMPILE_DEBUG_STATEMENTS
     APP_ASSERT(Array_Descriptor.Array_Domain.Is_A_Temporary == FALSE);
  // printf ("Calling Test_Consistency for Lhs \n");
     Test_Consistency("Test (on return) in floatArray::operator= (const floatArray & Rhs)");
#endif

#if USE_TEMPORARY_DELETE_FUNCTIONS
  // Manage temporary if it appears on the Rhs or Lhs
  // printf ("In operator= (before Delete_If_Temporary): Rhs.getReferenceCount() = %d \n",Rhs.getReferenceCount());
  // printf ("In operator= (before Delete_If_Temporary): Rhs.Array_Descriptor.SerialArray->getReferenceCount() = %d \n",
  //      Rhs.Array_Descriptor.SerialArray->getReferenceCount());
     Delete_If_Temporary ( Rhs );

  // printf ("In floatArray::operator= Array_ID on the stack is %d \n",SerialArray_Domain_Type::queryNextArrayID());

  // printf ("In operator= (before Delete_Lhs_If_Temporary): Lhs.getReferenceCount() = %d \n",this->getReferenceCount());
     Delete_Lhs_If_Temporary ( *this );
#endif

     if (Array_Descriptor.Array_Domain.Array_Conformability_Info != NULL)
        {
          Array_Descriptor.Array_Domain.Array_Conformability_Info->decrementReferenceCount();
          if (Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount() <
              Array_Conformability_Info_Type::getReferenceCountBase())
               delete Array_Descriptor.Array_Domain.Array_Conformability_Info;
          Array_Descriptor.Array_Domain.Array_Conformability_Info = NULL;
        }

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          this->displayReferenceCounts("THIS at BASE of floatArray::operator= (floatArray)");
        }
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Return from floatArray::operator= \n");

  // Commented out (12/14/2000)
  // Communication_Manager::Sync();
#endif

     return *this;
   }

// ************************************************************************************
// This is the abstract operator used for reduction operators
// ************************************************************************************
float
floatArray::Abstract_Operator (
   Array_Conformability_Info_Type *Array_Set, const floatArray & X, 
   floatSerialArray* X_Serial_PCE_Array, float x, int Operation_Type )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of #1 floatArray::Abstract_Operator(Array_Conformability_Info_Type,floatArray,float,int) \n");

  // This is the only test we can do on the input!
     X.Test_Consistency ("Test in #1 floatArray::Abstract_Operator(Array_Conformability_Info_Type,floatArray,float,int)");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X at TOP of #1 floatArray::Abstract_Operator()");
       // X_Serial_PCE_Array->displayReferenceCounts("X_Serial_PCE_Array at TOP of #1 floatArray::Abstract_Operator()");
        }
#endif

  // error checking
     APP_ASSERT(Array_Set != NULL);

  // We have to do a communication on the group of procesors owning 
  // this P++ array object we skip the the communication for now.

#if defined(USE_PADRE)
  // What PADRE function should we call?
  // printf ("NEED TO CALL PADRE \n"); APP_ABORT();
     APP_ASSERT (X.Array_Descriptor.Array_Domain.parallelPADRE_DescriptorPointer != NULL);
#else
     APP_ASSERT (X.Array_Descriptor.Array_Domain.BlockPartiArrayDecomposition != NULL);
#endif
  // The local reduction has already been done and passed in as the scalar "x" so we only have
  // further reduction to do if there are more than 1 processors in the multiprocesor system
     if ( Communication_Manager::Number_Of_Processors > 1 )
        {
          Reduction_Operation ( Operation_Type , x );
        }

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X at BOTTOM of #1 floatArray::Abstract_Operator()");
        }
#endif

#if USE_TEMPORARY_DELETE_FUNCTIONS
     Delete_If_Temporary (X);
#endif

     return x;
   }
 

// ************************************************************************
// usual binary operators: operator+ operator- operator* operator/ operator% 
// for array to scalar operations
// ************************************************************************
floatArray &
floatArray::Abstract_Operator ( 
   Array_Conformability_Info_Type *Array_Set, 
   const floatArray & X_ParallelArray, 
   floatSerialArray* X_Serial_PCE_Array, 
   floatSerialArray & X_SerialArray )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Inside of #2 floatArray::Abstract_Operator(Array_Conformability_Info_Type,floatArray,floatSerialArray) \n");

  // This is the only test we can do on the input!
     X_ParallelArray.Test_Consistency ("Test X_ParallelArray in #2 floatArray::Abstract_Operator(Array_Conformability_Info_Type,floatArray,floatSerialArray)");
     X_SerialArray.Test_Consistency   ("Test X_SerialArray in #2 floatArray::Abstract_Operator(Array_Conformability_Info_Type,floatArray,floatSerialArray)");

  // X_ParallelArray.view("X_ParallelArray in #2 floatArray::Abstract_Operator(Array_Conformability_Info_Type,floatArray,floatSerialArray)");
  // printf ("X_ParallelArray.Array_Descriptor.SerialArray->getBase(0) = %d \n",X_ParallelArray.Array_Descriptor.SerialArray->getBase(0));
  // printf ("X_ParallelArray.Array_Descriptor.SerialArray->getBase(1) = %d \n",X_ParallelArray.Array_Descriptor.SerialArray->getBase(1));
  // printf ("X_ParallelArray.Array_Descriptor.SerialArray->getBase(2) = %d \n",X_ParallelArray.Array_Descriptor.SerialArray->getBase(2));
  // printf ("X_ParallelArray.Array_Descriptor.SerialArray->getBase(3) = %d \n",X_ParallelArray.Array_Descriptor.SerialArray->getBase(3));
  // X_SerialArray.view("X_SerialArray in #2 floatArray::Abstract_Operator(Array_Conformability_Info_Type,floatArray,floatSerialArray)");
#endif

  // error checking
     APP_ASSERT(Array_Set != NULL);

  // The Partitioning array is the array object (usually the Lhs operand of a binary operation) which the 
  // resulting temporary will be partitioned similar to (if it is required to build the temporary).
  // If a scalar was used on the Lhs then we have to use the Rhs as a Partitioning_floatArray
  // floatArray* Partitioning_Array = &((floatArray &) X_ParallelArray);
  // APP_ASSERT (Partitioning_Array != NULL);

  // The size of the temporary must be the same as that of the Lhs (Lhs based owners compute rule)
     floatSerialArray *Data = &((floatSerialArray &) X_SerialArray);
     APP_ASSERT(Data != NULL);

  // Data->view ("view Data (as X_SerialArray AT TOP) in #2 floatArray::Abstract_Operator(Array_Conformability_Info_Type,floatArray,floatSerialArray)");

  // The size of the temporary must be the same as that of the Lhs (Lhs based owners compute rule)
     floatArray* Temporary = NULL;

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X_SerialArray.displayReferenceCounts("X_SerialArray at TOP of #2 floatArray::Abstract_Operator()");
        }
#endif

     if ( X_ParallelArray.isTemporary() )
        {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Inside of #2 floatArray::Abstract_Operator(): Reuse X_ParallelArray \n");
#endif

       // Temporary reuse of serial array already handled at Serial_A++ level
          Temporary = &((floatArray &) X_ParallelArray);

       // Temporary->view ("view temporary (as X_ParallelArray BEFORE fixupLocalBase) in #2 floatArray::Abstract_Operator(Array_Conformability_Info_Type,floatArray,floatSerialArray)");

          Array_Domain_Type::fixupLocalBase 
	     ( (Array_Domain_Type&)X_ParallelArray.Array_Descriptor.Array_Domain, 
	       Data->Array_Descriptor.Array_Domain , 
               X_ParallelArray.Array_Descriptor.Array_Domain , 
               X_ParallelArray.Array_Descriptor.SerialArray->
	       Array_Descriptor.Array_Domain );

       // Bugfix (12/3/2000) we have to place the X_SerialArray into the Temporary
          Delete_If_Temporary (*X_ParallelArray.Array_Descriptor.SerialArray);
          ((floatArray &) X_ParallelArray).Array_Descriptor.SerialArray = Data;

       // Temporary->view ("view temporary (as X_ParallelArray BEFORE Modify_Reference_Counts_And_Manage_Temporaries) in #2 floatArray::Abstract_Operator(Array_Conformability_Info_Type,floatArray,floatSerialArray)");

       // Modify_Reference_Counts_And_Manage_Temporaries ( Array_Set, Temporary, X_Serial_PCE_Array, Data );
          APP_ASSERT (Data->isTemporary() == TRUE);

#if COMPILE_DEBUG_STATEMENTS
          if (Diagnostic_Manager::getReferenceCountingReport() > 0)
             {
            // This mechanism outputs reports which allow us to trace the reference counts
               Temporary->displayReferenceCounts("Temporary (after Modify_Reference_Counts_And_Manage_Temporaries) of #2 floatArray::Abstract_Operator()");
               X_SerialArray.displayReferenceCounts("X_SerialArray (after Modify_Reference_Counts_And_Manage_Temporaries) of #2 floatArray::Abstract_Operator()");
             }
#endif

       // Temporary->view ("view temporary (as X_ParallelArray AFTER MRCAMT) in #2 floatArray::Abstract_Operator(Array_Conformability_Info_Type,floatArray,floatSerialArray)");
       // APP_ABORT();

#if COMPILE_DEBUG_STATEMENTS
          X_ParallelArray.Test_Consistency ("Test X_ParallelArray (temporary) in #2 floatArray::Abstract_Operator(Array_Conformability_Info_Type,floatArray,floatSerialArray)");
#endif
       // Data->view("Data (temporary) in #2 floatArray::Abstract_Operator(Array_Conformability_Info_Type,floatArray,floatSerialArray)");
       // X_ParallelArray.view("X_ParallelArray in Abstract_Operator");
       // APP_ABORT();
        }
       else
        {
       // No temporary to reuse so we have to build one!

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Inside of #2 floatArray::Abstract_Operator(): Build a new array to be the temporary \n");
#endif

       // This is more efficient than the previous version which build a descriptor and
       // then forced 2 copies of the descriptor before ending up with a temporary to use.
          bool AvoidBuildingIndirectAddressingView = TRUE;
       // Data->view("In abstract_op #2: Data");
       // X_ParallelArray.Array_Descriptor.Array_Domain.display("In abstract_op #2: X_ParallelArray: Array_Domain");
          Temporary = new floatArray ( Data , 
                                    &(X_ParallelArray.Array_Descriptor.Array_Domain),
                                    AvoidBuildingIndirectAddressingView );
          APP_ASSERT (Temporary != NULL);
          APP_ASSERT (Temporary->Array_Descriptor.SerialArray != NULL);

       // printf ("In #2 floatArray::Abstract_Operator: Temporary->Array_Descriptor.SerialArray->getReferenceCount() = %d \n",
       //      Temporary->Array_Descriptor.SerialArray->getReferenceCount());
       // APP_ASSERT (Temporary->Array_Descriptor.SerialArray->getReferenceCount() > 1);

       // The base now needs to be reset since temporaries always have APP_Global_Array_Base as a base
       // ... don't reset this if indirect addressing is used ...
	  if (!Temporary->Array_Descriptor.Array_Domain.Uses_Indirect_Addressing)
             Temporary->setBase(APP_Global_Array_Base);
          
       // Temporary->view ("view temporary in #2 floatArray::Abstract_Operator(Array_Conformability_Info_Type,floatArray,floatSerialArray)");
       // APP_ABORT();

          Temporary->setTemporary(TRUE);

       /*
       // ... (4/28/98, kdb) added this function call, don't know what 
       //   happened to it ...
       // ... (4/29/98, kdb) not sure this is needed ...
       */

       /*
          Array_Domain_Type::fixupLocalBase
	     (Descriptor->Array_Domain, Data->Array_Descriptor.Array_Domain,
	      X_ParallelArray.Array_Descriptor.Array_Domain,
	      X_ParallelArray.Array_Descriptor.SerialArray->
	      Array_Descriptor.Array_Domain);
       */

       // (11/4/2000) I think this is always NULL for the temporary just built (verify)!
          APP_ASSERT (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info == NULL);

       // This is how we acumulate the information used to determine message passing 
       // when the operator= is executed (to terminate the expression statement)!
          if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info != NULL)
             {
            // printf ("Commented out Array_Conformability_Info_Type::delete \n");
               Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->decrementReferenceCount();
               if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount() < 
                   Array_Conformability_Info_Type::getReferenceCountBase())
                    delete Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info;
             }
          Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info = Array_Set;

       // Bugfix (11/27/2000)
          Array_Set->incrementReferenceCount();
        }

#if COMPILE_DEBUG_STATEMENTS
     APP_ASSERT(Temporary->Array_Descriptor.Array_Domain.Is_A_Temporary == TRUE);
     Temporary->Test_Consistency ("Test (on return) in #2 floatArray::Abstract_Operator(ACIT,floatArray,floatSerialArray)");
     if (APP_DEBUG > 5)
        {
          Temporary->view ("view temporary (on return) in #2 floatArray::Abstract_Operator(ACIT,floatArray,floatSerialArray)");
        }

     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Temporary->displayReferenceCounts("Temporary at BASE of #2 floatArray::Abstract_Operator()");
          X_SerialArray.displayReferenceCounts("X_SerialArray at BASE of #2 floatArray::Abstract_Operator()");
        }
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving #2 floatArray::Abstract_Operator(Array_Conformability_Info_Type,floatArray,floatSerialArray) \n");
#endif

     return *Temporary;
   }





#if !defined(INTARRAY)
// if !defined(INTARRAY)
// ************************************************************************
// usual binary operators: operator+ operator- operator* operator/ operator% 
// for array to scalar operations
// ************************************************************************
intArray & 
floatArray::Abstract_Operator (
     Array_Conformability_Info_Type *Array_Set, 
     const floatArray & X_ParallelArray, 
     floatSerialArray* X_Serial_PCE_Array,
     intSerialArray & X_SerialArray )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of #3 floatArray::Abstract_Operator(Array_Conformability_Info_Type,floatArray,intSerialArray) \n");

  // This is the only test we can do on the input!
     X_ParallelArray.Test_Consistency ("Test X_ParallelArray in #3 floatArray::Abstract_Operator(Array_Conformability_Info_Type,floatArray,intSerialArray) ");
     X_SerialArray.Test_Consistency ("Test X_SerialArray in #3 floatArray::Abstract_Operator(Array_Conformability_Info_Type,floatArray,intSerialArray) ");
#endif

  // error checking
     APP_ASSERT(Array_Set != NULL);

  // The size of the temporary must be the same as that of the Lhs (Lhs based owners compute rule)
     intSerialArray *Data = &((intSerialArray &) X_SerialArray);
     APP_ASSERT(Data != NULL);

     intArray *Temporary = NULL;
  // This is more efficient than the previous version which build a descriptor and
  // then forced 2 copies of the descriptor before ending up with a temporary to use.
     bool AvoidBuildingIndirectAddressingView = TRUE;
  // Data->view("In abstract_op #2: Data");
  // X_ParallelArray.Array_Descriptor.Array_Domain.display("In abstract_op #2: X_ParallelArray: Array_Domain");
     Temporary = new intArray ( Data , 
                               &(X_ParallelArray.Array_Descriptor.Array_Domain),
                               AvoidBuildingIndirectAddressingView );

     APP_ASSERT (Temporary != NULL);
     APP_ASSERT (Temporary->Array_Descriptor.SerialArray != NULL);
  // printf ("In #3 floatArray::Abstract_Operator: Temporary->Array_Descriptor.SerialArray->getReferenceCount() = %d \n",
  //      Temporary->Array_Descriptor.SerialArray->getReferenceCount());
  // APP_ASSERT (Temporary->Array_Descriptor.SerialArray->getReferenceCount() > 1);

  // The base now needs to be reset since temporaries always have APP_Global_Array_Base as a base
  // ... don't reset this if indirect addressing is used ...
     if (!Temporary->Array_Descriptor.Array_Domain.Uses_Indirect_Addressing)
        Temporary->setBase(APP_Global_Array_Base);
          
  // Temporary->view ("view temporary in #3 floatArray::Abstract_Operator(Array_Conformability_Info_Type,floatArray,floatSerialArray)");
  // APP_ABORT();

     Temporary->setTemporary(TRUE);

  // ... maybe this isn't needed with the new constructor ...

     /*
     Array_Domain_Type::fixupLocalBase
	( Descriptor->Array_Domain, Data->Array_descriptor.Array_Domain,
	  X_ParallelArray.Array_Descriptor.Array_Domain,
	  X_ParallelArray.Array_Descriptor.SerialArray->
	  Array_Descriptor.Array_Domain);
     */

  // Test (12/4/2000) I think this might be true!
     APP_ASSERT (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info == NULL);

  // This is now we acumulate the information used to determine message passing 
  // when the operator= is executed!
     if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info != NULL)
        {
       // printf ("Commented out Array_Conformability_Info_Type::delete \n");
          Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->decrementReferenceCount();
          if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount() < 
              Array_Conformability_Info_Type::getReferenceCountBase())
               delete Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info;
        }

     Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info = Array_Set;
  // Bugfix (11/27/2000)
     Array_Set->incrementReferenceCount();

#if COMPILE_DEBUG_STATEMENTS
     APP_ASSERT(Temporary->isTemporary() == TRUE);
     Temporary->Test_Consistency ("Test (on return) in #3 floatArray::Abstract_Operator(Array_Domain.Array_Conformability_Info_Type,floatArray,intSerialArray)");

  // Temporary->displayReferenceCounts("Called inside of #3 floatArray::Abstract_Operator");
  // X_SerialArray.displayReferenceCounts("X_SerialArray at BASE of #3 floatArray::Abstract_Operator()");
#endif

     return *Temporary;
   }
// endif



#endif



// if !defined(INTARRAY)
// ************************************************************************
// usual binary operators: operator+ operator- operator* operator/ operator% 
// for array to scalar operations
// ************************************************************************
intArray & 
floatArray::Abstract_int_Conversion_Operator (
     Array_Conformability_Info_Type *Array_Set, 
     const floatArray & X_ParallelArray, 
     floatSerialArray* X_Serial_PCE_Array,
     intSerialArray & X_SerialArray )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of #3 floatArray::Abstract_Operator(Array_Conformability_Info_Type,floatArray,intSerialArray) \n");

  // This is the only test we can do on the input!
     X_ParallelArray.Test_Consistency ("Test X_ParallelArray in #3 floatArray::Abstract_Operator(Array_Conformability_Info_Type,floatArray,intSerialArray) ");
     X_SerialArray.Test_Consistency ("Test X_SerialArray in #3 floatArray::Abstract_Operator(Array_Conformability_Info_Type,floatArray,intSerialArray) ");
#endif

  // error checking
     APP_ASSERT(Array_Set != NULL);

  // The size of the temporary must be the same as that of the Lhs (Lhs based owners compute rule)
     intSerialArray *Data = &((intSerialArray &) X_SerialArray);
     APP_ASSERT(Data != NULL);

     intArray *Temporary = NULL;
  // This is more efficient than the previous version which build a descriptor and
  // then forced 2 copies of the descriptor before ending up with a temporary to use.
     bool AvoidBuildingIndirectAddressingView = TRUE;
  // Data->view("In abstract_op #2: Data");
  // X_ParallelArray.Array_Descriptor.Array_Domain.display("In abstract_op #2: X_ParallelArray: Array_Domain");
     Temporary = new intArray ( Data , 
                               &(X_ParallelArray.Array_Descriptor.Array_Domain),
                               AvoidBuildingIndirectAddressingView );

     APP_ASSERT (Temporary != NULL);
     APP_ASSERT (Temporary->Array_Descriptor.SerialArray != NULL);
  // printf ("In #3 floatArray::Abstract_Operator: Temporary->Array_Descriptor.SerialArray->getReferenceCount() = %d \n",
  //      Temporary->Array_Descriptor.SerialArray->getReferenceCount());
  // APP_ASSERT (Temporary->Array_Descriptor.SerialArray->getReferenceCount() > 1);

  // The base now needs to be reset since temporaries always have APP_Global_Array_Base as a base
  // ... don't reset this if indirect addressing is used ...
     if (!Temporary->Array_Descriptor.Array_Domain.Uses_Indirect_Addressing)
        Temporary->setBase(APP_Global_Array_Base);
          
  // Temporary->view ("view temporary in #3 floatArray::Abstract_Operator(Array_Conformability_Info_Type,floatArray,floatSerialArray)");
  // APP_ABORT();

     Temporary->setTemporary(TRUE);

  // ... maybe this isn't needed with the new constructor ...

     /*
     Array_Domain_Type::fixupLocalBase
	( Descriptor->Array_Domain, Data->Array_descriptor.Array_Domain,
	  X_ParallelArray.Array_Descriptor.Array_Domain,
	  X_ParallelArray.Array_Descriptor.SerialArray->
	  Array_Descriptor.Array_Domain);
     */

  // Test (12/4/2000) I think this might be true!
     APP_ASSERT (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info == NULL);

  // This is now we acumulate the information used to determine message passing 
  // when the operator= is executed!
     if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info != NULL)
        {
       // printf ("Commented out Array_Conformability_Info_Type::delete \n");
          Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->decrementReferenceCount();
          if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount() < 
              Array_Conformability_Info_Type::getReferenceCountBase())
               delete Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info;
        }

     Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info = Array_Set;
  // Bugfix (11/27/2000)
     Array_Set->incrementReferenceCount();

#if COMPILE_DEBUG_STATEMENTS
     APP_ASSERT(Temporary->isTemporary() == TRUE);
     Temporary->Test_Consistency ("Test (on return) in #3 floatArray::Abstract_Operator(Array_Domain.Array_Conformability_Info_Type,floatArray,intSerialArray)");

  // Temporary->displayReferenceCounts("Called inside of #3 floatArray::Abstract_Operator");
  // X_SerialArray.displayReferenceCounts("X_SerialArray at BASE of #3 floatArray::Abstract_Operator()");
#endif

     return *Temporary;
   }
// endif



// if !defined(INTARRAY)
// ************************************************************************
// usual binary operators: operator+ operator- operator* operator/ operator% 
// for array to scalar operations
// ************************************************************************
floatArray & 
floatArray::Abstract_float_Conversion_Operator (
     Array_Conformability_Info_Type *Array_Set, 
     const floatArray & X_ParallelArray, 
     floatSerialArray* X_Serial_PCE_Array,
     floatSerialArray & X_SerialArray )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of #3 floatArray::Abstract_Operator(Array_Conformability_Info_Type,floatArray,intSerialArray) \n");

  // This is the only test we can do on the input!
     X_ParallelArray.Test_Consistency ("Test X_ParallelArray in #3 floatArray::Abstract_Operator(Array_Conformability_Info_Type,floatArray,intSerialArray) ");
     X_SerialArray.Test_Consistency ("Test X_SerialArray in #3 floatArray::Abstract_Operator(Array_Conformability_Info_Type,floatArray,floatSerialArray) ");
#endif

  // error checking
     APP_ASSERT(Array_Set != NULL);

  // The size of the temporary must be the same as that of the Lhs (Lhs based owners compute rule)
     floatSerialArray *Data = &((floatSerialArray &) X_SerialArray);
     APP_ASSERT(Data != NULL);

     floatArray *Temporary = NULL;
  // This is more efficient than the previous version which build a descriptor and
  // then forced 2 copies of the descriptor before ending up with a temporary to use.
     bool AvoidBuildingIndirectAddressingView = TRUE;
  // Data->view("In abstract_op #2: Data");
  // X_ParallelArray.Array_Descriptor.Array_Domain.display("In abstract_op #2: X_ParallelArray: Array_Domain");
     Temporary = new floatArray ( Data , 
                               &(X_ParallelArray.Array_Descriptor.Array_Domain),
                               AvoidBuildingIndirectAddressingView );

     APP_ASSERT (Temporary != NULL);
     APP_ASSERT (Temporary->Array_Descriptor.SerialArray != NULL);
  // printf ("In #3 floatArray::Abstract_Operator: Temporary->Array_Descriptor.SerialArray->getReferenceCount() = %d \n",
  //      Temporary->Array_Descriptor.SerialArray->getReferenceCount());
  // APP_ASSERT (Temporary->Array_Descriptor.SerialArray->getReferenceCount() > 1);

  // The base now needs to be reset since temporaries always have APP_Global_Array_Base as a base
  // ... don't reset this if indirect addressing is used ...
     if (!Temporary->Array_Descriptor.Array_Domain.Uses_Indirect_Addressing)
        Temporary->setBase(APP_Global_Array_Base);
          
  // Temporary->view ("view temporary in #3 floatArray::Abstract_Operator(Array_Conformability_Info_Type,floatArray,floatSerialArray)");
  // APP_ABORT();

     Temporary->setTemporary(TRUE);

  // ... maybe this isn't needed with the new constructor ...

     /*
     Array_Domain_Type::fixupLocalBase
	( Descriptor->Array_Domain, Data->Array_descriptor.Array_Domain,
	  X_ParallelArray.Array_Descriptor.Array_Domain,
	  X_ParallelArray.Array_Descriptor.SerialArray->
	  Array_Descriptor.Array_Domain);
     */

  // Test (12/4/2000) I think this might be true!
     APP_ASSERT (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info == NULL);

  // This is now we acumulate the information used to determine message passing 
  // when the operator= is executed!
     if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info != NULL)
        {
       // printf ("Commented out Array_Conformability_Info_Type::delete \n");
          Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->decrementReferenceCount();
          if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount() < 
              Array_Conformability_Info_Type::getReferenceCountBase())
               delete Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info;
        }

     Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info = Array_Set;
  // Bugfix (11/27/2000)
     Array_Set->incrementReferenceCount();

#if COMPILE_DEBUG_STATEMENTS
     APP_ASSERT(Temporary->isTemporary() == TRUE);
     Temporary->Test_Consistency ("Test (on return) in #3 floatArray::Abstract_Operator(Array_Domain.Array_Conformability_Info_Type,floatArray,floatSerialArray)");

  // Temporary->displayReferenceCounts("Called inside of #3 floatArray::Abstract_Operator");
  // X_SerialArray.displayReferenceCounts("X_SerialArray at BASE of #3 floatArray::Abstract_Operator()");
#endif

     return *Temporary;
   }
// endif



// if !defined(INTARRAY)
// ************************************************************************
// usual binary operators: operator+ operator- operator* operator/ operator% 
// for array to scalar operations
// ************************************************************************
doubleArray & 
floatArray::Abstract_double_Conversion_Operator (
     Array_Conformability_Info_Type *Array_Set, 
     const floatArray & X_ParallelArray, 
     floatSerialArray* X_Serial_PCE_Array,
     doubleSerialArray & X_SerialArray )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of #3 floatArray::Abstract_Operator(Array_Conformability_Info_Type,floatArray,intSerialArray) \n");

  // This is the only test we can do on the input!
     X_ParallelArray.Test_Consistency ("Test X_ParallelArray in #3 floatArray::Abstract_Operator(Array_Conformability_Info_Type,floatArray,intSerialArray) ");
     X_SerialArray.Test_Consistency ("Test X_SerialArray in #3 floatArray::Abstract_Operator(Array_Conformability_Info_Type,floatArray,doubleSerialArray) ");
#endif

  // error checking
     APP_ASSERT(Array_Set != NULL);

  // The size of the temporary must be the same as that of the Lhs (Lhs based owners compute rule)
     doubleSerialArray *Data = &((doubleSerialArray &) X_SerialArray);
     APP_ASSERT(Data != NULL);

     doubleArray *Temporary = NULL;
  // This is more efficient than the previous version which build a descriptor and
  // then forced 2 copies of the descriptor before ending up with a temporary to use.
     bool AvoidBuildingIndirectAddressingView = TRUE;
  // Data->view("In abstract_op #2: Data");
  // X_ParallelArray.Array_Descriptor.Array_Domain.display("In abstract_op #2: X_ParallelArray: Array_Domain");
     Temporary = new doubleArray ( Data , 
                               &(X_ParallelArray.Array_Descriptor.Array_Domain),
                               AvoidBuildingIndirectAddressingView );

     APP_ASSERT (Temporary != NULL);
     APP_ASSERT (Temporary->Array_Descriptor.SerialArray != NULL);
  // printf ("In #3 floatArray::Abstract_Operator: Temporary->Array_Descriptor.SerialArray->getReferenceCount() = %d \n",
  //      Temporary->Array_Descriptor.SerialArray->getReferenceCount());
  // APP_ASSERT (Temporary->Array_Descriptor.SerialArray->getReferenceCount() > 1);

  // The base now needs to be reset since temporaries always have APP_Global_Array_Base as a base
  // ... don't reset this if indirect addressing is used ...
     if (!Temporary->Array_Descriptor.Array_Domain.Uses_Indirect_Addressing)
        Temporary->setBase(APP_Global_Array_Base);
          
  // Temporary->view ("view temporary in #3 floatArray::Abstract_Operator(Array_Conformability_Info_Type,floatArray,floatSerialArray)");
  // APP_ABORT();

     Temporary->setTemporary(TRUE);

  // ... maybe this isn't needed with the new constructor ...

     /*
     Array_Domain_Type::fixupLocalBase
	( Descriptor->Array_Domain, Data->Array_descriptor.Array_Domain,
	  X_ParallelArray.Array_Descriptor.Array_Domain,
	  X_ParallelArray.Array_Descriptor.SerialArray->
	  Array_Descriptor.Array_Domain);
     */

  // Test (12/4/2000) I think this might be true!
     APP_ASSERT (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info == NULL);

  // This is now we acumulate the information used to determine message passing 
  // when the operator= is executed!
     if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info != NULL)
        {
       // printf ("Commented out Array_Conformability_Info_Type::delete \n");
          Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->decrementReferenceCount();
          if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount() < 
              Array_Conformability_Info_Type::getReferenceCountBase())
               delete Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info;
        }

     Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info = Array_Set;
  // Bugfix (11/27/2000)
     Array_Set->incrementReferenceCount();

#if COMPILE_DEBUG_STATEMENTS
     APP_ASSERT(Temporary->isTemporary() == TRUE);
     Temporary->Test_Consistency ("Test (on return) in #3 floatArray::Abstract_Operator(Array_Domain.Array_Conformability_Info_Type,floatArray,doubleSerialArray)");

  // Temporary->displayReferenceCounts("Called inside of #3 floatArray::Abstract_Operator");
  // X_SerialArray.displayReferenceCounts("X_SerialArray at BASE of #3 floatArray::Abstract_Operator()");
#endif

     return *Temporary;
   }
// endif




// ************************************************************************
// usual binary operators: operator+ operator- operator* operator/ operator% 
// for array to array operations
// ************************************************************************
floatArray &
floatArray::Abstract_Operator (
     Array_Conformability_Info_Type *Array_Set, 
     const floatArray & Lhs_ParallelArray, const floatArray & Rhs_ParallelArray, 
     floatSerialArray* Lhs_Serial_PCE_Array, floatSerialArray* Rhs_Serial_PCE_Array, 
     floatSerialArray & X_SerialArray )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of #4 floatArray::Abstract_Operator(Array_Conformability_Info_Type,floatArray,floatArray,floatSerialArray) \n");
#endif

  // error checking
     APP_ASSERT(Array_Set != NULL);

#if COMPILE_DEBUG_STATEMENTS
  // printf ("floatArray::Abstract_Operator for binary operators! \n");
  // These are the only tests we can do on the input!
     Array_Set->Test_Consistency("Test Array_Set in #4 floatArray::Abstract_Operator(Array_Conformability_Info_Type,floatArray,floatArray,floatSerialArray)");
     Lhs_ParallelArray.Test_Consistency ("Test LHS in #4 floatArray::Abstract_Operator(Array_Conformability_Info_Type,floatArray,floatArray,floatSerialArray)");
     Rhs_ParallelArray.Test_Consistency ("Test RHS in #4 floatArray::Abstract_Operator(Array_Conformability_Info_Type,floatArray,floatArray,floatSerialArray)");
     X_SerialArray.Test_Consistency     ("Test X_SerialArray in #4 floatArray::Abstract_Operator(Array_Conformability_Info_Type,floatArray,floatArray,floatSerialArray)");
#endif

  // Used for loop indexing
  // int i = 0;

     if (Index::Index_Bounds_Checking)
          Lhs_ParallelArray.Test_Conformability (Rhs_ParallelArray);

  // The Partitioning array is the array object (usually the Lhs operand) which the 
  // resulting temporary will be partitioned similar to (if it is required to build the temporary).
  // If a scalar was used on the Lhs then we have to use the Rhs as a Partitioning_floatArray
  // floatArray* Partitioning_Array = &((floatArray &) Lhs_ParallelArray);
  // APP_ASSERT (Partitioning_Array != NULL);

  // The size of the temporary must be the same as that of the Lhs (Lhs based owners compute rule)
     floatSerialArray *Data = &((floatSerialArray &) X_SerialArray);
     APP_ASSERT(Data != NULL);

#if COMPILE_DEBUG_STATEMENTS
  // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
     if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
        {
          printf ("SerialArray input to floatArray::Abstract_Operator X_SerialArray: local range (%d,%d,%d) \n",
               X_SerialArray.getBase(0),
               X_SerialArray.getBound(0),
               X_SerialArray.getStride(0));
        }
#endif

     floatArray* Temporary = NULL;
     Memory_Source_Type Result_Is_Lhs_Or_Rhs_Or_New_Memory = Uninitialized_Source;
     if ( Lhs_ParallelArray.isTemporary() == TRUE )
        {
       // Temporary reuse of serial array already handled at Serial_A++ level
#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
               printf ("Reuse the Lhs since it is a temporary! \n");
#endif
          Temporary = &((floatArray &) Lhs_ParallelArray);
          Result_Is_Lhs_Or_Rhs_Or_New_Memory = Memory_From_Lhs;

       // Delete the old Array_Conformability_Info object so that the new one can be inserted
          if (Lhs_ParallelArray.Array_Descriptor.Array_Domain.Array_Conformability_Info != NULL)
             {
               Lhs_ParallelArray.Array_Descriptor.Array_Domain.Array_Conformability_Info->decrementReferenceCount();
               if (Lhs_ParallelArray.Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount() < 
                   Array_Conformability_Info_Type::getReferenceCountBase())
                    delete Lhs_ParallelArray.Array_Descriptor.Array_Domain.Array_Conformability_Info;
             }

#if COMPILE_DEBUG_STATEMENTS
          Array_Set->Test_Consistency("Test Array_Set in #4 floatArray::Abstract_Operator (after delete Lhs_ParallelArray.Array_Descriptor.Array_Domain.Array_Conformability_Info)");
#endif
       // ... a new Array_Set is computed now even if Lhs has one ...
       // APP_ASSERT( Lhs_ParallelArray.Array_Descriptor.Array_Domain.Array_Conformability_Info == Array_Set );
          ((floatArray&)Lhs_ParallelArray).Array_Descriptor.Array_Domain.Array_Conformability_Info = Array_Set;
       // Bugfix (11/27/2000)
          Array_Set->incrementReferenceCount();

#if COMPILE_DEBUG_STATEMENTS
          Array_Set->Test_Consistency("Lhs_ParallelArray.Array_Descriptor.Array_Domain.Array_Conformability_Info == Array_Set at MIPOINT of #4 floatArray::Abstract_Operator(Array_Conformability_Info_Type,floatArray,floatArray,floatSerialArray)");
#endif

       // Temporary->displayReferenceCounts("Temporary at MIPOINT of #4 floatArray::Abstract_Operator");

          Array_Domain_Type::fixupLocalBase 
	     ( (Array_Domain_Type&)Lhs_ParallelArray.Array_Descriptor.Array_Domain , 
	       Data->Array_Descriptor.Array_Domain , 
               Lhs_ParallelArray.Array_Descriptor.Array_Domain , 
               Lhs_ParallelArray.Array_Descriptor.SerialArray->
	       Array_Descriptor.Array_Domain );

       // Lhs_ParallelArray.view("In P++ Abstract_Operator() Lhs_ParallelArray");

          APP_ASSERT (Temporary != NULL);
          APP_ASSERT (Temporary->Array_Descriptor.SerialArray != NULL);
          APP_ASSERT (Temporary->Array_Descriptor.SerialArray->getReferenceCount() >= floatArray::getReferenceCountBase());
       // Delete the serial array in the Temporary that will be reused 
       // (so it can be replaced by the input serialArray!)
          Temporary->Array_Descriptor.SerialArray->decrementReferenceCount();
          if (Temporary->Array_Descriptor.SerialArray->getReferenceCount() < floatArray::getReferenceCountBase())
               delete Temporary->Array_Descriptor.SerialArray;

       // Copy the input serialArray into the temporary (the return value)
          Temporary->Array_Descriptor.SerialArray = Data;

       // Temporary->displayReferenceCounts("Temporary after insertion of X_SerialArray in #4 floatArray::Abstract_Operator");

          APP_ASSERT (Data->getRawDataReferenceCount() >= getRawDataReferenceCountBase());
       // printf ("Exiting in reuse of Lhs as a test ... \n");
       // APP_ABORT();
        }
       else
        {
       // In the case of a Full_VSG_Update only the Lhs can be reused the Rhs may not be reused
       // This is because the Rhs is reconstructed for each binary operation and the data for the
       // Rhs is assembled into the newly constructed Rhs (which is the size of the local Lhs)
       // from all the processors owning that part of the global indexspace required.
       // This detail was unexpected and might be handled more efficiently in the future.
       // if ( Rhs_ParallelArray.Array_Descriptor.Array_Domain.Is_A_Temporary == TRUE )
          if ( (Rhs_ParallelArray.Array_Descriptor.Array_Domain.Is_A_Temporary == TRUE) && 
               (Array_Set->Full_VSG_Update_Required == FALSE) )
             {
            // Temporary reuse of serial array already handled at Serial_A++ level
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Reuse the Rhs since it is a temporary! \n");
#endif
            // printf ("Reuse the Rhs since it is a temporary! Exiting ... \n");
            // APP_ABORT();

               Temporary = &((floatArray &) Rhs_ParallelArray);
               Result_Is_Lhs_Or_Rhs_Or_New_Memory = Memory_From_Rhs;

               if (Rhs_ParallelArray.Array_Descriptor.Array_Domain.Array_Conformability_Info != NULL)
                  {
#if COMPILE_DEBUG_STATEMENTS
                    if (APP_DEBUG > 0)
                         printf ("delete for Array_Conformability_Info_Type called in floatArray::Abstract_Operator (Array_Conformability_Info_Type,floatArray,floatArray,floatSerialArray)");
#endif
                    Rhs_ParallelArray.Array_Descriptor.Array_Domain.Array_Conformability_Info->decrementReferenceCount();
                    if (Rhs_ParallelArray.Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount() < 
                        Array_Conformability_Info_Type::getReferenceCountBase())
                         delete Rhs_ParallelArray.Array_Descriptor.Array_Domain.Array_Conformability_Info;
                  }

#if COMPILE_DEBUG_STATEMENTS
               Array_Set->Test_Consistency("Test Array_Set in #4 floatArray::Abstract_Operator (after delete Rhs_ParallelArray.Array_Descriptor.Array_Domain.Array_Conformability_Info)");
#endif
               // ... TEMP TEST:  there is no reason this should br
	       //   true now ...
               //APP_ASSERT( Rhs_ParallelArray.Array_Descriptor.Array_Domain.Array_Conformability_Info == Array_Set );

               ((floatArray&)Rhs_ParallelArray).Array_Descriptor.Array_Domain.Array_Conformability_Info = Array_Set;
            // Bugfix (11/27/2000)
               Array_Set->incrementReferenceCount();

            // Data->view("Data BEFORE setBase");

               Array_Domain_Type::fixupLocalBase 
		  ((Array_Domain_Type&)Rhs_ParallelArray.Array_Descriptor.Array_Domain , 
		   Data->Array_Descriptor.Array_Domain , 
                   Rhs_ParallelArray.Array_Descriptor.Array_Domain , 
                   Rhs_ParallelArray.Array_Descriptor.SerialArray->
		   Array_Descriptor.Array_Domain );

	       // ... (10/11/96,kdb) correct Array_Set because it is valid
	       //  only for Lhs ...

               // ... temporary is consistent with Rhs so reverse offsets ...
               bool reverse_offset = TRUE;

               Array_Domain_Type:: Fix_Array_Conformability 
		  ( *Array_Set, Rhs_ParallelArray.Array_Descriptor.Array_Domain , 
		     Data->Array_Descriptor.Array_Domain, reverse_offset );

#if 0
               Modify_Reference_Counts_And_Manage_Temporaries ( Array_Set, Temporary, Lhs_Serial_PCE_Array, Rhs_Serial_PCE_Array, Data );
#endif

            // Temporary->Array_Descriptor.SerialArray = Data;

            // Bugfix (2/5/96) Must increment reference count since the operator.C code
            // will delete the serial arrays that the PCE returns.
            // printf ("Increment reference count on array reused (LHS)! \n");
            // While we have to increment the SerialArray's reference count so that the delete in the 
            // binary operator will not destroy the SerialArray --- we have to decrement the RawDataReferenceCount
            // because we want to use the raw data obtained though the view taken in PCE and
            // we will not be using the original array from which that view was taken.

            // To complicate issues the relational operators return a newly built temporary and so 
            // in this case the reference count of the data is ZERO and we can't (and should not) 
            // decrement it.
            // if (Data->getRawDataReferenceCount() > 0)
            //      Data->decrementRawDataReferenceCount();
            // Data->incrementReferenceCount();
               APP_ASSERT (Data->getRawDataReferenceCount() >= getRawDataReferenceCountBase());

            // printf ("Exiting in reuse of Rhs as a test ... \n");
            // APP_ABORT();

            // Temporary->view("Temporary");
             }
            else
             {
            // No temporary to reuse so we have to build one!
            // later we can avoid the construction of a new descriptor

#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Build a temporary since neither Lhs nor Rhs is a temporary! \n");
#endif
            // printf ("Build a temporary since neither Lhs nor Rhs is a temporary! \n");
            // APP_ABORT();
            // We should better call the floatArray_Descriptor_Type::Build_Temporary_By_Example
            // Now we have a descriptor based on the descriptor from the Lhs operand.  But we
            // have to fix it up in order to use it. Or mabe we should change the design so 
            // we don't have to change it (i.e. no re-centering would then be required) 

            // printf ("ERROR: In Abstract_Operator we are doing redundent copying (exit until this is fixed) \n");
            // APP_ABORT();
            // This is more efficient than the previous version which build a descriptor and
            // then forced 2 copies of the descriptor before ending up with a temporary to use.
               bool AvoidBuildingIndirectAddressingView = TRUE;
            // Data->view("In abstract_op #2: Data");
            // X_ParallelArray.Array_Descriptor.Array_Domain.display("In abstract_op #2: X_ParallelArray: Array_Domain");
#if COMPILE_DEBUG_STATEMENTS
            // Temporary->view ("view temporary in #4 floatArray::Abstract_Operator(): AFTER Temporary is built!");
               if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
                  {
                    printf ("(Abstract_Operator()) - Lhs_ParallelArray: global range (%d,%d,%d) local range (%d,%d,%d) \n",
                         Lhs_ParallelArray.getGlobalMaskIndex(0).getBase(),
                         Lhs_ParallelArray.getGlobalMaskIndex(0).getBound(),
                         Lhs_ParallelArray.getGlobalMaskIndex(0).getStride(),
                         Lhs_ParallelArray.getLocalMaskIndex(0).getBase(),
                         Lhs_ParallelArray.getLocalMaskIndex(0).getBound(),
                         Lhs_ParallelArray.getLocalMaskIndex(0).getStride());
                  }
#endif

               Temporary = new floatArray ( Data ,
                                         &(Lhs_ParallelArray.Array_Descriptor.Array_Domain),
                                         AvoidBuildingIndirectAddressingView );

#if COMPILE_DEBUG_STATEMENTS
            // Temporary->view ("view temporary in #4 floatArray::Abstract_Operator(): AFTER Temporary is built!");
               if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
                  {
                    printf ("(Abstract_Operator()) - Build the parallel array as a return value (BEFORE setBase()) - Temporary: global range (%d,%d,%d) local range (%d,%d,%d) \n",
                         Temporary->getGlobalMaskIndex(0).getBase(),
                         Temporary->getGlobalMaskIndex(0).getBound(),
                         Temporary->getGlobalMaskIndex(0).getStride(),
                         Temporary->getLocalMaskIndex(0).getBase(),
                         Temporary->getLocalMaskIndex(0).getBound(),
                         Temporary->getLocalMaskIndex(0).getStride());
        }
#endif

            // The base now needs to be reset since temporaries always have APP_Global_Array_Base as a base
	    // ... only reset if no indirect addressing ...
	       if (!Temporary->Array_Descriptor.Array_Domain.Uses_Indirect_Addressing)
                    Temporary->setBase(APP_Global_Array_Base);

#if 0
               Temporary->view ("view temporary in #4 floatArray::Abstract_Operator(Array_Conformability_Info_Type,floatArray,floatSerialArray)");
            // APP_ABORT();
#endif

#if COMPILE_DEBUG_STATEMENTS
               if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
                  {
                    printf ("(Abstract_Operator()) - Build the parallel array as a return value (AFTER setBase()) - Temporary: global range (%d,%d,%d) local range (%d,%d,%d) \n",
                         Temporary->getGlobalMaskIndex(0).getBase(),
                         Temporary->getGlobalMaskIndex(0).getBound(),
                         Temporary->getGlobalMaskIndex(0).getStride(),
                         Temporary->getLocalMaskIndex(0).getBase(),
                         Temporary->getLocalMaskIndex(0).getBound(),
                         Temporary->getLocalMaskIndex(0).getStride());
        }
#endif

               Temporary->setTemporary(TRUE);
 
	    // ... might not need this now ...

            /*
	    Array_Domain_Type::fixupLocalBase
	       (Descriptor->Array_Domain,
	        Data->Array_Descriptor.Array_Domain,
		Lhs_ParallelArray.Array_Descriptor.Array_Domain,
		Lhs_ParallelArray.Array_Descriptor.SerialArray->
		Array_Descriptor.Array_Domain);
            */

            // This is now we acumulate the information used to determine message passing
            // when the operator= is executed!
               APP_ASSERT (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info == NULL);
               Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info = Array_Set;
            // Bugfix (11/27/2000)
               Array_Set->incrementReferenceCount();

            // ... temporary is consistent with Lhs so don't reverse offsets ...
               bool reverse_offset = FALSE;

               Array_Domain_Type:: Fix_Array_Conformability ( 
		    *Array_Set, Temporary->Array_Descriptor.Array_Domain , 
		    Data->Array_Descriptor.Array_Domain, reverse_offset);

            // Bugfix (9/14/95) This should fix the problem related to the Rhs being a temporary
            // and thus the Rhs serial array being reused in the A++ class libaray such that it
            // is referenced in the Temporary which the function returns and the Rhs.
            // The problem shows up when B is a null array and we have B = B + B * B;
            // I would guess that this is because the Null arrays objects in this case
            // are multibily referenced rather than redundently built.
            // But if so --- is this the best place to increment the reference count!
               APP_ASSERT ( Data != NULL );
               if ( (Rhs_ParallelArray.isTemporary() == TRUE) && (Data->isNullArray() == TRUE) )
                  {
#if COMPILE_DEBUG_STATEMENTS
                    if (APP_DEBUG > 1)
                         printf ("Special case of Rhs temporary and reuse of Rhs SerialArray (B = B + B * B) reference count incremented in #4 floatArray::Abstract_Operator \n");
#endif
                 // printf ("Special Case Exiting ... \n");
                 // APP_ABORT();
                    Data->incrementReferenceCount();
                  }

            // Temporary->view("Temporary at MIPOINT of floatArray::Abstract_Operator(Array_Conformability_Info_Type,floatArray,floatArray,floatSerialArray)");
            // Temporary->Test_Consistency ("Test TEMPORARY at BASE of #4 floatArray::Abstract_Operator(Array_Conformability_Info_Type,floatArray,floatArray,floatSerialArray)");

            // printf ("Exiting in #4 floatArray::Abstract_Operator ... \n");
            // APP_ABORT();

               Result_Is_Lhs_Or_Rhs_Or_New_Memory = Newly_Allocated_Memory;
             }
        }

     APP_ASSERT(Temporary != NULL);
     APP_ASSERT(Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info != NULL);
     APP_ASSERT(Temporary->Array_Descriptor.SerialArray != NULL);

  // printf ("In #4 floatArray::Abstract_Operator: Temporary->Array_Descriptor.SerialArray->Array_ID = %d \n",
  //      Temporary->Array_Descriptor.SerialArray->Array_ID());

#if COMPILE_DEBUG_STATEMENTS
     APP_ASSERT(Temporary->Array_Descriptor.Array_Domain.Is_A_Temporary == TRUE);
  // Temporary->Test_Consistency ("Test Temporary after construction in #4 floatArray::Abstract_Operator");

     if (APP_DEBUG > 1)
          printf ("Now delete the temporaries handed in as input if they were not reused! \n");
#endif

  // If the Lhs was a temporary then we used it so we don't have to call
  // the "Delete_If_Temporary ( Lhs_ParallelArray );" function for the LHS
  // We want to delete the RHS if it is a temporary unless we used it
     if (Result_Is_Lhs_Or_Rhs_Or_New_Memory != Memory_From_Rhs)
          Delete_If_Temporary ( Rhs_ParallelArray );

  // printf ("(AFTER RHS TEST) Temporary->Array_Descriptor.SerialArray->Array_Descriptor = %p \n",Temporary->Array_Descriptor.SerialArray->Array_Descriptor);
  // printf ("Set Temporary->Array_Descriptor.Array_Set = Array_Set in floatArray::Abstract_Binary_Operator \n");

#if COMPILE_DEBUG_STATEMENTS
     APP_ASSERT(Temporary->Array_Descriptor.Array_Domain.Is_A_Temporary == TRUE);
     Temporary->Test_Consistency ("Test TEMPORARY at BASE of #4 floatArray::Abstract_Operator(Array_Conformability_Info_Type,floatArray,floatArray,floatSerialArray)");

     if (APP_DEBUG > 1)
        {
          if (APP_DEBUG > 2)
             {
               Temporary->view ("view TEMPORARY at BASE of #4 floatArray::Abstract_Operator(Array_Conformability_Info_Type,floatArray,floatArray,floatSerialArray)");
            // Lhs_Serial_PCE_Array->view ("Lhs_Serial_PCE_Array at BASE of #4 floatArray::Abstract_Operator(Array_Conformability_Info_Type,floatArray,floatArray,floatSerialArray)");
            // Rhs_Serial_PCE_Array->view ("Rhs_Serial_PCE_Array at BASE of #4 floatArray::Abstract_Operator(Array_Conformability_Info_Type,floatArray,floatArray,floatSerialArray)");
             }
          printf ("************************************************************************************************* \n");
          printf ("************************************************************************************************* \n");
          printf ("Leaving #4 floatArray::Abstract_Operator (Array_Conformability_Info_Type,floatArray,floatArray,floatSerialArray) \n");
          printf ("************************************************************************************************* \n");
          printf ("************************************************************************************************* \n");
        }

     if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
        {
          printf ("Build the parallel array as a return value (#4 floatArray::Abstract_Operator) - Temporary: global range (%d,%d,%d) local range (%d,%d,%d) \n",
               Temporary->getGlobalMaskIndex(0).getBase(),
               Temporary->getGlobalMaskIndex(0).getBound(),
               Temporary->getGlobalMaskIndex(0).getStride(),
               Temporary->getLocalMaskIndex(0).getBase(),
               Temporary->getLocalMaskIndex(0).getBound(),
               Temporary->getLocalMaskIndex(0).getStride());
        }

  // Commented out (12/14/2000)
  // Communication_Manager::Sync();
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
        {
          APP_ASSERT (Temporary != NULL);
          APP_ASSERT (Temporary->Array_Descriptor.SerialArray != NULL);
          printf ("In #4 floatArray::Abstract_Operator(): Temporary->Array_Descriptor.Array_ID() = %d Temporary->SerialArray->Array_ID() = %d \n",
               Temporary->Array_ID(),Temporary->Array_Descriptor.SerialArray->Array_ID());
          printf ("************************************************************************************************* \n");
          printf ("Leaving #4 floatArray::Abstract_Operator(Array_Conformability_Info_Type,floatArray,floatArray,floatSerialArray) (Temporary SerialArray pointer = %p) \n",Temporary->Array_Descriptor.SerialArray);
          printf ("************************************************************************************************* \n");
        }
#endif

     return *Temporary;
   }

#if !defined(INTARRAY)
// replace operators for array to array
intArray &
floatArray::Abstract_Operator ( 
     Array_Conformability_Info_Type *Array_Set, 
     const floatArray & Lhs_ParallelArray, const floatArray & Rhs_ParallelArray, 
     floatSerialArray* Lhs_Serial_PCE_Array, floatSerialArray* Rhs_Serial_PCE_Array, 
     intSerialArray & X_SerialArray )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of #5 floatArray::Abstract_Operator(Array_Conformability_Info_Type,floatArray,floatArray,intSerialArray) \n");

  // This is the only test we can do on the input!
     Lhs_ParallelArray.Test_Consistency ("Test LHS in #5 floatArray::Abstract_Operator(Array_Conformability_Info_Type,floatArray,floatArray,intSerialArray)");
     Rhs_ParallelArray.Test_Consistency ("Test RHS in #5 floatArray::Abstract_Operator(Array_Conformability_Info_Type,floatArray,floatArray,intSerialArray)");
     X_SerialArray.Test_Consistency ("Test X_SerialArray in #5 floatArray::Abstract_Operator(Array_Conformability_Info_Type,floatArray,floatArray,intSerialArray)");
#endif

     if (Index::Index_Bounds_Checking)
          Lhs_ParallelArray.Test_Conformability (Rhs_ParallelArray);

  // error checking
     APP_ASSERT(Array_Set != NULL);

  // The Partitioning array is the array object (usually the Lhs operand) which the
  // resulting temporary will be partitioned similar to (if it is required to build the temporary).
  // If a scalar was used on the Lhs then we have to use the Rhs as a Partitioning_floatArray
  // floatArray* Partitioning_Array = &((floatArray &) Lhs_ParallelArray);
  // APP_ASSERT (Partitioning_Array != NULL);

  // The size of the temporary must be the same as that of the Lhs (Lhs based owners compute rule)
     intSerialArray *Data = &((intSerialArray &) X_SerialArray);
     APP_ASSERT(Data != NULL);

     intArray *Temporary = NULL;

  // printf ("ERROR: In Abstract_Operator we are doing redundent copying (exit until this is fixed) \n");
  // APP_ABORT();
  // This is more efficient than the previous version which build a descriptor and
  // then forced 2 copies of the descriptor before ending up with a temporary to use.
     bool AvoidBuildingIndirectAddressingView = TRUE;
  // Data->view("In abstract_op #2: Data");
  // X_ParallelArray.Array_Descriptor.Array_Domain.display("In abstract_op #2: X_ParallelArray: Array_Domain");
     Temporary = new intArray ( Data ,
                               &(Lhs_ParallelArray.Array_Descriptor.Array_Domain),
                               AvoidBuildingIndirectAddressingView );
 
  // The base now needs to be reset since temporaries always have APP_Global_Array_Base as a base
     Temporary->setBase(APP_Global_Array_Base);
 
  // Temporary->view ("view temporary in #5 floatArray::Abstract_Operator(Array_Conformability_Info_Type,floatArray,floatSerialArray)");
  // APP_ABORT();
 
     Temporary->setTemporary(TRUE);
 
  // This is now we acumulate the information used to determine message passing
  // when the operator= is executed!
  // APP_ASSERT (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info == NULL);
  // Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info = Array_Set;

#if USE_TEMPORARY_DELETE_FUNCTIONS
     Delete_If_Temporary ( Lhs_ParallelArray );
     Delete_If_Temporary ( Rhs_ParallelArray );
#endif

  // This is how we acumulate the information used to determine message passing 
  // when the operator= is executed!
     if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info != NULL)
        {
          Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->decrementReferenceCount();
          if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount() < 
              Array_Conformability_Info_Type::getReferenceCountBase())
          delete Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info;
        }

     Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info = Array_Set;
  // Bugfix (11/27/2000)
     Array_Set->incrementReferenceCount();

#if COMPILE_DEBUG_STATEMENTS
     APP_ASSERT(Temporary->Array_Descriptor.Array_Domain.Is_A_Temporary == TRUE);
     Temporary->Test_Consistency ("Test (on return) in #5 floatArray::Abstract_Operator(Array_Conformability_Info_Type,floatArray,floatArray,intSerialArray)");
#endif

     return *Temporary;
   }
#endif

// ******************************************************************************
// These Abstract_Modification_Operator operators are special because they don't
// return a temporary.  This function is used for the array to scalar relational operators 
// ******************************************************************************
floatArray &
floatArray::Abstract_Modification_Operator (
     Array_Conformability_Info_Type *Array_Set, 
     const floatArray & X_ParallelArray, 
     floatSerialArray* X_Serial_PCE_Array, 
     const floatSerialArray & X_SerialArray )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of #6 floatArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,floatArray,floatSerialArray) \n");

  // This is the only test we can do on the input!
     X_ParallelArray.Test_Consistency ("Test A in #6 floatArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,floatArray,floatSerialArray)");
     X_SerialArray.Test_Consistency ("Test B in #6 floatArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,floatArray,floatSerialArray)");
#endif

#if USE_TEMPORARY_DELETE_FUNCTIONS
  // Delete_If_Temporary ( X_ParallelArray );
#endif

     floatArray* Temporary = &((floatArray &) X_ParallelArray);

  // Modify_Reference_Counts_And_Manage_Temporaries ( Temporary , Data );

  // This is now we acumulate the information used to determine message passing 
  // when the operator= is executed!
     if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info != NULL)
        {
          Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->decrementReferenceCount();
          if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount() < 
              Array_Conformability_Info_Type::getReferenceCountBase())
          delete Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info;
        }
     Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info = NULL;

#if 0
  // Bugfix (11/27/2000)
     Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info = Array_Set;
  // Bugfix (11/27/2000)
     Array_Set->incrementReferenceCount();
#endif

#if 0
  // Need to delete the Array_Set to avoid a memory leak
     Array_Set->decrementReferenceCount();
     if (Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
          delete Array_Set;
     Array_Set = NULL;
#endif

#if COMPILE_DEBUG_STATEMENTS
  // APP_ASSERT(Temporary->Array_Descriptor.Array_Domain.Is_A_Temporary == TRUE);
     Temporary->Test_Consistency ("Test (on return) in #6 floatArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,floatArray,floatSerialArray)");
#endif

     return *Temporary;
   }

// ******************************************************************************
// These Abstract_Modification_Operator operators are special because they don't
// return a temporary.  This function is used for the array to array replace operators 
// ******************************************************************************
floatArray &
floatArray::Abstract_Modification_Operator (
     Array_Conformability_Info_Type *Array_Set, 
     const floatArray & This_ParallelArray, const floatArray & Lhs_ParallelArray, 
     floatSerialArray* This_Serial_PCE_Array, floatSerialArray* Lhs_Serial_PCE_Array, 
     const floatSerialArray & This_SerialArray )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of #7 floatArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,floatArray,floatArray,floatSerialArray) \n");

  // This is the only test we can do on the input!
     This_ParallelArray.Test_Consistency ("Test THIS in #7 floatArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,floatArray,floatArray,floatSerialArray)");
     Lhs_ParallelArray.Test_Consistency ("Test LHS in #7 floatArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,floatArray,floatArray,floatSerialArray)");
     This_SerialArray.Test_Consistency ("Test This_SerialArray in #7 floatArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,floatArray,floatArray,floatSerialArray)");
#endif

     if (Index::Index_Bounds_Checking)
          This_ParallelArray.Test_Conformability (Lhs_ParallelArray);

#if USE_TEMPORARY_DELETE_FUNCTIONS
  // Delete_If_Temporary ( This_ParallelArray );
     Delete_If_Temporary ( Lhs_ParallelArray );
#endif

     floatArray* Temporary = &((floatArray &) This_ParallelArray);

  // Modify_Reference_Counts_And_Manage_Temporaries ( Temporary , Data );

  // This is how we acumulate the information used to determine message passing 
  // when the operator= is executed!
     if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info != NULL)
        {
       // printf ("Commented out Array_Conformability_Info_Type::delete \n");
          Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->decrementReferenceCount();
          if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount() < 
              Array_Conformability_Info_Type::getReferenceCountBase())
               delete Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info;
        }
     Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info = NULL;

#if 0
  // Bugfix (11/27/2000)
     Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info = Array_Set;
  // Bugfix (11/27/2000)
     Array_Set->incrementReferenceCount();
#endif

#if 0
  // Need to delete the Array_Set to avoid a memory leak
     Array_Set->decrementReferenceCount();
     if (Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
          delete Array_Set;
     Array_Set = NULL;
#endif

#if COMPILE_DEBUG_STATEMENTS
  // APP_ASSERT(Temporary->Array_Descriptor.Array_Domain.Is_A_Temporary == TRUE);
     Temporary->Test_Consistency ("Test (on return) in #7 floatArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,floatArray,floatSerialArray)");
#endif

     return *Temporary;
   }

#if !defined(INTARRAY)
// ******************************************************************************
// These Abstract_Modification_Operator operators are special because they don't
// return a temporary.  This function is used for the array to array replace operators 
// ******************************************************************************
floatArray &
floatArray::Abstract_Modification_Operator (
     Array_Conformability_Info_Type *Array_Set, 
     const floatArray & This_ParallelArray, const intArray & Lhs_ParallelArray, 
     floatSerialArray* This_Serial_PCE_Array, intSerialArray* Lhs_Serial_PCE_Array, 
     const floatSerialArray & This_SerialArray )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of #8 floatArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,intArray,floatArray,floatSerialArray) \n");

  // This is the only test we can do on the input!
     This_ParallelArray.Test_Consistency ("Test THIS in #8 floatArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,intArray,floatArray,floatSerialArray)");
     Lhs_ParallelArray.Test_Consistency ("Test LHS in #8 floatArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,intArray,floatArray,floatSerialArray)");
     This_SerialArray.Test_Consistency ("Test This_SerialArray in #8 floatArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,intArray,floatArray,floatSerialArray)");
#endif

     if (Index::Index_Bounds_Checking)
          This_ParallelArray.Test_Conformability (Lhs_ParallelArray);

     APP_ASSERT(Array_Set != NULL);

#if USE_TEMPORARY_DELETE_FUNCTIONS
  // Delete_If_Temporary ( This_ParallelArray );
     Delete_If_Temporary ( Lhs_ParallelArray );
#endif

     floatArray* Temporary = &((floatArray &) This_ParallelArray);

  // Modify_Reference_Counts_And_Manage_Temporaries ( Temporary , Data );

  // This is now we acumulate the information used to determine message passing 
  // when the operator= is executed!
     if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info != NULL)
        {
       // printf ("Commented out Array_Conformability_Info_Type::delete \n");
          Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->decrementReferenceCount();
          if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount() < 
              Array_Conformability_Info_Type::getReferenceCountBase())
               delete Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info;
        }
     Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info = NULL;

#if 0
  // Bugfix (11/27/2000)
     Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info = Array_Set;
  // Bugfix (11/27/2000)
     Array_Set->incrementReferenceCount();
#endif

#if 0
  // Need to delete the Array_Set to avoid a memory leak
     Array_Set->decrementReferenceCount();
     if (Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
          delete Array_Set;
     Array_Set = NULL;
#endif

#if COMPILE_DEBUG_STATEMENTS
  // APP_ASSERT(Temporary->Array_Descriptor.Array_Domain.Is_A_Temporary == TRUE);
     Temporary->Test_Consistency ("Test (on return) in #8 floatArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,intArray,floatArray,floatSerialArray)");
#endif

     return *Temporary;
   }

// ******************************************************************************
// These Abstract_Modification_Operator operators are special because they don't
// return a temporary.  This function is used for the array to scalar relational operators 
// ******************************************************************************
floatArray &
floatArray::Abstract_Modification_Operator (
     Array_Conformability_Info_Type *Array_Set, 
     const floatArray & This_ParallelArray, 
     const intArray & Lhs_ParallelArray, 
     const floatArray & Rhs_ParallelArray, 
     floatSerialArray* This_Serial_PCE_Array, 
     intSerialArray* Lhs_Serial_PCE_Array, 
     floatSerialArray* Rhs_Serial_PCE_Array, 
     const floatSerialArray & This_SerialArray )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of #9 floatArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,floatArray,intArray,floatArray,floatSerialArray) \n");

  // This is the only test we can do on the input!
     This_ParallelArray.Test_Consistency ("Test in #9 floatArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,floatArray,intArray,floatArray,floatSerialArray)");
     Lhs_ParallelArray.Test_Consistency ("Test in #9 floatArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,floatArray,intArray,floatArray,floatSerialArray)");
     Rhs_ParallelArray.Test_Consistency ("Test in #9 floatArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,floatArray,intArray,floatArray,floatSerialArray)");
     This_SerialArray.Test_Consistency ("Test in #9 floatArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,floatArray,intArray,floatArray,floatSerialArray)");
#endif

     if (Index::Index_Bounds_Checking)
        {
          This_ParallelArray.Test_Conformability (Lhs_ParallelArray);
          This_ParallelArray.Test_Conformability (Rhs_ParallelArray);
#if EXTRA_ERROR_CHECKING
       // Redundent test
          Lhs_ParallelArray.Test_Conformability (Rhs_ParallelArray);
#endif
        }

#if USE_TEMPORARY_DELETE_FUNCTIONS
     Delete_If_Temporary ( Lhs_ParallelArray );
     Delete_If_Temporary ( Rhs_ParallelArray );
#endif

     floatArray* Temporary = &((floatArray &) This_ParallelArray);

  // Modify_Reference_Counts_And_Manage_Temporaries ( Temporary , Data );

  // This is now we acumulate the information used to determine message passing 
  // when the operator= is executed!
     if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info != NULL)
        {
          Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->decrementReferenceCount();
          if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount() < 
              Array_Conformability_Info_Type::getReferenceCountBase())
               delete Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info;
        }
     Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info = NULL;

#if 0
  // Bugfix (11/27/2000)
     Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info = Array_Set;
  // Bugfix (11/27/2000)
     Array_Set->incrementReferenceCount();
#endif

#if 0
  // Need to delete the Array_Set to avoid a memory leak
     Array_Set->decrementReferenceCount();
     if (Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
          delete Array_Set;
     Array_Set = NULL;
#endif

#if COMPILE_DEBUG_STATEMENTS
  // APP_ASSERT(Temporary->Array_Descriptor.Array_Domain.Is_A_Temporary == TRUE);
     Temporary->Test_Consistency ("Test (on return) in #9 floatArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,floatArray,intArray,floatArray,floatSerialArray)");
#endif

     return *Temporary;
   }

#endif

#undef FLOATARRAY

#define INTARRAY
intArray *Last_Lhs_intArray_Operand = NULL;

#if 0

#if !defined(GNU)
// inline
#endif
void
intArray::Modify_Reference_Counts_And_Manage_Temporaries ( 
   Array_Conformability_Info_Type *Array_Set, 
   intArray *Temporary, 
   intSerialArray* Lhs_Serial_PCE_Array, 
   intSerialArray* Rhs_Serial_PCE_Array, 
   intSerialArray *Data )
   {
  // The following is a confusing point of P++ reference count management.  It was (is)
  // the source of many memory leaks that were (are) difficult to track down.

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of intArray::Modify_Reference_Counts_And_Manage_Temporaries (Lhs,Rhs) \n");
#endif
  // This means that the Lhs_Serial_PCE_Array and Rhs_Serial_PCE_Array 
  // are both views built from Build_Pointer_To_View_Of_Array
  // bool Lhs_Is_A_View = TRUE;
  // bool Rhs_Is_A_View = TRUE;

     bool Lhs_Serial_PCE_Array_Reused = (Data == Lhs_Serial_PCE_Array);
     bool Rhs_Serial_PCE_Array_Reused = (Data == Rhs_Serial_PCE_Array);

     bool New_Array_Used = (Lhs_Serial_PCE_Array_Reused == FALSE) &&
                              (Rhs_Serial_PCE_Array_Reused == FALSE);

     if (New_Array_Used)
        {
       // We don't have to do anything the reference count of the Lhs_Serial_PCE_Array
       // and Rhs_Serial_PCE_Array should be 1 (or greater).
#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 0)
               printf ("New_Array_Used == TRUE \n");
#endif
       // In this case the serial binary operation build a new temporary to return (Lhs was not reused)
       // We delete the raw data because the serial array shell is deleted elsewhere?
#if 1
       // Test (11/10/2000) trying to fix reference counting mechanism
       // Temporary->Array_Descriptor.SerialArray->Delete_Array_Data();
          APP_ASSERT (Temporary != NULL);
          APP_ASSERT (Temporary->Array_Descriptor.SerialArray != NULL);
          APP_ASSERT (Temporary->Array_Descriptor.SerialArray->getReferenceCount() >= intArray::getReferenceCountBase());
          Temporary->Array_Descriptor.SerialArray->decrementReferenceCount();
          if (Temporary->Array_Descriptor.SerialArray->getReferenceCount() < intArray::getReferenceCountBase())
               delete Temporary->Array_Descriptor.SerialArray;
#endif
       // APP_ASSERT (Lhs_Serial_PCE_Array->getRawDataReferenceCount() > 0);
       // APP_ASSERT (Rhs_Serial_PCE_Array->getRawDataReferenceCount() > 0);
        }
       else
        {
#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 0)
               printf ("New_Array_Used == FALSE \n");
#endif
       // While we have to increment the SerialArray's reference count so that the delete in the 
       // binary operator will not destroy the SerialArray (this is because the Lhs_SerialArray and the 
       // result of the Serail array binary operation are the same array -- because the Lhs 
       // was resused) --- we have to decrement the RawDataReferenceCount
       // because we want to use the raw data obtained though the view taken in PCE and
       // we will not be using the original array from which that view was taken.
       // Data->decrementRawDataReferenceCount();

       // This code construction concerns me since it still does not seem that we should
       // be checking for the Data->getRawDataReferenceCount() > 0.

          printf ("##### Questionable decrement of the reference count if it is greater than the DataReferenceCountBase \n");
          if (Data->getRawDataReferenceCount() > getRawDataReferenceCountBase())
               Data->decrementRawDataReferenceCount();
          Data->incrementReferenceCount();
        }

  // Copy the input serialArray into the temporary (the return value for the calling (parent) functions)
     Temporary->Array_Descriptor.SerialArray = Data;
   }

#endif

// **************************************************************************
// **************************************************************************
//                   ABSTRACT OPERATORS SPECIFIC TO P++
//           (follows same logic as in A++ abstract operators)
//     (where statement logic is handled at the lower Serial_A++ level)
// **************************************************************************
// **************************************************************************
     
// **********************************************************
// The equals operator taking a scalar
// **********************************************************
intArray &
intArray::operator= ( int x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of intArray::operator=(int x)! \n");

  // This is the only test we can do on the input!
     Test_Consistency ("Test in intArray::operator= (int x)");
#endif

     APP_ASSERT(Array_Descriptor.SerialArray != NULL);
  // APP_ASSERT(Array_Descriptor.SerialArray->Array_Descriptor != NULL);

  // The SerialArray is a view of the correct portion of the raw data
  // so we can just call the SerialArray (A++) assignment operator.
  /* ...(5/5/98, kdb) WRONG: if there is a where mask this doesn't work ... */
  //   (*Array_Descriptor.SerialArray) = x;

     intSerialArray *This_SerialArray = NULL;
     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing)
               Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement (*this, This_SerialArray);
            else
               Array_Set = intArray::Parallel_Conformability_Enforcement (*this, This_SerialArray);
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->Array_Descriptor.SerialArray;

          if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE)
               Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement
                    (*this, This_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
            else
               Array_Set = intArray::Parallel_Conformability_Enforcement
                                (*this, This_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
          Where_Statement_Support::Where_Statement_Mask->Array_Descriptor.SerialArray = Mask_SerialArray;
        }

  // if (Array_Set == Null)
  //      Array_Set = new Array_Conformability_Info_Type();

  // APP_ASSERT(Array_Set != NULL);
     APP_ASSERT(This_SerialArray != NULL);

     *This_SerialArray = x;

     if (Where_Statement_Support::Where_Statement_Mask != NULL)
          Where_Statement_Support::Where_Statement_Mask->Array_Descriptor.SerialArray = Old_Mask_SerialArray;

#if defined(PPP) && defined(USE_PADRE)
     setLocalDomainInPADRE_Descriptor(NULL);
#endif

     APP_ASSERT (This_SerialArray->getReferenceCount() >= intArray::getReferenceCountBase());
     This_SerialArray->decrementReferenceCount();
     if (This_SerialArray->getReferenceCount() < intArray::getReferenceCountBase())
          delete This_SerialArray;
 
     if (Mask_SerialArray != NULL)
        {
#if defined(PPP) && defined(USE_PADRE)
          setLocalDomainInPADRE_Descriptor(NULL);
#endif
          APP_ASSERT (Mask_SerialArray->getReferenceCount() >= intArray::getReferenceCountBase());
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intArray::getReferenceCountBase())
               delete Mask_SerialArray;
        }

     This_SerialArray = NULL;
     Mask_SerialArray = NULL;

#if 1
     if (Communication_Manager::Number_Of_Processors > 1)
        {
          if (Array_Set != NULL)
             {
               if (Array_Set->Full_VSG_Update_Required == FALSE)
                  {
                    int update_any = FALSE;
                    int i;
                    for (i=0; i < MAX_ARRAY_DIMENSION; i++)
                         if ((Array_Set->Update_Left_Ghost_Boundary_Width [i] >0) ||
                             (Array_Set->Update_Right_Ghost_Boundary_Width[i] >0) )
                              update_any = TRUE;
                    if (update_any)
                       {
#if COMPILE_DEBUG_STATEMENTS
                         for (i =0; i < MAX_ARRAY_DIMENSION; i++)
                            {
                              if ((Array_Set->Update_Left_Ghost_Boundary_Width [i] >0) ||
                                  (Array_Set->Update_Right_Ghost_Boundary_Width[i] >0) )
                                 {
                                   if (Array_Descriptor.Array_Domain.InternalGhostCellWidth[i] == 0)
                                      {
                                        printf("ERROR: No ghost cells along axis %d to ", i);
                                        printf("support message passing \n");
                                        APP_ABORT();
                                      }
                                 }
                              if (Array_Descriptor.isLeftPartition(i))
                                 {
                                   if (Array_Set->Update_Left_Ghost_Boundary_Width [i] >0) 
                                      {
                                        printf("ERROR: Left processor shouldn't try to update ");
                                        printf("left ghost boundary on dim %d.\n",i);
                                      }
                                   APP_ASSERT (Array_Set->Update_Left_Ghost_Boundary_Width[i] == 0);
                                 }
                              if (Array_Descriptor.isRightPartition(i))
                                 {
                                   if (Array_Set->Update_Right_Ghost_Boundary_Width [i] >0) 
                                      {
                                        printf("ERROR: Right processor shouldn't try to update ");
                                        printf("right ghost boundary on dim %d.\n",i);
                                      }
                                   APP_ASSERT (Array_Set->Update_Right_Ghost_Boundary_Width[i] == 0);
                                 }
                            }
#endif
                         if (Array_Descriptor.Array_Domain.Array_Conformability_Info != NULL)
                            {
#if COMPILE_DEBUG_STATEMENTS
                              if (APP_DEBUG > 3)
                                 {
                                   printf ("Inside of operator=(scalar): Array_Conformability_Info->getReferenceCount() = %d \n",
                                        Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount());
                                 }
#endif
                              APP_ASSERT (Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount() >= 
                                   Array_Conformability_Info_Type::getReferenceCountBase());
                              Array_Descriptor.Array_Domain.Array_Conformability_Info->decrementReferenceCount();
                              if (Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount() <
                                  Array_Conformability_Info_Type::getReferenceCountBase())
                                 {
                                   delete Array_Descriptor.Array_Domain.Array_Conformability_Info;
                                 }
                            }
                         Array_Descriptor.Array_Domain.Array_Conformability_Info = Array_Set;

                      // Test added (11/26/2000)
                         APP_ASSERT (Array_Descriptor.Array_Domain.Array_Conformability_Info != NULL);

                      // We should modify this so that the ghost boundaries are set properly without communication
                         updateGhostBoundaries();
                         Array_Descriptor.Array_Domain.Array_Conformability_Info = NULL;
                       }
                  }
                 else
                  {
                 // Full VSG_Update is required 
                    updateGhostBoundaries();
                  }
             }
            else
             {
            // Array_Set is null because indirect addressing is used;
               updateGhostBoundaries();
             }
        } // end of (Number_Of_Processors > 1)
#endif

     if (Array_Set != NULL)
        {
#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 3)
             {
               printf ("Inside of ~Array_Domain_Type: Array_Set->getReferenceCount() = %d \n",
                    Array_Set->getReferenceCount());
             }
#endif
          APP_ASSERT (Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
          Array_Set->decrementReferenceCount();
          if (Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
               delete Array_Set;
          Array_Set = NULL;
        }

#if COMPILE_DEBUG_STATEMENTS
  // APP_ASSERT(Array_Descriptor.Array_Domain.Is_A_Temporary == FALSE);
     Test_Consistency ("Test (on return) in intArray::operator= (int x)");
#endif

#if USE_TEMPORARY_DELETE_FUNCTIONS
  // This would be unlikely but we have to make sure
     Delete_Lhs_If_Temporary ( *this );
#endif

     return *this;
   }

// **********************************************************
// The other equals operator taking an array object
// **********************************************************
intArray &
intArray::operator= ( const intArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if ( (APP_DEBUG > 1) || (Diagnostic_Manager::getReferenceCountingReport() > 0) )
        {
          printf ("\n\n\n@@@@@ Inside of intArray::operator=(const intArray & Rhs) (id=%d) = (id=%d) \n",Array_ID(),Rhs.Array_ID());
          this->displayReferenceCounts("Lhs in intArray & operator=(intArray,intArray)");
          Rhs.displayReferenceCounts("Rhs in intArray & operator=(intArray,intArray)");
       }

  // printf ("In intArray::operator=(Rhs): Rhs.getReferenceCount() = %d \n",Rhs.getReferenceCount());
  // printf ("In intArray::operator=(Rhs): Rhs.Array_Descriptor.SerialArray->getReferenceCount() = %d \n",
  //      Rhs.Array_Descriptor.SerialArray->getReferenceCount());

  // Temp code to debug reference count problem!
  // Rhs.view ("view Rhs in intArray::operator= (const intArray & Rhs)");

  // This is the only test we can do on the input!
  // printf ("Test_Consistency the input array objects \n");
     Test_Consistency ("Test Lhs in intArray::operator= (const intArray & Rhs)");
     Rhs.Test_Consistency ("Test Rhs in intArray::operator= (const intArray & Rhs)");

#if 0
#if COMPILE_DEBUG_STATEMENTS
     if (Communication_Manager::localProcessNumber() == 0)
        {
          Array_Descriptor.Array_Domain.display("Lhs in operator=");
        }
#endif
#endif

     if (APP_DEBUG > 3)
        {
          printf ("View the input array objects \n");
          view ("view Lhs in intArray::operator= (const intArray & Rhs)");
          Rhs.view ("view Rhs in intArray::operator= (const intArray & Rhs)");
       // printf ("Exiting at TOP of intArray::operator= ( const intArray & Rhs ) ... \n");
       // APP_ABORT();
        }
#endif

     if (Index::Index_Bounds_Checking)
          Test_Conformability (Rhs);

  // view("THIS inside of operator=");
  // Rhs.view("Rhs inside of operator=");

     intSerialArray** Lhs_SerialArray_Pointer = &Array_Descriptor.SerialArray;
     intSerialArray** Rhs_SerialArray_Pointer = &(((intArray &)Rhs).Array_Descriptor.SerialArray);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          this->displayReferenceCounts("THIS in intArray::operator= (intArray)");
          Rhs.displayReferenceCounts("Rhs in intArray::operator= (intArray)");
        }
#endif

  // reorder these tests for better efficiency (can it be improved?)
  // if ( (Array_Descriptor.Array_Reference_Count_Array[Array_Descriptor.Array_ID()] == 0) &&
  //       Binary_Conformable (Rhs) &&
  //       Rhs.Array_Descriptor.Array_Domain.Is_A_Temporary &&
  //      !Rhs.Array_Descriptor.Array_Domain.Is_A_View )
  /* bool Case_1 = (Array_Descriptor.Array_Reference_Count_Array[Array_Descriptor.Array_ID()] == 0) && */
  // ... (5/8/97, kdb) make sure this isn't a view also ...
     bool Case_1 = (getRawDataReferenceCount() == getRawDataReferenceCountBase()) &&
                       !isView() &&
                       Binary_Conformable (Rhs) &&
                       Rhs.isTemporary() &&
                      !Rhs.isView();
  // this case is specific to P++ because temporaries are not condensed in the assignment to the
  // Lhs if that Lhs is a Null Array.  This is an internal difference between A++ and P++.
  // It is done to permit the distribution to be identical for the Lhs instead of being remapped.
  // ... (5/30/98, kdb) this cann't be a null array and a view without problems ...
     bool Case_2 = (getRawDataReferenceCount() == getRawDataReferenceCountBase()) && 
                       Rhs.isTemporary() && isNullArray() && !isView(); 
     if (Case_1 || Case_2)
        {
       // Give back the original Array_Data memory (but only if it exists)!
       // But it should always exist so we don't really have the check!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
               printf ("Skip the assignment by stealing the data (delete the Lhs's data if not a NullArray)! \n");
#endif

       // Handle the case of assignment to a NULL array (i.e. undimensioned array object
       // or an array object dimensioned to length zero).
          if (isNullArray())
             {
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Lhs is a Null Array so copy descriptor before assignment! \n");
#endif

            // Bug Fix (8/17/94) Null array must have a properly computed array descriptor
            // APP_ASSERT( Array_Descriptor != NULL );
            // Array_Descriptor.ReferenceCountedDelete();
            // delete Array_Descriptor;

            // We can't steal the descriptor because the descriptor contains the Array_ID and it
            // would have to be different for each of the Lhs and Rhs array objects.
            // The Array_ID should be in the array objects.
               Array_Descriptor.Build_Temporary_By_Example ( Rhs.Array_Descriptor ); 
               Array_Descriptor.Array_Domain.Is_A_Temporary = FALSE;
             }
            else
             {
            // Bug fix for nonzero Lhs base -- the Rhs is a temporary and so it has a zero base and the A++
            // assignment is never called so the serial array we will substitute has zero base.  
            // So we have to fix the base of the rhs we will steal and place on the Lhs.
	    // ... (9/27/96, kdb) other values need to be set correctly also ...
            // BTNG APP_ASSERT ( MAX_ARRAY_DIMENSION == 4);
	       int temp;
               for (temp=0; temp < MAX_ARRAY_DIMENSION; temp++)
                  {
                    Rhs.Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain.Data_Base[temp] = 
                         Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain.Data_Base[temp];
                    Rhs.Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain.User_Base[temp] = 
                         Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain.User_Base[temp];
                    Rhs.Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain.Scalar_Offset[temp] = 
                         Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain.Scalar_Offset[temp];
                  }
             }

       // Why is this a warning!
          APP_ASSERT ( getRawDataReferenceCount() == getRawDataReferenceCountBase() );
          if ( getRawDataReferenceCount() > getRawDataReferenceCountBase() )
             {
               printf ("WARNING: Array_ID = %d getRawDataReferenceCount() = %d \n",
                    Array_Descriptor.Array_ID(),getRawDataReferenceCount() );
             }
          APP_ASSERT(getRawDataReferenceCount() == getRawDataReferenceCountBase());

       // printf ("Before Delete_Array_Data -- Skip in intArray::operator=(Rhs): Rhs.getReferenceCount() = %d \n",Rhs.getReferenceCount());
       // printf ("Before Delete_Array_Data -- Skip in intArray::operator=(Rhs): Rhs.Array_Descriptor.SerialArray->getReferenceCount() = %d \n",
       //      Rhs.Array_Descriptor.SerialArray->getReferenceCount());

       // view ("View of THIS from operator=");
          Delete_Array_Data ();
          *Lhs_SerialArray_Pointer = NULL;

       // APP_ASSERT(Array_Descriptor.Array_Domain.Array_Reference_Count_Array[Array_Descriptor.Array_ID()] == 
       //   intArray_Descriptor_Type
       //   ::Array_Reference_Count_Array[Array_Descriptor.Array_ID()] );

       // Since we stold the data we can zero the reference count (part of breaking the reference)
       // APP_ASSERT( intArray_Descriptor_Type::Array_Reference_Count_Array[Array_Descriptor.Array_ID()] >= 0 );
       // if ( intArray_Descriptor_Type::Array_Reference_Count_Array[Array_Descriptor.Array_ID()] >= 0 )
       // Array_Descriptor.Array_Reference_Count_Array[Array_Descriptor.Array_ID()] = 0;

          APP_ASSERT(*Lhs_SerialArray_Pointer == NULL);
       // Call the Serial_A++ assignment operator
          *Lhs_SerialArray_Pointer = *Rhs_SerialArray_Pointer;
          APP_ASSERT(*Lhs_SerialArray_Pointer != NULL);
          (*Lhs_SerialArray_Pointer)->Array_Descriptor.Array_Domain.Is_A_Temporary = FALSE;

       // increment the reference count so that after the Rhs is deleted we will still have the
       // serial array around on the Lhs.
          incrementRawDataReferenceCount();
          APP_ASSERT( getRawDataReferenceCount() > getRawDataReferenceCountBase());

#if 0
          printf ("After incrementRawDataReferenceCount() -- Skip in intArray::operator=(Rhs): Rhs.getReferenceCount() = %d \n",Rhs.getReferenceCount());
          printf ("After incrementRawDataReferenceCount() -- Skip in intArray::operator=(Rhs): Rhs.Array_Descriptor.SerialArray->getReferenceCount() = %d \n",Rhs.Array_Descriptor.SerialArray->getReferenceCount());
          printf ("After incrementRawDataReferenceCount() -- Skip in intArray::operator=(Rhs): Rhs.getRawDataReferenceCount() = %d \n",Rhs.getRawDataReferenceCount());
#endif

       // We want the Rhs to have a serial array when the Rhs is deleted as a temporary
       // if the reference counts are greater then 1 then the Lhs will be left with the data
       // after the Rhs is deleted!
       // *Rhs_SerialArray_Pointer = NULL;

       // Bugfix (12/19/94) Force base of SerialArray equal to the value in the P++ descriptor
          APP_ASSERT(Array_Descriptor.SerialArray != NULL);

       // (4/30/97,kdb) Reinitialize the Serial Array_View_Pointers because 
       // SerialArray has been copied from Rhs

	  SERIAL_POINTER_LIST_INITIALIZATION_MACRO;
        }
       else 
        {
       // Do the assignment the hard way (not so hard 
       // since we just call the A++ assignment operator)
#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
               printf ("Do the assignment by calling the Serial_A++ assignment function! \n");
#endif

       // The more complex logic contained in the Serial_A++ implementation can be skipped
       // since we are just calling the Serial_A++ assignment operator and so it is included
       // this means that cases such as A(I) = A(I+1) are properly handled
          APP_ASSERT(Array_Descriptor.SerialArray != NULL);
          APP_ASSERT(Rhs.Array_Descriptor.SerialArray != NULL);

       // We want to allow NULL operations to avoid building new Null arrays for the Lhs
       // So don't allow the Lhs to be rebuilt if the Rhs is a Null array!
       // if (Array_Descriptor.Array_Domain.Is_A_Null_Array)
       // if (Array_Descriptor.Array_Domain.Is_A_Null_Array && !Rhs.Array_Descriptor.Array_Domain.Is_A_Null_Array)
          if (isNullArray() && !Rhs.isNullArray())
             {
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Lhs is a Null Array! \n");
#endif

            // printf ("################################################################## \n");
            // printf ("Case of Lhs being a NULL ARRAY and Rhs is a VIEW -- commented out! \n");
            // printf ("################################################################## \n");
            // The problem here is that the view Lhs is not being build to be the correct size
            // to work well in the parallel environment.  The lhs in this case should be built
            // to be the size of the rhs array data assuming no view.  I.e. the lhs should be
            // the same size as what the rhs is a view of so that the partitioning of the 
            // lhs and the rhs will be alligned (when we get to the serial operator= below).
               bool HANDLE_CASE_OF_VIEW = FALSE;
               if ( Rhs.isView() && HANDLE_CASE_OF_VIEW )
                  {
                 // Force Lhs to be the size of the referenced part of the Rhs
                 // This implies a default partitioning when used with P++.
                 // printf ("Rhs.Array_Descriptor.Array_Domain.Is_A_View == TRUE -- Building a new descriptor from scratch! \n");
                 // int i = Rhs.Array_Descriptor.getLength(0);
                 // int j = Rhs.Array_Descriptor.getLength(1);
                 // int k = Rhs.Array_Descriptor.getLength(2);
                 // int l = Rhs.Array_Descriptor.getLength(3);

                    Integer_Array_MAX_ARRAY_DIMENSION_Type Integer_List;
		    int temp;
                    for (temp=0; temp < MAX_ARRAY_DIMENSION; temp++)
                         Integer_List[temp] = Rhs.Array_Descriptor.getLength(temp);

                 // This is NOT EFFICIENT -- but I gather that this case does not happen
                 // sense HANDLE_CASE_OF_VIEW is FALSE!
                 // Array_Descriptor = new intArray_Descriptor_Type(i,j,k,l);
                 // Array_Descriptor = 
                 //  new intArray_Descriptor_Type (Integer_List);
                    Array_Descriptor = intArray_Descriptor_Type
		                          (MAX_ARRAY_DIMENSION,Integer_List);

                    Array_Descriptor.display("Exiting in case of null Lhs of operator= ");
                    printf ("Exiting in case of null Lhs of operator= \n");
                    APP_ABORT();
                  }
                 else
                  {
                 // This is just calling the Array_Descriptor_Type's copy constructor so we have to
                 // modify the descriptor in the case it is a view.
                 // printf ("Rhs.Array_Descriptor.Array_Domain.Is_A_View == FALSE -- Building a copy of the Rhs descriptor! \n");

                 // If the Rhs is a Null Array then we should never have gotten to this point in the code!
                    APP_ASSERT( Rhs.isNullArray() == FALSE );

                 // This copies the domain in Rhs into the domain in the Lhs except that
                 // subsequently the bases are set to the default A++/P++ base (typically zero).
                    Array_Descriptor.Build_Temporary_By_Example ( Rhs.Array_Descriptor );

                 // ... change (10/29/96,kdb) set bases to Rhs bases because 
                 // Build_Temporary_By_Example sets the bases to zero 
                 // after copying Rhs and in the case of a null Lhs we now 
                 // want the bases of Lhs and Rhs to be the same ...

                 // ... set bases to Rhs bases ...
                 // Now we reset the base to be that of the Rhs (perhaps we should have just copied the
                 // domain explicitly -- since we copied the Rhs domain reset the base and then set the
                 // base again).
	            int nd = 0; 
                    for (nd=0;nd<MAX_ARRAY_DIMENSION;nd++)                    
		         setBase(Rhs.Array_Descriptor.getRawBase(nd),nd); 

                    setTemporary(FALSE);

#if COMPILE_DEBUG_STATEMENTS
                    if (APP_DEBUG > 5)
                       {
                         Rhs.Array_Descriptor.display("In operator= display (Rhs.Array_Descriptor)");
                         Array_Descriptor.display("In operator= display (Array_Descriptor)");
                       }
#endif

                    if (Array_Descriptor.Array_Domain.Array_Conformability_Info != NULL)
                       {
#if COMPILE_DEBUG_STATEMENTS
                         if (APP_DEBUG > 3)
                            {
                              printf ("Inside of operator=(Rhs): Array_Conformability_Info->getReferenceCount() = %d \n",
                                   Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount());
                            }
#endif
                         APP_ASSERT (Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount() >= 
                                     Array_Conformability_Info_Type::getReferenceCountBase());
                         Array_Descriptor.Array_Domain.Array_Conformability_Info->decrementReferenceCount();
                         if (Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount() 
                              < Array_Conformability_Info_Type::getReferenceCountBase())
                            {
                              delete Array_Descriptor.Array_Domain.Array_Conformability_Info;
                            }
                         Array_Descriptor.Array_Domain.Array_Conformability_Info = NULL;
                       }

                    APP_ASSERT( Array_Descriptor.Array_Domain.Array_Conformability_Info == NULL );
                 // Array_Descriptor.display ("CASE OF NULL LHS OF OPERATOR=");
                  }

               if (HANDLE_CASE_OF_VIEW)
                  {
                    APP_ASSERT( isView() == FALSE );
                  }

#if 0
#if !defined(USE_PADRE)
            // attempt at bug fix (6/10/2000)
               APP_ASSERT (Array_Descriptor.Array_Domain.BlockPartiArrayDomain == NULL);
               APP_ASSERT (Array_Descriptor.Array_Domain.BlockPartiArrayDecomposition == NULL);
#endif
#endif

            // Now allocate the data for the Lhs (so we can copy the Rhs into the Lhs)
            // printf ("Allocating data for local array on Lhs (Lhs was a null array)! \n");
               Allocate_Array_Data(TRUE);

            // printf ("Exiting inside of operator= \n");
            // APP_ABORT();
             }

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
               printf ("Now call the Serial_A++ assignment operator! \n");
       // Communication_Manager::Sync();
#endif

          APP_ASSERT(isTemporary() == FALSE);
          APP_ASSERT(Array_Descriptor.SerialArray != NULL);
          APP_ASSERT(Array_Descriptor.SerialArray->isTemporary() == FALSE);

       // Figure out the parallel communication required and form the conformable 
       // views of the SerialArray objects then call the SerialArray assignment 
       // operator.

          intSerialArray  *This_SerialArray     = NULL;
          intSerialArray  *Rhs_SerialArray      = NULL;
	  intSerialArray *Mask_SerialArray     = NULL;
	  intSerialArray *Old_Mask_SerialArray = NULL;

          Array_Conformability_Info_Type *Array_Set = NULL;
          if (Where_Statement_Support::Where_Statement_Mask == NULL)
             {
            // printf ("No Where Mask: Calling intArray::Parallel_Indirect_Conformability_Enforcement \n");
            // printf ("Exiting BEFORE intArray::Parallel_Conformability_Enforcement in operator= \n");
            // APP_ABORT();

               if ((usesIndirectAddressing() == TRUE) || (Rhs.usesIndirectAddressing() == TRUE))
                  {
                    Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( *this, This_SerialArray, Rhs, Rhs_SerialArray );
                  }
                 else
                  {
                    Array_Set = intArray::Parallel_Conformability_Enforcement ( *this, This_SerialArray, Rhs, Rhs_SerialArray );
                  }
             }
            else
             {
            // ... (4/10/97, kdb) save old serial where mask because
            // Serial_Where_Statement_Mask has to be temporarily 
            // reset to make where work ...

               Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->Array_Descriptor.SerialArray;

               if ((usesIndirectAddressing() == TRUE) || (Rhs.usesIndirectAddressing() == TRUE))
                  {
                    printf ("Sorry, not implemented: can't mix indirect addressing (case of where and 2 array objects).\n");
                    APP_ABORT();
                  }
                 else
                  {
                    Array_Set = intArray::Parallel_Conformability_Enforcement
                         (*this, This_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray, Rhs, Rhs_SerialArray );
                  }

            // ...  this will be reset later ...
               Where_Statement_Support::Where_Statement_Mask->Array_Descriptor.SerialArray = Mask_SerialArray; 
             }

       // printf ("DONE: Check Where_Statement_Support::Where_Statement_Mask \n");

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 5)
             {
               Rhs_SerialArray->Array_Descriptor.display("In operator= AFTER ASSIGNMENT Rhs_SerialArray");
               This_SerialArray->Array_Descriptor.display("In operator= AFTER ASSIGNMENT This_SerialArray");
             }
#endif

       // printf ("Check Array_Set \n");

          if (Array_Set == NULL)
               Array_Set = new Array_Conformability_Info_Type();

       // Communication_Manager::Sync();
          APP_ASSERT(Array_Set        != NULL);
          APP_ASSERT(This_SerialArray != NULL);
          APP_ASSERT(Rhs_SerialArray  != NULL);

       // APP_ASSERT(Rhs_SerialArray->Array_Descriptor != NULL);

       // Call the A++ assignment operator!
#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("Call the A++ assignment operator from the P++ assignment operator ... \n");
            // This_SerialArray->view("In intArray::operator= This_SerialArray");
            // Rhs_SerialArray ->view("In intArray::operator= Rhs_SerialArray");
             }
#endif

          *This_SerialArray = *Rhs_SerialArray;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("DONE: Call the A++ assignment operator from the P++ assignment operator ... \n");
               this->displayReferenceCounts("After serial array assignment Lhs in intArray & operator=(intArray,intArray)");
               This_SerialArray->displayReferenceCounts("After serial array assignment This_SerialArray");
               Rhs_SerialArray->displayReferenceCounts("After serial array assignment Rhs_SerialArray");
             }
#endif

       // Replace the where mask
	  if (Where_Statement_Support::Where_Statement_Mask != NULL)
               Where_Statement_Support::Where_Statement_Mask->Array_Descriptor.SerialArray = Old_Mask_SerialArray;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 6)
             {
               Rhs_SerialArray->view("In operator= AFTER ASSIGNMENT Rhs_SerialArray");
               This_SerialArray->view("In operator= AFTER ASSIGNMENT This_SerialArray");
               view("In operator= AFTER ASSIGNMENT *this");
             }
#endif

       // Bugfix (6/15/95) Memory in use reported by Markus (a similar problem 
       // should exist in all the binary operaotrs (so we will fix that next))
       // Now delete the This_SerialArray and Rhs_SerialArray obtained from
       // the intArray::Parallel_Conformability_Enforcement

#if defined(PPP) && defined(USE_PADRE)
       // We have to remove the references in PADRE to the Serial_Array object
       // which is being deleted.  This is a consequence of P++ using PADRE in a way
       // so as to prevent the redundent storage of Array_Domain objects
       // (specifically we use PADRE in a way so that only references are stored).
          setLocalDomainInPADRE_Descriptor(NULL);
          Rhs.setLocalDomainInPADRE_Descriptor(NULL);
#endif

       // printf ("Now delete the This_SerialArray in the P++ operator= (intArray) \n");

          This_SerialArray->decrementReferenceCount();
          if (This_SerialArray->getReferenceCount() < intArray::getReferenceCountBase())
               delete This_SerialArray;

       // printf ("Now delete the Rhs_SerialArray in the P++ operator= (intArray) \n");

       // (12/09/2000) Check for reuse of serialArray object in return value (do not delete it if it was reused)
          if (Rhs_SerialArray->isTemporary() == FALSE)
             {
               Rhs_SerialArray->decrementReferenceCount();
               if (Rhs_SerialArray->getReferenceCount() < intArray::getReferenceCountBase())
                    delete Rhs_SerialArray;
             }

       // printf ("Now delete the Mask_SerialArray in the P++ operator= (intArray) \n");

	  if (Mask_SerialArray != NULL) 
             {
#if defined(PPP) && defined(USE_PADRE)
               Where_Statement_Support::Where_Statement_Mask->setLocalDomainInPADRE_Descriptor(NULL);
#endif
               Mask_SerialArray->decrementReferenceCount();
               if (Mask_SerialArray->getReferenceCount() < intArray::getReferenceCountBase())
                    delete Mask_SerialArray;
             }

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 5)
             {
               view("In operator= AFTER ASSIGNMENT (AND CALL TO DELETE) *this");
             }
#endif
       // Now set these pointers to NULL since we don't use them any more
          This_SerialArray = NULL;
          Rhs_SerialArray  = NULL;
	  Mask_SerialArray = NULL;

       // printf ("Now do whatever message passing that is required in P++ operator= (intArray) \n");

       // Now we need to do the required message passing to fixup the "this" array
       // first check to see if just updating the ghost boundaries is enough

          if (Communication_Manager::Number_Of_Processors > 1)
             {
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
	          {
                    printf ("All ghost boundaries updated together (not seperate treatment for each edge) \n");
                  }
#endif
               if (Array_Set != NULL)
	          {
                    if (Array_Set->Full_VSG_Update_Required == FALSE)
                       {
                         int update_any = FALSE;
                         int i;
                         for (i=0; i < MAX_ARRAY_DIMENSION; i++)
                            {
                              if ((Array_Set->Update_Left_Ghost_Boundary_Width [i] > 0) || 
		                  (Array_Set->Update_Right_Ghost_Boundary_Width[i] > 0)) 
	                           update_any = TRUE;		 
                            }

                         if (update_any)
                            {
#if COMPILE_DEBUG_STATEMENTS
                              if (APP_DEBUG > 0)
                                   printf ("UPDATING GHOST BOUNDARIES (Overlap Update)! \n");

                           // error checking
                              for (i=0; i < MAX_ARRAY_DIMENSION; i++)
                                 {
                                   if ((Array_Set->Update_Left_Ghost_Boundary_Width[i] > 0) || 
                                       (Array_Set->Update_Right_Ghost_Boundary_Width[i] > 0))
                                      {
                                        if (Array_Descriptor.Array_Domain.InternalGhostCellWidth[i] == 0)
                                           {
                                             printf ("ERROR: No ghost cells along axis %d to ",i);
                                             printf ("support message passing ");
                                             printf ("(Full_VSG_Update_Required should be TRUE ");
                                             printf ("but it is FALSE) \n");
                                             APP_ABORT();
                                           }
                                      }

                                // If we are the leftmost processor then it is an error to 
                                // pass boundary info the the left
                                   if (Array_Descriptor.isLeftPartition(i))
                                      {
                                        if (Array_Set->Update_Left_Ghost_Boundary_Width[i] != 0)
                                           {
                                             printf ("ERROR: Left side -- Axis i = %d \n",i);
                                             Array_Set->display("ERROR: Left processor should not pass left -- Array_Set->Update_Left_Ghost_Boundary_Width[i] == 0");
                                             view ("ERROR: Left processor should not pass left -- Array_Set->Update_Left_Ghost_Boundary_Width[i] == 0");
                                           }
                                        APP_ASSERT (Array_Set->Update_Left_Ghost_Boundary_Width[i] == 0);
                                      }

                                // If we are the rightmost processor then it is an error to 
                                // pass boundary info the the right
                                   if (Array_Descriptor.isRightPartition(i))
                                      {
#if COMPILE_DEBUG_STATEMENTS
                                        if (Array_Set->Update_Right_Ghost_Boundary_Width[i] != 0)
                                           {
                                             printf ("ERROR: Right side -- Axis i = %d \n",i);
                                             Array_Set->display("ERROR: Right processor should not pass right -- Array_Set->Update_Right_Ghost_Boundary_Width[i] == 0");
                                             view ("ERROR: Right processor should not pass right -- Array_Set->Update_Right_Ghost_Boundary_Width[i] == 0");
                                           }
#endif
                                        APP_ASSERT (Array_Set->Update_Right_Ghost_Boundary_Width[i] == 0);
                                      }
                                 }
#endif
                           // For now we update all ghost boundaries but later we can be 
                           // more efficient and selective
                           /*
                           // ... (12/11/96,kdb) now only the necessary ghost cells
                           // are updated.  The updateGhostBoundaries code needs
                           // Array_Set to do this.  To avoid passing in an
                           // extra parameter, temporarily attach this to
                           // Array_Descriptor since the Array_Conformability_Info
                           // will be deleted anyways right after this. ...
                           */
                              if (Array_Descriptor.Array_Domain.Array_Conformability_Info != NULL)
                                 {
                                   Array_Descriptor.Array_Domain.Array_Conformability_Info->decrementReferenceCount();
                                   if (Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount() < 
                                       intArray::getReferenceCountBase())
                                        delete Array_Descriptor.Array_Domain.Array_Conformability_Info;
                                 }

                           // ... don't need reference counting ...
                              Array_Descriptor.Array_Domain.Array_Conformability_Info = Array_Set;
                              updateGhostBoundaries();
                              Array_Descriptor.Array_Domain.Array_Conformability_Info = NULL;
                            }
                           else
                            {
                           // No updates specified 
#if COMPILE_DEBUG_STATEMENTS
                              if (APP_DEBUG > 0)
                                   printf ("NO UPDATE TO GHOST BOUNDARIES REQUIRED! \n");
#endif
                            }
                       }
                      else
                       {
                      // Full_VSG_Update_Required == TRUE

#if COMPILE_DEBUG_STATEMENTS
                         if (APP_DEBUG > 0)
                            {
                              printf ("FULL VSG update required (already done) (but now we ");
                              printf ("have to update the ghost boundaries) \n");
                            }
#endif
                      // Since the Regular Section transfer used in PARTI does not update
                      // the ghost boundaries we have to force the update here!
                         updateGhostBoundaries();
                       }
                  }
                 else
                  {
                 // Array_Set is null because indirect addressing was used. 
                 // Update ghost boundaries.
                 // printf ("Calling updateGhostBoundaries() for 1 processor \n");
                    updateGhostBoundaries();
                  }
             } // end of (Communication_Manager::Number_Of_Processors > 1)

       // printf ("DONE with communication for operator= \n");

       // Need to delete the Array_Set to avoid a memory leak
       // Array_Set can be null now if indirect addressing
       // APP_ASSERT (Array_Set != NULL);
          if (Array_Set != NULL);
	     {
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 3)
                  {
                    printf ("Inside of operator=(Rhs): Array_Set->getReferenceCount() = %d \n",
                         Array_Set->getReferenceCount());
                  }
#endif

#if 1
               APP_ASSERT (Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
               Array_Set->decrementReferenceCount();
               if (Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
                    delete Array_Set;
               Array_Set = NULL;
#endif
	     }
        }

#if COMPILE_DEBUG_STATEMENTS
     APP_ASSERT(Array_Descriptor.Array_Domain.Is_A_Temporary == FALSE);
  // printf ("Calling Test_Consistency for Lhs \n");
     Test_Consistency("Test (on return) in intArray::operator= (const intArray & Rhs)");
#endif

#if USE_TEMPORARY_DELETE_FUNCTIONS
  // Manage temporary if it appears on the Rhs or Lhs
  // printf ("In operator= (before Delete_If_Temporary): Rhs.getReferenceCount() = %d \n",Rhs.getReferenceCount());
  // printf ("In operator= (before Delete_If_Temporary): Rhs.Array_Descriptor.SerialArray->getReferenceCount() = %d \n",
  //      Rhs.Array_Descriptor.SerialArray->getReferenceCount());
     Delete_If_Temporary ( Rhs );

  // printf ("In intArray::operator= Array_ID on the stack is %d \n",SerialArray_Domain_Type::queryNextArrayID());

  // printf ("In operator= (before Delete_Lhs_If_Temporary): Lhs.getReferenceCount() = %d \n",this->getReferenceCount());
     Delete_Lhs_If_Temporary ( *this );
#endif

     if (Array_Descriptor.Array_Domain.Array_Conformability_Info != NULL)
        {
          Array_Descriptor.Array_Domain.Array_Conformability_Info->decrementReferenceCount();
          if (Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount() <
              Array_Conformability_Info_Type::getReferenceCountBase())
               delete Array_Descriptor.Array_Domain.Array_Conformability_Info;
          Array_Descriptor.Array_Domain.Array_Conformability_Info = NULL;
        }

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          this->displayReferenceCounts("THIS at BASE of intArray::operator= (intArray)");
        }
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Return from intArray::operator= \n");

  // Commented out (12/14/2000)
  // Communication_Manager::Sync();
#endif

     return *this;
   }

// ************************************************************************************
// This is the abstract operator used for reduction operators
// ************************************************************************************
int
intArray::Abstract_Operator (
   Array_Conformability_Info_Type *Array_Set, const intArray & X, 
   intSerialArray* X_Serial_PCE_Array, int x, int Operation_Type )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of #1 intArray::Abstract_Operator(Array_Conformability_Info_Type,intArray,int,int) \n");

  // This is the only test we can do on the input!
     X.Test_Consistency ("Test in #1 intArray::Abstract_Operator(Array_Conformability_Info_Type,intArray,int,int)");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X at TOP of #1 intArray::Abstract_Operator()");
       // X_Serial_PCE_Array->displayReferenceCounts("X_Serial_PCE_Array at TOP of #1 intArray::Abstract_Operator()");
        }
#endif

  // error checking
     APP_ASSERT(Array_Set != NULL);

  // We have to do a communication on the group of procesors owning 
  // this P++ array object we skip the the communication for now.

#if defined(USE_PADRE)
  // What PADRE function should we call?
  // printf ("NEED TO CALL PADRE \n"); APP_ABORT();
     APP_ASSERT (X.Array_Descriptor.Array_Domain.parallelPADRE_DescriptorPointer != NULL);
#else
     APP_ASSERT (X.Array_Descriptor.Array_Domain.BlockPartiArrayDecomposition != NULL);
#endif
  // The local reduction has already been done and passed in as the scalar "x" so we only have
  // further reduction to do if there are more than 1 processors in the multiprocesor system
     if ( Communication_Manager::Number_Of_Processors > 1 )
        {
          Reduction_Operation ( Operation_Type , x );
        }

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X at BOTTOM of #1 intArray::Abstract_Operator()");
        }
#endif

#if USE_TEMPORARY_DELETE_FUNCTIONS
     Delete_If_Temporary (X);
#endif

     return x;
   }
 

// ************************************************************************
// usual binary operators: operator+ operator- operator* operator/ operator% 
// for array to scalar operations
// ************************************************************************
intArray &
intArray::Abstract_Operator ( 
   Array_Conformability_Info_Type *Array_Set, 
   const intArray & X_ParallelArray, 
   intSerialArray* X_Serial_PCE_Array, 
   intSerialArray & X_SerialArray )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Inside of #2 intArray::Abstract_Operator(Array_Conformability_Info_Type,intArray,intSerialArray) \n");

  // This is the only test we can do on the input!
     X_ParallelArray.Test_Consistency ("Test X_ParallelArray in #2 intArray::Abstract_Operator(Array_Conformability_Info_Type,intArray,intSerialArray)");
     X_SerialArray.Test_Consistency   ("Test X_SerialArray in #2 intArray::Abstract_Operator(Array_Conformability_Info_Type,intArray,intSerialArray)");

  // X_ParallelArray.view("X_ParallelArray in #2 intArray::Abstract_Operator(Array_Conformability_Info_Type,intArray,intSerialArray)");
  // printf ("X_ParallelArray.Array_Descriptor.SerialArray->getBase(0) = %d \n",X_ParallelArray.Array_Descriptor.SerialArray->getBase(0));
  // printf ("X_ParallelArray.Array_Descriptor.SerialArray->getBase(1) = %d \n",X_ParallelArray.Array_Descriptor.SerialArray->getBase(1));
  // printf ("X_ParallelArray.Array_Descriptor.SerialArray->getBase(2) = %d \n",X_ParallelArray.Array_Descriptor.SerialArray->getBase(2));
  // printf ("X_ParallelArray.Array_Descriptor.SerialArray->getBase(3) = %d \n",X_ParallelArray.Array_Descriptor.SerialArray->getBase(3));
  // X_SerialArray.view("X_SerialArray in #2 intArray::Abstract_Operator(Array_Conformability_Info_Type,intArray,intSerialArray)");
#endif

  // error checking
     APP_ASSERT(Array_Set != NULL);

  // The Partitioning array is the array object (usually the Lhs operand of a binary operation) which the 
  // resulting temporary will be partitioned similar to (if it is required to build the temporary).
  // If a scalar was used on the Lhs then we have to use the Rhs as a Partitioning_intArray
  // intArray* Partitioning_Array = &((intArray &) X_ParallelArray);
  // APP_ASSERT (Partitioning_Array != NULL);

  // The size of the temporary must be the same as that of the Lhs (Lhs based owners compute rule)
     intSerialArray *Data = &((intSerialArray &) X_SerialArray);
     APP_ASSERT(Data != NULL);

  // Data->view ("view Data (as X_SerialArray AT TOP) in #2 intArray::Abstract_Operator(Array_Conformability_Info_Type,intArray,intSerialArray)");

  // The size of the temporary must be the same as that of the Lhs (Lhs based owners compute rule)
     intArray* Temporary = NULL;

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X_SerialArray.displayReferenceCounts("X_SerialArray at TOP of #2 intArray::Abstract_Operator()");
        }
#endif

     if ( X_ParallelArray.isTemporary() )
        {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Inside of #2 intArray::Abstract_Operator(): Reuse X_ParallelArray \n");
#endif

       // Temporary reuse of serial array already handled at Serial_A++ level
          Temporary = &((intArray &) X_ParallelArray);

       // Temporary->view ("view temporary (as X_ParallelArray BEFORE fixupLocalBase) in #2 intArray::Abstract_Operator(Array_Conformability_Info_Type,intArray,intSerialArray)");

          Array_Domain_Type::fixupLocalBase 
	     ( (Array_Domain_Type&)X_ParallelArray.Array_Descriptor.Array_Domain, 
	       Data->Array_Descriptor.Array_Domain , 
               X_ParallelArray.Array_Descriptor.Array_Domain , 
               X_ParallelArray.Array_Descriptor.SerialArray->
	       Array_Descriptor.Array_Domain );

       // Bugfix (12/3/2000) we have to place the X_SerialArray into the Temporary
          Delete_If_Temporary (*X_ParallelArray.Array_Descriptor.SerialArray);
          ((intArray &) X_ParallelArray).Array_Descriptor.SerialArray = Data;

       // Temporary->view ("view temporary (as X_ParallelArray BEFORE Modify_Reference_Counts_And_Manage_Temporaries) in #2 intArray::Abstract_Operator(Array_Conformability_Info_Type,intArray,intSerialArray)");

       // Modify_Reference_Counts_And_Manage_Temporaries ( Array_Set, Temporary, X_Serial_PCE_Array, Data );
          APP_ASSERT (Data->isTemporary() == TRUE);

#if COMPILE_DEBUG_STATEMENTS
          if (Diagnostic_Manager::getReferenceCountingReport() > 0)
             {
            // This mechanism outputs reports which allow us to trace the reference counts
               Temporary->displayReferenceCounts("Temporary (after Modify_Reference_Counts_And_Manage_Temporaries) of #2 intArray::Abstract_Operator()");
               X_SerialArray.displayReferenceCounts("X_SerialArray (after Modify_Reference_Counts_And_Manage_Temporaries) of #2 intArray::Abstract_Operator()");
             }
#endif

       // Temporary->view ("view temporary (as X_ParallelArray AFTER MRCAMT) in #2 intArray::Abstract_Operator(Array_Conformability_Info_Type,intArray,intSerialArray)");
       // APP_ABORT();

#if COMPILE_DEBUG_STATEMENTS
          X_ParallelArray.Test_Consistency ("Test X_ParallelArray (temporary) in #2 intArray::Abstract_Operator(Array_Conformability_Info_Type,intArray,intSerialArray)");
#endif
       // Data->view("Data (temporary) in #2 intArray::Abstract_Operator(Array_Conformability_Info_Type,intArray,intSerialArray)");
       // X_ParallelArray.view("X_ParallelArray in Abstract_Operator");
       // APP_ABORT();
        }
       else
        {
       // No temporary to reuse so we have to build one!

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Inside of #2 intArray::Abstract_Operator(): Build a new array to be the temporary \n");
#endif

       // This is more efficient than the previous version which build a descriptor and
       // then forced 2 copies of the descriptor before ending up with a temporary to use.
          bool AvoidBuildingIndirectAddressingView = TRUE;
       // Data->view("In abstract_op #2: Data");
       // X_ParallelArray.Array_Descriptor.Array_Domain.display("In abstract_op #2: X_ParallelArray: Array_Domain");
          Temporary = new intArray ( Data , 
                                    &(X_ParallelArray.Array_Descriptor.Array_Domain),
                                    AvoidBuildingIndirectAddressingView );
          APP_ASSERT (Temporary != NULL);
          APP_ASSERT (Temporary->Array_Descriptor.SerialArray != NULL);

       // printf ("In #2 intArray::Abstract_Operator: Temporary->Array_Descriptor.SerialArray->getReferenceCount() = %d \n",
       //      Temporary->Array_Descriptor.SerialArray->getReferenceCount());
       // APP_ASSERT (Temporary->Array_Descriptor.SerialArray->getReferenceCount() > 1);

       // The base now needs to be reset since temporaries always have APP_Global_Array_Base as a base
       // ... don't reset this if indirect addressing is used ...
	  if (!Temporary->Array_Descriptor.Array_Domain.Uses_Indirect_Addressing)
             Temporary->setBase(APP_Global_Array_Base);
          
       // Temporary->view ("view temporary in #2 intArray::Abstract_Operator(Array_Conformability_Info_Type,intArray,intSerialArray)");
       // APP_ABORT();

          Temporary->setTemporary(TRUE);

       /*
       // ... (4/28/98, kdb) added this function call, don't know what 
       //   happened to it ...
       // ... (4/29/98, kdb) not sure this is needed ...
       */

       /*
          Array_Domain_Type::fixupLocalBase
	     (Descriptor->Array_Domain, Data->Array_Descriptor.Array_Domain,
	      X_ParallelArray.Array_Descriptor.Array_Domain,
	      X_ParallelArray.Array_Descriptor.SerialArray->
	      Array_Descriptor.Array_Domain);
       */

       // (11/4/2000) I think this is always NULL for the temporary just built (verify)!
          APP_ASSERT (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info == NULL);

       // This is how we acumulate the information used to determine message passing 
       // when the operator= is executed (to terminate the expression statement)!
          if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info != NULL)
             {
            // printf ("Commented out Array_Conformability_Info_Type::delete \n");
               Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->decrementReferenceCount();
               if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount() < 
                   Array_Conformability_Info_Type::getReferenceCountBase())
                    delete Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info;
             }
          Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info = Array_Set;

       // Bugfix (11/27/2000)
          Array_Set->incrementReferenceCount();
        }

#if COMPILE_DEBUG_STATEMENTS
     APP_ASSERT(Temporary->Array_Descriptor.Array_Domain.Is_A_Temporary == TRUE);
     Temporary->Test_Consistency ("Test (on return) in #2 intArray::Abstract_Operator(ACIT,intArray,intSerialArray)");
     if (APP_DEBUG > 5)
        {
          Temporary->view ("view temporary (on return) in #2 intArray::Abstract_Operator(ACIT,intArray,intSerialArray)");
        }

     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Temporary->displayReferenceCounts("Temporary at BASE of #2 intArray::Abstract_Operator()");
          X_SerialArray.displayReferenceCounts("X_SerialArray at BASE of #2 intArray::Abstract_Operator()");
        }
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving #2 intArray::Abstract_Operator(Array_Conformability_Info_Type,intArray,intSerialArray) \n");
#endif

     return *Temporary;
   }





#if !defined(INTARRAY)
// if !defined(INTARRAY)
// ************************************************************************
// usual binary operators: operator+ operator- operator* operator/ operator% 
// for array to scalar operations
// ************************************************************************
intArray & 
intArray::Abstract_Operator (
     Array_Conformability_Info_Type *Array_Set, 
     const intArray & X_ParallelArray, 
     intSerialArray* X_Serial_PCE_Array,
     intSerialArray & X_SerialArray )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of #3 intArray::Abstract_Operator(Array_Conformability_Info_Type,intArray,intSerialArray) \n");

  // This is the only test we can do on the input!
     X_ParallelArray.Test_Consistency ("Test X_ParallelArray in #3 intArray::Abstract_Operator(Array_Conformability_Info_Type,intArray,intSerialArray) ");
     X_SerialArray.Test_Consistency ("Test X_SerialArray in #3 intArray::Abstract_Operator(Array_Conformability_Info_Type,intArray,intSerialArray) ");
#endif

  // error checking
     APP_ASSERT(Array_Set != NULL);

  // The size of the temporary must be the same as that of the Lhs (Lhs based owners compute rule)
     intSerialArray *Data = &((intSerialArray &) X_SerialArray);
     APP_ASSERT(Data != NULL);

     intArray *Temporary = NULL;
  // This is more efficient than the previous version which build a descriptor and
  // then forced 2 copies of the descriptor before ending up with a temporary to use.
     bool AvoidBuildingIndirectAddressingView = TRUE;
  // Data->view("In abstract_op #2: Data");
  // X_ParallelArray.Array_Descriptor.Array_Domain.display("In abstract_op #2: X_ParallelArray: Array_Domain");
     Temporary = new intArray ( Data , 
                               &(X_ParallelArray.Array_Descriptor.Array_Domain),
                               AvoidBuildingIndirectAddressingView );

     APP_ASSERT (Temporary != NULL);
     APP_ASSERT (Temporary->Array_Descriptor.SerialArray != NULL);
  // printf ("In #3 intArray::Abstract_Operator: Temporary->Array_Descriptor.SerialArray->getReferenceCount() = %d \n",
  //      Temporary->Array_Descriptor.SerialArray->getReferenceCount());
  // APP_ASSERT (Temporary->Array_Descriptor.SerialArray->getReferenceCount() > 1);

  // The base now needs to be reset since temporaries always have APP_Global_Array_Base as a base
  // ... don't reset this if indirect addressing is used ...
     if (!Temporary->Array_Descriptor.Array_Domain.Uses_Indirect_Addressing)
        Temporary->setBase(APP_Global_Array_Base);
          
  // Temporary->view ("view temporary in #3 intArray::Abstract_Operator(Array_Conformability_Info_Type,intArray,intSerialArray)");
  // APP_ABORT();

     Temporary->setTemporary(TRUE);

  // ... maybe this isn't needed with the new constructor ...

     /*
     Array_Domain_Type::fixupLocalBase
	( Descriptor->Array_Domain, Data->Array_descriptor.Array_Domain,
	  X_ParallelArray.Array_Descriptor.Array_Domain,
	  X_ParallelArray.Array_Descriptor.SerialArray->
	  Array_Descriptor.Array_Domain);
     */

  // Test (12/4/2000) I think this might be true!
     APP_ASSERT (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info == NULL);

  // This is now we acumulate the information used to determine message passing 
  // when the operator= is executed!
     if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info != NULL)
        {
       // printf ("Commented out Array_Conformability_Info_Type::delete \n");
          Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->decrementReferenceCount();
          if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount() < 
              Array_Conformability_Info_Type::getReferenceCountBase())
               delete Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info;
        }

     Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info = Array_Set;
  // Bugfix (11/27/2000)
     Array_Set->incrementReferenceCount();

#if COMPILE_DEBUG_STATEMENTS
     APP_ASSERT(Temporary->isTemporary() == TRUE);
     Temporary->Test_Consistency ("Test (on return) in #3 intArray::Abstract_Operator(Array_Domain.Array_Conformability_Info_Type,intArray,intSerialArray)");

  // Temporary->displayReferenceCounts("Called inside of #3 intArray::Abstract_Operator");
  // X_SerialArray.displayReferenceCounts("X_SerialArray at BASE of #3 intArray::Abstract_Operator()");
#endif

     return *Temporary;
   }
// endif



#endif



// if !defined(INTARRAY)
// ************************************************************************
// usual binary operators: operator+ operator- operator* operator/ operator% 
// for array to scalar operations
// ************************************************************************
intArray & 
intArray::Abstract_int_Conversion_Operator (
     Array_Conformability_Info_Type *Array_Set, 
     const intArray & X_ParallelArray, 
     intSerialArray* X_Serial_PCE_Array,
     intSerialArray & X_SerialArray )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of #3 intArray::Abstract_Operator(Array_Conformability_Info_Type,intArray,intSerialArray) \n");

  // This is the only test we can do on the input!
     X_ParallelArray.Test_Consistency ("Test X_ParallelArray in #3 intArray::Abstract_Operator(Array_Conformability_Info_Type,intArray,intSerialArray) ");
     X_SerialArray.Test_Consistency ("Test X_SerialArray in #3 intArray::Abstract_Operator(Array_Conformability_Info_Type,intArray,intSerialArray) ");
#endif

  // error checking
     APP_ASSERT(Array_Set != NULL);

  // The size of the temporary must be the same as that of the Lhs (Lhs based owners compute rule)
     intSerialArray *Data = &((intSerialArray &) X_SerialArray);
     APP_ASSERT(Data != NULL);

     intArray *Temporary = NULL;
  // This is more efficient than the previous version which build a descriptor and
  // then forced 2 copies of the descriptor before ending up with a temporary to use.
     bool AvoidBuildingIndirectAddressingView = TRUE;
  // Data->view("In abstract_op #2: Data");
  // X_ParallelArray.Array_Descriptor.Array_Domain.display("In abstract_op #2: X_ParallelArray: Array_Domain");
     Temporary = new intArray ( Data , 
                               &(X_ParallelArray.Array_Descriptor.Array_Domain),
                               AvoidBuildingIndirectAddressingView );

     APP_ASSERT (Temporary != NULL);
     APP_ASSERT (Temporary->Array_Descriptor.SerialArray != NULL);
  // printf ("In #3 intArray::Abstract_Operator: Temporary->Array_Descriptor.SerialArray->getReferenceCount() = %d \n",
  //      Temporary->Array_Descriptor.SerialArray->getReferenceCount());
  // APP_ASSERT (Temporary->Array_Descriptor.SerialArray->getReferenceCount() > 1);

  // The base now needs to be reset since temporaries always have APP_Global_Array_Base as a base
  // ... don't reset this if indirect addressing is used ...
     if (!Temporary->Array_Descriptor.Array_Domain.Uses_Indirect_Addressing)
        Temporary->setBase(APP_Global_Array_Base);
          
  // Temporary->view ("view temporary in #3 intArray::Abstract_Operator(Array_Conformability_Info_Type,intArray,intSerialArray)");
  // APP_ABORT();

     Temporary->setTemporary(TRUE);

  // ... maybe this isn't needed with the new constructor ...

     /*
     Array_Domain_Type::fixupLocalBase
	( Descriptor->Array_Domain, Data->Array_descriptor.Array_Domain,
	  X_ParallelArray.Array_Descriptor.Array_Domain,
	  X_ParallelArray.Array_Descriptor.SerialArray->
	  Array_Descriptor.Array_Domain);
     */

  // Test (12/4/2000) I think this might be true!
     APP_ASSERT (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info == NULL);

  // This is now we acumulate the information used to determine message passing 
  // when the operator= is executed!
     if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info != NULL)
        {
       // printf ("Commented out Array_Conformability_Info_Type::delete \n");
          Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->decrementReferenceCount();
          if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount() < 
              Array_Conformability_Info_Type::getReferenceCountBase())
               delete Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info;
        }

     Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info = Array_Set;
  // Bugfix (11/27/2000)
     Array_Set->incrementReferenceCount();

#if COMPILE_DEBUG_STATEMENTS
     APP_ASSERT(Temporary->isTemporary() == TRUE);
     Temporary->Test_Consistency ("Test (on return) in #3 intArray::Abstract_Operator(Array_Domain.Array_Conformability_Info_Type,intArray,intSerialArray)");

  // Temporary->displayReferenceCounts("Called inside of #3 intArray::Abstract_Operator");
  // X_SerialArray.displayReferenceCounts("X_SerialArray at BASE of #3 intArray::Abstract_Operator()");
#endif

     return *Temporary;
   }
// endif



// if !defined(INTARRAY)
// ************************************************************************
// usual binary operators: operator+ operator- operator* operator/ operator% 
// for array to scalar operations
// ************************************************************************
floatArray & 
intArray::Abstract_float_Conversion_Operator (
     Array_Conformability_Info_Type *Array_Set, 
     const intArray & X_ParallelArray, 
     intSerialArray* X_Serial_PCE_Array,
     floatSerialArray & X_SerialArray )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of #3 intArray::Abstract_Operator(Array_Conformability_Info_Type,intArray,intSerialArray) \n");

  // This is the only test we can do on the input!
     X_ParallelArray.Test_Consistency ("Test X_ParallelArray in #3 intArray::Abstract_Operator(Array_Conformability_Info_Type,intArray,intSerialArray) ");
     X_SerialArray.Test_Consistency ("Test X_SerialArray in #3 intArray::Abstract_Operator(Array_Conformability_Info_Type,intArray,floatSerialArray) ");
#endif

  // error checking
     APP_ASSERT(Array_Set != NULL);

  // The size of the temporary must be the same as that of the Lhs (Lhs based owners compute rule)
     floatSerialArray *Data = &((floatSerialArray &) X_SerialArray);
     APP_ASSERT(Data != NULL);

     floatArray *Temporary = NULL;
  // This is more efficient than the previous version which build a descriptor and
  // then forced 2 copies of the descriptor before ending up with a temporary to use.
     bool AvoidBuildingIndirectAddressingView = TRUE;
  // Data->view("In abstract_op #2: Data");
  // X_ParallelArray.Array_Descriptor.Array_Domain.display("In abstract_op #2: X_ParallelArray: Array_Domain");
     Temporary = new floatArray ( Data , 
                               &(X_ParallelArray.Array_Descriptor.Array_Domain),
                               AvoidBuildingIndirectAddressingView );

     APP_ASSERT (Temporary != NULL);
     APP_ASSERT (Temporary->Array_Descriptor.SerialArray != NULL);
  // printf ("In #3 intArray::Abstract_Operator: Temporary->Array_Descriptor.SerialArray->getReferenceCount() = %d \n",
  //      Temporary->Array_Descriptor.SerialArray->getReferenceCount());
  // APP_ASSERT (Temporary->Array_Descriptor.SerialArray->getReferenceCount() > 1);

  // The base now needs to be reset since temporaries always have APP_Global_Array_Base as a base
  // ... don't reset this if indirect addressing is used ...
     if (!Temporary->Array_Descriptor.Array_Domain.Uses_Indirect_Addressing)
        Temporary->setBase(APP_Global_Array_Base);
          
  // Temporary->view ("view temporary in #3 intArray::Abstract_Operator(Array_Conformability_Info_Type,intArray,intSerialArray)");
  // APP_ABORT();

     Temporary->setTemporary(TRUE);

  // ... maybe this isn't needed with the new constructor ...

     /*
     Array_Domain_Type::fixupLocalBase
	( Descriptor->Array_Domain, Data->Array_descriptor.Array_Domain,
	  X_ParallelArray.Array_Descriptor.Array_Domain,
	  X_ParallelArray.Array_Descriptor.SerialArray->
	  Array_Descriptor.Array_Domain);
     */

  // Test (12/4/2000) I think this might be true!
     APP_ASSERT (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info == NULL);

  // This is now we acumulate the information used to determine message passing 
  // when the operator= is executed!
     if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info != NULL)
        {
       // printf ("Commented out Array_Conformability_Info_Type::delete \n");
          Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->decrementReferenceCount();
          if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount() < 
              Array_Conformability_Info_Type::getReferenceCountBase())
               delete Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info;
        }

     Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info = Array_Set;
  // Bugfix (11/27/2000)
     Array_Set->incrementReferenceCount();

#if COMPILE_DEBUG_STATEMENTS
     APP_ASSERT(Temporary->isTemporary() == TRUE);
     Temporary->Test_Consistency ("Test (on return) in #3 intArray::Abstract_Operator(Array_Domain.Array_Conformability_Info_Type,intArray,floatSerialArray)");

  // Temporary->displayReferenceCounts("Called inside of #3 intArray::Abstract_Operator");
  // X_SerialArray.displayReferenceCounts("X_SerialArray at BASE of #3 intArray::Abstract_Operator()");
#endif

     return *Temporary;
   }
// endif



// if !defined(INTARRAY)
// ************************************************************************
// usual binary operators: operator+ operator- operator* operator/ operator% 
// for array to scalar operations
// ************************************************************************
doubleArray & 
intArray::Abstract_double_Conversion_Operator (
     Array_Conformability_Info_Type *Array_Set, 
     const intArray & X_ParallelArray, 
     intSerialArray* X_Serial_PCE_Array,
     doubleSerialArray & X_SerialArray )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of #3 intArray::Abstract_Operator(Array_Conformability_Info_Type,intArray,intSerialArray) \n");

  // This is the only test we can do on the input!
     X_ParallelArray.Test_Consistency ("Test X_ParallelArray in #3 intArray::Abstract_Operator(Array_Conformability_Info_Type,intArray,intSerialArray) ");
     X_SerialArray.Test_Consistency ("Test X_SerialArray in #3 intArray::Abstract_Operator(Array_Conformability_Info_Type,intArray,doubleSerialArray) ");
#endif

  // error checking
     APP_ASSERT(Array_Set != NULL);

  // The size of the temporary must be the same as that of the Lhs (Lhs based owners compute rule)
     doubleSerialArray *Data = &((doubleSerialArray &) X_SerialArray);
     APP_ASSERT(Data != NULL);

     doubleArray *Temporary = NULL;
  // This is more efficient than the previous version which build a descriptor and
  // then forced 2 copies of the descriptor before ending up with a temporary to use.
     bool AvoidBuildingIndirectAddressingView = TRUE;
  // Data->view("In abstract_op #2: Data");
  // X_ParallelArray.Array_Descriptor.Array_Domain.display("In abstract_op #2: X_ParallelArray: Array_Domain");
     Temporary = new doubleArray ( Data , 
                               &(X_ParallelArray.Array_Descriptor.Array_Domain),
                               AvoidBuildingIndirectAddressingView );

     APP_ASSERT (Temporary != NULL);
     APP_ASSERT (Temporary->Array_Descriptor.SerialArray != NULL);
  // printf ("In #3 intArray::Abstract_Operator: Temporary->Array_Descriptor.SerialArray->getReferenceCount() = %d \n",
  //      Temporary->Array_Descriptor.SerialArray->getReferenceCount());
  // APP_ASSERT (Temporary->Array_Descriptor.SerialArray->getReferenceCount() > 1);

  // The base now needs to be reset since temporaries always have APP_Global_Array_Base as a base
  // ... don't reset this if indirect addressing is used ...
     if (!Temporary->Array_Descriptor.Array_Domain.Uses_Indirect_Addressing)
        Temporary->setBase(APP_Global_Array_Base);
          
  // Temporary->view ("view temporary in #3 intArray::Abstract_Operator(Array_Conformability_Info_Type,intArray,intSerialArray)");
  // APP_ABORT();

     Temporary->setTemporary(TRUE);

  // ... maybe this isn't needed with the new constructor ...

     /*
     Array_Domain_Type::fixupLocalBase
	( Descriptor->Array_Domain, Data->Array_descriptor.Array_Domain,
	  X_ParallelArray.Array_Descriptor.Array_Domain,
	  X_ParallelArray.Array_Descriptor.SerialArray->
	  Array_Descriptor.Array_Domain);
     */

  // Test (12/4/2000) I think this might be true!
     APP_ASSERT (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info == NULL);

  // This is now we acumulate the information used to determine message passing 
  // when the operator= is executed!
     if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info != NULL)
        {
       // printf ("Commented out Array_Conformability_Info_Type::delete \n");
          Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->decrementReferenceCount();
          if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount() < 
              Array_Conformability_Info_Type::getReferenceCountBase())
               delete Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info;
        }

     Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info = Array_Set;
  // Bugfix (11/27/2000)
     Array_Set->incrementReferenceCount();

#if COMPILE_DEBUG_STATEMENTS
     APP_ASSERT(Temporary->isTemporary() == TRUE);
     Temporary->Test_Consistency ("Test (on return) in #3 intArray::Abstract_Operator(Array_Domain.Array_Conformability_Info_Type,intArray,doubleSerialArray)");

  // Temporary->displayReferenceCounts("Called inside of #3 intArray::Abstract_Operator");
  // X_SerialArray.displayReferenceCounts("X_SerialArray at BASE of #3 intArray::Abstract_Operator()");
#endif

     return *Temporary;
   }
// endif




// ************************************************************************
// usual binary operators: operator+ operator- operator* operator/ operator% 
// for array to array operations
// ************************************************************************
intArray &
intArray::Abstract_Operator (
     Array_Conformability_Info_Type *Array_Set, 
     const intArray & Lhs_ParallelArray, const intArray & Rhs_ParallelArray, 
     intSerialArray* Lhs_Serial_PCE_Array, intSerialArray* Rhs_Serial_PCE_Array, 
     intSerialArray & X_SerialArray )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of #4 intArray::Abstract_Operator(Array_Conformability_Info_Type,intArray,intArray,intSerialArray) \n");
#endif

  // error checking
     APP_ASSERT(Array_Set != NULL);

#if COMPILE_DEBUG_STATEMENTS
  // printf ("intArray::Abstract_Operator for binary operators! \n");
  // These are the only tests we can do on the input!
     Array_Set->Test_Consistency("Test Array_Set in #4 intArray::Abstract_Operator(Array_Conformability_Info_Type,intArray,intArray,intSerialArray)");
     Lhs_ParallelArray.Test_Consistency ("Test LHS in #4 intArray::Abstract_Operator(Array_Conformability_Info_Type,intArray,intArray,intSerialArray)");
     Rhs_ParallelArray.Test_Consistency ("Test RHS in #4 intArray::Abstract_Operator(Array_Conformability_Info_Type,intArray,intArray,intSerialArray)");
     X_SerialArray.Test_Consistency     ("Test X_SerialArray in #4 intArray::Abstract_Operator(Array_Conformability_Info_Type,intArray,intArray,intSerialArray)");
#endif

  // Used for loop indexing
  // int i = 0;

     if (Index::Index_Bounds_Checking)
          Lhs_ParallelArray.Test_Conformability (Rhs_ParallelArray);

  // The Partitioning array is the array object (usually the Lhs operand) which the 
  // resulting temporary will be partitioned similar to (if it is required to build the temporary).
  // If a scalar was used on the Lhs then we have to use the Rhs as a Partitioning_intArray
  // intArray* Partitioning_Array = &((intArray &) Lhs_ParallelArray);
  // APP_ASSERT (Partitioning_Array != NULL);

  // The size of the temporary must be the same as that of the Lhs (Lhs based owners compute rule)
     intSerialArray *Data = &((intSerialArray &) X_SerialArray);
     APP_ASSERT(Data != NULL);

#if COMPILE_DEBUG_STATEMENTS
  // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
     if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
        {
          printf ("SerialArray input to intArray::Abstract_Operator X_SerialArray: local range (%d,%d,%d) \n",
               X_SerialArray.getBase(0),
               X_SerialArray.getBound(0),
               X_SerialArray.getStride(0));
        }
#endif

     intArray* Temporary = NULL;
     Memory_Source_Type Result_Is_Lhs_Or_Rhs_Or_New_Memory = Uninitialized_Source;
     if ( Lhs_ParallelArray.isTemporary() == TRUE )
        {
       // Temporary reuse of serial array already handled at Serial_A++ level
#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
               printf ("Reuse the Lhs since it is a temporary! \n");
#endif
          Temporary = &((intArray &) Lhs_ParallelArray);
          Result_Is_Lhs_Or_Rhs_Or_New_Memory = Memory_From_Lhs;

       // Delete the old Array_Conformability_Info object so that the new one can be inserted
          if (Lhs_ParallelArray.Array_Descriptor.Array_Domain.Array_Conformability_Info != NULL)
             {
               Lhs_ParallelArray.Array_Descriptor.Array_Domain.Array_Conformability_Info->decrementReferenceCount();
               if (Lhs_ParallelArray.Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount() < 
                   Array_Conformability_Info_Type::getReferenceCountBase())
                    delete Lhs_ParallelArray.Array_Descriptor.Array_Domain.Array_Conformability_Info;
             }

#if COMPILE_DEBUG_STATEMENTS
          Array_Set->Test_Consistency("Test Array_Set in #4 intArray::Abstract_Operator (after delete Lhs_ParallelArray.Array_Descriptor.Array_Domain.Array_Conformability_Info)");
#endif
       // ... a new Array_Set is computed now even if Lhs has one ...
       // APP_ASSERT( Lhs_ParallelArray.Array_Descriptor.Array_Domain.Array_Conformability_Info == Array_Set );
          ((intArray&)Lhs_ParallelArray).Array_Descriptor.Array_Domain.Array_Conformability_Info = Array_Set;
       // Bugfix (11/27/2000)
          Array_Set->incrementReferenceCount();

#if COMPILE_DEBUG_STATEMENTS
          Array_Set->Test_Consistency("Lhs_ParallelArray.Array_Descriptor.Array_Domain.Array_Conformability_Info == Array_Set at MIPOINT of #4 intArray::Abstract_Operator(Array_Conformability_Info_Type,intArray,intArray,intSerialArray)");
#endif

       // Temporary->displayReferenceCounts("Temporary at MIPOINT of #4 intArray::Abstract_Operator");

          Array_Domain_Type::fixupLocalBase 
	     ( (Array_Domain_Type&)Lhs_ParallelArray.Array_Descriptor.Array_Domain , 
	       Data->Array_Descriptor.Array_Domain , 
               Lhs_ParallelArray.Array_Descriptor.Array_Domain , 
               Lhs_ParallelArray.Array_Descriptor.SerialArray->
	       Array_Descriptor.Array_Domain );

       // Lhs_ParallelArray.view("In P++ Abstract_Operator() Lhs_ParallelArray");

          APP_ASSERT (Temporary != NULL);
          APP_ASSERT (Temporary->Array_Descriptor.SerialArray != NULL);
          APP_ASSERT (Temporary->Array_Descriptor.SerialArray->getReferenceCount() >= intArray::getReferenceCountBase());
       // Delete the serial array in the Temporary that will be reused 
       // (so it can be replaced by the input serialArray!)
          Temporary->Array_Descriptor.SerialArray->decrementReferenceCount();
          if (Temporary->Array_Descriptor.SerialArray->getReferenceCount() < intArray::getReferenceCountBase())
               delete Temporary->Array_Descriptor.SerialArray;

       // Copy the input serialArray into the temporary (the return value)
          Temporary->Array_Descriptor.SerialArray = Data;

       // Temporary->displayReferenceCounts("Temporary after insertion of X_SerialArray in #4 intArray::Abstract_Operator");

          APP_ASSERT (Data->getRawDataReferenceCount() >= getRawDataReferenceCountBase());
       // printf ("Exiting in reuse of Lhs as a test ... \n");
       // APP_ABORT();
        }
       else
        {
       // In the case of a Full_VSG_Update only the Lhs can be reused the Rhs may not be reused
       // This is because the Rhs is reconstructed for each binary operation and the data for the
       // Rhs is assembled into the newly constructed Rhs (which is the size of the local Lhs)
       // from all the processors owning that part of the global indexspace required.
       // This detail was unexpected and might be handled more efficiently in the future.
       // if ( Rhs_ParallelArray.Array_Descriptor.Array_Domain.Is_A_Temporary == TRUE )
          if ( (Rhs_ParallelArray.Array_Descriptor.Array_Domain.Is_A_Temporary == TRUE) && 
               (Array_Set->Full_VSG_Update_Required == FALSE) )
             {
            // Temporary reuse of serial array already handled at Serial_A++ level
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Reuse the Rhs since it is a temporary! \n");
#endif
            // printf ("Reuse the Rhs since it is a temporary! Exiting ... \n");
            // APP_ABORT();

               Temporary = &((intArray &) Rhs_ParallelArray);
               Result_Is_Lhs_Or_Rhs_Or_New_Memory = Memory_From_Rhs;

               if (Rhs_ParallelArray.Array_Descriptor.Array_Domain.Array_Conformability_Info != NULL)
                  {
#if COMPILE_DEBUG_STATEMENTS
                    if (APP_DEBUG > 0)
                         printf ("delete for Array_Conformability_Info_Type called in intArray::Abstract_Operator (Array_Conformability_Info_Type,intArray,intArray,intSerialArray)");
#endif
                    Rhs_ParallelArray.Array_Descriptor.Array_Domain.Array_Conformability_Info->decrementReferenceCount();
                    if (Rhs_ParallelArray.Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount() < 
                        Array_Conformability_Info_Type::getReferenceCountBase())
                         delete Rhs_ParallelArray.Array_Descriptor.Array_Domain.Array_Conformability_Info;
                  }

#if COMPILE_DEBUG_STATEMENTS
               Array_Set->Test_Consistency("Test Array_Set in #4 intArray::Abstract_Operator (after delete Rhs_ParallelArray.Array_Descriptor.Array_Domain.Array_Conformability_Info)");
#endif
               // ... TEMP TEST:  there is no reason this should br
	       //   true now ...
               //APP_ASSERT( Rhs_ParallelArray.Array_Descriptor.Array_Domain.Array_Conformability_Info == Array_Set );

               ((intArray&)Rhs_ParallelArray).Array_Descriptor.Array_Domain.Array_Conformability_Info = Array_Set;
            // Bugfix (11/27/2000)
               Array_Set->incrementReferenceCount();

            // Data->view("Data BEFORE setBase");

               Array_Domain_Type::fixupLocalBase 
		  ((Array_Domain_Type&)Rhs_ParallelArray.Array_Descriptor.Array_Domain , 
		   Data->Array_Descriptor.Array_Domain , 
                   Rhs_ParallelArray.Array_Descriptor.Array_Domain , 
                   Rhs_ParallelArray.Array_Descriptor.SerialArray->
		   Array_Descriptor.Array_Domain );

	       // ... (10/11/96,kdb) correct Array_Set because it is valid
	       //  only for Lhs ...

               // ... temporary is consistent with Rhs so reverse offsets ...
               bool reverse_offset = TRUE;

               Array_Domain_Type:: Fix_Array_Conformability 
		  ( *Array_Set, Rhs_ParallelArray.Array_Descriptor.Array_Domain , 
		     Data->Array_Descriptor.Array_Domain, reverse_offset );

#if 0
               Modify_Reference_Counts_And_Manage_Temporaries ( Array_Set, Temporary, Lhs_Serial_PCE_Array, Rhs_Serial_PCE_Array, Data );
#endif

            // Temporary->Array_Descriptor.SerialArray = Data;

            // Bugfix (2/5/96) Must increment reference count since the operator.C code
            // will delete the serial arrays that the PCE returns.
            // printf ("Increment reference count on array reused (LHS)! \n");
            // While we have to increment the SerialArray's reference count so that the delete in the 
            // binary operator will not destroy the SerialArray --- we have to decrement the RawDataReferenceCount
            // because we want to use the raw data obtained though the view taken in PCE and
            // we will not be using the original array from which that view was taken.

            // To complicate issues the relational operators return a newly built temporary and so 
            // in this case the reference count of the data is ZERO and we can't (and should not) 
            // decrement it.
            // if (Data->getRawDataReferenceCount() > 0)
            //      Data->decrementRawDataReferenceCount();
            // Data->incrementReferenceCount();
               APP_ASSERT (Data->getRawDataReferenceCount() >= getRawDataReferenceCountBase());

            // printf ("Exiting in reuse of Rhs as a test ... \n");
            // APP_ABORT();

            // Temporary->view("Temporary");
             }
            else
             {
            // No temporary to reuse so we have to build one!
            // later we can avoid the construction of a new descriptor

#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Build a temporary since neither Lhs nor Rhs is a temporary! \n");
#endif
            // printf ("Build a temporary since neither Lhs nor Rhs is a temporary! \n");
            // APP_ABORT();
            // We should better call the intArray_Descriptor_Type::Build_Temporary_By_Example
            // Now we have a descriptor based on the descriptor from the Lhs operand.  But we
            // have to fix it up in order to use it. Or mabe we should change the design so 
            // we don't have to change it (i.e. no re-centering would then be required) 

            // printf ("ERROR: In Abstract_Operator we are doing redundent copying (exit until this is fixed) \n");
            // APP_ABORT();
            // This is more efficient than the previous version which build a descriptor and
            // then forced 2 copies of the descriptor before ending up with a temporary to use.
               bool AvoidBuildingIndirectAddressingView = TRUE;
            // Data->view("In abstract_op #2: Data");
            // X_ParallelArray.Array_Descriptor.Array_Domain.display("In abstract_op #2: X_ParallelArray: Array_Domain");
#if COMPILE_DEBUG_STATEMENTS
            // Temporary->view ("view temporary in #4 intArray::Abstract_Operator(): AFTER Temporary is built!");
               if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
                  {
                    printf ("(Abstract_Operator()) - Lhs_ParallelArray: global range (%d,%d,%d) local range (%d,%d,%d) \n",
                         Lhs_ParallelArray.getGlobalMaskIndex(0).getBase(),
                         Lhs_ParallelArray.getGlobalMaskIndex(0).getBound(),
                         Lhs_ParallelArray.getGlobalMaskIndex(0).getStride(),
                         Lhs_ParallelArray.getLocalMaskIndex(0).getBase(),
                         Lhs_ParallelArray.getLocalMaskIndex(0).getBound(),
                         Lhs_ParallelArray.getLocalMaskIndex(0).getStride());
                  }
#endif

               Temporary = new intArray ( Data ,
                                         &(Lhs_ParallelArray.Array_Descriptor.Array_Domain),
                                         AvoidBuildingIndirectAddressingView );

#if COMPILE_DEBUG_STATEMENTS
            // Temporary->view ("view temporary in #4 intArray::Abstract_Operator(): AFTER Temporary is built!");
               if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
                  {
                    printf ("(Abstract_Operator()) - Build the parallel array as a return value (BEFORE setBase()) - Temporary: global range (%d,%d,%d) local range (%d,%d,%d) \n",
                         Temporary->getGlobalMaskIndex(0).getBase(),
                         Temporary->getGlobalMaskIndex(0).getBound(),
                         Temporary->getGlobalMaskIndex(0).getStride(),
                         Temporary->getLocalMaskIndex(0).getBase(),
                         Temporary->getLocalMaskIndex(0).getBound(),
                         Temporary->getLocalMaskIndex(0).getStride());
        }
#endif

            // The base now needs to be reset since temporaries always have APP_Global_Array_Base as a base
	    // ... only reset if no indirect addressing ...
	       if (!Temporary->Array_Descriptor.Array_Domain.Uses_Indirect_Addressing)
                    Temporary->setBase(APP_Global_Array_Base);

#if 0
               Temporary->view ("view temporary in #4 intArray::Abstract_Operator(Array_Conformability_Info_Type,intArray,intSerialArray)");
            // APP_ABORT();
#endif

#if COMPILE_DEBUG_STATEMENTS
               if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
                  {
                    printf ("(Abstract_Operator()) - Build the parallel array as a return value (AFTER setBase()) - Temporary: global range (%d,%d,%d) local range (%d,%d,%d) \n",
                         Temporary->getGlobalMaskIndex(0).getBase(),
                         Temporary->getGlobalMaskIndex(0).getBound(),
                         Temporary->getGlobalMaskIndex(0).getStride(),
                         Temporary->getLocalMaskIndex(0).getBase(),
                         Temporary->getLocalMaskIndex(0).getBound(),
                         Temporary->getLocalMaskIndex(0).getStride());
        }
#endif

               Temporary->setTemporary(TRUE);
 
	    // ... might not need this now ...

            /*
	    Array_Domain_Type::fixupLocalBase
	       (Descriptor->Array_Domain,
	        Data->Array_Descriptor.Array_Domain,
		Lhs_ParallelArray.Array_Descriptor.Array_Domain,
		Lhs_ParallelArray.Array_Descriptor.SerialArray->
		Array_Descriptor.Array_Domain);
            */

            // This is now we acumulate the information used to determine message passing
            // when the operator= is executed!
               APP_ASSERT (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info == NULL);
               Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info = Array_Set;
            // Bugfix (11/27/2000)
               Array_Set->incrementReferenceCount();

            // ... temporary is consistent with Lhs so don't reverse offsets ...
               bool reverse_offset = FALSE;

               Array_Domain_Type:: Fix_Array_Conformability ( 
		    *Array_Set, Temporary->Array_Descriptor.Array_Domain , 
		    Data->Array_Descriptor.Array_Domain, reverse_offset);

            // Bugfix (9/14/95) This should fix the problem related to the Rhs being a temporary
            // and thus the Rhs serial array being reused in the A++ class libaray such that it
            // is referenced in the Temporary which the function returns and the Rhs.
            // The problem shows up when B is a null array and we have B = B + B * B;
            // I would guess that this is because the Null arrays objects in this case
            // are multibily referenced rather than redundently built.
            // But if so --- is this the best place to increment the reference count!
               APP_ASSERT ( Data != NULL );
               if ( (Rhs_ParallelArray.isTemporary() == TRUE) && (Data->isNullArray() == TRUE) )
                  {
#if COMPILE_DEBUG_STATEMENTS
                    if (APP_DEBUG > 1)
                         printf ("Special case of Rhs temporary and reuse of Rhs SerialArray (B = B + B * B) reference count incremented in #4 intArray::Abstract_Operator \n");
#endif
                 // printf ("Special Case Exiting ... \n");
                 // APP_ABORT();
                    Data->incrementReferenceCount();
                  }

            // Temporary->view("Temporary at MIPOINT of intArray::Abstract_Operator(Array_Conformability_Info_Type,intArray,intArray,intSerialArray)");
            // Temporary->Test_Consistency ("Test TEMPORARY at BASE of #4 intArray::Abstract_Operator(Array_Conformability_Info_Type,intArray,intArray,intSerialArray)");

            // printf ("Exiting in #4 intArray::Abstract_Operator ... \n");
            // APP_ABORT();

               Result_Is_Lhs_Or_Rhs_Or_New_Memory = Newly_Allocated_Memory;
             }
        }

     APP_ASSERT(Temporary != NULL);
     APP_ASSERT(Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info != NULL);
     APP_ASSERT(Temporary->Array_Descriptor.SerialArray != NULL);

  // printf ("In #4 intArray::Abstract_Operator: Temporary->Array_Descriptor.SerialArray->Array_ID = %d \n",
  //      Temporary->Array_Descriptor.SerialArray->Array_ID());

#if COMPILE_DEBUG_STATEMENTS
     APP_ASSERT(Temporary->Array_Descriptor.Array_Domain.Is_A_Temporary == TRUE);
  // Temporary->Test_Consistency ("Test Temporary after construction in #4 intArray::Abstract_Operator");

     if (APP_DEBUG > 1)
          printf ("Now delete the temporaries handed in as input if they were not reused! \n");
#endif

  // If the Lhs was a temporary then we used it so we don't have to call
  // the "Delete_If_Temporary ( Lhs_ParallelArray );" function for the LHS
  // We want to delete the RHS if it is a temporary unless we used it
     if (Result_Is_Lhs_Or_Rhs_Or_New_Memory != Memory_From_Rhs)
          Delete_If_Temporary ( Rhs_ParallelArray );

  // printf ("(AFTER RHS TEST) Temporary->Array_Descriptor.SerialArray->Array_Descriptor = %p \n",Temporary->Array_Descriptor.SerialArray->Array_Descriptor);
  // printf ("Set Temporary->Array_Descriptor.Array_Set = Array_Set in intArray::Abstract_Binary_Operator \n");

#if COMPILE_DEBUG_STATEMENTS
     APP_ASSERT(Temporary->Array_Descriptor.Array_Domain.Is_A_Temporary == TRUE);
     Temporary->Test_Consistency ("Test TEMPORARY at BASE of #4 intArray::Abstract_Operator(Array_Conformability_Info_Type,intArray,intArray,intSerialArray)");

     if (APP_DEBUG > 1)
        {
          if (APP_DEBUG > 2)
             {
               Temporary->view ("view TEMPORARY at BASE of #4 intArray::Abstract_Operator(Array_Conformability_Info_Type,intArray,intArray,intSerialArray)");
            // Lhs_Serial_PCE_Array->view ("Lhs_Serial_PCE_Array at BASE of #4 intArray::Abstract_Operator(Array_Conformability_Info_Type,intArray,intArray,intSerialArray)");
            // Rhs_Serial_PCE_Array->view ("Rhs_Serial_PCE_Array at BASE of #4 intArray::Abstract_Operator(Array_Conformability_Info_Type,intArray,intArray,intSerialArray)");
             }
          printf ("************************************************************************************************* \n");
          printf ("************************************************************************************************* \n");
          printf ("Leaving #4 intArray::Abstract_Operator (Array_Conformability_Info_Type,intArray,intArray,intSerialArray) \n");
          printf ("************************************************************************************************* \n");
          printf ("************************************************************************************************* \n");
        }

     if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
        {
          printf ("Build the parallel array as a return value (#4 intArray::Abstract_Operator) - Temporary: global range (%d,%d,%d) local range (%d,%d,%d) \n",
               Temporary->getGlobalMaskIndex(0).getBase(),
               Temporary->getGlobalMaskIndex(0).getBound(),
               Temporary->getGlobalMaskIndex(0).getStride(),
               Temporary->getLocalMaskIndex(0).getBase(),
               Temporary->getLocalMaskIndex(0).getBound(),
               Temporary->getLocalMaskIndex(0).getStride());
        }

  // Commented out (12/14/2000)
  // Communication_Manager::Sync();
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
        {
          APP_ASSERT (Temporary != NULL);
          APP_ASSERT (Temporary->Array_Descriptor.SerialArray != NULL);
          printf ("In #4 intArray::Abstract_Operator(): Temporary->Array_Descriptor.Array_ID() = %d Temporary->SerialArray->Array_ID() = %d \n",
               Temporary->Array_ID(),Temporary->Array_Descriptor.SerialArray->Array_ID());
          printf ("************************************************************************************************* \n");
          printf ("Leaving #4 intArray::Abstract_Operator(Array_Conformability_Info_Type,intArray,intArray,intSerialArray) (Temporary SerialArray pointer = %p) \n",Temporary->Array_Descriptor.SerialArray);
          printf ("************************************************************************************************* \n");
        }
#endif

     return *Temporary;
   }

#if !defined(INTARRAY)
// replace operators for array to array
intArray &
intArray::Abstract_Operator ( 
     Array_Conformability_Info_Type *Array_Set, 
     const intArray & Lhs_ParallelArray, const intArray & Rhs_ParallelArray, 
     intSerialArray* Lhs_Serial_PCE_Array, intSerialArray* Rhs_Serial_PCE_Array, 
     intSerialArray & X_SerialArray )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of #5 intArray::Abstract_Operator(Array_Conformability_Info_Type,intArray,intArray,intSerialArray) \n");

  // This is the only test we can do on the input!
     Lhs_ParallelArray.Test_Consistency ("Test LHS in #5 intArray::Abstract_Operator(Array_Conformability_Info_Type,intArray,intArray,intSerialArray)");
     Rhs_ParallelArray.Test_Consistency ("Test RHS in #5 intArray::Abstract_Operator(Array_Conformability_Info_Type,intArray,intArray,intSerialArray)");
     X_SerialArray.Test_Consistency ("Test X_SerialArray in #5 intArray::Abstract_Operator(Array_Conformability_Info_Type,intArray,intArray,intSerialArray)");
#endif

     if (Index::Index_Bounds_Checking)
          Lhs_ParallelArray.Test_Conformability (Rhs_ParallelArray);

  // error checking
     APP_ASSERT(Array_Set != NULL);

  // The Partitioning array is the array object (usually the Lhs operand) which the
  // resulting temporary will be partitioned similar to (if it is required to build the temporary).
  // If a scalar was used on the Lhs then we have to use the Rhs as a Partitioning_intArray
  // intArray* Partitioning_Array = &((intArray &) Lhs_ParallelArray);
  // APP_ASSERT (Partitioning_Array != NULL);

  // The size of the temporary must be the same as that of the Lhs (Lhs based owners compute rule)
     intSerialArray *Data = &((intSerialArray &) X_SerialArray);
     APP_ASSERT(Data != NULL);

     intArray *Temporary = NULL;

  // printf ("ERROR: In Abstract_Operator we are doing redundent copying (exit until this is fixed) \n");
  // APP_ABORT();
  // This is more efficient than the previous version which build a descriptor and
  // then forced 2 copies of the descriptor before ending up with a temporary to use.
     bool AvoidBuildingIndirectAddressingView = TRUE;
  // Data->view("In abstract_op #2: Data");
  // X_ParallelArray.Array_Descriptor.Array_Domain.display("In abstract_op #2: X_ParallelArray: Array_Domain");
     Temporary = new intArray ( Data ,
                               &(Lhs_ParallelArray.Array_Descriptor.Array_Domain),
                               AvoidBuildingIndirectAddressingView );
 
  // The base now needs to be reset since temporaries always have APP_Global_Array_Base as a base
     Temporary->setBase(APP_Global_Array_Base);
 
  // Temporary->view ("view temporary in #5 intArray::Abstract_Operator(Array_Conformability_Info_Type,intArray,intSerialArray)");
  // APP_ABORT();
 
     Temporary->setTemporary(TRUE);
 
  // This is now we acumulate the information used to determine message passing
  // when the operator= is executed!
  // APP_ASSERT (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info == NULL);
  // Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info = Array_Set;

#if USE_TEMPORARY_DELETE_FUNCTIONS
     Delete_If_Temporary ( Lhs_ParallelArray );
     Delete_If_Temporary ( Rhs_ParallelArray );
#endif

  // This is how we acumulate the information used to determine message passing 
  // when the operator= is executed!
     if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info != NULL)
        {
          Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->decrementReferenceCount();
          if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount() < 
              Array_Conformability_Info_Type::getReferenceCountBase())
          delete Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info;
        }

     Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info = Array_Set;
  // Bugfix (11/27/2000)
     Array_Set->incrementReferenceCount();

#if COMPILE_DEBUG_STATEMENTS
     APP_ASSERT(Temporary->Array_Descriptor.Array_Domain.Is_A_Temporary == TRUE);
     Temporary->Test_Consistency ("Test (on return) in #5 intArray::Abstract_Operator(Array_Conformability_Info_Type,intArray,intArray,intSerialArray)");
#endif

     return *Temporary;
   }
#endif

// ******************************************************************************
// These Abstract_Modification_Operator operators are special because they don't
// return a temporary.  This function is used for the array to scalar relational operators 
// ******************************************************************************
intArray &
intArray::Abstract_Modification_Operator (
     Array_Conformability_Info_Type *Array_Set, 
     const intArray & X_ParallelArray, 
     intSerialArray* X_Serial_PCE_Array, 
     const intSerialArray & X_SerialArray )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of #6 intArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,intArray,intSerialArray) \n");

  // This is the only test we can do on the input!
     X_ParallelArray.Test_Consistency ("Test A in #6 intArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,intArray,intSerialArray)");
     X_SerialArray.Test_Consistency ("Test B in #6 intArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,intArray,intSerialArray)");
#endif

#if USE_TEMPORARY_DELETE_FUNCTIONS
  // Delete_If_Temporary ( X_ParallelArray );
#endif

     intArray* Temporary = &((intArray &) X_ParallelArray);

  // Modify_Reference_Counts_And_Manage_Temporaries ( Temporary , Data );

  // This is now we acumulate the information used to determine message passing 
  // when the operator= is executed!
     if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info != NULL)
        {
          Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->decrementReferenceCount();
          if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount() < 
              Array_Conformability_Info_Type::getReferenceCountBase())
          delete Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info;
        }
     Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info = NULL;

#if 0
  // Bugfix (11/27/2000)
     Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info = Array_Set;
  // Bugfix (11/27/2000)
     Array_Set->incrementReferenceCount();
#endif

#if 0
  // Need to delete the Array_Set to avoid a memory leak
     Array_Set->decrementReferenceCount();
     if (Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
          delete Array_Set;
     Array_Set = NULL;
#endif

#if COMPILE_DEBUG_STATEMENTS
  // APP_ASSERT(Temporary->Array_Descriptor.Array_Domain.Is_A_Temporary == TRUE);
     Temporary->Test_Consistency ("Test (on return) in #6 intArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,intArray,intSerialArray)");
#endif

     return *Temporary;
   }

// ******************************************************************************
// These Abstract_Modification_Operator operators are special because they don't
// return a temporary.  This function is used for the array to array replace operators 
// ******************************************************************************
intArray &
intArray::Abstract_Modification_Operator (
     Array_Conformability_Info_Type *Array_Set, 
     const intArray & This_ParallelArray, const intArray & Lhs_ParallelArray, 
     intSerialArray* This_Serial_PCE_Array, intSerialArray* Lhs_Serial_PCE_Array, 
     const intSerialArray & This_SerialArray )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of #7 intArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,intArray,intArray,intSerialArray) \n");

  // This is the only test we can do on the input!
     This_ParallelArray.Test_Consistency ("Test THIS in #7 intArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,intArray,intArray,intSerialArray)");
     Lhs_ParallelArray.Test_Consistency ("Test LHS in #7 intArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,intArray,intArray,intSerialArray)");
     This_SerialArray.Test_Consistency ("Test This_SerialArray in #7 intArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,intArray,intArray,intSerialArray)");
#endif

     if (Index::Index_Bounds_Checking)
          This_ParallelArray.Test_Conformability (Lhs_ParallelArray);

#if USE_TEMPORARY_DELETE_FUNCTIONS
  // Delete_If_Temporary ( This_ParallelArray );
     Delete_If_Temporary ( Lhs_ParallelArray );
#endif

     intArray* Temporary = &((intArray &) This_ParallelArray);

  // Modify_Reference_Counts_And_Manage_Temporaries ( Temporary , Data );

  // This is how we acumulate the information used to determine message passing 
  // when the operator= is executed!
     if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info != NULL)
        {
       // printf ("Commented out Array_Conformability_Info_Type::delete \n");
          Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->decrementReferenceCount();
          if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount() < 
              Array_Conformability_Info_Type::getReferenceCountBase())
               delete Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info;
        }
     Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info = NULL;

#if 0
  // Bugfix (11/27/2000)
     Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info = Array_Set;
  // Bugfix (11/27/2000)
     Array_Set->incrementReferenceCount();
#endif

#if 0
  // Need to delete the Array_Set to avoid a memory leak
     Array_Set->decrementReferenceCount();
     if (Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
          delete Array_Set;
     Array_Set = NULL;
#endif

#if COMPILE_DEBUG_STATEMENTS
  // APP_ASSERT(Temporary->Array_Descriptor.Array_Domain.Is_A_Temporary == TRUE);
     Temporary->Test_Consistency ("Test (on return) in #7 intArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,intArray,intSerialArray)");
#endif

     return *Temporary;
   }

#if !defined(INTARRAY)
// ******************************************************************************
// These Abstract_Modification_Operator operators are special because they don't
// return a temporary.  This function is used for the array to array replace operators 
// ******************************************************************************
intArray &
intArray::Abstract_Modification_Operator (
     Array_Conformability_Info_Type *Array_Set, 
     const intArray & This_ParallelArray, const intArray & Lhs_ParallelArray, 
     intSerialArray* This_Serial_PCE_Array, intSerialArray* Lhs_Serial_PCE_Array, 
     const intSerialArray & This_SerialArray )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of #8 intArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,intArray,intArray,intSerialArray) \n");

  // This is the only test we can do on the input!
     This_ParallelArray.Test_Consistency ("Test THIS in #8 intArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,intArray,intArray,intSerialArray)");
     Lhs_ParallelArray.Test_Consistency ("Test LHS in #8 intArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,intArray,intArray,intSerialArray)");
     This_SerialArray.Test_Consistency ("Test This_SerialArray in #8 intArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,intArray,intArray,intSerialArray)");
#endif

     if (Index::Index_Bounds_Checking)
          This_ParallelArray.Test_Conformability (Lhs_ParallelArray);

     APP_ASSERT(Array_Set != NULL);

#if USE_TEMPORARY_DELETE_FUNCTIONS
  // Delete_If_Temporary ( This_ParallelArray );
     Delete_If_Temporary ( Lhs_ParallelArray );
#endif

     intArray* Temporary = &((intArray &) This_ParallelArray);

  // Modify_Reference_Counts_And_Manage_Temporaries ( Temporary , Data );

  // This is now we acumulate the information used to determine message passing 
  // when the operator= is executed!
     if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info != NULL)
        {
       // printf ("Commented out Array_Conformability_Info_Type::delete \n");
          Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->decrementReferenceCount();
          if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount() < 
              Array_Conformability_Info_Type::getReferenceCountBase())
               delete Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info;
        }
     Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info = NULL;

#if 0
  // Bugfix (11/27/2000)
     Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info = Array_Set;
  // Bugfix (11/27/2000)
     Array_Set->incrementReferenceCount();
#endif

#if 0
  // Need to delete the Array_Set to avoid a memory leak
     Array_Set->decrementReferenceCount();
     if (Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
          delete Array_Set;
     Array_Set = NULL;
#endif

#if COMPILE_DEBUG_STATEMENTS
  // APP_ASSERT(Temporary->Array_Descriptor.Array_Domain.Is_A_Temporary == TRUE);
     Temporary->Test_Consistency ("Test (on return) in #8 intArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,intArray,intArray,intSerialArray)");
#endif

     return *Temporary;
   }

// ******************************************************************************
// These Abstract_Modification_Operator operators are special because they don't
// return a temporary.  This function is used for the array to scalar relational operators 
// ******************************************************************************
intArray &
intArray::Abstract_Modification_Operator (
     Array_Conformability_Info_Type *Array_Set, 
     const intArray & This_ParallelArray, 
     const intArray & Lhs_ParallelArray, 
     const intArray & Rhs_ParallelArray, 
     intSerialArray* This_Serial_PCE_Array, 
     intSerialArray* Lhs_Serial_PCE_Array, 
     intSerialArray* Rhs_Serial_PCE_Array, 
     const intSerialArray & This_SerialArray )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of #9 intArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,intArray,intArray,intArray,intSerialArray) \n");

  // This is the only test we can do on the input!
     This_ParallelArray.Test_Consistency ("Test in #9 intArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,intArray,intArray,intArray,intSerialArray)");
     Lhs_ParallelArray.Test_Consistency ("Test in #9 intArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,intArray,intArray,intArray,intSerialArray)");
     Rhs_ParallelArray.Test_Consistency ("Test in #9 intArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,intArray,intArray,intArray,intSerialArray)");
     This_SerialArray.Test_Consistency ("Test in #9 intArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,intArray,intArray,intArray,intSerialArray)");
#endif

     if (Index::Index_Bounds_Checking)
        {
          This_ParallelArray.Test_Conformability (Lhs_ParallelArray);
          This_ParallelArray.Test_Conformability (Rhs_ParallelArray);
#if EXTRA_ERROR_CHECKING
       // Redundent test
          Lhs_ParallelArray.Test_Conformability (Rhs_ParallelArray);
#endif
        }

#if USE_TEMPORARY_DELETE_FUNCTIONS
     Delete_If_Temporary ( Lhs_ParallelArray );
     Delete_If_Temporary ( Rhs_ParallelArray );
#endif

     intArray* Temporary = &((intArray &) This_ParallelArray);

  // Modify_Reference_Counts_And_Manage_Temporaries ( Temporary , Data );

  // This is now we acumulate the information used to determine message passing 
  // when the operator= is executed!
     if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info != NULL)
        {
          Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->decrementReferenceCount();
          if (Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info->getReferenceCount() < 
              Array_Conformability_Info_Type::getReferenceCountBase())
               delete Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info;
        }
     Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info = NULL;

#if 0
  // Bugfix (11/27/2000)
     Temporary->Array_Descriptor.Array_Domain.Array_Conformability_Info = Array_Set;
  // Bugfix (11/27/2000)
     Array_Set->incrementReferenceCount();
#endif

#if 0
  // Need to delete the Array_Set to avoid a memory leak
     Array_Set->decrementReferenceCount();
     if (Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
          delete Array_Set;
     Array_Set = NULL;
#endif

#if COMPILE_DEBUG_STATEMENTS
  // APP_ASSERT(Temporary->Array_Descriptor.Array_Domain.Is_A_Temporary == TRUE);
     Temporary->Test_Consistency ("Test (on return) in #9 intArray::Abstract_Modification_Operator(Array_Conformability_Info_Type,intArray,intArray,intArray,intSerialArray)");
#endif

     return *Temporary;
   }

#endif

#undef INTARRAY





















//----------------------------------------------------------------------


//---------------------------------------------------------------

//---------------------------------------------------------------


//---------------------------------------------------------------
