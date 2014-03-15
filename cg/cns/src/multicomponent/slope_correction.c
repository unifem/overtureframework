#include "riemann.h"
#include "eigen.h"
#include <math.h>
#include "macros.h"

void minmod(double alpha[5], double alpha1[5], double alpha2[5])
{
  int i;
  
  /*for (i=0; i<5; i++)
    {
    alpha[i]=0.5*(alpha1[i]+alpha2[i]);
    }
    return;*/
    
  for (i=0; i < 5; i++)
    {
      if (alpha1[i]*alpha2[i] <= 0)
	{
	  alpha[i] = 0.0;
	}
      else if (fabs(alpha2[i]) < fabs(alpha1[i]))
	{
	  alpha[i] = alpha2[i];
	}
      else
	{
	  alpha[i] = alpha1[i];
	}
    }
  return;
}

void get_prim (double *u, double *prim, double *rp)
{
  double gam, pie;
  if (u[0] <= -.001 || u[3] <= -.001)
    {
      printf ("error converting to primative variables\n");
      printf ("E=%e, rho=%e\n", u[3], u[0]);
      exit (0);
    }
  prim[0] = u[0];
  prim[1] = u[1]/u[0];
  prim[2] = u[2]/u[0];
  prim[4] = u[4]/u[0];
  gam = gamma(prim[4]);
  pie = pi(prim[4]);
  prim[3] = (gam-1.0)*(u[3]-0.5*u[0]*(prim[1]*prim[1]+prim[2]*prim[2]))-gam*pie;
  
  return ;
}

void get_con (double *u, double *prim, double *rp)
{
  double gam, pie;
  gam = gamma(prim[4]);
  pie = pi(prim[4]);
  if (prim[3] <= -.001 || prim[0] <= -.001)
    {
      printf ("error converting to conservative variables\n");
      printf ("P=%e, rho=%e\n", prim[3],prim[0]);
      exit (0);
    }
  u[0] = prim[0];
  u[1] = prim[0]*prim[1];
  u[2] = prim[0]*prim[2];
  u[3] = (prim[3]+gam*pie)/(gam-1.0)+0.5*prim[0]*(prim[1]*prim[1]+prim[2]*prim[2]);
  u[4] = prim[0]*prim[4];

  return;
}

