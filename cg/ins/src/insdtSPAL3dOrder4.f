! This file automatically generated from insdtSPAL.bf with bpp.
        subroutine insdtSPAL3dOrder4(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,
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
        real cb1, cb2, cv1, sigma, sigmai, kappa, cw1, cw2, cw3, cw3e6,
     &  cv1e3, cd0, cr0
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
        d14(kd) = 1./(12.*dr(kd))
        d24(kd) = 1./(12.*dr(kd)**2)
        ur4(i1,i2,i3,kd)=(8.*(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))-(u(i1+
     & 2,i2,i3,kd)-u(i1-2,i2,i3,kd)))*d14(0)
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
        rxt4(i1,i2,i3)=(8.*(rx(i1,i2,i3+1)-rx(i1,i2,i3-1))-(rx(i1,i2,
     & i3+2)-rx(i1,i2,i3-2)))*d14(2)
        ryr4(i1,i2,i3)=(8.*(ry(i1+1,i2,i3)-ry(i1-1,i2,i3))-(ry(i1+2,i2,
     & i3)-ry(i1-2,i2,i3)))*d14(0)
        rys4(i1,i2,i3)=(8.*(ry(i1,i2+1,i3)-ry(i1,i2-1,i3))-(ry(i1,i2+2,
     & i3)-ry(i1,i2-2,i3)))*d14(1)
        ryt4(i1,i2,i3)=(8.*(ry(i1,i2,i3+1)-ry(i1,i2,i3-1))-(ry(i1,i2,
     & i3+2)-ry(i1,i2,i3-2)))*d14(2)
        rzr4(i1,i2,i3)=(8.*(rz(i1+1,i2,i3)-rz(i1-1,i2,i3))-(rz(i1+2,i2,
     & i3)-rz(i1-2,i2,i3)))*d14(0)
        rzs4(i1,i2,i3)=(8.*(rz(i1,i2+1,i3)-rz(i1,i2-1,i3))-(rz(i1,i2+2,
     & i3)-rz(i1,i2-2,i3)))*d14(1)
        rzt4(i1,i2,i3)=(8.*(rz(i1,i2,i3+1)-rz(i1,i2,i3-1))-(rz(i1,i2,
     & i3+2)-rz(i1,i2,i3-2)))*d14(2)
        sxr4(i1,i2,i3)=(8.*(sx(i1+1,i2,i3)-sx(i1-1,i2,i3))-(sx(i1+2,i2,
     & i3)-sx(i1-2,i2,i3)))*d14(0)
        sxs4(i1,i2,i3)=(8.*(sx(i1,i2+1,i3)-sx(i1,i2-1,i3))-(sx(i1,i2+2,
     & i3)-sx(i1,i2-2,i3)))*d14(1)
        sxt4(i1,i2,i3)=(8.*(sx(i1,i2,i3+1)-sx(i1,i2,i3-1))-(sx(i1,i2,
     & i3+2)-sx(i1,i2,i3-2)))*d14(2)
        syr4(i1,i2,i3)=(8.*(sy(i1+1,i2,i3)-sy(i1-1,i2,i3))-(sy(i1+2,i2,
     & i3)-sy(i1-2,i2,i3)))*d14(0)
        sys4(i1,i2,i3)=(8.*(sy(i1,i2+1,i3)-sy(i1,i2-1,i3))-(sy(i1,i2+2,
     & i3)-sy(i1,i2-2,i3)))*d14(1)
        syt4(i1,i2,i3)=(8.*(sy(i1,i2,i3+1)-sy(i1,i2,i3-1))-(sy(i1,i2,
     & i3+2)-sy(i1,i2,i3-2)))*d14(2)
        szr4(i1,i2,i3)=(8.*(sz(i1+1,i2,i3)-sz(i1-1,i2,i3))-(sz(i1+2,i2,
     & i3)-sz(i1-2,i2,i3)))*d14(0)
        szs4(i1,i2,i3)=(8.*(sz(i1,i2+1,i3)-sz(i1,i2-1,i3))-(sz(i1,i2+2,
     & i3)-sz(i1,i2-2,i3)))*d14(1)
        szt4(i1,i2,i3)=(8.*(sz(i1,i2,i3+1)-sz(i1,i2,i3-1))-(sz(i1,i2,
     & i3+2)-sz(i1,i2,i3-2)))*d14(2)
        txr4(i1,i2,i3)=(8.*(tx(i1+1,i2,i3)-tx(i1-1,i2,i3))-(tx(i1+2,i2,
     & i3)-tx(i1-2,i2,i3)))*d14(0)
        txs4(i1,i2,i3)=(8.*(tx(i1,i2+1,i3)-tx(i1,i2-1,i3))-(tx(i1,i2+2,
     & i3)-tx(i1,i2-2,i3)))*d14(1)
        txt4(i1,i2,i3)=(8.*(tx(i1,i2,i3+1)-tx(i1,i2,i3-1))-(tx(i1,i2,
     & i3+2)-tx(i1,i2,i3-2)))*d14(2)
        tyr4(i1,i2,i3)=(8.*(ty(i1+1,i2,i3)-ty(i1-1,i2,i3))-(ty(i1+2,i2,
     & i3)-ty(i1-2,i2,i3)))*d14(0)
        tys4(i1,i2,i3)=(8.*(ty(i1,i2+1,i3)-ty(i1,i2-1,i3))-(ty(i1,i2+2,
     & i3)-ty(i1,i2-2,i3)))*d14(1)
        tyt4(i1,i2,i3)=(8.*(ty(i1,i2,i3+1)-ty(i1,i2,i3-1))-(ty(i1,i2,
     & i3+2)-ty(i1,i2,i3-2)))*d14(2)
        tzr4(i1,i2,i3)=(8.*(tz(i1+1,i2,i3)-tz(i1-1,i2,i3))-(tz(i1+2,i2,
     & i3)-tz(i1-2,i2,i3)))*d14(0)
        tzs4(i1,i2,i3)=(8.*(tz(i1,i2+1,i3)-tz(i1,i2-1,i3))-(tz(i1,i2+2,
     & i3)-tz(i1,i2-2,i3)))*d14(1)
        tzt4(i1,i2,i3)=(8.*(tz(i1,i2,i3+1)-tz(i1,i2,i3-1))-(tz(i1,i2,
     & i3+2)-tz(i1,i2,i3-2)))*d14(2)
        ux41(i1,i2,i3,kd)= rx(i1,i2,i3)*ur4(i1,i2,i3,kd)
        uy41(i1,i2,i3,kd)=0
        uz41(i1,i2,i3,kd)=0
        ux42(i1,i2,i3,kd)= rx(i1,i2,i3)*ur4(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & us4(i1,i2,i3,kd)
        uy42(i1,i2,i3,kd)= ry(i1,i2,i3)*ur4(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & us4(i1,i2,i3,kd)
        uz42(i1,i2,i3,kd)=0
        ux43(i1,i2,i3,kd)=rx(i1,i2,i3)*ur4(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & us4(i1,i2,i3,kd)+tx(i1,i2,i3)*ut4(i1,i2,i3,kd)
        uy43(i1,i2,i3,kd)=ry(i1,i2,i3)*ur4(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & us4(i1,i2,i3,kd)+ty(i1,i2,i3)*ut4(i1,i2,i3,kd)
        uz43(i1,i2,i3,kd)=rz(i1,i2,i3)*ur4(i1,i2,i3,kd)+sz(i1,i2,i3)*
     & us4(i1,i2,i3,kd)+tz(i1,i2,i3)*ut4(i1,i2,i3,kd)
        rxx41(i1,i2,i3)= rx(i1,i2,i3)*rxr4(i1,i2,i3)
        rxx42(i1,i2,i3)= rx(i1,i2,i3)*rxr4(i1,i2,i3)+sx(i1,i2,i3)*rxs4(
     & i1,i2,i3)
        rxy42(i1,i2,i3)= ry(i1,i2,i3)*rxr4(i1,i2,i3)+sy(i1,i2,i3)*rxs4(
     & i1,i2,i3)
        rxx43(i1,i2,i3)=rx(i1,i2,i3)*rxr4(i1,i2,i3)+sx(i1,i2,i3)*rxs4(
     & i1,i2,i3)+tx(i1,i2,i3)*rxt4(i1,i2,i3)
        rxy43(i1,i2,i3)=ry(i1,i2,i3)*rxr4(i1,i2,i3)+sy(i1,i2,i3)*rxs4(
     & i1,i2,i3)+ty(i1,i2,i3)*rxt4(i1,i2,i3)
        rxz43(i1,i2,i3)=rz(i1,i2,i3)*rxr4(i1,i2,i3)+sz(i1,i2,i3)*rxs4(
     & i1,i2,i3)+tz(i1,i2,i3)*rxt4(i1,i2,i3)
        ryx42(i1,i2,i3)= rx(i1,i2,i3)*ryr4(i1,i2,i3)+sx(i1,i2,i3)*rys4(
     & i1,i2,i3)
        ryy42(i1,i2,i3)= ry(i1,i2,i3)*ryr4(i1,i2,i3)+sy(i1,i2,i3)*rys4(
     & i1,i2,i3)
        ryx43(i1,i2,i3)=rx(i1,i2,i3)*ryr4(i1,i2,i3)+sx(i1,i2,i3)*rys4(
     & i1,i2,i3)+tx(i1,i2,i3)*ryt4(i1,i2,i3)
        ryy43(i1,i2,i3)=ry(i1,i2,i3)*ryr4(i1,i2,i3)+sy(i1,i2,i3)*rys4(
     & i1,i2,i3)+ty(i1,i2,i3)*ryt4(i1,i2,i3)
        ryz43(i1,i2,i3)=rz(i1,i2,i3)*ryr4(i1,i2,i3)+sz(i1,i2,i3)*rys4(
     & i1,i2,i3)+tz(i1,i2,i3)*ryt4(i1,i2,i3)
        rzx42(i1,i2,i3)= rx(i1,i2,i3)*rzr4(i1,i2,i3)+sx(i1,i2,i3)*rzs4(
     & i1,i2,i3)
        rzy42(i1,i2,i3)= ry(i1,i2,i3)*rzr4(i1,i2,i3)+sy(i1,i2,i3)*rzs4(
     & i1,i2,i3)
        rzx43(i1,i2,i3)=rx(i1,i2,i3)*rzr4(i1,i2,i3)+sx(i1,i2,i3)*rzs4(
     & i1,i2,i3)+tx(i1,i2,i3)*rzt4(i1,i2,i3)
        rzy43(i1,i2,i3)=ry(i1,i2,i3)*rzr4(i1,i2,i3)+sy(i1,i2,i3)*rzs4(
     & i1,i2,i3)+ty(i1,i2,i3)*rzt4(i1,i2,i3)
        rzz43(i1,i2,i3)=rz(i1,i2,i3)*rzr4(i1,i2,i3)+sz(i1,i2,i3)*rzs4(
     & i1,i2,i3)+tz(i1,i2,i3)*rzt4(i1,i2,i3)
        sxx42(i1,i2,i3)= rx(i1,i2,i3)*sxr4(i1,i2,i3)+sx(i1,i2,i3)*sxs4(
     & i1,i2,i3)
        sxy42(i1,i2,i3)= ry(i1,i2,i3)*sxr4(i1,i2,i3)+sy(i1,i2,i3)*sxs4(
     & i1,i2,i3)
        sxx43(i1,i2,i3)=rx(i1,i2,i3)*sxr4(i1,i2,i3)+sx(i1,i2,i3)*sxs4(
     & i1,i2,i3)+tx(i1,i2,i3)*sxt4(i1,i2,i3)
        sxy43(i1,i2,i3)=ry(i1,i2,i3)*sxr4(i1,i2,i3)+sy(i1,i2,i3)*sxs4(
     & i1,i2,i3)+ty(i1,i2,i3)*sxt4(i1,i2,i3)
        sxz43(i1,i2,i3)=rz(i1,i2,i3)*sxr4(i1,i2,i3)+sz(i1,i2,i3)*sxs4(
     & i1,i2,i3)+tz(i1,i2,i3)*sxt4(i1,i2,i3)
        syx42(i1,i2,i3)= rx(i1,i2,i3)*syr4(i1,i2,i3)+sx(i1,i2,i3)*sys4(
     & i1,i2,i3)
        syy42(i1,i2,i3)= ry(i1,i2,i3)*syr4(i1,i2,i3)+sy(i1,i2,i3)*sys4(
     & i1,i2,i3)
        syx43(i1,i2,i3)=rx(i1,i2,i3)*syr4(i1,i2,i3)+sx(i1,i2,i3)*sys4(
     & i1,i2,i3)+tx(i1,i2,i3)*syt4(i1,i2,i3)
        syy43(i1,i2,i3)=ry(i1,i2,i3)*syr4(i1,i2,i3)+sy(i1,i2,i3)*sys4(
     & i1,i2,i3)+ty(i1,i2,i3)*syt4(i1,i2,i3)
        syz43(i1,i2,i3)=rz(i1,i2,i3)*syr4(i1,i2,i3)+sz(i1,i2,i3)*sys4(
     & i1,i2,i3)+tz(i1,i2,i3)*syt4(i1,i2,i3)
        szx42(i1,i2,i3)= rx(i1,i2,i3)*szr4(i1,i2,i3)+sx(i1,i2,i3)*szs4(
     & i1,i2,i3)
        szy42(i1,i2,i3)= ry(i1,i2,i3)*szr4(i1,i2,i3)+sy(i1,i2,i3)*szs4(
     & i1,i2,i3)
        szx43(i1,i2,i3)=rx(i1,i2,i3)*szr4(i1,i2,i3)+sx(i1,i2,i3)*szs4(
     & i1,i2,i3)+tx(i1,i2,i3)*szt4(i1,i2,i3)
        szy43(i1,i2,i3)=ry(i1,i2,i3)*szr4(i1,i2,i3)+sy(i1,i2,i3)*szs4(
     & i1,i2,i3)+ty(i1,i2,i3)*szt4(i1,i2,i3)
        szz43(i1,i2,i3)=rz(i1,i2,i3)*szr4(i1,i2,i3)+sz(i1,i2,i3)*szs4(
     & i1,i2,i3)+tz(i1,i2,i3)*szt4(i1,i2,i3)
        txx42(i1,i2,i3)= rx(i1,i2,i3)*txr4(i1,i2,i3)+sx(i1,i2,i3)*txs4(
     & i1,i2,i3)
        txy42(i1,i2,i3)= ry(i1,i2,i3)*txr4(i1,i2,i3)+sy(i1,i2,i3)*txs4(
     & i1,i2,i3)
        txx43(i1,i2,i3)=rx(i1,i2,i3)*txr4(i1,i2,i3)+sx(i1,i2,i3)*txs4(
     & i1,i2,i3)+tx(i1,i2,i3)*txt4(i1,i2,i3)
        txy43(i1,i2,i3)=ry(i1,i2,i3)*txr4(i1,i2,i3)+sy(i1,i2,i3)*txs4(
     & i1,i2,i3)+ty(i1,i2,i3)*txt4(i1,i2,i3)
        txz43(i1,i2,i3)=rz(i1,i2,i3)*txr4(i1,i2,i3)+sz(i1,i2,i3)*txs4(
     & i1,i2,i3)+tz(i1,i2,i3)*txt4(i1,i2,i3)
        tyx42(i1,i2,i3)= rx(i1,i2,i3)*tyr4(i1,i2,i3)+sx(i1,i2,i3)*tys4(
     & i1,i2,i3)
        tyy42(i1,i2,i3)= ry(i1,i2,i3)*tyr4(i1,i2,i3)+sy(i1,i2,i3)*tys4(
     & i1,i2,i3)
        tyx43(i1,i2,i3)=rx(i1,i2,i3)*tyr4(i1,i2,i3)+sx(i1,i2,i3)*tys4(
     & i1,i2,i3)+tx(i1,i2,i3)*tyt4(i1,i2,i3)
        tyy43(i1,i2,i3)=ry(i1,i2,i3)*tyr4(i1,i2,i3)+sy(i1,i2,i3)*tys4(
     & i1,i2,i3)+ty(i1,i2,i3)*tyt4(i1,i2,i3)
        tyz43(i1,i2,i3)=rz(i1,i2,i3)*tyr4(i1,i2,i3)+sz(i1,i2,i3)*tys4(
     & i1,i2,i3)+tz(i1,i2,i3)*tyt4(i1,i2,i3)
        tzx42(i1,i2,i3)= rx(i1,i2,i3)*tzr4(i1,i2,i3)+sx(i1,i2,i3)*tzs4(
     & i1,i2,i3)
        tzy42(i1,i2,i3)= ry(i1,i2,i3)*tzr4(i1,i2,i3)+sy(i1,i2,i3)*tzs4(
     & i1,i2,i3)
        tzx43(i1,i2,i3)=rx(i1,i2,i3)*tzr4(i1,i2,i3)+sx(i1,i2,i3)*tzs4(
     & i1,i2,i3)+tx(i1,i2,i3)*tzt4(i1,i2,i3)
        tzy43(i1,i2,i3)=ry(i1,i2,i3)*tzr4(i1,i2,i3)+sy(i1,i2,i3)*tzs4(
     & i1,i2,i3)+ty(i1,i2,i3)*tzt4(i1,i2,i3)
        tzz43(i1,i2,i3)=rz(i1,i2,i3)*tzr4(i1,i2,i3)+sz(i1,i2,i3)*tzs4(
     & i1,i2,i3)+tz(i1,i2,i3)*tzt4(i1,i2,i3)
        uxx41(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*urr4(i1,i2,i3,kd)+(rxx42(
     & i1,i2,i3))*ur4(i1,i2,i3,kd)
        uyy41(i1,i2,i3,kd)=0
        uxy41(i1,i2,i3,kd)=0
        uxz41(i1,i2,i3,kd)=0
        uyz41(i1,i2,i3,kd)=0
        uzz41(i1,i2,i3,kd)=0
        ulaplacian41(i1,i2,i3,kd)=uxx41(i1,i2,i3,kd)
        uxx42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*urr4(i1,i2,i3,kd)+2.*(rx(
     & i1,i2,i3)*sx(i1,i2,i3))*urs4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2)*
     & uss4(i1,i2,i3,kd)+(rxx42(i1,i2,i3))*ur4(i1,i2,i3,kd)+(sxx42(i1,
     & i2,i3))*us4(i1,i2,i3,kd)
        uyy42(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*urr4(i1,i2,i3,kd)+2.*(ry(
     & i1,i2,i3)*sy(i1,i2,i3))*urs4(i1,i2,i3,kd)+(sy(i1,i2,i3)**2)*
     & uss4(i1,i2,i3,kd)+(ryy42(i1,i2,i3))*ur4(i1,i2,i3,kd)+(syy42(i1,
     & i2,i3))*us4(i1,i2,i3,kd)
        uxy42(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*urr4(i1,i2,i3,kd)+
     & (rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*urs4(i1,
     & i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*uss4(i1,i2,i3,kd)+rxy42(i1,
     & i2,i3)*ur4(i1,i2,i3,kd)+sxy42(i1,i2,i3)*us4(i1,i2,i3,kd)
        uxz42(i1,i2,i3,kd)=0
        uyz42(i1,i2,i3,kd)=0
        uzz42(i1,i2,i3,kd)=0
        ulaplacian42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & urr4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*
     & sy(i1,i2,i3))*urs4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)**
     & 2)*uss4(i1,i2,i3,kd)+(rxx42(i1,i2,i3)+ryy42(i1,i2,i3))*ur4(i1,
     & i2,i3,kd)+(sxx42(i1,i2,i3)+syy42(i1,i2,i3))*us4(i1,i2,i3,kd)
        uxx43(i1,i2,i3,kd)=rx(i1,i2,i3)**2*urr4(i1,i2,i3,kd)+sx(i1,i2,
     & i3)**2*uss4(i1,i2,i3,kd)+tx(i1,i2,i3)**2*utt4(i1,i2,i3,kd)+2.*
     & rx(i1,i2,i3)*sx(i1,i2,i3)*urs4(i1,i2,i3,kd)+2.*rx(i1,i2,i3)*tx(
     & i1,i2,i3)*urt4(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,i2,i3)*ust4(
     & i1,i2,i3,kd)+rxx43(i1,i2,i3)*ur4(i1,i2,i3,kd)+sxx43(i1,i2,i3)*
     & us4(i1,i2,i3,kd)+txx43(i1,i2,i3)*ut4(i1,i2,i3,kd)
        uyy43(i1,i2,i3,kd)=ry(i1,i2,i3)**2*urr4(i1,i2,i3,kd)+sy(i1,i2,
     & i3)**2*uss4(i1,i2,i3,kd)+ty(i1,i2,i3)**2*utt4(i1,i2,i3,kd)+2.*
     & ry(i1,i2,i3)*sy(i1,i2,i3)*urs4(i1,i2,i3,kd)+2.*ry(i1,i2,i3)*ty(
     & i1,i2,i3)*urt4(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,i2,i3)*ust4(
     & i1,i2,i3,kd)+ryy43(i1,i2,i3)*ur4(i1,i2,i3,kd)+syy43(i1,i2,i3)*
     & us4(i1,i2,i3,kd)+tyy43(i1,i2,i3)*ut4(i1,i2,i3,kd)
        uzz43(i1,i2,i3,kd)=rz(i1,i2,i3)**2*urr4(i1,i2,i3,kd)+sz(i1,i2,
     & i3)**2*uss4(i1,i2,i3,kd)+tz(i1,i2,i3)**2*utt4(i1,i2,i3,kd)+2.*
     & rz(i1,i2,i3)*sz(i1,i2,i3)*urs4(i1,i2,i3,kd)+2.*rz(i1,i2,i3)*tz(
     & i1,i2,i3)*urt4(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,i2,i3)*ust4(
     & i1,i2,i3,kd)+rzz43(i1,i2,i3)*ur4(i1,i2,i3,kd)+szz43(i1,i2,i3)*
     & us4(i1,i2,i3,kd)+tzz43(i1,i2,i3)*ut4(i1,i2,i3,kd)
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
        ulaplacian43(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*urr4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)**
     & 2+sz(i1,i2,i3)**2)*uss4(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+ty(i1,i2,
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
        ux43r(i1,i2,i3,kd)=(8.*(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))-(u(
     & i1+2,i2,i3,kd)-u(i1-2,i2,i3,kd)))*h41(0)
        uy43r(i1,i2,i3,kd)=(8.*(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))-(u(
     & i1,i2+2,i3,kd)-u(i1,i2-2,i3,kd)))*h41(1)
        uz43r(i1,i2,i3,kd)=(8.*(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))-(u(
     & i1,i2,i3+2,kd)-u(i1,i2,i3-2,kd)))*h41(2)
        uxx43r(i1,i2,i3,kd)=( -30.*u(i1,i2,i3,kd)+16.*(u(i1+1,i2,i3,kd)
     & +u(i1-1,i2,i3,kd))-(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd)) )*h42(0)
        uyy43r(i1,i2,i3,kd)=( -30.*u(i1,i2,i3,kd)+16.*(u(i1,i2+1,i3,kd)
     & +u(i1,i2-1,i3,kd))-(u(i1,i2+2,i3,kd)+u(i1,i2-2,i3,kd)) )*h42(1)
        uzz43r(i1,i2,i3,kd)=( -30.*u(i1,i2,i3,kd)+16.*(u(i1,i2,i3+1,kd)
     & +u(i1,i2,i3-1,kd))-(u(i1,i2,i3+2,kd)+u(i1,i2,i3-2,kd)) )*h42(2)
        uxy43r(i1,i2,i3,kd)=( (u(i1+2,i2+2,i3,kd)-u(i1-2,i2+2,i3,kd)- 
     & u(i1+2,i2-2,i3,kd)+u(i1-2,i2-2,i3,kd)) +8.*(u(i1-1,i2+2,i3,kd)-
     & u(i1-1,i2-2,i3,kd)-u(i1+1,i2+2,i3,kd)+u(i1+1,i2-2,i3,kd) +u(i1+
     & 2,i2-1,i3,kd)-u(i1-2,i2-1,i3,kd)-u(i1+2,i2+1,i3,kd)+u(i1-2,i2+
     & 1,i3,kd))+64.*(u(i1+1,i2+1,i3,kd)-u(i1-1,i2+1,i3,kd)- u(i1+1,
     & i2-1,i3,kd)+u(i1-1,i2-1,i3,kd)))*(h41(0)*h41(1))
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
       !    --- For 2nd order 2D artificial diffusion ---
        delta22(c)=    (u(i1+1,i2,i3,c)-4.*u(i1,i2,i3,c)+u(i1-1,i2,i3,
     & c)  +u(i1,i2+1,i3,c)                 +u(i1,i2-1,i3,c))
       !    --- For 2nd order 3D artificial diffusion ---
        delta23(c)= (u(i1+1,i2,i3,c)-6.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c)  
     &  +u(i1,i2+1,i3,c)                   +u(i1,i2-1,i3,c)  +u(i1,i2,
     & i3+1,c)                   +u(i1,i2,i3-1,c))
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
                 ! INS with Spalart-Allmaras turbulence model
                  u0x=ux43r(i1,i2,i3,uc)
                  u0y=uy43r(i1,i2,i3,uc)
                  v0x=ux43r(i1,i2,i3,vc)
                  v0y=uy43r(i1,i2,i3,vc)
                  u0z=uz43r(i1,i2,i3,uc)
                  v0z=uz43r(i1,i2,i3,vc)
                  w0x=ux43r(i1,i2,i3,wc)
                  w0y=uy43r(i1,i2,i3,wc)
                  w0z=uz43r(i1,i2,i3,wc)
                  n0=u(i1,i2,i3,nc)
                  chi=n0/nu
                  chi3=chi**3
                  fnu1=chi3/( chi3+cv1e3)
                  fnu2=1.-chi/(1.+chi*fnu1)
                  dd = dw(i1,i2,i3)+cd0
                  dKappaSq=(dd*kappa)**2
                   s=n0*fnu2/dKappaSq +sqrt( (u0y-v0x)**2 + (v0z-w0y)**
     & 2 + (w0x-u0z)**2 )
                  r= min( n0/( s*dKappaSq ), cr0 )
                  g=r+cw2*(r**6-r)
                  fw=g*( (1.+cw3e6)/(g**6+cw3e6) )**(1./6.)
                  nSqBydSq=cw1*fw*(n0/dd)**2
                  nuT = nu+n0*chi3/(chi3+cv1e3)
                  nuTd= chi3*(chi3+4.*cv1e3)/(chi3+cv1e3)**2
                  n0x=ux43r(i1,i2,i3,nc)
                  n0y=uy43r(i1,i2,i3,nc)
                  nuTx=n0x*nuTd
                  nuTy=n0y*nuTd
                    n0z=uz43r(i1,i2,i3,nc)
                    nuTz=n0z*nuTd
                  ut(i1,i2,i3,uc)= -uu(i1,i2,i3,uc)*u0x-uu(i1,i2,i3,vc)
     & *u0y-uu(i1,i2,i3,wc)*u0z-ux43r(i1,i2,i3,pc)+nuT*ulaplacian43r(
     & i1,i2,i3,uc)+nuTx*(2.*u0x    ) +nuTy*(u0y+v0x) +nuTz*(u0z+w0x)
                  ut(i1,i2,i3,vc)= -uu(i1,i2,i3,uc)*v0x-uu(i1,i2,i3,vc)
     & *v0y-uu(i1,i2,i3,wc)*v0z-uy43r(i1,i2,i3,pc)+nuT*ulaplacian43r(
     & i1,i2,i3,vc)+nuTx*(u0y+v0x) +nuTy*(2.*v0y) +nuTz*(v0z+w0y)
                  ut(i1,i2,i3,wc)= -uu(i1,i2,i3,uc)*w0x-uu(i1,i2,i3,vc)
     & *w0y-uu(i1,i2,i3,wc)*w0z-uz43r(i1,i2,i3,pc)+nuT*ulaplacian43r(
     & i1,i2,i3,wc)+nuTx*(u0z+w0x) +nuTy*(v0z+w0y) +nuTz*(2.*w0z)
                  ut(i1,i2,i3,nc)= -uu(i1,i2,i3,uc)*n0x-uu(i1,i2,i3,vc)
     & *n0y-uu(i1,i2,i3,wc)*n0z + cb1*s*u(i1,i2,i3,nc) + sigmai*(nu+u(
     & i1,i2,i3,nc))*(ulaplacian43r(i1,i2,i3,nc))+ ((1.+cb2)*sigmai)*(
     & n0x**2+n0y**2+n0z**2) - nSqBydSq
                ! end NO eq NO: 
               end if
              end do
              end do
              end do
             else
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
                ! INS with Spalart-Allmaras turbulence model
                 u0x=ux43r(i1,i2,i3,uc)
                 u0y=uy43r(i1,i2,i3,uc)
                 v0x=ux43r(i1,i2,i3,vc)
                 v0y=uy43r(i1,i2,i3,vc)
                 u0z=uz43r(i1,i2,i3,uc)
                 v0z=uz43r(i1,i2,i3,vc)
                 w0x=ux43r(i1,i2,i3,wc)
                 w0y=uy43r(i1,i2,i3,wc)
                 w0z=uz43r(i1,i2,i3,wc)
                 n0=u(i1,i2,i3,nc)
                 chi=n0/nu
                 chi3=chi**3
                 fnu1=chi3/( chi3+cv1e3)
                 fnu2=1.-chi/(1.+chi*fnu1)
                 dd = dw(i1,i2,i3)+cd0
                 dKappaSq=(dd*kappa)**2
                  s=n0*fnu2/dKappaSq +sqrt( (u0y-v0x)**2 + (v0z-w0y)**
     & 2 + (w0x-u0z)**2 )
                 r= min( n0/( s*dKappaSq ), cr0 )
                 g=r+cw2*(r**6-r)
                 fw=g*( (1.+cw3e6)/(g**6+cw3e6) )**(1./6.)
                 nSqBydSq=cw1*fw*(n0/dd)**2
                 nuT = nu+n0*chi3/(chi3+cv1e3)
                 nuTd= chi3*(chi3+4.*cv1e3)/(chi3+cv1e3)**2
                 n0x=ux43r(i1,i2,i3,nc)
                 n0y=uy43r(i1,i2,i3,nc)
                 nuTx=n0x*nuTd
                 nuTy=n0y*nuTd
                   n0z=uz43r(i1,i2,i3,nc)
                   nuTz=n0z*nuTd
                 ut(i1,i2,i3,uc)= -uu(i1,i2,i3,uc)*u0x-uu(i1,i2,i3,vc)*
     & u0y-uu(i1,i2,i3,wc)*u0z-ux43r(i1,i2,i3,pc)+nuT*ulaplacian43r(
     & i1,i2,i3,uc)+nuTx*(2.*u0x    ) +nuTy*(u0y+v0x) +nuTz*(u0z+w0x)
                 ut(i1,i2,i3,vc)= -uu(i1,i2,i3,uc)*v0x-uu(i1,i2,i3,vc)*
     & v0y-uu(i1,i2,i3,wc)*v0z-uy43r(i1,i2,i3,pc)+nuT*ulaplacian43r(
     & i1,i2,i3,vc)+nuTx*(u0y+v0x) +nuTy*(2.*v0y) +nuTz*(v0z+w0y)
                 ut(i1,i2,i3,wc)= -uu(i1,i2,i3,uc)*w0x-uu(i1,i2,i3,vc)*
     & w0y-uu(i1,i2,i3,wc)*w0z-uz43r(i1,i2,i3,pc)+nuT*ulaplacian43r(
     & i1,i2,i3,wc)+nuTx*(u0z+w0x) +nuTy*(v0z+w0y) +nuTz*(2.*w0z)
                 ut(i1,i2,i3,nc)= -uu(i1,i2,i3,uc)*n0x-uu(i1,i2,i3,vc)*
     & n0y-uu(i1,i2,i3,wc)*n0z + cb1*s*u(i1,i2,i3,nc) + sigmai*(nu+u(
     & i1,i2,i3,nc))*(ulaplacian43r(i1,i2,i3,nc))+ ((1.+cb2)*sigmai)*(
     & n0x**2+n0y**2+n0z**2) - nSqBydSq
               ! end NO eq NO: 
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
                  stop 6666
               end if
              end do
              end do
              end do
             else
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
                 stop 6666
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
                  stop 6666
               end if
              end do
              end do
              end do
             else
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
                 stop 6666
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
            if( implicitOption .eq.computeImplicitTermsSeparately )then
              if( useWhereMask.ne.0 )then
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                if( mask(i1,i2,i3).gt.0 )then
                  ! explicit terms only, no diffusion
                   ut(i1,i2,i3,uc)= -uu(i1,i2,i3,uc)*ux43r(i1,i2,i3,uc)
     & -uu(i1,i2,i3,vc)*uy43r(i1,i2,i3,uc)-uu(i1,i2,i3,wc)*uz43r(i1,
     & i2,i3,uc)-ux43r(i1,i2,i3,pc)
                   ut(i1,i2,i3,vc)= -uu(i1,i2,i3,uc)*ux43r(i1,i2,i3,vc)
     & -uu(i1,i2,i3,vc)*uy43r(i1,i2,i3,vc)-uu(i1,i2,i3,wc)*uz43r(i1,
     & i2,i3,vc)-uy43r(i1,i2,i3,pc)
                   ut(i1,i2,i3,wc)= -uu(i1,i2,i3,uc)*ux43r(i1,i2,i3,wc)
     & -uu(i1,i2,i3,vc)*uy43r(i1,i2,i3,wc)-uu(i1,i2,i3,wc)*uz43r(i1,
     & i2,i3,wc)-uz43r(i1,i2,i3,pc)
                  ! include implicit terms - diffusion
                   uti(i1,i2,i3,uc)= nu*ulaplacian43r(i1,i2,i3,uc)
                   uti(i1,i2,i3,vc)= nu*ulaplacian43r(i1,i2,i3,vc)
                   uti(i1,i2,i3,wc)= nu*ulaplacian43r(i1,i2,i3,wc)
                 ! end NO eq NO: 
                end if
               end do
               end do
               end do
              else
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                 ! explicit terms only, no diffusion
                  ut(i1,i2,i3,uc)= -uu(i1,i2,i3,uc)*ux43r(i1,i2,i3,uc)-
     & uu(i1,i2,i3,vc)*uy43r(i1,i2,i3,uc)-uu(i1,i2,i3,wc)*uz43r(i1,i2,
     & i3,uc)-ux43r(i1,i2,i3,pc)
                  ut(i1,i2,i3,vc)= -uu(i1,i2,i3,uc)*ux43r(i1,i2,i3,vc)-
     & uu(i1,i2,i3,vc)*uy43r(i1,i2,i3,vc)-uu(i1,i2,i3,wc)*uz43r(i1,i2,
     & i3,vc)-uy43r(i1,i2,i3,pc)
                  ut(i1,i2,i3,wc)= -uu(i1,i2,i3,uc)*ux43r(i1,i2,i3,wc)-
     & uu(i1,i2,i3,vc)*uy43r(i1,i2,i3,wc)-uu(i1,i2,i3,wc)*uz43r(i1,i2,
     & i3,wc)-uz43r(i1,i2,i3,pc)
                 ! include implicit terms - diffusion
                  uti(i1,i2,i3,uc)= nu*ulaplacian43r(i1,i2,i3,uc)
                  uti(i1,i2,i3,vc)= nu*ulaplacian43r(i1,i2,i3,vc)
                  uti(i1,i2,i3,wc)= nu*ulaplacian43r(i1,i2,i3,wc)
                ! end NO eq NO: 
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
                   ut(i1,i2,i3,uc)= -uu(i1,i2,i3,uc)*ux43r(i1,i2,i3,uc)
     & -uu(i1,i2,i3,vc)*uy43r(i1,i2,i3,uc)-uu(i1,i2,i3,wc)*uz43r(i1,
     & i2,i3,uc)-ux43r(i1,i2,i3,pc)
                   ut(i1,i2,i3,vc)= -uu(i1,i2,i3,uc)*ux43r(i1,i2,i3,vc)
     & -uu(i1,i2,i3,vc)*uy43r(i1,i2,i3,vc)-uu(i1,i2,i3,wc)*uz43r(i1,
     & i2,i3,vc)-uy43r(i1,i2,i3,pc)
                   ut(i1,i2,i3,wc)= -uu(i1,i2,i3,uc)*ux43r(i1,i2,i3,wc)
     & -uu(i1,i2,i3,vc)*uy43r(i1,i2,i3,wc)-uu(i1,i2,i3,wc)*uz43r(i1,
     & i2,i3,wc)-uz43r(i1,i2,i3,pc)
                 ! end NO eq NO: 
                end if
               end do
               end do
               end do
              else
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                 ! explicit terms only, no diffusion
                  ut(i1,i2,i3,uc)= -uu(i1,i2,i3,uc)*ux43r(i1,i2,i3,uc)-
     & uu(i1,i2,i3,vc)*uy43r(i1,i2,i3,uc)-uu(i1,i2,i3,wc)*uz43r(i1,i2,
     & i3,uc)-ux43r(i1,i2,i3,pc)
                  ut(i1,i2,i3,vc)= -uu(i1,i2,i3,uc)*ux43r(i1,i2,i3,vc)-
     & uu(i1,i2,i3,vc)*uy43r(i1,i2,i3,vc)-uu(i1,i2,i3,wc)*uz43r(i1,i2,
     & i3,vc)-uy43r(i1,i2,i3,pc)
                  ut(i1,i2,i3,wc)= -uu(i1,i2,i3,uc)*ux43r(i1,i2,i3,wc)-
     & uu(i1,i2,i3,vc)*uy43r(i1,i2,i3,wc)-uu(i1,i2,i3,wc)*uz43r(i1,i2,
     & i3,wc)-uz43r(i1,i2,i3,pc)
                ! end NO eq NO: 
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
            if( implicitOption .eq.computeImplicitTermsSeparately )then
              if( useWhereMask.ne.0 )then
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                if( mask(i1,i2,i3).gt.0 )then
                   stop 6666
                end if
               end do
               end do
               end do
              else
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                  stop 6666
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
                   stop 6666
                end if
               end do
               end do
               end do
              else
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                  stop 6666
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
            if( implicitOption .eq.computeImplicitTermsSeparately )then
              if( useWhereMask.ne.0 )then
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                if( mask(i1,i2,i3).gt.0 )then
                   stop 6666
                end if
               end do
               end do
               end do
              else
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                  stop 6666
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
                   stop 6666
                end if
               end do
               end do
               end do
              else
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                  stop 6666
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
                 ! INS with Spalart-Allmaras turbulence model
                  u0x=ux43(i1,i2,i3,uc)
                  u0y=uy43(i1,i2,i3,uc)
                  v0x=ux43(i1,i2,i3,vc)
                  v0y=uy43(i1,i2,i3,vc)
                  u0z=uz43(i1,i2,i3,uc)
                  v0z=uz43(i1,i2,i3,vc)
                  w0x=ux43(i1,i2,i3,wc)
                  w0y=uy43(i1,i2,i3,wc)
                  w0z=uz43(i1,i2,i3,wc)
                  n0=u(i1,i2,i3,nc)
                  chi=n0/nu
                  chi3=chi**3
                  fnu1=chi3/( chi3+cv1e3)
                  fnu2=1.-chi/(1.+chi*fnu1)
                  dd = dw(i1,i2,i3)+cd0
                  dKappaSq=(dd*kappa)**2
                   s=n0*fnu2/dKappaSq +sqrt( (u0y-v0x)**2 + (v0z-w0y)**
     & 2 + (w0x-u0z)**2 )
                  r= min( n0/( s*dKappaSq ), cr0 )
                  g=r+cw2*(r**6-r)
                  fw=g*( (1.+cw3e6)/(g**6+cw3e6) )**(1./6.)
                  nSqBydSq=cw1*fw*(n0/dd)**2
                  nuT = nu+n0*chi3/(chi3+cv1e3)
                  nuTd= chi3*(chi3+4.*cv1e3)/(chi3+cv1e3)**2
                  n0x=ux43(i1,i2,i3,nc)
                  n0y=uy43(i1,i2,i3,nc)
                  nuTx=n0x*nuTd
                  nuTy=n0y*nuTd
                    n0z=uz43(i1,i2,i3,nc)
                    nuTz=n0z*nuTd
                  ut(i1,i2,i3,uc)= -uu(i1,i2,i3,uc)*u0x-uu(i1,i2,i3,vc)
     & *u0y-uu(i1,i2,i3,wc)*u0z-ux43(i1,i2,i3,pc)+nuT*ulaplacian43(i1,
     & i2,i3,uc)+nuTx*(2.*u0x    ) +nuTy*(u0y+v0x) +nuTz*(u0z+w0x)
                  ut(i1,i2,i3,vc)= -uu(i1,i2,i3,uc)*v0x-uu(i1,i2,i3,vc)
     & *v0y-uu(i1,i2,i3,wc)*v0z-uy43(i1,i2,i3,pc)+nuT*ulaplacian43(i1,
     & i2,i3,vc)+nuTx*(u0y+v0x) +nuTy*(2.*v0y) +nuTz*(v0z+w0y)
                  ut(i1,i2,i3,wc)= -uu(i1,i2,i3,uc)*w0x-uu(i1,i2,i3,vc)
     & *w0y-uu(i1,i2,i3,wc)*w0z-uz43(i1,i2,i3,pc)+nuT*ulaplacian43(i1,
     & i2,i3,wc)+nuTx*(u0z+w0x) +nuTy*(v0z+w0y) +nuTz*(2.*w0z)
                  ut(i1,i2,i3,nc)= -uu(i1,i2,i3,uc)*n0x-uu(i1,i2,i3,vc)
     & *n0y-uu(i1,i2,i3,wc)*n0z + cb1*s*u(i1,i2,i3,nc) + sigmai*(nu+u(
     & i1,i2,i3,nc))*(ulaplacian43(i1,i2,i3,nc))+ ((1.+cb2)*sigmai)*(
     & n0x**2+n0y**2+n0z**2) - nSqBydSq
                ! end NO eq NO: 
               end if
              end do
              end do
              end do
             else
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
                ! INS with Spalart-Allmaras turbulence model
                 u0x=ux43(i1,i2,i3,uc)
                 u0y=uy43(i1,i2,i3,uc)
                 v0x=ux43(i1,i2,i3,vc)
                 v0y=uy43(i1,i2,i3,vc)
                 u0z=uz43(i1,i2,i3,uc)
                 v0z=uz43(i1,i2,i3,vc)
                 w0x=ux43(i1,i2,i3,wc)
                 w0y=uy43(i1,i2,i3,wc)
                 w0z=uz43(i1,i2,i3,wc)
                 n0=u(i1,i2,i3,nc)
                 chi=n0/nu
                 chi3=chi**3
                 fnu1=chi3/( chi3+cv1e3)
                 fnu2=1.-chi/(1.+chi*fnu1)
                 dd = dw(i1,i2,i3)+cd0
                 dKappaSq=(dd*kappa)**2
                  s=n0*fnu2/dKappaSq +sqrt( (u0y-v0x)**2 + (v0z-w0y)**
     & 2 + (w0x-u0z)**2 )
                 r= min( n0/( s*dKappaSq ), cr0 )
                 g=r+cw2*(r**6-r)
                 fw=g*( (1.+cw3e6)/(g**6+cw3e6) )**(1./6.)
                 nSqBydSq=cw1*fw*(n0/dd)**2
                 nuT = nu+n0*chi3/(chi3+cv1e3)
                 nuTd= chi3*(chi3+4.*cv1e3)/(chi3+cv1e3)**2
                 n0x=ux43(i1,i2,i3,nc)
                 n0y=uy43(i1,i2,i3,nc)
                 nuTx=n0x*nuTd
                 nuTy=n0y*nuTd
                   n0z=uz43(i1,i2,i3,nc)
                   nuTz=n0z*nuTd
                 ut(i1,i2,i3,uc)= -uu(i1,i2,i3,uc)*u0x-uu(i1,i2,i3,vc)*
     & u0y-uu(i1,i2,i3,wc)*u0z-ux43(i1,i2,i3,pc)+nuT*ulaplacian43(i1,
     & i2,i3,uc)+nuTx*(2.*u0x    ) +nuTy*(u0y+v0x) +nuTz*(u0z+w0x)
                 ut(i1,i2,i3,vc)= -uu(i1,i2,i3,uc)*v0x-uu(i1,i2,i3,vc)*
     & v0y-uu(i1,i2,i3,wc)*v0z-uy43(i1,i2,i3,pc)+nuT*ulaplacian43(i1,
     & i2,i3,vc)+nuTx*(u0y+v0x) +nuTy*(2.*v0y) +nuTz*(v0z+w0y)
                 ut(i1,i2,i3,wc)= -uu(i1,i2,i3,uc)*w0x-uu(i1,i2,i3,vc)*
     & w0y-uu(i1,i2,i3,wc)*w0z-uz43(i1,i2,i3,pc)+nuT*ulaplacian43(i1,
     & i2,i3,wc)+nuTx*(u0z+w0x) +nuTy*(v0z+w0y) +nuTz*(2.*w0z)
                 ut(i1,i2,i3,nc)= -uu(i1,i2,i3,uc)*n0x-uu(i1,i2,i3,vc)*
     & n0y-uu(i1,i2,i3,wc)*n0z + cb1*s*u(i1,i2,i3,nc) + sigmai*(nu+u(
     & i1,i2,i3,nc))*(ulaplacian43(i1,i2,i3,nc))+ ((1.+cb2)*sigmai)*(
     & n0x**2+n0y**2+n0z**2) - nSqBydSq
               ! end NO eq NO: 
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
                  stop 6666
               end if
              end do
              end do
              end do
             else
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
                 stop 6666
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
                  stop 6666
               end if
              end do
              end do
              end do
             else
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
                 stop 6666
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
            if( implicitOption .eq.computeImplicitTermsSeparately )then
              if( useWhereMask.ne.0 )then
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                if( mask(i1,i2,i3).gt.0 )then
                  ! explicit terms only, no diffusion
                   ut(i1,i2,i3,uc)= -uu(i1,i2,i3,uc)*ux43(i1,i2,i3,uc)-
     & uu(i1,i2,i3,vc)*uy43(i1,i2,i3,uc)-uu(i1,i2,i3,wc)*uz43(i1,i2,
     & i3,uc)-ux43(i1,i2,i3,pc)
                   ut(i1,i2,i3,vc)= -uu(i1,i2,i3,uc)*ux43(i1,i2,i3,vc)-
     & uu(i1,i2,i3,vc)*uy43(i1,i2,i3,vc)-uu(i1,i2,i3,wc)*uz43(i1,i2,
     & i3,vc)-uy43(i1,i2,i3,pc)
                   ut(i1,i2,i3,wc)= -uu(i1,i2,i3,uc)*ux43(i1,i2,i3,wc)-
     & uu(i1,i2,i3,vc)*uy43(i1,i2,i3,wc)-uu(i1,i2,i3,wc)*uz43(i1,i2,
     & i3,wc)-uz43(i1,i2,i3,pc)
                  ! include implicit terms - diffusion
                   uti(i1,i2,i3,uc)= nu*ulaplacian43(i1,i2,i3,uc)
                   uti(i1,i2,i3,vc)= nu*ulaplacian43(i1,i2,i3,vc)
                   uti(i1,i2,i3,wc)= nu*ulaplacian43(i1,i2,i3,wc)
                 ! end NO eq NO: 
                end if
               end do
               end do
               end do
              else
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                 ! explicit terms only, no diffusion
                  ut(i1,i2,i3,uc)= -uu(i1,i2,i3,uc)*ux43(i1,i2,i3,uc)-
     & uu(i1,i2,i3,vc)*uy43(i1,i2,i3,uc)-uu(i1,i2,i3,wc)*uz43(i1,i2,
     & i3,uc)-ux43(i1,i2,i3,pc)
                  ut(i1,i2,i3,vc)= -uu(i1,i2,i3,uc)*ux43(i1,i2,i3,vc)-
     & uu(i1,i2,i3,vc)*uy43(i1,i2,i3,vc)-uu(i1,i2,i3,wc)*uz43(i1,i2,
     & i3,vc)-uy43(i1,i2,i3,pc)
                  ut(i1,i2,i3,wc)= -uu(i1,i2,i3,uc)*ux43(i1,i2,i3,wc)-
     & uu(i1,i2,i3,vc)*uy43(i1,i2,i3,wc)-uu(i1,i2,i3,wc)*uz43(i1,i2,
     & i3,wc)-uz43(i1,i2,i3,pc)
                 ! include implicit terms - diffusion
                  uti(i1,i2,i3,uc)= nu*ulaplacian43(i1,i2,i3,uc)
                  uti(i1,i2,i3,vc)= nu*ulaplacian43(i1,i2,i3,vc)
                  uti(i1,i2,i3,wc)= nu*ulaplacian43(i1,i2,i3,wc)
                ! end NO eq NO: 
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
                   ut(i1,i2,i3,uc)= -uu(i1,i2,i3,uc)*ux43(i1,i2,i3,uc)-
     & uu(i1,i2,i3,vc)*uy43(i1,i2,i3,uc)-uu(i1,i2,i3,wc)*uz43(i1,i2,
     & i3,uc)-ux43(i1,i2,i3,pc)
                   ut(i1,i2,i3,vc)= -uu(i1,i2,i3,uc)*ux43(i1,i2,i3,vc)-
     & uu(i1,i2,i3,vc)*uy43(i1,i2,i3,vc)-uu(i1,i2,i3,wc)*uz43(i1,i2,
     & i3,vc)-uy43(i1,i2,i3,pc)
                   ut(i1,i2,i3,wc)= -uu(i1,i2,i3,uc)*ux43(i1,i2,i3,wc)-
     & uu(i1,i2,i3,vc)*uy43(i1,i2,i3,wc)-uu(i1,i2,i3,wc)*uz43(i1,i2,
     & i3,wc)-uz43(i1,i2,i3,pc)
                 ! end NO eq NO: 
                end if
               end do
               end do
               end do
              else
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                 ! explicit terms only, no diffusion
                  ut(i1,i2,i3,uc)= -uu(i1,i2,i3,uc)*ux43(i1,i2,i3,uc)-
     & uu(i1,i2,i3,vc)*uy43(i1,i2,i3,uc)-uu(i1,i2,i3,wc)*uz43(i1,i2,
     & i3,uc)-ux43(i1,i2,i3,pc)
                  ut(i1,i2,i3,vc)= -uu(i1,i2,i3,uc)*ux43(i1,i2,i3,vc)-
     & uu(i1,i2,i3,vc)*uy43(i1,i2,i3,vc)-uu(i1,i2,i3,wc)*uz43(i1,i2,
     & i3,vc)-uy43(i1,i2,i3,pc)
                  ut(i1,i2,i3,wc)= -uu(i1,i2,i3,uc)*ux43(i1,i2,i3,wc)-
     & uu(i1,i2,i3,vc)*uy43(i1,i2,i3,wc)-uu(i1,i2,i3,wc)*uz43(i1,i2,
     & i3,wc)-uz43(i1,i2,i3,pc)
                ! end NO eq NO: 
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
            if( implicitOption .eq.computeImplicitTermsSeparately )then
              if( useWhereMask.ne.0 )then
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                if( mask(i1,i2,i3).gt.0 )then
                   stop 6666
                end if
               end do
               end do
               end do
              else
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                  stop 6666
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
                   stop 6666
                end if
               end do
               end do
               end do
              else
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                  stop 6666
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
            if( implicitOption .eq.computeImplicitTermsSeparately )then
              if( useWhereMask.ne.0 )then
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                if( mask(i1,i2,i3).gt.0 )then
                   stop 6666
                end if
               end do
               end do
               end do
              else
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                  stop 6666
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
                   stop 6666
                end if
               end do
               end do
               end do
              else
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                  stop 6666
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
