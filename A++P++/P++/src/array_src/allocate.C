#define COMPILE_PPP

#include "A++.h"



/* EXPAND THE MACROS HERE! */

#define DOUBLEARRAY
#if defined(PPP)
// ********************************************************************************
// This function localizes details related to parallel allocation of the array data.
// It is called from the doubleArray::Allocate_Array_Data member function.
// In A++ the role of the "Allocate_Array" member function is to just allocate the
// raw array data used in the array object.  In this case the P++ descriptor is
// modified (the PARTI parallel descriptor is built and other modifications done to
// the P++ descriptor).  We might like to have the descriptors fully built before
// allocating the serial array data (and A++ array) -- but this would require that
// the distribution be defined at the time when the P++ descriptor is built
// and we need for the data AND the distribution to be defined AFTER the descriptor
// is built.  This simplified the specification of complex distributions as they
// arise in applications like adaptive mesh refinement.
// ********************************************************************************
void doubleArray_Descriptor_Type::
Allocate_Parallel_Array ( bool Force_Memory_Allocation )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of doubleArray_Descriptor_Type::Allocate_Parallel_Array! \n");
#endif

     int i;  // Local loop INDEX variable

#if defined(USE_PADRE)
  // #############################################################################
  // ######################  CASE OF USING PADRE (START)  ########################
  // #############################################################################

  // ************************************************************************
  //            THIS CODE DEMONSTRATES THE USE OF PADRE in P++
  // ************************************************************************

  // Make sure the array dimensions agree with PADRE.
     if( PADRE_MAX_ARRAY_DIMENSION != MAX_ARRAY_DIMENSION )
        {
          printf ("ERROR: PADRE_MAX_ARRAY_DIMENSION = %d != MAX_ARRAY_DIMENSION = %d \n",
               PADRE_MAX_ARRAY_DIMENSION,MAX_ARRAY_DIMENSION);
        }
     APP_ASSERT( PADRE_MAX_ARRAY_DIMENSION == MAX_ARRAY_DIMENSION );

     int Local_Sizes[MAX_ARRAY_DIMENSION];
     for (i=0; i < MAX_ARRAY_DIMENSION; i++)
          Local_Sizes[i] = 1;

#if COMPILE_DEBUG_STATEMENTS
  // Array_Domain.Test_Distribution_Consistency ("Called from doubleArray::Allocate_Parallel_Array");
#endif

     if (Array_Domain.Is_A_Null_Array)
        {
       // Build pointer to a serial Null array and return from function
          SerialArray = new doubleSerialArray();
          APP_ASSERT (SerialArray != NULL);

          for (i=0; i < MAX_ARRAY_DIMENSION; i++)
             Array_Domain.Local_Mask_Index [i] = Index(0,0,1,Null_Index);

          return;
        }

  // In some cases we get to this point and the parallelPADRE_DescriptorPointer
  // is NULL (so fix that here).
  // This is really the only place where the parallelPADRE_DescriptorPointer
  // should be initialized.
     if (Array_Domain.parallelPADRE_DescriptorPointer == NULL)
        {
          if (Array_Domain.Partitioning_Object_Pointer != NULL)
             {
               APP_ASSERT (Array_Domain.Partitioning_Object_Pointer != NULL);
               APP_ASSERT (Array_Domain.Partitioning_Object_Pointer->distributionPointer != NULL);

               Array_Domain.parallelPADRE_DescriptorPointer = 
                    new PADRE_Descriptor <BaseArray,Array_Domain_Type,SerialArray_Domain_Type>
                      (&(Array_Domain), Array_Domain.Partitioning_Object_Pointer->distributionPointer);
               APP_ASSERT (Array_Domain.parallelPADRE_DescriptorPointer != NULL);
             }
            else
             {
            // ... using DEFAULT PADRE_Distribtuion ...
               Array_Domain.parallelPADRE_DescriptorPointer = 
                    new PADRE_Descriptor <BaseArray,Array_Domain_Type,SerialArray_Domain_Type> (&Array_Domain);
               APP_ASSERT (Array_Domain.parallelPADRE_DescriptorPointer != NULL);
             }
        }

   // ... Some distribution libraries must construct their own data.  If
   // that is the case then adopt the data in the serialArray instead
   // of allocating the memory ...
      int preallocated_data = FALSE;
      double* local_data = NULL;

      Array_Domain.parallelPADRE_DescriptorPointer->allocateData(preallocated_data, &local_data); 

  // Find base and size values to build SerialArray (Local_Sizes includes the ghost boundaries!)
     APP_ASSERT (Array_Domain.parallelPADRE_DescriptorPointer != NULL);
     Array_Domain.parallelPADRE_DescriptorPointer->getLocalSizes(Local_Sizes);

  // Fill in the rest of the dimensions.  This could be handled more efficently.
  // These values were initialized previously but let's make sure that PARTI 
  // has not changed them

     for (i = Array_Domain.Domain_Dimension; i < MAX_ARRAY_DIMENSION; i++)
          APP_ASSERT (Local_Sizes[i] == 1);

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In doubleArray_Descriptor_Type::Allocate_Parallel_Array()");
	  printf ("--- Build SerialArray --\n");
	  printf ("Local_Sizes[0:%d] = ", MAX_ARRAY_DIMENSION);
	  printf(IO_CONTROL_STRING_MACRO_INTEGER, ARRAY_TO_LIST_MACRO(Local_Sizes) );
	  printf("\n");
        }
#endif

     if (SerialArray != NULL)
        {
       // delete any existing serial array object
          APP_ASSERT (SerialArray->getReferenceCount() >= doubleSerialArray::getReferenceCountBase());
          SerialArray->decrementReferenceCount();
          if (SerialArray->getReferenceCount() < doubleSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in allocate.C \n");
               delete SerialArray;
             }
          SerialArray = NULL;
        }

     if (preallocated_data)
        {
       // ... distribution library needed to allocate the data ...
          SerialArray = new doubleSerialArray ( local_data, ARRAY_TO_LIST_MACRO(Local_Sizes) );
        }
       else
        {
          SerialArray = new doubleSerialArray ( ARRAY_TO_LIST_MACRO(Local_Sizes), Force_Memory_Allocation);
        }

  // Need to allocate the data for the array 
     APP_ASSERT (SerialArray != NULL);

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 3)
        {
          printf ("########################################################## \n");
          printf ("########################################################## \n");
          printf ("##   SERIAL ARRAY IS BUILT --- NOW RESET LOCAL DOMAIN   ## \n");
          printf ("########################################################## \n");
          printf ("########################################################## \n");
        }
#endif

  // Initialize PADRE's pointer to the local domain
  // However if this array is a multiple reference (i.e not the orginal data)
  // then we don't want to replace the existing link of the original data's
  // domain to the PADRE_Descriptor.  So only set it if it is unset!
  // This might have to be a stack of these to avoid the array that represented 
  // the initial reference from being deleted which other arrays were using the 
  // PADRE_Descriptor (something that purify would report about).
  // This allows for views to reference the same PADRE_Descriptor (less run-time 
  // overhead).

     if (Array_Domain.parallelPADRE_DescriptorPointer->getLocalDomain() == NULL)
          Array_Domain.parallelPADRE_DescriptorPointer->setLocalDomain
             ( &(SerialArray->Array_Descriptor.Array_Domain) );

  // Now we have to initialize the local descriptor in the SerialArray object
  // We want to use only the input domain not the one necessarily stored in 
  // the PADRE_Descriptor.

     Array_Domain.parallelPADRE_DescriptorPointer->InitializeLocalDescriptor
        ( Array_Domain, SerialArray->Array_Descriptor.Array_Domain );

  // This step is representative of a post processing step which PADRE should call 
  // and which we should isolate into a member function that would be called by 
  // PADRE after the PADRE_Descriptor is setup.  Is this a good idea.  I think 
  // NOT (now that I think about it more)!

  // printf ("In Allocate.C: SerialArray->Array_Descriptor.Array_Domain.View_Offset = %d \n",
  //      SerialArray->Array_Descriptor.Array_Domain.View_Offset);

     DESCRIPTOR_SERIAL_POINTER_LIST_INITIALIZATION_MACRO

  // #############################################################################
  // #######################  CASE OF USING PADRE (END)  #########################
  // #############################################################################

#else
  // #############################################################################
  // ########################  CASE OF NO PADRE (START)  #########################
  // #############################################################################

  // Make sure the array dimensions agree with PARTI.
     APP_ASSERT( MAX_DIM == MAX_ARRAY_DIMENSION );

     APP_ASSERT (Communication_Manager::Number_Of_Processors > 0);
     APP_ASSERT (Communication_Manager::Number_Of_Processors <= MAX_PROCESSORS);

  // Array_Domain.display("Array_Domain AT TOP before allocation of serial array!");

  // For better efficency we could treat the simple single processor case special
  // but for now we will skip special processing so we can debug the 
  // multiprocessor case.

  // Make sure that Virtual Processor Spaces is defined
     if (Communication_Manager::VirtualProcessorSpace == NULL)
        {
#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 0)
               printf ("Build virtual processor space! \n");
#endif
       // Block Parti only implemented with 1D virtual processor spaces
          int Sizes[1];
          Sizes[0] = Communication_Manager::Number_Of_Processors;
          APP_ASSERT (Communication_Manager::Number_Of_Processors > 0);
          Communication_Manager::VirtualProcessorSpace = vProc(1,Sizes);
        }
       else
        {
#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 0)
               printf ("Virtual Processor Space ALREADY BUILT! \n");
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
        {
          printf ("(Virtual processor Space size) - Communication_Manager");
          printf ("::Number_Of_Processors = %d \n",Communication_Manager::Number_Of_Processors);
        }
#endif

     APP_ASSERT(Communication_Manager::VirtualProcessorSpace != NULL);

  // Change the way we compute the array size since we want to be able to build the 
  // data storage that will represent a view.
  // APP_ASSERT (Array_Domain.Is_A_View == FALSE);

  // We skip the construction of a Parti descriptor for the case of a Null 
  // array because it is not required and because Parti could not build a 
  // descriptor for an array of ZERO size.  But P++ must provide a valid pointer 
  // to a valid serial Null array.

     if (Array_Domain.Is_A_Null_Array)
        {
       // Build pointer to a serial Null array and return from function
          SerialArray = new doubleSerialArray();
          APP_ASSERT (SerialArray != NULL);

       // I think this is not required since it should have already been
       // set before calling this function
          for (i=0; i < MAX_ARRAY_DIMENSION; i++)
                Array_Domain.Local_Mask_Index [i] = Index(0,0,1,Null_Index);
          return;
        }

     int Array_Sizes[MAX_ARRAY_DIMENSION];
     Array_Domain.getRawDataSize (Array_Sizes);

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("\n");
          printf ("SIZES OF UNPARTITONED DATA (GLOBAL SIZES) \n");
          printf ("Building a partition with Array_Sizes[0]= %d\n",Array_Sizes[0]);
	  for (i=1; i < MAX_ARRAY_DIMENSION; i++)
             {
	       printf ("                          Array_Sizes[%d]= %d\n",i,Array_Sizes[i]);
	     }
          printf ("\n");
        }
