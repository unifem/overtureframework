! This file automatically generated from cnsSlipWallBC.bf with bpp.
!
! routines for applying a slip wall BC
!

c Use this next macro to declare the statement functions that are defined below
c To include derivatives of rx use OPTION=RX


c Define statement functions for difference approximations of order 2 
c To include derivatives of rx use OPTION=RX
c To include derivatives of rx use OPTION=RX




! ====================================================================
! Look up an integer parameter from the data base
! ====================================================================

! ====================================================================
! Look up a real parameter from the data base
! ====================================================================








! Extrapolate the normal component on the second ghost line

!   On the slip wall extended boundary points we solve for the normal components from:
!        u.x + v.y = 0
!       D+^p ( n.u ) = 0
! DIR = r,s,t
! DIM = 2,3

! Determine the tangential components of the velocity from 
!                D+D-
!                D+^6( tv.uv ) = o
!  **curvilinear grid case ***

! Determine the tangential components of the velocity from 
!                D+D-
!                D+^6( tv.uv ) = o


! Determine the tangential components of the velocity from the NS equations and extrapolation
!  We assume the equations for the tangential components decouple (which they do excecpt for cross terms
!   on non-orthogonal grids) -- We first solve for all components of uv and then just set the tangential ones.

! ******************** 3D Version ***************************



! ************* rectangular grid version *****************
!   In this case we only need to compute and assign the tangential components



! Apply the boundary condition div(u)=0 div(u).n=0 to determine the normal compoennts of the 2 ghost points
!  Curvilinear grid case
! DIR = r,s,t

!  Three-dimensional version






      subroutine cnsSlipWallBC(nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,
     & nd4b,ipar,rpar, u, u2,  gv, gv2, gtt, mask, x,rsxy, bc, 
     & indexRange, exact, uKnown, pdb, ierr )
