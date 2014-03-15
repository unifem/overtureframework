#include "Overture.h"

#include "ArraySimpleReal.h"
#include "ArraySimpleInt.h"


int
main()
{

  ArraySimpleReal a(2,3);
  VectorSimpleReal b(5);

  ArraySimpleInt c(3,3);
  c=1;

  a=0.;
  a(1,2)=1.2;
  a(0,1)=1;
  
  b=1.;
  b(3)=3;
  
  cout << "Here is a " << a << endl;
  cout << "Here is b " << b << endl;
  cout << "Here is c " << c << endl;
  return 0;
}
