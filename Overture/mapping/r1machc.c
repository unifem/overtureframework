#include <stdio.h>
#include <float.h>
#include <limits.h>
#include <math.h>

#include "OvertureDefine.h"

#define r1machc EXTERN_C_NAME(r1machc)
#define d1machc EXTERN_C_NAME(d1machc)
#define i1machc EXTERN_C_NAME(i1machc)

void
r1machc(int *i, float *x)
{
  switch(*i)
  {
  case 1: *x=FLT_MIN; break;
  case 2: *x=FLT_MAX; break;
  case 3: *x=FLT_EPSILON/FLT_RADIX; break;
  case 4: *x=FLT_EPSILON; break;
  case 5: *x=log10((double)FLT_RADIX); break;
  default:
    fprintf(stderr, "invalid argument: r1mach(%d)\n", *i);
    *x=0.;
  }
}

void
d1machc(int *i, double *x)
{
  switch(*i)
  {
  case 1: *x=DBL_MIN; break;
  case 2: *x=DBL_MAX; break;
  case 3: *x=DBL_EPSILON/FLT_RADIX; break;
  case 4: *x=DBL_EPSILON; break;
  case 5: *x=log10((double)FLT_RADIX); break;
  default:
  fprintf(stderr, "invalid argument: d1mach(%d)\n", *i);
    *x=0.;
  }
}


void
i1machc(int *i, int *x)
{
  switch(*i){
  case 1:  *x=5;    /* standard input */ break;
  case 2:  *x=6;    /* standard output */ break;
  case 3:  *x=7;    /* standard punch */ break;
  case 4:  *x=0;    /* standard error */ break;
  case 5:  *x=32;   /* bits per integer */ break;
  case 6:  *x=sizeof(int); break;
  case 7:  *x=2;    /* base for integers */ break;
  case 8:  *x=31;   /* digits of integer base */ break;
  case 9:  *x=INT_MAX; break;
  case 10: *x=FLT_RADIX; break;
  case 11: *x=FLT_MANT_DIG; break;
  case 12: *x=FLT_MIN_EXP; break;
  case 13: *x=FLT_MAX_EXP; break;
  case 14: *x=DBL_MANT_DIG; break;
  case 15: *x=DBL_MIN_EXP; break;
  case 16: *x=DBL_MAX_EXP; break;
  default:
    fprintf(stderr, "invalid argument: i1mach(%d)\n", *i);
    *x=0;
  }
}
