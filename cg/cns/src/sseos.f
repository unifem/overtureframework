c Functions to evaluate an EOS for a strong shock


      subroutine ssEosGetP( r,p,e, ipar,rpar )
c
c       Evaluate the strong-shock EOS for p=p(r,e), and derivative pe =dp/de
c  derivOption=ipar(3)
c           derivOption=1 : evaluate dp/dr (e=const)
c           derivOption=2 : evaluate dp/de (rho=const)
c           derivOption=3 : evaluate dp/dr and dp/de
c
      implicit none
      real r,p,e
      real rpar(*)
      integer ipar(*)

      real pnumer,pdenom,px,py
      real f(7),fx(7),x,y
      integer k,derivOption
      real rhoMin,pMin,eMin
      parameter( rhoMin=.01, eMin=.01, pMin=.01 )
      include 'sseos.h'

      if( r.lt.rhoMin )then
        write(*,'(" ssEosGetP:r=",e12.3," changed to ",e8.2)') r,rhoMin
        r=rhoMin
      end if
      if( e.lt.eMin )then
        write(*,'(" ssEosGetP:e=",e12.3," changed to ",e8.2)') e,eMin
        e=eMin
      end if

      x = r/rho0Poly-1.
      do k=1,7
       f(k) =  a(k,1) + x*(a(k,2) + x*( b0+ a(k,3) + x*(a(k,4)))) 
!       f(k) =  a(k,1) + a(k,2)*x + a(k,3)*x**2 + a(k,4)*x**3 + b0*x**2
      end do
  
      y = e/e0Poly
      pnumer = (f(1) + y*(f(2) + y*(f(3) + y*(f(4)))))*(1 + a0*x)
      pdenom= f(5) + y*(f(6) + y*(f(7)))
      p= pnumer/pdenom
!      p= (f(1) + f(2)*y + f(3)*y**2 + f(4)*y**3)*(1 + a0*x)/
!     &       (f(5) + f(6)*y + f(7)*y**2)

      if( p.lt.pMin )then
        write(*,'(" ssEosGetP:p=",e12.3," changed to ",e8.2)') p,pMin
        p=pMin
      end if


      derivOption=ipar(3)

      if( derivOption.lt.0 .or. derivOption.gt.3 )then
        write(*,'("ssEosGetP:ERROR: derivOption=",i6)') derivOption
        stop
      end if

      if( derivOption.eq.2 .or. derivOption.eq.3 )then
       ! evaluate dp/de

        py = (f(2) + y*(2.*f(3) + y*(3.*f(4))))*(1 + a0*x)/pdenom
     &     - pnumer*( f(6) + y*(2.*f(7)) )/pdenom**2

        rpar(2)=py/e0Poly    ! return here

      end if

      if( derivOption.eq.1 .or. derivOption.eq.3 )then
        ! evaluate dp/dr
        do k=1,7
         ! here is df/dx
         fx(k) =  a(k,2) + x*( 2.*(b0+ a(k,3)) + x*(3.*a(k,4)))
        end do
        px = ( (fx(1) + y*(fx(2) + y*(fx(3) + y*(fx(4)))))*(1 + a0*x) +
     &         (f(1) + y*(f(2) + y*(f(3) + y*(f(4)))))*a0 )/pdenom -
     &     pnumer*( fx(5) + y*(fx(6) + y*(fx(7))) )/pdenom**2

       rpar(1)=px/rho0Poly
       ! write(*,'("ssEosGetP: dp/dr=",f10.2)') rpar(1)
      end if

      return 
      end

      subroutine ssEosGetE( r,p,e, ipar,rpar )
