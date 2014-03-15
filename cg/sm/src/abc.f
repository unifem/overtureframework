! This file automatically generated from abc.bf with bpp.
c *******************************************************************************
c   Absorbing boundary conditions
c *******************************************************************************

c These next include files will define the macros that will define the difference approximations
c The actual macro is called below
c Use this next macro to declare the statement functions that are defined below
c To include derivatives of rx use OPTION=RX


c Define statement functions for difference approximations of order 2 
c To include derivatives of rx use OPTION=RX
c To include derivatives of rx use OPTION=RX



c Use this next macro to declare the statement functions that are defined below
c To include derivatives of rx use OPTION=RX


c Define statement functions for difference approximations of order 4 
c To include derivatives of rx use OPTION=RX
c To include derivatives of rx use OPTION=RX




c ************************************************************************************************
c  This macro is used for looping over the faces of a grid to assign booundary conditions
c
c extra: extra points to assign
c          Case 1: extra=numberOfGhostPoints -- for assigning extended boundaries
c          Case 2: extra=-1 -- for assigning ghost points but not including extended boundaries
c numberOfGhostPoints : number of ghost points (1 for 2nd order, 2 for fourth-order ...)
c ***********************************************************************************************


c ========================================================================
c Begin loop over edges in 3D
c ========================================================================



! ABC - Engquist Majda order 2
! This is only a first order in time approx.
! Generalized form:
! u.xt = c1abcem2*u.xx + c2abcem2*( u.yy + u.zz )
!   Taylor: p0=1 p2=-1/2
!   Cheby:  p0=1.00023, p2=-.515555



! Here is a 2nd-order in time approx








      subroutine abcSolidMechanics( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     & ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,gridIndexRange, u, un, f,
     & mask,rsxy, xy,bc, boundaryCondition, ipar, rpar, ierr )
