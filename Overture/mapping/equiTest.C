#include "Mapping.h"


int
equidistribute( const realArray & w, realArray & r );


// define F(x) 1.+(x)
#define F(x) 1.+(x)+(x)*(x)

int
main()
{
  Range R(0,10);
  realArray w(R),r(R);
  real h=1./(R.getBound()-R.getBase());
  for( int i=R.getBase(); i<=R.getBound(); i++ )
  {
    w(i)=F((i-R.getBase())*h);
  }
  w.display("here is w");
  equidistribute( w,r );
  r.display("here is r");
  
  // check if the function is equi-distributed
  for( i=R.getBase()+1; i<=R.getBound(); i++ )
  {
    w(i)=F(r(i));
    printf("equi = %e\n",.5*(w(i)+w(i-1))*(r(i)-r(i-1)));
  }




}
