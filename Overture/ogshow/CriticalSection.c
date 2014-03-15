/*
 *  Critical Section -- Wrapper for PThreads MUTEXes
 *   * Currently only used by mogl & GL_GraphicsInterface for Power Wall
 *
 *   Feb 20, 2001
 *   Who to blame: pf
 */
#include <stdio.h>
#include <assert.h>
#include <math.h>

#include "CriticalSection.h"

#define PF_DEBUG  1

#ifdef USE_POWERWALL
/* protect global data */
static pthread_mutex_t csMutex = PTHREAD_MUTEX_INITIALIZER;
#else
static pthread_mutex_t csMutex = 0;
#endif

static char csUnknown[] = "unknown";

int InitCriticalSection( CriticalSection *cs ) 
{
  int iflag=1;
#ifdef USE_POWERWALL
  pthread_mutex_lock( &csMutex );
  iflag = pthread_mutex_init( &cs->mutex, NULL  );
  iflag = iflag && pthread_mutex_init( &cs->active_mutex, NULL);
#endif
  cs->counter = 0;
  cs->name = csUnknown;
  cs->isActive = 1; /* true */
#ifdef USE_POWERWALL
  pthread_mutex_unlock( &csMutex );
#endif
  return( iflag );
}

void NameCriticalSection( CriticalSection *cs, char *name)
{
  BeginCriticalSection( cs );
  cs->name = name;
  EndCriticalSection( cs );
}

/* begin/end critical section */
void BeginCriticalSection( CriticalSection *cs)
{
  int active;
#ifdef USE_POWERWALL
#ifdef PF_DEBUG
  pthread_mutex_lock( &cs->active_mutex );   active=cs->isActive;
  pthread_mutex_unlock( &cs->active_mutex );
#else
  active=1;
#endif
  if( active ) pthread_mutex_lock( &( cs->mutex ) );
#endif
  cs->counter++;
  if (cs->counter>1)   {
    printf("BeginCriticalSection:: warning, %i threads accessing critical section `%s'.\n",
	   cs->counter, cs->name);
  }
}

void EndCriticalSection( CriticalSection *cs )
{
  int active;

  cs->counter--;
#ifdef USE_POWERWALL
#ifdef PF_DEBUG
  pthread_mutex_lock( &cs->active_mutex );   active=cs->isActive;
  pthread_mutex_unlock( &cs->active_mutex );
#else
  active=1;
#endif
  if( active ) pthread_mutex_unlock( &cs->mutex );
#endif
}

/* not threadsafe, but should always return <=1 */
int getNumberOfCriticalThreads( CriticalSection *cs )
{
  return( cs->counter );
}

void CriticalSectionOn( CriticalSection *cs )
{
#ifdef USE_POWERWALL
  printf("Critical section %s is ON.\n", cs->name);
  pthread_mutex_lock( &cs->active_mutex );  
  cs->isActive = 1; /* true */
  pthread_mutex_unlock( &cs->active_mutex );
#endif
}

void CriticalSectionOff( CriticalSection *cs )
{
#ifdef USE_POWERWALL
  printf("Critical section %s is OFF.\n", cs->name);
  fflush(NULL);
  pthread_mutex_lock( &cs->active_mutex );  
  cs->isActive = 0; /* false */
  pthread_mutex_unlock( &cs->active_mutex );
#endif
}
