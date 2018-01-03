#define BOUNDS_CHECK

// Force the use of a special version of the parallel printf
#define USE_PARALLEL_PRINTF
#include "A++.h"

// Tests to be done;
// 1) Different stencil widths
// 2) Predict and verify the number of message sent and recieved with each test
// 3) Compare to the equivalent portion of the serial array operation

// Use this in the macro below to report on the number of messages sent/recieved.
//        printf ("messagesSent = %d messagesRecieved = %d \n",messagesSent,messagesRecieved);


#define TEST_MACRO_1_PARAMETER(parallelStatement,serialStatement,documentationString) \
     for (i[5]=-indexRadius[5]; i[5] <= indexRadius[5]; i[5]++) \
     for (i[4]=-indexRadius[4]; i[4] <= indexRadius[4]; i[4]++) \
     for (i[3]=-indexRadius[3]; i[3] <= indexRadius[3]; i[3]++) \
     for (i[2]=-indexRadius[2]; i[2] <= indexRadius[2]; i[2]++) \
     for (i[1]=-indexRadius[1]; i[1] <= indexRadius[1]; i[1]++) \
     for (i[0]=-indexRadius[0]; i[0] <= indexRadius[0]; i[0]++) \
        { \
          testCounter++; \
          printf ("Number Of Arrays In Use (before): %d \n",Diagnostic_Manager::getNumberOfArraysInUse()); \
          int messagesSentBefore     = Diagnostic_Manager::getNumberOfMessagesSent(); \
          int messagesRecievedBefore = Diagnostic_Manager::getNumberOfMessagesReceived(); \
          printf ("Processing test: "); \
          switch (parallelX.numberOfDimensions()) \
             { \
	       case 1: \
                    printf (documentationString,testCounter,numberOfCases,i[0],j[0],k[0],l[0],m[0],n[0],o[0]); \
                    break; \
	       case 2: \
                    printf (documentationString,testCounter,numberOfCases, \
                         i[0],i[1],j[0],j[1],k[0],k[1],l[0],l[1],m[0],m[1],n[0],n[1],o[0],o[1]); \
                    break; \
	       case 3: \
                    printf (documentationString,testCounter,numberOfCases, \
                         i[0],i[1],i[2],j[0],j[1],j[2],k[0],k[1],k[2],l[0],l[1],l[2], \
                         m[0],m[1],m[2],n[0],n[1],n[2],o[0],o[1],o[2]); \
                    break; \
	       case 4: \
                    printf (documentationString,testCounter,numberOfCases, \
                         i[0],i[1],i[2],i[3],j[0],j[1],j[2],j[3],k[0],k[1],k[2],k[3],l[0],l[1],l[2],l[3], \
                         m[0],m[1],m[2],m[3],n[0],n[1],n[2],n[3],o[0],o[1],o[2],o[3]); \
                    break; \
	       case 5: \
                    printf (documentationString,testCounter,numberOfCases, \
                         i[0],i[1],i[2],i[3],i[4],j[0],j[1],j[2],j[3],j[4],k[0],k[1],k[2],k[3],k[4], \
                         l[0],l[0],l[0],l[0],l[0],m[0],m[1],m[2],m[3],m[4],n[0],n[1],n[2],n[3],n[4], \
                         o[0],o[1],o[2],o[3],o[4]); \
                    break; \
               case 6: \
                    printf (documentationString,testCounter,numberOfCases, \
                         i[0],i[1],i[2],i[3],i[4],i[5],j[0],j[1],j[2],j[3],j[4],j[5],k[0],k[1],k[2],k[3],k[4],k[5], \
                         l[0],l[1],l[2],l[3],l[4],l[5],m[0],m[1],m[2],m[3],m[4],m[5],n[0],n[1],n[2],n[3],n[4],n[5], \
                         o[0],o[1],o[2],o[3],o[4],o[5]); \
                    break; \
               default: \
                    printf ("ERROR: default reached (printf)! \n"); APP_ABORT(); \
             } \
       /* parallelStatement; */ \
          int messagesSent     = Diagnostic_Manager::getNumberOfMessagesSent() - messagesSentBefore;  \
          int messagesRecieved = Diagnostic_Manager::getNumberOfMessagesReceived() - messagesRecievedBefore; \
          serialStatement; \
          intSerialArray localArray; \
          intSerialArray serialArray; \
          Range localRange[6]; \
          for (index=0; index < dimension; index++) \
             { \
               localRange[index] = parallelX.getDomain().getLocalMaskIndex(index); \
             } \
          localArray.reference(*parallelX.getSerialArrayPointer()); \
          switch (parallelX.numberOfDimensions()) \
             { \
	       case 1: \
                    serialArray.reference(serialX(localRange[0])); \
                    break; \
	       case 2: \
                    serialArray.reference(serialX(localRange[0],localRange[1])); \
                    break; \
	       case 3: \
                    serialArray.reference(serialX(localRange[0],localRange[1],localRange[2])); \
                    break; \
	       case 4: \
                    serialArray.reference(serialX(localRange[0],localRange[1],localRange[2],localRange[3])); \
                    break; \
	       case 5: \
                    serialArray.reference(serialX(localRange[0],localRange[1],localRange[2],localRange[3],localRange[4])); \
                    break; \
	       case 6: \
                    serialArray.reference(serialX(localRange[0],localRange[1],localRange[2],localRange[3],localRange[4],localRange[5])); \
                    break; \
               default: \
                    printf ("ERROR: default reached! \n"); APP_ABORT(); \
             } \
          if ( /* sum(serialArray != localArray) != */ 0 ) \
             {  \
               printf (" : FAILED test! \n"); \
	       APP_ABORT(); \
             } \
            else \
             { \
               printf (" : PASSED test! \n"); \
               Diagnostic_Manager::displayCommunication("Global Communication"); \
             } \
          printf ("Number Of Arrays In Use (after): %d \n",Diagnostic_Manager::getNumberOfArraysInUse()); \
        }