!========================================================================
!
!     Apply a slip wall boundary condition 
!
!  u : solution at time t
!  u2 : solution at time t-dt
! 
! gv (input) : g' -  gridVelocity at time t (for moving grids)
! gvt (input) : g'' - we need the gridAcceleration on the boundaries
! gvtt (input) : g''' - we may need the 3rd time derivative of g on the boudary
!
!========================================================================
      implicit none
      integer nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b
!     integer *8 exact ! holds pointer to OGFunction
      integer exact ! holds pointer to OGFunction
      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real u2(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real gv(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
      real gv2(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
      real gtt(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
      real x(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
      real uKnown(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real rpar(0:*)

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer indexRange(0:1,0:2), bc(0:1,0:2)
      integer ipar(0:*),ierr

      double precision pdb  ! pointer to data base

!.......local

      integer kd,kd3,i1,i2,i3,n1a,n1b,n2a,n2b,n3a,n3b
      integer is,j1,j2,j3,side,axis,twilightZone,bcOption,
     & knownSolution,numberOfComponents
      integer rc,tc,uc,vc,wc,sc,unc,utc,n
      integer grid,orderOfAccuracy,gridIsMoving,useWhereMask
      integer gridType,isAxisymmetric,numberOfSpecies,radialAxis,
     & axisymmetricWithSwirl,urc,uac
      integer nr(0:1,0:2)

      integer ok,getInt,getReal
      real densityLowerBound, pressureLowerBound

      real sxi,syi,szi,txi,tyi,tzi,rxi,ryi,rzi
      real pn,rho,rhon,nDotGradR,nDotGradS,tp,tpn,rhor,rhos,tps,ps,tpm,
     & pm,pp,pr,tpr
      integer axisp1

      integer is1,is2,is3,js1,js2,js3,ks1,ks2,ks3,s,m,mm

      real t,dt
      real an1,an2,an3,nDotU,aNorm,epsx,gamma
      real dr(0:2),dx(0:2)
      real u0s,v0s,w0s,sgn

      real rra,ura,vra,wra, rsa,usa,vsa,wsa, urra,vrra,wrra, ussa,vssa,
     & wssa, rrsa, ursa,vrsa,wrsa
      real rxa,rya,sxa,sya, rxra,ryra,sxra,syra, rxsa,rysa,sxsa,sysa
      real ra,ua,va,wa,fra,rhot

      real hx,hy,gm1
      real r0,rx0,ry0,rxx0,rxy0,ryy0, rt0,rtx0,rty0,rtt0
      real u0,ux0,uy0,uxx0,uxy0,uyy0, ut0,utx0,uty0,utt0
      real v0,vx0,vy0,vxx0,vxy0,vyy0, vt0,vtx0,vty0,vtt0
      real p0,px0,py0,pxx0,pxy0,pyy0, pt0,ptx0,pty0,ptt0
      real q0,qx0,qy0,qxx0,qxy0,qyy0, qt0,qtx0,qty0,qtt0
      real fv(0:20),uv(0:10),uvp(0:10),uvm(0:10),z0,tm,ad2dt
      real ep ! holds the pointer to the TZ function
      integer debug
      logical testSym,getGhostByTaylor

      real r1,u1,v1,q1,p1,s1, s0,st0,stt0
      real ur1,vr1,nDotU1,nDotuv(2),adu(0:10),usp,usm
      integer k1,k2,k3

      real rr0,rxr0,ryr0
      real ur0,uxr0,uyr0
      real vr0,vxr0,vyr0
      real qr0,qxr0,qyr0
      real pr0,pxr0,pyr0
      real utr0,vtr0
      real u2xr22,u2xs22,u2yr22,u2ys22
      real gvux0,gvuy0,gvvx0,gvvy0,gttu0,gttv0
      real s1p,sr

!..................
      integer rectangular,curvilinear
      parameter( rectangular=0, curvilinear=1 )

      integer slipWallSymmetry, slipWallPressureEntropySymmetry, 
     & slipWallTaylor, slipWallCharacteristic
      parameter( slipWallSymmetry=0, slipWallPressureEntropySymmetry=1,
     &  slipWallTaylor=2, slipWallCharacteristic=3 )

      integer
     &     noSlipWall,
     &     inflowWithVelocityGiven,
     &     slipWall,
     &     outflow,
     &     convectiveOutflow,
     &     tractionFree,
     &     inflowWithPandTV,
     &     dirichletBoundaryCondition,
     &     symmetry,
     &     axisymmetric
      parameter( noSlipWall=1,inflowWithVelocityGiven=2,
     & slipWall=4,outflow=5,convectiveOutflow=14,tractionFree=15,
     & inflowWithPandTV=3,
     &  dirichletBoundaryCondition=12,
     &  symmetry=11,axisymmetric=13 )

      integer supersonicFlowInAnExpandingChannel,
     & userDefinedKnownSolution
      parameter( userDefinedKnownSolution=1, 
     & supersonicFlowInAnExpandingChannel=2 )

      ! declare variables for difference approximations of u and RX
       real d12
       real d22
       real h12
       real h22
       real rxr2
       real rxs2
       real rxt2
       real rxrr2
       real rxss2
       real rxrs2
       real ryr2
       real rys2
       real ryt2
       real ryrr2
       real ryss2
       real ryrs2
       real rzr2
       real rzs2
       real rzt2
       real rzrr2
       real rzss2
       real rzrs2
       real sxr2
       real sxs2
       real sxt2
       real sxrr2
       real sxss2
       real sxrs2
       real syr2
       real sys2
       real syt2
       real syrr2
       real syss2
       real syrs2
       real szr2
       real szs2
       real szt2
       real szrr2
       real szss2
       real szrs2
       real txr2
       real txs2
       real txt2
       real txrr2
       real txss2
       real txrs2
       real tyr2
       real tys2
       real tyt2
       real tyrr2
       real tyss2
       real tyrs2
       real tzr2
       real tzs2
       real tzt2
       real tzrr2
       real tzss2
       real tzrs2
       real rxx21
       real rxx22
       real rxy22
       real rxx23
       real rxy23
       real rxz23
       real ryx22
       real ryy22
       real ryx23
       real ryy23
       real ryz23
       real rzx22
       real rzy22
       real rzx23
       real rzy23
       real rzz23
       real sxx22
       real sxy22
       real sxx23
       real sxy23
       real sxz23
       real syx22
       real syy22
       real syx23
       real syy23
       real syz23
       real szx22
       real szy22
       real szx23
       real szy23
       real szz23
       real txx22
       real txy22
       real txx23
       real txy23
       real txz23
       real tyx22
       real tyy22
       real tyx23
       real tyy23
       real tyz23
       real tzx22
       real tzy22
       real tzx23
       real tzy23
       real tzz23
       real ur2
       real us2
       real ut2
       real urr2
       real uss2
       real urs2
       real utt2
       real urt2
       real ust2
       real urrr2
       real usss2
       real uttt2
       real ux21
       real uy21
       real uz21
       real ux22
       real uy22
       real uz22
       real ux23
       real uy23
       real uz23
       real uxx21
       real uyy21
       real uxy21
       real uxz21
       real uyz21
       real uzz21
       real ulaplacian21
       real uxx22
       real uyy22
       real uxy22
       real uxz22
       real uyz22
       real uzz22
       real ulaplacian22
       real uxx23
       real uyy23
       real uzz23
       real uxy23
       real uxz23
       real uyz23
       real ulaplacian23
       real ux23r
       real uy23r
       real uz23r
       real uxx23r
       real uyy23r
       real uxy23r
       real uzz23r
       real uxz23r
       real uyz23r
       real ux21r
       real uy21r
       real uz21r
       real uxx21r
       real uyy21r
       real uzz21r
       real uxy21r
       real uxz21r
       real uyz21r
       real ulaplacian21r
       real ux22r
       real uy22r
       real uz22r
       real uxx22r
       real uyy22r
       real uzz22r
       real uxy22r
       real uxz22r
       real uyz22r
       real ulaplacian22r
       real ulaplacian23r
       real uxxx22r
       real uyyy22r
       real uxxy22r
       real uxyy22r
       real uxxxx22r
       real uyyyy22r
       real uxxyy22r
       real uxxx23r
       real uyyy23r
       real uzzz23r
       real uxxy23r
       real uxxz23r
       real uxyy23r
       real uyyz23r
       real uxzz23r
       real uyzz23r
       real uxxxx23r
       real uyyyy23r
       real uzzzz23r
       real uxxyy23r
       real uxxzz23r
       real uyyzz23r
       real uLapSq22r
       real uLapSq23r
      ! declare difference approximations for u2
       real u2r2
       real u2s2
       real u2t2
       real u2rr2
       real u2ss2
       real u2rs2
       real u2tt2
       real u2rt2
       real u2st2
       real u2rrr2
       real u2sss2
       real u2ttt2
       real u2x21
       real u2y21
       real u2z21
       real u2x22
       real u2y22
       real u2z22
       real u2x23
       real u2y23
       real u2z23
       real u2xx21
       real u2yy21
       real u2xy21
       real u2xz21
       real u2yz21
       real u2zz21
       real u2laplacian21
       real u2xx22
       real u2yy22
       real u2xy22
       real u2xz22
       real u2yz22
       real u2zz22
       real u2laplacian22
       real u2xx23
       real u2yy23
       real u2zz23
       real u2xy23
       real u2xz23
       real u2yz23
       real u2laplacian23
       real u2x23r
       real u2y23r
       real u2z23r
       real u2xx23r
       real u2yy23r
       real u2xy23r
       real u2zz23r
       real u2xz23r
       real u2yz23r
       real u2x21r
       real u2y21r
       real u2z21r
       real u2xx21r
       real u2yy21r
       real u2zz21r
       real u2xy21r
       real u2xz21r
       real u2yz21r
       real u2laplacian21r
       real u2x22r
       real u2y22r
       real u2z22r
       real u2xx22r
       real u2yy22r
       real u2zz22r
       real u2xy22r
       real u2xz22r
       real u2yz22r
       real u2laplacian22r
       real u2laplacian23r
       real u2xxx22r
       real u2yyy22r
       real u2xxy22r
       real u2xyy22r
       real u2xxxx22r
       real u2yyyy22r
       real u2xxyy22r
       real u2xxx23r
       real u2yyy23r
       real u2zzz23r
       real u2xxy23r
       real u2xxz23r
       real u2xyy23r
       real u2yyz23r
       real u2xzz23r
       real u2yzz23r
       real u2xxxx23r
       real u2yyyy23r
       real u2zzzz23r
       real u2xxyy23r
       real u2xxzz23r
       real u2yyzz23r
       real u2LapSq22r
       real u2LapSq23r
      ! declare for derivatives of gv2
       real gv2r2
       real gv2s2
       real gv2t2
       real gv2rr2
       real gv2ss2
       real gv2rs2
       real gv2tt2
       real gv2rt2
       real gv2st2
       real gv2rrr2
       real gv2sss2
       real gv2ttt2
       real gv2x21
       real gv2y21
       real gv2z21
       real gv2x22
       real gv2y22
       real gv2z22
       real gv2x23
       real gv2y23
       real gv2z23
       real gv2xx21
       real gv2yy21
       real gv2xy21
       real gv2xz21
       real gv2yz21
       real gv2zz21
       real gv2laplacian21
       real gv2xx22
       real gv2yy22
       real gv2xy22
       real gv2xz22
       real gv2yz22
       real gv2zz22
       real gv2laplacian22
       real gv2xx23
       real gv2yy23
       real gv2zz23
       real gv2xy23
       real gv2xz23
       real gv2yz23
       real gv2laplacian23
       real gv2x23r
       real gv2y23r
       real gv2z23r
       real gv2xx23r
       real gv2yy23r
       real gv2xy23r
       real gv2zz23r
       real gv2xz23r
       real gv2yz23r
       real gv2x21r
       real gv2y21r
       real gv2z21r
       real gv2xx21r
       real gv2yy21r
       real gv2zz21r
       real gv2xy21r
       real gv2xz21r
       real gv2yz21r
       real gv2laplacian21r
       real gv2x22r
       real gv2y22r
       real gv2z22r
       real gv2xx22r
       real gv2yy22r
       real gv2zz22r
       real gv2xy22r
       real gv2xz22r
       real gv2yz22r
       real gv2laplacian22r
       real gv2laplacian23r
       real gv2xxx22r
       real gv2yyy22r
       real gv2xxy22r
       real gv2xyy22r
       real gv2xxxx22r
       real gv2yyyy22r
       real gv2xxyy22r
       real gv2xxx23r
       real gv2yyy23r
       real gv2zzz23r
       real gv2xxy23r
       real gv2xxz23r
       real gv2xyy23r
       real gv2yyz23r
       real gv2xzz23r
       real gv2yzz23r
       real gv2xxxx23r
       real gv2yyyy23r
       real gv2zzzz23r
       real gv2xxyy23r
       real gv2xxzz23r
       real gv2yyzz23r
       real gv2LapSq22r
       real gv2LapSq23r

! .............. begin statement functions
      real rx,ry,rz,sx,sy,sz,tx,ty,tz
      real ogf,diss2,ad2,disst2,tanDiss2

!.......statement functions for jacobian
      rx(i1,i2,i3)=rsxy(i1,i2,i3,0,0)
      ry(i1,i2,i3)=rsxy(i1,i2,i3,0,1)
      rz(i1,i2,i3)=rsxy(i1,i2,i3,0,2)
      sx(i1,i2,i3)=rsxy(i1,i2,i3,1,0)
      sy(i1,i2,i3)=rsxy(i1,i2,i3,1,1)
      sz(i1,i2,i3)=rsxy(i1,i2,i3,1,2)
      tx(i1,i2,i3)=rsxy(i1,i2,i3,2,0)
      ty(i1,i2,i3)=rsxy(i1,i2,i3,2,1)
      tz(i1,i2,i3)=rsxy(i1,i2,i3,2,2)

      diss2(i1,i2,i3,n)=ad2dt*(u2(i1+1,i2,i3,n)+u2(i1-1,i2,i3,n)+u2(i1,
     & i2-1,i3,n)+u2(i1,i2+1,i3,n)-4.*u2(i1,i2,i3,n))

      disst2(i1,i2,i3,n)=ad2dt*(u2(i1+js1,i2+js2,i3,n)+u2(i1-js1,i2-
     & js2,i3,n)-2.*u2(i1,i2,i3,n))

      ! another form of tangential dissipation: 
      tanDiss2(i1,i2,i3,n)=(1.+adu(n))*ad2dt*(u2(i1+js1,i2+js2,i3,n)+
     & u2(i1-js1,i2-js2,i3,n)-2.*u2(i1,i2,i3,n))


!     The next macro call will define the difference approximation statement functions
      d12(kd) = 1./(2.*dr(kd))
      d22(kd) = 1./(dr(kd)**2)
      ur2(i1,i2,i3,kd)=(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))*d12(0)
      us2(i1,i2,i3,kd)=(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))*d12(1)
      ut2(i1,i2,i3,kd)=(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))*d12(2)
      urr2(i1,i2,i3,kd)=(-2.*u(i1,i2,i3,kd)+(u(i1+1,i2,i3,kd)+u(i1-1,
     & i2,i3,kd)) )*d22(0)
      uss2(i1,i2,i3,kd)=(-2.*u(i1,i2,i3,kd)+(u(i1,i2+1,i3,kd)+u(i1,i2-
     & 1,i3,kd)) )*d22(1)
      urs2(i1,i2,i3,kd)=(ur2(i1,i2+1,i3,kd)-ur2(i1,i2-1,i3,kd))*d12(1)
      utt2(i1,i2,i3,kd)=(-2.*u(i1,i2,i3,kd)+(u(i1,i2,i3+1,kd)+u(i1,i2,
     & i3-1,kd)) )*d22(2)
      urt2(i1,i2,i3,kd)=(ur2(i1,i2,i3+1,kd)-ur2(i1,i2,i3-1,kd))*d12(2)
      ust2(i1,i2,i3,kd)=(us2(i1,i2,i3+1,kd)-us2(i1,i2,i3-1,kd))*d12(2)
      urrr2(i1,i2,i3,kd)=(-2.*(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))+(u(
     & i1+2,i2,i3,kd)-u(i1-2,i2,i3,kd)) )*d22(0)*d12(0)
      usss2(i1,i2,i3,kd)=(-2.*(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))+(u(
     & i1,i2+2,i3,kd)-u(i1,i2-2,i3,kd)) )*d22(1)*d12(1)
      uttt2(i1,i2,i3,kd)=(-2.*(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))+(u(
     & i1,i2,i3+2,kd)-u(i1,i2,i3-2,kd)) )*d22(2)*d12(2)
      rxr2(i1,i2,i3)=(rx(i1+1,i2,i3)-rx(i1-1,i2,i3))*d12(0)
      rxs2(i1,i2,i3)=(rx(i1,i2+1,i3)-rx(i1,i2-1,i3))*d12(1)
      rxt2(i1,i2,i3)=(rx(i1,i2,i3+1)-rx(i1,i2,i3-1))*d12(2)
      rxrr2(i1,i2,i3)=(-2.*rx(i1,i2,i3)+(rx(i1+1,i2,i3)+rx(i1-1,i2,i3))
     &  )*d22(0)
      rxss2(i1,i2,i3)=(-2.*rx(i1,i2,i3)+(rx(i1,i2+1,i3)+rx(i1,i2-1,i3))
     &  )*d22(1)
      rxrs2(i1,i2,i3)=(rxr2(i1,i2+1,i3)-rxr2(i1,i2-1,i3))*d12(1)
      ryr2(i1,i2,i3)=(ry(i1+1,i2,i3)-ry(i1-1,i2,i3))*d12(0)
      rys2(i1,i2,i3)=(ry(i1,i2+1,i3)-ry(i1,i2-1,i3))*d12(1)
      ryt2(i1,i2,i3)=(ry(i1,i2,i3+1)-ry(i1,i2,i3-1))*d12(2)
      ryrr2(i1,i2,i3)=(-2.*ry(i1,i2,i3)+(ry(i1+1,i2,i3)+ry(i1-1,i2,i3))
     &  )*d22(0)
      ryss2(i1,i2,i3)=(-2.*ry(i1,i2,i3)+(ry(i1,i2+1,i3)+ry(i1,i2-1,i3))
     &  )*d22(1)
      ryrs2(i1,i2,i3)=(ryr2(i1,i2+1,i3)-ryr2(i1,i2-1,i3))*d12(1)
      rzr2(i1,i2,i3)=(rz(i1+1,i2,i3)-rz(i1-1,i2,i3))*d12(0)
      rzs2(i1,i2,i3)=(rz(i1,i2+1,i3)-rz(i1,i2-1,i3))*d12(1)
      rzt2(i1,i2,i3)=(rz(i1,i2,i3+1)-rz(i1,i2,i3-1))*d12(2)
      rzrr2(i1,i2,i3)=(-2.*rz(i1,i2,i3)+(rz(i1+1,i2,i3)+rz(i1-1,i2,i3))
     &  )*d22(0)
      rzss2(i1,i2,i3)=(-2.*rz(i1,i2,i3)+(rz(i1,i2+1,i3)+rz(i1,i2-1,i3))
     &  )*d22(1)
      rzrs2(i1,i2,i3)=(rzr2(i1,i2+1,i3)-rzr2(i1,i2-1,i3))*d12(1)
      sxr2(i1,i2,i3)=(sx(i1+1,i2,i3)-sx(i1-1,i2,i3))*d12(0)
      sxs2(i1,i2,i3)=(sx(i1,i2+1,i3)-sx(i1,i2-1,i3))*d12(1)
      sxt2(i1,i2,i3)=(sx(i1,i2,i3+1)-sx(i1,i2,i3-1))*d12(2)
      sxrr2(i1,i2,i3)=(-2.*sx(i1,i2,i3)+(sx(i1+1,i2,i3)+sx(i1-1,i2,i3))
     &  )*d22(0)
      sxss2(i1,i2,i3)=(-2.*sx(i1,i2,i3)+(sx(i1,i2+1,i3)+sx(i1,i2-1,i3))
     &  )*d22(1)
      sxrs2(i1,i2,i3)=(sxr2(i1,i2+1,i3)-sxr2(i1,i2-1,i3))*d12(1)
      syr2(i1,i2,i3)=(sy(i1+1,i2,i3)-sy(i1-1,i2,i3))*d12(0)
      sys2(i1,i2,i3)=(sy(i1,i2+1,i3)-sy(i1,i2-1,i3))*d12(1)
      syt2(i1,i2,i3)=(sy(i1,i2,i3+1)-sy(i1,i2,i3-1))*d12(2)
      syrr2(i1,i2,i3)=(-2.*sy(i1,i2,i3)+(sy(i1+1,i2,i3)+sy(i1-1,i2,i3))
     &  )*d22(0)
      syss2(i1,i2,i3)=(-2.*sy(i1,i2,i3)+(sy(i1,i2+1,i3)+sy(i1,i2-1,i3))
     &  )*d22(1)
      syrs2(i1,i2,i3)=(syr2(i1,i2+1,i3)-syr2(i1,i2-1,i3))*d12(1)
      szr2(i1,i2,i3)=(sz(i1+1,i2,i3)-sz(i1-1,i2,i3))*d12(0)
      szs2(i1,i2,i3)=(sz(i1,i2+1,i3)-sz(i1,i2-1,i3))*d12(1)
      szt2(i1,i2,i3)=(sz(i1,i2,i3+1)-sz(i1,i2,i3-1))*d12(2)
      szrr2(i1,i2,i3)=(-2.*sz(i1,i2,i3)+(sz(i1+1,i2,i3)+sz(i1-1,i2,i3))
     &  )*d22(0)
      szss2(i1,i2,i3)=(-2.*sz(i1,i2,i3)+(sz(i1,i2+1,i3)+sz(i1,i2-1,i3))
     &  )*d22(1)
      szrs2(i1,i2,i3)=(szr2(i1,i2+1,i3)-szr2(i1,i2-1,i3))*d12(1)
      txr2(i1,i2,i3)=(tx(i1+1,i2,i3)-tx(i1-1,i2,i3))*d12(0)
      txs2(i1,i2,i3)=(tx(i1,i2+1,i3)-tx(i1,i2-1,i3))*d12(1)
      txt2(i1,i2,i3)=(tx(i1,i2,i3+1)-tx(i1,i2,i3-1))*d12(2)
      txrr2(i1,i2,i3)=(-2.*tx(i1,i2,i3)+(tx(i1+1,i2,i3)+tx(i1-1,i2,i3))
     &  )*d22(0)
      txss2(i1,i2,i3)=(-2.*tx(i1,i2,i3)+(tx(i1,i2+1,i3)+tx(i1,i2-1,i3))
     &  )*d22(1)
      txrs2(i1,i2,i3)=(txr2(i1,i2+1,i3)-txr2(i1,i2-1,i3))*d12(1)
      tyr2(i1,i2,i3)=(ty(i1+1,i2,i3)-ty(i1-1,i2,i3))*d12(0)
      tys2(i1,i2,i3)=(ty(i1,i2+1,i3)-ty(i1,i2-1,i3))*d12(1)
      tyt2(i1,i2,i3)=(ty(i1,i2,i3+1)-ty(i1,i2,i3-1))*d12(2)
      tyrr2(i1,i2,i3)=(-2.*ty(i1,i2,i3)+(ty(i1+1,i2,i3)+ty(i1-1,i2,i3))
     &  )*d22(0)
      tyss2(i1,i2,i3)=(-2.*ty(i1,i2,i3)+(ty(i1,i2+1,i3)+ty(i1,i2-1,i3))
     &  )*d22(1)
      tyrs2(i1,i2,i3)=(tyr2(i1,i2+1,i3)-tyr2(i1,i2-1,i3))*d12(1)
      tzr2(i1,i2,i3)=(tz(i1+1,i2,i3)-tz(i1-1,i2,i3))*d12(0)
      tzs2(i1,i2,i3)=(tz(i1,i2+1,i3)-tz(i1,i2-1,i3))*d12(1)
      tzt2(i1,i2,i3)=(tz(i1,i2,i3+1)-tz(i1,i2,i3-1))*d12(2)
      tzrr2(i1,i2,i3)=(-2.*tz(i1,i2,i3)+(tz(i1+1,i2,i3)+tz(i1-1,i2,i3))
     &  )*d22(0)
      tzss2(i1,i2,i3)=(-2.*tz(i1,i2,i3)+(tz(i1,i2+1,i3)+tz(i1,i2-1,i3))
     &  )*d22(1)
      tzrs2(i1,i2,i3)=(tzr2(i1,i2+1,i3)-tzr2(i1,i2-1,i3))*d12(1)
      ux21(i1,i2,i3,kd)= rx(i1,i2,i3)*ur2(i1,i2,i3,kd)
      uy21(i1,i2,i3,kd)=0
      uz21(i1,i2,i3,kd)=0
      ux22(i1,i2,i3,kd)= rx(i1,i2,i3)*ur2(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & us2(i1,i2,i3,kd)
      uy22(i1,i2,i3,kd)= ry(i1,i2,i3)*ur2(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & us2(i1,i2,i3,kd)
      uz22(i1,i2,i3,kd)=0
      ux23(i1,i2,i3,kd)=rx(i1,i2,i3)*ur2(i1,i2,i3,kd)+sx(i1,i2,i3)*us2(
     & i1,i2,i3,kd)+tx(i1,i2,i3)*ut2(i1,i2,i3,kd)
      uy23(i1,i2,i3,kd)=ry(i1,i2,i3)*ur2(i1,i2,i3,kd)+sy(i1,i2,i3)*us2(
     & i1,i2,i3,kd)+ty(i1,i2,i3)*ut2(i1,i2,i3,kd)
      uz23(i1,i2,i3,kd)=rz(i1,i2,i3)*ur2(i1,i2,i3,kd)+sz(i1,i2,i3)*us2(
     & i1,i2,i3,kd)+tz(i1,i2,i3)*ut2(i1,i2,i3,kd)
      rxx21(i1,i2,i3)= rx(i1,i2,i3)*rxr2(i1,i2,i3)
      rxx22(i1,i2,i3)= rx(i1,i2,i3)*rxr2(i1,i2,i3)+sx(i1,i2,i3)*rxs2(
     & i1,i2,i3)
      rxy22(i1,i2,i3)= ry(i1,i2,i3)*rxr2(i1,i2,i3)+sy(i1,i2,i3)*rxs2(
     & i1,i2,i3)
      rxx23(i1,i2,i3)=rx(i1,i2,i3)*rxr2(i1,i2,i3)+sx(i1,i2,i3)*rxs2(i1,
     & i2,i3)+tx(i1,i2,i3)*rxt2(i1,i2,i3)
      rxy23(i1,i2,i3)=ry(i1,i2,i3)*rxr2(i1,i2,i3)+sy(i1,i2,i3)*rxs2(i1,
     & i2,i3)+ty(i1,i2,i3)*rxt2(i1,i2,i3)
      rxz23(i1,i2,i3)=rz(i1,i2,i3)*rxr2(i1,i2,i3)+sz(i1,i2,i3)*rxs2(i1,
     & i2,i3)+tz(i1,i2,i3)*rxt2(i1,i2,i3)
      ryx22(i1,i2,i3)= rx(i1,i2,i3)*ryr2(i1,i2,i3)+sx(i1,i2,i3)*rys2(
     & i1,i2,i3)
      ryy22(i1,i2,i3)= ry(i1,i2,i3)*ryr2(i1,i2,i3)+sy(i1,i2,i3)*rys2(
     & i1,i2,i3)
      ryx23(i1,i2,i3)=rx(i1,i2,i3)*ryr2(i1,i2,i3)+sx(i1,i2,i3)*rys2(i1,
     & i2,i3)+tx(i1,i2,i3)*ryt2(i1,i2,i3)
      ryy23(i1,i2,i3)=ry(i1,i2,i3)*ryr2(i1,i2,i3)+sy(i1,i2,i3)*rys2(i1,
     & i2,i3)+ty(i1,i2,i3)*ryt2(i1,i2,i3)
      ryz23(i1,i2,i3)=rz(i1,i2,i3)*ryr2(i1,i2,i3)+sz(i1,i2,i3)*rys2(i1,
     & i2,i3)+tz(i1,i2,i3)*ryt2(i1,i2,i3)
      rzx22(i1,i2,i3)= rx(i1,i2,i3)*rzr2(i1,i2,i3)+sx(i1,i2,i3)*rzs2(
     & i1,i2,i3)
      rzy22(i1,i2,i3)= ry(i1,i2,i3)*rzr2(i1,i2,i3)+sy(i1,i2,i3)*rzs2(
     & i1,i2,i3)
      rzx23(i1,i2,i3)=rx(i1,i2,i3)*rzr2(i1,i2,i3)+sx(i1,i2,i3)*rzs2(i1,
     & i2,i3)+tx(i1,i2,i3)*rzt2(i1,i2,i3)
      rzy23(i1,i2,i3)=ry(i1,i2,i3)*rzr2(i1,i2,i3)+sy(i1,i2,i3)*rzs2(i1,
     & i2,i3)+ty(i1,i2,i3)*rzt2(i1,i2,i3)
      rzz23(i1,i2,i3)=rz(i1,i2,i3)*rzr2(i1,i2,i3)+sz(i1,i2,i3)*rzs2(i1,
     & i2,i3)+tz(i1,i2,i3)*rzt2(i1,i2,i3)
      sxx22(i1,i2,i3)= rx(i1,i2,i3)*sxr2(i1,i2,i3)+sx(i1,i2,i3)*sxs2(
     & i1,i2,i3)
      sxy22(i1,i2,i3)= ry(i1,i2,i3)*sxr2(i1,i2,i3)+sy(i1,i2,i3)*sxs2(
     & i1,i2,i3)
      sxx23(i1,i2,i3)=rx(i1,i2,i3)*sxr2(i1,i2,i3)+sx(i1,i2,i3)*sxs2(i1,
     & i2,i3)+tx(i1,i2,i3)*sxt2(i1,i2,i3)
      sxy23(i1,i2,i3)=ry(i1,i2,i3)*sxr2(i1,i2,i3)+sy(i1,i2,i3)*sxs2(i1,
     & i2,i3)+ty(i1,i2,i3)*sxt2(i1,i2,i3)
      sxz23(i1,i2,i3)=rz(i1,i2,i3)*sxr2(i1,i2,i3)+sz(i1,i2,i3)*sxs2(i1,
     & i2,i3)+tz(i1,i2,i3)*sxt2(i1,i2,i3)
      syx22(i1,i2,i3)= rx(i1,i2,i3)*syr2(i1,i2,i3)+sx(i1,i2,i3)*sys2(
     & i1,i2,i3)
      syy22(i1,i2,i3)= ry(i1,i2,i3)*syr2(i1,i2,i3)+sy(i1,i2,i3)*sys2(
     & i1,i2,i3)
      syx23(i1,i2,i3)=rx(i1,i2,i3)*syr2(i1,i2,i3)+sx(i1,i2,i3)*sys2(i1,
     & i2,i3)+tx(i1,i2,i3)*syt2(i1,i2,i3)
      syy23(i1,i2,i3)=ry(i1,i2,i3)*syr2(i1,i2,i3)+sy(i1,i2,i3)*sys2(i1,
     & i2,i3)+ty(i1,i2,i3)*syt2(i1,i2,i3)
      syz23(i1,i2,i3)=rz(i1,i2,i3)*syr2(i1,i2,i3)+sz(i1,i2,i3)*sys2(i1,
     & i2,i3)+tz(i1,i2,i3)*syt2(i1,i2,i3)
      szx22(i1,i2,i3)= rx(i1,i2,i3)*szr2(i1,i2,i3)+sx(i1,i2,i3)*szs2(
     & i1,i2,i3)
      szy22(i1,i2,i3)= ry(i1,i2,i3)*szr2(i1,i2,i3)+sy(i1,i2,i3)*szs2(
     & i1,i2,i3)
      szx23(i1,i2,i3)=rx(i1,i2,i3)*szr2(i1,i2,i3)+sx(i1,i2,i3)*szs2(i1,
     & i2,i3)+tx(i1,i2,i3)*szt2(i1,i2,i3)
      szy23(i1,i2,i3)=ry(i1,i2,i3)*szr2(i1,i2,i3)+sy(i1,i2,i3)*szs2(i1,
     & i2,i3)+ty(i1,i2,i3)*szt2(i1,i2,i3)
      szz23(i1,i2,i3)=rz(i1,i2,i3)*szr2(i1,i2,i3)+sz(i1,i2,i3)*szs2(i1,
     & i2,i3)+tz(i1,i2,i3)*szt2(i1,i2,i3)
      txx22(i1,i2,i3)= rx(i1,i2,i3)*txr2(i1,i2,i3)+sx(i1,i2,i3)*txs2(
     & i1,i2,i3)
      txy22(i1,i2,i3)= ry(i1,i2,i3)*txr2(i1,i2,i3)+sy(i1,i2,i3)*txs2(
     & i1,i2,i3)
      txx23(i1,i2,i3)=rx(i1,i2,i3)*txr2(i1,i2,i3)+sx(i1,i2,i3)*txs2(i1,
     & i2,i3)+tx(i1,i2,i3)*txt2(i1,i2,i3)
      txy23(i1,i2,i3)=ry(i1,i2,i3)*txr2(i1,i2,i3)+sy(i1,i2,i3)*txs2(i1,
     & i2,i3)+ty(i1,i2,i3)*txt2(i1,i2,i3)
      txz23(i1,i2,i3)=rz(i1,i2,i3)*txr2(i1,i2,i3)+sz(i1,i2,i3)*txs2(i1,
     & i2,i3)+tz(i1,i2,i3)*txt2(i1,i2,i3)
      tyx22(i1,i2,i3)= rx(i1,i2,i3)*tyr2(i1,i2,i3)+sx(i1,i2,i3)*tys2(
     & i1,i2,i3)
      tyy22(i1,i2,i3)= ry(i1,i2,i3)*tyr2(i1,i2,i3)+sy(i1,i2,i3)*tys2(
     & i1,i2,i3)
      tyx23(i1,i2,i3)=rx(i1,i2,i3)*tyr2(i1,i2,i3)+sx(i1,i2,i3)*tys2(i1,
     & i2,i3)+tx(i1,i2,i3)*tyt2(i1,i2,i3)
      tyy23(i1,i2,i3)=ry(i1,i2,i3)*tyr2(i1,i2,i3)+sy(i1,i2,i3)*tys2(i1,
     & i2,i3)+ty(i1,i2,i3)*tyt2(i1,i2,i3)
      tyz23(i1,i2,i3)=rz(i1,i2,i3)*tyr2(i1,i2,i3)+sz(i1,i2,i3)*tys2(i1,
     & i2,i3)+tz(i1,i2,i3)*tyt2(i1,i2,i3)
      tzx22(i1,i2,i3)= rx(i1,i2,i3)*tzr2(i1,i2,i3)+sx(i1,i2,i3)*tzs2(
     & i1,i2,i3)
      tzy22(i1,i2,i3)= ry(i1,i2,i3)*tzr2(i1,i2,i3)+sy(i1,i2,i3)*tzs2(
     & i1,i2,i3)
      tzx23(i1,i2,i3)=rx(i1,i2,i3)*tzr2(i1,i2,i3)+sx(i1,i2,i3)*tzs2(i1,
     & i2,i3)+tx(i1,i2,i3)*tzt2(i1,i2,i3)
      tzy23(i1,i2,i3)=ry(i1,i2,i3)*tzr2(i1,i2,i3)+sy(i1,i2,i3)*tzs2(i1,
     & i2,i3)+ty(i1,i2,i3)*tzt2(i1,i2,i3)
      tzz23(i1,i2,i3)=rz(i1,i2,i3)*tzr2(i1,i2,i3)+sz(i1,i2,i3)*tzs2(i1,
     & i2,i3)+tz(i1,i2,i3)*tzt2(i1,i2,i3)
      uxx21(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*urr2(i1,i2,i3,kd)+(rxx22(i1,
     & i2,i3))*ur2(i1,i2,i3,kd)
      uyy21(i1,i2,i3,kd)=0
      uxy21(i1,i2,i3,kd)=0
      uxz21(i1,i2,i3,kd)=0
      uyz21(i1,i2,i3,kd)=0
      uzz21(i1,i2,i3,kd)=0
      ulaplacian21(i1,i2,i3,kd)=uxx21(i1,i2,i3,kd)
      uxx22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*urr2(i1,i2,i3,kd)+2.*(rx(i1,
     & i2,i3)*sx(i1,i2,i3))*urs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2)*uss2(
     & i1,i2,i3,kd)+(rxx22(i1,i2,i3))*ur2(i1,i2,i3,kd)+(sxx22(i1,i2,
     & i3))*us2(i1,i2,i3,kd)
      uyy22(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*urr2(i1,i2,i3,kd)+2.*(ry(i1,
     & i2,i3)*sy(i1,i2,i3))*urs2(i1,i2,i3,kd)+(sy(i1,i2,i3)**2)*uss2(
     & i1,i2,i3,kd)+(ryy22(i1,i2,i3))*ur2(i1,i2,i3,kd)+(syy22(i1,i2,
     & i3))*us2(i1,i2,i3,kd)
      uxy22(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*urr2(i1,i2,i3,kd)+(
     & rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*urs2(i1,
     & i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*uss2(i1,i2,i3,kd)+rxy22(i1,
     & i2,i3)*ur2(i1,i2,i3,kd)+sxy22(i1,i2,i3)*us2(i1,i2,i3,kd)
      uxz22(i1,i2,i3,kd)=0
      uyz22(i1,i2,i3,kd)=0
      uzz22(i1,i2,i3,kd)=0
      ulaplacian22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*urr2(
     & i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,
     & i2,i3))*urs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)**2)*
     & uss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3)+ryy22(i1,i2,i3))*ur2(i1,i2,
     & i3,kd)+(sxx22(i1,i2,i3)+syy22(i1,i2,i3))*us2(i1,i2,i3,kd)
      uxx23(i1,i2,i3,kd)=rx(i1,i2,i3)**2*urr2(i1,i2,i3,kd)+sx(i1,i2,i3)
     & **2*uss2(i1,i2,i3,kd)+tx(i1,i2,i3)**2*utt2(i1,i2,i3,kd)+2.*rx(
     & i1,i2,i3)*sx(i1,i2,i3)*urs2(i1,i2,i3,kd)+2.*rx(i1,i2,i3)*tx(i1,
     & i2,i3)*urt2(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,i2,i3)*ust2(i1,
     & i2,i3,kd)+rxx23(i1,i2,i3)*ur2(i1,i2,i3,kd)+sxx23(i1,i2,i3)*us2(
     & i1,i2,i3,kd)+txx23(i1,i2,i3)*ut2(i1,i2,i3,kd)
      uyy23(i1,i2,i3,kd)=ry(i1,i2,i3)**2*urr2(i1,i2,i3,kd)+sy(i1,i2,i3)
     & **2*uss2(i1,i2,i3,kd)+ty(i1,i2,i3)**2*utt2(i1,i2,i3,kd)+2.*ry(
     & i1,i2,i3)*sy(i1,i2,i3)*urs2(i1,i2,i3,kd)+2.*ry(i1,i2,i3)*ty(i1,
     & i2,i3)*urt2(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,i2,i3)*ust2(i1,
     & i2,i3,kd)+ryy23(i1,i2,i3)*ur2(i1,i2,i3,kd)+syy23(i1,i2,i3)*us2(
     & i1,i2,i3,kd)+tyy23(i1,i2,i3)*ut2(i1,i2,i3,kd)
      uzz23(i1,i2,i3,kd)=rz(i1,i2,i3)**2*urr2(i1,i2,i3,kd)+sz(i1,i2,i3)
     & **2*uss2(i1,i2,i3,kd)+tz(i1,i2,i3)**2*utt2(i1,i2,i3,kd)+2.*rz(
     & i1,i2,i3)*sz(i1,i2,i3)*urs2(i1,i2,i3,kd)+2.*rz(i1,i2,i3)*tz(i1,
     & i2,i3)*urt2(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,i2,i3)*ust2(i1,
     & i2,i3,kd)+rzz23(i1,i2,i3)*ur2(i1,i2,i3,kd)+szz23(i1,i2,i3)*us2(
     & i1,i2,i3,kd)+tzz23(i1,i2,i3)*ut2(i1,i2,i3,kd)
      uxy23(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*urr2(i1,i2,i3,kd)+
     & sx(i1,i2,i3)*sy(i1,i2,i3)*uss2(i1,i2,i3,kd)+tx(i1,i2,i3)*ty(i1,
     & i2,i3)*utt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,
     & i3)*sx(i1,i2,i3))*urs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(i1,i2,i3)+
     & ry(i1,i2,i3)*tx(i1,i2,i3))*urt2(i1,i2,i3,kd)+(sx(i1,i2,i3)*ty(
     & i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*ust2(i1,i2,i3,kd)+rxy23(
     & i1,i2,i3)*ur2(i1,i2,i3,kd)+sxy23(i1,i2,i3)*us2(i1,i2,i3,kd)+
     & txy23(i1,i2,i3)*ut2(i1,i2,i3,kd)
      uxz23(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*urr2(i1,i2,i3,kd)+
     & sx(i1,i2,i3)*sz(i1,i2,i3)*uss2(i1,i2,i3,kd)+tx(i1,i2,i3)*tz(i1,
     & i2,i3)*utt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,i2,
     & i3)*sx(i1,i2,i3))*urs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(i1,i2,i3)+
     & rz(i1,i2,i3)*tx(i1,i2,i3))*urt2(i1,i2,i3,kd)+(sx(i1,i2,i3)*tz(
     & i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*ust2(i1,i2,i3,kd)+rxz23(
     & i1,i2,i3)*ur2(i1,i2,i3,kd)+sxz23(i1,i2,i3)*us2(i1,i2,i3,kd)+
     & txz23(i1,i2,i3)*ut2(i1,i2,i3,kd)
      uyz23(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*urr2(i1,i2,i3,kd)+
     & sy(i1,i2,i3)*sz(i1,i2,i3)*uss2(i1,i2,i3,kd)+ty(i1,i2,i3)*tz(i1,
     & i2,i3)*utt2(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,i2,
     & i3)*sy(i1,i2,i3))*urs2(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(i1,i2,i3)+
     & rz(i1,i2,i3)*ty(i1,i2,i3))*urt2(i1,i2,i3,kd)+(sy(i1,i2,i3)*tz(
     & i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*ust2(i1,i2,i3,kd)+ryz23(
     & i1,i2,i3)*ur2(i1,i2,i3,kd)+syz23(i1,i2,i3)*us2(i1,i2,i3,kd)+
     & tyz23(i1,i2,i3)*ut2(i1,i2,i3,kd)
      ulaplacian23(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(i1,
     & i2,i3)**2)*urr2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)**2+
     & sz(i1,i2,i3)**2)*uss2(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+ty(i1,i2,
     & i3)**2+tz(i1,i2,i3)**2)*utt2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(
     & i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,i2,i3))
     & *urs2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(i1,i2,i3)*
     & ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*urt2(i1,i2,i3,kd)+2.*(
     & sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)+sz(i1,i2,
     & i3)*tz(i1,i2,i3))*ust2(i1,i2,i3,kd)+(rxx23(i1,i2,i3)+ryy23(i1,
     & i2,i3)+rzz23(i1,i2,i3))*ur2(i1,i2,i3,kd)+(sxx23(i1,i2,i3)+
     & syy23(i1,i2,i3)+szz23(i1,i2,i3))*us2(i1,i2,i3,kd)+(txx23(i1,i2,
     & i3)+tyy23(i1,i2,i3)+tzz23(i1,i2,i3))*ut2(i1,i2,i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      h12(kd) = 1./(2.*dx(kd))
      h22(kd) = 1./(dx(kd)**2)
      ux23r(i1,i2,i3,kd)=(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))*h12(0)
      uy23r(i1,i2,i3,kd)=(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))*h12(1)
      uz23r(i1,i2,i3,kd)=(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))*h12(2)
      uxx23r(i1,i2,i3,kd)=(-2.*u(i1,i2,i3,kd)+(u(i1+1,i2,i3,kd)+u(i1-1,
     & i2,i3,kd)) )*h22(0)
      uyy23r(i1,i2,i3,kd)=(-2.*u(i1,i2,i3,kd)+(u(i1,i2+1,i3,kd)+u(i1,
     & i2-1,i3,kd)) )*h22(1)
      uxy23r(i1,i2,i3,kd)=(ux23r(i1,i2+1,i3,kd)-ux23r(i1,i2-1,i3,kd))*
     & h12(1)
      uzz23r(i1,i2,i3,kd)=(-2.*u(i1,i2,i3,kd)+(u(i1,i2,i3+1,kd)+u(i1,
     & i2,i3-1,kd)) )*h22(2)
      uxz23r(i1,i2,i3,kd)=(ux23r(i1,i2,i3+1,kd)-ux23r(i1,i2,i3-1,kd))*
     & h12(2)
      uyz23r(i1,i2,i3,kd)=(uy23r(i1,i2,i3+1,kd)-uy23r(i1,i2,i3-1,kd))*
     & h12(2)
      ux21r(i1,i2,i3,kd)= ux23r(i1,i2,i3,kd)
      uy21r(i1,i2,i3,kd)= uy23r(i1,i2,i3,kd)
      uz21r(i1,i2,i3,kd)= uz23r(i1,i2,i3,kd)
      uxx21r(i1,i2,i3,kd)= uxx23r(i1,i2,i3,kd)
      uyy21r(i1,i2,i3,kd)= uyy23r(i1,i2,i3,kd)
      uzz21r(i1,i2,i3,kd)= uzz23r(i1,i2,i3,kd)
      uxy21r(i1,i2,i3,kd)= uxy23r(i1,i2,i3,kd)
      uxz21r(i1,i2,i3,kd)= uxz23r(i1,i2,i3,kd)
      uyz21r(i1,i2,i3,kd)= uyz23r(i1,i2,i3,kd)
      ulaplacian21r(i1,i2,i3,kd)=uxx23r(i1,i2,i3,kd)
      ux22r(i1,i2,i3,kd)= ux23r(i1,i2,i3,kd)
      uy22r(i1,i2,i3,kd)= uy23r(i1,i2,i3,kd)
      uz22r(i1,i2,i3,kd)= uz23r(i1,i2,i3,kd)
      uxx22r(i1,i2,i3,kd)= uxx23r(i1,i2,i3,kd)
      uyy22r(i1,i2,i3,kd)= uyy23r(i1,i2,i3,kd)
      uzz22r(i1,i2,i3,kd)= uzz23r(i1,i2,i3,kd)
      uxy22r(i1,i2,i3,kd)= uxy23r(i1,i2,i3,kd)
      uxz22r(i1,i2,i3,kd)= uxz23r(i1,i2,i3,kd)
      uyz22r(i1,i2,i3,kd)= uyz23r(i1,i2,i3,kd)
      ulaplacian22r(i1,i2,i3,kd)=uxx23r(i1,i2,i3,kd)+uyy23r(i1,i2,i3,
     & kd)
      ulaplacian23r(i1,i2,i3,kd)=uxx23r(i1,i2,i3,kd)+uyy23r(i1,i2,i3,
     & kd)+uzz23r(i1,i2,i3,kd)
      uxxx22r(i1,i2,i3,kd)=(-2.*(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))+(u(
     & i1+2,i2,i3,kd)-u(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
      uyyy22r(i1,i2,i3,kd)=(-2.*(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))+(u(
     & i1,i2+2,i3,kd)-u(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
      uxxy22r(i1,i2,i3,kd)=( uxx22r(i1,i2+1,i3,kd)-uxx22r(i1,i2-1,i3,
     & kd))/(2.*dx(1))
      uxyy22r(i1,i2,i3,kd)=( uyy22r(i1+1,i2,i3,kd)-uyy22r(i1-1,i2,i3,
     & kd))/(2.*dx(0))
      uxxxx22r(i1,i2,i3,kd)=(6.*u(i1,i2,i3,kd)-4.*(u(i1+1,i2,i3,kd)+u(
     & i1-1,i2,i3,kd))+(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd)) )/(dx(0)**
     & 4)
      uyyyy22r(i1,i2,i3,kd)=(6.*u(i1,i2,i3,kd)-4.*(u(i1,i2+1,i3,kd)+u(
     & i1,i2-1,i3,kd))+(u(i1,i2+2,i3,kd)+u(i1,i2-2,i3,kd)) )/(dx(1)**
     & 4)
      uxxyy22r(i1,i2,i3,kd)=( 4.*u(i1,i2,i3,kd)     -2.*(u(i1+1,i2,i3,
     & kd)+u(i1-1,i2,i3,kd)+u(i1,i2+1,i3,kd)+u(i1,i2-1,i3,kd))   +   (
     & u(i1+1,i2+1,i3,kd)+u(i1-1,i2+1,i3,kd)+u(i1+1,i2-1,i3,kd)+u(i1-
     & 1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      ! 2D laplacian squared = u.xxxx + 2 u.xxyy + u.yyyy
      uLapSq22r(i1,i2,i3,kd)= ( 6.*u(i1,i2,i3,kd)   - 4.*(u(i1+1,i2,i3,
     & kd)+u(i1-1,i2,i3,kd))    +(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd)) )
     & /(dx(0)**4) +( 6.*u(i1,i2,i3,kd)    -4.*(u(i1,i2+1,i3,kd)+u(i1,
     & i2-1,i3,kd))    +(u(i1,i2+2,i3,kd)+u(i1,i2-2,i3,kd)) )/(dx(1)**
     & 4)  +( 8.*u(i1,i2,i3,kd)     -4.*(u(i1+1,i2,i3,kd)+u(i1-1,i2,
     & i3,kd)+u(i1,i2+1,i3,kd)+u(i1,i2-1,i3,kd))   +2.*(u(i1+1,i2+1,
     & i3,kd)+u(i1-1,i2+1,i3,kd)+u(i1+1,i2-1,i3,kd)+u(i1-1,i2-1,i3,kd)
     & ) )/(dx(0)**2*dx(1)**2)
      uxxx23r(i1,i2,i3,kd)=(-2.*(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))+(u(
     & i1+2,i2,i3,kd)-u(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
      uyyy23r(i1,i2,i3,kd)=(-2.*(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))+(u(
     & i1,i2+2,i3,kd)-u(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
      uzzz23r(i1,i2,i3,kd)=(-2.*(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))+(u(
     & i1,i2,i3+2,kd)-u(i1,i2,i3-2,kd)) )*h22(1)*h12(2)
      uxxy23r(i1,i2,i3,kd)=( uxx22r(i1,i2+1,i3,kd)-uxx22r(i1,i2-1,i3,
     & kd))/(2.*dx(1))
      uxyy23r(i1,i2,i3,kd)=( uyy22r(i1+1,i2,i3,kd)-uyy22r(i1-1,i2,i3,
     & kd))/(2.*dx(0))
      uxxz23r(i1,i2,i3,kd)=( uxx22r(i1,i2,i3+1,kd)-uxx22r(i1,i2,i3-1,
     & kd))/(2.*dx(2))
      uyyz23r(i1,i2,i3,kd)=( uyy22r(i1,i2,i3+1,kd)-uyy22r(i1,i2,i3-1,
     & kd))/(2.*dx(2))
      uxzz23r(i1,i2,i3,kd)=( uzz22r(i1+1,i2,i3,kd)-uzz22r(i1-1,i2,i3,
     & kd))/(2.*dx(0))
      uyzz23r(i1,i2,i3,kd)=( uzz22r(i1,i2+1,i3,kd)-uzz22r(i1,i2-1,i3,
     & kd))/(2.*dx(1))
      uxxxx23r(i1,i2,i3,kd)=(6.*u(i1,i2,i3,kd)-4.*(u(i1+1,i2,i3,kd)+u(
     & i1-1,i2,i3,kd))+(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd)) )/(dx(0)**
     & 4)
      uyyyy23r(i1,i2,i3,kd)=(6.*u(i1,i2,i3,kd)-4.*(u(i1,i2+1,i3,kd)+u(
     & i1,i2-1,i3,kd))+(u(i1,i2+2,i3,kd)+u(i1,i2-2,i3,kd)) )/(dx(1)**
     & 4)
      uzzzz23r(i1,i2,i3,kd)=(6.*u(i1,i2,i3,kd)-4.*(u(i1,i2,i3+1,kd)+u(
     & i1,i2,i3-1,kd))+(u(i1,i2,i3+2,kd)+u(i1,i2,i3-2,kd)) )/(dx(2)**
     & 4)
      uxxyy23r(i1,i2,i3,kd)=( 4.*u(i1,i2,i3,kd)     -2.*(u(i1+1,i2,i3,
     & kd)+u(i1-1,i2,i3,kd)+u(i1,i2+1,i3,kd)+u(i1,i2-1,i3,kd))   +   (
     & u(i1+1,i2+1,i3,kd)+u(i1-1,i2+1,i3,kd)+u(i1+1,i2-1,i3,kd)+u(i1-
     & 1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      uxxzz23r(i1,i2,i3,kd)=( 4.*u(i1,i2,i3,kd)     -2.*(u(i1+1,i2,i3,
     & kd)+u(i1-1,i2,i3,kd)+u(i1,i2,i3+1,kd)+u(i1,i2,i3-1,kd))   +   (
     & u(i1+1,i2,i3+1,kd)+u(i1-1,i2,i3+1,kd)+u(i1+1,i2,i3-1,kd)+u(i1-
     & 1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)**2)
      uyyzz23r(i1,i2,i3,kd)=( 4.*u(i1,i2,i3,kd)     -2.*(u(i1,i2+1,i3,
     & kd)  +u(i1,i2-1,i3,kd)+  u(i1,i2  ,i3+1,kd)+u(i1,i2  ,i3-1,kd))
     &    +   (u(i1,i2+1,i3+1,kd)+u(i1,i2-1,i3+1,kd)+u(i1,i2+1,i3-1,
     & kd)+u(i1,i2-1,i3-1,kd)) )/(dx(1)**2*dx(2)**2)
      ! 3D laplacian squared = u.xxxx + u.yyyy + u.zzzz + 2 (u.xxyy + u.xxzz + u.yyzz )
      uLapSq23r(i1,i2,i3,kd)= ( 6.*u(i1,i2,i3,kd)   - 4.*(u(i1+1,i2,i3,
     & kd)+u(i1-1,i2,i3,kd))    +(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd)) )
     & /(dx(0)**4) +( 6.*u(i1,i2,i3,kd)    -4.*(u(i1,i2+1,i3,kd)+u(i1,
     & i2-1,i3,kd))    +(u(i1,i2+2,i3,kd)+u(i1,i2-2,i3,kd)) )/(dx(1)**
     & 4)  +( 6.*u(i1,i2,i3,kd)    -4.*(u(i1,i2,i3+1,kd)+u(i1,i2,i3-1,
     & kd))    +(u(i1,i2,i3+2,kd)+u(i1,i2,i3-2,kd)) )/(dx(2)**4)  +( 
     & 8.*u(i1,i2,i3,kd)     -4.*(u(i1+1,i2,i3,kd)  +u(i1-1,i2,i3,kd) 
     &  +u(i1  ,i2+1,i3,kd)+u(i1  ,i2-1,i3,kd))   +2.*(u(i1+1,i2+1,i3,
     & kd)+u(i1-1,i2+1,i3,kd)+u(i1+1,i2-1,i3,kd)+u(i1-1,i2-1,i3,kd)) )
     & /(dx(0)**2*dx(1)**2)+( 8.*u(i1,i2,i3,kd)     -4.*(u(i1+1,i2,i3,
     & kd)  +u(i1-1,i2,i3,kd)  +u(i1  ,i2,i3+1,kd)+u(i1  ,i2,i3-1,kd))
     &    +2.*(u(i1+1,i2,i3+1,kd)+u(i1-1,i2,i3+1,kd)+u(i1+1,i2,i3-1,
     & kd)+u(i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)**2)+( 8.*u(i1,i2,i3,
     & kd)     -4.*(u(i1,i2+1,i3,kd)  +u(i1,i2-1,i3,kd)  +u(i1,i2  ,
     & i3+1,kd)+u(i1,i2  ,i3-1,kd))   +2.*(u(i1,i2+1,i3+1,kd)+u(i1,i2-
     & 1,i3+1,kd)+u(i1,i2+1,i3-1,kd)+u(i1,i2-1,i3-1,kd)) )/(dx(1)**2*
     & dx(2)**2)
      u2r2(i1,i2,i3,kd)=(u2(i1+1,i2,i3,kd)-u2(i1-1,i2,i3,kd))*d12(0)
      u2s2(i1,i2,i3,kd)=(u2(i1,i2+1,i3,kd)-u2(i1,i2-1,i3,kd))*d12(1)
      u2t2(i1,i2,i3,kd)=(u2(i1,i2,i3+1,kd)-u2(i1,i2,i3-1,kd))*d12(2)
      u2rr2(i1,i2,i3,kd)=(-2.*u2(i1,i2,i3,kd)+(u2(i1+1,i2,i3,kd)+u2(i1-
     & 1,i2,i3,kd)) )*d22(0)
      u2ss2(i1,i2,i3,kd)=(-2.*u2(i1,i2,i3,kd)+(u2(i1,i2+1,i3,kd)+u2(i1,
     & i2-1,i3,kd)) )*d22(1)
      u2rs2(i1,i2,i3,kd)=(u2r2(i1,i2+1,i3,kd)-u2r2(i1,i2-1,i3,kd))*d12(
     & 1)
      u2tt2(i1,i2,i3,kd)=(-2.*u2(i1,i2,i3,kd)+(u2(i1,i2,i3+1,kd)+u2(i1,
     & i2,i3-1,kd)) )*d22(2)
      u2rt2(i1,i2,i3,kd)=(u2r2(i1,i2,i3+1,kd)-u2r2(i1,i2,i3-1,kd))*d12(
     & 2)
      u2st2(i1,i2,i3,kd)=(u2s2(i1,i2,i3+1,kd)-u2s2(i1,i2,i3-1,kd))*d12(
     & 2)
      u2rrr2(i1,i2,i3,kd)=(-2.*(u2(i1+1,i2,i3,kd)-u2(i1-1,i2,i3,kd))+(
     & u2(i1+2,i2,i3,kd)-u2(i1-2,i2,i3,kd)) )*d22(0)*d12(0)
      u2sss2(i1,i2,i3,kd)=(-2.*(u2(i1,i2+1,i3,kd)-u2(i1,i2-1,i3,kd))+(
     & u2(i1,i2+2,i3,kd)-u2(i1,i2-2,i3,kd)) )*d22(1)*d12(1)
      u2ttt2(i1,i2,i3,kd)=(-2.*(u2(i1,i2,i3+1,kd)-u2(i1,i2,i3-1,kd))+(
     & u2(i1,i2,i3+2,kd)-u2(i1,i2,i3-2,kd)) )*d22(2)*d12(2)
      u2x21(i1,i2,i3,kd)= rx(i1,i2,i3)*u2r2(i1,i2,i3,kd)
      u2y21(i1,i2,i3,kd)=0
      u2z21(i1,i2,i3,kd)=0
      u2x22(i1,i2,i3,kd)= rx(i1,i2,i3)*u2r2(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & u2s2(i1,i2,i3,kd)
      u2y22(i1,i2,i3,kd)= ry(i1,i2,i3)*u2r2(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & u2s2(i1,i2,i3,kd)
      u2z22(i1,i2,i3,kd)=0
      u2x23(i1,i2,i3,kd)=rx(i1,i2,i3)*u2r2(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & u2s2(i1,i2,i3,kd)+tx(i1,i2,i3)*u2t2(i1,i2,i3,kd)
      u2y23(i1,i2,i3,kd)=ry(i1,i2,i3)*u2r2(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & u2s2(i1,i2,i3,kd)+ty(i1,i2,i3)*u2t2(i1,i2,i3,kd)
      u2z23(i1,i2,i3,kd)=rz(i1,i2,i3)*u2r2(i1,i2,i3,kd)+sz(i1,i2,i3)*
     & u2s2(i1,i2,i3,kd)+tz(i1,i2,i3)*u2t2(i1,i2,i3,kd)
      u2xx21(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*u2rr2(i1,i2,i3,kd)+(rxx22(
     & i1,i2,i3))*u2r2(i1,i2,i3,kd)
      u2yy21(i1,i2,i3,kd)=0
      u2xy21(i1,i2,i3,kd)=0
      u2xz21(i1,i2,i3,kd)=0
      u2yz21(i1,i2,i3,kd)=0
      u2zz21(i1,i2,i3,kd)=0
      u2laplacian21(i1,i2,i3,kd)=u2xx21(i1,i2,i3,kd)
      u2xx22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*u2rr2(i1,i2,i3,kd)+2.*(rx(
     & i1,i2,i3)*sx(i1,i2,i3))*u2rs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2)*
     & u2ss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3))*u2r2(i1,i2,i3,kd)+(sxx22(
     & i1,i2,i3))*u2s2(i1,i2,i3,kd)
      u2yy22(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*u2rr2(i1,i2,i3,kd)+2.*(ry(
     & i1,i2,i3)*sy(i1,i2,i3))*u2rs2(i1,i2,i3,kd)+(sy(i1,i2,i3)**2)*
     & u2ss2(i1,i2,i3,kd)+(ryy22(i1,i2,i3))*u2r2(i1,i2,i3,kd)+(syy22(
     & i1,i2,i3))*u2s2(i1,i2,i3,kd)
      u2xy22(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*u2rr2(i1,i2,i3,kd)+
     & (rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*u2rs2(i1,
     & i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*u2ss2(i1,i2,i3,kd)+rxy22(
     & i1,i2,i3)*u2r2(i1,i2,i3,kd)+sxy22(i1,i2,i3)*u2s2(i1,i2,i3,kd)
      u2xz22(i1,i2,i3,kd)=0
      u2yz22(i1,i2,i3,kd)=0
      u2zz22(i1,i2,i3,kd)=0
      u2laplacian22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & u2rr2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*
     & sy(i1,i2,i3))*u2rs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)*
     & *2)*u2ss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3)+ryy22(i1,i2,i3))*u2r2(
     & i1,i2,i3,kd)+(sxx22(i1,i2,i3)+syy22(i1,i2,i3))*u2s2(i1,i2,i3,
     & kd)
      u2xx23(i1,i2,i3,kd)=rx(i1,i2,i3)**2*u2rr2(i1,i2,i3,kd)+sx(i1,i2,
     & i3)**2*u2ss2(i1,i2,i3,kd)+tx(i1,i2,i3)**2*u2tt2(i1,i2,i3,kd)+
     & 2.*rx(i1,i2,i3)*sx(i1,i2,i3)*u2rs2(i1,i2,i3,kd)+2.*rx(i1,i2,i3)
     & *tx(i1,i2,i3)*u2rt2(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,i2,i3)*
     & u2st2(i1,i2,i3,kd)+rxx23(i1,i2,i3)*u2r2(i1,i2,i3,kd)+sxx23(i1,
     & i2,i3)*u2s2(i1,i2,i3,kd)+txx23(i1,i2,i3)*u2t2(i1,i2,i3,kd)
      u2yy23(i1,i2,i3,kd)=ry(i1,i2,i3)**2*u2rr2(i1,i2,i3,kd)+sy(i1,i2,
     & i3)**2*u2ss2(i1,i2,i3,kd)+ty(i1,i2,i3)**2*u2tt2(i1,i2,i3,kd)+
     & 2.*ry(i1,i2,i3)*sy(i1,i2,i3)*u2rs2(i1,i2,i3,kd)+2.*ry(i1,i2,i3)
     & *ty(i1,i2,i3)*u2rt2(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,i2,i3)*
     & u2st2(i1,i2,i3,kd)+ryy23(i1,i2,i3)*u2r2(i1,i2,i3,kd)+syy23(i1,
     & i2,i3)*u2s2(i1,i2,i3,kd)+tyy23(i1,i2,i3)*u2t2(i1,i2,i3,kd)
      u2zz23(i1,i2,i3,kd)=rz(i1,i2,i3)**2*u2rr2(i1,i2,i3,kd)+sz(i1,i2,
     & i3)**2*u2ss2(i1,i2,i3,kd)+tz(i1,i2,i3)**2*u2tt2(i1,i2,i3,kd)+
     & 2.*rz(i1,i2,i3)*sz(i1,i2,i3)*u2rs2(i1,i2,i3,kd)+2.*rz(i1,i2,i3)
     & *tz(i1,i2,i3)*u2rt2(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,i2,i3)*
     & u2st2(i1,i2,i3,kd)+rzz23(i1,i2,i3)*u2r2(i1,i2,i3,kd)+szz23(i1,
     & i2,i3)*u2s2(i1,i2,i3,kd)+tzz23(i1,i2,i3)*u2t2(i1,i2,i3,kd)
      u2xy23(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*u2rr2(i1,i2,i3,kd)+
     & sx(i1,i2,i3)*sy(i1,i2,i3)*u2ss2(i1,i2,i3,kd)+tx(i1,i2,i3)*ty(
     & i1,i2,i3)*u2tt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,
     & i2,i3)*sx(i1,i2,i3))*u2rs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(i1,i2,
     & i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*u2rt2(i1,i2,i3,kd)+(sx(i1,i2,i3)
     & *ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*u2st2(i1,i2,i3,kd)+
     & rxy23(i1,i2,i3)*u2r2(i1,i2,i3,kd)+sxy23(i1,i2,i3)*u2s2(i1,i2,
     & i3,kd)+txy23(i1,i2,i3)*u2t2(i1,i2,i3,kd)
      u2xz23(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*u2rr2(i1,i2,i3,kd)+
     & sx(i1,i2,i3)*sz(i1,i2,i3)*u2ss2(i1,i2,i3,kd)+tx(i1,i2,i3)*tz(
     & i1,i2,i3)*u2tt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,
     & i2,i3)*sx(i1,i2,i3))*u2rs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(i1,i2,
     & i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*u2rt2(i1,i2,i3,kd)+(sx(i1,i2,i3)
     & *tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*u2st2(i1,i2,i3,kd)+
     & rxz23(i1,i2,i3)*u2r2(i1,i2,i3,kd)+sxz23(i1,i2,i3)*u2s2(i1,i2,
     & i3,kd)+txz23(i1,i2,i3)*u2t2(i1,i2,i3,kd)
      u2yz23(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*u2rr2(i1,i2,i3,kd)+
     & sy(i1,i2,i3)*sz(i1,i2,i3)*u2ss2(i1,i2,i3,kd)+ty(i1,i2,i3)*tz(
     & i1,i2,i3)*u2tt2(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,
     & i2,i3)*sy(i1,i2,i3))*u2rs2(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(i1,i2,
     & i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*u2rt2(i1,i2,i3,kd)+(sy(i1,i2,i3)
     & *tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*u2st2(i1,i2,i3,kd)+
     & ryz23(i1,i2,i3)*u2r2(i1,i2,i3,kd)+syz23(i1,i2,i3)*u2s2(i1,i2,
     & i3,kd)+tyz23(i1,i2,i3)*u2t2(i1,i2,i3,kd)
      u2laplacian23(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*u2rr2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)*
     & *2+sz(i1,i2,i3)**2)*u2ss2(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+ty(i1,
     & i2,i3)**2+tz(i1,i2,i3)**2)*u2tt2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*
     & sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,i2,
     & i3))*u2rs2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(i1,
     & i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*u2rt2(i1,i2,i3,
     & kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)+
     & sz(i1,i2,i3)*tz(i1,i2,i3))*u2st2(i1,i2,i3,kd)+(rxx23(i1,i2,i3)+
     & ryy23(i1,i2,i3)+rzz23(i1,i2,i3))*u2r2(i1,i2,i3,kd)+(sxx23(i1,
     & i2,i3)+syy23(i1,i2,i3)+szz23(i1,i2,i3))*u2s2(i1,i2,i3,kd)+(
     & txx23(i1,i2,i3)+tyy23(i1,i2,i3)+tzz23(i1,i2,i3))*u2t2(i1,i2,i3,
     & kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      u2x23r(i1,i2,i3,kd)=(u2(i1+1,i2,i3,kd)-u2(i1-1,i2,i3,kd))*h12(0)
      u2y23r(i1,i2,i3,kd)=(u2(i1,i2+1,i3,kd)-u2(i1,i2-1,i3,kd))*h12(1)
      u2z23r(i1,i2,i3,kd)=(u2(i1,i2,i3+1,kd)-u2(i1,i2,i3-1,kd))*h12(2)
      u2xx23r(i1,i2,i3,kd)=(-2.*u2(i1,i2,i3,kd)+(u2(i1+1,i2,i3,kd)+u2(
     & i1-1,i2,i3,kd)) )*h22(0)
      u2yy23r(i1,i2,i3,kd)=(-2.*u2(i1,i2,i3,kd)+(u2(i1,i2+1,i3,kd)+u2(
     & i1,i2-1,i3,kd)) )*h22(1)
      u2xy23r(i1,i2,i3,kd)=(u2x23r(i1,i2+1,i3,kd)-u2x23r(i1,i2-1,i3,kd)
     & )*h12(1)
      u2zz23r(i1,i2,i3,kd)=(-2.*u2(i1,i2,i3,kd)+(u2(i1,i2,i3+1,kd)+u2(
     & i1,i2,i3-1,kd)) )*h22(2)
      u2xz23r(i1,i2,i3,kd)=(u2x23r(i1,i2,i3+1,kd)-u2x23r(i1,i2,i3-1,kd)
     & )*h12(2)
      u2yz23r(i1,i2,i3,kd)=(u2y23r(i1,i2,i3+1,kd)-u2y23r(i1,i2,i3-1,kd)
     & )*h12(2)
      u2x21r(i1,i2,i3,kd)= u2x23r(i1,i2,i3,kd)
      u2y21r(i1,i2,i3,kd)= u2y23r(i1,i2,i3,kd)
      u2z21r(i1,i2,i3,kd)= u2z23r(i1,i2,i3,kd)
      u2xx21r(i1,i2,i3,kd)= u2xx23r(i1,i2,i3,kd)
      u2yy21r(i1,i2,i3,kd)= u2yy23r(i1,i2,i3,kd)
      u2zz21r(i1,i2,i3,kd)= u2zz23r(i1,i2,i3,kd)
      u2xy21r(i1,i2,i3,kd)= u2xy23r(i1,i2,i3,kd)
      u2xz21r(i1,i2,i3,kd)= u2xz23r(i1,i2,i3,kd)
      u2yz21r(i1,i2,i3,kd)= u2yz23r(i1,i2,i3,kd)
      u2laplacian21r(i1,i2,i3,kd)=u2xx23r(i1,i2,i3,kd)
      u2x22r(i1,i2,i3,kd)= u2x23r(i1,i2,i3,kd)
      u2y22r(i1,i2,i3,kd)= u2y23r(i1,i2,i3,kd)
      u2z22r(i1,i2,i3,kd)= u2z23r(i1,i2,i3,kd)
      u2xx22r(i1,i2,i3,kd)= u2xx23r(i1,i2,i3,kd)
      u2yy22r(i1,i2,i3,kd)= u2yy23r(i1,i2,i3,kd)
      u2zz22r(i1,i2,i3,kd)= u2zz23r(i1,i2,i3,kd)
      u2xy22r(i1,i2,i3,kd)= u2xy23r(i1,i2,i3,kd)
      u2xz22r(i1,i2,i3,kd)= u2xz23r(i1,i2,i3,kd)
      u2yz22r(i1,i2,i3,kd)= u2yz23r(i1,i2,i3,kd)
      u2laplacian22r(i1,i2,i3,kd)=u2xx23r(i1,i2,i3,kd)+u2yy23r(i1,i2,
     & i3,kd)
      u2laplacian23r(i1,i2,i3,kd)=u2xx23r(i1,i2,i3,kd)+u2yy23r(i1,i2,
     & i3,kd)+u2zz23r(i1,i2,i3,kd)
      u2xxx22r(i1,i2,i3,kd)=(-2.*(u2(i1+1,i2,i3,kd)-u2(i1-1,i2,i3,kd))+
     & (u2(i1+2,i2,i3,kd)-u2(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
      u2yyy22r(i1,i2,i3,kd)=(-2.*(u2(i1,i2+1,i3,kd)-u2(i1,i2-1,i3,kd))+
     & (u2(i1,i2+2,i3,kd)-u2(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
      u2xxy22r(i1,i2,i3,kd)=( u2xx22r(i1,i2+1,i3,kd)-u2xx22r(i1,i2-1,
     & i3,kd))/(2.*dx(1))
      u2xyy22r(i1,i2,i3,kd)=( u2yy22r(i1+1,i2,i3,kd)-u2yy22r(i1-1,i2,
     & i3,kd))/(2.*dx(0))
      u2xxxx22r(i1,i2,i3,kd)=(6.*u2(i1,i2,i3,kd)-4.*(u2(i1+1,i2,i3,kd)+
     & u2(i1-1,i2,i3,kd))+(u2(i1+2,i2,i3,kd)+u2(i1-2,i2,i3,kd)) )/(dx(
     & 0)**4)
      u2yyyy22r(i1,i2,i3,kd)=(6.*u2(i1,i2,i3,kd)-4.*(u2(i1,i2+1,i3,kd)+
     & u2(i1,i2-1,i3,kd))+(u2(i1,i2+2,i3,kd)+u2(i1,i2-2,i3,kd)) )/(dx(
     & 1)**4)
      u2xxyy22r(i1,i2,i3,kd)=( 4.*u2(i1,i2,i3,kd)     -2.*(u2(i1+1,i2,
     & i3,kd)+u2(i1-1,i2,i3,kd)+u2(i1,i2+1,i3,kd)+u2(i1,i2-1,i3,kd))  
     &  +   (u2(i1+1,i2+1,i3,kd)+u2(i1-1,i2+1,i3,kd)+u2(i1+1,i2-1,i3,
     & kd)+u2(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      ! 2D laplacian squared = u2.xxxx + 2 u2.xxyy + u2.yyyy
      u2LapSq22r(i1,i2,i3,kd)= ( 6.*u2(i1,i2,i3,kd)   - 4.*(u2(i1+1,i2,
     & i3,kd)+u2(i1-1,i2,i3,kd))    +(u2(i1+2,i2,i3,kd)+u2(i1-2,i2,i3,
     & kd)) )/(dx(0)**4) +( 6.*u2(i1,i2,i3,kd)    -4.*(u2(i1,i2+1,i3,
     & kd)+u2(i1,i2-1,i3,kd))    +(u2(i1,i2+2,i3,kd)+u2(i1,i2-2,i3,kd)
     & ) )/(dx(1)**4)  +( 8.*u2(i1,i2,i3,kd)     -4.*(u2(i1+1,i2,i3,
     & kd)+u2(i1-1,i2,i3,kd)+u2(i1,i2+1,i3,kd)+u2(i1,i2-1,i3,kd))   +
     & 2.*(u2(i1+1,i2+1,i3,kd)+u2(i1-1,i2+1,i3,kd)+u2(i1+1,i2-1,i3,kd)
     & +u2(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      u2xxx23r(i1,i2,i3,kd)=(-2.*(u2(i1+1,i2,i3,kd)-u2(i1-1,i2,i3,kd))+
     & (u2(i1+2,i2,i3,kd)-u2(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
      u2yyy23r(i1,i2,i3,kd)=(-2.*(u2(i1,i2+1,i3,kd)-u2(i1,i2-1,i3,kd))+
     & (u2(i1,i2+2,i3,kd)-u2(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
      u2zzz23r(i1,i2,i3,kd)=(-2.*(u2(i1,i2,i3+1,kd)-u2(i1,i2,i3-1,kd))+
     & (u2(i1,i2,i3+2,kd)-u2(i1,i2,i3-2,kd)) )*h22(1)*h12(2)
      u2xxy23r(i1,i2,i3,kd)=( u2xx22r(i1,i2+1,i3,kd)-u2xx22r(i1,i2-1,
     & i3,kd))/(2.*dx(1))
      u2xyy23r(i1,i2,i3,kd)=( u2yy22r(i1+1,i2,i3,kd)-u2yy22r(i1-1,i2,
     & i3,kd))/(2.*dx(0))
      u2xxz23r(i1,i2,i3,kd)=( u2xx22r(i1,i2,i3+1,kd)-u2xx22r(i1,i2,i3-
     & 1,kd))/(2.*dx(2))
      u2yyz23r(i1,i2,i3,kd)=( u2yy22r(i1,i2,i3+1,kd)-u2yy22r(i1,i2,i3-
     & 1,kd))/(2.*dx(2))
      u2xzz23r(i1,i2,i3,kd)=( u2zz22r(i1+1,i2,i3,kd)-u2zz22r(i1-1,i2,
     & i3,kd))/(2.*dx(0))
      u2yzz23r(i1,i2,i3,kd)=( u2zz22r(i1,i2+1,i3,kd)-u2zz22r(i1,i2-1,
     & i3,kd))/(2.*dx(1))
      u2xxxx23r(i1,i2,i3,kd)=(6.*u2(i1,i2,i3,kd)-4.*(u2(i1+1,i2,i3,kd)+
     & u2(i1-1,i2,i3,kd))+(u2(i1+2,i2,i3,kd)+u2(i1-2,i2,i3,kd)) )/(dx(
     & 0)**4)
      u2yyyy23r(i1,i2,i3,kd)=(6.*u2(i1,i2,i3,kd)-4.*(u2(i1,i2+1,i3,kd)+
     & u2(i1,i2-1,i3,kd))+(u2(i1,i2+2,i3,kd)+u2(i1,i2-2,i3,kd)) )/(dx(
     & 1)**4)
      u2zzzz23r(i1,i2,i3,kd)=(6.*u2(i1,i2,i3,kd)-4.*(u2(i1,i2,i3+1,kd)+
     & u2(i1,i2,i3-1,kd))+(u2(i1,i2,i3+2,kd)+u2(i1,i2,i3-2,kd)) )/(dx(
     & 2)**4)
      u2xxyy23r(i1,i2,i3,kd)=( 4.*u2(i1,i2,i3,kd)     -2.*(u2(i1+1,i2,
     & i3,kd)+u2(i1-1,i2,i3,kd)+u2(i1,i2+1,i3,kd)+u2(i1,i2-1,i3,kd))  
     &  +   (u2(i1+1,i2+1,i3,kd)+u2(i1-1,i2+1,i3,kd)+u2(i1+1,i2-1,i3,
     & kd)+u2(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      u2xxzz23r(i1,i2,i3,kd)=( 4.*u2(i1,i2,i3,kd)     -2.*(u2(i1+1,i2,
     & i3,kd)+u2(i1-1,i2,i3,kd)+u2(i1,i2,i3+1,kd)+u2(i1,i2,i3-1,kd))  
     &  +   (u2(i1+1,i2,i3+1,kd)+u2(i1-1,i2,i3+1,kd)+u2(i1+1,i2,i3-1,
     & kd)+u2(i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)**2)
      u2yyzz23r(i1,i2,i3,kd)=( 4.*u2(i1,i2,i3,kd)     -2.*(u2(i1,i2+1,
     & i3,kd)  +u2(i1,i2-1,i3,kd)+  u2(i1,i2  ,i3+1,kd)+u2(i1,i2  ,i3-
     & 1,kd))   +   (u2(i1,i2+1,i3+1,kd)+u2(i1,i2-1,i3+1,kd)+u2(i1,i2+
     & 1,i3-1,kd)+u2(i1,i2-1,i3-1,kd)) )/(dx(1)**2*dx(2)**2)
      ! 3D laplacian squared = u2.xxxx + u2.yyyy + u2.zzzz + 2 (u2.xxyy + u2.xxzz + u2.yyzz )
      u2LapSq23r(i1,i2,i3,kd)= ( 6.*u2(i1,i2,i3,kd)   - 4.*(u2(i1+1,i2,
     & i3,kd)+u2(i1-1,i2,i3,kd))    +(u2(i1+2,i2,i3,kd)+u2(i1-2,i2,i3,
     & kd)) )/(dx(0)**4) +( 6.*u2(i1,i2,i3,kd)    -4.*(u2(i1,i2+1,i3,
     & kd)+u2(i1,i2-1,i3,kd))    +(u2(i1,i2+2,i3,kd)+u2(i1,i2-2,i3,kd)
     & ) )/(dx(1)**4)  +( 6.*u2(i1,i2,i3,kd)    -4.*(u2(i1,i2,i3+1,kd)
     & +u2(i1,i2,i3-1,kd))    +(u2(i1,i2,i3+2,kd)+u2(i1,i2,i3-2,kd)) )
     & /(dx(2)**4)  +( 8.*u2(i1,i2,i3,kd)     -4.*(u2(i1+1,i2,i3,kd)  
     & +u2(i1-1,i2,i3,kd)  +u2(i1  ,i2+1,i3,kd)+u2(i1  ,i2-1,i3,kd))  
     &  +2.*(u2(i1+1,i2+1,i3,kd)+u2(i1-1,i2+1,i3,kd)+u2(i1+1,i2-1,i3,
     & kd)+u2(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)+( 8.*u2(i1,i2,
     & i3,kd)     -4.*(u2(i1+1,i2,i3,kd)  +u2(i1-1,i2,i3,kd)  +u2(i1  
     & ,i2,i3+1,kd)+u2(i1  ,i2,i3-1,kd))   +2.*(u2(i1+1,i2,i3+1,kd)+
     & u2(i1-1,i2,i3+1,kd)+u2(i1+1,i2,i3-1,kd)+u2(i1-1,i2,i3-1,kd)) )
     & /(dx(0)**2*dx(2)**2)+( 8.*u2(i1,i2,i3,kd)     -4.*(u2(i1,i2+1,
     & i3,kd)  +u2(i1,i2-1,i3,kd)  +u2(i1,i2  ,i3+1,kd)+u2(i1,i2  ,i3-
     & 1,kd))   +2.*(u2(i1,i2+1,i3+1,kd)+u2(i1,i2-1,i3+1,kd)+u2(i1,i2+
     & 1,i3-1,kd)+u2(i1,i2-1,i3-1,kd)) )/(dx(1)**2*dx(2)**2)
      gv2r2(i1,i2,i3,kd)=(gv2(i1+1,i2,i3,kd)-gv2(i1-1,i2,i3,kd))*d12(0)
      gv2s2(i1,i2,i3,kd)=(gv2(i1,i2+1,i3,kd)-gv2(i1,i2-1,i3,kd))*d12(1)
      gv2t2(i1,i2,i3,kd)=(gv2(i1,i2,i3+1,kd)-gv2(i1,i2,i3-1,kd))*d12(2)
      gv2rr2(i1,i2,i3,kd)=(-2.*gv2(i1,i2,i3,kd)+(gv2(i1+1,i2,i3,kd)+
     & gv2(i1-1,i2,i3,kd)) )*d22(0)
      gv2ss2(i1,i2,i3,kd)=(-2.*gv2(i1,i2,i3,kd)+(gv2(i1,i2+1,i3,kd)+
     & gv2(i1,i2-1,i3,kd)) )*d22(1)
      gv2rs2(i1,i2,i3,kd)=(gv2r2(i1,i2+1,i3,kd)-gv2r2(i1,i2-1,i3,kd))*
     & d12(1)
      gv2tt2(i1,i2,i3,kd)=(-2.*gv2(i1,i2,i3,kd)+(gv2(i1,i2,i3+1,kd)+
     & gv2(i1,i2,i3-1,kd)) )*d22(2)
      gv2rt2(i1,i2,i3,kd)=(gv2r2(i1,i2,i3+1,kd)-gv2r2(i1,i2,i3-1,kd))*
     & d12(2)
      gv2st2(i1,i2,i3,kd)=(gv2s2(i1,i2,i3+1,kd)-gv2s2(i1,i2,i3-1,kd))*
     & d12(2)
      gv2rrr2(i1,i2,i3,kd)=(-2.*(gv2(i1+1,i2,i3,kd)-gv2(i1-1,i2,i3,kd))
     & +(gv2(i1+2,i2,i3,kd)-gv2(i1-2,i2,i3,kd)) )*d22(0)*d12(0)
      gv2sss2(i1,i2,i3,kd)=(-2.*(gv2(i1,i2+1,i3,kd)-gv2(i1,i2-1,i3,kd))
     & +(gv2(i1,i2+2,i3,kd)-gv2(i1,i2-2,i3,kd)) )*d22(1)*d12(1)
      gv2ttt2(i1,i2,i3,kd)=(-2.*(gv2(i1,i2,i3+1,kd)-gv2(i1,i2,i3-1,kd))
     & +(gv2(i1,i2,i3+2,kd)-gv2(i1,i2,i3-2,kd)) )*d22(2)*d12(2)
      gv2x21(i1,i2,i3,kd)= rx(i1,i2,i3)*gv2r2(i1,i2,i3,kd)
      gv2y21(i1,i2,i3,kd)=0
      gv2z21(i1,i2,i3,kd)=0
      gv2x22(i1,i2,i3,kd)= rx(i1,i2,i3)*gv2r2(i1,i2,i3,kd)+sx(i1,i2,i3)
     & *gv2s2(i1,i2,i3,kd)
      gv2y22(i1,i2,i3,kd)= ry(i1,i2,i3)*gv2r2(i1,i2,i3,kd)+sy(i1,i2,i3)
     & *gv2s2(i1,i2,i3,kd)
      gv2z22(i1,i2,i3,kd)=0
      gv2x23(i1,i2,i3,kd)=rx(i1,i2,i3)*gv2r2(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & gv2s2(i1,i2,i3,kd)+tx(i1,i2,i3)*gv2t2(i1,i2,i3,kd)
      gv2y23(i1,i2,i3,kd)=ry(i1,i2,i3)*gv2r2(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & gv2s2(i1,i2,i3,kd)+ty(i1,i2,i3)*gv2t2(i1,i2,i3,kd)
      gv2z23(i1,i2,i3,kd)=rz(i1,i2,i3)*gv2r2(i1,i2,i3,kd)+sz(i1,i2,i3)*
     & gv2s2(i1,i2,i3,kd)+tz(i1,i2,i3)*gv2t2(i1,i2,i3,kd)
      gv2xx21(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*gv2rr2(i1,i2,i3,kd)+(
     & rxx22(i1,i2,i3))*gv2r2(i1,i2,i3,kd)
      gv2yy21(i1,i2,i3,kd)=0
      gv2xy21(i1,i2,i3,kd)=0
      gv2xz21(i1,i2,i3,kd)=0
      gv2yz21(i1,i2,i3,kd)=0
      gv2zz21(i1,i2,i3,kd)=0
      gv2laplacian21(i1,i2,i3,kd)=gv2xx21(i1,i2,i3,kd)
      gv2xx22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*gv2rr2(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*gv2rs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**
     & 2)*gv2ss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3))*gv2r2(i1,i2,i3,kd)+(
     & sxx22(i1,i2,i3))*gv2s2(i1,i2,i3,kd)
      gv2yy22(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*gv2rr2(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*gv2rs2(i1,i2,i3,kd)+(sy(i1,i2,i3)**
     & 2)*gv2ss2(i1,i2,i3,kd)+(ryy22(i1,i2,i3))*gv2r2(i1,i2,i3,kd)+(
     & syy22(i1,i2,i3))*gv2s2(i1,i2,i3,kd)
      gv2xy22(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*gv2rr2(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & gv2rs2(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*gv2ss2(i1,i2,i3,
     & kd)+rxy22(i1,i2,i3)*gv2r2(i1,i2,i3,kd)+sxy22(i1,i2,i3)*gv2s2(
     & i1,i2,i3,kd)
      gv2xz22(i1,i2,i3,kd)=0
      gv2yz22(i1,i2,i3,kd)=0
      gv2zz22(i1,i2,i3,kd)=0
      gv2laplacian22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & gv2rr2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)
     & *sy(i1,i2,i3))*gv2rs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,
     & i3)**2)*gv2ss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3)+ryy22(i1,i2,i3))*
     & gv2r2(i1,i2,i3,kd)+(sxx22(i1,i2,i3)+syy22(i1,i2,i3))*gv2s2(i1,
     & i2,i3,kd)
      gv2xx23(i1,i2,i3,kd)=rx(i1,i2,i3)**2*gv2rr2(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*gv2ss2(i1,i2,i3,kd)+tx(i1,i2,i3)**2*gv2tt2(i1,i2,i3,
     & kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*gv2rs2(i1,i2,i3,kd)+2.*rx(i1,
     & i2,i3)*tx(i1,i2,i3)*gv2rt2(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,
     & i2,i3)*gv2st2(i1,i2,i3,kd)+rxx23(i1,i2,i3)*gv2r2(i1,i2,i3,kd)+
     & sxx23(i1,i2,i3)*gv2s2(i1,i2,i3,kd)+txx23(i1,i2,i3)*gv2t2(i1,i2,
     & i3,kd)
      gv2yy23(i1,i2,i3,kd)=ry(i1,i2,i3)**2*gv2rr2(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*gv2ss2(i1,i2,i3,kd)+ty(i1,i2,i3)**2*gv2tt2(i1,i2,i3,
     & kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*gv2rs2(i1,i2,i3,kd)+2.*ry(i1,
     & i2,i3)*ty(i1,i2,i3)*gv2rt2(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,
     & i2,i3)*gv2st2(i1,i2,i3,kd)+ryy23(i1,i2,i3)*gv2r2(i1,i2,i3,kd)+
     & syy23(i1,i2,i3)*gv2s2(i1,i2,i3,kd)+tyy23(i1,i2,i3)*gv2t2(i1,i2,
     & i3,kd)
      gv2zz23(i1,i2,i3,kd)=rz(i1,i2,i3)**2*gv2rr2(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*gv2ss2(i1,i2,i3,kd)+tz(i1,i2,i3)**2*gv2tt2(i1,i2,i3,
     & kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*gv2rs2(i1,i2,i3,kd)+2.*rz(i1,
     & i2,i3)*tz(i1,i2,i3)*gv2rt2(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,
     & i2,i3)*gv2st2(i1,i2,i3,kd)+rzz23(i1,i2,i3)*gv2r2(i1,i2,i3,kd)+
     & szz23(i1,i2,i3)*gv2s2(i1,i2,i3,kd)+tzz23(i1,i2,i3)*gv2t2(i1,i2,
     & i3,kd)
      gv2xy23(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*gv2rr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*gv2ss2(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & ty(i1,i2,i3)*gv2tt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(
     & i1,i2,i3)*sx(i1,i2,i3))*gv2rs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(
     & i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*gv2rt2(i1,i2,i3,kd)+(sx(
     & i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*gv2st2(i1,i2,
     & i3,kd)+rxy23(i1,i2,i3)*gv2r2(i1,i2,i3,kd)+sxy23(i1,i2,i3)*
     & gv2s2(i1,i2,i3,kd)+txy23(i1,i2,i3)*gv2t2(i1,i2,i3,kd)
      gv2xz23(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*gv2rr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*gv2ss2(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & tz(i1,i2,i3)*gv2tt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sx(i1,i2,i3))*gv2rs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(
     & i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*gv2rt2(i1,i2,i3,kd)+(sx(
     & i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*gv2st2(i1,i2,
     & i3,kd)+rxz23(i1,i2,i3)*gv2r2(i1,i2,i3,kd)+sxz23(i1,i2,i3)*
     & gv2s2(i1,i2,i3,kd)+txz23(i1,i2,i3)*gv2t2(i1,i2,i3,kd)
      gv2yz23(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*gv2rr2(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*gv2ss2(i1,i2,i3,kd)+ty(i1,i2,i3)*
     & tz(i1,i2,i3)*gv2tt2(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sy(i1,i2,i3))*gv2rs2(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(
     & i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*gv2rt2(i1,i2,i3,kd)+(sy(
     & i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*gv2st2(i1,i2,
     & i3,kd)+ryz23(i1,i2,i3)*gv2r2(i1,i2,i3,kd)+syz23(i1,i2,i3)*
     & gv2s2(i1,i2,i3,kd)+tyz23(i1,i2,i3)*gv2t2(i1,i2,i3,kd)
      gv2laplacian23(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*gv2rr2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)
     & **2+sz(i1,i2,i3)**2)*gv2ss2(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+ty(
     & i1,i2,i3)**2+tz(i1,i2,i3)**2)*gv2tt2(i1,i2,i3,kd)+2.*(rx(i1,i2,
     & i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,
     & i2,i3))*gv2rs2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(
     & i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*gv2rt2(i1,i2,
     & i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)
     & +sz(i1,i2,i3)*tz(i1,i2,i3))*gv2st2(i1,i2,i3,kd)+(rxx23(i1,i2,
     & i3)+ryy23(i1,i2,i3)+rzz23(i1,i2,i3))*gv2r2(i1,i2,i3,kd)+(sxx23(
     & i1,i2,i3)+syy23(i1,i2,i3)+szz23(i1,i2,i3))*gv2s2(i1,i2,i3,kd)+(
     & txx23(i1,i2,i3)+tyy23(i1,i2,i3)+tzz23(i1,i2,i3))*gv2t2(i1,i2,
     & i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      gv2x23r(i1,i2,i3,kd)=(gv2(i1+1,i2,i3,kd)-gv2(i1-1,i2,i3,kd))*h12(
     & 0)
      gv2y23r(i1,i2,i3,kd)=(gv2(i1,i2+1,i3,kd)-gv2(i1,i2-1,i3,kd))*h12(
     & 1)
      gv2z23r(i1,i2,i3,kd)=(gv2(i1,i2,i3+1,kd)-gv2(i1,i2,i3-1,kd))*h12(
     & 2)
      gv2xx23r(i1,i2,i3,kd)=(-2.*gv2(i1,i2,i3,kd)+(gv2(i1+1,i2,i3,kd)+
     & gv2(i1-1,i2,i3,kd)) )*h22(0)
      gv2yy23r(i1,i2,i3,kd)=(-2.*gv2(i1,i2,i3,kd)+(gv2(i1,i2+1,i3,kd)+
     & gv2(i1,i2-1,i3,kd)) )*h22(1)
      gv2xy23r(i1,i2,i3,kd)=(gv2x23r(i1,i2+1,i3,kd)-gv2x23r(i1,i2-1,i3,
     & kd))*h12(1)
      gv2zz23r(i1,i2,i3,kd)=(-2.*gv2(i1,i2,i3,kd)+(gv2(i1,i2,i3+1,kd)+
     & gv2(i1,i2,i3-1,kd)) )*h22(2)
      gv2xz23r(i1,i2,i3,kd)=(gv2x23r(i1,i2,i3+1,kd)-gv2x23r(i1,i2,i3-1,
     & kd))*h12(2)
      gv2yz23r(i1,i2,i3,kd)=(gv2y23r(i1,i2,i3+1,kd)-gv2y23r(i1,i2,i3-1,
     & kd))*h12(2)
      gv2x21r(i1,i2,i3,kd)= gv2x23r(i1,i2,i3,kd)
      gv2y21r(i1,i2,i3,kd)= gv2y23r(i1,i2,i3,kd)
      gv2z21r(i1,i2,i3,kd)= gv2z23r(i1,i2,i3,kd)
      gv2xx21r(i1,i2,i3,kd)= gv2xx23r(i1,i2,i3,kd)
      gv2yy21r(i1,i2,i3,kd)= gv2yy23r(i1,i2,i3,kd)
      gv2zz21r(i1,i2,i3,kd)= gv2zz23r(i1,i2,i3,kd)
      gv2xy21r(i1,i2,i3,kd)= gv2xy23r(i1,i2,i3,kd)
      gv2xz21r(i1,i2,i3,kd)= gv2xz23r(i1,i2,i3,kd)
      gv2yz21r(i1,i2,i3,kd)= gv2yz23r(i1,i2,i3,kd)
      gv2laplacian21r(i1,i2,i3,kd)=gv2xx23r(i1,i2,i3,kd)
      gv2x22r(i1,i2,i3,kd)= gv2x23r(i1,i2,i3,kd)
      gv2y22r(i1,i2,i3,kd)= gv2y23r(i1,i2,i3,kd)
      gv2z22r(i1,i2,i3,kd)= gv2z23r(i1,i2,i3,kd)
      gv2xx22r(i1,i2,i3,kd)= gv2xx23r(i1,i2,i3,kd)
      gv2yy22r(i1,i2,i3,kd)= gv2yy23r(i1,i2,i3,kd)
      gv2zz22r(i1,i2,i3,kd)= gv2zz23r(i1,i2,i3,kd)
      gv2xy22r(i1,i2,i3,kd)= gv2xy23r(i1,i2,i3,kd)
      gv2xz22r(i1,i2,i3,kd)= gv2xz23r(i1,i2,i3,kd)
      gv2yz22r(i1,i2,i3,kd)= gv2yz23r(i1,i2,i3,kd)
      gv2laplacian22r(i1,i2,i3,kd)=gv2xx23r(i1,i2,i3,kd)+gv2yy23r(i1,
     & i2,i3,kd)
      gv2laplacian23r(i1,i2,i3,kd)=gv2xx23r(i1,i2,i3,kd)+gv2yy23r(i1,
     & i2,i3,kd)+gv2zz23r(i1,i2,i3,kd)
      gv2xxx22r(i1,i2,i3,kd)=(-2.*(gv2(i1+1,i2,i3,kd)-gv2(i1-1,i2,i3,
     & kd))+(gv2(i1+2,i2,i3,kd)-gv2(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
      gv2yyy22r(i1,i2,i3,kd)=(-2.*(gv2(i1,i2+1,i3,kd)-gv2(i1,i2-1,i3,
     & kd))+(gv2(i1,i2+2,i3,kd)-gv2(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
      gv2xxy22r(i1,i2,i3,kd)=( gv2xx22r(i1,i2+1,i3,kd)-gv2xx22r(i1,i2-
     & 1,i3,kd))/(2.*dx(1))
      gv2xyy22r(i1,i2,i3,kd)=( gv2yy22r(i1+1,i2,i3,kd)-gv2yy22r(i1-1,
     & i2,i3,kd))/(2.*dx(0))
      gv2xxxx22r(i1,i2,i3,kd)=(6.*gv2(i1,i2,i3,kd)-4.*(gv2(i1+1,i2,i3,
     & kd)+gv2(i1-1,i2,i3,kd))+(gv2(i1+2,i2,i3,kd)+gv2(i1-2,i2,i3,kd))
     &  )/(dx(0)**4)
      gv2yyyy22r(i1,i2,i3,kd)=(6.*gv2(i1,i2,i3,kd)-4.*(gv2(i1,i2+1,i3,
     & kd)+gv2(i1,i2-1,i3,kd))+(gv2(i1,i2+2,i3,kd)+gv2(i1,i2-2,i3,kd))
     &  )/(dx(1)**4)
      gv2xxyy22r(i1,i2,i3,kd)=( 4.*gv2(i1,i2,i3,kd)     -2.*(gv2(i1+1,
     & i2,i3,kd)+gv2(i1-1,i2,i3,kd)+gv2(i1,i2+1,i3,kd)+gv2(i1,i2-1,i3,
     & kd))   +   (gv2(i1+1,i2+1,i3,kd)+gv2(i1-1,i2+1,i3,kd)+gv2(i1+1,
     & i2-1,i3,kd)+gv2(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      ! 2D laplacian squared = gv2.xxxx + 2 gv2.xxyy + gv2.yyyy
      gv2LapSq22r(i1,i2,i3,kd)= ( 6.*gv2(i1,i2,i3,kd)   - 4.*(gv2(i1+1,
     & i2,i3,kd)+gv2(i1-1,i2,i3,kd))    +(gv2(i1+2,i2,i3,kd)+gv2(i1-2,
     & i2,i3,kd)) )/(dx(0)**4) +( 6.*gv2(i1,i2,i3,kd)    -4.*(gv2(i1,
     & i2+1,i3,kd)+gv2(i1,i2-1,i3,kd))    +(gv2(i1,i2+2,i3,kd)+gv2(i1,
     & i2-2,i3,kd)) )/(dx(1)**4)  +( 8.*gv2(i1,i2,i3,kd)     -4.*(gv2(
     & i1+1,i2,i3,kd)+gv2(i1-1,i2,i3,kd)+gv2(i1,i2+1,i3,kd)+gv2(i1,i2-
     & 1,i3,kd))   +2.*(gv2(i1+1,i2+1,i3,kd)+gv2(i1-1,i2+1,i3,kd)+gv2(
     & i1+1,i2-1,i3,kd)+gv2(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      gv2xxx23r(i1,i2,i3,kd)=(-2.*(gv2(i1+1,i2,i3,kd)-gv2(i1-1,i2,i3,
     & kd))+(gv2(i1+2,i2,i3,kd)-gv2(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
      gv2yyy23r(i1,i2,i3,kd)=(-2.*(gv2(i1,i2+1,i3,kd)-gv2(i1,i2-1,i3,
     & kd))+(gv2(i1,i2+2,i3,kd)-gv2(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
      gv2zzz23r(i1,i2,i3,kd)=(-2.*(gv2(i1,i2,i3+1,kd)-gv2(i1,i2,i3-1,
     & kd))+(gv2(i1,i2,i3+2,kd)-gv2(i1,i2,i3-2,kd)) )*h22(1)*h12(2)
      gv2xxy23r(i1,i2,i3,kd)=( gv2xx22r(i1,i2+1,i3,kd)-gv2xx22r(i1,i2-
     & 1,i3,kd))/(2.*dx(1))
      gv2xyy23r(i1,i2,i3,kd)=( gv2yy22r(i1+1,i2,i3,kd)-gv2yy22r(i1-1,
     & i2,i3,kd))/(2.*dx(0))
      gv2xxz23r(i1,i2,i3,kd)=( gv2xx22r(i1,i2,i3+1,kd)-gv2xx22r(i1,i2,
     & i3-1,kd))/(2.*dx(2))
      gv2yyz23r(i1,i2,i3,kd)=( gv2yy22r(i1,i2,i3+1,kd)-gv2yy22r(i1,i2,
     & i3-1,kd))/(2.*dx(2))
      gv2xzz23r(i1,i2,i3,kd)=( gv2zz22r(i1+1,i2,i3,kd)-gv2zz22r(i1-1,
     & i2,i3,kd))/(2.*dx(0))
      gv2yzz23r(i1,i2,i3,kd)=( gv2zz22r(i1,i2+1,i3,kd)-gv2zz22r(i1,i2-
     & 1,i3,kd))/(2.*dx(1))
      gv2xxxx23r(i1,i2,i3,kd)=(6.*gv2(i1,i2,i3,kd)-4.*(gv2(i1+1,i2,i3,
     & kd)+gv2(i1-1,i2,i3,kd))+(gv2(i1+2,i2,i3,kd)+gv2(i1-2,i2,i3,kd))
     &  )/(dx(0)**4)
      gv2yyyy23r(i1,i2,i3,kd)=(6.*gv2(i1,i2,i3,kd)-4.*(gv2(i1,i2+1,i3,
     & kd)+gv2(i1,i2-1,i3,kd))+(gv2(i1,i2+2,i3,kd)+gv2(i1,i2-2,i3,kd))
     &  )/(dx(1)**4)
      gv2zzzz23r(i1,i2,i3,kd)=(6.*gv2(i1,i2,i3,kd)-4.*(gv2(i1,i2,i3+1,
     & kd)+gv2(i1,i2,i3-1,kd))+(gv2(i1,i2,i3+2,kd)+gv2(i1,i2,i3-2,kd))
     &  )/(dx(2)**4)
      gv2xxyy23r(i1,i2,i3,kd)=( 4.*gv2(i1,i2,i3,kd)     -2.*(gv2(i1+1,
     & i2,i3,kd)+gv2(i1-1,i2,i3,kd)+gv2(i1,i2+1,i3,kd)+gv2(i1,i2-1,i3,
     & kd))   +   (gv2(i1+1,i2+1,i3,kd)+gv2(i1-1,i2+1,i3,kd)+gv2(i1+1,
     & i2-1,i3,kd)+gv2(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      gv2xxzz23r(i1,i2,i3,kd)=( 4.*gv2(i1,i2,i3,kd)     -2.*(gv2(i1+1,
     & i2,i3,kd)+gv2(i1-1,i2,i3,kd)+gv2(i1,i2,i3+1,kd)+gv2(i1,i2,i3-1,
     & kd))   +   (gv2(i1+1,i2,i3+1,kd)+gv2(i1-1,i2,i3+1,kd)+gv2(i1+1,
     & i2,i3-1,kd)+gv2(i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)**2)
      gv2yyzz23r(i1,i2,i3,kd)=( 4.*gv2(i1,i2,i3,kd)     -2.*(gv2(i1,i2+
     & 1,i3,kd)  +gv2(i1,i2-1,i3,kd)+  gv2(i1,i2  ,i3+1,kd)+gv2(i1,i2 
     &  ,i3-1,kd))   +   (gv2(i1,i2+1,i3+1,kd)+gv2(i1,i2-1,i3+1,kd)+
     & gv2(i1,i2+1,i3-1,kd)+gv2(i1,i2-1,i3-1,kd)) )/(dx(1)**2*dx(2)**
     & 2)
      ! 3D laplacian squared = gv2.xxxx + gv2.yyyy + gv2.zzzz + 2 (gv2.xxyy + gv2.xxzz + gv2.yyzz )
      gv2LapSq23r(i1,i2,i3,kd)= ( 6.*gv2(i1,i2,i3,kd)   - 4.*(gv2(i1+1,
     & i2,i3,kd)+gv2(i1-1,i2,i3,kd))    +(gv2(i1+2,i2,i3,kd)+gv2(i1-2,
     & i2,i3,kd)) )/(dx(0)**4) +( 6.*gv2(i1,i2,i3,kd)    -4.*(gv2(i1,
     & i2+1,i3,kd)+gv2(i1,i2-1,i3,kd))    +(gv2(i1,i2+2,i3,kd)+gv2(i1,
     & i2-2,i3,kd)) )/(dx(1)**4)  +( 6.*gv2(i1,i2,i3,kd)    -4.*(gv2(
     & i1,i2,i3+1,kd)+gv2(i1,i2,i3-1,kd))    +(gv2(i1,i2,i3+2,kd)+gv2(
     & i1,i2,i3-2,kd)) )/(dx(2)**4)  +( 8.*gv2(i1,i2,i3,kd)     -4.*(
     & gv2(i1+1,i2,i3,kd)  +gv2(i1-1,i2,i3,kd)  +gv2(i1  ,i2+1,i3,kd)+
     & gv2(i1  ,i2-1,i3,kd))   +2.*(gv2(i1+1,i2+1,i3,kd)+gv2(i1-1,i2+
     & 1,i3,kd)+gv2(i1+1,i2-1,i3,kd)+gv2(i1-1,i2-1,i3,kd)) )/(dx(0)**
     & 2*dx(1)**2)+( 8.*gv2(i1,i2,i3,kd)     -4.*(gv2(i1+1,i2,i3,kd)  
     & +gv2(i1-1,i2,i3,kd)  +gv2(i1  ,i2,i3+1,kd)+gv2(i1  ,i2,i3-1,kd)
     & )   +2.*(gv2(i1+1,i2,i3+1,kd)+gv2(i1-1,i2,i3+1,kd)+gv2(i1+1,i2,
     & i3-1,kd)+gv2(i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)**2)+( 8.*gv2(
     & i1,i2,i3,kd)     -4.*(gv2(i1,i2+1,i3,kd)  +gv2(i1,i2-1,i3,kd)  
     & +gv2(i1,i2  ,i3+1,kd)+gv2(i1,i2  ,i3-1,kd))   +2.*(gv2(i1,i2+1,
     & i3+1,kd)+gv2(i1,i2-1,i3+1,kd)+gv2(i1,i2+1,i3-1,kd)+gv2(i1,i2-1,
     & i3-1,kd)) )/(dx(1)**2*dx(2)**2)

      u2xr22(i1,i2,i3,kd)= rx(i1,i2,i3)*u2rr2(i1,i2,i3,kd)+sx(i1,i2,i3)
     & *
     & u2rs2(i1,i2,i3,kd)+rxr2(i1,i2,i3)*u2r2(i1,i2,i3,kd)+sxr2(i1,i2,
     & i3)*
     & u2s2(i1,i2,i3,kd)
      u2xs22(i1,i2,i3,kd)= rx(i1,i2,i3)*u2rs2(i1,i2,i3,kd)+sx(i1,i2,i3)
     & *
     & u2ss2(i1,i2,i3,kd)+rxs2(i1,i2,i3)*u2r2(i1,i2,i3,kd)+sxs2(i1,i2,
     & i3)*
     & u2s2(i1,i2,i3,kd)

      u2yr22(i1,i2,i3,kd)= ry(i1,i2,i3)*u2rr2(i1,i2,i3,kd)+sy(i1,i2,i3)
     & *
     & u2rs2(i1,i2,i3,kd)+ryr2(i1,i2,i3)*u2r2(i1,i2,i3,kd)+syr2(i1,i2,
     & i3)*
     & u2s2(i1,i2,i3,kd)
      u2ys22(i1,i2,i3,kd)= ry(i1,i2,i3)*u2rs2(i1,i2,i3,kd)+sy(i1,i2,i3)
     & *
     & u2ss2(i1,i2,i3,kd)+rys2(i1,i2,i3)*u2r2(i1,i2,i3,kd)+sys2(i1,i2,
     & i3)*
     & u2s2(i1,i2,i3,kd)

!     --- end statement functions

! .............. end statement functions


      ierr=0
      ! write(*,*) 'Inside cnsSlipWallBC'

      rc                =ipar(0)
      uc                =ipar(1)
      vc                =ipar(2)
      wc                =ipar(3)
      tc                =ipar(4)
      sc                =ipar(5)
      numberOfSpecies   =ipar(6)
      grid              =ipar(7)
      gridType          =ipar(8)
      orderOfAccuracy   =ipar(9)
      gridIsMoving      =ipar(10)
      useWhereMask      =ipar(11)
      isAxisymmetric    =ipar(12)
      twilightZone      =ipar(13)
      bcOption          =ipar(14)
      debug             =ipar(15)
      knownSolution     =ipar(16)
      numberOfComponents=ipar(17)
      radialAxis        =ipar(18)  ! =axis1 if x=0 is axis of cylindrical symmetry, =axis2 if y=0 is..
      axisymmetricWithSwirl=ipar(19)

      dx(0)             =rpar(0)
      dx(1)             =rpar(1)
      dx(2)             =rpar(2)
      dr(0)             =rpar(3)
      dr(1)             =rpar(4)
      dr(2)             =rpar(5)
      t                 =rpar(6)
      dt                =rpar(7)
      epsx              =rpar(8)
      gamma             =rpar(9)
      ep                =rpar(10) !  holds the pointer to the TZ function

      if( debug .gt. 8 )then
        write(*,'(" **** slipWallBC: bcOption=",i4)') bcOption
      end if

      gm1=gamma-1.
      ad2=10.  ! artificial dissipation

      urc = uc+radialAxis  ! radial velocity for isAxisymmetric
      uac = uc+radialAxis-1 ! axial velocity

      ! Look up parameters from the data base,  *new way*
       ok=getReal(pdb,'densityLowerBound',densityLowerBound)
       if( ok.eq.0 )then
         write(*,'("*** cnsSlipWallBC:ERROR: unable to find 
     & densityLowerBound")')
         stop 2233
       end if
       ok=getReal(pdb,'pressureLowerBound',pressureLowerBound)
       if( ok.eq.0 )then
         write(*,'("*** cnsSlipWallBC:ERROR: unable to find 
     & pressureLowerBound")')
         stop 2233
       end if

      ! write(*,'(" **** slipWallBC: grid,bcOption,gridIsMoving=",3i4)') grid,bcOption,gridIsMoving
      ! write(*,'("*** slipWallBC: densityLowerBound,pressureLowerBound=",2e10.2)') densityLowerBound,pressureLowerBound


      ! bcOption=slipWallSymmetry
      ! bcOption=slipWallPressureEntropySymmetry

!       i1=2
!       i2=2
!       i3=0
!       write(*,*) 'insbc4: x,y,u,err = ',x(i1,i2,i3,0),x(i1,i2,i3,1),ogf(exact,x(i1,i2,i3,0),x(i1,i2,i3,1),0.,uc,t),!                                     u(i1,i2,i3,uc)-ogf(exact,x(i1,i2,i3,0),x(i1,i2,i3,1),0.,uc,t)

!      if( gridIsMoving.ne.0 )then
!        stop 5
!      end if


      do axis=0,2
      do side=0,1
         nr(side,axis)=indexRange(side,axis)
      end do
      end do


      if( .false. .and. bcOption.eq.slipWallPressureEntropySymmetry 
     & .and. knownSolution.gt.0 )then
      do i3=nd3a,nd3b
        do i2=nd2a,nd2b
          do i1=nd1a,nd1b
            u2(i1,i2,i3,rc)=uKnown(i1,i2,i3,rc)
            u2(i1,i2,i3,uc)=uKnown(i1,i2,i3,uc)
            u2(i1,i2,i3,vc)=uKnown(i1,i2,i3,vc)
            u2(i1,i2,i3,tc)=uKnown(i1,i2,i3,tc)
          end do
        end do
      end do
      end if

      ! *wdh* 100918 if( nd.eq.2 .and. gridType.eq.rectangular .and. twilightZone.eq.0 )then
      if( nd.eq.2 .and. gridType.eq.rectangular )then

        ! *********************************************************************
        ! ******* 2D non-moving, rectangular **********************************
        ! *********************************************************************

        if( gridIsMoving.ne.0 )then
          write(*,'("cnsSlipWallBC:ERROR: gridIsMoving not implemented 
     & yet for rectangular")')
          ! '
          stop 6642
        end if

        do axis=0,nd-1
        do side=0,1
        if( bc(side,axis).eq.slipWall )then

          nr(0,axis)=indexRange(side,axis)   ! set nr to point to this side
          nr(1,axis)=indexRange(side,axis)

          unc = uc+axis                ! normal component is uc or vc
          utc = uc+mod(axis+1,2)       ! tangential component

          if( axis.eq.0 )then
            is1 = 1-2*side
            is2=0
          else
            is1=0
            is2=1-2*side
          end if
          ks1=2*is1
          ks2=2*is2
          if( twilightZone.eq.0 )then
            n1a=nr(0,0)
            n1b=nr(1,0)
            n2a=nr(0,1)
            n2b=nr(1,1)
            n3a=nr(0,2)
            n3b=nr(1,2)
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
            ! do as a separate loop:  u(i1,i2,i3,unc)=0.
            u(i1-is1,i2-is2,i3,unc)=2.*u(i1,i2,i3,unc)-u(i1+is1,i2+is2,
     & i3,unc)
            u(i1-is1,i2-is2,i3,utc)=u(i1+is1,i2+is2,i3,utc)

            u(i1-is1,i2-is2,i3,rc )=u(i1+is1,i2+is2,i3, rc)
            u(i1-is1,i2-is2,i3,tc )=u(i1+is1,i2+is2,i3, tc)

            ! --- 2nd ghost line: ----
            u(i1-ks1,i2-ks2,i3,unc)=2.*u(i1,i2,i3,unc)-u(i1+ks1,i2+ks2,
     & i3,unc)
            u(i1-ks1,i2-ks2,i3,utc)=u(i1+ks1,i2+ks2,i3,utc)

            u(i1-ks1,i2-ks2,i3,rc )=u(i1+ks1,i2+ks2,i3, rc)
            u(i1-ks1,i2-ks2,i3,tc )=u(i1+ks1,i2+ks2,i3, tc)
            end do
            end do
            end do
          else
           ! TZ: *wdh* 100918
            n1a=nr(0,0)
            n1b=nr(1,0)
            n2a=nr(0,1)
            n2b=nr(1,1)
            n3a=nr(0,2)
            n3b=nr(1,2)
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
            ! do as a separate loop:  u(i1,i2,i3,unc)=0.
            do m=rc,tc ! evaluate (rho,u,v,tc)
              uv(m) =ogf(ep,x(i1    ,i2    ,i3,0),x(i1    ,i2    ,i3,1)
     & ,0.,m,t)
              uvp(m)=ogf(ep,x(i1+is1,i2+is2,i3,0),x(i1+is1,i2+is2,i3,1)
     & ,0.,m,t)
              uvm(m)=ogf(ep,x(i1-is1,i2-is2,i3,0),x(i1-is1,i2-is2,i3,1)
     & ,0.,m,t)
            end do

    !  write(*,'("cnsSlip: i1,i2=",2i3," x,y=",2e10.2," rho,u,v,t=",4e10.2)') i1,i2,x(i1    ,i2    ,i3,0),x(i1    ,i2    ,i3,1),uv(rc),uv(uc),uv(vc),uv(tc)

            u(i1-is1,i2-is2,i3,unc)=2.*u(i1,i2,i3,unc)-u(i1+is1,i2+is2,
     & i3,unc) + uvm(unc)-2.*uv(unc)+uvp(unc)
            u(i1-is1,i2-is2,i3,utc)=u(i1+is1,i2+is2,i3,utc) + uvm(utc)-
     & uvp(utc)

            u(i1-is1,i2-is2,i3,rc )=u(i1+is1,i2+is2,i3, rc) + uvm(rc)-
     & uvp(rc)
            u(i1-is1,i2-is2,i3,tc )=u(i1+is1,i2+is2,i3, tc) + uvm(tc)-
     & uvp(tc)

            ! --- 2nd ghost line: ----
            do m=rc,tc ! evaluate (rho,u,v,tc)
              uv(m) =ogf(ep,x(i1    ,i2    ,i3,0),x(i1    ,i2    ,i3,1)
     & ,0.,m,t)
              uvp(m)=ogf(ep,x(i1+ks1,i2+ks2,i3,0),x(i1+ks1,i2+ks2,i3,1)
     & ,0.,m,t)
              uvm(m)=ogf(ep,x(i1-ks1,i2-ks2,i3,0),x(i1-ks1,i2-ks2,i3,1)
     & ,0.,m,t)
            end do
            u(i1-ks1,i2-ks2,i3,unc)=2.*u(i1,i2,i3,unc)-u(i1+ks1,i2+ks2,
     & i3,unc) + uvm(unc)-2.*uv(unc)+uvp(unc)
            u(i1-ks1,i2-ks2,i3,utc)=u(i1+ks1,i2+ks2,i3,utc) + uvm(utc)-
     & uvp(utc)

            u(i1-ks1,i2-ks2,i3,rc )=u(i1+ks1,i2+ks2,i3, rc) + uvm(rc)-
     & uvp(rc)
            u(i1-ks1,i2-ks2,i3,tc )=u(i1+ks1,i2+ks2,i3, tc) + uvm(tc)-
     & uvp(tc)
            end do
            end do
            end do
          end if

          nr(0,axis)=indexRange(0,axis)  ! reset
          nr(1,axis)=indexRange(1,axis)

        else if( bc(side,axis).eq.axisymmetric )then

          ! axisymmetric and rectangular

          nr(0,axis)=indexRange(side,axis)   ! set nr to point to this side
          nr(1,axis)=indexRange(side,axis)

          if( axis.eq.0 )then
            is1 = 1-2*side
            is2=0
          else
            is1=0
            is2=1-2*side
          end if
          ks1=2*is1
          ks2=2*is2

          if( twilightZone.eq.0 )then
            n1a=nr(0,0)
            n1b=nr(1,0)
            n2a=nr(0,1)
            n2b=nr(1,1)
            n3a=nr(0,2)
            n3b=nr(1,2)
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
            ! *wdh* 060323 u(i1,i2,i3,vc)=0.
            u(i1,i2,i3,urc)=0.
            u(i1-is1,i2-is2,i3,urc)=-u(i1+is1,i2+is2,i3,urc)
            u(i1-ks1,i2-ks2,i3,urc)=-u(i1+ks1,i2+ks2,i3,urc)

            u(i1-is1,i2-is2,i3,rc)= u(i1+is1,i2+is2,i3,rc)
            u(i1-ks1,i2-ks2,i3,rc)= u(i1+ks1,i2+ks2,i3,rc)

            u(i1-is1,i2-is2,i3,uac)= u(i1+is1,i2+is2,i3,uac)
            u(i1-ks1,i2-ks2,i3,uac)= u(i1+ks1,i2+ks2,i3,uac)

            u(i1-is1,i2-is2,i3,tc)= u(i1+is1,i2+is2,i3,tc)
            u(i1-ks1,i2-ks2,i3,tc)= u(i1+ks1,i2+ks2,i3,tc)

            end do
            end do
            end do
          else
           ! TZ *wdh* 100918
            n1a=nr(0,0)
            n1b=nr(1,0)
            n2a=nr(0,1)
            n2b=nr(1,1)
            n3a=nr(0,2)
            n3b=nr(1,2)
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b

            do m=rc,tc ! evaluate (rho,u,v,tc)
              uv(m) =ogf(ep,x(i1    ,i2    ,i3,0),x(i1    ,i2    ,i3,1)
     & ,0.,m,t)
              uvp(m)=ogf(ep,x(i1+is1,i2+is2,i3,0),x(i1+is1,i2+is2,i3,1)
     & ,0.,m,t)
              uvm(m)=ogf(ep,x(i1-is1,i2-is2,i3,0),x(i1-is1,i2-is2,i3,1)
     & ,0.,m,t)
            end do
            u(i1,i2,i3,urc)=uv(urc)

            u(i1-is1,i2-is2,i3,urc)=-u(i1+is1,i2+is2,i3,urc) + uvm(urc)
     & +uvp(urc)
            u(i1-is1,i2-is2,i3,uac)= u(i1+is1,i2+is2,i3,uac) + uvm(uac)
     & -uvp(uac)
            u(i1-is1,i2-is2,i3,rc) = u(i1+is1,i2+is2,i3,rc)  + uvm(rc)-
     & uvp(rc)
            u(i1-is1,i2-is2,i3,tc) = u(i1+is1,i2+is2,i3,tc)  + uvm(tc)-
     & uvp(tc)

            do m=rc,tc ! evaluate (rho,u,v,tc)
              uv(m) =ogf(ep,x(i1    ,i2    ,i3,0),x(i1    ,i2    ,i3,1)
     & ,0.,m,t)
              uvp(m)=ogf(ep,x(i1+ks1,i2+ks2,i3,0),x(i1+ks1,i2+ks2,i3,1)
     & ,0.,m,t)
              uvm(m)=ogf(ep,x(i1-ks1,i2-ks2,i3,0),x(i1-ks1,i2-ks2,i3,1)
     & ,0.,m,t)
            end do
            u(i1-ks1,i2-ks2,i3,urc)=-u(i1+ks1,i2+ks2,i3,urc) + uvm(urc)
     & +uvp(urc)
            u(i1-ks1,i2-ks2,i3,uac)= u(i1+ks1,i2+ks2,i3,uac) + uvm(uac)
     & -uvp(uac)
            u(i1-ks1,i2-ks2,i3,rc) = u(i1+ks1,i2+ks2,i3,rc)  + uvm(rc)-
     & uvp(rc)
            u(i1-ks1,i2-ks2,i3,tc) = u(i1+ks1,i2+ks2,i3,tc)  + uvm(tc)-
     & uvp(tc)

            end do
            end do
            end do
          end if

          if( axisymmetricWithSwirl.eq.1 )then
             n1a=nr(0,0)
             n1b=nr(1,0)
             n2a=nr(0,1)
             n2b=nr(1,1)
             n3a=nr(0,2)
             n3b=nr(1,2)
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
!     kkc 060117 actually extrapolate w since it's derivative may not be zero at the axis
              u(i1-is1,i2-is2,i3,wc)= 2d0*u(i1,i2,i3,wc)-u(i1+is1,i2+
     & is2,i3,wc)
              u(i1-ks1,i2-ks2,i3,wc)=u(i1-is1,i2-is2,i3,wc) ! second ghost line

!              u(i1-is1,i2-is2,i3,wc)= u(i1+is1,i2+is2,i3,wc)
!              u(i1-ks1,i2-ks2,i3,wc)= u(i1+ks1,i2+ks2,i3,wc)
             end do
             end do
             end do
          end if

          ! species
          do s=sc,sc+numberOfSpecies-1
           n1a=nr(0,0)
           n1b=nr(1,0)
           n2a=nr(0,1)
           n2b=nr(1,1)
           n3a=nr(0,2)
           n3b=nr(1,2)
           do i3=n3a,n3b
           do i2=n2a,n2b
           do i1=n1a,n1b
            u(i1-is1,i2-is2,i3,s)= u(i1+is1,i2+is2,i3,s)
            u(i1-ks1,i2-ks2,i3,s)= u(i1+ks1,i2+ks2,i3,s)
           end do
           end do
           end do
          end do

          nr(0,axis)=indexRange(0,axis)  ! reset
          nr(1,axis)=indexRange(1,axis)

        end if
        end do
        end do

      !else if( nd.eq.2 .and. gridType.eq.curvilinear !         .and. (twilightZone.eq.0 .or. bcOption.eq.slipWallTaylor .or. bcOption.eq.slipWallCharacteristic !                .or. bcOption.eq.slipWallPressureEntropySymmetry ) )then
      ! We do TZ here now too *wdh* 100919
      else if( nd.eq.2 .and. gridType.eq.curvilinear )then

        do axis=0,nd-1
        do side=0,1
        if( bc(side,axis).eq.slipWall )then

          nr(0,axis)=indexRange(side,axis)   ! set nr to point to this side
          nr(1,axis)=indexRange(side,axis)

          axisp1 = mod(axis+1,nd)
          if( axis.eq.0 )then
            is1 = 1-2*side
            is2=0
            js1=0
            js2=1
          else
            is1=0
            is2=1-2*side
            js1=1
            js2=0
          end if
          sgn=1-2*side
          ks1=2*is1
          ks2=2*is2

          ! **** apply symmetry conditions only ***
          if( bcOption.eq.slipWallSymmetry .and. gridIsMoving.eq.0 )
     & then
           if( twilightZone.eq.0 )then
              n1a=nr(0,0)
              n1b=nr(1,0)
              n2a=nr(0,1)
              n2b=nr(1,1)
              n3a=nr(0,2)
              n3b=nr(1,2)
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
              ! done previously: u(i1,i2,i3,unc)=0.

              ! normal component is odd, tangential component is even
              an1=-rsxy(i1,i2,i3,axis,0)*sgn  ! here is the normal  *wdh* changed sign 040811
              an2=-rsxy(i1,i2,i3,axis,1)*sgn
              aNorm=max(epsx,an1**2+an2**2)

              ! n.u(-1) = n.( 2*u(0)-u(+1) )
              ! t.u(-1) = t.u(+1) 
              ! first make both components even
              u(i1-is1,i2-is2,i3,uc)=u(i1+is1,i2+is2,i3,uc)
              u(i1-is1,i2-is2,i3,vc)=u(i1+is1,i2+is2,i3,vc)
              ! now fix-up the normal component
              nDotU=an1*(-u(i1-is1,i2-is2,i3,uc) + 2.*u(i1,i2,i3,uc)-u(
     & i1+is1,i2+is2,i3,uc) )+an2*(-u(i1-is1,i2-is2,i3,vc) + 2.*u(i1,
     & i2,i3,vc)-u(i1+is1,i2+is2,i3,vc) )

              u(i1-is1,i2-is2,i3,uc)=u(i1-is1,i2-is2,i3,uc)+ nDotU*
     & an1/aNorm
              u(i1-is1,i2-is2,i3,vc)=u(i1-is1,i2-is2,i3,vc)+ nDotU*
     & an2/aNorm

              u(i1-is1,i2-is2,i3,rc )=u(i1+is1,i2+is2,i3, rc)
              u(i1-is1,i2-is2,i3,tc )=u(i1+is1,i2+is2,i3, tc)

              ! ----- 2nd ghost line: ------
              ! first make both components even
              u(i1-ks1,i2-ks2,i3,uc)=u(i1+ks1,i2+ks2,i3,uc)
              u(i1-ks1,i2-ks2,i3,vc)=u(i1+ks1,i2+ks2,i3,vc)
              ! now fix-up the normal component
              nDotU=an1*(-u(i1-ks1,i2-ks2,i3,uc) + 2.*u(i1,i2,i3,uc)-u(
     & i1+ks1,i2+ks2,i3,uc) )+an2*(-u(i1-ks1,i2-ks2,i3,vc) + 2.*u(i1,
     & i2,i3,vc)-u(i1+ks1,i2+ks2,i3,vc) )

              u(i1-ks1,i2-ks2,i3,uc)=u(i1-ks1,i2-ks2,i3,uc)+ nDotU*
     & an1/aNorm
              u(i1-ks1,i2-ks2,i3,vc)=u(i1-ks1,i2-ks2,i3,vc)+ nDotU*
     & an2/aNorm

              u(i1-ks1,i2-ks2,i3,rc )=u(i1+ks1,i2+ks2,i3, rc)
              u(i1-ks1,i2-ks2,i3,tc )=u(i1+ks1,i2+ks2,i3, tc)

              end do
              end do
              end do
           else
             ! TZ 
              n1a=nr(0,0)
              n1b=nr(1,0)
              n2a=nr(0,1)
              n2b=nr(1,1)
              n3a=nr(0,2)
              n3b=nr(1,2)
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
              ! done previously: u(i1,i2,i3,unc)=0.

              ! normal component is odd, tangential component is even
              an1=-rsxy(i1,i2,i3,axis,0)*sgn  ! here is the normal  *wdh* changed sign 040811
              an2=-rsxy(i1,i2,i3,axis,1)*sgn
              aNorm=max(epsx,an1**2+an2**2)

              do m=rc,tc ! evaluate (rho,u,v,tc)
                uv(m) =ogf(ep,x(i1    ,i2    ,i3,0),x(i1    ,i2    ,i3,
     & 1),0.,m,t)
                uvp(m)=ogf(ep,x(i1+is1,i2+is2,i3,0),x(i1+is1,i2+is2,i3,
     & 1),0.,m,t)
                uvm(m)=ogf(ep,x(i1-is1,i2-is2,i3,0),x(i1-is1,i2-is2,i3,
     & 1),0.,m,t)
              end do

              ! n.u(-1) = n.( 2*u(0)-u(+1) + ue(-1) -2*ue(0)+ue(+1) )
              ! t.u(-1) = t.(u(+1) + ue(-1) - ue(+1) )
              ! first make both components even
              u(i1-is1,i2-is2,i3,uc)=u(i1+is1,i2+is2,i3,uc) +uvm(uc)-
     & uvp(uc)
              u(i1-is1,i2-is2,i3,vc)=u(i1+is1,i2+is2,i3,vc) +uvm(vc)-
     & uvp(vc)
              ! now fix-up the normal component
              nDotU=an1*(-u(i1-is1,i2-is2,i3,uc) + 2.*u(i1,i2,i3,uc)-u(
     & i1+is1,i2+is2,i3,uc) +uvm(uc)-2.*uv(uc)+uvp(uc))+an2*(-u(i1-
     & is1,i2-is2,i3,vc) + 2.*u(i1,i2,i3,vc)-u(i1+is1,i2+is2,i3,vc) +
     & uvm(vc)-2.*uv(vc)+uvp(vc))

              u(i1-is1,i2-is2,i3,uc)=u(i1-is1,i2-is2,i3,uc)+ nDotU*
     & an1/aNorm
              u(i1-is1,i2-is2,i3,vc)=u(i1-is1,i2-is2,i3,vc)+ nDotU*
     & an2/aNorm

              u(i1-is1,i2-is2,i3,rc )=u(i1+is1,i2+is2,i3, rc)  + uvm(
     & rc)-uvp(rc)
              u(i1-is1,i2-is2,i3,tc )=u(i1+is1,i2+is2,i3, tc)  + uvm(
     & tc)-uvp(tc)

              ! ----- 2nd ghost line: ------
              do m=rc,tc ! evaluate (rho,u,v,tc)
                uv(m) =ogf(ep,x(i1    ,i2    ,i3,0),x(i1    ,i2    ,i3,
     & 1),0.,m,t)
                uvp(m)=ogf(ep,x(i1+ks1,i2+ks2,i3,0),x(i1+ks1,i2+ks2,i3,
     & 1),0.,m,t)
                uvm(m)=ogf(ep,x(i1-ks1,i2-ks2,i3,0),x(i1-ks1,i2-ks2,i3,
     & 1),0.,m,t)
              end do
              ! first make both components even
              u(i1-ks1,i2-ks2,i3,uc)=u(i1+ks1,i2+ks2,i3,uc) +uvm(uc)-
     & uvp(uc)
              u(i1-ks1,i2-ks2,i3,vc)=u(i1+ks1,i2+ks2,i3,vc) +uvm(vc)-
     & uvp(vc)
              ! now fix-up the normal component
              nDotU=an1*(-u(i1-ks1,i2-ks2,i3,uc) + 2.*u(i1,i2,i3,uc)-u(
     & i1+ks1,i2+ks2,i3,uc) +uvm(uc)-2.*uv(uc)+uvp(uc))+an2*(-u(i1-
     & ks1,i2-ks2,i3,vc) + 2.*u(i1,i2,i3,vc)-u(i1+ks1,i2+ks2,i3,vc) +
     & uvm(vc)-2.*uv(vc)+uvp(vc))

              u(i1-ks1,i2-ks2,i3,uc)=u(i1-ks1,i2-ks2,i3,uc)+ nDotU*
     & an1/aNorm
              u(i1-ks1,i2-ks2,i3,vc)=u(i1-ks1,i2-ks2,i3,vc)+ nDotU*
     & an2/aNorm

              u(i1-ks1,i2-ks2,i3,rc )=u(i1+ks1,i2+ks2,i3, rc)  + uvm(
     & rc)-uvp(rc)
              u(i1-ks1,i2-ks2,i3,tc )=u(i1+ks1,i2+ks2,i3, tc)  + uvm(
     & tc)-uvp(tc)

              end do
              end do
              end do
           end if


          else if( bcOption.eq.slipWallPressureEntropySymmetry .and. 
     & gridIsMoving.eq.0 )then
           ! ******************************************************************
           ! *******************slipWallPressureEntropySymmetry****************
           ! ******************************************************************

           ! write(*,'(" cnsSlipWall: slipWallPressureEntropySymmetry used")')
           if( dt.lt.0. )then
             write(*,'(" ***cnsSlipWall:WARNING: dt<0 for t=",e12.3)') 
     & t
             dt=0.
           else
             write(*,'(" ***cnsSlipWall:INFO: t,dt=",2(e12.3,1x))') t,
     & dt
           end if


            n1a=nr(0,0)
            n1b=nr(1,0)
            n2a=nr(0,1)
            n2b=nr(1,1)
            n3a=nr(0,2)
            n3b=nr(1,2)
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
             if( mask(i1,i2,i3).gt.0 )then
            ! Apply 
            !    p.n = rho*( sx*u + sy*v)*( -(n1*us+n2*vs) )
            !        = rho*( sx*u + sy*v)*( n1s*u+n2s*v )
            !    rho.n = p.n/(g*rho^g-1)   (from p=c*rho^g)
            !    p = rho*T
            ! p.n = (n1*rx+n2*ry)pr + (n1*sx+n2*sy)*ps 
            ! normal component is odd, tangential component is even

            rxi = rsxy(i1,i2,i3,axis,0)
            ryi = rsxy(i1,i2,i3,axis,1)
            sxi = rsxy(i1,i2,i3,axisp1,0)
            syi = rsxy(i1,i2,i3,axisp1,1)

            an1=-rxi*sgn  ! here is the outward normal *wdh* changed sign 040811
            an2=-ryi*sgn
            aNorm=sqrt(max(epsx,an1**2+an2**2))
            an1=an1/aNorm
            an2=an2/aNorm

            ! first set the normal component to zero -- this may already have been done --
            nDotU=an1*u(i1,i2,i3,uc)+an2*u(i1,i2,i3,vc)
            u(i1,i2,i3,uc)=u(i1,i2,i3,uc)- nDotU*an1
            u(i1,i2,i3,vc)=u(i1,i2,i3,vc)- nDotU*an2


            ! first make both components even
            u(i1-is1,i2-is2,i3,uc)=u(i1+is1,i2+is2,i3,uc)
            u(i1-is1,i2-is2,i3,vc)=u(i1+is1,i2+is2,i3,vc)

            u(i1-is1,i2-is2,i3,uc)=3.*u(i1,i2,i3,uc)-3.*u(i1+is1,i2+
     & is2,i3,uc)+u(i1+2*is1,i2+2*is2,i3,uc)
            u(i1-is1,i2-is2,i3,vc)=3.*u(i1,i2,i3,vc)-3.*u(i1+is1,i2+
     & is2,i3,vc)+u(i1+2*is1,i2+2*is2,i3,vc)

            ! now fix-up the normal component
            nDotU=an1*(-u(i1-is1,i2-is2,i3,uc) + 2.*u(i1,i2,i3,uc)-u(
     & i1+is1,i2+is2,i3,uc) )+an2*(-u(i1-is1,i2-is2,i3,vc) + 2.*u(i1,
     & i2,i3,vc)-u(i1+is1,i2+is2,i3,vc) )

            u(i1-is1,i2-is2,i3,uc)=u(i1-is1,i2-is2,i3,uc)+ nDotU*an1
            u(i1-is1,i2-is2,i3,vc)=u(i1-is1,i2-is2,i3,vc)+ nDotU*an2


            ! Now update the density and temperature
            rho = u(i1,i2,i3,rc)
            tp  = u(i1,i2,i3,tc)
            u0s=(u(i1+js1,i2+js2,i3,uc)-u(i1-js1,i2-js2,i3,uc))/(2.*dr(
     & axisp1))
            v0s=(u(i1+js1,i2+js2,i3,vc)-u(i1-js1,i2-js2,i3,vc))/(2.*dr(
     & axisp1))

            pn = - rho*( sxi*u(i1,i2,i3,uc)+syi*u(i1,i2,i3,vc) )*(an1*
     & u0s+an2*v0s)

            nDotGradR=an1*rxi+an2*ryi
            nDotGradS=an1*sxi+an2*syi

            rhos=(u(i1+js1,i2+js2,i3,rc)-u(i1-js1,i2-js2,i3,rc))/(2.*
     & dr(axisp1))
            tps =(u(i1+js1,i2+js2,i3,tc)-u(i1-js1,i2-js2,i3,tc))/(2.*
     & dr(axisp1))

            ! OPTION I: 
            ! Here we assume the entropy is constant, p/rho^gamma = kappa
            !   => pn = (gamma*p/rho)*rhon = (gamma*T)*rhon
            rhon = pn/(gamma*tp)


            u(i1-is1,i2-is2,i3,rc)=u(i1+is1,i2+is2,i3,rc)-2.*sgn*dr(
     & axis)/nDotGradR*(rhon - nDotGradS*rhos )


            ! end OPTION I


            ! OPTION II:
            ! Here we advance the equation for rho.r (taking the r-derivative of the continuity equation)

            if( .true. )then
            ra = u2(i1,i2,i3,rc)     ! u2 = solution at time t-dt
            ua = u2(i1,i2,i3,uc)
            va = u2(i1,i2,i3,vc)

            rrsa=u2rs2(i1,i2,i3,rc)
            ursa=u2rs2(i1,i2,i3,uc)
            vrsa=u2rs2(i1,i2,i3,vc)

            rxa=rsxy(i1,i2,i3,0,0)
            sxa=rsxy(i1,i2,i3,1,0)
            rya=rsxy(i1,i2,i3,0,1)
            sya=rsxy(i1,i2,i3,1,1)

            if( axis.eq.0 )then
              ! solve for q=rho.r
              ! (rho.r)_t + (u*rx+v*ry)_r rho_r + (u*sx+v*sy)_r rho_s + (u*sx+v*sy) rho_rs +
              !           rho_r ( div(u) ) + rho*( div(u)_r ) = 0 
              ! div(u) = rx*ur+sx*us + ry*vr+sy*vs

              rra = u2r2(i1,i2,i3,rc)
              rsa = u2s2(i1,i2,i3,rc)

              ura = u2r2(i1,i2,i3,uc)
              vra = u2r2(i1,i2,i3,vc)
              usa = u2s2(i1,i2,i3,uc)
              vsa = u2s2(i1,i2,i3,vc)

              urra=u2rr2(i1,i2,i3,uc)
              vrra=u2rr2(i1,i2,i3,vc)

              rxra=rxr2(i1,i2,i3)
              sxra=sxr2(i1,i2,i3)
              ryra=ryr2(i1,i2,i3)
              syra=syr2(i1,i2,i3)

              fra = (ura*rxa+ua*rxra + vra*rya+va*ryra)*rra + (ura*sxa+
     & ua*sxra + vra*sya+va*syra)*rsa +(ua*sxa+va*sya)*rrsa + (rxa*
     & ura+sxa*usa + rya*vra+sya*vsa)*rra + (rxa*urra+sxa*ursa+rya*
     & vrra+sya*vrsa+ rxra*ura+sxra*usa+ryra*vra+syra*vsa)*ra

              rhor  = rra - dt*fra  ! forward Euler step
              ! we could do a predictor-corrector approach to get 2nd-order

              u(i1-is1,i2-is2,i3,rc)=u(i1+is1,i2+is2,i3,rc)-2.*sgn*dr(
     & axis)*rhor

            else
              ! boundary s=constant

              rra = u2r2(i1,i2,i3,rc)
              rsa = u2s2(i1,i2,i3,rc)

              ura = u2r2(i1,i2,i3,uc)
              vra = u2r2(i1,i2,i3,vc)
              usa = u2s2(i1,i2,i3,uc)
              vsa = u2s2(i1,i2,i3,vc)

              ussa=u2ss2(i1,i2,i3,uc)
              vssa=u2ss2(i1,i2,i3,vc)

              rxsa=rxs2(i1,i2,i3)
              sxsa=sxs2(i1,i2,i3)
              rysa=rys2(i1,i2,i3)
              sysa=sys2(i1,i2,i3)

              fra = (usa*rxa+ua*rxsa + vsa*rya+va*rysa)*rra + (usa*sxa+
     & ua*sxsa + vsa*sya+va*sysa)*rsa +(ua*rxa+va*rya)*rrsa + (rxa*
     & ura+sxa*usa + rya*vra+sya*vsa)*rsa + (rxa*ursa+sxa*ussa+rya*
     & vrsa+sya*vssa+ rxsa*ura+sxsa*usa+rysa*vra+sysa*vsa)*ra

              rhor  = rsa - dt*fra  ! this is really rho.s (forward Euler step)
              ! we could do a predictor-corrector approach to get 2nd-order

            if( .true. )then
              rhot = (ua*rxa+va*rya)*rra + (ua*sxa+va*sya)*rsa +(rxa*
     & ura+sxa*usa + rya*vra+sya*vsa)*ra
            write(1,'("--> i1,i2=",2i3," rho.t, rho.st=",2(e12.2,1x))')
     &  i1,i2,rhot,fra
            end if

            if( .false. )then
            write(*,'("--> i1,i2=",2i3," rho: ur,u2r=",2(f10.5,1x))') 
     & i1,i2,ur2(i1,i2,i3,rc),u2r2(i1,i2,i3,rc)
            write(*,'("  rho: u,u2,ra,ua,va,=",5(f10.5,1x))') u(i1,i2,
     & i3,rc),u2(i1,i2,i3,rc),ra,ua,va
            write(*,'("  dt,rra,rsa,rrsa,ura,vra=",6(f10.5,1x))') dt,
     & rra,rsa,rrsa,ura,vra
            write(*,'("  usa,vsa,urra,vrra,ursa,vrsa=",6(f10.5,1x))') 
     & usa,vsa,urra,vrra,ursa,vrsa
            write(*,'("  rxa,sxa,rya,sya=",5(f10.5,1x))') rxa,sxa,rya,
     & sya
            write(*,'("  rxra,sxra,ryra,syra=",5(f10.5,1x))') rxra,
     & sxra,ryra,syra

            write(*,'("  fra, rho.r(entropy), rhor=",3(f10.5,1x))') 
     & fra,us2(i1,i2,i3,rc),rhor
            write(*,'(" i1,i2=",2i3," rho.n(entropy), rho.n=",2(f10.5,
     & 1x))') i1,i2,rhon,nDotGradR*rhor + nDotGradS*rhos
            end if
            if( .false. )then

            write(*,'(" i1,i2=",2i3," rho.n(entropy), rho.n=",2(f10.5,
     & 1x))') i1,i2,rhon,nDotGradR*rhor + nDotGradS*rhos
            write(*,'("  fra, rho.r(entropy), rhor=",3(f10.5,1x))') 
     & fra,us2(i1,i2,i3,rc),rhor

            end if


             u(i1-is1,i2-is2,i3,rc)=u(i1+is1,i2+is2,i3,rc)-2.*sgn*dr(
     & axis)*rhor

            end if

             rhon = nDotGradR*rhor + nDotGradS*rhos  ! rho.n
            end if
            ! end OPTION II


            tpn = (pn-rhon*tp)/rho  ! from p=rho*T
            u(i1-is1,i2-is2,i3,tc)=u(i1+is1,i2+is2,i3,tc)-2.*sgn*dr(
     & axis)/nDotGradR*(tpn  - nDotGradS*tps )

            ! ----- 2nd ghost line: ------
            ! first make both components even
            u(i1-ks1,i2-ks2,i3,uc)=u(i1+ks1,i2+ks2,i3,uc)
            u(i1-ks1,i2-ks2,i3,vc)=u(i1+ks1,i2+ks2,i3,vc)

            ! ***
            u(i1-ks1,i2-ks2,i3,uc)=3.*u(i1-is1,i2-is2,i3,uc)-3.*u(i1,
     & i2,i3,uc)+u(i1+is1,i2+is2,i3,uc)
            u(i1-ks1,i2-ks2,i3,vc)=3.*u(i1-is1,i2-is2,i3,vc)-3.*u(i1,
     & i2,i3,vc)+u(i1+is1,i2+is2,i3,vc)

            ! now fix-up the normal component
            nDotU=an1*(-u(i1-ks1,i2-ks2,i3,uc) + 2.*u(i1,i2,i3,uc)-u(
     & i1+ks1,i2+ks2,i3,uc) )+an2*(-u(i1-ks1,i2-ks2,i3,vc) + 2.*u(i1,
     & i2,i3,vc)-u(i1+ks1,i2+ks2,i3,vc) )

            u(i1-ks1,i2-ks2,i3,uc)=u(i1-ks1,i2-ks2,i3,uc)+ nDotU*an1
            u(i1-ks1,i2-ks2,i3,vc)=u(i1-ks1,i2-ks2,i3,vc)+ nDotU*an2

            ! use same formula as above but dr -> 2*dr
            u(i1-ks1,i2-ks2,i3,rc)=u(i1+ks1,i2+ks2,i3,rc) -4.*sgn*dr(
     & axis)/nDotGradR*(rhon - nDotGradS*rhos )
            u(i1-ks1,i2-ks2,i3,tc)=u(i1+ks1,i2+ks2,i3,tc) -4.*sgn*dr(
     & axis)/nDotGradR*(tpn  - nDotGradS*tps )

            ! ***** for testing use the exact solution ***
            if( i1.gt.n1b-2  .and. 
     & knownSolution.eq.supersonicFlowInAnExpandingChannel )then
            u(i1,i2,i3,rc)=uKnown(i1,i2,i3,rc)
            u(i1,i2,i3,uc)=uKnown(i1,i2,i3,uc)
            u(i1,i2,i3,vc)=uKnown(i1,i2,i3,vc)
            u(i1,i2,i3,tc)=uKnown(i1,i2,i3,tc)

            ! write(1,*) ' i1,i2=',i1,i2,' rhoTrue=',uKnown(i1,i2,i3,rc)

            u(i1-is1,i2-is2,i3,rc)=uKnown(i1-is1,i2-is2,i3,rc)
            u(i1-is1,i2-is2,i3,uc)=uKnown(i1-is1,i2-is2,i3,uc)
            u(i1-is1,i2-is2,i3,vc)=uKnown(i1-is1,i2-is2,i3,vc)
            u(i1-is1,i2-is2,i3,tc)=uKnown(i1-is1,i2-is2,i3,tc)

            u(i1-ks1,i2-ks2,i3,rc)=uKnown(i1-ks1,i2-ks2,i3,rc)
            u(i1-ks1,i2-ks2,i3,uc)=uKnown(i1-ks1,i2-ks2,i3,uc)
            u(i1-ks1,i2-ks2,i3,vc)=uKnown(i1-ks1,i2-ks2,i3,vc)
            u(i1-ks1,i2-ks2,i3,tc)=uKnown(i1-ks1,i2-ks2,i3,tc)

            else

            ! write(1,*) ' i1,i2=',i1,i2,' rhoTrue=',uKnown(i1,i2,i3,rc)

            ! u(i1,i2,i3,rc)=uKnown(i1,i2,i3,rc)
            ! u(i1,i2,i3,uc)=uKnown(i1,i2,i3,uc)
            ! u(i1,i2,i3,vc)=uKnown(i1,i2,i3,vc)
            ! u(i1,i2,i3,tc)=uKnown(i1,i2,i3,tc)

            !u(i1-is1,i2-is2,i3,rc)=uKnown(i1-is1,i2-is2,i3,rc)
            !u(i1-is1,i2-is2,i3,uc)=uKnown(i1-is1,i2-is2,i3,uc)
            !u(i1-is1,i2-is2,i3,vc)=uKnown(i1-is1,i2-is2,i3,vc)
            !u(i1-is1,i2-is2,i3,tc)=uKnown(i1-is1,i2-is2,i3,tc)

            !u(i1-ks1,i2-ks2,i3,rc)=uKnown(i1-ks1,i2-ks2,i3,rc)
            !u(i1-ks1,i2-ks2,i3,uc)=uKnown(i1-ks1,i2-ks2,i3,uc)
            !u(i1-ks1,i2-ks2,i3,vc)=uKnown(i1-ks1,i2-ks2,i3,vc)
            !u(i1-ks1,i2-ks2,i3,tc)=uKnown(i1-ks1,i2-ks2,i3,tc)
            end if

          else ! mask(i1,i2,i3) <=0
            ! ---------------------------------------------------------------------------------
            ! set points outside of interp or unused points 
            ! ---------------------------------------------------------------------------------
            ! -- note that we need to set ghost points
            ! where mask(i1,i2,i3)=0 if we are next to an interpolation point (pts 1,3 below)
            !                      0  I  X   X  X   <- inside
            !                      0  I  X   X  X   <- boundary
            !                      1  2  g   g  g   <- ghost line 1
            !                      3  4  g   g  g   <- ghost line 2
            rxi = rsxy(i1,i2,i3,axis,0)
            ryi = rsxy(i1,i2,i3,axis,1)
            sxi = rsxy(i1,i2,i3,axisp1,0)
            syi = rsxy(i1,i2,i3,axisp1,1)

            an1=-rxi*sgn
            an2=-ryi*sgn
            aNorm=sqrt(max(epsx,an1**2+an2**2))
            an1=an1/aNorm
            an2=an2/aNorm

            do mm=1,2   ! assign values on two ghost lines
              j1=i1-is1*mm
              j2=i2-is2*mm
              j3=i3-is3*mm
              k1=i1+is1*mm
              k2=i2+is2*mm
              k3=i3+is3*mm

              u(j1,j2,j3,rc)=u(k1,k2,k3,rc)   ! apply symmetry, is this ok ?
              u(j1,j2,j3,tc)=u(k1,k2,k3,tc)

              u(j1,j2,j3,uc) =3.*u(j1+is1,j2+is2,j3,uc)-3.*u(j1+2*is1,
     & j2+2*is2,j3,uc)+u(j1+3*is1,j2+3*is2,j3,uc)
              u(j1,j2,j3,vc) =3.*u(j1+is1,j2+is2,j3,vc)-3.*u(j1+2*is1,
     & j2+2*is2,j3,vc)+u(j1+3*is1,j2+3*is2,j3,vc)

              ! extrap normal component of u 
              !   -- this extrpolation will be consistent with an odd symmetry condition (u.rr=0)
              nDotU1 = an1*( 2.*u(i1,i2,i3,uc)-u(k1,k2,k3,uc) ) + an2*(
     &  2.*u(i1,i2,i3,vc)-u(k1,k2,k3,vc) )

              ! set the normal component to be nDotU1
              nDotU = an1*u(j1,j2,j3,uc)+an2*u(j1,j2,j3,vc) - nDotU1
              u(j1,j2,j3,uc)=u(j1,j2,j3,uc)- nDotU*an1
              u(j1,j2,j3,vc)=u(j1,j2,j3,vc)- nDotU*an2

            end do
            end if
           end do
           end do
           end do


          else if( bcOption.eq.slipWallTaylor .and. gridIsMoving.eq.0 )
     & then
           ! ******************************************************************
           ! **********************slipWallTaylor  ****************************
           ! ******************************************************************

           write(*,'(" cnsSlipWall: slipWallTaylor used")')
           if( dt.lt.0. )then
             write(*,'(" ***cnsSlipWall:WARNING: dt<0 for t=",e12.3)') 
     & t
             dt=0.
           else
             write(*,'(" ***cnsSlipWall:INFO: t,dt=",2(e12.3,1x))') t,
     & dt
           end if

           ad2dt=ad2*dt
           tm=t-dt
           z0=0.
           do m=0,20
             fv(m)=0  ! forcing for TZ is by default zero
           end do
            n1a=nr(0,0)
            n1b=nr(1,0)
            n2a=nr(0,1)
            n2b=nr(1,1)
            n3a=nr(0,2)
            n3b=nr(1,2)
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
             if( mask(i1,i2,i3).gt.0 )then
            ! Apply 
            !    p.n = rho*( sx*u + sy*v)*( -(n1*us+n2*vs) )
            !        = rho*( sx*u + sy*v)*( n1s*u+n2s*v )
            !    p = rho*T
            ! p.n = (n1*rx+n2*ry)pr + (n1*sx+n2*sy)*ps 
            ! normal component is odd, tangential component is even

            rxi = rsxy(i1,i2,i3,axis,0)
            ryi = rsxy(i1,i2,i3,axis,1)
            sxi = rsxy(i1,i2,i3,axisp1,0)
            syi = rsxy(i1,i2,i3,axisp1,1)

            an1=-rxi*sgn  ! here is the outward normal *wdh* changed sign 040811
            an2=-ryi*sgn
            aNorm=sqrt(max(epsx,an1**2+an2**2))
            an1=an1/aNorm
            an2=an2/aNorm

            ! first set the normal component to zero -- this may already have been done --
            nDotU=an1*u(i1,i2,i3,uc)+an2*u(i1,i2,i3,vc)
            if( twilightZone.ne.0 )then
              do m=1,2 ! evaluate (u,v)
                uv(m)=ogf(ep,x(i1,i2,i3,0),x(i1,i2,i3,1),0.,m,t)
              end do
              nDotU=nDotU-(an1*uv(1)+an2*uv(2))
            end if
            u(i1,i2,i3,uc)=u(i1,i2,i3,uc)- nDotU*an1
            u(i1,i2,i3,vc)=u(i1,i2,i3,vc)- nDotU*an2


            ! Get rho, u,v at ghost points by Taylor series
            hx = x(i1-is1,i2-is2,i3,0)-x(i1,i2,i3,0)
            hy = x(i1-is1,i2-is2,i3,1)-x(i1,i2,i3,1)

            r0  = u2(i1,i2,i3,rc)
            rx0 = u2x22(i1,i2,i3,rc)
            ry0 = u2y22(i1,i2,i3,rc)
            rxx0= u2xx22(i1,i2,i3,rc)
            rxy0= u2xy22(i1,i2,i3,rc)
            ryy0= u2yy22(i1,i2,i3,rc)

            u0  = u2(i1,i2,i3,uc)
            ux0 = u2x22(i1,i2,i3,uc)
            uy0 = u2y22(i1,i2,i3,uc)
            uxx0= u2xx22(i1,i2,i3,uc)
            uxy0= u2xy22(i1,i2,i3,uc)
            uyy0= u2yy22(i1,i2,i3,uc)

            v0  = u2(i1,i2,i3,vc)
            vx0 = u2x22(i1,i2,i3,vc)
            vy0 = u2y22(i1,i2,i3,vc)
            vxx0= u2xx22(i1,i2,i3,vc)
            vxy0= u2xy22(i1,i2,i3,vc)
            vyy0= u2yy22(i1,i2,i3,vc)

            q0  = u2(i1,i2,i3,tc)
            qx0 = u2x22(i1,i2,i3,tc)
            qy0 = u2y22(i1,i2,i3,tc)
            qxx0= u2xx22(i1,i2,i3,tc)
            qxy0= u2xy22(i1,i2,i3,tc)
            qyy0= u2yy22(i1,i2,i3,tc)

            p0 = r0*q0                     ! Rg needed
            px0 =rx0*q0+r0*qx0
            py0 =ry0*q0+r0*qy0

            pxx0=rxx0*q0+rx0*qx0 + rx0*qx0+r0*qxx0
            pxy0=rxy0*q0+ry0*qx0 + rx0*qy0+r0*qxy0
            pyy0=ryy0*q0+ry0*qy0 + ry0*qy0+r0*qyy0

            if( twilightZone.ne.0 )then
              ! evaluate TZ forcing at tm=t-dt
              call ogftaylor(ep,x(i1,i2,i3,0),x(i1,i2,i3,1),z0,tm,nd,
     & fv)
            end if



            pt0 = -( u0*px0 + v0*py0 + gamma*p0*(ux0+vy0) -fv(4) )
            ptx0 =-( ux0*px0+vx0*py0 + gamma*px0*(ux0+vy0) + u0*pxx0+
     & v0*pxy0 + gamma*p0*(uxx0+vxy0) -fv(14) )
            pty0 =-( uy0*px0+vy0*py0 + gamma*py0*(ux0+vy0) + u0*pxy0+
     & v0*pyy0 + gamma*p0*(uxy0+vyy0) -fv(19) )

            qt0 = -( u0*qx0 + v0*qy0 + gm1*q0*(ux0+vy0) -fv(3) )
            qtx0 =-( ux0*qx0+vx0*qy0 + gm1*qx0*(ux0+vy0) + u0*qxx0+v0*
     & qxy0 + gm1*q0*(uxx0+vxy0) -fv(13) )
            qty0 =-( uy0*qx0+vy0*qy0 + gm1*qy0*(ux0+vy0) + u0*qxy0+v0*
     & qyy0 + gm1*q0*(uxy0+vyy0) -fv(18) )

            rt0 = -( u0*rx0+v0*ry0 + r0*(ux0+vy0) -fv(0) )
            rtx0= -( ux0*rx0 +rx0*ux0 +vx0*ry0 + rx0*vy0 + u0*rxx0+v0*
     & rxy0 + r0*(uxx0+vxy0) -fv(10) )
            rty0= -( uy0*rx0 +ry0*ux0 +vy0*ry0 + ry0*vy0 + u0*rxy0+v0*
     & ryy0 + r0*(uxy0+vyy0) -fv(15) )

            ut0 = -( u0*ux0 + v0*uy0 + px0/r0 -fv(1) )
            utx0= -( ux0*ux0 +u0*uxx0 + vx0*uy0 + v0*uxy0 + pxx0/r0 - 
     & px0*rx0/(r0**2) -fv(11) )
            uty0= -( uy0*ux0 +u0*uxy0 + vy0*uy0 + v0*uyy0 + pxy0/r0 - 
     & px0*ry0/(r0**2) -fv(16) )

            vt0 = -( u0*vx0 + v0*vy0 + py0/r0 -fv(2) )
            vtx0= -( ux0*vx0 +u0*vxx0 + vx0*vy0 + v0*vxy0 + pxy0/r0 - 
     & py0*rx0/(r0**2) -fv(12) )
            vty0= -( uy0*vx0 +u0*vxy0 + vy0*vy0 + v0*vyy0 + pyy0/r0 - 
     & py0*ry0/(r0**2) -fv(17) )

            rtt0= -( ut0*rx0+vt0*ry0 + rt0*(ux0+vy0) + u0*rtx0+v0*rty0 
     & + r0*(utx0+vty0) -fv(5) )

            utt0= -( ut0*ux0 + vt0*uy0 + ptx0/r0 + u0*utx0 + v0*uty0 - 
     & px0*rt0/(r0**2) -fv(6) )
            vtt0= -( ut0*vx0 + vt0*vy0 + pty0/r0 + u0*vtx0 + v0*vty0 - 
     & py0*rt0/(r0**2) -fv(7) )

            ptt0= -( ut0*px0 + vt0*py0 + gamma*pt0*(ux0+vy0) + u0*ptx0 
     & + v0*pty0 + gamma*p0*(utx0+vty0) -fv(9) )
            qtt0= -( ut0*qx0 + vt0*qy0 + gm1*qt0*(ux0+vy0) + u0*qtx0 + 
     & v0*qty0 + gm1*q0*(utx0+vty0) -fv(8) )

! this version uses tangential artificial dissipation

! here we use a centered formula: (u(-1)^{n+1} + u(1)^{n+1})/2 = u(0)^n + dt*ut(0)^n + .5*dt^2*utt(0)^n

! here we use taylor series in x but get ux and uxx from previous times

            ! taylorbc2(i1-is1,i2-is2,i3) 
            ! taylorbc2a(i1-is1,i2-is2,i3, i1+is1,i2+is2,i3) 
             u(i1-is1,i2-is2,i3,rc) = r0 + dt*rt0 + hx*rx0 + hy*ry0 +
     & .5*dt**2*rtt0 + dt*( hx*rtx0+hy*rty0 ) + .5*hx**2*rxx0 + hx*hy*
     & rxy0 + .5*hy**2*ryy0 + diss2(i1,i2,i3,rc)+ disst2(i1-is1,i2-
     & is2,i3,rc)
             u(i1-is1,i2-is2,i3,uc) = u0 + dt*ut0 + hx*ux0 + hy*uy0 +
     & .5*dt**2*utt0 + dt*( hx*utx0+hy*uty0 ) + .5*hx**2*uxx0 + hx*hy*
     & uxy0 + .5*hy**2*uyy0 + diss2(i1,i2,i3,uc)+ disst2(i1-is1,i2-
     & is2,i3,uc)
             u(i1-is1,i2-is2,i3,vc) = v0 + dt*vt0 + hx*vx0 + hy*vy0 +
     & .5*dt**2*vtt0 + dt*( hx*vtx0+hy*vty0 ) + .5*hx**2*vxx0 + hx*hy*
     & vxy0 + .5*hy**2*vyy0 + diss2(i1,i2,i3,vc)+ disst2(i1-is1,i2-
     & is2,i3,vc)
             u(i1-is1,i2-is2,i3,tc) = q0 + dt*qt0 + hx*qx0 + hy*qy0 +
     & .5*dt**2*qtt0 + dt*( hx*qtx0+hy*qty0 ) + .5*hx**2*qxx0 + hx*hy*
     & qxy0 + .5*hy**2*qyy0 + diss2(i1,i2,i3,tc)+ disst2(i1-is1,i2-
     & is2,i3,tc)


            hx = x(i1-2*is1,i2-2*is2,i3,0)-x(i1,i2,i3,0)
            hy = x(i1-2*is1,i2-2*is2,i3,1)-x(i1,i2,i3,1)

            ! taylorbc2(i1-2*is1,i2-2*is2,i3) 
            ! taylorbc2a(i1-2*is1,i2-2*is2,i3, i1+2*is1,i2+2*is2,i3) 
             u(i1-2*is1,i2-2*is2,i3,rc) = r0 + dt*rt0 + hx*rx0 + hy*
     & ry0 +.5*dt**2*rtt0 + dt*( hx*rtx0+hy*rty0 ) + .5*hx**2*rxx0 + 
     & hx*hy*rxy0 + .5*hy**2*ryy0 + diss2(i1,i2,i3,rc)+ disst2(i1-2*
     & is1,i2-2*is2,i3,rc)
             u(i1-2*is1,i2-2*is2,i3,uc) = u0 + dt*ut0 + hx*ux0 + hy*
     & uy0 +.5*dt**2*utt0 + dt*( hx*utx0+hy*uty0 ) + .5*hx**2*uxx0 + 
     & hx*hy*uxy0 + .5*hy**2*uyy0 + diss2(i1,i2,i3,uc)+ disst2(i1-2*
     & is1,i2-2*is2,i3,uc)
             u(i1-2*is1,i2-2*is2,i3,vc) = v0 + dt*vt0 + hx*vx0 + hy*
     & vy0 +.5*dt**2*vtt0 + dt*( hx*vtx0+hy*vty0 ) + .5*hx**2*vxx0 + 
     & hx*hy*vxy0 + .5*hy**2*vyy0 + diss2(i1,i2,i3,vc)+ disst2(i1-2*
     & is1,i2-2*is2,i3,vc)
             u(i1-2*is1,i2-2*is2,i3,tc) = q0 + dt*qt0 + hx*qx0 + hy*
     & qy0 +.5*dt**2*qtt0 + dt*( hx*qtx0+hy*qty0 ) + .5*hx**2*qxx0 + 
     & hx*hy*qxy0 + .5*hy**2*qyy0 + diss2(i1,i2,i3,tc)+ disst2(i1-2*
     & is1,i2-2*is2,i3,tc)

            ! u(i1-is1,i2-is2,i3,rc)=uKnown(i1-is1,i2-is2,i3,rc) ! *****************
            ! u(i1-is1,i2-is2,i3,uc)=uKnown(i1-is1,i2-is2,i3,uc) ! *****************
            ! u(i1-is1,i2-is2,i3,vc)=uKnown(i1-is1,i2-is2,i3,vc) ! *****************
            ! u(i1-is1,i2-is2,i3,tc)=uKnown(i1-is1,i2-is2,i3,tc) ! *****************


            ! BC: p.n = rho*( ... )
            rho = u(i1,i2,i3,rc)
            tp  = u(i1,i2,i3,tc)
            u0s=(u(i1+js1,i2+js2,i3,uc)-u(i1-js1,i2-js2,i3,uc))/(2.*dr(
     & axisp1))
            v0s=(u(i1+js1,i2+js2,i3,vc)-u(i1-js1,i2-js2,i3,vc))/(2.*dr(
     & axisp1))

            pn = -rho*( sxi*u(i1,i2,i3,uc)+syi*u(i1,i2,i3,vc) )*(an1*
     & u0s+an2*v0s)

            rhos=(u(i1+js1,i2+js2,i3,rc)-u(i1-js1,i2-js2,i3,rc))/(2.*
     & dr(axisp1))
            tps =(u(i1+js1,i2+js2,i3,tc)-u(i1-js1,i2-js2,i3,tc))/(2.*
     & dr(axisp1))
            ps = rhos*tp + rho*tps

            ! Here we assign T(-1)  from the equation for p.n
            ! ** u(i1-is1,i2-is2,i3,tc)=u(i1+is1,i2+is2,i3,tc)-sgn*2.*dr(axis)*( pn - an2*ps )/an1

            ! pp= u(i1+is1,i2+is2,i3,rc)*u(i1+is1,i2+is2,i3,tc)
            ! pm =pp  -sgn*2.*dr(axis)*( pn - (an1*sxi+an2*syi)*ps )/(an1*rxi+an2*ryi)
            ! tpm=pm/u(i1-is1,i2-is2,i3,rc)


            pr = ( pn - (an1*sxi+an2*syi)*ps )/(an1*rxi+an2*ryi)
            rhor = sgn*(u(i1+is1,i2+is2,i3,rc)-u(i1-is1,i2-is2,i3,rc))
     & /(2.*dr(axis))
            tpr = (pr-rhor*tp)/rho
            tpm = u(i1+is1,i2+is2,i3,tc) -sgn*2.*dr(axis)*tpr
            u(i1-is1,i2-is2,i3,tc)=tpm

            tpm = u(i1+2*is1,i2+2*is2,i3,tc) -sgn*4.*dr(axis)*tpr
            u(i1-2*is1,i2-2*is2,i3,tc)=tpm



            ! u(i1-is1,i2-is2,i3,rc)=pm/u(i1-is1,i2-is2,i3,tc)

!             j1=i1-2*is1
!             j2=i2-2*is2
!             j3=i3-2*is3
!             k1=i1+2*is1
!             k2=i2+2*is2
!             k3=i3+2*is3
! 
!             pp= u(k1,k2,k3,rc)*u(k1,k2,k3,tc)
!             pm =pp  -sgn*2.*dr(axis)*( pn - (an1*sxi+an2*syi)*ps )/(an1*rxi+an2*ryi)
!             tpm=pm/u(i1-is1,i2-is2,i3,rc)

!           u(i1-is1,i2-is2,i3,tc)=tpm

             if( .false.  .and. 
     & knownSolution.eq.supersonicFlowInAnExpandingChannel )then
              ! do we need to impose the extra BC on n.u(-1) ?? 
              ! --> first set n.u(ghost)
               do mm=1,2
                 j1=i1-is1*mm
                 j2=i2-is2*mm
                 j3=i3-is3*mm

                 nDotU=an1*u(j1,j2,j3,uc)+an2*u(j1,j2,j3,vc)
                 do m=1,2 ! evaluate (u,v)
                   ! uv(m)=ogf(ep,x(j1,j2,j3,0),x(j1,j2,j3,1),0.,m,t)
                   uv(m)=uKnown(j1,j2,i3,m)
                 end do
                 nDotU=nDotU-(an1*uv(1)+an2*uv(2))
                 u(j1,j2,j3,uc)=u(j1,j2,j3,uc)- nDotU*an1
                 u(j1,j2,j3,vc)=u(j1,j2,j3,vc)- nDotU*an2

                 if( i1.gt.nr(1,0)-4 )then
                  u(j1,j2,i3,rc)=uKnown(j1,j2,i3,rc)
                  u(j1,j2,i3,uc)=uKnown(j1,j2,i3,uc)
                  u(j1,j2,i3,vc)=uKnown(j1,j2,i3,vc)
                  u(j1,j2,i3,tc)=uKnown(j1,j2,i3,tc)
                end if


               end do
             end if


            if( .false. .and. knownSolution.gt.0 )then
              j1=i1-is1
              j2=i2-is2
              write(1,'(" taylor: j1,j2=",2i3," pm,p(-1),true=",3f7.4)
     & ') j1,j2,pm,u(j1,j2,i3,rc)*u(j1,j2,i3,tc),uKnown(j1,j2,i3,rc)*
     & uKnown(j1,j2,i3,tc)

              ! write(1,'(" taylor: j1,j2=",2i3," tpm,T(-1),true=",3f7.4," r(-1),true=",2f7.4)') !   j1,j2,tpm,u(j1,j2,i3,tc),uKnown(j1,j2,i3,tc),u(j1,j2,i3,rc),uKnown(j1,j2,i3,rc)
               ! '
            end if

            ! ***** for testing use the exact solution ***
            if( twilightZone.ne.0  )then

              j1=i1-is1
              j2=i2-is2
              do m=0,3
                uv(m)=ogf(ep,x(j1,j2,i3,0),x(j1,j2,i3,1),0.,m,t)
              end do


      if( .false. )then
      write(*,'(" taylor: j1,j2=",2i3," (r,u,v,T)=(",4f6.3,") true=(",
     & 4f6.3,")")') j1,j2,u(j1,j2,i3,rc),u(j1,j2,i3,uc),u(j1,j2,i3,vc)
     & ,u(j1,j2,i3,tc),uv(0),uv(1),uv(2),uv(3)
              ! '
      write(*,'("       : rt0,rtt0,rx0,ry0=",4e11.3," fv(0)=rt(TZ)=",
     & f7.4)') rt0,rtt0,rx0,ry0,fv(0)
      write(*,'("       : rxx0,rxy0,ryy0,rtx0,rty0=",5e11.3)') rxx0,
     & rxy0,ryy0,rtx0,rty0
      write(*,'("       : ut0,vt0,qt0,pt0,utt0,vtt0,qtt0,ptt0=",8e11.3)
     & ') ut0,vt0,qt0,pt0,utt0,vtt0,qtt0,ptt0
            ! '
      end if
              ! set T
              ! m=3
              ! u(i1-is1,i2-is2,i3,m)=ogf(ep,x(i1-is1,i2-is2,i3,0),x(i1-is1,i2-is2,i3,1),0.,m,t)

              if( .false. )then
              do m=0,3
                u(i1-is1,i2-is2,i3,m)=ogf(ep,x(i1-is1,i2-is2,i3,0),x(
     & i1-is1,i2-is2,i3,1),0.,m,t)
                u(i1-2*is1,i2-2*is2,i3,m)=ogf(ep,x(i1-2*is1,i2-2*is2,
     & i3,0),x(i1-2*is1,i2-2*is2,i3,1),0.,m,t)
              end do
              end if

            else if( .false. .and. knownSolution.gt.0  ) then
               j1=i1-is1
               j2=i2-is2

      write(*,'(" taylor: j1,j2=",2i3," (r,u,v,T)=(",4f6.3,") true=(",
     & 4f6.3,")")') j1,j2,u(j1,j2,i3,rc),u(j1,j2,i3,uc),u(j1,j2,i3,vc)
     & ,u(j1,j2,i3,tc),uKnown(j1,j2,i3,rc),uKnown(j1,j2,i3,uc),uKnown(
     & j1,j2,i3,vc),uKnown(j1,j2,i3,tc)
               ! '
      write(*,'("   taylor: (r,u,v,T,p).x  =(",5f6.2,")")') rx0,ux0,
     & vx0,qx0,px0
      write(*,'("   taylor: (r,u,v,T,p).y  =(",5f6.2,")")') ry0,uy0,
     & vy0,qy0,py0
      write(*,'("   taylor: (r,u,v,T,p).t  =(",5f6.2,")")') rt0,ut0,
     & vt0,qt0,pt0
      write(*,'("   taylor: (r,u,v,T,p).xx =(",5f6.2,")")') rxx0,uxx0,
     & vxx0,qxx0,pxx0
      write(*,'("   taylor: (r,u,v,T,p).xy =(",5f6.2,")")') rxy0,uxy0,
     & vxy0,qxy0,pxy0
      write(*,'("   taylor: (r,u,v,T,p).yy =(",5f6.2,")")') ryy0,uyy0,
     & vyy0,qyy0,pyy0
      write(*,'("   taylor: (r,u,v,T,p).tt =(",5f6.2,")")') rtt0,utt0,
     & vtt0,qtt0,ptt0
      write(*,'("   taylor: (r,u,v,T,p).tx =(",5f6.2,")")') rtx0,utx0,
     & vtx0,qtx0,ptx0
      write(*,'("   taylor: (r,u,v,T,p).ty =(",5f6.2,")")') rty0,uty0,
     & vty0,qty0,pty0

!      write(*,'(" taylor: j1,j2=",2i3," (r,u,v,T)(t-dt)=(",4f6.3,")"') j1,j2,!               u2(j1,j2,i3,rc),u2(j1,j2,i3,uc),u2(j1,j2,i3,vc),u2(j1,j2,i3,tc)


               if( .false. .or. (i1.gt.nr(1,0)-4 .and. 
     & knownSolution.eq.supersonicFlowInAnExpandingChannel) )then
                u(j1,j2,i3,rc)=uKnown(j1,j2,i3,rc)
                u(j1,j2,i3,uc)=uKnown(j1,j2,i3,uc)
                u(j1,j2,i3,vc)=uKnown(j1,j2,i3,vc)
                u(j1,j2,i3,tc)=uKnown(j1,j2,i3,tc)
              end if
              if( knownSolution.eq.supersonicFlowInAnExpandingChannel 
     & .and. i1.gt.nr(1,0)-4 )then
               j1=i1-2*is1
               j2=i2-2*is2
               u(j1,j2,i3,rc)=uKnown(j1,j2,i3,rc)
               u(j1,j2,i3,uc)=uKnown(j1,j2,i3,uc)
               u(j1,j2,i3,vc)=uKnown(j1,j2,i3,vc)
               u(j1,j2,i3,tc)=uKnown(j1,j2,i3,tc)
               end if
            end if

          else ! mask(i1,i2,i3) <=0
            ! ---------------------------------------------------------------------------------
            ! set points outside of interp or unused points 
            ! ---------------------------------------------------------------------------------
            ! -- note that we need to set ghost points
            ! where mask(i1,i2,i3)=0 if we are next to an interpolation point (pts 1,3 below)
            !                      0  I  X   X  X   <- inside
            !                      0  I  X   X  X   <- boundary
            !                      1  2  g   g  g   <- ghost line 1
            !                      3  4  g   g  g   <- ghost line 2
            rxi = rsxy(i1,i2,i3,axis,0)
            ryi = rsxy(i1,i2,i3,axis,1)
            sxi = rsxy(i1,i2,i3,axisp1,0)
            syi = rsxy(i1,i2,i3,axisp1,1)

            an1=-rxi*sgn
            an2=-ryi*sgn
            aNorm=sqrt(max(epsx,an1**2+an2**2))
            an1=an1/aNorm
            an2=an2/aNorm

            do mm=1,2   ! assign values on two ghost lines
              j1=i1-is1*mm
              j2=i2-is2*mm
              j3=i3-is3*mm
              k1=i1+is1*mm
              k2=i2+is2*mm
              k3=i3+is3*mm

              u(j1,j2,j3,rc)=u(k1,k2,k3,rc)   ! apply symmetry, is this ok ?
              u(j1,j2,j3,tc)=u(k1,k2,k3,tc)

              u(j1,j2,j3,uc) =3.*u(j1+is1,j2+is2,j3,uc)-3.*u(j1+2*is1,
     & j2+2*is2,j3,uc)+u(j1+3*is1,j2+3*is2,j3,uc)
              u(j1,j2,j3,vc) =3.*u(j1+is1,j2+is2,j3,vc)-3.*u(j1+2*is1,
     & j2+2*is2,j3,vc)+u(j1+3*is1,j2+3*is2,j3,vc)

              ! extrap normal component of u 
              !   -- this extrpolation will be consistent with an odd symmetry condition (u.rr=0)
              nDotU1 = an1*( 2.*u(i1,i2,i3,uc)-u(k1,k2,k3,uc) ) + an2*(
     &  2.*u(i1,i2,i3,vc)-u(k1,k2,k3,vc) )

              ! set the normal component to be nDotU1
              nDotU = an1*u(j1,j2,j3,uc)+an2*u(j1,j2,j3,vc) - nDotU1
              u(j1,j2,j3,uc)=u(j1,j2,j3,uc)- nDotU*an1
              u(j1,j2,j3,vc)=u(j1,j2,j3,vc)- nDotU*an2

            end do
            end if
           end do
           end do
           end do

          else if( bcOption.eq.slipWallCharacteristic .and. dt.gt.0 )
     & then

           ! ******************************************************************
           ! **********************slipWallCharacteristic  ****************************
           ! ******************************************************************

           if( gridIsMoving.eq.1 )then
             write(*,'(" cnsSlipWall: slipWallCharacteristic used with 
     & moving grids")')
             ! '
           endif

           if( debug.gt.1 ) then
             write(*,'(" cnsSlipWall: slipWallCharacteristic used")')
           end if
           if( dt.lt.0. )then
             write(*,'(" ***cnsSlipWall:WARNING: dt<0 for t=",e12.3)') 
     & t
             dt=0.
           else
             if( debug.gt.1 ) then
               write(*,'(" ***cnsSlipWall:INFO: t,dt=",2(e12.3,1x))') 
     & t,dt
             end if
           end if

           ! ad2=5.
           ! ad2=max(10.,.5/dr(axisp1)) ! try this
           ad2=max(10.,1./dr(axisp1)) ! try this
           ! ad2=5.  ! for tanDiss2
           ad2dt=ad2*dt
           tm=t-dt
           z0=0.
           do m=0,20
             fv(m)=0  ! forcing for TZ is by default zero
           end do
            n1a=nr(0,0)
            n1b=nr(1,0)
            n2a=nr(0,1)
            n2b=nr(1,1)
            n3a=nr(0,2)
            n3b=nr(1,2)
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
             if( mask(i1,i2,i3).gt.0 )then

            rxi = rsxy(i1,i2,i3,axis,0)
            ryi = rsxy(i1,i2,i3,axis,1)
            sxi = rsxy(i1,i2,i3,axisp1,0)
            syi = rsxy(i1,i2,i3,axisp1,1)

            an1=-rxi*sgn  ! here is the outward normal *wdh* changed sign 040811
            an2=-ryi*sgn
            aNorm=sqrt(max(epsx,an1**2+an2**2))
            an1=an1/aNorm
            an2=an2/aNorm

            ! tangential velocity -- characteristic equation along the boundary
             ! Here are the solution and derivatives at the previous time
             r0  = u2(i1,i2,i3,rc)
             rx0 = u2x22(i1,i2,i3,rc)
             ry0 = u2y22(i1,i2,i3,rc)
             rxx0= u2xx22(i1,i2,i3,rc)
             rxy0= u2xy22(i1,i2,i3,rc)
             ryy0= u2yy22(i1,i2,i3,rc)
             u0  = u2(i1,i2,i3,uc)
             ux0 = u2x22(i1,i2,i3,uc)
             uy0 = u2y22(i1,i2,i3,uc)
             uxx0= u2xx22(i1,i2,i3,uc)
             uxy0= u2xy22(i1,i2,i3,uc)
             uyy0= u2yy22(i1,i2,i3,uc)
             v0  = u2(i1,i2,i3,vc)
             vx0 = u2x22(i1,i2,i3,vc)
             vy0 = u2y22(i1,i2,i3,vc)
             vxx0= u2xx22(i1,i2,i3,vc)
             vxy0= u2xy22(i1,i2,i3,vc)
             vyy0= u2yy22(i1,i2,i3,vc)
             q0  = u2(i1,i2,i3,tc)
             qx0 = u2x22(i1,i2,i3,tc)
             qy0 = u2y22(i1,i2,i3,tc)
             qxx0= u2xx22(i1,i2,i3,tc)
             qxy0= u2xy22(i1,i2,i3,tc)
             qyy0= u2yy22(i1,i2,i3,tc)
             p0 = r0*q0                     ! Rg needed
             px0 =rx0*q0+r0*qx0
             py0 =ry0*q0+r0*qy0
             pxx0=rxx0*q0+rx0*qx0 + rx0*qx0+r0*qxx0
             pxy0=rxy0*q0+ry0*qx0 + rx0*qy0+r0*qxy0
             pyy0=ryy0*q0+ry0*qy0 + ry0*qy0+r0*qyy0
             if( twilightZone.ne.0 )then
               ! evaluate TZ forcing at t
               call ogftaylor(ep,x(i1,i2,i3,0),x(i1,i2,i3,1),z0,tm,nd,
     & fv)
             end if
             if( gridIsMoving.ne.0 )then
               ! -- add moving grid terms ----
               if( twilightZone.eq.0 )then
                 do mm=0,19
                   fv(mm)=0.
                 end do
               end if
               ! *** note: we need gv at t-dt here -- 
               fv( 0)=fv( 0) + gv2(i1,i2,i3,0)*rx0 + gv2(i1,i2,i3,1)*
     & ry0
               fv( 1)=fv( 1) + gv2(i1,i2,i3,0)*ux0 + gv2(i1,i2,i3,1)*
     & uy0
               fv( 2)=fv( 2) + gv2(i1,i2,i3,0)*vx0 + gv2(i1,i2,i3,1)*
     & vy0
               fv( 3)=fv( 3) + gv2(i1,i2,i3,0)*qx0 + gv2(i1,i2,i3,1)*
     & qy0
               fv( 4)=fv( 4) + gv2(i1,i2,i3,0)*px0 + gv2(i1,i2,i3,1)*
     & py0
               ! estimate gtt (we cannot use gtt if we are not on the boundary)
               gttu0 = (gv(i1,i2,i3,0)-gv2(i1,i2,i3,0))/dt
               gttv0 = (gv(i1,i2,i3,1)-gv2(i1,i2,i3,1))/dt
               fv( 5)=fv( 5) + gttu0*rx0 + gttv0*ry0 + gv2(i1,i2,i3,0)*
     & rtx0 + gv2(i1,i2,i3,1)*rty0
               fv( 6)=fv( 6) + gttu0*ux0 + gttv0*uy0 + gv2(i1,i2,i3,0)*
     & utx0 + gv2(i1,i2,i3,1)*uty0
               fv( 7)=fv( 7) + gttu0*vx0 + gttv0*vy0 + gv2(i1,i2,i3,0)*
     & vtx0 + gv2(i1,i2,i3,1)*vty0
               fv( 8)=fv( 8) + gttu0*qx0 + gttv0*qy0 + gv2(i1,i2,i3,0)*
     & qtx0 + gv2(i1,i2,i3,1)*qty0
               fv( 9)=fv( 9) + gttu0*px0 + gttv0*py0 + gv2(i1,i2,i3,0)*
     & ptx0 + gv2(i1,i2,i3,1)*pty0
               ! we need derivatives of the grid velocity:
               gvux0=gv2x22(i1,i2,i3,0)
               gvuy0=gv2y22(i1,i2,i3,0)
               gvvx0=gv2x22(i1,i2,i3,1)
               gvvy0=gv2y22(i1,i2,i3,1)
               fv(10)=fv(10) + gv2(i1,i2,i3,0)*rxx0 + gv2(i1,i2,i3,1)*
     & rxy0 + gvux0*rx0 + gvvx0*ry0
               fv(11)=fv(11) + gv2(i1,i2,i3,0)*uxx0 + gv2(i1,i2,i3,1)*
     & uxy0 + gvux0*ux0 + gvvx0*uy0
               fv(12)=fv(12) + gv2(i1,i2,i3,0)*vxx0 + gv2(i1,i2,i3,1)*
     & vxy0 + gvux0*vx0 + gvvx0*vy0
               fv(13)=fv(13) + gv2(i1,i2,i3,0)*qxx0 + gv2(i1,i2,i3,1)*
     & qxy0 + gvux0*qx0 + gvvx0*qy0
               fv(14)=fv(14) + gv2(i1,i2,i3,0)*pxx0 + gv2(i1,i2,i3,1)*
     & pxy0 + gvux0*px0 + gvvx0*py0
               fv(15)=fv(15) + gv2(i1,i2,i3,0)*rxy0 + gv2(i1,i2,i3,1)*
     & ryy0 + gvuy0*rx0 + gvvy0*ry0
               fv(16)=fv(16) + gv2(i1,i2,i3,0)*uxy0 + gv2(i1,i2,i3,1)*
     & uyy0 + gvuy0*ux0 + gvvy0*uy0
               fv(17)=fv(17) + gv2(i1,i2,i3,0)*vxy0 + gv2(i1,i2,i3,1)*
     & vyy0 + gvuy0*vx0 + gvvy0*vy0
               fv(18)=fv(18) + gv2(i1,i2,i3,0)*qxy0 + gv2(i1,i2,i3,1)*
     & qyy0 + gvuy0*qx0 + gvvy0*qy0
               fv(19)=fv(19) + gv2(i1,i2,i3,0)*pxy0 + gv2(i1,i2,i3,1)*
     & pyy0 + gvuy0*px0 + gvvy0*py0
             end if
             pt0 = -( u0*px0 + v0*py0 + gamma*p0*(ux0+vy0) -fv(4) )
             ptx0 =-( ux0*px0+vx0*py0 + gamma*px0*(ux0+vy0) + u0*pxx0+
     & v0*pxy0 + gamma*p0*(uxx0+vxy0) -fv(14) )
             pty0 =-( uy0*px0+vy0*py0 + gamma*py0*(ux0+vy0) + u0*pxy0+
     & v0*pyy0 + gamma*p0*(uxy0+vyy0) -fv(19) )
             qt0 = -( u0*qx0 + v0*qy0 + gm1*q0*(ux0+vy0) -fv(3) )
             qtx0 =-( ux0*qx0+vx0*qy0 + gm1*qx0*(ux0+vy0) + u0*qxx0+v0*
     & qxy0 + gm1*q0*(uxx0+vxy0) -fv(13) )
             qty0 =-( uy0*qx0+vy0*qy0 + gm1*qy0*(ux0+vy0) + u0*qxy0+v0*
     & qyy0 + gm1*q0*(uxy0+vyy0) -fv(18) )
             rt0 = -( u0*rx0+v0*ry0 + r0*(ux0+vy0) -fv(0) )
             rtx0= -( ux0*rx0 +rx0*ux0 +vx0*ry0 + rx0*vy0 + u0*rxx0+v0*
     & rxy0 + r0*(uxx0+vxy0) -fv(10) )
             rty0= -( uy0*rx0 +ry0*ux0 +vy0*ry0 + ry0*vy0 + u0*rxy0+v0*
     & ryy0 + r0*(uxy0+vyy0) -fv(15) )
             ut0 = -( u0*ux0 + v0*uy0 + px0/r0 -fv(1) )
             utx0= -( ux0*ux0 +u0*uxx0 + vx0*uy0 + v0*uxy0 + pxx0/r0 - 
     & px0*rx0/(r0**2) -fv(11) )
             uty0= -( uy0*ux0 +u0*uxy0 + vy0*uy0 + v0*uyy0 + pxy0/r0 - 
     & px0*ry0/(r0**2) -fv(16) )
             vt0 = -( u0*vx0 + v0*vy0 + py0/r0 -fv(2) )
             vtx0= -( ux0*vx0 +u0*vxx0 + vx0*vy0 + v0*vxy0 + pxy0/r0 - 
     & py0*rx0/(r0**2) -fv(12) )
             vty0= -( uy0*vx0 +u0*vxy0 + vy0*vy0 + v0*vyy0 + pyy0/r0 - 
     & py0*ry0/(r0**2) -fv(17) )
             rtt0= -( ut0*rx0+vt0*ry0 + rt0*(ux0+vy0) + u0*rtx0+v0*
     & rty0 + r0*(utx0+vty0) -fv(5) )
             utt0= -( ut0*ux0 + vt0*uy0 + ptx0/r0 + u0*utx0 + v0*uty0 -
     &  px0*rt0/(r0**2) -fv(6) )
             vtt0= -( ut0*vx0 + vt0*vy0 + pty0/r0 + u0*vtx0 + v0*vty0 -
     &  py0*rt0/(r0**2) -fv(7) )
             ptt0= -( ut0*px0 + vt0*py0 + gamma*pt0*(ux0+vy0) + u0*
     & ptx0 + v0*pty0 + gamma*p0*(utx0+vty0) -fv(9) )
             qtt0= -( ut0*qx0 + vt0*qy0 + gm1*qt0*(ux0+vy0) + u0*qtx0 +
     &  v0*qty0 + gm1*q0*(utx0+vty0) -fv(8) )

            u1 = u0 + dt*ut0 + .5*dt**2*utt0
            v1 = v0 + dt*vt0 + .5*dt**2*vtt0

            if( debug.gt.2 )then
             write(*,'("--> i1,i2=",2i3," u1,v1=",2f10.5," ut0,vt0,
     & utt0,vtt0=",4(e8.2,1x))') i1,i2,u1,v1,ut0,vt0,utt0,vtt0
            end if
            ! '
            ! ** for testing: v1=0. 


            ! tv = -ryi*u1 +rxi*v1 ! tangential velocity on the boundary

            u(i1,i2,i3,uc)=u1  ! set both components, normal component will then be set below:
            u(i1,i2,i3,vc)=v1

            testSym=.false.
            if( testSym )then
              u(i1,i2,i3,vc)=0.
            end if


            ! Set the normal component to zero 
            nDotU=an1*u(i1,i2,i3,uc)+an2*u(i1,i2,i3,vc)
            if( gridIsMoving.ne.0 )then
              nDotU = nDotU - (an1*gv(i1,i2,i3,0)+an2*gv(i1,i2,i3,1))  ! n.u = n.gv for moving grids
            end if
            if( twilightZone.ne.0 )then
              do m=1,2 ! evaluate (u,v)
                uv(m)=ogf(ep,x(i1,i2,i3,0),x(i1,i2,i3,1),0.,m,t)
              end do
              nDotU=nDotU-(an1*uv(1)+an2*uv(2))
            end if
            u(i1,i2,i3,uc)=u(i1,i2,i3,uc)- nDotU*an1
            u(i1,i2,i3,vc)=u(i1,i2,i3,vc)- nDotU*an2




            ! Get u,v at ghost points by Taylor series from the boundary at the previous time 
            getGhostByTaylor=.true.
            if( getGhostByTaylor )then
             do mm=1,2
              j1=i1-is1*mm
              j2=i2-is2*mm
              j3=i3-is3*mm

              hx = x(j1,j2,j3,0)-x(i1,i2,i3,0)
              hy = x(j1,j2,j3,1)-x(i1,i2,i3,1)

              ! u.tau \approx sx*(rx*ur + sx*us) + sy*(ry*ur+sy*us) / sqrt( sx^2 + sy^2 )
              ! compute coeffcienst of dissipation that are proprootional to |u.tau| 
              ! computeAdu(j1,j2,j3,js1,js2,js3)
               r1 = r0 + dt*rt0 + hx*rx0 + hy*ry0 +.5*dt**2*rtt0 + dt*(
     &  hx*rtx0+hy*rty0 ) + .5*hx**2*rxx0 + hx*hy*rxy0 + .5*hy**2*
     & ryy0 +  disst2(j1,j2,j3,rc)
               u1 = u0 + dt*ut0 + hx*ux0 + hy*uy0 +.5*dt**2*utt0 + dt*(
     &  hx*utx0+hy*uty0 ) + .5*hx**2*uxx0 + hx*hy*uxy0 + .5*hy**2*
     & uyy0 +  disst2(j1,j2,j3,uc)
               v1 = v0 + dt*vt0 + hx*vx0 + hy*vy0 +.5*dt**2*vtt0 + dt*(
     &  hx*vtx0+hy*vty0 ) + .5*hx**2*vxx0 + hx*hy*vxy0 + .5*hy**2*
     & vyy0 +  disst2(j1,j2,j3,vc)
               q1 = q0 + dt*qt0 + hx*qx0 + hy*qy0 +.5*dt**2*qtt0 + dt*(
     &  hx*qtx0+hy*qty0 ) + .5*hx**2*qxx0 + hx*hy*qxy0 + .5*hy**2*
     & qyy0 +  disst2(j1,j2,j3,tc)

              u(j1,j2,j3,uc)=u1
              u(j1,j2,j3,vc)=v1
              ! nDotuv(mm)=an1*u1+an2*v1 
             end do
            end if
            ! nDotU1= an1*u1+an2*v1 ! we get nDotU at the first ghost line

!$$$            if( axis.eq.0. )then
!$$$              rr0  =  u2r2 (i1,i2,i3,rc)
!$$$              rxr0 = u2xr22(i1,i2,i3,rc)
!$$$              ryr0 = u2yr22(i1,i2,i3,rc)
!$$$
!$$$              ur0  =  u2r2 (i1,i2,i3,uc)
!$$$              uxr0 = u2xr22(i1,i2,i3,uc)
!$$$              uyr0 = u2yr22(i1,i2,i3,uc)
!$$$
!$$$              vr0  =  u2r2 (i1,i2,i3,vc)
!$$$              vxr0 = u2xr22(i1,i2,i3,vc)
!$$$              vyr0 = u2yr22(i1,i2,i3,vc)
!$$$
!$$$              qr0  =  u2r2 (i1,i2,i3,tc)
!$$$              qxr0 = u2xr22(i1,i2,i3,tc)
!$$$              qyr0 = u2yr22(i1,i2,i3,tc)
!$$$            else
!$$$              rr0  =  u2s2 (i1,i2,i3,rc)
!$$$              rxr0 = u2xs22(i1,i2,i3,rc)
!$$$              ryr0 = u2ys22(i1,i2,i3,rc)
!$$$
!$$$              ur0  =  u2s2 (i1,i2,i3,uc)
!$$$              uxr0 = u2xs22(i1,i2,i3,uc)
!$$$              uyr0 = u2ys22(i1,i2,i3,uc)
!$$$
!$$$              vr0  =  u2s2 (i1,i2,i3,vc)
!$$$              vxr0 = u2xs22(i1,i2,i3,vc)
!$$$              vyr0 = u2ys22(i1,i2,i3,vc)
!$$$
!$$$              qr0  =  u2s2 (i1,i2,i3,tc)
!$$$              qxr0 = u2xs22(i1,i2,i3,tc)
!$$$              qyr0 = u2ys22(i1,i2,i3,tc)
!$$$            end if
!$$$
!$$$            pxr0 = rxr0*q0 + rx0*qr0 + rr0*qx0 + r0*qxr0
!$$$            pyr0 = ryr0*q0 + ry0*qr0 + rr0*qy0 + r0*qyr0
!$$$
!$$$            utr0 = -( ur0*ux0 +u0*uxr0 + vr0*uy0 + v0*uyr0 + pxr0/r0 - px0*rr0/(r0**2) )
!$$$            vtr0 = -( ur0*vx0 +u0*vxr0 + vr0*vy0 + v0*vyr0 + pyr0/r0 - py0*rr0/(r0**2) )
!$$$
!$$$            ur1 = ur0 + dt*utr0 ! +  .5*dt**2*uttr0  ! we could potential approx. uttr0 
!$$$            vr1 = vr0 + dt*vtr0 ! +  .5*dt**2*vttr0

            ! ***** do not use the above for now *****

            ur1=0.
            vr1=0.


            ! 2. Entropy equation on the boundary   p/rho^gamma
            ! 3. Tangential component of the velocity from the characteristic equation
            ! 4. Outgoing characteristic equation: 
            !        g*p*rx*du/dt + g*p*ry*dv/dt + cb*dp/dt + G = 0 along  dr/dt = ub - cb 
            !    --> gives p on the boundary, p(0)

            ! Ghost line:
            !    5. p.r = ...   --> gives p(-1) 
            !    6. Extrap entropy ---> gives r(-1) when combined with p(-1)
            !    7. 
            !    8. Outgoing characteristic:   a1*u(-1) + a2*v(-1) + a3*p(-1) = ...  gives normal component of velocity



            s0 = p0/r0**gamma   ! "entropy" at previous time on the boundary

            st0 = pt0/r0**gamma - gamma*(p0/r0**(gamma+1))*rt0  ! should we use entropy eqn?

            stt0 = ptt0/r0**gamma - 2.*gamma*(pt0/r0**(gamma+1))*rt0 +
     & gamma*(gamma+1.)*(p0/r0**(gamma+2.))*rt0**2 - gamma*(p0/r0**(
     & gamma+1))*rtt0


            s1 = s0 + dt*st0 +.5*dt**2*stt0          ! should we use entropy eqn?

           ! outgoing characteristic on the boundary gives p
           ! trace the solution from the first line inside:
            j1=i1+is1
            j2=i2+is2
            j3=i3+is3

            hx = x(i1,i2,i3,0)-x(j1,j2,j3,0)
            hy = x(i1,i2,i3,1)-x(j1,j2,j3,1)


            ! get the solution derivatives at (j1,j2,j3) -- first line in
             ! Here are the solution and derivatives at the previous time
             r0  = u2(j1,j2,j3,rc)
             rx0 = u2x22(j1,j2,j3,rc)
             ry0 = u2y22(j1,j2,j3,rc)
             rxx0= u2xx22(j1,j2,j3,rc)
             rxy0= u2xy22(j1,j2,j3,rc)
             ryy0= u2yy22(j1,j2,j3,rc)
             u0  = u2(j1,j2,j3,uc)
             ux0 = u2x22(j1,j2,j3,uc)
             uy0 = u2y22(j1,j2,j3,uc)
             uxx0= u2xx22(j1,j2,j3,uc)
             uxy0= u2xy22(j1,j2,j3,uc)
             uyy0= u2yy22(j1,j2,j3,uc)
             v0  = u2(j1,j2,j3,vc)
             vx0 = u2x22(j1,j2,j3,vc)
             vy0 = u2y22(j1,j2,j3,vc)
             vxx0= u2xx22(j1,j2,j3,vc)
             vxy0= u2xy22(j1,j2,j3,vc)
             vyy0= u2yy22(j1,j2,j3,vc)
             q0  = u2(j1,j2,j3,tc)
             qx0 = u2x22(j1,j2,j3,tc)
             qy0 = u2y22(j1,j2,j3,tc)
             qxx0= u2xx22(j1,j2,j3,tc)
             qxy0= u2xy22(j1,j2,j3,tc)
             qyy0= u2yy22(j1,j2,j3,tc)
             p0 = r0*q0                     ! Rg needed
             px0 =rx0*q0+r0*qx0
             py0 =ry0*q0+r0*qy0
             pxx0=rxx0*q0+rx0*qx0 + rx0*qx0+r0*qxx0
             pxy0=rxy0*q0+ry0*qx0 + rx0*qy0+r0*qxy0
             pyy0=ryy0*q0+ry0*qy0 + ry0*qy0+r0*qyy0
             if( twilightZone.ne.0 )then
               ! evaluate TZ forcing at t
               call ogftaylor(ep,x(j1,j2,j3,0),x(j1,j2,j3,1),z0,tm,nd,
     & fv)
             end if
             if( gridIsMoving.ne.0 )then
               ! -- add moving grid terms ----
               if( twilightZone.eq.0 )then
                 do mm=0,19
                   fv(mm)=0.
                 end do
               end if
               ! *** note: we need gv at t-dt here -- 
               fv( 0)=fv( 0) + gv2(j1,j2,j3,0)*rx0 + gv2(j1,j2,j3,1)*
     & ry0
               fv( 1)=fv( 1) + gv2(j1,j2,j3,0)*ux0 + gv2(j1,j2,j3,1)*
     & uy0
               fv( 2)=fv( 2) + gv2(j1,j2,j3,0)*vx0 + gv2(j1,j2,j3,1)*
     & vy0
               fv( 3)=fv( 3) + gv2(j1,j2,j3,0)*qx0 + gv2(j1,j2,j3,1)*
     & qy0
               fv( 4)=fv( 4) + gv2(j1,j2,j3,0)*px0 + gv2(j1,j2,j3,1)*
     & py0
               ! estimate gtt (we cannot use gtt if we are not on the boundary)
               gttu0 = (gv(j1,j2,j3,0)-gv2(j1,j2,j3,0))/dt
               gttv0 = (gv(j1,j2,j3,1)-gv2(j1,j2,j3,1))/dt
               fv( 5)=fv( 5) + gttu0*rx0 + gttv0*ry0 + gv2(j1,j2,j3,0)*
     & rtx0 + gv2(j1,j2,j3,1)*rty0
               fv( 6)=fv( 6) + gttu0*ux0 + gttv0*uy0 + gv2(j1,j2,j3,0)*
     & utx0 + gv2(j1,j2,j3,1)*uty0
               fv( 7)=fv( 7) + gttu0*vx0 + gttv0*vy0 + gv2(j1,j2,j3,0)*
     & vtx0 + gv2(j1,j2,j3,1)*vty0
               fv( 8)=fv( 8) + gttu0*qx0 + gttv0*qy0 + gv2(j1,j2,j3,0)*
     & qtx0 + gv2(j1,j2,j3,1)*qty0
               fv( 9)=fv( 9) + gttu0*px0 + gttv0*py0 + gv2(j1,j2,j3,0)*
     & ptx0 + gv2(j1,j2,j3,1)*pty0
               ! we need derivatives of the grid velocity:
               gvux0=gv2x22(j1,j2,j3,0)
               gvuy0=gv2y22(j1,j2,j3,0)
               gvvx0=gv2x22(j1,j2,j3,1)
               gvvy0=gv2y22(j1,j2,j3,1)
               fv(10)=fv(10) + gv2(j1,j2,j3,0)*rxx0 + gv2(j1,j2,j3,1)*
     & rxy0 + gvux0*rx0 + gvvx0*ry0
               fv(11)=fv(11) + gv2(j1,j2,j3,0)*uxx0 + gv2(j1,j2,j3,1)*
     & uxy0 + gvux0*ux0 + gvvx0*uy0
               fv(12)=fv(12) + gv2(j1,j2,j3,0)*vxx0 + gv2(j1,j2,j3,1)*
     & vxy0 + gvux0*vx0 + gvvx0*vy0
               fv(13)=fv(13) + gv2(j1,j2,j3,0)*qxx0 + gv2(j1,j2,j3,1)*
     & qxy0 + gvux0*qx0 + gvvx0*qy0
               fv(14)=fv(14) + gv2(j1,j2,j3,0)*pxx0 + gv2(j1,j2,j3,1)*
     & pxy0 + gvux0*px0 + gvvx0*py0
               fv(15)=fv(15) + gv2(j1,j2,j3,0)*rxy0 + gv2(j1,j2,j3,1)*
     & ryy0 + gvuy0*rx0 + gvvy0*ry0
               fv(16)=fv(16) + gv2(j1,j2,j3,0)*uxy0 + gv2(j1,j2,j3,1)*
     & uyy0 + gvuy0*ux0 + gvvy0*uy0
               fv(17)=fv(17) + gv2(j1,j2,j3,0)*vxy0 + gv2(j1,j2,j3,1)*
     & vyy0 + gvuy0*vx0 + gvvy0*vy0
               fv(18)=fv(18) + gv2(j1,j2,j3,0)*qxy0 + gv2(j1,j2,j3,1)*
     & qyy0 + gvuy0*qx0 + gvvy0*qy0
               fv(19)=fv(19) + gv2(j1,j2,j3,0)*pxy0 + gv2(j1,j2,j3,1)*
     & pyy0 + gvuy0*px0 + gvvy0*py0
             end if
             pt0 = -( u0*px0 + v0*py0 + gamma*p0*(ux0+vy0) -fv(4) )
             ptx0 =-( ux0*px0+vx0*py0 + gamma*px0*(ux0+vy0) + u0*pxx0+
     & v0*pxy0 + gamma*p0*(uxx0+vxy0) -fv(14) )
             pty0 =-( uy0*px0+vy0*py0 + gamma*py0*(ux0+vy0) + u0*pxy0+
     & v0*pyy0 + gamma*p0*(uxy0+vyy0) -fv(19) )
             qt0 = -( u0*qx0 + v0*qy0 + gm1*q0*(ux0+vy0) -fv(3) )
             qtx0 =-( ux0*qx0+vx0*qy0 + gm1*qx0*(ux0+vy0) + u0*qxx0+v0*
     & qxy0 + gm1*q0*(uxx0+vxy0) -fv(13) )
             qty0 =-( uy0*qx0+vy0*qy0 + gm1*qy0*(ux0+vy0) + u0*qxy0+v0*
     & qyy0 + gm1*q0*(uxy0+vyy0) -fv(18) )
             rt0 = -( u0*rx0+v0*ry0 + r0*(ux0+vy0) -fv(0) )
             rtx0= -( ux0*rx0 +rx0*ux0 +vx0*ry0 + rx0*vy0 + u0*rxx0+v0*
     & rxy0 + r0*(uxx0+vxy0) -fv(10) )
             rty0= -( uy0*rx0 +ry0*ux0 +vy0*ry0 + ry0*vy0 + u0*rxy0+v0*
     & ryy0 + r0*(uxy0+vyy0) -fv(15) )
             ut0 = -( u0*ux0 + v0*uy0 + px0/r0 -fv(1) )
             utx0= -( ux0*ux0 +u0*uxx0 + vx0*uy0 + v0*uxy0 + pxx0/r0 - 
     & px0*rx0/(r0**2) -fv(11) )
             uty0= -( uy0*ux0 +u0*uxy0 + vy0*uy0 + v0*uyy0 + pxy0/r0 - 
     & px0*ry0/(r0**2) -fv(16) )
             vt0 = -( u0*vx0 + v0*vy0 + py0/r0 -fv(2) )
             vtx0= -( ux0*vx0 +u0*vxx0 + vx0*vy0 + v0*vxy0 + pxy0/r0 - 
     & py0*rx0/(r0**2) -fv(12) )
             vty0= -( uy0*vx0 +u0*vxy0 + vy0*vy0 + v0*vyy0 + pyy0/r0 - 
     & py0*ry0/(r0**2) -fv(17) )
             rtt0= -( ut0*rx0+vt0*ry0 + rt0*(ux0+vy0) + u0*rtx0+v0*
     & rty0 + r0*(utx0+vty0) -fv(5) )
             utt0= -( ut0*ux0 + vt0*uy0 + ptx0/r0 + u0*utx0 + v0*uty0 -
     &  px0*rt0/(r0**2) -fv(6) )
             vtt0= -( ut0*vx0 + vt0*vy0 + pty0/r0 + u0*vtx0 + v0*vty0 -
     &  py0*rt0/(r0**2) -fv(7) )
             ptt0= -( ut0*px0 + vt0*py0 + gamma*pt0*(ux0+vy0) + u0*
     & ptx0 + v0*pty0 + gamma*p0*(utx0+vty0) -fv(9) )
             qtt0= -( ut0*qx0 + vt0*qy0 + gm1*qt0*(ux0+vy0) + u0*qtx0 +
     &  v0*qty0 + gm1*q0*(utx0+vty0) -fv(8) )

            ! computeAdu(j1,j2,j3,js1,js2,js3)
             r1 = r0 + dt*rt0 + hx*rx0 + hy*ry0 +.5*dt**2*rtt0 + dt*( 
     & hx*rtx0+hy*rty0 ) + .5*hx**2*rxx0 + hx*hy*rxy0 + .5*hy**2*ryy0 
     & +  disst2(i1,i2,i3,rc)
             u1 = u0 + dt*ut0 + hx*ux0 + hy*uy0 +.5*dt**2*utt0 + dt*( 
     & hx*utx0+hy*uty0 ) + .5*hx**2*uxx0 + hx*hy*uxy0 + .5*hy**2*uyy0 
     & +  disst2(i1,i2,i3,uc)
             v1 = v0 + dt*vt0 + hx*vx0 + hy*vy0 +.5*dt**2*vtt0 + dt*( 
     & hx*vtx0+hy*vty0 ) + .5*hx**2*vxx0 + hx*hy*vxy0 + .5*hy**2*vyy0 
     & +  disst2(i1,i2,i3,vc)
             q1 = q0 + dt*qt0 + hx*qx0 + hy*qy0 +.5*dt**2*qtt0 + dt*( 
     & hx*qtx0+hy*qty0 ) + .5*hx**2*qxx0 + hx*hy*qxy0 + .5*hy**2*qyy0 
     & +  disst2(i1,i2,i3,tc)

            p1 = r1*q1           ! define p on the boundary
            if( debug.gt.2 )then
              write(*,'("--> i1,i2=",2i3," r1,u1,v1,q1,p1=",5(e10.4,1x)
     & )') i1,i2,r1,u1,v1,q1,p1
              ! '
            end if
            if( .false. .and. .not.testSym )then
              u(i1,i2,i3,rc)=(p1/s1)**(1./gamma)  ! get rho from entropy condition and p
              u(i1,i2,i3,tc)=p1/u(i1,i2,i3,rc)    ! get T = p/rho
            end if



             if( .false. .and. knownSolution.gt.0  )then
               ! set the true solution at two ghost lines for testing
               do mm=1,2
                 j1=i1-is1*mm
                 j2=i2-is2*mm
                 j3=i3-is3*mm

                 ! if( i1.gt.nr(1,0)-4 )then
                 do m=0,3
                   u(j1,j2,j3,m)=uKnown(j1,j2,j3,m)
                 end do

               end do
             end if


            ! **** ghost lines ***



            ! BC: p.n = rho*( ... )
            rho = u(i1,i2,i3,rc)
            tp  = u(i1,i2,i3,tc)
            u0s=(u(i1+js1,i2+js2,i3,uc)-u(i1-js1,i2-js2,i3,uc))/(2.*dr(
     & axisp1))
            v0s=(u(i1+js1,i2+js2,i3,vc)-u(i1-js1,i2-js2,i3,vc))/(2.*dr(
     & axisp1))

            pn = -rho*( sxi*u(i1,i2,i3,uc)+syi*u(i1,i2,i3,vc) )*(an1*
     & u0s+an2*v0s)
            if( gridIsMoving.ne.0 )then
              pn = pn - rho*( an1*gtt(i1,i2,i3,0)+an2*gtt(i1,i2,i3,1) )
     &  + rho*( sxi*gv(i1,i2,i3,0)+syi*gv(i1,i2,i3,1) )*(an1*u0s+an2*
     & v0s)
            end if

            rhos=(u(i1+js1,i2+js2,i3,rc)-u(i1-js1,i2-js2,i3,rc))/(2.*
     & dr(axisp1))
            tps =(u(i1+js1,i2+js2,i3,tc)-u(i1-js1,i2-js2,i3,tc))/(2.*
     & dr(axisp1))

            ps = rhos*tp + rho*tps

            ! Here we assign T(-1)  from the equation for p.n
            ! ** u(i1-is1,i2-is2,i3,tc)=u(i1+is1,i2+is2,i3,tc)-sgn*2.*dr(axis)*( pn - an2*ps )/an1

            ! pp= u(i1+is1,i2+is2,i3,rc)*u(i1+is1,i2+is2,i3,tc)
            ! pm =pp  -sgn*2.*dr(axis)*( pn - (an1*sxi+an2*syi)*ps )/(an1*rxi+an2*ryi)
            ! tpm=pm/u(i1-is1,i2-is2,i3,rc)


            pr = ( pn - (an1*sxi+an2*syi)*ps )/(an1*rxi+an2*ryi)

            if( .false. .and. testSym )then  ! ok
              pr=0.
            end if

            ! rhor = sgn*(u(i1+is1,i2+is2,i3,rc)-u(i1-is1,i2-is2,i3,rc))/(2.*dr(axis))

            ! If p/rho**gamma = S --> p.r = gamma*(p/rho)*rho.r + rho^gamma S_r 
            !             p.r = gamma*(p/rho)*rho.r + (p/S) S_r
            p1 = rho*tp            ! p at t
            s1 = p1/rho**gamma     ! S at t
            ! rhor = (pr/(gamma*p1))*rho
            s1p = u(i1+is1,i2+is2,i3,tc)/( u(i1+is1,i2+is2,i3,rc)**gm1 
     & )  ! S= p/rho^g = T/rho^gm1
            sr = sgn*(s1p - s1 )/dr(axis)  ! first order approx. to S.r
            rhor = (pr - (p1/s1)*sr )*rho/(gamma*p1)

            tpr = (pr-rhor*tp)/rho
            tpm = u(i1+is1,i2+is2,i3,tc) -sgn*2.*dr(axis)*tpr
            u(i1-is1,i2-is2,i3,tc)=tpm

            u(i1-is1,i2-is2,i3,rc)=u(i1+is1,i2+is2,i3,rc) - sgn*(2.*dr(
     & axis))*rhor

            tpm = u(i1+2*is1,i2+2*is2,i3,tc) -sgn*4.*dr(axis)*tpr
            u(i1-2*is1,i2-2*is2,i3,tc)=tpm

            u(i1-2*is1,i2-2*is2,i3,rc)=u(i1+2*is1,i2+2*is2,i3,rc) - 
     & sgn*(4.*dr(axis))*rhor

            if( debug.gt.2 )then
              write(*,'(" slip-char: i=",i3,i3," pn,pr,ps,rhor,gv,
     & gtt=",10(e9.2,1x))') i1,i2,pn,pr,ps,rhor,gv(i1,i2,i3,0),gv(i1,
     & i2,i3,1),gtt(i1,i2,i3,0),gtt(i1,i2,i3,1)
              ! '
            end if

            ! nDotU=nDotU1  ! from the "characteristics"

            ! get tangential component by advancing the (tau.uv).r 

            ! finish this 

            ! utr0 = -( ur0*ux0 +u0*uxr0 + vr0*uy0 + v0*ury0 + pxr0/r0 - px0*rr0/(r0**2) )

            ! ur1 = ur0 + dt*utr0 ! +  .5*dt**2*uttr0  ! we could potential approx. uttr0 
            ! vr1 = vr0 + dt*vtr0 ! +  .5*dt**2*vttr0
            ! ur1=0.
            ! vr1=0.
            if( .not.getGhostByTaylor )then
             do mm=1,2
              j1=i1-is1*mm
              j2=i2-is2*mm
              j3=i3-is3*mm
              k1=i1+is1*mm
              k2=i2+is2*mm
              k3=i3+is3*mm

              ! u(j1,j2,j3,uc) = u(k1,k2,k3,uc) - sgn*(2.*mm*dr(axis))*ur1
              ! u(j1,j2,j3,vc) = u(k1,k2,k3,vc) - sgn*(2.*mm*dr(axis))*vr1
              u(j1,j2,j3,uc) =3.*u(j1+is1,j2+is2,j3,uc)-3.*u(j1+2*is1,
     & j2+2*is2,j3,uc)+u(j1+3*is1,j2+3*is2,j3,uc)
              u(j1,j2,j3,vc) =3.*u(j1+is1,j2+is2,j3,vc)-3.*u(j1+2*is1,
     & j2+2*is2,j3,vc)+u(j1+3*is1,j2+3*is2,j3,vc)

              ! extrap normal component of u for now (could use outgoing char instead)
              !   -- this extrpolation will be consistent with an odd symmetry condition (u.rr=0)
              nDotU1 = an1*( 2.*u(i1,i2,i3,uc)-u(k1,k2,k3,uc) ) + an2*(
     &  2.*u(i1,i2,i3,vc)-u(k1,k2,k3,vc) )

              ! set the normal component to be nDotU1
              nDotU = an1*u(j1,j2,j3,uc)+an2*u(j1,j2,j3,vc) - nDotU1
              u(j1,j2,j3,uc)=u(j1,j2,j3,uc)- nDotU*an1
              u(j1,j2,j3,vc)=u(j1,j2,j3,vc)- nDotU*an2

              if( .false. .and. testSym )then  ! ok
                u(j1,j2,j3,vc)=0.
              end if

             end do
            end if

             ! set solution at bndry and 2 ghost lines at the ends -- fix this --
             if( .true. .and. (i1.gt.nr(1,0)-4 .and. 
     & knownSolution.eq.supersonicFlowInAnExpandingChannel)  )then
              do mm=0,2
              j1=i1-is1*mm
              j2=i2-is2*mm
              j3=i3-is3*mm
              u(j1,j2,i3,rc)=uKnown(j1,j2,i3,rc)
              u(j1,j2,i3,uc)=uKnown(j1,j2,i3,uc)
              u(j1,j2,i3,vc)=uKnown(j1,j2,i3,vc)
              u(j1,j2,i3,tc)=uKnown(j1,j2,i3,tc)
             end do
            end if


            ! ***** for testing use the exact solution ***
            if( twilightZone.ne.0  )then

             ! set solution at 2 ghost lines
             do mm=1,2
              j1=i1-is1*mm
              j2=i2-is2*mm
              j3=i3-is3*mm
              do m=0,3
                uv(m)=ogf(ep,x(j1,j2,j3,0),x(j1,j2,j3,1),0.,m,t)
                u(j1,j2,j3,m)=uv(m)
              end do
             end do
            end if

            if( twilightZone.ne.0 )then
              j1=i1
              j2=i2
              j3=i3
              do m=0,3
                uv(m)=ogf(ep,x(j1,j2,j3,0),x(j1,j2,j3,1),0.,m,t)
              end do

            write(*,'("--> i1,i2=",2i3," q=",4(f10.5,1x)," err=",4(
     & e10.1,1x))') i1,i2,u(i1,i2,i3,rc),u(i1,i2,i3,uc),u(i1,i2,i3,vc)
     & ,u(i1,i2,i3,tc),abs(u(i1,i2,i3,rc)-uv(rc)),abs(u(i1,i2,i3,uc)-
     & uv(uc)),abs(u(i1,i2,i3,vc)-uv(vc)),abs(u(i1,i2,i3,tc)-uv(tc))
            ! '
            end if


          else ! mask(i1,i2,i3) <=0
            ! ---------------------------------------------------------------------------------
            ! set points outside of interp or unused points 
            ! ---------------------------------------------------------------------------------
            ! -- note that we need to set ghost points
            ! where mask(i1,i2,i3)=0 if we are next to an interpolation point (pts 1,3 below)
            !                      0  I  X   X  X   <- inside
            !                      0  I  X   X  X   <- boundary
            !                      1  2  g   g  g   <- ghost line 1
            !                      3  4  g   g  g   <- ghost line 2
            rxi = rsxy(i1,i2,i3,axis,0)
            ryi = rsxy(i1,i2,i3,axis,1)
            sxi = rsxy(i1,i2,i3,axisp1,0)
            syi = rsxy(i1,i2,i3,axisp1,1)

            an1=-rxi*sgn
            an2=-ryi*sgn
            aNorm=sqrt(max(epsx,an1**2+an2**2))
            an1=an1/aNorm
            an2=an2/aNorm

            do mm=1,2   ! assign values on two ghost lines
              j1=i1-is1*mm
              j2=i2-is2*mm
              j3=i3-is3*mm
              k1=i1+is1*mm
              k2=i2+is2*mm
              k3=i3+is3*mm

              u(j1,j2,j3,rc)=u(k1,k2,k3,rc)   ! apply symmetry, is this ok ?
              u(j1,j2,j3,tc)=u(k1,k2,k3,tc)

              u(j1,j2,j3,uc) =3.*u(j1+is1,j2+is2,j3,uc)-3.*u(j1+2*is1,
     & j2+2*is2,j3,uc)+u(j1+3*is1,j2+3*is2,j3,uc)
              u(j1,j2,j3,vc) =3.*u(j1+is1,j2+is2,j3,vc)-3.*u(j1+2*is1,
     & j2+2*is2,j3,vc)+u(j1+3*is1,j2+3*is2,j3,vc)

              ! extrap normal component of u 
              !   -- this extrpolation will be consistent with an odd symmetry condition (u.rr=0)
              nDotU1 = an1*( 2.*u(i1,i2,i3,uc)-u(k1,k2,k3,uc) ) + an2*(
     &  2.*u(i1,i2,i3,vc)-u(k1,k2,k3,vc) )

              ! set the normal component to be nDotU1
              nDotU = an1*u(j1,j2,j3,uc)+an2*u(j1,j2,j3,vc) - nDotU1
              u(j1,j2,j3,uc)=u(j1,j2,j3,uc)- nDotU*an1
              u(j1,j2,j3,vc)=u(j1,j2,j3,vc)- nDotU*an2

            end do
            end if
           end do
           end do
           end do

          else if( dt.gt.0. )then
            write(*,'("cnsSlipWallBC: ERROR gridIsMoving=",i2," 
     & bcOption=",i3," dt=",e10.2)') gridIsMoving,bcOption,dt
             ! '
            write(*,'("  .. grid,side,axis=",3i4," bc=",i6)') grid,
     & side,axis,bc(side,axis)
            stop 8825
          end if ! bcOption

          nr(0,axis)=indexRange(0,axis)  ! reset
          nr(1,axis)=indexRange(1,axis)

        else if( bc(side,axis).eq.axisymmetric )then

          ! axisymmetric -- this is not quite right for stretched grids ---- fix this ---

          nr(0,axis)=indexRange(side,axis)   ! set nr to point to this side
          nr(1,axis)=indexRange(side,axis)

          if( axis.eq.0 )then
            is1 = 1-2*side
            is2=0
          else
            is1=0
            is2=1-2*side
          end if
          ks1=2*is1
          ks2=2*is2

          if( twilightZone.eq.0 )then
            n1a=nr(0,0)
            n1b=nr(1,0)
            n2a=nr(0,1)
            n2b=nr(1,1)
            n3a=nr(0,2)
            n3b=nr(1,2)
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
            u(i1,i2,i3,urc)=0.
            u(i1-is1,i2-is2,i3,urc)=-u(i1+is1,i2+is2,i3,urc)
            u(i1-ks1,i2-ks2,i3,urc)=-u(i1+ks1,i2+ks2,i3,urc)

            u(i1-is1,i2-is2,i3,rc)= u(i1+is1,i2+is2,i3,rc)
            u(i1-ks1,i2-ks2,i3,rc)= u(i1+ks1,i2+ks2,i3,rc)

            u(i1-is1,i2-is2,i3,uac)= u(i1+is1,i2+is2,i3,uac)
            u(i1-ks1,i2-ks2,i3,uac)= u(i1+ks1,i2+ks2,i3,uac)

            u(i1-is1,i2-is2,i3,tc)= u(i1+is1,i2+is2,i3,tc)
            u(i1-ks1,i2-ks2,i3,tc)= u(i1+ks1,i2+ks2,i3,tc)

            end do
            end do
            end do
          else
           ! TZ *wdh* 100918
            n1a=nr(0,0)
            n1b=nr(1,0)
            n2a=nr(0,1)
            n2b=nr(1,1)
            n3a=nr(0,2)
            n3b=nr(1,2)
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b

            do m=rc,tc ! evaluate (rho,u,v,tc)
              uv(m) =ogf(ep,x(i1    ,i2    ,i3,0),x(i1    ,i2    ,i3,1)
     & ,0.,m,t)
              uvp(m)=ogf(ep,x(i1+is1,i2+is2,i3,0),x(i1+is1,i2+is2,i3,1)
     & ,0.,m,t)
              uvm(m)=ogf(ep,x(i1-is1,i2-is2,i3,0),x(i1-is1,i2-is2,i3,1)
     & ,0.,m,t)
            end do
            u(i1,i2,i3,urc)=uv(urc)

            u(i1-is1,i2-is2,i3,urc)=-u(i1+is1,i2+is2,i3,urc) + uvm(urc)
     & +uvp(urc)
            u(i1-is1,i2-is2,i3,uac)= u(i1+is1,i2+is2,i3,uac) + uvm(uac)
     & -uvp(uac)
            u(i1-is1,i2-is2,i3,rc) = u(i1+is1,i2+is2,i3,rc)  + uvm(rc)-
     & uvp(rc)
            u(i1-is1,i2-is2,i3,tc) = u(i1+is1,i2+is2,i3,tc)  + uvm(tc)-
     & uvp(tc)

            do m=rc,tc ! evaluate (rho,u,v,tc)
              uv(m) =ogf(ep,x(i1    ,i2    ,i3,0),x(i1    ,i2    ,i3,1)
     & ,0.,m,t)
              uvp(m)=ogf(ep,x(i1+ks1,i2+ks2,i3,0),x(i1+ks1,i2+ks2,i3,1)
     & ,0.,m,t)
              uvm(m)=ogf(ep,x(i1-ks1,i2-ks2,i3,0),x(i1-ks1,i2-ks2,i3,1)
     & ,0.,m,t)
            end do
            u(i1-ks1,i2-ks2,i3,urc)=-u(i1+ks1,i2+ks2,i3,urc) + uvm(urc)
     & +uvp(urc)
            u(i1-ks1,i2-ks2,i3,uac)= u(i1+ks1,i2+ks2,i3,uac) + uvm(uac)
     & -uvp(uac)
            u(i1-ks1,i2-ks2,i3,rc) = u(i1+ks1,i2+ks2,i3,rc)  + uvm(rc)-
     & uvp(rc)
            u(i1-ks1,i2-ks2,i3,tc) = u(i1+ks1,i2+ks2,i3,tc)  + uvm(tc)-
     & uvp(tc)

            end do
            end do
            end do
          end if

          if( axisymmetricWithSwirl.eq.1 )then
             n1a=nr(0,0)
             n1b=nr(1,0)
             n2a=nr(0,1)
             n2b=nr(1,1)
             n3a=nr(0,2)
             n3b=nr(1,2)
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
!     kkc 060117 actually extrapolate w since it's derivative may not be zero at the axis
              u(i1-is1,i2-is2,i3,wc)= 2d0*u(i1,i2,i3,wc)-u(i1+is1,i2+
     & is2,i3,wc)
              u(i1-ks1,i2-ks2,i3,wc)=u(i1-is1,i2-is2,i3,wc) ! second ghost line
!              u(i1-is1,i2-is2,i3,wc)= u(i1+is1,i2+is2,i3,wc)
!              u(i1-ks1,i2-ks2,i3,wc)= u(i1+ks1,i2+ks2,i3,wc)
             end do
             end do
             end do
          end if

          ! species
          do s=sc,sc+numberOfSpecies-1
           n1a=nr(0,0)
           n1b=nr(1,0)
           n2a=nr(0,1)
           n2b=nr(1,1)
           n3a=nr(0,2)
           n3b=nr(1,2)
           do i3=n3a,n3b
           do i2=n2a,n2b
           do i1=n1a,n1b
            if( mask(i1,i2,i3).gt.0 )then
            u(i1-is1,i2-is2,i3,s)= u(i1+is1,i2+is2,i3,s)
            u(i1-ks1,i2-ks2,i3,s)= u(i1+ks1,i2+ks2,i3,s)
            end if
           end do
           end do
           end do
          end do

          nr(0,axis)=indexRange(0,axis)  ! reset
          nr(1,axis)=indexRange(1,axis)

        end if

        end do
        end do

      else

        write(*,'("cnsSlipWallBC:ERROR:Unknown bcOption=",i5)') 
     & bcOption
        stop 17342

      end if


      return
      end



