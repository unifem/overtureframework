/* Roe Solver for two component Euler equations and associated
 * functions.
 *
 * Jeff Banks
 * 4-26-04
 */

#include <math.h>
#include <stdio.h>
#include "eigen.h"
#include "macros.h"

#define TOL 0.001

void pressure (double rho, double e_hat, double lam,
	       double *p, double *p1, double *p2, double *p3,
	       double *rp)
{
  double gam, gam_p, pie, pie_p;
  gam = gamma(lam);
  gam_p = gamma_prime(lam);
  pie = pi(lam);
  pie_p = pi_prime(lam);
  p[0] = (gam-1.0)*e_hat-gam*pie;
  p1[0] = lam*(-e_hat*gam_p+gam*pie_p+gam_p*pie)/(rho);
  p2[0] = gam-1.0;
  p3[0] = (e_hat*gam_p-gam*pie_p-gam_p*pie)/rho;
}

double sound_speed(double u, double v, double p1, double p2,
		   double p3, double H, double lambda)
{
  double c2;
  c2 = -0.5*p2*(u*u+v*v)+H*p2+lambda*p3+p1;
  if (c2 < 0)
    {
      printf ("Error computing sound speed!!!! -- aborting\n c^2=%e\n", c2);
      printf ("P1=%e, P2=%e, P3=%e\n", p1, p2, p3);
      exit(0);
    }
  return (sqrt(c2));
}

void get_alpha(double alpha[5], double left[5],
	       double right[5], double el[5][5])
{
  int i, j;

  for (i = 0; i < 5; i++)
    {
      alpha[i] = 0.0;
      for (j = 0; j < 5; j++)
        {
          alpha[i] = alpha[i]+el[i][j]*(right[j]-left[j]);
        }
    }

  return;
}

void flux (double f[5], double u[5], double p,
	   double a, double b, double J, double gu, double gv)
{
  double v1, v2, f1[5], g1[5];
  int i;

  v1 = u[1]/u[0];
  v2 = u[2]/u[0];

  f1[0] = u[1];
  f1[1] = u[0]*v1*v1+p;
  f1[2] = u[0]*v1*v2;
  f1[3] = v1*(u[3]+p);
  f1[4] = v1*u[4];

  g1[0] = u[2];
  g1[1] = u[0]*v1*v2;
  g1[2] = u[0]*v2*v2+p;
  g1[3] = v2*(u[3]+p);
  g1[4] = v2*u[4];

  for (i = 0; i < 5; i++)
    {
      f[i] = J*(a*(f1[i]-gu*u[i])+b*(g1[i]-gv*u[i]));
    }

  return;
}

/* Actual Roe solver 
 */
