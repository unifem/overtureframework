

#define COMPILE_SERIAL_APP

#include "A++.h"

extern "C"
   {
/* machine.h is found in MDI/machine.h through a link in A++/inc lude and P++/inc lude */
#include "machine.h"

   }

void Delete_SerialArray ( const intArray    & parallelArray, intSerialArray*    serialArray, const Array_Conformability_Info_Type *Temporary_Array_Set );
void Delete_SerialArray ( const floatArray  & parallelArray, floatSerialArray*  serialArray, const Array_Conformability_Info_Type *Temporary_Array_Set );
void Delete_SerialArray ( const doubleArray & parallelArray, doubleSerialArray* serialArray, const Array_Conformability_Info_Type *Temporary_Array_Set );











#define INTARRAY
intSerialArray &
operator+ ( const intSerialArray & Lhs , const intSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
        {
          printf ("\n\n\n### Inside of operator+ (intSerialArray,intSerialArray) for intSerialArray class: (id=%d) = (id=%d) \n",
               Lhs.Array_ID(),Rhs.Array_ID());
        }

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator+");
     Rhs.Test_Consistency ("Test Rhs in operator+");
#endif

  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Lhs.Test_Conformability (Rhs);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          printf ("intSerialArray: Lhs.isTemporary() = %s \n",(Lhs.isTemporary()) ? "TRUE" : "FALSE");
          Lhs.displayReferenceCounts("Lhs in intSerialArray & operator+(intSerialArray,intSerialArray)");
          printf ("intSerialArray: Rhs.isTemporary() = %s \n",(Rhs.isTemporary()) ? "TRUE" : "FALSE");
          Rhs.displayReferenceCounts("Rhs in intSerialArray & operator+(intSerialArray,intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray* Lhs_SerialArray = NULL;
     intSerialArray* Rhs_SerialArray = NULL;

     intSerialArray* Mask_SerialArray = NULL;
     intSerialArray* Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	      (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
               Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
          if ( (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
               (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) )
             {
               printf ("Sorry, not implemented: can't mix indirect addressing using where statements and two array (binary) operators!\n");
               APP_ABORT();
             }
            else
             {
               Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement
                  (Lhs, Lhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray, Rhs, Rhs_SerialArray );
             }
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          printf ("intSerialArray: Lhs_SerialArray->isTemporary() = %s \n",(Lhs_SerialArray->isTemporary()) ? "TRUE" : "FALSE");
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in intSerialArray & operator+(intSerialArray,intSerialArray)");
          printf ("intSerialArray: Rhs_SerialArray->isTemporary() = %s \n",(Rhs_SerialArray->isTemporary()) ? "TRUE" : "FALSE");
          Rhs_SerialArray->displayReferenceCounts("Rhs_SerialArray in intSerialArray & operator+(intSerialArray,intSerialArray)");
        }
#endif

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Lhs_SerialArray != NULL);
     APP_ASSERT(Rhs_SerialArray != NULL);

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();
     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );
     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

  // return intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, *Lhs_SerialArray + *Rhs_SerialArray );
#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = Lhs;
#else
     intArray & Return_Value = intArray::Abstract_Operator 
	( Temporary_Array_Set, Lhs, Rhs, Lhs_SerialArray, Rhs_SerialArray, *Lhs_SerialArray + *Rhs_SerialArray );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (lhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

        }
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }

     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & operator+(intSerialArray,intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator ( Lhs , Rhs , 
               MDI_i_Add_Array_Plus_Array,
               MDI_i_Add_Array_Plus_Array_Accumulate_To_Operand , intSerialArray::Plus );
#endif
   }


intSerialArray &
intSerialArray::operator-- ()
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside intSerialArray::operator-- () for intSerialArray class!");

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in intSerialArray::operator-- -- Prefix operator");
#endif

     (*this) -= 1;
     return *this;
   }
 
intSerialArray &
intSerialArray::operator-- ( int x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside intSerialArray::operator%s (int=%d) for intSerialArray class! \n","--",x);

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in intSerialArray::operator--(x=0) -- Postfix operator");
#endif

  // Postfix operator always passes zero as argument (strange but true -- See Stroustrup p594)
     APP_ASSERT( x == 0 );
     (*this) -= 1;
     return *this;
   }


#ifdef INTARRAY
/* There is no >>= operator and so the >> must be handled as a special case -- skip it for now */
intSerialArray &
operator>> ( const intSerialArray & Lhs , const intSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
        {
          printf ("\n\n\n### Inside of operator>> (intSerialArray,intSerialArray) for intSerialArray class: (id=%d) = (id=%d) \n",
               Lhs.Array_ID(),Rhs.Array_ID());
        }

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator>>");
     Rhs.Test_Consistency ("Test Rhs in operator>>");
#endif

  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Lhs.Test_Conformability (Rhs);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          printf ("intSerialArray: Lhs.isTemporary() = %s \n",(Lhs.isTemporary()) ? "TRUE" : "FALSE");
          Lhs.displayReferenceCounts("Lhs in intSerialArray & operator>>(intSerialArray,intSerialArray)");
          printf ("intSerialArray: Rhs.isTemporary() = %s \n",(Rhs.isTemporary()) ? "TRUE" : "FALSE");
          Rhs.displayReferenceCounts("Rhs in intSerialArray & operator>>(intSerialArray,intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray* Lhs_SerialArray = NULL;
     intSerialArray* Rhs_SerialArray = NULL;

     intSerialArray* Mask_SerialArray = NULL;
     intSerialArray* Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	      (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
               Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
          if ( (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
               (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) )
             {
               printf ("Sorry, not implemented: can't mix indirect addressing using where statements and two array (binary) operators!\n");
               APP_ABORT();
             }
            else
             {
               Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement
                  (Lhs, Lhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray, Rhs, Rhs_SerialArray );
             }
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          printf ("intSerialArray: Lhs_SerialArray->isTemporary() = %s \n",(Lhs_SerialArray->isTemporary()) ? "TRUE" : "FALSE");
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in intSerialArray & operator>>(intSerialArray,intSerialArray)");
          printf ("intSerialArray: Rhs_SerialArray->isTemporary() = %s \n",(Rhs_SerialArray->isTemporary()) ? "TRUE" : "FALSE");
          Rhs_SerialArray->displayReferenceCounts("Rhs_SerialArray in intSerialArray & operator>>(intSerialArray,intSerialArray)");
        }
#endif

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Lhs_SerialArray != NULL);
     APP_ASSERT(Rhs_SerialArray != NULL);

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();
     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );
     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

  // return intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, *Lhs_SerialArray >> *Rhs_SerialArray );
#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = Lhs;
#else
     intArray & Return_Value = intArray::Abstract_Operator 
	( Temporary_Array_Set, Lhs, Rhs, Lhs_SerialArray, Rhs_SerialArray, *Lhs_SerialArray >> *Rhs_SerialArray );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (lhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

        }
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }

     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & operator>>(intSerialArray,intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator ( Lhs , Rhs , 
               MDI_i_BIT_RSHIFT_Array_BitwiseRShift_Array,
               MDI_i_BIT_RSHIFT_Array_BitwiseRShift_Array_Accumulate_To_Operand , intSerialArray::BitwiseRShift );
#endif
   }

intSerialArray &
operator>> ( const intSerialArray & Lhs , int x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("\n\n\n### Inside of operator>> (intSerialArray,int) for intSerialArray class: (id=%d) = scalar \n",Lhs.Array_ID());

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator>>");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in intSerialArray & operator>>(intSerialArray,int)");
        }
#endif

#if defined(PPP)
     intSerialArray* Lhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
               Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
               APP_ASSERT(Lhs_SerialArray != NULL);
            // Lhs_SerialArray->displayReferenceCounts("AFTER PCE: *Lhs_SerialArray in intSerialArray & operator>>(intSerialArray,int)");
             }
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
          if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
               Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
                    (Lhs, Lhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
             }
            else
             {
               Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
                    (Lhs, Lhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
             }
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          printf ("intSerialArray: Lhs_SerialArray->isTemporary() = %s \n",(Lhs_SerialArray->isTemporary()) ? "TRUE" : "FALSE");
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in intSerialArray & operator>>(intSerialArray,int)");
        }
#endif

  // (11/27/2000) Added error checking (will not work with indirect addessing later!!!)
     APP_ASSERT(Temporary_Array_Set != NULL);

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Lhs_SerialArray != NULL);

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );

#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = Lhs;
#else
     intArray & Return_Value = intArray::Abstract_Operator(Temporary_Array_Set,Lhs,Lhs_SerialArray,*Lhs_SerialArray >> x);
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (lhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in in intSerialArray & operator>>(intSerialArray,int)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator ( Lhs , x , 
               MDI_i_BIT_RSHIFT_Array_BitwiseRShift_Scalar,
               MDI_i_BIT_RSHIFT_Array_BitwiseRShift_Scalar_Accumulate_To_Operand , intSerialArray::Scalar_BitwiseRShift );
#endif
   }

intSerialArray &
operator>> ( int x , const intSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator>> (int,intSerialArray) for intSerialArray class!");

  // This is the only test we can do on the input!
     Rhs.Test_Consistency ("Test Rhs in operator>>");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs.displayReferenceCounts("Rhs in intSerialArray & operator>>(int,intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray* Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      ( Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = 
	      intArray::Parallel_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }
     if (Temporary_Array_Set == NULL) Temporary_Array_Set =
	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Rhs_SerialArray != NULL);

     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = Rhs;
#else
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Rhs, Rhs_SerialArray, x >> *Rhs_SerialArray );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & operator>>(int,intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator ( Rhs , x , 
               MDI_i_BIT_RSHIFT_Scalar_BitwiseRShift_Array,
               MDI_i_BIT_RSHIFT_Scalar_BitwiseRShift_Array_Accumulate_To_Operand , intSerialArray::Scalar_BitwiseRShift );
#endif
   }

#endif

