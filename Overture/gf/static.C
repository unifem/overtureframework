// include "OvertureInit.h"
#include "Overture.h"


class A
{
 public:

  A(){}
  ~A(){}

  int f( RealArray & f = Overture::nullRealArray() )
  {
    f.display("here is f");
    return 0;
  }
  
};
  

int
main(int argc, char *argv[])
{
  printf("Start main\n");
  Overture::start(argc,argv);
  
  A a;
  a.f();

  Overture::finish();
  printf("End main\n");
  return 0;
}
