! This file automatically generated from polyFunction.bf with bpp.
! polyFun(3D4)
      subroutine polyFunction3D4 (nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb,
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
! poly3D4(x1=xa(i1,i2,i3),y1=ya(i1,i2,i3),z1=za(i1,i2,i3),r(i1,i2,i3,n))
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
      r(i1,i2,i3,n)=(c(0,0,0,n)+c(0,0,1,n)*z1+c(0,0,2,n)*z2+c(0,0,3,n)*
     & z3+c(0,0,4,n)*z4+c(0,1,0,n)*y1+c(0,1,1,n)*y1z1+c(0,1,2,n)*y1z2+
     & c(0,1,3,n)*y1z3+c(0,1,4,n)*y1z4+c(0,2,0,n)*y2+c(0,2,1,n)*y2z1+
     & c(0,2,2,n)*y2z2+c(0,2,3,n)*y2z3+c(0,2,4,n)*y2z4+c(0,3,0,n)*y3+
     & c(0,3,1,n)*y3z1+c(0,3,2,n)*y3z2+c(0,3,3,n)*y3z3+c(0,3,4,n)*
     & y3z4+c(0,4,0,n)*y4+c(0,4,1,n)*y4z1+c(0,4,2,n)*y4z2+c(0,4,3,n)*
     & y4z3+c(0,4,4,n)*y4z4+c(1,0,0,n)*x1+c(1,0,1,n)*x1z1+c(1,0,2,n)*
     & x1z2+c(1,0,3,n)*x1z3+c(1,0,4,n)*x1z4+c(1,1,0,n)*x1y1+c(1,1,1,n)
     & *x1y1z1+c(1,1,2,n)*x1y1z2+c(1,1,3,n)*x1y1z3+c(1,1,4,n)*x1y1z4+
     & c(1,2,0,n)*x1y2+c(1,2,1,n)*x1y2z1+c(1,2,2,n)*x1y2z2+c(1,2,3,n)*
     & x1y2z3+c(1,2,4,n)*x1y2z4+c(1,3,0,n)*x1y3+c(1,3,1,n)*x1y3z1+c(1,
     & 3,2,n)*x1y3z2+c(1,3,3,n)*x1y3z3+c(1,3,4,n)*x1y3z4+c(1,4,0,n)*
     & x1y4+c(1,4,1,n)*x1y4z1+c(1,4,2,n)*x1y4z2+c(1,4,3,n)*x1y4z3+c(1,
     & 4,4,n)*x1y4z4+c(2,0,0,n)*x2+c(2,0,1,n)*x2z1+c(2,0,2,n)*x2z2+c(
     & 2,0,3,n)*x2z3+c(2,0,4,n)*x2z4+c(2,1,0,n)*x2y1+c(2,1,1,n)*
     & x2y1z1+c(2,1,2,n)*x2y1z2+c(2,1,3,n)*x2y1z3+c(2,1,4,n)*x2y1z4+c(
     & 2,2,0,n)*x2y2+c(2,2,1,n)*x2y2z1+c(2,2,2,n)*x2y2z2+c(2,2,3,n)*
     & x2y2z3+c(2,2,4,n)*x2y2z4+c(2,3,0,n)*x2y3+c(2,3,1,n)*x2y3z1+c(2,
     & 3,2,n)*x2y3z2+c(2,3,3,n)*x2y3z3+c(2,3,4,n)*x2y3z4+c(2,4,0,n)*
     & x2y4+c(2,4,1,n)*x2y4z1+c(2,4,2,n)*x2y4z2+c(2,4,3,n)*x2y4z3+c(2,
     & 4,4,n)*x2y4z4+c(3,0,0,n)*x3+c(3,0,1,n)*x3z1+c(3,0,2,n)*x3z2+c(
     & 3,0,3,n)*x3z3+c(3,0,4,n)*x3z4+c(3,1,0,n)*x3y1+c(3,1,1,n)*
     & x3y1z1+c(3,1,2,n)*x3y1z2+c(3,1,3,n)*x3y1z3+c(3,1,4,n)*x3y1z4+c(
     & 3,2,0,n)*x3y2+c(3,2,1,n)*x3y2z1+c(3,2,2,n)*x3y2z2+c(3,2,3,n)*
     & x3y2z3+c(3,2,4,n)*x3y2z4+c(3,3,0,n)*x3y3+c(3,3,1,n)*x3y3z1+c(3,
     & 3,2,n)*x3y3z2+c(3,3,3,n)*x3y3z3+c(3,3,4,n)*x3y3z4+c(3,4,0,n)*
     & x3y4+c(3,4,1,n)*x3y4z1+c(3,4,2,n)*x3y4z2+c(3,4,3,n)*x3y4z3+c(3,
     & 4,4,n)*x3y4z4+c(4,0,0,n)*x4+c(4,0,1,n)*x4z1+c(4,0,2,n)*x4z2+c(
     & 4,0,3,n)*x4z3+c(4,0,4,n)*x4z4+c(4,1,0,n)*x4y1+c(4,1,1,n)*
     & x4y1z1+c(4,1,2,n)*x4y1z2+c(4,1,3,n)*x4y1z3+c(4,1,4,n)*x4y1z4+c(
     & 4,2,0,n)*x4y2+c(4,2,1,n)*x4y2z1+c(4,2,2,n)*x4y2z2+c(4,2,3,n)*
     & x4y2z3+c(4,2,4,n)*x4y2z4+c(4,3,0,n)*x4y3+c(4,3,1,n)*x4y3z1+c(4,
     & 3,2,n)*x4y3z2+c(4,3,3,n)*x4y3z3+c(4,3,4,n)*x4y3z4+c(4,4,0,n)*
     & x4y4+c(4,4,1,n)*x4y4z1+c(4,4,2,n)*x4y4z2+c(4,4,3,n)*x4y4z3+c(4,
     & 4,4,n)*x4y4z4)*time
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
      r(i1,i2,i3,n)=(c(0,0,1,n)+c(0,0,2,n)*z1*2.+c(0,0,3,n)*z2*3.+c(0,
     & 0,4,n)*z3*4.+c(0,1,1,n)*y1+c(0,1,2,n)*y1z1*2.+c(0,1,3,n)*y1z2*
     & 3.+c(0,1,4,n)*y1z3*4.+c(0,2,1,n)*y2+c(0,2,2,n)*y2z1*2.+c(0,2,3,
     & n)*y2z2*3.+c(0,2,4,n)*y2z3*4.+c(0,3,1,n)*y3+c(0,3,2,n)*y3z1*2.+
     & c(0,3,3,n)*y3z2*3.+c(0,3,4,n)*y3z3*4.+c(0,4,1,n)*y4+c(0,4,2,n)*
     & y4z1*2.+c(0,4,3,n)*y4z2*3.+c(0,4,4,n)*y4z3*4.+c(1,0,1,n)*x1+c(
     & 1,0,2,n)*x1z1*2.+c(1,0,3,n)*x1z2*3.+c(1,0,4,n)*x1z3*4.+c(1,1,1,
     & n)*x1y1+c(1,1,2,n)*x1y1z1*2.+c(1,1,3,n)*x1y1z2*3.+c(1,1,4,n)*
     & x1y1z3*4.+c(1,2,1,n)*x1y2+c(1,2,2,n)*x1y2z1*2.+c(1,2,3,n)*
     & x1y2z2*3.+c(1,2,4,n)*x1y2z3*4.+c(1,3,1,n)*x1y3+c(1,3,2,n)*
     & x1y3z1*2.+c(1,3,3,n)*x1y3z2*3.+c(1,3,4,n)*x1y3z3*4.+c(1,4,1,n)*
     & x1y4+c(1,4,2,n)*x1y4z1*2.+c(1,4,3,n)*x1y4z2*3.+c(1,4,4,n)*
     & x1y4z3*4.+c(2,0,1,n)*x2+c(2,0,2,n)*x2z1*2.+c(2,0,3,n)*x2z2*3.+
     & c(2,0,4,n)*x2z3*4.+c(2,1,1,n)*x2y1+c(2,1,2,n)*x2y1z1*2.+c(2,1,
     & 3,n)*x2y1z2*3.+c(2,1,4,n)*x2y1z3*4.+c(2,2,1,n)*x2y2+c(2,2,2,n)*
     & x2y2z1*2.+c(2,2,3,n)*x2y2z2*3.+c(2,2,4,n)*x2y2z3*4.+c(2,3,1,n)*
     & x2y3+c(2,3,2,n)*x2y3z1*2.+c(2,3,3,n)*x2y3z2*3.+c(2,3,4,n)*
     & x2y3z3*4.+c(2,4,1,n)*x2y4+c(2,4,2,n)*x2y4z1*2.+c(2,4,3,n)*
     & x2y4z2*3.+c(2,4,4,n)*x2y4z3*4.+c(3,0,1,n)*x3+c(3,0,2,n)*x3z1*
     & 2.+c(3,0,3,n)*x3z2*3.+c(3,0,4,n)*x3z3*4.+c(3,1,1,n)*x3y1+c(3,1,
     & 2,n)*x3y1z1*2.+c(3,1,3,n)*x3y1z2*3.+c(3,1,4,n)*x3y1z3*4.+c(3,2,
     & 1,n)*x3y2+c(3,2,2,n)*x3y2z1*2.+c(3,2,3,n)*x3y2z2*3.+c(3,2,4,n)*
     & x3y2z3*4.+c(3,3,1,n)*x3y3+c(3,3,2,n)*x3y3z1*2.+c(3,3,3,n)*
     & x3y3z2*3.+c(3,3,4,n)*x3y3z3*4.+c(3,4,1,n)*x3y4+c(3,4,2,n)*
     & x3y4z1*2.+c(3,4,3,n)*x3y4z2*3.+c(3,4,4,n)*x3y4z3*4.+c(4,0,1,n)*
     & x4+c(4,0,2,n)*x4z1*2.+c(4,0,3,n)*x4z2*3.+c(4,0,4,n)*x4z3*4.+c(
     & 4,1,1,n)*x4y1+c(4,1,2,n)*x4y1z1*2.+c(4,1,3,n)*x4y1z2*3.+c(4,1,
     & 4,n)*x4y1z3*4.+c(4,2,1,n)*x4y2+c(4,2,2,n)*x4y2z1*2.+c(4,2,3,n)*
     & x4y2z2*3.+c(4,2,4,n)*x4y2z3*4.+c(4,3,1,n)*x4y3+c(4,3,2,n)*
     & x4y3z1*2.+c(4,3,3,n)*x4y3z2*3.+c(4,3,4,n)*x4y3z3*4.+c(4,4,1,n)*
     & x4y4+c(4,4,2,n)*x4y4z1*2.+c(4,4,3,n)*x4y4z2*3.+c(4,4,4,n)*
     & x4y4z3*4.)*time
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
      r(i1,i2,i3,n)=(c(0,0,2,n)*2.+c(0,0,3,n)*z1*6.+c(0,0,4,n)*z2*12.+
     & c(0,1,2,n)*y1*2.+c(0,1,3,n)*y1z1*6.+c(0,1,4,n)*y1z2*12.+c(0,2,
     & 2,n)*y2*2.+c(0,2,3,n)*y2z1*6.+c(0,2,4,n)*y2z2*12.+c(0,3,2,n)*
     & y3*2.+c(0,3,3,n)*y3z1*6.+c(0,3,4,n)*y3z2*12.+c(0,4,2,n)*y4*2.+
     & c(0,4,3,n)*y4z1*6.+c(0,4,4,n)*y4z2*12.+c(1,0,2,n)*x1*2.+c(1,0,
     & 3,n)*x1z1*6.+c(1,0,4,n)*x1z2*12.+c(1,1,2,n)*x1y1*2.+c(1,1,3,n)*
     & x1y1z1*6.+c(1,1,4,n)*x1y1z2*12.+c(1,2,2,n)*x1y2*2.+c(1,2,3,n)*
     & x1y2z1*6.+c(1,2,4,n)*x1y2z2*12.+c(1,3,2,n)*x1y3*2.+c(1,3,3,n)*
     & x1y3z1*6.+c(1,3,4,n)*x1y3z2*12.+c(1,4,2,n)*x1y4*2.+c(1,4,3,n)*
     & x1y4z1*6.+c(1,4,4,n)*x1y4z2*12.+c(2,0,2,n)*x2*2.+c(2,0,3,n)*
     & x2z1*6.+c(2,0,4,n)*x2z2*12.+c(2,1,2,n)*x2y1*2.+c(2,1,3,n)*
     & x2y1z1*6.+c(2,1,4,n)*x2y1z2*12.+c(2,2,2,n)*x2y2*2.+c(2,2,3,n)*
     & x2y2z1*6.+c(2,2,4,n)*x2y2z2*12.+c(2,3,2,n)*x2y3*2.+c(2,3,3,n)*
     & x2y3z1*6.+c(2,3,4,n)*x2y3z2*12.+c(2,4,2,n)*x2y4*2.+c(2,4,3,n)*
     & x2y4z1*6.+c(2,4,4,n)*x2y4z2*12.+c(3,0,2,n)*x3*2.+c(3,0,3,n)*
     & x3z1*6.+c(3,0,4,n)*x3z2*12.+c(3,1,2,n)*x3y1*2.+c(3,1,3,n)*
     & x3y1z1*6.+c(3,1,4,n)*x3y1z2*12.+c(3,2,2,n)*x3y2*2.+c(3,2,3,n)*
     & x3y2z1*6.+c(3,2,4,n)*x3y2z2*12.+c(3,3,2,n)*x3y3*2.+c(3,3,3,n)*
     & x3y3z1*6.+c(3,3,4,n)*x3y3z2*12.+c(3,4,2,n)*x3y4*2.+c(3,4,3,n)*
     & x3y4z1*6.+c(3,4,4,n)*x3y4z2*12.+c(4,0,2,n)*x4*2.+c(4,0,3,n)*
     & x4z1*6.+c(4,0,4,n)*x4z2*12.+c(4,1,2,n)*x4y1*2.+c(4,1,3,n)*
     & x4y1z1*6.+c(4,1,4,n)*x4y1z2*12.+c(4,2,2,n)*x4y2*2.+c(4,2,3,n)*
     & x4y2z1*6.+c(4,2,4,n)*x4y2z2*12.+c(4,3,2,n)*x4y3*2.+c(4,3,3,n)*
     & x4y3z1*6.+c(4,3,4,n)*x4y3z2*12.+c(4,4,2,n)*x4y4*2.+c(4,4,3,n)*
     & x4y4z1*6.+c(4,4,4,n)*x4y4z2*12.)*time
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
      r(i1,i2,i3,n)=(c(0,0,3,n)*6.+c(0,0,4,n)*z1*24.+c(0,1,3,n)*y1*6.+
     & c(0,1,4,n)*y1z1*24.+c(0,2,3,n)*y2*6.+c(0,2,4,n)*y2z1*24.+c(0,3,
     & 3,n)*y3*6.+c(0,3,4,n)*y3z1*24.+c(0,4,3,n)*y4*6.+c(0,4,4,n)*
     & y4z1*24.+c(1,0,3,n)*x1*6.+c(1,0,4,n)*x1z1*24.+c(1,1,3,n)*x1y1*
     & 6.+c(1,1,4,n)*x1y1z1*24.+c(1,2,3,n)*x1y2*6.+c(1,2,4,n)*x1y2z1*
     & 24.+c(1,3,3,n)*x1y3*6.+c(1,3,4,n)*x1y3z1*24.+c(1,4,3,n)*x1y4*
     & 6.+c(1,4,4,n)*x1y4z1*24.+c(2,0,3,n)*x2*6.+c(2,0,4,n)*x2z1*24.+
     & c(2,1,3,n)*x2y1*6.+c(2,1,4,n)*x2y1z1*24.+c(2,2,3,n)*x2y2*6.+c(
     & 2,2,4,n)*x2y2z1*24.+c(2,3,3,n)*x2y3*6.+c(2,3,4,n)*x2y3z1*24.+c(
     & 2,4,3,n)*x2y4*6.+c(2,4,4,n)*x2y4z1*24.+c(3,0,3,n)*x3*6.+c(3,0,
     & 4,n)*x3z1*24.+c(3,1,3,n)*x3y1*6.+c(3,1,4,n)*x3y1z1*24.+c(3,2,3,
     & n)*x3y2*6.+c(3,2,4,n)*x3y2z1*24.+c(3,3,3,n)*x3y3*6.+c(3,3,4,n)*
     & x3y3z1*24.+c(3,4,3,n)*x3y4*6.+c(3,4,4,n)*x3y4z1*24.+c(4,0,3,n)*
     & x4*6.+c(4,0,4,n)*x4z1*24.+c(4,1,3,n)*x4y1*6.+c(4,1,4,n)*x4y1z1*
     & 24.+c(4,2,3,n)*x4y2*6.+c(4,2,4,n)*x4y2z1*24.+c(4,3,3,n)*x4y3*
     & 6.+c(4,3,4,n)*x4y3z1*24.+c(4,4,3,n)*x4y4*6.+c(4,4,4,n)*x4y4z1*
     & 24.)*time
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
      y1=ya(i1,i2,i3)
      y2=y1*y1
      y3=y2*y1
      y4=y3*y1
      x1=xa(i1,i2,i3)
      x1y1=x1*y1
      x1y2=x1y1*y1
      x1y3=x1y2*y1
      x1y4=x1y3*y1
      x2=x1*x1
      x2y1=x1y1*x1
      x2y2=x1y2*x1
      x2y3=x1y3*x1
      x2y4=x1y4*x1
      x3=x2*x1
      x3y1=x2y1*x1
      x3y2=x2y2*x1
      x3y3=x2y3*x1
      x3y4=x2y4*x1
      x4=x3*x1
      x4y1=x3y1*x1
      x4y2=x3y2*x1
      x4y3=x3y3*x1
      x4y4=x3y4*x1
      r(i1,i2,i3,n)=(c(0,0,4,n)*24.+c(0,1,4,n)*y1*24.+c(0,2,4,n)*y2*
     & 24.+c(0,3,4,n)*y3*24.+c(0,4,4,n)*y4*24.+c(1,0,4,n)*x1*24.+c(1,
     & 1,4,n)*x1y1*24.+c(1,2,4,n)*x1y2*24.+c(1,3,4,n)*x1y3*24.+c(1,4,
     & 4,n)*x1y4*24.+c(2,0,4,n)*x2*24.+c(2,1,4,n)*x2y1*24.+c(2,2,4,n)*
     & x2y2*24.+c(2,3,4,n)*x2y3*24.+c(2,4,4,n)*x2y4*24.+c(3,0,4,n)*x3*
     & 24.+c(3,1,4,n)*x3y1*24.+c(3,2,4,n)*x3y2*24.+c(3,3,4,n)*x3y3*
     & 24.+c(3,4,4,n)*x3y4*24.+c(4,0,4,n)*x4*24.+c(4,1,4,n)*x4y1*24.+
     & c(4,2,4,n)*x4y2*24.+c(4,3,4,n)*x4y3*24.+c(4,4,4,n)*x4y4*24.)*
     & time
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
      r(i1,i2,i3,n)=(c(0,1,0,n)+c(0,1,1,n)*z1+c(0,1,2,n)*z2+c(0,1,3,n)*
     & z3+c(0,1,4,n)*z4+c(0,2,0,n)*y1*2.+c(0,2,1,n)*y1z1*2.+c(0,2,2,n)
     & *y1z2*2.+c(0,2,3,n)*y1z3*2.+c(0,2,4,n)*y1z4*2.+c(0,3,0,n)*y2*
     & 3.+c(0,3,1,n)*y2z1*3.+c(0,3,2,n)*y2z2*3.+c(0,3,3,n)*y2z3*3.+c(
     & 0,3,4,n)*y2z4*3.+c(0,4,0,n)*y3*4.+c(0,4,1,n)*y3z1*4.+c(0,4,2,n)
     & *y3z2*4.+c(0,4,3,n)*y3z3*4.+c(0,4,4,n)*y3z4*4.+c(1,1,0,n)*x1+c(
     & 1,1,1,n)*x1z1+c(1,1,2,n)*x1z2+c(1,1,3,n)*x1z3+c(1,1,4,n)*x1z4+
     & c(1,2,0,n)*x1y1*2.+c(1,2,1,n)*x1y1z1*2.+c(1,2,2,n)*x1y1z2*2.+c(
     & 1,2,3,n)*x1y1z3*2.+c(1,2,4,n)*x1y1z4*2.+c(1,3,0,n)*x1y2*3.+c(1,
     & 3,1,n)*x1y2z1*3.+c(1,3,2,n)*x1y2z2*3.+c(1,3,3,n)*x1y2z3*3.+c(1,
     & 3,4,n)*x1y2z4*3.+c(1,4,0,n)*x1y3*4.+c(1,4,1,n)*x1y3z1*4.+c(1,4,
     & 2,n)*x1y3z2*4.+c(1,4,3,n)*x1y3z3*4.+c(1,4,4,n)*x1y3z4*4.+c(2,1,
     & 0,n)*x2+c(2,1,1,n)*x2z1+c(2,1,2,n)*x2z2+c(2,1,3,n)*x2z3+c(2,1,
     & 4,n)*x2z4+c(2,2,0,n)*x2y1*2.+c(2,2,1,n)*x2y1z1*2.+c(2,2,2,n)*
     & x2y1z2*2.+c(2,2,3,n)*x2y1z3*2.+c(2,2,4,n)*x2y1z4*2.+c(2,3,0,n)*
     & x2y2*3.+c(2,3,1,n)*x2y2z1*3.+c(2,3,2,n)*x2y2z2*3.+c(2,3,3,n)*
     & x2y2z3*3.+c(2,3,4,n)*x2y2z4*3.+c(2,4,0,n)*x2y3*4.+c(2,4,1,n)*
     & x2y3z1*4.+c(2,4,2,n)*x2y3z2*4.+c(2,4,3,n)*x2y3z3*4.+c(2,4,4,n)*
     & x2y3z4*4.+c(3,1,0,n)*x3+c(3,1,1,n)*x3z1+c(3,1,2,n)*x3z2+c(3,1,
     & 3,n)*x3z3+c(3,1,4,n)*x3z4+c(3,2,0,n)*x3y1*2.+c(3,2,1,n)*x3y1z1*
     & 2.+c(3,2,2,n)*x3y1z2*2.+c(3,2,3,n)*x3y1z3*2.+c(3,2,4,n)*x3y1z4*
     & 2.+c(3,3,0,n)*x3y2*3.+c(3,3,1,n)*x3y2z1*3.+c(3,3,2,n)*x3y2z2*
     & 3.+c(3,3,3,n)*x3y2z3*3.+c(3,3,4,n)*x3y2z4*3.+c(3,4,0,n)*x3y3*
     & 4.+c(3,4,1,n)*x3y3z1*4.+c(3,4,2,n)*x3y3z2*4.+c(3,4,3,n)*x3y3z3*
     & 4.+c(3,4,4,n)*x3y3z4*4.+c(4,1,0,n)*x4+c(4,1,1,n)*x4z1+c(4,1,2,
     & n)*x4z2+c(4,1,3,n)*x4z3+c(4,1,4,n)*x4z4+c(4,2,0,n)*x4y1*2.+c(4,
     & 2,1,n)*x4y1z1*2.+c(4,2,2,n)*x4y1z2*2.+c(4,2,3,n)*x4y1z3*2.+c(4,
     & 2,4,n)*x4y1z4*2.+c(4,3,0,n)*x4y2*3.+c(4,3,1,n)*x4y2z1*3.+c(4,3,
     & 2,n)*x4y2z2*3.+c(4,3,3,n)*x4y2z3*3.+c(4,3,4,n)*x4y2z4*3.+c(4,4,
     & 0,n)*x4y3*4.+c(4,4,1,n)*x4y3z1*4.+c(4,4,2,n)*x4y3z2*4.+c(4,4,3,
     & n)*x4y3z3*4.+c(4,4,4,n)*x4y3z4*4.)*time
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
      r(i1,i2,i3,n)=(c(0,1,1,n)+c(0,1,2,n)*z1*2.+c(0,1,3,n)*z2*3.+c(0,
     & 1,4,n)*z3*4.+c(0,2,1,n)*y1*2.+c(0,2,2,n)*y1z1*4.+c(0,2,3,n)*
     & y1z2*6.+c(0,2,4,n)*y1z3*8.+c(0,3,1,n)*y2*3.+c(0,3,2,n)*y2z1*6.+
     & c(0,3,3,n)*y2z2*9.+c(0,3,4,n)*y2z3*12.+c(0,4,1,n)*y3*4.+c(0,4,
     & 2,n)*y3z1*8.+c(0,4,3,n)*y3z2*12.+c(0,4,4,n)*y3z3*16.+c(1,1,1,n)
     & *x1+c(1,1,2,n)*x1z1*2.+c(1,1,3,n)*x1z2*3.+c(1,1,4,n)*x1z3*4.+c(
     & 1,2,1,n)*x1y1*2.+c(1,2,2,n)*x1y1z1*4.+c(1,2,3,n)*x1y1z2*6.+c(1,
     & 2,4,n)*x1y1z3*8.+c(1,3,1,n)*x1y2*3.+c(1,3,2,n)*x1y2z1*6.+c(1,3,
     & 3,n)*x1y2z2*9.+c(1,3,4,n)*x1y2z3*12.+c(1,4,1,n)*x1y3*4.+c(1,4,
     & 2,n)*x1y3z1*8.+c(1,4,3,n)*x1y3z2*12.+c(1,4,4,n)*x1y3z3*16.+c(2,
     & 1,1,n)*x2+c(2,1,2,n)*x2z1*2.+c(2,1,3,n)*x2z2*3.+c(2,1,4,n)*
     & x2z3*4.+c(2,2,1,n)*x2y1*2.+c(2,2,2,n)*x2y1z1*4.+c(2,2,3,n)*
     & x2y1z2*6.+c(2,2,4,n)*x2y1z3*8.+c(2,3,1,n)*x2y2*3.+c(2,3,2,n)*
     & x2y2z1*6.+c(2,3,3,n)*x2y2z2*9.+c(2,3,4,n)*x2y2z3*12.+c(2,4,1,n)
     & *x2y3*4.+c(2,4,2,n)*x2y3z1*8.+c(2,4,3,n)*x2y3z2*12.+c(2,4,4,n)*
     & x2y3z3*16.+c(3,1,1,n)*x3+c(3,1,2,n)*x3z1*2.+c(3,1,3,n)*x3z2*3.+
     & c(3,1,4,n)*x3z3*4.+c(3,2,1,n)*x3y1*2.+c(3,2,2,n)*x3y1z1*4.+c(3,
     & 2,3,n)*x3y1z2*6.+c(3,2,4,n)*x3y1z3*8.+c(3,3,1,n)*x3y2*3.+c(3,3,
     & 2,n)*x3y2z1*6.+c(3,3,3,n)*x3y2z2*9.+c(3,3,4,n)*x3y2z3*12.+c(3,
     & 4,1,n)*x3y3*4.+c(3,4,2,n)*x3y3z1*8.+c(3,4,3,n)*x3y3z2*12.+c(3,
     & 4,4,n)*x3y3z3*16.+c(4,1,1,n)*x4+c(4,1,2,n)*x4z1*2.+c(4,1,3,n)*
     & x4z2*3.+c(4,1,4,n)*x4z3*4.+c(4,2,1,n)*x4y1*2.+c(4,2,2,n)*
     & x4y1z1*4.+c(4,2,3,n)*x4y1z2*6.+c(4,2,4,n)*x4y1z3*8.+c(4,3,1,n)*
     & x4y2*3.+c(4,3,2,n)*x4y2z1*6.+c(4,3,3,n)*x4y2z2*9.+c(4,3,4,n)*
     & x4y2z3*12.+c(4,4,1,n)*x4y3*4.+c(4,4,2,n)*x4y3z1*8.+c(4,4,3,n)*
     & x4y3z2*12.+c(4,4,4,n)*x4y3z3*16.)*time
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
      r(i1,i2,i3,n)=(c(0,1,2,n)*2.+c(0,1,3,n)*z1*6.+c(0,1,4,n)*z2*12.+
     & c(0,2,2,n)*y1*4.+c(0,2,3,n)*y1z1*12.+c(0,2,4,n)*y1z2*24.+c(0,3,
     & 2,n)*y2*6.+c(0,3,3,n)*y2z1*18.+c(0,3,4,n)*y2z2*36.+c(0,4,2,n)*
     & y3*8.+c(0,4,3,n)*y3z1*24.+c(0,4,4,n)*y3z2*48.+c(1,1,2,n)*x1*2.+
     & c(1,1,3,n)*x1z1*6.+c(1,1,4,n)*x1z2*12.+c(1,2,2,n)*x1y1*4.+c(1,
     & 2,3,n)*x1y1z1*12.+c(1,2,4,n)*x1y1z2*24.+c(1,3,2,n)*x1y2*6.+c(1,
     & 3,3,n)*x1y2z1*18.+c(1,3,4,n)*x1y2z2*36.+c(1,4,2,n)*x1y3*8.+c(1,
     & 4,3,n)*x1y3z1*24.+c(1,4,4,n)*x1y3z2*48.+c(2,1,2,n)*x2*2.+c(2,1,
     & 3,n)*x2z1*6.+c(2,1,4,n)*x2z2*12.+c(2,2,2,n)*x2y1*4.+c(2,2,3,n)*
     & x2y1z1*12.+c(2,2,4,n)*x2y1z2*24.+c(2,3,2,n)*x2y2*6.+c(2,3,3,n)*
     & x2y2z1*18.+c(2,3,4,n)*x2y2z2*36.+c(2,4,2,n)*x2y3*8.+c(2,4,3,n)*
     & x2y3z1*24.+c(2,4,4,n)*x2y3z2*48.+c(3,1,2,n)*x3*2.+c(3,1,3,n)*
     & x3z1*6.+c(3,1,4,n)*x3z2*12.+c(3,2,2,n)*x3y1*4.+c(3,2,3,n)*
     & x3y1z1*12.+c(3,2,4,n)*x3y1z2*24.+c(3,3,2,n)*x3y2*6.+c(3,3,3,n)*
     & x3y2z1*18.+c(3,3,4,n)*x3y2z2*36.+c(3,4,2,n)*x3y3*8.+c(3,4,3,n)*
     & x3y3z1*24.+c(3,4,4,n)*x3y3z2*48.+c(4,1,2,n)*x4*2.+c(4,1,3,n)*
     & x4z1*6.+c(4,1,4,n)*x4z2*12.+c(4,2,2,n)*x4y1*4.+c(4,2,3,n)*
     & x4y1z1*12.+c(4,2,4,n)*x4y1z2*24.+c(4,3,2,n)*x4y2*6.+c(4,3,3,n)*
     & x4y2z1*18.+c(4,3,4,n)*x4y2z2*36.+c(4,4,2,n)*x4y3*8.+c(4,4,3,n)*
     & x4y3z1*24.+c(4,4,4,n)*x4y3z2*48.)*time
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
      r(i1,i2,i3,n)=(c(0,1,3,n)*6.+c(0,1,4,n)*z1*24.+c(0,2,3,n)*y1*12.+
     & c(0,2,4,n)*y1z1*48.+c(0,3,3,n)*y2*18.+c(0,3,4,n)*y2z1*72.+c(0,
     & 4,3,n)*y3*24.+c(0,4,4,n)*y3z1*96.+c(1,1,3,n)*x1*6.+c(1,1,4,n)*
     & x1z1*24.+c(1,2,3,n)*x1y1*12.+c(1,2,4,n)*x1y1z1*48.+c(1,3,3,n)*
     & x1y2*18.+c(1,3,4,n)*x1y2z1*72.+c(1,4,3,n)*x1y3*24.+c(1,4,4,n)*
     & x1y3z1*96.+c(2,1,3,n)*x2*6.+c(2,1,4,n)*x2z1*24.+c(2,2,3,n)*
     & x2y1*12.+c(2,2,4,n)*x2y1z1*48.+c(2,3,3,n)*x2y2*18.+c(2,3,4,n)*
     & x2y2z1*72.+c(2,4,3,n)*x2y3*24.+c(2,4,4,n)*x2y3z1*96.+c(3,1,3,n)
     & *x3*6.+c(3,1,4,n)*x3z1*24.+c(3,2,3,n)*x3y1*12.+c(3,2,4,n)*
     & x3y1z1*48.+c(3,3,3,n)*x3y2*18.+c(3,3,4,n)*x3y2z1*72.+c(3,4,3,n)
     & *x3y3*24.+c(3,4,4,n)*x3y3z1*96.+c(4,1,3,n)*x4*6.+c(4,1,4,n)*
     & x4z1*24.+c(4,2,3,n)*x4y1*12.+c(4,2,4,n)*x4y1z1*48.+c(4,3,3,n)*
     & x4y2*18.+c(4,3,4,n)*x4y2z1*72.+c(4,4,3,n)*x4y3*24.+c(4,4,4,n)*
     & x4y3z1*96.)*time
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
      y1=ya(i1,i2,i3)
      y2=y1*y1
      y3=y2*y1
      x1=xa(i1,i2,i3)
      x1y1=x1*y1
      x1y2=x1y1*y1
      x1y3=x1y2*y1
      x2=x1*x1
      x2y1=x1y1*x1
      x2y2=x1y2*x1
      x2y3=x1y3*x1
      x3=x2*x1
      x3y1=x2y1*x1
      x3y2=x2y2*x1
      x3y3=x2y3*x1
      x4=x3*x1
      x4y1=x3y1*x1
      x4y2=x3y2*x1
      x4y3=x3y3*x1
      r(i1,i2,i3,n)=(c(0,1,4,n)*24.+c(0,2,4,n)*y1*48.+c(0,3,4,n)*y2*
     & 72.+c(0,4,4,n)*y3*96.+c(1,1,4,n)*x1*24.+c(1,2,4,n)*x1y1*48.+c(
     & 1,3,4,n)*x1y2*72.+c(1,4,4,n)*x1y3*96.+c(2,1,4,n)*x2*24.+c(2,2,
     & 4,n)*x2y1*48.+c(2,3,4,n)*x2y2*72.+c(2,4,4,n)*x2y3*96.+c(3,1,4,
     & n)*x3*24.+c(3,2,4,n)*x3y1*48.+c(3,3,4,n)*x3y2*72.+c(3,4,4,n)*
     & x3y3*96.+c(4,1,4,n)*x4*24.+c(4,2,4,n)*x4y1*48.+c(4,3,4,n)*x4y2*
     & 72.+c(4,4,4,n)*x4y3*96.)*time
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
      r(i1,i2,i3,n)=(c(0,2,0,n)*2.+c(0,2,1,n)*z1*2.+c(0,2,2,n)*z2*2.+c(
     & 0,2,3,n)*z3*2.+c(0,2,4,n)*z4*2.+c(0,3,0,n)*y1*6.+c(0,3,1,n)*
     & y1z1*6.+c(0,3,2,n)*y1z2*6.+c(0,3,3,n)*y1z3*6.+c(0,3,4,n)*y1z4*
     & 6.+c(0,4,0,n)*y2*12.+c(0,4,1,n)*y2z1*12.+c(0,4,2,n)*y2z2*12.+c(
     & 0,4,3,n)*y2z3*12.+c(0,4,4,n)*y2z4*12.+c(1,2,0,n)*x1*2.+c(1,2,1,
     & n)*x1z1*2.+c(1,2,2,n)*x1z2*2.+c(1,2,3,n)*x1z3*2.+c(1,2,4,n)*
     & x1z4*2.+c(1,3,0,n)*x1y1*6.+c(1,3,1,n)*x1y1z1*6.+c(1,3,2,n)*
     & x1y1z2*6.+c(1,3,3,n)*x1y1z3*6.+c(1,3,4,n)*x1y1z4*6.+c(1,4,0,n)*
     & x1y2*12.+c(1,4,1,n)*x1y2z1*12.+c(1,4,2,n)*x1y2z2*12.+c(1,4,3,n)
     & *x1y2z3*12.+c(1,4,4,n)*x1y2z4*12.+c(2,2,0,n)*x2*2.+c(2,2,1,n)*
     & x2z1*2.+c(2,2,2,n)*x2z2*2.+c(2,2,3,n)*x2z3*2.+c(2,2,4,n)*x2z4*
     & 2.+c(2,3,0,n)*x2y1*6.+c(2,3,1,n)*x2y1z1*6.+c(2,3,2,n)*x2y1z2*
     & 6.+c(2,3,3,n)*x2y1z3*6.+c(2,3,4,n)*x2y1z4*6.+c(2,4,0,n)*x2y2*
     & 12.+c(2,4,1,n)*x2y2z1*12.+c(2,4,2,n)*x2y2z2*12.+c(2,4,3,n)*
     & x2y2z3*12.+c(2,4,4,n)*x2y2z4*12.+c(3,2,0,n)*x3*2.+c(3,2,1,n)*
     & x3z1*2.+c(3,2,2,n)*x3z2*2.+c(3,2,3,n)*x3z3*2.+c(3,2,4,n)*x3z4*
     & 2.+c(3,3,0,n)*x3y1*6.+c(3,3,1,n)*x3y1z1*6.+c(3,3,2,n)*x3y1z2*
     & 6.+c(3,3,3,n)*x3y1z3*6.+c(3,3,4,n)*x3y1z4*6.+c(3,4,0,n)*x3y2*
     & 12.+c(3,4,1,n)*x3y2z1*12.+c(3,4,2,n)*x3y2z2*12.+c(3,4,3,n)*
     & x3y2z3*12.+c(3,4,4,n)*x3y2z4*12.+c(4,2,0,n)*x4*2.+c(4,2,1,n)*
     & x4z1*2.+c(4,2,2,n)*x4z2*2.+c(4,2,3,n)*x4z3*2.+c(4,2,4,n)*x4z4*
     & 2.+c(4,3,0,n)*x4y1*6.+c(4,3,1,n)*x4y1z1*6.+c(4,3,2,n)*x4y1z2*
     & 6.+c(4,3,3,n)*x4y1z3*6.+c(4,3,4,n)*x4y1z4*6.+c(4,4,0,n)*x4y2*
     & 12.+c(4,4,1,n)*x4y2z1*12.+c(4,4,2,n)*x4y2z2*12.+c(4,4,3,n)*
     & x4y2z3*12.+c(4,4,4,n)*x4y2z4*12.)*time
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
      r(i1,i2,i3,n)=(c(0,2,1,n)*2.+c(0,2,2,n)*z1*4.+c(0,2,3,n)*z2*6.+c(
     & 0,2,4,n)*z3*8.+c(0,3,1,n)*y1*6.+c(0,3,2,n)*y1z1*12.+c(0,3,3,n)*
     & y1z2*18.+c(0,3,4,n)*y1z3*24.+c(0,4,1,n)*y2*12.+c(0,4,2,n)*y2z1*
     & 24.+c(0,4,3,n)*y2z2*36.+c(0,4,4,n)*y2z3*48.+c(1,2,1,n)*x1*2.+c(
     & 1,2,2,n)*x1z1*4.+c(1,2,3,n)*x1z2*6.+c(1,2,4,n)*x1z3*8.+c(1,3,1,
     & n)*x1y1*6.+c(1,3,2,n)*x1y1z1*12.+c(1,3,3,n)*x1y1z2*18.+c(1,3,4,
     & n)*x1y1z3*24.+c(1,4,1,n)*x1y2*12.+c(1,4,2,n)*x1y2z1*24.+c(1,4,
     & 3,n)*x1y2z2*36.+c(1,4,4,n)*x1y2z3*48.+c(2,2,1,n)*x2*2.+c(2,2,2,
     & n)*x2z1*4.+c(2,2,3,n)*x2z2*6.+c(2,2,4,n)*x2z3*8.+c(2,3,1,n)*
     & x2y1*6.+c(2,3,2,n)*x2y1z1*12.+c(2,3,3,n)*x2y1z2*18.+c(2,3,4,n)*
     & x2y1z3*24.+c(2,4,1,n)*x2y2*12.+c(2,4,2,n)*x2y2z1*24.+c(2,4,3,n)
     & *x2y2z2*36.+c(2,4,4,n)*x2y2z3*48.+c(3,2,1,n)*x3*2.+c(3,2,2,n)*
     & x3z1*4.+c(3,2,3,n)*x3z2*6.+c(3,2,4,n)*x3z3*8.+c(3,3,1,n)*x3y1*
     & 6.+c(3,3,2,n)*x3y1z1*12.+c(3,3,3,n)*x3y1z2*18.+c(3,3,4,n)*
     & x3y1z3*24.+c(3,4,1,n)*x3y2*12.+c(3,4,2,n)*x3y2z1*24.+c(3,4,3,n)
     & *x3y2z2*36.+c(3,4,4,n)*x3y2z3*48.+c(4,2,1,n)*x4*2.+c(4,2,2,n)*
     & x4z1*4.+c(4,2,3,n)*x4z2*6.+c(4,2,4,n)*x4z3*8.+c(4,3,1,n)*x4y1*
     & 6.+c(4,3,2,n)*x4y1z1*12.+c(4,3,3,n)*x4y1z2*18.+c(4,3,4,n)*
     & x4y1z3*24.+c(4,4,1,n)*x4y2*12.+c(4,4,2,n)*x4y2z1*24.+c(4,4,3,n)
     & *x4y2z2*36.+c(4,4,4,n)*x4y2z3*48.)*time
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
      r(i1,i2,i3,n)=(c(0,2,2,n)*4.+c(0,2,3,n)*z1*12.+c(0,2,4,n)*z2*24.+
     & c(0,3,2,n)*y1*12.+c(0,3,3,n)*y1z1*36.+c(0,3,4,n)*y1z2*72.+c(0,
     & 4,2,n)*y2*24.+c(0,4,3,n)*y2z1*72.+c(0,4,4,n)*y2z2*144.+c(1,2,2,
     & n)*x1*4.+c(1,2,3,n)*x1z1*12.+c(1,2,4,n)*x1z2*24.+c(1,3,2,n)*
     & x1y1*12.+c(1,3,3,n)*x1y1z1*36.+c(1,3,4,n)*x1y1z2*72.+c(1,4,2,n)
     & *x1y2*24.+c(1,4,3,n)*x1y2z1*72.+c(1,4,4,n)*x1y2z2*144.+c(2,2,2,
     & n)*x2*4.+c(2,2,3,n)*x2z1*12.+c(2,2,4,n)*x2z2*24.+c(2,3,2,n)*
     & x2y1*12.+c(2,3,3,n)*x2y1z1*36.+c(2,3,4,n)*x2y1z2*72.+c(2,4,2,n)
     & *x2y2*24.+c(2,4,3,n)*x2y2z1*72.+c(2,4,4,n)*x2y2z2*144.+c(3,2,2,
     & n)*x3*4.+c(3,2,3,n)*x3z1*12.+c(3,2,4,n)*x3z2*24.+c(3,3,2,n)*
     & x3y1*12.+c(3,3,3,n)*x3y1z1*36.+c(3,3,4,n)*x3y1z2*72.+c(3,4,2,n)
     & *x3y2*24.+c(3,4,3,n)*x3y2z1*72.+c(3,4,4,n)*x3y2z2*144.+c(4,2,2,
     & n)*x4*4.+c(4,2,3,n)*x4z1*12.+c(4,2,4,n)*x4z2*24.+c(4,3,2,n)*
     & x4y1*12.+c(4,3,3,n)*x4y1z1*36.+c(4,3,4,n)*x4y1z2*72.+c(4,4,2,n)
     & *x4y2*24.+c(4,4,3,n)*x4y2z1*72.+c(4,4,4,n)*x4y2z2*144.)*time
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
      r(i1,i2,i3,n)=(c(0,2,3,n)*12.+c(0,2,4,n)*z1*48.+c(0,3,3,n)*y1*
     & 36.+c(0,3,4,n)*y1z1*144.+c(0,4,3,n)*y2*72.+c(0,4,4,n)*y2z1*
     & 288.+c(1,2,3,n)*x1*12.+c(1,2,4,n)*x1z1*48.+c(1,3,3,n)*x1y1*36.+
     & c(1,3,4,n)*x1y1z1*144.+c(1,4,3,n)*x1y2*72.+c(1,4,4,n)*x1y2z1*
     & 288.+c(2,2,3,n)*x2*12.+c(2,2,4,n)*x2z1*48.+c(2,3,3,n)*x2y1*36.+
     & c(2,3,4,n)*x2y1z1*144.+c(2,4,3,n)*x2y2*72.+c(2,4,4,n)*x2y2z1*
     & 288.+c(3,2,3,n)*x3*12.+c(3,2,4,n)*x3z1*48.+c(3,3,3,n)*x3y1*36.+
     & c(3,3,4,n)*x3y1z1*144.+c(3,4,3,n)*x3y2*72.+c(3,4,4,n)*x3y2z1*
     & 288.+c(4,2,3,n)*x4*12.+c(4,2,4,n)*x4z1*48.+c(4,3,3,n)*x4y1*36.+
     & c(4,3,4,n)*x4y1z1*144.+c(4,4,3,n)*x4y2*72.+c(4,4,4,n)*x4y2z1*
     & 288.)*time
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
      y1=ya(i1,i2,i3)
      y2=y1*y1
      x1=xa(i1,i2,i3)
      x1y1=x1*y1
      x1y2=x1y1*y1
      x2=x1*x1
      x2y1=x1y1*x1
      x2y2=x1y2*x1
      x3=x2*x1
      x3y1=x2y1*x1
      x3y2=x2y2*x1
      x4=x3*x1
      x4y1=x3y1*x1
      x4y2=x3y2*x1
      r(i1,i2,i3,n)=(c(0,2,4,n)*48.+c(0,3,4,n)*y1*144.+c(0,4,4,n)*y2*
     & 288.+c(1,2,4,n)*x1*48.+c(1,3,4,n)*x1y1*144.+c(1,4,4,n)*x1y2*
     & 288.+c(2,2,4,n)*x2*48.+c(2,3,4,n)*x2y1*144.+c(2,4,4,n)*x2y2*
     & 288.+c(3,2,4,n)*x3*48.+c(3,3,4,n)*x3y1*144.+c(3,4,4,n)*x3y2*
     & 288.+c(4,2,4,n)*x4*48.+c(4,3,4,n)*x4y1*144.+c(4,4,4,n)*x4y2*
     & 288.)*time
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
      r(i1,i2,i3,n)=(c(0,3,0,n)*6.+c(0,3,1,n)*z1*6.+c(0,3,2,n)*z2*6.+c(
     & 0,3,3,n)*z3*6.+c(0,3,4,n)*z4*6.+c(0,4,0,n)*y1*24.+c(0,4,1,n)*
     & y1z1*24.+c(0,4,2,n)*y1z2*24.+c(0,4,3,n)*y1z3*24.+c(0,4,4,n)*
     & y1z4*24.+c(1,3,0,n)*x1*6.+c(1,3,1,n)*x1z1*6.+c(1,3,2,n)*x1z2*
     & 6.+c(1,3,3,n)*x1z3*6.+c(1,3,4,n)*x1z4*6.+c(1,4,0,n)*x1y1*24.+c(
     & 1,4,1,n)*x1y1z1*24.+c(1,4,2,n)*x1y1z2*24.+c(1,4,3,n)*x1y1z3*
     & 24.+c(1,4,4,n)*x1y1z4*24.+c(2,3,0,n)*x2*6.+c(2,3,1,n)*x2z1*6.+
     & c(2,3,2,n)*x2z2*6.+c(2,3,3,n)*x2z3*6.+c(2,3,4,n)*x2z4*6.+c(2,4,
     & 0,n)*x2y1*24.+c(2,4,1,n)*x2y1z1*24.+c(2,4,2,n)*x2y1z2*24.+c(2,
     & 4,3,n)*x2y1z3*24.+c(2,4,4,n)*x2y1z4*24.+c(3,3,0,n)*x3*6.+c(3,3,
     & 1,n)*x3z1*6.+c(3,3,2,n)*x3z2*6.+c(3,3,3,n)*x3z3*6.+c(3,3,4,n)*
     & x3z4*6.+c(3,4,0,n)*x3y1*24.+c(3,4,1,n)*x3y1z1*24.+c(3,4,2,n)*
     & x3y1z2*24.+c(3,4,3,n)*x3y1z3*24.+c(3,4,4,n)*x3y1z4*24.+c(4,3,0,
     & n)*x4*6.+c(4,3,1,n)*x4z1*6.+c(4,3,2,n)*x4z2*6.+c(4,3,3,n)*x4z3*
     & 6.+c(4,3,4,n)*x4z4*6.+c(4,4,0,n)*x4y1*24.+c(4,4,1,n)*x4y1z1*
     & 24.+c(4,4,2,n)*x4y1z2*24.+c(4,4,3,n)*x4y1z3*24.+c(4,4,4,n)*
     & x4y1z4*24.)*time
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
      r(i1,i2,i3,n)=(c(0,3,1,n)*6.+c(0,3,2,n)*z1*12.+c(0,3,3,n)*z2*18.+
     & c(0,3,4,n)*z3*24.+c(0,4,1,n)*y1*24.+c(0,4,2,n)*y1z1*48.+c(0,4,
     & 3,n)*y1z2*72.+c(0,4,4,n)*y1z3*96.+c(1,3,1,n)*x1*6.+c(1,3,2,n)*
     & x1z1*12.+c(1,3,3,n)*x1z2*18.+c(1,3,4,n)*x1z3*24.+c(1,4,1,n)*
     & x1y1*24.+c(1,4,2,n)*x1y1z1*48.+c(1,4,3,n)*x1y1z2*72.+c(1,4,4,n)
     & *x1y1z3*96.+c(2,3,1,n)*x2*6.+c(2,3,2,n)*x2z1*12.+c(2,3,3,n)*
     & x2z2*18.+c(2,3,4,n)*x2z3*24.+c(2,4,1,n)*x2y1*24.+c(2,4,2,n)*
     & x2y1z1*48.+c(2,4,3,n)*x2y1z2*72.+c(2,4,4,n)*x2y1z3*96.+c(3,3,1,
     & n)*x3*6.+c(3,3,2,n)*x3z1*12.+c(3,3,3,n)*x3z2*18.+c(3,3,4,n)*
     & x3z3*24.+c(3,4,1,n)*x3y1*24.+c(3,4,2,n)*x3y1z1*48.+c(3,4,3,n)*
     & x3y1z2*72.+c(3,4,4,n)*x3y1z3*96.+c(4,3,1,n)*x4*6.+c(4,3,2,n)*
     & x4z1*12.+c(4,3,3,n)*x4z2*18.+c(4,3,4,n)*x4z3*24.+c(4,4,1,n)*
     & x4y1*24.+c(4,4,2,n)*x4y1z1*48.+c(4,4,3,n)*x4y1z2*72.+c(4,4,4,n)
     & *x4y1z3*96.)*time
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
      r(i1,i2,i3,n)=(c(0,3,2,n)*12.+c(0,3,3,n)*z1*36.+c(0,3,4,n)*z2*
     & 72.+c(0,4,2,n)*y1*48.+c(0,4,3,n)*y1z1*144.+c(0,4,4,n)*y1z2*
     & 288.+c(1,3,2,n)*x1*12.+c(1,3,3,n)*x1z1*36.+c(1,3,4,n)*x1z2*72.+
     & c(1,4,2,n)*x1y1*48.+c(1,4,3,n)*x1y1z1*144.+c(1,4,4,n)*x1y1z2*
     & 288.+c(2,3,2,n)*x2*12.+c(2,3,3,n)*x2z1*36.+c(2,3,4,n)*x2z2*72.+
     & c(2,4,2,n)*x2y1*48.+c(2,4,3,n)*x2y1z1*144.+c(2,4,4,n)*x2y1z2*
     & 288.+c(3,3,2,n)*x3*12.+c(3,3,3,n)*x3z1*36.+c(3,3,4,n)*x3z2*72.+
     & c(3,4,2,n)*x3y1*48.+c(3,4,3,n)*x3y1z1*144.+c(3,4,4,n)*x3y1z2*
     & 288.+c(4,3,2,n)*x4*12.+c(4,3,3,n)*x4z1*36.+c(4,3,4,n)*x4z2*72.+
     & c(4,4,2,n)*x4y1*48.+c(4,4,3,n)*x4y1z1*144.+c(4,4,4,n)*x4y1z2*
     & 288.)*time
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
      r(i1,i2,i3,n)=(c(0,3,3,n)*36.+c(0,3,4,n)*z1*144.+c(0,4,3,n)*y1*
     & 144.+c(0,4,4,n)*y1z1*576.+c(1,3,3,n)*x1*36.+c(1,3,4,n)*x1z1*
     & 144.+c(1,4,3,n)*x1y1*144.+c(1,4,4,n)*x1y1z1*576.+c(2,3,3,n)*x2*
     & 36.+c(2,3,4,n)*x2z1*144.+c(2,4,3,n)*x2y1*144.+c(2,4,4,n)*
     & x2y1z1*576.+c(3,3,3,n)*x3*36.+c(3,3,4,n)*x3z1*144.+c(3,4,3,n)*
     & x3y1*144.+c(3,4,4,n)*x3y1z1*576.+c(4,3,3,n)*x4*36.+c(4,3,4,n)*
     & x4z1*144.+c(4,4,3,n)*x4y1*144.+c(4,4,4,n)*x4y1z1*576.)*time
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
      y1=ya(i1,i2,i3)
      x1=xa(i1,i2,i3)
      x1y1=x1*y1
      x2=x1*x1
      x2y1=x1y1*x1
      x3=x2*x1
      x3y1=x2y1*x1
      x4=x3*x1
      x4y1=x3y1*x1
      r(i1,i2,i3,n)=(c(0,3,4,n)*144.+c(0,4,4,n)*y1*576.+c(1,3,4,n)*x1*
     & 144.+c(1,4,4,n)*x1y1*576.+c(2,3,4,n)*x2*144.+c(2,4,4,n)*x2y1*
     & 576.+c(3,3,4,n)*x3*144.+c(3,4,4,n)*x3y1*576.+c(4,3,4,n)*x4*
     & 144.+c(4,4,4,n)*x4y1*576.)*time
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
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3z4=x2z4*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4z2=x3z2*x1
      x4z3=x3z3*x1
      x4z4=x3z4*x1
      r(i1,i2,i3,n)=(c(0,4,0,n)*24.+c(0,4,1,n)*z1*24.+c(0,4,2,n)*z2*
     & 24.+c(0,4,3,n)*z3*24.+c(0,4,4,n)*z4*24.+c(1,4,0,n)*x1*24.+c(1,
     & 4,1,n)*x1z1*24.+c(1,4,2,n)*x1z2*24.+c(1,4,3,n)*x1z3*24.+c(1,4,
     & 4,n)*x1z4*24.+c(2,4,0,n)*x2*24.+c(2,4,1,n)*x2z1*24.+c(2,4,2,n)*
     & x2z2*24.+c(2,4,3,n)*x2z3*24.+c(2,4,4,n)*x2z4*24.+c(3,4,0,n)*x3*
     & 24.+c(3,4,1,n)*x3z1*24.+c(3,4,2,n)*x3z2*24.+c(3,4,3,n)*x3z3*
     & 24.+c(3,4,4,n)*x3z4*24.+c(4,4,0,n)*x4*24.+c(4,4,1,n)*x4z1*24.+
     & c(4,4,2,n)*x4z2*24.+c(4,4,3,n)*x4z3*24.+c(4,4,4,n)*x4z4*24.)*
     & time
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
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4z2=x3z2*x1
      x4z3=x3z3*x1
      r(i1,i2,i3,n)=(c(0,4,1,n)*24.+c(0,4,2,n)*z1*48.+c(0,4,3,n)*z2*
     & 72.+c(0,4,4,n)*z3*96.+c(1,4,1,n)*x1*24.+c(1,4,2,n)*x1z1*48.+c(
     & 1,4,3,n)*x1z2*72.+c(1,4,4,n)*x1z3*96.+c(2,4,1,n)*x2*24.+c(2,4,
     & 2,n)*x2z1*48.+c(2,4,3,n)*x2z2*72.+c(2,4,4,n)*x2z3*96.+c(3,4,1,
     & n)*x3*24.+c(3,4,2,n)*x3z1*48.+c(3,4,3,n)*x3z2*72.+c(3,4,4,n)*
     & x3z3*96.+c(4,4,1,n)*x4*24.+c(4,4,2,n)*x4z1*48.+c(4,4,3,n)*x4z2*
     & 72.+c(4,4,4,n)*x4z3*96.)*time
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
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4z2=x3z2*x1
      r(i1,i2,i3,n)=(c(0,4,2,n)*48.+c(0,4,3,n)*z1*144.+c(0,4,4,n)*z2*
     & 288.+c(1,4,2,n)*x1*48.+c(1,4,3,n)*x1z1*144.+c(1,4,4,n)*x1z2*
     & 288.+c(2,4,2,n)*x2*48.+c(2,4,3,n)*x2z1*144.+c(2,4,4,n)*x2z2*
     & 288.+c(3,4,2,n)*x3*48.+c(3,4,3,n)*x3z1*144.+c(3,4,4,n)*x3z2*
     & 288.+c(4,4,2,n)*x4*48.+c(4,4,3,n)*x4z1*144.+c(4,4,4,n)*x4z2*
     & 288.)*time
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
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x2=x1*x1
      x2z1=x1z1*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x4=x3*x1
      x4z1=x3z1*x1
      r(i1,i2,i3,n)=(c(0,4,3,n)*144.+c(0,4,4,n)*z1*576.+c(1,4,3,n)*x1*
     & 144.+c(1,4,4,n)*x1z1*576.+c(2,4,3,n)*x2*144.+c(2,4,4,n)*x2z1*
     & 576.+c(3,4,3,n)*x3*144.+c(3,4,4,n)*x3z1*576.+c(4,4,3,n)*x4*
     & 144.+c(4,4,4,n)*x4z1*576.)*time
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
      x1=xa(i1,i2,i3)
      x2=x1*x1
      x3=x2*x1
      x4=x3*x1
      r(i1,i2,i3,n)=(c(0,4,4,n)*576.+c(1,4,4,n)*x1*576.+c(2,4,4,n)*x2*
     & 576.+c(3,4,4,n)*x3*576.+c(4,4,4,n)*x4*576.)*time
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
      r(i1,i2,i3,n)=(c(1,0,0,n)+c(1,0,1,n)*z1+c(1,0,2,n)*z2+c(1,0,3,n)*
     & z3+c(1,0,4,n)*z4+c(1,1,0,n)*y1+c(1,1,1,n)*y1z1+c(1,1,2,n)*y1z2+
     & c(1,1,3,n)*y1z3+c(1,1,4,n)*y1z4+c(1,2,0,n)*y2+c(1,2,1,n)*y2z1+
     & c(1,2,2,n)*y2z2+c(1,2,3,n)*y2z3+c(1,2,4,n)*y2z4+c(1,3,0,n)*y3+
     & c(1,3,1,n)*y3z1+c(1,3,2,n)*y3z2+c(1,3,3,n)*y3z3+c(1,3,4,n)*
     & y3z4+c(1,4,0,n)*y4+c(1,4,1,n)*y4z1+c(1,4,2,n)*y4z2+c(1,4,3,n)*
     & y4z3+c(1,4,4,n)*y4z4+c(2,0,0,n)*x1*2.+c(2,0,1,n)*x1z1*2.+c(2,0,
     & 2,n)*x1z2*2.+c(2,0,3,n)*x1z3*2.+c(2,0,4,n)*x1z4*2.+c(2,1,0,n)*
     & x1y1*2.+c(2,1,1,n)*x1y1z1*2.+c(2,1,2,n)*x1y1z2*2.+c(2,1,3,n)*
     & x1y1z3*2.+c(2,1,4,n)*x1y1z4*2.+c(2,2,0,n)*x1y2*2.+c(2,2,1,n)*
     & x1y2z1*2.+c(2,2,2,n)*x1y2z2*2.+c(2,2,3,n)*x1y2z3*2.+c(2,2,4,n)*
     & x1y2z4*2.+c(2,3,0,n)*x1y3*2.+c(2,3,1,n)*x1y3z1*2.+c(2,3,2,n)*
     & x1y3z2*2.+c(2,3,3,n)*x1y3z3*2.+c(2,3,4,n)*x1y3z4*2.+c(2,4,0,n)*
     & x1y4*2.+c(2,4,1,n)*x1y4z1*2.+c(2,4,2,n)*x1y4z2*2.+c(2,4,3,n)*
     & x1y4z3*2.+c(2,4,4,n)*x1y4z4*2.+c(3,0,0,n)*x2*3.+c(3,0,1,n)*
     & x2z1*3.+c(3,0,2,n)*x2z2*3.+c(3,0,3,n)*x2z3*3.+c(3,0,4,n)*x2z4*
     & 3.+c(3,1,0,n)*x2y1*3.+c(3,1,1,n)*x2y1z1*3.+c(3,1,2,n)*x2y1z2*
     & 3.+c(3,1,3,n)*x2y1z3*3.+c(3,1,4,n)*x2y1z4*3.+c(3,2,0,n)*x2y2*
     & 3.+c(3,2,1,n)*x2y2z1*3.+c(3,2,2,n)*x2y2z2*3.+c(3,2,3,n)*x2y2z3*
     & 3.+c(3,2,4,n)*x2y2z4*3.+c(3,3,0,n)*x2y3*3.+c(3,3,1,n)*x2y3z1*
     & 3.+c(3,3,2,n)*x2y3z2*3.+c(3,3,3,n)*x2y3z3*3.+c(3,3,4,n)*x2y3z4*
     & 3.+c(3,4,0,n)*x2y4*3.+c(3,4,1,n)*x2y4z1*3.+c(3,4,2,n)*x2y4z2*
     & 3.+c(3,4,3,n)*x2y4z3*3.+c(3,4,4,n)*x2y4z4*3.+c(4,0,0,n)*x3*4.+
     & c(4,0,1,n)*x3z1*4.+c(4,0,2,n)*x3z2*4.+c(4,0,3,n)*x3z3*4.+c(4,0,
     & 4,n)*x3z4*4.+c(4,1,0,n)*x3y1*4.+c(4,1,1,n)*x3y1z1*4.+c(4,1,2,n)
     & *x3y1z2*4.+c(4,1,3,n)*x3y1z3*4.+c(4,1,4,n)*x3y1z4*4.+c(4,2,0,n)
     & *x3y2*4.+c(4,2,1,n)*x3y2z1*4.+c(4,2,2,n)*x3y2z2*4.+c(4,2,3,n)*
     & x3y2z3*4.+c(4,2,4,n)*x3y2z4*4.+c(4,3,0,n)*x3y3*4.+c(4,3,1,n)*
     & x3y3z1*4.+c(4,3,2,n)*x3y3z2*4.+c(4,3,3,n)*x3y3z3*4.+c(4,3,4,n)*
     & x3y3z4*4.+c(4,4,0,n)*x3y4*4.+c(4,4,1,n)*x3y4z1*4.+c(4,4,2,n)*
     & x3y4z2*4.+c(4,4,3,n)*x3y4z3*4.+c(4,4,4,n)*x3y4z4*4.)*time
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
      r(i1,i2,i3,n)=(c(1,0,1,n)+c(1,0,2,n)*z1*2.+c(1,0,3,n)*z2*3.+c(1,
     & 0,4,n)*z3*4.+c(1,1,1,n)*y1+c(1,1,2,n)*y1z1*2.+c(1,1,3,n)*y1z2*
     & 3.+c(1,1,4,n)*y1z3*4.+c(1,2,1,n)*y2+c(1,2,2,n)*y2z1*2.+c(1,2,3,
     & n)*y2z2*3.+c(1,2,4,n)*y2z3*4.+c(1,3,1,n)*y3+c(1,3,2,n)*y3z1*2.+
     & c(1,3,3,n)*y3z2*3.+c(1,3,4,n)*y3z3*4.+c(1,4,1,n)*y4+c(1,4,2,n)*
     & y4z1*2.+c(1,4,3,n)*y4z2*3.+c(1,4,4,n)*y4z3*4.+c(2,0,1,n)*x1*2.+
     & c(2,0,2,n)*x1z1*4.+c(2,0,3,n)*x1z2*6.+c(2,0,4,n)*x1z3*8.+c(2,1,
     & 1,n)*x1y1*2.+c(2,1,2,n)*x1y1z1*4.+c(2,1,3,n)*x1y1z2*6.+c(2,1,4,
     & n)*x1y1z3*8.+c(2,2,1,n)*x1y2*2.+c(2,2,2,n)*x1y2z1*4.+c(2,2,3,n)
     & *x1y2z2*6.+c(2,2,4,n)*x1y2z3*8.+c(2,3,1,n)*x1y3*2.+c(2,3,2,n)*
     & x1y3z1*4.+c(2,3,3,n)*x1y3z2*6.+c(2,3,4,n)*x1y3z3*8.+c(2,4,1,n)*
     & x1y4*2.+c(2,4,2,n)*x1y4z1*4.+c(2,4,3,n)*x1y4z2*6.+c(2,4,4,n)*
     & x1y4z3*8.+c(3,0,1,n)*x2*3.+c(3,0,2,n)*x2z1*6.+c(3,0,3,n)*x2z2*
     & 9.+c(3,0,4,n)*x2z3*12.+c(3,1,1,n)*x2y1*3.+c(3,1,2,n)*x2y1z1*6.+
     & c(3,1,3,n)*x2y1z2*9.+c(3,1,4,n)*x2y1z3*12.+c(3,2,1,n)*x2y2*3.+
     & c(3,2,2,n)*x2y2z1*6.+c(3,2,3,n)*x2y2z2*9.+c(3,2,4,n)*x2y2z3*
     & 12.+c(3,3,1,n)*x2y3*3.+c(3,3,2,n)*x2y3z1*6.+c(3,3,3,n)*x2y3z2*
     & 9.+c(3,3,4,n)*x2y3z3*12.+c(3,4,1,n)*x2y4*3.+c(3,4,2,n)*x2y4z1*
     & 6.+c(3,4,3,n)*x2y4z2*9.+c(3,4,4,n)*x2y4z3*12.+c(4,0,1,n)*x3*4.+
     & c(4,0,2,n)*x3z1*8.+c(4,0,3,n)*x3z2*12.+c(4,0,4,n)*x3z3*16.+c(4,
     & 1,1,n)*x3y1*4.+c(4,1,2,n)*x3y1z1*8.+c(4,1,3,n)*x3y1z2*12.+c(4,
     & 1,4,n)*x3y1z3*16.+c(4,2,1,n)*x3y2*4.+c(4,2,2,n)*x3y2z1*8.+c(4,
     & 2,3,n)*x3y2z2*12.+c(4,2,4,n)*x3y2z3*16.+c(4,3,1,n)*x3y3*4.+c(4,
     & 3,2,n)*x3y3z1*8.+c(4,3,3,n)*x3y3z2*12.+c(4,3,4,n)*x3y3z3*16.+c(
     & 4,4,1,n)*x3y4*4.+c(4,4,2,n)*x3y4z1*8.+c(4,4,3,n)*x3y4z2*12.+c(
     & 4,4,4,n)*x3y4z3*16.)*time
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
      r(i1,i2,i3,n)=(c(1,0,2,n)*2.+c(1,0,3,n)*z1*6.+c(1,0,4,n)*z2*12.+
     & c(1,1,2,n)*y1*2.+c(1,1,3,n)*y1z1*6.+c(1,1,4,n)*y1z2*12.+c(1,2,
     & 2,n)*y2*2.+c(1,2,3,n)*y2z1*6.+c(1,2,4,n)*y2z2*12.+c(1,3,2,n)*
     & y3*2.+c(1,3,3,n)*y3z1*6.+c(1,3,4,n)*y3z2*12.+c(1,4,2,n)*y4*2.+
     & c(1,4,3,n)*y4z1*6.+c(1,4,4,n)*y4z2*12.+c(2,0,2,n)*x1*4.+c(2,0,
     & 3,n)*x1z1*12.+c(2,0,4,n)*x1z2*24.+c(2,1,2,n)*x1y1*4.+c(2,1,3,n)
     & *x1y1z1*12.+c(2,1,4,n)*x1y1z2*24.+c(2,2,2,n)*x1y2*4.+c(2,2,3,n)
     & *x1y2z1*12.+c(2,2,4,n)*x1y2z2*24.+c(2,3,2,n)*x1y3*4.+c(2,3,3,n)
     & *x1y3z1*12.+c(2,3,4,n)*x1y3z2*24.+c(2,4,2,n)*x1y4*4.+c(2,4,3,n)
     & *x1y4z1*12.+c(2,4,4,n)*x1y4z2*24.+c(3,0,2,n)*x2*6.+c(3,0,3,n)*
     & x2z1*18.+c(3,0,4,n)*x2z2*36.+c(3,1,2,n)*x2y1*6.+c(3,1,3,n)*
     & x2y1z1*18.+c(3,1,4,n)*x2y1z2*36.+c(3,2,2,n)*x2y2*6.+c(3,2,3,n)*
     & x2y2z1*18.+c(3,2,4,n)*x2y2z2*36.+c(3,3,2,n)*x2y3*6.+c(3,3,3,n)*
     & x2y3z1*18.+c(3,3,4,n)*x2y3z2*36.+c(3,4,2,n)*x2y4*6.+c(3,4,3,n)*
     & x2y4z1*18.+c(3,4,4,n)*x2y4z2*36.+c(4,0,2,n)*x3*8.+c(4,0,3,n)*
     & x3z1*24.+c(4,0,4,n)*x3z2*48.+c(4,1,2,n)*x3y1*8.+c(4,1,3,n)*
     & x3y1z1*24.+c(4,1,4,n)*x3y1z2*48.+c(4,2,2,n)*x3y2*8.+c(4,2,3,n)*
     & x3y2z1*24.+c(4,2,4,n)*x3y2z2*48.+c(4,3,2,n)*x3y3*8.+c(4,3,3,n)*
     & x3y3z1*24.+c(4,3,4,n)*x3y3z2*48.+c(4,4,2,n)*x3y4*8.+c(4,4,3,n)*
     & x3y4z1*24.+c(4,4,4,n)*x3y4z2*48.)*time
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
      r(i1,i2,i3,n)=(c(1,0,3,n)*6.+c(1,0,4,n)*z1*24.+c(1,1,3,n)*y1*6.+
     & c(1,1,4,n)*y1z1*24.+c(1,2,3,n)*y2*6.+c(1,2,4,n)*y2z1*24.+c(1,3,
     & 3,n)*y3*6.+c(1,3,4,n)*y3z1*24.+c(1,4,3,n)*y4*6.+c(1,4,4,n)*
     & y4z1*24.+c(2,0,3,n)*x1*12.+c(2,0,4,n)*x1z1*48.+c(2,1,3,n)*x1y1*
     & 12.+c(2,1,4,n)*x1y1z1*48.+c(2,2,3,n)*x1y2*12.+c(2,2,4,n)*
     & x1y2z1*48.+c(2,3,3,n)*x1y3*12.+c(2,3,4,n)*x1y3z1*48.+c(2,4,3,n)
     & *x1y4*12.+c(2,4,4,n)*x1y4z1*48.+c(3,0,3,n)*x2*18.+c(3,0,4,n)*
     & x2z1*72.+c(3,1,3,n)*x2y1*18.+c(3,1,4,n)*x2y1z1*72.+c(3,2,3,n)*
     & x2y2*18.+c(3,2,4,n)*x2y2z1*72.+c(3,3,3,n)*x2y3*18.+c(3,3,4,n)*
     & x2y3z1*72.+c(3,4,3,n)*x2y4*18.+c(3,4,4,n)*x2y4z1*72.+c(4,0,3,n)
     & *x3*24.+c(4,0,4,n)*x3z1*96.+c(4,1,3,n)*x3y1*24.+c(4,1,4,n)*
     & x3y1z1*96.+c(4,2,3,n)*x3y2*24.+c(4,2,4,n)*x3y2z1*96.+c(4,3,3,n)
     & *x3y3*24.+c(4,3,4,n)*x3y3z1*96.+c(4,4,3,n)*x3y4*24.+c(4,4,4,n)*
     & x3y4z1*96.)*time
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
      y1=ya(i1,i2,i3)
      y2=y1*y1
      y3=y2*y1
      y4=y3*y1
      x1=xa(i1,i2,i3)
      x1y1=x1*y1
      x1y2=x1y1*y1
      x1y3=x1y2*y1
      x1y4=x1y3*y1
      x2=x1*x1
      x2y1=x1y1*x1
      x2y2=x1y2*x1
      x2y3=x1y3*x1
      x2y4=x1y4*x1
      x3=x2*x1
      x3y1=x2y1*x1
      x3y2=x2y2*x1
      x3y3=x2y3*x1
      x3y4=x2y4*x1
      r(i1,i2,i3,n)=(c(1,0,4,n)*24.+c(1,1,4,n)*y1*24.+c(1,2,4,n)*y2*
     & 24.+c(1,3,4,n)*y3*24.+c(1,4,4,n)*y4*24.+c(2,0,4,n)*x1*48.+c(2,
     & 1,4,n)*x1y1*48.+c(2,2,4,n)*x1y2*48.+c(2,3,4,n)*x1y3*48.+c(2,4,
     & 4,n)*x1y4*48.+c(3,0,4,n)*x2*72.+c(3,1,4,n)*x2y1*72.+c(3,2,4,n)*
     & x2y2*72.+c(3,3,4,n)*x2y3*72.+c(3,4,4,n)*x2y4*72.+c(4,0,4,n)*x3*
     & 96.+c(4,1,4,n)*x3y1*96.+c(4,2,4,n)*x3y2*96.+c(4,3,4,n)*x3y3*
     & 96.+c(4,4,4,n)*x3y4*96.)*time
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
      r(i1,i2,i3,n)=(c(1,1,0,n)+c(1,1,1,n)*z1+c(1,1,2,n)*z2+c(1,1,3,n)*
     & z3+c(1,1,4,n)*z4+c(1,2,0,n)*y1*2.+c(1,2,1,n)*y1z1*2.+c(1,2,2,n)
     & *y1z2*2.+c(1,2,3,n)*y1z3*2.+c(1,2,4,n)*y1z4*2.+c(1,3,0,n)*y2*
     & 3.+c(1,3,1,n)*y2z1*3.+c(1,3,2,n)*y2z2*3.+c(1,3,3,n)*y2z3*3.+c(
     & 1,3,4,n)*y2z4*3.+c(1,4,0,n)*y3*4.+c(1,4,1,n)*y3z1*4.+c(1,4,2,n)
     & *y3z2*4.+c(1,4,3,n)*y3z3*4.+c(1,4,4,n)*y3z4*4.+c(2,1,0,n)*x1*
     & 2.+c(2,1,1,n)*x1z1*2.+c(2,1,2,n)*x1z2*2.+c(2,1,3,n)*x1z3*2.+c(
     & 2,1,4,n)*x1z4*2.+c(2,2,0,n)*x1y1*4.+c(2,2,1,n)*x1y1z1*4.+c(2,2,
     & 2,n)*x1y1z2*4.+c(2,2,3,n)*x1y1z3*4.+c(2,2,4,n)*x1y1z4*4.+c(2,3,
     & 0,n)*x1y2*6.+c(2,3,1,n)*x1y2z1*6.+c(2,3,2,n)*x1y2z2*6.+c(2,3,3,
     & n)*x1y2z3*6.+c(2,3,4,n)*x1y2z4*6.+c(2,4,0,n)*x1y3*8.+c(2,4,1,n)
     & *x1y3z1*8.+c(2,4,2,n)*x1y3z2*8.+c(2,4,3,n)*x1y3z3*8.+c(2,4,4,n)
     & *x1y3z4*8.+c(3,1,0,n)*x2*3.+c(3,1,1,n)*x2z1*3.+c(3,1,2,n)*x2z2*
     & 3.+c(3,1,3,n)*x2z3*3.+c(3,1,4,n)*x2z4*3.+c(3,2,0,n)*x2y1*6.+c(
     & 3,2,1,n)*x2y1z1*6.+c(3,2,2,n)*x2y1z2*6.+c(3,2,3,n)*x2y1z3*6.+c(
     & 3,2,4,n)*x2y1z4*6.+c(3,3,0,n)*x2y2*9.+c(3,3,1,n)*x2y2z1*9.+c(3,
     & 3,2,n)*x2y2z2*9.+c(3,3,3,n)*x2y2z3*9.+c(3,3,4,n)*x2y2z4*9.+c(3,
     & 4,0,n)*x2y3*12.+c(3,4,1,n)*x2y3z1*12.+c(3,4,2,n)*x2y3z2*12.+c(
     & 3,4,3,n)*x2y3z3*12.+c(3,4,4,n)*x2y3z4*12.+c(4,1,0,n)*x3*4.+c(4,
     & 1,1,n)*x3z1*4.+c(4,1,2,n)*x3z2*4.+c(4,1,3,n)*x3z3*4.+c(4,1,4,n)
     & *x3z4*4.+c(4,2,0,n)*x3y1*8.+c(4,2,1,n)*x3y1z1*8.+c(4,2,2,n)*
     & x3y1z2*8.+c(4,2,3,n)*x3y1z3*8.+c(4,2,4,n)*x3y1z4*8.+c(4,3,0,n)*
     & x3y2*12.+c(4,3,1,n)*x3y2z1*12.+c(4,3,2,n)*x3y2z2*12.+c(4,3,3,n)
     & *x3y2z3*12.+c(4,3,4,n)*x3y2z4*12.+c(4,4,0,n)*x3y3*16.+c(4,4,1,
     & n)*x3y3z1*16.+c(4,4,2,n)*x3y3z2*16.+c(4,4,3,n)*x3y3z3*16.+c(4,
     & 4,4,n)*x3y3z4*16.)*time
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
      r(i1,i2,i3,n)=(c(1,1,1,n)+c(1,1,2,n)*z1*2.+c(1,1,3,n)*z2*3.+c(1,
     & 1,4,n)*z3*4.+c(1,2,1,n)*y1*2.+c(1,2,2,n)*y1z1*4.+c(1,2,3,n)*
     & y1z2*6.+c(1,2,4,n)*y1z3*8.+c(1,3,1,n)*y2*3.+c(1,3,2,n)*y2z1*6.+
     & c(1,3,3,n)*y2z2*9.+c(1,3,4,n)*y2z3*12.+c(1,4,1,n)*y3*4.+c(1,4,
     & 2,n)*y3z1*8.+c(1,4,3,n)*y3z2*12.+c(1,4,4,n)*y3z3*16.+c(2,1,1,n)
     & *x1*2.+c(2,1,2,n)*x1z1*4.+c(2,1,3,n)*x1z2*6.+c(2,1,4,n)*x1z3*
     & 8.+c(2,2,1,n)*x1y1*4.+c(2,2,2,n)*x1y1z1*8.+c(2,2,3,n)*x1y1z2*
     & 12.+c(2,2,4,n)*x1y1z3*16.+c(2,3,1,n)*x1y2*6.+c(2,3,2,n)*x1y2z1*
     & 12.+c(2,3,3,n)*x1y2z2*18.+c(2,3,4,n)*x1y2z3*24.+c(2,4,1,n)*
     & x1y3*8.+c(2,4,2,n)*x1y3z1*16.+c(2,4,3,n)*x1y3z2*24.+c(2,4,4,n)*
     & x1y3z3*32.+c(3,1,1,n)*x2*3.+c(3,1,2,n)*x2z1*6.+c(3,1,3,n)*x2z2*
     & 9.+c(3,1,4,n)*x2z3*12.+c(3,2,1,n)*x2y1*6.+c(3,2,2,n)*x2y1z1*
     & 12.+c(3,2,3,n)*x2y1z2*18.+c(3,2,4,n)*x2y1z3*24.+c(3,3,1,n)*
     & x2y2*9.+c(3,3,2,n)*x2y2z1*18.+c(3,3,3,n)*x2y2z2*27.+c(3,3,4,n)*
     & x2y2z3*36.+c(3,4,1,n)*x2y3*12.+c(3,4,2,n)*x2y3z1*24.+c(3,4,3,n)
     & *x2y3z2*36.+c(3,4,4,n)*x2y3z3*48.+c(4,1,1,n)*x3*4.+c(4,1,2,n)*
     & x3z1*8.+c(4,1,3,n)*x3z2*12.+c(4,1,4,n)*x3z3*16.+c(4,2,1,n)*
     & x3y1*8.+c(4,2,2,n)*x3y1z1*16.+c(4,2,3,n)*x3y1z2*24.+c(4,2,4,n)*
     & x3y1z3*32.+c(4,3,1,n)*x3y2*12.+c(4,3,2,n)*x3y2z1*24.+c(4,3,3,n)
     & *x3y2z2*36.+c(4,3,4,n)*x3y2z3*48.+c(4,4,1,n)*x3y3*16.+c(4,4,2,
     & n)*x3y3z1*32.+c(4,4,3,n)*x3y3z2*48.+c(4,4,4,n)*x3y3z3*64.)*time
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
      r(i1,i2,i3,n)=(c(1,1,2,n)*2.+c(1,1,3,n)*z1*6.+c(1,1,4,n)*z2*12.+
     & c(1,2,2,n)*y1*4.+c(1,2,3,n)*y1z1*12.+c(1,2,4,n)*y1z2*24.+c(1,3,
     & 2,n)*y2*6.+c(1,3,3,n)*y2z1*18.+c(1,3,4,n)*y2z2*36.+c(1,4,2,n)*
     & y3*8.+c(1,4,3,n)*y3z1*24.+c(1,4,4,n)*y3z2*48.+c(2,1,2,n)*x1*4.+
     & c(2,1,3,n)*x1z1*12.+c(2,1,4,n)*x1z2*24.+c(2,2,2,n)*x1y1*8.+c(2,
     & 2,3,n)*x1y1z1*24.+c(2,2,4,n)*x1y1z2*48.+c(2,3,2,n)*x1y2*12.+c(
     & 2,3,3,n)*x1y2z1*36.+c(2,3,4,n)*x1y2z2*72.+c(2,4,2,n)*x1y3*16.+
     & c(2,4,3,n)*x1y3z1*48.+c(2,4,4,n)*x1y3z2*96.+c(3,1,2,n)*x2*6.+c(
     & 3,1,3,n)*x2z1*18.+c(3,1,4,n)*x2z2*36.+c(3,2,2,n)*x2y1*12.+c(3,
     & 2,3,n)*x2y1z1*36.+c(3,2,4,n)*x2y1z2*72.+c(3,3,2,n)*x2y2*18.+c(
     & 3,3,3,n)*x2y2z1*54.+c(3,3,4,n)*x2y2z2*108.+c(3,4,2,n)*x2y3*24.+
     & c(3,4,3,n)*x2y3z1*72.+c(3,4,4,n)*x2y3z2*144.+c(4,1,2,n)*x3*8.+
     & c(4,1,3,n)*x3z1*24.+c(4,1,4,n)*x3z2*48.+c(4,2,2,n)*x3y1*16.+c(
     & 4,2,3,n)*x3y1z1*48.+c(4,2,4,n)*x3y1z2*96.+c(4,3,2,n)*x3y2*24.+
     & c(4,3,3,n)*x3y2z1*72.+c(4,3,4,n)*x3y2z2*144.+c(4,4,2,n)*x3y3*
     & 32.+c(4,4,3,n)*x3y3z1*96.+c(4,4,4,n)*x3y3z2*192.)*time
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
      r(i1,i2,i3,n)=(c(1,1,3,n)*6.+c(1,1,4,n)*z1*24.+c(1,2,3,n)*y1*12.+
     & c(1,2,4,n)*y1z1*48.+c(1,3,3,n)*y2*18.+c(1,3,4,n)*y2z1*72.+c(1,
     & 4,3,n)*y3*24.+c(1,4,4,n)*y3z1*96.+c(2,1,3,n)*x1*12.+c(2,1,4,n)*
     & x1z1*48.+c(2,2,3,n)*x1y1*24.+c(2,2,4,n)*x1y1z1*96.+c(2,3,3,n)*
     & x1y2*36.+c(2,3,4,n)*x1y2z1*144.+c(2,4,3,n)*x1y3*48.+c(2,4,4,n)*
     & x1y3z1*192.+c(3,1,3,n)*x2*18.+c(3,1,4,n)*x2z1*72.+c(3,2,3,n)*
     & x2y1*36.+c(3,2,4,n)*x2y1z1*144.+c(3,3,3,n)*x2y2*54.+c(3,3,4,n)*
     & x2y2z1*216.+c(3,4,3,n)*x2y3*72.+c(3,4,4,n)*x2y3z1*288.+c(4,1,3,
     & n)*x3*24.+c(4,1,4,n)*x3z1*96.+c(4,2,3,n)*x3y1*48.+c(4,2,4,n)*
     & x3y1z1*192.+c(4,3,3,n)*x3y2*72.+c(4,3,4,n)*x3y2z1*288.+c(4,4,3,
     & n)*x3y3*96.+c(4,4,4,n)*x3y3z1*384.)*time
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
      y1=ya(i1,i2,i3)
      y2=y1*y1
      y3=y2*y1
      x1=xa(i1,i2,i3)
      x1y1=x1*y1
      x1y2=x1y1*y1
      x1y3=x1y2*y1
      x2=x1*x1
      x2y1=x1y1*x1
      x2y2=x1y2*x1
      x2y3=x1y3*x1
      x3=x2*x1
      x3y1=x2y1*x1
      x3y2=x2y2*x1
      x3y3=x2y3*x1
      r(i1,i2,i3,n)=(c(1,1,4,n)*24.+c(1,2,4,n)*y1*48.+c(1,3,4,n)*y2*
     & 72.+c(1,4,4,n)*y3*96.+c(2,1,4,n)*x1*48.+c(2,2,4,n)*x1y1*96.+c(
     & 2,3,4,n)*x1y2*144.+c(2,4,4,n)*x1y3*192.+c(3,1,4,n)*x2*72.+c(3,
     & 2,4,n)*x2y1*144.+c(3,3,4,n)*x2y2*216.+c(3,4,4,n)*x2y3*288.+c(4,
     & 1,4,n)*x3*96.+c(4,2,4,n)*x3y1*192.+c(4,3,4,n)*x3y2*288.+c(4,4,
     & 4,n)*x3y3*384.)*time
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
      r(i1,i2,i3,n)=(c(1,2,0,n)*2.+c(1,2,1,n)*z1*2.+c(1,2,2,n)*z2*2.+c(
     & 1,2,3,n)*z3*2.+c(1,2,4,n)*z4*2.+c(1,3,0,n)*y1*6.+c(1,3,1,n)*
     & y1z1*6.+c(1,3,2,n)*y1z2*6.+c(1,3,3,n)*y1z3*6.+c(1,3,4,n)*y1z4*
     & 6.+c(1,4,0,n)*y2*12.+c(1,4,1,n)*y2z1*12.+c(1,4,2,n)*y2z2*12.+c(
     & 1,4,3,n)*y2z3*12.+c(1,4,4,n)*y2z4*12.+c(2,2,0,n)*x1*4.+c(2,2,1,
     & n)*x1z1*4.+c(2,2,2,n)*x1z2*4.+c(2,2,3,n)*x1z3*4.+c(2,2,4,n)*
     & x1z4*4.+c(2,3,0,n)*x1y1*12.+c(2,3,1,n)*x1y1z1*12.+c(2,3,2,n)*
     & x1y1z2*12.+c(2,3,3,n)*x1y1z3*12.+c(2,3,4,n)*x1y1z4*12.+c(2,4,0,
     & n)*x1y2*24.+c(2,4,1,n)*x1y2z1*24.+c(2,4,2,n)*x1y2z2*24.+c(2,4,
     & 3,n)*x1y2z3*24.+c(2,4,4,n)*x1y2z4*24.+c(3,2,0,n)*x2*6.+c(3,2,1,
     & n)*x2z1*6.+c(3,2,2,n)*x2z2*6.+c(3,2,3,n)*x2z3*6.+c(3,2,4,n)*
     & x2z4*6.+c(3,3,0,n)*x2y1*18.+c(3,3,1,n)*x2y1z1*18.+c(3,3,2,n)*
     & x2y1z2*18.+c(3,3,3,n)*x2y1z3*18.+c(3,3,4,n)*x2y1z4*18.+c(3,4,0,
     & n)*x2y2*36.+c(3,4,1,n)*x2y2z1*36.+c(3,4,2,n)*x2y2z2*36.+c(3,4,
     & 3,n)*x2y2z3*36.+c(3,4,4,n)*x2y2z4*36.+c(4,2,0,n)*x3*8.+c(4,2,1,
     & n)*x3z1*8.+c(4,2,2,n)*x3z2*8.+c(4,2,3,n)*x3z3*8.+c(4,2,4,n)*
     & x3z4*8.+c(4,3,0,n)*x3y1*24.+c(4,3,1,n)*x3y1z1*24.+c(4,3,2,n)*
     & x3y1z2*24.+c(4,3,3,n)*x3y1z3*24.+c(4,3,4,n)*x3y1z4*24.+c(4,4,0,
     & n)*x3y2*48.+c(4,4,1,n)*x3y2z1*48.+c(4,4,2,n)*x3y2z2*48.+c(4,4,
     & 3,n)*x3y2z3*48.+c(4,4,4,n)*x3y2z4*48.)*time
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
      r(i1,i2,i3,n)=(c(1,2,1,n)*2.+c(1,2,2,n)*z1*4.+c(1,2,3,n)*z2*6.+c(
     & 1,2,4,n)*z3*8.+c(1,3,1,n)*y1*6.+c(1,3,2,n)*y1z1*12.+c(1,3,3,n)*
     & y1z2*18.+c(1,3,4,n)*y1z3*24.+c(1,4,1,n)*y2*12.+c(1,4,2,n)*y2z1*
     & 24.+c(1,4,3,n)*y2z2*36.+c(1,4,4,n)*y2z3*48.+c(2,2,1,n)*x1*4.+c(
     & 2,2,2,n)*x1z1*8.+c(2,2,3,n)*x1z2*12.+c(2,2,4,n)*x1z3*16.+c(2,3,
     & 1,n)*x1y1*12.+c(2,3,2,n)*x1y1z1*24.+c(2,3,3,n)*x1y1z2*36.+c(2,
     & 3,4,n)*x1y1z3*48.+c(2,4,1,n)*x1y2*24.+c(2,4,2,n)*x1y2z1*48.+c(
     & 2,4,3,n)*x1y2z2*72.+c(2,4,4,n)*x1y2z3*96.+c(3,2,1,n)*x2*6.+c(3,
     & 2,2,n)*x2z1*12.+c(3,2,3,n)*x2z2*18.+c(3,2,4,n)*x2z3*24.+c(3,3,
     & 1,n)*x2y1*18.+c(3,3,2,n)*x2y1z1*36.+c(3,3,3,n)*x2y1z2*54.+c(3,
     & 3,4,n)*x2y1z3*72.+c(3,4,1,n)*x2y2*36.+c(3,4,2,n)*x2y2z1*72.+c(
     & 3,4,3,n)*x2y2z2*108.+c(3,4,4,n)*x2y2z3*144.+c(4,2,1,n)*x3*8.+c(
     & 4,2,2,n)*x3z1*16.+c(4,2,3,n)*x3z2*24.+c(4,2,4,n)*x3z3*32.+c(4,
     & 3,1,n)*x3y1*24.+c(4,3,2,n)*x3y1z1*48.+c(4,3,3,n)*x3y1z2*72.+c(
     & 4,3,4,n)*x3y1z3*96.+c(4,4,1,n)*x3y2*48.+c(4,4,2,n)*x3y2z1*96.+
     & c(4,4,3,n)*x3y2z2*144.+c(4,4,4,n)*x3y2z3*192.)*time
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
      r(i1,i2,i3,n)=(c(1,2,2,n)*4.+c(1,2,3,n)*z1*12.+c(1,2,4,n)*z2*24.+
     & c(1,3,2,n)*y1*12.+c(1,3,3,n)*y1z1*36.+c(1,3,4,n)*y1z2*72.+c(1,
     & 4,2,n)*y2*24.+c(1,4,3,n)*y2z1*72.+c(1,4,4,n)*y2z2*144.+c(2,2,2,
     & n)*x1*8.+c(2,2,3,n)*x1z1*24.+c(2,2,4,n)*x1z2*48.+c(2,3,2,n)*
     & x1y1*24.+c(2,3,3,n)*x1y1z1*72.+c(2,3,4,n)*x1y1z2*144.+c(2,4,2,
     & n)*x1y2*48.+c(2,4,3,n)*x1y2z1*144.+c(2,4,4,n)*x1y2z2*288.+c(3,
     & 2,2,n)*x2*12.+c(3,2,3,n)*x2z1*36.+c(3,2,4,n)*x2z2*72.+c(3,3,2,
     & n)*x2y1*36.+c(3,3,3,n)*x2y1z1*108.+c(3,3,4,n)*x2y1z2*216.+c(3,
     & 4,2,n)*x2y2*72.+c(3,4,3,n)*x2y2z1*216.+c(3,4,4,n)*x2y2z2*432.+
     & c(4,2,2,n)*x3*16.+c(4,2,3,n)*x3z1*48.+c(4,2,4,n)*x3z2*96.+c(4,
     & 3,2,n)*x3y1*48.+c(4,3,3,n)*x3y1z1*144.+c(4,3,4,n)*x3y1z2*288.+
     & c(4,4,2,n)*x3y2*96.+c(4,4,3,n)*x3y2z1*288.+c(4,4,4,n)*x3y2z2*
     & 576.)*time
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
      r(i1,i2,i3,n)=(c(1,2,3,n)*12.+c(1,2,4,n)*z1*48.+c(1,3,3,n)*y1*
     & 36.+c(1,3,4,n)*y1z1*144.+c(1,4,3,n)*y2*72.+c(1,4,4,n)*y2z1*
     & 288.+c(2,2,3,n)*x1*24.+c(2,2,4,n)*x1z1*96.+c(2,3,3,n)*x1y1*72.+
     & c(2,3,4,n)*x1y1z1*288.+c(2,4,3,n)*x1y2*144.+c(2,4,4,n)*x1y2z1*
     & 576.+c(3,2,3,n)*x2*36.+c(3,2,4,n)*x2z1*144.+c(3,3,3,n)*x2y1*
     & 108.+c(3,3,4,n)*x2y1z1*432.+c(3,4,3,n)*x2y2*216.+c(3,4,4,n)*
     & x2y2z1*864.+c(4,2,3,n)*x3*48.+c(4,2,4,n)*x3z1*192.+c(4,3,3,n)*
     & x3y1*144.+c(4,3,4,n)*x3y1z1*576.+c(4,4,3,n)*x3y2*288.+c(4,4,4,
     & n)*x3y2z1*1152.)*time
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
      y1=ya(i1,i2,i3)
      y2=y1*y1
      x1=xa(i1,i2,i3)
      x1y1=x1*y1
      x1y2=x1y1*y1
      x2=x1*x1
      x2y1=x1y1*x1
      x2y2=x1y2*x1
      x3=x2*x1
      x3y1=x2y1*x1
      x3y2=x2y2*x1
      r(i1,i2,i3,n)=(c(1,2,4,n)*48.+c(1,3,4,n)*y1*144.+c(1,4,4,n)*y2*
     & 288.+c(2,2,4,n)*x1*96.+c(2,3,4,n)*x1y1*288.+c(2,4,4,n)*x1y2*
     & 576.+c(3,2,4,n)*x2*144.+c(3,3,4,n)*x2y1*432.+c(3,4,4,n)*x2y2*
     & 864.+c(4,2,4,n)*x3*192.+c(4,3,4,n)*x3y1*576.+c(4,4,4,n)*x3y2*
     & 1152.)*time
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
      r(i1,i2,i3,n)=(c(1,3,0,n)*6.+c(1,3,1,n)*z1*6.+c(1,3,2,n)*z2*6.+c(
     & 1,3,3,n)*z3*6.+c(1,3,4,n)*z4*6.+c(1,4,0,n)*y1*24.+c(1,4,1,n)*
     & y1z1*24.+c(1,4,2,n)*y1z2*24.+c(1,4,3,n)*y1z3*24.+c(1,4,4,n)*
     & y1z4*24.+c(2,3,0,n)*x1*12.+c(2,3,1,n)*x1z1*12.+c(2,3,2,n)*x1z2*
     & 12.+c(2,3,3,n)*x1z3*12.+c(2,3,4,n)*x1z4*12.+c(2,4,0,n)*x1y1*
     & 48.+c(2,4,1,n)*x1y1z1*48.+c(2,4,2,n)*x1y1z2*48.+c(2,4,3,n)*
     & x1y1z3*48.+c(2,4,4,n)*x1y1z4*48.+c(3,3,0,n)*x2*18.+c(3,3,1,n)*
     & x2z1*18.+c(3,3,2,n)*x2z2*18.+c(3,3,3,n)*x2z3*18.+c(3,3,4,n)*
     & x2z4*18.+c(3,4,0,n)*x2y1*72.+c(3,4,1,n)*x2y1z1*72.+c(3,4,2,n)*
     & x2y1z2*72.+c(3,4,3,n)*x2y1z3*72.+c(3,4,4,n)*x2y1z4*72.+c(4,3,0,
     & n)*x3*24.+c(4,3,1,n)*x3z1*24.+c(4,3,2,n)*x3z2*24.+c(4,3,3,n)*
     & x3z3*24.+c(4,3,4,n)*x3z4*24.+c(4,4,0,n)*x3y1*96.+c(4,4,1,n)*
     & x3y1z1*96.+c(4,4,2,n)*x3y1z2*96.+c(4,4,3,n)*x3y1z3*96.+c(4,4,4,
     & n)*x3y1z4*96.)*time
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
      r(i1,i2,i3,n)=(c(1,3,1,n)*6.+c(1,3,2,n)*z1*12.+c(1,3,3,n)*z2*18.+
     & c(1,3,4,n)*z3*24.+c(1,4,1,n)*y1*24.+c(1,4,2,n)*y1z1*48.+c(1,4,
     & 3,n)*y1z2*72.+c(1,4,4,n)*y1z3*96.+c(2,3,1,n)*x1*12.+c(2,3,2,n)*
     & x1z1*24.+c(2,3,3,n)*x1z2*36.+c(2,3,4,n)*x1z3*48.+c(2,4,1,n)*
     & x1y1*48.+c(2,4,2,n)*x1y1z1*96.+c(2,4,3,n)*x1y1z2*144.+c(2,4,4,
     & n)*x1y1z3*192.+c(3,3,1,n)*x2*18.+c(3,3,2,n)*x2z1*36.+c(3,3,3,n)
     & *x2z2*54.+c(3,3,4,n)*x2z3*72.+c(3,4,1,n)*x2y1*72.+c(3,4,2,n)*
     & x2y1z1*144.+c(3,4,3,n)*x2y1z2*216.+c(3,4,4,n)*x2y1z3*288.+c(4,
     & 3,1,n)*x3*24.+c(4,3,2,n)*x3z1*48.+c(4,3,3,n)*x3z2*72.+c(4,3,4,
     & n)*x3z3*96.+c(4,4,1,n)*x3y1*96.+c(4,4,2,n)*x3y1z1*192.+c(4,4,3,
     & n)*x3y1z2*288.+c(4,4,4,n)*x3y1z3*384.)*time
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
      r(i1,i2,i3,n)=(c(1,3,2,n)*12.+c(1,3,3,n)*z1*36.+c(1,3,4,n)*z2*
     & 72.+c(1,4,2,n)*y1*48.+c(1,4,3,n)*y1z1*144.+c(1,4,4,n)*y1z2*
     & 288.+c(2,3,2,n)*x1*24.+c(2,3,3,n)*x1z1*72.+c(2,3,4,n)*x1z2*
     & 144.+c(2,4,2,n)*x1y1*96.+c(2,4,3,n)*x1y1z1*288.+c(2,4,4,n)*
     & x1y1z2*576.+c(3,3,2,n)*x2*36.+c(3,3,3,n)*x2z1*108.+c(3,3,4,n)*
     & x2z2*216.+c(3,4,2,n)*x2y1*144.+c(3,4,3,n)*x2y1z1*432.+c(3,4,4,
     & n)*x2y1z2*864.+c(4,3,2,n)*x3*48.+c(4,3,3,n)*x3z1*144.+c(4,3,4,
     & n)*x3z2*288.+c(4,4,2,n)*x3y1*192.+c(4,4,3,n)*x3y1z1*576.+c(4,4,
     & 4,n)*x3y1z2*1152.)*time
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
      r(i1,i2,i3,n)=(c(1,3,3,n)*36.+c(1,3,4,n)*z1*144.+c(1,4,3,n)*y1*
     & 144.+c(1,4,4,n)*y1z1*576.+c(2,3,3,n)*x1*72.+c(2,3,4,n)*x1z1*
     & 288.+c(2,4,3,n)*x1y1*288.+c(2,4,4,n)*x1y1z1*1152.+c(3,3,3,n)*
     & x2*108.+c(3,3,4,n)*x2z1*432.+c(3,4,3,n)*x2y1*432.+c(3,4,4,n)*
     & x2y1z1*1728.+c(4,3,3,n)*x3*144.+c(4,3,4,n)*x3z1*576.+c(4,4,3,n)
     & *x3y1*576.+c(4,4,4,n)*x3y1z1*2304.)*time
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
      y1=ya(i1,i2,i3)
      x1=xa(i1,i2,i3)
      x1y1=x1*y1
      x2=x1*x1
      x2y1=x1y1*x1
      x3=x2*x1
      x3y1=x2y1*x1
      r(i1,i2,i3,n)=(c(1,3,4,n)*144.+c(1,4,4,n)*y1*576.+c(2,3,4,n)*x1*
     & 288.+c(2,4,4,n)*x1y1*1152.+c(3,3,4,n)*x2*432.+c(3,4,4,n)*x2y1*
     & 1728.+c(4,3,4,n)*x3*576.+c(4,4,4,n)*x3y1*2304.)*time
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
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3z4=x2z4*x1
      r(i1,i2,i3,n)=(c(1,4,0,n)*24.+c(1,4,1,n)*z1*24.+c(1,4,2,n)*z2*
     & 24.+c(1,4,3,n)*z3*24.+c(1,4,4,n)*z4*24.+c(2,4,0,n)*x1*48.+c(2,
     & 4,1,n)*x1z1*48.+c(2,4,2,n)*x1z2*48.+c(2,4,3,n)*x1z3*48.+c(2,4,
     & 4,n)*x1z4*48.+c(3,4,0,n)*x2*72.+c(3,4,1,n)*x2z1*72.+c(3,4,2,n)*
     & x2z2*72.+c(3,4,3,n)*x2z3*72.+c(3,4,4,n)*x2z4*72.+c(4,4,0,n)*x3*
     & 96.+c(4,4,1,n)*x3z1*96.+c(4,4,2,n)*x3z2*96.+c(4,4,3,n)*x3z3*
     & 96.+c(4,4,4,n)*x3z4*96.)*time
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
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      r(i1,i2,i3,n)=(c(1,4,1,n)*24.+c(1,4,2,n)*z1*48.+c(1,4,3,n)*z2*
     & 72.+c(1,4,4,n)*z3*96.+c(2,4,1,n)*x1*48.+c(2,4,2,n)*x1z1*96.+c(
     & 2,4,3,n)*x1z2*144.+c(2,4,4,n)*x1z3*192.+c(3,4,1,n)*x2*72.+c(3,
     & 4,2,n)*x2z1*144.+c(3,4,3,n)*x2z2*216.+c(3,4,4,n)*x2z3*288.+c(4,
     & 4,1,n)*x3*96.+c(4,4,2,n)*x3z1*192.+c(4,4,3,n)*x3z2*288.+c(4,4,
     & 4,n)*x3z3*384.)*time
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
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      r(i1,i2,i3,n)=(c(1,4,2,n)*48.+c(1,4,3,n)*z1*144.+c(1,4,4,n)*z2*
     & 288.+c(2,4,2,n)*x1*96.+c(2,4,3,n)*x1z1*288.+c(2,4,4,n)*x1z2*
     & 576.+c(3,4,2,n)*x2*144.+c(3,4,3,n)*x2z1*432.+c(3,4,4,n)*x2z2*
     & 864.+c(4,4,2,n)*x3*192.+c(4,4,3,n)*x3z1*576.+c(4,4,4,n)*x3z2*
     & 1152.)*time
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
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x2=x1*x1
      x2z1=x1z1*x1
      x3=x2*x1
      x3z1=x2z1*x1
      r(i1,i2,i3,n)=(c(1,4,3,n)*144.+c(1,4,4,n)*z1*576.+c(2,4,3,n)*x1*
     & 288.+c(2,4,4,n)*x1z1*1152.+c(3,4,3,n)*x2*432.+c(3,4,4,n)*x2z1*
     & 1728.+c(4,4,3,n)*x3*576.+c(4,4,4,n)*x3z1*2304.)*time
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
      x1=xa(i1,i2,i3)
      x2=x1*x1
      x3=x2*x1
      r(i1,i2,i3,n)=(c(1,4,4,n)*576.+c(2,4,4,n)*x1*1152.+c(3,4,4,n)*x2*
     & 1728.+c(4,4,4,n)*x3*2304.)*time
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
      r(i1,i2,i3,n)=(c(2,0,0,n)*2.+c(2,0,1,n)*z1*2.+c(2,0,2,n)*z2*2.+c(
     & 2,0,3,n)*z3*2.+c(2,0,4,n)*z4*2.+c(2,1,0,n)*y1*2.+c(2,1,1,n)*
     & y1z1*2.+c(2,1,2,n)*y1z2*2.+c(2,1,3,n)*y1z3*2.+c(2,1,4,n)*y1z4*
     & 2.+c(2,2,0,n)*y2*2.+c(2,2,1,n)*y2z1*2.+c(2,2,2,n)*y2z2*2.+c(2,
     & 2,3,n)*y2z3*2.+c(2,2,4,n)*y2z4*2.+c(2,3,0,n)*y3*2.+c(2,3,1,n)*
     & y3z1*2.+c(2,3,2,n)*y3z2*2.+c(2,3,3,n)*y3z3*2.+c(2,3,4,n)*y3z4*
     & 2.+c(2,4,0,n)*y4*2.+c(2,4,1,n)*y4z1*2.+c(2,4,2,n)*y4z2*2.+c(2,
     & 4,3,n)*y4z3*2.+c(2,4,4,n)*y4z4*2.+c(3,0,0,n)*x1*6.+c(3,0,1,n)*
     & x1z1*6.+c(3,0,2,n)*x1z2*6.+c(3,0,3,n)*x1z3*6.+c(3,0,4,n)*x1z4*
     & 6.+c(3,1,0,n)*x1y1*6.+c(3,1,1,n)*x1y1z1*6.+c(3,1,2,n)*x1y1z2*
     & 6.+c(3,1,3,n)*x1y1z3*6.+c(3,1,4,n)*x1y1z4*6.+c(3,2,0,n)*x1y2*
     & 6.+c(3,2,1,n)*x1y2z1*6.+c(3,2,2,n)*x1y2z2*6.+c(3,2,3,n)*x1y2z3*
     & 6.+c(3,2,4,n)*x1y2z4*6.+c(3,3,0,n)*x1y3*6.+c(3,3,1,n)*x1y3z1*
     & 6.+c(3,3,2,n)*x1y3z2*6.+c(3,3,3,n)*x1y3z3*6.+c(3,3,4,n)*x1y3z4*
     & 6.+c(3,4,0,n)*x1y4*6.+c(3,4,1,n)*x1y4z1*6.+c(3,4,2,n)*x1y4z2*
     & 6.+c(3,4,3,n)*x1y4z3*6.+c(3,4,4,n)*x1y4z4*6.+c(4,0,0,n)*x2*12.+
     & c(4,0,1,n)*x2z1*12.+c(4,0,2,n)*x2z2*12.+c(4,0,3,n)*x2z3*12.+c(
     & 4,0,4,n)*x2z4*12.+c(4,1,0,n)*x2y1*12.+c(4,1,1,n)*x2y1z1*12.+c(
     & 4,1,2,n)*x2y1z2*12.+c(4,1,3,n)*x2y1z3*12.+c(4,1,4,n)*x2y1z4*
     & 12.+c(4,2,0,n)*x2y2*12.+c(4,2,1,n)*x2y2z1*12.+c(4,2,2,n)*
     & x2y2z2*12.+c(4,2,3,n)*x2y2z3*12.+c(4,2,4,n)*x2y2z4*12.+c(4,3,0,
     & n)*x2y3*12.+c(4,3,1,n)*x2y3z1*12.+c(4,3,2,n)*x2y3z2*12.+c(4,3,
     & 3,n)*x2y3z3*12.+c(4,3,4,n)*x2y3z4*12.+c(4,4,0,n)*x2y4*12.+c(4,
     & 4,1,n)*x2y4z1*12.+c(4,4,2,n)*x2y4z2*12.+c(4,4,3,n)*x2y4z3*12.+
     & c(4,4,4,n)*x2y4z4*12.)*time
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
      r(i1,i2,i3,n)=(c(2,0,1,n)*2.+c(2,0,2,n)*z1*4.+c(2,0,3,n)*z2*6.+c(
     & 2,0,4,n)*z3*8.+c(2,1,1,n)*y1*2.+c(2,1,2,n)*y1z1*4.+c(2,1,3,n)*
     & y1z2*6.+c(2,1,4,n)*y1z3*8.+c(2,2,1,n)*y2*2.+c(2,2,2,n)*y2z1*4.+
     & c(2,2,3,n)*y2z2*6.+c(2,2,4,n)*y2z3*8.+c(2,3,1,n)*y3*2.+c(2,3,2,
     & n)*y3z1*4.+c(2,3,3,n)*y3z2*6.+c(2,3,4,n)*y3z3*8.+c(2,4,1,n)*y4*
     & 2.+c(2,4,2,n)*y4z1*4.+c(2,4,3,n)*y4z2*6.+c(2,4,4,n)*y4z3*8.+c(
     & 3,0,1,n)*x1*6.+c(3,0,2,n)*x1z1*12.+c(3,0,3,n)*x1z2*18.+c(3,0,4,
     & n)*x1z3*24.+c(3,1,1,n)*x1y1*6.+c(3,1,2,n)*x1y1z1*12.+c(3,1,3,n)
     & *x1y1z2*18.+c(3,1,4,n)*x1y1z3*24.+c(3,2,1,n)*x1y2*6.+c(3,2,2,n)
     & *x1y2z1*12.+c(3,2,3,n)*x1y2z2*18.+c(3,2,4,n)*x1y2z3*24.+c(3,3,
     & 1,n)*x1y3*6.+c(3,3,2,n)*x1y3z1*12.+c(3,3,3,n)*x1y3z2*18.+c(3,3,
     & 4,n)*x1y3z3*24.+c(3,4,1,n)*x1y4*6.+c(3,4,2,n)*x1y4z1*12.+c(3,4,
     & 3,n)*x1y4z2*18.+c(3,4,4,n)*x1y4z3*24.+c(4,0,1,n)*x2*12.+c(4,0,
     & 2,n)*x2z1*24.+c(4,0,3,n)*x2z2*36.+c(4,0,4,n)*x2z3*48.+c(4,1,1,
     & n)*x2y1*12.+c(4,1,2,n)*x2y1z1*24.+c(4,1,3,n)*x2y1z2*36.+c(4,1,
     & 4,n)*x2y1z3*48.+c(4,2,1,n)*x2y2*12.+c(4,2,2,n)*x2y2z1*24.+c(4,
     & 2,3,n)*x2y2z2*36.+c(4,2,4,n)*x2y2z3*48.+c(4,3,1,n)*x2y3*12.+c(
     & 4,3,2,n)*x2y3z1*24.+c(4,3,3,n)*x2y3z2*36.+c(4,3,4,n)*x2y3z3*
     & 48.+c(4,4,1,n)*x2y4*12.+c(4,4,2,n)*x2y4z1*24.+c(4,4,3,n)*
     & x2y4z2*36.+c(4,4,4,n)*x2y4z3*48.)*time
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
      r(i1,i2,i3,n)=(c(2,0,2,n)*4.+c(2,0,3,n)*z1*12.+c(2,0,4,n)*z2*24.+
     & c(2,1,2,n)*y1*4.+c(2,1,3,n)*y1z1*12.+c(2,1,4,n)*y1z2*24.+c(2,2,
     & 2,n)*y2*4.+c(2,2,3,n)*y2z1*12.+c(2,2,4,n)*y2z2*24.+c(2,3,2,n)*
     & y3*4.+c(2,3,3,n)*y3z1*12.+c(2,3,4,n)*y3z2*24.+c(2,4,2,n)*y4*4.+
     & c(2,4,3,n)*y4z1*12.+c(2,4,4,n)*y4z2*24.+c(3,0,2,n)*x1*12.+c(3,
     & 0,3,n)*x1z1*36.+c(3,0,4,n)*x1z2*72.+c(3,1,2,n)*x1y1*12.+c(3,1,
     & 3,n)*x1y1z1*36.+c(3,1,4,n)*x1y1z2*72.+c(3,2,2,n)*x1y2*12.+c(3,
     & 2,3,n)*x1y2z1*36.+c(3,2,4,n)*x1y2z2*72.+c(3,3,2,n)*x1y3*12.+c(
     & 3,3,3,n)*x1y3z1*36.+c(3,3,4,n)*x1y3z2*72.+c(3,4,2,n)*x1y4*12.+
     & c(3,4,3,n)*x1y4z1*36.+c(3,4,4,n)*x1y4z2*72.+c(4,0,2,n)*x2*24.+
     & c(4,0,3,n)*x2z1*72.+c(4,0,4,n)*x2z2*144.+c(4,1,2,n)*x2y1*24.+c(
     & 4,1,3,n)*x2y1z1*72.+c(4,1,4,n)*x2y1z2*144.+c(4,2,2,n)*x2y2*24.+
     & c(4,2,3,n)*x2y2z1*72.+c(4,2,4,n)*x2y2z2*144.+c(4,3,2,n)*x2y3*
     & 24.+c(4,3,3,n)*x2y3z1*72.+c(4,3,4,n)*x2y3z2*144.+c(4,4,2,n)*
     & x2y4*24.+c(4,4,3,n)*x2y4z1*72.+c(4,4,4,n)*x2y4z2*144.)*time
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
      r(i1,i2,i3,n)=(c(2,0,3,n)*12.+c(2,0,4,n)*z1*48.+c(2,1,3,n)*y1*
     & 12.+c(2,1,4,n)*y1z1*48.+c(2,2,3,n)*y2*12.+c(2,2,4,n)*y2z1*48.+
     & c(2,3,3,n)*y3*12.+c(2,3,4,n)*y3z1*48.+c(2,4,3,n)*y4*12.+c(2,4,
     & 4,n)*y4z1*48.+c(3,0,3,n)*x1*36.+c(3,0,4,n)*x1z1*144.+c(3,1,3,n)
     & *x1y1*36.+c(3,1,4,n)*x1y1z1*144.+c(3,2,3,n)*x1y2*36.+c(3,2,4,n)
     & *x1y2z1*144.+c(3,3,3,n)*x1y3*36.+c(3,3,4,n)*x1y3z1*144.+c(3,4,
     & 3,n)*x1y4*36.+c(3,4,4,n)*x1y4z1*144.+c(4,0,3,n)*x2*72.+c(4,0,4,
     & n)*x2z1*288.+c(4,1,3,n)*x2y1*72.+c(4,1,4,n)*x2y1z1*288.+c(4,2,
     & 3,n)*x2y2*72.+c(4,2,4,n)*x2y2z1*288.+c(4,3,3,n)*x2y3*72.+c(4,3,
     & 4,n)*x2y3z1*288.+c(4,4,3,n)*x2y4*72.+c(4,4,4,n)*x2y4z1*288.)*
     & time
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
      y1=ya(i1,i2,i3)
      y2=y1*y1
      y3=y2*y1
      y4=y3*y1
      x1=xa(i1,i2,i3)
      x1y1=x1*y1
      x1y2=x1y1*y1
      x1y3=x1y2*y1
      x1y4=x1y3*y1
      x2=x1*x1
      x2y1=x1y1*x1
      x2y2=x1y2*x1
      x2y3=x1y3*x1
      x2y4=x1y4*x1
      r(i1,i2,i3,n)=(c(2,0,4,n)*48.+c(2,1,4,n)*y1*48.+c(2,2,4,n)*y2*
     & 48.+c(2,3,4,n)*y3*48.+c(2,4,4,n)*y4*48.+c(3,0,4,n)*x1*144.+c(3,
     & 1,4,n)*x1y1*144.+c(3,2,4,n)*x1y2*144.+c(3,3,4,n)*x1y3*144.+c(3,
     & 4,4,n)*x1y4*144.+c(4,0,4,n)*x2*288.+c(4,1,4,n)*x2y1*288.+c(4,2,
     & 4,n)*x2y2*288.+c(4,3,4,n)*x2y3*288.+c(4,4,4,n)*x2y4*288.)*time
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
      r(i1,i2,i3,n)=(c(2,1,0,n)*2.+c(2,1,1,n)*z1*2.+c(2,1,2,n)*z2*2.+c(
     & 2,1,3,n)*z3*2.+c(2,1,4,n)*z4*2.+c(2,2,0,n)*y1*4.+c(2,2,1,n)*
     & y1z1*4.+c(2,2,2,n)*y1z2*4.+c(2,2,3,n)*y1z3*4.+c(2,2,4,n)*y1z4*
     & 4.+c(2,3,0,n)*y2*6.+c(2,3,1,n)*y2z1*6.+c(2,3,2,n)*y2z2*6.+c(2,
     & 3,3,n)*y2z3*6.+c(2,3,4,n)*y2z4*6.+c(2,4,0,n)*y3*8.+c(2,4,1,n)*
     & y3z1*8.+c(2,4,2,n)*y3z2*8.+c(2,4,3,n)*y3z3*8.+c(2,4,4,n)*y3z4*
     & 8.+c(3,1,0,n)*x1*6.+c(3,1,1,n)*x1z1*6.+c(3,1,2,n)*x1z2*6.+c(3,
     & 1,3,n)*x1z3*6.+c(3,1,4,n)*x1z4*6.+c(3,2,0,n)*x1y1*12.+c(3,2,1,
     & n)*x1y1z1*12.+c(3,2,2,n)*x1y1z2*12.+c(3,2,3,n)*x1y1z3*12.+c(3,
     & 2,4,n)*x1y1z4*12.+c(3,3,0,n)*x1y2*18.+c(3,3,1,n)*x1y2z1*18.+c(
     & 3,3,2,n)*x1y2z2*18.+c(3,3,3,n)*x1y2z3*18.+c(3,3,4,n)*x1y2z4*
     & 18.+c(3,4,0,n)*x1y3*24.+c(3,4,1,n)*x1y3z1*24.+c(3,4,2,n)*
     & x1y3z2*24.+c(3,4,3,n)*x1y3z3*24.+c(3,4,4,n)*x1y3z4*24.+c(4,1,0,
     & n)*x2*12.+c(4,1,1,n)*x2z1*12.+c(4,1,2,n)*x2z2*12.+c(4,1,3,n)*
     & x2z3*12.+c(4,1,4,n)*x2z4*12.+c(4,2,0,n)*x2y1*24.+c(4,2,1,n)*
     & x2y1z1*24.+c(4,2,2,n)*x2y1z2*24.+c(4,2,3,n)*x2y1z3*24.+c(4,2,4,
     & n)*x2y1z4*24.+c(4,3,0,n)*x2y2*36.+c(4,3,1,n)*x2y2z1*36.+c(4,3,
     & 2,n)*x2y2z2*36.+c(4,3,3,n)*x2y2z3*36.+c(4,3,4,n)*x2y2z4*36.+c(
     & 4,4,0,n)*x2y3*48.+c(4,4,1,n)*x2y3z1*48.+c(4,4,2,n)*x2y3z2*48.+
     & c(4,4,3,n)*x2y3z3*48.+c(4,4,4,n)*x2y3z4*48.)*time
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
      r(i1,i2,i3,n)=(c(2,1,1,n)*2.+c(2,1,2,n)*z1*4.+c(2,1,3,n)*z2*6.+c(
     & 2,1,4,n)*z3*8.+c(2,2,1,n)*y1*4.+c(2,2,2,n)*y1z1*8.+c(2,2,3,n)*
     & y1z2*12.+c(2,2,4,n)*y1z3*16.+c(2,3,1,n)*y2*6.+c(2,3,2,n)*y2z1*
     & 12.+c(2,3,3,n)*y2z2*18.+c(2,3,4,n)*y2z3*24.+c(2,4,1,n)*y3*8.+c(
     & 2,4,2,n)*y3z1*16.+c(2,4,3,n)*y3z2*24.+c(2,4,4,n)*y3z3*32.+c(3,
     & 1,1,n)*x1*6.+c(3,1,2,n)*x1z1*12.+c(3,1,3,n)*x1z2*18.+c(3,1,4,n)
     & *x1z3*24.+c(3,2,1,n)*x1y1*12.+c(3,2,2,n)*x1y1z1*24.+c(3,2,3,n)*
     & x1y1z2*36.+c(3,2,4,n)*x1y1z3*48.+c(3,3,1,n)*x1y2*18.+c(3,3,2,n)
     & *x1y2z1*36.+c(3,3,3,n)*x1y2z2*54.+c(3,3,4,n)*x1y2z3*72.+c(3,4,
     & 1,n)*x1y3*24.+c(3,4,2,n)*x1y3z1*48.+c(3,4,3,n)*x1y3z2*72.+c(3,
     & 4,4,n)*x1y3z3*96.+c(4,1,1,n)*x2*12.+c(4,1,2,n)*x2z1*24.+c(4,1,
     & 3,n)*x2z2*36.+c(4,1,4,n)*x2z3*48.+c(4,2,1,n)*x2y1*24.+c(4,2,2,
     & n)*x2y1z1*48.+c(4,2,3,n)*x2y1z2*72.+c(4,2,4,n)*x2y1z3*96.+c(4,
     & 3,1,n)*x2y2*36.+c(4,3,2,n)*x2y2z1*72.+c(4,3,3,n)*x2y2z2*108.+c(
     & 4,3,4,n)*x2y2z3*144.+c(4,4,1,n)*x2y3*48.+c(4,4,2,n)*x2y3z1*96.+
     & c(4,4,3,n)*x2y3z2*144.+c(4,4,4,n)*x2y3z3*192.)*time
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
      r(i1,i2,i3,n)=(c(2,1,2,n)*4.+c(2,1,3,n)*z1*12.+c(2,1,4,n)*z2*24.+
     & c(2,2,2,n)*y1*8.+c(2,2,3,n)*y1z1*24.+c(2,2,4,n)*y1z2*48.+c(2,3,
     & 2,n)*y2*12.+c(2,3,3,n)*y2z1*36.+c(2,3,4,n)*y2z2*72.+c(2,4,2,n)*
     & y3*16.+c(2,4,3,n)*y3z1*48.+c(2,4,4,n)*y3z2*96.+c(3,1,2,n)*x1*
     & 12.+c(3,1,3,n)*x1z1*36.+c(3,1,4,n)*x1z2*72.+c(3,2,2,n)*x1y1*
     & 24.+c(3,2,3,n)*x1y1z1*72.+c(3,2,4,n)*x1y1z2*144.+c(3,3,2,n)*
     & x1y2*36.+c(3,3,3,n)*x1y2z1*108.+c(3,3,4,n)*x1y2z2*216.+c(3,4,2,
     & n)*x1y3*48.+c(3,4,3,n)*x1y3z1*144.+c(3,4,4,n)*x1y3z2*288.+c(4,
     & 1,2,n)*x2*24.+c(4,1,3,n)*x2z1*72.+c(4,1,4,n)*x2z2*144.+c(4,2,2,
     & n)*x2y1*48.+c(4,2,3,n)*x2y1z1*144.+c(4,2,4,n)*x2y1z2*288.+c(4,
     & 3,2,n)*x2y2*72.+c(4,3,3,n)*x2y2z1*216.+c(4,3,4,n)*x2y2z2*432.+
     & c(4,4,2,n)*x2y3*96.+c(4,4,3,n)*x2y3z1*288.+c(4,4,4,n)*x2y3z2*
     & 576.)*time
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
      r(i1,i2,i3,n)=(c(2,1,3,n)*12.+c(2,1,4,n)*z1*48.+c(2,2,3,n)*y1*
     & 24.+c(2,2,4,n)*y1z1*96.+c(2,3,3,n)*y2*36.+c(2,3,4,n)*y2z1*144.+
     & c(2,4,3,n)*y3*48.+c(2,4,4,n)*y3z1*192.+c(3,1,3,n)*x1*36.+c(3,1,
     & 4,n)*x1z1*144.+c(3,2,3,n)*x1y1*72.+c(3,2,4,n)*x1y1z1*288.+c(3,
     & 3,3,n)*x1y2*108.+c(3,3,4,n)*x1y2z1*432.+c(3,4,3,n)*x1y3*144.+c(
     & 3,4,4,n)*x1y3z1*576.+c(4,1,3,n)*x2*72.+c(4,1,4,n)*x2z1*288.+c(
     & 4,2,3,n)*x2y1*144.+c(4,2,4,n)*x2y1z1*576.+c(4,3,3,n)*x2y2*216.+
     & c(4,3,4,n)*x2y2z1*864.+c(4,4,3,n)*x2y3*288.+c(4,4,4,n)*x2y3z1*
     & 1152.)*time
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
      y1=ya(i1,i2,i3)
      y2=y1*y1
      y3=y2*y1
      x1=xa(i1,i2,i3)
      x1y1=x1*y1
      x1y2=x1y1*y1
      x1y3=x1y2*y1
      x2=x1*x1
      x2y1=x1y1*x1
      x2y2=x1y2*x1
      x2y3=x1y3*x1
      r(i1,i2,i3,n)=(c(2,1,4,n)*48.+c(2,2,4,n)*y1*96.+c(2,3,4,n)*y2*
     & 144.+c(2,4,4,n)*y3*192.+c(3,1,4,n)*x1*144.+c(3,2,4,n)*x1y1*
     & 288.+c(3,3,4,n)*x1y2*432.+c(3,4,4,n)*x1y3*576.+c(4,1,4,n)*x2*
     & 288.+c(4,2,4,n)*x2y1*576.+c(4,3,4,n)*x2y2*864.+c(4,4,4,n)*x2y3*
     & 1152.)*time
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
      r(i1,i2,i3,n)=(c(2,2,0,n)*4.+c(2,2,1,n)*z1*4.+c(2,2,2,n)*z2*4.+c(
     & 2,2,3,n)*z3*4.+c(2,2,4,n)*z4*4.+c(2,3,0,n)*y1*12.+c(2,3,1,n)*
     & y1z1*12.+c(2,3,2,n)*y1z2*12.+c(2,3,3,n)*y1z3*12.+c(2,3,4,n)*
     & y1z4*12.+c(2,4,0,n)*y2*24.+c(2,4,1,n)*y2z1*24.+c(2,4,2,n)*y2z2*
     & 24.+c(2,4,3,n)*y2z3*24.+c(2,4,4,n)*y2z4*24.+c(3,2,0,n)*x1*12.+
     & c(3,2,1,n)*x1z1*12.+c(3,2,2,n)*x1z2*12.+c(3,2,3,n)*x1z3*12.+c(
     & 3,2,4,n)*x1z4*12.+c(3,3,0,n)*x1y1*36.+c(3,3,1,n)*x1y1z1*36.+c(
     & 3,3,2,n)*x1y1z2*36.+c(3,3,3,n)*x1y1z3*36.+c(3,3,4,n)*x1y1z4*
     & 36.+c(3,4,0,n)*x1y2*72.+c(3,4,1,n)*x1y2z1*72.+c(3,4,2,n)*
     & x1y2z2*72.+c(3,4,3,n)*x1y2z3*72.+c(3,4,4,n)*x1y2z4*72.+c(4,2,0,
     & n)*x2*24.+c(4,2,1,n)*x2z1*24.+c(4,2,2,n)*x2z2*24.+c(4,2,3,n)*
     & x2z3*24.+c(4,2,4,n)*x2z4*24.+c(4,3,0,n)*x2y1*72.+c(4,3,1,n)*
     & x2y1z1*72.+c(4,3,2,n)*x2y1z2*72.+c(4,3,3,n)*x2y1z3*72.+c(4,3,4,
     & n)*x2y1z4*72.+c(4,4,0,n)*x2y2*144.+c(4,4,1,n)*x2y2z1*144.+c(4,
     & 4,2,n)*x2y2z2*144.+c(4,4,3,n)*x2y2z3*144.+c(4,4,4,n)*x2y2z4*
     & 144.)*time
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
      r(i1,i2,i3,n)=(c(2,2,1,n)*4.+c(2,2,2,n)*z1*8.+c(2,2,3,n)*z2*12.+
     & c(2,2,4,n)*z3*16.+c(2,3,1,n)*y1*12.+c(2,3,2,n)*y1z1*24.+c(2,3,
     & 3,n)*y1z2*36.+c(2,3,4,n)*y1z3*48.+c(2,4,1,n)*y2*24.+c(2,4,2,n)*
     & y2z1*48.+c(2,4,3,n)*y2z2*72.+c(2,4,4,n)*y2z3*96.+c(3,2,1,n)*x1*
     & 12.+c(3,2,2,n)*x1z1*24.+c(3,2,3,n)*x1z2*36.+c(3,2,4,n)*x1z3*
     & 48.+c(3,3,1,n)*x1y1*36.+c(3,3,2,n)*x1y1z1*72.+c(3,3,3,n)*
     & x1y1z2*108.+c(3,3,4,n)*x1y1z3*144.+c(3,4,1,n)*x1y2*72.+c(3,4,2,
     & n)*x1y2z1*144.+c(3,4,3,n)*x1y2z2*216.+c(3,4,4,n)*x1y2z3*288.+c(
     & 4,2,1,n)*x2*24.+c(4,2,2,n)*x2z1*48.+c(4,2,3,n)*x2z2*72.+c(4,2,
     & 4,n)*x2z3*96.+c(4,3,1,n)*x2y1*72.+c(4,3,2,n)*x2y1z1*144.+c(4,3,
     & 3,n)*x2y1z2*216.+c(4,3,4,n)*x2y1z3*288.+c(4,4,1,n)*x2y2*144.+c(
     & 4,4,2,n)*x2y2z1*288.+c(4,4,3,n)*x2y2z2*432.+c(4,4,4,n)*x2y2z3*
     & 576.)*time
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
      r(i1,i2,i3,n)=(c(2,2,2,n)*8.+c(2,2,3,n)*z1*24.+c(2,2,4,n)*z2*48.+
     & c(2,3,2,n)*y1*24.+c(2,3,3,n)*y1z1*72.+c(2,3,4,n)*y1z2*144.+c(2,
     & 4,2,n)*y2*48.+c(2,4,3,n)*y2z1*144.+c(2,4,4,n)*y2z2*288.+c(3,2,
     & 2,n)*x1*24.+c(3,2,3,n)*x1z1*72.+c(3,2,4,n)*x1z2*144.+c(3,3,2,n)
     & *x1y1*72.+c(3,3,3,n)*x1y1z1*216.+c(3,3,4,n)*x1y1z2*432.+c(3,4,
     & 2,n)*x1y2*144.+c(3,4,3,n)*x1y2z1*432.+c(3,4,4,n)*x1y2z2*864.+c(
     & 4,2,2,n)*x2*48.+c(4,2,3,n)*x2z1*144.+c(4,2,4,n)*x2z2*288.+c(4,
     & 3,2,n)*x2y1*144.+c(4,3,3,n)*x2y1z1*432.+c(4,3,4,n)*x2y1z2*864.+
     & c(4,4,2,n)*x2y2*288.+c(4,4,3,n)*x2y2z1*864.+c(4,4,4,n)*x2y2z2*
     & 1728.)*time
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
      r(i1,i2,i3,n)=(c(2,2,3,n)*24.+c(2,2,4,n)*z1*96.+c(2,3,3,n)*y1*
     & 72.+c(2,3,4,n)*y1z1*288.+c(2,4,3,n)*y2*144.+c(2,4,4,n)*y2z1*
     & 576.+c(3,2,3,n)*x1*72.+c(3,2,4,n)*x1z1*288.+c(3,3,3,n)*x1y1*
     & 216.+c(3,3,4,n)*x1y1z1*864.+c(3,4,3,n)*x1y2*432.+c(3,4,4,n)*
     & x1y2z1*1728.+c(4,2,3,n)*x2*144.+c(4,2,4,n)*x2z1*576.+c(4,3,3,n)
     & *x2y1*432.+c(4,3,4,n)*x2y1z1*1728.+c(4,4,3,n)*x2y2*864.+c(4,4,
     & 4,n)*x2y2z1*3456.)*time
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
      y1=ya(i1,i2,i3)
      y2=y1*y1
      x1=xa(i1,i2,i3)
      x1y1=x1*y1
      x1y2=x1y1*y1
      x2=x1*x1
      x2y1=x1y1*x1
      x2y2=x1y2*x1
      r(i1,i2,i3,n)=(c(2,2,4,n)*96.+c(2,3,4,n)*y1*288.+c(2,4,4,n)*y2*
     & 576.+c(3,2,4,n)*x1*288.+c(3,3,4,n)*x1y1*864.+c(3,4,4,n)*x1y2*
     & 1728.+c(4,2,4,n)*x2*576.+c(4,3,4,n)*x2y1*1728.+c(4,4,4,n)*x2y2*
     & 3456.)*time
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
      r(i1,i2,i3,n)=(c(2,3,0,n)*12.+c(2,3,1,n)*z1*12.+c(2,3,2,n)*z2*
     & 12.+c(2,3,3,n)*z3*12.+c(2,3,4,n)*z4*12.+c(2,4,0,n)*y1*48.+c(2,
     & 4,1,n)*y1z1*48.+c(2,4,2,n)*y1z2*48.+c(2,4,3,n)*y1z3*48.+c(2,4,
     & 4,n)*y1z4*48.+c(3,3,0,n)*x1*36.+c(3,3,1,n)*x1z1*36.+c(3,3,2,n)*
     & x1z2*36.+c(3,3,3,n)*x1z3*36.+c(3,3,4,n)*x1z4*36.+c(3,4,0,n)*
     & x1y1*144.+c(3,4,1,n)*x1y1z1*144.+c(3,4,2,n)*x1y1z2*144.+c(3,4,
     & 3,n)*x1y1z3*144.+c(3,4,4,n)*x1y1z4*144.+c(4,3,0,n)*x2*72.+c(4,
     & 3,1,n)*x2z1*72.+c(4,3,2,n)*x2z2*72.+c(4,3,3,n)*x2z3*72.+c(4,3,
     & 4,n)*x2z4*72.+c(4,4,0,n)*x2y1*288.+c(4,4,1,n)*x2y1z1*288.+c(4,
     & 4,2,n)*x2y1z2*288.+c(4,4,3,n)*x2y1z3*288.+c(4,4,4,n)*x2y1z4*
     & 288.)*time
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
      r(i1,i2,i3,n)=(c(2,3,1,n)*12.+c(2,3,2,n)*z1*24.+c(2,3,3,n)*z2*
     & 36.+c(2,3,4,n)*z3*48.+c(2,4,1,n)*y1*48.+c(2,4,2,n)*y1z1*96.+c(
     & 2,4,3,n)*y1z2*144.+c(2,4,4,n)*y1z3*192.+c(3,3,1,n)*x1*36.+c(3,
     & 3,2,n)*x1z1*72.+c(3,3,3,n)*x1z2*108.+c(3,3,4,n)*x1z3*144.+c(3,
     & 4,1,n)*x1y1*144.+c(3,4,2,n)*x1y1z1*288.+c(3,4,3,n)*x1y1z2*432.+
     & c(3,4,4,n)*x1y1z3*576.+c(4,3,1,n)*x2*72.+c(4,3,2,n)*x2z1*144.+
     & c(4,3,3,n)*x2z2*216.+c(4,3,4,n)*x2z3*288.+c(4,4,1,n)*x2y1*288.+
     & c(4,4,2,n)*x2y1z1*576.+c(4,4,3,n)*x2y1z2*864.+c(4,4,4,n)*
     & x2y1z3*1152.)*time
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
      r(i1,i2,i3,n)=(c(2,3,2,n)*24.+c(2,3,3,n)*z1*72.+c(2,3,4,n)*z2*
     & 144.+c(2,4,2,n)*y1*96.+c(2,4,3,n)*y1z1*288.+c(2,4,4,n)*y1z2*
     & 576.+c(3,3,2,n)*x1*72.+c(3,3,3,n)*x1z1*216.+c(3,3,4,n)*x1z2*
     & 432.+c(3,4,2,n)*x1y1*288.+c(3,4,3,n)*x1y1z1*864.+c(3,4,4,n)*
     & x1y1z2*1728.+c(4,3,2,n)*x2*144.+c(4,3,3,n)*x2z1*432.+c(4,3,4,n)
     & *x2z2*864.+c(4,4,2,n)*x2y1*576.+c(4,4,3,n)*x2y1z1*1728.+c(4,4,
     & 4,n)*x2y1z2*3456.)*time
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
      r(i1,i2,i3,n)=(c(2,3,3,n)*72.+c(2,3,4,n)*z1*288.+c(2,4,3,n)*y1*
     & 288.+c(2,4,4,n)*y1z1*1152.+c(3,3,3,n)*x1*216.+c(3,3,4,n)*x1z1*
     & 864.+c(3,4,3,n)*x1y1*864.+c(3,4,4,n)*x1y1z1*3456.+c(4,3,3,n)*
     & x2*432.+c(4,3,4,n)*x2z1*1728.+c(4,4,3,n)*x2y1*1728.+c(4,4,4,n)*
     & x2y1z1*6912.)*time
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
      y1=ya(i1,i2,i3)
      x1=xa(i1,i2,i3)
      x1y1=x1*y1
      x2=x1*x1
      x2y1=x1y1*x1
      r(i1,i2,i3,n)=(c(2,3,4,n)*288.+c(2,4,4,n)*y1*1152.+c(3,3,4,n)*x1*
     & 864.+c(3,4,4,n)*x1y1*3456.+c(4,3,4,n)*x2*1728.+c(4,4,4,n)*x2y1*
     & 6912.)*time
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
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      r(i1,i2,i3,n)=(c(2,4,0,n)*48.+c(2,4,1,n)*z1*48.+c(2,4,2,n)*z2*
     & 48.+c(2,4,3,n)*z3*48.+c(2,4,4,n)*z4*48.+c(3,4,0,n)*x1*144.+c(3,
     & 4,1,n)*x1z1*144.+c(3,4,2,n)*x1z2*144.+c(3,4,3,n)*x1z3*144.+c(3,
     & 4,4,n)*x1z4*144.+c(4,4,0,n)*x2*288.+c(4,4,1,n)*x2z1*288.+c(4,4,
     & 2,n)*x2z2*288.+c(4,4,3,n)*x2z3*288.+c(4,4,4,n)*x2z4*288.)*time
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
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      r(i1,i2,i3,n)=(c(2,4,1,n)*48.+c(2,4,2,n)*z1*96.+c(2,4,3,n)*z2*
     & 144.+c(2,4,4,n)*z3*192.+c(3,4,1,n)*x1*144.+c(3,4,2,n)*x1z1*
     & 288.+c(3,4,3,n)*x1z2*432.+c(3,4,4,n)*x1z3*576.+c(4,4,1,n)*x2*
     & 288.+c(4,4,2,n)*x2z1*576.+c(4,4,3,n)*x2z2*864.+c(4,4,4,n)*x2z3*
     & 1152.)*time
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
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      r(i1,i2,i3,n)=(c(2,4,2,n)*96.+c(2,4,3,n)*z1*288.+c(2,4,4,n)*z2*
     & 576.+c(3,4,2,n)*x1*288.+c(3,4,3,n)*x1z1*864.+c(3,4,4,n)*x1z2*
     & 1728.+c(4,4,2,n)*x2*576.+c(4,4,3,n)*x2z1*1728.+c(4,4,4,n)*x2z2*
     & 3456.)*time
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
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x2=x1*x1
      x2z1=x1z1*x1
      r(i1,i2,i3,n)=(c(2,4,3,n)*288.+c(2,4,4,n)*z1*1152.+c(3,4,3,n)*x1*
     & 864.+c(3,4,4,n)*x1z1*3456.+c(4,4,3,n)*x2*1728.+c(4,4,4,n)*x2z1*
     & 6912.)*time
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
      x1=xa(i1,i2,i3)
      x2=x1*x1
      r(i1,i2,i3,n)=(c(2,4,4,n)*1152.+c(3,4,4,n)*x1*3456.+c(4,4,4,n)*
     & x2*6912.)*time
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
      r(i1,i2,i3,n)=(c(3,0,0,n)*6.+c(3,0,1,n)*z1*6.+c(3,0,2,n)*z2*6.+c(
     & 3,0,3,n)*z3*6.+c(3,0,4,n)*z4*6.+c(3,1,0,n)*y1*6.+c(3,1,1,n)*
     & y1z1*6.+c(3,1,2,n)*y1z2*6.+c(3,1,3,n)*y1z3*6.+c(3,1,4,n)*y1z4*
     & 6.+c(3,2,0,n)*y2*6.+c(3,2,1,n)*y2z1*6.+c(3,2,2,n)*y2z2*6.+c(3,
     & 2,3,n)*y2z3*6.+c(3,2,4,n)*y2z4*6.+c(3,3,0,n)*y3*6.+c(3,3,1,n)*
     & y3z1*6.+c(3,3,2,n)*y3z2*6.+c(3,3,3,n)*y3z3*6.+c(3,3,4,n)*y3z4*
     & 6.+c(3,4,0,n)*y4*6.+c(3,4,1,n)*y4z1*6.+c(3,4,2,n)*y4z2*6.+c(3,
     & 4,3,n)*y4z3*6.+c(3,4,4,n)*y4z4*6.+c(4,0,0,n)*x1*24.+c(4,0,1,n)*
     & x1z1*24.+c(4,0,2,n)*x1z2*24.+c(4,0,3,n)*x1z3*24.+c(4,0,4,n)*
     & x1z4*24.+c(4,1,0,n)*x1y1*24.+c(4,1,1,n)*x1y1z1*24.+c(4,1,2,n)*
     & x1y1z2*24.+c(4,1,3,n)*x1y1z3*24.+c(4,1,4,n)*x1y1z4*24.+c(4,2,0,
     & n)*x1y2*24.+c(4,2,1,n)*x1y2z1*24.+c(4,2,2,n)*x1y2z2*24.+c(4,2,
     & 3,n)*x1y2z3*24.+c(4,2,4,n)*x1y2z4*24.+c(4,3,0,n)*x1y3*24.+c(4,
     & 3,1,n)*x1y3z1*24.+c(4,3,2,n)*x1y3z2*24.+c(4,3,3,n)*x1y3z3*24.+
     & c(4,3,4,n)*x1y3z4*24.+c(4,4,0,n)*x1y4*24.+c(4,4,1,n)*x1y4z1*
     & 24.+c(4,4,2,n)*x1y4z2*24.+c(4,4,3,n)*x1y4z3*24.+c(4,4,4,n)*
     & x1y4z4*24.)*time
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
      r(i1,i2,i3,n)=(c(3,0,1,n)*6.+c(3,0,2,n)*z1*12.+c(3,0,3,n)*z2*18.+
     & c(3,0,4,n)*z3*24.+c(3,1,1,n)*y1*6.+c(3,1,2,n)*y1z1*12.+c(3,1,3,
     & n)*y1z2*18.+c(3,1,4,n)*y1z3*24.+c(3,2,1,n)*y2*6.+c(3,2,2,n)*
     & y2z1*12.+c(3,2,3,n)*y2z2*18.+c(3,2,4,n)*y2z3*24.+c(3,3,1,n)*y3*
     & 6.+c(3,3,2,n)*y3z1*12.+c(3,3,3,n)*y3z2*18.+c(3,3,4,n)*y3z3*24.+
     & c(3,4,1,n)*y4*6.+c(3,4,2,n)*y4z1*12.+c(3,4,3,n)*y4z2*18.+c(3,4,
     & 4,n)*y4z3*24.+c(4,0,1,n)*x1*24.+c(4,0,2,n)*x1z1*48.+c(4,0,3,n)*
     & x1z2*72.+c(4,0,4,n)*x1z3*96.+c(4,1,1,n)*x1y1*24.+c(4,1,2,n)*
     & x1y1z1*48.+c(4,1,3,n)*x1y1z2*72.+c(4,1,4,n)*x1y1z3*96.+c(4,2,1,
     & n)*x1y2*24.+c(4,2,2,n)*x1y2z1*48.+c(4,2,3,n)*x1y2z2*72.+c(4,2,
     & 4,n)*x1y2z3*96.+c(4,3,1,n)*x1y3*24.+c(4,3,2,n)*x1y3z1*48.+c(4,
     & 3,3,n)*x1y3z2*72.+c(4,3,4,n)*x1y3z3*96.+c(4,4,1,n)*x1y4*24.+c(
     & 4,4,2,n)*x1y4z1*48.+c(4,4,3,n)*x1y4z2*72.+c(4,4,4,n)*x1y4z3*
     & 96.)*time
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
      r(i1,i2,i3,n)=(c(3,0,2,n)*12.+c(3,0,3,n)*z1*36.+c(3,0,4,n)*z2*
     & 72.+c(3,1,2,n)*y1*12.+c(3,1,3,n)*y1z1*36.+c(3,1,4,n)*y1z2*72.+
     & c(3,2,2,n)*y2*12.+c(3,2,3,n)*y2z1*36.+c(3,2,4,n)*y2z2*72.+c(3,
     & 3,2,n)*y3*12.+c(3,3,3,n)*y3z1*36.+c(3,3,4,n)*y3z2*72.+c(3,4,2,
     & n)*y4*12.+c(3,4,3,n)*y4z1*36.+c(3,4,4,n)*y4z2*72.+c(4,0,2,n)*
     & x1*48.+c(4,0,3,n)*x1z1*144.+c(4,0,4,n)*x1z2*288.+c(4,1,2,n)*
     & x1y1*48.+c(4,1,3,n)*x1y1z1*144.+c(4,1,4,n)*x1y1z2*288.+c(4,2,2,
     & n)*x1y2*48.+c(4,2,3,n)*x1y2z1*144.+c(4,2,4,n)*x1y2z2*288.+c(4,
     & 3,2,n)*x1y3*48.+c(4,3,3,n)*x1y3z1*144.+c(4,3,4,n)*x1y3z2*288.+
     & c(4,4,2,n)*x1y4*48.+c(4,4,3,n)*x1y4z1*144.+c(4,4,4,n)*x1y4z2*
     & 288.)*time
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
      r(i1,i2,i3,n)=(c(3,0,3,n)*36.+c(3,0,4,n)*z1*144.+c(3,1,3,n)*y1*
     & 36.+c(3,1,4,n)*y1z1*144.+c(3,2,3,n)*y2*36.+c(3,2,4,n)*y2z1*
     & 144.+c(3,3,3,n)*y3*36.+c(3,3,4,n)*y3z1*144.+c(3,4,3,n)*y4*36.+
     & c(3,4,4,n)*y4z1*144.+c(4,0,3,n)*x1*144.+c(4,0,4,n)*x1z1*576.+c(
     & 4,1,3,n)*x1y1*144.+c(4,1,4,n)*x1y1z1*576.+c(4,2,3,n)*x1y2*144.+
     & c(4,2,4,n)*x1y2z1*576.+c(4,3,3,n)*x1y3*144.+c(4,3,4,n)*x1y3z1*
     & 576.+c(4,4,3,n)*x1y4*144.+c(4,4,4,n)*x1y4z1*576.)*time
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
      y1=ya(i1,i2,i3)
      y2=y1*y1
      y3=y2*y1
      y4=y3*y1
      x1=xa(i1,i2,i3)
      x1y1=x1*y1
      x1y2=x1y1*y1
      x1y3=x1y2*y1
      x1y4=x1y3*y1
      r(i1,i2,i3,n)=(c(3,0,4,n)*144.+c(3,1,4,n)*y1*144.+c(3,2,4,n)*y2*
     & 144.+c(3,3,4,n)*y3*144.+c(3,4,4,n)*y4*144.+c(4,0,4,n)*x1*576.+
     & c(4,1,4,n)*x1y1*576.+c(4,2,4,n)*x1y2*576.+c(4,3,4,n)*x1y3*576.+
     & c(4,4,4,n)*x1y4*576.)*time
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
      r(i1,i2,i3,n)=(c(3,1,0,n)*6.+c(3,1,1,n)*z1*6.+c(3,1,2,n)*z2*6.+c(
     & 3,1,3,n)*z3*6.+c(3,1,4,n)*z4*6.+c(3,2,0,n)*y1*12.+c(3,2,1,n)*
     & y1z1*12.+c(3,2,2,n)*y1z2*12.+c(3,2,3,n)*y1z3*12.+c(3,2,4,n)*
     & y1z4*12.+c(3,3,0,n)*y2*18.+c(3,3,1,n)*y2z1*18.+c(3,3,2,n)*y2z2*
     & 18.+c(3,3,3,n)*y2z3*18.+c(3,3,4,n)*y2z4*18.+c(3,4,0,n)*y3*24.+
     & c(3,4,1,n)*y3z1*24.+c(3,4,2,n)*y3z2*24.+c(3,4,3,n)*y3z3*24.+c(
     & 3,4,4,n)*y3z4*24.+c(4,1,0,n)*x1*24.+c(4,1,1,n)*x1z1*24.+c(4,1,
     & 2,n)*x1z2*24.+c(4,1,3,n)*x1z3*24.+c(4,1,4,n)*x1z4*24.+c(4,2,0,
     & n)*x1y1*48.+c(4,2,1,n)*x1y1z1*48.+c(4,2,2,n)*x1y1z2*48.+c(4,2,
     & 3,n)*x1y1z3*48.+c(4,2,4,n)*x1y1z4*48.+c(4,3,0,n)*x1y2*72.+c(4,
     & 3,1,n)*x1y2z1*72.+c(4,3,2,n)*x1y2z2*72.+c(4,3,3,n)*x1y2z3*72.+
     & c(4,3,4,n)*x1y2z4*72.+c(4,4,0,n)*x1y3*96.+c(4,4,1,n)*x1y3z1*
     & 96.+c(4,4,2,n)*x1y3z2*96.+c(4,4,3,n)*x1y3z3*96.+c(4,4,4,n)*
     & x1y3z4*96.)*time
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
      r(i1,i2,i3,n)=(c(3,1,1,n)*6.+c(3,1,2,n)*z1*12.+c(3,1,3,n)*z2*18.+
     & c(3,1,4,n)*z3*24.+c(3,2,1,n)*y1*12.+c(3,2,2,n)*y1z1*24.+c(3,2,
     & 3,n)*y1z2*36.+c(3,2,4,n)*y1z3*48.+c(3,3,1,n)*y2*18.+c(3,3,2,n)*
     & y2z1*36.+c(3,3,3,n)*y2z2*54.+c(3,3,4,n)*y2z3*72.+c(3,4,1,n)*y3*
     & 24.+c(3,4,2,n)*y3z1*48.+c(3,4,3,n)*y3z2*72.+c(3,4,4,n)*y3z3*
     & 96.+c(4,1,1,n)*x1*24.+c(4,1,2,n)*x1z1*48.+c(4,1,3,n)*x1z2*72.+
     & c(4,1,4,n)*x1z3*96.+c(4,2,1,n)*x1y1*48.+c(4,2,2,n)*x1y1z1*96.+
     & c(4,2,3,n)*x1y1z2*144.+c(4,2,4,n)*x1y1z3*192.+c(4,3,1,n)*x1y2*
     & 72.+c(4,3,2,n)*x1y2z1*144.+c(4,3,3,n)*x1y2z2*216.+c(4,3,4,n)*
     & x1y2z3*288.+c(4,4,1,n)*x1y3*96.+c(4,4,2,n)*x1y3z1*192.+c(4,4,3,
     & n)*x1y3z2*288.+c(4,4,4,n)*x1y3z3*384.)*time
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
      r(i1,i2,i3,n)=(c(3,1,2,n)*12.+c(3,1,3,n)*z1*36.+c(3,1,4,n)*z2*
     & 72.+c(3,2,2,n)*y1*24.+c(3,2,3,n)*y1z1*72.+c(3,2,4,n)*y1z2*144.+
     & c(3,3,2,n)*y2*36.+c(3,3,3,n)*y2z1*108.+c(3,3,4,n)*y2z2*216.+c(
     & 3,4,2,n)*y3*48.+c(3,4,3,n)*y3z1*144.+c(3,4,4,n)*y3z2*288.+c(4,
     & 1,2,n)*x1*48.+c(4,1,3,n)*x1z1*144.+c(4,1,4,n)*x1z2*288.+c(4,2,
     & 2,n)*x1y1*96.+c(4,2,3,n)*x1y1z1*288.+c(4,2,4,n)*x1y1z2*576.+c(
     & 4,3,2,n)*x1y2*144.+c(4,3,3,n)*x1y2z1*432.+c(4,3,4,n)*x1y2z2*
     & 864.+c(4,4,2,n)*x1y3*192.+c(4,4,3,n)*x1y3z1*576.+c(4,4,4,n)*
     & x1y3z2*1152.)*time
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
      r(i1,i2,i3,n)=(c(3,1,3,n)*36.+c(3,1,4,n)*z1*144.+c(3,2,3,n)*y1*
     & 72.+c(3,2,4,n)*y1z1*288.+c(3,3,3,n)*y2*108.+c(3,3,4,n)*y2z1*
     & 432.+c(3,4,3,n)*y3*144.+c(3,4,4,n)*y3z1*576.+c(4,1,3,n)*x1*
     & 144.+c(4,1,4,n)*x1z1*576.+c(4,2,3,n)*x1y1*288.+c(4,2,4,n)*
     & x1y1z1*1152.+c(4,3,3,n)*x1y2*432.+c(4,3,4,n)*x1y2z1*1728.+c(4,
     & 4,3,n)*x1y3*576.+c(4,4,4,n)*x1y3z1*2304.)*time
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
      y1=ya(i1,i2,i3)
      y2=y1*y1
      y3=y2*y1
      x1=xa(i1,i2,i3)
      x1y1=x1*y1
      x1y2=x1y1*y1
      x1y3=x1y2*y1
      r(i1,i2,i3,n)=(c(3,1,4,n)*144.+c(3,2,4,n)*y1*288.+c(3,3,4,n)*y2*
     & 432.+c(3,4,4,n)*y3*576.+c(4,1,4,n)*x1*576.+c(4,2,4,n)*x1y1*
     & 1152.+c(4,3,4,n)*x1y2*1728.+c(4,4,4,n)*x1y3*2304.)*time
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
      r(i1,i2,i3,n)=(c(3,2,0,n)*12.+c(3,2,1,n)*z1*12.+c(3,2,2,n)*z2*
     & 12.+c(3,2,3,n)*z3*12.+c(3,2,4,n)*z4*12.+c(3,3,0,n)*y1*36.+c(3,
     & 3,1,n)*y1z1*36.+c(3,3,2,n)*y1z2*36.+c(3,3,3,n)*y1z3*36.+c(3,3,
     & 4,n)*y1z4*36.+c(3,4,0,n)*y2*72.+c(3,4,1,n)*y2z1*72.+c(3,4,2,n)*
     & y2z2*72.+c(3,4,3,n)*y2z3*72.+c(3,4,4,n)*y2z4*72.+c(4,2,0,n)*x1*
     & 48.+c(4,2,1,n)*x1z1*48.+c(4,2,2,n)*x1z2*48.+c(4,2,3,n)*x1z3*
     & 48.+c(4,2,4,n)*x1z4*48.+c(4,3,0,n)*x1y1*144.+c(4,3,1,n)*x1y1z1*
     & 144.+c(4,3,2,n)*x1y1z2*144.+c(4,3,3,n)*x1y1z3*144.+c(4,3,4,n)*
     & x1y1z4*144.+c(4,4,0,n)*x1y2*288.+c(4,4,1,n)*x1y2z1*288.+c(4,4,
     & 2,n)*x1y2z2*288.+c(4,4,3,n)*x1y2z3*288.+c(4,4,4,n)*x1y2z4*288.)
     & *time
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
      r(i1,i2,i3,n)=(c(3,2,1,n)*12.+c(3,2,2,n)*z1*24.+c(3,2,3,n)*z2*
     & 36.+c(3,2,4,n)*z3*48.+c(3,3,1,n)*y1*36.+c(3,3,2,n)*y1z1*72.+c(
     & 3,3,3,n)*y1z2*108.+c(3,3,4,n)*y1z3*144.+c(3,4,1,n)*y2*72.+c(3,
     & 4,2,n)*y2z1*144.+c(3,4,3,n)*y2z2*216.+c(3,4,4,n)*y2z3*288.+c(4,
     & 2,1,n)*x1*48.+c(4,2,2,n)*x1z1*96.+c(4,2,3,n)*x1z2*144.+c(4,2,4,
     & n)*x1z3*192.+c(4,3,1,n)*x1y1*144.+c(4,3,2,n)*x1y1z1*288.+c(4,3,
     & 3,n)*x1y1z2*432.+c(4,3,4,n)*x1y1z3*576.+c(4,4,1,n)*x1y2*288.+c(
     & 4,4,2,n)*x1y2z1*576.+c(4,4,3,n)*x1y2z2*864.+c(4,4,4,n)*x1y2z3*
     & 1152.)*time
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
      r(i1,i2,i3,n)=(c(3,2,2,n)*24.+c(3,2,3,n)*z1*72.+c(3,2,4,n)*z2*
     & 144.+c(3,3,2,n)*y1*72.+c(3,3,3,n)*y1z1*216.+c(3,3,4,n)*y1z2*
     & 432.+c(3,4,2,n)*y2*144.+c(3,4,3,n)*y2z1*432.+c(3,4,4,n)*y2z2*
     & 864.+c(4,2,2,n)*x1*96.+c(4,2,3,n)*x1z1*288.+c(4,2,4,n)*x1z2*
     & 576.+c(4,3,2,n)*x1y1*288.+c(4,3,3,n)*x1y1z1*864.+c(4,3,4,n)*
     & x1y1z2*1728.+c(4,4,2,n)*x1y2*576.+c(4,4,3,n)*x1y2z1*1728.+c(4,
     & 4,4,n)*x1y2z2*3456.)*time
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
      r(i1,i2,i3,n)=(c(3,2,3,n)*72.+c(3,2,4,n)*z1*288.+c(3,3,3,n)*y1*
     & 216.+c(3,3,4,n)*y1z1*864.+c(3,4,3,n)*y2*432.+c(3,4,4,n)*y2z1*
     & 1728.+c(4,2,3,n)*x1*288.+c(4,2,4,n)*x1z1*1152.+c(4,3,3,n)*x1y1*
     & 864.+c(4,3,4,n)*x1y1z1*3456.+c(4,4,3,n)*x1y2*1728.+c(4,4,4,n)*
     & x1y2z1*6912.)*time
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
      y1=ya(i1,i2,i3)
      y2=y1*y1
      x1=xa(i1,i2,i3)
      x1y1=x1*y1
      x1y2=x1y1*y1
      r(i1,i2,i3,n)=(c(3,2,4,n)*288.+c(3,3,4,n)*y1*864.+c(3,4,4,n)*y2*
     & 1728.+c(4,2,4,n)*x1*1152.+c(4,3,4,n)*x1y1*3456.+c(4,4,4,n)*
     & x1y2*6912.)*time
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
      r(i1,i2,i3,n)=(c(3,3,0,n)*36.+c(3,3,1,n)*z1*36.+c(3,3,2,n)*z2*
     & 36.+c(3,3,3,n)*z3*36.+c(3,3,4,n)*z4*36.+c(3,4,0,n)*y1*144.+c(3,
     & 4,1,n)*y1z1*144.+c(3,4,2,n)*y1z2*144.+c(3,4,3,n)*y1z3*144.+c(3,
     & 4,4,n)*y1z4*144.+c(4,3,0,n)*x1*144.+c(4,3,1,n)*x1z1*144.+c(4,3,
     & 2,n)*x1z2*144.+c(4,3,3,n)*x1z3*144.+c(4,3,4,n)*x1z4*144.+c(4,4,
     & 0,n)*x1y1*576.+c(4,4,1,n)*x1y1z1*576.+c(4,4,2,n)*x1y1z2*576.+c(
     & 4,4,3,n)*x1y1z3*576.+c(4,4,4,n)*x1y1z4*576.)*time
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
      r(i1,i2,i3,n)=(c(3,3,1,n)*36.+c(3,3,2,n)*z1*72.+c(3,3,3,n)*z2*
     & 108.+c(3,3,4,n)*z3*144.+c(3,4,1,n)*y1*144.+c(3,4,2,n)*y1z1*
     & 288.+c(3,4,3,n)*y1z2*432.+c(3,4,4,n)*y1z3*576.+c(4,3,1,n)*x1*
     & 144.+c(4,3,2,n)*x1z1*288.+c(4,3,3,n)*x1z2*432.+c(4,3,4,n)*x1z3*
     & 576.+c(4,4,1,n)*x1y1*576.+c(4,4,2,n)*x1y1z1*1152.+c(4,4,3,n)*
     & x1y1z2*1728.+c(4,4,4,n)*x1y1z3*2304.)*time
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
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      r(i1,i2,i3,n)=(c(3,3,2,n)*72.+c(3,3,3,n)*z1*216.+c(3,3,4,n)*z2*
     & 432.+c(3,4,2,n)*y1*288.+c(3,4,3,n)*y1z1*864.+c(3,4,4,n)*y1z2*
     & 1728.+c(4,3,2,n)*x1*288.+c(4,3,3,n)*x1z1*864.+c(4,3,4,n)*x1z2*
     & 1728.+c(4,4,2,n)*x1y1*1152.+c(4,4,3,n)*x1y1z1*3456.+c(4,4,4,n)*
     & x1y1z2*6912.)*time
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
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      r(i1,i2,i3,n)=(c(3,3,3,n)*216.+c(3,3,4,n)*z1*864.+c(3,4,3,n)*y1*
     & 864.+c(3,4,4,n)*y1z1*3456.+c(4,3,3,n)*x1*864.+c(4,3,4,n)*x1z1*
     & 3456.+c(4,4,3,n)*x1y1*3456.+c(4,4,4,n)*x1y1z1*13824.)*time
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
      y1=ya(i1,i2,i3)
      x1=xa(i1,i2,i3)
      x1y1=x1*y1
      r(i1,i2,i3,n)=(c(3,3,4,n)*864.+c(3,4,4,n)*y1*3456.+c(4,3,4,n)*x1*
     & 3456.+c(4,4,4,n)*x1y1*13824.)*time
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
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      r(i1,i2,i3,n)=(c(3,4,0,n)*144.+c(3,4,1,n)*z1*144.+c(3,4,2,n)*z2*
     & 144.+c(3,4,3,n)*z3*144.+c(3,4,4,n)*z4*144.+c(4,4,0,n)*x1*576.+
     & c(4,4,1,n)*x1z1*576.+c(4,4,2,n)*x1z2*576.+c(4,4,3,n)*x1z3*576.+
     & c(4,4,4,n)*x1z4*576.)*time
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
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      r(i1,i2,i3,n)=(c(3,4,1,n)*144.+c(3,4,2,n)*z1*288.+c(3,4,3,n)*z2*
     & 432.+c(3,4,4,n)*z3*576.+c(4,4,1,n)*x1*576.+c(4,4,2,n)*x1z1*
     & 1152.+c(4,4,3,n)*x1z2*1728.+c(4,4,4,n)*x1z3*2304.)*time
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
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      x1z2=x1z1*z1
      r(i1,i2,i3,n)=(c(3,4,2,n)*288.+c(3,4,3,n)*z1*864.+c(3,4,4,n)*z2*
     & 1728.+c(4,4,2,n)*x1*1152.+c(4,4,3,n)*x1z1*3456.+c(4,4,4,n)*
     & x1z2*6912.)*time
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
      x1=xa(i1,i2,i3)
      x1z1=x1*z1
      r(i1,i2,i3,n)=(c(3,4,3,n)*864.+c(3,4,4,n)*z1*3456.+c(4,4,3,n)*x1*
     & 3456.+c(4,4,4,n)*x1z1*13824.)*time
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
      x1=xa(i1,i2,i3)
      r(i1,i2,i3,n)=(c(3,4,4,n)*3456.+c(4,4,4,n)*x1*13824.)*time
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
      r(i1,i2,i3,n)=(c(4,0,0,n)*24.+c(4,0,1,n)*z1*24.+c(4,0,2,n)*z2*
     & 24.+c(4,0,3,n)*z3*24.+c(4,0,4,n)*z4*24.+c(4,1,0,n)*y1*24.+c(4,
     & 1,1,n)*y1z1*24.+c(4,1,2,n)*y1z2*24.+c(4,1,3,n)*y1z3*24.+c(4,1,
     & 4,n)*y1z4*24.+c(4,2,0,n)*y2*24.+c(4,2,1,n)*y2z1*24.+c(4,2,2,n)*
     & y2z2*24.+c(4,2,3,n)*y2z3*24.+c(4,2,4,n)*y2z4*24.+c(4,3,0,n)*y3*
     & 24.+c(4,3,1,n)*y3z1*24.+c(4,3,2,n)*y3z2*24.+c(4,3,3,n)*y3z3*
     & 24.+c(4,3,4,n)*y3z4*24.+c(4,4,0,n)*y4*24.+c(4,4,1,n)*y4z1*24.+
     & c(4,4,2,n)*y4z2*24.+c(4,4,3,n)*y4z3*24.+c(4,4,4,n)*y4z4*24.)*
     & time
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
      r(i1,i2,i3,n)=(c(4,0,1,n)*24.+c(4,0,2,n)*z1*48.+c(4,0,3,n)*z2*
     & 72.+c(4,0,4,n)*z3*96.+c(4,1,1,n)*y1*24.+c(4,1,2,n)*y1z1*48.+c(
     & 4,1,3,n)*y1z2*72.+c(4,1,4,n)*y1z3*96.+c(4,2,1,n)*y2*24.+c(4,2,
     & 2,n)*y2z1*48.+c(4,2,3,n)*y2z2*72.+c(4,2,4,n)*y2z3*96.+c(4,3,1,
     & n)*y3*24.+c(4,3,2,n)*y3z1*48.+c(4,3,3,n)*y3z2*72.+c(4,3,4,n)*
     & y3z3*96.+c(4,4,1,n)*y4*24.+c(4,4,2,n)*y4z1*48.+c(4,4,3,n)*y4z2*
     & 72.+c(4,4,4,n)*y4z3*96.)*time
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
      r(i1,i2,i3,n)=(c(4,0,2,n)*48.+c(4,0,3,n)*z1*144.+c(4,0,4,n)*z2*
     & 288.+c(4,1,2,n)*y1*48.+c(4,1,3,n)*y1z1*144.+c(4,1,4,n)*y1z2*
     & 288.+c(4,2,2,n)*y2*48.+c(4,2,3,n)*y2z1*144.+c(4,2,4,n)*y2z2*
     & 288.+c(4,3,2,n)*y3*48.+c(4,3,3,n)*y3z1*144.+c(4,3,4,n)*y3z2*
     & 288.+c(4,4,2,n)*y4*48.+c(4,4,3,n)*y4z1*144.+c(4,4,4,n)*y4z2*
     & 288.)*time
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
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y4=y3*y1
      y4z1=y3z1*y1
      r(i1,i2,i3,n)=(c(4,0,3,n)*144.+c(4,0,4,n)*z1*576.+c(4,1,3,n)*y1*
     & 144.+c(4,1,4,n)*y1z1*576.+c(4,2,3,n)*y2*144.+c(4,2,4,n)*y2z1*
     & 576.+c(4,3,3,n)*y3*144.+c(4,3,4,n)*y3z1*576.+c(4,4,3,n)*y4*
     & 144.+c(4,4,4,n)*y4z1*576.)*time
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
      y1=ya(i1,i2,i3)
      y2=y1*y1
      y3=y2*y1
      y4=y3*y1
      r(i1,i2,i3,n)=(c(4,0,4,n)*576.+c(4,1,4,n)*y1*576.+c(4,2,4,n)*y2*
     & 576.+c(4,3,4,n)*y3*576.+c(4,4,4,n)*y4*576.)*time
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
      r(i1,i2,i3,n)=(c(4,1,0,n)*24.+c(4,1,1,n)*z1*24.+c(4,1,2,n)*z2*
     & 24.+c(4,1,3,n)*z3*24.+c(4,1,4,n)*z4*24.+c(4,2,0,n)*y1*48.+c(4,
     & 2,1,n)*y1z1*48.+c(4,2,2,n)*y1z2*48.+c(4,2,3,n)*y1z3*48.+c(4,2,
     & 4,n)*y1z4*48.+c(4,3,0,n)*y2*72.+c(4,3,1,n)*y2z1*72.+c(4,3,2,n)*
     & y2z2*72.+c(4,3,3,n)*y2z3*72.+c(4,3,4,n)*y2z4*72.+c(4,4,0,n)*y3*
     & 96.+c(4,4,1,n)*y3z1*96.+c(4,4,2,n)*y3z2*96.+c(4,4,3,n)*y3z3*
     & 96.+c(4,4,4,n)*y3z4*96.)*time
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
      r(i1,i2,i3,n)=(c(4,1,1,n)*24.+c(4,1,2,n)*z1*48.+c(4,1,3,n)*z2*
     & 72.+c(4,1,4,n)*z3*96.+c(4,2,1,n)*y1*48.+c(4,2,2,n)*y1z1*96.+c(
     & 4,2,3,n)*y1z2*144.+c(4,2,4,n)*y1z3*192.+c(4,3,1,n)*y2*72.+c(4,
     & 3,2,n)*y2z1*144.+c(4,3,3,n)*y2z2*216.+c(4,3,4,n)*y2z3*288.+c(4,
     & 4,1,n)*y3*96.+c(4,4,2,n)*y3z1*192.+c(4,4,3,n)*y3z2*288.+c(4,4,
     & 4,n)*y3z3*384.)*time
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
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      r(i1,i2,i3,n)=(c(4,1,2,n)*48.+c(4,1,3,n)*z1*144.+c(4,1,4,n)*z2*
     & 288.+c(4,2,2,n)*y1*96.+c(4,2,3,n)*y1z1*288.+c(4,2,4,n)*y1z2*
     & 576.+c(4,3,2,n)*y2*144.+c(4,3,3,n)*y2z1*432.+c(4,3,4,n)*y2z2*
     & 864.+c(4,4,2,n)*y3*192.+c(4,4,3,n)*y3z1*576.+c(4,4,4,n)*y3z2*
     & 1152.)*time
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
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y3=y2*y1
      y3z1=y2z1*y1
      r(i1,i2,i3,n)=(c(4,1,3,n)*144.+c(4,1,4,n)*z1*576.+c(4,2,3,n)*y1*
     & 288.+c(4,2,4,n)*y1z1*1152.+c(4,3,3,n)*y2*432.+c(4,3,4,n)*y2z1*
     & 1728.+c(4,4,3,n)*y3*576.+c(4,4,4,n)*y3z1*2304.)*time
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
      y1=ya(i1,i2,i3)
      y2=y1*y1
      y3=y2*y1
      r(i1,i2,i3,n)=(c(4,1,4,n)*576.+c(4,2,4,n)*y1*1152.+c(4,3,4,n)*y2*
     & 1728.+c(4,4,4,n)*y3*2304.)*time
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
      r(i1,i2,i3,n)=(c(4,2,0,n)*48.+c(4,2,1,n)*z1*48.+c(4,2,2,n)*z2*
     & 48.+c(4,2,3,n)*z3*48.+c(4,2,4,n)*z4*48.+c(4,3,0,n)*y1*144.+c(4,
     & 3,1,n)*y1z1*144.+c(4,3,2,n)*y1z2*144.+c(4,3,3,n)*y1z3*144.+c(4,
     & 3,4,n)*y1z4*144.+c(4,4,0,n)*y2*288.+c(4,4,1,n)*y2z1*288.+c(4,4,
     & 2,n)*y2z2*288.+c(4,4,3,n)*y2z3*288.+c(4,4,4,n)*y2z4*288.)*time
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
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      r(i1,i2,i3,n)=(c(4,2,1,n)*48.+c(4,2,2,n)*z1*96.+c(4,2,3,n)*z2*
     & 144.+c(4,2,4,n)*z3*192.+c(4,3,1,n)*y1*144.+c(4,3,2,n)*y1z1*
     & 288.+c(4,3,3,n)*y1z2*432.+c(4,3,4,n)*y1z3*576.+c(4,4,1,n)*y2*
     & 288.+c(4,4,2,n)*y2z1*576.+c(4,4,3,n)*y2z2*864.+c(4,4,4,n)*y2z3*
     & 1152.)*time
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
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      r(i1,i2,i3,n)=(c(4,2,2,n)*96.+c(4,2,3,n)*z1*288.+c(4,2,4,n)*z2*
     & 576.+c(4,3,2,n)*y1*288.+c(4,3,3,n)*y1z1*864.+c(4,3,4,n)*y1z2*
     & 1728.+c(4,4,2,n)*y2*576.+c(4,4,3,n)*y2z1*1728.+c(4,4,4,n)*y2z2*
     & 3456.)*time
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
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      r(i1,i2,i3,n)=(c(4,2,3,n)*288.+c(4,2,4,n)*z1*1152.+c(4,3,3,n)*y1*
     & 864.+c(4,3,4,n)*y1z1*3456.+c(4,4,3,n)*y2*1728.+c(4,4,4,n)*y2z1*
     & 6912.)*time
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
      y1=ya(i1,i2,i3)
      y2=y1*y1
      r(i1,i2,i3,n)=(c(4,2,4,n)*1152.+c(4,3,4,n)*y1*3456.+c(4,4,4,n)*
     & y2*6912.)*time
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
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      r(i1,i2,i3,n)=(c(4,3,0,n)*144.+c(4,3,1,n)*z1*144.+c(4,3,2,n)*z2*
     & 144.+c(4,3,3,n)*z3*144.+c(4,3,4,n)*z4*144.+c(4,4,0,n)*y1*576.+
     & c(4,4,1,n)*y1z1*576.+c(4,4,2,n)*y1z2*576.+c(4,4,3,n)*y1z3*576.+
     & c(4,4,4,n)*y1z4*576.)*time
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
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      r(i1,i2,i3,n)=(c(4,3,1,n)*144.+c(4,3,2,n)*z1*288.+c(4,3,3,n)*z2*
     & 432.+c(4,3,4,n)*z3*576.+c(4,4,1,n)*y1*576.+c(4,4,2,n)*y1z1*
     & 1152.+c(4,4,3,n)*y1z2*1728.+c(4,4,4,n)*y1z3*2304.)*time
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
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      y1z2=y1z1*z1
      r(i1,i2,i3,n)=(c(4,3,2,n)*288.+c(4,3,3,n)*z1*864.+c(4,3,4,n)*z2*
     & 1728.+c(4,4,2,n)*y1*1152.+c(4,4,3,n)*y1z1*3456.+c(4,4,4,n)*
     & y1z2*6912.)*time
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
      y1=ya(i1,i2,i3)
      y1z1=y1*z1
      r(i1,i2,i3,n)=(c(4,3,3,n)*864.+c(4,3,4,n)*z1*3456.+c(4,4,3,n)*y1*
     & 3456.+c(4,4,4,n)*y1z1*13824.)*time
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
      y1=ya(i1,i2,i3)
      r(i1,i2,i3,n)=(c(4,3,4,n)*3456.+c(4,4,4,n)*y1*13824.)*time
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
      r(i1,i2,i3,n)=(c(4,4,0,n)*576.+c(4,4,1,n)*z1*576.+c(4,4,2,n)*z2*
     & 576.+c(4,4,3,n)*z3*576.+c(4,4,4,n)*z4*576.)*time
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
      r(i1,i2,i3,n)=(c(4,4,1,n)*576.+c(4,4,2,n)*z1*1152.+c(4,4,3,n)*z2*
     & 1728.+c(4,4,4,n)*z3*2304.)*time
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
      r(i1,i2,i3,n)=(c(4,4,2,n)*1152.+c(4,4,3,n)*z1*3456.+c(4,4,4,n)*
     & z2*6912.)*time
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
      r(i1,i2,i3,n)=(c(4,4,3,n)*3456.+c(4,4,4,n)*z1*13824.)*time
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
      r(i1,i2,i3,n)=(c(4,4,4,n)*13824.)*time
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
      z3=z2*z1
      z4=z3*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y2z3=x1y1z3*y1
      x1y2z4=x1y1z4*y1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x2y1z3=x1y1z3*x1
      x2y1z4=x1y1z4*x1
      x2y2z3=x1y2z3*x1
      x2y2z4=x1y2z4*x1
      x3z3=x2z3*x1
      x3z4=x2z4*x1
      x3y1z3=x2y1z3*x1
      x3y1z4=x2y1z4*x1
      x3y2z3=x2y2z3*x1
      x3y2z4=x2y2z4*x1
      x4z3=x3z3*x1
      x4z4=x3z4*x1
      x4y1z3=x3y1z3*x1
      x4y1z4=x3y1z4*x1
      x4y2z3=x3y2z3*x1
      x4y2z4=x3y2z4*x1
      y3z3=y2z3*y1
      y3z4=y2z4*y1
      y4z3=y3z3*y1
      y4z4=y3z4*y1
      x1y3z3=x1y2z3*y1
      x1y3z4=x1y2z4*y1
      x1y4z3=x1y3z3*y1
      x1y4z4=x1y3z4*y1
      x2y3z3=x1y3z3*x1
      x2y3z4=x1y3z4*x1
      x2y4z3=x1y4z3*x1
      x2y4z4=x1y4z4*x1
      r(i1,i2,i3,n)=(+c(0,0,2,n)*2.+c(0,0,3,n)*z1*6.+c(0,0,4,n)*z2*12.+
     & c(0,1,2,n)*y1*2.+c(0,1,3,n)*y1z1*6.+c(0,1,4,n)*y1z2*12.+c(0,2,
     & 2,n)*y2*2.+c(0,2,3,n)*y2z1*6.+c(0,2,4,n)*y2z2*12.+c(0,3,2,n)*
     & y3*2.+c(0,3,3,n)*y3z1*6.+c(0,3,4,n)*y3z2*12.+c(0,4,2,n)*y4*2.+
     & c(0,4,3,n)*y4z1*6.+c(0,4,4,n)*y4z2*12.+c(1,0,2,n)*x1*2.+c(1,0,
     & 3,n)*x1z1*6.+c(1,0,4,n)*x1z2*12.+c(1,1,2,n)*x1y1*2.+c(1,1,3,n)*
     & x1y1z1*6.+c(1,1,4,n)*x1y1z2*12.+c(1,2,2,n)*x1y2*2.+c(1,2,3,n)*
     & x1y2z1*6.+c(1,2,4,n)*x1y2z2*12.+c(1,3,2,n)*x1y3*2.+c(1,3,3,n)*
     & x1y3z1*6.+c(1,3,4,n)*x1y3z2*12.+c(1,4,2,n)*x1y4*2.+c(1,4,3,n)*
     & x1y4z1*6.+c(1,4,4,n)*x1y4z2*12.+c(2,0,2,n)*x2*2.+c(2,0,3,n)*
     & x2z1*6.+c(2,0,4,n)*x2z2*12.+c(2,1,2,n)*x2y1*2.+c(2,1,3,n)*
     & x2y1z1*6.+c(2,1,4,n)*x2y1z2*12.+c(2,2,2,n)*x2y2*2.+c(2,2,3,n)*
     & x2y2z1*6.+c(2,2,4,n)*x2y2z2*12.+c(2,3,2,n)*x2y3*2.+c(2,3,3,n)*
     & x2y3z1*6.+c(2,3,4,n)*x2y3z2*12.+c(2,4,2,n)*x2y4*2.+c(2,4,3,n)*
     & x2y4z1*6.+c(2,4,4,n)*x2y4z2*12.+c(3,0,2,n)*x3*2.+c(3,0,3,n)*
     & x3z1*6.+c(3,0,4,n)*x3z2*12.+c(3,1,2,n)*x3y1*2.+c(3,1,3,n)*
     & x3y1z1*6.+c(3,1,4,n)*x3y1z2*12.+c(3,2,2,n)*x3y2*2.+c(3,2,3,n)*
     & x3y2z1*6.+c(3,2,4,n)*x3y2z2*12.+c(3,3,2,n)*x3y3*2.+c(3,3,3,n)*
     & x3y3z1*6.+c(3,3,4,n)*x3y3z2*12.+c(3,4,2,n)*x3y4*2.+c(3,4,3,n)*
     & x3y4z1*6.+c(3,4,4,n)*x3y4z2*12.+c(4,0,2,n)*x4*2.+c(4,0,3,n)*
     & x4z1*6.+c(4,0,4,n)*x4z2*12.+c(4,1,2,n)*x4y1*2.+c(4,1,3,n)*
     & x4y1z1*6.+c(4,1,4,n)*x4y1z2*12.+c(4,2,2,n)*x4y2*2.+c(4,2,3,n)*
     & x4y2z1*6.+c(4,2,4,n)*x4y2z2*12.+c(4,3,2,n)*x4y3*2.+c(4,3,3,n)*
     & x4y3z1*6.+c(4,3,4,n)*x4y3z2*12.+c(4,4,2,n)*x4y4*2.+c(4,4,3,n)*
     & x4y4z1*6.+c(4,4,4,n)*x4y4z2*12.+c(0,2,0,n)*2.+c(0,2,1,n)*z1*2.+
     & c(0,2,2,n)*z2*2.+c(0,2,3,n)*z3*2.+c(0,2,4,n)*z4*2.+c(0,3,0,n)*
     & y1*6.+c(0,3,1,n)*y1z1*6.+c(0,3,2,n)*y1z2*6.+c(0,3,3,n)*y1z3*6.+
     & c(0,3,4,n)*y1z4*6.+c(0,4,0,n)*y2*12.+c(0,4,1,n)*y2z1*12.+c(0,4,
     & 2,n)*y2z2*12.+c(0,4,3,n)*y2z3*12.+c(0,4,4,n)*y2z4*12.+c(1,2,0,
     & n)*x1*2.+c(1,2,1,n)*x1z1*2.+c(1,2,2,n)*x1z2*2.+c(1,2,3,n)*x1z3*
     & 2.+c(1,2,4,n)*x1z4*2.+c(1,3,0,n)*x1y1*6.+c(1,3,1,n)*x1y1z1*6.+
     & c(1,3,2,n)*x1y1z2*6.+c(1,3,3,n)*x1y1z3*6.+c(1,3,4,n)*x1y1z4*6.+
     & c(1,4,0,n)*x1y2*12.+c(1,4,1,n)*x1y2z1*12.+c(1,4,2,n)*x1y2z2*
     & 12.+c(1,4,3,n)*x1y2z3*12.+c(1,4,4,n)*x1y2z4*12.+c(2,2,0,n)*x2*
     & 2.+c(2,2,1,n)*x2z1*2.+c(2,2,2,n)*x2z2*2.+c(2,2,3,n)*x2z3*2.+c(
     & 2,2,4,n)*x2z4*2.+c(2,3,0,n)*x2y1*6.+c(2,3,1,n)*x2y1z1*6.+c(2,3,
     & 2,n)*x2y1z2*6.+c(2,3,3,n)*x2y1z3*6.+c(2,3,4,n)*x2y1z4*6.+c(2,4,
     & 0,n)*x2y2*12.+c(2,4,1,n)*x2y2z1*12.+c(2,4,2,n)*x2y2z2*12.+c(2,
     & 4,3,n)*x2y2z3*12.+c(2,4,4,n)*x2y2z4*12.+c(3,2,0,n)*x3*2.+c(3,2,
     & 1,n)*x3z1*2.+c(3,2,2,n)*x3z2*2.+c(3,2,3,n)*x3z3*2.+c(3,2,4,n)*
     & x3z4*2.+c(3,3,0,n)*x3y1*6.+c(3,3,1,n)*x3y1z1*6.+c(3,3,2,n)*
     & x3y1z2*6.+c(3,3,3,n)*x3y1z3*6.+c(3,3,4,n)*x3y1z4*6.+c(3,4,0,n)*
     & x3y2*12.+c(3,4,1,n)*x3y2z1*12.+c(3,4,2,n)*x3y2z2*12.+c(3,4,3,n)
     & *x3y2z3*12.+c(3,4,4,n)*x3y2z4*12.+c(4,2,0,n)*x4*2.+c(4,2,1,n)*
     & x4z1*2.+c(4,2,2,n)*x4z2*2.+c(4,2,3,n)*x4z3*2.+c(4,2,4,n)*x4z4*
     & 2.+c(4,3,0,n)*x4y1*6.+c(4,3,1,n)*x4y1z1*6.+c(4,3,2,n)*x4y1z2*
     & 6.+c(4,3,3,n)*x4y1z3*6.+c(4,3,4,n)*x4y1z4*6.+c(4,4,0,n)*x4y2*
     & 12.+c(4,4,1,n)*x4y2z1*12.+c(4,4,2,n)*x4y2z2*12.+c(4,4,3,n)*
     & x4y2z3*12.+c(4,4,4,n)*x4y2z4*12.+c(2,0,0,n)*2.+c(2,0,1,n)*z1*
     & 2.+c(2,0,2,n)*z2*2.+c(2,0,3,n)*z3*2.+c(2,0,4,n)*z4*2.+c(2,1,0,
     & n)*y1*2.+c(2,1,1,n)*y1z1*2.+c(2,1,2,n)*y1z2*2.+c(2,1,3,n)*y1z3*
     & 2.+c(2,1,4,n)*y1z4*2.+c(2,2,0,n)*y2*2.+c(2,2,1,n)*y2z1*2.+c(2,
     & 2,2,n)*y2z2*2.+c(2,2,3,n)*y2z3*2.+c(2,2,4,n)*y2z4*2.+c(2,3,0,n)
     & *y3*2.+c(2,3,1,n)*y3z1*2.+c(2,3,2,n)*y3z2*2.+c(2,3,3,n)*y3z3*
     & 2.+c(2,3,4,n)*y3z4*2.+c(2,4,0,n)*y4*2.+c(2,4,1,n)*y4z1*2.+c(2,
     & 4,2,n)*y4z2*2.+c(2,4,3,n)*y4z3*2.+c(2,4,4,n)*y4z4*2.+c(3,0,0,n)
     & *x1*6.+c(3,0,1,n)*x1z1*6.+c(3,0,2,n)*x1z2*6.+c(3,0,3,n)*x1z3*
     & 6.+c(3,0,4,n)*x1z4*6.+c(3,1,0,n)*x1y1*6.+c(3,1,1,n)*x1y1z1*6.+
     & c(3,1,2,n)*x1y1z2*6.+c(3,1,3,n)*x1y1z3*6.+c(3,1,4,n)*x1y1z4*6.+
     & c(3,2,0,n)*x1y2*6.+c(3,2,1,n)*x1y2z1*6.+c(3,2,2,n)*x1y2z2*6.+c(
     & 3,2,3,n)*x1y2z3*6.+c(3,2,4,n)*x1y2z4*6.+c(3,3,0,n)*x1y3*6.+c(3,
     & 3,1,n)*x1y3z1*6.+c(3,3,2,n)*x1y3z2*6.+c(3,3,3,n)*x1y3z3*6.+c(3,
     & 3,4,n)*x1y3z4*6.+c(3,4,0,n)*x1y4*6.+c(3,4,1,n)*x1y4z1*6.+c(3,4,
     & 2,n)*x1y4z2*6.+c(3,4,3,n)*x1y4z3*6.+c(3,4,4,n)*x1y4z4*6.+c(4,0,
     & 0,n)*x2*12.+c(4,0,1,n)*x2z1*12.+c(4,0,2,n)*x2z2*12.+c(4,0,3,n)*
     & x2z3*12.+c(4,0,4,n)*x2z4*12.+c(4,1,0,n)*x2y1*12.+c(4,1,1,n)*
     & x2y1z1*12.+c(4,1,2,n)*x2y1z2*12.+c(4,1,3,n)*x2y1z3*12.+c(4,1,4,
     & n)*x2y1z4*12.+c(4,2,0,n)*x2y2*12.+c(4,2,1,n)*x2y2z1*12.+c(4,2,
     & 2,n)*x2y2z2*12.+c(4,2,3,n)*x2y2z3*12.+c(4,2,4,n)*x2y2z4*12.+c(
     & 4,3,0,n)*x2y3*12.+c(4,3,1,n)*x2y3z1*12.+c(4,3,2,n)*x2y3z2*12.+
     & c(4,3,3,n)*x2y3z3*12.+c(4,3,4,n)*x2y3z4*12.+c(4,4,0,n)*x2y4*
     & 12.+c(4,4,1,n)*x2y4z1*12.+c(4,4,2,n)*x2y4z2*12.+c(4,4,3,n)*
     & x2y4z3*12.+c(4,4,4,n)*x2y4z4*12.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else if( dx.gt.4 .or. dy.gt.4 .or. dz.gt.4 )then
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
