#define COMPILE_PPP

#include "A++.h"

#if !defined(USE_PADRE)
// extern "C" MPI_Comm Global_PARTI_PADRE_Interface_PADRE_Comm_World;
// extern MPI_Comm Global_PARTI_PADRE_Interface_PADRE_Comm_World;
#endif



#define DOUBLEARRAY
void
doubleArray::Scalar_Indexing_For_doubleArray_With_Message_Passing (
          int Address_Subscript, bool Off_Processor_With_Ghost_Boundaries,
          bool Off_Processor_Without_Ghost_Boundaries, int & Array_Index_For_References) const
   {
  // Send of receive messages as required for scalar indexing in parallel
  // Basically we go and get the data if it is on another processor OR we
  // broadcast the data if we own the data.  It is simple -- but expensive!
  // In the case of Optimization_Manager::Optimize_Scalar_Indexing == TRUE we can
  // skip this message passing and just return a reference to the data element
  // if we own it or a dummy variable if we don't.

#if defined(PPP)
  // This code is only important for P++.  It handles the message passing required to support
  // the scalar indexing (if it is left unoptimized).

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
        {
          printf ("Off_Processor_With_Ghost_Boundaries            = %s \n",(Off_Processor_With_Ghost_Boundaries)    ? "TRUE" : "FALSE" );
          printf ("Off_Processor_Without_Ghost_Boundaries         = %s \n",(Off_Processor_Without_Ghost_Boundaries) ? "TRUE" : "FALSE" );
          printf ("Optimization_Manager::Optimize_Scalar_Indexing = %s \n",(Optimization_Manager::Optimize_Scalar_Indexing) ? "TRUE" : "FALSE" );
        }
#endif

#if COMPILE_DEBUG_STATEMENTS
     Communication_Manager::Sync();
#endif

     MPI_Status status;
     int return_status                                         = MPI_SUCCESS;
     int GLOBAL_MPI_MESSAGE_TYPE_FOR_SCALAR_INDEXING_BROADCAST = 503;
     int My_Rank;
     double  store_to_check;
     double  temp_store;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("BEFORE: Static_double_Variable[%d] = %f \n",
                  Array_Index_For_References,(double)Static_double_Variable[Array_Index_For_References]);
          printf ("GLOBAL_MPI_MESSAGE_TYPE_FOR_SCALAR_INDEXING_BROADCAST = %d \n",
                  GLOBAL_MPI_MESSAGE_TYPE_FOR_SCALAR_INDEXING_BROADCAST);

          if (Off_Processor_With_Ghost_Boundaries)
               printf ("Value of array object (Array_ID=%d) at scalar INDEX (%d) IS OFF_PROCESSOR \n",
                       Array_Descriptor.Array_ID(),Address_Subscript);
            else
               printf ("Value of array object (Array_ID=%d) at scalar INDEX (%d) = %f \n",
                       Array_Descriptor.Array_ID(),Address_Subscript,
                       (double)Array_Descriptor.SerialArray->Array_Descriptor.
			  Array_Data [Address_Subscript]);
        }
#endif

  // figure out the node number using MPI calls

     return_status = MPI_Comm_rank(MPI_COMM_WORLD, &My_Rank);
     APP_ASSERT(return_status == MPI_SUCCESS);

  // We have to be able to reference many values simultaniously so we keep them in
  // an array and cycle through the array as a circular list.

     APP_ASSERT (Array_Index_For_References <= STATIC_LIMIT_FOR_PARALLEL_SCALAR_INDEXING_REFERENCES);
     if (Array_Index_For_References >= STATIC_LIMIT_FOR_PARALLEL_SCALAR_INDEXING_REFERENCES)
        {
#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 0)
               printf ("Resetting Array_Index_For_References to ZERO! \n");
#endif
          Array_Index_For_References = 0;
        }


     if ( (Off_Processor_Without_Ghost_Boundaries == FALSE) && (My_Rank != 0) )
        {
       // PARTI does not INCLUDE any broadcasts as part of it's message passing
       // so we write this explicitly using MPI calls

       // now we are on the processor that owns the data and the rank of that processor
       // is not zero so we have to send that data to processor 0 in order to bcast it
       // to all the others

          temp_store = Array_Descriptor.SerialArray->Array_Descriptor.
	     Array_Data[Address_Subscript];

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 0)
               printf ("ON PROCESSOR CASE (without ghost boundaries)! rank != 0 \n");
#endif
       // first we have to send the data to node 0 in order to make sure that
       // every node knows which node is the root-node during the broadcast
       // if we are already on node 0 we can proceed


#if defined(USE_PADRE)
          return_status = MPI_Send((void*)(&temp_store),
                                   1, MPI_DOUBLE, 0, 
                                   GLOBAL_MPI_MESSAGE_TYPE_FOR_SCALAR_INDEXING_BROADCAST,
                                   PADRE::MPICommunicator());
#else
      // Since we are linking with the version of PARTI in the PADRE library
      // we want to use the Global_PARTI_PADRE_Interface_PADRE_Comm_World variable.
      //  return_status = MPI_Send((void*)(&temp_store),
      //                           1, MPI_DOUBLE, 0, 
      //                           GLOBAL_MPI_MESSAGE_TYPE_FOR_SCALAR_INDEXING_BROADCAST,
      //                           Global_PARTI_P_plus_plus_Interface_PPP_Comm_World);
          return_status = MPI_Send((void*)(&temp_store),
                                   1, MPI_DOUBLE, 0, 
                                   GLOBAL_MPI_MESSAGE_TYPE_FOR_SCALAR_INDEXING_BROADCAST,
                                   Global_PARTI_PADRE_Interface_PADRE_Comm_World);
