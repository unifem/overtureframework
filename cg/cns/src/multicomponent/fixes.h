#ifndef __FIXES_H__
#define __FIXES_H__

double roe_fix (double *u, 
		double *mt, double *tt, /* top face */
		double *bb, double *mb, /* bottom face */
		double *mr, double *rr, /* right face */
		double *ll, double *ml, /* left face */
		double cr, double cs,
		double at, double bt, double Jt, double gut, double gvt, /* top face */
		double as, double bs, double Js, double gub, double gvb, /* bottom face */
		double ar, double br, double Jr, double gur, double gvr, /* right face */
		double al, double bl, double Jl, double gul, double gvl, /* left face */
		double J, double gu, double gv, double temp, double dt, double *rp);

double HLL_fix (double *u, 
		double *mt, double *tt, /* top face */
		double *bb, double *mb, /* bottom face */
		double *mr, double *rr, /* right face */
		double *ll, double *ml, /* left face */
		double cr, double cs,
		double at, double bt, double Jt, double gut, double gvt, /* top face */
		double as, double bs, double Js, double gub, double gvb, /* bottom face */
		double ar, double br, double Jr, double gur, double gvr, /* right face */
		double al, double bl, double Jl, double gul, double gvl, /* left face */
		double J, double gu, double gv, double temp, double dt, double *rp);

#endif
