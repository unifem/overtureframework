! This file automatically generated from insdtINS.bf with bpp.
         subroutine insdtINS3dOrder2(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,
     & nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,xy,rsxy,radiusInverse, 
     &  u,uu, ut,uti,gv,dw,  bc, ipar, rpar, ierr )
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
         integer nd, n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,
     & nd3b,nd4a,nd4b
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
         !     ---- local variables -----
         integer c,i1,i2,i3,kd,kd3,orderOfAccuracy,gridIsMoving,
     & useWhereMask
         integer gridIsImplicit,implicitOption,implicitMethod,
     & isAxisymmetric,use2ndOrderAD,use4thOrderAD
         integer rc,pc,uc,vc,wc,sc,nc,kc,ec,tc,grid,m,
     & advectPassiveScalar,vsc
         real nu,dt,nuPassiveScalar,adcPassiveScalar
         real dxi,dyi,dzi,dri,dsi,dti,dr2i,ds2i,dt2i
         real ad21,ad22,ad41,ad42,cd22,cd42,adc
         real ad21n,ad22n,ad41n,ad42n,cd22n,cd42n
         real yy,ri
         integer materialFormat
         real t
         integer gridType
         integer rectangular,curvilinear
         parameter( rectangular=0, curvilinear=1 )
         integer turbulenceModel,noTurbulenceModel
         integer baldwinLomax,spalartAllmaras,kEpsilon,kOmega,
     & largeEddySimulation
         parameter (noTurbulenceModel=0,baldwinLomax=1,kEpsilon=2,
     & kOmega=3,spalartAllmaras=4,largeEddySimulation=5 )
         integer pdeModel,standardModel,BoussinesqModel,
     & viscoPlasticModel,twoPhaseFlowModel
         parameter( standardModel=0,BoussinesqModel=1,
     & viscoPlasticModel=2,twoPhaseFlowModel=3 )
         integer upwindOrder,debug
         integer advectionOption, centeredAdvection,upwindAdvection,
     & bwenoAdvection
         parameter( centeredAdvection=0, upwindAdvection=1, 
     & bwenoAdvection=2 )
         real au,agu(0:5,0:5) ! for holdings upwind approximations to (a.grad)u
         integer computeAllTerms,doNotComputeImplicitTerms,
     & computeImplicitTermsSeparately,computeAllWithWeightedImplicit
         parameter( computeAllTerms=0,doNotComputeImplicitTerms=1,
     & computeImplicitTermsSeparately=2,
     & computeAllWithWeightedImplicit=3 )
         real rx,ry,rz,sx,sy,sz,tx,ty,tz
         real dr(0:2), dx(0:2)
         ! for SPAL TM
         real n0,n0x,n0y,n0z
         real cb1, cb2, cv1, sigma, sigmai, kappa, cw1, cw2, cw3, 
     & cw3e6, cv1e3, cd0, cr0
         real chi,chi3,fnu1,fnu2,s,r,g,fw,dKappaSq,nSqBydSq,dd
         real nuT,nuTx,nuTy,nuTz,nuTd
         real u0,u0x,u0y,u0z
         real v0,v0x,v0y,v0z
         real w0,w0x,w0y,w0z
         ! for k-epsilon
         real k0,k0x,k0y,k0z, e0,e0x,e0y,e0z
         real nuP,prod
         real cMu,cEps1,cEps2,sigmaEpsI,sigmaKI
         ! for visco-plastic
         ! real nuVP,etaVP,yieldStressVP,exponentVP,epsVP
         ! real eDotNorm,exp0
         ! real u0xx,u0xy,u0xz,u0yy,u0yz,u0zz
         ! real v0xx,v0xy,v0xz,v0yy,v0yz,v0zz
         ! real w0xx,w0xy,w0xz,w0yy,w0yz,w0zz
         real delta22,delta23,delta42,delta43
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
         !  --- begin statement functions
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
         urr2(i1,i2,i3,kd)=(-2.*u(i1,i2,i3,kd)+(u(i1+1,i2,i3,kd)+u(i1-
     & 1,i2,i3,kd)) )*d22(0)
         uss2(i1,i2,i3,kd)=(-2.*u(i1,i2,i3,kd)+(u(i1,i2+1,i3,kd)+u(i1,
     & i2-1,i3,kd)) )*d22(1)
         urs2(i1,i2,i3,kd)=(ur2(i1,i2+1,i3,kd)-ur2(i1,i2-1,i3,kd))*d12(
     & 1)
         utt2(i1,i2,i3,kd)=(-2.*u(i1,i2,i3,kd)+(u(i1,i2,i3+1,kd)+u(i1,
     & i2,i3-1,kd)) )*d22(2)
         urt2(i1,i2,i3,kd)=(ur2(i1,i2,i3+1,kd)-ur2(i1,i2,i3-1,kd))*d12(
     & 2)
         ust2(i1,i2,i3,kd)=(us2(i1,i2,i3+1,kd)-us2(i1,i2,i3-1,kd))*d12(
     & 2)
         urrr2(i1,i2,i3,kd)=(-2.*(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))+(
     & u(i1+2,i2,i3,kd)-u(i1-2,i2,i3,kd)) )*d22(0)*d12(0)
         usss2(i1,i2,i3,kd)=(-2.*(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))+(
     & u(i1,i2+2,i3,kd)-u(i1,i2-2,i3,kd)) )*d22(1)*d12(1)
         uttt2(i1,i2,i3,kd)=(-2.*(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))+(
     & u(i1,i2,i3+2,kd)-u(i1,i2,i3-2,kd)) )*d22(2)*d12(2)
         rxr2(i1,i2,i3)=(rx(i1+1,i2,i3)-rx(i1-1,i2,i3))*d12(0)
         rxs2(i1,i2,i3)=(rx(i1,i2+1,i3)-rx(i1,i2-1,i3))*d12(1)
         rxt2(i1,i2,i3)=(rx(i1,i2,i3+1)-rx(i1,i2,i3-1))*d12(2)
         rxrr2(i1,i2,i3)=(-2.*rx(i1,i2,i3)+(rx(i1+1,i2,i3)+rx(i1-1,i2,
     & i3)) )*d22(0)
         rxss2(i1,i2,i3)=(-2.*rx(i1,i2,i3)+(rx(i1,i2+1,i3)+rx(i1,i2-1,
     & i3)) )*d22(1)
         rxrs2(i1,i2,i3)=(rxr2(i1,i2+1,i3)-rxr2(i1,i2-1,i3))*d12(1)
         ryr2(i1,i2,i3)=(ry(i1+1,i2,i3)-ry(i1-1,i2,i3))*d12(0)
         rys2(i1,i2,i3)=(ry(i1,i2+1,i3)-ry(i1,i2-1,i3))*d12(1)
         ryt2(i1,i2,i3)=(ry(i1,i2,i3+1)-ry(i1,i2,i3-1))*d12(2)
         ryrr2(i1,i2,i3)=(-2.*ry(i1,i2,i3)+(ry(i1+1,i2,i3)+ry(i1-1,i2,
     & i3)) )*d22(0)
         ryss2(i1,i2,i3)=(-2.*ry(i1,i2,i3)+(ry(i1,i2+1,i3)+ry(i1,i2-1,
     & i3)) )*d22(1)
         ryrs2(i1,i2,i3)=(ryr2(i1,i2+1,i3)-ryr2(i1,i2-1,i3))*d12(1)
         rzr2(i1,i2,i3)=(rz(i1+1,i2,i3)-rz(i1-1,i2,i3))*d12(0)
         rzs2(i1,i2,i3)=(rz(i1,i2+1,i3)-rz(i1,i2-1,i3))*d12(1)
         rzt2(i1,i2,i3)=(rz(i1,i2,i3+1)-rz(i1,i2,i3-1))*d12(2)
         rzrr2(i1,i2,i3)=(-2.*rz(i1,i2,i3)+(rz(i1+1,i2,i3)+rz(i1-1,i2,
     & i3)) )*d22(0)
         rzss2(i1,i2,i3)=(-2.*rz(i1,i2,i3)+(rz(i1,i2+1,i3)+rz(i1,i2-1,
     & i3)) )*d22(1)
         rzrs2(i1,i2,i3)=(rzr2(i1,i2+1,i3)-rzr2(i1,i2-1,i3))*d12(1)
         sxr2(i1,i2,i3)=(sx(i1+1,i2,i3)-sx(i1-1,i2,i3))*d12(0)
         sxs2(i1,i2,i3)=(sx(i1,i2+1,i3)-sx(i1,i2-1,i3))*d12(1)
         sxt2(i1,i2,i3)=(sx(i1,i2,i3+1)-sx(i1,i2,i3-1))*d12(2)
         sxrr2(i1,i2,i3)=(-2.*sx(i1,i2,i3)+(sx(i1+1,i2,i3)+sx(i1-1,i2,
     & i3)) )*d22(0)
         sxss2(i1,i2,i3)=(-2.*sx(i1,i2,i3)+(sx(i1,i2+1,i3)+sx(i1,i2-1,
     & i3)) )*d22(1)
         sxrs2(i1,i2,i3)=(sxr2(i1,i2+1,i3)-sxr2(i1,i2-1,i3))*d12(1)
         syr2(i1,i2,i3)=(sy(i1+1,i2,i3)-sy(i1-1,i2,i3))*d12(0)
         sys2(i1,i2,i3)=(sy(i1,i2+1,i3)-sy(i1,i2-1,i3))*d12(1)
         syt2(i1,i2,i3)=(sy(i1,i2,i3+1)-sy(i1,i2,i3-1))*d12(2)
         syrr2(i1,i2,i3)=(-2.*sy(i1,i2,i3)+(sy(i1+1,i2,i3)+sy(i1-1,i2,
     & i3)) )*d22(0)
         syss2(i1,i2,i3)=(-2.*sy(i1,i2,i3)+(sy(i1,i2+1,i3)+sy(i1,i2-1,
     & i3)) )*d22(1)
         syrs2(i1,i2,i3)=(syr2(i1,i2+1,i3)-syr2(i1,i2-1,i3))*d12(1)
         szr2(i1,i2,i3)=(sz(i1+1,i2,i3)-sz(i1-1,i2,i3))*d12(0)
         szs2(i1,i2,i3)=(sz(i1,i2+1,i3)-sz(i1,i2-1,i3))*d12(1)
         szt2(i1,i2,i3)=(sz(i1,i2,i3+1)-sz(i1,i2,i3-1))*d12(2)
         szrr2(i1,i2,i3)=(-2.*sz(i1,i2,i3)+(sz(i1+1,i2,i3)+sz(i1-1,i2,
     & i3)) )*d22(0)
         szss2(i1,i2,i3)=(-2.*sz(i1,i2,i3)+(sz(i1,i2+1,i3)+sz(i1,i2-1,
     & i3)) )*d22(1)
         szrs2(i1,i2,i3)=(szr2(i1,i2+1,i3)-szr2(i1,i2-1,i3))*d12(1)
         txr2(i1,i2,i3)=(tx(i1+1,i2,i3)-tx(i1-1,i2,i3))*d12(0)
         txs2(i1,i2,i3)=(tx(i1,i2+1,i3)-tx(i1,i2-1,i3))*d12(1)
         txt2(i1,i2,i3)=(tx(i1,i2,i3+1)-tx(i1,i2,i3-1))*d12(2)
         txrr2(i1,i2,i3)=(-2.*tx(i1,i2,i3)+(tx(i1+1,i2,i3)+tx(i1-1,i2,
     & i3)) )*d22(0)
         txss2(i1,i2,i3)=(-2.*tx(i1,i2,i3)+(tx(i1,i2+1,i3)+tx(i1,i2-1,
     & i3)) )*d22(1)
         txrs2(i1,i2,i3)=(txr2(i1,i2+1,i3)-txr2(i1,i2-1,i3))*d12(1)
         tyr2(i1,i2,i3)=(ty(i1+1,i2,i3)-ty(i1-1,i2,i3))*d12(0)
         tys2(i1,i2,i3)=(ty(i1,i2+1,i3)-ty(i1,i2-1,i3))*d12(1)
         tyt2(i1,i2,i3)=(ty(i1,i2,i3+1)-ty(i1,i2,i3-1))*d12(2)
         tyrr2(i1,i2,i3)=(-2.*ty(i1,i2,i3)+(ty(i1+1,i2,i3)+ty(i1-1,i2,
     & i3)) )*d22(0)
         tyss2(i1,i2,i3)=(-2.*ty(i1,i2,i3)+(ty(i1,i2+1,i3)+ty(i1,i2-1,
     & i3)) )*d22(1)
         tyrs2(i1,i2,i3)=(tyr2(i1,i2+1,i3)-tyr2(i1,i2-1,i3))*d12(1)
         tzr2(i1,i2,i3)=(tz(i1+1,i2,i3)-tz(i1-1,i2,i3))*d12(0)
         tzs2(i1,i2,i3)=(tz(i1,i2+1,i3)-tz(i1,i2-1,i3))*d12(1)
         tzt2(i1,i2,i3)=(tz(i1,i2,i3+1)-tz(i1,i2,i3-1))*d12(2)
         tzrr2(i1,i2,i3)=(-2.*tz(i1,i2,i3)+(tz(i1+1,i2,i3)+tz(i1-1,i2,
     & i3)) )*d22(0)
         tzss2(i1,i2,i3)=(-2.*tz(i1,i2,i3)+(tz(i1,i2+1,i3)+tz(i1,i2-1,
     & i3)) )*d22(1)
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
         rxx22(i1,i2,i3)= rx(i1,i2,i3)*rxr2(i1,i2,i3)+sx(i1,i2,i3)*
     & rxs2(i1,i2,i3)
         rxy22(i1,i2,i3)= ry(i1,i2,i3)*rxr2(i1,i2,i3)+sy(i1,i2,i3)*
     & rxs2(i1,i2,i3)
         rxx23(i1,i2,i3)=rx(i1,i2,i3)*rxr2(i1,i2,i3)+sx(i1,i2,i3)*rxs2(
     & i1,i2,i3)+tx(i1,i2,i3)*rxt2(i1,i2,i3)
         rxy23(i1,i2,i3)=ry(i1,i2,i3)*rxr2(i1,i2,i3)+sy(i1,i2,i3)*rxs2(
     & i1,i2,i3)+ty(i1,i2,i3)*rxt2(i1,i2,i3)
         rxz23(i1,i2,i3)=rz(i1,i2,i3)*rxr2(i1,i2,i3)+sz(i1,i2,i3)*rxs2(
     & i1,i2,i3)+tz(i1,i2,i3)*rxt2(i1,i2,i3)
         ryx22(i1,i2,i3)= rx(i1,i2,i3)*ryr2(i1,i2,i3)+sx(i1,i2,i3)*
     & rys2(i1,i2,i3)
         ryy22(i1,i2,i3)= ry(i1,i2,i3)*ryr2(i1,i2,i3)+sy(i1,i2,i3)*
     & rys2(i1,i2,i3)
         ryx23(i1,i2,i3)=rx(i1,i2,i3)*ryr2(i1,i2,i3)+sx(i1,i2,i3)*rys2(
     & i1,i2,i3)+tx(i1,i2,i3)*ryt2(i1,i2,i3)
         ryy23(i1,i2,i3)=ry(i1,i2,i3)*ryr2(i1,i2,i3)+sy(i1,i2,i3)*rys2(
     & i1,i2,i3)+ty(i1,i2,i3)*ryt2(i1,i2,i3)
         ryz23(i1,i2,i3)=rz(i1,i2,i3)*ryr2(i1,i2,i3)+sz(i1,i2,i3)*rys2(
     & i1,i2,i3)+tz(i1,i2,i3)*ryt2(i1,i2,i3)
         rzx22(i1,i2,i3)= rx(i1,i2,i3)*rzr2(i1,i2,i3)+sx(i1,i2,i3)*
     & rzs2(i1,i2,i3)
         rzy22(i1,i2,i3)= ry(i1,i2,i3)*rzr2(i1,i2,i3)+sy(i1,i2,i3)*
     & rzs2(i1,i2,i3)
         rzx23(i1,i2,i3)=rx(i1,i2,i3)*rzr2(i1,i2,i3)+sx(i1,i2,i3)*rzs2(
     & i1,i2,i3)+tx(i1,i2,i3)*rzt2(i1,i2,i3)
         rzy23(i1,i2,i3)=ry(i1,i2,i3)*rzr2(i1,i2,i3)+sy(i1,i2,i3)*rzs2(
     & i1,i2,i3)+ty(i1,i2,i3)*rzt2(i1,i2,i3)
         rzz23(i1,i2,i3)=rz(i1,i2,i3)*rzr2(i1,i2,i3)+sz(i1,i2,i3)*rzs2(
     & i1,i2,i3)+tz(i1,i2,i3)*rzt2(i1,i2,i3)
         sxx22(i1,i2,i3)= rx(i1,i2,i3)*sxr2(i1,i2,i3)+sx(i1,i2,i3)*
     & sxs2(i1,i2,i3)
         sxy22(i1,i2,i3)= ry(i1,i2,i3)*sxr2(i1,i2,i3)+sy(i1,i2,i3)*
     & sxs2(i1,i2,i3)
         sxx23(i1,i2,i3)=rx(i1,i2,i3)*sxr2(i1,i2,i3)+sx(i1,i2,i3)*sxs2(
     & i1,i2,i3)+tx(i1,i2,i3)*sxt2(i1,i2,i3)
         sxy23(i1,i2,i3)=ry(i1,i2,i3)*sxr2(i1,i2,i3)+sy(i1,i2,i3)*sxs2(
     & i1,i2,i3)+ty(i1,i2,i3)*sxt2(i1,i2,i3)
         sxz23(i1,i2,i3)=rz(i1,i2,i3)*sxr2(i1,i2,i3)+sz(i1,i2,i3)*sxs2(
     & i1,i2,i3)+tz(i1,i2,i3)*sxt2(i1,i2,i3)
         syx22(i1,i2,i3)= rx(i1,i2,i3)*syr2(i1,i2,i3)+sx(i1,i2,i3)*
     & sys2(i1,i2,i3)
         syy22(i1,i2,i3)= ry(i1,i2,i3)*syr2(i1,i2,i3)+sy(i1,i2,i3)*
     & sys2(i1,i2,i3)
         syx23(i1,i2,i3)=rx(i1,i2,i3)*syr2(i1,i2,i3)+sx(i1,i2,i3)*sys2(
     & i1,i2,i3)+tx(i1,i2,i3)*syt2(i1,i2,i3)
         syy23(i1,i2,i3)=ry(i1,i2,i3)*syr2(i1,i2,i3)+sy(i1,i2,i3)*sys2(
     & i1,i2,i3)+ty(i1,i2,i3)*syt2(i1,i2,i3)
         syz23(i1,i2,i3)=rz(i1,i2,i3)*syr2(i1,i2,i3)+sz(i1,i2,i3)*sys2(
     & i1,i2,i3)+tz(i1,i2,i3)*syt2(i1,i2,i3)
         szx22(i1,i2,i3)= rx(i1,i2,i3)*szr2(i1,i2,i3)+sx(i1,i2,i3)*
     & szs2(i1,i2,i3)
         szy22(i1,i2,i3)= ry(i1,i2,i3)*szr2(i1,i2,i3)+sy(i1,i2,i3)*
     & szs2(i1,i2,i3)
         szx23(i1,i2,i3)=rx(i1,i2,i3)*szr2(i1,i2,i3)+sx(i1,i2,i3)*szs2(
     & i1,i2,i3)+tx(i1,i2,i3)*szt2(i1,i2,i3)
         szy23(i1,i2,i3)=ry(i1,i2,i3)*szr2(i1,i2,i3)+sy(i1,i2,i3)*szs2(
     & i1,i2,i3)+ty(i1,i2,i3)*szt2(i1,i2,i3)
         szz23(i1,i2,i3)=rz(i1,i2,i3)*szr2(i1,i2,i3)+sz(i1,i2,i3)*szs2(
     & i1,i2,i3)+tz(i1,i2,i3)*szt2(i1,i2,i3)
         txx22(i1,i2,i3)= rx(i1,i2,i3)*txr2(i1,i2,i3)+sx(i1,i2,i3)*
     & txs2(i1,i2,i3)
         txy22(i1,i2,i3)= ry(i1,i2,i3)*txr2(i1,i2,i3)+sy(i1,i2,i3)*
     & txs2(i1,i2,i3)
         txx23(i1,i2,i3)=rx(i1,i2,i3)*txr2(i1,i2,i3)+sx(i1,i2,i3)*txs2(
     & i1,i2,i3)+tx(i1,i2,i3)*txt2(i1,i2,i3)
         txy23(i1,i2,i3)=ry(i1,i2,i3)*txr2(i1,i2,i3)+sy(i1,i2,i3)*txs2(
     & i1,i2,i3)+ty(i1,i2,i3)*txt2(i1,i2,i3)
         txz23(i1,i2,i3)=rz(i1,i2,i3)*txr2(i1,i2,i3)+sz(i1,i2,i3)*txs2(
     & i1,i2,i3)+tz(i1,i2,i3)*txt2(i1,i2,i3)
         tyx22(i1,i2,i3)= rx(i1,i2,i3)*tyr2(i1,i2,i3)+sx(i1,i2,i3)*
     & tys2(i1,i2,i3)
         tyy22(i1,i2,i3)= ry(i1,i2,i3)*tyr2(i1,i2,i3)+sy(i1,i2,i3)*
     & tys2(i1,i2,i3)
         tyx23(i1,i2,i3)=rx(i1,i2,i3)*tyr2(i1,i2,i3)+sx(i1,i2,i3)*tys2(
     & i1,i2,i3)+tx(i1,i2,i3)*tyt2(i1,i2,i3)
         tyy23(i1,i2,i3)=ry(i1,i2,i3)*tyr2(i1,i2,i3)+sy(i1,i2,i3)*tys2(
     & i1,i2,i3)+ty(i1,i2,i3)*tyt2(i1,i2,i3)
         tyz23(i1,i2,i3)=rz(i1,i2,i3)*tyr2(i1,i2,i3)+sz(i1,i2,i3)*tys2(
     & i1,i2,i3)+tz(i1,i2,i3)*tyt2(i1,i2,i3)
         tzx22(i1,i2,i3)= rx(i1,i2,i3)*tzr2(i1,i2,i3)+sx(i1,i2,i3)*
     & tzs2(i1,i2,i3)
         tzy22(i1,i2,i3)= ry(i1,i2,i3)*tzr2(i1,i2,i3)+sy(i1,i2,i3)*
     & tzs2(i1,i2,i3)
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
         uxy22(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*urr2(i1,i2,i3,kd)
     & +(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*urs2(i1,
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
         uxy23(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*urr2(i1,i2,i3,kd)
     & +sx(i1,i2,i3)*sy(i1,i2,i3)*uss2(i1,i2,i3,kd)+tx(i1,i2,i3)*ty(
     & i1,i2,i3)*utt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,
     & i2,i3)*sx(i1,i2,i3))*urs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(i1,i2,
     & i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*urt2(i1,i2,i3,kd)+(sx(i1,i2,i3)*
     & ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*ust2(i1,i2,i3,kd)+
     & rxy23(i1,i2,i3)*ur2(i1,i2,i3,kd)+sxy23(i1,i2,i3)*us2(i1,i2,i3,
     & kd)+txy23(i1,i2,i3)*ut2(i1,i2,i3,kd)
         uxz23(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*urr2(i1,i2,i3,kd)
     & +sx(i1,i2,i3)*sz(i1,i2,i3)*uss2(i1,i2,i3,kd)+tx(i1,i2,i3)*tz(
     & i1,i2,i3)*utt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,
     & i2,i3)*sx(i1,i2,i3))*urs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(i1,i2,
     & i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*urt2(i1,i2,i3,kd)+(sx(i1,i2,i3)*
     & tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*ust2(i1,i2,i3,kd)+
     & rxz23(i1,i2,i3)*ur2(i1,i2,i3,kd)+sxz23(i1,i2,i3)*us2(i1,i2,i3,
     & kd)+txz23(i1,i2,i3)*ut2(i1,i2,i3,kd)
         uyz23(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*urr2(i1,i2,i3,kd)
     & +sy(i1,i2,i3)*sz(i1,i2,i3)*uss2(i1,i2,i3,kd)+ty(i1,i2,i3)*tz(
     & i1,i2,i3)*utt2(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,
     & i2,i3)*sy(i1,i2,i3))*urs2(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(i1,i2,
     & i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*urt2(i1,i2,i3,kd)+(sy(i1,i2,i3)*
     & tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*ust2(i1,i2,i3,kd)+
     & ryz23(i1,i2,i3)*ur2(i1,i2,i3,kd)+syz23(i1,i2,i3)*us2(i1,i2,i3,
     & kd)+tyz23(i1,i2,i3)*ut2(i1,i2,i3,kd)
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
         uxx23r(i1,i2,i3,kd)=(-2.*u(i1,i2,i3,kd)+(u(i1+1,i2,i3,kd)+u(
     & i1-1,i2,i3,kd)) )*h22(0)
         uyy23r(i1,i2,i3,kd)=(-2.*u(i1,i2,i3,kd)+(u(i1,i2+1,i3,kd)+u(
     & i1,i2-1,i3,kd)) )*h22(1)
         uxy23r(i1,i2,i3,kd)=(ux23r(i1,i2+1,i3,kd)-ux23r(i1,i2-1,i3,kd)
     & )*h12(1)
         uzz23r(i1,i2,i3,kd)=(-2.*u(i1,i2,i3,kd)+(u(i1,i2,i3+1,kd)+u(
     & i1,i2,i3-1,kd)) )*h22(2)
         uxz23r(i1,i2,i3,kd)=(ux23r(i1,i2,i3+1,kd)-ux23r(i1,i2,i3-1,kd)
     & )*h12(2)
         uyz23r(i1,i2,i3,kd)=(uy23r(i1,i2,i3+1,kd)-uy23r(i1,i2,i3-1,kd)
     & )*h12(2)
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
         ulaplacian22r(i1,i2,i3,kd)=uxx23r(i1,i2,i3,kd)+uyy23r(i1,i2,
     & i3,kd)
         ulaplacian23r(i1,i2,i3,kd)=uxx23r(i1,i2,i3,kd)+uyy23r(i1,i2,
     & i3,kd)+uzz23r(i1,i2,i3,kd)
         uxxx22r(i1,i2,i3,kd)=(-2.*(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))+
     & (u(i1+2,i2,i3,kd)-u(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
         uyyy22r(i1,i2,i3,kd)=(-2.*(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))+
     & (u(i1,i2+2,i3,kd)-u(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
         uxxy22r(i1,i2,i3,kd)=( uxx22r(i1,i2+1,i3,kd)-uxx22r(i1,i2-1,
     & i3,kd))/(2.*dx(1))
         uxyy22r(i1,i2,i3,kd)=( uyy22r(i1+1,i2,i3,kd)-uyy22r(i1-1,i2,
     & i3,kd))/(2.*dx(0))
         uxxxx22r(i1,i2,i3,kd)=(6.*u(i1,i2,i3,kd)-4.*(u(i1+1,i2,i3,kd)+
     & u(i1-1,i2,i3,kd))+(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd)) )/(dx(0)*
     & *4)
         uyyyy22r(i1,i2,i3,kd)=(6.*u(i1,i2,i3,kd)-4.*(u(i1,i2+1,i3,kd)+
     & u(i1,i2-1,i3,kd))+(u(i1,i2+2,i3,kd)+u(i1,i2-2,i3,kd)) )/(dx(1)*
     & *4)
         uxxyy22r(i1,i2,i3,kd)=( 4.*u(i1,i2,i3,kd)     -2.*(u(i1+1,i2,
     & i3,kd)+u(i1-1,i2,i3,kd)+u(i1,i2+1,i3,kd)+u(i1,i2-1,i3,kd))   + 
     &   (u(i1+1,i2+1,i3,kd)+u(i1-1,i2+1,i3,kd)+u(i1+1,i2-1,i3,kd)+u(
     & i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
         ! 2D laplacian squared = u.xxxx + 2 u.xxyy + u.yyyy
         uLapSq22r(i1,i2,i3,kd)= ( 6.*u(i1,i2,i3,kd)   - 4.*(u(i1+1,i2,
     & i3,kd)+u(i1-1,i2,i3,kd))    +(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd)
     & ) )/(dx(0)**4) +( 6.*u(i1,i2,i3,kd)    -4.*(u(i1,i2+1,i3,kd)+u(
     & i1,i2-1,i3,kd))    +(u(i1,i2+2,i3,kd)+u(i1,i2-2,i3,kd)) )/(dx(
     & 1)**4)  +( 8.*u(i1,i2,i3,kd)     -4.*(u(i1+1,i2,i3,kd)+u(i1-1,
     & i2,i3,kd)+u(i1,i2+1,i3,kd)+u(i1,i2-1,i3,kd))   +2.*(u(i1+1,i2+
     & 1,i3,kd)+u(i1-1,i2+1,i3,kd)+u(i1+1,i2-1,i3,kd)+u(i1-1,i2-1,i3,
     & kd)) )/(dx(0)**2*dx(1)**2)
         uxxx23r(i1,i2,i3,kd)=(-2.*(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))+
     & (u(i1+2,i2,i3,kd)-u(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
         uyyy23r(i1,i2,i3,kd)=(-2.*(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))+
     & (u(i1,i2+2,i3,kd)-u(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
         uzzz23r(i1,i2,i3,kd)=(-2.*(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))+
     & (u(i1,i2,i3+2,kd)-u(i1,i2,i3-2,kd)) )*h22(1)*h12(2)
         uxxy23r(i1,i2,i3,kd)=( uxx22r(i1,i2+1,i3,kd)-uxx22r(i1,i2-1,
     & i3,kd))/(2.*dx(1))
         uxyy23r(i1,i2,i3,kd)=( uyy22r(i1+1,i2,i3,kd)-uyy22r(i1-1,i2,
     & i3,kd))/(2.*dx(0))
         uxxz23r(i1,i2,i3,kd)=( uxx22r(i1,i2,i3+1,kd)-uxx22r(i1,i2,i3-
     & 1,kd))/(2.*dx(2))
         uyyz23r(i1,i2,i3,kd)=( uyy22r(i1,i2,i3+1,kd)-uyy22r(i1,i2,i3-
     & 1,kd))/(2.*dx(2))
         uxzz23r(i1,i2,i3,kd)=( uzz22r(i1+1,i2,i3,kd)-uzz22r(i1-1,i2,
     & i3,kd))/(2.*dx(0))
         uyzz23r(i1,i2,i3,kd)=( uzz22r(i1,i2+1,i3,kd)-uzz22r(i1,i2-1,
     & i3,kd))/(2.*dx(1))
         uxxxx23r(i1,i2,i3,kd)=(6.*u(i1,i2,i3,kd)-4.*(u(i1+1,i2,i3,kd)+
     & u(i1-1,i2,i3,kd))+(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd)) )/(dx(0)*
     & *4)
         uyyyy23r(i1,i2,i3,kd)=(6.*u(i1,i2,i3,kd)-4.*(u(i1,i2+1,i3,kd)+
     & u(i1,i2-1,i3,kd))+(u(i1,i2+2,i3,kd)+u(i1,i2-2,i3,kd)) )/(dx(1)*
     & *4)
         uzzzz23r(i1,i2,i3,kd)=(6.*u(i1,i2,i3,kd)-4.*(u(i1,i2,i3+1,kd)+
     & u(i1,i2,i3-1,kd))+(u(i1,i2,i3+2,kd)+u(i1,i2,i3-2,kd)) )/(dx(2)*
     & *4)
         uxxyy23r(i1,i2,i3,kd)=( 4.*u(i1,i2,i3,kd)     -2.*(u(i1+1,i2,
     & i3,kd)+u(i1-1,i2,i3,kd)+u(i1,i2+1,i3,kd)+u(i1,i2-1,i3,kd))   + 
     &   (u(i1+1,i2+1,i3,kd)+u(i1-1,i2+1,i3,kd)+u(i1+1,i2-1,i3,kd)+u(
     & i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
         uxxzz23r(i1,i2,i3,kd)=( 4.*u(i1,i2,i3,kd)     -2.*(u(i1+1,i2,
     & i3,kd)+u(i1-1,i2,i3,kd)+u(i1,i2,i3+1,kd)+u(i1,i2,i3-1,kd))   + 
     &   (u(i1+1,i2,i3+1,kd)+u(i1-1,i2,i3+1,kd)+u(i1+1,i2,i3-1,kd)+u(
     & i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)**2)
         uyyzz23r(i1,i2,i3,kd)=( 4.*u(i1,i2,i3,kd)     -2.*(u(i1,i2+1,
     & i3,kd)  +u(i1,i2-1,i3,kd)+  u(i1,i2  ,i3+1,kd)+u(i1,i2  ,i3-1,
     & kd))   +   (u(i1,i2+1,i3+1,kd)+u(i1,i2-1,i3+1,kd)+u(i1,i2+1,i3-
     & 1,kd)+u(i1,i2-1,i3-1,kd)) )/(dx(1)**2*dx(2)**2)
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
        !    --- For 2nd order 2D artificial diffusion ---
         delta22(c)=    (u(i1+1,i2,i3,c)-4.*u(i1,i2,i3,c)+u(i1-1,i2,i3,
     & c)  +u(i1,i2+1,i3,c)                 +u(i1,i2-1,i3,c))
        !    --- For 2nd order 3D artificial diffusion ---
         delta23(c)= (u(i1+1,i2,i3,c)-6.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c) 
     &   +u(i1,i2+1,i3,c)                   +u(i1,i2-1,i3,c)  +u(i1,
     & i2,i3+1,c)                   +u(i1,i2,i3-1,c))
        !     ---For fourth-order artificial diffusion in 2D
         delta42(c)= (   -u(i1+2,i2,i3,c)-u(i1-2,i2,i3,c)   -u(i1,i2+2,
     & i3,c)-u(i1,i2-2,i3,c)   +4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)   
     & +u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c))  -12.*u(i1,i2,i3,c) )
        !     ---For fourth-order artificial diffusion in 3D
         delta43(c)= (   -u(i1+2,i2,i3,c)-u(i1-2,i2,i3,c)  -u(i1,i2+2,
     & i3,c)-u(i1,i2-2,i3,c)  -u(i1,i2,i3+2,c)-u(i1,i2,i3-2,c)  +4.*(
     & u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)  +u(i1,i2+1,i3,c)+u(i1,i2-1,i3,
     & c)  +u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c)) -18.*u(i1,i2,i3,c) )
        !     --- end statement functions
         ierr=0
         ! write(*,'("Inside insdt: gridType=",i2)') gridType
         pc                 =ipar(0)
         uc                 =ipar(1)
         vc                 =ipar(2)
         wc                 =ipar(3)
         nc                 =ipar(4)
         sc                 =ipar(5)
         tc                 =ipar(6)  ! **new**
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
         rc                 =ipar(22)
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
        !       gravity(0)        =rpar(18)
        !      gravity(1)        =rpar(19)
        !      gravity(2)        =rpar(20)
        !      thermalExpansivity=rpar(21)
        !      adcBoussinesq     =rpar(22) ! coefficient of artificial diffusion for Boussinesq T equation 
        !      kThermal          =rpar(23)
         t                 =rpar(24)
        ! nuVP              =rpar(24)  ! for visco-plastic
         ! etaVP             =rpar(25)
         ! yieldStressVP     =rpar(26)
         ! exponentVP        =rpar(27)
         ! epsVP             =rpar(28)
        ! write(*,'(" insdt: eta,yield,exp,eps=",4e10.2)') etaVP,yieldStressVP,exponentVP,epsVP
         kc=nc
         ec=kc+1
         if( orderOfAccuracy.ne.2 .and. orderOfAccuracy.ne.4 )then
           write(*,'("insdt:ERROR orderOfAccuracy=",i6)') 
     & orderOfAccuracy
           stop 1
         end if
         if( gridType.ne.rectangular .and. gridType.ne.curvilinear )
     & then
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
           write(*,'("insdt:ERROR in kc: nd,uc,vc,wc,kc=",2i6)') nd,uc,
     & vc,wc,kc
           stop 5
         end if
        ! ** these are needed by self-adjoint terms **fix**
         dxi=1./dx(0)
         dyi=1./dx(1)
         dzi=1./dx(2)
         dri=1./dr(0)
         dsi=1./dr(1)
         dti=1./dr(2)
         dr2i=1./(2.*dr(0))
         ds2i=1./(2.*dr(1))
         dt2i=1./(2.*dr(2))
         if( turbulenceModel.eq.spalartAllmaras )then
           call getSpalartAllmarasParameters(cb1, cb2, cv1, sigma, 
     & sigmai, kappa, cw1, cw2, cw3, cw3e6, cv1e3, cd0, cr0)
         else if( turbulenceModel.eq.kEpsilon )then
          ! write(*,'(" insdt: k-epsilon: nc,kc,ec=",3i3)') nc,kc,ec
           call getKEpsilonParameters( cMu,cEps1,cEps2,sigmaEpsI,
     & sigmaKI )
           !  write(*,'(" insdt: cMu,cEps1,cEps2,sigmaEpsI,sigmaKI=",5f8.3)') cMu,cEps1,cEps2,sigmaEpsI,sigmaKI
         else if( turbulenceModel.eq.largeEddySimulation )then
           ! do nothing
         else if( turbulenceModel.ne.noTurbulenceModel )then
           stop 88
         end if
         adc=adcPassiveScalar ! coefficient of linear artificial diffusion
         cd22=ad22/(nd**2)
         cd42=ad42/(nd**2)
        !     *********************************      
        !     ********MAIN LOOPS***************      
        !     *********************************      
         if( gridType.eq.rectangular )then
          if( isAxisymmetric.eq.0 )then
           if( gridIsImplicit.eq.0 )then
            ! --- explicit time-stepping ---
            if( advectionOption.eq.centeredAdvection )then
              if( useWhereMask.ne.0 )then
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                if( mask(i1,i2,i3).gt.0 )then
                   ! INS, no AD
                    ut(i1,i2,i3,uc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,
     & uc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,uc)-uu(i1,i2,i3,wc)*uz23r(
     & i1,i2,i3,uc)-ux23r(i1,i2,i3,pc)+nu*ulaplacian23r(i1,i2,i3,uc)
                    ut(i1,i2,i3,vc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,
     & vc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,vc)-uu(i1,i2,i3,wc)*uz23r(
     & i1,i2,i3,vc)-uy23r(i1,i2,i3,pc)+nu*ulaplacian23r(i1,i2,i3,vc)
                    ut(i1,i2,i3,wc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,
     & wc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,wc)-uu(i1,i2,i3,wc)*uz23r(
     & i1,i2,i3,wc)-uz23r(i1,i2,i3,pc)+nu*ulaplacian23r(i1,i2,i3,wc)
                end if
               end do
               end do
               end do
              else
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                  ! INS, no AD
                   ut(i1,i2,i3,uc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,uc)
     & -uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,uc)-uu(i1,i2,i3,wc)*uz23r(i1,
     & i2,i3,uc)-ux23r(i1,i2,i3,pc)+nu*ulaplacian23r(i1,i2,i3,uc)
                   ut(i1,i2,i3,vc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,vc)
     & -uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,vc)-uu(i1,i2,i3,wc)*uz23r(i1,
     & i2,i3,vc)-uy23r(i1,i2,i3,pc)+nu*ulaplacian23r(i1,i2,i3,vc)
                   ut(i1,i2,i3,wc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,wc)
     & -uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,wc)-uu(i1,i2,i3,wc)*uz23r(i1,
     & i2,i3,wc)-uz23r(i1,i2,i3,pc)+nu*ulaplacian23r(i1,i2,i3,wc)
               end do
               end do
               end do
              end if
            else if( advectionOption.eq.upwindAdvection )then
              ! --- upwind ---
              if( useWhereMask.ne.0 )then
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                if( mask(i1,i2,i3).gt.0 )then
                  ! INS, no AD
                    ! --- upwind approximations ---
                      ! --- upwind scheme ---
                      ! for testing output this next message:
                      if( t.le. 0. )then
                        write(*,'(" getAdvection upwind scheme (7)")')
                      end if
                        ! --- CARTESIAN GRID ---
                        !- agu(uc,uc)=UU(uc)*UX(uc)
                        !- agu(vc,uc)=UU(vc)*UY(uc)
                        !- agu(uc,vc)=UU(uc)*UX(vc)
                        !- agu(vc,vc)=UU(vc)*UY(vc)
                        ! -- first order upwind --
                        if( upwindOrder.eq.1 )then
                         au = u(i1,i2,i3,uc)
                         if( au.gt.0. )then
                           ! u*ux = u*D-x(u)
                           agu(uc,uc)= au*(u(i1,i2,i3,uc)-u(i1-1,i2,i3,
     & uc))/(dx(0))
                           ! u*vx = u*D-x(v)
                           agu(uc,vc)= au*(u(i1,i2,i3,vc)-u(i1-1,i2,i3,
     & vc))/(dx(0))
                         else
                           ! u*ux = u*D+x(u)
                           agu(uc,uc)= au*(u(i1+1,i2,i3,uc)-u(i1,i2,i3,
     & uc))/(dx(0))
                           ! u*vx = u*D+x(v)
                           agu(uc,vc)= au*(u(i1+1,i2,i3,vc)-u(i1,i2,i3,
     & vc))/(dx(0))
                         end if
                         au = u(i1,i2,i3,vc)
                         if( au.gt.0. )then
                           ! v*uy = v*D-y(u)
                           agu(vc,uc)= au*(u(i1,i2,i3,uc)-u(i1,i2-1,i3,
     & uc))/(dx(1))
                           ! v*vy = v*D-y(v)
                           agu(vc,vc)= au*(u(i1,i2,i3,vc)-u(i1,i2-1,i3,
     & vc))/(dx(1))
                         else
                           ! v*uy = v*D+y(u)
                           agu(vc,uc)= au*(u(i1,i2+1,i3,uc)-u(i1,i2,i3,
     & uc))/(dx(1))
                           ! v*vy = v*D+y(v) 
                           agu(vc,vc)= au*(u(i1,i2+1,i3,vc)-u(i1,i2,i3,
     & vc))/(dx(1))
                         end if
                            ! finish me 
                            stop 777
                        else
                          write(*,'(" finish me, upwindOrder=",i2)') 
     & upwindOrder
                          stop 222
                        end if
                     ut(i1,i2,i3,uc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,
     & uc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,uc)-uu(i1,i2,i3,wc)*uz23r(
     & i1,i2,i3,uc)-ux23r(i1,i2,i3,pc)+nu*ulaplacian23r(i1,i2,i3,uc)
                     ut(i1,i2,i3,vc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,
     & vc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,vc)-uu(i1,i2,i3,wc)*uz23r(
     & i1,i2,i3,vc)-uy23r(i1,i2,i3,pc)+nu*ulaplacian23r(i1,i2,i3,vc)
                     ut(i1,i2,i3,wc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,
     & wc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,wc)-uu(i1,i2,i3,wc)*uz23r(
     & i1,i2,i3,wc)-uz23r(i1,i2,i3,pc)+nu*ulaplacian23r(i1,i2,i3,wc)
                end if
               end do
               end do
               end do
              else
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                 ! INS, no AD
                   ! --- upwind approximations ---
                     ! --- upwind scheme ---
                     ! for testing output this next message:
                     if( t.le. 0. )then
                       write(*,'(" getAdvection upwind scheme (7)")')
                     end if
                       ! --- CARTESIAN GRID ---
                       !- agu(uc,uc)=UU(uc)*UX(uc)
                       !- agu(vc,uc)=UU(vc)*UY(uc)
                       !- agu(uc,vc)=UU(uc)*UX(vc)
                       !- agu(vc,vc)=UU(vc)*UY(vc)
                       ! -- first order upwind --
                       if( upwindOrder.eq.1 )then
                        au = u(i1,i2,i3,uc)
                        if( au.gt.0. )then
                          ! u*ux = u*D-x(u)
                          agu(uc,uc)= au*(u(i1,i2,i3,uc)-u(i1-1,i2,i3,
     & uc))/(dx(0))
                          ! u*vx = u*D-x(v)
                          agu(uc,vc)= au*(u(i1,i2,i3,vc)-u(i1-1,i2,i3,
     & vc))/(dx(0))
                        else
                          ! u*ux = u*D+x(u)
                          agu(uc,uc)= au*(u(i1+1,i2,i3,uc)-u(i1,i2,i3,
     & uc))/(dx(0))
                          ! u*vx = u*D+x(v)
                          agu(uc,vc)= au*(u(i1+1,i2,i3,vc)-u(i1,i2,i3,
     & vc))/(dx(0))
                        end if
                        au = u(i1,i2,i3,vc)
                        if( au.gt.0. )then
                          ! v*uy = v*D-y(u)
                          agu(vc,uc)= au*(u(i1,i2,i3,uc)-u(i1,i2-1,i3,
     & uc))/(dx(1))
                          ! v*vy = v*D-y(v)
                          agu(vc,vc)= au*(u(i1,i2,i3,vc)-u(i1,i2-1,i3,
     & vc))/(dx(1))
                        else
                          ! v*uy = v*D+y(u)
                          agu(vc,uc)= au*(u(i1,i2+1,i3,uc)-u(i1,i2,i3,
     & uc))/(dx(1))
                          ! v*vy = v*D+y(v) 
                          agu(vc,vc)= au*(u(i1,i2+1,i3,vc)-u(i1,i2,i3,
     & vc))/(dx(1))
                        end if
                           ! finish me 
                           stop 777
                       else
                         write(*,'(" finish me, upwindOrder=",i2)') 
     & upwindOrder
                         stop 222
                       end if
                    ut(i1,i2,i3,uc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,
     & uc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,uc)-uu(i1,i2,i3,wc)*uz23r(
     & i1,i2,i3,uc)-ux23r(i1,i2,i3,pc)+nu*ulaplacian23r(i1,i2,i3,uc)
                    ut(i1,i2,i3,vc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,
     & vc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,vc)-uu(i1,i2,i3,wc)*uz23r(
     & i1,i2,i3,vc)-uy23r(i1,i2,i3,pc)+nu*ulaplacian23r(i1,i2,i3,vc)
                    ut(i1,i2,i3,wc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,
     & wc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,wc)-uu(i1,i2,i3,wc)*uz23r(
     & i1,i2,i3,wc)-uz23r(i1,i2,i3,pc)+nu*ulaplacian23r(i1,i2,i3,wc)
               end do
               end do
               end do
              end if
            else if( advectionOption.eq.bwenoAdvection )then
              ! --- bweno ---
              if( useWhereMask.ne.0 )then
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                if( mask(i1,i2,i3).gt.0 )then
                  ! INS, no AD
                    ! --- upwind approximations ---
                      ! --- upwind scheme ---
                      ! for testing output this next message:
                      if( t.le. 0. )then
                        write(*,'(" getAdvection upwind scheme (7)")')
                      end if
                        ! --- CARTESIAN GRID ---
                        !- agu(uc,uc)=UU(uc)*UX(uc)
                        !- agu(vc,uc)=UU(vc)*UY(uc)
                        !- agu(uc,vc)=UU(uc)*UX(vc)
                        !- agu(vc,vc)=UU(vc)*UY(vc)
                        ! -- first order upwind --
                        if( upwindOrder.eq.1 )then
                         au = u(i1,i2,i3,uc)
                         if( au.gt.0. )then
                           ! u*ux = u*D-x(u)
                           agu(uc,uc)= au*(u(i1,i2,i3,uc)-u(i1-1,i2,i3,
     & uc))/(dx(0))
                           ! u*vx = u*D-x(v)
                           agu(uc,vc)= au*(u(i1,i2,i3,vc)-u(i1-1,i2,i3,
     & vc))/(dx(0))
                         else
                           ! u*ux = u*D+x(u)
                           agu(uc,uc)= au*(u(i1+1,i2,i3,uc)-u(i1,i2,i3,
     & uc))/(dx(0))
                           ! u*vx = u*D+x(v)
                           agu(uc,vc)= au*(u(i1+1,i2,i3,vc)-u(i1,i2,i3,
     & vc))/(dx(0))
                         end if
                         au = u(i1,i2,i3,vc)
                         if( au.gt.0. )then
                           ! v*uy = v*D-y(u)
                           agu(vc,uc)= au*(u(i1,i2,i3,uc)-u(i1,i2-1,i3,
     & uc))/(dx(1))
                           ! v*vy = v*D-y(v)
                           agu(vc,vc)= au*(u(i1,i2,i3,vc)-u(i1,i2-1,i3,
     & vc))/(dx(1))
                         else
                           ! v*uy = v*D+y(u)
                           agu(vc,uc)= au*(u(i1,i2+1,i3,uc)-u(i1,i2,i3,
     & uc))/(dx(1))
                           ! v*vy = v*D+y(v) 
                           agu(vc,vc)= au*(u(i1,i2+1,i3,vc)-u(i1,i2,i3,
     & vc))/(dx(1))
                         end if
                            ! finish me 
                            stop 777
                        else
                          write(*,'(" finish me, upwindOrder=",i2)') 
     & upwindOrder
                          stop 222
                        end if
                     ut(i1,i2,i3,uc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,
     & uc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,uc)-uu(i1,i2,i3,wc)*uz23r(
     & i1,i2,i3,uc)-ux23r(i1,i2,i3,pc)+nu*ulaplacian23r(i1,i2,i3,uc)
                     ut(i1,i2,i3,vc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,
     & vc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,vc)-uu(i1,i2,i3,wc)*uz23r(
     & i1,i2,i3,vc)-uy23r(i1,i2,i3,pc)+nu*ulaplacian23r(i1,i2,i3,vc)
                     ut(i1,i2,i3,wc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,
     & wc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,wc)-uu(i1,i2,i3,wc)*uz23r(
     & i1,i2,i3,wc)-uz23r(i1,i2,i3,pc)+nu*ulaplacian23r(i1,i2,i3,wc)
                end if
               end do
               end do
               end do
              else
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                 ! INS, no AD
                   ! --- upwind approximations ---
                     ! --- upwind scheme ---
                     ! for testing output this next message:
                     if( t.le. 0. )then
                       write(*,'(" getAdvection upwind scheme (7)")')
                     end if
                       ! --- CARTESIAN GRID ---
                       !- agu(uc,uc)=UU(uc)*UX(uc)
                       !- agu(vc,uc)=UU(vc)*UY(uc)
                       !- agu(uc,vc)=UU(uc)*UX(vc)
                       !- agu(vc,vc)=UU(vc)*UY(vc)
                       ! -- first order upwind --
                       if( upwindOrder.eq.1 )then
                        au = u(i1,i2,i3,uc)
                        if( au.gt.0. )then
                          ! u*ux = u*D-x(u)
                          agu(uc,uc)= au*(u(i1,i2,i3,uc)-u(i1-1,i2,i3,
     & uc))/(dx(0))
                          ! u*vx = u*D-x(v)
                          agu(uc,vc)= au*(u(i1,i2,i3,vc)-u(i1-1,i2,i3,
     & vc))/(dx(0))
                        else
                          ! u*ux = u*D+x(u)
                          agu(uc,uc)= au*(u(i1+1,i2,i3,uc)-u(i1,i2,i3,
     & uc))/(dx(0))
                          ! u*vx = u*D+x(v)
                          agu(uc,vc)= au*(u(i1+1,i2,i3,vc)-u(i1,i2,i3,
     & vc))/(dx(0))
                        end if
                        au = u(i1,i2,i3,vc)
                        if( au.gt.0. )then
                          ! v*uy = v*D-y(u)
                          agu(vc,uc)= au*(u(i1,i2,i3,uc)-u(i1,i2-1,i3,
     & uc))/(dx(1))
                          ! v*vy = v*D-y(v)
                          agu(vc,vc)= au*(u(i1,i2,i3,vc)-u(i1,i2-1,i3,
     & vc))/(dx(1))
                        else
                          ! v*uy = v*D+y(u)
                          agu(vc,uc)= au*(u(i1,i2+1,i3,uc)-u(i1,i2,i3,
     & uc))/(dx(1))
                          ! v*vy = v*D+y(v) 
                          agu(vc,vc)= au*(u(i1,i2+1,i3,vc)-u(i1,i2,i3,
     & vc))/(dx(1))
                        end if
                           ! finish me 
                           stop 777
                       else
                         write(*,'(" finish me, upwindOrder=",i2)') 
     & upwindOrder
                         stop 222
                       end if
                    ut(i1,i2,i3,uc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,
     & uc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,uc)-uu(i1,i2,i3,wc)*uz23r(
     & i1,i2,i3,uc)-ux23r(i1,i2,i3,pc)+nu*ulaplacian23r(i1,i2,i3,uc)
                    ut(i1,i2,i3,vc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,
     & vc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,vc)-uu(i1,i2,i3,wc)*uz23r(
     & i1,i2,i3,vc)-uy23r(i1,i2,i3,pc)+nu*ulaplacian23r(i1,i2,i3,vc)
                    ut(i1,i2,i3,wc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,
     & wc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,wc)-uu(i1,i2,i3,wc)*uz23r(
     & i1,i2,i3,wc)-uz23r(i1,i2,i3,pc)+nu*ulaplacian23r(i1,i2,i3,wc)
               end do
               end do
               end do
              end if
            else
              write(*,'(" unknown advectionOption")')
              stop 1010
            end if
           else ! gridIsImplicit
            ! ---- implicit time-stepping ---
            if( advectionOption.eq.centeredAdvection )then
             if( implicitOption .eq.computeImplicitTermsSeparately )
     & then
               if( useWhereMask.ne.0 )then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                 if( mask(i1,i2,i3).gt.0 )then
                    ! explicit terms only, no diffusion
                     ut(i1,i2,i3,uc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,
     & uc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,uc)-uu(i1,i2,i3,wc)*uz23r(
     & i1,i2,i3,uc)-ux23r(i1,i2,i3,pc)
                     ut(i1,i2,i3,vc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,
     & vc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,vc)-uu(i1,i2,i3,wc)*uz23r(
     & i1,i2,i3,vc)-uy23r(i1,i2,i3,pc)
                     ut(i1,i2,i3,wc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,
     & wc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,wc)-uu(i1,i2,i3,wc)*uz23r(
     & i1,i2,i3,wc)-uz23r(i1,i2,i3,pc)
                    ! include implicit terms - diffusion
                     uti(i1,i2,i3,uc)= nu*ulaplacian23r(i1,i2,i3,uc)
                     uti(i1,i2,i3,vc)= nu*ulaplacian23r(i1,i2,i3,vc)
                     uti(i1,i2,i3,wc)= nu*ulaplacian23r(i1,i2,i3,wc)
                 end if
                end do
                end do
                end do
               else
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                   ! explicit terms only, no diffusion
                    ut(i1,i2,i3,uc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,
     & uc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,uc)-uu(i1,i2,i3,wc)*uz23r(
     & i1,i2,i3,uc)-ux23r(i1,i2,i3,pc)
                    ut(i1,i2,i3,vc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,
     & vc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,vc)-uu(i1,i2,i3,wc)*uz23r(
     & i1,i2,i3,vc)-uy23r(i1,i2,i3,pc)
                    ut(i1,i2,i3,wc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,
     & wc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,wc)-uu(i1,i2,i3,wc)*uz23r(
     & i1,i2,i3,wc)-uz23r(i1,i2,i3,pc)
                   ! include implicit terms - diffusion
                    uti(i1,i2,i3,uc)= nu*ulaplacian23r(i1,i2,i3,uc)
                    uti(i1,i2,i3,vc)= nu*ulaplacian23r(i1,i2,i3,vc)
                    uti(i1,i2,i3,wc)= nu*ulaplacian23r(i1,i2,i3,wc)
                end do
                end do
                end do
               end if
             else if( implicitOption.eq.doNotComputeImplicitTerms )then
               if( useWhereMask.ne.0 )then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                 if( mask(i1,i2,i3).gt.0 )then
                    ! explicit terms only, no diffusion
                     ut(i1,i2,i3,uc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,
     & uc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,uc)-uu(i1,i2,i3,wc)*uz23r(
     & i1,i2,i3,uc)-ux23r(i1,i2,i3,pc)
                     ut(i1,i2,i3,vc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,
     & vc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,vc)-uu(i1,i2,i3,wc)*uz23r(
     & i1,i2,i3,vc)-uy23r(i1,i2,i3,pc)
                     ut(i1,i2,i3,wc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,
     & wc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,wc)-uu(i1,i2,i3,wc)*uz23r(
     & i1,i2,i3,wc)-uz23r(i1,i2,i3,pc)
                 end if
                end do
                end do
                end do
               else
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                   ! explicit terms only, no diffusion
                    ut(i1,i2,i3,uc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,
     & uc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,uc)-uu(i1,i2,i3,wc)*uz23r(
     & i1,i2,i3,uc)-ux23r(i1,i2,i3,pc)
                    ut(i1,i2,i3,vc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,
     & vc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,vc)-uu(i1,i2,i3,wc)*uz23r(
     & i1,i2,i3,vc)-uy23r(i1,i2,i3,pc)
                    ut(i1,i2,i3,wc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,
     & wc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,wc)-uu(i1,i2,i3,wc)*uz23r(
     & i1,i2,i3,wc)-uz23r(i1,i2,i3,pc)
                end do
                end do
                end do
               end if
             else
              write(*,*)'insdt: Unknown implicitOption=',implicitOption
              stop 5
             end if  ! end implicitOption
            else if( advectionOption.eq.upwindAdvection )then
              ! --- upwind ---
             if( implicitOption .eq.computeImplicitTermsSeparately )
     & then
               if( useWhereMask.ne.0 )then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                 if( mask(i1,i2,i3).gt.0 )then
                    ! explicit terms only, no diffusion
                     ut(i1,i2,i3,uc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,
     & uc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,uc)-uu(i1,i2,i3,wc)*uz23r(
     & i1,i2,i3,uc)-ux23r(i1,i2,i3,pc)
                     ut(i1,i2,i3,vc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,
     & vc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,vc)-uu(i1,i2,i3,wc)*uz23r(
     & i1,i2,i3,vc)-uy23r(i1,i2,i3,pc)
                     ut(i1,i2,i3,wc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,
     & wc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,wc)-uu(i1,i2,i3,wc)*uz23r(
     & i1,i2,i3,wc)-uz23r(i1,i2,i3,pc)
                    ! include implicit terms - diffusion
                     uti(i1,i2,i3,uc)= nu*ulaplacian23r(i1,i2,i3,uc)
                     uti(i1,i2,i3,vc)= nu*ulaplacian23r(i1,i2,i3,vc)
                     uti(i1,i2,i3,wc)= nu*ulaplacian23r(i1,i2,i3,wc)
                 end if
                end do
                end do
                end do
               else
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                   ! explicit terms only, no diffusion
                    ut(i1,i2,i3,uc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,
     & uc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,uc)-uu(i1,i2,i3,wc)*uz23r(
     & i1,i2,i3,uc)-ux23r(i1,i2,i3,pc)
                    ut(i1,i2,i3,vc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,
     & vc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,vc)-uu(i1,i2,i3,wc)*uz23r(
     & i1,i2,i3,vc)-uy23r(i1,i2,i3,pc)
                    ut(i1,i2,i3,wc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,
     & wc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,wc)-uu(i1,i2,i3,wc)*uz23r(
     & i1,i2,i3,wc)-uz23r(i1,i2,i3,pc)
                   ! include implicit terms - diffusion
                    uti(i1,i2,i3,uc)= nu*ulaplacian23r(i1,i2,i3,uc)
                    uti(i1,i2,i3,vc)= nu*ulaplacian23r(i1,i2,i3,vc)
                    uti(i1,i2,i3,wc)= nu*ulaplacian23r(i1,i2,i3,wc)
                end do
                end do
                end do
               end if
             else if( implicitOption.eq.doNotComputeImplicitTerms )then
               if( useWhereMask.ne.0 )then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                 if( mask(i1,i2,i3).gt.0 )then
                    ! explicit terms only, no diffusion
                     ut(i1,i2,i3,uc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,
     & uc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,uc)-uu(i1,i2,i3,wc)*uz23r(
     & i1,i2,i3,uc)-ux23r(i1,i2,i3,pc)
                     ut(i1,i2,i3,vc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,
     & vc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,vc)-uu(i1,i2,i3,wc)*uz23r(
     & i1,i2,i3,vc)-uy23r(i1,i2,i3,pc)
                     ut(i1,i2,i3,wc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,
     & wc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,wc)-uu(i1,i2,i3,wc)*uz23r(
     & i1,i2,i3,wc)-uz23r(i1,i2,i3,pc)
                 end if
                end do
                end do
                end do
               else
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                   ! explicit terms only, no diffusion
                    ut(i1,i2,i3,uc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,
     & uc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,uc)-uu(i1,i2,i3,wc)*uz23r(
     & i1,i2,i3,uc)-ux23r(i1,i2,i3,pc)
                    ut(i1,i2,i3,vc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,
     & vc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,vc)-uu(i1,i2,i3,wc)*uz23r(
     & i1,i2,i3,vc)-uy23r(i1,i2,i3,pc)
                    ut(i1,i2,i3,wc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,
     & wc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,wc)-uu(i1,i2,i3,wc)*uz23r(
     & i1,i2,i3,wc)-uz23r(i1,i2,i3,pc)
                end do
                end do
                end do
               end if
             else
              write(*,*)'insdt: Unknown implicitOption=',implicitOption
              stop 6
             end if  ! end implicitOption
            else if( advectionOption.eq.bwenoAdvection )then
              ! --- bweno ---
             if( implicitOption .eq.computeImplicitTermsSeparately )
     & then
               if( useWhereMask.ne.0 )then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                 if( mask(i1,i2,i3).gt.0 )then
                    ! explicit terms only, no diffusion
                     ut(i1,i2,i3,uc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,
     & uc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,uc)-uu(i1,i2,i3,wc)*uz23r(
     & i1,i2,i3,uc)-ux23r(i1,i2,i3,pc)
                     ut(i1,i2,i3,vc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,
     & vc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,vc)-uu(i1,i2,i3,wc)*uz23r(
     & i1,i2,i3,vc)-uy23r(i1,i2,i3,pc)
                     ut(i1,i2,i3,wc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,
     & wc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,wc)-uu(i1,i2,i3,wc)*uz23r(
     & i1,i2,i3,wc)-uz23r(i1,i2,i3,pc)
                    ! include implicit terms - diffusion
                     uti(i1,i2,i3,uc)= nu*ulaplacian23r(i1,i2,i3,uc)
                     uti(i1,i2,i3,vc)= nu*ulaplacian23r(i1,i2,i3,vc)
                     uti(i1,i2,i3,wc)= nu*ulaplacian23r(i1,i2,i3,wc)
                 end if
                end do
                end do
                end do
               else
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                   ! explicit terms only, no diffusion
                    ut(i1,i2,i3,uc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,
     & uc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,uc)-uu(i1,i2,i3,wc)*uz23r(
     & i1,i2,i3,uc)-ux23r(i1,i2,i3,pc)
                    ut(i1,i2,i3,vc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,
     & vc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,vc)-uu(i1,i2,i3,wc)*uz23r(
     & i1,i2,i3,vc)-uy23r(i1,i2,i3,pc)
                    ut(i1,i2,i3,wc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,
     & wc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,wc)-uu(i1,i2,i3,wc)*uz23r(
     & i1,i2,i3,wc)-uz23r(i1,i2,i3,pc)
                   ! include implicit terms - diffusion
                    uti(i1,i2,i3,uc)= nu*ulaplacian23r(i1,i2,i3,uc)
                    uti(i1,i2,i3,vc)= nu*ulaplacian23r(i1,i2,i3,vc)
                    uti(i1,i2,i3,wc)= nu*ulaplacian23r(i1,i2,i3,wc)
                end do
                end do
                end do
               end if
             else if( implicitOption.eq.doNotComputeImplicitTerms )then
               if( useWhereMask.ne.0 )then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                 if( mask(i1,i2,i3).gt.0 )then
                    ! explicit terms only, no diffusion
                     ut(i1,i2,i3,uc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,
     & uc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,uc)-uu(i1,i2,i3,wc)*uz23r(
     & i1,i2,i3,uc)-ux23r(i1,i2,i3,pc)
                     ut(i1,i2,i3,vc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,
     & vc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,vc)-uu(i1,i2,i3,wc)*uz23r(
     & i1,i2,i3,vc)-uy23r(i1,i2,i3,pc)
                     ut(i1,i2,i3,wc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,
     & wc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,wc)-uu(i1,i2,i3,wc)*uz23r(
     & i1,i2,i3,wc)-uz23r(i1,i2,i3,pc)
                 end if
                end do
                end do
                end do
               else
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                   ! explicit terms only, no diffusion
                    ut(i1,i2,i3,uc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,
     & uc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,uc)-uu(i1,i2,i3,wc)*uz23r(
     & i1,i2,i3,uc)-ux23r(i1,i2,i3,pc)
                    ut(i1,i2,i3,vc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,
     & vc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,vc)-uu(i1,i2,i3,wc)*uz23r(
     & i1,i2,i3,vc)-uy23r(i1,i2,i3,pc)
                    ut(i1,i2,i3,wc)= -uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,
     & wc)-uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,wc)-uu(i1,i2,i3,wc)*uz23r(
     & i1,i2,i3,wc)-uz23r(i1,i2,i3,pc)
                end do
                end do
                end do
               end if
             else
              write(*,*)'insdt: Unknown implicitOption=',implicitOption
              stop 7
             end if  ! end implicitOption
            else
              write(*,'(" unknown advectionOption")')
              stop 1010
            end if
           end if
          else if( isAxisymmetric.eq.1 )then
           if( advectionOption.ne.centeredAdvection )then
             write(*,*) 'insdt.h : finish me for axisymmetric'
             stop 2020
           end if
          else
            stop 88733
          end if
         else if( gridType.eq.curvilinear )then
          if( isAxisymmetric.eq.0 )then
           if( gridIsImplicit.eq.0 )then
            ! --- explicit time-stepping ---
            if( advectionOption.eq.centeredAdvection )then
              if( useWhereMask.ne.0 )then
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                if( mask(i1,i2,i3).gt.0 )then
                   ! INS, no AD
                    ut(i1,i2,i3,uc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,uc)
     & -uu(i1,i2,i3,vc)*uy23(i1,i2,i3,uc)-uu(i1,i2,i3,wc)*uz23(i1,i2,
     & i3,uc)-ux23(i1,i2,i3,pc)+nu*ulaplacian23(i1,i2,i3,uc)
                    ut(i1,i2,i3,vc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,vc)
     & -uu(i1,i2,i3,vc)*uy23(i1,i2,i3,vc)-uu(i1,i2,i3,wc)*uz23(i1,i2,
     & i3,vc)-uy23(i1,i2,i3,pc)+nu*ulaplacian23(i1,i2,i3,vc)
                    ut(i1,i2,i3,wc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,wc)
     & -uu(i1,i2,i3,vc)*uy23(i1,i2,i3,wc)-uu(i1,i2,i3,wc)*uz23(i1,i2,
     & i3,wc)-uz23(i1,i2,i3,pc)+nu*ulaplacian23(i1,i2,i3,wc)
                end if
               end do
               end do
               end do
              else
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                  ! INS, no AD
                   ut(i1,i2,i3,uc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,uc)-
     & uu(i1,i2,i3,vc)*uy23(i1,i2,i3,uc)-uu(i1,i2,i3,wc)*uz23(i1,i2,
     & i3,uc)-ux23(i1,i2,i3,pc)+nu*ulaplacian23(i1,i2,i3,uc)
                   ut(i1,i2,i3,vc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,vc)-
     & uu(i1,i2,i3,vc)*uy23(i1,i2,i3,vc)-uu(i1,i2,i3,wc)*uz23(i1,i2,
     & i3,vc)-uy23(i1,i2,i3,pc)+nu*ulaplacian23(i1,i2,i3,vc)
                   ut(i1,i2,i3,wc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,wc)-
     & uu(i1,i2,i3,vc)*uy23(i1,i2,i3,wc)-uu(i1,i2,i3,wc)*uz23(i1,i2,
     & i3,wc)-uz23(i1,i2,i3,pc)+nu*ulaplacian23(i1,i2,i3,wc)
               end do
               end do
               end do
              end if
            else if( advectionOption.eq.upwindAdvection )then
              ! --- upwind ---
              if( useWhereMask.ne.0 )then
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                if( mask(i1,i2,i3).gt.0 )then
                  ! INS, no AD
                    ! --- upwind approximations ---
                      ! --- upwind scheme ---
                      ! for testing output this next message:
                      if( t.le. 0. )then
                        write(*,'(" getAdvection upwind scheme (7)")')
                      end if
                        ! --- CURVILINEAR GRID ---
                      !  #If "3" eq "2"
                      !    au = rsxy(i1,i2,i3,0,0)*u(i1,i2,i3,uc)+rsxy(i1,i2,i3,0,1)*u(i1,i2,i3,vc)
                      !  #Else
                      !    au = rsxy(i1,i2,i3,0,0)*u(i1,i2,i3,uc)+rsxy(i1,i2,i3,0,1)*u(i1,i2,i3,vc)+rsxy(i1,i2,i3,0,2)*u(i1,i2,i3,wc)
                      !  #End
                      !
                      !  if( au.gt.0. )then
                      !    agu(uc,uc)= au*(u(i1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(dr(0))
                      !  else
                      !    agu(uc,uc)= au*(u(i1+1,i2,i3,uc)-u(i1,i2,i3,uc))/(dr(0))
                      !  end if
                        agu(uc,uc)=uu(i1,i2,i3,uc)*ux23(i1,i2,i3,uc)
                        agu(vc,uc)=uu(i1,i2,i3,vc)*uy23(i1,i2,i3,uc)
                        agu(uc,vc)=uu(i1,i2,i3,uc)*ux23(i1,i2,i3,vc)
                        agu(vc,vc)=uu(i1,i2,i3,vc)*uy23(i1,i2,i3,vc)
                     ut(i1,i2,i3,uc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,
     & uc)-uu(i1,i2,i3,vc)*uy23(i1,i2,i3,uc)-uu(i1,i2,i3,wc)*uz23(i1,
     & i2,i3,uc)-ux23(i1,i2,i3,pc)+nu*ulaplacian23(i1,i2,i3,uc)
                     ut(i1,i2,i3,vc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,
     & vc)-uu(i1,i2,i3,vc)*uy23(i1,i2,i3,vc)-uu(i1,i2,i3,wc)*uz23(i1,
     & i2,i3,vc)-uy23(i1,i2,i3,pc)+nu*ulaplacian23(i1,i2,i3,vc)
                     ut(i1,i2,i3,wc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,
     & wc)-uu(i1,i2,i3,vc)*uy23(i1,i2,i3,wc)-uu(i1,i2,i3,wc)*uz23(i1,
     & i2,i3,wc)-uz23(i1,i2,i3,pc)+nu*ulaplacian23(i1,i2,i3,wc)
                end if
               end do
               end do
               end do
              else
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                 ! INS, no AD
                   ! --- upwind approximations ---
                     ! --- upwind scheme ---
                     ! for testing output this next message:
                     if( t.le. 0. )then
                       write(*,'(" getAdvection upwind scheme (7)")')
                     end if
                       ! --- CURVILINEAR GRID ---
                     !  #If "3" eq "2"
                     !    au = rsxy(i1,i2,i3,0,0)*u(i1,i2,i3,uc)+rsxy(i1,i2,i3,0,1)*u(i1,i2,i3,vc)
                     !  #Else
                     !    au = rsxy(i1,i2,i3,0,0)*u(i1,i2,i3,uc)+rsxy(i1,i2,i3,0,1)*u(i1,i2,i3,vc)+rsxy(i1,i2,i3,0,2)*u(i1,i2,i3,wc)
                     !  #End
                     !
                     !  if( au.gt.0. )then
                     !    agu(uc,uc)= au*(u(i1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(dr(0))
                     !  else
                     !    agu(uc,uc)= au*(u(i1+1,i2,i3,uc)-u(i1,i2,i3,uc))/(dr(0))
                     !  end if
                       agu(uc,uc)=uu(i1,i2,i3,uc)*ux23(i1,i2,i3,uc)
                       agu(vc,uc)=uu(i1,i2,i3,vc)*uy23(i1,i2,i3,uc)
                       agu(uc,vc)=uu(i1,i2,i3,uc)*ux23(i1,i2,i3,vc)
                       agu(vc,vc)=uu(i1,i2,i3,vc)*uy23(i1,i2,i3,vc)
                    ut(i1,i2,i3,uc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,uc)
     & -uu(i1,i2,i3,vc)*uy23(i1,i2,i3,uc)-uu(i1,i2,i3,wc)*uz23(i1,i2,
     & i3,uc)-ux23(i1,i2,i3,pc)+nu*ulaplacian23(i1,i2,i3,uc)
                    ut(i1,i2,i3,vc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,vc)
     & -uu(i1,i2,i3,vc)*uy23(i1,i2,i3,vc)-uu(i1,i2,i3,wc)*uz23(i1,i2,
     & i3,vc)-uy23(i1,i2,i3,pc)+nu*ulaplacian23(i1,i2,i3,vc)
                    ut(i1,i2,i3,wc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,wc)
     & -uu(i1,i2,i3,vc)*uy23(i1,i2,i3,wc)-uu(i1,i2,i3,wc)*uz23(i1,i2,
     & i3,wc)-uz23(i1,i2,i3,pc)+nu*ulaplacian23(i1,i2,i3,wc)
               end do
               end do
               end do
              end if
            else if( advectionOption.eq.bwenoAdvection )then
              ! --- bweno ---
              if( useWhereMask.ne.0 )then
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                if( mask(i1,i2,i3).gt.0 )then
                  ! INS, no AD
                    ! --- upwind approximations ---
                      ! --- upwind scheme ---
                      ! for testing output this next message:
                      if( t.le. 0. )then
                        write(*,'(" getAdvection upwind scheme (7)")')
                      end if
                        ! --- CURVILINEAR GRID ---
                      !  #If "3" eq "2"
                      !    au = rsxy(i1,i2,i3,0,0)*u(i1,i2,i3,uc)+rsxy(i1,i2,i3,0,1)*u(i1,i2,i3,vc)
                      !  #Else
                      !    au = rsxy(i1,i2,i3,0,0)*u(i1,i2,i3,uc)+rsxy(i1,i2,i3,0,1)*u(i1,i2,i3,vc)+rsxy(i1,i2,i3,0,2)*u(i1,i2,i3,wc)
                      !  #End
                      !
                      !  if( au.gt.0. )then
                      !    agu(uc,uc)= au*(u(i1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(dr(0))
                      !  else
                      !    agu(uc,uc)= au*(u(i1+1,i2,i3,uc)-u(i1,i2,i3,uc))/(dr(0))
                      !  end if
                        agu(uc,uc)=uu(i1,i2,i3,uc)*ux23(i1,i2,i3,uc)
                        agu(vc,uc)=uu(i1,i2,i3,vc)*uy23(i1,i2,i3,uc)
                        agu(uc,vc)=uu(i1,i2,i3,uc)*ux23(i1,i2,i3,vc)
                        agu(vc,vc)=uu(i1,i2,i3,vc)*uy23(i1,i2,i3,vc)
                     ut(i1,i2,i3,uc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,
     & uc)-uu(i1,i2,i3,vc)*uy23(i1,i2,i3,uc)-uu(i1,i2,i3,wc)*uz23(i1,
     & i2,i3,uc)-ux23(i1,i2,i3,pc)+nu*ulaplacian23(i1,i2,i3,uc)
                     ut(i1,i2,i3,vc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,
     & vc)-uu(i1,i2,i3,vc)*uy23(i1,i2,i3,vc)-uu(i1,i2,i3,wc)*uz23(i1,
     & i2,i3,vc)-uy23(i1,i2,i3,pc)+nu*ulaplacian23(i1,i2,i3,vc)
                     ut(i1,i2,i3,wc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,
     & wc)-uu(i1,i2,i3,vc)*uy23(i1,i2,i3,wc)-uu(i1,i2,i3,wc)*uz23(i1,
     & i2,i3,wc)-uz23(i1,i2,i3,pc)+nu*ulaplacian23(i1,i2,i3,wc)
                end if
               end do
               end do
               end do
              else
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                 ! INS, no AD
                   ! --- upwind approximations ---
                     ! --- upwind scheme ---
                     ! for testing output this next message:
                     if( t.le. 0. )then
                       write(*,'(" getAdvection upwind scheme (7)")')
                     end if
                       ! --- CURVILINEAR GRID ---
                     !  #If "3" eq "2"
                     !    au = rsxy(i1,i2,i3,0,0)*u(i1,i2,i3,uc)+rsxy(i1,i2,i3,0,1)*u(i1,i2,i3,vc)
                     !  #Else
                     !    au = rsxy(i1,i2,i3,0,0)*u(i1,i2,i3,uc)+rsxy(i1,i2,i3,0,1)*u(i1,i2,i3,vc)+rsxy(i1,i2,i3,0,2)*u(i1,i2,i3,wc)
                     !  #End
                     !
                     !  if( au.gt.0. )then
                     !    agu(uc,uc)= au*(u(i1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(dr(0))
                     !  else
                     !    agu(uc,uc)= au*(u(i1+1,i2,i3,uc)-u(i1,i2,i3,uc))/(dr(0))
                     !  end if
                       agu(uc,uc)=uu(i1,i2,i3,uc)*ux23(i1,i2,i3,uc)
                       agu(vc,uc)=uu(i1,i2,i3,vc)*uy23(i1,i2,i3,uc)
                       agu(uc,vc)=uu(i1,i2,i3,uc)*ux23(i1,i2,i3,vc)
                       agu(vc,vc)=uu(i1,i2,i3,vc)*uy23(i1,i2,i3,vc)
                    ut(i1,i2,i3,uc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,uc)
     & -uu(i1,i2,i3,vc)*uy23(i1,i2,i3,uc)-uu(i1,i2,i3,wc)*uz23(i1,i2,
     & i3,uc)-ux23(i1,i2,i3,pc)+nu*ulaplacian23(i1,i2,i3,uc)
                    ut(i1,i2,i3,vc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,vc)
     & -uu(i1,i2,i3,vc)*uy23(i1,i2,i3,vc)-uu(i1,i2,i3,wc)*uz23(i1,i2,
     & i3,vc)-uy23(i1,i2,i3,pc)+nu*ulaplacian23(i1,i2,i3,vc)
                    ut(i1,i2,i3,wc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,wc)
     & -uu(i1,i2,i3,vc)*uy23(i1,i2,i3,wc)-uu(i1,i2,i3,wc)*uz23(i1,i2,
     & i3,wc)-uz23(i1,i2,i3,pc)+nu*ulaplacian23(i1,i2,i3,wc)
               end do
               end do
               end do
              end if
            else
              write(*,'(" unknown advectionOption")')
              stop 1010
            end if
           else ! gridIsImplicit
            ! ---- implicit time-stepping ---
            if( advectionOption.eq.centeredAdvection )then
             if( implicitOption .eq.computeImplicitTermsSeparately )
     & then
               if( useWhereMask.ne.0 )then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                 if( mask(i1,i2,i3).gt.0 )then
                    ! explicit terms only, no diffusion
                     ut(i1,i2,i3,uc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,
     & uc)-uu(i1,i2,i3,vc)*uy23(i1,i2,i3,uc)-uu(i1,i2,i3,wc)*uz23(i1,
     & i2,i3,uc)-ux23(i1,i2,i3,pc)
                     ut(i1,i2,i3,vc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,
     & vc)-uu(i1,i2,i3,vc)*uy23(i1,i2,i3,vc)-uu(i1,i2,i3,wc)*uz23(i1,
     & i2,i3,vc)-uy23(i1,i2,i3,pc)
                     ut(i1,i2,i3,wc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,
     & wc)-uu(i1,i2,i3,vc)*uy23(i1,i2,i3,wc)-uu(i1,i2,i3,wc)*uz23(i1,
     & i2,i3,wc)-uz23(i1,i2,i3,pc)
                    ! include implicit terms - diffusion
                     uti(i1,i2,i3,uc)= nu*ulaplacian23(i1,i2,i3,uc)
                     uti(i1,i2,i3,vc)= nu*ulaplacian23(i1,i2,i3,vc)
                     uti(i1,i2,i3,wc)= nu*ulaplacian23(i1,i2,i3,wc)
                 end if
                end do
                end do
                end do
               else
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                   ! explicit terms only, no diffusion
                    ut(i1,i2,i3,uc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,uc)
     & -uu(i1,i2,i3,vc)*uy23(i1,i2,i3,uc)-uu(i1,i2,i3,wc)*uz23(i1,i2,
     & i3,uc)-ux23(i1,i2,i3,pc)
                    ut(i1,i2,i3,vc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,vc)
     & -uu(i1,i2,i3,vc)*uy23(i1,i2,i3,vc)-uu(i1,i2,i3,wc)*uz23(i1,i2,
     & i3,vc)-uy23(i1,i2,i3,pc)
                    ut(i1,i2,i3,wc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,wc)
     & -uu(i1,i2,i3,vc)*uy23(i1,i2,i3,wc)-uu(i1,i2,i3,wc)*uz23(i1,i2,
     & i3,wc)-uz23(i1,i2,i3,pc)
                   ! include implicit terms - diffusion
                    uti(i1,i2,i3,uc)= nu*ulaplacian23(i1,i2,i3,uc)
                    uti(i1,i2,i3,vc)= nu*ulaplacian23(i1,i2,i3,vc)
                    uti(i1,i2,i3,wc)= nu*ulaplacian23(i1,i2,i3,wc)
                end do
                end do
                end do
               end if
             else if( implicitOption.eq.doNotComputeImplicitTerms )then
               if( useWhereMask.ne.0 )then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                 if( mask(i1,i2,i3).gt.0 )then
                    ! explicit terms only, no diffusion
                     ut(i1,i2,i3,uc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,
     & uc)-uu(i1,i2,i3,vc)*uy23(i1,i2,i3,uc)-uu(i1,i2,i3,wc)*uz23(i1,
     & i2,i3,uc)-ux23(i1,i2,i3,pc)
                     ut(i1,i2,i3,vc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,
     & vc)-uu(i1,i2,i3,vc)*uy23(i1,i2,i3,vc)-uu(i1,i2,i3,wc)*uz23(i1,
     & i2,i3,vc)-uy23(i1,i2,i3,pc)
                     ut(i1,i2,i3,wc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,
     & wc)-uu(i1,i2,i3,vc)*uy23(i1,i2,i3,wc)-uu(i1,i2,i3,wc)*uz23(i1,
     & i2,i3,wc)-uz23(i1,i2,i3,pc)
                 end if
                end do
                end do
                end do
               else
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                   ! explicit terms only, no diffusion
                    ut(i1,i2,i3,uc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,uc)
     & -uu(i1,i2,i3,vc)*uy23(i1,i2,i3,uc)-uu(i1,i2,i3,wc)*uz23(i1,i2,
     & i3,uc)-ux23(i1,i2,i3,pc)
                    ut(i1,i2,i3,vc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,vc)
     & -uu(i1,i2,i3,vc)*uy23(i1,i2,i3,vc)-uu(i1,i2,i3,wc)*uz23(i1,i2,
     & i3,vc)-uy23(i1,i2,i3,pc)
                    ut(i1,i2,i3,wc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,wc)
     & -uu(i1,i2,i3,vc)*uy23(i1,i2,i3,wc)-uu(i1,i2,i3,wc)*uz23(i1,i2,
     & i3,wc)-uz23(i1,i2,i3,pc)
                end do
                end do
                end do
               end if
             else
              write(*,*)'insdt: Unknown implicitOption=',implicitOption
              stop 5
             end if  ! end implicitOption
            else if( advectionOption.eq.upwindAdvection )then
              ! --- upwind ---
             if( implicitOption .eq.computeImplicitTermsSeparately )
     & then
               if( useWhereMask.ne.0 )then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                 if( mask(i1,i2,i3).gt.0 )then
                    ! explicit terms only, no diffusion
                     ut(i1,i2,i3,uc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,
     & uc)-uu(i1,i2,i3,vc)*uy23(i1,i2,i3,uc)-uu(i1,i2,i3,wc)*uz23(i1,
     & i2,i3,uc)-ux23(i1,i2,i3,pc)
                     ut(i1,i2,i3,vc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,
     & vc)-uu(i1,i2,i3,vc)*uy23(i1,i2,i3,vc)-uu(i1,i2,i3,wc)*uz23(i1,
     & i2,i3,vc)-uy23(i1,i2,i3,pc)
                     ut(i1,i2,i3,wc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,
     & wc)-uu(i1,i2,i3,vc)*uy23(i1,i2,i3,wc)-uu(i1,i2,i3,wc)*uz23(i1,
     & i2,i3,wc)-uz23(i1,i2,i3,pc)
                    ! include implicit terms - diffusion
                     uti(i1,i2,i3,uc)= nu*ulaplacian23(i1,i2,i3,uc)
                     uti(i1,i2,i3,vc)= nu*ulaplacian23(i1,i2,i3,vc)
                     uti(i1,i2,i3,wc)= nu*ulaplacian23(i1,i2,i3,wc)
                 end if
                end do
                end do
                end do
               else
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                   ! explicit terms only, no diffusion
                    ut(i1,i2,i3,uc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,uc)
     & -uu(i1,i2,i3,vc)*uy23(i1,i2,i3,uc)-uu(i1,i2,i3,wc)*uz23(i1,i2,
     & i3,uc)-ux23(i1,i2,i3,pc)
                    ut(i1,i2,i3,vc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,vc)
     & -uu(i1,i2,i3,vc)*uy23(i1,i2,i3,vc)-uu(i1,i2,i3,wc)*uz23(i1,i2,
     & i3,vc)-uy23(i1,i2,i3,pc)
                    ut(i1,i2,i3,wc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,wc)
     & -uu(i1,i2,i3,vc)*uy23(i1,i2,i3,wc)-uu(i1,i2,i3,wc)*uz23(i1,i2,
     & i3,wc)-uz23(i1,i2,i3,pc)
                   ! include implicit terms - diffusion
                    uti(i1,i2,i3,uc)= nu*ulaplacian23(i1,i2,i3,uc)
                    uti(i1,i2,i3,vc)= nu*ulaplacian23(i1,i2,i3,vc)
                    uti(i1,i2,i3,wc)= nu*ulaplacian23(i1,i2,i3,wc)
                end do
                end do
                end do
               end if
             else if( implicitOption.eq.doNotComputeImplicitTerms )then
               if( useWhereMask.ne.0 )then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                 if( mask(i1,i2,i3).gt.0 )then
                    ! explicit terms only, no diffusion
                     ut(i1,i2,i3,uc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,
     & uc)-uu(i1,i2,i3,vc)*uy23(i1,i2,i3,uc)-uu(i1,i2,i3,wc)*uz23(i1,
     & i2,i3,uc)-ux23(i1,i2,i3,pc)
                     ut(i1,i2,i3,vc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,
     & vc)-uu(i1,i2,i3,vc)*uy23(i1,i2,i3,vc)-uu(i1,i2,i3,wc)*uz23(i1,
     & i2,i3,vc)-uy23(i1,i2,i3,pc)
                     ut(i1,i2,i3,wc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,
     & wc)-uu(i1,i2,i3,vc)*uy23(i1,i2,i3,wc)-uu(i1,i2,i3,wc)*uz23(i1,
     & i2,i3,wc)-uz23(i1,i2,i3,pc)
                 end if
                end do
                end do
                end do
               else
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                   ! explicit terms only, no diffusion
                    ut(i1,i2,i3,uc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,uc)
     & -uu(i1,i2,i3,vc)*uy23(i1,i2,i3,uc)-uu(i1,i2,i3,wc)*uz23(i1,i2,
     & i3,uc)-ux23(i1,i2,i3,pc)
                    ut(i1,i2,i3,vc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,vc)
     & -uu(i1,i2,i3,vc)*uy23(i1,i2,i3,vc)-uu(i1,i2,i3,wc)*uz23(i1,i2,
     & i3,vc)-uy23(i1,i2,i3,pc)
                    ut(i1,i2,i3,wc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,wc)
     & -uu(i1,i2,i3,vc)*uy23(i1,i2,i3,wc)-uu(i1,i2,i3,wc)*uz23(i1,i2,
     & i3,wc)-uz23(i1,i2,i3,pc)
                end do
                end do
                end do
               end if
             else
              write(*,*)'insdt: Unknown implicitOption=',implicitOption
              stop 6
             end if  ! end implicitOption
            else if( advectionOption.eq.bwenoAdvection )then
              ! --- bweno ---
             if( implicitOption .eq.computeImplicitTermsSeparately )
     & then
               if( useWhereMask.ne.0 )then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                 if( mask(i1,i2,i3).gt.0 )then
                    ! explicit terms only, no diffusion
                     ut(i1,i2,i3,uc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,
     & uc)-uu(i1,i2,i3,vc)*uy23(i1,i2,i3,uc)-uu(i1,i2,i3,wc)*uz23(i1,
     & i2,i3,uc)-ux23(i1,i2,i3,pc)
                     ut(i1,i2,i3,vc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,
     & vc)-uu(i1,i2,i3,vc)*uy23(i1,i2,i3,vc)-uu(i1,i2,i3,wc)*uz23(i1,
     & i2,i3,vc)-uy23(i1,i2,i3,pc)
                     ut(i1,i2,i3,wc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,
     & wc)-uu(i1,i2,i3,vc)*uy23(i1,i2,i3,wc)-uu(i1,i2,i3,wc)*uz23(i1,
     & i2,i3,wc)-uz23(i1,i2,i3,pc)
                    ! include implicit terms - diffusion
                     uti(i1,i2,i3,uc)= nu*ulaplacian23(i1,i2,i3,uc)
                     uti(i1,i2,i3,vc)= nu*ulaplacian23(i1,i2,i3,vc)
                     uti(i1,i2,i3,wc)= nu*ulaplacian23(i1,i2,i3,wc)
                 end if
                end do
                end do
                end do
               else
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                   ! explicit terms only, no diffusion
                    ut(i1,i2,i3,uc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,uc)
     & -uu(i1,i2,i3,vc)*uy23(i1,i2,i3,uc)-uu(i1,i2,i3,wc)*uz23(i1,i2,
     & i3,uc)-ux23(i1,i2,i3,pc)
                    ut(i1,i2,i3,vc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,vc)
     & -uu(i1,i2,i3,vc)*uy23(i1,i2,i3,vc)-uu(i1,i2,i3,wc)*uz23(i1,i2,
     & i3,vc)-uy23(i1,i2,i3,pc)
                    ut(i1,i2,i3,wc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,wc)
     & -uu(i1,i2,i3,vc)*uy23(i1,i2,i3,wc)-uu(i1,i2,i3,wc)*uz23(i1,i2,
     & i3,wc)-uz23(i1,i2,i3,pc)
                   ! include implicit terms - diffusion
                    uti(i1,i2,i3,uc)= nu*ulaplacian23(i1,i2,i3,uc)
                    uti(i1,i2,i3,vc)= nu*ulaplacian23(i1,i2,i3,vc)
                    uti(i1,i2,i3,wc)= nu*ulaplacian23(i1,i2,i3,wc)
                end do
                end do
                end do
               end if
             else if( implicitOption.eq.doNotComputeImplicitTerms )then
               if( useWhereMask.ne.0 )then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                 if( mask(i1,i2,i3).gt.0 )then
                    ! explicit terms only, no diffusion
                     ut(i1,i2,i3,uc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,
     & uc)-uu(i1,i2,i3,vc)*uy23(i1,i2,i3,uc)-uu(i1,i2,i3,wc)*uz23(i1,
     & i2,i3,uc)-ux23(i1,i2,i3,pc)
                     ut(i1,i2,i3,vc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,
     & vc)-uu(i1,i2,i3,vc)*uy23(i1,i2,i3,vc)-uu(i1,i2,i3,wc)*uz23(i1,
     & i2,i3,vc)-uy23(i1,i2,i3,pc)
                     ut(i1,i2,i3,wc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,
     & wc)-uu(i1,i2,i3,vc)*uy23(i1,i2,i3,wc)-uu(i1,i2,i3,wc)*uz23(i1,
     & i2,i3,wc)-uz23(i1,i2,i3,pc)
                 end if
                end do
                end do
                end do
               else
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                   ! explicit terms only, no diffusion
                    ut(i1,i2,i3,uc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,uc)
     & -uu(i1,i2,i3,vc)*uy23(i1,i2,i3,uc)-uu(i1,i2,i3,wc)*uz23(i1,i2,
     & i3,uc)-ux23(i1,i2,i3,pc)
                    ut(i1,i2,i3,vc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,vc)
     & -uu(i1,i2,i3,vc)*uy23(i1,i2,i3,vc)-uu(i1,i2,i3,wc)*uz23(i1,i2,
     & i3,vc)-uy23(i1,i2,i3,pc)
                    ut(i1,i2,i3,wc)= -uu(i1,i2,i3,uc)*ux23(i1,i2,i3,wc)
     & -uu(i1,i2,i3,vc)*uy23(i1,i2,i3,wc)-uu(i1,i2,i3,wc)*uz23(i1,i2,
     & i3,wc)-uz23(i1,i2,i3,pc)
                end do
                end do
                end do
               end if
             else
              write(*,*)'insdt: Unknown implicitOption=',implicitOption
              stop 7
             end if  ! end implicitOption
            else
              write(*,'(" unknown advectionOption")')
              stop 1010
            end if
           end if
          else if( isAxisymmetric.eq.1 )then
           if( advectionOption.ne.centeredAdvection )then
             write(*,*) 'insdt.h : finish me for axisymmetric'
             stop 2020
           end if
          else
            stop 88733
          end if
         else
           stop 77
         end if
         return
         end