#define TEST_MACRO_2_PARAMETER(parallelStatement,serialStatement,documentationString) \
     for (j[5]=-indexRadius[5]; j[5] <= indexRadius[5]; j[5]++) \
     for (j[4]=-indexRadius[4]; j[4] <= indexRadius[4]; j[4]++) \
     for (j[3]=-indexRadius[3]; j[3] <= indexRadius[3]; j[3]++) \
     for (j[2]=-indexRadius[2]; j[2] <= indexRadius[2]; j[2]++) \
     for (j[1]=-indexRadius[1]; j[1] <= indexRadius[1]; j[1]++) \
     for (j[0]=-indexRadius[0]; j[0] <= indexRadius[0]; j[0]++) \
        { \
          TEST_MACRO_1_PARAMETER(parallelStatement,serialStatement,documentationString) \
        }
#define TEST_MACRO_3_PARAMETER(parallelStatement,serialStatement,documentationString) \
     for (k[5]=-indexRadius[5]; k[5] <= indexRadius[5]; k[5]++) \
     for (k[4]=-indexRadius[4]; k[4] <= indexRadius[4]; k[4]++) \
     for (k[3]=-indexRadius[3]; k[3] <= indexRadius[3]; k[3]++) \
     for (k[2]=-indexRadius[2]; k[2] <= indexRadius[2]; k[2]++) \
     for (k[1]=-indexRadius[1]; k[1] <= indexRadius[1]; k[1]++) \
     for (k[0]=-indexRadius[0]; k[0] <= indexRadius[0]; k[0]++) \
        { \
          TEST_MACRO_2_PARAMETER(parallelStatement,serialStatement,documentationString) \
        }

#define TEST_MACRO_4_PARAMETER(parallelStatement,serialStatement,documentationString) \
     for (l[5]=-indexRadius[5]; l[5] <= indexRadius[5]; l[5]++) \
     for (l[4]=-indexRadius[4]; l[4] <= indexRadius[4]; l[4]++) \
     for (l[3]=-indexRadius[3]; l[3] <= indexRadius[3]; l[3]++) \
     for (l[2]=-indexRadius[2]; l[2] <= indexRadius[2]; l[2]++) \
     for (l[1]=-indexRadius[1]; l[1] <= indexRadius[1]; l[1]++) \
     for (l[0]=-indexRadius[0]; l[0] <= indexRadius[0]; l[0]++) \
        { \
          TEST_MACRO_3_PARAMETER(parallelStatement,serialStatement,documentationString) \
        }

#define TEST_MACRO_5_PARAMETER(parallelStatement,serialStatement,documentationString) \
     for (m[5]=-indexRadius[5]; m[5] <= indexRadius[5]; m[5]++) \
     for (m[4]=-indexRadius[4]; m[4] <= indexRadius[4]; m[4]++) \
     for (m[3]=-indexRadius[3]; m[3] <= indexRadius[3]; m[3]++) \
     for (m[2]=-indexRadius[2]; m[2] <= indexRadius[2]; m[2]++) \
     for (m[1]=-indexRadius[1]; m[1] <= indexRadius[1]; m[1]++) \
     for (m[0]=-indexRadius[0]; m[0] <= indexRadius[0]; m[0]++) \
        { \
          TEST_MACRO_4_PARAMETER(parallelStatement,serialStatement,documentationString) \
        }

