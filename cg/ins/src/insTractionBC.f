! This file automatically generated from insTractionBC.bf with bpp.
!=============================================================================================================
!
!   Routines to assign traction (free surface) Boundary conditions
!
! Notes:
!   July 15, 2017 -- initial version
!============================================================================================================
!

c Use this next macro to declare the statement functions that are defined below
c To include derivatives of rx use OPTION=RX


c Define statement functions for difference approximations of order 2 
c To include derivatives of rx use OPTION=RX
c To include derivatives of rx use OPTION=RX






! ************************************************************************************************
!  This macro is used for looping over the faces of a grid to assign booundary conditions
!
! extra: extra points to assign
!          Case 1: extra=numberOfGhostPoints -- for assigning extended boundaries
!          Case 2: extra=-1 -- for assigning ghost points but not including extended boundaries
! numberOfGhostPoints : number of ghost points (1 for 2nd order, 2 for fourth-order ...)
!
!
! Output:
!  n1a,n1b,n2a,n2b,n3a,n3b : from gridIndexRange
!  nn1a,nn1b,nn2a,nn2b,nn3a,nn3b : includes "extra" points
! 
! ***********************************************************************************************










! ==========================================================================================================
! Apply the TRACTION FREE boundary condition to determine the velocity on the ghost points
!   RECTANGULAR GRID CASE
! 
! ORDER: 2 or 4
! DIR = x, y, z
! DIM = 2 or 3 dimensions
!
! ==========================================================================================================


! ==========================================================================================================
! Apply the TRACTION FREE boundary condition to determine the velocity on the ghost points
!  Curvilinear grid case
! 
! ORDER: 2 or 4
! DIR = r,s,t
! GRIDTYPE: rectangular, curvilinear
!
! ==========================================================================================================
! ====================== END TRACTION FREE 2D CURVILINEAR =========================


! ==========================================================================================================
! Apply the TRACTION FREE boundary condition to determine the velocity on the ghost points
!  Curvilinear grid case **THREE-DIMENSIONS***
! 
! ORDER: 2 or 4
! DIR = r,s,t
! GRIDTYPE: rectangular, curvilinear
!
! ==========================================================================================================
! ====================== END TRACTION FREE 3D Order=2 CURVILINEAR =========================






! ==========================================================================================================
! Apply the boundary condition div(u)=0 div(u).n=0 to determine the normal components of the 2 ghost points
!  Curvilinear grid case
! DIR = r,s,t
! ==========================================================================================================

!  Three-dimensional version



      subroutine insTractionBC(bcOption, nd,nd1a,nd1b,nd2a,nd2b,nd3a,
     & nd3b,nd4a,nd4b,
     & ipar,rpar, u, mask, x,rsxy, gv, gtt, bc, gridIndexRange, ierr )
