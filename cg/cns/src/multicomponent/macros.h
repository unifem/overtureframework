#ifndef __MACROS_H__
#define __MACROS_H__

#define GAMMA1 rp[40]
#define CV1 rp[41]
#define PI1 rp[42]
#define GAMMA2 rp[43]
#define CV2 rp[44]
#define PI2 rp[45]

#define min(a, b)  ((a) <= (b) ? (a) : (b))
#define max(a, b)  ((a) >= (b) ? (a) : (b))
#define gamma(a) (((GAMMA1*CV1*(a))+(GAMMA2*CV2*(1.0-(a))))/((CV1*(a))+(CV2*(1.0-(a)))))
#define gamma_prime(a) ((CV1*CV2*(GAMMA1-GAMMA2))/(pow((CV1*(a)+CV2-CV2*(a)), 2.)))
/*#define gamma(a) (((GAMMA1-1.)*(GAMMA2-1.))/((a)*(GAMMA1-1.)+(1.-(a))*(GAMMA2-1.))+1.)
  #define gamma_prime(a) ((GAMMA2-1.)*(GAMMA1-1.)*(GAMMA2-GAMMA1)/(pow(((a)*(GAMMA1-1.)+(1.-(a))*(GAMMA2-1.)),2.)))*/
/*#define pi(a) (((PI1*CV1*(a))+(PI2*CV2*(1.0-(a))))/((CV1*(a))+(CV2*(1.0-(a)))))
  #define pi_prime(a) ((CV1*CV2*(PI1-PI2))/(pow((CV1*(a)+CV2-CV2*(a)), 2.)))*/
#define pi(a) (PI1*(a)+PI2*(1.0-(a)))
#define pi_prime(a) (PI1-PI2)

/* Macros for indexing arrays that were 
 * allocated in a "non-c" fassion. */
#define ind3(i,j,k,n1,n2,n3) (((((k)*(n2))+(j))*(n1))+(i))
#define ind2(i,j,n1,n2) (((j)*(n1))+(i))

#endif
