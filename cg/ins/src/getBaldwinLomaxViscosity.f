! This file automatically generated from getBaldwinLomaxViscosity.bf with bpp.

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

c **********************************************************************
c  This file contains some commonly used macros.
c **********************************************************************


c Define macros for the derivatives based on the dimension, order of accuracy and grid-type


c================================================================
c Input:
c ADTYPE: AD2, AD4, AD24
c TURBULENCE_MODEL: INS, SPAL
c Output:
c  artificialDissipation(cc) : inline macro for u,v,w
c  artificialDissipationTM(cc) : inline macro for "n" or k or epsilon
c================================================================


c================================================================
c Input:
c ADTYPE: AD2, AD4, AD24
c TURBULENCE_MODEL: INS, SPAL
c Output:
c  artificialDissipation(cc) : inline macro for u,v,w
c  artificialDissipationTM(cc) : inline macro for "n" or k or epsilon
c================================================================





c **************************************************************
c   Macro to compute Baldwin-Lomax Turbulent viscosity (from "lineSolveBL.h" )
c **************************************************************


c ================================================================================
c Define the Coefficient of Viscosity for the Baldwin-Lomax Model
c
c=================================================================================


       subroutine getBaldwinLomaxViscosity(nd,n1a,n1b,n2a,n2b,n3a,n3b,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,rsxy,xy,  u, v, 
     & dw, bc, boundaryCondition, ipar, rpar, pdb, ierr )