!=============================================================================================================
!     Assign traction (free surface) Boundary conditions
!
! Notes:
!============================================================================================================
      implicit none
      integer nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b
      real ep ! holds pointer to OGFunction
      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real x(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
      real gv(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
      real gtt(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
      real rpar(0:*)

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer gridIndexRange(0:1,0:2), bc(0:1,0:2)
      integer ipar(0:*),ierr

!.......local
      integer numberOfProcessors,outflowOption,
     & orderOfExtrapolationForOutflow,debug,myid
      integer kd,kd3,i1,i2,i3,n1a,n1b,n2a,n2b,n3a,n3b,c,nr0,nr1
      integer is,j1,j2,j3,side,axis,twilightZone,bcOption
      integer i1m,i2m,i3m,i1p,i2p,i3p
      integer pc,uc,vc,wc,sc,grid,orderOfAccuracy,gridIsMoving,
     & useWhereMask,tc,assignTemperature
      integer gridType,gridIsImplicit,implicitMethod,implicitOption,
     & isAxisymmetric
      integer use2ndOrderAD,use4thOrderAD,advectPassiveScalar
      integer nr(0:1,0:2)
      integer bcOptionWallNormal
      integer bc1,bc2,extrapOrder,ks1,kd1,ks2,kd2,is1,is2,is3
      integer axisp1,axisp2,extra,extra1a,extra1b,extra2a,extra2b,
     & extra3a,extra3b
      integer nn1a,nn1b,nn2a,nn2b,nn3a,nn3b
      integer numberOfGhostPoints

      real t,nu,ad21,ad22,ad41,ad42,nuPassiveScalar,adcPassiveScalar,
     & thermalExpansivity,te
      real cd42,adCoeff4, cd22, adCoeff2
      real dr(0:2),dx(0:2),d14v(0:2),d24v(0:2), gravity(0:2)
      ! real vy,vxy,ux,uxy
      real f1,f2,f3,det,alpha,beta,rxsq,rxsqr,ajs
      real an1,an2,an3,aNormi, t1,t2,t3, b1,b2,b3, crxd,cryd,crzd, ux,
     & uy, vx,vy,wx,wy,uz,vz,wz, um1,vm1,wm1
      real csf1,csf2,csf3,epsX
      real rxd,ryd,rzd,sxd,syd,szd,txd,tyd,tzd,rxsqd
      real rxi,ryi,rzi,sxi,syi,szi,txi,tyi,tzi,rxxi,ryyi,rzzi
      real a11,a12,a13,a21,a22,a23,a31,a32,a33
      real div,div11,div12,div13
      real dive,f1e,f2e,f3e,fMax,res1,res2,res3,resMax
      real c11,c12,c13,c21,c22,c23,c31,c32,c33
      real tvx,tvy,tvz, tvxe,tvye,tvze

      ! real u1,u2,v1,v2,w1,w2,f1u,f2u,f1v,f2v,f1w,f2w,uDotN1,uDotN2

      ! real u0,v0,w0, ux0,uy0,uz0, vx0,vy0,vz0, wx0,wy0,wz0
      ! real ug0,vg0,wg0, gtt0, gtt1, gtt2

      ! variables to hold the exact solution:
      real ue,uxe,uye,uze,uxxe,uyye,uzze,ute
      real ve,vxe,vye,vze,vxxe,vyye,vzze,vte
      real we,wxe,wye,wze,wxxe,wyye,wzze,wte
      real pe,pxe,pye,pze,pxxe,pyye,pzze,pte

      ! real uxa,uya,uza, vxa,vya,vza, wxa,wya,wza
!      real dr12,dr22, dx12,dx22
!      real ur2,us2,ut2, ux22,uy22, ux23,uy23,uz23, ux22r,uy22r, ux23r, uy23r, uz23r

      real uExtrap2,uExtrap3,epsu, u1a,u2a, uLim, clim

!..................
      integer rectangular,curvilinear
      parameter( rectangular=0, curvilinear=1 )

      integer doubleDiv,divAndDivN
      parameter( doubleDiv=0, divAndDivN=1 )
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
     &     axisymmetric,
     &     freeSurfaceBoundaryCondition,
     &     penaltyBoundaryCondition
      parameter( noSlipWall=1,inflowWithVelocityGiven=2,
     & slipWall=4,outflow=5,convectiveOutflow=14,tractionFree=15,
     & inflowWithPandTV=3,
     &  dirichletBoundaryCondition=12,
     &  symmetry=11,axisymmetric=13, penaltyBoundaryCondition=100,
     &  freeSurfaceBoundaryCondition=31 )

      ! outflowOption values:
      integer extrapolateOutflow,neumannAtOuflow
      parameter( extrapolateOutflow=0,neumannAtOuflow=1 )

      ! declare variables for difference approximations
      ! include 'declareDiffOrder4f.h'
      ! declareDifferenceOrder4(u,RX)

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

! .............. begin statement functions
      real divBCr2d,divBCs2d, divBCr3d,divBCs3d,divBCt3d
      real rx,ry,rz,sx,sy,sz,tx,ty,tz
      real insbfu2d,insbfv2d,insbfu3d,insbfv3d,insbfw3d,ogf
      real delta42,delta43, delta22, delta23

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

! .............. end statement functions


      ierr=0


      ! bcOptionWallNormal= doubleDiv : apply discrete div at -1 and -2
      !                   = divAndDivN : apply div(u)=0 and div(u).n=0
      bcOptionWallNormal=divAndDivN !  doubleDiv ! divAndDivN

      pc                =ipar(0)
      uc                =ipar(1)
      vc                =ipar(2)
      wc                =ipar(3)
      sc                =ipar(4)
      grid              =ipar(5)
      gridType          =ipar(6)
      orderOfAccuracy   =ipar(7)
      gridIsMoving      =ipar(8)
      useWhereMask      =ipar(9)
      gridIsImplicit    =ipar(10)
      implicitMethod    =ipar(11)
      implicitOption    =ipar(12)
      isAxisymmetric    =ipar(13)
      use2ndOrderAD     =ipar(14)
      use4thOrderAD     =ipar(15)
      twilightZone      =ipar(16)
      numberOfProcessors=ipar(17)
      outflowOption     =ipar(18)
      orderOfExtrapolationForOutflow=ipar(19) ! new *wdh* 100827 -- finish me --
      debug             =ipar(20)
      myid              =ipar(21)
      assignTemperature =ipar(22)
      tc                =ipar(23)


!     advectPassiveScalar=ipar(16)

      dx(0)             =rpar(0)
      dx(1)             =rpar(1)
      dx(2)             =rpar(2)
      dr(0)             =rpar(3)
      dr(1)             =rpar(4)
      dr(2)             =rpar(5)
      nu                =rpar(6)
      t                 =rpar(7)
      ad21              =rpar(8)
      ad22              =rpar(9)
      ad41              =rpar(10)
      ad42              =rpar(11)
      nuPassiveScalar   =rpar(12)
      adcPassiveScalar  =rpar(13)
      ajs               =rpar(14)
      gravity(0)        =rpar(15)
      gravity(1)        =rpar(16)
      gravity(2)        =rpar(17)
      thermalExpansivity=rpar(18)

      ep                =rpar(19) ! pointer for exact solution

      epsX = 1.e-30 ! fix me -- pass in

      if( .true. )then
        write(*,'("Inside insTractionBC nd=",i2," tz=",i2",t=",e9.2)') 
     & nd,twilightZone,t
      end if


      extra=0
      numberOfGhostPoints=1
      ! ================= START LOOP OVER SIDES ===============================
       ! *NOTE: extra is not used yet -- keep for future 
       extra1a=extra
       extra1b=extra
       extra2a=extra
       extra2b=extra
       if( nd.eq.3 )then
         extra3a=extra
         extra3b=extra
       else
         extra3a=0
         extra3b=0
       end if
       if( bc(0,0).lt.0 )then
         extra1a=max(0,extra1a) ! over-ride extra=-1 : assign ends in periodic directions (or internal parallel boundaries)
       else if( bc(0,0).eq.0 )then
         extra1a=numberOfGhostPoints  ! include interpolation points since we assign ghost points outside these
       end if
       ! **NOTE** the bc on the right may be negative even it is not on the left (for parallel)
       if( bc(1,0).lt.0 )then
         extra1b=max(0,extra1b) ! over-ride extra=-1 : assign ends in periodic directions
       else if( bc(1,0).eq.0 )then
         extra1b=numberOfGhostPoints
       end if
       if( bc(0,1).lt.0 )then
         extra2a=max(0,extra2a) ! over-ride extra=-1 : assign ends in periodic directions (or internal parallel boundaries)
       else if( bc(0,1).eq.0 )then
         extra2a=numberOfGhostPoints  ! include interpolation points since we assign ghost points outside these
       end if
       ! **NOTE** the bc on the right may be negative even it is not on the left (for parallel)
       if( bc(1,1).lt.0 )then
         extra2b=max(0,extra2b) ! over-ride extra=-1 : assign ends in periodic directions
       else if( bc(1,1).eq.0 )then
         extra2b=numberOfGhostPoints
       end if
       if(  nd.eq.3 )then
        if( bc(0,2).lt.0 )then
          extra3a=max(0,extra3a) ! over-ride extra=-1 : assign ends in periodic directions (or internal parallel boundaries)
        else if( bc(0,2).eq.0 )then
          extra3a=numberOfGhostPoints  ! include interpolation points since we assign ghost points outside these
        end if
        ! **NOTE** the bc on the right may be negative even it is not on the left (for parallel)
        if( bc(1,2).lt.0 )then
          extra3b=max(0,extra3b) ! over-ride extra=-1 : assign ends in periodic directions
        else if( bc(1,2).eq.0 )then
          extra3b=numberOfGhostPoints
        end if
       end if
       do axis=0,nd-1
       do side=0,1
         if( bc(side,axis).eq.tractionFree .or. bc(side,axis)
     & .eq.freeSurfaceBoundaryCondition )then
           ! write(*,'(" insTractionBC: nd,side,axis,bc=",4i4)') nd,side,axis,bc(side,axis)
           n1a=gridIndexRange(0,0)
           n1b=gridIndexRange(1,0)
           n2a=gridIndexRange(0,1)
           n2b=gridIndexRange(1,1)
           n3a=gridIndexRange(0,2)
           n3b=gridIndexRange(1,2)
           if( axis.eq.0 )then
             n1a=gridIndexRange(side,axis)
             n1b=gridIndexRange(side,axis)
           else if( axis.eq.1 )then
             n2a=gridIndexRange(side,axis)
             n2b=gridIndexRange(side,axis)
           else
             n3a=gridIndexRange(side,axis)
             n3b=gridIndexRange(side,axis)
           end if
           nn1a=gridIndexRange(0,0)-extra1a
           nn1b=gridIndexRange(1,0)+extra1b
           nn2a=gridIndexRange(0,1)-extra2a
           nn2b=gridIndexRange(1,1)+extra2b
           nn3a=gridIndexRange(0,2)-extra3a
           nn3b=gridIndexRange(1,2)+extra3b
           if( axis.eq.0 )then
             nn1a=gridIndexRange(side,axis)
             nn1b=gridIndexRange(side,axis)
           else if( axis.eq.1 )then
             nn2a=gridIndexRange(side,axis)
             nn2b=gridIndexRange(side,axis)
           else
             nn3a=gridIndexRange(side,axis)
             nn3b=gridIndexRange(side,axis)
           end if
           is=1-2*side
           is1=0
           is2=0
           is3=0
           if( axis.eq.0 )then
             is1=1-2*side
           else if( axis.eq.1 )then
             is2=1-2*side
           else if( axis.eq.2 )then
             is3=1-2*side
           else
             stop 5
           end if
           axisp1=mod(axis+1,nd)
           axisp2=mod(axis+2,nd)
           i3=n3a
           if( debug.gt.7 )then
             write(*,'(" insTractionBC: grid,side,axis=",3i3,", loop 
     & bounds: n1a,n1b,n2a,n2b,n3a,n3b=",6i3)') grid,side,axis,n1a,
     & n1b,n2a,n2b,n3a,n3b
           end if
         ! On interfaces we should use the bcf array values even for TZ since then
         ! we get a coupling at the interface: 
         !   bcf = n.sigma(fluid) + [ n.sigma_e(solid) - n.sigma_e(fluid) ]
         !-   if( interfaceType(side,axis,grid).eq.noInterface )then
         !-     assignTwilightZone=twilightZone
         !-   else
         !-    assignTwilightZone=0  ! this will turn off the use of TZ
         !-   end if

       if( nd.eq.2 )then
         ! --- 2D TRACTION  ----
        if( gridType.eq.0 )then
          ! --- RECTANGULAR ----
          if( orderOfAccuracy.eq.2 )then
            if( axis.eq.0 )then
              write(*,'("START TRACTION FREE LOOPS")')
              f1=0.
              f2=0.
              f3=0.
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
               i1m=i1-is1
               i2m=i2-is2
               i3m=i3-is3
               i1p=i1+is1
               i2p=i2+is2
               i3p=i3+is3
                ! --------------------------------------------------------------
                ! ----------------- 2D Traction Rectangular --------------------
                ! --------------------------------------------------------------
                 ! ux = - vy
                 ! vx = 0    
                 ! Note: we could instead use uxx = 0 ( = -vxy) 
                 if( twilightZone.eq.1 )then
                   ! assume the TZ solution is divergence free
                   call ogDeriv(ep,0,1,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),
     & 0.,t,vc,vxe )
                   ! call ogDeriv(ep,0,0,0,0,x(i1m,i2m,i3m,0),x(i1m,i2m,i3m,1),0.,t,vc,ve )
                   f2 = -is*2.*dx(axis)*vxe
                 end if
                 ! write(*,'(" i1,i2,i3=",3i3," i1m,i2m,i3m=",3i3," i1p,i2p,i3p=",2i3)') i1,i2,i3,i1m,i2m,i3m,i1p,i2p,i3p
                 !  write(*,'(" i1,i2=",2i3," vp,vm=",2f8.4," (vp-vm)/(2dx)=",f8.4)') i1,i2,u(i1p,i2p,i3p,vc),u(i1m,i2m,i3m,vc),(u(i1p,i2p,i3p,vc)-u(i1m,i2m,i3m,vc))/(2.*dx(axis))
                 u(i1m,i2m,i3m,uc)= u(i1p,i2p,i3p,uc) + is*2.*dx(axis)*
     & uy22r(i1,i2,i3,vc)
                 u(i1m,i2m,i3m,vc)= u(i1p,i2p,i3p,vc) + f2
                 ! write(*,'(" i1,i2=",2i3," dx,vxe=",2f8.4," vm,vem=",2f8.4)') i1,i2,dx(axis),vxe,u(i1m,i2m,i3m,vc),ve
              end do
              end do
              end do
            else if( axis.eq.1 )then
              write(*,'("START TRACTION FREE LOOPS")')
              f1=0.
              f2=0.
              f3=0.
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
               i1m=i1-is1
               i2m=i2-is2
               i3m=i3-is3
               i1p=i1+is1
               i2p=i2+is2
               i3p=i3+is3
                ! --------------------------------------------------------------
                ! ----------------- 2D Traction Rectangular --------------------
                ! --------------------------------------------------------------
                 ! uy =0 
                 ! vy = - ux
                 if( twilightZone.eq.1 )then
                   ! assume the TZ solution is divergence free
                   call ogDeriv(ep,0,0,1,0,x(i1,i2,i3,0),x(i1,i2,i3,1),
     & 0.,t,uc,uye )
                   f1 = -is*2.*dx(axis)*uye
                 end if
                 u(i1m,i2m,i3m,uc)= u(i1p,i2p,i3p,uc) + f1
                 u(i1m,i2m,i3m,vc)= u(i1p,i2p,i3p,vc) + is*2.*dx(axis)*
     & ux22r(i1,i2,i3,uc)
              end do
              end do
              end do
            else
              stop 4444
            end if
          else if( orderOfAccuracy.eq.4 )then
            stop 3333
          else
           stop 1234
          end if

        else if( gridType.eq.1 )then
          ! --- CURVILINEAR ----

         if( orderOfAccuracy.eq.2 )then

          do i3=n3a,n3b
          do i2=n2a,n2b
          do i1=n1a,n1b
           i1m=i1-is1
           i2m=i2-is2
           i3m=i3-is3
           i1p=i1+is1
           i2p=i2+is2
           i3p=i3+is3
           ! *************** TRACTION BC CURVILINEAR GRIDS ****************
           ! (rxd,ryd) : direction of the normal to r(axis)=const
           rxd = rsxy(i1,i2,i3,  axis,0)
           ryd = rsxy(i1,i2,i3,  axis,1)
           sxd = rsxy(i1,i2,i3,axisp1,0)
           syd = rsxy(i1,i2,i3,axisp1,1)
            an1 = rsxy(i1,i2,i3,axis,0)
            an2 = rsxy(i1,i2,i3,axis,1)
            aNormi = -is/max(epsX,sqrt(an1**2 + an2**2))
            an1=an1*aNormi
            an2=an2*aNormi
           ! tangent
           t1=-an2
           t2= an1
           ux = ux22(i1,i2,i3,uc)
           uy = uy22(i1,i2,i3,uc)
           vx = ux22(i1,i2,i3,vc)
           vy = uy22(i1,i2,i3,vc)
           ! crxd = coeff of u(-1) in u.x  
           ! cryd = coeff of u(-1) in u.y  
           crxd=-is*rxd/(2.*dr(axis))
           cryd=-is*ryd/(2.*dr(axis))
           ! First evaluate div(u) using current ghost values 
           !   f1 = ux+vy = a11*u(-1) + a12*v(-1) + rest
           !   rest = f1(uCurrent) - a11*uCurrent(-1) + a12*vCurrent(-1)
           f1 = ux+vy
           a11 = -is*rxd/(2.*dr(axis))
           a12 = -is*ryd/(2.*dr(axis))
           ! First evaluate the zero tangential traction equation using current ghost values 
           !  f2 = (1/mu) * tv.tauv.nv 
           !     =  2*ux t1*n1 + (uy+vx)*(t1*n2+t2*n1) + 2* t2*n2* vy 
           !     = csf1*ux + csf2*(uy+vx) + csf3*vy
           !     = a21*u(-1) + a22*v(-1) + .... = f2
           csf1= 2.*t1*an1
           csf2=(t1*an2+t2*an1)
           csf3= 2.*t2*an2
           f2 = csf1*ux + csf2*(uy+vx) + csf3*vy
           if( twilightZone.eq.1 )then
             ! assume the TZ solution is divergence free so we do not need to change f1
             call ogDeriv(ep,0,1,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),0.,t,
     & uc,uxe )
             call ogDeriv(ep,0,1,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),0.,t,
     & vc,vxe )
             call ogDeriv(ep,0,0,1,0,x(i1,i2,i3,0),x(i1,i2,i3,1),0.,t,
     & uc,uye )
             call ogDeriv(ep,0,0,1,0,x(i1,i2,i3,0),x(i1,i2,i3,1),0.,t,
     & vc,vye )
             ! Adjust for TZ:  
             f2 = f2 - (csf1*uxe + csf2*(uye+vxe) + csf3*vye)
           end if
           a21 = csf1*crxd + csf2*cryd
           a22 = csf2*crxd + csf3*cryd
           b1 = a11*u(i1m,i2m,i3m,uc) + a12*u(i1m,i2m,i3m,vc) - f1
           b2 = a21*u(i1m,i2m,i3m,uc) + a22*u(i1m,i2m,i3m,vc) - f2
           ! write(*,'(" i1,i2=",2i3," rxd,ryd=",2f8.4," sxd,syd=",2f10.4)') i1,i2,rxd,ryd,sxd,syd
           ! write(*,'(" i1,i2=",2i3," a11,a12=",2f8.4," a21,a22=",2f10.4)') i1,i2,a11,a12,a21,a22
           ! Solve
           !   [a11 a12 ][ u(-1)] = [ b1 ]
           !   [a21 a22 ][ v(-1)] = [ b2 ]
           !   
           det=a11*a22-a12*a21
           if( abs(det)<epsX )then
             write(*,'("InsTractionBC: ERROR: det<epsX !")')
             stop 6754
           endif
           um1 =( a22*b1-a12*b2)/det
           vm1 =(-a21*b1+a11*b2)/det
           u(i1m,i2m,i3m,uc)=um1
           u(i1m,i2m,i3m,vc)=vm1
          end do
          end do
          end do

         else
           stop 4455
         end if

        else

          stop 1111
        end if

       else
         ! ---- 3D TRACTION ----
        if( gridType.eq.0 )then
          ! --- RECTANGULAR ----
          if( orderOfAccuracy.eq.2 )then
            if( axis.eq.0 )then
              write(*,'("START TRACTION FREE LOOPS")')
              f1=0.
              f2=0.
              f3=0.
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
               i1m=i1-is1
               i2m=i2-is2
               i3m=i3-is3
               i1p=i1+is1
               i2p=i2+is2
               i3p=i3+is3
                ! --------------------------------------------------------------
                ! ----------------- 3D Traction Rectangular --------------------
                ! --------------------------------------------------------------
                 ! ux = - (vy + wz)
                 ! vx = 0    
                 ! wx = 0    
                 ! Note: we could instead use uxx = 0 ( = -vxy-wxy) 
                 if( twilightZone.eq.1 )then
                   ! assume the TZ solution is divergence free
                   call ogDeriv(ep,0,1,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),
     & x(i1,i2,i3,2),t,vc,vxe )
                   call ogDeriv(ep,0,1,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),
     & x(i1,i2,i3,2),t,wc,wxe )
                   call ogDeriv(ep,0,0,0,0,x(i1m,i2m,i3m,0),x(i1m,i2m,
     & i3m,1),x(i1m,i2m,i3m,2),t,vc,ve )
                   f2 = -is*2.*dx(axis)*vxe
                   f3 = -is*2.*dx(axis)*wxe
                 end if
                ! write(*,'("x: i1,i2,i3=",3i3," i1m,i2m,i3m=",3i3," i1p,i2p,i3p=",3i3)') i1,i2,i3,i1m,i2m,i3m,i1p,i2p,i3p
                ! write(*,'(" i1,i2,i3=",3i3," vp,vm=",2f8.4," (vp-vm)/(2dx)=",f8.4)') i1,i2,i3,u(i1p,i2p,i3p,vc),u(i1m,i2m,i3m,vc),(u(i1p,i2p,i3p,vc)-u(i1m,i2m,i3m,vc))/(2.*dx(axis))
                 u(i1m,i2m,i3m,uc)= u(i1p,i2p,i3p,uc) + is*2.*dx(axis)*
     & (uy23r(i1,i2,i3,vc)+uz23r(i1,i2,i3,wc))
                 u(i1m,i2m,i3m,vc)= u(i1p,i2p,i3p,vc) + f2
                 u(i1m,i2m,i3m,wc)= u(i1p,i2p,i3p,wc) + f3
                 ! if( abs(ve-u(i1m,i2m,i3m,vc)).gt. 1.e-5 )then
                 !   write(*,'("************ TROUBLE *********")') 
                 !   write(*,'(" i1,i2,i3=",3i3," dx,vxe=",2f8.4," vm,vem=",2f8.4)') i1,i2,i3,dx(axis),vxe,u(i1m,i2m,i3m,vc),ve
                 ! end if
              end do
              end do
              end do

            else if( axis.eq.1 )then
              write(*,'("CALL TRACTION FREE Y")')
              write(*,'("START TRACTION FREE LOOPS")')
              f1=0.
              f2=0.
              f3=0.
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
               i1m=i1-is1
               i2m=i2-is2
               i3m=i3-is3
               i1p=i1+is1
               i2p=i2+is2
               i3p=i3+is3
                ! --------------------------------------------------------------
                ! ----------------- 3D Traction Rectangular --------------------
                ! --------------------------------------------------------------
                 ! uy =0 
                 ! vy = -(ux+wz)
                 ! wy = 0
                 if( twilightZone.eq.1 )then
                   ! assume the TZ solution is divergence free
                   call ogDeriv(ep,0,0,1,0,x(i1,i2,i3,0),x(i1,i2,i3,1),
     & x(i1,i2,i3,2),t,uc,uye )
                   call ogDeriv(ep,0,0,1,0,x(i1,i2,i3,0),x(i1,i2,i3,1),
     & x(i1,i2,i3,2),t,wc,wye )
                   f1 = -is*2.*dx(axis)*uye
                   f3 = -is*2.*dx(axis)*wye
                 end if
                 !write(*,'("y: i1,i2,i3=",3i3," i1m,i2m,i3m=",3i3," i1p,i2p,i3p=",3i3)') i1,i2,i3,i1m,i2m,i3m,i1p,i2p,i3p
                 !write(*,'("y: side,axis=",2i3)') side,axis
                 u(i1m,i2m,i3m,uc)= u(i1p,i2p,i3p,uc) + f1
                 u(i1m,i2m,i3m,vc)= u(i1p,i2p,i3p,vc) + is*2.*dx(axis)*
     & (ux23r(i1,i2,i3,uc)+uz23r(i1,i2,i3,wc))
                 u(i1m,i2m,i3m,wc)= u(i1p,i2p,i3p,wc) + f3
              end do
              end do
              end do

            else if( axis.eq.2 )then
              write(*,'("START TRACTION FREE LOOPS")')
              f1=0.
              f2=0.
              f3=0.
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
               i1m=i1-is1
               i2m=i2-is2
               i3m=i3-is3
               i1p=i1+is1
               i2p=i2+is2
               i3p=i3+is3
                ! --------------------------------------------------------------
                ! ----------------- 3D Traction Rectangular --------------------
                ! --------------------------------------------------------------
                 ! uz =0 
                 ! vz = 0
                 ! wz = -(ux+vy)
                 if( twilightZone.eq.1 )then
                   ! assume the TZ solution is divergence free
                   call ogDeriv(ep,0,0,0,1,x(i1,i2,i3,0),x(i1,i2,i3,1),
     & x(i1,i2,i3,2),t,uc,uze )
                   call ogDeriv(ep,0,0,0,1,x(i1,i2,i3,0),x(i1,i2,i3,1),
     & x(i1,i2,i3,2),t,vc,vze )
                   f1 = -is*2.*dx(axis)*uze
                   f2 = -is*2.*dx(axis)*vze
                 end if
                 u(i1m,i2m,i3m,uc)= u(i1p,i2p,i3p,uc) + f1
                 u(i1m,i2m,i3m,vc)= u(i1p,i2p,i3p,vc) + f2
                 u(i1m,i2m,i3m,wc)= u(i1p,i2p,i3p,wc) + is*2.*dx(axis)*
     & (ux23r(i1,i2,i3,uc)+uy23r(i1,i2,i3,vc))
              end do
              end do
              end do
            else
              stop 4444
            end if
          else if( orderOfAccuracy.eq.4 )then
            stop 3333
          else
           stop 1234
          end if

        else if( gridType.eq.1 )then
          ! --- CURVILINEAR ----

          f1e=0.
          f2e=0.
          f3e=0.
          do i3=n3a,n3b
          do i2=n2a,n2b
          do i1=n1a,n1b
           i1m=i1-is1
           i2m=i2-is2
           i3m=i3-is3
           !i1p=i1+is1
           !i2p=i2+is2
           !i3p=i3+is3
           ! *************** TRACTION BC CURVILINEAR GRIDS ****************
           ! (rxd,ryd) : direction of the normal to r(axis)=const
           rxd = rsxy(i1,i2,i3,  axis,0)
           ryd = rsxy(i1,i2,i3,  axis,1)
           rzd = rsxy(i1,i2,i3,  axis,2)
           !sxd = rsxy(i1,i2,i3,axisp1,0)
           !syd = rsxy(i1,i2,i3,axisp1,1)
           !szd = rsxy(i1,i2,i3,axisp1,2)
           !txd = rsxy(i1,i2,i3,axisp2,0)
           !tyd = rsxy(i1,i2,i3,axisp2,1)
           !tzd = rsxy(i1,i2,i3,axisp2,2)
            an1 = rsxy(i1,i2,i3,axis,0)
            an2 = rsxy(i1,i2,i3,axis,1)
            an3 = rsxy(i1,i2,i3,axis,2)
            aNormi = -is/max(epsX,sqrt(an1**2 + an2**2+ an3**2))
            an1=an1*aNormi
            an2=an2*aNormi
            an3=an3*aNormi
           ux = ux23(i1,i2,i3,uc)
           uy = uy23(i1,i2,i3,uc)
           uz = uz23(i1,i2,i3,uc)
           vx = ux23(i1,i2,i3,vc)
           vy = uy23(i1,i2,i3,vc)
           vz = uz23(i1,i2,i3,vc)
           wx = ux23(i1,i2,i3,wc)
           wy = uy23(i1,i2,i3,wc)
           wz = uz23(i1,i2,i3,wc)
           ! divergence 
           div=ux+vy+wz
           ! traction vector = [tvx,tvy,tvz] (without mu)
           tvx = (2.*ux)*an1 + (uy+vx)*an2 + (uz+wx)*an3
           tvy = (uy+vx)*an1 + (2.*vy)*an2 + (vz+wy)*an3
           tvz = (uz+wx)*an1 + (vz+wy)*an2 + (2.*wz)*an3
           ! tvx = c11*u(-1) + c12*v(-1) + c13*w(-1)
           c11 = ( 2.*rxd*an1 +ryd*an2 + rzd*an3 )*(-is/(2.*dr(axis)))
           c12 = (             rxd*an2           )*(-is/(2.*dr(axis)))
           c13 = (                       rxd*an3 )*(-is/(2.*dr(axis)))
           ! tvy = c21*u(-1) + c22*v(-1) + c23*w(-1)
           c21 = ( ryd*an1                       )*(-is/(2.*dr(axis)))
           c22 = ( rxd*an1 +2.*ryd*an2 + rzd*an3 )*(-is/(2.*dr(axis)))
           c23 = (                       ryd*an3 )*(-is/(2.*dr(axis)))
           ! tvz = c31*u(-1) + c32*v(-1) + c33*w(-1)
           c31 = ( rzd*an1                       )*(-is/(2.*dr(axis)))
           c32 = (            rzd*an2            )*(-is/(2.*dr(axis)))
           c33 = ( rxd*an1  + ryd*an2+2.*rzd*an3 )*(-is/(2.*dr(axis)))
           ! 3 Equations are 
           !  fv = [f1, f2, f3 ] = div * nv + [1-nv nv^T] tv = 0
           ! Evaluate equations using current ghost values: 
           f1 = div*an1 + (1.-an1*an1)*tvx      -an1*an2* tvy     -an1*
     & an3 *tvz
           f2 = div*an2      -an2*an1 *tvx + (1.-an2*an2)*tvy     -an2*
     & an3 *tvz
           f3 = div*an3      -an3*an1 *tvx      -an3*an2 *tvy +(1.-an3*
     & an3)*tvz
           ! determine a(i,j): (coefficients of ghost in equations f1,f2,f3)
           ! f1 = a11*u(-1) + a12*v(-1) + a13*w(-1) + .....
           ! f2 = a21*u(-1) + a22*v(-1) + a23*w(-1) + .....
           ! f3 = a31*u(-1) + a32*v(-1) + a33*w(-1) + .....
           ! div = d11*u(-1) + d12*v(-1) + d13*w(-1)
           div11 = -is*rxd/(2.*dr(axis))
           div12 = -is*ryd/(2.*dr(axis))
           div13 = -is*rzd/(2.*dr(axis))
           a11 = div11*an1 + (1.-an1*an1)*c11      -an1*an2* c21     -
     & an1*an3 *c31 ! coeff of u(-1) in f1
           a12 = div12*an1 + (1.-an1*an1)*c12      -an1*an2* c22     -
     & an1*an3 *c32 ! coeff of v(-1) in f1
           a13 = div13*an1 + (1.-an1*an1)*c13      -an1*an2* c23     -
     & an1*an3 *c33 ! coeff of w(-1) in f1
           a21 = div11*an2      -an2*an1 *c11 + (1.-an2*an2)*c21     -
     & an2*an3 *c31
           a22 = div12*an2      -an2*an1 *c12 + (1.-an2*an2)*c22     -
     & an2*an3 *c32
           a23 = div13*an2      -an2*an1 *c13 + (1.-an2*an2)*c23     -
     & an2*an3 *c33
           a31 = div11*an3      -an3*an1 *c11      -an3*an2 *c21 +(1.-
     & an3*an3)*c31
           a32 = div12*an3      -an3*an1 *c12      -an3*an2 *c22 +(1.-
     & an3*an3)*c32
           a33 = div13*an3      -an3*an1 *c13      -an3*an2 *c23 +(1.-
     & an3*an3)*c33
           ! current values on the ghost
           um1 = u(i1m,i2m,i3m,uc)
           vm1 = u(i1m,i2m,i3m,vc)
           wm1 = u(i1m,i2m,i3m,wc)
           ! right hand sides to A x = b 
           b1 = a11*um1 + a12*vm1 + a13*wm1 -f1
           b2 = a21*um1 + a22*vm1 + a23*wm1 -f2
           b3 = a31*um1 + a32*vm1 + a33*wm1 -f3
           if( twilightZone.eq.1 )then
             ! ---- adjust RHS for TZ  ----
             ! assume the TZ solution is divergence free so we do not need to change f1
             call ogDeriv(ep,0,1,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,
     & i2,i3,2),t,uc,uxe )
             call ogDeriv(ep,0,1,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,
     & i2,i3,2),t,vc,vxe )
             call ogDeriv(ep,0,1,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,
     & i2,i3,2),t,wc,wxe )
             call ogDeriv(ep,0,0,1,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,
     & i2,i3,2),t,uc,uye )
             call ogDeriv(ep,0,0,1,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,
     & i2,i3,2),t,vc,vye )
             call ogDeriv(ep,0,0,1,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,
     & i2,i3,2),t,wc,wye )
             call ogDeriv(ep,0,0,0,1,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,
     & i2,i3,2),t,uc,uze )
             call ogDeriv(ep,0,0,0,1,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,
     & i2,i3,2),t,vc,vze )
             call ogDeriv(ep,0,0,0,1,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,
     & i2,i3,2),t,wc,wze )
             dive = uxe+vye+wze
             tvxe = ( 2.*uxe)*an1 + (uye+vxe)*an2 + (uze+wxe)*an3
             tvye = (uye+vxe)*an1 + ( 2.*vye)*an2 + (vze+wye)*an3
             tvze = (uze+wxe)*an1 + (vze+wye)*an2 + ( 2.*wze)*an3
             f1e = dive*an1 + (1.-an1*an1)*tvxe      -an1*an2* tvye    
     &  -an1*an3 *tvze
             f2e = dive*an2      -an2*an1 *tvxe + (1.-an2*an2)*tvye    
     &  -an2*an3 *tvze
             f3e = dive*an3      -an3*an1 *tvxe      -an3*an2 *tvye +(
     & 1.-an3*an3)*tvze
             b1 = b1 + f1e
             b2 = b2 + f2e
             b3 = b3 + f3e
           end if
           ! write(*,'(" i1,i2=",2i3," rxd,ryd=",2f8.4," sxd,syd=",2f10.4)') i1,i2,rxd,ryd,sxd,syd
           ! write(*,'(" i1,i2=",2i3," a11,a12=",2f8.4," a21,a22=",2f10.4)') i1,i2,a11,a12,a21,a22
           ! Solve
           !   [a11 a12 a13][ u(-1)] = [ b1 ]
           !   [a21 a22 a23][ v(-1)] = [ b2 ]
           !   [a31 a32 a33][ w(-1)] = [ b3 ]
           !   
           ! ***** check me
           det=a33*a11*a22-a33*a12*a21-a13*a31*a22+a31*a23*a12+a13*a32*
     & a21-a32*a23*a11
           if( abs(det)<epsX )then
             write(*,'("InsTractionBC: ERROR: det<epsX !")')
             stop 3439
           endif
           ! solve by Cramers *check me*
           um1=( a33*b1*a22-a13*b3*a22+a13*a32*b2+b3*a23*a12-a32*a23*
     & b1-a33*a12*b2)/det
           vm1=(-a23*a11*b3+a23*b1*a31+a11*a33*b2+a13*a21*b3-b1*a33*
     & a21-a13*b2*a31)/det
           wm1=( a11*b3*a22-a11*a32*b2-a12*a21*b3+a12*b2*a31-b1*a31*
     & a22+b1*a32*a21)/det
            if( .true. )then
            ! check answer
            res1 = a11*um1 + a12*vm1 + a13*wm1 -b1
            res2 = a21*um1 + a22*vm1 + a23*wm1 -b2
            res3 = a31*um1 + a32*vm1 + a33*wm1 -b3
            fMax = max(abs(res1),abs(res2),abs(res3))
            if( fMax.gt.1.e-10 )then
              write(*,'(" *** TROUBLE SOLVING LINEAR SYSTEM:  *****")')
              write(*,'(" i1,i2,i3=",3i3," res1,res2,res3=",3e9.2)') 
     & i1,i2,i3,res1,res2,res3
            end if
           end if
           u(i1m,i2m,i3m,uc)=um1
           u(i1m,i2m,i3m,vc)=vm1
           u(i1m,i2m,i3m,wc)=wm1
           if( .true. )then
            ! check answer
            ux = ux23(i1,i2,i3,uc)
            uy = uy23(i1,i2,i3,uc)
            uz = uz23(i1,i2,i3,uc)
            vx = ux23(i1,i2,i3,vc)
            vy = uy23(i1,i2,i3,vc)
            vz = uz23(i1,i2,i3,vc)
            wx = ux23(i1,i2,i3,wc)
            wy = uy23(i1,i2,i3,wc)
            wz = uz23(i1,i2,i3,wc)
            ! divergence 
            div=ux+vy+wz
            ! traction vector = [tvx,tvy,tvz] (without mu)
            tvx = (2.*ux)*an1 + (uy+vx)*an2 + (uz+wx)*an3
            tvy = (uy+vx)*an1 + (2.*vy)*an2 + (vz+wy)*an3
            tvz = (uz+wx)*an1 + (vz+wy)*an2 + (2.*wz)*an3
            ! Evaluate equations using current ghost values: 
            f1 = div*an1 + (1.-an1*an1)*tvx      -an1*an2* tvy     -
     & an1*an3 *tvz
            f2 = div*an2      -an2*an1 *tvx + (1.-an2*an2)*tvy     -
     & an2*an3 *tvz
            f3 = div*an3      -an3*an1 *tvx      -an3*an2 *tvy +(1.-
     & an3*an3)*tvz
            res1 = f1 - f1e
            res2 = f2 - f2e
            res3 = f3 - f3e
            resMax = max(abs(res1),abs(res2),abs(res3))
            if( resMax.gt.1.e-10 )then
              write(*,'(" i1,i2,i3=",3i3," res1,res2,res3=",3e9.2)') 
     & i1,i2,i3,res1,res2,res3
              write(*,'(" *** TROUBLE  WITH EQUATIONS *****")')
            end if
           end if
          end do
          end do
          end do

        else
          ! unknown gridType 
          stop 2222
        end if
       end if


        end if ! end if tractionFree or freeSurface BC
       end do ! end side
       end do ! end axis
      ! ================= END LOOP OVER SIDES ===============================


      return
      end