#ifdef INTARRAY
intSerialArray &
intSerialArray::operator&= ( const intSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of intSerialArray::operator&= (intSerialArray) for intSerialArray class!");

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in intSerialArray::operator&=");
     Rhs.Test_Consistency ("Test Rhs in intSerialArray::operator&=");
#endif

  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Test_Conformability (Rhs);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS in intSerialArray & intSerialArray::operator&=(intSerialArray)");
          Rhs.displayReferenceCounts("Rhs in intSerialArray & intSerialArray::operator&=(intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *This_SerialArray = NULL;
     intSerialArray *Rhs_SerialArray  = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if ((Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	    (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      ( *this , This_SerialArray, Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      ( *this , This_SerialArray, Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if ((Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	    (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
        {
	   puts ("ERROR: can't mix indirect addressing with 2 arrays and where.");
           APP_ABORT();
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (*this, This_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray,
	       Rhs, Rhs_SerialArray );
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set =	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(This_SerialArray != NULL);
     APP_ASSERT(Rhs_SerialArray  != NULL);

     bool thisIsTemporary = This_SerialArray->isTemporary();
     bool rhsIsTemporary  = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (thisIsTemporary == TRUE) || (thisIsTemporary == FALSE) );
     APP_ASSERT ( (rhsIsTemporary  == TRUE) || (rhsIsTemporary  == FALSE) );

#if !defined(MEMORY_LEAK_TEST)
     intArray::Abstract_Modification_Operator ( Temporary_Array_Set, *this, Rhs, This_SerialArray, Rhs_SerialArray, *This_SerialArray &= *Rhs_SerialArray );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (thisIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): This_SerialArray->getReferenceCount() = %d \n",
       //      This_SerialArray->getReferenceCount());

       // Must delete the This_SerialArray if it was taken directly from the This array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (This_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          This_SerialArray->decrementReferenceCount();
          if (This_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete This_SerialArray;
             }
          This_SerialArray = NULL;

        }
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }

     // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#else
     intSerialArray::Abstract_Operator_Operation_Equals ( *this , Rhs ,
               MDI_i_BIT_AND_Array_BitwiseAND_Array_Accumulate_To_Operand , intSerialArray::BitwiseAND_Equals );
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS (return value) in intSerialArray & intSerialArray::operator&=(intSerialArray)");
        }
#endif

     return *this;
   }

#endif

#ifdef INTARRAY
intSerialArray &
intSerialArray::operator|= ( const intSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of intSerialArray::operator|= (intSerialArray) for intSerialArray class!");

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in intSerialArray::operator|=");
     Rhs.Test_Consistency ("Test Rhs in intSerialArray::operator|=");
#endif

  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Test_Conformability (Rhs);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS in intSerialArray & intSerialArray::operator|=(intSerialArray)");
          Rhs.displayReferenceCounts("Rhs in intSerialArray & intSerialArray::operator|=(intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *This_SerialArray = NULL;
     intSerialArray *Rhs_SerialArray  = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if ((Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	    (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      ( *this , This_SerialArray, Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      ( *this , This_SerialArray, Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if ((Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	    (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
        {
	   puts ("ERROR: can't mix indirect addressing with 2 arrays and where.");
           APP_ABORT();
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (*this, This_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray,
	       Rhs, Rhs_SerialArray );
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set =	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(This_SerialArray != NULL);
     APP_ASSERT(Rhs_SerialArray  != NULL);

     bool thisIsTemporary = This_SerialArray->isTemporary();
     bool rhsIsTemporary  = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (thisIsTemporary == TRUE) || (thisIsTemporary == FALSE) );
     APP_ASSERT ( (rhsIsTemporary  == TRUE) || (rhsIsTemporary  == FALSE) );

#if !defined(MEMORY_LEAK_TEST)
     intArray::Abstract_Modification_Operator ( Temporary_Array_Set, *this, Rhs, This_SerialArray, Rhs_SerialArray, *This_SerialArray |= *Rhs_SerialArray );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (thisIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): This_SerialArray->getReferenceCount() = %d \n",
       //      This_SerialArray->getReferenceCount());

       // Must delete the This_SerialArray if it was taken directly from the This array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (This_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          This_SerialArray->decrementReferenceCount();
          if (This_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete This_SerialArray;
             }
          This_SerialArray = NULL;

        }
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }

     // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#else
     intSerialArray::Abstract_Operator_Operation_Equals ( *this , Rhs ,
               MDI_i_BIT_OR_Array_BitwiseOR_Array_Accumulate_To_Operand , intSerialArray::BitwiseOR_Equals );
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS (return value) in intSerialArray & intSerialArray::operator|=(intSerialArray)");
        }
#endif

     return *this;
   }

#endif

#ifdef INTARRAY
intSerialArray &
intSerialArray::operator^= ( const intSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of intSerialArray::operator^= (intSerialArray) for intSerialArray class!");

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in intSerialArray::operator^=");
     Rhs.Test_Consistency ("Test Rhs in intSerialArray::operator^=");
#endif

  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Test_Conformability (Rhs);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS in intSerialArray & intSerialArray::operator^=(intSerialArray)");
          Rhs.displayReferenceCounts("Rhs in intSerialArray & intSerialArray::operator^=(intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *This_SerialArray = NULL;
     intSerialArray *Rhs_SerialArray  = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if ((Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	    (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      ( *this , This_SerialArray, Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      ( *this , This_SerialArray, Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if ((Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	    (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
        {
	   puts ("ERROR: can't mix indirect addressing with 2 arrays and where.");
           APP_ABORT();
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (*this, This_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray,
	       Rhs, Rhs_SerialArray );
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set =	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(This_SerialArray != NULL);
     APP_ASSERT(Rhs_SerialArray  != NULL);

     bool thisIsTemporary = This_SerialArray->isTemporary();
     bool rhsIsTemporary  = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (thisIsTemporary == TRUE) || (thisIsTemporary == FALSE) );
     APP_ASSERT ( (rhsIsTemporary  == TRUE) || (rhsIsTemporary  == FALSE) );

#if !defined(MEMORY_LEAK_TEST)
     intArray::Abstract_Modification_Operator ( Temporary_Array_Set, *this, Rhs, This_SerialArray, Rhs_SerialArray, *This_SerialArray ^= *Rhs_SerialArray );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (thisIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): This_SerialArray->getReferenceCount() = %d \n",
       //      This_SerialArray->getReferenceCount());

       // Must delete the This_SerialArray if it was taken directly from the This array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (This_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          This_SerialArray->decrementReferenceCount();
          if (This_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete This_SerialArray;
             }
          This_SerialArray = NULL;

        }
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }

     // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#else
     intSerialArray::Abstract_Operator_Operation_Equals ( *this , Rhs ,
               MDI_i_BIT_XOR_Array_BitwiseXOR_Array_Accumulate_To_Operand , intSerialArray::BitwiseXOR_Equals );
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS (return value) in intSerialArray & intSerialArray::operator^=(intSerialArray)");
        }
#endif

     return *this;
   }

#endif

intSerialArray &
intSerialArray::convertTo_intArray () const
   {
// Used to implement the conversion functions between int float and double arrays

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of convertTo_intArray for intSerialArray class! \n");

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in intSerialArray::operatorconvertTo_intArray");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS in intSerialArray & intSerialArray::convertTo_intArray()");
        }
#endif

#if defined(PPP)
     intSerialArray *This_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
	       Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement ( *this, This_SerialArray);
             }
            else
             {
               Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement ( *this, This_SerialArray);
             }
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
          if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
               Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement (
                    *this, This_SerialArray, 
                    *Where_Statement_Support::Where_Statement_Mask,
                    Mask_SerialArray);
             }
            else
             {
               Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement 
	          ( *this, This_SerialArray, 
                    *Where_Statement_Support::Where_Statement_Mask,
                    Mask_SerialArray);
             }
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(This_SerialArray != NULL);

     bool thisIsTemporary = This_SerialArray->isTemporary();

     APP_ASSERT ( (thisIsTemporary == TRUE) || (thisIsTemporary == FALSE) );

#if defined(MEMORY_LEAK_TEST)
     printf ("Can't do MEMORY_LEAK_TEST in intSerialArray & intSerialArray::convertTo_intArray () \n");
#endif
     intArray & Return_Value = intArray::Abstract_int_Conversion_Operator ( Temporary_Array_Set, *this, This_SerialArray, This_SerialArray->convertTo_intArray() );

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
  // Since the Lhs is a different type than the Return Value it could not be reused!
     // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): This_SerialArray->getReferenceCount() = %d \n",
       //      This_SerialArray->getReferenceCount());

       // Must delete the This_SerialArray if it was taken directly from the This array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (This_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          This_SerialArray->decrementReferenceCount();
          if (This_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete This_SerialArray;
             }
          This_SerialArray = NULL;


  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & intSerialArray::convertTo_intArray()");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_int_Conversion_Operator ( *this ,
                   MDI_i_Array_convertTo_intArray_Array_Accumulate_To_Operand ,
                   intSerialArray::convertTo_intArrayFunction );
#endif
   }

floatSerialArray &
intSerialArray::convertTo_floatArray () const
   {
// Used to implement the conversion functions between int float and double arrays

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of convertTo_floatArray for intSerialArray class! \n");

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in intSerialArray::operatorconvertTo_floatArray");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS in floatSerialArray & intSerialArray::convertTo_floatArray()");
        }
#endif

#if defined(PPP)
     intSerialArray *This_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
	       Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement ( *this, This_SerialArray);
             }
            else
             {
               Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement ( *this, This_SerialArray);
             }
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
          if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
               Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement (
                    *this, This_SerialArray, 
                    *Where_Statement_Support::Where_Statement_Mask,
                    Mask_SerialArray);
             }
            else
             {
               Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement 
	          ( *this, This_SerialArray, 
                    *Where_Statement_Support::Where_Statement_Mask,
                    Mask_SerialArray);
             }
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(This_SerialArray != NULL);

     bool thisIsTemporary = This_SerialArray->isTemporary();

     APP_ASSERT ( (thisIsTemporary == TRUE) || (thisIsTemporary == FALSE) );

#if defined(MEMORY_LEAK_TEST)
     printf ("Can't do MEMORY_LEAK_TEST in intSerialArray & intSerialArray::convertTo_floatArray () \n");
#endif
     floatArray & Return_Value = intArray::Abstract_float_Conversion_Operator ( Temporary_Array_Set, *this, This_SerialArray, This_SerialArray->convertTo_floatArray() );

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
  // Since the Lhs is a different type than the Return Value it could not be reused!
     // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): This_SerialArray->getReferenceCount() = %d \n",
       //      This_SerialArray->getReferenceCount());

       // Must delete the This_SerialArray if it was taken directly from the This array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (This_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          This_SerialArray->decrementReferenceCount();
          if (This_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete This_SerialArray;
             }
          This_SerialArray = NULL;


  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in floatSerialArray & intSerialArray::convertTo_floatArray()");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_float_Conversion_Operator ( *this ,
                   MDI_i_Array_convertTo_floatArray_Array_Accumulate_To_Operand ,
                   intSerialArray::convertTo_floatArrayFunction );
#endif
   }

doubleSerialArray &
intSerialArray::convertTo_doubleArray () const
   {
// Used to implement the conversion functions between int float and double arrays

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of convertTo_doubleArray for intSerialArray class! \n");

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in intSerialArray::operatorconvertTo_doubleArray");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS in doubleSerialArray & intSerialArray::convertTo_doubleArray()");
        }
#endif

#if defined(PPP)
     intSerialArray *This_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
	       Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement ( *this, This_SerialArray);
             }
            else
             {
               Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement ( *this, This_SerialArray);
             }
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
          if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
               Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement (
                    *this, This_SerialArray, 
                    *Where_Statement_Support::Where_Statement_Mask,
                    Mask_SerialArray);
             }
            else
             {
               Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement 
	          ( *this, This_SerialArray, 
                    *Where_Statement_Support::Where_Statement_Mask,
                    Mask_SerialArray);
             }
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(This_SerialArray != NULL);

     bool thisIsTemporary = This_SerialArray->isTemporary();

     APP_ASSERT ( (thisIsTemporary == TRUE) || (thisIsTemporary == FALSE) );

#if defined(MEMORY_LEAK_TEST)
     printf ("Can't do MEMORY_LEAK_TEST in intSerialArray & intSerialArray::convertTo_doubleArray () \n");
#endif
     doubleArray & Return_Value = intArray::Abstract_double_Conversion_Operator ( Temporary_Array_Set, *this, This_SerialArray, This_SerialArray->convertTo_doubleArray() );

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
  // Since the Lhs is a different type than the Return Value it could not be reused!
     // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): This_SerialArray->getReferenceCount() = %d \n",
       //      This_SerialArray->getReferenceCount());

       // Must delete the This_SerialArray if it was taken directly from the This array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (This_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          This_SerialArray->decrementReferenceCount();
          if (This_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete This_SerialArray;
             }
          This_SerialArray = NULL;


  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & intSerialArray::convertTo_doubleArray()");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_double_Conversion_Operator ( *this ,
                   MDI_i_Array_convertTo_doubleArray_Array_Accumulate_To_Operand ,
                   intSerialArray::convertTo_doubleArrayFunction );
#endif
   }


intSerialArray &
intSerialArray::operator-= ( const intSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of intSerialArray::operator-= (intSerialArray) for intSerialArray class!");

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in intSerialArray::operator-=");
     Rhs.Test_Consistency ("Test Rhs in intSerialArray::operator-=");
#endif

  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Test_Conformability (Rhs);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS in intSerialArray & intSerialArray::operator-=(intSerialArray)");
          Rhs.displayReferenceCounts("Rhs in intSerialArray & intSerialArray::operator-=(intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *This_SerialArray = NULL;
     intSerialArray *Rhs_SerialArray  = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if ((Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	    (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      ( *this , This_SerialArray, Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      ( *this , This_SerialArray, Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if ((Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	    (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
        {
	   puts ("ERROR: can't mix indirect addressing with 2 arrays and where.");
           APP_ABORT();
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (*this, This_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray,
	       Rhs, Rhs_SerialArray );
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set =	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(This_SerialArray != NULL);
     APP_ASSERT(Rhs_SerialArray  != NULL);

     bool thisIsTemporary = This_SerialArray->isTemporary();
     bool rhsIsTemporary  = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (thisIsTemporary == TRUE) || (thisIsTemporary == FALSE) );
     APP_ASSERT ( (rhsIsTemporary  == TRUE) || (rhsIsTemporary  == FALSE) );

#if !defined(MEMORY_LEAK_TEST)
     intArray::Abstract_Modification_Operator ( Temporary_Array_Set, *this, Rhs, This_SerialArray, Rhs_SerialArray, *This_SerialArray -= *Rhs_SerialArray );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (thisIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): This_SerialArray->getReferenceCount() = %d \n",
       //      This_SerialArray->getReferenceCount());

       // Must delete the This_SerialArray if it was taken directly from the This array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (This_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          This_SerialArray->decrementReferenceCount();
          if (This_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete This_SerialArray;
             }
          This_SerialArray = NULL;

        }
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }

     // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#else
     intSerialArray::Abstract_Operator_Operation_Equals ( *this , Rhs ,
               MDI_i_Subtract_Array_Minus_Array_Accumulate_To_Operand , intSerialArray::Minus_Equals );
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS (return value) in intSerialArray & intSerialArray::operator-=(intSerialArray)");
        }
#endif

     return *this;
   }


intSerialArray &
intSerialArray::operator-= ( int x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of intSerialArray::operator-= (int) for intSerialArray class!");

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in intSerialArray::operator-=");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS in intSerialArray & intSerialArray::operator-=(int)");
        }
#endif

#if defined(PPP)
     intSerialArray *This_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( *this , This_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( *this , This_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (*this, This_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (*this, This_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set =	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(This_SerialArray != NULL);

     bool thisIsTemporary = This_SerialArray->isTemporary();

     APP_ASSERT ( (thisIsTemporary == TRUE) || (thisIsTemporary == FALSE) );

#if !defined(MEMORY_LEAK_TEST)
     intArray::Abstract_Modification_Operator ( Temporary_Array_Set, *this, This_SerialArray, *This_SerialArray -= x );
     // ... don't need to use macro because Return_Value won't be Mask ...
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (thisIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): This_SerialArray->getReferenceCount() = %d \n",
       //      This_SerialArray->getReferenceCount());

       // Must delete the This_SerialArray if it was taken directly from the This array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (This_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          This_SerialArray->decrementReferenceCount();
          if (This_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete This_SerialArray;
             }
          This_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;

#else
     intSerialArray::Abstract_Operator_Operation_Equals ( *this , x ,
        MDI_i_Subtract_Array_Minus_Scalar_Accumulate_To_Operand , intSerialArray::Scalar_Minus_Equals );
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS (return value) in intSerialArray & intSerialArray::operator-=(int)");
        }
#endif

     return *this;
   }


intSerialArray &
intSerialArray::operator- () const
   { 
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of unary minus operator operator- for intSerialArray class!");

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in intSerialArray::operatoroperator-");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS in intSerialArray & intSerialArray::operator-()");
        }
#endif

#if defined(PPP)
     intSerialArray *This_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( *this , This_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( *this , This_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (*this, This_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (*this, This_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set =	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(This_SerialArray != NULL);

     bool thisIsTemporary = This_SerialArray->isTemporary();

     APP_ASSERT ( (thisIsTemporary == TRUE) || (thisIsTemporary == FALSE) );

  // return intArray::Abstract_Operator ( Temporary_Array_Set, *this, This_SerialArray->operator-() );
#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = *this;
#else
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, *this, This_SerialArray, This_SerialArray->operator-() );
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value (before delete This_SerialArray) in intSerialArray & intSerialArray::operator-()");
        }

  // This is the only test we can do on the output!
     Return_Value.Test_Consistency ("Test Return_Value (before delete This_SerialArray) in intSerialArray::operatoroperator-");
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (thisIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): This_SerialArray->getReferenceCount() = %d \n",
       //      This_SerialArray->getReferenceCount());

       // Must delete the This_SerialArray if it was taken directly from the This array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (This_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          This_SerialArray->decrementReferenceCount();
          if (This_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete This_SerialArray;
             }
          This_SerialArray = NULL;

        }

     // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & intSerialArray::operator-()");
        }

  // This is the only test we can do on the output!
     Return_Value.Test_Consistency ("Test Return_Value in intSerialArray::operatoroperator-");
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Unary_Operator ( *this ,
                   MDI_i_Unary_Minus_Array ,
                   MDI_i_Unary_Minus_Array_Accumulate_To_Operand , intSerialArray::Unary_Minus );
#endif
   } 


intSerialArray &
operator* ( const intSerialArray & Lhs , const intSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
        {
          printf ("\n\n\n### Inside of operator* (intSerialArray,intSerialArray) for intSerialArray class: (id=%d) = (id=%d) \n",
               Lhs.Array_ID(),Rhs.Array_ID());
        }

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator*");
     Rhs.Test_Consistency ("Test Rhs in operator*");
#endif

  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Lhs.Test_Conformability (Rhs);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          printf ("intSerialArray: Lhs.isTemporary() = %s \n",(Lhs.isTemporary()) ? "TRUE" : "FALSE");
          Lhs.displayReferenceCounts("Lhs in intSerialArray & operator*(intSerialArray,intSerialArray)");
          printf ("intSerialArray: Rhs.isTemporary() = %s \n",(Rhs.isTemporary()) ? "TRUE" : "FALSE");
          Rhs.displayReferenceCounts("Rhs in intSerialArray & operator*(intSerialArray,intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray* Lhs_SerialArray = NULL;
     intSerialArray* Rhs_SerialArray = NULL;

     intSerialArray* Mask_SerialArray = NULL;
     intSerialArray* Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	      (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
               Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
          if ( (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
               (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) )
             {
               printf ("Sorry, not implemented: can't mix indirect addressing using where statements and two array (binary) operators!\n");
               APP_ABORT();
             }
            else
             {
               Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement
                  (Lhs, Lhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray, Rhs, Rhs_SerialArray );
             }
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          printf ("intSerialArray: Lhs_SerialArray->isTemporary() = %s \n",(Lhs_SerialArray->isTemporary()) ? "TRUE" : "FALSE");
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in intSerialArray & operator*(intSerialArray,intSerialArray)");
          printf ("intSerialArray: Rhs_SerialArray->isTemporary() = %s \n",(Rhs_SerialArray->isTemporary()) ? "TRUE" : "FALSE");
          Rhs_SerialArray->displayReferenceCounts("Rhs_SerialArray in intSerialArray & operator*(intSerialArray,intSerialArray)");
        }
#endif

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Lhs_SerialArray != NULL);
     APP_ASSERT(Rhs_SerialArray != NULL);

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();
     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );
     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

  // return intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, *Lhs_SerialArray * *Rhs_SerialArray );
#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = Lhs;
#else
     intArray & Return_Value = intArray::Abstract_Operator 
	( Temporary_Array_Set, Lhs, Rhs, Lhs_SerialArray, Rhs_SerialArray, *Lhs_SerialArray * *Rhs_SerialArray );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (lhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

        }
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }

     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & operator*(intSerialArray,intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator ( Lhs , Rhs , 
               MDI_i_Multiply_Array_Times_Array,
               MDI_i_Multiply_Array_Times_Array_Accumulate_To_Operand , intSerialArray::Times );
#endif
   }


intSerialArray &
operator* ( const intSerialArray & Lhs , int x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("\n\n\n### Inside of operator* (intSerialArray,int) for intSerialArray class: (id=%d) = scalar \n",Lhs.Array_ID());

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator*");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in intSerialArray & operator*(intSerialArray,int)");
        }
#endif

#if defined(PPP)
     intSerialArray* Lhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
               Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
               APP_ASSERT(Lhs_SerialArray != NULL);
            // Lhs_SerialArray->displayReferenceCounts("AFTER PCE: *Lhs_SerialArray in intSerialArray & operator*(intSerialArray,int)");
             }
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
          if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
               Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
                    (Lhs, Lhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
             }
            else
             {
               Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
                    (Lhs, Lhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
             }
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          printf ("intSerialArray: Lhs_SerialArray->isTemporary() = %s \n",(Lhs_SerialArray->isTemporary()) ? "TRUE" : "FALSE");
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in intSerialArray & operator*(intSerialArray,int)");
        }
#endif

  // (11/27/2000) Added error checking (will not work with indirect addessing later!!!)
     APP_ASSERT(Temporary_Array_Set != NULL);

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Lhs_SerialArray != NULL);

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );

#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = Lhs;
#else
     intArray & Return_Value = intArray::Abstract_Operator(Temporary_Array_Set,Lhs,Lhs_SerialArray,*Lhs_SerialArray * x);
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (lhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in in intSerialArray & operator*(intSerialArray,int)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator ( Lhs , x , 
               MDI_i_Multiply_Array_Times_Scalar,
               MDI_i_Multiply_Array_Times_Scalar_Accumulate_To_Operand , intSerialArray::Scalar_Times );
#endif
   }


intSerialArray &
operator* ( int x , const intSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator* (int,intSerialArray) for intSerialArray class!");

  // This is the only test we can do on the input!
     Rhs.Test_Consistency ("Test Rhs in operator*");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs.displayReferenceCounts("Rhs in intSerialArray & operator*(int,intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray* Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      ( Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = 
	      intArray::Parallel_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }
     if (Temporary_Array_Set == NULL) Temporary_Array_Set =
	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Rhs_SerialArray != NULL);

     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = Rhs;
#else
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Rhs, Rhs_SerialArray, x * *Rhs_SerialArray );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & operator*(int,intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator ( Rhs , x , 
               MDI_i_Multiply_Scalar_Times_Array,
               MDI_i_Multiply_Scalar_Times_Array_Accumulate_To_Operand , intSerialArray::Scalar_Times );
#endif
   }


intSerialArray &
intSerialArray::operator*= ( const intSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of intSerialArray::operator*= (intSerialArray) for intSerialArray class!");

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in intSerialArray::operator*=");
     Rhs.Test_Consistency ("Test Rhs in intSerialArray::operator*=");
#endif

  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Test_Conformability (Rhs);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS in intSerialArray & intSerialArray::operator*=(intSerialArray)");
          Rhs.displayReferenceCounts("Rhs in intSerialArray & intSerialArray::operator*=(intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *This_SerialArray = NULL;
     intSerialArray *Rhs_SerialArray  = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if ((Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	    (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      ( *this , This_SerialArray, Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      ( *this , This_SerialArray, Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if ((Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	    (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
        {
	   puts ("ERROR: can't mix indirect addressing with 2 arrays and where.");
           APP_ABORT();
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (*this, This_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray,
	       Rhs, Rhs_SerialArray );
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set =	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(This_SerialArray != NULL);
     APP_ASSERT(Rhs_SerialArray  != NULL);

     bool thisIsTemporary = This_SerialArray->isTemporary();
     bool rhsIsTemporary  = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (thisIsTemporary == TRUE) || (thisIsTemporary == FALSE) );
     APP_ASSERT ( (rhsIsTemporary  == TRUE) || (rhsIsTemporary  == FALSE) );

#if !defined(MEMORY_LEAK_TEST)
     intArray::Abstract_Modification_Operator ( Temporary_Array_Set, *this, Rhs, This_SerialArray, Rhs_SerialArray, *This_SerialArray *= *Rhs_SerialArray );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (thisIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): This_SerialArray->getReferenceCount() = %d \n",
       //      This_SerialArray->getReferenceCount());

       // Must delete the This_SerialArray if it was taken directly from the This array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (This_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          This_SerialArray->decrementReferenceCount();
          if (This_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete This_SerialArray;
             }
          This_SerialArray = NULL;

        }
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }

     // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#else
     intSerialArray::Abstract_Operator_Operation_Equals ( *this , Rhs ,
               MDI_i_Multiply_Array_Times_Array_Accumulate_To_Operand , intSerialArray::Times_Equals );
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS (return value) in intSerialArray & intSerialArray::operator*=(intSerialArray)");
        }
#endif

     return *this;
   }


intSerialArray &
intSerialArray::operator*= ( int x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of intSerialArray::operator*= (int) for intSerialArray class!");

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in intSerialArray::operator*=");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS in intSerialArray & intSerialArray::operator*=(int)");
        }
#endif

#if defined(PPP)
     intSerialArray *This_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( *this , This_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( *this , This_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (*this, This_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (*this, This_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set =	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(This_SerialArray != NULL);

     bool thisIsTemporary = This_SerialArray->isTemporary();

     APP_ASSERT ( (thisIsTemporary == TRUE) || (thisIsTemporary == FALSE) );

#if !defined(MEMORY_LEAK_TEST)
     intArray::Abstract_Modification_Operator ( Temporary_Array_Set, *this, This_SerialArray, *This_SerialArray *= x );
     // ... don't need to use macro because Return_Value won't be Mask ...
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (thisIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): This_SerialArray->getReferenceCount() = %d \n",
       //      This_SerialArray->getReferenceCount());

       // Must delete the This_SerialArray if it was taken directly from the This array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (This_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          This_SerialArray->decrementReferenceCount();
          if (This_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete This_SerialArray;
             }
          This_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;

#else
     intSerialArray::Abstract_Operator_Operation_Equals ( *this , x ,
        MDI_i_Multiply_Array_Times_Scalar_Accumulate_To_Operand , intSerialArray::Scalar_Times_Equals );
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS (return value) in intSerialArray & intSerialArray::operator*=(int)");
        }
#endif

     return *this;
   }


intSerialArray &
operator/ ( const intSerialArray & Lhs , const intSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator/ (intSerialArray,intSerialArray) for intSerialArray class!");

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator/");
     Rhs.Test_Consistency ("Test Rhs in operator/");
#endif

  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Lhs.Test_Conformability (Rhs);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in intSerialArray & operator/(intSerialArray,intSerialArray)");
          Rhs.displayReferenceCounts("Rhs in intSerialArray & operator/(intSerialArray,intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray* Lhs_SerialArray = NULL;
     intSerialArray* Rhs_SerialArray = NULL;

     intSerialArray* Mask_SerialArray = NULL;
     intSerialArray* Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	    (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
        {
	   puts ("ERROR: can't mix indirect addressing with 2 arrays and where.");
           APP_ABORT();
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray,
	       Rhs, Rhs_SerialArray );
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set =	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Lhs_SerialArray != NULL);
     APP_ASSERT(Rhs_SerialArray != NULL);

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();
     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );
     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

  // return intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, *Lhs_SerialArray / *Rhs_SerialArray );
#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = Lhs;
#else
     intArray & Return_Value = intArray::Abstract_Operator 
	( Temporary_Array_Set, Lhs, Rhs, 
	  Lhs_SerialArray, Rhs_SerialArray, *Lhs_SerialArray / *Rhs_SerialArray );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (lhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

        }
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }

     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & operator/(intSerialArray,intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator_Non_Commutative ( Lhs , Rhs , 
               MDI_i_Divide_Array_Divided_By_Array,
               MDI_i_Divide_Array_Divided_By_Array_Accumulate_To_Operand , intSerialArray::Divided_By );
#endif
   }


intSerialArray &
operator+ ( const intSerialArray & Lhs , int x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("\n\n\n### Inside of operator+ (intSerialArray,int) for intSerialArray class: (id=%d) = scalar \n",Lhs.Array_ID());

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator+");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in intSerialArray & operator+(intSerialArray,int)");
        }
#endif

#if defined(PPP)
     intSerialArray* Lhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
               Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
               APP_ASSERT(Lhs_SerialArray != NULL);
            // Lhs_SerialArray->displayReferenceCounts("AFTER PCE: *Lhs_SerialArray in intSerialArray & operator+(intSerialArray,int)");
             }
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
          if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
               Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
                    (Lhs, Lhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
             }
            else
             {
               Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
                    (Lhs, Lhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
             }
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          printf ("intSerialArray: Lhs_SerialArray->isTemporary() = %s \n",(Lhs_SerialArray->isTemporary()) ? "TRUE" : "FALSE");
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in intSerialArray & operator+(intSerialArray,int)");
        }
#endif

  // (11/27/2000) Added error checking (will not work with indirect addessing later!!!)
     APP_ASSERT(Temporary_Array_Set != NULL);

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Lhs_SerialArray != NULL);

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );

#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = Lhs;
#else
     intArray & Return_Value = intArray::Abstract_Operator(Temporary_Array_Set,Lhs,Lhs_SerialArray,*Lhs_SerialArray + x);
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (lhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in in intSerialArray & operator+(intSerialArray,int)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator ( Lhs , x , 
               MDI_i_Add_Array_Plus_Scalar,
               MDI_i_Add_Array_Plus_Scalar_Accumulate_To_Operand , intSerialArray::Scalar_Plus );
#endif
   }


intSerialArray &
operator/ ( const intSerialArray & Lhs , int x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator/ (intSerialArray,int) for intSerialArray class!");

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator/");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in intSerialArray & operator/(intSerialArray,int)");
        }
#endif

#if defined(PPP)
     intSerialArray* Lhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      ( Lhs, Lhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = 
	      intArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set =	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Lhs_SerialArray     != NULL);

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );

#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = Lhs;
#else
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Lhs_SerialArray, *Lhs_SerialArray / x );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (lhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & operator/(intSerialArray,int)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator ( Lhs , x , 
               MDI_i_Divide_Array_Divided_By_Scalar,
               MDI_i_Divide_Array_Divided_By_Scalar_Accumulate_To_Operand , intSerialArray::Scalar_Divided_By );
#endif
   }


intSerialArray &
operator/ ( int x , const intSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of operator/ (int,intSerialArray) for intSerialArray class! \n");

  // This is the only test we can do on the input!
     Rhs.Test_Consistency ("Test Rhs in operator/");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs.displayReferenceCounts("Rhs in intSerialArray & operator/(int,intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray* Rhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE)
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      ( Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = 
	      intArray::Parallel_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }
     if (Temporary_Array_Set == NULL) Temporary_Array_Set =
	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Rhs_SerialArray != NULL);

     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = Rhs;
#else
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Rhs, Rhs_SerialArray, x / *Rhs_SerialArray );
  // return intArray::Abstract_Binary_Operator ( Temporary_Array_Set, Rhs, x / *Rhs_SerialArray );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & operator/(int,intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator ( Rhs , x , 
               MDI_i_Divide_Scalar_Divided_By_Array,
               MDI_i_Divide_Scalar_Divided_By_Array_Accumulate_To_Operand , intSerialArray::Scalar_Divided_By );
#endif
   }


intSerialArray &
intSerialArray::operator/= ( const intSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of intSerialArray::operator/= (intSerialArray) for intSerialArray class!");

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in intSerialArray::operator/=");
     Rhs.Test_Consistency ("Test Rhs in intSerialArray::operator/=");
#endif

  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Test_Conformability (Rhs);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS in intSerialArray & intSerialArray::operator/=(intSerialArray)");
          Rhs.displayReferenceCounts("Rhs in intSerialArray & intSerialArray::operator/=(intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *This_SerialArray = NULL;
     intSerialArray *Rhs_SerialArray  = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if ((Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	    (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      ( *this , This_SerialArray, Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      ( *this , This_SerialArray, Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if ((Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	    (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
        {
	   puts ("ERROR: can't mix indirect addressing with 2 arrays and where.");
           APP_ABORT();
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (*this, This_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray,
	       Rhs, Rhs_SerialArray );
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set =	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(This_SerialArray != NULL);
     APP_ASSERT(Rhs_SerialArray  != NULL);

     bool thisIsTemporary = This_SerialArray->isTemporary();
     bool rhsIsTemporary  = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (thisIsTemporary == TRUE) || (thisIsTemporary == FALSE) );
     APP_ASSERT ( (rhsIsTemporary  == TRUE) || (rhsIsTemporary  == FALSE) );

#if !defined(MEMORY_LEAK_TEST)
     intArray::Abstract_Modification_Operator ( Temporary_Array_Set, *this, Rhs, This_SerialArray, Rhs_SerialArray, *This_SerialArray /= *Rhs_SerialArray );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (thisIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): This_SerialArray->getReferenceCount() = %d \n",
       //      This_SerialArray->getReferenceCount());

       // Must delete the This_SerialArray if it was taken directly from the This array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (This_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          This_SerialArray->decrementReferenceCount();
          if (This_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete This_SerialArray;
             }
          This_SerialArray = NULL;

        }
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }

     // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#else
     intSerialArray::Abstract_Operator_Operation_Equals ( *this , Rhs ,
               MDI_i_Divide_Array_Divided_By_Array_Accumulate_To_Operand , intSerialArray::Divided_By_Equals );
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS (return value) in intSerialArray & intSerialArray::operator/=(intSerialArray)");
        }
#endif

     return *this;
   }


intSerialArray &
intSerialArray::operator/= ( int x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of intSerialArray::operator/= (int) for intSerialArray class!");

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in intSerialArray::operator/=");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS in intSerialArray & intSerialArray::operator/=(int)");
        }
#endif

#if defined(PPP)
     intSerialArray *This_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( *this , This_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( *this , This_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (*this, This_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (*this, This_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set =	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(This_SerialArray != NULL);

     bool thisIsTemporary = This_SerialArray->isTemporary();

     APP_ASSERT ( (thisIsTemporary == TRUE) || (thisIsTemporary == FALSE) );

#if !defined(MEMORY_LEAK_TEST)
     intArray::Abstract_Modification_Operator ( Temporary_Array_Set, *this, This_SerialArray, *This_SerialArray /= x );
     // ... don't need to use macro because Return_Value won't be Mask ...
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (thisIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): This_SerialArray->getReferenceCount() = %d \n",
       //      This_SerialArray->getReferenceCount());

       // Must delete the This_SerialArray if it was taken directly from the This array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (This_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          This_SerialArray->decrementReferenceCount();
          if (This_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete This_SerialArray;
             }
          This_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;

#else
     intSerialArray::Abstract_Operator_Operation_Equals ( *this , x ,
        MDI_i_Divide_Array_Divided_By_Scalar_Accumulate_To_Operand , intSerialArray::Scalar_Divided_By_Equals );
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS (return value) in intSerialArray & intSerialArray::operator/=(int)");
        }
#endif

     return *this;
   }


intSerialArray &
operator% ( const intSerialArray & Lhs , const intSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
        {
          printf ("\n\n\n### Inside of operator% (intSerialArray,intSerialArray) for intSerialArray class: (id=%d) = (id=%d) \n",
               Lhs.Array_ID(),Rhs.Array_ID());
        }

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator%");
     Rhs.Test_Consistency ("Test Rhs in operator%");
#endif

  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Lhs.Test_Conformability (Rhs);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          printf ("intSerialArray: Lhs.isTemporary() = %s \n",(Lhs.isTemporary()) ? "TRUE" : "FALSE");
          Lhs.displayReferenceCounts("Lhs in intSerialArray & operator%(intSerialArray,intSerialArray)");
          printf ("intSerialArray: Rhs.isTemporary() = %s \n",(Rhs.isTemporary()) ? "TRUE" : "FALSE");
          Rhs.displayReferenceCounts("Rhs in intSerialArray & operator%(intSerialArray,intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray* Lhs_SerialArray = NULL;
     intSerialArray* Rhs_SerialArray = NULL;

     intSerialArray* Mask_SerialArray = NULL;
     intSerialArray* Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	      (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
               Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
          if ( (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
               (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) )
             {
               printf ("Sorry, not implemented: can't mix indirect addressing using where statements and two array (binary) operators!\n");
               APP_ABORT();
             }
            else
             {
               Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement
                  (Lhs, Lhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray, Rhs, Rhs_SerialArray );
             }
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          printf ("intSerialArray: Lhs_SerialArray->isTemporary() = %s \n",(Lhs_SerialArray->isTemporary()) ? "TRUE" : "FALSE");
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in intSerialArray & operator%(intSerialArray,intSerialArray)");
          printf ("intSerialArray: Rhs_SerialArray->isTemporary() = %s \n",(Rhs_SerialArray->isTemporary()) ? "TRUE" : "FALSE");
          Rhs_SerialArray->displayReferenceCounts("Rhs_SerialArray in intSerialArray & operator%(intSerialArray,intSerialArray)");
        }
#endif

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Lhs_SerialArray != NULL);
     APP_ASSERT(Rhs_SerialArray != NULL);

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();
     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );
     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

  // return intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, *Lhs_SerialArray % *Rhs_SerialArray );
#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = Lhs;
#else
     intArray & Return_Value = intArray::Abstract_Operator 
	( Temporary_Array_Set, Lhs, Rhs, Lhs_SerialArray, Rhs_SerialArray, *Lhs_SerialArray % *Rhs_SerialArray );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (lhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

        }
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }

     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & operator%(intSerialArray,intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator ( Lhs , Rhs , 
               MDI_i_Fmod_Array_Modulo_Array,
               MDI_i_Fmod_Array_Modulo_Array_Accumulate_To_Operand , intSerialArray::Modulo );
#endif
   }


intSerialArray &
operator% ( const intSerialArray & Lhs , int x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("\n\n\n### Inside of operator% (intSerialArray,int) for intSerialArray class: (id=%d) = scalar \n",Lhs.Array_ID());

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator%");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in intSerialArray & operator%(intSerialArray,int)");
        }
#endif

#if defined(PPP)
     intSerialArray* Lhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
               Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
               APP_ASSERT(Lhs_SerialArray != NULL);
            // Lhs_SerialArray->displayReferenceCounts("AFTER PCE: *Lhs_SerialArray in intSerialArray & operator%(intSerialArray,int)");
             }
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
          if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
               Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
                    (Lhs, Lhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
             }
            else
             {
               Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
                    (Lhs, Lhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
             }
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          printf ("intSerialArray: Lhs_SerialArray->isTemporary() = %s \n",(Lhs_SerialArray->isTemporary()) ? "TRUE" : "FALSE");
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in intSerialArray & operator%(intSerialArray,int)");
        }
#endif

  // (11/27/2000) Added error checking (will not work with indirect addessing later!!!)
     APP_ASSERT(Temporary_Array_Set != NULL);

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Lhs_SerialArray != NULL);

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );

#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = Lhs;
#else
     intArray & Return_Value = intArray::Abstract_Operator(Temporary_Array_Set,Lhs,Lhs_SerialArray,*Lhs_SerialArray % x);
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (lhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in in intSerialArray & operator%(intSerialArray,int)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator ( Lhs , x , 
               MDI_i_Fmod_Array_Modulo_Scalar,
               MDI_i_Fmod_Array_Modulo_Scalar_Accumulate_To_Operand , intSerialArray::Scalar_Modulo );
#endif
   }


intSerialArray &
operator% ( int x , const intSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator% (int,intSerialArray) for intSerialArray class!");

  // This is the only test we can do on the input!
     Rhs.Test_Consistency ("Test Rhs in operator%");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs.displayReferenceCounts("Rhs in intSerialArray & operator%(int,intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray* Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      ( Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = 
	      intArray::Parallel_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }
     if (Temporary_Array_Set == NULL) Temporary_Array_Set =
	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Rhs_SerialArray != NULL);

     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = Rhs;
#else
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Rhs, Rhs_SerialArray, x % *Rhs_SerialArray );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & operator%(int,intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator ( Rhs , x , 
               MDI_i_Fmod_Scalar_Modulo_Array,
               MDI_i_Fmod_Scalar_Modulo_Array_Accumulate_To_Operand , intSerialArray::Scalar_Modulo );
#endif
   }


intSerialArray &
intSerialArray::operator%= ( const intSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of intSerialArray::operator%= (intSerialArray) for intSerialArray class!");

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in intSerialArray::operator%=");
     Rhs.Test_Consistency ("Test Rhs in intSerialArray::operator%=");
#endif

  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Test_Conformability (Rhs);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS in intSerialArray & intSerialArray::operator%=(intSerialArray)");
          Rhs.displayReferenceCounts("Rhs in intSerialArray & intSerialArray::operator%=(intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *This_SerialArray = NULL;
     intSerialArray *Rhs_SerialArray  = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if ((Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	    (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      ( *this , This_SerialArray, Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      ( *this , This_SerialArray, Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if ((Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	    (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
        {
	   puts ("ERROR: can't mix indirect addressing with 2 arrays and where.");
           APP_ABORT();
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (*this, This_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray,
	       Rhs, Rhs_SerialArray );
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set =	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(This_SerialArray != NULL);
     APP_ASSERT(Rhs_SerialArray  != NULL);

     bool thisIsTemporary = This_SerialArray->isTemporary();
     bool rhsIsTemporary  = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (thisIsTemporary == TRUE) || (thisIsTemporary == FALSE) );
     APP_ASSERT ( (rhsIsTemporary  == TRUE) || (rhsIsTemporary  == FALSE) );

#if !defined(MEMORY_LEAK_TEST)
     intArray::Abstract_Modification_Operator ( Temporary_Array_Set, *this, Rhs, This_SerialArray, Rhs_SerialArray, *This_SerialArray %= *Rhs_SerialArray );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (thisIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): This_SerialArray->getReferenceCount() = %d \n",
       //      This_SerialArray->getReferenceCount());

       // Must delete the This_SerialArray if it was taken directly from the This array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (This_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          This_SerialArray->decrementReferenceCount();
          if (This_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete This_SerialArray;
             }
          This_SerialArray = NULL;

        }
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }

     // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#else
     intSerialArray::Abstract_Operator_Operation_Equals ( *this , Rhs ,
               MDI_i_Fmod_Array_Modulo_Array_Accumulate_To_Operand , intSerialArray::Modulo_Equals );
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS (return value) in intSerialArray & intSerialArray::operator%=(intSerialArray)");
        }
#endif

     return *this;
   }


intSerialArray &
intSerialArray::operator%= ( int x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of intSerialArray::operator%= (int) for intSerialArray class!");

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in intSerialArray::operator%=");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS in intSerialArray & intSerialArray::operator%=(int)");
        }
#endif

#if defined(PPP)
     intSerialArray *This_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( *this , This_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( *this , This_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (*this, This_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (*this, This_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set =	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(This_SerialArray != NULL);

     bool thisIsTemporary = This_SerialArray->isTemporary();

     APP_ASSERT ( (thisIsTemporary == TRUE) || (thisIsTemporary == FALSE) );

#if !defined(MEMORY_LEAK_TEST)
     intArray::Abstract_Modification_Operator ( Temporary_Array_Set, *this, This_SerialArray, *This_SerialArray %= x );
     // ... don't need to use macro because Return_Value won't be Mask ...
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (thisIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): This_SerialArray->getReferenceCount() = %d \n",
       //      This_SerialArray->getReferenceCount());

       // Must delete the This_SerialArray if it was taken directly from the This array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (This_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          This_SerialArray->decrementReferenceCount();
          if (This_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete This_SerialArray;
             }
          This_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;

#else
     intSerialArray::Abstract_Operator_Operation_Equals ( *this , x ,
        MDI_i_Fmod_Array_Modulo_Scalar_Accumulate_To_Operand , intSerialArray::Scalar_Modulo_Equals );
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS (return value) in intSerialArray & intSerialArray::operator%=(int)");
        }
#endif

     return *this;
   }


#ifndef INTARRAY
intSerialArray &
cos ( const intSerialArray & X )
   { 
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of cos for intSerialArray class! \n");

  // This is the only test we can do on the input!
     X.Test_Consistency ("Test X in cos ( const intSerialArray & X )");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in intSerialArray & cos(intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *X_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( X, X_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( X, X_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set =	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(X_SerialArray       != NULL);

     bool xIsTemporary = X_SerialArray->isTemporary();

     APP_ASSERT ( (xIsTemporary == TRUE) || (xIsTemporary == FALSE) );

  // return intArray::Abstract_Operator ( Temporary_Array_Set, X, cos(*X_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = X;
#else
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, X, X_SerialArray, cos(*X_SerialArray) );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (xIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): X_SerialArray->getReferenceCount() = %d \n",
       //      X_SerialArray->getReferenceCount());

       // Must delete the X_SerialArray if it was taken directly from the X array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (X_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          X_SerialArray->decrementReferenceCount();
          if (X_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete X_SerialArray;
             }
          X_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & cos(intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Unary_Operator ( X ,
                   MDI_i_Cos_Array ,
                   MDI_i_Cos_Array_Accumulate_To_Operand , intSerialArray::cos_Function );
#endif
   } 

#endif

intSerialArray &
operator+ ( int x , const intSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator+ (int,intSerialArray) for intSerialArray class!");

  // This is the only test we can do on the input!
     Rhs.Test_Consistency ("Test Rhs in operator+");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs.displayReferenceCounts("Rhs in intSerialArray & operator+(int,intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray* Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      ( Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = 
	      intArray::Parallel_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }
     if (Temporary_Array_Set == NULL) Temporary_Array_Set =
	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Rhs_SerialArray != NULL);

     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = Rhs;
#else
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Rhs, Rhs_SerialArray, x + *Rhs_SerialArray );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & operator+(int,intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator ( Rhs , x , 
               MDI_i_Add_Scalar_Plus_Array,
               MDI_i_Add_Scalar_Plus_Array_Accumulate_To_Operand , intSerialArray::Scalar_Plus );
#endif
   }


#ifndef INTARRAY
intSerialArray &
sin ( const intSerialArray & X )
   { 
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of sin for intSerialArray class! \n");

  // This is the only test we can do on the input!
     X.Test_Consistency ("Test X in sin ( const intSerialArray & X )");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in intSerialArray & sin(intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *X_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( X, X_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( X, X_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set =	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(X_SerialArray       != NULL);

     bool xIsTemporary = X_SerialArray->isTemporary();

     APP_ASSERT ( (xIsTemporary == TRUE) || (xIsTemporary == FALSE) );

  // return intArray::Abstract_Operator ( Temporary_Array_Set, X, sin(*X_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = X;
#else
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, X, X_SerialArray, sin(*X_SerialArray) );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (xIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): X_SerialArray->getReferenceCount() = %d \n",
       //      X_SerialArray->getReferenceCount());

       // Must delete the X_SerialArray if it was taken directly from the X array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (X_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          X_SerialArray->decrementReferenceCount();
          if (X_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete X_SerialArray;
             }
          X_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & sin(intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Unary_Operator ( X ,
                   MDI_i_Sin_Array ,
                   MDI_i_Sin_Array_Accumulate_To_Operand , intSerialArray::sin_Function );
#endif
   } 

#endif

#ifndef INTARRAY
intSerialArray &
tan ( const intSerialArray & X )
   { 
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of tan for intSerialArray class! \n");

  // This is the only test we can do on the input!
     X.Test_Consistency ("Test X in tan ( const intSerialArray & X )");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in intSerialArray & tan(intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *X_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( X, X_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( X, X_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set =	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(X_SerialArray       != NULL);

     bool xIsTemporary = X_SerialArray->isTemporary();

     APP_ASSERT ( (xIsTemporary == TRUE) || (xIsTemporary == FALSE) );

  // return intArray::Abstract_Operator ( Temporary_Array_Set, X, tan(*X_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = X;
#else
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, X, X_SerialArray, tan(*X_SerialArray) );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (xIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): X_SerialArray->getReferenceCount() = %d \n",
       //      X_SerialArray->getReferenceCount());

       // Must delete the X_SerialArray if it was taken directly from the X array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (X_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          X_SerialArray->decrementReferenceCount();
          if (X_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete X_SerialArray;
             }
          X_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & tan(intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Unary_Operator ( X ,
                   MDI_i_Tan_Array ,
                   MDI_i_Tan_Array_Accumulate_To_Operand , intSerialArray::tan_Function );
#endif
   } 

#endif

#ifndef INTARRAY
intSerialArray &
acos ( const intSerialArray & X )
   { 
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of acos for intSerialArray class! \n");

  // This is the only test we can do on the input!
     X.Test_Consistency ("Test X in acos ( const intSerialArray & X )");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in intSerialArray & acos(intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *X_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( X, X_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( X, X_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set =	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(X_SerialArray       != NULL);

     bool xIsTemporary = X_SerialArray->isTemporary();

     APP_ASSERT ( (xIsTemporary == TRUE) || (xIsTemporary == FALSE) );

  // return intArray::Abstract_Operator ( Temporary_Array_Set, X, acos(*X_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = X;
#else
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, X, X_SerialArray, acos(*X_SerialArray) );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (xIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): X_SerialArray->getReferenceCount() = %d \n",
       //      X_SerialArray->getReferenceCount());

       // Must delete the X_SerialArray if it was taken directly from the X array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (X_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          X_SerialArray->decrementReferenceCount();
          if (X_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete X_SerialArray;
             }
          X_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & acos(intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Unary_Operator ( X ,
                   MDI_i_Arc_Cos_Array ,
                   MDI_i_Arc_Cos_Array_Accumulate_To_Operand , intSerialArray::acos_Function );
#endif
   } 

#endif

#ifndef INTARRAY
intSerialArray &
asin ( const intSerialArray & X )
   { 
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of asin for intSerialArray class! \n");

  // This is the only test we can do on the input!
     X.Test_Consistency ("Test X in asin ( const intSerialArray & X )");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in intSerialArray & asin(intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *X_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( X, X_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( X, X_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set =	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(X_SerialArray       != NULL);

     bool xIsTemporary = X_SerialArray->isTemporary();

     APP_ASSERT ( (xIsTemporary == TRUE) || (xIsTemporary == FALSE) );

  // return intArray::Abstract_Operator ( Temporary_Array_Set, X, asin(*X_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = X;
#else
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, X, X_SerialArray, asin(*X_SerialArray) );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (xIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): X_SerialArray->getReferenceCount() = %d \n",
       //      X_SerialArray->getReferenceCount());

       // Must delete the X_SerialArray if it was taken directly from the X array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (X_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          X_SerialArray->decrementReferenceCount();
          if (X_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete X_SerialArray;
             }
          X_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & asin(intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Unary_Operator ( X ,
                   MDI_i_Arc_Sin_Array ,
                   MDI_i_Arc_Sin_Array_Accumulate_To_Operand , intSerialArray::asin_Function );
#endif
   } 

#endif

#ifndef INTARRAY
intSerialArray &
atan ( const intSerialArray & X )
   { 
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of atan for intSerialArray class! \n");

  // This is the only test we can do on the input!
     X.Test_Consistency ("Test X in atan ( const intSerialArray & X )");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in intSerialArray & atan(intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *X_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( X, X_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( X, X_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set =	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(X_SerialArray       != NULL);

     bool xIsTemporary = X_SerialArray->isTemporary();

     APP_ASSERT ( (xIsTemporary == TRUE) || (xIsTemporary == FALSE) );

  // return intArray::Abstract_Operator ( Temporary_Array_Set, X, atan(*X_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = X;
#else
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, X, X_SerialArray, atan(*X_SerialArray) );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (xIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): X_SerialArray->getReferenceCount() = %d \n",
       //      X_SerialArray->getReferenceCount());

       // Must delete the X_SerialArray if it was taken directly from the X array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (X_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          X_SerialArray->decrementReferenceCount();
          if (X_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete X_SerialArray;
             }
          X_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & atan(intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Unary_Operator ( X ,
                   MDI_i_Arc_Tan_Array ,
                   MDI_i_Arc_Tan_Array_Accumulate_To_Operand , intSerialArray::atan_Function );
#endif
   } 

#endif

#ifndef INTARRAY
intSerialArray &
cosh ( const intSerialArray & X )
   { 
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of cosh for intSerialArray class! \n");

  // This is the only test we can do on the input!
     X.Test_Consistency ("Test X in cosh ( const intSerialArray & X )");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in intSerialArray & cosh(intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *X_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( X, X_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( X, X_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set =	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(X_SerialArray       != NULL);

     bool xIsTemporary = X_SerialArray->isTemporary();

     APP_ASSERT ( (xIsTemporary == TRUE) || (xIsTemporary == FALSE) );

  // return intArray::Abstract_Operator ( Temporary_Array_Set, X, cosh(*X_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = X;
#else
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, X, X_SerialArray, cosh(*X_SerialArray) );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (xIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): X_SerialArray->getReferenceCount() = %d \n",
       //      X_SerialArray->getReferenceCount());

       // Must delete the X_SerialArray if it was taken directly from the X array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (X_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          X_SerialArray->decrementReferenceCount();
          if (X_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete X_SerialArray;
             }
          X_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & cosh(intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Unary_Operator ( X ,
                   MDI_i_Cosh_Array ,
                   MDI_i_Cosh_Array_Accumulate_To_Operand , intSerialArray::cosh_Function );
#endif
   } 

#endif

#ifndef INTARRAY
intSerialArray &
sinh ( const intSerialArray & X )
   { 
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of sinh for intSerialArray class! \n");

  // This is the only test we can do on the input!
     X.Test_Consistency ("Test X in sinh ( const intSerialArray & X )");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in intSerialArray & sinh(intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *X_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( X, X_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( X, X_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set =	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(X_SerialArray       != NULL);

     bool xIsTemporary = X_SerialArray->isTemporary();

     APP_ASSERT ( (xIsTemporary == TRUE) || (xIsTemporary == FALSE) );

  // return intArray::Abstract_Operator ( Temporary_Array_Set, X, sinh(*X_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = X;
#else
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, X, X_SerialArray, sinh(*X_SerialArray) );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (xIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): X_SerialArray->getReferenceCount() = %d \n",
       //      X_SerialArray->getReferenceCount());

       // Must delete the X_SerialArray if it was taken directly from the X array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (X_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          X_SerialArray->decrementReferenceCount();
          if (X_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete X_SerialArray;
             }
          X_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & sinh(intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Unary_Operator ( X ,
                   MDI_i_Sinh_Array ,
                   MDI_i_Sinh_Array_Accumulate_To_Operand , intSerialArray::sinh_Function );
#endif
   } 

#endif

#ifndef INTARRAY
intSerialArray &
tanh ( const intSerialArray & X )
   { 
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of tanh for intSerialArray class! \n");

  // This is the only test we can do on the input!
     X.Test_Consistency ("Test X in tanh ( const intSerialArray & X )");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in intSerialArray & tanh(intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *X_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( X, X_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( X, X_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set =	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(X_SerialArray       != NULL);

     bool xIsTemporary = X_SerialArray->isTemporary();

     APP_ASSERT ( (xIsTemporary == TRUE) || (xIsTemporary == FALSE) );

  // return intArray::Abstract_Operator ( Temporary_Array_Set, X, tanh(*X_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = X;
#else
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, X, X_SerialArray, tanh(*X_SerialArray) );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (xIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): X_SerialArray->getReferenceCount() = %d \n",
       //      X_SerialArray->getReferenceCount());

       // Must delete the X_SerialArray if it was taken directly from the X array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (X_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          X_SerialArray->decrementReferenceCount();
          if (X_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete X_SerialArray;
             }
          X_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & tanh(intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Unary_Operator ( X ,
                   MDI_i_Tanh_Array ,
                   MDI_i_Tanh_Array_Accumulate_To_Operand , intSerialArray::tanh_Function );
#endif
   } 

#endif

#ifndef INTARRAY
intSerialArray &
acosh ( const intSerialArray & X )
   { 
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of acosh for intSerialArray class! \n");

  // This is the only test we can do on the input!
     X.Test_Consistency ("Test X in acosh ( const intSerialArray & X )");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in intSerialArray & acosh(intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *X_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( X, X_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( X, X_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set =	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(X_SerialArray       != NULL);

     bool xIsTemporary = X_SerialArray->isTemporary();

     APP_ASSERT ( (xIsTemporary == TRUE) || (xIsTemporary == FALSE) );

  // return intArray::Abstract_Operator ( Temporary_Array_Set, X, acosh(*X_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = X;
#else
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, X, X_SerialArray, acosh(*X_SerialArray) );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (xIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): X_SerialArray->getReferenceCount() = %d \n",
       //      X_SerialArray->getReferenceCount());

       // Must delete the X_SerialArray if it was taken directly from the X array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (X_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          X_SerialArray->decrementReferenceCount();
          if (X_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete X_SerialArray;
             }
          X_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & acosh(intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Unary_Operator ( X ,
                   MDI_i_Arc_Cosh_Array ,
                   MDI_i_Arc_Cosh_Array_Accumulate_To_Operand , intSerialArray::acosh_Function );
#endif
   } 

#endif

#ifndef INTARRAY
intSerialArray &
asinh ( const intSerialArray & X )
   { 
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of asinh for intSerialArray class! \n");

  // This is the only test we can do on the input!
     X.Test_Consistency ("Test X in asinh ( const intSerialArray & X )");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in intSerialArray & asinh(intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *X_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( X, X_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( X, X_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set =	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(X_SerialArray       != NULL);

     bool xIsTemporary = X_SerialArray->isTemporary();

     APP_ASSERT ( (xIsTemporary == TRUE) || (xIsTemporary == FALSE) );

  // return intArray::Abstract_Operator ( Temporary_Array_Set, X, asinh(*X_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = X;
#else
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, X, X_SerialArray, asinh(*X_SerialArray) );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (xIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): X_SerialArray->getReferenceCount() = %d \n",
       //      X_SerialArray->getReferenceCount());

       // Must delete the X_SerialArray if it was taken directly from the X array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (X_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          X_SerialArray->decrementReferenceCount();
          if (X_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete X_SerialArray;
             }
          X_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & asinh(intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Unary_Operator ( X ,
                   MDI_i_Arc_Sinh_Array ,
                   MDI_i_Arc_Sinh_Array_Accumulate_To_Operand , intSerialArray::asinh_Function );
#endif
   } 

#endif

intSerialArray &
intSerialArray::operator++ ()
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside intSerialArray::operator++ () for intSerialArray class!");

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in intSerialArray::operator++ -- Prefix operator");
#endif

     (*this) += 1;
     return *this;
   }
 
intSerialArray &
intSerialArray::operator++ ( int x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside intSerialArray::operator%s (int=%d) for intSerialArray class! \n","++",x);

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in intSerialArray::operator++(x=0) -- Postfix operator");
#endif

  // Postfix operator always passes zero as argument (strange but true -- See Stroustrup p594)
     APP_ASSERT( x == 0 );
     (*this) += 1;
     return *this;
   }


#ifndef INTARRAY
intSerialArray &
atanh ( const intSerialArray & X )
   { 
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of atanh for intSerialArray class! \n");

  // This is the only test we can do on the input!
     X.Test_Consistency ("Test X in atanh ( const intSerialArray & X )");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in intSerialArray & atanh(intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *X_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( X, X_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( X, X_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set =	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(X_SerialArray       != NULL);

     bool xIsTemporary = X_SerialArray->isTemporary();

     APP_ASSERT ( (xIsTemporary == TRUE) || (xIsTemporary == FALSE) );

  // return intArray::Abstract_Operator ( Temporary_Array_Set, X, atanh(*X_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = X;
#else
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, X, X_SerialArray, atanh(*X_SerialArray) );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (xIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): X_SerialArray->getReferenceCount() = %d \n",
       //      X_SerialArray->getReferenceCount());

       // Must delete the X_SerialArray if it was taken directly from the X array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (X_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          X_SerialArray->decrementReferenceCount();
          if (X_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete X_SerialArray;
             }
          X_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & atanh(intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Unary_Operator ( X ,
                   MDI_i_Arc_Tanh_Array ,
                   MDI_i_Arc_Tanh_Array_Accumulate_To_Operand , intSerialArray::atanh_Function );
#endif
   } 

#endif

#ifndef INTARRAY
intSerialArray &
log ( const intSerialArray & X )
   { 
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of log for intSerialArray class! \n");

  // This is the only test we can do on the input!
     X.Test_Consistency ("Test X in log ( const intSerialArray & X )");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in intSerialArray & log(intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *X_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( X, X_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( X, X_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set =	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(X_SerialArray       != NULL);

     bool xIsTemporary = X_SerialArray->isTemporary();

     APP_ASSERT ( (xIsTemporary == TRUE) || (xIsTemporary == FALSE) );

  // return intArray::Abstract_Operator ( Temporary_Array_Set, X, log(*X_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = X;
#else
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, X, X_SerialArray, log(*X_SerialArray) );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (xIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): X_SerialArray->getReferenceCount() = %d \n",
       //      X_SerialArray->getReferenceCount());

       // Must delete the X_SerialArray if it was taken directly from the X array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (X_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          X_SerialArray->decrementReferenceCount();
          if (X_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete X_SerialArray;
             }
          X_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & log(intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Unary_Operator ( X ,
                   MDI_i_Log_Array ,
                   MDI_i_Log_Array_Accumulate_To_Operand , intSerialArray::log_Function );
#endif
   } 

#endif

#ifndef INTARRAY
intSerialArray &
log10 ( const intSerialArray & X )
   { 
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of log10 for intSerialArray class! \n");

  // This is the only test we can do on the input!
     X.Test_Consistency ("Test X in log10 ( const intSerialArray & X )");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in intSerialArray & log10(intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *X_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( X, X_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( X, X_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set =	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(X_SerialArray       != NULL);

     bool xIsTemporary = X_SerialArray->isTemporary();

     APP_ASSERT ( (xIsTemporary == TRUE) || (xIsTemporary == FALSE) );

  // return intArray::Abstract_Operator ( Temporary_Array_Set, X, log10(*X_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = X;
#else
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, X, X_SerialArray, log10(*X_SerialArray) );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (xIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): X_SerialArray->getReferenceCount() = %d \n",
       //      X_SerialArray->getReferenceCount());

       // Must delete the X_SerialArray if it was taken directly from the X array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (X_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          X_SerialArray->decrementReferenceCount();
          if (X_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete X_SerialArray;
             }
          X_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & log10(intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Unary_Operator ( X ,
                   MDI_i_Log10_Array ,
                   MDI_i_Log10_Array_Accumulate_To_Operand , intSerialArray::log10_Function );
#endif
   } 

#endif

#ifndef INTARRAY
intSerialArray &
exp ( const intSerialArray & X )
   { 
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of exp for intSerialArray class! \n");

  // This is the only test we can do on the input!
     X.Test_Consistency ("Test X in exp ( const intSerialArray & X )");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in intSerialArray & exp(intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *X_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( X, X_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( X, X_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set =	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(X_SerialArray       != NULL);

     bool xIsTemporary = X_SerialArray->isTemporary();

     APP_ASSERT ( (xIsTemporary == TRUE) || (xIsTemporary == FALSE) );

  // return intArray::Abstract_Operator ( Temporary_Array_Set, X, exp(*X_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = X;
#else
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, X, X_SerialArray, exp(*X_SerialArray) );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (xIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): X_SerialArray->getReferenceCount() = %d \n",
       //      X_SerialArray->getReferenceCount());

       // Must delete the X_SerialArray if it was taken directly from the X array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (X_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          X_SerialArray->decrementReferenceCount();
          if (X_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete X_SerialArray;
             }
          X_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & exp(intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Unary_Operator ( X ,
                   MDI_i_Exp_Array ,
                   MDI_i_Exp_Array_Accumulate_To_Operand , intSerialArray::exp_Function );
#endif
   } 

#endif

#ifndef INTARRAY
intSerialArray &
sqrt ( const intSerialArray & X )
   { 
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of sqrt for intSerialArray class! \n");

  // This is the only test we can do on the input!
     X.Test_Consistency ("Test X in sqrt ( const intSerialArray & X )");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in intSerialArray & sqrt(intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *X_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( X, X_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( X, X_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set =	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(X_SerialArray       != NULL);

     bool xIsTemporary = X_SerialArray->isTemporary();

     APP_ASSERT ( (xIsTemporary == TRUE) || (xIsTemporary == FALSE) );

  // return intArray::Abstract_Operator ( Temporary_Array_Set, X, sqrt(*X_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = X;
#else
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, X, X_SerialArray, sqrt(*X_SerialArray) );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (xIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): X_SerialArray->getReferenceCount() = %d \n",
       //      X_SerialArray->getReferenceCount());

       // Must delete the X_SerialArray if it was taken directly from the X array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (X_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          X_SerialArray->decrementReferenceCount();
          if (X_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete X_SerialArray;
             }
          X_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & sqrt(intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Unary_Operator ( X ,
                   MDI_i_Sqrt_Array ,
                   MDI_i_Sqrt_Array_Accumulate_To_Operand , intSerialArray::sqrt_Function );
#endif
   } 

#endif

#ifndef INTARRAY
intSerialArray &
fabs ( const intSerialArray & X )
   { 
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of fabs for intSerialArray class! \n");

  // This is the only test we can do on the input!
     X.Test_Consistency ("Test X in fabs ( const intSerialArray & X )");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in intSerialArray & fabs(intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *X_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( X, X_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( X, X_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set =	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(X_SerialArray       != NULL);

     bool xIsTemporary = X_SerialArray->isTemporary();

     APP_ASSERT ( (xIsTemporary == TRUE) || (xIsTemporary == FALSE) );

  // return intArray::Abstract_Operator ( Temporary_Array_Set, X, fabs(*X_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = X;
#else
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, X, X_SerialArray, fabs(*X_SerialArray) );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (xIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): X_SerialArray->getReferenceCount() = %d \n",
       //      X_SerialArray->getReferenceCount());

       // Must delete the X_SerialArray if it was taken directly from the X array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (X_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          X_SerialArray->decrementReferenceCount();
          if (X_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete X_SerialArray;
             }
          X_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & fabs(intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Unary_Operator ( X ,
                   MDI_i_Fabs_Array ,
                   MDI_i_Fabs_Array_Accumulate_To_Operand , intSerialArray::fabs_Function );
#endif
   } 

intSerialArray &
abs ( const intSerialArray & X )
   { 
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of abs for intSerialArray class! \n");

  // This is the only test we can do on the input!
     X.Test_Consistency ("Test X in abs ( const intSerialArray & X )");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in intSerialArray & abs(intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *X_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( X, X_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( X, X_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set =	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(X_SerialArray       != NULL);

     bool xIsTemporary = X_SerialArray->isTemporary();

     APP_ASSERT ( (xIsTemporary == TRUE) || (xIsTemporary == FALSE) );

  // return intArray::Abstract_Operator ( Temporary_Array_Set, X, abs(*X_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = X;
#else
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, X, X_SerialArray, abs(*X_SerialArray) );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (xIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): X_SerialArray->getReferenceCount() = %d \n",
       //      X_SerialArray->getReferenceCount());

       // Must delete the X_SerialArray if it was taken directly from the X array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (X_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          X_SerialArray->decrementReferenceCount();
          if (X_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete X_SerialArray;
             }
          X_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & abs(intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Unary_Operator ( X ,
                   MDI_i_Fabs_Array ,
                   MDI_i_Fabs_Array_Accumulate_To_Operand , intSerialArray::abs_Function );
#endif
   } 

#else
intSerialArray &
abs ( const intSerialArray & X )
   { 
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of abs for intSerialArray class! \n");

  // This is the only test we can do on the input!
     X.Test_Consistency ("Test X in abs ( const intSerialArray & X )");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in intSerialArray & abs(intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *X_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( X, X_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( X, X_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set =	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(X_SerialArray       != NULL);

     bool xIsTemporary = X_SerialArray->isTemporary();

     APP_ASSERT ( (xIsTemporary == TRUE) || (xIsTemporary == FALSE) );

  // return intArray::Abstract_Operator ( Temporary_Array_Set, X, abs(*X_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = X;
#else
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, X, X_SerialArray, abs(*X_SerialArray) );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (xIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): X_SerialArray->getReferenceCount() = %d \n",
       //      X_SerialArray->getReferenceCount());

       // Must delete the X_SerialArray if it was taken directly from the X array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (X_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          X_SerialArray->decrementReferenceCount();
          if (X_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete X_SerialArray;
             }
          X_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & abs(intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Unary_Operator ( X ,
                   MDI_i_Abs_Array ,
                   MDI_i_Abs_Array_Accumulate_To_Operand , intSerialArray::abs_Function );
#endif
   } 

#endif

#ifndef INTARRAY
intSerialArray &
ceil ( const intSerialArray & X )
   { 
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of ceil for intSerialArray class! \n");

  // This is the only test we can do on the input!
     X.Test_Consistency ("Test X in ceil ( const intSerialArray & X )");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in intSerialArray & ceil(intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *X_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( X, X_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( X, X_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set =	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(X_SerialArray       != NULL);

     bool xIsTemporary = X_SerialArray->isTemporary();

     APP_ASSERT ( (xIsTemporary == TRUE) || (xIsTemporary == FALSE) );

  // return intArray::Abstract_Operator ( Temporary_Array_Set, X, ceil(*X_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = X;
#else
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, X, X_SerialArray, ceil(*X_SerialArray) );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (xIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): X_SerialArray->getReferenceCount() = %d \n",
       //      X_SerialArray->getReferenceCount());

       // Must delete the X_SerialArray if it was taken directly from the X array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (X_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          X_SerialArray->decrementReferenceCount();
          if (X_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete X_SerialArray;
             }
          X_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & ceil(intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Unary_Operator ( X ,
                   MDI_i_Ceil_Array ,
                   MDI_i_Ceil_Array_Accumulate_To_Operand , intSerialArray::ceil_Function );
#endif
   } 

#endif

#ifndef INTARRAY
intSerialArray &
floor ( const intSerialArray & X )
   { 
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of floor for intSerialArray class! \n");

  // This is the only test we can do on the input!
     X.Test_Consistency ("Test X in floor ( const intSerialArray & X )");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in intSerialArray & floor(intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *X_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( X, X_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( X, X_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set =	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(X_SerialArray       != NULL);

     bool xIsTemporary = X_SerialArray->isTemporary();

     APP_ASSERT ( (xIsTemporary == TRUE) || (xIsTemporary == FALSE) );

  // return intArray::Abstract_Operator ( Temporary_Array_Set, X, floor(*X_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = X;
#else
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, X, X_SerialArray, floor(*X_SerialArray) );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (xIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): X_SerialArray->getReferenceCount() = %d \n",
       //      X_SerialArray->getReferenceCount());

       // Must delete the X_SerialArray if it was taken directly from the X array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (X_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          X_SerialArray->decrementReferenceCount();
          if (X_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete X_SerialArray;
             }
          X_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & floor(intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Unary_Operator ( X ,
                   MDI_i_Floor_Array ,
                   MDI_i_Floor_Array_Accumulate_To_Operand , intSerialArray::floor_Function );
#endif
   } 

#endif

#ifndef INTARRAY
intSerialArray &
intSerialArray::replace ( const intSerialArray & Lhs , const intSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of intSerialArray::replace (intSerialArray,intSerialArray) for intSerialArray class!");

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in intSerialArray::replace (intSerialArray,intSerialArray)");
     Lhs.Test_Consistency ("Test Lhs in intSerialArray::replace (intSerialArray,intSerialArray)");
     Rhs.Test_Consistency ("Test Rhs in intSerialArray::replace (intSerialArray,intSerialArray)");
#endif

  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Lhs.Test_Conformability (Rhs);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in intSerialArray & intSerialArray::replace(intSerialArray,intSerialArray)");
          Rhs.displayReferenceCounts("Rhs in intSerialArray & intSerialArray::replace(intSerialArray,intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray  *This_SerialArray = NULL;
     intSerialArray *Lhs_SerialArray = NULL;
     intSerialArray  *Rhs_SerialArray = NULL;
     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	 (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
     {
	puts ("ERROR: can't mix indirect addressing with 2 arrays and where.");
        APP_ABORT();
     }
     else
     {
        Temporary_Array_Set = 
            intSerialArray::Parallel_Conformability_Enforcement 
	       ( *this, This_SerialArray, Lhs, Lhs_SerialArray, 
	          Rhs  , Rhs_SerialArray );
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set =	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(This_SerialArray    != NULL);
     APP_ASSERT(Lhs_SerialArray     != NULL);
     APP_ASSERT(Rhs_SerialArray     != NULL);

     bool thisIsTemporary = This_SerialArray->isTemporary();
     bool lhsIsTemporary  = Lhs_SerialArray->isTemporary();
     bool rhsIsTemporary  = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (thisIsTemporary == TRUE) || (thisIsTemporary == FALSE) );
     APP_ASSERT ( (lhsIsTemporary == TRUE)  || (lhsIsTemporary == FALSE) );
     APP_ASSERT ( (rhsIsTemporary == TRUE)  || (rhsIsTemporary == FALSE) );

#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = Rhs;
#else
     intArray & Return_Value = intArray::Abstract_Modification_Operator ( Temporary_Array_Set, *this, Lhs, Rhs,
               This_SerialArray, Lhs_SerialArray, Rhs_SerialArray,
               This_SerialArray->replace (*Lhs_SerialArray, *Rhs_SerialArray) );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
  // Since the Lhs is a different type than the Return Value it could not be reused!
     // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }
     if (thisIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): This_SerialArray->getReferenceCount() = %d \n",
       //      This_SerialArray->getReferenceCount());

       // Must delete the This_SerialArray if it was taken directly from the This array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (This_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          This_SerialArray->decrementReferenceCount();
          if (This_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete This_SerialArray;
             }
          This_SerialArray = NULL;

        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & intSerialArray::replace(intSerialArray,intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Modification_Operator ( Lhs , Rhs , MDI_i_If_Array_Use_Array , intSerialArray::replace_Function );
#endif
   }

#endif

#ifndef INTARRAY
intSerialArray &
intSerialArray::replace ( const intSerialArray & Lhs , int x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of intSerialArray::replace (intSerialArray,x) for intSerialArray class! \n");

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in intSerialArray::replace (intSerialArray,int)");
     Lhs.Test_Consistency ("Test Lhs in intSerialArray::replace (intSerialArray,int)");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in intSerialArray & intSerialArray::replace(intSerialArray,int)");
        }
#endif

#if defined(PPP)
     intSerialArray  *This_SerialArray = NULL;
     intSerialArray *Lhs_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if ((Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	 (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
     {
        Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement ( *this, This_SerialArray, Lhs, Lhs_SerialArray );
     }
     else
     {
        Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement ( *this, This_SerialArray, Lhs, Lhs_SerialArray );
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set =	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(This_SerialArray    != NULL);
     APP_ASSERT(Lhs_SerialArray     != NULL);

     bool thisIsTemporary = This_SerialArray->isTemporary();
     bool lhsIsTemporary  = Lhs_SerialArray->isTemporary();

     APP_ASSERT ( (thisIsTemporary == TRUE) || (thisIsTemporary == FALSE) );
     APP_ASSERT ( (lhsIsTemporary  == TRUE) || (lhsIsTemporary  == FALSE) );

#if defined(MEMORY_LEAK_TEST)
     printf ("Can't do MEMORY_LEAK_TEST in intSerialArray & intSerialArray::replace ( const intSerialArray & Lhs , int x )");
#endif
     intArray & Return_Value = intArray::Abstract_Modification_Operator ( Temporary_Array_Set, *this, Lhs,
               This_SerialArray, Lhs_SerialArray, This_SerialArray->replace (*Lhs_SerialArray, x) );

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (thisIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): This_SerialArray->getReferenceCount() = %d \n",
       //      This_SerialArray->getReferenceCount());

       // Must delete the This_SerialArray if it was taken directly from the This array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (This_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          This_SerialArray->decrementReferenceCount();
          if (This_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete This_SerialArray;
             }
          This_SerialArray = NULL;

        }

  // Since the Lhs is a different type than the Return Value it could not be reused!
     // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;


     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & intSerialArray::replace(intSerialArray,int)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Modification_Operator ( Lhs , x , MDI_i_If_Array_Use_Scalar , intSerialArray::Scalar_replace_Function );
#endif
   }

#endif

intSerialArray &
intSerialArray::operator+= ( const intSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of intSerialArray::operator+= (intSerialArray) for intSerialArray class!");

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in intSerialArray::operator+=");
     Rhs.Test_Consistency ("Test Rhs in intSerialArray::operator+=");
#endif

  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Test_Conformability (Rhs);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS in intSerialArray & intSerialArray::operator+=(intSerialArray)");
          Rhs.displayReferenceCounts("Rhs in intSerialArray & intSerialArray::operator+=(intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *This_SerialArray = NULL;
     intSerialArray *Rhs_SerialArray  = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if ((Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	    (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      ( *this , This_SerialArray, Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      ( *this , This_SerialArray, Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if ((Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	    (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
        {
	   puts ("ERROR: can't mix indirect addressing with 2 arrays and where.");
           APP_ABORT();
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (*this, This_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray,
	       Rhs, Rhs_SerialArray );
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set =	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(This_SerialArray != NULL);
     APP_ASSERT(Rhs_SerialArray  != NULL);

     bool thisIsTemporary = This_SerialArray->isTemporary();
     bool rhsIsTemporary  = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (thisIsTemporary == TRUE) || (thisIsTemporary == FALSE) );
     APP_ASSERT ( (rhsIsTemporary  == TRUE) || (rhsIsTemporary  == FALSE) );

#if !defined(MEMORY_LEAK_TEST)
     intArray::Abstract_Modification_Operator ( Temporary_Array_Set, *this, Rhs, This_SerialArray, Rhs_SerialArray, *This_SerialArray += *Rhs_SerialArray );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (thisIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): This_SerialArray->getReferenceCount() = %d \n",
       //      This_SerialArray->getReferenceCount());

       // Must delete the This_SerialArray if it was taken directly from the This array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (This_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          This_SerialArray->decrementReferenceCount();
          if (This_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete This_SerialArray;
             }
          This_SerialArray = NULL;

        }
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }

     // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#else
     intSerialArray::Abstract_Operator_Operation_Equals ( *this , Rhs ,
               MDI_i_Add_Array_Plus_Array_Accumulate_To_Operand , intSerialArray::Plus_Equals );
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS (return value) in intSerialArray & intSerialArray::operator+=(intSerialArray)");
        }
#endif

     return *this;
   }


#ifndef INTARRAY
intSerialArray &
intSerialArray::replace ( int x , const intSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of intSerialArray::replace (x,intSerialArray) for intSerialArray class! \n");

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in intSerialArray::replace (int,intSerialArray)");
     Rhs.Test_Consistency ("Test Rhs in intSerialArray::replace (int,intSerialArray)");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs.displayReferenceCounts("Rhs in intSerialArray & intSerialArray::replace(int,intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *This_SerialArray = NULL;
     intSerialArray *Rhs_SerialArray  = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if ((Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	    (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
        {
           Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement( *this, This_SerialArray, Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement ( *this, This_SerialArray, Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if ((Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	    (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
        {
	   puts ("ERROR: can't mix indirect addressing with 2 arrays and where.");
           APP_ABORT();
        }
	else
	{
           Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement 
	      (*this, This_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray,
	       Rhs, Rhs_SerialArray );
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = 
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set =	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(This_SerialArray    != NULL);
     APP_ASSERT(Rhs_SerialArray     != NULL);

     bool thisIsTemporary = This_SerialArray->isTemporary();
     bool rhsIsTemporary  = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (thisIsTemporary == TRUE) || (thisIsTemporary == FALSE) );
     APP_ASSERT ( (rhsIsTemporary  == TRUE) || (rhsIsTemporary  == FALSE) );

#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = Rhs;
#else
     intArray & Return_Value = intArray::Abstract_Modification_Operator ( Temporary_Array_Set, *this, Rhs,
               This_SerialArray, Rhs_SerialArray,
               This_SerialArray->replace (x, *Rhs_SerialArray) );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (thisIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): This_SerialArray->getReferenceCount() = %d \n",
       //      This_SerialArray->getReferenceCount());

       // Must delete the This_SerialArray if it was taken directly from the This array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (This_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          This_SerialArray->decrementReferenceCount();
          if (This_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete This_SerialArray;
             }
          This_SerialArray = NULL;

        }
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & intSerialArray::replace(int,intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Modification_Operator ( Rhs , x , MDI_i_If_Scalar_Use_Array , Scalar_replace_Function );
#endif
   }

#endif

#ifndef INTARRAY
intSerialArray &
fmod ( const intSerialArray & Lhs , const intSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of fmod (intSerialArray,intSerialArray) for intSerialArray class! Lhs:rc=%d Rhs:rc=%d ",
               Lhs.getRawDataReferenceCount(),Rhs.getRawDataReferenceCount());

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in fmod");
     Rhs.Test_Consistency ("Test Rhs in fmod");
#endif
 
  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Lhs.Test_Conformability (Rhs);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in intSerialArray & fmod(intSerialArray,intSerialArray)");
          Rhs.displayReferenceCounts("Rhs in intSerialArray & fmod(intSerialArray,intSerialArray)");
        }
#endif

#if defined(PPP)
  // Pointers to views of Lhs and Rhs serial arrays which allow a conformable operation
     intSerialArray *Lhs_SerialArray = NULL;
     intSerialArray *Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...
     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	      (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
               Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement (Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement (Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	      (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
	       puts ("ERROR: can't mix indirect addressing with 2 arrays and where.");
               APP_ABORT();
             }
            else
             {
               Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
                    (Lhs, Lhs_SerialArray, 
                    *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray,
                     Rhs, Rhs_SerialArray );
             }

          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

  // New test (8/5/2000)
     APP_ASSERT(Temporary_Array_Set != NULL);
  // Temporary_Array_Set->display("Check to see what sort of communication model was used");

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Lhs_SerialArray != NULL);
     APP_ASSERT(Rhs_SerialArray != NULL);

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();
     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );
     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in intSerialArray & fmod(intSerialArray,intSerialArray)");
          Rhs_SerialArray->displayReferenceCounts("Rhs_SerialArray in intSerialArray & fmod(intSerialArray,intSerialArray)");
        }
#endif

  // Inputs to intArray::Abstract_Binary_Operator:
  //     1. Temporary_Array_Set is attached to the intArray temporary returned by Abstract_Binary_Operator
  //     2. Lhs is used to get the Lhs partition information (PARTI parallel descriptor) and array reuse
  //     3. Rhs is used to get the Rhs partition information (PARTI parallel descriptor) in case the Lhs was 
  //        a NULL array (no data and no defined partitioning (i.e. no PARTI parallel descriptor)) and array reuse
  //     4. The intSerialArray which is to be put into the intArray temporary returned by Abstract_Binary_Operator
  // return intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, fmod(*Lhs_SerialArray,*Rhs_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = Rhs;
#else
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, Lhs_SerialArray, Rhs_SerialArray, fmod(*Lhs_SerialArray,*Rhs_SerialArray) );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
  // if (Lhs_SerialArray != Return_Value.getSerialArrayPointer())
     if (lhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

        }
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & fmod(intSerialArray,intSerialArray)");
        }
#endif

     return Return_Value;
#else
  // This function can be either Abstract_Binary_Operator or Abstract_Binary_Operator_Non_Commutative
     return intSerialArray::Abstract_Binary_Operator_Non_Commutative ( Lhs , Rhs , MDI_i_Fmod_Array_Modulo_Array, MDI_i_Fmod_Array_Modulo_Array_Accumulate_To_Operand , intSerialArray::fmod_Function );
#endif
   }



intSerialArray &
mod ( const intSerialArray & Lhs , const intSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of mod (intSerialArray,intSerialArray) for intSerialArray class! Lhs:rc=%d Rhs:rc=%d ",
               Lhs.getRawDataReferenceCount(),Rhs.getRawDataReferenceCount());

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in mod");
     Rhs.Test_Consistency ("Test Rhs in mod");
#endif
 
  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Lhs.Test_Conformability (Rhs);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in intSerialArray & mod(intSerialArray,intSerialArray)");
          Rhs.displayReferenceCounts("Rhs in intSerialArray & mod(intSerialArray,intSerialArray)");
        }
#endif

#if defined(PPP)
  // Pointers to views of Lhs and Rhs serial arrays which allow a conformable operation
     intSerialArray *Lhs_SerialArray = NULL;
     intSerialArray *Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...
     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	      (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
               Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement (Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement (Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	      (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
	       puts ("ERROR: can't mix indirect addressing with 2 arrays and where.");
               APP_ABORT();
             }
            else
             {
               Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
                    (Lhs, Lhs_SerialArray, 
                    *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray,
                     Rhs, Rhs_SerialArray );
             }

          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

  // New test (8/5/2000)
     APP_ASSERT(Temporary_Array_Set != NULL);
  // Temporary_Array_Set->display("Check to see what sort of communication model was used");

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Lhs_SerialArray != NULL);
     APP_ASSERT(Rhs_SerialArray != NULL);

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();
     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );
     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in intSerialArray & mod(intSerialArray,intSerialArray)");
          Rhs_SerialArray->displayReferenceCounts("Rhs_SerialArray in intSerialArray & mod(intSerialArray,intSerialArray)");
        }
#endif

  // Inputs to intArray::Abstract_Binary_Operator:
  //     1. Temporary_Array_Set is attached to the intArray temporary returned by Abstract_Binary_Operator
  //     2. Lhs is used to get the Lhs partition information (PARTI parallel descriptor) and array reuse
  //     3. Rhs is used to get the Rhs partition information (PARTI parallel descriptor) in case the Lhs was 
  //        a NULL array (no data and no defined partitioning (i.e. no PARTI parallel descriptor)) and array reuse
  //     4. The intSerialArray which is to be put into the intArray temporary returned by Abstract_Binary_Operator
  // return intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, mod(*Lhs_SerialArray,*Rhs_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = Rhs;
#else
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, Lhs_SerialArray, Rhs_SerialArray, mod(*Lhs_SerialArray,*Rhs_SerialArray) );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
  // if (Lhs_SerialArray != Return_Value.getSerialArrayPointer())
     if (lhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

        }
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & mod(intSerialArray,intSerialArray)");
        }
#endif

     return Return_Value;
#else
  // This function can be either Abstract_Binary_Operator or Abstract_Binary_Operator_Non_Commutative
     return intSerialArray::Abstract_Binary_Operator_Non_Commutative ( Lhs , Rhs , MDI_i_Fmod_Array_Modulo_Array, MDI_i_Fmod_Array_Modulo_Array_Accumulate_To_Operand , intSerialArray::mod_Function );
#endif
   }



#else
intSerialArray &
mod ( const intSerialArray & Lhs , const intSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of mod (intSerialArray,intSerialArray) for intSerialArray class! Lhs:rc=%d Rhs:rc=%d ",
               Lhs.getRawDataReferenceCount(),Rhs.getRawDataReferenceCount());

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in mod");
     Rhs.Test_Consistency ("Test Rhs in mod");
#endif
 
  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Lhs.Test_Conformability (Rhs);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in intSerialArray & mod(intSerialArray,intSerialArray)");
          Rhs.displayReferenceCounts("Rhs in intSerialArray & mod(intSerialArray,intSerialArray)");
        }
#endif

#if defined(PPP)
  // Pointers to views of Lhs and Rhs serial arrays which allow a conformable operation
     intSerialArray *Lhs_SerialArray = NULL;
     intSerialArray *Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...
     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	      (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
               Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement (Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement (Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	      (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
	       puts ("ERROR: can't mix indirect addressing with 2 arrays and where.");
               APP_ABORT();
             }
            else
             {
               Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
                    (Lhs, Lhs_SerialArray, 
                    *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray,
                     Rhs, Rhs_SerialArray );
             }

          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

  // New test (8/5/2000)
     APP_ASSERT(Temporary_Array_Set != NULL);
  // Temporary_Array_Set->display("Check to see what sort of communication model was used");

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Lhs_SerialArray != NULL);
     APP_ASSERT(Rhs_SerialArray != NULL);

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();
     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );
     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in intSerialArray & mod(intSerialArray,intSerialArray)");
          Rhs_SerialArray->displayReferenceCounts("Rhs_SerialArray in intSerialArray & mod(intSerialArray,intSerialArray)");
        }
#endif

  // Inputs to intArray::Abstract_Binary_Operator:
  //     1. Temporary_Array_Set is attached to the intArray temporary returned by Abstract_Binary_Operator
  //     2. Lhs is used to get the Lhs partition information (PARTI parallel descriptor) and array reuse
  //     3. Rhs is used to get the Rhs partition information (PARTI parallel descriptor) in case the Lhs was 
  //        a NULL array (no data and no defined partitioning (i.e. no PARTI parallel descriptor)) and array reuse
  //     4. The intSerialArray which is to be put into the intArray temporary returned by Abstract_Binary_Operator
  // return intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, mod(*Lhs_SerialArray,*Rhs_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = Rhs;
#else
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, Lhs_SerialArray, Rhs_SerialArray, mod(*Lhs_SerialArray,*Rhs_SerialArray) );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
  // if (Lhs_SerialArray != Return_Value.getSerialArrayPointer())
     if (lhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

        }
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & mod(intSerialArray,intSerialArray)");
        }
#endif

     return Return_Value;
#else
  // This function can be either Abstract_Binary_Operator or Abstract_Binary_Operator_Non_Commutative
     return intSerialArray::Abstract_Binary_Operator_Non_Commutative ( Lhs , Rhs , MDI_i_Fmod_Array_Modulo_Array, MDI_i_Fmod_Array_Modulo_Array_Accumulate_To_Operand , intSerialArray::mod_Function );
#endif
   }



#endif

#ifndef INTARRAY
intSerialArray &
fmod ( int x , const intSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("\n\n\nInside of fmod (int,intSerialArray) for intSerialArray class! \n");

  // This is the only test we can do on the input!
     Rhs.Test_Consistency ("Test Rhs in fmod");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs.displayReferenceCounts("Rhs in intSerialArray & fmod(int,intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...
     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement
              (Rhs, Rhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}

        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Rhs_SerialArray     != NULL);

     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs_SerialArray->displayReferenceCounts("Rhs_SerialArray in intSerialArray & fmod(int,intSerialArray)");
        }
#endif

  // return intArray::Abstract_Operator ( Temporary_Array_Set, Rhs, fmod(x,*Rhs_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = Rhs;
#else
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Rhs, Rhs_SerialArray, fmod(x,*Rhs_SerialArray) );
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value (before delete macro) in intSerialArray & fmod(int,intSerialArray)");
        }
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & fmod(int,intSerialArray)");
        }
#endif

     return Return_Value;
#else
  // This function can be either Abstract_Binary_Operator or Abstract_Binary_Operator_Non_Commutative
     return intSerialArray::Abstract_Binary_Operator_Non_Commutative ( Rhs , x ,
               MDI_i_Fmod_Scalar_Modulo_Array,
               MDI_i_Fmod_Scalar_Modulo_Array_Accumulate_To_Operand , intSerialArray::Scalar_fmod_Function );
#endif
   }



intSerialArray &
mod ( int x , const intSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("\n\n\nInside of mod (int,intSerialArray) for intSerialArray class! \n");

  // This is the only test we can do on the input!
     Rhs.Test_Consistency ("Test Rhs in mod");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs.displayReferenceCounts("Rhs in intSerialArray & mod(int,intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...
     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement
              (Rhs, Rhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}

        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Rhs_SerialArray     != NULL);

     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs_SerialArray->displayReferenceCounts("Rhs_SerialArray in intSerialArray & mod(int,intSerialArray)");
        }
#endif

  // return intArray::Abstract_Operator ( Temporary_Array_Set, Rhs, mod(x,*Rhs_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = Rhs;
#else
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Rhs, Rhs_SerialArray, mod(x,*Rhs_SerialArray) );
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value (before delete macro) in intSerialArray & mod(int,intSerialArray)");
        }
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & mod(int,intSerialArray)");
        }
#endif

     return Return_Value;
#else
  // This function can be either Abstract_Binary_Operator or Abstract_Binary_Operator_Non_Commutative
     return intSerialArray::Abstract_Binary_Operator_Non_Commutative ( Rhs , x ,
               MDI_i_Fmod_Scalar_Modulo_Array,
               MDI_i_Fmod_Scalar_Modulo_Array_Accumulate_To_Operand , intSerialArray::Scalar_mod_Function );
#endif
   }



#else
intSerialArray &
mod ( int x , const intSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("\n\n\nInside of mod (int,intSerialArray) for intSerialArray class! \n");

  // This is the only test we can do on the input!
     Rhs.Test_Consistency ("Test Rhs in mod");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs.displayReferenceCounts("Rhs in intSerialArray & mod(int,intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...
     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement
              (Rhs, Rhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}

        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Rhs_SerialArray     != NULL);

     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs_SerialArray->displayReferenceCounts("Rhs_SerialArray in intSerialArray & mod(int,intSerialArray)");
        }
#endif

  // return intArray::Abstract_Operator ( Temporary_Array_Set, Rhs, mod(x,*Rhs_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = Rhs;
#else
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Rhs, Rhs_SerialArray, mod(x,*Rhs_SerialArray) );
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value (before delete macro) in intSerialArray & mod(int,intSerialArray)");
        }
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & mod(int,intSerialArray)");
        }
#endif

     return Return_Value;
#else
  // This function can be either Abstract_Binary_Operator or Abstract_Binary_Operator_Non_Commutative
     return intSerialArray::Abstract_Binary_Operator_Non_Commutative ( Rhs , x ,
               MDI_i_Fmod_Scalar_Modulo_Array,
               MDI_i_Fmod_Scalar_Modulo_Array_Accumulate_To_Operand , intSerialArray::Scalar_mod_Function );
#endif
   }



#endif

#ifndef INTARRAY
intSerialArray &
fmod ( const intSerialArray & Lhs , int x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of fmod (intSerialArray,int) for intSerialArray class!");

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in fmod");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts 
          Lhs.displayReferenceCounts("Lhs in intSerialArray & fmod(intSerialArray,int)");
        }
#endif

#if defined(PPP)
  // Pointers to views of Lhs serial arrays which allow a conformable operation
     intSerialArray *Lhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE)
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }
     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Lhs_SerialArray != NULL);

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in intSerialArray & fmod(intSerialArray,int)");
        }
#endif

  // return intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, fmod(*Lhs_SerialArray,x) );
#if defined(MEMORY_LEAK_TEST)
     puts ("Can't do MEMORY_LEAK_TEST in intSerialArray & fmod ( const intSerialArray & Lhs , int x )");
#endif
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Lhs_SerialArray, fmod(*Lhs_SerialArray,x) );

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (lhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray;
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & fmod(intSerialArray,int)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator ( Lhs , x ,
               MDI_i_Fmod_Array_Modulo_Scalar,
               MDI_i_Fmod_Array_Modulo_Scalar_Accumulate_To_Operand , intSerialArray::Scalar_fmod_Function );
#endif
   }

intSerialArray &
mod ( const intSerialArray & Lhs , int x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of mod (intSerialArray,int) for intSerialArray class!");

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in mod");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts 
          Lhs.displayReferenceCounts("Lhs in intSerialArray & mod(intSerialArray,int)");
        }
#endif

#if defined(PPP)
  // Pointers to views of Lhs serial arrays which allow a conformable operation
     intSerialArray *Lhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE)
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }
     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Lhs_SerialArray != NULL);

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in intSerialArray & mod(intSerialArray,int)");
        }
#endif

  // return intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, mod(*Lhs_SerialArray,x) );
#if defined(MEMORY_LEAK_TEST)
     puts ("Can't do MEMORY_LEAK_TEST in intSerialArray & mod ( const intSerialArray & Lhs , int x )");
#endif
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Lhs_SerialArray, mod(*Lhs_SerialArray,x) );

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (lhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray;
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & mod(intSerialArray,int)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator ( Lhs , x ,
               MDI_i_Fmod_Array_Modulo_Scalar,
               MDI_i_Fmod_Array_Modulo_Scalar_Accumulate_To_Operand , intSerialArray::Scalar_mod_Function );
#endif
   }

#else
intSerialArray &
mod ( const intSerialArray & Lhs , int x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of mod (intSerialArray,int) for intSerialArray class!");

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in mod");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts 
          Lhs.displayReferenceCounts("Lhs in intSerialArray & mod(intSerialArray,int)");
        }
#endif

#if defined(PPP)
  // Pointers to views of Lhs serial arrays which allow a conformable operation
     intSerialArray *Lhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE)
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }
     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Lhs_SerialArray != NULL);

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in intSerialArray & mod(intSerialArray,int)");
        }
#endif

  // return intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, mod(*Lhs_SerialArray,x) );
#if defined(MEMORY_LEAK_TEST)
     puts ("Can't do MEMORY_LEAK_TEST in intSerialArray & mod ( const intSerialArray & Lhs , int x )");
#endif
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Lhs_SerialArray, mod(*Lhs_SerialArray,x) );

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (lhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray;
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & mod(intSerialArray,int)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator ( Lhs , x ,
               MDI_i_Fmod_Array_Modulo_Scalar,
               MDI_i_Fmod_Array_Modulo_Scalar_Accumulate_To_Operand , intSerialArray::Scalar_mod_Function );
#endif
   }

#endif

intSerialArray &
pow ( const intSerialArray & Lhs , const intSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of pow (intSerialArray,intSerialArray) for intSerialArray class! Lhs:rc=%d Rhs:rc=%d ",
               Lhs.getRawDataReferenceCount(),Rhs.getRawDataReferenceCount());

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in pow");
     Rhs.Test_Consistency ("Test Rhs in pow");
#endif
 
  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Lhs.Test_Conformability (Rhs);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in intSerialArray & pow(intSerialArray,intSerialArray)");
          Rhs.displayReferenceCounts("Rhs in intSerialArray & pow(intSerialArray,intSerialArray)");
        }
#endif

#if defined(PPP)
  // Pointers to views of Lhs and Rhs serial arrays which allow a conformable operation
     intSerialArray *Lhs_SerialArray = NULL;
     intSerialArray *Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...
     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	      (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
               Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement (Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement (Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	      (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
	       puts ("ERROR: can't mix indirect addressing with 2 arrays and where.");
               APP_ABORT();
             }
            else
             {
               Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
                    (Lhs, Lhs_SerialArray, 
                    *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray,
                     Rhs, Rhs_SerialArray );
             }

          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

  // New test (8/5/2000)
     APP_ASSERT(Temporary_Array_Set != NULL);
  // Temporary_Array_Set->display("Check to see what sort of communication model was used");

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Lhs_SerialArray != NULL);
     APP_ASSERT(Rhs_SerialArray != NULL);

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();
     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );
     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in intSerialArray & pow(intSerialArray,intSerialArray)");
          Rhs_SerialArray->displayReferenceCounts("Rhs_SerialArray in intSerialArray & pow(intSerialArray,intSerialArray)");
        }
#endif

  // Inputs to intArray::Abstract_Binary_Operator:
  //     1. Temporary_Array_Set is attached to the intArray temporary returned by Abstract_Binary_Operator
  //     2. Lhs is used to get the Lhs partition information (PARTI parallel descriptor) and array reuse
  //     3. Rhs is used to get the Rhs partition information (PARTI parallel descriptor) in case the Lhs was 
  //        a NULL array (no data and no defined partitioning (i.e. no PARTI parallel descriptor)) and array reuse
  //     4. The intSerialArray which is to be put into the intArray temporary returned by Abstract_Binary_Operator
  // return intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, pow(*Lhs_SerialArray,*Rhs_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = Rhs;
#else
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, Lhs_SerialArray, Rhs_SerialArray, pow(*Lhs_SerialArray,*Rhs_SerialArray) );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
  // if (Lhs_SerialArray != Return_Value.getSerialArrayPointer())
     if (lhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

        }
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & pow(intSerialArray,intSerialArray)");
        }
#endif

     return Return_Value;
#else
  // This function can be either Abstract_Binary_Operator or Abstract_Binary_Operator_Non_Commutative
     return intSerialArray::Abstract_Binary_Operator_Non_Commutative ( Lhs , Rhs , MDI_i_Pow_Array_Raised_To_Array, MDI_i_Pow_Array_Raised_To_Array_Accumulate_To_Operand , intSerialArray::pow_Function );
#endif
   }




intSerialArray &
pow ( int x , const intSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("\n\n\nInside of pow (int,intSerialArray) for intSerialArray class! \n");

  // This is the only test we can do on the input!
     Rhs.Test_Consistency ("Test Rhs in pow");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs.displayReferenceCounts("Rhs in intSerialArray & pow(int,intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...
     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement
              (Rhs, Rhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}

        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Rhs_SerialArray     != NULL);

     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs_SerialArray->displayReferenceCounts("Rhs_SerialArray in intSerialArray & pow(int,intSerialArray)");
        }
#endif

  // return intArray::Abstract_Operator ( Temporary_Array_Set, Rhs, pow(x,*Rhs_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = Rhs;
#else
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Rhs, Rhs_SerialArray, pow(x,*Rhs_SerialArray) );
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value (before delete macro) in intSerialArray & pow(int,intSerialArray)");
        }
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & pow(int,intSerialArray)");
        }
#endif

     return Return_Value;
#else
  // This function can be either Abstract_Binary_Operator or Abstract_Binary_Operator_Non_Commutative
     return intSerialArray::Abstract_Binary_Operator_Non_Commutative ( Rhs , x ,
               MDI_i_Pow_Scalar_Raised_To_Array,
               MDI_i_Pow_Scalar_Raised_To_Array_Accumulate_To_Operand , intSerialArray::Scalar_pow_Function );
#endif
   }




intSerialArray &
pow ( const intSerialArray & Lhs , int x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of pow (intSerialArray,int) for intSerialArray class!");

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in pow");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts 
          Lhs.displayReferenceCounts("Lhs in intSerialArray & pow(intSerialArray,int)");
        }
#endif

#if defined(PPP)
  // Pointers to views of Lhs serial arrays which allow a conformable operation
     intSerialArray *Lhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE)
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }
     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Lhs_SerialArray != NULL);

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in intSerialArray & pow(intSerialArray,int)");
        }
#endif

  // return intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, pow(*Lhs_SerialArray,x) );
#if defined(MEMORY_LEAK_TEST)
     puts ("Can't do MEMORY_LEAK_TEST in intSerialArray & pow ( const intSerialArray & Lhs , int x )");
#endif
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Lhs_SerialArray, pow(*Lhs_SerialArray,x) );

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (lhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray;
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & pow(intSerialArray,int)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator ( Lhs , x ,
               MDI_i_Pow_Array_Raised_To_Scalar,
               MDI_i_Pow_Array_Raised_To_Scalar_Accumulate_To_Operand , intSerialArray::Scalar_pow_Function );
#endif
   }


intSerialArray &
sign ( const intSerialArray & Lhs , const intSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of sign (intSerialArray,intSerialArray) for intSerialArray class! Lhs:rc=%d Rhs:rc=%d ",
               Lhs.getRawDataReferenceCount(),Rhs.getRawDataReferenceCount());

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in sign");
     Rhs.Test_Consistency ("Test Rhs in sign");
#endif
 
  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Lhs.Test_Conformability (Rhs);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in intSerialArray & sign(intSerialArray,intSerialArray)");
          Rhs.displayReferenceCounts("Rhs in intSerialArray & sign(intSerialArray,intSerialArray)");
        }
#endif

#if defined(PPP)
  // Pointers to views of Lhs and Rhs serial arrays which allow a conformable operation
     intSerialArray *Lhs_SerialArray = NULL;
     intSerialArray *Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...
     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	      (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
               Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement (Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement (Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	      (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
	       puts ("ERROR: can't mix indirect addressing with 2 arrays and where.");
               APP_ABORT();
             }
            else
             {
               Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
                    (Lhs, Lhs_SerialArray, 
                    *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray,
                     Rhs, Rhs_SerialArray );
             }

          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

  // New test (8/5/2000)
     APP_ASSERT(Temporary_Array_Set != NULL);
  // Temporary_Array_Set->display("Check to see what sort of communication model was used");

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Lhs_SerialArray != NULL);
     APP_ASSERT(Rhs_SerialArray != NULL);

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();
     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );
     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in intSerialArray & sign(intSerialArray,intSerialArray)");
          Rhs_SerialArray->displayReferenceCounts("Rhs_SerialArray in intSerialArray & sign(intSerialArray,intSerialArray)");
        }
#endif

  // Inputs to intArray::Abstract_Binary_Operator:
  //     1. Temporary_Array_Set is attached to the intArray temporary returned by Abstract_Binary_Operator
  //     2. Lhs is used to get the Lhs partition information (PARTI parallel descriptor) and array reuse
  //     3. Rhs is used to get the Rhs partition information (PARTI parallel descriptor) in case the Lhs was 
  //        a NULL array (no data and no defined partitioning (i.e. no PARTI parallel descriptor)) and array reuse
  //     4. The intSerialArray which is to be put into the intArray temporary returned by Abstract_Binary_Operator
  // return intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, sign(*Lhs_SerialArray,*Rhs_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = Rhs;
#else
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, Lhs_SerialArray, Rhs_SerialArray, sign(*Lhs_SerialArray,*Rhs_SerialArray) );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
  // if (Lhs_SerialArray != Return_Value.getSerialArrayPointer())
     if (lhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

        }
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & sign(intSerialArray,intSerialArray)");
        }
#endif

     return Return_Value;
#else
  // This function can be either Abstract_Binary_Operator or Abstract_Binary_Operator_Non_Commutative
     return intSerialArray::Abstract_Binary_Operator_Non_Commutative ( Lhs , Rhs , MDI_i_Sign_Array_Of_Array, MDI_i_Sign_Array_Of_Array_Accumulate_To_Operand , intSerialArray::sign_Function );
#endif
   }




intSerialArray &
sign ( int x , const intSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("\n\n\nInside of sign (int,intSerialArray) for intSerialArray class! \n");

  // This is the only test we can do on the input!
     Rhs.Test_Consistency ("Test Rhs in sign");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs.displayReferenceCounts("Rhs in intSerialArray & sign(int,intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...
     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement
              (Rhs, Rhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}

        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Rhs_SerialArray     != NULL);

     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs_SerialArray->displayReferenceCounts("Rhs_SerialArray in intSerialArray & sign(int,intSerialArray)");
        }
#endif

  // return intArray::Abstract_Operator ( Temporary_Array_Set, Rhs, sign(x,*Rhs_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = Rhs;
#else
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Rhs, Rhs_SerialArray, sign(x,*Rhs_SerialArray) );
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value (before delete macro) in intSerialArray & sign(int,intSerialArray)");
        }
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & sign(int,intSerialArray)");
        }
#endif

     return Return_Value;
#else
  // This function can be either Abstract_Binary_Operator or Abstract_Binary_Operator_Non_Commutative
     return intSerialArray::Abstract_Binary_Operator_Non_Commutative ( Rhs , x ,
               MDI_i_Sign_Scalar_Of_Array,
               MDI_i_Sign_Scalar_Of_Array_Accumulate_To_Operand , intSerialArray::Scalar_sign_Function );
#endif
   }




intSerialArray &
sign ( const intSerialArray & Lhs , int x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of sign (intSerialArray,int) for intSerialArray class!");

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in sign");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts 
          Lhs.displayReferenceCounts("Lhs in intSerialArray & sign(intSerialArray,int)");
        }
#endif

#if defined(PPP)
  // Pointers to views of Lhs serial arrays which allow a conformable operation
     intSerialArray *Lhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE)
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }
     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Lhs_SerialArray != NULL);

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in intSerialArray & sign(intSerialArray,int)");
        }
#endif

  // return intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, sign(*Lhs_SerialArray,x) );
#if defined(MEMORY_LEAK_TEST)
     puts ("Can't do MEMORY_LEAK_TEST in intSerialArray & sign ( const intSerialArray & Lhs , int x )");
#endif
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Lhs_SerialArray, sign(*Lhs_SerialArray,x) );

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (lhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray;
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & sign(intSerialArray,int)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator ( Lhs , x ,
               MDI_i_Sign_Array_Of_Scalar,
               MDI_i_Sign_Array_Of_Scalar_Accumulate_To_Operand , intSerialArray::Scalar_sign_Function );
#endif
   }


intSerialArray &
intSerialArray::operator+= ( int x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of intSerialArray::operator+= (int) for intSerialArray class!");

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in intSerialArray::operator+=");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS in intSerialArray & intSerialArray::operator+=(int)");
        }
#endif

#if defined(PPP)
     intSerialArray *This_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( *this , This_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( *this , This_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (*this, This_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (*this, This_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set =	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(This_SerialArray != NULL);

     bool thisIsTemporary = This_SerialArray->isTemporary();

     APP_ASSERT ( (thisIsTemporary == TRUE) || (thisIsTemporary == FALSE) );

#if !defined(MEMORY_LEAK_TEST)
     intArray::Abstract_Modification_Operator ( Temporary_Array_Set, *this, This_SerialArray, *This_SerialArray += x );
     // ... don't need to use macro because Return_Value won't be Mask ...
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (thisIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): This_SerialArray->getReferenceCount() = %d \n",
       //      This_SerialArray->getReferenceCount());

       // Must delete the This_SerialArray if it was taken directly from the This array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (This_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          This_SerialArray->decrementReferenceCount();
          if (This_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete This_SerialArray;
             }
          This_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;

#else
     intSerialArray::Abstract_Operator_Operation_Equals ( *this , x ,
        MDI_i_Add_Array_Plus_Scalar_Accumulate_To_Operand , intSerialArray::Scalar_Plus_Equals );
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS (return value) in intSerialArray & intSerialArray::operator+=(int)");
        }
#endif

     return *this;
   }


// Most C++ compliers support a unary plus operator
intSerialArray &
intSerialArray::operator+ () const
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of unary plus intSerialArray::operator+ () for intSerialArray class! \n");
#endif

  // return *this;
     return (intSerialArray &)(*this);
   }


intSerialArray &
min ( const intSerialArray & Lhs , const intSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of min (intSerialArray,intSerialArray) for intSerialArray class! Lhs:rc=%d Rhs:rc=%d ",
               Lhs.getRawDataReferenceCount(),Rhs.getRawDataReferenceCount());

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in min");
     Rhs.Test_Consistency ("Test Rhs in min");
#endif
 
  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Lhs.Test_Conformability (Rhs);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in intSerialArray & min(intSerialArray,intSerialArray)");
          Rhs.displayReferenceCounts("Rhs in intSerialArray & min(intSerialArray,intSerialArray)");
        }
#endif

#if defined(PPP)
  // Pointers to views of Lhs and Rhs serial arrays which allow a conformable operation
     intSerialArray *Lhs_SerialArray = NULL;
     intSerialArray *Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...
     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	      (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
               Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement (Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement (Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	      (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
	       puts ("ERROR: can't mix indirect addressing with 2 arrays and where.");
               APP_ABORT();
             }
            else
             {
               Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
                    (Lhs, Lhs_SerialArray, 
                    *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray,
                     Rhs, Rhs_SerialArray );
             }

          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

  // New test (8/5/2000)
     APP_ASSERT(Temporary_Array_Set != NULL);
  // Temporary_Array_Set->display("Check to see what sort of communication model was used");

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Lhs_SerialArray != NULL);
     APP_ASSERT(Rhs_SerialArray != NULL);

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();
     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );
     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in intSerialArray & min(intSerialArray,intSerialArray)");
          Rhs_SerialArray->displayReferenceCounts("Rhs_SerialArray in intSerialArray & min(intSerialArray,intSerialArray)");
        }
#endif

  // Inputs to intArray::Abstract_Binary_Operator:
  //     1. Temporary_Array_Set is attached to the intArray temporary returned by Abstract_Binary_Operator
  //     2. Lhs is used to get the Lhs partition information (PARTI parallel descriptor) and array reuse
  //     3. Rhs is used to get the Rhs partition information (PARTI parallel descriptor) in case the Lhs was 
  //        a NULL array (no data and no defined partitioning (i.e. no PARTI parallel descriptor)) and array reuse
  //     4. The intSerialArray which is to be put into the intArray temporary returned by Abstract_Binary_Operator
  // return intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, min(*Lhs_SerialArray,*Rhs_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = Rhs;
#else
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, Lhs_SerialArray, Rhs_SerialArray, min(*Lhs_SerialArray,*Rhs_SerialArray) );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
  // if (Lhs_SerialArray != Return_Value.getSerialArrayPointer())
     if (lhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

        }
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & min(intSerialArray,intSerialArray)");
        }
#endif

     return Return_Value;
#else
  // This function can be either Abstract_Binary_Operator or Abstract_Binary_Operator_Non_Commutative
     return intSerialArray::Abstract_Binary_Operator ( Lhs , Rhs , MDI_i_Min_Array_And_Array, MDI_i_Min_Array_And_Array_Accumulate_To_Operand , intSerialArray::min_Function );
#endif
   }


intSerialArray &
min ( const intSerialArray & X , const intSerialArray & Y , const intSerialArray & Z )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of min (intSerialArray,intSerialArray,intSerialArray) for class! X:rc=%d Y:rc=%d Z:rc=%d ",
               X.getRawDataReferenceCount(),Y.getRawDataReferenceCount(),Z.getRawDataReferenceCount());
#endif

  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
        {
          X.Test_Conformability (Y);
          X.Test_Conformability (Z);
        }
     
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in intSerialArray & min (intSerialArray,intSerialArray,intSerialArray)");
          Y.displayReferenceCounts("Y in intSerialArray & min (intSerialArray,intSerialArray,intSerialArray)");
          Z.displayReferenceCounts("Z in intSerialArray & min (intSerialArray,intSerialArray,intSerialArray)");
        }
#endif

     return min ( X , min ( Y , Z ) );
   }


intSerialArray &
min ( int x , const intSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("\n\n\nInside of min (int,intSerialArray) for intSerialArray class! \n");

  // This is the only test we can do on the input!
     Rhs.Test_Consistency ("Test Rhs in min");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs.displayReferenceCounts("Rhs in intSerialArray & min(int,intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...
     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement
              (Rhs, Rhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}

        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Rhs_SerialArray     != NULL);

     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs_SerialArray->displayReferenceCounts("Rhs_SerialArray in intSerialArray & min(int,intSerialArray)");
        }
#endif

  // return intArray::Abstract_Operator ( Temporary_Array_Set, Rhs, min(x,*Rhs_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = Rhs;
#else
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Rhs, Rhs_SerialArray, min(x,*Rhs_SerialArray) );
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value (before delete macro) in intSerialArray & min(int,intSerialArray)");
        }
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & min(int,intSerialArray)");
        }
#endif

     return Return_Value;
#else
  // This function can be either Abstract_Binary_Operator or Abstract_Binary_Operator_Non_Commutative
     return intSerialArray::Abstract_Binary_Operator ( Rhs , x ,
               MDI_i_Min_Scalar_And_Array,
               MDI_i_Min_Scalar_And_Array_Accumulate_To_Operand , intSerialArray::Scalar_min_Function );
#endif
   }


intSerialArray &
min ( int x , const intSerialArray & Y , const intSerialArray & Z )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of min (int,intSerialArray,intSerialArray) for class! Y:rc=%d Z:rc=%d ",
               Y.getRawDataReferenceCount(),Z.getRawDataReferenceCount());
#endif

  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Y.Test_Conformability (Z);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Y.displayReferenceCounts("Y in intSerialArray & min (int,intSerialArray,intSerialArray)");
          Z.displayReferenceCounts("Z in intSerialArray & min (int,intSerialArray,intSerialArray)");
        }
#endif

     return min ( x , min ( Y , Z ) );
   }


intSerialArray &
min ( const intSerialArray & X , int y , const intSerialArray & Z )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of min (intSerialArray,int,intSerialArray) for class! X:rc=%d Z:rc=%d ",
               X.getRawDataReferenceCount(),Z.getRawDataReferenceCount());
#endif

  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          X.Test_Conformability (Z);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in intSerialArray & min (int,intSerialArray,intSerialArray)");
          Z.displayReferenceCounts("Z in intSerialArray & min (int,intSerialArray,intSerialArray)");
        }
#endif

     return min ( y , min ( X , Z ) );
   }


intSerialArray &
min ( const intSerialArray & Lhs , int x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of min (intSerialArray,int) for intSerialArray class!");

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in min");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts 
          Lhs.displayReferenceCounts("Lhs in intSerialArray & min(intSerialArray,int)");
        }
#endif

#if defined(PPP)
  // Pointers to views of Lhs serial arrays which allow a conformable operation
     intSerialArray *Lhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE)
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }
     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Lhs_SerialArray != NULL);

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in intSerialArray & min(intSerialArray,int)");
        }
#endif

  // return intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, min(*Lhs_SerialArray,x) );
#if defined(MEMORY_LEAK_TEST)
     puts ("Can't do MEMORY_LEAK_TEST in intSerialArray & min ( const intSerialArray & Lhs , int x )");
#endif
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Lhs_SerialArray, min(*Lhs_SerialArray,x) );

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (lhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray;
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & min(intSerialArray,int)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator ( Lhs , x ,
               MDI_i_Min_Array_And_Scalar,
               MDI_i_Min_Array_And_Scalar_Accumulate_To_Operand , intSerialArray::Scalar_min_Function );
#endif
   }


intSerialArray &
min ( const intSerialArray & X , const intSerialArray & Y , int z )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of min (intSerialArray,intSerialArray,int) for class! X:rc=%d Y:rc=%d ",
               X.getRawDataReferenceCount(),Y.getRawDataReferenceCount());
#endif

  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          X.Test_Conformability (Y);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in intSerialArray & min (intSerialArray,intSerialArray,int)");
          Y.displayReferenceCounts("Y in intSerialArray & min (intSerialArray,intSerialArray,int)");
        }
#endif

     return min ( min ( X , Y ) , z );
   }


int
min ( const intSerialArray & X )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of min (intSerialArray) returning int for intSerialArray class! \n");

  // This is the only test we can do on the input!
     X.Test_Consistency ("Test X in min (const intSerialArray & X)");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in int min (intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *X_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
               Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement ( X, X_SerialArray );
             }
            else
             {
               Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement ( X, X_SerialArray );
             }
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
          if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
               Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement 
	          (X, X_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
             }
            else
             {
               Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement 
                    (X, X_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
             }
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

     if (Temporary_Array_Set == NULL)
        {
          Temporary_Array_Set = new Array_Conformability_Info_Type();
        }

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(X_SerialArray != NULL);

     bool xIsTemporary = X_SerialArray->isTemporary();

     APP_ASSERT ( (xIsTemporary == TRUE) || (xIsTemporary == FALSE) );

  // We need to specify the type of operation so that the reduction operation between processors can be handled correctly
#if defined(MEMORY_LEAK_TEST)
     int Return_Value = 0;
#else
     int Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, X, X_SerialArray, min (*X_SerialArray) , intSerialArray::min_Function );
  // return intArray::Abstract_Reduction_Operator ( Temporary_Array_Set, X, min (*X_SerialArray) , intSerialArray::min_Function );
#endif

  // Delete the serial array unless it would have been absorbed by the serialArray in function
     if (xIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): X_SerialArray->getReferenceCount() = %d \n",
       //      X_SerialArray->getReferenceCount());

       // Must delete the X_SerialArray if it was taken directly from the X array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (X_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          X_SerialArray->decrementReferenceCount();
          if (X_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete X_SerialArray;
             }
          X_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
       {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


     return Return_Value;
#else
     return intSerialArray::Abstract_Unary_Operator ( X , MDI_i_Min_Array_Returning_Scalar , intSerialArray::min_Function );
#endif
   }


intSerialArray &
max ( const intSerialArray & Lhs , const intSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of max (intSerialArray,intSerialArray) for intSerialArray class! Lhs:rc=%d Rhs:rc=%d ",
               Lhs.getRawDataReferenceCount(),Rhs.getRawDataReferenceCount());

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in max");
     Rhs.Test_Consistency ("Test Rhs in max");
#endif
 
  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Lhs.Test_Conformability (Rhs);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in intSerialArray & max(intSerialArray,intSerialArray)");
          Rhs.displayReferenceCounts("Rhs in intSerialArray & max(intSerialArray,intSerialArray)");
        }
#endif

#if defined(PPP)
  // Pointers to views of Lhs and Rhs serial arrays which allow a conformable operation
     intSerialArray *Lhs_SerialArray = NULL;
     intSerialArray *Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...
     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	      (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
               Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement (Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement (Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	      (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
	       puts ("ERROR: can't mix indirect addressing with 2 arrays and where.");
               APP_ABORT();
             }
            else
             {
               Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
                    (Lhs, Lhs_SerialArray, 
                    *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray,
                     Rhs, Rhs_SerialArray );
             }

          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

  // New test (8/5/2000)
     APP_ASSERT(Temporary_Array_Set != NULL);
  // Temporary_Array_Set->display("Check to see what sort of communication model was used");

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Lhs_SerialArray != NULL);
     APP_ASSERT(Rhs_SerialArray != NULL);

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();
     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );
     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in intSerialArray & max(intSerialArray,intSerialArray)");
          Rhs_SerialArray->displayReferenceCounts("Rhs_SerialArray in intSerialArray & max(intSerialArray,intSerialArray)");
        }
#endif

  // Inputs to intArray::Abstract_Binary_Operator:
  //     1. Temporary_Array_Set is attached to the intArray temporary returned by Abstract_Binary_Operator
  //     2. Lhs is used to get the Lhs partition information (PARTI parallel descriptor) and array reuse
  //     3. Rhs is used to get the Rhs partition information (PARTI parallel descriptor) in case the Lhs was 
  //        a NULL array (no data and no defined partitioning (i.e. no PARTI parallel descriptor)) and array reuse
  //     4. The intSerialArray which is to be put into the intArray temporary returned by Abstract_Binary_Operator
  // return intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, max(*Lhs_SerialArray,*Rhs_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = Rhs;
#else
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, Lhs_SerialArray, Rhs_SerialArray, max(*Lhs_SerialArray,*Rhs_SerialArray) );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
  // if (Lhs_SerialArray != Return_Value.getSerialArrayPointer())
     if (lhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

        }
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & max(intSerialArray,intSerialArray)");
        }
