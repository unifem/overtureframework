/* File contains functions to calculate eigen values and vecors
 * of the Jacobian of the two component Euler equations.
 */

#ifndef __EIGEN_H__
#define __EIGEN_H__

void eigen_prim (double lambda[5], double er[5][5], double el[5][5],
		 double prim[5], double a1, double b1, double J, double *rp,
		 double gu, double gv);


/* function to calculate eigen values and vectors for the two component
 * Euler equations in 2 dimensions. This is the eigen calculator for 
 * primative variables ... used for Roe solver.
 *
 * Here we return eigen values in lambda, right eigen vectors in er,
 * and left eigen vectors in el. Also a1,a2 are the "direction", and
 * a3 the "length." (a1,a2) should be unit vector ... see notes 
 * for full explanation.
 */
void eigen_calc (double lambda[5], double er[5][5], double el[5][5],
		 double u, double v, double p2, double p3, 
		 double c, double H, double L,
		 double a1, double b1, double J,
		 double gu, double gv);

#endif
