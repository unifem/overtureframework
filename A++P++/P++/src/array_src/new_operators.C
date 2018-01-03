#define COMPILE_SERIAL_APP

#include "A++.h"

















int  doubleSerialArray::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

doubleSerialArray* doubleSerialArray::Current_Link                      = NULL;

int doubleSerialArray::Memory_Block_Index                = 0;

const int doubleSerialArray::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *doubleSerialArray::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *doubleSerialArray::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call doubleSerialArray::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
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

     if (Size != sizeof(doubleSerialArray))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from doubleSerialArray
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In doubleSerialArray::operator new: Calling malloc because Size(%d) != sizeof(doubleSerialArray)(%d) \n",Size,sizeof(doubleSerialArray));

          return malloc(Size);
        }
       else
        {
       // printf ("In doubleSerialArray::operator new: Using the pool mechanism Size(%d) == sizeof(doubleSerialArray)(%d) \n",Size,sizeof(doubleSerialArray));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (doubleSerialArray*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(doubleSerialArray) );
#else
               Current_Link = (doubleSerialArray*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(doubleSerialArray) ];
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

     doubleSerialArray* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from doubleSerialArray::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void doubleSerialArray::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In doubleSerialArray::operator delete: Size(%d)  sizeof(doubleSerialArray)(%d) \n",sizeOfObject,sizeof(doubleSerialArray));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(doubleSerialArray))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from doubleSerialArray
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In doubleSerialArray::operator delete: Calling global delete (free) because Size(%d) != sizeof(doubleSerialArray)(%d) \n",sizeOfObject,sizeof(doubleSerialArray));
             }