#endif

     return Return_Value;
#else
  // This function can be either Abstract_Binary_Operator or Abstract_Binary_Operator_Non_Commutative
     return intSerialArray::Abstract_Binary_Operator ( Lhs , Rhs , MDI_i_Max_Array_And_Array, MDI_i_Max_Array_And_Array_Accumulate_To_Operand , intSerialArray::max_Function );
#endif
   }


intSerialArray &
max ( int x , const intSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("\n\n\nInside of max (int,intSerialArray) for intSerialArray class! \n");

  // This is the only test we can do on the input!
     Rhs.Test_Consistency ("Test Rhs in max");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs.displayReferenceCounts("Rhs in intSerialArray & max(int,intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...
     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement
              (Rhs, Rhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}

        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Rhs_SerialArray     != NULL);

     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs_SerialArray->displayReferenceCounts("Rhs_SerialArray in intSerialArray & max(int,intSerialArray)");
        }
#endif

  // return intArray::Abstract_Operator ( Temporary_Array_Set, Rhs, max(x,*Rhs_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = Rhs;
#else
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Rhs, Rhs_SerialArray, max(x,*Rhs_SerialArray) );
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value (before delete macro) in intSerialArray & max(int,intSerialArray)");
        }
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & max(int,intSerialArray)");
        }
