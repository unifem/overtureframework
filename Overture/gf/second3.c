/*
 * $Header: /usr/gapps/overture/OvertureRepoCVS/overture/Overture/gf/second3.c,v 1.10 2008/12/15 18:50:39 henshaw Exp $
 */

#include "OvertureDefine.h"

#ifndef NO_BSD
#include <sys/time.h>
#include <sys/resource.h>
#ifdef OV_USE_DOUBLE
#define Float double
#else
#define Float float
#endif

#define second EXTERN_C_NAME(second)
#define secondf EXTERN_C_NAME(secondf)
#define ovtime EXTERN_C_NAME(ovtime)

/*  This function is here to avoid a load error with ibm xlC: *** fix me *** */
void _start()
{
}


/* This measures wall clock time */
#ifndef USE_PPP
void second( time ) Float *time;
{
  static struct timeval firstTime;
  static int called=0;
  
  struct timeval theTime;
  gettimeofday( &theTime, 0 );
  if (!called)
  {
    firstTime = theTime;
    called=1;
  }
  
  *time=(theTime.tv_sec - firstTime.tv_sec) + (theTime.tv_usec - firstTime.tv_usec)*1.e-6;
/*  printf("time inside second_: %e\n", *time); */
}

#else
/* in parallel use this */
void second( time ) Float *time;
{
  /*  time= MPI_Wtime();  */ /* this doesn't work right ? */
  *time=0.;
}
#endif


void secondf( time ) Float *time;
{
  second( time );
}

void ovtime( time ) Float *time;
{
  second( time );
}


/* -----

void second_(time)Float *time;
 {struct rusage itime;
  getrusage(RUSAGE_SELF,&itime);
  *time=.000001*(itime.ru_utime.tv_usec+itime.ru_stime.tv_usec)
   +itime.ru_utime.tv_sec+itime.ru_stime.tv_sec;
 }
 ---- */

/* define secondf for linux since there is already a second in g77 */
/* -----
void secondf_(time)Float *time;
 {struct rusage itime;
  getrusage(RUSAGE_SELF,&itime);
  *time=.000001*(itime.ru_utime.tv_usec+itime.ru_stime.tv_usec)
   +itime.ru_utime.tv_sec+itime.ru_stime.tv_sec;
 }
 ----- */

#endif

/* ----
int iand_(i,j)int *i,*j;{return(*i&*j);}

int ior_(i,j)int *i,*j;{return(*i|*j);}

int ishft_(i,nshift)int *i,*nshift;
 {int nbits;
  if(*nshift<0)
   {nbits= *nshift<-32?32:-*nshift;
    return((*i>>nbits)&(017777777777>>(nbits-1)));
   }else{
    nbits= *nshift%32;
    return((*i<<nbits)|((*i>>(32-nbits))&(~(037777777777<<nbits))));
   }
 }

 --- */