#endif

          APP_ASSERT(return_status == MPI_SUCCESS);

       // store value that has been sent in order to check later whether the same value
       // got received

       // Static_Variable array hasn't been set yet so this is an error
          //store_to_check = Static_double_Variable[Array_Index_For_References];
          store_to_check = temp_store; 

        }

     if ( (Off_Processor_Without_Ghost_Boundaries == TRUE) && (My_Rank == 0) )
        {
#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 0)
               printf ("ON PROCESSOR CASE (without ghost boundaries)! rank == 0 \n");
#endif
       // on node 0 we have to receive that data 

          return_status = MPI_Recv((void*)(&temp_store),
                                   1, MPI_DOUBLE, MPI_ANY_SOURCE, 
                                   GLOBAL_MPI_MESSAGE_TYPE_FOR_SCALAR_INDEXING_BROADCAST,
#if defined(USE_PADRE)
                                   PADRE::MPICommunicator(),
#else
                                // Since we are linking with the version of PARTI in the PADRE library
                                // we want to use the Global_PARTI_PADRE_Interface_PADRE_Comm_World variable.
                                // Global_PARTI_P_plus_plus_Interface_PPP_Comm_World,
                                   Global_PARTI_PADRE_Interface_PADRE_Comm_World,
#endif
                                   &status);

          APP_ASSERT(return_status == MPI_SUCCESS);
        }

     if ( (Off_Processor_Without_Ghost_Boundaries == FALSE) && (My_Rank == 0) )
       {
      // this is the case where processor zero actually has the data so we only need to
      // copy it the the right place in order to broadcast it

      // printf ("Assign to temp_store: Address_Subscript = %d \n",Address_Subscript);

	 temp_store = Array_Descriptor.SerialArray->Array_Descriptor.Array_Data[Address_Subscript];
       }
     
     // now node 0 has the data in temp_store and we can proceed with a broadcast
     // where root is node 0

     // MPI_Bcast already knows the type and so the number of values being
     // sent should be 1
        //return_status = MPI_Bcast((void*)(&temp_store),
        //                            sizeof( double ), MPI_DOUBLE, 0, MPI_COMM_WORLD);
        return_status = MPI_Bcast((void*)(&temp_store),
                                    1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        APP_ASSERT(return_status == MPI_SUCCESS);

     // now all the nodes should contain the same value in temp_store but oly the ones that
     // did not own the data in the first place have to copy it
     if (Off_Processor_Without_Ghost_Boundaries == TRUE)  
	 Static_double_Variable[Array_Index_For_References] = temp_store;

#if 0
#ifndef NDEBUG
     // test if the node that originally owned the data that was sent to node 0 in order 
     // to broadcast it receives the same value from the broadcast
     if ( (Off_Processor_Without_Ghost_Boundaries == FALSE) && (My_Rank != 0) )
       {
	 APP_ASSERT(Array_Descriptor.SerialArray->Array_Descriptor.
	    Array_Data[Address_Subscript] == store_to_check);
       }
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Off_Processor_Without_Ghost_Boundaries == FALSE )
         {
            APP_ASSERT(Array_Descriptor.SerialArray != NULL);
            APP_ASSERT(Array_Descriptor.SerialArray->Array_Descriptor.
	               Array_Data != NULL);
            if (APP_DEBUG > 0)
                 printf ("AFTER: Array_Descriptor.SerialArray->Array_Descriptor.Array_Data [Address_Subscript] = %f \n",
                    Array_Descriptor.SerialArray->Array_Descriptor.Array_Data [Address_Subscript]);
         }
#endif
#endif

#if COMPILE_DEBUG_STATEMENTS
//     Communication_Manager::Sync();
#endif
#endif
     }


#undef DOUBLEARRAY

#define FLOATARRAY
void
floatArray::Scalar_Indexing_For_floatArray_With_Message_Passing (
          int Address_Subscript, bool Off_Processor_With_Ghost_Boundaries,
          bool Off_Processor_Without_Ghost_Boundaries, int & Array_Index_For_References) const
   {
  // Send of receive messages as required for scalar indexing in parallel
  // Basically we go and get the data if it is on another processor OR we
  // broadcast the data if we own the data.  It is simple -- but expensive!
  // In the case of Optimization_Manager::Optimize_Scalar_Indexing == TRUE we can
  // skip this message passing and just return a reference to the data element
  // if we own it or a dummy variable if we don't.

#if defined(PPP)
  // This code is only important for P++.  It handles the message passing required to support
  // the scalar indexing (if it is left unoptimized).

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
        {
          printf ("Off_Processor_With_Ghost_Boundaries            = %s \n",(Off_Processor_With_Ghost_Boundaries)    ? "TRUE" : "FALSE" );
          printf ("Off_Processor_Without_Ghost_Boundaries         = %s \n",(Off_Processor_Without_Ghost_Boundaries) ? "TRUE" : "FALSE" );
          printf ("Optimization_Manager::Optimize_Scalar_Indexing = %s \n",(Optimization_Manager::Optimize_Scalar_Indexing) ? "TRUE" : "FALSE" );
        }
#endif

#if COMPILE_DEBUG_STATEMENTS
     Communication_Manager::Sync();
#endif

     MPI_Status status;
     int return_status                                         = MPI_SUCCESS;
     int GLOBAL_MPI_MESSAGE_TYPE_FOR_SCALAR_INDEXING_BROADCAST = 503;
     int My_Rank;
     float  store_to_check;
     float  temp_store;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("BEFORE: Static_float_Variable[%d] = %f \n",
                  Array_Index_For_References,(double)Static_float_Variable[Array_Index_For_References]);
          printf ("GLOBAL_MPI_MESSAGE_TYPE_FOR_SCALAR_INDEXING_BROADCAST = %d \n",
                  GLOBAL_MPI_MESSAGE_TYPE_FOR_SCALAR_INDEXING_BROADCAST);

          if (Off_Processor_With_Ghost_Boundaries)
               printf ("Value of array object (Array_ID=%d) at scalar INDEX (%d) IS OFF_PROCESSOR \n",
                       Array_Descriptor.Array_ID(),Address_Subscript);
            else
               printf ("Value of array object (Array_ID=%d) at scalar INDEX (%d) = %f \n",
                       Array_Descriptor.Array_ID(),Address_Subscript,
                       (double)Array_Descriptor.SerialArray->Array_Descriptor.
			  Array_Data [Address_Subscript]);
        }
#endif

  // figure out the node number using MPI calls

     return_status = MPI_Comm_rank(MPI_COMM_WORLD, &My_Rank);
     APP_ASSERT(return_status == MPI_SUCCESS);

  // We have to be able to reference many values simultaniously so we keep them in
  // an array and cycle through the array as a circular list.

     APP_ASSERT (Array_Index_For_References <= STATIC_LIMIT_FOR_PARALLEL_SCALAR_INDEXING_REFERENCES);
     if (Array_Index_For_References >= STATIC_LIMIT_FOR_PARALLEL_SCALAR_INDEXING_REFERENCES)
        {
#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 0)
               printf ("Resetting Array_Index_For_References to ZERO! \n");
#endif
          Array_Index_For_References = 0;
        }


     if ( (Off_Processor_Without_Ghost_Boundaries == FALSE) && (My_Rank != 0) )
        {
       // PARTI does not INCLUDE any broadcasts as part of it's message passing
       // so we write this explicitly using MPI calls

       // now we are on the processor that owns the data and the rank of that processor
       // is not zero so we have to send that data to processor 0 in order to bcast it
       // to all the others

          temp_store = Array_Descriptor.SerialArray->Array_Descriptor.
	     Array_Data[Address_Subscript];

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 0)
               printf ("ON PROCESSOR CASE (without ghost boundaries)! rank != 0 \n");