#define TEST_MACRO_6_PARAMETER(parallelStatement,serialStatement,documentationString) \
     for (n[5]=-indexRadius[5]; n[5] <= indexRadius[5]; n[5]++) \
     for (n[4]=-indexRadius[4]; n[4] <= indexRadius[4]; n[4]++) \
     for (n[3]=-indexRadius[3]; n[3] <= indexRadius[3]; n[3]++) \
     for (n[2]=-indexRadius[2]; n[2] <= indexRadius[2]; n[2]++) \
     for (n[1]=-indexRadius[1]; n[1] <= indexRadius[1]; n[1]++) \
     for (n[0]=-indexRadius[0]; n[0] <= indexRadius[0]; n[0]++) \
        { \
          TEST_MACRO_5_PARAMETER(parallelStatement,serialStatement,documentationString) \
        }

#define TEST_MACRO_7_PARAMETER(parallelStatement,serialStatement,documentationString) \
     for (o[5]=-indexRadius[5]; o[5] <= indexRadius[5]; o[5]++) \
     for (o[4]=-indexRadius[4]; o[4] <= indexRadius[4]; o[4]++) \
     for (o[3]=-indexRadius[3]; o[3] <= indexRadius[3]; o[3]++) \
     for (o[2]=-indexRadius[2]; o[2] <= indexRadius[2]; o[2]++) \
     for (o[1]=-indexRadius[1]; o[1] <= indexRadius[1]; o[1]++) \
     for (o[0]=-indexRadius[0]; o[0] <= indexRadius[0]; o[0]++) \
        { \
          TEST_MACRO_6_PARAMETER(parallelStatement,serialStatement,documentationString) \
        }

#define SETUP_MACRO \
     intArray parallelX; \
     intArray parallelY; \
     intSerialArray serialX; \
     intSerialArray serialY; \
     int testCounter               = 0; \
     int numberOfCases             = 0; \
     int numberOfCasesPerParameter[6];  \
     int indexRadius[6] = { 0,0,0,0,0,0}; \
     int i[6],j[6],k[6],l[6],m[6],n[6],o[6]; \
     int newBases[6] = { 10,-20,30,-40,50,-60}; \
     Range Ix[6]; \
     Range Iy[6]; \
     int minSize = 0; \
     int index = 0; \
     size = (int) pow (double(size), 1.0/double(dimension)); \
     printf ("Adjusted size = %d \n",size); \
     for (index=0; index < dimension; index++) \
        { \
          indexRadius[index] = maxIndexRadius; \
          minSize = (2 * indexRadius[index] + 1) + 2; \
          if (size < minSize) \
               size = minSize; \
	} \
     printf ("top of switch in SETUP_MACRO \n"); \
     switch (dimension) \
        { \
          case 1: parallelX.redim(size); parallelX.setBase(newBases[0],0); \
                  parallelY.redim(size); parallelY.setBase(newBases[0]*2,0); \
               break; \
          case 2: parallelX.redim(size,size); \
                  parallelY.redim(size,size); \
               break; \
          case 3: parallelX.redim(size,size,size); \
                  parallelY.redim(size,size,size); \
               break; \
          case 4: parallelX.redim(size,size,size,size); \
                  parallelY.redim(size,size,size,size); \
               break; \
          case 5: parallelX.redim(size,size,size,size,size); \
                  parallelY.redim(size,size,size,size,size); \
               break; \
          case 6: parallelX.redim(size,size,size,size,size,size); \
                  parallelY.redim(size,size,size,size,size,size); \
               break; \
          default: \
               printf ("ERROR: default reached (printf)! \n"); APP_ABORT(); \
        } \
     for (index=0; index < dimension; index++) \
        { \
          numberOfCasesPerParameter[index] = (2 * indexRadius[index]) + 1; \
          i[index] = j[index] = k[index] = l[index] = m[index] = n[index] = o[index] = 0; \
          Ix[index] = Range(parallelX.getBase(index)+indexRadius[index],parallelX.getBound(index)-indexRadius[index]); \
          Iy[index] = Range(parallelY.getBase(index)+indexRadius[index],parallelY.getBound(index)-indexRadius[index]); \
	} \
     initialize(parallelX,parallelY,serialX,serialY);


void
initialize (
   intArray & parallelX     , intArray & parallelY , 
   intSerialArray & serialX , intSerialArray & serialY )
   {
     printf ("Call seqAdd for each parallel and serial array \n");

  // This sort of initialization should help identify errors
     printf ("parallelX.seqAdd(1,10); \n");
     parallelX.seqAdd(1,10);

     printf ("parallelY.seqAdd(1,20); \n");
     parallelY.seqAdd(1,20);

     printf ("serialX.seqAdd(1,10); \n");
     serialX.seqAdd(1,10);

     printf ("serialY.seqAdd(1,20); \n");
     serialY.seqAdd(1,20);
   }

