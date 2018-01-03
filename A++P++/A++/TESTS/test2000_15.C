#include <A++.h>

// indirect addressing bug.

int 
main()
{

  Range R=5;

  intArray a(R,R),ia;
  a=0;
  a(1,1)=1;
  a(2,3)=1;

  ia=a.indexMap();
  ia.display("ia");
  
  return 0;
}