#endif
       // first we have to send the data to node 0 in order to make sure that
       // every node knows which node is the root-node during the broadcast
       // if we are already on node 0 we can proceed


#if defined(USE_PADRE)
          return_status = MPI_Send((void*)(&temp_store),
                                   1, MPI_FLOAT, 0, 
                                   GLOBAL_MPI_MESSAGE_TYPE_FOR_SCALAR_INDEXING_BROADCAST,
                                   PADRE::MPICommunicator());
#else
      // Since we are linking with the version of PARTI in the PADRE library
      // we want to use the Global_PARTI_PADRE_Interface_PADRE_Comm_World variable.
      //  return_status = MPI_Send((void*)(&temp_store),
      //                           1, MPI_FLOAT, 0, 
      //                           GLOBAL_MPI_MESSAGE_TYPE_FOR_SCALAR_INDEXING_BROADCAST,
      //                           Global_PARTI_P_plus_plus_Interface_PPP_Comm_World);
          return_status = MPI_Send((void*)(&temp_store),
                                   1, MPI_FLOAT, 0, 
                                   GLOBAL_MPI_MESSAGE_TYPE_FOR_SCALAR_INDEXING_BROADCAST,
                                   Global_PARTI_PADRE_Interface_PADRE_Comm_World);
#endif

          APP_ASSERT(return_status == MPI_SUCCESS);

       // store value that has been sent in order to check later whether the same value
       // got received

       // Static_Variable array hasn't been set yet so this is an error
          //store_to_check = Static_float_Variable[Array_Index_For_References];
          store_to_check = temp_store; 

        }

     if ( (Off_Processor_Without_Ghost_Boundaries == TRUE) && (My_Rank == 0) )
        {
#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 0)
               printf ("ON PROCESSOR CASE (without ghost boundaries)! rank == 0 \n");
#endif
       // on node 0 we have to receive that data 

          return_status = MPI_Recv((void*)(&temp_store),
                                   1, MPI_FLOAT, MPI_ANY_SOURCE, 
                                   GLOBAL_MPI_MESSAGE_TYPE_FOR_SCALAR_INDEXING_BROADCAST,
#if defined(USE_PADRE)
                                   PADRE::MPICommunicator(),
#else
                                // Since we are linking with the version of PARTI in the PADRE library
                                // we want to use the Global_PARTI_PADRE_Interface_PADRE_Comm_World variable.
                                // Global_PARTI_P_plus_plus_Interface_PPP_Comm_World,
                                   Global_PARTI_PADRE_Interface_PADRE_Comm_World,
#endif
                                   &status);

          APP_ASSERT(return_status == MPI_SUCCESS);
        }

     if ( (Off_Processor_Without_Ghost_Boundaries == FALSE) && (My_Rank == 0) )
       {
      // this is the case where processor zero actually has the data so we only need to
      // copy it the the right place in order to broadcast it

      // printf ("Assign to temp_store: Address_Subscript = %d \n",Address_Subscript);

	 temp_store = Array_Descriptor.SerialArray->Array_Descriptor.Array_Data[Address_Subscript];
       }
     
     // now node 0 has the data in temp_store and we can proceed with a broadcast
     // where root is node 0

     // MPI_Bcast already knows the type and so the number of values being
     // sent should be 1
        //return_status = MPI_Bcast((void*)(&temp_store),
        //                            sizeof( float ), MPI_FLOAT, 0, MPI_COMM_WORLD);
        return_status = MPI_Bcast((void*)(&temp_store),
                                    1, MPI_FLOAT, 0, MPI_COMM_WORLD);

        APP_ASSERT(return_status == MPI_SUCCESS);

     // now all the nodes should contain the same value in temp_store but oly the ones that
     // did not own the data in the first place have to copy it
     if (Off_Processor_Without_Ghost_Boundaries == TRUE)  
	 Static_float_Variable[Array_Index_For_References] = temp_store;

#if 0
#ifndef NDEBUG
     // test if the node that originally owned the data that was sent to node 0 in order 
     // to broadcast it receives the same value from the broadcast
     if ( (Off_Processor_Without_Ghost_Boundaries == FALSE) && (My_Rank != 0) )
       {
	 APP_ASSERT(Array_Descriptor.SerialArray->Array_Descriptor.
	    Array_Data[Address_Subscript] == store_to_check);
       }
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Off_Processor_Without_Ghost_Boundaries == FALSE )
         {
            APP_ASSERT(Array_Descriptor.SerialArray != NULL);
            APP_ASSERT(Array_Descriptor.SerialArray->Array_Descriptor.
	               Array_Data != NULL);
            if (APP_DEBUG > 0)
                 printf ("AFTER: Array_Descriptor.SerialArray->Array_Descriptor.Array_Data [Address_Subscript] = %f \n",
                    Array_Descriptor.SerialArray->Array_Descriptor.Array_Data [Address_Subscript]);
         }
#endif
#endif

#if COMPILE_DEBUG_STATEMENTS
//     Communication_Manager::Sync();
#endif
#endif
     }


#undef FLOATARRAY

