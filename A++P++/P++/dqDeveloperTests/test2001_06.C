#include "A++.h"
/*  main program starts here*/

extern int APP_Global_Array_ID;

void
reshapeTest ()
   {
     printf ("Reshape test ... \n");

     intArray xx(Range(0,1),Range(0,1),Range(0,1),Range(2,3));
     Partitioning_Type* partition = new Partitioning_Type();
     int ghost_boundary_width = 0;
     partition->partitionAlongAxis(0,FALSE,ghost_boundary_width);
     partition->partitionAlongAxis(1,FALSE,ghost_boundary_width);
     partition->partitionAlongAxis(2,FALSE,ghost_boundary_width);
     partition->partitionAlongAxis(3,TRUE,1);
     xx.partition(*partition);
     xx=3.;
     xx.reshape(Range(0,3),Range(0,0),Range(0,1),Range(0,1));

  // printf ("Diagnostic_Manager::getNumberOfArraysInUse() = %d \n",Diagnostic_Manager::getNumberOfArraysInUse());

#if 0
     for (int i=0; i < 100; i++)
        {
          intArray T(J(I),DEEPALIGNEDCOPY);
        }
#endif

     printf ("partition->getReferenceCount() = %d \n",partition->getReferenceCount());
     APP_ASSERT (partition->getReferenceCount() >= Internal_Partitioning_Type::getReferenceCountBase());
     partition->decrementReferenceCount();
     if (partition->getReferenceCount() < Internal_Partitioning_Type::getReferenceCountBase())
        {
          printf ("calling delete partition \n");
          delete partition;
        }
     partition = NULL;

  // ================= P++ finish ==============================

     printf ("Program Terminated Normally! \n");

  // select 50 somewhat arbitarily as an upper bound for the ids
     APP_ASSERT (APP_Global_Array_ID < 30);

  // Call the diagnostics mechanism to display memory usage
     Diagnostic_Manager::report();
   }

void
copyConstructorTest ()
   {
     printf ("Copy constructor test ... \n");

     intArray J(10);
     J = 3;
     J.seqAdd(10);

     Range I(3,9);

     I.display("theSubRange");

#if 1
     for (int i=0; i < 100; i++)
        {
          intArray T(J(I),DEEPALIGNEDCOPY);
        }
#else
     printf ("Calling copy constructor \n");
     intArray T(J(I),DEEPALIGNEDCOPY);
     printf ("DONE: Calling copy constructor \n");
#endif

  // select 50 somewhat arbitarily as an upper bound for the ids
     APP_ASSERT (APP_Global_Array_ID < 30);
   }

void
originalTest ()
   {
     printf ("Stefan's original test ... \n");

#if 1
     doubleArray C;
     C.redim(4);
#else
     doubleArray C(4);
#endif

     C = 1.0;

     Index iX(1,2);
     for (int i=0; i < 100; i++)
        {
          C(iX) = C(iX+1) - C(iX-1);
        }

  // select 50 somewhat arbitarily as an upper bound for the ids
     APP_ASSERT (APP_Global_Array_ID < 30);
   }

void
constructorTest ()
   {
     printf ("Constructor test ... \n");

     for (int i=0; i < 100; i++)
        {
          doubleArray B(10);
        }

  // select 50 somewhat arbitarily as an upper bound for the ids
     if (APP_Global_Array_ID >= 30)
          printf ("Error: APP_Global_Array_ID = %d \n",APP_Global_Array_ID);
     APP_ASSERT (APP_Global_Array_ID < 30);
   }

void
operatorTest ()
   {
     printf ("Operator test ... \n");

     doubleArray result(4);
     result = 1.;

     doubleArray A(4);
     A = 1.;
     doubleArray B(4);
     B = 1.;

  // printf ("A.Array_ID() = %d A.getLocalArray().Array_ID() = %d  B.Array_ID() = %d B.getLocalArray().Array_ID() = %d \n",
  //      A.Array_ID(),A.getLocalArray().Array_ID(),B.Array_ID(),B.getLocalArray().Array_ID());

     for (int i=0; i < 100; i++)
        {
          result = A + B;
        }

  // select 50 somewhat arbitarily as an upper bound for the ids
     APP_ASSERT (APP_Global_Array_ID < 30);
   }

int
main ( int argc, char** argv )
   {
     ios::sync_with_stdio();
     int Number_of_Processors=0;
     Optimization_Manager::Initialize_Virtual_Machine("",Number_of_Processors,argc,argv);
     Index::setBoundsCheck(on);

  // Turn on internal reference counting diagnostics
  // Diagnostic_Manager::setReferenceCountingReport(1);

#if 0
     constructorTest ();
     operatorTest ();
     originalTest();
     copyConstructorTest();
#endif

     reshapeTest();

  // Turn on internal reference counting diagnostics
     Diagnostic_Manager::setReferenceCountingReport(0);

  // select 50 somewhat arbitarily as an upper bound for the ids
     APP_ASSERT (APP_Global_Array_ID < 50);

     printf ("Program Terminated Normally! \n");

     Optimization_Manager::Exit_Virtual_Machine();
     return 0;
   }






