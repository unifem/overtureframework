#include "Overture.h"


class A : public ReferenceCounting
{
 public:

  realCompositeGridFunction deep(realCompositeGridFunction& u);
  realCompositeGridFunction shallow(realCompositeGridFunction& u);
  realCompositeGridFunction deep2(realCompositeGridFunction& u);
  realCompositeGridFunction shallow2(realCompositeGridFunction& u);
};

realCompositeGridFunction 
A::deep(realCompositeGridFunction& u)
{
  return u;
}

realCompositeGridFunction 
A::shallow(realCompositeGridFunction& u)
{
  return realCompositeGridFunction(u,SHALLOW);
}

realCompositeGridFunction 
A::deep2(realCompositeGridFunction& u)
{
  return deep(u);
}

realCompositeGridFunction 
A::shallow2(realCompositeGridFunction& u)
{
  return shallow(u);
}





int 
main()
{
  CompositeGrid cg;
  getFromADataBase(cg,"/home/henshaw/res/ogen/valve");

  realCompositeGridFunction u(cg);

  A a;

  const int number=200;
  int i;
  real time0=getCPU();
  for( i=0; i<number; i++ )
  {
    a.deep(u);
  }
  real time=getCPU()-time0;
  printf("time for deep    = %e \n",time);

  time0=getCPU();
  for( i=0; i<number; i++ )
  {
    a.shallow(u);
  }
  time=getCPU()-time0;
  printf("time for shallow = %e \n",time);

  time0=getCPU();
  for( i=0; i<number; i++ )
  {
    a.deep2(u);
  }
  time=getCPU()-time0;
  printf("time for deep2   = %e \n",time);

  time0=getCPU();
  for( i=0; i<number; i++ )
  {
    a.shallow2(u);
  }
  time=getCPU()-time0;
  printf("time for shallow2= %e \n",time);


  return 0;
}
