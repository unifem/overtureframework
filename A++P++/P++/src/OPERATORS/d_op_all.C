

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











#define DOUBLEARRAY
doubleSerialArray &
operator+ ( const doubleSerialArray & Lhs , const doubleSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
        {
          printf ("\n\n\n### Inside of operator+ (doubleSerialArray,doubleSerialArray) for doubleSerialArray class: (id=%d) = (id=%d) \n",
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
          printf ("doubleSerialArray: Lhs.isTemporary() = %s \n",(Lhs.isTemporary()) ? "TRUE" : "FALSE");
          Lhs.displayReferenceCounts("Lhs in doubleSerialArray & operator+(doubleSerialArray,doubleSerialArray)");
          printf ("doubleSerialArray: Rhs.isTemporary() = %s \n",(Rhs.isTemporary()) ? "TRUE" : "FALSE");
          Rhs.displayReferenceCounts("Rhs in doubleSerialArray & operator+(doubleSerialArray,doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray* Lhs_SerialArray = NULL;
     doubleSerialArray* Rhs_SerialArray = NULL;

     intSerialArray* Mask_SerialArray = NULL;
     intSerialArray* Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	      (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
               Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
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
               Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement
                  (Lhs, Lhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray, Rhs, Rhs_SerialArray );
             }
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          printf ("doubleSerialArray: Lhs_SerialArray->isTemporary() = %s \n",(Lhs_SerialArray->isTemporary()) ? "TRUE" : "FALSE");
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in doubleSerialArray & operator+(doubleSerialArray,doubleSerialArray)");
          printf ("doubleSerialArray: Rhs_SerialArray->isTemporary() = %s \n",(Rhs_SerialArray->isTemporary()) ? "TRUE" : "FALSE");
          Rhs_SerialArray->displayReferenceCounts("Rhs_SerialArray in doubleSerialArray & operator+(doubleSerialArray,doubleSerialArray)");
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

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, *Lhs_SerialArray + *Rhs_SerialArray );
#if defined(MEMORY_LEAK_TEST)
     doubleArray & Return_Value = Lhs;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator 
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & operator+(doubleSerialArray,doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator ( Lhs , Rhs , 
               MDI_d_Add_Array_Plus_Array,
               MDI_d_Add_Array_Plus_Array_Accumulate_To_Operand , doubleSerialArray::Plus );
#endif
   }


doubleSerialArray &
doubleSerialArray::operator-- ()
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside doubleSerialArray::operator-- () for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in doubleSerialArray::operator-- -- Prefix operator");
#endif

     (*this) -= 1;
     return *this;
   }
 
doubleSerialArray &
doubleSerialArray::operator-- ( int x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside doubleSerialArray::operator%s (int=%d) for doubleSerialArray class! \n","--",x);

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in doubleSerialArray::operator--(x=0) -- Postfix operator");
#endif

  // Postfix operator always passes zero as argument (strange but true -- See Stroustrup p594)
     APP_ASSERT( x == 0 );
     (*this) -= 1;
     return *this;
   }


#ifdef INTARRAY
/* There is no >>= operator and so the >> must be handled as a special case -- skip it for now */
doubleSerialArray &
operator>> ( const doubleSerialArray & Lhs , const doubleSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
        {
          printf ("\n\n\n### Inside of operator>> (doubleSerialArray,doubleSerialArray) for doubleSerialArray class: (id=%d) = (id=%d) \n",
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
          printf ("doubleSerialArray: Lhs.isTemporary() = %s \n",(Lhs.isTemporary()) ? "TRUE" : "FALSE");
          Lhs.displayReferenceCounts("Lhs in doubleSerialArray & operator>>(doubleSerialArray,doubleSerialArray)");
          printf ("doubleSerialArray: Rhs.isTemporary() = %s \n",(Rhs.isTemporary()) ? "TRUE" : "FALSE");
          Rhs.displayReferenceCounts("Rhs in doubleSerialArray & operator>>(doubleSerialArray,doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray* Lhs_SerialArray = NULL;
     doubleSerialArray* Rhs_SerialArray = NULL;

     intSerialArray* Mask_SerialArray = NULL;
     intSerialArray* Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	      (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
               Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
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
               Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement
                  (Lhs, Lhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray, Rhs, Rhs_SerialArray );
             }
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          printf ("doubleSerialArray: Lhs_SerialArray->isTemporary() = %s \n",(Lhs_SerialArray->isTemporary()) ? "TRUE" : "FALSE");
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in doubleSerialArray & operator>>(doubleSerialArray,doubleSerialArray)");
          printf ("doubleSerialArray: Rhs_SerialArray->isTemporary() = %s \n",(Rhs_SerialArray->isTemporary()) ? "TRUE" : "FALSE");
          Rhs_SerialArray->displayReferenceCounts("Rhs_SerialArray in doubleSerialArray & operator>>(doubleSerialArray,doubleSerialArray)");
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

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, *Lhs_SerialArray >> *Rhs_SerialArray );
#if defined(MEMORY_LEAK_TEST)
     doubleArray & Return_Value = Lhs;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator 
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & operator>>(doubleSerialArray,doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator ( Lhs , Rhs , 
               MDI_d_BIT_RSHIFT_Array_BitwiseRShift_Array,
               MDI_d_BIT_RSHIFT_Array_BitwiseRShift_Array_Accumulate_To_Operand , doubleSerialArray::BitwiseRShift );
#endif
   }

doubleSerialArray &
operator>> ( const doubleSerialArray & Lhs , double x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("\n\n\n### Inside of operator>> (doubleSerialArray,double) for doubleSerialArray class: (id=%d) = scalar \n",Lhs.Array_ID());

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator>>");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in doubleSerialArray & operator>>(doubleSerialArray,double)");
        }
#endif

#if defined(PPP)
     doubleSerialArray* Lhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
               Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
               APP_ASSERT(Lhs_SerialArray != NULL);
            // Lhs_SerialArray->displayReferenceCounts("AFTER PCE: *Lhs_SerialArray in doubleSerialArray & operator>>(doubleSerialArray,double)");
             }
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
          if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
               Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
                    (Lhs, Lhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
             }
            else
             {
               Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
                    (Lhs, Lhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
             }
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          printf ("doubleSerialArray: Lhs_SerialArray->isTemporary() = %s \n",(Lhs_SerialArray->isTemporary()) ? "TRUE" : "FALSE");
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in doubleSerialArray & operator>>(doubleSerialArray,double)");
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
     doubleArray & Return_Value = Lhs;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator(Temporary_Array_Set,Lhs,Lhs_SerialArray,*Lhs_SerialArray >> x);
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
          Return_Value.displayReferenceCounts("Return_Value in in doubleSerialArray & operator>>(doubleSerialArray,double)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator ( Lhs , x , 
               MDI_d_BIT_RSHIFT_Array_BitwiseRShift_Scalar,
               MDI_d_BIT_RSHIFT_Array_BitwiseRShift_Scalar_Accumulate_To_Operand , doubleSerialArray::Scalar_BitwiseRShift );
#endif
   }

doubleSerialArray &
operator>> ( double x , const doubleSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator>> (double,doubleSerialArray) for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Rhs.Test_Consistency ("Test Rhs in operator>>");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs.displayReferenceCounts("Rhs in doubleSerialArray & operator>>(double,doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray* Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      ( Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = 
	      doubleArray::Parallel_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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
     doubleArray & Return_Value = Rhs;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Rhs, Rhs_SerialArray, x >> *Rhs_SerialArray );
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & operator>>(double,doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator ( Rhs , x , 
               MDI_d_BIT_RSHIFT_Scalar_BitwiseRShift_Array,
               MDI_d_BIT_RSHIFT_Scalar_BitwiseRShift_Array_Accumulate_To_Operand , doubleSerialArray::Scalar_BitwiseRShift );
#endif
   }

#endif

#ifdef INTARRAY
doubleSerialArray &
doubleSerialArray::operator&= ( const doubleSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of doubleSerialArray::operator&= (doubleSerialArray) for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in doubleSerialArray::operator&=");
     Rhs.Test_Consistency ("Test Rhs in doubleSerialArray::operator&=");
#endif

  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Test_Conformability (Rhs);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS in doubleSerialArray & doubleSerialArray::operator&=(doubleSerialArray)");
          Rhs.displayReferenceCounts("Rhs in doubleSerialArray & doubleSerialArray::operator&=(doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *This_SerialArray = NULL;
     doubleSerialArray *Rhs_SerialArray  = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if ((Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	    (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      ( *this , This_SerialArray, Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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
     doubleArray::Abstract_Modification_Operator ( Temporary_Array_Set, *this, Rhs, This_SerialArray, Rhs_SerialArray, *This_SerialArray &= *Rhs_SerialArray );
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
     doubleSerialArray::Abstract_Operator_Operation_Equals ( *this , Rhs ,
               MDI_d_BIT_AND_Array_BitwiseAND_Array_Accumulate_To_Operand , doubleSerialArray::BitwiseAND_Equals );
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS (return value) in doubleSerialArray & doubleSerialArray::operator&=(doubleSerialArray)");
        }
#endif

     return *this;
   }

#endif

#ifdef INTARRAY
doubleSerialArray &
doubleSerialArray::operator|= ( const doubleSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of doubleSerialArray::operator|= (doubleSerialArray) for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in doubleSerialArray::operator|=");
     Rhs.Test_Consistency ("Test Rhs in doubleSerialArray::operator|=");
#endif

  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Test_Conformability (Rhs);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS in doubleSerialArray & doubleSerialArray::operator|=(doubleSerialArray)");
          Rhs.displayReferenceCounts("Rhs in doubleSerialArray & doubleSerialArray::operator|=(doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *This_SerialArray = NULL;
     doubleSerialArray *Rhs_SerialArray  = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if ((Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	    (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      ( *this , This_SerialArray, Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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
     doubleArray::Abstract_Modification_Operator ( Temporary_Array_Set, *this, Rhs, This_SerialArray, Rhs_SerialArray, *This_SerialArray |= *Rhs_SerialArray );
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
     doubleSerialArray::Abstract_Operator_Operation_Equals ( *this , Rhs ,
               MDI_d_BIT_OR_Array_BitwiseOR_Array_Accumulate_To_Operand , doubleSerialArray::BitwiseOR_Equals );
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS (return value) in doubleSerialArray & doubleSerialArray::operator|=(doubleSerialArray)");
        }
#endif

     return *this;
   }

#endif

#ifdef INTARRAY
doubleSerialArray &
doubleSerialArray::operator^= ( const doubleSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of doubleSerialArray::operator^= (doubleSerialArray) for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in doubleSerialArray::operator^=");
     Rhs.Test_Consistency ("Test Rhs in doubleSerialArray::operator^=");
#endif

  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Test_Conformability (Rhs);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS in doubleSerialArray & doubleSerialArray::operator^=(doubleSerialArray)");
          Rhs.displayReferenceCounts("Rhs in doubleSerialArray & doubleSerialArray::operator^=(doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *This_SerialArray = NULL;
     doubleSerialArray *Rhs_SerialArray  = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if ((Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	    (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      ( *this , This_SerialArray, Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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
     doubleArray::Abstract_Modification_Operator ( Temporary_Array_Set, *this, Rhs, This_SerialArray, Rhs_SerialArray, *This_SerialArray ^= *Rhs_SerialArray );
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
     doubleSerialArray::Abstract_Operator_Operation_Equals ( *this , Rhs ,
               MDI_d_BIT_XOR_Array_BitwiseXOR_Array_Accumulate_To_Operand , doubleSerialArray::BitwiseXOR_Equals );
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS (return value) in doubleSerialArray & doubleSerialArray::operator^=(doubleSerialArray)");
        }
#endif

     return *this;
   }

#endif

intSerialArray &
doubleSerialArray::convertTo_intArray () const
   {
// Used to implement the conversion functions between int float and double arrays

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of convertTo_intArray for doubleSerialArray class! \n");

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in doubleSerialArray::operatorconvertTo_intArray");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS in intSerialArray & doubleSerialArray::convertTo_intArray()");
        }
#endif

#if defined(PPP)
     doubleSerialArray *This_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
	       Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement ( *this, This_SerialArray);
             }
            else
             {
               Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement ( *this, This_SerialArray);
             }
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
          if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
               Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement (
                    *this, This_SerialArray, 
                    *Where_Statement_Support::Where_Statement_Mask,
                    Mask_SerialArray);
             }
            else
             {
               Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement 
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
     printf ("Can't do MEMORY_LEAK_TEST in intSerialArray & doubleSerialArray::convertTo_intArray () \n");
#endif
     intArray & Return_Value = doubleArray::Abstract_int_Conversion_Operator ( Temporary_Array_Set, *this, This_SerialArray, This_SerialArray->convertTo_intArray() );

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
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & doubleSerialArray::convertTo_intArray()");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_int_Conversion_Operator ( *this ,
                   MDI_d_Array_convertTo_intArray_Array_Accumulate_To_Operand ,
                   doubleSerialArray::convertTo_intArrayFunction );
#endif
   }

floatSerialArray &
doubleSerialArray::convertTo_floatArray () const
   {
// Used to implement the conversion functions between int float and double arrays

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of convertTo_floatArray for doubleSerialArray class! \n");

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in doubleSerialArray::operatorconvertTo_floatArray");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS in floatSerialArray & doubleSerialArray::convertTo_floatArray()");
        }
#endif

#if defined(PPP)
     doubleSerialArray *This_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
	       Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement ( *this, This_SerialArray);
             }
            else
             {
               Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement ( *this, This_SerialArray);
             }
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
          if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
               Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement (
                    *this, This_SerialArray, 
                    *Where_Statement_Support::Where_Statement_Mask,
                    Mask_SerialArray);
             }
            else
             {
               Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement 
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
     printf ("Can't do MEMORY_LEAK_TEST in intSerialArray & doubleSerialArray::convertTo_floatArray () \n");
#endif
     floatArray & Return_Value = doubleArray::Abstract_float_Conversion_Operator ( Temporary_Array_Set, *this, This_SerialArray, This_SerialArray->convertTo_floatArray() );

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
          Return_Value.displayReferenceCounts("Return_Value in floatSerialArray & doubleSerialArray::convertTo_floatArray()");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_float_Conversion_Operator ( *this ,
                   MDI_d_Array_convertTo_floatArray_Array_Accumulate_To_Operand ,
                   doubleSerialArray::convertTo_floatArrayFunction );
#endif
   }

doubleSerialArray &
doubleSerialArray::convertTo_doubleArray () const
   {
// Used to implement the conversion functions between int float and double arrays

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of convertTo_doubleArray for doubleSerialArray class! \n");

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in doubleSerialArray::operatorconvertTo_doubleArray");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS in doubleSerialArray & doubleSerialArray::convertTo_doubleArray()");
        }
#endif

#if defined(PPP)
     doubleSerialArray *This_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
	       Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement ( *this, This_SerialArray);
             }
            else
             {
               Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement ( *this, This_SerialArray);
             }
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
          if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
               Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement (
                    *this, This_SerialArray, 
                    *Where_Statement_Support::Where_Statement_Mask,
                    Mask_SerialArray);
             }
            else
             {
               Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement 
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
     printf ("Can't do MEMORY_LEAK_TEST in intSerialArray & doubleSerialArray::convertTo_doubleArray () \n");
#endif
     doubleArray & Return_Value = doubleArray::Abstract_double_Conversion_Operator ( Temporary_Array_Set, *this, This_SerialArray, This_SerialArray->convertTo_doubleArray() );

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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & doubleSerialArray::convertTo_doubleArray()");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_double_Conversion_Operator ( *this ,
                   MDI_d_Array_convertTo_doubleArray_Array_Accumulate_To_Operand ,
                   doubleSerialArray::convertTo_doubleArrayFunction );
#endif
   }


doubleSerialArray &
doubleSerialArray::operator-= ( const doubleSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of doubleSerialArray::operator-= (doubleSerialArray) for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in doubleSerialArray::operator-=");
     Rhs.Test_Consistency ("Test Rhs in doubleSerialArray::operator-=");
#endif

  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Test_Conformability (Rhs);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS in doubleSerialArray & doubleSerialArray::operator-=(doubleSerialArray)");
          Rhs.displayReferenceCounts("Rhs in doubleSerialArray & doubleSerialArray::operator-=(doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *This_SerialArray = NULL;
     doubleSerialArray *Rhs_SerialArray  = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if ((Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	    (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      ( *this , This_SerialArray, Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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
     doubleArray::Abstract_Modification_Operator ( Temporary_Array_Set, *this, Rhs, This_SerialArray, Rhs_SerialArray, *This_SerialArray -= *Rhs_SerialArray );
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
     doubleSerialArray::Abstract_Operator_Operation_Equals ( *this , Rhs ,
               MDI_d_Subtract_Array_Minus_Array_Accumulate_To_Operand , doubleSerialArray::Minus_Equals );
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS (return value) in doubleSerialArray & doubleSerialArray::operator-=(doubleSerialArray)");
        }
#endif

     return *this;
   }


doubleSerialArray &
doubleSerialArray::operator-= ( double x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of doubleSerialArray::operator-= (double) for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in doubleSerialArray::operator-=");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS in doubleSerialArray & doubleSerialArray::operator-=(double)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *This_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( *this , This_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( *this , This_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (*this, This_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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
     doubleArray::Abstract_Modification_Operator ( Temporary_Array_Set, *this, This_SerialArray, *This_SerialArray -= x );
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
     doubleSerialArray::Abstract_Operator_Operation_Equals ( *this , x ,
        MDI_d_Subtract_Array_Minus_Scalar_Accumulate_To_Operand , doubleSerialArray::Scalar_Minus_Equals );
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS (return value) in doubleSerialArray & doubleSerialArray::operator-=(double)");
        }
#endif

     return *this;
   }


doubleSerialArray &
doubleSerialArray::operator- () const
   { 
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of unary minus operator operator- for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in doubleSerialArray::operatoroperator-");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS in doubleSerialArray & doubleSerialArray::operator-()");
        }
#endif

#if defined(PPP)
     doubleSerialArray *This_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( *this , This_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( *this , This_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (*this, This_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, *this, This_SerialArray->operator-() );
#if defined(MEMORY_LEAK_TEST)
     doubleArray & Return_Value = *this;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, *this, This_SerialArray, This_SerialArray->operator-() );
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value (before delete This_SerialArray) in doubleSerialArray & doubleSerialArray::operator-()");
        }

  // This is the only test we can do on the output!
     Return_Value.Test_Consistency ("Test Return_Value (before delete This_SerialArray) in doubleSerialArray::operatoroperator-");
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & doubleSerialArray::operator-()");
        }

  // This is the only test we can do on the output!
     Return_Value.Test_Consistency ("Test Return_Value in doubleSerialArray::operatoroperator-");
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Unary_Operator ( *this ,
                   MDI_d_Unary_Minus_Array ,
                   MDI_d_Unary_Minus_Array_Accumulate_To_Operand , doubleSerialArray::Unary_Minus );
#endif
   } 


doubleSerialArray &
operator* ( const doubleSerialArray & Lhs , const doubleSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
        {
          printf ("\n\n\n### Inside of operator* (doubleSerialArray,doubleSerialArray) for doubleSerialArray class: (id=%d) = (id=%d) \n",
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
          printf ("doubleSerialArray: Lhs.isTemporary() = %s \n",(Lhs.isTemporary()) ? "TRUE" : "FALSE");
          Lhs.displayReferenceCounts("Lhs in doubleSerialArray & operator*(doubleSerialArray,doubleSerialArray)");
          printf ("doubleSerialArray: Rhs.isTemporary() = %s \n",(Rhs.isTemporary()) ? "TRUE" : "FALSE");
          Rhs.displayReferenceCounts("Rhs in doubleSerialArray & operator*(doubleSerialArray,doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray* Lhs_SerialArray = NULL;
     doubleSerialArray* Rhs_SerialArray = NULL;

     intSerialArray* Mask_SerialArray = NULL;
     intSerialArray* Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	      (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
               Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
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
               Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement
                  (Lhs, Lhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray, Rhs, Rhs_SerialArray );
             }
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          printf ("doubleSerialArray: Lhs_SerialArray->isTemporary() = %s \n",(Lhs_SerialArray->isTemporary()) ? "TRUE" : "FALSE");
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in doubleSerialArray & operator*(doubleSerialArray,doubleSerialArray)");
          printf ("doubleSerialArray: Rhs_SerialArray->isTemporary() = %s \n",(Rhs_SerialArray->isTemporary()) ? "TRUE" : "FALSE");
          Rhs_SerialArray->displayReferenceCounts("Rhs_SerialArray in doubleSerialArray & operator*(doubleSerialArray,doubleSerialArray)");
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

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, *Lhs_SerialArray * *Rhs_SerialArray );
#if defined(MEMORY_LEAK_TEST)
     doubleArray & Return_Value = Lhs;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator 
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & operator*(doubleSerialArray,doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator ( Lhs , Rhs , 
               MDI_d_Multiply_Array_Times_Array,
               MDI_d_Multiply_Array_Times_Array_Accumulate_To_Operand , doubleSerialArray::Times );
#endif
   }


doubleSerialArray &
operator* ( const doubleSerialArray & Lhs , double x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("\n\n\n### Inside of operator* (doubleSerialArray,double) for doubleSerialArray class: (id=%d) = scalar \n",Lhs.Array_ID());

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator*");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in doubleSerialArray & operator*(doubleSerialArray,double)");
        }
#endif

#if defined(PPP)
     doubleSerialArray* Lhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
               Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
               APP_ASSERT(Lhs_SerialArray != NULL);
            // Lhs_SerialArray->displayReferenceCounts("AFTER PCE: *Lhs_SerialArray in doubleSerialArray & operator*(doubleSerialArray,double)");
             }
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
          if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
               Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
                    (Lhs, Lhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
             }
            else
             {
               Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
                    (Lhs, Lhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
             }
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          printf ("doubleSerialArray: Lhs_SerialArray->isTemporary() = %s \n",(Lhs_SerialArray->isTemporary()) ? "TRUE" : "FALSE");
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in doubleSerialArray & operator*(doubleSerialArray,double)");
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
     doubleArray & Return_Value = Lhs;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator(Temporary_Array_Set,Lhs,Lhs_SerialArray,*Lhs_SerialArray * x);
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
          Return_Value.displayReferenceCounts("Return_Value in in doubleSerialArray & operator*(doubleSerialArray,double)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator ( Lhs , x , 
               MDI_d_Multiply_Array_Times_Scalar,
               MDI_d_Multiply_Array_Times_Scalar_Accumulate_To_Operand , doubleSerialArray::Scalar_Times );
#endif
   }


doubleSerialArray &
operator* ( double x , const doubleSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator* (double,doubleSerialArray) for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Rhs.Test_Consistency ("Test Rhs in operator*");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs.displayReferenceCounts("Rhs in doubleSerialArray & operator*(double,doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray* Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      ( Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = 
	      doubleArray::Parallel_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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
     doubleArray & Return_Value = Rhs;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Rhs, Rhs_SerialArray, x * *Rhs_SerialArray );
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & operator*(double,doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator ( Rhs , x , 
               MDI_d_Multiply_Scalar_Times_Array,
               MDI_d_Multiply_Scalar_Times_Array_Accumulate_To_Operand , doubleSerialArray::Scalar_Times );
#endif
   }


doubleSerialArray &
doubleSerialArray::operator*= ( const doubleSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of doubleSerialArray::operator*= (doubleSerialArray) for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in doubleSerialArray::operator*=");
     Rhs.Test_Consistency ("Test Rhs in doubleSerialArray::operator*=");
#endif

  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Test_Conformability (Rhs);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS in doubleSerialArray & doubleSerialArray::operator*=(doubleSerialArray)");
          Rhs.displayReferenceCounts("Rhs in doubleSerialArray & doubleSerialArray::operator*=(doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *This_SerialArray = NULL;
     doubleSerialArray *Rhs_SerialArray  = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if ((Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	    (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      ( *this , This_SerialArray, Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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
     doubleArray::Abstract_Modification_Operator ( Temporary_Array_Set, *this, Rhs, This_SerialArray, Rhs_SerialArray, *This_SerialArray *= *Rhs_SerialArray );
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
     doubleSerialArray::Abstract_Operator_Operation_Equals ( *this , Rhs ,
               MDI_d_Multiply_Array_Times_Array_Accumulate_To_Operand , doubleSerialArray::Times_Equals );
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS (return value) in doubleSerialArray & doubleSerialArray::operator*=(doubleSerialArray)");
        }
#endif

     return *this;
   }


doubleSerialArray &
doubleSerialArray::operator*= ( double x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of doubleSerialArray::operator*= (double) for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in doubleSerialArray::operator*=");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS in doubleSerialArray & doubleSerialArray::operator*=(double)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *This_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( *this , This_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( *this , This_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (*this, This_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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
     doubleArray::Abstract_Modification_Operator ( Temporary_Array_Set, *this, This_SerialArray, *This_SerialArray *= x );
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
     doubleSerialArray::Abstract_Operator_Operation_Equals ( *this , x ,
        MDI_d_Multiply_Array_Times_Scalar_Accumulate_To_Operand , doubleSerialArray::Scalar_Times_Equals );
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS (return value) in doubleSerialArray & doubleSerialArray::operator*=(double)");
        }
#endif

     return *this;
   }


doubleSerialArray &
operator/ ( const doubleSerialArray & Lhs , const doubleSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator/ (doubleSerialArray,doubleSerialArray) for doubleSerialArray class!");

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
          Lhs.displayReferenceCounts("Lhs in doubleSerialArray & operator/(doubleSerialArray,doubleSerialArray)");
          Rhs.displayReferenceCounts("Rhs in doubleSerialArray & operator/(doubleSerialArray,doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray* Lhs_SerialArray = NULL;
     doubleSerialArray* Rhs_SerialArray = NULL;

     intSerialArray* Mask_SerialArray = NULL;
     intSerialArray* Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, *Lhs_SerialArray / *Rhs_SerialArray );
#if defined(MEMORY_LEAK_TEST)
     doubleArray & Return_Value = Lhs;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator 
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & operator/(doubleSerialArray,doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator_Non_Commutative ( Lhs , Rhs , 
               MDI_d_Divide_Array_Divided_By_Array,
               MDI_d_Divide_Array_Divided_By_Array_Accumulate_To_Operand , doubleSerialArray::Divided_By );
#endif
   }


doubleSerialArray &
operator+ ( const doubleSerialArray & Lhs , double x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("\n\n\n### Inside of operator+ (doubleSerialArray,double) for doubleSerialArray class: (id=%d) = scalar \n",Lhs.Array_ID());

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator+");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in doubleSerialArray & operator+(doubleSerialArray,double)");
        }
#endif

#if defined(PPP)
     doubleSerialArray* Lhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
               Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
               APP_ASSERT(Lhs_SerialArray != NULL);
            // Lhs_SerialArray->displayReferenceCounts("AFTER PCE: *Lhs_SerialArray in doubleSerialArray & operator+(doubleSerialArray,double)");
             }
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
          if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
               Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
                    (Lhs, Lhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
             }
            else
             {
               Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
                    (Lhs, Lhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
             }
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          printf ("doubleSerialArray: Lhs_SerialArray->isTemporary() = %s \n",(Lhs_SerialArray->isTemporary()) ? "TRUE" : "FALSE");
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in doubleSerialArray & operator+(doubleSerialArray,double)");
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
     doubleArray & Return_Value = Lhs;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator(Temporary_Array_Set,Lhs,Lhs_SerialArray,*Lhs_SerialArray + x);
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
          Return_Value.displayReferenceCounts("Return_Value in in doubleSerialArray & operator+(doubleSerialArray,double)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator ( Lhs , x , 
               MDI_d_Add_Array_Plus_Scalar,
               MDI_d_Add_Array_Plus_Scalar_Accumulate_To_Operand , doubleSerialArray::Scalar_Plus );
#endif
   }


doubleSerialArray &
operator/ ( const doubleSerialArray & Lhs , double x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator/ (doubleSerialArray,double) for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator/");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in doubleSerialArray & operator/(doubleSerialArray,double)");
        }
#endif

#if defined(PPP)
     doubleSerialArray* Lhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      ( Lhs, Lhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = 
	      doubleArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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
     doubleArray & Return_Value = Lhs;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Lhs_SerialArray, *Lhs_SerialArray / x );
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & operator/(doubleSerialArray,double)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator ( Lhs , x , 
               MDI_d_Divide_Array_Divided_By_Scalar,
               MDI_d_Divide_Array_Divided_By_Scalar_Accumulate_To_Operand , doubleSerialArray::Scalar_Divided_By );
#endif
   }


doubleSerialArray &
operator/ ( double x , const doubleSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of operator/ (double,doubleSerialArray) for doubleSerialArray class! \n");

  // This is the only test we can do on the input!
     Rhs.Test_Consistency ("Test Rhs in operator/");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs.displayReferenceCounts("Rhs in doubleSerialArray & operator/(double,doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray* Rhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE)
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      ( Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = 
	      doubleArray::Parallel_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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
     doubleArray & Return_Value = Rhs;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Rhs, Rhs_SerialArray, x / *Rhs_SerialArray );
  // return doubleArray::Abstract_Binary_Operator ( Temporary_Array_Set, Rhs, x / *Rhs_SerialArray );
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & operator/(double,doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator ( Rhs , x , 
               MDI_d_Divide_Scalar_Divided_By_Array,
               MDI_d_Divide_Scalar_Divided_By_Array_Accumulate_To_Operand , doubleSerialArray::Scalar_Divided_By );
#endif
   }


doubleSerialArray &
doubleSerialArray::operator/= ( const doubleSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of doubleSerialArray::operator/= (doubleSerialArray) for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in doubleSerialArray::operator/=");
     Rhs.Test_Consistency ("Test Rhs in doubleSerialArray::operator/=");
#endif

  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Test_Conformability (Rhs);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS in doubleSerialArray & doubleSerialArray::operator/=(doubleSerialArray)");
          Rhs.displayReferenceCounts("Rhs in doubleSerialArray & doubleSerialArray::operator/=(doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *This_SerialArray = NULL;
     doubleSerialArray *Rhs_SerialArray  = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if ((Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	    (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      ( *this , This_SerialArray, Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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
     doubleArray::Abstract_Modification_Operator ( Temporary_Array_Set, *this, Rhs, This_SerialArray, Rhs_SerialArray, *This_SerialArray /= *Rhs_SerialArray );
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
     doubleSerialArray::Abstract_Operator_Operation_Equals ( *this , Rhs ,
               MDI_d_Divide_Array_Divided_By_Array_Accumulate_To_Operand , doubleSerialArray::Divided_By_Equals );
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS (return value) in doubleSerialArray & doubleSerialArray::operator/=(doubleSerialArray)");
        }
#endif

     return *this;
   }


doubleSerialArray &
doubleSerialArray::operator/= ( double x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of doubleSerialArray::operator/= (double) for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in doubleSerialArray::operator/=");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS in doubleSerialArray & doubleSerialArray::operator/=(double)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *This_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( *this , This_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( *this , This_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (*this, This_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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
     doubleArray::Abstract_Modification_Operator ( Temporary_Array_Set, *this, This_SerialArray, *This_SerialArray /= x );
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
     doubleSerialArray::Abstract_Operator_Operation_Equals ( *this , x ,
        MDI_d_Divide_Array_Divided_By_Scalar_Accumulate_To_Operand , doubleSerialArray::Scalar_Divided_By_Equals );
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS (return value) in doubleSerialArray & doubleSerialArray::operator/=(double)");
        }
#endif

     return *this;
   }


doubleSerialArray &
operator% ( const doubleSerialArray & Lhs , const doubleSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
        {
          printf ("\n\n\n### Inside of operator% (doubleSerialArray,doubleSerialArray) for doubleSerialArray class: (id=%d) = (id=%d) \n",
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
          printf ("doubleSerialArray: Lhs.isTemporary() = %s \n",(Lhs.isTemporary()) ? "TRUE" : "FALSE");
          Lhs.displayReferenceCounts("Lhs in doubleSerialArray & operator%(doubleSerialArray,doubleSerialArray)");
          printf ("doubleSerialArray: Rhs.isTemporary() = %s \n",(Rhs.isTemporary()) ? "TRUE" : "FALSE");
          Rhs.displayReferenceCounts("Rhs in doubleSerialArray & operator%(doubleSerialArray,doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray* Lhs_SerialArray = NULL;
     doubleSerialArray* Rhs_SerialArray = NULL;

     intSerialArray* Mask_SerialArray = NULL;
     intSerialArray* Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	      (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
               Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
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
               Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement
                  (Lhs, Lhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray, Rhs, Rhs_SerialArray );
             }
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          printf ("doubleSerialArray: Lhs_SerialArray->isTemporary() = %s \n",(Lhs_SerialArray->isTemporary()) ? "TRUE" : "FALSE");
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in doubleSerialArray & operator%(doubleSerialArray,doubleSerialArray)");
          printf ("doubleSerialArray: Rhs_SerialArray->isTemporary() = %s \n",(Rhs_SerialArray->isTemporary()) ? "TRUE" : "FALSE");
          Rhs_SerialArray->displayReferenceCounts("Rhs_SerialArray in doubleSerialArray & operator%(doubleSerialArray,doubleSerialArray)");
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

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, *Lhs_SerialArray % *Rhs_SerialArray );
#if defined(MEMORY_LEAK_TEST)
     doubleArray & Return_Value = Lhs;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator 
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & operator%(doubleSerialArray,doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator ( Lhs , Rhs , 
               MDI_d_Fmod_Array_Modulo_Array,
               MDI_d_Fmod_Array_Modulo_Array_Accumulate_To_Operand , doubleSerialArray::Modulo );
#endif
   }


doubleSerialArray &
operator% ( const doubleSerialArray & Lhs , double x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("\n\n\n### Inside of operator% (doubleSerialArray,double) for doubleSerialArray class: (id=%d) = scalar \n",Lhs.Array_ID());

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator%");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in doubleSerialArray & operator%(doubleSerialArray,double)");
        }
#endif

#if defined(PPP)
     doubleSerialArray* Lhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
               Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
               APP_ASSERT(Lhs_SerialArray != NULL);
            // Lhs_SerialArray->displayReferenceCounts("AFTER PCE: *Lhs_SerialArray in doubleSerialArray & operator%(doubleSerialArray,double)");
             }
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
          if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
               Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
                    (Lhs, Lhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
             }
            else
             {
               Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
                    (Lhs, Lhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
             }
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          printf ("doubleSerialArray: Lhs_SerialArray->isTemporary() = %s \n",(Lhs_SerialArray->isTemporary()) ? "TRUE" : "FALSE");
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in doubleSerialArray & operator%(doubleSerialArray,double)");
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
     doubleArray & Return_Value = Lhs;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator(Temporary_Array_Set,Lhs,Lhs_SerialArray,*Lhs_SerialArray % x);
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
          Return_Value.displayReferenceCounts("Return_Value in in doubleSerialArray & operator%(doubleSerialArray,double)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator ( Lhs , x , 
               MDI_d_Fmod_Array_Modulo_Scalar,
               MDI_d_Fmod_Array_Modulo_Scalar_Accumulate_To_Operand , doubleSerialArray::Scalar_Modulo );
#endif
   }


doubleSerialArray &
operator% ( double x , const doubleSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator% (double,doubleSerialArray) for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Rhs.Test_Consistency ("Test Rhs in operator%");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs.displayReferenceCounts("Rhs in doubleSerialArray & operator%(double,doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray* Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      ( Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = 
	      doubleArray::Parallel_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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
     doubleArray & Return_Value = Rhs;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Rhs, Rhs_SerialArray, x % *Rhs_SerialArray );
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & operator%(double,doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator ( Rhs , x , 
               MDI_d_Fmod_Scalar_Modulo_Array,
               MDI_d_Fmod_Scalar_Modulo_Array_Accumulate_To_Operand , doubleSerialArray::Scalar_Modulo );
#endif
   }


doubleSerialArray &
doubleSerialArray::operator%= ( const doubleSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of doubleSerialArray::operator%= (doubleSerialArray) for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in doubleSerialArray::operator%=");
     Rhs.Test_Consistency ("Test Rhs in doubleSerialArray::operator%=");
#endif

  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Test_Conformability (Rhs);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS in doubleSerialArray & doubleSerialArray::operator%=(doubleSerialArray)");
          Rhs.displayReferenceCounts("Rhs in doubleSerialArray & doubleSerialArray::operator%=(doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *This_SerialArray = NULL;
     doubleSerialArray *Rhs_SerialArray  = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if ((Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	    (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      ( *this , This_SerialArray, Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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
     doubleArray::Abstract_Modification_Operator ( Temporary_Array_Set, *this, Rhs, This_SerialArray, Rhs_SerialArray, *This_SerialArray %= *Rhs_SerialArray );
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
     doubleSerialArray::Abstract_Operator_Operation_Equals ( *this , Rhs ,
               MDI_d_Fmod_Array_Modulo_Array_Accumulate_To_Operand , doubleSerialArray::Modulo_Equals );
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS (return value) in doubleSerialArray & doubleSerialArray::operator%=(doubleSerialArray)");
        }
#endif

     return *this;
   }


doubleSerialArray &
doubleSerialArray::operator%= ( double x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of doubleSerialArray::operator%= (double) for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in doubleSerialArray::operator%=");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS in doubleSerialArray & doubleSerialArray::operator%=(double)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *This_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( *this , This_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( *this , This_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (*this, This_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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
     doubleArray::Abstract_Modification_Operator ( Temporary_Array_Set, *this, This_SerialArray, *This_SerialArray %= x );
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
     doubleSerialArray::Abstract_Operator_Operation_Equals ( *this , x ,
        MDI_d_Fmod_Array_Modulo_Scalar_Accumulate_To_Operand , doubleSerialArray::Scalar_Modulo_Equals );
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS (return value) in doubleSerialArray & doubleSerialArray::operator%=(double)");
        }
#endif

     return *this;
   }


#ifndef INTARRAY
doubleSerialArray &
cos ( const doubleSerialArray & X )
   { 
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of cos for doubleSerialArray class! \n");

  // This is the only test we can do on the input!
     X.Test_Consistency ("Test X in cos ( const doubleSerialArray & X )");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in doubleSerialArray & cos(doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *X_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( X, X_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( X, X_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, X, cos(*X_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     doubleArray & Return_Value = X;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, X, X_SerialArray, cos(*X_SerialArray) );
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & cos(doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Unary_Operator ( X ,
                   MDI_d_Cos_Array ,
                   MDI_d_Cos_Array_Accumulate_To_Operand , doubleSerialArray::cos_Function );
#endif
   } 

#endif

doubleSerialArray &
operator+ ( double x , const doubleSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator+ (double,doubleSerialArray) for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Rhs.Test_Consistency ("Test Rhs in operator+");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs.displayReferenceCounts("Rhs in doubleSerialArray & operator+(double,doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray* Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      ( Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = 
	      doubleArray::Parallel_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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
     doubleArray & Return_Value = Rhs;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Rhs, Rhs_SerialArray, x + *Rhs_SerialArray );
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & operator+(double,doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator ( Rhs , x , 
               MDI_d_Add_Scalar_Plus_Array,
               MDI_d_Add_Scalar_Plus_Array_Accumulate_To_Operand , doubleSerialArray::Scalar_Plus );
#endif
   }


#ifndef INTARRAY
doubleSerialArray &
sin ( const doubleSerialArray & X )
   { 
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of sin for doubleSerialArray class! \n");

  // This is the only test we can do on the input!
     X.Test_Consistency ("Test X in sin ( const doubleSerialArray & X )");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in doubleSerialArray & sin(doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *X_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( X, X_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( X, X_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, X, sin(*X_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     doubleArray & Return_Value = X;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, X, X_SerialArray, sin(*X_SerialArray) );
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & sin(doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Unary_Operator ( X ,
                   MDI_d_Sin_Array ,
                   MDI_d_Sin_Array_Accumulate_To_Operand , doubleSerialArray::sin_Function );
#endif
   } 

#endif

#ifndef INTARRAY
doubleSerialArray &
tan ( const doubleSerialArray & X )
   { 
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of tan for doubleSerialArray class! \n");

  // This is the only test we can do on the input!
     X.Test_Consistency ("Test X in tan ( const doubleSerialArray & X )");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in doubleSerialArray & tan(doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *X_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( X, X_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( X, X_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, X, tan(*X_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     doubleArray & Return_Value = X;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, X, X_SerialArray, tan(*X_SerialArray) );
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & tan(doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Unary_Operator ( X ,
                   MDI_d_Tan_Array ,
                   MDI_d_Tan_Array_Accumulate_To_Operand , doubleSerialArray::tan_Function );
#endif
   } 

#endif

#ifndef INTARRAY
doubleSerialArray &
acos ( const doubleSerialArray & X )
   { 
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of acos for doubleSerialArray class! \n");

  // This is the only test we can do on the input!
     X.Test_Consistency ("Test X in acos ( const doubleSerialArray & X )");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in doubleSerialArray & acos(doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *X_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( X, X_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( X, X_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, X, acos(*X_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     doubleArray & Return_Value = X;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, X, X_SerialArray, acos(*X_SerialArray) );
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & acos(doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Unary_Operator ( X ,
                   MDI_d_Arc_Cos_Array ,
                   MDI_d_Arc_Cos_Array_Accumulate_To_Operand , doubleSerialArray::acos_Function );
#endif
   } 

#endif

#ifndef INTARRAY
doubleSerialArray &
asin ( const doubleSerialArray & X )
   { 
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of asin for doubleSerialArray class! \n");

  // This is the only test we can do on the input!
     X.Test_Consistency ("Test X in asin ( const doubleSerialArray & X )");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in doubleSerialArray & asin(doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *X_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( X, X_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( X, X_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, X, asin(*X_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     doubleArray & Return_Value = X;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, X, X_SerialArray, asin(*X_SerialArray) );
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & asin(doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Unary_Operator ( X ,
                   MDI_d_Arc_Sin_Array ,
                   MDI_d_Arc_Sin_Array_Accumulate_To_Operand , doubleSerialArray::asin_Function );
#endif
   } 

#endif

#ifndef INTARRAY
doubleSerialArray &
atan ( const doubleSerialArray & X )
   { 
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of atan for doubleSerialArray class! \n");

  // This is the only test we can do on the input!
     X.Test_Consistency ("Test X in atan ( const doubleSerialArray & X )");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in doubleSerialArray & atan(doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *X_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( X, X_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( X, X_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, X, atan(*X_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     doubleArray & Return_Value = X;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, X, X_SerialArray, atan(*X_SerialArray) );
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & atan(doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Unary_Operator ( X ,
                   MDI_d_Arc_Tan_Array ,
                   MDI_d_Arc_Tan_Array_Accumulate_To_Operand , doubleSerialArray::atan_Function );
#endif
   } 

#endif

#ifndef INTARRAY
doubleSerialArray &
cosh ( const doubleSerialArray & X )
   { 
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of cosh for doubleSerialArray class! \n");

  // This is the only test we can do on the input!
     X.Test_Consistency ("Test X in cosh ( const doubleSerialArray & X )");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in doubleSerialArray & cosh(doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *X_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( X, X_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( X, X_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, X, cosh(*X_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     doubleArray & Return_Value = X;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, X, X_SerialArray, cosh(*X_SerialArray) );
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & cosh(doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Unary_Operator ( X ,
                   MDI_d_Cosh_Array ,
                   MDI_d_Cosh_Array_Accumulate_To_Operand , doubleSerialArray::cosh_Function );
#endif
   } 

#endif

#ifndef INTARRAY
doubleSerialArray &
sinh ( const doubleSerialArray & X )
   { 
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of sinh for doubleSerialArray class! \n");

  // This is the only test we can do on the input!
     X.Test_Consistency ("Test X in sinh ( const doubleSerialArray & X )");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in doubleSerialArray & sinh(doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *X_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( X, X_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( X, X_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, X, sinh(*X_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     doubleArray & Return_Value = X;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, X, X_SerialArray, sinh(*X_SerialArray) );
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & sinh(doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Unary_Operator ( X ,
                   MDI_d_Sinh_Array ,
                   MDI_d_Sinh_Array_Accumulate_To_Operand , doubleSerialArray::sinh_Function );
#endif
   } 

#endif

#ifndef INTARRAY
doubleSerialArray &
tanh ( const doubleSerialArray & X )
   { 
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of tanh for doubleSerialArray class! \n");

  // This is the only test we can do on the input!
     X.Test_Consistency ("Test X in tanh ( const doubleSerialArray & X )");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in doubleSerialArray & tanh(doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *X_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( X, X_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( X, X_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, X, tanh(*X_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     doubleArray & Return_Value = X;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, X, X_SerialArray, tanh(*X_SerialArray) );
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & tanh(doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Unary_Operator ( X ,
                   MDI_d_Tanh_Array ,
                   MDI_d_Tanh_Array_Accumulate_To_Operand , doubleSerialArray::tanh_Function );
#endif
   } 

#endif

#ifndef INTARRAY
doubleSerialArray &
acosh ( const doubleSerialArray & X )
   { 
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of acosh for doubleSerialArray class! \n");

  // This is the only test we can do on the input!
     X.Test_Consistency ("Test X in acosh ( const doubleSerialArray & X )");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in doubleSerialArray & acosh(doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *X_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( X, X_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( X, X_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, X, acosh(*X_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     doubleArray & Return_Value = X;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, X, X_SerialArray, acosh(*X_SerialArray) );
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & acosh(doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Unary_Operator ( X ,
                   MDI_d_Arc_Cosh_Array ,
                   MDI_d_Arc_Cosh_Array_Accumulate_To_Operand , doubleSerialArray::acosh_Function );
#endif
   } 

#endif

#ifndef INTARRAY
doubleSerialArray &
asinh ( const doubleSerialArray & X )
   { 
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of asinh for doubleSerialArray class! \n");

  // This is the only test we can do on the input!
     X.Test_Consistency ("Test X in asinh ( const doubleSerialArray & X )");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in doubleSerialArray & asinh(doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *X_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( X, X_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( X, X_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, X, asinh(*X_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     doubleArray & Return_Value = X;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, X, X_SerialArray, asinh(*X_SerialArray) );
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & asinh(doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Unary_Operator ( X ,
                   MDI_d_Arc_Sinh_Array ,
                   MDI_d_Arc_Sinh_Array_Accumulate_To_Operand , doubleSerialArray::asinh_Function );
#endif
   } 

#endif

doubleSerialArray &
doubleSerialArray::operator++ ()
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside doubleSerialArray::operator++ () for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in doubleSerialArray::operator++ -- Prefix operator");
#endif

     (*this) += 1;
     return *this;
   }
 
doubleSerialArray &
doubleSerialArray::operator++ ( int x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside doubleSerialArray::operator%s (int=%d) for doubleSerialArray class! \n","++",x);

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in doubleSerialArray::operator++(x=0) -- Postfix operator");
#endif

  // Postfix operator always passes zero as argument (strange but true -- See Stroustrup p594)
     APP_ASSERT( x == 0 );
     (*this) += 1;
     return *this;
   }


#ifndef INTARRAY
doubleSerialArray &
atanh ( const doubleSerialArray & X )
   { 
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of atanh for doubleSerialArray class! \n");

  // This is the only test we can do on the input!
     X.Test_Consistency ("Test X in atanh ( const doubleSerialArray & X )");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in doubleSerialArray & atanh(doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *X_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( X, X_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( X, X_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, X, atanh(*X_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     doubleArray & Return_Value = X;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, X, X_SerialArray, atanh(*X_SerialArray) );
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & atanh(doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Unary_Operator ( X ,
                   MDI_d_Arc_Tanh_Array ,
                   MDI_d_Arc_Tanh_Array_Accumulate_To_Operand , doubleSerialArray::atanh_Function );
#endif
   } 

#endif

#ifndef INTARRAY
doubleSerialArray &
log ( const doubleSerialArray & X )
   { 
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of log for doubleSerialArray class! \n");

  // This is the only test we can do on the input!
     X.Test_Consistency ("Test X in log ( const doubleSerialArray & X )");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in doubleSerialArray & log(doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *X_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( X, X_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( X, X_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, X, log(*X_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     doubleArray & Return_Value = X;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, X, X_SerialArray, log(*X_SerialArray) );
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & log(doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Unary_Operator ( X ,
                   MDI_d_Log_Array ,
                   MDI_d_Log_Array_Accumulate_To_Operand , doubleSerialArray::log_Function );
#endif
   } 

#endif

#ifndef INTARRAY
doubleSerialArray &
log10 ( const doubleSerialArray & X )
   { 
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of log10 for doubleSerialArray class! \n");

  // This is the only test we can do on the input!
     X.Test_Consistency ("Test X in log10 ( const doubleSerialArray & X )");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in doubleSerialArray & log10(doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *X_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( X, X_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( X, X_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, X, log10(*X_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     doubleArray & Return_Value = X;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, X, X_SerialArray, log10(*X_SerialArray) );
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & log10(doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Unary_Operator ( X ,
                   MDI_d_Log10_Array ,
                   MDI_d_Log10_Array_Accumulate_To_Operand , doubleSerialArray::log10_Function );
#endif
   } 

#endif

#ifndef INTARRAY
doubleSerialArray &
exp ( const doubleSerialArray & X )
   { 
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of exp for doubleSerialArray class! \n");

  // This is the only test we can do on the input!
     X.Test_Consistency ("Test X in exp ( const doubleSerialArray & X )");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in doubleSerialArray & exp(doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *X_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( X, X_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( X, X_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, X, exp(*X_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     doubleArray & Return_Value = X;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, X, X_SerialArray, exp(*X_SerialArray) );
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & exp(doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Unary_Operator ( X ,
                   MDI_d_Exp_Array ,
                   MDI_d_Exp_Array_Accumulate_To_Operand , doubleSerialArray::exp_Function );
#endif
   } 

#endif

#ifndef INTARRAY
doubleSerialArray &
sqrt ( const doubleSerialArray & X )
   { 
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of sqrt for doubleSerialArray class! \n");

  // This is the only test we can do on the input!
     X.Test_Consistency ("Test X in sqrt ( const doubleSerialArray & X )");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in doubleSerialArray & sqrt(doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *X_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( X, X_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( X, X_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, X, sqrt(*X_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     doubleArray & Return_Value = X;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, X, X_SerialArray, sqrt(*X_SerialArray) );
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & sqrt(doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Unary_Operator ( X ,
                   MDI_d_Sqrt_Array ,
                   MDI_d_Sqrt_Array_Accumulate_To_Operand , doubleSerialArray::sqrt_Function );
#endif
   } 

#endif

#ifndef INTARRAY
doubleSerialArray &
fabs ( const doubleSerialArray & X )
   { 
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of fabs for doubleSerialArray class! \n");

  // This is the only test we can do on the input!
     X.Test_Consistency ("Test X in fabs ( const doubleSerialArray & X )");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in doubleSerialArray & fabs(doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *X_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( X, X_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( X, X_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, X, fabs(*X_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     doubleArray & Return_Value = X;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, X, X_SerialArray, fabs(*X_SerialArray) );
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & fabs(doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Unary_Operator ( X ,
                   MDI_d_Fabs_Array ,
                   MDI_d_Fabs_Array_Accumulate_To_Operand , doubleSerialArray::fabs_Function );
#endif
   } 

doubleSerialArray &
abs ( const doubleSerialArray & X )
   { 
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of abs for doubleSerialArray class! \n");

  // This is the only test we can do on the input!
     X.Test_Consistency ("Test X in abs ( const doubleSerialArray & X )");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in doubleSerialArray & abs(doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *X_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( X, X_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( X, X_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, X, abs(*X_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     doubleArray & Return_Value = X;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, X, X_SerialArray, abs(*X_SerialArray) );
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & abs(doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Unary_Operator ( X ,
                   MDI_d_Fabs_Array ,
                   MDI_d_Fabs_Array_Accumulate_To_Operand , doubleSerialArray::abs_Function );
#endif
   } 

#else
doubleSerialArray &
abs ( const doubleSerialArray & X )
   { 
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of abs for doubleSerialArray class! \n");

  // This is the only test we can do on the input!
     X.Test_Consistency ("Test X in abs ( const doubleSerialArray & X )");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in doubleSerialArray & abs(doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *X_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( X, X_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( X, X_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, X, abs(*X_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     doubleArray & Return_Value = X;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, X, X_SerialArray, abs(*X_SerialArray) );
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & abs(doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Unary_Operator ( X ,
                   MDI_d_Abs_Array ,
                   MDI_d_Abs_Array_Accumulate_To_Operand , doubleSerialArray::abs_Function );
#endif
   } 

#endif

#ifndef INTARRAY
doubleSerialArray &
ceil ( const doubleSerialArray & X )
   { 
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of ceil for doubleSerialArray class! \n");

  // This is the only test we can do on the input!
     X.Test_Consistency ("Test X in ceil ( const doubleSerialArray & X )");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in doubleSerialArray & ceil(doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *X_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( X, X_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( X, X_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, X, ceil(*X_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     doubleArray & Return_Value = X;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, X, X_SerialArray, ceil(*X_SerialArray) );
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & ceil(doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Unary_Operator ( X ,
                   MDI_d_Ceil_Array ,
                   MDI_d_Ceil_Array_Accumulate_To_Operand , doubleSerialArray::ceil_Function );
#endif
   } 

#endif

#ifndef INTARRAY
doubleSerialArray &
floor ( const doubleSerialArray & X )
   { 
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of floor for doubleSerialArray class! \n");

  // This is the only test we can do on the input!
     X.Test_Consistency ("Test X in floor ( const doubleSerialArray & X )");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in doubleSerialArray & floor(doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *X_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( X, X_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( X, X_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (X, X_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, X, floor(*X_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     doubleArray & Return_Value = X;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, X, X_SerialArray, floor(*X_SerialArray) );
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & floor(doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Unary_Operator ( X ,
                   MDI_d_Floor_Array ,
                   MDI_d_Floor_Array_Accumulate_To_Operand , doubleSerialArray::floor_Function );
#endif
   } 

#endif

#ifndef INTARRAY
doubleSerialArray &
doubleSerialArray::replace ( const intSerialArray & Lhs , const doubleSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of doubleSerialArray::replace (intSerialArray,doubleSerialArray) for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in doubleSerialArray::replace (intSerialArray,doubleSerialArray)");
     Lhs.Test_Consistency ("Test Lhs in doubleSerialArray::replace (intSerialArray,doubleSerialArray)");
     Rhs.Test_Consistency ("Test Rhs in doubleSerialArray::replace (intSerialArray,doubleSerialArray)");
#endif

  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Lhs.Test_Conformability (Rhs);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in doubleSerialArray & doubleSerialArray::replace(intSerialArray,doubleSerialArray)");
          Rhs.displayReferenceCounts("Rhs in doubleSerialArray & doubleSerialArray::replace(intSerialArray,doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray  *This_SerialArray = NULL;
     intSerialArray *Lhs_SerialArray = NULL;
     doubleSerialArray  *Rhs_SerialArray = NULL;
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
            doubleSerialArray::Parallel_Conformability_Enforcement 
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
     doubleArray & Return_Value = Rhs;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Modification_Operator ( Temporary_Array_Set, *this, Lhs, Rhs,
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & doubleSerialArray::replace(intSerialArray,doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Modification_Operator ( Lhs , Rhs , MDI_d_If_Array_Use_Array , doubleSerialArray::replace_Function );
#endif
   }

#endif

#ifndef INTARRAY
doubleSerialArray &
doubleSerialArray::replace ( const intSerialArray & Lhs , double x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of doubleSerialArray::replace (intSerialArray,x) for doubleSerialArray class! \n");

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in doubleSerialArray::replace (intSerialArray,double)");
     Lhs.Test_Consistency ("Test Lhs in doubleSerialArray::replace (intSerialArray,double)");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in doubleSerialArray & doubleSerialArray::replace(intSerialArray,double)");
        }
#endif

#if defined(PPP)
     doubleSerialArray  *This_SerialArray = NULL;
     intSerialArray *Lhs_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if ((Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	 (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
     {
        Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement ( *this, This_SerialArray, Lhs, Lhs_SerialArray );
     }
     else
     {
        Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement ( *this, This_SerialArray, Lhs, Lhs_SerialArray );
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
     printf ("Can't do MEMORY_LEAK_TEST in doubleSerialArray & doubleSerialArray::replace ( const intSerialArray & Lhs , double x )");
#endif
     doubleArray & Return_Value = doubleArray::Abstract_Modification_Operator ( Temporary_Array_Set, *this, Lhs,
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & doubleSerialArray::replace(intSerialArray,double)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Modification_Operator ( Lhs , x , MDI_d_If_Array_Use_Scalar , doubleSerialArray::Scalar_replace_Function );
#endif
   }

#endif

doubleSerialArray &
doubleSerialArray::operator+= ( const doubleSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of doubleSerialArray::operator+= (doubleSerialArray) for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in doubleSerialArray::operator+=");
     Rhs.Test_Consistency ("Test Rhs in doubleSerialArray::operator+=");
#endif

  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Test_Conformability (Rhs);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS in doubleSerialArray & doubleSerialArray::operator+=(doubleSerialArray)");
          Rhs.displayReferenceCounts("Rhs in doubleSerialArray & doubleSerialArray::operator+=(doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *This_SerialArray = NULL;
     doubleSerialArray *Rhs_SerialArray  = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if ((Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	    (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      ( *this , This_SerialArray, Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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
     doubleArray::Abstract_Modification_Operator ( Temporary_Array_Set, *this, Rhs, This_SerialArray, Rhs_SerialArray, *This_SerialArray += *Rhs_SerialArray );
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
     doubleSerialArray::Abstract_Operator_Operation_Equals ( *this , Rhs ,
               MDI_d_Add_Array_Plus_Array_Accumulate_To_Operand , doubleSerialArray::Plus_Equals );
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS (return value) in doubleSerialArray & doubleSerialArray::operator+=(doubleSerialArray)");
        }
#endif

     return *this;
   }


#ifndef INTARRAY
doubleSerialArray &
doubleSerialArray::replace ( int x , const doubleSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of doubleSerialArray::replace (x,doubleSerialArray) for doubleSerialArray class! \n");

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in doubleSerialArray::replace (int,doubleSerialArray)");
     Rhs.Test_Consistency ("Test Rhs in doubleSerialArray::replace (int,doubleSerialArray)");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs.displayReferenceCounts("Rhs in doubleSerialArray & doubleSerialArray::replace(int,doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *This_SerialArray = NULL;
     doubleSerialArray *Rhs_SerialArray  = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if ((Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	    (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
        {
           Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement( *this, This_SerialArray, Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement ( *this, This_SerialArray, Rhs, Rhs_SerialArray );
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
           Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement 
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
     doubleArray & Return_Value = Rhs;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Modification_Operator ( Temporary_Array_Set, *this, Rhs,
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & doubleSerialArray::replace(int,doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Modification_Operator ( Rhs , x , MDI_d_If_Scalar_Use_Array , Scalar_replace_Function );
#endif
   }

#endif

#ifndef INTARRAY
doubleSerialArray &
fmod ( const doubleSerialArray & Lhs , const doubleSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of fmod (doubleSerialArray,doubleSerialArray) for doubleSerialArray class! Lhs:rc=%d Rhs:rc=%d ",
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
          Lhs.displayReferenceCounts("Lhs in doubleSerialArray & fmod(doubleSerialArray,doubleSerialArray)");
          Rhs.displayReferenceCounts("Rhs in doubleSerialArray & fmod(doubleSerialArray,doubleSerialArray)");
        }
#endif

#if defined(PPP)
  // Pointers to views of Lhs and Rhs serial arrays which allow a conformable operation
     doubleSerialArray *Lhs_SerialArray = NULL;
     doubleSerialArray *Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...
     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	      (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
               Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement (Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement (Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
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
               Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in doubleSerialArray & fmod(doubleSerialArray,doubleSerialArray)");
          Rhs_SerialArray->displayReferenceCounts("Rhs_SerialArray in doubleSerialArray & fmod(doubleSerialArray,doubleSerialArray)");
        }
#endif

  // Inputs to doubleArray::Abstract_Binary_Operator:
  //     1. Temporary_Array_Set is attached to the doubleArray temporary returned by Abstract_Binary_Operator
  //     2. Lhs is used to get the Lhs partition information (PARTI parallel descriptor) and array reuse
  //     3. Rhs is used to get the Rhs partition information (PARTI parallel descriptor) in case the Lhs was 
  //        a NULL array (no data and no defined partitioning (i.e. no PARTI parallel descriptor)) and array reuse
  //     4. The doubleSerialArray which is to be put into the doubleArray temporary returned by Abstract_Binary_Operator
  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, fmod(*Lhs_SerialArray,*Rhs_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     doubleArray & Return_Value = Rhs;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, Lhs_SerialArray, Rhs_SerialArray, fmod(*Lhs_SerialArray,*Rhs_SerialArray) );
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & fmod(doubleSerialArray,doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
  // This function can be either Abstract_Binary_Operator or Abstract_Binary_Operator_Non_Commutative
     return doubleSerialArray::Abstract_Binary_Operator_Non_Commutative ( Lhs , Rhs , MDI_d_Fmod_Array_Modulo_Array, MDI_d_Fmod_Array_Modulo_Array_Accumulate_To_Operand , doubleSerialArray::fmod_Function );
#endif
   }



doubleSerialArray &
mod ( const doubleSerialArray & Lhs , const doubleSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of mod (doubleSerialArray,doubleSerialArray) for doubleSerialArray class! Lhs:rc=%d Rhs:rc=%d ",
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
          Lhs.displayReferenceCounts("Lhs in doubleSerialArray & mod(doubleSerialArray,doubleSerialArray)");
          Rhs.displayReferenceCounts("Rhs in doubleSerialArray & mod(doubleSerialArray,doubleSerialArray)");
        }
#endif

#if defined(PPP)
  // Pointers to views of Lhs and Rhs serial arrays which allow a conformable operation
     doubleSerialArray *Lhs_SerialArray = NULL;
     doubleSerialArray *Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...
     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	      (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
               Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement (Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement (Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
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
               Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in doubleSerialArray & mod(doubleSerialArray,doubleSerialArray)");
          Rhs_SerialArray->displayReferenceCounts("Rhs_SerialArray in doubleSerialArray & mod(doubleSerialArray,doubleSerialArray)");
        }
#endif

  // Inputs to doubleArray::Abstract_Binary_Operator:
  //     1. Temporary_Array_Set is attached to the doubleArray temporary returned by Abstract_Binary_Operator
  //     2. Lhs is used to get the Lhs partition information (PARTI parallel descriptor) and array reuse
  //     3. Rhs is used to get the Rhs partition information (PARTI parallel descriptor) in case the Lhs was 
  //        a NULL array (no data and no defined partitioning (i.e. no PARTI parallel descriptor)) and array reuse
  //     4. The doubleSerialArray which is to be put into the doubleArray temporary returned by Abstract_Binary_Operator
  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, mod(*Lhs_SerialArray,*Rhs_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     doubleArray & Return_Value = Rhs;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, Lhs_SerialArray, Rhs_SerialArray, mod(*Lhs_SerialArray,*Rhs_SerialArray) );
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & mod(doubleSerialArray,doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
  // This function can be either Abstract_Binary_Operator or Abstract_Binary_Operator_Non_Commutative
     return doubleSerialArray::Abstract_Binary_Operator_Non_Commutative ( Lhs , Rhs , MDI_d_Fmod_Array_Modulo_Array, MDI_d_Fmod_Array_Modulo_Array_Accumulate_To_Operand , doubleSerialArray::mod_Function );
#endif
   }



#else
doubleSerialArray &
mod ( const doubleSerialArray & Lhs , const doubleSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of mod (doubleSerialArray,doubleSerialArray) for doubleSerialArray class! Lhs:rc=%d Rhs:rc=%d ",
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
          Lhs.displayReferenceCounts("Lhs in doubleSerialArray & mod(doubleSerialArray,doubleSerialArray)");
          Rhs.displayReferenceCounts("Rhs in doubleSerialArray & mod(doubleSerialArray,doubleSerialArray)");
        }
#endif

#if defined(PPP)
  // Pointers to views of Lhs and Rhs serial arrays which allow a conformable operation
     doubleSerialArray *Lhs_SerialArray = NULL;
     doubleSerialArray *Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...
     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	      (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
               Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement (Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement (Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
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
               Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in doubleSerialArray & mod(doubleSerialArray,doubleSerialArray)");
          Rhs_SerialArray->displayReferenceCounts("Rhs_SerialArray in doubleSerialArray & mod(doubleSerialArray,doubleSerialArray)");
        }
#endif

  // Inputs to doubleArray::Abstract_Binary_Operator:
  //     1. Temporary_Array_Set is attached to the doubleArray temporary returned by Abstract_Binary_Operator
  //     2. Lhs is used to get the Lhs partition information (PARTI parallel descriptor) and array reuse
  //     3. Rhs is used to get the Rhs partition information (PARTI parallel descriptor) in case the Lhs was 
  //        a NULL array (no data and no defined partitioning (i.e. no PARTI parallel descriptor)) and array reuse
  //     4. The doubleSerialArray which is to be put into the doubleArray temporary returned by Abstract_Binary_Operator
  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, mod(*Lhs_SerialArray,*Rhs_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     doubleArray & Return_Value = Rhs;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, Lhs_SerialArray, Rhs_SerialArray, mod(*Lhs_SerialArray,*Rhs_SerialArray) );
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & mod(doubleSerialArray,doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
  // This function can be either Abstract_Binary_Operator or Abstract_Binary_Operator_Non_Commutative
     return doubleSerialArray::Abstract_Binary_Operator_Non_Commutative ( Lhs , Rhs , MDI_d_Fmod_Array_Modulo_Array, MDI_d_Fmod_Array_Modulo_Array_Accumulate_To_Operand , doubleSerialArray::mod_Function );
#endif
   }



#endif

#ifndef INTARRAY
doubleSerialArray &
fmod ( double x , const doubleSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("\n\n\nInside of fmod (double,doubleSerialArray) for doubleSerialArray class! \n");

  // This is the only test we can do on the input!
     Rhs.Test_Consistency ("Test Rhs in fmod");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs.displayReferenceCounts("Rhs in doubleSerialArray & fmod(double,doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...
     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement
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
          Rhs_SerialArray->displayReferenceCounts("Rhs_SerialArray in doubleSerialArray & fmod(double,doubleSerialArray)");
        }
#endif

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, Rhs, fmod(x,*Rhs_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     doubleArray & Return_Value = Rhs;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Rhs, Rhs_SerialArray, fmod(x,*Rhs_SerialArray) );
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value (before delete macro) in doubleSerialArray & fmod(double,doubleSerialArray)");
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & fmod(double,doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
  // This function can be either Abstract_Binary_Operator or Abstract_Binary_Operator_Non_Commutative
     return doubleSerialArray::Abstract_Binary_Operator_Non_Commutative ( Rhs , x ,
               MDI_d_Fmod_Scalar_Modulo_Array,
               MDI_d_Fmod_Scalar_Modulo_Array_Accumulate_To_Operand , doubleSerialArray::Scalar_fmod_Function );
#endif
   }



doubleSerialArray &
mod ( double x , const doubleSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("\n\n\nInside of mod (double,doubleSerialArray) for doubleSerialArray class! \n");

  // This is the only test we can do on the input!
     Rhs.Test_Consistency ("Test Rhs in mod");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs.displayReferenceCounts("Rhs in doubleSerialArray & mod(double,doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...
     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement
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
          Rhs_SerialArray->displayReferenceCounts("Rhs_SerialArray in doubleSerialArray & mod(double,doubleSerialArray)");
        }
#endif

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, Rhs, mod(x,*Rhs_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     doubleArray & Return_Value = Rhs;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Rhs, Rhs_SerialArray, mod(x,*Rhs_SerialArray) );
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value (before delete macro) in doubleSerialArray & mod(double,doubleSerialArray)");
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & mod(double,doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
  // This function can be either Abstract_Binary_Operator or Abstract_Binary_Operator_Non_Commutative
     return doubleSerialArray::Abstract_Binary_Operator_Non_Commutative ( Rhs , x ,
               MDI_d_Fmod_Scalar_Modulo_Array,
               MDI_d_Fmod_Scalar_Modulo_Array_Accumulate_To_Operand , doubleSerialArray::Scalar_mod_Function );
#endif
   }



#else
doubleSerialArray &
mod ( double x , const doubleSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("\n\n\nInside of mod (double,doubleSerialArray) for doubleSerialArray class! \n");

  // This is the only test we can do on the input!
     Rhs.Test_Consistency ("Test Rhs in mod");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs.displayReferenceCounts("Rhs in doubleSerialArray & mod(double,doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...
     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement
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
          Rhs_SerialArray->displayReferenceCounts("Rhs_SerialArray in doubleSerialArray & mod(double,doubleSerialArray)");
        }
#endif

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, Rhs, mod(x,*Rhs_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     doubleArray & Return_Value = Rhs;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Rhs, Rhs_SerialArray, mod(x,*Rhs_SerialArray) );
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value (before delete macro) in doubleSerialArray & mod(double,doubleSerialArray)");
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & mod(double,doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
  // This function can be either Abstract_Binary_Operator or Abstract_Binary_Operator_Non_Commutative
     return doubleSerialArray::Abstract_Binary_Operator_Non_Commutative ( Rhs , x ,
               MDI_d_Fmod_Scalar_Modulo_Array,
               MDI_d_Fmod_Scalar_Modulo_Array_Accumulate_To_Operand , doubleSerialArray::Scalar_mod_Function );
#endif
   }



#endif

#ifndef INTARRAY
doubleSerialArray &
fmod ( const doubleSerialArray & Lhs , double x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of fmod (doubleSerialArray,double) for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in fmod");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts 
          Lhs.displayReferenceCounts("Lhs in doubleSerialArray & fmod(doubleSerialArray,double)");
        }
#endif

#if defined(PPP)
  // Pointers to views of Lhs serial arrays which allow a conformable operation
     doubleSerialArray *Lhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE)
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in doubleSerialArray & fmod(doubleSerialArray,double)");
        }
#endif

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, fmod(*Lhs_SerialArray,x) );
#if defined(MEMORY_LEAK_TEST)
     puts ("Can't do MEMORY_LEAK_TEST in doubleSerialArray & fmod ( const doubleSerialArray & Lhs , double x )");
#endif
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Lhs_SerialArray, fmod(*Lhs_SerialArray,x) );

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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & fmod(doubleSerialArray,double)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator ( Lhs , x ,
               MDI_d_Fmod_Array_Modulo_Scalar,
               MDI_d_Fmod_Array_Modulo_Scalar_Accumulate_To_Operand , doubleSerialArray::Scalar_fmod_Function );
#endif
   }

doubleSerialArray &
mod ( const doubleSerialArray & Lhs , double x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of mod (doubleSerialArray,double) for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in mod");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts 
          Lhs.displayReferenceCounts("Lhs in doubleSerialArray & mod(doubleSerialArray,double)");
        }
#endif

#if defined(PPP)
  // Pointers to views of Lhs serial arrays which allow a conformable operation
     doubleSerialArray *Lhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE)
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in doubleSerialArray & mod(doubleSerialArray,double)");
        }
#endif

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, mod(*Lhs_SerialArray,x) );
#if defined(MEMORY_LEAK_TEST)
     puts ("Can't do MEMORY_LEAK_TEST in doubleSerialArray & mod ( const doubleSerialArray & Lhs , double x )");
#endif
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Lhs_SerialArray, mod(*Lhs_SerialArray,x) );

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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & mod(doubleSerialArray,double)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator ( Lhs , x ,
               MDI_d_Fmod_Array_Modulo_Scalar,
               MDI_d_Fmod_Array_Modulo_Scalar_Accumulate_To_Operand , doubleSerialArray::Scalar_mod_Function );
#endif
   }

#else
doubleSerialArray &
mod ( const doubleSerialArray & Lhs , double x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of mod (doubleSerialArray,double) for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in mod");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts 
          Lhs.displayReferenceCounts("Lhs in doubleSerialArray & mod(doubleSerialArray,double)");
        }
#endif

#if defined(PPP)
  // Pointers to views of Lhs serial arrays which allow a conformable operation
     doubleSerialArray *Lhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE)
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in doubleSerialArray & mod(doubleSerialArray,double)");
        }
#endif

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, mod(*Lhs_SerialArray,x) );
#if defined(MEMORY_LEAK_TEST)
     puts ("Can't do MEMORY_LEAK_TEST in doubleSerialArray & mod ( const doubleSerialArray & Lhs , double x )");
#endif
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Lhs_SerialArray, mod(*Lhs_SerialArray,x) );

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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & mod(doubleSerialArray,double)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator ( Lhs , x ,
               MDI_d_Fmod_Array_Modulo_Scalar,
               MDI_d_Fmod_Array_Modulo_Scalar_Accumulate_To_Operand , doubleSerialArray::Scalar_mod_Function );
#endif
   }

#endif

doubleSerialArray &
pow ( const doubleSerialArray & Lhs , const doubleSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of pow (doubleSerialArray,doubleSerialArray) for doubleSerialArray class! Lhs:rc=%d Rhs:rc=%d ",
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
          Lhs.displayReferenceCounts("Lhs in doubleSerialArray & pow(doubleSerialArray,doubleSerialArray)");
          Rhs.displayReferenceCounts("Rhs in doubleSerialArray & pow(doubleSerialArray,doubleSerialArray)");
        }
#endif

#if defined(PPP)
  // Pointers to views of Lhs and Rhs serial arrays which allow a conformable operation
     doubleSerialArray *Lhs_SerialArray = NULL;
     doubleSerialArray *Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...
     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	      (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
               Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement (Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement (Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
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
               Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in doubleSerialArray & pow(doubleSerialArray,doubleSerialArray)");
          Rhs_SerialArray->displayReferenceCounts("Rhs_SerialArray in doubleSerialArray & pow(doubleSerialArray,doubleSerialArray)");
        }
#endif

  // Inputs to doubleArray::Abstract_Binary_Operator:
  //     1. Temporary_Array_Set is attached to the doubleArray temporary returned by Abstract_Binary_Operator
  //     2. Lhs is used to get the Lhs partition information (PARTI parallel descriptor) and array reuse
  //     3. Rhs is used to get the Rhs partition information (PARTI parallel descriptor) in case the Lhs was 
  //        a NULL array (no data and no defined partitioning (i.e. no PARTI parallel descriptor)) and array reuse
  //     4. The doubleSerialArray which is to be put into the doubleArray temporary returned by Abstract_Binary_Operator
  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, pow(*Lhs_SerialArray,*Rhs_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     doubleArray & Return_Value = Rhs;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, Lhs_SerialArray, Rhs_SerialArray, pow(*Lhs_SerialArray,*Rhs_SerialArray) );
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & pow(doubleSerialArray,doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
  // This function can be either Abstract_Binary_Operator or Abstract_Binary_Operator_Non_Commutative
     return doubleSerialArray::Abstract_Binary_Operator_Non_Commutative ( Lhs , Rhs , MDI_d_Pow_Array_Raised_To_Array, MDI_d_Pow_Array_Raised_To_Array_Accumulate_To_Operand , doubleSerialArray::pow_Function );
#endif
   }




doubleSerialArray &
pow ( double x , const doubleSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("\n\n\nInside of pow (double,doubleSerialArray) for doubleSerialArray class! \n");

  // This is the only test we can do on the input!
     Rhs.Test_Consistency ("Test Rhs in pow");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs.displayReferenceCounts("Rhs in doubleSerialArray & pow(double,doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...
     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement
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
          Rhs_SerialArray->displayReferenceCounts("Rhs_SerialArray in doubleSerialArray & pow(double,doubleSerialArray)");
        }
#endif

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, Rhs, pow(x,*Rhs_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     doubleArray & Return_Value = Rhs;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Rhs, Rhs_SerialArray, pow(x,*Rhs_SerialArray) );
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value (before delete macro) in doubleSerialArray & pow(double,doubleSerialArray)");
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & pow(double,doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
  // This function can be either Abstract_Binary_Operator or Abstract_Binary_Operator_Non_Commutative
     return doubleSerialArray::Abstract_Binary_Operator_Non_Commutative ( Rhs , x ,
               MDI_d_Pow_Scalar_Raised_To_Array,
               MDI_d_Pow_Scalar_Raised_To_Array_Accumulate_To_Operand , doubleSerialArray::Scalar_pow_Function );
#endif
   }




doubleSerialArray &
pow ( const doubleSerialArray & Lhs , double x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of pow (doubleSerialArray,double) for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in pow");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts 
          Lhs.displayReferenceCounts("Lhs in doubleSerialArray & pow(doubleSerialArray,double)");
        }
#endif

#if defined(PPP)
  // Pointers to views of Lhs serial arrays which allow a conformable operation
     doubleSerialArray *Lhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE)
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in doubleSerialArray & pow(doubleSerialArray,double)");
        }
#endif

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, pow(*Lhs_SerialArray,x) );
#if defined(MEMORY_LEAK_TEST)
     puts ("Can't do MEMORY_LEAK_TEST in doubleSerialArray & pow ( const doubleSerialArray & Lhs , double x )");
#endif
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Lhs_SerialArray, pow(*Lhs_SerialArray,x) );

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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & pow(doubleSerialArray,double)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator ( Lhs , x ,
               MDI_d_Pow_Array_Raised_To_Scalar,
               MDI_d_Pow_Array_Raised_To_Scalar_Accumulate_To_Operand , doubleSerialArray::Scalar_pow_Function );
#endif
   }


doubleSerialArray &
sign ( const doubleSerialArray & Lhs , const doubleSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of sign (doubleSerialArray,doubleSerialArray) for doubleSerialArray class! Lhs:rc=%d Rhs:rc=%d ",
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
          Lhs.displayReferenceCounts("Lhs in doubleSerialArray & sign(doubleSerialArray,doubleSerialArray)");
          Rhs.displayReferenceCounts("Rhs in doubleSerialArray & sign(doubleSerialArray,doubleSerialArray)");
        }
#endif

#if defined(PPP)
  // Pointers to views of Lhs and Rhs serial arrays which allow a conformable operation
     doubleSerialArray *Lhs_SerialArray = NULL;
     doubleSerialArray *Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...
     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	      (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
               Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement (Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement (Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
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
               Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in doubleSerialArray & sign(doubleSerialArray,doubleSerialArray)");
          Rhs_SerialArray->displayReferenceCounts("Rhs_SerialArray in doubleSerialArray & sign(doubleSerialArray,doubleSerialArray)");
        }
#endif

  // Inputs to doubleArray::Abstract_Binary_Operator:
  //     1. Temporary_Array_Set is attached to the doubleArray temporary returned by Abstract_Binary_Operator
  //     2. Lhs is used to get the Lhs partition information (PARTI parallel descriptor) and array reuse
  //     3. Rhs is used to get the Rhs partition information (PARTI parallel descriptor) in case the Lhs was 
  //        a NULL array (no data and no defined partitioning (i.e. no PARTI parallel descriptor)) and array reuse
  //     4. The doubleSerialArray which is to be put into the doubleArray temporary returned by Abstract_Binary_Operator
  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, sign(*Lhs_SerialArray,*Rhs_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     doubleArray & Return_Value = Rhs;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, Lhs_SerialArray, Rhs_SerialArray, sign(*Lhs_SerialArray,*Rhs_SerialArray) );
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & sign(doubleSerialArray,doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
  // This function can be either Abstract_Binary_Operator or Abstract_Binary_Operator_Non_Commutative
     return doubleSerialArray::Abstract_Binary_Operator_Non_Commutative ( Lhs , Rhs , MDI_d_Sign_Array_Of_Array, MDI_d_Sign_Array_Of_Array_Accumulate_To_Operand , doubleSerialArray::sign_Function );
#endif
   }




doubleSerialArray &
sign ( double x , const doubleSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("\n\n\nInside of sign (double,doubleSerialArray) for doubleSerialArray class! \n");

  // This is the only test we can do on the input!
     Rhs.Test_Consistency ("Test Rhs in sign");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs.displayReferenceCounts("Rhs in doubleSerialArray & sign(double,doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...
     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement
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
          Rhs_SerialArray->displayReferenceCounts("Rhs_SerialArray in doubleSerialArray & sign(double,doubleSerialArray)");
        }
#endif

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, Rhs, sign(x,*Rhs_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     doubleArray & Return_Value = Rhs;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Rhs, Rhs_SerialArray, sign(x,*Rhs_SerialArray) );
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value (before delete macro) in doubleSerialArray & sign(double,doubleSerialArray)");
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & sign(double,doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
  // This function can be either Abstract_Binary_Operator or Abstract_Binary_Operator_Non_Commutative
     return doubleSerialArray::Abstract_Binary_Operator_Non_Commutative ( Rhs , x ,
               MDI_d_Sign_Scalar_Of_Array,
               MDI_d_Sign_Scalar_Of_Array_Accumulate_To_Operand , doubleSerialArray::Scalar_sign_Function );
#endif
   }




doubleSerialArray &
sign ( const doubleSerialArray & Lhs , double x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of sign (doubleSerialArray,double) for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in sign");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts 
          Lhs.displayReferenceCounts("Lhs in doubleSerialArray & sign(doubleSerialArray,double)");
        }
#endif

#if defined(PPP)
  // Pointers to views of Lhs serial arrays which allow a conformable operation
     doubleSerialArray *Lhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE)
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in doubleSerialArray & sign(doubleSerialArray,double)");
        }
#endif

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, sign(*Lhs_SerialArray,x) );
#if defined(MEMORY_LEAK_TEST)
     puts ("Can't do MEMORY_LEAK_TEST in doubleSerialArray & sign ( const doubleSerialArray & Lhs , double x )");
#endif
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Lhs_SerialArray, sign(*Lhs_SerialArray,x) );

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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & sign(doubleSerialArray,double)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator ( Lhs , x ,
               MDI_d_Sign_Array_Of_Scalar,
               MDI_d_Sign_Array_Of_Scalar_Accumulate_To_Operand , doubleSerialArray::Scalar_sign_Function );
#endif
   }


doubleSerialArray &
doubleSerialArray::operator+= ( double x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of doubleSerialArray::operator+= (double) for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in doubleSerialArray::operator+=");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS in doubleSerialArray & doubleSerialArray::operator+=(double)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *This_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( *this , This_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( *this , This_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (*this, This_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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
     doubleArray::Abstract_Modification_Operator ( Temporary_Array_Set, *this, This_SerialArray, *This_SerialArray += x );
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
     doubleSerialArray::Abstract_Operator_Operation_Equals ( *this , x ,
        MDI_d_Add_Array_Plus_Scalar_Accumulate_To_Operand , doubleSerialArray::Scalar_Plus_Equals );
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS (return value) in doubleSerialArray & doubleSerialArray::operator+=(double)");
        }
#endif

     return *this;
   }


// Most C++ compliers support a unary plus operator
doubleSerialArray &
doubleSerialArray::operator+ () const
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of unary plus doubleSerialArray::operator+ () for doubleSerialArray class! \n");
#endif

  // return *this;
     return (doubleSerialArray &)(*this);
   }


doubleSerialArray &
min ( const doubleSerialArray & Lhs , const doubleSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of min (doubleSerialArray,doubleSerialArray) for doubleSerialArray class! Lhs:rc=%d Rhs:rc=%d ",
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
          Lhs.displayReferenceCounts("Lhs in doubleSerialArray & min(doubleSerialArray,doubleSerialArray)");
          Rhs.displayReferenceCounts("Rhs in doubleSerialArray & min(doubleSerialArray,doubleSerialArray)");
        }
#endif

#if defined(PPP)
  // Pointers to views of Lhs and Rhs serial arrays which allow a conformable operation
     doubleSerialArray *Lhs_SerialArray = NULL;
     doubleSerialArray *Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...
     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	      (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
               Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement (Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement (Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
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
               Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in doubleSerialArray & min(doubleSerialArray,doubleSerialArray)");
          Rhs_SerialArray->displayReferenceCounts("Rhs_SerialArray in doubleSerialArray & min(doubleSerialArray,doubleSerialArray)");
        }
#endif

  // Inputs to doubleArray::Abstract_Binary_Operator:
  //     1. Temporary_Array_Set is attached to the doubleArray temporary returned by Abstract_Binary_Operator
  //     2. Lhs is used to get the Lhs partition information (PARTI parallel descriptor) and array reuse
  //     3. Rhs is used to get the Rhs partition information (PARTI parallel descriptor) in case the Lhs was 
  //        a NULL array (no data and no defined partitioning (i.e. no PARTI parallel descriptor)) and array reuse
  //     4. The doubleSerialArray which is to be put into the doubleArray temporary returned by Abstract_Binary_Operator
  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, min(*Lhs_SerialArray,*Rhs_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     doubleArray & Return_Value = Rhs;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, Lhs_SerialArray, Rhs_SerialArray, min(*Lhs_SerialArray,*Rhs_SerialArray) );
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & min(doubleSerialArray,doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
  // This function can be either Abstract_Binary_Operator or Abstract_Binary_Operator_Non_Commutative
     return doubleSerialArray::Abstract_Binary_Operator ( Lhs , Rhs , MDI_d_Min_Array_And_Array, MDI_d_Min_Array_And_Array_Accumulate_To_Operand , doubleSerialArray::min_Function );
#endif
   }


doubleSerialArray &
min ( const doubleSerialArray & X , const doubleSerialArray & Y , const doubleSerialArray & Z )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of min (doubleSerialArray,doubleSerialArray,doubleSerialArray) for class! X:rc=%d Y:rc=%d Z:rc=%d ",
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
          X.displayReferenceCounts("X in doubleSerialArray & min (doubleSerialArray,doubleSerialArray,doubleSerialArray)");
          Y.displayReferenceCounts("Y in doubleSerialArray & min (doubleSerialArray,doubleSerialArray,doubleSerialArray)");
          Z.displayReferenceCounts("Z in doubleSerialArray & min (doubleSerialArray,doubleSerialArray,doubleSerialArray)");
        }
#endif

     return min ( X , min ( Y , Z ) );
   }


doubleSerialArray &
min ( double x , const doubleSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("\n\n\nInside of min (double,doubleSerialArray) for doubleSerialArray class! \n");

  // This is the only test we can do on the input!
     Rhs.Test_Consistency ("Test Rhs in min");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs.displayReferenceCounts("Rhs in doubleSerialArray & min(double,doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...
     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement
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
          Rhs_SerialArray->displayReferenceCounts("Rhs_SerialArray in doubleSerialArray & min(double,doubleSerialArray)");
        }
#endif

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, Rhs, min(x,*Rhs_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     doubleArray & Return_Value = Rhs;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Rhs, Rhs_SerialArray, min(x,*Rhs_SerialArray) );
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value (before delete macro) in doubleSerialArray & min(double,doubleSerialArray)");
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & min(double,doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
  // This function can be either Abstract_Binary_Operator or Abstract_Binary_Operator_Non_Commutative
     return doubleSerialArray::Abstract_Binary_Operator ( Rhs , x ,
               MDI_d_Min_Scalar_And_Array,
               MDI_d_Min_Scalar_And_Array_Accumulate_To_Operand , doubleSerialArray::Scalar_min_Function );
#endif
   }


doubleSerialArray &
min ( double x , const doubleSerialArray & Y , const doubleSerialArray & Z )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of min (double,doubleSerialArray,doubleSerialArray) for class! Y:rc=%d Z:rc=%d ",
               Y.getRawDataReferenceCount(),Z.getRawDataReferenceCount());
#endif

  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Y.Test_Conformability (Z);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Y.displayReferenceCounts("Y in doubleSerialArray & min (double,doubleSerialArray,doubleSerialArray)");
          Z.displayReferenceCounts("Z in doubleSerialArray & min (double,doubleSerialArray,doubleSerialArray)");
        }
#endif

     return min ( x , min ( Y , Z ) );
   }


doubleSerialArray &
min ( const doubleSerialArray & X , double y , const doubleSerialArray & Z )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of min (doubleSerialArray,double,doubleSerialArray) for class! X:rc=%d Z:rc=%d ",
               X.getRawDataReferenceCount(),Z.getRawDataReferenceCount());
#endif

  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          X.Test_Conformability (Z);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in doubleSerialArray & min (double,doubleSerialArray,doubleSerialArray)");
          Z.displayReferenceCounts("Z in doubleSerialArray & min (double,doubleSerialArray,doubleSerialArray)");
        }
#endif

     return min ( y , min ( X , Z ) );
   }


doubleSerialArray &
min ( const doubleSerialArray & Lhs , double x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of min (doubleSerialArray,double) for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in min");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts 
          Lhs.displayReferenceCounts("Lhs in doubleSerialArray & min(doubleSerialArray,double)");
        }
#endif

#if defined(PPP)
  // Pointers to views of Lhs serial arrays which allow a conformable operation
     doubleSerialArray *Lhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE)
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in doubleSerialArray & min(doubleSerialArray,double)");
        }
#endif

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, min(*Lhs_SerialArray,x) );
#if defined(MEMORY_LEAK_TEST)
     puts ("Can't do MEMORY_LEAK_TEST in doubleSerialArray & min ( const doubleSerialArray & Lhs , double x )");
#endif
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Lhs_SerialArray, min(*Lhs_SerialArray,x) );

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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & min(doubleSerialArray,double)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator ( Lhs , x ,
               MDI_d_Min_Array_And_Scalar,
               MDI_d_Min_Array_And_Scalar_Accumulate_To_Operand , doubleSerialArray::Scalar_min_Function );
#endif
   }


doubleSerialArray &
min ( const doubleSerialArray & X , const doubleSerialArray & Y , double z )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of min (doubleSerialArray,doubleSerialArray,double) for class! X:rc=%d Y:rc=%d ",
               X.getRawDataReferenceCount(),Y.getRawDataReferenceCount());
#endif

  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          X.Test_Conformability (Y);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in doubleSerialArray & min (doubleSerialArray,doubleSerialArray,double)");
          Y.displayReferenceCounts("Y in doubleSerialArray & min (doubleSerialArray,doubleSerialArray,double)");
        }
#endif

     return min ( min ( X , Y ) , z );
   }


double
min ( const doubleSerialArray & X )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of min (doubleSerialArray) returning double for doubleSerialArray class! \n");

  // This is the only test we can do on the input!
     X.Test_Consistency ("Test X in min (const doubleSerialArray & X)");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in double min (doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *X_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
               Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement ( X, X_SerialArray );
             }
            else
             {
               Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement ( X, X_SerialArray );
             }
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
          if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
               Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement 
	          (X, X_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
             }
            else
             {
               Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement 
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
     double Return_Value = 0;
#else
     double Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, X, X_SerialArray, min (*X_SerialArray) , doubleSerialArray::min_Function );
  // return doubleArray::Abstract_Reduction_Operator ( Temporary_Array_Set, X, min (*X_SerialArray) , doubleSerialArray::min_Function );
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
     return doubleSerialArray::Abstract_Unary_Operator ( X , MDI_d_Min_Array_Returning_Scalar , doubleSerialArray::min_Function );
#endif
   }


doubleSerialArray &
max ( const doubleSerialArray & Lhs , const doubleSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of max (doubleSerialArray,doubleSerialArray) for doubleSerialArray class! Lhs:rc=%d Rhs:rc=%d ",
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
          Lhs.displayReferenceCounts("Lhs in doubleSerialArray & max(doubleSerialArray,doubleSerialArray)");
          Rhs.displayReferenceCounts("Rhs in doubleSerialArray & max(doubleSerialArray,doubleSerialArray)");
        }
#endif

#if defined(PPP)
  // Pointers to views of Lhs and Rhs serial arrays which allow a conformable operation
     doubleSerialArray *Lhs_SerialArray = NULL;
     doubleSerialArray *Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...
     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	      (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
               Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement (Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement (Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
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
               Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in doubleSerialArray & max(doubleSerialArray,doubleSerialArray)");
          Rhs_SerialArray->displayReferenceCounts("Rhs_SerialArray in doubleSerialArray & max(doubleSerialArray,doubleSerialArray)");
        }
#endif

  // Inputs to doubleArray::Abstract_Binary_Operator:
  //     1. Temporary_Array_Set is attached to the doubleArray temporary returned by Abstract_Binary_Operator
  //     2. Lhs is used to get the Lhs partition information (PARTI parallel descriptor) and array reuse
  //     3. Rhs is used to get the Rhs partition information (PARTI parallel descriptor) in case the Lhs was 
  //        a NULL array (no data and no defined partitioning (i.e. no PARTI parallel descriptor)) and array reuse
  //     4. The doubleSerialArray which is to be put into the doubleArray temporary returned by Abstract_Binary_Operator
  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, max(*Lhs_SerialArray,*Rhs_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     doubleArray & Return_Value = Rhs;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, Lhs_SerialArray, Rhs_SerialArray, max(*Lhs_SerialArray,*Rhs_SerialArray) );
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & max(doubleSerialArray,doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
  // This function can be either Abstract_Binary_Operator or Abstract_Binary_Operator_Non_Commutative
     return doubleSerialArray::Abstract_Binary_Operator ( Lhs , Rhs , MDI_d_Max_Array_And_Array, MDI_d_Max_Array_And_Array_Accumulate_To_Operand , doubleSerialArray::max_Function );
#endif
   }


doubleSerialArray &
max ( double x , const doubleSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("\n\n\nInside of max (double,doubleSerialArray) for doubleSerialArray class! \n");

  // This is the only test we can do on the input!
     Rhs.Test_Consistency ("Test Rhs in max");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs.displayReferenceCounts("Rhs in doubleSerialArray & max(double,doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...
     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement
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
          Rhs_SerialArray->displayReferenceCounts("Rhs_SerialArray in doubleSerialArray & max(double,doubleSerialArray)");
        }
#endif

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, Rhs, max(x,*Rhs_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     doubleArray & Return_Value = Rhs;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Rhs, Rhs_SerialArray, max(x,*Rhs_SerialArray) );
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value (before delete macro) in doubleSerialArray & max(double,doubleSerialArray)");
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & max(double,doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
  // This function can be either Abstract_Binary_Operator or Abstract_Binary_Operator_Non_Commutative
     return doubleSerialArray::Abstract_Binary_Operator ( Rhs , x ,
               MDI_d_Max_Scalar_And_Array,
               MDI_d_Max_Scalar_And_Array_Accumulate_To_Operand , doubleSerialArray::Scalar_max_Function );
#endif
   }


doubleSerialArray &
max ( double x , const doubleSerialArray & Y , const doubleSerialArray & Z )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of max (double,doubleSerialArray,doubleSerialArray) for doubleSerialArray class!");
#endif

  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Y.Test_Conformability (Z);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Y.displayReferenceCounts("Y in doubleSerialArray & max (double,doubleSerialArray,doubleSerialArray)");
          Z.displayReferenceCounts("Z in doubleSerialArray & max (double,doubleSerialArray,doubleSerialArray)");
        }
#endif

     return max ( x , max ( Y , Z ) );
   }


doubleSerialArray &
max ( const doubleSerialArray & X , double y , const doubleSerialArray & Z )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of max (doubleSerialArray,double,doubleSerialArray) for doubleSerialArray class!");
#endif

  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          X.Test_Conformability (Z);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in doubleSerialArray & max (double,doubleSerialArray,doubleSerialArray)");
          Z.displayReferenceCounts("Z in doubleSerialArray & max (double,doubleSerialArray,doubleSerialArray)");
        }
#endif

     return max ( y , max ( X , Z ) );
   }


doubleSerialArray &
max ( const doubleSerialArray & Lhs , double x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of max (doubleSerialArray,double) for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in max");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts 
          Lhs.displayReferenceCounts("Lhs in doubleSerialArray & max(doubleSerialArray,double)");
        }
#endif

#if defined(PPP)
  // Pointers to views of Lhs serial arrays which allow a conformable operation
     doubleSerialArray *Lhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE)
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in doubleSerialArray & max(doubleSerialArray,double)");
        }
#endif

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, max(*Lhs_SerialArray,x) );
#if defined(MEMORY_LEAK_TEST)
     puts ("Can't do MEMORY_LEAK_TEST in doubleSerialArray & max ( const doubleSerialArray & Lhs , double x )");
#endif
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Lhs_SerialArray, max(*Lhs_SerialArray,x) );

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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & max(doubleSerialArray,double)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator ( Lhs , x ,
               MDI_d_Max_Array_And_Scalar,
               MDI_d_Max_Array_And_Scalar_Accumulate_To_Operand , doubleSerialArray::Scalar_max_Function );
#endif
   }


doubleSerialArray &
max ( const doubleSerialArray & X , const doubleSerialArray & Y , double z )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of max (doubleSerialArray,doubleSerialArray,double) for doubleSerialArray class! \n");
#endif

  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          X.Test_Conformability (Y);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in doubleSerialArray & max (doubleSerialArray,doubleSerialArray,double)");
          Y.displayReferenceCounts("Y in doubleSerialArray & max (doubleSerialArray,doubleSerialArray,double)");
        }
#endif

     return max ( max ( X , Y ) , z );
   }


double
max ( const doubleSerialArray & X )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of max (doubleSerialArray) returning double for doubleSerialArray class! \n");

  // This is the only test we can do on the input!
     X.Test_Consistency ("Test X in max (const doubleSerialArray & X)");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in double max (doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *X_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
               Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement ( X, X_SerialArray );
             }
	    else
	     {
               Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement ( X, X_SerialArray );
             }
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
          if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
               Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement (X, X_SerialArray, 
                    *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
             }
            else
             {
               Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement 
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
     double Return_Value = 0;
#else
     double Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, X, X_SerialArray, max (*X_SerialArray) , doubleSerialArray::max_Function );
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
     return doubleSerialArray::Abstract_Unary_Operator ( X , MDI_d_Max_Array_Returning_Scalar , doubleSerialArray::max_Function );
#endif
   }


doubleSerialArray &
max ( const doubleSerialArray & X , const doubleSerialArray & Y , const doubleSerialArray & Z )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of max (doubleSerialArray,doubleSerialArray,doubleSerialArray) for doubleSerialArray class! \n");
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
          X.displayReferenceCounts("X in doubleSerialArray & max (doubleSerialArray,doubleSerialArray,doubleSerialArray)");
          Y.displayReferenceCounts("Y in doubleSerialArray & max (doubleSerialArray,doubleSerialArray,doubleSerialArray)");
          Z.displayReferenceCounts("Z in doubleSerialArray & max (doubleSerialArray,doubleSerialArray,doubleSerialArray)");
        }
#endif

     return max ( X , max ( Y , Z ) );
   }


double
sum ( const doubleSerialArray & X )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of sum (const doubleSerialArray) returning double for doubleSerialArray class!");

  // This is the only test we can do on the input!
     X.Test_Consistency ("Test X in sum (const doubleSerialArray & X)");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X.displayReferenceCounts("X in double sum (doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *X_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE)
             {
               Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement ( X, X_SerialArray );
             }
            else
             {
               Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement ( X, X_SerialArray );
             }
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
          if (X.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
               Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement 
                    (X, X_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
             }
	    else
             {
               Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement 
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

  // This bug is fixed in the doubleSerialArray::Parallel_Conformability_Enforcement function by restricting the
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
     double Return_Value = 0;
#else
  // Note that we hand the sum operator a view and this means we have
  // to delete the X_SerialArray explicitly (unlike other operators)
     double Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, X, X_SerialArray, sum ((*X_SerialArray)(Index_Pointer_List)) , doubleSerialArray::sum_Function );
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
     return doubleSerialArray::Abstract_Unary_Operator ( X , MDI_d_Sum_Array_Returning_Scalar , doubleSerialArray::sum_Function );
#endif
   }


intSerialArray &
doubleSerialArray::operator! ()
   { 
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator! for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in doubleSerialArray::operatoroperator!");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS in intSerialArray & doubleSerialArray::operator!()");
        }
#endif

#if defined(PPP)
     doubleSerialArray *This_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
	       Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement ( *this, This_SerialArray);
             }
            else
             {
               Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement ( *this, This_SerialArray);
             }
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
          if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
               Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement (
                    *this, This_SerialArray, 
                    *Where_Statement_Support::Where_Statement_Mask,
                    Mask_SerialArray);
             }
            else
             {
               Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement 
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

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, *this, This_SerialArray->operator!() );
#if defined(MEMORY_LEAK_TEST)
     puts ("Can't do MEMORY_LEAK_TEST in intSerialArray & doubleSerialArray::operator! ()");
#endif
     intArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, *this, This_SerialArray, This_SerialArray->operator!() );

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
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & doubleSerialArray::operator!()");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Unary_Operator_Returning_IntArray ( *this ,
                   MDI_d_NOT_Array_Returning_IntArray ,
                   MDI_d_NOT_Array_Accumulate_To_Operand_Returning_IntArray , 
                   doubleSerialArray::Not );
#endif
   }


doubleSerialArray &
operator- ( const doubleSerialArray & Lhs , const doubleSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator- (doubleSerialArray,doubleSerialArray) for doubleSerialArray class!");

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
          Lhs.displayReferenceCounts("Lhs in doubleSerialArray & operator-(doubleSerialArray,doubleSerialArray)");
          Rhs.displayReferenceCounts("Rhs in doubleSerialArray & operator-(doubleSerialArray,doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray* Lhs_SerialArray = NULL;
     doubleSerialArray* Rhs_SerialArray = NULL;

     intSerialArray* Mask_SerialArray = NULL;
     intSerialArray* Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, *Lhs_SerialArray - *Rhs_SerialArray );
#if defined(MEMORY_LEAK_TEST)
     doubleArray & Return_Value = Lhs;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator 
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & operator-(doubleSerialArray,doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator_Non_Commutative ( Lhs , Rhs , 
               MDI_d_Subtract_Array_Minus_Array,
               MDI_d_Subtract_Array_Minus_Array_Accumulate_To_Operand , doubleSerialArray::Minus );
#endif
   }


intSerialArray &
operator< ( const doubleSerialArray & Lhs , const doubleSerialArray & Rhs )
   {   
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of operator< (doubleSerialArray(id=%d),doubleSerialArray(id=%d)) for doubleSerialArray class!",
               Lhs.Array_ID(),Rhs.Array_ID());

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator< (doubleSerialArray,doubleSerialArray)");
     Rhs.Test_Consistency ("Test Rhs in operator< (doubleSerialArray,doubleSerialArray)");
#endif
 
  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Lhs.Test_Conformability (Rhs);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in intSerialArray & operator<(doubleSerialArray,doubleSerialArray)");
          Rhs.displayReferenceCounts("Rhs in intSerialArray & operator<(doubleSerialArray,doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *Lhs_SerialArray = NULL;
     doubleSerialArray *Rhs_SerialArray = NULL;

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
               Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
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
               Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement (Lhs, Lhs_SerialArray, 
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
     Lhs.displayReferenceCounts ("Lhs after PCE in operator< (doubleSerialArray,doubleSerialArray)");
     Rhs.displayReferenceCounts ("Rhs after PCE in operator< (doubleSerialArray,doubleSerialArray)");
     Lhs_SerialArray->displayReferenceCounts ("Lhs_SerialArray after PCE in operator< (doubleSerialArray,doubleSerialArray)");
     Rhs_SerialArray->displayReferenceCounts ("Rhs_SerialArray after PCE in operator< (doubleSerialArray,doubleSerialArray)");
#endif

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();
     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );
     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

#if defined(MEMORY_LEAK_TEST)
     printf ("Can't do MEMORY_LEAK_TEST in intSerialArray & operator< ( const doubleSerialArray & Lhs , const doubleSerialArray & Rhs ) \n");
#endif
     intArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, Lhs_SerialArray, Rhs_SerialArray, *Lhs_SerialArray < *Rhs_SerialArray );

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
          Return_Value.displayReferenceCounts("Return_Value in operator< (doubleSerialArray,doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator_Returning_IntArray ( Lhs , Rhs ,
               MDI_d_LT_Array_LT_Array,
               MDI_d_LT_Array_LT_Array_Accumulate_To_Operand , doubleSerialArray::LT );
#endif
   }   


intSerialArray &
operator< ( const doubleSerialArray & Lhs , double x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator< (doubleSerialArray,double) for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator< (doubleSerialArray,double)");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in intSerialArray & operator<(doubleSerialArray,double)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *Lhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement 
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

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, *Lhs_SerialArray < x );
#if defined(MEMORY_LEAK_TEST)
     puts ("Can't do MEMORY_LEAK_TEST in intSerialArray & operator< ( const doubleSerialArray & Lhs , double x )");
#endif
     intArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Lhs_SerialArray, *Lhs_SerialArray < x );

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
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & operator<(doubleSerialArray,double)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator_Returning_IntArray ( Lhs , x ,
               MDI_d_LT_Array_LT_Scalar,
               MDI_d_LT_Array_LT_Scalar_Accumulate_To_Operand , doubleSerialArray::Scalar_LT );
#endif
   }


intSerialArray &
operator< ( double x , const doubleSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator< (double,doubleSerialArray) for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Rhs.Test_Consistency ("Test Rhs in operator< (double,doubleSerialArray)");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs.displayReferenceCounts("Rhs in intSerialArray & operator<(double,doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *Rhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement 
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
     printf ("Can't do MEMORY_LEAK_TEST in intSerialArray & operator< ( double x , const doubleSerialArray & Rhs ) \n");
#endif
     intArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Rhs, Rhs_SerialArray, x < *Rhs_SerialArray );

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
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & operator<(double,doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator_Returning_IntArray ( Rhs , x ,
               MDI_d_LT_Scalar_LT_Array,
               MDI_d_LT_Scalar_LT_Array_Accumulate_To_Operand , doubleSerialArray::Scalar_LT );
#endif
   }


intSerialArray &
operator> ( const doubleSerialArray & Lhs , const doubleSerialArray & Rhs )
   {   
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of operator> (doubleSerialArray(id=%d),doubleSerialArray(id=%d)) for doubleSerialArray class!",
               Lhs.Array_ID(),Rhs.Array_ID());

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator> (doubleSerialArray,doubleSerialArray)");
     Rhs.Test_Consistency ("Test Rhs in operator> (doubleSerialArray,doubleSerialArray)");
#endif
 
  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Lhs.Test_Conformability (Rhs);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in intSerialArray & operator>(doubleSerialArray,doubleSerialArray)");
          Rhs.displayReferenceCounts("Rhs in intSerialArray & operator>(doubleSerialArray,doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *Lhs_SerialArray = NULL;
     doubleSerialArray *Rhs_SerialArray = NULL;

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
               Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
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
               Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement (Lhs, Lhs_SerialArray, 
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
     Lhs.displayReferenceCounts ("Lhs after PCE in operator> (doubleSerialArray,doubleSerialArray)");
     Rhs.displayReferenceCounts ("Rhs after PCE in operator> (doubleSerialArray,doubleSerialArray)");
     Lhs_SerialArray->displayReferenceCounts ("Lhs_SerialArray after PCE in operator> (doubleSerialArray,doubleSerialArray)");
     Rhs_SerialArray->displayReferenceCounts ("Rhs_SerialArray after PCE in operator> (doubleSerialArray,doubleSerialArray)");
#endif

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();
     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );
     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

#if defined(MEMORY_LEAK_TEST)
     printf ("Can't do MEMORY_LEAK_TEST in intSerialArray & operator> ( const doubleSerialArray & Lhs , const doubleSerialArray & Rhs ) \n");
#endif
     intArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, Lhs_SerialArray, Rhs_SerialArray, *Lhs_SerialArray > *Rhs_SerialArray );

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
          Return_Value.displayReferenceCounts("Return_Value in operator> (doubleSerialArray,doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator_Returning_IntArray ( Lhs , Rhs ,
               MDI_d_GT_Array_GT_Array,
               MDI_d_GT_Array_GT_Array_Accumulate_To_Operand , doubleSerialArray::GT );
#endif
   }   


intSerialArray &
operator> ( const doubleSerialArray & Lhs , double x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator> (doubleSerialArray,double) for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator> (doubleSerialArray,double)");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in intSerialArray & operator>(doubleSerialArray,double)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *Lhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement 
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

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, *Lhs_SerialArray > x );
#if defined(MEMORY_LEAK_TEST)
     puts ("Can't do MEMORY_LEAK_TEST in intSerialArray & operator> ( const doubleSerialArray & Lhs , double x )");
#endif
     intArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Lhs_SerialArray, *Lhs_SerialArray > x );

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
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & operator>(doubleSerialArray,double)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator_Returning_IntArray ( Lhs , x ,
               MDI_d_GT_Array_GT_Scalar,
               MDI_d_GT_Array_GT_Scalar_Accumulate_To_Operand , doubleSerialArray::Scalar_GT );
#endif
   }


intSerialArray &
operator> ( double x , const doubleSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator> (double,doubleSerialArray) for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Rhs.Test_Consistency ("Test Rhs in operator> (double,doubleSerialArray)");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs.displayReferenceCounts("Rhs in intSerialArray & operator>(double,doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *Rhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement 
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
     printf ("Can't do MEMORY_LEAK_TEST in intSerialArray & operator> ( double x , const doubleSerialArray & Rhs ) \n");
#endif
     intArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Rhs, Rhs_SerialArray, x > *Rhs_SerialArray );

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
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & operator>(double,doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator_Returning_IntArray ( Rhs , x ,
               MDI_d_GT_Scalar_GT_Array,
               MDI_d_GT_Scalar_GT_Array_Accumulate_To_Operand , doubleSerialArray::Scalar_GT );
#endif
   }


intSerialArray &
operator<= ( const doubleSerialArray & Lhs , const doubleSerialArray & Rhs )
   {   
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of operator<= (doubleSerialArray(id=%d),doubleSerialArray(id=%d)) for doubleSerialArray class!",
               Lhs.Array_ID(),Rhs.Array_ID());

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator<= (doubleSerialArray,doubleSerialArray)");
     Rhs.Test_Consistency ("Test Rhs in operator<= (doubleSerialArray,doubleSerialArray)");
#endif
 
  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Lhs.Test_Conformability (Rhs);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in intSerialArray & operator<=(doubleSerialArray,doubleSerialArray)");
          Rhs.displayReferenceCounts("Rhs in intSerialArray & operator<=(doubleSerialArray,doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *Lhs_SerialArray = NULL;
     doubleSerialArray *Rhs_SerialArray = NULL;

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
               Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
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
               Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement (Lhs, Lhs_SerialArray, 
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
     Lhs.displayReferenceCounts ("Lhs after PCE in operator<= (doubleSerialArray,doubleSerialArray)");
     Rhs.displayReferenceCounts ("Rhs after PCE in operator<= (doubleSerialArray,doubleSerialArray)");
     Lhs_SerialArray->displayReferenceCounts ("Lhs_SerialArray after PCE in operator<= (doubleSerialArray,doubleSerialArray)");
     Rhs_SerialArray->displayReferenceCounts ("Rhs_SerialArray after PCE in operator<= (doubleSerialArray,doubleSerialArray)");
#endif

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();
     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );
     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

#if defined(MEMORY_LEAK_TEST)
     printf ("Can't do MEMORY_LEAK_TEST in intSerialArray & operator<= ( const doubleSerialArray & Lhs , const doubleSerialArray & Rhs ) \n");
#endif
     intArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, Lhs_SerialArray, Rhs_SerialArray, *Lhs_SerialArray <= *Rhs_SerialArray );

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
          Return_Value.displayReferenceCounts("Return_Value in operator<= (doubleSerialArray,doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator_Returning_IntArray ( Lhs , Rhs ,
               MDI_d_LTEQ_Array_LTEQ_Array,
               MDI_d_LTEQ_Array_LTEQ_Array_Accumulate_To_Operand , doubleSerialArray::LTEQ );
#endif
   }   


intSerialArray &
operator<= ( const doubleSerialArray & Lhs , double x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator<= (doubleSerialArray,double) for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator<= (doubleSerialArray,double)");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in intSerialArray & operator<=(doubleSerialArray,double)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *Lhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement 
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

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, *Lhs_SerialArray <= x );
#if defined(MEMORY_LEAK_TEST)
     puts ("Can't do MEMORY_LEAK_TEST in intSerialArray & operator<= ( const doubleSerialArray & Lhs , double x )");
#endif
     intArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Lhs_SerialArray, *Lhs_SerialArray <= x );

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
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & operator<=(doubleSerialArray,double)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator_Returning_IntArray ( Lhs , x ,
               MDI_d_LTEQ_Array_LTEQ_Scalar,
               MDI_d_LTEQ_Array_LTEQ_Scalar_Accumulate_To_Operand , doubleSerialArray::Scalar_LTEQ );
#endif
   }


intSerialArray &
operator<= ( double x , const doubleSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator<= (double,doubleSerialArray) for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Rhs.Test_Consistency ("Test Rhs in operator<= (double,doubleSerialArray)");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs.displayReferenceCounts("Rhs in intSerialArray & operator<=(double,doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *Rhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement 
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
     printf ("Can't do MEMORY_LEAK_TEST in intSerialArray & operator<= ( double x , const doubleSerialArray & Rhs ) \n");
#endif
     intArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Rhs, Rhs_SerialArray, x <= *Rhs_SerialArray );

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
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & operator<=(double,doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator_Returning_IntArray ( Rhs , x ,
               MDI_d_LTEQ_Scalar_LTEQ_Array,
               MDI_d_LTEQ_Scalar_LTEQ_Array_Accumulate_To_Operand , doubleSerialArray::Scalar_LTEQ );
#endif
   }


intSerialArray &
operator>= ( const doubleSerialArray & Lhs , const doubleSerialArray & Rhs )
   {   
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of operator>= (doubleSerialArray(id=%d),doubleSerialArray(id=%d)) for doubleSerialArray class!",
               Lhs.Array_ID(),Rhs.Array_ID());

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator>= (doubleSerialArray,doubleSerialArray)");
     Rhs.Test_Consistency ("Test Rhs in operator>= (doubleSerialArray,doubleSerialArray)");
#endif
 
  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Lhs.Test_Conformability (Rhs);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in intSerialArray & operator>=(doubleSerialArray,doubleSerialArray)");
          Rhs.displayReferenceCounts("Rhs in intSerialArray & operator>=(doubleSerialArray,doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *Lhs_SerialArray = NULL;
     doubleSerialArray *Rhs_SerialArray = NULL;

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
               Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
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
               Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement (Lhs, Lhs_SerialArray, 
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
     Lhs.displayReferenceCounts ("Lhs after PCE in operator>= (doubleSerialArray,doubleSerialArray)");
     Rhs.displayReferenceCounts ("Rhs after PCE in operator>= (doubleSerialArray,doubleSerialArray)");
     Lhs_SerialArray->displayReferenceCounts ("Lhs_SerialArray after PCE in operator>= (doubleSerialArray,doubleSerialArray)");
     Rhs_SerialArray->displayReferenceCounts ("Rhs_SerialArray after PCE in operator>= (doubleSerialArray,doubleSerialArray)");
#endif

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();
     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );
     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

#if defined(MEMORY_LEAK_TEST)
     printf ("Can't do MEMORY_LEAK_TEST in intSerialArray & operator>= ( const doubleSerialArray & Lhs , const doubleSerialArray & Rhs ) \n");
#endif
     intArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, Lhs_SerialArray, Rhs_SerialArray, *Lhs_SerialArray >= *Rhs_SerialArray );

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
          Return_Value.displayReferenceCounts("Return_Value in operator>= (doubleSerialArray,doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator_Returning_IntArray ( Lhs , Rhs ,
               MDI_d_GTEQ_Array_GTEQ_Array,
               MDI_d_GTEQ_Array_GTEQ_Array_Accumulate_To_Operand , doubleSerialArray::GTEQ );
#endif
   }   


doubleSerialArray &
operator- ( const doubleSerialArray & Lhs , double x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator- (doubleSerialArray,double) for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator-");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in doubleSerialArray & operator-(doubleSerialArray,double)");
        }
#endif

#if defined(PPP)
     doubleSerialArray* Lhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      ( Lhs, Lhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = 
	      doubleArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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
     doubleArray & Return_Value = Lhs;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Lhs_SerialArray, *Lhs_SerialArray - x );
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & operator-(doubleSerialArray,double)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator ( Lhs , x , 
               MDI_d_Subtract_Array_Minus_Scalar,
               MDI_d_Subtract_Array_Minus_Scalar_Accumulate_To_Operand , doubleSerialArray::Scalar_Minus );
#endif
   }


intSerialArray &
operator>= ( const doubleSerialArray & Lhs , double x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator>= (doubleSerialArray,double) for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator>= (doubleSerialArray,double)");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in intSerialArray & operator>=(doubleSerialArray,double)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *Lhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement 
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

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, *Lhs_SerialArray >= x );
#if defined(MEMORY_LEAK_TEST)
     puts ("Can't do MEMORY_LEAK_TEST in intSerialArray & operator>= ( const doubleSerialArray & Lhs , double x )");
#endif
     intArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Lhs_SerialArray, *Lhs_SerialArray >= x );

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
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & operator>=(doubleSerialArray,double)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator_Returning_IntArray ( Lhs , x ,
               MDI_d_GTEQ_Array_GTEQ_Scalar,
               MDI_d_GTEQ_Array_GTEQ_Scalar_Accumulate_To_Operand , doubleSerialArray::Scalar_GTEQ );
#endif
   }


intSerialArray &
operator>= ( double x , const doubleSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator>= (double,doubleSerialArray) for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Rhs.Test_Consistency ("Test Rhs in operator>= (double,doubleSerialArray)");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs.displayReferenceCounts("Rhs in intSerialArray & operator>=(double,doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *Rhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement 
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
     printf ("Can't do MEMORY_LEAK_TEST in intSerialArray & operator>= ( double x , const doubleSerialArray & Rhs ) \n");
#endif
     intArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Rhs, Rhs_SerialArray, x >= *Rhs_SerialArray );

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
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & operator>=(double,doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator_Returning_IntArray ( Rhs , x ,
               MDI_d_GTEQ_Scalar_GTEQ_Array,
               MDI_d_GTEQ_Scalar_GTEQ_Array_Accumulate_To_Operand , doubleSerialArray::Scalar_GTEQ );
#endif
   }


intSerialArray &
operator== ( const doubleSerialArray & Lhs , const doubleSerialArray & Rhs )
   {   
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of operator== (doubleSerialArray(id=%d),doubleSerialArray(id=%d)) for doubleSerialArray class!",
               Lhs.Array_ID(),Rhs.Array_ID());

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator== (doubleSerialArray,doubleSerialArray)");
     Rhs.Test_Consistency ("Test Rhs in operator== (doubleSerialArray,doubleSerialArray)");
#endif
 
  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Lhs.Test_Conformability (Rhs);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in intSerialArray & operator==(doubleSerialArray,doubleSerialArray)");
          Rhs.displayReferenceCounts("Rhs in intSerialArray & operator==(doubleSerialArray,doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *Lhs_SerialArray = NULL;
     doubleSerialArray *Rhs_SerialArray = NULL;

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
               Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
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
               Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement (Lhs, Lhs_SerialArray, 
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
     Lhs.displayReferenceCounts ("Lhs after PCE in operator== (doubleSerialArray,doubleSerialArray)");
     Rhs.displayReferenceCounts ("Rhs after PCE in operator== (doubleSerialArray,doubleSerialArray)");
     Lhs_SerialArray->displayReferenceCounts ("Lhs_SerialArray after PCE in operator== (doubleSerialArray,doubleSerialArray)");
     Rhs_SerialArray->displayReferenceCounts ("Rhs_SerialArray after PCE in operator== (doubleSerialArray,doubleSerialArray)");
#endif

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();
     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );
     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

#if defined(MEMORY_LEAK_TEST)
     printf ("Can't do MEMORY_LEAK_TEST in intSerialArray & operator== ( const doubleSerialArray & Lhs , const doubleSerialArray & Rhs ) \n");
#endif
     intArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, Lhs_SerialArray, Rhs_SerialArray, *Lhs_SerialArray == *Rhs_SerialArray );

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
          Return_Value.displayReferenceCounts("Return_Value in operator== (doubleSerialArray,doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator_Returning_IntArray ( Lhs , Rhs ,
               MDI_d_EQ_Array_EQ_Array,
               MDI_d_EQ_Array_EQ_Array_Accumulate_To_Operand , doubleSerialArray::EQ );
#endif
   }   


intSerialArray &
operator== ( const doubleSerialArray & Lhs , double x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator== (doubleSerialArray,double) for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator== (doubleSerialArray,double)");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in intSerialArray & operator==(doubleSerialArray,double)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *Lhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement 
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

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, *Lhs_SerialArray == x );
#if defined(MEMORY_LEAK_TEST)
     puts ("Can't do MEMORY_LEAK_TEST in intSerialArray & operator== ( const doubleSerialArray & Lhs , double x )");
#endif
     intArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Lhs_SerialArray, *Lhs_SerialArray == x );

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
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & operator==(doubleSerialArray,double)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator_Returning_IntArray ( Lhs , x ,
               MDI_d_EQ_Array_EQ_Scalar,
               MDI_d_EQ_Array_EQ_Scalar_Accumulate_To_Operand , doubleSerialArray::Scalar_EQ );
#endif
   }


intSerialArray &
operator== ( double x , const doubleSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator== (double,doubleSerialArray) for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Rhs.Test_Consistency ("Test Rhs in operator== (double,doubleSerialArray)");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs.displayReferenceCounts("Rhs in intSerialArray & operator==(double,doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *Rhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement 
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
     printf ("Can't do MEMORY_LEAK_TEST in intSerialArray & operator== ( double x , const doubleSerialArray & Rhs ) \n");
#endif
     intArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Rhs, Rhs_SerialArray, x == *Rhs_SerialArray );

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
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & operator==(double,doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator_Returning_IntArray ( Rhs , x ,
               MDI_d_EQ_Scalar_EQ_Array,
               MDI_d_EQ_Scalar_EQ_Array_Accumulate_To_Operand , doubleSerialArray::Scalar_EQ );
#endif
   }


intSerialArray &
operator!= ( const doubleSerialArray & Lhs , const doubleSerialArray & Rhs )
   {   
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of operator!= (doubleSerialArray(id=%d),doubleSerialArray(id=%d)) for doubleSerialArray class!",
               Lhs.Array_ID(),Rhs.Array_ID());

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator!= (doubleSerialArray,doubleSerialArray)");
     Rhs.Test_Consistency ("Test Rhs in operator!= (doubleSerialArray,doubleSerialArray)");
#endif
 
  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Lhs.Test_Conformability (Rhs);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in intSerialArray & operator!=(doubleSerialArray,doubleSerialArray)");
          Rhs.displayReferenceCounts("Rhs in intSerialArray & operator!=(doubleSerialArray,doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *Lhs_SerialArray = NULL;
     doubleSerialArray *Rhs_SerialArray = NULL;

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
               Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
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
               Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement (Lhs, Lhs_SerialArray, 
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
     Lhs.displayReferenceCounts ("Lhs after PCE in operator!= (doubleSerialArray,doubleSerialArray)");
     Rhs.displayReferenceCounts ("Rhs after PCE in operator!= (doubleSerialArray,doubleSerialArray)");
     Lhs_SerialArray->displayReferenceCounts ("Lhs_SerialArray after PCE in operator!= (doubleSerialArray,doubleSerialArray)");
     Rhs_SerialArray->displayReferenceCounts ("Rhs_SerialArray after PCE in operator!= (doubleSerialArray,doubleSerialArray)");
#endif

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();
     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );
     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

#if defined(MEMORY_LEAK_TEST)
     printf ("Can't do MEMORY_LEAK_TEST in intSerialArray & operator!= ( const doubleSerialArray & Lhs , const doubleSerialArray & Rhs ) \n");
#endif
     intArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, Lhs_SerialArray, Rhs_SerialArray, *Lhs_SerialArray != *Rhs_SerialArray );

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
          Return_Value.displayReferenceCounts("Return_Value in operator!= (doubleSerialArray,doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator_Returning_IntArray ( Lhs , Rhs ,
               MDI_d_NOT_EQ_Array_NOT_EQ_Array,
               MDI_d_NOT_EQ_Array_NOT_EQ_Array_Accumulate_To_Operand , doubleSerialArray::NOT_EQ );
#endif
   }   


intSerialArray &
operator!= ( const doubleSerialArray & Lhs , double x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator!= (doubleSerialArray,double) for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator!= (doubleSerialArray,double)");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in intSerialArray & operator!=(doubleSerialArray,double)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *Lhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement 
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

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, *Lhs_SerialArray != x );
#if defined(MEMORY_LEAK_TEST)
     puts ("Can't do MEMORY_LEAK_TEST in intSerialArray & operator!= ( const doubleSerialArray & Lhs , double x )");
#endif
     intArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Lhs_SerialArray, *Lhs_SerialArray != x );

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
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & operator!=(doubleSerialArray,double)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator_Returning_IntArray ( Lhs , x ,
               MDI_d_NOT_EQ_Array_NOT_EQ_Scalar,
               MDI_d_NOT_EQ_Array_NOT_EQ_Scalar_Accumulate_To_Operand , doubleSerialArray::Scalar_NOT_EQ );
#endif
   }


intSerialArray &
operator!= ( double x , const doubleSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator!= (double,doubleSerialArray) for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Rhs.Test_Consistency ("Test Rhs in operator!= (double,doubleSerialArray)");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs.displayReferenceCounts("Rhs in intSerialArray & operator!=(double,doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *Rhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement 
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
     printf ("Can't do MEMORY_LEAK_TEST in intSerialArray & operator!= ( double x , const doubleSerialArray & Rhs ) \n");
#endif
     intArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Rhs, Rhs_SerialArray, x != *Rhs_SerialArray );

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
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & operator!=(double,doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator_Returning_IntArray ( Rhs , x ,
               MDI_d_NOT_EQ_Scalar_NOT_EQ_Array,
               MDI_d_NOT_EQ_Scalar_NOT_EQ_Array_Accumulate_To_Operand , doubleSerialArray::Scalar_NOT_EQ );
#endif
   }


intSerialArray &
operator&& ( const doubleSerialArray & Lhs , const doubleSerialArray & Rhs )
   {   
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of operator&& (doubleSerialArray(id=%d),doubleSerialArray(id=%d)) for doubleSerialArray class!",
               Lhs.Array_ID(),Rhs.Array_ID());

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator&& (doubleSerialArray,doubleSerialArray)");
     Rhs.Test_Consistency ("Test Rhs in operator&& (doubleSerialArray,doubleSerialArray)");
#endif
 
  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Lhs.Test_Conformability (Rhs);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in intSerialArray & operator&&(doubleSerialArray,doubleSerialArray)");
          Rhs.displayReferenceCounts("Rhs in intSerialArray & operator&&(doubleSerialArray,doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *Lhs_SerialArray = NULL;
     doubleSerialArray *Rhs_SerialArray = NULL;

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
               Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
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
               Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement (Lhs, Lhs_SerialArray, 
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
     Lhs.displayReferenceCounts ("Lhs after PCE in operator&& (doubleSerialArray,doubleSerialArray)");
     Rhs.displayReferenceCounts ("Rhs after PCE in operator&& (doubleSerialArray,doubleSerialArray)");
     Lhs_SerialArray->displayReferenceCounts ("Lhs_SerialArray after PCE in operator&& (doubleSerialArray,doubleSerialArray)");
     Rhs_SerialArray->displayReferenceCounts ("Rhs_SerialArray after PCE in operator&& (doubleSerialArray,doubleSerialArray)");
#endif

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();
     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );
     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

#if defined(MEMORY_LEAK_TEST)
     printf ("Can't do MEMORY_LEAK_TEST in intSerialArray & operator&& ( const doubleSerialArray & Lhs , const doubleSerialArray & Rhs ) \n");
#endif
     intArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, Lhs_SerialArray, Rhs_SerialArray, *Lhs_SerialArray && *Rhs_SerialArray );

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
          Return_Value.displayReferenceCounts("Return_Value in operator&& (doubleSerialArray,doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator_Returning_IntArray ( Lhs , Rhs ,
               MDI_d_AND_Array_AND_Array,
               MDI_d_AND_Array_AND_Array_Accumulate_To_Operand , doubleSerialArray::AND );
#endif
   }   


intSerialArray &
operator&& ( const doubleSerialArray & Lhs , double x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator&& (doubleSerialArray,double) for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator&& (doubleSerialArray,double)");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in intSerialArray & operator&&(doubleSerialArray,double)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *Lhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement 
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

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, *Lhs_SerialArray && x );
#if defined(MEMORY_LEAK_TEST)
     puts ("Can't do MEMORY_LEAK_TEST in intSerialArray & operator&& ( const doubleSerialArray & Lhs , double x )");
#endif
     intArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Lhs_SerialArray, *Lhs_SerialArray && x );

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
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & operator&&(doubleSerialArray,double)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator_Returning_IntArray ( Lhs , x ,
               MDI_d_AND_Array_AND_Scalar,
               MDI_d_AND_Array_AND_Scalar_Accumulate_To_Operand , doubleSerialArray::Scalar_AND );
#endif
   }


doubleSerialArray &
operator- ( double x , const doubleSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of operator- (double,doubleSerialArray) for doubleSerialArray class! \n");

  // This is the only test we can do on the input!
     Rhs.Test_Consistency ("Test Rhs in operator-");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs.displayReferenceCounts("Rhs in doubleSerialArray & operator-(double,doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray* Rhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE)
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      ( Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = 
	      doubleArray::Parallel_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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
     doubleArray & Return_Value = Rhs;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Rhs, Rhs_SerialArray, x - *Rhs_SerialArray );
  // return doubleArray::Abstract_Binary_Operator ( Temporary_Array_Set, Rhs, x - *Rhs_SerialArray );
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & operator-(double,doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator ( Rhs , x , 
               MDI_d_Subtract_Scalar_Minus_Array,
               MDI_d_Subtract_Scalar_Minus_Array_Accumulate_To_Operand , doubleSerialArray::Scalar_Minus );
#endif
   }


intSerialArray &
operator&& ( double x , const doubleSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator&& (double,doubleSerialArray) for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Rhs.Test_Consistency ("Test Rhs in operator&& (double,doubleSerialArray)");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs.displayReferenceCounts("Rhs in intSerialArray & operator&&(double,doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *Rhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement 
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
     printf ("Can't do MEMORY_LEAK_TEST in intSerialArray & operator&& ( double x , const doubleSerialArray & Rhs ) \n");
#endif
     intArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Rhs, Rhs_SerialArray, x && *Rhs_SerialArray );

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
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & operator&&(double,doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator_Returning_IntArray ( Rhs , x ,
               MDI_d_AND_Scalar_AND_Array,
               MDI_d_AND_Scalar_AND_Array_Accumulate_To_Operand , doubleSerialArray::Scalar_AND );
#endif
   }


intSerialArray &
operator|| ( const doubleSerialArray & Lhs , const doubleSerialArray & Rhs )
   {   
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of operator|| (doubleSerialArray(id=%d),doubleSerialArray(id=%d)) for doubleSerialArray class!",
               Lhs.Array_ID(),Rhs.Array_ID());

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator|| (doubleSerialArray,doubleSerialArray)");
     Rhs.Test_Consistency ("Test Rhs in operator|| (doubleSerialArray,doubleSerialArray)");
#endif
 
  // Are the arrays the same size (otherwise issue error message and stop).
     if (Index::Index_Bounds_Checking)
          Lhs.Test_Conformability (Rhs);

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in intSerialArray & operator||(doubleSerialArray,doubleSerialArray)");
          Rhs.displayReferenceCounts("Rhs in intSerialArray & operator||(doubleSerialArray,doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *Lhs_SerialArray = NULL;
     doubleSerialArray *Rhs_SerialArray = NULL;

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
               Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
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
               Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement (Lhs, Lhs_SerialArray, 
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
     Lhs.displayReferenceCounts ("Lhs after PCE in operator|| (doubleSerialArray,doubleSerialArray)");
     Rhs.displayReferenceCounts ("Rhs after PCE in operator|| (doubleSerialArray,doubleSerialArray)");
     Lhs_SerialArray->displayReferenceCounts ("Lhs_SerialArray after PCE in operator|| (doubleSerialArray,doubleSerialArray)");
     Rhs_SerialArray->displayReferenceCounts ("Rhs_SerialArray after PCE in operator|| (doubleSerialArray,doubleSerialArray)");
#endif

     bool lhsIsTemporary = Lhs_SerialArray->isTemporary();
     bool rhsIsTemporary = Rhs_SerialArray->isTemporary();

     APP_ASSERT ( (lhsIsTemporary == TRUE) || (lhsIsTemporary == FALSE) );
     APP_ASSERT ( (rhsIsTemporary == TRUE) || (rhsIsTemporary == FALSE) );

#if defined(MEMORY_LEAK_TEST)
     printf ("Can't do MEMORY_LEAK_TEST in intSerialArray & operator|| ( const doubleSerialArray & Lhs , const doubleSerialArray & Rhs ) \n");
#endif
     intArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, Lhs_SerialArray, Rhs_SerialArray, *Lhs_SerialArray || *Rhs_SerialArray );

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
          Return_Value.displayReferenceCounts("Return_Value in operator|| (doubleSerialArray,doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator_Returning_IntArray ( Lhs , Rhs ,
               MDI_d_OR_Array_OR_Array,
               MDI_d_OR_Array_OR_Array_Accumulate_To_Operand , doubleSerialArray::OR );
#endif
   }   


intSerialArray &
operator|| ( const doubleSerialArray & Lhs , double x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator|| (doubleSerialArray,double) for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator|| (doubleSerialArray,double)");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in intSerialArray & operator||(doubleSerialArray,double)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *Lhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement 
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

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, *Lhs_SerialArray || x );
#if defined(MEMORY_LEAK_TEST)
     puts ("Can't do MEMORY_LEAK_TEST in intSerialArray & operator|| ( const doubleSerialArray & Lhs , double x )");
#endif
     intArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Lhs_SerialArray, *Lhs_SerialArray || x );

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
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & operator||(doubleSerialArray,double)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator_Returning_IntArray ( Lhs , x ,
               MDI_d_OR_Array_OR_Scalar,
               MDI_d_OR_Array_OR_Scalar_Accumulate_To_Operand , doubleSerialArray::Scalar_OR );
#endif
   }


intSerialArray &
operator|| ( double x , const doubleSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator|| (double,doubleSerialArray) for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Rhs.Test_Consistency ("Test Rhs in operator|| (double,doubleSerialArray)");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs.displayReferenceCounts("Rhs in intSerialArray & operator||(double,doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *Rhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleSerialArray::Parallel_Indirect_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleSerialArray::Parallel_Conformability_Enforcement 
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
     printf ("Can't do MEMORY_LEAK_TEST in intSerialArray & operator|| ( double x , const doubleSerialArray & Rhs ) \n");
#endif
     intArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Rhs, Rhs_SerialArray, x || *Rhs_SerialArray );

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
          Return_Value.displayReferenceCounts("Return_Value in intSerialArray & operator||(double,doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator_Returning_IntArray ( Rhs , x ,
               MDI_d_OR_Scalar_OR_Array,
               MDI_d_OR_Scalar_OR_Array_Accumulate_To_Operand , doubleSerialArray::Scalar_OR );
#endif
   }


// Sum along axis friend function!
doubleSerialArray &
sum ( const doubleSerialArray & inputArray , int Axis )
{
/*
// ... (Bug Fix, kdb, 7/1/96) Code was previuosly hardwired for 4
//  dimensions, this has been changed to an arbitrary number ...
*/

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          inputArray.displayReferenceCounts("X in doubleSerialArray & sum (doubleSerialArray,int)");
        }
#endif

//  We want to preserve the interface being for cost array objects but we
// need to use a non-const representation to make this work with the current MDI layer
// so we will cast away cost to make this work.
   doubleSerialArray & X = (doubleSerialArray &) inputArray;

#if defined(MEMORY_LEAK_TEST)
   puts ("Can't do MEMORY_LEAK_TEST in doubleSerialArray & sum ( const doubleSerialArray & X , int Axis )");
#endif

//==============================================================
#if COMPILE_DEBUG_STATEMENTS
   if (APP_DEBUG > 0)
      puts ("Inside of doubleSerialArray & sum ( const doubleSerialArray & X , int Axis ) for doubleSerialArray class!");
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
   X.Test_Consistency ("Test X in sum (const doubleSerialArray & X, int Axis)");
#endif
//==============================================================

   // Build result array (it will be marked as a temporary before we 
   // return)
   doubleSerialArray* Result = NULL;

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
       doubleSerialArray *View = new doubleSerialArray 
         ( X.Array_Descriptor.SerialArray->Array_Descriptor.Array_Data , 
	   X.Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain,Index_List);
       Result = new doubleSerialArray 
         (View , X.Array_Descriptor.Array_Domain,Index_List);
#else
      Result = new doubleSerialArray 
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
      Result = new doubleSerialArray (Integer_List);
   }

#if defined(PPP)
  // Skip this for now since it is sort of complex
  // puts ("P++ sum function (sum along an axis) not completely implemented yet (this function is more complex in P++ and will be done last)!");
  // APP_ABORT();
  // Use avoid compiler warning
  // int Avoid_Compiler_Warning = Axis;
  // Use avoid compiler warning
  // return (doubleSerialArray &) X;

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

  // puts ("Call doubleSerialArray sum along axis function!");
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

  // puts ("Returning from doubleArray sum along axis!");

  // Mark as a temporary
     Result->Array_Descriptor.Array_Domain.Is_A_Temporary = TRUE;
     Result->Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain.
	Is_A_Temporary = TRUE;

#if COMPILE_DEBUG_STATEMENTS
  // This is the only test we can do on the input!
     Result->Test_Consistency ("Test Result in sum (const doubleSerialArray & X, int Axis)");
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
          puts ("In sum(doubledArray,int): Where Mask usage not implemented (it is largely meaningless in this case)!");
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
       // efficient if we were to do the initialization in the MDI_d_Sum_Array_Along_Axis function
       // but that would be more complex so I will skip that for now.
          MDI_d_Assign_Array_Equals_Scalar_Accumulate_To_Operand 
	     ( Result->Array_Descriptor.Array_Data , 0 , Mask_Array_Data ,
              (array_domain*) &(Result->Array_Descriptor.Array_Domain) , 
	      Mask_Descriptor );
        }

  // Hand off to the MDI layer for more efficent computation
     MDI_d_Sum_Array_Along_Axis 
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
          if (X.getReferenceCount() < doubleSerialArray::getReferenceCountBase())
               delete &((doubleSerialArray &) X);
        }

  // puts ("Returning from doubleSerialArray sum along axis!");

  // Mark as a temporary
     Result->Array_Descriptor.Array_Domain.Is_A_Temporary = TRUE;

#if COMPILE_DEBUG_STATEMENTS
  // This is the only test we can do on the input!
     Result->Test_Consistency ("Test Result in sum (const doubleSerialArray & X, int Axis)");
#endif

     return *Result;
#endif
   }


#ifndef INTARRAY
doubleSerialArray &
atan2 ( const doubleSerialArray & Lhs , const doubleSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of atan2 (doubleSerialArray,doubleSerialArray) for doubleSerialArray class! Lhs:rc=%d Rhs:rc=%d ",
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
          Lhs.displayReferenceCounts("Lhs in doubleSerialArray & atan2(doubleSerialArray,doubleSerialArray)");
          Rhs.displayReferenceCounts("Rhs in doubleSerialArray & atan2(doubleSerialArray,doubleSerialArray)");
        }
#endif

#if defined(PPP)
  // Pointers to views of Lhs and Rhs serial arrays which allow a conformable operation
     doubleSerialArray *Lhs_SerialArray = NULL;
     doubleSerialArray *Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...
     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	      (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
               Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement (Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement (Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
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
               Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in doubleSerialArray & atan2(doubleSerialArray,doubleSerialArray)");
          Rhs_SerialArray->displayReferenceCounts("Rhs_SerialArray in doubleSerialArray & atan2(doubleSerialArray,doubleSerialArray)");
        }
#endif

  // Inputs to doubleArray::Abstract_Binary_Operator:
  //     1. Temporary_Array_Set is attached to the doubleArray temporary returned by Abstract_Binary_Operator
  //     2. Lhs is used to get the Lhs partition information (PARTI parallel descriptor) and array reuse
  //     3. Rhs is used to get the Rhs partition information (PARTI parallel descriptor) in case the Lhs was 
  //        a NULL array (no data and no defined partitioning (i.e. no PARTI parallel descriptor)) and array reuse
  //     4. The doubleSerialArray which is to be put into the doubleArray temporary returned by Abstract_Binary_Operator
  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, atan2(*Lhs_SerialArray,*Rhs_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     doubleArray & Return_Value = Rhs;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, Lhs_SerialArray, Rhs_SerialArray, atan2(*Lhs_SerialArray,*Rhs_SerialArray) );
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & atan2(doubleSerialArray,doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
  // This function can be either Abstract_Binary_Operator or Abstract_Binary_Operator_Non_Commutative
     return doubleSerialArray::Abstract_Binary_Operator_Non_Commutative ( Lhs , Rhs , MDI_d_Arc_Tan2_Array_ArcTan2_Array, MDI_d_Arc_Tan2_Array_ArcTan2_Array_Accumulate_To_Operand , doubleSerialArray::atan2_Function );
#endif
   }



doubleSerialArray &
atan2 ( double x , const doubleSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("\n\n\nInside of atan2 (double,doubleSerialArray) for doubleSerialArray class! \n");

  // This is the only test we can do on the input!
     Rhs.Test_Consistency ("Test Rhs in atan2");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs.displayReferenceCounts("Rhs in doubleSerialArray & atan2(double,doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...
     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement
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
          Rhs_SerialArray->displayReferenceCounts("Rhs_SerialArray in doubleSerialArray & atan2(double,doubleSerialArray)");
        }
#endif

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, Rhs, atan2(x,*Rhs_SerialArray) );
#if defined(MEMORY_LEAK_TEST)
     doubleArray & Return_Value = Rhs;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Rhs, Rhs_SerialArray, atan2(x,*Rhs_SerialArray) );
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value (before delete macro) in doubleSerialArray & atan2(double,doubleSerialArray)");
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & atan2(double,doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
  // This function can be either Abstract_Binary_Operator or Abstract_Binary_Operator_Non_Commutative
     return doubleSerialArray::Abstract_Binary_Operator_Non_Commutative ( Rhs , x ,
               MDI_d_Arc_Tan2_Scalar_ArcTan2_Array,
               MDI_d_Arc_Tan2_Scalar_ArcTan2_Array_Accumulate_To_Operand , doubleSerialArray::Scalar_atan2_Function );
#endif
   }



doubleSerialArray &
atan2 ( const doubleSerialArray & Lhs , double x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of atan2 (doubleSerialArray,double) for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in atan2");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts 
          Lhs.displayReferenceCounts("Lhs in doubleSerialArray & atan2(doubleSerialArray,double)");
        }
#endif

#if defined(PPP)
  // Pointers to views of Lhs serial arrays which allow a conformable operation
     doubleSerialArray *Lhs_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE)
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (Lhs, Lhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in doubleSerialArray & atan2(doubleSerialArray,double)");
        }
#endif

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, atan2(*Lhs_SerialArray,x) );
#if defined(MEMORY_LEAK_TEST)
     puts ("Can't do MEMORY_LEAK_TEST in doubleSerialArray & atan2 ( const doubleSerialArray & Lhs , double x )");
#endif
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Lhs_SerialArray, atan2(*Lhs_SerialArray,x) );

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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & atan2(doubleSerialArray,double)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator ( Lhs , x ,
               MDI_d_Arc_Tan2_Array_ArcTan2_Scalar,
               MDI_d_Arc_Tan2_Array_ArcTan2_Scalar_Accumulate_To_Operand , doubleSerialArray::Scalar_atan2_Function );
#endif
   }

#endif

#ifdef INTARRAY
doubleSerialArray &
operator& ( const doubleSerialArray & Lhs , const doubleSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
        {
          printf ("\n\n\n### Inside of operator& (doubleSerialArray,doubleSerialArray) for doubleSerialArray class: (id=%d) = (id=%d) \n",
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
          printf ("doubleSerialArray: Lhs.isTemporary() = %s \n",(Lhs.isTemporary()) ? "TRUE" : "FALSE");
          Lhs.displayReferenceCounts("Lhs in doubleSerialArray & operator&(doubleSerialArray,doubleSerialArray)");
          printf ("doubleSerialArray: Rhs.isTemporary() = %s \n",(Rhs.isTemporary()) ? "TRUE" : "FALSE");
          Rhs.displayReferenceCounts("Rhs in doubleSerialArray & operator&(doubleSerialArray,doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray* Lhs_SerialArray = NULL;
     doubleSerialArray* Rhs_SerialArray = NULL;

     intSerialArray* Mask_SerialArray = NULL;
     intSerialArray* Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	      (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
               Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
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
               Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement
                  (Lhs, Lhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray, Rhs, Rhs_SerialArray );
             }
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          printf ("doubleSerialArray: Lhs_SerialArray->isTemporary() = %s \n",(Lhs_SerialArray->isTemporary()) ? "TRUE" : "FALSE");
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in doubleSerialArray & operator&(doubleSerialArray,doubleSerialArray)");
          printf ("doubleSerialArray: Rhs_SerialArray->isTemporary() = %s \n",(Rhs_SerialArray->isTemporary()) ? "TRUE" : "FALSE");
          Rhs_SerialArray->displayReferenceCounts("Rhs_SerialArray in doubleSerialArray & operator&(doubleSerialArray,doubleSerialArray)");
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

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, *Lhs_SerialArray & *Rhs_SerialArray );
#if defined(MEMORY_LEAK_TEST)
     doubleArray & Return_Value = Lhs;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator 
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & operator&(doubleSerialArray,doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator ( Lhs , Rhs , 
               MDI_d_BIT_AND_Array_BitwiseAND_Array,
               MDI_d_BIT_AND_Array_BitwiseAND_Array_Accumulate_To_Operand , doubleSerialArray::BitwiseAND );
#endif
   }

doubleSerialArray &
operator& ( const doubleSerialArray & Lhs , double x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("\n\n\n### Inside of operator& (doubleSerialArray,double) for doubleSerialArray class: (id=%d) = scalar \n",Lhs.Array_ID());

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator&");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in doubleSerialArray & operator&(doubleSerialArray,double)");
        }
#endif

#if defined(PPP)
     doubleSerialArray* Lhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
               Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
               APP_ASSERT(Lhs_SerialArray != NULL);
            // Lhs_SerialArray->displayReferenceCounts("AFTER PCE: *Lhs_SerialArray in doubleSerialArray & operator&(doubleSerialArray,double)");
             }
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
          if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
               Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
                    (Lhs, Lhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
             }
            else
             {
               Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
                    (Lhs, Lhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
             }
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          printf ("doubleSerialArray: Lhs_SerialArray->isTemporary() = %s \n",(Lhs_SerialArray->isTemporary()) ? "TRUE" : "FALSE");
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in doubleSerialArray & operator&(doubleSerialArray,double)");
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
     doubleArray & Return_Value = Lhs;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator(Temporary_Array_Set,Lhs,Lhs_SerialArray,*Lhs_SerialArray & x);
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
          Return_Value.displayReferenceCounts("Return_Value in in doubleSerialArray & operator&(doubleSerialArray,double)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator ( Lhs , x , 
               MDI_d_BIT_AND_Array_BitwiseAND_Scalar,
               MDI_d_BIT_AND_Array_BitwiseAND_Scalar_Accumulate_To_Operand , doubleSerialArray::Scalar_BitwiseAND );
#endif
   }

doubleSerialArray &
operator& ( double x , const doubleSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator& (double,doubleSerialArray) for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Rhs.Test_Consistency ("Test Rhs in operator&");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs.displayReferenceCounts("Rhs in doubleSerialArray & operator&(double,doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray* Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      ( Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = 
	      doubleArray::Parallel_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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
     doubleArray & Return_Value = Rhs;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Rhs, Rhs_SerialArray, x & *Rhs_SerialArray );
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & operator&(double,doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator ( Rhs , x , 
               MDI_d_BIT_AND_Scalar_BitwiseAND_Array,
               MDI_d_BIT_AND_Scalar_BitwiseAND_Array_Accumulate_To_Operand , doubleSerialArray::Scalar_BitwiseAND );
#endif
   }

doubleSerialArray &
doubleSerialArray::operator&= ( double x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of doubleSerialArray::operator&= (double) for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in doubleSerialArray::operator&=");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS in doubleSerialArray & doubleSerialArray::operator&=(double)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *This_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( *this , This_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( *this , This_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (*this, This_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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
     doubleArray::Abstract_Modification_Operator ( Temporary_Array_Set, *this, This_SerialArray, *This_SerialArray &= x );
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
     doubleSerialArray::Abstract_Operator_Operation_Equals ( *this , x ,
        MDI_d_BIT_AND_Array_BitwiseAND_Scalar_Accumulate_To_Operand , doubleSerialArray::Scalar_BitwiseAND_Equals );
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS (return value) in doubleSerialArray & doubleSerialArray::operator&=(double)");
        }
#endif

     return *this;
   }

#endif

#ifdef INTARRAY
doubleSerialArray &
operator| ( const doubleSerialArray & Lhs , const doubleSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
        {
          printf ("\n\n\n### Inside of operator| (doubleSerialArray,doubleSerialArray) for doubleSerialArray class: (id=%d) = (id=%d) \n",
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
          printf ("doubleSerialArray: Lhs.isTemporary() = %s \n",(Lhs.isTemporary()) ? "TRUE" : "FALSE");
          Lhs.displayReferenceCounts("Lhs in doubleSerialArray & operator|(doubleSerialArray,doubleSerialArray)");
          printf ("doubleSerialArray: Rhs.isTemporary() = %s \n",(Rhs.isTemporary()) ? "TRUE" : "FALSE");
          Rhs.displayReferenceCounts("Rhs in doubleSerialArray & operator|(doubleSerialArray,doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray* Lhs_SerialArray = NULL;
     doubleSerialArray* Rhs_SerialArray = NULL;

     intSerialArray* Mask_SerialArray = NULL;
     intSerialArray* Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	      (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
               Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
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
               Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement
                  (Lhs, Lhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray, Rhs, Rhs_SerialArray );
             }
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          printf ("doubleSerialArray: Lhs_SerialArray->isTemporary() = %s \n",(Lhs_SerialArray->isTemporary()) ? "TRUE" : "FALSE");
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in doubleSerialArray & operator|(doubleSerialArray,doubleSerialArray)");
          printf ("doubleSerialArray: Rhs_SerialArray->isTemporary() = %s \n",(Rhs_SerialArray->isTemporary()) ? "TRUE" : "FALSE");
          Rhs_SerialArray->displayReferenceCounts("Rhs_SerialArray in doubleSerialArray & operator|(doubleSerialArray,doubleSerialArray)");
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

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, *Lhs_SerialArray | *Rhs_SerialArray );
#if defined(MEMORY_LEAK_TEST)
     doubleArray & Return_Value = Lhs;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator 
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & operator|(doubleSerialArray,doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator ( Lhs , Rhs , 
               MDI_d_BIT_OR_Array_BitwiseOR_Array,
               MDI_d_BIT_OR_Array_BitwiseOR_Array_Accumulate_To_Operand , doubleSerialArray::BitwiseOR );
#endif
   }

doubleSerialArray &
operator| ( const doubleSerialArray & Lhs , double x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("\n\n\n### Inside of operator| (doubleSerialArray,double) for doubleSerialArray class: (id=%d) = scalar \n",Lhs.Array_ID());

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator|");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in doubleSerialArray & operator|(doubleSerialArray,double)");
        }
#endif

#if defined(PPP)
     doubleSerialArray* Lhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
               Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
               APP_ASSERT(Lhs_SerialArray != NULL);
            // Lhs_SerialArray->displayReferenceCounts("AFTER PCE: *Lhs_SerialArray in doubleSerialArray & operator|(doubleSerialArray,double)");
             }
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
          if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
               Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
                    (Lhs, Lhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
             }
            else
             {
               Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
                    (Lhs, Lhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
             }
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          printf ("doubleSerialArray: Lhs_SerialArray->isTemporary() = %s \n",(Lhs_SerialArray->isTemporary()) ? "TRUE" : "FALSE");
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in doubleSerialArray & operator|(doubleSerialArray,double)");
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
     doubleArray & Return_Value = Lhs;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator(Temporary_Array_Set,Lhs,Lhs_SerialArray,*Lhs_SerialArray | x);
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
          Return_Value.displayReferenceCounts("Return_Value in in doubleSerialArray & operator|(doubleSerialArray,double)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator ( Lhs , x , 
               MDI_d_BIT_OR_Array_BitwiseOR_Scalar,
               MDI_d_BIT_OR_Array_BitwiseOR_Scalar_Accumulate_To_Operand , doubleSerialArray::Scalar_BitwiseOR );
#endif
   }

doubleSerialArray &
operator| ( double x , const doubleSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator| (double,doubleSerialArray) for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Rhs.Test_Consistency ("Test Rhs in operator|");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs.displayReferenceCounts("Rhs in doubleSerialArray & operator|(double,doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray* Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      ( Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = 
	      doubleArray::Parallel_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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
     doubleArray & Return_Value = Rhs;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Rhs, Rhs_SerialArray, x | *Rhs_SerialArray );
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & operator|(double,doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator ( Rhs , x , 
               MDI_d_BIT_OR_Scalar_BitwiseOR_Array,
               MDI_d_BIT_OR_Scalar_BitwiseOR_Array_Accumulate_To_Operand , doubleSerialArray::Scalar_BitwiseOR );
#endif
   }

doubleSerialArray &
doubleSerialArray::operator|= ( double x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of doubleSerialArray::operator|= (double) for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in doubleSerialArray::operator|=");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS in doubleSerialArray & doubleSerialArray::operator|=(double)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *This_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( *this , This_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( *this , This_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (*this, This_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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
     doubleArray::Abstract_Modification_Operator ( Temporary_Array_Set, *this, This_SerialArray, *This_SerialArray |= x );
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
     doubleSerialArray::Abstract_Operator_Operation_Equals ( *this , x ,
        MDI_d_BIT_OR_Array_BitwiseOR_Scalar_Accumulate_To_Operand , doubleSerialArray::Scalar_BitwiseOR_Equals );
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS (return value) in doubleSerialArray & doubleSerialArray::operator|=(double)");
        }
#endif

     return *this;
   }

#endif

#ifdef INTARRAY
doubleSerialArray &
operator^ ( const doubleSerialArray & Lhs , const doubleSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
        {
          printf ("\n\n\n### Inside of operator^ (doubleSerialArray,doubleSerialArray) for doubleSerialArray class: (id=%d) = (id=%d) \n",
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
          printf ("doubleSerialArray: Lhs.isTemporary() = %s \n",(Lhs.isTemporary()) ? "TRUE" : "FALSE");
          Lhs.displayReferenceCounts("Lhs in doubleSerialArray & operator^(doubleSerialArray,doubleSerialArray)");
          printf ("doubleSerialArray: Rhs.isTemporary() = %s \n",(Rhs.isTemporary()) ? "TRUE" : "FALSE");
          Rhs.displayReferenceCounts("Rhs in doubleSerialArray & operator^(doubleSerialArray,doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray* Lhs_SerialArray = NULL;
     doubleSerialArray* Rhs_SerialArray = NULL;

     intSerialArray* Mask_SerialArray = NULL;
     intSerialArray* Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	      (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
               Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
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
               Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement
                  (Lhs, Lhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray, Rhs, Rhs_SerialArray );
             }
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          printf ("doubleSerialArray: Lhs_SerialArray->isTemporary() = %s \n",(Lhs_SerialArray->isTemporary()) ? "TRUE" : "FALSE");
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in doubleSerialArray & operator^(doubleSerialArray,doubleSerialArray)");
          printf ("doubleSerialArray: Rhs_SerialArray->isTemporary() = %s \n",(Rhs_SerialArray->isTemporary()) ? "TRUE" : "FALSE");
          Rhs_SerialArray->displayReferenceCounts("Rhs_SerialArray in doubleSerialArray & operator^(doubleSerialArray,doubleSerialArray)");
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

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, *Lhs_SerialArray ^ *Rhs_SerialArray );
#if defined(MEMORY_LEAK_TEST)
     doubleArray & Return_Value = Lhs;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator 
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & operator^(doubleSerialArray,doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator ( Lhs , Rhs , 
               MDI_d_BIT_XOR_Array_BitwiseXOR_Array,
               MDI_d_BIT_XOR_Array_BitwiseXOR_Array_Accumulate_To_Operand , doubleSerialArray::BitwiseXOR );
#endif
   }

doubleSerialArray &
operator^ ( const doubleSerialArray & Lhs , double x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("\n\n\n### Inside of operator^ (doubleSerialArray,double) for doubleSerialArray class: (id=%d) = scalar \n",Lhs.Array_ID());

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator^");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in doubleSerialArray & operator^(doubleSerialArray,double)");
        }
#endif

#if defined(PPP)
     doubleSerialArray* Lhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
               Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
               APP_ASSERT(Lhs_SerialArray != NULL);
            // Lhs_SerialArray->displayReferenceCounts("AFTER PCE: *Lhs_SerialArray in doubleSerialArray & operator^(doubleSerialArray,double)");
             }
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
          if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
               Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
                    (Lhs, Lhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
             }
            else
             {
               Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
                    (Lhs, Lhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
             }
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          printf ("doubleSerialArray: Lhs_SerialArray->isTemporary() = %s \n",(Lhs_SerialArray->isTemporary()) ? "TRUE" : "FALSE");
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in doubleSerialArray & operator^(doubleSerialArray,double)");
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
     doubleArray & Return_Value = Lhs;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator(Temporary_Array_Set,Lhs,Lhs_SerialArray,*Lhs_SerialArray ^ x);
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
          Return_Value.displayReferenceCounts("Return_Value in in doubleSerialArray & operator^(doubleSerialArray,double)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator ( Lhs , x , 
               MDI_d_BIT_XOR_Array_BitwiseXOR_Scalar,
               MDI_d_BIT_XOR_Array_BitwiseXOR_Scalar_Accumulate_To_Operand , doubleSerialArray::Scalar_BitwiseXOR );
#endif
   }

doubleSerialArray &
operator^ ( double x , const doubleSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator^ (double,doubleSerialArray) for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Rhs.Test_Consistency ("Test Rhs in operator^");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs.displayReferenceCounts("Rhs in doubleSerialArray & operator^(double,doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray* Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      ( Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = 
	      doubleArray::Parallel_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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
     doubleArray & Return_Value = Rhs;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Rhs, Rhs_SerialArray, x ^ *Rhs_SerialArray );
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & operator^(double,doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator ( Rhs , x , 
               MDI_d_BIT_XOR_Scalar_BitwiseXOR_Array,
               MDI_d_BIT_XOR_Scalar_BitwiseXOR_Array_Accumulate_To_Operand , doubleSerialArray::Scalar_BitwiseXOR );
#endif
   }

doubleSerialArray &
doubleSerialArray::operator^= ( double x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of doubleSerialArray::operator^= (double) for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in doubleSerialArray::operator^=");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS in doubleSerialArray & doubleSerialArray::operator^=(double)");
        }
#endif

#if defined(PPP)
     doubleSerialArray *This_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( *this , This_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( *this , This_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (*this, This_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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
     doubleArray::Abstract_Modification_Operator ( Temporary_Array_Set, *this, This_SerialArray, *This_SerialArray ^= x );
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
     doubleSerialArray::Abstract_Operator_Operation_Equals ( *this , x ,
        MDI_d_BIT_XOR_Array_BitwiseXOR_Scalar_Accumulate_To_Operand , doubleSerialArray::Scalar_BitwiseXOR_Equals );
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS (return value) in doubleSerialArray & doubleSerialArray::operator^=(double)");
        }
#endif

     return *this;
   }

#endif

#ifdef INTARRAY
/* There is no <<= operator and so the << must be handled as a special case -- skip it for now */
doubleSerialArray &
operator<< ( const doubleSerialArray & Lhs , const doubleSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
        {
          printf ("\n\n\n### Inside of operator<< (doubleSerialArray,doubleSerialArray) for doubleSerialArray class: (id=%d) = (id=%d) \n",
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
          printf ("doubleSerialArray: Lhs.isTemporary() = %s \n",(Lhs.isTemporary()) ? "TRUE" : "FALSE");
          Lhs.displayReferenceCounts("Lhs in doubleSerialArray & operator<<(doubleSerialArray,doubleSerialArray)");
          printf ("doubleSerialArray: Rhs.isTemporary() = %s \n",(Rhs.isTemporary()) ? "TRUE" : "FALSE");
          Rhs.displayReferenceCounts("Rhs in doubleSerialArray & operator<<(doubleSerialArray,doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray* Lhs_SerialArray = NULL;
     doubleSerialArray* Rhs_SerialArray = NULL;

     intSerialArray* Mask_SerialArray = NULL;
     intSerialArray* Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if ((Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
	      (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE))
             {
               Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray, Rhs, Rhs_SerialArray );
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
               Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement
                  (Lhs, Lhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray, Rhs, Rhs_SerialArray );
             }
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          printf ("doubleSerialArray: Lhs_SerialArray->isTemporary() = %s \n",(Lhs_SerialArray->isTemporary()) ? "TRUE" : "FALSE");
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in doubleSerialArray & operator<<(doubleSerialArray,doubleSerialArray)");
          printf ("doubleSerialArray: Rhs_SerialArray->isTemporary() = %s \n",(Rhs_SerialArray->isTemporary()) ? "TRUE" : "FALSE");
          Rhs_SerialArray->displayReferenceCounts("Rhs_SerialArray in doubleSerialArray & operator<<(doubleSerialArray,doubleSerialArray)");
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

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, Lhs, Rhs, *Lhs_SerialArray << *Rhs_SerialArray );
#if defined(MEMORY_LEAK_TEST)
     doubleArray & Return_Value = Lhs;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator 
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & operator<<(doubleSerialArray,doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator ( Lhs , Rhs , 
               MDI_d_BIT_LSHIFT_Array_BitwiseLShift_Array,
               MDI_d_BIT_LSHIFT_Array_BitwiseLShift_Array_Accumulate_To_Operand , doubleSerialArray::BitwiseLShift );
#endif
   }

doubleSerialArray &
operator<< ( const doubleSerialArray & Lhs , double x )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("\n\n\n### Inside of operator<< (doubleSerialArray,double) for doubleSerialArray class: (id=%d) = scalar \n",Lhs.Array_ID());

  // This is the only test we can do on the input!
     Lhs.Test_Consistency ("Test Lhs in operator<<");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Lhs.displayReferenceCounts("Lhs in doubleSerialArray & operator<<(doubleSerialArray,double)");
        }
#endif

#if defined(PPP)
     doubleSerialArray* Lhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
        {
          if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
               Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
             }
            else
             {
               Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( Lhs, Lhs_SerialArray );
               APP_ASSERT(Lhs_SerialArray != NULL);
            // Lhs_SerialArray->displayReferenceCounts("AFTER PCE: *Lhs_SerialArray in doubleSerialArray & operator<<(doubleSerialArray,double)");
             }
        }
       else
        {
          Old_Mask_SerialArray = Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
          if (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
             {
               Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
                    (Lhs, Lhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
             }
            else
             {
               Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
                    (Lhs, Lhs_SerialArray, *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
             }
          *Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointerLoc() = Mask_SerialArray;
        }

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          printf ("doubleSerialArray: Lhs_SerialArray->isTemporary() = %s \n",(Lhs_SerialArray->isTemporary()) ? "TRUE" : "FALSE");
          Lhs_SerialArray->displayReferenceCounts("Lhs_SerialArray in doubleSerialArray & operator<<(doubleSerialArray,double)");
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
     doubleArray & Return_Value = Lhs;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator(Temporary_Array_Set,Lhs,Lhs_SerialArray,*Lhs_SerialArray << x);
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
          Return_Value.displayReferenceCounts("Return_Value in in doubleSerialArray & operator<<(doubleSerialArray,double)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator ( Lhs , x , 
               MDI_d_BIT_LSHIFT_Array_BitwiseLShift_Scalar,
               MDI_d_BIT_LSHIFT_Array_BitwiseLShift_Scalar_Accumulate_To_Operand , doubleSerialArray::Scalar_BitwiseLShift );
#endif
   }

doubleSerialArray &
operator<< ( double x , const doubleSerialArray & Rhs )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of operator<< (double,doubleSerialArray) for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Rhs.Test_Consistency ("Test Rhs in operator<<");
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Rhs.displayReferenceCounts("Rhs in doubleSerialArray & operator<<(double,doubleSerialArray)");
        }
#endif

#if defined(PPP)
     doubleSerialArray* Rhs_SerialArray = NULL;

  // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray     = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL;
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      ( Rhs, Rhs_SerialArray );
        }
	else
	{
           Temporary_Array_Set = 
	      doubleArray::Parallel_Conformability_Enforcement ( Rhs, Rhs_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (Rhs, Rhs_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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
     doubleArray & Return_Value = Rhs;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, Rhs, Rhs_SerialArray, x << *Rhs_SerialArray );
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & operator<<(double,doubleSerialArray)");
        }
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Binary_Operator ( Rhs , x , 
               MDI_d_BIT_LSHIFT_Scalar_BitwiseLShift_Array,
               MDI_d_BIT_LSHIFT_Scalar_BitwiseLShift_Array_Accumulate_To_Operand , doubleSerialArray::Scalar_BitwiseLShift );
#endif
   }


doubleSerialArray &
doubleSerialArray::operator~ () const
   { 
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          puts ("Inside of unary minus operator operator~ for doubleSerialArray class!");

  // This is the only test we can do on the input!
     Test_Consistency ("Test *this in doubleSerialArray::operatoroperator~");
#endif
 
#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          displayReferenceCounts("THIS in doubleSerialArray & doubleSerialArray::operator~()");
        }
#endif

#if defined(PPP)
     doubleSerialArray *This_SerialArray = NULL;

     // ... bug fix (4/10/97, kdb) must make serial where mask conformable ...

     intSerialArray *Mask_SerialArray = NULL;
     intSerialArray *Old_Mask_SerialArray = NULL;

     Array_Conformability_Info_Type *Temporary_Array_Set = NULL; 
     if (Where_Statement_Support::Where_Statement_Mask == NULL)
     {
        if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
	   Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement ( *this , This_SerialArray );
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement ( *this , This_SerialArray );
	}
     }
     else
     {
	Old_Mask_SerialArray = 
           Where_Statement_Support::Where_Statement_Mask->getSerialArrayPointer();
        if (Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) 
        {
           Temporary_Array_Set = doubleArray::Parallel_Indirect_Conformability_Enforcement 
	      (*this, This_SerialArray, 
               *Where_Statement_Support::Where_Statement_Mask, Mask_SerialArray);
        }
	else
	{
           Temporary_Array_Set = doubleArray::Parallel_Conformability_Enforcement 
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

  // return doubleArray::Abstract_Operator ( Temporary_Array_Set, *this, This_SerialArray->operator~() );
#if defined(MEMORY_LEAK_TEST)
     doubleArray & Return_Value = *this;
#else
     doubleArray & Return_Value = doubleArray::Abstract_Operator ( Temporary_Array_Set, *this, This_SerialArray, This_SerialArray->operator~() );
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          Return_Value.displayReferenceCounts("Return_Value (before delete This_SerialArray) in doubleSerialArray & doubleSerialArray::operator~()");
        }

  // This is the only test we can do on the output!
     Return_Value.Test_Consistency ("Test Return_Value (before delete This_SerialArray) in doubleSerialArray::operatoroperator~");
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
          Return_Value.displayReferenceCounts("Return_Value in doubleSerialArray & doubleSerialArray::operator~()");
        }

  // This is the only test we can do on the output!
     Return_Value.Test_Consistency ("Test Return_Value in doubleSerialArray::operatoroperator~");
#endif

     return Return_Value;
#else
     return doubleSerialArray::Abstract_Unary_Operator ( *this ,
                   MDI_d_BIT_COMPLEMENT_Array ,
                   MDI_d_BIT_COMPLEMENT_Array_Accumulate_To_Operand , doubleSerialArray::Unary_Minus );
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




 

 





 

 





















 









































