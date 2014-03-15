! This file automatically generated from polyFunction.bf with bpp.
! polyFun(1D0)
      subroutine polyEvaluate1D0 (r, x1,y1,z1,n,t, ndc1,ndc2,ndc3,nda, 
     & degree, degreeTime, a, c, dx,dy,dz,dt)
      !==========================================================================
      !==========================================================================
      integer ndc1,ndc2,ndc3,nda
      integer n,dx,dy,dz,dt,degree,degreeTime
      real x1,y1,z1,r
      real c(0:ndc1-1,0:ndc2-1,0:ndc3-1,0:*)
      real a(0:nda-1,0:*)
      real t
      integer laplace
      real time
      laplace=0
      if( dx.eq.-2 )then
      laplace=1
      end if
! #If "1D0" eq "3D5" || "1D0" eq "3D6"
! poly1D0(c,c,c,r)
      if( dx.eq.0.and.dy.eq.0.and.dz.eq.0.and.dt.eq.0 )then
! beginLoops($defineTime())
! defineTime()
      if( degreeTime.eq.0 )then
      time=a(0,n)
      else if( degreeTime.eq.1 )then
      time=a(0,n)+t*(a(1,n))
      else if( degreeTime.eq.2 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
      else if( degreeTime.eq.3 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
      else if( degreeTime.eq.4 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
      else if( degreeTime.eq.5 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))))
     & )
      else if( degreeTime.eq.6 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+t*(
     & a(6,n)))))))
      else
      write(*,*) 'ERROR invalid degreeTime'
      stop
      end if
      r=(c(0,0,0,n))*time
! endLoops()
      else if( dx.eq.0.and.dy.eq.0.and.dz.eq.0.and.dt.ge.1 )then
! beginLoops($defineTimeDerivative())
! defineTimeDerivative()
      if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
      r=(c(0,0,0,n))*time
! endLoops()
      else
! beginLoops(time=0.)
      time=0.
      r=0.
! endLoops()
      end if
! #If "1D0" eq "3D5" || "1D0" eq "3D6"
      return
      end
! polyFun(1D1)
      subroutine polyEvaluate1D1 (r, x1,y1,z1,n,t, ndc1,ndc2,ndc3,nda, 
     & degree, degreeTime, a, c, dx,dy,dz,dt)
      !==========================================================================
      !==========================================================================
      integer ndc1,ndc2,ndc3,nda
      integer n,dx,dy,dz,dt,degree,degreeTime
      real x1,y1,z1,r
      real c(0:ndc1-1,0:ndc2-1,0:ndc3-1,0:*)
      real a(0:nda-1,0:*)
      real t
      integer laplace
      real time
      laplace=0
      if( dx.eq.-2 )then
      laplace=1
      end if
! #If "1D1" eq "3D5" || "1D1" eq "3D6"
! poly1D1(c,c,c,r)
      if( dx.eq.0.and.dy.eq.0.and.dz.eq.0.and.dt.eq.0 )then
! beginLoops($defineTime())
! defineTime()
      if( degreeTime.eq.0 )then
      time=a(0,n)
      else if( degreeTime.eq.1 )then
      time=a(0,n)+t*(a(1,n))
      else if( degreeTime.eq.2 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
      else if( degreeTime.eq.3 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
      else if( degreeTime.eq.4 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
      else if( degreeTime.eq.5 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))))
     & )
      else if( degreeTime.eq.6 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+t*(
     & a(6,n)))))))
      else
      write(*,*) 'ERROR invalid degreeTime'
      stop
      end if
c 
      r=(c(0,0,0,n)+c(1,0,0,n)*x1)*time
! endLoops()
      else if( dx.eq.0.and.dy.eq.0.and.dz.eq.0.and.dt.ge.1 )then
! beginLoops($defineTimeDerivative())
! defineTimeDerivative()
      if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
c
      r=(c(0,0,0,n)+c(1,0,0,n)*x1)*time
! endLoops()
      else if( dx.eq.1.and.dy.eq.0.and.dz.eq.0.and.dt.eq.0  )then
! beginLoops($defineTime())
! defineTime()
      if( degreeTime.eq.0 )then
      time=a(0,n)
      else if( degreeTime.eq.1 )then
      time=a(0,n)+t*(a(1,n))
      else if( degreeTime.eq.2 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
      else if( degreeTime.eq.3 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
      else if( degreeTime.eq.4 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
      else if( degreeTime.eq.5 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))))
     & )
      else if( degreeTime.eq.6 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+t*(
     & a(6,n)))))))
      else
      write(*,*) 'ERROR invalid degreeTime'
      stop
      end if
      r=(c(1,0,0,n))*time
! endLoops()
      else
! beginLoops(time=0.)
      time=0.
      r=0.
