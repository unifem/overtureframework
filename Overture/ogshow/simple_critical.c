/* SIMPLE Race condition & Critical Section */

#include <stdio.h>
#include <pthread.h>

#include "CriticalSection.h"

void* do_one(void *);
void* do_two(void *);
void do_wrapup(int, int);

#define JMAX 1000000
#define NUMTHINGS 10
int r1=0, r2=0, rtotal=0;

CriticalSection csThing;
char csName[] = "Thing";


/*pthread_mutex_t rtotal_mutex = PTHREAD_MUTEX_INITIALIZER;*/

void* do_one( void *pnum_times_v )
{
  int *pnum_times =(int*) pnum_times_v;
  int i,j, x, temp_total;

  x=0;
  
  for (i=0; i<NUMTHINGS ; i++)
  {
    /*pthread_mutex_lock( &rtotal_mutex );*/
    BeginCriticalSection( &csThing );
    temp_total= rtotal;
    printf("doing one thing\n"); fflush(NULL);
    for( j=0; j<JMAX; j++) x=x+i;
    (*pnum_times)++;
    rtotal = temp_total+1;
    EndCriticalSection( &csThing );
    /*pthread_mutex_unlock( &rtotal_mutex );*/
  }
  return NULL;
}


void* do_another( void *pnum_times_v )
{
  int *pnum_times =(int*) pnum_times_v;

  int i,j, x, temp_total;
  x=0;
  
  for (i=0; i<NUMTHINGS ; i++)
  {
    /*pthread_mutex_lock( &rtotal_mutex );*/
    BeginCriticalSection( &csThing );
    temp_total = rtotal;
    printf("doing another thing\n"); fflush(NULL);
    for( j=0; j<JMAX; j++) x=x+i;
    (*pnum_times)++;
    rtotal = temp_total+1;
    EndCriticalSection( &csThing );
    /*pthread_mutex_unlock( &rtotal_mutex );*/
  }

  return NULL;
}

void do_wrapup( int one_times, int another_times)
{
  int total;
  
  total = one_times + another_times;
  printf( "Wrap up: one thing %d,  another %d; total = %d (global total %d).\n",
	  one_times, another_times, total, rtotal);
  
}

extern int main( int argc, char **argv)
{

  pthread_t thread1, thread2;

  InitCriticalSection( &csThing );
  NameCriticalSection( &csThing, csName );

  if (argc>1) CriticalSectionOff( &csThing );

  pthread_create( &thread1, NULL,
		  do_one,
		  (void *) &r1);

  pthread_create( &thread2, NULL,
		  do_another,
		  (void *) &r2);
  
  pthread_join( thread1, NULL);
  pthread_join( thread2, NULL);

  do_wrapup( r1, r2);
  return 0;
}
