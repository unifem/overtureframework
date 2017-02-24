! This file automatically generated from polyFunction.bf with bpp.
! polyFun(3D5)
      subroutine polyFunction3D5 (nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb,
     & ndrra,ndrrb,ndrsa,ndrsb,ndrta,ndrtb,ndrca,ndrcb,ndc1,ndc2,ndc3,
     & nra,nrb,nsa,nsb,nta,ntb, nca,ncb, nda, degree, degreeTime, t, 
     & a,c, r,xa,ya,za, dx,dy,dz,dt)
      !==========================================================================
      !   *** Define a polynomial function and it's derivatives ***
      ! nd : number of space dimensions
      ! nra,nrb,nsa,nsb,nta,ntb : return result in this array
      ! c : array of polynomial coefficients
      ! r  : return result in this array
      ! degree: degree of the polynomial
      ! dx,dy,dz: compute this derivative
      !==========================================================================
      integer nca,ncb,dx,dy,dz,dt,degree,degreeTime
      real xa(ndra:ndrb,ndsa:ndsb,ndta:ndtb)
      real ya(ndra:ndrb,ndsa:ndsb,ndta:ndtb)
      real za(ndra:ndrb,ndsa:ndsb,ndta:ndtb)
      ! real r(ndrra:ndrrb,ndrsa:ndrsb,ndrta:ndrtb,nca:ncb)
      real r(ndrra:ndrrb,ndrsa:ndrsb,ndrta:ndrtb,ndrca:ndrcb)
      real c(0:ndc1-1,0:ndc2-1,0:ndc3-1,0:*)
      real a(0:nda-1,0:*)
      real t
      integer n,i1,i2,i3,laplace
      real time
      laplace=0
      if( dx.eq.-2 )then
      laplace=1
      end if
! poly3D5(x1=xa(i1,i2,i3),y1=ya(i1,i2,i3),z1=za(i1,i2,i3),r(i1,i2,i3,n))
      if( dx.eq.0.and.dy.eq.0.and.dz.eq.0 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y1z5=y1z4*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
      y2z5=y1z5*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      y3z4=y2z4*y1
      y3z5=y2z5*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y4z2=y3z2*y1
      y4z3=y3z3*y1
      y4z4=y3z4*y1
      y4z5=y3z5*y1
      y5=y4*y1
      y5z1=y4z1*y1
      y5z2=y4z2*y1
      y5z3=y4z3*y1
      y5z4=y4z4*y1
      y5z5=y4z5*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1z5=x1z4*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y1z5=x1y1z4*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y2z4=x1y1z4*y1
      x1y2z5=x1y1z5*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x1y3z4=x1y2z4*y1
      x1y3z5=x1y2z5*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y4z2=x1y3z2*y1
      x1y4z3=x1y3z3*y1
      x1y4z4=x1y3z4*y1
      x1y4z5=x1y3z5*y1
      x1y5=x1y4*y1
      x1y5z1=x1y4z1*y1
      x1y5z2=x1y4z2*y1
      x1y5z3=x1y4z3*y1
      x1y5z4=x1y4z4*y1
      x1y5z5=x1y4z5*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x2z5=x1z5*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y1z4=x1y1z4*x1
      x2y1z5=x1y1z5*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y2z4=x1y2z4*x1
      x2y2z5=x1y2z5*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y3z3=x1y3z3*x1
      x2y3z4=x1y3z4*x1
      x2y3z5=x1y3z5*x1
      x2y4=x1y4*x1
      x2y4z1=x1y4z1*x1
      x2y4z2=x1y4z2*x1
      x2y4z3=x1y4z3*x1
      x2y4z4=x1y4z4*x1
      x2y4z5=x1y4z5*x1
      x2y5=x1y5*x1
      x2y5z1=x1y5z1*x1
      x2y5z2=x1y5z2*x1
      x2y5z3=x1y5z3*x1
      x2y5z4=x1y5z4*x1
      x2y5z5=x1y5z5*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3z4=x2z4*x1
      x3z5=x2z5*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x3y1z4=x2y1z4*x1
      x3y1z5=x2y1z5*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y2z3=x2y2z3*x1
      x3y2z4=x2y2z4*x1
      x3y2z5=x2y2z5*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x3y3z2=x2y3z2*x1
      x3y3z3=x2y3z3*x1
      x3y3z4=x2y3z4*x1
      x3y3z5=x2y3z5*x1
      x3y4=x2y4*x1
      x3y4z1=x2y4z1*x1
      x3y4z2=x2y4z2*x1
      x3y4z3=x2y4z3*x1
      x3y4z4=x2y4z4*x1
      x3y4z5=x2y4z5*x1
      x3y5=x2y5*x1
      x3y5z1=x2y5z1*x1
      x3y5z2=x2y5z2*x1
      x3y5z3=x2y5z3*x1
      x3y5z4=x2y5z4*x1
      x3y5z5=x2y5z5*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4z2=x3z2*x1
      x4z3=x3z3*x1
      x4z4=x3z4*x1
      x4z5=x3z5*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y1z2=x3y1z2*x1
      x4y1z3=x3y1z3*x1
      x4y1z4=x3y1z4*x1
      x4y1z5=x3y1z5*x1
      x4y2=x3y2*x1
      x4y2z1=x3y2z1*x1
      x4y2z2=x3y2z2*x1
      x4y2z3=x3y2z3*x1
      x4y2z4=x3y2z4*x1
      x4y2z5=x3y2z5*x1
      x4y3=x3y3*x1
      x4y3z1=x3y3z1*x1
      x4y3z2=x3y3z2*x1
      x4y3z3=x3y3z3*x1
      x4y3z4=x3y3z4*x1
      x4y3z5=x3y3z5*x1
      x4y4=x3y4*x1
      x4y4z1=x3y4z1*x1
      x4y4z2=x3y4z2*x1
      x4y4z3=x3y4z3*x1
      x4y4z4=x3y4z4*x1
      x4y4z5=x3y4z5*x1
      x4y5=x3y5*x1
      x4y5z1=x3y5z1*x1
      x4y5z2=x3y5z2*x1
      x4y5z3=x3y5z3*x1
      x4y5z4=x3y5z4*x1
      x4y5z5=x3y5z5*x1
      x5=x4*x1
      x5z1=x4z1*x1
      x5z2=x4z2*x1
      x5z3=x4z3*x1
      x5z4=x4z4*x1
      x5z5=x4z5*x1
      x5y1=x4y1*x1
      x5y1z1=x4y1z1*x1
      x5y1z2=x4y1z2*x1
      x5y1z3=x4y1z3*x1
      x5y1z4=x4y1z4*x1
      x5y1z5=x4y1z5*x1
      x5y2=x4y2*x1
      x5y2z1=x4y2z1*x1
      x5y2z2=x4y2z2*x1
      x5y2z3=x4y2z3*x1
      x5y2z4=x4y2z4*x1
      x5y2z5=x4y2z5*x1
      x5y3=x4y3*x1
      x5y3z1=x4y3z1*x1
      x5y3z2=x4y3z2*x1
      x5y3z3=x4y3z3*x1
      x5y3z4=x4y3z4*x1
      x5y3z5=x4y3z5*x1
      x5y4=x4y4*x1
      x5y4z1=x4y4z1*x1
      x5y4z2=x4y4z2*x1
      x5y4z3=x4y4z3*x1
      x5y4z4=x4y4z4*x1
      x5y4z5=x4y4z5*x1
      x5y5=x4y5*x1
      x5y5z1=x4y5z1*x1
      x5y5z2=x4y5z2*x1
      x5y5z3=x4y5z3*x1
      x5y5z4=x4y5z4*x1
      x5y5z5=x4y5z5*x1
      r(i1,i2,i3,n)=(c(0,0,0,n)+c(0,0,1,n)*z1+c(0,0,2,n)*z2+c(0,0,3,n)*
     & z3+c(0,0,4,n)*z4+c(0,0,5,n)*z5+c(0,1,0,n)*y1+c(0,1,1,n)*y1z1+c(
     & 0,1,2,n)*y1z2+c(0,1,3,n)*y1z3+c(0,1,4,n)*y1z4+c(0,1,5,n)*y1z5+
     & c(0,2,0,n)*y2+c(0,2,1,n)*y2z1+c(0,2,2,n)*y2z2+c(0,2,3,n)*y2z3+
     & c(0,2,4,n)*y2z4+c(0,2,5,n)*y2z5+c(0,3,0,n)*y3+c(0,3,1,n)*y3z1+
     & c(0,3,2,n)*y3z2+c(0,3,3,n)*y3z3+c(0,3,4,n)*y3z4+c(0,3,5,n)*
     & y3z5+c(0,4,0,n)*y4+c(0,4,1,n)*y4z1+c(0,4,2,n)*y4z2+c(0,4,3,n)*
     & y4z3+c(0,4,4,n)*y4z4+c(0,4,5,n)*y4z5+c(0,5,0,n)*y5+c(0,5,1,n)*
     & y5z1+c(0,5,2,n)*y5z2+c(0,5,3,n)*y5z3+c(0,5,4,n)*y5z4+c(0,5,5,n)
     & *y5z5+c(1,0,0,n)*x1+c(1,0,1,n)*x1z1+c(1,0,2,n)*x1z2+c(1,0,3,n)*
     & x1z3+c(1,0,4,n)*x1z4+c(1,0,5,n)*x1z5+c(1,1,0,n)*x1y1+c(1,1,1,n)
     & *x1y1z1+c(1,1,2,n)*x1y1z2+c(1,1,3,n)*x1y1z3+c(1,1,4,n)*x1y1z4+
     & c(1,1,5,n)*x1y1z5+c(1,2,0,n)*x1y2+c(1,2,1,n)*x1y2z1+c(1,2,2,n)*
     & x1y2z2+c(1,2,3,n)*x1y2z3+c(1,2,4,n)*x1y2z4+c(1,2,5,n)*x1y2z5+c(
     & 1,3,0,n)*x1y3+c(1,3,1,n)*x1y3z1+c(1,3,2,n)*x1y3z2+c(1,3,3,n)*
     & x1y3z3+c(1,3,4,n)*x1y3z4+c(1,3,5,n)*x1y3z5+c(1,4,0,n)*x1y4+c(1,
     & 4,1,n)*x1y4z1+c(1,4,2,n)*x1y4z2+c(1,4,3,n)*x1y4z3+c(1,4,4,n)*
     & x1y4z4+c(1,4,5,n)*x1y4z5+c(1,5,0,n)*x1y5+c(1,5,1,n)*x1y5z1+c(1,
     & 5,2,n)*x1y5z2+c(1,5,3,n)*x1y5z3+c(1,5,4,n)*x1y5z4+c(1,5,5,n)*
     & x1y5z5+c(2,0,0,n)*x2+c(2,0,1,n)*x2z1+c(2,0,2,n)*x2z2+c(2,0,3,n)
     & *x2z3+c(2,0,4,n)*x2z4+c(2,0,5,n)*x2z5+c(2,1,0,n)*x2y1+c(2,1,1,
     & n)*x2y1z1+c(2,1,2,n)*x2y1z2+c(2,1,3,n)*x2y1z3+c(2,1,4,n)*
     & x2y1z4+c(2,1,5,n)*x2y1z5+c(2,2,0,n)*x2y2+c(2,2,1,n)*x2y2z1+c(2,
     & 2,2,n)*x2y2z2+c(2,2,3,n)*x2y2z3+c(2,2,4,n)*x2y2z4+c(2,2,5,n)*
     & x2y2z5+c(2,3,0,n)*x2y3+c(2,3,1,n)*x2y3z1+c(2,3,2,n)*x2y3z2+c(2,
     & 3,3,n)*x2y3z3+c(2,3,4,n)*x2y3z4+c(2,3,5,n)*x2y3z5+c(2,4,0,n)*
     & x2y4+c(2,4,1,n)*x2y4z1+c(2,4,2,n)*x2y4z2+c(2,4,3,n)*x2y4z3+c(2,
     & 4,4,n)*x2y4z4+c(2,4,5,n)*x2y4z5+c(2,5,0,n)*x2y5+c(2,5,1,n)*
     & x2y5z1+c(2,5,2,n)*x2y5z2+c(2,5,3,n)*x2y5z3+c(2,5,4,n)*x2y5z4+c(
     & 2,5,5,n)*x2y5z5+c(3,0,0,n)*x3+c(3,0,1,n)*x3z1+c(3,0,2,n)*x3z2+
     & c(3,0,3,n)*x3z3+c(3,0,4,n)*x3z4+c(3,0,5,n)*x3z5+c(3,1,0,n)*
     & x3y1+c(3,1,1,n)*x3y1z1+c(3,1,2,n)*x3y1z2+c(3,1,3,n)*x3y1z3+c(3,
     & 1,4,n)*x3y1z4+c(3,1,5,n)*x3y1z5+c(3,2,0,n)*x3y2+c(3,2,1,n)*
     & x3y2z1+c(3,2,2,n)*x3y2z2+c(3,2,3,n)*x3y2z3+c(3,2,4,n)*x3y2z4+c(
     & 3,2,5,n)*x3y2z5+c(3,3,0,n)*x3y3+c(3,3,1,n)*x3y3z1+c(3,3,2,n)*
     & x3y3z2+c(3,3,3,n)*x3y3z3+c(3,3,4,n)*x3y3z4+c(3,3,5,n)*x3y3z5+c(
     & 3,4,0,n)*x3y4+c(3,4,1,n)*x3y4z1+c(3,4,2,n)*x3y4z2+c(3,4,3,n)*
     & x3y4z3+c(3,4,4,n)*x3y4z4+c(3,4,5,n)*x3y4z5+c(3,5,0,n)*x3y5+c(3,
     & 5,1,n)*x3y5z1+c(3,5,2,n)*x3y5z2+c(3,5,3,n)*x3y5z3+c(3,5,4,n)*
     & x3y5z4+c(3,5,5,n)*x3y5z5+c(4,0,0,n)*x4+c(4,0,1,n)*x4z1+c(4,0,2,
     & n)*x4z2+c(4,0,3,n)*x4z3+c(4,0,4,n)*x4z4+c(4,0,5,n)*x4z5+c(4,1,
     & 0,n)*x4y1+c(4,1,1,n)*x4y1z1+c(4,1,2,n)*x4y1z2+c(4,1,3,n)*
     & x4y1z3+c(4,1,4,n)*x4y1z4+c(4,1,5,n)*x4y1z5+c(4,2,0,n)*x4y2+c(4,
     & 2,1,n)*x4y2z1+c(4,2,2,n)*x4y2z2+c(4,2,3,n)*x4y2z3+c(4,2,4,n)*
     & x4y2z4+c(4,2,5,n)*x4y2z5+c(4,3,0,n)*x4y3+c(4,3,1,n)*x4y3z1+c(4,
     & 3,2,n)*x4y3z2+c(4,3,3,n)*x4y3z3+c(4,3,4,n)*x4y3z4+c(4,3,5,n)*
     & x4y3z5+c(4,4,0,n)*x4y4+c(4,4,1,n)*x4y4z1+c(4,4,2,n)*x4y4z2+c(4,
     & 4,3,n)*x4y4z3+c(4,4,4,n)*x4y4z4+c(4,4,5,n)*x4y4z5+c(4,5,0,n)*
     & x4y5+c(4,5,1,n)*x4y5z1+c(4,5,2,n)*x4y5z2+c(4,5,3,n)*x4y5z3+c(4,
     & 5,4,n)*x4y5z4+c(4,5,5,n)*x4y5z5+c(5,0,0,n)*x5+c(5,0,1,n)*x5z1+
     & c(5,0,2,n)*x5z2+c(5,0,3,n)*x5z3+c(5,0,4,n)*x5z4+c(5,0,5,n)*
     & x5z5+c(5,1,0,n)*x5y1+c(5,1,1,n)*x5y1z1+c(5,1,2,n)*x5y1z2+c(5,1,
     & 3,n)*x5y1z3+c(5,1,4,n)*x5y1z4+c(5,1,5,n)*x5y1z5+c(5,2,0,n)*
     & x5y2+c(5,2,1,n)*x5y2z1+c(5,2,2,n)*x5y2z2+c(5,2,3,n)*x5y2z3+c(5,
     & 2,4,n)*x5y2z4+c(5,2,5,n)*x5y2z5+c(5,3,0,n)*x5y3+c(5,3,1,n)*
     & x5y3z1+c(5,3,2,n)*x5y3z2+c(5,3,3,n)*x5y3z3+c(5,3,4,n)*x5y3z4+c(
     & 5,3,5,n)*x5y3z5+c(5,4,0,n)*x5y4+c(5,4,1,n)*x5y4z1+c(5,4,2,n)*
     & x5y4z2+c(5,4,3,n)*x5y4z3+c(5,4,4,n)*x5y4z4+c(5,4,5,n)*x5y4z5+c(
     & 5,5,0,n)*x5y5+c(5,5,1,n)*x5y5z1+c(5,5,2,n)*x5y5z2+c(5,5,3,n)*
     & x5y5z3+c(5,5,4,n)*x5y5z4+c(5,5,5,n)*x5y5z5)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.0.and.dy.eq.0.and.dz.eq.1 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      y3z4=y2z4*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y4z2=y3z2*y1
      y4z3=y3z3*y1
      y4z4=y3z4*y1
      y5=y4*y1
      y5z1=y4z1*y1
      y5z2=y4z2*y1
      y5z3=y4z3*y1
      y5z4=y4z4*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y2z4=x1y1z4*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x1y3z4=x1y2z4*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y4z2=x1y3z2*y1
      x1y4z3=x1y3z3*y1
      x1y4z4=x1y3z4*y1
      x1y5=x1y4*y1
      x1y5z1=x1y4z1*y1
      x1y5z2=x1y4z2*y1
      x1y5z3=x1y4z3*y1
      x1y5z4=x1y4z4*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y1z4=x1y1z4*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y2z4=x1y2z4*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y3z3=x1y3z3*x1
      x2y3z4=x1y3z4*x1
      x2y4=x1y4*x1
      x2y4z1=x1y4z1*x1
      x2y4z2=x1y4z2*x1
      x2y4z3=x1y4z3*x1
      x2y4z4=x1y4z4*x1
      x2y5=x1y5*x1
      x2y5z1=x1y5z1*x1
      x2y5z2=x1y5z2*x1
      x2y5z3=x1y5z3*x1
      x2y5z4=x1y5z4*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3z4=x2z4*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x3y1z4=x2y1z4*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y2z3=x2y2z3*x1
      x3y2z4=x2y2z4*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x3y3z2=x2y3z2*x1
      x3y3z3=x2y3z3*x1
      x3y3z4=x2y3z4*x1
      x3y4=x2y4*x1
      x3y4z1=x2y4z1*x1
      x3y4z2=x2y4z2*x1
      x3y4z3=x2y4z3*x1
      x3y4z4=x2y4z4*x1
      x3y5=x2y5*x1
      x3y5z1=x2y5z1*x1
      x3y5z2=x2y5z2*x1
      x3y5z3=x2y5z3*x1
      x3y5z4=x2y5z4*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4z2=x3z2*x1
      x4z3=x3z3*x1
      x4z4=x3z4*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y1z2=x3y1z2*x1
      x4y1z3=x3y1z3*x1
      x4y1z4=x3y1z4*x1
      x4y2=x3y2*x1
      x4y2z1=x3y2z1*x1
      x4y2z2=x3y2z2*x1
      x4y2z3=x3y2z3*x1
      x4y2z4=x3y2z4*x1
      x4y3=x3y3*x1
      x4y3z1=x3y3z1*x1
      x4y3z2=x3y3z2*x1
      x4y3z3=x3y3z3*x1
      x4y3z4=x3y3z4*x1
      x4y4=x3y4*x1
      x4y4z1=x3y4z1*x1
      x4y4z2=x3y4z2*x1
      x4y4z3=x3y4z3*x1
      x4y4z4=x3y4z4*x1
      x4y5=x3y5*x1
      x4y5z1=x3y5z1*x1
      x4y5z2=x3y5z2*x1
      x4y5z3=x3y5z3*x1
      x4y5z4=x3y5z4*x1
      x5=x4*x1
      x5z1=x4z1*x1
      x5z2=x4z2*x1
      x5z3=x4z3*x1
      x5z4=x4z4*x1
      x5y1=x4y1*x1
      x5y1z1=x4y1z1*x1
      x5y1z2=x4y1z2*x1
      x5y1z3=x4y1z3*x1
      x5y1z4=x4y1z4*x1
      x5y2=x4y2*x1
      x5y2z1=x4y2z1*x1
      x5y2z2=x4y2z2*x1
      x5y2z3=x4y2z3*x1
      x5y2z4=x4y2z4*x1
      x5y3=x4y3*x1
      x5y3z1=x4y3z1*x1
      x5y3z2=x4y3z2*x1
      x5y3z3=x4y3z3*x1
      x5y3z4=x4y3z4*x1
      x5y4=x4y4*x1
      x5y4z1=x4y4z1*x1
      x5y4z2=x4y4z2*x1
      x5y4z3=x4y4z3*x1
      x5y4z4=x4y4z4*x1
      x5y5=x4y5*x1
      x5y5z1=x4y5z1*x1
      x5y5z2=x4y5z2*x1
      x5y5z3=x4y5z3*x1
      x5y5z4=x4y5z4*x1
      r(i1,i2,i3,n)=(c(0,0,1,n)+c(0,0,2,n)*z1*2.+c(0,0,3,n)*z2*3.+c(0,
     & 0,4,n)*z3*4.+c(0,0,5,n)*z4*5.+c(0,1,1,n)*y1+c(0,1,2,n)*y1z1*2.+
     & c(0,1,3,n)*y1z2*3.+c(0,1,4,n)*y1z3*4.+c(0,1,5,n)*y1z4*5.+c(0,2,
     & 1,n)*y2+c(0,2,2,n)*y2z1*2.+c(0,2,3,n)*y2z2*3.+c(0,2,4,n)*y2z3*
     & 4.+c(0,2,5,n)*y2z4*5.+c(0,3,1,n)*y3+c(0,3,2,n)*y3z1*2.+c(0,3,3,
     & n)*y3z2*3.+c(0,3,4,n)*y3z3*4.+c(0,3,5,n)*y3z4*5.+c(0,4,1,n)*y4+
     & c(0,4,2,n)*y4z1*2.+c(0,4,3,n)*y4z2*3.+c(0,4,4,n)*y4z3*4.+c(0,4,
     & 5,n)*y4z4*5.+c(0,5,1,n)*y5+c(0,5,2,n)*y5z1*2.+c(0,5,3,n)*y5z2*
     & 3.+c(0,5,4,n)*y5z3*4.+c(0,5,5,n)*y5z4*5.+c(1,0,1,n)*x1+c(1,0,2,
     & n)*x1z1*2.+c(1,0,3,n)*x1z2*3.+c(1,0,4,n)*x1z3*4.+c(1,0,5,n)*
     & x1z4*5.+c(1,1,1,n)*x1y1+c(1,1,2,n)*x1y1z1*2.+c(1,1,3,n)*x1y1z2*
     & 3.+c(1,1,4,n)*x1y1z3*4.+c(1,1,5,n)*x1y1z4*5.+c(1,2,1,n)*x1y2+c(
     & 1,2,2,n)*x1y2z1*2.+c(1,2,3,n)*x1y2z2*3.+c(1,2,4,n)*x1y2z3*4.+c(
     & 1,2,5,n)*x1y2z4*5.+c(1,3,1,n)*x1y3+c(1,3,2,n)*x1y3z1*2.+c(1,3,
     & 3,n)*x1y3z2*3.+c(1,3,4,n)*x1y3z3*4.+c(1,3,5,n)*x1y3z4*5.+c(1,4,
     & 1,n)*x1y4+c(1,4,2,n)*x1y4z1*2.+c(1,4,3,n)*x1y4z2*3.+c(1,4,4,n)*
     & x1y4z3*4.+c(1,4,5,n)*x1y4z4*5.+c(1,5,1,n)*x1y5+c(1,5,2,n)*
     & x1y5z1*2.+c(1,5,3,n)*x1y5z2*3.+c(1,5,4,n)*x1y5z3*4.+c(1,5,5,n)*
     & x1y5z4*5.+c(2,0,1,n)*x2+c(2,0,2,n)*x2z1*2.+c(2,0,3,n)*x2z2*3.+
     & c(2,0,4,n)*x2z3*4.+c(2,0,5,n)*x2z4*5.+c(2,1,1,n)*x2y1+c(2,1,2,
     & n)*x2y1z1*2.+c(2,1,3,n)*x2y1z2*3.+c(2,1,4,n)*x2y1z3*4.+c(2,1,5,
     & n)*x2y1z4*5.+c(2,2,1,n)*x2y2+c(2,2,2,n)*x2y2z1*2.+c(2,2,3,n)*
     & x2y2z2*3.+c(2,2,4,n)*x2y2z3*4.+c(2,2,5,n)*x2y2z4*5.+c(2,3,1,n)*
     & x2y3+c(2,3,2,n)*x2y3z1*2.+c(2,3,3,n)*x2y3z2*3.+c(2,3,4,n)*
     & x2y3z3*4.+c(2,3,5,n)*x2y3z4*5.+c(2,4,1,n)*x2y4+c(2,4,2,n)*
     & x2y4z1*2.+c(2,4,3,n)*x2y4z2*3.+c(2,4,4,n)*x2y4z3*4.+c(2,4,5,n)*
     & x2y4z4*5.+c(2,5,1,n)*x2y5+c(2,5,2,n)*x2y5z1*2.+c(2,5,3,n)*
     & x2y5z2*3.+c(2,5,4,n)*x2y5z3*4.+c(2,5,5,n)*x2y5z4*5.+c(3,0,1,n)*
     & x3+c(3,0,2,n)*x3z1*2.+c(3,0,3,n)*x3z2*3.+c(3,0,4,n)*x3z3*4.+c(
     & 3,0,5,n)*x3z4*5.+c(3,1,1,n)*x3y1+c(3,1,2,n)*x3y1z1*2.+c(3,1,3,
     & n)*x3y1z2*3.+c(3,1,4,n)*x3y1z3*4.+c(3,1,5,n)*x3y1z4*5.+c(3,2,1,
     & n)*x3y2+c(3,2,2,n)*x3y2z1*2.+c(3,2,3,n)*x3y2z2*3.+c(3,2,4,n)*
     & x3y2z3*4.+c(3,2,5,n)*x3y2z4*5.+c(3,3,1,n)*x3y3+c(3,3,2,n)*
     & x3y3z1*2.+c(3,3,3,n)*x3y3z2*3.+c(3,3,4,n)*x3y3z3*4.+c(3,3,5,n)*
     & x3y3z4*5.+c(3,4,1,n)*x3y4+c(3,4,2,n)*x3y4z1*2.+c(3,4,3,n)*
     & x3y4z2*3.+c(3,4,4,n)*x3y4z3*4.+c(3,4,5,n)*x3y4z4*5.+c(3,5,1,n)*
     & x3y5+c(3,5,2,n)*x3y5z1*2.+c(3,5,3,n)*x3y5z2*3.+c(3,5,4,n)*
     & x3y5z3*4.+c(3,5,5,n)*x3y5z4*5.+c(4,0,1,n)*x4+c(4,0,2,n)*x4z1*
     & 2.+c(4,0,3,n)*x4z2*3.+c(4,0,4,n)*x4z3*4.+c(4,0,5,n)*x4z4*5.+c(
     & 4,1,1,n)*x4y1+c(4,1,2,n)*x4y1z1*2.+c(4,1,3,n)*x4y1z2*3.+c(4,1,
     & 4,n)*x4y1z3*4.+c(4,1,5,n)*x4y1z4*5.+c(4,2,1,n)*x4y2+c(4,2,2,n)*
     & x4y2z1*2.+c(4,2,3,n)*x4y2z2*3.+c(4,2,4,n)*x4y2z3*4.+c(4,2,5,n)*
     & x4y2z4*5.+c(4,3,1,n)*x4y3+c(4,3,2,n)*x4y3z1*2.+c(4,3,3,n)*
     & x4y3z2*3.+c(4,3,4,n)*x4y3z3*4.+c(4,3,5,n)*x4y3z4*5.+c(4,4,1,n)*
     & x4y4+c(4,4,2,n)*x4y4z1*2.+c(4,4,3,n)*x4y4z2*3.+c(4,4,4,n)*
     & x4y4z3*4.+c(4,4,5,n)*x4y4z4*5.+c(4,5,1,n)*x4y5+c(4,5,2,n)*
     & x4y5z1*2.+c(4,5,3,n)*x4y5z2*3.+c(4,5,4,n)*x4y5z3*4.+c(4,5,5,n)*
     & x4y5z4*5.+c(5,0,1,n)*x5+c(5,0,2,n)*x5z1*2.+c(5,0,3,n)*x5z2*3.+
     & c(5,0,4,n)*x5z3*4.+c(5,0,5,n)*x5z4*5.+c(5,1,1,n)*x5y1+c(5,1,2,
     & n)*x5y1z1*2.+c(5,1,3,n)*x5y1z2*3.+c(5,1,4,n)*x5y1z3*4.+c(5,1,5,
     & n)*x5y1z4*5.+c(5,2,1,n)*x5y2+c(5,2,2,n)*x5y2z1*2.+c(5,2,3,n)*
     & x5y2z2*3.+c(5,2,4,n)*x5y2z3*4.+c(5,2,5,n)*x5y2z4*5.+c(5,3,1,n)*
     & x5y3+c(5,3,2,n)*x5y3z1*2.+c(5,3,3,n)*x5y3z2*3.+c(5,3,4,n)*
     & x5y3z3*4.+c(5,3,5,n)*x5y3z4*5.+c(5,4,1,n)*x5y4+c(5,4,2,n)*
     & x5y4z1*2.+c(5,4,3,n)*x5y4z2*3.+c(5,4,4,n)*x5y4z3*4.+c(5,4,5,n)*
     & x5y4z4*5.+c(5,5,1,n)*x5y5+c(5,5,2,n)*x5y5z1*2.+c(5,5,3,n)*
     & x5y5z2*3.+c(5,5,4,n)*x5y5z3*4.+c(5,5,5,n)*x5y5z4*5.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.0.and.dy.eq.0.and.dz.eq.2 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y4z2=y3z2*y1
      y4z3=y3z3*y1
      y5=y4*y1
      y5z1=y4z1*y1
      y5z2=y4z2*y1
      y5z3=y4z3*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y4z2=x1y3z2*y1
      x1y4z3=x1y3z3*y1
      x1y5=x1y4*y1
      x1y5z1=x1y4z1*y1
      x1y5z2=x1y4z2*y1
      x1y5z3=x1y4z3*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y3z3=x1y3z3*x1
      x2y4=x1y4*x1
      x2y4z1=x1y4z1*x1
      x2y4z2=x1y4z2*x1
      x2y4z3=x1y4z3*x1
      x2y5=x1y5*x1
      x2y5z1=x1y5z1*x1
      x2y5z2=x1y5z2*x1
      x2y5z3=x1y5z3*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y2z3=x2y2z3*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x3y3z2=x2y3z2*x1
      x3y3z3=x2y3z3*x1
      x3y4=x2y4*x1
      x3y4z1=x2y4z1*x1
      x3y4z2=x2y4z2*x1
      x3y4z3=x2y4z3*x1
      x3y5=x2y5*x1
      x3y5z1=x2y5z1*x1
      x3y5z2=x2y5z2*x1
      x3y5z3=x2y5z3*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4z2=x3z2*x1
      x4z3=x3z3*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y1z2=x3y1z2*x1
      x4y1z3=x3y1z3*x1
      x4y2=x3y2*x1
      x4y2z1=x3y2z1*x1
      x4y2z2=x3y2z2*x1
      x4y2z3=x3y2z3*x1
      x4y3=x3y3*x1
      x4y3z1=x3y3z1*x1
      x4y3z2=x3y3z2*x1
      x4y3z3=x3y3z3*x1
      x4y4=x3y4*x1
      x4y4z1=x3y4z1*x1
      x4y4z2=x3y4z2*x1
      x4y4z3=x3y4z3*x1
      x4y5=x3y5*x1
      x4y5z1=x3y5z1*x1
      x4y5z2=x3y5z2*x1
      x4y5z3=x3y5z3*x1
      x5=x4*x1
      x5z1=x4z1*x1
      x5z2=x4z2*x1
      x5z3=x4z3*x1
      x5y1=x4y1*x1
      x5y1z1=x4y1z1*x1
      x5y1z2=x4y1z2*x1
      x5y1z3=x4y1z3*x1
      x5y2=x4y2*x1
      x5y2z1=x4y2z1*x1
      x5y2z2=x4y2z2*x1
      x5y2z3=x4y2z3*x1
      x5y3=x4y3*x1
      x5y3z1=x4y3z1*x1
      x5y3z2=x4y3z2*x1
      x5y3z3=x4y3z3*x1
      x5y4=x4y4*x1
      x5y4z1=x4y4z1*x1
      x5y4z2=x4y4z2*x1
      x5y4z3=x4y4z3*x1
      x5y5=x4y5*x1
      x5y5z1=x4y5z1*x1
      x5y5z2=x4y5z2*x1
      x5y5z3=x4y5z3*x1
      r(i1,i2,i3,n)=(c(0,0,2,n)*2.+c(0,0,3,n)*z1*6.+c(0,0,4,n)*z2*12.+
     & c(0,0,5,n)*z3*20.+c(0,1,2,n)*y1*2.+c(0,1,3,n)*y1z1*6.+c(0,1,4,
     & n)*y1z2*12.+c(0,1,5,n)*y1z3*20.+c(0,2,2,n)*y2*2.+c(0,2,3,n)*
     & y2z1*6.+c(0,2,4,n)*y2z2*12.+c(0,2,5,n)*y2z3*20.+c(0,3,2,n)*y3*
     & 2.+c(0,3,3,n)*y3z1*6.+c(0,3,4,n)*y3z2*12.+c(0,3,5,n)*y3z3*20.+
     & c(0,4,2,n)*y4*2.+c(0,4,3,n)*y4z1*6.+c(0,4,4,n)*y4z2*12.+c(0,4,
     & 5,n)*y4z3*20.+c(0,5,2,n)*y5*2.+c(0,5,3,n)*y5z1*6.+c(0,5,4,n)*
     & y5z2*12.+c(0,5,5,n)*y5z3*20.+c(1,0,2,n)*x1*2.+c(1,0,3,n)*x1z1*
     & 6.+c(1,0,4,n)*x1z2*12.+c(1,0,5,n)*x1z3*20.+c(1,1,2,n)*x1y1*2.+
     & c(1,1,3,n)*x1y1z1*6.+c(1,1,4,n)*x1y1z2*12.+c(1,1,5,n)*x1y1z3*
     & 20.+c(1,2,2,n)*x1y2*2.+c(1,2,3,n)*x1y2z1*6.+c(1,2,4,n)*x1y2z2*
     & 12.+c(1,2,5,n)*x1y2z3*20.+c(1,3,2,n)*x1y3*2.+c(1,3,3,n)*x1y3z1*
     & 6.+c(1,3,4,n)*x1y3z2*12.+c(1,3,5,n)*x1y3z3*20.+c(1,4,2,n)*x1y4*
     & 2.+c(1,4,3,n)*x1y4z1*6.+c(1,4,4,n)*x1y4z2*12.+c(1,4,5,n)*
     & x1y4z3*20.+c(1,5,2,n)*x1y5*2.+c(1,5,3,n)*x1y5z1*6.+c(1,5,4,n)*
     & x1y5z2*12.+c(1,5,5,n)*x1y5z3*20.+c(2,0,2,n)*x2*2.+c(2,0,3,n)*
     & x2z1*6.+c(2,0,4,n)*x2z2*12.+c(2,0,5,n)*x2z3*20.+c(2,1,2,n)*
     & x2y1*2.+c(2,1,3,n)*x2y1z1*6.+c(2,1,4,n)*x2y1z2*12.+c(2,1,5,n)*
     & x2y1z3*20.+c(2,2,2,n)*x2y2*2.+c(2,2,3,n)*x2y2z1*6.+c(2,2,4,n)*
     & x2y2z2*12.+c(2,2,5,n)*x2y2z3*20.+c(2,3,2,n)*x2y3*2.+c(2,3,3,n)*
     & x2y3z1*6.+c(2,3,4,n)*x2y3z2*12.+c(2,3,5,n)*x2y3z3*20.+c(2,4,2,
     & n)*x2y4*2.+c(2,4,3,n)*x2y4z1*6.+c(2,4,4,n)*x2y4z2*12.+c(2,4,5,
     & n)*x2y4z3*20.+c(2,5,2,n)*x2y5*2.+c(2,5,3,n)*x2y5z1*6.+c(2,5,4,
     & n)*x2y5z2*12.+c(2,5,5,n)*x2y5z3*20.+c(3,0,2,n)*x3*2.+c(3,0,3,n)
     & *x3z1*6.+c(3,0,4,n)*x3z2*12.+c(3,0,5,n)*x3z3*20.+c(3,1,2,n)*
     & x3y1*2.+c(3,1,3,n)*x3y1z1*6.+c(3,1,4,n)*x3y1z2*12.+c(3,1,5,n)*
     & x3y1z3*20.+c(3,2,2,n)*x3y2*2.+c(3,2,3,n)*x3y2z1*6.+c(3,2,4,n)*
     & x3y2z2*12.+c(3,2,5,n)*x3y2z3*20.+c(3,3,2,n)*x3y3*2.+c(3,3,3,n)*
     & x3y3z1*6.+c(3,3,4,n)*x3y3z2*12.+c(3,3,5,n)*x3y3z3*20.+c(3,4,2,
     & n)*x3y4*2.+c(3,4,3,n)*x3y4z1*6.+c(3,4,4,n)*x3y4z2*12.+c(3,4,5,
     & n)*x3y4z3*20.+c(3,5,2,n)*x3y5*2.+c(3,5,3,n)*x3y5z1*6.+c(3,5,4,
     & n)*x3y5z2*12.+c(3,5,5,n)*x3y5z3*20.+c(4,0,2,n)*x4*2.+c(4,0,3,n)
     & *x4z1*6.+c(4,0,4,n)*x4z2*12.+c(4,0,5,n)*x4z3*20.+c(4,1,2,n)*
     & x4y1*2.+c(4,1,3,n)*x4y1z1*6.+c(4,1,4,n)*x4y1z2*12.+c(4,1,5,n)*
     & x4y1z3*20.+c(4,2,2,n)*x4y2*2.+c(4,2,3,n)*x4y2z1*6.+c(4,2,4,n)*
     & x4y2z2*12.+c(4,2,5,n)*x4y2z3*20.+c(4,3,2,n)*x4y3*2.+c(4,3,3,n)*
     & x4y3z1*6.+c(4,3,4,n)*x4y3z2*12.+c(4,3,5,n)*x4y3z3*20.+c(4,4,2,
     & n)*x4y4*2.+c(4,4,3,n)*x4y4z1*6.+c(4,4,4,n)*x4y4z2*12.+c(4,4,5,
     & n)*x4y4z3*20.+c(4,5,2,n)*x4y5*2.+c(4,5,3,n)*x4y5z1*6.+c(4,5,4,
     & n)*x4y5z2*12.+c(4,5,5,n)*x4y5z3*20.+c(5,0,2,n)*x5*2.+c(5,0,3,n)
     & *x5z1*6.+c(5,0,4,n)*x5z2*12.+c(5,0,5,n)*x5z3*20.+c(5,1,2,n)*
     & x5y1*2.+c(5,1,3,n)*x5y1z1*6.+c(5,1,4,n)*x5y1z2*12.+c(5,1,5,n)*
     & x5y1z3*20.+c(5,2,2,n)*x5y2*2.+c(5,2,3,n)*x5y2z1*6.+c(5,2,4,n)*
     & x5y2z2*12.+c(5,2,5,n)*x5y2z3*20.+c(5,3,2,n)*x5y3*2.+c(5,3,3,n)*
     & x5y3z1*6.+c(5,3,4,n)*x5y3z2*12.+c(5,3,5,n)*x5y3z3*20.+c(5,4,2,
     & n)*x5y4*2.+c(5,4,3,n)*x5y4z1*6.+c(5,4,4,n)*x5y4z2*12.+c(5,4,5,
     & n)*x5y4z3*20.+c(5,5,2,n)*x5y5*2.+c(5,5,3,n)*x5y5z1*6.+c(5,5,4,
     & n)*x5y5z2*12.+c(5,5,5,n)*x5y5z3*20.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.0.and.dy.eq.0.and.dz.eq.3 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y4z2=y3z2*y1
      y5=y4*y1
      y5z1=y4z1*y1
      y5z2=y4z2*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y4z2=x1y3z2*y1
      x1y5=x1y4*y1
      x1y5z1=x1y4z1*y1
      x1y5z2=x1y4z2*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y4=x1y4*x1
      x2y4z1=x1y4z1*x1
      x2y4z2=x1y4z2*x1
      x2y5=x1y5*x1
      x2y5z1=x1y5z1*x1
      x2y5z2=x1y5z2*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x3y3z2=x2y3z2*x1
      x3y4=x2y4*x1
      x3y4z1=x2y4z1*x1
      x3y4z2=x2y4z2*x1
      x3y5=x2y5*x1
      x3y5z1=x2y5z1*x1
      x3y5z2=x2y5z2*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4z2=x3z2*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y1z2=x3y1z2*x1
      x4y2=x3y2*x1
      x4y2z1=x3y2z1*x1
      x4y2z2=x3y2z2*x1
      x4y3=x3y3*x1
      x4y3z1=x3y3z1*x1
      x4y3z2=x3y3z2*x1
      x4y4=x3y4*x1
      x4y4z1=x3y4z1*x1
      x4y4z2=x3y4z2*x1
      x4y5=x3y5*x1
      x4y5z1=x3y5z1*x1
      x4y5z2=x3y5z2*x1
      x5=x4*x1
      x5z1=x4z1*x1
      x5z2=x4z2*x1
      x5y1=x4y1*x1
      x5y1z1=x4y1z1*x1
      x5y1z2=x4y1z2*x1
      x5y2=x4y2*x1
      x5y2z1=x4y2z1*x1
      x5y2z2=x4y2z2*x1
      x5y3=x4y3*x1
      x5y3z1=x4y3z1*x1
      x5y3z2=x4y3z2*x1
      x5y4=x4y4*x1
      x5y4z1=x4y4z1*x1
      x5y4z2=x4y4z2*x1
      x5y5=x4y5*x1
      x5y5z1=x4y5z1*x1
      x5y5z2=x4y5z2*x1
      r(i1,i2,i3,n)=(c(0,0,3,n)*6.+c(0,0,4,n)*z1*24.+c(0,0,5,n)*z2*60.+
     & c(0,1,3,n)*y1*6.+c(0,1,4,n)*y1z1*24.+c(0,1,5,n)*y1z2*60.+c(0,2,
     & 3,n)*y2*6.+c(0,2,4,n)*y2z1*24.+c(0,2,5,n)*y2z2*60.+c(0,3,3,n)*
     & y3*6.+c(0,3,4,n)*y3z1*24.+c(0,3,5,n)*y3z2*60.+c(0,4,3,n)*y4*6.+
     & c(0,4,4,n)*y4z1*24.+c(0,4,5,n)*y4z2*60.+c(0,5,3,n)*y5*6.+c(0,5,
     & 4,n)*y5z1*24.+c(0,5,5,n)*y5z2*60.+c(1,0,3,n)*x1*6.+c(1,0,4,n)*
     & x1z1*24.+c(1,0,5,n)*x1z2*60.+c(1,1,3,n)*x1y1*6.+c(1,1,4,n)*
     & x1y1z1*24.+c(1,1,5,n)*x1y1z2*60.+c(1,2,3,n)*x1y2*6.+c(1,2,4,n)*
     & x1y2z1*24.+c(1,2,5,n)*x1y2z2*60.+c(1,3,3,n)*x1y3*6.+c(1,3,4,n)*
     & x1y3z1*24.+c(1,3,5,n)*x1y3z2*60.+c(1,4,3,n)*x1y4*6.+c(1,4,4,n)*
     & x1y4z1*24.+c(1,4,5,n)*x1y4z2*60.+c(1,5,3,n)*x1y5*6.+c(1,5,4,n)*
     & x1y5z1*24.+c(1,5,5,n)*x1y5z2*60.+c(2,0,3,n)*x2*6.+c(2,0,4,n)*
     & x2z1*24.+c(2,0,5,n)*x2z2*60.+c(2,1,3,n)*x2y1*6.+c(2,1,4,n)*
     & x2y1z1*24.+c(2,1,5,n)*x2y1z2*60.+c(2,2,3,n)*x2y2*6.+c(2,2,4,n)*
     & x2y2z1*24.+c(2,2,5,n)*x2y2z2*60.+c(2,3,3,n)*x2y3*6.+c(2,3,4,n)*
     & x2y3z1*24.+c(2,3,5,n)*x2y3z2*60.+c(2,4,3,n)*x2y4*6.+c(2,4,4,n)*
     & x2y4z1*24.+c(2,4,5,n)*x2y4z2*60.+c(2,5,3,n)*x2y5*6.+c(2,5,4,n)*
     & x2y5z1*24.+c(2,5,5,n)*x2y5z2*60.+c(3,0,3,n)*x3*6.+c(3,0,4,n)*
     & x3z1*24.+c(3,0,5,n)*x3z2*60.+c(3,1,3,n)*x3y1*6.+c(3,1,4,n)*
     & x3y1z1*24.+c(3,1,5,n)*x3y1z2*60.+c(3,2,3,n)*x3y2*6.+c(3,2,4,n)*
     & x3y2z1*24.+c(3,2,5,n)*x3y2z2*60.+c(3,3,3,n)*x3y3*6.+c(3,3,4,n)*
     & x3y3z1*24.+c(3,3,5,n)*x3y3z2*60.+c(3,4,3,n)*x3y4*6.+c(3,4,4,n)*
     & x3y4z1*24.+c(3,4,5,n)*x3y4z2*60.+c(3,5,3,n)*x3y5*6.+c(3,5,4,n)*
     & x3y5z1*24.+c(3,5,5,n)*x3y5z2*60.+c(4,0,3,n)*x4*6.+c(4,0,4,n)*
     & x4z1*24.+c(4,0,5,n)*x4z2*60.+c(4,1,3,n)*x4y1*6.+c(4,1,4,n)*
     & x4y1z1*24.+c(4,1,5,n)*x4y1z2*60.+c(4,2,3,n)*x4y2*6.+c(4,2,4,n)*
     & x4y2z1*24.+c(4,2,5,n)*x4y2z2*60.+c(4,3,3,n)*x4y3*6.+c(4,3,4,n)*
     & x4y3z1*24.+c(4,3,5,n)*x4y3z2*60.+c(4,4,3,n)*x4y4*6.+c(4,4,4,n)*
     & x4y4z1*24.+c(4,4,5,n)*x4y4z2*60.+c(4,5,3,n)*x4y5*6.+c(4,5,4,n)*
     & x4y5z1*24.+c(4,5,5,n)*x4y5z2*60.+c(5,0,3,n)*x5*6.+c(5,0,4,n)*
     & x5z1*24.+c(5,0,5,n)*x5z2*60.+c(5,1,3,n)*x5y1*6.+c(5,1,4,n)*
     & x5y1z1*24.+c(5,1,5,n)*x5y1z2*60.+c(5,2,3,n)*x5y2*6.+c(5,2,4,n)*
     & x5y2z1*24.+c(5,2,5,n)*x5y2z2*60.+c(5,3,3,n)*x5y3*6.+c(5,3,4,n)*
     & x5y3z1*24.+c(5,3,5,n)*x5y3z2*60.+c(5,4,3,n)*x5y4*6.+c(5,4,4,n)*
     & x5y4z1*24.+c(5,4,5,n)*x5y4z2*60.+c(5,5,3,n)*x5y5*6.+c(5,5,4,n)*
     & x5y5z1*24.+c(5,5,5,n)*x5y5z2*60.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.0.and.dy.eq.0.and.dz.eq.4 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y5=y4*y1
      y5z1=y4z1*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y5=x1y4*y1
      x1y5z1=x1y4z1*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y4=x1y4*x1
      x2y4z1=x1y4z1*x1
      x2y5=x1y5*x1
      x2y5z1=x1y5z1*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x3y4=x2y4*x1
      x3y4z1=x2y4z1*x1
      x3y5=x2y5*x1
      x3y5z1=x2y5z1*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y2=x3y2*x1
      x4y2z1=x3y2z1*x1
      x4y3=x3y3*x1
      x4y3z1=x3y3z1*x1
      x4y4=x3y4*x1
      x4y4z1=x3y4z1*x1
      x4y5=x3y5*x1
      x4y5z1=x3y5z1*x1
      x5=x4*x1
      x5z1=x4z1*x1
      x5y1=x4y1*x1
      x5y1z1=x4y1z1*x1
      x5y2=x4y2*x1
      x5y2z1=x4y2z1*x1
      x5y3=x4y3*x1
      x5y3z1=x4y3z1*x1
      x5y4=x4y4*x1
      x5y4z1=x4y4z1*x1
      x5y5=x4y5*x1
      x5y5z1=x4y5z1*x1
      r(i1,i2,i3,n)=(c(0,0,4,n)*24.+c(0,0,5,n)*z1*120.+c(0,1,4,n)*y1*
     & 24.+c(0,1,5,n)*y1z1*120.+c(0,2,4,n)*y2*24.+c(0,2,5,n)*y2z1*
     & 120.+c(0,3,4,n)*y3*24.+c(0,3,5,n)*y3z1*120.+c(0,4,4,n)*y4*24.+
     & c(0,4,5,n)*y4z1*120.+c(0,5,4,n)*y5*24.+c(0,5,5,n)*y5z1*120.+c(
     & 1,0,4,n)*x1*24.+c(1,0,5,n)*x1z1*120.+c(1,1,4,n)*x1y1*24.+c(1,1,
     & 5,n)*x1y1z1*120.+c(1,2,4,n)*x1y2*24.+c(1,2,5,n)*x1y2z1*120.+c(
     & 1,3,4,n)*x1y3*24.+c(1,3,5,n)*x1y3z1*120.+c(1,4,4,n)*x1y4*24.+c(
     & 1,4,5,n)*x1y4z1*120.+c(1,5,4,n)*x1y5*24.+c(1,5,5,n)*x1y5z1*
     & 120.+c(2,0,4,n)*x2*24.+c(2,0,5,n)*x2z1*120.+c(2,1,4,n)*x2y1*
     & 24.+c(2,1,5,n)*x2y1z1*120.+c(2,2,4,n)*x2y2*24.+c(2,2,5,n)*
     & x2y2z1*120.+c(2,3,4,n)*x2y3*24.+c(2,3,5,n)*x2y3z1*120.+c(2,4,4,
     & n)*x2y4*24.+c(2,4,5,n)*x2y4z1*120.+c(2,5,4,n)*x2y5*24.+c(2,5,5,
     & n)*x2y5z1*120.+c(3,0,4,n)*x3*24.+c(3,0,5,n)*x3z1*120.+c(3,1,4,
     & n)*x3y1*24.+c(3,1,5,n)*x3y1z1*120.+c(3,2,4,n)*x3y2*24.+c(3,2,5,
     & n)*x3y2z1*120.+c(3,3,4,n)*x3y3*24.+c(3,3,5,n)*x3y3z1*120.+c(3,
     & 4,4,n)*x3y4*24.+c(3,4,5,n)*x3y4z1*120.+c(3,5,4,n)*x3y5*24.+c(3,
     & 5,5,n)*x3y5z1*120.+c(4,0,4,n)*x4*24.+c(4,0,5,n)*x4z1*120.+c(4,
     & 1,4,n)*x4y1*24.+c(4,1,5,n)*x4y1z1*120.+c(4,2,4,n)*x4y2*24.+c(4,
     & 2,5,n)*x4y2z1*120.+c(4,3,4,n)*x4y3*24.+c(4,3,5,n)*x4y3z1*120.+
     & c(4,4,4,n)*x4y4*24.+c(4,4,5,n)*x4y4z1*120.+c(4,5,4,n)*x4y5*24.+
     & c(4,5,5,n)*x4y5z1*120.+c(5,0,4,n)*x5*24.+c(5,0,5,n)*x5z1*120.+
     & c(5,1,4,n)*x5y1*24.+c(5,1,5,n)*x5y1z1*120.+c(5,2,4,n)*x5y2*24.+
     & c(5,2,5,n)*x5y2z1*120.+c(5,3,4,n)*x5y3*24.+c(5,3,5,n)*x5y3z1*
     & 120.+c(5,4,4,n)*x5y4*24.+c(5,4,5,n)*x5y4z1*120.+c(5,5,4,n)*
     & x5y5*24.+c(5,5,5,n)*x5y5z1*120.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.0.and.dy.eq.1.and.dz.eq.0 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y1z5=y1z4*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
      y2z5=y1z5*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      y3z4=y2z4*y1
      y3z5=y2z5*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y4z2=y3z2*y1
      y4z3=y3z3*y1
      y4z4=y3z4*y1
      y4z5=y3z5*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1z5=x1z4*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y1z5=x1y1z4*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y2z4=x1y1z4*y1
      x1y2z5=x1y1z5*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x1y3z4=x1y2z4*y1
      x1y3z5=x1y2z5*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y4z2=x1y3z2*y1
      x1y4z3=x1y3z3*y1
      x1y4z4=x1y3z4*y1
      x1y4z5=x1y3z5*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x2z5=x1z5*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y1z4=x1y1z4*x1
      x2y1z5=x1y1z5*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y2z4=x1y2z4*x1
      x2y2z5=x1y2z5*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y3z3=x1y3z3*x1
      x2y3z4=x1y3z4*x1
      x2y3z5=x1y3z5*x1
      x2y4=x1y4*x1
      x2y4z1=x1y4z1*x1
      x2y4z2=x1y4z2*x1
      x2y4z3=x1y4z3*x1
      x2y4z4=x1y4z4*x1
      x2y4z5=x1y4z5*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3z4=x2z4*x1
      x3z5=x2z5*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x3y1z4=x2y1z4*x1
      x3y1z5=x2y1z5*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y2z3=x2y2z3*x1
      x3y2z4=x2y2z4*x1
      x3y2z5=x2y2z5*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x3y3z2=x2y3z2*x1
      x3y3z3=x2y3z3*x1
      x3y3z4=x2y3z4*x1
      x3y3z5=x2y3z5*x1
      x3y4=x2y4*x1
      x3y4z1=x2y4z1*x1
      x3y4z2=x2y4z2*x1
      x3y4z3=x2y4z3*x1
      x3y4z4=x2y4z4*x1
      x3y4z5=x2y4z5*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4z2=x3z2*x1
      x4z3=x3z3*x1
      x4z4=x3z4*x1
      x4z5=x3z5*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y1z2=x3y1z2*x1
      x4y1z3=x3y1z3*x1
      x4y1z4=x3y1z4*x1
      x4y1z5=x3y1z5*x1
      x4y2=x3y2*x1
      x4y2z1=x3y2z1*x1
      x4y2z2=x3y2z2*x1
      x4y2z3=x3y2z3*x1
      x4y2z4=x3y2z4*x1
      x4y2z5=x3y2z5*x1
      x4y3=x3y3*x1
      x4y3z1=x3y3z1*x1
      x4y3z2=x3y3z2*x1
      x4y3z3=x3y3z3*x1
      x4y3z4=x3y3z4*x1
      x4y3z5=x3y3z5*x1
      x4y4=x3y4*x1
      x4y4z1=x3y4z1*x1
      x4y4z2=x3y4z2*x1
      x4y4z3=x3y4z3*x1
      x4y4z4=x3y4z4*x1
      x4y4z5=x3y4z5*x1
      x5=x4*x1
      x5z1=x4z1*x1
      x5z2=x4z2*x1
      x5z3=x4z3*x1
      x5z4=x4z4*x1
      x5z5=x4z5*x1
      x5y1=x4y1*x1
      x5y1z1=x4y1z1*x1
      x5y1z2=x4y1z2*x1
      x5y1z3=x4y1z3*x1
      x5y1z4=x4y1z4*x1
      x5y1z5=x4y1z5*x1
      x5y2=x4y2*x1
      x5y2z1=x4y2z1*x1
      x5y2z2=x4y2z2*x1
      x5y2z3=x4y2z3*x1
      x5y2z4=x4y2z4*x1
      x5y2z5=x4y2z5*x1
      x5y3=x4y3*x1
      x5y3z1=x4y3z1*x1
      x5y3z2=x4y3z2*x1
      x5y3z3=x4y3z3*x1
      x5y3z4=x4y3z4*x1
      x5y3z5=x4y3z5*x1
      x5y4=x4y4*x1
      x5y4z1=x4y4z1*x1
      x5y4z2=x4y4z2*x1
      x5y4z3=x4y4z3*x1
      x5y4z4=x4y4z4*x1
      x5y4z5=x4y4z5*x1
      r(i1,i2,i3,n)=(c(0,1,0,n)+c(0,1,1,n)*z1+c(0,1,2,n)*z2+c(0,1,3,n)*
     & z3+c(0,1,4,n)*z4+c(0,1,5,n)*z5+c(0,2,0,n)*y1*2.+c(0,2,1,n)*
     & y1z1*2.+c(0,2,2,n)*y1z2*2.+c(0,2,3,n)*y1z3*2.+c(0,2,4,n)*y1z4*
     & 2.+c(0,2,5,n)*y1z5*2.+c(0,3,0,n)*y2*3.+c(0,3,1,n)*y2z1*3.+c(0,
     & 3,2,n)*y2z2*3.+c(0,3,3,n)*y2z3*3.+c(0,3,4,n)*y2z4*3.+c(0,3,5,n)
     & *y2z5*3.+c(0,4,0,n)*y3*4.+c(0,4,1,n)*y3z1*4.+c(0,4,2,n)*y3z2*
     & 4.+c(0,4,3,n)*y3z3*4.+c(0,4,4,n)*y3z4*4.+c(0,4,5,n)*y3z5*4.+c(
     & 0,5,0,n)*y4*5.+c(0,5,1,n)*y4z1*5.+c(0,5,2,n)*y4z2*5.+c(0,5,3,n)
     & *y4z3*5.+c(0,5,4,n)*y4z4*5.+c(0,5,5,n)*y4z5*5.+c(1,1,0,n)*x1+c(
     & 1,1,1,n)*x1z1+c(1,1,2,n)*x1z2+c(1,1,3,n)*x1z3+c(1,1,4,n)*x1z4+
     & c(1,1,5,n)*x1z5+c(1,2,0,n)*x1y1*2.+c(1,2,1,n)*x1y1z1*2.+c(1,2,
     & 2,n)*x1y1z2*2.+c(1,2,3,n)*x1y1z3*2.+c(1,2,4,n)*x1y1z4*2.+c(1,2,
     & 5,n)*x1y1z5*2.+c(1,3,0,n)*x1y2*3.+c(1,3,1,n)*x1y2z1*3.+c(1,3,2,
     & n)*x1y2z2*3.+c(1,3,3,n)*x1y2z3*3.+c(1,3,4,n)*x1y2z4*3.+c(1,3,5,
     & n)*x1y2z5*3.+c(1,4,0,n)*x1y3*4.+c(1,4,1,n)*x1y3z1*4.+c(1,4,2,n)
     & *x1y3z2*4.+c(1,4,3,n)*x1y3z3*4.+c(1,4,4,n)*x1y3z4*4.+c(1,4,5,n)
     & *x1y3z5*4.+c(1,5,0,n)*x1y4*5.+c(1,5,1,n)*x1y4z1*5.+c(1,5,2,n)*
     & x1y4z2*5.+c(1,5,3,n)*x1y4z3*5.+c(1,5,4,n)*x1y4z4*5.+c(1,5,5,n)*
     & x1y4z5*5.+c(2,1,0,n)*x2+c(2,1,1,n)*x2z1+c(2,1,2,n)*x2z2+c(2,1,
     & 3,n)*x2z3+c(2,1,4,n)*x2z4+c(2,1,5,n)*x2z5+c(2,2,0,n)*x2y1*2.+c(
     & 2,2,1,n)*x2y1z1*2.+c(2,2,2,n)*x2y1z2*2.+c(2,2,3,n)*x2y1z3*2.+c(
     & 2,2,4,n)*x2y1z4*2.+c(2,2,5,n)*x2y1z5*2.+c(2,3,0,n)*x2y2*3.+c(2,
     & 3,1,n)*x2y2z1*3.+c(2,3,2,n)*x2y2z2*3.+c(2,3,3,n)*x2y2z3*3.+c(2,
     & 3,4,n)*x2y2z4*3.+c(2,3,5,n)*x2y2z5*3.+c(2,4,0,n)*x2y3*4.+c(2,4,
     & 1,n)*x2y3z1*4.+c(2,4,2,n)*x2y3z2*4.+c(2,4,3,n)*x2y3z3*4.+c(2,4,
     & 4,n)*x2y3z4*4.+c(2,4,5,n)*x2y3z5*4.+c(2,5,0,n)*x2y4*5.+c(2,5,1,
     & n)*x2y4z1*5.+c(2,5,2,n)*x2y4z2*5.+c(2,5,3,n)*x2y4z3*5.+c(2,5,4,
     & n)*x2y4z4*5.+c(2,5,5,n)*x2y4z5*5.+c(3,1,0,n)*x3+c(3,1,1,n)*
     & x3z1+c(3,1,2,n)*x3z2+c(3,1,3,n)*x3z3+c(3,1,4,n)*x3z4+c(3,1,5,n)
     & *x3z5+c(3,2,0,n)*x3y1*2.+c(3,2,1,n)*x3y1z1*2.+c(3,2,2,n)*
     & x3y1z2*2.+c(3,2,3,n)*x3y1z3*2.+c(3,2,4,n)*x3y1z4*2.+c(3,2,5,n)*
     & x3y1z5*2.+c(3,3,0,n)*x3y2*3.+c(3,3,1,n)*x3y2z1*3.+c(3,3,2,n)*
     & x3y2z2*3.+c(3,3,3,n)*x3y2z3*3.+c(3,3,4,n)*x3y2z4*3.+c(3,3,5,n)*
     & x3y2z5*3.+c(3,4,0,n)*x3y3*4.+c(3,4,1,n)*x3y3z1*4.+c(3,4,2,n)*
     & x3y3z2*4.+c(3,4,3,n)*x3y3z3*4.+c(3,4,4,n)*x3y3z4*4.+c(3,4,5,n)*
     & x3y3z5*4.+c(3,5,0,n)*x3y4*5.+c(3,5,1,n)*x3y4z1*5.+c(3,5,2,n)*
     & x3y4z2*5.+c(3,5,3,n)*x3y4z3*5.+c(3,5,4,n)*x3y4z4*5.+c(3,5,5,n)*
     & x3y4z5*5.+c(4,1,0,n)*x4+c(4,1,1,n)*x4z1+c(4,1,2,n)*x4z2+c(4,1,
     & 3,n)*x4z3+c(4,1,4,n)*x4z4+c(4,1,5,n)*x4z5+c(4,2,0,n)*x4y1*2.+c(
     & 4,2,1,n)*x4y1z1*2.+c(4,2,2,n)*x4y1z2*2.+c(4,2,3,n)*x4y1z3*2.+c(
     & 4,2,4,n)*x4y1z4*2.+c(4,2,5,n)*x4y1z5*2.+c(4,3,0,n)*x4y2*3.+c(4,
     & 3,1,n)*x4y2z1*3.+c(4,3,2,n)*x4y2z2*3.+c(4,3,3,n)*x4y2z3*3.+c(4,
     & 3,4,n)*x4y2z4*3.+c(4,3,5,n)*x4y2z5*3.+c(4,4,0,n)*x4y3*4.+c(4,4,
     & 1,n)*x4y3z1*4.+c(4,4,2,n)*x4y3z2*4.+c(4,4,3,n)*x4y3z3*4.+c(4,4,
     & 4,n)*x4y3z4*4.+c(4,4,5,n)*x4y3z5*4.+c(4,5,0,n)*x4y4*5.+c(4,5,1,
     & n)*x4y4z1*5.+c(4,5,2,n)*x4y4z2*5.+c(4,5,3,n)*x4y4z3*5.+c(4,5,4,
     & n)*x4y4z4*5.+c(4,5,5,n)*x4y4z5*5.+c(5,1,0,n)*x5+c(5,1,1,n)*
     & x5z1+c(5,1,2,n)*x5z2+c(5,1,3,n)*x5z3+c(5,1,4,n)*x5z4+c(5,1,5,n)
     & *x5z5+c(5,2,0,n)*x5y1*2.+c(5,2,1,n)*x5y1z1*2.+c(5,2,2,n)*
     & x5y1z2*2.+c(5,2,3,n)*x5y1z3*2.+c(5,2,4,n)*x5y1z4*2.+c(5,2,5,n)*
     & x5y1z5*2.+c(5,3,0,n)*x5y2*3.+c(5,3,1,n)*x5y2z1*3.+c(5,3,2,n)*
     & x5y2z2*3.+c(5,3,3,n)*x5y2z3*3.+c(5,3,4,n)*x5y2z4*3.+c(5,3,5,n)*
     & x5y2z5*3.+c(5,4,0,n)*x5y3*4.+c(5,4,1,n)*x5y3z1*4.+c(5,4,2,n)*
     & x5y3z2*4.+c(5,4,3,n)*x5y3z3*4.+c(5,4,4,n)*x5y3z4*4.+c(5,4,5,n)*
     & x5y3z5*4.+c(5,5,0,n)*x5y4*5.+c(5,5,1,n)*x5y4z1*5.+c(5,5,2,n)*
     & x5y4z2*5.+c(5,5,3,n)*x5y4z3*5.+c(5,5,4,n)*x5y4z4*5.+c(5,5,5,n)*
     & x5y4z5*5.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.0.and.dy.eq.1.and.dz.eq.1 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      y3z4=y2z4*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y4z2=y3z2*y1
      y4z3=y3z3*y1
      y4z4=y3z4*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y2z4=x1y1z4*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x1y3z4=x1y2z4*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y4z2=x1y3z2*y1
      x1y4z3=x1y3z3*y1
      x1y4z4=x1y3z4*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y1z4=x1y1z4*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y2z4=x1y2z4*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y3z3=x1y3z3*x1
      x2y3z4=x1y3z4*x1
      x2y4=x1y4*x1
      x2y4z1=x1y4z1*x1
      x2y4z2=x1y4z2*x1
      x2y4z3=x1y4z3*x1
      x2y4z4=x1y4z4*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3z4=x2z4*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x3y1z4=x2y1z4*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y2z3=x2y2z3*x1
      x3y2z4=x2y2z4*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x3y3z2=x2y3z2*x1
      x3y3z3=x2y3z3*x1
      x3y3z4=x2y3z4*x1
      x3y4=x2y4*x1
      x3y4z1=x2y4z1*x1
      x3y4z2=x2y4z2*x1
      x3y4z3=x2y4z3*x1
      x3y4z4=x2y4z4*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4z2=x3z2*x1
      x4z3=x3z3*x1
      x4z4=x3z4*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y1z2=x3y1z2*x1
      x4y1z3=x3y1z3*x1
      x4y1z4=x3y1z4*x1
      x4y2=x3y2*x1
      x4y2z1=x3y2z1*x1
      x4y2z2=x3y2z2*x1
      x4y2z3=x3y2z3*x1
      x4y2z4=x3y2z4*x1
      x4y3=x3y3*x1
      x4y3z1=x3y3z1*x1
      x4y3z2=x3y3z2*x1
      x4y3z3=x3y3z3*x1
      x4y3z4=x3y3z4*x1
      x4y4=x3y4*x1
      x4y4z1=x3y4z1*x1
      x4y4z2=x3y4z2*x1
      x4y4z3=x3y4z3*x1
      x4y4z4=x3y4z4*x1
      x5=x4*x1
      x5z1=x4z1*x1
      x5z2=x4z2*x1
      x5z3=x4z3*x1
      x5z4=x4z4*x1
      x5y1=x4y1*x1
      x5y1z1=x4y1z1*x1
      x5y1z2=x4y1z2*x1
      x5y1z3=x4y1z3*x1
      x5y1z4=x4y1z4*x1
      x5y2=x4y2*x1
      x5y2z1=x4y2z1*x1
      x5y2z2=x4y2z2*x1
      x5y2z3=x4y2z3*x1
      x5y2z4=x4y2z4*x1
      x5y3=x4y3*x1
      x5y3z1=x4y3z1*x1
      x5y3z2=x4y3z2*x1
      x5y3z3=x4y3z3*x1
      x5y3z4=x4y3z4*x1
      x5y4=x4y4*x1
      x5y4z1=x4y4z1*x1
      x5y4z2=x4y4z2*x1
      x5y4z3=x4y4z3*x1
      x5y4z4=x4y4z4*x1
      r(i1,i2,i3,n)=(c(0,1,1,n)+c(0,1,2,n)*z1*2.+c(0,1,3,n)*z2*3.+c(0,
     & 1,4,n)*z3*4.+c(0,1,5,n)*z4*5.+c(0,2,1,n)*y1*2.+c(0,2,2,n)*y1z1*
     & 4.+c(0,2,3,n)*y1z2*6.+c(0,2,4,n)*y1z3*8.+c(0,2,5,n)*y1z4*10.+c(
     & 0,3,1,n)*y2*3.+c(0,3,2,n)*y2z1*6.+c(0,3,3,n)*y2z2*9.+c(0,3,4,n)
     & *y2z3*12.+c(0,3,5,n)*y2z4*15.+c(0,4,1,n)*y3*4.+c(0,4,2,n)*y3z1*
     & 8.+c(0,4,3,n)*y3z2*12.+c(0,4,4,n)*y3z3*16.+c(0,4,5,n)*y3z4*20.+
     & c(0,5,1,n)*y4*5.+c(0,5,2,n)*y4z1*10.+c(0,5,3,n)*y4z2*15.+c(0,5,
     & 4,n)*y4z3*20.+c(0,5,5,n)*y4z4*25.+c(1,1,1,n)*x1+c(1,1,2,n)*
     & x1z1*2.+c(1,1,3,n)*x1z2*3.+c(1,1,4,n)*x1z3*4.+c(1,1,5,n)*x1z4*
     & 5.+c(1,2,1,n)*x1y1*2.+c(1,2,2,n)*x1y1z1*4.+c(1,2,3,n)*x1y1z2*
     & 6.+c(1,2,4,n)*x1y1z3*8.+c(1,2,5,n)*x1y1z4*10.+c(1,3,1,n)*x1y2*
     & 3.+c(1,3,2,n)*x1y2z1*6.+c(1,3,3,n)*x1y2z2*9.+c(1,3,4,n)*x1y2z3*
     & 12.+c(1,3,5,n)*x1y2z4*15.+c(1,4,1,n)*x1y3*4.+c(1,4,2,n)*x1y3z1*
     & 8.+c(1,4,3,n)*x1y3z2*12.+c(1,4,4,n)*x1y3z3*16.+c(1,4,5,n)*
     & x1y3z4*20.+c(1,5,1,n)*x1y4*5.+c(1,5,2,n)*x1y4z1*10.+c(1,5,3,n)*
     & x1y4z2*15.+c(1,5,4,n)*x1y4z3*20.+c(1,5,5,n)*x1y4z4*25.+c(2,1,1,
     & n)*x2+c(2,1,2,n)*x2z1*2.+c(2,1,3,n)*x2z2*3.+c(2,1,4,n)*x2z3*4.+
     & c(2,1,5,n)*x2z4*5.+c(2,2,1,n)*x2y1*2.+c(2,2,2,n)*x2y1z1*4.+c(2,
     & 2,3,n)*x2y1z2*6.+c(2,2,4,n)*x2y1z3*8.+c(2,2,5,n)*x2y1z4*10.+c(
     & 2,3,1,n)*x2y2*3.+c(2,3,2,n)*x2y2z1*6.+c(2,3,3,n)*x2y2z2*9.+c(2,
     & 3,4,n)*x2y2z3*12.+c(2,3,5,n)*x2y2z4*15.+c(2,4,1,n)*x2y3*4.+c(2,
     & 4,2,n)*x2y3z1*8.+c(2,4,3,n)*x2y3z2*12.+c(2,4,4,n)*x2y3z3*16.+c(
     & 2,4,5,n)*x2y3z4*20.+c(2,5,1,n)*x2y4*5.+c(2,5,2,n)*x2y4z1*10.+c(
     & 2,5,3,n)*x2y4z2*15.+c(2,5,4,n)*x2y4z3*20.+c(2,5,5,n)*x2y4z4*
     & 25.+c(3,1,1,n)*x3+c(3,1,2,n)*x3z1*2.+c(3,1,3,n)*x3z2*3.+c(3,1,
     & 4,n)*x3z3*4.+c(3,1,5,n)*x3z4*5.+c(3,2,1,n)*x3y1*2.+c(3,2,2,n)*
     & x3y1z1*4.+c(3,2,3,n)*x3y1z2*6.+c(3,2,4,n)*x3y1z3*8.+c(3,2,5,n)*
     & x3y1z4*10.+c(3,3,1,n)*x3y2*3.+c(3,3,2,n)*x3y2z1*6.+c(3,3,3,n)*
     & x3y2z2*9.+c(3,3,4,n)*x3y2z3*12.+c(3,3,5,n)*x3y2z4*15.+c(3,4,1,
     & n)*x3y3*4.+c(3,4,2,n)*x3y3z1*8.+c(3,4,3,n)*x3y3z2*12.+c(3,4,4,
     & n)*x3y3z3*16.+c(3,4,5,n)*x3y3z4*20.+c(3,5,1,n)*x3y4*5.+c(3,5,2,
     & n)*x3y4z1*10.+c(3,5,3,n)*x3y4z2*15.+c(3,5,4,n)*x3y4z3*20.+c(3,
     & 5,5,n)*x3y4z4*25.+c(4,1,1,n)*x4+c(4,1,2,n)*x4z1*2.+c(4,1,3,n)*
     & x4z2*3.+c(4,1,4,n)*x4z3*4.+c(4,1,5,n)*x4z4*5.+c(4,2,1,n)*x4y1*
     & 2.+c(4,2,2,n)*x4y1z1*4.+c(4,2,3,n)*x4y1z2*6.+c(4,2,4,n)*x4y1z3*
     & 8.+c(4,2,5,n)*x4y1z4*10.+c(4,3,1,n)*x4y2*3.+c(4,3,2,n)*x4y2z1*
     & 6.+c(4,3,3,n)*x4y2z2*9.+c(4,3,4,n)*x4y2z3*12.+c(4,3,5,n)*
     & x4y2z4*15.+c(4,4,1,n)*x4y3*4.+c(4,4,2,n)*x4y3z1*8.+c(4,4,3,n)*
     & x4y3z2*12.+c(4,4,4,n)*x4y3z3*16.+c(4,4,5,n)*x4y3z4*20.+c(4,5,1,
     & n)*x4y4*5.+c(4,5,2,n)*x4y4z1*10.+c(4,5,3,n)*x4y4z2*15.+c(4,5,4,
     & n)*x4y4z3*20.+c(4,5,5,n)*x4y4z4*25.+c(5,1,1,n)*x5+c(5,1,2,n)*
     & x5z1*2.+c(5,1,3,n)*x5z2*3.+c(5,1,4,n)*x5z3*4.+c(5,1,5,n)*x5z4*
     & 5.+c(5,2,1,n)*x5y1*2.+c(5,2,2,n)*x5y1z1*4.+c(5,2,3,n)*x5y1z2*
     & 6.+c(5,2,4,n)*x5y1z3*8.+c(5,2,5,n)*x5y1z4*10.+c(5,3,1,n)*x5y2*
     & 3.+c(5,3,2,n)*x5y2z1*6.+c(5,3,3,n)*x5y2z2*9.+c(5,3,4,n)*x5y2z3*
     & 12.+c(5,3,5,n)*x5y2z4*15.+c(5,4,1,n)*x5y3*4.+c(5,4,2,n)*x5y3z1*
     & 8.+c(5,4,3,n)*x5y3z2*12.+c(5,4,4,n)*x5y3z3*16.+c(5,4,5,n)*
     & x5y3z4*20.+c(5,5,1,n)*x5y4*5.+c(5,5,2,n)*x5y4z1*10.+c(5,5,3,n)*
     & x5y4z2*15.+c(5,5,4,n)*x5y4z3*20.+c(5,5,5,n)*x5y4z4*25.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.0.and.dy.eq.1.and.dz.eq.2 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y4z2=y3z2*y1
      y4z3=y3z3*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y4z2=x1y3z2*y1
      x1y4z3=x1y3z3*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y3z3=x1y3z3*x1
      x2y4=x1y4*x1
      x2y4z1=x1y4z1*x1
      x2y4z2=x1y4z2*x1
      x2y4z3=x1y4z3*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y2z3=x2y2z3*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x3y3z2=x2y3z2*x1
      x3y3z3=x2y3z3*x1
      x3y4=x2y4*x1
      x3y4z1=x2y4z1*x1
      x3y4z2=x2y4z2*x1
      x3y4z3=x2y4z3*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4z2=x3z2*x1
      x4z3=x3z3*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y1z2=x3y1z2*x1
      x4y1z3=x3y1z3*x1
      x4y2=x3y2*x1
      x4y2z1=x3y2z1*x1
      x4y2z2=x3y2z2*x1
      x4y2z3=x3y2z3*x1
      x4y3=x3y3*x1
      x4y3z1=x3y3z1*x1
      x4y3z2=x3y3z2*x1
      x4y3z3=x3y3z3*x1
      x4y4=x3y4*x1
      x4y4z1=x3y4z1*x1
      x4y4z2=x3y4z2*x1
      x4y4z3=x3y4z3*x1
      x5=x4*x1
      x5z1=x4z1*x1
      x5z2=x4z2*x1
      x5z3=x4z3*x1
      x5y1=x4y1*x1
      x5y1z1=x4y1z1*x1
      x5y1z2=x4y1z2*x1
      x5y1z3=x4y1z3*x1
      x5y2=x4y2*x1
      x5y2z1=x4y2z1*x1
      x5y2z2=x4y2z2*x1
      x5y2z3=x4y2z3*x1
      x5y3=x4y3*x1
      x5y3z1=x4y3z1*x1
      x5y3z2=x4y3z2*x1
      x5y3z3=x4y3z3*x1
      x5y4=x4y4*x1
      x5y4z1=x4y4z1*x1
      x5y4z2=x4y4z2*x1
      x5y4z3=x4y4z3*x1
      r(i1,i2,i3,n)=(c(0,1,2,n)*2.+c(0,1,3,n)*z1*6.+c(0,1,4,n)*z2*12.+
     & c(0,1,5,n)*z3*20.+c(0,2,2,n)*y1*4.+c(0,2,3,n)*y1z1*12.+c(0,2,4,
     & n)*y1z2*24.+c(0,2,5,n)*y1z3*40.+c(0,3,2,n)*y2*6.+c(0,3,3,n)*
     & y2z1*18.+c(0,3,4,n)*y2z2*36.+c(0,3,5,n)*y2z3*60.+c(0,4,2,n)*y3*
     & 8.+c(0,4,3,n)*y3z1*24.+c(0,4,4,n)*y3z2*48.+c(0,4,5,n)*y3z3*80.+
     & c(0,5,2,n)*y4*10.+c(0,5,3,n)*y4z1*30.+c(0,5,4,n)*y4z2*60.+c(0,
     & 5,5,n)*y4z3*100.+c(1,1,2,n)*x1*2.+c(1,1,3,n)*x1z1*6.+c(1,1,4,n)
     & *x1z2*12.+c(1,1,5,n)*x1z3*20.+c(1,2,2,n)*x1y1*4.+c(1,2,3,n)*
     & x1y1z1*12.+c(1,2,4,n)*x1y1z2*24.+c(1,2,5,n)*x1y1z3*40.+c(1,3,2,
     & n)*x1y2*6.+c(1,3,3,n)*x1y2z1*18.+c(1,3,4,n)*x1y2z2*36.+c(1,3,5,
     & n)*x1y2z3*60.+c(1,4,2,n)*x1y3*8.+c(1,4,3,n)*x1y3z1*24.+c(1,4,4,
     & n)*x1y3z2*48.+c(1,4,5,n)*x1y3z3*80.+c(1,5,2,n)*x1y4*10.+c(1,5,
     & 3,n)*x1y4z1*30.+c(1,5,4,n)*x1y4z2*60.+c(1,5,5,n)*x1y4z3*100.+c(
     & 2,1,2,n)*x2*2.+c(2,1,3,n)*x2z1*6.+c(2,1,4,n)*x2z2*12.+c(2,1,5,
     & n)*x2z3*20.+c(2,2,2,n)*x2y1*4.+c(2,2,3,n)*x2y1z1*12.+c(2,2,4,n)
     & *x2y1z2*24.+c(2,2,5,n)*x2y1z3*40.+c(2,3,2,n)*x2y2*6.+c(2,3,3,n)
     & *x2y2z1*18.+c(2,3,4,n)*x2y2z2*36.+c(2,3,5,n)*x2y2z3*60.+c(2,4,
     & 2,n)*x2y3*8.+c(2,4,3,n)*x2y3z1*24.+c(2,4,4,n)*x2y3z2*48.+c(2,4,
     & 5,n)*x2y3z3*80.+c(2,5,2,n)*x2y4*10.+c(2,5,3,n)*x2y4z1*30.+c(2,
     & 5,4,n)*x2y4z2*60.+c(2,5,5,n)*x2y4z3*100.+c(3,1,2,n)*x3*2.+c(3,
     & 1,3,n)*x3z1*6.+c(3,1,4,n)*x3z2*12.+c(3,1,5,n)*x3z3*20.+c(3,2,2,
     & n)*x3y1*4.+c(3,2,3,n)*x3y1z1*12.+c(3,2,4,n)*x3y1z2*24.+c(3,2,5,
     & n)*x3y1z3*40.+c(3,3,2,n)*x3y2*6.+c(3,3,3,n)*x3y2z1*18.+c(3,3,4,
     & n)*x3y2z2*36.+c(3,3,5,n)*x3y2z3*60.+c(3,4,2,n)*x3y3*8.+c(3,4,3,
     & n)*x3y3z1*24.+c(3,4,4,n)*x3y3z2*48.+c(3,4,5,n)*x3y3z3*80.+c(3,
     & 5,2,n)*x3y4*10.+c(3,5,3,n)*x3y4z1*30.+c(3,5,4,n)*x3y4z2*60.+c(
     & 3,5,5,n)*x3y4z3*100.+c(4,1,2,n)*x4*2.+c(4,1,3,n)*x4z1*6.+c(4,1,
     & 4,n)*x4z2*12.+c(4,1,5,n)*x4z3*20.+c(4,2,2,n)*x4y1*4.+c(4,2,3,n)
     & *x4y1z1*12.+c(4,2,4,n)*x4y1z2*24.+c(4,2,5,n)*x4y1z3*40.+c(4,3,
     & 2,n)*x4y2*6.+c(4,3,3,n)*x4y2z1*18.+c(4,3,4,n)*x4y2z2*36.+c(4,3,
     & 5,n)*x4y2z3*60.+c(4,4,2,n)*x4y3*8.+c(4,4,3,n)*x4y3z1*24.+c(4,4,
     & 4,n)*x4y3z2*48.+c(4,4,5,n)*x4y3z3*80.+c(4,5,2,n)*x4y4*10.+c(4,
     & 5,3,n)*x4y4z1*30.+c(4,5,4,n)*x4y4z2*60.+c(4,5,5,n)*x4y4z3*100.+
     & c(5,1,2,n)*x5*2.+c(5,1,3,n)*x5z1*6.+c(5,1,4,n)*x5z2*12.+c(5,1,
     & 5,n)*x5z3*20.+c(5,2,2,n)*x5y1*4.+c(5,2,3,n)*x5y1z1*12.+c(5,2,4,
     & n)*x5y1z2*24.+c(5,2,5,n)*x5y1z3*40.+c(5,3,2,n)*x5y2*6.+c(5,3,3,
     & n)*x5y2z1*18.+c(5,3,4,n)*x5y2z2*36.+c(5,3,5,n)*x5y2z3*60.+c(5,
     & 4,2,n)*x5y3*8.+c(5,4,3,n)*x5y3z1*24.+c(5,4,4,n)*x5y3z2*48.+c(5,
     & 4,5,n)*x5y3z3*80.+c(5,5,2,n)*x5y4*10.+c(5,5,3,n)*x5y4z1*30.+c(
     & 5,5,4,n)*x5y4z2*60.+c(5,5,5,n)*x5y4z3*100.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.0.and.dy.eq.1.and.dz.eq.3 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y4z2=y3z2*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y4z2=x1y3z2*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y4=x1y4*x1
      x2y4z1=x1y4z1*x1
      x2y4z2=x1y4z2*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x3y3z2=x2y3z2*x1
      x3y4=x2y4*x1
      x3y4z1=x2y4z1*x1
      x3y4z2=x2y4z2*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4z2=x3z2*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y1z2=x3y1z2*x1
      x4y2=x3y2*x1
      x4y2z1=x3y2z1*x1
      x4y2z2=x3y2z2*x1
      x4y3=x3y3*x1
      x4y3z1=x3y3z1*x1
      x4y3z2=x3y3z2*x1
      x4y4=x3y4*x1
      x4y4z1=x3y4z1*x1
      x4y4z2=x3y4z2*x1
      x5=x4*x1
      x5z1=x4z1*x1
      x5z2=x4z2*x1
      x5y1=x4y1*x1
      x5y1z1=x4y1z1*x1
      x5y1z2=x4y1z2*x1
      x5y2=x4y2*x1
      x5y2z1=x4y2z1*x1
      x5y2z2=x4y2z2*x1
      x5y3=x4y3*x1
      x5y3z1=x4y3z1*x1
      x5y3z2=x4y3z2*x1
      x5y4=x4y4*x1
      x5y4z1=x4y4z1*x1
      x5y4z2=x4y4z2*x1
      r(i1,i2,i3,n)=(c(0,1,3,n)*6.+c(0,1,4,n)*z1*24.+c(0,1,5,n)*z2*60.+
     & c(0,2,3,n)*y1*12.+c(0,2,4,n)*y1z1*48.+c(0,2,5,n)*y1z2*120.+c(0,
     & 3,3,n)*y2*18.+c(0,3,4,n)*y2z1*72.+c(0,3,5,n)*y2z2*180.+c(0,4,3,
     & n)*y3*24.+c(0,4,4,n)*y3z1*96.+c(0,4,5,n)*y3z2*240.+c(0,5,3,n)*
     & y4*30.+c(0,5,4,n)*y4z1*120.+c(0,5,5,n)*y4z2*300.+c(1,1,3,n)*x1*
     & 6.+c(1,1,4,n)*x1z1*24.+c(1,1,5,n)*x1z2*60.+c(1,2,3,n)*x1y1*12.+
     & c(1,2,4,n)*x1y1z1*48.+c(1,2,5,n)*x1y1z2*120.+c(1,3,3,n)*x1y2*
     & 18.+c(1,3,4,n)*x1y2z1*72.+c(1,3,5,n)*x1y2z2*180.+c(1,4,3,n)*
     & x1y3*24.+c(1,4,4,n)*x1y3z1*96.+c(1,4,5,n)*x1y3z2*240.+c(1,5,3,
     & n)*x1y4*30.+c(1,5,4,n)*x1y4z1*120.+c(1,5,5,n)*x1y4z2*300.+c(2,
     & 1,3,n)*x2*6.+c(2,1,4,n)*x2z1*24.+c(2,1,5,n)*x2z2*60.+c(2,2,3,n)
     & *x2y1*12.+c(2,2,4,n)*x2y1z1*48.+c(2,2,5,n)*x2y1z2*120.+c(2,3,3,
     & n)*x2y2*18.+c(2,3,4,n)*x2y2z1*72.+c(2,3,5,n)*x2y2z2*180.+c(2,4,
     & 3,n)*x2y3*24.+c(2,4,4,n)*x2y3z1*96.+c(2,4,5,n)*x2y3z2*240.+c(2,
     & 5,3,n)*x2y4*30.+c(2,5,4,n)*x2y4z1*120.+c(2,5,5,n)*x2y4z2*300.+
     & c(3,1,3,n)*x3*6.+c(3,1,4,n)*x3z1*24.+c(3,1,5,n)*x3z2*60.+c(3,2,
     & 3,n)*x3y1*12.+c(3,2,4,n)*x3y1z1*48.+c(3,2,5,n)*x3y1z2*120.+c(3,
     & 3,3,n)*x3y2*18.+c(3,3,4,n)*x3y2z1*72.+c(3,3,5,n)*x3y2z2*180.+c(
     & 3,4,3,n)*x3y3*24.+c(3,4,4,n)*x3y3z1*96.+c(3,4,5,n)*x3y3z2*240.+
     & c(3,5,3,n)*x3y4*30.+c(3,5,4,n)*x3y4z1*120.+c(3,5,5,n)*x3y4z2*
     & 300.+c(4,1,3,n)*x4*6.+c(4,1,4,n)*x4z1*24.+c(4,1,5,n)*x4z2*60.+
     & c(4,2,3,n)*x4y1*12.+c(4,2,4,n)*x4y1z1*48.+c(4,2,5,n)*x4y1z2*
     & 120.+c(4,3,3,n)*x4y2*18.+c(4,3,4,n)*x4y2z1*72.+c(4,3,5,n)*
     & x4y2z2*180.+c(4,4,3,n)*x4y3*24.+c(4,4,4,n)*x4y3z1*96.+c(4,4,5,
     & n)*x4y3z2*240.+c(4,5,3,n)*x4y4*30.+c(4,5,4,n)*x4y4z1*120.+c(4,
     & 5,5,n)*x4y4z2*300.+c(5,1,3,n)*x5*6.+c(5,1,4,n)*x5z1*24.+c(5,1,
     & 5,n)*x5z2*60.+c(5,2,3,n)*x5y1*12.+c(5,2,4,n)*x5y1z1*48.+c(5,2,
     & 5,n)*x5y1z2*120.+c(5,3,3,n)*x5y2*18.+c(5,3,4,n)*x5y2z1*72.+c(5,
     & 3,5,n)*x5y2z2*180.+c(5,4,3,n)*x5y3*24.+c(5,4,4,n)*x5y3z1*96.+c(
     & 5,4,5,n)*x5y3z2*240.+c(5,5,3,n)*x5y4*30.+c(5,5,4,n)*x5y4z1*
     & 120.+c(5,5,5,n)*x5y4z2*300.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.0.and.dy.eq.1.and.dz.eq.4 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y4=y3*y1
      y4z1=y3z1*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y4=x1y4*x1
      x2y4z1=x1y4z1*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x3y4=x2y4*x1
      x3y4z1=x2y4z1*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y2=x3y2*x1
      x4y2z1=x3y2z1*x1
      x4y3=x3y3*x1
      x4y3z1=x3y3z1*x1
      x4y4=x3y4*x1
      x4y4z1=x3y4z1*x1
      x5=x4*x1
      x5z1=x4z1*x1
      x5y1=x4y1*x1
      x5y1z1=x4y1z1*x1
      x5y2=x4y2*x1
      x5y2z1=x4y2z1*x1
      x5y3=x4y3*x1
      x5y3z1=x4y3z1*x1
      x5y4=x4y4*x1
      x5y4z1=x4y4z1*x1
      r(i1,i2,i3,n)=(c(0,1,4,n)*24.+c(0,1,5,n)*z1*120.+c(0,2,4,n)*y1*
     & 48.+c(0,2,5,n)*y1z1*240.+c(0,3,4,n)*y2*72.+c(0,3,5,n)*y2z1*
     & 360.+c(0,4,4,n)*y3*96.+c(0,4,5,n)*y3z1*480.+c(0,5,4,n)*y4*120.+
     & c(0,5,5,n)*y4z1*600.+c(1,1,4,n)*x1*24.+c(1,1,5,n)*x1z1*120.+c(
     & 1,2,4,n)*x1y1*48.+c(1,2,5,n)*x1y1z1*240.+c(1,3,4,n)*x1y2*72.+c(
     & 1,3,5,n)*x1y2z1*360.+c(1,4,4,n)*x1y3*96.+c(1,4,5,n)*x1y3z1*
     & 480.+c(1,5,4,n)*x1y4*120.+c(1,5,5,n)*x1y4z1*600.+c(2,1,4,n)*x2*
     & 24.+c(2,1,5,n)*x2z1*120.+c(2,2,4,n)*x2y1*48.+c(2,2,5,n)*x2y1z1*
     & 240.+c(2,3,4,n)*x2y2*72.+c(2,3,5,n)*x2y2z1*360.+c(2,4,4,n)*
     & x2y3*96.+c(2,4,5,n)*x2y3z1*480.+c(2,5,4,n)*x2y4*120.+c(2,5,5,n)
     & *x2y4z1*600.+c(3,1,4,n)*x3*24.+c(3,1,5,n)*x3z1*120.+c(3,2,4,n)*
     & x3y1*48.+c(3,2,5,n)*x3y1z1*240.+c(3,3,4,n)*x3y2*72.+c(3,3,5,n)*
     & x3y2z1*360.+c(3,4,4,n)*x3y3*96.+c(3,4,5,n)*x3y3z1*480.+c(3,5,4,
     & n)*x3y4*120.+c(3,5,5,n)*x3y4z1*600.+c(4,1,4,n)*x4*24.+c(4,1,5,
     & n)*x4z1*120.+c(4,2,4,n)*x4y1*48.+c(4,2,5,n)*x4y1z1*240.+c(4,3,
     & 4,n)*x4y2*72.+c(4,3,5,n)*x4y2z1*360.+c(4,4,4,n)*x4y3*96.+c(4,4,
     & 5,n)*x4y3z1*480.+c(4,5,4,n)*x4y4*120.+c(4,5,5,n)*x4y4z1*600.+c(
     & 5,1,4,n)*x5*24.+c(5,1,5,n)*x5z1*120.+c(5,2,4,n)*x5y1*48.+c(5,2,
     & 5,n)*x5y1z1*240.+c(5,3,4,n)*x5y2*72.+c(5,3,5,n)*x5y2z1*360.+c(
     & 5,4,4,n)*x5y3*96.+c(5,4,5,n)*x5y3z1*480.+c(5,5,4,n)*x5y4*120.+
     & c(5,5,5,n)*x5y4z1*600.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.0.and.dy.eq.2.and.dz.eq.0 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y1z5=y1z4*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
      y2z5=y1z5*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      y3z4=y2z4*y1
      y3z5=y2z5*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1z5=x1z4*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y1z5=x1y1z4*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y2z4=x1y1z4*y1
      x1y2z5=x1y1z5*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x1y3z4=x1y2z4*y1
      x1y3z5=x1y2z5*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x2z5=x1z5*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y1z4=x1y1z4*x1
      x2y1z5=x1y1z5*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y2z4=x1y2z4*x1
      x2y2z5=x1y2z5*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y3z3=x1y3z3*x1
      x2y3z4=x1y3z4*x1
      x2y3z5=x1y3z5*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3z4=x2z4*x1
      x3z5=x2z5*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x3y1z4=x2y1z4*x1
      x3y1z5=x2y1z5*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y2z3=x2y2z3*x1
      x3y2z4=x2y2z4*x1
      x3y2z5=x2y2z5*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x3y3z2=x2y3z2*x1
      x3y3z3=x2y3z3*x1
      x3y3z4=x2y3z4*x1
      x3y3z5=x2y3z5*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4z2=x3z2*x1
      x4z3=x3z3*x1
      x4z4=x3z4*x1
      x4z5=x3z5*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y1z2=x3y1z2*x1
      x4y1z3=x3y1z3*x1
      x4y1z4=x3y1z4*x1
      x4y1z5=x3y1z5*x1
      x4y2=x3y2*x1
      x4y2z1=x3y2z1*x1
      x4y2z2=x3y2z2*x1
      x4y2z3=x3y2z3*x1
      x4y2z4=x3y2z4*x1
      x4y2z5=x3y2z5*x1
      x4y3=x3y3*x1
      x4y3z1=x3y3z1*x1
      x4y3z2=x3y3z2*x1
      x4y3z3=x3y3z3*x1
      x4y3z4=x3y3z4*x1
      x4y3z5=x3y3z5*x1
      x5=x4*x1
      x5z1=x4z1*x1
      x5z2=x4z2*x1
      x5z3=x4z3*x1
      x5z4=x4z4*x1
      x5z5=x4z5*x1
      x5y1=x4y1*x1
      x5y1z1=x4y1z1*x1
      x5y1z2=x4y1z2*x1
      x5y1z3=x4y1z3*x1
      x5y1z4=x4y1z4*x1
      x5y1z5=x4y1z5*x1
      x5y2=x4y2*x1
      x5y2z1=x4y2z1*x1
      x5y2z2=x4y2z2*x1
      x5y2z3=x4y2z3*x1
      x5y2z4=x4y2z4*x1
      x5y2z5=x4y2z5*x1
      x5y3=x4y3*x1
      x5y3z1=x4y3z1*x1
      x5y3z2=x4y3z2*x1
      x5y3z3=x4y3z3*x1
      x5y3z4=x4y3z4*x1
      x5y3z5=x4y3z5*x1
      r(i1,i2,i3,n)=(c(0,2,0,n)*2.+c(0,2,1,n)*z1*2.+c(0,2,2,n)*z2*2.+c(
     & 0,2,3,n)*z3*2.+c(0,2,4,n)*z4*2.+c(0,2,5,n)*z5*2.+c(0,3,0,n)*y1*
     & 6.+c(0,3,1,n)*y1z1*6.+c(0,3,2,n)*y1z2*6.+c(0,3,3,n)*y1z3*6.+c(
     & 0,3,4,n)*y1z4*6.+c(0,3,5,n)*y1z5*6.+c(0,4,0,n)*y2*12.+c(0,4,1,
     & n)*y2z1*12.+c(0,4,2,n)*y2z2*12.+c(0,4,3,n)*y2z3*12.+c(0,4,4,n)*
     & y2z4*12.+c(0,4,5,n)*y2z5*12.+c(0,5,0,n)*y3*20.+c(0,5,1,n)*y3z1*
     & 20.+c(0,5,2,n)*y3z2*20.+c(0,5,3,n)*y3z3*20.+c(0,5,4,n)*y3z4*
     & 20.+c(0,5,5,n)*y3z5*20.+c(1,2,0,n)*x1*2.+c(1,2,1,n)*x1z1*2.+c(
     & 1,2,2,n)*x1z2*2.+c(1,2,3,n)*x1z3*2.+c(1,2,4,n)*x1z4*2.+c(1,2,5,
     & n)*x1z5*2.+c(1,3,0,n)*x1y1*6.+c(1,3,1,n)*x1y1z1*6.+c(1,3,2,n)*
     & x1y1z2*6.+c(1,3,3,n)*x1y1z3*6.+c(1,3,4,n)*x1y1z4*6.+c(1,3,5,n)*
     & x1y1z5*6.+c(1,4,0,n)*x1y2*12.+c(1,4,1,n)*x1y2z1*12.+c(1,4,2,n)*
     & x1y2z2*12.+c(1,4,3,n)*x1y2z3*12.+c(1,4,4,n)*x1y2z4*12.+c(1,4,5,
     & n)*x1y2z5*12.+c(1,5,0,n)*x1y3*20.+c(1,5,1,n)*x1y3z1*20.+c(1,5,
     & 2,n)*x1y3z2*20.+c(1,5,3,n)*x1y3z3*20.+c(1,5,4,n)*x1y3z4*20.+c(
     & 1,5,5,n)*x1y3z5*20.+c(2,2,0,n)*x2*2.+c(2,2,1,n)*x2z1*2.+c(2,2,
     & 2,n)*x2z2*2.+c(2,2,3,n)*x2z3*2.+c(2,2,4,n)*x2z4*2.+c(2,2,5,n)*
     & x2z5*2.+c(2,3,0,n)*x2y1*6.+c(2,3,1,n)*x2y1z1*6.+c(2,3,2,n)*
     & x2y1z2*6.+c(2,3,3,n)*x2y1z3*6.+c(2,3,4,n)*x2y1z4*6.+c(2,3,5,n)*
     & x2y1z5*6.+c(2,4,0,n)*x2y2*12.+c(2,4,1,n)*x2y2z1*12.+c(2,4,2,n)*
     & x2y2z2*12.+c(2,4,3,n)*x2y2z3*12.+c(2,4,4,n)*x2y2z4*12.+c(2,4,5,
     & n)*x2y2z5*12.+c(2,5,0,n)*x2y3*20.+c(2,5,1,n)*x2y3z1*20.+c(2,5,
     & 2,n)*x2y3z2*20.+c(2,5,3,n)*x2y3z3*20.+c(2,5,4,n)*x2y3z4*20.+c(
     & 2,5,5,n)*x2y3z5*20.+c(3,2,0,n)*x3*2.+c(3,2,1,n)*x3z1*2.+c(3,2,
     & 2,n)*x3z2*2.+c(3,2,3,n)*x3z3*2.+c(3,2,4,n)*x3z4*2.+c(3,2,5,n)*
     & x3z5*2.+c(3,3,0,n)*x3y1*6.+c(3,3,1,n)*x3y1z1*6.+c(3,3,2,n)*
     & x3y1z2*6.+c(3,3,3,n)*x3y1z3*6.+c(3,3,4,n)*x3y1z4*6.+c(3,3,5,n)*
     & x3y1z5*6.+c(3,4,0,n)*x3y2*12.+c(3,4,1,n)*x3y2z1*12.+c(3,4,2,n)*
     & x3y2z2*12.+c(3,4,3,n)*x3y2z3*12.+c(3,4,4,n)*x3y2z4*12.+c(3,4,5,
     & n)*x3y2z5*12.+c(3,5,0,n)*x3y3*20.+c(3,5,1,n)*x3y3z1*20.+c(3,5,
     & 2,n)*x3y3z2*20.+c(3,5,3,n)*x3y3z3*20.+c(3,5,4,n)*x3y3z4*20.+c(
     & 3,5,5,n)*x3y3z5*20.+c(4,2,0,n)*x4*2.+c(4,2,1,n)*x4z1*2.+c(4,2,
     & 2,n)*x4z2*2.+c(4,2,3,n)*x4z3*2.+c(4,2,4,n)*x4z4*2.+c(4,2,5,n)*
     & x4z5*2.+c(4,3,0,n)*x4y1*6.+c(4,3,1,n)*x4y1z1*6.+c(4,3,2,n)*
     & x4y1z2*6.+c(4,3,3,n)*x4y1z3*6.+c(4,3,4,n)*x4y1z4*6.+c(4,3,5,n)*
     & x4y1z5*6.+c(4,4,0,n)*x4y2*12.+c(4,4,1,n)*x4y2z1*12.+c(4,4,2,n)*
     & x4y2z2*12.+c(4,4,3,n)*x4y2z3*12.+c(4,4,4,n)*x4y2z4*12.+c(4,4,5,
     & n)*x4y2z5*12.+c(4,5,0,n)*x4y3*20.+c(4,5,1,n)*x4y3z1*20.+c(4,5,
     & 2,n)*x4y3z2*20.+c(4,5,3,n)*x4y3z3*20.+c(4,5,4,n)*x4y3z4*20.+c(
     & 4,5,5,n)*x4y3z5*20.+c(5,2,0,n)*x5*2.+c(5,2,1,n)*x5z1*2.+c(5,2,
     & 2,n)*x5z2*2.+c(5,2,3,n)*x5z3*2.+c(5,2,4,n)*x5z4*2.+c(5,2,5,n)*
     & x5z5*2.+c(5,3,0,n)*x5y1*6.+c(5,3,1,n)*x5y1z1*6.+c(5,3,2,n)*
     & x5y1z2*6.+c(5,3,3,n)*x5y1z3*6.+c(5,3,4,n)*x5y1z4*6.+c(5,3,5,n)*
     & x5y1z5*6.+c(5,4,0,n)*x5y2*12.+c(5,4,1,n)*x5y2z1*12.+c(5,4,2,n)*
     & x5y2z2*12.+c(5,4,3,n)*x5y2z3*12.+c(5,4,4,n)*x5y2z4*12.+c(5,4,5,
     & n)*x5y2z5*12.+c(5,5,0,n)*x5y3*20.+c(5,5,1,n)*x5y3z1*20.+c(5,5,
     & 2,n)*x5y3z2*20.+c(5,5,3,n)*x5y3z3*20.+c(5,5,4,n)*x5y3z4*20.+c(
     & 5,5,5,n)*x5y3z5*20.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.0.and.dy.eq.2.and.dz.eq.1 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      y3z4=y2z4*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y2z4=x1y1z4*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x1y3z4=x1y2z4*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y1z4=x1y1z4*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y2z4=x1y2z4*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y3z3=x1y3z3*x1
      x2y3z4=x1y3z4*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3z4=x2z4*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x3y1z4=x2y1z4*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y2z3=x2y2z3*x1
      x3y2z4=x2y2z4*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x3y3z2=x2y3z2*x1
      x3y3z3=x2y3z3*x1
      x3y3z4=x2y3z4*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4z2=x3z2*x1
      x4z3=x3z3*x1
      x4z4=x3z4*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y1z2=x3y1z2*x1
      x4y1z3=x3y1z3*x1
      x4y1z4=x3y1z4*x1
      x4y2=x3y2*x1
      x4y2z1=x3y2z1*x1
      x4y2z2=x3y2z2*x1
      x4y2z3=x3y2z3*x1
      x4y2z4=x3y2z4*x1
      x4y3=x3y3*x1
      x4y3z1=x3y3z1*x1
      x4y3z2=x3y3z2*x1
      x4y3z3=x3y3z3*x1
      x4y3z4=x3y3z4*x1
      x5=x4*x1
      x5z1=x4z1*x1
      x5z2=x4z2*x1
      x5z3=x4z3*x1
      x5z4=x4z4*x1
      x5y1=x4y1*x1
      x5y1z1=x4y1z1*x1
      x5y1z2=x4y1z2*x1
      x5y1z3=x4y1z3*x1
      x5y1z4=x4y1z4*x1
      x5y2=x4y2*x1
      x5y2z1=x4y2z1*x1
      x5y2z2=x4y2z2*x1
      x5y2z3=x4y2z3*x1
      x5y2z4=x4y2z4*x1
      x5y3=x4y3*x1
      x5y3z1=x4y3z1*x1
      x5y3z2=x4y3z2*x1
      x5y3z3=x4y3z3*x1
      x5y3z4=x4y3z4*x1
      r(i1,i2,i3,n)=(c(0,2,1,n)*2.+c(0,2,2,n)*z1*4.+c(0,2,3,n)*z2*6.+c(
     & 0,2,4,n)*z3*8.+c(0,2,5,n)*z4*10.+c(0,3,1,n)*y1*6.+c(0,3,2,n)*
     & y1z1*12.+c(0,3,3,n)*y1z2*18.+c(0,3,4,n)*y1z3*24.+c(0,3,5,n)*
     & y1z4*30.+c(0,4,1,n)*y2*12.+c(0,4,2,n)*y2z1*24.+c(0,4,3,n)*y2z2*
     & 36.+c(0,4,4,n)*y2z3*48.+c(0,4,5,n)*y2z4*60.+c(0,5,1,n)*y3*20.+
     & c(0,5,2,n)*y3z1*40.+c(0,5,3,n)*y3z2*60.+c(0,5,4,n)*y3z3*80.+c(
     & 0,5,5,n)*y3z4*100.+c(1,2,1,n)*x1*2.+c(1,2,2,n)*x1z1*4.+c(1,2,3,
     & n)*x1z2*6.+c(1,2,4,n)*x1z3*8.+c(1,2,5,n)*x1z4*10.+c(1,3,1,n)*
     & x1y1*6.+c(1,3,2,n)*x1y1z1*12.+c(1,3,3,n)*x1y1z2*18.+c(1,3,4,n)*
     & x1y1z3*24.+c(1,3,5,n)*x1y1z4*30.+c(1,4,1,n)*x1y2*12.+c(1,4,2,n)
     & *x1y2z1*24.+c(1,4,3,n)*x1y2z2*36.+c(1,4,4,n)*x1y2z3*48.+c(1,4,
     & 5,n)*x1y2z4*60.+c(1,5,1,n)*x1y3*20.+c(1,5,2,n)*x1y3z1*40.+c(1,
     & 5,3,n)*x1y3z2*60.+c(1,5,4,n)*x1y3z3*80.+c(1,5,5,n)*x1y3z4*100.+
     & c(2,2,1,n)*x2*2.+c(2,2,2,n)*x2z1*4.+c(2,2,3,n)*x2z2*6.+c(2,2,4,
     & n)*x2z3*8.+c(2,2,5,n)*x2z4*10.+c(2,3,1,n)*x2y1*6.+c(2,3,2,n)*
     & x2y1z1*12.+c(2,3,3,n)*x2y1z2*18.+c(2,3,4,n)*x2y1z3*24.+c(2,3,5,
     & n)*x2y1z4*30.+c(2,4,1,n)*x2y2*12.+c(2,4,2,n)*x2y2z1*24.+c(2,4,
     & 3,n)*x2y2z2*36.+c(2,4,4,n)*x2y2z3*48.+c(2,4,5,n)*x2y2z4*60.+c(
     & 2,5,1,n)*x2y3*20.+c(2,5,2,n)*x2y3z1*40.+c(2,5,3,n)*x2y3z2*60.+
     & c(2,5,4,n)*x2y3z3*80.+c(2,5,5,n)*x2y3z4*100.+c(3,2,1,n)*x3*2.+
     & c(3,2,2,n)*x3z1*4.+c(3,2,3,n)*x3z2*6.+c(3,2,4,n)*x3z3*8.+c(3,2,
     & 5,n)*x3z4*10.+c(3,3,1,n)*x3y1*6.+c(3,3,2,n)*x3y1z1*12.+c(3,3,3,
     & n)*x3y1z2*18.+c(3,3,4,n)*x3y1z3*24.+c(3,3,5,n)*x3y1z4*30.+c(3,
     & 4,1,n)*x3y2*12.+c(3,4,2,n)*x3y2z1*24.+c(3,4,3,n)*x3y2z2*36.+c(
     & 3,4,4,n)*x3y2z3*48.+c(3,4,5,n)*x3y2z4*60.+c(3,5,1,n)*x3y3*20.+
     & c(3,5,2,n)*x3y3z1*40.+c(3,5,3,n)*x3y3z2*60.+c(3,5,4,n)*x3y3z3*
     & 80.+c(3,5,5,n)*x3y3z4*100.+c(4,2,1,n)*x4*2.+c(4,2,2,n)*x4z1*4.+
     & c(4,2,3,n)*x4z2*6.+c(4,2,4,n)*x4z3*8.+c(4,2,5,n)*x4z4*10.+c(4,
     & 3,1,n)*x4y1*6.+c(4,3,2,n)*x4y1z1*12.+c(4,3,3,n)*x4y1z2*18.+c(4,
     & 3,4,n)*x4y1z3*24.+c(4,3,5,n)*x4y1z4*30.+c(4,4,1,n)*x4y2*12.+c(
     & 4,4,2,n)*x4y2z1*24.+c(4,4,3,n)*x4y2z2*36.+c(4,4,4,n)*x4y2z3*
     & 48.+c(4,4,5,n)*x4y2z4*60.+c(4,5,1,n)*x4y3*20.+c(4,5,2,n)*
     & x4y3z1*40.+c(4,5,3,n)*x4y3z2*60.+c(4,5,4,n)*x4y3z3*80.+c(4,5,5,
     & n)*x4y3z4*100.+c(5,2,1,n)*x5*2.+c(5,2,2,n)*x5z1*4.+c(5,2,3,n)*
     & x5z2*6.+c(5,2,4,n)*x5z3*8.+c(5,2,5,n)*x5z4*10.+c(5,3,1,n)*x5y1*
     & 6.+c(5,3,2,n)*x5y1z1*12.+c(5,3,3,n)*x5y1z2*18.+c(5,3,4,n)*
     & x5y1z3*24.+c(5,3,5,n)*x5y1z4*30.+c(5,4,1,n)*x5y2*12.+c(5,4,2,n)
     & *x5y2z1*24.+c(5,4,3,n)*x5y2z2*36.+c(5,4,4,n)*x5y2z3*48.+c(5,4,
     & 5,n)*x5y2z4*60.+c(5,5,1,n)*x5y3*20.+c(5,5,2,n)*x5y3z1*40.+c(5,
     & 5,3,n)*x5y3z2*60.+c(5,5,4,n)*x5y3z3*80.+c(5,5,5,n)*x5y3z4*100.)
     & *time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.0.and.dy.eq.2.and.dz.eq.2 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y3z3=x1y3z3*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y2z3=x2y2z3*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x3y3z2=x2y3z2*x1
      x3y3z3=x2y3z3*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4z2=x3z2*x1
      x4z3=x3z3*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y1z2=x3y1z2*x1
      x4y1z3=x3y1z3*x1
      x4y2=x3y2*x1
      x4y2z1=x3y2z1*x1
      x4y2z2=x3y2z2*x1
      x4y2z3=x3y2z3*x1
      x4y3=x3y3*x1
      x4y3z1=x3y3z1*x1
      x4y3z2=x3y3z2*x1
      x4y3z3=x3y3z3*x1
      x5=x4*x1
      x5z1=x4z1*x1
      x5z2=x4z2*x1
      x5z3=x4z3*x1
      x5y1=x4y1*x1
      x5y1z1=x4y1z1*x1
      x5y1z2=x4y1z2*x1
      x5y1z3=x4y1z3*x1
      x5y2=x4y2*x1
      x5y2z1=x4y2z1*x1
      x5y2z2=x4y2z2*x1
      x5y2z3=x4y2z3*x1
      x5y3=x4y3*x1
      x5y3z1=x4y3z1*x1
      x5y3z2=x4y3z2*x1
      x5y3z3=x4y3z3*x1
      r(i1,i2,i3,n)=(c(0,2,2,n)*4.+c(0,2,3,n)*z1*12.+c(0,2,4,n)*z2*24.+
     & c(0,2,5,n)*z3*40.+c(0,3,2,n)*y1*12.+c(0,3,3,n)*y1z1*36.+c(0,3,
     & 4,n)*y1z2*72.+c(0,3,5,n)*y1z3*120.+c(0,4,2,n)*y2*24.+c(0,4,3,n)
     & *y2z1*72.+c(0,4,4,n)*y2z2*144.+c(0,4,5,n)*y2z3*240.+c(0,5,2,n)*
     & y3*40.+c(0,5,3,n)*y3z1*120.+c(0,5,4,n)*y3z2*240.+c(0,5,5,n)*
     & y3z3*400.+c(1,2,2,n)*x1*4.+c(1,2,3,n)*x1z1*12.+c(1,2,4,n)*x1z2*
     & 24.+c(1,2,5,n)*x1z3*40.+c(1,3,2,n)*x1y1*12.+c(1,3,3,n)*x1y1z1*
     & 36.+c(1,3,4,n)*x1y1z2*72.+c(1,3,5,n)*x1y1z3*120.+c(1,4,2,n)*
     & x1y2*24.+c(1,4,3,n)*x1y2z1*72.+c(1,4,4,n)*x1y2z2*144.+c(1,4,5,
     & n)*x1y2z3*240.+c(1,5,2,n)*x1y3*40.+c(1,5,3,n)*x1y3z1*120.+c(1,
     & 5,4,n)*x1y3z2*240.+c(1,5,5,n)*x1y3z3*400.+c(2,2,2,n)*x2*4.+c(2,
     & 2,3,n)*x2z1*12.+c(2,2,4,n)*x2z2*24.+c(2,2,5,n)*x2z3*40.+c(2,3,
     & 2,n)*x2y1*12.+c(2,3,3,n)*x2y1z1*36.+c(2,3,4,n)*x2y1z2*72.+c(2,
     & 3,5,n)*x2y1z3*120.+c(2,4,2,n)*x2y2*24.+c(2,4,3,n)*x2y2z1*72.+c(
     & 2,4,4,n)*x2y2z2*144.+c(2,4,5,n)*x2y2z3*240.+c(2,5,2,n)*x2y3*
     & 40.+c(2,5,3,n)*x2y3z1*120.+c(2,5,4,n)*x2y3z2*240.+c(2,5,5,n)*
     & x2y3z3*400.+c(3,2,2,n)*x3*4.+c(3,2,3,n)*x3z1*12.+c(3,2,4,n)*
     & x3z2*24.+c(3,2,5,n)*x3z3*40.+c(3,3,2,n)*x3y1*12.+c(3,3,3,n)*
     & x3y1z1*36.+c(3,3,4,n)*x3y1z2*72.+c(3,3,5,n)*x3y1z3*120.+c(3,4,
     & 2,n)*x3y2*24.+c(3,4,3,n)*x3y2z1*72.+c(3,4,4,n)*x3y2z2*144.+c(3,
     & 4,5,n)*x3y2z3*240.+c(3,5,2,n)*x3y3*40.+c(3,5,3,n)*x3y3z1*120.+
     & c(3,5,4,n)*x3y3z2*240.+c(3,5,5,n)*x3y3z3*400.+c(4,2,2,n)*x4*4.+
     & c(4,2,3,n)*x4z1*12.+c(4,2,4,n)*x4z2*24.+c(4,2,5,n)*x4z3*40.+c(
     & 4,3,2,n)*x4y1*12.+c(4,3,3,n)*x4y1z1*36.+c(4,3,4,n)*x4y1z2*72.+
     & c(4,3,5,n)*x4y1z3*120.+c(4,4,2,n)*x4y2*24.+c(4,4,3,n)*x4y2z1*
     & 72.+c(4,4,4,n)*x4y2z2*144.+c(4,4,5,n)*x4y2z3*240.+c(4,5,2,n)*
     & x4y3*40.+c(4,5,3,n)*x4y3z1*120.+c(4,5,4,n)*x4y3z2*240.+c(4,5,5,
     & n)*x4y3z3*400.+c(5,2,2,n)*x5*4.+c(5,2,3,n)*x5z1*12.+c(5,2,4,n)*
     & x5z2*24.+c(5,2,5,n)*x5z3*40.+c(5,3,2,n)*x5y1*12.+c(5,3,3,n)*
     & x5y1z1*36.+c(5,3,4,n)*x5y1z2*72.+c(5,3,5,n)*x5y1z3*120.+c(5,4,
     & 2,n)*x5y2*24.+c(5,4,3,n)*x5y2z1*72.+c(5,4,4,n)*x5y2z2*144.+c(5,
     & 4,5,n)*x5y2z3*240.+c(5,5,2,n)*x5y3*40.+c(5,5,3,n)*x5y3z1*120.+
     & c(5,5,4,n)*x5y3z2*240.+c(5,5,5,n)*x5y3z3*400.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.0.and.dy.eq.2.and.dz.eq.3 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x3y3z2=x2y3z2*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4z2=x3z2*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y1z2=x3y1z2*x1
      x4y2=x3y2*x1
      x4y2z1=x3y2z1*x1
      x4y2z2=x3y2z2*x1
      x4y3=x3y3*x1
      x4y3z1=x3y3z1*x1
      x4y3z2=x3y3z2*x1
      x5=x4*x1
      x5z1=x4z1*x1
      x5z2=x4z2*x1
      x5y1=x4y1*x1
      x5y1z1=x4y1z1*x1
      x5y1z2=x4y1z2*x1
      x5y2=x4y2*x1
      x5y2z1=x4y2z1*x1
      x5y2z2=x4y2z2*x1
      x5y3=x4y3*x1
      x5y3z1=x4y3z1*x1
      x5y3z2=x4y3z2*x1
      r(i1,i2,i3,n)=(c(0,2,3,n)*12.+c(0,2,4,n)*z1*48.+c(0,2,5,n)*z2*
     & 120.+c(0,3,3,n)*y1*36.+c(0,3,4,n)*y1z1*144.+c(0,3,5,n)*y1z2*
     & 360.+c(0,4,3,n)*y2*72.+c(0,4,4,n)*y2z1*288.+c(0,4,5,n)*y2z2*
     & 720.+c(0,5,3,n)*y3*120.+c(0,5,4,n)*y3z1*480.+c(0,5,5,n)*y3z2*
     & 1200.+c(1,2,3,n)*x1*12.+c(1,2,4,n)*x1z1*48.+c(1,2,5,n)*x1z2*
     & 120.+c(1,3,3,n)*x1y1*36.+c(1,3,4,n)*x1y1z1*144.+c(1,3,5,n)*
     & x1y1z2*360.+c(1,4,3,n)*x1y2*72.+c(1,4,4,n)*x1y2z1*288.+c(1,4,5,
     & n)*x1y2z2*720.+c(1,5,3,n)*x1y3*120.+c(1,5,4,n)*x1y3z1*480.+c(1,
     & 5,5,n)*x1y3z2*1200.+c(2,2,3,n)*x2*12.+c(2,2,4,n)*x2z1*48.+c(2,
     & 2,5,n)*x2z2*120.+c(2,3,3,n)*x2y1*36.+c(2,3,4,n)*x2y1z1*144.+c(
     & 2,3,5,n)*x2y1z2*360.+c(2,4,3,n)*x2y2*72.+c(2,4,4,n)*x2y2z1*
     & 288.+c(2,4,5,n)*x2y2z2*720.+c(2,5,3,n)*x2y3*120.+c(2,5,4,n)*
     & x2y3z1*480.+c(2,5,5,n)*x2y3z2*1200.+c(3,2,3,n)*x3*12.+c(3,2,4,
     & n)*x3z1*48.+c(3,2,5,n)*x3z2*120.+c(3,3,3,n)*x3y1*36.+c(3,3,4,n)
     & *x3y1z1*144.+c(3,3,5,n)*x3y1z2*360.+c(3,4,3,n)*x3y2*72.+c(3,4,
     & 4,n)*x3y2z1*288.+c(3,4,5,n)*x3y2z2*720.+c(3,5,3,n)*x3y3*120.+c(
     & 3,5,4,n)*x3y3z1*480.+c(3,5,5,n)*x3y3z2*1200.+c(4,2,3,n)*x4*12.+
     & c(4,2,4,n)*x4z1*48.+c(4,2,5,n)*x4z2*120.+c(4,3,3,n)*x4y1*36.+c(
     & 4,3,4,n)*x4y1z1*144.+c(4,3,5,n)*x4y1z2*360.+c(4,4,3,n)*x4y2*
     & 72.+c(4,4,4,n)*x4y2z1*288.+c(4,4,5,n)*x4y2z2*720.+c(4,5,3,n)*
     & x4y3*120.+c(4,5,4,n)*x4y3z1*480.+c(4,5,5,n)*x4y3z2*1200.+c(5,2,
     & 3,n)*x5*12.+c(5,2,4,n)*x5z1*48.+c(5,2,5,n)*x5z2*120.+c(5,3,3,n)
     & *x5y1*36.+c(5,3,4,n)*x5y1z1*144.+c(5,3,5,n)*x5y1z2*360.+c(5,4,
     & 3,n)*x5y2*72.+c(5,4,4,n)*x5y2z1*288.+c(5,4,5,n)*x5y2z2*720.+c(
     & 5,5,3,n)*x5y3*120.+c(5,5,4,n)*x5y3z1*480.+c(5,5,5,n)*x5y3z2*
     & 1200.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.0.and.dy.eq.2.and.dz.eq.4 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y3=y2*y1
      y3z1=y2z1*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y2=x3y2*x1
      x4y2z1=x3y2z1*x1
      x4y3=x3y3*x1
      x4y3z1=x3y3z1*x1
      x5=x4*x1
      x5z1=x4z1*x1
      x5y1=x4y1*x1
      x5y1z1=x4y1z1*x1
      x5y2=x4y2*x1
      x5y2z1=x4y2z1*x1
      x5y3=x4y3*x1
      x5y3z1=x4y3z1*x1
      r(i1,i2,i3,n)=(c(0,2,4,n)*48.+c(0,2,5,n)*z1*240.+c(0,3,4,n)*y1*
     & 144.+c(0,3,5,n)*y1z1*720.+c(0,4,4,n)*y2*288.+c(0,4,5,n)*y2z1*
     & 1440.+c(0,5,4,n)*y3*480.+c(0,5,5,n)*y3z1*2400.+c(1,2,4,n)*x1*
     & 48.+c(1,2,5,n)*x1z1*240.+c(1,3,4,n)*x1y1*144.+c(1,3,5,n)*
     & x1y1z1*720.+c(1,4,4,n)*x1y2*288.+c(1,4,5,n)*x1y2z1*1440.+c(1,5,
     & 4,n)*x1y3*480.+c(1,5,5,n)*x1y3z1*2400.+c(2,2,4,n)*x2*48.+c(2,2,
     & 5,n)*x2z1*240.+c(2,3,4,n)*x2y1*144.+c(2,3,5,n)*x2y1z1*720.+c(2,
     & 4,4,n)*x2y2*288.+c(2,4,5,n)*x2y2z1*1440.+c(2,5,4,n)*x2y3*480.+
     & c(2,5,5,n)*x2y3z1*2400.+c(3,2,4,n)*x3*48.+c(3,2,5,n)*x3z1*240.+
     & c(3,3,4,n)*x3y1*144.+c(3,3,5,n)*x3y1z1*720.+c(3,4,4,n)*x3y2*
     & 288.+c(3,4,5,n)*x3y2z1*1440.+c(3,5,4,n)*x3y3*480.+c(3,5,5,n)*
     & x3y3z1*2400.+c(4,2,4,n)*x4*48.+c(4,2,5,n)*x4z1*240.+c(4,3,4,n)*
     & x4y1*144.+c(4,3,5,n)*x4y1z1*720.+c(4,4,4,n)*x4y2*288.+c(4,4,5,
     & n)*x4y2z1*1440.+c(4,5,4,n)*x4y3*480.+c(4,5,5,n)*x4y3z1*2400.+c(
     & 5,2,4,n)*x5*48.+c(5,2,5,n)*x5z1*240.+c(5,3,4,n)*x5y1*144.+c(5,
     & 3,5,n)*x5y1z1*720.+c(5,4,4,n)*x5y2*288.+c(5,4,5,n)*x5y2z1*
     & 1440.+c(5,5,4,n)*x5y3*480.+c(5,5,5,n)*x5y3z1*2400.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.0.and.dy.eq.3.and.dz.eq.0 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y1z5=y1z4*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
      y2z5=y1z5*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1z5=x1z4*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y1z5=x1y1z4*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y2z4=x1y1z4*y1
      x1y2z5=x1y1z5*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x2z5=x1z5*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y1z4=x1y1z4*x1
      x2y1z5=x1y1z5*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y2z4=x1y2z4*x1
      x2y2z5=x1y2z5*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3z4=x2z4*x1
      x3z5=x2z5*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x3y1z4=x2y1z4*x1
      x3y1z5=x2y1z5*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y2z3=x2y2z3*x1
      x3y2z4=x2y2z4*x1
      x3y2z5=x2y2z5*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4z2=x3z2*x1
      x4z3=x3z3*x1
      x4z4=x3z4*x1
      x4z5=x3z5*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y1z2=x3y1z2*x1
      x4y1z3=x3y1z3*x1
      x4y1z4=x3y1z4*x1
      x4y1z5=x3y1z5*x1
      x4y2=x3y2*x1
      x4y2z1=x3y2z1*x1
      x4y2z2=x3y2z2*x1
      x4y2z3=x3y2z3*x1
      x4y2z4=x3y2z4*x1
      x4y2z5=x3y2z5*x1
      x5=x4*x1
      x5z1=x4z1*x1
      x5z2=x4z2*x1
      x5z3=x4z3*x1
      x5z4=x4z4*x1
      x5z5=x4z5*x1
      x5y1=x4y1*x1
      x5y1z1=x4y1z1*x1
      x5y1z2=x4y1z2*x1
      x5y1z3=x4y1z3*x1
      x5y1z4=x4y1z4*x1
      x5y1z5=x4y1z5*x1
      x5y2=x4y2*x1
      x5y2z1=x4y2z1*x1
      x5y2z2=x4y2z2*x1
      x5y2z3=x4y2z3*x1
      x5y2z4=x4y2z4*x1
      x5y2z5=x4y2z5*x1
      r(i1,i2,i3,n)=(c(0,3,0,n)*6.+c(0,3,1,n)*z1*6.+c(0,3,2,n)*z2*6.+c(
     & 0,3,3,n)*z3*6.+c(0,3,4,n)*z4*6.+c(0,3,5,n)*z5*6.+c(0,4,0,n)*y1*
     & 24.+c(0,4,1,n)*y1z1*24.+c(0,4,2,n)*y1z2*24.+c(0,4,3,n)*y1z3*
     & 24.+c(0,4,4,n)*y1z4*24.+c(0,4,5,n)*y1z5*24.+c(0,5,0,n)*y2*60.+
     & c(0,5,1,n)*y2z1*60.+c(0,5,2,n)*y2z2*60.+c(0,5,3,n)*y2z3*60.+c(
     & 0,5,4,n)*y2z4*60.+c(0,5,5,n)*y2z5*60.+c(1,3,0,n)*x1*6.+c(1,3,1,
     & n)*x1z1*6.+c(1,3,2,n)*x1z2*6.+c(1,3,3,n)*x1z3*6.+c(1,3,4,n)*
     & x1z4*6.+c(1,3,5,n)*x1z5*6.+c(1,4,0,n)*x1y1*24.+c(1,4,1,n)*
     & x1y1z1*24.+c(1,4,2,n)*x1y1z2*24.+c(1,4,3,n)*x1y1z3*24.+c(1,4,4,
     & n)*x1y1z4*24.+c(1,4,5,n)*x1y1z5*24.+c(1,5,0,n)*x1y2*60.+c(1,5,
     & 1,n)*x1y2z1*60.+c(1,5,2,n)*x1y2z2*60.+c(1,5,3,n)*x1y2z3*60.+c(
     & 1,5,4,n)*x1y2z4*60.+c(1,5,5,n)*x1y2z5*60.+c(2,3,0,n)*x2*6.+c(2,
     & 3,1,n)*x2z1*6.+c(2,3,2,n)*x2z2*6.+c(2,3,3,n)*x2z3*6.+c(2,3,4,n)
     & *x2z4*6.+c(2,3,5,n)*x2z5*6.+c(2,4,0,n)*x2y1*24.+c(2,4,1,n)*
     & x2y1z1*24.+c(2,4,2,n)*x2y1z2*24.+c(2,4,3,n)*x2y1z3*24.+c(2,4,4,
     & n)*x2y1z4*24.+c(2,4,5,n)*x2y1z5*24.+c(2,5,0,n)*x2y2*60.+c(2,5,
     & 1,n)*x2y2z1*60.+c(2,5,2,n)*x2y2z2*60.+c(2,5,3,n)*x2y2z3*60.+c(
     & 2,5,4,n)*x2y2z4*60.+c(2,5,5,n)*x2y2z5*60.+c(3,3,0,n)*x3*6.+c(3,
     & 3,1,n)*x3z1*6.+c(3,3,2,n)*x3z2*6.+c(3,3,3,n)*x3z3*6.+c(3,3,4,n)
     & *x3z4*6.+c(3,3,5,n)*x3z5*6.+c(3,4,0,n)*x3y1*24.+c(3,4,1,n)*
     & x3y1z1*24.+c(3,4,2,n)*x3y1z2*24.+c(3,4,3,n)*x3y1z3*24.+c(3,4,4,
     & n)*x3y1z4*24.+c(3,4,5,n)*x3y1z5*24.+c(3,5,0,n)*x3y2*60.+c(3,5,
     & 1,n)*x3y2z1*60.+c(3,5,2,n)*x3y2z2*60.+c(3,5,3,n)*x3y2z3*60.+c(
     & 3,5,4,n)*x3y2z4*60.+c(3,5,5,n)*x3y2z5*60.+c(4,3,0,n)*x4*6.+c(4,
     & 3,1,n)*x4z1*6.+c(4,3,2,n)*x4z2*6.+c(4,3,3,n)*x4z3*6.+c(4,3,4,n)
     & *x4z4*6.+c(4,3,5,n)*x4z5*6.+c(4,4,0,n)*x4y1*24.+c(4,4,1,n)*
     & x4y1z1*24.+c(4,4,2,n)*x4y1z2*24.+c(4,4,3,n)*x4y1z3*24.+c(4,4,4,
     & n)*x4y1z4*24.+c(4,4,5,n)*x4y1z5*24.+c(4,5,0,n)*x4y2*60.+c(4,5,
     & 1,n)*x4y2z1*60.+c(4,5,2,n)*x4y2z2*60.+c(4,5,3,n)*x4y2z3*60.+c(
     & 4,5,4,n)*x4y2z4*60.+c(4,5,5,n)*x4y2z5*60.+c(5,3,0,n)*x5*6.+c(5,
     & 3,1,n)*x5z1*6.+c(5,3,2,n)*x5z2*6.+c(5,3,3,n)*x5z3*6.+c(5,3,4,n)
     & *x5z4*6.+c(5,3,5,n)*x5z5*6.+c(5,4,0,n)*x5y1*24.+c(5,4,1,n)*
     & x5y1z1*24.+c(5,4,2,n)*x5y1z2*24.+c(5,4,3,n)*x5y1z3*24.+c(5,4,4,
     & n)*x5y1z4*24.+c(5,4,5,n)*x5y1z5*24.+c(5,5,0,n)*x5y2*60.+c(5,5,
     & 1,n)*x5y2z1*60.+c(5,5,2,n)*x5y2z2*60.+c(5,5,3,n)*x5y2z3*60.+c(
     & 5,5,4,n)*x5y2z4*60.+c(5,5,5,n)*x5y2z5*60.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.0.and.dy.eq.3.and.dz.eq.1 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y2z4=x1y1z4*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y1z4=x1y1z4*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y2z4=x1y2z4*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3z4=x2z4*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x3y1z4=x2y1z4*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y2z3=x2y2z3*x1
      x3y2z4=x2y2z4*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4z2=x3z2*x1
      x4z3=x3z3*x1
      x4z4=x3z4*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y1z2=x3y1z2*x1
      x4y1z3=x3y1z3*x1
      x4y1z4=x3y1z4*x1
      x4y2=x3y2*x1
      x4y2z1=x3y2z1*x1
      x4y2z2=x3y2z2*x1
      x4y2z3=x3y2z3*x1
      x4y2z4=x3y2z4*x1
      x5=x4*x1
      x5z1=x4z1*x1
      x5z2=x4z2*x1
      x5z3=x4z3*x1
      x5z4=x4z4*x1
      x5y1=x4y1*x1
      x5y1z1=x4y1z1*x1
      x5y1z2=x4y1z2*x1
      x5y1z3=x4y1z3*x1
      x5y1z4=x4y1z4*x1
      x5y2=x4y2*x1
      x5y2z1=x4y2z1*x1
      x5y2z2=x4y2z2*x1
      x5y2z3=x4y2z3*x1
      x5y2z4=x4y2z4*x1
      r(i1,i2,i3,n)=(c(0,3,1,n)*6.+c(0,3,2,n)*z1*12.+c(0,3,3,n)*z2*18.+
     & c(0,3,4,n)*z3*24.+c(0,3,5,n)*z4*30.+c(0,4,1,n)*y1*24.+c(0,4,2,
     & n)*y1z1*48.+c(0,4,3,n)*y1z2*72.+c(0,4,4,n)*y1z3*96.+c(0,4,5,n)*
     & y1z4*120.+c(0,5,1,n)*y2*60.+c(0,5,2,n)*y2z1*120.+c(0,5,3,n)*
     & y2z2*180.+c(0,5,4,n)*y2z3*240.+c(0,5,5,n)*y2z4*300.+c(1,3,1,n)*
     & x1*6.+c(1,3,2,n)*x1z1*12.+c(1,3,3,n)*x1z2*18.+c(1,3,4,n)*x1z3*
     & 24.+c(1,3,5,n)*x1z4*30.+c(1,4,1,n)*x1y1*24.+c(1,4,2,n)*x1y1z1*
     & 48.+c(1,4,3,n)*x1y1z2*72.+c(1,4,4,n)*x1y1z3*96.+c(1,4,5,n)*
     & x1y1z4*120.+c(1,5,1,n)*x1y2*60.+c(1,5,2,n)*x1y2z1*120.+c(1,5,3,
     & n)*x1y2z2*180.+c(1,5,4,n)*x1y2z3*240.+c(1,5,5,n)*x1y2z4*300.+c(
     & 2,3,1,n)*x2*6.+c(2,3,2,n)*x2z1*12.+c(2,3,3,n)*x2z2*18.+c(2,3,4,
     & n)*x2z3*24.+c(2,3,5,n)*x2z4*30.+c(2,4,1,n)*x2y1*24.+c(2,4,2,n)*
     & x2y1z1*48.+c(2,4,3,n)*x2y1z2*72.+c(2,4,4,n)*x2y1z3*96.+c(2,4,5,
     & n)*x2y1z4*120.+c(2,5,1,n)*x2y2*60.+c(2,5,2,n)*x2y2z1*120.+c(2,
     & 5,3,n)*x2y2z2*180.+c(2,5,4,n)*x2y2z3*240.+c(2,5,5,n)*x2y2z4*
     & 300.+c(3,3,1,n)*x3*6.+c(3,3,2,n)*x3z1*12.+c(3,3,3,n)*x3z2*18.+
     & c(3,3,4,n)*x3z3*24.+c(3,3,5,n)*x3z4*30.+c(3,4,1,n)*x3y1*24.+c(
     & 3,4,2,n)*x3y1z1*48.+c(3,4,3,n)*x3y1z2*72.+c(3,4,4,n)*x3y1z3*
     & 96.+c(3,4,5,n)*x3y1z4*120.+c(3,5,1,n)*x3y2*60.+c(3,5,2,n)*
     & x3y2z1*120.+c(3,5,3,n)*x3y2z2*180.+c(3,5,4,n)*x3y2z3*240.+c(3,
     & 5,5,n)*x3y2z4*300.+c(4,3,1,n)*x4*6.+c(4,3,2,n)*x4z1*12.+c(4,3,
     & 3,n)*x4z2*18.+c(4,3,4,n)*x4z3*24.+c(4,3,5,n)*x4z4*30.+c(4,4,1,
     & n)*x4y1*24.+c(4,4,2,n)*x4y1z1*48.+c(4,4,3,n)*x4y1z2*72.+c(4,4,
     & 4,n)*x4y1z3*96.+c(4,4,5,n)*x4y1z4*120.+c(4,5,1,n)*x4y2*60.+c(4,
     & 5,2,n)*x4y2z1*120.+c(4,5,3,n)*x4y2z2*180.+c(4,5,4,n)*x4y2z3*
     & 240.+c(4,5,5,n)*x4y2z4*300.+c(5,3,1,n)*x5*6.+c(5,3,2,n)*x5z1*
     & 12.+c(5,3,3,n)*x5z2*18.+c(5,3,4,n)*x5z3*24.+c(5,3,5,n)*x5z4*
     & 30.+c(5,4,1,n)*x5y1*24.+c(5,4,2,n)*x5y1z1*48.+c(5,4,3,n)*
     & x5y1z2*72.+c(5,4,4,n)*x5y1z3*96.+c(5,4,5,n)*x5y1z4*120.+c(5,5,
     & 1,n)*x5y2*60.+c(5,5,2,n)*x5y2z1*120.+c(5,5,3,n)*x5y2z2*180.+c(
     & 5,5,4,n)*x5y2z3*240.+c(5,5,5,n)*x5y2z4*300.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.0.and.dy.eq.3.and.dz.eq.2 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y2z3=x2y2z3*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4z2=x3z2*x1
      x4z3=x3z3*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y1z2=x3y1z2*x1
      x4y1z3=x3y1z3*x1
      x4y2=x3y2*x1
      x4y2z1=x3y2z1*x1
      x4y2z2=x3y2z2*x1
      x4y2z3=x3y2z3*x1
      x5=x4*x1
      x5z1=x4z1*x1
      x5z2=x4z2*x1
      x5z3=x4z3*x1
      x5y1=x4y1*x1
      x5y1z1=x4y1z1*x1
      x5y1z2=x4y1z2*x1
      x5y1z3=x4y1z3*x1
      x5y2=x4y2*x1
      x5y2z1=x4y2z1*x1
      x5y2z2=x4y2z2*x1
      x5y2z3=x4y2z3*x1
      r(i1,i2,i3,n)=(c(0,3,2,n)*12.+c(0,3,3,n)*z1*36.+c(0,3,4,n)*z2*
     & 72.+c(0,3,5,n)*z3*120.+c(0,4,2,n)*y1*48.+c(0,4,3,n)*y1z1*144.+
     & c(0,4,4,n)*y1z2*288.+c(0,4,5,n)*y1z3*480.+c(0,5,2,n)*y2*120.+c(
     & 0,5,3,n)*y2z1*360.+c(0,5,4,n)*y2z2*720.+c(0,5,5,n)*y2z3*1200.+
     & c(1,3,2,n)*x1*12.+c(1,3,3,n)*x1z1*36.+c(1,3,4,n)*x1z2*72.+c(1,
     & 3,5,n)*x1z3*120.+c(1,4,2,n)*x1y1*48.+c(1,4,3,n)*x1y1z1*144.+c(
     & 1,4,4,n)*x1y1z2*288.+c(1,4,5,n)*x1y1z3*480.+c(1,5,2,n)*x1y2*
     & 120.+c(1,5,3,n)*x1y2z1*360.+c(1,5,4,n)*x1y2z2*720.+c(1,5,5,n)*
     & x1y2z3*1200.+c(2,3,2,n)*x2*12.+c(2,3,3,n)*x2z1*36.+c(2,3,4,n)*
     & x2z2*72.+c(2,3,5,n)*x2z3*120.+c(2,4,2,n)*x2y1*48.+c(2,4,3,n)*
     & x2y1z1*144.+c(2,4,4,n)*x2y1z2*288.+c(2,4,5,n)*x2y1z3*480.+c(2,
     & 5,2,n)*x2y2*120.+c(2,5,3,n)*x2y2z1*360.+c(2,5,4,n)*x2y2z2*720.+
     & c(2,5,5,n)*x2y2z3*1200.+c(3,3,2,n)*x3*12.+c(3,3,3,n)*x3z1*36.+
     & c(3,3,4,n)*x3z2*72.+c(3,3,5,n)*x3z3*120.+c(3,4,2,n)*x3y1*48.+c(
     & 3,4,3,n)*x3y1z1*144.+c(3,4,4,n)*x3y1z2*288.+c(3,4,5,n)*x3y1z3*
     & 480.+c(3,5,2,n)*x3y2*120.+c(3,5,3,n)*x3y2z1*360.+c(3,5,4,n)*
     & x3y2z2*720.+c(3,5,5,n)*x3y2z3*1200.+c(4,3,2,n)*x4*12.+c(4,3,3,
     & n)*x4z1*36.+c(4,3,4,n)*x4z2*72.+c(4,3,5,n)*x4z3*120.+c(4,4,2,n)
     & *x4y1*48.+c(4,4,3,n)*x4y1z1*144.+c(4,4,4,n)*x4y1z2*288.+c(4,4,
     & 5,n)*x4y1z3*480.+c(4,5,2,n)*x4y2*120.+c(4,5,3,n)*x4y2z1*360.+c(
     & 4,5,4,n)*x4y2z2*720.+c(4,5,5,n)*x4y2z3*1200.+c(5,3,2,n)*x5*12.+
     & c(5,3,3,n)*x5z1*36.+c(5,3,4,n)*x5z2*72.+c(5,3,5,n)*x5z3*120.+c(
     & 5,4,2,n)*x5y1*48.+c(5,4,3,n)*x5y1z1*144.+c(5,4,4,n)*x5y1z2*
     & 288.+c(5,4,5,n)*x5y1z3*480.+c(5,5,2,n)*x5y2*120.+c(5,5,3,n)*
     & x5y2z1*360.+c(5,5,4,n)*x5y2z2*720.+c(5,5,5,n)*x5y2z3*1200.)*
     & time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.0.and.dy.eq.3.and.dz.eq.3 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4z2=x3z2*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y1z2=x3y1z2*x1
      x4y2=x3y2*x1
      x4y2z1=x3y2z1*x1
      x4y2z2=x3y2z2*x1
      x5=x4*x1
      x5z1=x4z1*x1
      x5z2=x4z2*x1
      x5y1=x4y1*x1
      x5y1z1=x4y1z1*x1
      x5y1z2=x4y1z2*x1
      x5y2=x4y2*x1
      x5y2z1=x4y2z1*x1
      x5y2z2=x4y2z2*x1
      r(i1,i2,i3,n)=(c(0,3,3,n)*36.+c(0,3,4,n)*z1*144.+c(0,3,5,n)*z2*
     & 360.+c(0,4,3,n)*y1*144.+c(0,4,4,n)*y1z1*576.+c(0,4,5,n)*y1z2*
     & 1440.+c(0,5,3,n)*y2*360.+c(0,5,4,n)*y2z1*1440.+c(0,5,5,n)*y2z2*
     & 3600.+c(1,3,3,n)*x1*36.+c(1,3,4,n)*x1z1*144.+c(1,3,5,n)*x1z2*
     & 360.+c(1,4,3,n)*x1y1*144.+c(1,4,4,n)*x1y1z1*576.+c(1,4,5,n)*
     & x1y1z2*1440.+c(1,5,3,n)*x1y2*360.+c(1,5,4,n)*x1y2z1*1440.+c(1,
     & 5,5,n)*x1y2z2*3600.+c(2,3,3,n)*x2*36.+c(2,3,4,n)*x2z1*144.+c(2,
     & 3,5,n)*x2z2*360.+c(2,4,3,n)*x2y1*144.+c(2,4,4,n)*x2y1z1*576.+c(
     & 2,4,5,n)*x2y1z2*1440.+c(2,5,3,n)*x2y2*360.+c(2,5,4,n)*x2y2z1*
     & 1440.+c(2,5,5,n)*x2y2z2*3600.+c(3,3,3,n)*x3*36.+c(3,3,4,n)*
     & x3z1*144.+c(3,3,5,n)*x3z2*360.+c(3,4,3,n)*x3y1*144.+c(3,4,4,n)*
     & x3y1z1*576.+c(3,4,5,n)*x3y1z2*1440.+c(3,5,3,n)*x3y2*360.+c(3,5,
     & 4,n)*x3y2z1*1440.+c(3,5,5,n)*x3y2z2*3600.+c(4,3,3,n)*x4*36.+c(
     & 4,3,4,n)*x4z1*144.+c(4,3,5,n)*x4z2*360.+c(4,4,3,n)*x4y1*144.+c(
     & 4,4,4,n)*x4y1z1*576.+c(4,4,5,n)*x4y1z2*1440.+c(4,5,3,n)*x4y2*
     & 360.+c(4,5,4,n)*x4y2z1*1440.+c(4,5,5,n)*x4y2z2*3600.+c(5,3,3,n)
     & *x5*36.+c(5,3,4,n)*x5z1*144.+c(5,3,5,n)*x5z2*360.+c(5,4,3,n)*
     & x5y1*144.+c(5,4,4,n)*x5y1z1*576.+c(5,4,5,n)*x5y1z2*1440.+c(5,5,
     & 3,n)*x5y2*360.+c(5,5,4,n)*x5y2z1*1440.+c(5,5,5,n)*x5y2z2*3600.)
     & *time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.0.and.dy.eq.3.and.dz.eq.4 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y2=x3y2*x1
      x4y2z1=x3y2z1*x1
      x5=x4*x1
      x5z1=x4z1*x1
      x5y1=x4y1*x1
      x5y1z1=x4y1z1*x1
      x5y2=x4y2*x1
      x5y2z1=x4y2z1*x1
      r(i1,i2,i3,n)=(c(0,3,4,n)*144.+c(0,3,5,n)*z1*720.+c(0,4,4,n)*y1*
     & 576.+c(0,4,5,n)*y1z1*2880.+c(0,5,4,n)*y2*1440.+c(0,5,5,n)*y2z1*
     & 7200.+c(1,3,4,n)*x1*144.+c(1,3,5,n)*x1z1*720.+c(1,4,4,n)*x1y1*
     & 576.+c(1,4,5,n)*x1y1z1*2880.+c(1,5,4,n)*x1y2*1440.+c(1,5,5,n)*
     & x1y2z1*7200.+c(2,3,4,n)*x2*144.+c(2,3,5,n)*x2z1*720.+c(2,4,4,n)
     & *x2y1*576.+c(2,4,5,n)*x2y1z1*2880.+c(2,5,4,n)*x2y2*1440.+c(2,5,
     & 5,n)*x2y2z1*7200.+c(3,3,4,n)*x3*144.+c(3,3,5,n)*x3z1*720.+c(3,
     & 4,4,n)*x3y1*576.+c(3,4,5,n)*x3y1z1*2880.+c(3,5,4,n)*x3y2*1440.+
     & c(3,5,5,n)*x3y2z1*7200.+c(4,3,4,n)*x4*144.+c(4,3,5,n)*x4z1*
     & 720.+c(4,4,4,n)*x4y1*576.+c(4,4,5,n)*x4y1z1*2880.+c(4,5,4,n)*
     & x4y2*1440.+c(4,5,5,n)*x4y2z1*7200.+c(5,3,4,n)*x5*144.+c(5,3,5,
     & n)*x5z1*720.+c(5,4,4,n)*x5y1*576.+c(5,4,5,n)*x5y1z1*2880.+c(5,
     & 5,4,n)*x5y2*1440.+c(5,5,5,n)*x5y2z1*7200.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.0.and.dy.eq.4.and.dz.eq.0 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y1z5=y1z4*z1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1z5=x1z4*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y1z5=x1y1z4*z1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x2z5=x1z5*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y1z4=x1y1z4*x1
      x2y1z5=x1y1z5*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3z4=x2z4*x1
      x3z5=x2z5*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x3y1z4=x2y1z4*x1
      x3y1z5=x2y1z5*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4z2=x3z2*x1
      x4z3=x3z3*x1
      x4z4=x3z4*x1
      x4z5=x3z5*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y1z2=x3y1z2*x1
      x4y1z3=x3y1z3*x1
      x4y1z4=x3y1z4*x1
      x4y1z5=x3y1z5*x1
      x5=x4*x1
      x5z1=x4z1*x1
      x5z2=x4z2*x1
      x5z3=x4z3*x1
      x5z4=x4z4*x1
      x5z5=x4z5*x1
      x5y1=x4y1*x1
      x5y1z1=x4y1z1*x1
      x5y1z2=x4y1z2*x1
      x5y1z3=x4y1z3*x1
      x5y1z4=x4y1z4*x1
      x5y1z5=x4y1z5*x1
      r(i1,i2,i3,n)=(c(0,4,0,n)*24.+c(0,4,1,n)*z1*24.+c(0,4,2,n)*z2*
     & 24.+c(0,4,3,n)*z3*24.+c(0,4,4,n)*z4*24.+c(0,4,5,n)*z5*24.+c(0,
     & 5,0,n)*y1*120.+c(0,5,1,n)*y1z1*120.+c(0,5,2,n)*y1z2*120.+c(0,5,
     & 3,n)*y1z3*120.+c(0,5,4,n)*y1z4*120.+c(0,5,5,n)*y1z5*120.+c(1,4,
     & 0,n)*x1*24.+c(1,4,1,n)*x1z1*24.+c(1,4,2,n)*x1z2*24.+c(1,4,3,n)*
     & x1z3*24.+c(1,4,4,n)*x1z4*24.+c(1,4,5,n)*x1z5*24.+c(1,5,0,n)*
     & x1y1*120.+c(1,5,1,n)*x1y1z1*120.+c(1,5,2,n)*x1y1z2*120.+c(1,5,
     & 3,n)*x1y1z3*120.+c(1,5,4,n)*x1y1z4*120.+c(1,5,5,n)*x1y1z5*120.+
     & c(2,4,0,n)*x2*24.+c(2,4,1,n)*x2z1*24.+c(2,4,2,n)*x2z2*24.+c(2,
     & 4,3,n)*x2z3*24.+c(2,4,4,n)*x2z4*24.+c(2,4,5,n)*x2z5*24.+c(2,5,
     & 0,n)*x2y1*120.+c(2,5,1,n)*x2y1z1*120.+c(2,5,2,n)*x2y1z2*120.+c(
     & 2,5,3,n)*x2y1z3*120.+c(2,5,4,n)*x2y1z4*120.+c(2,5,5,n)*x2y1z5*
     & 120.+c(3,4,0,n)*x3*24.+c(3,4,1,n)*x3z1*24.+c(3,4,2,n)*x3z2*24.+
     & c(3,4,3,n)*x3z3*24.+c(3,4,4,n)*x3z4*24.+c(3,4,5,n)*x3z5*24.+c(
     & 3,5,0,n)*x3y1*120.+c(3,5,1,n)*x3y1z1*120.+c(3,5,2,n)*x3y1z2*
     & 120.+c(3,5,3,n)*x3y1z3*120.+c(3,5,4,n)*x3y1z4*120.+c(3,5,5,n)*
     & x3y1z5*120.+c(4,4,0,n)*x4*24.+c(4,4,1,n)*x4z1*24.+c(4,4,2,n)*
     & x4z2*24.+c(4,4,3,n)*x4z3*24.+c(4,4,4,n)*x4z4*24.+c(4,4,5,n)*
     & x4z5*24.+c(4,5,0,n)*x4y1*120.+c(4,5,1,n)*x4y1z1*120.+c(4,5,2,n)
     & *x4y1z2*120.+c(4,5,3,n)*x4y1z3*120.+c(4,5,4,n)*x4y1z4*120.+c(4,
     & 5,5,n)*x4y1z5*120.+c(5,4,0,n)*x5*24.+c(5,4,1,n)*x5z1*24.+c(5,4,
     & 2,n)*x5z2*24.+c(5,4,3,n)*x5z3*24.+c(5,4,4,n)*x5z4*24.+c(5,4,5,
     & n)*x5z5*24.+c(5,5,0,n)*x5y1*120.+c(5,5,1,n)*x5y1z1*120.+c(5,5,
     & 2,n)*x5y1z2*120.+c(5,5,3,n)*x5y1z3*120.+c(5,5,4,n)*x5y1z4*120.+
     & c(5,5,5,n)*x5y1z5*120.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.0.and.dy.eq.4.and.dz.eq.1 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y1z4=x1y1z4*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3z4=x2z4*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x3y1z4=x2y1z4*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4z2=x3z2*x1
      x4z3=x3z3*x1
      x4z4=x3z4*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y1z2=x3y1z2*x1
      x4y1z3=x3y1z3*x1
      x4y1z4=x3y1z4*x1
      x5=x4*x1
      x5z1=x4z1*x1
      x5z2=x4z2*x1
      x5z3=x4z3*x1
      x5z4=x4z4*x1
      x5y1=x4y1*x1
      x5y1z1=x4y1z1*x1
      x5y1z2=x4y1z2*x1
      x5y1z3=x4y1z3*x1
      x5y1z4=x4y1z4*x1
      r(i1,i2,i3,n)=(c(0,4,1,n)*24.+c(0,4,2,n)*z1*48.+c(0,4,3,n)*z2*
     & 72.+c(0,4,4,n)*z3*96.+c(0,4,5,n)*z4*120.+c(0,5,1,n)*y1*120.+c(
     & 0,5,2,n)*y1z1*240.+c(0,5,3,n)*y1z2*360.+c(0,5,4,n)*y1z3*480.+c(
     & 0,5,5,n)*y1z4*600.+c(1,4,1,n)*x1*24.+c(1,4,2,n)*x1z1*48.+c(1,4,
     & 3,n)*x1z2*72.+c(1,4,4,n)*x1z3*96.+c(1,4,5,n)*x1z4*120.+c(1,5,1,
     & n)*x1y1*120.+c(1,5,2,n)*x1y1z1*240.+c(1,5,3,n)*x1y1z2*360.+c(1,
     & 5,4,n)*x1y1z3*480.+c(1,5,5,n)*x1y1z4*600.+c(2,4,1,n)*x2*24.+c(
     & 2,4,2,n)*x2z1*48.+c(2,4,3,n)*x2z2*72.+c(2,4,4,n)*x2z3*96.+c(2,
     & 4,5,n)*x2z4*120.+c(2,5,1,n)*x2y1*120.+c(2,5,2,n)*x2y1z1*240.+c(
     & 2,5,3,n)*x2y1z2*360.+c(2,5,4,n)*x2y1z3*480.+c(2,5,5,n)*x2y1z4*
     & 600.+c(3,4,1,n)*x3*24.+c(3,4,2,n)*x3z1*48.+c(3,4,3,n)*x3z2*72.+
     & c(3,4,4,n)*x3z3*96.+c(3,4,5,n)*x3z4*120.+c(3,5,1,n)*x3y1*120.+
     & c(3,5,2,n)*x3y1z1*240.+c(3,5,3,n)*x3y1z2*360.+c(3,5,4,n)*
     & x3y1z3*480.+c(3,5,5,n)*x3y1z4*600.+c(4,4,1,n)*x4*24.+c(4,4,2,n)
     & *x4z1*48.+c(4,4,3,n)*x4z2*72.+c(4,4,4,n)*x4z3*96.+c(4,4,5,n)*
     & x4z4*120.+c(4,5,1,n)*x4y1*120.+c(4,5,2,n)*x4y1z1*240.+c(4,5,3,
     & n)*x4y1z2*360.+c(4,5,4,n)*x4y1z3*480.+c(4,5,5,n)*x4y1z4*600.+c(
     & 5,4,1,n)*x5*24.+c(5,4,2,n)*x5z1*48.+c(5,4,3,n)*x5z2*72.+c(5,4,
     & 4,n)*x5z3*96.+c(5,4,5,n)*x5z4*120.+c(5,5,1,n)*x5y1*120.+c(5,5,
     & 2,n)*x5y1z1*240.+c(5,5,3,n)*x5y1z2*360.+c(5,5,4,n)*x5y1z3*480.+
     & c(5,5,5,n)*x5y1z4*600.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.0.and.dy.eq.4.and.dz.eq.2 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4z2=x3z2*x1
      x4z3=x3z3*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y1z2=x3y1z2*x1
      x4y1z3=x3y1z3*x1
      x5=x4*x1
      x5z1=x4z1*x1
      x5z2=x4z2*x1
      x5z3=x4z3*x1
      x5y1=x4y1*x1
      x5y1z1=x4y1z1*x1
      x5y1z2=x4y1z2*x1
      x5y1z3=x4y1z3*x1
      r(i1,i2,i3,n)=(c(0,4,2,n)*48.+c(0,4,3,n)*z1*144.+c(0,4,4,n)*z2*
     & 288.+c(0,4,5,n)*z3*480.+c(0,5,2,n)*y1*240.+c(0,5,3,n)*y1z1*
     & 720.+c(0,5,4,n)*y1z2*1440.+c(0,5,5,n)*y1z3*2400.+c(1,4,2,n)*x1*
     & 48.+c(1,4,3,n)*x1z1*144.+c(1,4,4,n)*x1z2*288.+c(1,4,5,n)*x1z3*
     & 480.+c(1,5,2,n)*x1y1*240.+c(1,5,3,n)*x1y1z1*720.+c(1,5,4,n)*
     & x1y1z2*1440.+c(1,5,5,n)*x1y1z3*2400.+c(2,4,2,n)*x2*48.+c(2,4,3,
     & n)*x2z1*144.+c(2,4,4,n)*x2z2*288.+c(2,4,5,n)*x2z3*480.+c(2,5,2,
     & n)*x2y1*240.+c(2,5,3,n)*x2y1z1*720.+c(2,5,4,n)*x2y1z2*1440.+c(
     & 2,5,5,n)*x2y1z3*2400.+c(3,4,2,n)*x3*48.+c(3,4,3,n)*x3z1*144.+c(
     & 3,4,4,n)*x3z2*288.+c(3,4,5,n)*x3z3*480.+c(3,5,2,n)*x3y1*240.+c(
     & 3,5,3,n)*x3y1z1*720.+c(3,5,4,n)*x3y1z2*1440.+c(3,5,5,n)*x3y1z3*
     & 2400.+c(4,4,2,n)*x4*48.+c(4,4,3,n)*x4z1*144.+c(4,4,4,n)*x4z2*
     & 288.+c(4,4,5,n)*x4z3*480.+c(4,5,2,n)*x4y1*240.+c(4,5,3,n)*
     & x4y1z1*720.+c(4,5,4,n)*x4y1z2*1440.+c(4,5,5,n)*x4y1z3*2400.+c(
     & 5,4,2,n)*x5*48.+c(5,4,3,n)*x5z1*144.+c(5,4,4,n)*x5z2*288.+c(5,
     & 4,5,n)*x5z3*480.+c(5,5,2,n)*x5y1*240.+c(5,5,3,n)*x5y1z1*720.+c(
     & 5,5,4,n)*x5y1z2*1440.+c(5,5,5,n)*x5y1z3*2400.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.0.and.dy.eq.4.and.dz.eq.3 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4z2=x3z2*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y1z2=x3y1z2*x1
      x5=x4*x1
      x5z1=x4z1*x1
      x5z2=x4z2*x1
      x5y1=x4y1*x1
      x5y1z1=x4y1z1*x1
      x5y1z2=x4y1z2*x1
      r(i1,i2,i3,n)=(c(0,4,3,n)*144.+c(0,4,4,n)*z1*576.+c(0,4,5,n)*z2*
     & 1440.+c(0,5,3,n)*y1*720.+c(0,5,4,n)*y1z1*2880.+c(0,5,5,n)*y1z2*
     & 7200.+c(1,4,3,n)*x1*144.+c(1,4,4,n)*x1z1*576.+c(1,4,5,n)*x1z2*
     & 1440.+c(1,5,3,n)*x1y1*720.+c(1,5,4,n)*x1y1z1*2880.+c(1,5,5,n)*
     & x1y1z2*7200.+c(2,4,3,n)*x2*144.+c(2,4,4,n)*x2z1*576.+c(2,4,5,n)
     & *x2z2*1440.+c(2,5,3,n)*x2y1*720.+c(2,5,4,n)*x2y1z1*2880.+c(2,5,
     & 5,n)*x2y1z2*7200.+c(3,4,3,n)*x3*144.+c(3,4,4,n)*x3z1*576.+c(3,
     & 4,5,n)*x3z2*1440.+c(3,5,3,n)*x3y1*720.+c(3,5,4,n)*x3y1z1*2880.+
     & c(3,5,5,n)*x3y1z2*7200.+c(4,4,3,n)*x4*144.+c(4,4,4,n)*x4z1*
     & 576.+c(4,4,5,n)*x4z2*1440.+c(4,5,3,n)*x4y1*720.+c(4,5,4,n)*
     & x4y1z1*2880.+c(4,5,5,n)*x4y1z2*7200.+c(5,4,3,n)*x5*144.+c(5,4,
     & 4,n)*x5z1*576.+c(5,4,5,n)*x5z2*1440.+c(5,5,3,n)*x5y1*720.+c(5,
     & 5,4,n)*x5y1z1*2880.+c(5,5,5,n)*x5y1z2*7200.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.0.and.dy.eq.4.and.dz.eq.4 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x2=x1*x1
      x2z1=x1z1*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x5=x4*x1
      x5z1=x4z1*x1
      x5y1=x4y1*x1
      x5y1z1=x4y1z1*x1
      r(i1,i2,i3,n)=(c(0,4,4,n)*576.+c(0,4,5,n)*z1*2880.+c(0,5,4,n)*y1*
     & 2880.+c(0,5,5,n)*y1z1*14400.+c(1,4,4,n)*x1*576.+c(1,4,5,n)*
     & x1z1*2880.+c(1,5,4,n)*x1y1*2880.+c(1,5,5,n)*x1y1z1*14400.+c(2,
     & 4,4,n)*x2*576.+c(2,4,5,n)*x2z1*2880.+c(2,5,4,n)*x2y1*2880.+c(2,
     & 5,5,n)*x2y1z1*14400.+c(3,4,4,n)*x3*576.+c(3,4,5,n)*x3z1*2880.+
     & c(3,5,4,n)*x3y1*2880.+c(3,5,5,n)*x3y1z1*14400.+c(4,4,4,n)*x4*
     & 576.+c(4,4,5,n)*x4z1*2880.+c(4,5,4,n)*x4y1*2880.+c(4,5,5,n)*
     & x4y1z1*14400.+c(5,4,4,n)*x5*576.+c(5,4,5,n)*x5z1*2880.+c(5,5,4,
     & n)*x5y1*2880.+c(5,5,5,n)*x5y1z1*14400.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.1.and.dy.eq.0.and.dz.eq.0 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y1z5=y1z4*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
      y2z5=y1z5*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      y3z4=y2z4*y1
      y3z5=y2z5*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y4z2=y3z2*y1
      y4z3=y3z3*y1
      y4z4=y3z4*y1
      y4z5=y3z5*y1
      y5=y4*y1
      y5z1=y4z1*y1
      y5z2=y4z2*y1
      y5z3=y4z3*y1
      y5z4=y4z4*y1
      y5z5=y4z5*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1z5=x1z4*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y1z5=x1y1z4*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y2z4=x1y1z4*y1
      x1y2z5=x1y1z5*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x1y3z4=x1y2z4*y1
      x1y3z5=x1y2z5*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y4z2=x1y3z2*y1
      x1y4z3=x1y3z3*y1
      x1y4z4=x1y3z4*y1
      x1y4z5=x1y3z5*y1
      x1y5=x1y4*y1
      x1y5z1=x1y4z1*y1
      x1y5z2=x1y4z2*y1
      x1y5z3=x1y4z3*y1
      x1y5z4=x1y4z4*y1
      x1y5z5=x1y4z5*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x2z5=x1z5*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y1z4=x1y1z4*x1
      x2y1z5=x1y1z5*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y2z4=x1y2z4*x1
      x2y2z5=x1y2z5*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y3z3=x1y3z3*x1
      x2y3z4=x1y3z4*x1
      x2y3z5=x1y3z5*x1
      x2y4=x1y4*x1
      x2y4z1=x1y4z1*x1
      x2y4z2=x1y4z2*x1
      x2y4z3=x1y4z3*x1
      x2y4z4=x1y4z4*x1
      x2y4z5=x1y4z5*x1
      x2y5=x1y5*x1
      x2y5z1=x1y5z1*x1
      x2y5z2=x1y5z2*x1
      x2y5z3=x1y5z3*x1
      x2y5z4=x1y5z4*x1
      x2y5z5=x1y5z5*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3z4=x2z4*x1
      x3z5=x2z5*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x3y1z4=x2y1z4*x1
      x3y1z5=x2y1z5*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y2z3=x2y2z3*x1
      x3y2z4=x2y2z4*x1
      x3y2z5=x2y2z5*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x3y3z2=x2y3z2*x1
      x3y3z3=x2y3z3*x1
      x3y3z4=x2y3z4*x1
      x3y3z5=x2y3z5*x1
      x3y4=x2y4*x1
      x3y4z1=x2y4z1*x1
      x3y4z2=x2y4z2*x1
      x3y4z3=x2y4z3*x1
      x3y4z4=x2y4z4*x1
      x3y4z5=x2y4z5*x1
      x3y5=x2y5*x1
      x3y5z1=x2y5z1*x1
      x3y5z2=x2y5z2*x1
      x3y5z3=x2y5z3*x1
      x3y5z4=x2y5z4*x1
      x3y5z5=x2y5z5*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4z2=x3z2*x1
      x4z3=x3z3*x1
      x4z4=x3z4*x1
      x4z5=x3z5*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y1z2=x3y1z2*x1
      x4y1z3=x3y1z3*x1
      x4y1z4=x3y1z4*x1
      x4y1z5=x3y1z5*x1
      x4y2=x3y2*x1
      x4y2z1=x3y2z1*x1
      x4y2z2=x3y2z2*x1
      x4y2z3=x3y2z3*x1
      x4y2z4=x3y2z4*x1
      x4y2z5=x3y2z5*x1
      x4y3=x3y3*x1
      x4y3z1=x3y3z1*x1
      x4y3z2=x3y3z2*x1
      x4y3z3=x3y3z3*x1
      x4y3z4=x3y3z4*x1
      x4y3z5=x3y3z5*x1
      x4y4=x3y4*x1
      x4y4z1=x3y4z1*x1
      x4y4z2=x3y4z2*x1
      x4y4z3=x3y4z3*x1
      x4y4z4=x3y4z4*x1
      x4y4z5=x3y4z5*x1
      x4y5=x3y5*x1
      x4y5z1=x3y5z1*x1
      x4y5z2=x3y5z2*x1
      x4y5z3=x3y5z3*x1
      x4y5z4=x3y5z4*x1
      x4y5z5=x3y5z5*x1
      r(i1,i2,i3,n)=(c(1,0,0,n)+c(1,0,1,n)*z1+c(1,0,2,n)*z2+c(1,0,3,n)*
     & z3+c(1,0,4,n)*z4+c(1,0,5,n)*z5+c(1,1,0,n)*y1+c(1,1,1,n)*y1z1+c(
     & 1,1,2,n)*y1z2+c(1,1,3,n)*y1z3+c(1,1,4,n)*y1z4+c(1,1,5,n)*y1z5+
     & c(1,2,0,n)*y2+c(1,2,1,n)*y2z1+c(1,2,2,n)*y2z2+c(1,2,3,n)*y2z3+
     & c(1,2,4,n)*y2z4+c(1,2,5,n)*y2z5+c(1,3,0,n)*y3+c(1,3,1,n)*y3z1+
     & c(1,3,2,n)*y3z2+c(1,3,3,n)*y3z3+c(1,3,4,n)*y3z4+c(1,3,5,n)*
     & y3z5+c(1,4,0,n)*y4+c(1,4,1,n)*y4z1+c(1,4,2,n)*y4z2+c(1,4,3,n)*
     & y4z3+c(1,4,4,n)*y4z4+c(1,4,5,n)*y4z5+c(1,5,0,n)*y5+c(1,5,1,n)*
     & y5z1+c(1,5,2,n)*y5z2+c(1,5,3,n)*y5z3+c(1,5,4,n)*y5z4+c(1,5,5,n)
     & *y5z5+c(2,0,0,n)*x1*2.+c(2,0,1,n)*x1z1*2.+c(2,0,2,n)*x1z2*2.+c(
     & 2,0,3,n)*x1z3*2.+c(2,0,4,n)*x1z4*2.+c(2,0,5,n)*x1z5*2.+c(2,1,0,
     & n)*x1y1*2.+c(2,1,1,n)*x1y1z1*2.+c(2,1,2,n)*x1y1z2*2.+c(2,1,3,n)
     & *x1y1z3*2.+c(2,1,4,n)*x1y1z4*2.+c(2,1,5,n)*x1y1z5*2.+c(2,2,0,n)
     & *x1y2*2.+c(2,2,1,n)*x1y2z1*2.+c(2,2,2,n)*x1y2z2*2.+c(2,2,3,n)*
     & x1y2z3*2.+c(2,2,4,n)*x1y2z4*2.+c(2,2,5,n)*x1y2z5*2.+c(2,3,0,n)*
     & x1y3*2.+c(2,3,1,n)*x1y3z1*2.+c(2,3,2,n)*x1y3z2*2.+c(2,3,3,n)*
     & x1y3z3*2.+c(2,3,4,n)*x1y3z4*2.+c(2,3,5,n)*x1y3z5*2.+c(2,4,0,n)*
     & x1y4*2.+c(2,4,1,n)*x1y4z1*2.+c(2,4,2,n)*x1y4z2*2.+c(2,4,3,n)*
     & x1y4z3*2.+c(2,4,4,n)*x1y4z4*2.+c(2,4,5,n)*x1y4z5*2.+c(2,5,0,n)*
     & x1y5*2.+c(2,5,1,n)*x1y5z1*2.+c(2,5,2,n)*x1y5z2*2.+c(2,5,3,n)*
     & x1y5z3*2.+c(2,5,4,n)*x1y5z4*2.+c(2,5,5,n)*x1y5z5*2.+c(3,0,0,n)*
     & x2*3.+c(3,0,1,n)*x2z1*3.+c(3,0,2,n)*x2z2*3.+c(3,0,3,n)*x2z3*3.+
     & c(3,0,4,n)*x2z4*3.+c(3,0,5,n)*x2z5*3.+c(3,1,0,n)*x2y1*3.+c(3,1,
     & 1,n)*x2y1z1*3.+c(3,1,2,n)*x2y1z2*3.+c(3,1,3,n)*x2y1z3*3.+c(3,1,
     & 4,n)*x2y1z4*3.+c(3,1,5,n)*x2y1z5*3.+c(3,2,0,n)*x2y2*3.+c(3,2,1,
     & n)*x2y2z1*3.+c(3,2,2,n)*x2y2z2*3.+c(3,2,3,n)*x2y2z3*3.+c(3,2,4,
     & n)*x2y2z4*3.+c(3,2,5,n)*x2y2z5*3.+c(3,3,0,n)*x2y3*3.+c(3,3,1,n)
     & *x2y3z1*3.+c(3,3,2,n)*x2y3z2*3.+c(3,3,3,n)*x2y3z3*3.+c(3,3,4,n)
     & *x2y3z4*3.+c(3,3,5,n)*x2y3z5*3.+c(3,4,0,n)*x2y4*3.+c(3,4,1,n)*
     & x2y4z1*3.+c(3,4,2,n)*x2y4z2*3.+c(3,4,3,n)*x2y4z3*3.+c(3,4,4,n)*
     & x2y4z4*3.+c(3,4,5,n)*x2y4z5*3.+c(3,5,0,n)*x2y5*3.+c(3,5,1,n)*
     & x2y5z1*3.+c(3,5,2,n)*x2y5z2*3.+c(3,5,3,n)*x2y5z3*3.+c(3,5,4,n)*
     & x2y5z4*3.+c(3,5,5,n)*x2y5z5*3.+c(4,0,0,n)*x3*4.+c(4,0,1,n)*
     & x3z1*4.+c(4,0,2,n)*x3z2*4.+c(4,0,3,n)*x3z3*4.+c(4,0,4,n)*x3z4*
     & 4.+c(4,0,5,n)*x3z5*4.+c(4,1,0,n)*x3y1*4.+c(4,1,1,n)*x3y1z1*4.+
     & c(4,1,2,n)*x3y1z2*4.+c(4,1,3,n)*x3y1z3*4.+c(4,1,4,n)*x3y1z4*4.+
     & c(4,1,5,n)*x3y1z5*4.+c(4,2,0,n)*x3y2*4.+c(4,2,1,n)*x3y2z1*4.+c(
     & 4,2,2,n)*x3y2z2*4.+c(4,2,3,n)*x3y2z3*4.+c(4,2,4,n)*x3y2z4*4.+c(
     & 4,2,5,n)*x3y2z5*4.+c(4,3,0,n)*x3y3*4.+c(4,3,1,n)*x3y3z1*4.+c(4,
     & 3,2,n)*x3y3z2*4.+c(4,3,3,n)*x3y3z3*4.+c(4,3,4,n)*x3y3z4*4.+c(4,
     & 3,5,n)*x3y3z5*4.+c(4,4,0,n)*x3y4*4.+c(4,4,1,n)*x3y4z1*4.+c(4,4,
     & 2,n)*x3y4z2*4.+c(4,4,3,n)*x3y4z3*4.+c(4,4,4,n)*x3y4z4*4.+c(4,4,
     & 5,n)*x3y4z5*4.+c(4,5,0,n)*x3y5*4.+c(4,5,1,n)*x3y5z1*4.+c(4,5,2,
     & n)*x3y5z2*4.+c(4,5,3,n)*x3y5z3*4.+c(4,5,4,n)*x3y5z4*4.+c(4,5,5,
     & n)*x3y5z5*4.+c(5,0,0,n)*x4*5.+c(5,0,1,n)*x4z1*5.+c(5,0,2,n)*
     & x4z2*5.+c(5,0,3,n)*x4z3*5.+c(5,0,4,n)*x4z4*5.+c(5,0,5,n)*x4z5*
     & 5.+c(5,1,0,n)*x4y1*5.+c(5,1,1,n)*x4y1z1*5.+c(5,1,2,n)*x4y1z2*
     & 5.+c(5,1,3,n)*x4y1z3*5.+c(5,1,4,n)*x4y1z4*5.+c(5,1,5,n)*x4y1z5*
     & 5.+c(5,2,0,n)*x4y2*5.+c(5,2,1,n)*x4y2z1*5.+c(5,2,2,n)*x4y2z2*
     & 5.+c(5,2,3,n)*x4y2z3*5.+c(5,2,4,n)*x4y2z4*5.+c(5,2,5,n)*x4y2z5*
     & 5.+c(5,3,0,n)*x4y3*5.+c(5,3,1,n)*x4y3z1*5.+c(5,3,2,n)*x4y3z2*
     & 5.+c(5,3,3,n)*x4y3z3*5.+c(5,3,4,n)*x4y3z4*5.+c(5,3,5,n)*x4y3z5*
     & 5.+c(5,4,0,n)*x4y4*5.+c(5,4,1,n)*x4y4z1*5.+c(5,4,2,n)*x4y4z2*
     & 5.+c(5,4,3,n)*x4y4z3*5.+c(5,4,4,n)*x4y4z4*5.+c(5,4,5,n)*x4y4z5*
     & 5.+c(5,5,0,n)*x4y5*5.+c(5,5,1,n)*x4y5z1*5.+c(5,5,2,n)*x4y5z2*
     & 5.+c(5,5,3,n)*x4y5z3*5.+c(5,5,4,n)*x4y5z4*5.+c(5,5,5,n)*x4y5z5*
     & 5.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.1.and.dy.eq.0.and.dz.eq.1 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      y3z4=y2z4*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y4z2=y3z2*y1
      y4z3=y3z3*y1
      y4z4=y3z4*y1
      y5=y4*y1
      y5z1=y4z1*y1
      y5z2=y4z2*y1
      y5z3=y4z3*y1
      y5z4=y4z4*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y2z4=x1y1z4*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x1y3z4=x1y2z4*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y4z2=x1y3z2*y1
      x1y4z3=x1y3z3*y1
      x1y4z4=x1y3z4*y1
      x1y5=x1y4*y1
      x1y5z1=x1y4z1*y1
      x1y5z2=x1y4z2*y1
      x1y5z3=x1y4z3*y1
      x1y5z4=x1y4z4*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y1z4=x1y1z4*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y2z4=x1y2z4*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y3z3=x1y3z3*x1
      x2y3z4=x1y3z4*x1
      x2y4=x1y4*x1
      x2y4z1=x1y4z1*x1
      x2y4z2=x1y4z2*x1
      x2y4z3=x1y4z3*x1
      x2y4z4=x1y4z4*x1
      x2y5=x1y5*x1
      x2y5z1=x1y5z1*x1
      x2y5z2=x1y5z2*x1
      x2y5z3=x1y5z3*x1
      x2y5z4=x1y5z4*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3z4=x2z4*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x3y1z4=x2y1z4*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y2z3=x2y2z3*x1
      x3y2z4=x2y2z4*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x3y3z2=x2y3z2*x1
      x3y3z3=x2y3z3*x1
      x3y3z4=x2y3z4*x1
      x3y4=x2y4*x1
      x3y4z1=x2y4z1*x1
      x3y4z2=x2y4z2*x1
      x3y4z3=x2y4z3*x1
      x3y4z4=x2y4z4*x1
      x3y5=x2y5*x1
      x3y5z1=x2y5z1*x1
      x3y5z2=x2y5z2*x1
      x3y5z3=x2y5z3*x1
      x3y5z4=x2y5z4*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4z2=x3z2*x1
      x4z3=x3z3*x1
      x4z4=x3z4*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y1z2=x3y1z2*x1
      x4y1z3=x3y1z3*x1
      x4y1z4=x3y1z4*x1
      x4y2=x3y2*x1
      x4y2z1=x3y2z1*x1
      x4y2z2=x3y2z2*x1
      x4y2z3=x3y2z3*x1
      x4y2z4=x3y2z4*x1
      x4y3=x3y3*x1
      x4y3z1=x3y3z1*x1
      x4y3z2=x3y3z2*x1
      x4y3z3=x3y3z3*x1
      x4y3z4=x3y3z4*x1
      x4y4=x3y4*x1
      x4y4z1=x3y4z1*x1
      x4y4z2=x3y4z2*x1
      x4y4z3=x3y4z3*x1
      x4y4z4=x3y4z4*x1
      x4y5=x3y5*x1
      x4y5z1=x3y5z1*x1
      x4y5z2=x3y5z2*x1
      x4y5z3=x3y5z3*x1
      x4y5z4=x3y5z4*x1
      r(i1,i2,i3,n)=(c(1,0,1,n)+c(1,0,2,n)*z1*2.+c(1,0,3,n)*z2*3.+c(1,
     & 0,4,n)*z3*4.+c(1,0,5,n)*z4*5.+c(1,1,1,n)*y1+c(1,1,2,n)*y1z1*2.+
     & c(1,1,3,n)*y1z2*3.+c(1,1,4,n)*y1z3*4.+c(1,1,5,n)*y1z4*5.+c(1,2,
     & 1,n)*y2+c(1,2,2,n)*y2z1*2.+c(1,2,3,n)*y2z2*3.+c(1,2,4,n)*y2z3*
     & 4.+c(1,2,5,n)*y2z4*5.+c(1,3,1,n)*y3+c(1,3,2,n)*y3z1*2.+c(1,3,3,
     & n)*y3z2*3.+c(1,3,4,n)*y3z3*4.+c(1,3,5,n)*y3z4*5.+c(1,4,1,n)*y4+
     & c(1,4,2,n)*y4z1*2.+c(1,4,3,n)*y4z2*3.+c(1,4,4,n)*y4z3*4.+c(1,4,
     & 5,n)*y4z4*5.+c(1,5,1,n)*y5+c(1,5,2,n)*y5z1*2.+c(1,5,3,n)*y5z2*
     & 3.+c(1,5,4,n)*y5z3*4.+c(1,5,5,n)*y5z4*5.+c(2,0,1,n)*x1*2.+c(2,
     & 0,2,n)*x1z1*4.+c(2,0,3,n)*x1z2*6.+c(2,0,4,n)*x1z3*8.+c(2,0,5,n)
     & *x1z4*10.+c(2,1,1,n)*x1y1*2.+c(2,1,2,n)*x1y1z1*4.+c(2,1,3,n)*
     & x1y1z2*6.+c(2,1,4,n)*x1y1z3*8.+c(2,1,5,n)*x1y1z4*10.+c(2,2,1,n)
     & *x1y2*2.+c(2,2,2,n)*x1y2z1*4.+c(2,2,3,n)*x1y2z2*6.+c(2,2,4,n)*
     & x1y2z3*8.+c(2,2,5,n)*x1y2z4*10.+c(2,3,1,n)*x1y3*2.+c(2,3,2,n)*
     & x1y3z1*4.+c(2,3,3,n)*x1y3z2*6.+c(2,3,4,n)*x1y3z3*8.+c(2,3,5,n)*
     & x1y3z4*10.+c(2,4,1,n)*x1y4*2.+c(2,4,2,n)*x1y4z1*4.+c(2,4,3,n)*
     & x1y4z2*6.+c(2,4,4,n)*x1y4z3*8.+c(2,4,5,n)*x1y4z4*10.+c(2,5,1,n)
     & *x1y5*2.+c(2,5,2,n)*x1y5z1*4.+c(2,5,3,n)*x1y5z2*6.+c(2,5,4,n)*
     & x1y5z3*8.+c(2,5,5,n)*x1y5z4*10.+c(3,0,1,n)*x2*3.+c(3,0,2,n)*
     & x2z1*6.+c(3,0,3,n)*x2z2*9.+c(3,0,4,n)*x2z3*12.+c(3,0,5,n)*x2z4*
     & 15.+c(3,1,1,n)*x2y1*3.+c(3,1,2,n)*x2y1z1*6.+c(3,1,3,n)*x2y1z2*
     & 9.+c(3,1,4,n)*x2y1z3*12.+c(3,1,5,n)*x2y1z4*15.+c(3,2,1,n)*x2y2*
     & 3.+c(3,2,2,n)*x2y2z1*6.+c(3,2,3,n)*x2y2z2*9.+c(3,2,4,n)*x2y2z3*
     & 12.+c(3,2,5,n)*x2y2z4*15.+c(3,3,1,n)*x2y3*3.+c(3,3,2,n)*x2y3z1*
     & 6.+c(3,3,3,n)*x2y3z2*9.+c(3,3,4,n)*x2y3z3*12.+c(3,3,5,n)*
     & x2y3z4*15.+c(3,4,1,n)*x2y4*3.+c(3,4,2,n)*x2y4z1*6.+c(3,4,3,n)*
     & x2y4z2*9.+c(3,4,4,n)*x2y4z3*12.+c(3,4,5,n)*x2y4z4*15.+c(3,5,1,
     & n)*x2y5*3.+c(3,5,2,n)*x2y5z1*6.+c(3,5,3,n)*x2y5z2*9.+c(3,5,4,n)
     & *x2y5z3*12.+c(3,5,5,n)*x2y5z4*15.+c(4,0,1,n)*x3*4.+c(4,0,2,n)*
     & x3z1*8.+c(4,0,3,n)*x3z2*12.+c(4,0,4,n)*x3z3*16.+c(4,0,5,n)*
     & x3z4*20.+c(4,1,1,n)*x3y1*4.+c(4,1,2,n)*x3y1z1*8.+c(4,1,3,n)*
     & x3y1z2*12.+c(4,1,4,n)*x3y1z3*16.+c(4,1,5,n)*x3y1z4*20.+c(4,2,1,
     & n)*x3y2*4.+c(4,2,2,n)*x3y2z1*8.+c(4,2,3,n)*x3y2z2*12.+c(4,2,4,
     & n)*x3y2z3*16.+c(4,2,5,n)*x3y2z4*20.+c(4,3,1,n)*x3y3*4.+c(4,3,2,
     & n)*x3y3z1*8.+c(4,3,3,n)*x3y3z2*12.+c(4,3,4,n)*x3y3z3*16.+c(4,3,
     & 5,n)*x3y3z4*20.+c(4,4,1,n)*x3y4*4.+c(4,4,2,n)*x3y4z1*8.+c(4,4,
     & 3,n)*x3y4z2*12.+c(4,4,4,n)*x3y4z3*16.+c(4,4,5,n)*x3y4z4*20.+c(
     & 4,5,1,n)*x3y5*4.+c(4,5,2,n)*x3y5z1*8.+c(4,5,3,n)*x3y5z2*12.+c(
     & 4,5,4,n)*x3y5z3*16.+c(4,5,5,n)*x3y5z4*20.+c(5,0,1,n)*x4*5.+c(5,
     & 0,2,n)*x4z1*10.+c(5,0,3,n)*x4z2*15.+c(5,0,4,n)*x4z3*20.+c(5,0,
     & 5,n)*x4z4*25.+c(5,1,1,n)*x4y1*5.+c(5,1,2,n)*x4y1z1*10.+c(5,1,3,
     & n)*x4y1z2*15.+c(5,1,4,n)*x4y1z3*20.+c(5,1,5,n)*x4y1z4*25.+c(5,
     & 2,1,n)*x4y2*5.+c(5,2,2,n)*x4y2z1*10.+c(5,2,3,n)*x4y2z2*15.+c(5,
     & 2,4,n)*x4y2z3*20.+c(5,2,5,n)*x4y2z4*25.+c(5,3,1,n)*x4y3*5.+c(5,
     & 3,2,n)*x4y3z1*10.+c(5,3,3,n)*x4y3z2*15.+c(5,3,4,n)*x4y3z3*20.+
     & c(5,3,5,n)*x4y3z4*25.+c(5,4,1,n)*x4y4*5.+c(5,4,2,n)*x4y4z1*10.+
     & c(5,4,3,n)*x4y4z2*15.+c(5,4,4,n)*x4y4z3*20.+c(5,4,5,n)*x4y4z4*
     & 25.+c(5,5,1,n)*x4y5*5.+c(5,5,2,n)*x4y5z1*10.+c(5,5,3,n)*x4y5z2*
     & 15.+c(5,5,4,n)*x4y5z3*20.+c(5,5,5,n)*x4y5z4*25.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.1.and.dy.eq.0.and.dz.eq.2 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y4z2=y3z2*y1
      y4z3=y3z3*y1
      y5=y4*y1
      y5z1=y4z1*y1
      y5z2=y4z2*y1
      y5z3=y4z3*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y4z2=x1y3z2*y1
      x1y4z3=x1y3z3*y1
      x1y5=x1y4*y1
      x1y5z1=x1y4z1*y1
      x1y5z2=x1y4z2*y1
      x1y5z3=x1y4z3*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y3z3=x1y3z3*x1
      x2y4=x1y4*x1
      x2y4z1=x1y4z1*x1
      x2y4z2=x1y4z2*x1
      x2y4z3=x1y4z3*x1
      x2y5=x1y5*x1
      x2y5z1=x1y5z1*x1
      x2y5z2=x1y5z2*x1
      x2y5z3=x1y5z3*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y2z3=x2y2z3*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x3y3z2=x2y3z2*x1
      x3y3z3=x2y3z3*x1
      x3y4=x2y4*x1
      x3y4z1=x2y4z1*x1
      x3y4z2=x2y4z2*x1
      x3y4z3=x2y4z3*x1
      x3y5=x2y5*x1
      x3y5z1=x2y5z1*x1
      x3y5z2=x2y5z2*x1
      x3y5z3=x2y5z3*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4z2=x3z2*x1
      x4z3=x3z3*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y1z2=x3y1z2*x1
      x4y1z3=x3y1z3*x1
      x4y2=x3y2*x1
      x4y2z1=x3y2z1*x1
      x4y2z2=x3y2z2*x1
      x4y2z3=x3y2z3*x1
      x4y3=x3y3*x1
      x4y3z1=x3y3z1*x1
      x4y3z2=x3y3z2*x1
      x4y3z3=x3y3z3*x1
      x4y4=x3y4*x1
      x4y4z1=x3y4z1*x1
      x4y4z2=x3y4z2*x1
      x4y4z3=x3y4z3*x1
      x4y5=x3y5*x1
      x4y5z1=x3y5z1*x1
      x4y5z2=x3y5z2*x1
      x4y5z3=x3y5z3*x1
      r(i1,i2,i3,n)=(c(1,0,2,n)*2.+c(1,0,3,n)*z1*6.+c(1,0,4,n)*z2*12.+
     & c(1,0,5,n)*z3*20.+c(1,1,2,n)*y1*2.+c(1,1,3,n)*y1z1*6.+c(1,1,4,
     & n)*y1z2*12.+c(1,1,5,n)*y1z3*20.+c(1,2,2,n)*y2*2.+c(1,2,3,n)*
     & y2z1*6.+c(1,2,4,n)*y2z2*12.+c(1,2,5,n)*y2z3*20.+c(1,3,2,n)*y3*
     & 2.+c(1,3,3,n)*y3z1*6.+c(1,3,4,n)*y3z2*12.+c(1,3,5,n)*y3z3*20.+
     & c(1,4,2,n)*y4*2.+c(1,4,3,n)*y4z1*6.+c(1,4,4,n)*y4z2*12.+c(1,4,
     & 5,n)*y4z3*20.+c(1,5,2,n)*y5*2.+c(1,5,3,n)*y5z1*6.+c(1,5,4,n)*
     & y5z2*12.+c(1,5,5,n)*y5z3*20.+c(2,0,2,n)*x1*4.+c(2,0,3,n)*x1z1*
     & 12.+c(2,0,4,n)*x1z2*24.+c(2,0,5,n)*x1z3*40.+c(2,1,2,n)*x1y1*4.+
     & c(2,1,3,n)*x1y1z1*12.+c(2,1,4,n)*x1y1z2*24.+c(2,1,5,n)*x1y1z3*
     & 40.+c(2,2,2,n)*x1y2*4.+c(2,2,3,n)*x1y2z1*12.+c(2,2,4,n)*x1y2z2*
     & 24.+c(2,2,5,n)*x1y2z3*40.+c(2,3,2,n)*x1y3*4.+c(2,3,3,n)*x1y3z1*
     & 12.+c(2,3,4,n)*x1y3z2*24.+c(2,3,5,n)*x1y3z3*40.+c(2,4,2,n)*
     & x1y4*4.+c(2,4,3,n)*x1y4z1*12.+c(2,4,4,n)*x1y4z2*24.+c(2,4,5,n)*
     & x1y4z3*40.+c(2,5,2,n)*x1y5*4.+c(2,5,3,n)*x1y5z1*12.+c(2,5,4,n)*
     & x1y5z2*24.+c(2,5,5,n)*x1y5z3*40.+c(3,0,2,n)*x2*6.+c(3,0,3,n)*
     & x2z1*18.+c(3,0,4,n)*x2z2*36.+c(3,0,5,n)*x2z3*60.+c(3,1,2,n)*
     & x2y1*6.+c(3,1,3,n)*x2y1z1*18.+c(3,1,4,n)*x2y1z2*36.+c(3,1,5,n)*
     & x2y1z3*60.+c(3,2,2,n)*x2y2*6.+c(3,2,3,n)*x2y2z1*18.+c(3,2,4,n)*
     & x2y2z2*36.+c(3,2,5,n)*x2y2z3*60.+c(3,3,2,n)*x2y3*6.+c(3,3,3,n)*
     & x2y3z1*18.+c(3,3,4,n)*x2y3z2*36.+c(3,3,5,n)*x2y3z3*60.+c(3,4,2,
     & n)*x2y4*6.+c(3,4,3,n)*x2y4z1*18.+c(3,4,4,n)*x2y4z2*36.+c(3,4,5,
     & n)*x2y4z3*60.+c(3,5,2,n)*x2y5*6.+c(3,5,3,n)*x2y5z1*18.+c(3,5,4,
     & n)*x2y5z2*36.+c(3,5,5,n)*x2y5z3*60.+c(4,0,2,n)*x3*8.+c(4,0,3,n)
     & *x3z1*24.+c(4,0,4,n)*x3z2*48.+c(4,0,5,n)*x3z3*80.+c(4,1,2,n)*
     & x3y1*8.+c(4,1,3,n)*x3y1z1*24.+c(4,1,4,n)*x3y1z2*48.+c(4,1,5,n)*
     & x3y1z3*80.+c(4,2,2,n)*x3y2*8.+c(4,2,3,n)*x3y2z1*24.+c(4,2,4,n)*
     & x3y2z2*48.+c(4,2,5,n)*x3y2z3*80.+c(4,3,2,n)*x3y3*8.+c(4,3,3,n)*
     & x3y3z1*24.+c(4,3,4,n)*x3y3z2*48.+c(4,3,5,n)*x3y3z3*80.+c(4,4,2,
     & n)*x3y4*8.+c(4,4,3,n)*x3y4z1*24.+c(4,4,4,n)*x3y4z2*48.+c(4,4,5,
     & n)*x3y4z3*80.+c(4,5,2,n)*x3y5*8.+c(4,5,3,n)*x3y5z1*24.+c(4,5,4,
     & n)*x3y5z2*48.+c(4,5,5,n)*x3y5z3*80.+c(5,0,2,n)*x4*10.+c(5,0,3,
     & n)*x4z1*30.+c(5,0,4,n)*x4z2*60.+c(5,0,5,n)*x4z3*100.+c(5,1,2,n)
     & *x4y1*10.+c(5,1,3,n)*x4y1z1*30.+c(5,1,4,n)*x4y1z2*60.+c(5,1,5,
     & n)*x4y1z3*100.+c(5,2,2,n)*x4y2*10.+c(5,2,3,n)*x4y2z1*30.+c(5,2,
     & 4,n)*x4y2z2*60.+c(5,2,5,n)*x4y2z3*100.+c(5,3,2,n)*x4y3*10.+c(5,
     & 3,3,n)*x4y3z1*30.+c(5,3,4,n)*x4y3z2*60.+c(5,3,5,n)*x4y3z3*100.+
     & c(5,4,2,n)*x4y4*10.+c(5,4,3,n)*x4y4z1*30.+c(5,4,4,n)*x4y4z2*
     & 60.+c(5,4,5,n)*x4y4z3*100.+c(5,5,2,n)*x4y5*10.+c(5,5,3,n)*
     & x4y5z1*30.+c(5,5,4,n)*x4y5z2*60.+c(5,5,5,n)*x4y5z3*100.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.1.and.dy.eq.0.and.dz.eq.3 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y4z2=y3z2*y1
      y5=y4*y1
      y5z1=y4z1*y1
      y5z2=y4z2*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y4z2=x1y3z2*y1
      x1y5=x1y4*y1
      x1y5z1=x1y4z1*y1
      x1y5z2=x1y4z2*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y4=x1y4*x1
      x2y4z1=x1y4z1*x1
      x2y4z2=x1y4z2*x1
      x2y5=x1y5*x1
      x2y5z1=x1y5z1*x1
      x2y5z2=x1y5z2*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x3y3z2=x2y3z2*x1
      x3y4=x2y4*x1
      x3y4z1=x2y4z1*x1
      x3y4z2=x2y4z2*x1
      x3y5=x2y5*x1
      x3y5z1=x2y5z1*x1
      x3y5z2=x2y5z2*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4z2=x3z2*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y1z2=x3y1z2*x1
      x4y2=x3y2*x1
      x4y2z1=x3y2z1*x1
      x4y2z2=x3y2z2*x1
      x4y3=x3y3*x1
      x4y3z1=x3y3z1*x1
      x4y3z2=x3y3z2*x1
      x4y4=x3y4*x1
      x4y4z1=x3y4z1*x1
      x4y4z2=x3y4z2*x1
      x4y5=x3y5*x1
      x4y5z1=x3y5z1*x1
      x4y5z2=x3y5z2*x1
      r(i1,i2,i3,n)=(c(1,0,3,n)*6.+c(1,0,4,n)*z1*24.+c(1,0,5,n)*z2*60.+
     & c(1,1,3,n)*y1*6.+c(1,1,4,n)*y1z1*24.+c(1,1,5,n)*y1z2*60.+c(1,2,
     & 3,n)*y2*6.+c(1,2,4,n)*y2z1*24.+c(1,2,5,n)*y2z2*60.+c(1,3,3,n)*
     & y3*6.+c(1,3,4,n)*y3z1*24.+c(1,3,5,n)*y3z2*60.+c(1,4,3,n)*y4*6.+
     & c(1,4,4,n)*y4z1*24.+c(1,4,5,n)*y4z2*60.+c(1,5,3,n)*y5*6.+c(1,5,
     & 4,n)*y5z1*24.+c(1,5,5,n)*y5z2*60.+c(2,0,3,n)*x1*12.+c(2,0,4,n)*
     & x1z1*48.+c(2,0,5,n)*x1z2*120.+c(2,1,3,n)*x1y1*12.+c(2,1,4,n)*
     & x1y1z1*48.+c(2,1,5,n)*x1y1z2*120.+c(2,2,3,n)*x1y2*12.+c(2,2,4,
     & n)*x1y2z1*48.+c(2,2,5,n)*x1y2z2*120.+c(2,3,3,n)*x1y3*12.+c(2,3,
     & 4,n)*x1y3z1*48.+c(2,3,5,n)*x1y3z2*120.+c(2,4,3,n)*x1y4*12.+c(2,
     & 4,4,n)*x1y4z1*48.+c(2,4,5,n)*x1y4z2*120.+c(2,5,3,n)*x1y5*12.+c(
     & 2,5,4,n)*x1y5z1*48.+c(2,5,5,n)*x1y5z2*120.+c(3,0,3,n)*x2*18.+c(
     & 3,0,4,n)*x2z1*72.+c(3,0,5,n)*x2z2*180.+c(3,1,3,n)*x2y1*18.+c(3,
     & 1,4,n)*x2y1z1*72.+c(3,1,5,n)*x2y1z2*180.+c(3,2,3,n)*x2y2*18.+c(
     & 3,2,4,n)*x2y2z1*72.+c(3,2,5,n)*x2y2z2*180.+c(3,3,3,n)*x2y3*18.+
     & c(3,3,4,n)*x2y3z1*72.+c(3,3,5,n)*x2y3z2*180.+c(3,4,3,n)*x2y4*
     & 18.+c(3,4,4,n)*x2y4z1*72.+c(3,4,5,n)*x2y4z2*180.+c(3,5,3,n)*
     & x2y5*18.+c(3,5,4,n)*x2y5z1*72.+c(3,5,5,n)*x2y5z2*180.+c(4,0,3,
     & n)*x3*24.+c(4,0,4,n)*x3z1*96.+c(4,0,5,n)*x3z2*240.+c(4,1,3,n)*
     & x3y1*24.+c(4,1,4,n)*x3y1z1*96.+c(4,1,5,n)*x3y1z2*240.+c(4,2,3,
     & n)*x3y2*24.+c(4,2,4,n)*x3y2z1*96.+c(4,2,5,n)*x3y2z2*240.+c(4,3,
     & 3,n)*x3y3*24.+c(4,3,4,n)*x3y3z1*96.+c(4,3,5,n)*x3y3z2*240.+c(4,
     & 4,3,n)*x3y4*24.+c(4,4,4,n)*x3y4z1*96.+c(4,4,5,n)*x3y4z2*240.+c(
     & 4,5,3,n)*x3y5*24.+c(4,5,4,n)*x3y5z1*96.+c(4,5,5,n)*x3y5z2*240.+
     & c(5,0,3,n)*x4*30.+c(5,0,4,n)*x4z1*120.+c(5,0,5,n)*x4z2*300.+c(
     & 5,1,3,n)*x4y1*30.+c(5,1,4,n)*x4y1z1*120.+c(5,1,5,n)*x4y1z2*
     & 300.+c(5,2,3,n)*x4y2*30.+c(5,2,4,n)*x4y2z1*120.+c(5,2,5,n)*
     & x4y2z2*300.+c(5,3,3,n)*x4y3*30.+c(5,3,4,n)*x4y3z1*120.+c(5,3,5,
     & n)*x4y3z2*300.+c(5,4,3,n)*x4y4*30.+c(5,4,4,n)*x4y4z1*120.+c(5,
     & 4,5,n)*x4y4z2*300.+c(5,5,3,n)*x4y5*30.+c(5,5,4,n)*x4y5z1*120.+
     & c(5,5,5,n)*x4y5z2*300.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.1.and.dy.eq.0.and.dz.eq.4 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y5=y4*y1
      y5z1=y4z1*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y5=x1y4*y1
      x1y5z1=x1y4z1*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y4=x1y4*x1
      x2y4z1=x1y4z1*x1
      x2y5=x1y5*x1
      x2y5z1=x1y5z1*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x3y4=x2y4*x1
      x3y4z1=x2y4z1*x1
      x3y5=x2y5*x1
      x3y5z1=x2y5z1*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y2=x3y2*x1
      x4y2z1=x3y2z1*x1
      x4y3=x3y3*x1
      x4y3z1=x3y3z1*x1
      x4y4=x3y4*x1
      x4y4z1=x3y4z1*x1
      x4y5=x3y5*x1
      x4y5z1=x3y5z1*x1
      r(i1,i2,i3,n)=(c(1,0,4,n)*24.+c(1,0,5,n)*z1*120.+c(1,1,4,n)*y1*
     & 24.+c(1,1,5,n)*y1z1*120.+c(1,2,4,n)*y2*24.+c(1,2,5,n)*y2z1*
     & 120.+c(1,3,4,n)*y3*24.+c(1,3,5,n)*y3z1*120.+c(1,4,4,n)*y4*24.+
     & c(1,4,5,n)*y4z1*120.+c(1,5,4,n)*y5*24.+c(1,5,5,n)*y5z1*120.+c(
     & 2,0,4,n)*x1*48.+c(2,0,5,n)*x1z1*240.+c(2,1,4,n)*x1y1*48.+c(2,1,
     & 5,n)*x1y1z1*240.+c(2,2,4,n)*x1y2*48.+c(2,2,5,n)*x1y2z1*240.+c(
     & 2,3,4,n)*x1y3*48.+c(2,3,5,n)*x1y3z1*240.+c(2,4,4,n)*x1y4*48.+c(
     & 2,4,5,n)*x1y4z1*240.+c(2,5,4,n)*x1y5*48.+c(2,5,5,n)*x1y5z1*
     & 240.+c(3,0,4,n)*x2*72.+c(3,0,5,n)*x2z1*360.+c(3,1,4,n)*x2y1*
     & 72.+c(3,1,5,n)*x2y1z1*360.+c(3,2,4,n)*x2y2*72.+c(3,2,5,n)*
     & x2y2z1*360.+c(3,3,4,n)*x2y3*72.+c(3,3,5,n)*x2y3z1*360.+c(3,4,4,
     & n)*x2y4*72.+c(3,4,5,n)*x2y4z1*360.+c(3,5,4,n)*x2y5*72.+c(3,5,5,
     & n)*x2y5z1*360.+c(4,0,4,n)*x3*96.+c(4,0,5,n)*x3z1*480.+c(4,1,4,
     & n)*x3y1*96.+c(4,1,5,n)*x3y1z1*480.+c(4,2,4,n)*x3y2*96.+c(4,2,5,
     & n)*x3y2z1*480.+c(4,3,4,n)*x3y3*96.+c(4,3,5,n)*x3y3z1*480.+c(4,
     & 4,4,n)*x3y4*96.+c(4,4,5,n)*x3y4z1*480.+c(4,5,4,n)*x3y5*96.+c(4,
     & 5,5,n)*x3y5z1*480.+c(5,0,4,n)*x4*120.+c(5,0,5,n)*x4z1*600.+c(5,
     & 1,4,n)*x4y1*120.+c(5,1,5,n)*x4y1z1*600.+c(5,2,4,n)*x4y2*120.+c(
     & 5,2,5,n)*x4y2z1*600.+c(5,3,4,n)*x4y3*120.+c(5,3,5,n)*x4y3z1*
     & 600.+c(5,4,4,n)*x4y4*120.+c(5,4,5,n)*x4y4z1*600.+c(5,5,4,n)*
     & x4y5*120.+c(5,5,5,n)*x4y5z1*600.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.1.and.dy.eq.1.and.dz.eq.0 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y1z5=y1z4*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
      y2z5=y1z5*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      y3z4=y2z4*y1
      y3z5=y2z5*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y4z2=y3z2*y1
      y4z3=y3z3*y1
      y4z4=y3z4*y1
      y4z5=y3z5*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1z5=x1z4*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y1z5=x1y1z4*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y2z4=x1y1z4*y1
      x1y2z5=x1y1z5*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x1y3z4=x1y2z4*y1
      x1y3z5=x1y2z5*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y4z2=x1y3z2*y1
      x1y4z3=x1y3z3*y1
      x1y4z4=x1y3z4*y1
      x1y4z5=x1y3z5*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x2z5=x1z5*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y1z4=x1y1z4*x1
      x2y1z5=x1y1z5*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y2z4=x1y2z4*x1
      x2y2z5=x1y2z5*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y3z3=x1y3z3*x1
      x2y3z4=x1y3z4*x1
      x2y3z5=x1y3z5*x1
      x2y4=x1y4*x1
      x2y4z1=x1y4z1*x1
      x2y4z2=x1y4z2*x1
      x2y4z3=x1y4z3*x1
      x2y4z4=x1y4z4*x1
      x2y4z5=x1y4z5*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3z4=x2z4*x1
      x3z5=x2z5*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x3y1z4=x2y1z4*x1
      x3y1z5=x2y1z5*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y2z3=x2y2z3*x1
      x3y2z4=x2y2z4*x1
      x3y2z5=x2y2z5*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x3y3z2=x2y3z2*x1
      x3y3z3=x2y3z3*x1
      x3y3z4=x2y3z4*x1
      x3y3z5=x2y3z5*x1
      x3y4=x2y4*x1
      x3y4z1=x2y4z1*x1
      x3y4z2=x2y4z2*x1
      x3y4z3=x2y4z3*x1
      x3y4z4=x2y4z4*x1
      x3y4z5=x2y4z5*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4z2=x3z2*x1
      x4z3=x3z3*x1
      x4z4=x3z4*x1
      x4z5=x3z5*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y1z2=x3y1z2*x1
      x4y1z3=x3y1z3*x1
      x4y1z4=x3y1z4*x1
      x4y1z5=x3y1z5*x1
      x4y2=x3y2*x1
      x4y2z1=x3y2z1*x1
      x4y2z2=x3y2z2*x1
      x4y2z3=x3y2z3*x1
      x4y2z4=x3y2z4*x1
      x4y2z5=x3y2z5*x1
      x4y3=x3y3*x1
      x4y3z1=x3y3z1*x1
      x4y3z2=x3y3z2*x1
      x4y3z3=x3y3z3*x1
      x4y3z4=x3y3z4*x1
      x4y3z5=x3y3z5*x1
      x4y4=x3y4*x1
      x4y4z1=x3y4z1*x1
      x4y4z2=x3y4z2*x1
      x4y4z3=x3y4z3*x1
      x4y4z4=x3y4z4*x1
      x4y4z5=x3y4z5*x1
      r(i1,i2,i3,n)=(c(1,1,0,n)+c(1,1,1,n)*z1+c(1,1,2,n)*z2+c(1,1,3,n)*
     & z3+c(1,1,4,n)*z4+c(1,1,5,n)*z5+c(1,2,0,n)*y1*2.+c(1,2,1,n)*
     & y1z1*2.+c(1,2,2,n)*y1z2*2.+c(1,2,3,n)*y1z3*2.+c(1,2,4,n)*y1z4*
     & 2.+c(1,2,5,n)*y1z5*2.+c(1,3,0,n)*y2*3.+c(1,3,1,n)*y2z1*3.+c(1,
     & 3,2,n)*y2z2*3.+c(1,3,3,n)*y2z3*3.+c(1,3,4,n)*y2z4*3.+c(1,3,5,n)
     & *y2z5*3.+c(1,4,0,n)*y3*4.+c(1,4,1,n)*y3z1*4.+c(1,4,2,n)*y3z2*
     & 4.+c(1,4,3,n)*y3z3*4.+c(1,4,4,n)*y3z4*4.+c(1,4,5,n)*y3z5*4.+c(
     & 1,5,0,n)*y4*5.+c(1,5,1,n)*y4z1*5.+c(1,5,2,n)*y4z2*5.+c(1,5,3,n)
     & *y4z3*5.+c(1,5,4,n)*y4z4*5.+c(1,5,5,n)*y4z5*5.+c(2,1,0,n)*x1*
     & 2.+c(2,1,1,n)*x1z1*2.+c(2,1,2,n)*x1z2*2.+c(2,1,3,n)*x1z3*2.+c(
     & 2,1,4,n)*x1z4*2.+c(2,1,5,n)*x1z5*2.+c(2,2,0,n)*x1y1*4.+c(2,2,1,
     & n)*x1y1z1*4.+c(2,2,2,n)*x1y1z2*4.+c(2,2,3,n)*x1y1z3*4.+c(2,2,4,
     & n)*x1y1z4*4.+c(2,2,5,n)*x1y1z5*4.+c(2,3,0,n)*x1y2*6.+c(2,3,1,n)
     & *x1y2z1*6.+c(2,3,2,n)*x1y2z2*6.+c(2,3,3,n)*x1y2z3*6.+c(2,3,4,n)
     & *x1y2z4*6.+c(2,3,5,n)*x1y2z5*6.+c(2,4,0,n)*x1y3*8.+c(2,4,1,n)*
     & x1y3z1*8.+c(2,4,2,n)*x1y3z2*8.+c(2,4,3,n)*x1y3z3*8.+c(2,4,4,n)*
     & x1y3z4*8.+c(2,4,5,n)*x1y3z5*8.+c(2,5,0,n)*x1y4*10.+c(2,5,1,n)*
     & x1y4z1*10.+c(2,5,2,n)*x1y4z2*10.+c(2,5,3,n)*x1y4z3*10.+c(2,5,4,
     & n)*x1y4z4*10.+c(2,5,5,n)*x1y4z5*10.+c(3,1,0,n)*x2*3.+c(3,1,1,n)
     & *x2z1*3.+c(3,1,2,n)*x2z2*3.+c(3,1,3,n)*x2z3*3.+c(3,1,4,n)*x2z4*
     & 3.+c(3,1,5,n)*x2z5*3.+c(3,2,0,n)*x2y1*6.+c(3,2,1,n)*x2y1z1*6.+
     & c(3,2,2,n)*x2y1z2*6.+c(3,2,3,n)*x2y1z3*6.+c(3,2,4,n)*x2y1z4*6.+
     & c(3,2,5,n)*x2y1z5*6.+c(3,3,0,n)*x2y2*9.+c(3,3,1,n)*x2y2z1*9.+c(
     & 3,3,2,n)*x2y2z2*9.+c(3,3,3,n)*x2y2z3*9.+c(3,3,4,n)*x2y2z4*9.+c(
     & 3,3,5,n)*x2y2z5*9.+c(3,4,0,n)*x2y3*12.+c(3,4,1,n)*x2y3z1*12.+c(
     & 3,4,2,n)*x2y3z2*12.+c(3,4,3,n)*x2y3z3*12.+c(3,4,4,n)*x2y3z4*
     & 12.+c(3,4,5,n)*x2y3z5*12.+c(3,5,0,n)*x2y4*15.+c(3,5,1,n)*
     & x2y4z1*15.+c(3,5,2,n)*x2y4z2*15.+c(3,5,3,n)*x2y4z3*15.+c(3,5,4,
     & n)*x2y4z4*15.+c(3,5,5,n)*x2y4z5*15.+c(4,1,0,n)*x3*4.+c(4,1,1,n)
     & *x3z1*4.+c(4,1,2,n)*x3z2*4.+c(4,1,3,n)*x3z3*4.+c(4,1,4,n)*x3z4*
     & 4.+c(4,1,5,n)*x3z5*4.+c(4,2,0,n)*x3y1*8.+c(4,2,1,n)*x3y1z1*8.+
     & c(4,2,2,n)*x3y1z2*8.+c(4,2,3,n)*x3y1z3*8.+c(4,2,4,n)*x3y1z4*8.+
     & c(4,2,5,n)*x3y1z5*8.+c(4,3,0,n)*x3y2*12.+c(4,3,1,n)*x3y2z1*12.+
     & c(4,3,2,n)*x3y2z2*12.+c(4,3,3,n)*x3y2z3*12.+c(4,3,4,n)*x3y2z4*
     & 12.+c(4,3,5,n)*x3y2z5*12.+c(4,4,0,n)*x3y3*16.+c(4,4,1,n)*
     & x3y3z1*16.+c(4,4,2,n)*x3y3z2*16.+c(4,4,3,n)*x3y3z3*16.+c(4,4,4,
     & n)*x3y3z4*16.+c(4,4,5,n)*x3y3z5*16.+c(4,5,0,n)*x3y4*20.+c(4,5,
     & 1,n)*x3y4z1*20.+c(4,5,2,n)*x3y4z2*20.+c(4,5,3,n)*x3y4z3*20.+c(
     & 4,5,4,n)*x3y4z4*20.+c(4,5,5,n)*x3y4z5*20.+c(5,1,0,n)*x4*5.+c(5,
     & 1,1,n)*x4z1*5.+c(5,1,2,n)*x4z2*5.+c(5,1,3,n)*x4z3*5.+c(5,1,4,n)
     & *x4z4*5.+c(5,1,5,n)*x4z5*5.+c(5,2,0,n)*x4y1*10.+c(5,2,1,n)*
     & x4y1z1*10.+c(5,2,2,n)*x4y1z2*10.+c(5,2,3,n)*x4y1z3*10.+c(5,2,4,
     & n)*x4y1z4*10.+c(5,2,5,n)*x4y1z5*10.+c(5,3,0,n)*x4y2*15.+c(5,3,
     & 1,n)*x4y2z1*15.+c(5,3,2,n)*x4y2z2*15.+c(5,3,3,n)*x4y2z3*15.+c(
     & 5,3,4,n)*x4y2z4*15.+c(5,3,5,n)*x4y2z5*15.+c(5,4,0,n)*x4y3*20.+
     & c(5,4,1,n)*x4y3z1*20.+c(5,4,2,n)*x4y3z2*20.+c(5,4,3,n)*x4y3z3*
     & 20.+c(5,4,4,n)*x4y3z4*20.+c(5,4,5,n)*x4y3z5*20.+c(5,5,0,n)*
     & x4y4*25.+c(5,5,1,n)*x4y4z1*25.+c(5,5,2,n)*x4y4z2*25.+c(5,5,3,n)
     & *x4y4z3*25.+c(5,5,4,n)*x4y4z4*25.+c(5,5,5,n)*x4y4z5*25.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.1.and.dy.eq.1.and.dz.eq.1 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      y3z4=y2z4*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y4z2=y3z2*y1
      y4z3=y3z3*y1
      y4z4=y3z4*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y2z4=x1y1z4*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x1y3z4=x1y2z4*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y4z2=x1y3z2*y1
      x1y4z3=x1y3z3*y1
      x1y4z4=x1y3z4*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y1z4=x1y1z4*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y2z4=x1y2z4*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y3z3=x1y3z3*x1
      x2y3z4=x1y3z4*x1
      x2y4=x1y4*x1
      x2y4z1=x1y4z1*x1
      x2y4z2=x1y4z2*x1
      x2y4z3=x1y4z3*x1
      x2y4z4=x1y4z4*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3z4=x2z4*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x3y1z4=x2y1z4*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y2z3=x2y2z3*x1
      x3y2z4=x2y2z4*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x3y3z2=x2y3z2*x1
      x3y3z3=x2y3z3*x1
      x3y3z4=x2y3z4*x1
      x3y4=x2y4*x1
      x3y4z1=x2y4z1*x1
      x3y4z2=x2y4z2*x1
      x3y4z3=x2y4z3*x1
      x3y4z4=x2y4z4*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4z2=x3z2*x1
      x4z3=x3z3*x1
      x4z4=x3z4*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y1z2=x3y1z2*x1
      x4y1z3=x3y1z3*x1
      x4y1z4=x3y1z4*x1
      x4y2=x3y2*x1
      x4y2z1=x3y2z1*x1
      x4y2z2=x3y2z2*x1
      x4y2z3=x3y2z3*x1
      x4y2z4=x3y2z4*x1
      x4y3=x3y3*x1
      x4y3z1=x3y3z1*x1
      x4y3z2=x3y3z2*x1
      x4y3z3=x3y3z3*x1
      x4y3z4=x3y3z4*x1
      x4y4=x3y4*x1
      x4y4z1=x3y4z1*x1
      x4y4z2=x3y4z2*x1
      x4y4z3=x3y4z3*x1
      x4y4z4=x3y4z4*x1
      r(i1,i2,i3,n)=(c(1,1,1,n)+c(1,1,2,n)*z1*2.+c(1,1,3,n)*z2*3.+c(1,
     & 1,4,n)*z3*4.+c(1,1,5,n)*z4*5.+c(1,2,1,n)*y1*2.+c(1,2,2,n)*y1z1*
     & 4.+c(1,2,3,n)*y1z2*6.+c(1,2,4,n)*y1z3*8.+c(1,2,5,n)*y1z4*10.+c(
     & 1,3,1,n)*y2*3.+c(1,3,2,n)*y2z1*6.+c(1,3,3,n)*y2z2*9.+c(1,3,4,n)
     & *y2z3*12.+c(1,3,5,n)*y2z4*15.+c(1,4,1,n)*y3*4.+c(1,4,2,n)*y3z1*
     & 8.+c(1,4,3,n)*y3z2*12.+c(1,4,4,n)*y3z3*16.+c(1,4,5,n)*y3z4*20.+
     & c(1,5,1,n)*y4*5.+c(1,5,2,n)*y4z1*10.+c(1,5,3,n)*y4z2*15.+c(1,5,
     & 4,n)*y4z3*20.+c(1,5,5,n)*y4z4*25.+c(2,1,1,n)*x1*2.+c(2,1,2,n)*
     & x1z1*4.+c(2,1,3,n)*x1z2*6.+c(2,1,4,n)*x1z3*8.+c(2,1,5,n)*x1z4*
     & 10.+c(2,2,1,n)*x1y1*4.+c(2,2,2,n)*x1y1z1*8.+c(2,2,3,n)*x1y1z2*
     & 12.+c(2,2,4,n)*x1y1z3*16.+c(2,2,5,n)*x1y1z4*20.+c(2,3,1,n)*
     & x1y2*6.+c(2,3,2,n)*x1y2z1*12.+c(2,3,3,n)*x1y2z2*18.+c(2,3,4,n)*
     & x1y2z3*24.+c(2,3,5,n)*x1y2z4*30.+c(2,4,1,n)*x1y3*8.+c(2,4,2,n)*
     & x1y3z1*16.+c(2,4,3,n)*x1y3z2*24.+c(2,4,4,n)*x1y3z3*32.+c(2,4,5,
     & n)*x1y3z4*40.+c(2,5,1,n)*x1y4*10.+c(2,5,2,n)*x1y4z1*20.+c(2,5,
     & 3,n)*x1y4z2*30.+c(2,5,4,n)*x1y4z3*40.+c(2,5,5,n)*x1y4z4*50.+c(
     & 3,1,1,n)*x2*3.+c(3,1,2,n)*x2z1*6.+c(3,1,3,n)*x2z2*9.+c(3,1,4,n)
     & *x2z3*12.+c(3,1,5,n)*x2z4*15.+c(3,2,1,n)*x2y1*6.+c(3,2,2,n)*
     & x2y1z1*12.+c(3,2,3,n)*x2y1z2*18.+c(3,2,4,n)*x2y1z3*24.+c(3,2,5,
     & n)*x2y1z4*30.+c(3,3,1,n)*x2y2*9.+c(3,3,2,n)*x2y2z1*18.+c(3,3,3,
     & n)*x2y2z2*27.+c(3,3,4,n)*x2y2z3*36.+c(3,3,5,n)*x2y2z4*45.+c(3,
     & 4,1,n)*x2y3*12.+c(3,4,2,n)*x2y3z1*24.+c(3,4,3,n)*x2y3z2*36.+c(
     & 3,4,4,n)*x2y3z3*48.+c(3,4,5,n)*x2y3z4*60.+c(3,5,1,n)*x2y4*15.+
     & c(3,5,2,n)*x2y4z1*30.+c(3,5,3,n)*x2y4z2*45.+c(3,5,4,n)*x2y4z3*
     & 60.+c(3,5,5,n)*x2y4z4*75.+c(4,1,1,n)*x3*4.+c(4,1,2,n)*x3z1*8.+
     & c(4,1,3,n)*x3z2*12.+c(4,1,4,n)*x3z3*16.+c(4,1,5,n)*x3z4*20.+c(
     & 4,2,1,n)*x3y1*8.+c(4,2,2,n)*x3y1z1*16.+c(4,2,3,n)*x3y1z2*24.+c(
     & 4,2,4,n)*x3y1z3*32.+c(4,2,5,n)*x3y1z4*40.+c(4,3,1,n)*x3y2*12.+
     & c(4,3,2,n)*x3y2z1*24.+c(4,3,3,n)*x3y2z2*36.+c(4,3,4,n)*x3y2z3*
     & 48.+c(4,3,5,n)*x3y2z4*60.+c(4,4,1,n)*x3y3*16.+c(4,4,2,n)*
     & x3y3z1*32.+c(4,4,3,n)*x3y3z2*48.+c(4,4,4,n)*x3y3z3*64.+c(4,4,5,
     & n)*x3y3z4*80.+c(4,5,1,n)*x3y4*20.+c(4,5,2,n)*x3y4z1*40.+c(4,5,
     & 3,n)*x3y4z2*60.+c(4,5,4,n)*x3y4z3*80.+c(4,5,5,n)*x3y4z4*100.+c(
     & 5,1,1,n)*x4*5.+c(5,1,2,n)*x4z1*10.+c(5,1,3,n)*x4z2*15.+c(5,1,4,
     & n)*x4z3*20.+c(5,1,5,n)*x4z4*25.+c(5,2,1,n)*x4y1*10.+c(5,2,2,n)*
     & x4y1z1*20.+c(5,2,3,n)*x4y1z2*30.+c(5,2,4,n)*x4y1z3*40.+c(5,2,5,
     & n)*x4y1z4*50.+c(5,3,1,n)*x4y2*15.+c(5,3,2,n)*x4y2z1*30.+c(5,3,
     & 3,n)*x4y2z2*45.+c(5,3,4,n)*x4y2z3*60.+c(5,3,5,n)*x4y2z4*75.+c(
     & 5,4,1,n)*x4y3*20.+c(5,4,2,n)*x4y3z1*40.+c(5,4,3,n)*x4y3z2*60.+
     & c(5,4,4,n)*x4y3z3*80.+c(5,4,5,n)*x4y3z4*100.+c(5,5,1,n)*x4y4*
     & 25.+c(5,5,2,n)*x4y4z1*50.+c(5,5,3,n)*x4y4z2*75.+c(5,5,4,n)*
     & x4y4z3*100.+c(5,5,5,n)*x4y4z4*125.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.1.and.dy.eq.1.and.dz.eq.2 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y4z2=y3z2*y1
      y4z3=y3z3*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y4z2=x1y3z2*y1
      x1y4z3=x1y3z3*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y3z3=x1y3z3*x1
      x2y4=x1y4*x1
      x2y4z1=x1y4z1*x1
      x2y4z2=x1y4z2*x1
      x2y4z3=x1y4z3*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y2z3=x2y2z3*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x3y3z2=x2y3z2*x1
      x3y3z3=x2y3z3*x1
      x3y4=x2y4*x1
      x3y4z1=x2y4z1*x1
      x3y4z2=x2y4z2*x1
      x3y4z3=x2y4z3*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4z2=x3z2*x1
      x4z3=x3z3*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y1z2=x3y1z2*x1
      x4y1z3=x3y1z3*x1
      x4y2=x3y2*x1
      x4y2z1=x3y2z1*x1
      x4y2z2=x3y2z2*x1
      x4y2z3=x3y2z3*x1
      x4y3=x3y3*x1
      x4y3z1=x3y3z1*x1
      x4y3z2=x3y3z2*x1
      x4y3z3=x3y3z3*x1
      x4y4=x3y4*x1
      x4y4z1=x3y4z1*x1
      x4y4z2=x3y4z2*x1
      x4y4z3=x3y4z3*x1
      r(i1,i2,i3,n)=(c(1,1,2,n)*2.+c(1,1,3,n)*z1*6.+c(1,1,4,n)*z2*12.+
     & c(1,1,5,n)*z3*20.+c(1,2,2,n)*y1*4.+c(1,2,3,n)*y1z1*12.+c(1,2,4,
     & n)*y1z2*24.+c(1,2,5,n)*y1z3*40.+c(1,3,2,n)*y2*6.+c(1,3,3,n)*
     & y2z1*18.+c(1,3,4,n)*y2z2*36.+c(1,3,5,n)*y2z3*60.+c(1,4,2,n)*y3*
     & 8.+c(1,4,3,n)*y3z1*24.+c(1,4,4,n)*y3z2*48.+c(1,4,5,n)*y3z3*80.+
     & c(1,5,2,n)*y4*10.+c(1,5,3,n)*y4z1*30.+c(1,5,4,n)*y4z2*60.+c(1,
     & 5,5,n)*y4z3*100.+c(2,1,2,n)*x1*4.+c(2,1,3,n)*x1z1*12.+c(2,1,4,
     & n)*x1z2*24.+c(2,1,5,n)*x1z3*40.+c(2,2,2,n)*x1y1*8.+c(2,2,3,n)*
     & x1y1z1*24.+c(2,2,4,n)*x1y1z2*48.+c(2,2,5,n)*x1y1z3*80.+c(2,3,2,
     & n)*x1y2*12.+c(2,3,3,n)*x1y2z1*36.+c(2,3,4,n)*x1y2z2*72.+c(2,3,
     & 5,n)*x1y2z3*120.+c(2,4,2,n)*x1y3*16.+c(2,4,3,n)*x1y3z1*48.+c(2,
     & 4,4,n)*x1y3z2*96.+c(2,4,5,n)*x1y3z3*160.+c(2,5,2,n)*x1y4*20.+c(
     & 2,5,3,n)*x1y4z1*60.+c(2,5,4,n)*x1y4z2*120.+c(2,5,5,n)*x1y4z3*
     & 200.+c(3,1,2,n)*x2*6.+c(3,1,3,n)*x2z1*18.+c(3,1,4,n)*x2z2*36.+
     & c(3,1,5,n)*x2z3*60.+c(3,2,2,n)*x2y1*12.+c(3,2,3,n)*x2y1z1*36.+
     & c(3,2,4,n)*x2y1z2*72.+c(3,2,5,n)*x2y1z3*120.+c(3,3,2,n)*x2y2*
     & 18.+c(3,3,3,n)*x2y2z1*54.+c(3,3,4,n)*x2y2z2*108.+c(3,3,5,n)*
     & x2y2z3*180.+c(3,4,2,n)*x2y3*24.+c(3,4,3,n)*x2y3z1*72.+c(3,4,4,
     & n)*x2y3z2*144.+c(3,4,5,n)*x2y3z3*240.+c(3,5,2,n)*x2y4*30.+c(3,
     & 5,3,n)*x2y4z1*90.+c(3,5,4,n)*x2y4z2*180.+c(3,5,5,n)*x2y4z3*
     & 300.+c(4,1,2,n)*x3*8.+c(4,1,3,n)*x3z1*24.+c(4,1,4,n)*x3z2*48.+
     & c(4,1,5,n)*x3z3*80.+c(4,2,2,n)*x3y1*16.+c(4,2,3,n)*x3y1z1*48.+
     & c(4,2,4,n)*x3y1z2*96.+c(4,2,5,n)*x3y1z3*160.+c(4,3,2,n)*x3y2*
     & 24.+c(4,3,3,n)*x3y2z1*72.+c(4,3,4,n)*x3y2z2*144.+c(4,3,5,n)*
     & x3y2z3*240.+c(4,4,2,n)*x3y3*32.+c(4,4,3,n)*x3y3z1*96.+c(4,4,4,
     & n)*x3y3z2*192.+c(4,4,5,n)*x3y3z3*320.+c(4,5,2,n)*x3y4*40.+c(4,
     & 5,3,n)*x3y4z1*120.+c(4,5,4,n)*x3y4z2*240.+c(4,5,5,n)*x3y4z3*
     & 400.+c(5,1,2,n)*x4*10.+c(5,1,3,n)*x4z1*30.+c(5,1,4,n)*x4z2*60.+
     & c(5,1,5,n)*x4z3*100.+c(5,2,2,n)*x4y1*20.+c(5,2,3,n)*x4y1z1*60.+
     & c(5,2,4,n)*x4y1z2*120.+c(5,2,5,n)*x4y1z3*200.+c(5,3,2,n)*x4y2*
     & 30.+c(5,3,3,n)*x4y2z1*90.+c(5,3,4,n)*x4y2z2*180.+c(5,3,5,n)*
     & x4y2z3*300.+c(5,4,2,n)*x4y3*40.+c(5,4,3,n)*x4y3z1*120.+c(5,4,4,
     & n)*x4y3z2*240.+c(5,4,5,n)*x4y3z3*400.+c(5,5,2,n)*x4y4*50.+c(5,
     & 5,3,n)*x4y4z1*150.+c(5,5,4,n)*x4y4z2*300.+c(5,5,5,n)*x4y4z3*
     & 500.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.1.and.dy.eq.1.and.dz.eq.3 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y4z2=y3z2*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y4z2=x1y3z2*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y4=x1y4*x1
      x2y4z1=x1y4z1*x1
      x2y4z2=x1y4z2*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x3y3z2=x2y3z2*x1
      x3y4=x2y4*x1
      x3y4z1=x2y4z1*x1
      x3y4z2=x2y4z2*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4z2=x3z2*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y1z2=x3y1z2*x1
      x4y2=x3y2*x1
      x4y2z1=x3y2z1*x1
      x4y2z2=x3y2z2*x1
      x4y3=x3y3*x1
      x4y3z1=x3y3z1*x1
      x4y3z2=x3y3z2*x1
      x4y4=x3y4*x1
      x4y4z1=x3y4z1*x1
      x4y4z2=x3y4z2*x1
      r(i1,i2,i3,n)=(c(1,1,3,n)*6.+c(1,1,4,n)*z1*24.+c(1,1,5,n)*z2*60.+
     & c(1,2,3,n)*y1*12.+c(1,2,4,n)*y1z1*48.+c(1,2,5,n)*y1z2*120.+c(1,
     & 3,3,n)*y2*18.+c(1,3,4,n)*y2z1*72.+c(1,3,5,n)*y2z2*180.+c(1,4,3,
     & n)*y3*24.+c(1,4,4,n)*y3z1*96.+c(1,4,5,n)*y3z2*240.+c(1,5,3,n)*
     & y4*30.+c(1,5,4,n)*y4z1*120.+c(1,5,5,n)*y4z2*300.+c(2,1,3,n)*x1*
     & 12.+c(2,1,4,n)*x1z1*48.+c(2,1,5,n)*x1z2*120.+c(2,2,3,n)*x1y1*
     & 24.+c(2,2,4,n)*x1y1z1*96.+c(2,2,5,n)*x1y1z2*240.+c(2,3,3,n)*
     & x1y2*36.+c(2,3,4,n)*x1y2z1*144.+c(2,3,5,n)*x1y2z2*360.+c(2,4,3,
     & n)*x1y3*48.+c(2,4,4,n)*x1y3z1*192.+c(2,4,5,n)*x1y3z2*480.+c(2,
     & 5,3,n)*x1y4*60.+c(2,5,4,n)*x1y4z1*240.+c(2,5,5,n)*x1y4z2*600.+
     & c(3,1,3,n)*x2*18.+c(3,1,4,n)*x2z1*72.+c(3,1,5,n)*x2z2*180.+c(3,
     & 2,3,n)*x2y1*36.+c(3,2,4,n)*x2y1z1*144.+c(3,2,5,n)*x2y1z2*360.+
     & c(3,3,3,n)*x2y2*54.+c(3,3,4,n)*x2y2z1*216.+c(3,3,5,n)*x2y2z2*
     & 540.+c(3,4,3,n)*x2y3*72.+c(3,4,4,n)*x2y3z1*288.+c(3,4,5,n)*
     & x2y3z2*720.+c(3,5,3,n)*x2y4*90.+c(3,5,4,n)*x2y4z1*360.+c(3,5,5,
     & n)*x2y4z2*900.+c(4,1,3,n)*x3*24.+c(4,1,4,n)*x3z1*96.+c(4,1,5,n)
     & *x3z2*240.+c(4,2,3,n)*x3y1*48.+c(4,2,4,n)*x3y1z1*192.+c(4,2,5,
     & n)*x3y1z2*480.+c(4,3,3,n)*x3y2*72.+c(4,3,4,n)*x3y2z1*288.+c(4,
     & 3,5,n)*x3y2z2*720.+c(4,4,3,n)*x3y3*96.+c(4,4,4,n)*x3y3z1*384.+
     & c(4,4,5,n)*x3y3z2*960.+c(4,5,3,n)*x3y4*120.+c(4,5,4,n)*x3y4z1*
     & 480.+c(4,5,5,n)*x3y4z2*1200.+c(5,1,3,n)*x4*30.+c(5,1,4,n)*x4z1*
     & 120.+c(5,1,5,n)*x4z2*300.+c(5,2,3,n)*x4y1*60.+c(5,2,4,n)*
     & x4y1z1*240.+c(5,2,5,n)*x4y1z2*600.+c(5,3,3,n)*x4y2*90.+c(5,3,4,
     & n)*x4y2z1*360.+c(5,3,5,n)*x4y2z2*900.+c(5,4,3,n)*x4y3*120.+c(5,
     & 4,4,n)*x4y3z1*480.+c(5,4,5,n)*x4y3z2*1200.+c(5,5,3,n)*x4y4*
     & 150.+c(5,5,4,n)*x4y4z1*600.+c(5,5,5,n)*x4y4z2*1500.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.1.and.dy.eq.1.and.dz.eq.4 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y4=y3*y1
      y4z1=y3z1*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y4=x1y4*x1
      x2y4z1=x1y4z1*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x3y4=x2y4*x1
      x3y4z1=x2y4z1*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y2=x3y2*x1
      x4y2z1=x3y2z1*x1
      x4y3=x3y3*x1
      x4y3z1=x3y3z1*x1
      x4y4=x3y4*x1
      x4y4z1=x3y4z1*x1
      r(i1,i2,i3,n)=(c(1,1,4,n)*24.+c(1,1,5,n)*z1*120.+c(1,2,4,n)*y1*
     & 48.+c(1,2,5,n)*y1z1*240.+c(1,3,4,n)*y2*72.+c(1,3,5,n)*y2z1*
     & 360.+c(1,4,4,n)*y3*96.+c(1,4,5,n)*y3z1*480.+c(1,5,4,n)*y4*120.+
     & c(1,5,5,n)*y4z1*600.+c(2,1,4,n)*x1*48.+c(2,1,5,n)*x1z1*240.+c(
     & 2,2,4,n)*x1y1*96.+c(2,2,5,n)*x1y1z1*480.+c(2,3,4,n)*x1y2*144.+
     & c(2,3,5,n)*x1y2z1*720.+c(2,4,4,n)*x1y3*192.+c(2,4,5,n)*x1y3z1*
     & 960.+c(2,5,4,n)*x1y4*240.+c(2,5,5,n)*x1y4z1*1200.+c(3,1,4,n)*
     & x2*72.+c(3,1,5,n)*x2z1*360.+c(3,2,4,n)*x2y1*144.+c(3,2,5,n)*
     & x2y1z1*720.+c(3,3,4,n)*x2y2*216.+c(3,3,5,n)*x2y2z1*1080.+c(3,4,
     & 4,n)*x2y3*288.+c(3,4,5,n)*x2y3z1*1440.+c(3,5,4,n)*x2y4*360.+c(
     & 3,5,5,n)*x2y4z1*1800.+c(4,1,4,n)*x3*96.+c(4,1,5,n)*x3z1*480.+c(
     & 4,2,4,n)*x3y1*192.+c(4,2,5,n)*x3y1z1*960.+c(4,3,4,n)*x3y2*288.+
     & c(4,3,5,n)*x3y2z1*1440.+c(4,4,4,n)*x3y3*384.+c(4,4,5,n)*x3y3z1*
     & 1920.+c(4,5,4,n)*x3y4*480.+c(4,5,5,n)*x3y4z1*2400.+c(5,1,4,n)*
     & x4*120.+c(5,1,5,n)*x4z1*600.+c(5,2,4,n)*x4y1*240.+c(5,2,5,n)*
     & x4y1z1*1200.+c(5,3,4,n)*x4y2*360.+c(5,3,5,n)*x4y2z1*1800.+c(5,
     & 4,4,n)*x4y3*480.+c(5,4,5,n)*x4y3z1*2400.+c(5,5,4,n)*x4y4*600.+
     & c(5,5,5,n)*x4y4z1*3000.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.1.and.dy.eq.2.and.dz.eq.0 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y1z5=y1z4*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
      y2z5=y1z5*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      y3z4=y2z4*y1
      y3z5=y2z5*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1z5=x1z4*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y1z5=x1y1z4*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y2z4=x1y1z4*y1
      x1y2z5=x1y1z5*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x1y3z4=x1y2z4*y1
      x1y3z5=x1y2z5*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x2z5=x1z5*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y1z4=x1y1z4*x1
      x2y1z5=x1y1z5*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y2z4=x1y2z4*x1
      x2y2z5=x1y2z5*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y3z3=x1y3z3*x1
      x2y3z4=x1y3z4*x1
      x2y3z5=x1y3z5*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3z4=x2z4*x1
      x3z5=x2z5*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x3y1z4=x2y1z4*x1
      x3y1z5=x2y1z5*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y2z3=x2y2z3*x1
      x3y2z4=x2y2z4*x1
      x3y2z5=x2y2z5*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x3y3z2=x2y3z2*x1
      x3y3z3=x2y3z3*x1
      x3y3z4=x2y3z4*x1
      x3y3z5=x2y3z5*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4z2=x3z2*x1
      x4z3=x3z3*x1
      x4z4=x3z4*x1
      x4z5=x3z5*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y1z2=x3y1z2*x1
      x4y1z3=x3y1z3*x1
      x4y1z4=x3y1z4*x1
      x4y1z5=x3y1z5*x1
      x4y2=x3y2*x1
      x4y2z1=x3y2z1*x1
      x4y2z2=x3y2z2*x1
      x4y2z3=x3y2z3*x1
      x4y2z4=x3y2z4*x1
      x4y2z5=x3y2z5*x1
      x4y3=x3y3*x1
      x4y3z1=x3y3z1*x1
      x4y3z2=x3y3z2*x1
      x4y3z3=x3y3z3*x1
      x4y3z4=x3y3z4*x1
      x4y3z5=x3y3z5*x1
      r(i1,i2,i3,n)=(c(1,2,0,n)*2.+c(1,2,1,n)*z1*2.+c(1,2,2,n)*z2*2.+c(
     & 1,2,3,n)*z3*2.+c(1,2,4,n)*z4*2.+c(1,2,5,n)*z5*2.+c(1,3,0,n)*y1*
     & 6.+c(1,3,1,n)*y1z1*6.+c(1,3,2,n)*y1z2*6.+c(1,3,3,n)*y1z3*6.+c(
     & 1,3,4,n)*y1z4*6.+c(1,3,5,n)*y1z5*6.+c(1,4,0,n)*y2*12.+c(1,4,1,
     & n)*y2z1*12.+c(1,4,2,n)*y2z2*12.+c(1,4,3,n)*y2z3*12.+c(1,4,4,n)*
     & y2z4*12.+c(1,4,5,n)*y2z5*12.+c(1,5,0,n)*y3*20.+c(1,5,1,n)*y3z1*
     & 20.+c(1,5,2,n)*y3z2*20.+c(1,5,3,n)*y3z3*20.+c(1,5,4,n)*y3z4*
     & 20.+c(1,5,5,n)*y3z5*20.+c(2,2,0,n)*x1*4.+c(2,2,1,n)*x1z1*4.+c(
     & 2,2,2,n)*x1z2*4.+c(2,2,3,n)*x1z3*4.+c(2,2,4,n)*x1z4*4.+c(2,2,5,
     & n)*x1z5*4.+c(2,3,0,n)*x1y1*12.+c(2,3,1,n)*x1y1z1*12.+c(2,3,2,n)
     & *x1y1z2*12.+c(2,3,3,n)*x1y1z3*12.+c(2,3,4,n)*x1y1z4*12.+c(2,3,
     & 5,n)*x1y1z5*12.+c(2,4,0,n)*x1y2*24.+c(2,4,1,n)*x1y2z1*24.+c(2,
     & 4,2,n)*x1y2z2*24.+c(2,4,3,n)*x1y2z3*24.+c(2,4,4,n)*x1y2z4*24.+
     & c(2,4,5,n)*x1y2z5*24.+c(2,5,0,n)*x1y3*40.+c(2,5,1,n)*x1y3z1*
     & 40.+c(2,5,2,n)*x1y3z2*40.+c(2,5,3,n)*x1y3z3*40.+c(2,5,4,n)*
     & x1y3z4*40.+c(2,5,5,n)*x1y3z5*40.+c(3,2,0,n)*x2*6.+c(3,2,1,n)*
     & x2z1*6.+c(3,2,2,n)*x2z2*6.+c(3,2,3,n)*x2z3*6.+c(3,2,4,n)*x2z4*
     & 6.+c(3,2,5,n)*x2z5*6.+c(3,3,0,n)*x2y1*18.+c(3,3,1,n)*x2y1z1*
     & 18.+c(3,3,2,n)*x2y1z2*18.+c(3,3,3,n)*x2y1z3*18.+c(3,3,4,n)*
     & x2y1z4*18.+c(3,3,5,n)*x2y1z5*18.+c(3,4,0,n)*x2y2*36.+c(3,4,1,n)
     & *x2y2z1*36.+c(3,4,2,n)*x2y2z2*36.+c(3,4,3,n)*x2y2z3*36.+c(3,4,
     & 4,n)*x2y2z4*36.+c(3,4,5,n)*x2y2z5*36.+c(3,5,0,n)*x2y3*60.+c(3,
     & 5,1,n)*x2y3z1*60.+c(3,5,2,n)*x2y3z2*60.+c(3,5,3,n)*x2y3z3*60.+
     & c(3,5,4,n)*x2y3z4*60.+c(3,5,5,n)*x2y3z5*60.+c(4,2,0,n)*x3*8.+c(
     & 4,2,1,n)*x3z1*8.+c(4,2,2,n)*x3z2*8.+c(4,2,3,n)*x3z3*8.+c(4,2,4,
     & n)*x3z4*8.+c(4,2,5,n)*x3z5*8.+c(4,3,0,n)*x3y1*24.+c(4,3,1,n)*
     & x3y1z1*24.+c(4,3,2,n)*x3y1z2*24.+c(4,3,3,n)*x3y1z3*24.+c(4,3,4,
     & n)*x3y1z4*24.+c(4,3,5,n)*x3y1z5*24.+c(4,4,0,n)*x3y2*48.+c(4,4,
     & 1,n)*x3y2z1*48.+c(4,4,2,n)*x3y2z2*48.+c(4,4,3,n)*x3y2z3*48.+c(
     & 4,4,4,n)*x3y2z4*48.+c(4,4,5,n)*x3y2z5*48.+c(4,5,0,n)*x3y3*80.+
     & c(4,5,1,n)*x3y3z1*80.+c(4,5,2,n)*x3y3z2*80.+c(4,5,3,n)*x3y3z3*
     & 80.+c(4,5,4,n)*x3y3z4*80.+c(4,5,5,n)*x3y3z5*80.+c(5,2,0,n)*x4*
     & 10.+c(5,2,1,n)*x4z1*10.+c(5,2,2,n)*x4z2*10.+c(5,2,3,n)*x4z3*
     & 10.+c(5,2,4,n)*x4z4*10.+c(5,2,5,n)*x4z5*10.+c(5,3,0,n)*x4y1*
     & 30.+c(5,3,1,n)*x4y1z1*30.+c(5,3,2,n)*x4y1z2*30.+c(5,3,3,n)*
     & x4y1z3*30.+c(5,3,4,n)*x4y1z4*30.+c(5,3,5,n)*x4y1z5*30.+c(5,4,0,
     & n)*x4y2*60.+c(5,4,1,n)*x4y2z1*60.+c(5,4,2,n)*x4y2z2*60.+c(5,4,
     & 3,n)*x4y2z3*60.+c(5,4,4,n)*x4y2z4*60.+c(5,4,5,n)*x4y2z5*60.+c(
     & 5,5,0,n)*x4y3*100.+c(5,5,1,n)*x4y3z1*100.+c(5,5,2,n)*x4y3z2*
     & 100.+c(5,5,3,n)*x4y3z3*100.+c(5,5,4,n)*x4y3z4*100.+c(5,5,5,n)*
     & x4y3z5*100.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.1.and.dy.eq.2.and.dz.eq.1 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      y3z4=y2z4*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y2z4=x1y1z4*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x1y3z4=x1y2z4*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y1z4=x1y1z4*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y2z4=x1y2z4*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y3z3=x1y3z3*x1
      x2y3z4=x1y3z4*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3z4=x2z4*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x3y1z4=x2y1z4*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y2z3=x2y2z3*x1
      x3y2z4=x2y2z4*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x3y3z2=x2y3z2*x1
      x3y3z3=x2y3z3*x1
      x3y3z4=x2y3z4*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4z2=x3z2*x1
      x4z3=x3z3*x1
      x4z4=x3z4*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y1z2=x3y1z2*x1
      x4y1z3=x3y1z3*x1
      x4y1z4=x3y1z4*x1
      x4y2=x3y2*x1
      x4y2z1=x3y2z1*x1
      x4y2z2=x3y2z2*x1
      x4y2z3=x3y2z3*x1
      x4y2z4=x3y2z4*x1
      x4y3=x3y3*x1
      x4y3z1=x3y3z1*x1
      x4y3z2=x3y3z2*x1
      x4y3z3=x3y3z3*x1
      x4y3z4=x3y3z4*x1
      r(i1,i2,i3,n)=(c(1,2,1,n)*2.+c(1,2,2,n)*z1*4.+c(1,2,3,n)*z2*6.+c(
     & 1,2,4,n)*z3*8.+c(1,2,5,n)*z4*10.+c(1,3,1,n)*y1*6.+c(1,3,2,n)*
     & y1z1*12.+c(1,3,3,n)*y1z2*18.+c(1,3,4,n)*y1z3*24.+c(1,3,5,n)*
     & y1z4*30.+c(1,4,1,n)*y2*12.+c(1,4,2,n)*y2z1*24.+c(1,4,3,n)*y2z2*
     & 36.+c(1,4,4,n)*y2z3*48.+c(1,4,5,n)*y2z4*60.+c(1,5,1,n)*y3*20.+
     & c(1,5,2,n)*y3z1*40.+c(1,5,3,n)*y3z2*60.+c(1,5,4,n)*y3z3*80.+c(
     & 1,5,5,n)*y3z4*100.+c(2,2,1,n)*x1*4.+c(2,2,2,n)*x1z1*8.+c(2,2,3,
     & n)*x1z2*12.+c(2,2,4,n)*x1z3*16.+c(2,2,5,n)*x1z4*20.+c(2,3,1,n)*
     & x1y1*12.+c(2,3,2,n)*x1y1z1*24.+c(2,3,3,n)*x1y1z2*36.+c(2,3,4,n)
     & *x1y1z3*48.+c(2,3,5,n)*x1y1z4*60.+c(2,4,1,n)*x1y2*24.+c(2,4,2,
     & n)*x1y2z1*48.+c(2,4,3,n)*x1y2z2*72.+c(2,4,4,n)*x1y2z3*96.+c(2,
     & 4,5,n)*x1y2z4*120.+c(2,5,1,n)*x1y3*40.+c(2,5,2,n)*x1y3z1*80.+c(
     & 2,5,3,n)*x1y3z2*120.+c(2,5,4,n)*x1y3z3*160.+c(2,5,5,n)*x1y3z4*
     & 200.+c(3,2,1,n)*x2*6.+c(3,2,2,n)*x2z1*12.+c(3,2,3,n)*x2z2*18.+
     & c(3,2,4,n)*x2z3*24.+c(3,2,5,n)*x2z4*30.+c(3,3,1,n)*x2y1*18.+c(
     & 3,3,2,n)*x2y1z1*36.+c(3,3,3,n)*x2y1z2*54.+c(3,3,4,n)*x2y1z3*
     & 72.+c(3,3,5,n)*x2y1z4*90.+c(3,4,1,n)*x2y2*36.+c(3,4,2,n)*
     & x2y2z1*72.+c(3,4,3,n)*x2y2z2*108.+c(3,4,4,n)*x2y2z3*144.+c(3,4,
     & 5,n)*x2y2z4*180.+c(3,5,1,n)*x2y3*60.+c(3,5,2,n)*x2y3z1*120.+c(
     & 3,5,3,n)*x2y3z2*180.+c(3,5,4,n)*x2y3z3*240.+c(3,5,5,n)*x2y3z4*
     & 300.+c(4,2,1,n)*x3*8.+c(4,2,2,n)*x3z1*16.+c(4,2,3,n)*x3z2*24.+
     & c(4,2,4,n)*x3z3*32.+c(4,2,5,n)*x3z4*40.+c(4,3,1,n)*x3y1*24.+c(
     & 4,3,2,n)*x3y1z1*48.+c(4,3,3,n)*x3y1z2*72.+c(4,3,4,n)*x3y1z3*
     & 96.+c(4,3,5,n)*x3y1z4*120.+c(4,4,1,n)*x3y2*48.+c(4,4,2,n)*
     & x3y2z1*96.+c(4,4,3,n)*x3y2z2*144.+c(4,4,4,n)*x3y2z3*192.+c(4,4,
     & 5,n)*x3y2z4*240.+c(4,5,1,n)*x3y3*80.+c(4,5,2,n)*x3y3z1*160.+c(
     & 4,5,3,n)*x3y3z2*240.+c(4,5,4,n)*x3y3z3*320.+c(4,5,5,n)*x3y3z4*
     & 400.+c(5,2,1,n)*x4*10.+c(5,2,2,n)*x4z1*20.+c(5,2,3,n)*x4z2*30.+
     & c(5,2,4,n)*x4z3*40.+c(5,2,5,n)*x4z4*50.+c(5,3,1,n)*x4y1*30.+c(
     & 5,3,2,n)*x4y1z1*60.+c(5,3,3,n)*x4y1z2*90.+c(5,3,4,n)*x4y1z3*
     & 120.+c(5,3,5,n)*x4y1z4*150.+c(5,4,1,n)*x4y2*60.+c(5,4,2,n)*
     & x4y2z1*120.+c(5,4,3,n)*x4y2z2*180.+c(5,4,4,n)*x4y2z3*240.+c(5,
     & 4,5,n)*x4y2z4*300.+c(5,5,1,n)*x4y3*100.+c(5,5,2,n)*x4y3z1*200.+
     & c(5,5,3,n)*x4y3z2*300.+c(5,5,4,n)*x4y3z3*400.+c(5,5,5,n)*
     & x4y3z4*500.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.1.and.dy.eq.2.and.dz.eq.2 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y3z3=x1y3z3*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y2z3=x2y2z3*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x3y3z2=x2y3z2*x1
      x3y3z3=x2y3z3*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4z2=x3z2*x1
      x4z3=x3z3*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y1z2=x3y1z2*x1
      x4y1z3=x3y1z3*x1
      x4y2=x3y2*x1
      x4y2z1=x3y2z1*x1
      x4y2z2=x3y2z2*x1
      x4y2z3=x3y2z3*x1
      x4y3=x3y3*x1
      x4y3z1=x3y3z1*x1
      x4y3z2=x3y3z2*x1
      x4y3z3=x3y3z3*x1
      r(i1,i2,i3,n)=(c(1,2,2,n)*4.+c(1,2,3,n)*z1*12.+c(1,2,4,n)*z2*24.+
     & c(1,2,5,n)*z3*40.+c(1,3,2,n)*y1*12.+c(1,3,3,n)*y1z1*36.+c(1,3,
     & 4,n)*y1z2*72.+c(1,3,5,n)*y1z3*120.+c(1,4,2,n)*y2*24.+c(1,4,3,n)
     & *y2z1*72.+c(1,4,4,n)*y2z2*144.+c(1,4,5,n)*y2z3*240.+c(1,5,2,n)*
     & y3*40.+c(1,5,3,n)*y3z1*120.+c(1,5,4,n)*y3z2*240.+c(1,5,5,n)*
     & y3z3*400.+c(2,2,2,n)*x1*8.+c(2,2,3,n)*x1z1*24.+c(2,2,4,n)*x1z2*
     & 48.+c(2,2,5,n)*x1z3*80.+c(2,3,2,n)*x1y1*24.+c(2,3,3,n)*x1y1z1*
     & 72.+c(2,3,4,n)*x1y1z2*144.+c(2,3,5,n)*x1y1z3*240.+c(2,4,2,n)*
     & x1y2*48.+c(2,4,3,n)*x1y2z1*144.+c(2,4,4,n)*x1y2z2*288.+c(2,4,5,
     & n)*x1y2z3*480.+c(2,5,2,n)*x1y3*80.+c(2,5,3,n)*x1y3z1*240.+c(2,
     & 5,4,n)*x1y3z2*480.+c(2,5,5,n)*x1y3z3*800.+c(3,2,2,n)*x2*12.+c(
     & 3,2,3,n)*x2z1*36.+c(3,2,4,n)*x2z2*72.+c(3,2,5,n)*x2z3*120.+c(3,
     & 3,2,n)*x2y1*36.+c(3,3,3,n)*x2y1z1*108.+c(3,3,4,n)*x2y1z2*216.+
     & c(3,3,5,n)*x2y1z3*360.+c(3,4,2,n)*x2y2*72.+c(3,4,3,n)*x2y2z1*
     & 216.+c(3,4,4,n)*x2y2z2*432.+c(3,4,5,n)*x2y2z3*720.+c(3,5,2,n)*
     & x2y3*120.+c(3,5,3,n)*x2y3z1*360.+c(3,5,4,n)*x2y3z2*720.+c(3,5,
     & 5,n)*x2y3z3*1200.+c(4,2,2,n)*x3*16.+c(4,2,3,n)*x3z1*48.+c(4,2,
     & 4,n)*x3z2*96.+c(4,2,5,n)*x3z3*160.+c(4,3,2,n)*x3y1*48.+c(4,3,3,
     & n)*x3y1z1*144.+c(4,3,4,n)*x3y1z2*288.+c(4,3,5,n)*x3y1z3*480.+c(
     & 4,4,2,n)*x3y2*96.+c(4,4,3,n)*x3y2z1*288.+c(4,4,4,n)*x3y2z2*
     & 576.+c(4,4,5,n)*x3y2z3*960.+c(4,5,2,n)*x3y3*160.+c(4,5,3,n)*
     & x3y3z1*480.+c(4,5,4,n)*x3y3z2*960.+c(4,5,5,n)*x3y3z3*1600.+c(5,
     & 2,2,n)*x4*20.+c(5,2,3,n)*x4z1*60.+c(5,2,4,n)*x4z2*120.+c(5,2,5,
     & n)*x4z3*200.+c(5,3,2,n)*x4y1*60.+c(5,3,3,n)*x4y1z1*180.+c(5,3,
     & 4,n)*x4y1z2*360.+c(5,3,5,n)*x4y1z3*600.+c(5,4,2,n)*x4y2*120.+c(
     & 5,4,3,n)*x4y2z1*360.+c(5,4,4,n)*x4y2z2*720.+c(5,4,5,n)*x4y2z3*
     & 1200.+c(5,5,2,n)*x4y3*200.+c(5,5,3,n)*x4y3z1*600.+c(5,5,4,n)*
     & x4y3z2*1200.+c(5,5,5,n)*x4y3z3*2000.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.1.and.dy.eq.2.and.dz.eq.3 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x3y3z2=x2y3z2*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4z2=x3z2*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y1z2=x3y1z2*x1
      x4y2=x3y2*x1
      x4y2z1=x3y2z1*x1
      x4y2z2=x3y2z2*x1
      x4y3=x3y3*x1
      x4y3z1=x3y3z1*x1
      x4y3z2=x3y3z2*x1
      r(i1,i2,i3,n)=(c(1,2,3,n)*12.+c(1,2,4,n)*z1*48.+c(1,2,5,n)*z2*
     & 120.+c(1,3,3,n)*y1*36.+c(1,3,4,n)*y1z1*144.+c(1,3,5,n)*y1z2*
     & 360.+c(1,4,3,n)*y2*72.+c(1,4,4,n)*y2z1*288.+c(1,4,5,n)*y2z2*
     & 720.+c(1,5,3,n)*y3*120.+c(1,5,4,n)*y3z1*480.+c(1,5,5,n)*y3z2*
     & 1200.+c(2,2,3,n)*x1*24.+c(2,2,4,n)*x1z1*96.+c(2,2,5,n)*x1z2*
     & 240.+c(2,3,3,n)*x1y1*72.+c(2,3,4,n)*x1y1z1*288.+c(2,3,5,n)*
     & x1y1z2*720.+c(2,4,3,n)*x1y2*144.+c(2,4,4,n)*x1y2z1*576.+c(2,4,
     & 5,n)*x1y2z2*1440.+c(2,5,3,n)*x1y3*240.+c(2,5,4,n)*x1y3z1*960.+
     & c(2,5,5,n)*x1y3z2*2400.+c(3,2,3,n)*x2*36.+c(3,2,4,n)*x2z1*144.+
     & c(3,2,5,n)*x2z2*360.+c(3,3,3,n)*x2y1*108.+c(3,3,4,n)*x2y1z1*
     & 432.+c(3,3,5,n)*x2y1z2*1080.+c(3,4,3,n)*x2y2*216.+c(3,4,4,n)*
     & x2y2z1*864.+c(3,4,5,n)*x2y2z2*2160.+c(3,5,3,n)*x2y3*360.+c(3,5,
     & 4,n)*x2y3z1*1440.+c(3,5,5,n)*x2y3z2*3600.+c(4,2,3,n)*x3*48.+c(
     & 4,2,4,n)*x3z1*192.+c(4,2,5,n)*x3z2*480.+c(4,3,3,n)*x3y1*144.+c(
     & 4,3,4,n)*x3y1z1*576.+c(4,3,5,n)*x3y1z2*1440.+c(4,4,3,n)*x3y2*
     & 288.+c(4,4,4,n)*x3y2z1*1152.+c(4,4,5,n)*x3y2z2*2880.+c(4,5,3,n)
     & *x3y3*480.+c(4,5,4,n)*x3y3z1*1920.+c(4,5,5,n)*x3y3z2*4800.+c(5,
     & 2,3,n)*x4*60.+c(5,2,4,n)*x4z1*240.+c(5,2,5,n)*x4z2*600.+c(5,3,
     & 3,n)*x4y1*180.+c(5,3,4,n)*x4y1z1*720.+c(5,3,5,n)*x4y1z2*1800.+
     & c(5,4,3,n)*x4y2*360.+c(5,4,4,n)*x4y2z1*1440.+c(5,4,5,n)*x4y2z2*
     & 3600.+c(5,5,3,n)*x4y3*600.+c(5,5,4,n)*x4y3z1*2400.+c(5,5,5,n)*
     & x4y3z2*6000.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.1.and.dy.eq.2.and.dz.eq.4 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y3=y2*y1
      y3z1=y2z1*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y2=x3y2*x1
      x4y2z1=x3y2z1*x1
      x4y3=x3y3*x1
      x4y3z1=x3y3z1*x1
      r(i1,i2,i3,n)=(c(1,2,4,n)*48.+c(1,2,5,n)*z1*240.+c(1,3,4,n)*y1*
     & 144.+c(1,3,5,n)*y1z1*720.+c(1,4,4,n)*y2*288.+c(1,4,5,n)*y2z1*
     & 1440.+c(1,5,4,n)*y3*480.+c(1,5,5,n)*y3z1*2400.+c(2,2,4,n)*x1*
     & 96.+c(2,2,5,n)*x1z1*480.+c(2,3,4,n)*x1y1*288.+c(2,3,5,n)*
     & x1y1z1*1440.+c(2,4,4,n)*x1y2*576.+c(2,4,5,n)*x1y2z1*2880.+c(2,
     & 5,4,n)*x1y3*960.+c(2,5,5,n)*x1y3z1*4800.+c(3,2,4,n)*x2*144.+c(
     & 3,2,5,n)*x2z1*720.+c(3,3,4,n)*x2y1*432.+c(3,3,5,n)*x2y1z1*
     & 2160.+c(3,4,4,n)*x2y2*864.+c(3,4,5,n)*x2y2z1*4320.+c(3,5,4,n)*
     & x2y3*1440.+c(3,5,5,n)*x2y3z1*7200.+c(4,2,4,n)*x3*192.+c(4,2,5,
     & n)*x3z1*960.+c(4,3,4,n)*x3y1*576.+c(4,3,5,n)*x3y1z1*2880.+c(4,
     & 4,4,n)*x3y2*1152.+c(4,4,5,n)*x3y2z1*5760.+c(4,5,4,n)*x3y3*
     & 1920.+c(4,5,5,n)*x3y3z1*9600.+c(5,2,4,n)*x4*240.+c(5,2,5,n)*
     & x4z1*1200.+c(5,3,4,n)*x4y1*720.+c(5,3,5,n)*x4y1z1*3600.+c(5,4,
     & 4,n)*x4y2*1440.+c(5,4,5,n)*x4y2z1*7200.+c(5,5,4,n)*x4y3*2400.+
     & c(5,5,5,n)*x4y3z1*12000.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.1.and.dy.eq.3.and.dz.eq.0 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y1z5=y1z4*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
      y2z5=y1z5*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1z5=x1z4*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y1z5=x1y1z4*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y2z4=x1y1z4*y1
      x1y2z5=x1y1z5*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x2z5=x1z5*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y1z4=x1y1z4*x1
      x2y1z5=x1y1z5*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y2z4=x1y2z4*x1
      x2y2z5=x1y2z5*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3z4=x2z4*x1
      x3z5=x2z5*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x3y1z4=x2y1z4*x1
      x3y1z5=x2y1z5*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y2z3=x2y2z3*x1
      x3y2z4=x2y2z4*x1
      x3y2z5=x2y2z5*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4z2=x3z2*x1
      x4z3=x3z3*x1
      x4z4=x3z4*x1
      x4z5=x3z5*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y1z2=x3y1z2*x1
      x4y1z3=x3y1z3*x1
      x4y1z4=x3y1z4*x1
      x4y1z5=x3y1z5*x1
      x4y2=x3y2*x1
      x4y2z1=x3y2z1*x1
      x4y2z2=x3y2z2*x1
      x4y2z3=x3y2z3*x1
      x4y2z4=x3y2z4*x1
      x4y2z5=x3y2z5*x1
      r(i1,i2,i3,n)=(c(1,3,0,n)*6.+c(1,3,1,n)*z1*6.+c(1,3,2,n)*z2*6.+c(
     & 1,3,3,n)*z3*6.+c(1,3,4,n)*z4*6.+c(1,3,5,n)*z5*6.+c(1,4,0,n)*y1*
     & 24.+c(1,4,1,n)*y1z1*24.+c(1,4,2,n)*y1z2*24.+c(1,4,3,n)*y1z3*
     & 24.+c(1,4,4,n)*y1z4*24.+c(1,4,5,n)*y1z5*24.+c(1,5,0,n)*y2*60.+
     & c(1,5,1,n)*y2z1*60.+c(1,5,2,n)*y2z2*60.+c(1,5,3,n)*y2z3*60.+c(
     & 1,5,4,n)*y2z4*60.+c(1,5,5,n)*y2z5*60.+c(2,3,0,n)*x1*12.+c(2,3,
     & 1,n)*x1z1*12.+c(2,3,2,n)*x1z2*12.+c(2,3,3,n)*x1z3*12.+c(2,3,4,
     & n)*x1z4*12.+c(2,3,5,n)*x1z5*12.+c(2,4,0,n)*x1y1*48.+c(2,4,1,n)*
     & x1y1z1*48.+c(2,4,2,n)*x1y1z2*48.+c(2,4,3,n)*x1y1z3*48.+c(2,4,4,
     & n)*x1y1z4*48.+c(2,4,5,n)*x1y1z5*48.+c(2,5,0,n)*x1y2*120.+c(2,5,
     & 1,n)*x1y2z1*120.+c(2,5,2,n)*x1y2z2*120.+c(2,5,3,n)*x1y2z3*120.+
     & c(2,5,4,n)*x1y2z4*120.+c(2,5,5,n)*x1y2z5*120.+c(3,3,0,n)*x2*
     & 18.+c(3,3,1,n)*x2z1*18.+c(3,3,2,n)*x2z2*18.+c(3,3,3,n)*x2z3*
     & 18.+c(3,3,4,n)*x2z4*18.+c(3,3,5,n)*x2z5*18.+c(3,4,0,n)*x2y1*
     & 72.+c(3,4,1,n)*x2y1z1*72.+c(3,4,2,n)*x2y1z2*72.+c(3,4,3,n)*
     & x2y1z3*72.+c(3,4,4,n)*x2y1z4*72.+c(3,4,5,n)*x2y1z5*72.+c(3,5,0,
     & n)*x2y2*180.+c(3,5,1,n)*x2y2z1*180.+c(3,5,2,n)*x2y2z2*180.+c(3,
     & 5,3,n)*x2y2z3*180.+c(3,5,4,n)*x2y2z4*180.+c(3,5,5,n)*x2y2z5*
     & 180.+c(4,3,0,n)*x3*24.+c(4,3,1,n)*x3z1*24.+c(4,3,2,n)*x3z2*24.+
     & c(4,3,3,n)*x3z3*24.+c(4,3,4,n)*x3z4*24.+c(4,3,5,n)*x3z5*24.+c(
     & 4,4,0,n)*x3y1*96.+c(4,4,1,n)*x3y1z1*96.+c(4,4,2,n)*x3y1z2*96.+
     & c(4,4,3,n)*x3y1z3*96.+c(4,4,4,n)*x3y1z4*96.+c(4,4,5,n)*x3y1z5*
     & 96.+c(4,5,0,n)*x3y2*240.+c(4,5,1,n)*x3y2z1*240.+c(4,5,2,n)*
     & x3y2z2*240.+c(4,5,3,n)*x3y2z3*240.+c(4,5,4,n)*x3y2z4*240.+c(4,
     & 5,5,n)*x3y2z5*240.+c(5,3,0,n)*x4*30.+c(5,3,1,n)*x4z1*30.+c(5,3,
     & 2,n)*x4z2*30.+c(5,3,3,n)*x4z3*30.+c(5,3,4,n)*x4z4*30.+c(5,3,5,
     & n)*x4z5*30.+c(5,4,0,n)*x4y1*120.+c(5,4,1,n)*x4y1z1*120.+c(5,4,
     & 2,n)*x4y1z2*120.+c(5,4,3,n)*x4y1z3*120.+c(5,4,4,n)*x4y1z4*120.+
     & c(5,4,5,n)*x4y1z5*120.+c(5,5,0,n)*x4y2*300.+c(5,5,1,n)*x4y2z1*
     & 300.+c(5,5,2,n)*x4y2z2*300.+c(5,5,3,n)*x4y2z3*300.+c(5,5,4,n)*
     & x4y2z4*300.+c(5,5,5,n)*x4y2z5*300.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.1.and.dy.eq.3.and.dz.eq.1 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y2z4=x1y1z4*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y1z4=x1y1z4*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y2z4=x1y2z4*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3z4=x2z4*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x3y1z4=x2y1z4*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y2z3=x2y2z3*x1
      x3y2z4=x2y2z4*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4z2=x3z2*x1
      x4z3=x3z3*x1
      x4z4=x3z4*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y1z2=x3y1z2*x1
      x4y1z3=x3y1z3*x1
      x4y1z4=x3y1z4*x1
      x4y2=x3y2*x1
      x4y2z1=x3y2z1*x1
      x4y2z2=x3y2z2*x1
      x4y2z3=x3y2z3*x1
      x4y2z4=x3y2z4*x1
      r(i1,i2,i3,n)=(c(1,3,1,n)*6.+c(1,3,2,n)*z1*12.+c(1,3,3,n)*z2*18.+
     & c(1,3,4,n)*z3*24.+c(1,3,5,n)*z4*30.+c(1,4,1,n)*y1*24.+c(1,4,2,
     & n)*y1z1*48.+c(1,4,3,n)*y1z2*72.+c(1,4,4,n)*y1z3*96.+c(1,4,5,n)*
     & y1z4*120.+c(1,5,1,n)*y2*60.+c(1,5,2,n)*y2z1*120.+c(1,5,3,n)*
     & y2z2*180.+c(1,5,4,n)*y2z3*240.+c(1,5,5,n)*y2z4*300.+c(2,3,1,n)*
     & x1*12.+c(2,3,2,n)*x1z1*24.+c(2,3,3,n)*x1z2*36.+c(2,3,4,n)*x1z3*
     & 48.+c(2,3,5,n)*x1z4*60.+c(2,4,1,n)*x1y1*48.+c(2,4,2,n)*x1y1z1*
     & 96.+c(2,4,3,n)*x1y1z2*144.+c(2,4,4,n)*x1y1z3*192.+c(2,4,5,n)*
     & x1y1z4*240.+c(2,5,1,n)*x1y2*120.+c(2,5,2,n)*x1y2z1*240.+c(2,5,
     & 3,n)*x1y2z2*360.+c(2,5,4,n)*x1y2z3*480.+c(2,5,5,n)*x1y2z4*600.+
     & c(3,3,1,n)*x2*18.+c(3,3,2,n)*x2z1*36.+c(3,3,3,n)*x2z2*54.+c(3,
     & 3,4,n)*x2z3*72.+c(3,3,5,n)*x2z4*90.+c(3,4,1,n)*x2y1*72.+c(3,4,
     & 2,n)*x2y1z1*144.+c(3,4,3,n)*x2y1z2*216.+c(3,4,4,n)*x2y1z3*288.+
     & c(3,4,5,n)*x2y1z4*360.+c(3,5,1,n)*x2y2*180.+c(3,5,2,n)*x2y2z1*
     & 360.+c(3,5,3,n)*x2y2z2*540.+c(3,5,4,n)*x2y2z3*720.+c(3,5,5,n)*
     & x2y2z4*900.+c(4,3,1,n)*x3*24.+c(4,3,2,n)*x3z1*48.+c(4,3,3,n)*
     & x3z2*72.+c(4,3,4,n)*x3z3*96.+c(4,3,5,n)*x3z4*120.+c(4,4,1,n)*
     & x3y1*96.+c(4,4,2,n)*x3y1z1*192.+c(4,4,3,n)*x3y1z2*288.+c(4,4,4,
     & n)*x3y1z3*384.+c(4,4,5,n)*x3y1z4*480.+c(4,5,1,n)*x3y2*240.+c(4,
     & 5,2,n)*x3y2z1*480.+c(4,5,3,n)*x3y2z2*720.+c(4,5,4,n)*x3y2z3*
     & 960.+c(4,5,5,n)*x3y2z4*1200.+c(5,3,1,n)*x4*30.+c(5,3,2,n)*x4z1*
     & 60.+c(5,3,3,n)*x4z2*90.+c(5,3,4,n)*x4z3*120.+c(5,3,5,n)*x4z4*
     & 150.+c(5,4,1,n)*x4y1*120.+c(5,4,2,n)*x4y1z1*240.+c(5,4,3,n)*
     & x4y1z2*360.+c(5,4,4,n)*x4y1z3*480.+c(5,4,5,n)*x4y1z4*600.+c(5,
     & 5,1,n)*x4y2*300.+c(5,5,2,n)*x4y2z1*600.+c(5,5,3,n)*x4y2z2*900.+
     & c(5,5,4,n)*x4y2z3*1200.+c(5,5,5,n)*x4y2z4*1500.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.1.and.dy.eq.3.and.dz.eq.2 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y2z3=x2y2z3*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4z2=x3z2*x1
      x4z3=x3z3*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y1z2=x3y1z2*x1
      x4y1z3=x3y1z3*x1
      x4y2=x3y2*x1
      x4y2z1=x3y2z1*x1
      x4y2z2=x3y2z2*x1
      x4y2z3=x3y2z3*x1
      r(i1,i2,i3,n)=(c(1,3,2,n)*12.+c(1,3,3,n)*z1*36.+c(1,3,4,n)*z2*
     & 72.+c(1,3,5,n)*z3*120.+c(1,4,2,n)*y1*48.+c(1,4,3,n)*y1z1*144.+
     & c(1,4,4,n)*y1z2*288.+c(1,4,5,n)*y1z3*480.+c(1,5,2,n)*y2*120.+c(
     & 1,5,3,n)*y2z1*360.+c(1,5,4,n)*y2z2*720.+c(1,5,5,n)*y2z3*1200.+
     & c(2,3,2,n)*x1*24.+c(2,3,3,n)*x1z1*72.+c(2,3,4,n)*x1z2*144.+c(2,
     & 3,5,n)*x1z3*240.+c(2,4,2,n)*x1y1*96.+c(2,4,3,n)*x1y1z1*288.+c(
     & 2,4,4,n)*x1y1z2*576.+c(2,4,5,n)*x1y1z3*960.+c(2,5,2,n)*x1y2*
     & 240.+c(2,5,3,n)*x1y2z1*720.+c(2,5,4,n)*x1y2z2*1440.+c(2,5,5,n)*
     & x1y2z3*2400.+c(3,3,2,n)*x2*36.+c(3,3,3,n)*x2z1*108.+c(3,3,4,n)*
     & x2z2*216.+c(3,3,5,n)*x2z3*360.+c(3,4,2,n)*x2y1*144.+c(3,4,3,n)*
     & x2y1z1*432.+c(3,4,4,n)*x2y1z2*864.+c(3,4,5,n)*x2y1z3*1440.+c(3,
     & 5,2,n)*x2y2*360.+c(3,5,3,n)*x2y2z1*1080.+c(3,5,4,n)*x2y2z2*
     & 2160.+c(3,5,5,n)*x2y2z3*3600.+c(4,3,2,n)*x3*48.+c(4,3,3,n)*
     & x3z1*144.+c(4,3,4,n)*x3z2*288.+c(4,3,5,n)*x3z3*480.+c(4,4,2,n)*
     & x3y1*192.+c(4,4,3,n)*x3y1z1*576.+c(4,4,4,n)*x3y1z2*1152.+c(4,4,
     & 5,n)*x3y1z3*1920.+c(4,5,2,n)*x3y2*480.+c(4,5,3,n)*x3y2z1*1440.+
     & c(4,5,4,n)*x3y2z2*2880.+c(4,5,5,n)*x3y2z3*4800.+c(5,3,2,n)*x4*
     & 60.+c(5,3,3,n)*x4z1*180.+c(5,3,4,n)*x4z2*360.+c(5,3,5,n)*x4z3*
     & 600.+c(5,4,2,n)*x4y1*240.+c(5,4,3,n)*x4y1z1*720.+c(5,4,4,n)*
     & x4y1z2*1440.+c(5,4,5,n)*x4y1z3*2400.+c(5,5,2,n)*x4y2*600.+c(5,
     & 5,3,n)*x4y2z1*1800.+c(5,5,4,n)*x4y2z2*3600.+c(5,5,5,n)*x4y2z3*
     & 6000.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.1.and.dy.eq.3.and.dz.eq.3 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4z2=x3z2*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y1z2=x3y1z2*x1
      x4y2=x3y2*x1
      x4y2z1=x3y2z1*x1
      x4y2z2=x3y2z2*x1
      r(i1,i2,i3,n)=(c(1,3,3,n)*36.+c(1,3,4,n)*z1*144.+c(1,3,5,n)*z2*
     & 360.+c(1,4,3,n)*y1*144.+c(1,4,4,n)*y1z1*576.+c(1,4,5,n)*y1z2*
     & 1440.+c(1,5,3,n)*y2*360.+c(1,5,4,n)*y2z1*1440.+c(1,5,5,n)*y2z2*
     & 3600.+c(2,3,3,n)*x1*72.+c(2,3,4,n)*x1z1*288.+c(2,3,5,n)*x1z2*
     & 720.+c(2,4,3,n)*x1y1*288.+c(2,4,4,n)*x1y1z1*1152.+c(2,4,5,n)*
     & x1y1z2*2880.+c(2,5,3,n)*x1y2*720.+c(2,5,4,n)*x1y2z1*2880.+c(2,
     & 5,5,n)*x1y2z2*7200.+c(3,3,3,n)*x2*108.+c(3,3,4,n)*x2z1*432.+c(
     & 3,3,5,n)*x2z2*1080.+c(3,4,3,n)*x2y1*432.+c(3,4,4,n)*x2y1z1*
     & 1728.+c(3,4,5,n)*x2y1z2*4320.+c(3,5,3,n)*x2y2*1080.+c(3,5,4,n)*
     & x2y2z1*4320.+c(3,5,5,n)*x2y2z2*10800.+c(4,3,3,n)*x3*144.+c(4,3,
     & 4,n)*x3z1*576.+c(4,3,5,n)*x3z2*1440.+c(4,4,3,n)*x3y1*576.+c(4,
     & 4,4,n)*x3y1z1*2304.+c(4,4,5,n)*x3y1z2*5760.+c(4,5,3,n)*x3y2*
     & 1440.+c(4,5,4,n)*x3y2z1*5760.+c(4,5,5,n)*x3y2z2*14400.+c(5,3,3,
     & n)*x4*180.+c(5,3,4,n)*x4z1*720.+c(5,3,5,n)*x4z2*1800.+c(5,4,3,
     & n)*x4y1*720.+c(5,4,4,n)*x4y1z1*2880.+c(5,4,5,n)*x4y1z2*7200.+c(
     & 5,5,3,n)*x4y2*1800.+c(5,5,4,n)*x4y2z1*7200.+c(5,5,5,n)*x4y2z2*
     & 18000.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.1.and.dy.eq.3.and.dz.eq.4 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y2=x3y2*x1
      x4y2z1=x3y2z1*x1
      r(i1,i2,i3,n)=(c(1,3,4,n)*144.+c(1,3,5,n)*z1*720.+c(1,4,4,n)*y1*
     & 576.+c(1,4,5,n)*y1z1*2880.+c(1,5,4,n)*y2*1440.+c(1,5,5,n)*y2z1*
     & 7200.+c(2,3,4,n)*x1*288.+c(2,3,5,n)*x1z1*1440.+c(2,4,4,n)*x1y1*
     & 1152.+c(2,4,5,n)*x1y1z1*5760.+c(2,5,4,n)*x1y2*2880.+c(2,5,5,n)*
     & x1y2z1*14400.+c(3,3,4,n)*x2*432.+c(3,3,5,n)*x2z1*2160.+c(3,4,4,
     & n)*x2y1*1728.+c(3,4,5,n)*x2y1z1*8640.+c(3,5,4,n)*x2y2*4320.+c(
     & 3,5,5,n)*x2y2z1*21600.+c(4,3,4,n)*x3*576.+c(4,3,5,n)*x3z1*
     & 2880.+c(4,4,4,n)*x3y1*2304.+c(4,4,5,n)*x3y1z1*11520.+c(4,5,4,n)
     & *x3y2*5760.+c(4,5,5,n)*x3y2z1*28800.+c(5,3,4,n)*x4*720.+c(5,3,
     & 5,n)*x4z1*3600.+c(5,4,4,n)*x4y1*2880.+c(5,4,5,n)*x4y1z1*14400.+
     & c(5,5,4,n)*x4y2*7200.+c(5,5,5,n)*x4y2z1*36000.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.1.and.dy.eq.4.and.dz.eq.0 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y1z5=y1z4*z1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1z5=x1z4*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y1z5=x1y1z4*z1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x2z5=x1z5*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y1z4=x1y1z4*x1
      x2y1z5=x1y1z5*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3z4=x2z4*x1
      x3z5=x2z5*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x3y1z4=x2y1z4*x1
      x3y1z5=x2y1z5*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4z2=x3z2*x1
      x4z3=x3z3*x1
      x4z4=x3z4*x1
      x4z5=x3z5*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y1z2=x3y1z2*x1
      x4y1z3=x3y1z3*x1
      x4y1z4=x3y1z4*x1
      x4y1z5=x3y1z5*x1
      r(i1,i2,i3,n)=(c(1,4,0,n)*24.+c(1,4,1,n)*z1*24.+c(1,4,2,n)*z2*
     & 24.+c(1,4,3,n)*z3*24.+c(1,4,4,n)*z4*24.+c(1,4,5,n)*z5*24.+c(1,
     & 5,0,n)*y1*120.+c(1,5,1,n)*y1z1*120.+c(1,5,2,n)*y1z2*120.+c(1,5,
     & 3,n)*y1z3*120.+c(1,5,4,n)*y1z4*120.+c(1,5,5,n)*y1z5*120.+c(2,4,
     & 0,n)*x1*48.+c(2,4,1,n)*x1z1*48.+c(2,4,2,n)*x1z2*48.+c(2,4,3,n)*
     & x1z3*48.+c(2,4,4,n)*x1z4*48.+c(2,4,5,n)*x1z5*48.+c(2,5,0,n)*
     & x1y1*240.+c(2,5,1,n)*x1y1z1*240.+c(2,5,2,n)*x1y1z2*240.+c(2,5,
     & 3,n)*x1y1z3*240.+c(2,5,4,n)*x1y1z4*240.+c(2,5,5,n)*x1y1z5*240.+
     & c(3,4,0,n)*x2*72.+c(3,4,1,n)*x2z1*72.+c(3,4,2,n)*x2z2*72.+c(3,
     & 4,3,n)*x2z3*72.+c(3,4,4,n)*x2z4*72.+c(3,4,5,n)*x2z5*72.+c(3,5,
     & 0,n)*x2y1*360.+c(3,5,1,n)*x2y1z1*360.+c(3,5,2,n)*x2y1z2*360.+c(
     & 3,5,3,n)*x2y1z3*360.+c(3,5,4,n)*x2y1z4*360.+c(3,5,5,n)*x2y1z5*
     & 360.+c(4,4,0,n)*x3*96.+c(4,4,1,n)*x3z1*96.+c(4,4,2,n)*x3z2*96.+
     & c(4,4,3,n)*x3z3*96.+c(4,4,4,n)*x3z4*96.+c(4,4,5,n)*x3z5*96.+c(
     & 4,5,0,n)*x3y1*480.+c(4,5,1,n)*x3y1z1*480.+c(4,5,2,n)*x3y1z2*
     & 480.+c(4,5,3,n)*x3y1z3*480.+c(4,5,4,n)*x3y1z4*480.+c(4,5,5,n)*
     & x3y1z5*480.+c(5,4,0,n)*x4*120.+c(5,4,1,n)*x4z1*120.+c(5,4,2,n)*
     & x4z2*120.+c(5,4,3,n)*x4z3*120.+c(5,4,4,n)*x4z4*120.+c(5,4,5,n)*
     & x4z5*120.+c(5,5,0,n)*x4y1*600.+c(5,5,1,n)*x4y1z1*600.+c(5,5,2,
     & n)*x4y1z2*600.+c(5,5,3,n)*x4y1z3*600.+c(5,5,4,n)*x4y1z4*600.+c(
     & 5,5,5,n)*x4y1z5*600.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.1.and.dy.eq.4.and.dz.eq.1 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y1z4=x1y1z4*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3z4=x2z4*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x3y1z4=x2y1z4*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4z2=x3z2*x1
      x4z3=x3z3*x1
      x4z4=x3z4*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y1z2=x3y1z2*x1
      x4y1z3=x3y1z3*x1
      x4y1z4=x3y1z4*x1
      r(i1,i2,i3,n)=(c(1,4,1,n)*24.+c(1,4,2,n)*z1*48.+c(1,4,3,n)*z2*
     & 72.+c(1,4,4,n)*z3*96.+c(1,4,5,n)*z4*120.+c(1,5,1,n)*y1*120.+c(
     & 1,5,2,n)*y1z1*240.+c(1,5,3,n)*y1z2*360.+c(1,5,4,n)*y1z3*480.+c(
     & 1,5,5,n)*y1z4*600.+c(2,4,1,n)*x1*48.+c(2,4,2,n)*x1z1*96.+c(2,4,
     & 3,n)*x1z2*144.+c(2,4,4,n)*x1z3*192.+c(2,4,5,n)*x1z4*240.+c(2,5,
     & 1,n)*x1y1*240.+c(2,5,2,n)*x1y1z1*480.+c(2,5,3,n)*x1y1z2*720.+c(
     & 2,5,4,n)*x1y1z3*960.+c(2,5,5,n)*x1y1z4*1200.+c(3,4,1,n)*x2*72.+
     & c(3,4,2,n)*x2z1*144.+c(3,4,3,n)*x2z2*216.+c(3,4,4,n)*x2z3*288.+
     & c(3,4,5,n)*x2z4*360.+c(3,5,1,n)*x2y1*360.+c(3,5,2,n)*x2y1z1*
     & 720.+c(3,5,3,n)*x2y1z2*1080.+c(3,5,4,n)*x2y1z3*1440.+c(3,5,5,n)
     & *x2y1z4*1800.+c(4,4,1,n)*x3*96.+c(4,4,2,n)*x3z1*192.+c(4,4,3,n)
     & *x3z2*288.+c(4,4,4,n)*x3z3*384.+c(4,4,5,n)*x3z4*480.+c(4,5,1,n)
     & *x3y1*480.+c(4,5,2,n)*x3y1z1*960.+c(4,5,3,n)*x3y1z2*1440.+c(4,
     & 5,4,n)*x3y1z3*1920.+c(4,5,5,n)*x3y1z4*2400.+c(5,4,1,n)*x4*120.+
     & c(5,4,2,n)*x4z1*240.+c(5,4,3,n)*x4z2*360.+c(5,4,4,n)*x4z3*480.+
     & c(5,4,5,n)*x4z4*600.+c(5,5,1,n)*x4y1*600.+c(5,5,2,n)*x4y1z1*
     & 1200.+c(5,5,3,n)*x4y1z2*1800.+c(5,5,4,n)*x4y1z3*2400.+c(5,5,5,
     & n)*x4y1z4*3000.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.1.and.dy.eq.4.and.dz.eq.2 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4z2=x3z2*x1
      x4z3=x3z3*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y1z2=x3y1z2*x1
      x4y1z3=x3y1z3*x1
      r(i1,i2,i3,n)=(c(1,4,2,n)*48.+c(1,4,3,n)*z1*144.+c(1,4,4,n)*z2*
     & 288.+c(1,4,5,n)*z3*480.+c(1,5,2,n)*y1*240.+c(1,5,3,n)*y1z1*
     & 720.+c(1,5,4,n)*y1z2*1440.+c(1,5,5,n)*y1z3*2400.+c(2,4,2,n)*x1*
     & 96.+c(2,4,3,n)*x1z1*288.+c(2,4,4,n)*x1z2*576.+c(2,4,5,n)*x1z3*
     & 960.+c(2,5,2,n)*x1y1*480.+c(2,5,3,n)*x1y1z1*1440.+c(2,5,4,n)*
     & x1y1z2*2880.+c(2,5,5,n)*x1y1z3*4800.+c(3,4,2,n)*x2*144.+c(3,4,
     & 3,n)*x2z1*432.+c(3,4,4,n)*x2z2*864.+c(3,4,5,n)*x2z3*1440.+c(3,
     & 5,2,n)*x2y1*720.+c(3,5,3,n)*x2y1z1*2160.+c(3,5,4,n)*x2y1z2*
     & 4320.+c(3,5,5,n)*x2y1z3*7200.+c(4,4,2,n)*x3*192.+c(4,4,3,n)*
     & x3z1*576.+c(4,4,4,n)*x3z2*1152.+c(4,4,5,n)*x3z3*1920.+c(4,5,2,
     & n)*x3y1*960.+c(4,5,3,n)*x3y1z1*2880.+c(4,5,4,n)*x3y1z2*5760.+c(
     & 4,5,5,n)*x3y1z3*9600.+c(5,4,2,n)*x4*240.+c(5,4,3,n)*x4z1*720.+
     & c(5,4,4,n)*x4z2*1440.+c(5,4,5,n)*x4z3*2400.+c(5,5,2,n)*x4y1*
     & 1200.+c(5,5,3,n)*x4y1z1*3600.+c(5,5,4,n)*x4y1z2*7200.+c(5,5,5,
     & n)*x4y1z3*12000.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.1.and.dy.eq.4.and.dz.eq.3 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4z2=x3z2*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y1z2=x3y1z2*x1
      r(i1,i2,i3,n)=(c(1,4,3,n)*144.+c(1,4,4,n)*z1*576.+c(1,4,5,n)*z2*
     & 1440.+c(1,5,3,n)*y1*720.+c(1,5,4,n)*y1z1*2880.+c(1,5,5,n)*y1z2*
     & 7200.+c(2,4,3,n)*x1*288.+c(2,4,4,n)*x1z1*1152.+c(2,4,5,n)*x1z2*
     & 2880.+c(2,5,3,n)*x1y1*1440.+c(2,5,4,n)*x1y1z1*5760.+c(2,5,5,n)*
     & x1y1z2*14400.+c(3,4,3,n)*x2*432.+c(3,4,4,n)*x2z1*1728.+c(3,4,5,
     & n)*x2z2*4320.+c(3,5,3,n)*x2y1*2160.+c(3,5,4,n)*x2y1z1*8640.+c(
     & 3,5,5,n)*x2y1z2*21600.+c(4,4,3,n)*x3*576.+c(4,4,4,n)*x3z1*
     & 2304.+c(4,4,5,n)*x3z2*5760.+c(4,5,3,n)*x3y1*2880.+c(4,5,4,n)*
     & x3y1z1*11520.+c(4,5,5,n)*x3y1z2*28800.+c(5,4,3,n)*x4*720.+c(5,
     & 4,4,n)*x4z1*2880.+c(5,4,5,n)*x4z2*7200.+c(5,5,3,n)*x4y1*3600.+
     & c(5,5,4,n)*x4y1z1*14400.+c(5,5,5,n)*x4y1z2*36000.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.1.and.dy.eq.4.and.dz.eq.4 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x2=x1*x1
      x2z1=x1z1*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      r(i1,i2,i3,n)=(c(1,4,4,n)*576.+c(1,4,5,n)*z1*2880.+c(1,5,4,n)*y1*
     & 2880.+c(1,5,5,n)*y1z1*14400.+c(2,4,4,n)*x1*1152.+c(2,4,5,n)*
     & x1z1*5760.+c(2,5,4,n)*x1y1*5760.+c(2,5,5,n)*x1y1z1*28800.+c(3,
     & 4,4,n)*x2*1728.+c(3,4,5,n)*x2z1*8640.+c(3,5,4,n)*x2y1*8640.+c(
     & 3,5,5,n)*x2y1z1*43200.+c(4,4,4,n)*x3*2304.+c(4,4,5,n)*x3z1*
     & 11520.+c(4,5,4,n)*x3y1*11520.+c(4,5,5,n)*x3y1z1*57600.+c(5,4,4,
     & n)*x4*2880.+c(5,4,5,n)*x4z1*14400.+c(5,5,4,n)*x4y1*14400.+c(5,
     & 5,5,n)*x4y1z1*72000.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.2.and.dy.eq.0.and.dz.eq.0 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y1z5=y1z4*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
      y2z5=y1z5*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      y3z4=y2z4*y1
      y3z5=y2z5*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y4z2=y3z2*y1
      y4z3=y3z3*y1
      y4z4=y3z4*y1
      y4z5=y3z5*y1
      y5=y4*y1
      y5z1=y4z1*y1
      y5z2=y4z2*y1
      y5z3=y4z3*y1
      y5z4=y4z4*y1
      y5z5=y4z5*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1z5=x1z4*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y1z5=x1y1z4*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y2z4=x1y1z4*y1
      x1y2z5=x1y1z5*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x1y3z4=x1y2z4*y1
      x1y3z5=x1y2z5*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y4z2=x1y3z2*y1
      x1y4z3=x1y3z3*y1
      x1y4z4=x1y3z4*y1
      x1y4z5=x1y3z5*y1
      x1y5=x1y4*y1
      x1y5z1=x1y4z1*y1
      x1y5z2=x1y4z2*y1
      x1y5z3=x1y4z3*y1
      x1y5z4=x1y4z4*y1
      x1y5z5=x1y4z5*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x2z5=x1z5*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y1z4=x1y1z4*x1
      x2y1z5=x1y1z5*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y2z4=x1y2z4*x1
      x2y2z5=x1y2z5*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y3z3=x1y3z3*x1
      x2y3z4=x1y3z4*x1
      x2y3z5=x1y3z5*x1
      x2y4=x1y4*x1
      x2y4z1=x1y4z1*x1
      x2y4z2=x1y4z2*x1
      x2y4z3=x1y4z3*x1
      x2y4z4=x1y4z4*x1
      x2y4z5=x1y4z5*x1
      x2y5=x1y5*x1
      x2y5z1=x1y5z1*x1
      x2y5z2=x1y5z2*x1
      x2y5z3=x1y5z3*x1
      x2y5z4=x1y5z4*x1
      x2y5z5=x1y5z5*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3z4=x2z4*x1
      x3z5=x2z5*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x3y1z4=x2y1z4*x1
      x3y1z5=x2y1z5*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y2z3=x2y2z3*x1
      x3y2z4=x2y2z4*x1
      x3y2z5=x2y2z5*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x3y3z2=x2y3z2*x1
      x3y3z3=x2y3z3*x1
      x3y3z4=x2y3z4*x1
      x3y3z5=x2y3z5*x1
      x3y4=x2y4*x1
      x3y4z1=x2y4z1*x1
      x3y4z2=x2y4z2*x1
      x3y4z3=x2y4z3*x1
      x3y4z4=x2y4z4*x1
      x3y4z5=x2y4z5*x1
      x3y5=x2y5*x1
      x3y5z1=x2y5z1*x1
      x3y5z2=x2y5z2*x1
      x3y5z3=x2y5z3*x1
      x3y5z4=x2y5z4*x1
      x3y5z5=x2y5z5*x1
      r(i1,i2,i3,n)=(c(2,0,0,n)*2.+c(2,0,1,n)*z1*2.+c(2,0,2,n)*z2*2.+c(
     & 2,0,3,n)*z3*2.+c(2,0,4,n)*z4*2.+c(2,0,5,n)*z5*2.+c(2,1,0,n)*y1*
     & 2.+c(2,1,1,n)*y1z1*2.+c(2,1,2,n)*y1z2*2.+c(2,1,3,n)*y1z3*2.+c(
     & 2,1,4,n)*y1z4*2.+c(2,1,5,n)*y1z5*2.+c(2,2,0,n)*y2*2.+c(2,2,1,n)
     & *y2z1*2.+c(2,2,2,n)*y2z2*2.+c(2,2,3,n)*y2z3*2.+c(2,2,4,n)*y2z4*
     & 2.+c(2,2,5,n)*y2z5*2.+c(2,3,0,n)*y3*2.+c(2,3,1,n)*y3z1*2.+c(2,
     & 3,2,n)*y3z2*2.+c(2,3,3,n)*y3z3*2.+c(2,3,4,n)*y3z4*2.+c(2,3,5,n)
     & *y3z5*2.+c(2,4,0,n)*y4*2.+c(2,4,1,n)*y4z1*2.+c(2,4,2,n)*y4z2*
     & 2.+c(2,4,3,n)*y4z3*2.+c(2,4,4,n)*y4z4*2.+c(2,4,5,n)*y4z5*2.+c(
     & 2,5,0,n)*y5*2.+c(2,5,1,n)*y5z1*2.+c(2,5,2,n)*y5z2*2.+c(2,5,3,n)
     & *y5z3*2.+c(2,5,4,n)*y5z4*2.+c(2,5,5,n)*y5z5*2.+c(3,0,0,n)*x1*
     & 6.+c(3,0,1,n)*x1z1*6.+c(3,0,2,n)*x1z2*6.+c(3,0,3,n)*x1z3*6.+c(
     & 3,0,4,n)*x1z4*6.+c(3,0,5,n)*x1z5*6.+c(3,1,0,n)*x1y1*6.+c(3,1,1,
     & n)*x1y1z1*6.+c(3,1,2,n)*x1y1z2*6.+c(3,1,3,n)*x1y1z3*6.+c(3,1,4,
     & n)*x1y1z4*6.+c(3,1,5,n)*x1y1z5*6.+c(3,2,0,n)*x1y2*6.+c(3,2,1,n)
     & *x1y2z1*6.+c(3,2,2,n)*x1y2z2*6.+c(3,2,3,n)*x1y2z3*6.+c(3,2,4,n)
     & *x1y2z4*6.+c(3,2,5,n)*x1y2z5*6.+c(3,3,0,n)*x1y3*6.+c(3,3,1,n)*
     & x1y3z1*6.+c(3,3,2,n)*x1y3z2*6.+c(3,3,3,n)*x1y3z3*6.+c(3,3,4,n)*
     & x1y3z4*6.+c(3,3,5,n)*x1y3z5*6.+c(3,4,0,n)*x1y4*6.+c(3,4,1,n)*
     & x1y4z1*6.+c(3,4,2,n)*x1y4z2*6.+c(3,4,3,n)*x1y4z3*6.+c(3,4,4,n)*
     & x1y4z4*6.+c(3,4,5,n)*x1y4z5*6.+c(3,5,0,n)*x1y5*6.+c(3,5,1,n)*
     & x1y5z1*6.+c(3,5,2,n)*x1y5z2*6.+c(3,5,3,n)*x1y5z3*6.+c(3,5,4,n)*
     & x1y5z4*6.+c(3,5,5,n)*x1y5z5*6.+c(4,0,0,n)*x2*12.+c(4,0,1,n)*
     & x2z1*12.+c(4,0,2,n)*x2z2*12.+c(4,0,3,n)*x2z3*12.+c(4,0,4,n)*
     & x2z4*12.+c(4,0,5,n)*x2z5*12.+c(4,1,0,n)*x2y1*12.+c(4,1,1,n)*
     & x2y1z1*12.+c(4,1,2,n)*x2y1z2*12.+c(4,1,3,n)*x2y1z3*12.+c(4,1,4,
     & n)*x2y1z4*12.+c(4,1,5,n)*x2y1z5*12.+c(4,2,0,n)*x2y2*12.+c(4,2,
     & 1,n)*x2y2z1*12.+c(4,2,2,n)*x2y2z2*12.+c(4,2,3,n)*x2y2z3*12.+c(
     & 4,2,4,n)*x2y2z4*12.+c(4,2,5,n)*x2y2z5*12.+c(4,3,0,n)*x2y3*12.+
     & c(4,3,1,n)*x2y3z1*12.+c(4,3,2,n)*x2y3z2*12.+c(4,3,3,n)*x2y3z3*
     & 12.+c(4,3,4,n)*x2y3z4*12.+c(4,3,5,n)*x2y3z5*12.+c(4,4,0,n)*
     & x2y4*12.+c(4,4,1,n)*x2y4z1*12.+c(4,4,2,n)*x2y4z2*12.+c(4,4,3,n)
     & *x2y4z3*12.+c(4,4,4,n)*x2y4z4*12.+c(4,4,5,n)*x2y4z5*12.+c(4,5,
     & 0,n)*x2y5*12.+c(4,5,1,n)*x2y5z1*12.+c(4,5,2,n)*x2y5z2*12.+c(4,
     & 5,3,n)*x2y5z3*12.+c(4,5,4,n)*x2y5z4*12.+c(4,5,5,n)*x2y5z5*12.+
     & c(5,0,0,n)*x3*20.+c(5,0,1,n)*x3z1*20.+c(5,0,2,n)*x3z2*20.+c(5,
     & 0,3,n)*x3z3*20.+c(5,0,4,n)*x3z4*20.+c(5,0,5,n)*x3z5*20.+c(5,1,
     & 0,n)*x3y1*20.+c(5,1,1,n)*x3y1z1*20.+c(5,1,2,n)*x3y1z2*20.+c(5,
     & 1,3,n)*x3y1z3*20.+c(5,1,4,n)*x3y1z4*20.+c(5,1,5,n)*x3y1z5*20.+
     & c(5,2,0,n)*x3y2*20.+c(5,2,1,n)*x3y2z1*20.+c(5,2,2,n)*x3y2z2*
     & 20.+c(5,2,3,n)*x3y2z3*20.+c(5,2,4,n)*x3y2z4*20.+c(5,2,5,n)*
     & x3y2z5*20.+c(5,3,0,n)*x3y3*20.+c(5,3,1,n)*x3y3z1*20.+c(5,3,2,n)
     & *x3y3z2*20.+c(5,3,3,n)*x3y3z3*20.+c(5,3,4,n)*x3y3z4*20.+c(5,3,
     & 5,n)*x3y3z5*20.+c(5,4,0,n)*x3y4*20.+c(5,4,1,n)*x3y4z1*20.+c(5,
     & 4,2,n)*x3y4z2*20.+c(5,4,3,n)*x3y4z3*20.+c(5,4,4,n)*x3y4z4*20.+
     & c(5,4,5,n)*x3y4z5*20.+c(5,5,0,n)*x3y5*20.+c(5,5,1,n)*x3y5z1*
     & 20.+c(5,5,2,n)*x3y5z2*20.+c(5,5,3,n)*x3y5z3*20.+c(5,5,4,n)*
     & x3y5z4*20.+c(5,5,5,n)*x3y5z5*20.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.2.and.dy.eq.0.and.dz.eq.1 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      y3z4=y2z4*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y4z2=y3z2*y1
      y4z3=y3z3*y1
      y4z4=y3z4*y1
      y5=y4*y1
      y5z1=y4z1*y1
      y5z2=y4z2*y1
      y5z3=y4z3*y1
      y5z4=y4z4*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y2z4=x1y1z4*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x1y3z4=x1y2z4*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y4z2=x1y3z2*y1
      x1y4z3=x1y3z3*y1
      x1y4z4=x1y3z4*y1
      x1y5=x1y4*y1
      x1y5z1=x1y4z1*y1
      x1y5z2=x1y4z2*y1
      x1y5z3=x1y4z3*y1
      x1y5z4=x1y4z4*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y1z4=x1y1z4*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y2z4=x1y2z4*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y3z3=x1y3z3*x1
      x2y3z4=x1y3z4*x1
      x2y4=x1y4*x1
      x2y4z1=x1y4z1*x1
      x2y4z2=x1y4z2*x1
      x2y4z3=x1y4z3*x1
      x2y4z4=x1y4z4*x1
      x2y5=x1y5*x1
      x2y5z1=x1y5z1*x1
      x2y5z2=x1y5z2*x1
      x2y5z3=x1y5z3*x1
      x2y5z4=x1y5z4*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3z4=x2z4*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x3y1z4=x2y1z4*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y2z3=x2y2z3*x1
      x3y2z4=x2y2z4*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x3y3z2=x2y3z2*x1
      x3y3z3=x2y3z3*x1
      x3y3z4=x2y3z4*x1
      x3y4=x2y4*x1
      x3y4z1=x2y4z1*x1
      x3y4z2=x2y4z2*x1
      x3y4z3=x2y4z3*x1
      x3y4z4=x2y4z4*x1
      x3y5=x2y5*x1
      x3y5z1=x2y5z1*x1
      x3y5z2=x2y5z2*x1
      x3y5z3=x2y5z3*x1
      x3y5z4=x2y5z4*x1
      r(i1,i2,i3,n)=(c(2,0,1,n)*2.+c(2,0,2,n)*z1*4.+c(2,0,3,n)*z2*6.+c(
     & 2,0,4,n)*z3*8.+c(2,0,5,n)*z4*10.+c(2,1,1,n)*y1*2.+c(2,1,2,n)*
     & y1z1*4.+c(2,1,3,n)*y1z2*6.+c(2,1,4,n)*y1z3*8.+c(2,1,5,n)*y1z4*
     & 10.+c(2,2,1,n)*y2*2.+c(2,2,2,n)*y2z1*4.+c(2,2,3,n)*y2z2*6.+c(2,
     & 2,4,n)*y2z3*8.+c(2,2,5,n)*y2z4*10.+c(2,3,1,n)*y3*2.+c(2,3,2,n)*
     & y3z1*4.+c(2,3,3,n)*y3z2*6.+c(2,3,4,n)*y3z3*8.+c(2,3,5,n)*y3z4*
     & 10.+c(2,4,1,n)*y4*2.+c(2,4,2,n)*y4z1*4.+c(2,4,3,n)*y4z2*6.+c(2,
     & 4,4,n)*y4z3*8.+c(2,4,5,n)*y4z4*10.+c(2,5,1,n)*y5*2.+c(2,5,2,n)*
     & y5z1*4.+c(2,5,3,n)*y5z2*6.+c(2,5,4,n)*y5z3*8.+c(2,5,5,n)*y5z4*
     & 10.+c(3,0,1,n)*x1*6.+c(3,0,2,n)*x1z1*12.+c(3,0,3,n)*x1z2*18.+c(
     & 3,0,4,n)*x1z3*24.+c(3,0,5,n)*x1z4*30.+c(3,1,1,n)*x1y1*6.+c(3,1,
     & 2,n)*x1y1z1*12.+c(3,1,3,n)*x1y1z2*18.+c(3,1,4,n)*x1y1z3*24.+c(
     & 3,1,5,n)*x1y1z4*30.+c(3,2,1,n)*x1y2*6.+c(3,2,2,n)*x1y2z1*12.+c(
     & 3,2,3,n)*x1y2z2*18.+c(3,2,4,n)*x1y2z3*24.+c(3,2,5,n)*x1y2z4*
     & 30.+c(3,3,1,n)*x1y3*6.+c(3,3,2,n)*x1y3z1*12.+c(3,3,3,n)*x1y3z2*
     & 18.+c(3,3,4,n)*x1y3z3*24.+c(3,3,5,n)*x1y3z4*30.+c(3,4,1,n)*
     & x1y4*6.+c(3,4,2,n)*x1y4z1*12.+c(3,4,3,n)*x1y4z2*18.+c(3,4,4,n)*
     & x1y4z3*24.+c(3,4,5,n)*x1y4z4*30.+c(3,5,1,n)*x1y5*6.+c(3,5,2,n)*
     & x1y5z1*12.+c(3,5,3,n)*x1y5z2*18.+c(3,5,4,n)*x1y5z3*24.+c(3,5,5,
     & n)*x1y5z4*30.+c(4,0,1,n)*x2*12.+c(4,0,2,n)*x2z1*24.+c(4,0,3,n)*
     & x2z2*36.+c(4,0,4,n)*x2z3*48.+c(4,0,5,n)*x2z4*60.+c(4,1,1,n)*
     & x2y1*12.+c(4,1,2,n)*x2y1z1*24.+c(4,1,3,n)*x2y1z2*36.+c(4,1,4,n)
     & *x2y1z3*48.+c(4,1,5,n)*x2y1z4*60.+c(4,2,1,n)*x2y2*12.+c(4,2,2,
     & n)*x2y2z1*24.+c(4,2,3,n)*x2y2z2*36.+c(4,2,4,n)*x2y2z3*48.+c(4,
     & 2,5,n)*x2y2z4*60.+c(4,3,1,n)*x2y3*12.+c(4,3,2,n)*x2y3z1*24.+c(
     & 4,3,3,n)*x2y3z2*36.+c(4,3,4,n)*x2y3z3*48.+c(4,3,5,n)*x2y3z4*
     & 60.+c(4,4,1,n)*x2y4*12.+c(4,4,2,n)*x2y4z1*24.+c(4,4,3,n)*
     & x2y4z2*36.+c(4,4,4,n)*x2y4z3*48.+c(4,4,5,n)*x2y4z4*60.+c(4,5,1,
     & n)*x2y5*12.+c(4,5,2,n)*x2y5z1*24.+c(4,5,3,n)*x2y5z2*36.+c(4,5,
     & 4,n)*x2y5z3*48.+c(4,5,5,n)*x2y5z4*60.+c(5,0,1,n)*x3*20.+c(5,0,
     & 2,n)*x3z1*40.+c(5,0,3,n)*x3z2*60.+c(5,0,4,n)*x3z3*80.+c(5,0,5,
     & n)*x3z4*100.+c(5,1,1,n)*x3y1*20.+c(5,1,2,n)*x3y1z1*40.+c(5,1,3,
     & n)*x3y1z2*60.+c(5,1,4,n)*x3y1z3*80.+c(5,1,5,n)*x3y1z4*100.+c(5,
     & 2,1,n)*x3y2*20.+c(5,2,2,n)*x3y2z1*40.+c(5,2,3,n)*x3y2z2*60.+c(
     & 5,2,4,n)*x3y2z3*80.+c(5,2,5,n)*x3y2z4*100.+c(5,3,1,n)*x3y3*20.+
     & c(5,3,2,n)*x3y3z1*40.+c(5,3,3,n)*x3y3z2*60.+c(5,3,4,n)*x3y3z3*
     & 80.+c(5,3,5,n)*x3y3z4*100.+c(5,4,1,n)*x3y4*20.+c(5,4,2,n)*
     & x3y4z1*40.+c(5,4,3,n)*x3y4z2*60.+c(5,4,4,n)*x3y4z3*80.+c(5,4,5,
     & n)*x3y4z4*100.+c(5,5,1,n)*x3y5*20.+c(5,5,2,n)*x3y5z1*40.+c(5,5,
     & 3,n)*x3y5z2*60.+c(5,5,4,n)*x3y5z3*80.+c(5,5,5,n)*x3y5z4*100.)*
     & time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.2.and.dy.eq.0.and.dz.eq.2 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y4z2=y3z2*y1
      y4z3=y3z3*y1
      y5=y4*y1
      y5z1=y4z1*y1
      y5z2=y4z2*y1
      y5z3=y4z3*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y4z2=x1y3z2*y1
      x1y4z3=x1y3z3*y1
      x1y5=x1y4*y1
      x1y5z1=x1y4z1*y1
      x1y5z2=x1y4z2*y1
      x1y5z3=x1y4z3*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y3z3=x1y3z3*x1
      x2y4=x1y4*x1
      x2y4z1=x1y4z1*x1
      x2y4z2=x1y4z2*x1
      x2y4z3=x1y4z3*x1
      x2y5=x1y5*x1
      x2y5z1=x1y5z1*x1
      x2y5z2=x1y5z2*x1
      x2y5z3=x1y5z3*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y2z3=x2y2z3*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x3y3z2=x2y3z2*x1
      x3y3z3=x2y3z3*x1
      x3y4=x2y4*x1
      x3y4z1=x2y4z1*x1
      x3y4z2=x2y4z2*x1
      x3y4z3=x2y4z3*x1
      x3y5=x2y5*x1
      x3y5z1=x2y5z1*x1
      x3y5z2=x2y5z2*x1
      x3y5z3=x2y5z3*x1
      r(i1,i2,i3,n)=(c(2,0,2,n)*4.+c(2,0,3,n)*z1*12.+c(2,0,4,n)*z2*24.+
     & c(2,0,5,n)*z3*40.+c(2,1,2,n)*y1*4.+c(2,1,3,n)*y1z1*12.+c(2,1,4,
     & n)*y1z2*24.+c(2,1,5,n)*y1z3*40.+c(2,2,2,n)*y2*4.+c(2,2,3,n)*
     & y2z1*12.+c(2,2,4,n)*y2z2*24.+c(2,2,5,n)*y2z3*40.+c(2,3,2,n)*y3*
     & 4.+c(2,3,3,n)*y3z1*12.+c(2,3,4,n)*y3z2*24.+c(2,3,5,n)*y3z3*40.+
     & c(2,4,2,n)*y4*4.+c(2,4,3,n)*y4z1*12.+c(2,4,4,n)*y4z2*24.+c(2,4,
     & 5,n)*y4z3*40.+c(2,5,2,n)*y5*4.+c(2,5,3,n)*y5z1*12.+c(2,5,4,n)*
     & y5z2*24.+c(2,5,5,n)*y5z3*40.+c(3,0,2,n)*x1*12.+c(3,0,3,n)*x1z1*
     & 36.+c(3,0,4,n)*x1z2*72.+c(3,0,5,n)*x1z3*120.+c(3,1,2,n)*x1y1*
     & 12.+c(3,1,3,n)*x1y1z1*36.+c(3,1,4,n)*x1y1z2*72.+c(3,1,5,n)*
     & x1y1z3*120.+c(3,2,2,n)*x1y2*12.+c(3,2,3,n)*x1y2z1*36.+c(3,2,4,
     & n)*x1y2z2*72.+c(3,2,5,n)*x1y2z3*120.+c(3,3,2,n)*x1y3*12.+c(3,3,
     & 3,n)*x1y3z1*36.+c(3,3,4,n)*x1y3z2*72.+c(3,3,5,n)*x1y3z3*120.+c(
     & 3,4,2,n)*x1y4*12.+c(3,4,3,n)*x1y4z1*36.+c(3,4,4,n)*x1y4z2*72.+
     & c(3,4,5,n)*x1y4z3*120.+c(3,5,2,n)*x1y5*12.+c(3,5,3,n)*x1y5z1*
     & 36.+c(3,5,4,n)*x1y5z2*72.+c(3,5,5,n)*x1y5z3*120.+c(4,0,2,n)*x2*
     & 24.+c(4,0,3,n)*x2z1*72.+c(4,0,4,n)*x2z2*144.+c(4,0,5,n)*x2z3*
     & 240.+c(4,1,2,n)*x2y1*24.+c(4,1,3,n)*x2y1z1*72.+c(4,1,4,n)*
     & x2y1z2*144.+c(4,1,5,n)*x2y1z3*240.+c(4,2,2,n)*x2y2*24.+c(4,2,3,
     & n)*x2y2z1*72.+c(4,2,4,n)*x2y2z2*144.+c(4,2,5,n)*x2y2z3*240.+c(
     & 4,3,2,n)*x2y3*24.+c(4,3,3,n)*x2y3z1*72.+c(4,3,4,n)*x2y3z2*144.+
     & c(4,3,5,n)*x2y3z3*240.+c(4,4,2,n)*x2y4*24.+c(4,4,3,n)*x2y4z1*
     & 72.+c(4,4,4,n)*x2y4z2*144.+c(4,4,5,n)*x2y4z3*240.+c(4,5,2,n)*
     & x2y5*24.+c(4,5,3,n)*x2y5z1*72.+c(4,5,4,n)*x2y5z2*144.+c(4,5,5,
     & n)*x2y5z3*240.+c(5,0,2,n)*x3*40.+c(5,0,3,n)*x3z1*120.+c(5,0,4,
     & n)*x3z2*240.+c(5,0,5,n)*x3z3*400.+c(5,1,2,n)*x3y1*40.+c(5,1,3,
     & n)*x3y1z1*120.+c(5,1,4,n)*x3y1z2*240.+c(5,1,5,n)*x3y1z3*400.+c(
     & 5,2,2,n)*x3y2*40.+c(5,2,3,n)*x3y2z1*120.+c(5,2,4,n)*x3y2z2*
     & 240.+c(5,2,5,n)*x3y2z3*400.+c(5,3,2,n)*x3y3*40.+c(5,3,3,n)*
     & x3y3z1*120.+c(5,3,4,n)*x3y3z2*240.+c(5,3,5,n)*x3y3z3*400.+c(5,
     & 4,2,n)*x3y4*40.+c(5,4,3,n)*x3y4z1*120.+c(5,4,4,n)*x3y4z2*240.+
     & c(5,4,5,n)*x3y4z3*400.+c(5,5,2,n)*x3y5*40.+c(5,5,3,n)*x3y5z1*
     & 120.+c(5,5,4,n)*x3y5z2*240.+c(5,5,5,n)*x3y5z3*400.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.2.and.dy.eq.0.and.dz.eq.3 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y4z2=y3z2*y1
      y5=y4*y1
      y5z1=y4z1*y1
      y5z2=y4z2*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y4z2=x1y3z2*y1
      x1y5=x1y4*y1
      x1y5z1=x1y4z1*y1
      x1y5z2=x1y4z2*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y4=x1y4*x1
      x2y4z1=x1y4z1*x1
      x2y4z2=x1y4z2*x1
      x2y5=x1y5*x1
      x2y5z1=x1y5z1*x1
      x2y5z2=x1y5z2*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x3y3z2=x2y3z2*x1
      x3y4=x2y4*x1
      x3y4z1=x2y4z1*x1
      x3y4z2=x2y4z2*x1
      x3y5=x2y5*x1
      x3y5z1=x2y5z1*x1
      x3y5z2=x2y5z2*x1
      r(i1,i2,i3,n)=(c(2,0,3,n)*12.+c(2,0,4,n)*z1*48.+c(2,0,5,n)*z2*
     & 120.+c(2,1,3,n)*y1*12.+c(2,1,4,n)*y1z1*48.+c(2,1,5,n)*y1z2*
     & 120.+c(2,2,3,n)*y2*12.+c(2,2,4,n)*y2z1*48.+c(2,2,5,n)*y2z2*
     & 120.+c(2,3,3,n)*y3*12.+c(2,3,4,n)*y3z1*48.+c(2,3,5,n)*y3z2*
     & 120.+c(2,4,3,n)*y4*12.+c(2,4,4,n)*y4z1*48.+c(2,4,5,n)*y4z2*
     & 120.+c(2,5,3,n)*y5*12.+c(2,5,4,n)*y5z1*48.+c(2,5,5,n)*y5z2*
     & 120.+c(3,0,3,n)*x1*36.+c(3,0,4,n)*x1z1*144.+c(3,0,5,n)*x1z2*
     & 360.+c(3,1,3,n)*x1y1*36.+c(3,1,4,n)*x1y1z1*144.+c(3,1,5,n)*
     & x1y1z2*360.+c(3,2,3,n)*x1y2*36.+c(3,2,4,n)*x1y2z1*144.+c(3,2,5,
     & n)*x1y2z2*360.+c(3,3,3,n)*x1y3*36.+c(3,3,4,n)*x1y3z1*144.+c(3,
     & 3,5,n)*x1y3z2*360.+c(3,4,3,n)*x1y4*36.+c(3,4,4,n)*x1y4z1*144.+
     & c(3,4,5,n)*x1y4z2*360.+c(3,5,3,n)*x1y5*36.+c(3,5,4,n)*x1y5z1*
     & 144.+c(3,5,5,n)*x1y5z2*360.+c(4,0,3,n)*x2*72.+c(4,0,4,n)*x2z1*
     & 288.+c(4,0,5,n)*x2z2*720.+c(4,1,3,n)*x2y1*72.+c(4,1,4,n)*
     & x2y1z1*288.+c(4,1,5,n)*x2y1z2*720.+c(4,2,3,n)*x2y2*72.+c(4,2,4,
     & n)*x2y2z1*288.+c(4,2,5,n)*x2y2z2*720.+c(4,3,3,n)*x2y3*72.+c(4,
     & 3,4,n)*x2y3z1*288.+c(4,3,5,n)*x2y3z2*720.+c(4,4,3,n)*x2y4*72.+
     & c(4,4,4,n)*x2y4z1*288.+c(4,4,5,n)*x2y4z2*720.+c(4,5,3,n)*x2y5*
     & 72.+c(4,5,4,n)*x2y5z1*288.+c(4,5,5,n)*x2y5z2*720.+c(5,0,3,n)*
     & x3*120.+c(5,0,4,n)*x3z1*480.+c(5,0,5,n)*x3z2*1200.+c(5,1,3,n)*
     & x3y1*120.+c(5,1,4,n)*x3y1z1*480.+c(5,1,5,n)*x3y1z2*1200.+c(5,2,
     & 3,n)*x3y2*120.+c(5,2,4,n)*x3y2z1*480.+c(5,2,5,n)*x3y2z2*1200.+
     & c(5,3,3,n)*x3y3*120.+c(5,3,4,n)*x3y3z1*480.+c(5,3,5,n)*x3y3z2*
     & 1200.+c(5,4,3,n)*x3y4*120.+c(5,4,4,n)*x3y4z1*480.+c(5,4,5,n)*
     & x3y4z2*1200.+c(5,5,3,n)*x3y5*120.+c(5,5,4,n)*x3y5z1*480.+c(5,5,
     & 5,n)*x3y5z2*1200.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.2.and.dy.eq.0.and.dz.eq.4 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y5=y4*y1
      y5z1=y4z1*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y5=x1y4*y1
      x1y5z1=x1y4z1*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y4=x1y4*x1
      x2y4z1=x1y4z1*x1
      x2y5=x1y5*x1
      x2y5z1=x1y5z1*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x3y4=x2y4*x1
      x3y4z1=x2y4z1*x1
      x3y5=x2y5*x1
      x3y5z1=x2y5z1*x1
      r(i1,i2,i3,n)=(c(2,0,4,n)*48.+c(2,0,5,n)*z1*240.+c(2,1,4,n)*y1*
     & 48.+c(2,1,5,n)*y1z1*240.+c(2,2,4,n)*y2*48.+c(2,2,5,n)*y2z1*
     & 240.+c(2,3,4,n)*y3*48.+c(2,3,5,n)*y3z1*240.+c(2,4,4,n)*y4*48.+
     & c(2,4,5,n)*y4z1*240.+c(2,5,4,n)*y5*48.+c(2,5,5,n)*y5z1*240.+c(
     & 3,0,4,n)*x1*144.+c(3,0,5,n)*x1z1*720.+c(3,1,4,n)*x1y1*144.+c(3,
     & 1,5,n)*x1y1z1*720.+c(3,2,4,n)*x1y2*144.+c(3,2,5,n)*x1y2z1*720.+
     & c(3,3,4,n)*x1y3*144.+c(3,3,5,n)*x1y3z1*720.+c(3,4,4,n)*x1y4*
     & 144.+c(3,4,5,n)*x1y4z1*720.+c(3,5,4,n)*x1y5*144.+c(3,5,5,n)*
     & x1y5z1*720.+c(4,0,4,n)*x2*288.+c(4,0,5,n)*x2z1*1440.+c(4,1,4,n)
     & *x2y1*288.+c(4,1,5,n)*x2y1z1*1440.+c(4,2,4,n)*x2y2*288.+c(4,2,
     & 5,n)*x2y2z1*1440.+c(4,3,4,n)*x2y3*288.+c(4,3,5,n)*x2y3z1*1440.+
     & c(4,4,4,n)*x2y4*288.+c(4,4,5,n)*x2y4z1*1440.+c(4,5,4,n)*x2y5*
     & 288.+c(4,5,5,n)*x2y5z1*1440.+c(5,0,4,n)*x3*480.+c(5,0,5,n)*
     & x3z1*2400.+c(5,1,4,n)*x3y1*480.+c(5,1,5,n)*x3y1z1*2400.+c(5,2,
     & 4,n)*x3y2*480.+c(5,2,5,n)*x3y2z1*2400.+c(5,3,4,n)*x3y3*480.+c(
     & 5,3,5,n)*x3y3z1*2400.+c(5,4,4,n)*x3y4*480.+c(5,4,5,n)*x3y4z1*
     & 2400.+c(5,5,4,n)*x3y5*480.+c(5,5,5,n)*x3y5z1*2400.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.2.and.dy.eq.1.and.dz.eq.0 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y1z5=y1z4*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
      y2z5=y1z5*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      y3z4=y2z4*y1
      y3z5=y2z5*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y4z2=y3z2*y1
      y4z3=y3z3*y1
      y4z4=y3z4*y1
      y4z5=y3z5*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1z5=x1z4*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y1z5=x1y1z4*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y2z4=x1y1z4*y1
      x1y2z5=x1y1z5*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x1y3z4=x1y2z4*y1
      x1y3z5=x1y2z5*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y4z2=x1y3z2*y1
      x1y4z3=x1y3z3*y1
      x1y4z4=x1y3z4*y1
      x1y4z5=x1y3z5*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x2z5=x1z5*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y1z4=x1y1z4*x1
      x2y1z5=x1y1z5*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y2z4=x1y2z4*x1
      x2y2z5=x1y2z5*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y3z3=x1y3z3*x1
      x2y3z4=x1y3z4*x1
      x2y3z5=x1y3z5*x1
      x2y4=x1y4*x1
      x2y4z1=x1y4z1*x1
      x2y4z2=x1y4z2*x1
      x2y4z3=x1y4z3*x1
      x2y4z4=x1y4z4*x1
      x2y4z5=x1y4z5*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3z4=x2z4*x1
      x3z5=x2z5*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x3y1z4=x2y1z4*x1
      x3y1z5=x2y1z5*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y2z3=x2y2z3*x1
      x3y2z4=x2y2z4*x1
      x3y2z5=x2y2z5*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x3y3z2=x2y3z2*x1
      x3y3z3=x2y3z3*x1
      x3y3z4=x2y3z4*x1
      x3y3z5=x2y3z5*x1
      x3y4=x2y4*x1
      x3y4z1=x2y4z1*x1
      x3y4z2=x2y4z2*x1
      x3y4z3=x2y4z3*x1
      x3y4z4=x2y4z4*x1
      x3y4z5=x2y4z5*x1
      r(i1,i2,i3,n)=(c(2,1,0,n)*2.+c(2,1,1,n)*z1*2.+c(2,1,2,n)*z2*2.+c(
     & 2,1,3,n)*z3*2.+c(2,1,4,n)*z4*2.+c(2,1,5,n)*z5*2.+c(2,2,0,n)*y1*
     & 4.+c(2,2,1,n)*y1z1*4.+c(2,2,2,n)*y1z2*4.+c(2,2,3,n)*y1z3*4.+c(
     & 2,2,4,n)*y1z4*4.+c(2,2,5,n)*y1z5*4.+c(2,3,0,n)*y2*6.+c(2,3,1,n)
     & *y2z1*6.+c(2,3,2,n)*y2z2*6.+c(2,3,3,n)*y2z3*6.+c(2,3,4,n)*y2z4*
     & 6.+c(2,3,5,n)*y2z5*6.+c(2,4,0,n)*y3*8.+c(2,4,1,n)*y3z1*8.+c(2,
     & 4,2,n)*y3z2*8.+c(2,4,3,n)*y3z3*8.+c(2,4,4,n)*y3z4*8.+c(2,4,5,n)
     & *y3z5*8.+c(2,5,0,n)*y4*10.+c(2,5,1,n)*y4z1*10.+c(2,5,2,n)*y4z2*
     & 10.+c(2,5,3,n)*y4z3*10.+c(2,5,4,n)*y4z4*10.+c(2,5,5,n)*y4z5*
     & 10.+c(3,1,0,n)*x1*6.+c(3,1,1,n)*x1z1*6.+c(3,1,2,n)*x1z2*6.+c(3,
     & 1,3,n)*x1z3*6.+c(3,1,4,n)*x1z4*6.+c(3,1,5,n)*x1z5*6.+c(3,2,0,n)
     & *x1y1*12.+c(3,2,1,n)*x1y1z1*12.+c(3,2,2,n)*x1y1z2*12.+c(3,2,3,
     & n)*x1y1z3*12.+c(3,2,4,n)*x1y1z4*12.+c(3,2,5,n)*x1y1z5*12.+c(3,
     & 3,0,n)*x1y2*18.+c(3,3,1,n)*x1y2z1*18.+c(3,3,2,n)*x1y2z2*18.+c(
     & 3,3,3,n)*x1y2z3*18.+c(3,3,4,n)*x1y2z4*18.+c(3,3,5,n)*x1y2z5*
     & 18.+c(3,4,0,n)*x1y3*24.+c(3,4,1,n)*x1y3z1*24.+c(3,4,2,n)*
     & x1y3z2*24.+c(3,4,3,n)*x1y3z3*24.+c(3,4,4,n)*x1y3z4*24.+c(3,4,5,
     & n)*x1y3z5*24.+c(3,5,0,n)*x1y4*30.+c(3,5,1,n)*x1y4z1*30.+c(3,5,
     & 2,n)*x1y4z2*30.+c(3,5,3,n)*x1y4z3*30.+c(3,5,4,n)*x1y4z4*30.+c(
     & 3,5,5,n)*x1y4z5*30.+c(4,1,0,n)*x2*12.+c(4,1,1,n)*x2z1*12.+c(4,
     & 1,2,n)*x2z2*12.+c(4,1,3,n)*x2z3*12.+c(4,1,4,n)*x2z4*12.+c(4,1,
     & 5,n)*x2z5*12.+c(4,2,0,n)*x2y1*24.+c(4,2,1,n)*x2y1z1*24.+c(4,2,
     & 2,n)*x2y1z2*24.+c(4,2,3,n)*x2y1z3*24.+c(4,2,4,n)*x2y1z4*24.+c(
     & 4,2,5,n)*x2y1z5*24.+c(4,3,0,n)*x2y2*36.+c(4,3,1,n)*x2y2z1*36.+
     & c(4,3,2,n)*x2y2z2*36.+c(4,3,3,n)*x2y2z3*36.+c(4,3,4,n)*x2y2z4*
     & 36.+c(4,3,5,n)*x2y2z5*36.+c(4,4,0,n)*x2y3*48.+c(4,4,1,n)*
     & x2y3z1*48.+c(4,4,2,n)*x2y3z2*48.+c(4,4,3,n)*x2y3z3*48.+c(4,4,4,
     & n)*x2y3z4*48.+c(4,4,5,n)*x2y3z5*48.+c(4,5,0,n)*x2y4*60.+c(4,5,
     & 1,n)*x2y4z1*60.+c(4,5,2,n)*x2y4z2*60.+c(4,5,3,n)*x2y4z3*60.+c(
     & 4,5,4,n)*x2y4z4*60.+c(4,5,5,n)*x2y4z5*60.+c(5,1,0,n)*x3*20.+c(
     & 5,1,1,n)*x3z1*20.+c(5,1,2,n)*x3z2*20.+c(5,1,3,n)*x3z3*20.+c(5,
     & 1,4,n)*x3z4*20.+c(5,1,5,n)*x3z5*20.+c(5,2,0,n)*x3y1*40.+c(5,2,
     & 1,n)*x3y1z1*40.+c(5,2,2,n)*x3y1z2*40.+c(5,2,3,n)*x3y1z3*40.+c(
     & 5,2,4,n)*x3y1z4*40.+c(5,2,5,n)*x3y1z5*40.+c(5,3,0,n)*x3y2*60.+
     & c(5,3,1,n)*x3y2z1*60.+c(5,3,2,n)*x3y2z2*60.+c(5,3,3,n)*x3y2z3*
     & 60.+c(5,3,4,n)*x3y2z4*60.+c(5,3,5,n)*x3y2z5*60.+c(5,4,0,n)*
     & x3y3*80.+c(5,4,1,n)*x3y3z1*80.+c(5,4,2,n)*x3y3z2*80.+c(5,4,3,n)
     & *x3y3z3*80.+c(5,4,4,n)*x3y3z4*80.+c(5,4,5,n)*x3y3z5*80.+c(5,5,
     & 0,n)*x3y4*100.+c(5,5,1,n)*x3y4z1*100.+c(5,5,2,n)*x3y4z2*100.+c(
     & 5,5,3,n)*x3y4z3*100.+c(5,5,4,n)*x3y4z4*100.+c(5,5,5,n)*x3y4z5*
     & 100.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.2.and.dy.eq.1.and.dz.eq.1 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      y3z4=y2z4*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y4z2=y3z2*y1
      y4z3=y3z3*y1
      y4z4=y3z4*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y2z4=x1y1z4*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x1y3z4=x1y2z4*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y4z2=x1y3z2*y1
      x1y4z3=x1y3z3*y1
      x1y4z4=x1y3z4*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y1z4=x1y1z4*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y2z4=x1y2z4*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y3z3=x1y3z3*x1
      x2y3z4=x1y3z4*x1
      x2y4=x1y4*x1
      x2y4z1=x1y4z1*x1
      x2y4z2=x1y4z2*x1
      x2y4z3=x1y4z3*x1
      x2y4z4=x1y4z4*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3z4=x2z4*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x3y1z4=x2y1z4*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y2z3=x2y2z3*x1
      x3y2z4=x2y2z4*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x3y3z2=x2y3z2*x1
      x3y3z3=x2y3z3*x1
      x3y3z4=x2y3z4*x1
      x3y4=x2y4*x1
      x3y4z1=x2y4z1*x1
      x3y4z2=x2y4z2*x1
      x3y4z3=x2y4z3*x1
      x3y4z4=x2y4z4*x1
      r(i1,i2,i3,n)=(c(2,1,1,n)*2.+c(2,1,2,n)*z1*4.+c(2,1,3,n)*z2*6.+c(
     & 2,1,4,n)*z3*8.+c(2,1,5,n)*z4*10.+c(2,2,1,n)*y1*4.+c(2,2,2,n)*
     & y1z1*8.+c(2,2,3,n)*y1z2*12.+c(2,2,4,n)*y1z3*16.+c(2,2,5,n)*
     & y1z4*20.+c(2,3,1,n)*y2*6.+c(2,3,2,n)*y2z1*12.+c(2,3,3,n)*y2z2*
     & 18.+c(2,3,4,n)*y2z3*24.+c(2,3,5,n)*y2z4*30.+c(2,4,1,n)*y3*8.+c(
     & 2,4,2,n)*y3z1*16.+c(2,4,3,n)*y3z2*24.+c(2,4,4,n)*y3z3*32.+c(2,
     & 4,5,n)*y3z4*40.+c(2,5,1,n)*y4*10.+c(2,5,2,n)*y4z1*20.+c(2,5,3,
     & n)*y4z2*30.+c(2,5,4,n)*y4z3*40.+c(2,5,5,n)*y4z4*50.+c(3,1,1,n)*
     & x1*6.+c(3,1,2,n)*x1z1*12.+c(3,1,3,n)*x1z2*18.+c(3,1,4,n)*x1z3*
     & 24.+c(3,1,5,n)*x1z4*30.+c(3,2,1,n)*x1y1*12.+c(3,2,2,n)*x1y1z1*
     & 24.+c(3,2,3,n)*x1y1z2*36.+c(3,2,4,n)*x1y1z3*48.+c(3,2,5,n)*
     & x1y1z4*60.+c(3,3,1,n)*x1y2*18.+c(3,3,2,n)*x1y2z1*36.+c(3,3,3,n)
     & *x1y2z2*54.+c(3,3,4,n)*x1y2z3*72.+c(3,3,5,n)*x1y2z4*90.+c(3,4,
     & 1,n)*x1y3*24.+c(3,4,2,n)*x1y3z1*48.+c(3,4,3,n)*x1y3z2*72.+c(3,
     & 4,4,n)*x1y3z3*96.+c(3,4,5,n)*x1y3z4*120.+c(3,5,1,n)*x1y4*30.+c(
     & 3,5,2,n)*x1y4z1*60.+c(3,5,3,n)*x1y4z2*90.+c(3,5,4,n)*x1y4z3*
     & 120.+c(3,5,5,n)*x1y4z4*150.+c(4,1,1,n)*x2*12.+c(4,1,2,n)*x2z1*
     & 24.+c(4,1,3,n)*x2z2*36.+c(4,1,4,n)*x2z3*48.+c(4,1,5,n)*x2z4*
     & 60.+c(4,2,1,n)*x2y1*24.+c(4,2,2,n)*x2y1z1*48.+c(4,2,3,n)*
     & x2y1z2*72.+c(4,2,4,n)*x2y1z3*96.+c(4,2,5,n)*x2y1z4*120.+c(4,3,
     & 1,n)*x2y2*36.+c(4,3,2,n)*x2y2z1*72.+c(4,3,3,n)*x2y2z2*108.+c(4,
     & 3,4,n)*x2y2z3*144.+c(4,3,5,n)*x2y2z4*180.+c(4,4,1,n)*x2y3*48.+
     & c(4,4,2,n)*x2y3z1*96.+c(4,4,3,n)*x2y3z2*144.+c(4,4,4,n)*x2y3z3*
     & 192.+c(4,4,5,n)*x2y3z4*240.+c(4,5,1,n)*x2y4*60.+c(4,5,2,n)*
     & x2y4z1*120.+c(4,5,3,n)*x2y4z2*180.+c(4,5,4,n)*x2y4z3*240.+c(4,
     & 5,5,n)*x2y4z4*300.+c(5,1,1,n)*x3*20.+c(5,1,2,n)*x3z1*40.+c(5,1,
     & 3,n)*x3z2*60.+c(5,1,4,n)*x3z3*80.+c(5,1,5,n)*x3z4*100.+c(5,2,1,
     & n)*x3y1*40.+c(5,2,2,n)*x3y1z1*80.+c(5,2,3,n)*x3y1z2*120.+c(5,2,
     & 4,n)*x3y1z3*160.+c(5,2,5,n)*x3y1z4*200.+c(5,3,1,n)*x3y2*60.+c(
     & 5,3,2,n)*x3y2z1*120.+c(5,3,3,n)*x3y2z2*180.+c(5,3,4,n)*x3y2z3*
     & 240.+c(5,3,5,n)*x3y2z4*300.+c(5,4,1,n)*x3y3*80.+c(5,4,2,n)*
     & x3y3z1*160.+c(5,4,3,n)*x3y3z2*240.+c(5,4,4,n)*x3y3z3*320.+c(5,
     & 4,5,n)*x3y3z4*400.+c(5,5,1,n)*x3y4*100.+c(5,5,2,n)*x3y4z1*200.+
     & c(5,5,3,n)*x3y4z2*300.+c(5,5,4,n)*x3y4z3*400.+c(5,5,5,n)*
     & x3y4z4*500.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.2.and.dy.eq.1.and.dz.eq.2 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y4z2=y3z2*y1
      y4z3=y3z3*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y4z2=x1y3z2*y1
      x1y4z3=x1y3z3*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y3z3=x1y3z3*x1
      x2y4=x1y4*x1
      x2y4z1=x1y4z1*x1
      x2y4z2=x1y4z2*x1
      x2y4z3=x1y4z3*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y2z3=x2y2z3*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x3y3z2=x2y3z2*x1
      x3y3z3=x2y3z3*x1
      x3y4=x2y4*x1
      x3y4z1=x2y4z1*x1
      x3y4z2=x2y4z2*x1
      x3y4z3=x2y4z3*x1
      r(i1,i2,i3,n)=(c(2,1,2,n)*4.+c(2,1,3,n)*z1*12.+c(2,1,4,n)*z2*24.+
     & c(2,1,5,n)*z3*40.+c(2,2,2,n)*y1*8.+c(2,2,3,n)*y1z1*24.+c(2,2,4,
     & n)*y1z2*48.+c(2,2,5,n)*y1z3*80.+c(2,3,2,n)*y2*12.+c(2,3,3,n)*
     & y2z1*36.+c(2,3,4,n)*y2z2*72.+c(2,3,5,n)*y2z3*120.+c(2,4,2,n)*
     & y3*16.+c(2,4,3,n)*y3z1*48.+c(2,4,4,n)*y3z2*96.+c(2,4,5,n)*y3z3*
     & 160.+c(2,5,2,n)*y4*20.+c(2,5,3,n)*y4z1*60.+c(2,5,4,n)*y4z2*
     & 120.+c(2,5,5,n)*y4z3*200.+c(3,1,2,n)*x1*12.+c(3,1,3,n)*x1z1*
     & 36.+c(3,1,4,n)*x1z2*72.+c(3,1,5,n)*x1z3*120.+c(3,2,2,n)*x1y1*
     & 24.+c(3,2,3,n)*x1y1z1*72.+c(3,2,4,n)*x1y1z2*144.+c(3,2,5,n)*
     & x1y1z3*240.+c(3,3,2,n)*x1y2*36.+c(3,3,3,n)*x1y2z1*108.+c(3,3,4,
     & n)*x1y2z2*216.+c(3,3,5,n)*x1y2z3*360.+c(3,4,2,n)*x1y3*48.+c(3,
     & 4,3,n)*x1y3z1*144.+c(3,4,4,n)*x1y3z2*288.+c(3,4,5,n)*x1y3z3*
     & 480.+c(3,5,2,n)*x1y4*60.+c(3,5,3,n)*x1y4z1*180.+c(3,5,4,n)*
     & x1y4z2*360.+c(3,5,5,n)*x1y4z3*600.+c(4,1,2,n)*x2*24.+c(4,1,3,n)
     & *x2z1*72.+c(4,1,4,n)*x2z2*144.+c(4,1,5,n)*x2z3*240.+c(4,2,2,n)*
     & x2y1*48.+c(4,2,3,n)*x2y1z1*144.+c(4,2,4,n)*x2y1z2*288.+c(4,2,5,
     & n)*x2y1z3*480.+c(4,3,2,n)*x2y2*72.+c(4,3,3,n)*x2y2z1*216.+c(4,
     & 3,4,n)*x2y2z2*432.+c(4,3,5,n)*x2y2z3*720.+c(4,4,2,n)*x2y3*96.+
     & c(4,4,3,n)*x2y3z1*288.+c(4,4,4,n)*x2y3z2*576.+c(4,4,5,n)*
     & x2y3z3*960.+c(4,5,2,n)*x2y4*120.+c(4,5,3,n)*x2y4z1*360.+c(4,5,
     & 4,n)*x2y4z2*720.+c(4,5,5,n)*x2y4z3*1200.+c(5,1,2,n)*x3*40.+c(5,
     & 1,3,n)*x3z1*120.+c(5,1,4,n)*x3z2*240.+c(5,1,5,n)*x3z3*400.+c(5,
     & 2,2,n)*x3y1*80.+c(5,2,3,n)*x3y1z1*240.+c(5,2,4,n)*x3y1z2*480.+
     & c(5,2,5,n)*x3y1z3*800.+c(5,3,2,n)*x3y2*120.+c(5,3,3,n)*x3y2z1*
     & 360.+c(5,3,4,n)*x3y2z2*720.+c(5,3,5,n)*x3y2z3*1200.+c(5,4,2,n)*
     & x3y3*160.+c(5,4,3,n)*x3y3z1*480.+c(5,4,4,n)*x3y3z2*960.+c(5,4,
     & 5,n)*x3y3z3*1600.+c(5,5,2,n)*x3y4*200.+c(5,5,3,n)*x3y4z1*600.+
     & c(5,5,4,n)*x3y4z2*1200.+c(5,5,5,n)*x3y4z3*2000.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.2.and.dy.eq.1.and.dz.eq.3 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y4z2=y3z2*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y4z2=x1y3z2*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y4=x1y4*x1
      x2y4z1=x1y4z1*x1
      x2y4z2=x1y4z2*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x3y3z2=x2y3z2*x1
      x3y4=x2y4*x1
      x3y4z1=x2y4z1*x1
      x3y4z2=x2y4z2*x1
      r(i1,i2,i3,n)=(c(2,1,3,n)*12.+c(2,1,4,n)*z1*48.+c(2,1,5,n)*z2*
     & 120.+c(2,2,3,n)*y1*24.+c(2,2,4,n)*y1z1*96.+c(2,2,5,n)*y1z2*
     & 240.+c(2,3,3,n)*y2*36.+c(2,3,4,n)*y2z1*144.+c(2,3,5,n)*y2z2*
     & 360.+c(2,4,3,n)*y3*48.+c(2,4,4,n)*y3z1*192.+c(2,4,5,n)*y3z2*
     & 480.+c(2,5,3,n)*y4*60.+c(2,5,4,n)*y4z1*240.+c(2,5,5,n)*y4z2*
     & 600.+c(3,1,3,n)*x1*36.+c(3,1,4,n)*x1z1*144.+c(3,1,5,n)*x1z2*
     & 360.+c(3,2,3,n)*x1y1*72.+c(3,2,4,n)*x1y1z1*288.+c(3,2,5,n)*
     & x1y1z2*720.+c(3,3,3,n)*x1y2*108.+c(3,3,4,n)*x1y2z1*432.+c(3,3,
     & 5,n)*x1y2z2*1080.+c(3,4,3,n)*x1y3*144.+c(3,4,4,n)*x1y3z1*576.+
     & c(3,4,5,n)*x1y3z2*1440.+c(3,5,3,n)*x1y4*180.+c(3,5,4,n)*x1y4z1*
     & 720.+c(3,5,5,n)*x1y4z2*1800.+c(4,1,3,n)*x2*72.+c(4,1,4,n)*x2z1*
     & 288.+c(4,1,5,n)*x2z2*720.+c(4,2,3,n)*x2y1*144.+c(4,2,4,n)*
     & x2y1z1*576.+c(4,2,5,n)*x2y1z2*1440.+c(4,3,3,n)*x2y2*216.+c(4,3,
     & 4,n)*x2y2z1*864.+c(4,3,5,n)*x2y2z2*2160.+c(4,4,3,n)*x2y3*288.+
     & c(4,4,4,n)*x2y3z1*1152.+c(4,4,5,n)*x2y3z2*2880.+c(4,5,3,n)*
     & x2y4*360.+c(4,5,4,n)*x2y4z1*1440.+c(4,5,5,n)*x2y4z2*3600.+c(5,
     & 1,3,n)*x3*120.+c(5,1,4,n)*x3z1*480.+c(5,1,5,n)*x3z2*1200.+c(5,
     & 2,3,n)*x3y1*240.+c(5,2,4,n)*x3y1z1*960.+c(5,2,5,n)*x3y1z2*
     & 2400.+c(5,3,3,n)*x3y2*360.+c(5,3,4,n)*x3y2z1*1440.+c(5,3,5,n)*
     & x3y2z2*3600.+c(5,4,3,n)*x3y3*480.+c(5,4,4,n)*x3y3z1*1920.+c(5,
     & 4,5,n)*x3y3z2*4800.+c(5,5,3,n)*x3y4*600.+c(5,5,4,n)*x3y4z1*
     & 2400.+c(5,5,5,n)*x3y4z2*6000.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.2.and.dy.eq.1.and.dz.eq.4 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y4=y3*y1
      y4z1=y3z1*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y4=x1y4*x1
      x2y4z1=x1y4z1*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x3y4=x2y4*x1
      x3y4z1=x2y4z1*x1
      r(i1,i2,i3,n)=(c(2,1,4,n)*48.+c(2,1,5,n)*z1*240.+c(2,2,4,n)*y1*
     & 96.+c(2,2,5,n)*y1z1*480.+c(2,3,4,n)*y2*144.+c(2,3,5,n)*y2z1*
     & 720.+c(2,4,4,n)*y3*192.+c(2,4,5,n)*y3z1*960.+c(2,5,4,n)*y4*
     & 240.+c(2,5,5,n)*y4z1*1200.+c(3,1,4,n)*x1*144.+c(3,1,5,n)*x1z1*
     & 720.+c(3,2,4,n)*x1y1*288.+c(3,2,5,n)*x1y1z1*1440.+c(3,3,4,n)*
     & x1y2*432.+c(3,3,5,n)*x1y2z1*2160.+c(3,4,4,n)*x1y3*576.+c(3,4,5,
     & n)*x1y3z1*2880.+c(3,5,4,n)*x1y4*720.+c(3,5,5,n)*x1y4z1*3600.+c(
     & 4,1,4,n)*x2*288.+c(4,1,5,n)*x2z1*1440.+c(4,2,4,n)*x2y1*576.+c(
     & 4,2,5,n)*x2y1z1*2880.+c(4,3,4,n)*x2y2*864.+c(4,3,5,n)*x2y2z1*
     & 4320.+c(4,4,4,n)*x2y3*1152.+c(4,4,5,n)*x2y3z1*5760.+c(4,5,4,n)*
     & x2y4*1440.+c(4,5,5,n)*x2y4z1*7200.+c(5,1,4,n)*x3*480.+c(5,1,5,
     & n)*x3z1*2400.+c(5,2,4,n)*x3y1*960.+c(5,2,5,n)*x3y1z1*4800.+c(5,
     & 3,4,n)*x3y2*1440.+c(5,3,5,n)*x3y2z1*7200.+c(5,4,4,n)*x3y3*
     & 1920.+c(5,4,5,n)*x3y3z1*9600.+c(5,5,4,n)*x3y4*2400.+c(5,5,5,n)*
     & x3y4z1*12000.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.2.and.dy.eq.2.and.dz.eq.0 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y1z5=y1z4*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
      y2z5=y1z5*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      y3z4=y2z4*y1
      y3z5=y2z5*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1z5=x1z4*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y1z5=x1y1z4*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y2z4=x1y1z4*y1
      x1y2z5=x1y1z5*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x1y3z4=x1y2z4*y1
      x1y3z5=x1y2z5*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x2z5=x1z5*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y1z4=x1y1z4*x1
      x2y1z5=x1y1z5*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y2z4=x1y2z4*x1
      x2y2z5=x1y2z5*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y3z3=x1y3z3*x1
      x2y3z4=x1y3z4*x1
      x2y3z5=x1y3z5*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3z4=x2z4*x1
      x3z5=x2z5*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x3y1z4=x2y1z4*x1
      x3y1z5=x2y1z5*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y2z3=x2y2z3*x1
      x3y2z4=x2y2z4*x1
      x3y2z5=x2y2z5*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x3y3z2=x2y3z2*x1
      x3y3z3=x2y3z3*x1
      x3y3z4=x2y3z4*x1
      x3y3z5=x2y3z5*x1
      r(i1,i2,i3,n)=(c(2,2,0,n)*4.+c(2,2,1,n)*z1*4.+c(2,2,2,n)*z2*4.+c(
     & 2,2,3,n)*z3*4.+c(2,2,4,n)*z4*4.+c(2,2,5,n)*z5*4.+c(2,3,0,n)*y1*
     & 12.+c(2,3,1,n)*y1z1*12.+c(2,3,2,n)*y1z2*12.+c(2,3,3,n)*y1z3*
     & 12.+c(2,3,4,n)*y1z4*12.+c(2,3,5,n)*y1z5*12.+c(2,4,0,n)*y2*24.+
     & c(2,4,1,n)*y2z1*24.+c(2,4,2,n)*y2z2*24.+c(2,4,3,n)*y2z3*24.+c(
     & 2,4,4,n)*y2z4*24.+c(2,4,5,n)*y2z5*24.+c(2,5,0,n)*y3*40.+c(2,5,
     & 1,n)*y3z1*40.+c(2,5,2,n)*y3z2*40.+c(2,5,3,n)*y3z3*40.+c(2,5,4,
     & n)*y3z4*40.+c(2,5,5,n)*y3z5*40.+c(3,2,0,n)*x1*12.+c(3,2,1,n)*
     & x1z1*12.+c(3,2,2,n)*x1z2*12.+c(3,2,3,n)*x1z3*12.+c(3,2,4,n)*
     & x1z4*12.+c(3,2,5,n)*x1z5*12.+c(3,3,0,n)*x1y1*36.+c(3,3,1,n)*
     & x1y1z1*36.+c(3,3,2,n)*x1y1z2*36.+c(3,3,3,n)*x1y1z3*36.+c(3,3,4,
     & n)*x1y1z4*36.+c(3,3,5,n)*x1y1z5*36.+c(3,4,0,n)*x1y2*72.+c(3,4,
     & 1,n)*x1y2z1*72.+c(3,4,2,n)*x1y2z2*72.+c(3,4,3,n)*x1y2z3*72.+c(
     & 3,4,4,n)*x1y2z4*72.+c(3,4,5,n)*x1y2z5*72.+c(3,5,0,n)*x1y3*120.+
     & c(3,5,1,n)*x1y3z1*120.+c(3,5,2,n)*x1y3z2*120.+c(3,5,3,n)*
     & x1y3z3*120.+c(3,5,4,n)*x1y3z4*120.+c(3,5,5,n)*x1y3z5*120.+c(4,
     & 2,0,n)*x2*24.+c(4,2,1,n)*x2z1*24.+c(4,2,2,n)*x2z2*24.+c(4,2,3,
     & n)*x2z3*24.+c(4,2,4,n)*x2z4*24.+c(4,2,5,n)*x2z5*24.+c(4,3,0,n)*
     & x2y1*72.+c(4,3,1,n)*x2y1z1*72.+c(4,3,2,n)*x2y1z2*72.+c(4,3,3,n)
     & *x2y1z3*72.+c(4,3,4,n)*x2y1z4*72.+c(4,3,5,n)*x2y1z5*72.+c(4,4,
     & 0,n)*x2y2*144.+c(4,4,1,n)*x2y2z1*144.+c(4,4,2,n)*x2y2z2*144.+c(
     & 4,4,3,n)*x2y2z3*144.+c(4,4,4,n)*x2y2z4*144.+c(4,4,5,n)*x2y2z5*
     & 144.+c(4,5,0,n)*x2y3*240.+c(4,5,1,n)*x2y3z1*240.+c(4,5,2,n)*
     & x2y3z2*240.+c(4,5,3,n)*x2y3z3*240.+c(4,5,4,n)*x2y3z4*240.+c(4,
     & 5,5,n)*x2y3z5*240.+c(5,2,0,n)*x3*40.+c(5,2,1,n)*x3z1*40.+c(5,2,
     & 2,n)*x3z2*40.+c(5,2,3,n)*x3z3*40.+c(5,2,4,n)*x3z4*40.+c(5,2,5,
     & n)*x3z5*40.+c(5,3,0,n)*x3y1*120.+c(5,3,1,n)*x3y1z1*120.+c(5,3,
     & 2,n)*x3y1z2*120.+c(5,3,3,n)*x3y1z3*120.+c(5,3,4,n)*x3y1z4*120.+
     & c(5,3,5,n)*x3y1z5*120.+c(5,4,0,n)*x3y2*240.+c(5,4,1,n)*x3y2z1*
     & 240.+c(5,4,2,n)*x3y2z2*240.+c(5,4,3,n)*x3y2z3*240.+c(5,4,4,n)*
     & x3y2z4*240.+c(5,4,5,n)*x3y2z5*240.+c(5,5,0,n)*x3y3*400.+c(5,5,
     & 1,n)*x3y3z1*400.+c(5,5,2,n)*x3y3z2*400.+c(5,5,3,n)*x3y3z3*400.+
     & c(5,5,4,n)*x3y3z4*400.+c(5,5,5,n)*x3y3z5*400.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.2.and.dy.eq.2.and.dz.eq.1 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      y3z4=y2z4*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y2z4=x1y1z4*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x1y3z4=x1y2z4*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y1z4=x1y1z4*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y2z4=x1y2z4*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y3z3=x1y3z3*x1
      x2y3z4=x1y3z4*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3z4=x2z4*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x3y1z4=x2y1z4*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y2z3=x2y2z3*x1
      x3y2z4=x2y2z4*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x3y3z2=x2y3z2*x1
      x3y3z3=x2y3z3*x1
      x3y3z4=x2y3z4*x1
      r(i1,i2,i3,n)=(c(2,2,1,n)*4.+c(2,2,2,n)*z1*8.+c(2,2,3,n)*z2*12.+
     & c(2,2,4,n)*z3*16.+c(2,2,5,n)*z4*20.+c(2,3,1,n)*y1*12.+c(2,3,2,
     & n)*y1z1*24.+c(2,3,3,n)*y1z2*36.+c(2,3,4,n)*y1z3*48.+c(2,3,5,n)*
     & y1z4*60.+c(2,4,1,n)*y2*24.+c(2,4,2,n)*y2z1*48.+c(2,4,3,n)*y2z2*
     & 72.+c(2,4,4,n)*y2z3*96.+c(2,4,5,n)*y2z4*120.+c(2,5,1,n)*y3*40.+
     & c(2,5,2,n)*y3z1*80.+c(2,5,3,n)*y3z2*120.+c(2,5,4,n)*y3z3*160.+
     & c(2,5,5,n)*y3z4*200.+c(3,2,1,n)*x1*12.+c(3,2,2,n)*x1z1*24.+c(3,
     & 2,3,n)*x1z2*36.+c(3,2,4,n)*x1z3*48.+c(3,2,5,n)*x1z4*60.+c(3,3,
     & 1,n)*x1y1*36.+c(3,3,2,n)*x1y1z1*72.+c(3,3,3,n)*x1y1z2*108.+c(3,
     & 3,4,n)*x1y1z3*144.+c(3,3,5,n)*x1y1z4*180.+c(3,4,1,n)*x1y2*72.+
     & c(3,4,2,n)*x1y2z1*144.+c(3,4,3,n)*x1y2z2*216.+c(3,4,4,n)*
     & x1y2z3*288.+c(3,4,5,n)*x1y2z4*360.+c(3,5,1,n)*x1y3*120.+c(3,5,
     & 2,n)*x1y3z1*240.+c(3,5,3,n)*x1y3z2*360.+c(3,5,4,n)*x1y3z3*480.+
     & c(3,5,5,n)*x1y3z4*600.+c(4,2,1,n)*x2*24.+c(4,2,2,n)*x2z1*48.+c(
     & 4,2,3,n)*x2z2*72.+c(4,2,4,n)*x2z3*96.+c(4,2,5,n)*x2z4*120.+c(4,
     & 3,1,n)*x2y1*72.+c(4,3,2,n)*x2y1z1*144.+c(4,3,3,n)*x2y1z2*216.+
     & c(4,3,4,n)*x2y1z3*288.+c(4,3,5,n)*x2y1z4*360.+c(4,4,1,n)*x2y2*
     & 144.+c(4,4,2,n)*x2y2z1*288.+c(4,4,3,n)*x2y2z2*432.+c(4,4,4,n)*
     & x2y2z3*576.+c(4,4,5,n)*x2y2z4*720.+c(4,5,1,n)*x2y3*240.+c(4,5,
     & 2,n)*x2y3z1*480.+c(4,5,3,n)*x2y3z2*720.+c(4,5,4,n)*x2y3z3*960.+
     & c(4,5,5,n)*x2y3z4*1200.+c(5,2,1,n)*x3*40.+c(5,2,2,n)*x3z1*80.+
     & c(5,2,3,n)*x3z2*120.+c(5,2,4,n)*x3z3*160.+c(5,2,5,n)*x3z4*200.+
     & c(5,3,1,n)*x3y1*120.+c(5,3,2,n)*x3y1z1*240.+c(5,3,3,n)*x3y1z2*
     & 360.+c(5,3,4,n)*x3y1z3*480.+c(5,3,5,n)*x3y1z4*600.+c(5,4,1,n)*
     & x3y2*240.+c(5,4,2,n)*x3y2z1*480.+c(5,4,3,n)*x3y2z2*720.+c(5,4,
     & 4,n)*x3y2z3*960.+c(5,4,5,n)*x3y2z4*1200.+c(5,5,1,n)*x3y3*400.+
     & c(5,5,2,n)*x3y3z1*800.+c(5,5,3,n)*x3y3z2*1200.+c(5,5,4,n)*
     & x3y3z3*1600.+c(5,5,5,n)*x3y3z4*2000.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.2.and.dy.eq.2.and.dz.eq.2 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y3z3=x1y3z3*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y2z3=x2y2z3*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x3y3z2=x2y3z2*x1
      x3y3z3=x2y3z3*x1
      r(i1,i2,i3,n)=(c(2,2,2,n)*8.+c(2,2,3,n)*z1*24.+c(2,2,4,n)*z2*48.+
     & c(2,2,5,n)*z3*80.+c(2,3,2,n)*y1*24.+c(2,3,3,n)*y1z1*72.+c(2,3,
     & 4,n)*y1z2*144.+c(2,3,5,n)*y1z3*240.+c(2,4,2,n)*y2*48.+c(2,4,3,
     & n)*y2z1*144.+c(2,4,4,n)*y2z2*288.+c(2,4,5,n)*y2z3*480.+c(2,5,2,
     & n)*y3*80.+c(2,5,3,n)*y3z1*240.+c(2,5,4,n)*y3z2*480.+c(2,5,5,n)*
     & y3z3*800.+c(3,2,2,n)*x1*24.+c(3,2,3,n)*x1z1*72.+c(3,2,4,n)*
     & x1z2*144.+c(3,2,5,n)*x1z3*240.+c(3,3,2,n)*x1y1*72.+c(3,3,3,n)*
     & x1y1z1*216.+c(3,3,4,n)*x1y1z2*432.+c(3,3,5,n)*x1y1z3*720.+c(3,
     & 4,2,n)*x1y2*144.+c(3,4,3,n)*x1y2z1*432.+c(3,4,4,n)*x1y2z2*864.+
     & c(3,4,5,n)*x1y2z3*1440.+c(3,5,2,n)*x1y3*240.+c(3,5,3,n)*x1y3z1*
     & 720.+c(3,5,4,n)*x1y3z2*1440.+c(3,5,5,n)*x1y3z3*2400.+c(4,2,2,n)
     & *x2*48.+c(4,2,3,n)*x2z1*144.+c(4,2,4,n)*x2z2*288.+c(4,2,5,n)*
     & x2z3*480.+c(4,3,2,n)*x2y1*144.+c(4,3,3,n)*x2y1z1*432.+c(4,3,4,
     & n)*x2y1z2*864.+c(4,3,5,n)*x2y1z3*1440.+c(4,4,2,n)*x2y2*288.+c(
     & 4,4,3,n)*x2y2z1*864.+c(4,4,4,n)*x2y2z2*1728.+c(4,4,5,n)*x2y2z3*
     & 2880.+c(4,5,2,n)*x2y3*480.+c(4,5,3,n)*x2y3z1*1440.+c(4,5,4,n)*
     & x2y3z2*2880.+c(4,5,5,n)*x2y3z3*4800.+c(5,2,2,n)*x3*80.+c(5,2,3,
     & n)*x3z1*240.+c(5,2,4,n)*x3z2*480.+c(5,2,5,n)*x3z3*800.+c(5,3,2,
     & n)*x3y1*240.+c(5,3,3,n)*x3y1z1*720.+c(5,3,4,n)*x3y1z2*1440.+c(
     & 5,3,5,n)*x3y1z3*2400.+c(5,4,2,n)*x3y2*480.+c(5,4,3,n)*x3y2z1*
     & 1440.+c(5,4,4,n)*x3y2z2*2880.+c(5,4,5,n)*x3y2z3*4800.+c(5,5,2,
     & n)*x3y3*800.+c(5,5,3,n)*x3y3z1*2400.+c(5,5,4,n)*x3y3z2*4800.+c(
     & 5,5,5,n)*x3y3z3*8000.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.2.and.dy.eq.2.and.dz.eq.3 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x3y3z2=x2y3z2*x1
      r(i1,i2,i3,n)=(c(2,2,3,n)*24.+c(2,2,4,n)*z1*96.+c(2,2,5,n)*z2*
     & 240.+c(2,3,3,n)*y1*72.+c(2,3,4,n)*y1z1*288.+c(2,3,5,n)*y1z2*
     & 720.+c(2,4,3,n)*y2*144.+c(2,4,4,n)*y2z1*576.+c(2,4,5,n)*y2z2*
     & 1440.+c(2,5,3,n)*y3*240.+c(2,5,4,n)*y3z1*960.+c(2,5,5,n)*y3z2*
     & 2400.+c(3,2,3,n)*x1*72.+c(3,2,4,n)*x1z1*288.+c(3,2,5,n)*x1z2*
     & 720.+c(3,3,3,n)*x1y1*216.+c(3,3,4,n)*x1y1z1*864.+c(3,3,5,n)*
     & x1y1z2*2160.+c(3,4,3,n)*x1y2*432.+c(3,4,4,n)*x1y2z1*1728.+c(3,
     & 4,5,n)*x1y2z2*4320.+c(3,5,3,n)*x1y3*720.+c(3,5,4,n)*x1y3z1*
     & 2880.+c(3,5,5,n)*x1y3z2*7200.+c(4,2,3,n)*x2*144.+c(4,2,4,n)*
     & x2z1*576.+c(4,2,5,n)*x2z2*1440.+c(4,3,3,n)*x2y1*432.+c(4,3,4,n)
     & *x2y1z1*1728.+c(4,3,5,n)*x2y1z2*4320.+c(4,4,3,n)*x2y2*864.+c(4,
     & 4,4,n)*x2y2z1*3456.+c(4,4,5,n)*x2y2z2*8640.+c(4,5,3,n)*x2y3*
     & 1440.+c(4,5,4,n)*x2y3z1*5760.+c(4,5,5,n)*x2y3z2*14400.+c(5,2,3,
     & n)*x3*240.+c(5,2,4,n)*x3z1*960.+c(5,2,5,n)*x3z2*2400.+c(5,3,3,
     & n)*x3y1*720.+c(5,3,4,n)*x3y1z1*2880.+c(5,3,5,n)*x3y1z2*7200.+c(
     & 5,4,3,n)*x3y2*1440.+c(5,4,4,n)*x3y2z1*5760.+c(5,4,5,n)*x3y2z2*
     & 14400.+c(5,5,3,n)*x3y3*2400.+c(5,5,4,n)*x3y3z1*9600.+c(5,5,5,n)
     & *x3y3z2*24000.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.2.and.dy.eq.2.and.dz.eq.4 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y3=y2*y1
      y3z1=y2z1*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      r(i1,i2,i3,n)=(c(2,2,4,n)*96.+c(2,2,5,n)*z1*480.+c(2,3,4,n)*y1*
     & 288.+c(2,3,5,n)*y1z1*1440.+c(2,4,4,n)*y2*576.+c(2,4,5,n)*y2z1*
     & 2880.+c(2,5,4,n)*y3*960.+c(2,5,5,n)*y3z1*4800.+c(3,2,4,n)*x1*
     & 288.+c(3,2,5,n)*x1z1*1440.+c(3,3,4,n)*x1y1*864.+c(3,3,5,n)*
     & x1y1z1*4320.+c(3,4,4,n)*x1y2*1728.+c(3,4,5,n)*x1y2z1*8640.+c(3,
     & 5,4,n)*x1y3*2880.+c(3,5,5,n)*x1y3z1*14400.+c(4,2,4,n)*x2*576.+
     & c(4,2,5,n)*x2z1*2880.+c(4,3,4,n)*x2y1*1728.+c(4,3,5,n)*x2y1z1*
     & 8640.+c(4,4,4,n)*x2y2*3456.+c(4,4,5,n)*x2y2z1*17280.+c(4,5,4,n)
     & *x2y3*5760.+c(4,5,5,n)*x2y3z1*28800.+c(5,2,4,n)*x3*960.+c(5,2,
     & 5,n)*x3z1*4800.+c(5,3,4,n)*x3y1*2880.+c(5,3,5,n)*x3y1z1*14400.+
     & c(5,4,4,n)*x3y2*5760.+c(5,4,5,n)*x3y2z1*28800.+c(5,5,4,n)*x3y3*
     & 9600.+c(5,5,5,n)*x3y3z1*48000.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.2.and.dy.eq.3.and.dz.eq.0 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y1z5=y1z4*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
      y2z5=y1z5*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1z5=x1z4*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y1z5=x1y1z4*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y2z4=x1y1z4*y1
      x1y2z5=x1y1z5*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x2z5=x1z5*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y1z4=x1y1z4*x1
      x2y1z5=x1y1z5*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y2z4=x1y2z4*x1
      x2y2z5=x1y2z5*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3z4=x2z4*x1
      x3z5=x2z5*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x3y1z4=x2y1z4*x1
      x3y1z5=x2y1z5*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y2z3=x2y2z3*x1
      x3y2z4=x2y2z4*x1
      x3y2z5=x2y2z5*x1
      r(i1,i2,i3,n)=(c(2,3,0,n)*12.+c(2,3,1,n)*z1*12.+c(2,3,2,n)*z2*
     & 12.+c(2,3,3,n)*z3*12.+c(2,3,4,n)*z4*12.+c(2,3,5,n)*z5*12.+c(2,
     & 4,0,n)*y1*48.+c(2,4,1,n)*y1z1*48.+c(2,4,2,n)*y1z2*48.+c(2,4,3,
     & n)*y1z3*48.+c(2,4,4,n)*y1z4*48.+c(2,4,5,n)*y1z5*48.+c(2,5,0,n)*
     & y2*120.+c(2,5,1,n)*y2z1*120.+c(2,5,2,n)*y2z2*120.+c(2,5,3,n)*
     & y2z3*120.+c(2,5,4,n)*y2z4*120.+c(2,5,5,n)*y2z5*120.+c(3,3,0,n)*
     & x1*36.+c(3,3,1,n)*x1z1*36.+c(3,3,2,n)*x1z2*36.+c(3,3,3,n)*x1z3*
     & 36.+c(3,3,4,n)*x1z4*36.+c(3,3,5,n)*x1z5*36.+c(3,4,0,n)*x1y1*
     & 144.+c(3,4,1,n)*x1y1z1*144.+c(3,4,2,n)*x1y1z2*144.+c(3,4,3,n)*
     & x1y1z3*144.+c(3,4,4,n)*x1y1z4*144.+c(3,4,5,n)*x1y1z5*144.+c(3,
     & 5,0,n)*x1y2*360.+c(3,5,1,n)*x1y2z1*360.+c(3,5,2,n)*x1y2z2*360.+
     & c(3,5,3,n)*x1y2z3*360.+c(3,5,4,n)*x1y2z4*360.+c(3,5,5,n)*
     & x1y2z5*360.+c(4,3,0,n)*x2*72.+c(4,3,1,n)*x2z1*72.+c(4,3,2,n)*
     & x2z2*72.+c(4,3,3,n)*x2z3*72.+c(4,3,4,n)*x2z4*72.+c(4,3,5,n)*
     & x2z5*72.+c(4,4,0,n)*x2y1*288.+c(4,4,1,n)*x2y1z1*288.+c(4,4,2,n)
     & *x2y1z2*288.+c(4,4,3,n)*x2y1z3*288.+c(4,4,4,n)*x2y1z4*288.+c(4,
     & 4,5,n)*x2y1z5*288.+c(4,5,0,n)*x2y2*720.+c(4,5,1,n)*x2y2z1*720.+
     & c(4,5,2,n)*x2y2z2*720.+c(4,5,3,n)*x2y2z3*720.+c(4,5,4,n)*
     & x2y2z4*720.+c(4,5,5,n)*x2y2z5*720.+c(5,3,0,n)*x3*120.+c(5,3,1,
     & n)*x3z1*120.+c(5,3,2,n)*x3z2*120.+c(5,3,3,n)*x3z3*120.+c(5,3,4,
     & n)*x3z4*120.+c(5,3,5,n)*x3z5*120.+c(5,4,0,n)*x3y1*480.+c(5,4,1,
     & n)*x3y1z1*480.+c(5,4,2,n)*x3y1z2*480.+c(5,4,3,n)*x3y1z3*480.+c(
     & 5,4,4,n)*x3y1z4*480.+c(5,4,5,n)*x3y1z5*480.+c(5,5,0,n)*x3y2*
     & 1200.+c(5,5,1,n)*x3y2z1*1200.+c(5,5,2,n)*x3y2z2*1200.+c(5,5,3,
     & n)*x3y2z3*1200.+c(5,5,4,n)*x3y2z4*1200.+c(5,5,5,n)*x3y2z5*
     & 1200.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.2.and.dy.eq.3.and.dz.eq.1 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y2z4=x1y1z4*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y1z4=x1y1z4*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y2z4=x1y2z4*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3z4=x2z4*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x3y1z4=x2y1z4*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y2z3=x2y2z3*x1
      x3y2z4=x2y2z4*x1
      r(i1,i2,i3,n)=(c(2,3,1,n)*12.+c(2,3,2,n)*z1*24.+c(2,3,3,n)*z2*
     & 36.+c(2,3,4,n)*z3*48.+c(2,3,5,n)*z4*60.+c(2,4,1,n)*y1*48.+c(2,
     & 4,2,n)*y1z1*96.+c(2,4,3,n)*y1z2*144.+c(2,4,4,n)*y1z3*192.+c(2,
     & 4,5,n)*y1z4*240.+c(2,5,1,n)*y2*120.+c(2,5,2,n)*y2z1*240.+c(2,5,
     & 3,n)*y2z2*360.+c(2,5,4,n)*y2z3*480.+c(2,5,5,n)*y2z4*600.+c(3,3,
     & 1,n)*x1*36.+c(3,3,2,n)*x1z1*72.+c(3,3,3,n)*x1z2*108.+c(3,3,4,n)
     & *x1z3*144.+c(3,3,5,n)*x1z4*180.+c(3,4,1,n)*x1y1*144.+c(3,4,2,n)
     & *x1y1z1*288.+c(3,4,3,n)*x1y1z2*432.+c(3,4,4,n)*x1y1z3*576.+c(3,
     & 4,5,n)*x1y1z4*720.+c(3,5,1,n)*x1y2*360.+c(3,5,2,n)*x1y2z1*720.+
     & c(3,5,3,n)*x1y2z2*1080.+c(3,5,4,n)*x1y2z3*1440.+c(3,5,5,n)*
     & x1y2z4*1800.+c(4,3,1,n)*x2*72.+c(4,3,2,n)*x2z1*144.+c(4,3,3,n)*
     & x2z2*216.+c(4,3,4,n)*x2z3*288.+c(4,3,5,n)*x2z4*360.+c(4,4,1,n)*
     & x2y1*288.+c(4,4,2,n)*x2y1z1*576.+c(4,4,3,n)*x2y1z2*864.+c(4,4,
     & 4,n)*x2y1z3*1152.+c(4,4,5,n)*x2y1z4*1440.+c(4,5,1,n)*x2y2*720.+
     & c(4,5,2,n)*x2y2z1*1440.+c(4,5,3,n)*x2y2z2*2160.+c(4,5,4,n)*
     & x2y2z3*2880.+c(4,5,5,n)*x2y2z4*3600.+c(5,3,1,n)*x3*120.+c(5,3,
     & 2,n)*x3z1*240.+c(5,3,3,n)*x3z2*360.+c(5,3,4,n)*x3z3*480.+c(5,3,
     & 5,n)*x3z4*600.+c(5,4,1,n)*x3y1*480.+c(5,4,2,n)*x3y1z1*960.+c(5,
     & 4,3,n)*x3y1z2*1440.+c(5,4,4,n)*x3y1z3*1920.+c(5,4,5,n)*x3y1z4*
     & 2400.+c(5,5,1,n)*x3y2*1200.+c(5,5,2,n)*x3y2z1*2400.+c(5,5,3,n)*
     & x3y2z2*3600.+c(5,5,4,n)*x3y2z3*4800.+c(5,5,5,n)*x3y2z4*6000.)*
     & time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.2.and.dy.eq.3.and.dz.eq.2 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y2z3=x2y2z3*x1
      r(i1,i2,i3,n)=(c(2,3,2,n)*24.+c(2,3,3,n)*z1*72.+c(2,3,4,n)*z2*
     & 144.+c(2,3,5,n)*z3*240.+c(2,4,2,n)*y1*96.+c(2,4,3,n)*y1z1*288.+
     & c(2,4,4,n)*y1z2*576.+c(2,4,5,n)*y1z3*960.+c(2,5,2,n)*y2*240.+c(
     & 2,5,3,n)*y2z1*720.+c(2,5,4,n)*y2z2*1440.+c(2,5,5,n)*y2z3*2400.+
     & c(3,3,2,n)*x1*72.+c(3,3,3,n)*x1z1*216.+c(3,3,4,n)*x1z2*432.+c(
     & 3,3,5,n)*x1z3*720.+c(3,4,2,n)*x1y1*288.+c(3,4,3,n)*x1y1z1*864.+
     & c(3,4,4,n)*x1y1z2*1728.+c(3,4,5,n)*x1y1z3*2880.+c(3,5,2,n)*
     & x1y2*720.+c(3,5,3,n)*x1y2z1*2160.+c(3,5,4,n)*x1y2z2*4320.+c(3,
     & 5,5,n)*x1y2z3*7200.+c(4,3,2,n)*x2*144.+c(4,3,3,n)*x2z1*432.+c(
     & 4,3,4,n)*x2z2*864.+c(4,3,5,n)*x2z3*1440.+c(4,4,2,n)*x2y1*576.+
     & c(4,4,3,n)*x2y1z1*1728.+c(4,4,4,n)*x2y1z2*3456.+c(4,4,5,n)*
     & x2y1z3*5760.+c(4,5,2,n)*x2y2*1440.+c(4,5,3,n)*x2y2z1*4320.+c(4,
     & 5,4,n)*x2y2z2*8640.+c(4,5,5,n)*x2y2z3*14400.+c(5,3,2,n)*x3*
     & 240.+c(5,3,3,n)*x3z1*720.+c(5,3,4,n)*x3z2*1440.+c(5,3,5,n)*
     & x3z3*2400.+c(5,4,2,n)*x3y1*960.+c(5,4,3,n)*x3y1z1*2880.+c(5,4,
     & 4,n)*x3y1z2*5760.+c(5,4,5,n)*x3y1z3*9600.+c(5,5,2,n)*x3y2*
     & 2400.+c(5,5,3,n)*x3y2z1*7200.+c(5,5,4,n)*x3y2z2*14400.+c(5,5,5,
     & n)*x3y2z3*24000.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.2.and.dy.eq.3.and.dz.eq.3 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      r(i1,i2,i3,n)=(c(2,3,3,n)*72.+c(2,3,4,n)*z1*288.+c(2,3,5,n)*z2*
     & 720.+c(2,4,3,n)*y1*288.+c(2,4,4,n)*y1z1*1152.+c(2,4,5,n)*y1z2*
     & 2880.+c(2,5,3,n)*y2*720.+c(2,5,4,n)*y2z1*2880.+c(2,5,5,n)*y2z2*
     & 7200.+c(3,3,3,n)*x1*216.+c(3,3,4,n)*x1z1*864.+c(3,3,5,n)*x1z2*
     & 2160.+c(3,4,3,n)*x1y1*864.+c(3,4,4,n)*x1y1z1*3456.+c(3,4,5,n)*
     & x1y1z2*8640.+c(3,5,3,n)*x1y2*2160.+c(3,5,4,n)*x1y2z1*8640.+c(3,
     & 5,5,n)*x1y2z2*21600.+c(4,3,3,n)*x2*432.+c(4,3,4,n)*x2z1*1728.+
     & c(4,3,5,n)*x2z2*4320.+c(4,4,3,n)*x2y1*1728.+c(4,4,4,n)*x2y1z1*
     & 6912.+c(4,4,5,n)*x2y1z2*17280.+c(4,5,3,n)*x2y2*4320.+c(4,5,4,n)
     & *x2y2z1*17280.+c(4,5,5,n)*x2y2z2*43200.+c(5,3,3,n)*x3*720.+c(5,
     & 3,4,n)*x3z1*2880.+c(5,3,5,n)*x3z2*7200.+c(5,4,3,n)*x3y1*2880.+
     & c(5,4,4,n)*x3y1z1*11520.+c(5,4,5,n)*x3y1z2*28800.+c(5,5,3,n)*
     & x3y2*7200.+c(5,5,4,n)*x3y2z1*28800.+c(5,5,5,n)*x3y2z2*72000.)*
     & time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.2.and.dy.eq.3.and.dz.eq.4 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      r(i1,i2,i3,n)=(c(2,3,4,n)*288.+c(2,3,5,n)*z1*1440.+c(2,4,4,n)*y1*
     & 1152.+c(2,4,5,n)*y1z1*5760.+c(2,5,4,n)*y2*2880.+c(2,5,5,n)*
     & y2z1*14400.+c(3,3,4,n)*x1*864.+c(3,3,5,n)*x1z1*4320.+c(3,4,4,n)
     & *x1y1*3456.+c(3,4,5,n)*x1y1z1*17280.+c(3,5,4,n)*x1y2*8640.+c(3,
     & 5,5,n)*x1y2z1*43200.+c(4,3,4,n)*x2*1728.+c(4,3,5,n)*x2z1*8640.+
     & c(4,4,4,n)*x2y1*6912.+c(4,4,5,n)*x2y1z1*34560.+c(4,5,4,n)*x2y2*
     & 17280.+c(4,5,5,n)*x2y2z1*86400.+c(5,3,4,n)*x3*2880.+c(5,3,5,n)*
     & x3z1*14400.+c(5,4,4,n)*x3y1*11520.+c(5,4,5,n)*x3y1z1*57600.+c(
     & 5,5,4,n)*x3y2*28800.+c(5,5,5,n)*x3y2z1*144000.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.2.and.dy.eq.4.and.dz.eq.0 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y1z5=y1z4*z1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1z5=x1z4*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y1z5=x1y1z4*z1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x2z5=x1z5*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y1z4=x1y1z4*x1
      x2y1z5=x1y1z5*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3z4=x2z4*x1
      x3z5=x2z5*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x3y1z4=x2y1z4*x1
      x3y1z5=x2y1z5*x1
      r(i1,i2,i3,n)=(c(2,4,0,n)*48.+c(2,4,1,n)*z1*48.+c(2,4,2,n)*z2*
     & 48.+c(2,4,3,n)*z3*48.+c(2,4,4,n)*z4*48.+c(2,4,5,n)*z5*48.+c(2,
     & 5,0,n)*y1*240.+c(2,5,1,n)*y1z1*240.+c(2,5,2,n)*y1z2*240.+c(2,5,
     & 3,n)*y1z3*240.+c(2,5,4,n)*y1z4*240.+c(2,5,5,n)*y1z5*240.+c(3,4,
     & 0,n)*x1*144.+c(3,4,1,n)*x1z1*144.+c(3,4,2,n)*x1z2*144.+c(3,4,3,
     & n)*x1z3*144.+c(3,4,4,n)*x1z4*144.+c(3,4,5,n)*x1z5*144.+c(3,5,0,
     & n)*x1y1*720.+c(3,5,1,n)*x1y1z1*720.+c(3,5,2,n)*x1y1z2*720.+c(3,
     & 5,3,n)*x1y1z3*720.+c(3,5,4,n)*x1y1z4*720.+c(3,5,5,n)*x1y1z5*
     & 720.+c(4,4,0,n)*x2*288.+c(4,4,1,n)*x2z1*288.+c(4,4,2,n)*x2z2*
     & 288.+c(4,4,3,n)*x2z3*288.+c(4,4,4,n)*x2z4*288.+c(4,4,5,n)*x2z5*
     & 288.+c(4,5,0,n)*x2y1*1440.+c(4,5,1,n)*x2y1z1*1440.+c(4,5,2,n)*
     & x2y1z2*1440.+c(4,5,3,n)*x2y1z3*1440.+c(4,5,4,n)*x2y1z4*1440.+c(
     & 4,5,5,n)*x2y1z5*1440.+c(5,4,0,n)*x3*480.+c(5,4,1,n)*x3z1*480.+
     & c(5,4,2,n)*x3z2*480.+c(5,4,3,n)*x3z3*480.+c(5,4,4,n)*x3z4*480.+
     & c(5,4,5,n)*x3z5*480.+c(5,5,0,n)*x3y1*2400.+c(5,5,1,n)*x3y1z1*
     & 2400.+c(5,5,2,n)*x3y1z2*2400.+c(5,5,3,n)*x3y1z3*2400.+c(5,5,4,
     & n)*x3y1z4*2400.+c(5,5,5,n)*x3y1z5*2400.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.2.and.dy.eq.4.and.dz.eq.1 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y1z4=x1y1z4*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3z4=x2z4*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x3y1z4=x2y1z4*x1
      r(i1,i2,i3,n)=(c(2,4,1,n)*48.+c(2,4,2,n)*z1*96.+c(2,4,3,n)*z2*
     & 144.+c(2,4,4,n)*z3*192.+c(2,4,5,n)*z4*240.+c(2,5,1,n)*y1*240.+
     & c(2,5,2,n)*y1z1*480.+c(2,5,3,n)*y1z2*720.+c(2,5,4,n)*y1z3*960.+
     & c(2,5,5,n)*y1z4*1200.+c(3,4,1,n)*x1*144.+c(3,4,2,n)*x1z1*288.+
     & c(3,4,3,n)*x1z2*432.+c(3,4,4,n)*x1z3*576.+c(3,4,5,n)*x1z4*720.+
     & c(3,5,1,n)*x1y1*720.+c(3,5,2,n)*x1y1z1*1440.+c(3,5,3,n)*x1y1z2*
     & 2160.+c(3,5,4,n)*x1y1z3*2880.+c(3,5,5,n)*x1y1z4*3600.+c(4,4,1,
     & n)*x2*288.+c(4,4,2,n)*x2z1*576.+c(4,4,3,n)*x2z2*864.+c(4,4,4,n)
     & *x2z3*1152.+c(4,4,5,n)*x2z4*1440.+c(4,5,1,n)*x2y1*1440.+c(4,5,
     & 2,n)*x2y1z1*2880.+c(4,5,3,n)*x2y1z2*4320.+c(4,5,4,n)*x2y1z3*
     & 5760.+c(4,5,5,n)*x2y1z4*7200.+c(5,4,1,n)*x3*480.+c(5,4,2,n)*
     & x3z1*960.+c(5,4,3,n)*x3z2*1440.+c(5,4,4,n)*x3z3*1920.+c(5,4,5,
     & n)*x3z4*2400.+c(5,5,1,n)*x3y1*2400.+c(5,5,2,n)*x3y1z1*4800.+c(
     & 5,5,3,n)*x3y1z2*7200.+c(5,5,4,n)*x3y1z3*9600.+c(5,5,5,n)*
     & x3y1z4*12000.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.2.and.dy.eq.4.and.dz.eq.2 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      r(i1,i2,i3,n)=(c(2,4,2,n)*96.+c(2,4,3,n)*z1*288.+c(2,4,4,n)*z2*
     & 576.+c(2,4,5,n)*z3*960.+c(2,5,2,n)*y1*480.+c(2,5,3,n)*y1z1*
     & 1440.+c(2,5,4,n)*y1z2*2880.+c(2,5,5,n)*y1z3*4800.+c(3,4,2,n)*
     & x1*288.+c(3,4,3,n)*x1z1*864.+c(3,4,4,n)*x1z2*1728.+c(3,4,5,n)*
     & x1z3*2880.+c(3,5,2,n)*x1y1*1440.+c(3,5,3,n)*x1y1z1*4320.+c(3,5,
     & 4,n)*x1y1z2*8640.+c(3,5,5,n)*x1y1z3*14400.+c(4,4,2,n)*x2*576.+
     & c(4,4,3,n)*x2z1*1728.+c(4,4,4,n)*x2z2*3456.+c(4,4,5,n)*x2z3*
     & 5760.+c(4,5,2,n)*x2y1*2880.+c(4,5,3,n)*x2y1z1*8640.+c(4,5,4,n)*
     & x2y1z2*17280.+c(4,5,5,n)*x2y1z3*28800.+c(5,4,2,n)*x3*960.+c(5,
     & 4,3,n)*x3z1*2880.+c(5,4,4,n)*x3z2*5760.+c(5,4,5,n)*x3z3*9600.+
     & c(5,5,2,n)*x3y1*4800.+c(5,5,3,n)*x3y1z1*14400.+c(5,5,4,n)*
     & x3y1z2*28800.+c(5,5,5,n)*x3y1z3*48000.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.2.and.dy.eq.4.and.dz.eq.3 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      r(i1,i2,i3,n)=(c(2,4,3,n)*288.+c(2,4,4,n)*z1*1152.+c(2,4,5,n)*z2*
     & 2880.+c(2,5,3,n)*y1*1440.+c(2,5,4,n)*y1z1*5760.+c(2,5,5,n)*
     & y1z2*14400.+c(3,4,3,n)*x1*864.+c(3,4,4,n)*x1z1*3456.+c(3,4,5,n)
     & *x1z2*8640.+c(3,5,3,n)*x1y1*4320.+c(3,5,4,n)*x1y1z1*17280.+c(3,
     & 5,5,n)*x1y1z2*43200.+c(4,4,3,n)*x2*1728.+c(4,4,4,n)*x2z1*6912.+
     & c(4,4,5,n)*x2z2*17280.+c(4,5,3,n)*x2y1*8640.+c(4,5,4,n)*x2y1z1*
     & 34560.+c(4,5,5,n)*x2y1z2*86400.+c(5,4,3,n)*x3*2880.+c(5,4,4,n)*
     & x3z1*11520.+c(5,4,5,n)*x3z2*28800.+c(5,5,3,n)*x3y1*14400.+c(5,
     & 5,4,n)*x3y1z1*57600.+c(5,5,5,n)*x3y1z2*144000.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.2.and.dy.eq.4.and.dz.eq.4 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x2=x1*x1
      x2z1=x1z1*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      r(i1,i2,i3,n)=(c(2,4,4,n)*1152.+c(2,4,5,n)*z1*5760.+c(2,5,4,n)*
     & y1*5760.+c(2,5,5,n)*y1z1*28800.+c(3,4,4,n)*x1*3456.+c(3,4,5,n)*
     & x1z1*17280.+c(3,5,4,n)*x1y1*17280.+c(3,5,5,n)*x1y1z1*86400.+c(
     & 4,4,4,n)*x2*6912.+c(4,4,5,n)*x2z1*34560.+c(4,5,4,n)*x2y1*
     & 34560.+c(4,5,5,n)*x2y1z1*172800.+c(5,4,4,n)*x3*11520.+c(5,4,5,
     & n)*x3z1*57600.+c(5,5,4,n)*x3y1*57600.+c(5,5,5,n)*x3y1z1*
     & 288000.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.3.and.dy.eq.0.and.dz.eq.0 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y1z5=y1z4*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
      y2z5=y1z5*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      y3z4=y2z4*y1
      y3z5=y2z5*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y4z2=y3z2*y1
      y4z3=y3z3*y1
      y4z4=y3z4*y1
      y4z5=y3z5*y1
      y5=y4*y1
      y5z1=y4z1*y1
      y5z2=y4z2*y1
      y5z3=y4z3*y1
      y5z4=y4z4*y1
      y5z5=y4z5*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1z5=x1z4*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y1z5=x1y1z4*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y2z4=x1y1z4*y1
      x1y2z5=x1y1z5*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x1y3z4=x1y2z4*y1
      x1y3z5=x1y2z5*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y4z2=x1y3z2*y1
      x1y4z3=x1y3z3*y1
      x1y4z4=x1y3z4*y1
      x1y4z5=x1y3z5*y1
      x1y5=x1y4*y1
      x1y5z1=x1y4z1*y1
      x1y5z2=x1y4z2*y1
      x1y5z3=x1y4z3*y1
      x1y5z4=x1y4z4*y1
      x1y5z5=x1y4z5*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x2z5=x1z5*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y1z4=x1y1z4*x1
      x2y1z5=x1y1z5*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y2z4=x1y2z4*x1
      x2y2z5=x1y2z5*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y3z3=x1y3z3*x1
      x2y3z4=x1y3z4*x1
      x2y3z5=x1y3z5*x1
      x2y4=x1y4*x1
      x2y4z1=x1y4z1*x1
      x2y4z2=x1y4z2*x1
      x2y4z3=x1y4z3*x1
      x2y4z4=x1y4z4*x1
      x2y4z5=x1y4z5*x1
      x2y5=x1y5*x1
      x2y5z1=x1y5z1*x1
      x2y5z2=x1y5z2*x1
      x2y5z3=x1y5z3*x1
      x2y5z4=x1y5z4*x1
      x2y5z5=x1y5z5*x1
      r(i1,i2,i3,n)=(c(3,0,0,n)*6.+c(3,0,1,n)*z1*6.+c(3,0,2,n)*z2*6.+c(
     & 3,0,3,n)*z3*6.+c(3,0,4,n)*z4*6.+c(3,0,5,n)*z5*6.+c(3,1,0,n)*y1*
     & 6.+c(3,1,1,n)*y1z1*6.+c(3,1,2,n)*y1z2*6.+c(3,1,3,n)*y1z3*6.+c(
     & 3,1,4,n)*y1z4*6.+c(3,1,5,n)*y1z5*6.+c(3,2,0,n)*y2*6.+c(3,2,1,n)
     & *y2z1*6.+c(3,2,2,n)*y2z2*6.+c(3,2,3,n)*y2z3*6.+c(3,2,4,n)*y2z4*
     & 6.+c(3,2,5,n)*y2z5*6.+c(3,3,0,n)*y3*6.+c(3,3,1,n)*y3z1*6.+c(3,
     & 3,2,n)*y3z2*6.+c(3,3,3,n)*y3z3*6.+c(3,3,4,n)*y3z4*6.+c(3,3,5,n)
     & *y3z5*6.+c(3,4,0,n)*y4*6.+c(3,4,1,n)*y4z1*6.+c(3,4,2,n)*y4z2*
     & 6.+c(3,4,3,n)*y4z3*6.+c(3,4,4,n)*y4z4*6.+c(3,4,5,n)*y4z5*6.+c(
     & 3,5,0,n)*y5*6.+c(3,5,1,n)*y5z1*6.+c(3,5,2,n)*y5z2*6.+c(3,5,3,n)
     & *y5z3*6.+c(3,5,4,n)*y5z4*6.+c(3,5,5,n)*y5z5*6.+c(4,0,0,n)*x1*
     & 24.+c(4,0,1,n)*x1z1*24.+c(4,0,2,n)*x1z2*24.+c(4,0,3,n)*x1z3*
     & 24.+c(4,0,4,n)*x1z4*24.+c(4,0,5,n)*x1z5*24.+c(4,1,0,n)*x1y1*
     & 24.+c(4,1,1,n)*x1y1z1*24.+c(4,1,2,n)*x1y1z2*24.+c(4,1,3,n)*
     & x1y1z3*24.+c(4,1,4,n)*x1y1z4*24.+c(4,1,5,n)*x1y1z5*24.+c(4,2,0,
     & n)*x1y2*24.+c(4,2,1,n)*x1y2z1*24.+c(4,2,2,n)*x1y2z2*24.+c(4,2,
     & 3,n)*x1y2z3*24.+c(4,2,4,n)*x1y2z4*24.+c(4,2,5,n)*x1y2z5*24.+c(
     & 4,3,0,n)*x1y3*24.+c(4,3,1,n)*x1y3z1*24.+c(4,3,2,n)*x1y3z2*24.+
     & c(4,3,3,n)*x1y3z3*24.+c(4,3,4,n)*x1y3z4*24.+c(4,3,5,n)*x1y3z5*
     & 24.+c(4,4,0,n)*x1y4*24.+c(4,4,1,n)*x1y4z1*24.+c(4,4,2,n)*
     & x1y4z2*24.+c(4,4,3,n)*x1y4z3*24.+c(4,4,4,n)*x1y4z4*24.+c(4,4,5,
     & n)*x1y4z5*24.+c(4,5,0,n)*x1y5*24.+c(4,5,1,n)*x1y5z1*24.+c(4,5,
     & 2,n)*x1y5z2*24.+c(4,5,3,n)*x1y5z3*24.+c(4,5,4,n)*x1y5z4*24.+c(
     & 4,5,5,n)*x1y5z5*24.+c(5,0,0,n)*x2*60.+c(5,0,1,n)*x2z1*60.+c(5,
     & 0,2,n)*x2z2*60.+c(5,0,3,n)*x2z3*60.+c(5,0,4,n)*x2z4*60.+c(5,0,
     & 5,n)*x2z5*60.+c(5,1,0,n)*x2y1*60.+c(5,1,1,n)*x2y1z1*60.+c(5,1,
     & 2,n)*x2y1z2*60.+c(5,1,3,n)*x2y1z3*60.+c(5,1,4,n)*x2y1z4*60.+c(
     & 5,1,5,n)*x2y1z5*60.+c(5,2,0,n)*x2y2*60.+c(5,2,1,n)*x2y2z1*60.+
     & c(5,2,2,n)*x2y2z2*60.+c(5,2,3,n)*x2y2z3*60.+c(5,2,4,n)*x2y2z4*
     & 60.+c(5,2,5,n)*x2y2z5*60.+c(5,3,0,n)*x2y3*60.+c(5,3,1,n)*
     & x2y3z1*60.+c(5,3,2,n)*x2y3z2*60.+c(5,3,3,n)*x2y3z3*60.+c(5,3,4,
     & n)*x2y3z4*60.+c(5,3,5,n)*x2y3z5*60.+c(5,4,0,n)*x2y4*60.+c(5,4,
     & 1,n)*x2y4z1*60.+c(5,4,2,n)*x2y4z2*60.+c(5,4,3,n)*x2y4z3*60.+c(
     & 5,4,4,n)*x2y4z4*60.+c(5,4,5,n)*x2y4z5*60.+c(5,5,0,n)*x2y5*60.+
     & c(5,5,1,n)*x2y5z1*60.+c(5,5,2,n)*x2y5z2*60.+c(5,5,3,n)*x2y5z3*
     & 60.+c(5,5,4,n)*x2y5z4*60.+c(5,5,5,n)*x2y5z5*60.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.3.and.dy.eq.0.and.dz.eq.1 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      y3z4=y2z4*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y4z2=y3z2*y1
      y4z3=y3z3*y1
      y4z4=y3z4*y1
      y5=y4*y1
      y5z1=y4z1*y1
      y5z2=y4z2*y1
      y5z3=y4z3*y1
      y5z4=y4z4*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y2z4=x1y1z4*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x1y3z4=x1y2z4*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y4z2=x1y3z2*y1
      x1y4z3=x1y3z3*y1
      x1y4z4=x1y3z4*y1
      x1y5=x1y4*y1
      x1y5z1=x1y4z1*y1
      x1y5z2=x1y4z2*y1
      x1y5z3=x1y4z3*y1
      x1y5z4=x1y4z4*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y1z4=x1y1z4*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y2z4=x1y2z4*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y3z3=x1y3z3*x1
      x2y3z4=x1y3z4*x1
      x2y4=x1y4*x1
      x2y4z1=x1y4z1*x1
      x2y4z2=x1y4z2*x1
      x2y4z3=x1y4z3*x1
      x2y4z4=x1y4z4*x1
      x2y5=x1y5*x1
      x2y5z1=x1y5z1*x1
      x2y5z2=x1y5z2*x1
      x2y5z3=x1y5z3*x1
      x2y5z4=x1y5z4*x1
      r(i1,i2,i3,n)=(c(3,0,1,n)*6.+c(3,0,2,n)*z1*12.+c(3,0,3,n)*z2*18.+
     & c(3,0,4,n)*z3*24.+c(3,0,5,n)*z4*30.+c(3,1,1,n)*y1*6.+c(3,1,2,n)
     & *y1z1*12.+c(3,1,3,n)*y1z2*18.+c(3,1,4,n)*y1z3*24.+c(3,1,5,n)*
     & y1z4*30.+c(3,2,1,n)*y2*6.+c(3,2,2,n)*y2z1*12.+c(3,2,3,n)*y2z2*
     & 18.+c(3,2,4,n)*y2z3*24.+c(3,2,5,n)*y2z4*30.+c(3,3,1,n)*y3*6.+c(
     & 3,3,2,n)*y3z1*12.+c(3,3,3,n)*y3z2*18.+c(3,3,4,n)*y3z3*24.+c(3,
     & 3,5,n)*y3z4*30.+c(3,4,1,n)*y4*6.+c(3,4,2,n)*y4z1*12.+c(3,4,3,n)
     & *y4z2*18.+c(3,4,4,n)*y4z3*24.+c(3,4,5,n)*y4z4*30.+c(3,5,1,n)*
     & y5*6.+c(3,5,2,n)*y5z1*12.+c(3,5,3,n)*y5z2*18.+c(3,5,4,n)*y5z3*
     & 24.+c(3,5,5,n)*y5z4*30.+c(4,0,1,n)*x1*24.+c(4,0,2,n)*x1z1*48.+
     & c(4,0,3,n)*x1z2*72.+c(4,0,4,n)*x1z3*96.+c(4,0,5,n)*x1z4*120.+c(
     & 4,1,1,n)*x1y1*24.+c(4,1,2,n)*x1y1z1*48.+c(4,1,3,n)*x1y1z2*72.+
     & c(4,1,4,n)*x1y1z3*96.+c(4,1,5,n)*x1y1z4*120.+c(4,2,1,n)*x1y2*
     & 24.+c(4,2,2,n)*x1y2z1*48.+c(4,2,3,n)*x1y2z2*72.+c(4,2,4,n)*
     & x1y2z3*96.+c(4,2,5,n)*x1y2z4*120.+c(4,3,1,n)*x1y3*24.+c(4,3,2,
     & n)*x1y3z1*48.+c(4,3,3,n)*x1y3z2*72.+c(4,3,4,n)*x1y3z3*96.+c(4,
     & 3,5,n)*x1y3z4*120.+c(4,4,1,n)*x1y4*24.+c(4,4,2,n)*x1y4z1*48.+c(
     & 4,4,3,n)*x1y4z2*72.+c(4,4,4,n)*x1y4z3*96.+c(4,4,5,n)*x1y4z4*
     & 120.+c(4,5,1,n)*x1y5*24.+c(4,5,2,n)*x1y5z1*48.+c(4,5,3,n)*
     & x1y5z2*72.+c(4,5,4,n)*x1y5z3*96.+c(4,5,5,n)*x1y5z4*120.+c(5,0,
     & 1,n)*x2*60.+c(5,0,2,n)*x2z1*120.+c(5,0,3,n)*x2z2*180.+c(5,0,4,
     & n)*x2z3*240.+c(5,0,5,n)*x2z4*300.+c(5,1,1,n)*x2y1*60.+c(5,1,2,
     & n)*x2y1z1*120.+c(5,1,3,n)*x2y1z2*180.+c(5,1,4,n)*x2y1z3*240.+c(
     & 5,1,5,n)*x2y1z4*300.+c(5,2,1,n)*x2y2*60.+c(5,2,2,n)*x2y2z1*
     & 120.+c(5,2,3,n)*x2y2z2*180.+c(5,2,4,n)*x2y2z3*240.+c(5,2,5,n)*
     & x2y2z4*300.+c(5,3,1,n)*x2y3*60.+c(5,3,2,n)*x2y3z1*120.+c(5,3,3,
     & n)*x2y3z2*180.+c(5,3,4,n)*x2y3z3*240.+c(5,3,5,n)*x2y3z4*300.+c(
     & 5,4,1,n)*x2y4*60.+c(5,4,2,n)*x2y4z1*120.+c(5,4,3,n)*x2y4z2*
     & 180.+c(5,4,4,n)*x2y4z3*240.+c(5,4,5,n)*x2y4z4*300.+c(5,5,1,n)*
     & x2y5*60.+c(5,5,2,n)*x2y5z1*120.+c(5,5,3,n)*x2y5z2*180.+c(5,5,4,
     & n)*x2y5z3*240.+c(5,5,5,n)*x2y5z4*300.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.3.and.dy.eq.0.and.dz.eq.2 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y4z2=y3z2*y1
      y4z3=y3z3*y1
      y5=y4*y1
      y5z1=y4z1*y1
      y5z2=y4z2*y1
      y5z3=y4z3*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y4z2=x1y3z2*y1
      x1y4z3=x1y3z3*y1
      x1y5=x1y4*y1
      x1y5z1=x1y4z1*y1
      x1y5z2=x1y4z2*y1
      x1y5z3=x1y4z3*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y3z3=x1y3z3*x1
      x2y4=x1y4*x1
      x2y4z1=x1y4z1*x1
      x2y4z2=x1y4z2*x1
      x2y4z3=x1y4z3*x1
      x2y5=x1y5*x1
      x2y5z1=x1y5z1*x1
      x2y5z2=x1y5z2*x1
      x2y5z3=x1y5z3*x1
      r(i1,i2,i3,n)=(c(3,0,2,n)*12.+c(3,0,3,n)*z1*36.+c(3,0,4,n)*z2*
     & 72.+c(3,0,5,n)*z3*120.+c(3,1,2,n)*y1*12.+c(3,1,3,n)*y1z1*36.+c(
     & 3,1,4,n)*y1z2*72.+c(3,1,5,n)*y1z3*120.+c(3,2,2,n)*y2*12.+c(3,2,
     & 3,n)*y2z1*36.+c(3,2,4,n)*y2z2*72.+c(3,2,5,n)*y2z3*120.+c(3,3,2,
     & n)*y3*12.+c(3,3,3,n)*y3z1*36.+c(3,3,4,n)*y3z2*72.+c(3,3,5,n)*
     & y3z3*120.+c(3,4,2,n)*y4*12.+c(3,4,3,n)*y4z1*36.+c(3,4,4,n)*
     & y4z2*72.+c(3,4,5,n)*y4z3*120.+c(3,5,2,n)*y5*12.+c(3,5,3,n)*
     & y5z1*36.+c(3,5,4,n)*y5z2*72.+c(3,5,5,n)*y5z3*120.+c(4,0,2,n)*
     & x1*48.+c(4,0,3,n)*x1z1*144.+c(4,0,4,n)*x1z2*288.+c(4,0,5,n)*
     & x1z3*480.+c(4,1,2,n)*x1y1*48.+c(4,1,3,n)*x1y1z1*144.+c(4,1,4,n)
     & *x1y1z2*288.+c(4,1,5,n)*x1y1z3*480.+c(4,2,2,n)*x1y2*48.+c(4,2,
     & 3,n)*x1y2z1*144.+c(4,2,4,n)*x1y2z2*288.+c(4,2,5,n)*x1y2z3*480.+
     & c(4,3,2,n)*x1y3*48.+c(4,3,3,n)*x1y3z1*144.+c(4,3,4,n)*x1y3z2*
     & 288.+c(4,3,5,n)*x1y3z3*480.+c(4,4,2,n)*x1y4*48.+c(4,4,3,n)*
     & x1y4z1*144.+c(4,4,4,n)*x1y4z2*288.+c(4,4,5,n)*x1y4z3*480.+c(4,
     & 5,2,n)*x1y5*48.+c(4,5,3,n)*x1y5z1*144.+c(4,5,4,n)*x1y5z2*288.+
     & c(4,5,5,n)*x1y5z3*480.+c(5,0,2,n)*x2*120.+c(5,0,3,n)*x2z1*360.+
     & c(5,0,4,n)*x2z2*720.+c(5,0,5,n)*x2z3*1200.+c(5,1,2,n)*x2y1*
     & 120.+c(5,1,3,n)*x2y1z1*360.+c(5,1,4,n)*x2y1z2*720.+c(5,1,5,n)*
     & x2y1z3*1200.+c(5,2,2,n)*x2y2*120.+c(5,2,3,n)*x2y2z1*360.+c(5,2,
     & 4,n)*x2y2z2*720.+c(5,2,5,n)*x2y2z3*1200.+c(5,3,2,n)*x2y3*120.+
     & c(5,3,3,n)*x2y3z1*360.+c(5,3,4,n)*x2y3z2*720.+c(5,3,5,n)*
     & x2y3z3*1200.+c(5,4,2,n)*x2y4*120.+c(5,4,3,n)*x2y4z1*360.+c(5,4,
     & 4,n)*x2y4z2*720.+c(5,4,5,n)*x2y4z3*1200.+c(5,5,2,n)*x2y5*120.+
     & c(5,5,3,n)*x2y5z1*360.+c(5,5,4,n)*x2y5z2*720.+c(5,5,5,n)*
     & x2y5z3*1200.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.3.and.dy.eq.0.and.dz.eq.3 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y4z2=y3z2*y1
      y5=y4*y1
      y5z1=y4z1*y1
      y5z2=y4z2*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y4z2=x1y3z2*y1
      x1y5=x1y4*y1
      x1y5z1=x1y4z1*y1
      x1y5z2=x1y4z2*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y4=x1y4*x1
      x2y4z1=x1y4z1*x1
      x2y4z2=x1y4z2*x1
      x2y5=x1y5*x1
      x2y5z1=x1y5z1*x1
      x2y5z2=x1y5z2*x1
      r(i1,i2,i3,n)=(c(3,0,3,n)*36.+c(3,0,4,n)*z1*144.+c(3,0,5,n)*z2*
     & 360.+c(3,1,3,n)*y1*36.+c(3,1,4,n)*y1z1*144.+c(3,1,5,n)*y1z2*
     & 360.+c(3,2,3,n)*y2*36.+c(3,2,4,n)*y2z1*144.+c(3,2,5,n)*y2z2*
     & 360.+c(3,3,3,n)*y3*36.+c(3,3,4,n)*y3z1*144.+c(3,3,5,n)*y3z2*
     & 360.+c(3,4,3,n)*y4*36.+c(3,4,4,n)*y4z1*144.+c(3,4,5,n)*y4z2*
     & 360.+c(3,5,3,n)*y5*36.+c(3,5,4,n)*y5z1*144.+c(3,5,5,n)*y5z2*
     & 360.+c(4,0,3,n)*x1*144.+c(4,0,4,n)*x1z1*576.+c(4,0,5,n)*x1z2*
     & 1440.+c(4,1,3,n)*x1y1*144.+c(4,1,4,n)*x1y1z1*576.+c(4,1,5,n)*
     & x1y1z2*1440.+c(4,2,3,n)*x1y2*144.+c(4,2,4,n)*x1y2z1*576.+c(4,2,
     & 5,n)*x1y2z2*1440.+c(4,3,3,n)*x1y3*144.+c(4,3,4,n)*x1y3z1*576.+
     & c(4,3,5,n)*x1y3z2*1440.+c(4,4,3,n)*x1y4*144.+c(4,4,4,n)*x1y4z1*
     & 576.+c(4,4,5,n)*x1y4z2*1440.+c(4,5,3,n)*x1y5*144.+c(4,5,4,n)*
     & x1y5z1*576.+c(4,5,5,n)*x1y5z2*1440.+c(5,0,3,n)*x2*360.+c(5,0,4,
     & n)*x2z1*1440.+c(5,0,5,n)*x2z2*3600.+c(5,1,3,n)*x2y1*360.+c(5,1,
     & 4,n)*x2y1z1*1440.+c(5,1,5,n)*x2y1z2*3600.+c(5,2,3,n)*x2y2*360.+
     & c(5,2,4,n)*x2y2z1*1440.+c(5,2,5,n)*x2y2z2*3600.+c(5,3,3,n)*
     & x2y3*360.+c(5,3,4,n)*x2y3z1*1440.+c(5,3,5,n)*x2y3z2*3600.+c(5,
     & 4,3,n)*x2y4*360.+c(5,4,4,n)*x2y4z1*1440.+c(5,4,5,n)*x2y4z2*
     & 3600.+c(5,5,3,n)*x2y5*360.+c(5,5,4,n)*x2y5z1*1440.+c(5,5,5,n)*
     & x2y5z2*3600.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.3.and.dy.eq.0.and.dz.eq.4 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y5=y4*y1
      y5z1=y4z1*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y5=x1y4*y1
      x1y5z1=x1y4z1*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y4=x1y4*x1
      x2y4z1=x1y4z1*x1
      x2y5=x1y5*x1
      x2y5z1=x1y5z1*x1
      r(i1,i2,i3,n)=(c(3,0,4,n)*144.+c(3,0,5,n)*z1*720.+c(3,1,4,n)*y1*
     & 144.+c(3,1,5,n)*y1z1*720.+c(3,2,4,n)*y2*144.+c(3,2,5,n)*y2z1*
     & 720.+c(3,3,4,n)*y3*144.+c(3,3,5,n)*y3z1*720.+c(3,4,4,n)*y4*
     & 144.+c(3,4,5,n)*y4z1*720.+c(3,5,4,n)*y5*144.+c(3,5,5,n)*y5z1*
     & 720.+c(4,0,4,n)*x1*576.+c(4,0,5,n)*x1z1*2880.+c(4,1,4,n)*x1y1*
     & 576.+c(4,1,5,n)*x1y1z1*2880.+c(4,2,4,n)*x1y2*576.+c(4,2,5,n)*
     & x1y2z1*2880.+c(4,3,4,n)*x1y3*576.+c(4,3,5,n)*x1y3z1*2880.+c(4,
     & 4,4,n)*x1y4*576.+c(4,4,5,n)*x1y4z1*2880.+c(4,5,4,n)*x1y5*576.+
     & c(4,5,5,n)*x1y5z1*2880.+c(5,0,4,n)*x2*1440.+c(5,0,5,n)*x2z1*
     & 7200.+c(5,1,4,n)*x2y1*1440.+c(5,1,5,n)*x2y1z1*7200.+c(5,2,4,n)*
     & x2y2*1440.+c(5,2,5,n)*x2y2z1*7200.+c(5,3,4,n)*x2y3*1440.+c(5,3,
     & 5,n)*x2y3z1*7200.+c(5,4,4,n)*x2y4*1440.+c(5,4,5,n)*x2y4z1*
     & 7200.+c(5,5,4,n)*x2y5*1440.+c(5,5,5,n)*x2y5z1*7200.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.3.and.dy.eq.1.and.dz.eq.0 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y1z5=y1z4*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
      y2z5=y1z5*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      y3z4=y2z4*y1
      y3z5=y2z5*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y4z2=y3z2*y1
      y4z3=y3z3*y1
      y4z4=y3z4*y1
      y4z5=y3z5*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1z5=x1z4*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y1z5=x1y1z4*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y2z4=x1y1z4*y1
      x1y2z5=x1y1z5*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x1y3z4=x1y2z4*y1
      x1y3z5=x1y2z5*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y4z2=x1y3z2*y1
      x1y4z3=x1y3z3*y1
      x1y4z4=x1y3z4*y1
      x1y4z5=x1y3z5*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x2z5=x1z5*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y1z4=x1y1z4*x1
      x2y1z5=x1y1z5*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y2z4=x1y2z4*x1
      x2y2z5=x1y2z5*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y3z3=x1y3z3*x1
      x2y3z4=x1y3z4*x1
      x2y3z5=x1y3z5*x1
      x2y4=x1y4*x1
      x2y4z1=x1y4z1*x1
      x2y4z2=x1y4z2*x1
      x2y4z3=x1y4z3*x1
      x2y4z4=x1y4z4*x1
      x2y4z5=x1y4z5*x1
      r(i1,i2,i3,n)=(c(3,1,0,n)*6.+c(3,1,1,n)*z1*6.+c(3,1,2,n)*z2*6.+c(
     & 3,1,3,n)*z3*6.+c(3,1,4,n)*z4*6.+c(3,1,5,n)*z5*6.+c(3,2,0,n)*y1*
     & 12.+c(3,2,1,n)*y1z1*12.+c(3,2,2,n)*y1z2*12.+c(3,2,3,n)*y1z3*
     & 12.+c(3,2,4,n)*y1z4*12.+c(3,2,5,n)*y1z5*12.+c(3,3,0,n)*y2*18.+
     & c(3,3,1,n)*y2z1*18.+c(3,3,2,n)*y2z2*18.+c(3,3,3,n)*y2z3*18.+c(
     & 3,3,4,n)*y2z4*18.+c(3,3,5,n)*y2z5*18.+c(3,4,0,n)*y3*24.+c(3,4,
     & 1,n)*y3z1*24.+c(3,4,2,n)*y3z2*24.+c(3,4,3,n)*y3z3*24.+c(3,4,4,
     & n)*y3z4*24.+c(3,4,5,n)*y3z5*24.+c(3,5,0,n)*y4*30.+c(3,5,1,n)*
     & y4z1*30.+c(3,5,2,n)*y4z2*30.+c(3,5,3,n)*y4z3*30.+c(3,5,4,n)*
     & y4z4*30.+c(3,5,5,n)*y4z5*30.+c(4,1,0,n)*x1*24.+c(4,1,1,n)*x1z1*
     & 24.+c(4,1,2,n)*x1z2*24.+c(4,1,3,n)*x1z3*24.+c(4,1,4,n)*x1z4*
     & 24.+c(4,1,5,n)*x1z5*24.+c(4,2,0,n)*x1y1*48.+c(4,2,1,n)*x1y1z1*
     & 48.+c(4,2,2,n)*x1y1z2*48.+c(4,2,3,n)*x1y1z3*48.+c(4,2,4,n)*
     & x1y1z4*48.+c(4,2,5,n)*x1y1z5*48.+c(4,3,0,n)*x1y2*72.+c(4,3,1,n)
     & *x1y2z1*72.+c(4,3,2,n)*x1y2z2*72.+c(4,3,3,n)*x1y2z3*72.+c(4,3,
     & 4,n)*x1y2z4*72.+c(4,3,5,n)*x1y2z5*72.+c(4,4,0,n)*x1y3*96.+c(4,
     & 4,1,n)*x1y3z1*96.+c(4,4,2,n)*x1y3z2*96.+c(4,4,3,n)*x1y3z3*96.+
     & c(4,4,4,n)*x1y3z4*96.+c(4,4,5,n)*x1y3z5*96.+c(4,5,0,n)*x1y4*
     & 120.+c(4,5,1,n)*x1y4z1*120.+c(4,5,2,n)*x1y4z2*120.+c(4,5,3,n)*
     & x1y4z3*120.+c(4,5,4,n)*x1y4z4*120.+c(4,5,5,n)*x1y4z5*120.+c(5,
     & 1,0,n)*x2*60.+c(5,1,1,n)*x2z1*60.+c(5,1,2,n)*x2z2*60.+c(5,1,3,
     & n)*x2z3*60.+c(5,1,4,n)*x2z4*60.+c(5,1,5,n)*x2z5*60.+c(5,2,0,n)*
     & x2y1*120.+c(5,2,1,n)*x2y1z1*120.+c(5,2,2,n)*x2y1z2*120.+c(5,2,
     & 3,n)*x2y1z3*120.+c(5,2,4,n)*x2y1z4*120.+c(5,2,5,n)*x2y1z5*120.+
     & c(5,3,0,n)*x2y2*180.+c(5,3,1,n)*x2y2z1*180.+c(5,3,2,n)*x2y2z2*
     & 180.+c(5,3,3,n)*x2y2z3*180.+c(5,3,4,n)*x2y2z4*180.+c(5,3,5,n)*
     & x2y2z5*180.+c(5,4,0,n)*x2y3*240.+c(5,4,1,n)*x2y3z1*240.+c(5,4,
     & 2,n)*x2y3z2*240.+c(5,4,3,n)*x2y3z3*240.+c(5,4,4,n)*x2y3z4*240.+
     & c(5,4,5,n)*x2y3z5*240.+c(5,5,0,n)*x2y4*300.+c(5,5,1,n)*x2y4z1*
     & 300.+c(5,5,2,n)*x2y4z2*300.+c(5,5,3,n)*x2y4z3*300.+c(5,5,4,n)*
     & x2y4z4*300.+c(5,5,5,n)*x2y4z5*300.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.3.and.dy.eq.1.and.dz.eq.1 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      y3z4=y2z4*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y4z2=y3z2*y1
      y4z3=y3z3*y1
      y4z4=y3z4*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y2z4=x1y1z4*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x1y3z4=x1y2z4*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y4z2=x1y3z2*y1
      x1y4z3=x1y3z3*y1
      x1y4z4=x1y3z4*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y1z4=x1y1z4*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y2z4=x1y2z4*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y3z3=x1y3z3*x1
      x2y3z4=x1y3z4*x1
      x2y4=x1y4*x1
      x2y4z1=x1y4z1*x1
      x2y4z2=x1y4z2*x1
      x2y4z3=x1y4z3*x1
      x2y4z4=x1y4z4*x1
      r(i1,i2,i3,n)=(c(3,1,1,n)*6.+c(3,1,2,n)*z1*12.+c(3,1,3,n)*z2*18.+
     & c(3,1,4,n)*z3*24.+c(3,1,5,n)*z4*30.+c(3,2,1,n)*y1*12.+c(3,2,2,
     & n)*y1z1*24.+c(3,2,3,n)*y1z2*36.+c(3,2,4,n)*y1z3*48.+c(3,2,5,n)*
     & y1z4*60.+c(3,3,1,n)*y2*18.+c(3,3,2,n)*y2z1*36.+c(3,3,3,n)*y2z2*
     & 54.+c(3,3,4,n)*y2z3*72.+c(3,3,5,n)*y2z4*90.+c(3,4,1,n)*y3*24.+
     & c(3,4,2,n)*y3z1*48.+c(3,4,3,n)*y3z2*72.+c(3,4,4,n)*y3z3*96.+c(
     & 3,4,5,n)*y3z4*120.+c(3,5,1,n)*y4*30.+c(3,5,2,n)*y4z1*60.+c(3,5,
     & 3,n)*y4z2*90.+c(3,5,4,n)*y4z3*120.+c(3,5,5,n)*y4z4*150.+c(4,1,
     & 1,n)*x1*24.+c(4,1,2,n)*x1z1*48.+c(4,1,3,n)*x1z2*72.+c(4,1,4,n)*
     & x1z3*96.+c(4,1,5,n)*x1z4*120.+c(4,2,1,n)*x1y1*48.+c(4,2,2,n)*
     & x1y1z1*96.+c(4,2,3,n)*x1y1z2*144.+c(4,2,4,n)*x1y1z3*192.+c(4,2,
     & 5,n)*x1y1z4*240.+c(4,3,1,n)*x1y2*72.+c(4,3,2,n)*x1y2z1*144.+c(
     & 4,3,3,n)*x1y2z2*216.+c(4,3,4,n)*x1y2z3*288.+c(4,3,5,n)*x1y2z4*
     & 360.+c(4,4,1,n)*x1y3*96.+c(4,4,2,n)*x1y3z1*192.+c(4,4,3,n)*
     & x1y3z2*288.+c(4,4,4,n)*x1y3z3*384.+c(4,4,5,n)*x1y3z4*480.+c(4,
     & 5,1,n)*x1y4*120.+c(4,5,2,n)*x1y4z1*240.+c(4,5,3,n)*x1y4z2*360.+
     & c(4,5,4,n)*x1y4z3*480.+c(4,5,5,n)*x1y4z4*600.+c(5,1,1,n)*x2*
     & 60.+c(5,1,2,n)*x2z1*120.+c(5,1,3,n)*x2z2*180.+c(5,1,4,n)*x2z3*
     & 240.+c(5,1,5,n)*x2z4*300.+c(5,2,1,n)*x2y1*120.+c(5,2,2,n)*
     & x2y1z1*240.+c(5,2,3,n)*x2y1z2*360.+c(5,2,4,n)*x2y1z3*480.+c(5,
     & 2,5,n)*x2y1z4*600.+c(5,3,1,n)*x2y2*180.+c(5,3,2,n)*x2y2z1*360.+
     & c(5,3,3,n)*x2y2z2*540.+c(5,3,4,n)*x2y2z3*720.+c(5,3,5,n)*
     & x2y2z4*900.+c(5,4,1,n)*x2y3*240.+c(5,4,2,n)*x2y3z1*480.+c(5,4,
     & 3,n)*x2y3z2*720.+c(5,4,4,n)*x2y3z3*960.+c(5,4,5,n)*x2y3z4*
     & 1200.+c(5,5,1,n)*x2y4*300.+c(5,5,2,n)*x2y4z1*600.+c(5,5,3,n)*
     & x2y4z2*900.+c(5,5,4,n)*x2y4z3*1200.+c(5,5,5,n)*x2y4z4*1500.)*
     & time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.3.and.dy.eq.1.and.dz.eq.2 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y4z2=y3z2*y1
      y4z3=y3z3*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y4z2=x1y3z2*y1
      x1y4z3=x1y3z3*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y3z3=x1y3z3*x1
      x2y4=x1y4*x1
      x2y4z1=x1y4z1*x1
      x2y4z2=x1y4z2*x1
      x2y4z3=x1y4z3*x1
      r(i1,i2,i3,n)=(c(3,1,2,n)*12.+c(3,1,3,n)*z1*36.+c(3,1,4,n)*z2*
     & 72.+c(3,1,5,n)*z3*120.+c(3,2,2,n)*y1*24.+c(3,2,3,n)*y1z1*72.+c(
     & 3,2,4,n)*y1z2*144.+c(3,2,5,n)*y1z3*240.+c(3,3,2,n)*y2*36.+c(3,
     & 3,3,n)*y2z1*108.+c(3,3,4,n)*y2z2*216.+c(3,3,5,n)*y2z3*360.+c(3,
     & 4,2,n)*y3*48.+c(3,4,3,n)*y3z1*144.+c(3,4,4,n)*y3z2*288.+c(3,4,
     & 5,n)*y3z3*480.+c(3,5,2,n)*y4*60.+c(3,5,3,n)*y4z1*180.+c(3,5,4,
     & n)*y4z2*360.+c(3,5,5,n)*y4z3*600.+c(4,1,2,n)*x1*48.+c(4,1,3,n)*
     & x1z1*144.+c(4,1,4,n)*x1z2*288.+c(4,1,5,n)*x1z3*480.+c(4,2,2,n)*
     & x1y1*96.+c(4,2,3,n)*x1y1z1*288.+c(4,2,4,n)*x1y1z2*576.+c(4,2,5,
     & n)*x1y1z3*960.+c(4,3,2,n)*x1y2*144.+c(4,3,3,n)*x1y2z1*432.+c(4,
     & 3,4,n)*x1y2z2*864.+c(4,3,5,n)*x1y2z3*1440.+c(4,4,2,n)*x1y3*
     & 192.+c(4,4,3,n)*x1y3z1*576.+c(4,4,4,n)*x1y3z2*1152.+c(4,4,5,n)*
     & x1y3z3*1920.+c(4,5,2,n)*x1y4*240.+c(4,5,3,n)*x1y4z1*720.+c(4,5,
     & 4,n)*x1y4z2*1440.+c(4,5,5,n)*x1y4z3*2400.+c(5,1,2,n)*x2*120.+c(
     & 5,1,3,n)*x2z1*360.+c(5,1,4,n)*x2z2*720.+c(5,1,5,n)*x2z3*1200.+
     & c(5,2,2,n)*x2y1*240.+c(5,2,3,n)*x2y1z1*720.+c(5,2,4,n)*x2y1z2*
     & 1440.+c(5,2,5,n)*x2y1z3*2400.+c(5,3,2,n)*x2y2*360.+c(5,3,3,n)*
     & x2y2z1*1080.+c(5,3,4,n)*x2y2z2*2160.+c(5,3,5,n)*x2y2z3*3600.+c(
     & 5,4,2,n)*x2y3*480.+c(5,4,3,n)*x2y3z1*1440.+c(5,4,4,n)*x2y3z2*
     & 2880.+c(5,4,5,n)*x2y3z3*4800.+c(5,5,2,n)*x2y4*600.+c(5,5,3,n)*
     & x2y4z1*1800.+c(5,5,4,n)*x2y4z2*3600.+c(5,5,5,n)*x2y4z3*6000.)*
     & time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.3.and.dy.eq.1.and.dz.eq.3 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y4z2=y3z2*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y4z2=x1y3z2*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y4=x1y4*x1
      x2y4z1=x1y4z1*x1
      x2y4z2=x1y4z2*x1
      r(i1,i2,i3,n)=(c(3,1,3,n)*36.+c(3,1,4,n)*z1*144.+c(3,1,5,n)*z2*
     & 360.+c(3,2,3,n)*y1*72.+c(3,2,4,n)*y1z1*288.+c(3,2,5,n)*y1z2*
     & 720.+c(3,3,3,n)*y2*108.+c(3,3,4,n)*y2z1*432.+c(3,3,5,n)*y2z2*
     & 1080.+c(3,4,3,n)*y3*144.+c(3,4,4,n)*y3z1*576.+c(3,4,5,n)*y3z2*
     & 1440.+c(3,5,3,n)*y4*180.+c(3,5,4,n)*y4z1*720.+c(3,5,5,n)*y4z2*
     & 1800.+c(4,1,3,n)*x1*144.+c(4,1,4,n)*x1z1*576.+c(4,1,5,n)*x1z2*
     & 1440.+c(4,2,3,n)*x1y1*288.+c(4,2,4,n)*x1y1z1*1152.+c(4,2,5,n)*
     & x1y1z2*2880.+c(4,3,3,n)*x1y2*432.+c(4,3,4,n)*x1y2z1*1728.+c(4,
     & 3,5,n)*x1y2z2*4320.+c(4,4,3,n)*x1y3*576.+c(4,4,4,n)*x1y3z1*
     & 2304.+c(4,4,5,n)*x1y3z2*5760.+c(4,5,3,n)*x1y4*720.+c(4,5,4,n)*
     & x1y4z1*2880.+c(4,5,5,n)*x1y4z2*7200.+c(5,1,3,n)*x2*360.+c(5,1,
     & 4,n)*x2z1*1440.+c(5,1,5,n)*x2z2*3600.+c(5,2,3,n)*x2y1*720.+c(5,
     & 2,4,n)*x2y1z1*2880.+c(5,2,5,n)*x2y1z2*7200.+c(5,3,3,n)*x2y2*
     & 1080.+c(5,3,4,n)*x2y2z1*4320.+c(5,3,5,n)*x2y2z2*10800.+c(5,4,3,
     & n)*x2y3*1440.+c(5,4,4,n)*x2y3z1*5760.+c(5,4,5,n)*x2y3z2*14400.+
     & c(5,5,3,n)*x2y4*1800.+c(5,5,4,n)*x2y4z1*7200.+c(5,5,5,n)*
     & x2y4z2*18000.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.3.and.dy.eq.1.and.dz.eq.4 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y4=y3*y1
      y4z1=y3z1*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y4=x1y4*x1
      x2y4z1=x1y4z1*x1
      r(i1,i2,i3,n)=(c(3,1,4,n)*144.+c(3,1,5,n)*z1*720.+c(3,2,4,n)*y1*
     & 288.+c(3,2,5,n)*y1z1*1440.+c(3,3,4,n)*y2*432.+c(3,3,5,n)*y2z1*
     & 2160.+c(3,4,4,n)*y3*576.+c(3,4,5,n)*y3z1*2880.+c(3,5,4,n)*y4*
     & 720.+c(3,5,5,n)*y4z1*3600.+c(4,1,4,n)*x1*576.+c(4,1,5,n)*x1z1*
     & 2880.+c(4,2,4,n)*x1y1*1152.+c(4,2,5,n)*x1y1z1*5760.+c(4,3,4,n)*
     & x1y2*1728.+c(4,3,5,n)*x1y2z1*8640.+c(4,4,4,n)*x1y3*2304.+c(4,4,
     & 5,n)*x1y3z1*11520.+c(4,5,4,n)*x1y4*2880.+c(4,5,5,n)*x1y4z1*
     & 14400.+c(5,1,4,n)*x2*1440.+c(5,1,5,n)*x2z1*7200.+c(5,2,4,n)*
     & x2y1*2880.+c(5,2,5,n)*x2y1z1*14400.+c(5,3,4,n)*x2y2*4320.+c(5,
     & 3,5,n)*x2y2z1*21600.+c(5,4,4,n)*x2y3*5760.+c(5,4,5,n)*x2y3z1*
     & 28800.+c(5,5,4,n)*x2y4*7200.+c(5,5,5,n)*x2y4z1*36000.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.3.and.dy.eq.2.and.dz.eq.0 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y1z5=y1z4*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
      y2z5=y1z5*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      y3z4=y2z4*y1
      y3z5=y2z5*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1z5=x1z4*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y1z5=x1y1z4*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y2z4=x1y1z4*y1
      x1y2z5=x1y1z5*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x1y3z4=x1y2z4*y1
      x1y3z5=x1y2z5*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x2z5=x1z5*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y1z4=x1y1z4*x1
      x2y1z5=x1y1z5*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y2z4=x1y2z4*x1
      x2y2z5=x1y2z5*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y3z3=x1y3z3*x1
      x2y3z4=x1y3z4*x1
      x2y3z5=x1y3z5*x1
      r(i1,i2,i3,n)=(c(3,2,0,n)*12.+c(3,2,1,n)*z1*12.+c(3,2,2,n)*z2*
     & 12.+c(3,2,3,n)*z3*12.+c(3,2,4,n)*z4*12.+c(3,2,5,n)*z5*12.+c(3,
     & 3,0,n)*y1*36.+c(3,3,1,n)*y1z1*36.+c(3,3,2,n)*y1z2*36.+c(3,3,3,
     & n)*y1z3*36.+c(3,3,4,n)*y1z4*36.+c(3,3,5,n)*y1z5*36.+c(3,4,0,n)*
     & y2*72.+c(3,4,1,n)*y2z1*72.+c(3,4,2,n)*y2z2*72.+c(3,4,3,n)*y2z3*
     & 72.+c(3,4,4,n)*y2z4*72.+c(3,4,5,n)*y2z5*72.+c(3,5,0,n)*y3*120.+
     & c(3,5,1,n)*y3z1*120.+c(3,5,2,n)*y3z2*120.+c(3,5,3,n)*y3z3*120.+
     & c(3,5,4,n)*y3z4*120.+c(3,5,5,n)*y3z5*120.+c(4,2,0,n)*x1*48.+c(
     & 4,2,1,n)*x1z1*48.+c(4,2,2,n)*x1z2*48.+c(4,2,3,n)*x1z3*48.+c(4,
     & 2,4,n)*x1z4*48.+c(4,2,5,n)*x1z5*48.+c(4,3,0,n)*x1y1*144.+c(4,3,
     & 1,n)*x1y1z1*144.+c(4,3,2,n)*x1y1z2*144.+c(4,3,3,n)*x1y1z3*144.+
     & c(4,3,4,n)*x1y1z4*144.+c(4,3,5,n)*x1y1z5*144.+c(4,4,0,n)*x1y2*
     & 288.+c(4,4,1,n)*x1y2z1*288.+c(4,4,2,n)*x1y2z2*288.+c(4,4,3,n)*
     & x1y2z3*288.+c(4,4,4,n)*x1y2z4*288.+c(4,4,5,n)*x1y2z5*288.+c(4,
     & 5,0,n)*x1y3*480.+c(4,5,1,n)*x1y3z1*480.+c(4,5,2,n)*x1y3z2*480.+
     & c(4,5,3,n)*x1y3z3*480.+c(4,5,4,n)*x1y3z4*480.+c(4,5,5,n)*
     & x1y3z5*480.+c(5,2,0,n)*x2*120.+c(5,2,1,n)*x2z1*120.+c(5,2,2,n)*
     & x2z2*120.+c(5,2,3,n)*x2z3*120.+c(5,2,4,n)*x2z4*120.+c(5,2,5,n)*
     & x2z5*120.+c(5,3,0,n)*x2y1*360.+c(5,3,1,n)*x2y1z1*360.+c(5,3,2,
     & n)*x2y1z2*360.+c(5,3,3,n)*x2y1z3*360.+c(5,3,4,n)*x2y1z4*360.+c(
     & 5,3,5,n)*x2y1z5*360.+c(5,4,0,n)*x2y2*720.+c(5,4,1,n)*x2y2z1*
     & 720.+c(5,4,2,n)*x2y2z2*720.+c(5,4,3,n)*x2y2z3*720.+c(5,4,4,n)*
     & x2y2z4*720.+c(5,4,5,n)*x2y2z5*720.+c(5,5,0,n)*x2y3*1200.+c(5,5,
     & 1,n)*x2y3z1*1200.+c(5,5,2,n)*x2y3z2*1200.+c(5,5,3,n)*x2y3z3*
     & 1200.+c(5,5,4,n)*x2y3z4*1200.+c(5,5,5,n)*x2y3z5*1200.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.3.and.dy.eq.2.and.dz.eq.1 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      y3z4=y2z4*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y2z4=x1y1z4*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x1y3z4=x1y2z4*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y1z4=x1y1z4*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y2z4=x1y2z4*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y3z3=x1y3z3*x1
      x2y3z4=x1y3z4*x1
      r(i1,i2,i3,n)=(c(3,2,1,n)*12.+c(3,2,2,n)*z1*24.+c(3,2,3,n)*z2*
     & 36.+c(3,2,4,n)*z3*48.+c(3,2,5,n)*z4*60.+c(3,3,1,n)*y1*36.+c(3,
     & 3,2,n)*y1z1*72.+c(3,3,3,n)*y1z2*108.+c(3,3,4,n)*y1z3*144.+c(3,
     & 3,5,n)*y1z4*180.+c(3,4,1,n)*y2*72.+c(3,4,2,n)*y2z1*144.+c(3,4,
     & 3,n)*y2z2*216.+c(3,4,4,n)*y2z3*288.+c(3,4,5,n)*y2z4*360.+c(3,5,
     & 1,n)*y3*120.+c(3,5,2,n)*y3z1*240.+c(3,5,3,n)*y3z2*360.+c(3,5,4,
     & n)*y3z3*480.+c(3,5,5,n)*y3z4*600.+c(4,2,1,n)*x1*48.+c(4,2,2,n)*
     & x1z1*96.+c(4,2,3,n)*x1z2*144.+c(4,2,4,n)*x1z3*192.+c(4,2,5,n)*
     & x1z4*240.+c(4,3,1,n)*x1y1*144.+c(4,3,2,n)*x1y1z1*288.+c(4,3,3,
     & n)*x1y1z2*432.+c(4,3,4,n)*x1y1z3*576.+c(4,3,5,n)*x1y1z4*720.+c(
     & 4,4,1,n)*x1y2*288.+c(4,4,2,n)*x1y2z1*576.+c(4,4,3,n)*x1y2z2*
     & 864.+c(4,4,4,n)*x1y2z3*1152.+c(4,4,5,n)*x1y2z4*1440.+c(4,5,1,n)
     & *x1y3*480.+c(4,5,2,n)*x1y3z1*960.+c(4,5,3,n)*x1y3z2*1440.+c(4,
     & 5,4,n)*x1y3z3*1920.+c(4,5,5,n)*x1y3z4*2400.+c(5,2,1,n)*x2*120.+
     & c(5,2,2,n)*x2z1*240.+c(5,2,3,n)*x2z2*360.+c(5,2,4,n)*x2z3*480.+
     & c(5,2,5,n)*x2z4*600.+c(5,3,1,n)*x2y1*360.+c(5,3,2,n)*x2y1z1*
     & 720.+c(5,3,3,n)*x2y1z2*1080.+c(5,3,4,n)*x2y1z3*1440.+c(5,3,5,n)
     & *x2y1z4*1800.+c(5,4,1,n)*x2y2*720.+c(5,4,2,n)*x2y2z1*1440.+c(5,
     & 4,3,n)*x2y2z2*2160.+c(5,4,4,n)*x2y2z3*2880.+c(5,4,5,n)*x2y2z4*
     & 3600.+c(5,5,1,n)*x2y3*1200.+c(5,5,2,n)*x2y3z1*2400.+c(5,5,3,n)*
     & x2y3z2*3600.+c(5,5,4,n)*x2y3z3*4800.+c(5,5,5,n)*x2y3z4*6000.)*
     & time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.3.and.dy.eq.2.and.dz.eq.2 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y3z3=x1y3z3*x1
      r(i1,i2,i3,n)=(c(3,2,2,n)*24.+c(3,2,3,n)*z1*72.+c(3,2,4,n)*z2*
     & 144.+c(3,2,5,n)*z3*240.+c(3,3,2,n)*y1*72.+c(3,3,3,n)*y1z1*216.+
     & c(3,3,4,n)*y1z2*432.+c(3,3,5,n)*y1z3*720.+c(3,4,2,n)*y2*144.+c(
     & 3,4,3,n)*y2z1*432.+c(3,4,4,n)*y2z2*864.+c(3,4,5,n)*y2z3*1440.+
     & c(3,5,2,n)*y3*240.+c(3,5,3,n)*y3z1*720.+c(3,5,4,n)*y3z2*1440.+
     & c(3,5,5,n)*y3z3*2400.+c(4,2,2,n)*x1*96.+c(4,2,3,n)*x1z1*288.+c(
     & 4,2,4,n)*x1z2*576.+c(4,2,5,n)*x1z3*960.+c(4,3,2,n)*x1y1*288.+c(
     & 4,3,3,n)*x1y1z1*864.+c(4,3,4,n)*x1y1z2*1728.+c(4,3,5,n)*x1y1z3*
     & 2880.+c(4,4,2,n)*x1y2*576.+c(4,4,3,n)*x1y2z1*1728.+c(4,4,4,n)*
     & x1y2z2*3456.+c(4,4,5,n)*x1y2z3*5760.+c(4,5,2,n)*x1y3*960.+c(4,
     & 5,3,n)*x1y3z1*2880.+c(4,5,4,n)*x1y3z2*5760.+c(4,5,5,n)*x1y3z3*
     & 9600.+c(5,2,2,n)*x2*240.+c(5,2,3,n)*x2z1*720.+c(5,2,4,n)*x2z2*
     & 1440.+c(5,2,5,n)*x2z3*2400.+c(5,3,2,n)*x2y1*720.+c(5,3,3,n)*
     & x2y1z1*2160.+c(5,3,4,n)*x2y1z2*4320.+c(5,3,5,n)*x2y1z3*7200.+c(
     & 5,4,2,n)*x2y2*1440.+c(5,4,3,n)*x2y2z1*4320.+c(5,4,4,n)*x2y2z2*
     & 8640.+c(5,4,5,n)*x2y2z3*14400.+c(5,5,2,n)*x2y3*2400.+c(5,5,3,n)
     & *x2y3z1*7200.+c(5,5,4,n)*x2y3z2*14400.+c(5,5,5,n)*x2y3z3*
     & 24000.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.3.and.dy.eq.2.and.dz.eq.3 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      r(i1,i2,i3,n)=(c(3,2,3,n)*72.+c(3,2,4,n)*z1*288.+c(3,2,5,n)*z2*
     & 720.+c(3,3,3,n)*y1*216.+c(3,3,4,n)*y1z1*864.+c(3,3,5,n)*y1z2*
     & 2160.+c(3,4,3,n)*y2*432.+c(3,4,4,n)*y2z1*1728.+c(3,4,5,n)*y2z2*
     & 4320.+c(3,5,3,n)*y3*720.+c(3,5,4,n)*y3z1*2880.+c(3,5,5,n)*y3z2*
     & 7200.+c(4,2,3,n)*x1*288.+c(4,2,4,n)*x1z1*1152.+c(4,2,5,n)*x1z2*
     & 2880.+c(4,3,3,n)*x1y1*864.+c(4,3,4,n)*x1y1z1*3456.+c(4,3,5,n)*
     & x1y1z2*8640.+c(4,4,3,n)*x1y2*1728.+c(4,4,4,n)*x1y2z1*6912.+c(4,
     & 4,5,n)*x1y2z2*17280.+c(4,5,3,n)*x1y3*2880.+c(4,5,4,n)*x1y3z1*
     & 11520.+c(4,5,5,n)*x1y3z2*28800.+c(5,2,3,n)*x2*720.+c(5,2,4,n)*
     & x2z1*2880.+c(5,2,5,n)*x2z2*7200.+c(5,3,3,n)*x2y1*2160.+c(5,3,4,
     & n)*x2y1z1*8640.+c(5,3,5,n)*x2y1z2*21600.+c(5,4,3,n)*x2y2*4320.+
     & c(5,4,4,n)*x2y2z1*17280.+c(5,4,5,n)*x2y2z2*43200.+c(5,5,3,n)*
     & x2y3*7200.+c(5,5,4,n)*x2y3z1*28800.+c(5,5,5,n)*x2y3z2*72000.)*
     & time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.3.and.dy.eq.2.and.dz.eq.4 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y3=y2*y1
      y3z1=y2z1*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      r(i1,i2,i3,n)=(c(3,2,4,n)*288.+c(3,2,5,n)*z1*1440.+c(3,3,4,n)*y1*
     & 864.+c(3,3,5,n)*y1z1*4320.+c(3,4,4,n)*y2*1728.+c(3,4,5,n)*y2z1*
     & 8640.+c(3,5,4,n)*y3*2880.+c(3,5,5,n)*y3z1*14400.+c(4,2,4,n)*x1*
     & 1152.+c(4,2,5,n)*x1z1*5760.+c(4,3,4,n)*x1y1*3456.+c(4,3,5,n)*
     & x1y1z1*17280.+c(4,4,4,n)*x1y2*6912.+c(4,4,5,n)*x1y2z1*34560.+c(
     & 4,5,4,n)*x1y3*11520.+c(4,5,5,n)*x1y3z1*57600.+c(5,2,4,n)*x2*
     & 2880.+c(5,2,5,n)*x2z1*14400.+c(5,3,4,n)*x2y1*8640.+c(5,3,5,n)*
     & x2y1z1*43200.+c(5,4,4,n)*x2y2*17280.+c(5,4,5,n)*x2y2z1*86400.+
     & c(5,5,4,n)*x2y3*28800.+c(5,5,5,n)*x2y3z1*144000.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.3.and.dy.eq.3.and.dz.eq.0 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y1z5=y1z4*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
      y2z5=y1z5*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1z5=x1z4*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y1z5=x1y1z4*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y2z4=x1y1z4*y1
      x1y2z5=x1y1z5*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x2z5=x1z5*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y1z4=x1y1z4*x1
      x2y1z5=x1y1z5*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y2z4=x1y2z4*x1
      x2y2z5=x1y2z5*x1
      r(i1,i2,i3,n)=(c(3,3,0,n)*36.+c(3,3,1,n)*z1*36.+c(3,3,2,n)*z2*
     & 36.+c(3,3,3,n)*z3*36.+c(3,3,4,n)*z4*36.+c(3,3,5,n)*z5*36.+c(3,
     & 4,0,n)*y1*144.+c(3,4,1,n)*y1z1*144.+c(3,4,2,n)*y1z2*144.+c(3,4,
     & 3,n)*y1z3*144.+c(3,4,4,n)*y1z4*144.+c(3,4,5,n)*y1z5*144.+c(3,5,
     & 0,n)*y2*360.+c(3,5,1,n)*y2z1*360.+c(3,5,2,n)*y2z2*360.+c(3,5,3,
     & n)*y2z3*360.+c(3,5,4,n)*y2z4*360.+c(3,5,5,n)*y2z5*360.+c(4,3,0,
     & n)*x1*144.+c(4,3,1,n)*x1z1*144.+c(4,3,2,n)*x1z2*144.+c(4,3,3,n)
     & *x1z3*144.+c(4,3,4,n)*x1z4*144.+c(4,3,5,n)*x1z5*144.+c(4,4,0,n)
     & *x1y1*576.+c(4,4,1,n)*x1y1z1*576.+c(4,4,2,n)*x1y1z2*576.+c(4,4,
     & 3,n)*x1y1z3*576.+c(4,4,4,n)*x1y1z4*576.+c(4,4,5,n)*x1y1z5*576.+
     & c(4,5,0,n)*x1y2*1440.+c(4,5,1,n)*x1y2z1*1440.+c(4,5,2,n)*
     & x1y2z2*1440.+c(4,5,3,n)*x1y2z3*1440.+c(4,5,4,n)*x1y2z4*1440.+c(
     & 4,5,5,n)*x1y2z5*1440.+c(5,3,0,n)*x2*360.+c(5,3,1,n)*x2z1*360.+
     & c(5,3,2,n)*x2z2*360.+c(5,3,3,n)*x2z3*360.+c(5,3,4,n)*x2z4*360.+
     & c(5,3,5,n)*x2z5*360.+c(5,4,0,n)*x2y1*1440.+c(5,4,1,n)*x2y1z1*
     & 1440.+c(5,4,2,n)*x2y1z2*1440.+c(5,4,3,n)*x2y1z3*1440.+c(5,4,4,
     & n)*x2y1z4*1440.+c(5,4,5,n)*x2y1z5*1440.+c(5,5,0,n)*x2y2*3600.+
     & c(5,5,1,n)*x2y2z1*3600.+c(5,5,2,n)*x2y2z2*3600.+c(5,5,3,n)*
     & x2y2z3*3600.+c(5,5,4,n)*x2y2z4*3600.+c(5,5,5,n)*x2y2z5*3600.)*
     & time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.3.and.dy.eq.3.and.dz.eq.1 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y2z4=x1y1z4*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y1z4=x1y1z4*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y2z4=x1y2z4*x1
      r(i1,i2,i3,n)=(c(3,3,1,n)*36.+c(3,3,2,n)*z1*72.+c(3,3,3,n)*z2*
     & 108.+c(3,3,4,n)*z3*144.+c(3,3,5,n)*z4*180.+c(3,4,1,n)*y1*144.+
     & c(3,4,2,n)*y1z1*288.+c(3,4,3,n)*y1z2*432.+c(3,4,4,n)*y1z3*576.+
     & c(3,4,5,n)*y1z4*720.+c(3,5,1,n)*y2*360.+c(3,5,2,n)*y2z1*720.+c(
     & 3,5,3,n)*y2z2*1080.+c(3,5,4,n)*y2z3*1440.+c(3,5,5,n)*y2z4*
     & 1800.+c(4,3,1,n)*x1*144.+c(4,3,2,n)*x1z1*288.+c(4,3,3,n)*x1z2*
     & 432.+c(4,3,4,n)*x1z3*576.+c(4,3,5,n)*x1z4*720.+c(4,4,1,n)*x1y1*
     & 576.+c(4,4,2,n)*x1y1z1*1152.+c(4,4,3,n)*x1y1z2*1728.+c(4,4,4,n)
     & *x1y1z3*2304.+c(4,4,5,n)*x1y1z4*2880.+c(4,5,1,n)*x1y2*1440.+c(
     & 4,5,2,n)*x1y2z1*2880.+c(4,5,3,n)*x1y2z2*4320.+c(4,5,4,n)*
     & x1y2z3*5760.+c(4,5,5,n)*x1y2z4*7200.+c(5,3,1,n)*x2*360.+c(5,3,
     & 2,n)*x2z1*720.+c(5,3,3,n)*x2z2*1080.+c(5,3,4,n)*x2z3*1440.+c(5,
     & 3,5,n)*x2z4*1800.+c(5,4,1,n)*x2y1*1440.+c(5,4,2,n)*x2y1z1*
     & 2880.+c(5,4,3,n)*x2y1z2*4320.+c(5,4,4,n)*x2y1z3*5760.+c(5,4,5,
     & n)*x2y1z4*7200.+c(5,5,1,n)*x2y2*3600.+c(5,5,2,n)*x2y2z1*7200.+
     & c(5,5,3,n)*x2y2z2*10800.+c(5,5,4,n)*x2y2z3*14400.+c(5,5,5,n)*
     & x2y2z4*18000.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.3.and.dy.eq.3.and.dz.eq.2 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      r(i1,i2,i3,n)=(c(3,3,2,n)*72.+c(3,3,3,n)*z1*216.+c(3,3,4,n)*z2*
     & 432.+c(3,3,5,n)*z3*720.+c(3,4,2,n)*y1*288.+c(3,4,3,n)*y1z1*
     & 864.+c(3,4,4,n)*y1z2*1728.+c(3,4,5,n)*y1z3*2880.+c(3,5,2,n)*y2*
     & 720.+c(3,5,3,n)*y2z1*2160.+c(3,5,4,n)*y2z2*4320.+c(3,5,5,n)*
     & y2z3*7200.+c(4,3,2,n)*x1*288.+c(4,3,3,n)*x1z1*864.+c(4,3,4,n)*
     & x1z2*1728.+c(4,3,5,n)*x1z3*2880.+c(4,4,2,n)*x1y1*1152.+c(4,4,3,
     & n)*x1y1z1*3456.+c(4,4,4,n)*x1y1z2*6912.+c(4,4,5,n)*x1y1z3*
     & 11520.+c(4,5,2,n)*x1y2*2880.+c(4,5,3,n)*x1y2z1*8640.+c(4,5,4,n)
     & *x1y2z2*17280.+c(4,5,5,n)*x1y2z3*28800.+c(5,3,2,n)*x2*720.+c(5,
     & 3,3,n)*x2z1*2160.+c(5,3,4,n)*x2z2*4320.+c(5,3,5,n)*x2z3*7200.+
     & c(5,4,2,n)*x2y1*2880.+c(5,4,3,n)*x2y1z1*8640.+c(5,4,4,n)*
     & x2y1z2*17280.+c(5,4,5,n)*x2y1z3*28800.+c(5,5,2,n)*x2y2*7200.+c(
     & 5,5,3,n)*x2y2z1*21600.+c(5,5,4,n)*x2y2z2*43200.+c(5,5,5,n)*
     & x2y2z3*72000.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.3.and.dy.eq.3.and.dz.eq.3 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      r(i1,i2,i3,n)=(c(3,3,3,n)*216.+c(3,3,4,n)*z1*864.+c(3,3,5,n)*z2*
     & 2160.+c(3,4,3,n)*y1*864.+c(3,4,4,n)*y1z1*3456.+c(3,4,5,n)*y1z2*
     & 8640.+c(3,5,3,n)*y2*2160.+c(3,5,4,n)*y2z1*8640.+c(3,5,5,n)*
     & y2z2*21600.+c(4,3,3,n)*x1*864.+c(4,3,4,n)*x1z1*3456.+c(4,3,5,n)
     & *x1z2*8640.+c(4,4,3,n)*x1y1*3456.+c(4,4,4,n)*x1y1z1*13824.+c(4,
     & 4,5,n)*x1y1z2*34560.+c(4,5,3,n)*x1y2*8640.+c(4,5,4,n)*x1y2z1*
     & 34560.+c(4,5,5,n)*x1y2z2*86400.+c(5,3,3,n)*x2*2160.+c(5,3,4,n)*
     & x2z1*8640.+c(5,3,5,n)*x2z2*21600.+c(5,4,3,n)*x2y1*8640.+c(5,4,
     & 4,n)*x2y1z1*34560.+c(5,4,5,n)*x2y1z2*86400.+c(5,5,3,n)*x2y2*
     & 21600.+c(5,5,4,n)*x2y2z1*86400.+c(5,5,5,n)*x2y2z2*216000.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.3.and.dy.eq.3.and.dz.eq.4 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      r(i1,i2,i3,n)=(c(3,3,4,n)*864.+c(3,3,5,n)*z1*4320.+c(3,4,4,n)*y1*
     & 3456.+c(3,4,5,n)*y1z1*17280.+c(3,5,4,n)*y2*8640.+c(3,5,5,n)*
     & y2z1*43200.+c(4,3,4,n)*x1*3456.+c(4,3,5,n)*x1z1*17280.+c(4,4,4,
     & n)*x1y1*13824.+c(4,4,5,n)*x1y1z1*69120.+c(4,5,4,n)*x1y2*34560.+
     & c(4,5,5,n)*x1y2z1*172800.+c(5,3,4,n)*x2*8640.+c(5,3,5,n)*x2z1*
     & 43200.+c(5,4,4,n)*x2y1*34560.+c(5,4,5,n)*x2y1z1*172800.+c(5,5,
     & 4,n)*x2y2*86400.+c(5,5,5,n)*x2y2z1*432000.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.3.and.dy.eq.4.and.dz.eq.0 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y1z5=y1z4*z1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1z5=x1z4*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y1z5=x1y1z4*z1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x2z5=x1z5*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y1z4=x1y1z4*x1
      x2y1z5=x1y1z5*x1
      r(i1,i2,i3,n)=(c(3,4,0,n)*144.+c(3,4,1,n)*z1*144.+c(3,4,2,n)*z2*
     & 144.+c(3,4,3,n)*z3*144.+c(3,4,4,n)*z4*144.+c(3,4,5,n)*z5*144.+
     & c(3,5,0,n)*y1*720.+c(3,5,1,n)*y1z1*720.+c(3,5,2,n)*y1z2*720.+c(
     & 3,5,3,n)*y1z3*720.+c(3,5,4,n)*y1z4*720.+c(3,5,5,n)*y1z5*720.+c(
     & 4,4,0,n)*x1*576.+c(4,4,1,n)*x1z1*576.+c(4,4,2,n)*x1z2*576.+c(4,
     & 4,3,n)*x1z3*576.+c(4,4,4,n)*x1z4*576.+c(4,4,5,n)*x1z5*576.+c(4,
     & 5,0,n)*x1y1*2880.+c(4,5,1,n)*x1y1z1*2880.+c(4,5,2,n)*x1y1z2*
     & 2880.+c(4,5,3,n)*x1y1z3*2880.+c(4,5,4,n)*x1y1z4*2880.+c(4,5,5,
     & n)*x1y1z5*2880.+c(5,4,0,n)*x2*1440.+c(5,4,1,n)*x2z1*1440.+c(5,
     & 4,2,n)*x2z2*1440.+c(5,4,3,n)*x2z3*1440.+c(5,4,4,n)*x2z4*1440.+
     & c(5,4,5,n)*x2z5*1440.+c(5,5,0,n)*x2y1*7200.+c(5,5,1,n)*x2y1z1*
     & 7200.+c(5,5,2,n)*x2y1z2*7200.+c(5,5,3,n)*x2y1z3*7200.+c(5,5,4,
     & n)*x2y1z4*7200.+c(5,5,5,n)*x2y1z5*7200.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.3.and.dy.eq.4.and.dz.eq.1 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y1z4=x1y1z4*x1
      r(i1,i2,i3,n)=(c(3,4,1,n)*144.+c(3,4,2,n)*z1*288.+c(3,4,3,n)*z2*
     & 432.+c(3,4,4,n)*z3*576.+c(3,4,5,n)*z4*720.+c(3,5,1,n)*y1*720.+
     & c(3,5,2,n)*y1z1*1440.+c(3,5,3,n)*y1z2*2160.+c(3,5,4,n)*y1z3*
     & 2880.+c(3,5,5,n)*y1z4*3600.+c(4,4,1,n)*x1*576.+c(4,4,2,n)*x1z1*
     & 1152.+c(4,4,3,n)*x1z2*1728.+c(4,4,4,n)*x1z3*2304.+c(4,4,5,n)*
     & x1z4*2880.+c(4,5,1,n)*x1y1*2880.+c(4,5,2,n)*x1y1z1*5760.+c(4,5,
     & 3,n)*x1y1z2*8640.+c(4,5,4,n)*x1y1z3*11520.+c(4,5,5,n)*x1y1z4*
     & 14400.+c(5,4,1,n)*x2*1440.+c(5,4,2,n)*x2z1*2880.+c(5,4,3,n)*
     & x2z2*4320.+c(5,4,4,n)*x2z3*5760.+c(5,4,5,n)*x2z4*7200.+c(5,5,1,
     & n)*x2y1*7200.+c(5,5,2,n)*x2y1z1*14400.+c(5,5,3,n)*x2y1z2*
     & 21600.+c(5,5,4,n)*x2y1z3*28800.+c(5,5,5,n)*x2y1z4*36000.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.3.and.dy.eq.4.and.dz.eq.2 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      r(i1,i2,i3,n)=(c(3,4,2,n)*288.+c(3,4,3,n)*z1*864.+c(3,4,4,n)*z2*
     & 1728.+c(3,4,5,n)*z3*2880.+c(3,5,2,n)*y1*1440.+c(3,5,3,n)*y1z1*
     & 4320.+c(3,5,4,n)*y1z2*8640.+c(3,5,5,n)*y1z3*14400.+c(4,4,2,n)*
     & x1*1152.+c(4,4,3,n)*x1z1*3456.+c(4,4,4,n)*x1z2*6912.+c(4,4,5,n)
     & *x1z3*11520.+c(4,5,2,n)*x1y1*5760.+c(4,5,3,n)*x1y1z1*17280.+c(
     & 4,5,4,n)*x1y1z2*34560.+c(4,5,5,n)*x1y1z3*57600.+c(5,4,2,n)*x2*
     & 2880.+c(5,4,3,n)*x2z1*8640.+c(5,4,4,n)*x2z2*17280.+c(5,4,5,n)*
     & x2z3*28800.+c(5,5,2,n)*x2y1*14400.+c(5,5,3,n)*x2y1z1*43200.+c(
     & 5,5,4,n)*x2y1z2*86400.+c(5,5,5,n)*x2y1z3*144000.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.3.and.dy.eq.4.and.dz.eq.3 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      r(i1,i2,i3,n)=(c(3,4,3,n)*864.+c(3,4,4,n)*z1*3456.+c(3,4,5,n)*z2*
     & 8640.+c(3,5,3,n)*y1*4320.+c(3,5,4,n)*y1z1*17280.+c(3,5,5,n)*
     & y1z2*43200.+c(4,4,3,n)*x1*3456.+c(4,4,4,n)*x1z1*13824.+c(4,4,5,
     & n)*x1z2*34560.+c(4,5,3,n)*x1y1*17280.+c(4,5,4,n)*x1y1z1*69120.+
     & c(4,5,5,n)*x1y1z2*172800.+c(5,4,3,n)*x2*8640.+c(5,4,4,n)*x2z1*
     & 34560.+c(5,4,5,n)*x2z2*86400.+c(5,5,3,n)*x2y1*43200.+c(5,5,4,n)
     & *x2y1z1*172800.+c(5,5,5,n)*x2y1z2*432000.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.3.and.dy.eq.4.and.dz.eq.4 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x2=x1*x1
      x2z1=x1z1*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      r(i1,i2,i3,n)=(c(3,4,4,n)*3456.+c(3,4,5,n)*z1*17280.+c(3,5,4,n)*
     & y1*17280.+c(3,5,5,n)*y1z1*86400.+c(4,4,4,n)*x1*13824.+c(4,4,5,
     & n)*x1z1*69120.+c(4,5,4,n)*x1y1*69120.+c(4,5,5,n)*x1y1z1*
     & 345600.+c(5,4,4,n)*x2*34560.+c(5,4,5,n)*x2z1*172800.+c(5,5,4,n)
     & *x2y1*172800.+c(5,5,5,n)*x2y1z1*864000.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.4.and.dy.eq.0.and.dz.eq.0 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y1z5=y1z4*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
      y2z5=y1z5*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      y3z4=y2z4*y1
      y3z5=y2z5*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y4z2=y3z2*y1
      y4z3=y3z3*y1
      y4z4=y3z4*y1
      y4z5=y3z5*y1
      y5=y4*y1
      y5z1=y4z1*y1
      y5z2=y4z2*y1
      y5z3=y4z3*y1
      y5z4=y4z4*y1
      y5z5=y4z5*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1z5=x1z4*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y1z5=x1y1z4*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y2z4=x1y1z4*y1
      x1y2z5=x1y1z5*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x1y3z4=x1y2z4*y1
      x1y3z5=x1y2z5*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y4z2=x1y3z2*y1
      x1y4z3=x1y3z3*y1
      x1y4z4=x1y3z4*y1
      x1y4z5=x1y3z5*y1
      x1y5=x1y4*y1
      x1y5z1=x1y4z1*y1
      x1y5z2=x1y4z2*y1
      x1y5z3=x1y4z3*y1
      x1y5z4=x1y4z4*y1
      x1y5z5=x1y4z5*y1
      r(i1,i2,i3,n)=(c(4,0,0,n)*24.+c(4,0,1,n)*z1*24.+c(4,0,2,n)*z2*
     & 24.+c(4,0,3,n)*z3*24.+c(4,0,4,n)*z4*24.+c(4,0,5,n)*z5*24.+c(4,
     & 1,0,n)*y1*24.+c(4,1,1,n)*y1z1*24.+c(4,1,2,n)*y1z2*24.+c(4,1,3,
     & n)*y1z3*24.+c(4,1,4,n)*y1z4*24.+c(4,1,5,n)*y1z5*24.+c(4,2,0,n)*
     & y2*24.+c(4,2,1,n)*y2z1*24.+c(4,2,2,n)*y2z2*24.+c(4,2,3,n)*y2z3*
     & 24.+c(4,2,4,n)*y2z4*24.+c(4,2,5,n)*y2z5*24.+c(4,3,0,n)*y3*24.+
     & c(4,3,1,n)*y3z1*24.+c(4,3,2,n)*y3z2*24.+c(4,3,3,n)*y3z3*24.+c(
     & 4,3,4,n)*y3z4*24.+c(4,3,5,n)*y3z5*24.+c(4,4,0,n)*y4*24.+c(4,4,
     & 1,n)*y4z1*24.+c(4,4,2,n)*y4z2*24.+c(4,4,3,n)*y4z3*24.+c(4,4,4,
     & n)*y4z4*24.+c(4,4,5,n)*y4z5*24.+c(4,5,0,n)*y5*24.+c(4,5,1,n)*
     & y5z1*24.+c(4,5,2,n)*y5z2*24.+c(4,5,3,n)*y5z3*24.+c(4,5,4,n)*
     & y5z4*24.+c(4,5,5,n)*y5z5*24.+c(5,0,0,n)*x1*120.+c(5,0,1,n)*
     & x1z1*120.+c(5,0,2,n)*x1z2*120.+c(5,0,3,n)*x1z3*120.+c(5,0,4,n)*
     & x1z4*120.+c(5,0,5,n)*x1z5*120.+c(5,1,0,n)*x1y1*120.+c(5,1,1,n)*
     & x1y1z1*120.+c(5,1,2,n)*x1y1z2*120.+c(5,1,3,n)*x1y1z3*120.+c(5,
     & 1,4,n)*x1y1z4*120.+c(5,1,5,n)*x1y1z5*120.+c(5,2,0,n)*x1y2*120.+
     & c(5,2,1,n)*x1y2z1*120.+c(5,2,2,n)*x1y2z2*120.+c(5,2,3,n)*
     & x1y2z3*120.+c(5,2,4,n)*x1y2z4*120.+c(5,2,5,n)*x1y2z5*120.+c(5,
     & 3,0,n)*x1y3*120.+c(5,3,1,n)*x1y3z1*120.+c(5,3,2,n)*x1y3z2*120.+
     & c(5,3,3,n)*x1y3z3*120.+c(5,3,4,n)*x1y3z4*120.+c(5,3,5,n)*
     & x1y3z5*120.+c(5,4,0,n)*x1y4*120.+c(5,4,1,n)*x1y4z1*120.+c(5,4,
     & 2,n)*x1y4z2*120.+c(5,4,3,n)*x1y4z3*120.+c(5,4,4,n)*x1y4z4*120.+
     & c(5,4,5,n)*x1y4z5*120.+c(5,5,0,n)*x1y5*120.+c(5,5,1,n)*x1y5z1*
     & 120.+c(5,5,2,n)*x1y5z2*120.+c(5,5,3,n)*x1y5z3*120.+c(5,5,4,n)*
     & x1y5z4*120.+c(5,5,5,n)*x1y5z5*120.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.4.and.dy.eq.0.and.dz.eq.1 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      y3z4=y2z4*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y4z2=y3z2*y1
      y4z3=y3z3*y1
      y4z4=y3z4*y1
      y5=y4*y1
      y5z1=y4z1*y1
      y5z2=y4z2*y1
      y5z3=y4z3*y1
      y5z4=y4z4*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y2z4=x1y1z4*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x1y3z4=x1y2z4*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y4z2=x1y3z2*y1
      x1y4z3=x1y3z3*y1
      x1y4z4=x1y3z4*y1
      x1y5=x1y4*y1
      x1y5z1=x1y4z1*y1
      x1y5z2=x1y4z2*y1
      x1y5z3=x1y4z3*y1
      x1y5z4=x1y4z4*y1
      r(i1,i2,i3,n)=(c(4,0,1,n)*24.+c(4,0,2,n)*z1*48.+c(4,0,3,n)*z2*
     & 72.+c(4,0,4,n)*z3*96.+c(4,0,5,n)*z4*120.+c(4,1,1,n)*y1*24.+c(4,
     & 1,2,n)*y1z1*48.+c(4,1,3,n)*y1z2*72.+c(4,1,4,n)*y1z3*96.+c(4,1,
     & 5,n)*y1z4*120.+c(4,2,1,n)*y2*24.+c(4,2,2,n)*y2z1*48.+c(4,2,3,n)
     & *y2z2*72.+c(4,2,4,n)*y2z3*96.+c(4,2,5,n)*y2z4*120.+c(4,3,1,n)*
     & y3*24.+c(4,3,2,n)*y3z1*48.+c(4,3,3,n)*y3z2*72.+c(4,3,4,n)*y3z3*
     & 96.+c(4,3,5,n)*y3z4*120.+c(4,4,1,n)*y4*24.+c(4,4,2,n)*y4z1*48.+
     & c(4,4,3,n)*y4z2*72.+c(4,4,4,n)*y4z3*96.+c(4,4,5,n)*y4z4*120.+c(
     & 4,5,1,n)*y5*24.+c(4,5,2,n)*y5z1*48.+c(4,5,3,n)*y5z2*72.+c(4,5,
     & 4,n)*y5z3*96.+c(4,5,5,n)*y5z4*120.+c(5,0,1,n)*x1*120.+c(5,0,2,
     & n)*x1z1*240.+c(5,0,3,n)*x1z2*360.+c(5,0,4,n)*x1z3*480.+c(5,0,5,
     & n)*x1z4*600.+c(5,1,1,n)*x1y1*120.+c(5,1,2,n)*x1y1z1*240.+c(5,1,
     & 3,n)*x1y1z2*360.+c(5,1,4,n)*x1y1z3*480.+c(5,1,5,n)*x1y1z4*600.+
     & c(5,2,1,n)*x1y2*120.+c(5,2,2,n)*x1y2z1*240.+c(5,2,3,n)*x1y2z2*
     & 360.+c(5,2,4,n)*x1y2z3*480.+c(5,2,5,n)*x1y2z4*600.+c(5,3,1,n)*
     & x1y3*120.+c(5,3,2,n)*x1y3z1*240.+c(5,3,3,n)*x1y3z2*360.+c(5,3,
     & 4,n)*x1y3z3*480.+c(5,3,5,n)*x1y3z4*600.+c(5,4,1,n)*x1y4*120.+c(
     & 5,4,2,n)*x1y4z1*240.+c(5,4,3,n)*x1y4z2*360.+c(5,4,4,n)*x1y4z3*
     & 480.+c(5,4,5,n)*x1y4z4*600.+c(5,5,1,n)*x1y5*120.+c(5,5,2,n)*
     & x1y5z1*240.+c(5,5,3,n)*x1y5z2*360.+c(5,5,4,n)*x1y5z3*480.+c(5,
     & 5,5,n)*x1y5z4*600.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.4.and.dy.eq.0.and.dz.eq.2 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y4z2=y3z2*y1
      y4z3=y3z3*y1
      y5=y4*y1
      y5z1=y4z1*y1
      y5z2=y4z2*y1
      y5z3=y4z3*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y4z2=x1y3z2*y1
      x1y4z3=x1y3z3*y1
      x1y5=x1y4*y1
      x1y5z1=x1y4z1*y1
      x1y5z2=x1y4z2*y1
      x1y5z3=x1y4z3*y1
      r(i1,i2,i3,n)=(c(4,0,2,n)*48.+c(4,0,3,n)*z1*144.+c(4,0,4,n)*z2*
     & 288.+c(4,0,5,n)*z3*480.+c(4,1,2,n)*y1*48.+c(4,1,3,n)*y1z1*144.+
     & c(4,1,4,n)*y1z2*288.+c(4,1,5,n)*y1z3*480.+c(4,2,2,n)*y2*48.+c(
     & 4,2,3,n)*y2z1*144.+c(4,2,4,n)*y2z2*288.+c(4,2,5,n)*y2z3*480.+c(
     & 4,3,2,n)*y3*48.+c(4,3,3,n)*y3z1*144.+c(4,3,4,n)*y3z2*288.+c(4,
     & 3,5,n)*y3z3*480.+c(4,4,2,n)*y4*48.+c(4,4,3,n)*y4z1*144.+c(4,4,
     & 4,n)*y4z2*288.+c(4,4,5,n)*y4z3*480.+c(4,5,2,n)*y5*48.+c(4,5,3,
     & n)*y5z1*144.+c(4,5,4,n)*y5z2*288.+c(4,5,5,n)*y5z3*480.+c(5,0,2,
     & n)*x1*240.+c(5,0,3,n)*x1z1*720.+c(5,0,4,n)*x1z2*1440.+c(5,0,5,
     & n)*x1z3*2400.+c(5,1,2,n)*x1y1*240.+c(5,1,3,n)*x1y1z1*720.+c(5,
     & 1,4,n)*x1y1z2*1440.+c(5,1,5,n)*x1y1z3*2400.+c(5,2,2,n)*x1y2*
     & 240.+c(5,2,3,n)*x1y2z1*720.+c(5,2,4,n)*x1y2z2*1440.+c(5,2,5,n)*
     & x1y2z3*2400.+c(5,3,2,n)*x1y3*240.+c(5,3,3,n)*x1y3z1*720.+c(5,3,
     & 4,n)*x1y3z2*1440.+c(5,3,5,n)*x1y3z3*2400.+c(5,4,2,n)*x1y4*240.+
     & c(5,4,3,n)*x1y4z1*720.+c(5,4,4,n)*x1y4z2*1440.+c(5,4,5,n)*
     & x1y4z3*2400.+c(5,5,2,n)*x1y5*240.+c(5,5,3,n)*x1y5z1*720.+c(5,5,
     & 4,n)*x1y5z2*1440.+c(5,5,5,n)*x1y5z3*2400.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.4.and.dy.eq.0.and.dz.eq.3 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y4z2=y3z2*y1
      y5=y4*y1
      y5z1=y4z1*y1
      y5z2=y4z2*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y4z2=x1y3z2*y1
      x1y5=x1y4*y1
      x1y5z1=x1y4z1*y1
      x1y5z2=x1y4z2*y1
      r(i1,i2,i3,n)=(c(4,0,3,n)*144.+c(4,0,4,n)*z1*576.+c(4,0,5,n)*z2*
     & 1440.+c(4,1,3,n)*y1*144.+c(4,1,4,n)*y1z1*576.+c(4,1,5,n)*y1z2*
     & 1440.+c(4,2,3,n)*y2*144.+c(4,2,4,n)*y2z1*576.+c(4,2,5,n)*y2z2*
     & 1440.+c(4,3,3,n)*y3*144.+c(4,3,4,n)*y3z1*576.+c(4,3,5,n)*y3z2*
     & 1440.+c(4,4,3,n)*y4*144.+c(4,4,4,n)*y4z1*576.+c(4,4,5,n)*y4z2*
     & 1440.+c(4,5,3,n)*y5*144.+c(4,5,4,n)*y5z1*576.+c(4,5,5,n)*y5z2*
     & 1440.+c(5,0,3,n)*x1*720.+c(5,0,4,n)*x1z1*2880.+c(5,0,5,n)*x1z2*
     & 7200.+c(5,1,3,n)*x1y1*720.+c(5,1,4,n)*x1y1z1*2880.+c(5,1,5,n)*
     & x1y1z2*7200.+c(5,2,3,n)*x1y2*720.+c(5,2,4,n)*x1y2z1*2880.+c(5,
     & 2,5,n)*x1y2z2*7200.+c(5,3,3,n)*x1y3*720.+c(5,3,4,n)*x1y3z1*
     & 2880.+c(5,3,5,n)*x1y3z2*7200.+c(5,4,3,n)*x1y4*720.+c(5,4,4,n)*
     & x1y4z1*2880.+c(5,4,5,n)*x1y4z2*7200.+c(5,5,3,n)*x1y5*720.+c(5,
     & 5,4,n)*x1y5z1*2880.+c(5,5,5,n)*x1y5z2*7200.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.4.and.dy.eq.0.and.dz.eq.4 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y5=y4*y1
      y5z1=y4z1*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y5=x1y4*y1
      x1y5z1=x1y4z1*y1
      r(i1,i2,i3,n)=(c(4,0,4,n)*576.+c(4,0,5,n)*z1*2880.+c(4,1,4,n)*y1*
     & 576.+c(4,1,5,n)*y1z1*2880.+c(4,2,4,n)*y2*576.+c(4,2,5,n)*y2z1*
     & 2880.+c(4,3,4,n)*y3*576.+c(4,3,5,n)*y3z1*2880.+c(4,4,4,n)*y4*
     & 576.+c(4,4,5,n)*y4z1*2880.+c(4,5,4,n)*y5*576.+c(4,5,5,n)*y5z1*
     & 2880.+c(5,0,4,n)*x1*2880.+c(5,0,5,n)*x1z1*14400.+c(5,1,4,n)*
     & x1y1*2880.+c(5,1,5,n)*x1y1z1*14400.+c(5,2,4,n)*x1y2*2880.+c(5,
     & 2,5,n)*x1y2z1*14400.+c(5,3,4,n)*x1y3*2880.+c(5,3,5,n)*x1y3z1*
     & 14400.+c(5,4,4,n)*x1y4*2880.+c(5,4,5,n)*x1y4z1*14400.+c(5,5,4,
     & n)*x1y5*2880.+c(5,5,5,n)*x1y5z1*14400.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.4.and.dy.eq.1.and.dz.eq.0 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y1z5=y1z4*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
      y2z5=y1z5*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      y3z4=y2z4*y1
      y3z5=y2z5*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y4z2=y3z2*y1
      y4z3=y3z3*y1
      y4z4=y3z4*y1
      y4z5=y3z5*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1z5=x1z4*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y1z5=x1y1z4*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y2z4=x1y1z4*y1
      x1y2z5=x1y1z5*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x1y3z4=x1y2z4*y1
      x1y3z5=x1y2z5*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y4z2=x1y3z2*y1
      x1y4z3=x1y3z3*y1
      x1y4z4=x1y3z4*y1
      x1y4z5=x1y3z5*y1
      r(i1,i2,i3,n)=(c(4,1,0,n)*24.+c(4,1,1,n)*z1*24.+c(4,1,2,n)*z2*
     & 24.+c(4,1,3,n)*z3*24.+c(4,1,4,n)*z4*24.+c(4,1,5,n)*z5*24.+c(4,
     & 2,0,n)*y1*48.+c(4,2,1,n)*y1z1*48.+c(4,2,2,n)*y1z2*48.+c(4,2,3,
     & n)*y1z3*48.+c(4,2,4,n)*y1z4*48.+c(4,2,5,n)*y1z5*48.+c(4,3,0,n)*
     & y2*72.+c(4,3,1,n)*y2z1*72.+c(4,3,2,n)*y2z2*72.+c(4,3,3,n)*y2z3*
     & 72.+c(4,3,4,n)*y2z4*72.+c(4,3,5,n)*y2z5*72.+c(4,4,0,n)*y3*96.+
     & c(4,4,1,n)*y3z1*96.+c(4,4,2,n)*y3z2*96.+c(4,4,3,n)*y3z3*96.+c(
     & 4,4,4,n)*y3z4*96.+c(4,4,5,n)*y3z5*96.+c(4,5,0,n)*y4*120.+c(4,5,
     & 1,n)*y4z1*120.+c(4,5,2,n)*y4z2*120.+c(4,5,3,n)*y4z3*120.+c(4,5,
     & 4,n)*y4z4*120.+c(4,5,5,n)*y4z5*120.+c(5,1,0,n)*x1*120.+c(5,1,1,
     & n)*x1z1*120.+c(5,1,2,n)*x1z2*120.+c(5,1,3,n)*x1z3*120.+c(5,1,4,
     & n)*x1z4*120.+c(5,1,5,n)*x1z5*120.+c(5,2,0,n)*x1y1*240.+c(5,2,1,
     & n)*x1y1z1*240.+c(5,2,2,n)*x1y1z2*240.+c(5,2,3,n)*x1y1z3*240.+c(
     & 5,2,4,n)*x1y1z4*240.+c(5,2,5,n)*x1y1z5*240.+c(5,3,0,n)*x1y2*
     & 360.+c(5,3,1,n)*x1y2z1*360.+c(5,3,2,n)*x1y2z2*360.+c(5,3,3,n)*
     & x1y2z3*360.+c(5,3,4,n)*x1y2z4*360.+c(5,3,5,n)*x1y2z5*360.+c(5,
     & 4,0,n)*x1y3*480.+c(5,4,1,n)*x1y3z1*480.+c(5,4,2,n)*x1y3z2*480.+
     & c(5,4,3,n)*x1y3z3*480.+c(5,4,4,n)*x1y3z4*480.+c(5,4,5,n)*
     & x1y3z5*480.+c(5,5,0,n)*x1y4*600.+c(5,5,1,n)*x1y4z1*600.+c(5,5,
     & 2,n)*x1y4z2*600.+c(5,5,3,n)*x1y4z3*600.+c(5,5,4,n)*x1y4z4*600.+
     & c(5,5,5,n)*x1y4z5*600.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.4.and.dy.eq.1.and.dz.eq.1 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      y3z4=y2z4*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y4z2=y3z2*y1
      y4z3=y3z3*y1
      y4z4=y3z4*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y2z4=x1y1z4*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x1y3z4=x1y2z4*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y4z2=x1y3z2*y1
      x1y4z3=x1y3z3*y1
      x1y4z4=x1y3z4*y1
      r(i1,i2,i3,n)=(c(4,1,1,n)*24.+c(4,1,2,n)*z1*48.+c(4,1,3,n)*z2*
     & 72.+c(4,1,4,n)*z3*96.+c(4,1,5,n)*z4*120.+c(4,2,1,n)*y1*48.+c(4,
     & 2,2,n)*y1z1*96.+c(4,2,3,n)*y1z2*144.+c(4,2,4,n)*y1z3*192.+c(4,
     & 2,5,n)*y1z4*240.+c(4,3,1,n)*y2*72.+c(4,3,2,n)*y2z1*144.+c(4,3,
     & 3,n)*y2z2*216.+c(4,3,4,n)*y2z3*288.+c(4,3,5,n)*y2z4*360.+c(4,4,
     & 1,n)*y3*96.+c(4,4,2,n)*y3z1*192.+c(4,4,3,n)*y3z2*288.+c(4,4,4,
     & n)*y3z3*384.+c(4,4,5,n)*y3z4*480.+c(4,5,1,n)*y4*120.+c(4,5,2,n)
     & *y4z1*240.+c(4,5,3,n)*y4z2*360.+c(4,5,4,n)*y4z3*480.+c(4,5,5,n)
     & *y4z4*600.+c(5,1,1,n)*x1*120.+c(5,1,2,n)*x1z1*240.+c(5,1,3,n)*
     & x1z2*360.+c(5,1,4,n)*x1z3*480.+c(5,1,5,n)*x1z4*600.+c(5,2,1,n)*
     & x1y1*240.+c(5,2,2,n)*x1y1z1*480.+c(5,2,3,n)*x1y1z2*720.+c(5,2,
     & 4,n)*x1y1z3*960.+c(5,2,5,n)*x1y1z4*1200.+c(5,3,1,n)*x1y2*360.+
     & c(5,3,2,n)*x1y2z1*720.+c(5,3,3,n)*x1y2z2*1080.+c(5,3,4,n)*
     & x1y2z3*1440.+c(5,3,5,n)*x1y2z4*1800.+c(5,4,1,n)*x1y3*480.+c(5,
     & 4,2,n)*x1y3z1*960.+c(5,4,3,n)*x1y3z2*1440.+c(5,4,4,n)*x1y3z3*
     & 1920.+c(5,4,5,n)*x1y3z4*2400.+c(5,5,1,n)*x1y4*600.+c(5,5,2,n)*
     & x1y4z1*1200.+c(5,5,3,n)*x1y4z2*1800.+c(5,5,4,n)*x1y4z3*2400.+c(
     & 5,5,5,n)*x1y4z4*3000.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.4.and.dy.eq.1.and.dz.eq.2 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y4z2=y3z2*y1
      y4z3=y3z3*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y4z2=x1y3z2*y1
      x1y4z3=x1y3z3*y1
      r(i1,i2,i3,n)=(c(4,1,2,n)*48.+c(4,1,3,n)*z1*144.+c(4,1,4,n)*z2*
     & 288.+c(4,1,5,n)*z3*480.+c(4,2,2,n)*y1*96.+c(4,2,3,n)*y1z1*288.+
     & c(4,2,4,n)*y1z2*576.+c(4,2,5,n)*y1z3*960.+c(4,3,2,n)*y2*144.+c(
     & 4,3,3,n)*y2z1*432.+c(4,3,4,n)*y2z2*864.+c(4,3,5,n)*y2z3*1440.+
     & c(4,4,2,n)*y3*192.+c(4,4,3,n)*y3z1*576.+c(4,4,4,n)*y3z2*1152.+
     & c(4,4,5,n)*y3z3*1920.+c(4,5,2,n)*y4*240.+c(4,5,3,n)*y4z1*720.+
     & c(4,5,4,n)*y4z2*1440.+c(4,5,5,n)*y4z3*2400.+c(5,1,2,n)*x1*240.+
     & c(5,1,3,n)*x1z1*720.+c(5,1,4,n)*x1z2*1440.+c(5,1,5,n)*x1z3*
     & 2400.+c(5,2,2,n)*x1y1*480.+c(5,2,3,n)*x1y1z1*1440.+c(5,2,4,n)*
     & x1y1z2*2880.+c(5,2,5,n)*x1y1z3*4800.+c(5,3,2,n)*x1y2*720.+c(5,
     & 3,3,n)*x1y2z1*2160.+c(5,3,4,n)*x1y2z2*4320.+c(5,3,5,n)*x1y2z3*
     & 7200.+c(5,4,2,n)*x1y3*960.+c(5,4,3,n)*x1y3z1*2880.+c(5,4,4,n)*
     & x1y3z2*5760.+c(5,4,5,n)*x1y3z3*9600.+c(5,5,2,n)*x1y4*1200.+c(5,
     & 5,3,n)*x1y4z1*3600.+c(5,5,4,n)*x1y4z2*7200.+c(5,5,5,n)*x1y4z3*
     & 12000.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.4.and.dy.eq.1.and.dz.eq.3 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y4z2=y3z2*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y4z2=x1y3z2*y1
      r(i1,i2,i3,n)=(c(4,1,3,n)*144.+c(4,1,4,n)*z1*576.+c(4,1,5,n)*z2*
     & 1440.+c(4,2,3,n)*y1*288.+c(4,2,4,n)*y1z1*1152.+c(4,2,5,n)*y1z2*
     & 2880.+c(4,3,3,n)*y2*432.+c(4,3,4,n)*y2z1*1728.+c(4,3,5,n)*y2z2*
     & 4320.+c(4,4,3,n)*y3*576.+c(4,4,4,n)*y3z1*2304.+c(4,4,5,n)*y3z2*
     & 5760.+c(4,5,3,n)*y4*720.+c(4,5,4,n)*y4z1*2880.+c(4,5,5,n)*y4z2*
     & 7200.+c(5,1,3,n)*x1*720.+c(5,1,4,n)*x1z1*2880.+c(5,1,5,n)*x1z2*
     & 7200.+c(5,2,3,n)*x1y1*1440.+c(5,2,4,n)*x1y1z1*5760.+c(5,2,5,n)*
     & x1y1z2*14400.+c(5,3,3,n)*x1y2*2160.+c(5,3,4,n)*x1y2z1*8640.+c(
     & 5,3,5,n)*x1y2z2*21600.+c(5,4,3,n)*x1y3*2880.+c(5,4,4,n)*x1y3z1*
     & 11520.+c(5,4,5,n)*x1y3z2*28800.+c(5,5,3,n)*x1y4*3600.+c(5,5,4,
     & n)*x1y4z1*14400.+c(5,5,5,n)*x1y4z2*36000.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.4.and.dy.eq.1.and.dz.eq.4 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y4=y3*y1
      y4z1=y3z1*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      r(i1,i2,i3,n)=(c(4,1,4,n)*576.+c(4,1,5,n)*z1*2880.+c(4,2,4,n)*y1*
     & 1152.+c(4,2,5,n)*y1z1*5760.+c(4,3,4,n)*y2*1728.+c(4,3,5,n)*
     & y2z1*8640.+c(4,4,4,n)*y3*2304.+c(4,4,5,n)*y3z1*11520.+c(4,5,4,
     & n)*y4*2880.+c(4,5,5,n)*y4z1*14400.+c(5,1,4,n)*x1*2880.+c(5,1,5,
     & n)*x1z1*14400.+c(5,2,4,n)*x1y1*5760.+c(5,2,5,n)*x1y1z1*28800.+
     & c(5,3,4,n)*x1y2*8640.+c(5,3,5,n)*x1y2z1*43200.+c(5,4,4,n)*x1y3*
     & 11520.+c(5,4,5,n)*x1y3z1*57600.+c(5,5,4,n)*x1y4*14400.+c(5,5,5,
     & n)*x1y4z1*72000.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.4.and.dy.eq.2.and.dz.eq.0 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y1z5=y1z4*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
      y2z5=y1z5*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      y3z4=y2z4*y1
      y3z5=y2z5*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1z5=x1z4*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y1z5=x1y1z4*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y2z4=x1y1z4*y1
      x1y2z5=x1y1z5*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x1y3z4=x1y2z4*y1
      x1y3z5=x1y2z5*y1
      r(i1,i2,i3,n)=(c(4,2,0,n)*48.+c(4,2,1,n)*z1*48.+c(4,2,2,n)*z2*
     & 48.+c(4,2,3,n)*z3*48.+c(4,2,4,n)*z4*48.+c(4,2,5,n)*z5*48.+c(4,
     & 3,0,n)*y1*144.+c(4,3,1,n)*y1z1*144.+c(4,3,2,n)*y1z2*144.+c(4,3,
     & 3,n)*y1z3*144.+c(4,3,4,n)*y1z4*144.+c(4,3,5,n)*y1z5*144.+c(4,4,
     & 0,n)*y2*288.+c(4,4,1,n)*y2z1*288.+c(4,4,2,n)*y2z2*288.+c(4,4,3,
     & n)*y2z3*288.+c(4,4,4,n)*y2z4*288.+c(4,4,5,n)*y2z5*288.+c(4,5,0,
     & n)*y3*480.+c(4,5,1,n)*y3z1*480.+c(4,5,2,n)*y3z2*480.+c(4,5,3,n)
     & *y3z3*480.+c(4,5,4,n)*y3z4*480.+c(4,5,5,n)*y3z5*480.+c(5,2,0,n)
     & *x1*240.+c(5,2,1,n)*x1z1*240.+c(5,2,2,n)*x1z2*240.+c(5,2,3,n)*
     & x1z3*240.+c(5,2,4,n)*x1z4*240.+c(5,2,5,n)*x1z5*240.+c(5,3,0,n)*
     & x1y1*720.+c(5,3,1,n)*x1y1z1*720.+c(5,3,2,n)*x1y1z2*720.+c(5,3,
     & 3,n)*x1y1z3*720.+c(5,3,4,n)*x1y1z4*720.+c(5,3,5,n)*x1y1z5*720.+
     & c(5,4,0,n)*x1y2*1440.+c(5,4,1,n)*x1y2z1*1440.+c(5,4,2,n)*
     & x1y2z2*1440.+c(5,4,3,n)*x1y2z3*1440.+c(5,4,4,n)*x1y2z4*1440.+c(
     & 5,4,5,n)*x1y2z5*1440.+c(5,5,0,n)*x1y3*2400.+c(5,5,1,n)*x1y3z1*
     & 2400.+c(5,5,2,n)*x1y3z2*2400.+c(5,5,3,n)*x1y3z3*2400.+c(5,5,4,
     & n)*x1y3z4*2400.+c(5,5,5,n)*x1y3z5*2400.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.4.and.dy.eq.2.and.dz.eq.1 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      y3z4=y2z4*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y2z4=x1y1z4*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x1y3z4=x1y2z4*y1
      r(i1,i2,i3,n)=(c(4,2,1,n)*48.+c(4,2,2,n)*z1*96.+c(4,2,3,n)*z2*
     & 144.+c(4,2,4,n)*z3*192.+c(4,2,5,n)*z4*240.+c(4,3,1,n)*y1*144.+
     & c(4,3,2,n)*y1z1*288.+c(4,3,3,n)*y1z2*432.+c(4,3,4,n)*y1z3*576.+
     & c(4,3,5,n)*y1z4*720.+c(4,4,1,n)*y2*288.+c(4,4,2,n)*y2z1*576.+c(
     & 4,4,3,n)*y2z2*864.+c(4,4,4,n)*y2z3*1152.+c(4,4,5,n)*y2z4*1440.+
     & c(4,5,1,n)*y3*480.+c(4,5,2,n)*y3z1*960.+c(4,5,3,n)*y3z2*1440.+
     & c(4,5,4,n)*y3z3*1920.+c(4,5,5,n)*y3z4*2400.+c(5,2,1,n)*x1*240.+
     & c(5,2,2,n)*x1z1*480.+c(5,2,3,n)*x1z2*720.+c(5,2,4,n)*x1z3*960.+
     & c(5,2,5,n)*x1z4*1200.+c(5,3,1,n)*x1y1*720.+c(5,3,2,n)*x1y1z1*
     & 1440.+c(5,3,3,n)*x1y1z2*2160.+c(5,3,4,n)*x1y1z3*2880.+c(5,3,5,
     & n)*x1y1z4*3600.+c(5,4,1,n)*x1y2*1440.+c(5,4,2,n)*x1y2z1*2880.+
     & c(5,4,3,n)*x1y2z2*4320.+c(5,4,4,n)*x1y2z3*5760.+c(5,4,5,n)*
     & x1y2z4*7200.+c(5,5,1,n)*x1y3*2400.+c(5,5,2,n)*x1y3z1*4800.+c(5,
     & 5,3,n)*x1y3z2*7200.+c(5,5,4,n)*x1y3z3*9600.+c(5,5,5,n)*x1y3z4*
     & 12000.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.4.and.dy.eq.2.and.dz.eq.2 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      r(i1,i2,i3,n)=(c(4,2,2,n)*96.+c(4,2,3,n)*z1*288.+c(4,2,4,n)*z2*
     & 576.+c(4,2,5,n)*z3*960.+c(4,3,2,n)*y1*288.+c(4,3,3,n)*y1z1*
     & 864.+c(4,3,4,n)*y1z2*1728.+c(4,3,5,n)*y1z3*2880.+c(4,4,2,n)*y2*
     & 576.+c(4,4,3,n)*y2z1*1728.+c(4,4,4,n)*y2z2*3456.+c(4,4,5,n)*
     & y2z3*5760.+c(4,5,2,n)*y3*960.+c(4,5,3,n)*y3z1*2880.+c(4,5,4,n)*
     & y3z2*5760.+c(4,5,5,n)*y3z3*9600.+c(5,2,2,n)*x1*480.+c(5,2,3,n)*
     & x1z1*1440.+c(5,2,4,n)*x1z2*2880.+c(5,2,5,n)*x1z3*4800.+c(5,3,2,
     & n)*x1y1*1440.+c(5,3,3,n)*x1y1z1*4320.+c(5,3,4,n)*x1y1z2*8640.+
     & c(5,3,5,n)*x1y1z3*14400.+c(5,4,2,n)*x1y2*2880.+c(5,4,3,n)*
     & x1y2z1*8640.+c(5,4,4,n)*x1y2z2*17280.+c(5,4,5,n)*x1y2z3*28800.+
     & c(5,5,2,n)*x1y3*4800.+c(5,5,3,n)*x1y3z1*14400.+c(5,5,4,n)*
     & x1y3z2*28800.+c(5,5,5,n)*x1y3z3*48000.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.4.and.dy.eq.2.and.dz.eq.3 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      r(i1,i2,i3,n)=(c(4,2,3,n)*288.+c(4,2,4,n)*z1*1152.+c(4,2,5,n)*z2*
     & 2880.+c(4,3,3,n)*y1*864.+c(4,3,4,n)*y1z1*3456.+c(4,3,5,n)*y1z2*
     & 8640.+c(4,4,3,n)*y2*1728.+c(4,4,4,n)*y2z1*6912.+c(4,4,5,n)*
     & y2z2*17280.+c(4,5,3,n)*y3*2880.+c(4,5,4,n)*y3z1*11520.+c(4,5,5,
     & n)*y3z2*28800.+c(5,2,3,n)*x1*1440.+c(5,2,4,n)*x1z1*5760.+c(5,2,
     & 5,n)*x1z2*14400.+c(5,3,3,n)*x1y1*4320.+c(5,3,4,n)*x1y1z1*
     & 17280.+c(5,3,5,n)*x1y1z2*43200.+c(5,4,3,n)*x1y2*8640.+c(5,4,4,
     & n)*x1y2z1*34560.+c(5,4,5,n)*x1y2z2*86400.+c(5,5,3,n)*x1y3*
     & 14400.+c(5,5,4,n)*x1y3z1*57600.+c(5,5,5,n)*x1y3z2*144000.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.4.and.dy.eq.2.and.dz.eq.4 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y3=y2*y1
      y3z1=y2z1*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      r(i1,i2,i3,n)=(c(4,2,4,n)*1152.+c(4,2,5,n)*z1*5760.+c(4,3,4,n)*
     & y1*3456.+c(4,3,5,n)*y1z1*17280.+c(4,4,4,n)*y2*6912.+c(4,4,5,n)*
     & y2z1*34560.+c(4,5,4,n)*y3*11520.+c(4,5,5,n)*y3z1*57600.+c(5,2,
     & 4,n)*x1*5760.+c(5,2,5,n)*x1z1*28800.+c(5,3,4,n)*x1y1*17280.+c(
     & 5,3,5,n)*x1y1z1*86400.+c(5,4,4,n)*x1y2*34560.+c(5,4,5,n)*
     & x1y2z1*172800.+c(5,5,4,n)*x1y3*57600.+c(5,5,5,n)*x1y3z1*
     & 288000.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.4.and.dy.eq.3.and.dz.eq.0 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y1z5=y1z4*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
      y2z5=y1z5*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1z5=x1z4*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y1z5=x1y1z4*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y2z4=x1y1z4*y1
      x1y2z5=x1y1z5*y1
      r(i1,i2,i3,n)=(c(4,3,0,n)*144.+c(4,3,1,n)*z1*144.+c(4,3,2,n)*z2*
     & 144.+c(4,3,3,n)*z3*144.+c(4,3,4,n)*z4*144.+c(4,3,5,n)*z5*144.+
     & c(4,4,0,n)*y1*576.+c(4,4,1,n)*y1z1*576.+c(4,4,2,n)*y1z2*576.+c(
     & 4,4,3,n)*y1z3*576.+c(4,4,4,n)*y1z4*576.+c(4,4,5,n)*y1z5*576.+c(
     & 4,5,0,n)*y2*1440.+c(4,5,1,n)*y2z1*1440.+c(4,5,2,n)*y2z2*1440.+
     & c(4,5,3,n)*y2z3*1440.+c(4,5,4,n)*y2z4*1440.+c(4,5,5,n)*y2z5*
     & 1440.+c(5,3,0,n)*x1*720.+c(5,3,1,n)*x1z1*720.+c(5,3,2,n)*x1z2*
     & 720.+c(5,3,3,n)*x1z3*720.+c(5,3,4,n)*x1z4*720.+c(5,3,5,n)*x1z5*
     & 720.+c(5,4,0,n)*x1y1*2880.+c(5,4,1,n)*x1y1z1*2880.+c(5,4,2,n)*
     & x1y1z2*2880.+c(5,4,3,n)*x1y1z3*2880.+c(5,4,4,n)*x1y1z4*2880.+c(
     & 5,4,5,n)*x1y1z5*2880.+c(5,5,0,n)*x1y2*7200.+c(5,5,1,n)*x1y2z1*
     & 7200.+c(5,5,2,n)*x1y2z2*7200.+c(5,5,3,n)*x1y2z3*7200.+c(5,5,4,
     & n)*x1y2z4*7200.+c(5,5,5,n)*x1y2z5*7200.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.4.and.dy.eq.3.and.dz.eq.1 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y2z4=x1y1z4*y1
      r(i1,i2,i3,n)=(c(4,3,1,n)*144.+c(4,3,2,n)*z1*288.+c(4,3,3,n)*z2*
     & 432.+c(4,3,4,n)*z3*576.+c(4,3,5,n)*z4*720.+c(4,4,1,n)*y1*576.+
     & c(4,4,2,n)*y1z1*1152.+c(4,4,3,n)*y1z2*1728.+c(4,4,4,n)*y1z3*
     & 2304.+c(4,4,5,n)*y1z4*2880.+c(4,5,1,n)*y2*1440.+c(4,5,2,n)*
     & y2z1*2880.+c(4,5,3,n)*y2z2*4320.+c(4,5,4,n)*y2z3*5760.+c(4,5,5,
     & n)*y2z4*7200.+c(5,3,1,n)*x1*720.+c(5,3,2,n)*x1z1*1440.+c(5,3,3,
     & n)*x1z2*2160.+c(5,3,4,n)*x1z3*2880.+c(5,3,5,n)*x1z4*3600.+c(5,
     & 4,1,n)*x1y1*2880.+c(5,4,2,n)*x1y1z1*5760.+c(5,4,3,n)*x1y1z2*
     & 8640.+c(5,4,4,n)*x1y1z3*11520.+c(5,4,5,n)*x1y1z4*14400.+c(5,5,
     & 1,n)*x1y2*7200.+c(5,5,2,n)*x1y2z1*14400.+c(5,5,3,n)*x1y2z2*
     & 21600.+c(5,5,4,n)*x1y2z3*28800.+c(5,5,5,n)*x1y2z4*36000.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.4.and.dy.eq.3.and.dz.eq.2 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      r(i1,i2,i3,n)=(c(4,3,2,n)*288.+c(4,3,3,n)*z1*864.+c(4,3,4,n)*z2*
     & 1728.+c(4,3,5,n)*z3*2880.+c(4,4,2,n)*y1*1152.+c(4,4,3,n)*y1z1*
     & 3456.+c(4,4,4,n)*y1z2*6912.+c(4,4,5,n)*y1z3*11520.+c(4,5,2,n)*
     & y2*2880.+c(4,5,3,n)*y2z1*8640.+c(4,5,4,n)*y2z2*17280.+c(4,5,5,
     & n)*y2z3*28800.+c(5,3,2,n)*x1*1440.+c(5,3,3,n)*x1z1*4320.+c(5,3,
     & 4,n)*x1z2*8640.+c(5,3,5,n)*x1z3*14400.+c(5,4,2,n)*x1y1*5760.+c(
     & 5,4,3,n)*x1y1z1*17280.+c(5,4,4,n)*x1y1z2*34560.+c(5,4,5,n)*
     & x1y1z3*57600.+c(5,5,2,n)*x1y2*14400.+c(5,5,3,n)*x1y2z1*43200.+
     & c(5,5,4,n)*x1y2z2*86400.+c(5,5,5,n)*x1y2z3*144000.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.4.and.dy.eq.3.and.dz.eq.3 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      r(i1,i2,i3,n)=(c(4,3,3,n)*864.+c(4,3,4,n)*z1*3456.+c(4,3,5,n)*z2*
     & 8640.+c(4,4,3,n)*y1*3456.+c(4,4,4,n)*y1z1*13824.+c(4,4,5,n)*
     & y1z2*34560.+c(4,5,3,n)*y2*8640.+c(4,5,4,n)*y2z1*34560.+c(4,5,5,
     & n)*y2z2*86400.+c(5,3,3,n)*x1*4320.+c(5,3,4,n)*x1z1*17280.+c(5,
     & 3,5,n)*x1z2*43200.+c(5,4,3,n)*x1y1*17280.+c(5,4,4,n)*x1y1z1*
     & 69120.+c(5,4,5,n)*x1y1z2*172800.+c(5,5,3,n)*x1y2*43200.+c(5,5,
     & 4,n)*x1y2z1*172800.+c(5,5,5,n)*x1y2z2*432000.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.4.and.dy.eq.3.and.dz.eq.4 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      r(i1,i2,i3,n)=(c(4,3,4,n)*3456.+c(4,3,5,n)*z1*17280.+c(4,4,4,n)*
     & y1*13824.+c(4,4,5,n)*y1z1*69120.+c(4,5,4,n)*y2*34560.+c(4,5,5,
     & n)*y2z1*172800.+c(5,3,4,n)*x1*17280.+c(5,3,5,n)*x1z1*86400.+c(
     & 5,4,4,n)*x1y1*69120.+c(5,4,5,n)*x1y1z1*345600.+c(5,5,4,n)*x1y2*
     & 172800.+c(5,5,5,n)*x1y2z1*864000.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.4.and.dy.eq.4.and.dz.eq.0 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y1z5=y1z4*z1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1z5=x1z4*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y1z5=x1y1z4*z1
      r(i1,i2,i3,n)=(c(4,4,0,n)*576.+c(4,4,1,n)*z1*576.+c(4,4,2,n)*z2*
     & 576.+c(4,4,3,n)*z3*576.+c(4,4,4,n)*z4*576.+c(4,4,5,n)*z5*576.+
     & c(4,5,0,n)*y1*2880.+c(4,5,1,n)*y1z1*2880.+c(4,5,2,n)*y1z2*
     & 2880.+c(4,5,3,n)*y1z3*2880.+c(4,5,4,n)*y1z4*2880.+c(4,5,5,n)*
     & y1z5*2880.+c(5,4,0,n)*x1*2880.+c(5,4,1,n)*x1z1*2880.+c(5,4,2,n)
     & *x1z2*2880.+c(5,4,3,n)*x1z3*2880.+c(5,4,4,n)*x1z4*2880.+c(5,4,
     & 5,n)*x1z5*2880.+c(5,5,0,n)*x1y1*14400.+c(5,5,1,n)*x1y1z1*
     & 14400.+c(5,5,2,n)*x1y1z2*14400.+c(5,5,3,n)*x1y1z3*14400.+c(5,5,
     & 4,n)*x1y1z4*14400.+c(5,5,5,n)*x1y1z5*14400.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.4.and.dy.eq.4.and.dz.eq.1 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      r(i1,i2,i3,n)=(c(4,4,1,n)*576.+c(4,4,2,n)*z1*1152.+c(4,4,3,n)*z2*
     & 1728.+c(4,4,4,n)*z3*2304.+c(4,4,5,n)*z4*2880.+c(4,5,1,n)*y1*
     & 2880.+c(4,5,2,n)*y1z1*5760.+c(4,5,3,n)*y1z2*8640.+c(4,5,4,n)*
     & y1z3*11520.+c(4,5,5,n)*y1z4*14400.+c(5,4,1,n)*x1*2880.+c(5,4,2,
     & n)*x1z1*5760.+c(5,4,3,n)*x1z2*8640.+c(5,4,4,n)*x1z3*11520.+c(5,
     & 4,5,n)*x1z4*14400.+c(5,5,1,n)*x1y1*14400.+c(5,5,2,n)*x1y1z1*
     & 28800.+c(5,5,3,n)*x1y1z2*43200.+c(5,5,4,n)*x1y1z3*57600.+c(5,5,
     & 5,n)*x1y1z4*72000.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.4.and.dy.eq.4.and.dz.eq.2 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      r(i1,i2,i3,n)=(c(4,4,2,n)*1152.+c(4,4,3,n)*z1*3456.+c(4,4,4,n)*
     & z2*6912.+c(4,4,5,n)*z3*11520.+c(4,5,2,n)*y1*5760.+c(4,5,3,n)*
     & y1z1*17280.+c(4,5,4,n)*y1z2*34560.+c(4,5,5,n)*y1z3*57600.+c(5,
     & 4,2,n)*x1*5760.+c(5,4,3,n)*x1z1*17280.+c(5,4,4,n)*x1z2*34560.+
     & c(5,4,5,n)*x1z3*57600.+c(5,5,2,n)*x1y1*28800.+c(5,5,3,n)*
     & x1y1z1*86400.+c(5,5,4,n)*x1y1z2*172800.+c(5,5,5,n)*x1y1z3*
     & 288000.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.4.and.dy.eq.4.and.dz.eq.3 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      r(i1,i2,i3,n)=(c(4,4,3,n)*3456.+c(4,4,4,n)*z1*13824.+c(4,4,5,n)*
     & z2*34560.+c(4,5,3,n)*y1*17280.+c(4,5,4,n)*y1z1*69120.+c(4,5,5,
     & n)*y1z2*172800.+c(5,4,3,n)*x1*17280.+c(5,4,4,n)*x1z1*69120.+c(
     & 5,4,5,n)*x1z2*172800.+c(5,5,3,n)*x1y1*86400.+c(5,5,4,n)*x1y1z1*
     & 345600.+c(5,5,5,n)*x1y1z2*864000.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.eq.4.and.dy.eq.4.and.dz.eq.4 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      r(i1,i2,i3,n)=(c(4,4,4,n)*13824.+c(4,4,5,n)*z1*69120.+c(4,5,4,n)*
     & y1*69120.+c(4,5,5,n)*y1z1*345600.+c(5,4,4,n)*x1*69120.+c(5,4,5,
     & n)*x1z1*345600.+c(5,5,4,n)*x1y1*345600.+c(5,5,5,n)*x1y1z1*
     & 1728000.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( laplace.eq.1 )then
! beginLoops($defineTime())
      do n=nca,ncb
! defineTime()
      if( dt.eq.0 )then
       if( degreeTime.eq.0 )then
        time=a(0,n)
       else if( degreeTime.eq.1 )then
        time=a(0,n)+t*(a(1,n))
       else if( degreeTime.eq.2 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
       else if( degreeTime.eq.3 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
       else if( degreeTime.eq.4 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
       else if( degreeTime.eq.5 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))
     & )))
       else if( degreeTime.eq.6 )then
        time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+
     & t*(a(6,n)))))))
       else
        write(*,*) 'ERROR invalid degreeTime'
        stop
       end if
      else if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      do i3=nta,ntb
      do i2=nsa,nsb
      do i1=nra,nrb
      z1=za(i1,i2,i3)
      z2=z1*z1
      z3=z2*z1
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y4z2=y3z2*y1
      y4z3=y3z3*y1
      y5=y4*y1
      y5z1=y4z1*y1
      y5z2=y4z2*y1
      y5z3=y4z3*y1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y4z2=x1y3z2*y1
      x1y4z3=x1y3z3*y1
      x1y5=x1y4*y1
      x1y5z1=x1y4z1*y1
      x1y5z2=x1y4z2*y1
      x1y5z3=x1y4z3*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y3z3=x1y3z3*x1
      x2y4=x1y4*x1
      x2y4z1=x1y4z1*x1
      x2y4z2=x1y4z2*x1
      x2y4z3=x1y4z3*x1
      x2y5=x1y5*x1
      x2y5z1=x1y5z1*x1
      x2y5z2=x1y5z2*x1
      x2y5z3=x1y5z3*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y2z3=x2y2z3*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x3y3z2=x2y3z2*x1
      x3y3z3=x2y3z3*x1
      x3y4=x2y4*x1
      x3y4z1=x2y4z1*x1
      x3y4z2=x2y4z2*x1
      x3y4z3=x2y4z3*x1
      x3y5=x2y5*x1
      x3y5z1=x2y5z1*x1
      x3y5z2=x2y5z2*x1
      x3y5z3=x2y5z3*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4z2=x3z2*x1
      x4z3=x3z3*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y1z2=x3y1z2*x1
      x4y1z3=x3y1z3*x1
      x4y2=x3y2*x1
      x4y2z1=x3y2z1*x1
      x4y2z2=x3y2z2*x1
      x4y2z3=x3y2z3*x1
      x4y3=x3y3*x1
      x4y3z1=x3y3z1*x1
      x4y3z2=x3y3z2*x1
      x4y3z3=x3y3z3*x1
      x4y4=x3y4*x1
      x4y4z1=x3y4z1*x1
      x4y4z2=x3y4z2*x1
      x4y4z3=x3y4z3*x1
      x4y5=x3y5*x1
      x4y5z1=x3y5z1*x1
      x4y5z2=x3y5z2*x1
      x4y5z3=x3y5z3*x1
      x5=x4*x1
      x5z1=x4z1*x1
      x5z2=x4z2*x1
      x5z3=x4z3*x1
      x5y1=x4y1*x1
      x5y1z1=x4y1z1*x1
      x5y1z2=x4y1z2*x1
      x5y1z3=x4y1z3*x1
      x5y2=x4y2*x1
      x5y2z1=x4y2z1*x1
      x5y2z2=x4y2z2*x1
      x5y2z3=x4y2z3*x1
      x5y3=x4y3*x1
      x5y3z1=x4y3z1*x1
      x5y3z2=x4y3z2*x1
      x5y3z3=x4y3z3*x1
      x5y4=x4y4*x1
      x5y4z1=x4y4z1*x1
      x5y4z2=x4y4z2*x1
      x5y4z3=x4y4z3*x1
      x5y5=x4y5*x1
      x5y5z1=x4y5z1*x1
      x5y5z2=x4y5z2*x1
      x5y5z3=x4y5z3*x1
      z4=z3*z1
      z5=z4*z1
      y1z4=y1z3*z1
      y1z5=y1z4*z1
      y2z4=y1z4*y1
      y2z5=y1z5*y1
      y3z4=y2z4*y1
      y3z5=y2z5*y1
      x1z4=x1z3*z1
      x1z5=x1z4*z1
      x1y1z4=x1y1z3*z1
      x1y1z5=x1y1z4*z1
      x1y2z4=x1y1z4*y1
      x1y2z5=x1y1z5*y1
      x1y3z4=x1y2z4*y1
      x1y3z5=x1y2z5*y1
      x2z4=x1z4*x1
      x2z5=x1z5*x1
      x2y1z4=x1y1z4*x1
      x2y1z5=x1y1z5*x1
      x2y2z4=x1y2z4*x1
      x2y2z5=x1y2z5*x1
      x2y3z4=x1y3z4*x1
      x2y3z5=x1y3z5*x1
      x3z4=x2z4*x1
      x3z5=x2z5*x1
      x3y1z4=x2y1z4*x1
      x3y1z5=x2y1z5*x1
      x3y2z4=x2y2z4*x1
      x3y2z5=x2y2z5*x1
      x3y3z4=x2y3z4*x1
      x3y3z5=x2y3z5*x1
      x4z4=x3z4*x1
      x4z5=x3z5*x1
      x4y1z4=x3y1z4*x1
      x4y1z5=x3y1z5*x1
      x4y2z4=x3y2z4*x1
      x4y2z5=x3y2z5*x1
      x4y3z4=x3y3z4*x1
      x4y3z5=x3y3z5*x1
      x5z4=x4z4*x1
      x5z5=x4z5*x1
      x5y1z4=x4y1z4*x1
      x5y1z5=x4y1z5*x1
      x5y2z4=x4y2z4*x1
      x5y2z5=x4y2z5*x1
      x5y3z4=x4y3z4*x1
      x5y3z5=x4y3z5*x1
      y4z4=y3z4*y1
      y4z5=y3z5*y1
      y5z4=y4z4*y1
      y5z5=y4z5*y1
      x1y4z4=x1y3z4*y1
      x1y4z5=x1y3z5*y1
      x1y5z4=x1y4z4*y1
      x1y5z5=x1y4z5*y1
      x2y4z4=x1y4z4*x1
      x2y4z5=x1y4z5*x1
      x2y5z4=x1y5z4*x1
      x2y5z5=x1y5z5*x1
      x3y4z4=x2y4z4*x1
      x3y4z5=x2y4z5*x1
      x3y5z4=x2y5z4*x1
      x3y5z5=x2y5z5*x1
      r(i1,i2,i3,n)=(+c(0,0,2,n)*2.+c(0,0,3,n)*z1*6.+c(0,0,4,n)*z2*12.+
     & c(0,0,5,n)*z3*20.+c(0,1,2,n)*y1*2.+c(0,1,3,n)*y1z1*6.+c(0,1,4,
     & n)*y1z2*12.+c(0,1,5,n)*y1z3*20.+c(0,2,2,n)*y2*2.+c(0,2,3,n)*
     & y2z1*6.+c(0,2,4,n)*y2z2*12.+c(0,2,5,n)*y2z3*20.+c(0,3,2,n)*y3*
     & 2.+c(0,3,3,n)*y3z1*6.+c(0,3,4,n)*y3z2*12.+c(0,3,5,n)*y3z3*20.+
     & c(0,4,2,n)*y4*2.+c(0,4,3,n)*y4z1*6.+c(0,4,4,n)*y4z2*12.+c(0,4,
     & 5,n)*y4z3*20.+c(0,5,2,n)*y5*2.+c(0,5,3,n)*y5z1*6.+c(0,5,4,n)*
     & y5z2*12.+c(0,5,5,n)*y5z3*20.+c(1,0,2,n)*x1*2.+c(1,0,3,n)*x1z1*
     & 6.+c(1,0,4,n)*x1z2*12.+c(1,0,5,n)*x1z3*20.+c(1,1,2,n)*x1y1*2.+
     & c(1,1,3,n)*x1y1z1*6.+c(1,1,4,n)*x1y1z2*12.+c(1,1,5,n)*x1y1z3*
     & 20.+c(1,2,2,n)*x1y2*2.+c(1,2,3,n)*x1y2z1*6.+c(1,2,4,n)*x1y2z2*
     & 12.+c(1,2,5,n)*x1y2z3*20.+c(1,3,2,n)*x1y3*2.+c(1,3,3,n)*x1y3z1*
     & 6.+c(1,3,4,n)*x1y3z2*12.+c(1,3,5,n)*x1y3z3*20.+c(1,4,2,n)*x1y4*
     & 2.+c(1,4,3,n)*x1y4z1*6.+c(1,4,4,n)*x1y4z2*12.+c(1,4,5,n)*
     & x1y4z3*20.+c(1,5,2,n)*x1y5*2.+c(1,5,3,n)*x1y5z1*6.+c(1,5,4,n)*
     & x1y5z2*12.+c(1,5,5,n)*x1y5z3*20.+c(2,0,2,n)*x2*2.+c(2,0,3,n)*
     & x2z1*6.+c(2,0,4,n)*x2z2*12.+c(2,0,5,n)*x2z3*20.+c(2,1,2,n)*
     & x2y1*2.+c(2,1,3,n)*x2y1z1*6.+c(2,1,4,n)*x2y1z2*12.+c(2,1,5,n)*
     & x2y1z3*20.+c(2,2,2,n)*x2y2*2.+c(2,2,3,n)*x2y2z1*6.+c(2,2,4,n)*
     & x2y2z2*12.+c(2,2,5,n)*x2y2z3*20.+c(2,3,2,n)*x2y3*2.+c(2,3,3,n)*
     & x2y3z1*6.+c(2,3,4,n)*x2y3z2*12.+c(2,3,5,n)*x2y3z3*20.+c(2,4,2,
     & n)*x2y4*2.+c(2,4,3,n)*x2y4z1*6.+c(2,4,4,n)*x2y4z2*12.+c(2,4,5,
     & n)*x2y4z3*20.+c(2,5,2,n)*x2y5*2.+c(2,5,3,n)*x2y5z1*6.+c(2,5,4,
     & n)*x2y5z2*12.+c(2,5,5,n)*x2y5z3*20.+c(3,0,2,n)*x3*2.+c(3,0,3,n)
     & *x3z1*6.+c(3,0,4,n)*x3z2*12.+c(3,0,5,n)*x3z3*20.+c(3,1,2,n)*
     & x3y1*2.+c(3,1,3,n)*x3y1z1*6.+c(3,1,4,n)*x3y1z2*12.+c(3,1,5,n)*
     & x3y1z3*20.+c(3,2,2,n)*x3y2*2.+c(3,2,3,n)*x3y2z1*6.+c(3,2,4,n)*
     & x3y2z2*12.+c(3,2,5,n)*x3y2z3*20.+c(3,3,2,n)*x3y3*2.+c(3,3,3,n)*
     & x3y3z1*6.+c(3,3,4,n)*x3y3z2*12.+c(3,3,5,n)*x3y3z3*20.+c(3,4,2,
     & n)*x3y4*2.+c(3,4,3,n)*x3y4z1*6.+c(3,4,4,n)*x3y4z2*12.+c(3,4,5,
     & n)*x3y4z3*20.+c(3,5,2,n)*x3y5*2.+c(3,5,3,n)*x3y5z1*6.+c(3,5,4,
     & n)*x3y5z2*12.+c(3,5,5,n)*x3y5z3*20.+c(4,0,2,n)*x4*2.+c(4,0,3,n)
     & *x4z1*6.+c(4,0,4,n)*x4z2*12.+c(4,0,5,n)*x4z3*20.+c(4,1,2,n)*
     & x4y1*2.+c(4,1,3,n)*x4y1z1*6.+c(4,1,4,n)*x4y1z2*12.+c(4,1,5,n)*
     & x4y1z3*20.+c(4,2,2,n)*x4y2*2.+c(4,2,3,n)*x4y2z1*6.+c(4,2,4,n)*
     & x4y2z2*12.+c(4,2,5,n)*x4y2z3*20.+c(4,3,2,n)*x4y3*2.+c(4,3,3,n)*
     & x4y3z1*6.+c(4,3,4,n)*x4y3z2*12.+c(4,3,5,n)*x4y3z3*20.+c(4,4,2,
     & n)*x4y4*2.+c(4,4,3,n)*x4y4z1*6.+c(4,4,4,n)*x4y4z2*12.+c(4,4,5,
     & n)*x4y4z3*20.+c(4,5,2,n)*x4y5*2.+c(4,5,3,n)*x4y5z1*6.+c(4,5,4,
     & n)*x4y5z2*12.+c(4,5,5,n)*x4y5z3*20.+c(5,0,2,n)*x5*2.+c(5,0,3,n)
     & *x5z1*6.+c(5,0,4,n)*x5z2*12.+c(5,0,5,n)*x5z3*20.+c(5,1,2,n)*
     & x5y1*2.+c(5,1,3,n)*x5y1z1*6.+c(5,1,4,n)*x5y1z2*12.+c(5,1,5,n)*
     & x5y1z3*20.+c(5,2,2,n)*x5y2*2.+c(5,2,3,n)*x5y2z1*6.+c(5,2,4,n)*
     & x5y2z2*12.+c(5,2,5,n)*x5y2z3*20.+c(5,3,2,n)*x5y3*2.+c(5,3,3,n)*
     & x5y3z1*6.+c(5,3,4,n)*x5y3z2*12.+c(5,3,5,n)*x5y3z3*20.+c(5,4,2,
     & n)*x5y4*2.+c(5,4,3,n)*x5y4z1*6.+c(5,4,4,n)*x5y4z2*12.+c(5,4,5,
     & n)*x5y4z3*20.+c(5,5,2,n)*x5y5*2.+c(5,5,3,n)*x5y5z1*6.+c(5,5,4,
     & n)*x5y5z2*12.+c(5,5,5,n)*x5y5z3*20. )
       r(i1,i2,i3,n)=(r(i1,i2,i3,n)+c(0,2,0,n)*2.+c(0,2,1,n)*z1*2.+c(0,
     & 2,2,n)*z2*2.+c(0,2,3,n)*z3*2.+c(0,2,4,n)*z4*2.+c(0,2,5,n)*z5*
     & 2.+c(0,3,0,n)*y1*6.+c(0,3,1,n)*y1z1*6.+c(0,3,2,n)*y1z2*6.+c(0,
     & 3,3,n)*y1z3*6.+c(0,3,4,n)*y1z4*6.+c(0,3,5,n)*y1z5*6.+c(0,4,0,n)
     & *y2*12.+c(0,4,1,n)*y2z1*12.+c(0,4,2,n)*y2z2*12.+c(0,4,3,n)*
     & y2z3*12.+c(0,4,4,n)*y2z4*12.+c(0,4,5,n)*y2z5*12.+c(0,5,0,n)*y3*
     & 20.+c(0,5,1,n)*y3z1*20.+c(0,5,2,n)*y3z2*20.+c(0,5,3,n)*y3z3*
     & 20.+c(0,5,4,n)*y3z4*20.+c(0,5,5,n)*y3z5*20.+c(1,2,0,n)*x1*2.+c(
     & 1,2,1,n)*x1z1*2.+c(1,2,2,n)*x1z2*2.+c(1,2,3,n)*x1z3*2.+c(1,2,4,
     & n)*x1z4*2.+c(1,2,5,n)*x1z5*2.+c(1,3,0,n)*x1y1*6.+c(1,3,1,n)*
     & x1y1z1*6.+c(1,3,2,n)*x1y1z2*6.+c(1,3,3,n)*x1y1z3*6.+c(1,3,4,n)*
     & x1y1z4*6.+c(1,3,5,n)*x1y1z5*6.+c(1,4,0,n)*x1y2*12.+c(1,4,1,n)*
     & x1y2z1*12.+c(1,4,2,n)*x1y2z2*12.+c(1,4,3,n)*x1y2z3*12.+c(1,4,4,
     & n)*x1y2z4*12.+c(1,4,5,n)*x1y2z5*12.+c(1,5,0,n)*x1y3*20.+c(1,5,
     & 1,n)*x1y3z1*20.+c(1,5,2,n)*x1y3z2*20.+c(1,5,3,n)*x1y3z3*20.+c(
     & 1,5,4,n)*x1y3z4*20.+c(1,5,5,n)*x1y3z5*20.+c(2,2,0,n)*x2*2.+c(2,
     & 2,1,n)*x2z1*2.+c(2,2,2,n)*x2z2*2.+c(2,2,3,n)*x2z3*2.+c(2,2,4,n)
     & *x2z4*2.+c(2,2,5,n)*x2z5*2.+c(2,3,0,n)*x2y1*6.+c(2,3,1,n)*
     & x2y1z1*6.+c(2,3,2,n)*x2y1z2*6.+c(2,3,3,n)*x2y1z3*6.+c(2,3,4,n)*
     & x2y1z4*6.+c(2,3,5,n)*x2y1z5*6.+c(2,4,0,n)*x2y2*12.+c(2,4,1,n)*
     & x2y2z1*12.+c(2,4,2,n)*x2y2z2*12.+c(2,4,3,n)*x2y2z3*12.+c(2,4,4,
     & n)*x2y2z4*12.+c(2,4,5,n)*x2y2z5*12.+c(2,5,0,n)*x2y3*20.+c(2,5,
     & 1,n)*x2y3z1*20.+c(2,5,2,n)*x2y3z2*20.+c(2,5,3,n)*x2y3z3*20.+c(
     & 2,5,4,n)*x2y3z4*20.+c(2,5,5,n)*x2y3z5*20.+c(3,2,0,n)*x3*2.+c(3,
     & 2,1,n)*x3z1*2.+c(3,2,2,n)*x3z2*2.+c(3,2,3,n)*x3z3*2.+c(3,2,4,n)
     & *x3z4*2.+c(3,2,5,n)*x3z5*2.+c(3,3,0,n)*x3y1*6.+c(3,3,1,n)*
     & x3y1z1*6.+c(3,3,2,n)*x3y1z2*6.+c(3,3,3,n)*x3y1z3*6.+c(3,3,4,n)*
     & x3y1z4*6.+c(3,3,5,n)*x3y1z5*6.+c(3,4,0,n)*x3y2*12.+c(3,4,1,n)*
     & x3y2z1*12.+c(3,4,2,n)*x3y2z2*12.+c(3,4,3,n)*x3y2z3*12.+c(3,4,4,
     & n)*x3y2z4*12.+c(3,4,5,n)*x3y2z5*12.+c(3,5,0,n)*x3y3*20.+c(3,5,
     & 1,n)*x3y3z1*20.+c(3,5,2,n)*x3y3z2*20.+c(3,5,3,n)*x3y3z3*20.+c(
     & 3,5,4,n)*x3y3z4*20.+c(3,5,5,n)*x3y3z5*20.+c(4,2,0,n)*x4*2.+c(4,
     & 2,1,n)*x4z1*2.+c(4,2,2,n)*x4z2*2.+c(4,2,3,n)*x4z3*2.+c(4,2,4,n)
     & *x4z4*2.+c(4,2,5,n)*x4z5*2.+c(4,3,0,n)*x4y1*6.+c(4,3,1,n)*
     & x4y1z1*6.+c(4,3,2,n)*x4y1z2*6.+c(4,3,3,n)*x4y1z3*6.+c(4,3,4,n)*
     & x4y1z4*6.+c(4,3,5,n)*x4y1z5*6.+c(4,4,0,n)*x4y2*12.+c(4,4,1,n)*
     & x4y2z1*12.+c(4,4,2,n)*x4y2z2*12.+c(4,4,3,n)*x4y2z3*12.+c(4,4,4,
     & n)*x4y2z4*12.+c(4,4,5,n)*x4y2z5*12.+c(4,5,0,n)*x4y3*20.+c(4,5,
     & 1,n)*x4y3z1*20.+c(4,5,2,n)*x4y3z2*20.+c(4,5,3,n)*x4y3z3*20.+c(
     & 4,5,4,n)*x4y3z4*20.+c(4,5,5,n)*x4y3z5*20.+c(5,2,0,n)*x5*2.+c(5,
     & 2,1,n)*x5z1*2.+c(5,2,2,n)*x5z2*2.+c(5,2,3,n)*x5z3*2.+c(5,2,4,n)
     & *x5z4*2.+c(5,2,5,n)*x5z5*2.+c(5,3,0,n)*x5y1*6.+c(5,3,1,n)*
     & x5y1z1*6.+c(5,3,2,n)*x5y1z2*6.+c(5,3,3,n)*x5y1z3*6.+c(5,3,4,n)*
     & x5y1z4*6.+c(5,3,5,n)*x5y1z5*6.+c(5,4,0,n)*x5y2*12.+c(5,4,1,n)*
     & x5y2z1*12.+c(5,4,2,n)*x5y2z2*12.+c(5,4,3,n)*x5y2z3*12.+c(5,4,4,
     & n)*x5y2z4*12.+c(5,4,5,n)*x5y2z5*12.+c(5,5,0,n)*x5y3*20.+c(5,5,
     & 1,n)*x5y3z1*20.+c(5,5,2,n)*x5y3z2*20.+c(5,5,3,n)*x5y3z3*20.+c(
     & 5,5,4,n)*x5y3z4*20.+c(5,5,5,n)*x5y3z5*20. )
       r(i1,i2,i3,n)=(r(i1,i2,i3,n)+c(2,0,0,n)*2.+c(2,0,1,n)*z1*2.+c(2,
     & 0,2,n)*z2*2.+c(2,0,3,n)*z3*2.+c(2,0,4,n)*z4*2.+c(2,0,5,n)*z5*
     & 2.+c(2,1,0,n)*y1*2.+c(2,1,1,n)*y1z1*2.+c(2,1,2,n)*y1z2*2.+c(2,
     & 1,3,n)*y1z3*2.+c(2,1,4,n)*y1z4*2.+c(2,1,5,n)*y1z5*2.+c(2,2,0,n)
     & *y2*2.+c(2,2,1,n)*y2z1*2.+c(2,2,2,n)*y2z2*2.+c(2,2,3,n)*y2z3*
     & 2.+c(2,2,4,n)*y2z4*2.+c(2,2,5,n)*y2z5*2.+c(2,3,0,n)*y3*2.+c(2,
     & 3,1,n)*y3z1*2.+c(2,3,2,n)*y3z2*2.+c(2,3,3,n)*y3z3*2.+c(2,3,4,n)
     & *y3z4*2.+c(2,3,5,n)*y3z5*2.+c(2,4,0,n)*y4*2.+c(2,4,1,n)*y4z1*
     & 2.+c(2,4,2,n)*y4z2*2.+c(2,4,3,n)*y4z3*2.+c(2,4,4,n)*y4z4*2.+c(
     & 2,4,5,n)*y4z5*2.+c(2,5,0,n)*y5*2.+c(2,5,1,n)*y5z1*2.+c(2,5,2,n)
     & *y5z2*2.+c(2,5,3,n)*y5z3*2.+c(2,5,4,n)*y5z4*2.+c(2,5,5,n)*y5z5*
     & 2.+c(3,0,0,n)*x1*6.+c(3,0,1,n)*x1z1*6.+c(3,0,2,n)*x1z2*6.+c(3,
     & 0,3,n)*x1z3*6.+c(3,0,4,n)*x1z4*6.+c(3,0,5,n)*x1z5*6.+c(3,1,0,n)
     & *x1y1*6.+c(3,1,1,n)*x1y1z1*6.+c(3,1,2,n)*x1y1z2*6.+c(3,1,3,n)*
     & x1y1z3*6.+c(3,1,4,n)*x1y1z4*6.+c(3,1,5,n)*x1y1z5*6.+c(3,2,0,n)*
     & x1y2*6.+c(3,2,1,n)*x1y2z1*6.+c(3,2,2,n)*x1y2z2*6.+c(3,2,3,n)*
     & x1y2z3*6.+c(3,2,4,n)*x1y2z4*6.+c(3,2,5,n)*x1y2z5*6.+c(3,3,0,n)*
     & x1y3*6.+c(3,3,1,n)*x1y3z1*6.+c(3,3,2,n)*x1y3z2*6.+c(3,3,3,n)*
     & x1y3z3*6.+c(3,3,4,n)*x1y3z4*6.+c(3,3,5,n)*x1y3z5*6.+c(3,4,0,n)*
     & x1y4*6.+c(3,4,1,n)*x1y4z1*6.+c(3,4,2,n)*x1y4z2*6.+c(3,4,3,n)*
     & x1y4z3*6.+c(3,4,4,n)*x1y4z4*6.+c(3,4,5,n)*x1y4z5*6.+c(3,5,0,n)*
     & x1y5*6.+c(3,5,1,n)*x1y5z1*6.+c(3,5,2,n)*x1y5z2*6.+c(3,5,3,n)*
     & x1y5z3*6.+c(3,5,4,n)*x1y5z4*6.+c(3,5,5,n)*x1y5z5*6.+c(4,0,0,n)*
     & x2*12.+c(4,0,1,n)*x2z1*12.+c(4,0,2,n)*x2z2*12.+c(4,0,3,n)*x2z3*
     & 12.+c(4,0,4,n)*x2z4*12.+c(4,0,5,n)*x2z5*12.+c(4,1,0,n)*x2y1*
     & 12.+c(4,1,1,n)*x2y1z1*12.+c(4,1,2,n)*x2y1z2*12.+c(4,1,3,n)*
     & x2y1z3*12.+c(4,1,4,n)*x2y1z4*12.+c(4,1,5,n)*x2y1z5*12.+c(4,2,0,
     & n)*x2y2*12.+c(4,2,1,n)*x2y2z1*12.+c(4,2,2,n)*x2y2z2*12.+c(4,2,
     & 3,n)*x2y2z3*12.+c(4,2,4,n)*x2y2z4*12.+c(4,2,5,n)*x2y2z5*12.+c(
     & 4,3,0,n)*x2y3*12.+c(4,3,1,n)*x2y3z1*12.+c(4,3,2,n)*x2y3z2*12.+
     & c(4,3,3,n)*x2y3z3*12.+c(4,3,4,n)*x2y3z4*12.+c(4,3,5,n)*x2y3z5*
     & 12.+c(4,4,0,n)*x2y4*12.+c(4,4,1,n)*x2y4z1*12.+c(4,4,2,n)*
     & x2y4z2*12.+c(4,4,3,n)*x2y4z3*12.+c(4,4,4,n)*x2y4z4*12.+c(4,4,5,
     & n)*x2y4z5*12.+c(4,5,0,n)*x2y5*12.+c(4,5,1,n)*x2y5z1*12.+c(4,5,
     & 2,n)*x2y5z2*12.+c(4,5,3,n)*x2y5z3*12.+c(4,5,4,n)*x2y5z4*12.+c(
     & 4,5,5,n)*x2y5z5*12.+c(5,0,0,n)*x3*20.+c(5,0,1,n)*x3z1*20.+c(5,
     & 0,2,n)*x3z2*20.+c(5,0,3,n)*x3z3*20.+c(5,0,4,n)*x3z4*20.+c(5,0,
     & 5,n)*x3z5*20.+c(5,1,0,n)*x3y1*20.+c(5,1,1,n)*x3y1z1*20.+c(5,1,
     & 2,n)*x3y1z2*20.+c(5,1,3,n)*x3y1z3*20.+c(5,1,4,n)*x3y1z4*20.+c(
     & 5,1,5,n)*x3y1z5*20.+c(5,2,0,n)*x3y2*20.+c(5,2,1,n)*x3y2z1*20.+
     & c(5,2,2,n)*x3y2z2*20.+c(5,2,3,n)*x3y2z3*20.+c(5,2,4,n)*x3y2z4*
     & 20.+c(5,2,5,n)*x3y2z5*20.+c(5,3,0,n)*x3y3*20.+c(5,3,1,n)*
     & x3y3z1*20.+c(5,3,2,n)*x3y3z2*20.+c(5,3,3,n)*x3y3z3*20.+c(5,3,4,
     & n)*x3y3z4*20.+c(5,3,5,n)*x3y3z5*20.+c(5,4,0,n)*x3y4*20.+c(5,4,
     & 1,n)*x3y4z1*20.+c(5,4,2,n)*x3y4z2*20.+c(5,4,3,n)*x3y4z3*20.+c(
     & 5,4,4,n)*x3y4z4*20.+c(5,4,5,n)*x3y4z5*20.+c(5,5,0,n)*x3y5*20.+
     & c(5,5,1,n)*x3y5z1*20.+c(5,5,2,n)*x3y5z2*20.+c(5,5,3,n)*x3y5z3*
     & 20.+c(5,5,4,n)*x3y5z4*20.+c(5,5,5,n)*x3y5z5*20.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.gt.5 .or. dy.gt.5 .or. dz.gt.5 )then
! beginLoops(time=0.)
       do n=nca,ncb
       time=0.
       do i3=nta,ntb
       do i2=nsa,nsb
       do i1=nra,nrb
        r(i1,i2,i3,n)=0.
! endLoops()
       end do
       end do
       end do
       end do
      else
       write(*,*) 'polyFunction:ERROR derivative not implemented'
       write(*,*) 'dx,dy,dz=',dx,dy,dz
       stop 6543
      end if
      return
      end