! endLoops()
      end if
! #If "1D1" eq "3D5" || "1D1" eq "3D6"
      return
      end
! polyFun(1D2)
      subroutine polyEvaluate1D2 (r, x1,y1,z1,n,t, ndc1,ndc2,ndc3,nda, 
     & degree, degreeTime, a, c, dx,dy,dz,dt)
      !==========================================================================
      !==========================================================================
      integer ndc1,ndc2,ndc3,nda
      integer n,dx,dy,dz,dt,degree,degreeTime
      real x1,y1,z1,r
      real c(0:ndc1-1,0:ndc2-1,0:ndc3-1,0:*)
      real a(0:nda-1,0:*)
      real t
      integer laplace
      real time
      laplace=0
      if( dx.eq.-2 )then
      laplace=1
      end if
! #If "1D2" eq "3D5" || "1D2" eq "3D6"
! poly1D2(c,c,c,r)
      if( dx.eq.0.and.dy.eq.0.and.dz.eq.0.and.dt.eq.0 )then
! beginLoops($defineTime())
! defineTime()
      if( degreeTime.eq.0 )then
      time=a(0,n)
      else if( degreeTime.eq.1 )then
      time=a(0,n)+t*(a(1,n))
      else if( degreeTime.eq.2 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
      else if( degreeTime.eq.3 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
      else if( degreeTime.eq.4 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
      else if( degreeTime.eq.5 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))))
     & )
      else if( degreeTime.eq.6 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+t*(
     & a(6,n)))))))
      else
      write(*,*) 'ERROR invalid degreeTime'
      stop
      end if
c 
      x2=x1*x1
      r=(c(0,0,0,n)+c(1,0,0,n)*x1+c(2,0,0,n)*x2)*time
! endLoops()
      else if( dx.eq.0.and.dy.eq.0.and.dz.eq.0.and.dt.ge.1 )then
! beginLoops($defineTimeDerivative())
! defineTimeDerivative()
      if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
c
      x2=x1*x1
      r=(c(0,0,0,n)+c(1,0,0,n)*x1+c(2,0,0,n)*x2)*time
! endLoops()
      else if( dx.eq.1.and.dy.eq.0.and.dz.eq.0.and.dt.eq.0  )then
! beginLoops($defineTime())
! defineTime()
      if( degreeTime.eq.0 )then
      time=a(0,n)
      else if( degreeTime.eq.1 )then
      time=a(0,n)+t*(a(1,n))
      else if( degreeTime.eq.2 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
      else if( degreeTime.eq.3 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
      else if( degreeTime.eq.4 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
      else if( degreeTime.eq.5 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))))
     & )
      else if( degreeTime.eq.6 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+t*(
     & a(6,n)))))))
      else
      write(*,*) 'ERROR invalid degreeTime'
      stop
      end if
c 
      r=(c(1,0,0,n)+c(2,0,0,n)*x1*2.)*time
! endLoops()
      else if( dx.eq.2.and.dy.eq.0.and.dz.eq.0.and.dt.eq.0  )then
! beginLoops($defineTime())
! defineTime()
      if( degreeTime.eq.0 )then
      time=a(0,n)
      else if( degreeTime.eq.1 )then
      time=a(0,n)+t*(a(1,n))
      else if( degreeTime.eq.2 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
      else if( degreeTime.eq.3 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
      else if( degreeTime.eq.4 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
      else if( degreeTime.eq.5 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))))
     & )
      else if( degreeTime.eq.6 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+t*(
     & a(6,n)))))))
      else
      write(*,*) 'ERROR invalid degreeTime'
      stop
      end if
      r=(c(2,0,0,n)*2.)*time
! endLoops()
      else if( laplace.eq.1 )then
! beginLoops($defineTime())
! defineTime()
      if( degreeTime.eq.0 )then
      time=a(0,n)
      else if( degreeTime.eq.1 )then
      time=a(0,n)+t*(a(1,n))
      else if( degreeTime.eq.2 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
      else if( degreeTime.eq.3 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
      else if( degreeTime.eq.4 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
      else if( degreeTime.eq.5 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))))
     & )
      else if( degreeTime.eq.6 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+t*(
     & a(6,n)))))))
      else
      write(*,*) 'ERROR invalid degreeTime'
      stop
      end if
      r=(+c(2,0,0,n)*2.)*time
! endLoops()
      else
! beginLoops(time=0.)
      time=0.
      r=0.
! endLoops()
      end if
! #If "1D2" eq "3D5" || "1D2" eq "3D6"
      return
      end
