#include "FortranIO.h"

int 
main(int argc, char **argv)
{
  Overture::start(argc,argv);  // initialize Overture

  aString name="tfio.dat";
  
  FortranIO fio;
  fio.open((const char*)name,"unformatted","unknown");

  int i=5;
  fio.print(i);
  real x=7.;
  fio.print(x);
  
  int dim[2]={3,4};
  realArray a(dim[0],dim[1]);
  a=4.;
  fio.print(dim,2);
  fio.print(a);

  // save a view
  realArray b(10);
  b.seqAdd(0.,1.);
  dim[0]=3, dim[1]=8;
  fio.print(dim,2);
  fio.print(b(Range(dim[0],dim[1])));

  realArray & u = b(Range(dim[0],dim[1]));
  printf(" u.getDataOffset(0)=%i \n",u.getDataOffset(0));

  fio.close();
  
  // We must read back just as we wrote

  fio.open((const char*)name,"unformatted","old");

  i=-1;
  fio.read(i);
  printf("i=%i (=5?)\n",i);

  x=-1.;
  fio.read(x);
  printf("x=%e (=7?)\n",x);
  
  dim[0]=dim[1]=-1;
  fio.read(dim,2);
  a.redim(dim[0],dim[1]);
  a=-1.;
  fio.read(a);
  a.display("Here is a. (=4?)");

  dim[0]=dim[1]=-1;
  fio.read(dim,2);
  b=-1.;
  fio.read(b(Range(dim[0],dim[1])));
  b(Range(dim[0],dim[1])).display("Here is b. (3..8?)");

  fio.close();

  Overture::finish();          
  return 0;

}
