#include "Mapping.h"

int 
main(int argc, char *argv[])
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems

  char form[]="%i %i";
  aString iFormat=ftor(form); // (const char*)iFormat);
  const char *iformat = (const char *)iFormat;
  printf("iFormat=%s, iformat=%s \n",(const char *)iFormat,iformat);

  return 0;
}
