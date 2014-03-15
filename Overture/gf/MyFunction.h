#ifndef MyFunction_h
#define MyFunction_h "MyFunction.h"

#include "OGFunction.h"

//===========================================================================================
// Define a Function and it derivatives
//   This function can be used to define the "exact solution" for
//   an Overlapping Grid Appliciation (aka. TwilightZone Flow)
//
//   Define an "exact solution" for an incompressible flow
//
//===========================================================================================
#define LF(x)   (pow((x),4)-2.*pow((x),3)+pow((x),2))
#define LFx(x)  (4.*pow((x),3)-6.*(x)*(x)+2.*(x))
#define LFxx(x) (12.*(x)*(x)-12.*(x)+2.)
#define LFxxx(x) (24.*(x)-12.)

#define LG(y) (pow((y),4)-(y)*(y))
#define LGy(y) (4.*pow((y),3)-2.*(y))
#define LGyy(y) (12.*(y)*(y)-2.)
#define LGyy(y) (12.*(y)*(y)-2.)
#define LGyyy(y) (24.*(y))
#define LGyyyy(y) (24.)


#define F(x) ( .2*pow((x),5) - .5*pow((x),4) + pow((x),3)/3. )
#define Fx(x) ( pow((x),4) - 2.*pow((x),3) + (x)*(x) )
#define Fxx(x) ( 4.*pow((x),3) - 6.*(x)*(x) + 2.*(x) )

#define U(x,y)   (8.*((x)*(x)*(x)*(x)-2.*(x)*(x)*(x)+(x)*(x))*(4.*(y)*(y)*(y)-2.*(y)))
#define UX(x,y)  (8.*(4.*pow((x),3.)-6.*pow((x),2.)+2.*(x))*(4.*pow((y),3.)-2.*(y)))
#define UY(x,y)  (8.*((x)*(x)*(x)*(x)-2.*(x)*(x)*(x)+(x)*(x))*(12.*(y)*(y)-2.))

#define UXX(x,y) (8.*(12.*(x)*(x)-12.*(x)+2.)*(4.*pow((y),3.)-2.*(y)))
#define UXY(x,y) (8.*(4.*pow((x),3.)-6.*pow((x),2.)+2.*(x))*(12.*(y)*(y)-2.))
#define UYY(x,y) (8.*(pow((x),4.)-2.*pow((x),3.)+(x)*(x))*(24.*(y)))

#define V(x,y)   (-8.*(4.*(x)*(x)*(x)-6.*(x)*(x)+2.*(x))*(pow((y),4.)-(y)*(y)))

#define VX(x,y)  (-8.*(12.*(x)*(x)-12.*(x)+2.)*(pow((y),4.)-(y)*(y)))

#define VY(x,y)  (-8.*(4.*pow((x),3.)-6.*(x)*(x)+2.*(x))*(4.*pow((y),3.)-2.*(y)))

#define VXX(x,y) (-8.*(24.*(x)-12.)*(pow((y),4.)-(y)*(y)))

#define VXY(x,y) (-8.*(12.*(x)*(x)-12.*(x)+2.)*(4.*pow((y),3.)-2.*(y)))
#define VYY(x,y) (-8.*(4.*pow((x),3.)-6.*(x)*(x)+2.*(x))*(12.*(y)*(y)-2.))


#define P(x,y) (  8.*reInverse*( F(x)*LGyyy(y)+LFx(x)*LGy(y) )   \
                  +32.*LF(x)*LF(x)*( LG(y)*LGyy(y)-LGy(y)*LGy(y) ) )

#define PX(x,y) (  8.*reInverse*( Fx(x)*LGyyy(y)+LFxx(x)*LGy(y) )   \
                  +64.*LF(x)*LFx(x)*( LG(y)*LGyy(y)-LGy(y)*LGy(y) )  )
#define PXX(x,y)(  8.*reInverse*( Fxx(x)*LGyyy(y)+LFxxx(x)*LGy(y) )   \
                  +64.*(LFx(x)*LFx(x)+LF(x)*LFxx(x))*( LG(y)*LGyy(y)-LGy(y)*LGy(y) )  )

#define PY(x,y) (  8.*reInverse*( F(x)*LGyyyy(y)+LFx(x)*LGyy(y) )   \
                  +32.*LF(x)*LF(x)*( LG(y)*LGyyy(y)-LGyy(y)*LGy(y) )  )

#define PYY(x,y) (  8.*reInverse*( LFx(x)*LGyyy(y) )   \
                  +32.*LF(x)*LF(x)*( LG(y)*LGyyyy(y)-LGyy(y)*LGyy(y) )  )

#define PXY(x,y) (  8.*reInverse*( Fx(x)*LGyyyy(y)+LFxx(x)*LGyy(y) )   \
                  +64.*LF(x)*LFx(x)*( LG(y)*LGyyy(y)-LGyy(y)*LGy(y) )  )

class MyFunction : public OGFunction
{

 private:
  int numberOfComponents;
  real reInverse;
  void checkArguments(const Index & N);

 public:
  

 //  
 // Create a polynomial with the given degree, number Of Space Dimensions and for
 // a maximum number of components
 // 
  MyFunction(const real & reynoldsNumber=100. );
  MyFunction(const MyFunction & ogp );
  MyFunction & operator=(const MyFunction & ogp );
  ~MyFunction(){}
  
  // Here are the member functions that you must define
  //     u(x,y,z,n) : function value at x,y,z for component n
  //    ux(x,y,z,n) : partial derivative of u with respect to x
  //      ...etc...


