#ifndef OGExact_h
#define OGExact_h "OGExact.h"
//
// Define Exact Solution "Twilight Zone" flows for overlapping
// grid applications
//

class OGExactSolution
{

  real fx,fy,fz,pi;

 public:
  

  OGExactSolution( const real fx=1., const real fy=1., const real fz=0. )
  { OGExactSolution::fx=fx; OGExactSolution::fy=fy; OGExactSolution::fz=fz; 
    pi=4.*atan(1.);
  }
    
  ~OGExactSolution(){};
  
  inline real u(const real x, const real y, const real z, const int )
  {
    return cos(fx*pi*x)*cos(fy*pi*y)*cos(fz*pi*z);
  }
  inline real ux(const real x, const real y, const real z, const int )
  {
    return -fx*pi*      sin(fx*pi*x)*cos(fy*pi*y)*cos(fz*pi*z);
  }
  inline real uy(const real x, const real y, const real z, const int )
  {
    return -fy*pi*      cos(fx*pi*x)*sin(fy*pi*y)*cos(fz*pi*z);
  }
  inline real uxx(const real x, const real y, const real z, const int )
  {
    return -fx*pi*fx*pi*cos(fx*pi*x)*cos(fy*pi*y)*cos(fz*pi*z);
  }
  inline real uxy(const real x, const real y, const real z, const int )
  {
    return fx*pi*fy*pi*sin(fx*pi*x)*sin(fy*pi*y)*cos(fz*pi*z);
  }
  inline real uyy(const real x, const real y, const real z, const int )
  {
    return -fy*pi*fy*pi*cos(fx*pi*x)*cos(fy*pi*y)*cos(fz*pi*z);
  }
  inline real uz(const real x, const real y, const real z, const int )
  {
    return -fz*pi*      cos(fx*pi*x)*cos(fy*pi*y)*sin(fz*pi*z);
  }
  inline real uxz(const real x, const real y, const real z, const int )
  {
    return fx*pi*fz*pi*sin(fx*pi*x)*cos(fy*pi*y)*sin(fz*pi*z);
  }
  inline real uyz(const real x, const real y, const real z, const int )
  {
    return fy*pi*fz*pi*cos(fx*pi*x)*sin(fy*pi*y)*sin(fz*pi*z);
  }
  inline real uzz(const real x, const real y, const real z, const int )
  {
    return -fz*pi*fz*pi*cos(fx*pi*x)*cos(fy*pi*y)*cos(fz*pi*z);
  }

};

#endif # OGExact_h
