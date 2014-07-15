/* This file contains functions to call and use the Roe solver for the
 * 2-component Euler equations. Here we call in the framework of a
 * Godunov solver and add in the optional pressure fix to keep the
 * ocilations in the neighborhood of changes in lambda to a minimum.
 *
 * Jeff Banks
 * 5-3-04
 */

#ifndef __DUDR2COMP__
#define __DUDR2COMP__

#include "slope_correction.h"
#include "riemann.h"
#include "fixes.h"
#include <stdio.h>
#include <math.h>
#include "macros.h"

#include "OvertureDefine.h"
#define second EXTERN_C_NAME(second)
// #define second second_

void twilightSource(void *exactP, double *rp, 
		    void *mgP, double x, double y,
		    double t, double h[5]);

/*
 * u - grid solution
 * du - grid update ... u(t=tn+dt) = u(t=tn)+dt*du
 * rx - grid metrics
 * det - grid Jacobians
 * rp - real parameters
 *   rp[0] = real part time stepping eigenvalue
 *   rp[1] = imaginary part of time stepping eigenvalue
 *   rp[30] = time to calculate fluxes
 *   rp[31] = time to calculate slope corrections
 *   rp[32] = time to calculate sources ... pressure correction
 *   rp[35] = time t
 *   rp[36] = viscosity parameter
 *   rp[37] = ds (grid spacing in axis-2)
 *   rp[38] = dr (grid spacing in axis-1)
 *   rp[39] = dt (time spacing)
 *   rp[40] = gamma1
 *   rp[41] = cv1
 *   rp[42] = pi1
 *   rp[43] = gamma2
 *   rp[44] = cv2
 *   rp[45] = pi2
 * ip - integer parameters
 *   ip[2] = moving grid flag
 *   ip[4] = order of calculation (1 or 2)
 *   ip[5] = method of Riemann calculation (1=Roe, 3=HLL)
 *   ip[10] = method of slope correction (1=primative, 0=conservative)
 *   ip[11] = flag to add on pressure fix (1=yes, 0=no)
 *   ip[12] = number of cells in axis-1
 *   ip[13] = number of cells in axis-2
 *   ip[14] = flag for twilight zone flow (1=on, 0=off)
 * workspace - large space of memory to work in
 * OGFunc - pointer to C++ object passed through to twilightSource.C
 * mg - pointer to C++ object passed through to twilightSource.C
 * vert - vertex information on the current grid (for twilight calculations)
 */