#endif

     return Return_Value;
#else
  // This function can be either Abstract_Binary_Operator or Abstract_Binary_Operator_Non_Commutative
     return intSerialArray::Abstract_Binary_Operator ( Rhs , x ,
               MDI_i_Max_Scalar_And_Array,
               MDI_i_Max_Scalar_And_Array_Accumulate_To_Operand , intSerialArray::Scalar_max_Function );
#endif
   }


intSerialArray &
max ( int x , const intSerialArray & Y , const intSerialArray & Z )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of max (int,intSerialArray,intSerialArray) for intSerialArray class!");
#endif

  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Y.Test_Conformability (Z);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Y.displayReferenceCounts("Y in intSerialArray & max (int,intSerialArray,intSerialArray)");
          Z.displayReferenceCounts("Z in intSerialArray & max (int,intSerialArray,intSerialArray)");
        }
#endif

     return max ( x , max ( Y , Z ) );
   }


intSerialArray &
max ( const intSerialArray & X , int y , const intSerialArray & Z )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of max (intSerialArray,int,intSerialArray) for intSerialArray class!");
#endif

  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          X.Test_Conformability (Z);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in intSerialArray & max (int,intSerialArray,intSerialArray)");
          Z.displayReferenceCounts("Z in intSerialArray & max (int,intSerialArray,intSerialArray)");
        }
