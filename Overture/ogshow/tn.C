#include <iostream.h>
#include <math.h>

int
main()
{
  float x;
  x=log(0.);
  if( x!=x*2. )
    cout << "x (1) = " << x << endl;
  else
    cout << "x (2) = " << x << endl;

  x=1./0.;
  x=x*x;
  
  printf(" x=%e \n",x);
  if( x==x*2. )
    cout << "x (1) = " << x << endl;
  else
    cout << "x (2) = " << x << endl;

}
