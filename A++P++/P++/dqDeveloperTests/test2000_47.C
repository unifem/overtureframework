/*
DATE:               09:29:33 11/09/0
VISITOR:            Stefan Nilsson
EMAIL:              nilsson2@llnl.gov
CATEGORY:           A++/P++
CLASS:              Software bug
SEVERITY:           serious
SUMMARY:           
Problem with indirect adressing of SerialArrays in P++
ENVIRONMENT:        
Sun,CC-4.2,/usr/casc/overture/A++P++/A++P++-DATE-00-10-10-TIME-13-33/SUN_CC/NODEBUG/A++P++/P++/lib/solaris_cc_CC/
DESCRIPTION:        
P++ crashes for a simple indirect adressing case with the message:
Assertion failed: referenceCount <= getReferenceCountBase(), file array.C, line 23642
signal ABRT (Abort) in _kill at 0xfef982c4
0xfef982c4: _kill+0x0008:       bgeu    _kill+0x30
Current function is main
   33     return 0;

This is a new bug. It does not occur when I use P++ version 0.7.5b (without PADRE)
HOW TO REPEAT:      
Run the supplied test code
TEST CODE:          
*/


#include "A++.h"

int 
main(int argc, char** argv)
{
  ios::sync_with_stdio();

  int Number_of_Processors=0;
  
  Optimization_Manager::Initialize_Virtual_Machine("",Number_of_Processors,argc,argv);

#ifdef _AIX
  // This call is made so that the Blue Pacific compiler will see the use of MPI directly
  // and then link to the appropriate library (libmpi).  mpCC determines the 
  // correct libraries to link and needs this to trigger linking to libmpi.
     MPI_Barrier(MPI_COMM_WORLD);
#endif

  Index::setBoundsCheck(on);

  intSerialArray x(10,10);
  
  intSerialArray i_index(3), j_index(3);
  
  i_index(0) = 3;
  j_index(0) = 4;
  i_index(1) = 4;
  j_index(1) = 5;
  i_index(2) = 5;
  j_index(2) = 6;

  x = 1;
  
  intSerialArray result(3);
  result = x(i_index,j_index);
  result.display();
  
  Optimization_Manager::Exit_Virtual_Machine();

  return 0;
}


