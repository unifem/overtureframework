#ifndef __CORRECTION_H__
#define __CORRECTION_H__

void minmod(double *alpha, double *alpha1, double *alpha2);

void get_prim (double *u, double *prim, double *rp);

void get_con (double *u, double *prim, double *rp);

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
	    int twilightZone, double h[5],
	    int move, double gu, double gv, double acm);

#endif
