! This file automatically generated from evalDispersionRelation.bf with bpp.
! =====================================================================================================
! 
! -----------------------------------------------------------------------------------
! Evaluate the dispersion relation for the generalized dispersion model (GDM)
!  With multiple polarization vectors 
! -----------------------------------------------------------------------------------
!
!       E_tt - c^2 Delta(E) = -alphaP P_tt
!       P_tt + b1 P_1 + b0 = a0*E + a1*E_t 
!
!  Input:
!      mode : mode to choose, i.e. which root to choose. If mode=-1 then the default root is chosen.
!                The default root is the one with largest NEGATIVE imaginary part.
!      Np : number of polarization vectors  
!      c,k, 
!      a0(0:Np-1), a1(0:Np-1), b0(0:Np-1), b1(0:Np-1), alphaP : GDM parameters
! Output:
!      sr(0:nEig-1),si(0:nEig-1) : real and imaginary part of eigenvalues: nEig = 2*NpP+2 
!      srm, sim : eigenvalue with largest imaginary part 
!      psir(0:Np-1),psii(0:Np-1) : real and imaginary parts of psi : P = psi*E for s=(srm,sim) 
!
! =====================================================================================================
      subroutine evalEigGDM( mode, Np, c,k, a0,a1,b0,b1,alphaP, sr,si, 
     & srm,sim,psir,psii )

      implicit none

      integer mode, Np
      real c,k, srm,sim
      real a0(0:*),a1(0:*),b0(0:*),b1(0:*),alphaP, psir(0:*),psii(0:*)
      real sr(0:*),si(0:*)

      ! local variables 
      real ck,ck2, eps
      integer nd,lda, lwork, i, iMode, j
      ! lwork>= 3*lda : for good performance lwork must generally be larger
      parameter( lda=10, lwork=10*lda )
      real a(0:lda-1,0:lda-1), work(lwork), vr(1), vl(1)
      integer info

      complex*16 s, psi

      ck=c*k
      ck2=ck**2
      if( Np .eq. 1 )then
        ! Companion matrix for GDM model Np=1, File written by CG/DMX/matlab/gdm.maple
! Companion matrix for GDM model Np=1, File written by CG/DMX/matlab/gdm.maple
      a(0,0) = 0
      a(0,1) = 0
      a(0,2) = 0
      a(0,3) = -ck2*b0(0)
      a(1,0) = 1
      a(1,1) = 0
      a(1,2) = 0
      a(1,3) = -ck2*b1(0)
      a(2,0) = 0
      a(2,1) = 1
      a(2,2) = 0
      a(2,3) = -alphaP*a0(0)-b0(0)-ck2
      a(3,0) = 0
      a(3,1) = 0
      a(3,2) = 1
      a(3,3) = -alphaP*a1(0)-b1(0)
      else if( Np .eq. 2 )then
! Companion matrix for GDM model Np=2, File written by CG/DMX/matlab/gdm.maple
      a(0,0) = 0
      a(0,1) = 0
      a(0,2) = 0
      a(0,3) = 0
      a(0,4) = 0
      a(0,5) = -ck2*b0(0)*b0(1)
      a(1,0) = 1
      a(1,1) = 0
      a(1,2) = 0
      a(1,3) = 0
      a(1,4) = 0
      a(1,5) = -ck2*b1(0)*b0(1)-ck2*b0(0)*b1(1)
      a(2,0) = 0
      a(2,1) = 1
      a(2,2) = 0
      a(2,3) = 0
      a(2,4) = 0
      a(2,5) = -ck2*b0(0)-ck2*b1(0)*b1(1)-(ck2+b0(0))*b0(1)-alphaP*(a0(
     & 1)*b0(0)+a0(0)*b0(1))
      a(3,0) = 0
      a(3,1) = 0
      a(3,2) = 1
      a(3,3) = 0
      a(3,4) = 0
      a(3,5) = -ck2*b1(0)-(ck2+b0(0))*b1(1)-b1(0)*b0(1)-alphaP*(a0(1)*
     & b1(0)+a1(1)*b0(0)+a0(0)*b1(1)+a1(0)*b0(1))
      a(4,0) = 0
      a(4,1) = 0
      a(4,2) = 0
      a(4,3) = 1
      a(4,4) = 0
      a(4,5) = -b1(0)*b1(1)-b0(0)-b0(1)-ck2-alphaP*(a1(1)*b1(0)+a1(0)*
     & b1(1)+a0(0)+a0(1))
      a(5,0) = 0
      a(5,1) = 0
      a(5,2) = 0
      a(5,3) = 0
      a(5,4) = 1
      a(5,5) = -b1(0)-b1(1)-alphaP*(a1(0)+a1(1))
      else if( Np .eq. 3 )then
