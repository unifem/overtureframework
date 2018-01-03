#include<A++.h>

// **************************************************************************
//                        MAIN PROGRAM FUNCTION
// **************************************************************************

int main(int argc, char** argv)
   {
     ios::sync_with_stdio();     // Syncs C++ and C I/O subsystems!
     Index::setBoundsCheck (On); // Turns on M++ array bounds checking!

     int Number_Of_Processors = 0;  // get number for default value from command line
  // printf ("Number_Of_Processors = %d \n",Number_Of_Processors);

  // Need to uncomment this line to provide the correct path within the distribution
     Optimization_Manager::Initialize_Virtual_Machine ("",Number_Of_Processors,argc,argv);

#ifdef _AIX
  // This call is made so that the Blue Pacific compiler will see the use of MPI directly
  // and then link to the appropriate library (libmpi).  mpCC determines the 
  // correct libraries to link and needs this to trigger linking to libmpi.
     MPI_Barrier(MPI_COMM_WORLD);
#endif
     
  // printf ("Run P++ code (Number_Of_Processors = %d) \n",Number_Of_Processors);

  // We want to test the APP_Unit_Range object to see if it was properly initialized
  // APP_Unit_Range.Test_Consistency("Test static initialization of APP_Unit_Range");

     Optimization_Manager::Exit_Virtual_Machine ();
     printf ("Program Terminated Normally! \n");
     return 0;
   }