#define INTARRAY
void
intArray::Scalar_Indexing_For_intArray_With_Message_Passing (
          int Address_Subscript, bool Off_Processor_With_Ghost_Boundaries,
          bool Off_Processor_Without_Ghost_Boundaries, int & Array_Index_For_References) const
   {
  // Send of receive messages as required for scalar indexing in parallel
  // Basically we go and get the data if it is on another processor OR we
  // broadcast the data if we own the data.  It is simple -- but expensive!
  // In the case of Optimization_Manager::Optimize_Scalar_Indexing == TRUE we can
  // skip this message passing and just return a reference to the data element
  // if we own it or a dummy variable if we don't.

#if defined(PPP)
  // This code is only important for P++.  It handles the message passing required to support
  // the scalar indexing (if it is left unoptimized).

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
        {
          printf ("Off_Processor_With_Ghost_Boundaries            = %s \n",(Off_Processor_With_Ghost_Boundaries)    ? "TRUE" : "FALSE" );
          printf ("Off_Processor_Without_Ghost_Boundaries         = %s \n",(Off_Processor_Without_Ghost_Boundaries) ? "TRUE" : "FALSE" );
          printf ("Optimization_Manager::Optimize_Scalar_Indexing = %s \n",(Optimization_Manager::Optimize_Scalar_Indexing) ? "TRUE" : "FALSE" );
        }
#endif

#if COMPILE_DEBUG_STATEMENTS
     Communication_Manager::Sync();
#endif

     MPI_Status status;
     int return_status                                         = MPI_SUCCESS;
     int GLOBAL_MPI_MESSAGE_TYPE_FOR_SCALAR_INDEXING_BROADCAST = 503;
     int My_Rank;
     int  store_to_check;
     int  temp_store;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("BEFORE: Static_int_Variable[%d] = %f \n",
                  Array_Index_For_References,(double)Static_int_Variable[Array_Index_For_References]);
          printf ("GLOBAL_MPI_MESSAGE_TYPE_FOR_SCALAR_INDEXING_BROADCAST = %d \n",
                  GLOBAL_MPI_MESSAGE_TYPE_FOR_SCALAR_INDEXING_BROADCAST);

          if (Off_Processor_With_Ghost_Boundaries)
               printf ("Value of array object (Array_ID=%d) at scalar INDEX (%d) IS OFF_PROCESSOR \n",
                       Array_Descriptor.Array_ID(),Address_Subscript);
            else
               printf ("Value of array object (Array_ID=%d) at scalar INDEX (%d) = %f \n",
                       Array_Descriptor.Array_ID(),Address_Subscript,
                       (double)Array_Descriptor.SerialArray->Array_Descriptor.
			  Array_Data [Address_Subscript]);
        }
#endif

  // figure out the node number using MPI calls

     return_status = MPI_Comm_rank(MPI_COMM_WORLD, &My_Rank);
     APP_ASSERT(return_status == MPI_SUCCESS);

  // We have to be able to reference many values simultaniously so we keep them in
  // an array and cycle through the array as a circular list.

     APP_ASSERT (Array_Index_For_References <= STATIC_LIMIT_FOR_PARALLEL_SCALAR_INDEXING_REFERENCES);
     if (Array_Index_For_References >= STATIC_LIMIT_FOR_PARALLEL_SCALAR_INDEXING_REFERENCES)
        {
#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 0)
               printf ("Resetting Array_Index_For_References to ZERO! \n");
#endif
          Array_Index_For_References = 0;
        }


     if ( (Off_Processor_Without_Ghost_Boundaries == FALSE) && (My_Rank != 0) )
        {
       // PARTI does not INCLUDE any broadcasts as part of it's message passing
       // so we write this explicitly using MPI calls

       // now we are on the processor that owns the data and the rank of that processor
       // is not zero so we have to send that data to processor 0 in order to bcast it
       // to all the others

          temp_store = Array_Descriptor.SerialArray->Array_Descriptor.
	     Array_Data[Address_Subscript];

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 0)
               printf ("ON PROCESSOR CASE (without ghost boundaries)! rank != 0 \n");
#endif
       // first we have to send the data to node 0 in order to make sure that
       // every node knows which node is the root-node during the broadcast
       // if we are already on node 0 we can proceed


#if defined(USE_PADRE)
          return_status = MPI_Send((void*)(&temp_store),
                                   1, MPI_INT, 0, 
                                   GLOBAL_MPI_MESSAGE_TYPE_FOR_SCALAR_INDEXING_BROADCAST,
                                   PADRE::MPICommunicator());
#else
      // Since we are linking with the version of PARTI in the PADRE library
      // we want to use the Global_PARTI_PADRE_Interface_PADRE_Comm_World variable.
      //  return_status = MPI_Send((void*)(&temp_store),
      //                           1, MPI_INT, 0, 
      //                           GLOBAL_MPI_MESSAGE_TYPE_FOR_SCALAR_INDEXING_BROADCAST,
      //                           Global_PARTI_P_plus_plus_Interface_PPP_Comm_World);
          return_status = MPI_Send((void*)(&temp_store),
                                   1, MPI_INT, 0, 
                                   GLOBAL_MPI_MESSAGE_TYPE_FOR_SCALAR_INDEXING_BROADCAST,
                                   Global_PARTI_PADRE_Interface_PADRE_Comm_World);
#endif

          APP_ASSERT(return_status == MPI_SUCCESS);

       // store value that has been sent in order to check later whether the same value
       // got received

       // Static_Variable array hasn't been set yet so this is an error
          //store_to_check = Static_int_Variable[Array_Index_For_References];
          store_to_check = temp_store; 

        }

     if ( (Off_Processor_Without_Ghost_Boundaries == TRUE) && (My_Rank == 0) )
        {
#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 0)
               printf ("ON PROCESSOR CASE (without ghost boundaries)! rank == 0 \n");
#endif
       // on node 0 we have to receive that data 

          return_status = MPI_Recv((void*)(&temp_store),
                                   1, MPI_INT, MPI_ANY_SOURCE, 
                                   GLOBAL_MPI_MESSAGE_TYPE_FOR_SCALAR_INDEXING_BROADCAST,
#if defined(USE_PADRE)
                                   PADRE::MPICommunicator(),
#else
                                // Since we are linking with the version of PARTI in the PADRE library
                                // we want to use the Global_PARTI_PADRE_Interface_PADRE_Comm_World variable.
                                // Global_PARTI_P_plus_plus_Interface_PPP_Comm_World,
                                   Global_PARTI_PADRE_Interface_PADRE_Comm_World,
#endif
                                   &status);

          APP_ASSERT(return_status == MPI_SUCCESS);
        }

     if ( (Off_Processor_Without_Ghost_Boundaries == FALSE) && (My_Rank == 0) )
       {
      // this is the case where processor zero actually has the data so we only need to
      // copy it the the right place in order to broadcast it

      // printf ("Assign to temp_store: Address_Subscript = %d \n",Address_Subscript);

	 temp_store = Array_Descriptor.SerialArray->Array_Descriptor.Array_Data[Address_Subscript];
       }
     
     // now node 0 has the data in temp_store and we can proceed with a broadcast
     // where root is node 0

     // MPI_Bcast already knows the type and so the number of values being
     // sent should be 1
        //return_status = MPI_Bcast((void*)(&temp_store),
        //                            sizeof( int ), MPI_INT, 0, MPI_COMM_WORLD);
        return_status = MPI_Bcast((void*)(&temp_store),
                                    1, MPI_INT, 0, MPI_COMM_WORLD);

        APP_ASSERT(return_status == MPI_SUCCESS);

     // now all the nodes should contain the same value in temp_store but oly the ones that
     // did not own the data in the first place have to copy it
     if (Off_Processor_Without_Ghost_Boundaries == TRUE)  
	 Static_int_Variable[Array_Index_For_References] = temp_store;

