// Here we define function that is to be called by Jeff's code to impliment
// the twilight zone flows. This function will take a location in space/time
// and return the associated vecotor right hand side forcing function.
#include "Parameters.h"
#include "OGPolyFunction.h"
#include "MappedGridOperators.h"
#include "macros.h"

extern "C" void twilightSource(void *exactP, double *rp, 
			       void *mgP, double x, double y,
			       double t, double h[5]);

void twilightSource (void *exactP, double *rp, 
		     void *mgP, double x, double y,
		     double t, double h[5])
{
  OGFunction & ex = *((OGFunction*)exactP);
  MappedGrid & mg = *((MappedGrid*)mgP);
  int rc = 0;
  int uc = 1;
  int vc = 2;
  int tc = 3;
  int sc = 4;

  // get rho and derivatives
  double r  = ex(x,y,0,rc,t);
  double rx = ex.x(x,y,0,rc,t);
  double ry = ex.y(x,y,0,rc,t);
  double rt = ex.t(x,y,0,rc,t);

  // get u and derivatives
  double u  = ex(x,y,0,uc,t);
  double ux = ex.x(x,y,0,uc,t);
  double uy = ex.y(x,y,0,uc,t);
  double ut = ex.t(x,y,0,uc,t);

  // get v and derivatives
  double v  = ex(x,y,0,vc,t);
  double vx = ex.x(x,y,0,vc,t);
  double vy = ex.y(x,y,0,vc,t);
  double vt = ex.t(x,y,0,vc,t);

  // Get T and derivatives ... note P=rho*T so T is NOT temperature
  double T  = ex(x,y,0,tc,t);
  double Tx = ex.x(x,y,0,tc,t);
  double Ty = ex.y(x,y,0,tc,t);
  double Tt = ex.t(x,y,0,tc,t);
  
  // get P and derivatives
  double P  = r*T;
  double Px = rx*T+r*Tx;
  double Py = ry*T+r*Ty;
  double Pt = rt*T+r*Tt;

  // get lambda and derivatives
  double L  = ex(x,y,0,sc,t);
  double Lx = ex.x(x,y,0,sc,t);
  double Ly = ex.y(x,y,0,sc,t);
  double Lt = ex.t(x,y,0,sc,t);
  
  // get gamma, gamma', pie, and pie' as grid functions
  double gam = gamma(L);
  double gamp = gamma_prime(L);
  double pie = pi(L);
  double piep = pi_prime(L);

  // get E and derivatives
  double E  = (P+gam*pie)/(gam-1.0)+0.5*r*(u*u+v*v);
  double Ex = -(P+gam*pie)*(Lx*gamp)/((gam-1.0)*(gam-1.0))+(Px+Lx*(gamp*pie+gam*piep))/(gam-1.0)+r*(u*ux+v*vx)+0.5*rx*(u*u+v*v);
  double Ey = -(P+gam*pie)*(Ly*gamp)/((gam-1.0)*(gam-1.0))+(Py+Ly*(gamp*pie+gam*piep))/(gam-1.0)+r*(u*uy+v*vy)+0.5*ry*(u*u+v*v);
  double Et = -(P+gam*pie)*(Lt*gamp)/((gam-1.0)*(gam-1.0))+(Pt+Lt*(gamp*pie+gam*piep))/(gam-1.0)+r*(u*ut+v*vt)+0.5*rt*(u*u+v*v);

  /*cout << E << " " << Et << " " << Ex << " " << Ey <<endl; 
    cout << L << " " << Lt << " " << Lx << " " << Ly <<endl; 
    cout << P << " " << Pt << " " << Px << " " << Py <<endl;
    cout << u << " " << ut << " " << ux << " " << uy <<endl;
    cout << v << " " << vt << " " << vx << " " << vy <<endl;*/
  
  // Set the right hand side arrays
  h[0] = rt+r*(ux+vy)+rx*u+ry*v;
  h[1] = rt*u+r*ut+2.0*r*u*ux+rx*u*u+Px+ry*u*v+r*uy*v+r*u*vy;
  h[2] = rt*v+r*vt+rx*u*v+r*ux*v+r*u*vx+2.0*r*v*vy+ry*v*v+Py;
  h[3] = Et+(ux+vy)*(E+P)+u*(Ex+Px)+v*(Ey+Py);
  h[4] = rt*L+r*Lt+rx*u*L+r*ux*L+r*u*Lx+ry*v*L+r*vy*L+r*v*Ly;
  /*for (int i=0; i<5; i++)
    {
    cout << h[i] << endl;
    }
    cout << "--------------------\n";
    exit (0);*/
  
  return;
}
