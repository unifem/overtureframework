// Problem Report 99-10-07-13-12-38

// This is really a P++ test problem, but it should compile and run with A++ as well.

#include <A++.h>

void test (int n)
   {
     const int Size = 258;
     Index I (1,Size-2,1);
     Partitioning_Type Partitioning(Range(0,n-1));
     doubleArray U(Size,Partitioning);
     U.fill(0.0);
     U(I) =  U(I-1);
   }

int main( int argc, char **argv)
   {
     int Number_Of_Processors = 1;
#if defined(USE_PPP)
     Optimization_Manager::Initialize_Virtual_Machine ("", Number_Of_Processors, argc, argv); 
#endif
     Optimization_Manager::Initialize_Virtual_Machine ("", Number_Of_Processors, argc, argv); 

     for( int processors = 1; processors <= Number_Of_Processors; processors++)
          test(processors);

     Optimization_Manager::Exit_Virtual_Machine();
     return 0;
   }
