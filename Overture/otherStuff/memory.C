#include <sys/resource.h>
#include <unistd.h>

int 
getResidentSetSize(double & residentSetSize)
{
  struct rusage temp;
  getrusage(RUSAGE_SELF,&temp);
  residentSetSize = ( (double) getpagesize())*( (double) temp.ru_maxrss );
  return 0;
}