#if 0
#ifndef NDEBUG
     // test if the node that originally owned the data that was sent to node 0 in order 
     // to broadcast it receives the same value from the broadcast
     if ( (Off_Processor_Without_Ghost_Boundaries == FALSE) && (My_Rank != 0) )
       {
	 APP_ASSERT(Array_Descriptor.SerialArray->Array_Descriptor.
	    Array_Data[Address_Subscript] == store_to_check);
       }
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Off_Processor_Without_Ghost_Boundaries == FALSE )
         {
            APP_ASSERT(Array_Descriptor.SerialArray != NULL);
            APP_ASSERT(Array_Descriptor.SerialArray->Array_Descriptor.
	               Array_Data != NULL);
            if (APP_DEBUG > 0)
                 printf ("AFTER: Array_Descriptor.SerialArray->Array_Descriptor.Array_Data [Address_Subscript] = %f \n",
                    Array_Descriptor.SerialArray->Array_Descriptor.Array_Data [Address_Subscript]);
         }
#endif
#endif

#if COMPILE_DEBUG_STATEMENTS
//     Communication_Manager::Sync();
#endif
#endif
     }


#undef INTARRAY

#define DOUBLEARRAY
#if !defined(PXX_ENABLE_MP_INTERFACE_MPI)
#error PXX_ENABLE_MP_INTERFACE_MPI macro should be specified
#endif

// ************************************************************************************
// This is the reduction operator which separates P++ from specific MPI calls
// ************************************************************************************

void
Reduction_Operation ( int Input_Function_Code , double &x )
   {
  // This code represents MPI specific functions 

  // There would seem to be no other alternative since PARTI does not handle reductions 
  // and we need to have P++ be MPI independent

     int return_status;
     double Receive_Buffer;

  // We use the Input_Function_Code as a key to specify the MPI function that will be used
  // with the MPI_Allreduce function.  The Input_Function_Code is just the operation_type passed into
  // the abstract operator by the sum max and min functions.
     
     MPI_Op Input_Function;
#if 0
     extern MPI_Op MPI_SUM;
     extern MPI_Op MPI_MAX;
     extern MPI_Op MPI_MIN;
#endif

     if (Input_Function_Code == doubleArray::sum_Function)
      {
        Input_Function = MPI_SUM;
      }
     else if (Input_Function_Code == doubleArray::max_Function)
      {
        Input_Function = MPI_MAX;
      }
     else if (Input_Function_Code == doubleArray::min_Function)
      {
        Input_Function = MPI_MIN;
      }
     else 
      {
        printf ("ERROR: default reached in Reduction_Operation! \n");
        APP_ABORT();
      }

#if defined(DOUBLEARRAY)
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of Reduction_Operation INPUT --- x = %f \n",x);
#endif

     return_status = MPI_Allreduce((void*)(&x), (void*)(&Receive_Buffer), 1, MPI_DOUBLE,
                                   Input_Function, MPI_COMM_WORLD);

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of Reduction_Operation after MPI_Allreduce -- Receive_Buffer = %f  return_status = %d\n",
                   Receive_Buffer,return_status);
#endif
#endif
#if defined(FLOATARRAY)
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of Reduction_Operation INPUT --- x = %f \n",x);
#endif

     return_status = MPI_Allreduce((void*)(&x), (void*)(&Receive_Buffer), 1, MPI_FLOAT,
                                   Input_Function, MPI_COMM_WORLD);

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of Reduction_Operation after MPI_Allreduce -- Receive_Buffer = %f  return_status = %d \n",
                   Receive_Buffer,return_status);
#endif
#endif
#if defined(INTARRAY)
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of Reduction_Operation INPUT --- x = %d \n",x);
#endif

     return_status = MPI_Allreduce((void*)(&x), (void*)(&Receive_Buffer), 1, MPI_INT,
                                   Input_Function, MPI_COMM_WORLD);

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
       printf ("Inside of Reduction_Operation after MPI_Allreduce -- Receive_Buffer = %d  Error_Code = %d \n",
                Receive_Buffer,return_status);
#endif
#endif

     APP_ASSERT(return_status  == MPI_SUCCESS);

  // the result is expected to be in x
     x = Receive_Buffer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
#if defined(INTARRAY)
          printf ("Leaving Reduction_Operation OUTPUT -- x = %d  \n",x);
#else
          printf ("Leaving Reduction_Operation OUTPUT -- x = %f  \n",x);
#endif
#endif
   }

#if 1
void
Communication_Manager::fillProcessorArray ( double* processorArray, double value )
   {
  // We assume that the processor array is a buffer of length equal to the number of processors
  // then we recieve from each processor data that is placed into this array using the processor number as the index
     int GLOBAL_MPI_MESSAGE_TYPE_FOR_FILL_PROCESSOR_ARRAY_BROADCAST = 999;
     MPI_Status status;
     int return_status = MPI_SUCCESS;

     int numberOfNodes = numberOfProcessors();

     int My_Rank = 0;
     return_status = MPI_Comm_rank(MPI_COMM_WORLD, &My_Rank);
     APP_ASSERT(return_status == MPI_SUCCESS);

     if ( My_Rank == 0 )
        {
       // Processor 0 recieves all the data
          processorArray[0] = value;
          int i;
          for (i=1; i < numberOfNodes; i++)
             {
               return_status = MPI_Recv((void*)(&(processorArray[i])),
                                   1, MPI_DOUBLE, i,
                                   GLOBAL_MPI_MESSAGE_TYPE_FOR_FILL_PROCESSOR_ARRAY_BROADCAST,
#if defined(USE_PADRE)
                                   PADRE::MPICommunicator(),
#else
			           Global_PARTI_PADRE_Interface_PADRE_Comm_World,
#endif
                                   &status);
               APP_ASSERT(return_status == MPI_SUCCESS);
	     }
        }
       else
        {
          return_status = MPI_Send((void*)(&value),
                                   1, MPI_DOUBLE, 0,
                                   GLOBAL_MPI_MESSAGE_TYPE_FOR_FILL_PROCESSOR_ARRAY_BROADCAST,
#if defined(USE_PADRE)
                                   PADRE::MPICommunicator()
#else
				   Global_PARTI_PADRE_Interface_PADRE_Comm_World
#endif
	    );
	  APP_ASSERT(return_status == MPI_SUCCESS);
        }

   }