void
test_1D ( int maxIndexRadius, int size )
   {
     printf ("Top of test_1D(%d) \n",size);
     int dimension = 1;
     SETUP_MACRO
     printf ("######################################################## \n");
     printf ("#######################  1D TESTS (Size = %3d) ######### \n",size);
     printf ("######################################################## \n");
     printf ("minSize = %d size = %d \n",minSize, size);

     testCounter = 0;
     numberOfCases = numberOfCasesPerParameter[0];
     printf ("################ 1 Parameter Test ###################### \n");
#if 1
     TEST_MACRO_1_PARAMETER(
          parallelX(Ix[0]+i[0]) = 1,
          serialX  (Ix[0]+i[0]) = 1,
          "Passed 1 parameter test #%-3d of %d X(I + %2d) = 1;")
#endif
     testCounter = 0;
     numberOfCases *= numberOfCasesPerParameter[0];
     printf ("################ 2 Parameter Test ###################### \n");
#if 1
     TEST_MACRO_2_PARAMETER(
          parallelX(Ix[0]+i[0]) = parallelY(Iy[0]+j[0]),
          serialX  (Ix[0]+i[0]) = serialY  (Iy[0]+j[0]),
          "Passed 2 parameter test #%-3d of %d X(Ix + %2d) = Y(Iy + %2d); ")
#endif
     testCounter = 0;
     numberOfCases *= numberOfCasesPerParameter[0];
#if 1
     TEST_MACRO_3_PARAMETER(
          parallelX(Ix[0]+i[0]) = parallelY(Iy[0]+j[0]) + parallelY(Iy[0]+k[0]),
          serialX  (Ix[0]+i[0]) = serialY  (Iy[0]+j[0]) + serialY  (Iy[0]+k[0]),
          "Passed 3 parameter test #%-3d of %d X(Ix + %2d) = Y(Iy + %2d) + Y(Iy + %2d);")
#endif
     testCounter = 0;
     numberOfCases *= numberOfCasesPerParameter[0];
#if 1
     TEST_MACRO_4_PARAMETER(
          parallelX(Ix[0]+i[0]) = parallelY(Iy[0]+j[0]) + parallelY(Iy[0]+k[0]) + parallelY(Iy[0]+l[0]),
          serialX  (Ix[0]+i[0]) = serialY  (Iy[0]+j[0]) + serialY  (Iy[0]+k[0]) + serialY  (Iy[0]+l[0]),
          "Passed 4 parameter test #%-3d of %d X(Ix + %2d) = Y(Iy + %2d) + Y(Iy + %2d) + Y(Iy + %2d);")
#endif
     testCounter = 0;
     numberOfCases *= numberOfCasesPerParameter[0];
#if 1
     TEST_MACRO_5_PARAMETER(
          parallelX(Ix[0]+i[0]) = parallelY(Iy[0]+j[0]) + parallelY(Iy[0]+k[0]) + parallelY(Iy[0]+l[0]) + parallelY(Iy[0]+m[0]),
          serialX  (Ix[0]+i[0]) = serialY  (Iy[0]+j[0]) + serialY  (Iy[0]+k[0]) + serialY  (Iy[0]+l[0]) + serialY  (Iy[0]+m[0]),
          "Passed 5 parameter test #%-3d of %d X(Ix + %2d) = Y(Iy + %2d) + Y(Iy + %2d) + Y(Iy + %2d) + Y(Iy + %2d);")
#endif
     testCounter = 0;
     numberOfCases *= numberOfCasesPerParameter[0];
#if 0
     TEST_MACRO_6_PARAMETER(
          parallelX(Ix[0]+i[0]) = parallelY(Iy[0]+j[0]) + parallelY(Iy[0]+k[0]) + parallelY(Iy[0]+l[0]) + parallelY(Iy[0]+m[0]) + parallelY(Iy[0]+n[0]),
          serialX  (Ix[0]+i[0]) = serialY  (Iy[0]+j[0]) + serialY  (Iy[0]+k[0]) + serialY  (Iy[0]+l[0]) + serialY  (Iy[0]+m[0]) + serialY  (Iy[0]+n[0]),
          "Passed 6 parameter test #%-3d of %d X(Ix + %2d) = Y(Iy + %2d) + Y(Iy + %2d) + Y(Iy + %2d) + Y(Iy + %2d) + Y(Iy + %2d);")
#endif
     testCounter = 0;
     numberOfCases *= numberOfCasesPerParameter[0];
#if 0
     TEST_MACRO_7_PARAMETER(
          parallelX(Ix[0]+i[0]) = parallelY(Iy[0]+j[0]) + parallelY(Iy[0]+k[0]) + parallelY(Iy[0]+l[0]) + parallelY(Iy[0]+m[0]) + parallelY(Iy[0]+n[0]) + parallelY(Iy[0]+o[0]),
          serialX  (Ix[0]+i[0]) = serialY  (Iy[0]+j[0]) + serialY  (Iy[0]+k[0]) + serialY  (Iy[0]+l[0]) + serialY  (Iy[0]+m[0]) + serialY  (Iy[0]+n[0]) + serialY  (Iy[0]+o[0]),
          "Passed 7 parameter test #%-3d of %d X(Ix + %2d) = Y(Iy + %2d) + Y(Iy + %2d) + Y(Iy + %2d) + Y(Iy + %2d) + Y(Iy + %2d) + Y(Iy + %2d);")
#endif
   }


