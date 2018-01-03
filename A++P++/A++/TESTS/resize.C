#if 0
Hi Dan,

the official bugreport page did not work so I will  submit them directly to
you instead.

The first bug I have found is that resize does not work with P++.

If I run the  attached file on more than 1 process I get INDEX  OUT
OF BOUNDS.

Stefan

Note from DQ: This bug has been fixed!

#endif

#define BOUNDS_CHECK
#include "A++.h"

int 
main(int argc, char** argv)
   {
     ios::sync_with_stdio();
     Index::setBoundsCheck(on);

     int Number_of_Processors = 0;
     // *wdh* 100924 
     Optimization_Manager::Initialize_Virtual_Machine((char*)"",Number_of_Processors,argc,argv);

#if 0
     doubleArray x(100,100), y(100,100), xpy(100,100);
#else
     Partitioning_Type thePartition (Range(0,0));
     doubleArray x(10,10,thePartition), y(10,10,thePartition), xpy(10,10,thePartition);
#endif

     x.setBase(5,0);
     printf ("X.getBase(0) = %d \n",x.getBase(0));

     x.resize(Range(10,29),Range(10,19));
     printf ("X.getBase(0) = %d \n",x.getBase(0));

     y.resize(20,10);
     xpy.resize(20,10);
  
     x=7;
     y=2;

     xpy = x + y*x;

  // xpy.display("xpy");

     APP_ASSERT ( max(xpy) == min(xpy) );

     if (max(xpy) != 21)
        {
          printf ("ERROR: resize function not working properly: Test Failed \n");
          APP_ABORT();
        }
       else
        {
          printf ("PASSED: Stefan's resize Test \n");
        }

#if 0
  // Dan's simpler debugging code
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

     printf ("Program Terminated Normally! \n");
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