#endif

  // Now to the alignment of array onto decomposition (finally building the parallel
  // array descriptor).  This niether allocates nor distributes any array data.
  // Special constructors will allow the user to specify specific data to exercise
  // greater control over the distribution of P++ arrays.  But for now we DEFINE
  // some defaults to allow a distribution onto the multiprocessor space.

     int Number_Of_Dimensions_To_Partition = Array_Domain.Domain_Dimension;
  // Bugfix (5/23/95) must handle case of combined Index and scalar indexing 
  // which would build a lower dimensional array
     APP_ASSERT (Number_Of_Dimensions_To_Partition >= Array_Domain_Type::computeArrayDimension ( Array_Domain ));

  // Build the Block Parti parallel descriptor
  // Get data from Partitioning object or use default if not available
     if ( Array_Domain.Partitioning_Object_Pointer != NULL )
        {
          APP_ASSERT( Array_Domain.Partitioning_Object_Pointer != NULL );

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("************************************** \n");
               printf ("Using an existing Partitioning_Object! \n");
               printf ("************************************** \n");
               Array_Domain.Partitioning_Object_Pointer->display("Using an existing Partitioning_Object!");
             }
#endif
       // In the case where the ghost boundaies are incremented the
       // BlockPartiArrayDecomposition is reused and so this is not a NULL pointer.
          if (Array_Domain.BlockPartiArrayDecomposition == NULL)
             {
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 0)
                    printf ("In Allocate_Parallel_Array -- BUILD A NEW Array_Domain.BlockPartiArrayDecomposition! \n");
#endif
               Array_Domain.BlockPartiArrayDecomposition = 
                  Array_Domain.Partitioning_Object_Pointer->Build_BlockPartiDecompostion ( Array_Sizes );

            // Bugfix (6/10/2000): delete any existing domain if we just built a new Decomposition
               if (Array_Domain.BlockPartiArrayDomain != NULL)
                  {
                 // Delete the Array_Domain.BlockPartiArrayDomain so that a new one will be built
                    delete_DARRAY ( Array_Domain.BlockPartiArrayDomain );
                    Array_Domain.BlockPartiArrayDomain = NULL;
                  }
             }
            else
             {
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("In Allocate_Parallel_Array -- Array_Domain.BlockPartiArrayDecomposition already exists! \n");
#endif
             }
          APP_ASSERT (Array_Domain.BlockPartiArrayDecomposition != NULL);

       // This really should be built again since it reflects the new ghost 
       // boundary widths.  At least in the case where this function is called 
       // after setInternalGhostCellWidth (we force the new one to be built only
       // if a new BlockPartiArrayDecomposition is built).

       // Bugfix (6/10/2000): sometimes this is a valid pointer and we want to just reuse it
       // APP_ASSERT (Array_Domain.BlockPartiArrayDomain == NULL);
          if (Array_Domain.BlockPartiArrayDomain == NULL)
             {
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("In Allocate_Parallel_Array -- BUILD A NEW Array_Domain.BlockPartiArrayDomain! \n");
#endif
               Array_Domain.BlockPartiArrayDomain = 
                  Array_Domain.Partitioning_Object_Pointer->
	             Build_BlockPartiArrayDomain 
	                (Array_Domain.BlockPartiArrayDecomposition , 
		         Array_Sizes, Array_Domain.InternalGhostCellWidth, 
		         Array_Domain.ExternalGhostCellWidth );
               APP_ASSERT(Array_Domain.BlockPartiArrayDomain != NULL);
             }
            else
             {
            // We could provide asserts that make sure it is the right one when we reuse it!
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("In Allocate_Parallel_Array -- Array_Domain.BlockPartiArrayDomain already exists! \n");
#endif
             }
          APP_ASSERT (Array_Domain.BlockPartiArrayDomain != NULL);
        }
       else
        {
          APP_ASSERT( Array_Domain.Partitioning_Object_Pointer == NULL );

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("*********************************************** \n");
               printf ("NO existing Partitioning_Object (use defaults)! \n");
               printf ("*********************************************** \n");
               Partitioning_Type::displayDefaultValues("NO existing Partitioning_Object (use defaults)!");
             }
#endif
          if (Array_Domain.Is_A_Null_Array == FALSE)
             {
            // In cases where the assignment is to a Null array the operator= 
	    // the Array descriptor is copied from the RHS the 
	    // BlockPartiArrayDecomposition point is copied as well so that 
	    // we have a valid pointer at this point.
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("In Allocate_Parallel_Array -- Array_Domain.Is_A_Null_Array == FALSE \n");
#endif
               if (Array_Domain.BlockPartiArrayDecomposition == NULL)
                  {
#if COMPILE_DEBUG_STATEMENTS
                    if (APP_DEBUG > 1)
                         printf ("In Allocate_Parallel_Array -- BUILD A NEW Array_Domai.Array_Domainn->BlockPartiArrayDecomposition! \n");
#endif
                    Array_Domain.BlockPartiArrayDecomposition = 
                         Partitioning_Type::Build_DefaultBlockPartiDecompostion ( Array_Sizes );
                    APP_ASSERT(Array_Domain.BlockPartiArrayDecomposition->referenceCount == 0);
                  }
                 else
                  {
#if COMPILE_DEBUG_STATEMENTS
                    if (APP_DEBUG > 1)
                         printf ("In Allocate_Parallel_Array -- Array_Domain.BlockPartiArrayDecomposition already exists! \n");
#endif
                  }
               APP_ASSERT (Array_Domain.BlockPartiArrayDecomposition != NULL);
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Inside of Parallel_Allocate:  (phase 1) Array_Domain.BlockPartiArrayDecomposition->referenceCount = %d \n",
                         Array_Domain.BlockPartiArrayDecomposition->referenceCount);
#endif
            // APP_ASSERT (Array_Domain.BlockPartiArrayDomain == NULL);
               if (Array_Domain.BlockPartiArrayDomain == NULL)
                  {
#if COMPILE_DEBUG_STATEMENTS
                    if (APP_DEBUG > 1)
                         printf ("In Allocate_Parallel_Array -- BUILD A NEW Array_Domain.BlockPartiArrayDomain! \n");
#endif
                    Array_Domain.BlockPartiArrayDomain =
                         Partitioning_Type::Build_DefaultBlockPartiArrayDomain (
                                   Array_Domain.BlockPartiArrayDecomposition , Array_Sizes ,
                                   Array_Domain.InternalGhostCellWidth , Array_Domain.ExternalGhostCellWidth );
                    APP_ASSERT(Array_Domain.BlockPartiArrayDomain->referenceCount == 0);
                  }
                 else
                  {
#if COMPILE_DEBUG_STATEMENTS
                    if (APP_DEBUG > 1)
                         printf ("In Allocate_Parallel_Array -- Array_Domain.BlockPartiArrayDomain already exists! \n");
#endif
                  }
               APP_ASSERT(Array_Domain.BlockPartiArrayDomain != NULL);
             }
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          APP_ASSERT(Array_Domain.BlockPartiArrayDomain        != NULL);
          APP_ASSERT(Array_Domain.BlockPartiArrayDecomposition != NULL);
          printf ("Inside of Parallel_Allocate (MIDDLE): Array_Domain.BlockPartiArrayDomain->referenceCount = %d \n",
               Array_Domain.BlockPartiArrayDomain->referenceCount);
          printf ("Inside of Parallel_Allocate (MIDDLE): Array_Domain.BlockPartiArrayDecomposition->referenceCount = %d \n",
               Array_Domain.BlockPartiArrayDecomposition->referenceCount);

        }
#endif
  // ****************************************************************************
  // ****************************************************************************
  // ******  Location of demarkation for the part of the function that ******
  // ******  can go into the Array_Descriptor_Type object (upper part)     ******
  // ******  and the part of the function that can go into the             ******
  // ******  Array_Domain_Type object.  (this is future work)              ******
  // ****************************************************************************
  // ****************************************************************************

  // Bugfix (12/18/94) to allow reuse of a P++ array object (required when a 
  // NULL P++ array object is initialized in the operator=() member function)

     if (SerialArray != NULL)
        {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Deleting an existing SerialArray! \n");
#endif
          APP_ASSERT(SerialArray->Array_Descriptor.Array_Domain.Is_A_Null_Array == TRUE);
          SerialArray->decrementReferenceCount();
          if (SerialArray->getReferenceCount() < SerialArray->getReferenceCountBase())
               delete SerialArray;
          SerialArray = NULL;
        }

     APP_ASSERT(SerialArray == NULL);

  // Get the size of the local partition for this processor and build a doubleSerialArray of that size.
     const int UNIT_STRIDE = 1;

     int Local_Sizes[MAX_ARRAY_DIMENSION];

  // initialize to unit values (to start)
     for ( i=0; i<MAX_ARRAY_DIMENSION; i++ )
        {
          Local_Sizes[i] = 1;
        }