void
test_2D ( int maxIndexRadius, int size )
   {
     printf ("Top of test_2D(%d) \n",size);
     int dimension = 2;
     SETUP_MACRO
     printf ("######################################################## \n");
     printf ("#######################  2D TESTS (Size = %3d) ######### \n",size);
     printf ("######################################################## \n");
     
     printf ("minSize = %d size = %d \n",minSize, size);

     testCounter = 0;
     numberOfCases = (int) pow(double(numberOfCasesPerParameter[1]),double(dimension));
     printf ("################ 1 Parameter Test ###################### \n");
     TEST_MACRO_1_PARAMETER(
          parallelX(Ix[0]+i[0],Ix[1]+i[1]) = 1,
          serialX  (Ix[0]+i[0],Ix[1]+i[1]) = 1,
          "Passed 1 parameter test #%-3d of %d X(Ix + %2d,Ix + %2d) = 1;")
     testCounter = 0;
     numberOfCases *= (int) pow(double(numberOfCasesPerParameter[1]),double(dimension));
     printf ("################ 2 Parameter Test ###################### \n");
     TEST_MACRO_2_PARAMETER(
          parallelX(Ix[0]+i[0],Ix[1]+i[1]) = parallelY(Iy[0]+j[0],Iy[1]+j[1]),
          serialX  (Ix[0]+i[0],Ix[1]+i[1]) = serialY  (Iy[0]+j[0],Iy[1]+j[1]),
          "Passed 2 parameter test #%-3d of %d X(Ix + %2d,Ix + %2d) = Y(Iy + %2d,Iy + %2d); ")

     testCounter = 0;
     numberOfCases *= (int) pow(double(numberOfCasesPerParameter[1]),double(dimension));
     TEST_MACRO_3_PARAMETER(
          parallelX(Ix[0]+i[0],Ix[1]+i[1]) = parallelY(Iy[0]+j[0],Iy[1]+j[1]) + parallelY(Iy[0]+k[0],Iy[1]+k[1]),
          serialX  (Ix[0]+i[0],Ix[1]+i[1]) = serialY  (Iy[0]+j[0],Iy[1]+j[1]) + serialY  (Iy[0]+k[0],Iy[1]+k[1]),
          "Passed 3 parameter test #%-3d of %d X(Ix + %2d,Ix + %2d) = Y(Iy + %2d,Iy + %2d) + Y(Iy + %2d,Iy + %2d);")
   }

#if 0
void
test_3D ( int maxIndexRadius, int size )
   {
     printf ("Top of test_2D(%d) \n",size);
     int dimension = 3;
     SETUP_MACRO
     printf ("######################################################## \n");
     printf ("#######################  3D TESTS (Size = %3d) ######### \n",size);
     printf ("######################################################## \n");

     printf ("minSize = %d size = %d \n",minSize, size);

     testCounter = 0;
     numberOfCases = (int) pow(double(numberOfCasesPerParameter[2]),double(dimension));
     printf ("################ 1 Parameter Test ###################### \n");
     TEST_MACRO_1_PARAMETER(
          parallelX(I[0]+i[0],I[1]+i[1],I[2]+i[2]) = 1,
          serialX  (I[0]+i[0],I[1]+i[1],I[2]+i[2]) = 1,
          "Passed 1 parameter test #%-3d of %d X(I + %-2d,I + %-2d,I + %-2d) = 1;")
     testCounter = 0;
     numberOfCases *= (int) pow(double(numberOfCasesPerParameter[2]),double(dimension));
     printf ("################ 2 Parameter Test ###################### \n");
     TEST_MACRO_2_PARAMETER(
          parallelX(I[0]+i[0],I[1]+i[1],I[2]+i[2]) = parallelY(I[0]+j[0],I[1]+j[1],I[2]+j[2]),
          serialX  (I[0]+i[0],I[1]+i[1],I[2]+i[2]) = serialY  (I[0]+j[0],I[1]+j[1],I[2]+j[2]),
          "Passed 2 parameter test #%-3d of %d X(I + %-2d,I + %-2d,I + %-2d) = Y(I + %-2d,I + %-2d,I + %-2d); ")

     testCounter = 0;
     numberOfCases *= (int) pow(double(numberOfCasesPerParameter[2]),double(dimension));
     TEST_MACRO_3_PARAMETER(
          parallelX(I[0]+i[0],I[1]+i[1],I[2]+i[2]) = parallelY(I[0]+j[0],I[1]+j[1],I[2]+j[2]) + parallelY(I[0]+k[0],I[1]+k[1],I[2]+k[2]),
          serialX  (I[0]+i[0],I[1]+i[1],I[1]+i[1]) = serialY  (I[0]+j[0],I[1]+j[1],I[2]+j[2]) + serialY  (I[0]+k[0],I[1]+k[1],I[2]+k[2]),
          "Passed 3 parameter test #%-3d of %d X(I + %2d,I + %2d,I + %2d) = Y(I + %2d,I + %2d,I + %2d) + Y(I + %2d,I + %2d,I + %2d);")
   }