c======================================================================
c
c nd : number of space dimensions
c
c n1a,n1b,n2a,n2b,n3a,n3b : 
c u : current solution
c v : save results in v(i1,i2,i3,nc). v and u may be the same
c
c dw: distance to wall
c======================================================================
       implicit none
       integer nd, n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,
     & nd3b,nd4a,nd4b
       real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
       real v(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
       real dw(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
       real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
       real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
       integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
       integer bc(0:1,0:*),boundaryCondition(0:1,0:*), ierr
       integer ipar(0:*)
       real rpar(0:*)
       double precision pdb  ! pointer to data base
       !     ---- local variables -----
       integer m,n,c,i1,i2,i3,orderOfAccuracy,useWhereMask,i1p,i2p,i3p
       integer pc,uc,vc,wc,tc,nc,vsc,grid,side,gridType
       integer twilightZoneFlow
       integer indexRange(0:1,0:2),is1,is2,is3
       real nu,dx(0:2),dr(0:2)
       integer turbulenceModel,noTurbulenceModel
       integer baldwinLomax,spalartAllmaras,kEpsilon,kOmega
       parameter (noTurbulenceModel=0,baldwinLomax=1,kEpsilon=2,
     & kOmega=3,spalartAllmaras=4 )
       integer axis,kd
       real kbl,alpha,a0p,ccp,ckleb,cwk !baldwin-lomax constants
       real magu,magumax,ymax,ulmax,lmixw,lmixmax,lmix2max,vto,vort,
     & fdotn,tawu ! baldwin-lomax tmp variables
       real yscale,yplus,nmag,ftan(3),norm(3),tauw,maxvt,ctrans,ditrip,
     & kappaF ! more baldwin-lomax tmp variables
       integer iswitch, ibb, ibe, i, ii1,ii2,ii3,io(3) ! baldwin-lomax loop variables
       integer itrip,jtrip,ktrip !baldwin-lomax trip location
       character *50 name
       integer ok,getInt,getReal
       integer noSlipWall,outflow,convectiveOutflow,tractionFree,
     & inflowWithPandTV,dirichletBoundaryCondition,symmetry,
     & axisymmetric
       parameter( noSlipWall=1,outflow=5,convectiveOutflow=14,
     & tractionFree=15,inflowWithPandTV=3,
     & dirichletBoundaryCondition=12,symmetry=11,axisymmetric=13 )
       integer rectangular,curvilinear
       parameter( rectangular=0, curvilinear=1 )
       integer interpolate,dirichlet,neumann,extrapolate
       parameter( interpolate=0, dirichlet=1, neumann=2, extrapolate=3 
     & )
       integer pdeModel,standardModel,BoussinesqModel,viscoPlasticModel
       parameter( standardModel=0,BoussinesqModel=1,
     & viscoPlasticModel=2 )
      !     --- begin statement functions
       real rxi
       real uu, ux2,uy2,uz2,ux2c,uy2c,ux3c,uy3c,uz3c
       real rx,ry,rz,sx,sy,sz,tx,ty,tz
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
       rxrr2(i1,i2,i3)=(-2.*rx(i1,i2,i3)+(rx(i1+1,i2,i3)+rx(i1-1,i2,i3)
     & ) )*d22(0)
       rxss2(i1,i2,i3)=(-2.*rx(i1,i2,i3)+(rx(i1,i2+1,i3)+rx(i1,i2-1,i3)
     & ) )*d22(1)
       rxrs2(i1,i2,i3)=(rxr2(i1,i2+1,i3)-rxr2(i1,i2-1,i3))*d12(1)
       ryr2(i1,i2,i3)=(ry(i1+1,i2,i3)-ry(i1-1,i2,i3))*d12(0)
       rys2(i1,i2,i3)=(ry(i1,i2+1,i3)-ry(i1,i2-1,i3))*d12(1)
       ryt2(i1,i2,i3)=(ry(i1,i2,i3+1)-ry(i1,i2,i3-1))*d12(2)
       ryrr2(i1,i2,i3)=(-2.*ry(i1,i2,i3)+(ry(i1+1,i2,i3)+ry(i1-1,i2,i3)
     & ) )*d22(0)
       ryss2(i1,i2,i3)=(-2.*ry(i1,i2,i3)+(ry(i1,i2+1,i3)+ry(i1,i2-1,i3)
     & ) )*d22(1)
       ryrs2(i1,i2,i3)=(ryr2(i1,i2+1,i3)-ryr2(i1,i2-1,i3))*d12(1)
       rzr2(i1,i2,i3)=(rz(i1+1,i2,i3)-rz(i1-1,i2,i3))*d12(0)
       rzs2(i1,i2,i3)=(rz(i1,i2+1,i3)-rz(i1,i2-1,i3))*d12(1)
       rzt2(i1,i2,i3)=(rz(i1,i2,i3+1)-rz(i1,i2,i3-1))*d12(2)
       rzrr2(i1,i2,i3)=(-2.*rz(i1,i2,i3)+(rz(i1+1,i2,i3)+rz(i1-1,i2,i3)
     & ) )*d22(0)
       rzss2(i1,i2,i3)=(-2.*rz(i1,i2,i3)+(rz(i1,i2+1,i3)+rz(i1,i2-1,i3)
     & ) )*d22(1)
       rzrs2(i1,i2,i3)=(rzr2(i1,i2+1,i3)-rzr2(i1,i2-1,i3))*d12(1)
       sxr2(i1,i2,i3)=(sx(i1+1,i2,i3)-sx(i1-1,i2,i3))*d12(0)
       sxs2(i1,i2,i3)=(sx(i1,i2+1,i3)-sx(i1,i2-1,i3))*d12(1)
       sxt2(i1,i2,i3)=(sx(i1,i2,i3+1)-sx(i1,i2,i3-1))*d12(2)
       sxrr2(i1,i2,i3)=(-2.*sx(i1,i2,i3)+(sx(i1+1,i2,i3)+sx(i1-1,i2,i3)
     & ) )*d22(0)
       sxss2(i1,i2,i3)=(-2.*sx(i1,i2,i3)+(sx(i1,i2+1,i3)+sx(i1,i2-1,i3)
     & ) )*d22(1)
       sxrs2(i1,i2,i3)=(sxr2(i1,i2+1,i3)-sxr2(i1,i2-1,i3))*d12(1)
       syr2(i1,i2,i3)=(sy(i1+1,i2,i3)-sy(i1-1,i2,i3))*d12(0)
       sys2(i1,i2,i3)=(sy(i1,i2+1,i3)-sy(i1,i2-1,i3))*d12(1)
       syt2(i1,i2,i3)=(sy(i1,i2,i3+1)-sy(i1,i2,i3-1))*d12(2)
       syrr2(i1,i2,i3)=(-2.*sy(i1,i2,i3)+(sy(i1+1,i2,i3)+sy(i1-1,i2,i3)
     & ) )*d22(0)
       syss2(i1,i2,i3)=(-2.*sy(i1,i2,i3)+(sy(i1,i2+1,i3)+sy(i1,i2-1,i3)
     & ) )*d22(1)
       syrs2(i1,i2,i3)=(syr2(i1,i2+1,i3)-syr2(i1,i2-1,i3))*d12(1)
       szr2(i1,i2,i3)=(sz(i1+1,i2,i3)-sz(i1-1,i2,i3))*d12(0)
       szs2(i1,i2,i3)=(sz(i1,i2+1,i3)-sz(i1,i2-1,i3))*d12(1)
       szt2(i1,i2,i3)=(sz(i1,i2,i3+1)-sz(i1,i2,i3-1))*d12(2)
       szrr2(i1,i2,i3)=(-2.*sz(i1,i2,i3)+(sz(i1+1,i2,i3)+sz(i1-1,i2,i3)
     & ) )*d22(0)
       szss2(i1,i2,i3)=(-2.*sz(i1,i2,i3)+(sz(i1,i2+1,i3)+sz(i1,i2-1,i3)
     & ) )*d22(1)
       szrs2(i1,i2,i3)=(szr2(i1,i2+1,i3)-szr2(i1,i2-1,i3))*d12(1)
       txr2(i1,i2,i3)=(tx(i1+1,i2,i3)-tx(i1-1,i2,i3))*d12(0)
       txs2(i1,i2,i3)=(tx(i1,i2+1,i3)-tx(i1,i2-1,i3))*d12(1)
       txt2(i1,i2,i3)=(tx(i1,i2,i3+1)-tx(i1,i2,i3-1))*d12(2)
       txrr2(i1,i2,i3)=(-2.*tx(i1,i2,i3)+(tx(i1+1,i2,i3)+tx(i1-1,i2,i3)
     & ) )*d22(0)
       txss2(i1,i2,i3)=(-2.*tx(i1,i2,i3)+(tx(i1,i2+1,i3)+tx(i1,i2-1,i3)
     & ) )*d22(1)
       txrs2(i1,i2,i3)=(txr2(i1,i2+1,i3)-txr2(i1,i2-1,i3))*d12(1)
       tyr2(i1,i2,i3)=(ty(i1+1,i2,i3)-ty(i1-1,i2,i3))*d12(0)
       tys2(i1,i2,i3)=(ty(i1,i2+1,i3)-ty(i1,i2-1,i3))*d12(1)
       tyt2(i1,i2,i3)=(ty(i1,i2,i3+1)-ty(i1,i2,i3-1))*d12(2)
       tyrr2(i1,i2,i3)=(-2.*ty(i1,i2,i3)+(ty(i1+1,i2,i3)+ty(i1-1,i2,i3)
     & ) )*d22(0)
       tyss2(i1,i2,i3)=(-2.*ty(i1,i2,i3)+(ty(i1,i2+1,i3)+ty(i1,i2-1,i3)
     & ) )*d22(1)
       tyrs2(i1,i2,i3)=(tyr2(i1,i2+1,i3)-tyr2(i1,i2-1,i3))*d12(1)
       tzr2(i1,i2,i3)=(tz(i1+1,i2,i3)-tz(i1-1,i2,i3))*d12(0)
       tzs2(i1,i2,i3)=(tz(i1,i2+1,i3)-tz(i1,i2-1,i3))*d12(1)
       tzt2(i1,i2,i3)=(tz(i1,i2,i3+1)-tz(i1,i2,i3-1))*d12(2)
       tzrr2(i1,i2,i3)=(-2.*tz(i1,i2,i3)+(tz(i1+1,i2,i3)+tz(i1-1,i2,i3)
     & ) )*d22(0)
       tzss2(i1,i2,i3)=(-2.*tz(i1,i2,i3)+(tz(i1,i2+1,i3)+tz(i1,i2-1,i3)
     & ) )*d22(1)
       tzrs2(i1,i2,i3)=(tzr2(i1,i2+1,i3)-tzr2(i1,i2-1,i3))*d12(1)
       ux21(i1,i2,i3,kd)= rx(i1,i2,i3)*ur2(i1,i2,i3,kd)
       uy21(i1,i2,i3,kd)=0
       uz21(i1,i2,i3,kd)=0
       ux22(i1,i2,i3,kd)= rx(i1,i2,i3)*ur2(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & us2(i1,i2,i3,kd)
       uy22(i1,i2,i3,kd)= ry(i1,i2,i3)*ur2(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & us2(i1,i2,i3,kd)
       uz22(i1,i2,i3,kd)=0
       ux23(i1,i2,i3,kd)=rx(i1,i2,i3)*ur2(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & us2(i1,i2,i3,kd)+tx(i1,i2,i3)*ut2(i1,i2,i3,kd)
       uy23(i1,i2,i3,kd)=ry(i1,i2,i3)*ur2(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & us2(i1,i2,i3,kd)+ty(i1,i2,i3)*ut2(i1,i2,i3,kd)
       uz23(i1,i2,i3,kd)=rz(i1,i2,i3)*ur2(i1,i2,i3,kd)+sz(i1,i2,i3)*
     & us2(i1,i2,i3,kd)+tz(i1,i2,i3)*ut2(i1,i2,i3,kd)
       rxx21(i1,i2,i3)= rx(i1,i2,i3)*rxr2(i1,i2,i3)
       rxx22(i1,i2,i3)= rx(i1,i2,i3)*rxr2(i1,i2,i3)+sx(i1,i2,i3)*rxs2(
     & i1,i2,i3)
       rxy22(i1,i2,i3)= ry(i1,i2,i3)*rxr2(i1,i2,i3)+sy(i1,i2,i3)*rxs2(
     & i1,i2,i3)
       rxx23(i1,i2,i3)=rx(i1,i2,i3)*rxr2(i1,i2,i3)+sx(i1,i2,i3)*rxs2(
     & i1,i2,i3)+tx(i1,i2,i3)*rxt2(i1,i2,i3)
       rxy23(i1,i2,i3)=ry(i1,i2,i3)*rxr2(i1,i2,i3)+sy(i1,i2,i3)*rxs2(
     & i1,i2,i3)+ty(i1,i2,i3)*rxt2(i1,i2,i3)
       rxz23(i1,i2,i3)=rz(i1,i2,i3)*rxr2(i1,i2,i3)+sz(i1,i2,i3)*rxs2(
     & i1,i2,i3)+tz(i1,i2,i3)*rxt2(i1,i2,i3)
       ryx22(i1,i2,i3)= rx(i1,i2,i3)*ryr2(i1,i2,i3)+sx(i1,i2,i3)*rys2(
     & i1,i2,i3)
       ryy22(i1,i2,i3)= ry(i1,i2,i3)*ryr2(i1,i2,i3)+sy(i1,i2,i3)*rys2(
     & i1,i2,i3)
       ryx23(i1,i2,i3)=rx(i1,i2,i3)*ryr2(i1,i2,i3)+sx(i1,i2,i3)*rys2(
     & i1,i2,i3)+tx(i1,i2,i3)*ryt2(i1,i2,i3)
       ryy23(i1,i2,i3)=ry(i1,i2,i3)*ryr2(i1,i2,i3)+sy(i1,i2,i3)*rys2(
     & i1,i2,i3)+ty(i1,i2,i3)*ryt2(i1,i2,i3)
       ryz23(i1,i2,i3)=rz(i1,i2,i3)*ryr2(i1,i2,i3)+sz(i1,i2,i3)*rys2(
     & i1,i2,i3)+tz(i1,i2,i3)*ryt2(i1,i2,i3)
       rzx22(i1,i2,i3)= rx(i1,i2,i3)*rzr2(i1,i2,i3)+sx(i1,i2,i3)*rzs2(
     & i1,i2,i3)
       rzy22(i1,i2,i3)= ry(i1,i2,i3)*rzr2(i1,i2,i3)+sy(i1,i2,i3)*rzs2(
     & i1,i2,i3)
       rzx23(i1,i2,i3)=rx(i1,i2,i3)*rzr2(i1,i2,i3)+sx(i1,i2,i3)*rzs2(
     & i1,i2,i3)+tx(i1,i2,i3)*rzt2(i1,i2,i3)
       rzy23(i1,i2,i3)=ry(i1,i2,i3)*rzr2(i1,i2,i3)+sy(i1,i2,i3)*rzs2(
     & i1,i2,i3)+ty(i1,i2,i3)*rzt2(i1,i2,i3)
       rzz23(i1,i2,i3)=rz(i1,i2,i3)*rzr2(i1,i2,i3)+sz(i1,i2,i3)*rzs2(
     & i1,i2,i3)+tz(i1,i2,i3)*rzt2(i1,i2,i3)
       sxx22(i1,i2,i3)= rx(i1,i2,i3)*sxr2(i1,i2,i3)+sx(i1,i2,i3)*sxs2(
     & i1,i2,i3)
       sxy22(i1,i2,i3)= ry(i1,i2,i3)*sxr2(i1,i2,i3)+sy(i1,i2,i3)*sxs2(
     & i1,i2,i3)
       sxx23(i1,i2,i3)=rx(i1,i2,i3)*sxr2(i1,i2,i3)+sx(i1,i2,i3)*sxs2(
     & i1,i2,i3)+tx(i1,i2,i3)*sxt2(i1,i2,i3)
       sxy23(i1,i2,i3)=ry(i1,i2,i3)*sxr2(i1,i2,i3)+sy(i1,i2,i3)*sxs2(
     & i1,i2,i3)+ty(i1,i2,i3)*sxt2(i1,i2,i3)
       sxz23(i1,i2,i3)=rz(i1,i2,i3)*sxr2(i1,i2,i3)+sz(i1,i2,i3)*sxs2(
     & i1,i2,i3)+tz(i1,i2,i3)*sxt2(i1,i2,i3)
       syx22(i1,i2,i3)= rx(i1,i2,i3)*syr2(i1,i2,i3)+sx(i1,i2,i3)*sys2(
     & i1,i2,i3)
       syy22(i1,i2,i3)= ry(i1,i2,i3)*syr2(i1,i2,i3)+sy(i1,i2,i3)*sys2(
     & i1,i2,i3)
       syx23(i1,i2,i3)=rx(i1,i2,i3)*syr2(i1,i2,i3)+sx(i1,i2,i3)*sys2(
     & i1,i2,i3)+tx(i1,i2,i3)*syt2(i1,i2,i3)
       syy23(i1,i2,i3)=ry(i1,i2,i3)*syr2(i1,i2,i3)+sy(i1,i2,i3)*sys2(
     & i1,i2,i3)+ty(i1,i2,i3)*syt2(i1,i2,i3)
       syz23(i1,i2,i3)=rz(i1,i2,i3)*syr2(i1,i2,i3)+sz(i1,i2,i3)*sys2(
     & i1,i2,i3)+tz(i1,i2,i3)*syt2(i1,i2,i3)
       szx22(i1,i2,i3)= rx(i1,i2,i3)*szr2(i1,i2,i3)+sx(i1,i2,i3)*szs2(
     & i1,i2,i3)
       szy22(i1,i2,i3)= ry(i1,i2,i3)*szr2(i1,i2,i3)+sy(i1,i2,i3)*szs2(
     & i1,i2,i3)
       szx23(i1,i2,i3)=rx(i1,i2,i3)*szr2(i1,i2,i3)+sx(i1,i2,i3)*szs2(
     & i1,i2,i3)+tx(i1,i2,i3)*szt2(i1,i2,i3)
       szy23(i1,i2,i3)=ry(i1,i2,i3)*szr2(i1,i2,i3)+sy(i1,i2,i3)*szs2(
     & i1,i2,i3)+ty(i1,i2,i3)*szt2(i1,i2,i3)
       szz23(i1,i2,i3)=rz(i1,i2,i3)*szr2(i1,i2,i3)+sz(i1,i2,i3)*szs2(
     & i1,i2,i3)+tz(i1,i2,i3)*szt2(i1,i2,i3)
       txx22(i1,i2,i3)= rx(i1,i2,i3)*txr2(i1,i2,i3)+sx(i1,i2,i3)*txs2(
     & i1,i2,i3)
       txy22(i1,i2,i3)= ry(i1,i2,i3)*txr2(i1,i2,i3)+sy(i1,i2,i3)*txs2(
     & i1,i2,i3)
       txx23(i1,i2,i3)=rx(i1,i2,i3)*txr2(i1,i2,i3)+sx(i1,i2,i3)*txs2(
     & i1,i2,i3)+tx(i1,i2,i3)*txt2(i1,i2,i3)
       txy23(i1,i2,i3)=ry(i1,i2,i3)*txr2(i1,i2,i3)+sy(i1,i2,i3)*txs2(
     & i1,i2,i3)+ty(i1,i2,i3)*txt2(i1,i2,i3)
       txz23(i1,i2,i3)=rz(i1,i2,i3)*txr2(i1,i2,i3)+sz(i1,i2,i3)*txs2(
     & i1,i2,i3)+tz(i1,i2,i3)*txt2(i1,i2,i3)
       tyx22(i1,i2,i3)= rx(i1,i2,i3)*tyr2(i1,i2,i3)+sx(i1,i2,i3)*tys2(
     & i1,i2,i3)
       tyy22(i1,i2,i3)= ry(i1,i2,i3)*tyr2(i1,i2,i3)+sy(i1,i2,i3)*tys2(
     & i1,i2,i3)
       tyx23(i1,i2,i3)=rx(i1,i2,i3)*tyr2(i1,i2,i3)+sx(i1,i2,i3)*tys2(
     & i1,i2,i3)+tx(i1,i2,i3)*tyt2(i1,i2,i3)
       tyy23(i1,i2,i3)=ry(i1,i2,i3)*tyr2(i1,i2,i3)+sy(i1,i2,i3)*tys2(
     & i1,i2,i3)+ty(i1,i2,i3)*tyt2(i1,i2,i3)
       tyz23(i1,i2,i3)=rz(i1,i2,i3)*tyr2(i1,i2,i3)+sz(i1,i2,i3)*tys2(
     & i1,i2,i3)+tz(i1,i2,i3)*tyt2(i1,i2,i3)
       tzx22(i1,i2,i3)= rx(i1,i2,i3)*tzr2(i1,i2,i3)+sx(i1,i2,i3)*tzs2(
     & i1,i2,i3)
       tzy22(i1,i2,i3)= ry(i1,i2,i3)*tzr2(i1,i2,i3)+sy(i1,i2,i3)*tzs2(
     & i1,i2,i3)
       tzx23(i1,i2,i3)=rx(i1,i2,i3)*tzr2(i1,i2,i3)+sx(i1,i2,i3)*tzs2(
     & i1,i2,i3)+tx(i1,i2,i3)*tzt2(i1,i2,i3)
       tzy23(i1,i2,i3)=ry(i1,i2,i3)*tzr2(i1,i2,i3)+sy(i1,i2,i3)*tzs2(
     & i1,i2,i3)+ty(i1,i2,i3)*tzt2(i1,i2,i3)
       tzz23(i1,i2,i3)=rz(i1,i2,i3)*tzr2(i1,i2,i3)+sz(i1,i2,i3)*tzs2(
     & i1,i2,i3)+tz(i1,i2,i3)*tzt2(i1,i2,i3)
       uxx21(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*urr2(i1,i2,i3,kd)+(rxx22(
     & i1,i2,i3))*ur2(i1,i2,i3,kd)
       uyy21(i1,i2,i3,kd)=0
       uxy21(i1,i2,i3,kd)=0
       uxz21(i1,i2,i3,kd)=0
       uyz21(i1,i2,i3,kd)=0
       uzz21(i1,i2,i3,kd)=0
       ulaplacian21(i1,i2,i3,kd)=uxx21(i1,i2,i3,kd)
       uxx22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*urr2(i1,i2,i3,kd)+2.*(rx(
     & i1,i2,i3)*sx(i1,i2,i3))*urs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2)*
     & uss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3))*ur2(i1,i2,i3,kd)+(sxx22(i1,
     & i2,i3))*us2(i1,i2,i3,kd)
       uyy22(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*urr2(i1,i2,i3,kd)+2.*(ry(
     & i1,i2,i3)*sy(i1,i2,i3))*urs2(i1,i2,i3,kd)+(sy(i1,i2,i3)**2)*
     & uss2(i1,i2,i3,kd)+(ryy22(i1,i2,i3))*ur2(i1,i2,i3,kd)+(syy22(i1,
     & i2,i3))*us2(i1,i2,i3,kd)
       uxy22(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*urr2(i1,i2,i3,kd)+(
     & rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*urs2(i1,
     & i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*uss2(i1,i2,i3,kd)+rxy22(i1,
     & i2,i3)*ur2(i1,i2,i3,kd)+sxy22(i1,i2,i3)*us2(i1,i2,i3,kd)
       uxz22(i1,i2,i3,kd)=0
       uyz22(i1,i2,i3,kd)=0
       uzz22(i1,i2,i3,kd)=0
       ulaplacian22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & urr2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*
     & sy(i1,i2,i3))*urs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)**
     & 2)*uss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3)+ryy22(i1,i2,i3))*ur2(i1,
     & i2,i3,kd)+(sxx22(i1,i2,i3)+syy22(i1,i2,i3))*us2(i1,i2,i3,kd)
       uxx23(i1,i2,i3,kd)=rx(i1,i2,i3)**2*urr2(i1,i2,i3,kd)+sx(i1,i2,
     & i3)**2*uss2(i1,i2,i3,kd)+tx(i1,i2,i3)**2*utt2(i1,i2,i3,kd)+2.*
     & rx(i1,i2,i3)*sx(i1,i2,i3)*urs2(i1,i2,i3,kd)+2.*rx(i1,i2,i3)*tx(
     & i1,i2,i3)*urt2(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,i2,i3)*ust2(
     & i1,i2,i3,kd)+rxx23(i1,i2,i3)*ur2(i1,i2,i3,kd)+sxx23(i1,i2,i3)*
     & us2(i1,i2,i3,kd)+txx23(i1,i2,i3)*ut2(i1,i2,i3,kd)
       uyy23(i1,i2,i3,kd)=ry(i1,i2,i3)**2*urr2(i1,i2,i3,kd)+sy(i1,i2,
     & i3)**2*uss2(i1,i2,i3,kd)+ty(i1,i2,i3)**2*utt2(i1,i2,i3,kd)+2.*
     & ry(i1,i2,i3)*sy(i1,i2,i3)*urs2(i1,i2,i3,kd)+2.*ry(i1,i2,i3)*ty(
     & i1,i2,i3)*urt2(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,i2,i3)*ust2(
     & i1,i2,i3,kd)+ryy23(i1,i2,i3)*ur2(i1,i2,i3,kd)+syy23(i1,i2,i3)*
     & us2(i1,i2,i3,kd)+tyy23(i1,i2,i3)*ut2(i1,i2,i3,kd)
       uzz23(i1,i2,i3,kd)=rz(i1,i2,i3)**2*urr2(i1,i2,i3,kd)+sz(i1,i2,
     & i3)**2*uss2(i1,i2,i3,kd)+tz(i1,i2,i3)**2*utt2(i1,i2,i3,kd)+2.*
     & rz(i1,i2,i3)*sz(i1,i2,i3)*urs2(i1,i2,i3,kd)+2.*rz(i1,i2,i3)*tz(
     & i1,i2,i3)*urt2(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,i2,i3)*ust2(
     & i1,i2,i3,kd)+rzz23(i1,i2,i3)*ur2(i1,i2,i3,kd)+szz23(i1,i2,i3)*
     & us2(i1,i2,i3,kd)+tzz23(i1,i2,i3)*ut2(i1,i2,i3,kd)
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
       ulaplacian23(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*urr2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)**
     & 2+sz(i1,i2,i3)**2)*uss2(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+ty(i1,i2,
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
       uxx23r(i1,i2,i3,kd)=(-2.*u(i1,i2,i3,kd)+(u(i1+1,i2,i3,kd)+u(i1-
     & 1,i2,i3,kd)) )*h22(0)
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
       uxxx22r(i1,i2,i3,kd)=(-2.*(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))+(
     & u(i1+2,i2,i3,kd)-u(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
       uyyy22r(i1,i2,i3,kd)=(-2.*(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))+(
     & u(i1,i2+2,i3,kd)-u(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
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
       uLapSq22r(i1,i2,i3,kd)= ( 6.*u(i1,i2,i3,kd)   - 4.*(u(i1+1,i2,
     & i3,kd)+u(i1-1,i2,i3,kd))    +(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd)
     & ) )/(dx(0)**4) +( 6.*u(i1,i2,i3,kd)    -4.*(u(i1,i2+1,i3,kd)+u(
     & i1,i2-1,i3,kd))    +(u(i1,i2+2,i3,kd)+u(i1,i2-2,i3,kd)) )/(dx(
     & 1)**4)  +( 8.*u(i1,i2,i3,kd)     -4.*(u(i1+1,i2,i3,kd)+u(i1-1,
     & i2,i3,kd)+u(i1,i2+1,i3,kd)+u(i1,i2-1,i3,kd))   +2.*(u(i1+1,i2+
     & 1,i3,kd)+u(i1-1,i2+1,i3,kd)+u(i1+1,i2-1,i3,kd)+u(i1-1,i2-1,i3,
     & kd)) )/(dx(0)**2*dx(1)**2)
       uxxx23r(i1,i2,i3,kd)=(-2.*(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))+(
     & u(i1+2,i2,i3,kd)-u(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
       uyyy23r(i1,i2,i3,kd)=(-2.*(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))+(
     & u(i1,i2+2,i3,kd)-u(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
       uzzz23r(i1,i2,i3,kd)=(-2.*(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))+(
     & u(i1,i2,i3+2,kd)-u(i1,i2,i3-2,kd)) )*h22(1)*h12(2)
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
       uLapSq23r(i1,i2,i3,kd)= ( 6.*u(i1,i2,i3,kd)   - 4.*(u(i1+1,i2,
     & i3,kd)+u(i1-1,i2,i3,kd))    +(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd)
     & ) )/(dx(0)**4) +( 6.*u(i1,i2,i3,kd)    -4.*(u(i1,i2+1,i3,kd)+u(
     & i1,i2-1,i3,kd))    +(u(i1,i2+2,i3,kd)+u(i1,i2-2,i3,kd)) )/(dx(
     & 1)**4)  +( 6.*u(i1,i2,i3,kd)    -4.*(u(i1,i2,i3+1,kd)+u(i1,i2,
     & i3-1,kd))    +(u(i1,i2,i3+2,kd)+u(i1,i2,i3-2,kd)) )/(dx(2)**4) 
     &  +( 8.*u(i1,i2,i3,kd)     -4.*(u(i1+1,i2,i3,kd)  +u(i1-1,i2,i3,
     & kd)  +u(i1  ,i2+1,i3,kd)+u(i1  ,i2-1,i3,kd))   +2.*(u(i1+1,i2+
     & 1,i3,kd)+u(i1-1,i2+1,i3,kd)+u(i1+1,i2-1,i3,kd)+u(i1-1,i2-1,i3,
     & kd)) )/(dx(0)**2*dx(1)**2)+( 8.*u(i1,i2,i3,kd)     -4.*(u(i1+1,
     & i2,i3,kd)  +u(i1-1,i2,i3,kd)  +u(i1  ,i2,i3+1,kd)+u(i1  ,i2,i3-
     & 1,kd))   +2.*(u(i1+1,i2,i3+1,kd)+u(i1-1,i2,i3+1,kd)+u(i1+1,i2,
     & i3-1,kd)+u(i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)**2)+( 8.*u(i1,
     & i2,i3,kd)     -4.*(u(i1,i2+1,i3,kd)  +u(i1,i2-1,i3,kd)  +u(i1,
     & i2  ,i3+1,kd)+u(i1,i2  ,i3-1,kd))   +2.*(u(i1,i2+1,i3+1,kd)+u(
     & i1,i2-1,i3+1,kd)+u(i1,i2+1,i3-1,kd)+u(i1,i2-1,i3-1,kd)) )/(dx(
     & 1)**2*dx(2)**2)
       rxi(m,n) = rsxy(i1,i2,i3,m,n)
       uu(c)    = u(i1,i2,i3,c)
       ux2(c)   = ux22r(i1,i2,i3,c)
       uy2(c)   = uy22r(i1,i2,i3,c)
       uz2(c)   = uz23r(i1,i2,i3,c)
       ux2c(m) = ux22(i1,i2,i3,m)
       uy2c(m) = uy22(i1,i2,i3,m)
       ux3c(m) = ux23(i1,i2,i3,m)
       uy3c(m) = uy23(i1,i2,i3,m)
       uz3c(m) = uz23(i1,i2,i3,m)
       ierr=0
       ! write(*,*) 'Inside getBaldwinLomaxViscosity'
       nc                =ipar(0)
       grid              =ipar(1)
       gridType          =ipar(2)
       orderOfAccuracy   =ipar(3)
       useWhereMask      =ipar(4)
       turbulenceModel   =ipar(5)
       twilightZoneFlow  =ipar(6)
       pdeModel          =ipar(7)
       itrip = ipar(50)
       jtrip = ipar(51)
       ktrip = ipar(52)
       ! write(*,*) "BL itrip,jtrip,ktrip=",itrip,jtrip,ktrip
       dx(0)             =rpar(0)
       dx(1)             =rpar(1)
       dx(2)             =rpar(2)
       dr(0)             =rpar(3)
       dr(1)             =rpar(4)
       dr(2)             =rpar(5)
       ok = getInt(pdb,'uc',uc)
       if( ok.eq.0 )then
         write(*,'("*** getBaldwinLomaxViscosity: ERROR: uc NOT FOUND")
     & ')
       end if
       ok = getInt(pdb,'vc',vc)
       if( ok.eq.0 )then
         write(*,'("*** getBaldwinLomaxViscosity: ERROR: vc NOT FOUND")
     & ')
       end if
       ok = getInt(pdb,'wc',wc)
       if( ok.eq.0 )then
         write(*,'("*** getBaldwinLomaxViscosity: ERROR: wc NOT FOUND")
     & ')
       end if
       ok = getReal(pdb,'nu',nu)
       if( ok.eq.0 )then
         write(*,'("*** getBaldwinLomaxViscosity: ERROR: nu NOT FOUND")
     & ')
       end if
       ! assign constants for baldwin-lomax  ***** get these from the data base *****
       kbl=.4      ! kappa : Von Karman constant
       alpha=.0168
       a0p=26.
c   ccp=1.6 : wilcox 
       ccp=2.6619
       ckleb=0.3
c   cwk=1  : wilcox
       cwk=.25
       if ( turbulenceModel.ne.baldwinLomax ) then
         stop 9002
       end if
       if( .false. )then
        ! for testing -- set viscosity equal to nu 
         do i3=n3a,n3b
         do i2=n2a,n2b
         do i1=n1a,n1b
          if( mask(i1,i2,i3).ne.0 )then
         ! v(i1,i2,i3,nc)=nu
         ! v(i1,i2,i3,nc)=nu + .1*( xy(i1,i2,i3,0) + xy(i1,i2,i3,1) )
         ! v(i1,i2,i3,nc)=nu + .1*( xy(i1,i2,i3,0)**2 )
         ! v(i1,i2,i3,nc)=nu + .1*( xy(i1,i2,i3,1)**2 )
         v(i1,i2,i3,nc)=nu + .1*( xy(i1,i2,i3,0)**2 + xy(i1,i2,i3,1)**
     & 2 )
          end if
         end do
         end do
         end do
       else
         do i3=n3a,n3b
         do i2=n2a,n2b
         do i1=n1a,n1b
          v(i1,i2,i3,nc)=nu !  give a default value to all points
         end do
         end do
         end do
         maxvt=0  ! holds the maxium value for nuT
         indexRange(0,0)=n1a
         indexRange(1,0)=n1b
         indexRange(0,1)=n2a
         indexRange(1,1)=n2b
         indexRange(0,2)=n3a
         indexRange(1,2)=n3b
         ! assign loop variables to correspond to the boundary
         do axis=0,nd-1
         do side=0,1
          ! write(*,*) "BL side, axis, bc ",side,axis,boundaryCondition(side,axis)
          if( boundaryCondition(side,axis).eq.noSlipWall )then
            is1=0
            is2=0
            is3=0
            if( axis.eq.0 )then
              is1=1-2*side
              n1a=indexRange(side,axis) !-is1 ! boundary is 1 pt outside
              n1b=n1a
            else if( axis.eq.1 )then
              is2=1-2*side
              n2a=indexRange(side,axis) !-is2
              n2b=n2a
            else
              is3=1-2*side
              n3a=indexRange(side,axis) !-is3
              n3b=n3a
            end if
            io(1)=0
            io(2)=0
            io(3)=0
            io(axis+1)=1-2*side
            ibb=indexRange(0,axis)
            ! ibe=indexRange(1,axis)-1   ! wdh: Why is there a -1 here ??
            ibe=indexRange(1,axis)  ! *wdh*
            !  write(*,*) ibb,ibe
            ! loop over points on the boundary 
            do ii3=n3a,n3b
            do ii2=n2a,n2b
            do ii1=n1a,n1b
              if ( ii3.ge.ktrip .and. ii2.ge.jtrip .and. ii1.ge.itrip )
     &  then
               i1 = ii1
               i2 = ii2
               i3 = ii3
               if ( nd.eq.2 ) then
                 if ( axis.eq.0 ) then
                   ditrip = ii2-jtrip
                 else
                   ditrip = ii1-itrip
                 endif
               else
                 if ( axis.eq.0 ) then
                   ditrip = min((ii3-ktrip),(ii2-jtrip))
                 else if ( axis.eq.1 ) then
                   ditrip = min((ii1-itrip),(ii3-ktrip))
                 else
                   ditrip = min((ii1-itrip),(ii2-jtrip))
                 endif
               endif
               ctrans = (1-exp(-ditrip/3.))**2
                !   ctrans=1
                ! write(*,*) i1,i2,i3,ctrans
               ! compute the normal to the boundary 
               norm(1) = 0
               norm(2) = 0
               norm(3) = 0
               if ( gridType.eq.rectangular ) then
                 norm(axis+1)=2*side-1
               else
                norm(1) = rxi(axis,0)
                norm(2) = rxi(axis,1)
                if ( nd.eq.3 )norm(3) = rxi(axis,2)
               end if
               nmag=sqrt(norm(1)**2+norm(2)**2+norm(3)**2)
               norm(1) = norm(1)/nmag
               norm(2) = norm(2)/nmag
               norm(3) = norm(3)/nmag
               ! first compute ftan = normal.( D_i u_j + D_j u_i )
               ftan(1) = 0
               ftan(2) = 0
               ftan(3) = 0
               if ( nd.eq.2 ) then
                 if ( gridType.eq.rectangular ) then
                  ftan(1) = 2*norm(1)*ux2(uc) + norm(2)*(ux2(vc)+uy2(
     & uc))
                  ftan(2) = norm(1)*(uy2(uc)+ux2(vc)) + 2*norm(2)*uy2(
     & vc)
                 else
                  ftan(1) = 2*norm(1)*ux2c(uc) + norm(2)*(ux2c(vc)+
     & uy2c(uc))
                  ftan(2) = norm(1)*(uy2c(uc)+ux2c(vc)) + 2*norm(2)*
     & uy2c(vc)
                 end if
               else
                 if ( gridType.eq.rectangular ) then
                   ftan(1)=2*norm(1)*ux2(uc)+norm(2)*(ux2(vc)+uy2(uc)) 
     & + norm(3)*(ux2(wc)+uz2(uc))
                   ftan(2)=norm(1)*(ux2(vc)+uy2(uc)) + 2*norm(2)*uy2(
     & vc) + norm(3)*(uy2(wc)+uz2(vc))
                   ftan(3)=norm(1)*(ux2(wc)+uz2(uc)) + norm(2)*(uy2(wc)
     & +uz2(vc)) + 2*norm(3)*uz2(wc)
                 else
                   ftan(1)=2*norm(1)*ux3c(uc)+ norm(2)*(ux3c(vc)+uy3c(
     & uc)) + norm(3)*(ux3c(wc)+uz3c(uc))
                   ftan(2)=norm(1)*(ux3c(vc)+uy3c(uc)) + 2*norm(2)*
     & uy3c(vc) +  norm(3)*(uy3c(wc)+uz3c(vc))
                   ftan(3)=norm(1)*(ux3c(wc)+uz3c(uc)) + norm(2)*(uy3c(
     & wc)+uz3c(vc)) + 2*norm(3)*uz3c(wc)
                 end if
               end if
               ! Now compute tangential part by subtracting off the normal component
               fdotn = ftan(1)*norm(1)+ftan(2)*norm(2)+ftan(3)*norm(3)
               ftan(1) = ftan(1) - norm(1)*fdotn
               ftan(2) = ftan(2) - norm(2)*fdotn
               ftan(3) = ftan(3) - norm(3)*fdotn
               ! Here is the wall shear stress: 
               tauw=nu*sqrt(ftan(1)**2+ftan(2)**2+ftan(3)**2)
               ! yplus = y*yscale
               yscale = sqrt(tauw)/nu ! assuming density=1 here...
               ymax=0
               lmixmax=0
               lmix2max=0
               ! maxumag = max_y ( |v| ) 
               ! *wdh* maxumag=0
               magumax=0.
               ulmax=0.
               ! Only assign points that are closer to this wall then any other wall 
               ibe = indexRange(1,axis)
               i = ibb
               do while( i.le.ibe )
                i1 = ii1 + io(1)*i
                i2 = ii2 + io(2)*i
                i3 = ii3 + io(3)*i
                i1p= i1 + io(1)
                i2p= i2 + io(2)
                i3p= i3 + io(3)
                if( dw(i1p,i2p,i3p).le.dw(i1,i2,i3) )then
                  ibe=min(i+1,ibe)  ! choose ibe=i+1 to make sure there is a bit of overlap
                  i=ibe+1
                end if
                i=i+1
               end do
               ! write(*,'("BL:  i=",3i3," ibb,ibe=",2i3)') ii1,ii2,ii3,ibb,ibe
               do i=ibb,ibe
                i1 = ii1 + io(1)*i
                i2 = ii2 + io(2)*i
                i3 = ii3 + io(3)*i
                ! compute the norm of the vorticity:
                if (gridType.eq.rectangular) then
                  if (nd.eq.2) then
                    vort = abs(ux2(vc)-uy2(uc))
                  else
                    vort = sqrt( (uy2(wc)-uz2(vc))*(uy2(wc)-uz2(vc)) - 
     & (ux2(wc)-uz2(uc))*(ux2(wc)-uz2(uc)) + (ux2(vc)-uy2(uc))*(ux2(
     & vc)-uy2(uc)) )
                  end if
                else
                  if (nd.eq.2) then
                    vort = abs(ux2c(vc)-uy2c(uc))
                  else
                    vort = sqrt( (uy3c(wc)-uz3c(vc))*(uy3c(wc)-uz3c(vc)
     & )- (ux3c(wc)-uz3c(uc))*(ux3c(wc)-uz3c(uc))+ (ux3c(vc)-uy3c(uc))
     & *(ux3c(vc)-uy3c(uc)))
                  end if
                end if
                yplus = dw(i1,i2,i3)*yscale
                ! lmix = kappa y ( 1 - exp( y+/A+ ) )
                ! nuT(inner) = lmixw = lmix^2 * w 
                ! wdh lmixw = vort* kbl*kbl*dw(i1,i2,i3)*dw(i1,i2,i3)*(1.-exp(-yplus/a0p))**2
                kappaF = kbl*dw(i1,i2,i3)*(1.-exp(-yplus/a0p)) ! wdh
                lmixw = vort*kappaF**2
                !  write(*,*) "yplus, vort ",yplus, vort
                !  write(*,*) "dw, yscale, yplus, lmixw  is ",dw(i1,i2,i3),"  ",yscale," ",yplus," " ,lmixw
                magu = v(i1,i2,i3,uc)**2 + v(i1,i2,i3,vc)**2
                if ( nd.eq.3 ) magu = magu + v(i1,i2,i3,wc)**2
                ! magumax = max(magu,maxumag)  *wdh* there was a bug here I think : there was both magumax and maxumag
                magumax = max(magu,magumax)
                ! F(y) =  y w ( 1 - exp( y+/A+ ) )
                ! lmixmax = max_y kappa*F(y) : occurs at y=ymax, ulmax=|v|(ymax) 
                ! wdh if ( (vort*kbl*dw(i1,i2,i3)*(1.-exp(-yplus/a0p))).gt.lmixmax ) then
                if( (vort*kappaF).gt.lmixmax ) then
                  ymax = dw(i1,i2,i3)
                  ulmax = magu
                  ! wdh lmixmax = vort*kbl*dw(i1,i2,i3)*(1.-exp(-yplus/a0p))
                  lmixmax = vort*kappaF
                  lmix2max = lmixw
                  !   write(*,*) "--",i,ymax,lmixmax,lmix2max
                end if
                ! save nuT = nuT(inner)
                v(i1,i2,i3,nc) = lmixw
               end do ! i=ibb,ibe
               ! now that we know lmixmax, ulmax and maxumag we can compute the eddy viscosity
               magumax = sqrt(magumax)
               ulmax = sqrt(ulmax)
               ! NOTE: Wilcox says to take ulamx=0 for boundary layer flows ??  ************* check this **********
               ulmax=0.  ! *wdh*
               !  write(*,*) "ymax is ",ymax," lmix2max ",lmix2max
               iswitch=0
               do i=ibb,ibe
                i1 = ii1 + io(1)*i
                i2 = ii2 + io(2)*i
                i3 = ii3 + io(3)*i
                ! vto = nuT(outer) = alpha*Ccp*Fwake*Fkleb(y,ymax/Ckleb)
                !  Fwake = min( ymax*Fmax, Cwk ymax Udif**2/Fmax )
                !  FKleb(y,d) = [ 1 + 5.5 (y/d)^6 ]^{-1}
                ! ulamx = |v| at y=ymax 
                ! maxumag = max |v| 
                ! vto = alpha*ccp*min(ymax*lmixmax/kbl, cwk*ymax*(maxumag-ulmax)**2*kbl/lmixmax) / (1+5.5*(dw(i1,i2,i3)*ckleb/ymax)**6)
                vto = alpha*ccp*min(ymax*lmixmax/kbl, cwk*ymax*(
     & magumax-ulmax)**2*kbl/lmixmax) / (1+5.5*(dw(i1,i2,i3)*
     & ckleb/ymax)**6)
                !  vto = alpha*ccp*ymax*lmixmax/kbl/(1+5.5*(dw(i1,i2,i3)*ckleb/ymax)**6)
                !  write(*,*) (1+5.5*(dw(i1,i2,i3)*ckleb/ymax)**6)
                ! write(*,'("i,j,k, yplus, vti,vto,ymax,dw=",3i4,5(e9.2,1x))') i1,i2,i3,dw(i1,i2,i3)*yscale,v(i1,i2,i3,nc), vto,ymax,dw(i1,i2,i3)
                 !  write(*,*) yscale*dw(i1,i2,i3),v(i1,i2,i3,nc),vto,iswitch
                if( (iswitch.eq.0 .and. vto.lt.v(i1,i2,i3,nc)).or. 
     & iswitch.gt.0 ) then
                  ! switch to nuT(outer) when nuT(outer) = nuT(inner) 
                   !  write(*,*) "switched at ",i, v(i1,i2,i3,nc), vto
                   v(i1,i2,i3,nc) = vto
                   if ( iswitch.eq.0 ) iswitch = i
                endif
                ! scale by ctrans -- this turns on nuT after the trip point 
                v(i1,i2,i3,nc) = nu + ctrans*v(i1,i2,i3,nc)  ! *wdh* include nu
                maxvt = max(maxvt,v(i1,i2,i3,nc))
               end do ! i=ibb,ibe
               ! smooth the eddy viscosity a bit near the switch from inner to outter solutions
               do i=max(ibb+1,iswitch-5),min(iswitch+5,ibe-2)
                i1 = ii1 + io(1)*i
                i2 = ii2 + io(2)*i
                i3 = ii3 + io(3)*i
                 !  yes, the relaxation coeff. is 1.  I'm just setting it equal to the neighbors now
                 !  yes, the i+1 node uses the updated version of the i node's value             
                v(i1,i2,i3,nc) = .5*(v(i1+io(1),i2+io(2),i3+io(3),nc)+
     & v(i1-io(1),i2-io(2),i3-io(3),nc))
                 !  also, it seems the region for this smoothing should increase as the boundary
                 !  layer increases in order to improve convergence.  +- 5 was chosen through trial and
                 !  error but could be made a function of iswitch or ymax for instance.
               enddo
              else
               ! point is before the trip point 
               ! do i=ibb,ibe
               !    i1 = ii1 + io(1)*i
               !    i2 = ii2 + io(2)*i
               !    i3 = ii3 + io(3)*i
               !    v(i1,i2,i3,nc) = nu  ! *wdh* =0. 
               ! end do
              end if
            end do ! i3=i3a,i3b
            end do ! i2=i2a,2b
            end do ! i1=i1a,i1b
            ! reset values
            if( axis.eq.0 )then
              n1a=indexRange(0,axis)
              n1b=indexRange(1,axis)
            else if( axis.eq.1 )then
              n2a=indexRange(0,axis)
              n2b=indexRange(1,axis)
            else
              n3a=indexRange(0,axis)
              n3b=indexRange(1,axis)
            end if
          end if  ! end if( boundaryCondition(side,axis).eq.noSlipWall )
         end do                    ! do side
         end do                    ! do axis
         write(*,*) "BL : max(nuT) is ",maxvt
       end if
       return
       end