#if COMPILE_DEBUG_STATEMENTS
     Array_Domain.Test_Distribution_Consistency("Called from doubleArray::Allocate_Parallel_Array");
  // printf ("Skipping call to Test_Distribution_Consistency in allocate.C \n");
#endif

  // Build base and size values to build SerialArray (laSizes includes the ghost boundaries!)
     APP_ASSERT (Array_Domain.BlockPartiArrayDomain != NULL);
     laSizes(Array_Domain.BlockPartiArrayDomain,Local_Sizes);

  // Fill in the rest of the dimensions.  This could be handled more efficently.
#if COMPILE_DEBUG_STATEMENTS
  // These values were initialized previously but let's make sure that PARTI has not changed them
     for (i = Array_Domain.Domain_Dimension; i < MAX_ARRAY_DIMENSION; i++)
          APP_ASSERT (Local_Sizes[i] == 1); 

     if (Array_Domain.Partitioning_Object_Pointer == NULL)
        {
       // Partitioning_Type::Test_Consistency ( Array_Domain.BlockPartiArrayDomain , 
       //      "Called from doubleArray::Allocate_Parallel_Array" );
          Internal_Partitioning_Type::staticTestConsistency ( Array_Domain.BlockPartiArrayDomain ,
               "Called from doubleArray::Allocate_Parallel_Array" );
        }
       else
        {
          Array_Domain.Partitioning_Object_Pointer->Test_Consistency ( 
               Array_Domain.BlockPartiArrayDomain , "Called from doubleArray::Allocate_Parallel_Array" );
        }

     if (APP_DEBUG > 1)
        {
          printf ("Build SerialArray -- ");
          for (i=0; i < MAX_ARRAY_DIMENSION; i++)
             {
	       printf("Local_Sizes[%d] = %d ",i,Local_Sizes[i]);
             }
          printf("\n");
        }
#endif

  // Array_Domain.display("Array_Domain before allocation of serial array!");

  // The process is:
  //    1. allocate the serial array with a size specified from the block PARTI function
  //    2. modify the array_domain objects
  //          a. Mostly the serial array's array_domain
  //          b. Also somewhat the parallel array objects array_domain as well (but very little)

  // Build the local processor's array
  // We can't build this as a strided object because it could get truncated at the base and bounds
  // and we have to have the serial array be exactly the specified size to use the distribution
  // mechanisms.
     SerialArray = new doubleSerialArray( ARRAY_TO_LIST_MACRO(Local_Sizes), Force_Memory_Allocation);

  // error checking
     APP_ASSERT(SerialArray != NULL);

     int Bases[MAX_ARRAY_DIMENSION];

  // initialize the work array
     for (i=0; i < MAX_ARRAY_DIMENSION; i++)
        {
          Bases[i] = 0;
        }

     bool Generated_A_Null_Array              = FALSE;
     int SerialArray_Cannot_Have_Contiguous_Data = FALSE;

  // Need to set the upper bound on the iteration to MAX_ARRAY_DIMENSION
  // since all the variables for each dimension must be initialized.
     int j;
     for (j=0; j < MAX_ARRAY_DIMENSION; j++)
        {
          if (j < Array_Domain.Domain_Dimension)
             {
               Bases[j] = gLBnd(Array_Domain.BlockPartiArrayDomain,j) - Array_Domain.InternalGhostCellWidth[j];
             }
            else
             {
            // ... (10/24/96) if ghost cell width is subtracted then local
            // base and bound will be out of range of global base and bound.
            // The local base and bound can't be increased to account for
            // this because array will be the wrong size. ...
            // Bases[j] = APP_Global_Array_Base - Array_Domain.InternalGhostCellWidth[j];

            // (6/28/2000) I think this should be set to ZERO instead of APP_Global_Array_Base!
               Bases[j] = APP_Global_Array_Base;
             }

       // Set the base to coorespond to the global address space -- but also 
       // set the Data_Base so it can be consistant with the P++ descriptor
       // This could be implemented without the function call overhead 
       // represented here but this simplifies the initial implementation for now.

#if 0
          printf ("(before setBase): getLocalBase(%d) = %d  getBase(%d) = %d  getLocalBound(%d) = %d getBound(%d) = %d \n",
               j,getLocalBase(j),j,getBase(j),j,getLocalBound(j),j,getBound(j));
	  printf ("(before setBase): SerialArray->getRawBase(%d) = %d  getRawBase(%d) = %d  SerialArray->getRawBound(%d) = %d getRawBound(%d) = %d \n",
		  j,SerialArray->getRawBase(j),j,getRawBase(j),j,SerialArray->getRawBound(j),j,getRawBound(j));
#endif

       // ... (10/28/96, kdb) need to add Data_Base otherwise code doesn't
       // work if Array_Domain has nonzero databases ...
       // DQ (7/2/2000): Should this be User_Base instead of Data_Base?
          SerialArray->setBase ( Bases[j] + Array_Domain.Data_Base[j], j );
        }

  // error checking
     APP_ASSERT(SerialArray != NULL);

  // details specific to the setup of the descriptor and separate from PADRE are isolated into
  // a separate function so that PADRE and non-PADRE versions of P++ can use a single version!
     Array_Domain.postAllocationSupport(SerialArray->Array_Descriptor.Array_Domain);

  // Now initialize the pointers (the one thing we could not place into the postAllocationSupport() member function)
     DESCRIPTOR_SERIAL_POINTER_LIST_INITIALIZATION_MACRO;

  // #############################################################################
  // #########################  CASE OF NO PADRE (END)  ##########################
  // #############################################################################
#endif

  // Temp code (and intermediate result)
  // SerialArray->Array_Descriptor.Array_Domain.display("In allocate.C: SerialArray->Array_Descriptor.Array_Domain");

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("At base of doubleArray_Descriptor_Type::Allocate_Parallel_Array \n");

     Test_Consistency ("Called from base of doubleArray_Descriptor_Type::Allocate_Parallel_Array");
#endif

  // printf ("Exiting at BASE of doubleArray_Descriptor_Type::Allocate_Parallel_Array() \n");
  // APP_ABORT();
   }
#else
#error "(allocate.C) This is only code for P++"
#endif

#undef DOUBLEARRAY

#define FLOATARRAY
#if defined(PPP)
// ********************************************************************************
// This function localizes details related to parallel allocation of the array data.
// It is called from the floatArray::Allocate_Array_Data member function.
// In A++ the role of the "Allocate_Array" member function is to just allocate the
// raw array data used in the array object.  In this case the P++ descriptor is
// modified (the PARTI parallel descriptor is built and other modifications done to
// the P++ descriptor).  We might like to have the descriptors fully built before
// allocating the serial array data (and A++ array) -- but this would require that
// the distribution be defined at the time when the P++ descriptor is built
// and we need for the data AND the distribution to be defined AFTER the descriptor
// is built.  This simplified the specification of complex distributions as they
// arise in applications like adaptive mesh refinement.
// ********************************************************************************
void floatArray_Descriptor_Type::
Allocate_Parallel_Array ( bool Force_Memory_Allocation )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of floatArray_Descriptor_Type::Allocate_Parallel_Array! \n");
#endif

     int i;  // Local loop INDEX variable

#if defined(USE_PADRE)
  // #############################################################################
  // ######################  CASE OF USING PADRE (START)  ########################
  // #############################################################################

  // ************************************************************************
  //            THIS CODE DEMONSTRATES THE USE OF PADRE in P++
  // ************************************************************************

  // Make sure the array dimensions agree with PADRE.
     if( PADRE_MAX_ARRAY_DIMENSION != MAX_ARRAY_DIMENSION )
        {
          printf ("ERROR: PADRE_MAX_ARRAY_DIMENSION = %d != MAX_ARRAY_DIMENSION = %d \n",
               PADRE_MAX_ARRAY_DIMENSION,MAX_ARRAY_DIMENSION);
        }
     APP_ASSERT( PADRE_MAX_ARRAY_DIMENSION == MAX_ARRAY_DIMENSION );

     int Local_Sizes[MAX_ARRAY_DIMENSION];
     for (i=0; i < MAX_ARRAY_DIMENSION; i++)
          Local_Sizes[i] = 1;

#if COMPILE_DEBUG_STATEMENTS
  // Array_Domain.Test_Distribution_Consistency ("Called from floatArray::Allocate_Parallel_Array");