#endif

     return max ( y , max ( X , Z ) );
   }


intSerialArray &
max ( const intSerialArray & Lhs , int x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of max (intSerialArray,int) for intSerialArray class!");

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in max");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts 
          Lhs.displayReferenceCounts("Lhs in intSerialArray & max(intSerialArray,int)");
        }
#endif

#if defined(PPP)
  // Pointers to views of Lhs serial arrays which allow a conformable operation
     intSerialArray *Lhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE)
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }
     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Lhs_SerialArray != NULL);

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in intSerialArray & max(intSerialArray,int)");
        }
#endif

  // return intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, max(*Lhs_SerialArray,x) );
#if defined(MEMORY_LEAK_TEST)
     puts ("Can't do MEMORY_LEAK_TEST in intSerialArray & max ( const intSerialArray & Lhs , int x )");
#endif
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Lhs_SerialArray, max(*Lhs_SerialArray,x) );

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (lhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray;
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & max(intSerialArray,int)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator ( Lhs , x ,
               MDI_i_Max_Array_And_Scalar,
               MDI_i_Max_Array_And_Scalar_Accumulate_To_Operand , intSerialArray::Scalar_max_Function );
#endif
   }


intSerialArray &
max ( const intSerialArray & X , const intSerialArray & Y , int z )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of max (intSerialArray,intSerialArray,int) for intSerialArray class! \n");
#endif

  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          X.Test_Conformability (Y);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in intSerialArray & max (intSerialArray,intSerialArray,int)");
          Y.displayReferenceCounts("Y in intSerialArray & max (intSerialArray,intSerialArray,int)");
        }
#endif

     return max ( max ( X , Y ) , z );
   }


int
max ( const intSerialArray & X )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of max (intSerialArray) returning int for intSerialArray class! \n");

  // This is the only test we can do on the input!
     X.Test_Consistency ("Test X in max (const intSerialArray & X)");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in int max (intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *X_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
               Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement ( X, X_SerialArray );
             }
	    else
	     {
               Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement ( X, X_SerialArray );
             }
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
          if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
               Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement (X, X_SerialArray, 
                    *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
             }
            else
             {
               Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement 
	            (X, X_SerialArray, 
                     *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	     }
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

     if (Temporary_Array_Set == NULL)
        {
       // printf ("Building the Array_Set in the max operator \n");
          Temporary_Array_Set =	new Array_Conformability_Info_Type();
        }

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(X_SerialArray       != NULL);

     bool xIsTemporary = X_SerialArray->isTemporary();

     APP_ASSERT ( (xIsTemporary == TRUE) || (xIsTemporary == FALSE) );

#if defined(MEMORY_LEAK_TEST)
     int Return_Value = 0;
#else
     int Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, X, X_SerialArray, max (*X_SerialArray) , intSerialArray::max_Function );
#endif

  // Delete the serial array unless it would have been absorbed by the serialArray in function
     if (xIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): X_SerialArray->getReferenceCount() = %d \n",
       //      X_SerialArray->getReferenceCount());

       // Must delete the X_SerialArray if it was taken directly from the X array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (X_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          X_SerialArray->decrementReferenceCount();
          if (X_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete X_SerialArray;
             }
          X_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


     return Return_Value;
#else
     return intSerialArray::Abstract_Unary_Operator ( X , MDI_i_Max_Array_Returning_Scalar , intSerialArray::max_Function );
#endif
   }


intSerialArray &
max ( const intSerialArray & X , const intSerialArray & Y , const intSerialArray & Z )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of max (intSerialArray,intSerialArray,intSerialArray) for intSerialArray class! \n");
#endif

  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
        {
          X.Test_Conformability (Y);
          X.Test_Conformability (Z);
        }

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in intSerialArray & max (intSerialArray,intSerialArray,intSerialArray)");
          Y.displayReferenceCounts("Y in intSerialArray & max (intSerialArray,intSerialArray,intSerialArray)");
          Z.displayReferenceCounts("Z in intSerialArray & max (intSerialArray,intSerialArray,intSerialArray)");
        }
#endif

     return max ( X , max ( Y , Z ) );
   }


int
sum ( const intSerialArray & X )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of sum (const intSerialArray) returning int for intSerialArray class!");

  // This is the only test we can do on the input!
     X.Test_Consistency ("Test X in sum (const intSerialArray & X)");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in int sum (intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *X_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE)
             {
               Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement ( X, X_SerialArray );
             }
            else
             {
               Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement ( X, X_SerialArray );
             }
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
          if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
               Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement 
                    (X, X_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
             }
	    else
             {
               Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement 
                    (X, X_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
             }
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set =	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(X_SerialArray != NULL);

     bool xIsTemporary = X_SerialArray->isTemporary();

     APP_ASSERT ( (xIsTemporary == TRUE) || (xIsTemporary == FALSE) );

  // This bug is fixed in the intSerialArray::Parallel_Conformability_Enforcement function by restricting the
  // view of the serial array returned to the non ghost boundary portion of the array.
  // If we did that then A = -A would require message passing to update the ghost boundaries.
  // So have to fix it here more directly.
  // Bugfix (2/7/96) P++ must avoid counting the ghost boundaries when performing reduction operations!
     Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type Index_Pointer_List;
     for (int i=0; i < MAX_ARRAY_DIMENSION; i++)
        {
       // Index_Pointer_List[i] = &(X.Array_Descriptor.Array_Domain.Local_Mask_Index[i]);
       // This is all a lot more complex if the stride is not the unit stride! 
       // So for now we avoid this case.
          int Ghost_Boundary_Width = X.Array_Descriptor.Array_Domain.InternalGhostCellWidth[i];
          int Local_Base  = X.getLocalBase(i);
          int Local_Bound = X.getLocalBound(i);

       // Left and right edges do not  i n c l u d e  a ghost boundary!
          if (X.Array_Descriptor.Array_Domain.isLeftPartition(i) == FALSE)
             Local_Base += Ghost_Boundary_Width;
          if (X.Array_Descriptor.Array_Domain.isRightPartition(i) == FALSE)
             Local_Bound -= Ghost_Boundary_Width;
       // APP_ASSERT(Local_Base <= Local_Bound);
       // ... (12/27/96,kdb) only valid value might be on a ghost cell so make a NULL INDEX in this case ...
          if (Local_Base <= Local_Bound)
               Index_Pointer_List[i] = new Range (Local_Base,Local_Bound);
            else
               Index_Pointer_List[i] = new Internal_Index (Local_Base,0);
#if 0
          APP_ASSERT (X.Array_Descriptor.Array_Domain.Stride[i] == 1);
#else
       // Now take the intersection of this with the local mask
          (*Index_Pointer_List[i]) = (*Index_Pointer_List[i])(X.Array_Descriptor.Array_Domain.Local_Mask_Index[i]);
#endif
        }

#if defined(MEMORY_LEAK_TEST)
     int Return_Value = 0;
#else
  // Note that we hand the sum operator a view and this means we have
  // to delete the X_SerialArray explicitly (unlike other operators)
     int Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, X, X_SerialArray, sum ((*X_SerialArray)(Index_Pointer_List)) , intSerialArray::sum_Function );
#endif

     for (int k=0; k < MAX_ARRAY_DIMENSION; k++)
        {
          if (Index_Pointer_List[k] != NULL)
             {
               delete Index_Pointer_List[k];
               Index_Pointer_List[k] = NULL;
             }
        }

  // Note that a view is handed into the sum operator (so this is not dependent upon the value of xIsTemporary)
     // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): X_SerialArray->getReferenceCount() = %d \n",
       //      X_SerialArray->getReferenceCount());

       // Must delete the X_SerialArray if it was taken directly from the X array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (X_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          X_SerialArray->decrementReferenceCount();
          if (X_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete X_SerialArray;
             }
          X_SerialArray = NULL;


  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


     return Return_Value;
#else
     return intSerialArray::Abstract_Unary_Operator ( X , MDI_i_Sum_Array_Returning_Scalar , intSerialArray::sum_Function );
#endif
   }


intSerialArray &
intSerialArray::operator! ()
   { 
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator! for intSerialArray class!");

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in intSerialArray::operatoroperator!");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS in intSerialArray & intSerialArray::operator!()");
        }
#endif

#if defined(PPP)
     intSerialArray *This_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
	       Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement ( *this, This_SerialArray);
             }
            else
             {
               Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement ( *this, This_SerialArray);
             }
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
          if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
               Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement (
                    *this, This_SerialArray, 
                    *Where_Statement_Support::Where_Statement_Mask,
                    Mask_SerialArray);
             }
            else
             {
               Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement 
	          ( *this, This_SerialArray, 
                    *Where_Statement_Support::Where_Statement_Mask,
                    Mask_SerialArray);
             }
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

     if (Temporary_Array_Set == NULL) Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(This_SerialArray    != NULL);

     bool thisIsTemporary = This_SerialArray->isTemporary();

     APP_ASSERT ( (thisIsTemporary == TRUE) || (thisIsTemporary == FALSE) );

  // return intArray::Abstract_Operator ( Temporary_Array_Set, *this, This_SerialArray->operator!() );
#if defined(MEMORY_LEAK_TEST)
     puts ("Can't do MEMORY_LEAK_TEST in intSerialArray & intSerialArray::operator! ()");
#endif
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, *this, This_SerialArray, This_SerialArray->operator!() );

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (thisIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): This_SerialArray->getReferenceCount() = %d \n",
       //      This_SerialArray->getReferenceCount());

       // Must delete the This_SerialArray if it was taken directly from the This array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (This_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          This_SerialArray->decrementReferenceCount();
          if (This_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete This_SerialArray;
             }
          This_SerialArray = NULL;

        }
     
  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & intSerialArray::operator!()");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Unary_Operator_Returning_IntArray ( *this ,
                   MDI_i_NOT_Array_Returning_IntArray ,
                   MDI_i_NOT_Array_Accumulate_To_Operand_Returning_IntArray , 
                   intSerialArray::Not );
#endif
   }


intSerialArray &
operator- ( const intSerialArray & Lhs , const intSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator- (intSerialArray,intSerialArray) for intSerialArray class!");

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator-");
     Rhs.Test_Consistency ("Test Rhs in operator-");
#endif

  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Lhs.Test_Conformability (Rhs);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in intSerialArray & operator-(intSerialArray,intSerialArray)");
          Rhs.displayReferenceCounts("Rhs in intSerialArray & operator-(intSerialArray,intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray* Lhs_SerialArray = NULL;
     intSerialArray* Rhs_SerialArray = NULL;

     intSerialArray* Mask_SerialArray = NULL;
     intSerialArray* Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	    (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
        {
	   puts ("ERROR: can't mix indirect addressing with 2 arrays and where.");
           APP_ABORT();
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray,
	       Rhs, Rhs_SerialArray );
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set =	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Lhs_SerialArray != NULL);
     APP_ASSERT(Rhs_SerialArray != NULL);

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();
     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );
     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

  // return intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, *Lhs_SerialArray - *Rhs_SerialArray );
#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = Lhs;
#else
     intArray & Return_Value = intArray::Abstract_Operator 
	( Temporary_Array_Set, Lhs, Rhs, 
	  Lhs_SerialArray, Rhs_SerialArray, *Lhs_SerialArray - *Rhs_SerialArray );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (lhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

        }
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }

     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & operator-(intSerialArray,intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator_Non_Commutative ( Lhs , Rhs , 
               MDI_i_Subtract_Array_Minus_Array,
               MDI_i_Subtract_Array_Minus_Array_Accumulate_To_Operand , intSerialArray::Minus );
#endif
   }


intSerialArray &
operator< ( const intSerialArray & Lhs , const intSerialArray & Rhs )
   {   
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of operator< (intSerialArray(id=%d),intSerialArray(id=%d)) for intSerialArray class!",
               Lhs.Array_ID(),Rhs.Array_ID());

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator< (intSerialArray,intSerialArray)");
     Rhs.Test_Consistency ("Test Rhs in operator< (intSerialArray,intSerialArray)");
#endif
 
  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Lhs.Test_Conformability (Rhs);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in intSerialArray & operator<(intSerialArray,intSerialArray)");
          Rhs.displayReferenceCounts("Rhs in intSerialArray & operator<(intSerialArray,intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *Lhs_SerialArray = NULL;
     intSerialArray *Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

  // printf ("Checking if WHERE statement is used before calling SerialArray::Parallel_Conformability_Enforcement from operator< \n");

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
       // printf ("Checking if indirect addressing is used before calling SerialArray::Parallel_Conformability_Enforcement from operator< \n");
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE)|| 
              (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
               Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE)|| 
              (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
               printf ("ERROR: can't mix indirect addressing with 2 arrays and where. \n");
               APP_ABORT();
             }
            else
             {
               Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray, Rhs, Rhs_SerialArray );
             }
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Lhs_SerialArray     != NULL);
     APP_ASSERT(Rhs_SerialArray     != NULL);
     APP_ASSERT(Temporary_Array_Set != NULL);

#if 0
     Lhs.displayReferenceCounts ("Lhs after PCE in operator< (intSerialArray,intSerialArray)");
     Rhs.displayReferenceCounts ("Rhs after PCE in operator< (intSerialArray,intSerialArray)");
     Lhs_SerialArray->displayReferenceCounts ("Lhs_SerialArray after PCE in operator< (intSerialArray,intSerialArray)");
     Rhs_SerialArray->displayReferenceCounts ("Rhs_SerialArray after PCE in operator< (intSerialArray,intSerialArray)");
#endif

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();
     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );
     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

#if defined(MEMORY_LEAK_TEST)
     printf ("Can't do MEMORY_LEAK_TEST in intSerialArray & operator< ( const intSerialArray & Lhs , const intSerialArray & Rhs ) \n");
#endif
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, Lhs_SerialArray, Rhs_SerialArray, *Lhs_SerialArray < *Rhs_SerialArray );

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
#ifndef INTARRAY
  // Since the Lhs is a different type than the Return Value it could not be reused!
     // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;


  // Since the Rhs is a different type than the Return Value it could not be reused!
     // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

