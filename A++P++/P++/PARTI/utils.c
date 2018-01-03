#include <stdio.h>
#include <math.h>
#include "bsparti.h"
#include "port.h"
#include "utils.h"

/* print an error message */
void error(str)
  char *str;
{
  printf("*** Error: Node %d : %s ***\n", PARTI_myproc(), str);
/* Code below this point added by Dan Quinlan! */
/* fflush(stdout); */
/* fflush(stderr); */
  fatal_error(str);
}

/* print an error message and exit */
void fatal_error(str)
  char *str;
{
  printf("*** Fatal error: Node %d : %s ***\n", PARTI_myproc(), str);
  /* and terminate processes on all nodes */
  PARTI_killall();
/*exit(-1);*/
  abort();
}

void init_array2(Z,dArrayPtr)
   DARRAY *dArrayPtr;
   int     *Z;
{
   int             i, j, node, blockSz, ghost1, ghost2;
 
   ghost1 = dArrayPtr->ghostCells[0];
   ghost2 = dArrayPtr->ghostCells[1];
   printf("NODE %d: ghost = %d, %d  dimVecL= %d,%d \n",
             PARTI_myproc(),ghost1,ghost2,dArrayPtr->dimVecL[0],dArrayPtr->dimVecL[1]);
   node = PARTI_myproc();
   for (i = 0; i < dArrayPtr->dimVecL[0]; i++) {
     for (j = 0; j < dArrayPtr->dimVecL[1]; j++) {
         Z[((i+ghost1)*(dArrayPtr->dimVecL[0]+(2*ghost1)))+j+ghost2] = node*100 + i*10 + j+1;
     }
   }

}


void init_array3(Z,dArrayPtr)
   DARRAY *dArrayPtr;
   int     Z[20][20][20];
{
   int             i, j, k, node, ghost1, ghost2, ghost3;
 
   ghost1 = dArrayPtr->ghostCells[0];
   ghost2 = dArrayPtr->ghostCells[1];
   ghost3 = dArrayPtr->ghostCells[2];
   node  = PARTI_myproc();

   for (i = ghost1; i < dArrayPtr->dimVecL[0]+ghost1; i++) {
     for (j = ghost2; j < dArrayPtr->dimVecL[1]+ghost2; j++) {
        for (k = ghost3; k < dArrayPtr->dimVecL[2]+ghost3; k++) {
           Z[i][j][k] = node*1000 + i*100 + j*10 + k + 1;
	}
     }
   }

}



/*
 * compute and return the smallest (prime) factor of n
 */
int least_prime_factor(n)
  int n;
{
  int i;

  if (n < 0) {
    error("can't factor a negative number");
    return n;
  }

  if (n < 2) return n;
  if (n % 2 == 0) return 2;

  for (i = 3; i <= sqrt(n); i+=2) {
    if (n % i == 0) return i;
  }

  return n;
}


/* return the largest prime factor of n */
int greatest_prime_factor(n)
  int n;
{
  int lpf;

  if (n < 0) {
    error("can't factor a negative number");
    return n;
  }

  /* after dividing out all the smaller prime factors, what's left is the */
  /* largest one */
  for (lpf = least_prime_factor(n); lpf != n; lpf = least_prime_factor(n)) {
    n = n/lpf;
  }

  return n;
}

  