#endif

     if (Array_Domain.Is_A_Null_Array)
        {
       // Build pointer to a serial Null array and return from function
          SerialArray = new floatSerialArray();
          APP_ASSERT (SerialArray != NULL);

          for (i=0; i < MAX_ARRAY_DIMENSION; i++)
             Array_Domain.Local_Mask_Index [i] = Index(0,0,1,Null_Index);

          return;
        }

  // In some cases we get to this point and the parallelPADRE_DescriptorPointer
  // is NULL (so fix that here).
  // This is really the only place where the parallelPADRE_DescriptorPointer
  // should be initialized.
     if (Array_Domain.parallelPADRE_DescriptorPointer == NULL)
        {
          if (Array_Domain.Partitioning_Object_Pointer != NULL)
             {
               APP_ASSERT (Array_Domain.Partitioning_Object_Pointer != NULL);
               APP_ASSERT (Array_Domain.Partitioning_Object_Pointer->distributionPointer != NULL);

               Array_Domain.parallelPADRE_DescriptorPointer = 
                    new PADRE_Descriptor <BaseArray,Array_Domain_Type,SerialArray_Domain_Type>
                      (&(Array_Domain), Array_Domain.Partitioning_Object_Pointer->distributionPointer);
               APP_ASSERT (Array_Domain.parallelPADRE_DescriptorPointer != NULL);
             }
            else
             {
            // ... using DEFAULT PADRE_Distribtuion ...
               Array_Domain.parallelPADRE_DescriptorPointer = 
                    new PADRE_Descriptor <BaseArray,Array_Domain_Type,SerialArray_Domain_Type> (&Array_Domain);
               APP_ASSERT (Array_Domain.parallelPADRE_DescriptorPointer != NULL);
             }
        }

   // ... Some distribution libraries must construct their own data.  If
   // that is the case then adopt the data in the serialArray instead
   // of allocating the memory ...
      int preallocated_data = FALSE;
      float* local_data = NULL;

      Array_Domain.parallelPADRE_DescriptorPointer->allocateData(preallocated_data, &local_data); 

  // Find base and size values to build SerialArray (Local_Sizes includes the ghost boundaries!)
     APP_ASSERT (Array_Domain.parallelPADRE_DescriptorPointer != NULL);
     Array_Domain.parallelPADRE_DescriptorPointer->getLocalSizes(Local_Sizes);

  // Fill in the rest of the dimensions.  This could be handled more efficently.
  // These values were initialized previously but let's make sure that PARTI 
  // has not changed them

     for (i = Array_Domain.Domain_Dimension; i < MAX_ARRAY_DIMENSION; i++)
          APP_ASSERT (Local_Sizes[i] == 1);

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In floatArray_Descriptor_Type::Allocate_Parallel_Array()");
	  printf ("--- Build SerialArray --\n");
	  printf ("Local_Sizes[0:%d] = ", MAX_ARRAY_DIMENSION);
	  printf(IO_CONTROL_STRING_MACRO_INTEGER, ARRAY_TO_LIST_MACRO(Local_Sizes) );
	  printf("\n");
        }
#endif

     if (SerialArray != NULL)
        {
       // delete any existing serial array object
          APP_ASSERT (SerialArray->getReferenceCount() >= floatSerialArray::getReferenceCountBase());
          SerialArray->decrementReferenceCount();
          if (SerialArray->getReferenceCount() < floatSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in allocate.C \n");
               delete SerialArray;
             }
          SerialArray = NULL;
        }

     if (preallocated_data)
        {
       // ... distribution library needed to allocate the data ...
          SerialArray = new floatSerialArray ( local_data, ARRAY_TO_LIST_MACRO(Local_Sizes) );
        }
       else
        {
          SerialArray = new floatSerialArray ( ARRAY_TO_LIST_MACRO(Local_Sizes), Force_Memory_Allocation);
        }

  // Need to allocate the data for the array 
     APP_ASSERT (SerialArray != NULL);

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 3)
        {
          printf ("########################################################## \n");
          printf ("########################################################## \n");
          printf ("##   SERIAL ARRAY IS BUILT --- NOW RESET LOCAL DOMAIN   ## \n");
          printf ("########################################################## \n");
          printf ("########################################################## \n");
        }
#endif

  // Initialize PADRE's pointer to the local domain
  // However if this array is a multiple reference (i.e not the orginal data)
  // then we don't want to replace the existing link of the original data's
  // domain to the PADRE_Descriptor.  So only set it if it is unset!
  // This might have to be a stack of these to avoid the array that represented 
  // the initial reference from being deleted which other arrays were using the 
  // PADRE_Descriptor (something that purify would report about).
  // This allows for views to reference the same PADRE_Descriptor (less run-time 
  // overhead).

     if (Array_Domain.parallelPADRE_DescriptorPointer->getLocalDomain() == NULL)
          Array_Domain.parallelPADRE_DescriptorPointer->setLocalDomain
             ( &(SerialArray->Array_Descriptor.Array_Domain) );

  // Now we have to initialize the local descriptor in the SerialArray object
  // We want to use only the input domain not the one necessarily stored in 
  // the PADRE_Descriptor.

     Array_Domain.parallelPADRE_DescriptorPointer->InitializeLocalDescriptor
        ( Array_Domain, SerialArray->Array_Descriptor.Array_Domain );

  // This step is representative of a post processing step which PADRE should call 
  // and which we should isolate into a member function that would be called by 
  // PADRE after the PADRE_Descriptor is setup.  Is this a good idea.  I think 
  // NOT (now that I think about it more)!

  // printf ("In Allocate.C: SerialArray->Array_Descriptor.Array_Domain.View_Offset = %d \n",
  //      SerialArray->Array_Descriptor.Array_Domain.View_Offset);

     DESCRIPTOR_SERIAL_POINTER_LIST_INITIALIZATION_MACRO

  // #############################################################################
  // #######################  CASE OF USING PADRE (END)  #########################
  // #############################################################################

#else
  // #############################################################################
  // ########################  CASE OF NO PADRE (START)  #########################
  // #############################################################################

  // Make sure the array dimensions agree with PARTI.
     APP_ASSERT( MAX_DIM == MAX_ARRAY_DIMENSION );

     APP_ASSERT (Communication_Manager::Number_Of_Processors > 0);
     APP_ASSERT (Communication_Manager::Number_Of_Processors <= MAX_PROCESSORS);

  // Array_Domain.display("Array_Domain AT TOP before allocation of serial array!");

  // For better efficency we could treat the simple single processor case special
  // but for now we will skip special processing so we can debug the 
  // multiprocessor case.

  // Make sure that Virtual Processor Spaces is defined
     if (Communication_Manager::VirtualProcessorSpace == NULL)
        {
#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 0)
               printf ("Build virtual processor space! \n");
#endif
       // Block Parti only implemented with 1D virtual processor spaces
          int Sizes[1];
          Sizes[0] = Communication_Manager::Number_Of_Processors;
          APP_ASSERT (Communication_Manager::Number_Of_Processors > 0);
          Communication_Manager::VirtualProcessorSpace = vProc(1,Sizes);
        }
       else
        {
#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 0)
               printf ("Virtual Processor Space ALREADY BUILT! \n");
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
        {
          printf ("(Virtual processor Space size) - Communication_Manager");
          printf ("::Number_Of_Processors = %d \n",Communication_Manager::Number_Of_Processors);
        }
#endif

     APP_ASSERT(Communication_Manager::VirtualProcessorSpace != NULL);

  // Change the way we compute the array size since we want to be able to build the 
  // data storage that will represent a view.
  // APP_ASSERT (Array_Domain.Is_A_View == FALSE);

  // We skip the construction of a Parti descriptor for the case of a Null 
  // array because it is not required and because Parti could not build a 
  // descriptor for an array of ZERO size.  But P++ must provide a valid pointer 
  // to a valid serial Null array.

     if (Array_Domain.Is_A_Null_Array)
        {
       // Build pointer to a serial Null array and return from function
          SerialArray = new floatSerialArray();
          APP_ASSERT (SerialArray != NULL);

       // I think this is not required since it should have already been
       // set before calling this function
          for (i=0; i < MAX_ARRAY_DIMENSION; i++)
                Array_Domain.Local_Mask_Index [i] = Index(0,0,1,Null_Index);
          return;
        }

     int Array_Sizes[MAX_ARRAY_DIMENSION];
     Array_Domain.getRawDataSize (Array_Sizes);

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("\n");
          printf ("SIZES OF UNPARTITONED DATA (GLOBAL SIZES) \n");
          printf ("Building a partition with Array_Sizes[0]= %d\n",Array_Sizes[0]);
	  for (i=1; i < MAX_ARRAY_DIMENSION; i++)
             {
	       printf ("                          Array_Sizes[%d]= %d\n",i,Array_Sizes[i]);
	     }
          printf ("\n");
        }
#endif

  // Now to the alignment of array onto decomposition (finally building the parallel
  // array descriptor).  This niether allocates nor distributes any array data.
  // Special constructors will allow the user to specify specific data to exercise
  // greater control over the distribution of P++ arrays.  But for now we DEFINE
  // some defaults to allow a distribution onto the multiprocessor space.

     int Number_Of_Dimensions_To_Partition = Array_Domain.Domain_Dimension;
  // Bugfix (5/23/95) must handle case of combined Index and scalar indexing 
  // which would build a lower dimensional array
     APP_ASSERT (Number_Of_Dimensions_To_Partition >= Array_Domain_Type::computeArrayDimension ( Array_Domain ));

  // Build the Block Parti parallel descriptor
  // Get data from Partitioning object or use default if not available
     if ( Array_Domain.Partitioning_Object_Pointer != NULL )
        {
          APP_ASSERT( Array_Domain.Partitioning_Object_Pointer != NULL );

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("************************************** \n");
               printf ("Using an existing Partitioning_Object! \n");
               printf ("************************************** \n");
               Array_Domain.Partitioning_Object_Pointer->display("Using an existing Partitioning_Object!");
             }
#endif
       // In the case where the ghost boundaies are incremented the
       // BlockPartiArrayDecomposition is reused and so this is not a NULL pointer.
          if (Array_Domain.BlockPartiArrayDecomposition == NULL)
             {
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 0)
                    printf ("In Allocate_Parallel_Array -- BUILD A NEW Array_Domain.BlockPartiArrayDecomposition! \n");