#else
     if (lhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

        }
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }
#endif

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray;
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in operator< (intSerialArray,intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator_Returning_IntArray ( Lhs , Rhs ,
               MDI_i_LT_Array_LT_Array,
               MDI_i_LT_Array_LT_Array_Accumulate_To_Operand , intSerialArray::LT );
#endif
   }   


intSerialArray &
operator< ( const intSerialArray & Lhs , int x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator< (intSerialArray,int) for intSerialArray class!");

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator< (intSerialArray,int)");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in intSerialArray & operator<(intSerialArray,int)");
        }
#endif

#if defined(PPP)
     intSerialArray *Lhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Lhs_SerialArray != NULL);

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );

  // return intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, *Lhs_SerialArray < x );
#if defined(MEMORY_LEAK_TEST)
     puts ("Can't do MEMORY_LEAK_TEST in intSerialArray & operator< ( const intSerialArray & Lhs , int x )");
#endif
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Lhs_SerialArray, *Lhs_SerialArray < x );

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
#ifndef INTARRAY
  // Since the Lhs is a different type than the Return Value it could not be reused!
     // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

#else
     if (lhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

        }
#endif

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & operator<(intSerialArray,int)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator_Returning_IntArray ( Lhs , x ,
               MDI_i_LT_Array_LT_Scalar,
               MDI_i_LT_Array_LT_Scalar_Accumulate_To_Operand , intSerialArray::Scalar_LT );
#endif
   }


intSerialArray &
operator< ( int x , const intSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator< (int,intSerialArray) for intSerialArray class!");

  // This is the only test we can do on the input!
     Rhs.Test_Consistency ("Test Rhs in operator< (int,intSerialArray)");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs.displayReferenceCounts("Rhs in intSerialArray & operator<(int,intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *Rhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Rhs_SerialArray     != NULL);

     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

#if defined(MEMORY_LEAK_TEST)
     printf ("Can't do MEMORY_LEAK_TEST in intSerialArray & operator< ( int x , const intSerialArray & Rhs ) \n");
#endif
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Rhs, Rhs_SerialArray, x < *Rhs_SerialArray );

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
#ifndef INTARRAY
  // Since the Lhs is a different type than the Return Value it could not be reused!
     // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

#else
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }
#endif

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & operator<(int,intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator_Returning_IntArray ( Rhs , x ,
               MDI_i_LT_Scalar_LT_Array,
               MDI_i_LT_Scalar_LT_Array_Accumulate_To_Operand , intSerialArray::Scalar_LT );
#endif
   }


intSerialArray &
operator> ( const intSerialArray & Lhs , const intSerialArray & Rhs )
   {   
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of operator> (intSerialArray(id=%d),intSerialArray(id=%d)) for intSerialArray class!",
               Lhs.Array_ID(),Rhs.Array_ID());

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator> (intSerialArray,intSerialArray)");
     Rhs.Test_Consistency ("Test Rhs in operator> (intSerialArray,intSerialArray)");
#endif
 
  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Lhs.Test_Conformability (Rhs);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in intSerialArray & operator>(intSerialArray,intSerialArray)");
          Rhs.displayReferenceCounts("Rhs in intSerialArray & operator>(intSerialArray,intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *Lhs_SerialArray = NULL;
     intSerialArray *Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

  // printf ("Checking if WHERE statement is used before calling SerialArray::Parallel_Conformability_Enforcement from operator> \n");

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
       // printf ("Checking if indirect addressing is used before calling SerialArray::Parallel_Conformability_Enforcement from operator> \n");
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE)|| 
              (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
               Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE)|| 
              (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
               printf ("ERROR: can't mix indirect addressing with 2 arrays and where. \n");
               APP_ABORT();
             }
            else
             {
               Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray, Rhs, Rhs_SerialArray );
             }
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Lhs_SerialArray     != NULL);
     APP_ASSERT(Rhs_SerialArray     != NULL);
     APP_ASSERT(Temporary_Array_Set != NULL);

#if 0
     Lhs.displayReferenceCounts ("Lhs after PCE in operator> (intSerialArray,intSerialArray)");
     Rhs.displayReferenceCounts ("Rhs after PCE in operator> (intSerialArray,intSerialArray)");
     Lhs_SerialArray->displayReferenceCounts ("Lhs_SerialArray after PCE in operator> (intSerialArray,intSerialArray)");
     Rhs_SerialArray->displayReferenceCounts ("Rhs_SerialArray after PCE in operator> (intSerialArray,intSerialArray)");
#endif

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();
     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );
     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

#if defined(MEMORY_LEAK_TEST)
     printf ("Can't do MEMORY_LEAK_TEST in intSerialArray & operator> ( const intSerialArray & Lhs , const intSerialArray & Rhs ) \n");
#endif
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, Lhs_SerialArray, Rhs_SerialArray, *Lhs_SerialArray > *Rhs_SerialArray );

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
#ifndef INTARRAY
  // Since the Lhs is a different type than the Return Value it could not be reused!
     // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;


  // Since the Rhs is a different type than the Return Value it could not be reused!
     // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

#else
     if (lhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

        }
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }
#endif

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray;
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in operator> (intSerialArray,intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator_Returning_IntArray ( Lhs , Rhs ,
               MDI_i_GT_Array_GT_Array,
               MDI_i_GT_Array_GT_Array_Accumulate_To_Operand , intSerialArray::GT );
#endif
   }   


intSerialArray &
operator> ( const intSerialArray & Lhs , int x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator> (intSerialArray,int) for intSerialArray class!");

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator> (intSerialArray,int)");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in intSerialArray & operator>(intSerialArray,int)");
        }
#endif

#if defined(PPP)
     intSerialArray *Lhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Lhs_SerialArray != NULL);

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );

  // return intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, *Lhs_SerialArray > x );
#if defined(MEMORY_LEAK_TEST)
     puts ("Can't do MEMORY_LEAK_TEST in intSerialArray & operator> ( const intSerialArray & Lhs , int x )");
#endif
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Lhs_SerialArray, *Lhs_SerialArray > x );

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
#ifndef INTARRAY
  // Since the Lhs is a different type than the Return Value it could not be reused!
     // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

#else
     if (lhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

        }
#endif

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & operator>(intSerialArray,int)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator_Returning_IntArray ( Lhs , x ,
               MDI_i_GT_Array_GT_Scalar,
               MDI_i_GT_Array_GT_Scalar_Accumulate_To_Operand , intSerialArray::Scalar_GT );
#endif
   }


intSerialArray &
operator> ( int x , const intSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator> (int,intSerialArray) for intSerialArray class!");

  // This is the only test we can do on the input!
     Rhs.Test_Consistency ("Test Rhs in operator> (int,intSerialArray)");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs.displayReferenceCounts("Rhs in intSerialArray & operator>(int,intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *Rhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Rhs_SerialArray     != NULL);

     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

#if defined(MEMORY_LEAK_TEST)
     printf ("Can't do MEMORY_LEAK_TEST in intSerialArray & operator> ( int x , const intSerialArray & Rhs ) \n");
#endif
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Rhs, Rhs_SerialArray, x > *Rhs_SerialArray );

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
#ifndef INTARRAY
  // Since the Lhs is a different type than the Return Value it could not be reused!
     // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

#else
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }
#endif

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & operator>(int,intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator_Returning_IntArray ( Rhs , x ,
               MDI_i_GT_Scalar_GT_Array,
               MDI_i_GT_Scalar_GT_Array_Accumulate_To_Operand , intSerialArray::Scalar_GT );
#endif
   }


intSerialArray &
operator<= ( const intSerialArray & Lhs , const intSerialArray & Rhs )
   {   
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of operator<= (intSerialArray(id=%d),intSerialArray(id=%d)) for intSerialArray class!",
               Lhs.Array_ID(),Rhs.Array_ID());

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator<= (intSerialArray,intSerialArray)");
     Rhs.Test_Consistency ("Test Rhs in operator<= (intSerialArray,intSerialArray)");
#endif
 
  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Lhs.Test_Conformability (Rhs);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in intSerialArray & operator<=(intSerialArray,intSerialArray)");
          Rhs.displayReferenceCounts("Rhs in intSerialArray & operator<=(intSerialArray,intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *Lhs_SerialArray = NULL;
     intSerialArray *Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

  // printf ("Checking if WHERE statement is used before calling SerialArray::Parallel_Conformability_Enforcement from operator<= \n");

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
       // printf ("Checking if indirect addressing is used before calling SerialArray::Parallel_Conformability_Enforcement from operator<= \n");
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE)|| 
              (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
               Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE)|| 
              (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
               printf ("ERROR: can't mix indirect addressing with 2 arrays and where. \n");
               APP_ABORT();
             }
            else
             {
               Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray, Rhs, Rhs_SerialArray );
             }
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Lhs_SerialArray     != NULL);
     APP_ASSERT(Rhs_SerialArray     != NULL);
     APP_ASSERT(Temporary_Array_Set != NULL);

#if 0
     Lhs.displayReferenceCounts ("Lhs after PCE in operator<= (intSerialArray,intSerialArray)");
     Rhs.displayReferenceCounts ("Rhs after PCE in operator<= (intSerialArray,intSerialArray)");
     Lhs_SerialArray->displayReferenceCounts ("Lhs_SerialArray after PCE in operator<= (intSerialArray,intSerialArray)");
     Rhs_SerialArray->displayReferenceCounts ("Rhs_SerialArray after PCE in operator<= (intSerialArray,intSerialArray)");
#endif

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();
     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );
     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

#if defined(MEMORY_LEAK_TEST)
     printf ("Can't do MEMORY_LEAK_TEST in intSerialArray & operator<= ( const intSerialArray & Lhs , const intSerialArray & Rhs ) \n");
#endif
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, Lhs_SerialArray, Rhs_SerialArray, *Lhs_SerialArray <= *Rhs_SerialArray );

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
#ifndef INTARRAY
  // Since the Lhs is a different type than the Return Value it could not be reused!
     // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;


  // Since the Rhs is a different type than the Return Value it could not be reused!
     // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

#else
     if (lhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

        }
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }
#endif

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray;
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in operator<= (intSerialArray,intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator_Returning_IntArray ( Lhs , Rhs ,
               MDI_i_LTEQ_Array_LTEQ_Array,
               MDI_i_LTEQ_Array_LTEQ_Array_Accumulate_To_Operand , intSerialArray::LTEQ );
#endif
   }   


intSerialArray &
operator<= ( const intSerialArray & Lhs , int x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator<= (intSerialArray,int) for intSerialArray class!");

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator<= (intSerialArray,int)");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in intSerialArray & operator<=(intSerialArray,int)");
        }
#endif

#if defined(PPP)
     intSerialArray *Lhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Lhs_SerialArray != NULL);

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );

  // return intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, *Lhs_SerialArray <= x );
#if defined(MEMORY_LEAK_TEST)
     puts ("Can't do MEMORY_LEAK_TEST in intSerialArray & operator<= ( const intSerialArray & Lhs , int x )");
#endif
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Lhs_SerialArray, *Lhs_SerialArray <= x );

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
#ifndef INTARRAY
  // Since the Lhs is a different type than the Return Value it could not be reused!
     // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

#else
     if (lhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

        }
#endif

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & operator<=(intSerialArray,int)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator_Returning_IntArray ( Lhs , x ,
               MDI_i_LTEQ_Array_LTEQ_Scalar,
               MDI_i_LTEQ_Array_LTEQ_Scalar_Accumulate_To_Operand , intSerialArray::Scalar_LTEQ );
#endif
   }


intSerialArray &
operator<= ( int x , const intSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator<= (int,intSerialArray) for intSerialArray class!");

  // This is the only test we can do on the input!
     Rhs.Test_Consistency ("Test Rhs in operator<= (int,intSerialArray)");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs.displayReferenceCounts("Rhs in intSerialArray & operator<=(int,intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *Rhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Rhs_SerialArray     != NULL);

     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

#if defined(MEMORY_LEAK_TEST)
     printf ("Can't do MEMORY_LEAK_TEST in intSerialArray & operator<= ( int x , const intSerialArray & Rhs ) \n");
#endif
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Rhs, Rhs_SerialArray, x <= *Rhs_SerialArray );

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
#ifndef INTARRAY
  // Since the Lhs is a different type than the Return Value it could not be reused!
     // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

#else
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }
#endif

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & operator<=(int,intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator_Returning_IntArray ( Rhs , x ,
               MDI_i_LTEQ_Scalar_LTEQ_Array,
               MDI_i_LTEQ_Scalar_LTEQ_Array_Accumulate_To_Operand , intSerialArray::Scalar_LTEQ );
#endif
   }


intSerialArray &
operator>= ( const intSerialArray & Lhs , const intSerialArray & Rhs )
   {   
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of operator>= (intSerialArray(id=%d),intSerialArray(id=%d)) for intSerialArray class!",
               Lhs.Array_ID(),Rhs.Array_ID());

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator>= (intSerialArray,intSerialArray)");
     Rhs.Test_Consistency ("Test Rhs in operator>= (intSerialArray,intSerialArray)");
#endif
 
  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Lhs.Test_Conformability (Rhs);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in intSerialArray & operator>=(intSerialArray,intSerialArray)");
          Rhs.displayReferenceCounts("Rhs in intSerialArray & operator>=(intSerialArray,intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *Lhs_SerialArray = NULL;
     intSerialArray *Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

  // printf ("Checking if WHERE statement is used before calling SerialArray::Parallel_Conformability_Enforcement from operator>= \n");

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
       // printf ("Checking if indirect addressing is used before calling SerialArray::Parallel_Conformability_Enforcement from operator>= \n");
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE)|| 
              (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
               Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE)|| 
              (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
               printf ("ERROR: can't mix indirect addressing with 2 arrays and where. \n");
               APP_ABORT();
             }
            else
             {
               Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray, Rhs, Rhs_SerialArray );
             }
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Lhs_SerialArray     != NULL);
     APP_ASSERT(Rhs_SerialArray     != NULL);
     APP_ASSERT(Temporary_Array_Set != NULL);

#if 0
     Lhs.displayReferenceCounts ("Lhs after PCE in operator>= (intSerialArray,intSerialArray)");
     Rhs.displayReferenceCounts ("Rhs after PCE in operator>= (intSerialArray,intSerialArray)");
     Lhs_SerialArray->displayReferenceCounts ("Lhs_SerialArray after PCE in operator>= (intSerialArray,intSerialArray)");
     Rhs_SerialArray->displayReferenceCounts ("Rhs_SerialArray after PCE in operator>= (intSerialArray,intSerialArray)");
#endif

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();
     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );
     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

#if defined(MEMORY_LEAK_TEST)
     printf ("Can't do MEMORY_LEAK_TEST in intSerialArray & operator>= ( const intSerialArray & Lhs , const intSerialArray & Rhs ) \n");
#endif
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, Lhs_SerialArray, Rhs_SerialArray, *Lhs_SerialArray >= *Rhs_SerialArray );

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
#ifndef INTARRAY
  // Since the Lhs is a different type than the Return Value it could not be reused!
     // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;


  // Since the Rhs is a different type than the Return Value it could not be reused!
     // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

#else
     if (lhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

        }
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }
#endif

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray;
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in operator>= (intSerialArray,intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator_Returning_IntArray ( Lhs , Rhs ,
               MDI_i_GTEQ_Array_GTEQ_Array,
               MDI_i_GTEQ_Array_GTEQ_Array_Accumulate_To_Operand , intSerialArray::GTEQ );
#endif
   }   


intSerialArray &
operator- ( const intSerialArray & Lhs , int x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator- (intSerialArray,int) for intSerialArray class!");

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator-");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in intSerialArray & operator-(intSerialArray,int)");
        }
#endif

#if defined(PPP)
     intSerialArray* Lhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      ( Lhs, Lhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = 
	      intArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set =	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Lhs_SerialArray     != NULL);

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );

#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = Lhs;
#else
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Lhs_SerialArray, *Lhs_SerialArray - x );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (lhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & operator-(intSerialArray,int)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator ( Lhs , x , 
               MDI_i_Subtract_Array_Minus_Scalar,
               MDI_i_Subtract_Array_Minus_Scalar_Accumulate_To_Operand , intSerialArray::Scalar_Minus );
#endif
   }


intSerialArray &
operator>= ( const intSerialArray & Lhs , int x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator>= (intSerialArray,int) for intSerialArray class!");

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator>= (intSerialArray,int)");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in intSerialArray & operator>=(intSerialArray,int)");
        }
#endif

#if defined(PPP)
     intSerialArray *Lhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Lhs_SerialArray != NULL);

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );

  // return intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, *Lhs_SerialArray >= x );
#if defined(MEMORY_LEAK_TEST)
     puts ("Can't do MEMORY_LEAK_TEST in intSerialArray & operator>= ( const intSerialArray & Lhs , int x )");
#endif
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Lhs_SerialArray, *Lhs_SerialArray >= x );

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
#ifndef INTARRAY
  // Since the Lhs is a different type than the Return Value it could not be reused!
     // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

#else
     if (lhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

        }
#endif

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & operator>=(intSerialArray,int)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator_Returning_IntArray ( Lhs , x ,
               MDI_i_GTEQ_Array_GTEQ_Scalar,
               MDI_i_GTEQ_Array_GTEQ_Scalar_Accumulate_To_Operand , intSerialArray::Scalar_GTEQ );
#endif
   }


intSerialArray &
operator>= ( int x , const intSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator>= (int,intSerialArray) for intSerialArray class!");

  // This is the only test we can do on the input!
     Rhs.Test_Consistency ("Test Rhs in operator>= (int,intSerialArray)");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs.displayReferenceCounts("Rhs in intSerialArray & operator>=(int,intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *Rhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Rhs_SerialArray     != NULL);

     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

#if defined(MEMORY_LEAK_TEST)
     printf ("Can't do MEMORY_LEAK_TEST in intSerialArray & operator>= ( int x , const intSerialArray & Rhs ) \n");
#endif
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Rhs, Rhs_SerialArray, x >= *Rhs_SerialArray );

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
#ifndef INTARRAY
  // Since the Lhs is a different type than the Return Value it could not be reused!
     // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

#else
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }
#endif

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & operator>=(int,intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator_Returning_IntArray ( Rhs , x ,
               MDI_i_GTEQ_Scalar_GTEQ_Array,
               MDI_i_GTEQ_Scalar_GTEQ_Array_Accumulate_To_Operand , intSerialArray::Scalar_GTEQ );
#endif
   }


intSerialArray &
operator== ( const intSerialArray & Lhs , const intSerialArray & Rhs )
   {   
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of operator== (intSerialArray(id=%d),intSerialArray(id=%d)) for intSerialArray class!",
               Lhs.Array_ID(),Rhs.Array_ID());

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator== (intSerialArray,intSerialArray)");
     Rhs.Test_Consistency ("Test Rhs in operator== (intSerialArray,intSerialArray)");
#endif
 
  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Lhs.Test_Conformability (Rhs);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in intSerialArray & operator==(intSerialArray,intSerialArray)");
          Rhs.displayReferenceCounts("Rhs in intSerialArray & operator==(intSerialArray,intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *Lhs_SerialArray = NULL;
     intSerialArray *Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

  // printf ("Checking if WHERE statement is used before calling SerialArray::Parallel_Conformability_Enforcement from operator== \n");

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
       // printf ("Checking if indirect addressing is used before calling SerialArray::Parallel_Conformability_Enforcement from operator== \n");
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE)|| 
              (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
               Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE)|| 
              (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
               printf ("ERROR: can't mix indirect addressing with 2 arrays and where. \n");
               APP_ABORT();
             }
            else
             {
               Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray, Rhs, Rhs_SerialArray );
             }
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Lhs_SerialArray     != NULL);
     APP_ASSERT(Rhs_SerialArray     != NULL);
     APP_ASSERT(Temporary_Array_Set != NULL);

#if 0
     Lhs.displayReferenceCounts ("Lhs after PCE in operator== (intSerialArray,intSerialArray)");
     Rhs.displayReferenceCounts ("Rhs after PCE in operator== (intSerialArray,intSerialArray)");
     Lhs_SerialArray->displayReferenceCounts ("Lhs_SerialArray after PCE in operator== (intSerialArray,intSerialArray)");
     Rhs_SerialArray->displayReferenceCounts ("Rhs_SerialArray after PCE in operator== (intSerialArray,intSerialArray)");
#endif

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();
     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );
     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

#if defined(MEMORY_LEAK_TEST)
     printf ("Can't do MEMORY_LEAK_TEST in intSerialArray & operator== ( const intSerialArray & Lhs , const intSerialArray & Rhs ) \n");
#endif
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, Lhs_SerialArray, Rhs_SerialArray, *Lhs_SerialArray == *Rhs_SerialArray );

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
#ifndef INTARRAY
  // Since the Lhs is a different type than the Return Value it could not be reused!
     // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;


  // Since the Rhs is a different type than the Return Value it could not be reused!
     // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

#else
     if (lhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

        }
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }
#endif

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray;
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in operator== (intSerialArray,intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator_Returning_IntArray ( Lhs , Rhs ,
               MDI_i_EQ_Array_EQ_Array,
               MDI_i_EQ_Array_EQ_Array_Accumulate_To_Operand , intSerialArray::EQ );
#endif
   }   


intSerialArray &
operator== ( const intSerialArray & Lhs , int x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator== (intSerialArray,int) for intSerialArray class!");

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator== (intSerialArray,int)");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in intSerialArray & operator==(intSerialArray,int)");
        }
#endif

#if defined(PPP)
     intSerialArray *Lhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Lhs_SerialArray != NULL);

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );

  // return intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, *Lhs_SerialArray == x );
#if defined(MEMORY_LEAK_TEST)
     puts ("Can't do MEMORY_LEAK_TEST in intSerialArray & operator== ( const intSerialArray & Lhs , int x )");
#endif
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Lhs_SerialArray, *Lhs_SerialArray == x );

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
#ifndef INTARRAY
  // Since the Lhs is a different type than the Return Value it could not be reused!
     // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

#else
     if (lhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

        }
#endif

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & operator==(intSerialArray,int)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator_Returning_IntArray ( Lhs , x ,
               MDI_i_EQ_Array_EQ_Scalar,
               MDI_i_EQ_Array_EQ_Scalar_Accumulate_To_Operand , intSerialArray::Scalar_EQ );
#endif
   }


intSerialArray &
operator== ( int x , const intSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator== (int,intSerialArray) for intSerialArray class!");

  // This is the only test we can do on the input!
     Rhs.Test_Consistency ("Test Rhs in operator== (int,intSerialArray)");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs.displayReferenceCounts("Rhs in intSerialArray & operator==(int,intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *Rhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Rhs_SerialArray     != NULL);

     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

#if defined(MEMORY_LEAK_TEST)
     printf ("Can't do MEMORY_LEAK_TEST in intSerialArray & operator== ( int x , const intSerialArray & Rhs ) \n");
#endif
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Rhs, Rhs_SerialArray, x == *Rhs_SerialArray );

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
#ifndef INTARRAY
  // Since the Lhs is a different type than the Return Value it could not be reused!
     // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

#else
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }
#endif

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & operator==(int,intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator_Returning_IntArray ( Rhs , x ,
               MDI_i_EQ_Scalar_EQ_Array,
               MDI_i_EQ_Scalar_EQ_Array_Accumulate_To_Operand , intSerialArray::Scalar_EQ );
#endif
   }


intSerialArray &
operator!= ( const intSerialArray & Lhs , const intSerialArray & Rhs )
   {   
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of operator!= (intSerialArray(id=%d),intSerialArray(id=%d)) for intSerialArray class!",
               Lhs.Array_ID(),Rhs.Array_ID());

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator!= (intSerialArray,intSerialArray)");
     Rhs.Test_Consistency ("Test Rhs in operator!= (intSerialArray,intSerialArray)");
#endif
 
  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Lhs.Test_Conformability (Rhs);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in intSerialArray & operator!=(intSerialArray,intSerialArray)");
          Rhs.displayReferenceCounts("Rhs in intSerialArray & operator!=(intSerialArray,intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *Lhs_SerialArray = NULL;
     intSerialArray *Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

  // printf ("Checking if WHERE statement is used before calling SerialArray::Parallel_Conformability_Enforcement from operator!= \n");

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
       // printf ("Checking if indirect addressing is used before calling SerialArray::Parallel_Conformability_Enforcement from operator!= \n");
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE)|| 
              (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
               Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE)|| 
              (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
               printf ("ERROR: can't mix indirect addressing with 2 arrays and where. \n");
               APP_ABORT();
             }
            else
             {
               Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray, Rhs, Rhs_SerialArray );
             }
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Lhs_SerialArray     != NULL);
     APP_ASSERT(Rhs_SerialArray     != NULL);
     APP_ASSERT(Temporary_Array_Set != NULL);

#if 0
     Lhs.displayReferenceCounts ("Lhs after PCE in operator!= (intSerialArray,intSerialArray)");
     Rhs.displayReferenceCounts ("Rhs after PCE in operator!= (intSerialArray,intSerialArray)");
     Lhs_SerialArray->displayReferenceCounts ("Lhs_SerialArray after PCE in operator!= (intSerialArray,intSerialArray)");
     Rhs_SerialArray->displayReferenceCounts ("Rhs_SerialArray after PCE in operator!= (intSerialArray,intSerialArray)");
#endif

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();
     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );
     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

#if defined(MEMORY_LEAK_TEST)
     printf ("Can't do MEMORY_LEAK_TEST in intSerialArray & operator!= ( const intSerialArray & Lhs , const intSerialArray & Rhs ) \n");
#endif
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, Lhs_SerialArray, Rhs_SerialArray, *Lhs_SerialArray != *Rhs_SerialArray );

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
#ifndef INTARRAY
  // Since the Lhs is a different type than the Return Value it could not be reused!
     // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;


  // Since the Rhs is a different type than the Return Value it could not be reused!
     // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

#else
     if (lhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

        }
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }
#endif

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray;
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in operator!= (intSerialArray,intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator_Returning_IntArray ( Lhs , Rhs ,
               MDI_i_NOT_EQ_Array_NOT_EQ_Array,
               MDI_i_NOT_EQ_Array_NOT_EQ_Array_Accumulate_To_Operand , intSerialArray::NOT_EQ );
#endif
   }   


intSerialArray &
operator!= ( const intSerialArray & Lhs , int x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator!= (intSerialArray,int) for intSerialArray class!");

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator!= (intSerialArray,int)");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in intSerialArray & operator!=(intSerialArray,int)");
        }
#endif

#if defined(PPP)
     intSerialArray *Lhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Lhs_SerialArray != NULL);

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );

  // return intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, *Lhs_SerialArray != x );
#if defined(MEMORY_LEAK_TEST)
     puts ("Can't do MEMORY_LEAK_TEST in intSerialArray & operator!= ( const intSerialArray & Lhs , int x )");
#endif
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Lhs_SerialArray, *Lhs_SerialArray != x );

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
#ifndef INTARRAY
  // Since the Lhs is a different type than the Return Value it could not be reused!
     // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

#else
     if (lhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

        }
#endif

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & operator!=(intSerialArray,int)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator_Returning_IntArray ( Lhs , x ,
               MDI_i_NOT_EQ_Array_NOT_EQ_Scalar,
               MDI_i_NOT_EQ_Array_NOT_EQ_Scalar_Accumulate_To_Operand , intSerialArray::Scalar_NOT_EQ );
#endif
   }


intSerialArray &
operator!= ( int x , const intSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator!= (int,intSerialArray) for intSerialArray class!");

  // This is the only test we can do on the input!
     Rhs.Test_Consistency ("Test Rhs in operator!= (int,intSerialArray)");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs.displayReferenceCounts("Rhs in intSerialArray & operator!=(int,intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *Rhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Rhs_SerialArray     != NULL);

     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

#if defined(MEMORY_LEAK_TEST)
     printf ("Can't do MEMORY_LEAK_TEST in intSerialArray & operator!= ( int x , const intSerialArray & Rhs ) \n");
#endif
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Rhs, Rhs_SerialArray, x != *Rhs_SerialArray );

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
#ifndef INTARRAY
  // Since the Lhs is a different type than the Return Value it could not be reused!
     // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

#else
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }
#endif

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & operator!=(int,intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator_Returning_IntArray ( Rhs , x ,
               MDI_i_NOT_EQ_Scalar_NOT_EQ_Array,
               MDI_i_NOT_EQ_Scalar_NOT_EQ_Array_Accumulate_To_Operand , intSerialArray::Scalar_NOT_EQ );
#endif
   }


intSerialArray &
operator&& ( const intSerialArray & Lhs , const intSerialArray & Rhs )
   {   
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of operator&& (intSerialArray(id=%d),intSerialArray(id=%d)) for intSerialArray class!",
               Lhs.Array_ID(),Rhs.Array_ID());

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator&& (intSerialArray,intSerialArray)");
     Rhs.Test_Consistency ("Test Rhs in operator&& (intSerialArray,intSerialArray)");
#endif
 
  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Lhs.Test_Conformability (Rhs);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in intSerialArray & operator&&(intSerialArray,intSerialArray)");
          Rhs.displayReferenceCounts("Rhs in intSerialArray & operator&&(intSerialArray,intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *Lhs_SerialArray = NULL;
     intSerialArray *Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

  // printf ("Checking if WHERE statement is used before calling SerialArray::Parallel_Conformability_Enforcement from operator&& \n");

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
       // printf ("Checking if indirect addressing is used before calling SerialArray::Parallel_Conformability_Enforcement from operator&& \n");
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE)|| 
              (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
               Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE)|| 
              (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
               printf ("ERROR: can't mix indirect addressing with 2 arrays and where. \n");
               APP_ABORT();
             }
            else
             {
               Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray, Rhs, Rhs_SerialArray );
             }
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Lhs_SerialArray     != NULL);
     APP_ASSERT(Rhs_SerialArray     != NULL);
     APP_ASSERT(Temporary_Array_Set != NULL);

