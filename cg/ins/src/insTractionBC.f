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
      logical useWallBC,useOutflowBC
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
      real f1,f2,f3,a11,a12,a21,a22,det,alpha,beta,rxsq,rxsqr,ajs
      real rxd,ryd,rzd,sxd,syd,szd,txd,tyd,tzd,rxsqd
      real rxi,ryi,rzi,sxi,syi,szi,txi,tyi,tzi,rxxi,ryyi,rzzi
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
           write(*,'(" insTractionBC: nd,side,axis,bc=",4i4)') nd,side,
     & axis,bc(side,axis)
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
           if( .true. .or. debug.gt.7 )then
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
          stop 0909

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

          stop 0909

        else

          stop 2222
        end if
       end if


        end if ! end if tractionFree or freeSurface BC
       end do ! end side
       end do ! end axis
      ! ================= END LOOP OVER SIDES ===============================

!-       ! for fourth-order dissipation:
!-       cd42=ad42/(nd**2)
!-       ! cd42=0. ! for testing
!-       adCoeff4=0.
!- 
!-       ! For second-order dissipation:
!-       cd22=ad22/(nd**2)
!-       adCoeff2=0.
!- 
!-       if( .false. .and. use4thOrderAD.ne.0 .and. t.le.0. )then
!-         write(*,'(" insbc4: t=",e10.2," use4thOrderAD=",i2," ad41,ad42=",2e10.2," outflowOption=",i2)') t,use4thOrderAD,ad41,ad42,outflowOption
!-       end if
!-       if( .false. .and. use2ndOrderAD.ne.0  .and. t.le.0. )then
!-         write(*,'(" insbc4: t=",e10.2," use2ndOrderAD=",i2," ad21,ad22=",2e10.2)') t,use2ndOrderAD,ad21,ad22
!-       end if
!- 
!- !       i1=2
!- !       i2=2
!- !       i3=0
!- !       write(*,*) 'insbc4: x,y,u,err = ',x(i1,i2,i3,0),x(i1,i2,i3,1),ogf(exact,x(i1,i2,i3,0),x(i1,i2,i3,1),0.,uc,t),!- !                                     u(i1,i2,i3,uc)-ogf(exact,x(i1,i2,i3,0),x(i1,i2,i3,1),0.,uc,t)
!- 
!-       ! if( t.le.0.0001 .and. gridIsMoving.ne.0 .and. mod(bcOption,2).eq.1 )then
!-       !   write(*,'("insbc4: *** Moving grids is on ***")')
!-       ! end if
!- 
!-       if( outflowOption.ne.0 .and. outflowOption.ne.1 )then
!-         write(*,'("insbc4: ERROR: unexpected outflowOption=",i6)') outflowOption
!-         stop 1706
!-       end if
!- 
!-       if( assignTemperature.ne.0 .and. tc.lt.0 .or. tc.gt.10000 )then
!-         write(*,'("insbc4: ERROR: assignTemperature.ne.0 but tc=",i6)') tc
!-         stop 1744
!-       end if
!- 
!- 
!-       if( mod(bcOption,2).eq.1  )then
!- 
!-       ! *************************************************************
!-       ! ********Update ghost pts outside interpolation points********
!-       ! *************************************************************
!- 
!- 
!-       ! We cannot apply the standard BC's to get the points marked 'E' below
!-       ! where the boundary points of a grid are interpolated
!-       !  i2=0   ----I----I----X----X----X------------------------
!-       !  i2=-1  ----E----E----G----G----G
!-       !  i2=-2  ----E----E----G----G----G
!- 
!-       ! Include ghost points on interpolation boundaries
!-       do axis=0,2
!-       do side=0,1
!-         is=1-2*side
!-         if( axis.lt.nd .and. bc(side,axis).eq.0 )then
!-           nr(side,axis)=indexRange(side,axis)-2*is
!-         else
!-           nr(side,axis)=indexRange(side,axis)
!-         end if
!-       end do
!-       end do
!- 
!-       ! write(*,'(''*** insbc4 grid='',i4,'' nr='',6i3,'' bc='',6i3)') grid,nr,bc
!- 
!-       do kd1=0,nd-1
!-       do ks1=0,1
!-         nr0=nr(0,kd1)  ! save these values
!-         nr1=nr(1,kd1)
!-         nr(0,kd1)=indexRange(ks1,kd1)
!-         nr(1,kd1)=nr(0,kd1)
!-         bc1=bc(ks1,kd1)
!-         is=1-2*ks1
!-         if( bc1.eq.noSlipWall .or. bc1.eq.outflow .or. bc1.eq.inflowWithVelocityGiven )then
!-    
!-           ! For now extrapolate these points
!-           ! We could do better -- on a noSlipWall we could use u.x=0 or v.y=0
!-           if( kd1.eq.0 )then
!-             loopse4NoMask(if( mask(i1,i2,i3).lt.0 )then,!-                           $extrapTwoGhost(5,r),!-                           end if,)
!-           else if( kd1.eq.1 )then
!-             loopse4NoMask(if( mask(i1,i2,i3).lt.0 )then,!-                           $extrapTwoGhost(5,s),!-                           end if,)
!-           else
!-             loopse4NoMask(if( mask(i1,i2,i3).lt.0 )then,!-                           $extrapTwoGhost(5,t),!-                           end if,)
!-           end if     
!-         end if
!-         ! reset
!-         nr(0,kd1)=nr0
!-         nr(1,kd1)=nr1
!-       end do
!-       end do
!- 
!-       do axis=0,2
!-       do side=0,1
!-          nr(side,axis)=indexRange(side,axis)
!-       end do
!-       end do
!- 
!-       ! *************************************************************
!-       ! *****************Update extended boundaries*****************
!-       ! *************************************************************
!-       do kd1=0,nd-1
!-       do ks1=0,1
!-        bc1=bc(ks1,kd1)
!-        if( bc1.eq.slipWall .or. bc1.eq.outflow .or. bc1.eq.inflowWithVelocityGiven )then
!- 	! In some cases we may need to assign values on the ghost points on the extended boundary
!-         ! For a noSlipWall these values are already set (u=0)
!-         !
!-         !                |                      |
!-         !                |                      |
!-         !      X----X----|----------------------|----X----X
!-         !                |                      |
!-         !                |                      |
!- 
!- 
!-         nr(0,kd1)=indexRange(ks1,kd1)
!-         nr(1,kd1)=nr(0,kd1)
!- 
!- 	do kd2=0,nd-1
!- 	if( kd2.ne.kd1 )then
!- 	do ks2=0,1
!-           bc2=bc(ks2,kd2)
!- 
!-           nr(0,kd2)=indexRange(ks2,kd2)
!-           nr(1,kd2)=nr(0,kd2)
!-           
!-           is=1-2*ks2
!- 
!-           if( bc1.eq.slipWall .and. ( bc2.eq.outflow .or. bc2.eq.inflowWithVelocityGiven) )then
!-             !  On the slip wall ghost points solve for the normal components:
!-             !       u.x + v.y = 0
!-             !      D+^p ( n.u ) = 0
!- 		
!-             !  printf(" Set points (%i,%i,%i),(%i,%i,%i) where slip wall meets outflow\n",
!-             !                  i1+is1,i2+is2,i3,i1+2*is1,i2+2*is2,i3)
!- 	    
!-             !  u.x+v.y=0
!-             !  D+4(u)=0
!-             if( bc2.eq.outflow .and. outflowOption.eq.neumannAtOuflow )then
!-             ! kkc 110311 added this adjustment for the special case of neumannAtOutflow
!-                is1=0
!-                is2=0
!-                is3=0
!-                if( kd2.eq.0 )then
!-                   is1=is
!-                else if( kd2.eq.1 )then
!-                   is2=is
!-                else
!-                   is3=is
!-                end if
!-                loopse4($boundaryConditionNeumannOutflow(none,2),,,)
!- 
!-             else 
!-              if( gridType.eq.rectangular )then
!- 
!-               if( nd.eq.2 )then
!-                 if( kd2.eq.0 )then
!- 	          loopse4(u(i1-is,i2,i3,uc)=-1.5*u(i1,i2,i3,uc)+3.*u(i1+is,i2,i3,uc)-.5*u(i1+2*is,i2,i3,uc)!- 	                +is*.25*dx(0)*12.*uy42r(i1,i2,i3,vc),!-                           u(i1-2*is,i2,i3,uc)=4.*(u(i1-is,i2,i3,uc)+u(i1+is,i2,i3,uc))-6.*u(i1,i2,i3,uc)-u(i1+2*is,i2,i3,uc),,)
!-                 else 
!- 	          loopse4(u(i1,i2-is,i3,vc)=-1.5*u(i1,i2,i3,vc)+3.*u(i1,i2+is,i3,vc)-.5*u(i1,i2+2*is,i3,vc)!-                           +is*.25*dx(1)*12.*ux42r(i1,i2,i3,uc),!-                           u(i1,i2-2*is,i3,vc)=4.*(u(i1,i2-is,i3,vc)+u(i1,i2+is,i3,vc))-6.*u(i1,i2,i3,vc)-u(i1,i2+2*is,i3,vc),,)
!-                 end if
!-               else ! 3D
!-                 if( kd2.eq.0 )then
!- 	          loopse4(u(i1-is,i2,i3,uc)=-1.5*u(i1,i2,i3,uc)+3.*u(i1+is,i2,i3,uc)-.5*u(i1+2*is,i2,i3,uc)!- 	                +is*.25*dx(0)*12.*(uy43r(i1,i2,i3,vc)+uz43r(i1,i2,i3,wc)),!-                           u(i1-2*is,i2,i3,uc)=4.*(u(i1-is,i2,i3,uc)+u(i1+is,i2,i3,uc))-6.*u(i1,i2,i3,uc)-u(i1+2*is,i2,i3,uc),,)
!-                 else if( kd2.eq.1 )then
!- 	          loopse4(u(i1,i2-is,i3,vc)=-1.5*u(i1,i2,i3,vc)+3.*u(i1,i2+is,i3,vc)-.5*u(i1,i2+2*is,i3,vc)!- 	                +is*.25*dx(1)*12.*(ux43r(i1,i2,i3,uc)+uz43r(i1,i2,i3,wc)),!-                          u(i1,i2-2*is,i3,vc)=4.*(u(i1,i2-is,i3,vc)+u(i1,i2+is,i3,vc))-6.*u(i1,i2,i3,vc)-u(i1,i2+2*is,i3,vc),,)
!-                 else
!- 	          loopse4(u(i1,i2,i3-is,wc)=-1.5*u(i1,i2,i3,wc)+3.*u(i1,i2,i3+is,wc)-.5*u(i1,i2,i3+2*is,wc)!- 	                +is*.25*dx(2)*12.*(ux43r(i1,i2,i3,uc)+uy43r(i1,i2,i3,vc)),!-                           u(i1,i2,i3-2*is,wc)=4.*(u(i1,i2,i3-is,wc)+u(i1,i2,i3+is,wc))-6.*u(i1,i2,i3,wc)-u(i1,i2,i3+2*is,wc),,)
!-                 end if
!-               end if
!- 
!-              else ! curvilinear
!- 
!-               extrapOrder=5
!-  	      if( extrapOrder.eq.5 )then
!-                 extrapolate(5)
!-               else
!- 	        write(*,*) 'insbc4:ERROR'
!-                 stop 3
!-               end if
!- 
!-               if( nd.eq.2 )then
!-                if( kd2.eq.0 )then
!-                  loopse4($divAndExtrap(r,2),,,)
!-                else 
!-                  loopse4($divAndExtrap(s,2),,,)
!-                end if
!-               else ! 3d
!-                if( kd2.eq.0 )then
!-                  loopse4($divAndExtrap(r,3),,,)
!-                else if( kd2.eq.1 )then
!-                  loopse4($divAndExtrap(s,3),,,)
!-                else
!-                  loopse4($divAndExtrap(t,3),,,)
!-                end if
!-               end if
!-              end if
!- 
!-             end if ! end if block for neumannAtOutflow option
!- 
!-           else if( (bc1.eq.outflow .and. (bc2.eq.outflow .or. bc2.eq.noSlipWall)) .or. bc1.eq.inflowWithVelocityGiven )then
!- 
!-             ! printf(" Set points (%i,%i,%i),(%i,%i,%i) on outflow extended boundary...\n",
!-             !     //                 i1+is,i2+is2,i3,i1+2*is1,i2+2*is2,i3)
!- 		
!-             ! if( bc1.eq.inflowWithVelocityGiven )then
!-             !  write(*,'('' Set extended inflow boundary, nr='',6i3)') nr
!-             ! end if
!- 
!-             ! write(*,*) 'Set outflow extended boundary, nr=',nr
!-             extrapOrder=5
!- 	    if( extrapOrder.eq.5 )then
!-               extrapolate(5)
!-             else
!- 	      write(*,*) 'insbc4:ERROR'
!-               stop 3
!-             end if
!-               		
!- 	  else 
!-           end if
!-           nr(0,kd2)=indexRange(0,kd2) ! reset
!-           nr(1,kd2)=indexRange(1,kd2)
!- 
!-         end do
!-         end if
!-         end do
!- 
!-         nr(0,kd1)=indexRange(0,kd1) ! reset
!-         nr(1,kd1)=indexRange(1,kd1)
!-        end if
!-       end do
!-       end do  
!-       end if ! update extended boundaries
!- 
!-       ! ...Get values outside corners in 2D,3D and edges in 3D using values on the extended boundary
!-       !      and values in the interior
!-       !      The corner or edge is labelled as (kd1,ks1),(kd2,ks2)
!-       if( mod(bcOption/2,2).eq.1 )then
!-       if( gridType.eq.curvilinear )then
!-         do axis=0,2
!-           d14v(axis)=1./(12.*dr(axis))
!-           d24v(axis)=1./(12.*dr(axis)**2)
!-         end do
!-       else
!-         do axis=0,2
!-           d14v(axis)=1./(12.*dx(axis))
!-           d24v(axis)=1./(12.*dx(axis)**2)
!-         end do
!-       end if
!-       do kd1=0,nd-2
!-       do kd2=kd1+1,nd-1
!-       do ks1=0,1
!-       do ks2=0,1
!- 
!-         if( bc(ks1,kd1).gt.0 .and. bc(ks2,kd2).gt.0 )then
!-           if( .true. )then
!-             ! new version 
!-             call inscr4( kd1+1,ks1+1,kd2+1,ks2+1,nd,indexRange,bc,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, !-                       ipar,rpar,u,t,d14v,d24v,ajs,x,rsxy,gridType )
!-           else
!-             call inscr( kd1+1,ks1+1,kd2+1,ks2+1,nd,indexRange,bc,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, !-                       u,t,d14v,d24v,ajs,x,rsxy )
!-           end if
!-         end if
!- 
!-       end do
!-       end do
!-       end do
!-       end do
!- 
!-       end if ! update corners 
!- 
!- 
!-       do axis=0,2
!-       do side=0,1
!-          nr(side,axis)=indexRange(side,axis)
!-       end do
!-       end do
!- 
!-       ! ***********************************************************
!-       ! ***********Assign the tangential components****************
!-       ! ***********************************************************
!-       if( mod(bcOption/4,2).eq.1 )then
!- 
!-       do axis=0,nd-1
!-        do kd=0,nd-1
!-        do side=0,1
!-          nr(side,kd)=indexRange(side,kd)
!- 	 if( kd.ne.axis .and. 
!-      &       (bc(side,kd).eq.noSlipWall .or.
!-      &        bc(side,kd).eq.inflowWithVelocityGiven .or.
!-      &        bc(side,kd).eq.slipWall) )then
!- 
!-            ! If the adjacent BC is a noSlipWall or inflow or slipWall then we do not need to assign
!-            ! ghost points on extended boundaries because these have already been assigned (e.g. u=0 for a noSlipWall)
!-         
!-            nr(side,kd)=nr(side,kd)+1-2*side   
!-          end if
!-        end do
!-        end do
!-        do side=0,1
!- 
!-         is=1-2*side
!-         nr(0,axis)=indexRange(side,axis)
!-         nr(1,axis)=nr(0,axis)
!- 
!- 
!- 
!-         useWallBC = bc(side,axis).eq.noSlipWall .or. bc(side,axis).eq.inflowWithVelocityGiven
!-         useOutflowBC = bc(side,axis).eq.outflow 
!- 
!-         if( .not.useWallBC .and. .not.useOutflowBC .and. bc(side,axis).ne.slipWall .and. !-             bc(side,axis).gt.0 .and. bc(side,axis).ne.dirichletBoundaryCondition .and. !-             bc(side,axis).ne.penaltyBoundaryCondition .and. bc(side,axis).ne.inflowWithPandTV )then
!-           write(*,*) 'insbc4:ERROR: unknown boundary condition=',bc(side,axis)
!-           stop 6
!-         end if
!- 
!- 
!-         ! Tangential components:
!-         !   Wall:
!-         !     Use equation plus extrapolation
!-         !   Outflow:
!-         !     outflowOption=0:
!-         !       D+D_(t.u(0)) = 0 and ((D+)^6)u(-2) = 0 
!-         !     outflowOption=1: (*wdh* 100613)
!-         !       
!-         !
!-         !
!- 
!-         if( useOutflowBC .and. outflowOption.eq.neumannAtOuflow )then
!-           ! Apply a Neumman like condition at outflow (Good for where there might be inflow locally)
!-           is1=0
!-           is2=0
!-           is3=0
!-           if( axis.eq.0 )then
!-            is1=is
!-           else if( axis.eq.1 )then
!-            is2=is
!-           else
!-            is3=is
!-           end if
!-           if( t.le.0 .and. debug.gt.3 )then
!-             if( myid.le.0 )then
!-               write(*,'("insbc4: apply neumman outflow: side,axis,grid=",3i4," at t=",e10.2)') side,axis,grid,t
!-             end if
!-           end if
!-           if( nd.eq.2 )then
!-             if( twilightZone.eq.0 )then
!-               loopse4($boundaryConditionNeumannOutflow(none,2),,,) 
!-             else
!-               loopse4($boundaryConditionNeumannOutflow(tz,2),,,) 
!-             end if
!-           else
!-             if( twilightZone.eq.0 )then
!-               loopse4($boundaryConditionNeumannOutflow(none,3),,,) 
!-             else
!-               loopse4($boundaryConditionNeumannOutflow(tz,3),,,) 
!-             end if
!-           end if
!-         end if
!- 
!-         if( gridType.eq.rectangular )then
!- 
!-           if( axis.eq.0 )then
!-             if( nd.eq.2 )then
!-               if( twilightZone.eq.0 )then
!-                 if( useWallBC )then
!-                   loopse4($boundaryConditionNavierStokesAndExtrapRectangular(r,none,2),,,)
!-                 else if( useOutflowBC )then
!-                  if( outflowOption.eq.extrapolateOutflow )then
!-                    loopse4($boundaryCondition2ndDifferenceAndExtrapRectangular(r,none,2),,,)
!-                  else if( outflowOption.eq.neumannAtOuflow )then
!-                    ! done above
!-                  else
!-                   write(*,'("insbc4: unknown outflowOption=",i6)') outflowOption
!-                   stop 5105
!-                  end if
!-                 end if
!-               else
!-                 if( useWallBC )then
!-                   loopse4($boundaryConditionNavierStokesAndExtrapRectangular(r,tz,2),,,)
!-                 else if( useOutflowBC )then
!-                  if( outflowOption.eq.extrapolateOutflow )then
!-                   loopse4($boundaryCondition2ndDifferenceAndExtrapRectangular(r,tz,2),,,)
!-                  else if( outflowOption.eq.neumannAtOuflow )then
!-                    ! done above
!-                  else
!-                   write(*,'("insbc4: unknown outflowOption=",i6)') outflowOption
!-                   stop 5105
!-                  end if
!-                 end if
!-               end if
!-             else ! nd==3
!-               if( twilightZone.eq.0 )then
!-                 if( useWallBC )then
!-                   loopse4($boundaryConditionNavierStokesAndExtrapRectangular(r,none,3),,,)
!-                 else if( useOutflowBC )then
!-                  if( outflowOption.eq.extrapolateOutflow )then
!-                   loopse4($boundaryCondition2ndDifferenceAndExtrapRectangular(r,none,3),,,)
!-                  else if( outflowOption.eq.neumannAtOuflow )then
!-                    ! done above
!-                  else
!-                   write(*,'("insbc4: unknown outflowOption=",i6)') outflowOption
!-                   stop 5105
!-                  end if
!-                 end if
!-               else
!-                 if( useWallBC )then
!-                   loopse4($boundaryConditionNavierStokesAndExtrapRectangular(r,tz,3),,,)
!-                 else if( useOutflowBC )then
!-                  if( outflowOption.eq.extrapolateOutflow )then
!-                   loopse4($boundaryCondition2ndDifferenceAndExtrapRectangular(r,tz,3),,,)
!-                  else if( outflowOption.eq.neumannAtOuflow )then
!-                    ! done above
!-                  else
!-                   write(*,'("insbc4: unknown outflowOption=",i6)') outflowOption
!-                   stop 5105
!-                  end if
!-                 end if
!-               end if
!- 
!-             end if
!- 
!-           else if( axis.eq.1 )then
!-             if( nd.eq.2 )then
!-               if( twilightZone.eq.0 )then
!-                 if( useWallBC )then
!-                   loopse4($boundaryConditionNavierStokesAndExtrapRectangular(s,none,2),,,)
!-                 else if( useOutflowBC )then
!-                  if( outflowOption.eq.extrapolateOutflow )then
!-                   loopse4($boundaryCondition2ndDifferenceAndExtrapRectangular(s,none,2),,,)
!-                  else if( outflowOption.eq.neumannAtOuflow )then
!-                    ! done above
!-                  else
!-                   write(*,'("insbc4: unknown outflowOption=",i6)') outflowOption
!-                   stop 5105
!-                  end if
!-                 end if
!-               else
!-                 if( useWallBC )then
!-                   loopse4($boundaryConditionNavierStokesAndExtrapRectangular(s,tz,2),,,)
!-                 else if( useOutflowBC )then
!-                  if( outflowOption.eq.extrapolateOutflow )then
!-                   loopse4($boundaryCondition2ndDifferenceAndExtrapRectangular(s,tz,2),,,)
!-                  else if( outflowOption.eq.neumannAtOuflow )then
!-                    ! done above
!-                  else
!-                   write(*,'("insbc4: unknown outflowOption=",i6)') outflowOption
!-                   stop 5105
!-                  end if
!-                 end if
!-               end if
!-             else ! nd==3
!-               if( twilightZone.eq.0 )then
!-                 if( useWallBC )then
!-                   loopse4($boundaryConditionNavierStokesAndExtrapRectangular(s,none,3),,,)
!-                 else if( useOutflowBC )then
!-                  if( outflowOption.eq.extrapolateOutflow )then
!-                   loopse4($boundaryCondition2ndDifferenceAndExtrapRectangular(s,none,3),,,)
!-                  else if( outflowOption.eq.neumannAtOuflow )then
!-                    ! done above
!-                  else
!-                   write(*,'("insbc4: unknown outflowOption=",i6)') outflowOption
!-                   stop 5105
!-                  end if
!-                 end if
!-               else
!-                 if( useWallBC )then
!-                   loopse4($boundaryConditionNavierStokesAndExtrapRectangular(s,tz,3),,,)
!-                 else if( useOutflowBC )then
!-                  if( outflowOption.eq.extrapolateOutflow )then
!-                   loopse4($boundaryCondition2ndDifferenceAndExtrapRectangular(s,tz,3),,,)
!-                  else if( outflowOption.eq.neumannAtOuflow )then
!-                    ! done above
!-                  else
!-                   write(*,'("insbc4: unknown outflowOption=",i6)') outflowOption
!-                   stop 5105
!-                  end if
!-                 end if
!-               end if
!-             end if
!- 
!-           else ! axis==2
!-             if( twilightZone.eq.0 )then
!-               if( useWallBC )then
!-                 loopse4($boundaryConditionNavierStokesAndExtrapRectangular(t,none,3),,,)
!-               else if( useOutflowBC )then
!-                 if( outflowOption.eq.extrapolateOutflow )then
!-                  loopse4($boundaryCondition2ndDifferenceAndExtrapRectangular(t,none,3),,,)
!-                 else if( outflowOption.eq.neumannAtOuflow )then
!-                   ! done above
!-                 else
!-                  write(*,'("insbc4: unknown outflowOption=",i6)') outflowOption
!-                  stop 5105
!-                 end if
!-               end if
!-             else
!-               if( useWallBC )then
!-                 loopse4($boundaryConditionNavierStokesAndExtrapRectangular(t,tz,3),,,)
!-               else if( useOutflowBC )then
!-                if( outflowOption.eq.extrapolateOutflow )then
!-                 loopse4($boundaryCondition2ndDifferenceAndExtrapRectangular(t,tz,3),,,)
!-                else if( outflowOption.eq.neumannAtOuflow )then
!-                  ! done above
!-                else
!-                 write(*,'("insbc4: unknown outflowOption=",i6)') outflowOption
!-                 stop 5105
!-                end if
!-               end if
!-             end if
!- 
!-           end if
!- 
!-         else ! curvilinear
!- 
!-           ! *************************************************************************
!-           ! *******************  Curvilinear  ***************************************
!-           ! *************************************************************************
!-           if( axis.eq.0 )then
!-             if( nd.eq.2 )then
!-               ! Solve
!-               !   F1(u(-1),u(-2)) = a11.u(-1) + a12.u(-2) + g1 = nu*(u.xx+u.yy) - u*u.x - v*u.y - u.t
!-               !   F2(u(-1),u(-2)) = a21.u(-1) + a22.u(-2) + g2 = D+^m( u(-2) ) 
!-               ! for (u(-1),u(-2)) and (v(-1),v(-2))
!-               ! Then adjust the tangential components
!-               !    \uv <- \uv + (\uv_old-\uv).nv
!-               
!-               ! write(*,*) 'insbc4: curvilinear: assign wall tangential axis=0 wall nr=',nr 
!-               if( twilightZone.eq.0 )then
!-                 if( useWallBC )then
!-                   loopse4($boundaryConditionNavierStokesAndExtrap2d(r,none),,,)                
!-                 else if( useOutflowBC )then
!-                  if( outflowOption.eq.extrapolateOutflow )then
!-                   loopse4($boundaryCondition2ndDifferenceAndExtrap(is,0,0,r,none,2),,,)                
!-                  else if( outflowOption.eq.neumannAtOuflow )then
!-                    ! done above
!-                  else
!-                   write(*,'("insbc4: unknown outflowOption=",i6)') outflowOption
!-                   stop 5105
!-                  end if
!-                 end if
!-               else
!-                 if( useWallBC )then
!-                   loopse4($boundaryConditionNavierStokesAndExtrap2d(r,tz),,,)                
!-                 else if( useOutflowBC )then
!-                  if( outflowOption.eq.extrapolateOutflow )then
!-                   loopse4($boundaryCondition2ndDifferenceAndExtrap(is,0,0,r,tz,2),,,)                
!-                  else if( outflowOption.eq.neumannAtOuflow )then
!-                    ! done above
!-                  else
!-                   write(*,'("insbc4: unknown outflowOption=",i6)') outflowOption
!-                   stop 5105
!-                  end if
!-                 end if
!-               end if
!-             else ! nd==3
!-               if( twilightZone.eq.0 )then
!-                 if( useWallBC )then
!-                   loopse4($boundaryConditionNavierStokesAndExtrap3d(r,none),,,)                
!-                 else if( useOutflowBC )then
!-                  if( outflowOption.eq.extrapolateOutflow )then
!-                   loopse4($boundaryCondition2ndDifferenceAndExtrap(is,0,0,r,none,3),,,)                
!-                  else if( outflowOption.eq.neumannAtOuflow )then
!-                    ! done above
!-                  else
!-                   write(*,'("insbc4: unknown outflowOption=",i6)') outflowOption
!-                   stop 5105
!-                  end if
!-                 end if
!-               else
!-                 if( useWallBC )then
!-                   loopse4($boundaryConditionNavierStokesAndExtrap3d(r,tz),,,)                
!-                 else if( useOutflowBC )then
!-                  if( outflowOption.eq.extrapolateOutflow )then
!-                   loopse4($boundaryCondition2ndDifferenceAndExtrap(is,0,0,r,tz,3),,,)                
!-                  else if( outflowOption.eq.neumannAtOuflow )then
!-                    ! done above
!-                  else
!-                   write(*,'("insbc4: unknown outflowOption=",i6)') outflowOption
!-                   stop 5105
!-                  end if
!-                 end if
!-               end if
!-             end if
!- 
!-           else if( axis.eq.1 )then
!-             if( nd.eq.2 )then
!-               if( twilightZone.eq.0 )then
!-                 if( useWallBC )then
!-                   loopse4($boundaryConditionNavierStokesAndExtrap2d(s,none),,,)                
!-                 else if( useOutflowBC )then
!-                  if( outflowOption.eq.extrapolateOutflow )then
!-                   loopse4($boundaryCondition2ndDifferenceAndExtrap(0,is,0,s,none,2),,,)                
!-                  else if( outflowOption.eq.neumannAtOuflow )then
!-                    ! done above
!-                  else
!-                   write(*,'("insbc4: unknown outflowOption=",i6)') outflowOption
!-                   stop 5105
!-                  end if
!-                 end if
!-               else
!-                 if( useWallBC )then
!-                   loopse4($boundaryConditionNavierStokesAndExtrap2d(s,tz),,,)                
!-                 else if( useOutflowBC )then
!-                  if( outflowOption.eq.extrapolateOutflow )then
!-                   loopse4($boundaryCondition2ndDifferenceAndExtrap(0,is,0,s,tz,2),,,)                
!-                  else if( outflowOption.eq.neumannAtOuflow )then
!-                    ! done above
!-                  else
!-                   write(*,'("insbc4: unknown outflowOption=",i6)') outflowOption
!-                   stop 5105
!-                  end if
!-                 end if
!-               end if
!-             else ! nd==3
!-               if( twilightZone.eq.0 )then
!-                 if( useWallBC )then
!-                   loopse4($boundaryConditionNavierStokesAndExtrap3d(s,none),,,)                
!-                 else if( useOutflowBC )then
!-                  if( outflowOption.eq.extrapolateOutflow )then
!-                   loopse4($boundaryCondition2ndDifferenceAndExtrap(0,is,0,s,none,3),,,)                
!-                  else if( outflowOption.eq.neumannAtOuflow )then
!-                    ! done above
!-                  else
!-                   write(*,'("insbc4: unknown outflowOption=",i6)') outflowOption
!-                   stop 5105
!-                  end if
!-                 end if
!-               else
!-                 if( useWallBC )then
!-                   loopse4($boundaryConditionNavierStokesAndExtrap3d(s,tz),,,)                
!-                 else if( useOutflowBC )then
!-                  if( outflowOption.eq.extrapolateOutflow )then
!-                   loopse4($boundaryCondition2ndDifferenceAndExtrap(0,is,0,s,tz,3),,,)                
!-                  else if( outflowOption.eq.neumannAtOuflow )then
!-                    ! done above
!-                  else
!-                   write(*,'("insbc4: unknown outflowOption=",i6)') outflowOption
!-                   stop 5105
!-                  end if
!-                 end if
!-               end if
!-             end if
!- 
!-           else ! axis==2
!-               if( twilightZone.eq.0 )then
!-                 if( useWallBC )then
!-                   loopse4($boundaryConditionNavierStokesAndExtrap3d(t,none),,,)                
!-                 else if( useOutflowBC )then
!-                  if( outflowOption.eq.extrapolateOutflow )then
!-                   loopse4($boundaryCondition2ndDifferenceAndExtrap(0,0,is,t,none,3),,,)                
!-                  else if( outflowOption.eq.neumannAtOuflow )then
!-                    ! done above
!-                  else
!-                   write(*,'("insbc4: unknown outflowOption=",i6)') outflowOption
!-                   stop 5105
!-                  end if
!-                 end if
!-               else
!-                 if( useWallBC )then
!-                   loopse4($boundaryConditionNavierStokesAndExtrap3d(t,tz),,,)                
!-                 else if( useOutflowBC )then
!-                  if( outflowOption.eq.extrapolateOutflow )then
!-                   loopse4($boundaryCondition2ndDifferenceAndExtrap(0,0,is,t,tz,3),,,)                
!-                  else if( outflowOption.eq.neumannAtOuflow )then
!-                    ! done above
!-                  else
!-                   write(*,'("insbc4: unknown outflowOption=",i6)') outflowOption
!-                   stop 5105
!-                  end if
!-                 end if
!-               end if
!- 
!-           end if
!- 
!-         end if ! end if gridType
!- 
!-       end do
!-       end do
!-       end if
!- 
!- 
!-       ! ***********************************************************
!-       ! **************Assign the normal component******************
!-       ! ***********************************************************
!-       if( mod(bcOption/8,2).eq.1 )then
!- 
!-       
!- 
!-       do axis=0,nd-1
!-        do kd=0,nd-1
!-        do side=0,1
!-          nr(side,kd)=indexRange(side,kd)
!- 	 if( kd.ne.axis .and. 
!-      &       (bc(side,kd).eq.noSlipWall .or.
!-      &        bc(side,kd).eq.inflowWithVelocityGiven .or.
!-      &        bc(side,kd).eq.slipWall) )then
!- 
!-            ! If the adjacent BC is a noSlipWall or inflow or slipWall then we do not need to assign
!-            ! ghost points on extended boundaries because these have already been assigned (e.g. u=0 for a noSlipWall)
!-         
!-            nr(side,kd)=nr(side,kd)+1-2*side   
!-          end if
!-        end do
!-        end do
!-        do side=0,1
!- 
!-         is=1-2*side
!-         nr(0,axis)=indexRange(side,axis)
!-         nr(1,axis)=nr(0,axis)
!- 
!- 
!-         if( bc(side,axis).eq.outflow .and. outflowOption.eq.neumannAtOuflow )then
!-           ! do nothing in this case, Neumann BC's have already been applied above *wdh* 100827
!- 
!-         else if( bc(side,axis).eq.noSlipWall .or. bc(side,axis).eq.inflowWithVelocityGiven .or. !-             bc(side,axis).eq.outflow )then
!- 
!-           ! set 2 ghost lines from div(u)=0
!- 
!-           if( gridType.eq.rectangular )then
!- 
!-             if( axis.eq.0 )then
!-               if( nd.eq.2 )then
!-                if( bcOptionWallNormal.eq.doubleDiv )then
!-                  loopse4(u(i1-  is,i2,i3,uc)=divBCr2d(i1+is),!-                          u(i1-2*is,i2,i3,uc)=divBCr2d(i1),,)
!-                else
!-                 ! u.x = -v.y
!-                 ! u.xx = -v.xy
!-                 ! write(*,*) 'assign axis==0 wall nr=',nr                
!-                 loopse4(vy=uy42r(i1,i2,i3,vc),!-                         vxy=uxy42r(i1,i2,i3,vc),!-                         u(i1-  is,i2,i3,uc)=3.75*u(i1,i2,i3,uc)-3.*u(i1+is,i2,i3,uc)+.25*u(i1+2*is,i2,i3,uc)!-                                            -1.5*(is*dx(0)*vy+dx(0)**2*vxy),!-                         u(i1-2*is,i2,i3,uc)=30.*u(i1,i2,i3,uc)-32.*u(i1+is,i2,i3,uc)+3.*u(i1+2*is,i2,i3,uc)!-                                            -(24.*is*dx(0)*vy+12.*dx(0)**2*vxy))
!-                end if
!-               else ! nd==3
!-                if( bcOptionWallNormal.eq.doubleDiv )then
!-                  loopse4(u(i1-  is,i2,i3,uc)=divBCr3d(i1+is),!-                          u(i1-2*is,i2,i3,uc)=divBCr3d(i1),,)
!-                else
!-                 ! u.x = -v.y-w.z
!-                 ! u.xx = -v.xy-w.xz
!-                 ! write(*,*) 'assign axis==0 wall nr=',nr                
!-                 loopse4(vy=  uy43r(i1,i2,i3,vc)+ uz43r(i1,i2,i3,wc),!-                         vxy=uxy43r(i1,i2,i3,vc)+uxz43r(i1,i2,i3,wc),!-                         u(i1-  is,i2,i3,uc)=3.75*u(i1,i2,i3,uc)-3.*u(i1+is,i2,i3,uc)+.25*u(i1+2*is,i2,i3,uc)!-                                            -1.5*(is*dx(0)*vy+dx(0)**2*vxy),!-                         u(i1-2*is,i2,i3,uc)=30.*u(i1,i2,i3,uc)-32.*u(i1+is,i2,i3,uc)+3.*u(i1+2*is,i2,i3,uc)!-                                            -(24.*is*dx(0)*vy+12.*dx(0)**2*vxy))
!-                end if
!- 
!-               end if
!- 
!-             else if( axis.eq.1 )then
!-               if( nd.eq.2 )then
!-                if( bcOptionWallNormal.eq.doubleDiv )then
!-                 loopse4(u(i1,i2-  is,i3,vc)=divBCs2d(i2+is),!-                         u(i1,i2-2*is,i3,vc)=divBCs2d(i2),,)
!-                else
!-                 ! write(*,*) 'assign axis==1 wall nr=',nr                
!-                 loopse4(ux=ux42r(i1,i2,i3,uc),!-                         uxy=uxy42r(i1,i2,i3,uc),!-                         u(i1,i2-  is,i3,vc)=3.75*u(i1,i2,i3,vc)-3.*u(i1,i2+is,i3,vc)+.25*u(i1,i2+2*is,i3,vc)!-                                            -1.5*(is*dx(1)*ux+dx(1)**2*uxy),!-                         u(i1,i2-2*is,i3,vc)=30.*u(i1,i2,i3,vc)-32.*u(i1,i2+is,i3,vc)+3.*u(i1,i2+2*is,i3,vc)!-                                            -(24.*is*dx(1)*ux+12.*dx(1)**2*uxy))
!-                end if
!-               else ! nd==3
!-                if( bcOptionWallNormal.eq.doubleDiv )then
!-                 loopse4(u(i1,i2-  is,i3,vc)=divBCs3d(i2+is),!-                         u(i1,i2-2*is,i3,vc)=divBCs3d(i2),,)
!-                else
!-                 ! v.y  = -u.x-w.z
!-                 ! v.yy = -u.xy-w.yz
!-                 ! write(*,*) 'assign axis==1 wall nr=',nr                
!-                 loopse4(ux=  ux43r(i1,i2,i3,uc)+ uz43r(i1,i2,i3,wc),!-                         uxy=uxy43r(i1,i2,i3,uc)+uyz43r(i1,i2,i3,wc),!-                         u(i1,i2-  is,i3,vc)=3.75*u(i1,i2,i3,vc)-3.*u(i1,i2+is,i3,vc)+.25*u(i1,i2+2*is,i3,vc)!-                                            -1.5*(is*dx(1)*ux+dx(1)**2*uxy),!-                         u(i1,i2-2*is,i3,vc)=30.*u(i1,i2,i3,vc)-32.*u(i1,i2+is,i3,vc)+3.*u(i1,i2+2*is,i3,vc)!-                                            -(24.*is*dx(1)*ux+12.*dx(1)**2*uxy))
!-                end if
!-               end if
!- 
!-             else ! axis==2
!-                if( bcOptionWallNormal.eq.doubleDiv )then
!-                  loopse4(u(i1,i2,i3,wc-  is)=divBCt3d(i3+is),!-                          u(i1,i2,i3,wc-2*is)=divBCt3d(i3),,)
!-                else
!-                 ! w.z = -u.x-v.y
!-                 ! w.zz =-u.xz-v.yz 
!-                 ! write(*,*) 'assign axis==0 wall nr=',nr                
!-                 loopse4(vy=  ux43r(i1,i2,i3,uc)+ uy43r(i1,i2,i3,vc),!-                         vxy=uxz43r(i1,i2,i3,uc)+uyz43r(i1,i2,i3,vc),!-                         u(i1,i2,i3-  is,wc)=3.75*u(i1,i2,i3,wc)-3.*u(i1,i2,i3+is,wc)+.25*u(i1,i2,i3+2*is,wc)!-                                            -1.5*(is*dx(2)*vy+dx(2)**2*vxy),!-                         u(i1,i2,i3-2*is,wc)=30.*u(i1,i2,i3,wc)-32.*u(i1,i2,i3+is,wc)+3.*u(i1,i2,i3+2*is,wc)!-                                            -(24.*is*dx(2)*vy+12.*dx(2)**2*vxy))
!-                end if
!- 
!-             end if
!- 
!-           else ! curvilinear
!- 
!-             ! *************************************************************************
!-             ! *******************  Curvilinear  ***************************************
!-             ! *************************************************************************
!-             if( axis.eq.0 )then
!-               if( nd.eq.2 )then
!-                if( bcOptionWallNormal.eq.doubleDiv )then
!-                 !* loopse4(u(i1-  is,i2,i3,uc)=divBCr2d(i1+is),!-                 !*         u(i1-2*is,i2,i3,uc)=divBCr2d(i1),,)
!-                else
!-                 ! F1(uv(-1),uv(-2)) = a11.uv(-1) + a12.uv(-2) + g1 = div(u) = rx*ur+sx*us + ry*vr+sy*vs
!-                 ! F2(uv(-1),uv(-2)) = a21.uv(-1) + a22.uv(-2) + g2 = div(u).r =rx*u.rr+rx.r*u.r+...
!-                 !   Choose  uv(-1) =  uv_old(-1) + alpha*(rx,ry)
!-                 !           uv(-2) =  uv_old(-2) + beta *(rx,ry)
!-                 ! So that F1=0 and F2=0 (note: (rx,ry) is parallel to the normal
!-                 !   -> solve for 
!-                 !    a11.(rx,ry)*alpha + a12.(rx,ry)*beta + F1(\uv_old) = 0 
!-                 !    a21.(rx,ry)*alpha + a22.(rx,ry)*beta + F2(\uv_old) = 0 
!-                 
!-                 ! write(*,*) 'insbc4: curvilinear: assign wall normal axis=0 wall nr=',nr   
!-                 loopse4($boundaryConditionDivAndDivN(r),,,)                
!- 
!-                end if
!-               else ! nd==3
!-                if( bcOptionWallNormal.eq.doubleDiv )then
!-                  !* loopse4(u(i1-  is,i2,i3,uc)=divBCr3d(i1+is),!-                  !*         u(i1-2*is,i2,i3,uc)=divBCr3d(i1),,)
!-                else
!-                 ! u.x = -v.y-w.z
!-                 ! u.xx = -v.xy-w.xz
!-                 ! write(*,*) 'assign axis==0 wall nr=',nr                
!- 
!-                 loopse4($boundaryConditionDivAndDivN3d(r),,,)
!-                end if
!- 
!-               end if
!- 
!-             else if( axis.eq.1 )then
!-               if( nd.eq.2 )then
!-                if( bcOptionWallNormal.eq.doubleDiv )then
!-                 !* loopse4(u(i1,i2-  is,i3,vc)=divBCs2d(i2+is),!-                 !*         u(i1,i2-2*is,i3,vc)=divBCs2d(i2),,)
!-                else
!-                 loopse4($boundaryConditionDivAndDivN(s),,,)
!-                end if
!-               else ! nd==3
!-                if( bcOptionWallNormal.eq.doubleDiv )then
!-                 !* loopse4(u(i1,i2-  is,i3,vc)=divBCs3d(i2+is),!-                 !*         u(i1,i2-2*is,i3,vc)=divBCs3d(i2),,)
!-                else
!-                 ! v.y  = -u.x-w.z
!-                 ! v.yy = -u.xy-w.yz
!-                 loopse4($boundaryConditionDivAndDivN3d(s),,,)
!-                end if
!-               end if
!- 
!-             else ! axis==2
!-                if( bcOptionWallNormal.eq.doubleDiv )then
!-                 !*  loopse4(u(i1,i2,i3,wc-  is)=divBCt3d(i3+is),!-                 !*          u(i1,i2,i3,wc-2*is)=divBCt3d(i3),,)
!-                else
!-                 ! w.z = -u.x-v.y
!-                 ! w.zz =-u.xz-v.yz 
!-                 ! write(*,*) 'assign axis==0 wall nr=',nr                
!-                 loopse4($boundaryConditionDivAndDivN3d(t),,,)
!-                end if
!- 
!-             end if
!- 
!- 
!-           end if
!- 
!-         else if( bc(side,axis).ne.slipWall .and. bc(side,axis).gt.0 .and. bc(side,axis).ne.dirichletBoundaryCondition .and. bc(side,axis).ne.penaltyBoundaryCondition .and. bc(side,axis).ne.inflowWithPandTV )then
!- 
!-           write(*,*) 'insbc4:ERROR: unknown boundary condition=',bc(side,axis)
!-           stop 6
!-         end if
!- 
!-       end do
!-       end do
!-       end if

      return
      end