#endif
               Array_Domain.BlockPartiArrayDecomposition = 
                  Array_Domain.Partitioning_Object_Pointer->Build_BlockPartiDecompostion ( Array_Sizes );

            // Bugfix (6/10/2000): delete any existing domain if we just built a new Decomposition
               if (Array_Domain.BlockPartiArrayDomain != NULL)
                  {
                 // Delete the Array_Domain.BlockPartiArrayDomain so that a new one will be built
                    delete_DARRAY ( Array_Domain.BlockPartiArrayDomain );
                    Array_Domain.BlockPartiArrayDomain = NULL;
                  }
             }
            else
             {
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("In Allocate_Parallel_Array -- Array_Domain.BlockPartiArrayDecomposition already exists! \n");
#endif
             }
          APP_ASSERT (Array_Domain.BlockPartiArrayDecomposition != NULL);

       // This really should be built again since it reflects the new ghost 
       // boundary widths.  At least in the case where this function is called 
       // after setInternalGhostCellWidth (we force the new one to be built only
       // if a new BlockPartiArrayDecomposition is built).

       // Bugfix (6/10/2000): sometimes this is a valid pointer and we want to just reuse it
       // APP_ASSERT (Array_Domain.BlockPartiArrayDomain == NULL);
          if (Array_Domain.BlockPartiArrayDomain == NULL)
             {
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("In Allocate_Parallel_Array -- BUILD A NEW Array_Domain.BlockPartiArrayDomain! \n");
#endif
               Array_Domain.BlockPartiArrayDomain = 
                  Array_Domain.Partitioning_Object_Pointer->
	             Build_BlockPartiArrayDomain 
	                (Array_Domain.BlockPartiArrayDecomposition , 
		         Array_Sizes, Array_Domain.InternalGhostCellWidth, 
		         Array_Domain.ExternalGhostCellWidth );
               APP_ASSERT(Array_Domain.BlockPartiArrayDomain != NULL);
             }
            else
             {
            // We could provide asserts that make sure it is the right one when we reuse it!
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("In Allocate_Parallel_Array -- Array_Domain.BlockPartiArrayDomain already exists! \n");
#endif
             }
          APP_ASSERT (Array_Domain.BlockPartiArrayDomain != NULL);
        }
       else
        {
          APP_ASSERT( Array_Domain.Partitioning_Object_Pointer == NULL );

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("*********************************************** \n");
               printf ("NO existing Partitioning_Object (use defaults)! \n");
               printf ("*********************************************** \n");
               Partitioning_Type::displayDefaultValues("NO existing Partitioning_Object (use defaults)!");
             }
#endif
          if (Array_Domain.Is_A_Null_Array == FALSE)
             {
            // In cases where the assignment is to a Null array the operator= 
	    // the Array descriptor is copied from the RHS the 
	    // BlockPartiArrayDecomposition point is copied as well so that 
	    // we have a valid pointer at this point.
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("In Allocate_Parallel_Array -- Array_Domain.Is_A_Null_Array == FALSE \n");
#endif
               if (Array_Domain.BlockPartiArrayDecomposition == NULL)
                  {
#if COMPILE_DEBUG_STATEMENTS
                    if (APP_DEBUG > 1)
                         printf ("In Allocate_Parallel_Array -- BUILD A NEW Array_Domai.Array_Domainn->BlockPartiArrayDecomposition! \n");
#endif
                    Array_Domain.BlockPartiArrayDecomposition = 
                         Partitioning_Type::Build_DefaultBlockPartiDecompostion ( Array_Sizes );
                    APP_ASSERT(Array_Domain.BlockPartiArrayDecomposition->referenceCount == 0);
                  }
                 else
                  {
#if COMPILE_DEBUG_STATEMENTS
                    if (APP_DEBUG > 1)
                         printf ("In Allocate_Parallel_Array -- Array_Domain.BlockPartiArrayDecomposition already exists! \n");
#endif
                  }
               APP_ASSERT (Array_Domain.BlockPartiArrayDecomposition != NULL);
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Inside of Parallel_Allocate:  (phase 1) Array_Domain.BlockPartiArrayDecomposition->referenceCount = %d \n",
                         Array_Domain.BlockPartiArrayDecomposition->referenceCount);
#endif
            // APP_ASSERT (Array_Domain.BlockPartiArrayDomain == NULL);
               if (Array_Domain.BlockPartiArrayDomain == NULL)
                  {
#if COMPILE_DEBUG_STATEMENTS
                    if (APP_DEBUG > 1)
                         printf ("In Allocate_Parallel_Array -- BUILD A NEW Array_Domain.BlockPartiArrayDomain! \n");
#endif
                    Array_Domain.BlockPartiArrayDomain =
                         Partitioning_Type::Build_DefaultBlockPartiArrayDomain (
                                   Array_Domain.BlockPartiArrayDecomposition , Array_Sizes ,
                                   Array_Domain.InternalGhostCellWidth , Array_Domain.ExternalGhostCellWidth );
                    APP_ASSERT(Array_Domain.BlockPartiArrayDomain->referenceCount == 0);
                  }
                 else
                  {
#if COMPILE_DEBUG_STATEMENTS
                    if (APP_DEBUG > 1)
                         printf ("In Allocate_Parallel_Array -- Array_Domain.BlockPartiArrayDomain already exists! \n");
#endif
                  }
               APP_ASSERT(Array_Domain.BlockPartiArrayDomain != NULL);
             }
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          APP_ASSERT(Array_Domain.BlockPartiArrayDomain        != NULL);
          APP_ASSERT(Array_Domain.BlockPartiArrayDecomposition != NULL);
          printf ("Inside of Parallel_Allocate (MIDDLE): Array_Domain.BlockPartiArrayDomain->referenceCount = %d \n",
               Array_Domain.BlockPartiArrayDomain->referenceCount);
          printf ("Inside of Parallel_Allocate (MIDDLE): Array_Domain.BlockPartiArrayDecomposition->referenceCount = %d \n",
               Array_Domain.BlockPartiArrayDecomposition->referenceCount);

        }
#endif
  // ****************************************************************************
  // ****************************************************************************
  // ******  Location of demarkation for the part of the function that ******
  // ******  can go into the Array_Descriptor_Type object (upper part)     ******
  // ******  and the part of the function that can go into the             ******
  // ******  Array_Domain_Type object.  (this is future work)              ******
  // ****************************************************************************
  // ****************************************************************************

  // Bugfix (12/18/94) to allow reuse of a P++ array object (required when a 
  // NULL P++ array object is initialized in the operator=() member function)

     if (SerialArray != NULL)
        {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Deleting an existing SerialArray! \n");
#endif
          APP_ASSERT(SerialArray->Array_Descriptor.Array_Domain.Is_A_Null_Array == TRUE);
          SerialArray->decrementReferenceCount();
          if (SerialArray->getReferenceCount() < SerialArray->getReferenceCountBase())
               delete SerialArray;
          SerialArray = NULL;
        }

     APP_ASSERT(SerialArray == NULL);

  // Get the size of the local partition for this processor and build a floatSerialArray of that size.
     const int UNIT_STRIDE = 1;

     int Local_Sizes[MAX_ARRAY_DIMENSION];

  // initialize to unit values (to start)
     for ( i=0; i<MAX_ARRAY_DIMENSION; i++ )
        {
          Local_Sizes[i] = 1;
        }

#if COMPILE_DEBUG_STATEMENTS
     Array_Domain.Test_Distribution_Consistency("Called from floatArray::Allocate_Parallel_Array");
  // printf ("Skipping call to Test_Distribution_Consistency in allocate.C \n");
#endif

  // Build base and size values to build SerialArray (laSizes includes the ghost boundaries!)
     APP_ASSERT (Array_Domain.BlockPartiArrayDomain != NULL);
     laSizes(Array_Domain.BlockPartiArrayDomain,Local_Sizes);

  // Fill in the rest of the dimensions.  This could be handled more efficently.
#if COMPILE_DEBUG_STATEMENTS
  // These values were initialized previously but let's make sure that PARTI has not changed them
     for (i = Array_Domain.Domain_Dimension; i < MAX_ARRAY_DIMENSION; i++)
          APP_ASSERT (Local_Sizes[i] == 1); 

     if (Array_Domain.Partitioning_Object_Pointer == NULL)
        {
       // Partitioning_Type::Test_Consistency ( Array_Domain.BlockPartiArrayDomain , 
       //      "Called from floatArray::Allocate_Parallel_Array" );
          Internal_Partitioning_Type::staticTestConsistency ( Array_Domain.BlockPartiArrayDomain ,
               "Called from floatArray::Allocate_Parallel_Array" );
        }
       else
        {
          Array_Domain.Partitioning_Object_Pointer->Test_Consistency ( 
               Array_Domain.BlockPartiArrayDomain , "Called from floatArray::Allocate_Parallel_Array" );
        }

     if (APP_DEBUG > 1)
        {
          printf ("Build SerialArray -- ");
          for (i=0; i < MAX_ARRAY_DIMENSION; i++)
             {
	       printf("Local_Sizes[%d] = %d ",i,Local_Sizes[i]);
             }
          printf("\n");
        }
