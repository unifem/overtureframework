// Test memory leak in P++

#include <A++.h>

static int DEBUG = 1;

int
main(int argc, char** argv)
   {
     ios::sync_with_stdio();     // Syncs C++ and C I/O subsystems!
     Index::setBoundsCheck (On); // Turns on M++ array bounds checking!

     int Number_Of_Processors = 0;  // get number for default value from command line
     Optimization_Manager::Initialize_Virtual_Machine("",Number_Of_Processors,argc,argv);

#ifdef _AIX
  // This call is made so that the Blue Pacific compiler will see the use of MPI directly
  // and then link to the appropriate library (libmpi).  mpCC determines the 
  // correct libraries to link and needs this to trigger linking to libmpi.
     MPI_Barrier(MPI_COMM_WORLD);
#endif

     Range I1(0,13),I2(1,12);

     doubleArray u(I1);
     u(I1) = 1.0;
  
     int iter=0;
     for(iter=0; iter < 4; iter++)
        {
          if( DEBUG > 0 && Communication_Manager::localProcessNumber() == 0 )
             {
               cout<<" iteration "<<iter<<", number of arrays in use before assignment: ";
               cout<<Diagnostic_Manager::getNumberOfArraysInUse()<<endl;
             }

       // u(I2) = u(I2-1)+u(I2+1);
          u = u + 1;

          if( DEBUG>0 && Communication_Manager::localProcessNumber() == 0 )
             {
               cout<<" iteration "<<iter<<", number of arrays in use after assignment: ";
               cout<<Diagnostic_Manager::getNumberOfArraysInUse()<<endl;
             }
        }
  
     if( DEBUG>0 && Communication_Manager::localProcessNumber() == 0 )
        {
          cout<<"Number of arrays in use after iteration loop: ";
          cout<<Diagnostic_Manager::getNumberOfArraysInUse()<<endl;
        }

     Optimization_Manager::Exit_Virtual_Machine ();
     printf ("Program Terminated Normally! \n");
     return 0;
   }