#if 0
     Lhs.displayReferenceCounts ("Lhs after PCE in operator&& (intSerialArray,intSerialArray)");
     Rhs.displayReferenceCounts ("Rhs after PCE in operator&& (intSerialArray,intSerialArray)");
     Lhs_SerialArray->displayReferenceCounts ("Lhs_SerialArray after PCE in operator&& (intSerialArray,intSerialArray)");
     Rhs_SerialArray->displayReferenceCounts ("Rhs_SerialArray after PCE in operator&& (intSerialArray,intSerialArray)");
#endif

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();
     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );
     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

#if defined(MEMORY_LEAK_TEST)
     printf ("Can't do MEMORY_LEAK_TEST in intSerialArray & operator&& ( const intSerialArray & Lhs , const intSerialArray & Rhs ) \n");
#endif
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, Lhs_SerialArray, Rhs_SerialArray, *Lhs_SerialArray && *Rhs_SerialArray );

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
#ifndef INTARRAY
  // Since the Lhs is a different type than the Return Value it could not be reused!
     // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;


  // Since the Rhs is a different type than the Return Value it could not be reused!
     // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

#else
     if (lhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

        }
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }
#endif

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray;
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in operator&& (intSerialArray,intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator_Returning_IntArray ( Lhs , Rhs ,
               MDI_i_AND_Array_AND_Array,
               MDI_i_AND_Array_AND_Array_Accumulate_To_Operand , intSerialArray::AND );
#endif
   }   


intSerialArray &
operator&& ( const intSerialArray & Lhs , int x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator&& (intSerialArray,int) for intSerialArray class!");

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator&& (intSerialArray,int)");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in intSerialArray & operator&&(intSerialArray,int)");
        }
#endif

#if defined(PPP)
     intSerialArray *Lhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Lhs_SerialArray != NULL);

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );

  // return intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, *Lhs_SerialArray && x );
#if defined(MEMORY_LEAK_TEST)
     puts ("Can't do MEMORY_LEAK_TEST in intSerialArray & operator&& ( const intSerialArray & Lhs , int x )");
#endif
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Lhs_SerialArray, *Lhs_SerialArray && x );

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
#ifndef INTARRAY
  // Since the Lhs is a different type than the Return Value it could not be reused!
     // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

#else
     if (lhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

        }
#endif

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & operator&&(intSerialArray,int)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator_Returning_IntArray ( Lhs , x ,
               MDI_i_AND_Array_AND_Scalar,
               MDI_i_AND_Array_AND_Scalar_Accumulate_To_Operand , intSerialArray::Scalar_AND );
#endif
   }


intSerialArray &
operator- ( int x , const intSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of operator- (int,intSerialArray) for intSerialArray class! \n");

  // This is the only test we can do on the input!
     Rhs.Test_Consistency ("Test Rhs in operator-");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs.displayReferenceCounts("Rhs in intSerialArray & operator-(int,intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray* Rhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE)
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      ( Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = 
	      intArray::Parallel_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }
     if (Temporary_Array_Set == NULL) Temporary_Array_Set =
	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Rhs_SerialArray != NULL);

     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = Rhs;
#else
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Rhs, Rhs_SerialArray, x - *Rhs_SerialArray );
  // return intArray::Abstract_Binary_Operator ( Temporary_Array_Set, Rhs, x - *Rhs_SerialArray );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & operator-(int,intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator ( Rhs , x , 
               MDI_i_Subtract_Scalar_Minus_Array,
               MDI_i_Subtract_Scalar_Minus_Array_Accumulate_To_Operand , intSerialArray::Scalar_Minus );
#endif
   }


intSerialArray &
operator&& ( int x , const intSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator&& (int,intSerialArray) for intSerialArray class!");

  // This is the only test we can do on the input!
     Rhs.Test_Consistency ("Test Rhs in operator&& (int,intSerialArray)");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs.displayReferenceCounts("Rhs in intSerialArray & operator&&(int,intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *Rhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Rhs_SerialArray     != NULL);

     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

#if defined(MEMORY_LEAK_TEST)
     printf ("Can't do MEMORY_LEAK_TEST in intSerialArray & operator&& ( int x , const intSerialArray & Rhs ) \n");
#endif
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Rhs, Rhs_SerialArray, x && *Rhs_SerialArray );

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
#ifndef INTARRAY
  // Since the Lhs is a different type than the Return Value it could not be reused!
     // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

#else
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }
#endif

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & operator&&(int,intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator_Returning_IntArray ( Rhs , x ,
               MDI_i_AND_Scalar_AND_Array,
               MDI_i_AND_Scalar_AND_Array_Accumulate_To_Operand , intSerialArray::Scalar_AND );
#endif
   }


intSerialArray &
operator|| ( const intSerialArray & Lhs , const intSerialArray & Rhs )
   {   
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of operator|| (intSerialArray(id=%d),intSerialArray(id=%d)) for intSerialArray class!",
               Lhs.Array_ID(),Rhs.Array_ID());

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator|| (intSerialArray,intSerialArray)");
     Rhs.Test_Consistency ("Test Rhs in operator|| (intSerialArray,intSerialArray)");
#endif
 
  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Lhs.Test_Conformability (Rhs);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in intSerialArray & operator||(intSerialArray,intSerialArray)");
          Rhs.displayReferenceCounts("Rhs in intSerialArray & operator||(intSerialArray,intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *Lhs_SerialArray = NULL;
     intSerialArray *Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

  // printf ("Checking if WHERE statement is used before calling SerialArray::Parallel_Conformability_Enforcement from operator|| \n");

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
       // printf ("Checking if indirect addressing is used before calling SerialArray::Parallel_Conformability_Enforcement from operator|| \n");
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE)|| 
              (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
               Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE)|| 
              (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
               printf ("ERROR: can't mix indirect addressing with 2 arrays and where. \n");
               APP_ABORT();
             }
            else
             {
               Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray, Rhs, Rhs_SerialArray );
             }
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Lhs_SerialArray     != NULL);
     APP_ASSERT(Rhs_SerialArray     != NULL);
     APP_ASSERT(Temporary_Array_Set != NULL);

#if 0
     Lhs.displayReferenceCounts ("Lhs after PCE in operator|| (intSerialArray,intSerialArray)");
     Rhs.displayReferenceCounts ("Rhs after PCE in operator|| (intSerialArray,intSerialArray)");
     Lhs_SerialArray->displayReferenceCounts ("Lhs_SerialArray after PCE in operator|| (intSerialArray,intSerialArray)");
     Rhs_SerialArray->displayReferenceCounts ("Rhs_SerialArray after PCE in operator|| (intSerialArray,intSerialArray)");
#endif

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();
     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );
     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

#if defined(MEMORY_LEAK_TEST)
     printf ("Can't do MEMORY_LEAK_TEST in intSerialArray & operator|| ( const intSerialArray & Lhs , const intSerialArray & Rhs ) \n");
#endif
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, Lhs_SerialArray, Rhs_SerialArray, *Lhs_SerialArray || *Rhs_SerialArray );

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
#ifndef INTARRAY
  // Since the Lhs is a different type than the Return Value it could not be reused!
     // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;


  // Since the Rhs is a different type than the Return Value it could not be reused!
     // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

#else
     if (lhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

        }
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }
#endif

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray;
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in operator|| (intSerialArray,intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator_Returning_IntArray ( Lhs , Rhs ,
               MDI_i_OR_Array_OR_Array,
               MDI_i_OR_Array_OR_Array_Accumulate_To_Operand , intSerialArray::OR );
#endif
   }   


intSerialArray &
operator|| ( const intSerialArray & Lhs , int x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator|| (intSerialArray,int) for intSerialArray class!");

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator|| (intSerialArray,int)");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in intSerialArray & operator||(intSerialArray,int)");
        }
#endif

#if defined(PPP)
     intSerialArray *Lhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Lhs_SerialArray != NULL);

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );

  // return intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, *Lhs_SerialArray || x );
#if defined(MEMORY_LEAK_TEST)
     puts ("Can't do MEMORY_LEAK_TEST in intSerialArray & operator|| ( const intSerialArray & Lhs , int x )");
#endif
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Lhs_SerialArray, *Lhs_SerialArray || x );

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
#ifndef INTARRAY
  // Since the Lhs is a different type than the Return Value it could not be reused!
     // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

#else
     if (lhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

        }
#endif

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & operator||(intSerialArray,int)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator_Returning_IntArray ( Lhs , x ,
               MDI_i_OR_Array_OR_Scalar,
               MDI_i_OR_Array_OR_Scalar_Accumulate_To_Operand , intSerialArray::Scalar_OR );
#endif
   }


intSerialArray &
operator|| ( int x , const intSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator|| (int,intSerialArray) for intSerialArray class!");

  // This is the only test we can do on the input!
     Rhs.Test_Consistency ("Test Rhs in operator|| (int,intSerialArray)");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs.displayReferenceCounts("Rhs in intSerialArray & operator||(int,intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *Rhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intSerialArray::Parallel_Indirect_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intSerialArray::Parallel_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Rhs_SerialArray     != NULL);

     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

#if defined(MEMORY_LEAK_TEST)
     printf ("Can't do MEMORY_LEAK_TEST in intSerialArray & operator|| ( int x , const intSerialArray & Rhs ) \n");
#endif
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Rhs, Rhs_SerialArray, x || *Rhs_SerialArray );

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
#ifndef INTARRAY
  // Since the Lhs is a different type than the Return Value it could not be reused!
     // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

#else
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }
#endif

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & operator||(int,intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator_Returning_IntArray ( Rhs , x ,
               MDI_i_OR_Scalar_OR_Array,
               MDI_i_OR_Scalar_OR_Array_Accumulate_To_Operand , intSerialArray::Scalar_OR );
#endif
   }


// Sum along axis friend function!
intSerialArray &
sum ( const intSerialArray & inputArray , int Axis )
{
/*
// ... (Bug Fix, kdb, 7/1/96) Code was previuosly hardwired for 4
//  dimensions, this has been changed to an arbitrary number ...
*/

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          inputArray.displayReferenceCounts("X in intSerialArray & sum (intSerialArray,int)");
        }
#endif

//  We want to preserve the interface being for cost array objects but we
// need to use a non-const representation to make this work with the current MDI layer
// so we will cast away cost to make this work.
   intSerialArray & X = (intSerialArray &) inputArray;

#if defined(MEMORY_LEAK_TEST)
   puts ("Can't do MEMORY_LEAK_TEST in intSerialArray & sum ( const intSerialArray & X , int Axis )");
#endif

//==============================================================
#if COMPILE_DEBUG_STATEMENTS
   if (APP_DEBUG > 0)
      puts ("Inside of intSerialArray & sum ( const intSerialArray & X , int Axis ) for intSerialArray class!");
#endif
//==============================================================

#if defined(PPP)
   // In P++ this function is only implemented for a single processor 
   // case (for now)!
   APP_ASSERT (Communication_Manager::Number_Of_Processors == 1);
#endif

//==============================================================
#if COMPILE_DEBUG_STATEMENTS
   // This is the only test we can do on the input!
   X.Test_Consistency ("Test X in sum (const intSerialArray & X, int Axis)");
#endif
//==============================================================

   // Build result array (it will be marked as a temporary before we 
   // return)
   intSerialArray* Result = NULL;

   if (X.Array_Descriptor.Array_Domain.Is_A_Temporary)
   {
      // We could reuse the temporary by taking a view of it (the view 
      // would be of one lower dimension) This avoids any 
      // initialization of the result and the return vaules are 
      // computed in place.

//==============================================================
#if COMPILE_DEBUG_STATEMENTS
      if (APP_DEBUG > 0) puts ("Input is a temporary!");
#endif
//==============================================================
 
      int Base   [MAX_ARRAY_DIMENSION];
      int Length [MAX_ARRAY_DIMENSION];
      int Stride [MAX_ARRAY_DIMENSION];

      int i;
      for (i=0;i<MAX_ARRAY_DIMENSION;i++)
      {
         Base[i]   = X.Array_Descriptor.Array_Domain.Base[i]+
            X.Array_Descriptor.Array_Domain.Data_Base[i];
         Length[i] = (X.Array_Descriptor.Array_Domain.Bound[i]-
            X.Array_Descriptor.Array_Domain.Base[i]) + 1;
         Stride[i] = X.Array_Descriptor.Array_Domain.Stride[i];
      }

      // Increment reference count of data we are building a view of!
      // This allows the temporary to be reused (thus accumulating the 
      // result into the temporary)
      // Note that the we have to delete the temporary at the end of 
      // the function in order to decrement the reference count on the 
      // data.  And since we reuse the temporary we have to increment 
      // (and then decrement) the base along the axis of sumation to 
      // avoid adding the first row to itself.  A nasty detail!

      // SerialArray_Descriptor_Type::Array_Reference_Count_Array 
      //    [X.Array_Descriptor.Array_Domain.Array_ID]++;

      X.incrementRawDataReferenceCount();

//==============================================================
#if COMPILE_DEBUG_STATEMENTS
      if (APP_DEBUG > 0)
        puts ("Do axis specific allocation of temporary!");
#endif
//==============================================================

      // Dimension the array with the specified Axis collapsed
      for (i=0; i<MAX_ARRAY_DIMENSION;i++)
         if (i == Axis)
	 {
	   Length[i] = 1;
	   Stride[i] = 1;
	 }

      Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type Index_List;
      Index Index_Array[MAX_ARRAY_DIMENSION];

      for (i=0; i<MAX_ARRAY_DIMENSION;i++)
      {
         Index_Array[i] = Index (Base[i],Length[i],Stride[i]);
         Index_List[i] = &Index_Array[i];
      }

#if defined(PPP)
      // Must build a view of X.SerialArray to make it consistant 
      // with the P++ descriptor

      // SerialArray_Descriptor_Type::Array_Reference_Count_Array 
      //   [X.SerialArray->Array_Descriptor.Array_Domain.Array_ID]++;

       X.incrementRawDataReferenceCount();
       intSerialArray *View = new intSerialArray 
         ( X.Array_Descriptor.SerialArray->Array_Descriptor.Array_Data , 
	   X.Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain,Index_List);
       Result = new intSerialArray 
         (View , X.Array_Descriptor.Array_Domain,Index_List);
#else
      Result = new intSerialArray 
         (X.Array_Descriptor.Array_Data , 
	  X.Array_Descriptor.Array_Domain,Index_List);
#endif

     // Modify the Input descriptor to avoid the sum of the first 
     // value along the chosen axis to itself. This allows the MDI 
     // function to accumulate the result into itself thus reusing 
     // the temporary.  But to make this work we have to skip the 
     // sumation of the
     // first row into itself.  A very nasty detail!
     // This should be a issue for any threaded computation (THREAD SAFETY ALERT)

     X.Array_Descriptor.Array_Domain.Base[Axis] += 
	X.Array_Descriptor.Array_Domain.Stride[Axis];

//==============================================================
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
        puts ("DONE with axis specific allocation of temporary!");
#endif
//==============================================================
   }
   else
   {
      Integer_Array_MAX_ARRAY_DIMENSION_Type Integer_List;
      int i;
      for (i=0;i<MAX_ARRAY_DIMENSION;i++)
         Integer_List[i] = X.getLength (i);

      Integer_List[Axis] = 1;

      // Input is not a temporary so we have to build a return array (of 
      // the correct dimensions)
      // Dimension the array with the specified Axis collapsed

      Integer_List[Axis] = 1;
      Result = new intSerialArray (Integer_List);
   }

#if defined(PPP)
  // Skip this for now since it is sort of complex
  // puts ("P++ sum function (sum along an axis) not completely implemented yet (this function is more complex in P++ and will be done last)!");
  // APP_ABORT();
  // Use avoid compiler warning
  // int Avoid_Compiler_Warning = Axis;
  // Use avoid compiler warning
  // return (intSerialArray &) X;

  // This could be more efficent!
     APP_ASSERT(Result != NULL);
     APP_ASSERT(Result->Array_Descriptor.SerialArray != NULL);
     //APP_ASSERT(Result->Array_Descriptor != NULL);
     APP_ASSERT(X.Array_Descriptor.SerialArray != NULL);
     //APP_ASSERT(X.Array_Descriptor != NULL);

  // Result->view("Result->view (BEFORE SUM)");
  // Result->SerialArray->view("Result->SerialArray->view");
  // if (X.Array_Descriptor.Array_Domain.Is_A_Temporary == TRUE)
  //      *(Result->SerialArray) = sum ( *(X.SerialArray) , Axis );

  // puts ("Call intSerialArray sum along axis function!");
  // Mark as a NON-temporary to avoid temporary handling which would absorb the temporary
  // This could be done more efficently by testing for a temporary and skipping the
  // call to the assignment operator! Later!
  // Result->Array_Descriptor.Array_Domain.Is_A_Temporary = FALSE;
     Result->Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain.
	Is_A_Temporary = FALSE;

     APP_ASSERT(Result->Array_Descriptor.SerialArray->Array_Descriptor.
		Array_Domain.Is_A_Temporary == FALSE);
     *(Result->Array_Descriptor.SerialArray) = 
	sum ( *(X.Array_Descriptor.SerialArray) , Axis );
  // Result->view("Result->view (AFTER SUM)");

  // puts ("Returning from intArray sum along axis!");

  // Mark as a temporary
     Result->Array_Descriptor.Array_Domain.Is_A_Temporary = TRUE;
     Result->Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain.
	Is_A_Temporary = TRUE;

#if COMPILE_DEBUG_STATEMENTS
  // This is the only test we can do on the input!
     Result->Test_Consistency ("Test Result in sum (const intSerialArray & X, int Axis)");
#endif

     return *Result;
  // End of P++ code
#else
  // Start of A++ code

  // Variables to hold data obtainted from inlined access functions
     int *Mask_Array_Data   = NULL;
     array_domain *Mask_Descriptor   = NULL;

  // Check for Where Mask
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
          puts ("In sum(intiArray,int): Where Mask usage not implemented (it is largely meaningless in this case)!");
          APP_ABORT();
       // Mask_Array_Data = Where_Statement_Support::Where_Statement_Mask->Array_Data;
       // Mask_Descriptor = (int*) Where_Statement_Support::Where_Statement_Mask->Array_Descriptor;
        }

     APP_ASSERT(Result->Array_Descriptor.Array_Data       != NULL);
     APP_ASSERT(X.Array_Descriptor.Array_Data             != NULL);
     //APP_ASSERT(Result->Array_Descriptor                != NULL);
     //APP_ASSERT(X.Array_Descriptor                      != NULL);

  // If the input was not a temporary then a temporary was created (which must be initialized to zero)
     if (X.Array_Descriptor.Array_Domain.Is_A_Temporary == FALSE)
        {
       // Initialize the just allocated temporary to ZERO.  Because this could be a 3D array
       // (one dimension less than the input) this operation is not trivial.  To simplify this
       // we call the MDI function for assignment of a scalar toan array directly.  This would be 
       // more efficient than calling the A++ operator= though this function could be made more
       // efficient if we were to do the initialization in the MDI_i_Sum_Array_Along_Axis function
       // but that would be more complex so I will skip that for now.
          MDI_i_Assign_Array_Equals_Scalar_Accumulate_To_Operand 
	     ( Result->Array_Descriptor.Array_Data , 0 , Mask_Array_Data ,
              (array_domain*) &(Result->Array_Descriptor.Array_Domain) , 
	      Mask_Descriptor );
        }

  // Hand off to the MDI layer for more efficent computation
     MDI_i_Sum_Array_Along_Axis 
	( Axis, Result->Array_Descriptor.Array_Data, X.Array_Descriptor.Array_Data, 
	  Mask_Array_Data, (array_domain*) &(Result->Array_Descriptor.Array_Domain), 
	  (array_domain*) &(X.Array_Descriptor.Array_Domain) , Mask_Descriptor );

  // Bug fix (9/11/94) we reuse the temporary!
  // Delete the input array object if it was a temporary
     if (X.Array_Descriptor.Array_Domain.Is_A_Temporary == TRUE)
        {
       // Above the descriptor was modified to avoid sumation of the first element along
       // the choosen axis with itself.  So now we have to undo it just to make sure that
       // even if the descriptor is referenced somewhere else we have have returned it to
       // its correct state.  I guess we could avoid the fixup if the descriptor's reference
       // count implied it had no additional references but this is more elegant.
          X.Array_Descriptor.Array_Domain.Base[Axis] =- X.Array_Descriptor.Array_Domain.Stride[Axis];

       // Now we have to delete the input since it was a temporary 
       // (this is part of the temporary management that A++ does)
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          X.decrementReferenceCount();
          if (X.getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete &((intSerialArray &) X);
        }

  // puts ("Returning from intSerialArray sum along axis!");

  // Mark as a temporary
     Result->Array_Descriptor.Array_Domain.Is_A_Temporary = TRUE;

#if COMPILE_DEBUG_STATEMENTS
  // This is the only test we can do on the input!
     Result->Test_Consistency ("Test Result in sum (const intSerialArray & X, int Axis)");
#endif

     return *Result;
#endif
   }


#ifndef INTARRAY
intSerialArray &
atan2 ( const intSerialArray & Lhs , const intSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of atan2 (intSerialArray,intSerialArray) for intSerialArray class! Lhs:rc=%d Rhs:rc=%d ",
               Lhs.getRawDataReferenceCount(),Rhs.getRawDataReferenceCount());

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in atan2");
     Rhs.Test_Consistency ("Test Rhs in atan2");
#endif
 
  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Lhs.Test_Conformability (Rhs);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in intSerialArray & atan2(intSerialArray,intSerialArray)");
          Rhs.displayReferenceCounts("Rhs in intSerialArray & atan2(intSerialArray,intSerialArray)");
        }
#endif

#if defined(PPP)
  // Pointers to views of Lhs and Rhs serial arrays which allow a conformable operation
     intSerialArray *Lhs_SerialArray = NULL;
     intSerialArray *Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...
     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	      (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
               Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement (Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement (Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	      (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
	       puts ("ERROR: can't mix indirect addressing with 2 arrays and where.");
               APP_ABORT();
             }
            else
             {
               Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
                    (Lhs, Lhs_SerialArray, 
                    *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray,
                     Rhs, Rhs_SerialArray );
             }

          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

  // New test (8/5/2000)
     APP_ASSERT(Temporary_Array_Set != NULL);
  // Temporary_Array_Set->display("Check to see what sort of communication model was used");

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Lhs_SerialArray != NULL);
     APP_ASSERT(Rhs_SerialArray != NULL);

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();
     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );
     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in intSerialArray & atan2(intSerialArray,intSerialArray)");
          Rhs_SerialArray->displayReferenceCounts("Rhs_SerialArray in intSerialArray & atan2(intSerialArray,intSerialArray)");
        }
#endif

  // Inputs to intArray::Abstract_Binary_Operator:
  //     1. Temporary_Array_Set is attached to the intArray temporary returned by Abstract_Binary_Operator
  //     2. Lhs is used to get the Lhs partition information (PARTI parallel descriptor) and array reuse
  //     3. Rhs is used to get the Rhs partition information (PARTI parallel descriptor) in case the Lhs was 
  //        a NULL array (no data and no defined partitioning (i.e. no PARTI parallel descriptor)) and array reuse
  //     4. The intSerialArray which is to be put into the intArray temporary returned by Abstract_Binary_Operator
  // return intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, atan2(*Lhs_SerialArray,*Rhs_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = Rhs;
#else
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, Lhs_SerialArray, Rhs_SerialArray, atan2(*Lhs_SerialArray,*Rhs_SerialArray) );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
  // if (Lhs_SerialArray != Return_Value.getSerialArrayPointer())
     if (lhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

        }
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & atan2(intSerialArray,intSerialArray)");
        }
#endif

     return Return_Value;
#else
  // This function can be either Abstract_Binary_Operator or Abstract_Binary_Operator_Non_Commutative
     return intSerialArray::Abstract_Binary_Operator_Non_Commutative ( Lhs , Rhs , MDI_i_Arc_Tan2_Array_ArcTan2_Array, MDI_i_Arc_Tan2_Array_ArcTan2_Array_Accumulate_To_Operand , intSerialArray::atan2_Function );
#endif
   }



intSerialArray &
atan2 ( int x , const intSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("\n\n\nInside of atan2 (int,intSerialArray) for intSerialArray class! \n");

  // This is the only test we can do on the input!
     Rhs.Test_Consistency ("Test Rhs in atan2");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs.displayReferenceCounts("Rhs in intSerialArray & atan2(int,intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray *Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...
     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement
              (Rhs, Rhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}

        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Rhs_SerialArray     != NULL);

     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs_SerialArray->displayReferenceCounts("Rhs_SerialArray in intSerialArray & atan2(int,intSerialArray)");
        }
#endif

  // return intArray::Abstract_Operator ( Temporary_Array_Set, Rhs, atan2(x,*Rhs_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = Rhs;
#else
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Rhs, Rhs_SerialArray, atan2(x,*Rhs_SerialArray) );
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value (before delete macro) in intSerialArray & atan2(int,intSerialArray)");
        }
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & atan2(int,intSerialArray)");
        }
#endif

     return Return_Value;
#else
  // This function can be either Abstract_Binary_Operator or Abstract_Binary_Operator_Non_Commutative
     return intSerialArray::Abstract_Binary_Operator_Non_Commutative ( Rhs , x ,
               MDI_i_Arc_Tan2_Scalar_ArcTan2_Array,
               MDI_i_Arc_Tan2_Scalar_ArcTan2_Array_Accumulate_To_Operand , intSerialArray::Scalar_atan2_Function );
#endif
   }



intSerialArray &
atan2 ( const intSerialArray & Lhs , int x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of atan2 (intSerialArray,int) for intSerialArray class!");

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in atan2");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts 
          Lhs.displayReferenceCounts("Lhs in intSerialArray & atan2(intSerialArray,int)");
        }
#endif

#if defined(PPP)
  // Pointers to views of Lhs serial arrays which allow a conformable operation
     intSerialArray *Lhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE)
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }
     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Lhs_SerialArray != NULL);

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in intSerialArray & atan2(intSerialArray,int)");
        }
#endif

  // return intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, atan2(*Lhs_SerialArray,x) );
#if defined(MEMORY_LEAK_TEST)
     puts ("Can't do MEMORY_LEAK_TEST in intSerialArray & atan2 ( const intSerialArray & Lhs , int x )");
#endif
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Lhs_SerialArray, atan2(*Lhs_SerialArray,x) );

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (lhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray;
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & atan2(intSerialArray,int)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator ( Lhs , x ,
               MDI_i_Arc_Tan2_Array_ArcTan2_Scalar,
               MDI_i_Arc_Tan2_Array_ArcTan2_Scalar_Accumulate_To_Operand , intSerialArray::Scalar_atan2_Function );
#endif
   }

#endif

#ifdef INTARRAY
intSerialArray &
operator& ( const intSerialArray & Lhs , const intSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
        {
          printf ("\n\n\n### Inside of operator& (intSerialArray,intSerialArray) for intSerialArray class: (id=%d) = (id=%d) \n",
               Lhs.Array_ID(),Rhs.Array_ID());
        }

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator&");
     Rhs.Test_Consistency ("Test Rhs in operator&");
#endif

  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Lhs.Test_Conformability (Rhs);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          printf ("intSerialArray: Lhs.isTemporary() = %s \n",(Lhs.isTemporary()) ? "TRUE" : "FALSE");
          Lhs.displayReferenceCounts("Lhs in intSerialArray & operator&(intSerialArray,intSerialArray)");
          printf ("intSerialArray: Rhs.isTemporary() = %s \n",(Rhs.isTemporary()) ? "TRUE" : "FALSE");
          Rhs.displayReferenceCounts("Rhs in intSerialArray & operator&(intSerialArray,intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray* Lhs_SerialArray = NULL;
     intSerialArray* Rhs_SerialArray = NULL;

     intSerialArray* Mask_SerialArray = NULL;
     intSerialArray* Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	      (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
               Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
          if ( (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
               (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) )
             {
               printf ("Sorry, not implemented: can't mix indirect addressing using where statements and two array (binary) operators!\n");
               APP_ABORT();
             }
            else
             {
               Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement
                  (Lhs, Lhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray, Rhs, Rhs_SerialArray );
             }
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          printf ("intSerialArray: Lhs_SerialArray->isTemporary() = %s \n",(Lhs_SerialArray->isTemporary()) ? "TRUE" : "FALSE");
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in intSerialArray & operator&(intSerialArray,intSerialArray)");
          printf ("intSerialArray: Rhs_SerialArray->isTemporary() = %s \n",(Rhs_SerialArray->isTemporary()) ? "TRUE" : "FALSE");
          Rhs_SerialArray->displayReferenceCounts("Rhs_SerialArray in intSerialArray & operator&(intSerialArray,intSerialArray)");
        }
#endif

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Lhs_SerialArray != NULL);
     APP_ASSERT(Rhs_SerialArray != NULL);

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();
     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );
     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

  // return intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, *Lhs_SerialArray & *Rhs_SerialArray );
#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = Lhs;
#else
     intArray & Return_Value = intArray::Abstract_Operator 
	( Temporary_Array_Set, Lhs, Rhs, Lhs_SerialArray, Rhs_SerialArray, *Lhs_SerialArray & *Rhs_SerialArray );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (lhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

        }
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }

     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & operator&(intSerialArray,intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator ( Lhs , Rhs , 
               MDI_i_BIT_AND_Array_BitwiseAND_Array,
               MDI_i_BIT_AND_Array_BitwiseAND_Array_Accumulate_To_Operand , intSerialArray::BitwiseAND );
#endif
   }

intSerialArray &
operator& ( const intSerialArray & Lhs , int x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("\n\n\n### Inside of operator& (intSerialArray,int) for intSerialArray class: (id=%d) = scalar \n",Lhs.Array_ID());

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator&");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in intSerialArray & operator&(intSerialArray,int)");
        }
