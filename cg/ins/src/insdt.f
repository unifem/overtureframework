! This file automatically generated from insdt.bf with bpp.
!
! Compute du/dt for the incompressible NS on rectangular AND curvilinear grids
!


! These next include files will define the macros that will define the difference approximations
! The actual macro is called below
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








! ====================================================
! SOLVER: INS, SPAL, KE
! ====================================================


! ==========================================================
!  Advect a passive scalar -- kernel
! ==========================================================

! ==============================================================
!  Advect a passive scalar -- build loops for different cases:
!     DIM,ORDER,GRIDTYPE
! ==============================================================

! =============================================================================
! Evaluate the variable viscosity and its derivatives
! ============================================================================

! ================================================================================
! Compute the first derivatives of the thermal conductivity
!
! DIM : 2 or 3 (number of space dimensions)
! ORDER : 2 or 4 (order of accuracy)
! GRIDTYPE : rectangular or curvilinear
! ================================================================================



! ==========================================================
!  Boussinseq approximation -- kernel
!
!  Add the Boussinseq (buoyancy) term to the momentum equations and
!  evaluate the Temperature equation.
!
! DIM : 2 or 3 (number of space dimensions)
! ORDER : 2 or 4 (order of accuracy)
! GRIDTYPE : rectangular or curvilinear
! IMPEXP: EXPLICIT, EXPLICIT_ONLY, BOTH
! TMODEL :  NONE, LES (turbulence model)
! VARMAT : CONST, PIECEWISE (piece-wise constant) , VAR (variable material properties)
! ==========================================================

! ==============================================================
!  Boussinesq Model -- build loops for different cases
!
! IMPEXP: EXPLICIT, EXPLICIT_ONLY, BOTH
! TMODEL :  NONE, LES (turbulence model)
! VARMAT : CONST, PIECEWISE (piece-wise constant) , VAR (variable material properties)
!
! ==============================================================


! ==============================================================
!  Boussinesq Model -- add on gravity terms, eval T equation
!
! TMODEL :  NONE, LES (turbulence model)
! VARMAT : CONST, PIECEWISE (piece-wise constant) , VAR (variable material properties)
! ==============================================================



      subroutine insdt(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,
     & nd3a,nd3b,nd4a,nd4b,mask,xy,rsxy,radiusInverse,  u,uu, ut,uti,
     & gv,dw,  ndMatProp,matIndex,matValpc,matVal, bc, ipar, rpar, 
     & ierr )
!======================================================================
!   Compute du/dt for the incompressible NS on rectangular grids
!     OPTIMIZED version for rectangular grids.
! nd : number of space dimensions
!
! gv : gridVelocity for moving grids
! uu : for moving grids uu is a workspace to hold u-gv, otherwise uu==u
! dw : distance to the wall for some turbulence models
!======================================================================
      implicit none
      integer nd, n1a,n1b,n2a,n2b,n3a,n3b,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b

      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real uu(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real ut(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real uti(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real gv(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
      real dw(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
      real radiusInverse(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer bc(0:1,0:2),ierr

      integer ipar(0:*)
      real rpar(0:*)

      ! -- arrays for variable material properties --
      integer constantMaterialProperties
      integer piecewiseConstantMaterialProperties
      integer variableMaterialProperties
      parameter( constantMaterialProperties=0,
     &           piecewiseConstantMaterialProperties=1,
     &           variableMaterialProperties=2 )
      integer materialFormat,ndMatProp
      integer matIndex(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real matValpc(0:ndMatProp-1,0:*)
      real matVal(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)

!     ---- local variables -----
      integer c,i1,i2,i3,kd,kd3,orderOfAccuracy,gridIsMoving,
     & useWhereMask
      integer gridIsImplicit,implicitOption,implicitMethod,debug,
     & isAxisymmetric,use2ndOrderAD,use4thOrderAD
      integer pc,uc,vc,wc,sc,nc,kc,ec,tc,vsc,grid,m,advectPassiveScalar
      real nu,dt,nuPassiveScalar,adcPassiveScalar,t
      real gravity(0:2), thermalExpansivity, adcBoussinesq,kThermal,
     & kThermalLES
      real dxi,dyi,dzi,dri,dsi,dti,dr2i,ds2i,dt2i
      real ad21,ad22,ad41,ad42,cd22,cd42,adc
      real ad21n,ad22n,ad41n,ad42n,cd22n,cd42n
      real yy,ri

      integer gridType
      integer rectangular,curvilinear
      parameter( rectangular=0, curvilinear=1 )

      integer turbulenceModel,noTurbulenceModel
      integer baldwinLomax,spalartAllmaras,kEpsilon,kOmega,
     & largeEddySimulation
      parameter (noTurbulenceModel=0,baldwinLomax=1,kEpsilon=2,
     & kOmega=3,spalartAllmaras=4,largeEddySimulation=5 )

      integer computeAllTerms,
     &     doNotComputeImplicitTerms,
     &     computeImplicitTermsSeparately,
     &     computeAllWithWeightedImplicit

      parameter( computeAllTerms=0,
     &           doNotComputeImplicitTerms=1,
     &           computeImplicitTermsSeparately=2,
     &           computeAllWithWeightedImplicit=3 )

      integer pdeModel,standardModel,BoussinesqModel,viscoPlasticModel,
     & twoPhaseFlowModel
      parameter( standardModel=0,BoussinesqModel=1,viscoPlasticModel=2,
     & twoPhaseFlowModel=3 )

      integer upwindOrder
      integer advectionOption, centeredAdvection,upwindAdvection,
     & bwenoAdvection
      parameter( centeredAdvection=0, upwindAdvection=1, 
     & bwenoAdvection=2 )
      real agu(0:5,0:5) ! for holdings upwind approximations to (a.grad)u

      real cdmz,cdpz,cdzm,cdzp,cdmzz,cdpzz,cdzmz,cdzpz,cdzzm,cdzzp,
     & cdDiag,cdm,cdp
      real uxmzzR,uymzzR,uzmzzR,uxzmzR,uyzmzR,uzzmzR,uxzzmR,uyzzmR,
     & uzzzmR
      real udmzC,udzmC,udmzzC,udzmzC,udzzmC
      real admzR,adzmR,admzzR,adzmzR,adzzmR
      real admzC,adzmC,admzzC,adzmzC,adzzmC
      real admzRSA,adzmRSA,admzzRSA,adzmzRSA,adzzmRSA
      real admzCSA,adzmCSA,admzzCSA,adzmzCSA,adzzmCSA
      real adE0,adE1,ade2,adE3d0,adE3d1,adE3d2,ad2f,ad3f

      real delta22,delta23,delta42,delta43

      real adCoeff2,adCoeff4

      real ad2,ad23,ad4,ad43
      real adSelfAdjoint2dR,adSelfAdjoint3dR,adSelfAdjoint2dC,
     & adSelfAdjoint3dC
      real adSelfAdjoint2dRSA,adSelfAdjoint3dRSA,adSelfAdjoint2dCSA,
     & adSelfAdjoint3dCSA

      real rx,ry,rz,sx,sy,sz,tx,ty,tz
      real dr(0:2), dx(0:2)

      ! for SPAL TM
      real n0,n0x,n0y,n0z
      real cb1, cb2, cv1, sigma, sigmai, kappa, cw1, cw2, cw3, cw3e6, 
     & cv1e3, cd0, cr0
      real chi,chi3,fnu1,fnu2,s,r,g,fw,dKappaSq,nSqBydSq,dd
      real nuT,nuTx,nuTy,nuTz,nuTd

      real u0,u0x,u0y,u0z
      real v0,v0x,v0y,v0z
      real w0,w0x,w0y,w0z
      ! for k-epsilon
      real k0,k0x,k0y,k0z, e0,e0x,e0y,e0z
      real nuP,prod
      real cMu,cEps1,cEps2,sigmaEpsI,sigmaKI

      real rhopc,rhov,   Cppc, Cpv, thermalKpc, thermalKv, Kx, Ky, Kz, 
     & Kr, Ks, Kt

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

!     --- begin statement functions
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


!    --- 2nd order 2D artificial diffusion ---
      ad2(c)=adc*(u(i1+1,i2,i3,c)-4.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c)
     &           +u(i1,i2+1,i3,c)                 +u(i1,i2-1,i3,c))

!    --- 2nd order 3D artificial diffusion ---
      ad23(c)=adc
     &    *(u(i1+1,i2,i3,c)-6.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c)
     &     +u(i1,i2+1,i3,c)                   +u(i1,i2-1,i3,c)
     &     +u(i1,i2,i3+1,c)                   +u(i1,i2,i3-1,c))

!     ---fourth-order artificial diffusion in 2D
      ad4(c)=adc
     &    *(   -u(i1+2,i2,i3,c)-u(i1-2,i2,i3,c)
     &         -u(i1,i2+2,i3,c)-u(i1,i2-2,i3,c)
     &     +4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)
     &         +u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c))
     &      -12.*u(i1,i2,i3,c) )
!     ---fourth-order artificial diffusion in 3D
      ad43(c)=adc
     &    *(   -u(i1+2,i2,i3,c)-u(i1-2,i2,i3,c)
     &         -u(i1,i2+2,i3,c)-u(i1,i2-2,i3,c)
     &         -u(i1,i2,i3+2,c)-u(i1,i2,i3-2,c)
     &     +4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)
     &         +u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c)
     &         +u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c))
     &      -18.*u(i1,i2,i3,c) )

!    --- For 2nd order 2D artificial diffusion ---
      delta22(c)=    (u(i1+1,i2,i3,c)-4.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c) 
     &  +u(i1,i2+1,i3,c)                 +u(i1,i2-1,i3,c))
!    --- For 2nd order 3D artificial diffusion ---
      delta23(c)= (u(i1+1,i2,i3,c)-6.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c)   +
     & u(i1,i2+1,i3,c)                   +u(i1,i2-1,i3,c)  +u(i1,i2,
     & i3+1,c)                   +u(i1,i2,i3-1,c))
!     ---For fourth-order artificial diffusion in 2D
      delta42(c)= (   -u(i1+2,i2,i3,c)-u(i1-2,i2,i3,c)   -u(i1,i2+2,i3,
     & c)-u(i1,i2-2,i3,c)   +4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)   +u(
     & i1,i2+1,i3,c)+u(i1,i2-1,i3,c))  -12.*u(i1,i2,i3,c) )
!     ---For fourth-order artificial diffusion in 3D
      delta43(c)= (   -u(i1+2,i2,i3,c)-u(i1-2,i2,i3,c)  -u(i1,i2+2,i3,
     & c)-u(i1,i2-2,i3,c)  -u(i1,i2,i3+2,c)-u(i1,i2,i3-2,c)  +4.*(u(
     & i1+1,i2,i3,c)+u(i1-1,i2,i3,c)  +u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c)
     &   +u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c)) -18.*u(i1,i2,i3,c) )


      ! Face centered derivatives for the self-adjoint artificial diffusion
      !     p=plus, m=minus, z=zero
      ! Rectangular grid
      uxmzzR(i1,i2,i3,c)=(u(i1,i2,i3,c)-u(i1-1,i2,i3,c))*dxi
      uymzzR(i1,i2,i3,c)=(u(i1,i2+1,i3,c)-u(i1,i2-1,i3,c)+u(i1-1,i2+1,
     & i3,c)-u(i1-1,i2-1,i3,c))*dyi*.25
      uzmzzR(i1,i2,i3,c)=(u(i1,i2,i3+1,c)-u(i1,i2,i3-1,c)+u(i1-1,i2,i3+
     & 1,c)-u(i1-1,i2,i3-1,c))*dzi*.25

      uxzmzR(i1,i2,i3,c)=(u(i1+1,i2,i3,c)-u(i1-1,i2,i3,c)+u(i1+1,i2-1,
     & i3,c)-u(i1-1,i2-1,i3,c))*dxi*.25
      uyzmzR(i1,i2,i3,c)=(u(i1,i2,i3,c)-u(i1,i2-1,i3,c))*dyi
      uzzmzR(i1,i2,i3,c)=(u(i1,i2,i3+1,c)-u(i1,i2,i3-1,c)+u(i1,i2-1,i3+
     & 1,c)-u(i1,i2-1,i3-1,c))*dzi*.25

      uxzzmR(i1,i2,i3,c)=(u(i1+1,i2,i3,c)-u(i1-1,i2,i3,c)+u(i1+1,i2,i3-
     & 1,c)-u(i1-1,i2,i3-1,c))*dxi*.25
      uyzzmR(i1,i2,i3,c)=(u(i1,i2+1,i3,c)-u(i1,i2-1,i3,c)+u(i1,i2+1,i3-
     & 1,c)-u(i1,i2-1,i3-1,c))*dyi*.25
      uzzzmR(i1,i2,i3,c)=(u(i1,i2,i3,c)-u(i1,i2,i3-1,c))*dzi

      ! curvilinear grid
      udmzC(i1,i2,i3,m,c)=(rsxy(i1,i2,i3,0,m)+rsxy(i1-1,i2,i3,0,m))*(u(
     & i1,i2,i3,c)-u(i1-1,i2  ,i3,c))*dr2i +
     &                    (rsxy(i1,i2,i3,1,m)+rsxy(i1-1,i2,i3,1,m))*(
     & u(i1,i2+1,i3,c)-u(i1,i2-1,i3,c)+ u(i1-1,i2+1,i3,c)-u(i1-1,i2-1,
     & i3,c))*dsi*.125
      udzmC(i1,i2,i3,m,c)=(rsxy(i1,i2,i3,1,m)+rsxy(i1,i2-1,i3,1,m))*(u(
     & i1,i2,i3,c)-u(i1,i2-1,i3,c))*ds2i +
     &                    (rsxy(i1,i2,i3,0,m)+rsxy(i1,i2-1,i3,0,m))*(
     & u(i1+1,i2,i3,c)-u(i1-1,i2,i3,c)+ u(i1+1,i2-1,i3,c)-u(i1-1,i2-1,
     & i3,c))*dri*.125

      udmzzC(i1,i2,i3,m,c)=(rsxy(i1,i2,i3,0,m)+rsxy(i1-1,i2,i3,0,m))*(
     & u(i1,i2,i3,c)-u(i1-1,i2  ,i3,c))*dr2i +
     &                     (rsxy(i1,i2,i3,1,m)+rsxy(i1-1,i2,i3,1,m))*(
     & u(i1,i2+1,i3,c)-u(i1,i2-1,i3,c)+ u(i1-1,i2+1,i3,c)-u(i1-1,i2-1,
     & i3,c))*dsi*.125+
     &                     (rsxy(i1,i2,i3,2,m)+rsxy(i1-1,i2,i3,2,m))*(
     & u(i1,i2,i3+1,c)-u(i1,i2,i3-1,c)+ u(i1-1,i2,i3+1,c)-u(i1-1,i2,
     & i3-1,c))*dti*.125
      udzmzC(i1,i2,i3,m,c)=(rsxy(i1,i2,i3,1,m)+rsxy(i1,i2-1,i3,1,m))*(
     & u(i1,i2,i3,c)-u(i1,i2-1,i3,c))*ds2i +
     &                     (rsxy(i1,i2,i3,0,m)+rsxy(i1,i2-1,i3,0,m))*(
     & u(i1+1,i2,i3,c)-u(i1-1,i2,i3,c)+ u(i1+1,i2-1,i3,c)-u(i1-1,i2-1,
     & i3,c))*dri*.125+
     &                     (rsxy(i1,i2,i3,2,m)+rsxy(i1,i2-1,i3,2,m))*(
     & u(i1,i2,i3+1,c)-u(i1,i2,i3-1,c)+ u(i1,i2-1,i3+1,c)-u(i1,i2-1,
     & i3-1,c))*dti*.125

      udzzmC(i1,i2,i3,m,c)=(rsxy(i1,i2,i3,2,m)+rsxy(i1,i2,i3-1,2,m))*(
     & u(i1,i2,i3,c)-u(i1,i2,i3-1,c))*dt2i +
     &                     (rsxy(i1,i2,i3,0,m)+rsxy(i1,i2,i3-1,0,m))*(
     & u(i1+1,i2,i3,c)-u(i1-1,i2,i3,c)+ u(i1+1,i2,i3-1,c)-u(i1-1,i2,
     & i3-1,c))*dri*.125+
     &                     (rsxy(i1,i2,i3,1,m)+rsxy(i1,i2,i3-1,1,m))*(
     & u(i1,i2+1,i3,c)-u(i1,i2-1,i3,c)+ u(i1,i2+1,i3-1,c)-u(i1,i2-1,
     & i3-1,c))*dsi*.125

      ! Coefficients of the artificial diffusion for the momentum equations
      ! 2D - rectangular
      admzR(i1,i2,i3)=ad21+cd22*( abs(uxmzzR(i1,i2,i3,uc))+abs(uxmzzR(
     & i1,i2,i3,vc))+
     & abs(uymzzR(i1,i2,i3,uc))+abs(uymzzR(i1,i2,i3,vc)) )

      adzmR(i1,i2,i3)=ad21+cd22*( abs(uxzmzR(i1,i2,i3,uc))+abs(uxzmzR(
     & i1,i2,i3,vc))+
     & abs(uyzmzR(i1,i2,i3,uc))+abs(uyzmzR(i1,i2,i3,vc)) )

      ! 3D
      admzzR(i1,i2,i3)=ad21+cd22*( abs(uxmzzR(i1,i2,i3,uc))+abs(uxmzzR(
     & i1,i2,i3,vc))+abs(uxmzzR(i1,i2,i3,wc))+
     & abs(uymzzR(i1,i2,i3,uc))+abs(uymzzR(i1,i2,i3,vc))+abs(uymzzR(
     & i1,i2,i3,wc))+
     & abs(uzmzzR(i1,i2,i3,uc))+abs(uzmzzR(i1,i2,i3,vc))+abs(uzmzzR(
     & i1,i2,i3,wc)) )

      adzmzR(i1,i2,i3)=ad21+cd22*( abs(uxzmzR(i1,i2,i3,uc))+abs(uxzmzR(
     & i1,i2,i3,vc))+abs(uxzmzR(i1,i2,i3,wc))+
     & abs(uyzmzR(i1,i2,i3,uc))+abs(uyzmzR(i1,i2,i3,vc))+abs(uyzmzR(
     & i1,i2,i3,wc))+
     & abs(uzzmzR(i1,i2,i3,uc))+abs(uzzmzR(i1,i2,i3,vc))+abs(uzzmzR(
     & i1,i2,i3,wc)) )

      adzzmR(i1,i2,i3)=ad21+cd22*( abs(uxzzmR(i1,i2,i3,uc))+abs(uxzzmR(
     & i1,i2,i3,vc))+abs(uxzzmR(i1,i2,i3,wc))+
     & abs(uyzzmR(i1,i2,i3,uc))+abs(uyzzmR(i1,i2,i3,vc))+abs(uyzzmR(
     & i1,i2,i3,wc))+
     & abs(uzzzmR(i1,i2,i3,uc))+abs(uzzzmR(i1,i2,i3,vc))+abs(uzzzmR(
     & i1,i2,i3,wc)) )
      ! 2D - curvilinear
      admzC(i1,i2,i3)=ad21+cd22*( abs(udmzC(i1,i2,i3,0,uc))+abs(udmzC(
     & i1,i2,i3,0,vc))+
     & abs(udmzC(i1,i2,i3,1,uc))+abs(udmzC(i1,i2,i3,1,vc)) )

      adzmC(i1,i2,i3)=ad21+cd22*( abs(udzmC(i1,i2,i3,0,uc))+abs(udzmC(
     & i1,i2,i3,0,vc))+
     & abs(udzmC(i1,i2,i3,1,uc))+abs(udzmC(i1,i2,i3,1,vc)) )

      ! 3D
      admzzC(i1,i2,i3)=ad21+cd22*( abs(udmzzC(i1,i2,i3,0,uc))+abs(
     & udmzzC(i1,i2,i3,0,vc))+abs(udmzzC(i1,i2,i3,0,wc))+
     & abs(udmzzC(i1,i2,i3,1,uc))+abs(udmzzC(i1,i2,i3,1,vc))+abs(
     & udmzzC(i1,i2,i3,1,wc))+
     & abs(udmzzC(i1,i2,i3,2,uc))+abs(udmzzC(i1,i2,i3,2,vc))+abs(
     & udmzzC(i1,i2,i3,2,wc)) )

      adzmzC(i1,i2,i3)=ad21+cd22*( abs(udzmzC(i1,i2,i3,0,uc))+abs(
     & udzmzC(i1,i2,i3,0,vc))+abs(udzmzC(i1,i2,i3,0,wc))+
     & abs(udzmzC(i1,i2,i3,1,uc))+abs(udzmzC(i1,i2,i3,1,vc))+abs(
     & udzmzC(i1,i2,i3,1,wc))+
     & abs(udzmzC(i1,i2,i3,2,uc))+abs(udzmzC(i1,i2,i3,2,vc))+abs(
     & udzmzC(i1,i2,i3,2,wc)) )

      adzzmC(i1,i2,i3)=ad21+cd22*( abs(udzzmC(i1,i2,i3,0,uc))+abs(
     & udzzmC(i1,i2,i3,0,vc))+abs(udzzmC(i1,i2,i3,0,wc))+
     & abs(udzzmC(i1,i2,i3,1,uc))+abs(udzzmC(i1,i2,i3,1,vc))+abs(
     & udzzmC(i1,i2,i3,1,wc))+
     & abs(udzzmC(i1,i2,i3,2,uc))+abs(udzzmC(i1,i2,i3,2,vc))+abs(
     & udzzmC(i1,i2,i3,2,wc)) )

      ! Coefficients of the artificial diffusion for the SA turbulence model
      ! 2D - rectangular
      admzRSA(i1,i2,i3)=ad21n+cd22n*( abs(uxmzzR(i1,i2,i3,nc))+abs(
     & uymzzR(i1,i2,i3,nc)) )
      adzmRSA(i1,i2,i3)=ad21n+cd22n*( abs(uxzmzR(i1,i2,i3,nc))+abs(
     & uyzmzR(i1,i2,i3,nc)) )
      ! 3D
      admzzRSA(i1,i2,i3)=ad21n+cd22n*( abs(uxmzzR(i1,i2,i3,nc))+abs(
     & uymzzR(i1,i2,i3,nc))+abs(uzmzzR(i1,i2,i3,nc)) )
      adzmzRSA(i1,i2,i3)=ad21n+cd22n*( abs(uxzmzR(i1,i2,i3,nc))+abs(
     & uyzmzR(i1,i2,i3,nc))+abs(uzzmzR(i1,i2,i3,nc)) )
      adzzmRSA(i1,i2,i3)=ad21n+cd22n*( abs(uxzzmR(i1,i2,i3,nc))+abs(
     & uyzzmR(i1,i2,i3,nc))+abs(uzzzmR(i1,i2,i3,nc)) )
      ! 2D - curvilinear
      admzCSA(i1,i2,i3)=ad21n+cd22n*( abs(udmzC(i1,i2,i3,0,nc))+abs(
     & udmzC(i1,i2,i3,1,nc)) )
      adzmCSA(i1,i2,i3)=ad21n+cd22n*( abs(udzmC(i1,i2,i3,0,nc))+abs(
     & udzmC(i1,i2,i3,1,nc)) )
      ! 3D
      admzzCSA(i1,i2,i3)=ad21n+cd22n*( abs(udmzzC(i1,i2,i3,0,nc))+abs(
     & udmzzC(i1,i2,i3,1,nc))+abs(udmzzC(i1,i2,i3,2,nc)))
      adzmzCSA(i1,i2,i3)=ad21n+cd22n*( abs(udzmzC(i1,i2,i3,0,nc))+abs(
     & udzmzC(i1,i2,i3,1,nc))+abs(udzmzC(i1,i2,i3,2,nc)))
      adzzmCSA(i1,i2,i3)=ad21n+cd22n*( abs(udzzmC(i1,i2,i3,0,nc))+abs(
     & udzzmC(i1,i2,i3,1,nc))+abs(udzzmC(i1,i2,i3,2,nc)))


      ! Here are the parts of the artificial diffusion that are explicit (appear on the RHS)
      adE0(i1,i2,i3,c) = cdzm*u(i1,i2-1,i3,c)+cdzp*u(i1,i2+1,i3,c)
      adE1(i1,i2,i3,c) = cdmz*u(i1-1,i2,i3,c)+cdpz*u(i1+1,i2,i3,c)
      adE2(i1,i2,i3,c) = 0.

      adE3d0(i1,i2,i3,c) = cdzmz*u(i1,i2-1,i3,c)+cdzpz*u(i1,i2+1,i3,c)+
     & cdzzm*u(i1,i2,i3-1,c)+cdzzp*u(i1,i2,i3+1,c)
      adE3d1(i1,i2,i3,c) = cdmzz*u(i1-1,i2,i3,c)+cdpzz*u(i1+1,i2,i3,c)+
     & cdzzm*u(i1,i2,i3-1,c)+cdzzp*u(i1,i2,i3+1,c)
      adE3d2(i1,i2,i3,c) = cdmzz*u(i1-1,i2,i3,c)+cdpzz*u(i1+1,i2,i3,c)+
     & cdzmz*u(i1,i2-1,i3,c)+cdzpz*u(i1,i2+1,i3,c)

      ad2f(i1,i2,i3,m)= -cdDiag*u(i1,i2,i3,m)+cdmz*u(i1-1,i2,i3,m)+
     & cdpz*u(i1+1,i2,i3,m)+
     & cdzm*u(i1,i2-1,i3,m)+cdzp*u(i1,i2+1,i3,m)

      ad3f(i1,i2,i3,m)= -cdDiag*u(i1,i2,i3,m)+cdmzz*u(i1-1,i2,i3,m)+
     & cdpzz*u(i1+1,i2,i3,m)+
     & cdzmz*u(i1,i2-1,i3,m)+cdzpz*u(i1,i2+1,i3,m)+
     & cdzzm*u(i1,i2,i3-1,m)+cdzzp*u(i1,i2,i3+1,m)

      ! Here are the full artificial diffusion terms 
      adSelfAdjoint2dR(i1,i2,i3,c)=admzR(i1  ,i2  ,i3  )*(u(i1-1,i2,i3,
     & c)-u(i1,i2,i3,c))+
     & admzR(i1+1,i2  ,i3  )*(u(i1+1,i2,i3,c)-u(i1,i2,i3,c))+
     & adzmR(i1  ,i2  ,i3  )*(u(i1,i2-1,i3,c)-u(i1,i2,i3,c))+
     & adzmR(i1  ,i2+1,i3  )*(u(i1,i2+1,i3,c)-u(i1,i2,i3,c))

      adSelfAdjoint3dR(i1,i2,i3,c)=admzzR(i1  ,i2  ,i3  )*(u(i1-1,i2,
     & i3,c)-u(i1,i2,i3,c))+
     & admzzR(i1+1,i2  ,i3  )*(u(i1+1,i2,i3,c)-u(i1,i2,i3,c))+
     & adzmzR(i1  ,i2  ,i3  )*(u(i1,i2-1,i3,c)-u(i1,i2,i3,c))+
     & adzmzR(i1  ,i2+1,i3  )*(u(i1,i2+1,i3,c)-u(i1,i2,i3,c))+
     & adzzmR(i1  ,i2  ,i3  )*(u(i1,i2,i3-1,c)-u(i1,i2,i3,c))+
     & adzzmR(i1  ,i2  ,i3+1)*(u(i1,i2,i3+1,c)-u(i1,i2,i3,c))


      adSelfAdjoint2dC(i1,i2,i3,c)=admzC(i1  ,i2  ,i3  )*(u(i1-1,i2,i3,
     & c)-u(i1,i2,i3,c))+
     & admzC(i1+1,i2  ,i3  )*(u(i1+1,i2,i3,c)-u(i1,i2,i3,c))+
     & adzmC(i1  ,i2  ,i3  )*(u(i1,i2-1,i3,c)-u(i1,i2,i3,c))+
     & adzmC(i1  ,i2+1,i3  )*(u(i1,i2+1,i3,c)-u(i1,i2,i3,c))

      adSelfAdjoint3dC(i1,i2,i3,c)=admzzC(i1  ,i2  ,i3  )*(u(i1-1,i2,
     & i3,c)-u(i1,i2,i3,c))+
     & admzzC(i1+1,i2  ,i3  )*(u(i1+1,i2,i3,c)-u(i1,i2,i3,c))+
     & adzmzC(i1  ,i2  ,i3  )*(u(i1,i2-1,i3,c)-u(i1,i2,i3,c))+
     & adzmzC(i1  ,i2+1,i3  )*(u(i1,i2+1,i3,c)-u(i1,i2,i3,c))+
     & adzzmC(i1  ,i2  ,i3  )*(u(i1,i2,i3-1,c)-u(i1,i2,i3,c))+
     & adzzmC(i1  ,i2  ,i3+1)*(u(i1,i2,i3+1,c)-u(i1,i2,i3,c))

      ! Here are versions for the turbulence model
      adSelfAdjoint2dRSA(i1,i2,i3,c)=admzRSA(i1  ,i2  ,i3  )*(u(i1-1,
     & i2,i3,c)-u(i1,i2,i3,c))+
     & admzRSA(i1+1,i2  ,i3  )*(u(i1+1,i2,i3,c)-u(i1,i2,i3,c))+
     & adzmRSA(i1  ,i2  ,i3  )*(u(i1,i2-1,i3,c)-u(i1,i2,i3,c))+
     & adzmRSA(i1  ,i2+1,i3  )*(u(i1,i2+1,i3,c)-u(i1,i2,i3,c))

      adSelfAdjoint3dRSA(i1,i2,i3,c)=admzzRSA(i1  ,i2  ,i3  )*(u(i1-1,
     & i2,i3,c)-u(i1,i2,i3,c))+
     & admzzRSA(i1+1,i2  ,i3  )*(u(i1+1,i2,i3,c)-u(i1,i2,i3,c))+
     & adzmzRSA(i1  ,i2  ,i3  )*(u(i1,i2-1,i3,c)-u(i1,i2,i3,c))+
     & adzmzRSA(i1  ,i2+1,i3  )*(u(i1,i2+1,i3,c)-u(i1,i2,i3,c))+
     & adzzmRSA(i1  ,i2  ,i3  )*(u(i1,i2,i3-1,c)-u(i1,i2,i3,c))+
     & adzzmRSA(i1  ,i2  ,i3+1)*(u(i1,i2,i3+1,c)-u(i1,i2,i3,c))


      adSelfAdjoint2dCSA(i1,i2,i3,c)=admzCSA(i1  ,i2  ,i3  )*(u(i1-1,
     & i2,i3,c)-u(i1,i2,i3,c))+
     & admzCSA(i1+1,i2  ,i3  )*(u(i1+1,i2,i3,c)-u(i1,i2,i3,c))+
     & adzmCSA(i1  ,i2  ,i3  )*(u(i1,i2-1,i3,c)-u(i1,i2,i3,c))+
     & adzmCSA(i1  ,i2+1,i3  )*(u(i1,i2+1,i3,c)-u(i1,i2,i3,c))

      adSelfAdjoint3dCSA(i1,i2,i3,c)=admzzCSA(i1  ,i2  ,i3  )*(u(i1-1,
     & i2,i3,c)-u(i1,i2,i3,c))+
     & admzzCSA(i1+1,i2  ,i3  )*(u(i1+1,i2,i3,c)-u(i1,i2,i3,c))+
     & adzmzCSA(i1  ,i2  ,i3  )*(u(i1,i2-1,i3,c)-u(i1,i2,i3,c))+
     & adzmzCSA(i1  ,i2+1,i3  )*(u(i1,i2+1,i3,c)-u(i1,i2,i3,c))+
     & adzzmCSA(i1  ,i2  ,i3  )*(u(i1,i2,i3-1,c)-u(i1,i2,i3,c))+
     & adzzmCSA(i1  ,i2  ,i3+1)*(u(i1,i2,i3+1,c)-u(i1,i2,i3,c))



      ! -- statement functions for variable material properties
      ! (rho,Cp,k) for materialFormat=piecewiseConstantMaterialProperties
      rhopc(i1,i2,i3)      = matValpc( 0, matIndex(i1,i2,i3))
      Cppc(i1,i2,i3)       = matValpc( 1, matIndex(i1,i2,i3))
      thermalKpc(i1,i2,i3) = matValpc( 2, matIndex(i1,i2,i3))

      ! (rho,Cp,k) for materialFormat=variableMaterialProperties
      rhov(i1,i2,i3)      = matVal(i1,i2,i3,0)
      Cpv(i1,i2,i3)       = matVal(i1,i2,i3,1)
      thermalKv(i1,i2,i3) = matVal(i1,i2,i3,2)