! polyFun(1D3)
      subroutine polyEvaluate1D3 (r, x1,y1,z1,n,t, ndc1,ndc2,ndc3,nda, 
     & degree, degreeTime, a, c, dx,dy,dz,dt)
      !==========================================================================
      !==========================================================================
      integer ndc1,ndc2,ndc3,nda
      integer n,dx,dy,dz,dt,degree,degreeTime
      real x1,y1,z1,r
      real c(0:ndc1-1,0:ndc2-1,0:ndc3-1,0:*)
      real a(0:nda-1,0:*)
      real t
      integer laplace
      real time
      laplace=0
      if( dx.eq.-2 )then
      laplace=1
      end if
! #If "1D3" eq "3D5" || "1D3" eq "3D6"
! poly1D3(c,c,c,r)
      if( dx.eq.0.and.dy.eq.0.and.dz.eq.0.and.dt.eq.0 )then
! beginLoops($defineTime())
! defineTime()
      if( degreeTime.eq.0 )then
      time=a(0,n)
      else if( degreeTime.eq.1 )then
      time=a(0,n)+t*(a(1,n))
      else if( degreeTime.eq.2 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
      else if( degreeTime.eq.3 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
      else if( degreeTime.eq.4 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
      else if( degreeTime.eq.5 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))))
     & )
      else if( degreeTime.eq.6 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+t*(
     & a(6,n)))))))
      else
      write(*,*) 'ERROR invalid degreeTime'
      stop
      end if
c 
      x2=x1*x1
      x3=x2*x1
      r=(c(0,0,0,n)+c(1,0,0,n)*x1+c(2,0,0,n)*x2+c(3,0,0,n)*x3)*time
! endLoops()
      else if( dx.eq.0.and.dy.eq.0.and.dz.eq.0.and.dt.ge.1 )then
! beginLoops($defineTimeDerivative())
! defineTimeDerivative()
      if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
c
      x2=x1*x1
      x3=x2*x1
      r=(c(0,0,0,n)+c(1,0,0,n)*x1+c(2,0,0,n)*x2+c(3,0,0,n)*x3)*time
! endLoops()
      else if( dx.eq.1.and.dy.eq.0.and.dz.eq.0.and.dt.eq.0  )then
! beginLoops($defineTime())
! defineTime()
      if( degreeTime.eq.0 )then
      time=a(0,n)
      else if( degreeTime.eq.1 )then
      time=a(0,n)+t*(a(1,n))
      else if( degreeTime.eq.2 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
      else if( degreeTime.eq.3 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
      else if( degreeTime.eq.4 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
      else if( degreeTime.eq.5 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))))
     & )
      else if( degreeTime.eq.6 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+t*(
     & a(6,n)))))))
      else
      write(*,*) 'ERROR invalid degreeTime'
      stop
      end if
c 
      x2=x1*x1
      r=(c(1,0,0,n)+c(2,0,0,n)*x1*2.+c(3,0,0,n)*x2*3.)*time
! endLoops()
      else if( dx.eq.2.and.dy.eq.0.and.dz.eq.0.and.dt.eq.0  )then
! beginLoops($defineTime())
! defineTime()
      if( degreeTime.eq.0 )then
      time=a(0,n)
      else if( degreeTime.eq.1 )then
      time=a(0,n)+t*(a(1,n))
      else if( degreeTime.eq.2 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
      else if( degreeTime.eq.3 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
      else if( degreeTime.eq.4 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
      else if( degreeTime.eq.5 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))))
     & )
      else if( degreeTime.eq.6 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+t*(
     & a(6,n)))))))
      else
      write(*,*) 'ERROR invalid degreeTime'
      stop
      end if
c 
      r=(c(2,0,0,n)*2.+c(3,0,0,n)*x1*6.)*time
! endLoops()
      else if( dx.eq.3.and.dy.eq.0.and.dz.eq.0.and.dt.eq.0  )then
! beginLoops($defineTime())
! defineTime()
      if( degreeTime.eq.0 )then
      time=a(0,n)
      else if( degreeTime.eq.1 )then
      time=a(0,n)+t*(a(1,n))
      else if( degreeTime.eq.2 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
      else if( degreeTime.eq.3 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
      else if( degreeTime.eq.4 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
      else if( degreeTime.eq.5 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))))
     & )
      else if( degreeTime.eq.6 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+t*(
     & a(6,n)))))))
      else
      write(*,*) 'ERROR invalid degreeTime'
      stop
      end if
      r=(c(3,0,0,n)*6.)*time
! endLoops()
      else if( laplace.eq.1 )then
! beginLoops($defineTime())
! defineTime()
      if( degreeTime.eq.0 )then
      time=a(0,n)
      else if( degreeTime.eq.1 )then
      time=a(0,n)+t*(a(1,n))
      else if( degreeTime.eq.2 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
      else if( degreeTime.eq.3 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
      else if( degreeTime.eq.4 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
      else if( degreeTime.eq.5 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))))
     & )
      else if( degreeTime.eq.6 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+t*(
     & a(6,n)))))))
      else
      write(*,*) 'ERROR invalid degreeTime'
      stop
      end if