! Companion matrix for GDM model Np=3, File written by CG/DMX/matlab/gdm.maple
      a(0,0) = 0
      a(0,1) = 0
      a(0,2) = 0
      a(0,3) = 0
      a(0,4) = 0
      a(0,5) = 0
      a(0,6) = 0
      a(0,7) = -ck2*b0(0)*b0(1)*b0(2)
      a(1,0) = 1
      a(1,1) = 0
      a(1,2) = 0
      a(1,3) = 0
      a(1,4) = 0
      a(1,5) = 0
      a(1,6) = 0
      a(1,7) = -ck2*b0(0)*b0(1)*b1(2)-(ck2*b1(0)*b0(1)+ck2*b0(0)*b1(1))
     & *b0(2)
      a(2,0) = 0
      a(2,1) = 1
      a(2,2) = 0
      a(2,3) = 0
      a(2,4) = 0
      a(2,5) = 0
      a(2,6) = 0
      a(2,7) = -ck2*b0(0)*b0(1)-(ck2*b1(0)*b0(1)+ck2*b0(0)*b1(1))*b1(2)
     & -(ck2*b0(0)+ck2*b1(0)*b1(1)+(ck2+b0(0))*b0(1))*b0(2)-alphaP*(
     & a0(2)*b0(0)*b0(1)+a0(1)*b0(0)*b0(2)+a0(0)*b0(1)*b0(2))
      a(3,0) = 0
      a(3,1) = 0
      a(3,2) = 1
      a(3,3) = 0
      a(3,4) = 0
      a(3,5) = 0
      a(3,6) = 0
      a(3,7) = -ck2*b1(0)*b0(1)-ck2*b0(0)*b1(1)-(ck2*b0(0)+ck2*b1(0)*
     & b1(1)+(ck2+b0(0))*b0(1))*b1(2)-(ck2*b1(0)+(ck2+b0(0))*b1(1)+b1(
     & 0)*b0(1))*b0(2)-alphaP*(a0(0)*b0(1)*b1(2)+(a0(0)*b1(1)+a1(0)*
     & b0(1))*b0(2)+a0(1)*b0(0)*b1(2)+(a0(1)*b1(0)+a1(1)*b0(0))*b0(2)+
     & a0(2)*b0(0)*b1(1)+(a0(2)*b1(0)+a1(2)*b0(0))*b0(1))
      a(4,0) = 0
      a(4,1) = 0
      a(4,2) = 0
      a(4,3) = 1
      a(4,4) = 0
      a(4,5) = 0
      a(4,6) = 0
      a(4,7) = -ck2*b0(0)-ck2*b1(0)*b1(1)-(ck2+b0(0))*b0(1)-(ck2*b1(0)+
     & (ck2+b0(0))*b1(1)+b1(0)*b0(1))*b1(2)-(b1(0)*b1(1)+b0(0)+b0(1)+
     & ck2)*b0(2)-alphaP*(a0(0)*b0(1)+(a0(0)*b1(1)+a1(0)*b0(1))*b1(2)+
     & (a1(0)*b1(1)+a0(0))*b0(2)+a0(1)*b0(0)+(a0(1)*b1(0)+a1(1)*b0(0))
     & *b1(2)+(a1(1)*b1(0)+a0(1))*b0(2)+a0(2)*b0(0)+(a0(2)*b1(0)+a1(2)
     & *b0(0))*b1(1)+(a1(2)*b1(0)+a0(2))*b0(1))
      a(5,0) = 0
      a(5,1) = 0
      a(5,2) = 0
      a(5,3) = 0
      a(5,4) = 1
      a(5,5) = 0
      a(5,6) = 0
      a(5,7) = -ck2*b1(0)-(ck2+b0(0))*b1(1)-b1(0)*b0(1)-(b1(0)*b1(1)+
     & b0(0)+b0(1)+ck2)*b1(2)-(b1(0)+b1(1))*b0(2)-alphaP*(a0(0)*b1(1)+
     & a1(0)*b0(1)+(a1(0)*b1(1)+a0(0))*b1(2)+a1(0)*b0(2)+a0(1)*b1(0)+
     & a1(1)*b0(0)+(a1(1)*b1(0)+a0(1))*b1(2)+a1(1)*b0(2)+a0(2)*b1(0)+
     & a1(2)*b0(0)+(a1(2)*b1(0)+a0(2))*b1(1)+a1(2)*b0(1))
      a(6,0) = 0
      a(6,1) = 0
      a(6,2) = 0
      a(6,3) = 0
      a(6,4) = 0
      a(6,5) = 1
      a(6,6) = 0
      a(6,7) = -b1(0)*b1(1)-b0(0)-b0(1)-ck2-(b1(0)+b1(1))*b1(2)-b0(2)-
     & alphaP*(a1(1)*b1(0)+a1(2)*b1(0)+a1(0)*b1(1)+a1(0)*b1(2)+a1(2)*
     & b1(1)+a1(1)*b1(2)+a0(0)+a0(1)+a0(2))
      a(7,0) = 0
      a(7,1) = 0
      a(7,2) = 0
      a(7,3) = 0
      a(7,4) = 0
      a(7,5) = 0
      a(7,6) = 1
      a(7,7) = -b1(0)-b1(1)-b1(2)-alphaP*(a1(0)+a1(1)+a1(2))
      else
        stop 1234
      end if

      nd = 2*Np+2 ! order of the matrix

      ! Compute eigenvalues of a general non-symtreic matrix 
      call dgeev( 'N','N',nd,a,lda,sr,si,vl,1,vr,1,work,lwork,info )
      write(*,'("evalEigGDM: return from dgeev: info=",i8)') info

      if( .true. )then
          write(*,'(" evalEigGDM: input: mode=",i6)') mode
        do i=0,nd-1
          write(*,'(" evalEigGDM: i=",i3," s=(",1P,e20.12,",", 1P,
     & e20.12,")")') i,sr(i),si(i)
       end do
      end if
      if( mode.ge.0 )then
       ! Take user supplied mode: 
       iMode=min(mode,nd-1)
      else
        ! choose s with largest NEGATIVE imaginary part
        iMode=0
        sim=si(iMode)
        do i=0,nd-1
          if( si(i) .lt. sim )then
            iMode=i
           sim=si(i)
          end if
        end do
      end if
      sim=si(iMode)
      srm=sr(iMode)


      ! P = psi(s)*E
      eps=1.e-14 ! FIX ME
      do j=0,Np-1
        s = cmplx(srm,sim)
        if( abs(real(s)) + abs(imag(s)) .gt.eps )then
          psi = (a0(j)+a1(j)*s)/(s**2+b1(j)*s+b0(j))
        else
          psi=0.
        end if

        psir(j) = real(psi)
        psii(j) = imag(psi)
      end do

      call flush(6)

      return
      end