#endif

#if 0
void
test_4D ( int maxIndexRadius, int size )
   {
     printf ("Top of test_2D(%d) \n",size);
     int dimension = 4;
     SETUP_MACRO
     printf ("######################################################## \n");
     printf ("#######################  4D TESTS (Size = %3d) ######### \n",size);
     printf ("######################################################## \n");

     printf ("minSize = %d size = %d \n",minSize, size);

     testCounter = 0;
     numberOfCases = (int) pow(double(numberOfCasesPerParameter[3]),double(dimension));
     printf ("################ 1 Parameter Test ###################### \n");
     TEST_MACRO_1_PARAMETER(
          parallelX(I[0]+i[0],I[1]+i[1],I[2]+i[2],I[3]+i[3]) = 1,
          serialX  (I[0]+i[0],I[1]+i[1],I[2]+i[2],I[3]+i[3]) = 1,
          "Passed 1 parameter test #%-3d of %d X(I + %-2d,I + %-2d,I + %-2d,I + %-2d) = 1;")

     testCounter = 0;
     numberOfCases *= (int) pow(double(numberOfCasesPerParameter[3]),double(dimension));
     printf ("################ 2 Parameter Test ###################### \n");
     TEST_MACRO_2_PARAMETER(
          parallelX(I[0]+i[0],I[1]+i[1],I[2]+i[2],I[3]+i[3]) = parallelY(I[0]+j[0],I[1]+j[1],I[2]+j[2],I[3]+j[3]),
          serialX  (I[0]+i[0],I[1]+i[1],I[2]+i[2],I[3]+i[3]) = serialY  (I[0]+j[0],I[1]+j[1],I[2]+j[2],I[3]+j[3]),
          "Passed 2 parameter test #%-3d of %d X(I + %-2d,I + %-2d,I + %-2d,I + %-2d) = Y(I + %-2d,I + %-2d,I + %-2d,I + %-2d); ")

     testCounter = 0;
     numberOfCases *= (int) pow(double(numberOfCasesPerParameter[3]),double(dimension));
     TEST_MACRO_3_PARAMETER(
          parallelX(I[0]+i[0],I[1]+i[1],I[2]+i[2],I[3]+i[3]) = parallelY(I[0]+j[0],I[1]+j[1],I[2]+j[2],I[3]+j[3]) + parallelY(I[0]+k[0],I[1]+k[1],I[2]+k[2],I[3]+k[3]),
          serialX  (I[0]+i[0],I[1]+i[1],I[1]+i[1],I[3]+i[3]) = serialY  (I[0]+j[0],I[1]+j[1],I[2]+j[2],I[3]+j[3]) + serialY  (I[0]+k[0],I[1]+k[1],I[2]+k[2],I[3]+k[3]),
          "Passed 3 parameter test #%-3d of %d X(I + %2d,I + %2d,I + %2d,I + %2d) = Y(I + %2d,I + %2d,I + %2d,I + %2d) + Y(I + %2d,I + %2d,I + %2d,I + %2d);")
   }
#endif

