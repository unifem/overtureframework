#include "MyFunction.h"

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

MyFunction::
MyFunction(const real & reynoldsNumber )
{
  numberOfComponents=3;
  reInverse=1./reynoldsNumber;
}

MyFunction::
MyFunction(const MyFunction & ogp )   // copy constructor is a deep copy
{
  *this=ogp;
}

//--------------------------------------------------------------------------------------
//  operator = is  a deep copy
//--------------------------------------------------------------------------------------
MyFunction & MyFunction::
operator=(const MyFunction & ogp )
{
  return *this;
}



// Here are the non-inlined versions : Use these when OGFunction
// is passed as an argument


real MyFunction::
v(const real x, const real y, const real z, const int n, const real t )
{
  return u(x,y,z,n,t);
}
real MyFunction::
vt(const real x, const real y, const real z, const int n, const real t )
{
  return ut(x,y,z,n,t);
}
real MyFunction::
vx(const real x, const real y, const real z, const int n, const real t )
{
  return ux(x,y,z,n,t);
}
real MyFunction::
vy(const real x, const real y, const real z, const int n, const real t )
{
  return uy(x,y,z,n,t);
}
real MyFunction::
vxx(const real x, const real y, const real z, const int n, const real t )
{
  return uxx(x,y,z,n,t);
}
real MyFunction::
vxy(const real x, const real y, const real z, const int n, const real t )
{
  return uxy(x,y,z,n,t);
}
real MyFunction::
vyy(const real x, const real y, const real z, const int n, const real t )
{
  return uyy(x,y,z,n,t);
}
real MyFunction::
vz(const real x, const real y, const real z, const int n, const real t )
{
  return uz(x,y,z,n,t);
}
real MyFunction::
vxz(const real x, const real y, const real z, const int n, const real t )
{
  return uxz(x,y,z,n,t);
}
real MyFunction::
vyz(const real x, const real y, const real z, const int n, const real t )
{
  return uyz(x,y,z,n,t);
}
real MyFunction::
vzz(const real x, const real y, const real z, const int n, const real t )
{
  return uzz(x,y,z,n,t);
}


// This routine is used locally to check some arguments
void MyFunction::
checkArguments(const Index & N)
{
  if( N.getBound() >= numberOfComponents )
  {
    cout << "MyFunction:ERROR: trying to evaluate a function with " << N.getBound()+1 
         << " components, \n  but the polynomial only supports numberOfComponents = " 
	   << numberOfComponents << endl;
    cout << "Why not create the MyFunction object with more components :-) \n";
    throw  "MyFunction:ERROR";
  }
}


#define X coord(I1,I2,I3,axis1)
#define Y coord(I1,I2,I3,axis2)
#define Z coord(I1,I2,I3,axis3)


RealArray MyFunction::
u(const MappedGrid & c, const Index & I1,
                            const Index & I2,const Index & I3, const Index & N, const real t)
{
  checkArguments( N );
  const RealArray & coord = (bool)c.isAllCellCentered ? c.center : c.vertex;
  RealArray result(Range(I1.getBase(),I1.getBound()),
                   Range(I2.getBase(),I2.getBound()),
                   Range(I3.getBase(),I3.getBound()),
                   Range( N.getBase(), N.getBound()));
                   
  for( int n=N.getBase(); n<=N.getBound(); n++ )
    if( n==0 )
      result(I1,I2,I3,n)=U(X,Y);
    else if( n==1 )
      result(I1,I2,I3,n)=V(X,Y);
    else
      result(I1,I2,I3,n)=P(X,Y);
  return result;
  
}

RealArray MyFunction::
ux(const MappedGrid & c, const Index & I1,
                            const Index & I2,const Index & I3, const Index & N, const real t)
{
  checkArguments( N );
  const RealArray & coord = (bool)c.isAllCellCentered ? c.center : c.vertex;
  RealArray result(Range(I1.getBase(),I1.getBound()),
                   Range(I2.getBase(),I2.getBound()),
                   Range(I3.getBase(),I3.getBound()),
                   Range( N.getBase(), N.getBound()));
  for( int n=N.getBase(); n<=N.getBound(); n++ )
    if( n==0 )
      result(I1,I2,I3,n)=UX(X,Y);
    else if( n==1 )
      result(I1,I2,I3,n)=VX(X,Y);
    else
      result(I1,I2,I3,n)=PX(X,Y);
  return result;
}