c
      r=(+c(2,0,0,n)*2.+c(3,0,0,n)*x1*6.)*time
! endLoops()
      else
! beginLoops(time=0.)
      time=0.
      r=0.
! endLoops()
      end if
! #If "1D3" eq "3D5" || "1D3" eq "3D6"
      return
      end
! polyFun(1D4)
      subroutine polyEvaluate1D4 (r, x1,y1,z1,n,t, ndc1,ndc2,ndc3,nda, 
     & degree, degreeTime, a, c, dx,dy,dz,dt)
      !==========================================================================
      !==========================================================================
      integer ndc1,ndc2,ndc3,nda
      integer n,dx,dy,dz,dt,degree,degreeTime
      real x1,y1,z1,r
      real c(0:ndc1-1,0:ndc2-1,0:ndc3-1,0:*)
      real a(0:nda-1,0:*)
      real t
      integer laplace
      real time
      laplace=0
      if( dx.eq.-2 )then
      laplace=1
      end if
! #If "1D4" eq "3D5" || "1D4" eq "3D6"
! poly1D4(c,c,c,r)
      if( dx.eq.0.and.dy.eq.0.and.dz.eq.0.and.dt.eq.0 )then
! beginLoops($defineTime())
! defineTime()
      if( degreeTime.eq.0 )then
      time=a(0,n)
      else if( degreeTime.eq.1 )then
      time=a(0,n)+t*(a(1,n))
      else if( degreeTime.eq.2 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
      else if( degreeTime.eq.3 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
      else if( degreeTime.eq.4 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
      else if( degreeTime.eq.5 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))))
     & )
      else if( degreeTime.eq.6 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+t*(
     & a(6,n)))))))
      else
      write(*,*) 'ERROR invalid degreeTime'
      stop
      end if
c 
      x2=x1*x1
      x3=x2*x1
      x4=x3*x1
      r=(c(0,0,0,n)+c(1,0,0,n)*x1+c(2,0,0,n)*x2+c(3,0,0,n)*x3+c(4,0,0,
     & n)*x4)*time
! endLoops()
      else if( dx.eq.0.and.dy.eq.0.and.dz.eq.0.and.dt.ge.1 )then
! beginLoops($defineTimeDerivative())
! defineTimeDerivative()
      if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
c
      x2=x1*x1
      x3=x2*x1
      x4=x3*x1
      r=(c(0,0,0,n)+c(1,0,0,n)*x1+c(2,0,0,n)*x2+c(3,0,0,n)*x3+c(4,0,0,
     & n)*x4)*time
! endLoops()
      else if( dx.eq.1.and.dy.eq.0.and.dz.eq.0.and.dt.eq.0  )then
! beginLoops($defineTime())
! defineTime()
      if( degreeTime.eq.0 )then
      time=a(0,n)
      else if( degreeTime.eq.1 )then
      time=a(0,n)+t*(a(1,n))
      else if( degreeTime.eq.2 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
      else if( degreeTime.eq.3 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
      else if( degreeTime.eq.4 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
      else if( degreeTime.eq.5 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))))
     & )
      else if( degreeTime.eq.6 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+t*(
     & a(6,n)))))))
      else
      write(*,*) 'ERROR invalid degreeTime'
      stop
      end if
c 
      x2=x1*x1
      x3=x2*x1
      r=(c(1,0,0,n)+c(2,0,0,n)*x1*2.+c(3,0,0,n)*x2*3.+c(4,0,0,n)*x3*4.)
     & *time
! endLoops()
      else if( dx.eq.2.and.dy.eq.0.and.dz.eq.0.and.dt.eq.0  )then
! beginLoops($defineTime())
! defineTime()
      if( degreeTime.eq.0 )then
      time=a(0,n)
      else if( degreeTime.eq.1 )then
      time=a(0,n)+t*(a(1,n))
      else if( degreeTime.eq.2 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
      else if( degreeTime.eq.3 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
      else if( degreeTime.eq.4 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
      else if( degreeTime.eq.5 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))))
     & )
      else if( degreeTime.eq.6 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+t*(
     & a(6,n)))))))
      else
      write(*,*) 'ERROR invalid degreeTime'
      stop
      end if
c 
      x2=x1*x1
      r=(c(2,0,0,n)*2.+c(3,0,0,n)*x1*6.+c(4,0,0,n)*x2*12.)*time
! endLoops()
      else if( dx.eq.3.and.dy.eq.0.and.dz.eq.0.and.dt.eq.0  )then