!     --- end statement functions

      ierr=0
      ! write(*,'("Inside insdt: gridType=",i2)') gridType

      pc                 =ipar(0)
      uc                 =ipar(1)
      vc                 =ipar(2)
      wc                 =ipar(3)
      nc                 =ipar(4)
      sc                 =ipar(5)
      tc                 =ipar(6)
      grid               =ipar(7)
      orderOfAccuracy    =ipar(8)
      gridIsMoving       =ipar(9)
      useWhereMask       =ipar(10)
      gridIsImplicit     =ipar(11)
      implicitMethod     =ipar(12)
      implicitOption     =ipar(13)
      isAxisymmetric     =ipar(14)
      use2ndOrderAD      =ipar(15)
      use4thOrderAD      =ipar(16)
      advectPassiveScalar=ipar(17)
      gridType           =ipar(18)
      turbulenceModel    =ipar(19)
      pdeModel           =ipar(20)
      vsc                =ipar(21)
      ! rc               =ipar(22)
      debug              =ipar(23)
      materialFormat     =ipar(24)
      advectionOption    =ipar(25)  ! *new* 2017/01/27
      upwindOrder        =ipar(26)

      dr(0)             =rpar(0)
      dr(1)             =rpar(1)
      dr(2)             =rpar(2)
      dx(0)             =rpar(3)
      dx(1)             =rpar(4)
      dx(2)             =rpar(5)
      nu                =rpar(6)
      ad21              =rpar(7)
      ad22              =rpar(8)
      ad41              =rpar(9)
      ad42              =rpar(10)
      nuPassiveScalar   =rpar(11)
      adcPassiveScalar  =rpar(12)
      ad21n             =rpar(13)
      ad22n             =rpar(14)
      ad41n             =rpar(15)
      ad42n             =rpar(16)

      gravity(0)        =rpar(18)
      gravity(1)        =rpar(19)
      gravity(2)        =rpar(20)
      thermalExpansivity=rpar(21)
      adcBoussinesq     =rpar(22) ! coefficient of artificial diffusion for Boussinesq T equation
      kThermal          =rpar(23)
      t                 =rpar(24)

      kc=nc
      ec=kc+1

      if( orderOfAccuracy.ne.2 .and. orderOfAccuracy.ne.4 )then
        write(*,'("insdt:ERROR orderOfAccuracy=",i6)') orderOfAccuracy
        stop 1
      end if
      if( gridType.ne.rectangular .and. gridType.ne.curvilinear )then
        write(*,'("insdt:ERROR gridType=",i6)') gridType
        stop 2
      end if
      if( uc.lt.0 .or. vc.lt.0 .or. (nd.eq.3 .and. wc.lt.0) )then
        write(*,'("insdt:ERROR uc,vc,ws=",3i6)') uc,vc,wc
        stop 4
      end if

