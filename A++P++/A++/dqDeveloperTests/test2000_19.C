// Example code to test the construction of array objects with 
// non unit strides.

#define BOUNDS_CHECK
#include<A++.h>

int
main(int argc, char** argv)
   {
     Index::setBoundsCheck (On); // Turns on A++/P++ array bounds checking!

     floatArray A (10);

     int numberOfIterations = 5;
     int i=0;

     APP_ASSERT (Diagnostic_Manager::getNumberOfArraysInUse() == 1);

  // Test for valid non Null Array
     for (i=0; i < numberOfIterations; i++)
        {
       // printf ("Number of Arrays = %d \n",Diagnostic_Manager::getNumberOfArraysInUse());
          A.redim(10);
        }
     APP_ASSERT (Diagnostic_Manager::getNumberOfArraysInUse() == 1);

  // Test for valid Null Array
     for (i=0; i < numberOfIterations; i++)
        {
       // printf ("Number of Arrays = %d \n",Diagnostic_Manager::getNumberOfArraysInUse());
          A.redim(0);
        }
     APP_ASSERT (Diagnostic_Manager::getNumberOfArraysInUse() == 1);

  // Test for valid non Null Array
     for (i=0; i < numberOfIterations; i++)
        {
       // printf ("Number of Arrays = %d \n",Diagnostic_Manager::getNumberOfArraysInUse());
          A.resize(10);
        }
     APP_ASSERT (Diagnostic_Manager::getNumberOfArraysInUse() == 1);

  // Test for valid Null Array
     for (i=0; i < numberOfIterations; i++)
        {
       // printf ("Number of Arrays = %d \n",Diagnostic_Manager::getNumberOfArraysInUse());
          A.resize(0);
        }
     APP_ASSERT (Diagnostic_Manager::getNumberOfArraysInUse() == 1);

  // reshape can't change the size of an array so set the size explicitly
     A.redim(10);

     printf ("Testing reshape: Number of Arrays = %d \n",Diagnostic_Manager::getNumberOfArraysInUse());
  // Test for valid non Null Array
     for (i=0; i < numberOfIterations; i++)
        {
          printf ("Number of Arrays = %d \n",Diagnostic_Manager::getNumberOfArraysInUse());
          A.reshape(10,1);
          A.reshape(1,10);
        }
     APP_ASSERT (Diagnostic_Manager::getNumberOfArraysInUse() == 1);

  // reshape can't change the size of an array so set the size explicitly
     A.redim(0);

  // Test for valid Null Array
     for (i=0; i < numberOfIterations; i++)
        {
          printf ("Number of Arrays = %d \n",Diagnostic_Manager::getNumberOfArraysInUse());
          A.reshape(0,1);
          A.reshape(1,0);
        }
     APP_ASSERT (Diagnostic_Manager::getNumberOfArraysInUse() == 1);

     return (0);
   }