! beginLoops($defineTime())
! defineTime()
      if( degreeTime.eq.0 )then
      time=a(0,n)
      else if( degreeTime.eq.1 )then
      time=a(0,n)+t*(a(1,n))
      else if( degreeTime.eq.2 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
      else if( degreeTime.eq.3 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
      else if( degreeTime.eq.4 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
      else if( degreeTime.eq.5 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))))
     & )
      else if( degreeTime.eq.6 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+t*(
     & a(6,n)))))))
      else
      write(*,*) 'ERROR invalid degreeTime'
      stop
      end if
c 
      r=(c(3,0,0,n)*6.+c(4,0,0,n)*x1*24.)*time
! endLoops()
      else if( dx.eq.4.and.dy.eq.0.and.dz.eq.0.and.dt.eq.0  )then
! beginLoops($defineTime())
! defineTime()
      if( degreeTime.eq.0 )then
      time=a(0,n)
      else if( degreeTime.eq.1 )then
      time=a(0,n)+t*(a(1,n))
      else if( degreeTime.eq.2 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
      else if( degreeTime.eq.3 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
      else if( degreeTime.eq.4 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
      else if( degreeTime.eq.5 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))))
     & )
      else if( degreeTime.eq.6 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+t*(
     & a(6,n)))))))
      else
      write(*,*) 'ERROR invalid degreeTime'
      stop
      end if
      r=(c(4,0,0,n)*24.)*time
! endLoops()
      else if( laplace.eq.1 )then
! beginLoops($defineTime())
! defineTime()
      if( degreeTime.eq.0 )then
      time=a(0,n)
      else if( degreeTime.eq.1 )then
      time=a(0,n)+t*(a(1,n))
      else if( degreeTime.eq.2 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
      else if( degreeTime.eq.3 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
      else if( degreeTime.eq.4 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
      else if( degreeTime.eq.5 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))))
     & )
      else if( degreeTime.eq.6 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+t*(
     & a(6,n)))))))
      else
      write(*,*) 'ERROR invalid degreeTime'
      stop
      end if
c
      x2=x1*x1
      r=(+c(2,0,0,n)*2.+c(3,0,0,n)*x1*6.+c(4,0,0,n)*x2*12.)*time
! endLoops()
      else
! beginLoops(time=0.)
      time=0.
      r=0.
! endLoops()
      end if
! #If "1D4" eq "3D5" || "1D4" eq "3D6"
      return
      end
! polyFun(1D5)
      subroutine polyEvaluate1D5 (r, x1,y1,z1,n,t, ndc1,ndc2,ndc3,nda, 
     & degree, degreeTime, a, c, dx,dy,dz,dt)
      !==========================================================================
      !==========================================================================
      integer ndc1,ndc2,ndc3,nda
      integer n,dx,dy,dz,dt,degree,degreeTime
      real x1,y1,z1,r
      real c(0:ndc1-1,0:ndc2-1,0:ndc3-1,0:*)
      real a(0:nda-1,0:*)
      real t
      integer laplace
      real time
      laplace=0
      if( dx.eq.-2 )then
      laplace=1
      end if
! #If "1D5" eq "3D5" || "1D5" eq "3D6"
! poly1D5(c,c,c,r)
      if( dx.eq.0.and.dy.eq.0.and.dz.eq.0.and.dt.eq.0 )then
! beginLoops($defineTime())
! defineTime()
      if( degreeTime.eq.0 )then
      time=a(0,n)
      else if( degreeTime.eq.1 )then
      time=a(0,n)+t*(a(1,n))
      else if( degreeTime.eq.2 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
      else if( degreeTime.eq.3 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
      else if( degreeTime.eq.4 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
      else if( degreeTime.eq.5 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))))
     & )
      else if( degreeTime.eq.6 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+t*(
     & a(6,n)))))))
      else
      write(*,*) 'ERROR invalid degreeTime'
      stop
      end if
c 
      x2=x1*x1
      x3=x2*x1
      x4=x3*x1
      x5=x4*x1
      r=(c(0,0,0,n)+c(1,0,0,n)*x1+c(2,0,0,n)*x2+c(3,0,0,n)*x3+c(4,0,0,
     & n)*x4+c(5,0,0,n)*x5)*time
! endLoops()
      else if( dx.eq.0.and.dy.eq.0.and.dz.eq.0.and.dt.ge.1 )then
! beginLoops($defineTimeDerivative())
! defineTimeDerivative()
      if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
c
      x2=x1*x1
      x3=x2*x1
      x4=x3*x1
      x5=x4*x1
      r=(c(0,0,0,n)+c(1,0,0,n)*x1+c(2,0,0,n)*x2+c(3,0,0,n)*x3+c(4,0,0,
     & n)*x4+c(5,0,0,n)*x5)*time
