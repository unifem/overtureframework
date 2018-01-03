#if 0
Hi Dan,
The official bugreport page didn't work so I'll  submit them directly to
you instead. The first bug I ve found is that resize does not work with P++.
If I run the  attached file on more than 1 process I get INDEX OUT
OF BOUNDS.

Stefan
#endif
#include "A++.h"

int 
main(int argc, char** argv)
   {
     ios::sync_with_stdio();

  // ... this is an attempt to put a stop in the code for mpi ...
     int pid_num = getpid();
     cout<<"pid="<<pid_num<<endl;

#if 0
     int wait1;
     cout << "Type first number to continue."<<endl;
     cin >> wait1 ;
#endif

     Index::setBoundsCheck(on);

     int Number_of_Processors=0;

     Optimization_Manager::Initialize_Virtual_Machine("",Number_of_Processors,argc,argv);

#ifdef _AIX
  // This call is made so that the Blue Pacific compiler will see the use of MPI directly
  // and then link to the appropriate library (libmpi).  mpCC determines the 
  // correct libraries to link and needs this to trigger linking to libmpi.
     MPI_Barrier(MPI_COMM_WORLD);
#endif

#if 0
     pid_num = getpid();
     cout<<"pid="<<pid_num<<endl;

#if defined(MPI)
  // ... this is an attempt to put a stop in the code for mpi for debugging
  // but this isn't necessary and doesn't work for pvm ...
     int wait0;
     cout << "Type first number to continue."<<endl;
     cin >> wait0 ;
#endif
#endif

#if 0
  // Original code
     doubleArray x(100,100), y(100,100), xpy(100,100);
  
     x.resize(200,100);
     y.resize(200,100);
     xpy.resize(200,100);
  
     x=100;
     y=1;
  
     xpy = x + y*x;
#else

     doubleArray x(10,10), y(10,10); // , xpy(10,10);

     x.resize(Range(0,20),Range(0,10));
     y.resize(Range(0,20),Range(0,10));

     printf ("Assign values to arrays");
     x=1;
     y=2;

     printf ("Assign x = y");
     APP_DEBUG = 0;
     x = y;
     APP_DEBUG = 0;

#endif
  
  // Call the diagnostics mechanism to display memory usage
     Diagnostic_Manager::report();

     Optimization_Manager::Exit_Virtual_Machine();

     return 0;
   }















#if 0
     Communication_Manager::Sync();
     if (Communication_Manager::localProcessNumber() == 0)
        {
          printf ("Processor 0 data \n");
          y.Array_Descriptor.display("y.Array_Descriptor");
          y.Array_Descriptor.SerialArray->Array_Descriptor.display("y.SerialArray");
        }

     Communication_Manager::Sync();
     printf ("################################################ \n");
     printf ("################################################ \n");
     printf ("################################################ \n");
     printf ("################################################ \n");
     if (Communication_Manager::localProcessNumber() == 1)
        {
          printf ("Processor 1 data \n");
          y.Array_Descriptor.display("y.Array_Descriptor");
          y.Array_Descriptor.SerialArray->Array_Descriptor.display("y.SerialArray");
        }
     Communication_Manager::Sync();

     printf ("Assign x = y");
  // xpy = x + y*x;
     x = y;
#endif
