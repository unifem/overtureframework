// file:  PADRE_Parti_NonTemplate.C

#ifndef PADRE_PARTI_NONTEMPLATE_C
#define PADRE_PARTI_NONTEMPLATE_C

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#if !defined(NO_Parti)

#include <PADRE_Global.h>
#include <PADRE_Parti.h>

// *****************************************************************************
//                     GLOBAL VARIABLE INITIALIZATION
// *****************************************************************************

// This is defined in P++ so we don't want to define it again here!
// extern MPI_Comm Global_PARTI_P_plus_plus_Interface_PPP_Comm_World;

// int   Global_PARTI_PADRE_Interface_Number_Of_Processors         = 0;
char* Global_PARTI_PADRE_Interface_Name_Of_Main_Processor_Group = MAIN_PROCESSOR_GROUP_NAME;

// *****************************************************************************
//                     STATIC VARIABLE INITIALIZATION
// *****************************************************************************

// These are initialized to default values that will trigger errors if they are not 
// reset properly within the initialization phase.
int    PARTI::numberOfProcessors              = -1;
VPROC* PARTI::VirtualProcessorSpace           = NULL;
int    PARTI::My_Task_ID                      = -1;
int    PARTI::Task_ID_Array[MAX_PROCESSORS];
char*  PARTI::MainProcessorGroupName          = MAIN_PROCESSOR_GROUP_NAME;
int    PARTI::My_Process_Number               = -1;

// template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
// int PARTI_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain>::
// defaultGhostCellWidth = 0;


// *****************************************************************************
//                     DEFINITION OF MEMBER FUNCTIONS
// *****************************************************************************

void PARTI::freeMemoryInUse()
   {
     if (VirtualProcessorSpace != NULL)
          delete_VPROC (VirtualProcessorSpace);
     VirtualProcessorSpace = NULL;
   }

int PARTI::localProcessNumber()
   {
     return My_Process_Number;
   }

void PARTI::SublibraryInitialization ()
{
  // here we initialize PARTI
     if (PADRE::debugLevel() > 0)
          printf("Inside PARTI::Initialization ()\n");

  // Make sure the array dimensions agree with PARTI.
     if( PADRE_MAX_ARRAY_DIMENSION != PARTI_MAX_ARRAY_DIMENSION ) {
       printf ("ERROR: PADRE_MAX_ARRAY_DIMENSION = %d != PARTI_MAX_ARRAY_DIMENSION = %d \n",
               PADRE_MAX_ARRAY_DIMENSION,PARTI_MAX_ARRAY_DIMENSION);
     }
     PADRE_ASSERT( PADRE_MAX_ARRAY_DIMENSION == PARTI_MAX_ARRAY_DIMENSION );

 // printf ("############# setup number of processors and VirtualProcessorSpace \n");

// #if defined(PARALLEL_PADRE)
#if 0
     FFlag = 1; // so PARTI treats all arrays as FORTRAN ordered arrays (required)
#endif

#if defined(PVM)
     myproc(My_Process_Number);  // initialize the PARTI library (we call the global myprog)
#endif

     MPI_Comm_dup(PADRE::MPICommunicator(), &Global_PARTI_PADRE_Interface_PADRE_Comm_World);
#if 1
     My_Process_Number  = PARTI_myproc();
     numberOfProcessors = PARTI_MPI_numprocs();
#else
     My_Process_Number  = 0;
     numberOfProcessors = 1;
#endif
     if (PADRE::debugLevel() > 0)
          printf ("This processor = %d numberOfProcessors = %d \n",My_Process_Number,numberOfProcessors);

#if 1
  // Bugfix (6/11/95) fixed bug in translation from old version of comm_man.C to
  // new version of comm_man.C (with ability to switch communication libraries PVM and MPI)
     Global_PARTI_PADRE_Interface_Number_Of_Processors = numberOfProcessors;

     PADRE_ASSERT(Global_PARTI_PADRE_Interface_Number_Of_Processors == numberOfProcessors);
     PADRE_ASSERT(Global_PARTI_PADRE_Interface_Number_Of_Processors > 0);
     PADRE_ASSERT(Global_PARTI_PADRE_Interface_Name_Of_Main_Processor_Group != NULL);
#endif
// #endif
      
     PADRE_ASSERT (numberOfProcessors > 0);
     PADRE_ASSERT (numberOfProcessors <= MAX_PROCESSORS);

  // Make sure that Virtual Processor Spaces is defined
     if (VirtualProcessorSpace == NULL) 
        {
          if (PADRE::debugLevel() > 0)
               printf ("Build virtual processor space! \n");

       // Block Parti only implemented with 1D virtual processor spaces
          int Sizes[1];
          Sizes[0] = numberOfProcessors;
          VirtualProcessorSpace = vProc(1,Sizes);
        }
       else 
        {
          if (PADRE::debugLevel() > 0)
               printf ("Virtual Processor Space ALREADY BUILT! \n");
        } // if

     if (PADRE::debugLevel() > 0)
          printf ("(Virtual processor Space size) - numberOfProcessors = %d \n", numberOfProcessors);
  
     PADRE_ASSERT(VirtualProcessorSpace != NULL);
   } // void PARTI::SublibraryInitialization ()


bool PARTI::isPARTIInitialized ()
   {
  // We might want a stronger test than this
     PADRE_ASSERT (numberOfProcessors > 0);
     PADRE_ASSERT (VirtualProcessorSpace != NULL);
  // PADRE_ASSERT (My_Task_ID > 0);
     PADRE_ASSERT (MainProcessorGroupName != NULL);
     PADRE_ASSERT (My_Process_Number >= 0);
     PADRE_ASSERT (VirtualProcessorSpace != NULL);
     return isParallelMachineInitialized();
   }

void PARTI::
testConsistency ( const char *Label ) const
   {
  // cout << "Inside of PARTI::testConsistency (" << Label << ") \n";

  // This seems to be a reasonable way to check the consistency
     PADRE_ASSERT (isPARTIInitialized() == true);
   }





#endif	// !defined(NO_Parti)

#endif	// PADRE_PARTI_NONTEMPLATE_C