! endLoops()
      else if( dx.eq.1.and.dy.eq.0.and.dz.eq.0.and.dt.eq.0  )then
! beginLoops($defineTime())
! defineTime()
      if( degreeTime.eq.0 )then
      time=a(0,n)
      else if( degreeTime.eq.1 )then
      time=a(0,n)+t*(a(1,n))
      else if( degreeTime.eq.2 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
      else if( degreeTime.eq.3 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
      else if( degreeTime.eq.4 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
      else if( degreeTime.eq.5 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))))
     & )
      else if( degreeTime.eq.6 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+t*(
     & a(6,n)))))))
      else
      write(*,*) 'ERROR invalid degreeTime'
      stop
      end if
c 
      x2=x1*x1
      x3=x2*x1
      x4=x3*x1
      r=(c(1,0,0,n)+c(2,0,0,n)*x1*2.+c(3,0,0,n)*x2*3.+c(4,0,0,n)*x3*4.+
     & c(5,0,0,n)*x4*5.)*time
! endLoops()
      else if( dx.eq.2.and.dy.eq.0.and.dz.eq.0.and.dt.eq.0  )then
! beginLoops($defineTime())
! defineTime()
      if( degreeTime.eq.0 )then
      time=a(0,n)
      else if( degreeTime.eq.1 )then
      time=a(0,n)+t*(a(1,n))
      else if( degreeTime.eq.2 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
      else if( degreeTime.eq.3 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
      else if( degreeTime.eq.4 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
      else if( degreeTime.eq.5 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))))
     & )
      else if( degreeTime.eq.6 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+t*(
     & a(6,n)))))))
      else
      write(*,*) 'ERROR invalid degreeTime'
      stop
      end if
c 
      x2=x1*x1
      x3=x2*x1
      r=(c(2,0,0,n)*2.+c(3,0,0,n)*x1*6.+c(4,0,0,n)*x2*12.+c(5,0,0,n)*
     & x3*20.)*time
! endLoops()
      else if( dx.eq.3.and.dy.eq.0.and.dz.eq.0.and.dt.eq.0  )then
! beginLoops($defineTime())
! defineTime()
      if( degreeTime.eq.0 )then
      time=a(0,n)
      else if( degreeTime.eq.1 )then
      time=a(0,n)+t*(a(1,n))
      else if( degreeTime.eq.2 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
      else if( degreeTime.eq.3 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
      else if( degreeTime.eq.4 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
      else if( degreeTime.eq.5 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))))
     & )
      else if( degreeTime.eq.6 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+t*(
     & a(6,n)))))))
      else
      write(*,*) 'ERROR invalid degreeTime'
      stop
      end if
c 
      x2=x1*x1
      r=(c(3,0,0,n)*6.+c(4,0,0,n)*x1*24.+c(5,0,0,n)*x2*60.)*time
! endLoops()
      else if( dx.eq.4.and.dy.eq.0.and.dz.eq.0.and.dt.eq.0  )then
! beginLoops($defineTime())
! defineTime()
      if( degreeTime.eq.0 )then
      time=a(0,n)
      else if( degreeTime.eq.1 )then
      time=a(0,n)+t*(a(1,n))
      else if( degreeTime.eq.2 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
      else if( degreeTime.eq.3 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
      else if( degreeTime.eq.4 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
      else if( degreeTime.eq.5 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))))
     & )
      else if( degreeTime.eq.6 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+t*(
     & a(6,n)))))))
      else
      write(*,*) 'ERROR invalid degreeTime'
      stop
      end if
c 
      r=(c(4,0,0,n)*24.+c(5,0,0,n)*x1*120.)*time
! endLoops()
      else if( laplace.eq.1 )then
! beginLoops($defineTime())
! defineTime()
      if( degreeTime.eq.0 )then
      time=a(0,n)
      else if( degreeTime.eq.1 )then
      time=a(0,n)+t*(a(1,n))
      else if( degreeTime.eq.2 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
      else if( degreeTime.eq.3 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
      else if( degreeTime.eq.4 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
      else if( degreeTime.eq.5 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))))
     & )
      else if( degreeTime.eq.6 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+t*(
     & a(6,n)))))))
      else
      write(*,*) 'ERROR invalid degreeTime'
      stop
      end if
c
      x2=x1*x1
      x3=x2*x1
      r=(+c(2,0,0,n)*2.+c(3,0,0,n)*x1*6.+c(4,0,0,n)*x2*12.+c(5,0,0,n)*
     & x3*20.)*time
! endLoops()
      else
! beginLoops(time=0.)
      time=0.
      r=0.
! endLoops()
      end if
! #If "1D5" eq "3D5" || "1D5" eq "3D6"
      return
      end