c ===================================================================================
c  Absorbing boundary conditions for Solid Mechanics
c
c  gridType : 0=rectangular, 1=curvilinear
c  useForcing : 1=use f for RHS to BC
c  side,axis : 0:1 and 0:2
c
c  u : solution at time t-dt
c  un : solution at time t (apply BC to this solution)
c
c ===================================================================================

      implicit none

      integer nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, ndf1a,ndf1b,ndf2a,
     & ndf2b,ndf3a,ndf3b,n1a,n1b,n2a,n2b,n3a,n3b, ndc, bc,ierr

      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
      real un(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
      real f(ndf1a:ndf1b,ndf2a:ndf2b,ndf3a:ndf3b,0:*)
      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
      real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
      integer gridIndexRange(0:1,0:2)

      integer ipar(0:*),boundaryCondition(0:1,0:2)
      real rpar(0:*)

c     --- local variables ----

      integer side,axis,gridType,orderOfAccuracy,orderOfExtrapolation,
     & useForcing,ex,ey,ez,hx,hy,hz,useWhereMask,grid,debug,side1,
     & side2,side3
      real dx(0:2),dr(0:2),t,ep,dt,c
      real dxa,dya,dza
      integer axisp1,axisp2,i1,i2,i3,is1,is2,is3,js1,js2,js3,ks1,ks2,
     & ks3
      integer extra,extra1a,extra1b,extra2a,extra2b,extra3a,extra3b,
     & numberOfGhostPoints
      integer edgeDirection,sidea,sideb,sidec,bc1,bc2

      real p0,p2,q0,q2,c1abcem2,c2abcem2

      ! boundary conditions parameters
! define BC parameters for fortran routines
! boundary conditions
c123456789012345678901234567890123456789012345678901234567890123456789
      integer interpolation,displacementBC,tractionBC
      integer slipWall,symmetry,interfaceBC
      integer abcEM2,abcPML,abc3,abc4,abc5,rbcNonLocal,rbcLocal,lastBC
      integer dirichletBoundaryCondition
      parameter( interpolation=0,displacementBC=1,tractionBC=2)
      parameter( slipWall=3,symmetry=4 )
      parameter( interfaceBC=5,abcEM2=6,abcPML=7,abc3=8,abc4=9 )
      parameter( abc5=10,rbcNonLocal=11,rbcLocal=12 )
      parameter( dirichletBoundaryCondition=13 )
      parameter( lastBC=14 )
! define interfaceType values for fortran routines
      integer noInterface                     ! no interface conditions are imposed
      integer heatFluxInterface               ! [ T.n ] = g
      integer tractionInterface               ! [ n.tau ] = g
      integer tractionAndHeatFluxInterface
      parameter( noInterface=0, heatFluxInterface=1 )
      parameter( tractionInterface=2,tractionAndHeatFluxInterface=3 )

      integer rectangular,curvilinear
      parameter(rectangular=0,curvilinear=1)


c     --- start statement function ----
      integer kd,m,n
      real rx,ry,rz,sx,sy,sz,tx,ty,tz
      ! include 'declareDiffOrder2f.h'
      ! include 'declareDiffOrder4f.h'
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
       real d14
       real d24
       real h41
       real h42
       real rxr4
       real rxs4
       real rxt4
       real ryr4
       real rys4
       real ryt4
       real rzr4
       real rzs4
       real rzt4
       real sxr4
       real sxs4
       real sxt4
       real syr4
       real sys4
       real syt4
       real szr4
       real szs4
       real szt4
       real txr4
       real txs4
       real txt4
       real tyr4
       real tys4
       real tyt4
       real tzr4
       real tzs4
       real tzt4
       real rxx41
       real rxx42
       real rxy42
       real rxx43
       real rxy43
       real rxz43
       real ryx42
       real ryy42
       real ryx43
       real ryy43
       real ryz43
       real rzx42
       real rzy42
       real rzx43
       real rzy43
       real rzz43
       real sxx42
       real sxy42
       real sxx43
       real sxy43
       real sxz43
       real syx42
       real syy42
       real syx43
       real syy43
       real syz43
       real szx42
       real szy42
       real szx43
       real szy43
       real szz43
       real txx42
       real txy42
       real txx43
       real txy43
       real txz43
       real tyx42
       real tyy42
       real tyx43
       real tyy43
       real tyz43
       real tzx42
       real tzy42
       real tzx43
       real tzy43
       real tzz43
       real ur4
       real us4
       real ut4
       real urr4
       real uss4
       real utt4
       real urs4
       real urt4
       real ust4
       real ux41
       real uy41
       real uz41
       real ux42
       real uy42
       real uz42
       real ux43
       real uy43
       real uz43
       real uxx41
       real uyy41
       real uxy41
       real uxz41
       real uyz41
       real uzz41
       real ulaplacian41
       real uxx42
       real uyy42
       real uxy42
       real uxz42
       real uyz42
       real uzz42
       real ulaplacian42
       real uxx43
       real uyy43
       real uzz43
       real uxy43
       real uxz43
       real uyz43
       real ulaplacian43
       real ux43r
       real uy43r
       real uz43r
       real uxx43r
       real uyy43r
       real uzz43r
       real uxy43r
       real uxz43r
       real uyz43r
       real ux41r
       real uy41r
       real uz41r
       real uxx41r
       real uyy41r
       real uzz41r
       real uxy41r
       real uxz41r
       real uyz41r
       real ulaplacian41r
       real ux42r
       real uy42r
       real uz42r
       real uxx42r
       real uyy42r
       real uzz42r
       real uxy42r
       real uxz42r
       real uyz42r
       real ulaplacian42r
       real ulaplacian43r

c.......statement functions for jacobian
      rx(i1,i2,i3)=rsxy(i1,i2,i3,0,0)
      ry(i1,i2,i3)=rsxy(i1,i2,i3,0,1)
      rz(i1,i2,i3)=rsxy(i1,i2,i3,0,2)
      sx(i1,i2,i3)=rsxy(i1,i2,i3,1,0)
      sy(i1,i2,i3)=rsxy(i1,i2,i3,1,1)
      sz(i1,i2,i3)=rsxy(i1,i2,i3,1,2)
      tx(i1,i2,i3)=rsxy(i1,i2,i3,2,0)
      ty(i1,i2,i3)=rsxy(i1,i2,i3,2,1)
      tz(i1,i2,i3)=rsxy(i1,i2,i3,2,2)


c     The next macro call will define the difference approximation statement functions
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
      d14(kd) = 1./(12.*dr(kd))
      d24(kd) = 1./(12.*dr(kd)**2)
      ur4(i1,i2,i3,kd)=(8.*(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))-(u(i1+2,
     & i2,i3,kd)-u(i1-2,i2,i3,kd)))*d14(0)
      us4(i1,i2,i3,kd)=(8.*(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))-(u(i1,
     & i2+2,i3,kd)-u(i1,i2-2,i3,kd)))*d14(1)
      ut4(i1,i2,i3,kd)=(8.*(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))-(u(i1,
     & i2,i3+2,kd)-u(i1,i2,i3-2,kd)))*d14(2)
      urr4(i1,i2,i3,kd)=(-30.*u(i1,i2,i3,kd)+16.*(u(i1+1,i2,i3,kd)+u(
     & i1-1,i2,i3,kd))-(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd)) )*d24(0)
      uss4(i1,i2,i3,kd)=(-30.*u(i1,i2,i3,kd)+16.*(u(i1,i2+1,i3,kd)+u(
     & i1,i2-1,i3,kd))-(u(i1,i2+2,i3,kd)+u(i1,i2-2,i3,kd)) )*d24(1)
      utt4(i1,i2,i3,kd)=(-30.*u(i1,i2,i3,kd)+16.*(u(i1,i2,i3+1,kd)+u(
     & i1,i2,i3-1,kd))-(u(i1,i2,i3+2,kd)+u(i1,i2,i3-2,kd)) )*d24(2)
      urs4(i1,i2,i3,kd)=(8.*(ur4(i1,i2+1,i3,kd)-ur4(i1,i2-1,i3,kd))-(
     & ur4(i1,i2+2,i3,kd)-ur4(i1,i2-2,i3,kd)))*d14(1)
      urt4(i1,i2,i3,kd)=(8.*(ur4(i1,i2,i3+1,kd)-ur4(i1,i2,i3-1,kd))-(
     & ur4(i1,i2,i3+2,kd)-ur4(i1,i2,i3-2,kd)))*d14(2)
      ust4(i1,i2,i3,kd)=(8.*(us4(i1,i2,i3+1,kd)-us4(i1,i2,i3-1,kd))-(
     & us4(i1,i2,i3+2,kd)-us4(i1,i2,i3-2,kd)))*d14(2)
      rxr4(i1,i2,i3)=(8.*(rx(i1+1,i2,i3)-rx(i1-1,i2,i3))-(rx(i1+2,i2,
     & i3)-rx(i1-2,i2,i3)))*d14(0)
      rxs4(i1,i2,i3)=(8.*(rx(i1,i2+1,i3)-rx(i1,i2-1,i3))-(rx(i1,i2+2,
     & i3)-rx(i1,i2-2,i3)))*d14(1)
      rxt4(i1,i2,i3)=(8.*(rx(i1,i2,i3+1)-rx(i1,i2,i3-1))-(rx(i1,i2,i3+
     & 2)-rx(i1,i2,i3-2)))*d14(2)
      ryr4(i1,i2,i3)=(8.*(ry(i1+1,i2,i3)-ry(i1-1,i2,i3))-(ry(i1+2,i2,
     & i3)-ry(i1-2,i2,i3)))*d14(0)
      rys4(i1,i2,i3)=(8.*(ry(i1,i2+1,i3)-ry(i1,i2-1,i3))-(ry(i1,i2+2,
     & i3)-ry(i1,i2-2,i3)))*d14(1)
      ryt4(i1,i2,i3)=(8.*(ry(i1,i2,i3+1)-ry(i1,i2,i3-1))-(ry(i1,i2,i3+
     & 2)-ry(i1,i2,i3-2)))*d14(2)
      rzr4(i1,i2,i3)=(8.*(rz(i1+1,i2,i3)-rz(i1-1,i2,i3))-(rz(i1+2,i2,
     & i3)-rz(i1-2,i2,i3)))*d14(0)
      rzs4(i1,i2,i3)=(8.*(rz(i1,i2+1,i3)-rz(i1,i2-1,i3))-(rz(i1,i2+2,
     & i3)-rz(i1,i2-2,i3)))*d14(1)
      rzt4(i1,i2,i3)=(8.*(rz(i1,i2,i3+1)-rz(i1,i2,i3-1))-(rz(i1,i2,i3+
     & 2)-rz(i1,i2,i3-2)))*d14(2)
      sxr4(i1,i2,i3)=(8.*(sx(i1+1,i2,i3)-sx(i1-1,i2,i3))-(sx(i1+2,i2,
     & i3)-sx(i1-2,i2,i3)))*d14(0)
      sxs4(i1,i2,i3)=(8.*(sx(i1,i2+1,i3)-sx(i1,i2-1,i3))-(sx(i1,i2+2,
     & i3)-sx(i1,i2-2,i3)))*d14(1)
      sxt4(i1,i2,i3)=(8.*(sx(i1,i2,i3+1)-sx(i1,i2,i3-1))-(sx(i1,i2,i3+
     & 2)-sx(i1,i2,i3-2)))*d14(2)
      syr4(i1,i2,i3)=(8.*(sy(i1+1,i2,i3)-sy(i1-1,i2,i3))-(sy(i1+2,i2,
     & i3)-sy(i1-2,i2,i3)))*d14(0)
      sys4(i1,i2,i3)=(8.*(sy(i1,i2+1,i3)-sy(i1,i2-1,i3))-(sy(i1,i2+2,
     & i3)-sy(i1,i2-2,i3)))*d14(1)
      syt4(i1,i2,i3)=(8.*(sy(i1,i2,i3+1)-sy(i1,i2,i3-1))-(sy(i1,i2,i3+
     & 2)-sy(i1,i2,i3-2)))*d14(2)
      szr4(i1,i2,i3)=(8.*(sz(i1+1,i2,i3)-sz(i1-1,i2,i3))-(sz(i1+2,i2,
     & i3)-sz(i1-2,i2,i3)))*d14(0)
      szs4(i1,i2,i3)=(8.*(sz(i1,i2+1,i3)-sz(i1,i2-1,i3))-(sz(i1,i2+2,
     & i3)-sz(i1,i2-2,i3)))*d14(1)
      szt4(i1,i2,i3)=(8.*(sz(i1,i2,i3+1)-sz(i1,i2,i3-1))-(sz(i1,i2,i3+
     & 2)-sz(i1,i2,i3-2)))*d14(2)
      txr4(i1,i2,i3)=(8.*(tx(i1+1,i2,i3)-tx(i1-1,i2,i3))-(tx(i1+2,i2,
     & i3)-tx(i1-2,i2,i3)))*d14(0)
      txs4(i1,i2,i3)=(8.*(tx(i1,i2+1,i3)-tx(i1,i2-1,i3))-(tx(i1,i2+2,
     & i3)-tx(i1,i2-2,i3)))*d14(1)
      txt4(i1,i2,i3)=(8.*(tx(i1,i2,i3+1)-tx(i1,i2,i3-1))-(tx(i1,i2,i3+
     & 2)-tx(i1,i2,i3-2)))*d14(2)
      tyr4(i1,i2,i3)=(8.*(ty(i1+1,i2,i3)-ty(i1-1,i2,i3))-(ty(i1+2,i2,
     & i3)-ty(i1-2,i2,i3)))*d14(0)
      tys4(i1,i2,i3)=(8.*(ty(i1,i2+1,i3)-ty(i1,i2-1,i3))-(ty(i1,i2+2,
     & i3)-ty(i1,i2-2,i3)))*d14(1)
      tyt4(i1,i2,i3)=(8.*(ty(i1,i2,i3+1)-ty(i1,i2,i3-1))-(ty(i1,i2,i3+
     & 2)-ty(i1,i2,i3-2)))*d14(2)
      tzr4(i1,i2,i3)=(8.*(tz(i1+1,i2,i3)-tz(i1-1,i2,i3))-(tz(i1+2,i2,
     & i3)-tz(i1-2,i2,i3)))*d14(0)
      tzs4(i1,i2,i3)=(8.*(tz(i1,i2+1,i3)-tz(i1,i2-1,i3))-(tz(i1,i2+2,
     & i3)-tz(i1,i2-2,i3)))*d14(1)
      tzt4(i1,i2,i3)=(8.*(tz(i1,i2,i3+1)-tz(i1,i2,i3-1))-(tz(i1,i2,i3+
     & 2)-tz(i1,i2,i3-2)))*d14(2)
      ux41(i1,i2,i3,kd)= rx(i1,i2,i3)*ur4(i1,i2,i3,kd)
      uy41(i1,i2,i3,kd)=0
      uz41(i1,i2,i3,kd)=0
      ux42(i1,i2,i3,kd)= rx(i1,i2,i3)*ur4(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & us4(i1,i2,i3,kd)
      uy42(i1,i2,i3,kd)= ry(i1,i2,i3)*ur4(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & us4(i1,i2,i3,kd)
      uz42(i1,i2,i3,kd)=0
      ux43(i1,i2,i3,kd)=rx(i1,i2,i3)*ur4(i1,i2,i3,kd)+sx(i1,i2,i3)*us4(
     & i1,i2,i3,kd)+tx(i1,i2,i3)*ut4(i1,i2,i3,kd)
      uy43(i1,i2,i3,kd)=ry(i1,i2,i3)*ur4(i1,i2,i3,kd)+sy(i1,i2,i3)*us4(
     & i1,i2,i3,kd)+ty(i1,i2,i3)*ut4(i1,i2,i3,kd)
      uz43(i1,i2,i3,kd)=rz(i1,i2,i3)*ur4(i1,i2,i3,kd)+sz(i1,i2,i3)*us4(
     & i1,i2,i3,kd)+tz(i1,i2,i3)*ut4(i1,i2,i3,kd)
      rxx41(i1,i2,i3)= rx(i1,i2,i3)*rxr4(i1,i2,i3)
      rxx42(i1,i2,i3)= rx(i1,i2,i3)*rxr4(i1,i2,i3)+sx(i1,i2,i3)*rxs4(
     & i1,i2,i3)
      rxy42(i1,i2,i3)= ry(i1,i2,i3)*rxr4(i1,i2,i3)+sy(i1,i2,i3)*rxs4(
     & i1,i2,i3)
      rxx43(i1,i2,i3)=rx(i1,i2,i3)*rxr4(i1,i2,i3)+sx(i1,i2,i3)*rxs4(i1,
     & i2,i3)+tx(i1,i2,i3)*rxt4(i1,i2,i3)
      rxy43(i1,i2,i3)=ry(i1,i2,i3)*rxr4(i1,i2,i3)+sy(i1,i2,i3)*rxs4(i1,
     & i2,i3)+ty(i1,i2,i3)*rxt4(i1,i2,i3)
      rxz43(i1,i2,i3)=rz(i1,i2,i3)*rxr4(i1,i2,i3)+sz(i1,i2,i3)*rxs4(i1,
     & i2,i3)+tz(i1,i2,i3)*rxt4(i1,i2,i3)
      ryx42(i1,i2,i3)= rx(i1,i2,i3)*ryr4(i1,i2,i3)+sx(i1,i2,i3)*rys4(
     & i1,i2,i3)
      ryy42(i1,i2,i3)= ry(i1,i2,i3)*ryr4(i1,i2,i3)+sy(i1,i2,i3)*rys4(
     & i1,i2,i3)
      ryx43(i1,i2,i3)=rx(i1,i2,i3)*ryr4(i1,i2,i3)+sx(i1,i2,i3)*rys4(i1,
     & i2,i3)+tx(i1,i2,i3)*ryt4(i1,i2,i3)
      ryy43(i1,i2,i3)=ry(i1,i2,i3)*ryr4(i1,i2,i3)+sy(i1,i2,i3)*rys4(i1,
     & i2,i3)+ty(i1,i2,i3)*ryt4(i1,i2,i3)
      ryz43(i1,i2,i3)=rz(i1,i2,i3)*ryr4(i1,i2,i3)+sz(i1,i2,i3)*rys4(i1,
     & i2,i3)+tz(i1,i2,i3)*ryt4(i1,i2,i3)
      rzx42(i1,i2,i3)= rx(i1,i2,i3)*rzr4(i1,i2,i3)+sx(i1,i2,i3)*rzs4(
     & i1,i2,i3)
      rzy42(i1,i2,i3)= ry(i1,i2,i3)*rzr4(i1,i2,i3)+sy(i1,i2,i3)*rzs4(
     & i1,i2,i3)
      rzx43(i1,i2,i3)=rx(i1,i2,i3)*rzr4(i1,i2,i3)+sx(i1,i2,i3)*rzs4(i1,
     & i2,i3)+tx(i1,i2,i3)*rzt4(i1,i2,i3)
      rzy43(i1,i2,i3)=ry(i1,i2,i3)*rzr4(i1,i2,i3)+sy(i1,i2,i3)*rzs4(i1,
     & i2,i3)+ty(i1,i2,i3)*rzt4(i1,i2,i3)
      rzz43(i1,i2,i3)=rz(i1,i2,i3)*rzr4(i1,i2,i3)+sz(i1,i2,i3)*rzs4(i1,
     & i2,i3)+tz(i1,i2,i3)*rzt4(i1,i2,i3)
      sxx42(i1,i2,i3)= rx(i1,i2,i3)*sxr4(i1,i2,i3)+sx(i1,i2,i3)*sxs4(
     & i1,i2,i3)
      sxy42(i1,i2,i3)= ry(i1,i2,i3)*sxr4(i1,i2,i3)+sy(i1,i2,i3)*sxs4(
     & i1,i2,i3)
      sxx43(i1,i2,i3)=rx(i1,i2,i3)*sxr4(i1,i2,i3)+sx(i1,i2,i3)*sxs4(i1,
     & i2,i3)+tx(i1,i2,i3)*sxt4(i1,i2,i3)
      sxy43(i1,i2,i3)=ry(i1,i2,i3)*sxr4(i1,i2,i3)+sy(i1,i2,i3)*sxs4(i1,
     & i2,i3)+ty(i1,i2,i3)*sxt4(i1,i2,i3)
      sxz43(i1,i2,i3)=rz(i1,i2,i3)*sxr4(i1,i2,i3)+sz(i1,i2,i3)*sxs4(i1,
     & i2,i3)+tz(i1,i2,i3)*sxt4(i1,i2,i3)
      syx42(i1,i2,i3)= rx(i1,i2,i3)*syr4(i1,i2,i3)+sx(i1,i2,i3)*sys4(
     & i1,i2,i3)
      syy42(i1,i2,i3)= ry(i1,i2,i3)*syr4(i1,i2,i3)+sy(i1,i2,i3)*sys4(
     & i1,i2,i3)
      syx43(i1,i2,i3)=rx(i1,i2,i3)*syr4(i1,i2,i3)+sx(i1,i2,i3)*sys4(i1,
     & i2,i3)+tx(i1,i2,i3)*syt4(i1,i2,i3)
      syy43(i1,i2,i3)=ry(i1,i2,i3)*syr4(i1,i2,i3)+sy(i1,i2,i3)*sys4(i1,
     & i2,i3)+ty(i1,i2,i3)*syt4(i1,i2,i3)
      syz43(i1,i2,i3)=rz(i1,i2,i3)*syr4(i1,i2,i3)+sz(i1,i2,i3)*sys4(i1,
     & i2,i3)+tz(i1,i2,i3)*syt4(i1,i2,i3)
      szx42(i1,i2,i3)= rx(i1,i2,i3)*szr4(i1,i2,i3)+sx(i1,i2,i3)*szs4(
     & i1,i2,i3)
      szy42(i1,i2,i3)= ry(i1,i2,i3)*szr4(i1,i2,i3)+sy(i1,i2,i3)*szs4(
     & i1,i2,i3)
      szx43(i1,i2,i3)=rx(i1,i2,i3)*szr4(i1,i2,i3)+sx(i1,i2,i3)*szs4(i1,
     & i2,i3)+tx(i1,i2,i3)*szt4(i1,i2,i3)
      szy43(i1,i2,i3)=ry(i1,i2,i3)*szr4(i1,i2,i3)+sy(i1,i2,i3)*szs4(i1,
     & i2,i3)+ty(i1,i2,i3)*szt4(i1,i2,i3)
      szz43(i1,i2,i3)=rz(i1,i2,i3)*szr4(i1,i2,i3)+sz(i1,i2,i3)*szs4(i1,
     & i2,i3)+tz(i1,i2,i3)*szt4(i1,i2,i3)
      txx42(i1,i2,i3)= rx(i1,i2,i3)*txr4(i1,i2,i3)+sx(i1,i2,i3)*txs4(
     & i1,i2,i3)
      txy42(i1,i2,i3)= ry(i1,i2,i3)*txr4(i1,i2,i3)+sy(i1,i2,i3)*txs4(
     & i1,i2,i3)
      txx43(i1,i2,i3)=rx(i1,i2,i3)*txr4(i1,i2,i3)+sx(i1,i2,i3)*txs4(i1,
     & i2,i3)+tx(i1,i2,i3)*txt4(i1,i2,i3)
      txy43(i1,i2,i3)=ry(i1,i2,i3)*txr4(i1,i2,i3)+sy(i1,i2,i3)*txs4(i1,
     & i2,i3)+ty(i1,i2,i3)*txt4(i1,i2,i3)
      txz43(i1,i2,i3)=rz(i1,i2,i3)*txr4(i1,i2,i3)+sz(i1,i2,i3)*txs4(i1,
     & i2,i3)+tz(i1,i2,i3)*txt4(i1,i2,i3)
      tyx42(i1,i2,i3)= rx(i1,i2,i3)*tyr4(i1,i2,i3)+sx(i1,i2,i3)*tys4(
     & i1,i2,i3)
      tyy42(i1,i2,i3)= ry(i1,i2,i3)*tyr4(i1,i2,i3)+sy(i1,i2,i3)*tys4(
     & i1,i2,i3)
      tyx43(i1,i2,i3)=rx(i1,i2,i3)*tyr4(i1,i2,i3)+sx(i1,i2,i3)*tys4(i1,
     & i2,i3)+tx(i1,i2,i3)*tyt4(i1,i2,i3)
      tyy43(i1,i2,i3)=ry(i1,i2,i3)*tyr4(i1,i2,i3)+sy(i1,i2,i3)*tys4(i1,
     & i2,i3)+ty(i1,i2,i3)*tyt4(i1,i2,i3)
      tyz43(i1,i2,i3)=rz(i1,i2,i3)*tyr4(i1,i2,i3)+sz(i1,i2,i3)*tys4(i1,
     & i2,i3)+tz(i1,i2,i3)*tyt4(i1,i2,i3)
      tzx42(i1,i2,i3)= rx(i1,i2,i3)*tzr4(i1,i2,i3)+sx(i1,i2,i3)*tzs4(
     & i1,i2,i3)
      tzy42(i1,i2,i3)= ry(i1,i2,i3)*tzr4(i1,i2,i3)+sy(i1,i2,i3)*tzs4(
     & i1,i2,i3)
      tzx43(i1,i2,i3)=rx(i1,i2,i3)*tzr4(i1,i2,i3)+sx(i1,i2,i3)*tzs4(i1,
     & i2,i3)+tx(i1,i2,i3)*tzt4(i1,i2,i3)
      tzy43(i1,i2,i3)=ry(i1,i2,i3)*tzr4(i1,i2,i3)+sy(i1,i2,i3)*tzs4(i1,
     & i2,i3)+ty(i1,i2,i3)*tzt4(i1,i2,i3)
      tzz43(i1,i2,i3)=rz(i1,i2,i3)*tzr4(i1,i2,i3)+sz(i1,i2,i3)*tzs4(i1,
     & i2,i3)+tz(i1,i2,i3)*tzt4(i1,i2,i3)
      uxx41(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*urr4(i1,i2,i3,kd)+(rxx42(i1,
     & i2,i3))*ur4(i1,i2,i3,kd)
      uyy41(i1,i2,i3,kd)=0
      uxy41(i1,i2,i3,kd)=0
      uxz41(i1,i2,i3,kd)=0
      uyz41(i1,i2,i3,kd)=0
      uzz41(i1,i2,i3,kd)=0
      ulaplacian41(i1,i2,i3,kd)=uxx41(i1,i2,i3,kd)
      uxx42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*urr4(i1,i2,i3,kd)+2.*(rx(i1,
     & i2,i3)*sx(i1,i2,i3))*urs4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2)*uss4(
     & i1,i2,i3,kd)+(rxx42(i1,i2,i3))*ur4(i1,i2,i3,kd)+(sxx42(i1,i2,
     & i3))*us4(i1,i2,i3,kd)
      uyy42(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*urr4(i1,i2,i3,kd)+2.*(ry(i1,
     & i2,i3)*sy(i1,i2,i3))*urs4(i1,i2,i3,kd)+(sy(i1,i2,i3)**2)*uss4(
     & i1,i2,i3,kd)+(ryy42(i1,i2,i3))*ur4(i1,i2,i3,kd)+(syy42(i1,i2,
     & i3))*us4(i1,i2,i3,kd)
      uxy42(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*urr4(i1,i2,i3,kd)+(
     & rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*urs4(i1,
     & i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*uss4(i1,i2,i3,kd)+rxy42(i1,
     & i2,i3)*ur4(i1,i2,i3,kd)+sxy42(i1,i2,i3)*us4(i1,i2,i3,kd)
      uxz42(i1,i2,i3,kd)=0
      uyz42(i1,i2,i3,kd)=0
      uzz42(i1,i2,i3,kd)=0
      ulaplacian42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*urr4(
     & i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,
     & i2,i3))*urs4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)**2)*
     & uss4(i1,i2,i3,kd)+(rxx42(i1,i2,i3)+ryy42(i1,i2,i3))*ur4(i1,i2,
     & i3,kd)+(sxx42(i1,i2,i3)+syy42(i1,i2,i3))*us4(i1,i2,i3,kd)
      uxx43(i1,i2,i3,kd)=rx(i1,i2,i3)**2*urr4(i1,i2,i3,kd)+sx(i1,i2,i3)
     & **2*uss4(i1,i2,i3,kd)+tx(i1,i2,i3)**2*utt4(i1,i2,i3,kd)+2.*rx(
     & i1,i2,i3)*sx(i1,i2,i3)*urs4(i1,i2,i3,kd)+2.*rx(i1,i2,i3)*tx(i1,
     & i2,i3)*urt4(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,i2,i3)*ust4(i1,
     & i2,i3,kd)+rxx43(i1,i2,i3)*ur4(i1,i2,i3,kd)+sxx43(i1,i2,i3)*us4(
     & i1,i2,i3,kd)+txx43(i1,i2,i3)*ut4(i1,i2,i3,kd)
      uyy43(i1,i2,i3,kd)=ry(i1,i2,i3)**2*urr4(i1,i2,i3,kd)+sy(i1,i2,i3)
     & **2*uss4(i1,i2,i3,kd)+ty(i1,i2,i3)**2*utt4(i1,i2,i3,kd)+2.*ry(
     & i1,i2,i3)*sy(i1,i2,i3)*urs4(i1,i2,i3,kd)+2.*ry(i1,i2,i3)*ty(i1,
     & i2,i3)*urt4(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,i2,i3)*ust4(i1,
     & i2,i3,kd)+ryy43(i1,i2,i3)*ur4(i1,i2,i3,kd)+syy43(i1,i2,i3)*us4(
     & i1,i2,i3,kd)+tyy43(i1,i2,i3)*ut4(i1,i2,i3,kd)
      uzz43(i1,i2,i3,kd)=rz(i1,i2,i3)**2*urr4(i1,i2,i3,kd)+sz(i1,i2,i3)
     & **2*uss4(i1,i2,i3,kd)+tz(i1,i2,i3)**2*utt4(i1,i2,i3,kd)+2.*rz(
     & i1,i2,i3)*sz(i1,i2,i3)*urs4(i1,i2,i3,kd)+2.*rz(i1,i2,i3)*tz(i1,
     & i2,i3)*urt4(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,i2,i3)*ust4(i1,
     & i2,i3,kd)+rzz43(i1,i2,i3)*ur4(i1,i2,i3,kd)+szz43(i1,i2,i3)*us4(
     & i1,i2,i3,kd)+tzz43(i1,i2,i3)*ut4(i1,i2,i3,kd)
      uxy43(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*urr4(i1,i2,i3,kd)+
     & sx(i1,i2,i3)*sy(i1,i2,i3)*uss4(i1,i2,i3,kd)+tx(i1,i2,i3)*ty(i1,
     & i2,i3)*utt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,
     & i3)*sx(i1,i2,i3))*urs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(i1,i2,i3)+
     & ry(i1,i2,i3)*tx(i1,i2,i3))*urt4(i1,i2,i3,kd)+(sx(i1,i2,i3)*ty(
     & i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*ust4(i1,i2,i3,kd)+rxy43(
     & i1,i2,i3)*ur4(i1,i2,i3,kd)+sxy43(i1,i2,i3)*us4(i1,i2,i3,kd)+
     & txy43(i1,i2,i3)*ut4(i1,i2,i3,kd)
      uxz43(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*urr4(i1,i2,i3,kd)+
     & sx(i1,i2,i3)*sz(i1,i2,i3)*uss4(i1,i2,i3,kd)+tx(i1,i2,i3)*tz(i1,
     & i2,i3)*utt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,i2,
     & i3)*sx(i1,i2,i3))*urs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(i1,i2,i3)+
     & rz(i1,i2,i3)*tx(i1,i2,i3))*urt4(i1,i2,i3,kd)+(sx(i1,i2,i3)*tz(
     & i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*ust4(i1,i2,i3,kd)+rxz43(
     & i1,i2,i3)*ur4(i1,i2,i3,kd)+sxz43(i1,i2,i3)*us4(i1,i2,i3,kd)+
     & txz43(i1,i2,i3)*ut4(i1,i2,i3,kd)
      uyz43(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*urr4(i1,i2,i3,kd)+
     & sy(i1,i2,i3)*sz(i1,i2,i3)*uss4(i1,i2,i3,kd)+ty(i1,i2,i3)*tz(i1,
     & i2,i3)*utt4(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,i2,
     & i3)*sy(i1,i2,i3))*urs4(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(i1,i2,i3)+
     & rz(i1,i2,i3)*ty(i1,i2,i3))*urt4(i1,i2,i3,kd)+(sy(i1,i2,i3)*tz(
     & i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*ust4(i1,i2,i3,kd)+ryz43(
     & i1,i2,i3)*ur4(i1,i2,i3,kd)+syz43(i1,i2,i3)*us4(i1,i2,i3,kd)+
     & tyz43(i1,i2,i3)*ut4(i1,i2,i3,kd)
      ulaplacian43(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(i1,
     & i2,i3)**2)*urr4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)**2+
     & sz(i1,i2,i3)**2)*uss4(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+ty(i1,i2,
     & i3)**2+tz(i1,i2,i3)**2)*utt4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(
     & i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,i2,i3))
     & *urs4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(i1,i2,i3)*
     & ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*urt4(i1,i2,i3,kd)+2.*(
     & sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)+sz(i1,i2,
     & i3)*tz(i1,i2,i3))*ust4(i1,i2,i3,kd)+(rxx43(i1,i2,i3)+ryy43(i1,
     & i2,i3)+rzz43(i1,i2,i3))*ur4(i1,i2,i3,kd)+(sxx43(i1,i2,i3)+
     & syy43(i1,i2,i3)+szz43(i1,i2,i3))*us4(i1,i2,i3,kd)+(txx43(i1,i2,
     & i3)+tyy43(i1,i2,i3)+tzz43(i1,i2,i3))*ut4(i1,i2,i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      h41(kd) = 1./(12.*dx(kd))
      h42(kd) = 1./(12.*dx(kd)**2)
      ux43r(i1,i2,i3,kd)=(8.*(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))-(u(i1+
     & 2,i2,i3,kd)-u(i1-2,i2,i3,kd)))*h41(0)
      uy43r(i1,i2,i3,kd)=(8.*(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))-(u(i1,
     & i2+2,i3,kd)-u(i1,i2-2,i3,kd)))*h41(1)
      uz43r(i1,i2,i3,kd)=(8.*(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))-(u(i1,
     & i2,i3+2,kd)-u(i1,i2,i3-2,kd)))*h41(2)
      uxx43r(i1,i2,i3,kd)=( -30.*u(i1,i2,i3,kd)+16.*(u(i1+1,i2,i3,kd)+
     & u(i1-1,i2,i3,kd))-(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd)) )*h42(0)
      uyy43r(i1,i2,i3,kd)=( -30.*u(i1,i2,i3,kd)+16.*(u(i1,i2+1,i3,kd)+
     & u(i1,i2-1,i3,kd))-(u(i1,i2+2,i3,kd)+u(i1,i2-2,i3,kd)) )*h42(1)
      uzz43r(i1,i2,i3,kd)=( -30.*u(i1,i2,i3,kd)+16.*(u(i1,i2,i3+1,kd)+
     & u(i1,i2,i3-1,kd))-(u(i1,i2,i3+2,kd)+u(i1,i2,i3-2,kd)) )*h42(2)
      uxy43r(i1,i2,i3,kd)=( (u(i1+2,i2+2,i3,kd)-u(i1-2,i2+2,i3,kd)- u(
     & i1+2,i2-2,i3,kd)+u(i1-2,i2-2,i3,kd)) +8.*(u(i1-1,i2+2,i3,kd)-u(
     & i1-1,i2-2,i3,kd)-u(i1+1,i2+2,i3,kd)+u(i1+1,i2-2,i3,kd) +u(i1+2,
     & i2-1,i3,kd)-u(i1-2,i2-1,i3,kd)-u(i1+2,i2+1,i3,kd)+u(i1-2,i2+1,
     & i3,kd))+64.*(u(i1+1,i2+1,i3,kd)-u(i1-1,i2+1,i3,kd)- u(i1+1,i2-
     & 1,i3,kd)+u(i1-1,i2-1,i3,kd)))*(h41(0)*h41(1))
      uxz43r(i1,i2,i3,kd)=( (u(i1+2,i2,i3+2,kd)-u(i1-2,i2,i3+2,kd)-u(
     & i1+2,i2,i3-2,kd)+u(i1-2,i2,i3-2,kd)) +8.*(u(i1-1,i2,i3+2,kd)-u(
     & i1-1,i2,i3-2,kd)-u(i1+1,i2,i3+2,kd)+u(i1+1,i2,i3-2,kd) +u(i1+2,
     & i2,i3-1,kd)-u(i1-2,i2,i3-1,kd)- u(i1+2,i2,i3+1,kd)+u(i1-2,i2,
     & i3+1,kd)) +64.*(u(i1+1,i2,i3+1,kd)-u(i1-1,i2,i3+1,kd)-u(i1+1,
     & i2,i3-1,kd)+u(i1-1,i2,i3-1,kd)) )*(h41(0)*h41(2))
      uyz43r(i1,i2,i3,kd)=( (u(i1,i2+2,i3+2,kd)-u(i1,i2-2,i3+2,kd)-u(
     & i1,i2+2,i3-2,kd)+u(i1,i2-2,i3-2,kd)) +8.*(u(i1,i2-1,i3+2,kd)-u(
     & i1,i2-1,i3-2,kd)-u(i1,i2+1,i3+2,kd)+u(i1,i2+1,i3-2,kd) +u(i1,
     & i2+2,i3-1,kd)-u(i1,i2-2,i3-1,kd)-u(i1,i2+2,i3+1,kd)+u(i1,i2-2,
     & i3+1,kd)) +64.*(u(i1,i2+1,i3+1,kd)-u(i1,i2-1,i3+1,kd)-u(i1,i2+
     & 1,i3-1,kd)+u(i1,i2-1,i3-1,kd)) )*(h41(1)*h41(2))
      ux41r(i1,i2,i3,kd)= ux43r(i1,i2,i3,kd)
      uy41r(i1,i2,i3,kd)= uy43r(i1,i2,i3,kd)
      uz41r(i1,i2,i3,kd)= uz43r(i1,i2,i3,kd)
      uxx41r(i1,i2,i3,kd)= uxx43r(i1,i2,i3,kd)
      uyy41r(i1,i2,i3,kd)= uyy43r(i1,i2,i3,kd)
      uzz41r(i1,i2,i3,kd)= uzz43r(i1,i2,i3,kd)
      uxy41r(i1,i2,i3,kd)= uxy43r(i1,i2,i3,kd)
      uxz41r(i1,i2,i3,kd)= uxz43r(i1,i2,i3,kd)
      uyz41r(i1,i2,i3,kd)= uyz43r(i1,i2,i3,kd)
      ulaplacian41r(i1,i2,i3,kd)=uxx43r(i1,i2,i3,kd)
      ux42r(i1,i2,i3,kd)= ux43r(i1,i2,i3,kd)
      uy42r(i1,i2,i3,kd)= uy43r(i1,i2,i3,kd)
      uz42r(i1,i2,i3,kd)= uz43r(i1,i2,i3,kd)
      uxx42r(i1,i2,i3,kd)= uxx43r(i1,i2,i3,kd)
      uyy42r(i1,i2,i3,kd)= uyy43r(i1,i2,i3,kd)
      uzz42r(i1,i2,i3,kd)= uzz43r(i1,i2,i3,kd)
      uxy42r(i1,i2,i3,kd)= uxy43r(i1,i2,i3,kd)
      uxz42r(i1,i2,i3,kd)= uxz43r(i1,i2,i3,kd)
      uyz42r(i1,i2,i3,kd)= uyz43r(i1,i2,i3,kd)
      ulaplacian42r(i1,i2,i3,kd)=uxx43r(i1,i2,i3,kd)+uyy43r(i1,i2,i3,
     & kd)
      ulaplacian43r(i1,i2,i3,kd)=uxx43r(i1,i2,i3,kd)+uyy43r(i1,i2,i3,
     & kd)+uzz43r(i1,i2,i3,kd)

