// USE_PPP is set in config.h depending upon if we are using P++ or not!
// define USE_PPP
//#define TEST_INDIRECT

//define TEST_ARRAY_OVERFLOW
//================================================================
// A++ library test P++ code.  Version 1.2.9 
//================================================================

//#define BOUNDS_CHECK

#include<A++.h>
#include<iostream>
using namespace std;

#define TYPE double
#define TYPEArray doubleArray
#define TYPESerialArray doubleSerialArray

#if 0
extern "C"
{
   int getpid();
}
#endif

#if 0
// The IBM can't evaluate this test properly (ARCH == xyz) is always TRUE!
// So we just comment out the test for now
#if (APP_ARCH == solaris && APP_CXX_Compiler == CC)
// Run this test only on solaris systems where the use of #error 
// is not more than a warning at compile time
#if defined(USE_PPP)
// Test the USE_PPP macro to verify that it is a working mechanism for users
// to know if they are using A++ or P++ within an application.
#error "CORRECT: USE_PPP IS DEFINED (tested preprocessor macro done only on Solaris with CC)!"
#else
#error "ERROR: USE_PPP NOT DEFINED (tested preprocessor macro done only on Solaris with CC)!"
#endif
#endif

// Comment out the entire test
#endif

#if 0
#if defined(PPP)
#error "PPP DEFINED!"
#else
#error "PPP NOT DEFINED!"
#endif
#endif

//Range APP_Unit_Range(0,0);

int main(int argc, char** argv)
{
  MemoryManagerType  Memory_Manager;

  //Index::setBoundsCheck(on);

  // ... this is an attempt to put a stop in the code for mpi ...
  int pid_num = getpid();
  cout<<"pid="<<pid_num<<endl;

#if 0
  int wait1;
  cout<<"Type first number to continue."<<endl;
  cin >> wait1 ;
#endif

  //================= P++ startup =============================

  printf("This is the new testppp, with 2 processors. \n");

  // ... leave this in with A++ also for debugging ...

  int Number_Of_Processors = 1;

  Optimization_Manager::Initialize_Virtual_Machine
     ("", Number_Of_Processors,argc,argv);
  if ( 0 && !Internal_Partitioning_Type::isFloatTypeSupported() ) {
    cerr << "************* WARNING: not running this example! *************\n"
	 << "* This example requires support for float type which is not\n"
	 << "* supported under this PADRE configuration.\n"
	 << flush;
    Optimization_Manager::Exit_Virtual_Machine();
    exit(0);
  }

  printf("Run P++ code(Number Of Processors=%d) \n",
    Number_Of_Processors);

  Diagnostic_Manager::setTrackArrayData();
  APP_ASSERT (Diagnostic_Manager::getTrackArrayData() == TRUE);

  //===========================================================

// This call is made so that the Blue Pacific compiler will see the use of MPI directly
// and then link to the appropriate library (libmpi).  mpCC determines the 
// correct libraries to link and needs this to trigger linking to libmpi.
  MPI_Barrier(MPI_COMM_WORLD);
  
#if 0
  //int pid_num = getpid();
  pid_num = getpid();
  cout<<"pid="<<pid_num<<endl;

#if defined(MPI)
  // ... this is an attempt to put a stop in the code for mpi for debugging
  //  but this isn't necessary and doesn't work for pvm ...
  int wait0;
  cout<<"Type first number to continue."<<endl;
  cin >> wait0 ;
#endif
#endif

  int ghost_cell_width = 0;
  //int ghost_cell_width = 1;
  Partitioning_Type::SpecifyDefaultInternalGhostBoundaryWidths
     (ghost_cell_width);

  Partitioning_Type *Single_Processor_Partition 
    = new Partitioning_Type(Range(0,0));
  
  //============================================================
  // ... test reshape, reference and resize bugs reprorted by Bill ...

  // ... this tests reshaping a 2 x 2 x 2 x 2 array into a 4 x 1 x 2 x 2
  //   array ...
  TYPEArray xx(Range(0,1),Range(0,1),Range(0,1),Range(2,3));
  Partitioning_Type* partition = new Partitioning_Type();
  int ghost_boundary_width = 0;
  partition->partitionAlongAxis(0,FALSE,ghost_boundary_width);
  partition->partitionAlongAxis(1,FALSE,ghost_boundary_width);
  partition->partitionAlongAxis(2,FALSE,ghost_boundary_width);
  partition->partitionAlongAxis(3,TRUE,1);
  xx.partition(*partition);
  xx=3.;
  xx.reshape(Range(0,3),Range(0,0),Range(0,1),Range(0,1));

  cout<<"Reshape test 1 okay"<<endl;


  // ... this test whether an array gets added to the list associated
  //  with its partition correctly by reference.  If there is an error
  //  it might not show up until cleanup at end ...

  Partitioning_Type* P = new Partitioning_Type(Number_Of_Processors);

  int numberOfGhostPoints=0;  // ******************************

  int kd;
  for (kd=0; kd<2; kd++)
  {
     P->partitionAlongAxis(kd, TRUE, numberOfGhostPoints);
  }
  for (kd=2; kd<4; kd++)
  {
     P->partitionAlongAxis(kd, FALSE, 0);
  }

  TYPEArray aa(3,3,2,2,*P),bb;
  //TYPEArray aa(3,3,2,2),bb;
  bb.reference(aa);
  aa = 1.;
  bb.reference(aa);

  cout<<"Reference with partition test1 okay"<<endl;


  // ... test resize with a partition before Bill finds this bug ...
  aa.resize(3,3,4);

  // ... if code doesn't crash it is probably working ...
  cout<<"resize with partition test1 okay"<<endl;

  delete partition;
  delete P;

  //============================================================
  // ... this code is slow so the user might want to turn it off ...
#if defined(TEST_ARRAY_OVERFLOW)
  // ... use modified Bill H.'s memory leak code ...

  int num_arrays,prev_number;
  int first_time = TRUE;
  int all_ok = TRUE;

  for (i=0;i<400;i++)
  {
    TYPEArray x(10),y;
    x = 1.;
    y = x*2.;
    num_arrays = Array_Descriptor_Type::getMaxNumberOfArrays();
    if (!first_time)
    {
      if (prev_number != num_arrays) 
      {
	 cout<<"ERROR: number of Array_ID's increasing at iter "<<i<<endl;
	 all_ok = FALSE;
      }
    }
    prev_number = num_arrays;
  }
  if (all_ok) cout<<"memory leak test okay"<<endl;
#endif 


  //============================================================


  delete Single_Processor_Partition;
  //================= P++ finish ==============================

  printf ("Program Terminated Normally! \n");

  // ... leave these 2 lines in for A++ also for debugging ...

// Call the diagnostics mechanism to display memory usage
  Diagnostic_Manager::report();

  Optimization_Manager::Exit_Virtual_Machine ();
  printf ("Virtual Machine exited! \n");

  //===========================================================

  return (0);

}



