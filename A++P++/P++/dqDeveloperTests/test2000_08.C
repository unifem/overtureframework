// A++/P++ Problem Report 0-04-11-10-51-13: Indirect addressing in where statement fails

#define BOUNDS_CHECK

#include <A++.h>

int
main( int argc, char *argv[])
   {
#if 1
     Index::setBoundsCheck(on);
#endif

     int Number_Of_Processors = 0;
     Optimization_Manager::Initialize_Virtual_Machine ("", Number_Of_Processors, argc, argv);

#ifdef _AIX
  // This call is made so that the Blue Pacific compiler will see the use of MPI directly
  // and then link to the appropriate library (libmpi).  mpCC determines the 
  // correct libraries to link and needs this to trigger linking to libmpi.
     MPI_Barrier(MPI_COMM_WORLD);
#endif

     int x, y, z;
     x = 0;
     y = 1;
     z = 1;

     float  a = 1;
     double b = 2;

     intArray X(10);
     intArray Y(10);
     intArray Z(10);
     X = x;
     Y = y;
     Z = z;

     floatArray  A(10);
     doubleArray B(10);
     A = a;
     B = b;

  // ********************************************************************
  // These test for existance but not correctness (still have to do this)
  // ********************************************************************

     printf ("Test Conversion Operators! \n");

  // Conversion operators
     X = Y.convertTo_intArray();
     x = (int) y;

     if (sum(X != x) != 0)
        {
          printf ("ERROR: X intArray Y intArray X = Y.convertTo_intArray() Test Failed \n");
          APP_ABORT();
        }
       else
        {
          printf ("PASSED: X intArray Y intArray X = Y.convertTo_intArray() Test \n");
        }

     X = A.convertTo_intArray();
     x = (int) a;

     if (sum(X != x) != 0)
        {
          printf ("ERROR: X intArray A floatArray X = A.convertTo_intArray() Test Failed \n");
          APP_ABORT();
        }
       else
        {
          printf ("PASSED: X intArray Y floatArray X = A.convertTo_intArray() Test \n");
        }

     X = B.convertTo_intArray();
     x = (int) b;

     if (sum(X != x) != 0)
        {
          printf ("ERROR: X intArray B doubleArray X = B.convertTo_intArray() Test Failed \n");
          APP_ABORT();
        }
       else
        {
          printf ("PASSED: X intArray B doubleArray X = B.convertTo_intArray() Test \n");
        }

     A = A.convertTo_floatArray();
     a = (float) a;

     if (sum(A != a) != 0)
        {
          printf ("ERROR: A floatArray A floatArray A = A.convertTo_floatArray() Test Failed \n");
          APP_ABORT();
        }
       else
        {
          printf ("PASSED: A floatArray A floatArray A = A.convertTo_floatArray() Test \n");
        }

     A = B.convertTo_floatArray();
     a = (float) b;

     if (sum(A != a) != 0)
        {
          printf ("ERROR: A floatArray B doubleArray A = B.convertTo_floatArray() Test Failed \n");
          APP_ABORT();
        }
       else
        {
          printf ("PASSED: A floatArray A doubleArray A = A.convertTo_floatArray() Test \n");
        }

     A = X.convertTo_floatArray();
     a = (float) x;

     if (sum(A != a) != 0)
        {
          printf ("ERROR: A floatArray B intArray A = B.convertTo_floatArray() Test Failed \n");
          APP_ABORT();
        }
       else
        {
          printf ("PASSED: A floatArray A intArray A = A.convertTo_floatArray() Test \n");
        }

     B = A.convertTo_doubleArray();
     b = (double) a;

     if (sum(B != b) != 0)
        {
          printf ("ERROR: B doubleArray A floatArray B = A.convertTo_doubleArray() Test Failed \n");
          APP_ABORT();
        }
       else
        {
          printf ("PASSED: B doubleArray A floatArray B = A.convertTo_doubleArray() Test \n");
        }

     B = B.convertTo_doubleArray();
     b = (double) b;

     if (sum(B != b) != 0)
        {
          printf ("ERROR: B doubleArray B doubleArray B = B.convertTo_doubleArray() Test Failed \n");
          APP_ABORT();
        }
       else
        {
          printf ("PASSED: B doubleArray B doubleArray B = B.convertTo_doubleArray() Test \n");
        }

     B = X.convertTo_doubleArray();
     b = (double) x;

     if (sum(B != b) != 0)
        {
          printf ("ERROR: B doubleArray X intArray B = X.convertTo_doubleArray() Test Failed \n");
          APP_ABORT();
        }
       else
        {
          printf ("PASSED: B doubleArray X intArray B = X.convertTo_doubleArray() Test \n");
        }

  // Ones Complement operator
     X = ~Y;
     x = ~y;

     if (sum(X != x) != 0)
        {
          printf ("ERROR: X = ~Y Test Failed \n");
          APP_ABORT();
        }
       else
        {
          printf ("PASSED: X = ~Y Test \n");
        }

  // Bitwise array operators
     X = Y | Z;
     x = y | z;

     if (sum(X != x) != 0)
        {
          printf ("ERROR: X = Y | Z Test Failed \n");
          APP_ABORT();
        }
       else
        {
          printf ("PASSED: X = Y | Z Test \n");
        }

     X = Y & Z;
     x = y & z;

     if (sum(X != x) != 0)
        {
          printf ("ERROR: X = Y & Z Test Failed \n");
          APP_ABORT();
        }
       else
        {
          printf ("PASSED: X = Y & Z Test \n");
        }

     X = Y ^ Z;
     x = y ^ z;

     if (sum(X != x) != 0)
        {
          printf ("ERROR: X = Y ^ Z Test Failed \n");
          APP_ABORT();
        }
       else
        {
          printf ("PASSED: X = Y ^ Z Test \n");
        }

     X = Y << Z;
     x = y << z;

     if (sum(X != x) != 0)
        {
          printf ("ERROR: X = Y << Z Test Failed \n");
          APP_ABORT();
        }
       else
        {
          printf ("PASSED: X = Y << Z Test \n");
        }

     X = Y >> Z;
     x = y >> z;

     if (sum(X != x) != 0)
        {
          printf ("ERROR: X = Y >> Z Test Failed \n");
          APP_ABORT();
        }
       else
        {
          printf ("PASSED: X = Y >> Z Test \n");
        }


  // self modifying operator with array
     X |= Y;
     x |= y;

     if (sum(X != x) != 0)
        {
          printf ("ERROR: X |= Y Test Failed \n");
          APP_ABORT();
        }
       else
        {
          printf ("PASSED: X |= Y Test \n");
        }

     X &= Y;
     x &= y;

     if (sum(X != x) != 0)
        {
          printf ("ERROR: X &= Y Test Failed \n");
          APP_ABORT();
        }
       else
        {
          printf ("PASSED: X &= Y Test \n");
        }

     X ^= Y;
     x ^= y;

     if (sum(X != x) != 0)
        {
          printf ("ERROR: X ^= Y Test Failed \n");
          APP_ABORT();
        }
       else
        {
          printf ("PASSED: X ^= Y Test \n");
        }


  // self modifying operator with scalar
     X |= 1;
     x |= 1;

     if (sum(X != x) != 0)
        {
          printf ("ERROR: X |= 1 Test Failed \n");
          APP_ABORT();
        }
       else
        {
          printf ("PASSED: X |= 1 Test \n");
        }

     X &= 1;
     x &= 1;

     if (sum(X != x) != 0)
        {
          printf ("ERROR: X &= 1 Test Failed \n");
          APP_ABORT();
        }
       else
        {
          printf ("PASSED: X &= 1 Test \n");
        }

     X ^= 1;
     x ^= 1;

     if (sum(X != x) != 0)
        {
          printf ("ERROR: X ^= 1 Test Failed \n");
          APP_ABORT();
        }
       else
        {
          printf ("PASSED: X ^= 1 Test \n");
        }


  // Mixed array scalar operators
     X = Y | 1;
     x = y | 1;

     if (sum(X != x) != 0)
        {
          printf ("ERROR: X = Y | 1 Test Failed \n");
          APP_ABORT();
        }
       else
        {
          printf ("PASSED: X = Y | 1 Test \n");
        }

     X = Y & 1;
     x = y & 1;

     if (sum(X != x) != 0)
        {
          printf ("ERROR: X = Y & 1 Test Failed \n");
          APP_ABORT();
        }
       else
        {
          printf ("PASSED: X = Y & 1 Test \n");
        }

     X = Y ^ 1;
     x = y ^ 1;

     if (sum(X != x) != 0)
        {
          printf ("ERROR: X = Y ^ 1 Test Failed \n");
          APP_ABORT();
        }
       else
        {
          printf ("PASSED: X = Y ^ 1 Test \n");
        }

     X = Y << 1;
     x = y << 1;

     if (sum(X != x) != 0)
        {
          printf ("ERROR: X = Y << 1 Test Failed \n");
          APP_ABORT();
        }
       else
        {
          printf ("PASSED: X = Y << 1 Test \n");
        }

     X = Y >> 1;
     x = y >> 1;

     if (sum(X != x) != 0)
        {
          printf ("ERROR: X = Y >> 1 Test Failed \n");
          APP_ABORT();
        }
       else
        {
          printf ("PASSED: X = Y >> 1 Test \n");
        }

  // Mixed scalar array operators
     X = 1 | Z;
     x = 1 | z;

     if (sum(X != x) != 0)
        {
          printf ("ERROR: X = 1 | Z Test Failed \n");
          APP_ABORT();
        }
       else
        {
          printf ("PASSED: X = 1 | Z Test \n");
        }

     X = 1 & Z;
     x = 1 & z;

     if (sum(X != x) != 0)
        {
          printf ("ERROR: X = 1 & Z Test Failed \n");
          APP_ABORT();
        }
       else
        {
          printf ("PASSED: X = 1 & Z Test \n");
        }

     X = 1 ^ Z;
     x = 1 ^ z;

     if (sum(X != x) != 0)
        {
          printf ("ERROR: X = 1 ^ Z Test Failed \n");
          APP_ABORT();
        }
       else
        {
          printf ("PASSED: X = 1 ^ Z Test \n");
        }

     X = 1 << Z;
     x = 1 << z;

     if (sum(X != x) != 0)
        {
          printf ("ERROR: X = 1 << Z Test Failed \n");
          APP_ABORT();
        }
       else
        {
          printf ("PASSED: X = 1 << Z Test \n");
        }

     X = 1 >> Z;
     x = 1 >> z;

     if (sum(X != x) != 0)
        {
          printf ("ERROR: X = 1 >> Z Test Failed \n");
          APP_ABORT();
        }
       else
        {
          printf ("PASSED: X = 1 >> Z Test \n");
        }

  // Call the diagnostics mechanism to display memory usage
     Diagnostic_Manager::report();

     printf ("Program Terminated Normally! \n");

     Optimization_Manager::Exit_Virtual_Machine();
     return 0;
   }

