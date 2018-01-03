// *****************************************************************
// BUG ALERT --- BUG ALERT --- BUG ALERT --- BUG ALERT --- BUG ALERT
// *****************************************************************
// BUG DISCRIPTION: A bug in the Sun C++ compiler was found which
// is particularly difficult to figure out. ALL template class member
// function not defined in the header file MUST be declared before any
// of the member function defined in the header file!!!!!  If this is not
// done then the compiler will not search the *.C file and will not instantiate
// the template function.  The result is that templated member function will not
// be found at link time of any application requiring the templated class's member
// function.
// *****************************************************************
// BUG ALERT --- BUG ALERT --- BUG ALERT --- BUG ALERT --- BUG ALERT
// *****************************************************************

// file: PADRE_Kelp.h

// This specifies the maximum array dimension that KELP can handle 
// (this is presently 4 -- but we have the code (untested) that 
// would allow it to be user defined at compile time).

#ifndef PADRE_Kelph
#define PADRE_Kelph

#include "../KELP/kelp1.2/kelp/include/kelp.h"

#define KELP_MAX_ARRAY_DIMENSION 4
#define FLOORPLAN FloorPlan4
#define XARRAY XArray4
#define GRID Grid4
#define MOVER Mover4
#define MOTIONPLAN MotionPlan4
#define REGION Region4
#define POINT Point4

// This variable provides a means of communicating the Number of processors and 
// a common name to use in referencing the PVM group of processors to the lower 
// level parallel library.

extern int   Global_KELP_PADRE_Interface_Number_Of_Processors;
extern char* Global_KELP_PADRE_Interface_Name_Of_Main_Processor_Group;

class KELP
{
private:
  KELP ( const KELP & X );
  KELP & operator= ( const KELP & X );
public:
 ~KELP ();
  KELP ();

  static bool isKELPInitialized ();


  // KELP must know the number of processors internally

  static int numberOfProcessors;


  // KELP needs this: array of virtual processors (processor space)

  // ... WARNING: IS THIS BLOCK PARTI? ...
  // //static VPROC *VirtualProcessorSpace;


  // the PVM interface needs these

  static int My_Task_ID;

  static char* MainProcessorGroupName;

  static int Task_ID_Array[MAX_PROCESSORS]; // array of task id


  // This variable is used within the Sync member function to
  // determine if the parallel machine has been initialized.
  // It is default initialized to be -1 and if it is -1 then
  // the parallel machine has NOT been initialized.  If it is
  // a valid processor number (greater than 0) then the parallel
  // machine has been initialized.
  // at a later point we should have a static member function that
  // determines if the parallel machine has been initialized.

  static int My_Process_Number;             // my process number

  static void SublibraryInitialization ();

  static int localProcessNumber();

  static bool isParallelMachineInitialized() 
     { return (My_Process_Number != -1); }

  // Tests the internal consistency of the object
  void testConsistency ( const char *Label = "" ) const;

  static void freeMemoryInUse ();

  friend ostream & operator<< (ostream & os, const KELP & X)
  {
    os << "{KELP:  "
       << "isKELPInitialized() = " << X.isKELPInitialized()
       << ", numberOfProcessors = " << KELP::numberOfProcessors
       << ", My_Task_ID = " << KELP::My_Task_ID << endl
       << ", My_Process_Number = " << KELP::My_Process_Number
       << ", localProcessNumber() = " << X.localProcessNumber() << endl
       << ", isParallelMachineInitialized() = " 
       << KELP::isParallelMachineInitialized()
       << "}" << endl;
    return os;
  }

}; // end class KELP

//========================================================================

#include "PADRE_Kelp_Distribution.h"
#include "PADRE_Kelp_Representation.h"
#include "PADRE_Kelp_Descriptor.h"

//========================================================================

#endif // PADRE_Kelph