  inline real u(const real x, const real y, const real z, const int n, const real t=0.)
  {
    if( n==0 )
      return U(x,y);
    else if( n==1 ) 
      return V(x,y);
    else
      return P(x,y);
  }
  inline real ut(const real x, const real y, const real z, const int n=0, const real t=0. )
  {
    return 0.;
  }
  inline real ux(const real x, const real y, const real z, const int n=0, const real t=0.)
  {
    return  (n==0 ? UX(x,y) : ( n==1 ? VX(x,y) : PX(x,y) ));
  }
  inline real uy(const real x, const real y, const real z, const int n=0, const real t=0.)
  {
    if( n==0 )
      return UY(x,y);
    else if( n==1 ) 
      return VY(x,y);
    else
      return PY(x,y);
  }
  inline real uxx(const real x, const real y, const real , const int n=0, const real t=0.)
  {
    return  n==0 ? UXX(x,y) : ( n==1 ? VXX(x,y) : PXX(x,y) );
  }
  inline real uxy(const real x, const real y, const real , const int n=0, const real t=0.)
  {
    return  n==0 ? UXY(x,y) : ( n==1 ? VXY(x,y) : PXY(x,y) );
  }
  inline real uyy(const real x, const real y, const real , const int n=0, const real t=0.)
  {
    return  n==0 ? UYY(x,y) : ( n==1 ? VYY(x,y) : PYY(x,y) );
  }
  inline real uz(const real x, const real y, const real z, const int n=0, const real t=0.)
  {
    return  0.;
  }
  inline real uxz(const real , const real , const real , const int n=0, const real t=0.)
  {
    return  0.;
  }
  inline real uyz(const real , const real , const real , const int n=0, const real t=0.)
  {
    return  0.;
  }
  inline real uzz(const real , const real , const real z, const int n=0, const real t=0.)
  {
    return  0.;
  }

  // Here are the non-inlined versions : Use these when OGFunction
  // is passed as an argument

  real v(const real , const real , const real , const int n=0, const real t=0. );

  real vt(const real , const real , const real , const int n=0, const real t=0. );

  real vx(const real , const real , const real , const int n=0, const real t=0. );

  real vy(const real , const real , const real , const int n=0, const real t=0. );

  real vxx(const real , const real , const real , const int n=0, const real t=0. );
  
  real vxy(const real , const real , const real , const int n=0, const real t=0. );
  
  real vyy(const real , const real , const real , const int n=0, const real t=0. );
  
  real vz(const real , const real , const real , const int n=0, const real t=0. );
  
  real vxz(const real , const real , const real , const int n=0, const real t=0. );
  
  real vyz(const real , const real , const real , const int n=0, const real t=0. );

  real vzz(const real , const real , const real , const int n=0, const real t=0. );

  // These versions take Index objects as input and return an A++
  // array 

  RealArray u(const MappedGrid & c, const Index & I1, const Index & I2, 
              const Index & I3, const Index & N, const real t=0.);

  RealArray ut(const MappedGrid & c, const Index & I1, const Index & I2, 
              const Index & I3, const Index & N, const real t=0.);

  RealArray ux(const MappedGrid & c, const Index & I1, const Index & I2, 
              const Index & I3, const Index & N, const real t=0.);

  RealArray uy(const MappedGrid & c, const Index & I1, const Index & I2, 
              const Index & I3, const Index & N, const real t=0.);

  RealArray uz(const MappedGrid & c, const Index & I1, const Index & I2, 
              const Index & I3, const Index & N, const real t=0.);

  RealArray uxx(const MappedGrid & c, const Index & I1, const Index & I2, 
              const Index & I3, const Index & N, const real t=0.);

  RealArray uyy(const MappedGrid & c, const Index & I1, const Index & I2, 
              const Index & I3, const Index & N, const real t=0.);

  RealArray uzz(const MappedGrid & c, const Index & I1, const Index & I2, 
              const Index & I3, const Index & N, const real t=0.);

  RealArray uxy(const MappedGrid & c, const Index & I1, const Index & I2, 
              const Index & I3, const Index & N, const real t=0.);

  RealArray uxz(const MappedGrid & c, const Index & I1, const Index & I2, 
              const Index & I3, const Index & N, const real t=0.);

  RealArray uyz(const MappedGrid & c, const Index & I1, const Index & I2, 
              const Index & I3, const Index & N, const real t=0.);

  realCompositeGridFunction u (CompositeGrid & cg, const int n=0, const real t=0.);
  realCompositeGridFunction ut(CompositeGrid & cg, const int n=0, const real t=0.);
  realCompositeGridFunction ux(CompositeGrid & cg, const int n=0, const real t=0.);
  realCompositeGridFunction uy(CompositeGrid & cg, const int n=0, const real t=0.);
  realCompositeGridFunction uz(CompositeGrid & cg, const int n=0, const real t=0.);
  realCompositeGridFunction uxx(CompositeGrid & cg, const int n=0, const real t=0.);
  realCompositeGridFunction uxy(CompositeGrid & cg, const int n=0, const real t=0.);
  realCompositeGridFunction uxz(CompositeGrid & cg, const int n=0, const real t=0.);
  realCompositeGridFunction uyy(CompositeGrid & cg, const int n=0, const real t=0.);
  realCompositeGridFunction uyz(CompositeGrid & cg, const int n=0, const real t=0.);
  realCompositeGridFunction uzz(CompositeGrid & cg, const int n=0, const real t=0.);
};

#undef LF
#undef LFx
#undef LFxx
#undef LFxxx
#undef LG
#undef LGy
#undef LGyy
#undef LGyy
#undef LGyyy
#undef LGyyyy
#undef F
#undef Fx
#undef Fxx
#undef U
#undef UX
#undef UY
#undef UXX
#undef UXY
#undef UYY
#undef V
#undef VX
#undef VY
#undef VXX
#undef VXY
#undef VYY
#undef P
#undef PX
#undef PXX
#undef PY
#undef PYY
#undef PXY

#endif 
