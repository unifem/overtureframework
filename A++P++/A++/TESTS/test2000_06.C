// Problem Report 99-10-26-11-57-04

#define BOUNDS_CHECK
#include <A++.h>

// David Brown's display function
// #include "Display.h"

int
main (int args, char **argv)
   {
     Index::setBoundsCheck(on);
  // ...Synchronize C++ and C I/O Subsystems
     ios::sync_with_stdio();

#if 0
  const int N0 = 11, N1 = 5;
  
  doubleArray acs(N0,N1,1), asl(N0,N1,1), aslm(N0,N1,1), cs(N0,N1,1), result(N0,N1,1), temp(N0,N1,1);
  
  cs = -1.0;
  acs = 10.;
  asl = 3.;
  aslm = 1.;
  
  cout << endl << endl << "=========DLB Bug report==================================" << endl;
  
  cout << "test sign(cs,min(acs,asl,aslm))" << endl;
  cout << "sign(*,min) bug test; the correct answer is -1.0" << endl << endl << endl;
  
  cs.display ("here is cs");
  acs.display ("here is acs");
  asl.display ("here is asl");
  aslm.display ("here is aslm");
  
  result = sign(cs, min(acs, asl, aslm).display("result of min in sign") );
  cout << endl << endl << "=============WRONG ANSWER====================" << endl;
  result.display ("result of sign(*,min)");
  temp = min(acs, asl, aslm);
  temp.display ("result of min only");
  result = sign(cs, temp);
  cout << endl << endl << "=============RIGHT ANSWER====================" << endl;
  result.display ("result of min followed by sign");
#endif

// Test a non commutative function (sign, pow, fmod, mod, atan2)!
#define FUNCTION pow

     doubleArray result_A(10);
     doubleArray result_B(10);
     doubleArray temp(10);
     doubleArray X(10);
     doubleArray Y(10);
     doubleArray Z(10);

     X =  2;
     Y =  3;
     Z =  5;

     result_A = FUNCTION ( Z , min(X, Y) );
     temp = min(X, Y);
     result_B = FUNCTION ( Z , temp );

     result_A.display("result_A");
     temp.display("temp");
     result_B.display("result_B");

     if ( sum (result_A != result_B) != 0)
        {
          printf ("ERROR: array to array test -- incorrect results! \n");
          APP_ABORT();
        }
       else
        {
          printf ("PASSED: Function Call (pow) array to array test. \n");
        }

     result_A = FUNCTION ( 5 , min(X, Y) );
     temp = min(X, Y);
     result_B = FUNCTION ( 5 , temp );

     result_A.display("result_A");
     temp.display("temp");
     result_B.display("result_B");

     if ( sum (result_A != result_B) != 0)
        {
          printf ("ERROR: scalar to array test -- incorrect results! \n");
          APP_ABORT();
        }
       else
        {
          printf ("PASSED: Function Call (pow) scalar to array test. \n");
        }

     result_A = FUNCTION ( min(X, Y), 5 );
     temp = min(X, Y);
     result_B = FUNCTION ( temp, 5 );

     result_A.display("result_A");
     temp.display("temp");
     result_B.display("result_B");

     if ( sum (result_A != result_B) != 0)
        {
          printf ("ERROR: array to scalar test -- incorrect results! \n");
          APP_ABORT();
        }
       else
        {
          printf ("PASSED: Function Call (pow) array to scalar test. \n");
        }

#if 0
     result_A = FUNCTION ( min(X, Y), Z );
     temp = min(X, Y);
     result_B = FUNCTION ( temp, Z );

     result_A.display("result_A");
     result_B.display("result_B");

     if ( sum (result_A != result_B) != 0)
        {
          printf ("ERROR: array to array test -- incorrect results! \n");
          APP_ABORT();
        }
       else
        {
          printf ("PASSED: Function Call (pow) array to array test. \n");
        }
#endif

#if 0
     result_A = FUNCTION ( 4 , min(X, Y) );
     temp = min(X, Y);
     result_B = FUNCTION ( 4 , temp );

     if ( sum (result_A != result_B) != 0)
        {
          printf ("ERROR: scalar to array test -- incorrect results! \n");
          APP_ABORT();
        }
       else
        {
          printf ("PASSED: Function Call (sign) scalar to array test. \n");
        }

     result_A = FUNCTION ( min(X, Y), -1 );
     temp = min(X, Y);
     result_B = FUNCTION ( temp, -1 );

     if ( sum (result_A != result_B) != 0)
        {
          printf ("ERROR: array to scalar test -- incorrect results! \n");
          APP_ABORT();
        }
       else
        {
          printf ("PASSED: Function Call (sign) array to scalar test. \n");
        }
#endif

     printf ("Program Terminated Normally! \n");
     return 0;
   }