void slope (double u[5], double ut[5],
	    double ub[5], double ur[5],
	    double ul[5], double un[5], 
	    double us[5], double ue[5], 
	    double uw[5], double cr, 
	    double cs, double a1,
	    double b1, double a2,
	    double b2, double J,
	    int order, int high_order_prim,
	    double dt, double *rp,
	    int twilightZone, double *h,
	    int move, double gu, double gv, double acm)
{
  double alpha_l[5], alpha_r[5], alpha[5];
  double lambda[5], el[5][5], er[5][5];
  double prim_l[5], prim_r[5], prim[5];
  double primn[5], prims[5], prime[5], primw[5];
  double temp, temp2;
  int k, s;
  double L, v1, v2, e_hat, p, p1, p2, p3, H, c;
  double xi, yi, ti;

  if (order == 1)
    {
      for (k = 0; k < 5; k++)
	{
	  un[k] = u[k];
	  us[k] = u[k];
	  ue[k] = u[k];
	  uw[k] = u[k];
	}
    }
  else if (high_order_prim == 1) /* do high order correction in primative variables */
    {
      /* first deal with corrections in the (a1,b1) direction */
      /* find primative variables for states */
      get_prim (ul, prim_l, rp);
      get_prim (u, prim, rp);
      get_prim (ur, prim_r, rp);
	      
      /* get eigenvalues for these primative variables */
      eigen_prim (lambda, er, el, prim, a1, b1, J, rp, gu, gv);
      
      /* get alphas */
      get_alpha (alpha_l, prim_l, prim, el);
      get_alpha (alpha_r, prim, prim_r, el);
      minmod (alpha, alpha_l, alpha_r);
      for (k = 0; k < 5; k++)
	{
	  alpha[k] = alpha[k]/2.0;
	  primn[k] = prim[k];
	  prims[k] = prim[k];
	  prime[k] = prim[k];
	  primw[k] = prim[k];
	}
      
      /* determine correction bit.*/
      for (s = 0; s < 5; s++)
	{
	  /* add correction onto top and bottom */
	  temp = -cr*lambda[s]*alpha[s]/J;
	  for (k = 0; k < 4; k++)
	    {
	      primn[k] += temp*er[k][s];
	      prims[k] += temp*er[k][s];
	    }
	  /* ACM */
	  k = 4;
	  primn[k] += acm*temp*er[k][s];
	  prims[k] += acm*temp*er[k][s];
	  /** ACM **/
	  /* upwind for west and east corrections */
	  temp = (-1.0-min(0,lambda[s]*cr/J))*alpha[s];
	  temp2 = (1.0-max(0,lambda[s]*cr/J))*alpha[s];
	  for (k=0; k<4; k++)
	    {
	      primw[k] += temp*er[k][s];
	      prime[k] += temp2*er[k][s];
	    }
	  /* ACM */
	  k = 4;
	  primw[k] += acm*temp*er[k][s];
	  prime[k] += acm*temp2*er[k][s];
	  /** ACM **/
	}
      
      /************************************
       * now deal with (a2,b2)direction */
      
      /* find primative variables for states */
      get_prim (ub, prim_l, rp);
      get_prim (ut, prim_r, rp);
	      
      /* get eigenvalues for these primative variables */
      eigen_prim (lambda, er, el, prim, a2, b2, J, rp, gu, gv);
	      
      /* get alphas */
      get_alpha (alpha_l, prim_l, prim, el);
      get_alpha (alpha_r, prim, prim_r, el);
      minmod (alpha, alpha_l, alpha_r); 
      for (k = 0; k < 5; k++)
	{
	  alpha[k] = alpha[k]/2.0;
	}
      
      /* determine correction bit.*/
      for (s = 0; s < 5; s++)
	{
	  /* add correction onto left and right */
	  temp = -cs*lambda[s]*alpha[s]/J;
	  for (k = 0; k < 4; k++)
	    {
	      primw[k] += temp*er[k][s];
	      prime[k] += temp*er[k][s];
	    }
	  /* ACM */
	  k = 4;
	  primw[k] += 2.0*temp*er[k][s];
	  prime[k] += 2.0*temp*er[k][s];
	  /** ACM **/
	  /* upwind for south and north corrections */
	  temp = (-1.0-min(0,lambda[s]*cs/J))*alpha[s];
	  temp2 = (1.0-max(0,lambda[s]*cs/J))*alpha[s];
	  for (k=0; k<4; k++)
	    {
	      prims[k] += temp*er[k][s];
	      primn[k] += temp2*er[k][s];
	    }
	  /* ACM */
	  k = 4;
	  prims[k] += 2.0*temp*er[k][s];
	  primn[k] += 2.0*temp2*er[k][s];
	  /** ACM **/
	}
      
      /* convert back into conserved variables */
      get_con(un, primn, rp);
      get_con(us, prims, rp);
      get_con(ue, prime, rp);
      get_con(uw, primw, rp);
      
      /* Here we add on the source if it is needed. */
      if (twilightZone == 1)
	{
	  for (k = 0; k < 5; k++)
	    {
	      temp = 0.5*dt*h[k];
	      un[k] += temp;
	      us[k] += temp;
	      ue[k] += temp;
	      uw[k] += temp;
	    }
	}
    }
  else /* do high order correction in conservative variables */
    {
      /* first deal with corrections in the (a1,b1) direction */
      /* Determine quantities to determine Jacobian */
      L = u[4]/u[0];
      v1 = u[1]/u[0];
      v2 = u[2]/u[0];
      e_hat = (u[3]-0.5*u[0]*(v1*v1+v2*v2));
      pressure (u[0], e_hat, L, 
		&p, &p1, &p2, &p3, rp);
      H = (u[3]+p)/u[0];
      c = sound_speed (v1, v2, p1, p2, p3, H, L);
      
      /* get eigenvalues and eigen vectors */
      eigen_calc(lambda, er, el, v1, v2, p2, p3, c, H, L, a1, b1, J, gu, gv);
      
      /* get alphas */
      get_alpha (alpha_l, ul, u, el);
      get_alpha (alpha_r, u, ur, el);
      minmod (alpha, alpha_l, alpha_r);
      for (k = 0; k < 5; k++)
	{
	  alpha[k] = alpha[k]/2.0;
	  un[k] = u[k];
	  us[k] = u[k];
	  ue[k] = u[k];
	  uw[k] = u[k];
	}
      
      /* determine correction bit.*/
      for (s = 0; s < 5; s++)
	{
	  /* add correction onto top and bottom */
	  temp = -cr*lambda[s]*alpha[s]/J;
	  for (k = 0; k < 4; k++)
	    {
	      un[k] += temp*er[k][s];
	      us[k] += temp*er[k][s];
	    }
	  /* ACM */
	  k = 4;
	  un[k] += acm*temp*er[k][s];
	  us[k] += acm*temp*er[k][s];
	  /** ACM **/
	  /* upwind for west and east corrections */
	  temp = (-1.0-min(0,lambda[s]*cr/J))*alpha[s];
	  temp2 = (1.0-max(0,lambda[s]*cr/J))*alpha[s];
	  for (k=0; k<5; k++)
	    {
	      primw[k] += temp*er[k][s];
	      prime[k] += temp2*er[k][s];
	    }
	  /* ACM */
	  k = 4;
	  uw[k] += acm*temp*er[k][s];
	  ue[k] += acm*temp2*er[k][s];
	  /** ACM **/
	}
      
      /************************************
       * now deal with (a2,b2)direction */
      
      /* get eigenvalues and eigen vectors */
      eigen_calc(lambda, er, el, v1, v2, p2, p3, c, H, L, a2, b2, J, gu, gv);
      
      /* get alphas */
      get_alpha (alpha_l, ub, u, el);
      get_alpha (alpha_r, u, ut, el);
      minmod (alpha, alpha_l, alpha_r); 
      for (k = 0; k < 5; k++)
	{
	  alpha[k] = alpha[k]/2.0;
	}
      
      /* determine correction bit.*/
      for (s = 0; s < 5; s++)
	{
	  /* add correction onto left and right */
	  temp = -cs*lambda[s]*alpha[s]/J;
	  for (k = 0; k < 5; k++)
	    {
	      uw[k] += temp*er[k][s];
	      ue[k] += temp*er[k][s];
	    }
	  /* ACM */
	  k = 4;
	  uw[k] += acm*temp*er[k][s];
	  ue[k] += acm*temp*er[k][s];
	  /** ACM **/
	  /* upwind for south and north corrections */
	  temp = (-1.0-min(0,lambda[s]*cs/J))*alpha[s];
	  temp2 = (1.0-max(0,lambda[s]*cs/J))*alpha[s];
	  for (k=0; k<5; k++)
	    {
	      prims[k] += temp*er[k][s];
	      primn[k] += temp2*er[k][s];
	    }
	  /* ACM */
	  k = 4;
	  us[k] += acm*temp*er[k][s];
	  un[k] += acm*temp2*er[k][s];
	  /** ACM **/
	}
      /* Here we add on the source if it is needed. 
	 MAS = method of analytic solution. */
      if (twilightZone == 1)
	{
	  for (k = 0; k < 5; k++)
	    {
	      temp = 0.5*dt*h[k];
	      un[k] += temp;
	      us[k] += temp;
	      ue[k] += temp;
	      uw[k] += temp;
	    }
	}
    }
  return;
}