!      write(*,'("insdt: turbulenceModel=",2i6)') turbulenceModel
!      write(*,'("insdt: nd,uc,vc,wc,kc=",2i6)') nd,uc,vc,wc,kc

      if( turbulenceModel.eq.kEpsilon .and. (kc.lt.uc+nd .or. 
     & kc.gt.1000) )then
        write(*,'("insdt:ERROR in kc: nd,uc,vc,wc,kc=",2i6)') nd,uc,vc,
     & wc,kc
        stop 5
      end if

      if( advectionOption.ne.centeredAdvection .and. t.le.0. )then
        write(*,'(" insdt: advectionOption=",i2," (0=Centered,1=Upwind,
     & 2=Bweno)")') advectionOption
        write(*,'(" insdt: upwindOrder=",i2, " (-1=default)")') 
     & upwindOrder
      end if
      ! --- Output rho, Cp and kThermal t=0 for testing ---
      if( materialFormat.ne.0 .and. t.le.0 .and. (nd1b-nd1a)*(nd2b-
     & nd2a).lt. 1000 )then

       write(*,'("insdt: variable material properties rho,Cp,kThermal 
     & for T")')
       write(*,'("insdt: rho:")')
       i3=nd3a
       do i2=nd2b,nd2a,-1
         if( materialFormat.eq.piecewiseConstantMaterialProperties )
     & then
          write(*,9000) (rhopc(i1,i2,i3),i1=nd1a,nd1b)
         else
          write(*,9000) (rhov(i1,i2,i3),i1=nd1a,nd1b)
         end if
       end do
       write(*,'("insdt: Cp:")')
       do i2=nd2b,nd2a,-1
         if( materialFormat.eq.piecewiseConstantMaterialProperties )
     & then
          write(*,9000) (Cppc(i1,i2,i3),i1=nd1a,nd1b)
         else
          write(*,9000) (Cpv(i1,i2,i3),i1=nd1a,nd1b)
         end if
       end do
       write(*,'("insdt: thermalConductivity:")')
       do i2=nd2b,nd2a,-1
         if( materialFormat.eq.piecewiseConstantMaterialProperties )
     & then
          write(*,9000) (thermalKpc(i1,i2,i3),i1=nd1a,nd1b)
         else
          write(*,9000) (thermalKv(i1,i2,i3),i1=nd1a,nd1b)
         end if
       end do
 9000  format(100(f5.1))

      end if

! ** these are needed by self-adjoint terms **fix**
      dxi=1./dx(0)
      dyi=1./dx(1)
      dzi=1./dx(2)
!     dx2i=1./(2.*dx(0))
!     dy2i=1./(2.*dx(1))
!     dz2i=1./(2.*dx(2))

      dri=1./dr(0)
      dsi=1./dr(1)
      dti=1./dr(2)
      dr2i=1./(2.*dr(0))
      ds2i=1./(2.*dr(1))
      dt2i=1./(2.*dr(2))

      if( turbulenceModel.eq.spalartAllmaras )then
        call getSpalartAllmarasParameters(cb1, cb2, cv1, sigma, sigmai,
     &  kappa, cw1, cw2, cw3, cw3e6, cv1e3, cd0, cr0)
      else if( turbulenceModel.eq.kEpsilon )then

       ! write(*,'(" insdt: k-epsilon: nc,kc,ec=",3i3)') nc,kc,ec

        call getKEpsilonParameters( cMu,cEps1,cEps2,sigmaEpsI,sigmaKI )
        !  write(*,'(" insdt: cMu,cEps1,cEps2,sigmaEpsI,sigmaKI=",5f8.3)') cMu,cEps1,cEps2,sigmaEpsI,sigmaKI

      else if( turbulenceModel.eq.largeEddySimulation )then

      else if( turbulenceModel.ne.noTurbulenceModel )then
        write(*,'(" insdt:ERROR: turbulenceModel=",i4," not expected")
     & ') turbulenceModel
        stop 88
      end if

      adc=adcPassiveScalar ! coefficient of linear artificial diffusion
      cd22=ad22/(nd**2)
      cd42=ad42/(nd**2)

      if( gridIsMoving.ne.0 )then
        ! compute uu = u -gv
        if( nd.eq.2 )then
          if( useWhereMask.ne.0 )then
           do i3=n3a,n3b
           do i2=n2a,n2b
           do i1=n1a,n1b
            if( mask(i1,i2,i3).gt.0 )then
             uu(i1,i2,i3,uc)=u(i1,i2,i3,uc)-gv(i1,i2,i3,0)
             uu(i1,i2,i3,vc)=u(i1,i2,i3,vc)-gv(i1,i2,i3,1)
            end if
           end do
           end do
           end do
          else
           do i3=n3a,n3b
           do i2=n2a,n2b
           do i1=n1a,n1b
            uu(i1,i2,i3,uc)=u(i1,i2,i3,uc)-gv(i1,i2,i3,0)
            uu(i1,i2,i3,vc)=u(i1,i2,i3,vc)-gv(i1,i2,i3,1)
           end do
           end do
           end do
          end if
        else if( nd.eq.3 )then
          if( useWhereMask.ne.0 )then
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
              if( mask(i1,i2,i3).gt.0 )then
                uu(i1,i2,i3,uc)=u(i1,i2,i3,uc)-gv(i1,i2,i3,0)
                uu(i1,i2,i3,vc)=u(i1,i2,i3,vc)-gv(i1,i2,i3,1)
                uu(i1,i2,i3,wc)=u(i1,i2,i3,wc)-gv(i1,i2,i3,2)
              end if
            end do
            end do
            end do
          else
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
                uu(i1,i2,i3,uc)=u(i1,i2,i3,uc)-gv(i1,i2,i3,0)
                uu(i1,i2,i3,vc)=u(i1,i2,i3,vc)-gv(i1,i2,i3,1)
                uu(i1,i2,i3,wc)=u(i1,i2,i3,wc)-gv(i1,i2,i3,2)
            end do
            end do
            end do
          end if
        else
          stop 11
        end if
      end if

!     *********************************      
!     ********MAIN LOOPS***************      
!     *********************************      

      if( (turbulenceModel.eq.noTurbulenceModel .and. 
     & pdeModel.eq.viscoPlasticModel) .or. 
     & turbulenceModel.eq.largeEddySimulation )then
        ! ins + visco-plastic model, or LES
        if( debug.gt.2 )then
          write(*,'(" insdt: compute du/dt for generic viscosity (VP), 
     & t=",e10.2)') t
        endif
         if( orderOfAccuracy.eq.2 )then
          if( nd.eq.2 )then
            call insdtVP2dOrder2(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,xy,rsxy,radiusInverse,  u,
     & uu, ut,uti,gv,dw,  bc, ipar, rpar, ierr )
          else
            call insdtVP3dOrder2(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,xy,rsxy,radiusInverse,  u,
     & uu, ut,uti,gv,dw,  bc, ipar, rpar, ierr )
          end if
        ! #If "VP" ne "VP" && "VP" ne "VD"
         else if( orderOfAccuracy.eq.4 )then
          if( nd.eq.2 )then
            call insdtVP2dOrder4(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,xy,rsxy,radiusInverse,  u,
     & uu, ut,uti,gv,dw,  bc, ipar, rpar, ierr )
          else
            call insdtVP3dOrder4(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,xy,rsxy,radiusInverse,  u,
     & uu, ut,uti,gv,dw,  bc, ipar, rpar, ierr )
          end if
         else
           stop 1111
         end if

      else if( turbulenceModel.eq.noTurbulenceModel )then

         if( orderOfAccuracy.eq.2 )then
          if( nd.eq.2 )then
            call insdtINS2dOrder2(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,xy,rsxy,radiusInverse,  u,
     & uu, ut,uti,gv,dw,  bc, ipar, rpar, ierr )
          else
            call insdtINS3dOrder2(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,xy,rsxy,radiusInverse,  u,
     & uu, ut,uti,gv,dw,  bc, ipar, rpar, ierr )
          end if
        ! #If "INS" ne "VP" && "INS" ne "VD"
         else if( orderOfAccuracy.eq.4 )then
          if( nd.eq.2 )then
            call insdtINS2dOrder4(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,xy,rsxy,radiusInverse,  u,
     & uu, ut,uti,gv,dw,  bc, ipar, rpar, ierr )
          else
            call insdtINS3dOrder4(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,xy,rsxy,radiusInverse,  u,
     & uu, ut,uti,gv,dw,  bc, ipar, rpar, ierr )
          end if
         else
           stop 1111
         end if

      else if( turbulenceModel.eq.spalartAllmaras )then

         if( orderOfAccuracy.eq.2 )then
          if( nd.eq.2 )then
            call insdtSPAL2dOrder2(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,
     & nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,xy,rsxy,radiusInverse, 
     &  u,uu, ut,uti,gv,dw,  bc, ipar, rpar, ierr )
          else
            call insdtSPAL3dOrder2(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,
     & nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,xy,rsxy,radiusInverse, 
     &  u,uu, ut,uti,gv,dw,  bc, ipar, rpar, ierr )
          end if
        ! #If "SPAL" ne "VP" && "SPAL" ne "VD"
         else if( orderOfAccuracy.eq.4 )then
          if( nd.eq.2 )then
            call insdtSPAL2dOrder4(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,
     & nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,xy,rsxy,radiusInverse, 
     &  u,uu, ut,uti,gv,dw,  bc, ipar, rpar, ierr )
          else
            call insdtSPAL3dOrder4(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,
     & nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,xy,rsxy,radiusInverse, 
     &  u,uu, ut,uti,gv,dw,  bc, ipar, rpar, ierr )
          end if
         else
           stop 1111
         end if

      else if( turbulenceModel.eq.kEpsilon )then

         if( orderOfAccuracy.eq.2 )then
          if( nd.eq.2 )then
            call insdtKE2dOrder2(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,xy,rsxy,radiusInverse,  u,
     & uu, ut,uti,gv,dw,  bc, ipar, rpar, ierr )
          else
            call insdtKE3dOrder2(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,xy,rsxy,radiusInverse,  u,
     & uu, ut,uti,gv,dw,  bc, ipar, rpar, ierr )
          end if
        ! #If "KE" ne "VP" && "KE" ne "VD"
         else if( orderOfAccuracy.eq.4 )then
          if( nd.eq.2 )then
            call insdtKE2dOrder4(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,xy,rsxy,radiusInverse,  u,
     & uu, ut,uti,gv,dw,  bc, ipar, rpar, ierr )
          else
            call insdtKE3dOrder4(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,xy,rsxy,radiusInverse,  u,
     & uu, ut,uti,gv,dw,  bc, ipar, rpar, ierr )
          end if
         else
           stop 1111
         end if

      else
        write(*,'("Unknown turbulence model")')
        stop 68
      end if


!     *********************************
!     ******** passive scalar *********
!     *********************************

      if( advectPassiveScalar.eq.1 )then
         if( gridType.eq.rectangular )then
          if( orderOfAccuracy.eq.2 )then
            if( nd.eq.2 )then
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                if( mask(i1,i2,i3).gt.0 )then
                  ut(i1,i2,i3,sc)= -uu(i1,i2,i3,uc)*ux22r(i1,i2,i3,sc)-
     & uu(i1,i2,i3,vc)*uy22r(i1,i2,i3,sc)+nuPassiveScalar*
     & ulaplacian22r(i1,i2,i3,sc)+adcPassiveScalar*delta22(sc)
                end if
               end do
               end do
               end do
            else
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                if( mask(i1,i2,i3).gt.0 )then
                  ut(i1,i2,i3,sc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,sc)-
     & uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,sc)-uu(i1,i2,i3,wc)*uz23r(i1,i2,
     & i3,sc)+nuPassiveScalar*ulaplacian23r(i1,i2,i3,sc)+
     & adcPassiveScalar*delta23(sc)
                end if
               end do
               end do
               end do
            end if
          else if( orderOfAccuracy.eq.4 )then
            if( nd.eq.2 )then
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                if( mask(i1,i2,i3).gt.0 )then
                  ut(i1,i2,i3,sc)= -uu(i1,i2,i3,uc)*ux42r(i1,i2,i3,sc)-
     & uu(i1,i2,i3,vc)*uy42r(i1,i2,i3,sc)+nuPassiveScalar*
     & ulaplacian42r(i1,i2,i3,sc)+adcPassiveScalar*delta42(sc)
                end if
               end do
               end do
               end do
            else
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                if( mask(i1,i2,i3).gt.0 )then
                  ut(i1,i2,i3,sc)= -uu(i1,i2,i3,uc)*ux43r(i1,i2,i3,sc)-
     & uu(i1,i2,i3,vc)*uy43r(i1,i2,i3,sc)-uu(i1,i2,i3,wc)*uz43r(i1,i2,
     & i3,sc)+nuPassiveScalar*ulaplacian43r(i1,i2,i3,sc)+
     & adcPassiveScalar*delta43(sc)
                end if
               end do
               end do
               end do
            end if
          else
           stop 1281
          end if
         else if( gridType.eq.curvilinear )then
          if( orderOfAccuracy.eq.2 )then
            if( nd.eq.2 )then
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                if( mask(i1,i2,i3).gt.0 )then
                  ut(i1,i2,i3,sc)= -uu(i1,i2,i3,uc)*ux22(i1,i2,i3,sc)-
     & uu(i1,i2,i3,vc)*uy22(i1,i2,i3,sc)+nuPassiveScalar*ulaplacian22(
     & i1,i2,i3,sc)+adcPassiveScalar*delta22(sc)
                end if
               end do
               end do
               end do
            else
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                if( mask(i1,i2,i3).gt.0 )then
                  ut(i1,i2,i3,sc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,sc)-
     & uu(i1,i2,i3,vc)*uy23(i1,i2,i3,sc)-uu(i1,i2,i3,wc)*uz23(i1,i2,
     & i3,sc)+nuPassiveScalar*ulaplacian23(i1,i2,i3,sc)+
     & adcPassiveScalar*delta23(sc)
                end if
               end do
               end do
               end do
            end if
          else if( orderOfAccuracy.eq.4 )then
            if( nd.eq.2 )then
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                if( mask(i1,i2,i3).gt.0 )then
                  ut(i1,i2,i3,sc)= -uu(i1,i2,i3,uc)*ux42(i1,i2,i3,sc)-
     & uu(i1,i2,i3,vc)*uy42(i1,i2,i3,sc)+nuPassiveScalar*ulaplacian42(
     & i1,i2,i3,sc)+adcPassiveScalar*delta42(sc)
                end if
               end do
               end do
               end do
            else
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                if( mask(i1,i2,i3).gt.0 )then
                  ut(i1,i2,i3,sc)= -uu(i1,i2,i3,uc)*ux43(i1,i2,i3,sc)-
     & uu(i1,i2,i3,vc)*uy43(i1,i2,i3,sc)-uu(i1,i2,i3,wc)*uz43(i1,i2,
     & i3,sc)+nuPassiveScalar*ulaplacian43(i1,i2,i3,sc)+
     & adcPassiveScalar*delta43(sc)
                end if
               end do
               end do
               end do
            end if
          else
           stop 1282
          end if
         else
           stop 1717
         end if
      end if


!     *********************************
!     ******** Boussinesq Model *******
!     *********************************

      ! write(*,'("insdt: pdeModel=",i2," kThermal=",e10.2)') pdeModel,kThermal
      ! ' 

      if( pdeModel.eq.BoussinesqModel .or. 
     & pdeModel.eq.viscoPlasticModel )then
        if( tc.lt.0 )then
          write(*,'("insdt:Boussinesq:ERROR: tc<0 !")')
          stop 8868
        end if
        if( turbulenceModel.eq.noTurbulenceModel )then

          if( materialFormat.eq.constantMaterialProperties )then
            ! const thermal diffusivity:
             if( gridIsImplicit.eq.0 )then ! explicit
                 if( gridType.eq.rectangular )then
                  if( orderOfAccuracy.eq.2 )then
                    if( nd.eq.2 )then
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! constant thermal diffusivity
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux22r(i1,
     & i2,i3,tc)-uu(i1,i2,i3,vc)*uy22r(i1,i2,i3,tc) +(kThermal*
     & ulaplacian22r(i1,i2,i3,tc)) +adcBoussinesq*delta2 2(tc)
                        if( isAxisymmetric.eq.1 )then
                          ! -- add on axisymmetric corrections ---
                          ri=radiusInverse(i1,i2,i3)
                          if( ri.ne.0. )then
                            ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uy22r(i1,i2,i3,tc)*ri )
                          else
                            ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uyy22r(i1,i2,i3,tc) )
                          end if
                        end if
                        end if
                       end do
                       end do
                       end do
                    else
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                            ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)-gravity(2)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! constant thermal diffusivity
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux23r(i1,
     & i2,i3,tc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,tc)-uu(i1,i2,i3,wc)*
     & uz23r(i1,i2,i3,tc) +(kThermal*ulaplacian23r(i1,i2,i3,tc)) +
     & adcBoussinesq*delta2 3(tc)
                        if( isAxisymmetric.eq.1 )then
                          ! -- add on axisymmetric corrections ---
                          ri=radiusInverse(i1,i2,i3)
                          if( ri.ne.0. )then
                            ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uy23r(i1,i2,i3,tc)*ri )
                          else
                            ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uyy23r(i1,i2,i3,tc) )
                          end if
                        end if
                        end if
                       end do
                       end do
                       end do
                    end if
                  else if( orderOfAccuracy.eq.4 )then
                    if( nd.eq.2 )then
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! constant thermal diffusivity
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux42r(i1,
     & i2,i3,tc)-uu(i1,i2,i3,vc)*uy42r(i1,i2,i3,tc) +(kThermal*
     & ulaplacian42r(i1,i2,i3,tc)) +adcBoussinesq*delta4 2(tc)
                        if( isAxisymmetric.eq.1 )then
                          ! -- add on axisymmetric corrections ---
                          ri=radiusInverse(i1,i2,i3)
                          if( ri.ne.0. )then
                            ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uy42r(i1,i2,i3,tc)*ri )
                          else
                            ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uyy42r(i1,i2,i3,tc) )
                          end if
                        end if
                        end if
                       end do
                       end do
                       end do
                    else
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                            ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)-gravity(2)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! constant thermal diffusivity
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux43r(i1,
     & i2,i3,tc)-uu(i1,i2,i3,vc)*uy43r(i1,i2,i3,tc)-uu(i1,i2,i3,wc)*
     & uz43r(i1,i2,i3,tc) +(kThermal*ulaplacian43r(i1,i2,i3,tc)) +
     & adcBoussinesq*delta4 3(tc)
                        if( isAxisymmetric.eq.1 )then
                          ! -- add on axisymmetric corrections ---
                          ri=radiusInverse(i1,i2,i3)
                          if( ri.ne.0. )then
                            ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uy43r(i1,i2,i3,tc)*ri )
                          else
                            ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uyy43r(i1,i2,i3,tc) )
                          end if
                        end if
                        end if
                       end do
                       end do
                       end do
                    end if
                  else
                   stop 2281
                  end if
                 else if( gridType.eq.curvilinear )then
                  if( orderOfAccuracy.eq.2 )then
                    if( nd.eq.2 )then
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! constant thermal diffusivity
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux22(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy22(i1,i2,i3,tc) +(kThermal*
     & ulaplacian22(i1,i2,i3,tc)) +adcBoussinesq*delta2 2(tc)
                        if( isAxisymmetric.eq.1 )then
                          ! -- add on axisymmetric corrections ---
                          ri=radiusInverse(i1,i2,i3)
                          if( ri.ne.0. )then
                            ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uy22(i1,i2,i3,tc)*ri )
                          else
                            ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uyy22(i1,i2,i3,tc) )
                          end if
                        end if
                        end if
                       end do
                       end do
                       end do
                    else
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                            ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)-gravity(2)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! constant thermal diffusivity
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy23(i1,i2,i3,tc)-uu(i1,i2,i3,wc)*uz23(
     & i1,i2,i3,tc) +(kThermal*ulaplacian23(i1,i2,i3,tc)) +
     & adcBoussinesq*delta2 3(tc)
                        if( isAxisymmetric.eq.1 )then
                          ! -- add on axisymmetric corrections ---
                          ri=radiusInverse(i1,i2,i3)
                          if( ri.ne.0. )then
                            ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uy23(i1,i2,i3,tc)*ri )
                          else
                            ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uyy23(i1,i2,i3,tc) )
                          end if
                        end if
                        end if
                       end do
                       end do
                       end do
                    end if
                  else if( orderOfAccuracy.eq.4 )then
                    if( nd.eq.2 )then
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! constant thermal diffusivity
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux42(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy42(i1,i2,i3,tc) +(kThermal*
     & ulaplacian42(i1,i2,i3,tc)) +adcBoussinesq*delta4 2(tc)
                        if( isAxisymmetric.eq.1 )then
                          ! -- add on axisymmetric corrections ---
                          ri=radiusInverse(i1,i2,i3)
                          if( ri.ne.0. )then
                            ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uy42(i1,i2,i3,tc)*ri )
                          else
                            ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uyy42(i1,i2,i3,tc) )
                          end if
                        end if
                        end if
                       end do
                       end do
                       end do
                    else
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                            ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)-gravity(2)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! constant thermal diffusivity
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux43(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy43(i1,i2,i3,tc)-uu(i1,i2,i3,wc)*uz43(
     & i1,i2,i3,tc) +(kThermal*ulaplacian43(i1,i2,i3,tc)) +
     & adcBoussinesq*delta4 3(tc)
                        if( isAxisymmetric.eq.1 )then
                          ! -- add on axisymmetric corrections ---
                          ri=radiusInverse(i1,i2,i3)
                          if( ri.ne.0. )then
                            ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uy43(i1,i2,i3,tc)*ri )
                          else
                            ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uyy43(i1,i2,i3,tc) )
                          end if
                        end if
                        end if
                       end do
                       end do
                       end do
                    end if
                  else
                   stop 2282
                  end if
                 else
                   stop 2717
                 end if
             else ! ***** implicit *******
              if( implicitOption .eq.computeImplicitTermsSeparately )
     & then
                 if( gridType.eq.rectangular )then
                  if( orderOfAccuracy.eq.2 )then
                    if( nd.eq.2 )then
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! constant thermal diffusivity
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux22r(i1,
     & i2,i3,tc)-uu(i1,i2,i3,vc)*uy22r(i1,i2,i3,tc)+adcBoussinesq*
     & delta2 2(tc)
                       ! include implicit terms - diffusion
                       uti(i1,i2,i3,tc)= (kThermal*ulaplacian22r(i1,i2,
     & i3,tc))
                       if( isAxisymmetric.eq.1 )then
                         ri=radiusInverse(i1,i2,i3)
                         if( ri.ne.0. )then
                           uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*(
     &  uy22r(i1,i2,i3,tc)*ri )
                         else
                           uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*(
     &  uyy22r(i1,i2,i3,tc) )
                         end if
                       end if
                        end if
                       end do
                       end do
                       end do
                    else
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                            ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)-gravity(2)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! constant thermal diffusivity
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux23r(i1,
     & i2,i3,tc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,tc)-uu(i1,i2,i3,wc)*
     & uz23r(i1,i2,i3,tc)+adcBoussinesq*delta2 3(tc)
                       ! include implicit terms - diffusion
                       uti(i1,i2,i3,tc)= (kThermal*ulaplacian23r(i1,i2,
     & i3,tc))
                       if( isAxisymmetric.eq.1 )then
                         ri=radiusInverse(i1,i2,i3)
                         if( ri.ne.0. )then
                           uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*(
     &  uy23r(i1,i2,i3,tc)*ri )
                         else
                           uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*(
     &  uyy23r(i1,i2,i3,tc) )
                         end if
                       end if
                        end if
                       end do
                       end do
                       end do
                    end if
                  else if( orderOfAccuracy.eq.4 )then
                    if( nd.eq.2 )then
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! constant thermal diffusivity
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux42r(i1,
     & i2,i3,tc)-uu(i1,i2,i3,vc)*uy42r(i1,i2,i3,tc)+adcBoussinesq*
     & delta4 2(tc)
                       ! include implicit terms - diffusion
                       uti(i1,i2,i3,tc)= (kThermal*ulaplacian42r(i1,i2,
     & i3,tc))
                       if( isAxisymmetric.eq.1 )then
                         ri=radiusInverse(i1,i2,i3)
                         if( ri.ne.0. )then
                           uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*(
     &  uy42r(i1,i2,i3,tc)*ri )
                         else
                           uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*(
     &  uyy42r(i1,i2,i3,tc) )
                         end if
                       end if
                        end if
                       end do
                       end do
                       end do
                    else
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                            ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)-gravity(2)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! constant thermal diffusivity
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux43r(i1,
     & i2,i3,tc)-uu(i1,i2,i3,vc)*uy43r(i1,i2,i3,tc)-uu(i1,i2,i3,wc)*
     & uz43r(i1,i2,i3,tc)+adcBoussinesq*delta4 3(tc)
                       ! include implicit terms - diffusion
                       uti(i1,i2,i3,tc)= (kThermal*ulaplacian43r(i1,i2,
     & i3,tc))
                       if( isAxisymmetric.eq.1 )then
                         ri=radiusInverse(i1,i2,i3)
                         if( ri.ne.0. )then
                           uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*(
     &  uy43r(i1,i2,i3,tc)*ri )
                         else
                           uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*(
     &  uyy43r(i1,i2,i3,tc) )
                         end if
                       end if
                        end if
                       end do
                       end do
                       end do
                    end if
                  else
                   stop 2281
                  end if
                 else if( gridType.eq.curvilinear )then
                  if( orderOfAccuracy.eq.2 )then
                    if( nd.eq.2 )then
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! constant thermal diffusivity
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux22(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy22(i1,i2,i3,tc)+adcBoussinesq*delta2 
     & 2(tc)
                       ! include implicit terms - diffusion
                       uti(i1,i2,i3,tc)= (kThermal*ulaplacian22(i1,i2,
     & i3,tc))
                       if( isAxisymmetric.eq.1 )then
                         ri=radiusInverse(i1,i2,i3)
                         if( ri.ne.0. )then
                           uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*(
     &  uy22(i1,i2,i3,tc)*ri )
                         else
                           uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*(
     &  uyy22(i1,i2,i3,tc) )
                         end if
                       end if
                        end if
                       end do
                       end do
                       end do
                    else
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                            ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)-gravity(2)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! constant thermal diffusivity
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy23(i1,i2,i3,tc)-uu(i1,i2,i3,wc)*uz23(
     & i1,i2,i3,tc)+adcBoussinesq*delta2 3(tc)
                       ! include implicit terms - diffusion
                       uti(i1,i2,i3,tc)= (kThermal*ulaplacian23(i1,i2,
     & i3,tc))
                       if( isAxisymmetric.eq.1 )then
                         ri=radiusInverse(i1,i2,i3)
                         if( ri.ne.0. )then
                           uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*(
     &  uy23(i1,i2,i3,tc)*ri )
                         else
                           uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*(
     &  uyy23(i1,i2,i3,tc) )
                         end if
                       end if
                        end if
                       end do
                       end do
                       end do
                    end if
                  else if( orderOfAccuracy.eq.4 )then
                    if( nd.eq.2 )then
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! constant thermal diffusivity
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux42(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy42(i1,i2,i3,tc)+adcBoussinesq*delta4 
     & 2(tc)
                       ! include implicit terms - diffusion
                       uti(i1,i2,i3,tc)= (kThermal*ulaplacian42(i1,i2,
     & i3,tc))
                       if( isAxisymmetric.eq.1 )then
                         ri=radiusInverse(i1,i2,i3)
                         if( ri.ne.0. )then
                           uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*(
     &  uy42(i1,i2,i3,tc)*ri )
                         else
                           uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*(
     &  uyy42(i1,i2,i3,tc) )
                         end if
                       end if
                        end if
                       end do
                       end do
                       end do
                    else
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                            ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)-gravity(2)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! constant thermal diffusivity
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux43(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy43(i1,i2,i3,tc)-uu(i1,i2,i3,wc)*uz43(
     & i1,i2,i3,tc)+adcBoussinesq*delta4 3(tc)
                       ! include implicit terms - diffusion
                       uti(i1,i2,i3,tc)= (kThermal*ulaplacian43(i1,i2,
     & i3,tc))
                       if( isAxisymmetric.eq.1 )then
                         ri=radiusInverse(i1,i2,i3)
                         if( ri.ne.0. )then
                           uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*(
     &  uy43(i1,i2,i3,tc)*ri )
                         else
                           uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*(
     &  uyy43(i1,i2,i3,tc) )
                         end if
                       end if
                        end if
                       end do
                       end do
                       end do
                    end if
                  else
                   stop 2282
                  end if
                 else
                   stop 2717
                 end if
              else if( implicitOption.eq.doNotComputeImplicitTerms )
     & then
                 if( gridType.eq.rectangular )then
                  if( orderOfAccuracy.eq.2 )then
                    if( nd.eq.2 )then
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! constant thermal diffusivity
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux22r(i1,
     & i2,i3,tc)-uu(i1,i2,i3,vc)*uy22r(i1,i2,i3,tc)+adcBoussinesq*
     & delta2 2(tc)
                        end if
                       end do
                       end do
                       end do
                    else
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                            ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)-gravity(2)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! constant thermal diffusivity
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux23r(i1,
     & i2,i3,tc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,tc)-uu(i1,i2,i3,wc)*
     & uz23r(i1,i2,i3,tc)+adcBoussinesq*delta2 3(tc)
                        end if
                       end do
                       end do
                       end do
                    end if
                  else if( orderOfAccuracy.eq.4 )then
                    if( nd.eq.2 )then
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! constant thermal diffusivity
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux42r(i1,
     & i2,i3,tc)-uu(i1,i2,i3,vc)*uy42r(i1,i2,i3,tc)+adcBoussinesq*
     & delta4 2(tc)
                        end if
                       end do
                       end do
                       end do
                    else
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                            ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)-gravity(2)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! constant thermal diffusivity
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux43r(i1,
     & i2,i3,tc)-uu(i1,i2,i3,vc)*uy43r(i1,i2,i3,tc)-uu(i1,i2,i3,wc)*
     & uz43r(i1,i2,i3,tc)+adcBoussinesq*delta4 3(tc)
                        end if
                       end do
                       end do
                       end do
                    end if
                  else
                   stop 2281
                  end if
                 else if( gridType.eq.curvilinear )then
                  if( orderOfAccuracy.eq.2 )then
                    if( nd.eq.2 )then
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! constant thermal diffusivity
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux22(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy22(i1,i2,i3,tc)+adcBoussinesq*delta2 
     & 2(tc)
                        end if
                       end do
                       end do
                       end do
                    else
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                            ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)-gravity(2)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! constant thermal diffusivity
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy23(i1,i2,i3,tc)-uu(i1,i2,i3,wc)*uz23(
     & i1,i2,i3,tc)+adcBoussinesq*delta2 3(tc)
                        end if
                       end do
                       end do
                       end do
                    end if
                  else if( orderOfAccuracy.eq.4 )then
                    if( nd.eq.2 )then
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! constant thermal diffusivity
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux42(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy42(i1,i2,i3,tc)+adcBoussinesq*delta4 
     & 2(tc)
                        end if
                       end do
                       end do
                       end do
                    else
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                            ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)-gravity(2)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! constant thermal diffusivity
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux43(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy43(i1,i2,i3,tc)-uu(i1,i2,i3,wc)*uz43(
     & i1,i2,i3,tc)+adcBoussinesq*delta4 3(tc)
                        end if
                       end do
                       end do
                       end do
                    end if
                  else
                   stop 2282
                  end if
                 else
                   stop 2717
                 end if
              else
               write(*,*)'insdt:boussinesq Unknown implicitOption=',
     & implicitOption
               stop 4135
              end if  ! end implicitOption
             end if
          else if( 
     & materialFormat.eq.piecewiseConstantMaterialProperties )then
            ! piece-wise constant material properties
            ! write(*,'(" insdt: piece-wise constant material property Heat Eqn...")')
             if( gridIsImplicit.eq.0 )then ! explicit
                 if( gridType.eq.rectangular )then
                  if( orderOfAccuracy.eq.2 )then
                    if( nd.eq.2 )then
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! Do this for now -- finish me - missing a term - use a conservative approx.  ****************************************************
                          ! Kx = (thermalKpc(i1+1,i2,i3)-thermalKpc(i1-1,i2,i3))/(2.*dx(0))
                          ! Ky = (thermalKpc(i1,i2+1,i3)-thermalKpc(i1,i2-1,i3))/(2.*dx(1))
                          ! Compute Kx, Ky, Kz:
                           ! --- For now we only do second-order accurate---
                            ! --- rectangular grid ---
                              Kx = (thermalKpc(i1+1,i2,i3)-thermalKpc(
     & i1-1,i2,i3))/(2.*dx(0))
                              Ky = (thermalKpc(i1,i2+1,i3)-thermalKpc(
     & i1,i2-1,i3))/(2.*dx(1))
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux22r(i1,
     & i2,i3,tc)-uu(i1,i2,i3,vc)*uy22r(i1,i2,i3,tc) +((thermalKpc(i1,
     & i2,i3)*ulaplacian22r(i1,i2,i3,tc)+Kx*ux22r(i1,i2,i3,tc)+Ky*
     & uy22r(i1,i2,i3,tc))/(rhopc(i1,i2,i3)*Cppc(i1,i2,i3))) +
     & adcBoussinesq*delta2 2(tc)
                        if( isAxisymmetric.eq.1 )then
                          ! -- add on axisymmetric corrections ---
                            ! finish me for axisymmetric and variable material properties
                            stop 8568
                          ri=radiusInverse(i1,i2,i3)
                          if( ri.ne.0. )then
                            ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uy22r(i1,i2,i3,tc)*ri )
                          else
                            ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uyy22r(i1,i2,i3,tc) )
                          end if
                        end if
                        end if
                       end do
                       end do
                       end do
                    else
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                            ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)-gravity(2)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! Do this for now -- finish me - missing a term - use a conservative approx.  ****************************************************
                          ! Kx = (thermalKpc(i1+1,i2,i3)-thermalKpc(i1-1,i2,i3))/(2.*dx(0))
                          ! Ky = (thermalKpc(i1,i2+1,i3)-thermalKpc(i1,i2-1,i3))/(2.*dx(1))
                          ! Compute Kx, Ky, Kz:
                           ! --- For now we only do second-order accurate---
                            ! --- rectangular grid ---
                              Kx = (thermalKpc(i1+1,i2,i3)-thermalKpc(
     & i1-1,i2,i3))/(2.*dx(0))
                              Ky = (thermalKpc(i1,i2+1,i3)-thermalKpc(
     & i1,i2-1,i3))/(2.*dx(1))
                                Kz = (thermalKpc(i1,i2,i3+1)-
     & thermalKpc(i1,i2,i3-1))/(2.*dx(2))
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux23r(i1,
     & i2,i3,tc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,tc)-uu(i1,i2,i3,wc)*
     & uz23r(i1,i2,i3,tc) +((thermalKpc(i1,i2,i3)*ulaplacian23r(i1,i2,
     & i3,tc)+Kx*ux23r(i1,i2,i3,tc)+Ky*uy23r(i1,i2,i3,tc))/(rhopc(i1,
     & i2,i3)*Cppc(i1,i2,i3))) +adcBoussinesq*delta2 3(tc)
                        if( isAxisymmetric.eq.1 )then
                          ! -- add on axisymmetric corrections ---
                            ! finish me for axisymmetric and variable material properties
                            stop 8568
                          ri=radiusInverse(i1,i2,i3)
                          if( ri.ne.0. )then
                            ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uy23r(i1,i2,i3,tc)*ri )
                          else
                            ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uyy23r(i1,i2,i3,tc) )
                          end if
                        end if
                        end if
                       end do
                       end do
                       end do
                    end if
                  else if( orderOfAccuracy.eq.4 )then
                    if( nd.eq.2 )then
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! Do this for now -- finish me - missing a term - use a conservative approx.  ****************************************************
                          ! Kx = (thermalKpc(i1+1,i2,i3)-thermalKpc(i1-1,i2,i3))/(2.*dx(0))
                          ! Ky = (thermalKpc(i1,i2+1,i3)-thermalKpc(i1,i2-1,i3))/(2.*dx(1))
                          ! Compute Kx, Ky, Kz:
                           ! --- For now we only do second-order accurate---
                            ! --- rectangular grid ---
                              Kx = (8.*(thermalKpc(i1+1,i2,i3)-
     & thermalKpc(i1-1,i2,i3))-(thermalKpc(i1+2,i2,i3)-thermalKpc(i1-
     & 2,i2,i3)))*h41(0)
                              Ky = (8.*(thermalKpc(i1,i2+1,i3)-
     & thermalKpc(i1,i2-1,i3))-(thermalKpc(i1,i2+2,i3)-thermalKpc(i1,
     & i2-2,i3)))*h41(1)
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux42r(i1,
     & i2,i3,tc)-uu(i1,i2,i3,vc)*uy42r(i1,i2,i3,tc) +((thermalKpc(i1,
     & i2,i3)*ulaplacian42r(i1,i2,i3,tc)+Kx*ux42r(i1,i2,i3,tc)+Ky*
     & uy42r(i1,i2,i3,tc))/(rhopc(i1,i2,i3)*Cppc(i1,i2,i3))) +
     & adcBoussinesq*delta4 2(tc)
                        if( isAxisymmetric.eq.1 )then
                          ! -- add on axisymmetric corrections ---
                            ! finish me for axisymmetric and variable material properties
                            stop 8568
                          ri=radiusInverse(i1,i2,i3)
                          if( ri.ne.0. )then
                            ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uy42r(i1,i2,i3,tc)*ri )
                          else
                            ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uyy42r(i1,i2,i3,tc) )
                          end if
                        end if
                        end if
                       end do
                       end do
                       end do
                    else
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                            ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)-gravity(2)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! Do this for now -- finish me - missing a term - use a conservative approx.  ****************************************************
                          ! Kx = (thermalKpc(i1+1,i2,i3)-thermalKpc(i1-1,i2,i3))/(2.*dx(0))
                          ! Ky = (thermalKpc(i1,i2+1,i3)-thermalKpc(i1,i2-1,i3))/(2.*dx(1))
                          ! Compute Kx, Ky, Kz:
                           ! --- For now we only do second-order accurate---
                            ! --- rectangular grid ---
                              Kx = (8.*(thermalKpc(i1+1,i2,i3)-
     & thermalKpc(i1-1,i2,i3))-(thermalKpc(i1+2,i2,i3)-thermalKpc(i1-
     & 2,i2,i3)))*h41(0)
                              Ky = (8.*(thermalKpc(i1,i2+1,i3)-
     & thermalKpc(i1,i2-1,i3))-(thermalKpc(i1,i2+2,i3)-thermalKpc(i1,
     & i2-2,i3)))*h41(1)
                                Kz = (8.*(thermalKpc(i1,i2,i3+1)-
     & thermalKpc(i1,i2,i3-1))-(thermalKpc(i1,i2,i3+2)-thermalKpc(i1,
     & i2,i3-2)))*h41(2)
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux43r(i1,
     & i2,i3,tc)-uu(i1,i2,i3,vc)*uy43r(i1,i2,i3,tc)-uu(i1,i2,i3,wc)*
     & uz43r(i1,i2,i3,tc) +((thermalKpc(i1,i2,i3)*ulaplacian43r(i1,i2,
     & i3,tc)+Kx*ux43r(i1,i2,i3,tc)+Ky*uy43r(i1,i2,i3,tc))/(rhopc(i1,
     & i2,i3)*Cppc(i1,i2,i3))) +adcBoussinesq*delta4 3(tc)
                        if( isAxisymmetric.eq.1 )then
                          ! -- add on axisymmetric corrections ---
                            ! finish me for axisymmetric and variable material properties
                            stop 8568
                          ri=radiusInverse(i1,i2,i3)
                          if( ri.ne.0. )then
                            ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uy43r(i1,i2,i3,tc)*ri )
                          else
                            ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uyy43r(i1,i2,i3,tc) )
                          end if
                        end if
                        end if
                       end do
                       end do
                       end do
                    end if
                  else
                   stop 2281
                  end if
                 else if( gridType.eq.curvilinear )then
                  if( orderOfAccuracy.eq.2 )then
                    if( nd.eq.2 )then
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! Do this for now -- finish me - missing a term - use a conservative approx.  ****************************************************
                          ! Kx = (thermalKpc(i1+1,i2,i3)-thermalKpc(i1-1,i2,i3))/(2.*dx(0))
                          ! Ky = (thermalKpc(i1,i2+1,i3)-thermalKpc(i1,i2-1,i3))/(2.*dx(1))
                          ! Compute Kx, Ky, Kz:
                           ! --- For now we only do second-order accurate---
                              ! --- curvilinear grid ---
                                Kr = (thermalKpc(i1+1,i2,i3)-
     & thermalKpc(i1-1,i2,i3))/(2.*dr(0))
                                Ks = (thermalKpc(i1,i2+1,i3)-
     & thermalKpc(i1,i2-1,i3))/(2.*dr(1))
                                Kx = rx(i1,i2,i3)*Kr+sx(i1,i2,i3)*Ks
                                Ky = ry(i1,i2,i3)*Kr+sy(i1,i2,i3)*Ks
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux22(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy22(i1,i2,i3,tc) +((thermalKpc(i1,i2,
     & i3)*ulaplacian22(i1,i2,i3,tc)+Kx*ux22(i1,i2,i3,tc)+Ky*uy22(i1,
     & i2,i3,tc))/(rhopc(i1,i2,i3)*Cppc(i1,i2,i3))) +adcBoussinesq*
     & delta2 2(tc)
                        if( isAxisymmetric.eq.1 )then
                          ! -- add on axisymmetric corrections ---
                            ! finish me for axisymmetric and variable material properties
                            stop 8568
                          ri=radiusInverse(i1,i2,i3)
                          if( ri.ne.0. )then
                            ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uy22(i1,i2,i3,tc)*ri )
                          else
                            ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uyy22(i1,i2,i3,tc) )
                          end if
                        end if
                        end if
                       end do
                       end do
                       end do
                    else
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                            ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)-gravity(2)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! Do this for now -- finish me - missing a term - use a conservative approx.  ****************************************************
                          ! Kx = (thermalKpc(i1+1,i2,i3)-thermalKpc(i1-1,i2,i3))/(2.*dx(0))
                          ! Ky = (thermalKpc(i1,i2+1,i3)-thermalKpc(i1,i2-1,i3))/(2.*dx(1))
                          ! Compute Kx, Ky, Kz:
                           ! --- For now we only do second-order accurate---
                              ! --- curvilinear grid ---
                                Kr = (thermalKpc(i1+1,i2,i3)-
     & thermalKpc(i1-1,i2,i3))/(2.*dr(0))
                                Ks = (thermalKpc(i1,i2+1,i3)-
     & thermalKpc(i1,i2-1,i3))/(2.*dr(1))
                                  Kt = (thermalKpc(i1,i2,i3+1)-
     & thermalKpc(i1,i2,i3-1))/(2.*dr(2))
                                Kx = rx(i1,i2,i3)*Kr+sx(i1,i2,i3)*Ks+
     & tx(i1,i2,i3)*Kt
                                Ky = ry(i1,i2,i3)*Kr+sy(i1,i2,i3)*Ks+
     & ty(i1,i2,i3)*Kt
                                Kz = rz(i1,i2,i3)*Kr+sz(i1,i2,i3)*Ks+
     & tz(i1,i2,i3)*Kt
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy23(i1,i2,i3,tc)-uu(i1,i2,i3,wc)*uz23(
     & i1,i2,i3,tc) +((thermalKpc(i1,i2,i3)*ulaplacian23(i1,i2,i3,tc)+
     & Kx*ux23(i1,i2,i3,tc)+Ky*uy23(i1,i2,i3,tc))/(rhopc(i1,i2,i3)*
     & Cppc(i1,i2,i3))) +adcBoussinesq*delta2 3(tc)
                        if( isAxisymmetric.eq.1 )then
                          ! -- add on axisymmetric corrections ---
                            ! finish me for axisymmetric and variable material properties
                            stop 8568
                          ri=radiusInverse(i1,i2,i3)
                          if( ri.ne.0. )then
                            ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uy23(i1,i2,i3,tc)*ri )
                          else
                            ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uyy23(i1,i2,i3,tc) )
                          end if
                        end if
                        end if
                       end do
                       end do
                       end do
                    end if
                  else if( orderOfAccuracy.eq.4 )then
                    if( nd.eq.2 )then
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! Do this for now -- finish me - missing a term - use a conservative approx.  ****************************************************
                          ! Kx = (thermalKpc(i1+1,i2,i3)-thermalKpc(i1-1,i2,i3))/(2.*dx(0))
                          ! Ky = (thermalKpc(i1,i2+1,i3)-thermalKpc(i1,i2-1,i3))/(2.*dx(1))
                          ! Compute Kx, Ky, Kz:
                           ! --- For now we only do second-order accurate---
                              ! --- curvilinear grid ---
                                Kr = (8.*(thermalKpc(i1+1,i2,i3)-
     & thermalKpc(i1-1,i2,i3))-(thermalKpc(i1+2,i2,i3)-thermalKpc(i1-
     & 2,i2,i3)))*d14(0)
                                Ks = (8.*(thermalKpc(i1,i2+1,i3)-
     & thermalKpc(i1,i2-1,i3))-(thermalKpc(i1,i2+2,i3)-thermalKpc(i1,
     & i2-2,i3)))*d14(1)
                                Kx = rx(i1,i2,i3)*Kr+sx(i1,i2,i3)*Ks
                                Ky = ry(i1,i2,i3)*Kr+sy(i1,i2,i3)*Ks
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux42(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy42(i1,i2,i3,tc) +((thermalKpc(i1,i2,
     & i3)*ulaplacian42(i1,i2,i3,tc)+Kx*ux42(i1,i2,i3,tc)+Ky*uy42(i1,
     & i2,i3,tc))/(rhopc(i1,i2,i3)*Cppc(i1,i2,i3))) +adcBoussinesq*
     & delta4 2(tc)
                        if( isAxisymmetric.eq.1 )then
                          ! -- add on axisymmetric corrections ---
                            ! finish me for axisymmetric and variable material properties
                            stop 8568
                          ri=radiusInverse(i1,i2,i3)
                          if( ri.ne.0. )then
                            ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uy42(i1,i2,i3,tc)*ri )
                          else
                            ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uyy42(i1,i2,i3,tc) )
                          end if
                        end if
                        end if
                       end do
                       end do
                       end do
                    else
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                            ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)-gravity(2)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! Do this for now -- finish me - missing a term - use a conservative approx.  ****************************************************
                          ! Kx = (thermalKpc(i1+1,i2,i3)-thermalKpc(i1-1,i2,i3))/(2.*dx(0))
                          ! Ky = (thermalKpc(i1,i2+1,i3)-thermalKpc(i1,i2-1,i3))/(2.*dx(1))
                          ! Compute Kx, Ky, Kz:
                           ! --- For now we only do second-order accurate---
                              ! --- curvilinear grid ---
                                Kr = (8.*(thermalKpc(i1+1,i2,i3)-
     & thermalKpc(i1-1,i2,i3))-(thermalKpc(i1+2,i2,i3)-thermalKpc(i1-
     & 2,i2,i3)))*d14(0)
                                Ks = (8.*(thermalKpc(i1,i2+1,i3)-
     & thermalKpc(i1,i2-1,i3))-(thermalKpc(i1,i2+2,i3)-thermalKpc(i1,
     & i2-2,i3)))*d14(1)
                                  Kt = (8.*(thermalKpc(i1,i2,i3+1)-
     & thermalKpc(i1,i2,i3-1))-(thermalKpc(i1,i2,i3+2)-thermalKpc(i1,
     & i2,i3-2)))*d14(2)
                                Kx = rx(i1,i2,i3)*Kr+sx(i1,i2,i3)*Ks+
     & tx(i1,i2,i3)*Kt
                                Ky = ry(i1,i2,i3)*Kr+sy(i1,i2,i3)*Ks+
     & ty(i1,i2,i3)*Kt
                                Kz = rz(i1,i2,i3)*Kr+sz(i1,i2,i3)*Ks+
     & tz(i1,i2,i3)*Kt
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux43(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy43(i1,i2,i3,tc)-uu(i1,i2,i3,wc)*uz43(
     & i1,i2,i3,tc) +((thermalKpc(i1,i2,i3)*ulaplacian43(i1,i2,i3,tc)+
     & Kx*ux43(i1,i2,i3,tc)+Ky*uy43(i1,i2,i3,tc))/(rhopc(i1,i2,i3)*
     & Cppc(i1,i2,i3))) +adcBoussinesq*delta4 3(tc)
                        if( isAxisymmetric.eq.1 )then
                          ! -- add on axisymmetric corrections ---
                            ! finish me for axisymmetric and variable material properties
                            stop 8568
                          ri=radiusInverse(i1,i2,i3)
                          if( ri.ne.0. )then
                            ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uy43(i1,i2,i3,tc)*ri )
                          else
                            ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uyy43(i1,i2,i3,tc) )
                          end if
                        end if
                        end if
                       end do
                       end do
                       end do
                    end if
                  else
                   stop 2282
                  end if
                 else
                   stop 2717
                 end if
             else ! ***** implicit *******
              if( implicitOption .eq.computeImplicitTermsSeparately )
     & then
                 if( gridType.eq.rectangular )then
                  if( orderOfAccuracy.eq.2 )then
                    if( nd.eq.2 )then
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! Do this for now -- finish me - missing a term - use a conservative approx.  ****************************************************
                          ! Kx = (thermalKpc(i1+1,i2,i3)-thermalKpc(i1-1,i2,i3))/(2.*dx(0))
                          ! Ky = (thermalKpc(i1,i2+1,i3)-thermalKpc(i1,i2-1,i3))/(2.*dx(1))
                          ! Compute Kx, Ky, Kz:
                           ! --- For now we only do second-order accurate---
                            ! --- rectangular grid ---
                              Kx = (thermalKpc(i1+1,i2,i3)-thermalKpc(
     & i1-1,i2,i3))/(2.*dx(0))
                              Ky = (thermalKpc(i1,i2+1,i3)-thermalKpc(
     & i1,i2-1,i3))/(2.*dx(1))
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux22r(i1,
     & i2,i3,tc)-uu(i1,i2,i3,vc)*uy22r(i1,i2,i3,tc)+adcBoussinesq*
     & delta2 2(tc)
                       ! include implicit terms - diffusion
                       uti(i1,i2,i3,tc)= ((thermalKpc(i1,i2,i3)*
     & ulaplacian22r(i1,i2,i3,tc)+Kx*ux22r(i1,i2,i3,tc)+Ky*uy22r(i1,
     & i2,i3,tc))/(rhopc(i1,i2,i3)*Cppc(i1,i2,i3)))
                       if( isAxisymmetric.eq.1 )then
                           ! finish me for axisymmetric and variable material properties
                           stop 8569
                         ri=radiusInverse(i1,i2,i3)
                         if( ri.ne.0. )then
                           uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*(
     &  uy22r(i1,i2,i3,tc)*ri )
                         else
                           uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*(
     &  uyy22r(i1,i2,i3,tc) )
                         end if
                       end if
                        end if
                       end do
                       end do
                       end do
                    else
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                            ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)-gravity(2)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! Do this for now -- finish me - missing a term - use a conservative approx.  ****************************************************
                          ! Kx = (thermalKpc(i1+1,i2,i3)-thermalKpc(i1-1,i2,i3))/(2.*dx(0))
                          ! Ky = (thermalKpc(i1,i2+1,i3)-thermalKpc(i1,i2-1,i3))/(2.*dx(1))
                          ! Compute Kx, Ky, Kz:
                           ! --- For now we only do second-order accurate---
                            ! --- rectangular grid ---
                              Kx = (thermalKpc(i1+1,i2,i3)-thermalKpc(
     & i1-1,i2,i3))/(2.*dx(0))
                              Ky = (thermalKpc(i1,i2+1,i3)-thermalKpc(
     & i1,i2-1,i3))/(2.*dx(1))
                                Kz = (thermalKpc(i1,i2,i3+1)-
     & thermalKpc(i1,i2,i3-1))/(2.*dx(2))
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux23r(i1,
     & i2,i3,tc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,tc)-uu(i1,i2,i3,wc)*
     & uz23r(i1,i2,i3,tc)+adcBoussinesq*delta2 3(tc)
                       ! include implicit terms - diffusion
                       uti(i1,i2,i3,tc)= ((thermalKpc(i1,i2,i3)*
     & ulaplacian23r(i1,i2,i3,tc)+Kx*ux23r(i1,i2,i3,tc)+Ky*uy23r(i1,
     & i2,i3,tc))/(rhopc(i1,i2,i3)*Cppc(i1,i2,i3)))
                       if( isAxisymmetric.eq.1 )then
                           ! finish me for axisymmetric and variable material properties
                           stop 8569
                         ri=radiusInverse(i1,i2,i3)
                         if( ri.ne.0. )then
                           uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*(
     &  uy23r(i1,i2,i3,tc)*ri )
                         else
                           uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*(
     &  uyy23r(i1,i2,i3,tc) )
                         end if
                       end if
                        end if
                       end do
                       end do
                       end do
                    end if
                  else if( orderOfAccuracy.eq.4 )then
                    if( nd.eq.2 )then
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! Do this for now -- finish me - missing a term - use a conservative approx.  ****************************************************
                          ! Kx = (thermalKpc(i1+1,i2,i3)-thermalKpc(i1-1,i2,i3))/(2.*dx(0))
                          ! Ky = (thermalKpc(i1,i2+1,i3)-thermalKpc(i1,i2-1,i3))/(2.*dx(1))
                          ! Compute Kx, Ky, Kz:
                           ! --- For now we only do second-order accurate---
                            ! --- rectangular grid ---
                              Kx = (8.*(thermalKpc(i1+1,i2,i3)-
     & thermalKpc(i1-1,i2,i3))-(thermalKpc(i1+2,i2,i3)-thermalKpc(i1-
     & 2,i2,i3)))*h41(0)
                              Ky = (8.*(thermalKpc(i1,i2+1,i3)-
     & thermalKpc(i1,i2-1,i3))-(thermalKpc(i1,i2+2,i3)-thermalKpc(i1,
     & i2-2,i3)))*h41(1)
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux42r(i1,
     & i2,i3,tc)-uu(i1,i2,i3,vc)*uy42r(i1,i2,i3,tc)+adcBoussinesq*
     & delta4 2(tc)
                       ! include implicit terms - diffusion
                       uti(i1,i2,i3,tc)= ((thermalKpc(i1,i2,i3)*
     & ulaplacian42r(i1,i2,i3,tc)+Kx*ux42r(i1,i2,i3,tc)+Ky*uy42r(i1,
     & i2,i3,tc))/(rhopc(i1,i2,i3)*Cppc(i1,i2,i3)))
                       if( isAxisymmetric.eq.1 )then
                           ! finish me for axisymmetric and variable material properties
                           stop 8569
                         ri=radiusInverse(i1,i2,i3)
                         if( ri.ne.0. )then
                           uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*(
     &  uy42r(i1,i2,i3,tc)*ri )
                         else
                           uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*(
     &  uyy42r(i1,i2,i3,tc) )
                         end if
                       end if
                        end if
                       end do
                       end do
                       end do
                    else
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                            ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)-gravity(2)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! Do this for now -- finish me - missing a term - use a conservative approx.  ****************************************************
                          ! Kx = (thermalKpc(i1+1,i2,i3)-thermalKpc(i1-1,i2,i3))/(2.*dx(0))
                          ! Ky = (thermalKpc(i1,i2+1,i3)-thermalKpc(i1,i2-1,i3))/(2.*dx(1))
                          ! Compute Kx, Ky, Kz:
                           ! --- For now we only do second-order accurate---
                            ! --- rectangular grid ---
                              Kx = (8.*(thermalKpc(i1+1,i2,i3)-
     & thermalKpc(i1-1,i2,i3))-(thermalKpc(i1+2,i2,i3)-thermalKpc(i1-
     & 2,i2,i3)))*h41(0)
                              Ky = (8.*(thermalKpc(i1,i2+1,i3)-
     & thermalKpc(i1,i2-1,i3))-(thermalKpc(i1,i2+2,i3)-thermalKpc(i1,
     & i2-2,i3)))*h41(1)
                                Kz = (8.*(thermalKpc(i1,i2,i3+1)-
     & thermalKpc(i1,i2,i3-1))-(thermalKpc(i1,i2,i3+2)-thermalKpc(i1,
     & i2,i3-2)))*h41(2)
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux43r(i1,
     & i2,i3,tc)-uu(i1,i2,i3,vc)*uy43r(i1,i2,i3,tc)-uu(i1,i2,i3,wc)*
     & uz43r(i1,i2,i3,tc)+adcBoussinesq*delta4 3(tc)
                       ! include implicit terms - diffusion
                       uti(i1,i2,i3,tc)= ((thermalKpc(i1,i2,i3)*
     & ulaplacian43r(i1,i2,i3,tc)+Kx*ux43r(i1,i2,i3,tc)+Ky*uy43r(i1,
     & i2,i3,tc))/(rhopc(i1,i2,i3)*Cppc(i1,i2,i3)))
                       if( isAxisymmetric.eq.1 )then
                           ! finish me for axisymmetric and variable material properties
                           stop 8569
                         ri=radiusInverse(i1,i2,i3)
                         if( ri.ne.0. )then
                           uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*(
     &  uy43r(i1,i2,i3,tc)*ri )
                         else
                           uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*(
     &  uyy43r(i1,i2,i3,tc) )
                         end if
                       end if
                        end if
                       end do
                       end do
                       end do
                    end if
                  else
                   stop 2281
                  end if
                 else if( gridType.eq.curvilinear )then
                  if( orderOfAccuracy.eq.2 )then
                    if( nd.eq.2 )then
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! Do this for now -- finish me - missing a term - use a conservative approx.  ****************************************************
                          ! Kx = (thermalKpc(i1+1,i2,i3)-thermalKpc(i1-1,i2,i3))/(2.*dx(0))
                          ! Ky = (thermalKpc(i1,i2+1,i3)-thermalKpc(i1,i2-1,i3))/(2.*dx(1))
                          ! Compute Kx, Ky, Kz:
                           ! --- For now we only do second-order accurate---
                              ! --- curvilinear grid ---
                                Kr = (thermalKpc(i1+1,i2,i3)-
     & thermalKpc(i1-1,i2,i3))/(2.*dr(0))
                                Ks = (thermalKpc(i1,i2+1,i3)-
     & thermalKpc(i1,i2-1,i3))/(2.*dr(1))
                                Kx = rx(i1,i2,i3)*Kr+sx(i1,i2,i3)*Ks
                                Ky = ry(i1,i2,i3)*Kr+sy(i1,i2,i3)*Ks
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux22(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy22(i1,i2,i3,tc)+adcBoussinesq*delta2 
     & 2(tc)
                       ! include implicit terms - diffusion
                       uti(i1,i2,i3,tc)= ((thermalKpc(i1,i2,i3)*
     & ulaplacian22(i1,i2,i3,tc)+Kx*ux22(i1,i2,i3,tc)+Ky*uy22(i1,i2,
     & i3,tc))/(rhopc(i1,i2,i3)*Cppc(i1,i2,i3)))
                       if( isAxisymmetric.eq.1 )then
                           ! finish me for axisymmetric and variable material properties
                           stop 8569
                         ri=radiusInverse(i1,i2,i3)
                         if( ri.ne.0. )then
                           uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*(
     &  uy22(i1,i2,i3,tc)*ri )
                         else
                           uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*(
     &  uyy22(i1,i2,i3,tc) )
                         end if
                       end if
                        end if
                       end do
                       end do
                       end do
                    else
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                            ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)-gravity(2)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! Do this for now -- finish me - missing a term - use a conservative approx.  ****************************************************
                          ! Kx = (thermalKpc(i1+1,i2,i3)-thermalKpc(i1-1,i2,i3))/(2.*dx(0))
                          ! Ky = (thermalKpc(i1,i2+1,i3)-thermalKpc(i1,i2-1,i3))/(2.*dx(1))
                          ! Compute Kx, Ky, Kz:
                           ! --- For now we only do second-order accurate---
                              ! --- curvilinear grid ---
                                Kr = (thermalKpc(i1+1,i2,i3)-
     & thermalKpc(i1-1,i2,i3))/(2.*dr(0))
                                Ks = (thermalKpc(i1,i2+1,i3)-
     & thermalKpc(i1,i2-1,i3))/(2.*dr(1))
                                  Kt = (thermalKpc(i1,i2,i3+1)-
     & thermalKpc(i1,i2,i3-1))/(2.*dr(2))
                                Kx = rx(i1,i2,i3)*Kr+sx(i1,i2,i3)*Ks+
     & tx(i1,i2,i3)*Kt
                                Ky = ry(i1,i2,i3)*Kr+sy(i1,i2,i3)*Ks+
     & ty(i1,i2,i3)*Kt
                                Kz = rz(i1,i2,i3)*Kr+sz(i1,i2,i3)*Ks+
     & tz(i1,i2,i3)*Kt
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy23(i1,i2,i3,tc)-uu(i1,i2,i3,wc)*uz23(
     & i1,i2,i3,tc)+adcBoussinesq*delta2 3(tc)
                       ! include implicit terms - diffusion
                       uti(i1,i2,i3,tc)= ((thermalKpc(i1,i2,i3)*
     & ulaplacian23(i1,i2,i3,tc)+Kx*ux23(i1,i2,i3,tc)+Ky*uy23(i1,i2,
     & i3,tc))/(rhopc(i1,i2,i3)*Cppc(i1,i2,i3)))
                       if( isAxisymmetric.eq.1 )then
                           ! finish me for axisymmetric and variable material properties
                           stop 8569
                         ri=radiusInverse(i1,i2,i3)
                         if( ri.ne.0. )then
                           uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*(
     &  uy23(i1,i2,i3,tc)*ri )
                         else
                           uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*(
     &  uyy23(i1,i2,i3,tc) )
                         end if
                       end if
                        end if
                       end do
                       end do
                       end do
                    end if
                  else if( orderOfAccuracy.eq.4 )then
                    if( nd.eq.2 )then
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! Do this for now -- finish me - missing a term - use a conservative approx.  ****************************************************
                          ! Kx = (thermalKpc(i1+1,i2,i3)-thermalKpc(i1-1,i2,i3))/(2.*dx(0))
                          ! Ky = (thermalKpc(i1,i2+1,i3)-thermalKpc(i1,i2-1,i3))/(2.*dx(1))
                          ! Compute Kx, Ky, Kz:
                           ! --- For now we only do second-order accurate---
                              ! --- curvilinear grid ---
                                Kr = (8.*(thermalKpc(i1+1,i2,i3)-
     & thermalKpc(i1-1,i2,i3))-(thermalKpc(i1+2,i2,i3)-thermalKpc(i1-
     & 2,i2,i3)))*d14(0)
                                Ks = (8.*(thermalKpc(i1,i2+1,i3)-
     & thermalKpc(i1,i2-1,i3))-(thermalKpc(i1,i2+2,i3)-thermalKpc(i1,
     & i2-2,i3)))*d14(1)
                                Kx = rx(i1,i2,i3)*Kr+sx(i1,i2,i3)*Ks
                                Ky = ry(i1,i2,i3)*Kr+sy(i1,i2,i3)*Ks
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux42(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy42(i1,i2,i3,tc)+adcBoussinesq*delta4 
     & 2(tc)
                       ! include implicit terms - diffusion
                       uti(i1,i2,i3,tc)= ((thermalKpc(i1,i2,i3)*
     & ulaplacian42(i1,i2,i3,tc)+Kx*ux42(i1,i2,i3,tc)+Ky*uy42(i1,i2,
     & i3,tc))/(rhopc(i1,i2,i3)*Cppc(i1,i2,i3)))
                       if( isAxisymmetric.eq.1 )then
                           ! finish me for axisymmetric and variable material properties
                           stop 8569
                         ri=radiusInverse(i1,i2,i3)
                         if( ri.ne.0. )then
                           uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*(
     &  uy42(i1,i2,i3,tc)*ri )
                         else
                           uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*(
     &  uyy42(i1,i2,i3,tc) )
                         end if
                       end if
                        end if
                       end do
                       end do
                       end do
                    else
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                            ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)-gravity(2)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! Do this for now -- finish me - missing a term - use a conservative approx.  ****************************************************
                          ! Kx = (thermalKpc(i1+1,i2,i3)-thermalKpc(i1-1,i2,i3))/(2.*dx(0))
                          ! Ky = (thermalKpc(i1,i2+1,i3)-thermalKpc(i1,i2-1,i3))/(2.*dx(1))
                          ! Compute Kx, Ky, Kz:
                           ! --- For now we only do second-order accurate---
                              ! --- curvilinear grid ---
                                Kr = (8.*(thermalKpc(i1+1,i2,i3)-
     & thermalKpc(i1-1,i2,i3))-(thermalKpc(i1+2,i2,i3)-thermalKpc(i1-
     & 2,i2,i3)))*d14(0)
                                Ks = (8.*(thermalKpc(i1,i2+1,i3)-
     & thermalKpc(i1,i2-1,i3))-(thermalKpc(i1,i2+2,i3)-thermalKpc(i1,
     & i2-2,i3)))*d14(1)
                                  Kt = (8.*(thermalKpc(i1,i2,i3+1)-
     & thermalKpc(i1,i2,i3-1))-(thermalKpc(i1,i2,i3+2)-thermalKpc(i1,
     & i2,i3-2)))*d14(2)
                                Kx = rx(i1,i2,i3)*Kr+sx(i1,i2,i3)*Ks+
     & tx(i1,i2,i3)*Kt
                                Ky = ry(i1,i2,i3)*Kr+sy(i1,i2,i3)*Ks+
     & ty(i1,i2,i3)*Kt
                                Kz = rz(i1,i2,i3)*Kr+sz(i1,i2,i3)*Ks+
     & tz(i1,i2,i3)*Kt
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux43(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy43(i1,i2,i3,tc)-uu(i1,i2,i3,wc)*uz43(
     & i1,i2,i3,tc)+adcBoussinesq*delta4 3(tc)
                       ! include implicit terms - diffusion
                       uti(i1,i2,i3,tc)= ((thermalKpc(i1,i2,i3)*
     & ulaplacian43(i1,i2,i3,tc)+Kx*ux43(i1,i2,i3,tc)+Ky*uy43(i1,i2,
     & i3,tc))/(rhopc(i1,i2,i3)*Cppc(i1,i2,i3)))
                       if( isAxisymmetric.eq.1 )then
                           ! finish me for axisymmetric and variable material properties
                           stop 8569
                         ri=radiusInverse(i1,i2,i3)
                         if( ri.ne.0. )then
                           uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*(
     &  uy43(i1,i2,i3,tc)*ri )
                         else
                           uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*(
     &  uyy43(i1,i2,i3,tc) )
                         end if
                       end if
                        end if
                       end do
                       end do
                       end do
                    end if
                  else
                   stop 2282
                  end if
                 else
                   stop 2717
                 end if
              else if( implicitOption.eq.doNotComputeImplicitTerms )
     & then
                 if( gridType.eq.rectangular )then
                  if( orderOfAccuracy.eq.2 )then
                    if( nd.eq.2 )then
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! Do this for now -- finish me - missing a term - use a conservative approx.  ****************************************************
                          ! Kx = (thermalKpc(i1+1,i2,i3)-thermalKpc(i1-1,i2,i3))/(2.*dx(0))
                          ! Ky = (thermalKpc(i1,i2+1,i3)-thermalKpc(i1,i2-1,i3))/(2.*dx(1))
                          ! Compute Kx, Ky, Kz:
                           ! --- For now we only do second-order accurate---
                            ! --- rectangular grid ---
                              Kx = (thermalKpc(i1+1,i2,i3)-thermalKpc(
     & i1-1,i2,i3))/(2.*dx(0))
                              Ky = (thermalKpc(i1,i2+1,i3)-thermalKpc(
     & i1,i2-1,i3))/(2.*dx(1))
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux22r(i1,
     & i2,i3,tc)-uu(i1,i2,i3,vc)*uy22r(i1,i2,i3,tc)+adcBoussinesq*
     & delta2 2(tc)
                        end if
                       end do
                       end do
                       end do
                    else
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                            ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)-gravity(2)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! Do this for now -- finish me - missing a term - use a conservative approx.  ****************************************************
                          ! Kx = (thermalKpc(i1+1,i2,i3)-thermalKpc(i1-1,i2,i3))/(2.*dx(0))
                          ! Ky = (thermalKpc(i1,i2+1,i3)-thermalKpc(i1,i2-1,i3))/(2.*dx(1))
                          ! Compute Kx, Ky, Kz:
                           ! --- For now we only do second-order accurate---
                            ! --- rectangular grid ---
                              Kx = (thermalKpc(i1+1,i2,i3)-thermalKpc(
     & i1-1,i2,i3))/(2.*dx(0))
                              Ky = (thermalKpc(i1,i2+1,i3)-thermalKpc(
     & i1,i2-1,i3))/(2.*dx(1))
                                Kz = (thermalKpc(i1,i2,i3+1)-
     & thermalKpc(i1,i2,i3-1))/(2.*dx(2))
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux23r(i1,
     & i2,i3,tc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,tc)-uu(i1,i2,i3,wc)*
     & uz23r(i1,i2,i3,tc)+adcBoussinesq*delta2 3(tc)
                        end if
                       end do
                       end do
                       end do
                    end if
                  else if( orderOfAccuracy.eq.4 )then
                    if( nd.eq.2 )then
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! Do this for now -- finish me - missing a term - use a conservative approx.  ****************************************************
                          ! Kx = (thermalKpc(i1+1,i2,i3)-thermalKpc(i1-1,i2,i3))/(2.*dx(0))
                          ! Ky = (thermalKpc(i1,i2+1,i3)-thermalKpc(i1,i2-1,i3))/(2.*dx(1))
                          ! Compute Kx, Ky, Kz:
                           ! --- For now we only do second-order accurate---
                            ! --- rectangular grid ---
                              Kx = (8.*(thermalKpc(i1+1,i2,i3)-
     & thermalKpc(i1-1,i2,i3))-(thermalKpc(i1+2,i2,i3)-thermalKpc(i1-
     & 2,i2,i3)))*h41(0)
                              Ky = (8.*(thermalKpc(i1,i2+1,i3)-
     & thermalKpc(i1,i2-1,i3))-(thermalKpc(i1,i2+2,i3)-thermalKpc(i1,
     & i2-2,i3)))*h41(1)
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux42r(i1,
     & i2,i3,tc)-uu(i1,i2,i3,vc)*uy42r(i1,i2,i3,tc)+adcBoussinesq*
     & delta4 2(tc)
                        end if
                       end do
                       end do
                       end do
                    else
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                            ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)-gravity(2)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! Do this for now -- finish me - missing a term - use a conservative approx.  ****************************************************
                          ! Kx = (thermalKpc(i1+1,i2,i3)-thermalKpc(i1-1,i2,i3))/(2.*dx(0))
                          ! Ky = (thermalKpc(i1,i2+1,i3)-thermalKpc(i1,i2-1,i3))/(2.*dx(1))
                          ! Compute Kx, Ky, Kz:
                           ! --- For now we only do second-order accurate---
                            ! --- rectangular grid ---
                              Kx = (8.*(thermalKpc(i1+1,i2,i3)-
     & thermalKpc(i1-1,i2,i3))-(thermalKpc(i1+2,i2,i3)-thermalKpc(i1-
     & 2,i2,i3)))*h41(0)
                              Ky = (8.*(thermalKpc(i1,i2+1,i3)-
     & thermalKpc(i1,i2-1,i3))-(thermalKpc(i1,i2+2,i3)-thermalKpc(i1,
     & i2-2,i3)))*h41(1)
                                Kz = (8.*(thermalKpc(i1,i2,i3+1)-
     & thermalKpc(i1,i2,i3-1))-(thermalKpc(i1,i2,i3+2)-thermalKpc(i1,
     & i2,i3-2)))*h41(2)
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux43r(i1,
     & i2,i3,tc)-uu(i1,i2,i3,vc)*uy43r(i1,i2,i3,tc)-uu(i1,i2,i3,wc)*
     & uz43r(i1,i2,i3,tc)+adcBoussinesq*delta4 3(tc)
                        end if
                       end do
                       end do
                       end do
                    end if
                  else
                   stop 2281
                  end if
                 else if( gridType.eq.curvilinear )then
                  if( orderOfAccuracy.eq.2 )then
                    if( nd.eq.2 )then
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! Do this for now -- finish me - missing a term - use a conservative approx.  ****************************************************
                          ! Kx = (thermalKpc(i1+1,i2,i3)-thermalKpc(i1-1,i2,i3))/(2.*dx(0))
                          ! Ky = (thermalKpc(i1,i2+1,i3)-thermalKpc(i1,i2-1,i3))/(2.*dx(1))
                          ! Compute Kx, Ky, Kz:
                           ! --- For now we only do second-order accurate---
                              ! --- curvilinear grid ---
                                Kr = (thermalKpc(i1+1,i2,i3)-
     & thermalKpc(i1-1,i2,i3))/(2.*dr(0))
                                Ks = (thermalKpc(i1,i2+1,i3)-
     & thermalKpc(i1,i2-1,i3))/(2.*dr(1))
                                Kx = rx(i1,i2,i3)*Kr+sx(i1,i2,i3)*Ks
                                Ky = ry(i1,i2,i3)*Kr+sy(i1,i2,i3)*Ks
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux22(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy22(i1,i2,i3,tc)+adcBoussinesq*delta2 
     & 2(tc)
                        end if
                       end do
                       end do
                       end do
                    else
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                            ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)-gravity(2)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! Do this for now -- finish me - missing a term - use a conservative approx.  ****************************************************
                          ! Kx = (thermalKpc(i1+1,i2,i3)-thermalKpc(i1-1,i2,i3))/(2.*dx(0))
                          ! Ky = (thermalKpc(i1,i2+1,i3)-thermalKpc(i1,i2-1,i3))/(2.*dx(1))
                          ! Compute Kx, Ky, Kz:
                           ! --- For now we only do second-order accurate---
                              ! --- curvilinear grid ---
                                Kr = (thermalKpc(i1+1,i2,i3)-
     & thermalKpc(i1-1,i2,i3))/(2.*dr(0))
                                Ks = (thermalKpc(i1,i2+1,i3)-
     & thermalKpc(i1,i2-1,i3))/(2.*dr(1))
                                  Kt = (thermalKpc(i1,i2,i3+1)-
     & thermalKpc(i1,i2,i3-1))/(2.*dr(2))
                                Kx = rx(i1,i2,i3)*Kr+sx(i1,i2,i3)*Ks+
     & tx(i1,i2,i3)*Kt
                                Ky = ry(i1,i2,i3)*Kr+sy(i1,i2,i3)*Ks+
     & ty(i1,i2,i3)*Kt
                                Kz = rz(i1,i2,i3)*Kr+sz(i1,i2,i3)*Ks+
     & tz(i1,i2,i3)*Kt
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy23(i1,i2,i3,tc)-uu(i1,i2,i3,wc)*uz23(
     & i1,i2,i3,tc)+adcBoussinesq*delta2 3(tc)
                        end if
                       end do
                       end do
                       end do
                    end if
                  else if( orderOfAccuracy.eq.4 )then
                    if( nd.eq.2 )then
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! Do this for now -- finish me - missing a term - use a conservative approx.  ****************************************************
                          ! Kx = (thermalKpc(i1+1,i2,i3)-thermalKpc(i1-1,i2,i3))/(2.*dx(0))
                          ! Ky = (thermalKpc(i1,i2+1,i3)-thermalKpc(i1,i2-1,i3))/(2.*dx(1))
                          ! Compute Kx, Ky, Kz:
                           ! --- For now we only do second-order accurate---
                              ! --- curvilinear grid ---
                                Kr = (8.*(thermalKpc(i1+1,i2,i3)-
     & thermalKpc(i1-1,i2,i3))-(thermalKpc(i1+2,i2,i3)-thermalKpc(i1-
     & 2,i2,i3)))*d14(0)
                                Ks = (8.*(thermalKpc(i1,i2+1,i3)-
     & thermalKpc(i1,i2-1,i3))-(thermalKpc(i1,i2+2,i3)-thermalKpc(i1,
     & i2-2,i3)))*d14(1)
                                Kx = rx(i1,i2,i3)*Kr+sx(i1,i2,i3)*Ks
                                Ky = ry(i1,i2,i3)*Kr+sy(i1,i2,i3)*Ks
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux42(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy42(i1,i2,i3,tc)+adcBoussinesq*delta4 
     & 2(tc)
                        end if
                       end do
                       end do
                       end do
                    else
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                            ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)-gravity(2)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! Do this for now -- finish me - missing a term - use a conservative approx.  ****************************************************
                          ! Kx = (thermalKpc(i1+1,i2,i3)-thermalKpc(i1-1,i2,i3))/(2.*dx(0))
                          ! Ky = (thermalKpc(i1,i2+1,i3)-thermalKpc(i1,i2-1,i3))/(2.*dx(1))
                          ! Compute Kx, Ky, Kz:
                           ! --- For now we only do second-order accurate---
                              ! --- curvilinear grid ---
                                Kr = (8.*(thermalKpc(i1+1,i2,i3)-
     & thermalKpc(i1-1,i2,i3))-(thermalKpc(i1+2,i2,i3)-thermalKpc(i1-
     & 2,i2,i3)))*d14(0)
                                Ks = (8.*(thermalKpc(i1,i2+1,i3)-
     & thermalKpc(i1,i2-1,i3))-(thermalKpc(i1,i2+2,i3)-thermalKpc(i1,
     & i2-2,i3)))*d14(1)
                                  Kt = (8.*(thermalKpc(i1,i2,i3+1)-
     & thermalKpc(i1,i2,i3-1))-(thermalKpc(i1,i2,i3+2)-thermalKpc(i1,
     & i2,i3-2)))*d14(2)
                                Kx = rx(i1,i2,i3)*Kr+sx(i1,i2,i3)*Ks+
     & tx(i1,i2,i3)*Kt
                                Ky = ry(i1,i2,i3)*Kr+sy(i1,i2,i3)*Ks+
     & ty(i1,i2,i3)*Kt
                                Kz = rz(i1,i2,i3)*Kr+sz(i1,i2,i3)*Ks+
     & tz(i1,i2,i3)*Kt
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux43(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy43(i1,i2,i3,tc)-uu(i1,i2,i3,wc)*uz43(
     & i1,i2,i3,tc)+adcBoussinesq*delta4 3(tc)
                        end if
                       end do
                       end do
                       end do
                    end if
                  else
                   stop 2282
                  end if
                 else
                   stop 2717
                 end if
              else
               write(*,*)'insdt:boussinesq Unknown implicitOption=',
     & implicitOption
               stop 4135
              end if  ! end implicitOption
             end if
          else if( materialFormat.eq.variableMaterialProperties )then
            ! variable material property 
            ! write(*,'(" insdt: variable material property Heat Eqn...")')
             if( gridIsImplicit.eq.0 )then ! explicit
                 if( gridType.eq.rectangular )then
                  if( orderOfAccuracy.eq.2 )then
                    if( nd.eq.2 )then
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! Kx = (thermalKv(i1+1,i2,i3)-thermalKv(i1-1,i2,i3))/(2.*dx(0))
                          ! Ky = (thermalKv(i1,i2+1,i3)-thermalKv(i1,i2-1,i3))/(2.*dx(1))
                          ! Compute Kx, Ky, Kz:
                           ! --- For now we only do second-order accurate---
                            ! --- rectangular grid ---
                              Kx = (thermalKv(i1+1,i2,i3)-thermalKv(i1-
     & 1,i2,i3))/(2.*dx(0))
                              Ky = (thermalKv(i1,i2+1,i3)-thermalKv(i1,
     & i2-1,i3))/(2.*dx(1))
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux22r(i1,
     & i2,i3,tc)-uu(i1,i2,i3,vc)*uy22r(i1,i2,i3,tc) +((thermalKv(i1,
     & i2,i3)*ulaplacian22r(i1,i2,i3,tc)+Kx*ux22r(i1,i2,i3,tc)+Ky*
     & uy22r(i1,i2,i3,tc))/(rhov(i1,i2,i3)*Cpv(i1,i2,i3))) +
     & adcBoussinesq*delta2 2(tc)
                        if( isAxisymmetric.eq.1 )then
                          ! -- add on axisymmetric corrections ---
                            ! finish me for axisymmetric and variable material properties
                            stop 8568
                          ri=radiusInverse(i1,i2,i3)
                          if( ri.ne.0. )then
                            ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uy22r(i1,i2,i3,tc)*ri )
                          else
                            ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uyy22r(i1,i2,i3,tc) )
                          end if
                        end if
                        end if
                       end do
                       end do
                       end do
                    else
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                            ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)-gravity(2)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! Kx = (thermalKv(i1+1,i2,i3)-thermalKv(i1-1,i2,i3))/(2.*dx(0))
                          ! Ky = (thermalKv(i1,i2+1,i3)-thermalKv(i1,i2-1,i3))/(2.*dx(1))
                          ! Compute Kx, Ky, Kz:
                           ! --- For now we only do second-order accurate---
                            ! --- rectangular grid ---
                              Kx = (thermalKv(i1+1,i2,i3)-thermalKv(i1-
     & 1,i2,i3))/(2.*dx(0))
                              Ky = (thermalKv(i1,i2+1,i3)-thermalKv(i1,
     & i2-1,i3))/(2.*dx(1))
                                Kz = (thermalKv(i1,i2,i3+1)-thermalKv(
     & i1,i2,i3-1))/(2.*dx(2))
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux23r(i1,
     & i2,i3,tc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,tc)-uu(i1,i2,i3,wc)*
     & uz23r(i1,i2,i3,tc) +((thermalKv(i1,i2,i3)*ulaplacian23r(i1,i2,
     & i3,tc)+Kx*ux23r(i1,i2,i3,tc)+Ky*uy23r(i1,i2,i3,tc))/(rhov(i1,
     & i2,i3)*Cpv(i1,i2,i3))) +adcBoussinesq*delta2 3(tc)
                        if( isAxisymmetric.eq.1 )then
                          ! -- add on axisymmetric corrections ---
                            ! finish me for axisymmetric and variable material properties
                            stop 8568
                          ri=radiusInverse(i1,i2,i3)
                          if( ri.ne.0. )then
                            ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uy23r(i1,i2,i3,tc)*ri )
                          else
                            ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uyy23r(i1,i2,i3,tc) )
                          end if
                        end if
                        end if
                       end do
                       end do
                       end do
                    end if
                  else if( orderOfAccuracy.eq.4 )then
                    if( nd.eq.2 )then
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! Kx = (thermalKv(i1+1,i2,i3)-thermalKv(i1-1,i2,i3))/(2.*dx(0))
                          ! Ky = (thermalKv(i1,i2+1,i3)-thermalKv(i1,i2-1,i3))/(2.*dx(1))
                          ! Compute Kx, Ky, Kz:
                           ! --- For now we only do second-order accurate---
                            ! --- rectangular grid ---
                              Kx = (8.*(thermalKv(i1+1,i2,i3)-
     & thermalKv(i1-1,i2,i3))-(thermalKv(i1+2,i2,i3)-thermalKv(i1-2,
     & i2,i3)))*h41(0)
                              Ky = (8.*(thermalKv(i1,i2+1,i3)-
     & thermalKv(i1,i2-1,i3))-(thermalKv(i1,i2+2,i3)-thermalKv(i1,i2-
     & 2,i3)))*h41(1)
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux42r(i1,
     & i2,i3,tc)-uu(i1,i2,i3,vc)*uy42r(i1,i2,i3,tc) +((thermalKv(i1,
     & i2,i3)*ulaplacian42r(i1,i2,i3,tc)+Kx*ux42r(i1,i2,i3,tc)+Ky*
     & uy42r(i1,i2,i3,tc))/(rhov(i1,i2,i3)*Cpv(i1,i2,i3))) +
     & adcBoussinesq*delta4 2(tc)
                        if( isAxisymmetric.eq.1 )then
                          ! -- add on axisymmetric corrections ---
                            ! finish me for axisymmetric and variable material properties
                            stop 8568
                          ri=radiusInverse(i1,i2,i3)
                          if( ri.ne.0. )then
                            ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uy42r(i1,i2,i3,tc)*ri )
                          else
                            ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uyy42r(i1,i2,i3,tc) )
                          end if
                        end if
                        end if
                       end do
                       end do
                       end do
                    else
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                            ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)-gravity(2)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! Kx = (thermalKv(i1+1,i2,i3)-thermalKv(i1-1,i2,i3))/(2.*dx(0))
                          ! Ky = (thermalKv(i1,i2+1,i3)-thermalKv(i1,i2-1,i3))/(2.*dx(1))
                          ! Compute Kx, Ky, Kz:
                           ! --- For now we only do second-order accurate---
                            ! --- rectangular grid ---
                              Kx = (8.*(thermalKv(i1+1,i2,i3)-
     & thermalKv(i1-1,i2,i3))-(thermalKv(i1+2,i2,i3)-thermalKv(i1-2,
     & i2,i3)))*h41(0)
                              Ky = (8.*(thermalKv(i1,i2+1,i3)-
     & thermalKv(i1,i2-1,i3))-(thermalKv(i1,i2+2,i3)-thermalKv(i1,i2-
     & 2,i3)))*h41(1)
                                Kz = (8.*(thermalKv(i1,i2,i3+1)-
     & thermalKv(i1,i2,i3-1))-(thermalKv(i1,i2,i3+2)-thermalKv(i1,i2,
     & i3-2)))*h41(2)
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux43r(i1,
     & i2,i3,tc)-uu(i1,i2,i3,vc)*uy43r(i1,i2,i3,tc)-uu(i1,i2,i3,wc)*
     & uz43r(i1,i2,i3,tc) +((thermalKv(i1,i2,i3)*ulaplacian43r(i1,i2,
     & i3,tc)+Kx*ux43r(i1,i2,i3,tc)+Ky*uy43r(i1,i2,i3,tc))/(rhov(i1,
     & i2,i3)*Cpv(i1,i2,i3))) +adcBoussinesq*delta4 3(tc)
                        if( isAxisymmetric.eq.1 )then
                          ! -- add on axisymmetric corrections ---
                            ! finish me for axisymmetric and variable material properties
                            stop 8568
                          ri=radiusInverse(i1,i2,i3)
                          if( ri.ne.0. )then
                            ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uy43r(i1,i2,i3,tc)*ri )
                          else
                            ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uyy43r(i1,i2,i3,tc) )
                          end if
                        end if
                        end if
                       end do
                       end do
                       end do
                    end if
                  else
                   stop 2281
                  end if
                 else if( gridType.eq.curvilinear )then
                  if( orderOfAccuracy.eq.2 )then
                    if( nd.eq.2 )then
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! Kx = (thermalKv(i1+1,i2,i3)-thermalKv(i1-1,i2,i3))/(2.*dx(0))
                          ! Ky = (thermalKv(i1,i2+1,i3)-thermalKv(i1,i2-1,i3))/(2.*dx(1))
                          ! Compute Kx, Ky, Kz:
                           ! --- For now we only do second-order accurate---
                              ! --- curvilinear grid ---
                                Kr = (thermalKv(i1+1,i2,i3)-thermalKv(
     & i1-1,i2,i3))/(2.*dr(0))
                                Ks = (thermalKv(i1,i2+1,i3)-thermalKv(
     & i1,i2-1,i3))/(2.*dr(1))
                                Kx = rx(i1,i2,i3)*Kr+sx(i1,i2,i3)*Ks
                                Ky = ry(i1,i2,i3)*Kr+sy(i1,i2,i3)*Ks
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux22(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy22(i1,i2,i3,tc) +((thermalKv(i1,i2,i3)
     & *ulaplacian22(i1,i2,i3,tc)+Kx*ux22(i1,i2,i3,tc)+Ky*uy22(i1,i2,
     & i3,tc))/(rhov(i1,i2,i3)*Cpv(i1,i2,i3))) +adcBoussinesq*delta2 
     & 2(tc)
                        if( isAxisymmetric.eq.1 )then
                          ! -- add on axisymmetric corrections ---
                            ! finish me for axisymmetric and variable material properties
                            stop 8568
                          ri=radiusInverse(i1,i2,i3)
                          if( ri.ne.0. )then
                            ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uy22(i1,i2,i3,tc)*ri )
                          else
                            ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uyy22(i1,i2,i3,tc) )
                          end if
                        end if
                        end if
                       end do
                       end do
                       end do
                    else
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                            ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)-gravity(2)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! Kx = (thermalKv(i1+1,i2,i3)-thermalKv(i1-1,i2,i3))/(2.*dx(0))
                          ! Ky = (thermalKv(i1,i2+1,i3)-thermalKv(i1,i2-1,i3))/(2.*dx(1))
                          ! Compute Kx, Ky, Kz:
                           ! --- For now we only do second-order accurate---
                              ! --- curvilinear grid ---
                                Kr = (thermalKv(i1+1,i2,i3)-thermalKv(
     & i1-1,i2,i3))/(2.*dr(0))
                                Ks = (thermalKv(i1,i2+1,i3)-thermalKv(
     & i1,i2-1,i3))/(2.*dr(1))
                                  Kt = (thermalKv(i1,i2,i3+1)-
     & thermalKv(i1,i2,i3-1))/(2.*dr(2))
                                Kx = rx(i1,i2,i3)*Kr+sx(i1,i2,i3)*Ks+
     & tx(i1,i2,i3)*Kt
                                Ky = ry(i1,i2,i3)*Kr+sy(i1,i2,i3)*Ks+
     & ty(i1,i2,i3)*Kt
                                Kz = rz(i1,i2,i3)*Kr+sz(i1,i2,i3)*Ks+
     & tz(i1,i2,i3)*Kt
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy23(i1,i2,i3,tc)-uu(i1,i2,i3,wc)*uz23(
     & i1,i2,i3,tc) +((thermalKv(i1,i2,i3)*ulaplacian23(i1,i2,i3,tc)+
     & Kx*ux23(i1,i2,i3,tc)+Ky*uy23(i1,i2,i3,tc))/(rhov(i1,i2,i3)*Cpv(
     & i1,i2,i3))) +adcBoussinesq*delta2 3(tc)
                        if( isAxisymmetric.eq.1 )then
                          ! -- add on axisymmetric corrections ---
                            ! finish me for axisymmetric and variable material properties
                            stop 8568
                          ri=radiusInverse(i1,i2,i3)
                          if( ri.ne.0. )then
                            ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uy23(i1,i2,i3,tc)*ri )
                          else
                            ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uyy23(i1,i2,i3,tc) )
                          end if
                        end if
                        end if
                       end do
                       end do
                       end do
                    end if
                  else if( orderOfAccuracy.eq.4 )then
                    if( nd.eq.2 )then
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! Kx = (thermalKv(i1+1,i2,i3)-thermalKv(i1-1,i2,i3))/(2.*dx(0))
                          ! Ky = (thermalKv(i1,i2+1,i3)-thermalKv(i1,i2-1,i3))/(2.*dx(1))
                          ! Compute Kx, Ky, Kz:
                           ! --- For now we only do second-order accurate---
                              ! --- curvilinear grid ---
                                Kr = (8.*(thermalKv(i1+1,i2,i3)-
     & thermalKv(i1-1,i2,i3))-(thermalKv(i1+2,i2,i3)-thermalKv(i1-2,
     & i2,i3)))*d14(0)
                                Ks = (8.*(thermalKv(i1,i2+1,i3)-
     & thermalKv(i1,i2-1,i3))-(thermalKv(i1,i2+2,i3)-thermalKv(i1,i2-
     & 2,i3)))*d14(1)
                                Kx = rx(i1,i2,i3)*Kr+sx(i1,i2,i3)*Ks
                                Ky = ry(i1,i2,i3)*Kr+sy(i1,i2,i3)*Ks
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux42(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy42(i1,i2,i3,tc) +((thermalKv(i1,i2,i3)
     & *ulaplacian42(i1,i2,i3,tc)+Kx*ux42(i1,i2,i3,tc)+Ky*uy42(i1,i2,
     & i3,tc))/(rhov(i1,i2,i3)*Cpv(i1,i2,i3))) +adcBoussinesq*delta4 
     & 2(tc)
                        if( isAxisymmetric.eq.1 )then
                          ! -- add on axisymmetric corrections ---
                            ! finish me for axisymmetric and variable material properties
                            stop 8568
                          ri=radiusInverse(i1,i2,i3)
                          if( ri.ne.0. )then
                            ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uy42(i1,i2,i3,tc)*ri )
                          else
                            ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uyy42(i1,i2,i3,tc) )
                          end if
                        end if
                        end if
                       end do
                       end do
                       end do
                    else
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                            ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)-gravity(2)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! Kx = (thermalKv(i1+1,i2,i3)-thermalKv(i1-1,i2,i3))/(2.*dx(0))
                          ! Ky = (thermalKv(i1,i2+1,i3)-thermalKv(i1,i2-1,i3))/(2.*dx(1))
                          ! Compute Kx, Ky, Kz:
                           ! --- For now we only do second-order accurate---
                              ! --- curvilinear grid ---
                                Kr = (8.*(thermalKv(i1+1,i2,i3)-
     & thermalKv(i1-1,i2,i3))-(thermalKv(i1+2,i2,i3)-thermalKv(i1-2,
     & i2,i3)))*d14(0)
                                Ks = (8.*(thermalKv(i1,i2+1,i3)-
     & thermalKv(i1,i2-1,i3))-(thermalKv(i1,i2+2,i3)-thermalKv(i1,i2-
     & 2,i3)))*d14(1)
                                  Kt = (8.*(thermalKv(i1,i2,i3+1)-
     & thermalKv(i1,i2,i3-1))-(thermalKv(i1,i2,i3+2)-thermalKv(i1,i2,
     & i3-2)))*d14(2)
                                Kx = rx(i1,i2,i3)*Kr+sx(i1,i2,i3)*Ks+
     & tx(i1,i2,i3)*Kt
                                Ky = ry(i1,i2,i3)*Kr+sy(i1,i2,i3)*Ks+
     & ty(i1,i2,i3)*Kt
                                Kz = rz(i1,i2,i3)*Kr+sz(i1,i2,i3)*Ks+
     & tz(i1,i2,i3)*Kt
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux43(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy43(i1,i2,i3,tc)-uu(i1,i2,i3,wc)*uz43(
     & i1,i2,i3,tc) +((thermalKv(i1,i2,i3)*ulaplacian43(i1,i2,i3,tc)+
     & Kx*ux43(i1,i2,i3,tc)+Ky*uy43(i1,i2,i3,tc))/(rhov(i1,i2,i3)*Cpv(
     & i1,i2,i3))) +adcBoussinesq*delta4 3(tc)
                        if( isAxisymmetric.eq.1 )then
                          ! -- add on axisymmetric corrections ---
                            ! finish me for axisymmetric and variable material properties
                            stop 8568
                          ri=radiusInverse(i1,i2,i3)
                          if( ri.ne.0. )then
                            ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uy43(i1,i2,i3,tc)*ri )
                          else
                            ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uyy43(i1,i2,i3,tc) )
                          end if
                        end if
                        end if
                       end do
                       end do
                       end do
                    end if
                  else
                   stop 2282
                  end if
                 else
                   stop 2717
                 end if
             else ! ***** implicit *******
              if( implicitOption .eq.computeImplicitTermsSeparately )
     & then
                 if( gridType.eq.rectangular )then
                  if( orderOfAccuracy.eq.2 )then
                    if( nd.eq.2 )then
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! Kx = (thermalKv(i1+1,i2,i3)-thermalKv(i1-1,i2,i3))/(2.*dx(0))
                          ! Ky = (thermalKv(i1,i2+1,i3)-thermalKv(i1,i2-1,i3))/(2.*dx(1))
                          ! Compute Kx, Ky, Kz:
                           ! --- For now we only do second-order accurate---
                            ! --- rectangular grid ---
                              Kx = (thermalKv(i1+1,i2,i3)-thermalKv(i1-
     & 1,i2,i3))/(2.*dx(0))
                              Ky = (thermalKv(i1,i2+1,i3)-thermalKv(i1,
     & i2-1,i3))/(2.*dx(1))
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux22r(i1,
     & i2,i3,tc)-uu(i1,i2,i3,vc)*uy22r(i1,i2,i3,tc)+adcBoussinesq*
     & delta2 2(tc)
                       ! include implicit terms - diffusion
                       uti(i1,i2,i3,tc)= ((thermalKv(i1,i2,i3)*
     & ulaplacian22r(i1,i2,i3,tc)+Kx*ux22r(i1,i2,i3,tc)+Ky*uy22r(i1,
     & i2,i3,tc))/(rhov(i1,i2,i3)*Cpv(i1,i2,i3)))
                       if( isAxisymmetric.eq.1 )then
                           ! finish me for axisymmetric and variable material properties
                           stop 8569
                         ri=radiusInverse(i1,i2,i3)
                         if( ri.ne.0. )then
                           uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*(
     &  uy22r(i1,i2,i3,tc)*ri )
                         else
                           uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*(
     &  uyy22r(i1,i2,i3,tc) )
                         end if
                       end if
                        end if
                       end do
                       end do
                       end do
                    else
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                            ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)-gravity(2)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! Kx = (thermalKv(i1+1,i2,i3)-thermalKv(i1-1,i2,i3))/(2.*dx(0))
                          ! Ky = (thermalKv(i1,i2+1,i3)-thermalKv(i1,i2-1,i3))/(2.*dx(1))
                          ! Compute Kx, Ky, Kz:
                           ! --- For now we only do second-order accurate---
                            ! --- rectangular grid ---
                              Kx = (thermalKv(i1+1,i2,i3)-thermalKv(i1-
     & 1,i2,i3))/(2.*dx(0))
                              Ky = (thermalKv(i1,i2+1,i3)-thermalKv(i1,
     & i2-1,i3))/(2.*dx(1))
                                Kz = (thermalKv(i1,i2,i3+1)-thermalKv(
     & i1,i2,i3-1))/(2.*dx(2))
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux23r(i1,
     & i2,i3,tc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,tc)-uu(i1,i2,i3,wc)*
     & uz23r(i1,i2,i3,tc)+adcBoussinesq*delta2 3(tc)
                       ! include implicit terms - diffusion
                       uti(i1,i2,i3,tc)= ((thermalKv(i1,i2,i3)*
     & ulaplacian23r(i1,i2,i3,tc)+Kx*ux23r(i1,i2,i3,tc)+Ky*uy23r(i1,
     & i2,i3,tc))/(rhov(i1,i2,i3)*Cpv(i1,i2,i3)))
                       if( isAxisymmetric.eq.1 )then
                           ! finish me for axisymmetric and variable material properties
                           stop 8569
                         ri=radiusInverse(i1,i2,i3)
                         if( ri.ne.0. )then
                           uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*(
     &  uy23r(i1,i2,i3,tc)*ri )
                         else
                           uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*(
     &  uyy23r(i1,i2,i3,tc) )
                         end if
                       end if
                        end if
                       end do
                       end do
                       end do
                    end if
                  else if( orderOfAccuracy.eq.4 )then
                    if( nd.eq.2 )then
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! Kx = (thermalKv(i1+1,i2,i3)-thermalKv(i1-1,i2,i3))/(2.*dx(0))
                          ! Ky = (thermalKv(i1,i2+1,i3)-thermalKv(i1,i2-1,i3))/(2.*dx(1))
                          ! Compute Kx, Ky, Kz:
                           ! --- For now we only do second-order accurate---
                            ! --- rectangular grid ---
                              Kx = (8.*(thermalKv(i1+1,i2,i3)-
     & thermalKv(i1-1,i2,i3))-(thermalKv(i1+2,i2,i3)-thermalKv(i1-2,
     & i2,i3)))*h41(0)
                              Ky = (8.*(thermalKv(i1,i2+1,i3)-
     & thermalKv(i1,i2-1,i3))-(thermalKv(i1,i2+2,i3)-thermalKv(i1,i2-
     & 2,i3)))*h41(1)
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux42r(i1,
     & i2,i3,tc)-uu(i1,i2,i3,vc)*uy42r(i1,i2,i3,tc)+adcBoussinesq*
     & delta4 2(tc)
                       ! include implicit terms - diffusion
                       uti(i1,i2,i3,tc)= ((thermalKv(i1,i2,i3)*
     & ulaplacian42r(i1,i2,i3,tc)+Kx*ux42r(i1,i2,i3,tc)+Ky*uy42r(i1,
     & i2,i3,tc))/(rhov(i1,i2,i3)*Cpv(i1,i2,i3)))
                       if( isAxisymmetric.eq.1 )then
                           ! finish me for axisymmetric and variable material properties
                           stop 8569
                         ri=radiusInverse(i1,i2,i3)
                         if( ri.ne.0. )then
                           uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*(
     &  uy42r(i1,i2,i3,tc)*ri )
                         else
                           uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*(
     &  uyy42r(i1,i2,i3,tc) )
                         end if
                       end if
                        end if
                       end do
                       end do
                       end do
                    else
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                            ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)-gravity(2)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! Kx = (thermalKv(i1+1,i2,i3)-thermalKv(i1-1,i2,i3))/(2.*dx(0))
                          ! Ky = (thermalKv(i1,i2+1,i3)-thermalKv(i1,i2-1,i3))/(2.*dx(1))
                          ! Compute Kx, Ky, Kz:
                           ! --- For now we only do second-order accurate---
                            ! --- rectangular grid ---
                              Kx = (8.*(thermalKv(i1+1,i2,i3)-
     & thermalKv(i1-1,i2,i3))-(thermalKv(i1+2,i2,i3)-thermalKv(i1-2,
     & i2,i3)))*h41(0)
                              Ky = (8.*(thermalKv(i1,i2+1,i3)-
     & thermalKv(i1,i2-1,i3))-(thermalKv(i1,i2+2,i3)-thermalKv(i1,i2-
     & 2,i3)))*h41(1)
                                Kz = (8.*(thermalKv(i1,i2,i3+1)-
     & thermalKv(i1,i2,i3-1))-(thermalKv(i1,i2,i3+2)-thermalKv(i1,i2,
     & i3-2)))*h41(2)
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux43r(i1,
     & i2,i3,tc)-uu(i1,i2,i3,vc)*uy43r(i1,i2,i3,tc)-uu(i1,i2,i3,wc)*
     & uz43r(i1,i2,i3,tc)+adcBoussinesq*delta4 3(tc)
                       ! include implicit terms - diffusion
                       uti(i1,i2,i3,tc)= ((thermalKv(i1,i2,i3)*
     & ulaplacian43r(i1,i2,i3,tc)+Kx*ux43r(i1,i2,i3,tc)+Ky*uy43r(i1,
     & i2,i3,tc))/(rhov(i1,i2,i3)*Cpv(i1,i2,i3)))
                       if( isAxisymmetric.eq.1 )then
                           ! finish me for axisymmetric and variable material properties
                           stop 8569
                         ri=radiusInverse(i1,i2,i3)
                         if( ri.ne.0. )then
                           uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*(
     &  uy43r(i1,i2,i3,tc)*ri )
                         else
                           uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*(
     &  uyy43r(i1,i2,i3,tc) )
                         end if
                       end if
                        end if
                       end do
                       end do
                       end do
                    end if
                  else
                   stop 2281
                  end if
                 else if( gridType.eq.curvilinear )then
                  if( orderOfAccuracy.eq.2 )then
                    if( nd.eq.2 )then
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! Kx = (thermalKv(i1+1,i2,i3)-thermalKv(i1-1,i2,i3))/(2.*dx(0))
                          ! Ky = (thermalKv(i1,i2+1,i3)-thermalKv(i1,i2-1,i3))/(2.*dx(1))
                          ! Compute Kx, Ky, Kz:
                           ! --- For now we only do second-order accurate---
                              ! --- curvilinear grid ---
                                Kr = (thermalKv(i1+1,i2,i3)-thermalKv(
     & i1-1,i2,i3))/(2.*dr(0))
                                Ks = (thermalKv(i1,i2+1,i3)-thermalKv(
     & i1,i2-1,i3))/(2.*dr(1))
                                Kx = rx(i1,i2,i3)*Kr+sx(i1,i2,i3)*Ks
                                Ky = ry(i1,i2,i3)*Kr+sy(i1,i2,i3)*Ks
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux22(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy22(i1,i2,i3,tc)+adcBoussinesq*delta2 
     & 2(tc)
                       ! include implicit terms - diffusion
                       uti(i1,i2,i3,tc)= ((thermalKv(i1,i2,i3)*
     & ulaplacian22(i1,i2,i3,tc)+Kx*ux22(i1,i2,i3,tc)+Ky*uy22(i1,i2,
     & i3,tc))/(rhov(i1,i2,i3)*Cpv(i1,i2,i3)))
                       if( isAxisymmetric.eq.1 )then
                           ! finish me for axisymmetric and variable material properties
                           stop 8569
                         ri=radiusInverse(i1,i2,i3)
                         if( ri.ne.0. )then
                           uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*(
     &  uy22(i1,i2,i3,tc)*ri )
                         else
                           uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*(
     &  uyy22(i1,i2,i3,tc) )
                         end if
                       end if
                        end if
                       end do
                       end do
                       end do
                    else
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                            ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)-gravity(2)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! Kx = (thermalKv(i1+1,i2,i3)-thermalKv(i1-1,i2,i3))/(2.*dx(0))
                          ! Ky = (thermalKv(i1,i2+1,i3)-thermalKv(i1,i2-1,i3))/(2.*dx(1))
                          ! Compute Kx, Ky, Kz:
                           ! --- For now we only do second-order accurate---
                              ! --- curvilinear grid ---
                                Kr = (thermalKv(i1+1,i2,i3)-thermalKv(
     & i1-1,i2,i3))/(2.*dr(0))
                                Ks = (thermalKv(i1,i2+1,i3)-thermalKv(
     & i1,i2-1,i3))/(2.*dr(1))
                                  Kt = (thermalKv(i1,i2,i3+1)-
     & thermalKv(i1,i2,i3-1))/(2.*dr(2))
                                Kx = rx(i1,i2,i3)*Kr+sx(i1,i2,i3)*Ks+
     & tx(i1,i2,i3)*Kt
                                Ky = ry(i1,i2,i3)*Kr+sy(i1,i2,i3)*Ks+
     & ty(i1,i2,i3)*Kt
                                Kz = rz(i1,i2,i3)*Kr+sz(i1,i2,i3)*Ks+
     & tz(i1,i2,i3)*Kt
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy23(i1,i2,i3,tc)-uu(i1,i2,i3,wc)*uz23(
     & i1,i2,i3,tc)+adcBoussinesq*delta2 3(tc)
                       ! include implicit terms - diffusion
                       uti(i1,i2,i3,tc)= ((thermalKv(i1,i2,i3)*
     & ulaplacian23(i1,i2,i3,tc)+Kx*ux23(i1,i2,i3,tc)+Ky*uy23(i1,i2,
     & i3,tc))/(rhov(i1,i2,i3)*Cpv(i1,i2,i3)))
                       if( isAxisymmetric.eq.1 )then
                           ! finish me for axisymmetric and variable material properties
                           stop 8569
                         ri=radiusInverse(i1,i2,i3)
                         if( ri.ne.0. )then
                           uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*(
     &  uy23(i1,i2,i3,tc)*ri )
                         else
                           uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*(
     &  uyy23(i1,i2,i3,tc) )
                         end if
                       end if
                        end if
                       end do
                       end do
                       end do
                    end if
                  else if( orderOfAccuracy.eq.4 )then
                    if( nd.eq.2 )then
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! Kx = (thermalKv(i1+1,i2,i3)-thermalKv(i1-1,i2,i3))/(2.*dx(0))
                          ! Ky = (thermalKv(i1,i2+1,i3)-thermalKv(i1,i2-1,i3))/(2.*dx(1))
                          ! Compute Kx, Ky, Kz:
                           ! --- For now we only do second-order accurate---
                              ! --- curvilinear grid ---
                                Kr = (8.*(thermalKv(i1+1,i2,i3)-
     & thermalKv(i1-1,i2,i3))-(thermalKv(i1+2,i2,i3)-thermalKv(i1-2,
     & i2,i3)))*d14(0)
                                Ks = (8.*(thermalKv(i1,i2+1,i3)-
     & thermalKv(i1,i2-1,i3))-(thermalKv(i1,i2+2,i3)-thermalKv(i1,i2-
     & 2,i3)))*d14(1)
                                Kx = rx(i1,i2,i3)*Kr+sx(i1,i2,i3)*Ks
                                Ky = ry(i1,i2,i3)*Kr+sy(i1,i2,i3)*Ks
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux42(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy42(i1,i2,i3,tc)+adcBoussinesq*delta4 
     & 2(tc)
                       ! include implicit terms - diffusion
                       uti(i1,i2,i3,tc)= ((thermalKv(i1,i2,i3)*
     & ulaplacian42(i1,i2,i3,tc)+Kx*ux42(i1,i2,i3,tc)+Ky*uy42(i1,i2,
     & i3,tc))/(rhov(i1,i2,i3)*Cpv(i1,i2,i3)))
                       if( isAxisymmetric.eq.1 )then
                           ! finish me for axisymmetric and variable material properties
                           stop 8569
                         ri=radiusInverse(i1,i2,i3)
                         if( ri.ne.0. )then
                           uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*(
     &  uy42(i1,i2,i3,tc)*ri )
                         else
                           uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*(
     &  uyy42(i1,i2,i3,tc) )
                         end if
                       end if
                        end if
                       end do
                       end do
                       end do
                    else
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                            ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)-gravity(2)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! Kx = (thermalKv(i1+1,i2,i3)-thermalKv(i1-1,i2,i3))/(2.*dx(0))
                          ! Ky = (thermalKv(i1,i2+1,i3)-thermalKv(i1,i2-1,i3))/(2.*dx(1))
                          ! Compute Kx, Ky, Kz:
                           ! --- For now we only do second-order accurate---
                              ! --- curvilinear grid ---
                                Kr = (8.*(thermalKv(i1+1,i2,i3)-
     & thermalKv(i1-1,i2,i3))-(thermalKv(i1+2,i2,i3)-thermalKv(i1-2,
     & i2,i3)))*d14(0)
                                Ks = (8.*(thermalKv(i1,i2+1,i3)-
     & thermalKv(i1,i2-1,i3))-(thermalKv(i1,i2+2,i3)-thermalKv(i1,i2-
     & 2,i3)))*d14(1)
                                  Kt = (8.*(thermalKv(i1,i2,i3+1)-
     & thermalKv(i1,i2,i3-1))-(thermalKv(i1,i2,i3+2)-thermalKv(i1,i2,
     & i3-2)))*d14(2)
                                Kx = rx(i1,i2,i3)*Kr+sx(i1,i2,i3)*Ks+
     & tx(i1,i2,i3)*Kt
                                Ky = ry(i1,i2,i3)*Kr+sy(i1,i2,i3)*Ks+
     & ty(i1,i2,i3)*Kt
                                Kz = rz(i1,i2,i3)*Kr+sz(i1,i2,i3)*Ks+
     & tz(i1,i2,i3)*Kt
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux43(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy43(i1,i2,i3,tc)-uu(i1,i2,i3,wc)*uz43(
     & i1,i2,i3,tc)+adcBoussinesq*delta4 3(tc)
                       ! include implicit terms - diffusion
                       uti(i1,i2,i3,tc)= ((thermalKv(i1,i2,i3)*
     & ulaplacian43(i1,i2,i3,tc)+Kx*ux43(i1,i2,i3,tc)+Ky*uy43(i1,i2,
     & i3,tc))/(rhov(i1,i2,i3)*Cpv(i1,i2,i3)))
                       if( isAxisymmetric.eq.1 )then
                           ! finish me for axisymmetric and variable material properties
                           stop 8569
                         ri=radiusInverse(i1,i2,i3)
                         if( ri.ne.0. )then
                           uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*(
     &  uy43(i1,i2,i3,tc)*ri )
                         else
                           uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*(
     &  uyy43(i1,i2,i3,tc) )
                         end if
                       end if
                        end if
                       end do
                       end do
                       end do
                    end if
                  else
                   stop 2282
                  end if
                 else
                   stop 2717
                 end if
              else if( implicitOption.eq.doNotComputeImplicitTerms )
     & then
                 if( gridType.eq.rectangular )then
                  if( orderOfAccuracy.eq.2 )then
                    if( nd.eq.2 )then
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! Kx = (thermalKv(i1+1,i2,i3)-thermalKv(i1-1,i2,i3))/(2.*dx(0))
                          ! Ky = (thermalKv(i1,i2+1,i3)-thermalKv(i1,i2-1,i3))/(2.*dx(1))
                          ! Compute Kx, Ky, Kz:
                           ! --- For now we only do second-order accurate---
                            ! --- rectangular grid ---
                              Kx = (thermalKv(i1+1,i2,i3)-thermalKv(i1-
     & 1,i2,i3))/(2.*dx(0))
                              Ky = (thermalKv(i1,i2+1,i3)-thermalKv(i1,
     & i2-1,i3))/(2.*dx(1))
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux22r(i1,
     & i2,i3,tc)-uu(i1,i2,i3,vc)*uy22r(i1,i2,i3,tc)+adcBoussinesq*
     & delta2 2(tc)
                        end if
                       end do
                       end do
                       end do
                    else
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                            ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)-gravity(2)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! Kx = (thermalKv(i1+1,i2,i3)-thermalKv(i1-1,i2,i3))/(2.*dx(0))
                          ! Ky = (thermalKv(i1,i2+1,i3)-thermalKv(i1,i2-1,i3))/(2.*dx(1))
                          ! Compute Kx, Ky, Kz:
                           ! --- For now we only do second-order accurate---
                            ! --- rectangular grid ---
                              Kx = (thermalKv(i1+1,i2,i3)-thermalKv(i1-
     & 1,i2,i3))/(2.*dx(0))
                              Ky = (thermalKv(i1,i2+1,i3)-thermalKv(i1,
     & i2-1,i3))/(2.*dx(1))
                                Kz = (thermalKv(i1,i2,i3+1)-thermalKv(
     & i1,i2,i3-1))/(2.*dx(2))
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux23r(i1,
     & i2,i3,tc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,tc)-uu(i1,i2,i3,wc)*
     & uz23r(i1,i2,i3,tc)+adcBoussinesq*delta2 3(tc)
                        end if
                       end do
                       end do
                       end do
                    end if
                  else if( orderOfAccuracy.eq.4 )then
                    if( nd.eq.2 )then
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! Kx = (thermalKv(i1+1,i2,i3)-thermalKv(i1-1,i2,i3))/(2.*dx(0))
                          ! Ky = (thermalKv(i1,i2+1,i3)-thermalKv(i1,i2-1,i3))/(2.*dx(1))
                          ! Compute Kx, Ky, Kz:
                           ! --- For now we only do second-order accurate---
                            ! --- rectangular grid ---
                              Kx = (8.*(thermalKv(i1+1,i2,i3)-
     & thermalKv(i1-1,i2,i3))-(thermalKv(i1+2,i2,i3)-thermalKv(i1-2,
     & i2,i3)))*h41(0)
                              Ky = (8.*(thermalKv(i1,i2+1,i3)-
     & thermalKv(i1,i2-1,i3))-(thermalKv(i1,i2+2,i3)-thermalKv(i1,i2-
     & 2,i3)))*h41(1)
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux42r(i1,
     & i2,i3,tc)-uu(i1,i2,i3,vc)*uy42r(i1,i2,i3,tc)+adcBoussinesq*
     & delta4 2(tc)
                        end if
                       end do
                       end do
                       end do
                    else
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                            ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)-gravity(2)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! Kx = (thermalKv(i1+1,i2,i3)-thermalKv(i1-1,i2,i3))/(2.*dx(0))
                          ! Ky = (thermalKv(i1,i2+1,i3)-thermalKv(i1,i2-1,i3))/(2.*dx(1))
                          ! Compute Kx, Ky, Kz:
                           ! --- For now we only do second-order accurate---
                            ! --- rectangular grid ---
                              Kx = (8.*(thermalKv(i1+1,i2,i3)-
     & thermalKv(i1-1,i2,i3))-(thermalKv(i1+2,i2,i3)-thermalKv(i1-2,
     & i2,i3)))*h41(0)
                              Ky = (8.*(thermalKv(i1,i2+1,i3)-
     & thermalKv(i1,i2-1,i3))-(thermalKv(i1,i2+2,i3)-thermalKv(i1,i2-
     & 2,i3)))*h41(1)
                                Kz = (8.*(thermalKv(i1,i2,i3+1)-
     & thermalKv(i1,i2,i3-1))-(thermalKv(i1,i2,i3+2)-thermalKv(i1,i2,
     & i3-2)))*h41(2)
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux43r(i1,
     & i2,i3,tc)-uu(i1,i2,i3,vc)*uy43r(i1,i2,i3,tc)-uu(i1,i2,i3,wc)*
     & uz43r(i1,i2,i3,tc)+adcBoussinesq*delta4 3(tc)
                        end if
                       end do
                       end do
                       end do
                    end if
                  else
                   stop 2281
                  end if
                 else if( gridType.eq.curvilinear )then
                  if( orderOfAccuracy.eq.2 )then
                    if( nd.eq.2 )then
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! Kx = (thermalKv(i1+1,i2,i3)-thermalKv(i1-1,i2,i3))/(2.*dx(0))
                          ! Ky = (thermalKv(i1,i2+1,i3)-thermalKv(i1,i2-1,i3))/(2.*dx(1))
                          ! Compute Kx, Ky, Kz:
                           ! --- For now we only do second-order accurate---
                              ! --- curvilinear grid ---
                                Kr = (thermalKv(i1+1,i2,i3)-thermalKv(
     & i1-1,i2,i3))/(2.*dr(0))
                                Ks = (thermalKv(i1,i2+1,i3)-thermalKv(
     & i1,i2-1,i3))/(2.*dr(1))
                                Kx = rx(i1,i2,i3)*Kr+sx(i1,i2,i3)*Ks
                                Ky = ry(i1,i2,i3)*Kr+sy(i1,i2,i3)*Ks
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux22(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy22(i1,i2,i3,tc)+adcBoussinesq*delta2 
     & 2(tc)
                        end if
                       end do
                       end do
                       end do
                    else
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                            ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)-gravity(2)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! Kx = (thermalKv(i1+1,i2,i3)-thermalKv(i1-1,i2,i3))/(2.*dx(0))
                          ! Ky = (thermalKv(i1,i2+1,i3)-thermalKv(i1,i2-1,i3))/(2.*dx(1))
                          ! Compute Kx, Ky, Kz:
                           ! --- For now we only do second-order accurate---
                              ! --- curvilinear grid ---
                                Kr = (thermalKv(i1+1,i2,i3)-thermalKv(
     & i1-1,i2,i3))/(2.*dr(0))
                                Ks = (thermalKv(i1,i2+1,i3)-thermalKv(
     & i1,i2-1,i3))/(2.*dr(1))
                                  Kt = (thermalKv(i1,i2,i3+1)-
     & thermalKv(i1,i2,i3-1))/(2.*dr(2))
                                Kx = rx(i1,i2,i3)*Kr+sx(i1,i2,i3)*Ks+
     & tx(i1,i2,i3)*Kt
                                Ky = ry(i1,i2,i3)*Kr+sy(i1,i2,i3)*Ks+
     & ty(i1,i2,i3)*Kt
                                Kz = rz(i1,i2,i3)*Kr+sz(i1,i2,i3)*Ks+
     & tz(i1,i2,i3)*Kt
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy23(i1,i2,i3,tc)-uu(i1,i2,i3,wc)*uz23(
     & i1,i2,i3,tc)+adcBoussinesq*delta2 3(tc)
                        end if
                       end do
                       end do
                       end do
                    end if
                  else if( orderOfAccuracy.eq.4 )then
                    if( nd.eq.2 )then
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! Kx = (thermalKv(i1+1,i2,i3)-thermalKv(i1-1,i2,i3))/(2.*dx(0))
                          ! Ky = (thermalKv(i1,i2+1,i3)-thermalKv(i1,i2-1,i3))/(2.*dx(1))
                          ! Compute Kx, Ky, Kz:
                           ! --- For now we only do second-order accurate---
                              ! --- curvilinear grid ---
                                Kr = (8.*(thermalKv(i1+1,i2,i3)-
     & thermalKv(i1-1,i2,i3))-(thermalKv(i1+2,i2,i3)-thermalKv(i1-2,
     & i2,i3)))*d14(0)
                                Ks = (8.*(thermalKv(i1,i2+1,i3)-
     & thermalKv(i1,i2-1,i3))-(thermalKv(i1,i2+2,i3)-thermalKv(i1,i2-
     & 2,i3)))*d14(1)
                                Kx = rx(i1,i2,i3)*Kr+sx(i1,i2,i3)*Ks
                                Ky = ry(i1,i2,i3)*Kr+sy(i1,i2,i3)*Ks
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux42(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy42(i1,i2,i3,tc)+adcBoussinesq*delta4 
     & 2(tc)
                        end if
                       end do
                       end do
                       end do
                    else
                       ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                       kThermalLES = kThermal/nu
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          ! add on gravity terms to the momentum equations:
                          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                            ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)-gravity(2)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ! --- Temperature Equation ---
                        ! Define the diffusion term: 
                          ! Kx = (thermalKv(i1+1,i2,i3)-thermalKv(i1-1,i2,i3))/(2.*dx(0))
                          ! Ky = (thermalKv(i1,i2+1,i3)-thermalKv(i1,i2-1,i3))/(2.*dx(1))
                          ! Compute Kx, Ky, Kz:
                           ! --- For now we only do second-order accurate---
                              ! --- curvilinear grid ---
                                Kr = (8.*(thermalKv(i1+1,i2,i3)-
     & thermalKv(i1-1,i2,i3))-(thermalKv(i1+2,i2,i3)-thermalKv(i1-2,
     & i2,i3)))*d14(0)
                                Ks = (8.*(thermalKv(i1,i2+1,i3)-
     & thermalKv(i1,i2-1,i3))-(thermalKv(i1,i2+2,i3)-thermalKv(i1,i2-
     & 2,i3)))*d14(1)
                                  Kt = (8.*(thermalKv(i1,i2,i3+1)-
     & thermalKv(i1,i2,i3-1))-(thermalKv(i1,i2,i3+2)-thermalKv(i1,i2,
     & i3-2)))*d14(2)
                                Kx = rx(i1,i2,i3)*Kr+sx(i1,i2,i3)*Ks+
     & tx(i1,i2,i3)*Kt
                                Ky = ry(i1,i2,i3)*Kr+sy(i1,i2,i3)*Ks+
     & ty(i1,i2,i3)*Kt
                                Kz = rz(i1,i2,i3)*Kr+sz(i1,i2,i3)*Ks+
     & tz(i1,i2,i3)*Kt
                          ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux43(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy43(i1,i2,i3,tc)-uu(i1,i2,i3,wc)*uz43(
     & i1,i2,i3,tc)+adcBoussinesq*delta4 3(tc)
                        end if
                       end do
                       end do
                       end do
                    end if
                  else
                   stop 2282
                  end if
                 else
                   stop 2717
                 end if
              else
               write(*,*)'insdt:boussinesq Unknown implicitOption=',
     & implicitOption
               stop 4135
              end if  ! end implicitOption
             end if
          end if


        else if( turbulenceModel.eq.largeEddySimulation )then
          ! variable thermal diffusivity:
           if( gridIsImplicit.eq.0 )then ! explicit
               if( gridType.eq.rectangular )then
                if( orderOfAccuracy.eq.2 )then
                  if( nd.eq.2 )then
                     ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                     kThermalLES = kThermal/nu
                      if( t.le.0 )then
                        write(*,'(" --- insdt:evaluate LES with 
     & variable diffusivity Temperature equation t=",e10.2,"---")') t
                      end if
                     do i3=n3a,n3b
                     do i2=n2a,n2b
                     do i1=n1a,n1b
                      if( mask(i1,i2,i3).gt.0 )then
                        ! add on gravity terms to the momentum equations:
                        ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                      ! --- Temperature Equation ---
                      ! Define the diffusion term: 
                      ! variable (turbulent) thermal diffusivity
                      ! Evaluate nuT and its derivatives: 
                         nuT = u(i1,i2,i3,vsc)
                         nuTx=ux22r(i1,i2,i3,vsc)
                         nuTy=uy22r(i1,i2,i3,vsc)
                        ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux22r(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy22r(i1,i2,i3,tc) +((nuT*ulaplacian22r(
     & i1,i2,i3,tc)+nuTx*(ux22r(i1,i2,i3,tc))+nuTy*(uy22r(i1,i2,i3,tc)
     & ))*kThermalLES) +adcBoussinesq*delta2 2(tc)
                      if( isAxisymmetric.eq.1 )then
                        ! -- add on axisymmetric corrections ---
                          ! finish me for axisymmetric and variable (turbulent) thermal diffusivity
                          stop 8567
                        ri=radiusInverse(i1,i2,i3)
                        if( ri.ne.0. )then
                          ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uy22r(i1,i2,i3,tc)*ri )
                        else
                          ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uyy22r(i1,i2,i3,tc) )
                        end if
                      end if
                      end if
                     end do
                     end do
                     end do
                  else
                     ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                     kThermalLES = kThermal/nu
                      if( t.le.0 )then
                        write(*,'(" --- insdt:evaluate LES with 
     & variable diffusivity Temperature equation t=",e10.2,"---")') t
                      end if
                     do i3=n3a,n3b
                     do i2=n2a,n2b
                     do i1=n1a,n1b
                      if( mask(i1,i2,i3).gt.0 )then
                        ! add on gravity terms to the momentum equations:
                        ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)-gravity(2)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                      ! --- Temperature Equation ---
                      ! Define the diffusion term: 
                      ! variable (turbulent) thermal diffusivity
                      ! Evaluate nuT and its derivatives: 
                         nuT = u(i1,i2,i3,vsc)
                         nuTx=ux23r(i1,i2,i3,vsc)
                         nuTy=uy23r(i1,i2,i3,vsc)
                          nuTz=uz23r(i1,i2,i3,vsc)
                        ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,tc)-uu(i1,i2,i3,wc)*
     & uz23r(i1,i2,i3,tc) +((nuT*ulaplacian23r(i1,i2,i3,tc)+nuTx*(
     & ux23r(i1,i2,i3,tc))+nuTy*(uy23r(i1,i2,i3,tc))+nuTz*(uz23r(i1,
     & i2,i3,tc)))*kThermalLES) +adcBoussinesq*delta2 3(tc)
                      if( isAxisymmetric.eq.1 )then
                        ! -- add on axisymmetric corrections ---
                          ! finish me for axisymmetric and variable (turbulent) thermal diffusivity
                          stop 8567
                        ri=radiusInverse(i1,i2,i3)
                        if( ri.ne.0. )then
                          ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uy23r(i1,i2,i3,tc)*ri )
                        else
                          ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uyy23r(i1,i2,i3,tc) )
                        end if
                      end if
                      end if
                     end do
                     end do
                     end do
                  end if
                else if( orderOfAccuracy.eq.4 )then
                  if( nd.eq.2 )then
                     ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                     kThermalLES = kThermal/nu
                      if( t.le.0 )then
                        write(*,'(" --- insdt:evaluate LES with 
     & variable diffusivity Temperature equation t=",e10.2,"---")') t
                      end if
                     do i3=n3a,n3b
                     do i2=n2a,n2b
                     do i1=n1a,n1b
                      if( mask(i1,i2,i3).gt.0 )then
                        ! add on gravity terms to the momentum equations:
                        ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                      ! --- Temperature Equation ---
                      ! Define the diffusion term: 
                      ! variable (turbulent) thermal diffusivity
                      ! Evaluate nuT and its derivatives: 
                         nuT = u(i1,i2,i3,vsc)
                         nuTx=ux42r(i1,i2,i3,vsc)
                         nuTy=uy42r(i1,i2,i3,vsc)
                        ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux42r(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy42r(i1,i2,i3,tc) +((nuT*ulaplacian42r(
     & i1,i2,i3,tc)+nuTx*(ux42r(i1,i2,i3,tc))+nuTy*(uy42r(i1,i2,i3,tc)
     & ))*kThermalLES) +adcBoussinesq*delta4 2(tc)
                      if( isAxisymmetric.eq.1 )then
                        ! -- add on axisymmetric corrections ---
                          ! finish me for axisymmetric and variable (turbulent) thermal diffusivity
                          stop 8567
                        ri=radiusInverse(i1,i2,i3)
                        if( ri.ne.0. )then
                          ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uy42r(i1,i2,i3,tc)*ri )
                        else
                          ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uyy42r(i1,i2,i3,tc) )
                        end if
                      end if
                      end if
                     end do
                     end do
                     end do
                  else
                     ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                     kThermalLES = kThermal/nu
                      if( t.le.0 )then
                        write(*,'(" --- insdt:evaluate LES with 
     & variable diffusivity Temperature equation t=",e10.2,"---")') t
                      end if
                     do i3=n3a,n3b
                     do i2=n2a,n2b
                     do i1=n1a,n1b
                      if( mask(i1,i2,i3).gt.0 )then
                        ! add on gravity terms to the momentum equations:
                        ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)-gravity(2)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                      ! --- Temperature Equation ---
                      ! Define the diffusion term: 
                      ! variable (turbulent) thermal diffusivity
                      ! Evaluate nuT and its derivatives: 
                         nuT = u(i1,i2,i3,vsc)
                         nuTx=ux43r(i1,i2,i3,vsc)
                         nuTy=uy43r(i1,i2,i3,vsc)
                          nuTz=uz43r(i1,i2,i3,vsc)
                        ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux43r(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy43r(i1,i2,i3,tc)-uu(i1,i2,i3,wc)*
     & uz43r(i1,i2,i3,tc) +((nuT*ulaplacian43r(i1,i2,i3,tc)+nuTx*(
     & ux43r(i1,i2,i3,tc))+nuTy*(uy43r(i1,i2,i3,tc))+nuTz*(uz43r(i1,
     & i2,i3,tc)))*kThermalLES) +adcBoussinesq*delta4 3(tc)
                      if( isAxisymmetric.eq.1 )then
                        ! -- add on axisymmetric corrections ---
                          ! finish me for axisymmetric and variable (turbulent) thermal diffusivity
                          stop 8567
                        ri=radiusInverse(i1,i2,i3)
                        if( ri.ne.0. )then
                          ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uy43r(i1,i2,i3,tc)*ri )
                        else
                          ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uyy43r(i1,i2,i3,tc) )
                        end if
                      end if
                      end if
                     end do
                     end do
                     end do
                  end if
                else
                 stop 2281
                end if
               else if( gridType.eq.curvilinear )then
                if( orderOfAccuracy.eq.2 )then
                  if( nd.eq.2 )then
                     ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                     kThermalLES = kThermal/nu
                      if( t.le.0 )then
                        write(*,'(" --- insdt:evaluate LES with 
     & variable diffusivity Temperature equation t=",e10.2,"---")') t
                      end if
                     do i3=n3a,n3b
                     do i2=n2a,n2b
                     do i1=n1a,n1b
                      if( mask(i1,i2,i3).gt.0 )then
                        ! add on gravity terms to the momentum equations:
                        ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                      ! --- Temperature Equation ---
                      ! Define the diffusion term: 
                      ! variable (turbulent) thermal diffusivity
                      ! Evaluate nuT and its derivatives: 
                         nuT = u(i1,i2,i3,vsc)
                         nuTx=ux22(i1,i2,i3,vsc)
                         nuTy=uy22(i1,i2,i3,vsc)
                        ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux22(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy22(i1,i2,i3,tc) +((nuT*ulaplacian22(
     & i1,i2,i3,tc)+nuTx*(ux22(i1,i2,i3,tc))+nuTy*(uy22(i1,i2,i3,tc)))
     & *kThermalLES) +adcBoussinesq*delta2 2(tc)
                      if( isAxisymmetric.eq.1 )then
                        ! -- add on axisymmetric corrections ---
                          ! finish me for axisymmetric and variable (turbulent) thermal diffusivity
                          stop 8567
                        ri=radiusInverse(i1,i2,i3)
                        if( ri.ne.0. )then
                          ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uy22(i1,i2,i3,tc)*ri )
                        else
                          ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uyy22(i1,i2,i3,tc) )
                        end if
                      end if
                      end if
                     end do
                     end do
                     end do
                  else
                     ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                     kThermalLES = kThermal/nu
                      if( t.le.0 )then
                        write(*,'(" --- insdt:evaluate LES with 
     & variable diffusivity Temperature equation t=",e10.2,"---")') t
                      end if
                     do i3=n3a,n3b
                     do i2=n2a,n2b
                     do i1=n1a,n1b
                      if( mask(i1,i2,i3).gt.0 )then
                        ! add on gravity terms to the momentum equations:
                        ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)-gravity(2)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                      ! --- Temperature Equation ---
                      ! Define the diffusion term: 
                      ! variable (turbulent) thermal diffusivity
                      ! Evaluate nuT and its derivatives: 
                         nuT = u(i1,i2,i3,vsc)
                         nuTx=ux23(i1,i2,i3,vsc)
                         nuTy=uy23(i1,i2,i3,vsc)
                          nuTz=uz23(i1,i2,i3,vsc)
                        ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy23(i1,i2,i3,tc)-uu(i1,i2,i3,wc)*uz23(
     & i1,i2,i3,tc) +((nuT*ulaplacian23(i1,i2,i3,tc)+nuTx*(ux23(i1,i2,
     & i3,tc))+nuTy*(uy23(i1,i2,i3,tc))+nuTz*(uz23(i1,i2,i3,tc)))*
     & kThermalLES) +adcBoussinesq*delta2 3(tc)
                      if( isAxisymmetric.eq.1 )then
                        ! -- add on axisymmetric corrections ---
                          ! finish me for axisymmetric and variable (turbulent) thermal diffusivity
                          stop 8567
                        ri=radiusInverse(i1,i2,i3)
                        if( ri.ne.0. )then
                          ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uy23(i1,i2,i3,tc)*ri )
                        else
                          ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uyy23(i1,i2,i3,tc) )
                        end if
                      end if
                      end if
                     end do
                     end do
                     end do
                  end if
                else if( orderOfAccuracy.eq.4 )then
                  if( nd.eq.2 )then
                     ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                     kThermalLES = kThermal/nu
                      if( t.le.0 )then
                        write(*,'(" --- insdt:evaluate LES with 
     & variable diffusivity Temperature equation t=",e10.2,"---")') t
                      end if
                     do i3=n3a,n3b
                     do i2=n2a,n2b
                     do i1=n1a,n1b
                      if( mask(i1,i2,i3).gt.0 )then
                        ! add on gravity terms to the momentum equations:
                        ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                      ! --- Temperature Equation ---
                      ! Define the diffusion term: 
                      ! variable (turbulent) thermal diffusivity
                      ! Evaluate nuT and its derivatives: 
                         nuT = u(i1,i2,i3,vsc)
                         nuTx=ux42(i1,i2,i3,vsc)
                         nuTy=uy42(i1,i2,i3,vsc)
                        ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux42(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy42(i1,i2,i3,tc) +((nuT*ulaplacian42(
     & i1,i2,i3,tc)+nuTx*(ux42(i1,i2,i3,tc))+nuTy*(uy42(i1,i2,i3,tc)))
     & *kThermalLES) +adcBoussinesq*delta4 2(tc)
                      if( isAxisymmetric.eq.1 )then
                        ! -- add on axisymmetric corrections ---
                          ! finish me for axisymmetric and variable (turbulent) thermal diffusivity
                          stop 8567
                        ri=radiusInverse(i1,i2,i3)
                        if( ri.ne.0. )then
                          ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uy42(i1,i2,i3,tc)*ri )
                        else
                          ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uyy42(i1,i2,i3,tc) )
                        end if
                      end if
                      end if
                     end do
                     end do
                     end do
                  else
                     ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                     kThermalLES = kThermal/nu
                      if( t.le.0 )then
                        write(*,'(" --- insdt:evaluate LES with 
     & variable diffusivity Temperature equation t=",e10.2,"---")') t
                      end if
                     do i3=n3a,n3b
                     do i2=n2a,n2b
                     do i1=n1a,n1b
                      if( mask(i1,i2,i3).gt.0 )then
                        ! add on gravity terms to the momentum equations:
                        ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)-gravity(2)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                      ! --- Temperature Equation ---
                      ! Define the diffusion term: 
                      ! variable (turbulent) thermal diffusivity
                      ! Evaluate nuT and its derivatives: 
                         nuT = u(i1,i2,i3,vsc)
                         nuTx=ux43(i1,i2,i3,vsc)
                         nuTy=uy43(i1,i2,i3,vsc)
                          nuTz=uz43(i1,i2,i3,vsc)
                        ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux43(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy43(i1,i2,i3,tc)-uu(i1,i2,i3,wc)*uz43(
     & i1,i2,i3,tc) +((nuT*ulaplacian43(i1,i2,i3,tc)+nuTx*(ux43(i1,i2,
     & i3,tc))+nuTy*(uy43(i1,i2,i3,tc))+nuTz*(uz43(i1,i2,i3,tc)))*
     & kThermalLES) +adcBoussinesq*delta4 3(tc)
                      if( isAxisymmetric.eq.1 )then
                        ! -- add on axisymmetric corrections ---
                          ! finish me for axisymmetric and variable (turbulent) thermal diffusivity
                          stop 8567
                        ri=radiusInverse(i1,i2,i3)
                        if( ri.ne.0. )then
                          ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uy43(i1,i2,i3,tc)*ri )
                        else
                          ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( 
     & uyy43(i1,i2,i3,tc) )
                        end if
                      end if
                      end if
                     end do
                     end do
                     end do
                  end if
                else
                 stop 2282
                end if
               else
                 stop 2717
               end if
           else ! ***** implicit *******
            if( implicitOption .eq.computeImplicitTermsSeparately )then
               if( gridType.eq.rectangular )then
                if( orderOfAccuracy.eq.2 )then
                  if( nd.eq.2 )then
                     ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                     kThermalLES = kThermal/nu
                      if( t.le.0 )then
                        write(*,'(" --- insdt:evaluate LES with 
     & variable diffusivity Temperature equation t=",e10.2,"---")') t
                      end if
                     do i3=n3a,n3b
                     do i2=n2a,n2b
                     do i1=n1a,n1b
                      if( mask(i1,i2,i3).gt.0 )then
                        ! add on gravity terms to the momentum equations:
                        ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                      ! --- Temperature Equation ---
                      ! Define the diffusion term: 
                      ! variable (turbulent) thermal diffusivity
                      ! Evaluate nuT and its derivatives: 
                         nuT = u(i1,i2,i3,vsc)
                         nuTx=ux22r(i1,i2,i3,vsc)
                         nuTy=uy22r(i1,i2,i3,vsc)
                        ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux22r(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy22r(i1,i2,i3,tc)+adcBoussinesq*delta2 
     & 2(tc)
                     ! include implicit terms - diffusion
                     uti(i1,i2,i3,tc)= ((nuT*ulaplacian22r(i1,i2,i3,tc)
     & +nuTx*(ux22r(i1,i2,i3,tc))+nuTy*(uy22r(i1,i2,i3,tc)))*
     & kThermalLES)
                     if( isAxisymmetric.eq.1 )then
                       ri=radiusInverse(i1,i2,i3)
                       if( ri.ne.0. )then
                         uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*( 
     & uy22r(i1,i2,i3,tc)*ri )
                       else
                         uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*( 
     & uyy22r(i1,i2,i3,tc) )
                       end if
                     end if
                      end if
                     end do
                     end do
                     end do
                  else
                     ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                     kThermalLES = kThermal/nu
                      if( t.le.0 )then
                        write(*,'(" --- insdt:evaluate LES with 
     & variable diffusivity Temperature equation t=",e10.2,"---")') t
                      end if
                     do i3=n3a,n3b
                     do i2=n2a,n2b
                     do i1=n1a,n1b
                      if( mask(i1,i2,i3).gt.0 )then
                        ! add on gravity terms to the momentum equations:
                        ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)-gravity(2)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                      ! --- Temperature Equation ---
                      ! Define the diffusion term: 
                      ! variable (turbulent) thermal diffusivity
                      ! Evaluate nuT and its derivatives: 
                         nuT = u(i1,i2,i3,vsc)
                         nuTx=ux23r(i1,i2,i3,vsc)
                         nuTy=uy23r(i1,i2,i3,vsc)
                          nuTz=uz23r(i1,i2,i3,vsc)
                        ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,tc)-uu(i1,i2,i3,wc)*
     & uz23r(i1,i2,i3,tc)+adcBoussinesq*delta2 3(tc)
                     ! include implicit terms - diffusion
                     uti(i1,i2,i3,tc)= ((nuT*ulaplacian23r(i1,i2,i3,tc)
     & +nuTx*(ux23r(i1,i2,i3,tc))+nuTy*(uy23r(i1,i2,i3,tc))+nuTz*(
     & uz23r(i1,i2,i3,tc)))*kThermalLES)
                     if( isAxisymmetric.eq.1 )then
                       ri=radiusInverse(i1,i2,i3)
                       if( ri.ne.0. )then
                         uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*( 
     & uy23r(i1,i2,i3,tc)*ri )
                       else
                         uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*( 
     & uyy23r(i1,i2,i3,tc) )
                       end if
                     end if
                      end if
                     end do
                     end do
                     end do
                  end if
                else if( orderOfAccuracy.eq.4 )then
                  if( nd.eq.2 )then
                     ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                     kThermalLES = kThermal/nu
                      if( t.le.0 )then
                        write(*,'(" --- insdt:evaluate LES with 
     & variable diffusivity Temperature equation t=",e10.2,"---")') t
                      end if
                     do i3=n3a,n3b
                     do i2=n2a,n2b
                     do i1=n1a,n1b
                      if( mask(i1,i2,i3).gt.0 )then
                        ! add on gravity terms to the momentum equations:
                        ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                      ! --- Temperature Equation ---
                      ! Define the diffusion term: 
                      ! variable (turbulent) thermal diffusivity
                      ! Evaluate nuT and its derivatives: 
                         nuT = u(i1,i2,i3,vsc)
                         nuTx=ux42r(i1,i2,i3,vsc)
                         nuTy=uy42r(i1,i2,i3,vsc)
                        ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux42r(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy42r(i1,i2,i3,tc)+adcBoussinesq*delta4 
     & 2(tc)
                     ! include implicit terms - diffusion
                     uti(i1,i2,i3,tc)= ((nuT*ulaplacian42r(i1,i2,i3,tc)
     & +nuTx*(ux42r(i1,i2,i3,tc))+nuTy*(uy42r(i1,i2,i3,tc)))*
     & kThermalLES)
                     if( isAxisymmetric.eq.1 )then
                       ri=radiusInverse(i1,i2,i3)
                       if( ri.ne.0. )then
                         uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*( 
     & uy42r(i1,i2,i3,tc)*ri )
                       else
                         uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*( 
     & uyy42r(i1,i2,i3,tc) )
                       end if
                     end if
                      end if
                     end do
                     end do
                     end do
                  else
                     ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                     kThermalLES = kThermal/nu
                      if( t.le.0 )then
                        write(*,'(" --- insdt:evaluate LES with 
     & variable diffusivity Temperature equation t=",e10.2,"---")') t
                      end if
                     do i3=n3a,n3b
                     do i2=n2a,n2b
                     do i1=n1a,n1b
                      if( mask(i1,i2,i3).gt.0 )then
                        ! add on gravity terms to the momentum equations:
                        ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)-gravity(2)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                      ! --- Temperature Equation ---
                      ! Define the diffusion term: 
                      ! variable (turbulent) thermal diffusivity
                      ! Evaluate nuT and its derivatives: 
                         nuT = u(i1,i2,i3,vsc)
                         nuTx=ux43r(i1,i2,i3,vsc)
                         nuTy=uy43r(i1,i2,i3,vsc)
                          nuTz=uz43r(i1,i2,i3,vsc)
                        ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux43r(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy43r(i1,i2,i3,tc)-uu(i1,i2,i3,wc)*
     & uz43r(i1,i2,i3,tc)+adcBoussinesq*delta4 3(tc)
                     ! include implicit terms - diffusion
                     uti(i1,i2,i3,tc)= ((nuT*ulaplacian43r(i1,i2,i3,tc)
     & +nuTx*(ux43r(i1,i2,i3,tc))+nuTy*(uy43r(i1,i2,i3,tc))+nuTz*(
     & uz43r(i1,i2,i3,tc)))*kThermalLES)
                     if( isAxisymmetric.eq.1 )then
                       ri=radiusInverse(i1,i2,i3)
                       if( ri.ne.0. )then
                         uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*( 
     & uy43r(i1,i2,i3,tc)*ri )
                       else
                         uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*( 
     & uyy43r(i1,i2,i3,tc) )
                       end if
                     end if
                      end if
                     end do
                     end do
                     end do
                  end if
                else
                 stop 2281
                end if
               else if( gridType.eq.curvilinear )then
                if( orderOfAccuracy.eq.2 )then
                  if( nd.eq.2 )then
                     ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                     kThermalLES = kThermal/nu
                      if( t.le.0 )then
                        write(*,'(" --- insdt:evaluate LES with 
     & variable diffusivity Temperature equation t=",e10.2,"---")') t
                      end if
                     do i3=n3a,n3b
                     do i2=n2a,n2b
                     do i1=n1a,n1b
                      if( mask(i1,i2,i3).gt.0 )then
                        ! add on gravity terms to the momentum equations:
                        ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                      ! --- Temperature Equation ---
                      ! Define the diffusion term: 
                      ! variable (turbulent) thermal diffusivity
                      ! Evaluate nuT and its derivatives: 
                         nuT = u(i1,i2,i3,vsc)
                         nuTx=ux22(i1,i2,i3,vsc)
                         nuTy=uy22(i1,i2,i3,vsc)
                        ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux22(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy22(i1,i2,i3,tc)+adcBoussinesq*delta2 
     & 2(tc)
                     ! include implicit terms - diffusion
                     uti(i1,i2,i3,tc)= ((nuT*ulaplacian22(i1,i2,i3,tc)+
     & nuTx*(ux22(i1,i2,i3,tc))+nuTy*(uy22(i1,i2,i3,tc)))*kThermalLES)
                     if( isAxisymmetric.eq.1 )then
                       ri=radiusInverse(i1,i2,i3)
                       if( ri.ne.0. )then
                         uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*( 
     & uy22(i1,i2,i3,tc)*ri )
                       else
                         uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*( 
     & uyy22(i1,i2,i3,tc) )
                       end if
                     end if
                      end if
                     end do
                     end do
                     end do
                  else
                     ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                     kThermalLES = kThermal/nu
                      if( t.le.0 )then
                        write(*,'(" --- insdt:evaluate LES with 
     & variable diffusivity Temperature equation t=",e10.2,"---")') t
                      end if
                     do i3=n3a,n3b
                     do i2=n2a,n2b
                     do i1=n1a,n1b
                      if( mask(i1,i2,i3).gt.0 )then
                        ! add on gravity terms to the momentum equations:
                        ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)-gravity(2)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                      ! --- Temperature Equation ---
                      ! Define the diffusion term: 
                      ! variable (turbulent) thermal diffusivity
                      ! Evaluate nuT and its derivatives: 
                         nuT = u(i1,i2,i3,vsc)
                         nuTx=ux23(i1,i2,i3,vsc)
                         nuTy=uy23(i1,i2,i3,vsc)
                          nuTz=uz23(i1,i2,i3,vsc)
                        ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy23(i1,i2,i3,tc)-uu(i1,i2,i3,wc)*uz23(
     & i1,i2,i3,tc)+adcBoussinesq*delta2 3(tc)
                     ! include implicit terms - diffusion
                     uti(i1,i2,i3,tc)= ((nuT*ulaplacian23(i1,i2,i3,tc)+
     & nuTx*(ux23(i1,i2,i3,tc))+nuTy*(uy23(i1,i2,i3,tc))+nuTz*(uz23(
     & i1,i2,i3,tc)))*kThermalLES)
                     if( isAxisymmetric.eq.1 )then
                       ri=radiusInverse(i1,i2,i3)
                       if( ri.ne.0. )then
                         uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*( 
     & uy23(i1,i2,i3,tc)*ri )
                       else
                         uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*( 
     & uyy23(i1,i2,i3,tc) )
                       end if
                     end if
                      end if
                     end do
                     end do
                     end do
                  end if
                else if( orderOfAccuracy.eq.4 )then
                  if( nd.eq.2 )then
                     ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                     kThermalLES = kThermal/nu
                      if( t.le.0 )then
                        write(*,'(" --- insdt:evaluate LES with 
     & variable diffusivity Temperature equation t=",e10.2,"---")') t
                      end if
                     do i3=n3a,n3b
                     do i2=n2a,n2b
                     do i1=n1a,n1b
                      if( mask(i1,i2,i3).gt.0 )then
                        ! add on gravity terms to the momentum equations:
                        ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                      ! --- Temperature Equation ---
                      ! Define the diffusion term: 
                      ! variable (turbulent) thermal diffusivity
                      ! Evaluate nuT and its derivatives: 
                         nuT = u(i1,i2,i3,vsc)
                         nuTx=ux42(i1,i2,i3,vsc)
                         nuTy=uy42(i1,i2,i3,vsc)
                        ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux42(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy42(i1,i2,i3,tc)+adcBoussinesq*delta4 
     & 2(tc)
                     ! include implicit terms - diffusion
                     uti(i1,i2,i3,tc)= ((nuT*ulaplacian42(i1,i2,i3,tc)+
     & nuTx*(ux42(i1,i2,i3,tc))+nuTy*(uy42(i1,i2,i3,tc)))*kThermalLES)
                     if( isAxisymmetric.eq.1 )then
                       ri=radiusInverse(i1,i2,i3)
                       if( ri.ne.0. )then
                         uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*( 
     & uy42(i1,i2,i3,tc)*ri )
                       else
                         uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*( 
     & uyy42(i1,i2,i3,tc) )
                       end if
                     end if
                      end if
                     end do
                     end do
                     end do
                  else
                     ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                     kThermalLES = kThermal/nu
                      if( t.le.0 )then
                        write(*,'(" --- insdt:evaluate LES with 
     & variable diffusivity Temperature equation t=",e10.2,"---")') t
                      end if
                     do i3=n3a,n3b
                     do i2=n2a,n2b
                     do i1=n1a,n1b
                      if( mask(i1,i2,i3).gt.0 )then
                        ! add on gravity terms to the momentum equations:
                        ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)-gravity(2)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                      ! --- Temperature Equation ---
                      ! Define the diffusion term: 
                      ! variable (turbulent) thermal diffusivity
                      ! Evaluate nuT and its derivatives: 
                         nuT = u(i1,i2,i3,vsc)
                         nuTx=ux43(i1,i2,i3,vsc)
                         nuTy=uy43(i1,i2,i3,vsc)
                          nuTz=uz43(i1,i2,i3,vsc)
                        ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux43(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy43(i1,i2,i3,tc)-uu(i1,i2,i3,wc)*uz43(
     & i1,i2,i3,tc)+adcBoussinesq*delta4 3(tc)
                     ! include implicit terms - diffusion
                     uti(i1,i2,i3,tc)= ((nuT*ulaplacian43(i1,i2,i3,tc)+
     & nuTx*(ux43(i1,i2,i3,tc))+nuTy*(uy43(i1,i2,i3,tc))+nuTz*(uz43(
     & i1,i2,i3,tc)))*kThermalLES)
                     if( isAxisymmetric.eq.1 )then
                       ri=radiusInverse(i1,i2,i3)
                       if( ri.ne.0. )then
                         uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*( 
     & uy43(i1,i2,i3,tc)*ri )
                       else
                         uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*( 
     & uyy43(i1,i2,i3,tc) )
                       end if
                     end if
                      end if
                     end do
                     end do
                     end do
                  end if
                else
                 stop 2282
                end if
               else
                 stop 2717
               end if
            else if( implicitOption.eq.doNotComputeImplicitTerms )then
               if( gridType.eq.rectangular )then
                if( orderOfAccuracy.eq.2 )then
                  if( nd.eq.2 )then
                     ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                     kThermalLES = kThermal/nu
                      if( t.le.0 )then
                        write(*,'(" --- insdt:evaluate LES with 
     & variable diffusivity Temperature equation t=",e10.2,"---")') t
                      end if
                     do i3=n3a,n3b
                     do i2=n2a,n2b
                     do i1=n1a,n1b
                      if( mask(i1,i2,i3).gt.0 )then
                        ! add on gravity terms to the momentum equations:
                        ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                      ! --- Temperature Equation ---
                      ! Define the diffusion term: 
                      ! variable (turbulent) thermal diffusivity
                      ! Evaluate nuT and its derivatives: 
                        ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux22r(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy22r(i1,i2,i3,tc)+adcBoussinesq*delta2 
     & 2(tc)
                      end if
                     end do
                     end do
                     end do
                  else
                     ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                     kThermalLES = kThermal/nu
                      if( t.le.0 )then
                        write(*,'(" --- insdt:evaluate LES with 
     & variable diffusivity Temperature equation t=",e10.2,"---")') t
                      end if
                     do i3=n3a,n3b
                     do i2=n2a,n2b
                     do i1=n1a,n1b
                      if( mask(i1,i2,i3).gt.0 )then
                        ! add on gravity terms to the momentum equations:
                        ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)-gravity(2)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                      ! --- Temperature Equation ---
                      ! Define the diffusion term: 
                      ! variable (turbulent) thermal diffusivity
                      ! Evaluate nuT and its derivatives: 
                        ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,tc)-uu(i1,i2,i3,wc)*
     & uz23r(i1,i2,i3,tc)+adcBoussinesq*delta2 3(tc)
                      end if
                     end do
                     end do
                     end do
                  end if
                else if( orderOfAccuracy.eq.4 )then
                  if( nd.eq.2 )then
                     ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                     kThermalLES = kThermal/nu
                      if( t.le.0 )then
                        write(*,'(" --- insdt:evaluate LES with 
     & variable diffusivity Temperature equation t=",e10.2,"---")') t
                      end if
                     do i3=n3a,n3b
                     do i2=n2a,n2b
                     do i1=n1a,n1b
                      if( mask(i1,i2,i3).gt.0 )then
                        ! add on gravity terms to the momentum equations:
                        ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                      ! --- Temperature Equation ---
                      ! Define the diffusion term: 
                      ! variable (turbulent) thermal diffusivity
                      ! Evaluate nuT and its derivatives: 
                        ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux42r(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy42r(i1,i2,i3,tc)+adcBoussinesq*delta4 
     & 2(tc)
                      end if
                     end do
                     end do
                     end do
                  else
                     ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                     kThermalLES = kThermal/nu
                      if( t.le.0 )then
                        write(*,'(" --- insdt:evaluate LES with 
     & variable diffusivity Temperature equation t=",e10.2,"---")') t
                      end if
                     do i3=n3a,n3b
                     do i2=n2a,n2b
                     do i1=n1a,n1b
                      if( mask(i1,i2,i3).gt.0 )then
                        ! add on gravity terms to the momentum equations:
                        ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)-gravity(2)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                      ! --- Temperature Equation ---
                      ! Define the diffusion term: 
                      ! variable (turbulent) thermal diffusivity
                      ! Evaluate nuT and its derivatives: 
                        ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux43r(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy43r(i1,i2,i3,tc)-uu(i1,i2,i3,wc)*
     & uz43r(i1,i2,i3,tc)+adcBoussinesq*delta4 3(tc)
                      end if
                     end do
                     end do
                     end do
                  end if
                else
                 stop 2281
                end if
               else if( gridType.eq.curvilinear )then
                if( orderOfAccuracy.eq.2 )then
                  if( nd.eq.2 )then
                     ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                     kThermalLES = kThermal/nu
                      if( t.le.0 )then
                        write(*,'(" --- insdt:evaluate LES with 
     & variable diffusivity Temperature equation t=",e10.2,"---")') t
                      end if
                     do i3=n3a,n3b
                     do i2=n2a,n2b
                     do i1=n1a,n1b
                      if( mask(i1,i2,i3).gt.0 )then
                        ! add on gravity terms to the momentum equations:
                        ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                      ! --- Temperature Equation ---
                      ! Define the diffusion term: 
                      ! variable (turbulent) thermal diffusivity
                      ! Evaluate nuT and its derivatives: 
                        ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux22(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy22(i1,i2,i3,tc)+adcBoussinesq*delta2 
     & 2(tc)
                      end if
                     end do
                     end do
                     end do
                  else
                     ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                     kThermalLES = kThermal/nu
                      if( t.le.0 )then
                        write(*,'(" --- insdt:evaluate LES with 
     & variable diffusivity Temperature equation t=",e10.2,"---")') t
                      end if
                     do i3=n3a,n3b
                     do i2=n2a,n2b
                     do i1=n1a,n1b
                      if( mask(i1,i2,i3).gt.0 )then
                        ! add on gravity terms to the momentum equations:
                        ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)-gravity(2)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                      ! --- Temperature Equation ---
                      ! Define the diffusion term: 
                      ! variable (turbulent) thermal diffusivity
                      ! Evaluate nuT and its derivatives: 
                        ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy23(i1,i2,i3,tc)-uu(i1,i2,i3,wc)*uz23(
     & i1,i2,i3,tc)+adcBoussinesq*delta2 3(tc)
                      end if
                     end do
                     end do
                     end do
                  end if
                else if( orderOfAccuracy.eq.4 )then
                  if( nd.eq.2 )then
                     ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                     kThermalLES = kThermal/nu
                      if( t.le.0 )then
                        write(*,'(" --- insdt:evaluate LES with 
     & variable diffusivity Temperature equation t=",e10.2,"---")') t
                      end if
                     do i3=n3a,n3b
                     do i2=n2a,n2b
                     do i1=n1a,n1b
                      if( mask(i1,i2,i3).gt.0 )then
                        ! add on gravity terms to the momentum equations:
                        ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                      ! --- Temperature Equation ---
                      ! Define the diffusion term: 
                      ! variable (turbulent) thermal diffusivity
                      ! Evaluate nuT and its derivatives: 
                        ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux42(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy42(i1,i2,i3,tc)+adcBoussinesq*delta4 
     & 2(tc)
                      end if
                     end do
                     end do
                     end do
                  else
                     ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
                     kThermalLES = kThermal/nu
                      if( t.le.0 )then
                        write(*,'(" --- insdt:evaluate LES with 
     & variable diffusivity Temperature equation t=",e10.2,"---")') t
                      end if
                     do i3=n3a,n3b
                     do i2=n2a,n2b
                     do i1=n1a,n1b
                      if( mask(i1,i2,i3).gt.0 )then
                        ! add on gravity terms to the momentum equations:
                        ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                        ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                          ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)-gravity(2)*
     & thermalExpansivity*u(i1,i2,i3,tc)
                      ! --- Temperature Equation ---
                      ! Define the diffusion term: 
                      ! variable (turbulent) thermal diffusivity
                      ! Evaluate nuT and its derivatives: 
                        ut(i1,i2,i3,tc)= -uu(i1,i2,i3,uc)*ux43(i1,i2,
     & i3,tc)-uu(i1,i2,i3,vc)*uy43(i1,i2,i3,tc)-uu(i1,i2,i3,wc)*uz43(
     & i1,i2,i3,tc)+adcBoussinesq*delta4 3(tc)
                      end if
                     end do
                     end do
                     end do
                  end if
                else
                 stop 2282
                end if
               else
                 stop 2717
               end if
            else
             write(*,*)'insdt:boussinesq Unknown implicitOption=',
     & implicitOption
             stop 4135
            end if  ! end implicitOption
           end if
        else
          write(*,'("insdt: Solving T equation : Unknown turbulence 
     & model=",i6)') turbulenceModel
          stop 4005
        end if
      end if

!     **********************************
!     ****** artificial diffusion ******  
!     **********************************

      if( use2ndOrderAD.eq.1 .or. use4thOrderAD.eq.1 )then
       if( nd.eq.2 )then
        ! -- 2D --
        if( gridType.eq.rectangular )then
          call insad2dr(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,
     & nd3a,nd3b,nd4a,nd4b,mask,xy,rsxy,radiusInverse,  u,uu, ut,uti,
     & gv,dw,  ndMatProp,matIndex,matValpc,matVal, bc, ipar, rpar, 
     & ierr )
        else if( gridType.eq.curvilinear )then
          call insad2dc(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,
     & nd3a,nd3b,nd4a,nd4b,mask,xy,rsxy,radiusInverse,  u,uu, ut,uti,
     & gv,dw,  ndMatProp,matIndex,matValpc,matVal, bc, ipar, rpar, 
     & ierr )
        else
          stop 77
        end if
       else
        ! -- 3D --
        if( gridType.eq.rectangular )then
          call insad3dr(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,
     & nd3a,nd3b,nd4a,nd4b,mask,xy,rsxy,radiusInverse,  u,uu, ut,uti,
     & gv,dw,  ndMatProp,matIndex,matValpc,matVal, bc, ipar, rpar, 
     & ierr )
        else if( gridType.eq.curvilinear )then
          call insad3dc(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,
     & nd3a,nd3b,nd4a,nd4b,mask,xy,rsxy,radiusInverse,  u,uu, ut,uti,
     & gv,dw,  ndMatProp,matIndex,matValpc,matVal, bc, ipar, rpar, 
     & ierr )
        else
          stop 77
        end if
       end if
      end if
      return
      end




! ..................................................................................


      subroutine insArtificialDiffusion(nd,
     & n1a,n1b,n2a,n2b,n3a,n3b,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,
     & mask,rsxy,  u,v,  ipar, rpar, ierr )
!======================================================================
!   Add on the artificial diffusion in a semi-implicit way
!
!  Approximately add on: 
!       v = u + dt*AD( v )
! by iterating
!       v(0) = u
!       for k=0,1,..
!         v(k+1) = u + dt*AD( v(k) )   --- but do this implicitly
!
!======================================================================
      implicit none
      integer nd, n1a,n1b,n2a,n2b,n3a,n3b,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b

      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real v(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer ierr

      integer ipar(0:*)
      real rpar(0:*)

!     ---- local variables -----
      integer c,i1,i2,i3,orderOfAccuracy,gridType,useWhereMask,
     & numberOfIterations
      integer gridIsImplicit,use2ndOrderAD,use4thOrderAD,use6thOrderAD
      integer pc,uc,vc,wc,sc,grid,m,nc,advectPassiveScalar
      real dt,dr(0:2),dx(0:2),adcPassiveScalar
      real ad21,ad22,ad41,ad42,ad61,ad62,cd22,cd42,cd62,adc
      real ad21n,ad22n,ad41n,ad42n,cd22n,cd42n

      integer rectangular,curvilinear
      parameter( rectangular=0, curvilinear=1 )

      integer computeAllTerms,
     &     doNotComputeImplicitTerms,
     &     computeImplicitTermsSeparately,
     &     computeAllWithWeightedImplicit

      parameter( computeAllTerms=0,
     &           doNotComputeImplicitTerms=1,
     &           computeImplicitTermsSeparately=2,
     &           computeAllWithWeightedImplicit=3 )

      ! declare variables for difference approximations
      ! include 'declareDiffOrder2f.h'

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


!      real cdmz,cdpz,cdzm,cdzp,cdmzz,cdpzz,cdzmz,cdzpz,cdzzm,cdzzp,cdDiag,cdm,cdp
!      real uxmzzR,uymzzR,uzmzzR,uxzmzR,uyzmzR,uzzmzR,uxzzmR,uyzzmR,uzzzmR
!      real udmzC,udzmC,udmzzC,udzmzC,udzzmC
!      real admzR,adzmR,admzzR,adzmzR,adzzmR
!      real admzC,adzmC,admzzC,adzmzC,adzzmC
!     real admzRSA,adzmRSA,admzzRSA,adzmzRSA,adzzmRSA
!     real admzCSA,adzmCSA,admzzCSA,adzmzCSA,adzzmCSA
!     real adE0,adE1,ade2,adE3d0,adE3d1,adE3d2,ad2f,ad3f
!     real dr2i,ds2i,dt2i,dri,dsi,dti

!     --- begin statement functions
      real ad2Coeff,ad2,ad23Coeff,ad23,ad4Coeff,ad4,ad43Coeff,ad43
      real ad2rCoeff,ad23rCoeff,ad4rCoeff,ad43rCoeff
      real ad2nd,ad23nd,ad4nd,ad43nd

!      real adSelfAdjoint2dR,adSelfAdjoint3dR,adSelfAdjoint2dC,adSelfAdjoint3dC
!      real adSelfAdjoint2dRSA,adSelfAdjoint3dRSA,adSelfAdjoint2dCSA,adSelfAdjoint3dCSA
!***  include 'insDeriv.h'

! .............. begin statement functions
      integer kd
      real rx,ry,rz,sx,sy,sz,tx,ty,tz

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

!    --- 2nd order 2D artificial diffusion ---
      ad2Coeff()=(ad21 + cd22*
     &    ( abs(ux22(i1,i2,i3,uc))+abs(uy22(i1,i2,i3,uc))
     &     +abs(ux22(i1,i2,i3,vc))+abs(uy22(i1,i2,i3,vc)) ) )
      ad2rCoeff()=(ad21 + cd22*
     &    ( abs(ux22r(i1,i2,i3,uc))+abs(uy22r(i1,i2,i3,uc))
     &     +abs(ux22r(i1,i2,i3,vc))+abs(uy22r(i1,i2,i3,vc)) ) )
      ad2(c)=adc*(u(i1+1,i2,i3,c)-4.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c)
     &           +u(i1,i2+1,i3,c)                 +u(i1,i2-1,i3,c))
      ad2nd(c)=adc*(u(i1+1,i2,i3,c)                 +u(i1-1,i2,i3,c)  ! no diagonal term
     &             +u(i1,i2+1,i3,c)                 +u(i1,i2-1,i3,c))

!    --- 2nd order 3D artificial diffusion ---
      ad23Coeff()=(ad21 + cd22*
     & ( abs(ux23(i1,i2,i3,uc))+abs(uy23(i1,i2,i3,uc))+abs(uz23(i1,i2,
     & i3,uc))
     & +abs(ux23(i1,i2,i3,vc))+abs(uy23(i1,i2,i3,vc))+abs(uz23(i1,i2,
     & i3,vc))
     & +abs(ux23(i1,i2,i3,wc))+abs(uy23(i1,i2,i3,wc))+abs(uz23(i1,i2,
     & i3,wc)) ) )
      ad23rCoeff()=(ad21 + cd22*
     & ( abs(ux23r(i1,i2,i3,uc))+abs(uy23r(i1,i2,i3,uc))+abs(uz23r(i1,
     & i2,i3,uc))
     & +abs(ux23r(i1,i2,i3,vc))+abs(uy23r(i1,i2,i3,vc))+abs(uz23r(i1,
     & i2,i3,vc))
     & +abs(ux23r(i1,i2,i3,wc))+abs(uy23r(i1,i2,i3,wc))+abs(uz23r(i1,
     & i2,i3,wc)) ) )
      ad23(c)=adc
     &    *(u(i1+1,i2,i3,c)-6.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c)
     &     +u(i1,i2+1,i3,c)                   +u(i1,i2-1,i3,c)
     &     +u(i1,i2,i3+1,c)                   +u(i1,i2,i3-1,c))
      ad23nd(c)=adc
     &    *(u(i1+1,i2,i3,c)                   +u(i1-1,i2,i3,c)
     &     +u(i1,i2+1,i3,c)                   +u(i1,i2-1,i3,c)
     &     +u(i1,i2,i3+1,c)                   +u(i1,i2,i3-1,c))

!     ---fourth-order artificial diffusion in 2D
      ad4Coeff()=(ad41 + cd42*
     &    ( abs(ux22(i1,i2,i3,uc))+abs(uy22(i1,i2,i3,uc))
     &     +abs(ux22(i1,i2,i3,vc))+abs(uy22(i1,i2,i3,vc)) ) )
      ad4rCoeff()=(ad41 + cd42*
     &    ( abs(ux22r(i1,i2,i3,uc))+abs(uy22r(i1,i2,i3,uc))
     &     +abs(ux22r(i1,i2,i3,vc))+abs(uy22r(i1,i2,i3,vc)) ) )
      ad4(c)=adc
     &    *(   -u(i1+2,i2,i3,c)-u(i1-2,i2,i3,c)
     &         -u(i1,i2+2,i3,c)-u(i1,i2-2,i3,c)
     &     +4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)
     &         +u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c))
     &      -12.*u(i1,i2,i3,c) )
      ad4nd(c)=adc
     &    *(   -u(i1+2,i2,i3,c)-u(i1-2,i2,i3,c)
     &         -u(i1,i2+2,i3,c)-u(i1,i2-2,i3,c)
     &     +4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)
     &         +u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c))
     &                         )
!     ---fourth-order artificial diffusion in 3D
      ad43Coeff()=
     &   (ad41 + cd42*
     & ( abs(ux23(i1,i2,i3,uc))+abs(uy23(i1,i2,i3,uc))+abs(uz23(i1,i2,
     & i3,uc))
     & +abs(ux23(i1,i2,i3,vc))+abs(uy23(i1,i2,i3,vc))+abs(uz23(i1,i2,
     & i3,vc))
     & +abs(ux23(i1,i2,i3,wc))+abs(uy23(i1,i2,i3,wc))+abs(uz23(i1,i2,
     & i3,wc)) ) )
      ad43rCoeff()=
     &   (ad41 + cd42*
     & ( abs(ux23r(i1,i2,i3,uc))+abs(uy23r(i1,i2,i3,uc))+abs(uz23r(i1,
     & i2,i3,uc))
     & +abs(ux23r(i1,i2,i3,vc))+abs(uy23r(i1,i2,i3,vc))+abs(uz23r(i1,
     & i2,i3,vc))
     & +abs(ux23r(i1,i2,i3,wc))+abs(uy23r(i1,i2,i3,wc))+abs(uz23r(i1,
     & i2,i3,wc)) ) )
      ad43(c)=adc
     &    *(   -u(i1+2,i2,i3,c)-u(i1-2,i2,i3,c)
     &         -u(i1,i2+2,i3,c)-u(i1,i2-2,i3,c)
     &         -u(i1,i2,i3+2,c)-u(i1,i2,i3-2,c)
     &     +4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)
     &         +u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c)
     &         +u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c))
     &      -18.*u(i1,i2,i3,c) )
      ad43nd(c)=adc
     &    *(   -u(i1+2,i2,i3,c)-u(i1-2,i2,i3,c)
     &         -u(i1,i2+2,i3,c)-u(i1,i2-2,i3,c)
     &         -u(i1,i2,i3+2,c)-u(i1,i2,i3-2,c)
     &     +4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)
     &         +u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c)
     &         +u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c))
     &                         )

!**** Include "selfAdjointArtificialDiffusion.h"

!     --- end statement functions

      ierr=0
      ! write(*,*) 'Inside insdt'

      pc                 =ipar(0)
      uc                 =ipar(1)
      vc                 =ipar(2)
      wc                 =ipar(3)
      sc                 =ipar(4)
      grid               =ipar(5)
      orderOfAccuracy    =ipar(6)
      gridType           =ipar(7)
      useWhereMask       =ipar(8)
      gridIsImplicit     =ipar(9)
      use2ndOrderAD      =ipar(10)
      use4thOrderAD      =ipar(11)
      use6thOrderAD      =ipar(12)
      advectPassiveScalar=ipar(13)
      numberOfIterations =ipar(14)

      dx(0)             =rpar(0)
      dx(1)             =rpar(1)
      dx(2)             =rpar(2)
      dr(0)             =rpar(3)
      dr(1)             =rpar(4)
      dr(2)             =rpar(5)
      ad21              =rpar(6)
      ad22              =rpar(7)
      ad41              =rpar(8)
      ad42              =rpar(9)
      ad61              =rpar(10)
      ad62              =rpar(11)
      adcPassiveScalar  =rpar(12)

      dt                = rpar(11) ! ************* add this **************

!$$$      dxi=1./dx
!$$$      dyi=1./dy
!$$$      dzi=1./dz
!$$$      dx2i=1./(2.*dx)
!$$$      dy2i=1./(2.*dy)
!$$$      dz2i=1./(2.*dz)
!$$$      dxsqi=1./(dx*dx)
!$$$      dysqi=1./(dy*dy)
!$$$      dzsqi=1./(dz*dz)
!$$$
!$$$      if( orderOfAccuracy.eq.4 )then
!$$$        dx12i=1./(12.*dx)
!$$$        dy12i=1./(12.*dy)
!$$$        dz12i=1./(12.*dz)
!$$$        dxsq12i=1./(12.*dx**2)
!$$$        dysq12i=1./(12.*dy**2)
!$$$        dzsq12i=1./(12.*dz**2)
!$$$      end if


!     **********************************
!     ****** artificial diffusion ******  
!     **********************************


      cd22n=ad22n/nd
      cd42n=ad42n/nd

      if( use2ndOrderAD.eq.1 .and.
     &     (ad21.gt.0. .or. ad22.gt.0.) ) then

!      *******************************************
!      ****** 2nd-order artificial diffusion *****
!      *******************************************

       cd22=ad22/(nd**2)
       if( nd.eq.1 )then
         stop 1
       else if( nd.eq.2 )then

!       non-self-adjoint form:
!       loopse4(adc=ad2Coeff(), !               ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)+ad2(uc),!               ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)+ad2(vc),)

!        self-adjoint form:
!        loopse4($defineArtificialDiffusionCoefficients(2,R,), !                ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)+ad2f(i1,i2,i3,uc),!                ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)+ad2f(i1,i2,i3,vc),)

         ! -- Here is a aprtially implicit version ---
         if( gridType.eq.rectangular )then
          if( useWhereMask.ne.0 )then
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
              if( mask(i1,i2,i3).gt.0 )then
                adc=dt*ad2rCoeff()
                v(i1,i2,i3,uc)=(u(i1,i2,i3,uc)+ad2nd(uc))/(1.+4.*adc)
                v(i1,i2,i3,vc)=(u(i1,i2,i3,vc)+ad2nd(vc))/(1.+4.*adc)

              end if
            end do
            end do
            end do
          else
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
                adc=dt*ad2rCoeff()
                v(i1,i2,i3,uc)=(u(i1,i2,i3,uc)+ad2nd(uc))/(1.+4.*adc)
                v(i1,i2,i3,vc)=(u(i1,i2,i3,vc)+ad2nd(vc))/(1.+4.*adc)

            end do
            end do
            end do
          end if
         else
          if( useWhereMask.ne.0 )then
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
              if( mask(i1,i2,i3).gt.0 )then
                adc=dt*ad2Coeff()
                v(i1,i2,i3,uc)=(u(i1,i2,i3,uc)+ad2nd(uc))/(1.+4.*adc)
                v(i1,i2,i3,vc)=(u(i1,i2,i3,vc)+ad2nd(vc))/(1.+4.*adc)

              end if
            end do
            end do
            end do
          else
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
                adc=dt*ad2Coeff()
                v(i1,i2,i3,uc)=(u(i1,i2,i3,uc)+ad2nd(uc))/(1.+4.*adc)
                v(i1,i2,i3,vc)=(u(i1,i2,i3,vc)+ad2nd(vc))/(1.+4.*adc)

            end do
            end do
            end do
          end if
         end if

       else ! 3D