! polyFun(1D6)
      subroutine polyEvaluate1D6 (r, x1,y1,z1,n,t, ndc1,ndc2,ndc3,nda, 
     & degree, degreeTime, a, c, dx,dy,dz,dt)
      !==========================================================================
      !==========================================================================
      integer ndc1,ndc2,ndc3,nda
      integer n,dx,dy,dz,dt,degree,degreeTime
      real x1,y1,z1,r
      real c(0:ndc1-1,0:ndc2-1,0:ndc3-1,0:*)
      real a(0:nda-1,0:*)
      real t
      integer laplace
      real time
      laplace=0
      if( dx.eq.-2 )then
      laplace=1
      end if
! #If "1D6" eq "3D5" || "1D6" eq "3D6"
! poly1D6(c,c,c,r)
      if( dx.eq.0.and.dy.eq.0.and.dz.eq.0.and.dt.eq.0 )then
! beginLoops($defineTime())
! defineTime()
      if( degreeTime.eq.0 )then
      time=a(0,n)
      else if( degreeTime.eq.1 )then
      time=a(0,n)+t*(a(1,n))
      else if( degreeTime.eq.2 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
      else if( degreeTime.eq.3 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
      else if( degreeTime.eq.4 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
      else if( degreeTime.eq.5 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))))
     & )
      else if( degreeTime.eq.6 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+t*(
     & a(6,n)))))))
      else
      write(*,*) 'ERROR invalid degreeTime'
      stop
      end if
c 
      x2=x1*x1
      x3=x2*x1
      x4=x3*x1
      x5=x4*x1
      x6=x5*x1
      r=(c(0,0,0,n)+c(1,0,0,n)*x1+c(2,0,0,n)*x2+c(3,0,0,n)*x3+c(4,0,0,
     & n)*x4+c(5,0,0,n)*x5+c(6,0,0,n)*x6)*time
! endLoops()
      else if( dx.eq.0.and.dy.eq.0.and.dz.eq.0.and.dt.ge.1 )then
! beginLoops($defineTimeDerivative())
! defineTimeDerivative()
      if( dt.eq.1 )then
       ! --- first time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=a(1,n)
       else if( degreeTime.eq.2 )then
        time=a(1,n)+t*(2.*a(2,n))
       else if( degreeTime.eq.3 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
       else if( degreeTime.eq.4 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
       else if( degreeTime.eq.5 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & ))))
       else if( degreeTime.eq.6 )then
        time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)
     & +t*(6.*a(6,n))))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1834
       end if
      else if( dt.eq.2 )then
       ! --- 2nd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=2.*a(2,n)
       else if( degreeTime.eq.3 )then
        time=2.*a(2,n)+t*(6.*a(3,n))
       else if( degreeTime.eq.4 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
       else if( degreeTime.eq.5 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
       else if( degreeTime.eq.6 )then
        time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*
     & a(6,n)))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1835
       end if
      else if( dt.eq.3 )then
       ! --- 3rd time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=6.*a(3,n)
       else if( degreeTime.eq.4 )then
        time=6.*a(3,n)+t*(24.*a(4,n))
       else if( degreeTime.eq.5 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
       else if( degreeTime.eq.6 )then
        time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1836
       end if
      else if( dt.eq.4 )then
       ! --- 4th time derivative ---
       if( degreeTime.eq.0 )then
        time=0.
       else if( degreeTime.eq.1 )then
        time=0.
       else if( degreeTime.eq.2 )then
        time=0.
       else if( degreeTime.eq.3 )then
        time=0.
       else if( degreeTime.eq.4 )then
        time=24.*a(4,n)
       else if( degreeTime.eq.5 )then
        time=24.*a(4,n)+t*(120.*a(5,n))
       else if( degreeTime.eq.6 )then
        time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
       else
        write(*,*) 'PolyFunction:ERROR invalid degreeTime'
        stop 1837
       end if
      else
        write(*,*) 'PolyFunction:ERROR: too many time derivatives'
        stop 1838
      end if
c
      x2=x1*x1
      x3=x2*x1
      x4=x3*x1
      x5=x4*x1
      x6=x5*x1
      r=(c(0,0,0,n)+c(1,0,0,n)*x1+c(2,0,0,n)*x2+c(3,0,0,n)*x3+c(4,0,0,
     & n)*x4+c(5,0,0,n)*x5+c(6,0,0,n)*x6)*time
! endLoops()
      else if( dx.eq.1.and.dy.eq.0.and.dz.eq.0.and.dt.eq.0  )then
! beginLoops($defineTime())
! defineTime()
      if( degreeTime.eq.0 )then
      time=a(0,n)
      else if( degreeTime.eq.1 )then
      time=a(0,n)+t*(a(1,n))
      else if( degreeTime.eq.2 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
      else if( degreeTime.eq.3 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
      else if( degreeTime.eq.4 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
      else if( degreeTime.eq.5 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))))
     & )
      else if( degreeTime.eq.6 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+t*(
     & a(6,n)))))))
      else
      write(*,*) 'ERROR invalid degreeTime'
      stop
      end if
