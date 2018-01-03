/*
  This program hangs on an MPI_Barrier call when adding the 2 6D arrays.
  r_ are the ranges for defining the arrays.
  i_ are the internal ranges, a subset of r_.
  Error occurs for the Tera cluster, using cxx.

// DQ Comment on this bug:
// Accessing only a smaller portion of the array or having the array 
// have odd bases for each axis causes this example to hang!
*/


#define BOUNDS_CHECK 
#include <A++.h>

int main(int argc,char** argv)
{
  int num_of_process=4;
  Optimization_Manager::Initialize_Virtual_Machine(" ",num_of_process,argc,argv);

#ifdef _AIX
  // This call is made so that the Blue Pacific compiler will see the use of MPI directly
  // and then link to the appropriate library (libmpi).  mpCC determines the 
  // correct libraries to link and needs this to trigger linking to libmpi.
     MPI_Barrier(MPI_COMM_WORLD);
#endif

  Index::setBoundsCheck(ON);    // Turn on array bounds checking.

  Index all;

#if 0
// This fails
  Range r0(0,3)         , i0(1,2)
      , r1(0,1)       , i1(0,1)
      , r2(0,1)       , i2(0,1)
      , r3(0,5)       , i3(0,5) 
      , r4(0,18)       , i4(0,18)
   // , r5(0,66)       , i5(0,66)
   // , r5(55,66)       , i5(55,66)
      ;
and
  Range r0(0,3)         , i0(1,2)
      , r1(0,1)       , i1(0,1)
      , r2(0,1)       , i2(0,1)
      , r3(0,1)       , i3(0,1) 
      , r4(0,4)       , i4(0,4)
      ;

#endif

#if 1
// Accessing only a smaller portion of the array or having the array 
// have odd bases for each axis causes this example to hang!
#if 1
  Range r0(0,3)         , i0(1,2)
      , r1(0,1)       , i1(0,1)
      , r2(0,1)       , i2(0,1)
      , r3(0,1)       , i3(0,1) 
      , r4(0,4)       , i4(0,4)
      ;
#else
  Range r0(0,3)         , i0(1,2)
      , r1(2,7)         , i1(4,5)
      , r2(34,43)       , i2(37,42)
      , r3(-42,-36)     , i3(-40,-37) 
      , r4(-4,18)       , i4(6,14)
      , r5(55,66)       , i5(58,63)
      ;
#endif
#else
  Range r0(0,5)       , i0(1,4)
      , r1(0,5)       , i1(1,4)
      , r2(0,5)       , i2(1,4)
      , r3(0,5)       , i3(1,4) 
      , r4(0,5)       , i4(1,4)
      , r5(0,5)       , i5(1,4)
      ;
#endif

#define DIM 5

#if DIM == 4
  intArray B0(r0,r1,r2,r3);
  intArray B1(r0,r1,r2,r3);
  B0 = 1;
  B1 = 1;
  B1(i0,i1,i2,i3) = B0(i0+1,i1,i2,i3);

  intSerialArray SerialX(r0,r1,r2,r3);
  intSerialArray SerialY(r0,r1,r2,r3);
  SerialX = 1;
  SerialY = 1;
  SerialX(i0,i1,i2,i3) = SerialY(i0 +1,i1,i2,i3);

  Range l0 = B0.getDomain().getLocalMaskIndex(0);
  Range l1 = B0.getDomain().getLocalMaskIndex(1);
  Range l2 = B0.getDomain().getLocalMaskIndex(2);
  Range l3 = B0.getDomain().getLocalMaskIndex(3);
  intSerialArray & localArray = *(B0.getSerialArrayPointer());
  
  if ( sum(SerialX(l0,l1,l2,l3) != localArray) != 0 )
    {  
      printf ("Failed test! \n");
      APP_ABORT();
    }
  else
    {
      printf ("Passed test! \n");
    }
#elif DIM == 5

  intArray B0(r0,r1,r2,r3,r4);
// intArray B1(r0,r1,r2,r3,r4);
  B0 = 1;
// B1 = 1;

#if 0
  // Restrict output to processor 1 only
     Communication_Manager::setOutputProcessor(1);
#endif

     B0.displayPartitioning("from test2000_29.C");

     Optimization_Manager::setForceVSG_Update(On);

  // Set reporting of message passing interpretation (for debugging)
     Diagnostic_Manager::setMessagePassingInterpretationReport(1);

     intArray & lhs = B0(i0,i1,i2,i3,i4);
     intArray & rhs = B0(i0+1,i1,i2,i3,i4);

     APP_DEBUG = 0;
//   B0(i0,i1,i2,i3,i4) = B0(i0+1,i1,i2,i3,i4);
     lhs = rhs;
     APP_DEBUG = 0;

// APP_ABORT();

  intSerialArray SerialX(r0,r1,r2,r3,r4);
  intSerialArray SerialY(r0,r1,r2,r3,r4);
  SerialX = 1;
  SerialY = 1;
  SerialX(i0,i1,i2,i3,i4) = SerialY(i0 +1,i1,i2,i3,i4);

  Range l0 = B0.getDomain().getLocalMaskIndex(0);
  Range l1 = B0.getDomain().getLocalMaskIndex(1);
  Range l2 = B0.getDomain().getLocalMaskIndex(2);
  Range l3 = B0.getDomain().getLocalMaskIndex(3);
  Range l4 = B0.getDomain().getLocalMaskIndex(4);
  intSerialArray & localArray = *(B0.getSerialArrayPointer());
  
  if ( sum(SerialX(l0,l1,l2,l3,l4) != localArray) != 0 )
    {  
      printf ("Failed test! \n");
      APP_ABORT();
    }
  else
    {
      printf ("Passed test! \n");
    }

#endif

  /*
  intArray C0(r0,r1,r2,r3,r4);
  intArray C1(r0,r1,r2,r3,r4);
  C0 = 1;
  C1(i0,i1,i2,i3,i4) = C0(i0+1,i1,i2,i3,i4) + C0(i0-1,i1,i2,i3,i4);
  */

  /*
  intArray A0(r0,r1,r2,r3,r4,r5);
  intArray A1(r0,r1,r2,r3,r4,r5);
  A0 = 1;
  A1(i0,i1,i2,i3,i4,i5) = A0(i0+1,i1,i2,i3,i4,i5) + A0(i0-1,i1,i2,i3,i4,i5);
  */


  // Call the diagnostics mechanism to display memory usage
     Diagnostic_Manager::report();

  Optimization_Manager::Exit_Virtual_Machine();
}
