!
!  Evaluate the bessel function J_nu(z) with complex argument z=(zr,ri)
!  Return J = (jr,ji)
!
      subroutine cBesselJ( nu, zr,zi, jr,ji )

      double precision nu, zr,zi,jr,ji

      double precision cjr(10), cji(10)
      integer np,nz,ierr,kode

      kode=1 ! do not scale result by exp(-abs(zi))
      np=1 ! we just want J_nu 
      call zbesj( zr,zi,nu,kode,np,cjr,cji,nz,ierr)
      if( nz.ne.0 .or. ierr.ne.0 )then
        write(*,'("WARNING: zbesj: nz,ierr=",2i4)') nz,ierr
      end if

      jr=cjr(1)
      ji=cji(1)

      return 
      end 

!
!  Evaluate the bessel function Y_nu(z) with complex argument z=(zr,ri)
!  Return Y = (yr,yi)
!
      subroutine cBesselY( nu, zr,zi, yr,yi )

      double precision nu, zr,zi,yr,yi

      double precision cyr(10), cyi(10),cwkr(10),cwrki(10)
      integer np,nz,ierr,kode

      kode=1 ! do not scale result by exp(-abs(zi))
      np=1 ! we just want Y_nu 
      call zbesy( zr,zi,nu,kode,np,cyr,cyi,nz,
     &   cwkr,cwrki,ierr)
      if( nz.ne.0 .or. ierr.ne.0 )then
        write(*,'("WARNING: zbesy: nz,ierr=",2i4)') nz,ierr
      end if
      !write(*,'(" zbesy: nz,ierr=",2i4)') nz,ierr
      !write(*,'(" zbesy: Y=",2e12.4)') cyr(1),cyi(1)

      yr=cyr(1)
      yi=cyi(1)

      return 
      end 

!
!  Evaluate the real and imaginary parts of the solution to
!  the oscillating buble.
!    v_r    (r,theta,t) = \tilde{v}_r    (r) exp(i (n theta - omega t))
!    v_theta(r,theta,t) = \tilde{v}_theta(r) exp(i (n theta - omega t))
!    p      (r,theta,t) = \tilde{p}      (r) exp(i (n theta - omega t))
!  
!  Inputs:
!    r:   radius
!    Rb:   radius of boundary
!    n:   number of periods
!    mu:  viscosity
!    lr:  real part of lambda
!    li:  imag part of lambda
!    cr:  real part of integration constant
!    ci:  imag part of integration constant
!    p0r: real part of particular soln constant
!    p0i: imag part of particular soln constant
!  Outputs
!    vrr: real part of \tilde{v}_r
!    vri: imag part of \tilde{v}_r
!    vtr: real part of \tilde{v}_theta
!    vti: imag part of \tilde{v}_theta
!    pr:  real part of \tilde{p}
!    pi:  imag part of \tilde{p}
!

      subroutine evalOscillatingBubble(r,Rb, n, mu, lr,li, 
     +     cr,ci,dr,di,vrr,vri,vtr,vti,pr,pi)
c     todo, need to consider r = 0...
      implicit none
      double precision r,Rb,mu,
     +     lr,li,cr,ci,dr,di,
     +     vrr,vri,vtr,vti,pr,pi,
     +     jnr,jni,
     +     jnpr,jnpi,jnmr,jnmi,
     +     fnu,zr,zi
      integer n
      complex*16 vr,vt,p, ! final solution 
     +     l,c,d,nc,      ! complex versions of inputs
     +     rl,rc,Rbc,muc, ! 
     +     jn,jnp,jnm,    ! bessel functions
     +     f,I            ! dummy variable


c     get complex vars
      l = dcmplx(lr,li)
      c = dcmplx(cr,ci)
      d = dcmplx(dr,di)
      I = dcmplx(0.0d0,1.0d0)

      rc  = dcmplx(r ,0.0d0) ! complex r
      Rbc = dcmplx(Rb,0.0d0) ! complex Rb
      muc = dcmplx(mu,0.0d0) ! complex mu
      rl  = l*rc             ! r * lambda
      nc  = dcmplx(dble(n),0.0d0) ! complex n

      f = (d * nc) / (muc * (Rbc ** n) * (l ** 2))

c     check if r is close to 0 or not
      if (abs(r) > 1.0d-14) then
c       standard computations when r != 0

c       evaluate bessel funcs
        call cBesselJ( dble(n  ),dreal(rl),dimag(rl),jnr ,jni )
        call cBesselJ( dble(n+1),dreal(rl),dimag(rl),jnpr,jnpi)

        jn  = dcmplx(jnr ,jni )
        jnp = dcmplx(jnpr,jnpi)

c       evaluate vr, vt, and p
c       radial component of velocity
        vr = (c * jn / rc) + (f * (rc ** (n-1)))

c       circumferential component of velocity
        vt = (c * I * jn / rc)
     +       -(c * I * l * jnp / nc) 
     +       +(I * f * (rc ** (n-1)))


      else
c       r is close to 0, so we will evaluate Jn / r 
c       using the limit definition
        
c       evaluate bessel funcs
        call cBesselJ( dble(n-1),dreal(rl),dimag(rl),jnmr,jnmi)
        call cBesselJ( dble(n+1),dreal(rl),dimag(rl),jnpr,jnpi)
        
        jnm = dcmplx(jnmr,jnmi)
        jnp = dcmplx(jnpr,jnpi)

c       evaluate vr, vt, and p
c       radial component of velocity
        vr = (c * l * (jnm + jnp) / (dcmplx(2.0d0,0.0d0)))
     +       +(f * (rc ** (n-1)))

c       circumferential component of velocity
        vt = (c * I * l * (jnm + jnp) / (dcmplx(2.0d0,0.0d0)))
     +       -(c * I * l * jnp / nc) 
     +       +(I * f * (rc ** (n-1)))

c$$$        write(*,'("r=0, jnmr,jnmi=",e12.3,e12.3)') jnmr,jnmi
c$$$        write(*,'("r=0, jnpr,jnpi=",e12.3,e12.3)') jnpr,jnpi
c$$$        write(*,'("r=0, vr,vt=",e12.3,e12.3)') dreal(vr),dreal(vt)
      end if
      

c       pressure
        p = d * (rc ** n) / (Rbc ** n)

c       assign vr, vt, and p
        vrr = dreal(vr)
        vri = dimag(vr)
        vtr = dreal(vt)
        vti = dimag(vt)
        pr  = dreal( p)
        pi  = dimag( p)

      return
      end
