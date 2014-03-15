#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdio.h>

int 
getResidentSetSize(double & residentSetSize)
{
/* ----
  struct rusage temp;
  int ierr=getrusage(RUSAGE_SELF,&temp);
  printf(" ***ierr=%i, maximum resident set size=%e, page size=%e \n",ierr,
         (double)temp.ru_maxrss,(double) getpagesize());
  residentSetSize = ( (double) getpagesize())*( (double) temp.ru_maxrss );
---- */
  residentSetSize=0.;
  return 0;
}