void roe(double* left, double* right, 
	 double* f, double* max_speed,
	 double a1, double b1, double det, double *rp,
	 double gu, double gv)
{
  /* Roe average quantities */
  double u_T, v_T, H_T, L_T, p1_T, p2_T, p3_T, c_T;
  /* Roe average eigenvalues */
  double lambda[5];
  /* Roe average eigenvectors */
  double er_T[5][5], el_T[5][5];
  /* wave strengths */
  double alpha[5];
  /* left and right primative variables */
  double rho_l, u_l, v_l, E_l, p_l, H_l, L_l;
  double p1_l, p2_l, p3_l, e_hat_l;
  double rho_r, u_r, v_r, E_r, p_r, H_r, L_r;
  double p1_r, p2_r, p3_r, e_hat_r;
  double rrl, rrr; /* root rho_l, and root rho_r */
  double p1_guess, p2_guess, p3_guess, A, B, C, D, temp, gam_T, pi_T, p_T, e_hat_T;
  /*double a, b, rad;*/
  int i;

  /*rad = sqrt(a1*a1+b1*b1);
    a = a1/rad;
    b = b1/rad;*/

  /* calculate right and left primative variables */
  rho_l = left[0];
  u_l = left[1]/rho_l;
  v_l = left[2]/rho_l;
  E_l = left[3];
  L_l = left[4]/rho_l;

  rho_r = right[0];
  u_r = right[1]/rho_r;
  v_r = right[2]/rho_r;
  E_r = right[3];
  L_r = right[4]/rho_r;

  e_hat_l = E_l-0.5*rho_l*(u_l*u_l+v_l*v_l);
  e_hat_r = E_r-0.5*rho_r*(u_r*u_r+v_r*v_r);
  pressure(rho_l, e_hat_l, L_l,
	   &p_l, &p1_l, &p2_l, &p3_l, rp);
  pressure(rho_r, e_hat_r, L_r,
	   &p_r, &p1_r, &p2_r, &p3_r, rp);

  /* calculate left and right enthalpy */
  H_r = (E_r+p_r)/rho_r;
  H_l = (E_l+p_l)/rho_l;

  /* calculate Roe averages */
  if (rho_l < 0 || rho_r < 0)
    {
      printf ("Error in Roe averaging, negative density!!! -- aborting\n");
      exit(0);
    }
  rrl = sqrt(rho_l);
  rrr = sqrt(rho_r);
  u_T = (rrl*u_l+rrr*u_r)/(rrl+rrr);
  v_T = (rrl*v_l+rrr*v_r)/(rrl+rrr);
  H_T = (rrl*H_l+rrr*H_r)/(rrl+rrr);
  L_T = (rrl*L_l+rrr*L_r)/(rrl+rrr);

  /*p1_guess = (p1_l+p1_r)/2.0;
    p2_guess = (p2_l+p2_r)/2.0;
    p3_guess = (p3_l+p3_r)/2.0;*/

  gam_T = gamma(L_T);
  pi_T = pi(L_T);
  p_T = (rrr*rrl*(gam_T-1.0)*(H_T-0.5*(u_T*u_T+v_T*v_T)))/gam_T-pi_T;
  e_hat_T = (p_T+gam_T*pi_T)/(gam_T-1.0);
  p3_guess = -1./(rrr*rrl)*(gamma_prime(L_T)*(pi_T-e_hat_T)+gam_T*pi_prime(L_T));
  p2_guess = gam_T-1.0;;
  p1_guess = -L_T*p3_guess;
  
  A = rho_r-rho_l;
  B = e_hat_r-e_hat_l;
  C = right[4]-left[4];
  D = p_r-p_l;
  temp = A*A+B*B+C*C;
  if (temp <= TOL)
    {
      p1_T = p1_guess;
      p2_T = p2_guess;
      p3_T = p3_guess;
    }
  else
    {
      temp = (D-A*p1_guess-B*p2_guess-C*p3_guess)/temp;
      p1_T = temp*A+p1_guess;
      p2_T = temp*B+p2_guess;
      p3_T = temp*C+p3_guess; 
    }
  c_T = sound_speed(u_T, v_T, p1_T, p2_T, p3_T, H_T, L_T);

  /* compute Roe average eigenvalues and eigenvectors */
  eigen_calc(lambda, er_T, el_T, u_T, v_T, p2_T, p3_T, 
	     c_T, H_T, L_T, a1, b1, det, gu, gv);

  /* determine max eigenvalue */
  for (i = 0; i < 5; i++)
    {
      max_speed[0] = max(max_speed[0], fabs(lambda[i]/det));
    }

  /* compute wave strengths */
  get_alpha(alpha, left, right, el_T);

  /* calculate numerical flux */
  if (lambda[1]*det <= 0)
    {
      flux (f, right, p_r, a1, b1, det, gu, gv);
      if (lambda[4]*det > 0)
	{
	  /* no rarefaction check for now */
	  for (i = 0; i < 5; i++)
	    {
	      f[i] = f[i]-alpha[4]*lambda[4]*er_T[i][4];
	    }
	}
    }
  else
    {
      flux (f, left, p_l, a1, b1, det, gu, gv);
      if (lambda[0]*det < 0)
	{
	  /* no rarefaction check for now */
	  for (i = 0; i < 5; i++)
	    {
	      f[i] = f[i]+alpha[0]*lambda[0]*er_T[i][0];
	    }
	}
    }
  return;
}

/* HLL solver */
void HLL(double* left, double* right, 
	 double* f, double* max_speed,
	 double a1, double b1, double det, double *rp,
	 double gu, double gv)
{
  double sl, sr, cl, cr;
  double rho_l, rho_r, ul, vl, ur, vr, L_l, L_r, E_l, E_r;
  double e_hat_l, e_hat_r, p_l, p_r;
  double p1_l, p2_l, p3_l, p1_r, p2_r, p3_r;
  double H_l, H_r;
  double a, b, rad, V_r, V_l;
  double fl[5], fr[5], temp;
  int i;

  rad = sqrt(a1*a1+b1*b1);
  a = a1/rad;
  b = b1/rad;

  rho_l = left[0];
  ul = left[1]/rho_l;
  vl = left[2]/rho_l;
  L_l = left[4]/rho_l;
  E_l = left[3];

  rho_r = right[0];
  ur = right[1]/rho_r;
  vr = right[2]/rho_r;
  L_r = right[4]/rho_r;
  E_r = right[3];

  e_hat_l = E_l-0.5*rho_l*(ul*ul+vl*vl);
  e_hat_r = E_r-0.5*rho_r*(ur*ur+vr*vr);

  pressure(rho_l, e_hat_l, L_l,
	   &p_l, &p1_l, &p2_l, &p3_l, rp);
  pressure(rho_r, e_hat_r, L_r,
	   &p_r, &p1_r, &p2_r, &p3_r, rp);

  /*  H_r = (E_r+p_r)/rho_r;
      H_l = (E_l+p_l)/rho_l;
      
      cl = sound_speed(ul, vl, p1_l, p2_l, p3_l, H_l, L_l);
      cr = sound_speed(ur, vr, p1_r, p2_r, p3_r, H_r, L_r);*/
  cl = sqrt(gamma(L_l)*(p_l+pi(L_l))/rho_l);
  cr = sqrt(gamma(L_r)*(p_r+pi(L_r))/rho_r);
  
  V_r = (a*(ur-gu)+b*(vr-gv));
  V_l = (a*(ul-gu)+b*(vl-gv));
  
  sl = det*rad*min((V_l-cl), (V_r-cr));
  sr = det*rad*max((V_l+cl), (V_r+cr));
  /*printf ("in HLL: sl=%e sr=%e a=%e b=%e\n", sl, sr, a1, b1);*/

  /* determine max eigenvalue for time stepping */
  max_speed[0] = max(max_speed[0], fabs(sl/det));
  max_speed[0] = max(max_speed[0], fabs(sr/det));

  if (det*sl >= 0)
    {
      flux (f, left, p_l, a1, b1, det, gu, gv);
      return;
    }
  else if (det*sr <= 0)
    {
      flux (f, right, p_r, a1, b1, det, gu, gv);
      return;
    }
  else
    {
      flux (fl, left, p_l, a1, b1, det, gu, gv);
      flux (fr, right, p_r, a1, b1, det, gu, gv);
      temp = 1.0/(sr-sl);
      for (i = 0; i < 5; i++)
	{
	  f[i] = temp*(sr*fl[i]-sl*fr[i]+sr*sl*(right[i]-left[i]));
	}
      return ;
    }
}


