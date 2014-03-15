#include "Overture.h"


//================================================================================
//  Test indirect addressing
//
//================================================================================


int main()
{

  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking


  const int number =1000;
  RealArray u(number+1,number+1),v(number+1,number+1),w(number+1,number+1);
  IntegerArray ia(number),ja(number);

  for( int i=0; i<number; i++ )
  {
    ia(i)=i;
    ja(i)=i;
  }


  u=1.; v=2.; w=3.;
    
  real time0=getCPU();
  for( i=0; i<number; i++ )
  {
      u(ia(i),ia(i))=v(ia(i),ia(i))+w(ia(i),ia(i))
                    +v(ia(i),ia(i))+w(ia(i),ia(i))
		    +v(ia(i),ia(i))+w(ia(i),ia(i));
  }
  real timeLoop = getCPU()-time0;

  time0=getCPU();
    
  u(ia,ja)=v(ia,ja);
  u(ia,ja)+=v(ia,ja);
  u(ia,ja)+=v(ia,ja);
  u(ia,ja)+=v(ia,ja);
  u(ia,ja)+=v(ia,ja);
  u(ia,ja)+=v(ia,ja);

  real time1 = getCPU()-time0;
    


  time0=getCPU();
    
  u(ia,ja)=v(ia+1,ja+1)+w(ia+1,ja+1);    
  u(ia,ja)+=v(ia+1,ja+1)+w(ia+1,ja+1);    
  u(ia,ja)+=v(ia+1,ja+1)+w(ia+1,ja+1);    

  real time2 = getCPU()-time0;

  time0=getCPU();
    
  u(ia,ja)=v(ia+1,ja+1)+w(ia+1,ja+1)+v(ia+1,ja+1)+w(ia+1,ja+1)+v(ia+1,ja+1)+w(ia+1,ja+1);    

  real time3= getCPU()-time0;


  printf("time for loop = %e ,Time for indirect = %e, %e, %e, \n",timeLoop, time1,time2,time3);
    
  return 0;
}
