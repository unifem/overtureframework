/* Roe Solver for two component Euler equations and associated
 * functions.
 *
 * Jeff Banks
 * 4-26-04
 */

#ifndef __RIEMANN_H__
#define __RIEMANN_H__

/*double max (double a, double b);
  double min (double a, double b);
  double gamma (double lambda);
  double gamma_prime (double lambda);
  double pi (double lambda);
  double pi_prime (double lambda);*/
void pressure (double rho, double e_hat, double lam,
	       double* p, double* p1, double* p2, double* p3,
	       double *rp);
double sound_speed (double u, double v, double p1, double p2,
		    double p3, double H, double lambda);
void get_alpha (double alpha[5], double left[5],
		double right[5], double el[5][5]);
void flux (double f[5], double u[5], double p,
	   double a, double b, double J, double gu, double gv);
void roe (double* left, double* right, 
	  double* f, double* max_speed,
	  double a1, double b1, double det, double *rp,
	  double gu, double gv);
void HLL (double* left, double* right, 
	  double* f, double* max_speed,
	  double a1, double b1, double det, double *rp,
	  double gu, double gv);
void HLLC (double* left, double* right, 
	   double* f, double* max_speed,
	   double a1, double b1, double det, double *rp,
	   double gu, double gv);

#endif
