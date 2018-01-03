/*
 * p_hello.C -- a hello program (in pthread)
 *
 * Use: "CC test2000_16.C -lpthread" to compile
 *      "CC test2000_16.C" will appear to link properly, 
 *      but it will cause pthread_create() to return -1.
 */

#if 0
#include <stdio.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdlib.h>

#include <assert.h>
#else
#include<A++.h>
#endif

#define MAX_THREAD 1000

typedef struct
   {
     int id;
   } parm;

void*
hello(void *arg)
   {
     parm *p=(parm *)arg;
     printf("Hello from node %d\n", p->id);
     return (NULL);
   }

int
main(int argc, char* argv[])
   {
#ifdef USE_PTHREADS
     int n,i;
     int errorCode;
     pthread_t *threads;
     pthread_attr_t pthread_custom_attr;
     parm *p;

#if 0
     if (argc != 2)
        {
          printf ("Usage: %s n\n  where n is no. of threads\n",argv[0]);
          exit(1);
        }

     n=atoi(argv[1]);

     if ((n < 1) || (n > MAX_THREAD))
        {
          printf ("The no of thread should between 1 and %d.\n",MAX_THREAD);
          exit(1);
        }
#else
     n = 16;
#endif

     threads=(pthread_t *)malloc(n*sizeof(*threads));
     pthread_attr_init(&pthread_custom_attr);

     p=(parm *)malloc(sizeof(parm)*n);
     assert (p != NULL);

  /* Start up thread */

     for (i=0; i<n; i++)
        {
          p[i].id=i;
          errorCode = pthread_create(&threads[i], &pthread_custom_attr, hello, (void *)(p+i));
          assert (errorCode == 0);
        }

  /* Synchronize the completion of each thread. */

     for (i=0; i<n; i++)
        {
          pthread_join(threads[i],NULL);
        }
     free(p);
#else
     printf ("Pthreads not tested USE_PTHREADS not defined! \n");
#endif
   }