RealArray MyFunction::
uy(const MappedGrid & c, const Index & I1,
                            const Index & I2,const Index & I3, const Index & N, const real t)
{
  checkArguments( N );
  const RealArray & coord = (bool)c.isAllCellCentered ? c.center : c.vertex;
  RealArray result(Range(I1.getBase(),I1.getBound()),
                   Range(I2.getBase(),I2.getBound()),
                   Range(I3.getBase(),I3.getBound()),
                   Range( N.getBase(), N.getBound()));
  for( int n=N.getBase(); n<=N.getBound(); n++ )
    if( n==0 )
      result(I1,I2,I3,n)=UY(X,Y);
    else if( n==1 )
      result(I1,I2,I3,n)=VY(X,Y);
    else
      result(I1,I2,I3,n)=PY(X,Y);
  return result;
}
RealArray MyFunction::
uz(const MappedGrid & c, const Index & I1,
                            const Index & I2,const Index & I3, const Index & N, const real t)
{
  checkArguments( N );
  const RealArray & coord = (bool)c.isAllCellCentered ? c.center : c.vertex;
  RealArray result(Range(I1.getBase(),I1.getBound()),
                   Range(I2.getBase(),I2.getBound()),
                   Range(I3.getBase(),I3.getBound()),
                   Range( N.getBase(), N.getBound()));
  result=0.;
  return result;

}
RealArray MyFunction::
ut(const MappedGrid & c, const Index & I1,
                            const Index & I2,const Index & I3, const Index & N, const real t)
{
  checkArguments( N );
  const RealArray & coord = (bool)c.isAllCellCentered ? c.center : c.vertex;
  RealArray result(Range(I1.getBase(),I1.getBound()),
                   Range(I2.getBase(),I2.getBound()),
                   Range(I3.getBase(),I3.getBound()),
                   Range( N.getBase(), N.getBound()));
  result=0.;
  return result;
}
RealArray MyFunction::
uxx(const MappedGrid & c, const Index & I1,
                            const Index & I2,const Index & I3, const Index & N, const real t)
{
  checkArguments( N );
  const RealArray & coord = (bool)c.isAllCellCentered ? c.center : c.vertex;
  RealArray result(Range(I1.getBase(),I1.getBound()),
                   Range(I2.getBase(),I2.getBound()),
                   Range(I3.getBase(),I3.getBound()),
                   Range( N.getBase(), N.getBound()));
  for( int n=N.getBase(); n<=N.getBound(); n++ )
    if( n==0 )
      result(I1,I2,I3,n)=UXX(X,Y);
    else if( n==1 )
      result(I1,I2,I3,n)=VXX(X,Y);
    else
      result(I1,I2,I3,n)=PXX(X,Y);
  return result;
}
RealArray MyFunction::
uyy(const MappedGrid & c, const Index & I1,
                            const Index & I2,const Index & I3, const Index & N, const real t)
{
  checkArguments( N );
  const RealArray & coord = (bool)c.isAllCellCentered ? c.center : c.vertex;
  RealArray result(Range(I1.getBase(),I1.getBound()),
                   Range(I2.getBase(),I2.getBound()),
                   Range(I3.getBase(),I3.getBound()),
                   Range( N.getBase(), N.getBound()));
  for( int n=N.getBase(); n<=N.getBound(); n++ )
    if( n==0 )
      result(I1,I2,I3,n)=UYY(X,Y);
    else if( n==1 )
      result(I1,I2,I3,n)=VYY(X,Y);
    else
      result(I1,I2,I3,n)=PYY(X,Y);
  return result;
}
RealArray MyFunction::
uzz(const MappedGrid & c, const Index & I1,
                            const Index & I2,const Index & I3, const Index & N, const real t)
{
  checkArguments( N );
  const RealArray & coord = (bool)c.isAllCellCentered ? c.center : c.vertex;
  RealArray result(Range(I1.getBase(),I1.getBound()),
                   Range(I2.getBase(),I2.getBound()),
                   Range(I3.getBase(),I3.getBound()),
                   Range( N.getBase(), N.getBound()));
  result=0.;
  return result;
}
RealArray MyFunction::
uxy(const MappedGrid & c, const Index & I1,
                            const Index & I2,const Index & I3, const Index & N, const real t)
{
  checkArguments( N );
  const RealArray & coord = (bool)c.isAllCellCentered ? c.center : c.vertex;
  RealArray result(Range(I1.getBase(),I1.getBound()),
                   Range(I2.getBase(),I2.getBound()),
                   Range(I3.getBase(),I3.getBound()),
                   Range( N.getBase(), N.getBound()));
  for( int n=N.getBase(); n<=N.getBound(); n++ )
    if( n==0 )
      result(I1,I2,I3,n)=UXY(X,Y);
    else if( n==1 )
      result(I1,I2,I3,n)=VXY(X,Y);
    else
      result(I1,I2,I3,n)=PXY(X,Y);
  return result;
}
RealArray MyFunction::
uxz(const MappedGrid & c, const Index & I1,
                            const Index & I2,const Index & I3, const Index & N, const real t)
{
  checkArguments( N );
  const RealArray & coord = (bool)c.isAllCellCentered ? c.center : c.vertex;
  RealArray result(Range(I1.getBase(),I1.getBound()),
                   Range(I2.getBase(),I2.getBound()),
                   Range(I3.getBase(),I3.getBound()),
                   Range( N.getBase(), N.getBound()));
  result=0.;
  return result;
}
RealArray MyFunction::
uyz(const MappedGrid & c, const Index & I1,
                            const Index & I2,const Index & I3, const Index & N, const real t)
{
  checkArguments( N );
  const RealArray & coord = (bool)c.isAllCellCentered ? c.center : c.vertex;
  RealArray result(Range(I1.getBase(),I1.getBound()),
                   Range(I2.getBase(),I2.getBound()),
                   Range(I3.getBase(),I3.getBound()),
                   Range( N.getBase(), N.getBound()));
  result=0.;
  return result;
}

