// This is Stefan's first bug for 2001.  It is a problem that only shows up
// when using P++ with PADRE.  The work around is to us P++ without PADRE.

#include "A++.h"
// include <bool.h>

/*  main program starts here */

int
main ( int argc, char** argv )
   {
     ios::sync_with_stdio();
     int Number_of_Processors=0;
     Optimization_Manager::Initialize_Virtual_Machine("",Number_of_Processors,argc,argv);

     Index::setBoundsCheck(on);
     Partitioning_Type oneDPartitioning;
     oneDPartitioning.partitionAlongAxis(0, false, 1);
     oneDPartitioning.partitionAlongAxis(1, true, 1);

#if 1
     doubleArray onedarray;
     onedarray.partition(oneDPartitioning);
     onedarray.redim(10,10); 
     onedarray = 1.;
#endif

     Index low(1), hi(8), iX(0,10);

#if 0
     doubleArray result (10,10,oneDPartitioning);
#else
     doubleArray result;
     result.partition(oneDPartitioning);
     result.redim(10,10); 
#endif

     result = 1.;

#if 1
     cout << "I = ";

  // for (int i=0; i < 1000; i++)
     for (int i=0; i < 10; i++)
        {
          cout << i << " ";
          result(iX,low) = onedarray(iX,low+1) - onedarray(iX,low-1);
          result(low,iX) = onedarray(low+1,iX) - onedarray(low-1,iX);
          result(iX,hi) = onedarray(iX,hi+1) - onedarray(iX,hi-1); 
          result(hi,iX) = onedarray(hi+1,iX) - onedarray(hi-1,iX);
        }
#endif

     result.display();

     Optimization_Manager::Exit_Virtual_Machine();
     return 0;
   }
