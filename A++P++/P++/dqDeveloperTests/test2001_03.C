#include "A++.h"
int main(int argc, char *argv[])
   {  
     int numberOfProcessors = 0;  
     Optimization_Manager::Initialize_Virtual_Machine ("", numberOfProcessors, argc, argv);


     Optimization_Manager::Exit_Virtual_Machine();  
     return 0;
   }