!        loopse4(adc=ad23Coeff(), !                ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)+ad23(uc),!                ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)+ad23(vc),!                ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)+ad23(wc) )

!        loopse4($defineArtificialDiffusionCoefficients(3,R,), !                ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)+ad3f(i1,i2,i3,uc),!                ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)+ad3f(i1,i2,i3,vc),!                ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)+ad3f(i1,i2,i3,wc) )

         stop 2
       end if

      end if

      if( use4thOrderAD.eq.1 .and.
     &     (ad41.gt.0. .or. ad42.gt.0.) ) then

       cd42=ad42/(nd**2)
       if( nd.eq.1 )then
         stop 1
       else if( nd.eq.2 )then

         if( gridType.eq.rectangular )then
          if( useWhereMask.ne.0 )then
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
              if( mask(i1,i2,i3).gt.0 )then
                adc=dt*ad4rCoeff()
                v(i1,i2,i3,uc)=(u(i1,i2,i3,uc)+ad4nd(uc))/(1.+12.*adc)
                v(i1,i2,i3,vc)=(u(i1,i2,i3,vc)+ad4nd(vc))/(1.+12.*adc)

              end if
            end do
            end do
            end do
          else
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
                adc=dt*ad4rCoeff()
                v(i1,i2,i3,uc)=(u(i1,i2,i3,uc)+ad4nd(uc))/(1.+12.*adc)
                v(i1,i2,i3,vc)=(u(i1,i2,i3,vc)+ad4nd(vc))/(1.+12.*adc)

            end do
            end do
            end do
          end if
         else
          if( useWhereMask.ne.0 )then
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
              if( mask(i1,i2,i3).gt.0 )then
                adc=dt*ad4Coeff()
                v(i1,i2,i3,uc)=(u(i1,i2,i3,uc)+ad4nd(uc))/(1.+12.*adc)
                v(i1,i2,i3,vc)=(u(i1,i2,i3,vc)+ad4nd(vc))/(1.+12.*adc)

              end if
            end do
            end do
            end do
          else
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
                adc=dt*ad4Coeff()
                v(i1,i2,i3,uc)=(u(i1,i2,i3,uc)+ad4nd(uc))/(1.+12.*adc)
                v(i1,i2,i3,vc)=(u(i1,i2,i3,vc)+ad4nd(vc))/(1.+12.*adc)

            end do
            end do
            end do
          end if
         end if

       else ! 3D
         stop 2