#if 0
void
test_5D ( int maxIndexRadius, int size )
   {
     printf ("Top of test_2D(%d) \n",size);
     int dimension = 5;
     SETUP_MACRO
     printf ("######################################################## \n");
     printf ("#######################  5D TESTS (Size = %3d) ######### \n",size);
     printf ("######################################################## \n");

     printf ("minSize = %d size = %d \n",minSize, size);

     testCounter = 0;
     numberOfCases = (int) pow(double(numberOfCasesPerParameter[4]),double(dimension));
     printf ("################ 1 Parameter Test ###################### \n");
     TEST_MACRO_1_PARAMETER(
          parallelX(I[0]+i[0],I[1]+i[1],I[2]+i[2],I[3]+i[3],I[4]+i[4]) = 1,
          serialX  (I[0]+i[0],I[1]+i[1],I[2]+i[2],I[3]+i[3],I[4]+i[4]) = 1,
          "Passed 1 parameter test #%-3d of %d X(I + %-2d,I + %-2d,I + %-2d,I + %-2d) = 1;")
     testCounter = 0;
     numberOfCases *= (int) pow(double(numberOfCasesPerParameter[4]),double(dimension));
     printf ("################ 2 Parameter Test ###################### \n");
     TEST_MACRO_2_PARAMETER(
          parallelX(I[0]+i[0],I[1]+i[1],I[2]+i[2],I[3]+i[3],I[4]+i[4]) = parallelY(I[0]+j[0],I[1]+j[1],I[2]+j[2],I[3]+j[3],I[4]+j[4]),
          serialX  (I[0]+i[0],I[1]+i[1],I[2]+i[2],I[3]+i[3],I[4]+i[4]) = serialY  (I[0]+j[0],I[1]+j[1],I[2]+j[2],I[3]+j[3],I[4]+j[4]),
          "Passed 2 parameter test #%-3d of %d X(I + %-2d,I + %-2d,I + %-2d,I + %-2d,I + %-2d) = Y(I + %-2d,I + %-2d,I + %-2d,I + %-2d,I + %-2d); ")

     testCounter = 0;
     numberOfCases *= (int) pow(double(numberOfCasesPerParameter[4]),double(dimension));
     TEST_MACRO_3_PARAMETER(
          parallelX(I[0]+i[0],I[1]+i[1],I[2]+i[2],I[3]+i[3],I[4]+i[4]) = parallelY(I[0]+j[0],I[1]+j[1],I[2]+j[2],I[3]+j[3],I[4]+j[4]) + parallelY(I[0]+k[0],I[1]+k[1],I[2]+k[2],I[3]+k[3],I[4]+k[4]),
          serialX  (I[0]+i[0],I[1]+i[1],I[1]+i[1],I[3]+i[3],I[4]+i[4]) = serialY  (I[0]+j[0],I[1]+j[1],I[2]+j[2],I[3]+j[3],I[4]+j[4]) + serialY  (I[0]+k[0],I[1]+k[1],I[2]+k[2],I[3]+k[3],I[4]+k[4]),
          "Passed 3 parameter test #%-3d of %d X(I + %2d,I + %2d,I + %2d,I + %2d,I + %2d) = Y(I + %2d,I + %2d,I + %2d,I + %2d,I + %2d) + Y(I + %2d,I + %2d,I + %2d,I + %2d,I + %2d);")
   }
#endif

#if 0
void
test_6D ( int maxIndexRadius, int size )
   {
     printf ("Top of test_2D(%d) \n",size);
     int dimension = 6;
     SETUP_MACRO
     printf ("######################################################## \n");
     printf ("#######################  6D TESTS (Size = %3d) ######### \n",size);
     printf ("######################################################## \n");

     printf ("minSize = %d size = %d \n",minSize, size);

     testCounter = 0;
     numberOfCases = (int) pow(double(numberOfCasesPerParameter[5]),double(dimension));
     printf ("################ 1 Parameter Test ###################### \n");
     TEST_MACRO_1_PARAMETER(
          parallelX(I[0]+i[0],I[1]+i[1],I[2]+i[2],I[3]+i[3],I[4]+i[4],I[5]+i[5]) = 1,
          serialX  (I[0]+i[0],I[1]+i[1],I[2]+i[2],I[3]+i[3],I[4]+i[4],I[5]+i[5]) = 1,
          "Passed 1 parameter test #%-3d of %d X(I + %-2d,I + %-2d,I + %-2d,I + %-2d,I + %-2d,I + %-2d) = 1;")

     testCounter = 0;
     numberOfCases *= (int) pow(double(numberOfCasesPerParameter[5]),double(dimension));
     printf ("################ 2 Parameter Test ###################### \n");
     TEST_MACRO_2_PARAMETER(
          parallelX(I[0]+i[0],I[1]+i[1],I[2]+i[2],I[3]+i[3],I[4]+i[4],I[5]+i[5]) = parallelY(I[0]+j[0],I[1]+j[1],I[2]+j[2],I[3]+j[3],I[4]+j[4],I[5]+j[5]),
          serialX  (I[0]+i[0],I[1]+i[1],I[2]+i[2],I[3]+i[3],I[4]+i[4],I[5]+i[5]) = serialY  (I[0]+j[0],I[1]+j[1],I[2]+j[2],I[3]+j[3],I[4]+j[4],I[5]+j[5]),
          "Passed 2 parameter test #%-3d of %d X(I + %-2d,I + %-2d,I + %-2d,I + %-2d,I + %-2d,I + %-2d) = Y(I + %-2d,I + %-2d,I + %-2d,I + %-2d,I + %-2d,I + %-2d); ")

     testCounter = 0;
     numberOfCases *= (int) pow(double(numberOfCasesPerParameter[5]),double(dimension));
     TEST_MACRO_3_PARAMETER(
          parallelX(I[0]+i[0],I[1]+i[1],I[2]+i[2],I[3]+i[3],I[4]+i[4],I[5]+i[5]) = parallelY(I[0]+j[0],I[1]+j[1],I[2]+j[2],I[3]+j[3],I[4]+j[4],I[5]+j[5]) + parallelY(I[0]+k[0],I[1]+k[1],I[2]+k[2],I[3]+k[3],I[4]+k[4],I[5]+k[5]),
          serialX  (I[0]+i[0],I[1]+i[1],I[1]+i[1],I[3]+i[3],I[4]+i[4],I[5]+i[5]) = serialY  (I[0]+j[0],I[1]+j[1],I[2]+j[2],I[3]+j[3],I[4]+j[4],I[5]+j[5]) + serialY  (I[0]+k[0],I[1]+k[1],I[2]+k[2],I[3]+k[3],I[4]+k[4],I[5]+k[5]),
          "Passed 3 parameter test #%-3d of %d X(I + %2d,I + %2d,I + %2d,I + %2d,I + %2d,I + %2d) = Y(I + %2d,I + %2d,I + %2d,I + %2d,I + %2d,I + %2d) + Y(I + %2d,I + %2d,I + %2d,I + %2d,I + %2d,I + %2d);")
   }