#endif


#undef DOUBLEARRAY

#define FLOATARRAY
#if !defined(PXX_ENABLE_MP_INTERFACE_MPI)
#error PXX_ENABLE_MP_INTERFACE_MPI macro should be specified
#endif

// ************************************************************************************
// This is the reduction operator which separates P++ from specific MPI calls
// ************************************************************************************

void
Reduction_Operation ( int Input_Function_Code , float &x )
   {
  // This code represents MPI specific functions 

  // There would seem to be no other alternative since PARTI does not handle reductions 
  // and we need to have P++ be MPI independent

     int return_status;
     float Receive_Buffer;

  // We use the Input_Function_Code as a key to specify the MPI function that will be used
  // with the MPI_Allreduce function.  The Input_Function_Code is just the operation_type passed into
  // the abstract operator by the sum max and min functions.
     
     MPI_Op Input_Function;
#if 0
     extern MPI_Op MPI_SUM;
     extern MPI_Op MPI_MAX;
     extern MPI_Op MPI_MIN;
#endif

     if (Input_Function_Code == doubleArray::sum_Function)
      {
        Input_Function = MPI_SUM;
      }
     else if (Input_Function_Code == doubleArray::max_Function)
      {
        Input_Function = MPI_MAX;
      }
     else if (Input_Function_Code == doubleArray::min_Function)
      {
        Input_Function = MPI_MIN;
      }
     else 
      {
        printf ("ERROR: default reached in Reduction_Operation! \n");
        APP_ABORT();
      }

#if defined(DOUBLEARRAY)
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of Reduction_Operation INPUT --- x = %f \n",x);
#endif

     return_status = MPI_Allreduce((void*)(&x), (void*)(&Receive_Buffer), 1, MPI_DOUBLE,
                                   Input_Function, MPI_COMM_WORLD);

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of Reduction_Operation after MPI_Allreduce -- Receive_Buffer = %f  return_status = %d\n",
                   Receive_Buffer,return_status);
#endif
#endif
#if defined(FLOATARRAY)
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of Reduction_Operation INPUT --- x = %f \n",x);
#endif

     return_status = MPI_Allreduce((void*)(&x), (void*)(&Receive_Buffer), 1, MPI_FLOAT,
                                   Input_Function, MPI_COMM_WORLD);

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of Reduction_Operation after MPI_Allreduce -- Receive_Buffer = %f  return_status = %d \n",
                   Receive_Buffer,return_status);
#endif
#endif
#if defined(INTARRAY)
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of Reduction_Operation INPUT --- x = %d \n",x);
#endif

     return_status = MPI_Allreduce((void*)(&x), (void*)(&Receive_Buffer), 1, MPI_INT,
                                   Input_Function, MPI_COMM_WORLD);

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
       printf ("Inside of Reduction_Operation after MPI_Allreduce -- Receive_Buffer = %d  Error_Code = %d \n",
                Receive_Buffer,return_status);
#endif
#endif

     APP_ASSERT(return_status  == MPI_SUCCESS);

  // the result is expected to be in x
     x = Receive_Buffer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
#if defined(INTARRAY)
          printf ("Leaving Reduction_Operation OUTPUT -- x = %d  \n",x);
#else
          printf ("Leaving Reduction_Operation OUTPUT -- x = %f  \n",x);
#endif
#endif
   }

#if 1
void
Communication_Manager::fillProcessorArray ( float* processorArray, float value )
   {
  // We assume that the processor array is a buffer of length equal to the number of processors
  // then we recieve from each processor data that is placed into this array using the processor number as the index
     int GLOBAL_MPI_MESSAGE_TYPE_FOR_FILL_PROCESSOR_ARRAY_BROADCAST = 999;
     MPI_Status status;
     int return_status = MPI_SUCCESS;

     int numberOfNodes = numberOfProcessors();

     int My_Rank = 0;
     return_status = MPI_Comm_rank(MPI_COMM_WORLD, &My_Rank);
     APP_ASSERT(return_status == MPI_SUCCESS);

     if ( My_Rank == 0 )
        {
       // Processor 0 recieves all the data
          processorArray[0] = value;
          int i;
          for (i=1; i < numberOfNodes; i++)
             {
               return_status = MPI_Recv((void*)(&(processorArray[i])),
                                   1, MPI_FLOAT, i,
                                   GLOBAL_MPI_MESSAGE_TYPE_FOR_FILL_PROCESSOR_ARRAY_BROADCAST,
#if defined(USE_PADRE)
                                   PADRE::MPICommunicator(),
#else
			           Global_PARTI_PADRE_Interface_PADRE_Comm_World,
#endif
                                   &status);
               APP_ASSERT(return_status == MPI_SUCCESS);
	     }
        }
       else
        {
          return_status = MPI_Send((void*)(&value),
                                   1, MPI_FLOAT, 0,
                                   GLOBAL_MPI_MESSAGE_TYPE_FOR_FILL_PROCESSOR_ARRAY_BROADCAST,
#if defined(USE_PADRE)
                                   PADRE::MPICommunicator()
#else
				   Global_PARTI_PADRE_Interface_PADRE_Comm_World
#endif
	    );
	  APP_ASSERT(return_status == MPI_SUCCESS);
        }

   }
#endif


#undef FLOATARRAY

#define INTARRAY
#if !defined(PXX_ENABLE_MP_INTERFACE_MPI)
#error PXX_ENABLE_MP_INTERFACE_MPI macro should be specified
#endif

// ************************************************************************************
// This is the reduction operator which separates P++ from specific MPI calls
// ************************************************************************************

