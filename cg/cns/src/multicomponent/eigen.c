/* File contains functions to calculate eigen values and vecors
 * of the Jacobian of the two component Euler equations.
 */

#include "riemann.h"
#include <math.h>
#include "macros.h"

void eigen_prim (double lambda[5], double er[5][5], double el[5][5],
		 double prim[5], double a1, double b1, double J, double *rp,
		 double gu, double gv)
{
  double g, P, c, pie;
  double c2, c3, t1, t2, w;
  double a, b, rad;
  
  rad = sqrt(a1*a1+b1*b1);
  a = a1/rad;
  b = b1/rad;
  g = gamma(prim[4]);
  pie = pi(prim[4]);
  P = prim[3];
  if (g*(P+pie)/prim[0] < 0)
    {
      printf ("Error in eigenvector calculation for primative variables!!\n");
      printf ("P=%e, gamma=%e, rho=%e\n", P,g,prim[0]);
      exit (0);
    }
  c = sqrt(g*(P+pie)/prim[0]);
  c2 = c*c;
  c3 = c2*c;
  t1 = 1.0/(2.0*c3);
  t2 = 1.0/(2.0*c2*g*(P+pie));
  w = a*prim[1]+b*prim[2];

  /* eigen values */
  lambda[0] = J*rad*((w-c)-(a*gu+b*gv));
  lambda[1] = J*rad*(w-(a*gu+b*gv));
  lambda[2] = J*rad*(w-(a*gu+b*gv));
  lambda[3] = J*rad*(w-(a*gu+b*gv));
  lambda[4] = J*rad*((w+c)-(a*gu+b*gv));

  /* right eigen vecotrs */
  er[0][0] = -g*(P+pie);
  er[1][0] = a*c3;
  er[2][0] = b*c3;
  er[3][0] = -g*(P+pie)*c2;
  er[4][0] = 0.0;
  
  er[0][1] = 1.0;
  er[1][1] = 0.0;
  er[2][1] = 0.0;
  er[3][1] = 0.0;
  er[4][1] = 0.0;

  er[0][2] = 0.0;
  er[1][2] = -b;
  er[2][2] = a;
  er[3][2] = 0.0;
  er[4][2] = 0.0;

  er[0][3] = 0.0;
  er[1][3] = 0.0;
  er[2][3] = 0.0;
  er[3][3] = 0.0;
  er[4][3] = 1.0;
 
  er[0][4] = g*(P+pie);
  er[1][4] = a*c3;
  er[2][4] = b*c3;
  er[3][4] = g*(P+pie)*c2;
  er[4][4] = 0.0;

  /* left eigen vectors */
  el[0][0] = 0.0;
  el[1][0] = 1.0;
  el[2][0] = 0.0;
  el[3][0] = 0.0;
  el[4][0] = 0.0;

  el[0][1] = a*t1;
  el[1][1] = 0.0;
  el[2][1] = -b;
  el[3][1] = 0.0;
  el[4][1] = a*t1;
 

  el[0][2] = b*t1;
  el[1][2] = 0.0;
  el[2][2] = a;
  el[3][2] = 0.0;
  el[4][2] = b*t1;

  el[0][3] = -t2;
  el[1][3] = -1.0/c2;
  el[2][3] = 0.0;
  el[3][3] = 0.0;
  el[4][3] = t2;

  el[0][4] = 0.0;
  el[1][4] = 0.0;
  el[2][4] = 0.0;
  el[3][4] = 1.0;
  el[4][4] = 0.0;

  return;
}


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
		 double a1, double b1, double J, double gu, double gv)
{
  double w, w2, c2, t1, t2, t3;
  double a, b, rad;

  /* useful temporary quantities */
  rad = (sqrt(a1*a1+b1*b1));
  a = a1/rad;
  b = b1/rad;
  w = a*u+b*v;
  w2 = u*u+v*v;
  c2 = c*c;
  t1 = -L*p3-H*p2+p2*w2;
  t2 = 1.0/(2.0*c2);
  t3 = 1.0/c2;
  

  /* eigen values */
  lambda[0] = J*rad*((w-c)-(a*gu+b*gv));
  lambda[1] = J*rad*(w-(a*gu+b*gv));
  lambda[2] = J*rad*(w-(a*gu+b*gv));
  lambda[3] = J*rad*(w-(a*gu+b*gv));
  lambda[4] = J*rad*((w+c)-(a*gu+b*gv));

  /* right eigen vectors */
  er[0][0] = 1.0;
  er[1][0] = u-a*c;
  er[2][0] = v-b*c;
  er[3][0] = H-w*c;
  er[4][0] = L;
  
  er[0][1] = 0.0;
  er[1][1] = 0.0;
  er[2][1] = 0.0;
  er[3][1] = -p3/p2;
  er[4][1] = 1.0;

  er[0][2] = 1.0;
  er[1][2] = u;
  er[2][2] = v;
  er[3][2] = (p2*H+L*p3-c2)/p2;
  er[4][2] = 0.0;

  er[0][3] = 0.0;
  er[1][3] = -b;
  er[2][3] = a;
  er[3][3] = a*v-b*u;
  er[4][3] = 0.0;

  er[0][4] = 1.0;
  er[1][4] = u+a*c;
  er[2][4] = v+b*c;
  er[3][4] = H+w*c;
  er[4][4] = L;
  

  /* left eigen vectors */
  el[0][0] = t2*(t1+c2+c*w);
  el[1][0] = -L*t3*(t1+c2);
  el[2][0] = -t3*t1;
  el[3][0] = b*u-a*v;
  el[4][0] = t2*(t1+c2-c*w);

  el[0][1] = -t2*(p2*u+a*c);
  el[1][1] = t3*L*u*p2;
  el[2][1] = t3*u*p2;
  el[3][1] = -b;
  el[4][1] = -t2*(p2*u-a*c);

  el[0][2] = -t2*(p2*v+b*c);
  el[1][2] = t3*L*v*p2;
  el[2][2] = t3*v*p2;
  el[3][2] = a;
  el[4][2] = -t2*(p2*v-b*c);

  el[0][3] = t2*p2;
  el[1][3] = -t3*p2*L;
  el[2][3] = -t3*p2;
  el[3][3] = 0.0;
  el[4][3] = t2*p2;

  el[0][4] = t2*p3;
  el[1][4] = t3*(c2-L*p3);
  el[2][4] = -t3*p3;
  el[3][4] = 0.0;
  el[4][4] = t2*p3;

  return;
}
