#include "Mapping.h"
#include "Square.h"

const char *
ftor(const char *s);

int 
main()
{

  real time = getCPU();
  
  cout << "[%e %f] -> [" << ftor("%e %f") << "]" << endl;
  cout << "[%e %e %e %e] -> [" << ftor("%e %e %e %e]") << "]" << endl;
  cout << "[%f %f %f %f] -> [" << ftor("%f %f %f %f") << "]" << endl;

  real time2=getCPU()-time;
  cout << "time= " << time << ", time2=" << time2 << endl;

  real x=1;
  for( int j=0; j<50; j++ )
  {
    
    for( int i=0; i<1000000; i++ )
    {
      if( i%2 == 0 )
	x=x*3.14;
      else
	x=x/3.14;
    }
    cout << "x=" << x << endl;
    
    time2=getCPU()-time;
    cout << "time= " << time << ", time2=" << time2 << endl;
  }  

  return 0;  
}
