#include "mappings.h"

typedef struct {
	real	re;
	real	im;
} complex;

#define PI 3.14159265358979

#ifdef F_NO_UNDERSCORE
extern void 
invkarmanntrefftz();
extern void 
cg(); /*? Fortran and C return value problems... */
extern void 
translate();
extern void
cffti();
extern void
cfftb();
extern void
cfftf();
#else
extern void 
invkarmanntrefftz_();
extern void 
cg_(); /*? Fortran and C return value problems... */
extern void 
translate_();
extern void
cffti_();
extern void
cfftb_();
extern void
cfftf_();
#endif

void 
mktgcoeff(int npoints,real *points,tgcoefflist *tgcoeff){	
	real			*a, *an;
	real			beta, *b, *bn;
	complex			c;
	int			i,it,itmax=50;
	real			*logr;
	CPCspline		logrspline;
	int			n, nn, np;
	real			p[2];
	real			sum;
	real			*theta, theta_i, theta_t;
	real*			wsave;
	complex			*y, *y1, *y2;
	complex			z0,z1;
	complex         	*zc;
	
	n = 2*npoints;
	n = 64;
	zc = (complex *)malloc(npoints*sizeof(complex));
        memcpy(zc,points,sizeof(complex)*npoints);
	zc[0].re = zc[0].re+tgcoeff->dxte;
	zc[0].im = zc[0].im+tgcoeff->dyte;
	zc[npoints-1].re = zc[0].re;
	zc[npoints-1].im = zc[0].im;
#ifdef F_NO_UNDERSCORE
	invkarmanntrefftz(zc,&npoints,&beta,&z0,&z1,
			  &(tgcoeff->rte),&(tgcoeff->dtau));
	cg(&c, &npoints, zc);
	c = cneg(c);
	translate(&npoints,zc,&c);
#else
	invkarmanntrefftz_(zc,&npoints,&beta,&z0,&z1,
			   &(tgcoeff->rte),&(tgcoeff->dtau));
	cg_(&c, &npoints, zc);
	c = cneg(c);
	translate_(&npoints,zc,&c);
#endif
	
/* Time to calculate coefficients from the near circle... */

	np = npoints;
	nn = 2*n;
	a = (real *)malloc((n+1)*sizeof(real));
	b = (real *)malloc((n+1)*sizeof(real));
	memset(a,0,(n+1)*sizeof(real));
	memset(b,0,(n+1)*sizeof(real));
	logr = (real *)calloc(np,sizeof(real));
	theta = (real *)calloc(np,sizeof(real));
	y = (complex *)calloc(nn,sizeof(complex));
	wsave = (real *)calloc(4*nn+15,sizeof(real));

	theta_t = carg(zc[0]);
	for (i=0;i<np;i++)
	{
		theta[i] = carg(zc[i]);
		theta[i] = theta[i] + 2*PI*(i>0 && theta[i] < theta[i-1]);
		logr[i] = log(c_abs(zc[i]));
	}
	logrspline = mkCPCspline(aspointlist(	"Dummy",
				npoints,
				logr,
				theta));
	reparametrizeCPCspline(&logrspline,theta);
	free(logr);
	free(theta);
		
#ifdef F_NO_UNDERSCORE
	cffti(&nn,wsave);
#else
	cffti_(&nn,wsave);
#endif
	
/* Iteration loop for Theodorssen-Garrick transform */
	for (it=0;it<itmax;it++)
	{
		for (sum = 0, bn = b+1; bn < b+n;bn++)
			sum = sum + *bn;
                b[0] = theta_t - sum;
		y[0].re = b[0];
		y[0].im = 0;
		y[n].re = 0;
		y[n].im = 0;
	/*	y[n].im = a[n-1];*/
		for (an = a+1, bn = b+1, y1 = y+1, y2 = y+nn-1; y1 < y+n; y1++, y2--, an++, bn++)
		{
			y1->re = *bn/2;
			y1->im = *an/2;
			*y2 = cconjugate(*y1);
		}
#ifdef F_NO_UNDERSCORE
		cfftb(&nn,y,wsave);
#else
		cfftb_(&nn,y,wsave);
#endif
		for (i=0;i<nn;i++)
		{
			theta_i = y[i].re+2*PI*i/nn;
                        theta_i = theta_i+
			  2*PI*( (theta_i<theta_t) - (theta_i>(2*PI+theta_t)) );
			CPCp(p,theta_i,logrspline);
			y[i].re = p[0];
			y[i].im = 0.0;
		}

#ifdef F_NO_UNDERSCORE
		cfftf(&nn,y,wsave);
#else
		cfftf_(&nn,y,wsave);
#endif
		
		for (i=1;i<n;i++)
		{
			a[i] = 2*y[i].re/nn;
			b[i] = -2*y[i].im/nn;
		}
		a[0] = y[0].re/nn;
		a[n] = y[n].re/nn;
	}
/* End of loop */


	tgcoeff->npoints = n+1;
	tgcoeff->a = a;
	tgcoeff->b = b;
	tgcoeff->te = theta_t;
	tgcoeff->beta = beta;
	tgcoeff->z0 = z0;
	tgcoeff->z1 = z1;
	tgcoeff->c = cneg(c);
	
/* Clean up allocated memory */
	free(y);
	free(zc);
	free(wsave);

	free(logrspline.B);
	free(logrspline.ulist);
	free(logrspline.name);
       
} 






#include "mappings.h"

#define PI 3.14159265358979

#ifdef F_NO_UNDERSCORE
extern void karmanntrefftz();
extern void theodorsengarrick();
extern void translate();
#else
extern void karmanntrefftz_();
extern void theodorsengarrick_();
extern void translate_();
#endif

void mktggrid(real *x, real *y, real r, real s, tgcoefflist tgcoeff)
{	
	real		angle;
	real		radius;
	int		m;
	complex		z;

/* Make a grid-point by forward transformations */
	m = 1;
        radius = 1.+s;
        angle = r*2.*PI;
        z.re = radius*cos(angle);
        z.im = radius*sin(angle);
#ifdef F_NO_UNDERSCORE
	theodorsengarrick(
#else
	theodorsengarrick_(
#endif
		&z,
		&m,
		tgcoeff.a,
		tgcoeff.b,
		&(tgcoeff.npoints));
#ifdef F_NO_UNDERSCORE
	translate(&m,&z,&(tgcoeff.c));
#else
	translate_(&m,&z,&(tgcoeff.c));
#endif
#ifdef F_NO_UNDERSCORE
	karmanntrefftz(
#else
	karmanntrefftz_(
#endif
		&z,
		&m,
		&(tgcoeff.beta),
		&(tgcoeff.z0),
		&(tgcoeff.z1));
	*x = z.re;
	*y = z.im;
} 