void
Reduction_Operation ( int Input_Function_Code , int &x )
   {
  // This code represents MPI specific functions 

  // There would seem to be no other alternative since PARTI does not handle reductions 
  // and we need to have P++ be MPI independent

     int return_status;
     int Receive_Buffer;

  // We use the Input_Function_Code as a key to specify the MPI function that will be used
  // with the MPI_Allreduce function.  The Input_Function_Code is just the operation_type passed into
  // the abstract operator by the sum max and min functions.
     
     MPI_Op Input_Function;
#if 0
     extern MPI_Op MPI_SUM;
     extern MPI_Op MPI_MAX;
     extern MPI_Op MPI_MIN;
#endif

     if (Input_Function_Code == doubleArray::sum_Function)
      {
        Input_Function = MPI_SUM;
      }
     else if (Input_Function_Code == doubleArray::max_Function)
      {
        Input_Function = MPI_MAX;
      }
     else if (Input_Function_Code == doubleArray::min_Function)
      {
        Input_Function = MPI_MIN;
      }
     else 
      {
        printf ("ERROR: default reached in Reduction_Operation! \n");
        APP_ABORT();
      }

#if defined(DOUBLEARRAY)
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of Reduction_Operation INPUT --- x = %f \n",x);
#endif

     return_status = MPI_Allreduce((void*)(&x), (void*)(&Receive_Buffer), 1, MPI_DOUBLE,
                                   Input_Function, MPI_COMM_WORLD);

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of Reduction_Operation after MPI_Allreduce -- Receive_Buffer = %f  return_status = %d\n",
                   Receive_Buffer,return_status);
#endif
#endif
#if defined(FLOATARRAY)
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of Reduction_Operation INPUT --- x = %f \n",x);
#endif

     return_status = MPI_Allreduce((void*)(&x), (void*)(&Receive_Buffer), 1, MPI_FLOAT,
                                   Input_Function, MPI_COMM_WORLD);

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of Reduction_Operation after MPI_Allreduce -- Receive_Buffer = %f  return_status = %d \n",
                   Receive_Buffer,return_status);
#endif
#endif
#if defined(INTARRAY)
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of Reduction_Operation INPUT --- x = %d \n",x);
#endif

     return_status = MPI_Allreduce((void*)(&x), (void*)(&Receive_Buffer), 1, MPI_INT,
                                   Input_Function, MPI_COMM_WORLD);

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
       printf ("Inside of Reduction_Operation after MPI_Allreduce -- Receive_Buffer = %d  Error_Code = %d \n",
                Receive_Buffer,return_status);
#endif
#endif

     APP_ASSERT(return_status  == MPI_SUCCESS);

  // the result is expected to be in x
     x = Receive_Buffer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
#if defined(INTARRAY)
          printf ("Leaving Reduction_Operation OUTPUT -- x = %d  \n",x);
#else
          printf ("Leaving Reduction_Operation OUTPUT -- x = %f  \n",x);
#endif
#endif
   }

#if 1
void
Communication_Manager::fillProcessorArray ( int* processorArray, int value )
   {
  // We assume that the processor array is a buffer of length equal to the number of processors
  // then we recieve from each processor data that is placed into this array using the processor number as the index
     int GLOBAL_MPI_MESSAGE_TYPE_FOR_FILL_PROCESSOR_ARRAY_BROADCAST = 999;
     MPI_Status status;
     int return_status = MPI_SUCCESS;

     int numberOfNodes = numberOfProcessors();

     int My_Rank = 0;
     return_status = MPI_Comm_rank(MPI_COMM_WORLD, &My_Rank);
     APP_ASSERT(return_status == MPI_SUCCESS);

     if ( My_Rank == 0 )
        {
       // Processor 0 recieves all the data
          processorArray[0] = value;
          int i;
          for (i=1; i < numberOfNodes; i++)
             {
               return_status = MPI_Recv((void*)(&(processorArray[i])),
                                   1, MPI_INT, i,
                                   GLOBAL_MPI_MESSAGE_TYPE_FOR_FILL_PROCESSOR_ARRAY_BROADCAST,
#if defined(USE_PADRE)
                                   PADRE::MPICommunicator(),
#else
			           Global_PARTI_PADRE_Interface_PADRE_Comm_World,
#endif
                                   &status);
               APP_ASSERT(return_status == MPI_SUCCESS);
	     }
        }
       else
        {
          return_status = MPI_Send((void*)(&value),
                                   1, MPI_INT, 0,
                                   GLOBAL_MPI_MESSAGE_TYPE_FOR_FILL_PROCESSOR_ARRAY_BROADCAST,
#if defined(USE_PADRE)
                                   PADRE::MPICommunicator()
#else
				   Global_PARTI_PADRE_Interface_PADRE_Comm_World
#endif
	    );
	  APP_ASSERT(return_status == MPI_SUCCESS);
        }

   }
#endif


#undef INTARRAY

#if !defined(PXX_ENABLE_MP_INTERFACE_MPI)
#error PXX_ENABLE_MP_INTERFACE_MPI macro should be specified
#endif

#if 0
#if defined(USE_PADRE)
// void Communication_Manager::MPICommWorld( MPI_Comm *interCommunicator )
void
MPICommWorld( MPI_Comm *interCommunicator )
   {
     MPI_Comm_dup( MPI_COMM_WORLD, interCommunicator );
   }
#endif
#endif