#endif

#define MAX_NUMBER_OF_ARRAY_SIZES 10

int
main(int argc, char** argv)
   {
     ios::sync_with_stdio();
     Index::setBoundsCheck(on);

     int numberOfProcessors = 0;
     Optimization_Manager::Initialize_Virtual_Machine("",numberOfProcessors,argc,argv);

#ifdef _AIX
  // This call is made so that the Blue Pacific compiler will see the use of MPI directly
  // and then link to the appropriate library (libmpi).  mpCC determines the 
  // correct libraries to link and needs this to trigger linking to libmpi.
     MPI_Barrier(MPI_COMM_WORLD);
#endif

#if 1
  Partitioning_Type::SpecifyDefaultInternalGhostBoundaryWidths(1);
#endif

#if 1
  // This is the default while we get the VSG mode (and P++ generally) to work robustly
     Optimization_Manager::setForceVSG_Update(On);
#endif

  // Select a problem from or range of problem sizes
     int maxNumberOfSizes = MAX_NUMBER_OF_ARRAY_SIZES-1;
     int minSizeIndex     = 4;
     int maxSizeIndex     = 4;

     int i = 0;
     int sizeArray[MAX_NUMBER_OF_ARRAY_SIZES];

#if 0
     for (i=0; i < maxNumberOfSizes; i++)
          sizeArray[i] = (double(i + maxNumberOfSizes - 2) / double(maxNumberOfSizes-1)) * double(numberOfProcessors);
#else
  // setup the sizes to be a range from less than the number
  // of processors to greater than the number of processors
     sizeArray[0] =   0.5 * double(numberOfProcessors);
     sizeArray[1] =   1.0 * double(numberOfProcessors);
     sizeArray[2] =   2.0 * double(numberOfProcessors);
     sizeArray[3] =   4.0 * double(numberOfProcessors);
     sizeArray[4] =  10.0 * double(numberOfProcessors);
     sizeArray[5] = 100.0 * double(numberOfProcessors);
     sizeArray[6] = 1000.0 * double(numberOfProcessors);
     sizeArray[7] = 10000.0 * double(numberOfProcessors);
     sizeArray[8] = 100000.0 * double(numberOfProcessors);
     sizeArray[9] = 1000000.0 * double(numberOfProcessors);
#endif

     APP_ASSERT (maxNumberOfSizes < MAX_NUMBER_OF_ARRAY_SIZES);
     APP_ASSERT (maxNumberOfSizes >= 2);

  // Partitioning_Type::SpecifyDefaultInternalGhostBoundaryWidths(0);

  // set the radius of the stencil (radius 1 in 1D would be a 3 point stencil)
     int minIndexRadius = 1;
     int maxIndexRadius = 1;

#if 0
  // Only use a subrange so that this test will not take TOO LONG!
     for (i=minSizeIndex; i <= maxSizeIndex; i++)
#else
     i = 5;
#endif
        {
          printf ("***************************************************************** \n");
          printf ("***************  TESTING PROBLEM SIZE = %4d ********************* \n",sizeArray[i]);
          printf ("***************************************************************** \n");
#if 1
          test_1D ( maxIndexRadius, sizeArray[i] );
#endif
#if 1
          test_2D ( maxIndexRadius, sizeArray[i] );
#endif
#if 0
          test_3D ( maxIndexRadius, sizeArray[i] );
#endif
#if 0
          test_4D ( maxIndexRadius, sizeArray[i] );
#endif
#if 0
          test_5D ( maxIndexRadius, sizeArray[i] );
#endif
#if 0
          test_6D ( maxIndexRadius, sizeArray[i] );
#endif
        }

     APP_DEBUG = 0;

  // Call the diagnostics mechanism to display memory usage
     Diagnostic_Manager::report();

     printf ("Program Terminated Normally! \n");
     Optimization_Manager::Exit_Virtual_Machine();

     return 0;
   }