#endif

  // Array_Domain.display("Array_Domain before allocation of serial array!");

  // The process is:
  //    1. allocate the serial array with a size specified from the block PARTI function
  //    2. modify the array_domain objects
  //          a. Mostly the serial array's array_domain
  //          b. Also somewhat the parallel array objects array_domain as well (but very little)

  // Build the local processor's array
  // We can't build this as a strided object because it could get truncated at the base and bounds
  // and we have to have the serial array be exactly the specified size to use the distribution
  // mechanisms.
     SerialArray = new floatSerialArray( ARRAY_TO_LIST_MACRO(Local_Sizes), Force_Memory_Allocation);

  // error checking
     APP_ASSERT(SerialArray != NULL);

     int Bases[MAX_ARRAY_DIMENSION];

  // initialize the work array
     for (i=0; i < MAX_ARRAY_DIMENSION; i++)
        {
          Bases[i] = 0;
        }

     bool Generated_A_Null_Array              = FALSE;
     int SerialArray_Cannot_Have_Contiguous_Data = FALSE;

  // Need to set the upper bound on the iteration to MAX_ARRAY_DIMENSION
  // since all the variables for each dimension must be initialized.
     int j;
     for (j=0; j < MAX_ARRAY_DIMENSION; j++)
        {
          if (j < Array_Domain.Domain_Dimension)
             {
               Bases[j] = gLBnd(Array_Domain.BlockPartiArrayDomain,j) - Array_Domain.InternalGhostCellWidth[j];
             }
            else
             {
            // ... (10/24/96) if ghost cell width is subtracted then local
            // base and bound will be out of range of global base and bound.
            // The local base and bound can't be increased to account for
            // this because array will be the wrong size. ...
            // Bases[j] = APP_Global_Array_Base - Array_Domain.InternalGhostCellWidth[j];

            // (6/28/2000) I think this should be set to ZERO instead of APP_Global_Array_Base!
               Bases[j] = APP_Global_Array_Base;
             }

       // Set the base to coorespond to the global address space -- but also 
       // set the Data_Base so it can be consistant with the P++ descriptor
       // This could be implemented without the function call overhead 
       // represented here but this simplifies the initial implementation for now.

#if 0
          printf ("(before setBase): getLocalBase(%d) = %d  getBase(%d) = %d  getLocalBound(%d) = %d getBound(%d) = %d \n",
               j,getLocalBase(j),j,getBase(j),j,getLocalBound(j),j,getBound(j));
	  printf ("(before setBase): SerialArray->getRawBase(%d) = %d  getRawBase(%d) = %d  SerialArray->getRawBound(%d) = %d getRawBound(%d) = %d \n",
		  j,SerialArray->getRawBase(j),j,getRawBase(j),j,SerialArray->getRawBound(j),j,getRawBound(j));
#endif

       // ... (10/28/96, kdb) need to add Data_Base otherwise code doesn't
       // work if Array_Domain has nonzero databases ...
       // DQ (7/2/2000): Should this be User_Base instead of Data_Base?
          SerialArray->setBase ( Bases[j] + Array_Domain.Data_Base[j], j );
        }

  // error checking
     APP_ASSERT(SerialArray != NULL);

  // details specific to the setup of the descriptor and separate from PADRE are isolated into
  // a separate function so that PADRE and non-PADRE versions of P++ can use a single version!
     Array_Domain.postAllocationSupport(SerialArray->Array_Descriptor.Array_Domain);

  // Now initialize the pointers (the one thing we could not place into the postAllocationSupport() member function)
     DESCRIPTOR_SERIAL_POINTER_LIST_INITIALIZATION_MACRO;

  // #############################################################################
  // #########################  CASE OF NO PADRE (END)  ##########################
  // #############################################################################
#endif

  // Temp code (and intermediate result)
  // SerialArray->Array_Descriptor.Array_Domain.display("In allocate.C: SerialArray->Array_Descriptor.Array_Domain");

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("At base of floatArray_Descriptor_Type::Allocate_Parallel_Array \n");

     Test_Consistency ("Called from base of floatArray_Descriptor_Type::Allocate_Parallel_Array");
#endif

  // printf ("Exiting at BASE of floatArray_Descriptor_Type::Allocate_Parallel_Array() \n");
  // APP_ABORT();
   }
#else
#error "(allocate.C) This is only code for P++"
#endif

#undef FLOATARRAY

#define INTARRAY
#if defined(PPP)
// ********************************************************************************
// This function localizes details related to parallel allocation of the array data.
// It is called from the intArray::Allocate_Array_Data member function.
// In A++ the role of the "Allocate_Array" member function is to just allocate the
// raw array data used in the array object.  In this case the P++ descriptor is
// modified (the PARTI parallel descriptor is built and other modifications done to
// the P++ descriptor).  We might like to have the descriptors fully built before
// allocating the serial array data (and A++ array) -- but this would require that
// the distribution be defined at the time when the P++ descriptor is built
// and we need for the data AND the distribution to be defined AFTER the descriptor
// is built.  This simplified the specification of complex distributions as they
// arise in applications like adaptive mesh refinement.
// ********************************************************************************
void intArray_Descriptor_Type::
Allocate_Parallel_Array ( bool Force_Memory_Allocation )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of intArray_Descriptor_Type::Allocate_Parallel_Array! \n");
#endif

     int i;  // Local loop INDEX variable

#if defined(USE_PADRE)
  // #############################################################################
  // ######################  CASE OF USING PADRE (START)  ########################
  // #############################################################################

  // ************************************************************************
  //            THIS CODE DEMONSTRATES THE USE OF PADRE in P++
  // ************************************************************************

  // Make sure the array dimensions agree with PADRE.
     if( PADRE_MAX_ARRAY_DIMENSION != MAX_ARRAY_DIMENSION )
        {
          printf ("ERROR: PADRE_MAX_ARRAY_DIMENSION = %d != MAX_ARRAY_DIMENSION = %d \n",
               PADRE_MAX_ARRAY_DIMENSION,MAX_ARRAY_DIMENSION);
        }
     APP_ASSERT( PADRE_MAX_ARRAY_DIMENSION == MAX_ARRAY_DIMENSION );

     int Local_Sizes[MAX_ARRAY_DIMENSION];
     for (i=0; i < MAX_ARRAY_DIMENSION; i++)
          Local_Sizes[i] = 1;

#if COMPILE_DEBUG_STATEMENTS
  // Array_Domain.Test_Distribution_Consistency ("Called from intArray::Allocate_Parallel_Array");
#endif

     if (Array_Domain.Is_A_Null_Array)
        {
       // Build pointer to a serial Null array and return from function
          SerialArray = new intSerialArray();
          APP_ASSERT (SerialArray != NULL);

          for (i=0; i < MAX_ARRAY_DIMENSION; i++)
             Array_Domain.Local_Mask_Index [i] = Index(0,0,1,Null_Index);

          return;
        }

  // In some cases we get to this point and the parallelPADRE_DescriptorPointer
  // is NULL (so fix that here).
  // This is really the only place where the parallelPADRE_DescriptorPointer
  // should be initialized.
     if (Array_Domain.parallelPADRE_DescriptorPointer == NULL)
        {
          if (Array_Domain.Partitioning_Object_Pointer != NULL)
             {
               APP_ASSERT (Array_Domain.Partitioning_Object_Pointer != NULL);
               APP_ASSERT (Array_Domain.Partitioning_Object_Pointer->distributionPointer != NULL);

               Array_Domain.parallelPADRE_DescriptorPointer = 
                    new PADRE_Descriptor <BaseArray,Array_Domain_Type,SerialArray_Domain_Type>
                      (&(Array_Domain), Array_Domain.Partitioning_Object_Pointer->distributionPointer);
               APP_ASSERT (Array_Domain.parallelPADRE_DescriptorPointer != NULL);
             }
            else
             {
            // ... using DEFAULT PADRE_Distribtuion ...
               Array_Domain.parallelPADRE_DescriptorPointer = 
                    new PADRE_Descriptor <BaseArray,Array_Domain_Type,SerialArray_Domain_Type> (&Array_Domain);
               APP_ASSERT (Array_Domain.parallelPADRE_DescriptorPointer != NULL);
             }
        }

   // ... Some distribution libraries must construct their own data.  If
   // that is the case then adopt the data in the serialArray instead
   // of allocating the memory ...
      int preallocated_data = FALSE;
      int* local_data = NULL;

      Array_Domain.parallelPADRE_DescriptorPointer->allocateData(preallocated_data, &local_data); 

  // Find base and size values to build SerialArray (Local_Sizes includes the ghost boundaries!)
     APP_ASSERT (Array_Domain.parallelPADRE_DescriptorPointer != NULL);
     Array_Domain.parallelPADRE_DescriptorPointer->getLocalSizes(Local_Sizes);

  // Fill in the rest of the dimensions.  This could be handled more efficently.
  // These values were initialized previously but let's make sure that PARTI 
  // has not changed them

     for (i = Array_Domain.Domain_Dimension; i < MAX_ARRAY_DIMENSION; i++)
          APP_ASSERT (Local_Sizes[i] == 1);

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In intArray_Descriptor_Type::Allocate_Parallel_Array()");
	  printf ("--- Build SerialArray --\n");
	  printf ("Local_Sizes[0:%d] = ", MAX_ARRAY_DIMENSION);
	  printf(IO_CONTROL_STRING_MACRO_INTEGER, ARRAY_TO_LIST_MACRO(Local_Sizes) );
	  printf("\n");
        }
#endif

     if (SerialArray != NULL)
        {
       // delete any existing serial array object
          APP_ASSERT (SerialArray->getReferenceCount() >= intSerialArray::getReferenceCountBase());
          SerialArray->decrementReferenceCount();
          if (SerialArray->getReferenceCount() < intSerialArray::getReferenceCountBase())
             {
            // printf ("Deleting the serial array in allocate.C \n");
               delete SerialArray;
             }
          SerialArray = NULL;
        }

     if (preallocated_data)
        {
       // ... distribution library needed to allocate the data ...
          SerialArray = new intSerialArray ( local_data, ARRAY_TO_LIST_MACRO(Local_Sizes) );
        }
       else
        {
          SerialArray = new intSerialArray ( ARRAY_TO_LIST_MACRO(Local_Sizes), Force_Memory_Allocation);
        }

  // Need to allocate the data for the array 
     APP_ASSERT (SerialArray != NULL);

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 3)
        {
          printf ("########################################################## \n");
          printf ("########################################################## \n");
          printf ("##   SERIAL ARRAY IS BUILT --- NOW RESET LOCAL DOMAIN   ## \n");
          printf ("########################################################## \n");
          printf ("########################################################## \n");
        }