!        loopse4(adc=ad43Coeff(), !                ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)+ad43(uc),!                ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)+ad43(vc),!                ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)+ad43(wc) )

       end if

      end if

!     *************************************************
!     *********Advect a passive scalar ****************
!     *************************************************
      if( advectPassiveScalar.eq.1 )then
       adc=adcPassiveScalar ! coefficient of linear artificial diffusion
!       if( nd.eq.1 )then
!         if( orderOfAccuracy.eq.2 )then
!           loopse4(ut(i1,i2,i3,sc)= -uu(uc)*ux2(sc)+nuPassiveScalar*uxx2(sc),,,)
!         else
!           loopse4(ut(i1,i2,i3,sc)= -uu(uc)*ux4(sc)+nuPassiveScalar*uxx4(sc),,,)
!         end if
!
!       else if( nd.eq.2 )then
!         if( orderOfAccuracy.eq.2 )then
!          loopse4(ut(i1,i2,i3,sc)= -uu(uc)*ux2(sc)-uu(vc)*uy2(sc)+nuPassiveScalar*lap2d2(sc)+ad2(sc),,,)
!         else ! order==4
!          loopse4(ut(i1,i2,i3,sc)= -uu(uc)*ux4(sc)-uu(vc)*uy4(sc)+nuPassiveScalar*lap2d4(sc)+ad4(sc),,,)
!         end if
!       else ! nd==3
!        if( orderOfAccuracy.eq.2 )then
!           loopse4(ut(i1,i2,i3,sc)= -uu(uc)*ux2(sc)-uu(vc)*uy2(sc)-uu(wc)*uz2(sc)!                                    +nuPassiveScalar*lap3d2(sc)+ad23(sc),,,)
!        else
!           loopse4(ut(i1,i2,i3,sc)= -uu(uc)*ux4(sc)-uu(vc)*uy4(sc)-uu(wc)*uz4(sc)!                                    +nuPassiveScalar*lap3d4(sc)+ad43(sc),,,)
!        end if
!       end if ! end nd

      end if ! advectPassiveScalar


      return
      end