int dudr2comp (double *u, 
	       double *du,
	       double *rx, double *det, double *gvel,
	       double *rx2, double *det2, double *gvel2,
	       double *rp, 
	       int *ip, 
	       double *workspace,
	       void *OGFunc, void *mg, 
	       double *vert)
{
  int slopeCorr =    ip[10];
  int fix =          ip[11];
  int NR =           ip[12];
  int NS =           ip[13];
  int twilightZone = ip[14];
  int move =         ip[2];
  int order =        ip[4];
  int meth =         ip[5];
  double dt =        rp[39];
  double dr =        rp[38];
  double ds =        rp[37];
  double vis =       rp[36];
  double t =         rp[35];
  double acm =       rp[49];
  
  /* results of flux calculations*/
  double FN[5], FS[5], FE[5], FW[5]; /* north, south, east, west */
  
  double u0[5], ul[5], ur[5], ut[5], ub[5];

  double f_fs[5], g_fs[5], v1, v2, lam,  p;
  double yrs, ysr, xrs, xsr;

  /* Slope corrected values by rows ... minus row, zeroth row and plus row
   * Here we realize that each row is (NR+4)*5 elements long
   */
  double *u0n, *u0s, *u0e, *u0w;
  double *umn, *ums, *ume, *umw;
  double *upn, *ups, *upe, *upw;
  double *divm, *div0, *divp, *tempPoint;
  
  /* We need one vector of length (NR+4)*5 for the storage of the bottom
   * fluxes once we step up one row so they need not be calculated again */
  double *F_bot;

  double cr, cs, J, JM;
  double J1, J2, J3, J4;
  double a1, b1, a2, b2, a3, b3, a4, b4;
  double speed[2], rad;
  double dp, rhot, v1t, v2t, Et, Yt;
  double vrm, vrp, vsm, vsp,  nu, max_vis = 0.0;
  double tflux = 0.0, tslope = 0.0, tsource = 0.0, t0=0, t1=0;
  double xi, yi, ti;
  double h[5];
  double gu=0.0, gv=0.0, gub=0.0, gut=0.0, gul=0.0, gur=0.0;
  double gvb=0.0, gvt=0.0, gvl=0.0, gvr=0.0;
  double mxr, mxs, myr, mys, temp, moveRHS[5];

  int i, j, k, i2;
  int n1, n2;

  n1 = NR+4;
  n2 = NS+4;

  u0n = &workspace[0];
  u0s = &workspace[n1*5];
  u0e = &workspace[2*n1*5];
  u0w = &workspace[3*n1*5];
  umn = &workspace[4*n1*5];
  ums = &workspace[5*n1*5];
  ume = &workspace[6*n1*5];
  umw = &workspace[7*n1*5];
  upn = &workspace[8*n1*5];
  ups = &workspace[9*n1*5];
  upe = &workspace[10*n1*5];
  upw = &workspace[11*n1*5];
  F_bot = &workspace[12*n1*5];
  divm = &workspace[13*n1*5];
  div0 = &workspace[13*n1*5+n1];
  divp = &workspace[13*n1*5+2*n1];
  
  cr = dt/dr;
  cs = dt/ds;

  speed[0] = 0.0;
  speed[1] = 0.0;

  /* Find slope corrections for the first row of ghost cells
   * and first row of interior cells. Also use loop to find the
   * fluxes along the "southern" (j = 1.5) boundary.
   */
  for (i = 1; i < n1-1; i++)
    {
      second(&t0);
      j = 1;
      /* first row of ghost cells */
      for (k = 0; k < 5; k++)
	{
	  u0[k] = u[ind3(i,j,k,n1,n2,5)];
	  ul[k] = u[ind3(i-1,j,k,n1,n2,5)];
	  ur[k] = u[ind3(i+1,j,k,n1,n2,5)];
	  ub[k] = u[ind3(i,j-1,k,n1,n2,5)];
	  ut[k] = u[ind3(i,j+1,k,n1,n2,5)];
	}
      if (move == 0)
	{
	  a1 = rx[ind3(i,j,0,n1,n2,4)]; /* y_s = J*r_x */
	  b1 = rx[ind3(i,j,2,n1,n2,4)]; /* -x_s = J*r_y */
	  a2 = rx[ind3(i,j,1,n1,n2,4)]; /* -y_r = J*s_x */
	  b2 = rx[ind3(i,j,3,n1,n2,4)]; /* x_r = J*s_y */
	  J = det[ind2(i,j,n1,n2)];
	}
      else
	{
	  /* moving grid metrics go here */
	  a1 = 0.5*(rx[ind3(i,j,0,n1,n2,4)]+rx2[ind3(i,j,0,n1,n2,4)]);
	  b1 = 0.5*(rx[ind3(i,j,2,n1,n2,4)]+rx2[ind3(i,j,2,n1,n2,4)]);
	  a2 = 0.5*(rx[ind3(i,j,1,n1,n2,4)]+rx2[ind3(i,j,1,n1,n2,4)]);
	  b2 = 0.5*(rx[ind3(i,j,3,n1,n2,4)]+rx2[ind3(i,j,3,n1,n2,4)]);
	  J = 0.5*(det[ind2(i,j,n1,n2)]+det2[ind2(i,j,n1,n2)]);
	  gu = gvel[ind3(i,j,0,n1,n2,2)];
	  gv = gvel[ind3(i,j,1,n1,n2,2)];
	}
      
      /* if we are in twilight zone fill h vector */
      if (twilightZone == 1 && order == 2)
	{
	  twilightSource(OGFunc, rp, mg, vert[ind3(i,j,0,n1,n2,2)],
			 vert[ind3(i,j,1,n1,n2,2)], t, h);
	}
      slope(u0, ut, ub, ur, ul,
	    &umn[5*i], &ums[5*i], &ume[5*i], &umw[5*i], 
	    cr, cs, a1, b1, a2, b2, J, order, 
	    slopeCorr, dt, rp, twilightZone, h,
	    move, gu, gv, acm);

      /* first row of interior cells */
      j = 2;
      for (k = 0; k < 5; k++)
	{
	  u0[k] = u[ind3(i,j,k,n1,n2,5)];
	  ul[k] = u[ind3(i-1,j,k,n1,n2,5)];
	  ur[k] = u[ind3(i+1,j,k,n1,n2,5)];
	  ub[k] = u[ind3(i,j-1,k,n1,n2,5)];
	  ut[k] = u[ind3(i,j+1,k,n1,n2,5)];
	}
      if (move == 0)
	{
	  a1 = rx[ind3(i,j,0,n1,n2,4)]; /* y_s = J*r_x */
	  b1 = rx[ind3(i,j,2,n1,n2,4)]; /* -x_s = J*r_y */
	  a2 = rx[ind3(i,j,1,n1,n2,4)]; /* -y_r = J*s_x */
	  b2 = rx[ind3(i,j,3,n1,n2,4)]; /* x_r = J*s_y */
	  J = det[ind2(i,j,n1,n2)];
	}
      else
	{
	  /* moving grid metrics here */
	  a1 = 0.5*(rx[ind3(i,j,0,n1,n2,4)]+rx2[ind3(i,j,0,n1,n2,4)]);
	  b1 = 0.5*(rx[ind3(i,j,2,n1,n2,4)]+rx2[ind3(i,j,2,n1,n2,4)]);
	  a2 = 0.5*(rx[ind3(i,j,1,n1,n2,4)]+rx2[ind3(i,j,1,n1,n2,4)]);
	  b2 = 0.5*(rx[ind3(i,j,3,n1,n2,4)]+rx2[ind3(i,j,3,n1,n2,4)]);
	  J = 0.5*(det[ind2(i,j,n1,n2)]+det2[ind2(i,j,n1,n2)]);
	  gu = gvel[ind3(i,j,0,n1,n2,2)];
	  gv = gvel[ind3(i,j,1,n1,n2,2)];
	}

      /* if we are in twilight zone fill h vector */
      if (twilightZone == 1 && order == 2)
	{
	  twilightSource(OGFunc, rp, mg, vert[ind3(i,j,0,n1,n2,2)],
			 vert[ind3(i,j,1,n1,n2,2)], t, h);
	}
      slope(u0, ut, ub, ur, ul,
	    &u0n[5*i], &u0s[5*i], &u0e[5*i], &u0w[5*i], 
	    cr, cs, a1, b1, a2, b2, J, order, slopeCorr, 
	    dt, rp, twilightZone, h,
	    move, gu, gv, acm);
      second(&t1);
      tslope += t1-t0;

      /* Find first row of fluxes */
      second(&t0);
      j = 2;
      if (move == 0)
	{
	  J1 = (det[ind2(i,j,n1,n2)]+det[ind2(i,j-1,n1,n2)])/2.0;
	  a1 = (rx[ind3(i,j,1,n1,n2,4)]+rx[ind3(i,j-1,1,n1,n2,4)])/2.0; /* -y_r = J*s_x */
	  b1 = (rx[ind3(i,j,3,n1,n2,4)]+rx[ind3(i,j-1,3,n1,n2,4)])/2.0; /* x_r = J*s_y */
	}
      else
	{
	  /* moving grid metrics here */
	  J1 = (0.5*(det[ind2(i,j,n1,n2)]+det2[ind2(i,j,n1,n2)])
		+0.5*(det[ind2(i,j-1,n1,n2)]+det2[ind2(i,j-1,n1,n2)]))/2.0;
	  a1 = (0.5*(rx[ind3(i,j,1,n1,n2,4)]+rx2[ind3(i,j,1,n1,n2,4)])
		+0.5*(rx[ind3(i,j-1,1,n1,n2,4)]+rx2[ind3(i,j-1,1,n1,n2,4)]))/2.0;
	  b1 = (0.5*(rx[ind3(i,j,3,n1,n2,4)]+rx2[ind3(i,j,3,n1,n2,4)])
		+0.5*(rx[ind3(i,j-1,3,n1,n2,4)]+rx2[ind3(i,j-1,3,n1,n2,4)]))/2.0;
	  gub = 0.25*(gvel[ind3(i,j,0,n1,n2,2)]+gvel2[ind3(i,j,0,n1,n2,2)]
		     +gvel[ind3(i,j-1,0,n1,n2,2)]+gvel2[ind3(i,j-1,0,n1,n2,2)]);
	  gvb = 0.25*(gvel[ind3(i,j,1,n1,n2,2)]+gvel2[ind3(i,j,1,n1,n2,2)]
		     +gvel[ind3(i,j-1,1,n1,n2,2)]+gvel2[ind3(i,j-1,1,n1,n2,2)]);
	}
      if (meth == 1)
	{
	  roe (&umn[5*i], &u0s[5*i], &F_bot[5*i], &speed[1], a1, b1, J1, rp, gub, gvb);
	}
      else if (meth == 3)
	{
	  HLL (&umn[5*i], &u0s[5*i], &F_bot[5*i], &speed[1], a1, b1, J1, rp, gub, gvb);
	}
      else
	{
	  printf ("Method not known ... aborting\n");
	  exit (0);
	}
      second(&t1);
      tflux += t1-t0;

      /* set the divergence of the first line of ghost cells */
      j = 1;
      vrm = (rx[ind3(i,j,0,n1,n2,4)]*u[ind3(i-1,j,1,n1,n2,5)]+
	     rx[ind3(i,j,2,n1,n2,4)]*u[ind3(i-1,j,2,n1,n2,5)])/u[ind3(i-1,j,0,n1,n2,5)];
      vrp = (rx[ind3(i,j,0,n1,n2,4)]*u[ind3(i+1,j,1,n1,n2,5)]+
	     rx[ind3(i,j,2,n1,n2,4)]*u[ind3(i+1,j,2,n1,n2,5)])/u[ind3(i+1,j,0,n1,n2,5)];
      vsm = (rx[ind3(i,j,1,n1,n2,4)]*u[ind3(i,j-1,1,n1,n2,5)]+
	     rx[ind3(i,j,3,n1,n2,4)]*u[ind3(i,j-1,2,n1,n2,5)])/u[ind3(i,j-1,0,n1,n2,5)];
      vsp = (rx[ind3(i,j,1,n1,n2,4)]*u[ind3(i,j+1,1,n1,n2,5)]+
	     rx[ind3(i,j,3,n1,n2,4)]*u[ind3(i,j+1,2,n1,n2,5)])/u[ind3(i,j+1,0,n1,n2,5)];
      divm[i] = (vrp-vrm)/(2.0*dr)+(vsp-vsm)/(2.0*ds);
      max_vis = max(max_vis, -divm[i]);
      
      /* set divergence of first line of interior cells */
      j = 2;
      vrm = (rx[ind3(i,j,0,n1,n2,4)]*u[ind3(i-1,j,1,n1,n2,5)]+
	     rx[ind3(i,j,2,n1,n2,4)]*u[ind3(i-1,j,2,n1,n2,5)])/u[ind3(i-1,j,0,n1,n2,5)];
      vrp = (rx[ind3(i,j,0,n1,n2,4)]*u[ind3(i+1,j,1,n1,n2,5)]+
	     rx[ind3(i,j,2,n1,n2,4)]*u[ind3(i+1,j,2,n1,n2,5)])/u[ind3(i+1,j,0,n1,n2,5)];
      vsm = (rx[ind3(i,j,1,n1,n2,4)]*u[ind3(i,j-1,1,n1,n2,5)]+
	     rx[ind3(i,j,3,n1,n2,4)]*u[ind3(i,j-1,2,n1,n2,5)])/u[ind3(i,j-1,0,n1,n2,5)];
      vsp = (rx[ind3(i,j,1,n1,n2,4)]*u[ind3(i,j+1,1,n1,n2,5)]+
	     rx[ind3(i,j,3,n1,n2,4)]*u[ind3(i,j+1,2,n1,n2,5)])/u[ind3(i,j+1,0,n1,n2,5)];
      div0[i] = (vrp-vrm)/(2.0*dr)+(vsp-vsm)/(2.0*ds);
      max_vis = max(max_vis, -div0[i]);
    }

  /* now we loop over the rows of the grid to update solution */
  for (j = 2; j < n2-2; j++)
    {
      /* get the "plus" row of slope corrections */
      for (i = 1; i < n1-1; i++)
	{
	  second(&t0);
	  for (k = 0; k < 5; k++)
	    {
	      u0[k] = u[ind3(i,j+1,k,n1,n2,5)];
	      ul[k] = u[ind3(i-1,j+1,k,n1,n2,5)];
	      ur[k] = u[ind3(i+1,j+1,k,n1,n2,5)];
	      ub[k] = u[ind3(i,j,k,n1,n2,5)];
	      ut[k] = u[ind3(i,j+2,k,n1,n2,5)];
	    }
	  if (move == 0)
	    {
	      a1 = rx[ind3(i,j+1,0,n1,n2,4)]; /* y_s = J*r_x */
	      b1 = rx[ind3(i,j+1,2,n1,n2,4)]; /* -x_s = J*r_y */
	      a2 = rx[ind3(i,j+1,1,n1,n2,4)]; /* -y_r = J*s_x */
	      b2 = rx[ind3(i,j+1,3,n1,n2,4)]; /* x_r = J*s_y */
	      J = det[ind2(i,j+1,n1,n2)];
	    }
	  else
	    {
	      /* moving grid metrics here */
	      a1 = 0.5*(rx[ind3(i,j+1,0,n1,n2,4)]+rx2[ind3(i,j+1,0,n1,n2,4)]);
	      b1 = 0.5*(rx[ind3(i,j+1,2,n1,n2,4)]+rx2[ind3(i,j+1,2,n1,n2,4)]);
	      a2 = 0.5*(rx[ind3(i,j+1,1,n1,n2,4)]+rx2[ind3(i,j+1,1,n1,n2,4)]);
	      b2 = 0.5*(rx[ind3(i,j+1,3,n1,n2,4)]+rx2[ind3(i,j+1,3,n1,n2,4)]);
	      J = 0.5*(det[ind2(i,j+1,n1,n2)]+det2[ind2(i,j+1,n1,n2)]);
	      gu = gvel[ind3(i,j+1,0,n1,n2,2)];
	      gv = gvel[ind3(i,j+1,1,n1,n2,2)];
	    }
	  
	  /* if we are in twilight zone fill h vector */
	  if (twilightZone == 1 && order == 2)
	    {
	      twilightSource(OGFunc, rp, mg, vert[ind3(i,j+1,0,n1,n2,2)],
			     vert[ind3(i,j+1,1,n1,n2,2)], t, h);
	    }
	  slope(u0, ut, ub, ur, ul,
		&upn[5*i], &ups[5*i], &upe[5*i], &upw[5*i], 
		cr, cs, a1, b1, a2, b2, J, order, slopeCorr, 
		dt, rp, twilightZone, h, move, gu, gv, acm);
	  second(&t1);
	  tslope += t1-t0;

	  /* calculate divergence of the next row up */ 
	  vrm = (rx[ind3(i,j+1,0,n1,n2,4)]*u[ind3(i-1,j+1,1,n1,n2,5)]+
		 rx[ind3(i,j+1,2,n1,n2,4)]*u[ind3(i-1,j+1,2,n1,n2,5)])/u[ind3(i-1,j+1,0,n1,n2,5)];
	  vrp = (rx[ind3(i,j+1,0,n1,n2,4)]*u[ind3(i+1,j+1,1,n1,n2,5)]+
		 rx[ind3(i,j+1,2,n1,n2,4)]*u[ind3(i+1,j+1,2,n1,n2,5)])/u[ind3(i+1,j+1,0,n1,n2,5)];
	  vsm = (rx[ind3(i,j+1,1,n1,n2,4)]*u[ind3(i,j,1,n1,n2,5)]+
		 rx[ind3(i,j+1,3,n1,n2,4)]*u[ind3(i,j,2,n1,n2,5)])/u[ind3(i,j,0,n1,n2,5)];
	  vsp = (rx[ind3(i,j+1,1,n1,n2,4)]*u[ind3(i,j+2,1,n1,n2,5)]+
		 rx[ind3(i,j+1,3,n1,n2,4)]*u[ind3(i,j+2,2,n1,n2,5)])/u[ind3(i,j+2,0,n1,n2,5)];
	  divp[i] = (vrp-vrm)/(2.0*dr)+(vsp-vsm)/(2.0*ds);
	  max_vis = max(max_vis, -divp[i]);
	}
      /* get the westmost flux */
      second(&t0);
      if (move == 0)
	{
	  J4 = (det[ind2(2,j,n1,n2)]+det[ind2(1,j,n1,n2)])/2.0;
	  a4 = (rx[ind3(2,j,0,n1,n2,4)]+rx[ind3(1,j,0,n1,n2,4)])/2.0; /* y_s = J*r_x */
	  b4 = (rx[ind3(2,j,2,n1,n2,4)]+rx[ind3(1,j,2,n1,n2,4)])/2.0; /* -x_s = J*r_y */
	}
      else
	{
	  /* moving grid metrics here */
	  J4 = (0.5*(det[ind2(2,j,n1,n2)]+det2[ind2(2,j,n1,n2)])
		+0.5*(det[ind2(1,j,n1,n2)]+det2[ind2(1,j,n1,n2)]))/2.0;
	  a4 = (0.5*(rx[ind3(2,j,0,n1,n2,4)]+rx2[ind3(2,j,0,n1,n2,4)])
		+0.5*(rx[ind3(1,j,0,n1,n2,4)]+rx2[ind3(1,j,0,n1,n2,4)]))/2.0;
	  b4 = (0.5*(rx[ind3(2,j,2,n1,n2,4)]+rx2[ind3(2,j,2,n1,n2,4)])
		+0.5*(rx[ind3(1,j,2,n1,n2,4)]+rx2[ind3(1,j,2,n1,n2,4)]))/2.0;
	  gul = 0.25*(gvel[ind3(2,j,0,n1,n2,2)]+gvel2[ind3(2,j,0,n1,n2,2)]
		     +gvel[ind3(1,j,0,n1,n2,2)]+gvel2[ind3(1,j,0,n1,n2,2)]);
	  gvl = 0.25*(gvel[ind3(2,j,1,n1,n2,2)]+gvel2[ind3(2,j,1,n1,n2,2)]
		     +gvel[ind3(1,j,1,n1,n2,2)]+gvel2[ind3(1,j,1,n1,n2,2)]);
	}
      if (meth == 1)
	{
	  roe (&u0e[5*1], &u0w[5*2], FW, &speed[0], a4, b4, J4, rp, gul, gvl);
	}
      else if (meth == 3)
	{
	  HLL (&u0e[5*1], &u0w[5*2], FW, &speed[0], a4, b4, J4, rp, gul, gvl);
	}
      else
	{
	  printf ("Method not known ... aborting\n");
	  exit (0);
	}
      second(&t1);
      tflux += t1-t0;
      
      /* now loop over columns to get fluxes and update grid */
      for (i = 2; i < n1-2; i++)
	{
	  second(&t0);
	  if (move == 0)
	    {
	      /* get east flux */
	      J3 = (det[ind2(i+1,j,n1,n2)]+det[ind2(i,j,n1,n2)])/2.0;
	      a3 = (rx[ind3(i+1,j,0,n1,n2,4)]+rx[ind3(i,j,0,n1,n2,4)])/2.0; /* y_s = J*r_x */
	      b3 = (rx[ind3(i+1,j,2,n1,n2,4)]+rx[ind3(i,j,2,n1,n2,4)])/2.0; /* -x_s = J*r_y */
	      
	      /* get north flux */
	      J1 = (det[ind2(i,j+1,n1,n2)]+det[ind2(i,j,n1,n2)])/2.0;
	      a1 = (rx[ind3(i,j+1,1,n1,n2,4)]+rx[ind3(i,j,1,n1,n2,4)])/2.0; /* -y_r = J*s_x */
	      b1 = (rx[ind3(i,j+1,3,n1,n2,4)]+rx[ind3(i,j,3,n1,n2,4)])/2.0; /* x_r = J*s_y */
	    }
	  else
	    {
	      /* moving grid metrics here */
	      J3 = (0.5*(det[ind2(i+1,j,n1,n2)]+det2[ind2(i+1,j,n1,n2)])
		    +0.5*(det[ind2(i,j,n1,n2)]+det2[ind2(i,j,n1,n2)]))/2.0;
	      a3 = (0.5*(rx[ind3(i+1,j,0,n1,n2,4)]+rx2[ind3(i+1,j,0,n1,n2,4)])
		    +0.5*(rx[ind3(i,j,0,n1,n2,4)]+rx2[ind3(i,j,0,n1,n2,4)]))/2.0;
	      b3 = (0.5*(rx[ind3(i+1,j,2,n1,n2,4)]+rx2[ind3(i+1,j,2,n1,n2,4)])
		    +0.5*(rx[ind3(i,j,2,n1,n2,4)]+rx2[ind3(i,j,2,n1,n2,4)]))/2.0;
	      gur = 0.25*(gvel[ind3(i+1,j,0,n1,n2,2)]+gvel2[ind3(i+1,j,0,n1,n2,2)]
			 +gvel[ind3(i,j,0,n1,n2,2)]+gvel2[ind3(i,j,0,n1,n2,2)]);
	      gvr = 0.25*(gvel[ind3(i+1,j,1,n1,n2,2)]+gvel2[ind3(i+1,j,1,n1,n2,2)]
			 +gvel[ind3(i,j,1,n1,n2,2)]+gvel2[ind3(i,j,1,n1,n2,2)]);

	      J1 = (0.5*(det[ind2(i,j+1,n1,n2)]+det2[ind2(i,j+1,n1,n2)])
		    +0.5*(det[ind2(i,j,n1,n2)]+det2[ind2(i,j,n1,n2)]))/2.0;
	      a1 = (0.5*(rx[ind3(i,j+1,1,n1,n2,4)]+rx2[ind3(i,j+1,1,n1,n2,4)])
		    +0.5*(rx[ind3(i,j,1,n1,n2,4)]+rx2[ind3(i,j,1,n1,n2,4)]))/2.0;
	      b1 = (0.5*(rx[ind3(i,j+1,3,n1,n2,4)]+rx2[ind3(i,j+1,3,n1,n2,4)])
		    +0.5*(rx[ind3(i,j,3,n1,n2,4)]+rx2[ind3(i,j,3,n1,n2,4)]))/2.0;
	      gut = 0.25*(gvel[ind3(i,j+1,0,n1,n2,2)]+gvel2[ind3(i,j+1,0,n1,n2,2)]
			 +gvel[ind3(i,j,0,n1,n2,2)]+gvel2[ind3(i,j,0,n1,n2,2)]);
	      gvt = 0.25*(gvel[ind3(i,j+1,1,n1,n2,2)]+gvel2[ind3(i,j+1,1,n1,n2,2)]
			 +gvel[ind3(i,j,1,n1,n2,2)]+gvel2[ind3(i,j,1,n1,n2,2)]);
	    }

	  if (meth == 1)
	    {
	      roe (&u0e[5*i], &u0w[5*(i+1)], FE, &speed[0], a3, b3, J3, rp, gur, gvr);
	      roe (&u0n[5*i], &ups[5*i], FN, &speed[1], a1, b1, J1, rp, gut, gvt);
	    }
	  else if (meth == 3)
	    {
	      HLL (&u0e[5*i], &u0w[5*(i+1)], FE, &speed[0], a3, b3, J3, rp, gur, gvr);
	      HLL (&u0n[5*i], &ups[5*i], FN, &speed[1], a1, b1, J1, rp, gut, gvt);
	    }
	  else
	    {
	      printf ("Method not known ... aborting\n");
	      exit (0);
	    }
	  second(&t1);
	  tflux += t1-t0;

	  for (k = 0; k < 5; k++)
	    {
	      u0[k] = u[ind3(i,j,k,n1,n2,5)];
	    }
	  /* Get fluxes for free stream correction. */
	  v1 = u[ind3(i,j,1,n1,n2,5)]/u[ind3(i,j,0,n1,n2,5)];
	  v2 = u[ind3(i,j,2,n1,n2,5)]/u[ind3(i,j,0,n1,n2,5)];
	  lam = u[ind3(i,j,4,n1,n2,5)]/u[ind3(i,j,0,n1,n2,5)];
	  p = (gamma(lam)-1.0)
	    *(u[ind3(i,j,3,n1,n2,5)]-0.5*u[ind3(i,j,0,n1,n2,5)]*(v1*v1+v2*v2))
	    -gamma(lam)*pi(lam);
	  if (move != 0)
	    {
	      gu = 0.5*(gvel[ind3(i,j,0,n1,n2,2)]+gvel2[ind3(i,j,0,n1,n2,2)]);
	      gv = 0.5*(gvel[ind3(i,j,1,n1,n2,2)]+gvel2[ind3(i,j,1,n1,n2,2)]);
	    }
	  flux(f_fs, u0, p, 1.0, 0.0, 1.0, gu, gv);
	  flux(g_fs, u0, p, 0.0, 1.0, 1.0, gu, gv);
	  
	  /* Get approximations to derivatives of 
	   * mappings for free stream correction. */
	  if (move == 0)
	    {
	      J = det[ind2(i+1,j,n1,n2)];
	      JM = det[ind2(i-1,j,n1,n2)];
	      ysr = (J*rx[ind3(i+1,j,0,n1,n2,4)]-JM*rx[ind3(i-1,j,0,n1,n2,4)])/(2.0*dr);
	      xsr = (-J*rx[ind3(i+1,j,2,n1,n2,4)]+JM*rx[ind3(i-1,j,2,n1,n2,4)])/(2.0*dr); 
	      
	      J = det[ind2(i,j+1,n1,n2)];
	      JM = det[ind2(i,j-1,n1,n2)];
	      yrs = (-J*rx[ind3(i,j+1,1,n1,n2,4)]+JM*rx[ind3(i,j-1,1,n1,n2,4)])/(2.0*ds);
	      xrs = (J*rx[ind3(i,j+1,3,n1,n2,4)]-JM*rx[ind3(i,j-1,3,n1,n2,4)])/(2.0*ds);
	    }
	  else
	    {
	      /* moving grid free stream metrics */
	      J = 0.5*(det[ind2(i+1,j,n1,n2)]+det2[ind2(i+1,j,n1,n2)]);
	      JM = 0.5*(det[ind2(i-1,j,n1,n2)]+det[ind2(i-1,j,n1,n2)]);
	      ysr = (J*0.5*(rx[ind3(i+1,j,0,n1,n2,4)]+rx2[ind3(i+1,j,0,n1,n2,4)])
		     -JM*0.5*(rx[ind3(i-1,j,0,n1,n2,4)]+rx2[ind3(i-1,j,0,n1,n2,4)]))/(2.0*dr);
	      xsr = (-J*0.5*(rx[ind3(i+1,j,2,n1,n2,4)]+rx2[ind3(i+1,j,2,n1,n2,4)])
		     +JM*0.5*(rx[ind3(i-1,j,2,n1,n2,4)]+rx2[ind3(i-1,j,2,n1,n2,4)]))/(2.0*dr); 
	      
	      J = 0.5*(det[ind2(i,j+1,n1,n2)]+det2[ind2(i,j+1,n1,n2)]);
	      JM = 0.5*(det[ind2(i,j-1,n1,n2)]+det2[ind2(i,j-1,n1,n2)]);
	      yrs = (-J*0.5*(rx[ind3(i,j+1,1,n1,n2,4)]+rx2[ind3(i,j+1,1,n1,n2,4)])
		     +JM*0.5*(rx[ind3(i,j-1,1,n1,n2,4)]+rx2[ind3(i,j-1,1,n1,n2,4)]))/(2.0*ds);
	      xrs = (J*0.5*(rx[ind3(i,j+1,3,n1,n2,4)]+rx2[ind3(i,j+1,3,n1,n2,4)])
		     -JM*0.5*(rx[ind3(i,j-1,3,n1,n2,4)]+rx2[ind3(i,j-1,3,n1,n2,4)]))/(2.0*ds);
	    }

	  /* calculate updates and second free stream correction for moving grids */
	  if (move == 0)
	    {
	      J = det[ind2(i,j,n1,n2)];
	      temp = 0.0;
	    }
	  else
	    {
	      J = 0.5*(det[ind2(i,j,n1,n2)]+det2[ind2(i,j,n1,n2)]);
	      mxr = (0.5*(gvel[ind3(i+1,j,0,n1,n2,2)]+gvel2[ind3(i+1,j,0,n1,n2,2)])
		     -0.5*(gvel[ind3(i-1,j,0,n1,n2,2)]+gvel2[ind3(i-1,j,0,n1,n2,2)]))/(2.0*dr);
	      myr = (0.5*(gvel[ind3(i+1,j,1,n1,n2,2)]+gvel2[ind3(i+1,j,1,n1,n2,2)])
		     -0.5*(gvel[ind3(i-1,j,1,n1,n2,2)]+gvel2[ind3(i-1,j,1,n1,n2,2)]))/(2.0*dr);
	      mxs = (0.5*(gvel[ind3(i,j+1,0,n1,n2,2)]+gvel2[ind3(i,j+1,0,n1,n2,2)])
		     -0.5*(gvel[ind3(i,j-1,0,n1,n2,2)]+gvel2[ind3(i,j-1,0,n1,n2,2)]))/(2.0*ds);
	      mys = (0.5*(gvel[ind3(i,j+1,1,n1,n2,2)]+gvel2[ind3(i,j+1,1,n1,n2,2)])
		     -0.5*(gvel[ind3(i,j-1,1,n1,n2,2)]+gvel2[ind3(i,j-1,1,n1,n2,2)]))/(2.0*ds);
	      temp = -(0.5*(rx[ind3(i,j,0,n1,n2,4)]+rx2[ind3(i,j,0,n1,n2,4)])*mxr
		       +0.5*(rx[ind3(i,j,2,n1,n2,4)]+rx2[ind3(i,j,2,n1,n2,4)])*myr
		       +0.5*(rx[ind3(i,j,1,n1,n2,4)]+rx2[ind3(i,j,1,n1,n2,4)])*mxs
		       +0.5*(rx[ind3(i,j,3,n1,n2,4)]+rx2[ind3(i,j,3,n1,n2,4)])*mys);
	    }
	  for (k = 0; k < 5; k++)
	    {
	      i2 = 5*i+k;
	      /* update fluxes */
	      du[ind3(i,j,k,n1,n2,5)] = (FW[k]-FE[k])/dr+(F_bot[i2]-FN[k])/ds;

	      /* Add on Free stream correction. */
	      du[ind3(i,j,k,n1,n2,5)] += -((yrs-ysr)*f_fs[k]+(xsr-xrs)*g_fs[k]);

	      du[ind3(i,j,k,n1,n2,5)] = du[ind3(i,j,k,n1,n2,5)]/J;

	      /* Add on artificial viscosity */
	      /* west interface, then east, then north, then south */
	      nu = vis*max(0.0, -(div0[i-1]+div0[i])/2.0);
	      du[ind3(i,j,k,n1,n2,5)] += -nu*(u[ind3(i,j,k,n1,n2,5)]-u[ind3(i-1,j,k,n1,n2,5)]);
		
	      nu = vis*max(0.0, -(div0[i]+div0[i+1])/2.0);
	      du[ind3(i,j,k,n1,n2,5)] += nu*(u[ind3(i+1,j,k,n1,n2,5)]-u[ind3(i,j,k,n1,n2,5)]);
	      
	      nu = vis*max(0.0, -(div0[i]+divp[i])/2.0);
	      du[ind3(i,j,k,n1,n2,5)] += nu*(u[ind3(i,j+1,k,n1,n2,5)]-u[ind3(i,j,k,n1,n2,5)]);
	      
	      nu = vis*max(0.0, -(divm[i]+div0[i])/2.0);
	      du[ind3(i,j,k,n1,n2,5)] += -nu*(u[ind3(i,j,k,n1,n2,5)]-u[ind3(i,j-1,k,n1,n2,5)]);

	      /* add on movong grids free stream term */
	      du[ind3(i,j,k,n1,n2,5)] += u[ind3(i,j,k,n1,n2,5)]*temp;

	      F_bot[i2] = FN[k];
	      FW[k] = FE[k];
	    }
	  
	  /* add pressure fix onto solution */
	  if (fix == 1)
	    {
	      second(&t0);
	      for (k = 0; k < 5; k++)
		{
		  u0[k] = u[ind3(i,j,k,n1,n2,5)];
		}
	      if (move == 0)
		{
		  J2 = (det[ind2(i,j,n1,n2)]+det[ind2(i,j-1,n1,n2)])/2.0;
		  a2 = (rx[ind3(i,j,1,n1,n2,4)]+rx[ind3(i,j-1,1,n1,n2,4)])/2.0; /* -y_r = J*s_x */
		  b2 = (rx[ind3(i,j,3,n1,n2,4)]+rx[ind3(i,j-1,3,n1,n2,4)])/2.0; /* x_r = J*s_y */
		  J = det[ind2(i,j,n1,n2)];
		}
	      else
		{
		  J2 = (0.5*(det[ind2(i,j,n1,n2)]+det2[ind2(i,j,n1,n2)])
			+0.5*(det[ind2(i,j-1,n1,n2)]+det2[ind2(i,j-1,n1,n2)]))/2.0;
		  a2 = (0.5*(rx[ind3(i,j,1,n1,n2,4)]+rx2[ind3(i,j,1,n1,n2,4)])
			+0.5*(rx[ind3(i,j-1,1,n1,n2,4)]+rx2[ind3(i,j-1,1,n1,n2,4)]))/2.0;
		  b2 = (0.5*(rx[ind3(i,j,3,n1,n2,4)]+rx2[ind3(i,j,3,n1,n2,4)])
			+0.5*(rx[ind3(i,j-1,3,n1,n2,4)]+rx2[ind3(i,j-1,3,n1,n2,4)]))/2.0;
		  gub = 0.25*(gvel[ind3(i,j,0,n1,n2,2)]+gvel2[ind3(i,j,0,n1,n2,2)]
			      +gvel[ind3(i,j-1,0,n1,n2,2)]+gvel2[ind3(i,j-1,0,n1,n2,2)]);
		  gvb = 0.25*(gvel[ind3(i,j,1,n1,n2,2)]+gvel2[ind3(i,j,1,n1,n2,2)]
			      +gvel[ind3(i,j-1,1,n1,n2,2)]+gvel2[ind3(i,j-1,1,n1,n2,2)]);
		  J = 0.5*(det[ind2(i,j,n1,n2)]+det2[ind2(i,j,n1,n2)]);
		  gu = 0.5*(gvel[ind3(i,j,0,n1,n2,2)]+gvel2[ind3(i,j,0,n1,n2,2)]);
		  gv = 0.5*(gvel[ind3(i,j,1,n1,n2,2)]+gvel2[ind3(i,j,1,n1,n2,2)]);
		}
	      if (meth == 1)
		{
		  dp = roe_fix (u0,
				&u0n[5*i], &ups[5*i], /* north */
				&umn[5*i], &u0s[5*i], /* south */
				&u0e[5*i], &u0w[5*(i+1)], /* east */
				&u0e[5*(i-1)], &u0w[5*i], /* west */
				cr, cs,
				a1, b1, J1, gut, gvt, /* north */
				a2, b2, J2, gub, gvb, /* south */
				a3, b3, J3, gur, gvr, /* east */
				a4, b4, J4, gul, gvl, /* west */
				J, gu, gv, temp, dt, rp);
		}
	      else if (meth == 3)
		{
		  dp = HLL_fix (u0,
				&u0n[5*i], &ups[5*i], /* north */
				&umn[5*i], &u0s[5*i], /* south */
				&u0e[5*i], &u0w[5*(i+1)], /* east */
				&u0e[5*(i-1)], &u0w[5*i], /* west */
				cr, cs,
				a1, b1, J1, gut, gvt, /* north */
				a2, b2, J2, gub, gvb, /* south */
				a3, b3, J3, gur, gvr, /* east */
				a4, b4, J4, gul, gvl, /* west */
				J, gu, gv, temp, dt, rp);
		}
	      else
		{
		  printf ("Method not known ... aborting\n");
		  exit (0);
		}
	      rhot = u[ind3(i,j,0,n1,n2,5)]+dt*du[ind3(i,j,0,n1,n2,5)];
	      Yt = u[ind3(i,j,4,n1,n2,5)]+dt*du[ind3(i,j,4,n1,n2,5)];
	      du[ind3(i,j,3,n1,n2,5)] += dp/(dt*(gamma(Yt/rhot)-1.0));

	      second(&t1);
	      tsource += t1-t0;
	    }
	  J4 = J3;
	  a4 = a3;
	  b4 = b3;
	  gul = gur;
	  gvl = gvr;
	}

      /* perform some pointer arithmatic to update workspace pointers. */
      tempPoint = divm;
      divm = div0;
      div0 = divp;
      divp = tempPoint;

      tempPoint = umn;
      umn = u0n;
      u0n = upn;
      upn = tempPoint;

      tempPoint = ums;
      ums = u0s;
      u0s = ups;
      ups = tempPoint;

      tempPoint = ume;
      ume = u0e;
      u0e = upe;
      upe = tempPoint;

      tempPoint = umw;
      umw = u0w;
      u0w = upw;
      upw = tempPoint;
    }
  if (twilightZone == 1)
    {
      for (j=2; j<n2-2; j++)
	{
	  for (i=2; i<n1-2; i++)
	    {
	      twilightSource(OGFunc, rp, mg, vert[ind3(i,j,0,n1,n2,2)],
			     vert[ind3(i,j,1,n1,n2,2)], t+0.5*dt, h);
	      for (k=0; k<5; k++)
		{
		  du[ind3(i,j,k,n1,n2,5)] += h[k];
		}
	    }
	}
    }

  /* set returned max eigenvalues */
  rp[0] = 4.0*vis*max_vis;
  rp[1] = (speed[0]/dr+speed[1]/ds);

  rp[30] = tflux;
  rp[31] = tslope;
  rp[32] = tsource;
  
  return 0;
}

#endif
