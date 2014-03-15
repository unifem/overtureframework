! This file automatically generated from polyFunction.bf with bpp.
! polyFun(3D0)
      subroutine polyEvaluate3D0 (r, x1,y1,z1,n,t, ndc1,ndc2,ndc3,nda, 
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
! #If "3D0" eq "3D5" || "3D0" eq "3D6"
! poly3D0(c,c,c,r)
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
! #If "3D0" eq "3D5" || "3D0" eq "3D6"
      return
      end
! polyFun(3D1)
      subroutine polyEvaluate3D1 (r, x1,y1,z1,n,t, ndc1,ndc2,ndc3,nda, 
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
! #If "3D1" eq "3D5" || "3D1" eq "3D6"
! poly3D1(c,c,c,r)
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
c 
      y1z1=y1*z1
c 
      x1z1=x1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      r=(c(0,0,0,n)+c(0,0,1,n)*z1+c(0,1,0,n)*y1+c(0,1,1,n)*y1z1+c(1,0,
     & 0,n)*x1+c(1,0,1,n)*x1z1+c(1,1,0,n)*x1y1+c(1,1,1,n)*x1y1z1)*time
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
c
      y1z1=y1*z1
c
      x1z1=x1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      r=(c(0,0,0,n)+c(0,0,1,n)*z1+c(0,1,0,n)*y1+c(0,1,1,n)*y1z1+c(1,0,
     & 0,n)*x1+c(1,0,1,n)*x1z1+c(1,1,0,n)*x1y1+c(1,1,1,n)*x1y1z1)*time
! endLoops()
      else if( dx.eq.0.and.dy.eq.0.and.dz.eq.1.and.dt.eq.0  )then
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
c 
      x1y1=x1*y1
      r=(c(0,0,1,n)+c(0,1,1,n)*y1+c(1,0,1,n)*x1+c(1,1,1,n)*x1y1)*time
! endLoops()
      else if( dx.eq.0.and.dy.eq.1.and.dz.eq.0.and.dt.eq.0  )then
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
c 
      x1z1=x1*z1
      r=(c(0,1,0,n)+c(0,1,1,n)*z1+c(1,1,0,n)*x1+c(1,1,1,n)*x1z1)*time
! endLoops()
      else if( dx.eq.0.and.dy.eq.1.and.dz.eq.1.and.dt.eq.0  )then
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
      r=(c(0,1,1,n)+c(1,1,1,n)*x1)*time
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
c 
      y1z1=y1*z1
      r=(c(1,0,0,n)+c(1,0,1,n)*z1+c(1,1,0,n)*y1+c(1,1,1,n)*y1z1)*time
! endLoops()
      else if( dx.eq.1.and.dy.eq.0.and.dz.eq.1.and.dt.eq.0  )then
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
      r=(c(1,0,1,n)+c(1,1,1,n)*y1)*time
! endLoops()
      else if( dx.eq.1.and.dy.eq.1.and.dz.eq.0.and.dt.eq.0  )then
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
      r=(c(1,1,0,n)+c(1,1,1,n)*z1)*time
! endLoops()
      else if( dx.eq.1.and.dy.eq.1.and.dz.eq.1.and.dt.eq.0  )then
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
      r=(c(1,1,1,n))*time
! endLoops()
      else
! beginLoops(time=0.)
      time=0.
      r=0.
! endLoops()
      end if
! #If "3D1" eq "3D5" || "3D1" eq "3D6"
      return
      end
! polyFun(3D2)
      subroutine polyEvaluate3D2 (r, x1,y1,z1,n,t, ndc1,ndc2,ndc3,nda, 
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
! #If "3D2" eq "3D5" || "3D2" eq "3D6"
! poly3D2(c,c,c,r)
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
      z2=z1*z1
c 
      y1z1=y1*z1
      y1z2=y1z1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
c 
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
      r=(c(0,0,0,n)+c(0,0,1,n)*z1+c(0,0,2,n)*z2+c(0,1,0,n)*y1+c(0,1,1,
     & n)*y1z1+c(0,1,2,n)*y1z2+c(0,2,0,n)*y2+c(0,2,1,n)*y2z1+c(0,2,2,
     & n)*y2z2+c(1,0,0,n)*x1+c(1,0,1,n)*x1z1+c(1,0,2,n)*x1z2+c(1,1,0,
     & n)*x1y1+c(1,1,1,n)*x1y1z1+c(1,1,2,n)*x1y1z2+c(1,2,0,n)*x1y2+c(
     & 1,2,1,n)*x1y2z1+c(1,2,2,n)*x1y2z2+c(2,0,0,n)*x2+c(2,0,1,n)*
     & x2z1+c(2,0,2,n)*x2z2+c(2,1,0,n)*x2y1+c(2,1,1,n)*x2y1z1+c(2,1,2,
     & n)*x2y1z2+c(2,2,0,n)*x2y2+c(2,2,1,n)*x2y2z1+c(2,2,2,n)*x2y2z2)*
     & time
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
      z2=z1*z1
c
      y1z1=y1*z1
      y1z2=y1z1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
c
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
      r=(c(0,0,0,n)+c(0,0,1,n)*z1+c(0,0,2,n)*z2+c(0,1,0,n)*y1+c(0,1,1,
     & n)*y1z1+c(0,1,2,n)*y1z2+c(0,2,0,n)*y2+c(0,2,1,n)*y2z1+c(0,2,2,
     & n)*y2z2+c(1,0,0,n)*x1+c(1,0,1,n)*x1z1+c(1,0,2,n)*x1z2+c(1,1,0,
     & n)*x1y1+c(1,1,1,n)*x1y1z1+c(1,1,2,n)*x1y1z2+c(1,2,0,n)*x1y2+c(
     & 1,2,1,n)*x1y2z1+c(1,2,2,n)*x1y2z2+c(2,0,0,n)*x2+c(2,0,1,n)*
     & x2z1+c(2,0,2,n)*x2z2+c(2,1,0,n)*x2y1+c(2,1,1,n)*x2y1z1+c(2,1,2,
     & n)*x2y1z2+c(2,2,0,n)*x2y2+c(2,2,1,n)*x2y2z1+c(2,2,2,n)*x2y2z2)*
     & time
! endLoops()
      else if( dx.eq.0.and.dy.eq.0.and.dz.eq.1.and.dt.eq.0  )then
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
c 
      y1z1=y1*z1
      y2=y1*y1
      y2z1=y1z1*y1
c 
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
      r=(c(0,0,1,n)+c(0,0,2,n)*z1*2.+c(0,1,1,n)*y1+c(0,1,2,n)*y1z1*2.+
     & c(0,2,1,n)*y2+c(0,2,2,n)*y2z1*2.+c(1,0,1,n)*x1+c(1,0,2,n)*x1z1*
     & 2.+c(1,1,1,n)*x1y1+c(1,1,2,n)*x1y1z1*2.+c(1,2,1,n)*x1y2+c(1,2,
     & 2,n)*x1y2z1*2.+c(2,0,1,n)*x2+c(2,0,2,n)*x2z1*2.+c(2,1,1,n)*
     & x2y1+c(2,1,2,n)*x2y1z1*2.+c(2,2,1,n)*x2y2+c(2,2,2,n)*x2y2z1*2.)
     & *time
! endLoops()
      else if( dx.eq.0.and.dy.eq.0.and.dz.eq.2.and.dt.eq.0  )then
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
      y2=y1*y1
c 
      x1y1=x1*y1
      x1y2=x1y1*y1
      x2=x1*x1
      x2y1=x1y1*x1
      x2y2=x1y2*x1
      r=(c(0,0,2,n)*2.+c(0,1,2,n)*y1*2.+c(0,2,2,n)*y2*2.+c(1,0,2,n)*x1*
     & 2.+c(1,1,2,n)*x1y1*2.+c(1,2,2,n)*x1y2*2.+c(2,0,2,n)*x2*2.+c(2,
     & 1,2,n)*x2y1*2.+c(2,2,2,n)*x2y2*2.)*time
! endLoops()
      else if( dx.eq.0.and.dy.eq.1.and.dz.eq.0.and.dt.eq.0  )then
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
      z2=z1*z1
c 
      y1z1=y1*z1
      y1z2=y1z1*z1
c 
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
      r=(c(0,1,0,n)+c(0,1,1,n)*z1+c(0,1,2,n)*z2+c(0,2,0,n)*y1*2.+c(0,2,
     & 1,n)*y1z1*2.+c(0,2,2,n)*y1z2*2.+c(1,1,0,n)*x1+c(1,1,1,n)*x1z1+
     & c(1,1,2,n)*x1z2+c(1,2,0,n)*x1y1*2.+c(1,2,1,n)*x1y1z1*2.+c(1,2,
     & 2,n)*x1y1z2*2.+c(2,1,0,n)*x2+c(2,1,1,n)*x2z1+c(2,1,2,n)*x2z2+c(
     & 2,2,0,n)*x2y1*2.+c(2,2,1,n)*x2y1z1*2.+c(2,2,2,n)*x2y1z2*2.)*
     & time
! endLoops()
      else if( dx.eq.0.and.dy.eq.1.and.dz.eq.1.and.dt.eq.0  )then
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
c 
      y1z1=y1*z1
c 
      x1z1=x1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x2=x1*x1
      x2z1=x1z1*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      r=(c(0,1,1,n)+c(0,1,2,n)*z1*2.+c(0,2,1,n)*y1*2.+c(0,2,2,n)*y1z1*
     & 4.+c(1,1,1,n)*x1+c(1,1,2,n)*x1z1*2.+c(1,2,1,n)*x1y1*2.+c(1,2,2,
     & n)*x1y1z1*4.+c(2,1,1,n)*x2+c(2,1,2,n)*x2z1*2.+c(2,2,1,n)*x2y1*
     & 2.+c(2,2,2,n)*x2y1z1*4.)*time
! endLoops()
      else if( dx.eq.0.and.dy.eq.1.and.dz.eq.2.and.dt.eq.0  )then
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
c 
      x1y1=x1*y1
      x2=x1*x1
      x2y1=x1y1*x1
      r=(c(0,1,2,n)*2.+c(0,2,2,n)*y1*4.+c(1,1,2,n)*x1*2.+c(1,2,2,n)*
     & x1y1*4.+c(2,1,2,n)*x2*2.+c(2,2,2,n)*x2y1*4.)*time
! endLoops()
      else if( dx.eq.0.and.dy.eq.2.and.dz.eq.0.and.dt.eq.0  )then
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
      z2=z1*z1
c 
      x1z1=x1*z1
      x1z2=x1z1*z1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      r=(c(0,2,0,n)*2.+c(0,2,1,n)*z1*2.+c(0,2,2,n)*z2*2.+c(1,2,0,n)*x1*
     & 2.+c(1,2,1,n)*x1z1*2.+c(1,2,2,n)*x1z2*2.+c(2,2,0,n)*x2*2.+c(2,
     & 2,1,n)*x2z1*2.+c(2,2,2,n)*x2z2*2.)*time
! endLoops()
      else if( dx.eq.0.and.dy.eq.2.and.dz.eq.1.and.dt.eq.0  )then
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
c 
      x1z1=x1*z1
      x2=x1*x1
      x2z1=x1z1*x1
      r=(c(0,2,1,n)*2.+c(0,2,2,n)*z1*4.+c(1,2,1,n)*x1*2.+c(1,2,2,n)*
     & x1z1*4.+c(2,2,1,n)*x2*2.+c(2,2,2,n)*x2z1*4.)*time
! endLoops()
      else if( dx.eq.0.and.dy.eq.2.and.dz.eq.2.and.dt.eq.0  )then
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
      r=(c(0,2,2,n)*4.+c(1,2,2,n)*x1*4.+c(2,2,2,n)*x2*4.)*time
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
      z2=z1*z1
c 
      y1z1=y1*z1
      y1z2=y1z1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
c 
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      r=(c(1,0,0,n)+c(1,0,1,n)*z1+c(1,0,2,n)*z2+c(1,1,0,n)*y1+c(1,1,1,
     & n)*y1z1+c(1,1,2,n)*y1z2+c(1,2,0,n)*y2+c(1,2,1,n)*y2z1+c(1,2,2,
     & n)*y2z2+c(2,0,0,n)*x1*2.+c(2,0,1,n)*x1z1*2.+c(2,0,2,n)*x1z2*2.+
     & c(2,1,0,n)*x1y1*2.+c(2,1,1,n)*x1y1z1*2.+c(2,1,2,n)*x1y1z2*2.+c(
     & 2,2,0,n)*x1y2*2.+c(2,2,1,n)*x1y2z1*2.+c(2,2,2,n)*x1y2z2*2.)*
     & time
! endLoops()
      else if( dx.eq.1.and.dy.eq.0.and.dz.eq.1.and.dt.eq.0  )then
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
c 
      y1z1=y1*z1
      y2=y1*y1
      y2z1=y1z1*y1
c 
      x1z1=x1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      r=(c(1,0,1,n)+c(1,0,2,n)*z1*2.+c(1,1,1,n)*y1+c(1,1,2,n)*y1z1*2.+
     & c(1,2,1,n)*y2+c(1,2,2,n)*y2z1*2.+c(2,0,1,n)*x1*2.+c(2,0,2,n)*
     & x1z1*4.+c(2,1,1,n)*x1y1*2.+c(2,1,2,n)*x1y1z1*4.+c(2,2,1,n)*
     & x1y2*2.+c(2,2,2,n)*x1y2z1*4.)*time
! endLoops()
      else if( dx.eq.1.and.dy.eq.0.and.dz.eq.2.and.dt.eq.0  )then
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
      y2=y1*y1
c 
      x1y1=x1*y1
      x1y2=x1y1*y1
      r=(c(1,0,2,n)*2.+c(1,1,2,n)*y1*2.+c(1,2,2,n)*y2*2.+c(2,0,2,n)*x1*
     & 4.+c(2,1,2,n)*x1y1*4.+c(2,2,2,n)*x1y2*4.)*time
! endLoops()
      else if( dx.eq.1.and.dy.eq.1.and.dz.eq.0.and.dt.eq.0  )then
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
      z2=z1*z1
c 
      y1z1=y1*z1
      y1z2=y1z1*z1
c 
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      r=(c(1,1,0,n)+c(1,1,1,n)*z1+c(1,1,2,n)*z2+c(1,2,0,n)*y1*2.+c(1,2,
     & 1,n)*y1z1*2.+c(1,2,2,n)*y1z2*2.+c(2,1,0,n)*x1*2.+c(2,1,1,n)*
     & x1z1*2.+c(2,1,2,n)*x1z2*2.+c(2,2,0,n)*x1y1*4.+c(2,2,1,n)*
     & x1y1z1*4.+c(2,2,2,n)*x1y1z2*4.)*time
! endLoops()
      else if( dx.eq.1.and.dy.eq.1.and.dz.eq.1.and.dt.eq.0  )then
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
c 
      y1z1=y1*z1
c 
      x1z1=x1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      r=(c(1,1,1,n)+c(1,1,2,n)*z1*2.+c(1,2,1,n)*y1*2.+c(1,2,2,n)*y1z1*
     & 4.+c(2,1,1,n)*x1*2.+c(2,1,2,n)*x1z1*4.+c(2,2,1,n)*x1y1*4.+c(2,
     & 2,2,n)*x1y1z1*8.)*time
! endLoops()
      else if( dx.eq.1.and.dy.eq.1.and.dz.eq.2.and.dt.eq.0  )then
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
c 
      x1y1=x1*y1
      r=(c(1,1,2,n)*2.+c(1,2,2,n)*y1*4.+c(2,1,2,n)*x1*4.+c(2,2,2,n)*
     & x1y1*8.)*time
! endLoops()
      else if( dx.eq.1.and.dy.eq.2.and.dz.eq.0.and.dt.eq.0  )then
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
      z2=z1*z1
c 
      x1z1=x1*z1
      x1z2=x1z1*z1
      r=(c(1,2,0,n)*2.+c(1,2,1,n)*z1*2.+c(1,2,2,n)*z2*2.+c(2,2,0,n)*x1*
     & 4.+c(2,2,1,n)*x1z1*4.+c(2,2,2,n)*x1z2*4.)*time
! endLoops()
      else if( dx.eq.1.and.dy.eq.2.and.dz.eq.1.and.dt.eq.0  )then
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
c 
      x1z1=x1*z1
      r=(c(1,2,1,n)*2.+c(1,2,2,n)*z1*4.+c(2,2,1,n)*x1*4.+c(2,2,2,n)*
     & x1z1*8.)*time
! endLoops()
      else if( dx.eq.1.and.dy.eq.2.and.dz.eq.2.and.dt.eq.0  )then
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
      r=(c(1,2,2,n)*4.+c(2,2,2,n)*x1*8.)*time
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
      z2=z1*z1
c 
      y1z1=y1*z1
      y1z2=y1z1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      r=(c(2,0,0,n)*2.+c(2,0,1,n)*z1*2.+c(2,0,2,n)*z2*2.+c(2,1,0,n)*y1*
     & 2.+c(2,1,1,n)*y1z1*2.+c(2,1,2,n)*y1z2*2.+c(2,2,0,n)*y2*2.+c(2,
     & 2,1,n)*y2z1*2.+c(2,2,2,n)*y2z2*2.)*time
! endLoops()
      else if( dx.eq.2.and.dy.eq.0.and.dz.eq.1.and.dt.eq.0  )then
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
c 
      y1z1=y1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      r=(c(2,0,1,n)*2.+c(2,0,2,n)*z1*4.+c(2,1,1,n)*y1*2.+c(2,1,2,n)*
     & y1z1*4.+c(2,2,1,n)*y2*2.+c(2,2,2,n)*y2z1*4.)*time
! endLoops()
      else if( dx.eq.2.and.dy.eq.0.and.dz.eq.2.and.dt.eq.0  )then
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
      y2=y1*y1
      r=(c(2,0,2,n)*4.+c(2,1,2,n)*y1*4.+c(2,2,2,n)*y2*4.)*time
! endLoops()
      else if( dx.eq.2.and.dy.eq.1.and.dz.eq.0.and.dt.eq.0  )then
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
      z2=z1*z1
c 
      y1z1=y1*z1
      y1z2=y1z1*z1
      r=(c(2,1,0,n)*2.+c(2,1,1,n)*z1*2.+c(2,1,2,n)*z2*2.+c(2,2,0,n)*y1*
     & 4.+c(2,2,1,n)*y1z1*4.+c(2,2,2,n)*y1z2*4.)*time
! endLoops()
      else if( dx.eq.2.and.dy.eq.1.and.dz.eq.1.and.dt.eq.0  )then
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
c 
      y1z1=y1*z1
      r=(c(2,1,1,n)*2.+c(2,1,2,n)*z1*4.+c(2,2,1,n)*y1*4.+c(2,2,2,n)*
     & y1z1*8.)*time
! endLoops()
      else if( dx.eq.2.and.dy.eq.1.and.dz.eq.2.and.dt.eq.0  )then
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
      r=(c(2,1,2,n)*4.+c(2,2,2,n)*y1*8.)*time
! endLoops()
      else if( dx.eq.2.and.dy.eq.2.and.dz.eq.0.and.dt.eq.0  )then
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
      z2=z1*z1
      r=(c(2,2,0,n)*4.+c(2,2,1,n)*z1*4.+c(2,2,2,n)*z2*4.)*time
! endLoops()
      else if( dx.eq.2.and.dy.eq.2.and.dz.eq.1.and.dt.eq.0  )then
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
      r=(c(2,2,1,n)*4.+c(2,2,2,n)*z1*8.)*time
! endLoops()
      else if( dx.eq.2.and.dy.eq.2.and.dz.eq.2.and.dt.eq.0  )then
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
      r=(c(2,2,2,n)*8.)*time
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
      y2=y1*y1
c
      x1y1=x1*y1
      x1y2=x1y1*y1
      x2=x1*x1
      x2y1=x1y1*x1
      x2y2=x1y2*x1
c
      z2=z1*z1
      x1z1=x1*z1
      x1z2=x1z1*z1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      y1z1=y1*z1
      y1z2=y1z1*z1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      r=(+c(0,0,2,n)*2.+c(0,1,2,n)*y1*2.+c(0,2,2,n)*y2*2.+c(1,0,2,n)*
     & x1*2.+c(1,1,2,n)*x1y1*2.+c(1,2,2,n)*x1y2*2.+c(2,0,2,n)*x2*2.+c(
     & 2,1,2,n)*x2y1*2.+c(2,2,2,n)*x2y2*2.+c(0,2,0,n)*2.+c(0,2,1,n)*
     & z1*2.+c(0,2,2,n)*z2*2.+c(1,2,0,n)*x1*2.+c(1,2,1,n)*x1z1*2.+c(1,
     & 2,2,n)*x1z2*2.+c(2,2,0,n)*x2*2.+c(2,2,1,n)*x2z1*2.+c(2,2,2,n)*
     & x2z2*2.+c(2,0,0,n)*2.+c(2,0,1,n)*z1*2.+c(2,0,2,n)*z2*2.+c(2,1,
     & 0,n)*y1*2.+c(2,1,1,n)*y1z1*2.+c(2,1,2,n)*y1z2*2.+c(2,2,0,n)*y2*
     & 2.+c(2,2,1,n)*y2z1*2.+c(2,2,2,n)*y2z2*2.)*time
! endLoops()
      else
! beginLoops(time=0.)
      time=0.
      r=0.
! endLoops()
      end if
! #If "3D2" eq "3D5" || "3D2" eq "3D6"
      return
      end
! polyFun(3D3)
      subroutine polyEvaluate3D3 (r, x1,y1,z1,n,t, ndc1,ndc2,ndc3,nda, 
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
! #If "3D3" eq "3D5" || "3D3" eq "3D6"
! poly3D3(c,c,c,r)
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
      z2=z1*z1
      z3=z2*z1
c 
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
c 
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
      r=(c(0,0,0,n)+c(0,0,1,n)*z1+c(0,0,2,n)*z2+c(0,0,3,n)*z3+c(0,1,0,
     & n)*y1+c(0,1,1,n)*y1z1+c(0,1,2,n)*y1z2+c(0,1,3,n)*y1z3+c(0,2,0,
     & n)*y2+c(0,2,1,n)*y2z1+c(0,2,2,n)*y2z2+c(0,2,3,n)*y2z3+c(0,3,0,
     & n)*y3+c(0,3,1,n)*y3z1+c(0,3,2,n)*y3z2+c(0,3,3,n)*y3z3+c(1,0,0,
     & n)*x1+c(1,0,1,n)*x1z1+c(1,0,2,n)*x1z2+c(1,0,3,n)*x1z3+c(1,1,0,
     & n)*x1y1+c(1,1,1,n)*x1y1z1+c(1,1,2,n)*x1y1z2+c(1,1,3,n)*x1y1z3+
     & c(1,2,0,n)*x1y2+c(1,2,1,n)*x1y2z1+c(1,2,2,n)*x1y2z2+c(1,2,3,n)*
     & x1y2z3+c(1,3,0,n)*x1y3+c(1,3,1,n)*x1y3z1+c(1,3,2,n)*x1y3z2+c(1,
     & 3,3,n)*x1y3z3+c(2,0,0,n)*x2+c(2,0,1,n)*x2z1+c(2,0,2,n)*x2z2+c(
     & 2,0,3,n)*x2z3+c(2,1,0,n)*x2y1+c(2,1,1,n)*x2y1z1+c(2,1,2,n)*
     & x2y1z2+c(2,1,3,n)*x2y1z3+c(2,2,0,n)*x2y2+c(2,2,1,n)*x2y2z1+c(2,
     & 2,2,n)*x2y2z2+c(2,2,3,n)*x2y2z3+c(2,3,0,n)*x2y3+c(2,3,1,n)*
     & x2y3z1+c(2,3,2,n)*x2y3z2+c(2,3,3,n)*x2y3z3+c(3,0,0,n)*x3+c(3,0,
     & 1,n)*x3z1+c(3,0,2,n)*x3z2+c(3,0,3,n)*x3z3+c(3,1,0,n)*x3y1+c(3,
     & 1,1,n)*x3y1z1+c(3,1,2,n)*x3y1z2+c(3,1,3,n)*x3y1z3+c(3,2,0,n)*
     & x3y2+c(3,2,1,n)*x3y2z1+c(3,2,2,n)*x3y2z2+c(3,2,3,n)*x3y2z3+c(3,
     & 3,0,n)*x3y3+c(3,3,1,n)*x3y3z1+c(3,3,2,n)*x3y3z2+c(3,3,3,n)*
     & x3y3z3)*time
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
      z2=z1*z1
      z3=z2*z1
c
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
c
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
      r=(c(0,0,0,n)+c(0,0,1,n)*z1+c(0,0,2,n)*z2+c(0,0,3,n)*z3+c(0,1,0,
     & n)*y1+c(0,1,1,n)*y1z1+c(0,1,2,n)*y1z2+c(0,1,3,n)*y1z3+c(0,2,0,
     & n)*y2+c(0,2,1,n)*y2z1+c(0,2,2,n)*y2z2+c(0,2,3,n)*y2z3+c(0,3,0,
     & n)*y3+c(0,3,1,n)*y3z1+c(0,3,2,n)*y3z2+c(0,3,3,n)*y3z3+c(1,0,0,
     & n)*x1+c(1,0,1,n)*x1z1+c(1,0,2,n)*x1z2+c(1,0,3,n)*x1z3+c(1,1,0,
     & n)*x1y1+c(1,1,1,n)*x1y1z1+c(1,1,2,n)*x1y1z2+c(1,1,3,n)*x1y1z3+
     & c(1,2,0,n)*x1y2+c(1,2,1,n)*x1y2z1+c(1,2,2,n)*x1y2z2+c(1,2,3,n)*
     & x1y2z3+c(1,3,0,n)*x1y3+c(1,3,1,n)*x1y3z1+c(1,3,2,n)*x1y3z2+c(1,
     & 3,3,n)*x1y3z3+c(2,0,0,n)*x2+c(2,0,1,n)*x2z1+c(2,0,2,n)*x2z2+c(
     & 2,0,3,n)*x2z3+c(2,1,0,n)*x2y1+c(2,1,1,n)*x2y1z1+c(2,1,2,n)*
     & x2y1z2+c(2,1,3,n)*x2y1z3+c(2,2,0,n)*x2y2+c(2,2,1,n)*x2y2z1+c(2,
     & 2,2,n)*x2y2z2+c(2,2,3,n)*x2y2z3+c(2,3,0,n)*x2y3+c(2,3,1,n)*
     & x2y3z1+c(2,3,2,n)*x2y3z2+c(2,3,3,n)*x2y3z3+c(3,0,0,n)*x3+c(3,0,
     & 1,n)*x3z1+c(3,0,2,n)*x3z2+c(3,0,3,n)*x3z3+c(3,1,0,n)*x3y1+c(3,
     & 1,1,n)*x3y1z1+c(3,1,2,n)*x3y1z2+c(3,1,3,n)*x3y1z3+c(3,2,0,n)*
     & x3y2+c(3,2,1,n)*x3y2z1+c(3,2,2,n)*x3y2z2+c(3,2,3,n)*x3y2z3+c(3,
     & 3,0,n)*x3y3+c(3,3,1,n)*x3y3z1+c(3,3,2,n)*x3y3z2+c(3,3,3,n)*
     & x3y3z3)*time
! endLoops()
      else if( dx.eq.0.and.dy.eq.0.and.dz.eq.1.and.dt.eq.0  )then
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
      z2=z1*z1
c 
      y1z1=y1*z1
      y1z2=y1z1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
c 
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
      r=(c(0,0,1,n)+c(0,0,2,n)*z1*2.+c(0,0,3,n)*z2*3.+c(0,1,1,n)*y1+c(
     & 0,1,2,n)*y1z1*2.+c(0,1,3,n)*y1z2*3.+c(0,2,1,n)*y2+c(0,2,2,n)*
     & y2z1*2.+c(0,2,3,n)*y2z2*3.+c(0,3,1,n)*y3+c(0,3,2,n)*y3z1*2.+c(
     & 0,3,3,n)*y3z2*3.+c(1,0,1,n)*x1+c(1,0,2,n)*x1z1*2.+c(1,0,3,n)*
     & x1z2*3.+c(1,1,1,n)*x1y1+c(1,1,2,n)*x1y1z1*2.+c(1,1,3,n)*x1y1z2*
     & 3.+c(1,2,1,n)*x1y2+c(1,2,2,n)*x1y2z1*2.+c(1,2,3,n)*x1y2z2*3.+c(
     & 1,3,1,n)*x1y3+c(1,3,2,n)*x1y3z1*2.+c(1,3,3,n)*x1y3z2*3.+c(2,0,
     & 1,n)*x2+c(2,0,2,n)*x2z1*2.+c(2,0,3,n)*x2z2*3.+c(2,1,1,n)*x2y1+
     & c(2,1,2,n)*x2y1z1*2.+c(2,1,3,n)*x2y1z2*3.+c(2,2,1,n)*x2y2+c(2,
     & 2,2,n)*x2y2z1*2.+c(2,2,3,n)*x2y2z2*3.+c(2,3,1,n)*x2y3+c(2,3,2,
     & n)*x2y3z1*2.+c(2,3,3,n)*x2y3z2*3.+c(3,0,1,n)*x3+c(3,0,2,n)*
     & x3z1*2.+c(3,0,3,n)*x3z2*3.+c(3,1,1,n)*x3y1+c(3,1,2,n)*x3y1z1*
     & 2.+c(3,1,3,n)*x3y1z2*3.+c(3,2,1,n)*x3y2+c(3,2,2,n)*x3y2z1*2.+c(
     & 3,2,3,n)*x3y2z2*3.+c(3,3,1,n)*x3y3+c(3,3,2,n)*x3y3z1*2.+c(3,3,
     & 3,n)*x3y3z2*3.)*time
! endLoops()
      else if( dx.eq.0.and.dy.eq.0.and.dz.eq.2.and.dt.eq.0  )then
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
c 
      y1z1=y1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y3=y2*y1
      y3z1=y2z1*y1
c 
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
      r=(c(0,0,2,n)*2.+c(0,0,3,n)*z1*6.+c(0,1,2,n)*y1*2.+c(0,1,3,n)*
     & y1z1*6.+c(0,2,2,n)*y2*2.+c(0,2,3,n)*y2z1*6.+c(0,3,2,n)*y3*2.+c(
     & 0,3,3,n)*y3z1*6.+c(1,0,2,n)*x1*2.+c(1,0,3,n)*x1z1*6.+c(1,1,2,n)
     & *x1y1*2.+c(1,1,3,n)*x1y1z1*6.+c(1,2,2,n)*x1y2*2.+c(1,2,3,n)*
     & x1y2z1*6.+c(1,3,2,n)*x1y3*2.+c(1,3,3,n)*x1y3z1*6.+c(2,0,2,n)*
     & x2*2.+c(2,0,3,n)*x2z1*6.+c(2,1,2,n)*x2y1*2.+c(2,1,3,n)*x2y1z1*
     & 6.+c(2,2,2,n)*x2y2*2.+c(2,2,3,n)*x2y2z1*6.+c(2,3,2,n)*x2y3*2.+
     & c(2,3,3,n)*x2y3z1*6.+c(3,0,2,n)*x3*2.+c(3,0,3,n)*x3z1*6.+c(3,1,
     & 2,n)*x3y1*2.+c(3,1,3,n)*x3y1z1*6.+c(3,2,2,n)*x3y2*2.+c(3,2,3,n)
     & *x3y2z1*6.+c(3,3,2,n)*x3y3*2.+c(3,3,3,n)*x3y3z1*6.)*time
! endLoops()
      else if( dx.eq.0.and.dy.eq.1.and.dz.eq.0.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
c 
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
c 
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
      r=(c(0,1,0,n)+c(0,1,1,n)*z1+c(0,1,2,n)*z2+c(0,1,3,n)*z3+c(0,2,0,
     & n)*y1*2.+c(0,2,1,n)*y1z1*2.+c(0,2,2,n)*y1z2*2.+c(0,2,3,n)*y1z3*
     & 2.+c(0,3,0,n)*y2*3.+c(0,3,1,n)*y2z1*3.+c(0,3,2,n)*y2z2*3.+c(0,
     & 3,3,n)*y2z3*3.+c(1,1,0,n)*x1+c(1,1,1,n)*x1z1+c(1,1,2,n)*x1z2+c(
     & 1,1,3,n)*x1z3+c(1,2,0,n)*x1y1*2.+c(1,2,1,n)*x1y1z1*2.+c(1,2,2,
     & n)*x1y1z2*2.+c(1,2,3,n)*x1y1z3*2.+c(1,3,0,n)*x1y2*3.+c(1,3,1,n)
     & *x1y2z1*3.+c(1,3,2,n)*x1y2z2*3.+c(1,3,3,n)*x1y2z3*3.+c(2,1,0,n)
     & *x2+c(2,1,1,n)*x2z1+c(2,1,2,n)*x2z2+c(2,1,3,n)*x2z3+c(2,2,0,n)*
     & x2y1*2.+c(2,2,1,n)*x2y1z1*2.+c(2,2,2,n)*x2y1z2*2.+c(2,2,3,n)*
     & x2y1z3*2.+c(2,3,0,n)*x2y2*3.+c(2,3,1,n)*x2y2z1*3.+c(2,3,2,n)*
     & x2y2z2*3.+c(2,3,3,n)*x2y2z3*3.+c(3,1,0,n)*x3+c(3,1,1,n)*x3z1+c(
     & 3,1,2,n)*x3z2+c(3,1,3,n)*x3z3+c(3,2,0,n)*x3y1*2.+c(3,2,1,n)*
     & x3y1z1*2.+c(3,2,2,n)*x3y1z2*2.+c(3,2,3,n)*x3y1z3*2.+c(3,3,0,n)*
     & x3y2*3.+c(3,3,1,n)*x3y2z1*3.+c(3,3,2,n)*x3y2z2*3.+c(3,3,3,n)*
     & x3y2z3*3.)*time
! endLoops()
      else if( dx.eq.0.and.dy.eq.1.and.dz.eq.1.and.dt.eq.0  )then
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
      z2=z1*z1
c 
      y1z1=y1*z1
      y1z2=y1z1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
c 
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
      r=(c(0,1,1,n)+c(0,1,2,n)*z1*2.+c(0,1,3,n)*z2*3.+c(0,2,1,n)*y1*2.+
     & c(0,2,2,n)*y1z1*4.+c(0,2,3,n)*y1z2*6.+c(0,3,1,n)*y2*3.+c(0,3,2,
     & n)*y2z1*6.+c(0,3,3,n)*y2z2*9.+c(1,1,1,n)*x1+c(1,1,2,n)*x1z1*2.+
     & c(1,1,3,n)*x1z2*3.+c(1,2,1,n)*x1y1*2.+c(1,2,2,n)*x1y1z1*4.+c(1,
     & 2,3,n)*x1y1z2*6.+c(1,3,1,n)*x1y2*3.+c(1,3,2,n)*x1y2z1*6.+c(1,3,
     & 3,n)*x1y2z2*9.+c(2,1,1,n)*x2+c(2,1,2,n)*x2z1*2.+c(2,1,3,n)*
     & x2z2*3.+c(2,2,1,n)*x2y1*2.+c(2,2,2,n)*x2y1z1*4.+c(2,2,3,n)*
     & x2y1z2*6.+c(2,3,1,n)*x2y2*3.+c(2,3,2,n)*x2y2z1*6.+c(2,3,3,n)*
     & x2y2z2*9.+c(3,1,1,n)*x3+c(3,1,2,n)*x3z1*2.+c(3,1,3,n)*x3z2*3.+
     & c(3,2,1,n)*x3y1*2.+c(3,2,2,n)*x3y1z1*4.+c(3,2,3,n)*x3y1z2*6.+c(
     & 3,3,1,n)*x3y2*3.+c(3,3,2,n)*x3y2z1*6.+c(3,3,3,n)*x3y2z2*9.)*
     & time
! endLoops()
      else if( dx.eq.0.and.dy.eq.1.and.dz.eq.2.and.dt.eq.0  )then
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
c 
      y1z1=y1*z1
      y2=y1*y1
      y2z1=y1z1*y1
c 
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
      r=(c(0,1,2,n)*2.+c(0,1,3,n)*z1*6.+c(0,2,2,n)*y1*4.+c(0,2,3,n)*
     & y1z1*12.+c(0,3,2,n)*y2*6.+c(0,3,3,n)*y2z1*18.+c(1,1,2,n)*x1*2.+
     & c(1,1,3,n)*x1z1*6.+c(1,2,2,n)*x1y1*4.+c(1,2,3,n)*x1y1z1*12.+c(
     & 1,3,2,n)*x1y2*6.+c(1,3,3,n)*x1y2z1*18.+c(2,1,2,n)*x2*2.+c(2,1,
     & 3,n)*x2z1*6.+c(2,2,2,n)*x2y1*4.+c(2,2,3,n)*x2y1z1*12.+c(2,3,2,
     & n)*x2y2*6.+c(2,3,3,n)*x2y2z1*18.+c(3,1,2,n)*x3*2.+c(3,1,3,n)*
     & x3z1*6.+c(3,2,2,n)*x3y1*4.+c(3,2,3,n)*x3y1z1*12.+c(3,3,2,n)*
     & x3y2*6.+c(3,3,3,n)*x3y2z1*18.)*time
! endLoops()
      else if( dx.eq.0.and.dy.eq.2.and.dz.eq.0.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
c 
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
c 
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
      r=(c(0,2,0,n)*2.+c(0,2,1,n)*z1*2.+c(0,2,2,n)*z2*2.+c(0,2,3,n)*z3*
     & 2.+c(0,3,0,n)*y1*6.+c(0,3,1,n)*y1z1*6.+c(0,3,2,n)*y1z2*6.+c(0,
     & 3,3,n)*y1z3*6.+c(1,2,0,n)*x1*2.+c(1,2,1,n)*x1z1*2.+c(1,2,2,n)*
     & x1z2*2.+c(1,2,3,n)*x1z3*2.+c(1,3,0,n)*x1y1*6.+c(1,3,1,n)*
     & x1y1z1*6.+c(1,3,2,n)*x1y1z2*6.+c(1,3,3,n)*x1y1z3*6.+c(2,2,0,n)*
     & x2*2.+c(2,2,1,n)*x2z1*2.+c(2,2,2,n)*x2z2*2.+c(2,2,3,n)*x2z3*2.+
     & c(2,3,0,n)*x2y1*6.+c(2,3,1,n)*x2y1z1*6.+c(2,3,2,n)*x2y1z2*6.+c(
     & 2,3,3,n)*x2y1z3*6.+c(3,2,0,n)*x3*2.+c(3,2,1,n)*x3z1*2.+c(3,2,2,
     & n)*x3z2*2.+c(3,2,3,n)*x3z3*2.+c(3,3,0,n)*x3y1*6.+c(3,3,1,n)*
     & x3y1z1*6.+c(3,3,2,n)*x3y1z2*6.+c(3,3,3,n)*x3y1z3*6.)*time
! endLoops()
      else if( dx.eq.0.and.dy.eq.2.and.dz.eq.1.and.dt.eq.0  )then
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
      z2=z1*z1
c 
      y1z1=y1*z1
      y1z2=y1z1*z1
c 
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
      r=(c(0,2,1,n)*2.+c(0,2,2,n)*z1*4.+c(0,2,3,n)*z2*6.+c(0,3,1,n)*y1*
     & 6.+c(0,3,2,n)*y1z1*12.+c(0,3,3,n)*y1z2*18.+c(1,2,1,n)*x1*2.+c(
     & 1,2,2,n)*x1z1*4.+c(1,2,3,n)*x1z2*6.+c(1,3,1,n)*x1y1*6.+c(1,3,2,
     & n)*x1y1z1*12.+c(1,3,3,n)*x1y1z2*18.+c(2,2,1,n)*x2*2.+c(2,2,2,n)
     & *x2z1*4.+c(2,2,3,n)*x2z2*6.+c(2,3,1,n)*x2y1*6.+c(2,3,2,n)*
     & x2y1z1*12.+c(2,3,3,n)*x2y1z2*18.+c(3,2,1,n)*x3*2.+c(3,2,2,n)*
     & x3z1*4.+c(3,2,3,n)*x3z2*6.+c(3,3,1,n)*x3y1*6.+c(3,3,2,n)*
     & x3y1z1*12.+c(3,3,3,n)*x3y1z2*18.)*time
! endLoops()
      else if( dx.eq.0.and.dy.eq.2.and.dz.eq.2.and.dt.eq.0  )then
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
c 
      y1z1=y1*z1
c 
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
      r=(c(0,2,2,n)*4.+c(0,2,3,n)*z1*12.+c(0,3,2,n)*y1*12.+c(0,3,3,n)*
     & y1z1*36.+c(1,2,2,n)*x1*4.+c(1,2,3,n)*x1z1*12.+c(1,3,2,n)*x1y1*
     & 12.+c(1,3,3,n)*x1y1z1*36.+c(2,2,2,n)*x2*4.+c(2,2,3,n)*x2z1*12.+
     & c(2,3,2,n)*x2y1*12.+c(2,3,3,n)*x2y1z1*36.+c(3,2,2,n)*x3*4.+c(3,
     & 2,3,n)*x3z1*12.+c(3,3,2,n)*x3y1*12.+c(3,3,3,n)*x3y1z1*36.)*time
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
      z2=z1*z1
      z3=z2*z1
c 
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
c 
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
      r=(c(1,0,0,n)+c(1,0,1,n)*z1+c(1,0,2,n)*z2+c(1,0,3,n)*z3+c(1,1,0,
     & n)*y1+c(1,1,1,n)*y1z1+c(1,1,2,n)*y1z2+c(1,1,3,n)*y1z3+c(1,2,0,
     & n)*y2+c(1,2,1,n)*y2z1+c(1,2,2,n)*y2z2+c(1,2,3,n)*y2z3+c(1,3,0,
     & n)*y3+c(1,3,1,n)*y3z1+c(1,3,2,n)*y3z2+c(1,3,3,n)*y3z3+c(2,0,0,
     & n)*x1*2.+c(2,0,1,n)*x1z1*2.+c(2,0,2,n)*x1z2*2.+c(2,0,3,n)*x1z3*
     & 2.+c(2,1,0,n)*x1y1*2.+c(2,1,1,n)*x1y1z1*2.+c(2,1,2,n)*x1y1z2*
     & 2.+c(2,1,3,n)*x1y1z3*2.+c(2,2,0,n)*x1y2*2.+c(2,2,1,n)*x1y2z1*
     & 2.+c(2,2,2,n)*x1y2z2*2.+c(2,2,3,n)*x1y2z3*2.+c(2,3,0,n)*x1y3*
     & 2.+c(2,3,1,n)*x1y3z1*2.+c(2,3,2,n)*x1y3z2*2.+c(2,3,3,n)*x1y3z3*
     & 2.+c(3,0,0,n)*x2*3.+c(3,0,1,n)*x2z1*3.+c(3,0,2,n)*x2z2*3.+c(3,
     & 0,3,n)*x2z3*3.+c(3,1,0,n)*x2y1*3.+c(3,1,1,n)*x2y1z1*3.+c(3,1,2,
     & n)*x2y1z2*3.+c(3,1,3,n)*x2y1z3*3.+c(3,2,0,n)*x2y2*3.+c(3,2,1,n)
     & *x2y2z1*3.+c(3,2,2,n)*x2y2z2*3.+c(3,2,3,n)*x2y2z3*3.+c(3,3,0,n)
     & *x2y3*3.+c(3,3,1,n)*x2y3z1*3.+c(3,3,2,n)*x2y3z2*3.+c(3,3,3,n)*
     & x2y3z3*3.)*time
! endLoops()
      else if( dx.eq.1.and.dy.eq.0.and.dz.eq.1.and.dt.eq.0  )then
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
      z2=z1*z1
c 
      y1z1=y1*z1
      y1z2=y1z1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
c 
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
      r=(c(1,0,1,n)+c(1,0,2,n)*z1*2.+c(1,0,3,n)*z2*3.+c(1,1,1,n)*y1+c(
     & 1,1,2,n)*y1z1*2.+c(1,1,3,n)*y1z2*3.+c(1,2,1,n)*y2+c(1,2,2,n)*
     & y2z1*2.+c(1,2,3,n)*y2z2*3.+c(1,3,1,n)*y3+c(1,3,2,n)*y3z1*2.+c(
     & 1,3,3,n)*y3z2*3.+c(2,0,1,n)*x1*2.+c(2,0,2,n)*x1z1*4.+c(2,0,3,n)
     & *x1z2*6.+c(2,1,1,n)*x1y1*2.+c(2,1,2,n)*x1y1z1*4.+c(2,1,3,n)*
     & x1y1z2*6.+c(2,2,1,n)*x1y2*2.+c(2,2,2,n)*x1y2z1*4.+c(2,2,3,n)*
     & x1y2z2*6.+c(2,3,1,n)*x1y3*2.+c(2,3,2,n)*x1y3z1*4.+c(2,3,3,n)*
     & x1y3z2*6.+c(3,0,1,n)*x2*3.+c(3,0,2,n)*x2z1*6.+c(3,0,3,n)*x2z2*
     & 9.+c(3,1,1,n)*x2y1*3.+c(3,1,2,n)*x2y1z1*6.+c(3,1,3,n)*x2y1z2*
     & 9.+c(3,2,1,n)*x2y2*3.+c(3,2,2,n)*x2y2z1*6.+c(3,2,3,n)*x2y2z2*
     & 9.+c(3,3,1,n)*x2y3*3.+c(3,3,2,n)*x2y3z1*6.+c(3,3,3,n)*x2y3z2*
     & 9.)*time
! endLoops()
      else if( dx.eq.1.and.dy.eq.0.and.dz.eq.2.and.dt.eq.0  )then
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
c 
      y1z1=y1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y3=y2*y1
      y3z1=y2z1*y1
c 
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
      r=(c(1,0,2,n)*2.+c(1,0,3,n)*z1*6.+c(1,1,2,n)*y1*2.+c(1,1,3,n)*
     & y1z1*6.+c(1,2,2,n)*y2*2.+c(1,2,3,n)*y2z1*6.+c(1,3,2,n)*y3*2.+c(
     & 1,3,3,n)*y3z1*6.+c(2,0,2,n)*x1*4.+c(2,0,3,n)*x1z1*12.+c(2,1,2,
     & n)*x1y1*4.+c(2,1,3,n)*x1y1z1*12.+c(2,2,2,n)*x1y2*4.+c(2,2,3,n)*
     & x1y2z1*12.+c(2,3,2,n)*x1y3*4.+c(2,3,3,n)*x1y3z1*12.+c(3,0,2,n)*
     & x2*6.+c(3,0,3,n)*x2z1*18.+c(3,1,2,n)*x2y1*6.+c(3,1,3,n)*x2y1z1*
     & 18.+c(3,2,2,n)*x2y2*6.+c(3,2,3,n)*x2y2z1*18.+c(3,3,2,n)*x2y3*
     & 6.+c(3,3,3,n)*x2y3z1*18.)*time
! endLoops()
      else if( dx.eq.1.and.dy.eq.1.and.dz.eq.0.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
c 
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
c 
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
      r=(c(1,1,0,n)+c(1,1,1,n)*z1+c(1,1,2,n)*z2+c(1,1,3,n)*z3+c(1,2,0,
     & n)*y1*2.+c(1,2,1,n)*y1z1*2.+c(1,2,2,n)*y1z2*2.+c(1,2,3,n)*y1z3*
     & 2.+c(1,3,0,n)*y2*3.+c(1,3,1,n)*y2z1*3.+c(1,3,2,n)*y2z2*3.+c(1,
     & 3,3,n)*y2z3*3.+c(2,1,0,n)*x1*2.+c(2,1,1,n)*x1z1*2.+c(2,1,2,n)*
     & x1z2*2.+c(2,1,3,n)*x1z3*2.+c(2,2,0,n)*x1y1*4.+c(2,2,1,n)*
     & x1y1z1*4.+c(2,2,2,n)*x1y1z2*4.+c(2,2,3,n)*x1y1z3*4.+c(2,3,0,n)*
     & x1y2*6.+c(2,3,1,n)*x1y2z1*6.+c(2,3,2,n)*x1y2z2*6.+c(2,3,3,n)*
     & x1y2z3*6.+c(3,1,0,n)*x2*3.+c(3,1,1,n)*x2z1*3.+c(3,1,2,n)*x2z2*
     & 3.+c(3,1,3,n)*x2z3*3.+c(3,2,0,n)*x2y1*6.+c(3,2,1,n)*x2y1z1*6.+
     & c(3,2,2,n)*x2y1z2*6.+c(3,2,3,n)*x2y1z3*6.+c(3,3,0,n)*x2y2*9.+c(
     & 3,3,1,n)*x2y2z1*9.+c(3,3,2,n)*x2y2z2*9.+c(3,3,3,n)*x2y2z3*9.)*
     & time
! endLoops()
      else if( dx.eq.1.and.dy.eq.1.and.dz.eq.1.and.dt.eq.0  )then
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
      z2=z1*z1
c 
      y1z1=y1*z1
      y1z2=y1z1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
c 
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
      r=(c(1,1,1,n)+c(1,1,2,n)*z1*2.+c(1,1,3,n)*z2*3.+c(1,2,1,n)*y1*2.+
     & c(1,2,2,n)*y1z1*4.+c(1,2,3,n)*y1z2*6.+c(1,3,1,n)*y2*3.+c(1,3,2,
     & n)*y2z1*6.+c(1,3,3,n)*y2z2*9.+c(2,1,1,n)*x1*2.+c(2,1,2,n)*x1z1*
     & 4.+c(2,1,3,n)*x1z2*6.+c(2,2,1,n)*x1y1*4.+c(2,2,2,n)*x1y1z1*8.+
     & c(2,2,3,n)*x1y1z2*12.+c(2,3,1,n)*x1y2*6.+c(2,3,2,n)*x1y2z1*12.+
     & c(2,3,3,n)*x1y2z2*18.+c(3,1,1,n)*x2*3.+c(3,1,2,n)*x2z1*6.+c(3,
     & 1,3,n)*x2z2*9.+c(3,2,1,n)*x2y1*6.+c(3,2,2,n)*x2y1z1*12.+c(3,2,
     & 3,n)*x2y1z2*18.+c(3,3,1,n)*x2y2*9.+c(3,3,2,n)*x2y2z1*18.+c(3,3,
     & 3,n)*x2y2z2*27.)*time
! endLoops()
      else if( dx.eq.1.and.dy.eq.1.and.dz.eq.2.and.dt.eq.0  )then
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
c 
      y1z1=y1*z1
      y2=y1*y1
      y2z1=y1z1*y1
c 
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
      r=(c(1,1,2,n)*2.+c(1,1,3,n)*z1*6.+c(1,2,2,n)*y1*4.+c(1,2,3,n)*
     & y1z1*12.+c(1,3,2,n)*y2*6.+c(1,3,3,n)*y2z1*18.+c(2,1,2,n)*x1*4.+
     & c(2,1,3,n)*x1z1*12.+c(2,2,2,n)*x1y1*8.+c(2,2,3,n)*x1y1z1*24.+c(
     & 2,3,2,n)*x1y2*12.+c(2,3,3,n)*x1y2z1*36.+c(3,1,2,n)*x2*6.+c(3,1,
     & 3,n)*x2z1*18.+c(3,2,2,n)*x2y1*12.+c(3,2,3,n)*x2y1z1*36.+c(3,3,
     & 2,n)*x2y2*18.+c(3,3,3,n)*x2y2z1*54.)*time
! endLoops()
      else if( dx.eq.1.and.dy.eq.2.and.dz.eq.0.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
c 
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
c 
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
      r=(c(1,2,0,n)*2.+c(1,2,1,n)*z1*2.+c(1,2,2,n)*z2*2.+c(1,2,3,n)*z3*
     & 2.+c(1,3,0,n)*y1*6.+c(1,3,1,n)*y1z1*6.+c(1,3,2,n)*y1z2*6.+c(1,
     & 3,3,n)*y1z3*6.+c(2,2,0,n)*x1*4.+c(2,2,1,n)*x1z1*4.+c(2,2,2,n)*
     & x1z2*4.+c(2,2,3,n)*x1z3*4.+c(2,3,0,n)*x1y1*12.+c(2,3,1,n)*
     & x1y1z1*12.+c(2,3,2,n)*x1y1z2*12.+c(2,3,3,n)*x1y1z3*12.+c(3,2,0,
     & n)*x2*6.+c(3,2,1,n)*x2z1*6.+c(3,2,2,n)*x2z2*6.+c(3,2,3,n)*x2z3*
     & 6.+c(3,3,0,n)*x2y1*18.+c(3,3,1,n)*x2y1z1*18.+c(3,3,2,n)*x2y1z2*
     & 18.+c(3,3,3,n)*x2y1z3*18.)*time
! endLoops()
      else if( dx.eq.1.and.dy.eq.2.and.dz.eq.1.and.dt.eq.0  )then
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
      z2=z1*z1
c 
      y1z1=y1*z1
      y1z2=y1z1*z1
c 
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
      r=(c(1,2,1,n)*2.+c(1,2,2,n)*z1*4.+c(1,2,3,n)*z2*6.+c(1,3,1,n)*y1*
     & 6.+c(1,3,2,n)*y1z1*12.+c(1,3,3,n)*y1z2*18.+c(2,2,1,n)*x1*4.+c(
     & 2,2,2,n)*x1z1*8.+c(2,2,3,n)*x1z2*12.+c(2,3,1,n)*x1y1*12.+c(2,3,
     & 2,n)*x1y1z1*24.+c(2,3,3,n)*x1y1z2*36.+c(3,2,1,n)*x2*6.+c(3,2,2,
     & n)*x2z1*12.+c(3,2,3,n)*x2z2*18.+c(3,3,1,n)*x2y1*18.+c(3,3,2,n)*
     & x2y1z1*36.+c(3,3,3,n)*x2y1z2*54.)*time
! endLoops()
      else if( dx.eq.1.and.dy.eq.2.and.dz.eq.2.and.dt.eq.0  )then
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
c 
      y1z1=y1*z1
c 
      x1z1=x1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x2=x1*x1
      x2z1=x1z1*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      r=(c(1,2,2,n)*4.+c(1,2,3,n)*z1*12.+c(1,3,2,n)*y1*12.+c(1,3,3,n)*
     & y1z1*36.+c(2,2,2,n)*x1*8.+c(2,2,3,n)*x1z1*24.+c(2,3,2,n)*x1y1*
     & 24.+c(2,3,3,n)*x1y1z1*72.+c(3,2,2,n)*x2*12.+c(3,2,3,n)*x2z1*
     & 36.+c(3,3,2,n)*x2y1*36.+c(3,3,3,n)*x2y1z1*108.)*time
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
      z2=z1*z1
      z3=z2*z1
c 
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
c 
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
      r=(c(2,0,0,n)*2.+c(2,0,1,n)*z1*2.+c(2,0,2,n)*z2*2.+c(2,0,3,n)*z3*
     & 2.+c(2,1,0,n)*y1*2.+c(2,1,1,n)*y1z1*2.+c(2,1,2,n)*y1z2*2.+c(2,
     & 1,3,n)*y1z3*2.+c(2,2,0,n)*y2*2.+c(2,2,1,n)*y2z1*2.+c(2,2,2,n)*
     & y2z2*2.+c(2,2,3,n)*y2z3*2.+c(2,3,0,n)*y3*2.+c(2,3,1,n)*y3z1*2.+
     & c(2,3,2,n)*y3z2*2.+c(2,3,3,n)*y3z3*2.+c(3,0,0,n)*x1*6.+c(3,0,1,
     & n)*x1z1*6.+c(3,0,2,n)*x1z2*6.+c(3,0,3,n)*x1z3*6.+c(3,1,0,n)*
     & x1y1*6.+c(3,1,1,n)*x1y1z1*6.+c(3,1,2,n)*x1y1z2*6.+c(3,1,3,n)*
     & x1y1z3*6.+c(3,2,0,n)*x1y2*6.+c(3,2,1,n)*x1y2z1*6.+c(3,2,2,n)*
     & x1y2z2*6.+c(3,2,3,n)*x1y2z3*6.+c(3,3,0,n)*x1y3*6.+c(3,3,1,n)*
     & x1y3z1*6.+c(3,3,2,n)*x1y3z2*6.+c(3,3,3,n)*x1y3z3*6.)*time
! endLoops()
      else if( dx.eq.2.and.dy.eq.0.and.dz.eq.1.and.dt.eq.0  )then
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
      z2=z1*z1
c 
      y1z1=y1*z1
      y1z2=y1z1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
c 
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
      r=(c(2,0,1,n)*2.+c(2,0,2,n)*z1*4.+c(2,0,3,n)*z2*6.+c(2,1,1,n)*y1*
     & 2.+c(2,1,2,n)*y1z1*4.+c(2,1,3,n)*y1z2*6.+c(2,2,1,n)*y2*2.+c(2,
     & 2,2,n)*y2z1*4.+c(2,2,3,n)*y2z2*6.+c(2,3,1,n)*y3*2.+c(2,3,2,n)*
     & y3z1*4.+c(2,3,3,n)*y3z2*6.+c(3,0,1,n)*x1*6.+c(3,0,2,n)*x1z1*
     & 12.+c(3,0,3,n)*x1z2*18.+c(3,1,1,n)*x1y1*6.+c(3,1,2,n)*x1y1z1*
     & 12.+c(3,1,3,n)*x1y1z2*18.+c(3,2,1,n)*x1y2*6.+c(3,2,2,n)*x1y2z1*
     & 12.+c(3,2,3,n)*x1y2z2*18.+c(3,3,1,n)*x1y3*6.+c(3,3,2,n)*x1y3z1*
     & 12.+c(3,3,3,n)*x1y3z2*18.)*time
! endLoops()
      else if( dx.eq.2.and.dy.eq.0.and.dz.eq.2.and.dt.eq.0  )then
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
c 
      y1z1=y1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y3=y2*y1
      y3z1=y2z1*y1
c 
      x1z1=x1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      r=(c(2,0,2,n)*4.+c(2,0,3,n)*z1*12.+c(2,1,2,n)*y1*4.+c(2,1,3,n)*
     & y1z1*12.+c(2,2,2,n)*y2*4.+c(2,2,3,n)*y2z1*12.+c(2,3,2,n)*y3*4.+
     & c(2,3,3,n)*y3z1*12.+c(3,0,2,n)*x1*12.+c(3,0,3,n)*x1z1*36.+c(3,
     & 1,2,n)*x1y1*12.+c(3,1,3,n)*x1y1z1*36.+c(3,2,2,n)*x1y2*12.+c(3,
     & 2,3,n)*x1y2z1*36.+c(3,3,2,n)*x1y3*12.+c(3,3,3,n)*x1y3z1*36.)*
     & time
! endLoops()
      else if( dx.eq.2.and.dy.eq.1.and.dz.eq.0.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
c 
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
c 
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
      r=(c(2,1,0,n)*2.+c(2,1,1,n)*z1*2.+c(2,1,2,n)*z2*2.+c(2,1,3,n)*z3*
     & 2.+c(2,2,0,n)*y1*4.+c(2,2,1,n)*y1z1*4.+c(2,2,2,n)*y1z2*4.+c(2,
     & 2,3,n)*y1z3*4.+c(2,3,0,n)*y2*6.+c(2,3,1,n)*y2z1*6.+c(2,3,2,n)*
     & y2z2*6.+c(2,3,3,n)*y2z3*6.+c(3,1,0,n)*x1*6.+c(3,1,1,n)*x1z1*6.+
     & c(3,1,2,n)*x1z2*6.+c(3,1,3,n)*x1z3*6.+c(3,2,0,n)*x1y1*12.+c(3,
     & 2,1,n)*x1y1z1*12.+c(3,2,2,n)*x1y1z2*12.+c(3,2,3,n)*x1y1z3*12.+
     & c(3,3,0,n)*x1y2*18.+c(3,3,1,n)*x1y2z1*18.+c(3,3,2,n)*x1y2z2*
     & 18.+c(3,3,3,n)*x1y2z3*18.)*time
! endLoops()
      else if( dx.eq.2.and.dy.eq.1.and.dz.eq.1.and.dt.eq.0  )then
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
      z2=z1*z1
c 
      y1z1=y1*z1
      y1z2=y1z1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
c 
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      r=(c(2,1,1,n)*2.+c(2,1,2,n)*z1*4.+c(2,1,3,n)*z2*6.+c(2,2,1,n)*y1*
     & 4.+c(2,2,2,n)*y1z1*8.+c(2,2,3,n)*y1z2*12.+c(2,3,1,n)*y2*6.+c(2,
     & 3,2,n)*y2z1*12.+c(2,3,3,n)*y2z2*18.+c(3,1,1,n)*x1*6.+c(3,1,2,n)
     & *x1z1*12.+c(3,1,3,n)*x1z2*18.+c(3,2,1,n)*x1y1*12.+c(3,2,2,n)*
     & x1y1z1*24.+c(3,2,3,n)*x1y1z2*36.+c(3,3,1,n)*x1y2*18.+c(3,3,2,n)
     & *x1y2z1*36.+c(3,3,3,n)*x1y2z2*54.)*time
! endLoops()
      else if( dx.eq.2.and.dy.eq.1.and.dz.eq.2.and.dt.eq.0  )then
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
c 
      y1z1=y1*z1
      y2=y1*y1
      y2z1=y1z1*y1
c 
      x1z1=x1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      r=(c(2,1,2,n)*4.+c(2,1,3,n)*z1*12.+c(2,2,2,n)*y1*8.+c(2,2,3,n)*
     & y1z1*24.+c(2,3,2,n)*y2*12.+c(2,3,3,n)*y2z1*36.+c(3,1,2,n)*x1*
     & 12.+c(3,1,3,n)*x1z1*36.+c(3,2,2,n)*x1y1*24.+c(3,2,3,n)*x1y1z1*
     & 72.+c(3,3,2,n)*x1y2*36.+c(3,3,3,n)*x1y2z1*108.)*time
! endLoops()
      else if( dx.eq.2.and.dy.eq.2.and.dz.eq.0.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
c 
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
c 
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      r=(c(2,2,0,n)*4.+c(2,2,1,n)*z1*4.+c(2,2,2,n)*z2*4.+c(2,2,3,n)*z3*
     & 4.+c(2,3,0,n)*y1*12.+c(2,3,1,n)*y1z1*12.+c(2,3,2,n)*y1z2*12.+c(
     & 2,3,3,n)*y1z3*12.+c(3,2,0,n)*x1*12.+c(3,2,1,n)*x1z1*12.+c(3,2,
     & 2,n)*x1z2*12.+c(3,2,3,n)*x1z3*12.+c(3,3,0,n)*x1y1*36.+c(3,3,1,
     & n)*x1y1z1*36.+c(3,3,2,n)*x1y1z2*36.+c(3,3,3,n)*x1y1z3*36.)*time
! endLoops()
      else if( dx.eq.2.and.dy.eq.2.and.dz.eq.1.and.dt.eq.0  )then
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
      z2=z1*z1
c 
      y1z1=y1*z1
      y1z2=y1z1*z1
c 
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      r=(c(2,2,1,n)*4.+c(2,2,2,n)*z1*8.+c(2,2,3,n)*z2*12.+c(2,3,1,n)*
     & y1*12.+c(2,3,2,n)*y1z1*24.+c(2,3,3,n)*y1z2*36.+c(3,2,1,n)*x1*
     & 12.+c(3,2,2,n)*x1z1*24.+c(3,2,3,n)*x1z2*36.+c(3,3,1,n)*x1y1*
     & 36.+c(3,3,2,n)*x1y1z1*72.+c(3,3,3,n)*x1y1z2*108.)*time
! endLoops()
      else if( dx.eq.2.and.dy.eq.2.and.dz.eq.2.and.dt.eq.0  )then
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
c 
      y1z1=y1*z1
c 
      x1z1=x1*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      r=(c(2,2,2,n)*8.+c(2,2,3,n)*z1*24.+c(2,3,2,n)*y1*24.+c(2,3,3,n)*
     & y1z1*72.+c(3,2,2,n)*x1*24.+c(3,2,3,n)*x1z1*72.+c(3,3,2,n)*x1y1*
     & 72.+c(3,3,3,n)*x1y1z1*216.)*time
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
      z2=z1*z1
      z3=z2*z1
c 
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
      r=(c(3,0,0,n)*6.+c(3,0,1,n)*z1*6.+c(3,0,2,n)*z2*6.+c(3,0,3,n)*z3*
     & 6.+c(3,1,0,n)*y1*6.+c(3,1,1,n)*y1z1*6.+c(3,1,2,n)*y1z2*6.+c(3,
     & 1,3,n)*y1z3*6.+c(3,2,0,n)*y2*6.+c(3,2,1,n)*y2z1*6.+c(3,2,2,n)*
     & y2z2*6.+c(3,2,3,n)*y2z3*6.+c(3,3,0,n)*y3*6.+c(3,3,1,n)*y3z1*6.+
     & c(3,3,2,n)*y3z2*6.+c(3,3,3,n)*y3z3*6.)*time
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
c
      y1z1=y1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y3=y2*y1
      y3z1=y2z1*y1
c
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
      z2=z1*z1
      z3=z2*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      r=(+c(0,0,2,n)*2.+c(0,0,3,n)*z1*6.+c(0,1,2,n)*y1*2.+c(0,1,3,n)*
     & y1z1*6.+c(0,2,2,n)*y2*2.+c(0,2,3,n)*y2z1*6.+c(0,3,2,n)*y3*2.+c(
     & 0,3,3,n)*y3z1*6.+c(1,0,2,n)*x1*2.+c(1,0,3,n)*x1z1*6.+c(1,1,2,n)
     & *x1y1*2.+c(1,1,3,n)*x1y1z1*6.+c(1,2,2,n)*x1y2*2.+c(1,2,3,n)*
     & x1y2z1*6.+c(1,3,2,n)*x1y3*2.+c(1,3,3,n)*x1y3z1*6.+c(2,0,2,n)*
     & x2*2.+c(2,0,3,n)*x2z1*6.+c(2,1,2,n)*x2y1*2.+c(2,1,3,n)*x2y1z1*
     & 6.+c(2,2,2,n)*x2y2*2.+c(2,2,3,n)*x2y2z1*6.+c(2,3,2,n)*x2y3*2.+
     & c(2,3,3,n)*x2y3z1*6.+c(3,0,2,n)*x3*2.+c(3,0,3,n)*x3z1*6.+c(3,1,
     & 2,n)*x3y1*2.+c(3,1,3,n)*x3y1z1*6.+c(3,2,2,n)*x3y2*2.+c(3,2,3,n)
     & *x3y2z1*6.+c(3,3,2,n)*x3y3*2.+c(3,3,3,n)*x3y3z1*6.+c(0,2,0,n)*
     & 2.+c(0,2,1,n)*z1*2.+c(0,2,2,n)*z2*2.+c(0,2,3,n)*z3*2.+c(0,3,0,
     & n)*y1*6.+c(0,3,1,n)*y1z1*6.+c(0,3,2,n)*y1z2*6.+c(0,3,3,n)*y1z3*
     & 6.+c(1,2,0,n)*x1*2.+c(1,2,1,n)*x1z1*2.+c(1,2,2,n)*x1z2*2.+c(1,
     & 2,3,n)*x1z3*2.+c(1,3,0,n)*x1y1*6.+c(1,3,1,n)*x1y1z1*6.+c(1,3,2,
     & n)*x1y1z2*6.+c(1,3,3,n)*x1y1z3*6.+c(2,2,0,n)*x2*2.+c(2,2,1,n)*
     & x2z1*2.+c(2,2,2,n)*x2z2*2.+c(2,2,3,n)*x2z3*2.+c(2,3,0,n)*x2y1*
     & 6.+c(2,3,1,n)*x2y1z1*6.+c(2,3,2,n)*x2y1z2*6.+c(2,3,3,n)*x2y1z3*
     & 6.+c(3,2,0,n)*x3*2.+c(3,2,1,n)*x3z1*2.+c(3,2,2,n)*x3z2*2.+c(3,
     & 2,3,n)*x3z3*2.+c(3,3,0,n)*x3y1*6.+c(3,3,1,n)*x3y1z1*6.+c(3,3,2,
     & n)*x3y1z2*6.+c(3,3,3,n)*x3y1z3*6.+c(2,0,0,n)*2.+c(2,0,1,n)*z1*
     & 2.+c(2,0,2,n)*z2*2.+c(2,0,3,n)*z3*2.+c(2,1,0,n)*y1*2.+c(2,1,1,
     & n)*y1z1*2.+c(2,1,2,n)*y1z2*2.+c(2,1,3,n)*y1z3*2.+c(2,2,0,n)*y2*
     & 2.+c(2,2,1,n)*y2z1*2.+c(2,2,2,n)*y2z2*2.+c(2,2,3,n)*y2z3*2.+c(
     & 2,3,0,n)*y3*2.+c(2,3,1,n)*y3z1*2.+c(2,3,2,n)*y3z2*2.+c(2,3,3,n)
     & *y3z3*2.+c(3,0,0,n)*x1*6.+c(3,0,1,n)*x1z1*6.+c(3,0,2,n)*x1z2*
     & 6.+c(3,0,3,n)*x1z3*6.+c(3,1,0,n)*x1y1*6.+c(3,1,1,n)*x1y1z1*6.+
     & c(3,1,2,n)*x1y1z2*6.+c(3,1,3,n)*x1y1z3*6.+c(3,2,0,n)*x1y2*6.+c(
     & 3,2,1,n)*x1y2z1*6.+c(3,2,2,n)*x1y2z2*6.+c(3,2,3,n)*x1y2z3*6.+c(
     & 3,3,0,n)*x1y3*6.+c(3,3,1,n)*x1y3z1*6.+c(3,3,2,n)*x1y3z2*6.+c(3,
     & 3,3,n)*x1y3z3*6.)*time
! endLoops()
      else
! beginLoops(time=0.)
      time=0.
      r=0.
! endLoops()
      end if
! #If "3D3" eq "3D5" || "3D3" eq "3D6"
      return
      end
! polyFun(3D4)
      subroutine polyEvaluate3D4 (r, x1,y1,z1,n,t, ndc1,ndc2,ndc3,nda, 
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
! #If "3D4" eq "3D5" || "3D4" eq "3D6"
! poly3D4(c,c,c,r)
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
c 
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
c 
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
      r=(c(0,0,0,n)+c(0,0,1,n)*z1+c(0,0,2,n)*z2+c(0,0,3,n)*z3+c(0,0,4,
     & n)*z4+c(0,1,0,n)*y1+c(0,1,1,n)*y1z1+c(0,1,2,n)*y1z2+c(0,1,3,n)*
     & y1z3+c(0,1,4,n)*y1z4+c(0,2,0,n)*y2+c(0,2,1,n)*y2z1+c(0,2,2,n)*
     & y2z2+c(0,2,3,n)*y2z3+c(0,2,4,n)*y2z4+c(0,3,0,n)*y3+c(0,3,1,n)*
     & y3z1+c(0,3,2,n)*y3z2+c(0,3,3,n)*y3z3+c(0,3,4,n)*y3z4+c(0,4,0,n)
     & *y4+c(0,4,1,n)*y4z1+c(0,4,2,n)*y4z2+c(0,4,3,n)*y4z3+c(0,4,4,n)*
     & y4z4+c(1,0,0,n)*x1+c(1,0,1,n)*x1z1+c(1,0,2,n)*x1z2+c(1,0,3,n)*
     & x1z3+c(1,0,4,n)*x1z4+c(1,1,0,n)*x1y1+c(1,1,1,n)*x1y1z1+c(1,1,2,
     & n)*x1y1z2+c(1,1,3,n)*x1y1z3+c(1,1,4,n)*x1y1z4+c(1,2,0,n)*x1y2+
     & c(1,2,1,n)*x1y2z1+c(1,2,2,n)*x1y2z2+c(1,2,3,n)*x1y2z3+c(1,2,4,
     & n)*x1y2z4+c(1,3,0,n)*x1y3+c(1,3,1,n)*x1y3z1+c(1,3,2,n)*x1y3z2+
     & c(1,3,3,n)*x1y3z3+c(1,3,4,n)*x1y3z4+c(1,4,0,n)*x1y4+c(1,4,1,n)*
     & x1y4z1+c(1,4,2,n)*x1y4z2+c(1,4,3,n)*x1y4z3+c(1,4,4,n)*x1y4z4+c(
     & 2,0,0,n)*x2+c(2,0,1,n)*x2z1+c(2,0,2,n)*x2z2+c(2,0,3,n)*x2z3+c(
     & 2,0,4,n)*x2z4+c(2,1,0,n)*x2y1+c(2,1,1,n)*x2y1z1+c(2,1,2,n)*
     & x2y1z2+c(2,1,3,n)*x2y1z3+c(2,1,4,n)*x2y1z4+c(2,2,0,n)*x2y2+c(2,
     & 2,1,n)*x2y2z1+c(2,2,2,n)*x2y2z2+c(2,2,3,n)*x2y2z3+c(2,2,4,n)*
     & x2y2z4+c(2,3,0,n)*x2y3+c(2,3,1,n)*x2y3z1+c(2,3,2,n)*x2y3z2+c(2,
     & 3,3,n)*x2y3z3+c(2,3,4,n)*x2y3z4+c(2,4,0,n)*x2y4+c(2,4,1,n)*
     & x2y4z1+c(2,4,2,n)*x2y4z2+c(2,4,3,n)*x2y4z3+c(2,4,4,n)*x2y4z4+c(
     & 3,0,0,n)*x3+c(3,0,1,n)*x3z1+c(3,0,2,n)*x3z2+c(3,0,3,n)*x3z3+c(
     & 3,0,4,n)*x3z4+c(3,1,0,n)*x3y1+c(3,1,1,n)*x3y1z1+c(3,1,2,n)*
     & x3y1z2+c(3,1,3,n)*x3y1z3+c(3,1,4,n)*x3y1z4+c(3,2,0,n)*x3y2+c(3,
     & 2,1,n)*x3y2z1+c(3,2,2,n)*x3y2z2+c(3,2,3,n)*x3y2z3+c(3,2,4,n)*
     & x3y2z4+c(3,3,0,n)*x3y3+c(3,3,1,n)*x3y3z1+c(3,3,2,n)*x3y3z2+c(3,
     & 3,3,n)*x3y3z3+c(3,3,4,n)*x3y3z4+c(3,4,0,n)*x3y4+c(3,4,1,n)*
     & x3y4z1+c(3,4,2,n)*x3y4z2+c(3,4,3,n)*x3y4z3+c(3,4,4,n)*x3y4z4+c(
     & 4,0,0,n)*x4+c(4,0,1,n)*x4z1+c(4,0,2,n)*x4z2+c(4,0,3,n)*x4z3+c(
     & 4,0,4,n)*x4z4+c(4,1,0,n)*x4y1+c(4,1,1,n)*x4y1z1+c(4,1,2,n)*
     & x4y1z2+c(4,1,3,n)*x4y1z3+c(4,1,4,n)*x4y1z4+c(4,2,0,n)*x4y2+c(4,
     & 2,1,n)*x4y2z1+c(4,2,2,n)*x4y2z2+c(4,2,3,n)*x4y2z3+c(4,2,4,n)*
     & x4y2z4+c(4,3,0,n)*x4y3+c(4,3,1,n)*x4y3z1+c(4,3,2,n)*x4y3z2+c(4,
     & 3,3,n)*x4y3z3+c(4,3,4,n)*x4y3z4+c(4,4,0,n)*x4y4+c(4,4,1,n)*
     & x4y4z1+c(4,4,2,n)*x4y4z2+c(4,4,3,n)*x4y4z3+c(4,4,4,n)*x4y4z4)*
     & time
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
c
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
c
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
      r=(c(0,0,0,n)+c(0,0,1,n)*z1+c(0,0,2,n)*z2+c(0,0,3,n)*z3+c(0,0,4,
     & n)*z4+c(0,1,0,n)*y1+c(0,1,1,n)*y1z1+c(0,1,2,n)*y1z2+c(0,1,3,n)*
     & y1z3+c(0,1,4,n)*y1z4+c(0,2,0,n)*y2+c(0,2,1,n)*y2z1+c(0,2,2,n)*
     & y2z2+c(0,2,3,n)*y2z3+c(0,2,4,n)*y2z4+c(0,3,0,n)*y3+c(0,3,1,n)*
     & y3z1+c(0,3,2,n)*y3z2+c(0,3,3,n)*y3z3+c(0,3,4,n)*y3z4+c(0,4,0,n)
     & *y4+c(0,4,1,n)*y4z1+c(0,4,2,n)*y4z2+c(0,4,3,n)*y4z3+c(0,4,4,n)*
     & y4z4+c(1,0,0,n)*x1+c(1,0,1,n)*x1z1+c(1,0,2,n)*x1z2+c(1,0,3,n)*
     & x1z3+c(1,0,4,n)*x1z4+c(1,1,0,n)*x1y1+c(1,1,1,n)*x1y1z1+c(1,1,2,
     & n)*x1y1z2+c(1,1,3,n)*x1y1z3+c(1,1,4,n)*x1y1z4+c(1,2,0,n)*x1y2+
     & c(1,2,1,n)*x1y2z1+c(1,2,2,n)*x1y2z2+c(1,2,3,n)*x1y2z3+c(1,2,4,
     & n)*x1y2z4+c(1,3,0,n)*x1y3+c(1,3,1,n)*x1y3z1+c(1,3,2,n)*x1y3z2+
     & c(1,3,3,n)*x1y3z3+c(1,3,4,n)*x1y3z4+c(1,4,0,n)*x1y4+c(1,4,1,n)*
     & x1y4z1+c(1,4,2,n)*x1y4z2+c(1,4,3,n)*x1y4z3+c(1,4,4,n)*x1y4z4+c(
     & 2,0,0,n)*x2+c(2,0,1,n)*x2z1+c(2,0,2,n)*x2z2+c(2,0,3,n)*x2z3+c(
     & 2,0,4,n)*x2z4+c(2,1,0,n)*x2y1+c(2,1,1,n)*x2y1z1+c(2,1,2,n)*
     & x2y1z2+c(2,1,3,n)*x2y1z3+c(2,1,4,n)*x2y1z4+c(2,2,0,n)*x2y2+c(2,
     & 2,1,n)*x2y2z1+c(2,2,2,n)*x2y2z2+c(2,2,3,n)*x2y2z3+c(2,2,4,n)*
     & x2y2z4+c(2,3,0,n)*x2y3+c(2,3,1,n)*x2y3z1+c(2,3,2,n)*x2y3z2+c(2,
     & 3,3,n)*x2y3z3+c(2,3,4,n)*x2y3z4+c(2,4,0,n)*x2y4+c(2,4,1,n)*
     & x2y4z1+c(2,4,2,n)*x2y4z2+c(2,4,3,n)*x2y4z3+c(2,4,4,n)*x2y4z4+c(
     & 3,0,0,n)*x3+c(3,0,1,n)*x3z1+c(3,0,2,n)*x3z2+c(3,0,3,n)*x3z3+c(
     & 3,0,4,n)*x3z4+c(3,1,0,n)*x3y1+c(3,1,1,n)*x3y1z1+c(3,1,2,n)*
     & x3y1z2+c(3,1,3,n)*x3y1z3+c(3,1,4,n)*x3y1z4+c(3,2,0,n)*x3y2+c(3,
     & 2,1,n)*x3y2z1+c(3,2,2,n)*x3y2z2+c(3,2,3,n)*x3y2z3+c(3,2,4,n)*
     & x3y2z4+c(3,3,0,n)*x3y3+c(3,3,1,n)*x3y3z1+c(3,3,2,n)*x3y3z2+c(3,
     & 3,3,n)*x3y3z3+c(3,3,4,n)*x3y3z4+c(3,4,0,n)*x3y4+c(3,4,1,n)*
     & x3y4z1+c(3,4,2,n)*x3y4z2+c(3,4,3,n)*x3y4z3+c(3,4,4,n)*x3y4z4+c(
     & 4,0,0,n)*x4+c(4,0,1,n)*x4z1+c(4,0,2,n)*x4z2+c(4,0,3,n)*x4z3+c(
     & 4,0,4,n)*x4z4+c(4,1,0,n)*x4y1+c(4,1,1,n)*x4y1z1+c(4,1,2,n)*
     & x4y1z2+c(4,1,3,n)*x4y1z3+c(4,1,4,n)*x4y1z4+c(4,2,0,n)*x4y2+c(4,
     & 2,1,n)*x4y2z1+c(4,2,2,n)*x4y2z2+c(4,2,3,n)*x4y2z3+c(4,2,4,n)*
     & x4y2z4+c(4,3,0,n)*x4y3+c(4,3,1,n)*x4y3z1+c(4,3,2,n)*x4y3z2+c(4,
     & 3,3,n)*x4y3z3+c(4,3,4,n)*x4y3z4+c(4,4,0,n)*x4y4+c(4,4,1,n)*
     & x4y4z1+c(4,4,2,n)*x4y4z2+c(4,4,3,n)*x4y4z3+c(4,4,4,n)*x4y4z4)*
     & time
! endLoops()
      else if( dx.eq.0.and.dy.eq.0.and.dz.eq.1.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
c 
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
c 
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
      r=(c(0,0,1,n)+c(0,0,2,n)*z1*2.+c(0,0,3,n)*z2*3.+c(0,0,4,n)*z3*4.+
     & c(0,1,1,n)*y1+c(0,1,2,n)*y1z1*2.+c(0,1,3,n)*y1z2*3.+c(0,1,4,n)*
     & y1z3*4.+c(0,2,1,n)*y2+c(0,2,2,n)*y2z1*2.+c(0,2,3,n)*y2z2*3.+c(
     & 0,2,4,n)*y2z3*4.+c(0,3,1,n)*y3+c(0,3,2,n)*y3z1*2.+c(0,3,3,n)*
     & y3z2*3.+c(0,3,4,n)*y3z3*4.+c(0,4,1,n)*y4+c(0,4,2,n)*y4z1*2.+c(
     & 0,4,3,n)*y4z2*3.+c(0,4,4,n)*y4z3*4.+c(1,0,1,n)*x1+c(1,0,2,n)*
     & x1z1*2.+c(1,0,3,n)*x1z2*3.+c(1,0,4,n)*x1z3*4.+c(1,1,1,n)*x1y1+
     & c(1,1,2,n)*x1y1z1*2.+c(1,1,3,n)*x1y1z2*3.+c(1,1,4,n)*x1y1z3*4.+
     & c(1,2,1,n)*x1y2+c(1,2,2,n)*x1y2z1*2.+c(1,2,3,n)*x1y2z2*3.+c(1,
     & 2,4,n)*x1y2z3*4.+c(1,3,1,n)*x1y3+c(1,3,2,n)*x1y3z1*2.+c(1,3,3,
     & n)*x1y3z2*3.+c(1,3,4,n)*x1y3z3*4.+c(1,4,1,n)*x1y4+c(1,4,2,n)*
     & x1y4z1*2.+c(1,4,3,n)*x1y4z2*3.+c(1,4,4,n)*x1y4z3*4.+c(2,0,1,n)*
     & x2+c(2,0,2,n)*x2z1*2.+c(2,0,3,n)*x2z2*3.+c(2,0,4,n)*x2z3*4.+c(
     & 2,1,1,n)*x2y1+c(2,1,2,n)*x2y1z1*2.+c(2,1,3,n)*x2y1z2*3.+c(2,1,
     & 4,n)*x2y1z3*4.+c(2,2,1,n)*x2y2+c(2,2,2,n)*x2y2z1*2.+c(2,2,3,n)*
     & x2y2z2*3.+c(2,2,4,n)*x2y2z3*4.+c(2,3,1,n)*x2y3+c(2,3,2,n)*
     & x2y3z1*2.+c(2,3,3,n)*x2y3z2*3.+c(2,3,4,n)*x2y3z3*4.+c(2,4,1,n)*
     & x2y4+c(2,4,2,n)*x2y4z1*2.+c(2,4,3,n)*x2y4z2*3.+c(2,4,4,n)*
     & x2y4z3*4.+c(3,0,1,n)*x3+c(3,0,2,n)*x3z1*2.+c(3,0,3,n)*x3z2*3.+
     & c(3,0,4,n)*x3z3*4.+c(3,1,1,n)*x3y1+c(3,1,2,n)*x3y1z1*2.+c(3,1,
     & 3,n)*x3y1z2*3.+c(3,1,4,n)*x3y1z3*4.+c(3,2,1,n)*x3y2+c(3,2,2,n)*
     & x3y2z1*2.+c(3,2,3,n)*x3y2z2*3.+c(3,2,4,n)*x3y2z3*4.+c(3,3,1,n)*
     & x3y3+c(3,3,2,n)*x3y3z1*2.+c(3,3,3,n)*x3y3z2*3.+c(3,3,4,n)*
     & x3y3z3*4.+c(3,4,1,n)*x3y4+c(3,4,2,n)*x3y4z1*2.+c(3,4,3,n)*
     & x3y4z2*3.+c(3,4,4,n)*x3y4z3*4.+c(4,0,1,n)*x4+c(4,0,2,n)*x4z1*
     & 2.+c(4,0,3,n)*x4z2*3.+c(4,0,4,n)*x4z3*4.+c(4,1,1,n)*x4y1+c(4,1,
     & 2,n)*x4y1z1*2.+c(4,1,3,n)*x4y1z2*3.+c(4,1,4,n)*x4y1z3*4.+c(4,2,
     & 1,n)*x4y2+c(4,2,2,n)*x4y2z1*2.+c(4,2,3,n)*x4y2z2*3.+c(4,2,4,n)*
     & x4y2z3*4.+c(4,3,1,n)*x4y3+c(4,3,2,n)*x4y3z1*2.+c(4,3,3,n)*
     & x4y3z2*3.+c(4,3,4,n)*x4y3z3*4.+c(4,4,1,n)*x4y4+c(4,4,2,n)*
     & x4y4z1*2.+c(4,4,3,n)*x4y4z2*3.+c(4,4,4,n)*x4y4z3*4.)*time
! endLoops()
      else if( dx.eq.0.and.dy.eq.0.and.dz.eq.2.and.dt.eq.0  )then
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
      z2=z1*z1
c 
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
c 
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
      r=(c(0,0,2,n)*2.+c(0,0,3,n)*z1*6.+c(0,0,4,n)*z2*12.+c(0,1,2,n)*
     & y1*2.+c(0,1,3,n)*y1z1*6.+c(0,1,4,n)*y1z2*12.+c(0,2,2,n)*y2*2.+
     & c(0,2,3,n)*y2z1*6.+c(0,2,4,n)*y2z2*12.+c(0,3,2,n)*y3*2.+c(0,3,
     & 3,n)*y3z1*6.+c(0,3,4,n)*y3z2*12.+c(0,4,2,n)*y4*2.+c(0,4,3,n)*
     & y4z1*6.+c(0,4,4,n)*y4z2*12.+c(1,0,2,n)*x1*2.+c(1,0,3,n)*x1z1*
     & 6.+c(1,0,4,n)*x1z2*12.+c(1,1,2,n)*x1y1*2.+c(1,1,3,n)*x1y1z1*6.+
     & c(1,1,4,n)*x1y1z2*12.+c(1,2,2,n)*x1y2*2.+c(1,2,3,n)*x1y2z1*6.+
     & c(1,2,4,n)*x1y2z2*12.+c(1,3,2,n)*x1y3*2.+c(1,3,3,n)*x1y3z1*6.+
     & c(1,3,4,n)*x1y3z2*12.+c(1,4,2,n)*x1y4*2.+c(1,4,3,n)*x1y4z1*6.+
     & c(1,4,4,n)*x1y4z2*12.+c(2,0,2,n)*x2*2.+c(2,0,3,n)*x2z1*6.+c(2,
     & 0,4,n)*x2z2*12.+c(2,1,2,n)*x2y1*2.+c(2,1,3,n)*x2y1z1*6.+c(2,1,
     & 4,n)*x2y1z2*12.+c(2,2,2,n)*x2y2*2.+c(2,2,3,n)*x2y2z1*6.+c(2,2,
     & 4,n)*x2y2z2*12.+c(2,3,2,n)*x2y3*2.+c(2,3,3,n)*x2y3z1*6.+c(2,3,
     & 4,n)*x2y3z2*12.+c(2,4,2,n)*x2y4*2.+c(2,4,3,n)*x2y4z1*6.+c(2,4,
     & 4,n)*x2y4z2*12.+c(3,0,2,n)*x3*2.+c(3,0,3,n)*x3z1*6.+c(3,0,4,n)*
     & x3z2*12.+c(3,1,2,n)*x3y1*2.+c(3,1,3,n)*x3y1z1*6.+c(3,1,4,n)*
     & x3y1z2*12.+c(3,2,2,n)*x3y2*2.+c(3,2,3,n)*x3y2z1*6.+c(3,2,4,n)*
     & x3y2z2*12.+c(3,3,2,n)*x3y3*2.+c(3,3,3,n)*x3y3z1*6.+c(3,3,4,n)*
     & x3y3z2*12.+c(3,4,2,n)*x3y4*2.+c(3,4,3,n)*x3y4z1*6.+c(3,4,4,n)*
     & x3y4z2*12.+c(4,0,2,n)*x4*2.+c(4,0,3,n)*x4z1*6.+c(4,0,4,n)*x4z2*
     & 12.+c(4,1,2,n)*x4y1*2.+c(4,1,3,n)*x4y1z1*6.+c(4,1,4,n)*x4y1z2*
     & 12.+c(4,2,2,n)*x4y2*2.+c(4,2,3,n)*x4y2z1*6.+c(4,2,4,n)*x4y2z2*
     & 12.+c(4,3,2,n)*x4y3*2.+c(4,3,3,n)*x4y3z1*6.+c(4,3,4,n)*x4y3z2*
     & 12.+c(4,4,2,n)*x4y4*2.+c(4,4,3,n)*x4y4z1*6.+c(4,4,4,n)*x4y4z2*
     & 12.)*time
! endLoops()
      else if( dx.eq.0.and.dy.eq.1.and.dz.eq.0.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
c 
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
c 
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
      r=(c(0,1,0,n)+c(0,1,1,n)*z1+c(0,1,2,n)*z2+c(0,1,3,n)*z3+c(0,1,4,
     & n)*z4+c(0,2,0,n)*y1*2.+c(0,2,1,n)*y1z1*2.+c(0,2,2,n)*y1z2*2.+c(
     & 0,2,3,n)*y1z3*2.+c(0,2,4,n)*y1z4*2.+c(0,3,0,n)*y2*3.+c(0,3,1,n)
     & *y2z1*3.+c(0,3,2,n)*y2z2*3.+c(0,3,3,n)*y2z3*3.+c(0,3,4,n)*y2z4*
     & 3.+c(0,4,0,n)*y3*4.+c(0,4,1,n)*y3z1*4.+c(0,4,2,n)*y3z2*4.+c(0,
     & 4,3,n)*y3z3*4.+c(0,4,4,n)*y3z4*4.+c(1,1,0,n)*x1+c(1,1,1,n)*
     & x1z1+c(1,1,2,n)*x1z2+c(1,1,3,n)*x1z3+c(1,1,4,n)*x1z4+c(1,2,0,n)
     & *x1y1*2.+c(1,2,1,n)*x1y1z1*2.+c(1,2,2,n)*x1y1z2*2.+c(1,2,3,n)*
     & x1y1z3*2.+c(1,2,4,n)*x1y1z4*2.+c(1,3,0,n)*x1y2*3.+c(1,3,1,n)*
     & x1y2z1*3.+c(1,3,2,n)*x1y2z2*3.+c(1,3,3,n)*x1y2z3*3.+c(1,3,4,n)*
     & x1y2z4*3.+c(1,4,0,n)*x1y3*4.+c(1,4,1,n)*x1y3z1*4.+c(1,4,2,n)*
     & x1y3z2*4.+c(1,4,3,n)*x1y3z3*4.+c(1,4,4,n)*x1y3z4*4.+c(2,1,0,n)*
     & x2+c(2,1,1,n)*x2z1+c(2,1,2,n)*x2z2+c(2,1,3,n)*x2z3+c(2,1,4,n)*
     & x2z4+c(2,2,0,n)*x2y1*2.+c(2,2,1,n)*x2y1z1*2.+c(2,2,2,n)*x2y1z2*
     & 2.+c(2,2,3,n)*x2y1z3*2.+c(2,2,4,n)*x2y1z4*2.+c(2,3,0,n)*x2y2*
     & 3.+c(2,3,1,n)*x2y2z1*3.+c(2,3,2,n)*x2y2z2*3.+c(2,3,3,n)*x2y2z3*
     & 3.+c(2,3,4,n)*x2y2z4*3.+c(2,4,0,n)*x2y3*4.+c(2,4,1,n)*x2y3z1*
     & 4.+c(2,4,2,n)*x2y3z2*4.+c(2,4,3,n)*x2y3z3*4.+c(2,4,4,n)*x2y3z4*
     & 4.+c(3,1,0,n)*x3+c(3,1,1,n)*x3z1+c(3,1,2,n)*x3z2+c(3,1,3,n)*
     & x3z3+c(3,1,4,n)*x3z4+c(3,2,0,n)*x3y1*2.+c(3,2,1,n)*x3y1z1*2.+c(
     & 3,2,2,n)*x3y1z2*2.+c(3,2,3,n)*x3y1z3*2.+c(3,2,4,n)*x3y1z4*2.+c(
     & 3,3,0,n)*x3y2*3.+c(3,3,1,n)*x3y2z1*3.+c(3,3,2,n)*x3y2z2*3.+c(3,
     & 3,3,n)*x3y2z3*3.+c(3,3,4,n)*x3y2z4*3.+c(3,4,0,n)*x3y3*4.+c(3,4,
     & 1,n)*x3y3z1*4.+c(3,4,2,n)*x3y3z2*4.+c(3,4,3,n)*x3y3z3*4.+c(3,4,
     & 4,n)*x3y3z4*4.+c(4,1,0,n)*x4+c(4,1,1,n)*x4z1+c(4,1,2,n)*x4z2+c(
     & 4,1,3,n)*x4z3+c(4,1,4,n)*x4z4+c(4,2,0,n)*x4y1*2.+c(4,2,1,n)*
     & x4y1z1*2.+c(4,2,2,n)*x4y1z2*2.+c(4,2,3,n)*x4y1z3*2.+c(4,2,4,n)*
     & x4y1z4*2.+c(4,3,0,n)*x4y2*3.+c(4,3,1,n)*x4y2z1*3.+c(4,3,2,n)*
     & x4y2z2*3.+c(4,3,3,n)*x4y2z3*3.+c(4,3,4,n)*x4y2z4*3.+c(4,4,0,n)*
     & x4y3*4.+c(4,4,1,n)*x4y3z1*4.+c(4,4,2,n)*x4y3z2*4.+c(4,4,3,n)*
     & x4y3z3*4.+c(4,4,4,n)*x4y3z4*4.)*time
! endLoops()
      else if( dx.eq.0.and.dy.eq.1.and.dz.eq.1.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
c 
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
c 
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
      r=(c(0,1,1,n)+c(0,1,2,n)*z1*2.+c(0,1,3,n)*z2*3.+c(0,1,4,n)*z3*4.+
     & c(0,2,1,n)*y1*2.+c(0,2,2,n)*y1z1*4.+c(0,2,3,n)*y1z2*6.+c(0,2,4,
     & n)*y1z3*8.+c(0,3,1,n)*y2*3.+c(0,3,2,n)*y2z1*6.+c(0,3,3,n)*y2z2*
     & 9.+c(0,3,4,n)*y2z3*12.+c(0,4,1,n)*y3*4.+c(0,4,2,n)*y3z1*8.+c(0,
     & 4,3,n)*y3z2*12.+c(0,4,4,n)*y3z3*16.+c(1,1,1,n)*x1+c(1,1,2,n)*
     & x1z1*2.+c(1,1,3,n)*x1z2*3.+c(1,1,4,n)*x1z3*4.+c(1,2,1,n)*x1y1*
     & 2.+c(1,2,2,n)*x1y1z1*4.+c(1,2,3,n)*x1y1z2*6.+c(1,2,4,n)*x1y1z3*
     & 8.+c(1,3,1,n)*x1y2*3.+c(1,3,2,n)*x1y2z1*6.+c(1,3,3,n)*x1y2z2*
     & 9.+c(1,3,4,n)*x1y2z3*12.+c(1,4,1,n)*x1y3*4.+c(1,4,2,n)*x1y3z1*
     & 8.+c(1,4,3,n)*x1y3z2*12.+c(1,4,4,n)*x1y3z3*16.+c(2,1,1,n)*x2+c(
     & 2,1,2,n)*x2z1*2.+c(2,1,3,n)*x2z2*3.+c(2,1,4,n)*x2z3*4.+c(2,2,1,
     & n)*x2y1*2.+c(2,2,2,n)*x2y1z1*4.+c(2,2,3,n)*x2y1z2*6.+c(2,2,4,n)
     & *x2y1z3*8.+c(2,3,1,n)*x2y2*3.+c(2,3,2,n)*x2y2z1*6.+c(2,3,3,n)*
     & x2y2z2*9.+c(2,3,4,n)*x2y2z3*12.+c(2,4,1,n)*x2y3*4.+c(2,4,2,n)*
     & x2y3z1*8.+c(2,4,3,n)*x2y3z2*12.+c(2,4,4,n)*x2y3z3*16.+c(3,1,1,
     & n)*x3+c(3,1,2,n)*x3z1*2.+c(3,1,3,n)*x3z2*3.+c(3,1,4,n)*x3z3*4.+
     & c(3,2,1,n)*x3y1*2.+c(3,2,2,n)*x3y1z1*4.+c(3,2,3,n)*x3y1z2*6.+c(
     & 3,2,4,n)*x3y1z3*8.+c(3,3,1,n)*x3y2*3.+c(3,3,2,n)*x3y2z1*6.+c(3,
     & 3,3,n)*x3y2z2*9.+c(3,3,4,n)*x3y2z3*12.+c(3,4,1,n)*x3y3*4.+c(3,
     & 4,2,n)*x3y3z1*8.+c(3,4,3,n)*x3y3z2*12.+c(3,4,4,n)*x3y3z3*16.+c(
     & 4,1,1,n)*x4+c(4,1,2,n)*x4z1*2.+c(4,1,3,n)*x4z2*3.+c(4,1,4,n)*
     & x4z3*4.+c(4,2,1,n)*x4y1*2.+c(4,2,2,n)*x4y1z1*4.+c(4,2,3,n)*
     & x4y1z2*6.+c(4,2,4,n)*x4y1z3*8.+c(4,3,1,n)*x4y2*3.+c(4,3,2,n)*
     & x4y2z1*6.+c(4,3,3,n)*x4y2z2*9.+c(4,3,4,n)*x4y2z3*12.+c(4,4,1,n)
     & *x4y3*4.+c(4,4,2,n)*x4y3z1*8.+c(4,4,3,n)*x4y3z2*12.+c(4,4,4,n)*
     & x4y3z3*16.)*time
! endLoops()
      else if( dx.eq.0.and.dy.eq.1.and.dz.eq.2.and.dt.eq.0  )then
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
      z2=z1*z1
c 
      y1z1=y1*z1
      y1z2=y1z1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
c 
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
      r=(c(0,1,2,n)*2.+c(0,1,3,n)*z1*6.+c(0,1,4,n)*z2*12.+c(0,2,2,n)*
     & y1*4.+c(0,2,3,n)*y1z1*12.+c(0,2,4,n)*y1z2*24.+c(0,3,2,n)*y2*6.+
     & c(0,3,3,n)*y2z1*18.+c(0,3,4,n)*y2z2*36.+c(0,4,2,n)*y3*8.+c(0,4,
     & 3,n)*y3z1*24.+c(0,4,4,n)*y3z2*48.+c(1,1,2,n)*x1*2.+c(1,1,3,n)*
     & x1z1*6.+c(1,1,4,n)*x1z2*12.+c(1,2,2,n)*x1y1*4.+c(1,2,3,n)*
     & x1y1z1*12.+c(1,2,4,n)*x1y1z2*24.+c(1,3,2,n)*x1y2*6.+c(1,3,3,n)*
     & x1y2z1*18.+c(1,3,4,n)*x1y2z2*36.+c(1,4,2,n)*x1y3*8.+c(1,4,3,n)*
     & x1y3z1*24.+c(1,4,4,n)*x1y3z2*48.+c(2,1,2,n)*x2*2.+c(2,1,3,n)*
     & x2z1*6.+c(2,1,4,n)*x2z2*12.+c(2,2,2,n)*x2y1*4.+c(2,2,3,n)*
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
      else if( dx.eq.0.and.dy.eq.2.and.dz.eq.0.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
c 
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
c 
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
      r=(c(0,2,0,n)*2.+c(0,2,1,n)*z1*2.+c(0,2,2,n)*z2*2.+c(0,2,3,n)*z3*
     & 2.+c(0,2,4,n)*z4*2.+c(0,3,0,n)*y1*6.+c(0,3,1,n)*y1z1*6.+c(0,3,
     & 2,n)*y1z2*6.+c(0,3,3,n)*y1z3*6.+c(0,3,4,n)*y1z4*6.+c(0,4,0,n)*
     & y2*12.+c(0,4,1,n)*y2z1*12.+c(0,4,2,n)*y2z2*12.+c(0,4,3,n)*y2z3*
     & 12.+c(0,4,4,n)*y2z4*12.+c(1,2,0,n)*x1*2.+c(1,2,1,n)*x1z1*2.+c(
     & 1,2,2,n)*x1z2*2.+c(1,2,3,n)*x1z3*2.+c(1,2,4,n)*x1z4*2.+c(1,3,0,
     & n)*x1y1*6.+c(1,3,1,n)*x1y1z1*6.+c(1,3,2,n)*x1y1z2*6.+c(1,3,3,n)
     & *x1y1z3*6.+c(1,3,4,n)*x1y1z4*6.+c(1,4,0,n)*x1y2*12.+c(1,4,1,n)*
     & x1y2z1*12.+c(1,4,2,n)*x1y2z2*12.+c(1,4,3,n)*x1y2z3*12.+c(1,4,4,
     & n)*x1y2z4*12.+c(2,2,0,n)*x2*2.+c(2,2,1,n)*x2z1*2.+c(2,2,2,n)*
     & x2z2*2.+c(2,2,3,n)*x2z3*2.+c(2,2,4,n)*x2z4*2.+c(2,3,0,n)*x2y1*
     & 6.+c(2,3,1,n)*x2y1z1*6.+c(2,3,2,n)*x2y1z2*6.+c(2,3,3,n)*x2y1z3*
     & 6.+c(2,3,4,n)*x2y1z4*6.+c(2,4,0,n)*x2y2*12.+c(2,4,1,n)*x2y2z1*
     & 12.+c(2,4,2,n)*x2y2z2*12.+c(2,4,3,n)*x2y2z3*12.+c(2,4,4,n)*
     & x2y2z4*12.+c(3,2,0,n)*x3*2.+c(3,2,1,n)*x3z1*2.+c(3,2,2,n)*x3z2*
     & 2.+c(3,2,3,n)*x3z3*2.+c(3,2,4,n)*x3z4*2.+c(3,3,0,n)*x3y1*6.+c(
     & 3,3,1,n)*x3y1z1*6.+c(3,3,2,n)*x3y1z2*6.+c(3,3,3,n)*x3y1z3*6.+c(
     & 3,3,4,n)*x3y1z4*6.+c(3,4,0,n)*x3y2*12.+c(3,4,1,n)*x3y2z1*12.+c(
     & 3,4,2,n)*x3y2z2*12.+c(3,4,3,n)*x3y2z3*12.+c(3,4,4,n)*x3y2z4*
     & 12.+c(4,2,0,n)*x4*2.+c(4,2,1,n)*x4z1*2.+c(4,2,2,n)*x4z2*2.+c(4,
     & 2,3,n)*x4z3*2.+c(4,2,4,n)*x4z4*2.+c(4,3,0,n)*x4y1*6.+c(4,3,1,n)
     & *x4y1z1*6.+c(4,3,2,n)*x4y1z2*6.+c(4,3,3,n)*x4y1z3*6.+c(4,3,4,n)
     & *x4y1z4*6.+c(4,4,0,n)*x4y2*12.+c(4,4,1,n)*x4y2z1*12.+c(4,4,2,n)
     & *x4y2z2*12.+c(4,4,3,n)*x4y2z3*12.+c(4,4,4,n)*x4y2z4*12.)*time
! endLoops()
      else if( dx.eq.0.and.dy.eq.2.and.dz.eq.1.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
c 
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
c 
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
      r=(c(0,2,1,n)*2.+c(0,2,2,n)*z1*4.+c(0,2,3,n)*z2*6.+c(0,2,4,n)*z3*
     & 8.+c(0,3,1,n)*y1*6.+c(0,3,2,n)*y1z1*12.+c(0,3,3,n)*y1z2*18.+c(
     & 0,3,4,n)*y1z3*24.+c(0,4,1,n)*y2*12.+c(0,4,2,n)*y2z1*24.+c(0,4,
     & 3,n)*y2z2*36.+c(0,4,4,n)*y2z3*48.+c(1,2,1,n)*x1*2.+c(1,2,2,n)*
     & x1z1*4.+c(1,2,3,n)*x1z2*6.+c(1,2,4,n)*x1z3*8.+c(1,3,1,n)*x1y1*
     & 6.+c(1,3,2,n)*x1y1z1*12.+c(1,3,3,n)*x1y1z2*18.+c(1,3,4,n)*
     & x1y1z3*24.+c(1,4,1,n)*x1y2*12.+c(1,4,2,n)*x1y2z1*24.+c(1,4,3,n)
     & *x1y2z2*36.+c(1,4,4,n)*x1y2z3*48.+c(2,2,1,n)*x2*2.+c(2,2,2,n)*
     & x2z1*4.+c(2,2,3,n)*x2z2*6.+c(2,2,4,n)*x2z3*8.+c(2,3,1,n)*x2y1*
     & 6.+c(2,3,2,n)*x2y1z1*12.+c(2,3,3,n)*x2y1z2*18.+c(2,3,4,n)*
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
      else if( dx.eq.0.and.dy.eq.2.and.dz.eq.2.and.dt.eq.0  )then
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
      z2=z1*z1
c 
      y1z1=y1*z1
      y1z2=y1z1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
c 
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
      r=(c(0,2,2,n)*4.+c(0,2,3,n)*z1*12.+c(0,2,4,n)*z2*24.+c(0,3,2,n)*
     & y1*12.+c(0,3,3,n)*y1z1*36.+c(0,3,4,n)*y1z2*72.+c(0,4,2,n)*y2*
     & 24.+c(0,4,3,n)*y2z1*72.+c(0,4,4,n)*y2z2*144.+c(1,2,2,n)*x1*4.+
     & c(1,2,3,n)*x1z1*12.+c(1,2,4,n)*x1z2*24.+c(1,3,2,n)*x1y1*12.+c(
     & 1,3,3,n)*x1y1z1*36.+c(1,3,4,n)*x1y1z2*72.+c(1,4,2,n)*x1y2*24.+
     & c(1,4,3,n)*x1y2z1*72.+c(1,4,4,n)*x1y2z2*144.+c(2,2,2,n)*x2*4.+
     & c(2,2,3,n)*x2z1*12.+c(2,2,4,n)*x2z2*24.+c(2,3,2,n)*x2y1*12.+c(
     & 2,3,3,n)*x2y1z1*36.+c(2,3,4,n)*x2y1z2*72.+c(2,4,2,n)*x2y2*24.+
     & c(2,4,3,n)*x2y2z1*72.+c(2,4,4,n)*x2y2z2*144.+c(3,2,2,n)*x3*4.+
     & c(3,2,3,n)*x3z1*12.+c(3,2,4,n)*x3z2*24.+c(3,3,2,n)*x3y1*12.+c(
     & 3,3,3,n)*x3y1z1*36.+c(3,3,4,n)*x3y1z2*72.+c(3,4,2,n)*x3y2*24.+
     & c(3,4,3,n)*x3y2z1*72.+c(3,4,4,n)*x3y2z2*144.+c(4,2,2,n)*x4*4.+
     & c(4,2,3,n)*x4z1*12.+c(4,2,4,n)*x4z2*24.+c(4,3,2,n)*x4y1*12.+c(
     & 4,3,3,n)*x4y1z1*36.+c(4,3,4,n)*x4y1z2*72.+c(4,4,2,n)*x4y2*24.+
     & c(4,4,3,n)*x4y2z1*72.+c(4,4,4,n)*x4y2z2*144.)*time
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
c 
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
c 
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
      r=(c(1,0,0,n)+c(1,0,1,n)*z1+c(1,0,2,n)*z2+c(1,0,3,n)*z3+c(1,0,4,
     & n)*z4+c(1,1,0,n)*y1+c(1,1,1,n)*y1z1+c(1,1,2,n)*y1z2+c(1,1,3,n)*
     & y1z3+c(1,1,4,n)*y1z4+c(1,2,0,n)*y2+c(1,2,1,n)*y2z1+c(1,2,2,n)*
     & y2z2+c(1,2,3,n)*y2z3+c(1,2,4,n)*y2z4+c(1,3,0,n)*y3+c(1,3,1,n)*
     & y3z1+c(1,3,2,n)*y3z2+c(1,3,3,n)*y3z3+c(1,3,4,n)*y3z4+c(1,4,0,n)
     & *y4+c(1,4,1,n)*y4z1+c(1,4,2,n)*y4z2+c(1,4,3,n)*y4z3+c(1,4,4,n)*
     & y4z4+c(2,0,0,n)*x1*2.+c(2,0,1,n)*x1z1*2.+c(2,0,2,n)*x1z2*2.+c(
     & 2,0,3,n)*x1z3*2.+c(2,0,4,n)*x1z4*2.+c(2,1,0,n)*x1y1*2.+c(2,1,1,
     & n)*x1y1z1*2.+c(2,1,2,n)*x1y1z2*2.+c(2,1,3,n)*x1y1z3*2.+c(2,1,4,
     & n)*x1y1z4*2.+c(2,2,0,n)*x1y2*2.+c(2,2,1,n)*x1y2z1*2.+c(2,2,2,n)
     & *x1y2z2*2.+c(2,2,3,n)*x1y2z3*2.+c(2,2,4,n)*x1y2z4*2.+c(2,3,0,n)
     & *x1y3*2.+c(2,3,1,n)*x1y3z1*2.+c(2,3,2,n)*x1y3z2*2.+c(2,3,3,n)*
     & x1y3z3*2.+c(2,3,4,n)*x1y3z4*2.+c(2,4,0,n)*x1y4*2.+c(2,4,1,n)*
     & x1y4z1*2.+c(2,4,2,n)*x1y4z2*2.+c(2,4,3,n)*x1y4z3*2.+c(2,4,4,n)*
     & x1y4z4*2.+c(3,0,0,n)*x2*3.+c(3,0,1,n)*x2z1*3.+c(3,0,2,n)*x2z2*
     & 3.+c(3,0,3,n)*x2z3*3.+c(3,0,4,n)*x2z4*3.+c(3,1,0,n)*x2y1*3.+c(
     & 3,1,1,n)*x2y1z1*3.+c(3,1,2,n)*x2y1z2*3.+c(3,1,3,n)*x2y1z3*3.+c(
     & 3,1,4,n)*x2y1z4*3.+c(3,2,0,n)*x2y2*3.+c(3,2,1,n)*x2y2z1*3.+c(3,
     & 2,2,n)*x2y2z2*3.+c(3,2,3,n)*x2y2z3*3.+c(3,2,4,n)*x2y2z4*3.+c(3,
     & 3,0,n)*x2y3*3.+c(3,3,1,n)*x2y3z1*3.+c(3,3,2,n)*x2y3z2*3.+c(3,3,
     & 3,n)*x2y3z3*3.+c(3,3,4,n)*x2y3z4*3.+c(3,4,0,n)*x2y4*3.+c(3,4,1,
     & n)*x2y4z1*3.+c(3,4,2,n)*x2y4z2*3.+c(3,4,3,n)*x2y4z3*3.+c(3,4,4,
     & n)*x2y4z4*3.+c(4,0,0,n)*x3*4.+c(4,0,1,n)*x3z1*4.+c(4,0,2,n)*
     & x3z2*4.+c(4,0,3,n)*x3z3*4.+c(4,0,4,n)*x3z4*4.+c(4,1,0,n)*x3y1*
     & 4.+c(4,1,1,n)*x3y1z1*4.+c(4,1,2,n)*x3y1z2*4.+c(4,1,3,n)*x3y1z3*
     & 4.+c(4,1,4,n)*x3y1z4*4.+c(4,2,0,n)*x3y2*4.+c(4,2,1,n)*x3y2z1*
     & 4.+c(4,2,2,n)*x3y2z2*4.+c(4,2,3,n)*x3y2z3*4.+c(4,2,4,n)*x3y2z4*
     & 4.+c(4,3,0,n)*x3y3*4.+c(4,3,1,n)*x3y3z1*4.+c(4,3,2,n)*x3y3z2*
     & 4.+c(4,3,3,n)*x3y3z3*4.+c(4,3,4,n)*x3y3z4*4.+c(4,4,0,n)*x3y4*
     & 4.+c(4,4,1,n)*x3y4z1*4.+c(4,4,2,n)*x3y4z2*4.+c(4,4,3,n)*x3y4z3*
     & 4.+c(4,4,4,n)*x3y4z4*4.)*time
! endLoops()
      else if( dx.eq.1.and.dy.eq.0.and.dz.eq.1.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
c 
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
c 
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
      r=(c(1,0,1,n)+c(1,0,2,n)*z1*2.+c(1,0,3,n)*z2*3.+c(1,0,4,n)*z3*4.+
     & c(1,1,1,n)*y1+c(1,1,2,n)*y1z1*2.+c(1,1,3,n)*y1z2*3.+c(1,1,4,n)*
     & y1z3*4.+c(1,2,1,n)*y2+c(1,2,2,n)*y2z1*2.+c(1,2,3,n)*y2z2*3.+c(
     & 1,2,4,n)*y2z3*4.+c(1,3,1,n)*y3+c(1,3,2,n)*y3z1*2.+c(1,3,3,n)*
     & y3z2*3.+c(1,3,4,n)*y3z3*4.+c(1,4,1,n)*y4+c(1,4,2,n)*y4z1*2.+c(
     & 1,4,3,n)*y4z2*3.+c(1,4,4,n)*y4z3*4.+c(2,0,1,n)*x1*2.+c(2,0,2,n)
     & *x1z1*4.+c(2,0,3,n)*x1z2*6.+c(2,0,4,n)*x1z3*8.+c(2,1,1,n)*x1y1*
     & 2.+c(2,1,2,n)*x1y1z1*4.+c(2,1,3,n)*x1y1z2*6.+c(2,1,4,n)*x1y1z3*
     & 8.+c(2,2,1,n)*x1y2*2.+c(2,2,2,n)*x1y2z1*4.+c(2,2,3,n)*x1y2z2*
     & 6.+c(2,2,4,n)*x1y2z3*8.+c(2,3,1,n)*x1y3*2.+c(2,3,2,n)*x1y3z1*
     & 4.+c(2,3,3,n)*x1y3z2*6.+c(2,3,4,n)*x1y3z3*8.+c(2,4,1,n)*x1y4*
     & 2.+c(2,4,2,n)*x1y4z1*4.+c(2,4,3,n)*x1y4z2*6.+c(2,4,4,n)*x1y4z3*
     & 8.+c(3,0,1,n)*x2*3.+c(3,0,2,n)*x2z1*6.+c(3,0,3,n)*x2z2*9.+c(3,
     & 0,4,n)*x2z3*12.+c(3,1,1,n)*x2y1*3.+c(3,1,2,n)*x2y1z1*6.+c(3,1,
     & 3,n)*x2y1z2*9.+c(3,1,4,n)*x2y1z3*12.+c(3,2,1,n)*x2y2*3.+c(3,2,
     & 2,n)*x2y2z1*6.+c(3,2,3,n)*x2y2z2*9.+c(3,2,4,n)*x2y2z3*12.+c(3,
     & 3,1,n)*x2y3*3.+c(3,3,2,n)*x2y3z1*6.+c(3,3,3,n)*x2y3z2*9.+c(3,3,
     & 4,n)*x2y3z3*12.+c(3,4,1,n)*x2y4*3.+c(3,4,2,n)*x2y4z1*6.+c(3,4,
     & 3,n)*x2y4z2*9.+c(3,4,4,n)*x2y4z3*12.+c(4,0,1,n)*x3*4.+c(4,0,2,
     & n)*x3z1*8.+c(4,0,3,n)*x3z2*12.+c(4,0,4,n)*x3z3*16.+c(4,1,1,n)*
     & x3y1*4.+c(4,1,2,n)*x3y1z1*8.+c(4,1,3,n)*x3y1z2*12.+c(4,1,4,n)*
     & x3y1z3*16.+c(4,2,1,n)*x3y2*4.+c(4,2,2,n)*x3y2z1*8.+c(4,2,3,n)*
     & x3y2z2*12.+c(4,2,4,n)*x3y2z3*16.+c(4,3,1,n)*x3y3*4.+c(4,3,2,n)*
     & x3y3z1*8.+c(4,3,3,n)*x3y3z2*12.+c(4,3,4,n)*x3y3z3*16.+c(4,4,1,
     & n)*x3y4*4.+c(4,4,2,n)*x3y4z1*8.+c(4,4,3,n)*x3y4z2*12.+c(4,4,4,
     & n)*x3y4z3*16.)*time
! endLoops()
      else if( dx.eq.1.and.dy.eq.0.and.dz.eq.2.and.dt.eq.0  )then
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
      z2=z1*z1
c 
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
c 
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
      r=(c(1,0,2,n)*2.+c(1,0,3,n)*z1*6.+c(1,0,4,n)*z2*12.+c(1,1,2,n)*
     & y1*2.+c(1,1,3,n)*y1z1*6.+c(1,1,4,n)*y1z2*12.+c(1,2,2,n)*y2*2.+
     & c(1,2,3,n)*y2z1*6.+c(1,2,4,n)*y2z2*12.+c(1,3,2,n)*y3*2.+c(1,3,
     & 3,n)*y3z1*6.+c(1,3,4,n)*y3z2*12.+c(1,4,2,n)*y4*2.+c(1,4,3,n)*
     & y4z1*6.+c(1,4,4,n)*y4z2*12.+c(2,0,2,n)*x1*4.+c(2,0,3,n)*x1z1*
     & 12.+c(2,0,4,n)*x1z2*24.+c(2,1,2,n)*x1y1*4.+c(2,1,3,n)*x1y1z1*
     & 12.+c(2,1,4,n)*x1y1z2*24.+c(2,2,2,n)*x1y2*4.+c(2,2,3,n)*x1y2z1*
     & 12.+c(2,2,4,n)*x1y2z2*24.+c(2,3,2,n)*x1y3*4.+c(2,3,3,n)*x1y3z1*
     & 12.+c(2,3,4,n)*x1y3z2*24.+c(2,4,2,n)*x1y4*4.+c(2,4,3,n)*x1y4z1*
     & 12.+c(2,4,4,n)*x1y4z2*24.+c(3,0,2,n)*x2*6.+c(3,0,3,n)*x2z1*18.+
     & c(3,0,4,n)*x2z2*36.+c(3,1,2,n)*x2y1*6.+c(3,1,3,n)*x2y1z1*18.+c(
     & 3,1,4,n)*x2y1z2*36.+c(3,2,2,n)*x2y2*6.+c(3,2,3,n)*x2y2z1*18.+c(
     & 3,2,4,n)*x2y2z2*36.+c(3,3,2,n)*x2y3*6.+c(3,3,3,n)*x2y3z1*18.+c(
     & 3,3,4,n)*x2y3z2*36.+c(3,4,2,n)*x2y4*6.+c(3,4,3,n)*x2y4z1*18.+c(
     & 3,4,4,n)*x2y4z2*36.+c(4,0,2,n)*x3*8.+c(4,0,3,n)*x3z1*24.+c(4,0,
     & 4,n)*x3z2*48.+c(4,1,2,n)*x3y1*8.+c(4,1,3,n)*x3y1z1*24.+c(4,1,4,
     & n)*x3y1z2*48.+c(4,2,2,n)*x3y2*8.+c(4,2,3,n)*x3y2z1*24.+c(4,2,4,
     & n)*x3y2z2*48.+c(4,3,2,n)*x3y3*8.+c(4,3,3,n)*x3y3z1*24.+c(4,3,4,
     & n)*x3y3z2*48.+c(4,4,2,n)*x3y4*8.+c(4,4,3,n)*x3y4z1*24.+c(4,4,4,
     & n)*x3y4z2*48.)*time
! endLoops()
      else if( dx.eq.1.and.dy.eq.1.and.dz.eq.0.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
c 
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
c 
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
      r=(c(1,1,0,n)+c(1,1,1,n)*z1+c(1,1,2,n)*z2+c(1,1,3,n)*z3+c(1,1,4,
     & n)*z4+c(1,2,0,n)*y1*2.+c(1,2,1,n)*y1z1*2.+c(1,2,2,n)*y1z2*2.+c(
     & 1,2,3,n)*y1z3*2.+c(1,2,4,n)*y1z4*2.+c(1,3,0,n)*y2*3.+c(1,3,1,n)
     & *y2z1*3.+c(1,3,2,n)*y2z2*3.+c(1,3,3,n)*y2z3*3.+c(1,3,4,n)*y2z4*
     & 3.+c(1,4,0,n)*y3*4.+c(1,4,1,n)*y3z1*4.+c(1,4,2,n)*y3z2*4.+c(1,
     & 4,3,n)*y3z3*4.+c(1,4,4,n)*y3z4*4.+c(2,1,0,n)*x1*2.+c(2,1,1,n)*
     & x1z1*2.+c(2,1,2,n)*x1z2*2.+c(2,1,3,n)*x1z3*2.+c(2,1,4,n)*x1z4*
     & 2.+c(2,2,0,n)*x1y1*4.+c(2,2,1,n)*x1y1z1*4.+c(2,2,2,n)*x1y1z2*
     & 4.+c(2,2,3,n)*x1y1z3*4.+c(2,2,4,n)*x1y1z4*4.+c(2,3,0,n)*x1y2*
     & 6.+c(2,3,1,n)*x1y2z1*6.+c(2,3,2,n)*x1y2z2*6.+c(2,3,3,n)*x1y2z3*
     & 6.+c(2,3,4,n)*x1y2z4*6.+c(2,4,0,n)*x1y3*8.+c(2,4,1,n)*x1y3z1*
     & 8.+c(2,4,2,n)*x1y3z2*8.+c(2,4,3,n)*x1y3z3*8.+c(2,4,4,n)*x1y3z4*
     & 8.+c(3,1,0,n)*x2*3.+c(3,1,1,n)*x2z1*3.+c(3,1,2,n)*x2z2*3.+c(3,
     & 1,3,n)*x2z3*3.+c(3,1,4,n)*x2z4*3.+c(3,2,0,n)*x2y1*6.+c(3,2,1,n)
     & *x2y1z1*6.+c(3,2,2,n)*x2y1z2*6.+c(3,2,3,n)*x2y1z3*6.+c(3,2,4,n)
     & *x2y1z4*6.+c(3,3,0,n)*x2y2*9.+c(3,3,1,n)*x2y2z1*9.+c(3,3,2,n)*
     & x2y2z2*9.+c(3,3,3,n)*x2y2z3*9.+c(3,3,4,n)*x2y2z4*9.+c(3,4,0,n)*
     & x2y3*12.+c(3,4,1,n)*x2y3z1*12.+c(3,4,2,n)*x2y3z2*12.+c(3,4,3,n)
     & *x2y3z3*12.+c(3,4,4,n)*x2y3z4*12.+c(4,1,0,n)*x3*4.+c(4,1,1,n)*
     & x3z1*4.+c(4,1,2,n)*x3z2*4.+c(4,1,3,n)*x3z3*4.+c(4,1,4,n)*x3z4*
     & 4.+c(4,2,0,n)*x3y1*8.+c(4,2,1,n)*x3y1z1*8.+c(4,2,2,n)*x3y1z2*
     & 8.+c(4,2,3,n)*x3y1z3*8.+c(4,2,4,n)*x3y1z4*8.+c(4,3,0,n)*x3y2*
     & 12.+c(4,3,1,n)*x3y2z1*12.+c(4,3,2,n)*x3y2z2*12.+c(4,3,3,n)*
     & x3y2z3*12.+c(4,3,4,n)*x3y2z4*12.+c(4,4,0,n)*x3y3*16.+c(4,4,1,n)
     & *x3y3z1*16.+c(4,4,2,n)*x3y3z2*16.+c(4,4,3,n)*x3y3z3*16.+c(4,4,
     & 4,n)*x3y3z4*16.)*time
! endLoops()
      else if( dx.eq.1.and.dy.eq.1.and.dz.eq.1.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
c 
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
c 
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
      r=(c(1,1,1,n)+c(1,1,2,n)*z1*2.+c(1,1,3,n)*z2*3.+c(1,1,4,n)*z3*4.+
     & c(1,2,1,n)*y1*2.+c(1,2,2,n)*y1z1*4.+c(1,2,3,n)*y1z2*6.+c(1,2,4,
     & n)*y1z3*8.+c(1,3,1,n)*y2*3.+c(1,3,2,n)*y2z1*6.+c(1,3,3,n)*y2z2*
     & 9.+c(1,3,4,n)*y2z3*12.+c(1,4,1,n)*y3*4.+c(1,4,2,n)*y3z1*8.+c(1,
     & 4,3,n)*y3z2*12.+c(1,4,4,n)*y3z3*16.+c(2,1,1,n)*x1*2.+c(2,1,2,n)
     & *x1z1*4.+c(2,1,3,n)*x1z2*6.+c(2,1,4,n)*x1z3*8.+c(2,2,1,n)*x1y1*
     & 4.+c(2,2,2,n)*x1y1z1*8.+c(2,2,3,n)*x1y1z2*12.+c(2,2,4,n)*
     & x1y1z3*16.+c(2,3,1,n)*x1y2*6.+c(2,3,2,n)*x1y2z1*12.+c(2,3,3,n)*
     & x1y2z2*18.+c(2,3,4,n)*x1y2z3*24.+c(2,4,1,n)*x1y3*8.+c(2,4,2,n)*
     & x1y3z1*16.+c(2,4,3,n)*x1y3z2*24.+c(2,4,4,n)*x1y3z3*32.+c(3,1,1,
     & n)*x2*3.+c(3,1,2,n)*x2z1*6.+c(3,1,3,n)*x2z2*9.+c(3,1,4,n)*x2z3*
     & 12.+c(3,2,1,n)*x2y1*6.+c(3,2,2,n)*x2y1z1*12.+c(3,2,3,n)*x2y1z2*
     & 18.+c(3,2,4,n)*x2y1z3*24.+c(3,3,1,n)*x2y2*9.+c(3,3,2,n)*x2y2z1*
     & 18.+c(3,3,3,n)*x2y2z2*27.+c(3,3,4,n)*x2y2z3*36.+c(3,4,1,n)*
     & x2y3*12.+c(3,4,2,n)*x2y3z1*24.+c(3,4,3,n)*x2y3z2*36.+c(3,4,4,n)
     & *x2y3z3*48.+c(4,1,1,n)*x3*4.+c(4,1,2,n)*x3z1*8.+c(4,1,3,n)*
     & x3z2*12.+c(4,1,4,n)*x3z3*16.+c(4,2,1,n)*x3y1*8.+c(4,2,2,n)*
     & x3y1z1*16.+c(4,2,3,n)*x3y1z2*24.+c(4,2,4,n)*x3y1z3*32.+c(4,3,1,
     & n)*x3y2*12.+c(4,3,2,n)*x3y2z1*24.+c(4,3,3,n)*x3y2z2*36.+c(4,3,
     & 4,n)*x3y2z3*48.+c(4,4,1,n)*x3y3*16.+c(4,4,2,n)*x3y3z1*32.+c(4,
     & 4,3,n)*x3y3z2*48.+c(4,4,4,n)*x3y3z3*64.)*time
! endLoops()
      else if( dx.eq.1.and.dy.eq.1.and.dz.eq.2.and.dt.eq.0  )then
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
      z2=z1*z1
c 
      y1z1=y1*z1
      y1z2=y1z1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
c 
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
      r=(c(1,1,2,n)*2.+c(1,1,3,n)*z1*6.+c(1,1,4,n)*z2*12.+c(1,2,2,n)*
     & y1*4.+c(1,2,3,n)*y1z1*12.+c(1,2,4,n)*y1z2*24.+c(1,3,2,n)*y2*6.+
     & c(1,3,3,n)*y2z1*18.+c(1,3,4,n)*y2z2*36.+c(1,4,2,n)*y3*8.+c(1,4,
     & 3,n)*y3z1*24.+c(1,4,4,n)*y3z2*48.+c(2,1,2,n)*x1*4.+c(2,1,3,n)*
     & x1z1*12.+c(2,1,4,n)*x1z2*24.+c(2,2,2,n)*x1y1*8.+c(2,2,3,n)*
     & x1y1z1*24.+c(2,2,4,n)*x1y1z2*48.+c(2,3,2,n)*x1y2*12.+c(2,3,3,n)
     & *x1y2z1*36.+c(2,3,4,n)*x1y2z2*72.+c(2,4,2,n)*x1y3*16.+c(2,4,3,
     & n)*x1y3z1*48.+c(2,4,4,n)*x1y3z2*96.+c(3,1,2,n)*x2*6.+c(3,1,3,n)
     & *x2z1*18.+c(3,1,4,n)*x2z2*36.+c(3,2,2,n)*x2y1*12.+c(3,2,3,n)*
     & x2y1z1*36.+c(3,2,4,n)*x2y1z2*72.+c(3,3,2,n)*x2y2*18.+c(3,3,3,n)
     & *x2y2z1*54.+c(3,3,4,n)*x2y2z2*108.+c(3,4,2,n)*x2y3*24.+c(3,4,3,
     & n)*x2y3z1*72.+c(3,4,4,n)*x2y3z2*144.+c(4,1,2,n)*x3*8.+c(4,1,3,
     & n)*x3z1*24.+c(4,1,4,n)*x3z2*48.+c(4,2,2,n)*x3y1*16.+c(4,2,3,n)*
     & x3y1z1*48.+c(4,2,4,n)*x3y1z2*96.+c(4,3,2,n)*x3y2*24.+c(4,3,3,n)
     & *x3y2z1*72.+c(4,3,4,n)*x3y2z2*144.+c(4,4,2,n)*x3y3*32.+c(4,4,3,
     & n)*x3y3z1*96.+c(4,4,4,n)*x3y3z2*192.)*time
! endLoops()
      else if( dx.eq.1.and.dy.eq.2.and.dz.eq.0.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
c 
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
c 
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
      r=(c(1,2,0,n)*2.+c(1,2,1,n)*z1*2.+c(1,2,2,n)*z2*2.+c(1,2,3,n)*z3*
     & 2.+c(1,2,4,n)*z4*2.+c(1,3,0,n)*y1*6.+c(1,3,1,n)*y1z1*6.+c(1,3,
     & 2,n)*y1z2*6.+c(1,3,3,n)*y1z3*6.+c(1,3,4,n)*y1z4*6.+c(1,4,0,n)*
     & y2*12.+c(1,4,1,n)*y2z1*12.+c(1,4,2,n)*y2z2*12.+c(1,4,3,n)*y2z3*
     & 12.+c(1,4,4,n)*y2z4*12.+c(2,2,0,n)*x1*4.+c(2,2,1,n)*x1z1*4.+c(
     & 2,2,2,n)*x1z2*4.+c(2,2,3,n)*x1z3*4.+c(2,2,4,n)*x1z4*4.+c(2,3,0,
     & n)*x1y1*12.+c(2,3,1,n)*x1y1z1*12.+c(2,3,2,n)*x1y1z2*12.+c(2,3,
     & 3,n)*x1y1z3*12.+c(2,3,4,n)*x1y1z4*12.+c(2,4,0,n)*x1y2*24.+c(2,
     & 4,1,n)*x1y2z1*24.+c(2,4,2,n)*x1y2z2*24.+c(2,4,3,n)*x1y2z3*24.+
     & c(2,4,4,n)*x1y2z4*24.+c(3,2,0,n)*x2*6.+c(3,2,1,n)*x2z1*6.+c(3,
     & 2,2,n)*x2z2*6.+c(3,2,3,n)*x2z3*6.+c(3,2,4,n)*x2z4*6.+c(3,3,0,n)
     & *x2y1*18.+c(3,3,1,n)*x2y1z1*18.+c(3,3,2,n)*x2y1z2*18.+c(3,3,3,
     & n)*x2y1z3*18.+c(3,3,4,n)*x2y1z4*18.+c(3,4,0,n)*x2y2*36.+c(3,4,
     & 1,n)*x2y2z1*36.+c(3,4,2,n)*x2y2z2*36.+c(3,4,3,n)*x2y2z3*36.+c(
     & 3,4,4,n)*x2y2z4*36.+c(4,2,0,n)*x3*8.+c(4,2,1,n)*x3z1*8.+c(4,2,
     & 2,n)*x3z2*8.+c(4,2,3,n)*x3z3*8.+c(4,2,4,n)*x3z4*8.+c(4,3,0,n)*
     & x3y1*24.+c(4,3,1,n)*x3y1z1*24.+c(4,3,2,n)*x3y1z2*24.+c(4,3,3,n)
     & *x3y1z3*24.+c(4,3,4,n)*x3y1z4*24.+c(4,4,0,n)*x3y2*48.+c(4,4,1,
     & n)*x3y2z1*48.+c(4,4,2,n)*x3y2z2*48.+c(4,4,3,n)*x3y2z3*48.+c(4,
     & 4,4,n)*x3y2z4*48.)*time
! endLoops()
      else if( dx.eq.1.and.dy.eq.2.and.dz.eq.1.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
c 
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
c 
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
      r=(c(1,2,1,n)*2.+c(1,2,2,n)*z1*4.+c(1,2,3,n)*z2*6.+c(1,2,4,n)*z3*
     & 8.+c(1,3,1,n)*y1*6.+c(1,3,2,n)*y1z1*12.+c(1,3,3,n)*y1z2*18.+c(
     & 1,3,4,n)*y1z3*24.+c(1,4,1,n)*y2*12.+c(1,4,2,n)*y2z1*24.+c(1,4,
     & 3,n)*y2z2*36.+c(1,4,4,n)*y2z3*48.+c(2,2,1,n)*x1*4.+c(2,2,2,n)*
     & x1z1*8.+c(2,2,3,n)*x1z2*12.+c(2,2,4,n)*x1z3*16.+c(2,3,1,n)*
     & x1y1*12.+c(2,3,2,n)*x1y1z1*24.+c(2,3,3,n)*x1y1z2*36.+c(2,3,4,n)
     & *x1y1z3*48.+c(2,4,1,n)*x1y2*24.+c(2,4,2,n)*x1y2z1*48.+c(2,4,3,
     & n)*x1y2z2*72.+c(2,4,4,n)*x1y2z3*96.+c(3,2,1,n)*x2*6.+c(3,2,2,n)
     & *x2z1*12.+c(3,2,3,n)*x2z2*18.+c(3,2,4,n)*x2z3*24.+c(3,3,1,n)*
     & x2y1*18.+c(3,3,2,n)*x2y1z1*36.+c(3,3,3,n)*x2y1z2*54.+c(3,3,4,n)
     & *x2y1z3*72.+c(3,4,1,n)*x2y2*36.+c(3,4,2,n)*x2y2z1*72.+c(3,4,3,
     & n)*x2y2z2*108.+c(3,4,4,n)*x2y2z3*144.+c(4,2,1,n)*x3*8.+c(4,2,2,
     & n)*x3z1*16.+c(4,2,3,n)*x3z2*24.+c(4,2,4,n)*x3z3*32.+c(4,3,1,n)*
     & x3y1*24.+c(4,3,2,n)*x3y1z1*48.+c(4,3,3,n)*x3y1z2*72.+c(4,3,4,n)
     & *x3y1z3*96.+c(4,4,1,n)*x3y2*48.+c(4,4,2,n)*x3y2z1*96.+c(4,4,3,
     & n)*x3y2z2*144.+c(4,4,4,n)*x3y2z3*192.)*time
! endLoops()
      else if( dx.eq.1.and.dy.eq.2.and.dz.eq.2.and.dt.eq.0  )then
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
      z2=z1*z1
c 
      y1z1=y1*z1
      y1z2=y1z1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
c 
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
      r=(c(1,2,2,n)*4.+c(1,2,3,n)*z1*12.+c(1,2,4,n)*z2*24.+c(1,3,2,n)*
     & y1*12.+c(1,3,3,n)*y1z1*36.+c(1,3,4,n)*y1z2*72.+c(1,4,2,n)*y2*
     & 24.+c(1,4,3,n)*y2z1*72.+c(1,4,4,n)*y2z2*144.+c(2,2,2,n)*x1*8.+
     & c(2,2,3,n)*x1z1*24.+c(2,2,4,n)*x1z2*48.+c(2,3,2,n)*x1y1*24.+c(
     & 2,3,3,n)*x1y1z1*72.+c(2,3,4,n)*x1y1z2*144.+c(2,4,2,n)*x1y2*48.+
     & c(2,4,3,n)*x1y2z1*144.+c(2,4,4,n)*x1y2z2*288.+c(3,2,2,n)*x2*
     & 12.+c(3,2,3,n)*x2z1*36.+c(3,2,4,n)*x2z2*72.+c(3,3,2,n)*x2y1*
     & 36.+c(3,3,3,n)*x2y1z1*108.+c(3,3,4,n)*x2y1z2*216.+c(3,4,2,n)*
     & x2y2*72.+c(3,4,3,n)*x2y2z1*216.+c(3,4,4,n)*x2y2z2*432.+c(4,2,2,
     & n)*x3*16.+c(4,2,3,n)*x3z1*48.+c(4,2,4,n)*x3z2*96.+c(4,3,2,n)*
     & x3y1*48.+c(4,3,3,n)*x3y1z1*144.+c(4,3,4,n)*x3y1z2*288.+c(4,4,2,
     & n)*x3y2*96.+c(4,4,3,n)*x3y2z1*288.+c(4,4,4,n)*x3y2z2*576.)*time
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
c 
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
c 
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
      r=(c(2,0,0,n)*2.+c(2,0,1,n)*z1*2.+c(2,0,2,n)*z2*2.+c(2,0,3,n)*z3*
     & 2.+c(2,0,4,n)*z4*2.+c(2,1,0,n)*y1*2.+c(2,1,1,n)*y1z1*2.+c(2,1,
     & 2,n)*y1z2*2.+c(2,1,3,n)*y1z3*2.+c(2,1,4,n)*y1z4*2.+c(2,2,0,n)*
     & y2*2.+c(2,2,1,n)*y2z1*2.+c(2,2,2,n)*y2z2*2.+c(2,2,3,n)*y2z3*2.+
     & c(2,2,4,n)*y2z4*2.+c(2,3,0,n)*y3*2.+c(2,3,1,n)*y3z1*2.+c(2,3,2,
     & n)*y3z2*2.+c(2,3,3,n)*y3z3*2.+c(2,3,4,n)*y3z4*2.+c(2,4,0,n)*y4*
     & 2.+c(2,4,1,n)*y4z1*2.+c(2,4,2,n)*y4z2*2.+c(2,4,3,n)*y4z3*2.+c(
     & 2,4,4,n)*y4z4*2.+c(3,0,0,n)*x1*6.+c(3,0,1,n)*x1z1*6.+c(3,0,2,n)
     & *x1z2*6.+c(3,0,3,n)*x1z3*6.+c(3,0,4,n)*x1z4*6.+c(3,1,0,n)*x1y1*
     & 6.+c(3,1,1,n)*x1y1z1*6.+c(3,1,2,n)*x1y1z2*6.+c(3,1,3,n)*x1y1z3*
     & 6.+c(3,1,4,n)*x1y1z4*6.+c(3,2,0,n)*x1y2*6.+c(3,2,1,n)*x1y2z1*
     & 6.+c(3,2,2,n)*x1y2z2*6.+c(3,2,3,n)*x1y2z3*6.+c(3,2,4,n)*x1y2z4*
     & 6.+c(3,3,0,n)*x1y3*6.+c(3,3,1,n)*x1y3z1*6.+c(3,3,2,n)*x1y3z2*
     & 6.+c(3,3,3,n)*x1y3z3*6.+c(3,3,4,n)*x1y3z4*6.+c(3,4,0,n)*x1y4*
     & 6.+c(3,4,1,n)*x1y4z1*6.+c(3,4,2,n)*x1y4z2*6.+c(3,4,3,n)*x1y4z3*
     & 6.+c(3,4,4,n)*x1y4z4*6.+c(4,0,0,n)*x2*12.+c(4,0,1,n)*x2z1*12.+
     & c(4,0,2,n)*x2z2*12.+c(4,0,3,n)*x2z3*12.+c(4,0,4,n)*x2z4*12.+c(
     & 4,1,0,n)*x2y1*12.+c(4,1,1,n)*x2y1z1*12.+c(4,1,2,n)*x2y1z2*12.+
     & c(4,1,3,n)*x2y1z3*12.+c(4,1,4,n)*x2y1z4*12.+c(4,2,0,n)*x2y2*
     & 12.+c(4,2,1,n)*x2y2z1*12.+c(4,2,2,n)*x2y2z2*12.+c(4,2,3,n)*
     & x2y2z3*12.+c(4,2,4,n)*x2y2z4*12.+c(4,3,0,n)*x2y3*12.+c(4,3,1,n)
     & *x2y3z1*12.+c(4,3,2,n)*x2y3z2*12.+c(4,3,3,n)*x2y3z3*12.+c(4,3,
     & 4,n)*x2y3z4*12.+c(4,4,0,n)*x2y4*12.+c(4,4,1,n)*x2y4z1*12.+c(4,
     & 4,2,n)*x2y4z2*12.+c(4,4,3,n)*x2y4z3*12.+c(4,4,4,n)*x2y4z4*12.)*
     & time
! endLoops()
      else if( dx.eq.2.and.dy.eq.0.and.dz.eq.1.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
c 
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
c 
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
      r=(c(2,0,1,n)*2.+c(2,0,2,n)*z1*4.+c(2,0,3,n)*z2*6.+c(2,0,4,n)*z3*
     & 8.+c(2,1,1,n)*y1*2.+c(2,1,2,n)*y1z1*4.+c(2,1,3,n)*y1z2*6.+c(2,
     & 1,4,n)*y1z3*8.+c(2,2,1,n)*y2*2.+c(2,2,2,n)*y2z1*4.+c(2,2,3,n)*
     & y2z2*6.+c(2,2,4,n)*y2z3*8.+c(2,3,1,n)*y3*2.+c(2,3,2,n)*y3z1*4.+
     & c(2,3,3,n)*y3z2*6.+c(2,3,4,n)*y3z3*8.+c(2,4,1,n)*y4*2.+c(2,4,2,
     & n)*y4z1*4.+c(2,4,3,n)*y4z2*6.+c(2,4,4,n)*y4z3*8.+c(3,0,1,n)*x1*
     & 6.+c(3,0,2,n)*x1z1*12.+c(3,0,3,n)*x1z2*18.+c(3,0,4,n)*x1z3*24.+
     & c(3,1,1,n)*x1y1*6.+c(3,1,2,n)*x1y1z1*12.+c(3,1,3,n)*x1y1z2*18.+
     & c(3,1,4,n)*x1y1z3*24.+c(3,2,1,n)*x1y2*6.+c(3,2,2,n)*x1y2z1*12.+
     & c(3,2,3,n)*x1y2z2*18.+c(3,2,4,n)*x1y2z3*24.+c(3,3,1,n)*x1y3*6.+
     & c(3,3,2,n)*x1y3z1*12.+c(3,3,3,n)*x1y3z2*18.+c(3,3,4,n)*x1y3z3*
     & 24.+c(3,4,1,n)*x1y4*6.+c(3,4,2,n)*x1y4z1*12.+c(3,4,3,n)*x1y4z2*
     & 18.+c(3,4,4,n)*x1y4z3*24.+c(4,0,1,n)*x2*12.+c(4,0,2,n)*x2z1*
     & 24.+c(4,0,3,n)*x2z2*36.+c(4,0,4,n)*x2z3*48.+c(4,1,1,n)*x2y1*
     & 12.+c(4,1,2,n)*x2y1z1*24.+c(4,1,3,n)*x2y1z2*36.+c(4,1,4,n)*
     & x2y1z3*48.+c(4,2,1,n)*x2y2*12.+c(4,2,2,n)*x2y2z1*24.+c(4,2,3,n)
     & *x2y2z2*36.+c(4,2,4,n)*x2y2z3*48.+c(4,3,1,n)*x2y3*12.+c(4,3,2,
     & n)*x2y3z1*24.+c(4,3,3,n)*x2y3z2*36.+c(4,3,4,n)*x2y3z3*48.+c(4,
     & 4,1,n)*x2y4*12.+c(4,4,2,n)*x2y4z1*24.+c(4,4,3,n)*x2y4z2*36.+c(
     & 4,4,4,n)*x2y4z3*48.)*time
! endLoops()
      else if( dx.eq.2.and.dy.eq.0.and.dz.eq.2.and.dt.eq.0  )then
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
      z2=z1*z1
c 
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
c 
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
      r=(c(2,0,2,n)*4.+c(2,0,3,n)*z1*12.+c(2,0,4,n)*z2*24.+c(2,1,2,n)*
     & y1*4.+c(2,1,3,n)*y1z1*12.+c(2,1,4,n)*y1z2*24.+c(2,2,2,n)*y2*4.+
     & c(2,2,3,n)*y2z1*12.+c(2,2,4,n)*y2z2*24.+c(2,3,2,n)*y3*4.+c(2,3,
     & 3,n)*y3z1*12.+c(2,3,4,n)*y3z2*24.+c(2,4,2,n)*y4*4.+c(2,4,3,n)*
     & y4z1*12.+c(2,4,4,n)*y4z2*24.+c(3,0,2,n)*x1*12.+c(3,0,3,n)*x1z1*
     & 36.+c(3,0,4,n)*x1z2*72.+c(3,1,2,n)*x1y1*12.+c(3,1,3,n)*x1y1z1*
     & 36.+c(3,1,4,n)*x1y1z2*72.+c(3,2,2,n)*x1y2*12.+c(3,2,3,n)*
     & x1y2z1*36.+c(3,2,4,n)*x1y2z2*72.+c(3,3,2,n)*x1y3*12.+c(3,3,3,n)
     & *x1y3z1*36.+c(3,3,4,n)*x1y3z2*72.+c(3,4,2,n)*x1y4*12.+c(3,4,3,
     & n)*x1y4z1*36.+c(3,4,4,n)*x1y4z2*72.+c(4,0,2,n)*x2*24.+c(4,0,3,
     & n)*x2z1*72.+c(4,0,4,n)*x2z2*144.+c(4,1,2,n)*x2y1*24.+c(4,1,3,n)
     & *x2y1z1*72.+c(4,1,4,n)*x2y1z2*144.+c(4,2,2,n)*x2y2*24.+c(4,2,3,
     & n)*x2y2z1*72.+c(4,2,4,n)*x2y2z2*144.+c(4,3,2,n)*x2y3*24.+c(4,3,
     & 3,n)*x2y3z1*72.+c(4,3,4,n)*x2y3z2*144.+c(4,4,2,n)*x2y4*24.+c(4,
     & 4,3,n)*x2y4z1*72.+c(4,4,4,n)*x2y4z2*144.)*time
! endLoops()
      else if( dx.eq.2.and.dy.eq.1.and.dz.eq.0.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
c 
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
c 
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
      r=(c(2,1,0,n)*2.+c(2,1,1,n)*z1*2.+c(2,1,2,n)*z2*2.+c(2,1,3,n)*z3*
     & 2.+c(2,1,4,n)*z4*2.+c(2,2,0,n)*y1*4.+c(2,2,1,n)*y1z1*4.+c(2,2,
     & 2,n)*y1z2*4.+c(2,2,3,n)*y1z3*4.+c(2,2,4,n)*y1z4*4.+c(2,3,0,n)*
     & y2*6.+c(2,3,1,n)*y2z1*6.+c(2,3,2,n)*y2z2*6.+c(2,3,3,n)*y2z3*6.+
     & c(2,3,4,n)*y2z4*6.+c(2,4,0,n)*y3*8.+c(2,4,1,n)*y3z1*8.+c(2,4,2,
     & n)*y3z2*8.+c(2,4,3,n)*y3z3*8.+c(2,4,4,n)*y3z4*8.+c(3,1,0,n)*x1*
     & 6.+c(3,1,1,n)*x1z1*6.+c(3,1,2,n)*x1z2*6.+c(3,1,3,n)*x1z3*6.+c(
     & 3,1,4,n)*x1z4*6.+c(3,2,0,n)*x1y1*12.+c(3,2,1,n)*x1y1z1*12.+c(3,
     & 2,2,n)*x1y1z2*12.+c(3,2,3,n)*x1y1z3*12.+c(3,2,4,n)*x1y1z4*12.+
     & c(3,3,0,n)*x1y2*18.+c(3,3,1,n)*x1y2z1*18.+c(3,3,2,n)*x1y2z2*
     & 18.+c(3,3,3,n)*x1y2z3*18.+c(3,3,4,n)*x1y2z4*18.+c(3,4,0,n)*
     & x1y3*24.+c(3,4,1,n)*x1y3z1*24.+c(3,4,2,n)*x1y3z2*24.+c(3,4,3,n)
     & *x1y3z3*24.+c(3,4,4,n)*x1y3z4*24.+c(4,1,0,n)*x2*12.+c(4,1,1,n)*
     & x2z1*12.+c(4,1,2,n)*x2z2*12.+c(4,1,3,n)*x2z3*12.+c(4,1,4,n)*
     & x2z4*12.+c(4,2,0,n)*x2y1*24.+c(4,2,1,n)*x2y1z1*24.+c(4,2,2,n)*
     & x2y1z2*24.+c(4,2,3,n)*x2y1z3*24.+c(4,2,4,n)*x2y1z4*24.+c(4,3,0,
     & n)*x2y2*36.+c(4,3,1,n)*x2y2z1*36.+c(4,3,2,n)*x2y2z2*36.+c(4,3,
     & 3,n)*x2y2z3*36.+c(4,3,4,n)*x2y2z4*36.+c(4,4,0,n)*x2y3*48.+c(4,
     & 4,1,n)*x2y3z1*48.+c(4,4,2,n)*x2y3z2*48.+c(4,4,3,n)*x2y3z3*48.+
     & c(4,4,4,n)*x2y3z4*48.)*time
! endLoops()
      else if( dx.eq.2.and.dy.eq.1.and.dz.eq.1.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
c 
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
c 
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
      r=(c(2,1,1,n)*2.+c(2,1,2,n)*z1*4.+c(2,1,3,n)*z2*6.+c(2,1,4,n)*z3*
     & 8.+c(2,2,1,n)*y1*4.+c(2,2,2,n)*y1z1*8.+c(2,2,3,n)*y1z2*12.+c(2,
     & 2,4,n)*y1z3*16.+c(2,3,1,n)*y2*6.+c(2,3,2,n)*y2z1*12.+c(2,3,3,n)
     & *y2z2*18.+c(2,3,4,n)*y2z3*24.+c(2,4,1,n)*y3*8.+c(2,4,2,n)*y3z1*
     & 16.+c(2,4,3,n)*y3z2*24.+c(2,4,4,n)*y3z3*32.+c(3,1,1,n)*x1*6.+c(
     & 3,1,2,n)*x1z1*12.+c(3,1,3,n)*x1z2*18.+c(3,1,4,n)*x1z3*24.+c(3,
     & 2,1,n)*x1y1*12.+c(3,2,2,n)*x1y1z1*24.+c(3,2,3,n)*x1y1z2*36.+c(
     & 3,2,4,n)*x1y1z3*48.+c(3,3,1,n)*x1y2*18.+c(3,3,2,n)*x1y2z1*36.+
     & c(3,3,3,n)*x1y2z2*54.+c(3,3,4,n)*x1y2z3*72.+c(3,4,1,n)*x1y3*
     & 24.+c(3,4,2,n)*x1y3z1*48.+c(3,4,3,n)*x1y3z2*72.+c(3,4,4,n)*
     & x1y3z3*96.+c(4,1,1,n)*x2*12.+c(4,1,2,n)*x2z1*24.+c(4,1,3,n)*
     & x2z2*36.+c(4,1,4,n)*x2z3*48.+c(4,2,1,n)*x2y1*24.+c(4,2,2,n)*
     & x2y1z1*48.+c(4,2,3,n)*x2y1z2*72.+c(4,2,4,n)*x2y1z3*96.+c(4,3,1,
     & n)*x2y2*36.+c(4,3,2,n)*x2y2z1*72.+c(4,3,3,n)*x2y2z2*108.+c(4,3,
     & 4,n)*x2y2z3*144.+c(4,4,1,n)*x2y3*48.+c(4,4,2,n)*x2y3z1*96.+c(4,
     & 4,3,n)*x2y3z2*144.+c(4,4,4,n)*x2y3z3*192.)*time
! endLoops()
      else if( dx.eq.2.and.dy.eq.1.and.dz.eq.2.and.dt.eq.0  )then
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
      z2=z1*z1
c 
      y1z1=y1*z1
      y1z2=y1z1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
c 
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
      r=(c(2,1,2,n)*4.+c(2,1,3,n)*z1*12.+c(2,1,4,n)*z2*24.+c(2,2,2,n)*
     & y1*8.+c(2,2,3,n)*y1z1*24.+c(2,2,4,n)*y1z2*48.+c(2,3,2,n)*y2*
     & 12.+c(2,3,3,n)*y2z1*36.+c(2,3,4,n)*y2z2*72.+c(2,4,2,n)*y3*16.+
     & c(2,4,3,n)*y3z1*48.+c(2,4,4,n)*y3z2*96.+c(3,1,2,n)*x1*12.+c(3,
     & 1,3,n)*x1z1*36.+c(3,1,4,n)*x1z2*72.+c(3,2,2,n)*x1y1*24.+c(3,2,
     & 3,n)*x1y1z1*72.+c(3,2,4,n)*x1y1z2*144.+c(3,3,2,n)*x1y2*36.+c(3,
     & 3,3,n)*x1y2z1*108.+c(3,3,4,n)*x1y2z2*216.+c(3,4,2,n)*x1y3*48.+
     & c(3,4,3,n)*x1y3z1*144.+c(3,4,4,n)*x1y3z2*288.+c(4,1,2,n)*x2*
     & 24.+c(4,1,3,n)*x2z1*72.+c(4,1,4,n)*x2z2*144.+c(4,2,2,n)*x2y1*
     & 48.+c(4,2,3,n)*x2y1z1*144.+c(4,2,4,n)*x2y1z2*288.+c(4,3,2,n)*
     & x2y2*72.+c(4,3,3,n)*x2y2z1*216.+c(4,3,4,n)*x2y2z2*432.+c(4,4,2,
     & n)*x2y3*96.+c(4,4,3,n)*x2y3z1*288.+c(4,4,4,n)*x2y3z2*576.)*time
! endLoops()
      else if( dx.eq.2.and.dy.eq.2.and.dz.eq.0.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
c 
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
c 
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
      r=(c(2,2,0,n)*4.+c(2,2,1,n)*z1*4.+c(2,2,2,n)*z2*4.+c(2,2,3,n)*z3*
     & 4.+c(2,2,4,n)*z4*4.+c(2,3,0,n)*y1*12.+c(2,3,1,n)*y1z1*12.+c(2,
     & 3,2,n)*y1z2*12.+c(2,3,3,n)*y1z3*12.+c(2,3,4,n)*y1z4*12.+c(2,4,
     & 0,n)*y2*24.+c(2,4,1,n)*y2z1*24.+c(2,4,2,n)*y2z2*24.+c(2,4,3,n)*
     & y2z3*24.+c(2,4,4,n)*y2z4*24.+c(3,2,0,n)*x1*12.+c(3,2,1,n)*x1z1*
     & 12.+c(3,2,2,n)*x1z2*12.+c(3,2,3,n)*x1z3*12.+c(3,2,4,n)*x1z4*
     & 12.+c(3,3,0,n)*x1y1*36.+c(3,3,1,n)*x1y1z1*36.+c(3,3,2,n)*
     & x1y1z2*36.+c(3,3,3,n)*x1y1z3*36.+c(3,3,4,n)*x1y1z4*36.+c(3,4,0,
     & n)*x1y2*72.+c(3,4,1,n)*x1y2z1*72.+c(3,4,2,n)*x1y2z2*72.+c(3,4,
     & 3,n)*x1y2z3*72.+c(3,4,4,n)*x1y2z4*72.+c(4,2,0,n)*x2*24.+c(4,2,
     & 1,n)*x2z1*24.+c(4,2,2,n)*x2z2*24.+c(4,2,3,n)*x2z3*24.+c(4,2,4,
     & n)*x2z4*24.+c(4,3,0,n)*x2y1*72.+c(4,3,1,n)*x2y1z1*72.+c(4,3,2,
     & n)*x2y1z2*72.+c(4,3,3,n)*x2y1z3*72.+c(4,3,4,n)*x2y1z4*72.+c(4,
     & 4,0,n)*x2y2*144.+c(4,4,1,n)*x2y2z1*144.+c(4,4,2,n)*x2y2z2*144.+
     & c(4,4,3,n)*x2y2z3*144.+c(4,4,4,n)*x2y2z4*144.)*time
! endLoops()
      else if( dx.eq.2.and.dy.eq.2.and.dz.eq.1.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
c 
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
c 
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
      r=(c(2,2,1,n)*4.+c(2,2,2,n)*z1*8.+c(2,2,3,n)*z2*12.+c(2,2,4,n)*
     & z3*16.+c(2,3,1,n)*y1*12.+c(2,3,2,n)*y1z1*24.+c(2,3,3,n)*y1z2*
     & 36.+c(2,3,4,n)*y1z3*48.+c(2,4,1,n)*y2*24.+c(2,4,2,n)*y2z1*48.+
     & c(2,4,3,n)*y2z2*72.+c(2,4,4,n)*y2z3*96.+c(3,2,1,n)*x1*12.+c(3,
     & 2,2,n)*x1z1*24.+c(3,2,3,n)*x1z2*36.+c(3,2,4,n)*x1z3*48.+c(3,3,
     & 1,n)*x1y1*36.+c(3,3,2,n)*x1y1z1*72.+c(3,3,3,n)*x1y1z2*108.+c(3,
     & 3,4,n)*x1y1z3*144.+c(3,4,1,n)*x1y2*72.+c(3,4,2,n)*x1y2z1*144.+
     & c(3,4,3,n)*x1y2z2*216.+c(3,4,4,n)*x1y2z3*288.+c(4,2,1,n)*x2*
     & 24.+c(4,2,2,n)*x2z1*48.+c(4,2,3,n)*x2z2*72.+c(4,2,4,n)*x2z3*
     & 96.+c(4,3,1,n)*x2y1*72.+c(4,3,2,n)*x2y1z1*144.+c(4,3,3,n)*
     & x2y1z2*216.+c(4,3,4,n)*x2y1z3*288.+c(4,4,1,n)*x2y2*144.+c(4,4,
     & 2,n)*x2y2z1*288.+c(4,4,3,n)*x2y2z2*432.+c(4,4,4,n)*x2y2z3*576.)
     & *time
! endLoops()
      else if( dx.eq.2.and.dy.eq.2.and.dz.eq.2.and.dt.eq.0  )then
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
      z2=z1*z1
c 
      y1z1=y1*z1
      y1z2=y1z1*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
c 
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
      r=(c(2,2,2,n)*8.+c(2,2,3,n)*z1*24.+c(2,2,4,n)*z2*48.+c(2,3,2,n)*
     & y1*24.+c(2,3,3,n)*y1z1*72.+c(2,3,4,n)*y1z2*144.+c(2,4,2,n)*y2*
     & 48.+c(2,4,3,n)*y2z1*144.+c(2,4,4,n)*y2z2*288.+c(3,2,2,n)*x1*
     & 24.+c(3,2,3,n)*x1z1*72.+c(3,2,4,n)*x1z2*144.+c(3,3,2,n)*x1y1*
     & 72.+c(3,3,3,n)*x1y1z1*216.+c(3,3,4,n)*x1y1z2*432.+c(3,4,2,n)*
     & x1y2*144.+c(3,4,3,n)*x1y2z1*432.+c(3,4,4,n)*x1y2z2*864.+c(4,2,
     & 2,n)*x2*48.+c(4,2,3,n)*x2z1*144.+c(4,2,4,n)*x2z2*288.+c(4,3,2,
     & n)*x2y1*144.+c(4,3,3,n)*x2y1z1*432.+c(4,3,4,n)*x2y1z2*864.+c(4,
     & 4,2,n)*x2y2*288.+c(4,4,3,n)*x2y2z1*864.+c(4,4,4,n)*x2y2z2*
     & 1728.)*time
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
c 
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
c 
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
      r=(c(3,0,0,n)*6.+c(3,0,1,n)*z1*6.+c(3,0,2,n)*z2*6.+c(3,0,3,n)*z3*
     & 6.+c(3,0,4,n)*z4*6.+c(3,1,0,n)*y1*6.+c(3,1,1,n)*y1z1*6.+c(3,1,
     & 2,n)*y1z2*6.+c(3,1,3,n)*y1z3*6.+c(3,1,4,n)*y1z4*6.+c(3,2,0,n)*
     & y2*6.+c(3,2,1,n)*y2z1*6.+c(3,2,2,n)*y2z2*6.+c(3,2,3,n)*y2z3*6.+
     & c(3,2,4,n)*y2z4*6.+c(3,3,0,n)*y3*6.+c(3,3,1,n)*y3z1*6.+c(3,3,2,
     & n)*y3z2*6.+c(3,3,3,n)*y3z3*6.+c(3,3,4,n)*y3z4*6.+c(3,4,0,n)*y4*
     & 6.+c(3,4,1,n)*y4z1*6.+c(3,4,2,n)*y4z2*6.+c(3,4,3,n)*y4z3*6.+c(
     & 3,4,4,n)*y4z4*6.+c(4,0,0,n)*x1*24.+c(4,0,1,n)*x1z1*24.+c(4,0,2,
     & n)*x1z2*24.+c(4,0,3,n)*x1z3*24.+c(4,0,4,n)*x1z4*24.+c(4,1,0,n)*
     & x1y1*24.+c(4,1,1,n)*x1y1z1*24.+c(4,1,2,n)*x1y1z2*24.+c(4,1,3,n)
     & *x1y1z3*24.+c(4,1,4,n)*x1y1z4*24.+c(4,2,0,n)*x1y2*24.+c(4,2,1,
     & n)*x1y2z1*24.+c(4,2,2,n)*x1y2z2*24.+c(4,2,3,n)*x1y2z3*24.+c(4,
     & 2,4,n)*x1y2z4*24.+c(4,3,0,n)*x1y3*24.+c(4,3,1,n)*x1y3z1*24.+c(
     & 4,3,2,n)*x1y3z2*24.+c(4,3,3,n)*x1y3z3*24.+c(4,3,4,n)*x1y3z4*
     & 24.+c(4,4,0,n)*x1y4*24.+c(4,4,1,n)*x1y4z1*24.+c(4,4,2,n)*
     & x1y4z2*24.+c(4,4,3,n)*x1y4z3*24.+c(4,4,4,n)*x1y4z4*24.)*time
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
c 
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
      r=(c(4,0,0,n)*24.+c(4,0,1,n)*z1*24.+c(4,0,2,n)*z2*24.+c(4,0,3,n)*
     & z3*24.+c(4,0,4,n)*z4*24.+c(4,1,0,n)*y1*24.+c(4,1,1,n)*y1z1*24.+
     & c(4,1,2,n)*y1z2*24.+c(4,1,3,n)*y1z3*24.+c(4,1,4,n)*y1z4*24.+c(
     & 4,2,0,n)*y2*24.+c(4,2,1,n)*y2z1*24.+c(4,2,2,n)*y2z2*24.+c(4,2,
     & 3,n)*y2z3*24.+c(4,2,4,n)*y2z4*24.+c(4,3,0,n)*y3*24.+c(4,3,1,n)*
     & y3z1*24.+c(4,3,2,n)*y3z2*24.+c(4,3,3,n)*y3z3*24.+c(4,3,4,n)*
     & y3z4*24.+c(4,4,0,n)*y4*24.+c(4,4,1,n)*y4z1*24.+c(4,4,2,n)*y4z2*
     & 24.+c(4,4,3,n)*y4z3*24.+c(4,4,4,n)*y4z4*24.)*time
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
      z2=z1*z1
c
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
c
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
      r=(+c(0,0,2,n)*2.+c(0,0,3,n)*z1*6.+c(0,0,4,n)*z2*12.+c(0,1,2,n)*
     & y1*2.+c(0,1,3,n)*y1z1*6.+c(0,1,4,n)*y1z2*12.+c(0,2,2,n)*y2*2.+
     & c(0,2,3,n)*y2z1*6.+c(0,2,4,n)*y2z2*12.+c(0,3,2,n)*y3*2.+c(0,3,
     & 3,n)*y3z1*6.+c(0,3,4,n)*y3z2*12.+c(0,4,2,n)*y4*2.+c(0,4,3,n)*
     & y4z1*6.+c(0,4,4,n)*y4z2*12.+c(1,0,2,n)*x1*2.+c(1,0,3,n)*x1z1*
     & 6.+c(1,0,4,n)*x1z2*12.+c(1,1,2,n)*x1y1*2.+c(1,1,3,n)*x1y1z1*6.+
     & c(1,1,4,n)*x1y1z2*12.+c(1,2,2,n)*x1y2*2.+c(1,2,3,n)*x1y2z1*6.+
     & c(1,2,4,n)*x1y2z2*12.+c(1,3,2,n)*x1y3*2.+c(1,3,3,n)*x1y3z1*6.+
     & c(1,3,4,n)*x1y3z2*12.+c(1,4,2,n)*x1y4*2.+c(1,4,3,n)*x1y4z1*6.+
     & c(1,4,4,n)*x1y4z2*12.+c(2,0,2,n)*x2*2.+c(2,0,3,n)*x2z1*6.+c(2,
     & 0,4,n)*x2z2*12.+c(2,1,2,n)*x2y1*2.+c(2,1,3,n)*x2y1z1*6.+c(2,1,
     & 4,n)*x2y1z2*12.+c(2,2,2,n)*x2y2*2.+c(2,2,3,n)*x2y2z1*6.+c(2,2,
     & 4,n)*x2y2z2*12.+c(2,3,2,n)*x2y3*2.+c(2,3,3,n)*x2y3z1*6.+c(2,3,
     & 4,n)*x2y3z2*12.+c(2,4,2,n)*x2y4*2.+c(2,4,3,n)*x2y4z1*6.+c(2,4,
     & 4,n)*x2y4z2*12.+c(3,0,2,n)*x3*2.+c(3,0,3,n)*x3z1*6.+c(3,0,4,n)*
     & x3z2*12.+c(3,1,2,n)*x3y1*2.+c(3,1,3,n)*x3y1z1*6.+c(3,1,4,n)*
     & x3y1z2*12.+c(3,2,2,n)*x3y2*2.+c(3,2,3,n)*x3y2z1*6.+c(3,2,4,n)*
     & x3y2z2*12.+c(3,3,2,n)*x3y3*2.+c(3,3,3,n)*x3y3z1*6.+c(3,3,4,n)*
     & x3y3z2*12.+c(3,4,2,n)*x3y4*2.+c(3,4,3,n)*x3y4z1*6.+c(3,4,4,n)*
     & x3y4z2*12.+c(4,0,2,n)*x4*2.+c(4,0,3,n)*x4z1*6.+c(4,0,4,n)*x4z2*
     & 12.+c(4,1,2,n)*x4y1*2.+c(4,1,3,n)*x4y1z1*6.+c(4,1,4,n)*x4y1z2*
     & 12.+c(4,2,2,n)*x4y2*2.+c(4,2,3,n)*x4y2z1*6.+c(4,2,4,n)*x4y2z2*
     & 12.+c(4,3,2,n)*x4y3*2.+c(4,3,3,n)*x4y3z1*6.+c(4,3,4,n)*x4y3z2*
     & 12.+c(4,4,2,n)*x4y4*2.+c(4,4,3,n)*x4y4z1*6.+c(4,4,4,n)*x4y4z2*
     & 12.+c(0,2,0,n)*2.+c(0,2,1,n)*z1*2.+c(0,2,2,n)*z2*2.+c(0,2,3,n)*
     & z3*2.+c(0,2,4,n)*z4*2.+c(0,3,0,n)*y1*6.+c(0,3,1,n)*y1z1*6.+c(0,
     & 3,2,n)*y1z2*6.+c(0,3,3,n)*y1z3*6.+c(0,3,4,n)*y1z4*6.+c(0,4,0,n)
     & *y2*12.+c(0,4,1,n)*y2z1*12.+c(0,4,2,n)*y2z2*12.+c(0,4,3,n)*
     & y2z3*12.+c(0,4,4,n)*y2z4*12.+c(1,2,0,n)*x1*2.+c(1,2,1,n)*x1z1*
     & 2.+c(1,2,2,n)*x1z2*2.+c(1,2,3,n)*x1z3*2.+c(1,2,4,n)*x1z4*2.+c(
     & 1,3,0,n)*x1y1*6.+c(1,3,1,n)*x1y1z1*6.+c(1,3,2,n)*x1y1z2*6.+c(1,
     & 3,3,n)*x1y1z3*6.+c(1,3,4,n)*x1y1z4*6.+c(1,4,0,n)*x1y2*12.+c(1,
     & 4,1,n)*x1y2z1*12.+c(1,4,2,n)*x1y2z2*12.+c(1,4,3,n)*x1y2z3*12.+
     & c(1,4,4,n)*x1y2z4*12.+c(2,2,0,n)*x2*2.+c(2,2,1,n)*x2z1*2.+c(2,
     & 2,2,n)*x2z2*2.+c(2,2,3,n)*x2z3*2.+c(2,2,4,n)*x2z4*2.+c(2,3,0,n)
     & *x2y1*6.+c(2,3,1,n)*x2y1z1*6.+c(2,3,2,n)*x2y1z2*6.+c(2,3,3,n)*
     & x2y1z3*6.+c(2,3,4,n)*x2y1z4*6.+c(2,4,0,n)*x2y2*12.+c(2,4,1,n)*
     & x2y2z1*12.+c(2,4,2,n)*x2y2z2*12.+c(2,4,3,n)*x2y2z3*12.+c(2,4,4,
     & n)*x2y2z4*12.+c(3,2,0,n)*x3*2.+c(3,2,1,n)*x3z1*2.+c(3,2,2,n)*
     & x3z2*2.+c(3,2,3,n)*x3z3*2.+c(3,2,4,n)*x3z4*2.+c(3,3,0,n)*x3y1*
     & 6.+c(3,3,1,n)*x3y1z1*6.+c(3,3,2,n)*x3y1z2*6.+c(3,3,3,n)*x3y1z3*
     & 6.+c(3,3,4,n)*x3y1z4*6.+c(3,4,0,n)*x3y2*12.+c(3,4,1,n)*x3y2z1*
     & 12.+c(3,4,2,n)*x3y2z2*12.+c(3,4,3,n)*x3y2z3*12.+c(3,4,4,n)*
     & x3y2z4*12.+c(4,2,0,n)*x4*2.+c(4,2,1,n)*x4z1*2.+c(4,2,2,n)*x4z2*
     & 2.+c(4,2,3,n)*x4z3*2.+c(4,2,4,n)*x4z4*2.+c(4,3,0,n)*x4y1*6.+c(
     & 4,3,1,n)*x4y1z1*6.+c(4,3,2,n)*x4y1z2*6.+c(4,3,3,n)*x4y1z3*6.+c(
     & 4,3,4,n)*x4y1z4*6.+c(4,4,0,n)*x4y2*12.+c(4,4,1,n)*x4y2z1*12.+c(
     & 4,4,2,n)*x4y2z2*12.+c(4,4,3,n)*x4y2z3*12.+c(4,4,4,n)*x4y2z4*
     & 12.+c(2,0,0,n)*2.+c(2,0,1,n)*z1*2.+c(2,0,2,n)*z2*2.+c(2,0,3,n)*
     & z3*2.+c(2,0,4,n)*z4*2.+c(2,1,0,n)*y1*2.+c(2,1,1,n)*y1z1*2.+c(2,
     & 1,2,n)*y1z2*2.+c(2,1,3,n)*y1z3*2.+c(2,1,4,n)*y1z4*2.+c(2,2,0,n)
     & *y2*2.+c(2,2,1,n)*y2z1*2.+c(2,2,2,n)*y2z2*2.+c(2,2,3,n)*y2z3*
     & 2.+c(2,2,4,n)*y2z4*2.+c(2,3,0,n)*y3*2.+c(2,3,1,n)*y3z1*2.+c(2,
     & 3,2,n)*y3z2*2.+c(2,3,3,n)*y3z3*2.+c(2,3,4,n)*y3z4*2.+c(2,4,0,n)
     & *y4*2.+c(2,4,1,n)*y4z1*2.+c(2,4,2,n)*y4z2*2.+c(2,4,3,n)*y4z3*
     & 2.+c(2,4,4,n)*y4z4*2.+c(3,0,0,n)*x1*6.+c(3,0,1,n)*x1z1*6.+c(3,
     & 0,2,n)*x1z2*6.+c(3,0,3,n)*x1z3*6.+c(3,0,4,n)*x1z4*6.+c(3,1,0,n)
     & *x1y1*6.+c(3,1,1,n)*x1y1z1*6.+c(3,1,2,n)*x1y1z2*6.+c(3,1,3,n)*
     & x1y1z3*6.+c(3,1,4,n)*x1y1z4*6.+c(3,2,0,n)*x1y2*6.+c(3,2,1,n)*
     & x1y2z1*6.+c(3,2,2,n)*x1y2z2*6.+c(3,2,3,n)*x1y2z3*6.+c(3,2,4,n)*
     & x1y2z4*6.+c(3,3,0,n)*x1y3*6.+c(3,3,1,n)*x1y3z1*6.+c(3,3,2,n)*
     & x1y3z2*6.+c(3,3,3,n)*x1y3z3*6.+c(3,3,4,n)*x1y3z4*6.+c(3,4,0,n)*
     & x1y4*6.+c(3,4,1,n)*x1y4z1*6.+c(3,4,2,n)*x1y4z2*6.+c(3,4,3,n)*
     & x1y4z3*6.+c(3,4,4,n)*x1y4z4*6.+c(4,0,0,n)*x2*12.+c(4,0,1,n)*
     & x2z1*12.+c(4,0,2,n)*x2z2*12.+c(4,0,3,n)*x2z3*12.+c(4,0,4,n)*
     & x2z4*12.+c(4,1,0,n)*x2y1*12.+c(4,1,1,n)*x2y1z1*12.+c(4,1,2,n)*
     & x2y1z2*12.+c(4,1,3,n)*x2y1z3*12.+c(4,1,4,n)*x2y1z4*12.+c(4,2,0,
     & n)*x2y2*12.+c(4,2,1,n)*x2y2z1*12.+c(4,2,2,n)*x2y2z2*12.+c(4,2,
     & 3,n)*x2y2z3*12.+c(4,2,4,n)*x2y2z4*12.+c(4,3,0,n)*x2y3*12.+c(4,
     & 3,1,n)*x2y3z1*12.+c(4,3,2,n)*x2y3z2*12.+c(4,3,3,n)*x2y3z3*12.+
     & c(4,3,4,n)*x2y3z4*12.+c(4,4,0,n)*x2y4*12.+c(4,4,1,n)*x2y4z1*
     & 12.+c(4,4,2,n)*x2y4z2*12.+c(4,4,3,n)*x2y4z3*12.+c(4,4,4,n)*
     & x2y4z4*12.)*time
! endLoops()
      else
! beginLoops(time=0.)
      time=0.
      r=0.
! endLoops()
      end if
! #If "3D4" eq "3D5" || "3D4" eq "3D6"
      return
      end
! polyFun(3D5)
      subroutine polyEvaluate3D5 (r, x1,y1,z1,n,t, ndc1,ndc2,ndc3,nda, 
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
! #If "3D5" eq "3D5" || "3D5" eq "3D6"
#ifndef IFORT_COMPILER
! poly3D5(c,c,c,r)
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
c 
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
c 
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
      r=(c(0,0,0,n)+c(0,0,1,n)*z1+c(0,0,2,n)*z2+c(0,0,3,n)*z3+c(0,0,4,
     & n)*z4+c(0,0,5,n)*z5+c(0,1,0,n)*y1+c(0,1,1,n)*y1z1+c(0,1,2,n)*
     & y1z2+c(0,1,3,n)*y1z3+c(0,1,4,n)*y1z4+c(0,1,5,n)*y1z5+c(0,2,0,n)
     & *y2+c(0,2,1,n)*y2z1+c(0,2,2,n)*y2z2+c(0,2,3,n)*y2z3+c(0,2,4,n)*
     & y2z4+c(0,2,5,n)*y2z5+c(0,3,0,n)*y3+c(0,3,1,n)*y3z1+c(0,3,2,n)*
     & y3z2+c(0,3,3,n)*y3z3+c(0,3,4,n)*y3z4+c(0,3,5,n)*y3z5+c(0,4,0,n)
     & *y4+c(0,4,1,n)*y4z1+c(0,4,2,n)*y4z2+c(0,4,3,n)*y4z3+c(0,4,4,n)*
     & y4z4+c(0,4,5,n)*y4z5+c(0,5,0,n)*y5+c(0,5,1,n)*y5z1+c(0,5,2,n)*
     & y5z2+c(0,5,3,n)*y5z3+c(0,5,4,n)*y5z4+c(0,5,5,n)*y5z5+c(1,0,0,n)
     & *x1+c(1,0,1,n)*x1z1+c(1,0,2,n)*x1z2+c(1,0,3,n)*x1z3+c(1,0,4,n)*
     & x1z4+c(1,0,5,n)*x1z5+c(1,1,0,n)*x1y1+c(1,1,1,n)*x1y1z1+c(1,1,2,
     & n)*x1y1z2+c(1,1,3,n)*x1y1z3+c(1,1,4,n)*x1y1z4+c(1,1,5,n)*
     & x1y1z5+c(1,2,0,n)*x1y2+c(1,2,1,n)*x1y2z1+c(1,2,2,n)*x1y2z2+c(1,
     & 2,3,n)*x1y2z3+c(1,2,4,n)*x1y2z4+c(1,2,5,n)*x1y2z5+c(1,3,0,n)*
     & x1y3+c(1,3,1,n)*x1y3z1+c(1,3,2,n)*x1y3z2+c(1,3,3,n)*x1y3z3+c(1,
     & 3,4,n)*x1y3z4+c(1,3,5,n)*x1y3z5+c(1,4,0,n)*x1y4+c(1,4,1,n)*
     & x1y4z1+c(1,4,2,n)*x1y4z2+c(1,4,3,n)*x1y4z3+c(1,4,4,n)*x1y4z4+c(
     & 1,4,5,n)*x1y4z5+c(1,5,0,n)*x1y5+c(1,5,1,n)*x1y5z1+c(1,5,2,n)*
     & x1y5z2+c(1,5,3,n)*x1y5z3+c(1,5,4,n)*x1y5z4+c(1,5,5,n)*x1y5z5+c(
     & 2,0,0,n)*x2+c(2,0,1,n)*x2z1+c(2,0,2,n)*x2z2+c(2,0,3,n)*x2z3+c(
     & 2,0,4,n)*x2z4+c(2,0,5,n)*x2z5+c(2,1,0,n)*x2y1+c(2,1,1,n)*
     & x2y1z1+c(2,1,2,n)*x2y1z2+c(2,1,3,n)*x2y1z3+c(2,1,4,n)*x2y1z4+c(
     & 2,1,5,n)*x2y1z5+c(2,2,0,n)*x2y2+c(2,2,1,n)*x2y2z1+c(2,2,2,n)*
     & x2y2z2+c(2,2,3,n)*x2y2z3+c(2,2,4,n)*x2y2z4+c(2,2,5,n)*x2y2z5+c(
     & 2,3,0,n)*x2y3+c(2,3,1,n)*x2y3z1+c(2,3,2,n)*x2y3z2+c(2,3,3,n)*
     & x2y3z3+c(2,3,4,n)*x2y3z4+c(2,3,5,n)*x2y3z5+c(2,4,0,n)*x2y4+c(2,
     & 4,1,n)*x2y4z1+c(2,4,2,n)*x2y4z2+c(2,4,3,n)*x2y4z3+c(2,4,4,n)*
     & x2y4z4+c(2,4,5,n)*x2y4z5+c(2,5,0,n)*x2y5+c(2,5,1,n)*x2y5z1+c(2,
     & 5,2,n)*x2y5z2+c(2,5,3,n)*x2y5z3+c(2,5,4,n)*x2y5z4+c(2,5,5,n)*
     & x2y5z5+c(3,0,0,n)*x3+c(3,0,1,n)*x3z1+c(3,0,2,n)*x3z2+c(3,0,3,n)
     & *x3z3+c(3,0,4,n)*x3z4+c(3,0,5,n)*x3z5+c(3,1,0,n)*x3y1+c(3,1,1,
     & n)*x3y1z1+c(3,1,2,n)*x3y1z2+c(3,1,3,n)*x3y1z3+c(3,1,4,n)*
     & x3y1z4+c(3,1,5,n)*x3y1z5+c(3,2,0,n)*x3y2+c(3,2,1,n)*x3y2z1+c(3,
     & 2,2,n)*x3y2z2+c(3,2,3,n)*x3y2z3+c(3,2,4,n)*x3y2z4+c(3,2,5,n)*
     & x3y2z5+c(3,3,0,n)*x3y3+c(3,3,1,n)*x3y3z1+c(3,3,2,n)*x3y3z2+c(3,
     & 3,3,n)*x3y3z3+c(3,3,4,n)*x3y3z4+c(3,3,5,n)*x3y3z5+c(3,4,0,n)*
     & x3y4+c(3,4,1,n)*x3y4z1+c(3,4,2,n)*x3y4z2+c(3,4,3,n)*x3y4z3+c(3,
     & 4,4,n)*x3y4z4+c(3,4,5,n)*x3y4z5+c(3,5,0,n)*x3y5+c(3,5,1,n)*
     & x3y5z1+c(3,5,2,n)*x3y5z2+c(3,5,3,n)*x3y5z3+c(3,5,4,n)*x3y5z4+c(
     & 3,5,5,n)*x3y5z5+c(4,0,0,n)*x4+c(4,0,1,n)*x4z1+c(4,0,2,n)*x4z2+
     & c(4,0,3,n)*x4z3+c(4,0,4,n)*x4z4+c(4,0,5,n)*x4z5+c(4,1,0,n)*
     & x4y1+c(4,1,1,n)*x4y1z1+c(4,1,2,n)*x4y1z2+c(4,1,3,n)*x4y1z3+c(4,
     & 1,4,n)*x4y1z4+c(4,1,5,n)*x4y1z5+c(4,2,0,n)*x4y2+c(4,2,1,n)*
     & x4y2z1+c(4,2,2,n)*x4y2z2+c(4,2,3,n)*x4y2z3+c(4,2,4,n)*x4y2z4+c(
     & 4,2,5,n)*x4y2z5+c(4,3,0,n)*x4y3+c(4,3,1,n)*x4y3z1+c(4,3,2,n)*
     & x4y3z2+c(4,3,3,n)*x4y3z3+c(4,3,4,n)*x4y3z4+c(4,3,5,n)*x4y3z5+c(
     & 4,4,0,n)*x4y4+c(4,4,1,n)*x4y4z1+c(4,4,2,n)*x4y4z2+c(4,4,3,n)*
     & x4y4z3+c(4,4,4,n)*x4y4z4+c(4,4,5,n)*x4y4z5+c(4,5,0,n)*x4y5+c(4,
     & 5,1,n)*x4y5z1+c(4,5,2,n)*x4y5z2+c(4,5,3,n)*x4y5z3+c(4,5,4,n)*
     & x4y5z4+c(4,5,5,n)*x4y5z5+c(5,0,0,n)*x5+c(5,0,1,n)*x5z1+c(5,0,2,
     & n)*x5z2+c(5,0,3,n)*x5z3+c(5,0,4,n)*x5z4+c(5,0,5,n)*x5z5+c(5,1,
     & 0,n)*x5y1+c(5,1,1,n)*x5y1z1+c(5,1,2,n)*x5y1z2+c(5,1,3,n)*
     & x5y1z3+c(5,1,4,n)*x5y1z4+c(5,1,5,n)*x5y1z5+c(5,2,0,n)*x5y2+c(5,
     & 2,1,n)*x5y2z1+c(5,2,2,n)*x5y2z2+c(5,2,3,n)*x5y2z3+c(5,2,4,n)*
     & x5y2z4+c(5,2,5,n)*x5y2z5+c(5,3,0,n)*x5y3+c(5,3,1,n)*x5y3z1+c(5,
     & 3,2,n)*x5y3z2+c(5,3,3,n)*x5y3z3+c(5,3,4,n)*x5y3z4+c(5,3,5,n)*
     & x5y3z5+c(5,4,0,n)*x5y4+c(5,4,1,n)*x5y4z1+c(5,4,2,n)*x5y4z2+c(5,
     & 4,3,n)*x5y4z3+c(5,4,4,n)*x5y4z4+c(5,4,5,n)*x5y4z5+c(5,5,0,n)*
     & x5y5+c(5,5,1,n)*x5y5z1+c(5,5,2,n)*x5y5z2+c(5,5,3,n)*x5y5z3+c(5,
     & 5,4,n)*x5y5z4+c(5,5,5,n)*x5y5z5)*time
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
c
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
c
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
      r=(c(0,0,0,n)+c(0,0,1,n)*z1+c(0,0,2,n)*z2+c(0,0,3,n)*z3+c(0,0,4,
     & n)*z4+c(0,0,5,n)*z5+c(0,1,0,n)*y1+c(0,1,1,n)*y1z1+c(0,1,2,n)*
     & y1z2+c(0,1,3,n)*y1z3+c(0,1,4,n)*y1z4+c(0,1,5,n)*y1z5+c(0,2,0,n)
     & *y2+c(0,2,1,n)*y2z1+c(0,2,2,n)*y2z2+c(0,2,3,n)*y2z3+c(0,2,4,n)*
     & y2z4+c(0,2,5,n)*y2z5+c(0,3,0,n)*y3+c(0,3,1,n)*y3z1+c(0,3,2,n)*
     & y3z2+c(0,3,3,n)*y3z3+c(0,3,4,n)*y3z4+c(0,3,5,n)*y3z5+c(0,4,0,n)
     & *y4+c(0,4,1,n)*y4z1+c(0,4,2,n)*y4z2+c(0,4,3,n)*y4z3+c(0,4,4,n)*
     & y4z4+c(0,4,5,n)*y4z5+c(0,5,0,n)*y5+c(0,5,1,n)*y5z1+c(0,5,2,n)*
     & y5z2+c(0,5,3,n)*y5z3+c(0,5,4,n)*y5z4+c(0,5,5,n)*y5z5+c(1,0,0,n)
     & *x1+c(1,0,1,n)*x1z1+c(1,0,2,n)*x1z2+c(1,0,3,n)*x1z3+c(1,0,4,n)*
     & x1z4+c(1,0,5,n)*x1z5+c(1,1,0,n)*x1y1+c(1,1,1,n)*x1y1z1+c(1,1,2,
     & n)*x1y1z2+c(1,1,3,n)*x1y1z3+c(1,1,4,n)*x1y1z4+c(1,1,5,n)*
     & x1y1z5+c(1,2,0,n)*x1y2+c(1,2,1,n)*x1y2z1+c(1,2,2,n)*x1y2z2+c(1,
     & 2,3,n)*x1y2z3+c(1,2,4,n)*x1y2z4+c(1,2,5,n)*x1y2z5+c(1,3,0,n)*
     & x1y3+c(1,3,1,n)*x1y3z1+c(1,3,2,n)*x1y3z2+c(1,3,3,n)*x1y3z3+c(1,
     & 3,4,n)*x1y3z4+c(1,3,5,n)*x1y3z5+c(1,4,0,n)*x1y4+c(1,4,1,n)*
     & x1y4z1+c(1,4,2,n)*x1y4z2+c(1,4,3,n)*x1y4z3+c(1,4,4,n)*x1y4z4+c(
     & 1,4,5,n)*x1y4z5+c(1,5,0,n)*x1y5+c(1,5,1,n)*x1y5z1+c(1,5,2,n)*
     & x1y5z2+c(1,5,3,n)*x1y5z3+c(1,5,4,n)*x1y5z4+c(1,5,5,n)*x1y5z5+c(
     & 2,0,0,n)*x2+c(2,0,1,n)*x2z1+c(2,0,2,n)*x2z2+c(2,0,3,n)*x2z3+c(
     & 2,0,4,n)*x2z4+c(2,0,5,n)*x2z5+c(2,1,0,n)*x2y1+c(2,1,1,n)*
     & x2y1z1+c(2,1,2,n)*x2y1z2+c(2,1,3,n)*x2y1z3+c(2,1,4,n)*x2y1z4+c(
     & 2,1,5,n)*x2y1z5+c(2,2,0,n)*x2y2+c(2,2,1,n)*x2y2z1+c(2,2,2,n)*
     & x2y2z2+c(2,2,3,n)*x2y2z3+c(2,2,4,n)*x2y2z4+c(2,2,5,n)*x2y2z5+c(
     & 2,3,0,n)*x2y3+c(2,3,1,n)*x2y3z1+c(2,3,2,n)*x2y3z2+c(2,3,3,n)*
     & x2y3z3+c(2,3,4,n)*x2y3z4+c(2,3,5,n)*x2y3z5+c(2,4,0,n)*x2y4+c(2,
     & 4,1,n)*x2y4z1+c(2,4,2,n)*x2y4z2+c(2,4,3,n)*x2y4z3+c(2,4,4,n)*
     & x2y4z4+c(2,4,5,n)*x2y4z5+c(2,5,0,n)*x2y5+c(2,5,1,n)*x2y5z1+c(2,
     & 5,2,n)*x2y5z2+c(2,5,3,n)*x2y5z3+c(2,5,4,n)*x2y5z4+c(2,5,5,n)*
     & x2y5z5+c(3,0,0,n)*x3+c(3,0,1,n)*x3z1+c(3,0,2,n)*x3z2+c(3,0,3,n)
     & *x3z3+c(3,0,4,n)*x3z4+c(3,0,5,n)*x3z5+c(3,1,0,n)*x3y1+c(3,1,1,
     & n)*x3y1z1+c(3,1,2,n)*x3y1z2+c(3,1,3,n)*x3y1z3+c(3,1,4,n)*
     & x3y1z4+c(3,1,5,n)*x3y1z5+c(3,2,0,n)*x3y2+c(3,2,1,n)*x3y2z1+c(3,
     & 2,2,n)*x3y2z2+c(3,2,3,n)*x3y2z3+c(3,2,4,n)*x3y2z4+c(3,2,5,n)*
     & x3y2z5+c(3,3,0,n)*x3y3+c(3,3,1,n)*x3y3z1+c(3,3,2,n)*x3y3z2+c(3,
     & 3,3,n)*x3y3z3+c(3,3,4,n)*x3y3z4+c(3,3,5,n)*x3y3z5+c(3,4,0,n)*
     & x3y4+c(3,4,1,n)*x3y4z1+c(3,4,2,n)*x3y4z2+c(3,4,3,n)*x3y4z3+c(3,
     & 4,4,n)*x3y4z4+c(3,4,5,n)*x3y4z5+c(3,5,0,n)*x3y5+c(3,5,1,n)*
     & x3y5z1+c(3,5,2,n)*x3y5z2+c(3,5,3,n)*x3y5z3+c(3,5,4,n)*x3y5z4+c(
     & 3,5,5,n)*x3y5z5+c(4,0,0,n)*x4+c(4,0,1,n)*x4z1+c(4,0,2,n)*x4z2+
     & c(4,0,3,n)*x4z3+c(4,0,4,n)*x4z4+c(4,0,5,n)*x4z5+c(4,1,0,n)*
     & x4y1+c(4,1,1,n)*x4y1z1+c(4,1,2,n)*x4y1z2+c(4,1,3,n)*x4y1z3+c(4,
     & 1,4,n)*x4y1z4+c(4,1,5,n)*x4y1z5+c(4,2,0,n)*x4y2+c(4,2,1,n)*
     & x4y2z1+c(4,2,2,n)*x4y2z2+c(4,2,3,n)*x4y2z3+c(4,2,4,n)*x4y2z4+c(
     & 4,2,5,n)*x4y2z5+c(4,3,0,n)*x4y3+c(4,3,1,n)*x4y3z1+c(4,3,2,n)*
     & x4y3z2+c(4,3,3,n)*x4y3z3+c(4,3,4,n)*x4y3z4+c(4,3,5,n)*x4y3z5+c(
     & 4,4,0,n)*x4y4+c(4,4,1,n)*x4y4z1+c(4,4,2,n)*x4y4z2+c(4,4,3,n)*
     & x4y4z3+c(4,4,4,n)*x4y4z4+c(4,4,5,n)*x4y4z5+c(4,5,0,n)*x4y5+c(4,
     & 5,1,n)*x4y5z1+c(4,5,2,n)*x4y5z2+c(4,5,3,n)*x4y5z3+c(4,5,4,n)*
     & x4y5z4+c(4,5,5,n)*x4y5z5+c(5,0,0,n)*x5+c(5,0,1,n)*x5z1+c(5,0,2,
     & n)*x5z2+c(5,0,3,n)*x5z3+c(5,0,4,n)*x5z4+c(5,0,5,n)*x5z5+c(5,1,
     & 0,n)*x5y1+c(5,1,1,n)*x5y1z1+c(5,1,2,n)*x5y1z2+c(5,1,3,n)*
     & x5y1z3+c(5,1,4,n)*x5y1z4+c(5,1,5,n)*x5y1z5+c(5,2,0,n)*x5y2+c(5,
     & 2,1,n)*x5y2z1+c(5,2,2,n)*x5y2z2+c(5,2,3,n)*x5y2z3+c(5,2,4,n)*
     & x5y2z4+c(5,2,5,n)*x5y2z5+c(5,3,0,n)*x5y3+c(5,3,1,n)*x5y3z1+c(5,
     & 3,2,n)*x5y3z2+c(5,3,3,n)*x5y3z3+c(5,3,4,n)*x5y3z4+c(5,3,5,n)*
     & x5y3z5+c(5,4,0,n)*x5y4+c(5,4,1,n)*x5y4z1+c(5,4,2,n)*x5y4z2+c(5,
     & 4,3,n)*x5y4z3+c(5,4,4,n)*x5y4z4+c(5,4,5,n)*x5y4z5+c(5,5,0,n)*
     & x5y5+c(5,5,1,n)*x5y5z1+c(5,5,2,n)*x5y5z2+c(5,5,3,n)*x5y5z3+c(5,
     & 5,4,n)*x5y5z4+c(5,5,5,n)*x5y5z5)*time
! endLoops()
      else if( dx.eq.0.and.dy.eq.0.and.dz.eq.1.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
c 
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
c 
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
      r=(c(0,0,1,n)+c(0,0,2,n)*z1*2.+c(0,0,3,n)*z2*3.+c(0,0,4,n)*z3*4.+
     & c(0,0,5,n)*z4*5.+c(0,1,1,n)*y1+c(0,1,2,n)*y1z1*2.+c(0,1,3,n)*
     & y1z2*3.+c(0,1,4,n)*y1z3*4.+c(0,1,5,n)*y1z4*5.+c(0,2,1,n)*y2+c(
     & 0,2,2,n)*y2z1*2.+c(0,2,3,n)*y2z2*3.+c(0,2,4,n)*y2z3*4.+c(0,2,5,
     & n)*y2z4*5.+c(0,3,1,n)*y3+c(0,3,2,n)*y3z1*2.+c(0,3,3,n)*y3z2*3.+
     & c(0,3,4,n)*y3z3*4.+c(0,3,5,n)*y3z4*5.+c(0,4,1,n)*y4+c(0,4,2,n)*
     & y4z1*2.+c(0,4,3,n)*y4z2*3.+c(0,4,4,n)*y4z3*4.+c(0,4,5,n)*y4z4*
     & 5.+c(0,5,1,n)*y5+c(0,5,2,n)*y5z1*2.+c(0,5,3,n)*y5z2*3.+c(0,5,4,
     & n)*y5z3*4.+c(0,5,5,n)*y5z4*5.+c(1,0,1,n)*x1+c(1,0,2,n)*x1z1*2.+
     & c(1,0,3,n)*x1z2*3.+c(1,0,4,n)*x1z3*4.+c(1,0,5,n)*x1z4*5.+c(1,1,
     & 1,n)*x1y1+c(1,1,2,n)*x1y1z1*2.+c(1,1,3,n)*x1y1z2*3.+c(1,1,4,n)*
     & x1y1z3*4.+c(1,1,5,n)*x1y1z4*5.+c(1,2,1,n)*x1y2+c(1,2,2,n)*
     & x1y2z1*2.+c(1,2,3,n)*x1y2z2*3.+c(1,2,4,n)*x1y2z3*4.+c(1,2,5,n)*
     & x1y2z4*5.+c(1,3,1,n)*x1y3+c(1,3,2,n)*x1y3z1*2.+c(1,3,3,n)*
     & x1y3z2*3.+c(1,3,4,n)*x1y3z3*4.+c(1,3,5,n)*x1y3z4*5.+c(1,4,1,n)*
     & x1y4+c(1,4,2,n)*x1y4z1*2.+c(1,4,3,n)*x1y4z2*3.+c(1,4,4,n)*
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
      else if( dx.eq.0.and.dy.eq.0.and.dz.eq.2.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
c 
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
c 
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
      r=(c(0,0,2,n)*2.+c(0,0,3,n)*z1*6.+c(0,0,4,n)*z2*12.+c(0,0,5,n)*
     & z3*20.+c(0,1,2,n)*y1*2.+c(0,1,3,n)*y1z1*6.+c(0,1,4,n)*y1z2*12.+
     & c(0,1,5,n)*y1z3*20.+c(0,2,2,n)*y2*2.+c(0,2,3,n)*y2z1*6.+c(0,2,
     & 4,n)*y2z2*12.+c(0,2,5,n)*y2z3*20.+c(0,3,2,n)*y3*2.+c(0,3,3,n)*
     & y3z1*6.+c(0,3,4,n)*y3z2*12.+c(0,3,5,n)*y3z3*20.+c(0,4,2,n)*y4*
     & 2.+c(0,4,3,n)*y4z1*6.+c(0,4,4,n)*y4z2*12.+c(0,4,5,n)*y4z3*20.+
     & c(0,5,2,n)*y5*2.+c(0,5,3,n)*y5z1*6.+c(0,5,4,n)*y5z2*12.+c(0,5,
     & 5,n)*y5z3*20.+c(1,0,2,n)*x1*2.+c(1,0,3,n)*x1z1*6.+c(1,0,4,n)*
     & x1z2*12.+c(1,0,5,n)*x1z3*20.+c(1,1,2,n)*x1y1*2.+c(1,1,3,n)*
     & x1y1z1*6.+c(1,1,4,n)*x1y1z2*12.+c(1,1,5,n)*x1y1z3*20.+c(1,2,2,
     & n)*x1y2*2.+c(1,2,3,n)*x1y2z1*6.+c(1,2,4,n)*x1y2z2*12.+c(1,2,5,
     & n)*x1y2z3*20.+c(1,3,2,n)*x1y3*2.+c(1,3,3,n)*x1y3z1*6.+c(1,3,4,
     & n)*x1y3z2*12.+c(1,3,5,n)*x1y3z3*20.+c(1,4,2,n)*x1y4*2.+c(1,4,3,
     & n)*x1y4z1*6.+c(1,4,4,n)*x1y4z2*12.+c(1,4,5,n)*x1y4z3*20.+c(1,5,
     & 2,n)*x1y5*2.+c(1,5,3,n)*x1y5z1*6.+c(1,5,4,n)*x1y5z2*12.+c(1,5,
     & 5,n)*x1y5z3*20.+c(2,0,2,n)*x2*2.+c(2,0,3,n)*x2z1*6.+c(2,0,4,n)*
     & x2z2*12.+c(2,0,5,n)*x2z3*20.+c(2,1,2,n)*x2y1*2.+c(2,1,3,n)*
     & x2y1z1*6.+c(2,1,4,n)*x2y1z2*12.+c(2,1,5,n)*x2y1z3*20.+c(2,2,2,
     & n)*x2y2*2.+c(2,2,3,n)*x2y2z1*6.+c(2,2,4,n)*x2y2z2*12.+c(2,2,5,
     & n)*x2y2z3*20.+c(2,3,2,n)*x2y3*2.+c(2,3,3,n)*x2y3z1*6.+c(2,3,4,
     & n)*x2y3z2*12.+c(2,3,5,n)*x2y3z3*20.+c(2,4,2,n)*x2y4*2.+c(2,4,3,
     & n)*x2y4z1*6.+c(2,4,4,n)*x2y4z2*12.+c(2,4,5,n)*x2y4z3*20.+c(2,5,
     & 2,n)*x2y5*2.+c(2,5,3,n)*x2y5z1*6.+c(2,5,4,n)*x2y5z2*12.+c(2,5,
     & 5,n)*x2y5z3*20.+c(3,0,2,n)*x3*2.+c(3,0,3,n)*x3z1*6.+c(3,0,4,n)*
     & x3z2*12.+c(3,0,5,n)*x3z3*20.+c(3,1,2,n)*x3y1*2.+c(3,1,3,n)*
     & x3y1z1*6.+c(3,1,4,n)*x3y1z2*12.+c(3,1,5,n)*x3y1z3*20.+c(3,2,2,
     & n)*x3y2*2.+c(3,2,3,n)*x3y2z1*6.+c(3,2,4,n)*x3y2z2*12.+c(3,2,5,
     & n)*x3y2z3*20.+c(3,3,2,n)*x3y3*2.+c(3,3,3,n)*x3y3z1*6.+c(3,3,4,
     & n)*x3y3z2*12.+c(3,3,5,n)*x3y3z3*20.+c(3,4,2,n)*x3y4*2.+c(3,4,3,
     & n)*x3y4z1*6.+c(3,4,4,n)*x3y4z2*12.+c(3,4,5,n)*x3y4z3*20.+c(3,5,
     & 2,n)*x3y5*2.+c(3,5,3,n)*x3y5z1*6.+c(3,5,4,n)*x3y5z2*12.+c(3,5,
     & 5,n)*x3y5z3*20.+c(4,0,2,n)*x4*2.+c(4,0,3,n)*x4z1*6.+c(4,0,4,n)*
     & x4z2*12.+c(4,0,5,n)*x4z3*20.+c(4,1,2,n)*x4y1*2.+c(4,1,3,n)*
     & x4y1z1*6.+c(4,1,4,n)*x4y1z2*12.+c(4,1,5,n)*x4y1z3*20.+c(4,2,2,
     & n)*x4y2*2.+c(4,2,3,n)*x4y2z1*6.+c(4,2,4,n)*x4y2z2*12.+c(4,2,5,
     & n)*x4y2z3*20.+c(4,3,2,n)*x4y3*2.+c(4,3,3,n)*x4y3z1*6.+c(4,3,4,
     & n)*x4y3z2*12.+c(4,3,5,n)*x4y3z3*20.+c(4,4,2,n)*x4y4*2.+c(4,4,3,
     & n)*x4y4z1*6.+c(4,4,4,n)*x4y4z2*12.+c(4,4,5,n)*x4y4z3*20.+c(4,5,
     & 2,n)*x4y5*2.+c(4,5,3,n)*x4y5z1*6.+c(4,5,4,n)*x4y5z2*12.+c(4,5,
     & 5,n)*x4y5z3*20.+c(5,0,2,n)*x5*2.+c(5,0,3,n)*x5z1*6.+c(5,0,4,n)*
     & x5z2*12.+c(5,0,5,n)*x5z3*20.+c(5,1,2,n)*x5y1*2.+c(5,1,3,n)*
     & x5y1z1*6.+c(5,1,4,n)*x5y1z2*12.+c(5,1,5,n)*x5y1z3*20.+c(5,2,2,
     & n)*x5y2*2.+c(5,2,3,n)*x5y2z1*6.+c(5,2,4,n)*x5y2z2*12.+c(5,2,5,
     & n)*x5y2z3*20.+c(5,3,2,n)*x5y3*2.+c(5,3,3,n)*x5y3z1*6.+c(5,3,4,
     & n)*x5y3z2*12.+c(5,3,5,n)*x5y3z3*20.+c(5,4,2,n)*x5y4*2.+c(5,4,3,
     & n)*x5y4z1*6.+c(5,4,4,n)*x5y4z2*12.+c(5,4,5,n)*x5y4z3*20.+c(5,5,
     & 2,n)*x5y5*2.+c(5,5,3,n)*x5y5z1*6.+c(5,5,4,n)*x5y5z2*12.+c(5,5,
     & 5,n)*x5y5z3*20.)*time
! endLoops()
      else if( dx.eq.0.and.dy.eq.1.and.dz.eq.0.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
c 
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
c 
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
      r=(c(0,1,0,n)+c(0,1,1,n)*z1+c(0,1,2,n)*z2+c(0,1,3,n)*z3+c(0,1,4,
     & n)*z4+c(0,1,5,n)*z5+c(0,2,0,n)*y1*2.+c(0,2,1,n)*y1z1*2.+c(0,2,
     & 2,n)*y1z2*2.+c(0,2,3,n)*y1z3*2.+c(0,2,4,n)*y1z4*2.+c(0,2,5,n)*
     & y1z5*2.+c(0,3,0,n)*y2*3.+c(0,3,1,n)*y2z1*3.+c(0,3,2,n)*y2z2*3.+
     & c(0,3,3,n)*y2z3*3.+c(0,3,4,n)*y2z4*3.+c(0,3,5,n)*y2z5*3.+c(0,4,
     & 0,n)*y3*4.+c(0,4,1,n)*y3z1*4.+c(0,4,2,n)*y3z2*4.+c(0,4,3,n)*
     & y3z3*4.+c(0,4,4,n)*y3z4*4.+c(0,4,5,n)*y3z5*4.+c(0,5,0,n)*y4*5.+
     & c(0,5,1,n)*y4z1*5.+c(0,5,2,n)*y4z2*5.+c(0,5,3,n)*y4z3*5.+c(0,5,
     & 4,n)*y4z4*5.+c(0,5,5,n)*y4z5*5.+c(1,1,0,n)*x1+c(1,1,1,n)*x1z1+
     & c(1,1,2,n)*x1z2+c(1,1,3,n)*x1z3+c(1,1,4,n)*x1z4+c(1,1,5,n)*
     & x1z5+c(1,2,0,n)*x1y1*2.+c(1,2,1,n)*x1y1z1*2.+c(1,2,2,n)*x1y1z2*
     & 2.+c(1,2,3,n)*x1y1z3*2.+c(1,2,4,n)*x1y1z4*2.+c(1,2,5,n)*x1y1z5*
     & 2.+c(1,3,0,n)*x1y2*3.+c(1,3,1,n)*x1y2z1*3.+c(1,3,2,n)*x1y2z2*
     & 3.+c(1,3,3,n)*x1y2z3*3.+c(1,3,4,n)*x1y2z4*3.+c(1,3,5,n)*x1y2z5*
     & 3.+c(1,4,0,n)*x1y3*4.+c(1,4,1,n)*x1y3z1*4.+c(1,4,2,n)*x1y3z2*
     & 4.+c(1,4,3,n)*x1y3z3*4.+c(1,4,4,n)*x1y3z4*4.+c(1,4,5,n)*x1y3z5*
     & 4.+c(1,5,0,n)*x1y4*5.+c(1,5,1,n)*x1y4z1*5.+c(1,5,2,n)*x1y4z2*
     & 5.+c(1,5,3,n)*x1y4z3*5.+c(1,5,4,n)*x1y4z4*5.+c(1,5,5,n)*x1y4z5*
     & 5.+c(2,1,0,n)*x2+c(2,1,1,n)*x2z1+c(2,1,2,n)*x2z2+c(2,1,3,n)*
     & x2z3+c(2,1,4,n)*x2z4+c(2,1,5,n)*x2z5+c(2,2,0,n)*x2y1*2.+c(2,2,
     & 1,n)*x2y1z1*2.+c(2,2,2,n)*x2y1z2*2.+c(2,2,3,n)*x2y1z3*2.+c(2,2,
     & 4,n)*x2y1z4*2.+c(2,2,5,n)*x2y1z5*2.+c(2,3,0,n)*x2y2*3.+c(2,3,1,
     & n)*x2y2z1*3.+c(2,3,2,n)*x2y2z2*3.+c(2,3,3,n)*x2y2z3*3.+c(2,3,4,
     & n)*x2y2z4*3.+c(2,3,5,n)*x2y2z5*3.+c(2,4,0,n)*x2y3*4.+c(2,4,1,n)
     & *x2y3z1*4.+c(2,4,2,n)*x2y3z2*4.+c(2,4,3,n)*x2y3z3*4.+c(2,4,4,n)
     & *x2y3z4*4.+c(2,4,5,n)*x2y3z5*4.+c(2,5,0,n)*x2y4*5.+c(2,5,1,n)*
     & x2y4z1*5.+c(2,5,2,n)*x2y4z2*5.+c(2,5,3,n)*x2y4z3*5.+c(2,5,4,n)*
     & x2y4z4*5.+c(2,5,5,n)*x2y4z5*5.+c(3,1,0,n)*x3+c(3,1,1,n)*x3z1+c(
     & 3,1,2,n)*x3z2+c(3,1,3,n)*x3z3+c(3,1,4,n)*x3z4+c(3,1,5,n)*x3z5+
     & c(3,2,0,n)*x3y1*2.+c(3,2,1,n)*x3y1z1*2.+c(3,2,2,n)*x3y1z2*2.+c(
     & 3,2,3,n)*x3y1z3*2.+c(3,2,4,n)*x3y1z4*2.+c(3,2,5,n)*x3y1z5*2.+c(
     & 3,3,0,n)*x3y2*3.+c(3,3,1,n)*x3y2z1*3.+c(3,3,2,n)*x3y2z2*3.+c(3,
     & 3,3,n)*x3y2z3*3.+c(3,3,4,n)*x3y2z4*3.+c(3,3,5,n)*x3y2z5*3.+c(3,
     & 4,0,n)*x3y3*4.+c(3,4,1,n)*x3y3z1*4.+c(3,4,2,n)*x3y3z2*4.+c(3,4,
     & 3,n)*x3y3z3*4.+c(3,4,4,n)*x3y3z4*4.+c(3,4,5,n)*x3y3z5*4.+c(3,5,
     & 0,n)*x3y4*5.+c(3,5,1,n)*x3y4z1*5.+c(3,5,2,n)*x3y4z2*5.+c(3,5,3,
     & n)*x3y4z3*5.+c(3,5,4,n)*x3y4z4*5.+c(3,5,5,n)*x3y4z5*5.+c(4,1,0,
     & n)*x4+c(4,1,1,n)*x4z1+c(4,1,2,n)*x4z2+c(4,1,3,n)*x4z3+c(4,1,4,
     & n)*x4z4+c(4,1,5,n)*x4z5+c(4,2,0,n)*x4y1*2.+c(4,2,1,n)*x4y1z1*
     & 2.+c(4,2,2,n)*x4y1z2*2.+c(4,2,3,n)*x4y1z3*2.+c(4,2,4,n)*x4y1z4*
     & 2.+c(4,2,5,n)*x4y1z5*2.+c(4,3,0,n)*x4y2*3.+c(4,3,1,n)*x4y2z1*
     & 3.+c(4,3,2,n)*x4y2z2*3.+c(4,3,3,n)*x4y2z3*3.+c(4,3,4,n)*x4y2z4*
     & 3.+c(4,3,5,n)*x4y2z5*3.+c(4,4,0,n)*x4y3*4.+c(4,4,1,n)*x4y3z1*
     & 4.+c(4,4,2,n)*x4y3z2*4.+c(4,4,3,n)*x4y3z3*4.+c(4,4,4,n)*x4y3z4*
     & 4.+c(4,4,5,n)*x4y3z5*4.+c(4,5,0,n)*x4y4*5.+c(4,5,1,n)*x4y4z1*
     & 5.+c(4,5,2,n)*x4y4z2*5.+c(4,5,3,n)*x4y4z3*5.+c(4,5,4,n)*x4y4z4*
     & 5.+c(4,5,5,n)*x4y4z5*5.+c(5,1,0,n)*x5+c(5,1,1,n)*x5z1+c(5,1,2,
     & n)*x5z2+c(5,1,3,n)*x5z3+c(5,1,4,n)*x5z4+c(5,1,5,n)*x5z5+c(5,2,
     & 0,n)*x5y1*2.+c(5,2,1,n)*x5y1z1*2.+c(5,2,2,n)*x5y1z2*2.+c(5,2,3,
     & n)*x5y1z3*2.+c(5,2,4,n)*x5y1z4*2.+c(5,2,5,n)*x5y1z5*2.+c(5,3,0,
     & n)*x5y2*3.+c(5,3,1,n)*x5y2z1*3.+c(5,3,2,n)*x5y2z2*3.+c(5,3,3,n)
     & *x5y2z3*3.+c(5,3,4,n)*x5y2z4*3.+c(5,3,5,n)*x5y2z5*3.+c(5,4,0,n)
     & *x5y3*4.+c(5,4,1,n)*x5y3z1*4.+c(5,4,2,n)*x5y3z2*4.+c(5,4,3,n)*
     & x5y3z3*4.+c(5,4,4,n)*x5y3z4*4.+c(5,4,5,n)*x5y3z5*4.+c(5,5,0,n)*
     & x5y4*5.+c(5,5,1,n)*x5y4z1*5.+c(5,5,2,n)*x5y4z2*5.+c(5,5,3,n)*
     & x5y4z3*5.+c(5,5,4,n)*x5y4z4*5.+c(5,5,5,n)*x5y4z5*5.)*time
! endLoops()
      else if( dx.eq.0.and.dy.eq.1.and.dz.eq.1.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
c 
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
c 
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
      r=(c(0,1,1,n)+c(0,1,2,n)*z1*2.+c(0,1,3,n)*z2*3.+c(0,1,4,n)*z3*4.+
     & c(0,1,5,n)*z4*5.+c(0,2,1,n)*y1*2.+c(0,2,2,n)*y1z1*4.+c(0,2,3,n)
     & *y1z2*6.+c(0,2,4,n)*y1z3*8.+c(0,2,5,n)*y1z4*10.+c(0,3,1,n)*y2*
     & 3.+c(0,3,2,n)*y2z1*6.+c(0,3,3,n)*y2z2*9.+c(0,3,4,n)*y2z3*12.+c(
     & 0,3,5,n)*y2z4*15.+c(0,4,1,n)*y3*4.+c(0,4,2,n)*y3z1*8.+c(0,4,3,
     & n)*y3z2*12.+c(0,4,4,n)*y3z3*16.+c(0,4,5,n)*y3z4*20.+c(0,5,1,n)*
     & y4*5.+c(0,5,2,n)*y4z1*10.+c(0,5,3,n)*y4z2*15.+c(0,5,4,n)*y4z3*
     & 20.+c(0,5,5,n)*y4z4*25.+c(1,1,1,n)*x1+c(1,1,2,n)*x1z1*2.+c(1,1,
     & 3,n)*x1z2*3.+c(1,1,4,n)*x1z3*4.+c(1,1,5,n)*x1z4*5.+c(1,2,1,n)*
     & x1y1*2.+c(1,2,2,n)*x1y1z1*4.+c(1,2,3,n)*x1y1z2*6.+c(1,2,4,n)*
     & x1y1z3*8.+c(1,2,5,n)*x1y1z4*10.+c(1,3,1,n)*x1y2*3.+c(1,3,2,n)*
     & x1y2z1*6.+c(1,3,3,n)*x1y2z2*9.+c(1,3,4,n)*x1y2z3*12.+c(1,3,5,n)
     & *x1y2z4*15.+c(1,4,1,n)*x1y3*4.+c(1,4,2,n)*x1y3z1*8.+c(1,4,3,n)*
     & x1y3z2*12.+c(1,4,4,n)*x1y3z3*16.+c(1,4,5,n)*x1y3z4*20.+c(1,5,1,
     & n)*x1y4*5.+c(1,5,2,n)*x1y4z1*10.+c(1,5,3,n)*x1y4z2*15.+c(1,5,4,
     & n)*x1y4z3*20.+c(1,5,5,n)*x1y4z4*25.+c(2,1,1,n)*x2+c(2,1,2,n)*
     & x2z1*2.+c(2,1,3,n)*x2z2*3.+c(2,1,4,n)*x2z3*4.+c(2,1,5,n)*x2z4*
     & 5.+c(2,2,1,n)*x2y1*2.+c(2,2,2,n)*x2y1z1*4.+c(2,2,3,n)*x2y1z2*
     & 6.+c(2,2,4,n)*x2y1z3*8.+c(2,2,5,n)*x2y1z4*10.+c(2,3,1,n)*x2y2*
     & 3.+c(2,3,2,n)*x2y2z1*6.+c(2,3,3,n)*x2y2z2*9.+c(2,3,4,n)*x2y2z3*
     & 12.+c(2,3,5,n)*x2y2z4*15.+c(2,4,1,n)*x2y3*4.+c(2,4,2,n)*x2y3z1*
     & 8.+c(2,4,3,n)*x2y3z2*12.+c(2,4,4,n)*x2y3z3*16.+c(2,4,5,n)*
     & x2y3z4*20.+c(2,5,1,n)*x2y4*5.+c(2,5,2,n)*x2y4z1*10.+c(2,5,3,n)*
     & x2y4z2*15.+c(2,5,4,n)*x2y4z3*20.+c(2,5,5,n)*x2y4z4*25.+c(3,1,1,
     & n)*x3+c(3,1,2,n)*x3z1*2.+c(3,1,3,n)*x3z2*3.+c(3,1,4,n)*x3z3*4.+
     & c(3,1,5,n)*x3z4*5.+c(3,2,1,n)*x3y1*2.+c(3,2,2,n)*x3y1z1*4.+c(3,
     & 2,3,n)*x3y1z2*6.+c(3,2,4,n)*x3y1z3*8.+c(3,2,5,n)*x3y1z4*10.+c(
     & 3,3,1,n)*x3y2*3.+c(3,3,2,n)*x3y2z1*6.+c(3,3,3,n)*x3y2z2*9.+c(3,
     & 3,4,n)*x3y2z3*12.+c(3,3,5,n)*x3y2z4*15.+c(3,4,1,n)*x3y3*4.+c(3,
     & 4,2,n)*x3y3z1*8.+c(3,4,3,n)*x3y3z2*12.+c(3,4,4,n)*x3y3z3*16.+c(
     & 3,4,5,n)*x3y3z4*20.+c(3,5,1,n)*x3y4*5.+c(3,5,2,n)*x3y4z1*10.+c(
     & 3,5,3,n)*x3y4z2*15.+c(3,5,4,n)*x3y4z3*20.+c(3,5,5,n)*x3y4z4*
     & 25.+c(4,1,1,n)*x4+c(4,1,2,n)*x4z1*2.+c(4,1,3,n)*x4z2*3.+c(4,1,
     & 4,n)*x4z3*4.+c(4,1,5,n)*x4z4*5.+c(4,2,1,n)*x4y1*2.+c(4,2,2,n)*
     & x4y1z1*4.+c(4,2,3,n)*x4y1z2*6.+c(4,2,4,n)*x4y1z3*8.+c(4,2,5,n)*
     & x4y1z4*10.+c(4,3,1,n)*x4y2*3.+c(4,3,2,n)*x4y2z1*6.+c(4,3,3,n)*
     & x4y2z2*9.+c(4,3,4,n)*x4y2z3*12.+c(4,3,5,n)*x4y2z4*15.+c(4,4,1,
     & n)*x4y3*4.+c(4,4,2,n)*x4y3z1*8.+c(4,4,3,n)*x4y3z2*12.+c(4,4,4,
     & n)*x4y3z3*16.+c(4,4,5,n)*x4y3z4*20.+c(4,5,1,n)*x4y4*5.+c(4,5,2,
     & n)*x4y4z1*10.+c(4,5,3,n)*x4y4z2*15.+c(4,5,4,n)*x4y4z3*20.+c(4,
     & 5,5,n)*x4y4z4*25.+c(5,1,1,n)*x5+c(5,1,2,n)*x5z1*2.+c(5,1,3,n)*
     & x5z2*3.+c(5,1,4,n)*x5z3*4.+c(5,1,5,n)*x5z4*5.+c(5,2,1,n)*x5y1*
     & 2.+c(5,2,2,n)*x5y1z1*4.+c(5,2,3,n)*x5y1z2*6.+c(5,2,4,n)*x5y1z3*
     & 8.+c(5,2,5,n)*x5y1z4*10.+c(5,3,1,n)*x5y2*3.+c(5,3,2,n)*x5y2z1*
     & 6.+c(5,3,3,n)*x5y2z2*9.+c(5,3,4,n)*x5y2z3*12.+c(5,3,5,n)*
     & x5y2z4*15.+c(5,4,1,n)*x5y3*4.+c(5,4,2,n)*x5y3z1*8.+c(5,4,3,n)*
     & x5y3z2*12.+c(5,4,4,n)*x5y3z3*16.+c(5,4,5,n)*x5y3z4*20.+c(5,5,1,
     & n)*x5y4*5.+c(5,5,2,n)*x5y4z1*10.+c(5,5,3,n)*x5y4z2*15.+c(5,5,4,
     & n)*x5y4z3*20.+c(5,5,5,n)*x5y4z4*25.)*time
! endLoops()
      else if( dx.eq.0.and.dy.eq.1.and.dz.eq.2.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
c 
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
c 
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
      r=(c(0,1,2,n)*2.+c(0,1,3,n)*z1*6.+c(0,1,4,n)*z2*12.+c(0,1,5,n)*
     & z3*20.+c(0,2,2,n)*y1*4.+c(0,2,3,n)*y1z1*12.+c(0,2,4,n)*y1z2*
     & 24.+c(0,2,5,n)*y1z3*40.+c(0,3,2,n)*y2*6.+c(0,3,3,n)*y2z1*18.+c(
     & 0,3,4,n)*y2z2*36.+c(0,3,5,n)*y2z3*60.+c(0,4,2,n)*y3*8.+c(0,4,3,
     & n)*y3z1*24.+c(0,4,4,n)*y3z2*48.+c(0,4,5,n)*y3z3*80.+c(0,5,2,n)*
     & y4*10.+c(0,5,3,n)*y4z1*30.+c(0,5,4,n)*y4z2*60.+c(0,5,5,n)*y4z3*
     & 100.+c(1,1,2,n)*x1*2.+c(1,1,3,n)*x1z1*6.+c(1,1,4,n)*x1z2*12.+c(
     & 1,1,5,n)*x1z3*20.+c(1,2,2,n)*x1y1*4.+c(1,2,3,n)*x1y1z1*12.+c(1,
     & 2,4,n)*x1y1z2*24.+c(1,2,5,n)*x1y1z3*40.+c(1,3,2,n)*x1y2*6.+c(1,
     & 3,3,n)*x1y2z1*18.+c(1,3,4,n)*x1y2z2*36.+c(1,3,5,n)*x1y2z3*60.+
     & c(1,4,2,n)*x1y3*8.+c(1,4,3,n)*x1y3z1*24.+c(1,4,4,n)*x1y3z2*48.+
     & c(1,4,5,n)*x1y3z3*80.+c(1,5,2,n)*x1y4*10.+c(1,5,3,n)*x1y4z1*
     & 30.+c(1,5,4,n)*x1y4z2*60.+c(1,5,5,n)*x1y4z3*100.+c(2,1,2,n)*x2*
     & 2.+c(2,1,3,n)*x2z1*6.+c(2,1,4,n)*x2z2*12.+c(2,1,5,n)*x2z3*20.+
     & c(2,2,2,n)*x2y1*4.+c(2,2,3,n)*x2y1z1*12.+c(2,2,4,n)*x2y1z2*24.+
     & c(2,2,5,n)*x2y1z3*40.+c(2,3,2,n)*x2y2*6.+c(2,3,3,n)*x2y2z1*18.+
     & c(2,3,4,n)*x2y2z2*36.+c(2,3,5,n)*x2y2z3*60.+c(2,4,2,n)*x2y3*8.+
     & c(2,4,3,n)*x2y3z1*24.+c(2,4,4,n)*x2y3z2*48.+c(2,4,5,n)*x2y3z3*
     & 80.+c(2,5,2,n)*x2y4*10.+c(2,5,3,n)*x2y4z1*30.+c(2,5,4,n)*
     & x2y4z2*60.+c(2,5,5,n)*x2y4z3*100.+c(3,1,2,n)*x3*2.+c(3,1,3,n)*
     & x3z1*6.+c(3,1,4,n)*x3z2*12.+c(3,1,5,n)*x3z3*20.+c(3,2,2,n)*
     & x3y1*4.+c(3,2,3,n)*x3y1z1*12.+c(3,2,4,n)*x3y1z2*24.+c(3,2,5,n)*
     & x3y1z3*40.+c(3,3,2,n)*x3y2*6.+c(3,3,3,n)*x3y2z1*18.+c(3,3,4,n)*
     & x3y2z2*36.+c(3,3,5,n)*x3y2z3*60.+c(3,4,2,n)*x3y3*8.+c(3,4,3,n)*
     & x3y3z1*24.+c(3,4,4,n)*x3y3z2*48.+c(3,4,5,n)*x3y3z3*80.+c(3,5,2,
     & n)*x3y4*10.+c(3,5,3,n)*x3y4z1*30.+c(3,5,4,n)*x3y4z2*60.+c(3,5,
     & 5,n)*x3y4z3*100.+c(4,1,2,n)*x4*2.+c(4,1,3,n)*x4z1*6.+c(4,1,4,n)
     & *x4z2*12.+c(4,1,5,n)*x4z3*20.+c(4,2,2,n)*x4y1*4.+c(4,2,3,n)*
     & x4y1z1*12.+c(4,2,4,n)*x4y1z2*24.+c(4,2,5,n)*x4y1z3*40.+c(4,3,2,
     & n)*x4y2*6.+c(4,3,3,n)*x4y2z1*18.+c(4,3,4,n)*x4y2z2*36.+c(4,3,5,
     & n)*x4y2z3*60.+c(4,4,2,n)*x4y3*8.+c(4,4,3,n)*x4y3z1*24.+c(4,4,4,
     & n)*x4y3z2*48.+c(4,4,5,n)*x4y3z3*80.+c(4,5,2,n)*x4y4*10.+c(4,5,
     & 3,n)*x4y4z1*30.+c(4,5,4,n)*x4y4z2*60.+c(4,5,5,n)*x4y4z3*100.+c(
     & 5,1,2,n)*x5*2.+c(5,1,3,n)*x5z1*6.+c(5,1,4,n)*x5z2*12.+c(5,1,5,
     & n)*x5z3*20.+c(5,2,2,n)*x5y1*4.+c(5,2,3,n)*x5y1z1*12.+c(5,2,4,n)
     & *x5y1z2*24.+c(5,2,5,n)*x5y1z3*40.+c(5,3,2,n)*x5y2*6.+c(5,3,3,n)
     & *x5y2z1*18.+c(5,3,4,n)*x5y2z2*36.+c(5,3,5,n)*x5y2z3*60.+c(5,4,
     & 2,n)*x5y3*8.+c(5,4,3,n)*x5y3z1*24.+c(5,4,4,n)*x5y3z2*48.+c(5,4,
     & 5,n)*x5y3z3*80.+c(5,5,2,n)*x5y4*10.+c(5,5,3,n)*x5y4z1*30.+c(5,
     & 5,4,n)*x5y4z2*60.+c(5,5,5,n)*x5y4z3*100.)*time
! endLoops()
      else if( dx.eq.0.and.dy.eq.2.and.dz.eq.0.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
c 
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
c 
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
      r=(c(0,2,0,n)*2.+c(0,2,1,n)*z1*2.+c(0,2,2,n)*z2*2.+c(0,2,3,n)*z3*
     & 2.+c(0,2,4,n)*z4*2.+c(0,2,5,n)*z5*2.+c(0,3,0,n)*y1*6.+c(0,3,1,
     & n)*y1z1*6.+c(0,3,2,n)*y1z2*6.+c(0,3,3,n)*y1z3*6.+c(0,3,4,n)*
     & y1z4*6.+c(0,3,5,n)*y1z5*6.+c(0,4,0,n)*y2*12.+c(0,4,1,n)*y2z1*
     & 12.+c(0,4,2,n)*y2z2*12.+c(0,4,3,n)*y2z3*12.+c(0,4,4,n)*y2z4*
     & 12.+c(0,4,5,n)*y2z5*12.+c(0,5,0,n)*y3*20.+c(0,5,1,n)*y3z1*20.+
     & c(0,5,2,n)*y3z2*20.+c(0,5,3,n)*y3z3*20.+c(0,5,4,n)*y3z4*20.+c(
     & 0,5,5,n)*y3z5*20.+c(1,2,0,n)*x1*2.+c(1,2,1,n)*x1z1*2.+c(1,2,2,
     & n)*x1z2*2.+c(1,2,3,n)*x1z3*2.+c(1,2,4,n)*x1z4*2.+c(1,2,5,n)*
     & x1z5*2.+c(1,3,0,n)*x1y1*6.+c(1,3,1,n)*x1y1z1*6.+c(1,3,2,n)*
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
      else if( dx.eq.0.and.dy.eq.2.and.dz.eq.1.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
c 
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
c 
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
      r=(c(0,2,1,n)*2.+c(0,2,2,n)*z1*4.+c(0,2,3,n)*z2*6.+c(0,2,4,n)*z3*
     & 8.+c(0,2,5,n)*z4*10.+c(0,3,1,n)*y1*6.+c(0,3,2,n)*y1z1*12.+c(0,
     & 3,3,n)*y1z2*18.+c(0,3,4,n)*y1z3*24.+c(0,3,5,n)*y1z4*30.+c(0,4,
     & 1,n)*y2*12.+c(0,4,2,n)*y2z1*24.+c(0,4,3,n)*y2z2*36.+c(0,4,4,n)*
     & y2z3*48.+c(0,4,5,n)*y2z4*60.+c(0,5,1,n)*y3*20.+c(0,5,2,n)*y3z1*
     & 40.+c(0,5,3,n)*y3z2*60.+c(0,5,4,n)*y3z3*80.+c(0,5,5,n)*y3z4*
     & 100.+c(1,2,1,n)*x1*2.+c(1,2,2,n)*x1z1*4.+c(1,2,3,n)*x1z2*6.+c(
     & 1,2,4,n)*x1z3*8.+c(1,2,5,n)*x1z4*10.+c(1,3,1,n)*x1y1*6.+c(1,3,
     & 2,n)*x1y1z1*12.+c(1,3,3,n)*x1y1z2*18.+c(1,3,4,n)*x1y1z3*24.+c(
     & 1,3,5,n)*x1y1z4*30.+c(1,4,1,n)*x1y2*12.+c(1,4,2,n)*x1y2z1*24.+
     & c(1,4,3,n)*x1y2z2*36.+c(1,4,4,n)*x1y2z3*48.+c(1,4,5,n)*x1y2z4*
     & 60.+c(1,5,1,n)*x1y3*20.+c(1,5,2,n)*x1y3z1*40.+c(1,5,3,n)*
     & x1y3z2*60.+c(1,5,4,n)*x1y3z3*80.+c(1,5,5,n)*x1y3z4*100.+c(2,2,
     & 1,n)*x2*2.+c(2,2,2,n)*x2z1*4.+c(2,2,3,n)*x2z2*6.+c(2,2,4,n)*
     & x2z3*8.+c(2,2,5,n)*x2z4*10.+c(2,3,1,n)*x2y1*6.+c(2,3,2,n)*
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
      else if( dx.eq.0.and.dy.eq.2.and.dz.eq.2.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
c 
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
c 
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
      r=(c(0,2,2,n)*4.+c(0,2,3,n)*z1*12.+c(0,2,4,n)*z2*24.+c(0,2,5,n)*
     & z3*40.+c(0,3,2,n)*y1*12.+c(0,3,3,n)*y1z1*36.+c(0,3,4,n)*y1z2*
     & 72.+c(0,3,5,n)*y1z3*120.+c(0,4,2,n)*y2*24.+c(0,4,3,n)*y2z1*72.+
     & c(0,4,4,n)*y2z2*144.+c(0,4,5,n)*y2z3*240.+c(0,5,2,n)*y3*40.+c(
     & 0,5,3,n)*y3z1*120.+c(0,5,4,n)*y3z2*240.+c(0,5,5,n)*y3z3*400.+c(
     & 1,2,2,n)*x1*4.+c(1,2,3,n)*x1z1*12.+c(1,2,4,n)*x1z2*24.+c(1,2,5,
     & n)*x1z3*40.+c(1,3,2,n)*x1y1*12.+c(1,3,3,n)*x1y1z1*36.+c(1,3,4,
     & n)*x1y1z2*72.+c(1,3,5,n)*x1y1z3*120.+c(1,4,2,n)*x1y2*24.+c(1,4,
     & 3,n)*x1y2z1*72.+c(1,4,4,n)*x1y2z2*144.+c(1,4,5,n)*x1y2z3*240.+
     & c(1,5,2,n)*x1y3*40.+c(1,5,3,n)*x1y3z1*120.+c(1,5,4,n)*x1y3z2*
     & 240.+c(1,5,5,n)*x1y3z3*400.+c(2,2,2,n)*x2*4.+c(2,2,3,n)*x2z1*
     & 12.+c(2,2,4,n)*x2z2*24.+c(2,2,5,n)*x2z3*40.+c(2,3,2,n)*x2y1*
     & 12.+c(2,3,3,n)*x2y1z1*36.+c(2,3,4,n)*x2y1z2*72.+c(2,3,5,n)*
     & x2y1z3*120.+c(2,4,2,n)*x2y2*24.+c(2,4,3,n)*x2y2z1*72.+c(2,4,4,
     & n)*x2y2z2*144.+c(2,4,5,n)*x2y2z3*240.+c(2,5,2,n)*x2y3*40.+c(2,
     & 5,3,n)*x2y3z1*120.+c(2,5,4,n)*x2y3z2*240.+c(2,5,5,n)*x2y3z3*
     & 400.+c(3,2,2,n)*x3*4.+c(3,2,3,n)*x3z1*12.+c(3,2,4,n)*x3z2*24.+
     & c(3,2,5,n)*x3z3*40.+c(3,3,2,n)*x3y1*12.+c(3,3,3,n)*x3y1z1*36.+
     & c(3,3,4,n)*x3y1z2*72.+c(3,3,5,n)*x3y1z3*120.+c(3,4,2,n)*x3y2*
     & 24.+c(3,4,3,n)*x3y2z1*72.+c(3,4,4,n)*x3y2z2*144.+c(3,4,5,n)*
     & x3y2z3*240.+c(3,5,2,n)*x3y3*40.+c(3,5,3,n)*x3y3z1*120.+c(3,5,4,
     & n)*x3y3z2*240.+c(3,5,5,n)*x3y3z3*400.+c(4,2,2,n)*x4*4.+c(4,2,3,
     & n)*x4z1*12.+c(4,2,4,n)*x4z2*24.+c(4,2,5,n)*x4z3*40.+c(4,3,2,n)*
     & x4y1*12.+c(4,3,3,n)*x4y1z1*36.+c(4,3,4,n)*x4y1z2*72.+c(4,3,5,n)
     & *x4y1z3*120.+c(4,4,2,n)*x4y2*24.+c(4,4,3,n)*x4y2z1*72.+c(4,4,4,
     & n)*x4y2z2*144.+c(4,4,5,n)*x4y2z3*240.+c(4,5,2,n)*x4y3*40.+c(4,
     & 5,3,n)*x4y3z1*120.+c(4,5,4,n)*x4y3z2*240.+c(4,5,5,n)*x4y3z3*
     & 400.+c(5,2,2,n)*x5*4.+c(5,2,3,n)*x5z1*12.+c(5,2,4,n)*x5z2*24.+
     & c(5,2,5,n)*x5z3*40.+c(5,3,2,n)*x5y1*12.+c(5,3,3,n)*x5y1z1*36.+
     & c(5,3,4,n)*x5y1z2*72.+c(5,3,5,n)*x5y1z3*120.+c(5,4,2,n)*x5y2*
     & 24.+c(5,4,3,n)*x5y2z1*72.+c(5,4,4,n)*x5y2z2*144.+c(5,4,5,n)*
     & x5y2z3*240.+c(5,5,2,n)*x5y3*40.+c(5,5,3,n)*x5y3z1*120.+c(5,5,4,
     & n)*x5y3z2*240.+c(5,5,5,n)*x5y3z3*400.)*time
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
c 
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
c 
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
      r=(c(1,0,0,n)+c(1,0,1,n)*z1+c(1,0,2,n)*z2+c(1,0,3,n)*z3+c(1,0,4,
     & n)*z4+c(1,0,5,n)*z5+c(1,1,0,n)*y1+c(1,1,1,n)*y1z1+c(1,1,2,n)*
     & y1z2+c(1,1,3,n)*y1z3+c(1,1,4,n)*y1z4+c(1,1,5,n)*y1z5+c(1,2,0,n)
     & *y2+c(1,2,1,n)*y2z1+c(1,2,2,n)*y2z2+c(1,2,3,n)*y2z3+c(1,2,4,n)*
     & y2z4+c(1,2,5,n)*y2z5+c(1,3,0,n)*y3+c(1,3,1,n)*y3z1+c(1,3,2,n)*
     & y3z2+c(1,3,3,n)*y3z3+c(1,3,4,n)*y3z4+c(1,3,5,n)*y3z5+c(1,4,0,n)
     & *y4+c(1,4,1,n)*y4z1+c(1,4,2,n)*y4z2+c(1,4,3,n)*y4z3+c(1,4,4,n)*
     & y4z4+c(1,4,5,n)*y4z5+c(1,5,0,n)*y5+c(1,5,1,n)*y5z1+c(1,5,2,n)*
     & y5z2+c(1,5,3,n)*y5z3+c(1,5,4,n)*y5z4+c(1,5,5,n)*y5z5+c(2,0,0,n)
     & *x1*2.+c(2,0,1,n)*x1z1*2.+c(2,0,2,n)*x1z2*2.+c(2,0,3,n)*x1z3*
     & 2.+c(2,0,4,n)*x1z4*2.+c(2,0,5,n)*x1z5*2.+c(2,1,0,n)*x1y1*2.+c(
     & 2,1,1,n)*x1y1z1*2.+c(2,1,2,n)*x1y1z2*2.+c(2,1,3,n)*x1y1z3*2.+c(
     & 2,1,4,n)*x1y1z4*2.+c(2,1,5,n)*x1y1z5*2.+c(2,2,0,n)*x1y2*2.+c(2,
     & 2,1,n)*x1y2z1*2.+c(2,2,2,n)*x1y2z2*2.+c(2,2,3,n)*x1y2z3*2.+c(2,
     & 2,4,n)*x1y2z4*2.+c(2,2,5,n)*x1y2z5*2.+c(2,3,0,n)*x1y3*2.+c(2,3,
     & 1,n)*x1y3z1*2.+c(2,3,2,n)*x1y3z2*2.+c(2,3,3,n)*x1y3z3*2.+c(2,3,
     & 4,n)*x1y3z4*2.+c(2,3,5,n)*x1y3z5*2.+c(2,4,0,n)*x1y4*2.+c(2,4,1,
     & n)*x1y4z1*2.+c(2,4,2,n)*x1y4z2*2.+c(2,4,3,n)*x1y4z3*2.+c(2,4,4,
     & n)*x1y4z4*2.+c(2,4,5,n)*x1y4z5*2.+c(2,5,0,n)*x1y5*2.+c(2,5,1,n)
     & *x1y5z1*2.+c(2,5,2,n)*x1y5z2*2.+c(2,5,3,n)*x1y5z3*2.+c(2,5,4,n)
     & *x1y5z4*2.+c(2,5,5,n)*x1y5z5*2.+c(3,0,0,n)*x2*3.+c(3,0,1,n)*
     & x2z1*3.+c(3,0,2,n)*x2z2*3.+c(3,0,3,n)*x2z3*3.+c(3,0,4,n)*x2z4*
     & 3.+c(3,0,5,n)*x2z5*3.+c(3,1,0,n)*x2y1*3.+c(3,1,1,n)*x2y1z1*3.+
     & c(3,1,2,n)*x2y1z2*3.+c(3,1,3,n)*x2y1z3*3.+c(3,1,4,n)*x2y1z4*3.+
     & c(3,1,5,n)*x2y1z5*3.+c(3,2,0,n)*x2y2*3.+c(3,2,1,n)*x2y2z1*3.+c(
     & 3,2,2,n)*x2y2z2*3.+c(3,2,3,n)*x2y2z3*3.+c(3,2,4,n)*x2y2z4*3.+c(
     & 3,2,5,n)*x2y2z5*3.+c(3,3,0,n)*x2y3*3.+c(3,3,1,n)*x2y3z1*3.+c(3,
     & 3,2,n)*x2y3z2*3.+c(3,3,3,n)*x2y3z3*3.+c(3,3,4,n)*x2y3z4*3.+c(3,
     & 3,5,n)*x2y3z5*3.+c(3,4,0,n)*x2y4*3.+c(3,4,1,n)*x2y4z1*3.+c(3,4,
     & 2,n)*x2y4z2*3.+c(3,4,3,n)*x2y4z3*3.+c(3,4,4,n)*x2y4z4*3.+c(3,4,
     & 5,n)*x2y4z5*3.+c(3,5,0,n)*x2y5*3.+c(3,5,1,n)*x2y5z1*3.+c(3,5,2,
     & n)*x2y5z2*3.+c(3,5,3,n)*x2y5z3*3.+c(3,5,4,n)*x2y5z4*3.+c(3,5,5,
     & n)*x2y5z5*3.+c(4,0,0,n)*x3*4.+c(4,0,1,n)*x3z1*4.+c(4,0,2,n)*
     & x3z2*4.+c(4,0,3,n)*x3z3*4.+c(4,0,4,n)*x3z4*4.+c(4,0,5,n)*x3z5*
     & 4.+c(4,1,0,n)*x3y1*4.+c(4,1,1,n)*x3y1z1*4.+c(4,1,2,n)*x3y1z2*
     & 4.+c(4,1,3,n)*x3y1z3*4.+c(4,1,4,n)*x3y1z4*4.+c(4,1,5,n)*x3y1z5*
     & 4.+c(4,2,0,n)*x3y2*4.+c(4,2,1,n)*x3y2z1*4.+c(4,2,2,n)*x3y2z2*
     & 4.+c(4,2,3,n)*x3y2z3*4.+c(4,2,4,n)*x3y2z4*4.+c(4,2,5,n)*x3y2z5*
     & 4.+c(4,3,0,n)*x3y3*4.+c(4,3,1,n)*x3y3z1*4.+c(4,3,2,n)*x3y3z2*
     & 4.+c(4,3,3,n)*x3y3z3*4.+c(4,3,4,n)*x3y3z4*4.+c(4,3,5,n)*x3y3z5*
     & 4.+c(4,4,0,n)*x3y4*4.+c(4,4,1,n)*x3y4z1*4.+c(4,4,2,n)*x3y4z2*
     & 4.+c(4,4,3,n)*x3y4z3*4.+c(4,4,4,n)*x3y4z4*4.+c(4,4,5,n)*x3y4z5*
     & 4.+c(4,5,0,n)*x3y5*4.+c(4,5,1,n)*x3y5z1*4.+c(4,5,2,n)*x3y5z2*
     & 4.+c(4,5,3,n)*x3y5z3*4.+c(4,5,4,n)*x3y5z4*4.+c(4,5,5,n)*x3y5z5*
     & 4.+c(5,0,0,n)*x4*5.+c(5,0,1,n)*x4z1*5.+c(5,0,2,n)*x4z2*5.+c(5,
     & 0,3,n)*x4z3*5.+c(5,0,4,n)*x4z4*5.+c(5,0,5,n)*x4z5*5.+c(5,1,0,n)
     & *x4y1*5.+c(5,1,1,n)*x4y1z1*5.+c(5,1,2,n)*x4y1z2*5.+c(5,1,3,n)*
     & x4y1z3*5.+c(5,1,4,n)*x4y1z4*5.+c(5,1,5,n)*x4y1z5*5.+c(5,2,0,n)*
     & x4y2*5.+c(5,2,1,n)*x4y2z1*5.+c(5,2,2,n)*x4y2z2*5.+c(5,2,3,n)*
     & x4y2z3*5.+c(5,2,4,n)*x4y2z4*5.+c(5,2,5,n)*x4y2z5*5.+c(5,3,0,n)*
     & x4y3*5.+c(5,3,1,n)*x4y3z1*5.+c(5,3,2,n)*x4y3z2*5.+c(5,3,3,n)*
     & x4y3z3*5.+c(5,3,4,n)*x4y3z4*5.+c(5,3,5,n)*x4y3z5*5.+c(5,4,0,n)*
     & x4y4*5.+c(5,4,1,n)*x4y4z1*5.+c(5,4,2,n)*x4y4z2*5.+c(5,4,3,n)*
     & x4y4z3*5.+c(5,4,4,n)*x4y4z4*5.+c(5,4,5,n)*x4y4z5*5.+c(5,5,0,n)*
     & x4y5*5.+c(5,5,1,n)*x4y5z1*5.+c(5,5,2,n)*x4y5z2*5.+c(5,5,3,n)*
     & x4y5z3*5.+c(5,5,4,n)*x4y5z4*5.+c(5,5,5,n)*x4y5z5*5.)*time
! endLoops()
      else if( dx.eq.1.and.dy.eq.0.and.dz.eq.1.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
c 
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
c 
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
      r=(c(1,0,1,n)+c(1,0,2,n)*z1*2.+c(1,0,3,n)*z2*3.+c(1,0,4,n)*z3*4.+
     & c(1,0,5,n)*z4*5.+c(1,1,1,n)*y1+c(1,1,2,n)*y1z1*2.+c(1,1,3,n)*
     & y1z2*3.+c(1,1,4,n)*y1z3*4.+c(1,1,5,n)*y1z4*5.+c(1,2,1,n)*y2+c(
     & 1,2,2,n)*y2z1*2.+c(1,2,3,n)*y2z2*3.+c(1,2,4,n)*y2z3*4.+c(1,2,5,
     & n)*y2z4*5.+c(1,3,1,n)*y3+c(1,3,2,n)*y3z1*2.+c(1,3,3,n)*y3z2*3.+
     & c(1,3,4,n)*y3z3*4.+c(1,3,5,n)*y3z4*5.+c(1,4,1,n)*y4+c(1,4,2,n)*
     & y4z1*2.+c(1,4,3,n)*y4z2*3.+c(1,4,4,n)*y4z3*4.+c(1,4,5,n)*y4z4*
     & 5.+c(1,5,1,n)*y5+c(1,5,2,n)*y5z1*2.+c(1,5,3,n)*y5z2*3.+c(1,5,4,
     & n)*y5z3*4.+c(1,5,5,n)*y5z4*5.+c(2,0,1,n)*x1*2.+c(2,0,2,n)*x1z1*
     & 4.+c(2,0,3,n)*x1z2*6.+c(2,0,4,n)*x1z3*8.+c(2,0,5,n)*x1z4*10.+c(
     & 2,1,1,n)*x1y1*2.+c(2,1,2,n)*x1y1z1*4.+c(2,1,3,n)*x1y1z2*6.+c(2,
     & 1,4,n)*x1y1z3*8.+c(2,1,5,n)*x1y1z4*10.+c(2,2,1,n)*x1y2*2.+c(2,
     & 2,2,n)*x1y2z1*4.+c(2,2,3,n)*x1y2z2*6.+c(2,2,4,n)*x1y2z3*8.+c(2,
     & 2,5,n)*x1y2z4*10.+c(2,3,1,n)*x1y3*2.+c(2,3,2,n)*x1y3z1*4.+c(2,
     & 3,3,n)*x1y3z2*6.+c(2,3,4,n)*x1y3z3*8.+c(2,3,5,n)*x1y3z4*10.+c(
     & 2,4,1,n)*x1y4*2.+c(2,4,2,n)*x1y4z1*4.+c(2,4,3,n)*x1y4z2*6.+c(2,
     & 4,4,n)*x1y4z3*8.+c(2,4,5,n)*x1y4z4*10.+c(2,5,1,n)*x1y5*2.+c(2,
     & 5,2,n)*x1y5z1*4.+c(2,5,3,n)*x1y5z2*6.+c(2,5,4,n)*x1y5z3*8.+c(2,
     & 5,5,n)*x1y5z4*10.+c(3,0,1,n)*x2*3.+c(3,0,2,n)*x2z1*6.+c(3,0,3,
     & n)*x2z2*9.+c(3,0,4,n)*x2z3*12.+c(3,0,5,n)*x2z4*15.+c(3,1,1,n)*
     & x2y1*3.+c(3,1,2,n)*x2y1z1*6.+c(3,1,3,n)*x2y1z2*9.+c(3,1,4,n)*
     & x2y1z3*12.+c(3,1,5,n)*x2y1z4*15.+c(3,2,1,n)*x2y2*3.+c(3,2,2,n)*
     & x2y2z1*6.+c(3,2,3,n)*x2y2z2*9.+c(3,2,4,n)*x2y2z3*12.+c(3,2,5,n)
     & *x2y2z4*15.+c(3,3,1,n)*x2y3*3.+c(3,3,2,n)*x2y3z1*6.+c(3,3,3,n)*
     & x2y3z2*9.+c(3,3,4,n)*x2y3z3*12.+c(3,3,5,n)*x2y3z4*15.+c(3,4,1,
     & n)*x2y4*3.+c(3,4,2,n)*x2y4z1*6.+c(3,4,3,n)*x2y4z2*9.+c(3,4,4,n)
     & *x2y4z3*12.+c(3,4,5,n)*x2y4z4*15.+c(3,5,1,n)*x2y5*3.+c(3,5,2,n)
     & *x2y5z1*6.+c(3,5,3,n)*x2y5z2*9.+c(3,5,4,n)*x2y5z3*12.+c(3,5,5,
     & n)*x2y5z4*15.+c(4,0,1,n)*x3*4.+c(4,0,2,n)*x3z1*8.+c(4,0,3,n)*
     & x3z2*12.+c(4,0,4,n)*x3z3*16.+c(4,0,5,n)*x3z4*20.+c(4,1,1,n)*
     & x3y1*4.+c(4,1,2,n)*x3y1z1*8.+c(4,1,3,n)*x3y1z2*12.+c(4,1,4,n)*
     & x3y1z3*16.+c(4,1,5,n)*x3y1z4*20.+c(4,2,1,n)*x3y2*4.+c(4,2,2,n)*
     & x3y2z1*8.+c(4,2,3,n)*x3y2z2*12.+c(4,2,4,n)*x3y2z3*16.+c(4,2,5,
     & n)*x3y2z4*20.+c(4,3,1,n)*x3y3*4.+c(4,3,2,n)*x3y3z1*8.+c(4,3,3,
     & n)*x3y3z2*12.+c(4,3,4,n)*x3y3z3*16.+c(4,3,5,n)*x3y3z4*20.+c(4,
     & 4,1,n)*x3y4*4.+c(4,4,2,n)*x3y4z1*8.+c(4,4,3,n)*x3y4z2*12.+c(4,
     & 4,4,n)*x3y4z3*16.+c(4,4,5,n)*x3y4z4*20.+c(4,5,1,n)*x3y5*4.+c(4,
     & 5,2,n)*x3y5z1*8.+c(4,5,3,n)*x3y5z2*12.+c(4,5,4,n)*x3y5z3*16.+c(
     & 4,5,5,n)*x3y5z4*20.+c(5,0,1,n)*x4*5.+c(5,0,2,n)*x4z1*10.+c(5,0,
     & 3,n)*x4z2*15.+c(5,0,4,n)*x4z3*20.+c(5,0,5,n)*x4z4*25.+c(5,1,1,
     & n)*x4y1*5.+c(5,1,2,n)*x4y1z1*10.+c(5,1,3,n)*x4y1z2*15.+c(5,1,4,
     & n)*x4y1z3*20.+c(5,1,5,n)*x4y1z4*25.+c(5,2,1,n)*x4y2*5.+c(5,2,2,
     & n)*x4y2z1*10.+c(5,2,3,n)*x4y2z2*15.+c(5,2,4,n)*x4y2z3*20.+c(5,
     & 2,5,n)*x4y2z4*25.+c(5,3,1,n)*x4y3*5.+c(5,3,2,n)*x4y3z1*10.+c(5,
     & 3,3,n)*x4y3z2*15.+c(5,3,4,n)*x4y3z3*20.+c(5,3,5,n)*x4y3z4*25.+
     & c(5,4,1,n)*x4y4*5.+c(5,4,2,n)*x4y4z1*10.+c(5,4,3,n)*x4y4z2*15.+
     & c(5,4,4,n)*x4y4z3*20.+c(5,4,5,n)*x4y4z4*25.+c(5,5,1,n)*x4y5*5.+
     & c(5,5,2,n)*x4y5z1*10.+c(5,5,3,n)*x4y5z2*15.+c(5,5,4,n)*x4y5z3*
     & 20.+c(5,5,5,n)*x4y5z4*25.)*time
! endLoops()
      else if( dx.eq.1.and.dy.eq.0.and.dz.eq.2.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
c 
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
c 
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
      r=(c(1,0,2,n)*2.+c(1,0,3,n)*z1*6.+c(1,0,4,n)*z2*12.+c(1,0,5,n)*
     & z3*20.+c(1,1,2,n)*y1*2.+c(1,1,3,n)*y1z1*6.+c(1,1,4,n)*y1z2*12.+
     & c(1,1,5,n)*y1z3*20.+c(1,2,2,n)*y2*2.+c(1,2,3,n)*y2z1*6.+c(1,2,
     & 4,n)*y2z2*12.+c(1,2,5,n)*y2z3*20.+c(1,3,2,n)*y3*2.+c(1,3,3,n)*
     & y3z1*6.+c(1,3,4,n)*y3z2*12.+c(1,3,5,n)*y3z3*20.+c(1,4,2,n)*y4*
     & 2.+c(1,4,3,n)*y4z1*6.+c(1,4,4,n)*y4z2*12.+c(1,4,5,n)*y4z3*20.+
     & c(1,5,2,n)*y5*2.+c(1,5,3,n)*y5z1*6.+c(1,5,4,n)*y5z2*12.+c(1,5,
     & 5,n)*y5z3*20.+c(2,0,2,n)*x1*4.+c(2,0,3,n)*x1z1*12.+c(2,0,4,n)*
     & x1z2*24.+c(2,0,5,n)*x1z3*40.+c(2,1,2,n)*x1y1*4.+c(2,1,3,n)*
     & x1y1z1*12.+c(2,1,4,n)*x1y1z2*24.+c(2,1,5,n)*x1y1z3*40.+c(2,2,2,
     & n)*x1y2*4.+c(2,2,3,n)*x1y2z1*12.+c(2,2,4,n)*x1y2z2*24.+c(2,2,5,
     & n)*x1y2z3*40.+c(2,3,2,n)*x1y3*4.+c(2,3,3,n)*x1y3z1*12.+c(2,3,4,
     & n)*x1y3z2*24.+c(2,3,5,n)*x1y3z3*40.+c(2,4,2,n)*x1y4*4.+c(2,4,3,
     & n)*x1y4z1*12.+c(2,4,4,n)*x1y4z2*24.+c(2,4,5,n)*x1y4z3*40.+c(2,
     & 5,2,n)*x1y5*4.+c(2,5,3,n)*x1y5z1*12.+c(2,5,4,n)*x1y5z2*24.+c(2,
     & 5,5,n)*x1y5z3*40.+c(3,0,2,n)*x2*6.+c(3,0,3,n)*x2z1*18.+c(3,0,4,
     & n)*x2z2*36.+c(3,0,5,n)*x2z3*60.+c(3,1,2,n)*x2y1*6.+c(3,1,3,n)*
     & x2y1z1*18.+c(3,1,4,n)*x2y1z2*36.+c(3,1,5,n)*x2y1z3*60.+c(3,2,2,
     & n)*x2y2*6.+c(3,2,3,n)*x2y2z1*18.+c(3,2,4,n)*x2y2z2*36.+c(3,2,5,
     & n)*x2y2z3*60.+c(3,3,2,n)*x2y3*6.+c(3,3,3,n)*x2y3z1*18.+c(3,3,4,
     & n)*x2y3z2*36.+c(3,3,5,n)*x2y3z3*60.+c(3,4,2,n)*x2y4*6.+c(3,4,3,
     & n)*x2y4z1*18.+c(3,4,4,n)*x2y4z2*36.+c(3,4,5,n)*x2y4z3*60.+c(3,
     & 5,2,n)*x2y5*6.+c(3,5,3,n)*x2y5z1*18.+c(3,5,4,n)*x2y5z2*36.+c(3,
     & 5,5,n)*x2y5z3*60.+c(4,0,2,n)*x3*8.+c(4,0,3,n)*x3z1*24.+c(4,0,4,
     & n)*x3z2*48.+c(4,0,5,n)*x3z3*80.+c(4,1,2,n)*x3y1*8.+c(4,1,3,n)*
     & x3y1z1*24.+c(4,1,4,n)*x3y1z2*48.+c(4,1,5,n)*x3y1z3*80.+c(4,2,2,
     & n)*x3y2*8.+c(4,2,3,n)*x3y2z1*24.+c(4,2,4,n)*x3y2z2*48.+c(4,2,5,
     & n)*x3y2z3*80.+c(4,3,2,n)*x3y3*8.+c(4,3,3,n)*x3y3z1*24.+c(4,3,4,
     & n)*x3y3z2*48.+c(4,3,5,n)*x3y3z3*80.+c(4,4,2,n)*x3y4*8.+c(4,4,3,
     & n)*x3y4z1*24.+c(4,4,4,n)*x3y4z2*48.+c(4,4,5,n)*x3y4z3*80.+c(4,
     & 5,2,n)*x3y5*8.+c(4,5,3,n)*x3y5z1*24.+c(4,5,4,n)*x3y5z2*48.+c(4,
     & 5,5,n)*x3y5z3*80.+c(5,0,2,n)*x4*10.+c(5,0,3,n)*x4z1*30.+c(5,0,
     & 4,n)*x4z2*60.+c(5,0,5,n)*x4z3*100.+c(5,1,2,n)*x4y1*10.+c(5,1,3,
     & n)*x4y1z1*30.+c(5,1,4,n)*x4y1z2*60.+c(5,1,5,n)*x4y1z3*100.+c(5,
     & 2,2,n)*x4y2*10.+c(5,2,3,n)*x4y2z1*30.+c(5,2,4,n)*x4y2z2*60.+c(
     & 5,2,5,n)*x4y2z3*100.+c(5,3,2,n)*x4y3*10.+c(5,3,3,n)*x4y3z1*30.+
     & c(5,3,4,n)*x4y3z2*60.+c(5,3,5,n)*x4y3z3*100.+c(5,4,2,n)*x4y4*
     & 10.+c(5,4,3,n)*x4y4z1*30.+c(5,4,4,n)*x4y4z2*60.+c(5,4,5,n)*
     & x4y4z3*100.+c(5,5,2,n)*x4y5*10.+c(5,5,3,n)*x4y5z1*30.+c(5,5,4,
     & n)*x4y5z2*60.+c(5,5,5,n)*x4y5z3*100.)*time
! endLoops()
      else if( dx.eq.1.and.dy.eq.1.and.dz.eq.0.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
c 
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
c 
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
      r=(c(1,1,0,n)+c(1,1,1,n)*z1+c(1,1,2,n)*z2+c(1,1,3,n)*z3+c(1,1,4,
     & n)*z4+c(1,1,5,n)*z5+c(1,2,0,n)*y1*2.+c(1,2,1,n)*y1z1*2.+c(1,2,
     & 2,n)*y1z2*2.+c(1,2,3,n)*y1z3*2.+c(1,2,4,n)*y1z4*2.+c(1,2,5,n)*
     & y1z5*2.+c(1,3,0,n)*y2*3.+c(1,3,1,n)*y2z1*3.+c(1,3,2,n)*y2z2*3.+
     & c(1,3,3,n)*y2z3*3.+c(1,3,4,n)*y2z4*3.+c(1,3,5,n)*y2z5*3.+c(1,4,
     & 0,n)*y3*4.+c(1,4,1,n)*y3z1*4.+c(1,4,2,n)*y3z2*4.+c(1,4,3,n)*
     & y3z3*4.+c(1,4,4,n)*y3z4*4.+c(1,4,5,n)*y3z5*4.+c(1,5,0,n)*y4*5.+
     & c(1,5,1,n)*y4z1*5.+c(1,5,2,n)*y4z2*5.+c(1,5,3,n)*y4z3*5.+c(1,5,
     & 4,n)*y4z4*5.+c(1,5,5,n)*y4z5*5.+c(2,1,0,n)*x1*2.+c(2,1,1,n)*
     & x1z1*2.+c(2,1,2,n)*x1z2*2.+c(2,1,3,n)*x1z3*2.+c(2,1,4,n)*x1z4*
     & 2.+c(2,1,5,n)*x1z5*2.+c(2,2,0,n)*x1y1*4.+c(2,2,1,n)*x1y1z1*4.+
     & c(2,2,2,n)*x1y1z2*4.+c(2,2,3,n)*x1y1z3*4.+c(2,2,4,n)*x1y1z4*4.+
     & c(2,2,5,n)*x1y1z5*4.+c(2,3,0,n)*x1y2*6.+c(2,3,1,n)*x1y2z1*6.+c(
     & 2,3,2,n)*x1y2z2*6.+c(2,3,3,n)*x1y2z3*6.+c(2,3,4,n)*x1y2z4*6.+c(
     & 2,3,5,n)*x1y2z5*6.+c(2,4,0,n)*x1y3*8.+c(2,4,1,n)*x1y3z1*8.+c(2,
     & 4,2,n)*x1y3z2*8.+c(2,4,3,n)*x1y3z3*8.+c(2,4,4,n)*x1y3z4*8.+c(2,
     & 4,5,n)*x1y3z5*8.+c(2,5,0,n)*x1y4*10.+c(2,5,1,n)*x1y4z1*10.+c(2,
     & 5,2,n)*x1y4z2*10.+c(2,5,3,n)*x1y4z3*10.+c(2,5,4,n)*x1y4z4*10.+
     & c(2,5,5,n)*x1y4z5*10.+c(3,1,0,n)*x2*3.+c(3,1,1,n)*x2z1*3.+c(3,
     & 1,2,n)*x2z2*3.+c(3,1,3,n)*x2z3*3.+c(3,1,4,n)*x2z4*3.+c(3,1,5,n)
     & *x2z5*3.+c(3,2,0,n)*x2y1*6.+c(3,2,1,n)*x2y1z1*6.+c(3,2,2,n)*
     & x2y1z2*6.+c(3,2,3,n)*x2y1z3*6.+c(3,2,4,n)*x2y1z4*6.+c(3,2,5,n)*
     & x2y1z5*6.+c(3,3,0,n)*x2y2*9.+c(3,3,1,n)*x2y2z1*9.+c(3,3,2,n)*
     & x2y2z2*9.+c(3,3,3,n)*x2y2z3*9.+c(3,3,4,n)*x2y2z4*9.+c(3,3,5,n)*
     & x2y2z5*9.+c(3,4,0,n)*x2y3*12.+c(3,4,1,n)*x2y3z1*12.+c(3,4,2,n)*
     & x2y3z2*12.+c(3,4,3,n)*x2y3z3*12.+c(3,4,4,n)*x2y3z4*12.+c(3,4,5,
     & n)*x2y3z5*12.+c(3,5,0,n)*x2y4*15.+c(3,5,1,n)*x2y4z1*15.+c(3,5,
     & 2,n)*x2y4z2*15.+c(3,5,3,n)*x2y4z3*15.+c(3,5,4,n)*x2y4z4*15.+c(
     & 3,5,5,n)*x2y4z5*15.+c(4,1,0,n)*x3*4.+c(4,1,1,n)*x3z1*4.+c(4,1,
     & 2,n)*x3z2*4.+c(4,1,3,n)*x3z3*4.+c(4,1,4,n)*x3z4*4.+c(4,1,5,n)*
     & x3z5*4.+c(4,2,0,n)*x3y1*8.+c(4,2,1,n)*x3y1z1*8.+c(4,2,2,n)*
     & x3y1z2*8.+c(4,2,3,n)*x3y1z3*8.+c(4,2,4,n)*x3y1z4*8.+c(4,2,5,n)*
     & x3y1z5*8.+c(4,3,0,n)*x3y2*12.+c(4,3,1,n)*x3y2z1*12.+c(4,3,2,n)*
     & x3y2z2*12.+c(4,3,3,n)*x3y2z3*12.+c(4,3,4,n)*x3y2z4*12.+c(4,3,5,
     & n)*x3y2z5*12.+c(4,4,0,n)*x3y3*16.+c(4,4,1,n)*x3y3z1*16.+c(4,4,
     & 2,n)*x3y3z2*16.+c(4,4,3,n)*x3y3z3*16.+c(4,4,4,n)*x3y3z4*16.+c(
     & 4,4,5,n)*x3y3z5*16.+c(4,5,0,n)*x3y4*20.+c(4,5,1,n)*x3y4z1*20.+
     & c(4,5,2,n)*x3y4z2*20.+c(4,5,3,n)*x3y4z3*20.+c(4,5,4,n)*x3y4z4*
     & 20.+c(4,5,5,n)*x3y4z5*20.+c(5,1,0,n)*x4*5.+c(5,1,1,n)*x4z1*5.+
     & c(5,1,2,n)*x4z2*5.+c(5,1,3,n)*x4z3*5.+c(5,1,4,n)*x4z4*5.+c(5,1,
     & 5,n)*x4z5*5.+c(5,2,0,n)*x4y1*10.+c(5,2,1,n)*x4y1z1*10.+c(5,2,2,
     & n)*x4y1z2*10.+c(5,2,3,n)*x4y1z3*10.+c(5,2,4,n)*x4y1z4*10.+c(5,
     & 2,5,n)*x4y1z5*10.+c(5,3,0,n)*x4y2*15.+c(5,3,1,n)*x4y2z1*15.+c(
     & 5,3,2,n)*x4y2z2*15.+c(5,3,3,n)*x4y2z3*15.+c(5,3,4,n)*x4y2z4*
     & 15.+c(5,3,5,n)*x4y2z5*15.+c(5,4,0,n)*x4y3*20.+c(5,4,1,n)*
     & x4y3z1*20.+c(5,4,2,n)*x4y3z2*20.+c(5,4,3,n)*x4y3z3*20.+c(5,4,4,
     & n)*x4y3z4*20.+c(5,4,5,n)*x4y3z5*20.+c(5,5,0,n)*x4y4*25.+c(5,5,
     & 1,n)*x4y4z1*25.+c(5,5,2,n)*x4y4z2*25.+c(5,5,3,n)*x4y4z3*25.+c(
     & 5,5,4,n)*x4y4z4*25.+c(5,5,5,n)*x4y4z5*25.)*time
! endLoops()
      else if( dx.eq.1.and.dy.eq.1.and.dz.eq.1.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
c 
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
c 
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
      r=(c(1,1,1,n)+c(1,1,2,n)*z1*2.+c(1,1,3,n)*z2*3.+c(1,1,4,n)*z3*4.+
     & c(1,1,5,n)*z4*5.+c(1,2,1,n)*y1*2.+c(1,2,2,n)*y1z1*4.+c(1,2,3,n)
     & *y1z2*6.+c(1,2,4,n)*y1z3*8.+c(1,2,5,n)*y1z4*10.+c(1,3,1,n)*y2*
     & 3.+c(1,3,2,n)*y2z1*6.+c(1,3,3,n)*y2z2*9.+c(1,3,4,n)*y2z3*12.+c(
     & 1,3,5,n)*y2z4*15.+c(1,4,1,n)*y3*4.+c(1,4,2,n)*y3z1*8.+c(1,4,3,
     & n)*y3z2*12.+c(1,4,4,n)*y3z3*16.+c(1,4,5,n)*y3z4*20.+c(1,5,1,n)*
     & y4*5.+c(1,5,2,n)*y4z1*10.+c(1,5,3,n)*y4z2*15.+c(1,5,4,n)*y4z3*
     & 20.+c(1,5,5,n)*y4z4*25.+c(2,1,1,n)*x1*2.+c(2,1,2,n)*x1z1*4.+c(
     & 2,1,3,n)*x1z2*6.+c(2,1,4,n)*x1z3*8.+c(2,1,5,n)*x1z4*10.+c(2,2,
     & 1,n)*x1y1*4.+c(2,2,2,n)*x1y1z1*8.+c(2,2,3,n)*x1y1z2*12.+c(2,2,
     & 4,n)*x1y1z3*16.+c(2,2,5,n)*x1y1z4*20.+c(2,3,1,n)*x1y2*6.+c(2,3,
     & 2,n)*x1y2z1*12.+c(2,3,3,n)*x1y2z2*18.+c(2,3,4,n)*x1y2z3*24.+c(
     & 2,3,5,n)*x1y2z4*30.+c(2,4,1,n)*x1y3*8.+c(2,4,2,n)*x1y3z1*16.+c(
     & 2,4,3,n)*x1y3z2*24.+c(2,4,4,n)*x1y3z3*32.+c(2,4,5,n)*x1y3z4*
     & 40.+c(2,5,1,n)*x1y4*10.+c(2,5,2,n)*x1y4z1*20.+c(2,5,3,n)*
     & x1y4z2*30.+c(2,5,4,n)*x1y4z3*40.+c(2,5,5,n)*x1y4z4*50.+c(3,1,1,
     & n)*x2*3.+c(3,1,2,n)*x2z1*6.+c(3,1,3,n)*x2z2*9.+c(3,1,4,n)*x2z3*
     & 12.+c(3,1,5,n)*x2z4*15.+c(3,2,1,n)*x2y1*6.+c(3,2,2,n)*x2y1z1*
     & 12.+c(3,2,3,n)*x2y1z2*18.+c(3,2,4,n)*x2y1z3*24.+c(3,2,5,n)*
     & x2y1z4*30.+c(3,3,1,n)*x2y2*9.+c(3,3,2,n)*x2y2z1*18.+c(3,3,3,n)*
     & x2y2z2*27.+c(3,3,4,n)*x2y2z3*36.+c(3,3,5,n)*x2y2z4*45.+c(3,4,1,
     & n)*x2y3*12.+c(3,4,2,n)*x2y3z1*24.+c(3,4,3,n)*x2y3z2*36.+c(3,4,
     & 4,n)*x2y3z3*48.+c(3,4,5,n)*x2y3z4*60.+c(3,5,1,n)*x2y4*15.+c(3,
     & 5,2,n)*x2y4z1*30.+c(3,5,3,n)*x2y4z2*45.+c(3,5,4,n)*x2y4z3*60.+
     & c(3,5,5,n)*x2y4z4*75.+c(4,1,1,n)*x3*4.+c(4,1,2,n)*x3z1*8.+c(4,
     & 1,3,n)*x3z2*12.+c(4,1,4,n)*x3z3*16.+c(4,1,5,n)*x3z4*20.+c(4,2,
     & 1,n)*x3y1*8.+c(4,2,2,n)*x3y1z1*16.+c(4,2,3,n)*x3y1z2*24.+c(4,2,
     & 4,n)*x3y1z3*32.+c(4,2,5,n)*x3y1z4*40.+c(4,3,1,n)*x3y2*12.+c(4,
     & 3,2,n)*x3y2z1*24.+c(4,3,3,n)*x3y2z2*36.+c(4,3,4,n)*x3y2z3*48.+
     & c(4,3,5,n)*x3y2z4*60.+c(4,4,1,n)*x3y3*16.+c(4,4,2,n)*x3y3z1*
     & 32.+c(4,4,3,n)*x3y3z2*48.+c(4,4,4,n)*x3y3z3*64.+c(4,4,5,n)*
     & x3y3z4*80.+c(4,5,1,n)*x3y4*20.+c(4,5,2,n)*x3y4z1*40.+c(4,5,3,n)
     & *x3y4z2*60.+c(4,5,4,n)*x3y4z3*80.+c(4,5,5,n)*x3y4z4*100.+c(5,1,
     & 1,n)*x4*5.+c(5,1,2,n)*x4z1*10.+c(5,1,3,n)*x4z2*15.+c(5,1,4,n)*
     & x4z3*20.+c(5,1,5,n)*x4z4*25.+c(5,2,1,n)*x4y1*10.+c(5,2,2,n)*
     & x4y1z1*20.+c(5,2,3,n)*x4y1z2*30.+c(5,2,4,n)*x4y1z3*40.+c(5,2,5,
     & n)*x4y1z4*50.+c(5,3,1,n)*x4y2*15.+c(5,3,2,n)*x4y2z1*30.+c(5,3,
     & 3,n)*x4y2z2*45.+c(5,3,4,n)*x4y2z3*60.+c(5,3,5,n)*x4y2z4*75.+c(
     & 5,4,1,n)*x4y3*20.+c(5,4,2,n)*x4y3z1*40.+c(5,4,3,n)*x4y3z2*60.+
     & c(5,4,4,n)*x4y3z3*80.+c(5,4,5,n)*x4y3z4*100.+c(5,5,1,n)*x4y4*
     & 25.+c(5,5,2,n)*x4y4z1*50.+c(5,5,3,n)*x4y4z2*75.+c(5,5,4,n)*
     & x4y4z3*100.+c(5,5,5,n)*x4y4z4*125.)*time
! endLoops()
      else if( dx.eq.1.and.dy.eq.1.and.dz.eq.2.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
c 
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
c 
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
      r=(c(1,1,2,n)*2.+c(1,1,3,n)*z1*6.+c(1,1,4,n)*z2*12.+c(1,1,5,n)*
     & z3*20.+c(1,2,2,n)*y1*4.+c(1,2,3,n)*y1z1*12.+c(1,2,4,n)*y1z2*
     & 24.+c(1,2,5,n)*y1z3*40.+c(1,3,2,n)*y2*6.+c(1,3,3,n)*y2z1*18.+c(
     & 1,3,4,n)*y2z2*36.+c(1,3,5,n)*y2z3*60.+c(1,4,2,n)*y3*8.+c(1,4,3,
     & n)*y3z1*24.+c(1,4,4,n)*y3z2*48.+c(1,4,5,n)*y3z3*80.+c(1,5,2,n)*
     & y4*10.+c(1,5,3,n)*y4z1*30.+c(1,5,4,n)*y4z2*60.+c(1,5,5,n)*y4z3*
     & 100.+c(2,1,2,n)*x1*4.+c(2,1,3,n)*x1z1*12.+c(2,1,4,n)*x1z2*24.+
     & c(2,1,5,n)*x1z3*40.+c(2,2,2,n)*x1y1*8.+c(2,2,3,n)*x1y1z1*24.+c(
     & 2,2,4,n)*x1y1z2*48.+c(2,2,5,n)*x1y1z3*80.+c(2,3,2,n)*x1y2*12.+
     & c(2,3,3,n)*x1y2z1*36.+c(2,3,4,n)*x1y2z2*72.+c(2,3,5,n)*x1y2z3*
     & 120.+c(2,4,2,n)*x1y3*16.+c(2,4,3,n)*x1y3z1*48.+c(2,4,4,n)*
     & x1y3z2*96.+c(2,4,5,n)*x1y3z3*160.+c(2,5,2,n)*x1y4*20.+c(2,5,3,
     & n)*x1y4z1*60.+c(2,5,4,n)*x1y4z2*120.+c(2,5,5,n)*x1y4z3*200.+c(
     & 3,1,2,n)*x2*6.+c(3,1,3,n)*x2z1*18.+c(3,1,4,n)*x2z2*36.+c(3,1,5,
     & n)*x2z3*60.+c(3,2,2,n)*x2y1*12.+c(3,2,3,n)*x2y1z1*36.+c(3,2,4,
     & n)*x2y1z2*72.+c(3,2,5,n)*x2y1z3*120.+c(3,3,2,n)*x2y2*18.+c(3,3,
     & 3,n)*x2y2z1*54.+c(3,3,4,n)*x2y2z2*108.+c(3,3,5,n)*x2y2z3*180.+
     & c(3,4,2,n)*x2y3*24.+c(3,4,3,n)*x2y3z1*72.+c(3,4,4,n)*x2y3z2*
     & 144.+c(3,4,5,n)*x2y3z3*240.+c(3,5,2,n)*x2y4*30.+c(3,5,3,n)*
     & x2y4z1*90.+c(3,5,4,n)*x2y4z2*180.+c(3,5,5,n)*x2y4z3*300.+c(4,1,
     & 2,n)*x3*8.+c(4,1,3,n)*x3z1*24.+c(4,1,4,n)*x3z2*48.+c(4,1,5,n)*
     & x3z3*80.+c(4,2,2,n)*x3y1*16.+c(4,2,3,n)*x3y1z1*48.+c(4,2,4,n)*
     & x3y1z2*96.+c(4,2,5,n)*x3y1z3*160.+c(4,3,2,n)*x3y2*24.+c(4,3,3,
     & n)*x3y2z1*72.+c(4,3,4,n)*x3y2z2*144.+c(4,3,5,n)*x3y2z3*240.+c(
     & 4,4,2,n)*x3y3*32.+c(4,4,3,n)*x3y3z1*96.+c(4,4,4,n)*x3y3z2*192.+
     & c(4,4,5,n)*x3y3z3*320.+c(4,5,2,n)*x3y4*40.+c(4,5,3,n)*x3y4z1*
     & 120.+c(4,5,4,n)*x3y4z2*240.+c(4,5,5,n)*x3y4z3*400.+c(5,1,2,n)*
     & x4*10.+c(5,1,3,n)*x4z1*30.+c(5,1,4,n)*x4z2*60.+c(5,1,5,n)*x4z3*
     & 100.+c(5,2,2,n)*x4y1*20.+c(5,2,3,n)*x4y1z1*60.+c(5,2,4,n)*
     & x4y1z2*120.+c(5,2,5,n)*x4y1z3*200.+c(5,3,2,n)*x4y2*30.+c(5,3,3,
     & n)*x4y2z1*90.+c(5,3,4,n)*x4y2z2*180.+c(5,3,5,n)*x4y2z3*300.+c(
     & 5,4,2,n)*x4y3*40.+c(5,4,3,n)*x4y3z1*120.+c(5,4,4,n)*x4y3z2*
     & 240.+c(5,4,5,n)*x4y3z3*400.+c(5,5,2,n)*x4y4*50.+c(5,5,3,n)*
     & x4y4z1*150.+c(5,5,4,n)*x4y4z2*300.+c(5,5,5,n)*x4y4z3*500.)*time
! endLoops()
      else if( dx.eq.1.and.dy.eq.2.and.dz.eq.0.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
c 
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
c 
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
      r=(c(1,2,0,n)*2.+c(1,2,1,n)*z1*2.+c(1,2,2,n)*z2*2.+c(1,2,3,n)*z3*
     & 2.+c(1,2,4,n)*z4*2.+c(1,2,5,n)*z5*2.+c(1,3,0,n)*y1*6.+c(1,3,1,
     & n)*y1z1*6.+c(1,3,2,n)*y1z2*6.+c(1,3,3,n)*y1z3*6.+c(1,3,4,n)*
     & y1z4*6.+c(1,3,5,n)*y1z5*6.+c(1,4,0,n)*y2*12.+c(1,4,1,n)*y2z1*
     & 12.+c(1,4,2,n)*y2z2*12.+c(1,4,3,n)*y2z3*12.+c(1,4,4,n)*y2z4*
     & 12.+c(1,4,5,n)*y2z5*12.+c(1,5,0,n)*y3*20.+c(1,5,1,n)*y3z1*20.+
     & c(1,5,2,n)*y3z2*20.+c(1,5,3,n)*y3z3*20.+c(1,5,4,n)*y3z4*20.+c(
     & 1,5,5,n)*y3z5*20.+c(2,2,0,n)*x1*4.+c(2,2,1,n)*x1z1*4.+c(2,2,2,
     & n)*x1z2*4.+c(2,2,3,n)*x1z3*4.+c(2,2,4,n)*x1z4*4.+c(2,2,5,n)*
     & x1z5*4.+c(2,3,0,n)*x1y1*12.+c(2,3,1,n)*x1y1z1*12.+c(2,3,2,n)*
     & x1y1z2*12.+c(2,3,3,n)*x1y1z3*12.+c(2,3,4,n)*x1y1z4*12.+c(2,3,5,
     & n)*x1y1z5*12.+c(2,4,0,n)*x1y2*24.+c(2,4,1,n)*x1y2z1*24.+c(2,4,
     & 2,n)*x1y2z2*24.+c(2,4,3,n)*x1y2z3*24.+c(2,4,4,n)*x1y2z4*24.+c(
     & 2,4,5,n)*x1y2z5*24.+c(2,5,0,n)*x1y3*40.+c(2,5,1,n)*x1y3z1*40.+
     & c(2,5,2,n)*x1y3z2*40.+c(2,5,3,n)*x1y3z3*40.+c(2,5,4,n)*x1y3z4*
     & 40.+c(2,5,5,n)*x1y3z5*40.+c(3,2,0,n)*x2*6.+c(3,2,1,n)*x2z1*6.+
     & c(3,2,2,n)*x2z2*6.+c(3,2,3,n)*x2z3*6.+c(3,2,4,n)*x2z4*6.+c(3,2,
     & 5,n)*x2z5*6.+c(3,3,0,n)*x2y1*18.+c(3,3,1,n)*x2y1z1*18.+c(3,3,2,
     & n)*x2y1z2*18.+c(3,3,3,n)*x2y1z3*18.+c(3,3,4,n)*x2y1z4*18.+c(3,
     & 3,5,n)*x2y1z5*18.+c(3,4,0,n)*x2y2*36.+c(3,4,1,n)*x2y2z1*36.+c(
     & 3,4,2,n)*x2y2z2*36.+c(3,4,3,n)*x2y2z3*36.+c(3,4,4,n)*x2y2z4*
     & 36.+c(3,4,5,n)*x2y2z5*36.+c(3,5,0,n)*x2y3*60.+c(3,5,1,n)*
     & x2y3z1*60.+c(3,5,2,n)*x2y3z2*60.+c(3,5,3,n)*x2y3z3*60.+c(3,5,4,
     & n)*x2y3z4*60.+c(3,5,5,n)*x2y3z5*60.+c(4,2,0,n)*x3*8.+c(4,2,1,n)
     & *x3z1*8.+c(4,2,2,n)*x3z2*8.+c(4,2,3,n)*x3z3*8.+c(4,2,4,n)*x3z4*
     & 8.+c(4,2,5,n)*x3z5*8.+c(4,3,0,n)*x3y1*24.+c(4,3,1,n)*x3y1z1*
     & 24.+c(4,3,2,n)*x3y1z2*24.+c(4,3,3,n)*x3y1z3*24.+c(4,3,4,n)*
     & x3y1z4*24.+c(4,3,5,n)*x3y1z5*24.+c(4,4,0,n)*x3y2*48.+c(4,4,1,n)
     & *x3y2z1*48.+c(4,4,2,n)*x3y2z2*48.+c(4,4,3,n)*x3y2z3*48.+c(4,4,
     & 4,n)*x3y2z4*48.+c(4,4,5,n)*x3y2z5*48.+c(4,5,0,n)*x3y3*80.+c(4,
     & 5,1,n)*x3y3z1*80.+c(4,5,2,n)*x3y3z2*80.+c(4,5,3,n)*x3y3z3*80.+
     & c(4,5,4,n)*x3y3z4*80.+c(4,5,5,n)*x3y3z5*80.+c(5,2,0,n)*x4*10.+
     & c(5,2,1,n)*x4z1*10.+c(5,2,2,n)*x4z2*10.+c(5,2,3,n)*x4z3*10.+c(
     & 5,2,4,n)*x4z4*10.+c(5,2,5,n)*x4z5*10.+c(5,3,0,n)*x4y1*30.+c(5,
     & 3,1,n)*x4y1z1*30.+c(5,3,2,n)*x4y1z2*30.+c(5,3,3,n)*x4y1z3*30.+
     & c(5,3,4,n)*x4y1z4*30.+c(5,3,5,n)*x4y1z5*30.+c(5,4,0,n)*x4y2*
     & 60.+c(5,4,1,n)*x4y2z1*60.+c(5,4,2,n)*x4y2z2*60.+c(5,4,3,n)*
     & x4y2z3*60.+c(5,4,4,n)*x4y2z4*60.+c(5,4,5,n)*x4y2z5*60.+c(5,5,0,
     & n)*x4y3*100.+c(5,5,1,n)*x4y3z1*100.+c(5,5,2,n)*x4y3z2*100.+c(5,
     & 5,3,n)*x4y3z3*100.+c(5,5,4,n)*x4y3z4*100.+c(5,5,5,n)*x4y3z5*
     & 100.)*time
! endLoops()
      else if( dx.eq.1.and.dy.eq.2.and.dz.eq.1.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
c 
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
c 
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
      r=(c(1,2,1,n)*2.+c(1,2,2,n)*z1*4.+c(1,2,3,n)*z2*6.+c(1,2,4,n)*z3*
     & 8.+c(1,2,5,n)*z4*10.+c(1,3,1,n)*y1*6.+c(1,3,2,n)*y1z1*12.+c(1,
     & 3,3,n)*y1z2*18.+c(1,3,4,n)*y1z3*24.+c(1,3,5,n)*y1z4*30.+c(1,4,
     & 1,n)*y2*12.+c(1,4,2,n)*y2z1*24.+c(1,4,3,n)*y2z2*36.+c(1,4,4,n)*
     & y2z3*48.+c(1,4,5,n)*y2z4*60.+c(1,5,1,n)*y3*20.+c(1,5,2,n)*y3z1*
     & 40.+c(1,5,3,n)*y3z2*60.+c(1,5,4,n)*y3z3*80.+c(1,5,5,n)*y3z4*
     & 100.+c(2,2,1,n)*x1*4.+c(2,2,2,n)*x1z1*8.+c(2,2,3,n)*x1z2*12.+c(
     & 2,2,4,n)*x1z3*16.+c(2,2,5,n)*x1z4*20.+c(2,3,1,n)*x1y1*12.+c(2,
     & 3,2,n)*x1y1z1*24.+c(2,3,3,n)*x1y1z2*36.+c(2,3,4,n)*x1y1z3*48.+
     & c(2,3,5,n)*x1y1z4*60.+c(2,4,1,n)*x1y2*24.+c(2,4,2,n)*x1y2z1*
     & 48.+c(2,4,3,n)*x1y2z2*72.+c(2,4,4,n)*x1y2z3*96.+c(2,4,5,n)*
     & x1y2z4*120.+c(2,5,1,n)*x1y3*40.+c(2,5,2,n)*x1y3z1*80.+c(2,5,3,
     & n)*x1y3z2*120.+c(2,5,4,n)*x1y3z3*160.+c(2,5,5,n)*x1y3z4*200.+c(
     & 3,2,1,n)*x2*6.+c(3,2,2,n)*x2z1*12.+c(3,2,3,n)*x2z2*18.+c(3,2,4,
     & n)*x2z3*24.+c(3,2,5,n)*x2z4*30.+c(3,3,1,n)*x2y1*18.+c(3,3,2,n)*
     & x2y1z1*36.+c(3,3,3,n)*x2y1z2*54.+c(3,3,4,n)*x2y1z3*72.+c(3,3,5,
     & n)*x2y1z4*90.+c(3,4,1,n)*x2y2*36.+c(3,4,2,n)*x2y2z1*72.+c(3,4,
     & 3,n)*x2y2z2*108.+c(3,4,4,n)*x2y2z3*144.+c(3,4,5,n)*x2y2z4*180.+
     & c(3,5,1,n)*x2y3*60.+c(3,5,2,n)*x2y3z1*120.+c(3,5,3,n)*x2y3z2*
     & 180.+c(3,5,4,n)*x2y3z3*240.+c(3,5,5,n)*x2y3z4*300.+c(4,2,1,n)*
     & x3*8.+c(4,2,2,n)*x3z1*16.+c(4,2,3,n)*x3z2*24.+c(4,2,4,n)*x3z3*
     & 32.+c(4,2,5,n)*x3z4*40.+c(4,3,1,n)*x3y1*24.+c(4,3,2,n)*x3y1z1*
     & 48.+c(4,3,3,n)*x3y1z2*72.+c(4,3,4,n)*x3y1z3*96.+c(4,3,5,n)*
     & x3y1z4*120.+c(4,4,1,n)*x3y2*48.+c(4,4,2,n)*x3y2z1*96.+c(4,4,3,
     & n)*x3y2z2*144.+c(4,4,4,n)*x3y2z3*192.+c(4,4,5,n)*x3y2z4*240.+c(
     & 4,5,1,n)*x3y3*80.+c(4,5,2,n)*x3y3z1*160.+c(4,5,3,n)*x3y3z2*
     & 240.+c(4,5,4,n)*x3y3z3*320.+c(4,5,5,n)*x3y3z4*400.+c(5,2,1,n)*
     & x4*10.+c(5,2,2,n)*x4z1*20.+c(5,2,3,n)*x4z2*30.+c(5,2,4,n)*x4z3*
     & 40.+c(5,2,5,n)*x4z4*50.+c(5,3,1,n)*x4y1*30.+c(5,3,2,n)*x4y1z1*
     & 60.+c(5,3,3,n)*x4y1z2*90.+c(5,3,4,n)*x4y1z3*120.+c(5,3,5,n)*
     & x4y1z4*150.+c(5,4,1,n)*x4y2*60.+c(5,4,2,n)*x4y2z1*120.+c(5,4,3,
     & n)*x4y2z2*180.+c(5,4,4,n)*x4y2z3*240.+c(5,4,5,n)*x4y2z4*300.+c(
     & 5,5,1,n)*x4y3*100.+c(5,5,2,n)*x4y3z1*200.+c(5,5,3,n)*x4y3z2*
     & 300.+c(5,5,4,n)*x4y3z3*400.+c(5,5,5,n)*x4y3z4*500.)*time
! endLoops()
      else if( dx.eq.1.and.dy.eq.2.and.dz.eq.2.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
c 
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
c 
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
      r=(c(1,2,2,n)*4.+c(1,2,3,n)*z1*12.+c(1,2,4,n)*z2*24.+c(1,2,5,n)*
     & z3*40.+c(1,3,2,n)*y1*12.+c(1,3,3,n)*y1z1*36.+c(1,3,4,n)*y1z2*
     & 72.+c(1,3,5,n)*y1z3*120.+c(1,4,2,n)*y2*24.+c(1,4,3,n)*y2z1*72.+
     & c(1,4,4,n)*y2z2*144.+c(1,4,5,n)*y2z3*240.+c(1,5,2,n)*y3*40.+c(
     & 1,5,3,n)*y3z1*120.+c(1,5,4,n)*y3z2*240.+c(1,5,5,n)*y3z3*400.+c(
     & 2,2,2,n)*x1*8.+c(2,2,3,n)*x1z1*24.+c(2,2,4,n)*x1z2*48.+c(2,2,5,
     & n)*x1z3*80.+c(2,3,2,n)*x1y1*24.+c(2,3,3,n)*x1y1z1*72.+c(2,3,4,
     & n)*x1y1z2*144.+c(2,3,5,n)*x1y1z3*240.+c(2,4,2,n)*x1y2*48.+c(2,
     & 4,3,n)*x1y2z1*144.+c(2,4,4,n)*x1y2z2*288.+c(2,4,5,n)*x1y2z3*
     & 480.+c(2,5,2,n)*x1y3*80.+c(2,5,3,n)*x1y3z1*240.+c(2,5,4,n)*
     & x1y3z2*480.+c(2,5,5,n)*x1y3z3*800.+c(3,2,2,n)*x2*12.+c(3,2,3,n)
     & *x2z1*36.+c(3,2,4,n)*x2z2*72.+c(3,2,5,n)*x2z3*120.+c(3,3,2,n)*
     & x2y1*36.+c(3,3,3,n)*x2y1z1*108.+c(3,3,4,n)*x2y1z2*216.+c(3,3,5,
     & n)*x2y1z3*360.+c(3,4,2,n)*x2y2*72.+c(3,4,3,n)*x2y2z1*216.+c(3,
     & 4,4,n)*x2y2z2*432.+c(3,4,5,n)*x2y2z3*720.+c(3,5,2,n)*x2y3*120.+
     & c(3,5,3,n)*x2y3z1*360.+c(3,5,4,n)*x2y3z2*720.+c(3,5,5,n)*
     & x2y3z3*1200.+c(4,2,2,n)*x3*16.+c(4,2,3,n)*x3z1*48.+c(4,2,4,n)*
     & x3z2*96.+c(4,2,5,n)*x3z3*160.+c(4,3,2,n)*x3y1*48.+c(4,3,3,n)*
     & x3y1z1*144.+c(4,3,4,n)*x3y1z2*288.+c(4,3,5,n)*x3y1z3*480.+c(4,
     & 4,2,n)*x3y2*96.+c(4,4,3,n)*x3y2z1*288.+c(4,4,4,n)*x3y2z2*576.+
     & c(4,4,5,n)*x3y2z3*960.+c(4,5,2,n)*x3y3*160.+c(4,5,3,n)*x3y3z1*
     & 480.+c(4,5,4,n)*x3y3z2*960.+c(4,5,5,n)*x3y3z3*1600.+c(5,2,2,n)*
     & x4*20.+c(5,2,3,n)*x4z1*60.+c(5,2,4,n)*x4z2*120.+c(5,2,5,n)*
     & x4z3*200.+c(5,3,2,n)*x4y1*60.+c(5,3,3,n)*x4y1z1*180.+c(5,3,4,n)
     & *x4y1z2*360.+c(5,3,5,n)*x4y1z3*600.+c(5,4,2,n)*x4y2*120.+c(5,4,
     & 3,n)*x4y2z1*360.+c(5,4,4,n)*x4y2z2*720.+c(5,4,5,n)*x4y2z3*
     & 1200.+c(5,5,2,n)*x4y3*200.+c(5,5,3,n)*x4y3z1*600.+c(5,5,4,n)*
     & x4y3z2*1200.+c(5,5,5,n)*x4y3z3*2000.)*time
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
c 
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
c 
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
      r=(c(2,0,0,n)*2.+c(2,0,1,n)*z1*2.+c(2,0,2,n)*z2*2.+c(2,0,3,n)*z3*
     & 2.+c(2,0,4,n)*z4*2.+c(2,0,5,n)*z5*2.+c(2,1,0,n)*y1*2.+c(2,1,1,
     & n)*y1z1*2.+c(2,1,2,n)*y1z2*2.+c(2,1,3,n)*y1z3*2.+c(2,1,4,n)*
     & y1z4*2.+c(2,1,5,n)*y1z5*2.+c(2,2,0,n)*y2*2.+c(2,2,1,n)*y2z1*2.+
     & c(2,2,2,n)*y2z2*2.+c(2,2,3,n)*y2z3*2.+c(2,2,4,n)*y2z4*2.+c(2,2,
     & 5,n)*y2z5*2.+c(2,3,0,n)*y3*2.+c(2,3,1,n)*y3z1*2.+c(2,3,2,n)*
     & y3z2*2.+c(2,3,3,n)*y3z3*2.+c(2,3,4,n)*y3z4*2.+c(2,3,5,n)*y3z5*
     & 2.+c(2,4,0,n)*y4*2.+c(2,4,1,n)*y4z1*2.+c(2,4,2,n)*y4z2*2.+c(2,
     & 4,3,n)*y4z3*2.+c(2,4,4,n)*y4z4*2.+c(2,4,5,n)*y4z5*2.+c(2,5,0,n)
     & *y5*2.+c(2,5,1,n)*y5z1*2.+c(2,5,2,n)*y5z2*2.+c(2,5,3,n)*y5z3*
     & 2.+c(2,5,4,n)*y5z4*2.+c(2,5,5,n)*y5z5*2.+c(3,0,0,n)*x1*6.+c(3,
     & 0,1,n)*x1z1*6.+c(3,0,2,n)*x1z2*6.+c(3,0,3,n)*x1z3*6.+c(3,0,4,n)
     & *x1z4*6.+c(3,0,5,n)*x1z5*6.+c(3,1,0,n)*x1y1*6.+c(3,1,1,n)*
     & x1y1z1*6.+c(3,1,2,n)*x1y1z2*6.+c(3,1,3,n)*x1y1z3*6.+c(3,1,4,n)*
     & x1y1z4*6.+c(3,1,5,n)*x1y1z5*6.+c(3,2,0,n)*x1y2*6.+c(3,2,1,n)*
     & x1y2z1*6.+c(3,2,2,n)*x1y2z2*6.+c(3,2,3,n)*x1y2z3*6.+c(3,2,4,n)*
     & x1y2z4*6.+c(3,2,5,n)*x1y2z5*6.+c(3,3,0,n)*x1y3*6.+c(3,3,1,n)*
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
      else if( dx.eq.2.and.dy.eq.0.and.dz.eq.1.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
c 
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
c 
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
      r=(c(2,0,1,n)*2.+c(2,0,2,n)*z1*4.+c(2,0,3,n)*z2*6.+c(2,0,4,n)*z3*
     & 8.+c(2,0,5,n)*z4*10.+c(2,1,1,n)*y1*2.+c(2,1,2,n)*y1z1*4.+c(2,1,
     & 3,n)*y1z2*6.+c(2,1,4,n)*y1z3*8.+c(2,1,5,n)*y1z4*10.+c(2,2,1,n)*
     & y2*2.+c(2,2,2,n)*y2z1*4.+c(2,2,3,n)*y2z2*6.+c(2,2,4,n)*y2z3*8.+
     & c(2,2,5,n)*y2z4*10.+c(2,3,1,n)*y3*2.+c(2,3,2,n)*y3z1*4.+c(2,3,
     & 3,n)*y3z2*6.+c(2,3,4,n)*y3z3*8.+c(2,3,5,n)*y3z4*10.+c(2,4,1,n)*
     & y4*2.+c(2,4,2,n)*y4z1*4.+c(2,4,3,n)*y4z2*6.+c(2,4,4,n)*y4z3*8.+
     & c(2,4,5,n)*y4z4*10.+c(2,5,1,n)*y5*2.+c(2,5,2,n)*y5z1*4.+c(2,5,
     & 3,n)*y5z2*6.+c(2,5,4,n)*y5z3*8.+c(2,5,5,n)*y5z4*10.+c(3,0,1,n)*
     & x1*6.+c(3,0,2,n)*x1z1*12.+c(3,0,3,n)*x1z2*18.+c(3,0,4,n)*x1z3*
     & 24.+c(3,0,5,n)*x1z4*30.+c(3,1,1,n)*x1y1*6.+c(3,1,2,n)*x1y1z1*
     & 12.+c(3,1,3,n)*x1y1z2*18.+c(3,1,4,n)*x1y1z3*24.+c(3,1,5,n)*
     & x1y1z4*30.+c(3,2,1,n)*x1y2*6.+c(3,2,2,n)*x1y2z1*12.+c(3,2,3,n)*
     & x1y2z2*18.+c(3,2,4,n)*x1y2z3*24.+c(3,2,5,n)*x1y2z4*30.+c(3,3,1,
     & n)*x1y3*6.+c(3,3,2,n)*x1y3z1*12.+c(3,3,3,n)*x1y3z2*18.+c(3,3,4,
     & n)*x1y3z3*24.+c(3,3,5,n)*x1y3z4*30.+c(3,4,1,n)*x1y4*6.+c(3,4,2,
     & n)*x1y4z1*12.+c(3,4,3,n)*x1y4z2*18.+c(3,4,4,n)*x1y4z3*24.+c(3,
     & 4,5,n)*x1y4z4*30.+c(3,5,1,n)*x1y5*6.+c(3,5,2,n)*x1y5z1*12.+c(3,
     & 5,3,n)*x1y5z2*18.+c(3,5,4,n)*x1y5z3*24.+c(3,5,5,n)*x1y5z4*30.+
     & c(4,0,1,n)*x2*12.+c(4,0,2,n)*x2z1*24.+c(4,0,3,n)*x2z2*36.+c(4,
     & 0,4,n)*x2z3*48.+c(4,0,5,n)*x2z4*60.+c(4,1,1,n)*x2y1*12.+c(4,1,
     & 2,n)*x2y1z1*24.+c(4,1,3,n)*x2y1z2*36.+c(4,1,4,n)*x2y1z3*48.+c(
     & 4,1,5,n)*x2y1z4*60.+c(4,2,1,n)*x2y2*12.+c(4,2,2,n)*x2y2z1*24.+
     & c(4,2,3,n)*x2y2z2*36.+c(4,2,4,n)*x2y2z3*48.+c(4,2,5,n)*x2y2z4*
     & 60.+c(4,3,1,n)*x2y3*12.+c(4,3,2,n)*x2y3z1*24.+c(4,3,3,n)*
     & x2y3z2*36.+c(4,3,4,n)*x2y3z3*48.+c(4,3,5,n)*x2y3z4*60.+c(4,4,1,
     & n)*x2y4*12.+c(4,4,2,n)*x2y4z1*24.+c(4,4,3,n)*x2y4z2*36.+c(4,4,
     & 4,n)*x2y4z3*48.+c(4,4,5,n)*x2y4z4*60.+c(4,5,1,n)*x2y5*12.+c(4,
     & 5,2,n)*x2y5z1*24.+c(4,5,3,n)*x2y5z2*36.+c(4,5,4,n)*x2y5z3*48.+
     & c(4,5,5,n)*x2y5z4*60.+c(5,0,1,n)*x3*20.+c(5,0,2,n)*x3z1*40.+c(
     & 5,0,3,n)*x3z2*60.+c(5,0,4,n)*x3z3*80.+c(5,0,5,n)*x3z4*100.+c(5,
     & 1,1,n)*x3y1*20.+c(5,1,2,n)*x3y1z1*40.+c(5,1,3,n)*x3y1z2*60.+c(
     & 5,1,4,n)*x3y1z3*80.+c(5,1,5,n)*x3y1z4*100.+c(5,2,1,n)*x3y2*20.+
     & c(5,2,2,n)*x3y2z1*40.+c(5,2,3,n)*x3y2z2*60.+c(5,2,4,n)*x3y2z3*
     & 80.+c(5,2,5,n)*x3y2z4*100.+c(5,3,1,n)*x3y3*20.+c(5,3,2,n)*
     & x3y3z1*40.+c(5,3,3,n)*x3y3z2*60.+c(5,3,4,n)*x3y3z3*80.+c(5,3,5,
     & n)*x3y3z4*100.+c(5,4,1,n)*x3y4*20.+c(5,4,2,n)*x3y4z1*40.+c(5,4,
     & 3,n)*x3y4z2*60.+c(5,4,4,n)*x3y4z3*80.+c(5,4,5,n)*x3y4z4*100.+c(
     & 5,5,1,n)*x3y5*20.+c(5,5,2,n)*x3y5z1*40.+c(5,5,3,n)*x3y5z2*60.+
     & c(5,5,4,n)*x3y5z3*80.+c(5,5,5,n)*x3y5z4*100.)*time
! endLoops()
      else if( dx.eq.2.and.dy.eq.0.and.dz.eq.2.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
c 
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
c 
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
      r=(c(2,0,2,n)*4.+c(2,0,3,n)*z1*12.+c(2,0,4,n)*z2*24.+c(2,0,5,n)*
     & z3*40.+c(2,1,2,n)*y1*4.+c(2,1,3,n)*y1z1*12.+c(2,1,4,n)*y1z2*
     & 24.+c(2,1,5,n)*y1z3*40.+c(2,2,2,n)*y2*4.+c(2,2,3,n)*y2z1*12.+c(
     & 2,2,4,n)*y2z2*24.+c(2,2,5,n)*y2z3*40.+c(2,3,2,n)*y3*4.+c(2,3,3,
     & n)*y3z1*12.+c(2,3,4,n)*y3z2*24.+c(2,3,5,n)*y3z3*40.+c(2,4,2,n)*
     & y4*4.+c(2,4,3,n)*y4z1*12.+c(2,4,4,n)*y4z2*24.+c(2,4,5,n)*y4z3*
     & 40.+c(2,5,2,n)*y5*4.+c(2,5,3,n)*y5z1*12.+c(2,5,4,n)*y5z2*24.+c(
     & 2,5,5,n)*y5z3*40.+c(3,0,2,n)*x1*12.+c(3,0,3,n)*x1z1*36.+c(3,0,
     & 4,n)*x1z2*72.+c(3,0,5,n)*x1z3*120.+c(3,1,2,n)*x1y1*12.+c(3,1,3,
     & n)*x1y1z1*36.+c(3,1,4,n)*x1y1z2*72.+c(3,1,5,n)*x1y1z3*120.+c(3,
     & 2,2,n)*x1y2*12.+c(3,2,3,n)*x1y2z1*36.+c(3,2,4,n)*x1y2z2*72.+c(
     & 3,2,5,n)*x1y2z3*120.+c(3,3,2,n)*x1y3*12.+c(3,3,3,n)*x1y3z1*36.+
     & c(3,3,4,n)*x1y3z2*72.+c(3,3,5,n)*x1y3z3*120.+c(3,4,2,n)*x1y4*
     & 12.+c(3,4,3,n)*x1y4z1*36.+c(3,4,4,n)*x1y4z2*72.+c(3,4,5,n)*
     & x1y4z3*120.+c(3,5,2,n)*x1y5*12.+c(3,5,3,n)*x1y5z1*36.+c(3,5,4,
     & n)*x1y5z2*72.+c(3,5,5,n)*x1y5z3*120.+c(4,0,2,n)*x2*24.+c(4,0,3,
     & n)*x2z1*72.+c(4,0,4,n)*x2z2*144.+c(4,0,5,n)*x2z3*240.+c(4,1,2,
     & n)*x2y1*24.+c(4,1,3,n)*x2y1z1*72.+c(4,1,4,n)*x2y1z2*144.+c(4,1,
     & 5,n)*x2y1z3*240.+c(4,2,2,n)*x2y2*24.+c(4,2,3,n)*x2y2z1*72.+c(4,
     & 2,4,n)*x2y2z2*144.+c(4,2,5,n)*x2y2z3*240.+c(4,3,2,n)*x2y3*24.+
     & c(4,3,3,n)*x2y3z1*72.+c(4,3,4,n)*x2y3z2*144.+c(4,3,5,n)*x2y3z3*
     & 240.+c(4,4,2,n)*x2y4*24.+c(4,4,3,n)*x2y4z1*72.+c(4,4,4,n)*
     & x2y4z2*144.+c(4,4,5,n)*x2y4z3*240.+c(4,5,2,n)*x2y5*24.+c(4,5,3,
     & n)*x2y5z1*72.+c(4,5,4,n)*x2y5z2*144.+c(4,5,5,n)*x2y5z3*240.+c(
     & 5,0,2,n)*x3*40.+c(5,0,3,n)*x3z1*120.+c(5,0,4,n)*x3z2*240.+c(5,
     & 0,5,n)*x3z3*400.+c(5,1,2,n)*x3y1*40.+c(5,1,3,n)*x3y1z1*120.+c(
     & 5,1,4,n)*x3y1z2*240.+c(5,1,5,n)*x3y1z3*400.+c(5,2,2,n)*x3y2*
     & 40.+c(5,2,3,n)*x3y2z1*120.+c(5,2,4,n)*x3y2z2*240.+c(5,2,5,n)*
     & x3y2z3*400.+c(5,3,2,n)*x3y3*40.+c(5,3,3,n)*x3y3z1*120.+c(5,3,4,
     & n)*x3y3z2*240.+c(5,3,5,n)*x3y3z3*400.+c(5,4,2,n)*x3y4*40.+c(5,
     & 4,3,n)*x3y4z1*120.+c(5,4,4,n)*x3y4z2*240.+c(5,4,5,n)*x3y4z3*
     & 400.+c(5,5,2,n)*x3y5*40.+c(5,5,3,n)*x3y5z1*120.+c(5,5,4,n)*
     & x3y5z2*240.+c(5,5,5,n)*x3y5z3*400.)*time
! endLoops()
      else if( dx.eq.2.and.dy.eq.1.and.dz.eq.0.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
c 
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
c 
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
      r=(c(2,1,0,n)*2.+c(2,1,1,n)*z1*2.+c(2,1,2,n)*z2*2.+c(2,1,3,n)*z3*
     & 2.+c(2,1,4,n)*z4*2.+c(2,1,5,n)*z5*2.+c(2,2,0,n)*y1*4.+c(2,2,1,
     & n)*y1z1*4.+c(2,2,2,n)*y1z2*4.+c(2,2,3,n)*y1z3*4.+c(2,2,4,n)*
     & y1z4*4.+c(2,2,5,n)*y1z5*4.+c(2,3,0,n)*y2*6.+c(2,3,1,n)*y2z1*6.+
     & c(2,3,2,n)*y2z2*6.+c(2,3,3,n)*y2z3*6.+c(2,3,4,n)*y2z4*6.+c(2,3,
     & 5,n)*y2z5*6.+c(2,4,0,n)*y3*8.+c(2,4,1,n)*y3z1*8.+c(2,4,2,n)*
     & y3z2*8.+c(2,4,3,n)*y3z3*8.+c(2,4,4,n)*y3z4*8.+c(2,4,5,n)*y3z5*
     & 8.+c(2,5,0,n)*y4*10.+c(2,5,1,n)*y4z1*10.+c(2,5,2,n)*y4z2*10.+c(
     & 2,5,3,n)*y4z3*10.+c(2,5,4,n)*y4z4*10.+c(2,5,5,n)*y4z5*10.+c(3,
     & 1,0,n)*x1*6.+c(3,1,1,n)*x1z1*6.+c(3,1,2,n)*x1z2*6.+c(3,1,3,n)*
     & x1z3*6.+c(3,1,4,n)*x1z4*6.+c(3,1,5,n)*x1z5*6.+c(3,2,0,n)*x1y1*
     & 12.+c(3,2,1,n)*x1y1z1*12.+c(3,2,2,n)*x1y1z2*12.+c(3,2,3,n)*
     & x1y1z3*12.+c(3,2,4,n)*x1y1z4*12.+c(3,2,5,n)*x1y1z5*12.+c(3,3,0,
     & n)*x1y2*18.+c(3,3,1,n)*x1y2z1*18.+c(3,3,2,n)*x1y2z2*18.+c(3,3,
     & 3,n)*x1y2z3*18.+c(3,3,4,n)*x1y2z4*18.+c(3,3,5,n)*x1y2z5*18.+c(
     & 3,4,0,n)*x1y3*24.+c(3,4,1,n)*x1y3z1*24.+c(3,4,2,n)*x1y3z2*24.+
     & c(3,4,3,n)*x1y3z3*24.+c(3,4,4,n)*x1y3z4*24.+c(3,4,5,n)*x1y3z5*
     & 24.+c(3,5,0,n)*x1y4*30.+c(3,5,1,n)*x1y4z1*30.+c(3,5,2,n)*
     & x1y4z2*30.+c(3,5,3,n)*x1y4z3*30.+c(3,5,4,n)*x1y4z4*30.+c(3,5,5,
     & n)*x1y4z5*30.+c(4,1,0,n)*x2*12.+c(4,1,1,n)*x2z1*12.+c(4,1,2,n)*
     & x2z2*12.+c(4,1,3,n)*x2z3*12.+c(4,1,4,n)*x2z4*12.+c(4,1,5,n)*
     & x2z5*12.+c(4,2,0,n)*x2y1*24.+c(4,2,1,n)*x2y1z1*24.+c(4,2,2,n)*
     & x2y1z2*24.+c(4,2,3,n)*x2y1z3*24.+c(4,2,4,n)*x2y1z4*24.+c(4,2,5,
     & n)*x2y1z5*24.+c(4,3,0,n)*x2y2*36.+c(4,3,1,n)*x2y2z1*36.+c(4,3,
     & 2,n)*x2y2z2*36.+c(4,3,3,n)*x2y2z3*36.+c(4,3,4,n)*x2y2z4*36.+c(
     & 4,3,5,n)*x2y2z5*36.+c(4,4,0,n)*x2y3*48.+c(4,4,1,n)*x2y3z1*48.+
     & c(4,4,2,n)*x2y3z2*48.+c(4,4,3,n)*x2y3z3*48.+c(4,4,4,n)*x2y3z4*
     & 48.+c(4,4,5,n)*x2y3z5*48.+c(4,5,0,n)*x2y4*60.+c(4,5,1,n)*
     & x2y4z1*60.+c(4,5,2,n)*x2y4z2*60.+c(4,5,3,n)*x2y4z3*60.+c(4,5,4,
     & n)*x2y4z4*60.+c(4,5,5,n)*x2y4z5*60.+c(5,1,0,n)*x3*20.+c(5,1,1,
     & n)*x3z1*20.+c(5,1,2,n)*x3z2*20.+c(5,1,3,n)*x3z3*20.+c(5,1,4,n)*
     & x3z4*20.+c(5,1,5,n)*x3z5*20.+c(5,2,0,n)*x3y1*40.+c(5,2,1,n)*
     & x3y1z1*40.+c(5,2,2,n)*x3y1z2*40.+c(5,2,3,n)*x3y1z3*40.+c(5,2,4,
     & n)*x3y1z4*40.+c(5,2,5,n)*x3y1z5*40.+c(5,3,0,n)*x3y2*60.+c(5,3,
     & 1,n)*x3y2z1*60.+c(5,3,2,n)*x3y2z2*60.+c(5,3,3,n)*x3y2z3*60.+c(
     & 5,3,4,n)*x3y2z4*60.+c(5,3,5,n)*x3y2z5*60.+c(5,4,0,n)*x3y3*80.+
     & c(5,4,1,n)*x3y3z1*80.+c(5,4,2,n)*x3y3z2*80.+c(5,4,3,n)*x3y3z3*
     & 80.+c(5,4,4,n)*x3y3z4*80.+c(5,4,5,n)*x3y3z5*80.+c(5,5,0,n)*
     & x3y4*100.+c(5,5,1,n)*x3y4z1*100.+c(5,5,2,n)*x3y4z2*100.+c(5,5,
     & 3,n)*x3y4z3*100.+c(5,5,4,n)*x3y4z4*100.+c(5,5,5,n)*x3y4z5*100.)
     & *time
! endLoops()
      else if( dx.eq.2.and.dy.eq.1.and.dz.eq.1.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
c 
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
c 
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
      r=(c(2,1,1,n)*2.+c(2,1,2,n)*z1*4.+c(2,1,3,n)*z2*6.+c(2,1,4,n)*z3*
     & 8.+c(2,1,5,n)*z4*10.+c(2,2,1,n)*y1*4.+c(2,2,2,n)*y1z1*8.+c(2,2,
     & 3,n)*y1z2*12.+c(2,2,4,n)*y1z3*16.+c(2,2,5,n)*y1z4*20.+c(2,3,1,
     & n)*y2*6.+c(2,3,2,n)*y2z1*12.+c(2,3,3,n)*y2z2*18.+c(2,3,4,n)*
     & y2z3*24.+c(2,3,5,n)*y2z4*30.+c(2,4,1,n)*y3*8.+c(2,4,2,n)*y3z1*
     & 16.+c(2,4,3,n)*y3z2*24.+c(2,4,4,n)*y3z3*32.+c(2,4,5,n)*y3z4*
     & 40.+c(2,5,1,n)*y4*10.+c(2,5,2,n)*y4z1*20.+c(2,5,3,n)*y4z2*30.+
     & c(2,5,4,n)*y4z3*40.+c(2,5,5,n)*y4z4*50.+c(3,1,1,n)*x1*6.+c(3,1,
     & 2,n)*x1z1*12.+c(3,1,3,n)*x1z2*18.+c(3,1,4,n)*x1z3*24.+c(3,1,5,
     & n)*x1z4*30.+c(3,2,1,n)*x1y1*12.+c(3,2,2,n)*x1y1z1*24.+c(3,2,3,
     & n)*x1y1z2*36.+c(3,2,4,n)*x1y1z3*48.+c(3,2,5,n)*x1y1z4*60.+c(3,
     & 3,1,n)*x1y2*18.+c(3,3,2,n)*x1y2z1*36.+c(3,3,3,n)*x1y2z2*54.+c(
     & 3,3,4,n)*x1y2z3*72.+c(3,3,5,n)*x1y2z4*90.+c(3,4,1,n)*x1y3*24.+
     & c(3,4,2,n)*x1y3z1*48.+c(3,4,3,n)*x1y3z2*72.+c(3,4,4,n)*x1y3z3*
     & 96.+c(3,4,5,n)*x1y3z4*120.+c(3,5,1,n)*x1y4*30.+c(3,5,2,n)*
     & x1y4z1*60.+c(3,5,3,n)*x1y4z2*90.+c(3,5,4,n)*x1y4z3*120.+c(3,5,
     & 5,n)*x1y4z4*150.+c(4,1,1,n)*x2*12.+c(4,1,2,n)*x2z1*24.+c(4,1,3,
     & n)*x2z2*36.+c(4,1,4,n)*x2z3*48.+c(4,1,5,n)*x2z4*60.+c(4,2,1,n)*
     & x2y1*24.+c(4,2,2,n)*x2y1z1*48.+c(4,2,3,n)*x2y1z2*72.+c(4,2,4,n)
     & *x2y1z3*96.+c(4,2,5,n)*x2y1z4*120.+c(4,3,1,n)*x2y2*36.+c(4,3,2,
     & n)*x2y2z1*72.+c(4,3,3,n)*x2y2z2*108.+c(4,3,4,n)*x2y2z3*144.+c(
     & 4,3,5,n)*x2y2z4*180.+c(4,4,1,n)*x2y3*48.+c(4,4,2,n)*x2y3z1*96.+
     & c(4,4,3,n)*x2y3z2*144.+c(4,4,4,n)*x2y3z3*192.+c(4,4,5,n)*
     & x2y3z4*240.+c(4,5,1,n)*x2y4*60.+c(4,5,2,n)*x2y4z1*120.+c(4,5,3,
     & n)*x2y4z2*180.+c(4,5,4,n)*x2y4z3*240.+c(4,5,5,n)*x2y4z4*300.+c(
     & 5,1,1,n)*x3*20.+c(5,1,2,n)*x3z1*40.+c(5,1,3,n)*x3z2*60.+c(5,1,
     & 4,n)*x3z3*80.+c(5,1,5,n)*x3z4*100.+c(5,2,1,n)*x3y1*40.+c(5,2,2,
     & n)*x3y1z1*80.+c(5,2,3,n)*x3y1z2*120.+c(5,2,4,n)*x3y1z3*160.+c(
     & 5,2,5,n)*x3y1z4*200.+c(5,3,1,n)*x3y2*60.+c(5,3,2,n)*x3y2z1*
     & 120.+c(5,3,3,n)*x3y2z2*180.+c(5,3,4,n)*x3y2z3*240.+c(5,3,5,n)*
     & x3y2z4*300.+c(5,4,1,n)*x3y3*80.+c(5,4,2,n)*x3y3z1*160.+c(5,4,3,
     & n)*x3y3z2*240.+c(5,4,4,n)*x3y3z3*320.+c(5,4,5,n)*x3y3z4*400.+c(
     & 5,5,1,n)*x3y4*100.+c(5,5,2,n)*x3y4z1*200.+c(5,5,3,n)*x3y4z2*
     & 300.+c(5,5,4,n)*x3y4z3*400.+c(5,5,5,n)*x3y4z4*500.)*time
! endLoops()
      else if( dx.eq.2.and.dy.eq.1.and.dz.eq.2.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
c 
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
c 
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
      r=(c(2,1,2,n)*4.+c(2,1,3,n)*z1*12.+c(2,1,4,n)*z2*24.+c(2,1,5,n)*
     & z3*40.+c(2,2,2,n)*y1*8.+c(2,2,3,n)*y1z1*24.+c(2,2,4,n)*y1z2*
     & 48.+c(2,2,5,n)*y1z3*80.+c(2,3,2,n)*y2*12.+c(2,3,3,n)*y2z1*36.+
     & c(2,3,4,n)*y2z2*72.+c(2,3,5,n)*y2z3*120.+c(2,4,2,n)*y3*16.+c(2,
     & 4,3,n)*y3z1*48.+c(2,4,4,n)*y3z2*96.+c(2,4,5,n)*y3z3*160.+c(2,5,
     & 2,n)*y4*20.+c(2,5,3,n)*y4z1*60.+c(2,5,4,n)*y4z2*120.+c(2,5,5,n)
     & *y4z3*200.+c(3,1,2,n)*x1*12.+c(3,1,3,n)*x1z1*36.+c(3,1,4,n)*
     & x1z2*72.+c(3,1,5,n)*x1z3*120.+c(3,2,2,n)*x1y1*24.+c(3,2,3,n)*
     & x1y1z1*72.+c(3,2,4,n)*x1y1z2*144.+c(3,2,5,n)*x1y1z3*240.+c(3,3,
     & 2,n)*x1y2*36.+c(3,3,3,n)*x1y2z1*108.+c(3,3,4,n)*x1y2z2*216.+c(
     & 3,3,5,n)*x1y2z3*360.+c(3,4,2,n)*x1y3*48.+c(3,4,3,n)*x1y3z1*
     & 144.+c(3,4,4,n)*x1y3z2*288.+c(3,4,5,n)*x1y3z3*480.+c(3,5,2,n)*
     & x1y4*60.+c(3,5,3,n)*x1y4z1*180.+c(3,5,4,n)*x1y4z2*360.+c(3,5,5,
     & n)*x1y4z3*600.+c(4,1,2,n)*x2*24.+c(4,1,3,n)*x2z1*72.+c(4,1,4,n)
     & *x2z2*144.+c(4,1,5,n)*x2z3*240.+c(4,2,2,n)*x2y1*48.+c(4,2,3,n)*
     & x2y1z1*144.+c(4,2,4,n)*x2y1z2*288.+c(4,2,5,n)*x2y1z3*480.+c(4,
     & 3,2,n)*x2y2*72.+c(4,3,3,n)*x2y2z1*216.+c(4,3,4,n)*x2y2z2*432.+
     & c(4,3,5,n)*x2y2z3*720.+c(4,4,2,n)*x2y3*96.+c(4,4,3,n)*x2y3z1*
     & 288.+c(4,4,4,n)*x2y3z2*576.+c(4,4,5,n)*x2y3z3*960.+c(4,5,2,n)*
     & x2y4*120.+c(4,5,3,n)*x2y4z1*360.+c(4,5,4,n)*x2y4z2*720.+c(4,5,
     & 5,n)*x2y4z3*1200.+c(5,1,2,n)*x3*40.+c(5,1,3,n)*x3z1*120.+c(5,1,
     & 4,n)*x3z2*240.+c(5,1,5,n)*x3z3*400.+c(5,2,2,n)*x3y1*80.+c(5,2,
     & 3,n)*x3y1z1*240.+c(5,2,4,n)*x3y1z2*480.+c(5,2,5,n)*x3y1z3*800.+
     & c(5,3,2,n)*x3y2*120.+c(5,3,3,n)*x3y2z1*360.+c(5,3,4,n)*x3y2z2*
     & 720.+c(5,3,5,n)*x3y2z3*1200.+c(5,4,2,n)*x3y3*160.+c(5,4,3,n)*
     & x3y3z1*480.+c(5,4,4,n)*x3y3z2*960.+c(5,4,5,n)*x3y3z3*1600.+c(5,
     & 5,2,n)*x3y4*200.+c(5,5,3,n)*x3y4z1*600.+c(5,5,4,n)*x3y4z2*
     & 1200.+c(5,5,5,n)*x3y4z3*2000.)*time
! endLoops()
      else if( dx.eq.2.and.dy.eq.2.and.dz.eq.0.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
c 
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
c 
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
      r=(c(2,2,0,n)*4.+c(2,2,1,n)*z1*4.+c(2,2,2,n)*z2*4.+c(2,2,3,n)*z3*
     & 4.+c(2,2,4,n)*z4*4.+c(2,2,5,n)*z5*4.+c(2,3,0,n)*y1*12.+c(2,3,1,
     & n)*y1z1*12.+c(2,3,2,n)*y1z2*12.+c(2,3,3,n)*y1z3*12.+c(2,3,4,n)*
     & y1z4*12.+c(2,3,5,n)*y1z5*12.+c(2,4,0,n)*y2*24.+c(2,4,1,n)*y2z1*
     & 24.+c(2,4,2,n)*y2z2*24.+c(2,4,3,n)*y2z3*24.+c(2,4,4,n)*y2z4*
     & 24.+c(2,4,5,n)*y2z5*24.+c(2,5,0,n)*y3*40.+c(2,5,1,n)*y3z1*40.+
     & c(2,5,2,n)*y3z2*40.+c(2,5,3,n)*y3z3*40.+c(2,5,4,n)*y3z4*40.+c(
     & 2,5,5,n)*y3z5*40.+c(3,2,0,n)*x1*12.+c(3,2,1,n)*x1z1*12.+c(3,2,
     & 2,n)*x1z2*12.+c(3,2,3,n)*x1z3*12.+c(3,2,4,n)*x1z4*12.+c(3,2,5,
     & n)*x1z5*12.+c(3,3,0,n)*x1y1*36.+c(3,3,1,n)*x1y1z1*36.+c(3,3,2,
     & n)*x1y1z2*36.+c(3,3,3,n)*x1y1z3*36.+c(3,3,4,n)*x1y1z4*36.+c(3,
     & 3,5,n)*x1y1z5*36.+c(3,4,0,n)*x1y2*72.+c(3,4,1,n)*x1y2z1*72.+c(
     & 3,4,2,n)*x1y2z2*72.+c(3,4,3,n)*x1y2z3*72.+c(3,4,4,n)*x1y2z4*
     & 72.+c(3,4,5,n)*x1y2z5*72.+c(3,5,0,n)*x1y3*120.+c(3,5,1,n)*
     & x1y3z1*120.+c(3,5,2,n)*x1y3z2*120.+c(3,5,3,n)*x1y3z3*120.+c(3,
     & 5,4,n)*x1y3z4*120.+c(3,5,5,n)*x1y3z5*120.+c(4,2,0,n)*x2*24.+c(
     & 4,2,1,n)*x2z1*24.+c(4,2,2,n)*x2z2*24.+c(4,2,3,n)*x2z3*24.+c(4,
     & 2,4,n)*x2z4*24.+c(4,2,5,n)*x2z5*24.+c(4,3,0,n)*x2y1*72.+c(4,3,
     & 1,n)*x2y1z1*72.+c(4,3,2,n)*x2y1z2*72.+c(4,3,3,n)*x2y1z3*72.+c(
     & 4,3,4,n)*x2y1z4*72.+c(4,3,5,n)*x2y1z5*72.+c(4,4,0,n)*x2y2*144.+
     & c(4,4,1,n)*x2y2z1*144.+c(4,4,2,n)*x2y2z2*144.+c(4,4,3,n)*
     & x2y2z3*144.+c(4,4,4,n)*x2y2z4*144.+c(4,4,5,n)*x2y2z5*144.+c(4,
     & 5,0,n)*x2y3*240.+c(4,5,1,n)*x2y3z1*240.+c(4,5,2,n)*x2y3z2*240.+
     & c(4,5,3,n)*x2y3z3*240.+c(4,5,4,n)*x2y3z4*240.+c(4,5,5,n)*
     & x2y3z5*240.+c(5,2,0,n)*x3*40.+c(5,2,1,n)*x3z1*40.+c(5,2,2,n)*
     & x3z2*40.+c(5,2,3,n)*x3z3*40.+c(5,2,4,n)*x3z4*40.+c(5,2,5,n)*
     & x3z5*40.+c(5,3,0,n)*x3y1*120.+c(5,3,1,n)*x3y1z1*120.+c(5,3,2,n)
     & *x3y1z2*120.+c(5,3,3,n)*x3y1z3*120.+c(5,3,4,n)*x3y1z4*120.+c(5,
     & 3,5,n)*x3y1z5*120.+c(5,4,0,n)*x3y2*240.+c(5,4,1,n)*x3y2z1*240.+
     & c(5,4,2,n)*x3y2z2*240.+c(5,4,3,n)*x3y2z3*240.+c(5,4,4,n)*
     & x3y2z4*240.+c(5,4,5,n)*x3y2z5*240.+c(5,5,0,n)*x3y3*400.+c(5,5,
     & 1,n)*x3y3z1*400.+c(5,5,2,n)*x3y3z2*400.+c(5,5,3,n)*x3y3z3*400.+
     & c(5,5,4,n)*x3y3z4*400.+c(5,5,5,n)*x3y3z5*400.)*time
! endLoops()
      else if( dx.eq.2.and.dy.eq.2.and.dz.eq.1.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
c 
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
c 
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
      r=(c(2,2,1,n)*4.+c(2,2,2,n)*z1*8.+c(2,2,3,n)*z2*12.+c(2,2,4,n)*
     & z3*16.+c(2,2,5,n)*z4*20.+c(2,3,1,n)*y1*12.+c(2,3,2,n)*y1z1*24.+
     & c(2,3,3,n)*y1z2*36.+c(2,3,4,n)*y1z3*48.+c(2,3,5,n)*y1z4*60.+c(
     & 2,4,1,n)*y2*24.+c(2,4,2,n)*y2z1*48.+c(2,4,3,n)*y2z2*72.+c(2,4,
     & 4,n)*y2z3*96.+c(2,4,5,n)*y2z4*120.+c(2,5,1,n)*y3*40.+c(2,5,2,n)
     & *y3z1*80.+c(2,5,3,n)*y3z2*120.+c(2,5,4,n)*y3z3*160.+c(2,5,5,n)*
     & y3z4*200.+c(3,2,1,n)*x1*12.+c(3,2,2,n)*x1z1*24.+c(3,2,3,n)*
     & x1z2*36.+c(3,2,4,n)*x1z3*48.+c(3,2,5,n)*x1z4*60.+c(3,3,1,n)*
     & x1y1*36.+c(3,3,2,n)*x1y1z1*72.+c(3,3,3,n)*x1y1z2*108.+c(3,3,4,
     & n)*x1y1z3*144.+c(3,3,5,n)*x1y1z4*180.+c(3,4,1,n)*x1y2*72.+c(3,
     & 4,2,n)*x1y2z1*144.+c(3,4,3,n)*x1y2z2*216.+c(3,4,4,n)*x1y2z3*
     & 288.+c(3,4,5,n)*x1y2z4*360.+c(3,5,1,n)*x1y3*120.+c(3,5,2,n)*
     & x1y3z1*240.+c(3,5,3,n)*x1y3z2*360.+c(3,5,4,n)*x1y3z3*480.+c(3,
     & 5,5,n)*x1y3z4*600.+c(4,2,1,n)*x2*24.+c(4,2,2,n)*x2z1*48.+c(4,2,
     & 3,n)*x2z2*72.+c(4,2,4,n)*x2z3*96.+c(4,2,5,n)*x2z4*120.+c(4,3,1,
     & n)*x2y1*72.+c(4,3,2,n)*x2y1z1*144.+c(4,3,3,n)*x2y1z2*216.+c(4,
     & 3,4,n)*x2y1z3*288.+c(4,3,5,n)*x2y1z4*360.+c(4,4,1,n)*x2y2*144.+
     & c(4,4,2,n)*x2y2z1*288.+c(4,4,3,n)*x2y2z2*432.+c(4,4,4,n)*
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
      else if( dx.eq.2.and.dy.eq.2.and.dz.eq.2.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
c 
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
c 
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
      r=(c(2,2,2,n)*8.+c(2,2,3,n)*z1*24.+c(2,2,4,n)*z2*48.+c(2,2,5,n)*
     & z3*80.+c(2,3,2,n)*y1*24.+c(2,3,3,n)*y1z1*72.+c(2,3,4,n)*y1z2*
     & 144.+c(2,3,5,n)*y1z3*240.+c(2,4,2,n)*y2*48.+c(2,4,3,n)*y2z1*
     & 144.+c(2,4,4,n)*y2z2*288.+c(2,4,5,n)*y2z3*480.+c(2,5,2,n)*y3*
     & 80.+c(2,5,3,n)*y3z1*240.+c(2,5,4,n)*y3z2*480.+c(2,5,5,n)*y3z3*
     & 800.+c(3,2,2,n)*x1*24.+c(3,2,3,n)*x1z1*72.+c(3,2,4,n)*x1z2*
     & 144.+c(3,2,5,n)*x1z3*240.+c(3,3,2,n)*x1y1*72.+c(3,3,3,n)*
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
c 
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
c 
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
      r=(c(3,0,0,n)*6.+c(3,0,1,n)*z1*6.+c(3,0,2,n)*z2*6.+c(3,0,3,n)*z3*
     & 6.+c(3,0,4,n)*z4*6.+c(3,0,5,n)*z5*6.+c(3,1,0,n)*y1*6.+c(3,1,1,
     & n)*y1z1*6.+c(3,1,2,n)*y1z2*6.+c(3,1,3,n)*y1z3*6.+c(3,1,4,n)*
     & y1z4*6.+c(3,1,5,n)*y1z5*6.+c(3,2,0,n)*y2*6.+c(3,2,1,n)*y2z1*6.+
     & c(3,2,2,n)*y2z2*6.+c(3,2,3,n)*y2z3*6.+c(3,2,4,n)*y2z4*6.+c(3,2,
     & 5,n)*y2z5*6.+c(3,3,0,n)*y3*6.+c(3,3,1,n)*y3z1*6.+c(3,3,2,n)*
     & y3z2*6.+c(3,3,3,n)*y3z3*6.+c(3,3,4,n)*y3z4*6.+c(3,3,5,n)*y3z5*
     & 6.+c(3,4,0,n)*y4*6.+c(3,4,1,n)*y4z1*6.+c(3,4,2,n)*y4z2*6.+c(3,
     & 4,3,n)*y4z3*6.+c(3,4,4,n)*y4z4*6.+c(3,4,5,n)*y4z5*6.+c(3,5,0,n)
     & *y5*6.+c(3,5,1,n)*y5z1*6.+c(3,5,2,n)*y5z2*6.+c(3,5,3,n)*y5z3*
     & 6.+c(3,5,4,n)*y5z4*6.+c(3,5,5,n)*y5z5*6.+c(4,0,0,n)*x1*24.+c(4,
     & 0,1,n)*x1z1*24.+c(4,0,2,n)*x1z2*24.+c(4,0,3,n)*x1z3*24.+c(4,0,
     & 4,n)*x1z4*24.+c(4,0,5,n)*x1z5*24.+c(4,1,0,n)*x1y1*24.+c(4,1,1,
     & n)*x1y1z1*24.+c(4,1,2,n)*x1y1z2*24.+c(4,1,3,n)*x1y1z3*24.+c(4,
     & 1,4,n)*x1y1z4*24.+c(4,1,5,n)*x1y1z5*24.+c(4,2,0,n)*x1y2*24.+c(
     & 4,2,1,n)*x1y2z1*24.+c(4,2,2,n)*x1y2z2*24.+c(4,2,3,n)*x1y2z3*
     & 24.+c(4,2,4,n)*x1y2z4*24.+c(4,2,5,n)*x1y2z5*24.+c(4,3,0,n)*
     & x1y3*24.+c(4,3,1,n)*x1y3z1*24.+c(4,3,2,n)*x1y3z2*24.+c(4,3,3,n)
     & *x1y3z3*24.+c(4,3,4,n)*x1y3z4*24.+c(4,3,5,n)*x1y3z5*24.+c(4,4,
     & 0,n)*x1y4*24.+c(4,4,1,n)*x1y4z1*24.+c(4,4,2,n)*x1y4z2*24.+c(4,
     & 4,3,n)*x1y4z3*24.+c(4,4,4,n)*x1y4z4*24.+c(4,4,5,n)*x1y4z5*24.+
     & c(4,5,0,n)*x1y5*24.+c(4,5,1,n)*x1y5z1*24.+c(4,5,2,n)*x1y5z2*
     & 24.+c(4,5,3,n)*x1y5z3*24.+c(4,5,4,n)*x1y5z4*24.+c(4,5,5,n)*
     & x1y5z5*24.+c(5,0,0,n)*x2*60.+c(5,0,1,n)*x2z1*60.+c(5,0,2,n)*
     & x2z2*60.+c(5,0,3,n)*x2z3*60.+c(5,0,4,n)*x2z4*60.+c(5,0,5,n)*
     & x2z5*60.+c(5,1,0,n)*x2y1*60.+c(5,1,1,n)*x2y1z1*60.+c(5,1,2,n)*
     & x2y1z2*60.+c(5,1,3,n)*x2y1z3*60.+c(5,1,4,n)*x2y1z4*60.+c(5,1,5,
     & n)*x2y1z5*60.+c(5,2,0,n)*x2y2*60.+c(5,2,1,n)*x2y2z1*60.+c(5,2,
     & 2,n)*x2y2z2*60.+c(5,2,3,n)*x2y2z3*60.+c(5,2,4,n)*x2y2z4*60.+c(
     & 5,2,5,n)*x2y2z5*60.+c(5,3,0,n)*x2y3*60.+c(5,3,1,n)*x2y3z1*60.+
     & c(5,3,2,n)*x2y3z2*60.+c(5,3,3,n)*x2y3z3*60.+c(5,3,4,n)*x2y3z4*
     & 60.+c(5,3,5,n)*x2y3z5*60.+c(5,4,0,n)*x2y4*60.+c(5,4,1,n)*
     & x2y4z1*60.+c(5,4,2,n)*x2y4z2*60.+c(5,4,3,n)*x2y4z3*60.+c(5,4,4,
     & n)*x2y4z4*60.+c(5,4,5,n)*x2y4z5*60.+c(5,5,0,n)*x2y5*60.+c(5,5,
     & 1,n)*x2y5z1*60.+c(5,5,2,n)*x2y5z2*60.+c(5,5,3,n)*x2y5z3*60.+c(
     & 5,5,4,n)*x2y5z4*60.+c(5,5,5,n)*x2y5z5*60.)*time
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
c 
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
c 
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
      r=(c(4,0,0,n)*24.+c(4,0,1,n)*z1*24.+c(4,0,2,n)*z2*24.+c(4,0,3,n)*
     & z3*24.+c(4,0,4,n)*z4*24.+c(4,0,5,n)*z5*24.+c(4,1,0,n)*y1*24.+c(
     & 4,1,1,n)*y1z1*24.+c(4,1,2,n)*y1z2*24.+c(4,1,3,n)*y1z3*24.+c(4,
     & 1,4,n)*y1z4*24.+c(4,1,5,n)*y1z5*24.+c(4,2,0,n)*y2*24.+c(4,2,1,
     & n)*y2z1*24.+c(4,2,2,n)*y2z2*24.+c(4,2,3,n)*y2z3*24.+c(4,2,4,n)*
     & y2z4*24.+c(4,2,5,n)*y2z5*24.+c(4,3,0,n)*y3*24.+c(4,3,1,n)*y3z1*
     & 24.+c(4,3,2,n)*y3z2*24.+c(4,3,3,n)*y3z3*24.+c(4,3,4,n)*y3z4*
     & 24.+c(4,3,5,n)*y3z5*24.+c(4,4,0,n)*y4*24.+c(4,4,1,n)*y4z1*24.+
     & c(4,4,2,n)*y4z2*24.+c(4,4,3,n)*y4z3*24.+c(4,4,4,n)*y4z4*24.+c(
     & 4,4,5,n)*y4z5*24.+c(4,5,0,n)*y5*24.+c(4,5,1,n)*y5z1*24.+c(4,5,
     & 2,n)*y5z2*24.+c(4,5,3,n)*y5z3*24.+c(4,5,4,n)*y5z4*24.+c(4,5,5,
     & n)*y5z5*24.+c(5,0,0,n)*x1*120.+c(5,0,1,n)*x1z1*120.+c(5,0,2,n)*
     & x1z2*120.+c(5,0,3,n)*x1z3*120.+c(5,0,4,n)*x1z4*120.+c(5,0,5,n)*
     & x1z5*120.+c(5,1,0,n)*x1y1*120.+c(5,1,1,n)*x1y1z1*120.+c(5,1,2,
     & n)*x1y1z2*120.+c(5,1,3,n)*x1y1z3*120.+c(5,1,4,n)*x1y1z4*120.+c(
     & 5,1,5,n)*x1y1z5*120.+c(5,2,0,n)*x1y2*120.+c(5,2,1,n)*x1y2z1*
     & 120.+c(5,2,2,n)*x1y2z2*120.+c(5,2,3,n)*x1y2z3*120.+c(5,2,4,n)*
     & x1y2z4*120.+c(5,2,5,n)*x1y2z5*120.+c(5,3,0,n)*x1y3*120.+c(5,3,
     & 1,n)*x1y3z1*120.+c(5,3,2,n)*x1y3z2*120.+c(5,3,3,n)*x1y3z3*120.+
     & c(5,3,4,n)*x1y3z4*120.+c(5,3,5,n)*x1y3z5*120.+c(5,4,0,n)*x1y4*
     & 120.+c(5,4,1,n)*x1y4z1*120.+c(5,4,2,n)*x1y4z2*120.+c(5,4,3,n)*
     & x1y4z3*120.+c(5,4,4,n)*x1y4z4*120.+c(5,4,5,n)*x1y4z5*120.+c(5,
     & 5,0,n)*x1y5*120.+c(5,5,1,n)*x1y5z1*120.+c(5,5,2,n)*x1y5z2*120.+
     & c(5,5,3,n)*x1y5z3*120.+c(5,5,4,n)*x1y5z4*120.+c(5,5,5,n)*
     & x1y5z5*120.)*time
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
      z2=z1*z1
      z3=z2*z1
c
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
c
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
      r=(+c(0,0,2,n)*2.+c(0,0,3,n)*z1*6.+c(0,0,4,n)*z2*12.+c(0,0,5,n)*
     & z3*20.+c(0,1,2,n)*y1*2.+c(0,1,3,n)*y1z1*6.+c(0,1,4,n)*y1z2*12.+
     & c(0,1,5,n)*y1z3*20.+c(0,2,2,n)*y2*2.+c(0,2,3,n)*y2z1*6.+c(0,2,
     & 4,n)*y2z2*12.+c(0,2,5,n)*y2z3*20.+c(0,3,2,n)*y3*2.+c(0,3,3,n)*
     & y3z1*6.+c(0,3,4,n)*y3z2*12.+c(0,3,5,n)*y3z3*20.+c(0,4,2,n)*y4*
     & 2.+c(0,4,3,n)*y4z1*6.+c(0,4,4,n)*y4z2*12.+c(0,4,5,n)*y4z3*20.+
     & c(0,5,2,n)*y5*2.+c(0,5,3,n)*y5z1*6.+c(0,5,4,n)*y5z2*12.+c(0,5,
     & 5,n)*y5z3*20.+c(1,0,2,n)*x1*2.+c(1,0,3,n)*x1z1*6.+c(1,0,4,n)*
     & x1z2*12.+c(1,0,5,n)*x1z3*20.+c(1,1,2,n)*x1y1*2.+c(1,1,3,n)*
     & x1y1z1*6.+c(1,1,4,n)*x1y1z2*12.+c(1,1,5,n)*x1y1z3*20.+c(1,2,2,
     & n)*x1y2*2.+c(1,2,3,n)*x1y2z1*6.+c(1,2,4,n)*x1y2z2*12.+c(1,2,5,
     & n)*x1y2z3*20.+c(1,3,2,n)*x1y3*2.+c(1,3,3,n)*x1y3z1*6.+c(1,3,4,
     & n)*x1y3z2*12.+c(1,3,5,n)*x1y3z3*20.+c(1,4,2,n)*x1y4*2.+c(1,4,3,
     & n)*x1y4z1*6.+c(1,4,4,n)*x1y4z2*12.+c(1,4,5,n)*x1y4z3*20.+c(1,5,
     & 2,n)*x1y5*2.+c(1,5,3,n)*x1y5z1*6.+c(1,5,4,n)*x1y5z2*12.+c(1,5,
     & 5,n)*x1y5z3*20.+c(2,0,2,n)*x2*2.+c(2,0,3,n)*x2z1*6.+c(2,0,4,n)*
     & x2z2*12.+c(2,0,5,n)*x2z3*20.+c(2,1,2,n)*x2y1*2.+c(2,1,3,n)*
     & x2y1z1*6.+c(2,1,4,n)*x2y1z2*12.+c(2,1,5,n)*x2y1z3*20.+c(2,2,2,
     & n)*x2y2*2.+c(2,2,3,n)*x2y2z1*6.+c(2,2,4,n)*x2y2z2*12.+c(2,2,5,
     & n)*x2y2z3*20.+c(2,3,2,n)*x2y3*2.+c(2,3,3,n)*x2y3z1*6.+c(2,3,4,
     & n)*x2y3z2*12.+c(2,3,5,n)*x2y3z3*20.+c(2,4,2,n)*x2y4*2.+c(2,4,3,
     & n)*x2y4z1*6.+c(2,4,4,n)*x2y4z2*12.+c(2,4,5,n)*x2y4z3*20.+c(2,5,
     & 2,n)*x2y5*2.+c(2,5,3,n)*x2y5z1*6.+c(2,5,4,n)*x2y5z2*12.+c(2,5,
     & 5,n)*x2y5z3*20.+c(3,0,2,n)*x3*2.+c(3,0,3,n)*x3z1*6.+c(3,0,4,n)*
     & x3z2*12.+c(3,0,5,n)*x3z3*20.+c(3,1,2,n)*x3y1*2.+c(3,1,3,n)*
     & x3y1z1*6.+c(3,1,4,n)*x3y1z2*12.+c(3,1,5,n)*x3y1z3*20.+c(3,2,2,
     & n)*x3y2*2.+c(3,2,3,n)*x3y2z1*6.+c(3,2,4,n)*x3y2z2*12.+c(3,2,5,
     & n)*x3y2z3*20.+c(3,3,2,n)*x3y3*2.+c(3,3,3,n)*x3y3z1*6.+c(3,3,4,
     & n)*x3y3z2*12.+c(3,3,5,n)*x3y3z3*20.+c(3,4,2,n)*x3y4*2.+c(3,4,3,
     & n)*x3y4z1*6.+c(3,4,4,n)*x3y4z2*12.+c(3,4,5,n)*x3y4z3*20.+c(3,5,
     & 2,n)*x3y5*2.+c(3,5,3,n)*x3y5z1*6.+c(3,5,4,n)*x3y5z2*12.+c(3,5,
     & 5,n)*x3y5z3*20.+c(4,0,2,n)*x4*2.+c(4,0,3,n)*x4z1*6.+c(4,0,4,n)*
     & x4z2*12.+c(4,0,5,n)*x4z3*20.+c(4,1,2,n)*x4y1*2.+c(4,1,3,n)*
     & x4y1z1*6.+c(4,1,4,n)*x4y1z2*12.+c(4,1,5,n)*x4y1z3*20.+c(4,2,2,
     & n)*x4y2*2.+c(4,2,3,n)*x4y2z1*6.+c(4,2,4,n)*x4y2z2*12.+c(4,2,5,
     & n)*x4y2z3*20.+c(4,3,2,n)*x4y3*2.+c(4,3,3,n)*x4y3z1*6.+c(4,3,4,
     & n)*x4y3z2*12.+c(4,3,5,n)*x4y3z3*20.+c(4,4,2,n)*x4y4*2.+c(4,4,3,
     & n)*x4y4z1*6.+c(4,4,4,n)*x4y4z2*12.+c(4,4,5,n)*x4y4z3*20.+c(4,5,
     & 2,n)*x4y5*2.+c(4,5,3,n)*x4y5z1*6.+c(4,5,4,n)*x4y5z2*12.+c(4,5,
     & 5,n)*x4y5z3*20.+c(5,0,2,n)*x5*2.+c(5,0,3,n)*x5z1*6.+c(5,0,4,n)*
     & x5z2*12.+c(5,0,5,n)*x5z3*20.+c(5,1,2,n)*x5y1*2.+c(5,1,3,n)*
     & x5y1z1*6.+c(5,1,4,n)*x5y1z2*12.+c(5,1,5,n)*x5y1z3*20.+c(5,2,2,
     & n)*x5y2*2.+c(5,2,3,n)*x5y2z1*6.+c(5,2,4,n)*x5y2z2*12.+c(5,2,5,
     & n)*x5y2z3*20.+c(5,3,2,n)*x5y3*2.+c(5,3,3,n)*x5y3z1*6.+c(5,3,4,
     & n)*x5y3z2*12.+c(5,3,5,n)*x5y3z3*20.+c(5,4,2,n)*x5y4*2.+c(5,4,3,
     & n)*x5y4z1*6.+c(5,4,4,n)*x5y4z2*12.+c(5,4,5,n)*x5y4z3*20.+c(5,5,
     & 2,n)*x5y5*2.+c(5,5,3,n)*x5y5z1*6.+c(5,5,4,n)*x5y5z2*12.+c(5,5,
     & 5,n)*x5y5z3*20. )
       r=(r+c(0,2,0,n)*2.+c(0,2,1,n)*z1*2.+c(0,2,2,n)*z2*2.+c(0,2,3,n)*
     & z3*2.+c(0,2,4,n)*z4*2.+c(0,2,5,n)*z5*2.+c(0,3,0,n)*y1*6.+c(0,3,
     & 1,n)*y1z1*6.+c(0,3,2,n)*y1z2*6.+c(0,3,3,n)*y1z3*6.+c(0,3,4,n)*
     & y1z4*6.+c(0,3,5,n)*y1z5*6.+c(0,4,0,n)*y2*12.+c(0,4,1,n)*y2z1*
     & 12.+c(0,4,2,n)*y2z2*12.+c(0,4,3,n)*y2z3*12.+c(0,4,4,n)*y2z4*
     & 12.+c(0,4,5,n)*y2z5*12.+c(0,5,0,n)*y3*20.+c(0,5,1,n)*y3z1*20.+
     & c(0,5,2,n)*y3z2*20.+c(0,5,3,n)*y3z3*20.+c(0,5,4,n)*y3z4*20.+c(
     & 0,5,5,n)*y3z5*20.+c(1,2,0,n)*x1*2.+c(1,2,1,n)*x1z1*2.+c(1,2,2,
     & n)*x1z2*2.+c(1,2,3,n)*x1z3*2.+c(1,2,4,n)*x1z4*2.+c(1,2,5,n)*
     & x1z5*2.+c(1,3,0,n)*x1y1*6.+c(1,3,1,n)*x1y1z1*6.+c(1,3,2,n)*
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
     & 5,5,5,n)*x5y3z5*20. )
       r=(r+c(2,0,0,n)*2.+c(2,0,1,n)*z1*2.+c(2,0,2,n)*z2*2.+c(2,0,3,n)*
     & z3*2.+c(2,0,4,n)*z4*2.+c(2,0,5,n)*z5*2.+c(2,1,0,n)*y1*2.+c(2,1,
     & 1,n)*y1z1*2.+c(2,1,2,n)*y1z2*2.+c(2,1,3,n)*y1z3*2.+c(2,1,4,n)*
     & y1z4*2.+c(2,1,5,n)*y1z5*2.+c(2,2,0,n)*y2*2.+c(2,2,1,n)*y2z1*2.+
     & c(2,2,2,n)*y2z2*2.+c(2,2,3,n)*y2z3*2.+c(2,2,4,n)*y2z4*2.+c(2,2,
     & 5,n)*y2z5*2.+c(2,3,0,n)*y3*2.+c(2,3,1,n)*y3z1*2.+c(2,3,2,n)*
     & y3z2*2.+c(2,3,3,n)*y3z3*2.+c(2,3,4,n)*y3z4*2.+c(2,3,5,n)*y3z5*
     & 2.+c(2,4,0,n)*y4*2.+c(2,4,1,n)*y4z1*2.+c(2,4,2,n)*y4z2*2.+c(2,
     & 4,3,n)*y4z3*2.+c(2,4,4,n)*y4z4*2.+c(2,4,5,n)*y4z5*2.+c(2,5,0,n)
     & *y5*2.+c(2,5,1,n)*y5z1*2.+c(2,5,2,n)*y5z2*2.+c(2,5,3,n)*y5z3*
     & 2.+c(2,5,4,n)*y5z4*2.+c(2,5,5,n)*y5z5*2.+c(3,0,0,n)*x1*6.+c(3,
     & 0,1,n)*x1z1*6.+c(3,0,2,n)*x1z2*6.+c(3,0,3,n)*x1z3*6.+c(3,0,4,n)
     & *x1z4*6.+c(3,0,5,n)*x1z5*6.+c(3,1,0,n)*x1y1*6.+c(3,1,1,n)*
     & x1y1z1*6.+c(3,1,2,n)*x1y1z2*6.+c(3,1,3,n)*x1y1z3*6.+c(3,1,4,n)*
     & x1y1z4*6.+c(3,1,5,n)*x1y1z5*6.+c(3,2,0,n)*x1y2*6.+c(3,2,1,n)*
     & x1y2z1*6.+c(3,2,2,n)*x1y2z2*6.+c(3,2,3,n)*x1y2z3*6.+c(3,2,4,n)*
     & x1y2z4*6.+c(3,2,5,n)*x1y2z5*6.+c(3,3,0,n)*x1y3*6.+c(3,3,1,n)*
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
      else
! beginLoops(time=0.)
      time=0.
      r=0.
! endLoops()
      end if
! #If "3D5" eq "3D5" || "3D5" eq "3D6"
#else
      write(*,'(''ERROR: poly3D5 not available for ifort'')')
#endif
      return
      end
! polyFun(3D6)
      subroutine polyEvaluate3D6 (r, x1,y1,z1,n,t, ndc1,ndc2,ndc3,nda, 
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
! #If "3D6" eq "3D5" || "3D6" eq "3D6"
#ifndef IFORT_COMPILER
! poly3D6(c,c,c,r)
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
      z6=z5*z1
c 
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y1z5=y1z4*z1
      y1z6=y1z5*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
      y2z5=y1z5*y1
      y2z6=y1z6*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      y3z4=y2z4*y1
      y3z5=y2z5*y1
      y3z6=y2z6*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y4z2=y3z2*y1
      y4z3=y3z3*y1
      y4z4=y3z4*y1
      y4z5=y3z5*y1
      y4z6=y3z6*y1
      y5=y4*y1
      y5z1=y4z1*y1
      y5z2=y4z2*y1
      y5z3=y4z3*y1
      y5z4=y4z4*y1
      y5z5=y4z5*y1
      y5z6=y4z6*y1
      y6=y5*y1
      y6z1=y5z1*y1
      y6z2=y5z2*y1
      y6z3=y5z3*y1
      y6z4=y5z4*y1
      y6z5=y5z5*y1
      y6z6=y5z6*y1
c 
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1z5=x1z4*z1
      x1z6=x1z5*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y1z5=x1y1z4*z1
      x1y1z6=x1y1z5*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y2z4=x1y1z4*y1
      x1y2z5=x1y1z5*y1
      x1y2z6=x1y1z6*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x1y3z4=x1y2z4*y1
      x1y3z5=x1y2z5*y1
      x1y3z6=x1y2z6*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y4z2=x1y3z2*y1
      x1y4z3=x1y3z3*y1
      x1y4z4=x1y3z4*y1
      x1y4z5=x1y3z5*y1
      x1y4z6=x1y3z6*y1
      x1y5=x1y4*y1
      x1y5z1=x1y4z1*y1
      x1y5z2=x1y4z2*y1
      x1y5z3=x1y4z3*y1
      x1y5z4=x1y4z4*y1
      x1y5z5=x1y4z5*y1
      x1y5z6=x1y4z6*y1
      x1y6=x1y5*y1
      x1y6z1=x1y5z1*y1
      x1y6z2=x1y5z2*y1
      x1y6z3=x1y5z3*y1
      x1y6z4=x1y5z4*y1
      x1y6z5=x1y5z5*y1
      x1y6z6=x1y5z6*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x2z5=x1z5*x1
      x2z6=x1z6*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y1z4=x1y1z4*x1
      x2y1z5=x1y1z5*x1
      x2y1z6=x1y1z6*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y2z4=x1y2z4*x1
      x2y2z5=x1y2z5*x1
      x2y2z6=x1y2z6*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y3z3=x1y3z3*x1
      x2y3z4=x1y3z4*x1
      x2y3z5=x1y3z5*x1
      x2y3z6=x1y3z6*x1
      x2y4=x1y4*x1
      x2y4z1=x1y4z1*x1
      x2y4z2=x1y4z2*x1
      x2y4z3=x1y4z3*x1
      x2y4z4=x1y4z4*x1
      x2y4z5=x1y4z5*x1
      x2y4z6=x1y4z6*x1
      x2y5=x1y5*x1
      x2y5z1=x1y5z1*x1
      x2y5z2=x1y5z2*x1
      x2y5z3=x1y5z3*x1
      x2y5z4=x1y5z4*x1
      x2y5z5=x1y5z5*x1
      x2y5z6=x1y5z6*x1
      x2y6=x1y6*x1
      x2y6z1=x1y6z1*x1
      x2y6z2=x1y6z2*x1
      x2y6z3=x1y6z3*x1
      x2y6z4=x1y6z4*x1
      x2y6z5=x1y6z5*x1
      x2y6z6=x1y6z6*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3z4=x2z4*x1
      x3z5=x2z5*x1
      x3z6=x2z6*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x3y1z4=x2y1z4*x1
      x3y1z5=x2y1z5*x1
      x3y1z6=x2y1z6*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y2z3=x2y2z3*x1
      x3y2z4=x2y2z4*x1
      x3y2z5=x2y2z5*x1
      x3y2z6=x2y2z6*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x3y3z2=x2y3z2*x1
      x3y3z3=x2y3z3*x1
      x3y3z4=x2y3z4*x1
      x3y3z5=x2y3z5*x1
      x3y3z6=x2y3z6*x1
      x3y4=x2y4*x1
      x3y4z1=x2y4z1*x1
      x3y4z2=x2y4z2*x1
      x3y4z3=x2y4z3*x1
      x3y4z4=x2y4z4*x1
      x3y4z5=x2y4z5*x1
      x3y4z6=x2y4z6*x1
      x3y5=x2y5*x1
      x3y5z1=x2y5z1*x1
      x3y5z2=x2y5z2*x1
      x3y5z3=x2y5z3*x1
      x3y5z4=x2y5z4*x1
      x3y5z5=x2y5z5*x1
      x3y5z6=x2y5z6*x1
      x3y6=x2y6*x1
      x3y6z1=x2y6z1*x1
      x3y6z2=x2y6z2*x1
      x3y6z3=x2y6z3*x1
      x3y6z4=x2y6z4*x1
      x3y6z5=x2y6z5*x1
      x3y6z6=x2y6z6*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4z2=x3z2*x1
      x4z3=x3z3*x1
      x4z4=x3z4*x1
      x4z5=x3z5*x1
      x4z6=x3z6*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y1z2=x3y1z2*x1
      x4y1z3=x3y1z3*x1
      x4y1z4=x3y1z4*x1
      x4y1z5=x3y1z5*x1
      x4y1z6=x3y1z6*x1
      x4y2=x3y2*x1
      x4y2z1=x3y2z1*x1
      x4y2z2=x3y2z2*x1
      x4y2z3=x3y2z3*x1
      x4y2z4=x3y2z4*x1
      x4y2z5=x3y2z5*x1
      x4y2z6=x3y2z6*x1
      x4y3=x3y3*x1
      x4y3z1=x3y3z1*x1
      x4y3z2=x3y3z2*x1
      x4y3z3=x3y3z3*x1
      x4y3z4=x3y3z4*x1
      x4y3z5=x3y3z5*x1
      x4y3z6=x3y3z6*x1
      x4y4=x3y4*x1
      x4y4z1=x3y4z1*x1
      x4y4z2=x3y4z2*x1
      x4y4z3=x3y4z3*x1
      x4y4z4=x3y4z4*x1
      x4y4z5=x3y4z5*x1
      x4y4z6=x3y4z6*x1
      x4y5=x3y5*x1
      x4y5z1=x3y5z1*x1
      x4y5z2=x3y5z2*x1
      x4y5z3=x3y5z3*x1
      x4y5z4=x3y5z4*x1
      x4y5z5=x3y5z5*x1
      x4y5z6=x3y5z6*x1
      x4y6=x3y6*x1
      x4y6z1=x3y6z1*x1
      x4y6z2=x3y6z2*x1
      x4y6z3=x3y6z3*x1
      x4y6z4=x3y6z4*x1
      x4y6z5=x3y6z5*x1
      x4y6z6=x3y6z6*x1
      x5=x4*x1
      x5z1=x4z1*x1
      x5z2=x4z2*x1
      x5z3=x4z3*x1
      x5z4=x4z4*x1
      x5z5=x4z5*x1
      x5z6=x4z6*x1
      x5y1=x4y1*x1
      x5y1z1=x4y1z1*x1
      x5y1z2=x4y1z2*x1
      x5y1z3=x4y1z3*x1
      x5y1z4=x4y1z4*x1
      x5y1z5=x4y1z5*x1
      x5y1z6=x4y1z6*x1
      x5y2=x4y2*x1
      x5y2z1=x4y2z1*x1
      x5y2z2=x4y2z2*x1
      x5y2z3=x4y2z3*x1
      x5y2z4=x4y2z4*x1
      x5y2z5=x4y2z5*x1
      x5y2z6=x4y2z6*x1
      x5y3=x4y3*x1
      x5y3z1=x4y3z1*x1
      x5y3z2=x4y3z2*x1
      x5y3z3=x4y3z3*x1
      x5y3z4=x4y3z4*x1
      x5y3z5=x4y3z5*x1
      x5y3z6=x4y3z6*x1
      x5y4=x4y4*x1
      x5y4z1=x4y4z1*x1
      x5y4z2=x4y4z2*x1
      x5y4z3=x4y4z3*x1
      x5y4z4=x4y4z4*x1
      x5y4z5=x4y4z5*x1
      x5y4z6=x4y4z6*x1
      x5y5=x4y5*x1
      x5y5z1=x4y5z1*x1
      x5y5z2=x4y5z2*x1
      x5y5z3=x4y5z3*x1
      x5y5z4=x4y5z4*x1
      x5y5z5=x4y5z5*x1
      x5y5z6=x4y5z6*x1
      x5y6=x4y6*x1
      x5y6z1=x4y6z1*x1
      x5y6z2=x4y6z2*x1
      x5y6z3=x4y6z3*x1
      x5y6z4=x4y6z4*x1
      x5y6z5=x4y6z5*x1
      x5y6z6=x4y6z6*x1
      x6=x5*x1
      x6z1=x5z1*x1
      x6z2=x5z2*x1
      x6z3=x5z3*x1
      x6z4=x5z4*x1
      x6z5=x5z5*x1
      x6z6=x5z6*x1
      x6y1=x5y1*x1
      x6y1z1=x5y1z1*x1
      x6y1z2=x5y1z2*x1
      x6y1z3=x5y1z3*x1
      x6y1z4=x5y1z4*x1
      x6y1z5=x5y1z5*x1
      x6y1z6=x5y1z6*x1
      x6y2=x5y2*x1
      x6y2z1=x5y2z1*x1
      x6y2z2=x5y2z2*x1
      x6y2z3=x5y2z3*x1
      x6y2z4=x5y2z4*x1
      x6y2z5=x5y2z5*x1
      x6y2z6=x5y2z6*x1
      x6y3=x5y3*x1
      x6y3z1=x5y3z1*x1
      x6y3z2=x5y3z2*x1
      x6y3z3=x5y3z3*x1
      x6y3z4=x5y3z4*x1
      x6y3z5=x5y3z5*x1
      x6y3z6=x5y3z6*x1
      x6y4=x5y4*x1
      x6y4z1=x5y4z1*x1
      x6y4z2=x5y4z2*x1
      x6y4z3=x5y4z3*x1
      x6y4z4=x5y4z4*x1
      x6y4z5=x5y4z5*x1
      x6y4z6=x5y4z6*x1
      x6y5=x5y5*x1
      x6y5z1=x5y5z1*x1
      x6y5z2=x5y5z2*x1
      x6y5z3=x5y5z3*x1
      x6y5z4=x5y5z4*x1
      x6y5z5=x5y5z5*x1
      x6y5z6=x5y5z6*x1
      x6y6=x5y6*x1
      x6y6z1=x5y6z1*x1
      x6y6z2=x5y6z2*x1
      x6y6z3=x5y6z3*x1
      x6y6z4=x5y6z4*x1
      x6y6z5=x5y6z5*x1
      x6y6z6=x5y6z6*x1
      r=(c(0,0,0,n)+c(0,0,1,n)*z1+c(0,0,2,n)*z2+c(0,0,3,n)*z3+c(0,0,4,
     & n)*z4+c(0,0,5,n)*z5+c(0,0,6,n)*z6+c(0,1,0,n)*y1+c(0,1,1,n)*
     & y1z1+c(0,1,2,n)*y1z2+c(0,1,3,n)*y1z3+c(0,1,4,n)*y1z4+c(0,1,5,n)
     & *y1z5+c(0,1,6,n)*y1z6+c(0,2,0,n)*y2+c(0,2,1,n)*y2z1+c(0,2,2,n)*
     & y2z2+c(0,2,3,n)*y2z3+c(0,2,4,n)*y2z4+c(0,2,5,n)*y2z5+c(0,2,6,n)
     & *y2z6+c(0,3,0,n)*y3+c(0,3,1,n)*y3z1+c(0,3,2,n)*y3z2+c(0,3,3,n)*
     & y3z3+c(0,3,4,n)*y3z4+c(0,3,5,n)*y3z5+c(0,3,6,n)*y3z6+c(0,4,0,n)
     & *y4+c(0,4,1,n)*y4z1+c(0,4,2,n)*y4z2+c(0,4,3,n)*y4z3+c(0,4,4,n)*
     & y4z4+c(0,4,5,n)*y4z5+c(0,4,6,n)*y4z6+c(0,5,0,n)*y5+c(0,5,1,n)*
     & y5z1+c(0,5,2,n)*y5z2+c(0,5,3,n)*y5z3+c(0,5,4,n)*y5z4+c(0,5,5,n)
     & *y5z5+c(0,5,6,n)*y5z6+c(0,6,0,n)*y6+c(0,6,1,n)*y6z1+c(0,6,2,n)*
     & y6z2+c(0,6,3,n)*y6z3+c(0,6,4,n)*y6z4+c(0,6,5,n)*y6z5+c(0,6,6,n)
     & *y6z6+c(1,0,0,n)*x1+c(1,0,1,n)*x1z1+c(1,0,2,n)*x1z2+c(1,0,3,n)*
     & x1z3+c(1,0,4,n)*x1z4+c(1,0,5,n)*x1z5+c(1,0,6,n)*x1z6+c(1,1,0,n)
     & *x1y1+c(1,1,1,n)*x1y1z1+c(1,1,2,n)*x1y1z2+c(1,1,3,n)*x1y1z3+c(
     & 1,1,4,n)*x1y1z4+c(1,1,5,n)*x1y1z5+c(1,1,6,n)*x1y1z6+c(1,2,0,n)*
     & x1y2+c(1,2,1,n)*x1y2z1+c(1,2,2,n)*x1y2z2+c(1,2,3,n)*x1y2z3+c(1,
     & 2,4,n)*x1y2z4+c(1,2,5,n)*x1y2z5+c(1,2,6,n)*x1y2z6+c(1,3,0,n)*
     & x1y3+c(1,3,1,n)*x1y3z1+c(1,3,2,n)*x1y3z2+c(1,3,3,n)*x1y3z3+c(1,
     & 3,4,n)*x1y3z4+c(1,3,5,n)*x1y3z5+c(1,3,6,n)*x1y3z6+c(1,4,0,n)*
     & x1y4+c(1,4,1,n)*x1y4z1+c(1,4,2,n)*x1y4z2+c(1,4,3,n)*x1y4z3+c(1,
     & 4,4,n)*x1y4z4+c(1,4,5,n)*x1y4z5+c(1,4,6,n)*x1y4z6+c(1,5,0,n)*
     & x1y5+c(1,5,1,n)*x1y5z1+c(1,5,2,n)*x1y5z2+c(1,5,3,n)*x1y5z3+c(1,
     & 5,4,n)*x1y5z4+c(1,5,5,n)*x1y5z5+c(1,5,6,n)*x1y5z6+c(1,6,0,n)*
     & x1y6+c(1,6,1,n)*x1y6z1+c(1,6,2,n)*x1y6z2+c(1,6,3,n)*x1y6z3+c(1,
     & 6,4,n)*x1y6z4+c(1,6,5,n)*x1y6z5+c(1,6,6,n)*x1y6z6+c(2,0,0,n)*
     & x2+c(2,0,1,n)*x2z1+c(2,0,2,n)*x2z2+c(2,0,3,n)*x2z3+c(2,0,4,n)*
     & x2z4+c(2,0,5,n)*x2z5+c(2,0,6,n)*x2z6+c(2,1,0,n)*x2y1+c(2,1,1,n)
     & *x2y1z1+c(2,1,2,n)*x2y1z2+c(2,1,3,n)*x2y1z3+c(2,1,4,n)*x2y1z4+
     & c(2,1,5,n)*x2y1z5+c(2,1,6,n)*x2y1z6+c(2,2,0,n)*x2y2+c(2,2,1,n)*
     & x2y2z1+c(2,2,2,n)*x2y2z2+c(2,2,3,n)*x2y2z3+c(2,2,4,n)*x2y2z4+c(
     & 2,2,5,n)*x2y2z5+c(2,2,6,n)*x2y2z6+c(2,3,0,n)*x2y3+c(2,3,1,n)*
     & x2y3z1+c(2,3,2,n)*x2y3z2+c(2,3,3,n)*x2y3z3+c(2,3,4,n)*x2y3z4+c(
     & 2,3,5,n)*x2y3z5+c(2,3,6,n)*x2y3z6+c(2,4,0,n)*x2y4+c(2,4,1,n)*
     & x2y4z1+c(2,4,2,n)*x2y4z2+c(2,4,3,n)*x2y4z3+c(2,4,4,n)*x2y4z4+c(
     & 2,4,5,n)*x2y4z5+c(2,4,6,n)*x2y4z6+c(2,5,0,n)*x2y5+c(2,5,1,n)*
     & x2y5z1+c(2,5,2,n)*x2y5z2+c(2,5,3,n)*x2y5z3+c(2,5,4,n)*x2y5z4+c(
     & 2,5,5,n)*x2y5z5+c(2,5,6,n)*x2y5z6+c(2,6,0,n)*x2y6+c(2,6,1,n)*
     & x2y6z1+c(2,6,2,n)*x2y6z2+c(2,6,3,n)*x2y6z3+c(2,6,4,n)*x2y6z4+c(
     & 2,6,5,n)*x2y6z5+c(2,6,6,n)*x2y6z6+c(3,0,0,n)*x3+c(3,0,1,n)*
     & x3z1+c(3,0,2,n)*x3z2+c(3,0,3,n)*x3z3+c(3,0,4,n)*x3z4+c(3,0,5,n)
     & *x3z5+c(3,0,6,n)*x3z6+c(3,1,0,n)*x3y1+c(3,1,1,n)*x3y1z1+c(3,1,
     & 2,n)*x3y1z2+c(3,1,3,n)*x3y1z3+c(3,1,4,n)*x3y1z4+c(3,1,5,n)*
     & x3y1z5+c(3,1,6,n)*x3y1z6+c(3,2,0,n)*x3y2+c(3,2,1,n)*x3y2z1+c(3,
     & 2,2,n)*x3y2z2+c(3,2,3,n)*x3y2z3+c(3,2,4,n)*x3y2z4+c(3,2,5,n)*
     & x3y2z5+c(3,2,6,n)*x3y2z6+c(3,3,0,n)*x3y3+c(3,3,1,n)*x3y3z1+c(3,
     & 3,2,n)*x3y3z2+c(3,3,3,n)*x3y3z3+c(3,3,4,n)*x3y3z4+c(3,3,5,n)*
     & x3y3z5+c(3,3,6,n)*x3y3z6+c(3,4,0,n)*x3y4+c(3,4,1,n)*x3y4z1+c(3,
     & 4,2,n)*x3y4z2+c(3,4,3,n)*x3y4z3+c(3,4,4,n)*x3y4z4+c(3,4,5,n)*
     & x3y4z5+c(3,4,6,n)*x3y4z6+c(3,5,0,n)*x3y5+c(3,5,1,n)*x3y5z1+c(3,
     & 5,2,n)*x3y5z2+c(3,5,3,n)*x3y5z3+c(3,5,4,n)*x3y5z4+c(3,5,5,n)*
     & x3y5z5+c(3,5,6,n)*x3y5z6+c(3,6,0,n)*x3y6+c(3,6,1,n)*x3y6z1+c(3,
     & 6,2,n)*x3y6z2+c(3,6,3,n)*x3y6z3+c(3,6,4,n)*x3y6z4+c(3,6,5,n)*
     & x3y6z5+c(3,6,6,n)*x3y6z6+c(4,0,0,n)*x4+c(4,0,1,n)*x4z1+c(4,0,2,
     & n)*x4z2+c(4,0,3,n)*x4z3+c(4,0,4,n)*x4z4+c(4,0,5,n)*x4z5+c(4,0,
     & 6,n)*x4z6+c(4,1,0,n)*x4y1+c(4,1,1,n)*x4y1z1+c(4,1,2,n)*x4y1z2+
     & c(4,1,3,n)*x4y1z3+c(4,1,4,n)*x4y1z4+c(4,1,5,n)*x4y1z5+c(4,1,6,
     & n)*x4y1z6+c(4,2,0,n)*x4y2+c(4,2,1,n)*x4y2z1+c(4,2,2,n)*x4y2z2+
     & c(4,2,3,n)*x4y2z3+c(4,2,4,n)*x4y2z4+c(4,2,5,n)*x4y2z5+c(4,2,6,
     & n)*x4y2z6+c(4,3,0,n)*x4y3+c(4,3,1,n)*x4y3z1+c(4,3,2,n)*x4y3z2+
     & c(4,3,3,n)*x4y3z3+c(4,3,4,n)*x4y3z4+c(4,3,5,n)*x4y3z5+c(4,3,6,
     & n)*x4y3z6+c(4,4,0,n)*x4y4+c(4,4,1,n)*x4y4z1+c(4,4,2,n)*x4y4z2+
     & c(4,4,3,n)*x4y4z3+c(4,4,4,n)*x4y4z4+c(4,4,5,n)*x4y4z5+c(4,4,6,
     & n)*x4y4z6+c(4,5,0,n)*x4y5+c(4,5,1,n)*x4y5z1+c(4,5,2,n)*x4y5z2+
     & c(4,5,3,n)*x4y5z3+c(4,5,4,n)*x4y5z4+c(4,5,5,n)*x4y5z5+c(4,5,6,
     & n)*x4y5z6+c(4,6,0,n)*x4y6+c(4,6,1,n)*x4y6z1+c(4,6,2,n)*x4y6z2+
     & c(4,6,3,n)*x4y6z3+c(4,6,4,n)*x4y6z4+c(4,6,5,n)*x4y6z5+c(4,6,6,
     & n)*x4y6z6+c(5,0,0,n)*x5+c(5,0,1,n)*x5z1+c(5,0,2,n)*x5z2+c(5,0,
     & 3,n)*x5z3+c(5,0,4,n)*x5z4+c(5,0,5,n)*x5z5+c(5,0,6,n)*x5z6+c(5,
     & 1,0,n)*x5y1+c(5,1,1,n)*x5y1z1+c(5,1,2,n)*x5y1z2+c(5,1,3,n)*
     & x5y1z3+c(5,1,4,n)*x5y1z4+c(5,1,5,n)*x5y1z5+c(5,1,6,n)*x5y1z6+c(
     & 5,2,0,n)*x5y2+c(5,2,1,n)*x5y2z1+c(5,2,2,n)*x5y2z2+c(5,2,3,n)*
     & x5y2z3+c(5,2,4,n)*x5y2z4+c(5,2,5,n)*x5y2z5+c(5,2,6,n)*x5y2z6+c(
     & 5,3,0,n)*x5y3+c(5,3,1,n)*x5y3z1+c(5,3,2,n)*x5y3z2+c(5,3,3,n)*
     & x5y3z3+c(5,3,4,n)*x5y3z4+c(5,3,5,n)*x5y3z5+c(5,3,6,n)*x5y3z6+c(
     & 5,4,0,n)*x5y4+c(5,4,1,n)*x5y4z1+c(5,4,2,n)*x5y4z2+c(5,4,3,n)*
     & x5y4z3+c(5,4,4,n)*x5y4z4+c(5,4,5,n)*x5y4z5+c(5,4,6,n)*x5y4z6+c(
     & 5,5,0,n)*x5y5+c(5,5,1,n)*x5y5z1+c(5,5,2,n)*x5y5z2)
       r=(r+c(5,5,3,n)*x5y5z3+c(5,5,4,n)*x5y5z4+c(5,5,5,n)*x5y5z5+c(5,
     & 5,6,n)*x5y5z6+c(5,6,0,n)*x5y6+c(5,6,1,n)*x5y6z1+c(5,6,2,n)*
     & x5y6z2+c(5,6,3,n)*x5y6z3+c(5,6,4,n)*x5y6z4+c(5,6,5,n)*x5y6z5+c(
     & 5,6,6,n)*x5y6z6+c(6,0,0,n)*x6+c(6,0,1,n)*x6z1+c(6,0,2,n)*x6z2+
     & c(6,0,3,n)*x6z3+c(6,0,4,n)*x6z4+c(6,0,5,n)*x6z5+c(6,0,6,n)*
     & x6z6+c(6,1,0,n)*x6y1+c(6,1,1,n)*x6y1z1+c(6,1,2,n)*x6y1z2+c(6,1,
     & 3,n)*x6y1z3+c(6,1,4,n)*x6y1z4+c(6,1,5,n)*x6y1z5+c(6,1,6,n)*
     & x6y1z6+c(6,2,0,n)*x6y2+c(6,2,1,n)*x6y2z1+c(6,2,2,n)*x6y2z2+c(6,
     & 2,3,n)*x6y2z3+c(6,2,4,n)*x6y2z4+c(6,2,5,n)*x6y2z5+c(6,2,6,n)*
     & x6y2z6+c(6,3,0,n)*x6y3+c(6,3,1,n)*x6y3z1+c(6,3,2,n)*x6y3z2+c(6,
     & 3,3,n)*x6y3z3+c(6,3,4,n)*x6y3z4+c(6,3,5,n)*x6y3z5+c(6,3,6,n)*
     & x6y3z6+c(6,4,0,n)*x6y4+c(6,4,1,n)*x6y4z1+c(6,4,2,n)*x6y4z2+c(6,
     & 4,3,n)*x6y4z3+c(6,4,4,n)*x6y4z4+c(6,4,5,n)*x6y4z5+c(6,4,6,n)*
     & x6y4z6+c(6,5,0,n)*x6y5+c(6,5,1,n)*x6y5z1+c(6,5,2,n)*x6y5z2+c(6,
     & 5,3,n)*x6y5z3+c(6,5,4,n)*x6y5z4+c(6,5,5,n)*x6y5z5+c(6,5,6,n)*
     & x6y5z6+c(6,6,0,n)*x6y6+c(6,6,1,n)*x6y6z1+c(6,6,2,n)*x6y6z2+c(6,
     & 6,3,n)*x6y6z3+c(6,6,4,n)*x6y6z4+c(6,6,5,n)*x6y6z5+c(6,6,6,n)*
     & x6y6z6)*time
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
      z6=z5*z1
c
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y1z5=y1z4*z1
      y1z6=y1z5*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
      y2z5=y1z5*y1
      y2z6=y1z6*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      y3z4=y2z4*y1
      y3z5=y2z5*y1
      y3z6=y2z6*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y4z2=y3z2*y1
      y4z3=y3z3*y1
      y4z4=y3z4*y1
      y4z5=y3z5*y1
      y4z6=y3z6*y1
      y5=y4*y1
      y5z1=y4z1*y1
      y5z2=y4z2*y1
      y5z3=y4z3*y1
      y5z4=y4z4*y1
      y5z5=y4z5*y1
      y5z6=y4z6*y1
      y6=y5*y1
      y6z1=y5z1*y1
      y6z2=y5z2*y1
      y6z3=y5z3*y1
      y6z4=y5z4*y1
      y6z5=y5z5*y1
      y6z6=y5z6*y1
c
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1z5=x1z4*z1
      x1z6=x1z5*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y1z5=x1y1z4*z1
      x1y1z6=x1y1z5*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y2z4=x1y1z4*y1
      x1y2z5=x1y1z5*y1
      x1y2z6=x1y1z6*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x1y3z4=x1y2z4*y1
      x1y3z5=x1y2z5*y1
      x1y3z6=x1y2z6*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y4z2=x1y3z2*y1
      x1y4z3=x1y3z3*y1
      x1y4z4=x1y3z4*y1
      x1y4z5=x1y3z5*y1
      x1y4z6=x1y3z6*y1
      x1y5=x1y4*y1
      x1y5z1=x1y4z1*y1
      x1y5z2=x1y4z2*y1
      x1y5z3=x1y4z3*y1
      x1y5z4=x1y4z4*y1
      x1y5z5=x1y4z5*y1
      x1y5z6=x1y4z6*y1
      x1y6=x1y5*y1
      x1y6z1=x1y5z1*y1
      x1y6z2=x1y5z2*y1
      x1y6z3=x1y5z3*y1
      x1y6z4=x1y5z4*y1
      x1y6z5=x1y5z5*y1
      x1y6z6=x1y5z6*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x2z5=x1z5*x1
      x2z6=x1z6*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y1z4=x1y1z4*x1
      x2y1z5=x1y1z5*x1
      x2y1z6=x1y1z6*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y2z4=x1y2z4*x1
      x2y2z5=x1y2z5*x1
      x2y2z6=x1y2z6*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y3z3=x1y3z3*x1
      x2y3z4=x1y3z4*x1
      x2y3z5=x1y3z5*x1
      x2y3z6=x1y3z6*x1
      x2y4=x1y4*x1
      x2y4z1=x1y4z1*x1
      x2y4z2=x1y4z2*x1
      x2y4z3=x1y4z3*x1
      x2y4z4=x1y4z4*x1
      x2y4z5=x1y4z5*x1
      x2y4z6=x1y4z6*x1
      x2y5=x1y5*x1
      x2y5z1=x1y5z1*x1
      x2y5z2=x1y5z2*x1
      x2y5z3=x1y5z3*x1
      x2y5z4=x1y5z4*x1
      x2y5z5=x1y5z5*x1
      x2y5z6=x1y5z6*x1
      x2y6=x1y6*x1
      x2y6z1=x1y6z1*x1
      x2y6z2=x1y6z2*x1
      x2y6z3=x1y6z3*x1
      x2y6z4=x1y6z4*x1
      x2y6z5=x1y6z5*x1
      x2y6z6=x1y6z6*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3z4=x2z4*x1
      x3z5=x2z5*x1
      x3z6=x2z6*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x3y1z4=x2y1z4*x1
      x3y1z5=x2y1z5*x1
      x3y1z6=x2y1z6*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y2z3=x2y2z3*x1
      x3y2z4=x2y2z4*x1
      x3y2z5=x2y2z5*x1
      x3y2z6=x2y2z6*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x3y3z2=x2y3z2*x1
      x3y3z3=x2y3z3*x1
      x3y3z4=x2y3z4*x1
      x3y3z5=x2y3z5*x1
      x3y3z6=x2y3z6*x1
      x3y4=x2y4*x1
      x3y4z1=x2y4z1*x1
      x3y4z2=x2y4z2*x1
      x3y4z3=x2y4z3*x1
      x3y4z4=x2y4z4*x1
      x3y4z5=x2y4z5*x1
      x3y4z6=x2y4z6*x1
      x3y5=x2y5*x1
      x3y5z1=x2y5z1*x1
      x3y5z2=x2y5z2*x1
      x3y5z3=x2y5z3*x1
      x3y5z4=x2y5z4*x1
      x3y5z5=x2y5z5*x1
      x3y5z6=x2y5z6*x1
      x3y6=x2y6*x1
      x3y6z1=x2y6z1*x1
      x3y6z2=x2y6z2*x1
      x3y6z3=x2y6z3*x1
      x3y6z4=x2y6z4*x1
      x3y6z5=x2y6z5*x1
      x3y6z6=x2y6z6*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4z2=x3z2*x1
      x4z3=x3z3*x1
      x4z4=x3z4*x1
      x4z5=x3z5*x1
      x4z6=x3z6*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y1z2=x3y1z2*x1
      x4y1z3=x3y1z3*x1
      x4y1z4=x3y1z4*x1
      x4y1z5=x3y1z5*x1
      x4y1z6=x3y1z6*x1
      x4y2=x3y2*x1
      x4y2z1=x3y2z1*x1
      x4y2z2=x3y2z2*x1
      x4y2z3=x3y2z3*x1
      x4y2z4=x3y2z4*x1
      x4y2z5=x3y2z5*x1
      x4y2z6=x3y2z6*x1
      x4y3=x3y3*x1
      x4y3z1=x3y3z1*x1
      x4y3z2=x3y3z2*x1
      x4y3z3=x3y3z3*x1
      x4y3z4=x3y3z4*x1
      x4y3z5=x3y3z5*x1
      x4y3z6=x3y3z6*x1
      x4y4=x3y4*x1
      x4y4z1=x3y4z1*x1
      x4y4z2=x3y4z2*x1
      x4y4z3=x3y4z3*x1
      x4y4z4=x3y4z4*x1
      x4y4z5=x3y4z5*x1
      x4y4z6=x3y4z6*x1
      x4y5=x3y5*x1
      x4y5z1=x3y5z1*x1
      x4y5z2=x3y5z2*x1
      x4y5z3=x3y5z3*x1
      x4y5z4=x3y5z4*x1
      x4y5z5=x3y5z5*x1
      x4y5z6=x3y5z6*x1
      x4y6=x3y6*x1
      x4y6z1=x3y6z1*x1
      x4y6z2=x3y6z2*x1
      x4y6z3=x3y6z3*x1
      x4y6z4=x3y6z4*x1
      x4y6z5=x3y6z5*x1
      x4y6z6=x3y6z6*x1
      x5=x4*x1
      x5z1=x4z1*x1
      x5z2=x4z2*x1
      x5z3=x4z3*x1
      x5z4=x4z4*x1
      x5z5=x4z5*x1
      x5z6=x4z6*x1
      x5y1=x4y1*x1
      x5y1z1=x4y1z1*x1
      x5y1z2=x4y1z2*x1
      x5y1z3=x4y1z3*x1
      x5y1z4=x4y1z4*x1
      x5y1z5=x4y1z5*x1
      x5y1z6=x4y1z6*x1
      x5y2=x4y2*x1
      x5y2z1=x4y2z1*x1
      x5y2z2=x4y2z2*x1
      x5y2z3=x4y2z3*x1
      x5y2z4=x4y2z4*x1
      x5y2z5=x4y2z5*x1
      x5y2z6=x4y2z6*x1
      x5y3=x4y3*x1
      x5y3z1=x4y3z1*x1
      x5y3z2=x4y3z2*x1
      x5y3z3=x4y3z3*x1
      x5y3z4=x4y3z4*x1
      x5y3z5=x4y3z5*x1
      x5y3z6=x4y3z6*x1
      x5y4=x4y4*x1
      x5y4z1=x4y4z1*x1
      x5y4z2=x4y4z2*x1
      x5y4z3=x4y4z3*x1
      x5y4z4=x4y4z4*x1
      x5y4z5=x4y4z5*x1
      x5y4z6=x4y4z6*x1
      x5y5=x4y5*x1
      x5y5z1=x4y5z1*x1
      x5y5z2=x4y5z2*x1
      x5y5z3=x4y5z3*x1
      x5y5z4=x4y5z4*x1
      x5y5z5=x4y5z5*x1
      x5y5z6=x4y5z6*x1
      x5y6=x4y6*x1
      x5y6z1=x4y6z1*x1
      x5y6z2=x4y6z2*x1
      x5y6z3=x4y6z3*x1
      x5y6z4=x4y6z4*x1
      x5y6z5=x4y6z5*x1
      x5y6z6=x4y6z6*x1
      x6=x5*x1
      x6z1=x5z1*x1
      x6z2=x5z2*x1
      x6z3=x5z3*x1
      x6z4=x5z4*x1
      x6z5=x5z5*x1
      x6z6=x5z6*x1
      x6y1=x5y1*x1
      x6y1z1=x5y1z1*x1
      x6y1z2=x5y1z2*x1
      x6y1z3=x5y1z3*x1
      x6y1z4=x5y1z4*x1
      x6y1z5=x5y1z5*x1
      x6y1z6=x5y1z6*x1
      x6y2=x5y2*x1
      x6y2z1=x5y2z1*x1
      x6y2z2=x5y2z2*x1
      x6y2z3=x5y2z3*x1
      x6y2z4=x5y2z4*x1
      x6y2z5=x5y2z5*x1
      x6y2z6=x5y2z6*x1
      x6y3=x5y3*x1
      x6y3z1=x5y3z1*x1
      x6y3z2=x5y3z2*x1
      x6y3z3=x5y3z3*x1
      x6y3z4=x5y3z4*x1
      x6y3z5=x5y3z5*x1
      x6y3z6=x5y3z6*x1
      x6y4=x5y4*x1
      x6y4z1=x5y4z1*x1
      x6y4z2=x5y4z2*x1
      x6y4z3=x5y4z3*x1
      x6y4z4=x5y4z4*x1
      x6y4z5=x5y4z5*x1
      x6y4z6=x5y4z6*x1
      x6y5=x5y5*x1
      x6y5z1=x5y5z1*x1
      x6y5z2=x5y5z2*x1
      x6y5z3=x5y5z3*x1
      x6y5z4=x5y5z4*x1
      x6y5z5=x5y5z5*x1
      x6y5z6=x5y5z6*x1
      x6y6=x5y6*x1
      x6y6z1=x5y6z1*x1
      x6y6z2=x5y6z2*x1
      x6y6z3=x5y6z3*x1
      x6y6z4=x5y6z4*x1
      x6y6z5=x5y6z5*x1
      x6y6z6=x5y6z6*x1
      r=(c(0,0,0,n)+c(0,0,1,n)*z1+c(0,0,2,n)*z2+c(0,0,3,n)*z3+c(0,0,4,
     & n)*z4+c(0,0,5,n)*z5+c(0,0,6,n)*z6+c(0,1,0,n)*y1+c(0,1,1,n)*
     & y1z1+c(0,1,2,n)*y1z2+c(0,1,3,n)*y1z3+c(0,1,4,n)*y1z4+c(0,1,5,n)
     & *y1z5+c(0,1,6,n)*y1z6+c(0,2,0,n)*y2+c(0,2,1,n)*y2z1+c(0,2,2,n)*
     & y2z2+c(0,2,3,n)*y2z3+c(0,2,4,n)*y2z4+c(0,2,5,n)*y2z5+c(0,2,6,n)
     & *y2z6+c(0,3,0,n)*y3+c(0,3,1,n)*y3z1+c(0,3,2,n)*y3z2+c(0,3,3,n)*
     & y3z3+c(0,3,4,n)*y3z4+c(0,3,5,n)*y3z5+c(0,3,6,n)*y3z6+c(0,4,0,n)
     & *y4+c(0,4,1,n)*y4z1+c(0,4,2,n)*y4z2+c(0,4,3,n)*y4z3+c(0,4,4,n)*
     & y4z4+c(0,4,5,n)*y4z5+c(0,4,6,n)*y4z6+c(0,5,0,n)*y5+c(0,5,1,n)*
     & y5z1+c(0,5,2,n)*y5z2+c(0,5,3,n)*y5z3+c(0,5,4,n)*y5z4+c(0,5,5,n)
     & *y5z5+c(0,5,6,n)*y5z6+c(0,6,0,n)*y6+c(0,6,1,n)*y6z1+c(0,6,2,n)*
     & y6z2+c(0,6,3,n)*y6z3+c(0,6,4,n)*y6z4+c(0,6,5,n)*y6z5+c(0,6,6,n)
     & *y6z6+c(1,0,0,n)*x1+c(1,0,1,n)*x1z1+c(1,0,2,n)*x1z2+c(1,0,3,n)*
     & x1z3+c(1,0,4,n)*x1z4+c(1,0,5,n)*x1z5+c(1,0,6,n)*x1z6+c(1,1,0,n)
     & *x1y1+c(1,1,1,n)*x1y1z1+c(1,1,2,n)*x1y1z2+c(1,1,3,n)*x1y1z3+c(
     & 1,1,4,n)*x1y1z4+c(1,1,5,n)*x1y1z5+c(1,1,6,n)*x1y1z6+c(1,2,0,n)*
     & x1y2+c(1,2,1,n)*x1y2z1+c(1,2,2,n)*x1y2z2+c(1,2,3,n)*x1y2z3+c(1,
     & 2,4,n)*x1y2z4+c(1,2,5,n)*x1y2z5+c(1,2,6,n)*x1y2z6+c(1,3,0,n)*
     & x1y3+c(1,3,1,n)*x1y3z1+c(1,3,2,n)*x1y3z2+c(1,3,3,n)*x1y3z3+c(1,
     & 3,4,n)*x1y3z4+c(1,3,5,n)*x1y3z5+c(1,3,6,n)*x1y3z6+c(1,4,0,n)*
     & x1y4+c(1,4,1,n)*x1y4z1+c(1,4,2,n)*x1y4z2+c(1,4,3,n)*x1y4z3+c(1,
     & 4,4,n)*x1y4z4+c(1,4,5,n)*x1y4z5+c(1,4,6,n)*x1y4z6+c(1,5,0,n)*
     & x1y5+c(1,5,1,n)*x1y5z1+c(1,5,2,n)*x1y5z2+c(1,5,3,n)*x1y5z3+c(1,
     & 5,4,n)*x1y5z4+c(1,5,5,n)*x1y5z5+c(1,5,6,n)*x1y5z6+c(1,6,0,n)*
     & x1y6+c(1,6,1,n)*x1y6z1+c(1,6,2,n)*x1y6z2+c(1,6,3,n)*x1y6z3+c(1,
     & 6,4,n)*x1y6z4+c(1,6,5,n)*x1y6z5+c(1,6,6,n)*x1y6z6+c(2,0,0,n)*
     & x2+c(2,0,1,n)*x2z1+c(2,0,2,n)*x2z2+c(2,0,3,n)*x2z3+c(2,0,4,n)*
     & x2z4+c(2,0,5,n)*x2z5+c(2,0,6,n)*x2z6+c(2,1,0,n)*x2y1+c(2,1,1,n)
     & *x2y1z1+c(2,1,2,n)*x2y1z2+c(2,1,3,n)*x2y1z3+c(2,1,4,n)*x2y1z4+
     & c(2,1,5,n)*x2y1z5+c(2,1,6,n)*x2y1z6+c(2,2,0,n)*x2y2+c(2,2,1,n)*
     & x2y2z1+c(2,2,2,n)*x2y2z2+c(2,2,3,n)*x2y2z3+c(2,2,4,n)*x2y2z4+c(
     & 2,2,5,n)*x2y2z5+c(2,2,6,n)*x2y2z6+c(2,3,0,n)*x2y3+c(2,3,1,n)*
     & x2y3z1+c(2,3,2,n)*x2y3z2+c(2,3,3,n)*x2y3z3+c(2,3,4,n)*x2y3z4+c(
     & 2,3,5,n)*x2y3z5+c(2,3,6,n)*x2y3z6+c(2,4,0,n)*x2y4+c(2,4,1,n)*
     & x2y4z1+c(2,4,2,n)*x2y4z2+c(2,4,3,n)*x2y4z3+c(2,4,4,n)*x2y4z4+c(
     & 2,4,5,n)*x2y4z5+c(2,4,6,n)*x2y4z6+c(2,5,0,n)*x2y5+c(2,5,1,n)*
     & x2y5z1+c(2,5,2,n)*x2y5z2+c(2,5,3,n)*x2y5z3+c(2,5,4,n)*x2y5z4+c(
     & 2,5,5,n)*x2y5z5+c(2,5,6,n)*x2y5z6+c(2,6,0,n)*x2y6+c(2,6,1,n)*
     & x2y6z1+c(2,6,2,n)*x2y6z2+c(2,6,3,n)*x2y6z3+c(2,6,4,n)*x2y6z4+c(
     & 2,6,5,n)*x2y6z5+c(2,6,6,n)*x2y6z6+c(3,0,0,n)*x3+c(3,0,1,n)*
     & x3z1+c(3,0,2,n)*x3z2+c(3,0,3,n)*x3z3+c(3,0,4,n)*x3z4+c(3,0,5,n)
     & *x3z5+c(3,0,6,n)*x3z6+c(3,1,0,n)*x3y1+c(3,1,1,n)*x3y1z1+c(3,1,
     & 2,n)*x3y1z2+c(3,1,3,n)*x3y1z3+c(3,1,4,n)*x3y1z4+c(3,1,5,n)*
     & x3y1z5+c(3,1,6,n)*x3y1z6+c(3,2,0,n)*x3y2+c(3,2,1,n)*x3y2z1+c(3,
     & 2,2,n)*x3y2z2+c(3,2,3,n)*x3y2z3+c(3,2,4,n)*x3y2z4+c(3,2,5,n)*
     & x3y2z5+c(3,2,6,n)*x3y2z6+c(3,3,0,n)*x3y3+c(3,3,1,n)*x3y3z1+c(3,
     & 3,2,n)*x3y3z2+c(3,3,3,n)*x3y3z3+c(3,3,4,n)*x3y3z4+c(3,3,5,n)*
     & x3y3z5+c(3,3,6,n)*x3y3z6+c(3,4,0,n)*x3y4+c(3,4,1,n)*x3y4z1+c(3,
     & 4,2,n)*x3y4z2+c(3,4,3,n)*x3y4z3+c(3,4,4,n)*x3y4z4+c(3,4,5,n)*
     & x3y4z5+c(3,4,6,n)*x3y4z6+c(3,5,0,n)*x3y5+c(3,5,1,n)*x3y5z1+c(3,
     & 5,2,n)*x3y5z2+c(3,5,3,n)*x3y5z3+c(3,5,4,n)*x3y5z4+c(3,5,5,n)*
     & x3y5z5+c(3,5,6,n)*x3y5z6+c(3,6,0,n)*x3y6+c(3,6,1,n)*x3y6z1+c(3,
     & 6,2,n)*x3y6z2+c(3,6,3,n)*x3y6z3+c(3,6,4,n)*x3y6z4+c(3,6,5,n)*
     & x3y6z5+c(3,6,6,n)*x3y6z6+c(4,0,0,n)*x4+c(4,0,1,n)*x4z1+c(4,0,2,
     & n)*x4z2+c(4,0,3,n)*x4z3+c(4,0,4,n)*x4z4+c(4,0,5,n)*x4z5+c(4,0,
     & 6,n)*x4z6+c(4,1,0,n)*x4y1+c(4,1,1,n)*x4y1z1+c(4,1,2,n)*x4y1z2+
     & c(4,1,3,n)*x4y1z3+c(4,1,4,n)*x4y1z4+c(4,1,5,n)*x4y1z5+c(4,1,6,
     & n)*x4y1z6+c(4,2,0,n)*x4y2+c(4,2,1,n)*x4y2z1+c(4,2,2,n)*x4y2z2+
     & c(4,2,3,n)*x4y2z3+c(4,2,4,n)*x4y2z4+c(4,2,5,n)*x4y2z5+c(4,2,6,
     & n)*x4y2z6+c(4,3,0,n)*x4y3+c(4,3,1,n)*x4y3z1+c(4,3,2,n)*x4y3z2+
     & c(4,3,3,n)*x4y3z3+c(4,3,4,n)*x4y3z4+c(4,3,5,n)*x4y3z5+c(4,3,6,
     & n)*x4y3z6+c(4,4,0,n)*x4y4+c(4,4,1,n)*x4y4z1+c(4,4,2,n)*x4y4z2+
     & c(4,4,3,n)*x4y4z3+c(4,4,4,n)*x4y4z4+c(4,4,5,n)*x4y4z5+c(4,4,6,
     & n)*x4y4z6+c(4,5,0,n)*x4y5+c(4,5,1,n)*x4y5z1+c(4,5,2,n)*x4y5z2+
     & c(4,5,3,n)*x4y5z3+c(4,5,4,n)*x4y5z4+c(4,5,5,n)*x4y5z5+c(4,5,6,
     & n)*x4y5z6+c(4,6,0,n)*x4y6+c(4,6,1,n)*x4y6z1+c(4,6,2,n)*x4y6z2+
     & c(4,6,3,n)*x4y6z3+c(4,6,4,n)*x4y6z4+c(4,6,5,n)*x4y6z5+c(4,6,6,
     & n)*x4y6z6+c(5,0,0,n)*x5+c(5,0,1,n)*x5z1+c(5,0,2,n)*x5z2+c(5,0,
     & 3,n)*x5z3+c(5,0,4,n)*x5z4+c(5,0,5,n)*x5z5+c(5,0,6,n)*x5z6+c(5,
     & 1,0,n)*x5y1+c(5,1,1,n)*x5y1z1+c(5,1,2,n)*x5y1z2+c(5,1,3,n)*
     & x5y1z3+c(5,1,4,n)*x5y1z4+c(5,1,5,n)*x5y1z5+c(5,1,6,n)*x5y1z6+c(
     & 5,2,0,n)*x5y2+c(5,2,1,n)*x5y2z1+c(5,2,2,n)*x5y2z2+c(5,2,3,n)*
     & x5y2z3+c(5,2,4,n)*x5y2z4+c(5,2,5,n)*x5y2z5+c(5,2,6,n)*x5y2z6+c(
     & 5,3,0,n)*x5y3+c(5,3,1,n)*x5y3z1+c(5,3,2,n)*x5y3z2+c(5,3,3,n)*
     & x5y3z3+c(5,3,4,n)*x5y3z4+c(5,3,5,n)*x5y3z5+c(5,3,6,n)*x5y3z6+c(
     & 5,4,0,n)*x5y4+c(5,4,1,n)*x5y4z1+c(5,4,2,n)*x5y4z2+c(5,4,3,n)*
     & x5y4z3+c(5,4,4,n)*x5y4z4+c(5,4,5,n)*x5y4z5+c(5,4,6,n)*x5y4z6+c(
     & 5,5,0,n)*x5y5+c(5,5,1,n)*x5y5z1+c(5,5,2,n)*x5y5z2)
       r=(r+c(5,5,3,n)*x5y5z3+c(5,5,4,n)*x5y5z4+c(5,5,5,n)*x5y5z5+c(5,
     & 5,6,n)*x5y5z6+c(5,6,0,n)*x5y6+c(5,6,1,n)*x5y6z1+c(5,6,2,n)*
     & x5y6z2+c(5,6,3,n)*x5y6z3+c(5,6,4,n)*x5y6z4+c(5,6,5,n)*x5y6z5+c(
     & 5,6,6,n)*x5y6z6+c(6,0,0,n)*x6+c(6,0,1,n)*x6z1+c(6,0,2,n)*x6z2+
     & c(6,0,3,n)*x6z3+c(6,0,4,n)*x6z4+c(6,0,5,n)*x6z5+c(6,0,6,n)*
     & x6z6+c(6,1,0,n)*x6y1+c(6,1,1,n)*x6y1z1+c(6,1,2,n)*x6y1z2+c(6,1,
     & 3,n)*x6y1z3+c(6,1,4,n)*x6y1z4+c(6,1,5,n)*x6y1z5+c(6,1,6,n)*
     & x6y1z6+c(6,2,0,n)*x6y2+c(6,2,1,n)*x6y2z1+c(6,2,2,n)*x6y2z2+c(6,
     & 2,3,n)*x6y2z3+c(6,2,4,n)*x6y2z4+c(6,2,5,n)*x6y2z5+c(6,2,6,n)*
     & x6y2z6+c(6,3,0,n)*x6y3+c(6,3,1,n)*x6y3z1+c(6,3,2,n)*x6y3z2+c(6,
     & 3,3,n)*x6y3z3+c(6,3,4,n)*x6y3z4+c(6,3,5,n)*x6y3z5+c(6,3,6,n)*
     & x6y3z6+c(6,4,0,n)*x6y4+c(6,4,1,n)*x6y4z1+c(6,4,2,n)*x6y4z2+c(6,
     & 4,3,n)*x6y4z3+c(6,4,4,n)*x6y4z4+c(6,4,5,n)*x6y4z5+c(6,4,6,n)*
     & x6y4z6+c(6,5,0,n)*x6y5+c(6,5,1,n)*x6y5z1+c(6,5,2,n)*x6y5z2+c(6,
     & 5,3,n)*x6y5z3+c(6,5,4,n)*x6y5z4+c(6,5,5,n)*x6y5z5+c(6,5,6,n)*
     & x6y5z6+c(6,6,0,n)*x6y6+c(6,6,1,n)*x6y6z1+c(6,6,2,n)*x6y6z2+c(6,
     & 6,3,n)*x6y6z3+c(6,6,4,n)*x6y6z4+c(6,6,5,n)*x6y6z5+c(6,6,6,n)*
     & x6y6z6)*time
! endLoops()
      else if( dx.eq.0.and.dy.eq.0.and.dz.eq.1.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
c 
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
      y6=y5*y1
      y6z1=y5z1*y1
      y6z2=y5z2*y1
      y6z3=y5z3*y1
      y6z4=y5z4*y1
      y6z5=y5z5*y1
c 
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
      x1y6=x1y5*y1
      x1y6z1=x1y5z1*y1
      x1y6z2=x1y5z2*y1
      x1y6z3=x1y5z3*y1
      x1y6z4=x1y5z4*y1
      x1y6z5=x1y5z5*y1
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
      x2y6=x1y6*x1
      x2y6z1=x1y6z1*x1
      x2y6z2=x1y6z2*x1
      x2y6z3=x1y6z3*x1
      x2y6z4=x1y6z4*x1
      x2y6z5=x1y6z5*x1
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
      x3y6=x2y6*x1
      x3y6z1=x2y6z1*x1
      x3y6z2=x2y6z2*x1
      x3y6z3=x2y6z3*x1
      x3y6z4=x2y6z4*x1
      x3y6z5=x2y6z5*x1
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
      x4y6=x3y6*x1
      x4y6z1=x3y6z1*x1
      x4y6z2=x3y6z2*x1
      x4y6z3=x3y6z3*x1
      x4y6z4=x3y6z4*x1
      x4y6z5=x3y6z5*x1
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
      x5y6=x4y6*x1
      x5y6z1=x4y6z1*x1
      x5y6z2=x4y6z2*x1
      x5y6z3=x4y6z3*x1
      x5y6z4=x4y6z4*x1
      x5y6z5=x4y6z5*x1
      x6=x5*x1
      x6z1=x5z1*x1
      x6z2=x5z2*x1
      x6z3=x5z3*x1
      x6z4=x5z4*x1
      x6z5=x5z5*x1
      x6y1=x5y1*x1
      x6y1z1=x5y1z1*x1
      x6y1z2=x5y1z2*x1
      x6y1z3=x5y1z3*x1
      x6y1z4=x5y1z4*x1
      x6y1z5=x5y1z5*x1
      x6y2=x5y2*x1
      x6y2z1=x5y2z1*x1
      x6y2z2=x5y2z2*x1
      x6y2z3=x5y2z3*x1
      x6y2z4=x5y2z4*x1
      x6y2z5=x5y2z5*x1
      x6y3=x5y3*x1
      x6y3z1=x5y3z1*x1
      x6y3z2=x5y3z2*x1
      x6y3z3=x5y3z3*x1
      x6y3z4=x5y3z4*x1
      x6y3z5=x5y3z5*x1
      x6y4=x5y4*x1
      x6y4z1=x5y4z1*x1
      x6y4z2=x5y4z2*x1
      x6y4z3=x5y4z3*x1
      x6y4z4=x5y4z4*x1
      x6y4z5=x5y4z5*x1
      x6y5=x5y5*x1
      x6y5z1=x5y5z1*x1
      x6y5z2=x5y5z2*x1
      x6y5z3=x5y5z3*x1
      x6y5z4=x5y5z4*x1
      x6y5z5=x5y5z5*x1
      x6y6=x5y6*x1
      x6y6z1=x5y6z1*x1
      x6y6z2=x5y6z2*x1
      x6y6z3=x5y6z3*x1
      x6y6z4=x5y6z4*x1
      x6y6z5=x5y6z5*x1
      r=(c(0,0,1,n)+c(0,0,2,n)*z1*2.+c(0,0,3,n)*z2*3.+c(0,0,4,n)*z3*4.+
     & c(0,0,5,n)*z4*5.+c(0,0,6,n)*z5*6.+c(0,1,1,n)*y1+c(0,1,2,n)*
     & y1z1*2.+c(0,1,3,n)*y1z2*3.+c(0,1,4,n)*y1z3*4.+c(0,1,5,n)*y1z4*
     & 5.+c(0,1,6,n)*y1z5*6.+c(0,2,1,n)*y2+c(0,2,2,n)*y2z1*2.+c(0,2,3,
     & n)*y2z2*3.+c(0,2,4,n)*y2z3*4.+c(0,2,5,n)*y2z4*5.+c(0,2,6,n)*
     & y2z5*6.+c(0,3,1,n)*y3+c(0,3,2,n)*y3z1*2.+c(0,3,3,n)*y3z2*3.+c(
     & 0,3,4,n)*y3z3*4.+c(0,3,5,n)*y3z4*5.+c(0,3,6,n)*y3z5*6.+c(0,4,1,
     & n)*y4+c(0,4,2,n)*y4z1*2.+c(0,4,3,n)*y4z2*3.+c(0,4,4,n)*y4z3*4.+
     & c(0,4,5,n)*y4z4*5.+c(0,4,6,n)*y4z5*6.+c(0,5,1,n)*y5+c(0,5,2,n)*
     & y5z1*2.+c(0,5,3,n)*y5z2*3.+c(0,5,4,n)*y5z3*4.+c(0,5,5,n)*y5z4*
     & 5.+c(0,5,6,n)*y5z5*6.+c(0,6,1,n)*y6+c(0,6,2,n)*y6z1*2.+c(0,6,3,
     & n)*y6z2*3.+c(0,6,4,n)*y6z3*4.+c(0,6,5,n)*y6z4*5.+c(0,6,6,n)*
     & y6z5*6.+c(1,0,1,n)*x1+c(1,0,2,n)*x1z1*2.+c(1,0,3,n)*x1z2*3.+c(
     & 1,0,4,n)*x1z3*4.+c(1,0,5,n)*x1z4*5.+c(1,0,6,n)*x1z5*6.+c(1,1,1,
     & n)*x1y1+c(1,1,2,n)*x1y1z1*2.+c(1,1,3,n)*x1y1z2*3.+c(1,1,4,n)*
     & x1y1z3*4.+c(1,1,5,n)*x1y1z4*5.+c(1,1,6,n)*x1y1z5*6.+c(1,2,1,n)*
     & x1y2+c(1,2,2,n)*x1y2z1*2.+c(1,2,3,n)*x1y2z2*3.+c(1,2,4,n)*
     & x1y2z3*4.+c(1,2,5,n)*x1y2z4*5.+c(1,2,6,n)*x1y2z5*6.+c(1,3,1,n)*
     & x1y3+c(1,3,2,n)*x1y3z1*2.+c(1,3,3,n)*x1y3z2*3.+c(1,3,4,n)*
     & x1y3z3*4.+c(1,3,5,n)*x1y3z4*5.+c(1,3,6,n)*x1y3z5*6.+c(1,4,1,n)*
     & x1y4+c(1,4,2,n)*x1y4z1*2.+c(1,4,3,n)*x1y4z2*3.+c(1,4,4,n)*
     & x1y4z3*4.+c(1,4,5,n)*x1y4z4*5.+c(1,4,6,n)*x1y4z5*6.+c(1,5,1,n)*
     & x1y5+c(1,5,2,n)*x1y5z1*2.+c(1,5,3,n)*x1y5z2*3.+c(1,5,4,n)*
     & x1y5z3*4.+c(1,5,5,n)*x1y5z4*5.+c(1,5,6,n)*x1y5z5*6.+c(1,6,1,n)*
     & x1y6+c(1,6,2,n)*x1y6z1*2.+c(1,6,3,n)*x1y6z2*3.+c(1,6,4,n)*
     & x1y6z3*4.+c(1,6,5,n)*x1y6z4*5.+c(1,6,6,n)*x1y6z5*6.+c(2,0,1,n)*
     & x2+c(2,0,2,n)*x2z1*2.+c(2,0,3,n)*x2z2*3.+c(2,0,4,n)*x2z3*4.+c(
     & 2,0,5,n)*x2z4*5.+c(2,0,6,n)*x2z5*6.+c(2,1,1,n)*x2y1+c(2,1,2,n)*
     & x2y1z1*2.+c(2,1,3,n)*x2y1z2*3.+c(2,1,4,n)*x2y1z3*4.+c(2,1,5,n)*
     & x2y1z4*5.+c(2,1,6,n)*x2y1z5*6.+c(2,2,1,n)*x2y2+c(2,2,2,n)*
     & x2y2z1*2.+c(2,2,3,n)*x2y2z2*3.+c(2,2,4,n)*x2y2z3*4.+c(2,2,5,n)*
     & x2y2z4*5.+c(2,2,6,n)*x2y2z5*6.+c(2,3,1,n)*x2y3+c(2,3,2,n)*
     & x2y3z1*2.+c(2,3,3,n)*x2y3z2*3.+c(2,3,4,n)*x2y3z3*4.+c(2,3,5,n)*
     & x2y3z4*5.+c(2,3,6,n)*x2y3z5*6.+c(2,4,1,n)*x2y4+c(2,4,2,n)*
     & x2y4z1*2.+c(2,4,3,n)*x2y4z2*3.+c(2,4,4,n)*x2y4z3*4.+c(2,4,5,n)*
     & x2y4z4*5.+c(2,4,6,n)*x2y4z5*6.+c(2,5,1,n)*x2y5+c(2,5,2,n)*
     & x2y5z1*2.+c(2,5,3,n)*x2y5z2*3.+c(2,5,4,n)*x2y5z3*4.+c(2,5,5,n)*
     & x2y5z4*5.+c(2,5,6,n)*x2y5z5*6.+c(2,6,1,n)*x2y6+c(2,6,2,n)*
     & x2y6z1*2.+c(2,6,3,n)*x2y6z2*3.+c(2,6,4,n)*x2y6z3*4.+c(2,6,5,n)*
     & x2y6z4*5.+c(2,6,6,n)*x2y6z5*6.+c(3,0,1,n)*x3+c(3,0,2,n)*x3z1*
     & 2.+c(3,0,3,n)*x3z2*3.+c(3,0,4,n)*x3z3*4.+c(3,0,5,n)*x3z4*5.+c(
     & 3,0,6,n)*x3z5*6.+c(3,1,1,n)*x3y1+c(3,1,2,n)*x3y1z1*2.+c(3,1,3,
     & n)*x3y1z2*3.+c(3,1,4,n)*x3y1z3*4.+c(3,1,5,n)*x3y1z4*5.+c(3,1,6,
     & n)*x3y1z5*6.+c(3,2,1,n)*x3y2+c(3,2,2,n)*x3y2z1*2.+c(3,2,3,n)*
     & x3y2z2*3.+c(3,2,4,n)*x3y2z3*4.+c(3,2,5,n)*x3y2z4*5.+c(3,2,6,n)*
     & x3y2z5*6.+c(3,3,1,n)*x3y3+c(3,3,2,n)*x3y3z1*2.+c(3,3,3,n)*
     & x3y3z2*3.+c(3,3,4,n)*x3y3z3*4.+c(3,3,5,n)*x3y3z4*5.+c(3,3,6,n)*
     & x3y3z5*6.+c(3,4,1,n)*x3y4+c(3,4,2,n)*x3y4z1*2.+c(3,4,3,n)*
     & x3y4z2*3.+c(3,4,4,n)*x3y4z3*4.+c(3,4,5,n)*x3y4z4*5.+c(3,4,6,n)*
     & x3y4z5*6.+c(3,5,1,n)*x3y5+c(3,5,2,n)*x3y5z1*2.+c(3,5,3,n)*
     & x3y5z2*3.+c(3,5,4,n)*x3y5z3*4.+c(3,5,5,n)*x3y5z4*5.+c(3,5,6,n)*
     & x3y5z5*6.+c(3,6,1,n)*x3y6+c(3,6,2,n)*x3y6z1*2.+c(3,6,3,n)*
     & x3y6z2*3.+c(3,6,4,n)*x3y6z3*4.+c(3,6,5,n)*x3y6z4*5.+c(3,6,6,n)*
     & x3y6z5*6.+c(4,0,1,n)*x4+c(4,0,2,n)*x4z1*2.+c(4,0,3,n)*x4z2*3.+
     & c(4,0,4,n)*x4z3*4.+c(4,0,5,n)*x4z4*5.+c(4,0,6,n)*x4z5*6.+c(4,1,
     & 1,n)*x4y1+c(4,1,2,n)*x4y1z1*2.+c(4,1,3,n)*x4y1z2*3.+c(4,1,4,n)*
     & x4y1z3*4.+c(4,1,5,n)*x4y1z4*5.+c(4,1,6,n)*x4y1z5*6.+c(4,2,1,n)*
     & x4y2+c(4,2,2,n)*x4y2z1*2.+c(4,2,3,n)*x4y2z2*3.+c(4,2,4,n)*
     & x4y2z3*4.+c(4,2,5,n)*x4y2z4*5.+c(4,2,6,n)*x4y2z5*6.+c(4,3,1,n)*
     & x4y3+c(4,3,2,n)*x4y3z1*2.+c(4,3,3,n)*x4y3z2*3.+c(4,3,4,n)*
     & x4y3z3*4.+c(4,3,5,n)*x4y3z4*5.+c(4,3,6,n)*x4y3z5*6.+c(4,4,1,n)*
     & x4y4+c(4,4,2,n)*x4y4z1*2.+c(4,4,3,n)*x4y4z2*3.+c(4,4,4,n)*
     & x4y4z3*4.+c(4,4,5,n)*x4y4z4*5.+c(4,4,6,n)*x4y4z5*6.+c(4,5,1,n)*
     & x4y5+c(4,5,2,n)*x4y5z1*2.+c(4,5,3,n)*x4y5z2*3.+c(4,5,4,n)*
     & x4y5z3*4.+c(4,5,5,n)*x4y5z4*5.+c(4,5,6,n)*x4y5z5*6.+c(4,6,1,n)*
     & x4y6+c(4,6,2,n)*x4y6z1*2.+c(4,6,3,n)*x4y6z2*3.+c(4,6,4,n)*
     & x4y6z3*4.+c(4,6,5,n)*x4y6z4*5.+c(4,6,6,n)*x4y6z5*6.+c(5,0,1,n)*
     & x5+c(5,0,2,n)*x5z1*2.+c(5,0,3,n)*x5z2*3.+c(5,0,4,n)*x5z3*4.+c(
     & 5,0,5,n)*x5z4*5.+c(5,0,6,n)*x5z5*6.+c(5,1,1,n)*x5y1+c(5,1,2,n)*
     & x5y1z1*2.+c(5,1,3,n)*x5y1z2*3.+c(5,1,4,n)*x5y1z3*4.+c(5,1,5,n)*
     & x5y1z4*5.+c(5,1,6,n)*x5y1z5*6.+c(5,2,1,n)*x5y2+c(5,2,2,n)*
     & x5y2z1*2.+c(5,2,3,n)*x5y2z2*3.+c(5,2,4,n)*x5y2z3*4.+c(5,2,5,n)*
     & x5y2z4*5.+c(5,2,6,n)*x5y2z5*6.+c(5,3,1,n)*x5y3+c(5,3,2,n)*
     & x5y3z1*2.+c(5,3,3,n)*x5y3z2*3.+c(5,3,4,n)*x5y3z3*4.+c(5,3,5,n)*
     & x5y3z4*5.+c(5,3,6,n)*x5y3z5*6.+c(5,4,1,n)*x5y4+c(5,4,2,n)*
     & x5y4z1*2.+c(5,4,3,n)*x5y4z2*3.+c(5,4,4,n)*x5y4z3*4.+c(5,4,5,n)*
     & x5y4z4*5.+c(5,4,6,n)*x5y4z5*6.+c(5,5,1,n)*x5y5+c(5,5,2,n)*
     & x5y5z1*2.+c(5,5,3,n)*x5y5z2*3.+c(5,5,4,n)*x5y5z3*4.+c(5,5,5,n)*
     & x5y5z4*5.+c(5,5,6,n)*x5y5z5*6.)
       r=(r+c(5,6,1,n)*x5y6+c(5,6,2,n)*x5y6z1*2.+c(5,6,3,n)*x5y6z2*3.+
     & c(5,6,4,n)*x5y6z3*4.+c(5,6,5,n)*x5y6z4*5.+c(5,6,6,n)*x5y6z5*6.+
     & c(6,0,1,n)*x6+c(6,0,2,n)*x6z1*2.+c(6,0,3,n)*x6z2*3.+c(6,0,4,n)*
     & x6z3*4.+c(6,0,5,n)*x6z4*5.+c(6,0,6,n)*x6z5*6.+c(6,1,1,n)*x6y1+
     & c(6,1,2,n)*x6y1z1*2.+c(6,1,3,n)*x6y1z2*3.+c(6,1,4,n)*x6y1z3*4.+
     & c(6,1,5,n)*x6y1z4*5.+c(6,1,6,n)*x6y1z5*6.+c(6,2,1,n)*x6y2+c(6,
     & 2,2,n)*x6y2z1*2.+c(6,2,3,n)*x6y2z2*3.+c(6,2,4,n)*x6y2z3*4.+c(6,
     & 2,5,n)*x6y2z4*5.+c(6,2,6,n)*x6y2z5*6.+c(6,3,1,n)*x6y3+c(6,3,2,
     & n)*x6y3z1*2.+c(6,3,3,n)*x6y3z2*3.+c(6,3,4,n)*x6y3z3*4.+c(6,3,5,
     & n)*x6y3z4*5.+c(6,3,6,n)*x6y3z5*6.+c(6,4,1,n)*x6y4+c(6,4,2,n)*
     & x6y4z1*2.+c(6,4,3,n)*x6y4z2*3.+c(6,4,4,n)*x6y4z3*4.+c(6,4,5,n)*
     & x6y4z4*5.+c(6,4,6,n)*x6y4z5*6.+c(6,5,1,n)*x6y5+c(6,5,2,n)*
     & x6y5z1*2.+c(6,5,3,n)*x6y5z2*3.+c(6,5,4,n)*x6y5z3*4.+c(6,5,5,n)*
     & x6y5z4*5.+c(6,5,6,n)*x6y5z5*6.+c(6,6,1,n)*x6y6+c(6,6,2,n)*
     & x6y6z1*2.+c(6,6,3,n)*x6y6z2*3.+c(6,6,4,n)*x6y6z3*4.+c(6,6,5,n)*
     & x6y6z4*5.+c(6,6,6,n)*x6y6z5*6.)*time
! endLoops()
      else if( dx.eq.0.and.dy.eq.0.and.dz.eq.2.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
c 
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
      y6=y5*y1
      y6z1=y5z1*y1
      y6z2=y5z2*y1
      y6z3=y5z3*y1
      y6z4=y5z4*y1
c 
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
      x1y6=x1y5*y1
      x1y6z1=x1y5z1*y1
      x1y6z2=x1y5z2*y1
      x1y6z3=x1y5z3*y1
      x1y6z4=x1y5z4*y1
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
      x2y6=x1y6*x1
      x2y6z1=x1y6z1*x1
      x2y6z2=x1y6z2*x1
      x2y6z3=x1y6z3*x1
      x2y6z4=x1y6z4*x1
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
      x3y6=x2y6*x1
      x3y6z1=x2y6z1*x1
      x3y6z2=x2y6z2*x1
      x3y6z3=x2y6z3*x1
      x3y6z4=x2y6z4*x1
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
      x4y6=x3y6*x1
      x4y6z1=x3y6z1*x1
      x4y6z2=x3y6z2*x1
      x4y6z3=x3y6z3*x1
      x4y6z4=x3y6z4*x1
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
      x5y6=x4y6*x1
      x5y6z1=x4y6z1*x1
      x5y6z2=x4y6z2*x1
      x5y6z3=x4y6z3*x1
      x5y6z4=x4y6z4*x1
      x6=x5*x1
      x6z1=x5z1*x1
      x6z2=x5z2*x1
      x6z3=x5z3*x1
      x6z4=x5z4*x1
      x6y1=x5y1*x1
      x6y1z1=x5y1z1*x1
      x6y1z2=x5y1z2*x1
      x6y1z3=x5y1z3*x1
      x6y1z4=x5y1z4*x1
      x6y2=x5y2*x1
      x6y2z1=x5y2z1*x1
      x6y2z2=x5y2z2*x1
      x6y2z3=x5y2z3*x1
      x6y2z4=x5y2z4*x1
      x6y3=x5y3*x1
      x6y3z1=x5y3z1*x1
      x6y3z2=x5y3z2*x1
      x6y3z3=x5y3z3*x1
      x6y3z4=x5y3z4*x1
      x6y4=x5y4*x1
      x6y4z1=x5y4z1*x1
      x6y4z2=x5y4z2*x1
      x6y4z3=x5y4z3*x1
      x6y4z4=x5y4z4*x1
      x6y5=x5y5*x1
      x6y5z1=x5y5z1*x1
      x6y5z2=x5y5z2*x1
      x6y5z3=x5y5z3*x1
      x6y5z4=x5y5z4*x1
      x6y6=x5y6*x1
      x6y6z1=x5y6z1*x1
      x6y6z2=x5y6z2*x1
      x6y6z3=x5y6z3*x1
      x6y6z4=x5y6z4*x1
      r=(c(0,0,2,n)*2.+c(0,0,3,n)*z1*6.+c(0,0,4,n)*z2*12.+c(0,0,5,n)*
     & z3*20.+c(0,0,6,n)*z4*30.+c(0,1,2,n)*y1*2.+c(0,1,3,n)*y1z1*6.+c(
     & 0,1,4,n)*y1z2*12.+c(0,1,5,n)*y1z3*20.+c(0,1,6,n)*y1z4*30.+c(0,
     & 2,2,n)*y2*2.+c(0,2,3,n)*y2z1*6.+c(0,2,4,n)*y2z2*12.+c(0,2,5,n)*
     & y2z3*20.+c(0,2,6,n)*y2z4*30.+c(0,3,2,n)*y3*2.+c(0,3,3,n)*y3z1*
     & 6.+c(0,3,4,n)*y3z2*12.+c(0,3,5,n)*y3z3*20.+c(0,3,6,n)*y3z4*30.+
     & c(0,4,2,n)*y4*2.+c(0,4,3,n)*y4z1*6.+c(0,4,4,n)*y4z2*12.+c(0,4,
     & 5,n)*y4z3*20.+c(0,4,6,n)*y4z4*30.+c(0,5,2,n)*y5*2.+c(0,5,3,n)*
     & y5z1*6.+c(0,5,4,n)*y5z2*12.+c(0,5,5,n)*y5z3*20.+c(0,5,6,n)*
     & y5z4*30.+c(0,6,2,n)*y6*2.+c(0,6,3,n)*y6z1*6.+c(0,6,4,n)*y6z2*
     & 12.+c(0,6,5,n)*y6z3*20.+c(0,6,6,n)*y6z4*30.+c(1,0,2,n)*x1*2.+c(
     & 1,0,3,n)*x1z1*6.+c(1,0,4,n)*x1z2*12.+c(1,0,5,n)*x1z3*20.+c(1,0,
     & 6,n)*x1z4*30.+c(1,1,2,n)*x1y1*2.+c(1,1,3,n)*x1y1z1*6.+c(1,1,4,
     & n)*x1y1z2*12.+c(1,1,5,n)*x1y1z3*20.+c(1,1,6,n)*x1y1z4*30.+c(1,
     & 2,2,n)*x1y2*2.+c(1,2,3,n)*x1y2z1*6.+c(1,2,4,n)*x1y2z2*12.+c(1,
     & 2,5,n)*x1y2z3*20.+c(1,2,6,n)*x1y2z4*30.+c(1,3,2,n)*x1y3*2.+c(1,
     & 3,3,n)*x1y3z1*6.+c(1,3,4,n)*x1y3z2*12.+c(1,3,5,n)*x1y3z3*20.+c(
     & 1,3,6,n)*x1y3z4*30.+c(1,4,2,n)*x1y4*2.+c(1,4,3,n)*x1y4z1*6.+c(
     & 1,4,4,n)*x1y4z2*12.+c(1,4,5,n)*x1y4z3*20.+c(1,4,6,n)*x1y4z4*
     & 30.+c(1,5,2,n)*x1y5*2.+c(1,5,3,n)*x1y5z1*6.+c(1,5,4,n)*x1y5z2*
     & 12.+c(1,5,5,n)*x1y5z3*20.+c(1,5,6,n)*x1y5z4*30.+c(1,6,2,n)*
     & x1y6*2.+c(1,6,3,n)*x1y6z1*6.+c(1,6,4,n)*x1y6z2*12.+c(1,6,5,n)*
     & x1y6z3*20.+c(1,6,6,n)*x1y6z4*30.+c(2,0,2,n)*x2*2.+c(2,0,3,n)*
     & x2z1*6.+c(2,0,4,n)*x2z2*12.+c(2,0,5,n)*x2z3*20.+c(2,0,6,n)*
     & x2z4*30.+c(2,1,2,n)*x2y1*2.+c(2,1,3,n)*x2y1z1*6.+c(2,1,4,n)*
     & x2y1z2*12.+c(2,1,5,n)*x2y1z3*20.+c(2,1,6,n)*x2y1z4*30.+c(2,2,2,
     & n)*x2y2*2.+c(2,2,3,n)*x2y2z1*6.+c(2,2,4,n)*x2y2z2*12.+c(2,2,5,
     & n)*x2y2z3*20.+c(2,2,6,n)*x2y2z4*30.+c(2,3,2,n)*x2y3*2.+c(2,3,3,
     & n)*x2y3z1*6.+c(2,3,4,n)*x2y3z2*12.+c(2,3,5,n)*x2y3z3*20.+c(2,3,
     & 6,n)*x2y3z4*30.+c(2,4,2,n)*x2y4*2.+c(2,4,3,n)*x2y4z1*6.+c(2,4,
     & 4,n)*x2y4z2*12.+c(2,4,5,n)*x2y4z3*20.+c(2,4,6,n)*x2y4z4*30.+c(
     & 2,5,2,n)*x2y5*2.+c(2,5,3,n)*x2y5z1*6.+c(2,5,4,n)*x2y5z2*12.+c(
     & 2,5,5,n)*x2y5z3*20.+c(2,5,6,n)*x2y5z4*30.+c(2,6,2,n)*x2y6*2.+c(
     & 2,6,3,n)*x2y6z1*6.+c(2,6,4,n)*x2y6z2*12.+c(2,6,5,n)*x2y6z3*20.+
     & c(2,6,6,n)*x2y6z4*30.+c(3,0,2,n)*x3*2.+c(3,0,3,n)*x3z1*6.+c(3,
     & 0,4,n)*x3z2*12.+c(3,0,5,n)*x3z3*20.+c(3,0,6,n)*x3z4*30.+c(3,1,
     & 2,n)*x3y1*2.+c(3,1,3,n)*x3y1z1*6.+c(3,1,4,n)*x3y1z2*12.+c(3,1,
     & 5,n)*x3y1z3*20.+c(3,1,6,n)*x3y1z4*30.+c(3,2,2,n)*x3y2*2.+c(3,2,
     & 3,n)*x3y2z1*6.+c(3,2,4,n)*x3y2z2*12.+c(3,2,5,n)*x3y2z3*20.+c(3,
     & 2,6,n)*x3y2z4*30.+c(3,3,2,n)*x3y3*2.+c(3,3,3,n)*x3y3z1*6.+c(3,
     & 3,4,n)*x3y3z2*12.+c(3,3,5,n)*x3y3z3*20.+c(3,3,6,n)*x3y3z4*30.+
     & c(3,4,2,n)*x3y4*2.+c(3,4,3,n)*x3y4z1*6.+c(3,4,4,n)*x3y4z2*12.+
     & c(3,4,5,n)*x3y4z3*20.+c(3,4,6,n)*x3y4z4*30.+c(3,5,2,n)*x3y5*2.+
     & c(3,5,3,n)*x3y5z1*6.+c(3,5,4,n)*x3y5z2*12.+c(3,5,5,n)*x3y5z3*
     & 20.+c(3,5,6,n)*x3y5z4*30.+c(3,6,2,n)*x3y6*2.+c(3,6,3,n)*x3y6z1*
     & 6.+c(3,6,4,n)*x3y6z2*12.+c(3,6,5,n)*x3y6z3*20.+c(3,6,6,n)*
     & x3y6z4*30.+c(4,0,2,n)*x4*2.+c(4,0,3,n)*x4z1*6.+c(4,0,4,n)*x4z2*
     & 12.+c(4,0,5,n)*x4z3*20.+c(4,0,6,n)*x4z4*30.+c(4,1,2,n)*x4y1*2.+
     & c(4,1,3,n)*x4y1z1*6.+c(4,1,4,n)*x4y1z2*12.+c(4,1,5,n)*x4y1z3*
     & 20.+c(4,1,6,n)*x4y1z4*30.+c(4,2,2,n)*x4y2*2.+c(4,2,3,n)*x4y2z1*
     & 6.+c(4,2,4,n)*x4y2z2*12.+c(4,2,5,n)*x4y2z3*20.+c(4,2,6,n)*
     & x4y2z4*30.+c(4,3,2,n)*x4y3*2.+c(4,3,3,n)*x4y3z1*6.+c(4,3,4,n)*
     & x4y3z2*12.+c(4,3,5,n)*x4y3z3*20.+c(4,3,6,n)*x4y3z4*30.+c(4,4,2,
     & n)*x4y4*2.+c(4,4,3,n)*x4y4z1*6.+c(4,4,4,n)*x4y4z2*12.+c(4,4,5,
     & n)*x4y4z3*20.+c(4,4,6,n)*x4y4z4*30.+c(4,5,2,n)*x4y5*2.+c(4,5,3,
     & n)*x4y5z1*6.+c(4,5,4,n)*x4y5z2*12.+c(4,5,5,n)*x4y5z3*20.+c(4,5,
     & 6,n)*x4y5z4*30.+c(4,6,2,n)*x4y6*2.+c(4,6,3,n)*x4y6z1*6.+c(4,6,
     & 4,n)*x4y6z2*12.+c(4,6,5,n)*x4y6z3*20.+c(4,6,6,n)*x4y6z4*30.+c(
     & 5,0,2,n)*x5*2.+c(5,0,3,n)*x5z1*6.+c(5,0,4,n)*x5z2*12.+c(5,0,5,
     & n)*x5z3*20.+c(5,0,6,n)*x5z4*30.+c(5,1,2,n)*x5y1*2.+c(5,1,3,n)*
     & x5y1z1*6.+c(5,1,4,n)*x5y1z2*12.+c(5,1,5,n)*x5y1z3*20.+c(5,1,6,
     & n)*x5y1z4*30.+c(5,2,2,n)*x5y2*2.+c(5,2,3,n)*x5y2z1*6.+c(5,2,4,
     & n)*x5y2z2*12.+c(5,2,5,n)*x5y2z3*20.+c(5,2,6,n)*x5y2z4*30.+c(5,
     & 3,2,n)*x5y3*2.+c(5,3,3,n)*x5y3z1*6.+c(5,3,4,n)*x5y3z2*12.+c(5,
     & 3,5,n)*x5y3z3*20.+c(5,3,6,n)*x5y3z4*30.+c(5,4,2,n)*x5y4*2.+c(5,
     & 4,3,n)*x5y4z1*6.+c(5,4,4,n)*x5y4z2*12.+c(5,4,5,n)*x5y4z3*20.+c(
     & 5,4,6,n)*x5y4z4*30.+c(5,5,2,n)*x5y5*2.+c(5,5,3,n)*x5y5z1*6.+c(
     & 5,5,4,n)*x5y5z2*12.+c(5,5,5,n)*x5y5z3*20.+c(5,5,6,n)*x5y5z4*
     & 30.+c(5,6,2,n)*x5y6*2.+c(5,6,3,n)*x5y6z1*6.+c(5,6,4,n)*x5y6z2*
     & 12.+c(5,6,5,n)*x5y6z3*20.+c(5,6,6,n)*x5y6z4*30.+c(6,0,2,n)*x6*
     & 2.+c(6,0,3,n)*x6z1*6.+c(6,0,4,n)*x6z2*12.+c(6,0,5,n)*x6z3*20.+
     & c(6,0,6,n)*x6z4*30.+c(6,1,2,n)*x6y1*2.+c(6,1,3,n)*x6y1z1*6.+c(
     & 6,1,4,n)*x6y1z2*12.+c(6,1,5,n)*x6y1z3*20.+c(6,1,6,n)*x6y1z4*
     & 30.+c(6,2,2,n)*x6y2*2.+c(6,2,3,n)*x6y2z1*6.+c(6,2,4,n)*x6y2z2*
     & 12.+c(6,2,5,n)*x6y2z3*20.+c(6,2,6,n)*x6y2z4*30.+c(6,3,2,n)*
     & x6y3*2.+c(6,3,3,n)*x6y3z1*6.+c(6,3,4,n)*x6y3z2*12.+c(6,3,5,n)*
     & x6y3z3*20.+c(6,3,6,n)*x6y3z4*30.+c(6,4,2,n)*x6y4*2.+c(6,4,3,n)*
     & x6y4z1*6.+c(6,4,4,n)*x6y4z2*12.+c(6,4,5,n)*x6y4z3*20.)
       r=(r+c(6,4,6,n)*x6y4z4*30.+c(6,5,2,n)*x6y5*2.+c(6,5,3,n)*x6y5z1*
     & 6.+c(6,5,4,n)*x6y5z2*12.+c(6,5,5,n)*x6y5z3*20.+c(6,5,6,n)*
     & x6y5z4*30.+c(6,6,2,n)*x6y6*2.+c(6,6,3,n)*x6y6z1*6.+c(6,6,4,n)*
     & x6y6z2*12.+c(6,6,5,n)*x6y6z3*20.+c(6,6,6,n)*x6y6z4*30.)*time
! endLoops()
      else if( dx.eq.0.and.dy.eq.1.and.dz.eq.0.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
      z6=z5*z1
c 
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y1z5=y1z4*z1
      y1z6=y1z5*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
      y2z5=y1z5*y1
      y2z6=y1z6*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      y3z4=y2z4*y1
      y3z5=y2z5*y1
      y3z6=y2z6*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y4z2=y3z2*y1
      y4z3=y3z3*y1
      y4z4=y3z4*y1
      y4z5=y3z5*y1
      y4z6=y3z6*y1
      y5=y4*y1
      y5z1=y4z1*y1
      y5z2=y4z2*y1
      y5z3=y4z3*y1
      y5z4=y4z4*y1
      y5z5=y4z5*y1
      y5z6=y4z6*y1
c 
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1z5=x1z4*z1
      x1z6=x1z5*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y1z5=x1y1z4*z1
      x1y1z6=x1y1z5*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y2z4=x1y1z4*y1
      x1y2z5=x1y1z5*y1
      x1y2z6=x1y1z6*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x1y3z4=x1y2z4*y1
      x1y3z5=x1y2z5*y1
      x1y3z6=x1y2z6*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y4z2=x1y3z2*y1
      x1y4z3=x1y3z3*y1
      x1y4z4=x1y3z4*y1
      x1y4z5=x1y3z5*y1
      x1y4z6=x1y3z6*y1
      x1y5=x1y4*y1
      x1y5z1=x1y4z1*y1
      x1y5z2=x1y4z2*y1
      x1y5z3=x1y4z3*y1
      x1y5z4=x1y4z4*y1
      x1y5z5=x1y4z5*y1
      x1y5z6=x1y4z6*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x2z5=x1z5*x1
      x2z6=x1z6*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y1z4=x1y1z4*x1
      x2y1z5=x1y1z5*x1
      x2y1z6=x1y1z6*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y2z4=x1y2z4*x1
      x2y2z5=x1y2z5*x1
      x2y2z6=x1y2z6*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y3z3=x1y3z3*x1
      x2y3z4=x1y3z4*x1
      x2y3z5=x1y3z5*x1
      x2y3z6=x1y3z6*x1
      x2y4=x1y4*x1
      x2y4z1=x1y4z1*x1
      x2y4z2=x1y4z2*x1
      x2y4z3=x1y4z3*x1
      x2y4z4=x1y4z4*x1
      x2y4z5=x1y4z5*x1
      x2y4z6=x1y4z6*x1
      x2y5=x1y5*x1
      x2y5z1=x1y5z1*x1
      x2y5z2=x1y5z2*x1
      x2y5z3=x1y5z3*x1
      x2y5z4=x1y5z4*x1
      x2y5z5=x1y5z5*x1
      x2y5z6=x1y5z6*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3z4=x2z4*x1
      x3z5=x2z5*x1
      x3z6=x2z6*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x3y1z4=x2y1z4*x1
      x3y1z5=x2y1z5*x1
      x3y1z6=x2y1z6*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y2z3=x2y2z3*x1
      x3y2z4=x2y2z4*x1
      x3y2z5=x2y2z5*x1
      x3y2z6=x2y2z6*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x3y3z2=x2y3z2*x1
      x3y3z3=x2y3z3*x1
      x3y3z4=x2y3z4*x1
      x3y3z5=x2y3z5*x1
      x3y3z6=x2y3z6*x1
      x3y4=x2y4*x1
      x3y4z1=x2y4z1*x1
      x3y4z2=x2y4z2*x1
      x3y4z3=x2y4z3*x1
      x3y4z4=x2y4z4*x1
      x3y4z5=x2y4z5*x1
      x3y4z6=x2y4z6*x1
      x3y5=x2y5*x1
      x3y5z1=x2y5z1*x1
      x3y5z2=x2y5z2*x1
      x3y5z3=x2y5z3*x1
      x3y5z4=x2y5z4*x1
      x3y5z5=x2y5z5*x1
      x3y5z6=x2y5z6*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4z2=x3z2*x1
      x4z3=x3z3*x1
      x4z4=x3z4*x1
      x4z5=x3z5*x1
      x4z6=x3z6*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y1z2=x3y1z2*x1
      x4y1z3=x3y1z3*x1
      x4y1z4=x3y1z4*x1
      x4y1z5=x3y1z5*x1
      x4y1z6=x3y1z6*x1
      x4y2=x3y2*x1
      x4y2z1=x3y2z1*x1
      x4y2z2=x3y2z2*x1
      x4y2z3=x3y2z3*x1
      x4y2z4=x3y2z4*x1
      x4y2z5=x3y2z5*x1
      x4y2z6=x3y2z6*x1
      x4y3=x3y3*x1
      x4y3z1=x3y3z1*x1
      x4y3z2=x3y3z2*x1
      x4y3z3=x3y3z3*x1
      x4y3z4=x3y3z4*x1
      x4y3z5=x3y3z5*x1
      x4y3z6=x3y3z6*x1
      x4y4=x3y4*x1
      x4y4z1=x3y4z1*x1
      x4y4z2=x3y4z2*x1
      x4y4z3=x3y4z3*x1
      x4y4z4=x3y4z4*x1
      x4y4z5=x3y4z5*x1
      x4y4z6=x3y4z6*x1
      x4y5=x3y5*x1
      x4y5z1=x3y5z1*x1
      x4y5z2=x3y5z2*x1
      x4y5z3=x3y5z3*x1
      x4y5z4=x3y5z4*x1
      x4y5z5=x3y5z5*x1
      x4y5z6=x3y5z6*x1
      x5=x4*x1
      x5z1=x4z1*x1
      x5z2=x4z2*x1
      x5z3=x4z3*x1
      x5z4=x4z4*x1
      x5z5=x4z5*x1
      x5z6=x4z6*x1
      x5y1=x4y1*x1
      x5y1z1=x4y1z1*x1
      x5y1z2=x4y1z2*x1
      x5y1z3=x4y1z3*x1
      x5y1z4=x4y1z4*x1
      x5y1z5=x4y1z5*x1
      x5y1z6=x4y1z6*x1
      x5y2=x4y2*x1
      x5y2z1=x4y2z1*x1
      x5y2z2=x4y2z2*x1
      x5y2z3=x4y2z3*x1
      x5y2z4=x4y2z4*x1
      x5y2z5=x4y2z5*x1
      x5y2z6=x4y2z6*x1
      x5y3=x4y3*x1
      x5y3z1=x4y3z1*x1
      x5y3z2=x4y3z2*x1
      x5y3z3=x4y3z3*x1
      x5y3z4=x4y3z4*x1
      x5y3z5=x4y3z5*x1
      x5y3z6=x4y3z6*x1
      x5y4=x4y4*x1
      x5y4z1=x4y4z1*x1
      x5y4z2=x4y4z2*x1
      x5y4z3=x4y4z3*x1
      x5y4z4=x4y4z4*x1
      x5y4z5=x4y4z5*x1
      x5y4z6=x4y4z6*x1
      x5y5=x4y5*x1
      x5y5z1=x4y5z1*x1
      x5y5z2=x4y5z2*x1
      x5y5z3=x4y5z3*x1
      x5y5z4=x4y5z4*x1
      x5y5z5=x4y5z5*x1
      x5y5z6=x4y5z6*x1
      x6=x5*x1
      x6z1=x5z1*x1
      x6z2=x5z2*x1
      x6z3=x5z3*x1
      x6z4=x5z4*x1
      x6z5=x5z5*x1
      x6z6=x5z6*x1
      x6y1=x5y1*x1
      x6y1z1=x5y1z1*x1
      x6y1z2=x5y1z2*x1
      x6y1z3=x5y1z3*x1
      x6y1z4=x5y1z4*x1
      x6y1z5=x5y1z5*x1
      x6y1z6=x5y1z6*x1
      x6y2=x5y2*x1
      x6y2z1=x5y2z1*x1
      x6y2z2=x5y2z2*x1
      x6y2z3=x5y2z3*x1
      x6y2z4=x5y2z4*x1
      x6y2z5=x5y2z5*x1
      x6y2z6=x5y2z6*x1
      x6y3=x5y3*x1
      x6y3z1=x5y3z1*x1
      x6y3z2=x5y3z2*x1
      x6y3z3=x5y3z3*x1
      x6y3z4=x5y3z4*x1
      x6y3z5=x5y3z5*x1
      x6y3z6=x5y3z6*x1
      x6y4=x5y4*x1
      x6y4z1=x5y4z1*x1
      x6y4z2=x5y4z2*x1
      x6y4z3=x5y4z3*x1
      x6y4z4=x5y4z4*x1
      x6y4z5=x5y4z5*x1
      x6y4z6=x5y4z6*x1
      x6y5=x5y5*x1
      x6y5z1=x5y5z1*x1
      x6y5z2=x5y5z2*x1
      x6y5z3=x5y5z3*x1
      x6y5z4=x5y5z4*x1
      x6y5z5=x5y5z5*x1
      x6y5z6=x5y5z6*x1
      r=(c(0,1,0,n)+c(0,1,1,n)*z1+c(0,1,2,n)*z2+c(0,1,3,n)*z3+c(0,1,4,
     & n)*z4+c(0,1,5,n)*z5+c(0,1,6,n)*z6+c(0,2,0,n)*y1*2.+c(0,2,1,n)*
     & y1z1*2.+c(0,2,2,n)*y1z2*2.+c(0,2,3,n)*y1z3*2.+c(0,2,4,n)*y1z4*
     & 2.+c(0,2,5,n)*y1z5*2.+c(0,2,6,n)*y1z6*2.+c(0,3,0,n)*y2*3.+c(0,
     & 3,1,n)*y2z1*3.+c(0,3,2,n)*y2z2*3.+c(0,3,3,n)*y2z3*3.+c(0,3,4,n)
     & *y2z4*3.+c(0,3,5,n)*y2z5*3.+c(0,3,6,n)*y2z6*3.+c(0,4,0,n)*y3*
     & 4.+c(0,4,1,n)*y3z1*4.+c(0,4,2,n)*y3z2*4.+c(0,4,3,n)*y3z3*4.+c(
     & 0,4,4,n)*y3z4*4.+c(0,4,5,n)*y3z5*4.+c(0,4,6,n)*y3z6*4.+c(0,5,0,
     & n)*y4*5.+c(0,5,1,n)*y4z1*5.+c(0,5,2,n)*y4z2*5.+c(0,5,3,n)*y4z3*
     & 5.+c(0,5,4,n)*y4z4*5.+c(0,5,5,n)*y4z5*5.+c(0,5,6,n)*y4z6*5.+c(
     & 0,6,0,n)*y5*6.+c(0,6,1,n)*y5z1*6.+c(0,6,2,n)*y5z2*6.+c(0,6,3,n)
     & *y5z3*6.+c(0,6,4,n)*y5z4*6.+c(0,6,5,n)*y5z5*6.+c(0,6,6,n)*y5z6*
     & 6.+c(1,1,0,n)*x1+c(1,1,1,n)*x1z1+c(1,1,2,n)*x1z2+c(1,1,3,n)*
     & x1z3+c(1,1,4,n)*x1z4+c(1,1,5,n)*x1z5+c(1,1,6,n)*x1z6+c(1,2,0,n)
     & *x1y1*2.+c(1,2,1,n)*x1y1z1*2.+c(1,2,2,n)*x1y1z2*2.+c(1,2,3,n)*
     & x1y1z3*2.+c(1,2,4,n)*x1y1z4*2.+c(1,2,5,n)*x1y1z5*2.+c(1,2,6,n)*
     & x1y1z6*2.+c(1,3,0,n)*x1y2*3.+c(1,3,1,n)*x1y2z1*3.+c(1,3,2,n)*
     & x1y2z2*3.+c(1,3,3,n)*x1y2z3*3.+c(1,3,4,n)*x1y2z4*3.+c(1,3,5,n)*
     & x1y2z5*3.+c(1,3,6,n)*x1y2z6*3.+c(1,4,0,n)*x1y3*4.+c(1,4,1,n)*
     & x1y3z1*4.+c(1,4,2,n)*x1y3z2*4.+c(1,4,3,n)*x1y3z3*4.+c(1,4,4,n)*
     & x1y3z4*4.+c(1,4,5,n)*x1y3z5*4.+c(1,4,6,n)*x1y3z6*4.+c(1,5,0,n)*
     & x1y4*5.+c(1,5,1,n)*x1y4z1*5.+c(1,5,2,n)*x1y4z2*5.+c(1,5,3,n)*
     & x1y4z3*5.+c(1,5,4,n)*x1y4z4*5.+c(1,5,5,n)*x1y4z5*5.+c(1,5,6,n)*
     & x1y4z6*5.+c(1,6,0,n)*x1y5*6.+c(1,6,1,n)*x1y5z1*6.+c(1,6,2,n)*
     & x1y5z2*6.+c(1,6,3,n)*x1y5z3*6.+c(1,6,4,n)*x1y5z4*6.+c(1,6,5,n)*
     & x1y5z5*6.+c(1,6,6,n)*x1y5z6*6.+c(2,1,0,n)*x2+c(2,1,1,n)*x2z1+c(
     & 2,1,2,n)*x2z2+c(2,1,3,n)*x2z3+c(2,1,4,n)*x2z4+c(2,1,5,n)*x2z5+
     & c(2,1,6,n)*x2z6+c(2,2,0,n)*x2y1*2.+c(2,2,1,n)*x2y1z1*2.+c(2,2,
     & 2,n)*x2y1z2*2.+c(2,2,3,n)*x2y1z3*2.+c(2,2,4,n)*x2y1z4*2.+c(2,2,
     & 5,n)*x2y1z5*2.+c(2,2,6,n)*x2y1z6*2.+c(2,3,0,n)*x2y2*3.+c(2,3,1,
     & n)*x2y2z1*3.+c(2,3,2,n)*x2y2z2*3.+c(2,3,3,n)*x2y2z3*3.+c(2,3,4,
     & n)*x2y2z4*3.+c(2,3,5,n)*x2y2z5*3.+c(2,3,6,n)*x2y2z6*3.+c(2,4,0,
     & n)*x2y3*4.+c(2,4,1,n)*x2y3z1*4.+c(2,4,2,n)*x2y3z2*4.+c(2,4,3,n)
     & *x2y3z3*4.+c(2,4,4,n)*x2y3z4*4.+c(2,4,5,n)*x2y3z5*4.+c(2,4,6,n)
     & *x2y3z6*4.+c(2,5,0,n)*x2y4*5.+c(2,5,1,n)*x2y4z1*5.+c(2,5,2,n)*
     & x2y4z2*5.+c(2,5,3,n)*x2y4z3*5.+c(2,5,4,n)*x2y4z4*5.+c(2,5,5,n)*
     & x2y4z5*5.+c(2,5,6,n)*x2y4z6*5.+c(2,6,0,n)*x2y5*6.+c(2,6,1,n)*
     & x2y5z1*6.+c(2,6,2,n)*x2y5z2*6.+c(2,6,3,n)*x2y5z3*6.+c(2,6,4,n)*
     & x2y5z4*6.+c(2,6,5,n)*x2y5z5*6.+c(2,6,6,n)*x2y5z6*6.+c(3,1,0,n)*
     & x3+c(3,1,1,n)*x3z1+c(3,1,2,n)*x3z2+c(3,1,3,n)*x3z3+c(3,1,4,n)*
     & x3z4+c(3,1,5,n)*x3z5+c(3,1,6,n)*x3z6+c(3,2,0,n)*x3y1*2.+c(3,2,
     & 1,n)*x3y1z1*2.+c(3,2,2,n)*x3y1z2*2.+c(3,2,3,n)*x3y1z3*2.+c(3,2,
     & 4,n)*x3y1z4*2.+c(3,2,5,n)*x3y1z5*2.+c(3,2,6,n)*x3y1z6*2.+c(3,3,
     & 0,n)*x3y2*3.+c(3,3,1,n)*x3y2z1*3.+c(3,3,2,n)*x3y2z2*3.+c(3,3,3,
     & n)*x3y2z3*3.+c(3,3,4,n)*x3y2z4*3.+c(3,3,5,n)*x3y2z5*3.+c(3,3,6,
     & n)*x3y2z6*3.+c(3,4,0,n)*x3y3*4.+c(3,4,1,n)*x3y3z1*4.+c(3,4,2,n)
     & *x3y3z2*4.+c(3,4,3,n)*x3y3z3*4.+c(3,4,4,n)*x3y3z4*4.+c(3,4,5,n)
     & *x3y3z5*4.+c(3,4,6,n)*x3y3z6*4.+c(3,5,0,n)*x3y4*5.+c(3,5,1,n)*
     & x3y4z1*5.+c(3,5,2,n)*x3y4z2*5.+c(3,5,3,n)*x3y4z3*5.+c(3,5,4,n)*
     & x3y4z4*5.+c(3,5,5,n)*x3y4z5*5.+c(3,5,6,n)*x3y4z6*5.+c(3,6,0,n)*
     & x3y5*6.+c(3,6,1,n)*x3y5z1*6.+c(3,6,2,n)*x3y5z2*6.+c(3,6,3,n)*
     & x3y5z3*6.+c(3,6,4,n)*x3y5z4*6.+c(3,6,5,n)*x3y5z5*6.+c(3,6,6,n)*
     & x3y5z6*6.+c(4,1,0,n)*x4+c(4,1,1,n)*x4z1+c(4,1,2,n)*x4z2+c(4,1,
     & 3,n)*x4z3+c(4,1,4,n)*x4z4+c(4,1,5,n)*x4z5+c(4,1,6,n)*x4z6+c(4,
     & 2,0,n)*x4y1*2.+c(4,2,1,n)*x4y1z1*2.+c(4,2,2,n)*x4y1z2*2.+c(4,2,
     & 3,n)*x4y1z3*2.+c(4,2,4,n)*x4y1z4*2.+c(4,2,5,n)*x4y1z5*2.+c(4,2,
     & 6,n)*x4y1z6*2.+c(4,3,0,n)*x4y2*3.+c(4,3,1,n)*x4y2z1*3.+c(4,3,2,
     & n)*x4y2z2*3.+c(4,3,3,n)*x4y2z3*3.+c(4,3,4,n)*x4y2z4*3.+c(4,3,5,
     & n)*x4y2z5*3.+c(4,3,6,n)*x4y2z6*3.+c(4,4,0,n)*x4y3*4.+c(4,4,1,n)
     & *x4y3z1*4.+c(4,4,2,n)*x4y3z2*4.+c(4,4,3,n)*x4y3z3*4.+c(4,4,4,n)
     & *x4y3z4*4.+c(4,4,5,n)*x4y3z5*4.+c(4,4,6,n)*x4y3z6*4.+c(4,5,0,n)
     & *x4y4*5.+c(4,5,1,n)*x4y4z1*5.+c(4,5,2,n)*x4y4z2*5.+c(4,5,3,n)*
     & x4y4z3*5.+c(4,5,4,n)*x4y4z4*5.+c(4,5,5,n)*x4y4z5*5.+c(4,5,6,n)*
     & x4y4z6*5.+c(4,6,0,n)*x4y5*6.+c(4,6,1,n)*x4y5z1*6.+c(4,6,2,n)*
     & x4y5z2*6.+c(4,6,3,n)*x4y5z3*6.+c(4,6,4,n)*x4y5z4*6.+c(4,6,5,n)*
     & x4y5z5*6.+c(4,6,6,n)*x4y5z6*6.+c(5,1,0,n)*x5+c(5,1,1,n)*x5z1+c(
     & 5,1,2,n)*x5z2+c(5,1,3,n)*x5z3+c(5,1,4,n)*x5z4+c(5,1,5,n)*x5z5+
     & c(5,1,6,n)*x5z6+c(5,2,0,n)*x5y1*2.+c(5,2,1,n)*x5y1z1*2.+c(5,2,
     & 2,n)*x5y1z2*2.+c(5,2,3,n)*x5y1z3*2.+c(5,2,4,n)*x5y1z4*2.+c(5,2,
     & 5,n)*x5y1z5*2.+c(5,2,6,n)*x5y1z6*2.+c(5,3,0,n)*x5y2*3.+c(5,3,1,
     & n)*x5y2z1*3.+c(5,3,2,n)*x5y2z2*3.+c(5,3,3,n)*x5y2z3*3.+c(5,3,4,
     & n)*x5y2z4*3.+c(5,3,5,n)*x5y2z5*3.+c(5,3,6,n)*x5y2z6*3.+c(5,4,0,
     & n)*x5y3*4.+c(5,4,1,n)*x5y3z1*4.+c(5,4,2,n)*x5y3z2*4.+c(5,4,3,n)
     & *x5y3z3*4.+c(5,4,4,n)*x5y3z4*4.+c(5,4,5,n)*x5y3z5*4.+c(5,4,6,n)
     & *x5y3z6*4.+c(5,5,0,n)*x5y4*5.+c(5,5,1,n)*x5y4z1*5.+c(5,5,2,n)*
     & x5y4z2*5.+c(5,5,3,n)*x5y4z3*5.+c(5,5,4,n)*x5y4z4*5.+c(5,5,5,n)*
     & x5y4z5*5.+c(5,5,6,n)*x5y4z6*5.+c(5,6,0,n)*x5y5*6.+c(5,6,1,n)*
     & x5y5z1*6.)
       r=(r+c(5,6,2,n)*x5y5z2*6.+c(5,6,3,n)*x5y5z3*6.+c(5,6,4,n)*
     & x5y5z4*6.+c(5,6,5,n)*x5y5z5*6.+c(5,6,6,n)*x5y5z6*6.+c(6,1,0,n)*
     & x6+c(6,1,1,n)*x6z1+c(6,1,2,n)*x6z2+c(6,1,3,n)*x6z3+c(6,1,4,n)*
     & x6z4+c(6,1,5,n)*x6z5+c(6,1,6,n)*x6z6+c(6,2,0,n)*x6y1*2.+c(6,2,
     & 1,n)*x6y1z1*2.+c(6,2,2,n)*x6y1z2*2.+c(6,2,3,n)*x6y1z3*2.+c(6,2,
     & 4,n)*x6y1z4*2.+c(6,2,5,n)*x6y1z5*2.+c(6,2,6,n)*x6y1z6*2.+c(6,3,
     & 0,n)*x6y2*3.+c(6,3,1,n)*x6y2z1*3.+c(6,3,2,n)*x6y2z2*3.+c(6,3,3,
     & n)*x6y2z3*3.+c(6,3,4,n)*x6y2z4*3.+c(6,3,5,n)*x6y2z5*3.+c(6,3,6,
     & n)*x6y2z6*3.+c(6,4,0,n)*x6y3*4.+c(6,4,1,n)*x6y3z1*4.+c(6,4,2,n)
     & *x6y3z2*4.+c(6,4,3,n)*x6y3z3*4.+c(6,4,4,n)*x6y3z4*4.+c(6,4,5,n)
     & *x6y3z5*4.+c(6,4,6,n)*x6y3z6*4.+c(6,5,0,n)*x6y4*5.+c(6,5,1,n)*
     & x6y4z1*5.+c(6,5,2,n)*x6y4z2*5.+c(6,5,3,n)*x6y4z3*5.+c(6,5,4,n)*
     & x6y4z4*5.+c(6,5,5,n)*x6y4z5*5.+c(6,5,6,n)*x6y4z6*5.+c(6,6,0,n)*
     & x6y5*6.+c(6,6,1,n)*x6y5z1*6.+c(6,6,2,n)*x6y5z2*6.+c(6,6,3,n)*
     & x6y5z3*6.+c(6,6,4,n)*x6y5z4*6.+c(6,6,5,n)*x6y5z5*6.+c(6,6,6,n)*
     & x6y5z6*6.)*time
! endLoops()
      else if( dx.eq.0.and.dy.eq.1.and.dz.eq.1.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
c 
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
c 
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
      x6=x5*x1
      x6z1=x5z1*x1
      x6z2=x5z2*x1
      x6z3=x5z3*x1
      x6z4=x5z4*x1
      x6z5=x5z5*x1
      x6y1=x5y1*x1
      x6y1z1=x5y1z1*x1
      x6y1z2=x5y1z2*x1
      x6y1z3=x5y1z3*x1
      x6y1z4=x5y1z4*x1
      x6y1z5=x5y1z5*x1
      x6y2=x5y2*x1
      x6y2z1=x5y2z1*x1
      x6y2z2=x5y2z2*x1
      x6y2z3=x5y2z3*x1
      x6y2z4=x5y2z4*x1
      x6y2z5=x5y2z5*x1
      x6y3=x5y3*x1
      x6y3z1=x5y3z1*x1
      x6y3z2=x5y3z2*x1
      x6y3z3=x5y3z3*x1
      x6y3z4=x5y3z4*x1
      x6y3z5=x5y3z5*x1
      x6y4=x5y4*x1
      x6y4z1=x5y4z1*x1
      x6y4z2=x5y4z2*x1
      x6y4z3=x5y4z3*x1
      x6y4z4=x5y4z4*x1
      x6y4z5=x5y4z5*x1
      x6y5=x5y5*x1
      x6y5z1=x5y5z1*x1
      x6y5z2=x5y5z2*x1
      x6y5z3=x5y5z3*x1
      x6y5z4=x5y5z4*x1
      x6y5z5=x5y5z5*x1
      r=(c(0,1,1,n)+c(0,1,2,n)*z1*2.+c(0,1,3,n)*z2*3.+c(0,1,4,n)*z3*4.+
     & c(0,1,5,n)*z4*5.+c(0,1,6,n)*z5*6.+c(0,2,1,n)*y1*2.+c(0,2,2,n)*
     & y1z1*4.+c(0,2,3,n)*y1z2*6.+c(0,2,4,n)*y1z3*8.+c(0,2,5,n)*y1z4*
     & 10.+c(0,2,6,n)*y1z5*12.+c(0,3,1,n)*y2*3.+c(0,3,2,n)*y2z1*6.+c(
     & 0,3,3,n)*y2z2*9.+c(0,3,4,n)*y2z3*12.+c(0,3,5,n)*y2z4*15.+c(0,3,
     & 6,n)*y2z5*18.+c(0,4,1,n)*y3*4.+c(0,4,2,n)*y3z1*8.+c(0,4,3,n)*
     & y3z2*12.+c(0,4,4,n)*y3z3*16.+c(0,4,5,n)*y3z4*20.+c(0,4,6,n)*
     & y3z5*24.+c(0,5,1,n)*y4*5.+c(0,5,2,n)*y4z1*10.+c(0,5,3,n)*y4z2*
     & 15.+c(0,5,4,n)*y4z3*20.+c(0,5,5,n)*y4z4*25.+c(0,5,6,n)*y4z5*
     & 30.+c(0,6,1,n)*y5*6.+c(0,6,2,n)*y5z1*12.+c(0,6,3,n)*y5z2*18.+c(
     & 0,6,4,n)*y5z3*24.+c(0,6,5,n)*y5z4*30.+c(0,6,6,n)*y5z5*36.+c(1,
     & 1,1,n)*x1+c(1,1,2,n)*x1z1*2.+c(1,1,3,n)*x1z2*3.+c(1,1,4,n)*
     & x1z3*4.+c(1,1,5,n)*x1z4*5.+c(1,1,6,n)*x1z5*6.+c(1,2,1,n)*x1y1*
     & 2.+c(1,2,2,n)*x1y1z1*4.+c(1,2,3,n)*x1y1z2*6.+c(1,2,4,n)*x1y1z3*
     & 8.+c(1,2,5,n)*x1y1z4*10.+c(1,2,6,n)*x1y1z5*12.+c(1,3,1,n)*x1y2*
     & 3.+c(1,3,2,n)*x1y2z1*6.+c(1,3,3,n)*x1y2z2*9.+c(1,3,4,n)*x1y2z3*
     & 12.+c(1,3,5,n)*x1y2z4*15.+c(1,3,6,n)*x1y2z5*18.+c(1,4,1,n)*
     & x1y3*4.+c(1,4,2,n)*x1y3z1*8.+c(1,4,3,n)*x1y3z2*12.+c(1,4,4,n)*
     & x1y3z3*16.+c(1,4,5,n)*x1y3z4*20.+c(1,4,6,n)*x1y3z5*24.+c(1,5,1,
     & n)*x1y4*5.+c(1,5,2,n)*x1y4z1*10.+c(1,5,3,n)*x1y4z2*15.+c(1,5,4,
     & n)*x1y4z3*20.+c(1,5,5,n)*x1y4z4*25.+c(1,5,6,n)*x1y4z5*30.+c(1,
     & 6,1,n)*x1y5*6.+c(1,6,2,n)*x1y5z1*12.+c(1,6,3,n)*x1y5z2*18.+c(1,
     & 6,4,n)*x1y5z3*24.+c(1,6,5,n)*x1y5z4*30.+c(1,6,6,n)*x1y5z5*36.+
     & c(2,1,1,n)*x2+c(2,1,2,n)*x2z1*2.+c(2,1,3,n)*x2z2*3.+c(2,1,4,n)*
     & x2z3*4.+c(2,1,5,n)*x2z4*5.+c(2,1,6,n)*x2z5*6.+c(2,2,1,n)*x2y1*
     & 2.+c(2,2,2,n)*x2y1z1*4.+c(2,2,3,n)*x2y1z2*6.+c(2,2,4,n)*x2y1z3*
     & 8.+c(2,2,5,n)*x2y1z4*10.+c(2,2,6,n)*x2y1z5*12.+c(2,3,1,n)*x2y2*
     & 3.+c(2,3,2,n)*x2y2z1*6.+c(2,3,3,n)*x2y2z2*9.+c(2,3,4,n)*x2y2z3*
     & 12.+c(2,3,5,n)*x2y2z4*15.+c(2,3,6,n)*x2y2z5*18.+c(2,4,1,n)*
     & x2y3*4.+c(2,4,2,n)*x2y3z1*8.+c(2,4,3,n)*x2y3z2*12.+c(2,4,4,n)*
     & x2y3z3*16.+c(2,4,5,n)*x2y3z4*20.+c(2,4,6,n)*x2y3z5*24.+c(2,5,1,
     & n)*x2y4*5.+c(2,5,2,n)*x2y4z1*10.+c(2,5,3,n)*x2y4z2*15.+c(2,5,4,
     & n)*x2y4z3*20.+c(2,5,5,n)*x2y4z4*25.+c(2,5,6,n)*x2y4z5*30.+c(2,
     & 6,1,n)*x2y5*6.+c(2,6,2,n)*x2y5z1*12.+c(2,6,3,n)*x2y5z2*18.+c(2,
     & 6,4,n)*x2y5z3*24.+c(2,6,5,n)*x2y5z4*30.+c(2,6,6,n)*x2y5z5*36.+
     & c(3,1,1,n)*x3+c(3,1,2,n)*x3z1*2.+c(3,1,3,n)*x3z2*3.+c(3,1,4,n)*
     & x3z3*4.+c(3,1,5,n)*x3z4*5.+c(3,1,6,n)*x3z5*6.+c(3,2,1,n)*x3y1*
     & 2.+c(3,2,2,n)*x3y1z1*4.+c(3,2,3,n)*x3y1z2*6.+c(3,2,4,n)*x3y1z3*
     & 8.+c(3,2,5,n)*x3y1z4*10.+c(3,2,6,n)*x3y1z5*12.+c(3,3,1,n)*x3y2*
     & 3.+c(3,3,2,n)*x3y2z1*6.+c(3,3,3,n)*x3y2z2*9.+c(3,3,4,n)*x3y2z3*
     & 12.+c(3,3,5,n)*x3y2z4*15.+c(3,3,6,n)*x3y2z5*18.+c(3,4,1,n)*
     & x3y3*4.+c(3,4,2,n)*x3y3z1*8.+c(3,4,3,n)*x3y3z2*12.+c(3,4,4,n)*
     & x3y3z3*16.+c(3,4,5,n)*x3y3z4*20.+c(3,4,6,n)*x3y3z5*24.+c(3,5,1,
     & n)*x3y4*5.+c(3,5,2,n)*x3y4z1*10.+c(3,5,3,n)*x3y4z2*15.+c(3,5,4,
     & n)*x3y4z3*20.+c(3,5,5,n)*x3y4z4*25.+c(3,5,6,n)*x3y4z5*30.+c(3,
     & 6,1,n)*x3y5*6.+c(3,6,2,n)*x3y5z1*12.+c(3,6,3,n)*x3y5z2*18.+c(3,
     & 6,4,n)*x3y5z3*24.+c(3,6,5,n)*x3y5z4*30.+c(3,6,6,n)*x3y5z5*36.+
     & c(4,1,1,n)*x4+c(4,1,2,n)*x4z1*2.+c(4,1,3,n)*x4z2*3.+c(4,1,4,n)*
     & x4z3*4.+c(4,1,5,n)*x4z4*5.+c(4,1,6,n)*x4z5*6.+c(4,2,1,n)*x4y1*
     & 2.+c(4,2,2,n)*x4y1z1*4.+c(4,2,3,n)*x4y1z2*6.+c(4,2,4,n)*x4y1z3*
     & 8.+c(4,2,5,n)*x4y1z4*10.+c(4,2,6,n)*x4y1z5*12.+c(4,3,1,n)*x4y2*
     & 3.+c(4,3,2,n)*x4y2z1*6.+c(4,3,3,n)*x4y2z2*9.+c(4,3,4,n)*x4y2z3*
     & 12.+c(4,3,5,n)*x4y2z4*15.+c(4,3,6,n)*x4y2z5*18.+c(4,4,1,n)*
     & x4y3*4.+c(4,4,2,n)*x4y3z1*8.+c(4,4,3,n)*x4y3z2*12.+c(4,4,4,n)*
     & x4y3z3*16.+c(4,4,5,n)*x4y3z4*20.+c(4,4,6,n)*x4y3z5*24.+c(4,5,1,
     & n)*x4y4*5.+c(4,5,2,n)*x4y4z1*10.+c(4,5,3,n)*x4y4z2*15.+c(4,5,4,
     & n)*x4y4z3*20.+c(4,5,5,n)*x4y4z4*25.+c(4,5,6,n)*x4y4z5*30.+c(4,
     & 6,1,n)*x4y5*6.+c(4,6,2,n)*x4y5z1*12.+c(4,6,3,n)*x4y5z2*18.+c(4,
     & 6,4,n)*x4y5z3*24.+c(4,6,5,n)*x4y5z4*30.+c(4,6,6,n)*x4y5z5*36.+
     & c(5,1,1,n)*x5+c(5,1,2,n)*x5z1*2.+c(5,1,3,n)*x5z2*3.+c(5,1,4,n)*
     & x5z3*4.+c(5,1,5,n)*x5z4*5.+c(5,1,6,n)*x5z5*6.+c(5,2,1,n)*x5y1*
     & 2.+c(5,2,2,n)*x5y1z1*4.+c(5,2,3,n)*x5y1z2*6.+c(5,2,4,n)*x5y1z3*
     & 8.+c(5,2,5,n)*x5y1z4*10.+c(5,2,6,n)*x5y1z5*12.+c(5,3,1,n)*x5y2*
     & 3.+c(5,3,2,n)*x5y2z1*6.+c(5,3,3,n)*x5y2z2*9.+c(5,3,4,n)*x5y2z3*
     & 12.+c(5,3,5,n)*x5y2z4*15.+c(5,3,6,n)*x5y2z5*18.+c(5,4,1,n)*
     & x5y3*4.+c(5,4,2,n)*x5y3z1*8.+c(5,4,3,n)*x5y3z2*12.+c(5,4,4,n)*
     & x5y3z3*16.+c(5,4,5,n)*x5y3z4*20.+c(5,4,6,n)*x5y3z5*24.+c(5,5,1,
     & n)*x5y4*5.+c(5,5,2,n)*x5y4z1*10.+c(5,5,3,n)*x5y4z2*15.+c(5,5,4,
     & n)*x5y4z3*20.+c(5,5,5,n)*x5y4z4*25.+c(5,5,6,n)*x5y4z5*30.+c(5,
     & 6,1,n)*x5y5*6.+c(5,6,2,n)*x5y5z1*12.+c(5,6,3,n)*x5y5z2*18.+c(5,
     & 6,4,n)*x5y5z3*24.+c(5,6,5,n)*x5y5z4*30.+c(5,6,6,n)*x5y5z5*36.+
     & c(6,1,1,n)*x6+c(6,1,2,n)*x6z1*2.+c(6,1,3,n)*x6z2*3.+c(6,1,4,n)*
     & x6z3*4.+c(6,1,5,n)*x6z4*5.+c(6,1,6,n)*x6z5*6.+c(6,2,1,n)*x6y1*
     & 2.+c(6,2,2,n)*x6y1z1*4.+c(6,2,3,n)*x6y1z2*6.+c(6,2,4,n)*x6y1z3*
     & 8.+c(6,2,5,n)*x6y1z4*10.+c(6,2,6,n)*x6y1z5*12.+c(6,3,1,n)*x6y2*
     & 3.+c(6,3,2,n)*x6y2z1*6.+c(6,3,3,n)*x6y2z2*9.+c(6,3,4,n)*x6y2z3*
     & 12.+c(6,3,5,n)*x6y2z4*15.+c(6,3,6,n)*x6y2z5*18.+c(6,4,1,n)*
     & x6y3*4.+c(6,4,2,n)*x6y3z1*8.)
       r=(r+c(6,4,3,n)*x6y3z2*12.+c(6,4,4,n)*x6y3z3*16.+c(6,4,5,n)*
     & x6y3z4*20.+c(6,4,6,n)*x6y3z5*24.+c(6,5,1,n)*x6y4*5.+c(6,5,2,n)*
     & x6y4z1*10.+c(6,5,3,n)*x6y4z2*15.+c(6,5,4,n)*x6y4z3*20.+c(6,5,5,
     & n)*x6y4z4*25.+c(6,5,6,n)*x6y4z5*30.+c(6,6,1,n)*x6y5*6.+c(6,6,2,
     & n)*x6y5z1*12.+c(6,6,3,n)*x6y5z2*18.+c(6,6,4,n)*x6y5z3*24.+c(6,
     & 6,5,n)*x6y5z4*30.+c(6,6,6,n)*x6y5z5*36.)*time
! endLoops()
      else if( dx.eq.0.and.dy.eq.1.and.dz.eq.2.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
c 
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
c 
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
      x6=x5*x1
      x6z1=x5z1*x1
      x6z2=x5z2*x1
      x6z3=x5z3*x1
      x6z4=x5z4*x1
      x6y1=x5y1*x1
      x6y1z1=x5y1z1*x1
      x6y1z2=x5y1z2*x1
      x6y1z3=x5y1z3*x1
      x6y1z4=x5y1z4*x1
      x6y2=x5y2*x1
      x6y2z1=x5y2z1*x1
      x6y2z2=x5y2z2*x1
      x6y2z3=x5y2z3*x1
      x6y2z4=x5y2z4*x1
      x6y3=x5y3*x1
      x6y3z1=x5y3z1*x1
      x6y3z2=x5y3z2*x1
      x6y3z3=x5y3z3*x1
      x6y3z4=x5y3z4*x1
      x6y4=x5y4*x1
      x6y4z1=x5y4z1*x1
      x6y4z2=x5y4z2*x1
      x6y4z3=x5y4z3*x1
      x6y4z4=x5y4z4*x1
      x6y5=x5y5*x1
      x6y5z1=x5y5z1*x1
      x6y5z2=x5y5z2*x1
      x6y5z3=x5y5z3*x1
      x6y5z4=x5y5z4*x1
      r=(c(0,1,2,n)*2.+c(0,1,3,n)*z1*6.+c(0,1,4,n)*z2*12.+c(0,1,5,n)*
     & z3*20.+c(0,1,6,n)*z4*30.+c(0,2,2,n)*y1*4.+c(0,2,3,n)*y1z1*12.+
     & c(0,2,4,n)*y1z2*24.+c(0,2,5,n)*y1z3*40.+c(0,2,6,n)*y1z4*60.+c(
     & 0,3,2,n)*y2*6.+c(0,3,3,n)*y2z1*18.+c(0,3,4,n)*y2z2*36.+c(0,3,5,
     & n)*y2z3*60.+c(0,3,6,n)*y2z4*90.+c(0,4,2,n)*y3*8.+c(0,4,3,n)*
     & y3z1*24.+c(0,4,4,n)*y3z2*48.+c(0,4,5,n)*y3z3*80.+c(0,4,6,n)*
     & y3z4*120.+c(0,5,2,n)*y4*10.+c(0,5,3,n)*y4z1*30.+c(0,5,4,n)*
     & y4z2*60.+c(0,5,5,n)*y4z3*100.+c(0,5,6,n)*y4z4*150.+c(0,6,2,n)*
     & y5*12.+c(0,6,3,n)*y5z1*36.+c(0,6,4,n)*y5z2*72.+c(0,6,5,n)*y5z3*
     & 120.+c(0,6,6,n)*y5z4*180.+c(1,1,2,n)*x1*2.+c(1,1,3,n)*x1z1*6.+
     & c(1,1,4,n)*x1z2*12.+c(1,1,5,n)*x1z3*20.+c(1,1,6,n)*x1z4*30.+c(
     & 1,2,2,n)*x1y1*4.+c(1,2,3,n)*x1y1z1*12.+c(1,2,4,n)*x1y1z2*24.+c(
     & 1,2,5,n)*x1y1z3*40.+c(1,2,6,n)*x1y1z4*60.+c(1,3,2,n)*x1y2*6.+c(
     & 1,3,3,n)*x1y2z1*18.+c(1,3,4,n)*x1y2z2*36.+c(1,3,5,n)*x1y2z3*
     & 60.+c(1,3,6,n)*x1y2z4*90.+c(1,4,2,n)*x1y3*8.+c(1,4,3,n)*x1y3z1*
     & 24.+c(1,4,4,n)*x1y3z2*48.+c(1,4,5,n)*x1y3z3*80.+c(1,4,6,n)*
     & x1y3z4*120.+c(1,5,2,n)*x1y4*10.+c(1,5,3,n)*x1y4z1*30.+c(1,5,4,
     & n)*x1y4z2*60.+c(1,5,5,n)*x1y4z3*100.+c(1,5,6,n)*x1y4z4*150.+c(
     & 1,6,2,n)*x1y5*12.+c(1,6,3,n)*x1y5z1*36.+c(1,6,4,n)*x1y5z2*72.+
     & c(1,6,5,n)*x1y5z3*120.+c(1,6,6,n)*x1y5z4*180.+c(2,1,2,n)*x2*2.+
     & c(2,1,3,n)*x2z1*6.+c(2,1,4,n)*x2z2*12.+c(2,1,5,n)*x2z3*20.+c(2,
     & 1,6,n)*x2z4*30.+c(2,2,2,n)*x2y1*4.+c(2,2,3,n)*x2y1z1*12.+c(2,2,
     & 4,n)*x2y1z2*24.+c(2,2,5,n)*x2y1z3*40.+c(2,2,6,n)*x2y1z4*60.+c(
     & 2,3,2,n)*x2y2*6.+c(2,3,3,n)*x2y2z1*18.+c(2,3,4,n)*x2y2z2*36.+c(
     & 2,3,5,n)*x2y2z3*60.+c(2,3,6,n)*x2y2z4*90.+c(2,4,2,n)*x2y3*8.+c(
     & 2,4,3,n)*x2y3z1*24.+c(2,4,4,n)*x2y3z2*48.+c(2,4,5,n)*x2y3z3*
     & 80.+c(2,4,6,n)*x2y3z4*120.+c(2,5,2,n)*x2y4*10.+c(2,5,3,n)*
     & x2y4z1*30.+c(2,5,4,n)*x2y4z2*60.+c(2,5,5,n)*x2y4z3*100.+c(2,5,
     & 6,n)*x2y4z4*150.+c(2,6,2,n)*x2y5*12.+c(2,6,3,n)*x2y5z1*36.+c(2,
     & 6,4,n)*x2y5z2*72.+c(2,6,5,n)*x2y5z3*120.+c(2,6,6,n)*x2y5z4*
     & 180.+c(3,1,2,n)*x3*2.+c(3,1,3,n)*x3z1*6.+c(3,1,4,n)*x3z2*12.+c(
     & 3,1,5,n)*x3z3*20.+c(3,1,6,n)*x3z4*30.+c(3,2,2,n)*x3y1*4.+c(3,2,
     & 3,n)*x3y1z1*12.+c(3,2,4,n)*x3y1z2*24.+c(3,2,5,n)*x3y1z3*40.+c(
     & 3,2,6,n)*x3y1z4*60.+c(3,3,2,n)*x3y2*6.+c(3,3,3,n)*x3y2z1*18.+c(
     & 3,3,4,n)*x3y2z2*36.+c(3,3,5,n)*x3y2z3*60.+c(3,3,6,n)*x3y2z4*
     & 90.+c(3,4,2,n)*x3y3*8.+c(3,4,3,n)*x3y3z1*24.+c(3,4,4,n)*x3y3z2*
     & 48.+c(3,4,5,n)*x3y3z3*80.+c(3,4,6,n)*x3y3z4*120.+c(3,5,2,n)*
     & x3y4*10.+c(3,5,3,n)*x3y4z1*30.+c(3,5,4,n)*x3y4z2*60.+c(3,5,5,n)
     & *x3y4z3*100.+c(3,5,6,n)*x3y4z4*150.+c(3,6,2,n)*x3y5*12.+c(3,6,
     & 3,n)*x3y5z1*36.+c(3,6,4,n)*x3y5z2*72.+c(3,6,5,n)*x3y5z3*120.+c(
     & 3,6,6,n)*x3y5z4*180.+c(4,1,2,n)*x4*2.+c(4,1,3,n)*x4z1*6.+c(4,1,
     & 4,n)*x4z2*12.+c(4,1,5,n)*x4z3*20.+c(4,1,6,n)*x4z4*30.+c(4,2,2,
     & n)*x4y1*4.+c(4,2,3,n)*x4y1z1*12.+c(4,2,4,n)*x4y1z2*24.+c(4,2,5,
     & n)*x4y1z3*40.+c(4,2,6,n)*x4y1z4*60.+c(4,3,2,n)*x4y2*6.+c(4,3,3,
     & n)*x4y2z1*18.+c(4,3,4,n)*x4y2z2*36.+c(4,3,5,n)*x4y2z3*60.+c(4,
     & 3,6,n)*x4y2z4*90.+c(4,4,2,n)*x4y3*8.+c(4,4,3,n)*x4y3z1*24.+c(4,
     & 4,4,n)*x4y3z2*48.+c(4,4,5,n)*x4y3z3*80.+c(4,4,6,n)*x4y3z4*120.+
     & c(4,5,2,n)*x4y4*10.+c(4,5,3,n)*x4y4z1*30.+c(4,5,4,n)*x4y4z2*
     & 60.+c(4,5,5,n)*x4y4z3*100.+c(4,5,6,n)*x4y4z4*150.+c(4,6,2,n)*
     & x4y5*12.+c(4,6,3,n)*x4y5z1*36.+c(4,6,4,n)*x4y5z2*72.+c(4,6,5,n)
     & *x4y5z3*120.+c(4,6,6,n)*x4y5z4*180.+c(5,1,2,n)*x5*2.+c(5,1,3,n)
     & *x5z1*6.+c(5,1,4,n)*x5z2*12.+c(5,1,5,n)*x5z3*20.+c(5,1,6,n)*
     & x5z4*30.+c(5,2,2,n)*x5y1*4.+c(5,2,3,n)*x5y1z1*12.+c(5,2,4,n)*
     & x5y1z2*24.+c(5,2,5,n)*x5y1z3*40.+c(5,2,6,n)*x5y1z4*60.+c(5,3,2,
     & n)*x5y2*6.+c(5,3,3,n)*x5y2z1*18.+c(5,3,4,n)*x5y2z2*36.+c(5,3,5,
     & n)*x5y2z3*60.+c(5,3,6,n)*x5y2z4*90.+c(5,4,2,n)*x5y3*8.+c(5,4,3,
     & n)*x5y3z1*24.+c(5,4,4,n)*x5y3z2*48.+c(5,4,5,n)*x5y3z3*80.+c(5,
     & 4,6,n)*x5y3z4*120.+c(5,5,2,n)*x5y4*10.+c(5,5,3,n)*x5y4z1*30.+c(
     & 5,5,4,n)*x5y4z2*60.+c(5,5,5,n)*x5y4z3*100.+c(5,5,6,n)*x5y4z4*
     & 150.+c(5,6,2,n)*x5y5*12.+c(5,6,3,n)*x5y5z1*36.+c(5,6,4,n)*
     & x5y5z2*72.+c(5,6,5,n)*x5y5z3*120.+c(5,6,6,n)*x5y5z4*180.+c(6,1,
     & 2,n)*x6*2.+c(6,1,3,n)*x6z1*6.+c(6,1,4,n)*x6z2*12.+c(6,1,5,n)*
     & x6z3*20.+c(6,1,6,n)*x6z4*30.+c(6,2,2,n)*x6y1*4.+c(6,2,3,n)*
     & x6y1z1*12.+c(6,2,4,n)*x6y1z2*24.+c(6,2,5,n)*x6y1z3*40.+c(6,2,6,
     & n)*x6y1z4*60.+c(6,3,2,n)*x6y2*6.+c(6,3,3,n)*x6y2z1*18.+c(6,3,4,
     & n)*x6y2z2*36.+c(6,3,5,n)*x6y2z3*60.+c(6,3,6,n)*x6y2z4*90.+c(6,
     & 4,2,n)*x6y3*8.+c(6,4,3,n)*x6y3z1*24.+c(6,4,4,n)*x6y3z2*48.+c(6,
     & 4,5,n)*x6y3z3*80.+c(6,4,6,n)*x6y3z4*120.+c(6,5,2,n)*x6y4*10.+c(
     & 6,5,3,n)*x6y4z1*30.+c(6,5,4,n)*x6y4z2*60.+c(6,5,5,n)*x6y4z3*
     & 100.+c(6,5,6,n)*x6y4z4*150.+c(6,6,2,n)*x6y5*12.+c(6,6,3,n)*
     & x6y5z1*36.+c(6,6,4,n)*x6y5z2*72.+c(6,6,5,n)*x6y5z3*120.+c(6,6,
     & 6,n)*x6y5z4*180.)*time
! endLoops()
      else if( dx.eq.0.and.dy.eq.2.and.dz.eq.0.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
      z6=z5*z1
c 
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y1z5=y1z4*z1
      y1z6=y1z5*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
      y2z5=y1z5*y1
      y2z6=y1z6*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      y3z4=y2z4*y1
      y3z5=y2z5*y1
      y3z6=y2z6*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y4z2=y3z2*y1
      y4z3=y3z3*y1
      y4z4=y3z4*y1
      y4z5=y3z5*y1
      y4z6=y3z6*y1
c 
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1z5=x1z4*z1
      x1z6=x1z5*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y1z5=x1y1z4*z1
      x1y1z6=x1y1z5*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y2z4=x1y1z4*y1
      x1y2z5=x1y1z5*y1
      x1y2z6=x1y1z6*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x1y3z4=x1y2z4*y1
      x1y3z5=x1y2z5*y1
      x1y3z6=x1y2z6*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y4z2=x1y3z2*y1
      x1y4z3=x1y3z3*y1
      x1y4z4=x1y3z4*y1
      x1y4z5=x1y3z5*y1
      x1y4z6=x1y3z6*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x2z5=x1z5*x1
      x2z6=x1z6*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y1z4=x1y1z4*x1
      x2y1z5=x1y1z5*x1
      x2y1z6=x1y1z6*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y2z4=x1y2z4*x1
      x2y2z5=x1y2z5*x1
      x2y2z6=x1y2z6*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y3z3=x1y3z3*x1
      x2y3z4=x1y3z4*x1
      x2y3z5=x1y3z5*x1
      x2y3z6=x1y3z6*x1
      x2y4=x1y4*x1
      x2y4z1=x1y4z1*x1
      x2y4z2=x1y4z2*x1
      x2y4z3=x1y4z3*x1
      x2y4z4=x1y4z4*x1
      x2y4z5=x1y4z5*x1
      x2y4z6=x1y4z6*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3z4=x2z4*x1
      x3z5=x2z5*x1
      x3z6=x2z6*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x3y1z4=x2y1z4*x1
      x3y1z5=x2y1z5*x1
      x3y1z6=x2y1z6*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y2z3=x2y2z3*x1
      x3y2z4=x2y2z4*x1
      x3y2z5=x2y2z5*x1
      x3y2z6=x2y2z6*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x3y3z2=x2y3z2*x1
      x3y3z3=x2y3z3*x1
      x3y3z4=x2y3z4*x1
      x3y3z5=x2y3z5*x1
      x3y3z6=x2y3z6*x1
      x3y4=x2y4*x1
      x3y4z1=x2y4z1*x1
      x3y4z2=x2y4z2*x1
      x3y4z3=x2y4z3*x1
      x3y4z4=x2y4z4*x1
      x3y4z5=x2y4z5*x1
      x3y4z6=x2y4z6*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4z2=x3z2*x1
      x4z3=x3z3*x1
      x4z4=x3z4*x1
      x4z5=x3z5*x1
      x4z6=x3z6*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y1z2=x3y1z2*x1
      x4y1z3=x3y1z3*x1
      x4y1z4=x3y1z4*x1
      x4y1z5=x3y1z5*x1
      x4y1z6=x3y1z6*x1
      x4y2=x3y2*x1
      x4y2z1=x3y2z1*x1
      x4y2z2=x3y2z2*x1
      x4y2z3=x3y2z3*x1
      x4y2z4=x3y2z4*x1
      x4y2z5=x3y2z5*x1
      x4y2z6=x3y2z6*x1
      x4y3=x3y3*x1
      x4y3z1=x3y3z1*x1
      x4y3z2=x3y3z2*x1
      x4y3z3=x3y3z3*x1
      x4y3z4=x3y3z4*x1
      x4y3z5=x3y3z5*x1
      x4y3z6=x3y3z6*x1
      x4y4=x3y4*x1
      x4y4z1=x3y4z1*x1
      x4y4z2=x3y4z2*x1
      x4y4z3=x3y4z3*x1
      x4y4z4=x3y4z4*x1
      x4y4z5=x3y4z5*x1
      x4y4z6=x3y4z6*x1
      x5=x4*x1
      x5z1=x4z1*x1
      x5z2=x4z2*x1
      x5z3=x4z3*x1
      x5z4=x4z4*x1
      x5z5=x4z5*x1
      x5z6=x4z6*x1
      x5y1=x4y1*x1
      x5y1z1=x4y1z1*x1
      x5y1z2=x4y1z2*x1
      x5y1z3=x4y1z3*x1
      x5y1z4=x4y1z4*x1
      x5y1z5=x4y1z5*x1
      x5y1z6=x4y1z6*x1
      x5y2=x4y2*x1
      x5y2z1=x4y2z1*x1
      x5y2z2=x4y2z2*x1
      x5y2z3=x4y2z3*x1
      x5y2z4=x4y2z4*x1
      x5y2z5=x4y2z5*x1
      x5y2z6=x4y2z6*x1
      x5y3=x4y3*x1
      x5y3z1=x4y3z1*x1
      x5y3z2=x4y3z2*x1
      x5y3z3=x4y3z3*x1
      x5y3z4=x4y3z4*x1
      x5y3z5=x4y3z5*x1
      x5y3z6=x4y3z6*x1
      x5y4=x4y4*x1
      x5y4z1=x4y4z1*x1
      x5y4z2=x4y4z2*x1
      x5y4z3=x4y4z3*x1
      x5y4z4=x4y4z4*x1
      x5y4z5=x4y4z5*x1
      x5y4z6=x4y4z6*x1
      x6=x5*x1
      x6z1=x5z1*x1
      x6z2=x5z2*x1
      x6z3=x5z3*x1
      x6z4=x5z4*x1
      x6z5=x5z5*x1
      x6z6=x5z6*x1
      x6y1=x5y1*x1
      x6y1z1=x5y1z1*x1
      x6y1z2=x5y1z2*x1
      x6y1z3=x5y1z3*x1
      x6y1z4=x5y1z4*x1
      x6y1z5=x5y1z5*x1
      x6y1z6=x5y1z6*x1
      x6y2=x5y2*x1
      x6y2z1=x5y2z1*x1
      x6y2z2=x5y2z2*x1
      x6y2z3=x5y2z3*x1
      x6y2z4=x5y2z4*x1
      x6y2z5=x5y2z5*x1
      x6y2z6=x5y2z6*x1
      x6y3=x5y3*x1
      x6y3z1=x5y3z1*x1
      x6y3z2=x5y3z2*x1
      x6y3z3=x5y3z3*x1
      x6y3z4=x5y3z4*x1
      x6y3z5=x5y3z5*x1
      x6y3z6=x5y3z6*x1
      x6y4=x5y4*x1
      x6y4z1=x5y4z1*x1
      x6y4z2=x5y4z2*x1
      x6y4z3=x5y4z3*x1
      x6y4z4=x5y4z4*x1
      x6y4z5=x5y4z5*x1
      x6y4z6=x5y4z6*x1
      r=(c(0,2,0,n)*2.+c(0,2,1,n)*z1*2.+c(0,2,2,n)*z2*2.+c(0,2,3,n)*z3*
     & 2.+c(0,2,4,n)*z4*2.+c(0,2,5,n)*z5*2.+c(0,2,6,n)*z6*2.+c(0,3,0,
     & n)*y1*6.+c(0,3,1,n)*y1z1*6.+c(0,3,2,n)*y1z2*6.+c(0,3,3,n)*y1z3*
     & 6.+c(0,3,4,n)*y1z4*6.+c(0,3,5,n)*y1z5*6.+c(0,3,6,n)*y1z6*6.+c(
     & 0,4,0,n)*y2*12.+c(0,4,1,n)*y2z1*12.+c(0,4,2,n)*y2z2*12.+c(0,4,
     & 3,n)*y2z3*12.+c(0,4,4,n)*y2z4*12.+c(0,4,5,n)*y2z5*12.+c(0,4,6,
     & n)*y2z6*12.+c(0,5,0,n)*y3*20.+c(0,5,1,n)*y3z1*20.+c(0,5,2,n)*
     & y3z2*20.+c(0,5,3,n)*y3z3*20.+c(0,5,4,n)*y3z4*20.+c(0,5,5,n)*
     & y3z5*20.+c(0,5,6,n)*y3z6*20.+c(0,6,0,n)*y4*30.+c(0,6,1,n)*y4z1*
     & 30.+c(0,6,2,n)*y4z2*30.+c(0,6,3,n)*y4z3*30.+c(0,6,4,n)*y4z4*
     & 30.+c(0,6,5,n)*y4z5*30.+c(0,6,6,n)*y4z6*30.+c(1,2,0,n)*x1*2.+c(
     & 1,2,1,n)*x1z1*2.+c(1,2,2,n)*x1z2*2.+c(1,2,3,n)*x1z3*2.+c(1,2,4,
     & n)*x1z4*2.+c(1,2,5,n)*x1z5*2.+c(1,2,6,n)*x1z6*2.+c(1,3,0,n)*
     & x1y1*6.+c(1,3,1,n)*x1y1z1*6.+c(1,3,2,n)*x1y1z2*6.+c(1,3,3,n)*
     & x1y1z3*6.+c(1,3,4,n)*x1y1z4*6.+c(1,3,5,n)*x1y1z5*6.+c(1,3,6,n)*
     & x1y1z6*6.+c(1,4,0,n)*x1y2*12.+c(1,4,1,n)*x1y2z1*12.+c(1,4,2,n)*
     & x1y2z2*12.+c(1,4,3,n)*x1y2z3*12.+c(1,4,4,n)*x1y2z4*12.+c(1,4,5,
     & n)*x1y2z5*12.+c(1,4,6,n)*x1y2z6*12.+c(1,5,0,n)*x1y3*20.+c(1,5,
     & 1,n)*x1y3z1*20.+c(1,5,2,n)*x1y3z2*20.+c(1,5,3,n)*x1y3z3*20.+c(
     & 1,5,4,n)*x1y3z4*20.+c(1,5,5,n)*x1y3z5*20.+c(1,5,6,n)*x1y3z6*
     & 20.+c(1,6,0,n)*x1y4*30.+c(1,6,1,n)*x1y4z1*30.+c(1,6,2,n)*
     & x1y4z2*30.+c(1,6,3,n)*x1y4z3*30.+c(1,6,4,n)*x1y4z4*30.+c(1,6,5,
     & n)*x1y4z5*30.+c(1,6,6,n)*x1y4z6*30.+c(2,2,0,n)*x2*2.+c(2,2,1,n)
     & *x2z1*2.+c(2,2,2,n)*x2z2*2.+c(2,2,3,n)*x2z3*2.+c(2,2,4,n)*x2z4*
     & 2.+c(2,2,5,n)*x2z5*2.+c(2,2,6,n)*x2z6*2.+c(2,3,0,n)*x2y1*6.+c(
     & 2,3,1,n)*x2y1z1*6.+c(2,3,2,n)*x2y1z2*6.+c(2,3,3,n)*x2y1z3*6.+c(
     & 2,3,4,n)*x2y1z4*6.+c(2,3,5,n)*x2y1z5*6.+c(2,3,6,n)*x2y1z6*6.+c(
     & 2,4,0,n)*x2y2*12.+c(2,4,1,n)*x2y2z1*12.+c(2,4,2,n)*x2y2z2*12.+
     & c(2,4,3,n)*x2y2z3*12.+c(2,4,4,n)*x2y2z4*12.+c(2,4,5,n)*x2y2z5*
     & 12.+c(2,4,6,n)*x2y2z6*12.+c(2,5,0,n)*x2y3*20.+c(2,5,1,n)*
     & x2y3z1*20.+c(2,5,2,n)*x2y3z2*20.+c(2,5,3,n)*x2y3z3*20.+c(2,5,4,
     & n)*x2y3z4*20.+c(2,5,5,n)*x2y3z5*20.+c(2,5,6,n)*x2y3z6*20.+c(2,
     & 6,0,n)*x2y4*30.+c(2,6,1,n)*x2y4z1*30.+c(2,6,2,n)*x2y4z2*30.+c(
     & 2,6,3,n)*x2y4z3*30.+c(2,6,4,n)*x2y4z4*30.+c(2,6,5,n)*x2y4z5*
     & 30.+c(2,6,6,n)*x2y4z6*30.+c(3,2,0,n)*x3*2.+c(3,2,1,n)*x3z1*2.+
     & c(3,2,2,n)*x3z2*2.+c(3,2,3,n)*x3z3*2.+c(3,2,4,n)*x3z4*2.+c(3,2,
     & 5,n)*x3z5*2.+c(3,2,6,n)*x3z6*2.+c(3,3,0,n)*x3y1*6.+c(3,3,1,n)*
     & x3y1z1*6.+c(3,3,2,n)*x3y1z2*6.+c(3,3,3,n)*x3y1z3*6.+c(3,3,4,n)*
     & x3y1z4*6.+c(3,3,5,n)*x3y1z5*6.+c(3,3,6,n)*x3y1z6*6.+c(3,4,0,n)*
     & x3y2*12.+c(3,4,1,n)*x3y2z1*12.+c(3,4,2,n)*x3y2z2*12.+c(3,4,3,n)
     & *x3y2z3*12.+c(3,4,4,n)*x3y2z4*12.+c(3,4,5,n)*x3y2z5*12.+c(3,4,
     & 6,n)*x3y2z6*12.+c(3,5,0,n)*x3y3*20.+c(3,5,1,n)*x3y3z1*20.+c(3,
     & 5,2,n)*x3y3z2*20.+c(3,5,3,n)*x3y3z3*20.+c(3,5,4,n)*x3y3z4*20.+
     & c(3,5,5,n)*x3y3z5*20.+c(3,5,6,n)*x3y3z6*20.+c(3,6,0,n)*x3y4*
     & 30.+c(3,6,1,n)*x3y4z1*30.+c(3,6,2,n)*x3y4z2*30.+c(3,6,3,n)*
     & x3y4z3*30.+c(3,6,4,n)*x3y4z4*30.+c(3,6,5,n)*x3y4z5*30.+c(3,6,6,
     & n)*x3y4z6*30.+c(4,2,0,n)*x4*2.+c(4,2,1,n)*x4z1*2.+c(4,2,2,n)*
     & x4z2*2.+c(4,2,3,n)*x4z3*2.+c(4,2,4,n)*x4z4*2.+c(4,2,5,n)*x4z5*
     & 2.+c(4,2,6,n)*x4z6*2.+c(4,3,0,n)*x4y1*6.+c(4,3,1,n)*x4y1z1*6.+
     & c(4,3,2,n)*x4y1z2*6.+c(4,3,3,n)*x4y1z3*6.+c(4,3,4,n)*x4y1z4*6.+
     & c(4,3,5,n)*x4y1z5*6.+c(4,3,6,n)*x4y1z6*6.+c(4,4,0,n)*x4y2*12.+
     & c(4,4,1,n)*x4y2z1*12.+c(4,4,2,n)*x4y2z2*12.+c(4,4,3,n)*x4y2z3*
     & 12.+c(4,4,4,n)*x4y2z4*12.+c(4,4,5,n)*x4y2z5*12.+c(4,4,6,n)*
     & x4y2z6*12.+c(4,5,0,n)*x4y3*20.+c(4,5,1,n)*x4y3z1*20.+c(4,5,2,n)
     & *x4y3z2*20.+c(4,5,3,n)*x4y3z3*20.+c(4,5,4,n)*x4y3z4*20.+c(4,5,
     & 5,n)*x4y3z5*20.+c(4,5,6,n)*x4y3z6*20.+c(4,6,0,n)*x4y4*30.+c(4,
     & 6,1,n)*x4y4z1*30.+c(4,6,2,n)*x4y4z2*30.+c(4,6,3,n)*x4y4z3*30.+
     & c(4,6,4,n)*x4y4z4*30.+c(4,6,5,n)*x4y4z5*30.+c(4,6,6,n)*x4y4z6*
     & 30.+c(5,2,0,n)*x5*2.+c(5,2,1,n)*x5z1*2.+c(5,2,2,n)*x5z2*2.+c(5,
     & 2,3,n)*x5z3*2.+c(5,2,4,n)*x5z4*2.+c(5,2,5,n)*x5z5*2.+c(5,2,6,n)
     & *x5z6*2.+c(5,3,0,n)*x5y1*6.+c(5,3,1,n)*x5y1z1*6.+c(5,3,2,n)*
     & x5y1z2*6.+c(5,3,3,n)*x5y1z3*6.+c(5,3,4,n)*x5y1z4*6.+c(5,3,5,n)*
     & x5y1z5*6.+c(5,3,6,n)*x5y1z6*6.+c(5,4,0,n)*x5y2*12.+c(5,4,1,n)*
     & x5y2z1*12.+c(5,4,2,n)*x5y2z2*12.+c(5,4,3,n)*x5y2z3*12.+c(5,4,4,
     & n)*x5y2z4*12.+c(5,4,5,n)*x5y2z5*12.+c(5,4,6,n)*x5y2z6*12.+c(5,
     & 5,0,n)*x5y3*20.+c(5,5,1,n)*x5y3z1*20.+c(5,5,2,n)*x5y3z2*20.+c(
     & 5,5,3,n)*x5y3z3*20.+c(5,5,4,n)*x5y3z4*20.+c(5,5,5,n)*x5y3z5*
     & 20.+c(5,5,6,n)*x5y3z6*20.+c(5,6,0,n)*x5y4*30.+c(5,6,1,n)*
     & x5y4z1*30.+c(5,6,2,n)*x5y4z2*30.+c(5,6,3,n)*x5y4z3*30.+c(5,6,4,
     & n)*x5y4z4*30.+c(5,6,5,n)*x5y4z5*30.+c(5,6,6,n)*x5y4z6*30.+c(6,
     & 2,0,n)*x6*2.+c(6,2,1,n)*x6z1*2.+c(6,2,2,n)*x6z2*2.+c(6,2,3,n)*
     & x6z3*2.+c(6,2,4,n)*x6z4*2.+c(6,2,5,n)*x6z5*2.+c(6,2,6,n)*x6z6*
     & 2.+c(6,3,0,n)*x6y1*6.+c(6,3,1,n)*x6y1z1*6.+c(6,3,2,n)*x6y1z2*
     & 6.+c(6,3,3,n)*x6y1z3*6.+c(6,3,4,n)*x6y1z4*6.+c(6,3,5,n)*x6y1z5*
     & 6.+c(6,3,6,n)*x6y1z6*6.+c(6,4,0,n)*x6y2*12.+c(6,4,1,n)*x6y2z1*
     & 12.+c(6,4,2,n)*x6y2z2*12.+c(6,4,3,n)*x6y2z3*12.+c(6,4,4,n)*
     & x6y2z4*12.+c(6,4,5,n)*x6y2z5*12.+c(6,4,6,n)*x6y2z6*12.+c(6,5,0,
     & n)*x6y3*20.+c(6,5,1,n)*x6y3z1*20.+c(6,5,2,n)*x6y3z2*20.)
       r=(r+c(6,5,3,n)*x6y3z3*20.+c(6,5,4,n)*x6y3z4*20.+c(6,5,5,n)*
     & x6y3z5*20.+c(6,5,6,n)*x6y3z6*20.+c(6,6,0,n)*x6y4*30.+c(6,6,1,n)
     & *x6y4z1*30.+c(6,6,2,n)*x6y4z2*30.+c(6,6,3,n)*x6y4z3*30.+c(6,6,
     & 4,n)*x6y4z4*30.+c(6,6,5,n)*x6y4z5*30.+c(6,6,6,n)*x6y4z6*30.)*
     & time
! endLoops()
      else if( dx.eq.0.and.dy.eq.2.and.dz.eq.1.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
c 
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
c 
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
      x6=x5*x1
      x6z1=x5z1*x1
      x6z2=x5z2*x1
      x6z3=x5z3*x1
      x6z4=x5z4*x1
      x6z5=x5z5*x1
      x6y1=x5y1*x1
      x6y1z1=x5y1z1*x1
      x6y1z2=x5y1z2*x1
      x6y1z3=x5y1z3*x1
      x6y1z4=x5y1z4*x1
      x6y1z5=x5y1z5*x1
      x6y2=x5y2*x1
      x6y2z1=x5y2z1*x1
      x6y2z2=x5y2z2*x1
      x6y2z3=x5y2z3*x1
      x6y2z4=x5y2z4*x1
      x6y2z5=x5y2z5*x1
      x6y3=x5y3*x1
      x6y3z1=x5y3z1*x1
      x6y3z2=x5y3z2*x1
      x6y3z3=x5y3z3*x1
      x6y3z4=x5y3z4*x1
      x6y3z5=x5y3z5*x1
      x6y4=x5y4*x1
      x6y4z1=x5y4z1*x1
      x6y4z2=x5y4z2*x1
      x6y4z3=x5y4z3*x1
      x6y4z4=x5y4z4*x1
      x6y4z5=x5y4z5*x1
      r=(c(0,2,1,n)*2.+c(0,2,2,n)*z1*4.+c(0,2,3,n)*z2*6.+c(0,2,4,n)*z3*
     & 8.+c(0,2,5,n)*z4*10.+c(0,2,6,n)*z5*12.+c(0,3,1,n)*y1*6.+c(0,3,
     & 2,n)*y1z1*12.+c(0,3,3,n)*y1z2*18.+c(0,3,4,n)*y1z3*24.+c(0,3,5,
     & n)*y1z4*30.+c(0,3,6,n)*y1z5*36.+c(0,4,1,n)*y2*12.+c(0,4,2,n)*
     & y2z1*24.+c(0,4,3,n)*y2z2*36.+c(0,4,4,n)*y2z3*48.+c(0,4,5,n)*
     & y2z4*60.+c(0,4,6,n)*y2z5*72.+c(0,5,1,n)*y3*20.+c(0,5,2,n)*y3z1*
     & 40.+c(0,5,3,n)*y3z2*60.+c(0,5,4,n)*y3z3*80.+c(0,5,5,n)*y3z4*
     & 100.+c(0,5,6,n)*y3z5*120.+c(0,6,1,n)*y4*30.+c(0,6,2,n)*y4z1*
     & 60.+c(0,6,3,n)*y4z2*90.+c(0,6,4,n)*y4z3*120.+c(0,6,5,n)*y4z4*
     & 150.+c(0,6,6,n)*y4z5*180.+c(1,2,1,n)*x1*2.+c(1,2,2,n)*x1z1*4.+
     & c(1,2,3,n)*x1z2*6.+c(1,2,4,n)*x1z3*8.+c(1,2,5,n)*x1z4*10.+c(1,
     & 2,6,n)*x1z5*12.+c(1,3,1,n)*x1y1*6.+c(1,3,2,n)*x1y1z1*12.+c(1,3,
     & 3,n)*x1y1z2*18.+c(1,3,4,n)*x1y1z3*24.+c(1,3,5,n)*x1y1z4*30.+c(
     & 1,3,6,n)*x1y1z5*36.+c(1,4,1,n)*x1y2*12.+c(1,4,2,n)*x1y2z1*24.+
     & c(1,4,3,n)*x1y2z2*36.+c(1,4,4,n)*x1y2z3*48.+c(1,4,5,n)*x1y2z4*
     & 60.+c(1,4,6,n)*x1y2z5*72.+c(1,5,1,n)*x1y3*20.+c(1,5,2,n)*
     & x1y3z1*40.+c(1,5,3,n)*x1y3z2*60.+c(1,5,4,n)*x1y3z3*80.+c(1,5,5,
     & n)*x1y3z4*100.+c(1,5,6,n)*x1y3z5*120.+c(1,6,1,n)*x1y4*30.+c(1,
     & 6,2,n)*x1y4z1*60.+c(1,6,3,n)*x1y4z2*90.+c(1,6,4,n)*x1y4z3*120.+
     & c(1,6,5,n)*x1y4z4*150.+c(1,6,6,n)*x1y4z5*180.+c(2,2,1,n)*x2*2.+
     & c(2,2,2,n)*x2z1*4.+c(2,2,3,n)*x2z2*6.+c(2,2,4,n)*x2z3*8.+c(2,2,
     & 5,n)*x2z4*10.+c(2,2,6,n)*x2z5*12.+c(2,3,1,n)*x2y1*6.+c(2,3,2,n)
     & *x2y1z1*12.+c(2,3,3,n)*x2y1z2*18.+c(2,3,4,n)*x2y1z3*24.+c(2,3,
     & 5,n)*x2y1z4*30.+c(2,3,6,n)*x2y1z5*36.+c(2,4,1,n)*x2y2*12.+c(2,
     & 4,2,n)*x2y2z1*24.+c(2,4,3,n)*x2y2z2*36.+c(2,4,4,n)*x2y2z3*48.+
     & c(2,4,5,n)*x2y2z4*60.+c(2,4,6,n)*x2y2z5*72.+c(2,5,1,n)*x2y3*
     & 20.+c(2,5,2,n)*x2y3z1*40.+c(2,5,3,n)*x2y3z2*60.+c(2,5,4,n)*
     & x2y3z3*80.+c(2,5,5,n)*x2y3z4*100.+c(2,5,6,n)*x2y3z5*120.+c(2,6,
     & 1,n)*x2y4*30.+c(2,6,2,n)*x2y4z1*60.+c(2,6,3,n)*x2y4z2*90.+c(2,
     & 6,4,n)*x2y4z3*120.+c(2,6,5,n)*x2y4z4*150.+c(2,6,6,n)*x2y4z5*
     & 180.+c(3,2,1,n)*x3*2.+c(3,2,2,n)*x3z1*4.+c(3,2,3,n)*x3z2*6.+c(
     & 3,2,4,n)*x3z3*8.+c(3,2,5,n)*x3z4*10.+c(3,2,6,n)*x3z5*12.+c(3,3,
     & 1,n)*x3y1*6.+c(3,3,2,n)*x3y1z1*12.+c(3,3,3,n)*x3y1z2*18.+c(3,3,
     & 4,n)*x3y1z3*24.+c(3,3,5,n)*x3y1z4*30.+c(3,3,6,n)*x3y1z5*36.+c(
     & 3,4,1,n)*x3y2*12.+c(3,4,2,n)*x3y2z1*24.+c(3,4,3,n)*x3y2z2*36.+
     & c(3,4,4,n)*x3y2z3*48.+c(3,4,5,n)*x3y2z4*60.+c(3,4,6,n)*x3y2z5*
     & 72.+c(3,5,1,n)*x3y3*20.+c(3,5,2,n)*x3y3z1*40.+c(3,5,3,n)*
     & x3y3z2*60.+c(3,5,4,n)*x3y3z3*80.+c(3,5,5,n)*x3y3z4*100.+c(3,5,
     & 6,n)*x3y3z5*120.+c(3,6,1,n)*x3y4*30.+c(3,6,2,n)*x3y4z1*60.+c(3,
     & 6,3,n)*x3y4z2*90.+c(3,6,4,n)*x3y4z3*120.+c(3,6,5,n)*x3y4z4*
     & 150.+c(3,6,6,n)*x3y4z5*180.+c(4,2,1,n)*x4*2.+c(4,2,2,n)*x4z1*
     & 4.+c(4,2,3,n)*x4z2*6.+c(4,2,4,n)*x4z3*8.+c(4,2,5,n)*x4z4*10.+c(
     & 4,2,6,n)*x4z5*12.+c(4,3,1,n)*x4y1*6.+c(4,3,2,n)*x4y1z1*12.+c(4,
     & 3,3,n)*x4y1z2*18.+c(4,3,4,n)*x4y1z3*24.+c(4,3,5,n)*x4y1z4*30.+
     & c(4,3,6,n)*x4y1z5*36.+c(4,4,1,n)*x4y2*12.+c(4,4,2,n)*x4y2z1*
     & 24.+c(4,4,3,n)*x4y2z2*36.+c(4,4,4,n)*x4y2z3*48.+c(4,4,5,n)*
     & x4y2z4*60.+c(4,4,6,n)*x4y2z5*72.+c(4,5,1,n)*x4y3*20.+c(4,5,2,n)
     & *x4y3z1*40.+c(4,5,3,n)*x4y3z2*60.+c(4,5,4,n)*x4y3z3*80.+c(4,5,
     & 5,n)*x4y3z4*100.+c(4,5,6,n)*x4y3z5*120.+c(4,6,1,n)*x4y4*30.+c(
     & 4,6,2,n)*x4y4z1*60.+c(4,6,3,n)*x4y4z2*90.+c(4,6,4,n)*x4y4z3*
     & 120.+c(4,6,5,n)*x4y4z4*150.+c(4,6,6,n)*x4y4z5*180.+c(5,2,1,n)*
     & x5*2.+c(5,2,2,n)*x5z1*4.+c(5,2,3,n)*x5z2*6.+c(5,2,4,n)*x5z3*8.+
     & c(5,2,5,n)*x5z4*10.+c(5,2,6,n)*x5z5*12.+c(5,3,1,n)*x5y1*6.+c(5,
     & 3,2,n)*x5y1z1*12.+c(5,3,3,n)*x5y1z2*18.+c(5,3,4,n)*x5y1z3*24.+
     & c(5,3,5,n)*x5y1z4*30.+c(5,3,6,n)*x5y1z5*36.+c(5,4,1,n)*x5y2*
     & 12.+c(5,4,2,n)*x5y2z1*24.+c(5,4,3,n)*x5y2z2*36.+c(5,4,4,n)*
     & x5y2z3*48.+c(5,4,5,n)*x5y2z4*60.+c(5,4,6,n)*x5y2z5*72.+c(5,5,1,
     & n)*x5y3*20.+c(5,5,2,n)*x5y3z1*40.+c(5,5,3,n)*x5y3z2*60.+c(5,5,
     & 4,n)*x5y3z3*80.+c(5,5,5,n)*x5y3z4*100.+c(5,5,6,n)*x5y3z5*120.+
     & c(5,6,1,n)*x5y4*30.+c(5,6,2,n)*x5y4z1*60.+c(5,6,3,n)*x5y4z2*
     & 90.+c(5,6,4,n)*x5y4z3*120.+c(5,6,5,n)*x5y4z4*150.+c(5,6,6,n)*
     & x5y4z5*180.+c(6,2,1,n)*x6*2.+c(6,2,2,n)*x6z1*4.+c(6,2,3,n)*
     & x6z2*6.+c(6,2,4,n)*x6z3*8.+c(6,2,5,n)*x6z4*10.+c(6,2,6,n)*x6z5*
     & 12.+c(6,3,1,n)*x6y1*6.+c(6,3,2,n)*x6y1z1*12.+c(6,3,3,n)*x6y1z2*
     & 18.+c(6,3,4,n)*x6y1z3*24.+c(6,3,5,n)*x6y1z4*30.+c(6,3,6,n)*
     & x6y1z5*36.+c(6,4,1,n)*x6y2*12.+c(6,4,2,n)*x6y2z1*24.+c(6,4,3,n)
     & *x6y2z2*36.+c(6,4,4,n)*x6y2z3*48.+c(6,4,5,n)*x6y2z4*60.+c(6,4,
     & 6,n)*x6y2z5*72.+c(6,5,1,n)*x6y3*20.+c(6,5,2,n)*x6y3z1*40.+c(6,
     & 5,3,n)*x6y3z2*60.+c(6,5,4,n)*x6y3z3*80.+c(6,5,5,n)*x6y3z4*100.+
     & c(6,5,6,n)*x6y3z5*120.+c(6,6,1,n)*x6y4*30.+c(6,6,2,n)*x6y4z1*
     & 60.+c(6,6,3,n)*x6y4z2*90.+c(6,6,4,n)*x6y4z3*120.+c(6,6,5,n)*
     & x6y4z4*150.+c(6,6,6,n)*x6y4z5*180.)*time
! endLoops()
      else if( dx.eq.0.and.dy.eq.2.and.dz.eq.2.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
c 
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
c 
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
      x6=x5*x1
      x6z1=x5z1*x1
      x6z2=x5z2*x1
      x6z3=x5z3*x1
      x6z4=x5z4*x1
      x6y1=x5y1*x1
      x6y1z1=x5y1z1*x1
      x6y1z2=x5y1z2*x1
      x6y1z3=x5y1z3*x1
      x6y1z4=x5y1z4*x1
      x6y2=x5y2*x1
      x6y2z1=x5y2z1*x1
      x6y2z2=x5y2z2*x1
      x6y2z3=x5y2z3*x1
      x6y2z4=x5y2z4*x1
      x6y3=x5y3*x1
      x6y3z1=x5y3z1*x1
      x6y3z2=x5y3z2*x1
      x6y3z3=x5y3z3*x1
      x6y3z4=x5y3z4*x1
      x6y4=x5y4*x1
      x6y4z1=x5y4z1*x1
      x6y4z2=x5y4z2*x1
      x6y4z3=x5y4z3*x1
      x6y4z4=x5y4z4*x1
      r=(c(0,2,2,n)*4.+c(0,2,3,n)*z1*12.+c(0,2,4,n)*z2*24.+c(0,2,5,n)*
     & z3*40.+c(0,2,6,n)*z4*60.+c(0,3,2,n)*y1*12.+c(0,3,3,n)*y1z1*36.+
     & c(0,3,4,n)*y1z2*72.+c(0,3,5,n)*y1z3*120.+c(0,3,6,n)*y1z4*180.+
     & c(0,4,2,n)*y2*24.+c(0,4,3,n)*y2z1*72.+c(0,4,4,n)*y2z2*144.+c(0,
     & 4,5,n)*y2z3*240.+c(0,4,6,n)*y2z4*360.+c(0,5,2,n)*y3*40.+c(0,5,
     & 3,n)*y3z1*120.+c(0,5,4,n)*y3z2*240.+c(0,5,5,n)*y3z3*400.+c(0,5,
     & 6,n)*y3z4*600.+c(0,6,2,n)*y4*60.+c(0,6,3,n)*y4z1*180.+c(0,6,4,
     & n)*y4z2*360.+c(0,6,5,n)*y4z3*600.+c(0,6,6,n)*y4z4*900.+c(1,2,2,
     & n)*x1*4.+c(1,2,3,n)*x1z1*12.+c(1,2,4,n)*x1z2*24.+c(1,2,5,n)*
     & x1z3*40.+c(1,2,6,n)*x1z4*60.+c(1,3,2,n)*x1y1*12.+c(1,3,3,n)*
     & x1y1z1*36.+c(1,3,4,n)*x1y1z2*72.+c(1,3,5,n)*x1y1z3*120.+c(1,3,
     & 6,n)*x1y1z4*180.+c(1,4,2,n)*x1y2*24.+c(1,4,3,n)*x1y2z1*72.+c(1,
     & 4,4,n)*x1y2z2*144.+c(1,4,5,n)*x1y2z3*240.+c(1,4,6,n)*x1y2z4*
     & 360.+c(1,5,2,n)*x1y3*40.+c(1,5,3,n)*x1y3z1*120.+c(1,5,4,n)*
     & x1y3z2*240.+c(1,5,5,n)*x1y3z3*400.+c(1,5,6,n)*x1y3z4*600.+c(1,
     & 6,2,n)*x1y4*60.+c(1,6,3,n)*x1y4z1*180.+c(1,6,4,n)*x1y4z2*360.+
     & c(1,6,5,n)*x1y4z3*600.+c(1,6,6,n)*x1y4z4*900.+c(2,2,2,n)*x2*4.+
     & c(2,2,3,n)*x2z1*12.+c(2,2,4,n)*x2z2*24.+c(2,2,5,n)*x2z3*40.+c(
     & 2,2,6,n)*x2z4*60.+c(2,3,2,n)*x2y1*12.+c(2,3,3,n)*x2y1z1*36.+c(
     & 2,3,4,n)*x2y1z2*72.+c(2,3,5,n)*x2y1z3*120.+c(2,3,6,n)*x2y1z4*
     & 180.+c(2,4,2,n)*x2y2*24.+c(2,4,3,n)*x2y2z1*72.+c(2,4,4,n)*
     & x2y2z2*144.+c(2,4,5,n)*x2y2z3*240.+c(2,4,6,n)*x2y2z4*360.+c(2,
     & 5,2,n)*x2y3*40.+c(2,5,3,n)*x2y3z1*120.+c(2,5,4,n)*x2y3z2*240.+
     & c(2,5,5,n)*x2y3z3*400.+c(2,5,6,n)*x2y3z4*600.+c(2,6,2,n)*x2y4*
     & 60.+c(2,6,3,n)*x2y4z1*180.+c(2,6,4,n)*x2y4z2*360.+c(2,6,5,n)*
     & x2y4z3*600.+c(2,6,6,n)*x2y4z4*900.+c(3,2,2,n)*x3*4.+c(3,2,3,n)*
     & x3z1*12.+c(3,2,4,n)*x3z2*24.+c(3,2,5,n)*x3z3*40.+c(3,2,6,n)*
     & x3z4*60.+c(3,3,2,n)*x3y1*12.+c(3,3,3,n)*x3y1z1*36.+c(3,3,4,n)*
     & x3y1z2*72.+c(3,3,5,n)*x3y1z3*120.+c(3,3,6,n)*x3y1z4*180.+c(3,4,
     & 2,n)*x3y2*24.+c(3,4,3,n)*x3y2z1*72.+c(3,4,4,n)*x3y2z2*144.+c(3,
     & 4,5,n)*x3y2z3*240.+c(3,4,6,n)*x3y2z4*360.+c(3,5,2,n)*x3y3*40.+
     & c(3,5,3,n)*x3y3z1*120.+c(3,5,4,n)*x3y3z2*240.+c(3,5,5,n)*
     & x3y3z3*400.+c(3,5,6,n)*x3y3z4*600.+c(3,6,2,n)*x3y4*60.+c(3,6,3,
     & n)*x3y4z1*180.+c(3,6,4,n)*x3y4z2*360.+c(3,6,5,n)*x3y4z3*600.+c(
     & 3,6,6,n)*x3y4z4*900.+c(4,2,2,n)*x4*4.+c(4,2,3,n)*x4z1*12.+c(4,
     & 2,4,n)*x4z2*24.+c(4,2,5,n)*x4z3*40.+c(4,2,6,n)*x4z4*60.+c(4,3,
     & 2,n)*x4y1*12.+c(4,3,3,n)*x4y1z1*36.+c(4,3,4,n)*x4y1z2*72.+c(4,
     & 3,5,n)*x4y1z3*120.+c(4,3,6,n)*x4y1z4*180.+c(4,4,2,n)*x4y2*24.+
     & c(4,4,3,n)*x4y2z1*72.+c(4,4,4,n)*x4y2z2*144.+c(4,4,5,n)*x4y2z3*
     & 240.+c(4,4,6,n)*x4y2z4*360.+c(4,5,2,n)*x4y3*40.+c(4,5,3,n)*
     & x4y3z1*120.+c(4,5,4,n)*x4y3z2*240.+c(4,5,5,n)*x4y3z3*400.+c(4,
     & 5,6,n)*x4y3z4*600.+c(4,6,2,n)*x4y4*60.+c(4,6,3,n)*x4y4z1*180.+
     & c(4,6,4,n)*x4y4z2*360.+c(4,6,5,n)*x4y4z3*600.+c(4,6,6,n)*
     & x4y4z4*900.+c(5,2,2,n)*x5*4.+c(5,2,3,n)*x5z1*12.+c(5,2,4,n)*
     & x5z2*24.+c(5,2,5,n)*x5z3*40.+c(5,2,6,n)*x5z4*60.+c(5,3,2,n)*
     & x5y1*12.+c(5,3,3,n)*x5y1z1*36.+c(5,3,4,n)*x5y1z2*72.+c(5,3,5,n)
     & *x5y1z3*120.+c(5,3,6,n)*x5y1z4*180.+c(5,4,2,n)*x5y2*24.+c(5,4,
     & 3,n)*x5y2z1*72.+c(5,4,4,n)*x5y2z2*144.+c(5,4,5,n)*x5y2z3*240.+
     & c(5,4,6,n)*x5y2z4*360.+c(5,5,2,n)*x5y3*40.+c(5,5,3,n)*x5y3z1*
     & 120.+c(5,5,4,n)*x5y3z2*240.+c(5,5,5,n)*x5y3z3*400.+c(5,5,6,n)*
     & x5y3z4*600.+c(5,6,2,n)*x5y4*60.+c(5,6,3,n)*x5y4z1*180.+c(5,6,4,
     & n)*x5y4z2*360.+c(5,6,5,n)*x5y4z3*600.+c(5,6,6,n)*x5y4z4*900.+c(
     & 6,2,2,n)*x6*4.+c(6,2,3,n)*x6z1*12.+c(6,2,4,n)*x6z2*24.+c(6,2,5,
     & n)*x6z3*40.+c(6,2,6,n)*x6z4*60.+c(6,3,2,n)*x6y1*12.+c(6,3,3,n)*
     & x6y1z1*36.+c(6,3,4,n)*x6y1z2*72.+c(6,3,5,n)*x6y1z3*120.+c(6,3,
     & 6,n)*x6y1z4*180.+c(6,4,2,n)*x6y2*24.+c(6,4,3,n)*x6y2z1*72.+c(6,
     & 4,4,n)*x6y2z2*144.+c(6,4,5,n)*x6y2z3*240.+c(6,4,6,n)*x6y2z4*
     & 360.+c(6,5,2,n)*x6y3*40.+c(6,5,3,n)*x6y3z1*120.+c(6,5,4,n)*
     & x6y3z2*240.+c(6,5,5,n)*x6y3z3*400.+c(6,5,6,n)*x6y3z4*600.+c(6,
     & 6,2,n)*x6y4*60.+c(6,6,3,n)*x6y4z1*180.+c(6,6,4,n)*x6y4z2*360.+
     & c(6,6,5,n)*x6y4z3*600.+c(6,6,6,n)*x6y4z4*900.)*time
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
      z6=z5*z1
c 
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y1z5=y1z4*z1
      y1z6=y1z5*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
      y2z5=y1z5*y1
      y2z6=y1z6*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      y3z4=y2z4*y1
      y3z5=y2z5*y1
      y3z6=y2z6*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y4z2=y3z2*y1
      y4z3=y3z3*y1
      y4z4=y3z4*y1
      y4z5=y3z5*y1
      y4z6=y3z6*y1
      y5=y4*y1
      y5z1=y4z1*y1
      y5z2=y4z2*y1
      y5z3=y4z3*y1
      y5z4=y4z4*y1
      y5z5=y4z5*y1
      y5z6=y4z6*y1
      y6=y5*y1
      y6z1=y5z1*y1
      y6z2=y5z2*y1
      y6z3=y5z3*y1
      y6z4=y5z4*y1
      y6z5=y5z5*y1
      y6z6=y5z6*y1
c 
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1z5=x1z4*z1
      x1z6=x1z5*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y1z5=x1y1z4*z1
      x1y1z6=x1y1z5*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y2z4=x1y1z4*y1
      x1y2z5=x1y1z5*y1
      x1y2z6=x1y1z6*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x1y3z4=x1y2z4*y1
      x1y3z5=x1y2z5*y1
      x1y3z6=x1y2z6*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y4z2=x1y3z2*y1
      x1y4z3=x1y3z3*y1
      x1y4z4=x1y3z4*y1
      x1y4z5=x1y3z5*y1
      x1y4z6=x1y3z6*y1
      x1y5=x1y4*y1
      x1y5z1=x1y4z1*y1
      x1y5z2=x1y4z2*y1
      x1y5z3=x1y4z3*y1
      x1y5z4=x1y4z4*y1
      x1y5z5=x1y4z5*y1
      x1y5z6=x1y4z6*y1
      x1y6=x1y5*y1
      x1y6z1=x1y5z1*y1
      x1y6z2=x1y5z2*y1
      x1y6z3=x1y5z3*y1
      x1y6z4=x1y5z4*y1
      x1y6z5=x1y5z5*y1
      x1y6z6=x1y5z6*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x2z5=x1z5*x1
      x2z6=x1z6*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y1z4=x1y1z4*x1
      x2y1z5=x1y1z5*x1
      x2y1z6=x1y1z6*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y2z4=x1y2z4*x1
      x2y2z5=x1y2z5*x1
      x2y2z6=x1y2z6*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y3z3=x1y3z3*x1
      x2y3z4=x1y3z4*x1
      x2y3z5=x1y3z5*x1
      x2y3z6=x1y3z6*x1
      x2y4=x1y4*x1
      x2y4z1=x1y4z1*x1
      x2y4z2=x1y4z2*x1
      x2y4z3=x1y4z3*x1
      x2y4z4=x1y4z4*x1
      x2y4z5=x1y4z5*x1
      x2y4z6=x1y4z6*x1
      x2y5=x1y5*x1
      x2y5z1=x1y5z1*x1
      x2y5z2=x1y5z2*x1
      x2y5z3=x1y5z3*x1
      x2y5z4=x1y5z4*x1
      x2y5z5=x1y5z5*x1
      x2y5z6=x1y5z6*x1
      x2y6=x1y6*x1
      x2y6z1=x1y6z1*x1
      x2y6z2=x1y6z2*x1
      x2y6z3=x1y6z3*x1
      x2y6z4=x1y6z4*x1
      x2y6z5=x1y6z5*x1
      x2y6z6=x1y6z6*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3z4=x2z4*x1
      x3z5=x2z5*x1
      x3z6=x2z6*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x3y1z4=x2y1z4*x1
      x3y1z5=x2y1z5*x1
      x3y1z6=x2y1z6*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y2z3=x2y2z3*x1
      x3y2z4=x2y2z4*x1
      x3y2z5=x2y2z5*x1
      x3y2z6=x2y2z6*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x3y3z2=x2y3z2*x1
      x3y3z3=x2y3z3*x1
      x3y3z4=x2y3z4*x1
      x3y3z5=x2y3z5*x1
      x3y3z6=x2y3z6*x1
      x3y4=x2y4*x1
      x3y4z1=x2y4z1*x1
      x3y4z2=x2y4z2*x1
      x3y4z3=x2y4z3*x1
      x3y4z4=x2y4z4*x1
      x3y4z5=x2y4z5*x1
      x3y4z6=x2y4z6*x1
      x3y5=x2y5*x1
      x3y5z1=x2y5z1*x1
      x3y5z2=x2y5z2*x1
      x3y5z3=x2y5z3*x1
      x3y5z4=x2y5z4*x1
      x3y5z5=x2y5z5*x1
      x3y5z6=x2y5z6*x1
      x3y6=x2y6*x1
      x3y6z1=x2y6z1*x1
      x3y6z2=x2y6z2*x1
      x3y6z3=x2y6z3*x1
      x3y6z4=x2y6z4*x1
      x3y6z5=x2y6z5*x1
      x3y6z6=x2y6z6*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4z2=x3z2*x1
      x4z3=x3z3*x1
      x4z4=x3z4*x1
      x4z5=x3z5*x1
      x4z6=x3z6*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y1z2=x3y1z2*x1
      x4y1z3=x3y1z3*x1
      x4y1z4=x3y1z4*x1
      x4y1z5=x3y1z5*x1
      x4y1z6=x3y1z6*x1
      x4y2=x3y2*x1
      x4y2z1=x3y2z1*x1
      x4y2z2=x3y2z2*x1
      x4y2z3=x3y2z3*x1
      x4y2z4=x3y2z4*x1
      x4y2z5=x3y2z5*x1
      x4y2z6=x3y2z6*x1
      x4y3=x3y3*x1
      x4y3z1=x3y3z1*x1
      x4y3z2=x3y3z2*x1
      x4y3z3=x3y3z3*x1
      x4y3z4=x3y3z4*x1
      x4y3z5=x3y3z5*x1
      x4y3z6=x3y3z6*x1
      x4y4=x3y4*x1
      x4y4z1=x3y4z1*x1
      x4y4z2=x3y4z2*x1
      x4y4z3=x3y4z3*x1
      x4y4z4=x3y4z4*x1
      x4y4z5=x3y4z5*x1
      x4y4z6=x3y4z6*x1
      x4y5=x3y5*x1
      x4y5z1=x3y5z1*x1
      x4y5z2=x3y5z2*x1
      x4y5z3=x3y5z3*x1
      x4y5z4=x3y5z4*x1
      x4y5z5=x3y5z5*x1
      x4y5z6=x3y5z6*x1
      x4y6=x3y6*x1
      x4y6z1=x3y6z1*x1
      x4y6z2=x3y6z2*x1
      x4y6z3=x3y6z3*x1
      x4y6z4=x3y6z4*x1
      x4y6z5=x3y6z5*x1
      x4y6z6=x3y6z6*x1
      x5=x4*x1
      x5z1=x4z1*x1
      x5z2=x4z2*x1
      x5z3=x4z3*x1
      x5z4=x4z4*x1
      x5z5=x4z5*x1
      x5z6=x4z6*x1
      x5y1=x4y1*x1
      x5y1z1=x4y1z1*x1
      x5y1z2=x4y1z2*x1
      x5y1z3=x4y1z3*x1
      x5y1z4=x4y1z4*x1
      x5y1z5=x4y1z5*x1
      x5y1z6=x4y1z6*x1
      x5y2=x4y2*x1
      x5y2z1=x4y2z1*x1
      x5y2z2=x4y2z2*x1
      x5y2z3=x4y2z3*x1
      x5y2z4=x4y2z4*x1
      x5y2z5=x4y2z5*x1
      x5y2z6=x4y2z6*x1
      x5y3=x4y3*x1
      x5y3z1=x4y3z1*x1
      x5y3z2=x4y3z2*x1
      x5y3z3=x4y3z3*x1
      x5y3z4=x4y3z4*x1
      x5y3z5=x4y3z5*x1
      x5y3z6=x4y3z6*x1
      x5y4=x4y4*x1
      x5y4z1=x4y4z1*x1
      x5y4z2=x4y4z2*x1
      x5y4z3=x4y4z3*x1
      x5y4z4=x4y4z4*x1
      x5y4z5=x4y4z5*x1
      x5y4z6=x4y4z6*x1
      x5y5=x4y5*x1
      x5y5z1=x4y5z1*x1
      x5y5z2=x4y5z2*x1
      x5y5z3=x4y5z3*x1
      x5y5z4=x4y5z4*x1
      x5y5z5=x4y5z5*x1
      x5y5z6=x4y5z6*x1
      x5y6=x4y6*x1
      x5y6z1=x4y6z1*x1
      x5y6z2=x4y6z2*x1
      x5y6z3=x4y6z3*x1
      x5y6z4=x4y6z4*x1
      x5y6z5=x4y6z5*x1
      x5y6z6=x4y6z6*x1
      r=(c(1,0,0,n)+c(1,0,1,n)*z1+c(1,0,2,n)*z2+c(1,0,3,n)*z3+c(1,0,4,
     & n)*z4+c(1,0,5,n)*z5+c(1,0,6,n)*z6+c(1,1,0,n)*y1+c(1,1,1,n)*
     & y1z1+c(1,1,2,n)*y1z2+c(1,1,3,n)*y1z3+c(1,1,4,n)*y1z4+c(1,1,5,n)
     & *y1z5+c(1,1,6,n)*y1z6+c(1,2,0,n)*y2+c(1,2,1,n)*y2z1+c(1,2,2,n)*
     & y2z2+c(1,2,3,n)*y2z3+c(1,2,4,n)*y2z4+c(1,2,5,n)*y2z5+c(1,2,6,n)
     & *y2z6+c(1,3,0,n)*y3+c(1,3,1,n)*y3z1+c(1,3,2,n)*y3z2+c(1,3,3,n)*
     & y3z3+c(1,3,4,n)*y3z4+c(1,3,5,n)*y3z5+c(1,3,6,n)*y3z6+c(1,4,0,n)
     & *y4+c(1,4,1,n)*y4z1+c(1,4,2,n)*y4z2+c(1,4,3,n)*y4z3+c(1,4,4,n)*
     & y4z4+c(1,4,5,n)*y4z5+c(1,4,6,n)*y4z6+c(1,5,0,n)*y5+c(1,5,1,n)*
     & y5z1+c(1,5,2,n)*y5z2+c(1,5,3,n)*y5z3+c(1,5,4,n)*y5z4+c(1,5,5,n)
     & *y5z5+c(1,5,6,n)*y5z6+c(1,6,0,n)*y6+c(1,6,1,n)*y6z1+c(1,6,2,n)*
     & y6z2+c(1,6,3,n)*y6z3+c(1,6,4,n)*y6z4+c(1,6,5,n)*y6z5+c(1,6,6,n)
     & *y6z6+c(2,0,0,n)*x1*2.+c(2,0,1,n)*x1z1*2.+c(2,0,2,n)*x1z2*2.+c(
     & 2,0,3,n)*x1z3*2.+c(2,0,4,n)*x1z4*2.+c(2,0,5,n)*x1z5*2.+c(2,0,6,
     & n)*x1z6*2.+c(2,1,0,n)*x1y1*2.+c(2,1,1,n)*x1y1z1*2.+c(2,1,2,n)*
     & x1y1z2*2.+c(2,1,3,n)*x1y1z3*2.+c(2,1,4,n)*x1y1z4*2.+c(2,1,5,n)*
     & x1y1z5*2.+c(2,1,6,n)*x1y1z6*2.+c(2,2,0,n)*x1y2*2.+c(2,2,1,n)*
     & x1y2z1*2.+c(2,2,2,n)*x1y2z2*2.+c(2,2,3,n)*x1y2z3*2.+c(2,2,4,n)*
     & x1y2z4*2.+c(2,2,5,n)*x1y2z5*2.+c(2,2,6,n)*x1y2z6*2.+c(2,3,0,n)*
     & x1y3*2.+c(2,3,1,n)*x1y3z1*2.+c(2,3,2,n)*x1y3z2*2.+c(2,3,3,n)*
     & x1y3z3*2.+c(2,3,4,n)*x1y3z4*2.+c(2,3,5,n)*x1y3z5*2.+c(2,3,6,n)*
     & x1y3z6*2.+c(2,4,0,n)*x1y4*2.+c(2,4,1,n)*x1y4z1*2.+c(2,4,2,n)*
     & x1y4z2*2.+c(2,4,3,n)*x1y4z3*2.+c(2,4,4,n)*x1y4z4*2.+c(2,4,5,n)*
     & x1y4z5*2.+c(2,4,6,n)*x1y4z6*2.+c(2,5,0,n)*x1y5*2.+c(2,5,1,n)*
     & x1y5z1*2.+c(2,5,2,n)*x1y5z2*2.+c(2,5,3,n)*x1y5z3*2.+c(2,5,4,n)*
     & x1y5z4*2.+c(2,5,5,n)*x1y5z5*2.+c(2,5,6,n)*x1y5z6*2.+c(2,6,0,n)*
     & x1y6*2.+c(2,6,1,n)*x1y6z1*2.+c(2,6,2,n)*x1y6z2*2.+c(2,6,3,n)*
     & x1y6z3*2.+c(2,6,4,n)*x1y6z4*2.+c(2,6,5,n)*x1y6z5*2.+c(2,6,6,n)*
     & x1y6z6*2.+c(3,0,0,n)*x2*3.+c(3,0,1,n)*x2z1*3.+c(3,0,2,n)*x2z2*
     & 3.+c(3,0,3,n)*x2z3*3.+c(3,0,4,n)*x2z4*3.+c(3,0,5,n)*x2z5*3.+c(
     & 3,0,6,n)*x2z6*3.+c(3,1,0,n)*x2y1*3.+c(3,1,1,n)*x2y1z1*3.+c(3,1,
     & 2,n)*x2y1z2*3.+c(3,1,3,n)*x2y1z3*3.+c(3,1,4,n)*x2y1z4*3.+c(3,1,
     & 5,n)*x2y1z5*3.+c(3,1,6,n)*x2y1z6*3.+c(3,2,0,n)*x2y2*3.+c(3,2,1,
     & n)*x2y2z1*3.+c(3,2,2,n)*x2y2z2*3.+c(3,2,3,n)*x2y2z3*3.+c(3,2,4,
     & n)*x2y2z4*3.+c(3,2,5,n)*x2y2z5*3.+c(3,2,6,n)*x2y2z6*3.+c(3,3,0,
     & n)*x2y3*3.+c(3,3,1,n)*x2y3z1*3.+c(3,3,2,n)*x2y3z2*3.+c(3,3,3,n)
     & *x2y3z3*3.+c(3,3,4,n)*x2y3z4*3.+c(3,3,5,n)*x2y3z5*3.+c(3,3,6,n)
     & *x2y3z6*3.+c(3,4,0,n)*x2y4*3.+c(3,4,1,n)*x2y4z1*3.+c(3,4,2,n)*
     & x2y4z2*3.+c(3,4,3,n)*x2y4z3*3.+c(3,4,4,n)*x2y4z4*3.+c(3,4,5,n)*
     & x2y4z5*3.+c(3,4,6,n)*x2y4z6*3.+c(3,5,0,n)*x2y5*3.+c(3,5,1,n)*
     & x2y5z1*3.+c(3,5,2,n)*x2y5z2*3.+c(3,5,3,n)*x2y5z3*3.+c(3,5,4,n)*
     & x2y5z4*3.+c(3,5,5,n)*x2y5z5*3.+c(3,5,6,n)*x2y5z6*3.+c(3,6,0,n)*
     & x2y6*3.+c(3,6,1,n)*x2y6z1*3.+c(3,6,2,n)*x2y6z2*3.+c(3,6,3,n)*
     & x2y6z3*3.+c(3,6,4,n)*x2y6z4*3.+c(3,6,5,n)*x2y6z5*3.+c(3,6,6,n)*
     & x2y6z6*3.+c(4,0,0,n)*x3*4.+c(4,0,1,n)*x3z1*4.+c(4,0,2,n)*x3z2*
     & 4.+c(4,0,3,n)*x3z3*4.+c(4,0,4,n)*x3z4*4.+c(4,0,5,n)*x3z5*4.+c(
     & 4,0,6,n)*x3z6*4.+c(4,1,0,n)*x3y1*4.+c(4,1,1,n)*x3y1z1*4.+c(4,1,
     & 2,n)*x3y1z2*4.+c(4,1,3,n)*x3y1z3*4.+c(4,1,4,n)*x3y1z4*4.+c(4,1,
     & 5,n)*x3y1z5*4.+c(4,1,6,n)*x3y1z6*4.+c(4,2,0,n)*x3y2*4.+c(4,2,1,
     & n)*x3y2z1*4.+c(4,2,2,n)*x3y2z2*4.+c(4,2,3,n)*x3y2z3*4.+c(4,2,4,
     & n)*x3y2z4*4.+c(4,2,5,n)*x3y2z5*4.+c(4,2,6,n)*x3y2z6*4.+c(4,3,0,
     & n)*x3y3*4.+c(4,3,1,n)*x3y3z1*4.+c(4,3,2,n)*x3y3z2*4.+c(4,3,3,n)
     & *x3y3z3*4.+c(4,3,4,n)*x3y3z4*4.+c(4,3,5,n)*x3y3z5*4.+c(4,3,6,n)
     & *x3y3z6*4.+c(4,4,0,n)*x3y4*4.+c(4,4,1,n)*x3y4z1*4.+c(4,4,2,n)*
     & x3y4z2*4.+c(4,4,3,n)*x3y4z3*4.+c(4,4,4,n)*x3y4z4*4.+c(4,4,5,n)*
     & x3y4z5*4.+c(4,4,6,n)*x3y4z6*4.+c(4,5,0,n)*x3y5*4.+c(4,5,1,n)*
     & x3y5z1*4.+c(4,5,2,n)*x3y5z2*4.+c(4,5,3,n)*x3y5z3*4.+c(4,5,4,n)*
     & x3y5z4*4.+c(4,5,5,n)*x3y5z5*4.+c(4,5,6,n)*x3y5z6*4.+c(4,6,0,n)*
     & x3y6*4.+c(4,6,1,n)*x3y6z1*4.+c(4,6,2,n)*x3y6z2*4.+c(4,6,3,n)*
     & x3y6z3*4.+c(4,6,4,n)*x3y6z4*4.+c(4,6,5,n)*x3y6z5*4.+c(4,6,6,n)*
     & x3y6z6*4.+c(5,0,0,n)*x4*5.+c(5,0,1,n)*x4z1*5.+c(5,0,2,n)*x4z2*
     & 5.+c(5,0,3,n)*x4z3*5.+c(5,0,4,n)*x4z4*5.+c(5,0,5,n)*x4z5*5.+c(
     & 5,0,6,n)*x4z6*5.+c(5,1,0,n)*x4y1*5.+c(5,1,1,n)*x4y1z1*5.+c(5,1,
     & 2,n)*x4y1z2*5.+c(5,1,3,n)*x4y1z3*5.+c(5,1,4,n)*x4y1z4*5.+c(5,1,
     & 5,n)*x4y1z5*5.+c(5,1,6,n)*x4y1z6*5.+c(5,2,0,n)*x4y2*5.+c(5,2,1,
     & n)*x4y2z1*5.+c(5,2,2,n)*x4y2z2*5.+c(5,2,3,n)*x4y2z3*5.+c(5,2,4,
     & n)*x4y2z4*5.+c(5,2,5,n)*x4y2z5*5.+c(5,2,6,n)*x4y2z6*5.+c(5,3,0,
     & n)*x4y3*5.+c(5,3,1,n)*x4y3z1*5.+c(5,3,2,n)*x4y3z2*5.+c(5,3,3,n)
     & *x4y3z3*5.+c(5,3,4,n)*x4y3z4*5.+c(5,3,5,n)*x4y3z5*5.+c(5,3,6,n)
     & *x4y3z6*5.+c(5,4,0,n)*x4y4*5.+c(5,4,1,n)*x4y4z1*5.+c(5,4,2,n)*
     & x4y4z2*5.+c(5,4,3,n)*x4y4z3*5.+c(5,4,4,n)*x4y4z4*5.+c(5,4,5,n)*
     & x4y4z5*5.+c(5,4,6,n)*x4y4z6*5.+c(5,5,0,n)*x4y5*5.+c(5,5,1,n)*
     & x4y5z1*5.+c(5,5,2,n)*x4y5z2*5.+c(5,5,3,n)*x4y5z3*5.+c(5,5,4,n)*
     & x4y5z4*5.+c(5,5,5,n)*x4y5z5*5.+c(5,5,6,n)*x4y5z6*5.+c(5,6,0,n)*
     & x4y6*5.+c(5,6,1,n)*x4y6z1*5.+c(5,6,2,n)*x4y6z2*5.+c(5,6,3,n)*
     & x4y6z3*5.+c(5,6,4,n)*x4y6z4*5.+c(5,6,5,n)*x4y6z5*5.+c(5,6,6,n)*
     & x4y6z6*5.+c(6,0,0,n)*x5*6.+c(6,0,1,n)*x5z1*6.+c(6,0,2,n)*x5z2*
     & 6.)
       r=(r+c(6,0,3,n)*x5z3*6.+c(6,0,4,n)*x5z4*6.+c(6,0,5,n)*x5z5*6.+c(
     & 6,0,6,n)*x5z6*6.+c(6,1,0,n)*x5y1*6.+c(6,1,1,n)*x5y1z1*6.+c(6,1,
     & 2,n)*x5y1z2*6.+c(6,1,3,n)*x5y1z3*6.+c(6,1,4,n)*x5y1z4*6.+c(6,1,
     & 5,n)*x5y1z5*6.+c(6,1,6,n)*x5y1z6*6.+c(6,2,0,n)*x5y2*6.+c(6,2,1,
     & n)*x5y2z1*6.+c(6,2,2,n)*x5y2z2*6.+c(6,2,3,n)*x5y2z3*6.+c(6,2,4,
     & n)*x5y2z4*6.+c(6,2,5,n)*x5y2z5*6.+c(6,2,6,n)*x5y2z6*6.+c(6,3,0,
     & n)*x5y3*6.+c(6,3,1,n)*x5y3z1*6.+c(6,3,2,n)*x5y3z2*6.+c(6,3,3,n)
     & *x5y3z3*6.+c(6,3,4,n)*x5y3z4*6.+c(6,3,5,n)*x5y3z5*6.+c(6,3,6,n)
     & *x5y3z6*6.+c(6,4,0,n)*x5y4*6.+c(6,4,1,n)*x5y4z1*6.+c(6,4,2,n)*
     & x5y4z2*6.+c(6,4,3,n)*x5y4z3*6.+c(6,4,4,n)*x5y4z4*6.+c(6,4,5,n)*
     & x5y4z5*6.+c(6,4,6,n)*x5y4z6*6.+c(6,5,0,n)*x5y5*6.+c(6,5,1,n)*
     & x5y5z1*6.+c(6,5,2,n)*x5y5z2*6.+c(6,5,3,n)*x5y5z3*6.+c(6,5,4,n)*
     & x5y5z4*6.+c(6,5,5,n)*x5y5z5*6.+c(6,5,6,n)*x5y5z6*6.+c(6,6,0,n)*
     & x5y6*6.+c(6,6,1,n)*x5y6z1*6.+c(6,6,2,n)*x5y6z2*6.+c(6,6,3,n)*
     & x5y6z3*6.+c(6,6,4,n)*x5y6z4*6.+c(6,6,5,n)*x5y6z5*6.+c(6,6,6,n)*
     & x5y6z6*6.)*time
! endLoops()
      else if( dx.eq.1.and.dy.eq.0.and.dz.eq.1.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
c 
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
      y6=y5*y1
      y6z1=y5z1*y1
      y6z2=y5z2*y1
      y6z3=y5z3*y1
      y6z4=y5z4*y1
      y6z5=y5z5*y1
c 
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
      x1y6=x1y5*y1
      x1y6z1=x1y5z1*y1
      x1y6z2=x1y5z2*y1
      x1y6z3=x1y5z3*y1
      x1y6z4=x1y5z4*y1
      x1y6z5=x1y5z5*y1
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
      x2y6=x1y6*x1
      x2y6z1=x1y6z1*x1
      x2y6z2=x1y6z2*x1
      x2y6z3=x1y6z3*x1
      x2y6z4=x1y6z4*x1
      x2y6z5=x1y6z5*x1
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
      x3y6=x2y6*x1
      x3y6z1=x2y6z1*x1
      x3y6z2=x2y6z2*x1
      x3y6z3=x2y6z3*x1
      x3y6z4=x2y6z4*x1
      x3y6z5=x2y6z5*x1
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
      x4y6=x3y6*x1
      x4y6z1=x3y6z1*x1
      x4y6z2=x3y6z2*x1
      x4y6z3=x3y6z3*x1
      x4y6z4=x3y6z4*x1
      x4y6z5=x3y6z5*x1
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
      x5y6=x4y6*x1
      x5y6z1=x4y6z1*x1
      x5y6z2=x4y6z2*x1
      x5y6z3=x4y6z3*x1
      x5y6z4=x4y6z4*x1
      x5y6z5=x4y6z5*x1
      r=(c(1,0,1,n)+c(1,0,2,n)*z1*2.+c(1,0,3,n)*z2*3.+c(1,0,4,n)*z3*4.+
     & c(1,0,5,n)*z4*5.+c(1,0,6,n)*z5*6.+c(1,1,1,n)*y1+c(1,1,2,n)*
     & y1z1*2.+c(1,1,3,n)*y1z2*3.+c(1,1,4,n)*y1z3*4.+c(1,1,5,n)*y1z4*
     & 5.+c(1,1,6,n)*y1z5*6.+c(1,2,1,n)*y2+c(1,2,2,n)*y2z1*2.+c(1,2,3,
     & n)*y2z2*3.+c(1,2,4,n)*y2z3*4.+c(1,2,5,n)*y2z4*5.+c(1,2,6,n)*
     & y2z5*6.+c(1,3,1,n)*y3+c(1,3,2,n)*y3z1*2.+c(1,3,3,n)*y3z2*3.+c(
     & 1,3,4,n)*y3z3*4.+c(1,3,5,n)*y3z4*5.+c(1,3,6,n)*y3z5*6.+c(1,4,1,
     & n)*y4+c(1,4,2,n)*y4z1*2.+c(1,4,3,n)*y4z2*3.+c(1,4,4,n)*y4z3*4.+
     & c(1,4,5,n)*y4z4*5.+c(1,4,6,n)*y4z5*6.+c(1,5,1,n)*y5+c(1,5,2,n)*
     & y5z1*2.+c(1,5,3,n)*y5z2*3.+c(1,5,4,n)*y5z3*4.+c(1,5,5,n)*y5z4*
     & 5.+c(1,5,6,n)*y5z5*6.+c(1,6,1,n)*y6+c(1,6,2,n)*y6z1*2.+c(1,6,3,
     & n)*y6z2*3.+c(1,6,4,n)*y6z3*4.+c(1,6,5,n)*y6z4*5.+c(1,6,6,n)*
     & y6z5*6.+c(2,0,1,n)*x1*2.+c(2,0,2,n)*x1z1*4.+c(2,0,3,n)*x1z2*6.+
     & c(2,0,4,n)*x1z3*8.+c(2,0,5,n)*x1z4*10.+c(2,0,6,n)*x1z5*12.+c(2,
     & 1,1,n)*x1y1*2.+c(2,1,2,n)*x1y1z1*4.+c(2,1,3,n)*x1y1z2*6.+c(2,1,
     & 4,n)*x1y1z3*8.+c(2,1,5,n)*x1y1z4*10.+c(2,1,6,n)*x1y1z5*12.+c(2,
     & 2,1,n)*x1y2*2.+c(2,2,2,n)*x1y2z1*4.+c(2,2,3,n)*x1y2z2*6.+c(2,2,
     & 4,n)*x1y2z3*8.+c(2,2,5,n)*x1y2z4*10.+c(2,2,6,n)*x1y2z5*12.+c(2,
     & 3,1,n)*x1y3*2.+c(2,3,2,n)*x1y3z1*4.+c(2,3,3,n)*x1y3z2*6.+c(2,3,
     & 4,n)*x1y3z3*8.+c(2,3,5,n)*x1y3z4*10.+c(2,3,6,n)*x1y3z5*12.+c(2,
     & 4,1,n)*x1y4*2.+c(2,4,2,n)*x1y4z1*4.+c(2,4,3,n)*x1y4z2*6.+c(2,4,
     & 4,n)*x1y4z3*8.+c(2,4,5,n)*x1y4z4*10.+c(2,4,6,n)*x1y4z5*12.+c(2,
     & 5,1,n)*x1y5*2.+c(2,5,2,n)*x1y5z1*4.+c(2,5,3,n)*x1y5z2*6.+c(2,5,
     & 4,n)*x1y5z3*8.+c(2,5,5,n)*x1y5z4*10.+c(2,5,6,n)*x1y5z5*12.+c(2,
     & 6,1,n)*x1y6*2.+c(2,6,2,n)*x1y6z1*4.+c(2,6,3,n)*x1y6z2*6.+c(2,6,
     & 4,n)*x1y6z3*8.+c(2,6,5,n)*x1y6z4*10.+c(2,6,6,n)*x1y6z5*12.+c(3,
     & 0,1,n)*x2*3.+c(3,0,2,n)*x2z1*6.+c(3,0,3,n)*x2z2*9.+c(3,0,4,n)*
     & x2z3*12.+c(3,0,5,n)*x2z4*15.+c(3,0,6,n)*x2z5*18.+c(3,1,1,n)*
     & x2y1*3.+c(3,1,2,n)*x2y1z1*6.+c(3,1,3,n)*x2y1z2*9.+c(3,1,4,n)*
     & x2y1z3*12.+c(3,1,5,n)*x2y1z4*15.+c(3,1,6,n)*x2y1z5*18.+c(3,2,1,
     & n)*x2y2*3.+c(3,2,2,n)*x2y2z1*6.+c(3,2,3,n)*x2y2z2*9.+c(3,2,4,n)
     & *x2y2z3*12.+c(3,2,5,n)*x2y2z4*15.+c(3,2,6,n)*x2y2z5*18.+c(3,3,
     & 1,n)*x2y3*3.+c(3,3,2,n)*x2y3z1*6.+c(3,3,3,n)*x2y3z2*9.+c(3,3,4,
     & n)*x2y3z3*12.+c(3,3,5,n)*x2y3z4*15.+c(3,3,6,n)*x2y3z5*18.+c(3,
     & 4,1,n)*x2y4*3.+c(3,4,2,n)*x2y4z1*6.+c(3,4,3,n)*x2y4z2*9.+c(3,4,
     & 4,n)*x2y4z3*12.+c(3,4,5,n)*x2y4z4*15.+c(3,4,6,n)*x2y4z5*18.+c(
     & 3,5,1,n)*x2y5*3.+c(3,5,2,n)*x2y5z1*6.+c(3,5,3,n)*x2y5z2*9.+c(3,
     & 5,4,n)*x2y5z3*12.+c(3,5,5,n)*x2y5z4*15.+c(3,5,6,n)*x2y5z5*18.+
     & c(3,6,1,n)*x2y6*3.+c(3,6,2,n)*x2y6z1*6.+c(3,6,3,n)*x2y6z2*9.+c(
     & 3,6,4,n)*x2y6z3*12.+c(3,6,5,n)*x2y6z4*15.+c(3,6,6,n)*x2y6z5*
     & 18.+c(4,0,1,n)*x3*4.+c(4,0,2,n)*x3z1*8.+c(4,0,3,n)*x3z2*12.+c(
     & 4,0,4,n)*x3z3*16.+c(4,0,5,n)*x3z4*20.+c(4,0,6,n)*x3z5*24.+c(4,
     & 1,1,n)*x3y1*4.+c(4,1,2,n)*x3y1z1*8.+c(4,1,3,n)*x3y1z2*12.+c(4,
     & 1,4,n)*x3y1z3*16.+c(4,1,5,n)*x3y1z4*20.+c(4,1,6,n)*x3y1z5*24.+
     & c(4,2,1,n)*x3y2*4.+c(4,2,2,n)*x3y2z1*8.+c(4,2,3,n)*x3y2z2*12.+
     & c(4,2,4,n)*x3y2z3*16.+c(4,2,5,n)*x3y2z4*20.+c(4,2,6,n)*x3y2z5*
     & 24.+c(4,3,1,n)*x3y3*4.+c(4,3,2,n)*x3y3z1*8.+c(4,3,3,n)*x3y3z2*
     & 12.+c(4,3,4,n)*x3y3z3*16.+c(4,3,5,n)*x3y3z4*20.+c(4,3,6,n)*
     & x3y3z5*24.+c(4,4,1,n)*x3y4*4.+c(4,4,2,n)*x3y4z1*8.+c(4,4,3,n)*
     & x3y4z2*12.+c(4,4,4,n)*x3y4z3*16.+c(4,4,5,n)*x3y4z4*20.+c(4,4,6,
     & n)*x3y4z5*24.+c(4,5,1,n)*x3y5*4.+c(4,5,2,n)*x3y5z1*8.+c(4,5,3,
     & n)*x3y5z2*12.+c(4,5,4,n)*x3y5z3*16.+c(4,5,5,n)*x3y5z4*20.+c(4,
     & 5,6,n)*x3y5z5*24.+c(4,6,1,n)*x3y6*4.+c(4,6,2,n)*x3y6z1*8.+c(4,
     & 6,3,n)*x3y6z2*12.+c(4,6,4,n)*x3y6z3*16.+c(4,6,5,n)*x3y6z4*20.+
     & c(4,6,6,n)*x3y6z5*24.+c(5,0,1,n)*x4*5.+c(5,0,2,n)*x4z1*10.+c(5,
     & 0,3,n)*x4z2*15.+c(5,0,4,n)*x4z3*20.+c(5,0,5,n)*x4z4*25.+c(5,0,
     & 6,n)*x4z5*30.+c(5,1,1,n)*x4y1*5.+c(5,1,2,n)*x4y1z1*10.+c(5,1,3,
     & n)*x4y1z2*15.+c(5,1,4,n)*x4y1z3*20.+c(5,1,5,n)*x4y1z4*25.+c(5,
     & 1,6,n)*x4y1z5*30.+c(5,2,1,n)*x4y2*5.+c(5,2,2,n)*x4y2z1*10.+c(5,
     & 2,3,n)*x4y2z2*15.+c(5,2,4,n)*x4y2z3*20.+c(5,2,5,n)*x4y2z4*25.+
     & c(5,2,6,n)*x4y2z5*30.+c(5,3,1,n)*x4y3*5.+c(5,3,2,n)*x4y3z1*10.+
     & c(5,3,3,n)*x4y3z2*15.+c(5,3,4,n)*x4y3z3*20.+c(5,3,5,n)*x4y3z4*
     & 25.+c(5,3,6,n)*x4y3z5*30.+c(5,4,1,n)*x4y4*5.+c(5,4,2,n)*x4y4z1*
     & 10.+c(5,4,3,n)*x4y4z2*15.+c(5,4,4,n)*x4y4z3*20.+c(5,4,5,n)*
     & x4y4z4*25.+c(5,4,6,n)*x4y4z5*30.+c(5,5,1,n)*x4y5*5.+c(5,5,2,n)*
     & x4y5z1*10.+c(5,5,3,n)*x4y5z2*15.+c(5,5,4,n)*x4y5z3*20.+c(5,5,5,
     & n)*x4y5z4*25.+c(5,5,6,n)*x4y5z5*30.+c(5,6,1,n)*x4y6*5.+c(5,6,2,
     & n)*x4y6z1*10.+c(5,6,3,n)*x4y6z2*15.+c(5,6,4,n)*x4y6z3*20.+c(5,
     & 6,5,n)*x4y6z4*25.+c(5,6,6,n)*x4y6z5*30.+c(6,0,1,n)*x5*6.+c(6,0,
     & 2,n)*x5z1*12.+c(6,0,3,n)*x5z2*18.+c(6,0,4,n)*x5z3*24.+c(6,0,5,
     & n)*x5z4*30.+c(6,0,6,n)*x5z5*36.+c(6,1,1,n)*x5y1*6.+c(6,1,2,n)*
     & x5y1z1*12.+c(6,1,3,n)*x5y1z2*18.+c(6,1,4,n)*x5y1z3*24.+c(6,1,5,
     & n)*x5y1z4*30.+c(6,1,6,n)*x5y1z5*36.+c(6,2,1,n)*x5y2*6.+c(6,2,2,
     & n)*x5y2z1*12.+c(6,2,3,n)*x5y2z2*18.+c(6,2,4,n)*x5y2z3*24.+c(6,
     & 2,5,n)*x5y2z4*30.+c(6,2,6,n)*x5y2z5*36.+c(6,3,1,n)*x5y3*6.+c(6,
     & 3,2,n)*x5y3z1*12.+c(6,3,3,n)*x5y3z2*18.+c(6,3,4,n)*x5y3z3*24.+
     & c(6,3,5,n)*x5y3z4*30.+c(6,3,6,n)*x5y3z5*36.+c(6,4,1,n)*x5y4*6.+
     & c(6,4,2,n)*x5y4z1*12.)
       r=(r+c(6,4,3,n)*x5y4z2*18.+c(6,4,4,n)*x5y4z3*24.+c(6,4,5,n)*
     & x5y4z4*30.+c(6,4,6,n)*x5y4z5*36.+c(6,5,1,n)*x5y5*6.+c(6,5,2,n)*
     & x5y5z1*12.+c(6,5,3,n)*x5y5z2*18.+c(6,5,4,n)*x5y5z3*24.+c(6,5,5,
     & n)*x5y5z4*30.+c(6,5,6,n)*x5y5z5*36.+c(6,6,1,n)*x5y6*6.+c(6,6,2,
     & n)*x5y6z1*12.+c(6,6,3,n)*x5y6z2*18.+c(6,6,4,n)*x5y6z3*24.+c(6,
     & 6,5,n)*x5y6z4*30.+c(6,6,6,n)*x5y6z5*36.)*time
! endLoops()
      else if( dx.eq.1.and.dy.eq.0.and.dz.eq.2.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
c 
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
      y6=y5*y1
      y6z1=y5z1*y1
      y6z2=y5z2*y1
      y6z3=y5z3*y1
      y6z4=y5z4*y1
c 
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
      x1y6=x1y5*y1
      x1y6z1=x1y5z1*y1
      x1y6z2=x1y5z2*y1
      x1y6z3=x1y5z3*y1
      x1y6z4=x1y5z4*y1
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
      x2y6=x1y6*x1
      x2y6z1=x1y6z1*x1
      x2y6z2=x1y6z2*x1
      x2y6z3=x1y6z3*x1
      x2y6z4=x1y6z4*x1
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
      x3y6=x2y6*x1
      x3y6z1=x2y6z1*x1
      x3y6z2=x2y6z2*x1
      x3y6z3=x2y6z3*x1
      x3y6z4=x2y6z4*x1
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
      x4y6=x3y6*x1
      x4y6z1=x3y6z1*x1
      x4y6z2=x3y6z2*x1
      x4y6z3=x3y6z3*x1
      x4y6z4=x3y6z4*x1
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
      x5y6=x4y6*x1
      x5y6z1=x4y6z1*x1
      x5y6z2=x4y6z2*x1
      x5y6z3=x4y6z3*x1
      x5y6z4=x4y6z4*x1
      r=(c(1,0,2,n)*2.+c(1,0,3,n)*z1*6.+c(1,0,4,n)*z2*12.+c(1,0,5,n)*
     & z3*20.+c(1,0,6,n)*z4*30.+c(1,1,2,n)*y1*2.+c(1,1,3,n)*y1z1*6.+c(
     & 1,1,4,n)*y1z2*12.+c(1,1,5,n)*y1z3*20.+c(1,1,6,n)*y1z4*30.+c(1,
     & 2,2,n)*y2*2.+c(1,2,3,n)*y2z1*6.+c(1,2,4,n)*y2z2*12.+c(1,2,5,n)*
     & y2z3*20.+c(1,2,6,n)*y2z4*30.+c(1,3,2,n)*y3*2.+c(1,3,3,n)*y3z1*
     & 6.+c(1,3,4,n)*y3z2*12.+c(1,3,5,n)*y3z3*20.+c(1,3,6,n)*y3z4*30.+
     & c(1,4,2,n)*y4*2.+c(1,4,3,n)*y4z1*6.+c(1,4,4,n)*y4z2*12.+c(1,4,
     & 5,n)*y4z3*20.+c(1,4,6,n)*y4z4*30.+c(1,5,2,n)*y5*2.+c(1,5,3,n)*
     & y5z1*6.+c(1,5,4,n)*y5z2*12.+c(1,5,5,n)*y5z3*20.+c(1,5,6,n)*
     & y5z4*30.+c(1,6,2,n)*y6*2.+c(1,6,3,n)*y6z1*6.+c(1,6,4,n)*y6z2*
     & 12.+c(1,6,5,n)*y6z3*20.+c(1,6,6,n)*y6z4*30.+c(2,0,2,n)*x1*4.+c(
     & 2,0,3,n)*x1z1*12.+c(2,0,4,n)*x1z2*24.+c(2,0,5,n)*x1z3*40.+c(2,
     & 0,6,n)*x1z4*60.+c(2,1,2,n)*x1y1*4.+c(2,1,3,n)*x1y1z1*12.+c(2,1,
     & 4,n)*x1y1z2*24.+c(2,1,5,n)*x1y1z3*40.+c(2,1,6,n)*x1y1z4*60.+c(
     & 2,2,2,n)*x1y2*4.+c(2,2,3,n)*x1y2z1*12.+c(2,2,4,n)*x1y2z2*24.+c(
     & 2,2,5,n)*x1y2z3*40.+c(2,2,6,n)*x1y2z4*60.+c(2,3,2,n)*x1y3*4.+c(
     & 2,3,3,n)*x1y3z1*12.+c(2,3,4,n)*x1y3z2*24.+c(2,3,5,n)*x1y3z3*
     & 40.+c(2,3,6,n)*x1y3z4*60.+c(2,4,2,n)*x1y4*4.+c(2,4,3,n)*x1y4z1*
     & 12.+c(2,4,4,n)*x1y4z2*24.+c(2,4,5,n)*x1y4z3*40.+c(2,4,6,n)*
     & x1y4z4*60.+c(2,5,2,n)*x1y5*4.+c(2,5,3,n)*x1y5z1*12.+c(2,5,4,n)*
     & x1y5z2*24.+c(2,5,5,n)*x1y5z3*40.+c(2,5,6,n)*x1y5z4*60.+c(2,6,2,
     & n)*x1y6*4.+c(2,6,3,n)*x1y6z1*12.+c(2,6,4,n)*x1y6z2*24.+c(2,6,5,
     & n)*x1y6z3*40.+c(2,6,6,n)*x1y6z4*60.+c(3,0,2,n)*x2*6.+c(3,0,3,n)
     & *x2z1*18.+c(3,0,4,n)*x2z2*36.+c(3,0,5,n)*x2z3*60.+c(3,0,6,n)*
     & x2z4*90.+c(3,1,2,n)*x2y1*6.+c(3,1,3,n)*x2y1z1*18.+c(3,1,4,n)*
     & x2y1z2*36.+c(3,1,5,n)*x2y1z3*60.+c(3,1,6,n)*x2y1z4*90.+c(3,2,2,
     & n)*x2y2*6.+c(3,2,3,n)*x2y2z1*18.+c(3,2,4,n)*x2y2z2*36.+c(3,2,5,
     & n)*x2y2z3*60.+c(3,2,6,n)*x2y2z4*90.+c(3,3,2,n)*x2y3*6.+c(3,3,3,
     & n)*x2y3z1*18.+c(3,3,4,n)*x2y3z2*36.+c(3,3,5,n)*x2y3z3*60.+c(3,
     & 3,6,n)*x2y3z4*90.+c(3,4,2,n)*x2y4*6.+c(3,4,3,n)*x2y4z1*18.+c(3,
     & 4,4,n)*x2y4z2*36.+c(3,4,5,n)*x2y4z3*60.+c(3,4,6,n)*x2y4z4*90.+
     & c(3,5,2,n)*x2y5*6.+c(3,5,3,n)*x2y5z1*18.+c(3,5,4,n)*x2y5z2*36.+
     & c(3,5,5,n)*x2y5z3*60.+c(3,5,6,n)*x2y5z4*90.+c(3,6,2,n)*x2y6*6.+
     & c(3,6,3,n)*x2y6z1*18.+c(3,6,4,n)*x2y6z2*36.+c(3,6,5,n)*x2y6z3*
     & 60.+c(3,6,6,n)*x2y6z4*90.+c(4,0,2,n)*x3*8.+c(4,0,3,n)*x3z1*24.+
     & c(4,0,4,n)*x3z2*48.+c(4,0,5,n)*x3z3*80.+c(4,0,6,n)*x3z4*120.+c(
     & 4,1,2,n)*x3y1*8.+c(4,1,3,n)*x3y1z1*24.+c(4,1,4,n)*x3y1z2*48.+c(
     & 4,1,5,n)*x3y1z3*80.+c(4,1,6,n)*x3y1z4*120.+c(4,2,2,n)*x3y2*8.+
     & c(4,2,3,n)*x3y2z1*24.+c(4,2,4,n)*x3y2z2*48.+c(4,2,5,n)*x3y2z3*
     & 80.+c(4,2,6,n)*x3y2z4*120.+c(4,3,2,n)*x3y3*8.+c(4,3,3,n)*
     & x3y3z1*24.+c(4,3,4,n)*x3y3z2*48.+c(4,3,5,n)*x3y3z3*80.+c(4,3,6,
     & n)*x3y3z4*120.+c(4,4,2,n)*x3y4*8.+c(4,4,3,n)*x3y4z1*24.+c(4,4,
     & 4,n)*x3y4z2*48.+c(4,4,5,n)*x3y4z3*80.+c(4,4,6,n)*x3y4z4*120.+c(
     & 4,5,2,n)*x3y5*8.+c(4,5,3,n)*x3y5z1*24.+c(4,5,4,n)*x3y5z2*48.+c(
     & 4,5,5,n)*x3y5z3*80.+c(4,5,6,n)*x3y5z4*120.+c(4,6,2,n)*x3y6*8.+
     & c(4,6,3,n)*x3y6z1*24.+c(4,6,4,n)*x3y6z2*48.+c(4,6,5,n)*x3y6z3*
     & 80.+c(4,6,6,n)*x3y6z4*120.+c(5,0,2,n)*x4*10.+c(5,0,3,n)*x4z1*
     & 30.+c(5,0,4,n)*x4z2*60.+c(5,0,5,n)*x4z3*100.+c(5,0,6,n)*x4z4*
     & 150.+c(5,1,2,n)*x4y1*10.+c(5,1,3,n)*x4y1z1*30.+c(5,1,4,n)*
     & x4y1z2*60.+c(5,1,5,n)*x4y1z3*100.+c(5,1,6,n)*x4y1z4*150.+c(5,2,
     & 2,n)*x4y2*10.+c(5,2,3,n)*x4y2z1*30.+c(5,2,4,n)*x4y2z2*60.+c(5,
     & 2,5,n)*x4y2z3*100.+c(5,2,6,n)*x4y2z4*150.+c(5,3,2,n)*x4y3*10.+
     & c(5,3,3,n)*x4y3z1*30.+c(5,3,4,n)*x4y3z2*60.+c(5,3,5,n)*x4y3z3*
     & 100.+c(5,3,6,n)*x4y3z4*150.+c(5,4,2,n)*x4y4*10.+c(5,4,3,n)*
     & x4y4z1*30.+c(5,4,4,n)*x4y4z2*60.+c(5,4,5,n)*x4y4z3*100.+c(5,4,
     & 6,n)*x4y4z4*150.+c(5,5,2,n)*x4y5*10.+c(5,5,3,n)*x4y5z1*30.+c(5,
     & 5,4,n)*x4y5z2*60.+c(5,5,5,n)*x4y5z3*100.+c(5,5,6,n)*x4y5z4*
     & 150.+c(5,6,2,n)*x4y6*10.+c(5,6,3,n)*x4y6z1*30.+c(5,6,4,n)*
     & x4y6z2*60.+c(5,6,5,n)*x4y6z3*100.+c(5,6,6,n)*x4y6z4*150.+c(6,0,
     & 2,n)*x5*12.+c(6,0,3,n)*x5z1*36.+c(6,0,4,n)*x5z2*72.+c(6,0,5,n)*
     & x5z3*120.+c(6,0,6,n)*x5z4*180.+c(6,1,2,n)*x5y1*12.+c(6,1,3,n)*
     & x5y1z1*36.+c(6,1,4,n)*x5y1z2*72.+c(6,1,5,n)*x5y1z3*120.+c(6,1,
     & 6,n)*x5y1z4*180.+c(6,2,2,n)*x5y2*12.+c(6,2,3,n)*x5y2z1*36.+c(6,
     & 2,4,n)*x5y2z2*72.+c(6,2,5,n)*x5y2z3*120.+c(6,2,6,n)*x5y2z4*
     & 180.+c(6,3,2,n)*x5y3*12.+c(6,3,3,n)*x5y3z1*36.+c(6,3,4,n)*
     & x5y3z2*72.+c(6,3,5,n)*x5y3z3*120.+c(6,3,6,n)*x5y3z4*180.+c(6,4,
     & 2,n)*x5y4*12.+c(6,4,3,n)*x5y4z1*36.+c(6,4,4,n)*x5y4z2*72.+c(6,
     & 4,5,n)*x5y4z3*120.+c(6,4,6,n)*x5y4z4*180.+c(6,5,2,n)*x5y5*12.+
     & c(6,5,3,n)*x5y5z1*36.+c(6,5,4,n)*x5y5z2*72.+c(6,5,5,n)*x5y5z3*
     & 120.+c(6,5,6,n)*x5y5z4*180.+c(6,6,2,n)*x5y6*12.+c(6,6,3,n)*
     & x5y6z1*36.+c(6,6,4,n)*x5y6z2*72.+c(6,6,5,n)*x5y6z3*120.+c(6,6,
     & 6,n)*x5y6z4*180.)*time
! endLoops()
      else if( dx.eq.1.and.dy.eq.1.and.dz.eq.0.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
      z6=z5*z1
c 
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y1z5=y1z4*z1
      y1z6=y1z5*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
      y2z5=y1z5*y1
      y2z6=y1z6*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      y3z4=y2z4*y1
      y3z5=y2z5*y1
      y3z6=y2z6*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y4z2=y3z2*y1
      y4z3=y3z3*y1
      y4z4=y3z4*y1
      y4z5=y3z5*y1
      y4z6=y3z6*y1
      y5=y4*y1
      y5z1=y4z1*y1
      y5z2=y4z2*y1
      y5z3=y4z3*y1
      y5z4=y4z4*y1
      y5z5=y4z5*y1
      y5z6=y4z6*y1
c 
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1z5=x1z4*z1
      x1z6=x1z5*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y1z5=x1y1z4*z1
      x1y1z6=x1y1z5*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y2z4=x1y1z4*y1
      x1y2z5=x1y1z5*y1
      x1y2z6=x1y1z6*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x1y3z4=x1y2z4*y1
      x1y3z5=x1y2z5*y1
      x1y3z6=x1y2z6*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y4z2=x1y3z2*y1
      x1y4z3=x1y3z3*y1
      x1y4z4=x1y3z4*y1
      x1y4z5=x1y3z5*y1
      x1y4z6=x1y3z6*y1
      x1y5=x1y4*y1
      x1y5z1=x1y4z1*y1
      x1y5z2=x1y4z2*y1
      x1y5z3=x1y4z3*y1
      x1y5z4=x1y4z4*y1
      x1y5z5=x1y4z5*y1
      x1y5z6=x1y4z6*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x2z5=x1z5*x1
      x2z6=x1z6*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y1z4=x1y1z4*x1
      x2y1z5=x1y1z5*x1
      x2y1z6=x1y1z6*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y2z4=x1y2z4*x1
      x2y2z5=x1y2z5*x1
      x2y2z6=x1y2z6*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y3z3=x1y3z3*x1
      x2y3z4=x1y3z4*x1
      x2y3z5=x1y3z5*x1
      x2y3z6=x1y3z6*x1
      x2y4=x1y4*x1
      x2y4z1=x1y4z1*x1
      x2y4z2=x1y4z2*x1
      x2y4z3=x1y4z3*x1
      x2y4z4=x1y4z4*x1
      x2y4z5=x1y4z5*x1
      x2y4z6=x1y4z6*x1
      x2y5=x1y5*x1
      x2y5z1=x1y5z1*x1
      x2y5z2=x1y5z2*x1
      x2y5z3=x1y5z3*x1
      x2y5z4=x1y5z4*x1
      x2y5z5=x1y5z5*x1
      x2y5z6=x1y5z6*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3z4=x2z4*x1
      x3z5=x2z5*x1
      x3z6=x2z6*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x3y1z4=x2y1z4*x1
      x3y1z5=x2y1z5*x1
      x3y1z6=x2y1z6*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y2z3=x2y2z3*x1
      x3y2z4=x2y2z4*x1
      x3y2z5=x2y2z5*x1
      x3y2z6=x2y2z6*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x3y3z2=x2y3z2*x1
      x3y3z3=x2y3z3*x1
      x3y3z4=x2y3z4*x1
      x3y3z5=x2y3z5*x1
      x3y3z6=x2y3z6*x1
      x3y4=x2y4*x1
      x3y4z1=x2y4z1*x1
      x3y4z2=x2y4z2*x1
      x3y4z3=x2y4z3*x1
      x3y4z4=x2y4z4*x1
      x3y4z5=x2y4z5*x1
      x3y4z6=x2y4z6*x1
      x3y5=x2y5*x1
      x3y5z1=x2y5z1*x1
      x3y5z2=x2y5z2*x1
      x3y5z3=x2y5z3*x1
      x3y5z4=x2y5z4*x1
      x3y5z5=x2y5z5*x1
      x3y5z6=x2y5z6*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4z2=x3z2*x1
      x4z3=x3z3*x1
      x4z4=x3z4*x1
      x4z5=x3z5*x1
      x4z6=x3z6*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y1z2=x3y1z2*x1
      x4y1z3=x3y1z3*x1
      x4y1z4=x3y1z4*x1
      x4y1z5=x3y1z5*x1
      x4y1z6=x3y1z6*x1
      x4y2=x3y2*x1
      x4y2z1=x3y2z1*x1
      x4y2z2=x3y2z2*x1
      x4y2z3=x3y2z3*x1
      x4y2z4=x3y2z4*x1
      x4y2z5=x3y2z5*x1
      x4y2z6=x3y2z6*x1
      x4y3=x3y3*x1
      x4y3z1=x3y3z1*x1
      x4y3z2=x3y3z2*x1
      x4y3z3=x3y3z3*x1
      x4y3z4=x3y3z4*x1
      x4y3z5=x3y3z5*x1
      x4y3z6=x3y3z6*x1
      x4y4=x3y4*x1
      x4y4z1=x3y4z1*x1
      x4y4z2=x3y4z2*x1
      x4y4z3=x3y4z3*x1
      x4y4z4=x3y4z4*x1
      x4y4z5=x3y4z5*x1
      x4y4z6=x3y4z6*x1
      x4y5=x3y5*x1
      x4y5z1=x3y5z1*x1
      x4y5z2=x3y5z2*x1
      x4y5z3=x3y5z3*x1
      x4y5z4=x3y5z4*x1
      x4y5z5=x3y5z5*x1
      x4y5z6=x3y5z6*x1
      x5=x4*x1
      x5z1=x4z1*x1
      x5z2=x4z2*x1
      x5z3=x4z3*x1
      x5z4=x4z4*x1
      x5z5=x4z5*x1
      x5z6=x4z6*x1
      x5y1=x4y1*x1
      x5y1z1=x4y1z1*x1
      x5y1z2=x4y1z2*x1
      x5y1z3=x4y1z3*x1
      x5y1z4=x4y1z4*x1
      x5y1z5=x4y1z5*x1
      x5y1z6=x4y1z6*x1
      x5y2=x4y2*x1
      x5y2z1=x4y2z1*x1
      x5y2z2=x4y2z2*x1
      x5y2z3=x4y2z3*x1
      x5y2z4=x4y2z4*x1
      x5y2z5=x4y2z5*x1
      x5y2z6=x4y2z6*x1
      x5y3=x4y3*x1
      x5y3z1=x4y3z1*x1
      x5y3z2=x4y3z2*x1
      x5y3z3=x4y3z3*x1
      x5y3z4=x4y3z4*x1
      x5y3z5=x4y3z5*x1
      x5y3z6=x4y3z6*x1
      x5y4=x4y4*x1
      x5y4z1=x4y4z1*x1
      x5y4z2=x4y4z2*x1
      x5y4z3=x4y4z3*x1
      x5y4z4=x4y4z4*x1
      x5y4z5=x4y4z5*x1
      x5y4z6=x4y4z6*x1
      x5y5=x4y5*x1
      x5y5z1=x4y5z1*x1
      x5y5z2=x4y5z2*x1
      x5y5z3=x4y5z3*x1
      x5y5z4=x4y5z4*x1
      x5y5z5=x4y5z5*x1
      x5y5z6=x4y5z6*x1
      r=(c(1,1,0,n)+c(1,1,1,n)*z1+c(1,1,2,n)*z2+c(1,1,3,n)*z3+c(1,1,4,
     & n)*z4+c(1,1,5,n)*z5+c(1,1,6,n)*z6+c(1,2,0,n)*y1*2.+c(1,2,1,n)*
     & y1z1*2.+c(1,2,2,n)*y1z2*2.+c(1,2,3,n)*y1z3*2.+c(1,2,4,n)*y1z4*
     & 2.+c(1,2,5,n)*y1z5*2.+c(1,2,6,n)*y1z6*2.+c(1,3,0,n)*y2*3.+c(1,
     & 3,1,n)*y2z1*3.+c(1,3,2,n)*y2z2*3.+c(1,3,3,n)*y2z3*3.+c(1,3,4,n)
     & *y2z4*3.+c(1,3,5,n)*y2z5*3.+c(1,3,6,n)*y2z6*3.+c(1,4,0,n)*y3*
     & 4.+c(1,4,1,n)*y3z1*4.+c(1,4,2,n)*y3z2*4.+c(1,4,3,n)*y3z3*4.+c(
     & 1,4,4,n)*y3z4*4.+c(1,4,5,n)*y3z5*4.+c(1,4,6,n)*y3z6*4.+c(1,5,0,
     & n)*y4*5.+c(1,5,1,n)*y4z1*5.+c(1,5,2,n)*y4z2*5.+c(1,5,3,n)*y4z3*
     & 5.+c(1,5,4,n)*y4z4*5.+c(1,5,5,n)*y4z5*5.+c(1,5,6,n)*y4z6*5.+c(
     & 1,6,0,n)*y5*6.+c(1,6,1,n)*y5z1*6.+c(1,6,2,n)*y5z2*6.+c(1,6,3,n)
     & *y5z3*6.+c(1,6,4,n)*y5z4*6.+c(1,6,5,n)*y5z5*6.+c(1,6,6,n)*y5z6*
     & 6.+c(2,1,0,n)*x1*2.+c(2,1,1,n)*x1z1*2.+c(2,1,2,n)*x1z2*2.+c(2,
     & 1,3,n)*x1z3*2.+c(2,1,4,n)*x1z4*2.+c(2,1,5,n)*x1z5*2.+c(2,1,6,n)
     & *x1z6*2.+c(2,2,0,n)*x1y1*4.+c(2,2,1,n)*x1y1z1*4.+c(2,2,2,n)*
     & x1y1z2*4.+c(2,2,3,n)*x1y1z3*4.+c(2,2,4,n)*x1y1z4*4.+c(2,2,5,n)*
     & x1y1z5*4.+c(2,2,6,n)*x1y1z6*4.+c(2,3,0,n)*x1y2*6.+c(2,3,1,n)*
     & x1y2z1*6.+c(2,3,2,n)*x1y2z2*6.+c(2,3,3,n)*x1y2z3*6.+c(2,3,4,n)*
     & x1y2z4*6.+c(2,3,5,n)*x1y2z5*6.+c(2,3,6,n)*x1y2z6*6.+c(2,4,0,n)*
     & x1y3*8.+c(2,4,1,n)*x1y3z1*8.+c(2,4,2,n)*x1y3z2*8.+c(2,4,3,n)*
     & x1y3z3*8.+c(2,4,4,n)*x1y3z4*8.+c(2,4,5,n)*x1y3z5*8.+c(2,4,6,n)*
     & x1y3z6*8.+c(2,5,0,n)*x1y4*10.+c(2,5,1,n)*x1y4z1*10.+c(2,5,2,n)*
     & x1y4z2*10.+c(2,5,3,n)*x1y4z3*10.+c(2,5,4,n)*x1y4z4*10.+c(2,5,5,
     & n)*x1y4z5*10.+c(2,5,6,n)*x1y4z6*10.+c(2,6,0,n)*x1y5*12.+c(2,6,
     & 1,n)*x1y5z1*12.+c(2,6,2,n)*x1y5z2*12.+c(2,6,3,n)*x1y5z3*12.+c(
     & 2,6,4,n)*x1y5z4*12.+c(2,6,5,n)*x1y5z5*12.+c(2,6,6,n)*x1y5z6*
     & 12.+c(3,1,0,n)*x2*3.+c(3,1,1,n)*x2z1*3.+c(3,1,2,n)*x2z2*3.+c(3,
     & 1,3,n)*x2z3*3.+c(3,1,4,n)*x2z4*3.+c(3,1,5,n)*x2z5*3.+c(3,1,6,n)
     & *x2z6*3.+c(3,2,0,n)*x2y1*6.+c(3,2,1,n)*x2y1z1*6.+c(3,2,2,n)*
     & x2y1z2*6.+c(3,2,3,n)*x2y1z3*6.+c(3,2,4,n)*x2y1z4*6.+c(3,2,5,n)*
     & x2y1z5*6.+c(3,2,6,n)*x2y1z6*6.+c(3,3,0,n)*x2y2*9.+c(3,3,1,n)*
     & x2y2z1*9.+c(3,3,2,n)*x2y2z2*9.+c(3,3,3,n)*x2y2z3*9.+c(3,3,4,n)*
     & x2y2z4*9.+c(3,3,5,n)*x2y2z5*9.+c(3,3,6,n)*x2y2z6*9.+c(3,4,0,n)*
     & x2y3*12.+c(3,4,1,n)*x2y3z1*12.+c(3,4,2,n)*x2y3z2*12.+c(3,4,3,n)
     & *x2y3z3*12.+c(3,4,4,n)*x2y3z4*12.+c(3,4,5,n)*x2y3z5*12.+c(3,4,
     & 6,n)*x2y3z6*12.+c(3,5,0,n)*x2y4*15.+c(3,5,1,n)*x2y4z1*15.+c(3,
     & 5,2,n)*x2y4z2*15.+c(3,5,3,n)*x2y4z3*15.+c(3,5,4,n)*x2y4z4*15.+
     & c(3,5,5,n)*x2y4z5*15.+c(3,5,6,n)*x2y4z6*15.+c(3,6,0,n)*x2y5*
     & 18.+c(3,6,1,n)*x2y5z1*18.+c(3,6,2,n)*x2y5z2*18.+c(3,6,3,n)*
     & x2y5z3*18.+c(3,6,4,n)*x2y5z4*18.+c(3,6,5,n)*x2y5z5*18.+c(3,6,6,
     & n)*x2y5z6*18.+c(4,1,0,n)*x3*4.+c(4,1,1,n)*x3z1*4.+c(4,1,2,n)*
     & x3z2*4.+c(4,1,3,n)*x3z3*4.+c(4,1,4,n)*x3z4*4.+c(4,1,5,n)*x3z5*
     & 4.+c(4,1,6,n)*x3z6*4.+c(4,2,0,n)*x3y1*8.+c(4,2,1,n)*x3y1z1*8.+
     & c(4,2,2,n)*x3y1z2*8.+c(4,2,3,n)*x3y1z3*8.+c(4,2,4,n)*x3y1z4*8.+
     & c(4,2,5,n)*x3y1z5*8.+c(4,2,6,n)*x3y1z6*8.+c(4,3,0,n)*x3y2*12.+
     & c(4,3,1,n)*x3y2z1*12.+c(4,3,2,n)*x3y2z2*12.+c(4,3,3,n)*x3y2z3*
     & 12.+c(4,3,4,n)*x3y2z4*12.+c(4,3,5,n)*x3y2z5*12.+c(4,3,6,n)*
     & x3y2z6*12.+c(4,4,0,n)*x3y3*16.+c(4,4,1,n)*x3y3z1*16.+c(4,4,2,n)
     & *x3y3z2*16.+c(4,4,3,n)*x3y3z3*16.+c(4,4,4,n)*x3y3z4*16.+c(4,4,
     & 5,n)*x3y3z5*16.+c(4,4,6,n)*x3y3z6*16.+c(4,5,0,n)*x3y4*20.+c(4,
     & 5,1,n)*x3y4z1*20.+c(4,5,2,n)*x3y4z2*20.+c(4,5,3,n)*x3y4z3*20.+
     & c(4,5,4,n)*x3y4z4*20.+c(4,5,5,n)*x3y4z5*20.+c(4,5,6,n)*x3y4z6*
     & 20.+c(4,6,0,n)*x3y5*24.+c(4,6,1,n)*x3y5z1*24.+c(4,6,2,n)*
     & x3y5z2*24.+c(4,6,3,n)*x3y5z3*24.+c(4,6,4,n)*x3y5z4*24.+c(4,6,5,
     & n)*x3y5z5*24.+c(4,6,6,n)*x3y5z6*24.+c(5,1,0,n)*x4*5.+c(5,1,1,n)
     & *x4z1*5.+c(5,1,2,n)*x4z2*5.+c(5,1,3,n)*x4z3*5.+c(5,1,4,n)*x4z4*
     & 5.+c(5,1,5,n)*x4z5*5.+c(5,1,6,n)*x4z6*5.+c(5,2,0,n)*x4y1*10.+c(
     & 5,2,1,n)*x4y1z1*10.+c(5,2,2,n)*x4y1z2*10.+c(5,2,3,n)*x4y1z3*
     & 10.+c(5,2,4,n)*x4y1z4*10.+c(5,2,5,n)*x4y1z5*10.+c(5,2,6,n)*
     & x4y1z6*10.+c(5,3,0,n)*x4y2*15.+c(5,3,1,n)*x4y2z1*15.+c(5,3,2,n)
     & *x4y2z2*15.+c(5,3,3,n)*x4y2z3*15.+c(5,3,4,n)*x4y2z4*15.+c(5,3,
     & 5,n)*x4y2z5*15.+c(5,3,6,n)*x4y2z6*15.+c(5,4,0,n)*x4y3*20.+c(5,
     & 4,1,n)*x4y3z1*20.+c(5,4,2,n)*x4y3z2*20.+c(5,4,3,n)*x4y3z3*20.+
     & c(5,4,4,n)*x4y3z4*20.+c(5,4,5,n)*x4y3z5*20.+c(5,4,6,n)*x4y3z6*
     & 20.+c(5,5,0,n)*x4y4*25.+c(5,5,1,n)*x4y4z1*25.+c(5,5,2,n)*
     & x4y4z2*25.+c(5,5,3,n)*x4y4z3*25.+c(5,5,4,n)*x4y4z4*25.+c(5,5,5,
     & n)*x4y4z5*25.+c(5,5,6,n)*x4y4z6*25.+c(5,6,0,n)*x4y5*30.+c(5,6,
     & 1,n)*x4y5z1*30.+c(5,6,2,n)*x4y5z2*30.+c(5,6,3,n)*x4y5z3*30.+c(
     & 5,6,4,n)*x4y5z4*30.+c(5,6,5,n)*x4y5z5*30.+c(5,6,6,n)*x4y5z6*
     & 30.+c(6,1,0,n)*x5*6.+c(6,1,1,n)*x5z1*6.+c(6,1,2,n)*x5z2*6.+c(6,
     & 1,3,n)*x5z3*6.+c(6,1,4,n)*x5z4*6.+c(6,1,5,n)*x5z5*6.+c(6,1,6,n)
     & *x5z6*6.+c(6,2,0,n)*x5y1*12.+c(6,2,1,n)*x5y1z1*12.+c(6,2,2,n)*
     & x5y1z2*12.+c(6,2,3,n)*x5y1z3*12.+c(6,2,4,n)*x5y1z4*12.+c(6,2,5,
     & n)*x5y1z5*12.+c(6,2,6,n)*x5y1z6*12.+c(6,3,0,n)*x5y2*18.+c(6,3,
     & 1,n)*x5y2z1*18.+c(6,3,2,n)*x5y2z2*18.+c(6,3,3,n)*x5y2z3*18.+c(
     & 6,3,4,n)*x5y2z4*18.+c(6,3,5,n)*x5y2z5*18.+c(6,3,6,n)*x5y2z6*
     & 18.+c(6,4,0,n)*x5y3*24.+c(6,4,1,n)*x5y3z1*24.+c(6,4,2,n)*
     & x5y3z2*24.+c(6,4,3,n)*x5y3z3*24.+c(6,4,4,n)*x5y3z4*24.)
       r=(r+c(6,4,5,n)*x5y3z5*24.+c(6,4,6,n)*x5y3z6*24.+c(6,5,0,n)*
     & x5y4*30.+c(6,5,1,n)*x5y4z1*30.+c(6,5,2,n)*x5y4z2*30.+c(6,5,3,n)
     & *x5y4z3*30.+c(6,5,4,n)*x5y4z4*30.+c(6,5,5,n)*x5y4z5*30.+c(6,5,
     & 6,n)*x5y4z6*30.+c(6,6,0,n)*x5y5*36.+c(6,6,1,n)*x5y5z1*36.+c(6,
     & 6,2,n)*x5y5z2*36.+c(6,6,3,n)*x5y5z3*36.+c(6,6,4,n)*x5y5z4*36.+
     & c(6,6,5,n)*x5y5z5*36.+c(6,6,6,n)*x5y5z6*36.)*time
! endLoops()
      else if( dx.eq.1.and.dy.eq.1.and.dz.eq.1.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
c 
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
c 
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
      r=(c(1,1,1,n)+c(1,1,2,n)*z1*2.+c(1,1,3,n)*z2*3.+c(1,1,4,n)*z3*4.+
     & c(1,1,5,n)*z4*5.+c(1,1,6,n)*z5*6.+c(1,2,1,n)*y1*2.+c(1,2,2,n)*
     & y1z1*4.+c(1,2,3,n)*y1z2*6.+c(1,2,4,n)*y1z3*8.+c(1,2,5,n)*y1z4*
     & 10.+c(1,2,6,n)*y1z5*12.+c(1,3,1,n)*y2*3.+c(1,3,2,n)*y2z1*6.+c(
     & 1,3,3,n)*y2z2*9.+c(1,3,4,n)*y2z3*12.+c(1,3,5,n)*y2z4*15.+c(1,3,
     & 6,n)*y2z5*18.+c(1,4,1,n)*y3*4.+c(1,4,2,n)*y3z1*8.+c(1,4,3,n)*
     & y3z2*12.+c(1,4,4,n)*y3z3*16.+c(1,4,5,n)*y3z4*20.+c(1,4,6,n)*
     & y3z5*24.+c(1,5,1,n)*y4*5.+c(1,5,2,n)*y4z1*10.+c(1,5,3,n)*y4z2*
     & 15.+c(1,5,4,n)*y4z3*20.+c(1,5,5,n)*y4z4*25.+c(1,5,6,n)*y4z5*
     & 30.+c(1,6,1,n)*y5*6.+c(1,6,2,n)*y5z1*12.+c(1,6,3,n)*y5z2*18.+c(
     & 1,6,4,n)*y5z3*24.+c(1,6,5,n)*y5z4*30.+c(1,6,6,n)*y5z5*36.+c(2,
     & 1,1,n)*x1*2.+c(2,1,2,n)*x1z1*4.+c(2,1,3,n)*x1z2*6.+c(2,1,4,n)*
     & x1z3*8.+c(2,1,5,n)*x1z4*10.+c(2,1,6,n)*x1z5*12.+c(2,2,1,n)*
     & x1y1*4.+c(2,2,2,n)*x1y1z1*8.+c(2,2,3,n)*x1y1z2*12.+c(2,2,4,n)*
     & x1y1z3*16.+c(2,2,5,n)*x1y1z4*20.+c(2,2,6,n)*x1y1z5*24.+c(2,3,1,
     & n)*x1y2*6.+c(2,3,2,n)*x1y2z1*12.+c(2,3,3,n)*x1y2z2*18.+c(2,3,4,
     & n)*x1y2z3*24.+c(2,3,5,n)*x1y2z4*30.+c(2,3,6,n)*x1y2z5*36.+c(2,
     & 4,1,n)*x1y3*8.+c(2,4,2,n)*x1y3z1*16.+c(2,4,3,n)*x1y3z2*24.+c(2,
     & 4,4,n)*x1y3z3*32.+c(2,4,5,n)*x1y3z4*40.+c(2,4,6,n)*x1y3z5*48.+
     & c(2,5,1,n)*x1y4*10.+c(2,5,2,n)*x1y4z1*20.+c(2,5,3,n)*x1y4z2*
     & 30.+c(2,5,4,n)*x1y4z3*40.+c(2,5,5,n)*x1y4z4*50.+c(2,5,6,n)*
     & x1y4z5*60.+c(2,6,1,n)*x1y5*12.+c(2,6,2,n)*x1y5z1*24.+c(2,6,3,n)
     & *x1y5z2*36.+c(2,6,4,n)*x1y5z3*48.+c(2,6,5,n)*x1y5z4*60.+c(2,6,
     & 6,n)*x1y5z5*72.+c(3,1,1,n)*x2*3.+c(3,1,2,n)*x2z1*6.+c(3,1,3,n)*
     & x2z2*9.+c(3,1,4,n)*x2z3*12.+c(3,1,5,n)*x2z4*15.+c(3,1,6,n)*
     & x2z5*18.+c(3,2,1,n)*x2y1*6.+c(3,2,2,n)*x2y1z1*12.+c(3,2,3,n)*
     & x2y1z2*18.+c(3,2,4,n)*x2y1z3*24.+c(3,2,5,n)*x2y1z4*30.+c(3,2,6,
     & n)*x2y1z5*36.+c(3,3,1,n)*x2y2*9.+c(3,3,2,n)*x2y2z1*18.+c(3,3,3,
     & n)*x2y2z2*27.+c(3,3,4,n)*x2y2z3*36.+c(3,3,5,n)*x2y2z4*45.+c(3,
     & 3,6,n)*x2y2z5*54.+c(3,4,1,n)*x2y3*12.+c(3,4,2,n)*x2y3z1*24.+c(
     & 3,4,3,n)*x2y3z2*36.+c(3,4,4,n)*x2y3z3*48.+c(3,4,5,n)*x2y3z4*
     & 60.+c(3,4,6,n)*x2y3z5*72.+c(3,5,1,n)*x2y4*15.+c(3,5,2,n)*
     & x2y4z1*30.+c(3,5,3,n)*x2y4z2*45.+c(3,5,4,n)*x2y4z3*60.+c(3,5,5,
     & n)*x2y4z4*75.+c(3,5,6,n)*x2y4z5*90.+c(3,6,1,n)*x2y5*18.+c(3,6,
     & 2,n)*x2y5z1*36.+c(3,6,3,n)*x2y5z2*54.+c(3,6,4,n)*x2y5z3*72.+c(
     & 3,6,5,n)*x2y5z4*90.+c(3,6,6,n)*x2y5z5*108.+c(4,1,1,n)*x3*4.+c(
     & 4,1,2,n)*x3z1*8.+c(4,1,3,n)*x3z2*12.+c(4,1,4,n)*x3z3*16.+c(4,1,
     & 5,n)*x3z4*20.+c(4,1,6,n)*x3z5*24.+c(4,2,1,n)*x3y1*8.+c(4,2,2,n)
     & *x3y1z1*16.+c(4,2,3,n)*x3y1z2*24.+c(4,2,4,n)*x3y1z3*32.+c(4,2,
     & 5,n)*x3y1z4*40.+c(4,2,6,n)*x3y1z5*48.+c(4,3,1,n)*x3y2*12.+c(4,
     & 3,2,n)*x3y2z1*24.+c(4,3,3,n)*x3y2z2*36.+c(4,3,4,n)*x3y2z3*48.+
     & c(4,3,5,n)*x3y2z4*60.+c(4,3,6,n)*x3y2z5*72.+c(4,4,1,n)*x3y3*
     & 16.+c(4,4,2,n)*x3y3z1*32.+c(4,4,3,n)*x3y3z2*48.+c(4,4,4,n)*
     & x3y3z3*64.+c(4,4,5,n)*x3y3z4*80.+c(4,4,6,n)*x3y3z5*96.+c(4,5,1,
     & n)*x3y4*20.+c(4,5,2,n)*x3y4z1*40.+c(4,5,3,n)*x3y4z2*60.+c(4,5,
     & 4,n)*x3y4z3*80.+c(4,5,5,n)*x3y4z4*100.+c(4,5,6,n)*x3y4z5*120.+
     & c(4,6,1,n)*x3y5*24.+c(4,6,2,n)*x3y5z1*48.+c(4,6,3,n)*x3y5z2*
     & 72.+c(4,6,4,n)*x3y5z3*96.+c(4,6,5,n)*x3y5z4*120.+c(4,6,6,n)*
     & x3y5z5*144.+c(5,1,1,n)*x4*5.+c(5,1,2,n)*x4z1*10.+c(5,1,3,n)*
     & x4z2*15.+c(5,1,4,n)*x4z3*20.+c(5,1,5,n)*x4z4*25.+c(5,1,6,n)*
     & x4z5*30.+c(5,2,1,n)*x4y1*10.+c(5,2,2,n)*x4y1z1*20.+c(5,2,3,n)*
     & x4y1z2*30.+c(5,2,4,n)*x4y1z3*40.+c(5,2,5,n)*x4y1z4*50.+c(5,2,6,
     & n)*x4y1z5*60.+c(5,3,1,n)*x4y2*15.+c(5,3,2,n)*x4y2z1*30.+c(5,3,
     & 3,n)*x4y2z2*45.+c(5,3,4,n)*x4y2z3*60.+c(5,3,5,n)*x4y2z4*75.+c(
     & 5,3,6,n)*x4y2z5*90.+c(5,4,1,n)*x4y3*20.+c(5,4,2,n)*x4y3z1*40.+
     & c(5,4,3,n)*x4y3z2*60.+c(5,4,4,n)*x4y3z3*80.+c(5,4,5,n)*x4y3z4*
     & 100.+c(5,4,6,n)*x4y3z5*120.+c(5,5,1,n)*x4y4*25.+c(5,5,2,n)*
     & x4y4z1*50.+c(5,5,3,n)*x4y4z2*75.+c(5,5,4,n)*x4y4z3*100.+c(5,5,
     & 5,n)*x4y4z4*125.+c(5,5,6,n)*x4y4z5*150.+c(5,6,1,n)*x4y5*30.+c(
     & 5,6,2,n)*x4y5z1*60.+c(5,6,3,n)*x4y5z2*90.+c(5,6,4,n)*x4y5z3*
     & 120.+c(5,6,5,n)*x4y5z4*150.+c(5,6,6,n)*x4y5z5*180.+c(6,1,1,n)*
     & x5*6.+c(6,1,2,n)*x5z1*12.+c(6,1,3,n)*x5z2*18.+c(6,1,4,n)*x5z3*
     & 24.+c(6,1,5,n)*x5z4*30.+c(6,1,6,n)*x5z5*36.+c(6,2,1,n)*x5y1*
     & 12.+c(6,2,2,n)*x5y1z1*24.+c(6,2,3,n)*x5y1z2*36.+c(6,2,4,n)*
     & x5y1z3*48.+c(6,2,5,n)*x5y1z4*60.+c(6,2,6,n)*x5y1z5*72.+c(6,3,1,
     & n)*x5y2*18.+c(6,3,2,n)*x5y2z1*36.+c(6,3,3,n)*x5y2z2*54.+c(6,3,
     & 4,n)*x5y2z3*72.+c(6,3,5,n)*x5y2z4*90.+c(6,3,6,n)*x5y2z5*108.+c(
     & 6,4,1,n)*x5y3*24.+c(6,4,2,n)*x5y3z1*48.+c(6,4,3,n)*x5y3z2*72.+
     & c(6,4,4,n)*x5y3z3*96.+c(6,4,5,n)*x5y3z4*120.+c(6,4,6,n)*x5y3z5*
     & 144.+c(6,5,1,n)*x5y4*30.+c(6,5,2,n)*x5y4z1*60.+c(6,5,3,n)*
     & x5y4z2*90.+c(6,5,4,n)*x5y4z3*120.+c(6,5,5,n)*x5y4z4*150.+c(6,5,
     & 6,n)*x5y4z5*180.+c(6,6,1,n)*x5y5*36.+c(6,6,2,n)*x5y5z1*72.+c(6,
     & 6,3,n)*x5y5z2*108.+c(6,6,4,n)*x5y5z3*144.+c(6,6,5,n)*x5y5z4*
     & 180.+c(6,6,6,n)*x5y5z5*216.)*time
! endLoops()
      else if( dx.eq.1.and.dy.eq.1.and.dz.eq.2.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
c 
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
c 
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
      r=(c(1,1,2,n)*2.+c(1,1,3,n)*z1*6.+c(1,1,4,n)*z2*12.+c(1,1,5,n)*
     & z3*20.+c(1,1,6,n)*z4*30.+c(1,2,2,n)*y1*4.+c(1,2,3,n)*y1z1*12.+
     & c(1,2,4,n)*y1z2*24.+c(1,2,5,n)*y1z3*40.+c(1,2,6,n)*y1z4*60.+c(
     & 1,3,2,n)*y2*6.+c(1,3,3,n)*y2z1*18.+c(1,3,4,n)*y2z2*36.+c(1,3,5,
     & n)*y2z3*60.+c(1,3,6,n)*y2z4*90.+c(1,4,2,n)*y3*8.+c(1,4,3,n)*
     & y3z1*24.+c(1,4,4,n)*y3z2*48.+c(1,4,5,n)*y3z3*80.+c(1,4,6,n)*
     & y3z4*120.+c(1,5,2,n)*y4*10.+c(1,5,3,n)*y4z1*30.+c(1,5,4,n)*
     & y4z2*60.+c(1,5,5,n)*y4z3*100.+c(1,5,6,n)*y4z4*150.+c(1,6,2,n)*
     & y5*12.+c(1,6,3,n)*y5z1*36.+c(1,6,4,n)*y5z2*72.+c(1,6,5,n)*y5z3*
     & 120.+c(1,6,6,n)*y5z4*180.+c(2,1,2,n)*x1*4.+c(2,1,3,n)*x1z1*12.+
     & c(2,1,4,n)*x1z2*24.+c(2,1,5,n)*x1z3*40.+c(2,1,6,n)*x1z4*60.+c(
     & 2,2,2,n)*x1y1*8.+c(2,2,3,n)*x1y1z1*24.+c(2,2,4,n)*x1y1z2*48.+c(
     & 2,2,5,n)*x1y1z3*80.+c(2,2,6,n)*x1y1z4*120.+c(2,3,2,n)*x1y2*12.+
     & c(2,3,3,n)*x1y2z1*36.+c(2,3,4,n)*x1y2z2*72.+c(2,3,5,n)*x1y2z3*
     & 120.+c(2,3,6,n)*x1y2z4*180.+c(2,4,2,n)*x1y3*16.+c(2,4,3,n)*
     & x1y3z1*48.+c(2,4,4,n)*x1y3z2*96.+c(2,4,5,n)*x1y3z3*160.+c(2,4,
     & 6,n)*x1y3z4*240.+c(2,5,2,n)*x1y4*20.+c(2,5,3,n)*x1y4z1*60.+c(2,
     & 5,4,n)*x1y4z2*120.+c(2,5,5,n)*x1y4z3*200.+c(2,5,6,n)*x1y4z4*
     & 300.+c(2,6,2,n)*x1y5*24.+c(2,6,3,n)*x1y5z1*72.+c(2,6,4,n)*
     & x1y5z2*144.+c(2,6,5,n)*x1y5z3*240.+c(2,6,6,n)*x1y5z4*360.+c(3,
     & 1,2,n)*x2*6.+c(3,1,3,n)*x2z1*18.+c(3,1,4,n)*x2z2*36.+c(3,1,5,n)
     & *x2z3*60.+c(3,1,6,n)*x2z4*90.+c(3,2,2,n)*x2y1*12.+c(3,2,3,n)*
     & x2y1z1*36.+c(3,2,4,n)*x2y1z2*72.+c(3,2,5,n)*x2y1z3*120.+c(3,2,
     & 6,n)*x2y1z4*180.+c(3,3,2,n)*x2y2*18.+c(3,3,3,n)*x2y2z1*54.+c(3,
     & 3,4,n)*x2y2z2*108.+c(3,3,5,n)*x2y2z3*180.+c(3,3,6,n)*x2y2z4*
     & 270.+c(3,4,2,n)*x2y3*24.+c(3,4,3,n)*x2y3z1*72.+c(3,4,4,n)*
     & x2y3z2*144.+c(3,4,5,n)*x2y3z3*240.+c(3,4,6,n)*x2y3z4*360.+c(3,
     & 5,2,n)*x2y4*30.+c(3,5,3,n)*x2y4z1*90.+c(3,5,4,n)*x2y4z2*180.+c(
     & 3,5,5,n)*x2y4z3*300.+c(3,5,6,n)*x2y4z4*450.+c(3,6,2,n)*x2y5*
     & 36.+c(3,6,3,n)*x2y5z1*108.+c(3,6,4,n)*x2y5z2*216.+c(3,6,5,n)*
     & x2y5z3*360.+c(3,6,6,n)*x2y5z4*540.+c(4,1,2,n)*x3*8.+c(4,1,3,n)*
     & x3z1*24.+c(4,1,4,n)*x3z2*48.+c(4,1,5,n)*x3z3*80.+c(4,1,6,n)*
     & x3z4*120.+c(4,2,2,n)*x3y1*16.+c(4,2,3,n)*x3y1z1*48.+c(4,2,4,n)*
     & x3y1z2*96.+c(4,2,5,n)*x3y1z3*160.+c(4,2,6,n)*x3y1z4*240.+c(4,3,
     & 2,n)*x3y2*24.+c(4,3,3,n)*x3y2z1*72.+c(4,3,4,n)*x3y2z2*144.+c(4,
     & 3,5,n)*x3y2z3*240.+c(4,3,6,n)*x3y2z4*360.+c(4,4,2,n)*x3y3*32.+
     & c(4,4,3,n)*x3y3z1*96.+c(4,4,4,n)*x3y3z2*192.+c(4,4,5,n)*x3y3z3*
     & 320.+c(4,4,6,n)*x3y3z4*480.+c(4,5,2,n)*x3y4*40.+c(4,5,3,n)*
     & x3y4z1*120.+c(4,5,4,n)*x3y4z2*240.+c(4,5,5,n)*x3y4z3*400.+c(4,
     & 5,6,n)*x3y4z4*600.+c(4,6,2,n)*x3y5*48.+c(4,6,3,n)*x3y5z1*144.+
     & c(4,6,4,n)*x3y5z2*288.+c(4,6,5,n)*x3y5z3*480.+c(4,6,6,n)*
     & x3y5z4*720.+c(5,1,2,n)*x4*10.+c(5,1,3,n)*x4z1*30.+c(5,1,4,n)*
     & x4z2*60.+c(5,1,5,n)*x4z3*100.+c(5,1,6,n)*x4z4*150.+c(5,2,2,n)*
     & x4y1*20.+c(5,2,3,n)*x4y1z1*60.+c(5,2,4,n)*x4y1z2*120.+c(5,2,5,
     & n)*x4y1z3*200.+c(5,2,6,n)*x4y1z4*300.+c(5,3,2,n)*x4y2*30.+c(5,
     & 3,3,n)*x4y2z1*90.+c(5,3,4,n)*x4y2z2*180.+c(5,3,5,n)*x4y2z3*
     & 300.+c(5,3,6,n)*x4y2z4*450.+c(5,4,2,n)*x4y3*40.+c(5,4,3,n)*
     & x4y3z1*120.+c(5,4,4,n)*x4y3z2*240.+c(5,4,5,n)*x4y3z3*400.+c(5,
     & 4,6,n)*x4y3z4*600.+c(5,5,2,n)*x4y4*50.+c(5,5,3,n)*x4y4z1*150.+
     & c(5,5,4,n)*x4y4z2*300.+c(5,5,5,n)*x4y4z3*500.+c(5,5,6,n)*
     & x4y4z4*750.+c(5,6,2,n)*x4y5*60.+c(5,6,3,n)*x4y5z1*180.+c(5,6,4,
     & n)*x4y5z2*360.+c(5,6,5,n)*x4y5z3*600.+c(5,6,6,n)*x4y5z4*900.+c(
     & 6,1,2,n)*x5*12.+c(6,1,3,n)*x5z1*36.+c(6,1,4,n)*x5z2*72.+c(6,1,
     & 5,n)*x5z3*120.+c(6,1,6,n)*x5z4*180.+c(6,2,2,n)*x5y1*24.+c(6,2,
     & 3,n)*x5y1z1*72.+c(6,2,4,n)*x5y1z2*144.+c(6,2,5,n)*x5y1z3*240.+
     & c(6,2,6,n)*x5y1z4*360.+c(6,3,2,n)*x5y2*36.+c(6,3,3,n)*x5y2z1*
     & 108.+c(6,3,4,n)*x5y2z2*216.+c(6,3,5,n)*x5y2z3*360.+c(6,3,6,n)*
     & x5y2z4*540.+c(6,4,2,n)*x5y3*48.+c(6,4,3,n)*x5y3z1*144.+c(6,4,4,
     & n)*x5y3z2*288.+c(6,4,5,n)*x5y3z3*480.+c(6,4,6,n)*x5y3z4*720.+c(
     & 6,5,2,n)*x5y4*60.+c(6,5,3,n)*x5y4z1*180.+c(6,5,4,n)*x5y4z2*
     & 360.+c(6,5,5,n)*x5y4z3*600.+c(6,5,6,n)*x5y4z4*900.+c(6,6,2,n)*
     & x5y5*72.+c(6,6,3,n)*x5y5z1*216.+c(6,6,4,n)*x5y5z2*432.+c(6,6,5,
     & n)*x5y5z3*720.+c(6,6,6,n)*x5y5z4*1080.)*time
! endLoops()
      else if( dx.eq.1.and.dy.eq.2.and.dz.eq.0.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
      z6=z5*z1
c 
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y1z5=y1z4*z1
      y1z6=y1z5*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
      y2z5=y1z5*y1
      y2z6=y1z6*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      y3z4=y2z4*y1
      y3z5=y2z5*y1
      y3z6=y2z6*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y4z2=y3z2*y1
      y4z3=y3z3*y1
      y4z4=y3z4*y1
      y4z5=y3z5*y1
      y4z6=y3z6*y1
c 
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1z5=x1z4*z1
      x1z6=x1z5*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y1z5=x1y1z4*z1
      x1y1z6=x1y1z5*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y2z4=x1y1z4*y1
      x1y2z5=x1y1z5*y1
      x1y2z6=x1y1z6*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x1y3z4=x1y2z4*y1
      x1y3z5=x1y2z5*y1
      x1y3z6=x1y2z6*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y4z2=x1y3z2*y1
      x1y4z3=x1y3z3*y1
      x1y4z4=x1y3z4*y1
      x1y4z5=x1y3z5*y1
      x1y4z6=x1y3z6*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x2z5=x1z5*x1
      x2z6=x1z6*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y1z4=x1y1z4*x1
      x2y1z5=x1y1z5*x1
      x2y1z6=x1y1z6*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y2z4=x1y2z4*x1
      x2y2z5=x1y2z5*x1
      x2y2z6=x1y2z6*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y3z3=x1y3z3*x1
      x2y3z4=x1y3z4*x1
      x2y3z5=x1y3z5*x1
      x2y3z6=x1y3z6*x1
      x2y4=x1y4*x1
      x2y4z1=x1y4z1*x1
      x2y4z2=x1y4z2*x1
      x2y4z3=x1y4z3*x1
      x2y4z4=x1y4z4*x1
      x2y4z5=x1y4z5*x1
      x2y4z6=x1y4z6*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3z4=x2z4*x1
      x3z5=x2z5*x1
      x3z6=x2z6*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x3y1z4=x2y1z4*x1
      x3y1z5=x2y1z5*x1
      x3y1z6=x2y1z6*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y2z3=x2y2z3*x1
      x3y2z4=x2y2z4*x1
      x3y2z5=x2y2z5*x1
      x3y2z6=x2y2z6*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x3y3z2=x2y3z2*x1
      x3y3z3=x2y3z3*x1
      x3y3z4=x2y3z4*x1
      x3y3z5=x2y3z5*x1
      x3y3z6=x2y3z6*x1
      x3y4=x2y4*x1
      x3y4z1=x2y4z1*x1
      x3y4z2=x2y4z2*x1
      x3y4z3=x2y4z3*x1
      x3y4z4=x2y4z4*x1
      x3y4z5=x2y4z5*x1
      x3y4z6=x2y4z6*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4z2=x3z2*x1
      x4z3=x3z3*x1
      x4z4=x3z4*x1
      x4z5=x3z5*x1
      x4z6=x3z6*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y1z2=x3y1z2*x1
      x4y1z3=x3y1z3*x1
      x4y1z4=x3y1z4*x1
      x4y1z5=x3y1z5*x1
      x4y1z6=x3y1z6*x1
      x4y2=x3y2*x1
      x4y2z1=x3y2z1*x1
      x4y2z2=x3y2z2*x1
      x4y2z3=x3y2z3*x1
      x4y2z4=x3y2z4*x1
      x4y2z5=x3y2z5*x1
      x4y2z6=x3y2z6*x1
      x4y3=x3y3*x1
      x4y3z1=x3y3z1*x1
      x4y3z2=x3y3z2*x1
      x4y3z3=x3y3z3*x1
      x4y3z4=x3y3z4*x1
      x4y3z5=x3y3z5*x1
      x4y3z6=x3y3z6*x1
      x4y4=x3y4*x1
      x4y4z1=x3y4z1*x1
      x4y4z2=x3y4z2*x1
      x4y4z3=x3y4z3*x1
      x4y4z4=x3y4z4*x1
      x4y4z5=x3y4z5*x1
      x4y4z6=x3y4z6*x1
      x5=x4*x1
      x5z1=x4z1*x1
      x5z2=x4z2*x1
      x5z3=x4z3*x1
      x5z4=x4z4*x1
      x5z5=x4z5*x1
      x5z6=x4z6*x1
      x5y1=x4y1*x1
      x5y1z1=x4y1z1*x1
      x5y1z2=x4y1z2*x1
      x5y1z3=x4y1z3*x1
      x5y1z4=x4y1z4*x1
      x5y1z5=x4y1z5*x1
      x5y1z6=x4y1z6*x1
      x5y2=x4y2*x1
      x5y2z1=x4y2z1*x1
      x5y2z2=x4y2z2*x1
      x5y2z3=x4y2z3*x1
      x5y2z4=x4y2z4*x1
      x5y2z5=x4y2z5*x1
      x5y2z6=x4y2z6*x1
      x5y3=x4y3*x1
      x5y3z1=x4y3z1*x1
      x5y3z2=x4y3z2*x1
      x5y3z3=x4y3z3*x1
      x5y3z4=x4y3z4*x1
      x5y3z5=x4y3z5*x1
      x5y3z6=x4y3z6*x1
      x5y4=x4y4*x1
      x5y4z1=x4y4z1*x1
      x5y4z2=x4y4z2*x1
      x5y4z3=x4y4z3*x1
      x5y4z4=x4y4z4*x1
      x5y4z5=x4y4z5*x1
      x5y4z6=x4y4z6*x1
      r=(c(1,2,0,n)*2.+c(1,2,1,n)*z1*2.+c(1,2,2,n)*z2*2.+c(1,2,3,n)*z3*
     & 2.+c(1,2,4,n)*z4*2.+c(1,2,5,n)*z5*2.+c(1,2,6,n)*z6*2.+c(1,3,0,
     & n)*y1*6.+c(1,3,1,n)*y1z1*6.+c(1,3,2,n)*y1z2*6.+c(1,3,3,n)*y1z3*
     & 6.+c(1,3,4,n)*y1z4*6.+c(1,3,5,n)*y1z5*6.+c(1,3,6,n)*y1z6*6.+c(
     & 1,4,0,n)*y2*12.+c(1,4,1,n)*y2z1*12.+c(1,4,2,n)*y2z2*12.+c(1,4,
     & 3,n)*y2z3*12.+c(1,4,4,n)*y2z4*12.+c(1,4,5,n)*y2z5*12.+c(1,4,6,
     & n)*y2z6*12.+c(1,5,0,n)*y3*20.+c(1,5,1,n)*y3z1*20.+c(1,5,2,n)*
     & y3z2*20.+c(1,5,3,n)*y3z3*20.+c(1,5,4,n)*y3z4*20.+c(1,5,5,n)*
     & y3z5*20.+c(1,5,6,n)*y3z6*20.+c(1,6,0,n)*y4*30.+c(1,6,1,n)*y4z1*
     & 30.+c(1,6,2,n)*y4z2*30.+c(1,6,3,n)*y4z3*30.+c(1,6,4,n)*y4z4*
     & 30.+c(1,6,5,n)*y4z5*30.+c(1,6,6,n)*y4z6*30.+c(2,2,0,n)*x1*4.+c(
     & 2,2,1,n)*x1z1*4.+c(2,2,2,n)*x1z2*4.+c(2,2,3,n)*x1z3*4.+c(2,2,4,
     & n)*x1z4*4.+c(2,2,5,n)*x1z5*4.+c(2,2,6,n)*x1z6*4.+c(2,3,0,n)*
     & x1y1*12.+c(2,3,1,n)*x1y1z1*12.+c(2,3,2,n)*x1y1z2*12.+c(2,3,3,n)
     & *x1y1z3*12.+c(2,3,4,n)*x1y1z4*12.+c(2,3,5,n)*x1y1z5*12.+c(2,3,
     & 6,n)*x1y1z6*12.+c(2,4,0,n)*x1y2*24.+c(2,4,1,n)*x1y2z1*24.+c(2,
     & 4,2,n)*x1y2z2*24.+c(2,4,3,n)*x1y2z3*24.+c(2,4,4,n)*x1y2z4*24.+
     & c(2,4,5,n)*x1y2z5*24.+c(2,4,6,n)*x1y2z6*24.+c(2,5,0,n)*x1y3*
     & 40.+c(2,5,1,n)*x1y3z1*40.+c(2,5,2,n)*x1y3z2*40.+c(2,5,3,n)*
     & x1y3z3*40.+c(2,5,4,n)*x1y3z4*40.+c(2,5,5,n)*x1y3z5*40.+c(2,5,6,
     & n)*x1y3z6*40.+c(2,6,0,n)*x1y4*60.+c(2,6,1,n)*x1y4z1*60.+c(2,6,
     & 2,n)*x1y4z2*60.+c(2,6,3,n)*x1y4z3*60.+c(2,6,4,n)*x1y4z4*60.+c(
     & 2,6,5,n)*x1y4z5*60.+c(2,6,6,n)*x1y4z6*60.+c(3,2,0,n)*x2*6.+c(3,
     & 2,1,n)*x2z1*6.+c(3,2,2,n)*x2z2*6.+c(3,2,3,n)*x2z3*6.+c(3,2,4,n)
     & *x2z4*6.+c(3,2,5,n)*x2z5*6.+c(3,2,6,n)*x2z6*6.+c(3,3,0,n)*x2y1*
     & 18.+c(3,3,1,n)*x2y1z1*18.+c(3,3,2,n)*x2y1z2*18.+c(3,3,3,n)*
     & x2y1z3*18.+c(3,3,4,n)*x2y1z4*18.+c(3,3,5,n)*x2y1z5*18.+c(3,3,6,
     & n)*x2y1z6*18.+c(3,4,0,n)*x2y2*36.+c(3,4,1,n)*x2y2z1*36.+c(3,4,
     & 2,n)*x2y2z2*36.+c(3,4,3,n)*x2y2z3*36.+c(3,4,4,n)*x2y2z4*36.+c(
     & 3,4,5,n)*x2y2z5*36.+c(3,4,6,n)*x2y2z6*36.+c(3,5,0,n)*x2y3*60.+
     & c(3,5,1,n)*x2y3z1*60.+c(3,5,2,n)*x2y3z2*60.+c(3,5,3,n)*x2y3z3*
     & 60.+c(3,5,4,n)*x2y3z4*60.+c(3,5,5,n)*x2y3z5*60.+c(3,5,6,n)*
     & x2y3z6*60.+c(3,6,0,n)*x2y4*90.+c(3,6,1,n)*x2y4z1*90.+c(3,6,2,n)
     & *x2y4z2*90.+c(3,6,3,n)*x2y4z3*90.+c(3,6,4,n)*x2y4z4*90.+c(3,6,
     & 5,n)*x2y4z5*90.+c(3,6,6,n)*x2y4z6*90.+c(4,2,0,n)*x3*8.+c(4,2,1,
     & n)*x3z1*8.+c(4,2,2,n)*x3z2*8.+c(4,2,3,n)*x3z3*8.+c(4,2,4,n)*
     & x3z4*8.+c(4,2,5,n)*x3z5*8.+c(4,2,6,n)*x3z6*8.+c(4,3,0,n)*x3y1*
     & 24.+c(4,3,1,n)*x3y1z1*24.+c(4,3,2,n)*x3y1z2*24.+c(4,3,3,n)*
     & x3y1z3*24.+c(4,3,4,n)*x3y1z4*24.+c(4,3,5,n)*x3y1z5*24.+c(4,3,6,
     & n)*x3y1z6*24.+c(4,4,0,n)*x3y2*48.+c(4,4,1,n)*x3y2z1*48.+c(4,4,
     & 2,n)*x3y2z2*48.+c(4,4,3,n)*x3y2z3*48.+c(4,4,4,n)*x3y2z4*48.+c(
     & 4,4,5,n)*x3y2z5*48.+c(4,4,6,n)*x3y2z6*48.+c(4,5,0,n)*x3y3*80.+
     & c(4,5,1,n)*x3y3z1*80.+c(4,5,2,n)*x3y3z2*80.+c(4,5,3,n)*x3y3z3*
     & 80.+c(4,5,4,n)*x3y3z4*80.+c(4,5,5,n)*x3y3z5*80.+c(4,5,6,n)*
     & x3y3z6*80.+c(4,6,0,n)*x3y4*120.+c(4,6,1,n)*x3y4z1*120.+c(4,6,2,
     & n)*x3y4z2*120.+c(4,6,3,n)*x3y4z3*120.+c(4,6,4,n)*x3y4z4*120.+c(
     & 4,6,5,n)*x3y4z5*120.+c(4,6,6,n)*x3y4z6*120.+c(5,2,0,n)*x4*10.+
     & c(5,2,1,n)*x4z1*10.+c(5,2,2,n)*x4z2*10.+c(5,2,3,n)*x4z3*10.+c(
     & 5,2,4,n)*x4z4*10.+c(5,2,5,n)*x4z5*10.+c(5,2,6,n)*x4z6*10.+c(5,
     & 3,0,n)*x4y1*30.+c(5,3,1,n)*x4y1z1*30.+c(5,3,2,n)*x4y1z2*30.+c(
     & 5,3,3,n)*x4y1z3*30.+c(5,3,4,n)*x4y1z4*30.+c(5,3,5,n)*x4y1z5*
     & 30.+c(5,3,6,n)*x4y1z6*30.+c(5,4,0,n)*x4y2*60.+c(5,4,1,n)*
     & x4y2z1*60.+c(5,4,2,n)*x4y2z2*60.+c(5,4,3,n)*x4y2z3*60.+c(5,4,4,
     & n)*x4y2z4*60.+c(5,4,5,n)*x4y2z5*60.+c(5,4,6,n)*x4y2z6*60.+c(5,
     & 5,0,n)*x4y3*100.+c(5,5,1,n)*x4y3z1*100.+c(5,5,2,n)*x4y3z2*100.+
     & c(5,5,3,n)*x4y3z3*100.+c(5,5,4,n)*x4y3z4*100.+c(5,5,5,n)*
     & x4y3z5*100.+c(5,5,6,n)*x4y3z6*100.+c(5,6,0,n)*x4y4*150.+c(5,6,
     & 1,n)*x4y4z1*150.+c(5,6,2,n)*x4y4z2*150.+c(5,6,3,n)*x4y4z3*150.+
     & c(5,6,4,n)*x4y4z4*150.+c(5,6,5,n)*x4y4z5*150.+c(5,6,6,n)*
     & x4y4z6*150.+c(6,2,0,n)*x5*12.+c(6,2,1,n)*x5z1*12.+c(6,2,2,n)*
     & x5z2*12.+c(6,2,3,n)*x5z3*12.+c(6,2,4,n)*x5z4*12.+c(6,2,5,n)*
     & x5z5*12.+c(6,2,6,n)*x5z6*12.+c(6,3,0,n)*x5y1*36.+c(6,3,1,n)*
     & x5y1z1*36.+c(6,3,2,n)*x5y1z2*36.+c(6,3,3,n)*x5y1z3*36.+c(6,3,4,
     & n)*x5y1z4*36.+c(6,3,5,n)*x5y1z5*36.+c(6,3,6,n)*x5y1z6*36.+c(6,
     & 4,0,n)*x5y2*72.+c(6,4,1,n)*x5y2z1*72.+c(6,4,2,n)*x5y2z2*72.+c(
     & 6,4,3,n)*x5y2z3*72.+c(6,4,4,n)*x5y2z4*72.+c(6,4,5,n)*x5y2z5*
     & 72.+c(6,4,6,n)*x5y2z6*72.+c(6,5,0,n)*x5y3*120.+c(6,5,1,n)*
     & x5y3z1*120.+c(6,5,2,n)*x5y3z2*120.+c(6,5,3,n)*x5y3z3*120.+c(6,
     & 5,4,n)*x5y3z4*120.+c(6,5,5,n)*x5y3z5*120.+c(6,5,6,n)*x5y3z6*
     & 120.+c(6,6,0,n)*x5y4*180.+c(6,6,1,n)*x5y4z1*180.+c(6,6,2,n)*
     & x5y4z2*180.+c(6,6,3,n)*x5y4z3*180.+c(6,6,4,n)*x5y4z4*180.+c(6,
     & 6,5,n)*x5y4z5*180.+c(6,6,6,n)*x5y4z6*180.)*time
! endLoops()
      else if( dx.eq.1.and.dy.eq.2.and.dz.eq.1.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
c 
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
c 
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
      r=(c(1,2,1,n)*2.+c(1,2,2,n)*z1*4.+c(1,2,3,n)*z2*6.+c(1,2,4,n)*z3*
     & 8.+c(1,2,5,n)*z4*10.+c(1,2,6,n)*z5*12.+c(1,3,1,n)*y1*6.+c(1,3,
     & 2,n)*y1z1*12.+c(1,3,3,n)*y1z2*18.+c(1,3,4,n)*y1z3*24.+c(1,3,5,
     & n)*y1z4*30.+c(1,3,6,n)*y1z5*36.+c(1,4,1,n)*y2*12.+c(1,4,2,n)*
     & y2z1*24.+c(1,4,3,n)*y2z2*36.+c(1,4,4,n)*y2z3*48.+c(1,4,5,n)*
     & y2z4*60.+c(1,4,6,n)*y2z5*72.+c(1,5,1,n)*y3*20.+c(1,5,2,n)*y3z1*
     & 40.+c(1,5,3,n)*y3z2*60.+c(1,5,4,n)*y3z3*80.+c(1,5,5,n)*y3z4*
     & 100.+c(1,5,6,n)*y3z5*120.+c(1,6,1,n)*y4*30.+c(1,6,2,n)*y4z1*
     & 60.+c(1,6,3,n)*y4z2*90.+c(1,6,4,n)*y4z3*120.+c(1,6,5,n)*y4z4*
     & 150.+c(1,6,6,n)*y4z5*180.+c(2,2,1,n)*x1*4.+c(2,2,2,n)*x1z1*8.+
     & c(2,2,3,n)*x1z2*12.+c(2,2,4,n)*x1z3*16.+c(2,2,5,n)*x1z4*20.+c(
     & 2,2,6,n)*x1z5*24.+c(2,3,1,n)*x1y1*12.+c(2,3,2,n)*x1y1z1*24.+c(
     & 2,3,3,n)*x1y1z2*36.+c(2,3,4,n)*x1y1z3*48.+c(2,3,5,n)*x1y1z4*
     & 60.+c(2,3,6,n)*x1y1z5*72.+c(2,4,1,n)*x1y2*24.+c(2,4,2,n)*
     & x1y2z1*48.+c(2,4,3,n)*x1y2z2*72.+c(2,4,4,n)*x1y2z3*96.+c(2,4,5,
     & n)*x1y2z4*120.+c(2,4,6,n)*x1y2z5*144.+c(2,5,1,n)*x1y3*40.+c(2,
     & 5,2,n)*x1y3z1*80.+c(2,5,3,n)*x1y3z2*120.+c(2,5,4,n)*x1y3z3*
     & 160.+c(2,5,5,n)*x1y3z4*200.+c(2,5,6,n)*x1y3z5*240.+c(2,6,1,n)*
     & x1y4*60.+c(2,6,2,n)*x1y4z1*120.+c(2,6,3,n)*x1y4z2*180.+c(2,6,4,
     & n)*x1y4z3*240.+c(2,6,5,n)*x1y4z4*300.+c(2,6,6,n)*x1y4z5*360.+c(
     & 3,2,1,n)*x2*6.+c(3,2,2,n)*x2z1*12.+c(3,2,3,n)*x2z2*18.+c(3,2,4,
     & n)*x2z3*24.+c(3,2,5,n)*x2z4*30.+c(3,2,6,n)*x2z5*36.+c(3,3,1,n)*
     & x2y1*18.+c(3,3,2,n)*x2y1z1*36.+c(3,3,3,n)*x2y1z2*54.+c(3,3,4,n)
     & *x2y1z3*72.+c(3,3,5,n)*x2y1z4*90.+c(3,3,6,n)*x2y1z5*108.+c(3,4,
     & 1,n)*x2y2*36.+c(3,4,2,n)*x2y2z1*72.+c(3,4,3,n)*x2y2z2*108.+c(3,
     & 4,4,n)*x2y2z3*144.+c(3,4,5,n)*x2y2z4*180.+c(3,4,6,n)*x2y2z5*
     & 216.+c(3,5,1,n)*x2y3*60.+c(3,5,2,n)*x2y3z1*120.+c(3,5,3,n)*
     & x2y3z2*180.+c(3,5,4,n)*x2y3z3*240.+c(3,5,5,n)*x2y3z4*300.+c(3,
     & 5,6,n)*x2y3z5*360.+c(3,6,1,n)*x2y4*90.+c(3,6,2,n)*x2y4z1*180.+
     & c(3,6,3,n)*x2y4z2*270.+c(3,6,4,n)*x2y4z3*360.+c(3,6,5,n)*
     & x2y4z4*450.+c(3,6,6,n)*x2y4z5*540.+c(4,2,1,n)*x3*8.+c(4,2,2,n)*
     & x3z1*16.+c(4,2,3,n)*x3z2*24.+c(4,2,4,n)*x3z3*32.+c(4,2,5,n)*
     & x3z4*40.+c(4,2,6,n)*x3z5*48.+c(4,3,1,n)*x3y1*24.+c(4,3,2,n)*
     & x3y1z1*48.+c(4,3,3,n)*x3y1z2*72.+c(4,3,4,n)*x3y1z3*96.+c(4,3,5,
     & n)*x3y1z4*120.+c(4,3,6,n)*x3y1z5*144.+c(4,4,1,n)*x3y2*48.+c(4,
     & 4,2,n)*x3y2z1*96.+c(4,4,3,n)*x3y2z2*144.+c(4,4,4,n)*x3y2z3*
     & 192.+c(4,4,5,n)*x3y2z4*240.+c(4,4,6,n)*x3y2z5*288.+c(4,5,1,n)*
     & x3y3*80.+c(4,5,2,n)*x3y3z1*160.+c(4,5,3,n)*x3y3z2*240.+c(4,5,4,
     & n)*x3y3z3*320.+c(4,5,5,n)*x3y3z4*400.+c(4,5,6,n)*x3y3z5*480.+c(
     & 4,6,1,n)*x3y4*120.+c(4,6,2,n)*x3y4z1*240.+c(4,6,3,n)*x3y4z2*
     & 360.+c(4,6,4,n)*x3y4z3*480.+c(4,6,5,n)*x3y4z4*600.+c(4,6,6,n)*
     & x3y4z5*720.+c(5,2,1,n)*x4*10.+c(5,2,2,n)*x4z1*20.+c(5,2,3,n)*
     & x4z2*30.+c(5,2,4,n)*x4z3*40.+c(5,2,5,n)*x4z4*50.+c(5,2,6,n)*
     & x4z5*60.+c(5,3,1,n)*x4y1*30.+c(5,3,2,n)*x4y1z1*60.+c(5,3,3,n)*
     & x4y1z2*90.+c(5,3,4,n)*x4y1z3*120.+c(5,3,5,n)*x4y1z4*150.+c(5,3,
     & 6,n)*x4y1z5*180.+c(5,4,1,n)*x4y2*60.+c(5,4,2,n)*x4y2z1*120.+c(
     & 5,4,3,n)*x4y2z2*180.+c(5,4,4,n)*x4y2z3*240.+c(5,4,5,n)*x4y2z4*
     & 300.+c(5,4,6,n)*x4y2z5*360.+c(5,5,1,n)*x4y3*100.+c(5,5,2,n)*
     & x4y3z1*200.+c(5,5,3,n)*x4y3z2*300.+c(5,5,4,n)*x4y3z3*400.+c(5,
     & 5,5,n)*x4y3z4*500.+c(5,5,6,n)*x4y3z5*600.+c(5,6,1,n)*x4y4*150.+
     & c(5,6,2,n)*x4y4z1*300.+c(5,6,3,n)*x4y4z2*450.+c(5,6,4,n)*
     & x4y4z3*600.+c(5,6,5,n)*x4y4z4*750.+c(5,6,6,n)*x4y4z5*900.+c(6,
     & 2,1,n)*x5*12.+c(6,2,2,n)*x5z1*24.+c(6,2,3,n)*x5z2*36.+c(6,2,4,
     & n)*x5z3*48.+c(6,2,5,n)*x5z4*60.+c(6,2,6,n)*x5z5*72.+c(6,3,1,n)*
     & x5y1*36.+c(6,3,2,n)*x5y1z1*72.+c(6,3,3,n)*x5y1z2*108.+c(6,3,4,
     & n)*x5y1z3*144.+c(6,3,5,n)*x5y1z4*180.+c(6,3,6,n)*x5y1z5*216.+c(
     & 6,4,1,n)*x5y2*72.+c(6,4,2,n)*x5y2z1*144.+c(6,4,3,n)*x5y2z2*
     & 216.+c(6,4,4,n)*x5y2z3*288.+c(6,4,5,n)*x5y2z4*360.+c(6,4,6,n)*
     & x5y2z5*432.+c(6,5,1,n)*x5y3*120.+c(6,5,2,n)*x5y3z1*240.+c(6,5,
     & 3,n)*x5y3z2*360.+c(6,5,4,n)*x5y3z3*480.+c(6,5,5,n)*x5y3z4*600.+
     & c(6,5,6,n)*x5y3z5*720.+c(6,6,1,n)*x5y4*180.+c(6,6,2,n)*x5y4z1*
     & 360.+c(6,6,3,n)*x5y4z2*540.+c(6,6,4,n)*x5y4z3*720.+c(6,6,5,n)*
     & x5y4z4*900.+c(6,6,6,n)*x5y4z5*1080.)*time
! endLoops()
      else if( dx.eq.1.and.dy.eq.2.and.dz.eq.2.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
c 
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
c 
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
      r=(c(1,2,2,n)*4.+c(1,2,3,n)*z1*12.+c(1,2,4,n)*z2*24.+c(1,2,5,n)*
     & z3*40.+c(1,2,6,n)*z4*60.+c(1,3,2,n)*y1*12.+c(1,3,3,n)*y1z1*36.+
     & c(1,3,4,n)*y1z2*72.+c(1,3,5,n)*y1z3*120.+c(1,3,6,n)*y1z4*180.+
     & c(1,4,2,n)*y2*24.+c(1,4,3,n)*y2z1*72.+c(1,4,4,n)*y2z2*144.+c(1,
     & 4,5,n)*y2z3*240.+c(1,4,6,n)*y2z4*360.+c(1,5,2,n)*y3*40.+c(1,5,
     & 3,n)*y3z1*120.+c(1,5,4,n)*y3z2*240.+c(1,5,5,n)*y3z3*400.+c(1,5,
     & 6,n)*y3z4*600.+c(1,6,2,n)*y4*60.+c(1,6,3,n)*y4z1*180.+c(1,6,4,
     & n)*y4z2*360.+c(1,6,5,n)*y4z3*600.+c(1,6,6,n)*y4z4*900.+c(2,2,2,
     & n)*x1*8.+c(2,2,3,n)*x1z1*24.+c(2,2,4,n)*x1z2*48.+c(2,2,5,n)*
     & x1z3*80.+c(2,2,6,n)*x1z4*120.+c(2,3,2,n)*x1y1*24.+c(2,3,3,n)*
     & x1y1z1*72.+c(2,3,4,n)*x1y1z2*144.+c(2,3,5,n)*x1y1z3*240.+c(2,3,
     & 6,n)*x1y1z4*360.+c(2,4,2,n)*x1y2*48.+c(2,4,3,n)*x1y2z1*144.+c(
     & 2,4,4,n)*x1y2z2*288.+c(2,4,5,n)*x1y2z3*480.+c(2,4,6,n)*x1y2z4*
     & 720.+c(2,5,2,n)*x1y3*80.+c(2,5,3,n)*x1y3z1*240.+c(2,5,4,n)*
     & x1y3z2*480.+c(2,5,5,n)*x1y3z3*800.+c(2,5,6,n)*x1y3z4*1200.+c(2,
     & 6,2,n)*x1y4*120.+c(2,6,3,n)*x1y4z1*360.+c(2,6,4,n)*x1y4z2*720.+
     & c(2,6,5,n)*x1y4z3*1200.+c(2,6,6,n)*x1y4z4*1800.+c(3,2,2,n)*x2*
     & 12.+c(3,2,3,n)*x2z1*36.+c(3,2,4,n)*x2z2*72.+c(3,2,5,n)*x2z3*
     & 120.+c(3,2,6,n)*x2z4*180.+c(3,3,2,n)*x2y1*36.+c(3,3,3,n)*
     & x2y1z1*108.+c(3,3,4,n)*x2y1z2*216.+c(3,3,5,n)*x2y1z3*360.+c(3,
     & 3,6,n)*x2y1z4*540.+c(3,4,2,n)*x2y2*72.+c(3,4,3,n)*x2y2z1*216.+
     & c(3,4,4,n)*x2y2z2*432.+c(3,4,5,n)*x2y2z3*720.+c(3,4,6,n)*
     & x2y2z4*1080.+c(3,5,2,n)*x2y3*120.+c(3,5,3,n)*x2y3z1*360.+c(3,5,
     & 4,n)*x2y3z2*720.+c(3,5,5,n)*x2y3z3*1200.+c(3,5,6,n)*x2y3z4*
     & 1800.+c(3,6,2,n)*x2y4*180.+c(3,6,3,n)*x2y4z1*540.+c(3,6,4,n)*
     & x2y4z2*1080.+c(3,6,5,n)*x2y4z3*1800.+c(3,6,6,n)*x2y4z4*2700.+c(
     & 4,2,2,n)*x3*16.+c(4,2,3,n)*x3z1*48.+c(4,2,4,n)*x3z2*96.+c(4,2,
     & 5,n)*x3z3*160.+c(4,2,6,n)*x3z4*240.+c(4,3,2,n)*x3y1*48.+c(4,3,
     & 3,n)*x3y1z1*144.+c(4,3,4,n)*x3y1z2*288.+c(4,3,5,n)*x3y1z3*480.+
     & c(4,3,6,n)*x3y1z4*720.+c(4,4,2,n)*x3y2*96.+c(4,4,3,n)*x3y2z1*
     & 288.+c(4,4,4,n)*x3y2z2*576.+c(4,4,5,n)*x3y2z3*960.+c(4,4,6,n)*
     & x3y2z4*1440.+c(4,5,2,n)*x3y3*160.+c(4,5,3,n)*x3y3z1*480.+c(4,5,
     & 4,n)*x3y3z2*960.+c(4,5,5,n)*x3y3z3*1600.+c(4,5,6,n)*x3y3z4*
     & 2400.+c(4,6,2,n)*x3y4*240.+c(4,6,3,n)*x3y4z1*720.+c(4,6,4,n)*
     & x3y4z2*1440.+c(4,6,5,n)*x3y4z3*2400.+c(4,6,6,n)*x3y4z4*3600.+c(
     & 5,2,2,n)*x4*20.+c(5,2,3,n)*x4z1*60.+c(5,2,4,n)*x4z2*120.+c(5,2,
     & 5,n)*x4z3*200.+c(5,2,6,n)*x4z4*300.+c(5,3,2,n)*x4y1*60.+c(5,3,
     & 3,n)*x4y1z1*180.+c(5,3,4,n)*x4y1z2*360.+c(5,3,5,n)*x4y1z3*600.+
     & c(5,3,6,n)*x4y1z4*900.+c(5,4,2,n)*x4y2*120.+c(5,4,3,n)*x4y2z1*
     & 360.+c(5,4,4,n)*x4y2z2*720.+c(5,4,5,n)*x4y2z3*1200.+c(5,4,6,n)*
     & x4y2z4*1800.+c(5,5,2,n)*x4y3*200.+c(5,5,3,n)*x4y3z1*600.+c(5,5,
     & 4,n)*x4y3z2*1200.+c(5,5,5,n)*x4y3z3*2000.+c(5,5,6,n)*x4y3z4*
     & 3000.+c(5,6,2,n)*x4y4*300.+c(5,6,3,n)*x4y4z1*900.+c(5,6,4,n)*
     & x4y4z2*1800.+c(5,6,5,n)*x4y4z3*3000.+c(5,6,6,n)*x4y4z4*4500.+c(
     & 6,2,2,n)*x5*24.+c(6,2,3,n)*x5z1*72.+c(6,2,4,n)*x5z2*144.+c(6,2,
     & 5,n)*x5z3*240.+c(6,2,6,n)*x5z4*360.+c(6,3,2,n)*x5y1*72.+c(6,3,
     & 3,n)*x5y1z1*216.+c(6,3,4,n)*x5y1z2*432.+c(6,3,5,n)*x5y1z3*720.+
     & c(6,3,6,n)*x5y1z4*1080.+c(6,4,2,n)*x5y2*144.+c(6,4,3,n)*x5y2z1*
     & 432.+c(6,4,4,n)*x5y2z2*864.+c(6,4,5,n)*x5y2z3*1440.+c(6,4,6,n)*
     & x5y2z4*2160.+c(6,5,2,n)*x5y3*240.+c(6,5,3,n)*x5y3z1*720.+c(6,5,
     & 4,n)*x5y3z2*1440.+c(6,5,5,n)*x5y3z3*2400.+c(6,5,6,n)*x5y3z4*
     & 3600.+c(6,6,2,n)*x5y4*360.+c(6,6,3,n)*x5y4z1*1080.+c(6,6,4,n)*
     & x5y4z2*2160.+c(6,6,5,n)*x5y4z3*3600.+c(6,6,6,n)*x5y4z4*5400.)*
     & time
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
      z6=z5*z1
c 
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y1z5=y1z4*z1
      y1z6=y1z5*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
      y2z5=y1z5*y1
      y2z6=y1z6*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      y3z4=y2z4*y1
      y3z5=y2z5*y1
      y3z6=y2z6*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y4z2=y3z2*y1
      y4z3=y3z3*y1
      y4z4=y3z4*y1
      y4z5=y3z5*y1
      y4z6=y3z6*y1
      y5=y4*y1
      y5z1=y4z1*y1
      y5z2=y4z2*y1
      y5z3=y4z3*y1
      y5z4=y4z4*y1
      y5z5=y4z5*y1
      y5z6=y4z6*y1
      y6=y5*y1
      y6z1=y5z1*y1
      y6z2=y5z2*y1
      y6z3=y5z3*y1
      y6z4=y5z4*y1
      y6z5=y5z5*y1
      y6z6=y5z6*y1
c 
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1z5=x1z4*z1
      x1z6=x1z5*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y1z5=x1y1z4*z1
      x1y1z6=x1y1z5*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y2z4=x1y1z4*y1
      x1y2z5=x1y1z5*y1
      x1y2z6=x1y1z6*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x1y3z4=x1y2z4*y1
      x1y3z5=x1y2z5*y1
      x1y3z6=x1y2z6*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y4z2=x1y3z2*y1
      x1y4z3=x1y3z3*y1
      x1y4z4=x1y3z4*y1
      x1y4z5=x1y3z5*y1
      x1y4z6=x1y3z6*y1
      x1y5=x1y4*y1
      x1y5z1=x1y4z1*y1
      x1y5z2=x1y4z2*y1
      x1y5z3=x1y4z3*y1
      x1y5z4=x1y4z4*y1
      x1y5z5=x1y4z5*y1
      x1y5z6=x1y4z6*y1
      x1y6=x1y5*y1
      x1y6z1=x1y5z1*y1
      x1y6z2=x1y5z2*y1
      x1y6z3=x1y5z3*y1
      x1y6z4=x1y5z4*y1
      x1y6z5=x1y5z5*y1
      x1y6z6=x1y5z6*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x2z5=x1z5*x1
      x2z6=x1z6*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y1z4=x1y1z4*x1
      x2y1z5=x1y1z5*x1
      x2y1z6=x1y1z6*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y2z4=x1y2z4*x1
      x2y2z5=x1y2z5*x1
      x2y2z6=x1y2z6*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y3z3=x1y3z3*x1
      x2y3z4=x1y3z4*x1
      x2y3z5=x1y3z5*x1
      x2y3z6=x1y3z6*x1
      x2y4=x1y4*x1
      x2y4z1=x1y4z1*x1
      x2y4z2=x1y4z2*x1
      x2y4z3=x1y4z3*x1
      x2y4z4=x1y4z4*x1
      x2y4z5=x1y4z5*x1
      x2y4z6=x1y4z6*x1
      x2y5=x1y5*x1
      x2y5z1=x1y5z1*x1
      x2y5z2=x1y5z2*x1
      x2y5z3=x1y5z3*x1
      x2y5z4=x1y5z4*x1
      x2y5z5=x1y5z5*x1
      x2y5z6=x1y5z6*x1
      x2y6=x1y6*x1
      x2y6z1=x1y6z1*x1
      x2y6z2=x1y6z2*x1
      x2y6z3=x1y6z3*x1
      x2y6z4=x1y6z4*x1
      x2y6z5=x1y6z5*x1
      x2y6z6=x1y6z6*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3z4=x2z4*x1
      x3z5=x2z5*x1
      x3z6=x2z6*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x3y1z4=x2y1z4*x1
      x3y1z5=x2y1z5*x1
      x3y1z6=x2y1z6*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y2z3=x2y2z3*x1
      x3y2z4=x2y2z4*x1
      x3y2z5=x2y2z5*x1
      x3y2z6=x2y2z6*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x3y3z2=x2y3z2*x1
      x3y3z3=x2y3z3*x1
      x3y3z4=x2y3z4*x1
      x3y3z5=x2y3z5*x1
      x3y3z6=x2y3z6*x1
      x3y4=x2y4*x1
      x3y4z1=x2y4z1*x1
      x3y4z2=x2y4z2*x1
      x3y4z3=x2y4z3*x1
      x3y4z4=x2y4z4*x1
      x3y4z5=x2y4z5*x1
      x3y4z6=x2y4z6*x1
      x3y5=x2y5*x1
      x3y5z1=x2y5z1*x1
      x3y5z2=x2y5z2*x1
      x3y5z3=x2y5z3*x1
      x3y5z4=x2y5z4*x1
      x3y5z5=x2y5z5*x1
      x3y5z6=x2y5z6*x1
      x3y6=x2y6*x1
      x3y6z1=x2y6z1*x1
      x3y6z2=x2y6z2*x1
      x3y6z3=x2y6z3*x1
      x3y6z4=x2y6z4*x1
      x3y6z5=x2y6z5*x1
      x3y6z6=x2y6z6*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4z2=x3z2*x1
      x4z3=x3z3*x1
      x4z4=x3z4*x1
      x4z5=x3z5*x1
      x4z6=x3z6*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y1z2=x3y1z2*x1
      x4y1z3=x3y1z3*x1
      x4y1z4=x3y1z4*x1
      x4y1z5=x3y1z5*x1
      x4y1z6=x3y1z6*x1
      x4y2=x3y2*x1
      x4y2z1=x3y2z1*x1
      x4y2z2=x3y2z2*x1
      x4y2z3=x3y2z3*x1
      x4y2z4=x3y2z4*x1
      x4y2z5=x3y2z5*x1
      x4y2z6=x3y2z6*x1
      x4y3=x3y3*x1
      x4y3z1=x3y3z1*x1
      x4y3z2=x3y3z2*x1
      x4y3z3=x3y3z3*x1
      x4y3z4=x3y3z4*x1
      x4y3z5=x3y3z5*x1
      x4y3z6=x3y3z6*x1
      x4y4=x3y4*x1
      x4y4z1=x3y4z1*x1
      x4y4z2=x3y4z2*x1
      x4y4z3=x3y4z3*x1
      x4y4z4=x3y4z4*x1
      x4y4z5=x3y4z5*x1
      x4y4z6=x3y4z6*x1
      x4y5=x3y5*x1
      x4y5z1=x3y5z1*x1
      x4y5z2=x3y5z2*x1
      x4y5z3=x3y5z3*x1
      x4y5z4=x3y5z4*x1
      x4y5z5=x3y5z5*x1
      x4y5z6=x3y5z6*x1
      x4y6=x3y6*x1
      x4y6z1=x3y6z1*x1
      x4y6z2=x3y6z2*x1
      x4y6z3=x3y6z3*x1
      x4y6z4=x3y6z4*x1
      x4y6z5=x3y6z5*x1
      x4y6z6=x3y6z6*x1
      r=(c(2,0,0,n)*2.+c(2,0,1,n)*z1*2.+c(2,0,2,n)*z2*2.+c(2,0,3,n)*z3*
     & 2.+c(2,0,4,n)*z4*2.+c(2,0,5,n)*z5*2.+c(2,0,6,n)*z6*2.+c(2,1,0,
     & n)*y1*2.+c(2,1,1,n)*y1z1*2.+c(2,1,2,n)*y1z2*2.+c(2,1,3,n)*y1z3*
     & 2.+c(2,1,4,n)*y1z4*2.+c(2,1,5,n)*y1z5*2.+c(2,1,6,n)*y1z6*2.+c(
     & 2,2,0,n)*y2*2.+c(2,2,1,n)*y2z1*2.+c(2,2,2,n)*y2z2*2.+c(2,2,3,n)
     & *y2z3*2.+c(2,2,4,n)*y2z4*2.+c(2,2,5,n)*y2z5*2.+c(2,2,6,n)*y2z6*
     & 2.+c(2,3,0,n)*y3*2.+c(2,3,1,n)*y3z1*2.+c(2,3,2,n)*y3z2*2.+c(2,
     & 3,3,n)*y3z3*2.+c(2,3,4,n)*y3z4*2.+c(2,3,5,n)*y3z5*2.+c(2,3,6,n)
     & *y3z6*2.+c(2,4,0,n)*y4*2.+c(2,4,1,n)*y4z1*2.+c(2,4,2,n)*y4z2*
     & 2.+c(2,4,3,n)*y4z3*2.+c(2,4,4,n)*y4z4*2.+c(2,4,5,n)*y4z5*2.+c(
     & 2,4,6,n)*y4z6*2.+c(2,5,0,n)*y5*2.+c(2,5,1,n)*y5z1*2.+c(2,5,2,n)
     & *y5z2*2.+c(2,5,3,n)*y5z3*2.+c(2,5,4,n)*y5z4*2.+c(2,5,5,n)*y5z5*
     & 2.+c(2,5,6,n)*y5z6*2.+c(2,6,0,n)*y6*2.+c(2,6,1,n)*y6z1*2.+c(2,
     & 6,2,n)*y6z2*2.+c(2,6,3,n)*y6z3*2.+c(2,6,4,n)*y6z4*2.+c(2,6,5,n)
     & *y6z5*2.+c(2,6,6,n)*y6z6*2.+c(3,0,0,n)*x1*6.+c(3,0,1,n)*x1z1*
     & 6.+c(3,0,2,n)*x1z2*6.+c(3,0,3,n)*x1z3*6.+c(3,0,4,n)*x1z4*6.+c(
     & 3,0,5,n)*x1z5*6.+c(3,0,6,n)*x1z6*6.+c(3,1,0,n)*x1y1*6.+c(3,1,1,
     & n)*x1y1z1*6.+c(3,1,2,n)*x1y1z2*6.+c(3,1,3,n)*x1y1z3*6.+c(3,1,4,
     & n)*x1y1z4*6.+c(3,1,5,n)*x1y1z5*6.+c(3,1,6,n)*x1y1z6*6.+c(3,2,0,
     & n)*x1y2*6.+c(3,2,1,n)*x1y2z1*6.+c(3,2,2,n)*x1y2z2*6.+c(3,2,3,n)
     & *x1y2z3*6.+c(3,2,4,n)*x1y2z4*6.+c(3,2,5,n)*x1y2z5*6.+c(3,2,6,n)
     & *x1y2z6*6.+c(3,3,0,n)*x1y3*6.+c(3,3,1,n)*x1y3z1*6.+c(3,3,2,n)*
     & x1y3z2*6.+c(3,3,3,n)*x1y3z3*6.+c(3,3,4,n)*x1y3z4*6.+c(3,3,5,n)*
     & x1y3z5*6.+c(3,3,6,n)*x1y3z6*6.+c(3,4,0,n)*x1y4*6.+c(3,4,1,n)*
     & x1y4z1*6.+c(3,4,2,n)*x1y4z2*6.+c(3,4,3,n)*x1y4z3*6.+c(3,4,4,n)*
     & x1y4z4*6.+c(3,4,5,n)*x1y4z5*6.+c(3,4,6,n)*x1y4z6*6.+c(3,5,0,n)*
     & x1y5*6.+c(3,5,1,n)*x1y5z1*6.+c(3,5,2,n)*x1y5z2*6.+c(3,5,3,n)*
     & x1y5z3*6.+c(3,5,4,n)*x1y5z4*6.+c(3,5,5,n)*x1y5z5*6.+c(3,5,6,n)*
     & x1y5z6*6.+c(3,6,0,n)*x1y6*6.+c(3,6,1,n)*x1y6z1*6.+c(3,6,2,n)*
     & x1y6z2*6.+c(3,6,3,n)*x1y6z3*6.+c(3,6,4,n)*x1y6z4*6.+c(3,6,5,n)*
     & x1y6z5*6.+c(3,6,6,n)*x1y6z6*6.+c(4,0,0,n)*x2*12.+c(4,0,1,n)*
     & x2z1*12.+c(4,0,2,n)*x2z2*12.+c(4,0,3,n)*x2z3*12.+c(4,0,4,n)*
     & x2z4*12.+c(4,0,5,n)*x2z5*12.+c(4,0,6,n)*x2z6*12.+c(4,1,0,n)*
     & x2y1*12.+c(4,1,1,n)*x2y1z1*12.+c(4,1,2,n)*x2y1z2*12.+c(4,1,3,n)
     & *x2y1z3*12.+c(4,1,4,n)*x2y1z4*12.+c(4,1,5,n)*x2y1z5*12.+c(4,1,
     & 6,n)*x2y1z6*12.+c(4,2,0,n)*x2y2*12.+c(4,2,1,n)*x2y2z1*12.+c(4,
     & 2,2,n)*x2y2z2*12.+c(4,2,3,n)*x2y2z3*12.+c(4,2,4,n)*x2y2z4*12.+
     & c(4,2,5,n)*x2y2z5*12.+c(4,2,6,n)*x2y2z6*12.+c(4,3,0,n)*x2y3*
     & 12.+c(4,3,1,n)*x2y3z1*12.+c(4,3,2,n)*x2y3z2*12.+c(4,3,3,n)*
     & x2y3z3*12.+c(4,3,4,n)*x2y3z4*12.+c(4,3,5,n)*x2y3z5*12.+c(4,3,6,
     & n)*x2y3z6*12.+c(4,4,0,n)*x2y4*12.+c(4,4,1,n)*x2y4z1*12.+c(4,4,
     & 2,n)*x2y4z2*12.+c(4,4,3,n)*x2y4z3*12.+c(4,4,4,n)*x2y4z4*12.+c(
     & 4,4,5,n)*x2y4z5*12.+c(4,4,6,n)*x2y4z6*12.+c(4,5,0,n)*x2y5*12.+
     & c(4,5,1,n)*x2y5z1*12.+c(4,5,2,n)*x2y5z2*12.+c(4,5,3,n)*x2y5z3*
     & 12.+c(4,5,4,n)*x2y5z4*12.+c(4,5,5,n)*x2y5z5*12.+c(4,5,6,n)*
     & x2y5z6*12.+c(4,6,0,n)*x2y6*12.+c(4,6,1,n)*x2y6z1*12.+c(4,6,2,n)
     & *x2y6z2*12.+c(4,6,3,n)*x2y6z3*12.+c(4,6,4,n)*x2y6z4*12.+c(4,6,
     & 5,n)*x2y6z5*12.+c(4,6,6,n)*x2y6z6*12.+c(5,0,0,n)*x3*20.+c(5,0,
     & 1,n)*x3z1*20.+c(5,0,2,n)*x3z2*20.+c(5,0,3,n)*x3z3*20.+c(5,0,4,
     & n)*x3z4*20.+c(5,0,5,n)*x3z5*20.+c(5,0,6,n)*x3z6*20.+c(5,1,0,n)*
     & x3y1*20.+c(5,1,1,n)*x3y1z1*20.+c(5,1,2,n)*x3y1z2*20.+c(5,1,3,n)
     & *x3y1z3*20.+c(5,1,4,n)*x3y1z4*20.+c(5,1,5,n)*x3y1z5*20.+c(5,1,
     & 6,n)*x3y1z6*20.+c(5,2,0,n)*x3y2*20.+c(5,2,1,n)*x3y2z1*20.+c(5,
     & 2,2,n)*x3y2z2*20.+c(5,2,3,n)*x3y2z3*20.+c(5,2,4,n)*x3y2z4*20.+
     & c(5,2,5,n)*x3y2z5*20.+c(5,2,6,n)*x3y2z6*20.+c(5,3,0,n)*x3y3*
     & 20.+c(5,3,1,n)*x3y3z1*20.+c(5,3,2,n)*x3y3z2*20.+c(5,3,3,n)*
     & x3y3z3*20.+c(5,3,4,n)*x3y3z4*20.+c(5,3,5,n)*x3y3z5*20.+c(5,3,6,
     & n)*x3y3z6*20.+c(5,4,0,n)*x3y4*20.+c(5,4,1,n)*x3y4z1*20.+c(5,4,
     & 2,n)*x3y4z2*20.+c(5,4,3,n)*x3y4z3*20.+c(5,4,4,n)*x3y4z4*20.+c(
     & 5,4,5,n)*x3y4z5*20.+c(5,4,6,n)*x3y4z6*20.+c(5,5,0,n)*x3y5*20.+
     & c(5,5,1,n)*x3y5z1*20.+c(5,5,2,n)*x3y5z2*20.+c(5,5,3,n)*x3y5z3*
     & 20.+c(5,5,4,n)*x3y5z4*20.+c(5,5,5,n)*x3y5z5*20.+c(5,5,6,n)*
     & x3y5z6*20.+c(5,6,0,n)*x3y6*20.+c(5,6,1,n)*x3y6z1*20.+c(5,6,2,n)
     & *x3y6z2*20.+c(5,6,3,n)*x3y6z3*20.+c(5,6,4,n)*x3y6z4*20.+c(5,6,
     & 5,n)*x3y6z5*20.+c(5,6,6,n)*x3y6z6*20.+c(6,0,0,n)*x4*30.+c(6,0,
     & 1,n)*x4z1*30.+c(6,0,2,n)*x4z2*30.+c(6,0,3,n)*x4z3*30.+c(6,0,4,
     & n)*x4z4*30.+c(6,0,5,n)*x4z5*30.+c(6,0,6,n)*x4z6*30.+c(6,1,0,n)*
     & x4y1*30.+c(6,1,1,n)*x4y1z1*30.+c(6,1,2,n)*x4y1z2*30.+c(6,1,3,n)
     & *x4y1z3*30.+c(6,1,4,n)*x4y1z4*30.+c(6,1,5,n)*x4y1z5*30.+c(6,1,
     & 6,n)*x4y1z6*30.+c(6,2,0,n)*x4y2*30.+c(6,2,1,n)*x4y2z1*30.+c(6,
     & 2,2,n)*x4y2z2*30.+c(6,2,3,n)*x4y2z3*30.+c(6,2,4,n)*x4y2z4*30.+
     & c(6,2,5,n)*x4y2z5*30.+c(6,2,6,n)*x4y2z6*30.+c(6,3,0,n)*x4y3*
     & 30.+c(6,3,1,n)*x4y3z1*30.+c(6,3,2,n)*x4y3z2*30.+c(6,3,3,n)*
     & x4y3z3*30.+c(6,3,4,n)*x4y3z4*30.+c(6,3,5,n)*x4y3z5*30.+c(6,3,6,
     & n)*x4y3z6*30.+c(6,4,0,n)*x4y4*30.+c(6,4,1,n)*x4y4z1*30.+c(6,4,
     & 2,n)*x4y4z2*30.+c(6,4,3,n)*x4y4z3*30.+c(6,4,4,n)*x4y4z4*30.+c(
     & 6,4,5,n)*x4y4z5*30.+c(6,4,6,n)*x4y4z6*30.+c(6,5,0,n)*x4y5*30.+
     & c(6,5,1,n)*x4y5z1*30.+c(6,5,2,n)*x4y5z2*30.)
       r=(r+c(6,5,3,n)*x4y5z3*30.+c(6,5,4,n)*x4y5z4*30.+c(6,5,5,n)*
     & x4y5z5*30.+c(6,5,6,n)*x4y5z6*30.+c(6,6,0,n)*x4y6*30.+c(6,6,1,n)
     & *x4y6z1*30.+c(6,6,2,n)*x4y6z2*30.+c(6,6,3,n)*x4y6z3*30.+c(6,6,
     & 4,n)*x4y6z4*30.+c(6,6,5,n)*x4y6z5*30.+c(6,6,6,n)*x4y6z6*30.)*
     & time
! endLoops()
      else if( dx.eq.2.and.dy.eq.0.and.dz.eq.1.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
c 
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
      y6=y5*y1
      y6z1=y5z1*y1
      y6z2=y5z2*y1
      y6z3=y5z3*y1
      y6z4=y5z4*y1
      y6z5=y5z5*y1
c 
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
      x1y6=x1y5*y1
      x1y6z1=x1y5z1*y1
      x1y6z2=x1y5z2*y1
      x1y6z3=x1y5z3*y1
      x1y6z4=x1y5z4*y1
      x1y6z5=x1y5z5*y1
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
      x2y6=x1y6*x1
      x2y6z1=x1y6z1*x1
      x2y6z2=x1y6z2*x1
      x2y6z3=x1y6z3*x1
      x2y6z4=x1y6z4*x1
      x2y6z5=x1y6z5*x1
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
      x3y6=x2y6*x1
      x3y6z1=x2y6z1*x1
      x3y6z2=x2y6z2*x1
      x3y6z3=x2y6z3*x1
      x3y6z4=x2y6z4*x1
      x3y6z5=x2y6z5*x1
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
      x4y6=x3y6*x1
      x4y6z1=x3y6z1*x1
      x4y6z2=x3y6z2*x1
      x4y6z3=x3y6z3*x1
      x4y6z4=x3y6z4*x1
      x4y6z5=x3y6z5*x1
      r=(c(2,0,1,n)*2.+c(2,0,2,n)*z1*4.+c(2,0,3,n)*z2*6.+c(2,0,4,n)*z3*
     & 8.+c(2,0,5,n)*z4*10.+c(2,0,6,n)*z5*12.+c(2,1,1,n)*y1*2.+c(2,1,
     & 2,n)*y1z1*4.+c(2,1,3,n)*y1z2*6.+c(2,1,4,n)*y1z3*8.+c(2,1,5,n)*
     & y1z4*10.+c(2,1,6,n)*y1z5*12.+c(2,2,1,n)*y2*2.+c(2,2,2,n)*y2z1*
     & 4.+c(2,2,3,n)*y2z2*6.+c(2,2,4,n)*y2z3*8.+c(2,2,5,n)*y2z4*10.+c(
     & 2,2,6,n)*y2z5*12.+c(2,3,1,n)*y3*2.+c(2,3,2,n)*y3z1*4.+c(2,3,3,
     & n)*y3z2*6.+c(2,3,4,n)*y3z3*8.+c(2,3,5,n)*y3z4*10.+c(2,3,6,n)*
     & y3z5*12.+c(2,4,1,n)*y4*2.+c(2,4,2,n)*y4z1*4.+c(2,4,3,n)*y4z2*
     & 6.+c(2,4,4,n)*y4z3*8.+c(2,4,5,n)*y4z4*10.+c(2,4,6,n)*y4z5*12.+
     & c(2,5,1,n)*y5*2.+c(2,5,2,n)*y5z1*4.+c(2,5,3,n)*y5z2*6.+c(2,5,4,
     & n)*y5z3*8.+c(2,5,5,n)*y5z4*10.+c(2,5,6,n)*y5z5*12.+c(2,6,1,n)*
     & y6*2.+c(2,6,2,n)*y6z1*4.+c(2,6,3,n)*y6z2*6.+c(2,6,4,n)*y6z3*8.+
     & c(2,6,5,n)*y6z4*10.+c(2,6,6,n)*y6z5*12.+c(3,0,1,n)*x1*6.+c(3,0,
     & 2,n)*x1z1*12.+c(3,0,3,n)*x1z2*18.+c(3,0,4,n)*x1z3*24.+c(3,0,5,
     & n)*x1z4*30.+c(3,0,6,n)*x1z5*36.+c(3,1,1,n)*x1y1*6.+c(3,1,2,n)*
     & x1y1z1*12.+c(3,1,3,n)*x1y1z2*18.+c(3,1,4,n)*x1y1z3*24.+c(3,1,5,
     & n)*x1y1z4*30.+c(3,1,6,n)*x1y1z5*36.+c(3,2,1,n)*x1y2*6.+c(3,2,2,
     & n)*x1y2z1*12.+c(3,2,3,n)*x1y2z2*18.+c(3,2,4,n)*x1y2z3*24.+c(3,
     & 2,5,n)*x1y2z4*30.+c(3,2,6,n)*x1y2z5*36.+c(3,3,1,n)*x1y3*6.+c(3,
     & 3,2,n)*x1y3z1*12.+c(3,3,3,n)*x1y3z2*18.+c(3,3,4,n)*x1y3z3*24.+
     & c(3,3,5,n)*x1y3z4*30.+c(3,3,6,n)*x1y3z5*36.+c(3,4,1,n)*x1y4*6.+
     & c(3,4,2,n)*x1y4z1*12.+c(3,4,3,n)*x1y4z2*18.+c(3,4,4,n)*x1y4z3*
     & 24.+c(3,4,5,n)*x1y4z4*30.+c(3,4,6,n)*x1y4z5*36.+c(3,5,1,n)*
     & x1y5*6.+c(3,5,2,n)*x1y5z1*12.+c(3,5,3,n)*x1y5z2*18.+c(3,5,4,n)*
     & x1y5z3*24.+c(3,5,5,n)*x1y5z4*30.+c(3,5,6,n)*x1y5z5*36.+c(3,6,1,
     & n)*x1y6*6.+c(3,6,2,n)*x1y6z1*12.+c(3,6,3,n)*x1y6z2*18.+c(3,6,4,
     & n)*x1y6z3*24.+c(3,6,5,n)*x1y6z4*30.+c(3,6,6,n)*x1y6z5*36.+c(4,
     & 0,1,n)*x2*12.+c(4,0,2,n)*x2z1*24.+c(4,0,3,n)*x2z2*36.+c(4,0,4,
     & n)*x2z3*48.+c(4,0,5,n)*x2z4*60.+c(4,0,6,n)*x2z5*72.+c(4,1,1,n)*
     & x2y1*12.+c(4,1,2,n)*x2y1z1*24.+c(4,1,3,n)*x2y1z2*36.+c(4,1,4,n)
     & *x2y1z3*48.+c(4,1,5,n)*x2y1z4*60.+c(4,1,6,n)*x2y1z5*72.+c(4,2,
     & 1,n)*x2y2*12.+c(4,2,2,n)*x2y2z1*24.+c(4,2,3,n)*x2y2z2*36.+c(4,
     & 2,4,n)*x2y2z3*48.+c(4,2,5,n)*x2y2z4*60.+c(4,2,6,n)*x2y2z5*72.+
     & c(4,3,1,n)*x2y3*12.+c(4,3,2,n)*x2y3z1*24.+c(4,3,3,n)*x2y3z2*
     & 36.+c(4,3,4,n)*x2y3z3*48.+c(4,3,5,n)*x2y3z4*60.+c(4,3,6,n)*
     & x2y3z5*72.+c(4,4,1,n)*x2y4*12.+c(4,4,2,n)*x2y4z1*24.+c(4,4,3,n)
     & *x2y4z2*36.+c(4,4,4,n)*x2y4z3*48.+c(4,4,5,n)*x2y4z4*60.+c(4,4,
     & 6,n)*x2y4z5*72.+c(4,5,1,n)*x2y5*12.+c(4,5,2,n)*x2y5z1*24.+c(4,
     & 5,3,n)*x2y5z2*36.+c(4,5,4,n)*x2y5z3*48.+c(4,5,5,n)*x2y5z4*60.+
     & c(4,5,6,n)*x2y5z5*72.+c(4,6,1,n)*x2y6*12.+c(4,6,2,n)*x2y6z1*
     & 24.+c(4,6,3,n)*x2y6z2*36.+c(4,6,4,n)*x2y6z3*48.+c(4,6,5,n)*
     & x2y6z4*60.+c(4,6,6,n)*x2y6z5*72.+c(5,0,1,n)*x3*20.+c(5,0,2,n)*
     & x3z1*40.+c(5,0,3,n)*x3z2*60.+c(5,0,4,n)*x3z3*80.+c(5,0,5,n)*
     & x3z4*100.+c(5,0,6,n)*x3z5*120.+c(5,1,1,n)*x3y1*20.+c(5,1,2,n)*
     & x3y1z1*40.+c(5,1,3,n)*x3y1z2*60.+c(5,1,4,n)*x3y1z3*80.+c(5,1,5,
     & n)*x3y1z4*100.+c(5,1,6,n)*x3y1z5*120.+c(5,2,1,n)*x3y2*20.+c(5,
     & 2,2,n)*x3y2z1*40.+c(5,2,3,n)*x3y2z2*60.+c(5,2,4,n)*x3y2z3*80.+
     & c(5,2,5,n)*x3y2z4*100.+c(5,2,6,n)*x3y2z5*120.+c(5,3,1,n)*x3y3*
     & 20.+c(5,3,2,n)*x3y3z1*40.+c(5,3,3,n)*x3y3z2*60.+c(5,3,4,n)*
     & x3y3z3*80.+c(5,3,5,n)*x3y3z4*100.+c(5,3,6,n)*x3y3z5*120.+c(5,4,
     & 1,n)*x3y4*20.+c(5,4,2,n)*x3y4z1*40.+c(5,4,3,n)*x3y4z2*60.+c(5,
     & 4,4,n)*x3y4z3*80.+c(5,4,5,n)*x3y4z4*100.+c(5,4,6,n)*x3y4z5*
     & 120.+c(5,5,1,n)*x3y5*20.+c(5,5,2,n)*x3y5z1*40.+c(5,5,3,n)*
     & x3y5z2*60.+c(5,5,4,n)*x3y5z3*80.+c(5,5,5,n)*x3y5z4*100.+c(5,5,
     & 6,n)*x3y5z5*120.+c(5,6,1,n)*x3y6*20.+c(5,6,2,n)*x3y6z1*40.+c(5,
     & 6,3,n)*x3y6z2*60.+c(5,6,4,n)*x3y6z3*80.+c(5,6,5,n)*x3y6z4*100.+
     & c(5,6,6,n)*x3y6z5*120.+c(6,0,1,n)*x4*30.+c(6,0,2,n)*x4z1*60.+c(
     & 6,0,3,n)*x4z2*90.+c(6,0,4,n)*x4z3*120.+c(6,0,5,n)*x4z4*150.+c(
     & 6,0,6,n)*x4z5*180.+c(6,1,1,n)*x4y1*30.+c(6,1,2,n)*x4y1z1*60.+c(
     & 6,1,3,n)*x4y1z2*90.+c(6,1,4,n)*x4y1z3*120.+c(6,1,5,n)*x4y1z4*
     & 150.+c(6,1,6,n)*x4y1z5*180.+c(6,2,1,n)*x4y2*30.+c(6,2,2,n)*
     & x4y2z1*60.+c(6,2,3,n)*x4y2z2*90.+c(6,2,4,n)*x4y2z3*120.+c(6,2,
     & 5,n)*x4y2z4*150.+c(6,2,6,n)*x4y2z5*180.+c(6,3,1,n)*x4y3*30.+c(
     & 6,3,2,n)*x4y3z1*60.+c(6,3,3,n)*x4y3z2*90.+c(6,3,4,n)*x4y3z3*
     & 120.+c(6,3,5,n)*x4y3z4*150.+c(6,3,6,n)*x4y3z5*180.+c(6,4,1,n)*
     & x4y4*30.+c(6,4,2,n)*x4y4z1*60.+c(6,4,3,n)*x4y4z2*90.+c(6,4,4,n)
     & *x4y4z3*120.+c(6,4,5,n)*x4y4z4*150.+c(6,4,6,n)*x4y4z5*180.+c(6,
     & 5,1,n)*x4y5*30.+c(6,5,2,n)*x4y5z1*60.+c(6,5,3,n)*x4y5z2*90.+c(
     & 6,5,4,n)*x4y5z3*120.+c(6,5,5,n)*x4y5z4*150.+c(6,5,6,n)*x4y5z5*
     & 180.+c(6,6,1,n)*x4y6*30.+c(6,6,2,n)*x4y6z1*60.+c(6,6,3,n)*
     & x4y6z2*90.+c(6,6,4,n)*x4y6z3*120.+c(6,6,5,n)*x4y6z4*150.+c(6,6,
     & 6,n)*x4y6z5*180.)*time
! endLoops()
      else if( dx.eq.2.and.dy.eq.0.and.dz.eq.2.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
c 
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
      y6=y5*y1
      y6z1=y5z1*y1
      y6z2=y5z2*y1
      y6z3=y5z3*y1
      y6z4=y5z4*y1
c 
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
      x1y6=x1y5*y1
      x1y6z1=x1y5z1*y1
      x1y6z2=x1y5z2*y1
      x1y6z3=x1y5z3*y1
      x1y6z4=x1y5z4*y1
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
      x2y6=x1y6*x1
      x2y6z1=x1y6z1*x1
      x2y6z2=x1y6z2*x1
      x2y6z3=x1y6z3*x1
      x2y6z4=x1y6z4*x1
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
      x3y6=x2y6*x1
      x3y6z1=x2y6z1*x1
      x3y6z2=x2y6z2*x1
      x3y6z3=x2y6z3*x1
      x3y6z4=x2y6z4*x1
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
      x4y6=x3y6*x1
      x4y6z1=x3y6z1*x1
      x4y6z2=x3y6z2*x1
      x4y6z3=x3y6z3*x1
      x4y6z4=x3y6z4*x1
      r=(c(2,0,2,n)*4.+c(2,0,3,n)*z1*12.+c(2,0,4,n)*z2*24.+c(2,0,5,n)*
     & z3*40.+c(2,0,6,n)*z4*60.+c(2,1,2,n)*y1*4.+c(2,1,3,n)*y1z1*12.+
     & c(2,1,4,n)*y1z2*24.+c(2,1,5,n)*y1z3*40.+c(2,1,6,n)*y1z4*60.+c(
     & 2,2,2,n)*y2*4.+c(2,2,3,n)*y2z1*12.+c(2,2,4,n)*y2z2*24.+c(2,2,5,
     & n)*y2z3*40.+c(2,2,6,n)*y2z4*60.+c(2,3,2,n)*y3*4.+c(2,3,3,n)*
     & y3z1*12.+c(2,3,4,n)*y3z2*24.+c(2,3,5,n)*y3z3*40.+c(2,3,6,n)*
     & y3z4*60.+c(2,4,2,n)*y4*4.+c(2,4,3,n)*y4z1*12.+c(2,4,4,n)*y4z2*
     & 24.+c(2,4,5,n)*y4z3*40.+c(2,4,6,n)*y4z4*60.+c(2,5,2,n)*y5*4.+c(
     & 2,5,3,n)*y5z1*12.+c(2,5,4,n)*y5z2*24.+c(2,5,5,n)*y5z3*40.+c(2,
     & 5,6,n)*y5z4*60.+c(2,6,2,n)*y6*4.+c(2,6,3,n)*y6z1*12.+c(2,6,4,n)
     & *y6z2*24.+c(2,6,5,n)*y6z3*40.+c(2,6,6,n)*y6z4*60.+c(3,0,2,n)*
     & x1*12.+c(3,0,3,n)*x1z1*36.+c(3,0,4,n)*x1z2*72.+c(3,0,5,n)*x1z3*
     & 120.+c(3,0,6,n)*x1z4*180.+c(3,1,2,n)*x1y1*12.+c(3,1,3,n)*
     & x1y1z1*36.+c(3,1,4,n)*x1y1z2*72.+c(3,1,5,n)*x1y1z3*120.+c(3,1,
     & 6,n)*x1y1z4*180.+c(3,2,2,n)*x1y2*12.+c(3,2,3,n)*x1y2z1*36.+c(3,
     & 2,4,n)*x1y2z2*72.+c(3,2,5,n)*x1y2z3*120.+c(3,2,6,n)*x1y2z4*
     & 180.+c(3,3,2,n)*x1y3*12.+c(3,3,3,n)*x1y3z1*36.+c(3,3,4,n)*
     & x1y3z2*72.+c(3,3,5,n)*x1y3z3*120.+c(3,3,6,n)*x1y3z4*180.+c(3,4,
     & 2,n)*x1y4*12.+c(3,4,3,n)*x1y4z1*36.+c(3,4,4,n)*x1y4z2*72.+c(3,
     & 4,5,n)*x1y4z3*120.+c(3,4,6,n)*x1y4z4*180.+c(3,5,2,n)*x1y5*12.+
     & c(3,5,3,n)*x1y5z1*36.+c(3,5,4,n)*x1y5z2*72.+c(3,5,5,n)*x1y5z3*
     & 120.+c(3,5,6,n)*x1y5z4*180.+c(3,6,2,n)*x1y6*12.+c(3,6,3,n)*
     & x1y6z1*36.+c(3,6,4,n)*x1y6z2*72.+c(3,6,5,n)*x1y6z3*120.+c(3,6,
     & 6,n)*x1y6z4*180.+c(4,0,2,n)*x2*24.+c(4,0,3,n)*x2z1*72.+c(4,0,4,
     & n)*x2z2*144.+c(4,0,5,n)*x2z3*240.+c(4,0,6,n)*x2z4*360.+c(4,1,2,
     & n)*x2y1*24.+c(4,1,3,n)*x2y1z1*72.+c(4,1,4,n)*x2y1z2*144.+c(4,1,
     & 5,n)*x2y1z3*240.+c(4,1,6,n)*x2y1z4*360.+c(4,2,2,n)*x2y2*24.+c(
     & 4,2,3,n)*x2y2z1*72.+c(4,2,4,n)*x2y2z2*144.+c(4,2,5,n)*x2y2z3*
     & 240.+c(4,2,6,n)*x2y2z4*360.+c(4,3,2,n)*x2y3*24.+c(4,3,3,n)*
     & x2y3z1*72.+c(4,3,4,n)*x2y3z2*144.+c(4,3,5,n)*x2y3z3*240.+c(4,3,
     & 6,n)*x2y3z4*360.+c(4,4,2,n)*x2y4*24.+c(4,4,3,n)*x2y4z1*72.+c(4,
     & 4,4,n)*x2y4z2*144.+c(4,4,5,n)*x2y4z3*240.+c(4,4,6,n)*x2y4z4*
     & 360.+c(4,5,2,n)*x2y5*24.+c(4,5,3,n)*x2y5z1*72.+c(4,5,4,n)*
     & x2y5z2*144.+c(4,5,5,n)*x2y5z3*240.+c(4,5,6,n)*x2y5z4*360.+c(4,
     & 6,2,n)*x2y6*24.+c(4,6,3,n)*x2y6z1*72.+c(4,6,4,n)*x2y6z2*144.+c(
     & 4,6,5,n)*x2y6z3*240.+c(4,6,6,n)*x2y6z4*360.+c(5,0,2,n)*x3*40.+
     & c(5,0,3,n)*x3z1*120.+c(5,0,4,n)*x3z2*240.+c(5,0,5,n)*x3z3*400.+
     & c(5,0,6,n)*x3z4*600.+c(5,1,2,n)*x3y1*40.+c(5,1,3,n)*x3y1z1*
     & 120.+c(5,1,4,n)*x3y1z2*240.+c(5,1,5,n)*x3y1z3*400.+c(5,1,6,n)*
     & x3y1z4*600.+c(5,2,2,n)*x3y2*40.+c(5,2,3,n)*x3y2z1*120.+c(5,2,4,
     & n)*x3y2z2*240.+c(5,2,5,n)*x3y2z3*400.+c(5,2,6,n)*x3y2z4*600.+c(
     & 5,3,2,n)*x3y3*40.+c(5,3,3,n)*x3y3z1*120.+c(5,3,4,n)*x3y3z2*
     & 240.+c(5,3,5,n)*x3y3z3*400.+c(5,3,6,n)*x3y3z4*600.+c(5,4,2,n)*
     & x3y4*40.+c(5,4,3,n)*x3y4z1*120.+c(5,4,4,n)*x3y4z2*240.+c(5,4,5,
     & n)*x3y4z3*400.+c(5,4,6,n)*x3y4z4*600.+c(5,5,2,n)*x3y5*40.+c(5,
     & 5,3,n)*x3y5z1*120.+c(5,5,4,n)*x3y5z2*240.+c(5,5,5,n)*x3y5z3*
     & 400.+c(5,5,6,n)*x3y5z4*600.+c(5,6,2,n)*x3y6*40.+c(5,6,3,n)*
     & x3y6z1*120.+c(5,6,4,n)*x3y6z2*240.+c(5,6,5,n)*x3y6z3*400.+c(5,
     & 6,6,n)*x3y6z4*600.+c(6,0,2,n)*x4*60.+c(6,0,3,n)*x4z1*180.+c(6,
     & 0,4,n)*x4z2*360.+c(6,0,5,n)*x4z3*600.+c(6,0,6,n)*x4z4*900.+c(6,
     & 1,2,n)*x4y1*60.+c(6,1,3,n)*x4y1z1*180.+c(6,1,4,n)*x4y1z2*360.+
     & c(6,1,5,n)*x4y1z3*600.+c(6,1,6,n)*x4y1z4*900.+c(6,2,2,n)*x4y2*
     & 60.+c(6,2,3,n)*x4y2z1*180.+c(6,2,4,n)*x4y2z2*360.+c(6,2,5,n)*
     & x4y2z3*600.+c(6,2,6,n)*x4y2z4*900.+c(6,3,2,n)*x4y3*60.+c(6,3,3,
     & n)*x4y3z1*180.+c(6,3,4,n)*x4y3z2*360.+c(6,3,5,n)*x4y3z3*600.+c(
     & 6,3,6,n)*x4y3z4*900.+c(6,4,2,n)*x4y4*60.+c(6,4,3,n)*x4y4z1*
     & 180.+c(6,4,4,n)*x4y4z2*360.+c(6,4,5,n)*x4y4z3*600.+c(6,4,6,n)*
     & x4y4z4*900.+c(6,5,2,n)*x4y5*60.+c(6,5,3,n)*x4y5z1*180.+c(6,5,4,
     & n)*x4y5z2*360.+c(6,5,5,n)*x4y5z3*600.+c(6,5,6,n)*x4y5z4*900.+c(
     & 6,6,2,n)*x4y6*60.+c(6,6,3,n)*x4y6z1*180.+c(6,6,4,n)*x4y6z2*
     & 360.+c(6,6,5,n)*x4y6z3*600.+c(6,6,6,n)*x4y6z4*900.)*time
! endLoops()
      else if( dx.eq.2.and.dy.eq.1.and.dz.eq.0.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
      z6=z5*z1
c 
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y1z5=y1z4*z1
      y1z6=y1z5*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
      y2z5=y1z5*y1
      y2z6=y1z6*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      y3z4=y2z4*y1
      y3z5=y2z5*y1
      y3z6=y2z6*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y4z2=y3z2*y1
      y4z3=y3z3*y1
      y4z4=y3z4*y1
      y4z5=y3z5*y1
      y4z6=y3z6*y1
      y5=y4*y1
      y5z1=y4z1*y1
      y5z2=y4z2*y1
      y5z3=y4z3*y1
      y5z4=y4z4*y1
      y5z5=y4z5*y1
      y5z6=y4z6*y1
c 
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1z5=x1z4*z1
      x1z6=x1z5*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y1z5=x1y1z4*z1
      x1y1z6=x1y1z5*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y2z4=x1y1z4*y1
      x1y2z5=x1y1z5*y1
      x1y2z6=x1y1z6*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x1y3z4=x1y2z4*y1
      x1y3z5=x1y2z5*y1
      x1y3z6=x1y2z6*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y4z2=x1y3z2*y1
      x1y4z3=x1y3z3*y1
      x1y4z4=x1y3z4*y1
      x1y4z5=x1y3z5*y1
      x1y4z6=x1y3z6*y1
      x1y5=x1y4*y1
      x1y5z1=x1y4z1*y1
      x1y5z2=x1y4z2*y1
      x1y5z3=x1y4z3*y1
      x1y5z4=x1y4z4*y1
      x1y5z5=x1y4z5*y1
      x1y5z6=x1y4z6*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x2z5=x1z5*x1
      x2z6=x1z6*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y1z4=x1y1z4*x1
      x2y1z5=x1y1z5*x1
      x2y1z6=x1y1z6*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y2z4=x1y2z4*x1
      x2y2z5=x1y2z5*x1
      x2y2z6=x1y2z6*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y3z3=x1y3z3*x1
      x2y3z4=x1y3z4*x1
      x2y3z5=x1y3z5*x1
      x2y3z6=x1y3z6*x1
      x2y4=x1y4*x1
      x2y4z1=x1y4z1*x1
      x2y4z2=x1y4z2*x1
      x2y4z3=x1y4z3*x1
      x2y4z4=x1y4z4*x1
      x2y4z5=x1y4z5*x1
      x2y4z6=x1y4z6*x1
      x2y5=x1y5*x1
      x2y5z1=x1y5z1*x1
      x2y5z2=x1y5z2*x1
      x2y5z3=x1y5z3*x1
      x2y5z4=x1y5z4*x1
      x2y5z5=x1y5z5*x1
      x2y5z6=x1y5z6*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3z4=x2z4*x1
      x3z5=x2z5*x1
      x3z6=x2z6*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x3y1z4=x2y1z4*x1
      x3y1z5=x2y1z5*x1
      x3y1z6=x2y1z6*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y2z3=x2y2z3*x1
      x3y2z4=x2y2z4*x1
      x3y2z5=x2y2z5*x1
      x3y2z6=x2y2z6*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x3y3z2=x2y3z2*x1
      x3y3z3=x2y3z3*x1
      x3y3z4=x2y3z4*x1
      x3y3z5=x2y3z5*x1
      x3y3z6=x2y3z6*x1
      x3y4=x2y4*x1
      x3y4z1=x2y4z1*x1
      x3y4z2=x2y4z2*x1
      x3y4z3=x2y4z3*x1
      x3y4z4=x2y4z4*x1
      x3y4z5=x2y4z5*x1
      x3y4z6=x2y4z6*x1
      x3y5=x2y5*x1
      x3y5z1=x2y5z1*x1
      x3y5z2=x2y5z2*x1
      x3y5z3=x2y5z3*x1
      x3y5z4=x2y5z4*x1
      x3y5z5=x2y5z5*x1
      x3y5z6=x2y5z6*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4z2=x3z2*x1
      x4z3=x3z3*x1
      x4z4=x3z4*x1
      x4z5=x3z5*x1
      x4z6=x3z6*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y1z2=x3y1z2*x1
      x4y1z3=x3y1z3*x1
      x4y1z4=x3y1z4*x1
      x4y1z5=x3y1z5*x1
      x4y1z6=x3y1z6*x1
      x4y2=x3y2*x1
      x4y2z1=x3y2z1*x1
      x4y2z2=x3y2z2*x1
      x4y2z3=x3y2z3*x1
      x4y2z4=x3y2z4*x1
      x4y2z5=x3y2z5*x1
      x4y2z6=x3y2z6*x1
      x4y3=x3y3*x1
      x4y3z1=x3y3z1*x1
      x4y3z2=x3y3z2*x1
      x4y3z3=x3y3z3*x1
      x4y3z4=x3y3z4*x1
      x4y3z5=x3y3z5*x1
      x4y3z6=x3y3z6*x1
      x4y4=x3y4*x1
      x4y4z1=x3y4z1*x1
      x4y4z2=x3y4z2*x1
      x4y4z3=x3y4z3*x1
      x4y4z4=x3y4z4*x1
      x4y4z5=x3y4z5*x1
      x4y4z6=x3y4z6*x1
      x4y5=x3y5*x1
      x4y5z1=x3y5z1*x1
      x4y5z2=x3y5z2*x1
      x4y5z3=x3y5z3*x1
      x4y5z4=x3y5z4*x1
      x4y5z5=x3y5z5*x1
      x4y5z6=x3y5z6*x1
      r=(c(2,1,0,n)*2.+c(2,1,1,n)*z1*2.+c(2,1,2,n)*z2*2.+c(2,1,3,n)*z3*
     & 2.+c(2,1,4,n)*z4*2.+c(2,1,5,n)*z5*2.+c(2,1,6,n)*z6*2.+c(2,2,0,
     & n)*y1*4.+c(2,2,1,n)*y1z1*4.+c(2,2,2,n)*y1z2*4.+c(2,2,3,n)*y1z3*
     & 4.+c(2,2,4,n)*y1z4*4.+c(2,2,5,n)*y1z5*4.+c(2,2,6,n)*y1z6*4.+c(
     & 2,3,0,n)*y2*6.+c(2,3,1,n)*y2z1*6.+c(2,3,2,n)*y2z2*6.+c(2,3,3,n)
     & *y2z3*6.+c(2,3,4,n)*y2z4*6.+c(2,3,5,n)*y2z5*6.+c(2,3,6,n)*y2z6*
     & 6.+c(2,4,0,n)*y3*8.+c(2,4,1,n)*y3z1*8.+c(2,4,2,n)*y3z2*8.+c(2,
     & 4,3,n)*y3z3*8.+c(2,4,4,n)*y3z4*8.+c(2,4,5,n)*y3z5*8.+c(2,4,6,n)
     & *y3z6*8.+c(2,5,0,n)*y4*10.+c(2,5,1,n)*y4z1*10.+c(2,5,2,n)*y4z2*
     & 10.+c(2,5,3,n)*y4z3*10.+c(2,5,4,n)*y4z4*10.+c(2,5,5,n)*y4z5*
     & 10.+c(2,5,6,n)*y4z6*10.+c(2,6,0,n)*y5*12.+c(2,6,1,n)*y5z1*12.+
     & c(2,6,2,n)*y5z2*12.+c(2,6,3,n)*y5z3*12.+c(2,6,4,n)*y5z4*12.+c(
     & 2,6,5,n)*y5z5*12.+c(2,6,6,n)*y5z6*12.+c(3,1,0,n)*x1*6.+c(3,1,1,
     & n)*x1z1*6.+c(3,1,2,n)*x1z2*6.+c(3,1,3,n)*x1z3*6.+c(3,1,4,n)*
     & x1z4*6.+c(3,1,5,n)*x1z5*6.+c(3,1,6,n)*x1z6*6.+c(3,2,0,n)*x1y1*
     & 12.+c(3,2,1,n)*x1y1z1*12.+c(3,2,2,n)*x1y1z2*12.+c(3,2,3,n)*
     & x1y1z3*12.+c(3,2,4,n)*x1y1z4*12.+c(3,2,5,n)*x1y1z5*12.+c(3,2,6,
     & n)*x1y1z6*12.+c(3,3,0,n)*x1y2*18.+c(3,3,1,n)*x1y2z1*18.+c(3,3,
     & 2,n)*x1y2z2*18.+c(3,3,3,n)*x1y2z3*18.+c(3,3,4,n)*x1y2z4*18.+c(
     & 3,3,5,n)*x1y2z5*18.+c(3,3,6,n)*x1y2z6*18.+c(3,4,0,n)*x1y3*24.+
     & c(3,4,1,n)*x1y3z1*24.+c(3,4,2,n)*x1y3z2*24.+c(3,4,3,n)*x1y3z3*
     & 24.+c(3,4,4,n)*x1y3z4*24.+c(3,4,5,n)*x1y3z5*24.+c(3,4,6,n)*
     & x1y3z6*24.+c(3,5,0,n)*x1y4*30.+c(3,5,1,n)*x1y4z1*30.+c(3,5,2,n)
     & *x1y4z2*30.+c(3,5,3,n)*x1y4z3*30.+c(3,5,4,n)*x1y4z4*30.+c(3,5,
     & 5,n)*x1y4z5*30.+c(3,5,6,n)*x1y4z6*30.+c(3,6,0,n)*x1y5*36.+c(3,
     & 6,1,n)*x1y5z1*36.+c(3,6,2,n)*x1y5z2*36.+c(3,6,3,n)*x1y5z3*36.+
     & c(3,6,4,n)*x1y5z4*36.+c(3,6,5,n)*x1y5z5*36.+c(3,6,6,n)*x1y5z6*
     & 36.+c(4,1,0,n)*x2*12.+c(4,1,1,n)*x2z1*12.+c(4,1,2,n)*x2z2*12.+
     & c(4,1,3,n)*x2z3*12.+c(4,1,4,n)*x2z4*12.+c(4,1,5,n)*x2z5*12.+c(
     & 4,1,6,n)*x2z6*12.+c(4,2,0,n)*x2y1*24.+c(4,2,1,n)*x2y1z1*24.+c(
     & 4,2,2,n)*x2y1z2*24.+c(4,2,3,n)*x2y1z3*24.+c(4,2,4,n)*x2y1z4*
     & 24.+c(4,2,5,n)*x2y1z5*24.+c(4,2,6,n)*x2y1z6*24.+c(4,3,0,n)*
     & x2y2*36.+c(4,3,1,n)*x2y2z1*36.+c(4,3,2,n)*x2y2z2*36.+c(4,3,3,n)
     & *x2y2z3*36.+c(4,3,4,n)*x2y2z4*36.+c(4,3,5,n)*x2y2z5*36.+c(4,3,
     & 6,n)*x2y2z6*36.+c(4,4,0,n)*x2y3*48.+c(4,4,1,n)*x2y3z1*48.+c(4,
     & 4,2,n)*x2y3z2*48.+c(4,4,3,n)*x2y3z3*48.+c(4,4,4,n)*x2y3z4*48.+
     & c(4,4,5,n)*x2y3z5*48.+c(4,4,6,n)*x2y3z6*48.+c(4,5,0,n)*x2y4*
     & 60.+c(4,5,1,n)*x2y4z1*60.+c(4,5,2,n)*x2y4z2*60.+c(4,5,3,n)*
     & x2y4z3*60.+c(4,5,4,n)*x2y4z4*60.+c(4,5,5,n)*x2y4z5*60.+c(4,5,6,
     & n)*x2y4z6*60.+c(4,6,0,n)*x2y5*72.+c(4,6,1,n)*x2y5z1*72.+c(4,6,
     & 2,n)*x2y5z2*72.+c(4,6,3,n)*x2y5z3*72.+c(4,6,4,n)*x2y5z4*72.+c(
     & 4,6,5,n)*x2y5z5*72.+c(4,6,6,n)*x2y5z6*72.+c(5,1,0,n)*x3*20.+c(
     & 5,1,1,n)*x3z1*20.+c(5,1,2,n)*x3z2*20.+c(5,1,3,n)*x3z3*20.+c(5,
     & 1,4,n)*x3z4*20.+c(5,1,5,n)*x3z5*20.+c(5,1,6,n)*x3z6*20.+c(5,2,
     & 0,n)*x3y1*40.+c(5,2,1,n)*x3y1z1*40.+c(5,2,2,n)*x3y1z2*40.+c(5,
     & 2,3,n)*x3y1z3*40.+c(5,2,4,n)*x3y1z4*40.+c(5,2,5,n)*x3y1z5*40.+
     & c(5,2,6,n)*x3y1z6*40.+c(5,3,0,n)*x3y2*60.+c(5,3,1,n)*x3y2z1*
     & 60.+c(5,3,2,n)*x3y2z2*60.+c(5,3,3,n)*x3y2z3*60.+c(5,3,4,n)*
     & x3y2z4*60.+c(5,3,5,n)*x3y2z5*60.+c(5,3,6,n)*x3y2z6*60.+c(5,4,0,
     & n)*x3y3*80.+c(5,4,1,n)*x3y3z1*80.+c(5,4,2,n)*x3y3z2*80.+c(5,4,
     & 3,n)*x3y3z3*80.+c(5,4,4,n)*x3y3z4*80.+c(5,4,5,n)*x3y3z5*80.+c(
     & 5,4,6,n)*x3y3z6*80.+c(5,5,0,n)*x3y4*100.+c(5,5,1,n)*x3y4z1*
     & 100.+c(5,5,2,n)*x3y4z2*100.+c(5,5,3,n)*x3y4z3*100.+c(5,5,4,n)*
     & x3y4z4*100.+c(5,5,5,n)*x3y4z5*100.+c(5,5,6,n)*x3y4z6*100.+c(5,
     & 6,0,n)*x3y5*120.+c(5,6,1,n)*x3y5z1*120.+c(5,6,2,n)*x3y5z2*120.+
     & c(5,6,3,n)*x3y5z3*120.+c(5,6,4,n)*x3y5z4*120.+c(5,6,5,n)*
     & x3y5z5*120.+c(5,6,6,n)*x3y5z6*120.+c(6,1,0,n)*x4*30.+c(6,1,1,n)
     & *x4z1*30.+c(6,1,2,n)*x4z2*30.+c(6,1,3,n)*x4z3*30.+c(6,1,4,n)*
     & x4z4*30.+c(6,1,5,n)*x4z5*30.+c(6,1,6,n)*x4z6*30.+c(6,2,0,n)*
     & x4y1*60.+c(6,2,1,n)*x4y1z1*60.+c(6,2,2,n)*x4y1z2*60.+c(6,2,3,n)
     & *x4y1z3*60.+c(6,2,4,n)*x4y1z4*60.+c(6,2,5,n)*x4y1z5*60.+c(6,2,
     & 6,n)*x4y1z6*60.+c(6,3,0,n)*x4y2*90.+c(6,3,1,n)*x4y2z1*90.+c(6,
     & 3,2,n)*x4y2z2*90.+c(6,3,3,n)*x4y2z3*90.+c(6,3,4,n)*x4y2z4*90.+
     & c(6,3,5,n)*x4y2z5*90.+c(6,3,6,n)*x4y2z6*90.+c(6,4,0,n)*x4y3*
     & 120.+c(6,4,1,n)*x4y3z1*120.+c(6,4,2,n)*x4y3z2*120.+c(6,4,3,n)*
     & x4y3z3*120.+c(6,4,4,n)*x4y3z4*120.+c(6,4,5,n)*x4y3z5*120.+c(6,
     & 4,6,n)*x4y3z6*120.+c(6,5,0,n)*x4y4*150.+c(6,5,1,n)*x4y4z1*150.+
     & c(6,5,2,n)*x4y4z2*150.+c(6,5,3,n)*x4y4z3*150.+c(6,5,4,n)*
     & x4y4z4*150.+c(6,5,5,n)*x4y4z5*150.+c(6,5,6,n)*x4y4z6*150.+c(6,
     & 6,0,n)*x4y5*180.+c(6,6,1,n)*x4y5z1*180.+c(6,6,2,n)*x4y5z2*180.+
     & c(6,6,3,n)*x4y5z3*180.+c(6,6,4,n)*x4y5z4*180.+c(6,6,5,n)*
     & x4y5z5*180.+c(6,6,6,n)*x4y5z6*180.)*time
! endLoops()
      else if( dx.eq.2.and.dy.eq.1.and.dz.eq.1.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
c 
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
c 
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
      r=(c(2,1,1,n)*2.+c(2,1,2,n)*z1*4.+c(2,1,3,n)*z2*6.+c(2,1,4,n)*z3*
     & 8.+c(2,1,5,n)*z4*10.+c(2,1,6,n)*z5*12.+c(2,2,1,n)*y1*4.+c(2,2,
     & 2,n)*y1z1*8.+c(2,2,3,n)*y1z2*12.+c(2,2,4,n)*y1z3*16.+c(2,2,5,n)
     & *y1z4*20.+c(2,2,6,n)*y1z5*24.+c(2,3,1,n)*y2*6.+c(2,3,2,n)*y2z1*
     & 12.+c(2,3,3,n)*y2z2*18.+c(2,3,4,n)*y2z3*24.+c(2,3,5,n)*y2z4*
     & 30.+c(2,3,6,n)*y2z5*36.+c(2,4,1,n)*y3*8.+c(2,4,2,n)*y3z1*16.+c(
     & 2,4,3,n)*y3z2*24.+c(2,4,4,n)*y3z3*32.+c(2,4,5,n)*y3z4*40.+c(2,
     & 4,6,n)*y3z5*48.+c(2,5,1,n)*y4*10.+c(2,5,2,n)*y4z1*20.+c(2,5,3,
     & n)*y4z2*30.+c(2,5,4,n)*y4z3*40.+c(2,5,5,n)*y4z4*50.+c(2,5,6,n)*
     & y4z5*60.+c(2,6,1,n)*y5*12.+c(2,6,2,n)*y5z1*24.+c(2,6,3,n)*y5z2*
     & 36.+c(2,6,4,n)*y5z3*48.+c(2,6,5,n)*y5z4*60.+c(2,6,6,n)*y5z5*
     & 72.+c(3,1,1,n)*x1*6.+c(3,1,2,n)*x1z1*12.+c(3,1,3,n)*x1z2*18.+c(
     & 3,1,4,n)*x1z3*24.+c(3,1,5,n)*x1z4*30.+c(3,1,6,n)*x1z5*36.+c(3,
     & 2,1,n)*x1y1*12.+c(3,2,2,n)*x1y1z1*24.+c(3,2,3,n)*x1y1z2*36.+c(
     & 3,2,4,n)*x1y1z3*48.+c(3,2,5,n)*x1y1z4*60.+c(3,2,6,n)*x1y1z5*
     & 72.+c(3,3,1,n)*x1y2*18.+c(3,3,2,n)*x1y2z1*36.+c(3,3,3,n)*
     & x1y2z2*54.+c(3,3,4,n)*x1y2z3*72.+c(3,3,5,n)*x1y2z4*90.+c(3,3,6,
     & n)*x1y2z5*108.+c(3,4,1,n)*x1y3*24.+c(3,4,2,n)*x1y3z1*48.+c(3,4,
     & 3,n)*x1y3z2*72.+c(3,4,4,n)*x1y3z3*96.+c(3,4,5,n)*x1y3z4*120.+c(
     & 3,4,6,n)*x1y3z5*144.+c(3,5,1,n)*x1y4*30.+c(3,5,2,n)*x1y4z1*60.+
     & c(3,5,3,n)*x1y4z2*90.+c(3,5,4,n)*x1y4z3*120.+c(3,5,5,n)*x1y4z4*
     & 150.+c(3,5,6,n)*x1y4z5*180.+c(3,6,1,n)*x1y5*36.+c(3,6,2,n)*
     & x1y5z1*72.+c(3,6,3,n)*x1y5z2*108.+c(3,6,4,n)*x1y5z3*144.+c(3,6,
     & 5,n)*x1y5z4*180.+c(3,6,6,n)*x1y5z5*216.+c(4,1,1,n)*x2*12.+c(4,
     & 1,2,n)*x2z1*24.+c(4,1,3,n)*x2z2*36.+c(4,1,4,n)*x2z3*48.+c(4,1,
     & 5,n)*x2z4*60.+c(4,1,6,n)*x2z5*72.+c(4,2,1,n)*x2y1*24.+c(4,2,2,
     & n)*x2y1z1*48.+c(4,2,3,n)*x2y1z2*72.+c(4,2,4,n)*x2y1z3*96.+c(4,
     & 2,5,n)*x2y1z4*120.+c(4,2,6,n)*x2y1z5*144.+c(4,3,1,n)*x2y2*36.+
     & c(4,3,2,n)*x2y2z1*72.+c(4,3,3,n)*x2y2z2*108.+c(4,3,4,n)*x2y2z3*
     & 144.+c(4,3,5,n)*x2y2z4*180.+c(4,3,6,n)*x2y2z5*216.+c(4,4,1,n)*
     & x2y3*48.+c(4,4,2,n)*x2y3z1*96.+c(4,4,3,n)*x2y3z2*144.+c(4,4,4,
     & n)*x2y3z3*192.+c(4,4,5,n)*x2y3z4*240.+c(4,4,6,n)*x2y3z5*288.+c(
     & 4,5,1,n)*x2y4*60.+c(4,5,2,n)*x2y4z1*120.+c(4,5,3,n)*x2y4z2*
     & 180.+c(4,5,4,n)*x2y4z3*240.+c(4,5,5,n)*x2y4z4*300.+c(4,5,6,n)*
     & x2y4z5*360.+c(4,6,1,n)*x2y5*72.+c(4,6,2,n)*x2y5z1*144.+c(4,6,3,
     & n)*x2y5z2*216.+c(4,6,4,n)*x2y5z3*288.+c(4,6,5,n)*x2y5z4*360.+c(
     & 4,6,6,n)*x2y5z5*432.+c(5,1,1,n)*x3*20.+c(5,1,2,n)*x3z1*40.+c(5,
     & 1,3,n)*x3z2*60.+c(5,1,4,n)*x3z3*80.+c(5,1,5,n)*x3z4*100.+c(5,1,
     & 6,n)*x3z5*120.+c(5,2,1,n)*x3y1*40.+c(5,2,2,n)*x3y1z1*80.+c(5,2,
     & 3,n)*x3y1z2*120.+c(5,2,4,n)*x3y1z3*160.+c(5,2,5,n)*x3y1z4*200.+
     & c(5,2,6,n)*x3y1z5*240.+c(5,3,1,n)*x3y2*60.+c(5,3,2,n)*x3y2z1*
     & 120.+c(5,3,3,n)*x3y2z2*180.+c(5,3,4,n)*x3y2z3*240.+c(5,3,5,n)*
     & x3y2z4*300.+c(5,3,6,n)*x3y2z5*360.+c(5,4,1,n)*x3y3*80.+c(5,4,2,
     & n)*x3y3z1*160.+c(5,4,3,n)*x3y3z2*240.+c(5,4,4,n)*x3y3z3*320.+c(
     & 5,4,5,n)*x3y3z4*400.+c(5,4,6,n)*x3y3z5*480.+c(5,5,1,n)*x3y4*
     & 100.+c(5,5,2,n)*x3y4z1*200.+c(5,5,3,n)*x3y4z2*300.+c(5,5,4,n)*
     & x3y4z3*400.+c(5,5,5,n)*x3y4z4*500.+c(5,5,6,n)*x3y4z5*600.+c(5,
     & 6,1,n)*x3y5*120.+c(5,6,2,n)*x3y5z1*240.+c(5,6,3,n)*x3y5z2*360.+
     & c(5,6,4,n)*x3y5z3*480.+c(5,6,5,n)*x3y5z4*600.+c(5,6,6,n)*
     & x3y5z5*720.+c(6,1,1,n)*x4*30.+c(6,1,2,n)*x4z1*60.+c(6,1,3,n)*
     & x4z2*90.+c(6,1,4,n)*x4z3*120.+c(6,1,5,n)*x4z4*150.+c(6,1,6,n)*
     & x4z5*180.+c(6,2,1,n)*x4y1*60.+c(6,2,2,n)*x4y1z1*120.+c(6,2,3,n)
     & *x4y1z2*180.+c(6,2,4,n)*x4y1z3*240.+c(6,2,5,n)*x4y1z4*300.+c(6,
     & 2,6,n)*x4y1z5*360.+c(6,3,1,n)*x4y2*90.+c(6,3,2,n)*x4y2z1*180.+
     & c(6,3,3,n)*x4y2z2*270.+c(6,3,4,n)*x4y2z3*360.+c(6,3,5,n)*
     & x4y2z4*450.+c(6,3,6,n)*x4y2z5*540.+c(6,4,1,n)*x4y3*120.+c(6,4,
     & 2,n)*x4y3z1*240.+c(6,4,3,n)*x4y3z2*360.+c(6,4,4,n)*x4y3z3*480.+
     & c(6,4,5,n)*x4y3z4*600.+c(6,4,6,n)*x4y3z5*720.+c(6,5,1,n)*x4y4*
     & 150.+c(6,5,2,n)*x4y4z1*300.+c(6,5,3,n)*x4y4z2*450.+c(6,5,4,n)*
     & x4y4z3*600.+c(6,5,5,n)*x4y4z4*750.+c(6,5,6,n)*x4y4z5*900.+c(6,
     & 6,1,n)*x4y5*180.+c(6,6,2,n)*x4y5z1*360.+c(6,6,3,n)*x4y5z2*540.+
     & c(6,6,4,n)*x4y5z3*720.+c(6,6,5,n)*x4y5z4*900.+c(6,6,6,n)*
     & x4y5z5*1080.)*time
! endLoops()
      else if( dx.eq.2.and.dy.eq.1.and.dz.eq.2.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
c 
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
c 
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
      r=(c(2,1,2,n)*4.+c(2,1,3,n)*z1*12.+c(2,1,4,n)*z2*24.+c(2,1,5,n)*
     & z3*40.+c(2,1,6,n)*z4*60.+c(2,2,2,n)*y1*8.+c(2,2,3,n)*y1z1*24.+
     & c(2,2,4,n)*y1z2*48.+c(2,2,5,n)*y1z3*80.+c(2,2,6,n)*y1z4*120.+c(
     & 2,3,2,n)*y2*12.+c(2,3,3,n)*y2z1*36.+c(2,3,4,n)*y2z2*72.+c(2,3,
     & 5,n)*y2z3*120.+c(2,3,6,n)*y2z4*180.+c(2,4,2,n)*y3*16.+c(2,4,3,
     & n)*y3z1*48.+c(2,4,4,n)*y3z2*96.+c(2,4,5,n)*y3z3*160.+c(2,4,6,n)
     & *y3z4*240.+c(2,5,2,n)*y4*20.+c(2,5,3,n)*y4z1*60.+c(2,5,4,n)*
     & y4z2*120.+c(2,5,5,n)*y4z3*200.+c(2,5,6,n)*y4z4*300.+c(2,6,2,n)*
     & y5*24.+c(2,6,3,n)*y5z1*72.+c(2,6,4,n)*y5z2*144.+c(2,6,5,n)*
     & y5z3*240.+c(2,6,6,n)*y5z4*360.+c(3,1,2,n)*x1*12.+c(3,1,3,n)*
     & x1z1*36.+c(3,1,4,n)*x1z2*72.+c(3,1,5,n)*x1z3*120.+c(3,1,6,n)*
     & x1z4*180.+c(3,2,2,n)*x1y1*24.+c(3,2,3,n)*x1y1z1*72.+c(3,2,4,n)*
     & x1y1z2*144.+c(3,2,5,n)*x1y1z3*240.+c(3,2,6,n)*x1y1z4*360.+c(3,
     & 3,2,n)*x1y2*36.+c(3,3,3,n)*x1y2z1*108.+c(3,3,4,n)*x1y2z2*216.+
     & c(3,3,5,n)*x1y2z3*360.+c(3,3,6,n)*x1y2z4*540.+c(3,4,2,n)*x1y3*
     & 48.+c(3,4,3,n)*x1y3z1*144.+c(3,4,4,n)*x1y3z2*288.+c(3,4,5,n)*
     & x1y3z3*480.+c(3,4,6,n)*x1y3z4*720.+c(3,5,2,n)*x1y4*60.+c(3,5,3,
     & n)*x1y4z1*180.+c(3,5,4,n)*x1y4z2*360.+c(3,5,5,n)*x1y4z3*600.+c(
     & 3,5,6,n)*x1y4z4*900.+c(3,6,2,n)*x1y5*72.+c(3,6,3,n)*x1y5z1*
     & 216.+c(3,6,4,n)*x1y5z2*432.+c(3,6,5,n)*x1y5z3*720.+c(3,6,6,n)*
     & x1y5z4*1080.+c(4,1,2,n)*x2*24.+c(4,1,3,n)*x2z1*72.+c(4,1,4,n)*
     & x2z2*144.+c(4,1,5,n)*x2z3*240.+c(4,1,6,n)*x2z4*360.+c(4,2,2,n)*
     & x2y1*48.+c(4,2,3,n)*x2y1z1*144.+c(4,2,4,n)*x2y1z2*288.+c(4,2,5,
     & n)*x2y1z3*480.+c(4,2,6,n)*x2y1z4*720.+c(4,3,2,n)*x2y2*72.+c(4,
     & 3,3,n)*x2y2z1*216.+c(4,3,4,n)*x2y2z2*432.+c(4,3,5,n)*x2y2z3*
     & 720.+c(4,3,6,n)*x2y2z4*1080.+c(4,4,2,n)*x2y3*96.+c(4,4,3,n)*
     & x2y3z1*288.+c(4,4,4,n)*x2y3z2*576.+c(4,4,5,n)*x2y3z3*960.+c(4,
     & 4,6,n)*x2y3z4*1440.+c(4,5,2,n)*x2y4*120.+c(4,5,3,n)*x2y4z1*
     & 360.+c(4,5,4,n)*x2y4z2*720.+c(4,5,5,n)*x2y4z3*1200.+c(4,5,6,n)*
     & x2y4z4*1800.+c(4,6,2,n)*x2y5*144.+c(4,6,3,n)*x2y5z1*432.+c(4,6,
     & 4,n)*x2y5z2*864.+c(4,6,5,n)*x2y5z3*1440.+c(4,6,6,n)*x2y5z4*
     & 2160.+c(5,1,2,n)*x3*40.+c(5,1,3,n)*x3z1*120.+c(5,1,4,n)*x3z2*
     & 240.+c(5,1,5,n)*x3z3*400.+c(5,1,6,n)*x3z4*600.+c(5,2,2,n)*x3y1*
     & 80.+c(5,2,3,n)*x3y1z1*240.+c(5,2,4,n)*x3y1z2*480.+c(5,2,5,n)*
     & x3y1z3*800.+c(5,2,6,n)*x3y1z4*1200.+c(5,3,2,n)*x3y2*120.+c(5,3,
     & 3,n)*x3y2z1*360.+c(5,3,4,n)*x3y2z2*720.+c(5,3,5,n)*x3y2z3*
     & 1200.+c(5,3,6,n)*x3y2z4*1800.+c(5,4,2,n)*x3y3*160.+c(5,4,3,n)*
     & x3y3z1*480.+c(5,4,4,n)*x3y3z2*960.+c(5,4,5,n)*x3y3z3*1600.+c(5,
     & 4,6,n)*x3y3z4*2400.+c(5,5,2,n)*x3y4*200.+c(5,5,3,n)*x3y4z1*
     & 600.+c(5,5,4,n)*x3y4z2*1200.+c(5,5,5,n)*x3y4z3*2000.+c(5,5,6,n)
     & *x3y4z4*3000.+c(5,6,2,n)*x3y5*240.+c(5,6,3,n)*x3y5z1*720.+c(5,
     & 6,4,n)*x3y5z2*1440.+c(5,6,5,n)*x3y5z3*2400.+c(5,6,6,n)*x3y5z4*
     & 3600.+c(6,1,2,n)*x4*60.+c(6,1,3,n)*x4z1*180.+c(6,1,4,n)*x4z2*
     & 360.+c(6,1,5,n)*x4z3*600.+c(6,1,6,n)*x4z4*900.+c(6,2,2,n)*x4y1*
     & 120.+c(6,2,3,n)*x4y1z1*360.+c(6,2,4,n)*x4y1z2*720.+c(6,2,5,n)*
     & x4y1z3*1200.+c(6,2,6,n)*x4y1z4*1800.+c(6,3,2,n)*x4y2*180.+c(6,
     & 3,3,n)*x4y2z1*540.+c(6,3,4,n)*x4y2z2*1080.+c(6,3,5,n)*x4y2z3*
     & 1800.+c(6,3,6,n)*x4y2z4*2700.+c(6,4,2,n)*x4y3*240.+c(6,4,3,n)*
     & x4y3z1*720.+c(6,4,4,n)*x4y3z2*1440.+c(6,4,5,n)*x4y3z3*2400.+c(
     & 6,4,6,n)*x4y3z4*3600.+c(6,5,2,n)*x4y4*300.+c(6,5,3,n)*x4y4z1*
     & 900.+c(6,5,4,n)*x4y4z2*1800.+c(6,5,5,n)*x4y4z3*3000.+c(6,5,6,n)
     & *x4y4z4*4500.+c(6,6,2,n)*x4y5*360.+c(6,6,3,n)*x4y5z1*1080.+c(6,
     & 6,4,n)*x4y5z2*2160.+c(6,6,5,n)*x4y5z3*3600.+c(6,6,6,n)*x4y5z4*
     & 5400.)*time
! endLoops()
      else if( dx.eq.2.and.dy.eq.2.and.dz.eq.0.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
      z6=z5*z1
c 
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y1z5=y1z4*z1
      y1z6=y1z5*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
      y2z5=y1z5*y1
      y2z6=y1z6*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      y3z4=y2z4*y1
      y3z5=y2z5*y1
      y3z6=y2z6*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y4z2=y3z2*y1
      y4z3=y3z3*y1
      y4z4=y3z4*y1
      y4z5=y3z5*y1
      y4z6=y3z6*y1
c 
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1z5=x1z4*z1
      x1z6=x1z5*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y1z5=x1y1z4*z1
      x1y1z6=x1y1z5*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y2z4=x1y1z4*y1
      x1y2z5=x1y1z5*y1
      x1y2z6=x1y1z6*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x1y3z4=x1y2z4*y1
      x1y3z5=x1y2z5*y1
      x1y3z6=x1y2z6*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y4z2=x1y3z2*y1
      x1y4z3=x1y3z3*y1
      x1y4z4=x1y3z4*y1
      x1y4z5=x1y3z5*y1
      x1y4z6=x1y3z6*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x2z5=x1z5*x1
      x2z6=x1z6*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y1z4=x1y1z4*x1
      x2y1z5=x1y1z5*x1
      x2y1z6=x1y1z6*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y2z4=x1y2z4*x1
      x2y2z5=x1y2z5*x1
      x2y2z6=x1y2z6*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y3z3=x1y3z3*x1
      x2y3z4=x1y3z4*x1
      x2y3z5=x1y3z5*x1
      x2y3z6=x1y3z6*x1
      x2y4=x1y4*x1
      x2y4z1=x1y4z1*x1
      x2y4z2=x1y4z2*x1
      x2y4z3=x1y4z3*x1
      x2y4z4=x1y4z4*x1
      x2y4z5=x1y4z5*x1
      x2y4z6=x1y4z6*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3z4=x2z4*x1
      x3z5=x2z5*x1
      x3z6=x2z6*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x3y1z4=x2y1z4*x1
      x3y1z5=x2y1z5*x1
      x3y1z6=x2y1z6*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y2z3=x2y2z3*x1
      x3y2z4=x2y2z4*x1
      x3y2z5=x2y2z5*x1
      x3y2z6=x2y2z6*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x3y3z2=x2y3z2*x1
      x3y3z3=x2y3z3*x1
      x3y3z4=x2y3z4*x1
      x3y3z5=x2y3z5*x1
      x3y3z6=x2y3z6*x1
      x3y4=x2y4*x1
      x3y4z1=x2y4z1*x1
      x3y4z2=x2y4z2*x1
      x3y4z3=x2y4z3*x1
      x3y4z4=x2y4z4*x1
      x3y4z5=x2y4z5*x1
      x3y4z6=x2y4z6*x1
      x4=x3*x1
      x4z1=x3z1*x1
      x4z2=x3z2*x1
      x4z3=x3z3*x1
      x4z4=x3z4*x1
      x4z5=x3z5*x1
      x4z6=x3z6*x1
      x4y1=x3y1*x1
      x4y1z1=x3y1z1*x1
      x4y1z2=x3y1z2*x1
      x4y1z3=x3y1z3*x1
      x4y1z4=x3y1z4*x1
      x4y1z5=x3y1z5*x1
      x4y1z6=x3y1z6*x1
      x4y2=x3y2*x1
      x4y2z1=x3y2z1*x1
      x4y2z2=x3y2z2*x1
      x4y2z3=x3y2z3*x1
      x4y2z4=x3y2z4*x1
      x4y2z5=x3y2z5*x1
      x4y2z6=x3y2z6*x1
      x4y3=x3y3*x1
      x4y3z1=x3y3z1*x1
      x4y3z2=x3y3z2*x1
      x4y3z3=x3y3z3*x1
      x4y3z4=x3y3z4*x1
      x4y3z5=x3y3z5*x1
      x4y3z6=x3y3z6*x1
      x4y4=x3y4*x1
      x4y4z1=x3y4z1*x1
      x4y4z2=x3y4z2*x1
      x4y4z3=x3y4z3*x1
      x4y4z4=x3y4z4*x1
      x4y4z5=x3y4z5*x1
      x4y4z6=x3y4z6*x1
      r=(c(2,2,0,n)*4.+c(2,2,1,n)*z1*4.+c(2,2,2,n)*z2*4.+c(2,2,3,n)*z3*
     & 4.+c(2,2,4,n)*z4*4.+c(2,2,5,n)*z5*4.+c(2,2,6,n)*z6*4.+c(2,3,0,
     & n)*y1*12.+c(2,3,1,n)*y1z1*12.+c(2,3,2,n)*y1z2*12.+c(2,3,3,n)*
     & y1z3*12.+c(2,3,4,n)*y1z4*12.+c(2,3,5,n)*y1z5*12.+c(2,3,6,n)*
     & y1z6*12.+c(2,4,0,n)*y2*24.+c(2,4,1,n)*y2z1*24.+c(2,4,2,n)*y2z2*
     & 24.+c(2,4,3,n)*y2z3*24.+c(2,4,4,n)*y2z4*24.+c(2,4,5,n)*y2z5*
     & 24.+c(2,4,6,n)*y2z6*24.+c(2,5,0,n)*y3*40.+c(2,5,1,n)*y3z1*40.+
     & c(2,5,2,n)*y3z2*40.+c(2,5,3,n)*y3z3*40.+c(2,5,4,n)*y3z4*40.+c(
     & 2,5,5,n)*y3z5*40.+c(2,5,6,n)*y3z6*40.+c(2,6,0,n)*y4*60.+c(2,6,
     & 1,n)*y4z1*60.+c(2,6,2,n)*y4z2*60.+c(2,6,3,n)*y4z3*60.+c(2,6,4,
     & n)*y4z4*60.+c(2,6,5,n)*y4z5*60.+c(2,6,6,n)*y4z6*60.+c(3,2,0,n)*
     & x1*12.+c(3,2,1,n)*x1z1*12.+c(3,2,2,n)*x1z2*12.+c(3,2,3,n)*x1z3*
     & 12.+c(3,2,4,n)*x1z4*12.+c(3,2,5,n)*x1z5*12.+c(3,2,6,n)*x1z6*
     & 12.+c(3,3,0,n)*x1y1*36.+c(3,3,1,n)*x1y1z1*36.+c(3,3,2,n)*
     & x1y1z2*36.+c(3,3,3,n)*x1y1z3*36.+c(3,3,4,n)*x1y1z4*36.+c(3,3,5,
     & n)*x1y1z5*36.+c(3,3,6,n)*x1y1z6*36.+c(3,4,0,n)*x1y2*72.+c(3,4,
     & 1,n)*x1y2z1*72.+c(3,4,2,n)*x1y2z2*72.+c(3,4,3,n)*x1y2z3*72.+c(
     & 3,4,4,n)*x1y2z4*72.+c(3,4,5,n)*x1y2z5*72.+c(3,4,6,n)*x1y2z6*
     & 72.+c(3,5,0,n)*x1y3*120.+c(3,5,1,n)*x1y3z1*120.+c(3,5,2,n)*
     & x1y3z2*120.+c(3,5,3,n)*x1y3z3*120.+c(3,5,4,n)*x1y3z4*120.+c(3,
     & 5,5,n)*x1y3z5*120.+c(3,5,6,n)*x1y3z6*120.+c(3,6,0,n)*x1y4*180.+
     & c(3,6,1,n)*x1y4z1*180.+c(3,6,2,n)*x1y4z2*180.+c(3,6,3,n)*
     & x1y4z3*180.+c(3,6,4,n)*x1y4z4*180.+c(3,6,5,n)*x1y4z5*180.+c(3,
     & 6,6,n)*x1y4z6*180.+c(4,2,0,n)*x2*24.+c(4,2,1,n)*x2z1*24.+c(4,2,
     & 2,n)*x2z2*24.+c(4,2,3,n)*x2z3*24.+c(4,2,4,n)*x2z4*24.+c(4,2,5,
     & n)*x2z5*24.+c(4,2,6,n)*x2z6*24.+c(4,3,0,n)*x2y1*72.+c(4,3,1,n)*
     & x2y1z1*72.+c(4,3,2,n)*x2y1z2*72.+c(4,3,3,n)*x2y1z3*72.+c(4,3,4,
     & n)*x2y1z4*72.+c(4,3,5,n)*x2y1z5*72.+c(4,3,6,n)*x2y1z6*72.+c(4,
     & 4,0,n)*x2y2*144.+c(4,4,1,n)*x2y2z1*144.+c(4,4,2,n)*x2y2z2*144.+
     & c(4,4,3,n)*x2y2z3*144.+c(4,4,4,n)*x2y2z4*144.+c(4,4,5,n)*
     & x2y2z5*144.+c(4,4,6,n)*x2y2z6*144.+c(4,5,0,n)*x2y3*240.+c(4,5,
     & 1,n)*x2y3z1*240.+c(4,5,2,n)*x2y3z2*240.+c(4,5,3,n)*x2y3z3*240.+
     & c(4,5,4,n)*x2y3z4*240.+c(4,5,5,n)*x2y3z5*240.+c(4,5,6,n)*
     & x2y3z6*240.+c(4,6,0,n)*x2y4*360.+c(4,6,1,n)*x2y4z1*360.+c(4,6,
     & 2,n)*x2y4z2*360.+c(4,6,3,n)*x2y4z3*360.+c(4,6,4,n)*x2y4z4*360.+
     & c(4,6,5,n)*x2y4z5*360.+c(4,6,6,n)*x2y4z6*360.+c(5,2,0,n)*x3*
     & 40.+c(5,2,1,n)*x3z1*40.+c(5,2,2,n)*x3z2*40.+c(5,2,3,n)*x3z3*
     & 40.+c(5,2,4,n)*x3z4*40.+c(5,2,5,n)*x3z5*40.+c(5,2,6,n)*x3z6*
     & 40.+c(5,3,0,n)*x3y1*120.+c(5,3,1,n)*x3y1z1*120.+c(5,3,2,n)*
     & x3y1z2*120.+c(5,3,3,n)*x3y1z3*120.+c(5,3,4,n)*x3y1z4*120.+c(5,
     & 3,5,n)*x3y1z5*120.+c(5,3,6,n)*x3y1z6*120.+c(5,4,0,n)*x3y2*240.+
     & c(5,4,1,n)*x3y2z1*240.+c(5,4,2,n)*x3y2z2*240.+c(5,4,3,n)*
     & x3y2z3*240.+c(5,4,4,n)*x3y2z4*240.+c(5,4,5,n)*x3y2z5*240.+c(5,
     & 4,6,n)*x3y2z6*240.+c(5,5,0,n)*x3y3*400.+c(5,5,1,n)*x3y3z1*400.+
     & c(5,5,2,n)*x3y3z2*400.+c(5,5,3,n)*x3y3z3*400.+c(5,5,4,n)*
     & x3y3z4*400.+c(5,5,5,n)*x3y3z5*400.+c(5,5,6,n)*x3y3z6*400.+c(5,
     & 6,0,n)*x3y4*600.+c(5,6,1,n)*x3y4z1*600.+c(5,6,2,n)*x3y4z2*600.+
     & c(5,6,3,n)*x3y4z3*600.+c(5,6,4,n)*x3y4z4*600.+c(5,6,5,n)*
     & x3y4z5*600.+c(5,6,6,n)*x3y4z6*600.+c(6,2,0,n)*x4*60.+c(6,2,1,n)
     & *x4z1*60.+c(6,2,2,n)*x4z2*60.+c(6,2,3,n)*x4z3*60.+c(6,2,4,n)*
     & x4z4*60.+c(6,2,5,n)*x4z5*60.+c(6,2,6,n)*x4z6*60.+c(6,3,0,n)*
     & x4y1*180.+c(6,3,1,n)*x4y1z1*180.+c(6,3,2,n)*x4y1z2*180.+c(6,3,
     & 3,n)*x4y1z3*180.+c(6,3,4,n)*x4y1z4*180.+c(6,3,5,n)*x4y1z5*180.+
     & c(6,3,6,n)*x4y1z6*180.+c(6,4,0,n)*x4y2*360.+c(6,4,1,n)*x4y2z1*
     & 360.+c(6,4,2,n)*x4y2z2*360.+c(6,4,3,n)*x4y2z3*360.+c(6,4,4,n)*
     & x4y2z4*360.+c(6,4,5,n)*x4y2z5*360.+c(6,4,6,n)*x4y2z6*360.+c(6,
     & 5,0,n)*x4y3*600.+c(6,5,1,n)*x4y3z1*600.+c(6,5,2,n)*x4y3z2*600.+
     & c(6,5,3,n)*x4y3z3*600.+c(6,5,4,n)*x4y3z4*600.+c(6,5,5,n)*
     & x4y3z5*600.+c(6,5,6,n)*x4y3z6*600.+c(6,6,0,n)*x4y4*900.+c(6,6,
     & 1,n)*x4y4z1*900.+c(6,6,2,n)*x4y4z2*900.+c(6,6,3,n)*x4y4z3*900.+
     & c(6,6,4,n)*x4y4z4*900.+c(6,6,5,n)*x4y4z5*900.+c(6,6,6,n)*
     & x4y4z6*900.)*time
! endLoops()
      else if( dx.eq.2.and.dy.eq.2.and.dz.eq.1.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
c 
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
c 
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
      r=(c(2,2,1,n)*4.+c(2,2,2,n)*z1*8.+c(2,2,3,n)*z2*12.+c(2,2,4,n)*
     & z3*16.+c(2,2,5,n)*z4*20.+c(2,2,6,n)*z5*24.+c(2,3,1,n)*y1*12.+c(
     & 2,3,2,n)*y1z1*24.+c(2,3,3,n)*y1z2*36.+c(2,3,4,n)*y1z3*48.+c(2,
     & 3,5,n)*y1z4*60.+c(2,3,6,n)*y1z5*72.+c(2,4,1,n)*y2*24.+c(2,4,2,
     & n)*y2z1*48.+c(2,4,3,n)*y2z2*72.+c(2,4,4,n)*y2z3*96.+c(2,4,5,n)*
     & y2z4*120.+c(2,4,6,n)*y2z5*144.+c(2,5,1,n)*y3*40.+c(2,5,2,n)*
     & y3z1*80.+c(2,5,3,n)*y3z2*120.+c(2,5,4,n)*y3z3*160.+c(2,5,5,n)*
     & y3z4*200.+c(2,5,6,n)*y3z5*240.+c(2,6,1,n)*y4*60.+c(2,6,2,n)*
     & y4z1*120.+c(2,6,3,n)*y4z2*180.+c(2,6,4,n)*y4z3*240.+c(2,6,5,n)*
     & y4z4*300.+c(2,6,6,n)*y4z5*360.+c(3,2,1,n)*x1*12.+c(3,2,2,n)*
     & x1z1*24.+c(3,2,3,n)*x1z2*36.+c(3,2,4,n)*x1z3*48.+c(3,2,5,n)*
     & x1z4*60.+c(3,2,6,n)*x1z5*72.+c(3,3,1,n)*x1y1*36.+c(3,3,2,n)*
     & x1y1z1*72.+c(3,3,3,n)*x1y1z2*108.+c(3,3,4,n)*x1y1z3*144.+c(3,3,
     & 5,n)*x1y1z4*180.+c(3,3,6,n)*x1y1z5*216.+c(3,4,1,n)*x1y2*72.+c(
     & 3,4,2,n)*x1y2z1*144.+c(3,4,3,n)*x1y2z2*216.+c(3,4,4,n)*x1y2z3*
     & 288.+c(3,4,5,n)*x1y2z4*360.+c(3,4,6,n)*x1y2z5*432.+c(3,5,1,n)*
     & x1y3*120.+c(3,5,2,n)*x1y3z1*240.+c(3,5,3,n)*x1y3z2*360.+c(3,5,
     & 4,n)*x1y3z3*480.+c(3,5,5,n)*x1y3z4*600.+c(3,5,6,n)*x1y3z5*720.+
     & c(3,6,1,n)*x1y4*180.+c(3,6,2,n)*x1y4z1*360.+c(3,6,3,n)*x1y4z2*
     & 540.+c(3,6,4,n)*x1y4z3*720.+c(3,6,5,n)*x1y4z4*900.+c(3,6,6,n)*
     & x1y4z5*1080.+c(4,2,1,n)*x2*24.+c(4,2,2,n)*x2z1*48.+c(4,2,3,n)*
     & x2z2*72.+c(4,2,4,n)*x2z3*96.+c(4,2,5,n)*x2z4*120.+c(4,2,6,n)*
     & x2z5*144.+c(4,3,1,n)*x2y1*72.+c(4,3,2,n)*x2y1z1*144.+c(4,3,3,n)
     & *x2y1z2*216.+c(4,3,4,n)*x2y1z3*288.+c(4,3,5,n)*x2y1z4*360.+c(4,
     & 3,6,n)*x2y1z5*432.+c(4,4,1,n)*x2y2*144.+c(4,4,2,n)*x2y2z1*288.+
     & c(4,4,3,n)*x2y2z2*432.+c(4,4,4,n)*x2y2z3*576.+c(4,4,5,n)*
     & x2y2z4*720.+c(4,4,6,n)*x2y2z5*864.+c(4,5,1,n)*x2y3*240.+c(4,5,
     & 2,n)*x2y3z1*480.+c(4,5,3,n)*x2y3z2*720.+c(4,5,4,n)*x2y3z3*960.+
     & c(4,5,5,n)*x2y3z4*1200.+c(4,5,6,n)*x2y3z5*1440.+c(4,6,1,n)*
     & x2y4*360.+c(4,6,2,n)*x2y4z1*720.+c(4,6,3,n)*x2y4z2*1080.+c(4,6,
     & 4,n)*x2y4z3*1440.+c(4,6,5,n)*x2y4z4*1800.+c(4,6,6,n)*x2y4z5*
     & 2160.+c(5,2,1,n)*x3*40.+c(5,2,2,n)*x3z1*80.+c(5,2,3,n)*x3z2*
     & 120.+c(5,2,4,n)*x3z3*160.+c(5,2,5,n)*x3z4*200.+c(5,2,6,n)*x3z5*
     & 240.+c(5,3,1,n)*x3y1*120.+c(5,3,2,n)*x3y1z1*240.+c(5,3,3,n)*
     & x3y1z2*360.+c(5,3,4,n)*x3y1z3*480.+c(5,3,5,n)*x3y1z4*600.+c(5,
     & 3,6,n)*x3y1z5*720.+c(5,4,1,n)*x3y2*240.+c(5,4,2,n)*x3y2z1*480.+
     & c(5,4,3,n)*x3y2z2*720.+c(5,4,4,n)*x3y2z3*960.+c(5,4,5,n)*
     & x3y2z4*1200.+c(5,4,6,n)*x3y2z5*1440.+c(5,5,1,n)*x3y3*400.+c(5,
     & 5,2,n)*x3y3z1*800.+c(5,5,3,n)*x3y3z2*1200.+c(5,5,4,n)*x3y3z3*
     & 1600.+c(5,5,5,n)*x3y3z4*2000.+c(5,5,6,n)*x3y3z5*2400.+c(5,6,1,
     & n)*x3y4*600.+c(5,6,2,n)*x3y4z1*1200.+c(5,6,3,n)*x3y4z2*1800.+c(
     & 5,6,4,n)*x3y4z3*2400.+c(5,6,5,n)*x3y4z4*3000.+c(5,6,6,n)*
     & x3y4z5*3600.+c(6,2,1,n)*x4*60.+c(6,2,2,n)*x4z1*120.+c(6,2,3,n)*
     & x4z2*180.+c(6,2,4,n)*x4z3*240.+c(6,2,5,n)*x4z4*300.+c(6,2,6,n)*
     & x4z5*360.+c(6,3,1,n)*x4y1*180.+c(6,3,2,n)*x4y1z1*360.+c(6,3,3,
     & n)*x4y1z2*540.+c(6,3,4,n)*x4y1z3*720.+c(6,3,5,n)*x4y1z4*900.+c(
     & 6,3,6,n)*x4y1z5*1080.+c(6,4,1,n)*x4y2*360.+c(6,4,2,n)*x4y2z1*
     & 720.+c(6,4,3,n)*x4y2z2*1080.+c(6,4,4,n)*x4y2z3*1440.+c(6,4,5,n)
     & *x4y2z4*1800.+c(6,4,6,n)*x4y2z5*2160.+c(6,5,1,n)*x4y3*600.+c(6,
     & 5,2,n)*x4y3z1*1200.+c(6,5,3,n)*x4y3z2*1800.+c(6,5,4,n)*x4y3z3*
     & 2400.+c(6,5,5,n)*x4y3z4*3000.+c(6,5,6,n)*x4y3z5*3600.+c(6,6,1,
     & n)*x4y4*900.+c(6,6,2,n)*x4y4z1*1800.+c(6,6,3,n)*x4y4z2*2700.+c(
     & 6,6,4,n)*x4y4z3*3600.+c(6,6,5,n)*x4y4z4*4500.+c(6,6,6,n)*
     & x4y4z5*5400.)*time
! endLoops()
      else if( dx.eq.2.and.dy.eq.2.and.dz.eq.2.and.dt.eq.0  )then
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
c 
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
c 
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
      r=(c(2,2,2,n)*8.+c(2,2,3,n)*z1*24.+c(2,2,4,n)*z2*48.+c(2,2,5,n)*
     & z3*80.+c(2,2,6,n)*z4*120.+c(2,3,2,n)*y1*24.+c(2,3,3,n)*y1z1*
     & 72.+c(2,3,4,n)*y1z2*144.+c(2,3,5,n)*y1z3*240.+c(2,3,6,n)*y1z4*
     & 360.+c(2,4,2,n)*y2*48.+c(2,4,3,n)*y2z1*144.+c(2,4,4,n)*y2z2*
     & 288.+c(2,4,5,n)*y2z3*480.+c(2,4,6,n)*y2z4*720.+c(2,5,2,n)*y3*
     & 80.+c(2,5,3,n)*y3z1*240.+c(2,5,4,n)*y3z2*480.+c(2,5,5,n)*y3z3*
     & 800.+c(2,5,6,n)*y3z4*1200.+c(2,6,2,n)*y4*120.+c(2,6,3,n)*y4z1*
     & 360.+c(2,6,4,n)*y4z2*720.+c(2,6,5,n)*y4z3*1200.+c(2,6,6,n)*
     & y4z4*1800.+c(3,2,2,n)*x1*24.+c(3,2,3,n)*x1z1*72.+c(3,2,4,n)*
     & x1z2*144.+c(3,2,5,n)*x1z3*240.+c(3,2,6,n)*x1z4*360.+c(3,3,2,n)*
     & x1y1*72.+c(3,3,3,n)*x1y1z1*216.+c(3,3,4,n)*x1y1z2*432.+c(3,3,5,
     & n)*x1y1z3*720.+c(3,3,6,n)*x1y1z4*1080.+c(3,4,2,n)*x1y2*144.+c(
     & 3,4,3,n)*x1y2z1*432.+c(3,4,4,n)*x1y2z2*864.+c(3,4,5,n)*x1y2z3*
     & 1440.+c(3,4,6,n)*x1y2z4*2160.+c(3,5,2,n)*x1y3*240.+c(3,5,3,n)*
     & x1y3z1*720.+c(3,5,4,n)*x1y3z2*1440.+c(3,5,5,n)*x1y3z3*2400.+c(
     & 3,5,6,n)*x1y3z4*3600.+c(3,6,2,n)*x1y4*360.+c(3,6,3,n)*x1y4z1*
     & 1080.+c(3,6,4,n)*x1y4z2*2160.+c(3,6,5,n)*x1y4z3*3600.+c(3,6,6,
     & n)*x1y4z4*5400.+c(4,2,2,n)*x2*48.+c(4,2,3,n)*x2z1*144.+c(4,2,4,
     & n)*x2z2*288.+c(4,2,5,n)*x2z3*480.+c(4,2,6,n)*x2z4*720.+c(4,3,2,
     & n)*x2y1*144.+c(4,3,3,n)*x2y1z1*432.+c(4,3,4,n)*x2y1z2*864.+c(4,
     & 3,5,n)*x2y1z3*1440.+c(4,3,6,n)*x2y1z4*2160.+c(4,4,2,n)*x2y2*
     & 288.+c(4,4,3,n)*x2y2z1*864.+c(4,4,4,n)*x2y2z2*1728.+c(4,4,5,n)*
     & x2y2z3*2880.+c(4,4,6,n)*x2y2z4*4320.+c(4,5,2,n)*x2y3*480.+c(4,
     & 5,3,n)*x2y3z1*1440.+c(4,5,4,n)*x2y3z2*2880.+c(4,5,5,n)*x2y3z3*
     & 4800.+c(4,5,6,n)*x2y3z4*7200.+c(4,6,2,n)*x2y4*720.+c(4,6,3,n)*
     & x2y4z1*2160.+c(4,6,4,n)*x2y4z2*4320.+c(4,6,5,n)*x2y4z3*7200.+c(
     & 4,6,6,n)*x2y4z4*10800.+c(5,2,2,n)*x3*80.+c(5,2,3,n)*x3z1*240.+
     & c(5,2,4,n)*x3z2*480.+c(5,2,5,n)*x3z3*800.+c(5,2,6,n)*x3z4*
     & 1200.+c(5,3,2,n)*x3y1*240.+c(5,3,3,n)*x3y1z1*720.+c(5,3,4,n)*
     & x3y1z2*1440.+c(5,3,5,n)*x3y1z3*2400.+c(5,3,6,n)*x3y1z4*3600.+c(
     & 5,4,2,n)*x3y2*480.+c(5,4,3,n)*x3y2z1*1440.+c(5,4,4,n)*x3y2z2*
     & 2880.+c(5,4,5,n)*x3y2z3*4800.+c(5,4,6,n)*x3y2z4*7200.+c(5,5,2,
     & n)*x3y3*800.+c(5,5,3,n)*x3y3z1*2400.+c(5,5,4,n)*x3y3z2*4800.+c(
     & 5,5,5,n)*x3y3z3*8000.+c(5,5,6,n)*x3y3z4*12000.+c(5,6,2,n)*x3y4*
     & 1200.+c(5,6,3,n)*x3y4z1*3600.+c(5,6,4,n)*x3y4z2*7200.+c(5,6,5,
     & n)*x3y4z3*12000.+c(5,6,6,n)*x3y4z4*18000.+c(6,2,2,n)*x4*120.+c(
     & 6,2,3,n)*x4z1*360.+c(6,2,4,n)*x4z2*720.+c(6,2,5,n)*x4z3*1200.+
     & c(6,2,6,n)*x4z4*1800.+c(6,3,2,n)*x4y1*360.+c(6,3,3,n)*x4y1z1*
     & 1080.+c(6,3,4,n)*x4y1z2*2160.+c(6,3,5,n)*x4y1z3*3600.+c(6,3,6,
     & n)*x4y1z4*5400.+c(6,4,2,n)*x4y2*720.+c(6,4,3,n)*x4y2z1*2160.+c(
     & 6,4,4,n)*x4y2z2*4320.+c(6,4,5,n)*x4y2z3*7200.+c(6,4,6,n)*
     & x4y2z4*10800.+c(6,5,2,n)*x4y3*1200.+c(6,5,3,n)*x4y3z1*3600.+c(
     & 6,5,4,n)*x4y3z2*7200.+c(6,5,5,n)*x4y3z3*12000.+c(6,5,6,n)*
     & x4y3z4*18000.+c(6,6,2,n)*x4y4*1800.+c(6,6,3,n)*x4y4z1*5400.+c(
     & 6,6,4,n)*x4y4z2*10800.+c(6,6,5,n)*x4y4z3*18000.+c(6,6,6,n)*
     & x4y4z4*27000.)*time
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
      z6=z5*z1
c 
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y1z5=y1z4*z1
      y1z6=y1z5*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
      y2z5=y1z5*y1
      y2z6=y1z6*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      y3z4=y2z4*y1
      y3z5=y2z5*y1
      y3z6=y2z6*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y4z2=y3z2*y1
      y4z3=y3z3*y1
      y4z4=y3z4*y1
      y4z5=y3z5*y1
      y4z6=y3z6*y1
      y5=y4*y1
      y5z1=y4z1*y1
      y5z2=y4z2*y1
      y5z3=y4z3*y1
      y5z4=y4z4*y1
      y5z5=y4z5*y1
      y5z6=y4z6*y1
      y6=y5*y1
      y6z1=y5z1*y1
      y6z2=y5z2*y1
      y6z3=y5z3*y1
      y6z4=y5z4*y1
      y6z5=y5z5*y1
      y6z6=y5z6*y1
c 
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1z5=x1z4*z1
      x1z6=x1z5*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y1z5=x1y1z4*z1
      x1y1z6=x1y1z5*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y2z4=x1y1z4*y1
      x1y2z5=x1y1z5*y1
      x1y2z6=x1y1z6*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x1y3z4=x1y2z4*y1
      x1y3z5=x1y2z5*y1
      x1y3z6=x1y2z6*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y4z2=x1y3z2*y1
      x1y4z3=x1y3z3*y1
      x1y4z4=x1y3z4*y1
      x1y4z5=x1y3z5*y1
      x1y4z6=x1y3z6*y1
      x1y5=x1y4*y1
      x1y5z1=x1y4z1*y1
      x1y5z2=x1y4z2*y1
      x1y5z3=x1y4z3*y1
      x1y5z4=x1y4z4*y1
      x1y5z5=x1y4z5*y1
      x1y5z6=x1y4z6*y1
      x1y6=x1y5*y1
      x1y6z1=x1y5z1*y1
      x1y6z2=x1y5z2*y1
      x1y6z3=x1y5z3*y1
      x1y6z4=x1y5z4*y1
      x1y6z5=x1y5z5*y1
      x1y6z6=x1y5z6*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x2z5=x1z5*x1
      x2z6=x1z6*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y1z4=x1y1z4*x1
      x2y1z5=x1y1z5*x1
      x2y1z6=x1y1z6*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y2z4=x1y2z4*x1
      x2y2z5=x1y2z5*x1
      x2y2z6=x1y2z6*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y3z3=x1y3z3*x1
      x2y3z4=x1y3z4*x1
      x2y3z5=x1y3z5*x1
      x2y3z6=x1y3z6*x1
      x2y4=x1y4*x1
      x2y4z1=x1y4z1*x1
      x2y4z2=x1y4z2*x1
      x2y4z3=x1y4z3*x1
      x2y4z4=x1y4z4*x1
      x2y4z5=x1y4z5*x1
      x2y4z6=x1y4z6*x1
      x2y5=x1y5*x1
      x2y5z1=x1y5z1*x1
      x2y5z2=x1y5z2*x1
      x2y5z3=x1y5z3*x1
      x2y5z4=x1y5z4*x1
      x2y5z5=x1y5z5*x1
      x2y5z6=x1y5z6*x1
      x2y6=x1y6*x1
      x2y6z1=x1y6z1*x1
      x2y6z2=x1y6z2*x1
      x2y6z3=x1y6z3*x1
      x2y6z4=x1y6z4*x1
      x2y6z5=x1y6z5*x1
      x2y6z6=x1y6z6*x1
      x3=x2*x1
      x3z1=x2z1*x1
      x3z2=x2z2*x1
      x3z3=x2z3*x1
      x3z4=x2z4*x1
      x3z5=x2z5*x1
      x3z6=x2z6*x1
      x3y1=x2y1*x1
      x3y1z1=x2y1z1*x1
      x3y1z2=x2y1z2*x1
      x3y1z3=x2y1z3*x1
      x3y1z4=x2y1z4*x1
      x3y1z5=x2y1z5*x1
      x3y1z6=x2y1z6*x1
      x3y2=x2y2*x1
      x3y2z1=x2y2z1*x1
      x3y2z2=x2y2z2*x1
      x3y2z3=x2y2z3*x1
      x3y2z4=x2y2z4*x1
      x3y2z5=x2y2z5*x1
      x3y2z6=x2y2z6*x1
      x3y3=x2y3*x1
      x3y3z1=x2y3z1*x1
      x3y3z2=x2y3z2*x1
      x3y3z3=x2y3z3*x1
      x3y3z4=x2y3z4*x1
      x3y3z5=x2y3z5*x1
      x3y3z6=x2y3z6*x1
      x3y4=x2y4*x1
      x3y4z1=x2y4z1*x1
      x3y4z2=x2y4z2*x1
      x3y4z3=x2y4z3*x1
      x3y4z4=x2y4z4*x1
      x3y4z5=x2y4z5*x1
      x3y4z6=x2y4z6*x1
      x3y5=x2y5*x1
      x3y5z1=x2y5z1*x1
      x3y5z2=x2y5z2*x1
      x3y5z3=x2y5z3*x1
      x3y5z4=x2y5z4*x1
      x3y5z5=x2y5z5*x1
      x3y5z6=x2y5z6*x1
      x3y6=x2y6*x1
      x3y6z1=x2y6z1*x1
      x3y6z2=x2y6z2*x1
      x3y6z3=x2y6z3*x1
      x3y6z4=x2y6z4*x1
      x3y6z5=x2y6z5*x1
      x3y6z6=x2y6z6*x1
      r=(c(3,0,0,n)*6.+c(3,0,1,n)*z1*6.+c(3,0,2,n)*z2*6.+c(3,0,3,n)*z3*
     & 6.+c(3,0,4,n)*z4*6.+c(3,0,5,n)*z5*6.+c(3,0,6,n)*z6*6.+c(3,1,0,
     & n)*y1*6.+c(3,1,1,n)*y1z1*6.+c(3,1,2,n)*y1z2*6.+c(3,1,3,n)*y1z3*
     & 6.+c(3,1,4,n)*y1z4*6.+c(3,1,5,n)*y1z5*6.+c(3,1,6,n)*y1z6*6.+c(
     & 3,2,0,n)*y2*6.+c(3,2,1,n)*y2z1*6.+c(3,2,2,n)*y2z2*6.+c(3,2,3,n)
     & *y2z3*6.+c(3,2,4,n)*y2z4*6.+c(3,2,5,n)*y2z5*6.+c(3,2,6,n)*y2z6*
     & 6.+c(3,3,0,n)*y3*6.+c(3,3,1,n)*y3z1*6.+c(3,3,2,n)*y3z2*6.+c(3,
     & 3,3,n)*y3z3*6.+c(3,3,4,n)*y3z4*6.+c(3,3,5,n)*y3z5*6.+c(3,3,6,n)
     & *y3z6*6.+c(3,4,0,n)*y4*6.+c(3,4,1,n)*y4z1*6.+c(3,4,2,n)*y4z2*
     & 6.+c(3,4,3,n)*y4z3*6.+c(3,4,4,n)*y4z4*6.+c(3,4,5,n)*y4z5*6.+c(
     & 3,4,6,n)*y4z6*6.+c(3,5,0,n)*y5*6.+c(3,5,1,n)*y5z1*6.+c(3,5,2,n)
     & *y5z2*6.+c(3,5,3,n)*y5z3*6.+c(3,5,4,n)*y5z4*6.+c(3,5,5,n)*y5z5*
     & 6.+c(3,5,6,n)*y5z6*6.+c(3,6,0,n)*y6*6.+c(3,6,1,n)*y6z1*6.+c(3,
     & 6,2,n)*y6z2*6.+c(3,6,3,n)*y6z3*6.+c(3,6,4,n)*y6z4*6.+c(3,6,5,n)
     & *y6z5*6.+c(3,6,6,n)*y6z6*6.+c(4,0,0,n)*x1*24.+c(4,0,1,n)*x1z1*
     & 24.+c(4,0,2,n)*x1z2*24.+c(4,0,3,n)*x1z3*24.+c(4,0,4,n)*x1z4*
     & 24.+c(4,0,5,n)*x1z5*24.+c(4,0,6,n)*x1z6*24.+c(4,1,0,n)*x1y1*
     & 24.+c(4,1,1,n)*x1y1z1*24.+c(4,1,2,n)*x1y1z2*24.+c(4,1,3,n)*
     & x1y1z3*24.+c(4,1,4,n)*x1y1z4*24.+c(4,1,5,n)*x1y1z5*24.+c(4,1,6,
     & n)*x1y1z6*24.+c(4,2,0,n)*x1y2*24.+c(4,2,1,n)*x1y2z1*24.+c(4,2,
     & 2,n)*x1y2z2*24.+c(4,2,3,n)*x1y2z3*24.+c(4,2,4,n)*x1y2z4*24.+c(
     & 4,2,5,n)*x1y2z5*24.+c(4,2,6,n)*x1y2z6*24.+c(4,3,0,n)*x1y3*24.+
     & c(4,3,1,n)*x1y3z1*24.+c(4,3,2,n)*x1y3z2*24.+c(4,3,3,n)*x1y3z3*
     & 24.+c(4,3,4,n)*x1y3z4*24.+c(4,3,5,n)*x1y3z5*24.+c(4,3,6,n)*
     & x1y3z6*24.+c(4,4,0,n)*x1y4*24.+c(4,4,1,n)*x1y4z1*24.+c(4,4,2,n)
     & *x1y4z2*24.+c(4,4,3,n)*x1y4z3*24.+c(4,4,4,n)*x1y4z4*24.+c(4,4,
     & 5,n)*x1y4z5*24.+c(4,4,6,n)*x1y4z6*24.+c(4,5,0,n)*x1y5*24.+c(4,
     & 5,1,n)*x1y5z1*24.+c(4,5,2,n)*x1y5z2*24.+c(4,5,3,n)*x1y5z3*24.+
     & c(4,5,4,n)*x1y5z4*24.+c(4,5,5,n)*x1y5z5*24.+c(4,5,6,n)*x1y5z6*
     & 24.+c(4,6,0,n)*x1y6*24.+c(4,6,1,n)*x1y6z1*24.+c(4,6,2,n)*
     & x1y6z2*24.+c(4,6,3,n)*x1y6z3*24.+c(4,6,4,n)*x1y6z4*24.+c(4,6,5,
     & n)*x1y6z5*24.+c(4,6,6,n)*x1y6z6*24.+c(5,0,0,n)*x2*60.+c(5,0,1,
     & n)*x2z1*60.+c(5,0,2,n)*x2z2*60.+c(5,0,3,n)*x2z3*60.+c(5,0,4,n)*
     & x2z4*60.+c(5,0,5,n)*x2z5*60.+c(5,0,6,n)*x2z6*60.+c(5,1,0,n)*
     & x2y1*60.+c(5,1,1,n)*x2y1z1*60.+c(5,1,2,n)*x2y1z2*60.+c(5,1,3,n)
     & *x2y1z3*60.+c(5,1,4,n)*x2y1z4*60.+c(5,1,5,n)*x2y1z5*60.+c(5,1,
     & 6,n)*x2y1z6*60.+c(5,2,0,n)*x2y2*60.+c(5,2,1,n)*x2y2z1*60.+c(5,
     & 2,2,n)*x2y2z2*60.+c(5,2,3,n)*x2y2z3*60.+c(5,2,4,n)*x2y2z4*60.+
     & c(5,2,5,n)*x2y2z5*60.+c(5,2,6,n)*x2y2z6*60.+c(5,3,0,n)*x2y3*
     & 60.+c(5,3,1,n)*x2y3z1*60.+c(5,3,2,n)*x2y3z2*60.+c(5,3,3,n)*
     & x2y3z3*60.+c(5,3,4,n)*x2y3z4*60.+c(5,3,5,n)*x2y3z5*60.+c(5,3,6,
     & n)*x2y3z6*60.+c(5,4,0,n)*x2y4*60.+c(5,4,1,n)*x2y4z1*60.+c(5,4,
     & 2,n)*x2y4z2*60.+c(5,4,3,n)*x2y4z3*60.+c(5,4,4,n)*x2y4z4*60.+c(
     & 5,4,5,n)*x2y4z5*60.+c(5,4,6,n)*x2y4z6*60.+c(5,5,0,n)*x2y5*60.+
     & c(5,5,1,n)*x2y5z1*60.+c(5,5,2,n)*x2y5z2*60.+c(5,5,3,n)*x2y5z3*
     & 60.+c(5,5,4,n)*x2y5z4*60.+c(5,5,5,n)*x2y5z5*60.+c(5,5,6,n)*
     & x2y5z6*60.+c(5,6,0,n)*x2y6*60.+c(5,6,1,n)*x2y6z1*60.+c(5,6,2,n)
     & *x2y6z2*60.+c(5,6,3,n)*x2y6z3*60.+c(5,6,4,n)*x2y6z4*60.+c(5,6,
     & 5,n)*x2y6z5*60.+c(5,6,6,n)*x2y6z6*60.+c(6,0,0,n)*x3*120.+c(6,0,
     & 1,n)*x3z1*120.+c(6,0,2,n)*x3z2*120.+c(6,0,3,n)*x3z3*120.+c(6,0,
     & 4,n)*x3z4*120.+c(6,0,5,n)*x3z5*120.+c(6,0,6,n)*x3z6*120.+c(6,1,
     & 0,n)*x3y1*120.+c(6,1,1,n)*x3y1z1*120.+c(6,1,2,n)*x3y1z2*120.+c(
     & 6,1,3,n)*x3y1z3*120.+c(6,1,4,n)*x3y1z4*120.+c(6,1,5,n)*x3y1z5*
     & 120.+c(6,1,6,n)*x3y1z6*120.+c(6,2,0,n)*x3y2*120.+c(6,2,1,n)*
     & x3y2z1*120.+c(6,2,2,n)*x3y2z2*120.+c(6,2,3,n)*x3y2z3*120.+c(6,
     & 2,4,n)*x3y2z4*120.+c(6,2,5,n)*x3y2z5*120.+c(6,2,6,n)*x3y2z6*
     & 120.+c(6,3,0,n)*x3y3*120.+c(6,3,1,n)*x3y3z1*120.+c(6,3,2,n)*
     & x3y3z2*120.+c(6,3,3,n)*x3y3z3*120.+c(6,3,4,n)*x3y3z4*120.+c(6,
     & 3,5,n)*x3y3z5*120.+c(6,3,6,n)*x3y3z6*120.+c(6,4,0,n)*x3y4*120.+
     & c(6,4,1,n)*x3y4z1*120.+c(6,4,2,n)*x3y4z2*120.+c(6,4,3,n)*
     & x3y4z3*120.+c(6,4,4,n)*x3y4z4*120.+c(6,4,5,n)*x3y4z5*120.+c(6,
     & 4,6,n)*x3y4z6*120.+c(6,5,0,n)*x3y5*120.+c(6,5,1,n)*x3y5z1*120.+
     & c(6,5,2,n)*x3y5z2*120.+c(6,5,3,n)*x3y5z3*120.+c(6,5,4,n)*
     & x3y5z4*120.+c(6,5,5,n)*x3y5z5*120.+c(6,5,6,n)*x3y5z6*120.+c(6,
     & 6,0,n)*x3y6*120.+c(6,6,1,n)*x3y6z1*120.+c(6,6,2,n)*x3y6z2*120.+
     & c(6,6,3,n)*x3y6z3*120.+c(6,6,4,n)*x3y6z4*120.+c(6,6,5,n)*
     & x3y6z5*120.+c(6,6,6,n)*x3y6z6*120.)*time
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
      z5=z4*z1
      z6=z5*z1
c 
      y1z1=y1*z1
      y1z2=y1z1*z1
      y1z3=y1z2*z1
      y1z4=y1z3*z1
      y1z5=y1z4*z1
      y1z6=y1z5*z1
      y2=y1*y1
      y2z1=y1z1*y1
      y2z2=y1z2*y1
      y2z3=y1z3*y1
      y2z4=y1z4*y1
      y2z5=y1z5*y1
      y2z6=y1z6*y1
      y3=y2*y1
      y3z1=y2z1*y1
      y3z2=y2z2*y1
      y3z3=y2z3*y1
      y3z4=y2z4*y1
      y3z5=y2z5*y1
      y3z6=y2z6*y1
      y4=y3*y1
      y4z1=y3z1*y1
      y4z2=y3z2*y1
      y4z3=y3z3*y1
      y4z4=y3z4*y1
      y4z5=y3z5*y1
      y4z6=y3z6*y1
      y5=y4*y1
      y5z1=y4z1*y1
      y5z2=y4z2*y1
      y5z3=y4z3*y1
      y5z4=y4z4*y1
      y5z5=y4z5*y1
      y5z6=y4z6*y1
      y6=y5*y1
      y6z1=y5z1*y1
      y6z2=y5z2*y1
      y6z3=y5z3*y1
      y6z4=y5z4*y1
      y6z5=y5z5*y1
      y6z6=y5z6*y1
c 
      x1z1=x1*z1
      x1z2=x1z1*z1
      x1z3=x1z2*z1
      x1z4=x1z3*z1
      x1z5=x1z4*z1
      x1z6=x1z5*z1
      x1y1=x1*y1
      x1y1z1=x1*y1*z1
      x1y1z2=x1y1z1*z1
      x1y1z3=x1y1z2*z1
      x1y1z4=x1y1z3*z1
      x1y1z5=x1y1z4*z1
      x1y1z6=x1y1z5*z1
      x1y2=x1y1*y1
      x1y2z1=x1y1z1*y1
      x1y2z2=x1y1z2*y1
      x1y2z3=x1y1z3*y1
      x1y2z4=x1y1z4*y1
      x1y2z5=x1y1z5*y1
      x1y2z6=x1y1z6*y1
      x1y3=x1y2*y1
      x1y3z1=x1y2z1*y1
      x1y3z2=x1y2z2*y1
      x1y3z3=x1y2z3*y1
      x1y3z4=x1y2z4*y1
      x1y3z5=x1y2z5*y1
      x1y3z6=x1y2z6*y1
      x1y4=x1y3*y1
      x1y4z1=x1y3z1*y1
      x1y4z2=x1y3z2*y1
      x1y4z3=x1y3z3*y1
      x1y4z4=x1y3z4*y1
      x1y4z5=x1y3z5*y1
      x1y4z6=x1y3z6*y1
      x1y5=x1y4*y1
      x1y5z1=x1y4z1*y1
      x1y5z2=x1y4z2*y1
      x1y5z3=x1y4z3*y1
      x1y5z4=x1y4z4*y1
      x1y5z5=x1y4z5*y1
      x1y5z6=x1y4z6*y1
      x1y6=x1y5*y1
      x1y6z1=x1y5z1*y1
      x1y6z2=x1y5z2*y1
      x1y6z3=x1y5z3*y1
      x1y6z4=x1y5z4*y1
      x1y6z5=x1y5z5*y1
      x1y6z6=x1y5z6*y1
      x2=x1*x1
      x2z1=x1z1*x1
      x2z2=x1z2*x1
      x2z3=x1z3*x1
      x2z4=x1z4*x1
      x2z5=x1z5*x1
      x2z6=x1z6*x1
      x2y1=x1y1*x1
      x2y1z1=x1y1z1*x1
      x2y1z2=x1y1z2*x1
      x2y1z3=x1y1z3*x1
      x2y1z4=x1y1z4*x1
      x2y1z5=x1y1z5*x1
      x2y1z6=x1y1z6*x1
      x2y2=x1y2*x1
      x2y2z1=x1y2z1*x1
      x2y2z2=x1y2z2*x1
      x2y2z3=x1y2z3*x1
      x2y2z4=x1y2z4*x1
      x2y2z5=x1y2z5*x1
      x2y2z6=x1y2z6*x1
      x2y3=x1y3*x1
      x2y3z1=x1y3z1*x1
      x2y3z2=x1y3z2*x1
      x2y3z3=x1y3z3*x1
      x2y3z4=x1y3z4*x1
      x2y3z5=x1y3z5*x1
      x2y3z6=x1y3z6*x1
      x2y4=x1y4*x1
      x2y4z1=x1y4z1*x1
      x2y4z2=x1y4z2*x1
      x2y4z3=x1y4z3*x1
      x2y4z4=x1y4z4*x1
      x2y4z5=x1y4z5*x1
      x2y4z6=x1y4z6*x1
      x2y5=x1y5*x1
      x2y5z1=x1y5z1*x1
      x2y5z2=x1y5z2*x1
      x2y5z3=x1y5z3*x1
      x2y5z4=x1y5z4*x1
      x2y5z5=x1y5z5*x1
      x2y5z6=x1y5z6*x1
      x2y6=x1y6*x1
      x2y6z1=x1y6z1*x1
      x2y6z2=x1y6z2*x1
      x2y6z3=x1y6z3*x1
      x2y6z4=x1y6z4*x1
      x2y6z5=x1y6z5*x1
      x2y6z6=x1y6z6*x1
      r=(c(4,0,0,n)*24.+c(4,0,1,n)*z1*24.+c(4,0,2,n)*z2*24.+c(4,0,3,n)*
     & z3*24.+c(4,0,4,n)*z4*24.+c(4,0,5,n)*z5*24.+c(4,0,6,n)*z6*24.+c(
     & 4,1,0,n)*y1*24.+c(4,1,1,n)*y1z1*24.+c(4,1,2,n)*y1z2*24.+c(4,1,
     & 3,n)*y1z3*24.+c(4,1,4,n)*y1z4*24.+c(4,1,5,n)*y1z5*24.+c(4,1,6,
     & n)*y1z6*24.+c(4,2,0,n)*y2*24.+c(4,2,1,n)*y2z1*24.+c(4,2,2,n)*
     & y2z2*24.+c(4,2,3,n)*y2z3*24.+c(4,2,4,n)*y2z4*24.+c(4,2,5,n)*
     & y2z5*24.+c(4,2,6,n)*y2z6*24.+c(4,3,0,n)*y3*24.+c(4,3,1,n)*y3z1*
     & 24.+c(4,3,2,n)*y3z2*24.+c(4,3,3,n)*y3z3*24.+c(4,3,4,n)*y3z4*
     & 24.+c(4,3,5,n)*y3z5*24.+c(4,3,6,n)*y3z6*24.+c(4,4,0,n)*y4*24.+
     & c(4,4,1,n)*y4z1*24.+c(4,4,2,n)*y4z2*24.+c(4,4,3,n)*y4z3*24.+c(
     & 4,4,4,n)*y4z4*24.+c(4,4,5,n)*y4z5*24.+c(4,4,6,n)*y4z6*24.+c(4,
     & 5,0,n)*y5*24.+c(4,5,1,n)*y5z1*24.+c(4,5,2,n)*y5z2*24.+c(4,5,3,
     & n)*y5z3*24.+c(4,5,4,n)*y5z4*24.+c(4,5,5,n)*y5z5*24.+c(4,5,6,n)*
     & y5z6*24.+c(4,6,0,n)*y6*24.+c(4,6,1,n)*y6z1*24.+c(4,6,2,n)*y6z2*
     & 24.+c(4,6,3,n)*y6z3*24.+c(4,6,4,n)*y6z4*24.+c(4,6,5,n)*y6z5*
     & 24.+c(4,6,6,n)*y6z6*24.+c(5,0,0,n)*x1*120.+c(5,0,1,n)*x1z1*
     & 120.+c(5,0,2,n)*x1z2*120.+c(5,0,3,n)*x1z3*120.+c(5,0,4,n)*x1z4*
     & 120.+c(5,0,5,n)*x1z5*120.+c(5,0,6,n)*x1z6*120.+c(5,1,0,n)*x1y1*
     & 120.+c(5,1,1,n)*x1y1z1*120.+c(5,1,2,n)*x1y1z2*120.+c(5,1,3,n)*
     & x1y1z3*120.+c(5,1,4,n)*x1y1z4*120.+c(5,1,5,n)*x1y1z5*120.+c(5,
     & 1,6,n)*x1y1z6*120.+c(5,2,0,n)*x1y2*120.+c(5,2,1,n)*x1y2z1*120.+
     & c(5,2,2,n)*x1y2z2*120.+c(5,2,3,n)*x1y2z3*120.+c(5,2,4,n)*
     & x1y2z4*120.+c(5,2,5,n)*x1y2z5*120.+c(5,2,6,n)*x1y2z6*120.+c(5,
     & 3,0,n)*x1y3*120.+c(5,3,1,n)*x1y3z1*120.+c(5,3,2,n)*x1y3z2*120.+
     & c(5,3,3,n)*x1y3z3*120.+c(5,3,4,n)*x1y3z4*120.+c(5,3,5,n)*
     & x1y3z5*120.+c(5,3,6,n)*x1y3z6*120.+c(5,4,0,n)*x1y4*120.+c(5,4,
     & 1,n)*x1y4z1*120.+c(5,4,2,n)*x1y4z2*120.+c(5,4,3,n)*x1y4z3*120.+
     & c(5,4,4,n)*x1y4z4*120.+c(5,4,5,n)*x1y4z5*120.+c(5,4,6,n)*
     & x1y4z6*120.+c(5,5,0,n)*x1y5*120.+c(5,5,1,n)*x1y5z1*120.+c(5,5,
     & 2,n)*x1y5z2*120.+c(5,5,3,n)*x1y5z3*120.+c(5,5,4,n)*x1y5z4*120.+
     & c(5,5,5,n)*x1y5z5*120.+c(5,5,6,n)*x1y5z6*120.+c(5,6,0,n)*x1y6*
     & 120.+c(5,6,1,n)*x1y6z1*120.+c(5,6,2,n)*x1y6z2*120.+c(5,6,3,n)*
     & x1y6z3*120.+c(5,6,4,n)*x1y6z4*120.+c(5,6,5,n)*x1y6z5*120.+c(5,
     & 6,6,n)*x1y6z6*120.+c(6,0,0,n)*x2*360.+c(6,0,1,n)*x2z1*360.+c(6,
     & 0,2,n)*x2z2*360.+c(6,0,3,n)*x2z3*360.+c(6,0,4,n)*x2z4*360.+c(6,
     & 0,5,n)*x2z5*360.+c(6,0,6,n)*x2z6*360.+c(6,1,0,n)*x2y1*360.+c(6,
     & 1,1,n)*x2y1z1*360.+c(6,1,2,n)*x2y1z2*360.+c(6,1,3,n)*x2y1z3*
     & 360.+c(6,1,4,n)*x2y1z4*360.+c(6,1,5,n)*x2y1z5*360.+c(6,1,6,n)*
     & x2y1z6*360.+c(6,2,0,n)*x2y2*360.+c(6,2,1,n)*x2y2z1*360.+c(6,2,
     & 2,n)*x2y2z2*360.+c(6,2,3,n)*x2y2z3*360.+c(6,2,4,n)*x2y2z4*360.+
     & c(6,2,5,n)*x2y2z5*360.+c(6,2,6,n)*x2y2z6*360.+c(6,3,0,n)*x2y3*
     & 360.+c(6,3,1,n)*x2y3z1*360.+c(6,3,2,n)*x2y3z2*360.+c(6,3,3,n)*
     & x2y3z3*360.+c(6,3,4,n)*x2y3z4*360.+c(6,3,5,n)*x2y3z5*360.+c(6,
     & 3,6,n)*x2y3z6*360.+c(6,4,0,n)*x2y4*360.+c(6,4,1,n)*x2y4z1*360.+
     & c(6,4,2,n)*x2y4z2*360.+c(6,4,3,n)*x2y4z3*360.+c(6,4,4,n)*
     & x2y4z4*360.+c(6,4,5,n)*x2y4z5*360.+c(6,4,6,n)*x2y4z6*360.+c(6,
     & 5,0,n)*x2y5*360.+c(6,5,1,n)*x2y5z1*360.+c(6,5,2,n)*x2y5z2*360.+
     & c(6,5,3,n)*x2y5z3*360.+c(6,5,4,n)*x2y5z4*360.+c(6,5,5,n)*
     & x2y5z5*360.+c(6,5,6,n)*x2y5z6*360.+c(6,6,0,n)*x2y6*360.+c(6,6,
     & 1,n)*x2y6z1*360.+c(6,6,2,n)*x2y6z2*360.+c(6,6,3,n)*x2y6z3*360.+
     & c(6,6,4,n)*x2y6z4*360.+c(6,6,5,n)*x2y6z5*360.+c(6,6,6,n)*
     & x2y6z6*360.)*time
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
      z2=z1*z1
      z3=z2*z1
      z4=z3*z1
c
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
      y6=y5*y1
      y6z1=y5z1*y1
      y6z2=y5z2*y1
      y6z3=y5z3*y1
      y6z4=y5z4*y1
c
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
      x1y6=x1y5*y1
      x1y6z1=x1y5z1*y1
      x1y6z2=x1y5z2*y1
      x1y6z3=x1y5z3*y1
      x1y6z4=x1y5z4*y1
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
      x2y6=x1y6*x1
      x2y6z1=x1y6z1*x1
      x2y6z2=x1y6z2*x1
      x2y6z3=x1y6z3*x1
      x2y6z4=x1y6z4*x1
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
      x3y6=x2y6*x1
      x3y6z1=x2y6z1*x1
      x3y6z2=x2y6z2*x1
      x3y6z3=x2y6z3*x1
      x3y6z4=x2y6z4*x1
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
      x4y6=x3y6*x1
      x4y6z1=x3y6z1*x1
      x4y6z2=x3y6z2*x1
      x4y6z3=x3y6z3*x1
      x4y6z4=x3y6z4*x1
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
      x5y6=x4y6*x1
      x5y6z1=x4y6z1*x1
      x5y6z2=x4y6z2*x1
      x5y6z3=x4y6z3*x1
      x5y6z4=x4y6z4*x1
      x6=x5*x1
      x6z1=x5z1*x1
      x6z2=x5z2*x1
      x6z3=x5z3*x1
      x6z4=x5z4*x1
      x6y1=x5y1*x1
      x6y1z1=x5y1z1*x1
      x6y1z2=x5y1z2*x1
      x6y1z3=x5y1z3*x1
      x6y1z4=x5y1z4*x1
      x6y2=x5y2*x1
      x6y2z1=x5y2z1*x1
      x6y2z2=x5y2z2*x1
      x6y2z3=x5y2z3*x1
      x6y2z4=x5y2z4*x1
      x6y3=x5y3*x1
      x6y3z1=x5y3z1*x1
      x6y3z2=x5y3z2*x1
      x6y3z3=x5y3z3*x1
      x6y3z4=x5y3z4*x1
      x6y4=x5y4*x1
      x6y4z1=x5y4z1*x1
      x6y4z2=x5y4z2*x1
      x6y4z3=x5y4z3*x1
      x6y4z4=x5y4z4*x1
      x6y5=x5y5*x1
      x6y5z1=x5y5z1*x1
      x6y5z2=x5y5z2*x1
      x6y5z3=x5y5z3*x1
      x6y5z4=x5y5z4*x1
      x6y6=x5y6*x1
      x6y6z1=x5y6z1*x1
      x6y6z2=x5y6z2*x1
      x6y6z3=x5y6z3*x1
      x6y6z4=x5y6z4*x1
      z5=z4*z1
      z6=z5*z1
      y1z5=y1z4*z1
      y1z6=y1z5*z1
      y2z5=y1z5*y1
      y2z6=y1z6*y1
      y3z5=y2z5*y1
      y3z6=y2z6*y1
      y4z5=y3z5*y1
      y4z6=y3z6*y1
      x1z5=x1z4*z1
      x1z6=x1z5*z1
      x1y1z5=x1y1z4*z1
      x1y1z6=x1y1z5*z1
      x1y2z5=x1y1z5*y1
      x1y2z6=x1y1z6*y1
      x1y3z5=x1y2z5*y1
      x1y3z6=x1y2z6*y1
      x1y4z5=x1y3z5*y1
      x1y4z6=x1y3z6*y1
      x2z5=x1z5*x1
      x2z6=x1z6*x1
      x2y1z5=x1y1z5*x1
      x2y1z6=x1y1z6*x1
      x2y2z5=x1y2z5*x1
      x2y2z6=x1y2z6*x1
      x2y3z5=x1y3z5*x1
      x2y3z6=x1y3z6*x1
      x2y4z5=x1y4z5*x1
      x2y4z6=x1y4z6*x1
      x3z5=x2z5*x1
      x3z6=x2z6*x1
      x3y1z5=x2y1z5*x1
      x3y1z6=x2y1z6*x1
      x3y2z5=x2y2z5*x1
      x3y2z6=x2y2z6*x1
      x3y3z5=x2y3z5*x1
      x3y3z6=x2y3z6*x1
      x3y4z5=x2y4z5*x1
      x3y4z6=x2y4z6*x1
      x4z5=x3z5*x1
      x4z6=x3z6*x1
      x4y1z5=x3y1z5*x1
      x4y1z6=x3y1z6*x1
      x4y2z5=x3y2z5*x1
      x4y2z6=x3y2z6*x1
      x4y3z5=x3y3z5*x1
      x4y3z6=x3y3z6*x1
      x4y4z5=x3y4z5*x1
      x4y4z6=x3y4z6*x1
      x5z5=x4z5*x1
      x5z6=x4z6*x1
      x5y1z5=x4y1z5*x1
      x5y1z6=x4y1z6*x1
      x5y2z5=x4y2z5*x1
      x5y2z6=x4y2z6*x1
      x5y3z5=x4y3z5*x1
      x5y3z6=x4y3z6*x1
      x5y4z5=x4y4z5*x1
      x5y4z6=x4y4z6*x1
      x6z5=x5z5*x1
      x6z6=x5z6*x1
      x6y1z5=x5y1z5*x1
      x6y1z6=x5y1z6*x1
      x6y2z5=x5y2z5*x1
      x6y2z6=x5y2z6*x1
      x6y3z5=x5y3z5*x1
      x6y3z6=x5y3z6*x1
      x6y4z5=x5y4z5*x1
      x6y4z6=x5y4z6*x1
      y5z5=y4z5*y1
      y5z6=y4z6*y1
      y6z5=y5z5*y1
      y6z6=y5z6*y1
      x1y5z5=x1y4z5*y1
      x1y5z6=x1y4z6*y1
      x1y6z5=x1y5z5*y1
      x1y6z6=x1y5z6*y1
      x2y5z5=x1y5z5*x1
      x2y5z6=x1y5z6*x1
      x2y6z5=x1y6z5*x1
      x2y6z6=x1y6z6*x1
      x3y5z5=x2y5z5*x1
      x3y5z6=x2y5z6*x1
      x3y6z5=x2y6z5*x1
      x3y6z6=x2y6z6*x1
      x4y5z5=x3y5z5*x1
      x4y5z6=x3y5z6*x1
      x4y6z5=x3y6z5*x1
      x4y6z6=x3y6z6*x1
      r=(+c(0,0,2,n)*2.+c(0,0,3,n)*z1*6.+c(0,0,4,n)*z2*12.+c(0,0,5,n)*
     & z3*20.+c(0,0,6,n)*z4*30.+c(0,1,2,n)*y1*2.+c(0,1,3,n)*y1z1*6.+c(
     & 0,1,4,n)*y1z2*12.+c(0,1,5,n)*y1z3*20.+c(0,1,6,n)*y1z4*30.+c(0,
     & 2,2,n)*y2*2.+c(0,2,3,n)*y2z1*6.+c(0,2,4,n)*y2z2*12.+c(0,2,5,n)*
     & y2z3*20.+c(0,2,6,n)*y2z4*30.+c(0,3,2,n)*y3*2.+c(0,3,3,n)*y3z1*
     & 6.+c(0,3,4,n)*y3z2*12.+c(0,3,5,n)*y3z3*20.+c(0,3,6,n)*y3z4*30.+
     & c(0,4,2,n)*y4*2.+c(0,4,3,n)*y4z1*6.+c(0,4,4,n)*y4z2*12.+c(0,4,
     & 5,n)*y4z3*20.+c(0,4,6,n)*y4z4*30.+c(0,5,2,n)*y5*2.+c(0,5,3,n)*
     & y5z1*6.+c(0,5,4,n)*y5z2*12.+c(0,5,5,n)*y5z3*20.+c(0,5,6,n)*
     & y5z4*30.+c(0,6,2,n)*y6*2.+c(0,6,3,n)*y6z1*6.+c(0,6,4,n)*y6z2*
     & 12.+c(0,6,5,n)*y6z3*20.+c(0,6,6,n)*y6z4*30.+c(1,0,2,n)*x1*2.+c(
     & 1,0,3,n)*x1z1*6.+c(1,0,4,n)*x1z2*12.+c(1,0,5,n)*x1z3*20.+c(1,0,
     & 6,n)*x1z4*30.+c(1,1,2,n)*x1y1*2.+c(1,1,3,n)*x1y1z1*6.+c(1,1,4,
     & n)*x1y1z2*12.+c(1,1,5,n)*x1y1z3*20.+c(1,1,6,n)*x1y1z4*30.+c(1,
     & 2,2,n)*x1y2*2.+c(1,2,3,n)*x1y2z1*6.+c(1,2,4,n)*x1y2z2*12.+c(1,
     & 2,5,n)*x1y2z3*20.+c(1,2,6,n)*x1y2z4*30.+c(1,3,2,n)*x1y3*2.+c(1,
     & 3,3,n)*x1y3z1*6.+c(1,3,4,n)*x1y3z2*12.+c(1,3,5,n)*x1y3z3*20.+c(
     & 1,3,6,n)*x1y3z4*30.+c(1,4,2,n)*x1y4*2.+c(1,4,3,n)*x1y4z1*6.+c(
     & 1,4,4,n)*x1y4z2*12.+c(1,4,5,n)*x1y4z3*20.+c(1,4,6,n)*x1y4z4*
     & 30.+c(1,5,2,n)*x1y5*2.+c(1,5,3,n)*x1y5z1*6.+c(1,5,4,n)*x1y5z2*
     & 12.+c(1,5,5,n)*x1y5z3*20.+c(1,5,6,n)*x1y5z4*30.+c(1,6,2,n)*
     & x1y6*2.+c(1,6,3,n)*x1y6z1*6.+c(1,6,4,n)*x1y6z2*12.+c(1,6,5,n)*
     & x1y6z3*20.+c(1,6,6,n)*x1y6z4*30.+c(2,0,2,n)*x2*2.+c(2,0,3,n)*
     & x2z1*6.+c(2,0,4,n)*x2z2*12.+c(2,0,5,n)*x2z3*20.+c(2,0,6,n)*
     & x2z4*30.+c(2,1,2,n)*x2y1*2.+c(2,1,3,n)*x2y1z1*6.+c(2,1,4,n)*
     & x2y1z2*12.+c(2,1,5,n)*x2y1z3*20.+c(2,1,6,n)*x2y1z4*30.+c(2,2,2,
     & n)*x2y2*2.+c(2,2,3,n)*x2y2z1*6.+c(2,2,4,n)*x2y2z2*12.+c(2,2,5,
     & n)*x2y2z3*20.+c(2,2,6,n)*x2y2z4*30.+c(2,3,2,n)*x2y3*2.+c(2,3,3,
     & n)*x2y3z1*6.+c(2,3,4,n)*x2y3z2*12.+c(2,3,5,n)*x2y3z3*20.+c(2,3,
     & 6,n)*x2y3z4*30.+c(2,4,2,n)*x2y4*2.+c(2,4,3,n)*x2y4z1*6.+c(2,4,
     & 4,n)*x2y4z2*12.+c(2,4,5,n)*x2y4z3*20.+c(2,4,6,n)*x2y4z4*30.+c(
     & 2,5,2,n)*x2y5*2.+c(2,5,3,n)*x2y5z1*6.+c(2,5,4,n)*x2y5z2*12.+c(
     & 2,5,5,n)*x2y5z3*20.+c(2,5,6,n)*x2y5z4*30.+c(2,6,2,n)*x2y6*2.+c(
     & 2,6,3,n)*x2y6z1*6.+c(2,6,4,n)*x2y6z2*12.+c(2,6,5,n)*x2y6z3*20.+
     & c(2,6,6,n)*x2y6z4*30.+c(3,0,2,n)*x3*2.+c(3,0,3,n)*x3z1*6.+c(3,
     & 0,4,n)*x3z2*12.+c(3,0,5,n)*x3z3*20.+c(3,0,6,n)*x3z4*30.+c(3,1,
     & 2,n)*x3y1*2.+c(3,1,3,n)*x3y1z1*6.+c(3,1,4,n)*x3y1z2*12.+c(3,1,
     & 5,n)*x3y1z3*20.+c(3,1,6,n)*x3y1z4*30.+c(3,2,2,n)*x3y2*2.+c(3,2,
     & 3,n)*x3y2z1*6.+c(3,2,4,n)*x3y2z2*12.+c(3,2,5,n)*x3y2z3*20.+c(3,
     & 2,6,n)*x3y2z4*30.+c(3,3,2,n)*x3y3*2.+c(3,3,3,n)*x3y3z1*6.+c(3,
     & 3,4,n)*x3y3z2*12.+c(3,3,5,n)*x3y3z3*20.+c(3,3,6,n)*x3y3z4*30.+
     & c(3,4,2,n)*x3y4*2.+c(3,4,3,n)*x3y4z1*6.+c(3,4,4,n)*x3y4z2*12.+
     & c(3,4,5,n)*x3y4z3*20.+c(3,4,6,n)*x3y4z4*30.+c(3,5,2,n)*x3y5*2.+
     & c(3,5,3,n)*x3y5z1*6.+c(3,5,4,n)*x3y5z2*12.+c(3,5,5,n)*x3y5z3*
     & 20.+c(3,5,6,n)*x3y5z4*30.+c(3,6,2,n)*x3y6*2.+c(3,6,3,n)*x3y6z1*
     & 6.+c(3,6,4,n)*x3y6z2*12.+c(3,6,5,n)*x3y6z3*20.+c(3,6,6,n)*
     & x3y6z4*30.+c(4,0,2,n)*x4*2.+c(4,0,3,n)*x4z1*6.+c(4,0,4,n)*x4z2*
     & 12.+c(4,0,5,n)*x4z3*20.+c(4,0,6,n)*x4z4*30.+c(4,1,2,n)*x4y1*2.+
     & c(4,1,3,n)*x4y1z1*6.+c(4,1,4,n)*x4y1z2*12.+c(4,1,5,n)*x4y1z3*
     & 20.+c(4,1,6,n)*x4y1z4*30.+c(4,2,2,n)*x4y2*2.+c(4,2,3,n)*x4y2z1*
     & 6.+c(4,2,4,n)*x4y2z2*12.+c(4,2,5,n)*x4y2z3*20.+c(4,2,6,n)*
     & x4y2z4*30.+c(4,3,2,n)*x4y3*2.+c(4,3,3,n)*x4y3z1*6.+c(4,3,4,n)*
     & x4y3z2*12.+c(4,3,5,n)*x4y3z3*20.+c(4,3,6,n)*x4y3z4*30.+c(4,4,2,
     & n)*x4y4*2.+c(4,4,3,n)*x4y4z1*6.+c(4,4,4,n)*x4y4z2*12.+c(4,4,5,
     & n)*x4y4z3*20.+c(4,4,6,n)*x4y4z4*30.+c(4,5,2,n)*x4y5*2.+c(4,5,3,
     & n)*x4y5z1*6.+c(4,5,4,n)*x4y5z2*12.+c(4,5,5,n)*x4y5z3*20.+c(4,5,
     & 6,n)*x4y5z4*30.+c(4,6,2,n)*x4y6*2.+c(4,6,3,n)*x4y6z1*6.+c(4,6,
     & 4,n)*x4y6z2*12.+c(4,6,5,n)*x4y6z3*20.+c(4,6,6,n)*x4y6z4*30.+c(
     & 5,0,2,n)*x5*2.+c(5,0,3,n)*x5z1*6.+c(5,0,4,n)*x5z2*12.+c(5,0,5,
     & n)*x5z3*20.+c(5,0,6,n)*x5z4*30.+c(5,1,2,n)*x5y1*2.+c(5,1,3,n)*
     & x5y1z1*6.+c(5,1,4,n)*x5y1z2*12.+c(5,1,5,n)*x5y1z3*20.+c(5,1,6,
     & n)*x5y1z4*30.+c(5,2,2,n)*x5y2*2.+c(5,2,3,n)*x5y2z1*6.+c(5,2,4,
     & n)*x5y2z2*12.+c(5,2,5,n)*x5y2z3*20.+c(5,2,6,n)*x5y2z4*30.+c(5,
     & 3,2,n)*x5y3*2.+c(5,3,3,n)*x5y3z1*6.+c(5,3,4,n)*x5y3z2*12.+c(5,
     & 3,5,n)*x5y3z3*20.+c(5,3,6,n)*x5y3z4*30.+c(5,4,2,n)*x5y4*2.+c(5,
     & 4,3,n)*x5y4z1*6.+c(5,4,4,n)*x5y4z2*12.+c(5,4,5,n)*x5y4z3*20.+c(
     & 5,4,6,n)*x5y4z4*30.+c(5,5,2,n)*x5y5*2.+c(5,5,3,n)*x5y5z1*6.+c(
     & 5,5,4,n)*x5y5z2*12.+c(5,5,5,n)*x5y5z3*20.+c(5,5,6,n)*x5y5z4*
     & 30.+c(5,6,2,n)*x5y6*2.+c(5,6,3,n)*x5y6z1*6.+c(5,6,4,n)*x5y6z2*
     & 12.+c(5,6,5,n)*x5y6z3*20.+c(5,6,6,n)*x5y6z4*30.+c(6,0,2,n)*x6*
     & 2.+c(6,0,3,n)*x6z1*6.+c(6,0,4,n)*x6z2*12.+c(6,0,5,n)*x6z3*20.+
     & c(6,0,6,n)*x6z4*30.+c(6,1,2,n)*x6y1*2.+c(6,1,3,n)*x6y1z1*6.+c(
     & 6,1,4,n)*x6y1z2*12.+c(6,1,5,n)*x6y1z3*20.+c(6,1,6,n)*x6y1z4*
     & 30.+c(6,2,2,n)*x6y2*2.+c(6,2,3,n)*x6y2z1*6.+c(6,2,4,n)*x6y2z2*
     & 12.+c(6,2,5,n)*x6y2z3*20.+c(6,2,6,n)*x6y2z4*30.+c(6,3,2,n)*
     & x6y3*2.+c(6,3,3,n)*x6y3z1*6.+c(6,3,4,n)*x6y3z2*12.+c(6,3,5,n)*
     & x6y3z3*20.+c(6,3,6,n)*x6y3z4*30.+c(6,4,2,n)*x6y4*2.+c(6,4,3,n)*
     & x6y4z1*6.+c(6,4,4,n)*x6y4z2*12.+c(6,4,5,n)*x6y4z3*20.)
       r=(r+c(6,4,6,n)*x6y4z4*30.+c(6,5,2,n)*x6y5*2.+c(6,5,3,n)*x6y5z1*
     & 6.+c(6,5,4,n)*x6y5z2*12.+c(6,5,5,n)*x6y5z3*20.+c(6,5,6,n)*
     & x6y5z4*30.+c(6,6,2,n)*x6y6*2.+c(6,6,3,n)*x6y6z1*6.+c(6,6,4,n)*
     & x6y6z2*12.+c(6,6,5,n)*x6y6z3*20.+c(6,6,6,n)*x6y6z4*30. )
       r=(r+c(0,2,0,n)*2.+c(0,2,1,n)*z1*2.+c(0,2,2,n)*z2*2.+c(0,2,3,n)*
     & z3*2.+c(0,2,4,n)*z4*2.+c(0,2,5,n)*z5*2.+c(0,2,6,n)*z6*2.+c(0,3,
     & 0,n)*y1*6.+c(0,3,1,n)*y1z1*6.+c(0,3,2,n)*y1z2*6.+c(0,3,3,n)*
     & y1z3*6.+c(0,3,4,n)*y1z4*6.+c(0,3,5,n)*y1z5*6.+c(0,3,6,n)*y1z6*
     & 6.+c(0,4,0,n)*y2*12.+c(0,4,1,n)*y2z1*12.+c(0,4,2,n)*y2z2*12.+c(
     & 0,4,3,n)*y2z3*12.+c(0,4,4,n)*y2z4*12.+c(0,4,5,n)*y2z5*12.+c(0,
     & 4,6,n)*y2z6*12.+c(0,5,0,n)*y3*20.+c(0,5,1,n)*y3z1*20.+c(0,5,2,
     & n)*y3z2*20.+c(0,5,3,n)*y3z3*20.+c(0,5,4,n)*y3z4*20.+c(0,5,5,n)*
     & y3z5*20.+c(0,5,6,n)*y3z6*20.+c(0,6,0,n)*y4*30.+c(0,6,1,n)*y4z1*
     & 30.+c(0,6,2,n)*y4z2*30.+c(0,6,3,n)*y4z3*30.+c(0,6,4,n)*y4z4*
     & 30.+c(0,6,5,n)*y4z5*30.+c(0,6,6,n)*y4z6*30.+c(1,2,0,n)*x1*2.+c(
     & 1,2,1,n)*x1z1*2.+c(1,2,2,n)*x1z2*2.+c(1,2,3,n)*x1z3*2.+c(1,2,4,
     & n)*x1z4*2.+c(1,2,5,n)*x1z5*2.+c(1,2,6,n)*x1z6*2.+c(1,3,0,n)*
     & x1y1*6.+c(1,3,1,n)*x1y1z1*6.+c(1,3,2,n)*x1y1z2*6.+c(1,3,3,n)*
     & x1y1z3*6.+c(1,3,4,n)*x1y1z4*6.+c(1,3,5,n)*x1y1z5*6.+c(1,3,6,n)*
     & x1y1z6*6.+c(1,4,0,n)*x1y2*12.+c(1,4,1,n)*x1y2z1*12.+c(1,4,2,n)*
     & x1y2z2*12.+c(1,4,3,n)*x1y2z3*12.+c(1,4,4,n)*x1y2z4*12.+c(1,4,5,
     & n)*x1y2z5*12.+c(1,4,6,n)*x1y2z6*12.+c(1,5,0,n)*x1y3*20.+c(1,5,
     & 1,n)*x1y3z1*20.+c(1,5,2,n)*x1y3z2*20.+c(1,5,3,n)*x1y3z3*20.+c(
     & 1,5,4,n)*x1y3z4*20.+c(1,5,5,n)*x1y3z5*20.+c(1,5,6,n)*x1y3z6*
     & 20.+c(1,6,0,n)*x1y4*30.+c(1,6,1,n)*x1y4z1*30.+c(1,6,2,n)*
     & x1y4z2*30.+c(1,6,3,n)*x1y4z3*30.+c(1,6,4,n)*x1y4z4*30.+c(1,6,5,
     & n)*x1y4z5*30.+c(1,6,6,n)*x1y4z6*30.+c(2,2,0,n)*x2*2.+c(2,2,1,n)
     & *x2z1*2.+c(2,2,2,n)*x2z2*2.+c(2,2,3,n)*x2z3*2.+c(2,2,4,n)*x2z4*
     & 2.+c(2,2,5,n)*x2z5*2.+c(2,2,6,n)*x2z6*2.+c(2,3,0,n)*x2y1*6.+c(
     & 2,3,1,n)*x2y1z1*6.+c(2,3,2,n)*x2y1z2*6.+c(2,3,3,n)*x2y1z3*6.+c(
     & 2,3,4,n)*x2y1z4*6.+c(2,3,5,n)*x2y1z5*6.+c(2,3,6,n)*x2y1z6*6.+c(
     & 2,4,0,n)*x2y2*12.+c(2,4,1,n)*x2y2z1*12.+c(2,4,2,n)*x2y2z2*12.+
     & c(2,4,3,n)*x2y2z3*12.+c(2,4,4,n)*x2y2z4*12.+c(2,4,5,n)*x2y2z5*
     & 12.+c(2,4,6,n)*x2y2z6*12.+c(2,5,0,n)*x2y3*20.+c(2,5,1,n)*
     & x2y3z1*20.+c(2,5,2,n)*x2y3z2*20.+c(2,5,3,n)*x2y3z3*20.+c(2,5,4,
     & n)*x2y3z4*20.+c(2,5,5,n)*x2y3z5*20.+c(2,5,6,n)*x2y3z6*20.+c(2,
     & 6,0,n)*x2y4*30.+c(2,6,1,n)*x2y4z1*30.+c(2,6,2,n)*x2y4z2*30.+c(
     & 2,6,3,n)*x2y4z3*30.+c(2,6,4,n)*x2y4z4*30.+c(2,6,5,n)*x2y4z5*
     & 30.+c(2,6,6,n)*x2y4z6*30.+c(3,2,0,n)*x3*2.+c(3,2,1,n)*x3z1*2.+
     & c(3,2,2,n)*x3z2*2.+c(3,2,3,n)*x3z3*2.+c(3,2,4,n)*x3z4*2.+c(3,2,
     & 5,n)*x3z5*2.+c(3,2,6,n)*x3z6*2.+c(3,3,0,n)*x3y1*6.+c(3,3,1,n)*
     & x3y1z1*6.+c(3,3,2,n)*x3y1z2*6.+c(3,3,3,n)*x3y1z3*6.+c(3,3,4,n)*
     & x3y1z4*6.+c(3,3,5,n)*x3y1z5*6.+c(3,3,6,n)*x3y1z6*6.+c(3,4,0,n)*
     & x3y2*12.+c(3,4,1,n)*x3y2z1*12.+c(3,4,2,n)*x3y2z2*12.+c(3,4,3,n)
     & *x3y2z3*12.+c(3,4,4,n)*x3y2z4*12.+c(3,4,5,n)*x3y2z5*12.+c(3,4,
     & 6,n)*x3y2z6*12.+c(3,5,0,n)*x3y3*20.+c(3,5,1,n)*x3y3z1*20.+c(3,
     & 5,2,n)*x3y3z2*20.+c(3,5,3,n)*x3y3z3*20.+c(3,5,4,n)*x3y3z4*20.+
     & c(3,5,5,n)*x3y3z5*20.+c(3,5,6,n)*x3y3z6*20.+c(3,6,0,n)*x3y4*
     & 30.+c(3,6,1,n)*x3y4z1*30.+c(3,6,2,n)*x3y4z2*30.+c(3,6,3,n)*
     & x3y4z3*30.+c(3,6,4,n)*x3y4z4*30.+c(3,6,5,n)*x3y4z5*30.+c(3,6,6,
     & n)*x3y4z6*30.+c(4,2,0,n)*x4*2.+c(4,2,1,n)*x4z1*2.+c(4,2,2,n)*
     & x4z2*2.+c(4,2,3,n)*x4z3*2.+c(4,2,4,n)*x4z4*2.+c(4,2,5,n)*x4z5*
     & 2.+c(4,2,6,n)*x4z6*2.+c(4,3,0,n)*x4y1*6.+c(4,3,1,n)*x4y1z1*6.+
     & c(4,3,2,n)*x4y1z2*6.+c(4,3,3,n)*x4y1z3*6.+c(4,3,4,n)*x4y1z4*6.+
     & c(4,3,5,n)*x4y1z5*6.+c(4,3,6,n)*x4y1z6*6.+c(4,4,0,n)*x4y2*12.+
     & c(4,4,1,n)*x4y2z1*12.+c(4,4,2,n)*x4y2z2*12.+c(4,4,3,n)*x4y2z3*
     & 12.+c(4,4,4,n)*x4y2z4*12.+c(4,4,5,n)*x4y2z5*12.+c(4,4,6,n)*
     & x4y2z6*12.+c(4,5,0,n)*x4y3*20.+c(4,5,1,n)*x4y3z1*20.+c(4,5,2,n)
     & *x4y3z2*20.+c(4,5,3,n)*x4y3z3*20.+c(4,5,4,n)*x4y3z4*20.+c(4,5,
     & 5,n)*x4y3z5*20.+c(4,5,6,n)*x4y3z6*20.+c(4,6,0,n)*x4y4*30.+c(4,
     & 6,1,n)*x4y4z1*30.+c(4,6,2,n)*x4y4z2*30.+c(4,6,3,n)*x4y4z3*30.+
     & c(4,6,4,n)*x4y4z4*30.+c(4,6,5,n)*x4y4z5*30.+c(4,6,6,n)*x4y4z6*
     & 30.+c(5,2,0,n)*x5*2.+c(5,2,1,n)*x5z1*2.+c(5,2,2,n)*x5z2*2.+c(5,
     & 2,3,n)*x5z3*2.+c(5,2,4,n)*x5z4*2.+c(5,2,5,n)*x5z5*2.+c(5,2,6,n)
     & *x5z6*2.+c(5,3,0,n)*x5y1*6.+c(5,3,1,n)*x5y1z1*6.+c(5,3,2,n)*
     & x5y1z2*6.+c(5,3,3,n)*x5y1z3*6.+c(5,3,4,n)*x5y1z4*6.+c(5,3,5,n)*
     & x5y1z5*6.+c(5,3,6,n)*x5y1z6*6.+c(5,4,0,n)*x5y2*12.+c(5,4,1,n)*
     & x5y2z1*12.+c(5,4,2,n)*x5y2z2*12.+c(5,4,3,n)*x5y2z3*12.+c(5,4,4,
     & n)*x5y2z4*12.+c(5,4,5,n)*x5y2z5*12.+c(5,4,6,n)*x5y2z6*12.+c(5,
     & 5,0,n)*x5y3*20.+c(5,5,1,n)*x5y3z1*20.+c(5,5,2,n)*x5y3z2*20.+c(
     & 5,5,3,n)*x5y3z3*20.+c(5,5,4,n)*x5y3z4*20.+c(5,5,5,n)*x5y3z5*
     & 20.+c(5,5,6,n)*x5y3z6*20.+c(5,6,0,n)*x5y4*30.+c(5,6,1,n)*
     & x5y4z1*30.+c(5,6,2,n)*x5y4z2*30.+c(5,6,3,n)*x5y4z3*30.+c(5,6,4,
     & n)*x5y4z4*30.+c(5,6,5,n)*x5y4z5*30.+c(5,6,6,n)*x5y4z6*30.+c(6,
     & 2,0,n)*x6*2.+c(6,2,1,n)*x6z1*2.+c(6,2,2,n)*x6z2*2.+c(6,2,3,n)*
     & x6z3*2.+c(6,2,4,n)*x6z4*2.+c(6,2,5,n)*x6z5*2.+c(6,2,6,n)*x6z6*
     & 2.+c(6,3,0,n)*x6y1*6.+c(6,3,1,n)*x6y1z1*6.+c(6,3,2,n)*x6y1z2*
     & 6.+c(6,3,3,n)*x6y1z3*6.+c(6,3,4,n)*x6y1z4*6.+c(6,3,5,n)*x6y1z5*
     & 6.+c(6,3,6,n)*x6y1z6*6.+c(6,4,0,n)*x6y2*12.+c(6,4,1,n)*x6y2z1*
     & 12.+c(6,4,2,n)*x6y2z2*12.+c(6,4,3,n)*x6y2z3*12.+c(6,4,4,n)*
     & x6y2z4*12.+c(6,4,5,n)*x6y2z5*12.+c(6,4,6,n)*x6y2z6*12.+c(6,5,0,
     & n)*x6y3*20.+c(6,5,1,n)*x6y3z1*20.+c(6,5,2,n)*x6y3z2*20.)
       r=(r+c(6,5,3,n)*x6y3z3*20.+c(6,5,4,n)*x6y3z4*20.+c(6,5,5,n)*
     & x6y3z5*20.+c(6,5,6,n)*x6y3z6*20.+c(6,6,0,n)*x6y4*30.+c(6,6,1,n)
     & *x6y4z1*30.+c(6,6,2,n)*x6y4z2*30.+c(6,6,3,n)*x6y4z3*30.+c(6,6,
     & 4,n)*x6y4z4*30.+c(6,6,5,n)*x6y4z5*30.+c(6,6,6,n)*x6y4z6*30. )
       r=(r+c(2,0,0,n)*2.+c(2,0,1,n)*z1*2.+c(2,0,2,n)*z2*2.+c(2,0,3,n)*
     & z3*2.+c(2,0,4,n)*z4*2.+c(2,0,5,n)*z5*2.+c(2,0,6,n)*z6*2.+c(2,1,
     & 0,n)*y1*2.+c(2,1,1,n)*y1z1*2.+c(2,1,2,n)*y1z2*2.+c(2,1,3,n)*
     & y1z3*2.+c(2,1,4,n)*y1z4*2.+c(2,1,5,n)*y1z5*2.+c(2,1,6,n)*y1z6*
     & 2.+c(2,2,0,n)*y2*2.+c(2,2,1,n)*y2z1*2.+c(2,2,2,n)*y2z2*2.+c(2,
     & 2,3,n)*y2z3*2.+c(2,2,4,n)*y2z4*2.+c(2,2,5,n)*y2z5*2.+c(2,2,6,n)
     & *y2z6*2.+c(2,3,0,n)*y3*2.+c(2,3,1,n)*y3z1*2.+c(2,3,2,n)*y3z2*
     & 2.+c(2,3,3,n)*y3z3*2.+c(2,3,4,n)*y3z4*2.+c(2,3,5,n)*y3z5*2.+c(
     & 2,3,6,n)*y3z6*2.+c(2,4,0,n)*y4*2.+c(2,4,1,n)*y4z1*2.+c(2,4,2,n)
     & *y4z2*2.+c(2,4,3,n)*y4z3*2.+c(2,4,4,n)*y4z4*2.+c(2,4,5,n)*y4z5*
     & 2.+c(2,4,6,n)*y4z6*2.+c(2,5,0,n)*y5*2.+c(2,5,1,n)*y5z1*2.+c(2,
     & 5,2,n)*y5z2*2.+c(2,5,3,n)*y5z3*2.+c(2,5,4,n)*y5z4*2.+c(2,5,5,n)
     & *y5z5*2.+c(2,5,6,n)*y5z6*2.+c(2,6,0,n)*y6*2.+c(2,6,1,n)*y6z1*
     & 2.+c(2,6,2,n)*y6z2*2.+c(2,6,3,n)*y6z3*2.+c(2,6,4,n)*y6z4*2.+c(
     & 2,6,5,n)*y6z5*2.+c(2,6,6,n)*y6z6*2.+c(3,0,0,n)*x1*6.+c(3,0,1,n)
     & *x1z1*6.+c(3,0,2,n)*x1z2*6.+c(3,0,3,n)*x1z3*6.+c(3,0,4,n)*x1z4*
     & 6.+c(3,0,5,n)*x1z5*6.+c(3,0,6,n)*x1z6*6.+c(3,1,0,n)*x1y1*6.+c(
     & 3,1,1,n)*x1y1z1*6.+c(3,1,2,n)*x1y1z2*6.+c(3,1,3,n)*x1y1z3*6.+c(
     & 3,1,4,n)*x1y1z4*6.+c(3,1,5,n)*x1y1z5*6.+c(3,1,6,n)*x1y1z6*6.+c(
     & 3,2,0,n)*x1y2*6.+c(3,2,1,n)*x1y2z1*6.+c(3,2,2,n)*x1y2z2*6.+c(3,
     & 2,3,n)*x1y2z3*6.+c(3,2,4,n)*x1y2z4*6.+c(3,2,5,n)*x1y2z5*6.+c(3,
     & 2,6,n)*x1y2z6*6.+c(3,3,0,n)*x1y3*6.+c(3,3,1,n)*x1y3z1*6.+c(3,3,
     & 2,n)*x1y3z2*6.+c(3,3,3,n)*x1y3z3*6.+c(3,3,4,n)*x1y3z4*6.+c(3,3,
     & 5,n)*x1y3z5*6.+c(3,3,6,n)*x1y3z6*6.+c(3,4,0,n)*x1y4*6.+c(3,4,1,
     & n)*x1y4z1*6.+c(3,4,2,n)*x1y4z2*6.+c(3,4,3,n)*x1y4z3*6.+c(3,4,4,
     & n)*x1y4z4*6.+c(3,4,5,n)*x1y4z5*6.+c(3,4,6,n)*x1y4z6*6.+c(3,5,0,
     & n)*x1y5*6.+c(3,5,1,n)*x1y5z1*6.+c(3,5,2,n)*x1y5z2*6.+c(3,5,3,n)
     & *x1y5z3*6.+c(3,5,4,n)*x1y5z4*6.+c(3,5,5,n)*x1y5z5*6.+c(3,5,6,n)
     & *x1y5z6*6.+c(3,6,0,n)*x1y6*6.+c(3,6,1,n)*x1y6z1*6.+c(3,6,2,n)*
     & x1y6z2*6.+c(3,6,3,n)*x1y6z3*6.+c(3,6,4,n)*x1y6z4*6.+c(3,6,5,n)*
     & x1y6z5*6.+c(3,6,6,n)*x1y6z6*6.+c(4,0,0,n)*x2*12.+c(4,0,1,n)*
     & x2z1*12.+c(4,0,2,n)*x2z2*12.+c(4,0,3,n)*x2z3*12.+c(4,0,4,n)*
     & x2z4*12.+c(4,0,5,n)*x2z5*12.+c(4,0,6,n)*x2z6*12.+c(4,1,0,n)*
     & x2y1*12.+c(4,1,1,n)*x2y1z1*12.+c(4,1,2,n)*x2y1z2*12.+c(4,1,3,n)
     & *x2y1z3*12.+c(4,1,4,n)*x2y1z4*12.+c(4,1,5,n)*x2y1z5*12.+c(4,1,
     & 6,n)*x2y1z6*12.+c(4,2,0,n)*x2y2*12.+c(4,2,1,n)*x2y2z1*12.+c(4,
     & 2,2,n)*x2y2z2*12.+c(4,2,3,n)*x2y2z3*12.+c(4,2,4,n)*x2y2z4*12.+
     & c(4,2,5,n)*x2y2z5*12.+c(4,2,6,n)*x2y2z6*12.+c(4,3,0,n)*x2y3*
     & 12.+c(4,3,1,n)*x2y3z1*12.+c(4,3,2,n)*x2y3z2*12.+c(4,3,3,n)*
     & x2y3z3*12.+c(4,3,4,n)*x2y3z4*12.+c(4,3,5,n)*x2y3z5*12.+c(4,3,6,
     & n)*x2y3z6*12.+c(4,4,0,n)*x2y4*12.+c(4,4,1,n)*x2y4z1*12.+c(4,4,
     & 2,n)*x2y4z2*12.+c(4,4,3,n)*x2y4z3*12.+c(4,4,4,n)*x2y4z4*12.+c(
     & 4,4,5,n)*x2y4z5*12.+c(4,4,6,n)*x2y4z6*12.+c(4,5,0,n)*x2y5*12.+
     & c(4,5,1,n)*x2y5z1*12.+c(4,5,2,n)*x2y5z2*12.+c(4,5,3,n)*x2y5z3*
     & 12.+c(4,5,4,n)*x2y5z4*12.+c(4,5,5,n)*x2y5z5*12.+c(4,5,6,n)*
     & x2y5z6*12.+c(4,6,0,n)*x2y6*12.+c(4,6,1,n)*x2y6z1*12.+c(4,6,2,n)
     & *x2y6z2*12.+c(4,6,3,n)*x2y6z3*12.+c(4,6,4,n)*x2y6z4*12.+c(4,6,
     & 5,n)*x2y6z5*12.+c(4,6,6,n)*x2y6z6*12.+c(5,0,0,n)*x3*20.+c(5,0,
     & 1,n)*x3z1*20.+c(5,0,2,n)*x3z2*20.+c(5,0,3,n)*x3z3*20.+c(5,0,4,
     & n)*x3z4*20.+c(5,0,5,n)*x3z5*20.+c(5,0,6,n)*x3z6*20.+c(5,1,0,n)*
     & x3y1*20.+c(5,1,1,n)*x3y1z1*20.+c(5,1,2,n)*x3y1z2*20.+c(5,1,3,n)
     & *x3y1z3*20.+c(5,1,4,n)*x3y1z4*20.+c(5,1,5,n)*x3y1z5*20.+c(5,1,
     & 6,n)*x3y1z6*20.+c(5,2,0,n)*x3y2*20.+c(5,2,1,n)*x3y2z1*20.+c(5,
     & 2,2,n)*x3y2z2*20.+c(5,2,3,n)*x3y2z3*20.+c(5,2,4,n)*x3y2z4*20.+
     & c(5,2,5,n)*x3y2z5*20.+c(5,2,6,n)*x3y2z6*20.+c(5,3,0,n)*x3y3*
     & 20.+c(5,3,1,n)*x3y3z1*20.+c(5,3,2,n)*x3y3z2*20.+c(5,3,3,n)*
     & x3y3z3*20.+c(5,3,4,n)*x3y3z4*20.+c(5,3,5,n)*x3y3z5*20.+c(5,3,6,
     & n)*x3y3z6*20.+c(5,4,0,n)*x3y4*20.+c(5,4,1,n)*x3y4z1*20.+c(5,4,
     & 2,n)*x3y4z2*20.+c(5,4,3,n)*x3y4z3*20.+c(5,4,4,n)*x3y4z4*20.+c(
     & 5,4,5,n)*x3y4z5*20.+c(5,4,6,n)*x3y4z6*20.+c(5,5,0,n)*x3y5*20.+
     & c(5,5,1,n)*x3y5z1*20.+c(5,5,2,n)*x3y5z2*20.+c(5,5,3,n)*x3y5z3*
     & 20.+c(5,5,4,n)*x3y5z4*20.+c(5,5,5,n)*x3y5z5*20.+c(5,5,6,n)*
     & x3y5z6*20.+c(5,6,0,n)*x3y6*20.+c(5,6,1,n)*x3y6z1*20.+c(5,6,2,n)
     & *x3y6z2*20.+c(5,6,3,n)*x3y6z3*20.+c(5,6,4,n)*x3y6z4*20.+c(5,6,
     & 5,n)*x3y6z5*20.+c(5,6,6,n)*x3y6z6*20.+c(6,0,0,n)*x4*30.+c(6,0,
     & 1,n)*x4z1*30.+c(6,0,2,n)*x4z2*30.+c(6,0,3,n)*x4z3*30.+c(6,0,4,
     & n)*x4z4*30.+c(6,0,5,n)*x4z5*30.+c(6,0,6,n)*x4z6*30.+c(6,1,0,n)*
     & x4y1*30.+c(6,1,1,n)*x4y1z1*30.+c(6,1,2,n)*x4y1z2*30.+c(6,1,3,n)
     & *x4y1z3*30.+c(6,1,4,n)*x4y1z4*30.+c(6,1,5,n)*x4y1z5*30.+c(6,1,
     & 6,n)*x4y1z6*30.+c(6,2,0,n)*x4y2*30.+c(6,2,1,n)*x4y2z1*30.+c(6,
     & 2,2,n)*x4y2z2*30.+c(6,2,3,n)*x4y2z3*30.+c(6,2,4,n)*x4y2z4*30.+
     & c(6,2,5,n)*x4y2z5*30.+c(6,2,6,n)*x4y2z6*30.+c(6,3,0,n)*x4y3*
     & 30.+c(6,3,1,n)*x4y3z1*30.+c(6,3,2,n)*x4y3z2*30.+c(6,3,3,n)*
     & x4y3z3*30.+c(6,3,4,n)*x4y3z4*30.+c(6,3,5,n)*x4y3z5*30.+c(6,3,6,
     & n)*x4y3z6*30.+c(6,4,0,n)*x4y4*30.+c(6,4,1,n)*x4y4z1*30.+c(6,4,
     & 2,n)*x4y4z2*30.+c(6,4,3,n)*x4y4z3*30.+c(6,4,4,n)*x4y4z4*30.+c(
     & 6,4,5,n)*x4y4z5*30.+c(6,4,6,n)*x4y4z6*30.+c(6,5,0,n)*x4y5*30.+
     & c(6,5,1,n)*x4y5z1*30.+c(6,5,2,n)*x4y5z2*30.)
       r=(r+c(6,5,3,n)*x4y5z3*30.+c(6,5,4,n)*x4y5z4*30.+c(6,5,5,n)*
     & x4y5z5*30.+c(6,5,6,n)*x4y5z6*30.+c(6,6,0,n)*x4y6*30.+c(6,6,1,n)
     & *x4y6z1*30.+c(6,6,2,n)*x4y6z2*30.+c(6,6,3,n)*x4y6z3*30.+c(6,6,
     & 4,n)*x4y6z4*30.+c(6,6,5,n)*x4y6z5*30.+c(6,6,6,n)*x4y6z6*30.)*
     & time
! endLoops()
      else
! beginLoops(time=0.)
      time=0.
      r=0.
! endLoops()
      end if
! #If "3D6" eq "3D5" || "3D6" eq "3D6"
#else
      write(*,'(''ERROR: poly3D6 not available for ifort'')')
#endif
      return
      end