c ======================================================================
c
c        Compute e from (r,p)
c        e (input/output) : on input an initial guess
c
c ======================================================================
      implicit none
      real r,p,e
      real rpar(*)
      integer ipar(*)

      real de,p0,pe,gamma
      integer it,maxit,derivOption
      real tol

      real rhoMin,pMin,eMin
      parameter( rhoMin=.01, eMin=.01, pMin=.01 )
      include 'sseos.h'


      ! First get an initial guess for Newton if none is given
      if( e.le.0 )then
        ! e=1.
        gamma=1.4
        e = p/(r*(gamma-1))   ! initial guess from ideal gas
      end if

      if( r.lt.rhoMin )then
        write(*,'(" invertEOS:r=",e12.3," changed to ",e8.2)') r,rhoMin
        r=rhoMin
      end if
      if( p.lt.pMin )then
        write(*,'(" invertEOS:p=",e12.3," changed to ",e8.2)') p,pMin
        p=pMin
      end if

      ! Use Newton's method to invert the EOS

      ! F(e) = 0 --> F(e) = P(r,e)-p
      ! F(e+de) = F(e) + F' de + ...
      !   F'(e)*de = -F(e)

      derivOption=ipar(3) ! save value 
      ipar(3)=2 ! eval dp/de 

      tol=1.e-8  ! if Newton converges quadratically the error should be tol**2 
      de=1.
      maxit=10
      do it=1,maxit

        ! call getEOS( r,e, p0,pe )
        call ssEosGetP( r,p0,e, ipar,rpar )
        pe=rpar(2)

        de = -(p0-p)/pe
c        write(*,'(" Newton: it,r,p,e=",i3,3f9.2," F,pe,de=",'//
c     &    'e9.1,e9.1,e10.1)') it,r,p,e,p0-p,pe,de
        e=e+de
        if( abs(de).lt.tol )then
          goto 100
        end if
      end do
      if( abs(de).gt.tol )then
        write(*,'(" invertEOS:ERROR: no convergence, it,de="i4,e10.3)')
     &   it,de
        write(*,'(" invertEOS:input: r,p,e=",3e12.3)') r,p,e
      end if

 100  continue
      ipar(3)=derivOption ! reset value

      if( e.lt.eMin )then
        write(*,'(" invertEOS:e=",e12.3," changed to ",e8.2)') e,eMin
        e=eMin
      end if


      if( derivOption.eq.0 )then
        ! do nothing
      else if( derivOption.eq.2 )then
       ! evaluate dp/de
       ! rpar(2)=pe    ! return here
       stop 7734
      else if( derivOption.eq.1 )then
        stop 7735
      else
        write(*,'("ssEosGetE:ERROR: derivOption=",i6)') derivOption
        stop
      end if



      return
      end


      subroutine sseosInit()
c ======================================================================
c       Initialize the strong-shock EOS
c ======================================================================

      implicit none

      include 'sseos.h'


      ! Here are values for Air

      a0=1.
      b0=0.
      e0Poly=2.5  ! what should this be ? 
      rho0Poly=1.

      a(1,1)=1.813710e-3
      a(1,2)=1.075440e-2
      a(1,3)=1.51680e-2
      a(1,4)=6.227670e-3

      a(2,1)=2.92189
      a(2,2)=6.529790
      a(2,3)=5.117580
      a(2,4)=1.509410
      
      a(3,1)=1.139630e1
      a(3,2)=8.787560
      a(3,3)=5.24548e-1
      a(3,4)=3.12411
      
      a(4,1)=8.89380
      a(4,2)=1.77792e1
      a(4,3)=9.12073
      a(4,4)=2.35273e-1
      
      a(5,1)=1.0
      a(5,2)=6.77579e-1
      a(5,3)=1.49785e-1
      a(5,4)=4.71705e-1
      
      a(6,1)=7.24225e1
      a(6,2)=6.19438e1
      a(6,3)=-9.79515
      a(6,4)=7.0704e-1
      
      a(7,1)=1.6783e1
      a(7,2)=3.33395e1
      a(7,3)=1.67022e1
      a(7,4)=1.45505e-1
 

      return
      end