#endif

          free(Pointer);
        }
       else
        {
          doubleSerialArray *New_Link = (doubleSerialArray*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In doubleSerialArray::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(doubleSerialArray)(%d) \n",Pointer,sizeOfObject,sizeof(doubleSerialArray));
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
               printf ("ERROR: In doubleSerialArray::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving doubleSerialArray::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }


int  doubleSerialArray_Function_Steal_Data::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

doubleSerialArray_Function_Steal_Data* doubleSerialArray_Function_Steal_Data::Current_Link                      = NULL;

int doubleSerialArray_Function_Steal_Data::Memory_Block_Index                = 0;

const int doubleSerialArray_Function_Steal_Data::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *doubleSerialArray_Function_Steal_Data::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *doubleSerialArray_Function_Steal_Data::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call doubleSerialArray_Function_Steal_Data::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
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

     if (Size != sizeof(doubleSerialArray_Function_Steal_Data))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from doubleSerialArray_Function_Steal_Data
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In doubleSerialArray_Function_Steal_Data::operator new: Calling malloc because Size(%d) != sizeof(doubleSerialArray_Function_Steal_Data)(%d) \n",Size,sizeof(doubleSerialArray_Function_Steal_Data));

          return malloc(Size);
        }
       else
        {
       // printf ("In doubleSerialArray_Function_Steal_Data::operator new: Using the pool mechanism Size(%d) == sizeof(doubleSerialArray_Function_Steal_Data)(%d) \n",Size,sizeof(doubleSerialArray_Function_Steal_Data));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (doubleSerialArray_Function_Steal_Data*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(doubleSerialArray_Function_Steal_Data) );
#else
               Current_Link = (doubleSerialArray_Function_Steal_Data*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(doubleSerialArray_Function_Steal_Data) ];
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

     doubleSerialArray_Function_Steal_Data* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from doubleSerialArray_Function_Steal_Data::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void doubleSerialArray_Function_Steal_Data::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In doubleSerialArray_Function_Steal_Data::operator delete: Size(%d)  sizeof(doubleSerialArray_Function_Steal_Data)(%d) \n",sizeOfObject,sizeof(doubleSerialArray_Function_Steal_Data));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(doubleSerialArray_Function_Steal_Data))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from doubleSerialArray_Function_Steal_Data
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In doubleSerialArray_Function_Steal_Data::operator delete: Calling global delete (free) because Size(%d) != sizeof(doubleSerialArray_Function_Steal_Data)(%d) \n",sizeOfObject,sizeof(doubleSerialArray_Function_Steal_Data));
             }
#endif

          free(Pointer);
        }
       else
        {
          doubleSerialArray_Function_Steal_Data *New_Link = (doubleSerialArray_Function_Steal_Data*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In doubleSerialArray_Function_Steal_Data::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(doubleSerialArray_Function_Steal_Data)(%d) \n",Pointer,sizeOfObject,sizeof(doubleSerialArray_Function_Steal_Data));
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
               printf ("ERROR: In doubleSerialArray_Function_Steal_Data::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving doubleSerialArray_Function_Steal_Data::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }


int  doubleSerialArray_Function_0::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

doubleSerialArray_Function_0* doubleSerialArray_Function_0::Current_Link                      = NULL;

int doubleSerialArray_Function_0::Memory_Block_Index                = 0;

const int doubleSerialArray_Function_0::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *doubleSerialArray_Function_0::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *doubleSerialArray_Function_0::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call doubleSerialArray_Function_0::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
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

     if (Size != sizeof(doubleSerialArray_Function_0))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from doubleSerialArray_Function_0
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In doubleSerialArray_Function_0::operator new: Calling malloc because Size(%d) != sizeof(doubleSerialArray_Function_0)(%d) \n",Size,sizeof(doubleSerialArray_Function_0));

          return malloc(Size);
        }
       else
        {
       // printf ("In doubleSerialArray_Function_0::operator new: Using the pool mechanism Size(%d) == sizeof(doubleSerialArray_Function_0)(%d) \n",Size,sizeof(doubleSerialArray_Function_0));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (doubleSerialArray_Function_0*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(doubleSerialArray_Function_0) );
#else
               Current_Link = (doubleSerialArray_Function_0*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(doubleSerialArray_Function_0) ];
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

     doubleSerialArray_Function_0* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from doubleSerialArray_Function_0::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void doubleSerialArray_Function_0::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In doubleSerialArray_Function_0::operator delete: Size(%d)  sizeof(doubleSerialArray_Function_0)(%d) \n",sizeOfObject,sizeof(doubleSerialArray_Function_0));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(doubleSerialArray_Function_0))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from doubleSerialArray_Function_0
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In doubleSerialArray_Function_0::operator delete: Calling global delete (free) because Size(%d) != sizeof(doubleSerialArray_Function_0)(%d) \n",sizeOfObject,sizeof(doubleSerialArray_Function_0));
             }
#endif

          free(Pointer);
        }
       else
        {
          doubleSerialArray_Function_0 *New_Link = (doubleSerialArray_Function_0*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In doubleSerialArray_Function_0::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(doubleSerialArray_Function_0)(%d) \n",Pointer,sizeOfObject,sizeof(doubleSerialArray_Function_0));
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
               printf ("ERROR: In doubleSerialArray_Function_0::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving doubleSerialArray_Function_0::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }


int  doubleSerialArray_Function_1::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

doubleSerialArray_Function_1* doubleSerialArray_Function_1::Current_Link                      = NULL;

int doubleSerialArray_Function_1::Memory_Block_Index                = 0;

const int doubleSerialArray_Function_1::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *doubleSerialArray_Function_1::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *doubleSerialArray_Function_1::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call doubleSerialArray_Function_1::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
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

     if (Size != sizeof(doubleSerialArray_Function_1))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from doubleSerialArray_Function_1
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In doubleSerialArray_Function_1::operator new: Calling malloc because Size(%d) != sizeof(doubleSerialArray_Function_1)(%d) \n",Size,sizeof(doubleSerialArray_Function_1));

          return malloc(Size);
        }
       else
        {
       // printf ("In doubleSerialArray_Function_1::operator new: Using the pool mechanism Size(%d) == sizeof(doubleSerialArray_Function_1)(%d) \n",Size,sizeof(doubleSerialArray_Function_1));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (doubleSerialArray_Function_1*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(doubleSerialArray_Function_1) );
#else
               Current_Link = (doubleSerialArray_Function_1*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(doubleSerialArray_Function_1) ];
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

     doubleSerialArray_Function_1* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from doubleSerialArray_Function_1::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void doubleSerialArray_Function_1::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In doubleSerialArray_Function_1::operator delete: Size(%d)  sizeof(doubleSerialArray_Function_1)(%d) \n",sizeOfObject,sizeof(doubleSerialArray_Function_1));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(doubleSerialArray_Function_1))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from doubleSerialArray_Function_1
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In doubleSerialArray_Function_1::operator delete: Calling global delete (free) because Size(%d) != sizeof(doubleSerialArray_Function_1)(%d) \n",sizeOfObject,sizeof(doubleSerialArray_Function_1));
             }
#endif

          free(Pointer);
        }
       else
        {
          doubleSerialArray_Function_1 *New_Link = (doubleSerialArray_Function_1*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In doubleSerialArray_Function_1::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(doubleSerialArray_Function_1)(%d) \n",Pointer,sizeOfObject,sizeof(doubleSerialArray_Function_1));
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
               printf ("ERROR: In doubleSerialArray_Function_1::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving doubleSerialArray_Function_1::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }


int  doubleSerialArray_Function_2::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

doubleSerialArray_Function_2* doubleSerialArray_Function_2::Current_Link                      = NULL;

int doubleSerialArray_Function_2::Memory_Block_Index                = 0;

const int doubleSerialArray_Function_2::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *doubleSerialArray_Function_2::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *doubleSerialArray_Function_2::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call doubleSerialArray_Function_2::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
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

     if (Size != sizeof(doubleSerialArray_Function_2))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from doubleSerialArray_Function_2
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In doubleSerialArray_Function_2::operator new: Calling malloc because Size(%d) != sizeof(doubleSerialArray_Function_2)(%d) \n",Size,sizeof(doubleSerialArray_Function_2));

          return malloc(Size);
        }
       else
        {
       // printf ("In doubleSerialArray_Function_2::operator new: Using the pool mechanism Size(%d) == sizeof(doubleSerialArray_Function_2)(%d) \n",Size,sizeof(doubleSerialArray_Function_2));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (doubleSerialArray_Function_2*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(doubleSerialArray_Function_2) );
#else
               Current_Link = (doubleSerialArray_Function_2*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(doubleSerialArray_Function_2) ];
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

     doubleSerialArray_Function_2* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from doubleSerialArray_Function_2::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void doubleSerialArray_Function_2::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In doubleSerialArray_Function_2::operator delete: Size(%d)  sizeof(doubleSerialArray_Function_2)(%d) \n",sizeOfObject,sizeof(doubleSerialArray_Function_2));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(doubleSerialArray_Function_2))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from doubleSerialArray_Function_2
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In doubleSerialArray_Function_2::operator delete: Calling global delete (free) because Size(%d) != sizeof(doubleSerialArray_Function_2)(%d) \n",sizeOfObject,sizeof(doubleSerialArray_Function_2));
             }
#endif

          free(Pointer);
        }
       else
        {
          doubleSerialArray_Function_2 *New_Link = (doubleSerialArray_Function_2*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In doubleSerialArray_Function_2::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(doubleSerialArray_Function_2)(%d) \n",Pointer,sizeOfObject,sizeof(doubleSerialArray_Function_2));
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
               printf ("ERROR: In doubleSerialArray_Function_2::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving doubleSerialArray_Function_2::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }


int  doubleSerialArray_Function_3::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

doubleSerialArray_Function_3* doubleSerialArray_Function_3::Current_Link                      = NULL;

int doubleSerialArray_Function_3::Memory_Block_Index                = 0;

const int doubleSerialArray_Function_3::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *doubleSerialArray_Function_3::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *doubleSerialArray_Function_3::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call doubleSerialArray_Function_3::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
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

     if (Size != sizeof(doubleSerialArray_Function_3))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from doubleSerialArray_Function_3
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In doubleSerialArray_Function_3::operator new: Calling malloc because Size(%d) != sizeof(doubleSerialArray_Function_3)(%d) \n",Size,sizeof(doubleSerialArray_Function_3));

          return malloc(Size);
        }
       else
        {
       // printf ("In doubleSerialArray_Function_3::operator new: Using the pool mechanism Size(%d) == sizeof(doubleSerialArray_Function_3)(%d) \n",Size,sizeof(doubleSerialArray_Function_3));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (doubleSerialArray_Function_3*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(doubleSerialArray_Function_3) );
#else
               Current_Link = (doubleSerialArray_Function_3*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(doubleSerialArray_Function_3) ];
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

     doubleSerialArray_Function_3* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from doubleSerialArray_Function_3::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void doubleSerialArray_Function_3::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In doubleSerialArray_Function_3::operator delete: Size(%d)  sizeof(doubleSerialArray_Function_3)(%d) \n",sizeOfObject,sizeof(doubleSerialArray_Function_3));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(doubleSerialArray_Function_3))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from doubleSerialArray_Function_3
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In doubleSerialArray_Function_3::operator delete: Calling global delete (free) because Size(%d) != sizeof(doubleSerialArray_Function_3)(%d) \n",sizeOfObject,sizeof(doubleSerialArray_Function_3));
             }
#endif

          free(Pointer);
        }
       else
        {
          doubleSerialArray_Function_3 *New_Link = (doubleSerialArray_Function_3*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In doubleSerialArray_Function_3::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(doubleSerialArray_Function_3)(%d) \n",Pointer,sizeOfObject,sizeof(doubleSerialArray_Function_3));
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
               printf ("ERROR: In doubleSerialArray_Function_3::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving doubleSerialArray_Function_3::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }


int  doubleSerialArray_Function_4::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

doubleSerialArray_Function_4* doubleSerialArray_Function_4::Current_Link                      = NULL;

int doubleSerialArray_Function_4::Memory_Block_Index                = 0;

const int doubleSerialArray_Function_4::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *doubleSerialArray_Function_4::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *doubleSerialArray_Function_4::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call doubleSerialArray_Function_4::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
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

     if (Size != sizeof(doubleSerialArray_Function_4))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from doubleSerialArray_Function_4
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In doubleSerialArray_Function_4::operator new: Calling malloc because Size(%d) != sizeof(doubleSerialArray_Function_4)(%d) \n",Size,sizeof(doubleSerialArray_Function_4));

          return malloc(Size);
        }
       else
        {
       // printf ("In doubleSerialArray_Function_4::operator new: Using the pool mechanism Size(%d) == sizeof(doubleSerialArray_Function_4)(%d) \n",Size,sizeof(doubleSerialArray_Function_4));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (doubleSerialArray_Function_4*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(doubleSerialArray_Function_4) );
#else
               Current_Link = (doubleSerialArray_Function_4*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(doubleSerialArray_Function_4) ];
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

     doubleSerialArray_Function_4* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from doubleSerialArray_Function_4::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void doubleSerialArray_Function_4::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In doubleSerialArray_Function_4::operator delete: Size(%d)  sizeof(doubleSerialArray_Function_4)(%d) \n",sizeOfObject,sizeof(doubleSerialArray_Function_4));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(doubleSerialArray_Function_4))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from doubleSerialArray_Function_4
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In doubleSerialArray_Function_4::operator delete: Calling global delete (free) because Size(%d) != sizeof(doubleSerialArray_Function_4)(%d) \n",sizeOfObject,sizeof(doubleSerialArray_Function_4));
             }
#endif

          free(Pointer);
        }
       else
        {
          doubleSerialArray_Function_4 *New_Link = (doubleSerialArray_Function_4*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In doubleSerialArray_Function_4::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(doubleSerialArray_Function_4)(%d) \n",Pointer,sizeOfObject,sizeof(doubleSerialArray_Function_4));
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
               printf ("ERROR: In doubleSerialArray_Function_4::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving doubleSerialArray_Function_4::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }


int  doubleSerialArray_Function_5::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

doubleSerialArray_Function_5* doubleSerialArray_Function_5::Current_Link                      = NULL;

int doubleSerialArray_Function_5::Memory_Block_Index                = 0;

const int doubleSerialArray_Function_5::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *doubleSerialArray_Function_5::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *doubleSerialArray_Function_5::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call doubleSerialArray_Function_5::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
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

     if (Size != sizeof(doubleSerialArray_Function_5))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from doubleSerialArray_Function_5
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In doubleSerialArray_Function_5::operator new: Calling malloc because Size(%d) != sizeof(doubleSerialArray_Function_5)(%d) \n",Size,sizeof(doubleSerialArray_Function_5));

          return malloc(Size);
        }
       else
        {
       // printf ("In doubleSerialArray_Function_5::operator new: Using the pool mechanism Size(%d) == sizeof(doubleSerialArray_Function_5)(%d) \n",Size,sizeof(doubleSerialArray_Function_5));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (doubleSerialArray_Function_5*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(doubleSerialArray_Function_5) );
#else
               Current_Link = (doubleSerialArray_Function_5*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(doubleSerialArray_Function_5) ];
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

     doubleSerialArray_Function_5* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from doubleSerialArray_Function_5::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void doubleSerialArray_Function_5::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In doubleSerialArray_Function_5::operator delete: Size(%d)  sizeof(doubleSerialArray_Function_5)(%d) \n",sizeOfObject,sizeof(doubleSerialArray_Function_5));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(doubleSerialArray_Function_5))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from doubleSerialArray_Function_5
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In doubleSerialArray_Function_5::operator delete: Calling global delete (free) because Size(%d) != sizeof(doubleSerialArray_Function_5)(%d) \n",sizeOfObject,sizeof(doubleSerialArray_Function_5));
             }
#endif

          free(Pointer);
        }
       else
        {
          doubleSerialArray_Function_5 *New_Link = (doubleSerialArray_Function_5*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In doubleSerialArray_Function_5::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(doubleSerialArray_Function_5)(%d) \n",Pointer,sizeOfObject,sizeof(doubleSerialArray_Function_5));
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
               printf ("ERROR: In doubleSerialArray_Function_5::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving doubleSerialArray_Function_5::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }


int  doubleSerialArray_Function_6::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

doubleSerialArray_Function_6* doubleSerialArray_Function_6::Current_Link                      = NULL;

int doubleSerialArray_Function_6::Memory_Block_Index                = 0;

const int doubleSerialArray_Function_6::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *doubleSerialArray_Function_6::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *doubleSerialArray_Function_6::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call doubleSerialArray_Function_6::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
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

     if (Size != sizeof(doubleSerialArray_Function_6))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from doubleSerialArray_Function_6
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In doubleSerialArray_Function_6::operator new: Calling malloc because Size(%d) != sizeof(doubleSerialArray_Function_6)(%d) \n",Size,sizeof(doubleSerialArray_Function_6));

          return malloc(Size);
        }
       else
        {
       // printf ("In doubleSerialArray_Function_6::operator new: Using the pool mechanism Size(%d) == sizeof(doubleSerialArray_Function_6)(%d) \n",Size,sizeof(doubleSerialArray_Function_6));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (doubleSerialArray_Function_6*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(doubleSerialArray_Function_6) );
#else
               Current_Link = (doubleSerialArray_Function_6*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(doubleSerialArray_Function_6) ];
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

     doubleSerialArray_Function_6* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from doubleSerialArray_Function_6::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void doubleSerialArray_Function_6::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In doubleSerialArray_Function_6::operator delete: Size(%d)  sizeof(doubleSerialArray_Function_6)(%d) \n",sizeOfObject,sizeof(doubleSerialArray_Function_6));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(doubleSerialArray_Function_6))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from doubleSerialArray_Function_6
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In doubleSerialArray_Function_6::operator delete: Calling global delete (free) because Size(%d) != sizeof(doubleSerialArray_Function_6)(%d) \n",sizeOfObject,sizeof(doubleSerialArray_Function_6));
             }
#endif

          free(Pointer);
        }
       else
        {
          doubleSerialArray_Function_6 *New_Link = (doubleSerialArray_Function_6*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In doubleSerialArray_Function_6::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(doubleSerialArray_Function_6)(%d) \n",Pointer,sizeOfObject,sizeof(doubleSerialArray_Function_6));
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
               printf ("ERROR: In doubleSerialArray_Function_6::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving doubleSerialArray_Function_6::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }


int  doubleSerialArray_Function_7::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

doubleSerialArray_Function_7* doubleSerialArray_Function_7::Current_Link                      = NULL;

int doubleSerialArray_Function_7::Memory_Block_Index                = 0;

const int doubleSerialArray_Function_7::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *doubleSerialArray_Function_7::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *doubleSerialArray_Function_7::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call doubleSerialArray_Function_7::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
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

     if (Size != sizeof(doubleSerialArray_Function_7))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from doubleSerialArray_Function_7
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In doubleSerialArray_Function_7::operator new: Calling malloc because Size(%d) != sizeof(doubleSerialArray_Function_7)(%d) \n",Size,sizeof(doubleSerialArray_Function_7));

          return malloc(Size);
        }
       else
        {
       // printf ("In doubleSerialArray_Function_7::operator new: Using the pool mechanism Size(%d) == sizeof(doubleSerialArray_Function_7)(%d) \n",Size,sizeof(doubleSerialArray_Function_7));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (doubleSerialArray_Function_7*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(doubleSerialArray_Function_7) );
#else
               Current_Link = (doubleSerialArray_Function_7*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(doubleSerialArray_Function_7) ];
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

     doubleSerialArray_Function_7* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from doubleSerialArray_Function_7::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void doubleSerialArray_Function_7::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In doubleSerialArray_Function_7::operator delete: Size(%d)  sizeof(doubleSerialArray_Function_7)(%d) \n",sizeOfObject,sizeof(doubleSerialArray_Function_7));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(doubleSerialArray_Function_7))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from doubleSerialArray_Function_7
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In doubleSerialArray_Function_7::operator delete: Calling global delete (free) because Size(%d) != sizeof(doubleSerialArray_Function_7)(%d) \n",sizeOfObject,sizeof(doubleSerialArray_Function_7));
             }
#endif

          free(Pointer);
        }
       else
        {
          doubleSerialArray_Function_7 *New_Link = (doubleSerialArray_Function_7*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In doubleSerialArray_Function_7::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(doubleSerialArray_Function_7)(%d) \n",Pointer,sizeOfObject,sizeof(doubleSerialArray_Function_7));
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
               printf ("ERROR: In doubleSerialArray_Function_7::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving doubleSerialArray_Function_7::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }


#ifndef INTARRAY
int  doubleSerialArray_Function_8::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

doubleSerialArray_Function_8* doubleSerialArray_Function_8::Current_Link                      = NULL;

int doubleSerialArray_Function_8::Memory_Block_Index                = 0;

const int doubleSerialArray_Function_8::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *doubleSerialArray_Function_8::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *doubleSerialArray_Function_8::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call doubleSerialArray_Function_8::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
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

     if (Size != sizeof(doubleSerialArray_Function_8))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from doubleSerialArray_Function_8
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In doubleSerialArray_Function_8::operator new: Calling malloc because Size(%d) != sizeof(doubleSerialArray_Function_8)(%d) \n",Size,sizeof(doubleSerialArray_Function_8));

          return malloc(Size);
        }
       else
        {
       // printf ("In doubleSerialArray_Function_8::operator new: Using the pool mechanism Size(%d) == sizeof(doubleSerialArray_Function_8)(%d) \n",Size,sizeof(doubleSerialArray_Function_8));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (doubleSerialArray_Function_8*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(doubleSerialArray_Function_8) );
#else
               Current_Link = (doubleSerialArray_Function_8*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(doubleSerialArray_Function_8) ];
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

     doubleSerialArray_Function_8* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from doubleSerialArray_Function_8::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void doubleSerialArray_Function_8::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In doubleSerialArray_Function_8::operator delete: Size(%d)  sizeof(doubleSerialArray_Function_8)(%d) \n",sizeOfObject,sizeof(doubleSerialArray_Function_8));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(doubleSerialArray_Function_8))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from doubleSerialArray_Function_8
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In doubleSerialArray_Function_8::operator delete: Calling global delete (free) because Size(%d) != sizeof(doubleSerialArray_Function_8)(%d) \n",sizeOfObject,sizeof(doubleSerialArray_Function_8));
             }
#endif

          free(Pointer);
        }
       else
        {
          doubleSerialArray_Function_8 *New_Link = (doubleSerialArray_Function_8*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In doubleSerialArray_Function_8::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(doubleSerialArray_Function_8)(%d) \n",Pointer,sizeOfObject,sizeof(doubleSerialArray_Function_8));
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
               printf ("ERROR: In doubleSerialArray_Function_8::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving doubleSerialArray_Function_8::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }

#endif

int  doubleSerialArray_Function_9::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

doubleSerialArray_Function_9* doubleSerialArray_Function_9::Current_Link                      = NULL;

int doubleSerialArray_Function_9::Memory_Block_Index                = 0;

const int doubleSerialArray_Function_9::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *doubleSerialArray_Function_9::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *doubleSerialArray_Function_9::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call doubleSerialArray_Function_9::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
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

     if (Size != sizeof(doubleSerialArray_Function_9))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from doubleSerialArray_Function_9
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In doubleSerialArray_Function_9::operator new: Calling malloc because Size(%d) != sizeof(doubleSerialArray_Function_9)(%d) \n",Size,sizeof(doubleSerialArray_Function_9));

          return malloc(Size);
        }
       else
        {
       // printf ("In doubleSerialArray_Function_9::operator new: Using the pool mechanism Size(%d) == sizeof(doubleSerialArray_Function_9)(%d) \n",Size,sizeof(doubleSerialArray_Function_9));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (doubleSerialArray_Function_9*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(doubleSerialArray_Function_9) );
#else
               Current_Link = (doubleSerialArray_Function_9*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(doubleSerialArray_Function_9) ];
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

     doubleSerialArray_Function_9* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from doubleSerialArray_Function_9::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void doubleSerialArray_Function_9::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In doubleSerialArray_Function_9::operator delete: Size(%d)  sizeof(doubleSerialArray_Function_9)(%d) \n",sizeOfObject,sizeof(doubleSerialArray_Function_9));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(doubleSerialArray_Function_9))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from doubleSerialArray_Function_9
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In doubleSerialArray_Function_9::operator delete: Calling global delete (free) because Size(%d) != sizeof(doubleSerialArray_Function_9)(%d) \n",sizeOfObject,sizeof(doubleSerialArray_Function_9));
             }
#endif

          free(Pointer);
        }
       else
        {
          doubleSerialArray_Function_9 *New_Link = (doubleSerialArray_Function_9*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In doubleSerialArray_Function_9::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(doubleSerialArray_Function_9)(%d) \n",Pointer,sizeOfObject,sizeof(doubleSerialArray_Function_9));
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
               printf ("ERROR: In doubleSerialArray_Function_9::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving doubleSerialArray_Function_9::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }


int  doubleSerialArray_Function_11::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

doubleSerialArray_Function_11* doubleSerialArray_Function_11::Current_Link                      = NULL;

int doubleSerialArray_Function_11::Memory_Block_Index                = 0;

const int doubleSerialArray_Function_11::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *doubleSerialArray_Function_11::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *doubleSerialArray_Function_11::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call doubleSerialArray_Function_11::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
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

     if (Size != sizeof(doubleSerialArray_Function_11))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from doubleSerialArray_Function_11
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In doubleSerialArray_Function_11::operator new: Calling malloc because Size(%d) != sizeof(doubleSerialArray_Function_11)(%d) \n",Size,sizeof(doubleSerialArray_Function_11));

          return malloc(Size);
        }
       else
        {
       // printf ("In doubleSerialArray_Function_11::operator new: Using the pool mechanism Size(%d) == sizeof(doubleSerialArray_Function_11)(%d) \n",Size,sizeof(doubleSerialArray_Function_11));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (doubleSerialArray_Function_11*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(doubleSerialArray_Function_11) );
#else
               Current_Link = (doubleSerialArray_Function_11*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(doubleSerialArray_Function_11) ];
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

     doubleSerialArray_Function_11* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from doubleSerialArray_Function_11::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void doubleSerialArray_Function_11::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In doubleSerialArray_Function_11::operator delete: Size(%d)  sizeof(doubleSerialArray_Function_11)(%d) \n",sizeOfObject,sizeof(doubleSerialArray_Function_11));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(doubleSerialArray_Function_11))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from doubleSerialArray_Function_11
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In doubleSerialArray_Function_11::operator delete: Calling global delete (free) because Size(%d) != sizeof(doubleSerialArray_Function_11)(%d) \n",sizeOfObject,sizeof(doubleSerialArray_Function_11));
             }
#endif

          free(Pointer);
        }
       else
        {
          doubleSerialArray_Function_11 *New_Link = (doubleSerialArray_Function_11*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In doubleSerialArray_Function_11::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(doubleSerialArray_Function_11)(%d) \n",Pointer,sizeOfObject,sizeof(doubleSerialArray_Function_11));
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
               printf ("ERROR: In doubleSerialArray_Function_11::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving doubleSerialArray_Function_11::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }


int  doubleSerialArray_Function_12::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

doubleSerialArray_Function_12* doubleSerialArray_Function_12::Current_Link                      = NULL;

int doubleSerialArray_Function_12::Memory_Block_Index                = 0;

const int doubleSerialArray_Function_12::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *doubleSerialArray_Function_12::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *doubleSerialArray_Function_12::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call doubleSerialArray_Function_12::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
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

     if (Size != sizeof(doubleSerialArray_Function_12))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from doubleSerialArray_Function_12
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In doubleSerialArray_Function_12::operator new: Calling malloc because Size(%d) != sizeof(doubleSerialArray_Function_12)(%d) \n",Size,sizeof(doubleSerialArray_Function_12));

          return malloc(Size);
        }
       else
        {
       // printf ("In doubleSerialArray_Function_12::operator new: Using the pool mechanism Size(%d) == sizeof(doubleSerialArray_Function_12)(%d) \n",Size,sizeof(doubleSerialArray_Function_12));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (doubleSerialArray_Function_12*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(doubleSerialArray_Function_12) );
#else
               Current_Link = (doubleSerialArray_Function_12*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(doubleSerialArray_Function_12) ];
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

     doubleSerialArray_Function_12* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from doubleSerialArray_Function_12::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void doubleSerialArray_Function_12::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In doubleSerialArray_Function_12::operator delete: Size(%d)  sizeof(doubleSerialArray_Function_12)(%d) \n",sizeOfObject,sizeof(doubleSerialArray_Function_12));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(doubleSerialArray_Function_12))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from doubleSerialArray_Function_12
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In doubleSerialArray_Function_12::operator delete: Calling global delete (free) because Size(%d) != sizeof(doubleSerialArray_Function_12)(%d) \n",sizeOfObject,sizeof(doubleSerialArray_Function_12));
             }
#endif

          free(Pointer);
        }
       else
        {
          doubleSerialArray_Function_12 *New_Link = (doubleSerialArray_Function_12*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In doubleSerialArray_Function_12::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(doubleSerialArray_Function_12)(%d) \n",Pointer,sizeOfObject,sizeof(doubleSerialArray_Function_12));
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
               printf ("ERROR: In doubleSerialArray_Function_12::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving doubleSerialArray_Function_12::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }


int  doubleSerialArray_Function_14::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

doubleSerialArray_Function_14* doubleSerialArray_Function_14::Current_Link                      = NULL;

int doubleSerialArray_Function_14::Memory_Block_Index                = 0;

const int doubleSerialArray_Function_14::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *doubleSerialArray_Function_14::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *doubleSerialArray_Function_14::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call doubleSerialArray_Function_14::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
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

     if (Size != sizeof(doubleSerialArray_Function_14))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from doubleSerialArray_Function_14
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In doubleSerialArray_Function_14::operator new: Calling malloc because Size(%d) != sizeof(doubleSerialArray_Function_14)(%d) \n",Size,sizeof(doubleSerialArray_Function_14));

          return malloc(Size);
        }
       else
        {
       // printf ("In doubleSerialArray_Function_14::operator new: Using the pool mechanism Size(%d) == sizeof(doubleSerialArray_Function_14)(%d) \n",Size,sizeof(doubleSerialArray_Function_14));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (doubleSerialArray_Function_14*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(doubleSerialArray_Function_14) );
#else
               Current_Link = (doubleSerialArray_Function_14*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(doubleSerialArray_Function_14) ];
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

     doubleSerialArray_Function_14* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from doubleSerialArray_Function_14::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void doubleSerialArray_Function_14::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In doubleSerialArray_Function_14::operator delete: Size(%d)  sizeof(doubleSerialArray_Function_14)(%d) \n",sizeOfObject,sizeof(doubleSerialArray_Function_14));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(doubleSerialArray_Function_14))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from doubleSerialArray_Function_14
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In doubleSerialArray_Function_14::operator delete: Calling global delete (free) because Size(%d) != sizeof(doubleSerialArray_Function_14)(%d) \n",sizeOfObject,sizeof(doubleSerialArray_Function_14));
             }
#endif

          free(Pointer);
        }
       else
        {
          doubleSerialArray_Function_14 *New_Link = (doubleSerialArray_Function_14*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In doubleSerialArray_Function_14::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(doubleSerialArray_Function_14)(%d) \n",Pointer,sizeOfObject,sizeof(doubleSerialArray_Function_14));
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
               printf ("ERROR: In doubleSerialArray_Function_14::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving doubleSerialArray_Function_14::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }


int  doubleSerialArray_Function_15::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

doubleSerialArray_Function_15* doubleSerialArray_Function_15::Current_Link                      = NULL;

int doubleSerialArray_Function_15::Memory_Block_Index                = 0;

const int doubleSerialArray_Function_15::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *doubleSerialArray_Function_15::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *doubleSerialArray_Function_15::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call doubleSerialArray_Function_15::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
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

     if (Size != sizeof(doubleSerialArray_Function_15))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from doubleSerialArray_Function_15
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In doubleSerialArray_Function_15::operator new: Calling malloc because Size(%d) != sizeof(doubleSerialArray_Function_15)(%d) \n",Size,sizeof(doubleSerialArray_Function_15));

          return malloc(Size);
        }
       else
        {
       // printf ("In doubleSerialArray_Function_15::operator new: Using the pool mechanism Size(%d) == sizeof(doubleSerialArray_Function_15)(%d) \n",Size,sizeof(doubleSerialArray_Function_15));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (doubleSerialArray_Function_15*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(doubleSerialArray_Function_15) );
#else
               Current_Link = (doubleSerialArray_Function_15*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(doubleSerialArray_Function_15) ];
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

     doubleSerialArray_Function_15* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from doubleSerialArray_Function_15::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void doubleSerialArray_Function_15::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In doubleSerialArray_Function_15::operator delete: Size(%d)  sizeof(doubleSerialArray_Function_15)(%d) \n",sizeOfObject,sizeof(doubleSerialArray_Function_15));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(doubleSerialArray_Function_15))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from doubleSerialArray_Function_15
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In doubleSerialArray_Function_15::operator delete: Calling global delete (free) because Size(%d) != sizeof(doubleSerialArray_Function_15)(%d) \n",sizeOfObject,sizeof(doubleSerialArray_Function_15));
             }
#endif

          free(Pointer);
        }
       else
        {
          doubleSerialArray_Function_15 *New_Link = (doubleSerialArray_Function_15*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In doubleSerialArray_Function_15::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(doubleSerialArray_Function_15)(%d) \n",Pointer,sizeOfObject,sizeof(doubleSerialArray_Function_15));
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
               printf ("ERROR: In doubleSerialArray_Function_15::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving doubleSerialArray_Function_15::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }


#if COMPILE_DEFERRED_DISPLAY_AND_VIEW_FUNCTIONS
int  doubleSerialArray_Function_16::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

doubleSerialArray_Function_16* doubleSerialArray_Function_16::Current_Link                      = NULL;

int doubleSerialArray_Function_16::Memory_Block_Index                = 0;

const int doubleSerialArray_Function_16::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *doubleSerialArray_Function_16::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *doubleSerialArray_Function_16::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call doubleSerialArray_Function_16::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
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

     if (Size != sizeof(doubleSerialArray_Function_16))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from doubleSerialArray_Function_16
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In doubleSerialArray_Function_16::operator new: Calling malloc because Size(%d) != sizeof(doubleSerialArray_Function_16)(%d) \n",Size,sizeof(doubleSerialArray_Function_16));

          return malloc(Size);
        }
       else
        {
       // printf ("In doubleSerialArray_Function_16::operator new: Using the pool mechanism Size(%d) == sizeof(doubleSerialArray_Function_16)(%d) \n",Size,sizeof(doubleSerialArray_Function_16));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (doubleSerialArray_Function_16*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(doubleSerialArray_Function_16) );
#else
               Current_Link = (doubleSerialArray_Function_16*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(doubleSerialArray_Function_16) ];
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

     doubleSerialArray_Function_16* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from doubleSerialArray_Function_16::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void doubleSerialArray_Function_16::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In doubleSerialArray_Function_16::operator delete: Size(%d)  sizeof(doubleSerialArray_Function_16)(%d) \n",sizeOfObject,sizeof(doubleSerialArray_Function_16));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(doubleSerialArray_Function_16))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from doubleSerialArray_Function_16
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In doubleSerialArray_Function_16::operator delete: Calling global delete (free) because Size(%d) != sizeof(doubleSerialArray_Function_16)(%d) \n",sizeOfObject,sizeof(doubleSerialArray_Function_16));
             }
#endif

          free(Pointer);
        }
       else
        {
          doubleSerialArray_Function_16 *New_Link = (doubleSerialArray_Function_16*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In doubleSerialArray_Function_16::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(doubleSerialArray_Function_16)(%d) \n",Pointer,sizeOfObject,sizeof(doubleSerialArray_Function_16));
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
               printf ("ERROR: In doubleSerialArray_Function_16::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving doubleSerialArray_Function_16::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }

#endif

int  doubleSerialArray_Aggregate_Operator::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

doubleSerialArray_Aggregate_Operator* doubleSerialArray_Aggregate_Operator::Current_Link                      = NULL;

int doubleSerialArray_Aggregate_Operator::Memory_Block_Index                = 0;

const int doubleSerialArray_Aggregate_Operator::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *doubleSerialArray_Aggregate_Operator::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *doubleSerialArray_Aggregate_Operator::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call doubleSerialArray_Aggregate_Operator::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
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

     if (Size != sizeof(doubleSerialArray_Aggregate_Operator))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from doubleSerialArray_Aggregate_Operator
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In doubleSerialArray_Aggregate_Operator::operator new: Calling malloc because Size(%d) != sizeof(doubleSerialArray_Aggregate_Operator)(%d) \n",Size,sizeof(doubleSerialArray_Aggregate_Operator));

          return malloc(Size);
        }
       else
        {
       // printf ("In doubleSerialArray_Aggregate_Operator::operator new: Using the pool mechanism Size(%d) == sizeof(doubleSerialArray_Aggregate_Operator)(%d) \n",Size,sizeof(doubleSerialArray_Aggregate_Operator));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (doubleSerialArray_Aggregate_Operator*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(doubleSerialArray_Aggregate_Operator) );
#else
               Current_Link = (doubleSerialArray_Aggregate_Operator*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(doubleSerialArray_Aggregate_Operator) ];
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

     doubleSerialArray_Aggregate_Operator* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from doubleSerialArray_Aggregate_Operator::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void doubleSerialArray_Aggregate_Operator::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In doubleSerialArray_Aggregate_Operator::operator delete: Size(%d)  sizeof(doubleSerialArray_Aggregate_Operator)(%d) \n",sizeOfObject,sizeof(doubleSerialArray_Aggregate_Operator));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(doubleSerialArray_Aggregate_Operator))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from doubleSerialArray_Aggregate_Operator
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In doubleSerialArray_Aggregate_Operator::operator delete: Calling global delete (free) because Size(%d) != sizeof(doubleSerialArray_Aggregate_Operator)(%d) \n",sizeOfObject,sizeof(doubleSerialArray_Aggregate_Operator));
             }
#endif

          free(Pointer);
        }
       else
        {
          doubleSerialArray_Aggregate_Operator *New_Link = (doubleSerialArray_Aggregate_Operator*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In doubleSerialArray_Aggregate_Operator::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(doubleSerialArray_Aggregate_Operator)(%d) \n",Pointer,sizeOfObject,sizeof(doubleSerialArray_Aggregate_Operator));
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
               printf ("ERROR: In doubleSerialArray_Aggregate_Operator::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving doubleSerialArray_Aggregate_Operator::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }


int  floatSerialArray::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

floatSerialArray* floatSerialArray::Current_Link                      = NULL;

int floatSerialArray::Memory_Block_Index                = 0;

const int floatSerialArray::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *floatSerialArray::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *floatSerialArray::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call floatSerialArray::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
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

     if (Size != sizeof(floatSerialArray))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from floatSerialArray
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In floatSerialArray::operator new: Calling malloc because Size(%d) != sizeof(floatSerialArray)(%d) \n",Size,sizeof(floatSerialArray));

          return malloc(Size);
        }
       else
        {
       // printf ("In floatSerialArray::operator new: Using the pool mechanism Size(%d) == sizeof(floatSerialArray)(%d) \n",Size,sizeof(floatSerialArray));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (floatSerialArray*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(floatSerialArray) );
#else
               Current_Link = (floatSerialArray*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(floatSerialArray) ];
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

     floatSerialArray* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from floatSerialArray::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void floatSerialArray::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In floatSerialArray::operator delete: Size(%d)  sizeof(floatSerialArray)(%d) \n",sizeOfObject,sizeof(floatSerialArray));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(floatSerialArray))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from floatSerialArray
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In floatSerialArray::operator delete: Calling global delete (free) because Size(%d) != sizeof(floatSerialArray)(%d) \n",sizeOfObject,sizeof(floatSerialArray));
             }
#endif

          free(Pointer);
        }
       else
        {
          floatSerialArray *New_Link = (floatSerialArray*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In floatSerialArray::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(floatSerialArray)(%d) \n",Pointer,sizeOfObject,sizeof(floatSerialArray));
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
               printf ("ERROR: In floatSerialArray::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving floatSerialArray::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }


int  floatSerialArray_Function_Steal_Data::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

floatSerialArray_Function_Steal_Data* floatSerialArray_Function_Steal_Data::Current_Link                      = NULL;

int floatSerialArray_Function_Steal_Data::Memory_Block_Index                = 0;

const int floatSerialArray_Function_Steal_Data::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *floatSerialArray_Function_Steal_Data::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *floatSerialArray_Function_Steal_Data::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call floatSerialArray_Function_Steal_Data::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
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

     if (Size != sizeof(floatSerialArray_Function_Steal_Data))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from floatSerialArray_Function_Steal_Data
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In floatSerialArray_Function_Steal_Data::operator new: Calling malloc because Size(%d) != sizeof(floatSerialArray_Function_Steal_Data)(%d) \n",Size,sizeof(floatSerialArray_Function_Steal_Data));

          return malloc(Size);
        }
       else
        {
       // printf ("In floatSerialArray_Function_Steal_Data::operator new: Using the pool mechanism Size(%d) == sizeof(floatSerialArray_Function_Steal_Data)(%d) \n",Size,sizeof(floatSerialArray_Function_Steal_Data));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (floatSerialArray_Function_Steal_Data*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(floatSerialArray_Function_Steal_Data) );
#else
               Current_Link = (floatSerialArray_Function_Steal_Data*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(floatSerialArray_Function_Steal_Data) ];
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

     floatSerialArray_Function_Steal_Data* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from floatSerialArray_Function_Steal_Data::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void floatSerialArray_Function_Steal_Data::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In floatSerialArray_Function_Steal_Data::operator delete: Size(%d)  sizeof(floatSerialArray_Function_Steal_Data)(%d) \n",sizeOfObject,sizeof(floatSerialArray_Function_Steal_Data));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(floatSerialArray_Function_Steal_Data))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from floatSerialArray_Function_Steal_Data
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In floatSerialArray_Function_Steal_Data::operator delete: Calling global delete (free) because Size(%d) != sizeof(floatSerialArray_Function_Steal_Data)(%d) \n",sizeOfObject,sizeof(floatSerialArray_Function_Steal_Data));
             }
#endif

          free(Pointer);
        }
       else
        {
          floatSerialArray_Function_Steal_Data *New_Link = (floatSerialArray_Function_Steal_Data*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In floatSerialArray_Function_Steal_Data::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(floatSerialArray_Function_Steal_Data)(%d) \n",Pointer,sizeOfObject,sizeof(floatSerialArray_Function_Steal_Data));
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
               printf ("ERROR: In floatSerialArray_Function_Steal_Data::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving floatSerialArray_Function_Steal_Data::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }


int  floatSerialArray_Function_0::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

floatSerialArray_Function_0* floatSerialArray_Function_0::Current_Link                      = NULL;

int floatSerialArray_Function_0::Memory_Block_Index                = 0;

const int floatSerialArray_Function_0::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *floatSerialArray_Function_0::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *floatSerialArray_Function_0::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call floatSerialArray_Function_0::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
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

     if (Size != sizeof(floatSerialArray_Function_0))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from floatSerialArray_Function_0
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In floatSerialArray_Function_0::operator new: Calling malloc because Size(%d) != sizeof(floatSerialArray_Function_0)(%d) \n",Size,sizeof(floatSerialArray_Function_0));

          return malloc(Size);
        }
       else
        {
       // printf ("In floatSerialArray_Function_0::operator new: Using the pool mechanism Size(%d) == sizeof(floatSerialArray_Function_0)(%d) \n",Size,sizeof(floatSerialArray_Function_0));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (floatSerialArray_Function_0*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(floatSerialArray_Function_0) );
#else
               Current_Link = (floatSerialArray_Function_0*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(floatSerialArray_Function_0) ];
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

     floatSerialArray_Function_0* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from floatSerialArray_Function_0::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void floatSerialArray_Function_0::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In floatSerialArray_Function_0::operator delete: Size(%d)  sizeof(floatSerialArray_Function_0)(%d) \n",sizeOfObject,sizeof(floatSerialArray_Function_0));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(floatSerialArray_Function_0))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from floatSerialArray_Function_0
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In floatSerialArray_Function_0::operator delete: Calling global delete (free) because Size(%d) != sizeof(floatSerialArray_Function_0)(%d) \n",sizeOfObject,sizeof(floatSerialArray_Function_0));
             }
#endif

          free(Pointer);
        }
       else
        {
          floatSerialArray_Function_0 *New_Link = (floatSerialArray_Function_0*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In floatSerialArray_Function_0::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(floatSerialArray_Function_0)(%d) \n",Pointer,sizeOfObject,sizeof(floatSerialArray_Function_0));
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
               printf ("ERROR: In floatSerialArray_Function_0::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving floatSerialArray_Function_0::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }


int  floatSerialArray_Function_1::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

floatSerialArray_Function_1* floatSerialArray_Function_1::Current_Link                      = NULL;

int floatSerialArray_Function_1::Memory_Block_Index                = 0;

const int floatSerialArray_Function_1::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *floatSerialArray_Function_1::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *floatSerialArray_Function_1::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call floatSerialArray_Function_1::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
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

     if (Size != sizeof(floatSerialArray_Function_1))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from floatSerialArray_Function_1
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In floatSerialArray_Function_1::operator new: Calling malloc because Size(%d) != sizeof(floatSerialArray_Function_1)(%d) \n",Size,sizeof(floatSerialArray_Function_1));

          return malloc(Size);
        }
       else
        {
       // printf ("In floatSerialArray_Function_1::operator new: Using the pool mechanism Size(%d) == sizeof(floatSerialArray_Function_1)(%d) \n",Size,sizeof(floatSerialArray_Function_1));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (floatSerialArray_Function_1*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(floatSerialArray_Function_1) );
#else
               Current_Link = (floatSerialArray_Function_1*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(floatSerialArray_Function_1) ];
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

     floatSerialArray_Function_1* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from floatSerialArray_Function_1::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void floatSerialArray_Function_1::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In floatSerialArray_Function_1::operator delete: Size(%d)  sizeof(floatSerialArray_Function_1)(%d) \n",sizeOfObject,sizeof(floatSerialArray_Function_1));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(floatSerialArray_Function_1))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from floatSerialArray_Function_1
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In floatSerialArray_Function_1::operator delete: Calling global delete (free) because Size(%d) != sizeof(floatSerialArray_Function_1)(%d) \n",sizeOfObject,sizeof(floatSerialArray_Function_1));
             }
#endif

          free(Pointer);
        }
       else
        {
          floatSerialArray_Function_1 *New_Link = (floatSerialArray_Function_1*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In floatSerialArray_Function_1::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(floatSerialArray_Function_1)(%d) \n",Pointer,sizeOfObject,sizeof(floatSerialArray_Function_1));
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
               printf ("ERROR: In floatSerialArray_Function_1::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving floatSerialArray_Function_1::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }


int  floatSerialArray_Function_2::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

floatSerialArray_Function_2* floatSerialArray_Function_2::Current_Link                      = NULL;

int floatSerialArray_Function_2::Memory_Block_Index                = 0;

const int floatSerialArray_Function_2::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *floatSerialArray_Function_2::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *floatSerialArray_Function_2::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call floatSerialArray_Function_2::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
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

     if (Size != sizeof(floatSerialArray_Function_2))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from floatSerialArray_Function_2
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In floatSerialArray_Function_2::operator new: Calling malloc because Size(%d) != sizeof(floatSerialArray_Function_2)(%d) \n",Size,sizeof(floatSerialArray_Function_2));

          return malloc(Size);
        }
       else
        {
       // printf ("In floatSerialArray_Function_2::operator new: Using the pool mechanism Size(%d) == sizeof(floatSerialArray_Function_2)(%d) \n",Size,sizeof(floatSerialArray_Function_2));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (floatSerialArray_Function_2*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(floatSerialArray_Function_2) );
#else
               Current_Link = (floatSerialArray_Function_2*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(floatSerialArray_Function_2) ];
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

     floatSerialArray_Function_2* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from floatSerialArray_Function_2::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void floatSerialArray_Function_2::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In floatSerialArray_Function_2::operator delete: Size(%d)  sizeof(floatSerialArray_Function_2)(%d) \n",sizeOfObject,sizeof(floatSerialArray_Function_2));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(floatSerialArray_Function_2))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from floatSerialArray_Function_2
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In floatSerialArray_Function_2::operator delete: Calling global delete (free) because Size(%d) != sizeof(floatSerialArray_Function_2)(%d) \n",sizeOfObject,sizeof(floatSerialArray_Function_2));
             }
#endif

          free(Pointer);
        }
       else
        {
          floatSerialArray_Function_2 *New_Link = (floatSerialArray_Function_2*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In floatSerialArray_Function_2::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(floatSerialArray_Function_2)(%d) \n",Pointer,sizeOfObject,sizeof(floatSerialArray_Function_2));
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
               printf ("ERROR: In floatSerialArray_Function_2::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving floatSerialArray_Function_2::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }


int  floatSerialArray_Function_3::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

floatSerialArray_Function_3* floatSerialArray_Function_3::Current_Link                      = NULL;

int floatSerialArray_Function_3::Memory_Block_Index                = 0;

const int floatSerialArray_Function_3::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *floatSerialArray_Function_3::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *floatSerialArray_Function_3::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call floatSerialArray_Function_3::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
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

     if (Size != sizeof(floatSerialArray_Function_3))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from floatSerialArray_Function_3
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In floatSerialArray_Function_3::operator new: Calling malloc because Size(%d) != sizeof(floatSerialArray_Function_3)(%d) \n",Size,sizeof(floatSerialArray_Function_3));

          return malloc(Size);
        }
       else
        {
       // printf ("In floatSerialArray_Function_3::operator new: Using the pool mechanism Size(%d) == sizeof(floatSerialArray_Function_3)(%d) \n",Size,sizeof(floatSerialArray_Function_3));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (floatSerialArray_Function_3*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(floatSerialArray_Function_3) );
#else
               Current_Link = (floatSerialArray_Function_3*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(floatSerialArray_Function_3) ];
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

     floatSerialArray_Function_3* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from floatSerialArray_Function_3::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void floatSerialArray_Function_3::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In floatSerialArray_Function_3::operator delete: Size(%d)  sizeof(floatSerialArray_Function_3)(%d) \n",sizeOfObject,sizeof(floatSerialArray_Function_3));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(floatSerialArray_Function_3))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from floatSerialArray_Function_3
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In floatSerialArray_Function_3::operator delete: Calling global delete (free) because Size(%d) != sizeof(floatSerialArray_Function_3)(%d) \n",sizeOfObject,sizeof(floatSerialArray_Function_3));
             }
#endif

          free(Pointer);
        }
       else
        {
          floatSerialArray_Function_3 *New_Link = (floatSerialArray_Function_3*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In floatSerialArray_Function_3::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(floatSerialArray_Function_3)(%d) \n",Pointer,sizeOfObject,sizeof(floatSerialArray_Function_3));
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
               printf ("ERROR: In floatSerialArray_Function_3::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving floatSerialArray_Function_3::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }


int  floatSerialArray_Function_4::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

floatSerialArray_Function_4* floatSerialArray_Function_4::Current_Link                      = NULL;

int floatSerialArray_Function_4::Memory_Block_Index                = 0;

const int floatSerialArray_Function_4::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *floatSerialArray_Function_4::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *floatSerialArray_Function_4::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call floatSerialArray_Function_4::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
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

     if (Size != sizeof(floatSerialArray_Function_4))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from floatSerialArray_Function_4
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In floatSerialArray_Function_4::operator new: Calling malloc because Size(%d) != sizeof(floatSerialArray_Function_4)(%d) \n",Size,sizeof(floatSerialArray_Function_4));

          return malloc(Size);
        }
       else
        {
       // printf ("In floatSerialArray_Function_4::operator new: Using the pool mechanism Size(%d) == sizeof(floatSerialArray_Function_4)(%d) \n",Size,sizeof(floatSerialArray_Function_4));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (floatSerialArray_Function_4*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(floatSerialArray_Function_4) );
#else
               Current_Link = (floatSerialArray_Function_4*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(floatSerialArray_Function_4) ];
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

     floatSerialArray_Function_4* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from floatSerialArray_Function_4::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void floatSerialArray_Function_4::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In floatSerialArray_Function_4::operator delete: Size(%d)  sizeof(floatSerialArray_Function_4)(%d) \n",sizeOfObject,sizeof(floatSerialArray_Function_4));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(floatSerialArray_Function_4))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from floatSerialArray_Function_4
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In floatSerialArray_Function_4::operator delete: Calling global delete (free) because Size(%d) != sizeof(floatSerialArray_Function_4)(%d) \n",sizeOfObject,sizeof(floatSerialArray_Function_4));
             }
#endif

          free(Pointer);
        }
       else
        {
          floatSerialArray_Function_4 *New_Link = (floatSerialArray_Function_4*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In floatSerialArray_Function_4::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(floatSerialArray_Function_4)(%d) \n",Pointer,sizeOfObject,sizeof(floatSerialArray_Function_4));
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
               printf ("ERROR: In floatSerialArray_Function_4::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving floatSerialArray_Function_4::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }


int  floatSerialArray_Function_5::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

floatSerialArray_Function_5* floatSerialArray_Function_5::Current_Link                      = NULL;

int floatSerialArray_Function_5::Memory_Block_Index                = 0;

const int floatSerialArray_Function_5::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *floatSerialArray_Function_5::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *floatSerialArray_Function_5::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call floatSerialArray_Function_5::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
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

     if (Size != sizeof(floatSerialArray_Function_5))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from floatSerialArray_Function_5
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In floatSerialArray_Function_5::operator new: Calling malloc because Size(%d) != sizeof(floatSerialArray_Function_5)(%d) \n",Size,sizeof(floatSerialArray_Function_5));

          return malloc(Size);
        }
       else
        {
       // printf ("In floatSerialArray_Function_5::operator new: Using the pool mechanism Size(%d) == sizeof(floatSerialArray_Function_5)(%d) \n",Size,sizeof(floatSerialArray_Function_5));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (floatSerialArray_Function_5*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(floatSerialArray_Function_5) );
#else
               Current_Link = (floatSerialArray_Function_5*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(floatSerialArray_Function_5) ];
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

     floatSerialArray_Function_5* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from floatSerialArray_Function_5::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void floatSerialArray_Function_5::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In floatSerialArray_Function_5::operator delete: Size(%d)  sizeof(floatSerialArray_Function_5)(%d) \n",sizeOfObject,sizeof(floatSerialArray_Function_5));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(floatSerialArray_Function_5))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from floatSerialArray_Function_5
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In floatSerialArray_Function_5::operator delete: Calling global delete (free) because Size(%d) != sizeof(floatSerialArray_Function_5)(%d) \n",sizeOfObject,sizeof(floatSerialArray_Function_5));
             }
#endif

          free(Pointer);
        }
       else
        {
          floatSerialArray_Function_5 *New_Link = (floatSerialArray_Function_5*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In floatSerialArray_Function_5::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(floatSerialArray_Function_5)(%d) \n",Pointer,sizeOfObject,sizeof(floatSerialArray_Function_5));
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
               printf ("ERROR: In floatSerialArray_Function_5::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving floatSerialArray_Function_5::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }


int  floatSerialArray_Function_6::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

floatSerialArray_Function_6* floatSerialArray_Function_6::Current_Link                      = NULL;

int floatSerialArray_Function_6::Memory_Block_Index                = 0;

const int floatSerialArray_Function_6::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *floatSerialArray_Function_6::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *floatSerialArray_Function_6::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call floatSerialArray_Function_6::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
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

     if (Size != sizeof(floatSerialArray_Function_6))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from floatSerialArray_Function_6
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In floatSerialArray_Function_6::operator new: Calling malloc because Size(%d) != sizeof(floatSerialArray_Function_6)(%d) \n",Size,sizeof(floatSerialArray_Function_6));

          return malloc(Size);
        }
       else
        {
       // printf ("In floatSerialArray_Function_6::operator new: Using the pool mechanism Size(%d) == sizeof(floatSerialArray_Function_6)(%d) \n",Size,sizeof(floatSerialArray_Function_6));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (floatSerialArray_Function_6*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(floatSerialArray_Function_6) );
#else
               Current_Link = (floatSerialArray_Function_6*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(floatSerialArray_Function_6) ];
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

     floatSerialArray_Function_6* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from floatSerialArray_Function_6::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void floatSerialArray_Function_6::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In floatSerialArray_Function_6::operator delete: Size(%d)  sizeof(floatSerialArray_Function_6)(%d) \n",sizeOfObject,sizeof(floatSerialArray_Function_6));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(floatSerialArray_Function_6))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from floatSerialArray_Function_6
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In floatSerialArray_Function_6::operator delete: Calling global delete (free) because Size(%d) != sizeof(floatSerialArray_Function_6)(%d) \n",sizeOfObject,sizeof(floatSerialArray_Function_6));
             }
#endif

          free(Pointer);
        }
       else
        {
          floatSerialArray_Function_6 *New_Link = (floatSerialArray_Function_6*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In floatSerialArray_Function_6::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(floatSerialArray_Function_6)(%d) \n",Pointer,sizeOfObject,sizeof(floatSerialArray_Function_6));
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
               printf ("ERROR: In floatSerialArray_Function_6::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving floatSerialArray_Function_6::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }


int  floatSerialArray_Function_7::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

floatSerialArray_Function_7* floatSerialArray_Function_7::Current_Link                      = NULL;

int floatSerialArray_Function_7::Memory_Block_Index                = 0;

const int floatSerialArray_Function_7::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *floatSerialArray_Function_7::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *floatSerialArray_Function_7::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call floatSerialArray_Function_7::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
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

     if (Size != sizeof(floatSerialArray_Function_7))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from floatSerialArray_Function_7
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In floatSerialArray_Function_7::operator new: Calling malloc because Size(%d) != sizeof(floatSerialArray_Function_7)(%d) \n",Size,sizeof(floatSerialArray_Function_7));

          return malloc(Size);
        }
       else
        {
       // printf ("In floatSerialArray_Function_7::operator new: Using the pool mechanism Size(%d) == sizeof(floatSerialArray_Function_7)(%d) \n",Size,sizeof(floatSerialArray_Function_7));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (floatSerialArray_Function_7*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(floatSerialArray_Function_7) );
#else
               Current_Link = (floatSerialArray_Function_7*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(floatSerialArray_Function_7) ];
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

     floatSerialArray_Function_7* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from floatSerialArray_Function_7::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void floatSerialArray_Function_7::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In floatSerialArray_Function_7::operator delete: Size(%d)  sizeof(floatSerialArray_Function_7)(%d) \n",sizeOfObject,sizeof(floatSerialArray_Function_7));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(floatSerialArray_Function_7))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from floatSerialArray_Function_7
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In floatSerialArray_Function_7::operator delete: Calling global delete (free) because Size(%d) != sizeof(floatSerialArray_Function_7)(%d) \n",sizeOfObject,sizeof(floatSerialArray_Function_7));
             }
#endif

          free(Pointer);
        }
       else
        {
          floatSerialArray_Function_7 *New_Link = (floatSerialArray_Function_7*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In floatSerialArray_Function_7::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(floatSerialArray_Function_7)(%d) \n",Pointer,sizeOfObject,sizeof(floatSerialArray_Function_7));
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
               printf ("ERROR: In floatSerialArray_Function_7::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving floatSerialArray_Function_7::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }


#ifndef INTARRAY
int  floatSerialArray_Function_8::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

floatSerialArray_Function_8* floatSerialArray_Function_8::Current_Link                      = NULL;

int floatSerialArray_Function_8::Memory_Block_Index                = 0;

const int floatSerialArray_Function_8::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *floatSerialArray_Function_8::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *floatSerialArray_Function_8::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call floatSerialArray_Function_8::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
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

     if (Size != sizeof(floatSerialArray_Function_8))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from floatSerialArray_Function_8
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In floatSerialArray_Function_8::operator new: Calling malloc because Size(%d) != sizeof(floatSerialArray_Function_8)(%d) \n",Size,sizeof(floatSerialArray_Function_8));

          return malloc(Size);
        }
       else
        {
       // printf ("In floatSerialArray_Function_8::operator new: Using the pool mechanism Size(%d) == sizeof(floatSerialArray_Function_8)(%d) \n",Size,sizeof(floatSerialArray_Function_8));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (floatSerialArray_Function_8*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(floatSerialArray_Function_8) );
#else
               Current_Link = (floatSerialArray_Function_8*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(floatSerialArray_Function_8) ];
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

     floatSerialArray_Function_8* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from floatSerialArray_Function_8::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void floatSerialArray_Function_8::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In floatSerialArray_Function_8::operator delete: Size(%d)  sizeof(floatSerialArray_Function_8)(%d) \n",sizeOfObject,sizeof(floatSerialArray_Function_8));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(floatSerialArray_Function_8))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from floatSerialArray_Function_8
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In floatSerialArray_Function_8::operator delete: Calling global delete (free) because Size(%d) != sizeof(floatSerialArray_Function_8)(%d) \n",sizeOfObject,sizeof(floatSerialArray_Function_8));
             }
#endif

          free(Pointer);
        }
       else
        {
          floatSerialArray_Function_8 *New_Link = (floatSerialArray_Function_8*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In floatSerialArray_Function_8::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(floatSerialArray_Function_8)(%d) \n",Pointer,sizeOfObject,sizeof(floatSerialArray_Function_8));
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
               printf ("ERROR: In floatSerialArray_Function_8::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving floatSerialArray_Function_8::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }

#endif

int  floatSerialArray_Function_9::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

floatSerialArray_Function_9* floatSerialArray_Function_9::Current_Link                      = NULL;

int floatSerialArray_Function_9::Memory_Block_Index                = 0;

const int floatSerialArray_Function_9::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *floatSerialArray_Function_9::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *floatSerialArray_Function_9::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call floatSerialArray_Function_9::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
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

     if (Size != sizeof(floatSerialArray_Function_9))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from floatSerialArray_Function_9
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In floatSerialArray_Function_9::operator new: Calling malloc because Size(%d) != sizeof(floatSerialArray_Function_9)(%d) \n",Size,sizeof(floatSerialArray_Function_9));

          return malloc(Size);
        }
       else
        {
       // printf ("In floatSerialArray_Function_9::operator new: Using the pool mechanism Size(%d) == sizeof(floatSerialArray_Function_9)(%d) \n",Size,sizeof(floatSerialArray_Function_9));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (floatSerialArray_Function_9*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(floatSerialArray_Function_9) );
#else
               Current_Link = (floatSerialArray_Function_9*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(floatSerialArray_Function_9) ];
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

     floatSerialArray_Function_9* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from floatSerialArray_Function_9::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void floatSerialArray_Function_9::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In floatSerialArray_Function_9::operator delete: Size(%d)  sizeof(floatSerialArray_Function_9)(%d) \n",sizeOfObject,sizeof(floatSerialArray_Function_9));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(floatSerialArray_Function_9))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from floatSerialArray_Function_9
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In floatSerialArray_Function_9::operator delete: Calling global delete (free) because Size(%d) != sizeof(floatSerialArray_Function_9)(%d) \n",sizeOfObject,sizeof(floatSerialArray_Function_9));
             }
#endif

          free(Pointer);
        }
       else
        {
          floatSerialArray_Function_9 *New_Link = (floatSerialArray_Function_9*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In floatSerialArray_Function_9::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(floatSerialArray_Function_9)(%d) \n",Pointer,sizeOfObject,sizeof(floatSerialArray_Function_9));
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
               printf ("ERROR: In floatSerialArray_Function_9::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving floatSerialArray_Function_9::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }


int  floatSerialArray_Function_11::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

floatSerialArray_Function_11* floatSerialArray_Function_11::Current_Link                      = NULL;

int floatSerialArray_Function_11::Memory_Block_Index                = 0;

const int floatSerialArray_Function_11::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *floatSerialArray_Function_11::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *floatSerialArray_Function_11::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call floatSerialArray_Function_11::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
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

     if (Size != sizeof(floatSerialArray_Function_11))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from floatSerialArray_Function_11
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In floatSerialArray_Function_11::operator new: Calling malloc because Size(%d) != sizeof(floatSerialArray_Function_11)(%d) \n",Size,sizeof(floatSerialArray_Function_11));

          return malloc(Size);
        }
       else
        {
       // printf ("In floatSerialArray_Function_11::operator new: Using the pool mechanism Size(%d) == sizeof(floatSerialArray_Function_11)(%d) \n",Size,sizeof(floatSerialArray_Function_11));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (floatSerialArray_Function_11*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(floatSerialArray_Function_11) );
#else
               Current_Link = (floatSerialArray_Function_11*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(floatSerialArray_Function_11) ];
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

     floatSerialArray_Function_11* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from floatSerialArray_Function_11::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void floatSerialArray_Function_11::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In floatSerialArray_Function_11::operator delete: Size(%d)  sizeof(floatSerialArray_Function_11)(%d) \n",sizeOfObject,sizeof(floatSerialArray_Function_11));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(floatSerialArray_Function_11))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from floatSerialArray_Function_11
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In floatSerialArray_Function_11::operator delete: Calling global delete (free) because Size(%d) != sizeof(floatSerialArray_Function_11)(%d) \n",sizeOfObject,sizeof(floatSerialArray_Function_11));
             }
#endif

          free(Pointer);
        }
       else
        {
          floatSerialArray_Function_11 *New_Link = (floatSerialArray_Function_11*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In floatSerialArray_Function_11::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(floatSerialArray_Function_11)(%d) \n",Pointer,sizeOfObject,sizeof(floatSerialArray_Function_11));
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
               printf ("ERROR: In floatSerialArray_Function_11::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving floatSerialArray_Function_11::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }


int  floatSerialArray_Function_12::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

floatSerialArray_Function_12* floatSerialArray_Function_12::Current_Link                      = NULL;

int floatSerialArray_Function_12::Memory_Block_Index                = 0;

const int floatSerialArray_Function_12::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *floatSerialArray_Function_12::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *floatSerialArray_Function_12::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call floatSerialArray_Function_12::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
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

     if (Size != sizeof(floatSerialArray_Function_12))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from floatSerialArray_Function_12
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In floatSerialArray_Function_12::operator new: Calling malloc because Size(%d) != sizeof(floatSerialArray_Function_12)(%d) \n",Size,sizeof(floatSerialArray_Function_12));

          return malloc(Size);
        }
       else
        {
       // printf ("In floatSerialArray_Function_12::operator new: Using the pool mechanism Size(%d) == sizeof(floatSerialArray_Function_12)(%d) \n",Size,sizeof(floatSerialArray_Function_12));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (floatSerialArray_Function_12*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(floatSerialArray_Function_12) );
#else
               Current_Link = (floatSerialArray_Function_12*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(floatSerialArray_Function_12) ];
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

     floatSerialArray_Function_12* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from floatSerialArray_Function_12::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void floatSerialArray_Function_12::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In floatSerialArray_Function_12::operator delete: Size(%d)  sizeof(floatSerialArray_Function_12)(%d) \n",sizeOfObject,sizeof(floatSerialArray_Function_12));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(floatSerialArray_Function_12))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from floatSerialArray_Function_12
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In floatSerialArray_Function_12::operator delete: Calling global delete (free) because Size(%d) != sizeof(floatSerialArray_Function_12)(%d) \n",sizeOfObject,sizeof(floatSerialArray_Function_12));
             }
#endif

          free(Pointer);
        }
       else
        {
          floatSerialArray_Function_12 *New_Link = (floatSerialArray_Function_12*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In floatSerialArray_Function_12::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(floatSerialArray_Function_12)(%d) \n",Pointer,sizeOfObject,sizeof(floatSerialArray_Function_12));
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
               printf ("ERROR: In floatSerialArray_Function_12::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving floatSerialArray_Function_12::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }


int  floatSerialArray_Function_14::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

floatSerialArray_Function_14* floatSerialArray_Function_14::Current_Link                      = NULL;

int floatSerialArray_Function_14::Memory_Block_Index                = 0;

const int floatSerialArray_Function_14::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *floatSerialArray_Function_14::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *floatSerialArray_Function_14::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call floatSerialArray_Function_14::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
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

     if (Size != sizeof(floatSerialArray_Function_14))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from floatSerialArray_Function_14
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In floatSerialArray_Function_14::operator new: Calling malloc because Size(%d) != sizeof(floatSerialArray_Function_14)(%d) \n",Size,sizeof(floatSerialArray_Function_14));

          return malloc(Size);
        }
       else
        {
       // printf ("In floatSerialArray_Function_14::operator new: Using the pool mechanism Size(%d) == sizeof(floatSerialArray_Function_14)(%d) \n",Size,sizeof(floatSerialArray_Function_14));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (floatSerialArray_Function_14*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(floatSerialArray_Function_14) );
#else
               Current_Link = (floatSerialArray_Function_14*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(floatSerialArray_Function_14) ];
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

     floatSerialArray_Function_14* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from floatSerialArray_Function_14::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void floatSerialArray_Function_14::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In floatSerialArray_Function_14::operator delete: Size(%d)  sizeof(floatSerialArray_Function_14)(%d) \n",sizeOfObject,sizeof(floatSerialArray_Function_14));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(floatSerialArray_Function_14))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from floatSerialArray_Function_14
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In floatSerialArray_Function_14::operator delete: Calling global delete (free) because Size(%d) != sizeof(floatSerialArray_Function_14)(%d) \n",sizeOfObject,sizeof(floatSerialArray_Function_14));
             }
#endif

          free(Pointer);
        }
       else
        {
          floatSerialArray_Function_14 *New_Link = (floatSerialArray_Function_14*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In floatSerialArray_Function_14::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(floatSerialArray_Function_14)(%d) \n",Pointer,sizeOfObject,sizeof(floatSerialArray_Function_14));
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
               printf ("ERROR: In floatSerialArray_Function_14::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving floatSerialArray_Function_14::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }


int  floatSerialArray_Function_15::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

floatSerialArray_Function_15* floatSerialArray_Function_15::Current_Link                      = NULL;

int floatSerialArray_Function_15::Memory_Block_Index                = 0;

const int floatSerialArray_Function_15::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *floatSerialArray_Function_15::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *floatSerialArray_Function_15::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call floatSerialArray_Function_15::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
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

     if (Size != sizeof(floatSerialArray_Function_15))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from floatSerialArray_Function_15
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In floatSerialArray_Function_15::operator new: Calling malloc because Size(%d) != sizeof(floatSerialArray_Function_15)(%d) \n",Size,sizeof(floatSerialArray_Function_15));

          return malloc(Size);
        }
       else
        {
       // printf ("In floatSerialArray_Function_15::operator new: Using the pool mechanism Size(%d) == sizeof(floatSerialArray_Function_15)(%d) \n",Size,sizeof(floatSerialArray_Function_15));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (floatSerialArray_Function_15*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(floatSerialArray_Function_15) );
#else
               Current_Link = (floatSerialArray_Function_15*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(floatSerialArray_Function_15) ];
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

     floatSerialArray_Function_15* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from floatSerialArray_Function_15::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void floatSerialArray_Function_15::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In floatSerialArray_Function_15::operator delete: Size(%d)  sizeof(floatSerialArray_Function_15)(%d) \n",sizeOfObject,sizeof(floatSerialArray_Function_15));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(floatSerialArray_Function_15))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from floatSerialArray_Function_15
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In floatSerialArray_Function_15::operator delete: Calling global delete (free) because Size(%d) != sizeof(floatSerialArray_Function_15)(%d) \n",sizeOfObject,sizeof(floatSerialArray_Function_15));
             }
#endif

          free(Pointer);
        }
       else
        {
          floatSerialArray_Function_15 *New_Link = (floatSerialArray_Function_15*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In floatSerialArray_Function_15::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(floatSerialArray_Function_15)(%d) \n",Pointer,sizeOfObject,sizeof(floatSerialArray_Function_15));
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
               printf ("ERROR: In floatSerialArray_Function_15::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving floatSerialArray_Function_15::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }


#if COMPILE_DEFERRED_DISPLAY_AND_VIEW_FUNCTIONS
int  floatSerialArray_Function_16::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

floatSerialArray_Function_16* floatSerialArray_Function_16::Current_Link                      = NULL;

int floatSerialArray_Function_16::Memory_Block_Index                = 0;

const int floatSerialArray_Function_16::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *floatSerialArray_Function_16::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *floatSerialArray_Function_16::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call floatSerialArray_Function_16::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
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

     if (Size != sizeof(floatSerialArray_Function_16))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from floatSerialArray_Function_16
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In floatSerialArray_Function_16::operator new: Calling malloc because Size(%d) != sizeof(floatSerialArray_Function_16)(%d) \n",Size,sizeof(floatSerialArray_Function_16));

          return malloc(Size);
        }
       else
        {
       // printf ("In floatSerialArray_Function_16::operator new: Using the pool mechanism Size(%d) == sizeof(floatSerialArray_Function_16)(%d) \n",Size,sizeof(floatSerialArray_Function_16));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (floatSerialArray_Function_16*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(floatSerialArray_Function_16) );
#else
               Current_Link = (floatSerialArray_Function_16*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(floatSerialArray_Function_16) ];
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

     floatSerialArray_Function_16* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from floatSerialArray_Function_16::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void floatSerialArray_Function_16::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In floatSerialArray_Function_16::operator delete: Size(%d)  sizeof(floatSerialArray_Function_16)(%d) \n",sizeOfObject,sizeof(floatSerialArray_Function_16));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(floatSerialArray_Function_16))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from floatSerialArray_Function_16
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In floatSerialArray_Function_16::operator delete: Calling global delete (free) because Size(%d) != sizeof(floatSerialArray_Function_16)(%d) \n",sizeOfObject,sizeof(floatSerialArray_Function_16));
             }
#endif

          free(Pointer);
        }
       else
        {
          floatSerialArray_Function_16 *New_Link = (floatSerialArray_Function_16*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In floatSerialArray_Function_16::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(floatSerialArray_Function_16)(%d) \n",Pointer,sizeOfObject,sizeof(floatSerialArray_Function_16));
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
               printf ("ERROR: In floatSerialArray_Function_16::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving floatSerialArray_Function_16::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }

#endif

int  floatSerialArray_Aggregate_Operator::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

floatSerialArray_Aggregate_Operator* floatSerialArray_Aggregate_Operator::Current_Link                      = NULL;

int floatSerialArray_Aggregate_Operator::Memory_Block_Index                = 0;

const int floatSerialArray_Aggregate_Operator::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *floatSerialArray_Aggregate_Operator::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *floatSerialArray_Aggregate_Operator::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call floatSerialArray_Aggregate_Operator::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
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

     if (Size != sizeof(floatSerialArray_Aggregate_Operator))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from floatSerialArray_Aggregate_Operator
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In floatSerialArray_Aggregate_Operator::operator new: Calling malloc because Size(%d) != sizeof(floatSerialArray_Aggregate_Operator)(%d) \n",Size,sizeof(floatSerialArray_Aggregate_Operator));

          return malloc(Size);
        }
       else
        {
       // printf ("In floatSerialArray_Aggregate_Operator::operator new: Using the pool mechanism Size(%d) == sizeof(floatSerialArray_Aggregate_Operator)(%d) \n",Size,sizeof(floatSerialArray_Aggregate_Operator));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (floatSerialArray_Aggregate_Operator*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(floatSerialArray_Aggregate_Operator) );
#else
               Current_Link = (floatSerialArray_Aggregate_Operator*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(floatSerialArray_Aggregate_Operator) ];
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

     floatSerialArray_Aggregate_Operator* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from floatSerialArray_Aggregate_Operator::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void floatSerialArray_Aggregate_Operator::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In floatSerialArray_Aggregate_Operator::operator delete: Size(%d)  sizeof(floatSerialArray_Aggregate_Operator)(%d) \n",sizeOfObject,sizeof(floatSerialArray_Aggregate_Operator));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(floatSerialArray_Aggregate_Operator))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from floatSerialArray_Aggregate_Operator
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In floatSerialArray_Aggregate_Operator::operator delete: Calling global delete (free) because Size(%d) != sizeof(floatSerialArray_Aggregate_Operator)(%d) \n",sizeOfObject,sizeof(floatSerialArray_Aggregate_Operator));
             }
#endif

          free(Pointer);
        }
       else
        {
          floatSerialArray_Aggregate_Operator *New_Link = (floatSerialArray_Aggregate_Operator*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In floatSerialArray_Aggregate_Operator::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(floatSerialArray_Aggregate_Operator)(%d) \n",Pointer,sizeOfObject,sizeof(floatSerialArray_Aggregate_Operator));
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
               printf ("ERROR: In floatSerialArray_Aggregate_Operator::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving floatSerialArray_Aggregate_Operator::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }



#define INTARRAY
int  intSerialArray::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

intSerialArray* intSerialArray::Current_Link                      = NULL;

int intSerialArray::Memory_Block_Index                = 0;

const int intSerialArray::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *intSerialArray::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *intSerialArray::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call intSerialArray::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
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

     if (Size != sizeof(intSerialArray))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from intSerialArray
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In intSerialArray::operator new: Calling malloc because Size(%d) != sizeof(intSerialArray)(%d) \n",Size,sizeof(intSerialArray));

          return malloc(Size);
        }
       else
        {
       // printf ("In intSerialArray::operator new: Using the pool mechanism Size(%d) == sizeof(intSerialArray)(%d) \n",Size,sizeof(intSerialArray));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (intSerialArray*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(intSerialArray) );
#else
               Current_Link = (intSerialArray*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(intSerialArray) ];
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

     intSerialArray* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from intSerialArray::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void intSerialArray::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In intSerialArray::operator delete: Size(%d)  sizeof(intSerialArray)(%d) \n",sizeOfObject,sizeof(intSerialArray));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(intSerialArray))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from intSerialArray
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In intSerialArray::operator delete: Calling global delete (free) because Size(%d) != sizeof(intSerialArray)(%d) \n",sizeOfObject,sizeof(intSerialArray));
             }
#endif

          free(Pointer);
        }
       else
        {
          intSerialArray *New_Link = (intSerialArray*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In intSerialArray::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(intSerialArray)(%d) \n",Pointer,sizeOfObject,sizeof(intSerialArray));
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
               printf ("ERROR: In intSerialArray::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving intSerialArray::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }


int  intSerialArray_Function_Steal_Data::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

intSerialArray_Function_Steal_Data* intSerialArray_Function_Steal_Data::Current_Link                      = NULL;

int intSerialArray_Function_Steal_Data::Memory_Block_Index                = 0;

const int intSerialArray_Function_Steal_Data::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *intSerialArray_Function_Steal_Data::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *intSerialArray_Function_Steal_Data::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call intSerialArray_Function_Steal_Data::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
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

     if (Size != sizeof(intSerialArray_Function_Steal_Data))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from intSerialArray_Function_Steal_Data
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In intSerialArray_Function_Steal_Data::operator new: Calling malloc because Size(%d) != sizeof(intSerialArray_Function_Steal_Data)(%d) \n",Size,sizeof(intSerialArray_Function_Steal_Data));

          return malloc(Size);
        }
       else
        {
       // printf ("In intSerialArray_Function_Steal_Data::operator new: Using the pool mechanism Size(%d) == sizeof(intSerialArray_Function_Steal_Data)(%d) \n",Size,sizeof(intSerialArray_Function_Steal_Data));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (intSerialArray_Function_Steal_Data*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(intSerialArray_Function_Steal_Data) );
#else
               Current_Link = (intSerialArray_Function_Steal_Data*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(intSerialArray_Function_Steal_Data) ];
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

     intSerialArray_Function_Steal_Data* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from intSerialArray_Function_Steal_Data::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void intSerialArray_Function_Steal_Data::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In intSerialArray_Function_Steal_Data::operator delete: Size(%d)  sizeof(intSerialArray_Function_Steal_Data)(%d) \n",sizeOfObject,sizeof(intSerialArray_Function_Steal_Data));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(intSerialArray_Function_Steal_Data))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from intSerialArray_Function_Steal_Data
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In intSerialArray_Function_Steal_Data::operator delete: Calling global delete (free) because Size(%d) != sizeof(intSerialArray_Function_Steal_Data)(%d) \n",sizeOfObject,sizeof(intSerialArray_Function_Steal_Data));
             }
#endif

          free(Pointer);
        }
       else
        {
          intSerialArray_Function_Steal_Data *New_Link = (intSerialArray_Function_Steal_Data*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In intSerialArray_Function_Steal_Data::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(intSerialArray_Function_Steal_Data)(%d) \n",Pointer,sizeOfObject,sizeof(intSerialArray_Function_Steal_Data));
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
               printf ("ERROR: In intSerialArray_Function_Steal_Data::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving intSerialArray_Function_Steal_Data::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }


int  intSerialArray_Function_0::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

intSerialArray_Function_0* intSerialArray_Function_0::Current_Link                      = NULL;

int intSerialArray_Function_0::Memory_Block_Index                = 0;

const int intSerialArray_Function_0::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *intSerialArray_Function_0::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *intSerialArray_Function_0::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call intSerialArray_Function_0::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
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

     if (Size != sizeof(intSerialArray_Function_0))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from intSerialArray_Function_0
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In intSerialArray_Function_0::operator new: Calling malloc because Size(%d) != sizeof(intSerialArray_Function_0)(%d) \n",Size,sizeof(intSerialArray_Function_0));

          return malloc(Size);
        }
       else
        {
       // printf ("In intSerialArray_Function_0::operator new: Using the pool mechanism Size(%d) == sizeof(intSerialArray_Function_0)(%d) \n",Size,sizeof(intSerialArray_Function_0));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (intSerialArray_Function_0*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(intSerialArray_Function_0) );
#else
               Current_Link = (intSerialArray_Function_0*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(intSerialArray_Function_0) ];
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

     intSerialArray_Function_0* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from intSerialArray_Function_0::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void intSerialArray_Function_0::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In intSerialArray_Function_0::operator delete: Size(%d)  sizeof(intSerialArray_Function_0)(%d) \n",sizeOfObject,sizeof(intSerialArray_Function_0));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(intSerialArray_Function_0))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from intSerialArray_Function_0
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In intSerialArray_Function_0::operator delete: Calling global delete (free) because Size(%d) != sizeof(intSerialArray_Function_0)(%d) \n",sizeOfObject,sizeof(intSerialArray_Function_0));
             }
#endif

          free(Pointer);
        }
       else
        {
          intSerialArray_Function_0 *New_Link = (intSerialArray_Function_0*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In intSerialArray_Function_0::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(intSerialArray_Function_0)(%d) \n",Pointer,sizeOfObject,sizeof(intSerialArray_Function_0));
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
               printf ("ERROR: In intSerialArray_Function_0::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving intSerialArray_Function_0::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }


int  intSerialArray_Function_1::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

intSerialArray_Function_1* intSerialArray_Function_1::Current_Link                      = NULL;

int intSerialArray_Function_1::Memory_Block_Index                = 0;

const int intSerialArray_Function_1::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *intSerialArray_Function_1::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *intSerialArray_Function_1::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call intSerialArray_Function_1::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
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

     if (Size != sizeof(intSerialArray_Function_1))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from intSerialArray_Function_1
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In intSerialArray_Function_1::operator new: Calling malloc because Size(%d) != sizeof(intSerialArray_Function_1)(%d) \n",Size,sizeof(intSerialArray_Function_1));

          return malloc(Size);
        }
       else
        {
       // printf ("In intSerialArray_Function_1::operator new: Using the pool mechanism Size(%d) == sizeof(intSerialArray_Function_1)(%d) \n",Size,sizeof(intSerialArray_Function_1));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (intSerialArray_Function_1*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(intSerialArray_Function_1) );
#else
               Current_Link = (intSerialArray_Function_1*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(intSerialArray_Function_1) ];
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

     intSerialArray_Function_1* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from intSerialArray_Function_1::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void intSerialArray_Function_1::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In intSerialArray_Function_1::operator delete: Size(%d)  sizeof(intSerialArray_Function_1)(%d) \n",sizeOfObject,sizeof(intSerialArray_Function_1));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(intSerialArray_Function_1))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from intSerialArray_Function_1
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In intSerialArray_Function_1::operator delete: Calling global delete (free) because Size(%d) != sizeof(intSerialArray_Function_1)(%d) \n",sizeOfObject,sizeof(intSerialArray_Function_1));
             }
#endif

          free(Pointer);
        }
       else
        {
          intSerialArray_Function_1 *New_Link = (intSerialArray_Function_1*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In intSerialArray_Function_1::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(intSerialArray_Function_1)(%d) \n",Pointer,sizeOfObject,sizeof(intSerialArray_Function_1));
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
               printf ("ERROR: In intSerialArray_Function_1::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving intSerialArray_Function_1::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }


int  intSerialArray_Function_2::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

intSerialArray_Function_2* intSerialArray_Function_2::Current_Link                      = NULL;

int intSerialArray_Function_2::Memory_Block_Index                = 0;

const int intSerialArray_Function_2::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *intSerialArray_Function_2::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *intSerialArray_Function_2::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call intSerialArray_Function_2::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
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

     if (Size != sizeof(intSerialArray_Function_2))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from intSerialArray_Function_2
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In intSerialArray_Function_2::operator new: Calling malloc because Size(%d) != sizeof(intSerialArray_Function_2)(%d) \n",Size,sizeof(intSerialArray_Function_2));

          return malloc(Size);
        }
       else
        {
       // printf ("In intSerialArray_Function_2::operator new: Using the pool mechanism Size(%d) == sizeof(intSerialArray_Function_2)(%d) \n",Size,sizeof(intSerialArray_Function_2));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (intSerialArray_Function_2*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(intSerialArray_Function_2) );
#else
               Current_Link = (intSerialArray_Function_2*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(intSerialArray_Function_2) ];
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

     intSerialArray_Function_2* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from intSerialArray_Function_2::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void intSerialArray_Function_2::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In intSerialArray_Function_2::operator delete: Size(%d)  sizeof(intSerialArray_Function_2)(%d) \n",sizeOfObject,sizeof(intSerialArray_Function_2));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(intSerialArray_Function_2))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from intSerialArray_Function_2
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In intSerialArray_Function_2::operator delete: Calling global delete (free) because Size(%d) != sizeof(intSerialArray_Function_2)(%d) \n",sizeOfObject,sizeof(intSerialArray_Function_2));
             }
#endif

          free(Pointer);
        }
       else
        {
          intSerialArray_Function_2 *New_Link = (intSerialArray_Function_2*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In intSerialArray_Function_2::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(intSerialArray_Function_2)(%d) \n",Pointer,sizeOfObject,sizeof(intSerialArray_Function_2));
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
               printf ("ERROR: In intSerialArray_Function_2::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving intSerialArray_Function_2::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }


int  intSerialArray_Function_3::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

intSerialArray_Function_3* intSerialArray_Function_3::Current_Link                      = NULL;

int intSerialArray_Function_3::Memory_Block_Index                = 0;

const int intSerialArray_Function_3::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *intSerialArray_Function_3::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *intSerialArray_Function_3::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call intSerialArray_Function_3::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
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

     if (Size != sizeof(intSerialArray_Function_3))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from intSerialArray_Function_3
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In intSerialArray_Function_3::operator new: Calling malloc because Size(%d) != sizeof(intSerialArray_Function_3)(%d) \n",Size,sizeof(intSerialArray_Function_3));

          return malloc(Size);
        }
       else
        {
       // printf ("In intSerialArray_Function_3::operator new: Using the pool mechanism Size(%d) == sizeof(intSerialArray_Function_3)(%d) \n",Size,sizeof(intSerialArray_Function_3));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (intSerialArray_Function_3*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(intSerialArray_Function_3) );
#else
               Current_Link = (intSerialArray_Function_3*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(intSerialArray_Function_3) ];
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

     intSerialArray_Function_3* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from intSerialArray_Function_3::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void intSerialArray_Function_3::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In intSerialArray_Function_3::operator delete: Size(%d)  sizeof(intSerialArray_Function_3)(%d) \n",sizeOfObject,sizeof(intSerialArray_Function_3));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(intSerialArray_Function_3))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from intSerialArray_Function_3
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In intSerialArray_Function_3::operator delete: Calling global delete (free) because Size(%d) != sizeof(intSerialArray_Function_3)(%d) \n",sizeOfObject,sizeof(intSerialArray_Function_3));
             }
#endif

          free(Pointer);
        }
       else
        {
          intSerialArray_Function_3 *New_Link = (intSerialArray_Function_3*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In intSerialArray_Function_3::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(intSerialArray_Function_3)(%d) \n",Pointer,sizeOfObject,sizeof(intSerialArray_Function_3));
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
               printf ("ERROR: In intSerialArray_Function_3::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving intSerialArray_Function_3::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }


int  intSerialArray_Function_4::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

intSerialArray_Function_4* intSerialArray_Function_4::Current_Link                      = NULL;

int intSerialArray_Function_4::Memory_Block_Index                = 0;

const int intSerialArray_Function_4::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *intSerialArray_Function_4::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *intSerialArray_Function_4::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call intSerialArray_Function_4::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
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

     if (Size != sizeof(intSerialArray_Function_4))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from intSerialArray_Function_4
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In intSerialArray_Function_4::operator new: Calling malloc because Size(%d) != sizeof(intSerialArray_Function_4)(%d) \n",Size,sizeof(intSerialArray_Function_4));

          return malloc(Size);
        }
       else
        {
       // printf ("In intSerialArray_Function_4::operator new: Using the pool mechanism Size(%d) == sizeof(intSerialArray_Function_4)(%d) \n",Size,sizeof(intSerialArray_Function_4));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (intSerialArray_Function_4*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(intSerialArray_Function_4) );
#else
               Current_Link = (intSerialArray_Function_4*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(intSerialArray_Function_4) ];
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

     intSerialArray_Function_4* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from intSerialArray_Function_4::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void intSerialArray_Function_4::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In intSerialArray_Function_4::operator delete: Size(%d)  sizeof(intSerialArray_Function_4)(%d) \n",sizeOfObject,sizeof(intSerialArray_Function_4));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(intSerialArray_Function_4))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from intSerialArray_Function_4
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In intSerialArray_Function_4::operator delete: Calling global delete (free) because Size(%d) != sizeof(intSerialArray_Function_4)(%d) \n",sizeOfObject,sizeof(intSerialArray_Function_4));
             }
#endif

          free(Pointer);
        }
       else
        {
          intSerialArray_Function_4 *New_Link = (intSerialArray_Function_4*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In intSerialArray_Function_4::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(intSerialArray_Function_4)(%d) \n",Pointer,sizeOfObject,sizeof(intSerialArray_Function_4));
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
               printf ("ERROR: In intSerialArray_Function_4::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving intSerialArray_Function_4::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }


int  intSerialArray_Function_5::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

intSerialArray_Function_5* intSerialArray_Function_5::Current_Link                      = NULL;

int intSerialArray_Function_5::Memory_Block_Index                = 0;

const int intSerialArray_Function_5::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *intSerialArray_Function_5::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *intSerialArray_Function_5::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call intSerialArray_Function_5::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
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

     if (Size != sizeof(intSerialArray_Function_5))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from intSerialArray_Function_5
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In intSerialArray_Function_5::operator new: Calling malloc because Size(%d) != sizeof(intSerialArray_Function_5)(%d) \n",Size,sizeof(intSerialArray_Function_5));

          return malloc(Size);
        }
       else
        {
       // printf ("In intSerialArray_Function_5::operator new: Using the pool mechanism Size(%d) == sizeof(intSerialArray_Function_5)(%d) \n",Size,sizeof(intSerialArray_Function_5));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (intSerialArray_Function_5*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(intSerialArray_Function_5) );
#else
               Current_Link = (intSerialArray_Function_5*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(intSerialArray_Function_5) ];
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

     intSerialArray_Function_5* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from intSerialArray_Function_5::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void intSerialArray_Function_5::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In intSerialArray_Function_5::operator delete: Size(%d)  sizeof(intSerialArray_Function_5)(%d) \n",sizeOfObject,sizeof(intSerialArray_Function_5));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(intSerialArray_Function_5))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from intSerialArray_Function_5
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In intSerialArray_Function_5::operator delete: Calling global delete (free) because Size(%d) != sizeof(intSerialArray_Function_5)(%d) \n",sizeOfObject,sizeof(intSerialArray_Function_5));
             }
#endif

          free(Pointer);
        }
       else
        {
          intSerialArray_Function_5 *New_Link = (intSerialArray_Function_5*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In intSerialArray_Function_5::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(intSerialArray_Function_5)(%d) \n",Pointer,sizeOfObject,sizeof(intSerialArray_Function_5));
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
               printf ("ERROR: In intSerialArray_Function_5::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving intSerialArray_Function_5::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }


int  intSerialArray_Function_6::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

intSerialArray_Function_6* intSerialArray_Function_6::Current_Link                      = NULL;

int intSerialArray_Function_6::Memory_Block_Index                = 0;

const int intSerialArray_Function_6::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *intSerialArray_Function_6::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *intSerialArray_Function_6::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call intSerialArray_Function_6::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
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

     if (Size != sizeof(intSerialArray_Function_6))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from intSerialArray_Function_6
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In intSerialArray_Function_6::operator new: Calling malloc because Size(%d) != sizeof(intSerialArray_Function_6)(%d) \n",Size,sizeof(intSerialArray_Function_6));

          return malloc(Size);
        }
       else
        {
       // printf ("In intSerialArray_Function_6::operator new: Using the pool mechanism Size(%d) == sizeof(intSerialArray_Function_6)(%d) \n",Size,sizeof(intSerialArray_Function_6));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (intSerialArray_Function_6*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(intSerialArray_Function_6) );
#else
               Current_Link = (intSerialArray_Function_6*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(intSerialArray_Function_6) ];
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

     intSerialArray_Function_6* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from intSerialArray_Function_6::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void intSerialArray_Function_6::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In intSerialArray_Function_6::operator delete: Size(%d)  sizeof(intSerialArray_Function_6)(%d) \n",sizeOfObject,sizeof(intSerialArray_Function_6));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(intSerialArray_Function_6))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from intSerialArray_Function_6
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In intSerialArray_Function_6::operator delete: Calling global delete (free) because Size(%d) != sizeof(intSerialArray_Function_6)(%d) \n",sizeOfObject,sizeof(intSerialArray_Function_6));
             }
#endif

          free(Pointer);
        }
       else
        {
          intSerialArray_Function_6 *New_Link = (intSerialArray_Function_6*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In intSerialArray_Function_6::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(intSerialArray_Function_6)(%d) \n",Pointer,sizeOfObject,sizeof(intSerialArray_Function_6));
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
               printf ("ERROR: In intSerialArray_Function_6::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving intSerialArray_Function_6::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }


int  intSerialArray_Function_7::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

intSerialArray_Function_7* intSerialArray_Function_7::Current_Link                      = NULL;

int intSerialArray_Function_7::Memory_Block_Index                = 0;

const int intSerialArray_Function_7::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *intSerialArray_Function_7::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *intSerialArray_Function_7::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call intSerialArray_Function_7::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
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

     if (Size != sizeof(intSerialArray_Function_7))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from intSerialArray_Function_7
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In intSerialArray_Function_7::operator new: Calling malloc because Size(%d) != sizeof(intSerialArray_Function_7)(%d) \n",Size,sizeof(intSerialArray_Function_7));

          return malloc(Size);
        }
       else
        {
       // printf ("In intSerialArray_Function_7::operator new: Using the pool mechanism Size(%d) == sizeof(intSerialArray_Function_7)(%d) \n",Size,sizeof(intSerialArray_Function_7));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (intSerialArray_Function_7*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(intSerialArray_Function_7) );
#else
               Current_Link = (intSerialArray_Function_7*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(intSerialArray_Function_7) ];
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

     intSerialArray_Function_7* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from intSerialArray_Function_7::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void intSerialArray_Function_7::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In intSerialArray_Function_7::operator delete: Size(%d)  sizeof(intSerialArray_Function_7)(%d) \n",sizeOfObject,sizeof(intSerialArray_Function_7));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(intSerialArray_Function_7))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from intSerialArray_Function_7
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In intSerialArray_Function_7::operator delete: Calling global delete (free) because Size(%d) != sizeof(intSerialArray_Function_7)(%d) \n",sizeOfObject,sizeof(intSerialArray_Function_7));
             }
#endif

          free(Pointer);
        }
       else
        {
          intSerialArray_Function_7 *New_Link = (intSerialArray_Function_7*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In intSerialArray_Function_7::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(intSerialArray_Function_7)(%d) \n",Pointer,sizeOfObject,sizeof(intSerialArray_Function_7));
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
               printf ("ERROR: In intSerialArray_Function_7::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving intSerialArray_Function_7::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }


#ifndef INTARRAY
int  intSerialArray_Function_8::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

intSerialArray_Function_8* intSerialArray_Function_8::Current_Link                      = NULL;

int intSerialArray_Function_8::Memory_Block_Index                = 0;

const int intSerialArray_Function_8::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *intSerialArray_Function_8::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *intSerialArray_Function_8::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call intSerialArray_Function_8::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
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

     if (Size != sizeof(intSerialArray_Function_8))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from intSerialArray_Function_8
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In intSerialArray_Function_8::operator new: Calling malloc because Size(%d) != sizeof(intSerialArray_Function_8)(%d) \n",Size,sizeof(intSerialArray_Function_8));

          return malloc(Size);
        }
       else
        {
       // printf ("In intSerialArray_Function_8::operator new: Using the pool mechanism Size(%d) == sizeof(intSerialArray_Function_8)(%d) \n",Size,sizeof(intSerialArray_Function_8));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (intSerialArray_Function_8*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(intSerialArray_Function_8) );
#else
               Current_Link = (intSerialArray_Function_8*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(intSerialArray_Function_8) ];
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

     intSerialArray_Function_8* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from intSerialArray_Function_8::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void intSerialArray_Function_8::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In intSerialArray_Function_8::operator delete: Size(%d)  sizeof(intSerialArray_Function_8)(%d) \n",sizeOfObject,sizeof(intSerialArray_Function_8));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(intSerialArray_Function_8))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from intSerialArray_Function_8
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In intSerialArray_Function_8::operator delete: Calling global delete (free) because Size(%d) != sizeof(intSerialArray_Function_8)(%d) \n",sizeOfObject,sizeof(intSerialArray_Function_8));
             }
#endif

          free(Pointer);
        }
       else
        {
          intSerialArray_Function_8 *New_Link = (intSerialArray_Function_8*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In intSerialArray_Function_8::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(intSerialArray_Function_8)(%d) \n",Pointer,sizeOfObject,sizeof(intSerialArray_Function_8));
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
               printf ("ERROR: In intSerialArray_Function_8::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving intSerialArray_Function_8::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }

#endif

int  intSerialArray_Function_9::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

intSerialArray_Function_9* intSerialArray_Function_9::Current_Link                      = NULL;

int intSerialArray_Function_9::Memory_Block_Index                = 0;

const int intSerialArray_Function_9::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *intSerialArray_Function_9::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *intSerialArray_Function_9::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call intSerialArray_Function_9::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
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

     if (Size != sizeof(intSerialArray_Function_9))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from intSerialArray_Function_9
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In intSerialArray_Function_9::operator new: Calling malloc because Size(%d) != sizeof(intSerialArray_Function_9)(%d) \n",Size,sizeof(intSerialArray_Function_9));

          return malloc(Size);
        }
       else
        {
       // printf ("In intSerialArray_Function_9::operator new: Using the pool mechanism Size(%d) == sizeof(intSerialArray_Function_9)(%d) \n",Size,sizeof(intSerialArray_Function_9));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (intSerialArray_Function_9*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(intSerialArray_Function_9) );
#else
               Current_Link = (intSerialArray_Function_9*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(intSerialArray_Function_9) ];
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

     intSerialArray_Function_9* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from intSerialArray_Function_9::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void intSerialArray_Function_9::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In intSerialArray_Function_9::operator delete: Size(%d)  sizeof(intSerialArray_Function_9)(%d) \n",sizeOfObject,sizeof(intSerialArray_Function_9));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(intSerialArray_Function_9))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from intSerialArray_Function_9
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In intSerialArray_Function_9::operator delete: Calling global delete (free) because Size(%d) != sizeof(intSerialArray_Function_9)(%d) \n",sizeOfObject,sizeof(intSerialArray_Function_9));
             }
#endif

          free(Pointer);
        }
       else
        {
          intSerialArray_Function_9 *New_Link = (intSerialArray_Function_9*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In intSerialArray_Function_9::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(intSerialArray_Function_9)(%d) \n",Pointer,sizeOfObject,sizeof(intSerialArray_Function_9));
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
               printf ("ERROR: In intSerialArray_Function_9::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving intSerialArray_Function_9::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }


int  intSerialArray_Function_11::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

intSerialArray_Function_11* intSerialArray_Function_11::Current_Link                      = NULL;

int intSerialArray_Function_11::Memory_Block_Index                = 0;

const int intSerialArray_Function_11::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *intSerialArray_Function_11::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *intSerialArray_Function_11::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call intSerialArray_Function_11::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
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

     if (Size != sizeof(intSerialArray_Function_11))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from intSerialArray_Function_11
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In intSerialArray_Function_11::operator new: Calling malloc because Size(%d) != sizeof(intSerialArray_Function_11)(%d) \n",Size,sizeof(intSerialArray_Function_11));

          return malloc(Size);
        }
       else
        {
       // printf ("In intSerialArray_Function_11::operator new: Using the pool mechanism Size(%d) == sizeof(intSerialArray_Function_11)(%d) \n",Size,sizeof(intSerialArray_Function_11));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (intSerialArray_Function_11*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(intSerialArray_Function_11) );
#else
               Current_Link = (intSerialArray_Function_11*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(intSerialArray_Function_11) ];
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

     intSerialArray_Function_11* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from intSerialArray_Function_11::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void intSerialArray_Function_11::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In intSerialArray_Function_11::operator delete: Size(%d)  sizeof(intSerialArray_Function_11)(%d) \n",sizeOfObject,sizeof(intSerialArray_Function_11));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(intSerialArray_Function_11))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from intSerialArray_Function_11
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In intSerialArray_Function_11::operator delete: Calling global delete (free) because Size(%d) != sizeof(intSerialArray_Function_11)(%d) \n",sizeOfObject,sizeof(intSerialArray_Function_11));
             }
#endif

          free(Pointer);
        }
       else
        {
          intSerialArray_Function_11 *New_Link = (intSerialArray_Function_11*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In intSerialArray_Function_11::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(intSerialArray_Function_11)(%d) \n",Pointer,sizeOfObject,sizeof(intSerialArray_Function_11));
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
               printf ("ERROR: In intSerialArray_Function_11::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving intSerialArray_Function_11::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }


int  intSerialArray_Function_12::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

intSerialArray_Function_12* intSerialArray_Function_12::Current_Link                      = NULL;

int intSerialArray_Function_12::Memory_Block_Index                = 0;

const int intSerialArray_Function_12::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *intSerialArray_Function_12::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *intSerialArray_Function_12::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call intSerialArray_Function_12::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
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

     if (Size != sizeof(intSerialArray_Function_12))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from intSerialArray_Function_12
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In intSerialArray_Function_12::operator new: Calling malloc because Size(%d) != sizeof(intSerialArray_Function_12)(%d) \n",Size,sizeof(intSerialArray_Function_12));

          return malloc(Size);
        }
       else
        {
       // printf ("In intSerialArray_Function_12::operator new: Using the pool mechanism Size(%d) == sizeof(intSerialArray_Function_12)(%d) \n",Size,sizeof(intSerialArray_Function_12));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (intSerialArray_Function_12*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(intSerialArray_Function_12) );
#else
               Current_Link = (intSerialArray_Function_12*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(intSerialArray_Function_12) ];
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

     intSerialArray_Function_12* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from intSerialArray_Function_12::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void intSerialArray_Function_12::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In intSerialArray_Function_12::operator delete: Size(%d)  sizeof(intSerialArray_Function_12)(%d) \n",sizeOfObject,sizeof(intSerialArray_Function_12));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(intSerialArray_Function_12))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from intSerialArray_Function_12
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In intSerialArray_Function_12::operator delete: Calling global delete (free) because Size(%d) != sizeof(intSerialArray_Function_12)(%d) \n",sizeOfObject,sizeof(intSerialArray_Function_12));
             }
#endif

          free(Pointer);
        }
       else
        {
          intSerialArray_Function_12 *New_Link = (intSerialArray_Function_12*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In intSerialArray_Function_12::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(intSerialArray_Function_12)(%d) \n",Pointer,sizeOfObject,sizeof(intSerialArray_Function_12));
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
               printf ("ERROR: In intSerialArray_Function_12::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving intSerialArray_Function_12::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }


int  intSerialArray_Function_14::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

intSerialArray_Function_14* intSerialArray_Function_14::Current_Link                      = NULL;

int intSerialArray_Function_14::Memory_Block_Index                = 0;

const int intSerialArray_Function_14::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *intSerialArray_Function_14::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *intSerialArray_Function_14::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call intSerialArray_Function_14::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
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

     if (Size != sizeof(intSerialArray_Function_14))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from intSerialArray_Function_14
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In intSerialArray_Function_14::operator new: Calling malloc because Size(%d) != sizeof(intSerialArray_Function_14)(%d) \n",Size,sizeof(intSerialArray_Function_14));

          return malloc(Size);
        }
       else
        {
       // printf ("In intSerialArray_Function_14::operator new: Using the pool mechanism Size(%d) == sizeof(intSerialArray_Function_14)(%d) \n",Size,sizeof(intSerialArray_Function_14));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (intSerialArray_Function_14*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(intSerialArray_Function_14) );
#else
               Current_Link = (intSerialArray_Function_14*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(intSerialArray_Function_14) ];
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

     intSerialArray_Function_14* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from intSerialArray_Function_14::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void intSerialArray_Function_14::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In intSerialArray_Function_14::operator delete: Size(%d)  sizeof(intSerialArray_Function_14)(%d) \n",sizeOfObject,sizeof(intSerialArray_Function_14));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(intSerialArray_Function_14))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from intSerialArray_Function_14
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In intSerialArray_Function_14::operator delete: Calling global delete (free) because Size(%d) != sizeof(intSerialArray_Function_14)(%d) \n",sizeOfObject,sizeof(intSerialArray_Function_14));
             }
#endif

          free(Pointer);
        }
       else
        {
          intSerialArray_Function_14 *New_Link = (intSerialArray_Function_14*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In intSerialArray_Function_14::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(intSerialArray_Function_14)(%d) \n",Pointer,sizeOfObject,sizeof(intSerialArray_Function_14));
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
               printf ("ERROR: In intSerialArray_Function_14::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving intSerialArray_Function_14::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }


int  intSerialArray_Function_15::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

intSerialArray_Function_15* intSerialArray_Function_15::Current_Link                      = NULL;

int intSerialArray_Function_15::Memory_Block_Index                = 0;

const int intSerialArray_Function_15::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *intSerialArray_Function_15::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *intSerialArray_Function_15::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call intSerialArray_Function_15::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
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

     if (Size != sizeof(intSerialArray_Function_15))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from intSerialArray_Function_15
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In intSerialArray_Function_15::operator new: Calling malloc because Size(%d) != sizeof(intSerialArray_Function_15)(%d) \n",Size,sizeof(intSerialArray_Function_15));

          return malloc(Size);
        }
       else
        {
       // printf ("In intSerialArray_Function_15::operator new: Using the pool mechanism Size(%d) == sizeof(intSerialArray_Function_15)(%d) \n",Size,sizeof(intSerialArray_Function_15));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (intSerialArray_Function_15*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(intSerialArray_Function_15) );
#else
               Current_Link = (intSerialArray_Function_15*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(intSerialArray_Function_15) ];
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

     intSerialArray_Function_15* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from intSerialArray_Function_15::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void intSerialArray_Function_15::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In intSerialArray_Function_15::operator delete: Size(%d)  sizeof(intSerialArray_Function_15)(%d) \n",sizeOfObject,sizeof(intSerialArray_Function_15));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(intSerialArray_Function_15))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from intSerialArray_Function_15
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In intSerialArray_Function_15::operator delete: Calling global delete (free) because Size(%d) != sizeof(intSerialArray_Function_15)(%d) \n",sizeOfObject,sizeof(intSerialArray_Function_15));
             }
#endif

          free(Pointer);
        }
       else
        {
          intSerialArray_Function_15 *New_Link = (intSerialArray_Function_15*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In intSerialArray_Function_15::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(intSerialArray_Function_15)(%d) \n",Pointer,sizeOfObject,sizeof(intSerialArray_Function_15));
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
               printf ("ERROR: In intSerialArray_Function_15::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving intSerialArray_Function_15::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }


#if COMPILE_DEFERRED_DISPLAY_AND_VIEW_FUNCTIONS
int  intSerialArray_Function_16::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

intSerialArray_Function_16* intSerialArray_Function_16::Current_Link                      = NULL;

int intSerialArray_Function_16::Memory_Block_Index                = 0;

const int intSerialArray_Function_16::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *intSerialArray_Function_16::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *intSerialArray_Function_16::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call intSerialArray_Function_16::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
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

     if (Size != sizeof(intSerialArray_Function_16))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from intSerialArray_Function_16
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In intSerialArray_Function_16::operator new: Calling malloc because Size(%d) != sizeof(intSerialArray_Function_16)(%d) \n",Size,sizeof(intSerialArray_Function_16));

          return malloc(Size);
        }
       else
        {
       // printf ("In intSerialArray_Function_16::operator new: Using the pool mechanism Size(%d) == sizeof(intSerialArray_Function_16)(%d) \n",Size,sizeof(intSerialArray_Function_16));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (intSerialArray_Function_16*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(intSerialArray_Function_16) );
#else
               Current_Link = (intSerialArray_Function_16*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(intSerialArray_Function_16) ];
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

     intSerialArray_Function_16* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from intSerialArray_Function_16::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void intSerialArray_Function_16::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In intSerialArray_Function_16::operator delete: Size(%d)  sizeof(intSerialArray_Function_16)(%d) \n",sizeOfObject,sizeof(intSerialArray_Function_16));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(intSerialArray_Function_16))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from intSerialArray_Function_16
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In intSerialArray_Function_16::operator delete: Calling global delete (free) because Size(%d) != sizeof(intSerialArray_Function_16)(%d) \n",sizeOfObject,sizeof(intSerialArray_Function_16));
             }
#endif

          free(Pointer);
        }
       else
        {
          intSerialArray_Function_16 *New_Link = (intSerialArray_Function_16*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In intSerialArray_Function_16::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(intSerialArray_Function_16)(%d) \n",Pointer,sizeOfObject,sizeof(intSerialArray_Function_16));
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
               printf ("ERROR: In intSerialArray_Function_16::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving intSerialArray_Function_16::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }

#endif

int  intSerialArray_Aggregate_Operator::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

intSerialArray_Aggregate_Operator* intSerialArray_Aggregate_Operator::Current_Link                      = NULL;

int intSerialArray_Aggregate_Operator::Memory_Block_Index                = 0;

const int intSerialArray_Aggregate_Operator::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *intSerialArray_Aggregate_Operator::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *intSerialArray_Aggregate_Operator::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call intSerialArray_Aggregate_Operator::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
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

     if (Size != sizeof(intSerialArray_Aggregate_Operator))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from intSerialArray_Aggregate_Operator
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In intSerialArray_Aggregate_Operator::operator new: Calling malloc because Size(%d) != sizeof(intSerialArray_Aggregate_Operator)(%d) \n",Size,sizeof(intSerialArray_Aggregate_Operator));

          return malloc(Size);
        }
       else
        {
       // printf ("In intSerialArray_Aggregate_Operator::operator new: Using the pool mechanism Size(%d) == sizeof(intSerialArray_Aggregate_Operator)(%d) \n",Size,sizeof(intSerialArray_Aggregate_Operator));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (intSerialArray_Aggregate_Operator*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(intSerialArray_Aggregate_Operator) );
#else
               Current_Link = (intSerialArray_Aggregate_Operator*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(intSerialArray_Aggregate_Operator) ];
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

     intSerialArray_Aggregate_Operator* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from intSerialArray_Aggregate_Operator::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void intSerialArray_Aggregate_Operator::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In intSerialArray_Aggregate_Operator::operator delete: Size(%d)  sizeof(intSerialArray_Aggregate_Operator)(%d) \n",sizeOfObject,sizeof(intSerialArray_Aggregate_Operator));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(intSerialArray_Aggregate_Operator))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from intSerialArray_Aggregate_Operator
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In intSerialArray_Aggregate_Operator::operator delete: Calling global delete (free) because Size(%d) != sizeof(intSerialArray_Aggregate_Operator)(%d) \n",sizeOfObject,sizeof(intSerialArray_Aggregate_Operator));
             }
#endif

          free(Pointer);
        }
       else
        {
          intSerialArray_Aggregate_Operator *New_Link = (intSerialArray_Aggregate_Operator*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In intSerialArray_Aggregate_Operator::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(intSerialArray_Aggregate_Operator)(%d) \n",Pointer,sizeOfObject,sizeof(intSerialArray_Aggregate_Operator));
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
               printf ("ERROR: In intSerialArray_Aggregate_Operator::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving intSerialArray_Aggregate_Operator::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }


#undef INTARRAY




