#undef X
#undef Y
#undef Z
#define X coord(I[0],I[1],I[2],axis1)
#define Y coord(I[0],I[1],I[2],axis2)
#define Z coord(I[0],I[1],I[2],axis3)

/* --------
RealArray 
u(const realMappedGridFunction & u, 
  const real t,
  const Index & I0, 
  const Index & I1, 
  const Index & I2, 
  const Index & I3) 
{
  
  checkArguments( u,I0,I1,I2,I3 );
  const RealArray & coord = (bool)c.isAllCellCentered ? c.center : c.vertex;
  RealArray result(Range(I0.getBase(),I0.getBound()),
                   Range(I1.getBase(),I1.getBound()),
                   Range(I2.getBase(),I2.getBound()),
                   Range(I3.getBase(),I3.getBound()));
                   
  Index *II = { I0,I1,I2,I3 };   
  Index I[4];
  for( int i=0; i<3; i++ )
    I[i]=II[u.positionOfCoordinate(i)];
    
  Index & N = II[positionOfComponent(0)];
  
  int n;
  if( c.numberOfDimensions==2 )     
    for( n=N.getBase(); n<=N.getBound(); n++ )
      result(I0,I1,I2,I3)=
          (cc(0,0,0,n)+X*(cc(1,0,0,n)+X*(cc(2,0,0,n)+X*(cc(3,0,0,n)+X*cc(4,0,0,n))))
                      +Y*(cc(0,1,0,n)+Y*(cc(0,2,0,n)+Y*(cc(0,3,0,n)+Y*cc(0,4,0,n)))))*TIME(n,t);
  else
    for( n=N.getBase(); n<=N.getBound(); n++ )
      result(I0,I1,I2,I3)=
          (cc(0,0,0,n)+X*(cc(1,0,0,n)+X*(cc(2,0,0,n)+X*(cc(3,0,0,n)+X*cc(4,0,0,n))))
                    +Y*(cc(0,1,0,n)+Y*(cc(0,2,0,n)+Y*(cc(0,3,0,n)+Y*cc(0,4,0,n))))
  		    +Z*(cc(0,0,1,n)+Z*(cc(0,0,2,n)+Z*(cc(0,0,3,n)+Z*cc(0,0,4,n)))))*TIME(n,t);
  return result;
}  
---- */




realCompositeGridFunction MyFunction::
u (CompositeGrid & cg, const int n, const real t)
{
  return OGFunction::u(cg,n,t);
} 
realCompositeGridFunction MyFunction::
ut(CompositeGrid & cg, const int n, const real t)
{
  return OGFunction::ut(cg,n,t);
} 
realCompositeGridFunction MyFunction::
ux(CompositeGrid & cg, const int n, const real t)
{
  return OGFunction::ux(cg,n,t);
} 
realCompositeGridFunction MyFunction::
uy(CompositeGrid & cg, const int n, const real t)
{
  return OGFunction::uy(cg,n,t);
} 
realCompositeGridFunction MyFunction::
uz(CompositeGrid & cg, const int n, const real t)
{
  return OGFunction::uz(cg,n,t);
} 
realCompositeGridFunction MyFunction::
uxx(CompositeGrid & cg, const int n, const real t)
{
  return OGFunction::uxx(cg,n,t);
} 
realCompositeGridFunction MyFunction::
uxy(CompositeGrid & cg, const int n, const real t)
{
  return OGFunction::uxy(cg,n,t);
} 
realCompositeGridFunction MyFunction::
uxz(CompositeGrid & cg, const int n, const real t)
{
  return OGFunction::uxz(cg,n,t);
} 
realCompositeGridFunction MyFunction::
uyy(CompositeGrid & cg, const int n, const real t)
{
  return OGFunction::uyy(cg,n,t);
} 
realCompositeGridFunction MyFunction::
uyz(CompositeGrid & cg, const int n, const real t)
{
  return OGFunction::uyz(cg,n,t);
} 
realCompositeGridFunction MyFunction::
uzz(CompositeGrid & cg, const int n, const real t)
{
  return OGFunction::uzz(cg,n,t);
} 
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

#undef X
#undef Y
#undef Z
