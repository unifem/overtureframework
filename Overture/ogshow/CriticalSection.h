/* Critical Sections -- for Overture/VDL */
#ifndef CRITICAL_SECTION_H
#define CRITICAL_SECTION_H

/* only include/replace PThreads if hasn't been included */
#ifndef PTHREAD_MUTEX_INITIALIZER	
#ifdef USE_POWERWALL
#include <pthread.h>
#else
/* DUMMY pthread interface */
typedef int pthread_mutex_t;
#endif
#endif

typedef struct {
  pthread_mutex_t mutex;
  int             counter;
  char            *name;
  int             isActive;
  pthread_mutex_t active_mutex;
} CriticalSection;


/* Function definitions */

int  InitCriticalSection  ( CriticalSection *cs);
void BeginCriticalSection( CriticalSection *cs);
void EndCriticalSection  ( CriticalSection *cs );

void CriticalSectionOn ( CriticalSection *cs );
void CriticalSectionOff( CriticalSection *cs );

void NameCriticalSection( CriticalSection *cs, char *name);
int getNumberOfCriticalThreads( CriticalSection *cs );

#endif