c............... end statement functions

      ierr=0

      side                 =ipar(0)
      axis                 =ipar(1)
      n1a                  =ipar(2)
      n1b                  =ipar(3)
      n2a                  =ipar(4)
      n2b                  =ipar(5)
      n3a                  =ipar(6)
      n3b                  =ipar(7)
      gridType             =ipar(8)
      orderOfAccuracy      =ipar(9)
      orderOfExtrapolation =ipar(10)
      useForcing           =ipar(11)
      ex                   =ipar(12)
      ey                   =ipar(13)
      ez                   =ipar(14)
      hx                   =ipar(15)
      hy                   =ipar(16)
      hz                   =ipar(17)
      useWhereMask         =ipar(18)
      grid                 =ipar(19)
      debug                =ipar(20)

      dx(0)                =rpar(0)
      dx(1)                =rpar(1)
      dx(2)                =rpar(2)
      dr(0)                =rpar(3)
      dr(1)                =rpar(4)
      dr(2)                =rpar(5)
      t                    =rpar(6)
      ep                   =rpar(7) ! pointer for exact solution
      dt                   =rpar(8)
      c                    =rpar(9)

      if( debug.gt.1 )then
        write(*,'(" abcMaxwell: **START** grid=",i4," side,axis=",2i2)
     & ') grid,side,axis
      end if

      ! Engquist-Majda 2nd-order
      !    u.xt = (1/c)*u.tt - c/2 * (u.yy + u.zz)   at x=0
      !         = c*( u.xx + .5*( u.yy + u.zz ) 

      ! We need un : u(t+dt) 
      !         u  : u(t)

      ! Generalized form:
      ! u.xt = c1abcem2*u.xx + c2abcem2*( u.yy + u.zz )
      !   Taylor: p0=1 p2=-1/2
      !   Cheby:  p0=1.00023, p2=-.515555
      p0=1.
      p2=-.5
      ! p0=1.00023   !   Cheby on a subinterval
      ! p2=-.515555  !   Cheby on a subinterval
      c1abcem2=c*p0
      c2abcem2=c*(p0+p2)


      extra=-1  ! no need to do corners -- these are already done in another way
      extra=0 ! re-compute corners
      numberOfGhostPoints=orderOfAccuracy/2

      dxa=dx(0)
      dya=dx(1)
      dza=dx(2)

      ! ------------------------------------------------------------------------
      ! ------------------Corners-----------------------------------------------
      ! ------------------------------------------------------------------------

      if( nd.eq.2 )then
       i3=gridIndexRange(0,2)
       do side1=0,1
       do side2=0,1
        if( boundaryCondition(side1,0).ge.abcEM2 .and. 
     & boundaryCondition(side1,0).le.abc5 .and. boundaryCondition(
     & side2,1).ge.abcEM2 .and. boundaryCondition(side2,1).le.abc5 )
     & then

          i1=gridIndexRange(side1,0) ! (i1,i2,i3)=corner point
          i2=gridIndexRange(side2,1)

          ! write(*,'(" ABC:set corner: grid,side1,side2,i1,i2=",3i3,2i5)') grid,side1,side2,i1,i2

          is1=1-2*side1
          is2=0
          is3=0

          ! Use first-order-in-time formula since it doesn't require other ghost point at new time (un)
          un(i1-is1,i2-is2,i3-is3,ex)=un(i1+is1,i2+is2,i3+is3,ex)-(u(
     & i1+is1,i2+is2,i3+is3,ex)-u(i1-is1,i2-is2,i3-is3,ex))-(2.*dxa*
     & dt)*(c1abcem2*uxx22r(i1,i2,i3,ex)+c2abcem2*uyy22r(i1,i2,i3,ex))
          un(i1-is1,i2-is2,i3-is3,ey)=un(i1+is1,i2+is2,i3+is3,ey)-(u(
     & i1+is1,i2+is2,i3+is3,ey)-u(i1-is1,i2-is2,i3-is3,ey))-(2.*dxa*
     & dt)*(c1abcem2*uxx22r(i1,i2,i3,ey)+c2abcem2*uyy22r(i1,i2,i3,ey))
          un(i1-is1,i2-is2,i3-is3,hz)=un(i1+is1,i2+is2,i3+is3,hz)-(u(
     & i1+is1,i2+is2,i3+is3,hz)-u(i1-is1,i2-is2,i3-is3,hz))-(2.*dxa*
     & dt)*(c1abcem2*uxx22r(i1,i2,i3,hz)+c2abcem2*uyy22r(i1,i2,i3,hz))

          is1=0
          is2=1-2*side2

          un(i1-is1,i2-is2,i3-is3,ex)=un(i1+is1,i2+is2,i3+is3,ex)-(u(
     & i1+is1,i2+is2,i3+is3,ex)-u(i1-is1,i2-is2,i3-is3,ex))-(2.*dya*
     & dt)*(c1abcem2*uyy22r(i1,i2,i3,ex)+c2abcem2*uxx22r(i1,i2,i3,ex))
          un(i1-is1,i2-is2,i3-is3,ey)=un(i1+is1,i2+is2,i3+is3,ey)-(u(
     & i1+is1,i2+is2,i3+is3,ey)-u(i1-is1,i2-is2,i3-is3,ey))-(2.*dya*
     & dt)*(c1abcem2*uyy22r(i1,i2,i3,ey)+c2abcem2*uxx22r(i1,i2,i3,ey))
          un(i1-is1,i2-is2,i3-is3,hz)=un(i1+is1,i2+is2,i3+is3,hz)-(u(
     & i1+is1,i2+is2,i3+is3,hz)-u(i1-is1,i2-is2,i3-is3,hz))-(2.*dya*
     & dt)*(c1abcem2*uyy22r(i1,i2,i3,hz)+c2abcem2*uxx22r(i1,i2,i3,hz))

          ! extrap corner -- could do better
          is1=1-2*side1
          is2=1-2*side2
          un(i1-is1,i2-is2,i3,ex)=2.*un(i1,i2,i3,ex)-un(i1+is1,i2+is2,
     & i3,ex)
          un(i1-is1,i2-is2,i3,ey)=2.*un(i1,i2,i3,ey)-un(i1+is1,i2+is2,
     & i3,ey)
          un(i1-is1,i2-is2,i3,hz)=                   un(i1+is1,i2+is2,
     & i3,hz)

          if( orderOfAccuracy.eq.4 )then

           is1=1-2*side1
           is2=0
             un(i1-2*is1,i2-2*is2,i3-2*is3,ex)=4.*un(i1-is1,i2-is2,i3-
     & is3,ex)-6.*un(i1,i2,i3,ex)+4.*un(i1+is1,i2+is2,i3+is3,ex)-un(
     & i1+2*is1,i2+2*is2,i3+2*is3,ex)
             un(i1-2*is1,i2-2*is2,i3-2*is3,ey)=4.*un(i1-is1,i2-is2,i3-
     & is3,ey)-6.*un(i1,i2,i3,ey)+4.*un(i1+is1,i2+is2,i3+is3,ey)-un(
     & i1+2*is1,i2+2*is2,i3+2*is3,ey)
             un(i1-2*is1,i2-2*is2,i3-2*is3,hz)=4.*un(i1-is1,i2-is2,i3-
     & is3,hz)-6.*un(i1,i2,i3,hz)+4.*un(i1+is1,i2+is2,i3+is3,hz)-un(
     & i1+2*is1,i2+2*is2,i3+2*is3,hz)

           is1=0
           is2=1-2*side2

             un(i1-2*is1,i2-2*is2,i3-2*is3,ex)=4.*un(i1-is1,i2-is2,i3-
     & is3,ex)-6.*un(i1,i2,i3,ex)+4.*un(i1+is1,i2+is2,i3+is3,ex)-un(
     & i1+2*is1,i2+2*is2,i3+2*is3,ex)
             un(i1-2*is1,i2-2*is2,i3-2*is3,ey)=4.*un(i1-is1,i2-is2,i3-
     & is3,ey)-6.*un(i1,i2,i3,ey)+4.*un(i1+is1,i2+is2,i3+is3,ey)-un(
     & i1+2*is1,i2+2*is2,i3+2*is3,ey)
             un(i1-2*is1,i2-2*is2,i3-2*is3,hz)=4.*un(i1-is1,i2-is2,i3-
     & is3,hz)-6.*un(i1,i2,i3,hz)+4.*un(i1+is1,i2+is2,i3+is3,hz)-un(
     & i1+2*is1,i2+2*is2,i3+2*is3,hz)

           is1=1-2*side1
           is2=1-2*side2

             un(i1-2*is1,i2-2*is2,i3-2*is3,ex)=4.*un(i1-is1,i2-is2,i3-
     & is3,ex)-6.*un(i1,i2,i3,ex)+4.*un(i1+is1,i2+is2,i3+is3,ex)-un(
     & i1+2*is1,i2+2*is2,i3+2*is3,ex)
             un(i1-2*is1,i2-2*is2,i3-2*is3,ey)=4.*un(i1-is1,i2-is2,i3-
     & is3,ey)-6.*un(i1,i2,i3,ey)+4.*un(i1+is1,i2+is2,i3+is3,ey)-un(
     & i1+2*is1,i2+2*is2,i3+2*is3,ey)
             un(i1-2*is1,i2-2*is2,i3-2*is3,hz)=4.*un(i1-is1,i2-is2,i3-
     & is3,hz)-6.*un(i1,i2,i3,hz)+4.*un(i1+is1,i2+is2,i3+is3,hz)-un(
     & i1+2*is1,i2+2*is2,i3+2*is3,hz)

          end if


        end if
       end do
       end do

      else ! ***** 3D *****

        do edgeDirection=0,2 ! direction parallel to the edge
        do sidea=0,1
        do sideb=0,1
         if( edgeDirection.eq.0 )then
           side1=0
           side2=sidea
           side3=sideb
         else if( edgeDirection.eq.1 )then
           side1=sideb
           side2=0
           side3=sidea
         else
           side1=sidea
           side2=sideb
           side3=0
         end if
        is1=1-2*(side1)
        is2=1-2*(side2)
        is3=1-2*(side3)
        if( edgeDirection.eq.2 )then
         is3=0
         n1a=gridIndexRange(side1,0)
         n1b=gridIndexRange(side1,0)
         n2a=gridIndexRange(side2,1)
         n2b=gridIndexRange(side2,1)
         n3a=gridIndexRange(0,2)
         n3b=gridIndexRange(1,2)
         bc1=boundaryCondition(side1,0)
         bc2=boundaryCondition(side2,1)
        else if( edgeDirection.eq.1 )then
         is2=0
         n1a=gridIndexRange(side1,0)
         n1b=gridIndexRange(side1,0)
         n2a=gridIndexRange(    0,1)
         n2b=gridIndexRange(    1,1)
         n3a=gridIndexRange(side3,2)
         n3b=gridIndexRange(side3,2)
         bc1=boundaryCondition(side1,0)
         bc2=boundaryCondition(side3,2)
        else
         is1=0
         n1a=gridIndexRange(    0,0)
         n1b=gridIndexRange(    1,0)
         n2a=gridIndexRange(side2,1)
         n2b=gridIndexRange(side2,1)
         n3a=gridIndexRange(side3,2)
         n3b=gridIndexRange(side3,2)
         bc1=boundaryCondition(side2,1)
         bc2=boundaryCondition(side3,2)
        end if
        if( bc1.ge.abcEM2 .and. bc1.le.abc5 .and. bc2.ge.abcEM2 .and. 
     & bc2.le.abc5 )then

         if( edgeDirection.eq.0 )then

          i2=n2a
          i3=n3a
          do i1=n1a,n1b
            ! Use first-order-in-time formula since it doesn't require other ghost point at new time (un)
            un(i1,i2,i3-is3,ex)=un(i1+0,i2+0,i3+is3,ex)-(u(i1+0,i2+0,
     & i3+is3,ex)-u(i1-0,i2-0,i3-is3,ex))-(2.*dza*dt)*(c1abcem2*
     & uzz23r(i1,i2,i3,ex)+c2abcem2*(uxx23r(i1,i2,i3,ex)+uyy23r(i1,i2,
     & i3,ex)))
            un(i1,i2,i3-is3,ey)=un(i1+0,i2+0,i3+is3,ey)-(u(i1+0,i2+0,
     & i3+is3,ey)-u(i1-0,i2-0,i3-is3,ey))-(2.*dza*dt)*(c1abcem2*
     & uzz23r(i1,i2,i3,ey)+c2abcem2*(uxx23r(i1,i2,i3,ey)+uyy23r(i1,i2,
     & i3,ey)))
            un(i1,i2,i3-is3,ez)=un(i1+0,i2+0,i3+is3,ez)-(u(i1+0,i2+0,
     & i3+is3,ez)-u(i1-0,i2-0,i3-is3,ez))-(2.*dza*dt)*(c1abcem2*
     & uzz23r(i1,i2,i3,ez)+c2abcem2*(uxx23r(i1,i2,i3,ez)+uyy23r(i1,i2,
     & i3,ez)))

            un(i1,i2-is2,i3,ex)=un(i1+0,i2+is2,i3+0,ex)-(u(i1+0,i2+is2,
     & i3+0,ex)-u(i1-0,i2-is2,i3-0,ex))-(2.*dya*dt)*(c1abcem2*uyy23r(
     & i1,i2,i3,ex)+c2abcem2*(uxx23r(i1,i2,i3,ex)+uzz23r(i1,i2,i3,ex))
     & )
            un(i1,i2-is2,i3,ey)=un(i1+0,i2+is2,i3+0,ey)-(u(i1+0,i2+is2,
     & i3+0,ey)-u(i1-0,i2-is2,i3-0,ey))-(2.*dya*dt)*(c1abcem2*uyy23r(
     & i1,i2,i3,ey)+c2abcem2*(uxx23r(i1,i2,i3,ey)+uzz23r(i1,i2,i3,ey))
     & )
            un(i1,i2-is2,i3,ez)=un(i1+0,i2+is2,i3+0,ez)-(u(i1+0,i2+is2,
     & i3+0,ez)-u(i1-0,i2-is2,i3-0,ez))-(2.*dya*dt)*(c1abcem2*uyy23r(
     & i1,i2,i3,ez)+c2abcem2*(uxx23r(i1,i2,i3,ez)+uzz23r(i1,i2,i3,ez))
     & )

            ! extrap edge-corner point -- could do better
            un(i1,i2-is2,i3-is3,ex)=                   un(i1,i2+is2,i3+
     & is3,ex)
            un(i1,i2-is2,i3-is3,ey)=2.*un(i1,i2,i3,ex)-un(i1,i2+is2,i3+
     & is3,ey)
            un(i1,i2-is2,i3-is3,ez)=2.*un(i1,i2,i3,ez)-un(i1,i2+is2,i3+
     & is3,ez)

            if( orderOfAccuracy.eq.4 )then
               un(i1-2*0,i2-2*0,i3-2*is3,ex)=4.*un(i1-0,i2-0,i3-is3,ex)
     & -6.*un(i1,i2,i3,ex)+4.*un(i1+0,i2+0,i3+is3,ex)-un(i1+2*0,i2+2*
     & 0,i3+2*is3,ex)
               un(i1-2*0,i2-2*0,i3-2*is3,ey)=4.*un(i1-0,i2-0,i3-is3,ey)
     & -6.*un(i1,i2,i3,ey)+4.*un(i1+0,i2+0,i3+is3,ey)-un(i1+2*0,i2+2*
     & 0,i3+2*is3,ey)
               un(i1-2*0,i2-2*0,i3-2*is3,hz)=4.*un(i1-0,i2-0,i3-is3,hz)
     & -6.*un(i1,i2,i3,hz)+4.*un(i1+0,i2+0,i3+is3,hz)-un(i1+2*0,i2+2*
     & 0,i3+2*is3,hz)

               un(i1-2*0,i2-2*is2,i3-2*0,ex)=4.*un(i1-0,i2-is2,i3-0,ex)
     & -6.*un(i1,i2,i3,ex)+4.*un(i1+0,i2+is2,i3+0,ex)-un(i1+2*0,i2+2*
     & is2,i3+2*0,ex)
               un(i1-2*0,i2-2*is2,i3-2*0,ey)=4.*un(i1-0,i2-is2,i3-0,ey)
     & -6.*un(i1,i2,i3,ey)+4.*un(i1+0,i2+is2,i3+0,ey)-un(i1+2*0,i2+2*
     & is2,i3+2*0,ey)
               un(i1-2*0,i2-2*is2,i3-2*0,hz)=4.*un(i1-0,i2-is2,i3-0,hz)
     & -6.*un(i1,i2,i3,hz)+4.*un(i1+0,i2+is2,i3+0,hz)-un(i1+2*0,i2+2*
     & is2,i3+2*0,hz)

               un(i1-2*is1,i2-2*is2,i3-2*is3,ex)=4.*un(i1-is1,i2-is2,
     & i3-is3,ex)-6.*un(i1,i2,i3,ex)+4.*un(i1+is1,i2+is2,i3+is3,ex)-
     & un(i1+2*is1,i2+2*is2,i3+2*is3,ex)
               un(i1-2*is1,i2-2*is2,i3-2*is3,ey)=4.*un(i1-is1,i2-is2,
     & i3-is3,ey)-6.*un(i1,i2,i3,ey)+4.*un(i1+is1,i2+is2,i3+is3,ey)-
     & un(i1+2*is1,i2+2*is2,i3+2*is3,ey)
               un(i1-2*is1,i2-2*is2,i3-2*is3,hz)=4.*un(i1-is1,i2-is2,
     & i3-is3,hz)-6.*un(i1,i2,i3,hz)+4.*un(i1+is1,i2+is2,i3+is3,hz)-
     & un(i1+2*is1,i2+2*is2,i3+2*is3,hz)
           end if

          end do

         else if( edgeDirection.eq.1 )then

          i1=n1a
          i3=n3a
          do i2=n2a,n2b
            ! Use first-order-in-time formula since it doesn't require other ghost point at new time (un)
            un(i1-is1,i2,i3,ex)=un(i1+is1,i2+0,i3+0,ex)-(u(i1+is1,i2+0,
     & i3+0,ex)-u(i1-is1,i2-0,i3-0,ex))-(2.*dxa*dt)*(c1abcem2*uxx23r(
     & i1,i2,i3,ex)+c2abcem2*(uyy23r(i1,i2,i3,ex)+uzz23r(i1,i2,i3,ex))
     & )
            un(i1-is1,i2,i3,ey)=un(i1+is1,i2+0,i3+0,ey)-(u(i1+is1,i2+0,
     & i3+0,ey)-u(i1-is1,i2-0,i3-0,ey))-(2.*dxa*dt)*(c1abcem2*uxx23r(
     & i1,i2,i3,ey)+c2abcem2*(uyy23r(i1,i2,i3,ey)+uzz23r(i1,i2,i3,ey))
     & )
            un(i1-is1,i2,i3,ez)=un(i1+is1,i2+0,i3+0,ez)-(u(i1+is1,i2+0,
     & i3+0,ez)-u(i1-is1,i2-0,i3-0,ez))-(2.*dxa*dt)*(c1abcem2*uxx23r(
     & i1,i2,i3,ez)+c2abcem2*(uyy23r(i1,i2,i3,ez)+uzz23r(i1,i2,i3,ez))
     & )

            un(i1,i2,i3-is3,ex)=un(i1+0,i2+0,i3+is3,ex)-(u(i1+0,i2+0,
     & i3+is3,ex)-u(i1-0,i2-0,i3-is3,ex))-(2.*dza*dt)*(c1abcem2*
     & uzz23r(i1,i2,i3,ex)+c2abcem2*(uxx23r(i1,i2,i3,ex)+uyy23r(i1,i2,
     & i3,ex)))
            un(i1,i2,i3-is3,ey)=un(i1+0,i2+0,i3+is3,ey)-(u(i1+0,i2+0,
     & i3+is3,ey)-u(i1-0,i2-0,i3-is3,ey))-(2.*dza*dt)*(c1abcem2*
     & uzz23r(i1,i2,i3,ey)+c2abcem2*(uxx23r(i1,i2,i3,ey)+uyy23r(i1,i2,
     & i3,ey)))
            un(i1,i2,i3-is3,ez)=un(i1+0,i2+0,i3+is3,ez)-(u(i1+0,i2+0,
     & i3+is3,ez)-u(i1-0,i2-0,i3-is3,ez))-(2.*dza*dt)*(c1abcem2*
     & uzz23r(i1,i2,i3,ez)+c2abcem2*(uxx23r(i1,i2,i3,ez)+uyy23r(i1,i2,
     & i3,ez)))

            un(i1-is1,i2,i3-is3,ex)=2.*un(i1,i2,i3,ex)-un(i1+is1,i2,i3+
     & is3,ex)
            un(i1-is1,i2,i3-is3,ey)=                   un(i1+is1,i2,i3+
     & is3,ey)
            un(i1-is1,i2,i3-is3,ez)=2.*un(i1,i2,i3,ez)-un(i1+is1,i2,i3+
     & is3,ez)

            if( orderOfAccuracy.eq.4 )then
               un(i1-2*is1,i2-2*0,i3-2*0,ex)=4.*un(i1-is1,i2-0,i3-0,ex)
     & -6.*un(i1,i2,i3,ex)+4.*un(i1+is1,i2+0,i3+0,ex)-un(i1+2*is1,i2+
     & 2*0,i3+2*0,ex)
               un(i1-2*is1,i2-2*0,i3-2*0,ey)=4.*un(i1-is1,i2-0,i3-0,ey)
     & -6.*un(i1,i2,i3,ey)+4.*un(i1+is1,i2+0,i3+0,ey)-un(i1+2*is1,i2+
     & 2*0,i3+2*0,ey)
               un(i1-2*is1,i2-2*0,i3-2*0,hz)=4.*un(i1-is1,i2-0,i3-0,hz)
     & -6.*un(i1,i2,i3,hz)+4.*un(i1+is1,i2+0,i3+0,hz)-un(i1+2*is1,i2+
     & 2*0,i3+2*0,hz)

               un(i1-2*0,i2-2*0,i3-2*is3,ex)=4.*un(i1-0,i2-0,i3-is3,ex)
     & -6.*un(i1,i2,i3,ex)+4.*un(i1+0,i2+0,i3+is3,ex)-un(i1+2*0,i2+2*
     & 0,i3+2*is3,ex)
               un(i1-2*0,i2-2*0,i3-2*is3,ey)=4.*un(i1-0,i2-0,i3-is3,ey)
     & -6.*un(i1,i2,i3,ey)+4.*un(i1+0,i2+0,i3+is3,ey)-un(i1+2*0,i2+2*
     & 0,i3+2*is3,ey)
               un(i1-2*0,i2-2*0,i3-2*is3,hz)=4.*un(i1-0,i2-0,i3-is3,hz)
     & -6.*un(i1,i2,i3,hz)+4.*un(i1+0,i2+0,i3+is3,hz)-un(i1+2*0,i2+2*
     & 0,i3+2*is3,hz)

               un(i1-2*is1,i2-2*is2,i3-2*is3,ex)=4.*un(i1-is1,i2-is2,
     & i3-is3,ex)-6.*un(i1,i2,i3,ex)+4.*un(i1+is1,i2+is2,i3+is3,ex)-
     & un(i1+2*is1,i2+2*is2,i3+2*is3,ex)
               un(i1-2*is1,i2-2*is2,i3-2*is3,ey)=4.*un(i1-is1,i2-is2,
     & i3-is3,ey)-6.*un(i1,i2,i3,ey)+4.*un(i1+is1,i2+is2,i3+is3,ey)-
     & un(i1+2*is1,i2+2*is2,i3+2*is3,ey)
               un(i1-2*is1,i2-2*is2,i3-2*is3,hz)=4.*un(i1-is1,i2-is2,
     & i3-is3,hz)-6.*un(i1,i2,i3,hz)+4.*un(i1+is1,i2+is2,i3+is3,hz)-
     & un(i1+2*is1,i2+2*is2,i3+2*is3,hz)
            end if

          end do

         else if( edgeDirection.eq.2 )then
          ! write(*,'(" ABC:set corner: grid,side1,side2,i1,i2=",3i3,2i5)') grid,side1,side2,i1,i2
          i1=n1a
          i2=n2a
          do i3=n3a,n3b
            ! Use first-order-in-time formula since it doesn't require other ghost point at new time (un)
            un(i1-is1,i2,i3,ex)=un(i1+is1,i2+0,i3+0,ex)-(u(i1+is1,i2+0,
     & i3+0,ex)-u(i1-is1,i2-0,i3-0,ex))-(2.*dxa*dt)*(c1abcem2*uxx23r(
     & i1,i2,i3,ex)+c2abcem2*(uyy23r(i1,i2,i3,ex)+uzz23r(i1,i2,i3,ex))
     & )
            un(i1-is1,i2,i3,ey)=un(i1+is1,i2+0,i3+0,ey)-(u(i1+is1,i2+0,
     & i3+0,ey)-u(i1-is1,i2-0,i3-0,ey))-(2.*dxa*dt)*(c1abcem2*uxx23r(
     & i1,i2,i3,ey)+c2abcem2*(uyy23r(i1,i2,i3,ey)+uzz23r(i1,i2,i3,ey))
     & )
            un(i1-is1,i2,i3,ez)=un(i1+is1,i2+0,i3+0,ez)-(u(i1+is1,i2+0,
     & i3+0,ez)-u(i1-is1,i2-0,i3-0,ez))-(2.*dxa*dt)*(c1abcem2*uxx23r(
     & i1,i2,i3,ez)+c2abcem2*(uyy23r(i1,i2,i3,ez)+uzz23r(i1,i2,i3,ez))
     & )

            un(i1,i2-is2,i3,ex)=un(i1+0,i2+is2,i3+0,ex)-(u(i1+0,i2+is2,
     & i3+0,ex)-u(i1-0,i2-is2,i3-0,ex))-(2.*dya*dt)*(c1abcem2*uyy23r(
     & i1,i2,i3,ex)+c2abcem2*(uxx23r(i1,i2,i3,ex)+uzz23r(i1,i2,i3,ex))
     & )
            un(i1,i2-is2,i3,ey)=un(i1+0,i2+is2,i3+0,ey)-(u(i1+0,i2+is2,
     & i3+0,ey)-u(i1-0,i2-is2,i3-0,ey))-(2.*dya*dt)*(c1abcem2*uyy23r(
     & i1,i2,i3,ey)+c2abcem2*(uxx23r(i1,i2,i3,ey)+uzz23r(i1,i2,i3,ey))
     & )
            un(i1,i2-is2,i3,ez)=un(i1+0,i2+is2,i3+0,ez)-(u(i1+0,i2+is2,
     & i3+0,ez)-u(i1-0,i2-is2,i3-0,ez))-(2.*dya*dt)*(c1abcem2*uyy23r(
     & i1,i2,i3,ez)+c2abcem2*(uxx23r(i1,i2,i3,ez)+uzz23r(i1,i2,i3,ez))
     & )

            ! extrap edge-corner point -- could do better
            un(i1-is1,i2-is2,i3,ex)=2.*un(i1,i2,i3,ex)-un(i1+is1,i2+
     & is2,i3,ex)
            un(i1-is1,i2-is2,i3,ey)=2.*un(i1,i2,i3,ey)-un(i1+is1,i2+
     & is2,i3,ey)
            un(i1-is1,i2-is2,i3,ez)=                   un(i1+is1,i2+
     & is2,i3,ez)

            if( orderOfAccuracy.eq.4 )then
               un(i1-2*is1,i2-2*0,i3-2*0,ex)=4.*un(i1-is1,i2-0,i3-0,ex)
     & -6.*un(i1,i2,i3,ex)+4.*un(i1+is1,i2+0,i3+0,ex)-un(i1+2*is1,i2+
     & 2*0,i3+2*0,ex)
               un(i1-2*is1,i2-2*0,i3-2*0,ey)=4.*un(i1-is1,i2-0,i3-0,ey)
     & -6.*un(i1,i2,i3,ey)+4.*un(i1+is1,i2+0,i3+0,ey)-un(i1+2*is1,i2+
     & 2*0,i3+2*0,ey)
               un(i1-2*is1,i2-2*0,i3-2*0,hz)=4.*un(i1-is1,i2-0,i3-0,hz)
     & -6.*un(i1,i2,i3,hz)+4.*un(i1+is1,i2+0,i3+0,hz)-un(i1+2*is1,i2+
     & 2*0,i3+2*0,hz)

               un(i1-2*0,i2-2*is2,i3-2*0,ex)=4.*un(i1-0,i2-is2,i3-0,ex)
     & -6.*un(i1,i2,i3,ex)+4.*un(i1+0,i2+is2,i3+0,ex)-un(i1+2*0,i2+2*
     & is2,i3+2*0,ex)
               un(i1-2*0,i2-2*is2,i3-2*0,ey)=4.*un(i1-0,i2-is2,i3-0,ey)
     & -6.*un(i1,i2,i3,ey)+4.*un(i1+0,i2+is2,i3+0,ey)-un(i1+2*0,i2+2*
     & is2,i3+2*0,ey)
               un(i1-2*0,i2-2*is2,i3-2*0,hz)=4.*un(i1-0,i2-is2,i3-0,hz)
     & -6.*un(i1,i2,i3,hz)+4.*un(i1+0,i2+is2,i3+0,hz)-un(i1+2*0,i2+2*
     & is2,i3+2*0,hz)

               un(i1-2*is1,i2-2*is2,i3-2*is3,ex)=4.*un(i1-is1,i2-is2,
     & i3-is3,ex)-6.*un(i1,i2,i3,ex)+4.*un(i1+is1,i2+is2,i3+is3,ex)-
     & un(i1+2*is1,i2+2*is2,i3+2*is3,ex)
               un(i1-2*is1,i2-2*is2,i3-2*is3,ey)=4.*un(i1-is1,i2-is2,
     & i3-is3,ey)-6.*un(i1,i2,i3,ey)+4.*un(i1+is1,i2+is2,i3+is3,ey)-
     & un(i1+2*is1,i2+2*is2,i3+2*is3,ey)
               un(i1-2*is1,i2-2*is2,i3-2*is3,hz)=4.*un(i1-is1,i2-is2,
     & i3-is3,hz)-6.*un(i1,i2,i3,hz)+4.*un(i1+is1,i2+is2,i3+is3,hz)-
     & un(i1+2*is1,i2+2*is2,i3+2*is3,hz)
            end if
          end do

         end if ! end if edgeDirection
        end if ! bc
        end do ! end sideb
        end do ! end sidea
        end do ! end edgeDirection

       ! ***** vertices *****
       !  normal-direction:     u -> +u
       !  tangential-direction  u -> -u
       !     u(-1,-1,-1) = +u(1,1,1)
       do side3=0,1
       do side2=0,1
       do side1=0,1
        if( boundaryCondition(side1,0).ge.abcEM2 .and. 
     & boundaryCondition(side1,0).le.abc5 .and. boundaryCondition(
     & side2,1).ge.abcEM2 .and. boundaryCondition(side2,1).le.abc5 
     & .and. boundaryCondition(side3,2).ge.abcEM2 .and. 
     & boundaryCondition(side3,2).le.abc5 )then
         i1=gridIndexRange(side1,0)
         i2=gridIndexRange(side2,1)
         i3=gridIndexRange(side3,2)

         is1=1-2*side1
         is2=1-2*side2
         is3=1-2*side3

         un(i1-is1,i2-is2,i3-is3,ex)=un(i1+is1,i2+is2,i3+is3,ex)
         un(i1-is1,i2-is2,i3-is3,ey)=un(i1+is1,i2+is2,i3+is3,ey)
         un(i1-is1,i2-is2,i3-is3,ez)=un(i1+is1,i2+is2,i3+is3,ez)
        end if
       end do
       end do
       end do

      end if

      ! -------------------------------------------------------------------------
      ! ------------------Loop over Sides----------------------------------------
      ! -------------------------------------------------------------------------
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
       if( boundaryCondition(0,0).lt.0 )then
         extra1a=max(0,extra1a) ! over-ride extra=-1 : assign ends in periodic directions
         extra1b=extra1a
       else
         if( boundaryCondition(0,0).eq.0 )then
           extra1a=numberOfGhostPoints  ! include interpolation points since we assign ghost points outside these
         end if
         if( boundaryCondition(1,0).eq.0 )then
           extra1b=numberOfGhostPoints
         end if
       end if
       if( boundaryCondition(0,1).lt.0 )then
        extra2a=max(0,extra2a) ! over-ride extra=-1 : assign ends in periodic directions
        extra2b=extra2a
       else
         if( boundaryCondition(0,1).eq.0 )then
           extra2a=numberOfGhostPoints
         end if
         if( boundaryCondition(1,1).eq.0 )then
           extra2b=numberOfGhostPoints
         end if
       end if
       if(  nd.eq.3 .and. boundaryCondition(0,2).lt.0 )then
        extra3a=max(0,extra3a) ! over-ride extra=-1 : assign ends in periodic directions
        extra3b=extra3a
       else
         if( boundaryCondition(0,2).eq.0 )then
           extra3a=numberOfGhostPoints
         end if
         if( boundaryCondition(1,2).eq.0 )then
           extra3b=numberOfGhostPoints
         end if
       end if
       do axis=0,nd-1
       do side=0,1
         if( boundaryCondition(side,axis).ge.abcEM2 .and. 
     & boundaryCondition(side,axis).le.abc5 )then
           ! write(*,'(" bcOpt: side,axis,bc=",3i2)') side,axis,boundaryCondition(side,axis)
           n1a=gridIndexRange(0,0)-extra1a
           n1b=gridIndexRange(1,0)+extra1b
           n2a=gridIndexRange(0,1)-extra2a
           n2b=gridIndexRange(1,1)+extra2b
           n3a=gridIndexRange(0,2)-extra3a
           n3b=gridIndexRange(1,2)+extra3b
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
           ! (js1,js2,js3) used to compute tangential derivatives
           js1=0
           js2=0
           js3=0
           if( axisp1.eq.0 )then
             js1=1-2*side
           else if( axisp1.eq.1 )then
             js2=1-2*side
           else if( axisp1.eq.2 )then
             js3=1-2*side
           else
             stop 5
           end if
           ! (ks1,ks2,ks3) used to compute second tangential derivative
           ks1=0
           ks2=0
           ks3=0
           if( axisp2.eq.0 )then
             ks1=1-2*side
           else if( axisp2.eq.1 )then
             ks2=1-2*side
           else if( axisp2.eq.2 )then
             ks3=1-2*side
           else
             stop 5
           end if


       if( gridType.eq.rectangular .and. orderOfACcuracy.eq.2 )then
        ! ***********************************************
        ! ************rectangular grid*******************
        ! ***********************************************


        ! write(*,'(" Apply abcEM2: grid,side,axis=",3i3," dt,c=",2e12.3)') grid,side,axis,dt,c
        if( nd.eq.2 )then
         if( axis.eq.0 )then
          do i3=n3a,n3b
          do i2=n2a,n2b
          do i1=n1a,n1b

           un(i1-is1,i2-is2,i3-is3,ex)=(un(i1+is1,i2+is2,i3+is3,ex)-(u(
     & i1+is1,i2+is2,i3+is3,ex)-u(i1-is1,i2-is2,i3-is3,ex))-(dxa*dt)*(
     & c1abcem2*uxx22r(i1,i2,i3,ex)+c2abcem2*uyy22r(i1,i2,i3,ex)+
     & c1abcem2*(-2.*un(i1,i2,i3,ex)+un(i1+is1,i2,i3,ex))/dxa**2+
     & c2abcem2*(un(i1,i2-1,i3,ex)-2.*un(i1,i2,i3,ex)+un(i1,i2+1,i3,
     & ex))/dx(1)**2))/(1.+c1abcem2*dt/dxa)
           un(i1-is1,i2-is2,i3-is3,ey)=(un(i1+is1,i2+is2,i3+is3,ey)-(u(
     & i1+is1,i2+is2,i3+is3,ey)-u(i1-is1,i2-is2,i3-is3,ey))-(dxa*dt)*(
     & c1abcem2*uxx22r(i1,i2,i3,ey)+c2abcem2*uyy22r(i1,i2,i3,ey)+
     & c1abcem2*(-2.*un(i1,i2,i3,ey)+un(i1+is1,i2,i3,ey))/dxa**2+
     & c2abcem2*(un(i1,i2-1,i3,ey)-2.*un(i1,i2,i3,ey)+un(i1,i2+1,i3,
     & ey))/dx(1)**2))/(1.+c1abcem2*dt/dxa)
           un(i1-is1,i2-is2,i3-is3,hz)=(un(i1+is1,i2+is2,i3+is3,hz)-(u(
     & i1+is1,i2+is2,i3+is3,hz)-u(i1-is1,i2-is2,i3-is3,hz))-(dxa*dt)*(
     & c1abcem2*uxx22r(i1,i2,i3,hz)+c2abcem2*uyy22r(i1,i2,i3,hz)+
     & c1abcem2*(-2.*un(i1,i2,i3,hz)+un(i1+is1,i2,i3,hz))/dxa**2+
     & c2abcem2*(un(i1,i2-1,i3,hz)-2.*un(i1,i2,i3,hz)+un(i1,i2+1,i3,
     & hz))/dx(1)**2))/(1.+c1abcem2*dt/dxa)

          end do
          end do
          end do
         else if( axis.eq.1 )then
          do i3=n3a,n3b
          do i2=n2a,n2b
          do i1=n1a,n1b
           un(i1-is1,i2-is2,i3-is3,ex)=(un(i1+is1,i2+is2,i3+is3,ex)-(u(
     & i1+is1,i2+is2,i3+is3,ex)-u(i1-is1,i2-is2,i3-is3,ex))-(dya*dt)*(
     & c1abcem2*uyy22r(i1,i2,i3,ex)+c2abcem2*uxx22r(i1,i2,i3,ex)+
     & c1abcem2*(-2.*un(i1,i2,i3,ex)+un(i1,i2+is2,i3,ex))/dya**2+
     & c2abcem2*(un(i1-1,i2,i3,ex)-2.*un(i1,i2,i3,ex)+un(i1+1,i2,i3,
     & ex))/dx(0)**2))/(1.+c1abcem2*dt/dya)
           un(i1-is1,i2-is2,i3-is3,ey)=(un(i1+is1,i2+is2,i3+is3,ey)-(u(
     & i1+is1,i2+is2,i3+is3,ey)-u(i1-is1,i2-is2,i3-is3,ey))-(dya*dt)*(
     & c1abcem2*uyy22r(i1,i2,i3,ey)+c2abcem2*uxx22r(i1,i2,i3,ey)+
     & c1abcem2*(-2.*un(i1,i2,i3,ey)+un(i1,i2+is2,i3,ey))/dya**2+
     & c2abcem2*(un(i1-1,i2,i3,ey)-2.*un(i1,i2,i3,ey)+un(i1+1,i2,i3,
     & ey))/dx(0)**2))/(1.+c1abcem2*dt/dya)
           un(i1-is1,i2-is2,i3-is3,hz)=(un(i1+is1,i2+is2,i3+is3,hz)-(u(
     & i1+is1,i2+is2,i3+is3,hz)-u(i1-is1,i2-is2,i3-is3,hz))-(dya*dt)*(
     & c1abcem2*uyy22r(i1,i2,i3,hz)+c2abcem2*uxx22r(i1,i2,i3,hz)+
     & c1abcem2*(-2.*un(i1,i2,i3,hz)+un(i1,i2+is2,i3,hz))/dya**2+
     & c2abcem2*(un(i1-1,i2,i3,hz)-2.*un(i1,i2,i3,hz)+un(i1+1,i2,i3,
     & hz))/dx(0)**2))/(1.+c1abcem2*dt/dya)
          end do
          end do
          end do
         else
          stop 94677
         end if

        else ! ***** 3D *****
         if( axis.eq.0 )then
          do i3=n3a,n3b
          do i2=n2a,n2b
          do i1=n1a,n1b
           un(i1-is1,i2-is2,i3-is3,ex)=(un(i1+is1,i2+is2,i3+is3,ex)-(u(
     & i1+is1,i2+is2,i3+is3,ex)-u(i1-is1,i2-is2,i3-is3,ex))-(dxa*dt)*(
     & c1abcem2*uxx23r(i1,i2,i3,ex)+c2abcem2*(uyy23r(i1,i2,i3,ex)+
     & uzz23r(i1,i2,i3,ex))+c1abcem2*(-2.*un(i1,i2,i3,ex)+un(i1+is1,
     & i2,i3,ex))/dxa**2+c2abcem2*(un(i1,i2-1,i3,ex)-2.*un(i1,i2,i3,
     & ex)+un(i1,i2+1,i3,ex))/dx(1)**2+c2abcem2*(un(i1,i2,i3-1,ex)-2.*
     & un(i1,i2,i3,ex)+un(i1,i2,i3+1,ex))/dx(2)**2))/(1.+c1abcem2*
     & dt/dxa)
           un(i1-is1,i2-is2,i3-is3,ey)=(un(i1+is1,i2+is2,i3+is3,ey)-(u(
     & i1+is1,i2+is2,i3+is3,ey)-u(i1-is1,i2-is2,i3-is3,ey))-(dxa*dt)*(
     & c1abcem2*uxx23r(i1,i2,i3,ey)+c2abcem2*(uyy23r(i1,i2,i3,ey)+
     & uzz23r(i1,i2,i3,ey))+c1abcem2*(-2.*un(i1,i2,i3,ey)+un(i1+is1,
     & i2,i3,ey))/dxa**2+c2abcem2*(un(i1,i2-1,i3,ey)-2.*un(i1,i2,i3,
     & ey)+un(i1,i2+1,i3,ey))/dx(1)**2+c2abcem2*(un(i1,i2,i3-1,ey)-2.*
     & un(i1,i2,i3,ey)+un(i1,i2,i3+1,ey))/dx(2)**2))/(1.+c1abcem2*
     & dt/dxa)
           un(i1-is1,i2-is2,i3-is3,ez)=(un(i1+is1,i2+is2,i3+is3,ez)-(u(
     & i1+is1,i2+is2,i3+is3,ez)-u(i1-is1,i2-is2,i3-is3,ez))-(dxa*dt)*(
     & c1abcem2*uxx23r(i1,i2,i3,ez)+c2abcem2*(uyy23r(i1,i2,i3,ez)+
     & uzz23r(i1,i2,i3,ez))+c1abcem2*(-2.*un(i1,i2,i3,ez)+un(i1+is1,
     & i2,i3,ez))/dxa**2+c2abcem2*(un(i1,i2-1,i3,ez)-2.*un(i1,i2,i3,
     & ez)+un(i1,i2+1,i3,ez))/dx(1)**2+c2abcem2*(un(i1,i2,i3-1,ez)-2.*
     & un(i1,i2,i3,ez)+un(i1,i2,i3+1,ez))/dx(2)**2))/(1.+c1abcem2*
     & dt/dxa)
          end do
          end do
          end do
         else if( axis.eq.1 )then
          do i3=n3a,n3b
          do i2=n2a,n2b
          do i1=n1a,n1b
           un(i1-is1,i2-is2,i3-is3,ex)=(un(i1+is1,i2+is2,i3+is3,ex)-(u(
     & i1+is1,i2+is2,i3+is3,ex)-u(i1-is1,i2-is2,i3-is3,ex))-(dya*dt)*(
     & c1abcem2*uyy23r(i1,i2,i3,ex)+c2abcem2*(uxx23r(i1,i2,i3,ex)+
     & uzz23r(i1,i2,i3,ex))+c1abcem2*(-2.*un(i1,i2,i3,ex)+un(i1,i2+
     & is2,i3,ex))/dya**2+c2abcem2*(un(i1-1,i2,i3,ex)-2.*un(i1,i2,i3,
     & ex)+un(i1+1,i2,i3,ex))/dx(0)**2+c2abcem2*(un(i1,i2,i3-1,ex)-2.*
     & un(i1,i2,i3,ex)+un(i1,i2,i3+1,ex))/dx(2)**2))/(1.+c1abcem2*
     & dt/dya)
           un(i1-is1,i2-is2,i3-is3,ey)=(un(i1+is1,i2+is2,i3+is3,ey)-(u(
     & i1+is1,i2+is2,i3+is3,ey)-u(i1-is1,i2-is2,i3-is3,ey))-(dya*dt)*(
     & c1abcem2*uyy23r(i1,i2,i3,ey)+c2abcem2*(uxx23r(i1,i2,i3,ey)+
     & uzz23r(i1,i2,i3,ey))+c1abcem2*(-2.*un(i1,i2,i3,ey)+un(i1,i2+
     & is2,i3,ey))/dya**2+c2abcem2*(un(i1-1,i2,i3,ey)-2.*un(i1,i2,i3,
     & ey)+un(i1+1,i2,i3,ey))/dx(0)**2+c2abcem2*(un(i1,i2,i3-1,ey)-2.*
     & un(i1,i2,i3,ey)+un(i1,i2,i3+1,ey))/dx(2)**2))/(1.+c1abcem2*
     & dt/dya)
           un(i1-is1,i2-is2,i3-is3,ez)=(un(i1+is1,i2+is2,i3+is3,ez)-(u(
     & i1+is1,i2+is2,i3+is3,ez)-u(i1-is1,i2-is2,i3-is3,ez))-(dya*dt)*(
     & c1abcem2*uyy23r(i1,i2,i3,ez)+c2abcem2*(uxx23r(i1,i2,i3,ez)+
     & uzz23r(i1,i2,i3,ez))+c1abcem2*(-2.*un(i1,i2,i3,ez)+un(i1,i2+
     & is2,i3,ez))/dya**2+c2abcem2*(un(i1-1,i2,i3,ez)-2.*un(i1,i2,i3,
     & ez)+un(i1+1,i2,i3,ez))/dx(0)**2+c2abcem2*(un(i1,i2,i3-1,ez)-2.*
     & un(i1,i2,i3,ez)+un(i1,i2,i3+1,ez))/dx(2)**2))/(1.+c1abcem2*
     & dt/dya)
          end do
          end do
          end do
         else if( axis.eq.2 )then
          do i3=n3a,n3b
          do i2=n2a,n2b
          do i1=n1a,n1b
           un(i1-is1,i2-is2,i3-is3,ex)=(un(i1+is1,i2+is2,i3+is3,ex)-(u(
     & i1+is1,i2+is2,i3+is3,ex)-u(i1-is1,i2-is2,i3-is3,ex))-(dza*dt)*(
     & c1abcem2*uzz23r(i1,i2,i3,ex)+.5*(uxx23r(i1,i2,i3,ex)+uyy23r(i1,
     & i2,i3,ex))+c1abcem2*(-2.*un(i1,i2,i3,ex)+un(i1,i2,i3+is3,ex))
     & /dza**2+c2abcem2*(un(i1-1,i2,i3,ex)-2.*un(i1,i2,i3,ex)+un(i1+1,
     & i2,i3,ex))/dx(0)**2+c2abcem2*(un(i1,i2-1,i3,ex)-2.*un(i1,i2,i3,
     & ex)+un(i1,i2+1,i3,ex))/dx(1)**2))/(1.+c1abcem2*dt/dza)
           un(i1-is1,i2-is2,i3-is3,ey)=(un(i1+is1,i2+is2,i3+is3,ey)-(u(
     & i1+is1,i2+is2,i3+is3,ey)-u(i1-is1,i2-is2,i3-is3,ey))-(dza*dt)*(
     & c1abcem2*uzz23r(i1,i2,i3,ey)+.5*(uxx23r(i1,i2,i3,ey)+uyy23r(i1,
     & i2,i3,ey))+c1abcem2*(-2.*un(i1,i2,i3,ey)+un(i1,i2,i3+is3,ey))
     & /dza**2+c2abcem2*(un(i1-1,i2,i3,ey)-2.*un(i1,i2,i3,ey)+un(i1+1,
     & i2,i3,ey))/dx(0)**2+c2abcem2*(un(i1,i2-1,i3,ey)-2.*un(i1,i2,i3,
     & ey)+un(i1,i2+1,i3,ey))/dx(1)**2))/(1.+c1abcem2*dt/dza)
           un(i1-is1,i2-is2,i3-is3,ez)=(un(i1+is1,i2+is2,i3+is3,ez)-(u(
     & i1+is1,i2+is2,i3+is3,ez)-u(i1-is1,i2-is2,i3-is3,ez))-(dza*dt)*(
     & c1abcem2*uzz23r(i1,i2,i3,ez)+.5*(uxx23r(i1,i2,i3,ez)+uyy23r(i1,
     & i2,i3,ez))+c1abcem2*(-2.*un(i1,i2,i3,ez)+un(i1,i2,i3+is3,ez))
     & /dza**2+c2abcem2*(un(i1-1,i2,i3,ez)-2.*un(i1,i2,i3,ez)+un(i1+1,
     & i2,i3,ez))/dx(0)**2+c2abcem2*(un(i1,i2-1,i3,ez)-2.*un(i1,i2,i3,
     & ez)+un(i1,i2+1,i3,ez))/dx(1)**2))/(1.+c1abcem2*dt/dza)
          end do
          end do
          end do
         else
          stop 46766
         end if

        end if

       else if( gridType.eq.rectangular .and. orderOfAccuracy.eq.4 )
     & then

        ! ***********************************************
        ! ************rectangular grid*******************
        ! ************ fourth-order   *******************
        ! ***********************************************


        ! >>>>>>>>>>>>>>>> this is only second-order ---- fix this <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


        ! write(*,'(" Apply abcEM2: grid,side,axis=",3i3," dt,c=",2e12.3)') grid,side,axis,dt,c
        if( nd.eq.2 )then
         if( axis.eq.0 )then
          do i3=n3a,n3b
          do i2=n2a,n2b
          do i1=n1a,n1b

           un(i1-is1,i2-is2,i3-is3,ex)=(un(i1+is1,i2+is2,i3+is3,ex)-(u(
     & i1+is1,i2+is2,i3+is3,ex)-u(i1-is1,i2-is2,i3-is3,ex))-(dxa*dt)*(
     & c1abcem2*uxx22r(i1,i2,i3,ex)+c2abcem2*uyy22r(i1,i2,i3,ex)+
     & c1abcem2*(-2.*un(i1,i2,i3,ex)+un(i1+is1,i2,i3,ex))/dxa**2+
     & c2abcem2*(un(i1,i2-1,i3,ex)-2.*un(i1,i2,i3,ex)+un(i1,i2+1,i3,
     & ex))/dx(1)**2))/(1.+c1abcem2*dt/dxa)
           un(i1-is1,i2-is2,i3-is3,ey)=(un(i1+is1,i2+is2,i3+is3,ey)-(u(
     & i1+is1,i2+is2,i3+is3,ey)-u(i1-is1,i2-is2,i3-is3,ey))-(dxa*dt)*(
     & c1abcem2*uxx22r(i1,i2,i3,ey)+c2abcem2*uyy22r(i1,i2,i3,ey)+
     & c1abcem2*(-2.*un(i1,i2,i3,ey)+un(i1+is1,i2,i3,ey))/dxa**2+
     & c2abcem2*(un(i1,i2-1,i3,ey)-2.*un(i1,i2,i3,ey)+un(i1,i2+1,i3,
     & ey))/dx(1)**2))/(1.+c1abcem2*dt/dxa)
           un(i1-is1,i2-is2,i3-is3,hz)=(un(i1+is1,i2+is2,i3+is3,hz)-(u(
     & i1+is1,i2+is2,i3+is3,hz)-u(i1-is1,i2-is2,i3-is3,hz))-(dxa*dt)*(
     & c1abcem2*uxx22r(i1,i2,i3,hz)+c2abcem2*uyy22r(i1,i2,i3,hz)+
     & c1abcem2*(-2.*un(i1,i2,i3,hz)+un(i1+is1,i2,i3,hz))/dxa**2+
     & c2abcem2*(un(i1,i2-1,i3,hz)-2.*un(i1,i2,i3,hz)+un(i1,i2+1,i3,
     & hz))/dx(1)**2))/(1.+c1abcem2*dt/dxa)

             un(i1-2*is1,i2-2*is2,i3-2*is3,ex)=4.*un(i1-is1,i2-is2,i3-
     & is3,ex)-6.*un(i1,i2,i3,ex)+4.*un(i1+is1,i2+is2,i3+is3,ex)-un(
     & i1+2*is1,i2+2*is2,i3+2*is3,ex)
             un(i1-2*is1,i2-2*is2,i3-2*is3,ey)=4.*un(i1-is1,i2-is2,i3-
     & is3,ey)-6.*un(i1,i2,i3,ey)+4.*un(i1+is1,i2+is2,i3+is3,ey)-un(
     & i1+2*is1,i2+2*is2,i3+2*is3,ey)
             un(i1-2*is1,i2-2*is2,i3-2*is3,hz)=4.*un(i1-is1,i2-is2,i3-
     & is3,hz)-6.*un(i1,i2,i3,hz)+4.*un(i1+is1,i2+is2,i3+is3,hz)-un(
     & i1+2*is1,i2+2*is2,i3+2*is3,hz)

          end do
          end do
          end do
         else if( axis.eq.1 )then
          do i3=n3a,n3b
          do i2=n2a,n2b
          do i1=n1a,n1b
           un(i1-is1,i2-is2,i3-is3,ex)=(un(i1+is1,i2+is2,i3+is3,ex)-(u(
     & i1+is1,i2+is2,i3+is3,ex)-u(i1-is1,i2-is2,i3-is3,ex))-(dya*dt)*(
     & c1abcem2*uyy22r(i1,i2,i3,ex)+c2abcem2*uxx22r(i1,i2,i3,ex)+
     & c1abcem2*(-2.*un(i1,i2,i3,ex)+un(i1,i2+is2,i3,ex))/dya**2+
     & c2abcem2*(un(i1-1,i2,i3,ex)-2.*un(i1,i2,i3,ex)+un(i1+1,i2,i3,
     & ex))/dx(0)**2))/(1.+c1abcem2*dt/dya)
           un(i1-is1,i2-is2,i3-is3,ey)=(un(i1+is1,i2+is2,i3+is3,ey)-(u(
     & i1+is1,i2+is2,i3+is3,ey)-u(i1-is1,i2-is2,i3-is3,ey))-(dya*dt)*(
     & c1abcem2*uyy22r(i1,i2,i3,ey)+c2abcem2*uxx22r(i1,i2,i3,ey)+
     & c1abcem2*(-2.*un(i1,i2,i3,ey)+un(i1,i2+is2,i3,ey))/dya**2+
     & c2abcem2*(un(i1-1,i2,i3,ey)-2.*un(i1,i2,i3,ey)+un(i1+1,i2,i3,
     & ey))/dx(0)**2))/(1.+c1abcem2*dt/dya)
           un(i1-is1,i2-is2,i3-is3,hz)=(un(i1+is1,i2+is2,i3+is3,hz)-(u(
     & i1+is1,i2+is2,i3+is3,hz)-u(i1-is1,i2-is2,i3-is3,hz))-(dya*dt)*(
     & c1abcem2*uyy22r(i1,i2,i3,hz)+c2abcem2*uxx22r(i1,i2,i3,hz)+
     & c1abcem2*(-2.*un(i1,i2,i3,hz)+un(i1,i2+is2,i3,hz))/dya**2+
     & c2abcem2*(un(i1-1,i2,i3,hz)-2.*un(i1,i2,i3,hz)+un(i1+1,i2,i3,
     & hz))/dx(0)**2))/(1.+c1abcem2*dt/dya)

             un(i1-2*is1,i2-2*is2,i3-2*is3,ex)=4.*un(i1-is1,i2-is2,i3-
     & is3,ex)-6.*un(i1,i2,i3,ex)+4.*un(i1+is1,i2+is2,i3+is3,ex)-un(
     & i1+2*is1,i2+2*is2,i3+2*is3,ex)
             un(i1-2*is1,i2-2*is2,i3-2*is3,ey)=4.*un(i1-is1,i2-is2,i3-
     & is3,ey)-6.*un(i1,i2,i3,ey)+4.*un(i1+is1,i2+is2,i3+is3,ey)-un(
     & i1+2*is1,i2+2*is2,i3+2*is3,ey)
             un(i1-2*is1,i2-2*is2,i3-2*is3,hz)=4.*un(i1-is1,i2-is2,i3-
     & is3,hz)-6.*un(i1,i2,i3,hz)+4.*un(i1+is1,i2+is2,i3+is3,hz)-un(
     & i1+2*is1,i2+2*is2,i3+2*is3,hz)

          end do
          end do
          end do
         else
          stop 9477
         end if

        else ! ***** 3D *****
         if( axis.eq.0 )then
          do i3=n3a,n3b
          do i2=n2a,n2b
          do i1=n1a,n1b
           un(i1-is1,i2-is2,i3-is3,ex)=(un(i1+is1,i2+is2,i3+is3,ex)-(u(
     & i1+is1,i2+is2,i3+is3,ex)-u(i1-is1,i2-is2,i3-is3,ex))-(dxa*dt)*(
     & c1abcem2*uxx23r(i1,i2,i3,ex)+c2abcem2*(uyy23r(i1,i2,i3,ex)+
     & uzz23r(i1,i2,i3,ex))+c1abcem2*(-2.*un(i1,i2,i3,ex)+un(i1+is1,
     & i2,i3,ex))/dxa**2+c2abcem2*(un(i1,i2-1,i3,ex)-2.*un(i1,i2,i3,
     & ex)+un(i1,i2+1,i3,ex))/dx(1)**2+c2abcem2*(un(i1,i2,i3-1,ex)-2.*
     & un(i1,i2,i3,ex)+un(i1,i2,i3+1,ex))/dx(2)**2))/(1.+c1abcem2*
     & dt/dxa)
           un(i1-is1,i2-is2,i3-is3,ey)=(un(i1+is1,i2+is2,i3+is3,ey)-(u(
     & i1+is1,i2+is2,i3+is3,ey)-u(i1-is1,i2-is2,i3-is3,ey))-(dxa*dt)*(
     & c1abcem2*uxx23r(i1,i2,i3,ey)+c2abcem2*(uyy23r(i1,i2,i3,ey)+
     & uzz23r(i1,i2,i3,ey))+c1abcem2*(-2.*un(i1,i2,i3,ey)+un(i1+is1,
     & i2,i3,ey))/dxa**2+c2abcem2*(un(i1,i2-1,i3,ey)-2.*un(i1,i2,i3,
     & ey)+un(i1,i2+1,i3,ey))/dx(1)**2+c2abcem2*(un(i1,i2,i3-1,ey)-2.*
     & un(i1,i2,i3,ey)+un(i1,i2,i3+1,ey))/dx(2)**2))/(1.+c1abcem2*
     & dt/dxa)
           un(i1-is1,i2-is2,i3-is3,ez)=(un(i1+is1,i2+is2,i3+is3,ez)-(u(
     & i1+is1,i2+is2,i3+is3,ez)-u(i1-is1,i2-is2,i3-is3,ez))-(dxa*dt)*(
     & c1abcem2*uxx23r(i1,i2,i3,ez)+c2abcem2*(uyy23r(i1,i2,i3,ez)+
     & uzz23r(i1,i2,i3,ez))+c1abcem2*(-2.*un(i1,i2,i3,ez)+un(i1+is1,
     & i2,i3,ez))/dxa**2+c2abcem2*(un(i1,i2-1,i3,ez)-2.*un(i1,i2,i3,
     & ez)+un(i1,i2+1,i3,ez))/dx(1)**2+c2abcem2*(un(i1,i2,i3-1,ez)-2.*
     & un(i1,i2,i3,ez)+un(i1,i2,i3+1,ez))/dx(2)**2))/(1.+c1abcem2*
     & dt/dxa)

             un(i1-2*is1,i2-2*is2,i3-2*is3,ex)=4.*un(i1-is1,i2-is2,i3-
     & is3,ex)-6.*un(i1,i2,i3,ex)+4.*un(i1+is1,i2+is2,i3+is3,ex)-un(
     & i1+2*is1,i2+2*is2,i3+2*is3,ex)
             un(i1-2*is1,i2-2*is2,i3-2*is3,ey)=4.*un(i1-is1,i2-is2,i3-
     & is3,ey)-6.*un(i1,i2,i3,ey)+4.*un(i1+is1,i2+is2,i3+is3,ey)-un(
     & i1+2*is1,i2+2*is2,i3+2*is3,ey)
             un(i1-2*is1,i2-2*is2,i3-2*is3,ez)=4.*un(i1-is1,i2-is2,i3-
     & is3,ez)-6.*un(i1,i2,i3,ez)+4.*un(i1+is1,i2+is2,i3+is3,ez)-un(
     & i1+2*is1,i2+2*is2,i3+2*is3,ez)
          end do
          end do
          end do
         else if( axis.eq.1 )then
          do i3=n3a,n3b
          do i2=n2a,n2b
          do i1=n1a,n1b
           un(i1-is1,i2-is2,i3-is3,ex)=(un(i1+is1,i2+is2,i3+is3,ex)-(u(
     & i1+is1,i2+is2,i3+is3,ex)-u(i1-is1,i2-is2,i3-is3,ex))-(dya*dt)*(
     & c1abcem2*uyy23r(i1,i2,i3,ex)+c2abcem2*(uxx23r(i1,i2,i3,ex)+
     & uzz23r(i1,i2,i3,ex))+c1abcem2*(-2.*un(i1,i2,i3,ex)+un(i1,i2+
     & is2,i3,ex))/dya**2+c2abcem2*(un(i1-1,i2,i3,ex)-2.*un(i1,i2,i3,
     & ex)+un(i1+1,i2,i3,ex))/dx(0)**2+c2abcem2*(un(i1,i2,i3-1,ex)-2.*
     & un(i1,i2,i3,ex)+un(i1,i2,i3+1,ex))/dx(2)**2))/(1.+c1abcem2*
     & dt/dya)
           un(i1-is1,i2-is2,i3-is3,ey)=(un(i1+is1,i2+is2,i3+is3,ey)-(u(
     & i1+is1,i2+is2,i3+is3,ey)-u(i1-is1,i2-is2,i3-is3,ey))-(dya*dt)*(
     & c1abcem2*uyy23r(i1,i2,i3,ey)+c2abcem2*(uxx23r(i1,i2,i3,ey)+
     & uzz23r(i1,i2,i3,ey))+c1abcem2*(-2.*un(i1,i2,i3,ey)+un(i1,i2+
     & is2,i3,ey))/dya**2+c2abcem2*(un(i1-1,i2,i3,ey)-2.*un(i1,i2,i3,
     & ey)+un(i1+1,i2,i3,ey))/dx(0)**2+c2abcem2*(un(i1,i2,i3-1,ey)-2.*
     & un(i1,i2,i3,ey)+un(i1,i2,i3+1,ey))/dx(2)**2))/(1.+c1abcem2*
     & dt/dya)
           un(i1-is1,i2-is2,i3-is3,ez)=(un(i1+is1,i2+is2,i3+is3,ez)-(u(
     & i1+is1,i2+is2,i3+is3,ez)-u(i1-is1,i2-is2,i3-is3,ez))-(dya*dt)*(
     & c1abcem2*uyy23r(i1,i2,i3,ez)+c2abcem2*(uxx23r(i1,i2,i3,ez)+
     & uzz23r(i1,i2,i3,ez))+c1abcem2*(-2.*un(i1,i2,i3,ez)+un(i1,i2+
     & is2,i3,ez))/dya**2+c2abcem2*(un(i1-1,i2,i3,ez)-2.*un(i1,i2,i3,
     & ez)+un(i1+1,i2,i3,ez))/dx(0)**2+c2abcem2*(un(i1,i2,i3-1,ez)-2.*
     & un(i1,i2,i3,ez)+un(i1,i2,i3+1,ez))/dx(2)**2))/(1.+c1abcem2*
     & dt/dya)

             un(i1-2*is1,i2-2*is2,i3-2*is3,ex)=4.*un(i1-is1,i2-is2,i3-
     & is3,ex)-6.*un(i1,i2,i3,ex)+4.*un(i1+is1,i2+is2,i3+is3,ex)-un(
     & i1+2*is1,i2+2*is2,i3+2*is3,ex)
             un(i1-2*is1,i2-2*is2,i3-2*is3,ey)=4.*un(i1-is1,i2-is2,i3-
     & is3,ey)-6.*un(i1,i2,i3,ey)+4.*un(i1+is1,i2+is2,i3+is3,ey)-un(
     & i1+2*is1,i2+2*is2,i3+2*is3,ey)
             un(i1-2*is1,i2-2*is2,i3-2*is3,ez)=4.*un(i1-is1,i2-is2,i3-
     & is3,ez)-6.*un(i1,i2,i3,ez)+4.*un(i1+is1,i2+is2,i3+is3,ez)-un(
     & i1+2*is1,i2+2*is2,i3+2*is3,ez)
          end do
          end do
          end do
         else if( axis.eq.2 )then
          do i3=n3a,n3b
          do i2=n2a,n2b
          do i1=n1a,n1b
           un(i1-is1,i2-is2,i3-is3,ex)=(un(i1+is1,i2+is2,i3+is3,ex)-(u(
     & i1+is1,i2+is2,i3+is3,ex)-u(i1-is1,i2-is2,i3-is3,ex))-(dza*dt)*(
     & c1abcem2*uzz23r(i1,i2,i3,ex)+.5*(uxx23r(i1,i2,i3,ex)+uyy23r(i1,
     & i2,i3,ex))+c1abcem2*(-2.*un(i1,i2,i3,ex)+un(i1,i2,i3+is3,ex))
     & /dza**2+c2abcem2*(un(i1-1,i2,i3,ex)-2.*un(i1,i2,i3,ex)+un(i1+1,
     & i2,i3,ex))/dx(0)**2+c2abcem2*(un(i1,i2-1,i3,ex)-2.*un(i1,i2,i3,
     & ex)+un(i1,i2+1,i3,ex))/dx(1)**2))/(1.+c1abcem2*dt/dza)
           un(i1-is1,i2-is2,i3-is3,ey)=(un(i1+is1,i2+is2,i3+is3,ey)-(u(
     & i1+is1,i2+is2,i3+is3,ey)-u(i1-is1,i2-is2,i3-is3,ey))-(dza*dt)*(
     & c1abcem2*uzz23r(i1,i2,i3,ey)+.5*(uxx23r(i1,i2,i3,ey)+uyy23r(i1,
     & i2,i3,ey))+c1abcem2*(-2.*un(i1,i2,i3,ey)+un(i1,i2,i3+is3,ey))
     & /dza**2+c2abcem2*(un(i1-1,i2,i3,ey)-2.*un(i1,i2,i3,ey)+un(i1+1,
     & i2,i3,ey))/dx(0)**2+c2abcem2*(un(i1,i2-1,i3,ey)-2.*un(i1,i2,i3,
     & ey)+un(i1,i2+1,i3,ey))/dx(1)**2))/(1.+c1abcem2*dt/dza)
           un(i1-is1,i2-is2,i3-is3,ez)=(un(i1+is1,i2+is2,i3+is3,ez)-(u(
     & i1+is1,i2+is2,i3+is3,ez)-u(i1-is1,i2-is2,i3-is3,ez))-(dza*dt)*(
     & c1abcem2*uzz23r(i1,i2,i3,ez)+.5*(uxx23r(i1,i2,i3,ez)+uyy23r(i1,
     & i2,i3,ez))+c1abcem2*(-2.*un(i1,i2,i3,ez)+un(i1,i2,i3+is3,ez))
     & /dza**2+c2abcem2*(un(i1-1,i2,i3,ez)-2.*un(i1,i2,i3,ez)+un(i1+1,
     & i2,i3,ez))/dx(0)**2+c2abcem2*(un(i1,i2-1,i3,ez)-2.*un(i1,i2,i3,
     & ez)+un(i1,i2+1,i3,ez))/dx(1)**2))/(1.+c1abcem2*dt/dza)

             un(i1-2*is1,i2-2*is2,i3-2*is3,ex)=4.*un(i1-is1,i2-is2,i3-
     & is3,ex)-6.*un(i1,i2,i3,ex)+4.*un(i1+is1,i2+is2,i3+is3,ex)-un(
     & i1+2*is1,i2+2*is2,i3+2*is3,ex)
             un(i1-2*is1,i2-2*is2,i3-2*is3,ey)=4.*un(i1-is1,i2-is2,i3-
     & is3,ey)-6.*un(i1,i2,i3,ey)+4.*un(i1+is1,i2+is2,i3+is3,ey)-un(
     & i1+2*is1,i2+2*is2,i3+2*is3,ey)
             un(i1-2*is1,i2-2*is2,i3-2*is3,ez)=4.*un(i1-is1,i2-is2,i3-
     & is3,ez)-6.*un(i1,i2,i3,ez)+4.*un(i1+is1,i2+is2,i3+is3,ez)-un(
     & i1+2*is1,i2+2*is2,i3+2*is3,ez)
          end do
          end do
          end do
         else
          stop 94766
         end if

        end if

       else
         stop 2255
       end if

         end if
       end do
       end do

      return
      end