c 
      x2=x1*x1
      x3=x2*x1
      x4=x3*x1
      x5=x4*x1
      r=(c(1,0,0,n)+c(2,0,0,n)*x1*2.+c(3,0,0,n)*x2*3.+c(4,0,0,n)*x3*4.+
     & c(5,0,0,n)*x4*5.+c(6,0,0,n)*x5*6.)*time
! endLoops()
      else if( dx.eq.2.and.dy.eq.0.and.dz.eq.0.and.dt.eq.0  )then
! beginLoops($defineTime())
! defineTime()
      if( degreeTime.eq.0 )then
      time=a(0,n)
      else if( degreeTime.eq.1 )then
      time=a(0,n)+t*(a(1,n))
      else if( degreeTime.eq.2 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
      else if( degreeTime.eq.3 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
      else if( degreeTime.eq.4 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
      else if( degreeTime.eq.5 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))))
     & )
      else if( degreeTime.eq.6 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+t*(
     & a(6,n)))))))
      else
      write(*,*) 'ERROR invalid degreeTime'
      stop
      end if
c 
      x2=x1*x1
      x3=x2*x1
      x4=x3*x1
      r=(c(2,0,0,n)*2.+c(3,0,0,n)*x1*6.+c(4,0,0,n)*x2*12.+c(5,0,0,n)*
     & x3*20.+c(6,0,0,n)*x4*30.)*time
! endLoops()
      else if( dx.eq.3.and.dy.eq.0.and.dz.eq.0.and.dt.eq.0  )then
! beginLoops($defineTime())
! defineTime()
      if( degreeTime.eq.0 )then
      time=a(0,n)
      else if( degreeTime.eq.1 )then
      time=a(0,n)+t*(a(1,n))
      else if( degreeTime.eq.2 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
      else if( degreeTime.eq.3 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
      else if( degreeTime.eq.4 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
      else if( degreeTime.eq.5 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))))
     & )
      else if( degreeTime.eq.6 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+t*(
     & a(6,n)))))))
      else
      write(*,*) 'ERROR invalid degreeTime'
      stop
      end if
c 
      x2=x1*x1
      x3=x2*x1
      r=(c(3,0,0,n)*6.+c(4,0,0,n)*x1*24.+c(5,0,0,n)*x2*60.+c(6,0,0,n)*
     & x3*120.)*time
! endLoops()
      else if( dx.eq.4.and.dy.eq.0.and.dz.eq.0.and.dt.eq.0  )then
! beginLoops($defineTime())
! defineTime()
      if( degreeTime.eq.0 )then
      time=a(0,n)
      else if( degreeTime.eq.1 )then
      time=a(0,n)+t*(a(1,n))
      else if( degreeTime.eq.2 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
      else if( degreeTime.eq.3 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
      else if( degreeTime.eq.4 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
      else if( degreeTime.eq.5 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))))
     & )
      else if( degreeTime.eq.6 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+t*(
     & a(6,n)))))))
      else
      write(*,*) 'ERROR invalid degreeTime'
      stop
      end if
c 
      x2=x1*x1
      r=(c(4,0,0,n)*24.+c(5,0,0,n)*x1*120.+c(6,0,0,n)*x2*360.)*time
! endLoops()
      else if( laplace.eq.1 )then
! beginLoops($defineTime())
! defineTime()
      if( degreeTime.eq.0 )then
      time=a(0,n)
      else if( degreeTime.eq.1 )then
      time=a(0,n)+t*(a(1,n))
      else if( degreeTime.eq.2 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
      else if( degreeTime.eq.3 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
      else if( degreeTime.eq.4 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
      else if( degreeTime.eq.5 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)))))
     & )
      else if( degreeTime.eq.6 )then
      time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+t*(
     & a(6,n)))))))
      else
      write(*,*) 'ERROR invalid degreeTime'
      stop
      end if
c
      x2=x1*x1
      x3=x2*x1
      x4=x3*x1
      r=(+c(2,0,0,n)*2.+c(3,0,0,n)*x1*6.+c(4,0,0,n)*x2*12.+c(5,0,0,n)*
     & x3*20.+c(6,0,0,n)*x4*30.)*time
! endLoops()
      else
! beginLoops(time=0.)
      time=0.
      r=0.
! endLoops()
      end if
! #If "1D6" eq "3D5" || "1D6" eq "3D6"
      return
      end