#endif

#if defined(PPP)
     intSerialArray* Lhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
               Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
               APP_ASSERT(Lhs_SerialArray != NULL);
            // Lhs_SerialArray->displayReferenceCounts("AFTER PCE: *Lhs_SerialArray in intSerialArray & operator&(intSerialArray,int)");
             }
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
          if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
               Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
                    (Lhs, Lhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
             }
            else
             {
               Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
                    (Lhs, Lhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
             }
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          printf ("intSerialArray: Lhs_SerialArray->isTemporary() = %s \n",(Lhs_SerialArray->isTemporary()) ? "TRUE" : "FALSE");
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in intSerialArray & operator&(intSerialArray,int)");
        }
#endif

  // (11/27/2000) Added error checking (will not work with indirect addessing later!!!)
     APP_ASSERT(Temporary_Array_Set != NULL);

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Lhs_SerialArray != NULL);

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );

#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = Lhs;
#else
     intArray & Return_Value = intArray::Abstract_Operator(Temporary_Array_Set,Lhs,Lhs_SerialArray,*Lhs_SerialArray & x);
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (lhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in in intSerialArray & operator&(intSerialArray,int)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator ( Lhs , x , 
               MDI_i_BIT_AND_Array_BitwiseAND_Scalar,
               MDI_i_BIT_AND_Array_BitwiseAND_Scalar_Accumulate_To_Operand , intSerialArray::Scalar_BitwiseAND );
#endif
   }

intSerialArray &
operator& ( int x , const intSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator& (int,intSerialArray) for intSerialArray class!");

  // This is the only test we can do on the input!
     Rhs.Test_Consistency ("Test Rhs in operator&");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs.displayReferenceCounts("Rhs in intSerialArray & operator&(int,intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray* Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      ( Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = 
	      intArray::Parallel_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }
     if (Temporary_Array_Set == NULL) Temporary_Array_Set =
	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Rhs_SerialArray != NULL);

     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = Rhs;
#else
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Rhs, Rhs_SerialArray, x & *Rhs_SerialArray );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & operator&(int,intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator ( Rhs , x , 
               MDI_i_BIT_AND_Scalar_BitwiseAND_Array,
               MDI_i_BIT_AND_Scalar_BitwiseAND_Array_Accumulate_To_Operand , intSerialArray::Scalar_BitwiseAND );
#endif
   }

intSerialArray &
intSerialArray::operator&= ( int x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of intSerialArray::operator&= (int) for intSerialArray class!");

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in intSerialArray::operator&=");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS in intSerialArray & intSerialArray::operator&=(int)");
        }
#endif

#if defined(PPP)
     intSerialArray *This_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( *this , This_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( *this , This_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (*this, This_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (*this, This_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set =	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(This_SerialArray != NULL);

     bool thisIsTemporary = This_SerialArray->isTemporary();

     APP_ASSERT ( (thisIsTemporary == TRUE) || (thisIsTemporary == FALSE) );

#if !defined(MEMORY_LEAK_TEST)
     intArray::Abstract_Modification_Operator ( Temporary_Array_Set, *this, This_SerialArray, *This_SerialArray &= x );
     // ... don't need to use macro because Return_Value won't be Mask ...
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (thisIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): This_SerialArray->getReferenceCount() = %d \n",
       //      This_SerialArray->getReferenceCount());

       // Must delete the This_SerialArray if it was taken directly from the This array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (This_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          This_SerialArray->decrementReferenceCount();
          if (This_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete This_SerialArray;
             }
          This_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;

#else
     intSerialArray::Abstract_Operator_Operation_Equals ( *this , x ,
        MDI_i_BIT_AND_Array_BitwiseAND_Scalar_Accumulate_To_Operand , intSerialArray::Scalar_BitwiseAND_Equals );
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS (return value) in intSerialArray & intSerialArray::operator&=(int)");
        }
#endif

     return *this;
   }

#endif

#ifdef INTARRAY
intSerialArray &
operator| ( const intSerialArray & Lhs , const intSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
        {
          printf ("\n\n\n### Inside of operator| (intSerialArray,intSerialArray) for intSerialArray class: (id=%d) = (id=%d) \n",
               Lhs.Array_ID(),Rhs.Array_ID());
        }

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator|");
     Rhs.Test_Consistency ("Test Rhs in operator|");
#endif

  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Lhs.Test_Conformability (Rhs);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          printf ("intSerialArray: Lhs.isTemporary() = %s \n",(Lhs.isTemporary()) ? "TRUE" : "FALSE");
          Lhs.displayReferenceCounts("Lhs in intSerialArray & operator|(intSerialArray,intSerialArray)");
          printf ("intSerialArray: Rhs.isTemporary() = %s \n",(Rhs.isTemporary()) ? "TRUE" : "FALSE");
          Rhs.displayReferenceCounts("Rhs in intSerialArray & operator|(intSerialArray,intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray* Lhs_SerialArray = NULL;
     intSerialArray* Rhs_SerialArray = NULL;

     intSerialArray* Mask_SerialArray = NULL;
     intSerialArray* Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	      (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
               Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
          if ( (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
               (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) )
             {
               printf ("Sorry, not implemented: can't mix indirect addressing using where statements and two array (binary) operators!\n");
               APP_ABORT();
             }
            else
             {
               Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement
                  (Lhs, Lhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray, Rhs, Rhs_SerialArray );
             }
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          printf ("intSerialArray: Lhs_SerialArray->isTemporary() = %s \n",(Lhs_SerialArray->isTemporary()) ? "TRUE" : "FALSE");
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in intSerialArray & operator|(intSerialArray,intSerialArray)");
          printf ("intSerialArray: Rhs_SerialArray->isTemporary() = %s \n",(Rhs_SerialArray->isTemporary()) ? "TRUE" : "FALSE");
          Rhs_SerialArray->displayReferenceCounts("Rhs_SerialArray in intSerialArray & operator|(intSerialArray,intSerialArray)");
        }
#endif

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Lhs_SerialArray != NULL);
     APP_ASSERT(Rhs_SerialArray != NULL);

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();
     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );
     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

  // return intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, *Lhs_SerialArray | *Rhs_SerialArray );
#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = Lhs;
#else
     intArray & Return_Value = intArray::Abstract_Operator 
	( Temporary_Array_Set, Lhs, Rhs, Lhs_SerialArray, Rhs_SerialArray, *Lhs_SerialArray | *Rhs_SerialArray );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (lhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

        }
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }

     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & operator|(intSerialArray,intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator ( Lhs , Rhs , 
               MDI_i_BIT_OR_Array_BitwiseOR_Array,
               MDI_i_BIT_OR_Array_BitwiseOR_Array_Accumulate_To_Operand , intSerialArray::BitwiseOR );
#endif
   }

intSerialArray &
operator| ( const intSerialArray & Lhs , int x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("\n\n\n### Inside of operator| (intSerialArray,int) for intSerialArray class: (id=%d) = scalar \n",Lhs.Array_ID());

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator|");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in intSerialArray & operator|(intSerialArray,int)");
        }
#endif

#if defined(PPP)
     intSerialArray* Lhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
               Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
               APP_ASSERT(Lhs_SerialArray != NULL);
            // Lhs_SerialArray->displayReferenceCounts("AFTER PCE: *Lhs_SerialArray in intSerialArray & operator|(intSerialArray,int)");
             }
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
          if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
               Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
                    (Lhs, Lhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
             }
            else
             {
               Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
                    (Lhs, Lhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
             }
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          printf ("intSerialArray: Lhs_SerialArray->isTemporary() = %s \n",(Lhs_SerialArray->isTemporary()) ? "TRUE" : "FALSE");
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in intSerialArray & operator|(intSerialArray,int)");
        }
#endif

  // (11/27/2000) Added error checking (will not work with indirect addessing later!!!)
     APP_ASSERT(Temporary_Array_Set != NULL);

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Lhs_SerialArray != NULL);

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );

#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = Lhs;
#else
     intArray & Return_Value = intArray::Abstract_Operator(Temporary_Array_Set,Lhs,Lhs_SerialArray,*Lhs_SerialArray | x);
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (lhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in in intSerialArray & operator|(intSerialArray,int)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator ( Lhs , x , 
               MDI_i_BIT_OR_Array_BitwiseOR_Scalar,
               MDI_i_BIT_OR_Array_BitwiseOR_Scalar_Accumulate_To_Operand , intSerialArray::Scalar_BitwiseOR );
#endif
   }

intSerialArray &
operator| ( int x , const intSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator| (int,intSerialArray) for intSerialArray class!");

  // This is the only test we can do on the input!
     Rhs.Test_Consistency ("Test Rhs in operator|");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs.displayReferenceCounts("Rhs in intSerialArray & operator|(int,intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray* Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      ( Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = 
	      intArray::Parallel_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }
     if (Temporary_Array_Set == NULL) Temporary_Array_Set =
	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Rhs_SerialArray != NULL);

     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = Rhs;
#else
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Rhs, Rhs_SerialArray, x | *Rhs_SerialArray );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & operator|(int,intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator ( Rhs , x , 
               MDI_i_BIT_OR_Scalar_BitwiseOR_Array,
               MDI_i_BIT_OR_Scalar_BitwiseOR_Array_Accumulate_To_Operand , intSerialArray::Scalar_BitwiseOR );
#endif
   }

intSerialArray &
intSerialArray::operator|= ( int x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of intSerialArray::operator|= (int) for intSerialArray class!");

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in intSerialArray::operator|=");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS in intSerialArray & intSerialArray::operator|=(int)");
        }
#endif

#if defined(PPP)
     intSerialArray *This_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( *this , This_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( *this , This_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (*this, This_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (*this, This_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set =	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(This_SerialArray != NULL);

     bool thisIsTemporary = This_SerialArray->isTemporary();

     APP_ASSERT ( (thisIsTemporary == TRUE) || (thisIsTemporary == FALSE) );

#if !defined(MEMORY_LEAK_TEST)
     intArray::Abstract_Modification_Operator ( Temporary_Array_Set, *this, This_SerialArray, *This_SerialArray |= x );
     // ... don't need to use macro because Return_Value won't be Mask ...
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (thisIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): This_SerialArray->getReferenceCount() = %d \n",
       //      This_SerialArray->getReferenceCount());

       // Must delete the This_SerialArray if it was taken directly from the This array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (This_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          This_SerialArray->decrementReferenceCount();
          if (This_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete This_SerialArray;
             }
          This_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;

#else
     intSerialArray::Abstract_Operator_Operation_Equals ( *this , x ,
        MDI_i_BIT_OR_Array_BitwiseOR_Scalar_Accumulate_To_Operand , intSerialArray::Scalar_BitwiseOR_Equals );
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS (return value) in intSerialArray & intSerialArray::operator|=(int)");
        }
#endif

     return *this;
   }

#endif

#ifdef INTARRAY
intSerialArray &
operator^ ( const intSerialArray & Lhs , const intSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
        {
          printf ("\n\n\n### Inside of operator^ (intSerialArray,intSerialArray) for intSerialArray class: (id=%d) = (id=%d) \n",
               Lhs.Array_ID(),Rhs.Array_ID());
        }

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator^");
     Rhs.Test_Consistency ("Test Rhs in operator^");
#endif

  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Lhs.Test_Conformability (Rhs);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          printf ("intSerialArray: Lhs.isTemporary() = %s \n",(Lhs.isTemporary()) ? "TRUE" : "FALSE");
          Lhs.displayReferenceCounts("Lhs in intSerialArray & operator^(intSerialArray,intSerialArray)");
          printf ("intSerialArray: Rhs.isTemporary() = %s \n",(Rhs.isTemporary()) ? "TRUE" : "FALSE");
          Rhs.displayReferenceCounts("Rhs in intSerialArray & operator^(intSerialArray,intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray* Lhs_SerialArray = NULL;
     intSerialArray* Rhs_SerialArray = NULL;

     intSerialArray* Mask_SerialArray = NULL;
     intSerialArray* Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	      (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
               Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
          if ( (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
               (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) )
             {
               printf ("Sorry, not implemented: can't mix indirect addressing using where statements and two array (binary) operators!\n");
               APP_ABORT();
             }
            else
             {
               Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement
                  (Lhs, Lhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray, Rhs, Rhs_SerialArray );
             }
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          printf ("intSerialArray: Lhs_SerialArray->isTemporary() = %s \n",(Lhs_SerialArray->isTemporary()) ? "TRUE" : "FALSE");
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in intSerialArray & operator^(intSerialArray,intSerialArray)");
          printf ("intSerialArray: Rhs_SerialArray->isTemporary() = %s \n",(Rhs_SerialArray->isTemporary()) ? "TRUE" : "FALSE");
          Rhs_SerialArray->displayReferenceCounts("Rhs_SerialArray in intSerialArray & operator^(intSerialArray,intSerialArray)");
        }
#endif

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Lhs_SerialArray != NULL);
     APP_ASSERT(Rhs_SerialArray != NULL);

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();
     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );
     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

  // return intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, *Lhs_SerialArray ^ *Rhs_SerialArray );
#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = Lhs;
#else
     intArray & Return_Value = intArray::Abstract_Operator 
	( Temporary_Array_Set, Lhs, Rhs, Lhs_SerialArray, Rhs_SerialArray, *Lhs_SerialArray ^ *Rhs_SerialArray );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (lhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

        }
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }

     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & operator^(intSerialArray,intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator ( Lhs , Rhs , 
               MDI_i_BIT_XOR_Array_BitwiseXOR_Array,
               MDI_i_BIT_XOR_Array_BitwiseXOR_Array_Accumulate_To_Operand , intSerialArray::BitwiseXOR );
#endif
   }

intSerialArray &
operator^ ( const intSerialArray & Lhs , int x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("\n\n\n### Inside of operator^ (intSerialArray,int) for intSerialArray class: (id=%d) = scalar \n",Lhs.Array_ID());

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator^");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in intSerialArray & operator^(intSerialArray,int)");
        }
#endif

#if defined(PPP)
     intSerialArray* Lhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
               Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
               APP_ASSERT(Lhs_SerialArray != NULL);
            // Lhs_SerialArray->displayReferenceCounts("AFTER PCE: *Lhs_SerialArray in intSerialArray & operator^(intSerialArray,int)");
             }
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
          if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
               Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
                    (Lhs, Lhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
             }
            else
             {
               Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
                    (Lhs, Lhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
             }
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          printf ("intSerialArray: Lhs_SerialArray->isTemporary() = %s \n",(Lhs_SerialArray->isTemporary()) ? "TRUE" : "FALSE");
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in intSerialArray & operator^(intSerialArray,int)");
        }
#endif

  // (11/27/2000) Added error checking (will not work with indirect addessing later!!!)
     APP_ASSERT(Temporary_Array_Set != NULL);

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Lhs_SerialArray != NULL);

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );

#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = Lhs;
#else
     intArray & Return_Value = intArray::Abstract_Operator(Temporary_Array_Set,Lhs,Lhs_SerialArray,*Lhs_SerialArray ^ x);
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (lhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in in intSerialArray & operator^(intSerialArray,int)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator ( Lhs , x , 
               MDI_i_BIT_XOR_Array_BitwiseXOR_Scalar,
               MDI_i_BIT_XOR_Array_BitwiseXOR_Scalar_Accumulate_To_Operand , intSerialArray::Scalar_BitwiseXOR );
#endif
   }

intSerialArray &
operator^ ( int x , const intSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator^ (int,intSerialArray) for intSerialArray class!");

  // This is the only test we can do on the input!
     Rhs.Test_Consistency ("Test Rhs in operator^");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs.displayReferenceCounts("Rhs in intSerialArray & operator^(int,intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray* Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      ( Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = 
	      intArray::Parallel_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }
     if (Temporary_Array_Set == NULL) Temporary_Array_Set =
	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Rhs_SerialArray != NULL);

     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = Rhs;
#else
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Rhs, Rhs_SerialArray, x ^ *Rhs_SerialArray );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & operator^(int,intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator ( Rhs , x , 
               MDI_i_BIT_XOR_Scalar_BitwiseXOR_Array,
               MDI_i_BIT_XOR_Scalar_BitwiseXOR_Array_Accumulate_To_Operand , intSerialArray::Scalar_BitwiseXOR );
#endif
   }

intSerialArray &
intSerialArray::operator^= ( int x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of intSerialArray::operator^= (int) for intSerialArray class!");

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in intSerialArray::operator^=");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS in intSerialArray & intSerialArray::operator^=(int)");
        }
#endif

#if defined(PPP)
     intSerialArray *This_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( *this , This_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( *this , This_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (*this, This_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (*this, This_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set =	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(This_SerialArray != NULL);

     bool thisIsTemporary = This_SerialArray->isTemporary();

     APP_ASSERT ( (thisIsTemporary == TRUE) || (thisIsTemporary == FALSE) );

#if !defined(MEMORY_LEAK_TEST)
     intArray::Abstract_Modification_Operator ( Temporary_Array_Set, *this, This_SerialArray, *This_SerialArray ^= x );
     // ... don't need to use macro because Return_Value won't be Mask ...
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (thisIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): This_SerialArray->getReferenceCount() = %d \n",
       //      This_SerialArray->getReferenceCount());

       // Must delete the This_SerialArray if it was taken directly from the This array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (This_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          This_SerialArray->decrementReferenceCount();
          if (This_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete This_SerialArray;
             }
          This_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;

#else
     intSerialArray::Abstract_Operator_Operation_Equals ( *this , x ,
        MDI_i_BIT_XOR_Array_BitwiseXOR_Scalar_Accumulate_To_Operand , intSerialArray::Scalar_BitwiseXOR_Equals );
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS (return value) in intSerialArray & intSerialArray::operator^=(int)");
        }
#endif

     return *this;
   }

#endif

#ifdef INTARRAY
/* There is no <<= operator and so the << must be handled as a special case -- skip it for now */
intSerialArray &
operator<< ( const intSerialArray & Lhs , const intSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
        {
          printf ("\n\n\n### Inside of operator<< (intSerialArray,intSerialArray) for intSerialArray class: (id=%d) = (id=%d) \n",
               Lhs.Array_ID(),Rhs.Array_ID());
        }

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator<<");
     Rhs.Test_Consistency ("Test Rhs in operator<<");
#endif

  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Lhs.Test_Conformability (Rhs);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          printf ("intSerialArray: Lhs.isTemporary() = %s \n",(Lhs.isTemporary()) ? "TRUE" : "FALSE");
          Lhs.displayReferenceCounts("Lhs in intSerialArray & operator<<(intSerialArray,intSerialArray)");
          printf ("intSerialArray: Rhs.isTemporary() = %s \n",(Rhs.isTemporary()) ? "TRUE" : "FALSE");
          Rhs.displayReferenceCounts("Rhs in intSerialArray & operator<<(intSerialArray,intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray* Lhs_SerialArray = NULL;
     intSerialArray* Rhs_SerialArray = NULL;

     intSerialArray* Mask_SerialArray = NULL;
     intSerialArray* Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	      (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
               Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
          if ( (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
               (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) )
             {
               printf ("Sorry, not implemented: can't mix indirect addressing using where statements and two array (binary) operators!\n");
               APP_ABORT();
             }
            else
             {
               Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement
                  (Lhs, Lhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray, Rhs, Rhs_SerialArray );
             }
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          printf ("intSerialArray: Lhs_SerialArray->isTemporary() = %s \n",(Lhs_SerialArray->isTemporary()) ? "TRUE" : "FALSE");
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in intSerialArray & operator<<(intSerialArray,intSerialArray)");
          printf ("intSerialArray: Rhs_SerialArray->isTemporary() = %s \n",(Rhs_SerialArray->isTemporary()) ? "TRUE" : "FALSE");
          Rhs_SerialArray->displayReferenceCounts("Rhs_SerialArray in intSerialArray & operator<<(intSerialArray,intSerialArray)");
        }
#endif

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Lhs_SerialArray != NULL);
     APP_ASSERT(Rhs_SerialArray != NULL);

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();
     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );
     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

  // return intArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, *Lhs_SerialArray << *Rhs_SerialArray );
#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = Lhs;
#else
     intArray & Return_Value = intArray::Abstract_Operator 
	( Temporary_Array_Set, Lhs, Rhs, Lhs_SerialArray, Rhs_SerialArray, *Lhs_SerialArray << *Rhs_SerialArray );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (lhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

        }
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }

     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & operator<<(intSerialArray,intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator ( Lhs , Rhs , 
               MDI_i_BIT_LSHIFT_Array_BitwiseLShift_Array,
               MDI_i_BIT_LSHIFT_Array_BitwiseLShift_Array_Accumulate_To_Operand , intSerialArray::BitwiseLShift );
#endif
   }

intSerialArray &
operator<< ( const intSerialArray & Lhs , int x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("\n\n\n### Inside of operator<< (intSerialArray,int) for intSerialArray class: (id=%d) = scalar \n",Lhs.Array_ID());

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator<<");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in intSerialArray & operator<<(intSerialArray,int)");
        }
#endif

#if defined(PPP)
     intSerialArray* Lhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
               Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
               APP_ASSERT(Lhs_SerialArray != NULL);
            // Lhs_SerialArray->displayReferenceCounts("AFTER PCE: *Lhs_SerialArray in intSerialArray & operator<<(intSerialArray,int)");
             }
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
          if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
               Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
                    (Lhs, Lhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
             }
            else
             {
               Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
                    (Lhs, Lhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
             }
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          printf ("intSerialArray: Lhs_SerialArray->isTemporary() = %s \n",(Lhs_SerialArray->isTemporary()) ? "TRUE" : "FALSE");
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in intSerialArray & operator<<(intSerialArray,int)");
        }
#endif

  // (11/27/2000) Added error checking (will not work with indirect addessing later!!!)
     APP_ASSERT(Temporary_Array_Set != NULL);

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set = new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Lhs_SerialArray != NULL);

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );

#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = Lhs;
#else
     intArray & Return_Value = intArray::Abstract_Operator(Temporary_Array_Set,Lhs,Lhs_SerialArray,*Lhs_SerialArray << x);
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (lhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Lhs_SerialArray->getReferenceCount() = %d \n",
       //      Lhs_SerialArray->getReferenceCount());

       // Must delete the Lhs_SerialArray if it was taken directly from the Lhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Lhs_SerialArray->decrementReferenceCount();
          if (Lhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Lhs_SerialArray;
             }
          Lhs_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in in intSerialArray & operator<<(intSerialArray,int)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator ( Lhs , x , 
               MDI_i_BIT_LSHIFT_Array_BitwiseLShift_Scalar,
               MDI_i_BIT_LSHIFT_Array_BitwiseLShift_Scalar_Accumulate_To_Operand , intSerialArray::Scalar_BitwiseLShift );
#endif
   }

intSerialArray &
operator<< ( int x , const intSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator<< (int,intSerialArray) for intSerialArray class!");

  // This is the only test we can do on the input!
     Rhs.Test_Consistency ("Test Rhs in operator<<");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs.displayReferenceCounts("Rhs in intSerialArray & operator<<(int,intSerialArray)");
        }
#endif

#if defined(PPP)
     intSerialArray* Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      ( Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = 
	      intArray::Parallel_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }
     if (Temporary_Array_Set == NULL) Temporary_Array_Set =
	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(Rhs_SerialArray != NULL);

     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = Rhs;
#else
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, Rhs, Rhs_SerialArray, x << *Rhs_SerialArray );
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (rhsIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): Rhs_SerialArray->getReferenceCount() = %d \n",
       //      Rhs_SerialArray->getReferenceCount());

       // Must delete the Rhs_SerialArray if it was taken directly from the Rhs array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          Rhs_SerialArray->decrementReferenceCount();
          if (Rhs_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete Rhs_SerialArray;
             }
          Rhs_SerialArray = NULL;

        }

  // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & operator<<(int,intSerialArray)");
        }
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Binary_Operator ( Rhs , x , 
               MDI_i_BIT_LSHIFT_Scalar_BitwiseLShift_Array,
               MDI_i_BIT_LSHIFT_Scalar_BitwiseLShift_Array_Accumulate_To_Operand , intSerialArray::Scalar_BitwiseLShift );
#endif
   }


intSerialArray &
intSerialArray::operator~ () const
   { 
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of unary minus operator operator~ for intSerialArray class!");

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in intSerialArray::operatoroperator~");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS in intSerialArray & intSerialArray::operator~()");
        }
#endif

#if defined(PPP)
     intSerialArray *This_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement ( *this , This_SerialArray );
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement ( *this , This_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = intArray::Parallel_Indirect_Conformability_Enforcement 
	      (*this, This_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = intArray::Parallel_Conformability_Enforcement 
	      (*this, This_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
	}
        *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() =
	   Mask_SerialArray;
     }

     if (Temporary_Array_Set == NULL)
          Temporary_Array_Set =	new Array_Conformability_Info_Type();

     APP_ASSERT(Temporary_Array_Set != NULL);
     APP_ASSERT(This_SerialArray != NULL);

     bool thisIsTemporary = This_SerialArray->isTemporary();

     APP_ASSERT ( (thisIsTemporary == TRUE) || (thisIsTemporary == FALSE) );

  // return intArray::Abstract_Operator ( Temporary_Array_Set, *this, This_SerialArray->operator~() );
#if defined(MEMORY_LEAK_TEST)
     intArray & Return_Value = *this;
#else
     intArray & Return_Value = intArray::Abstract_Operator ( Temporary_Array_Set, *this, This_SerialArray, This_SerialArray->operator~() );
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value (before delete This_SerialArray) in intSerialArray & intSerialArray::operator~()");
        }

  // This is the only test we can do on the output!
     Return_Value.Test_Consistency ("Test Return_Value (before delete This_SerialArray) in intSerialArray::operatoroperator~");
#endif

  // Check for reuse of serialArray object in return value (do not delete it if it was reused)
     if (thisIsTemporary == FALSE)
        {
          // Only delete the serial array data when the Overlap update model is used
       // printf ("In Macro Delete SerialArray (before decrement): This_SerialArray->getReferenceCount() = %d \n",
       //      This_SerialArray->getReferenceCount());

       // Must delete the This_SerialArray if it was taken directly from the This array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          APP_ASSERT (This_SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          This_SerialArray->decrementReferenceCount();
          if (This_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete This_SerialArray;
             }
          This_SerialArray = NULL;

        }

     // ... don't need to use macro because Return_Value won't be Mask ...
     if (Where_Statement_Support::Where_Statement_Mask != NULL)
        {
         *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Old_Mask_SerialArray; 
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          Mask_SerialArray->decrementReferenceCount();
          if (Mask_SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
               delete Mask_SerialArray;
          Mask_SerialArray = NULL;
        }

     // Delete the Temporary_Array_Set
  // printf ("In MACRO in operator.C: Temporary_Array_Set->getReferenceCount() = %d \n",Temporary_Array_Set->getReferenceCount());
     APP_ASSERT (Temporary_Array_Set->getReferenceCount() >= Array_Conformability_Info_Type::getReferenceCountBase());
     Temporary_Array_Set->decrementReferenceCount();
     if (Temporary_Array_Set->getReferenceCount() < Array_Conformability_Info_Type::getReferenceCountBase())
        {
       // printf ("COMMENTED OUT CALL TO DELETE: Deleting the Temporary_Array_Set in Macro Delete Temporary_Array_Set \n");
          delete Temporary_Array_Set;
        }
     Temporary_Array_Set = NULL;


#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & intSerialArray::operator~()");
        }

  // This is the only test we can do on the output!
     Return_Value.Test_Consistency ("Test Return_Value in intSerialArray::operatoroperator~");
#endif

     return Return_Value;
#else
     return intSerialArray::Abstract_Unary_Operator ( *this ,
                   MDI_i_BIT_COMPLEMENT_Array ,
                   MDI_i_BIT_COMPLEMENT_Array_Accumulate_To_Operand , intSerialArray::Unary_Minus );
#endif
   } 

#endif



















/*
// This code would simplify the macro to a function call
define(Macro_Delete_X_SerialArray,    Delete_SerialArray(X,X_SerialArray,Temporary_Array_Set);)
define(Macro_Delete_This_SerialArray, Delete_SerialArray((*this),This_SerialArray,Temporary_Array_Set);)
define(Macro_Delete_Lhs_SerialArray,  Delete_SerialArray(Lhs,Lhs_SerialArray,Temporary_Array_Set);)
define(Macro_Delete_Rhs_SerialArray,  Delete_SerialArray(Rhs,Rhs_SerialArray,Temporary_Array_Set);)

void
Delete_PCE_SerialArray (
     const $4Array & parallelArray,
     $4SerialArray* serialArray,
     const Array_Conformability_Info_Type *Temporary_Array_Set )
   {
     APP_ASSERT(Temporary_Array_Set != NULL);
     if (Temporary_Array_Set->Full_VSG_Update_Required == FALSE || TRUE)
        {
       // Only delete the serial array data when the Overlap update model is used

       // printf ("In Macro Delete SerialArray (before decrement): $1_SerialArray->getReferenceCount() = %d \n",
       //      $1_SerialArray->getReferenceCount());

       // Handle case where PADRE is used
#if defined(USE_PADRE)
       // when using PADRE we have to clear the use of the SerialArray_Domain before
       // deleting the SerialArray object.
          parallelArray.setLocalDomainInPADRE_Descriptor(NULL);
#endif

       // Can't reference Return_Value in all functions
       // APP_ASSERT ($1_SerialArray != NULL);
       // APP_ASSERT (Return_Value.Array_Descriptor.SerialArray != NULL);
       // APP_ASSERT (Return_Value.Array_Descriptor.SerialArray->Array_ID() != $1_SerialArray->Array_ID());

       // Must delete the $1_SerialArray if it was taken directly from the $1 array!
       // Added conventional mechanism for reference counting control
       // operator delete no longer decriments the referenceCount.
          serialArray->decrementReferenceCount();
          if (serialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in Macro Delete SerialArray \n");
               delete serialArray;
             }
          serialArray = NULL;
        }
   }
*/




 

 





 

 





















 









