! =====================================================================================================
! 
! -----------------------------------------------------------------------------------
! Evaluate the "INVERSE" dispersion relation (compute k=*(kr,ki) given s=(sr,si) 
! for the generalized dispersion model (GDM) With multiple polarization vectors 
! -----------------------------------------------------------------------------------
!
!       E_tt - c^2 Delta(E) = -alphaP P_tt
!       P_tt + b1 P_1 + b0 = a0*E + a1*E_t 
!
!  Input:
!      sr,si : s=(sr,si)
!      Np : number of polarization vectors  
!      c  : 
!      a0(0:Np-1), a1(0:Np-1), b0(0:Np-1), b1(0:Np-1), alphaP : GDM parameters
! Output:
!      kr,ki : k=(kr,ki) = complex wave number 
!      psir(0:Np-1),psii(0:Np-1) : real and imaginary parts of psi : P = psi*E for s=(srm,sim) 
!
! =====================================================================================================
      subroutine evalInverseGDM( c, sr,si, Np, a0,a1,b0,b1,alphaP, kr,
     & ki,psir,psii )

      implicit none

      real c,sr,si
      integer Np
      real a0(0:*),a1(0:*),b0(0:*),b1(0:*),alphaP, psir(0:*),psii(0:*)
      real kr,ki

      ! local variables 
      real eps
      integer j
      complex*16 s, psi, sum , k

      s=cmplx(sr,si)

      sum = 0.
      do j=0,Np-1
        ! Phat = psi(s) * Ehat
        psi = (a0(j)+a1(j)*s)/(s**2+b1(j)*s+b0(j))
        psir(j) = real(psi)
        psii(j) = imag(psi)

        sum =sum + psi
      end do

      ! (ck)^2 = -s^2 - alphaP* s^2 *sum_k{ psi_k(s) }
      k = csqrt(-s*s -alphaP*s*s*( sum ) )/c

      kr = real(k)
      ki = imag(k)

      return
      end










! -----------------------------------------------------------------------------------
! Evaluate the dispersion relation for the generalized dispersion model (GDM)
! -----------------------------------------------------------------------------------
!
!       E_tt - c^2 Delta(E) = -alphaP P_tt
!       P_tt + b1 P_1 + b0 = a0*E + a1*E_t 
!
!  Input:
!      c,k, a0,a1,b0,b1,alphaP
! Output:
!      sr,si : real and imaginary part of s
!      psir,psii : real and imaginary parts of psi : P = psi*E 
!
      subroutine evalGeneralizedDispersionRelation( c,k, a0,a1,b0,b1,
     & alphaP, sr,si, psir,psii )


      ! implicit none
      implicit complex*16 (t)

      real c,k, a0,a1,b0,b1,alphaP, psir,psii
      real ck,ck2,  f ,ap, cki,ck2i
      real sr,si
      complex*16 ai, ss,s,cComplex, psi

      ! cComplex = dcmplx(c,0.) ! convert c to complex to force complex arithmetic below
      ck=c*k
      ck2=ck*ck
      cki=1/ck
      ck2i=1./ck2

      ap=alphaP

! File generated by overtureFramework/cg/mx/codes/gdm.maple

! File generated by overtureFramework/cg/mx/codes/dispersion.maple
! Here is root 1 from the dispersion relation exp( i*k*x + s*t) .
      t1 = a1*ap
      t4 = b1*ck+t1*ck
      t5 = ck ** 2
      t6 = 1./t5
      t9 = t4 ** 2
      t10 = t5 ** 2
      t11 = 1./t10
      t12 = t9*t11
      t14 = a0*ap
      t15 = t14+t5+b0
      t16 = t15*t6
      t18 = a0*a1
      t19 = ap ** 2
      t24 = a1 ** 2
      t25 = t24*t19
      t26 = t5*b0
      t32 = a0 ** 2
      t33 = t32*a0
      t34 = t19*ap
      t35 = t33*t34
      t37 = t32*t19
      t40 = b1 ** 2
      t41 = t40*t5
      t46 = b0*b1
      t47 = t46*t5
      t50 = t40*t10
      t52 = t10*t5
      t58 = b0*t40
      t59 = t58*t5
      t61 = b0*t10
      t63 = b0 ** 2
      t66 = t63*t5
      t68 = t63*b0
      t70 = t24*a1
      t71 = t70*t34
      t72 = t40*b1
      t76 = t32*t24
      t77 = t19 ** 2
      t86 = a0*t24
      t91 = t24 ** 2
      t95 = t46*t10
      t98 = t40 ** 2
      t99 = t98*t10
      t102 = t40*t52
      t110 = t77*b0
      t114 = t32*a1
      t130 = t63*b1
      t131 = t130*t5
      t134 = t58*t10
      t137 = b0*t52
      t144 = 12.*t71*t72*t10-3.*t76*t77*t40*t5-0.54E2*a0*t70*t77*t47-
     & 6.*t86*t34*t40*t10+0.81E2*t91*t77*t66-0.54E2*t71*t95+0.36E2*
     & t25*t99-3.*t25*t102+12.*t33*t24*t77*ap*b0+0.36E2*t76*t110*t5-
     & 6.*t114*t34*t72*t5-0.168E3*t86*t34*t59+0.36E2*t86*t34*b0*t10-
     & 0.66E2*t18*t19*t72*t10+0.270E3*t71*t131-0.150E3*t25*t134+12.*
     & t25*t137+0.36E2*t1*t98*b1*t10
      t165 = t34*t63
      t169 = t18*t19
      t170 = b0*t72
      t180 = t63*t40
      t181 = t180*t5
      t184 = t63*t10
      t198 = t10 ** 2
      t201 = -0.60E2*t1*t72*t52+0.24E2*t33*a1*t110*b1+12.*t35*t41+
     & 0.36E2*t76*t77*t63+0.312E3*t114*t34*t47-3.*t37*t98*t5+0.36E2*
     & t37*t50-0.360E3*t86*t165*t5-0.174E3*t169*t170*t5+0.552E3*t169*
     & t95-0.60E2*t14*t99+0.36E2*t14*t102+0.321E3*t25*t181-0.396E3*
     & t25*t184-0.192E3*t1*t170*t10+0.264E3*t1*t46*t52+12.*t98*t40*
     & t10+0.24E2*t98*t52+12.*t40*t198
      t203 = t32 ** 2
      t223 = b0*t98
      t231 = t68*t5
      t251 = -0.48E2*t203*t77*b0+12.*t35*t58-0.192E3*t35*t26+0.72E2*
     & t114*t165*b1+0.312E3*t37*t59-0.288E3*t37*t61+0.36E2*t86*t34*
     & t68-0.240E3*t169*t131-0.60E2*t14*t223*t5+0.156E3*t14*t134-
     & 0.192E3*t14*t137-0.396E3*t25*t231+0.156E3*t1*t63*t72*t5+
     & 0.264E3*t1*t130*t10-0.96E2*t223*t10-0.144E3*t58*t52-0.48E2*b0*
     & t198-0.192E3*t35*t63+0.36E2*t37*t180
      t262 = t63 ** 2
      t278 = t68*t40
      t298 = -0.552E3*t1*t68*b1*t5+0.72E2*t18*t19*t68*b1+0.24E2*t1*
     & t262*b1+0.24E2*t63*t98*t5-0.48E2*t262*b0+0.264E3*t180*t10-
     & 0.288E3*t68*t10+0.156E3*t14*t181+0.192E3*t14*t184+0.192E3*t14*
     & t231-0.192E3*t14*t262+0.36E2*t14*t278+12.*t25*t262+12.*t262*
     & t40+0.192E3*t262*t5-0.144E3*t278*t5-0.192E3*t37*t66-0.288E3*
     & t37*t68+0.192E3*t63*t52
      t301 = sqrt(t144+t201+t251+t298)
      t304 = -0.36E2*t18*t19*b1*t5-0.36E2*t1*b1*t10+0.24E2*t37*b0+12.*
     & t301*ck+0.180E3*t1*t47+0.24E2*t14*t10-0.240E3*t14*t26-0.36E2*
     & t14*t41+0.24E2*t14*t63+0.108E3*t25*t26+0.24E2*t37*t5+8.*t35+
     & 0.72E2*t50+8.*t52+0.72E2*t59-0.264E3*t61-0.264E3*t66+8.*t68
      t305 = t304 ** (1./3.)
      t307 = t6*t305/6.
      t321 = 2./3.*(-3.*t1*b1*t5+2.*t14*b0+2.*t14*t5+t10+0.14E2*t26+
     & t37-3.*t41+t63)*t6/t305
      t323 = sqrt(t12/4.-2./3.*t16+t307+t321)
      t340 = sqrt(t12/2.-4./3.*t16-t307-t321+(t15*t11*t4-2.*b1/ck-t9*
     & t4/t52/4.)/t323)
      ss = -t4*t6/4.+t323/2.+t340/2.


      ! The valid root will have an imaginary part close to I*ck 
      if( abs(dimag(ss)) < .01*ck )then
! File generated by overtureFramework/cg/mx/codes/dispersion.maple
! Here is root 3 from the dispersion relation exp( i*k*x + s*t) .
      t1 = a1*ap
      t4 = b1*ck+t1*ck
      t5 = ck ** 2
      t6 = 1./t5
      t9 = t4 ** 2
      t10 = t5 ** 2
      t11 = 1./t10
      t12 = t9*t11
      t14 = a0*ap
      t15 = t14+t5+b0
      t16 = t15*t6
      t18 = a0*a1
      t19 = ap ** 2
      t24 = a1 ** 2
      t25 = t24*t19
      t26 = t5*b0
      t32 = a0 ** 2
      t33 = t32*a0
      t34 = t19*ap
      t35 = t33*t34
      t37 = t32*t19
      t40 = b1 ** 2
      t41 = t40*t5
      t46 = b0*b1
      t47 = t46*t5
      t50 = t40*t10
      t52 = t10*t5
      t58 = b0*t40
      t59 = t58*t5
      t61 = b0*t10
      t63 = b0 ** 2
      t66 = t63*t5
      t68 = t63*b0
      t70 = t24*a1
      t71 = t70*t34
      t72 = t40*b1
      t76 = t32*t24
      t77 = t19 ** 2
      t86 = a0*t24
      t91 = t24 ** 2
      t95 = t46*t10
      t98 = t40 ** 2
      t99 = t98*t10
      t102 = t40*t52
      t110 = t77*b0
      t114 = t32*a1
      t130 = t63*b1
      t131 = t130*t5
      t134 = t58*t10
      t137 = b0*t52
      t144 = 12.*t71*t72*t10-3.*t76*t77*t40*t5-0.54E2*a0*t70*t77*t47-
     & 6.*t86*t34*t40*t10+0.81E2*t91*t77*t66-0.54E2*t71*t95+0.36E2*
     & t25*t99-3.*t25*t102+12.*t33*t24*t77*ap*b0+0.36E2*t76*t110*t5-
     & 6.*t114*t34*t72*t5-0.168E3*t86*t34*t59+0.36E2*t86*t34*b0*t10-
     & 0.66E2*t18*t19*t72*t10+0.270E3*t71*t131-0.150E3*t25*t134+12.*
     & t25*t137+0.36E2*t1*t98*b1*t10
      t165 = t34*t63
      t169 = t18*t19
      t170 = b0*t72
      t180 = t63*t40
      t181 = t180*t5
      t184 = t63*t10
      t198 = t10 ** 2
      t201 = -0.60E2*t1*t72*t52+0.24E2*t33*a1*t110*b1+12.*t35*t41+
     & 0.36E2*t76*t77*t63+0.312E3*t114*t34*t47-3.*t37*t98*t5+0.36E2*
     & t37*t50-0.360E3*t86*t165*t5-0.174E3*t169*t170*t5+0.552E3*t169*
     & t95-0.60E2*t14*t99+0.36E2*t14*t102+0.321E3*t25*t181-0.396E3*
     & t25*t184-0.192E3*t1*t170*t10+0.264E3*t1*t46*t52+12.*t98*t40*
     & t10+0.24E2*t98*t52+12.*t40*t198
      t203 = t32 ** 2
      t223 = b0*t98
      t231 = t68*t5
      t251 = -0.48E2*t203*t77*b0+12.*t35*t58-0.192E3*t35*t26+0.72E2*
     & t114*t165*b1+0.312E3*t37*t59-0.288E3*t37*t61+0.36E2*t86*t34*
     & t68-0.240E3*t169*t131-0.60E2*t14*t223*t5+0.156E3*t14*t134-
     & 0.192E3*t14*t137-0.396E3*t25*t231+0.156E3*t1*t63*t72*t5+
     & 0.264E3*t1*t130*t10-0.96E2*t223*t10-0.144E3*t58*t52-0.48E2*b0*
     & t198-0.192E3*t35*t63+0.36E2*t37*t180
      t262 = t63 ** 2
      t278 = t68*t40
      t298 = -0.552E3*t1*t68*b1*t5+0.72E2*t18*t19*t68*b1+0.24E2*t1*
     & t262*b1+0.24E2*t63*t98*t5-0.48E2*t262*b0+0.264E3*t180*t10-
     & 0.288E3*t68*t10+0.156E3*t14*t181+0.192E3*t14*t184+0.192E3*t14*
     & t231-0.192E3*t14*t262+0.36E2*t14*t278+12.*t25*t262+12.*t262*
     & t40+0.192E3*t262*t5-0.144E3*t278*t5-0.192E3*t37*t66-0.288E3*
     & t37*t68+0.192E3*t63*t52
      t301 = sqrt(t144+t201+t251+t298)
      t304 = -0.36E2*t18*t19*b1*t5-0.36E2*t1*b1*t10+0.24E2*t37*b0+12.*
     & t301*ck+0.180E3*t1*t47+0.24E2*t14*t10-0.240E3*t14*t26-0.36E2*
     & t14*t41+0.24E2*t14*t63+0.108E3*t25*t26+0.24E2*t37*t5+8.*t35+
     & 0.72E2*t50+8.*t52+0.72E2*t59-0.264E3*t61-0.264E3*t66+8.*t68
      t305 = t304 ** (1./3.)
      t307 = t6*t305/6.
      t321 = 2./3.*(-3.*t1*b1*t5+2.*t14*b0+2.*t14*t5+t10+0.14E2*t26+
     & t37-3.*t41+t63)*t6/t305
      t323 = sqrt(t12/4.-2./3.*t16+t307+t321)
      t340 = sqrt(t12/2.-4./3.*t16-t307-t321-(t15*t11*t4-2.*b1/ck-t9*
     & t4/t52/4.)/t323)
      ss = -t4*t6/4.-t323/2.+t340/2.

        write(*,'("evalGDM: Use root 3")')
        if( abs(dimag(ss)) < .01*ck )then
          write(*,'("evalGDM: INVALID root found!?")')
          stop 6666
        end if
      else
        write(*,'("evalGDM: Use root 1")')
      end if

      s =ss*ck

      if( dimag(s)>0. )then
        s=dconjg(s) ! choose right moving wave, imag(s) < 0
      end if

      sr= dreal(s)
      si= dimag(s)

      ! P = psi(s)*E
      psi = (a0+a1*s)/(s**2+b1*s+b0)
      psir = dreal(psi)
      psii = dimag(psi)  ! NOTE: CHOOSE RIGHT MOVING WAVE

      ! check root:
      f = cabs((ss**2 + 1)*(ss**2 + cki*b1*ss+ck2i*b0) + ap*ss**2*cki*(
     &  a1*ss+ cki*a0 ) )

      write(*,'("evalGDM: c,k,a0,a1,b0,b1=",6(1P,e20.12)," alphaP=",
     & 1Pe20.12)') c,k,a0,a1,b0,b1,alphaP
      write(*,'("evalGDM: sr,si=",2(1P,e24.15)," |f|=",e12.4)') sr,si,f
      write(*,'("evalGDM: psir,psii=",2e12.4)') psir,psii


      return
      end


! Evaluate the dispersion relation for the Drude model 
!
!    E_tt + c^2 k^2 E = -alphaP*P_tt
!
!  Input:
!      c0,eps,gam,omegap,k : 
! Output:
!      reS,imS
!
      subroutine evalDispersionRelation( c0,eps,gam,omegap,k, reS,imS )


      implicit none

      real c0,eps,gam,omegap,k, ck2, epsi, om2, det
      real reS,imS
      complex*16 ai, c, s

      ck2=(c0*k)**2
      epsi=1./eps
      om2=omegap**2

       ! ai=cmplx(0.,1.)  ! i 
       c = cmplx(c0,0.) ! convert c to complex to force complex arithmetic below
! File generated by overtureFramework/cg/mx/codes/dispersion.maple
! Here is root 3 from the dispersion relation exp( i*k*x + s*t) .
! s = -1/12*(36*epsi*gam*om2-72*ck2*gam-8*gam^3+12*(12*epsi^3*om2^3-3*epsi^2*gam^2*om2^2+36*ck2*epsi^2*om2^2-60*ck2*epsi*gam^2*om2+12*ck2*gam^4+36*ck2^2*epsi*om2+24*ck2^2*gam^2+12*ck2^3)^(1/2))^(1/3)+3*(1/3*epsi*om2+1/3*ck2-1/9*gam^2)/(36*epsi*gam*om2-72*ck2*gam-8*gam^3+12*(12*epsi^3*om2^3-3*epsi^2*gam^2*om2^2+36*ck2*epsi^2*om2^2-60*ck2*epsi*gam^2*om2+12*ck2*gam^4+36*ck2^2*epsi*om2+24*ck2^2*gam^2+12*ck2^3)^(1/2))^(1/3)-1/3*gam+1/2*I*3^(1/2)*(1/6*(36*epsi*gam*om2-72*ck2*gam-8*gam^3+12*(12*epsi^3*om2^3-3*epsi^2*gam^2*om2^2+36*ck2*epsi^2*om2^2-60*ck2*epsi*gam^2*om2+12*ck2*gam^4+36*ck2^2*epsi*om2+24*ck2^2*gam^2+12*ck2^3)^(1/2))^(1/3)+6*(1/3*epsi*om2+1/3*ck2-1/9*gam^2)/(36*epsi*gam*om2-72*ck2*gam-8*gam^3+12*(12*epsi^3*om2^3-3*epsi^2*gam^2*om2^2+36*ck2*epsi^2*om2^2-60*ck2*epsi*gam^2*om2+12*ck2*gam^4+36*ck2^2*epsi*om2+24*ck2^2*gam^2+12*ck2^3)^(1/2))^(1/3))
      s = -(((36*epsi*gam*om2)-(72*ck2*gam)-(8*gam ** 3)+12.*sqrt((12*
     & epsi ** 3*om2 ** 3-3*epsi ** 2*om2 ** 2*gam ** 2+36*epsi ** 2*
     & om2 ** 2*ck2-60*epsi*om2*ck2*gam ** 2+12*ck2*gam ** 4+36*epsi*
     & om2*ck2 ** 2+24*ck2 ** 2*gam ** 2+12*ck2 ** 3))) ** (1./3.)
     & /12.)+(3.*((epsi*om2)/3.+(ck2)/3.-(gam ** 2)/0.9E1)*((36*epsi*
     & gam*om2)-(72*ck2*gam)-(8*gam ** 3)+12.*sqrt((12*epsi ** 3*om2 *
     & * 3-3*epsi ** 2*om2 ** 2*gam ** 2+36*epsi ** 2*om2 ** 2*ck2-60*
     & epsi*om2*ck2*gam ** 2+12*ck2*gam ** 4+36*epsi*om2*ck2 ** 2+24*
     & ck2 ** 2*gam ** 2+12*ck2 ** 3))) ** (-1./3.))-((gam)/3.)+cmplx(
     & 0, 1./2.)*sqrt(3.)*(((36*epsi*gam*om2)-(72*ck2*gam)-(8*gam ** 
     & 3)+12.*sqrt((12*epsi ** 3*om2 ** 3-3*epsi ** 2*om2 ** 2*gam ** 
     & 2+36*epsi ** 2*om2 ** 2*ck2-60*epsi*om2*ck2*gam ** 2+12*ck2*
     & gam ** 4+36*epsi*om2*ck2 ** 2+24*ck2 ** 2*gam ** 2+12*ck2 ** 3)
     & )) ** (1./3.)/6.+6.*((epsi*om2)/3.+(ck2)/3.-(gam ** 2)/0.9E1)*(
     & (36*epsi*gam*om2)-(72*ck2*gam)-(8*gam ** 3)+12.*sqrt((12*epsi *
     & * 3*om2 ** 3-3*epsi ** 2*om2 ** 2*gam ** 2+36*epsi ** 2*om2 ** 
     & 2*ck2-60*epsi*om2*ck2*gam ** 2+12*ck2*gam ** 4+36*epsi*om2*ck2 
     & ** 2+24*ck2 ** 2*gam ** 2+12*ck2 ** 3))) ** (-1./3.))

      reS= real(s)
      imS= aimag(s)

      ! check root:
      det = cabs((s**2 + ck2)*(s**2 + gam*s) + om2*s**2*epsi)
      write(*,'("*OLD* evalDisp: eps,omegap=",2e12.4," gam,k=",2e12.4,
     & " |det|=",e12.4)') eps,omegap,gam,k,det
      write(*,'("*OLD* evalDisp: reS,imS=",2(1P,e22.15))') reS,imS
      return
      end



