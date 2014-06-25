! This file automatically generated from polyFunction.bf with bpp.
! polyFun(2D0)
      subroutine polyFunction2D0 (nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb,
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
! poly2D0(x1=xa(i1,i2,i3),y1=ya(i1,i2,i3),z1=za(i1,i2,i3),r(i1,i2,i3,n))
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
      r(i1,i2,i3,n)=(c(0,0,0,n))*time
! endLoops()
      end do
      end do
      end do
      end do
      else
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
      end if
      return
      end
! polyFun(2D1)
      subroutine polyFunction2D1 (nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb,
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
! poly2D1(x1=xa(i1,i2,i3),y1=ya(i1,i2,i3),z1=za(i1,i2,i3),r(i1,i2,i3,n))
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
      y1=ya(i1,i2,i3)
      x1=xa(i1,i2,i3)
      x1y1=x1*y1
      r(i1,i2,i3,n)=(c(0,0,0,n)+c(0,1,0,n)*y1+c(1,0,0,n)*x1+c(1,1,0,n)*
     & x1y1)*time
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
      x1=xa(i1,i2,i3)
      r(i1,i2,i3,n)=(c(0,1,0,n)+c(1,1,0,n)*x1)*time
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
      y1=ya(i1,i2,i3)
      r(i1,i2,i3,n)=(c(1,0,0,n)+c(1,1,0,n)*y1)*time
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
      r(i1,i2,i3,n)=(c(1,1,0,n))*time
! endLoops()
      end do
      end do
      end do
      end do
      else
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
      end if
      return
      end
! polyFun(2D2)
      subroutine polyFunction2D2 (nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb,
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
! poly2D2(x1=xa(i1,i2,i3),y1=ya(i1,i2,i3),z1=za(i1,i2,i3),r(i1,i2,i3,n))
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
      y1=ya(i1,i2,i3)
      y2=y1*y1
      x1=xa(i1,i2,i3)
      x1y1=x1*y1
      x1y2=x1y1*y1
      x2=x1*x1
      x2y1=x1y1*x1
      x2y2=x1y2*x1
      r(i1,i2,i3,n)=(c(0,0,0,n)+c(0,1,0,n)*y1+c(0,2,0,n)*y2+c(1,0,0,n)*
     & x1+c(1,1,0,n)*x1y1+c(1,2,0,n)*x1y2+c(2,0,0,n)*x2+c(2,1,0,n)*
     & x2y1+c(2,2,0,n)*x2y2)*time
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
      y1=ya(i1,i2,i3)
      x1=xa(i1,i2,i3)
      x1y1=x1*y1
      x2=x1*x1
      x2y1=x1y1*x1
      r(i1,i2,i3,n)=(c(0,1,0,n)+c(0,2,0,n)*y1*2.+c(1,1,0,n)*x1+c(1,2,0,
     & n)*x1y1*2.+c(2,1,0,n)*x2+c(2,2,0,n)*x2y1*2.)*time
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
      x1=xa(i1,i2,i3)
      x2=x1*x1
      r(i1,i2,i3,n)=(c(0,2,0,n)*2.+c(1,2,0,n)*x1*2.+c(2,2,0,n)*x2*2.)*
     & time
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
      y1=ya(i1,i2,i3)
      y2=y1*y1
      x1=xa(i1,i2,i3)
      x1y1=x1*y1
      x1y2=x1y1*y1
      r(i1,i2,i3,n)=(c(1,0,0,n)+c(1,1,0,n)*y1+c(1,2,0,n)*y2+c(2,0,0,n)*
     & x1*2.+c(2,1,0,n)*x1y1*2.+c(2,2,0,n)*x1y2*2.)*time
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
      y1=ya(i1,i2,i3)
      x1=xa(i1,i2,i3)
      x1y1=x1*y1
      r(i1,i2,i3,n)=(c(1,1,0,n)+c(1,2,0,n)*y1*2.+c(2,1,0,n)*x1*2.+c(2,
     & 2,0,n)*x1y1*4.)*time
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
      x1=xa(i1,i2,i3)
      r(i1,i2,i3,n)=(c(1,2,0,n)*2.+c(2,2,0,n)*x1*4.)*time
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
      y1=ya(i1,i2,i3)
      y2=y1*y1
      r(i1,i2,i3,n)=(c(2,0,0,n)*2.+c(2,1,0,n)*y1*2.+c(2,2,0,n)*y2*2.)*
     & time
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
      y1=ya(i1,i2,i3)
      r(i1,i2,i3,n)=(c(2,1,0,n)*2.+c(2,2,0,n)*y1*4.)*time
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
      r(i1,i2,i3,n)=(c(2,2,0,n)*4.)*time
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
      x1=xa(i1,i2,i3)
      x2=x1*x1
      y1=ya(i1,i2,i3)
      y2=y1*y1
      r(i1,i2,i3,n)=(+c(0,2,0,n)*2.+c(1,2,0,n)*x1*2.+c(2,2,0,n)*x2*2.+
     & c(2,0,0,n)*2.+c(2,1,0,n)*y1*2.+c(2,2,0,n)*y2*2.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else
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
      end if
      return
      end
! polyFun(2D3)
      subroutine polyFunction2D3 (nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb,
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
! poly2D3(x1=xa(i1,i2,i3),y1=ya(i1,i2,i3),z1=za(i1,i2,i3),r(i1,i2,i3,n))
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
      r(i1,i2,i3,n)=(c(0,0,0,n)+c(0,1,0,n)*y1+c(0,2,0,n)*y2+c(0,3,0,n)*
     & y3+c(1,0,0,n)*x1+c(1,1,0,n)*x1y1+c(1,2,0,n)*x1y2+c(1,3,0,n)*
     & x1y3+c(2,0,0,n)*x2+c(2,1,0,n)*x2y1+c(2,2,0,n)*x2y2+c(2,3,0,n)*
     & x2y3+c(3,0,0,n)*x3+c(3,1,0,n)*x3y1+c(3,2,0,n)*x3y2+c(3,3,0,n)*
     & x3y3)*time
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
      r(i1,i2,i3,n)=(c(0,1,0,n)+c(0,2,0,n)*y1*2.+c(0,3,0,n)*y2*3.+c(1,
     & 1,0,n)*x1+c(1,2,0,n)*x1y1*2.+c(1,3,0,n)*x1y2*3.+c(2,1,0,n)*x2+
     & c(2,2,0,n)*x2y1*2.+c(2,3,0,n)*x2y2*3.+c(3,1,0,n)*x3+c(3,2,0,n)*
     & x3y1*2.+c(3,3,0,n)*x3y2*3.)*time
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
      y1=ya(i1,i2,i3)
      x1=xa(i1,i2,i3)
      x1y1=x1*y1
      x2=x1*x1
      x2y1=x1y1*x1
      x3=x2*x1
      x3y1=x2y1*x1
      r(i1,i2,i3,n)=(c(0,2,0,n)*2.+c(0,3,0,n)*y1*6.+c(1,2,0,n)*x1*2.+c(
     & 1,3,0,n)*x1y1*6.+c(2,2,0,n)*x2*2.+c(2,3,0,n)*x2y1*6.+c(3,2,0,n)
     & *x3*2.+c(3,3,0,n)*x3y1*6.)*time
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
      r(i1,i2,i3,n)=(c(1,0,0,n)+c(1,1,0,n)*y1+c(1,2,0,n)*y2+c(1,3,0,n)*
     & y3+c(2,0,0,n)*x1*2.+c(2,1,0,n)*x1y1*2.+c(2,2,0,n)*x1y2*2.+c(2,
     & 3,0,n)*x1y3*2.+c(3,0,0,n)*x2*3.+c(3,1,0,n)*x2y1*3.+c(3,2,0,n)*
     & x2y2*3.+c(3,3,0,n)*x2y3*3.)*time
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
      y1=ya(i1,i2,i3)
      y2=y1*y1
      x1=xa(i1,i2,i3)
      x1y1=x1*y1
      x1y2=x1y1*y1
      x2=x1*x1
      x2y1=x1y1*x1
      x2y2=x1y2*x1
      r(i1,i2,i3,n)=(c(1,1,0,n)+c(1,2,0,n)*y1*2.+c(1,3,0,n)*y2*3.+c(2,
     & 1,0,n)*x1*2.+c(2,2,0,n)*x1y1*4.+c(2,3,0,n)*x1y2*6.+c(3,1,0,n)*
     & x2*3.+c(3,2,0,n)*x2y1*6.+c(3,3,0,n)*x2y2*9.)*time
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
      y1=ya(i1,i2,i3)
      x1=xa(i1,i2,i3)
      x1y1=x1*y1
      x2=x1*x1
      x2y1=x1y1*x1
      r(i1,i2,i3,n)=(c(1,2,0,n)*2.+c(1,3,0,n)*y1*6.+c(2,2,0,n)*x1*4.+c(
     & 2,3,0,n)*x1y1*12.+c(3,2,0,n)*x2*6.+c(3,3,0,n)*x2y1*18.)*time
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
      y1=ya(i1,i2,i3)
      y2=y1*y1
      y3=y2*y1
      x1=xa(i1,i2,i3)
      x1y1=x1*y1
      x1y2=x1y1*y1
      x1y3=x1y2*y1
      r(i1,i2,i3,n)=(c(2,0,0,n)*2.+c(2,1,0,n)*y1*2.+c(2,2,0,n)*y2*2.+c(
     & 2,3,0,n)*y3*2.+c(3,0,0,n)*x1*6.+c(3,1,0,n)*x1y1*6.+c(3,2,0,n)*
     & x1y2*6.+c(3,3,0,n)*x1y3*6.)*time
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
      y1=ya(i1,i2,i3)
      y2=y1*y1
      x1=xa(i1,i2,i3)
      x1y1=x1*y1
      x1y2=x1y1*y1
      r(i1,i2,i3,n)=(c(2,1,0,n)*2.+c(2,2,0,n)*y1*4.+c(2,3,0,n)*y2*6.+c(
     & 3,1,0,n)*x1*6.+c(3,2,0,n)*x1y1*12.+c(3,3,0,n)*x1y2*18.)*time
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
      y1=ya(i1,i2,i3)
      x1=xa(i1,i2,i3)
      x1y1=x1*y1
      r(i1,i2,i3,n)=(c(2,2,0,n)*4.+c(2,3,0,n)*y1*12.+c(3,2,0,n)*x1*12.+
     & c(3,3,0,n)*x1y1*36.)*time
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
      y1=ya(i1,i2,i3)
      y2=y1*y1
      y3=y2*y1
      r(i1,i2,i3,n)=(c(3,0,0,n)*6.+c(3,1,0,n)*y1*6.+c(3,2,0,n)*y2*6.+c(
     & 3,3,0,n)*y3*6.)*time
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
      y1=ya(i1,i2,i3)
      x1=xa(i1,i2,i3)
      x1y1=x1*y1
      x2=x1*x1
      x2y1=x1y1*x1
      x3=x2*x1
      x3y1=x2y1*x1
      y2=y1*y1
      y3=y2*y1
      x1y2=x1y1*y1
      x1y3=x1y2*y1
      r(i1,i2,i3,n)=(+c(0,2,0,n)*2.+c(0,3,0,n)*y1*6.+c(1,2,0,n)*x1*2.+
     & c(1,3,0,n)*x1y1*6.+c(2,2,0,n)*x2*2.+c(2,3,0,n)*x2y1*6.+c(3,2,0,
     & n)*x3*2.+c(3,3,0,n)*x3y1*6.+c(2,0,0,n)*2.+c(2,1,0,n)*y1*2.+c(2,
     & 2,0,n)*y2*2.+c(2,3,0,n)*y3*2.+c(3,0,0,n)*x1*6.+c(3,1,0,n)*x1y1*
     & 6.+c(3,2,0,n)*x1y2*6.+c(3,3,0,n)*x1y3*6.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else
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
      end if
      return
      end
! polyFun(2D4)
      subroutine polyFunction2D4 (nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb,
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
! poly2D4(x1=xa(i1,i2,i3),y1=ya(i1,i2,i3),z1=za(i1,i2,i3),r(i1,i2,i3,n))
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
      r(i1,i2,i3,n)=(c(0,0,0,n)+c(0,1,0,n)*y1+c(0,2,0,n)*y2+c(0,3,0,n)*
     & y3+c(0,4,0,n)*y4+c(1,0,0,n)*x1+c(1,1,0,n)*x1y1+c(1,2,0,n)*x1y2+
     & c(1,3,0,n)*x1y3+c(1,4,0,n)*x1y4+c(2,0,0,n)*x2+c(2,1,0,n)*x2y1+
     & c(2,2,0,n)*x2y2+c(2,3,0,n)*x2y3+c(2,4,0,n)*x2y4+c(3,0,0,n)*x3+
     & c(3,1,0,n)*x3y1+c(3,2,0,n)*x3y2+c(3,3,0,n)*x3y3+c(3,4,0,n)*
     & x3y4+c(4,0,0,n)*x4+c(4,1,0,n)*x4y1+c(4,2,0,n)*x4y2+c(4,3,0,n)*
     & x4y3+c(4,4,0,n)*x4y4)*time
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
      r(i1,i2,i3,n)=(c(0,1,0,n)+c(0,2,0,n)*y1*2.+c(0,3,0,n)*y2*3.+c(0,
     & 4,0,n)*y3*4.+c(1,1,0,n)*x1+c(1,2,0,n)*x1y1*2.+c(1,3,0,n)*x1y2*
     & 3.+c(1,4,0,n)*x1y3*4.+c(2,1,0,n)*x2+c(2,2,0,n)*x2y1*2.+c(2,3,0,
     & n)*x2y2*3.+c(2,4,0,n)*x2y3*4.+c(3,1,0,n)*x3+c(3,2,0,n)*x3y1*2.+
     & c(3,3,0,n)*x3y2*3.+c(3,4,0,n)*x3y3*4.+c(4,1,0,n)*x4+c(4,2,0,n)*
     & x4y1*2.+c(4,3,0,n)*x4y2*3.+c(4,4,0,n)*x4y3*4.)*time
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
      r(i1,i2,i3,n)=(c(0,2,0,n)*2.+c(0,3,0,n)*y1*6.+c(0,4,0,n)*y2*12.+
     & c(1,2,0,n)*x1*2.+c(1,3,0,n)*x1y1*6.+c(1,4,0,n)*x1y2*12.+c(2,2,
     & 0,n)*x2*2.+c(2,3,0,n)*x2y1*6.+c(2,4,0,n)*x2y2*12.+c(3,2,0,n)*
     & x3*2.+c(3,3,0,n)*x3y1*6.+c(3,4,0,n)*x3y2*12.+c(4,2,0,n)*x4*2.+
     & c(4,3,0,n)*x4y1*6.+c(4,4,0,n)*x4y2*12.)*time
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
      r(i1,i2,i3,n)=(c(1,0,0,n)+c(1,1,0,n)*y1+c(1,2,0,n)*y2+c(1,3,0,n)*
     & y3+c(1,4,0,n)*y4+c(2,0,0,n)*x1*2.+c(2,1,0,n)*x1y1*2.+c(2,2,0,n)
     & *x1y2*2.+c(2,3,0,n)*x1y3*2.+c(2,4,0,n)*x1y4*2.+c(3,0,0,n)*x2*
     & 3.+c(3,1,0,n)*x2y1*3.+c(3,2,0,n)*x2y2*3.+c(3,3,0,n)*x2y3*3.+c(
     & 3,4,0,n)*x2y4*3.+c(4,0,0,n)*x3*4.+c(4,1,0,n)*x3y1*4.+c(4,2,0,n)
     & *x3y2*4.+c(4,3,0,n)*x3y3*4.+c(4,4,0,n)*x3y4*4.)*time
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
      r(i1,i2,i3,n)=(c(1,1,0,n)+c(1,2,0,n)*y1*2.+c(1,3,0,n)*y2*3.+c(1,
     & 4,0,n)*y3*4.+c(2,1,0,n)*x1*2.+c(2,2,0,n)*x1y1*4.+c(2,3,0,n)*
     & x1y2*6.+c(2,4,0,n)*x1y3*8.+c(3,1,0,n)*x2*3.+c(3,2,0,n)*x2y1*6.+
     & c(3,3,0,n)*x2y2*9.+c(3,4,0,n)*x2y3*12.+c(4,1,0,n)*x3*4.+c(4,2,
     & 0,n)*x3y1*8.+c(4,3,0,n)*x3y2*12.+c(4,4,0,n)*x3y3*16.)*time
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
      r(i1,i2,i3,n)=(c(1,2,0,n)*2.+c(1,3,0,n)*y1*6.+c(1,4,0,n)*y2*12.+
     & c(2,2,0,n)*x1*4.+c(2,3,0,n)*x1y1*12.+c(2,4,0,n)*x1y2*24.+c(3,2,
     & 0,n)*x2*6.+c(3,3,0,n)*x2y1*18.+c(3,4,0,n)*x2y2*36.+c(4,2,0,n)*
     & x3*8.+c(4,3,0,n)*x3y1*24.+c(4,4,0,n)*x3y2*48.)*time
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
      r(i1,i2,i3,n)=(c(2,0,0,n)*2.+c(2,1,0,n)*y1*2.+c(2,2,0,n)*y2*2.+c(
     & 2,3,0,n)*y3*2.+c(2,4,0,n)*y4*2.+c(3,0,0,n)*x1*6.+c(3,1,0,n)*
     & x1y1*6.+c(3,2,0,n)*x1y2*6.+c(3,3,0,n)*x1y3*6.+c(3,4,0,n)*x1y4*
     & 6.+c(4,0,0,n)*x2*12.+c(4,1,0,n)*x2y1*12.+c(4,2,0,n)*x2y2*12.+c(
     & 4,3,0,n)*x2y3*12.+c(4,4,0,n)*x2y4*12.)*time
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
      r(i1,i2,i3,n)=(c(2,1,0,n)*2.+c(2,2,0,n)*y1*4.+c(2,3,0,n)*y2*6.+c(
     & 2,4,0,n)*y3*8.+c(3,1,0,n)*x1*6.+c(3,2,0,n)*x1y1*12.+c(3,3,0,n)*
     & x1y2*18.+c(3,4,0,n)*x1y3*24.+c(4,1,0,n)*x2*12.+c(4,2,0,n)*x2y1*
     & 24.+c(4,3,0,n)*x2y2*36.+c(4,4,0,n)*x2y3*48.)*time
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
      y1=ya(i1,i2,i3)
      y2=y1*y1
      x1=xa(i1,i2,i3)
      x1y1=x1*y1
      x1y2=x1y1*y1
      x2=x1*x1
      x2y1=x1y1*x1
      x2y2=x1y2*x1
      r(i1,i2,i3,n)=(c(2,2,0,n)*4.+c(2,3,0,n)*y1*12.+c(2,4,0,n)*y2*24.+
     & c(3,2,0,n)*x1*12.+c(3,3,0,n)*x1y1*36.+c(3,4,0,n)*x1y2*72.+c(4,
     & 2,0,n)*x2*24.+c(4,3,0,n)*x2y1*72.+c(4,4,0,n)*x2y2*144.)*time
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
      y1=ya(i1,i2,i3)
      y2=y1*y1
      y3=y2*y1
      y4=y3*y1
      x1=xa(i1,i2,i3)
      x1y1=x1*y1
      x1y2=x1y1*y1
      x1y3=x1y2*y1
      x1y4=x1y3*y1
      r(i1,i2,i3,n)=(c(3,0,0,n)*6.+c(3,1,0,n)*y1*6.+c(3,2,0,n)*y2*6.+c(
     & 3,3,0,n)*y3*6.+c(3,4,0,n)*y4*6.+c(4,0,0,n)*x1*24.+c(4,1,0,n)*
     & x1y1*24.+c(4,2,0,n)*x1y2*24.+c(4,3,0,n)*x1y3*24.+c(4,4,0,n)*
     & x1y4*24.)*time
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
      y1=ya(i1,i2,i3)
      y2=y1*y1
      y3=y2*y1
      y4=y3*y1
      r(i1,i2,i3,n)=(c(4,0,0,n)*24.+c(4,1,0,n)*y1*24.+c(4,2,0,n)*y2*
     & 24.+c(4,3,0,n)*y3*24.+c(4,4,0,n)*y4*24.)*time
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
      y3=y2*y1
      y4=y3*y1
      x1y3=x1y2*y1
      x1y4=x1y3*y1
      x2y3=x1y3*x1
      x2y4=x1y4*x1
      r(i1,i2,i3,n)=(+c(0,2,0,n)*2.+c(0,3,0,n)*y1*6.+c(0,4,0,n)*y2*12.+
     & c(1,2,0,n)*x1*2.+c(1,3,0,n)*x1y1*6.+c(1,4,0,n)*x1y2*12.+c(2,2,
     & 0,n)*x2*2.+c(2,3,0,n)*x2y1*6.+c(2,4,0,n)*x2y2*12.+c(3,2,0,n)*
     & x3*2.+c(3,3,0,n)*x3y1*6.+c(3,4,0,n)*x3y2*12.+c(4,2,0,n)*x4*2.+
     & c(4,3,0,n)*x4y1*6.+c(4,4,0,n)*x4y2*12.+c(2,0,0,n)*2.+c(2,1,0,n)
     & *y1*2.+c(2,2,0,n)*y2*2.+c(2,3,0,n)*y3*2.+c(2,4,0,n)*y4*2.+c(3,
     & 0,0,n)*x1*6.+c(3,1,0,n)*x1y1*6.+c(3,2,0,n)*x1y2*6.+c(3,3,0,n)*
     & x1y3*6.+c(3,4,0,n)*x1y4*6.+c(4,0,0,n)*x2*12.+c(4,1,0,n)*x2y1*
     & 12.+c(4,2,0,n)*x2y2*12.+c(4,3,0,n)*x2y3*12.+c(4,4,0,n)*x2y4*
     & 12.)*time
! endLoops()
      end do
      end do
      end do
      end do
      else
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
      end if
      return
      end