#endif

  // Initialize PADRE's pointer to the local domain
  // However if this array is a multiple reference (i.e not the orginal data)
  // then we don't want to replace the existing link of the original data's
  // domain to the PADRE_Descriptor.  So only set it if it is unset!
  // This might have to be a stack of these to avoid the array that represented 
  // the initial reference from being deleted which other arrays were using the 
  // PADRE_Descriptor (something that purify would report about).
  // This allows for views to reference the same PADRE_Descriptor (less run-time 
  // overhead).

     if (Array_Domain.parallelPADRE_DescriptorPointer->getLocalDomain() == NULL)
          Array_Domain.parallelPADRE_DescriptorPointer->setLocalDomain
             ( &(SerialArray->Array_Descriptor.Array_Domain) );

  // Now we have to initialize the local descriptor in the SerialArray object
  // We want to use only the input domain not the one necessarily stored in 
  // the PADRE_Descriptor.

     Array_Domain.parallelPADRE_DescriptorPointer->InitializeLocalDescriptor
        ( Array_Domain, SerialArray->Array_Descriptor.Array_Domain );

  // This step is representative of a post processing step which PADRE should call 
  // and which we should isolate into a member function that would be called by 
  // PADRE after the PADRE_Descriptor is setup.  Is this a good idea.  I think 
  // NOT (now that I think about it more)!

  // printf ("In Allocate.C: SerialArray->Array_Descriptor.Array_Domain.View_Offset = %d \n",
  //      SerialArray->Array_Descriptor.Array_Domain.View_Offset);

     DESCRIPTOR_SERIAL_POINTER_LIST_INITIALIZATION_MACRO

  // #############################################################################
  // #######################  CASE OF USING PADRE (END)  #########################
  // #############################################################################

#else
  // #############################################################################
  // ########################  CASE OF NO PADRE (START)  #########################
  // #############################################################################

  // Make sure the array dimensions agree with PARTI.
     APP_ASSERT( MAX_DIM == MAX_ARRAY_DIMENSION );

     APP_ASSERT (Communication_Manager::Number_Of_Processors > 0);
     APP_ASSERT (Communication_Manager::Number_Of_Processors <= MAX_PROCESSORS);

  // Array_Domain.display("Array_Domain AT TOP before allocation of serial array!");

  // For better efficency we could treat the simple single processor case special
  // but for now we will skip special processing so we can debug the 
  // multiprocessor case.

  // Make sure that Virtual Processor Spaces is defined
     if (Communication_Manager::VirtualProcessorSpace == NULL)
        {
#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 0)
               printf ("Build virtual processor space! \n");
#endif
       // Block Parti only implemented with 1D virtual processor spaces
          int Sizes[1];
          Sizes[0] = Communication_Manager::Number_Of_Processors;
          APP_ASSERT (Communication_Manager::Number_Of_Processors > 0);
          Communication_Manager::VirtualProcessorSpace = vProc(1,Sizes);
        }
       else
        {
#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 0)
               printf ("Virtual Processor Space ALREADY BUILT! \n");
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
        {
          printf ("(Virtual processor Space size) - Communication_Manager");
          printf ("::Number_Of_Processors = %d \n",Communication_Manager::Number_Of_Processors);
        }
#endif

     APP_ASSERT(Communication_Manager::VirtualProcessorSpace != NULL);

  // Change the way we compute the array size since we want to be able to build the 
  // data storage that will represent a view.
  // APP_ASSERT (Array_Domain.Is_A_View == FALSE);

  // We skip the construction of a Parti descriptor for the case of a Null 
  // array because it is not required and because Parti could not build a 
  // descriptor for an array of ZERO size.  But P++ must provide a valid pointer 
  // to a valid serial Null array.

     if (Array_Domain.Is_A_Null_Array)
        {
       // Build pointer to a serial Null array and return from function
          SerialArray = new intSerialArray();
          APP_ASSERT (SerialArray != NULL);

       // I think this is not required since it should have already been
       // set before calling this function
          for (i=0; i < MAX_ARRAY_DIMENSION; i++)
                Array_Domain.Local_Mask_Index [i] = Index(0,0,1,Null_Index);
          return;
        }

     int Array_Sizes[MAX_ARRAY_DIMENSION];
     Array_Domain.getRawDataSize (Array_Sizes);

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("\n");
          printf ("SIZES OF UNPARTITONED DATA (GLOBAL SIZES) \n");
          printf ("Building a partition with Array_Sizes[0]= %d\n",Array_Sizes[0]);
	  for (i=1; i < MAX_ARRAY_DIMENSION; i++)
             {
	       printf ("                          Array_Sizes[%d]= %d\n",i,Array_Sizes[i]);
	     }
          printf ("\n");
        }
#endif

  // Now to the alignment of array onto decomposition (finally building the parallel
  // array descriptor).  This niether allocates nor distributes any array data.
  // Special constructors will allow the user to specify specific data to exercise
  // greater control over the distribution of P++ arrays.  But for now we DEFINE
  // some defaults to allow a distribution onto the multiprocessor space.

     int Number_Of_Dimensions_To_Partition = Array_Domain.Domain_Dimension;
  // Bugfix (5/23/95) must handle case of combined Index and scalar indexing 
  // which would build a lower dimensional array
     APP_ASSERT (Number_Of_Dimensions_To_Partition >= Array_Domain_Type::computeArrayDimension ( Array_Domain ));

  // Build the Block Parti parallel descriptor
  // Get data from Partitioning object or use default if not available
     if ( Array_Domain.Partitioning_Object_Pointer != NULL )
        {
          APP_ASSERT( Array_Domain.Partitioning_Object_Pointer != NULL );

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("************************************** \n");
               printf ("Using an existing Partitioning_Object! \n");
               printf ("************************************** \n");
               Array_Domain.Partitioning_Object_Pointer->display("Using an existing Partitioning_Object!");
             }
#endif
       // In the case where the ghost boundaies are incremented the
       // BlockPartiArrayDecomposition is reused and so this is not a NULL pointer.
          if (Array_Domain.BlockPartiArrayDecomposition == NULL)
             {
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 0)
                    printf ("In Allocate_Parallel_Array -- BUILD A NEW Array_Domain.BlockPartiArrayDecomposition! \n");
#endif
               Array_Domain.BlockPartiArrayDecomposition = 
                  Array_Domain.Partitioning_Object_Pointer->Build_BlockPartiDecompostion ( Array_Sizes );

            // Bugfix (6/10/2000): delete any existing domain if we just built a new Decomposition
               if (Array_Domain.BlockPartiArrayDomain != NULL)
                  {
                 // Delete the Array_Domain.BlockPartiArrayDomain so that a new one will be built
                    delete_DARRAY ( Array_Domain.BlockPartiArrayDomain );
                    Array_Domain.BlockPartiArrayDomain = NULL;
                  }
             }
            else
             {
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("In Allocate_Parallel_Array -- Array_Domain.BlockPartiArrayDecomposition already exists! \n");
#endif
             }
          APP_ASSERT (Array_Domain.BlockPartiArrayDecomposition != NULL);

       // This really should be built again since it reflects the new ghost 
       // boundary widths.  At least in the case where this function is called 
       // after setInternalGhostCellWidth (we force the new one to be built only
       // if a new BlockPartiArrayDecomposition is built).

       // Bugfix (6/10/2000): sometimes this is a valid pointer and we want to just reuse it
       // APP_ASSERT (Array_Domain.BlockPartiArrayDomain == NULL);
          if (Array_Domain.BlockPartiArrayDomain == NULL)
             {
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("In Allocate_Parallel_Array -- BUILD A NEW Array_Domain.BlockPartiArrayDomain! \n");
#endif
               Array_Domain.BlockPartiArrayDomain = 
                  Array_Domain.Partitioning_Object_Pointer->
	             Build_BlockPartiArrayDomain 
	                (Array_Domain.BlockPartiArrayDecomposition , 
		         Array_Sizes, Array_Domain.InternalGhostCellWidth, 
		         Array_Domain.ExternalGhostCellWidth );
               APP_ASSERT(Array_Domain.BlockPartiArrayDomain != NULL);
             }
            else
             {
            // We could provide asserts that make sure it is the right one when we reuse it!
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("In Allocate_Parallel_Array -- Array_Domain.BlockPartiArrayDomain already exists! \n");
#endif
             }
          APP_ASSERT (Array_Domain.BlockPartiArrayDomain != NULL);
        }
       else
        {
          APP_ASSERT( Array_Domain.Partitioning_Object_Pointer == NULL );

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("*********************************************** \n");
               printf ("NO existing Partitioning_Object (use defaults)! \n");
               printf ("*********************************************** \n");
               Partitioning_Type::displayDefaultValues("NO existing Partitioning_Object (use defaults)!");
             }
#endif
          if (Array_Domain.Is_A_Null_Array == FALSE)
             {
            // In cases where the assignment is to a Null array the operator= 
	    // the Array descriptor is copied from the RHS the 
	    // BlockPartiArrayDecomposition point is copied as well so that 
	    // we have a valid pointer at this point.
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("In Allocate_Parallel_Array -- Array_Domain.Is_A_Null_Array == FALSE \n");
#endif
               if (Array_Domain.BlockPartiArrayDecomposition == NULL)
                  {
#if COMPILE_DEBUG_STATEMENTS
                    if (APP_DEBUG > 1)
                         printf ("In Allocate_Parallel_Array -- BUILD A NEW Array_Domai.Array_Domainn->BlockPartiArrayDecomposition! \n");
#endif
                    Array_Domain.BlockPartiArrayDecomposition = 
                         Partitioning_Type::Build_DefaultBlockPartiDecompostion ( Array_Sizes );
                    APP_ASSERT(Array_Domain.BlockPartiArrayDecomposition->referenceCount == 0);
                  }
                 else
                  {
#if COMPILE_DEBUG_STATEMENTS
                    if (APP_DEBUG > 1)
                         printf ("In Allocate_Parallel_Array -- Array_Domain.BlockPartiArrayDecomposition already exists! \n");
#endif
                  }
               APP_ASSERT (Array_Domain.BlockPartiArrayDecomposition != NULL);
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Inside of Parallel_Allocate:  (phase 1) Array_Domain.BlockPartiArrayDecomposition->referenceCount = %d \n",
                         Array_Domain.BlockPartiArrayDecomposition->referenceCount);