/* HLLC solver (C is for contact)
 * as presented in Toro */
void HLLC(double* left, double* right, 
	  double* f, double* max_speed,
	  double a1, double b1, double det, double *rp,
	  double gu, double gv)
{
  double sl, sr, s_star, cl, cr;
  double rho_l, rho_r, ul, vl, ur, vr, L_l, L_r, E_l, E_r;
  double e_hat_l, e_hat_r, p_l, p_r;
  double p1_l, p2_l, p3_l, p1_r, p2_r, p3_r;
  double H_l, H_r;
  double a, b, rad, V_r, V_l;
  double fl[5], fr[5], temp;
  double rho_bar, c_bar;
  int i;

  rad = sqrt(a1*a1+b1*b1);
  a = a1/rad;
  b = b1/rad;

  rho_l = left[0];
  ul = left[1]/rho_l;
  vl = left[2]/rho_l;
  L_l = left[4]/rho_l;
  E_l = left[3];

  rho_r = right[0];
  ur = right[1]/rho_r;
  vr = right[2]/rho_r;
  L_r = right[4]/rho_r;
  E_r = right[3];

  e_hat_l = E_l-0.5*rho_l*(ul*ul+vl*vl);
  e_hat_r = E_r-0.5*rho_r*(ur*ur+vr*vr);

  pressure(rho_l, e_hat_l, L_l,
	   &p_l, &p1_l, &p2_l, &p3_l, rp);
  pressure(rho_r, e_hat_r, L_r,
	   &p_r, &p1_r, &p2_r, &p3_r, rp);

  /*H_r = (E_r+p_r)/rho_r;
    H_l = (E_l+p_l)/rho_l;
    
    cl = sound_speed(ul, vl, p1_l, p2_l, p3_l, H_l, L_l);
    cr = sound_speed(ur, vr, p1_r, p2_r, p3_r, H_r, L_r);*/
  cl = sqrt(gamma(L_l)*(p_l+pi(L_l))/rho_l);
  cr = sqrt(gamma(L_r)*(p_r+pi(L_r))/rho_r);

  V_r = a*ur+b*vr;
  V_l = a*ul+b*vl;

  sl = det*rad*min((V_l-cl), (V_r-cr));
  sr = det*rad*max((V_l+cl), (V_r+cr));
  rho_bar = 0.5*(rho_l+rho_r);
  c_bar = 0.5*(cl+cr);
  s_star = det*rad*0.5*(V_l+V_r-(p_r-p_l)/(rho_bar*c_bar));

  /* determine max eigenvalue for time stepping */
  max_speed[0] = max(max_speed[0], fabs(sl/det));
  max_speed[0] = max(max_speed[0], fabs(sr/det));

  if (det*sl >= 0)
    {
      flux (f, left, p_l, a1, b1, det, gu, gv);
      return ;
    }
  else if (det*sr <= 0)
    {
      flux (f, right, p_r, a1, b1, det, gu, gv);
      return;
    }
  /*********************************************/
  else if (det*sl <= 0 && det*s_star >= 0)
    {
      flux (fl, left, p_l, a1, b1, det, gu, gv);
      for (i = 0; i < 5; i++)
	{
	  f[i] = fl[i]+sl;
	}
    }
  else
    {
      flux (fl, left, p_l, a1, b1, det, gu, gv);
      flux (fr, right, p_r, a1, b1, det, gu, gv);
      temp = det*1.0/(sr-sl);
      for (i = 0; i < 5; i++)
	{
	  f[i] = temp*(sr*fl[i]-sl*fr[i]+sr*sl*(right[i]-left[i]));
	}
      return ;
    }
}