void
Communication_Manager::Start_Parallel_Machine (
     char* Application_Program_Name , int & Input_Number_Of_Processors, int & argc, char** & argv)
       {
      // APP_DEBUG = 3;
#if COMPILE_DEBUG_STATEMENTS
	 if (APP_DEBUG > -1)
	      printf("Inside Communication_Manager::Start_Parallel_Machine (MPI version) (APP_DEBUG = %d) \n",APP_DEBUG);
#endif
	 int return_status;
	 int flag;
	 
      // check whether MPI_Init has already been called
      // might not be neccessary
	 
	 return_status = MPI_Initialized (&flag);
	 APP_ASSERT(flag == FALSE); 
	 
      // argc and argv have to be declared in the main program
      // they are the parameters to main()

      // printf ("Calling MPI_Init() \n");
	 return_status = MPI_Init(&argc, &argv);
	 APP_ASSERT(return_status == MPI_SUCCESS);

      // printf ("Calling MPI_Comm_size \n");
      // determine size of parallel machine
	 int Total_Number_Of_Processors;
	 return_status = MPI_Comm_size(MPI_COMM_WORLD, &Total_Number_Of_Processors);
	 APP_ASSERT(return_status == MPI_SUCCESS);

#if 0
	 if (Total_Number_Of_Processors != Input_Number_Of_Processors)
	   {
	     printf("WARNING: Processors available: %d,  Processors requested: %d\n",
		    Total_Number_Of_Processors, Input_Number_Of_Processors);
             printf("WARNING: running on %d processors\n", Total_Number_Of_Processors);

             printf ("ERROR: Number of Processors requestion inconsistant with number of processors available! \n");
             APP_ABORT();
	   }
#else
       // Assign the number of processors to Input_Number_Of_Processors 
       // so that it will be returned properly
          Input_Number_Of_Processors = Total_Number_Of_Processors;
#endif

      // printf ("Calling MPI_Comm_rank \n");
      // determine my process number
         return_status = MPI_Comm_rank(MPI_COMM_WORLD, &My_Process_Number);
         APP_ASSERT(return_status == MPI_SUCCESS);
 
      // printf("P++ runs on %d processors (APP_DEBUG = %d) \n",Total_Number_Of_Processors,APP_DEBUG);

      // wait for everyone to start up
	 return_status = MPI_Barrier(MPI_COMM_WORLD);
	 APP_ASSERT(return_status == MPI_SUCCESS);

	 Number_Of_Processors = Total_Number_Of_Processors;

      // printf ("Setup MPI_COMM_WORLD \n");
      // initialize Global_PARTI_PADRE_Interface_PADRE_Comm_World for use with PARTI
#if defined(USE_PADRE)
         // MPI_Comm_dup(MPI_COMM_WORLD, &Global_PARTI_PADRE_Interface_PADRE_Comm_World);
      // PADRE::Initialization::initialize( 
      //      &Communication_Manager::MPICommWorld,
      //       Communication_Manager::numberOfProcessors() );
      PADRE::Initialization::initialize( MPI_COMM_WORLD, Number_Of_Processors );
#else
      // Since we are linking with the version of PARTI in the PADRE library
      // we want to use the Global_PARTI_PADRE_Interface_PADRE_Comm_World variable.
      // MPI_Comm_dup(MPI_COMM_WORLD, &Global_PARTI_P_plus_plus_Interface_PPP_Comm_World);
         MPI_Comm_dup(MPI_COMM_WORLD, &Global_PARTI_PADRE_Interface_PADRE_Comm_World);
#endif

#if 0
// When we use MPI the local directory is already set properly.
// so we don't have to do anything
#if defined(AUTO_INITIALIZE_APPLICATION_PATH_NAME)
      // Now change the current directory on all processes to the same directory as the executable.
      // This way any file I/O in the current directory works on all processors.

      // First do a broadcast of the pathname from processor 0 to the other processors
         const int MAXIMUM_PATH_LENGTH = 500;
         char pathName[MAXIMUM_PATH_LENGTH];
         int My_Rank = 0;
         return_status = MPI_Comm_rank(MPI_COMM_WORLD, &My_Rank);
         APP_ASSERT(return_status == MPI_SUCCESS);
         int GLOBAL_MPI_MESSAGE_TYPE_FOR_PATH_BROADCAST = 504;
         MPI_Status status;
         printf ("Test 5 \n");
         if ( My_Rank == 0 )
            {
              getcwd(pathName,MAXIMUM_PATH_LENGTH);
              printf ("On processor %d START broadcast \n",My_Rank);
           // return_status = MPI_Bcast((void*)(pathName), MAXIMUM_PATH_LENGTH, MPI_CHAR, 0, MPI_COMM_WORLD );
              APP_ASSERT(return_status == MPI_SUCCESS);
              printf ("On processor %d DONE broadcast \n",My_Rank);
            }
           else
            {
              printf ("On processor %d START recieve \n",My_Rank);
           // return_status = MPI_Recv((void*)(pathName), MAXIMUM_PATH_LENGTH, MPI_CHAR, MPI_ANY_SOURCE, 
           //                      GLOBAL_MPI_MESSAGE_TYPE_FOR_PATH_BROADCAST,
           //                      Global_PARTI_PADRE_Interface_PADRE_Comm_World,
           //                      &status);
              APP_ASSERT(return_status == MPI_SUCCESS);
              printf ("On processor %d DONE recieve \n",My_Rank);

           // Now change the directory of all but the zeroth process
              chdir(pathName);
            }
     
         cout << "Changed local directory to match Processor directory on processor " << My_Rank << " : " << getcwd(NULL, 10000) << endl;
         Communication_Manager::Sync();
         cout << "Processing rest of application now!" << endl;
#endif
#endif
      // cout << "Local directory on processor " << My_Process_Number << " : " << getcwd(NULL, 10000) << endl;
       }


#if !defined(PXX_ENABLE_MP_INTERFACE_MPI)
#error PXX_ENABLE_MP_INTERFACE_MPI macro should be specified
#endif

void Communication_Manager::Stop_Parallel_Machine()
       {
#if COMPILE_DEBUG_STATEMENTS
	 if (APP_DEBUG > 0)
	   printf("Inside Communication_Manager::Stop_Parallel_Machine  (MPI version)\n");
#endif

	 int return_status;
	 int error_code;
	 
	 // wait for everyone to end their calculations
	 return_status = MPI_Barrier(MPI_COMM_WORLD);
	 APP_ASSERT(return_status == MPI_SUCCESS);

      // abort the parallel machine
	 return_status = MPI_Finalize();
	 APP_ASSERT(return_status == MPI_SUCCESS);
       }


#if !defined(PXX_ENABLE_MP_INTERFACE_MPI)
#error PXX_ENABLE_MP_INTERFACE_MPI macro should be specified
#endif

void Communication_Manager::Sync()
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf("Inside Communication_Manager::Sync  (MPI version)\n");
#endif
  // Finish writing out all the I/O before the sync (this simplifies debugging)
     fflush(stdout);
     fflush(stderr);

  // This is the check for if MPI has been initialied using the
  // Communication_Manager::Start_Parallel_Machine member function.
  // We would expect this function to be called first but in the case of static
  // P++ arrays some MPI functions can be called before the main program
  // is executed.  No meaningful P++ operations can take place before
  // the Parallel machine is initialized but we can allow Sync to be called
  // and to have no effect.
     if (IsParallelMachine_Initialized() == TRUE)
        {
          int return_code = MPI_Barrier(MPI_COMM_WORLD);
          APP_ASSERT(return_code == MPI_SUCCESS);
        }
   }


#if !defined(PXX_ENABLE_MP_INTERFACE_MPI)
#error PXX_ENABLE_MP_INTERFACE_MPI macro should be specified
#endif

double Communication_Manager::Wall_Clock_Time()
       {
	 return MPI_Wtime();
       }





