#endif
            // APP_ASSERT (Array_Domain.BlockPartiArrayDomain == NULL);
               if (Array_Domain.BlockPartiArrayDomain == NULL)
                  {
#if COMPILE_DEBUG_STATEMENTS
                    if (APP_DEBUG > 1)
                         printf ("In Allocate_Parallel_Array -- BUILD A NEW Array_Domain.BlockPartiArrayDomain! \n");
#endif
                    Array_Domain.BlockPartiArrayDomain =
                         Partitioning_Type::Build_DefaultBlockPartiArrayDomain (
                                   Array_Domain.BlockPartiArrayDecomposition , Array_Sizes ,
                                   Array_Domain.InternalGhostCellWidth , Array_Domain.ExternalGhostCellWidth );
                    APP_ASSERT(Array_Domain.BlockPartiArrayDomain->referenceCount == 0);
                  }
                 else
                  {
#if COMPILE_DEBUG_STATEMENTS
                    if (APP_DEBUG > 1)
                         printf ("In Allocate_Parallel_Array -- Array_Domain.BlockPartiArrayDomain already exists! \n");
#endif
                  }
               APP_ASSERT(Array_Domain.BlockPartiArrayDomain != NULL);
             }
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          APP_ASSERT(Array_Domain.BlockPartiArrayDomain        != NULL);
          APP_ASSERT(Array_Domain.BlockPartiArrayDecomposition != NULL);
          printf ("Inside of Parallel_Allocate (MIDDLE): Array_Domain.BlockPartiArrayDomain->referenceCount = %d \n",
               Array_Domain.BlockPartiArrayDomain->referenceCount);
          printf ("Inside of Parallel_Allocate (MIDDLE): Array_Domain.BlockPartiArrayDecomposition->referenceCount = %d \n",
               Array_Domain.BlockPartiArrayDecomposition->referenceCount);

        }
#endif
  // ****************************************************************************
  // ****************************************************************************
  // ******  Location of demarkation for the part of the function that ******
  // ******  can go into the Array_Descriptor_Type object (upper part)     ******
  // ******  and the part of the function that can go into the             ******
  // ******  Array_Domain_Type object.  (this is future work)              ******
  // ****************************************************************************
  // ****************************************************************************

  // Bugfix (12/18/94) to allow reuse of a P++ array object (required when a 
  // NULL P++ array object is initialized in the operator=() member function)

     if (SerialArray != NULL)
        {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Deleting an existing SerialArray! \n");
#endif
          APP_ASSERT(SerialArray->Array_Descriptor.Array_Domain.Is_A_Null_Array == TRUE);
          SerialArray->decrementReferenceCount();
          if (SerialArray->getReferenceCount() < SerialArray->getReferenceCountBase())
               delete SerialArray;
          SerialArray = NULL;
        }

     APP_ASSERT(SerialArray == NULL);

  // Get the size of the local partition for this processor and build a intSerialArray of that size.
     const int UNIT_STRIDE = 1;

     int Local_Sizes[MAX_ARRAY_DIMENSION];

  // initialize to unit values (to start)
     for ( i=0; i<MAX_ARRAY_DIMENSION; i++ )
        {
          Local_Sizes[i] = 1;
        }

#if COMPILE_DEBUG_STATEMENTS
     Array_Domain.Test_Distribution_Consistency("Called from intArray::Allocate_Parallel_Array");
  // printf ("Skipping call to Test_Distribution_Consistency in allocate.C \n");
#endif

  // Build base and size values to build SerialArray (laSizes includes the ghost boundaries!)
     APP_ASSERT (Array_Domain.BlockPartiArrayDomain != NULL);
     laSizes(Array_Domain.BlockPartiArrayDomain,Local_Sizes);

  // Fill in the rest of the dimensions.  This could be handled more efficently.
#if COMPILE_DEBUG_STATEMENTS
  // These values were initialized previously but let's make sure that PARTI has not changed them
     for (i = Array_Domain.Domain_Dimension; i < MAX_ARRAY_DIMENSION; i++)
          APP_ASSERT (Local_Sizes[i] == 1); 

     if (Array_Domain.Partitioning_Object_Pointer == NULL)
        {
       // Partitioning_Type::Test_Consistency ( Array_Domain.BlockPartiArrayDomain , 
       //      "Called from intArray::Allocate_Parallel_Array" );
          Internal_Partitioning_Type::staticTestConsistency ( Array_Domain.BlockPartiArrayDomain ,
               "Called from intArray::Allocate_Parallel_Array" );
        }
       else
        {
          Array_Domain.Partitioning_Object_Pointer->Test_Consistency ( 
               Array_Domain.BlockPartiArrayDomain , "Called from intArray::Allocate_Parallel_Array" );
        }

     if (APP_DEBUG > 1)
        {
          printf ("Build SerialArray -- ");
          for (i=0; i < MAX_ARRAY_DIMENSION; i++)
             {
	       printf("Local_Sizes[%d] = %d ",i,Local_Sizes[i]);
             }
          printf("\n");
        }
#endif

  // Array_Domain.display("Array_Domain before allocation of serial array!");

  // The process is:
  //    1. allocate the serial array with a size specified from the block PARTI function
  //    2. modify the array_domain objects
  //          a. Mostly the serial array's array_domain
  //          b. Also somewhat the parallel array objects array_domain as well (but very little)

  // Build the local processor's array
  // We can't build this as a strided object because it could get truncated at the base and bounds
  // and we have to have the serial array be exactly the specified size to use the distribution
  // mechanisms.
     SerialArray = new intSerialArray( ARRAY_TO_LIST_MACRO(Local_Sizes), Force_Memory_Allocation);

  // error checking
     APP_ASSERT(SerialArray != NULL);

     int Bases[MAX_ARRAY_DIMENSION];

  // initialize the work array
     for (i=0; i < MAX_ARRAY_DIMENSION; i++)
        {
          Bases[i] = 0;
        }

     bool Generated_A_Null_Array              = FALSE;
     int SerialArray_Cannot_Have_Contiguous_Data = FALSE;

  // Need to set the upper bound on the iteration to MAX_ARRAY_DIMENSION
  // since all the variables for each dimension must be initialized.
     int j;
     for (j=0; j < MAX_ARRAY_DIMENSION; j++)
        {
          if (j < Array_Domain.Domain_Dimension)
             {
               Bases[j] = gLBnd(Array_Domain.BlockPartiArrayDomain,j) - Array_Domain.InternalGhostCellWidth[j];
             }
            else
             {
            // ... (10/24/96) if ghost cell width is subtracted then local
            // base and bound will be out of range of global base and bound.
            // The local base and bound can't be increased to account for
            // this because array will be the wrong size. ...
            // Bases[j] = APP_Global_Array_Base - Array_Domain.InternalGhostCellWidth[j];

            // (6/28/2000) I think this should be set to ZERO instead of APP_Global_Array_Base!
               Bases[j] = APP_Global_Array_Base;
             }

       // Set the base to coorespond to the global address space -- but also 
       // set the Data_Base so it can be consistant with the P++ descriptor
       // This could be implemented without the function call overhead 
       // represented here but this simplifies the initial implementation for now.

#if 0
          printf ("(before setBase): getLocalBase(%d) = %d  getBase(%d) = %d  getLocalBound(%d) = %d getBound(%d) = %d \n",
               j,getLocalBase(j),j,getBase(j),j,getLocalBound(j),j,getBound(j));
	  printf ("(before setBase): SerialArray->getRawBase(%d) = %d  getRawBase(%d) = %d  SerialArray->getRawBound(%d) = %d getRawBound(%d) = %d \n",
		  j,SerialArray->getRawBase(j),j,getRawBase(j),j,SerialArray->getRawBound(j),j,getRawBound(j));
#endif

       // ... (10/28/96, kdb) need to add Data_Base otherwise code doesn't
       // work if Array_Domain has nonzero databases ...
       // DQ (7/2/2000): Should this be User_Base instead of Data_Base?
          SerialArray->setBase ( Bases[j] + Array_Domain.Data_Base[j], j );
        }

  // error checking
     APP_ASSERT(SerialArray != NULL);

  // details specific to the setup of the descriptor and separate from PADRE are isolated into
  // a separate function so that PADRE and non-PADRE versions of P++ can use a single version!
     Array_Domain.postAllocationSupport(SerialArray->Array_Descriptor.Array_Domain);

  // Now initialize the pointers (the one thing we could not place into the postAllocationSupport() member function)
     DESCRIPTOR_SERIAL_POINTER_LIST_INITIALIZATION_MACRO;

  // #############################################################################
  // #########################  CASE OF NO PADRE (END)  ##########################
  // #############################################################################
#endif

  // Temp code (and intermediate result)
  // SerialArray->Array_Descriptor.Array_Domain.display("In allocate.C: SerialArray->Array_Descriptor.Array_Domain");

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("At base of intArray_Descriptor_Type::Allocate_Parallel_Array \n");

     Test_Consistency ("Called from base of intArray_Descriptor_Type::Allocate_Parallel_Array");
#endif

  // printf ("Exiting at BASE of intArray_Descriptor_Type::Allocate_Parallel_Array() \n");
  // APP_ABORT();
   }
#else
#error "(allocate.C) This is only code for P++"
#endif

#undef INTARRAY






