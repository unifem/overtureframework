#include <math.h>
#include "riemann.h"
#include "macros.h"

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
		double J, double gu, double gv, double moveRHS, double dt, double *rp)
{
  double rho, v1, v2, p, E, L;
  double wl, wr, Vsq;
  double rhot, pt, Et, Lt, Yt;
  double drho, dv1, dv2, dE, dL, dY;
  double Ell, Eml, Err, Emr, Lll, Lml, Lrr, Lmr;
  double gam, pie, abswl, abswr;
  double dp;
  double f_fs[5], g_fs[5], ysr, xrs, xsr, yrs, dr, ds;
  double wrng, wlng;

  dr = dt/cr;
  ds = dt/cs;

  rho = u[0];
  v1 = u[1]/rho;
  v2 = u[2]/rho;
  Vsq = v1*v1+v2*v2;
  L = u[4]/rho;
  E = u[3];
  gam = gamma(L);
  pie = pi(L);
  p = (gam-1.0)*(E-0.5*rho*(Vsq))-gam*pie;

  /* find correction in east/west direction */
  wr = ar*(v1-gur)+br*(v2-gvr);
  wl = al*(v1-gul)+bl*(v2-gvl);
  wrng = ar*v1+br*v2;
  wlng = al*v1+bl*v2;
  abswl = Jl*fabs(wl);
  abswr = Jr*fabs(wr);
  
  drho = -cr*0.5*(Jr*wr*(rr[0]+mr[0])-Jl*wl*(ml[0]+ll[0])
		  -abswr*(rr[0]-mr[0])+abswl*(ml[0]-ll[0]));
  dY = -cr*0.5*(Jr*wr*(rr[4]+mr[4])-Jl*wl*(ml[4]+ll[4])
		-abswr*(rr[4]-mr[4])+abswl*(ml[4]-ll[4]));

  Lll = ll[4]/ll[0];
  Lml = ml[4]/ml[0];
  Lrr = rr[4]/rr[0];
  Lmr = mr[4]/mr[0];

  gam = gamma(Lll);
  pie = pi(Lll);
  Ell = (p+gam*pie)/(gam-1.0)+0.5*ll[0]*(Vsq); 
  gam = gamma(Lml);
  pie = pi(Lml);
  Eml = (p+gam*pie)/(gam-1.0)+0.5*ml[0]*(Vsq);
  gam = gamma(Lrr);
  pie = pi(Lrr);
  Err = (p+gam*pie)/(gam-1.0)+0.5*rr[0]*(Vsq);
  gam = gamma(Lmr);
  pie = pi(Lmr);
  Emr = (p+gam*pie)/(gam-1.0)+0.5*mr[0]*(Vsq);
  
  dE = -cr*0.5*(Jr*(wr*(Err+Emr)+wrng*2.0*p)-Jl*(wl*(Eml+Ell)+wlng*2.0*p)
		-abswr*(Err-Emr)+abswl*(Eml-Ell));

  /* find correction in north/south direction */
  wr = at*(v1-gut)+bt*(v2-gvt);
  wl = as*(v1-gub)+bs*(v2-gvb);
  wrng = at*v1+bt*v2;
  wlng = as*v1+bs*v2;
  abswl = Js*fabs(wl);
  abswr = Jt*fabs(wr);
  
  drho += -cs*0.5*(Jt*wr*(tt[0]+mt[0])-Js*wl*(mb[0]+bb[0])
		   -abswr*(tt[0]-mt[0])+abswl*(mb[0]-bb[0]));
  dY += -cs*0.5*(Jt*wr*(tt[4]+mt[4])-Js*wl*(mb[4]+bb[4])
		 -abswr*(tt[4]-mt[4])+abswl*(mb[4]-bb[4]));
  Lll = bb[4]/bb[0];
  Lml = mb[4]/mb[0];
  Lrr = tt[4]/tt[0];
  Lmr = mt[4]/mt[0];
  gam = gamma(Lll);
  pie = pi(Lll);
  Ell = (p+gam*pie)/(gam-1.0)+0.5*bb[0]*(Vsq);
  gam = gamma(Lml);
  pie = pi(Lml);
  Eml = (p+gam*pie)/(gam-1.0)+0.5*mb[0]*(Vsq);
  gam = gamma(Lrr);
  pie = pi(Lrr);
  Err = (p+gam*pie)/(gam-1.0)+0.5*tt[0]*(Vsq);
  gam = gamma(Lmr);
  pie = pi(Lmr);
  Emr = (p+gam*pie)/(gam-1.0)+0.5*mt[0]*(Vsq);
  
  dE += -cs*0.5*(Jt*(wr*(Err+Emr)+wrng*2.0*p)-Js*(wl*(Eml+Ell)+wlng*2.0*p)
		 -abswr*(Err-Emr)+abswl*(Eml-Ell));

  /* Perform free stream correction */
  flux(f_fs, u, p, 1.0, 0.0, 1.0, gu, gv);
  flux(g_fs, u, p, 0.0, 1.0, 1.0, gu, gv);
  ysr = (Jr*ar-Jl*al)/dr;
  xsr = (-Jr*br+Jl*bl)/dr;
  yrs = (-Jt*at+Js*as)/ds;
  xrs = (Jt*bt-Js*bs)/ds;

  drho += -dt*((yrs-ysr)*f_fs[0]+(xsr-xrs)*g_fs[0]);
  dE += -dt*((yrs-ysr)*f_fs[3]+(xsr-xrs)*g_fs[3]);
  dY += -dt*((yrs-ysr)*f_fs[4]+(xsr-xrs)*g_fs[4]);

  rhot = rho+drho/J+dt*moveRHS*u[0];
  Et = E+dE/J+dt*moveRHS*u[3];
  Yt = u[4]+dY/J+dt*moveRHS*u[4];

  Lt = Yt/rhot;
  gam = gamma(Lt);
  pie = pi(Lt);
  pt = (gam-1.0)*(Et-0.5*rhot*(Vsq))-gam*pie;
  dp = p-pt;

  return (dp);
}

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
		double J, double gu, double gv, double moveRHS, double dt,
		double *rp)
{
  double rho, v1, v2, p, E, L;
  double wl, wr, Vsq, srp, slp, srm, slm;
  double gam, pie;
  double radl, radr;
  double crr, cmr, cll, cml;
  double rhot, pt, Lt, Et, Yt;
  double drho, dE, dY;
  double Lll, Lml, Lrr, Lmr, Ell, Eml, Err, Emr;
  double dp;
  double f_fs[5], g_fs[5], ysr, xrs, xsr, yrs, dr, ds;
  double wlng, wrng;

  dr = dt/cr;
  ds = dt/cs;
  
  rho = u[0];
  v1 = u[1]/rho;
  v2 = u[2]/rho;
  Vsq = v1*v1+v2*v2;
  L = u[4]/rho;
  E = u[3];
  gam = gamma(L);
  pie = pi(L);
  p = (gam-1.0)*(E-0.5*rho*(Vsq))-gam*pie;

  /* find corrections in the east/west direction */
  Lll = ll[4]/ll[0];
  Lml = ml[4]/ml[0];
  Lrr = rr[4]/rr[0];
  Lmr = mr[4]/mr[0];

  radl = sqrt(al*al+bl*bl);
  radr = sqrt(ar*ar+br*br);
  wr = (ar*(v1-gur)+br*(v2-gvr))/radr;
  wl = (al*(v1-gul)+bl*(v2-gvl))/radl;

  gam = gamma(Lll);
  pie = pi(Lll);
  Ell = (p+gam*pie)/(gam-1.0)+0.5*ll[0]*(Vsq);
  cll = sqrt(gam*(p+pie)/ll[0]);
  gam = gamma(Lml);
  pie = pi(Lml);
  Eml = (p+gam*pie)/(gam-1.0)+0.5*ml[0]*(Vsq);
  cml = sqrt(gam*(p+pie)/ml[0]);
  gam = gamma(Lmr);
  pie = pi(Lmr);
  Emr = (p+gam*pie)/(gam-1.0)+0.5*mr[0]*(Vsq);
  cmr = sqrt(gam*(p+pie)/mr[0]);
  gam = gamma(Lrr);
  pie = pi(Lrr);
  Err = (p+gam*pie)/(gam-1.0)+0.5*rr[0]*(Vsq);
  crr = sqrt(gam*(p+pie)/rr[0]);

  slm = Jl*radl*(min(wl-cll, wl-cml));
  srm = Jl*radl*(max(wl+cll, wl+cml));
  slp = Jr*radr*(min(wr-cmr, wr-crr));
  srp = Jr*radr*(max(wr+cmr, wr+crr));

  wrng = (ar*v1+br*v2);
  wlng = (al*v1+bl*v2);
  wr = wr*radr;
  wl = wl*radl;

  drho = -cr*(1.0/(srp-slp)*(Jr*wr*(srp*mr[0]-slp*rr[0])+slp*srp*(rr[0]-mr[0]))
	      -1.0/(srm-slm)*(Jl*wl*(srm*ll[0]-slm*ml[0])+slm*srm*(ml[0]-ll[0])));
  dY = -cr*(1.0/(srp-slp)*(Jr*wr*(srp*mr[4]-slp*rr[4])+slp*srp*(rr[4]-mr[4]))
	    -1.0/(srm-slm)*(Jl*wl*(srm*ll[4]-slm*ml[4])+slm*srm*(ml[4]-ll[4])));
  dE = -cr*(1.0/(srp-slp)*(Jr*(srp*(wr*Emr+wrng*p)-slp*(wr*Err+wrng*p))+slp*srp*(Err-Emr))
	    -1.0/(srm-slm)*(Jl*(srm*(wl*Ell+wlng*p)-slm*(wl*Eml+wlng*p))+slm*srm*(Eml-Ell)));
  
  /* find correction in north/south direction */
  Lll = bb[4]/bb[0];
  Lml = mb[4]/mb[0];
  Lrr = tt[4]/tt[0];
  Lmr = mt[4]/mt[0];

  radl = sqrt(as*as+bs*bs);
  radr = sqrt(at*at+bt*bt);
  wr = (at*(v1-gut)+bt*(v2-gvt))/radr;
  wl = (as*(v1-gub)+bs*(v2-gvb))/radl;

  gam = gamma(Lll);
  pie = pi(Lll);
  Ell = (p+gam*pie)/(gam-1.0)+0.5*bb[0]*(Vsq); 	    
  cll = sqrt(gam*(p+pie)/bb[0]);
  gam = gamma(Lml);
  pie = pi(Lml);
  Eml = (p+gam*pie)/(gam-1.0)+0.5*mb[0]*(Vsq);
  cml = sqrt(gam*(p+pie)/mb[0]);
  gam = gamma(Lmr);
  pie = pi(Lmr);
  Emr = (p+gam*pie)/(gam-1.0)+0.5*mt[0]*(Vsq);
  cmr = sqrt(gam*(p+pie)/mt[0]);
  gam = gamma(Lrr);
  pie = pi(Lrr);
  Err = (p+gam*pie)/(gam-1.0)+0.5*tt[0]*(Vsq);
  crr = sqrt(gam*(p+pie)/tt[0]);

  slm = Js*radl*(min(wl-cll, wl-cml));
  srm = Js*radl*(max(wl+cll, wl+cml));
  slp = Jt*radr*(min(wr-cmr, wr-crr));
  srp = Jt*radr*(max(wr+cmr, wr+crr));

  wrng = (at*v1+bt*v2);
  wlng = (as*v1+bs*v2);
  wr = wr*radr;
  wl = wl*radl;

  drho += -cs*(1.0/(srp-slp)*(Jt*wr*(srp*mt[0]-slp*tt[0])+slp*srp*(tt[0]-mt[0]))
	       -1.0/(srm-slm)*(Js*wl*(srm*bb[0]-slm*mb[0])+slm*srm*(mb[0]-bb[0])));
  dY += -cs*(1.0/(srp-slp)*(Jt*wr*(srp*mt[4]-slp*tt[4])+slp*srp*(tt[4]-mt[4]))
	     -1.0/(srm-slm)*(Js*wl*(srm*bb[4]-slm*mb[4])+slm*srm*(mb[4]-bb[4])));
  dE += -cs*(1.0/(srp-slp)*(Jt*(srp*(wr*Emr+wrng*p)-slp*(wr*Err+wrng*p))+slp*srp*(Err-Emr))
	    -1.0/(srm-slm)*(Js*(srm*(wl*Ell+wlng*p)-slm*(wl*Eml+wlng*p))+slm*srm*(Eml-Ell)));
  
  /* Perform free stream correction */
  flux(f_fs, u, p, 1.0, 0.0, 1.0, gu, gv);
  flux(g_fs, u, p, 0.0, 1.0, 1.0, gu, gv);
  ysr = (Jr*ar-Jl*al)/dr;
  xsr = (-Jr*br+Jl*bl)/dr;
  yrs = (-Jt*at+Js*as)/ds;
  xrs = (Jt*bt-Js*bs)/ds;

  drho += -dt*((yrs-ysr)*f_fs[0]+(xsr-xrs)*g_fs[0]);
  dE += -dt*((yrs-ysr)*f_fs[3]+(xsr-xrs)*g_fs[3]);
  dY += -dt*((yrs-ysr)*f_fs[4]+(xsr-xrs)*g_fs[4]);

  rhot = rho+drho/J+dt*moveRHS*u[0];
  Et = E+dE/J+dt*moveRHS*u[3];
  Yt = u[4]+dY/J+dt*moveRHS*u[4];

  Lt = Yt/rhot;
  gam = gamma(Lt);
  pie = pi(Lt);
  pt = (gam-1.0)*(Et-0.5*rhot*(Vsq))-gam*pie;
  dp = p-pt;
  
  return (dp);
}
