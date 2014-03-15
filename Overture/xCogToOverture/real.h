#ifndef real_h
#define real_h

#ifdef SINGLE

/* 32 bits floating point precision */
#define NEWTON_EPS 1.0e-5
//typedef float real;

#else

/* 64 bits floating point precision */
#define NEWTON_EPS 1.0e-10
#ifndef NO_REAL
typedef double real;
#endif

#endif

#endif
