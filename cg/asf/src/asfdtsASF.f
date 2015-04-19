! This file automatically generated from asfdts.bf with bpp.
        subroutine asfdtsASF(nd, n1a,n1b,n2a,n2b,n3a,n3b, nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,xy, rsxy,  u,uu, gv,dw, rL,
     & pL, dtVar, bc, ipar, rpar, pdb, ierr )
c======================================================================
c
c    Determine the time step for the ASF equations.
c    ---------------------------------------------
c
c nd : number of space dimensions
c
c gv : gridVelocity for moving grids
c uu : for moving grids uu is a workspace to hold u-gv, otherwise uu==u
c dw : distance to the wall for some turbulence models
c p : pressure
c dp : work space for Jameson dissipation
c======================================================================
        implicit none
        integer nd, n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,
     & nd3b,nd4a,nd4b
        real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
        real uu(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
        real gv(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
        real dw(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
        real rL(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)    ! linearized rho
        real pL(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)    ! linearized pressure
        real dtVar(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
        real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
        real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
        integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
        integer bc(0:1,0:2),ierr
        integer ipar(0:*)
        real rpar(0:*)
        double precision pdb  ! pointer to data base
        !   ---- local variables -----
        integer pdeModel,linearizeImplicitMethod,explicitMethod
        real pressureLevel
        real reLambdaImplicit,imLambdaImplicit,reLambdaExplicit,
     & imLambdaExplicit
        real imPartImplicit,imPartExplicit
        integer ok,getInt,getReal
        integer c,i1,i2,i3,kd,kd3,orderOfAccuracy,gridIsMoving,
     & useWhereMask
        integer gridIsImplicit,implicitOption,implicitMethod,
     & isAxisymmetric
        integer use2ndOrderAD,use4thOrderAD,useLocalTimeStepping
        integer rc,tc,pc,uc,vc,wc,sc,nc,kc,ec,grid,m,
     & advectPassiveScalar
        real dt,nuPassiveScalar,adcPassiveScalar
        real dtVarMin,dtVarMax,dtMax,dtMaxInverse
        real dxi,dyi,dzi,dri,dsi,dti,dr2i,ds2i,dt2i
        real ad21,ad22,ad41,ad42,cd22,cd42,adc
        real ad21n,ad22n,ad41n,ad42n,cd22n,cd42n
        real adASF,adSPAL
        real scaleFactor,factor,cDt,cdv,cr,crr
        integer i1a,i2a,i3a
        real yy,yEps,xa,ya,za
c real av2,aw2,av4,aw4,aj,dist,vm,alam,w2,w4,wmax,cc
        integer axis,axisp1,is(0:2)
        integer m1a,m1b,m2a,m2b,m3a,m3b
        integer gridType
        integer rectangular,curvilinear
        parameter( rectangular=0, curvilinear=1 )
        integer turbulenceModel,noTurbulenceModel
        integer baldwinLomax,spalartAllmaras,kEpsilon,kOmega
        parameter (noTurbulenceModel=0,baldwinLomax=1,kEpsilon=2,
     & kOmega=3,spalartAllmaras=4 )
        integer computeAllTerms,doNotComputeImplicitTerms,
     & computeImplicitTermsSeparately,computeAllWithWeightedImplicit
        parameter( computeAllTerms=0,doNotComputeImplicitTerms=1,
     & computeImplicitTermsSeparately=2,
     & computeAllWithWeightedImplicit=3 )
        ! These should match those in OB_Parameters.h
        integer notImplicit,backwardEuler,secondOrderBDF,crankNicolson,
     & lineImplicit
        parameter( notImplicit=0,backwardEuler=1,secondOrderBDF=2,
     & crankNicolson=3,lineImplicit=4 )
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
        real rePart,imPart,imLambda,reLambda,a1,a2,a3,nuMax,cSq,gamma,
     & Rg,ajEps,vn,mu,kThermal,mukt,muktbr
        real cSq0
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
        real xc,yc,zc
        rx(i1,i2,i3)=rsxy(i1,i2,i3,0,0)
        ry(i1,i2,i3)=rsxy(i1,i2,i3,0,1)
        rz(i1,i2,i3)=rsxy(i1,i2,i3,0,2)
        sx(i1,i2,i3)=rsxy(i1,i2,i3,1,0)
        sy(i1,i2,i3)=rsxy(i1,i2,i3,1,1)
        sz(i1,i2,i3)=rsxy(i1,i2,i3,1,2)
        tx(i1,i2,i3)=rsxy(i1,i2,i3,2,0)
        ty(i1,i2,i3)=rsxy(i1,i2,i3,2,1)
        tz(i1,i2,i3)=rsxy(i1,i2,i3,2,2)
        ! for cartesian coordinates
        xc(i1) = xa + dx(0)*(i1-i1a)
        yc(i2) = ya + dx(1)*(i2-i2a)
        zc(i3) = za + dx(2)*(i3-i3a)
        !   The next macro call will define the difference approximation statement functions
        d12(kd) = 1./(2.*dr(kd))
        d22(kd) = 1./(dr(kd)**2)
        ur2(i1,i2,i3,kd)=(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))*d12(0)
        us2(i1,i2,i3,kd)=(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))*d12(1)
        ut2(i1,i2,i3,kd)=(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))*d12(2)
        urr2(i1,i2,i3,kd)=(-2.*u(i1,i2,i3,kd)+(u(i1+1,i2,i3,kd)+u(i1-1,
     & i2,i3,kd)) )*d22(0)
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
        urrr2(i1,i2,i3,kd)=(-2.*(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))+(u(
     & i1+2,i2,i3,kd)-u(i1-2,i2,i3,kd)) )*d22(0)*d12(0)
        usss2(i1,i2,i3,kd)=(-2.*(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))+(u(
     & i1,i2+2,i3,kd)-u(i1,i2-2,i3,kd)) )*d22(1)*d12(1)
        uttt2(i1,i2,i3,kd)=(-2.*(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))+(u(
     & i1,i2,i3+2,kd)-u(i1,i2,i3-2,kd)) )*d22(2)*d12(2)
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
        uxy22(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*urr2(i1,i2,i3,kd)+
     & (rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*urs2(i1,
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
        uxy23r(i1,i2,i3,kd)=(ux23r(i1,i2+1,i3,kd)-ux23r(i1,i2-1,i3,kd))
     & *h12(1)
        uzz23r(i1,i2,i3,kd)=(-2.*u(i1,i2,i3,kd)+(u(i1,i2,i3+1,kd)+u(i1,
     & i2,i3-1,kd)) )*h22(2)
        uxz23r(i1,i2,i3,kd)=(ux23r(i1,i2,i3+1,kd)-ux23r(i1,i2,i3-1,kd))
     & *h12(2)
        uyz23r(i1,i2,i3,kd)=(uy23r(i1,i2,i3+1,kd)-uy23r(i1,i2,i3-1,kd))
     & *h12(2)
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
        !    --- 2nd order 2D artificial diffusion ---
        ad2(c)=adc*(u(i1+1,i2,i3,c)-4.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c)  +
     & u(i1,i2+1,i3,c)                 +u(i1,i2-1,i3,c))
        !    --- 2nd order 3D artificial diffusion ---
        ad23(c)=adc*(u(i1+1,i2,i3,c)-6.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c)  
     & +u(i1,i2+1,i3,c)                   +u(i1,i2-1,i3,c) +u(i1,i2,
     & i3+1,c)                   +u(i1,i2,i3-1,c))
        !     ---fourth-order artificial diffusion in 2D
        ad4(c)=adc*(   -u(i1+2,i2,i3,c)-u(i1-2,i2,i3,c)    -u(i1,i2+2,
     & i3,c)-u(i1,i2-2,i3,c)    +4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)  
     &   +u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c))   -12.*u(i1,i2,i3,c) )
        !     ---fourth-order artificial diffusion in 3D
        ad43(c)=adc*(   -u(i1+2,i2,i3,c)-u(i1-2,i2,i3,c)   -u(i1,i2+2,
     & i3,c)-u(i1,i2-2,i3,c)   -u(i1,i2,i3+2,c)-u(i1,i2,i3-2,c)   +4.*
     & (u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)   +u(i1,i2+1,i3,c)+u(i1,i2-1,
     & i3,c)   +u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c))  -18.*u(i1,i2,i3,c) )
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
c Include "selfAdjointArtificialDiffusion.h"
        !     --- end statement functions
        ierr=0
         ok = getInt(pdb,'rc',rc)
         if( ok.eq.1 )then
           ! write(*,'("*** asfdts: ''rc''=",i4)') rc
         else
           write(*,'("*** asfdts: ''rc'' NOT FOUND")')
         end if
         ok = getInt(pdb,'uc',uc)
         if( ok.eq.1 )then
           ! write(*,'("*** asfdts: ''uc''=",i4)') uc
         else
           write(*,'("*** asfdts: ''uc'' NOT FOUND")')
         end if
         ok = getInt(pdb,'vc',vc)
         if( ok.eq.1 )then
           ! write(*,'("*** asfdts: ''vc''=",i4)') vc
         else
           write(*,'("*** asfdts: ''vc'' NOT FOUND")')
         end if
         ok = getInt(pdb,'wc',wc)
         if( ok.eq.1 )then
           ! write(*,'("*** asfdts: ''wc''=",i4)') wc
         else
           write(*,'("*** asfdts: ''wc'' NOT FOUND")')
         end if
         ok = getInt(pdb,'tc',tc)
         if( ok.eq.1 )then
           ! write(*,'("*** asfdts: ''tc''=",i4)') tc
         else
           write(*,'("*** asfdts: ''tc'' NOT FOUND")')
         end if
         ok = getInt(pdb,'pc',pc)
         if( ok.eq.1 )then
           ! write(*,'("*** asfdts: ''pc''=",i4)') pc
         else
           write(*,'("*** asfdts: ''pc'' NOT FOUND")')
         end if
         ok = getInt(pdb,'kc',kc)
         if( ok.eq.1 )then
           ! write(*,'("*** asfdts: ''kc''=",i4)') kc
         else
           write(*,'("*** asfdts: ''kc'' NOT FOUND")')
         end if
         ok = getInt(pdb,'orderOfAccuracy',orderOfAccuracy)
         if( ok.eq.1 )then
           ! write(*,'("*** asfdts: ''orderOfAccuracy''=",i4)') orderOfAccuracy
         else
           write(*,'("*** asfdts: ''orderOfAccuracy'' NOT FOUND")')
         end if
         ok = getInt(pdb,'useLocalTimeStepping',useLocalTimeStepping)
         if( ok.eq.1 )then
           ! write(*,'("*** asfdts: ''useLocalTimeStepping''=",i4)') useLocalTimeStepping
         else
           write(*,'("*** asfdts: ''useLocalTimeStepping'' NOT FOUND")
     & ')
         end if
         ok = getInt(pdb,'linearizeImplicitMethod',
     & linearizeImplicitMethod)
         if( ok.eq.1 )then
           ! write(*,'("*** asfdts: ''linearizeImplicitMethod''=",i4)') linearizeImplicitMethod
         else
           write(*,'("*** asfdts: ''linearizeImplicitMethod'' NOT 
     & FOUND")')
         end if
         ok = getInt(pdb,'explicitMethod',explicitMethod)
         if( ok.eq.1 )then
           ! write(*,'("*** asfdts: ''explicitMethod''=",i4)') explicitMethod
         else
           write(*,'("*** asfdts: ''explicitMethod'' NOT FOUND")')
         end if
        pdeModel            =ipar( 0)
        turbulenceModel     =ipar( 1)
        grid                =ipar( 2)
        gridIsMoving        =ipar( 3)
        useWhereMask        =ipar( 4)
        gridIsImplicit      =ipar( 5)
        implicitMethod      =ipar( 6)
        implicitOption      =ipar( 7)
        isAxisymmetric      =ipar( 8)
        use2ndOrderAD       =ipar( 9)
        use4thOrderAD       =ipar(10)
        advectPassiveScalar =ipar(11)
        gridType            =ipar(12)
        i1a                 =ipar(13)
        i2a                 =ipar(14)
        i3a                 =ipar(15)
        reLambdaImplicit  =rpar( 0) ! returned here
        imLambdaImplicit  =rpar( 1) ! returned here
        reLambdaExplicit  =rpar( 2) ! returned here
        imLambdaExplicit  =rpar( 3) ! returned here
        dr(0)             =rpar( 4)
        dr(1)             =rpar( 5)
        dr(2)             =rpar( 6)
        dx(0)             =rpar( 7)
        dx(1)             =rpar( 8)
        dx(2)             =rpar( 9)
        adcPassiveScalar  =rpar(10)
        xa                =rpar(11)
        ya                =rpar(12)
        za                =rpar(13)
        yEps              =rpar(14) ! for axisymmetric y<yEps => y is on the axis
        ajEps             =rpar(15) ! for minimum value of the jacobian
        cdv=0. ! no divergence damping *wdh* 2015/04/19
         ok = getReal(pdb,'ad21',ad21)
         if( ok.eq.1 )then
           ! write(*,'("*** asfdts: ''ad21''=",e11.4)') ad21
         else
           write(*,'("*** asfdts: ''ad21'' NOT FOUND")')
         end if
         ok = getReal(pdb,'ad22',ad22)
         if( ok.eq.1 )then
           ! write(*,'("*** asfdts: ''ad22''=",e11.4)') ad22
         else
           write(*,'("*** asfdts: ''ad22'' NOT FOUND")')
         end if
         ok = getReal(pdb,'ad41',ad41)
         if( ok.eq.1 )then
           ! write(*,'("*** asfdts: ''ad41''=",e11.4)') ad41
         else
           write(*,'("*** asfdts: ''ad41'' NOT FOUND")')
         end if
         ok = getReal(pdb,'ad42',ad42)
         if( ok.eq.1 )then
           ! write(*,'("*** asfdts: ''ad42''=",e11.4)') ad42
         else
           write(*,'("*** asfdts: ''ad42'' NOT FOUND")')
         end if
         ok = getReal(pdb,'ad21n',ad21n)
         if( ok.eq.1 )then
           ! write(*,'("*** asfdts: ''ad21n''=",e11.4)') ad21n
         else
           write(*,'("*** asfdts: ''ad21n'' NOT FOUND")')
         end if
         ok = getReal(pdb,'ad22n',ad22n)
         if( ok.eq.1 )then
           ! write(*,'("*** asfdts: ''ad22n''=",e11.4)') ad22n
         else
           write(*,'("*** asfdts: ''ad22n'' NOT FOUND")')
         end if
         ok = getReal(pdb,'ad41n',ad41n)
         if( ok.eq.1 )then
           ! write(*,'("*** asfdts: ''ad41n''=",e11.4)') ad41n
         else
           write(*,'("*** asfdts: ''ad41n'' NOT FOUND")')
         end if
         ok = getReal(pdb,'ad42n',ad42n)
         if( ok.eq.1 )then
           ! write(*,'("*** asfdts: ''ad42n''=",e11.4)') ad42n
         else
           write(*,'("*** asfdts: ''ad42n'' NOT FOUND")')
         end if
         ok = getReal(pdb,'nuPassiveScalar',nuPassiveScalar)
         if( ok.eq.1 )then
           ! write(*,'("*** asfdts: ''nuPassiveScalar''=",e11.4)') nuPassiveScalar
         else
           write(*,'("*** asfdts: ''nuPassiveScalar'' NOT FOUND")')
         end if
         ok = getReal(pdb,'gamma',gamma)
         if( ok.eq.1 )then
           ! write(*,'("*** asfdts: ''gamma''=",e11.4)') gamma
         else
           write(*,'("*** asfdts: ''gamma'' NOT FOUND")')
         end if
         ok = getReal(pdb,'Rg',Rg)
         if( ok.eq.1 )then
           ! write(*,'("*** asfdts: ''Rg''=",e11.4)') Rg
         else
           write(*,'("*** asfdts: ''Rg'' NOT FOUND")')
         end if
         ok = getReal(pdb,'mu',mu)
         if( ok.eq.1 )then
           ! write(*,'("*** asfdts: ''mu''=",e11.4)') mu
         else
           write(*,'("*** asfdts: ''mu'' NOT FOUND")')
         end if
         ok = getReal(pdb,'kThermal',kThermal)
         if( ok.eq.1 )then
           ! write(*,'("*** asfdts: ''kThermal''=",e11.4)') kThermal
         else
           write(*,'("*** asfdts: ''kThermal'' NOT FOUND")')
         end if
         ok = getReal(pdb,'dtMax',dtMax)
         if( ok.eq.1 )then
           ! write(*,'("*** asfdts: ''dtMax''=",e11.4)') dtMax
         else
           write(*,'("*** asfdts: ''dtMax'' NOT FOUND")')
         end if
         ok = getReal(pdb,'pressureLevel',pressureLevel)
         if( ok.eq.1 )then
           ! write(*,'("*** asfdts: ''pressureLevel''=",e11.4)') pressureLevel
         else
           write(*,'("*** asfdts: ''pressureLevel'' NOT FOUND")')
         end if
        ec=kc+1
        mukt = max(4./3.*mu,(gamma-1.)*kThermal)
        write(*,'("asfdts: gridType,gridIsImplicit,implicitMethod,
     & implicitOption,useLocalTimeStepping=",10i3)') gridType,
     & gridIsImplicit,implicitMethod,implicitOption,
     & useLocalTimeStepping
         ! ' 
        write(*,'("asfdts: n1a,n1b,n2a,n2b,n3a,n3b=",6i4)') n1a,n1b,
     & n2a,n2b,n3a,n3b
        write(*,'("asfdts: mu,kThermal=",10f6.3)') mu,kThermal
        if( orderOfAccuracy.ne.2 .and. orderOfAccuracy.ne.4 )then
          write(*,'("asfdts:ERROR orderOfAccuracy=",i6)') 
     & orderOfAccuracy
          stop 1
        end if
        if( gridType.ne.rectangular .and. gridType.ne.curvilinear )then
          write(*,'("asfdts:ERROR gridType=",i6)') gridType
          stop 2
        end if
        if( uc.lt.0 .or. vc.lt.0 .or. (nd.eq.3 .and. wc.lt.0) )then
          write(*,'("asfdts:ERROR uc,vc,ws=",3i6)') uc,vc,wc
          stop 4
        end if
        if( turbulenceModel.eq.kEpsilon .and. (kc.lt.uc+nd .or. 
     & kc.gt.1000) )then
          write(*,'("asfdts:ERROR in kc: nd,uc,vc,wc,kc=",2i6)') nd,uc,
     & vc,wc,kc
          stop 5
        end if
        if( nd.ne.2 .and. nd.ne.3 )then
          write(*,'("asfdts:ERROR nd=",i6)') nd
          stop 1
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
          call getSpalartAllmarasParameters(cb1, cb2, cv1, sigma, 
     & sigmai, kappa, cw1, cw2, cw3, cw3e6, cv1e3, cd0, cr0)
        else if( turbulenceModel.eq.kEpsilon )then
         ! write(*,'(" asfdts: k-epsilon: nc,kc,ec=",3i3)') nc,kc,ec
          call getKEpsilonParameters( cMu,cEps1,cEps2,sigmaEpsI,
     & sigmaKI )
          !  write(*,'(" asfdts: cMu,cEps1,cEps2,sigmaEpsI,sigmaKI=",5f8.3)') cMu,cEps1,cEps2,sigmaEpsI,sigmaKI
        else if( turbulenceModel.ne.noTurbulenceModel )then
          stop 88
        end if
        adc=adcPassiveScalar ! coefficient of linear artificial diffusion
        cd22=ad22/(nd**2)
        cd42=ad42/(nd**2)
        cd22n=ad22/nd
        cd42n=ad42/nd
        dtMaxInverse=1./dtMax
        !     correction factors for divergence damping term
        !     this is an over estimate ****
        if( cdv.eq.0. )then
          scaleFactor=0.
        else
          scaleFactor = 1.
          if( isAxisymmetric.eq.1 )then
            scaleFactor=2.
          end if
        end if
        factor=1.5*scaleFactor
        if( gridIsMoving.ne.0 )then
          ! compute uu = u -gv
          if( nd.eq.2 )then
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
          else if( nd.eq.3 )then
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
            stop 11
          end if
        end if
        !      if( isAxisymmetric.eq.1 )then
        !        computeAxisymmetricCorrection()
        !      end if
        !     *****************************************************
        !     ********DETERMINE THE TIME STEPPING EIGENVALUES *****
        !     *****************************************************      
         if( useLocalTimeStepping.eq.0 )then
           if( gridIsImplicit.eq.0 )then
c Don''t split by ADTYPE since this makes the file too long for no big benefit.
c if( use2ndOrderAD.eq.0 .and. use4thOrderAD.eq.0 )then 
c  getTimeSteppingEigenvaluesByOrder(ASF,GLOBAL,EXPLICIT,NONE)
c else if( use2ndOrderAD.eq.1 .and. use4thOrderAD.eq.0 )then 
c  getTimeSteppingEigenvaluesByOrder(ASF,GLOBAL,EXPLICIT,AD2)
c else if( use2ndOrderAD.eq.0 .and. use4thOrderAD.eq.1 )then 
c  getTimeSteppingEigenvaluesByOrder(ASF,GLOBAL,EXPLICIT,AD4)
c else if( use2ndOrderAD.eq.1 .and. use4thOrderAD.eq.1 )then 
c  getTimeSteppingEigenvaluesByOrder(ASF,GLOBAL,EXPLICIT,AD24)
c else
c   stop 123
c end if
              if( orderOfAccuracy.eq.2 )then
                if( nd.eq.2 )then
                   if( isAxisymmetric.eq.0 )then
                    if( gridType.eq.rectangular )then
                      ! scale factors for 2nd and fourth order:
                        cr = 1.      ! [-1  1 ]/ 2
                        crr = 4.     ! [1 -2 1  ]/ 1
                      imLambda=0.
                      reLambda=0.
                      imLambdaExplicit=0.
                      reLambdaExplicit=0.
                      imLambdaImplicit=0.
                      reLambdaImplicit=0.
                      dtVarMin=1.e22  ! we need a REAL_MAX for fortran
                      dtVarMax=0.
c ...............................................
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                      ! speed of sound squared:
                       cSq = gamma*Rg*u(i1,i2,i3,tc)
                          imPartImplicit= cr*( abs(uu(i1,i2,i3,uc))/dx(
     & 0)+abs(uu(i1,i2,i3,vc))/dx(1) )
                          imPartExplicit=imPartImplicit +sqrt( cSq*( 
     & 1./dx(0)**2+1./dx(1)**2 ) )
                        if( linearizeImplicitMethod.eq.1 )then
                          ! ************** check this ***************
                          ! in the linearized method some of the pressure is done explicitly
                          cSq0 = gamma*abs((u(i1,i2,i3,pc)-pL(i1,i2,i3)
     & )*(1./u(i1,i2,i3,rc)-1./rL(i1,i2,i3)))
                            imPartImplicit=imPartImplicit+sqrt( cSq0*( 
     & 1./dx(0)**2+1./dx(1)**2 ) )
                        end if
                            rePart= crr*(mukt/u(i1,i2,i3,rc))*( 1./dx(
     & 0)**2 + 1./dx(1)**2 )
                        if( use2ndOrderAD.eq.1 )then
                              rePart=rePart + 8.*( ad21 + cd22*( abs(
     & ux22r(i1,i2,i3,uc))+abs(ux22r(i1,i2,i3,vc))+abs(uy22r(i1,i2,i3,
     & uc))+abs(uy22r(i1,i2,i3,vc)) ) )
                        end if
                        if(  use4thOrderAD.eq.1 )then
                            rePart=rePart + 32.*( ad41 + cd42*(abs(
     & ux22r(i1,i2,i3,uc))+abs(ux22r(i1,i2,i3,vc))+abs(uy22r(i1,i2,i3,
     & uc))+abs(uy22r(i1,i2,i3,vc))) )
                        end if
                       reLambdaImplicit=max(reLambdaImplicit,rePart)
                       imLambdaImplicit=max(imLambdaImplicit,
     & imPartImplicit)
                       reLambdaExplicit=max(reLambdaExplicit,rePart)
                       imLambdaExplicit=max(imLambdaExplicit,
     & imPartExplicit)
                        end if
                       end do
                       end do
                       end do
                    else if(  gridType.eq.curvilinear )then
                      ! scale factors for 2nd and fourth order:
                        cr = 1.      ! [-1  1 ]/ 2
                        crr = 4.     ! [1 -2 1  ]/ 1
                      imLambda=0.
                      reLambda=0.
                      imLambdaExplicit=0.
                      reLambdaExplicit=0.
                      imLambdaImplicit=0.
                      reLambdaImplicit=0.
                      dtVarMin=1.e22  ! we need a REAL_MAX for fortran
                      dtVarMax=0.
c ...............................................
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                      ! speed of sound squared:
                       cSq = gamma*Rg*u(i1,i2,i3,tc)
                          a1   = uu(i1,i2,i3,uc)*rx(i1,i2,i3)+uu(i1,i2,
     & i3,vc)*ry(i1,i2,i3)
                          a2   = uu(i1,i2,i3,uc)*sx(i1,i2,i3)+uu(i1,i2,
     & i3,vc)*sy(i1,i2,i3)
                          ! constant nu case
                            ! ************* check this ***********
                            a1=a1 -(mu/u(i1,i2,i3,rc))*( rxx22(i1,i2,
     & i3) + ryy22(i1,i2,i3) )
                            a2=a2 -(mu/u(i1,i2,i3,rc))*( sxx22(i1,i2,
     & i3) + syy22(i1,i2,i3) )
                            rePart = (mukt/u(i1,i2,i3,rc))*( ( rx(i1,
     & i2,i3)**2          + ry(i1,i2,i3)**2          )*(crr/(dr(0)*dr(
     & 0))) +abs( rx(i1,i2,i3)*sx(i1,i2,i3)+ry(i1,i2,i3)*sy(i1,i2,i3) 
     & )*( cr/(dr(0)*dr(1))) +( sx(i1,i2,i3)**2          + sy(i1,i2,
     & i3)**2          )*(crr/(dr(1)*dr(1))) )
                          imPartImplicit=cr*(abs(a1)/dr(0)+abs(a2)/dr(
     & 1))
                          ! write(*,'(" asfdts: i1,i2=",2i3," cr,a1,a2,dr(0),dr(1),imPartI=",6f6.2)') i1,i2,cr,a1,a2,dr(0),dr(1),imPartImplicit
                          imPartExplicit=sqrt( cSq*( rx(i1,i2,i3)**2 *(
     & 1./(dr(0)**2))                        +sy(i1,i2,i3)**2 *(1./(
     & dr(1)**2)) ) )
                        if( linearizeImplicitMethod.eq.1 )then
                          ! ************** check this ***************
                          ! in the linearized method some of the pressure is done explicitly
                          cSq0 = gamma*abs((u(i1,i2,i3,pc)-pL(i1,i2,i3)
     & )*(1./u(i1,i2,i3,rc)-1./rL(i1,i2,i3)))
                            imPartImplicit=imPartImplicit+sqrt( cSq0*( 
     & rx(i1,i2,i3)**2 *(1./(dr(0)**2))                         +sy(
     & i1,i2,i3)**2 *(1./(dr(1)**2)) ) )
                          ! write(*,'(" asfdts: i1,i2=",2i3," cSq0,pL,rL,imPartI=",5f6.2)') i1,i2,cSq0,pL(i1,i2,i3),rL(i1,i2,i3),imPartImplicit
                        end if
                      ! end curvilinear:
                        if( use2ndOrderAD.eq.1 )then
                              rePart=rePart + 8.*( ad21 + cd22*( abs(
     & ux22(i1,i2,i3,uc))+abs(ux22(i1,i2,i3,vc))+abs(uy22(i1,i2,i3,uc)
     & )+abs(uy22(i1,i2,i3,vc)) ) )
                        end if
                        if(  use4thOrderAD.eq.1 )then
                            rePart=rePart + 32.*( ad41 + cd42*(abs(
     & ux22(i1,i2,i3,uc))+abs(ux22(i1,i2,i3,vc))+abs(uy22(i1,i2,i3,uc)
     & )+abs(uy22(i1,i2,i3,vc))) )
                        end if
                       reLambdaImplicit=max(reLambdaImplicit,rePart)
                       imLambdaImplicit=max(imLambdaImplicit,
     & imPartImplicit)
                       reLambdaExplicit=max(reLambdaExplicit,rePart)
                       imLambdaExplicit=max(imLambdaExplicit,
     & imPartExplicit)
                        end if
                       end do
                       end do
                       end do
                    else
                      stop 123
                    end if
                   else
                    ! No axis-symmetric versions yet
                    ! kkc 051115 there is now !
                      if( gridType.eq.rectangular )then
                        ! scale factors for 2nd and fourth order:
                          cr = 1.      ! [-1  1 ]/ 2
                          crr = 4.     ! [1 -2 1  ]/ 1
                        imLambda=0.
                        reLambda=0.
                        imLambdaExplicit=0.
                        reLambdaExplicit=0.
                        imLambdaImplicit=0.
                        reLambdaImplicit=0.
                        dtVarMin=1.e22  ! we need a REAL_MAX for fortran
                        dtVarMax=0.
c ...............................................
                         do i3=n3a,n3b
                         do i2=n2a,n2b
                         do i1=n1a,n1b
                          if( mask(i1,i2,i3).gt.0 )then
                        ! speed of sound squared:
                         cSq = gamma*Rg*u(i1,i2,i3,tc)
                            imPartImplicit= cr*( abs(uu(i1,i2,i3,uc))
     & /dx(0)+abs(uu(i1,i2,i3,vc))/dx(1) )
                            imPartExplicit=imPartImplicit +sqrt( cSq*( 
     & 1./dx(0)**2+1./dx(1)**2 ) )
                          if( linearizeImplicitMethod.eq.1 )then
                            ! ************** check this ***************
                            ! in the linearized method some of the pressure is done explicitly
                            cSq0 = gamma*abs((u(i1,i2,i3,pc)-pL(i1,i2,
     & i3))*(1./u(i1,i2,i3,rc)-1./rL(i1,i2,i3)))
                              imPartImplicit=imPartImplicit+sqrt( cSq0*
     & ( 1./dx(0)**2+1./dx(1)**2 ) )
                          end if
                              rePart= crr*(mukt/u(i1,i2,i3,rc))*( 
     & 1./dx(0)**2 + 1./dx(1)**2 )
                              yy=yc(i2)
                              if( abs(yy).ge.dx(1) )then
                                imPartImplicit=imPartImplicit + cr*( 
     & 1./(yy*dx(1)) )  ! u.y/y
                                imPartExplicit=imPartExplicit + cr*( 
     & 1./(yy*dx(1)) )
                              else
                                rePart=rePart + crr*(mu/u(i1,i2,i3,rc))
     & *( 1./dx(1)**2 )   ! u.yy
                              end if
                          if( use2ndOrderAD.eq.1 )then
                                rePart=rePart + 8.*( ad21 + cd22*( abs(
     & ux22r(i1,i2,i3,uc))+abs(ux22r(i1,i2,i3,vc))+abs(uy22r(i1,i2,i3,
     & uc))+abs(uy22r(i1,i2,i3,vc)) ) )
                          end if
                          if(  use4thOrderAD.eq.1 )then
                              rePart=rePart + 32.*( ad41 + cd42*(abs(
     & ux22r(i1,i2,i3,uc))+abs(ux22r(i1,i2,i3,vc))+abs(uy22r(i1,i2,i3,
     & uc))+abs(uy22r(i1,i2,i3,vc))) )
                          end if
                         reLambdaImplicit=max(reLambdaImplicit,rePart)
                         imLambdaImplicit=max(imLambdaImplicit,
     & imPartImplicit)
                         reLambdaExplicit=max(reLambdaExplicit,rePart)
                         imLambdaExplicit=max(imLambdaExplicit,
     & imPartExplicit)
                          end if
                         end do
                         end do
                         end do
                      else if(  gridType.eq.curvilinear )then
                        ! scale factors for 2nd and fourth order:
                          cr = 1.      ! [-1  1 ]/ 2
                          crr = 4.     ! [1 -2 1  ]/ 1
                        imLambda=0.
                        reLambda=0.
                        imLambdaExplicit=0.
                        reLambdaExplicit=0.
                        imLambdaImplicit=0.
                        reLambdaImplicit=0.
                        dtVarMin=1.e22  ! we need a REAL_MAX for fortran
                        dtVarMax=0.
c ...............................................
                         do i3=n3a,n3b
                         do i2=n2a,n2b
                         do i1=n1a,n1b
                          if( mask(i1,i2,i3).gt.0 )then
                        ! speed of sound squared:
                         cSq = gamma*Rg*u(i1,i2,i3,tc)
                            a1   = uu(i1,i2,i3,uc)*rx(i1,i2,i3)+uu(i1,
     & i2,i3,vc)*ry(i1,i2,i3)
                            a2   = uu(i1,i2,i3,uc)*sx(i1,i2,i3)+uu(i1,
     & i2,i3,vc)*sy(i1,i2,i3)
                            ! constant nu case
                              ! ************* check this ***********
                              a1=a1 -(mu/u(i1,i2,i3,rc))*( rxx22(i1,i2,
     & i3) + ryy22(i1,i2,i3) )
                              a2=a2 -(mu/u(i1,i2,i3,rc))*( sxx22(i1,i2,
     & i3) + syy22(i1,i2,i3) )
                              rePart = (mukt/u(i1,i2,i3,rc))*( ( rx(i1,
     & i2,i3)**2          + ry(i1,i2,i3)**2          )*(crr/(dr(0)*dr(
     & 0))) +abs( rx(i1,i2,i3)*sx(i1,i2,i3)+ry(i1,i2,i3)*sy(i1,i2,i3) 
     & )*( cr/(dr(0)*dr(1))) +( sx(i1,i2,i3)**2          + sy(i1,i2,
     & i3)**2          )*(crr/(dr(1)*dr(1))) )
                              yy=xy(i1,i2,i3,1)
                              if( abs(yy).gt.yEps )then
                                ! u.y/y 
                                a1 = a1 -(mu/u(i1,i2,i3,rc))*( ry(i1,
     & i2,i3)/yy )
                                a2 = a2 -(mu/u(i1,i2,i3,rc))*( sy(i1,
     & i2,i3)/yy )
                              else
                                ! u.yy 
                                rePart= rePart+(mu/u(i1,i2,i3,rc))*(  (
     &                             ry(i1,i2,i3)**2          )*(crr/(
     & dr(0)*dr(0))) +abs(                           ry(i1,i2,i3)*sy(
     & i1,i2,i3) )*( cr/(dr(0)*dr(1))) +(                            
     & sy(i1,i2,i3)**2          )*(crr/(dr(1)*dr(1))) )
                              end if
                            imPartImplicit=cr*(abs(a1)/dr(0)+abs(a2)
     & /dr(1))
                            ! write(*,'(" asfdts: i1,i2=",2i3," cr,a1,a2,dr(0),dr(1),imPartI=",6f6.2)') i1,i2,cr,a1,a2,dr(0),dr(1),imPartImplicit
                            imPartExplicit=sqrt( cSq*( rx(i1,i2,i3)**2 
     & *(1./(dr(0)**2))                        +sy(i1,i2,i3)**2 *(1./(
     & dr(1)**2)) ) )
                          if( linearizeImplicitMethod.eq.1 )then
                            ! ************** check this ***************
                            ! in the linearized method some of the pressure is done explicitly
                            cSq0 = gamma*abs((u(i1,i2,i3,pc)-pL(i1,i2,
     & i3))*(1./u(i1,i2,i3,rc)-1./rL(i1,i2,i3)))
                              imPartImplicit=imPartImplicit+sqrt( cSq0*
     & ( rx(i1,i2,i3)**2 *(1./(dr(0)**2))                         +sy(
     & i1,i2,i3)**2 *(1./(dr(1)**2)) ) )
                            ! write(*,'(" asfdts: i1,i2=",2i3," cSq0,pL,rL,imPartI=",5f6.2)') i1,i2,cSq0,pL(i1,i2,i3),rL(i1,i2,i3),imPartImplicit
                          end if
                        ! end curvilinear:
                          if( use2ndOrderAD.eq.1 )then
                                rePart=rePart + 8.*( ad21 + cd22*( abs(
     & ux22(i1,i2,i3,uc))+abs(ux22(i1,i2,i3,vc))+abs(uy22(i1,i2,i3,uc)
     & )+abs(uy22(i1,i2,i3,vc)) ) )
                          end if
                          if(  use4thOrderAD.eq.1 )then
                              rePart=rePart + 32.*( ad41 + cd42*(abs(
     & ux22(i1,i2,i3,uc))+abs(ux22(i1,i2,i3,vc))+abs(uy22(i1,i2,i3,uc)
     & )+abs(uy22(i1,i2,i3,vc))) )
                          end if
                         reLambdaImplicit=max(reLambdaImplicit,rePart)
                         imLambdaImplicit=max(imLambdaImplicit,
     & imPartImplicit)
                         reLambdaExplicit=max(reLambdaExplicit,rePart)
                         imLambdaExplicit=max(imLambdaExplicit,
     & imPartExplicit)
                          end if
                         end do
                         end do
                         end do
                      else
                       stop 123
                      end if
                   end if
                else if( nd.eq.3 )then
                   if( isAxisymmetric.eq.0 )then
                    if( gridType.eq.rectangular )then
                      ! scale factors for 2nd and fourth order:
                        cr = 1.      ! [-1  1 ]/ 2
                        crr = 4.     ! [1 -2 1  ]/ 1
                      imLambda=0.
                      reLambda=0.
                      imLambdaExplicit=0.
                      reLambdaExplicit=0.
                      imLambdaImplicit=0.
                      reLambdaImplicit=0.
                      dtVarMin=1.e22  ! we need a REAL_MAX for fortran
                      dtVarMax=0.
c ...............................................
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                      ! speed of sound squared:
                       cSq = gamma*Rg*u(i1,i2,i3,tc)
                          imPartImplicit= cr*( abs(uu(i1,i2,i3,uc))/dx(
     & 0) + abs(uu(i1,i2,i3,vc))/dx(1) + abs(uu(i1,i2,i3,wc))/dx(2) )
                          imPartExplicit=imPartImplicit + sqrt( cSq*( 
     & 1./dx(0)**2 +1./dx(1)**2 +1./dx(2)**2 ) )
                        if( linearizeImplicitMethod.eq.1 )then
                          ! ************** check this ***************
                          ! in the linearized method some of the pressure is done explicitly
                          cSq0 = gamma*abs((u(i1,i2,i3,pc)-pL(i1,i2,i3)
     & )*(1./u(i1,i2,i3,rc)-1./rL(i1,i2,i3)))
                            imPartImplicit=imPartImplicit+ sqrt( cSq0*(
     &  1./dx(0)**2 +1./dx(1)**2 +1./dx(2)**2 ) )
                        end if
                            rePart= crr*(mukt/u(i1,i2,i3,rc))*( 1./dx(
     & 0)**2 + 1./dx(1)**2 + 1./dx(2)**2 )
                        if( use2ndOrderAD.eq.1 )then
                              rePart=rePart + 12.*( ad21 + cd22*( abs(
     & ux23r(i1,i2,i3,uc))+abs(ux23r(i1,i2,i3,vc))+abs(ux23r(i1,i2,i3,
     & wc))+abs(uy23r(i1,i2,i3,uc))+abs(uy23r(i1,i2,i3,vc))+abs(uy23r(
     & i1,i2,i3,wc))+abs(uz23r(i1,i2,i3,uc))+abs(uz23r(i1,i2,i3,vc))+
     & abs(uz23r(i1,i2,i3,wc))) )
                        end if
                        if(  use4thOrderAD.eq.1 )then
                            rePart=rePart + 48.*( ad21 + cd22*( abs(
     & ux23r(i1,i2,i3,uc))+abs(ux23r(i1,i2,i3,vc))+abs(ux23r(i1,i2,i3,
     & wc))+abs(uy23r(i1,i2,i3,uc))+abs(uy23r(i1,i2,i3,vc))+abs(uy23r(
     & i1,i2,i3,wc))+abs(uz23r(i1,i2,i3,uc))+abs(uz23r(i1,i2,i3,vc))+
     & abs(uz23r(i1,i2,i3,wc))) )
                        end if
                       reLambdaImplicit=max(reLambdaImplicit,rePart)
                       imLambdaImplicit=max(imLambdaImplicit,
     & imPartImplicit)
                       reLambdaExplicit=max(reLambdaExplicit,rePart)
                       imLambdaExplicit=max(imLambdaExplicit,
     & imPartExplicit)
                        end if
                       end do
                       end do
                       end do
                    else if(  gridType.eq.curvilinear )then
                      ! scale factors for 2nd and fourth order:
                        cr = 1.      ! [-1  1 ]/ 2
                        crr = 4.     ! [1 -2 1  ]/ 1
                      imLambda=0.
                      reLambda=0.
                      imLambdaExplicit=0.
                      reLambdaExplicit=0.
                      imLambdaImplicit=0.
                      reLambdaImplicit=0.
                      dtVarMin=1.e22  ! we need a REAL_MAX for fortran
                      dtVarMax=0.
c ...............................................
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                      ! speed of sound squared:
                       cSq = gamma*Rg*u(i1,i2,i3,tc)
                          a1   = uu(i1,i2,i3,uc)*rx(i1,i2,i3)+uu(i1,i2,
     & i3,vc)*ry(i1,i2,i3)+uu(i1,i2,i3,wc)*rz(i1,i2,i3)
                          a2   = uu(i1,i2,i3,uc)*sx(i1,i2,i3)+uu(i1,i2,
     & i3,vc)*sy(i1,i2,i3)+uu(i1,i2,i3,wc)*sz(i1,i2,i3)
                          a3   = uu(i1,i2,i3,uc)*tx(i1,i2,i3)+uu(i1,i2,
     & i3,vc)*ty(i1,i2,i3)+uu(i1,i2,i3,wc)*tz(i1,i2,i3)
                          ! constant nu case
                            muktbr=mukt/u(i1,i2,i3,rc)
                            a1=a1 -muktbr*( rxx23(i1,i2,i3) + ryy23(i1,
     & i2,i3) + rzz23(i1,i2,i3))
                            a2=a2 -muktbr*( sxx23(i1,i2,i3) + syy23(i1,
     & i2,i3) + szz23(i1,i2,i3) )
                            a3=a3 -muktbr*( txx23(i1,i2,i3) + tyy23(i1,
     & i2,i3) + tzz23(i1,i2,i3) )
                            rePart = muktbr*( ( rx(i1,i2,i3)**2        
     &   + ry(i1,i2,i3)**2         + rz(i1,i2,i3)**2 )*(crr/(dr(0)*dr(
     & 0))) +( sx(i1,i2,i3)**2          + sy(i1,i2,i3)**2         + 
     & sz(i1,i2,i3)**2 )*(crr/(dr(1)*dr(1)))  +( tx(i1,i2,i3)**2      
     &     + ty(i1,i2,i3)**2         + tz(i1,i2,i3)**2 )*(crr/(dr(2)*
     & dr(2)))  +abs( rx(i1,i2,i3)*sx(i1,i2,i3)+ry(i1,i2,i3)*sy(i1,i2,
     & i3)+rz(i1,i2,i3)*sz(i1,i2,i3) )*(cr/(dr(0)*dr(1))) +abs( rx(i1,
     & i2,i3)*tx(i1,i2,i3)+ry(i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(
     & i1,i2,i3) )*(cr/(dr(0)*dr(2))) +abs( sx(i1,i2,i3)*tx(i1,i2,i3)+
     & sy(i1,i2,i3)*ty(i1,i2,i3)+sz(i1,i2,i3)*tz(i1,i2,i3) )*(cr/(dr(
     & 1)*dr(2))) )
                          imPartImplicit = cr*( abs(a1)/dr(0)+abs(a2)
     & /dr(1)+abs(a3)/dr(2) )
                          imPartExplicit=sqrt( cSq*( rx(i1,i2,i3)**2 *(
     & 1./(dr(0)**2))                        +sy(i1,i2,i3)**2 *(1./(
     & dr(1)**2))                        +tz(i1,i2,i3)**2 *(1./(dr(2)*
     & *2)) ) )
                        if( linearizeImplicitMethod.eq.1 )then
                          ! ************** check this ***************
                          ! in the linearized method some of the pressure is done explicitly
                          cSq0 = gamma*abs((u(i1,i2,i3,pc)-pL(i1,i2,i3)
     & )*(1./u(i1,i2,i3,rc)-1./rL(i1,i2,i3)))
                            imPartImplicit=imPartImplicit+sqrt( cSq0*( 
     & rx(i1,i2,i3)**2 *(1./(dr(0)**2))                         +sy(
     & i1,i2,i3)**2 *(1./(dr(1)**2))                         +tz(i1,
     & i2,i3)**2 *(1./(dr(2)**2)) ) )
                          ! write(*,'(" asfdts: i1,i2=",2i3," cSq0,pL,rL,imPartI=",5f6.2)') i1,i2,cSq0,pL(i1,i2,i3),rL(i1,i2,i3),imPartImplicit
                        end if
                      ! end curvilinear:
                        if( use2ndOrderAD.eq.1 )then
                              rePart=rePart + 12.*( ad21 + cd22*( abs(
     & ux23(i1,i2,i3,uc))+abs(ux23(i1,i2,i3,vc))+abs(ux23(i1,i2,i3,wc)
     & )+abs(uy23(i1,i2,i3,uc))+abs(uy23(i1,i2,i3,vc))+abs(uy23(i1,i2,
     & i3,wc))+abs(uz23(i1,i2,i3,uc))+abs(uz23(i1,i2,i3,vc))+abs(uz23(
     & i1,i2,i3,wc))) )
                        end if
                        if(  use4thOrderAD.eq.1 )then
                            rePart=rePart + 48.*( ad21 + cd22*( abs(
     & ux23(i1,i2,i3,uc))+abs(ux23(i1,i2,i3,vc))+abs(ux23(i1,i2,i3,wc)
     & )+abs(uy23(i1,i2,i3,uc))+abs(uy23(i1,i2,i3,vc))+abs(uy23(i1,i2,
     & i3,wc))+abs(uz23(i1,i2,i3,uc))+abs(uz23(i1,i2,i3,vc))+abs(uz23(
     & i1,i2,i3,wc))) )
                        end if
                       reLambdaImplicit=max(reLambdaImplicit,rePart)
                       imLambdaImplicit=max(imLambdaImplicit,
     & imPartImplicit)
                       reLambdaExplicit=max(reLambdaExplicit,rePart)
                       imLambdaExplicit=max(imLambdaExplicit,
     & imPartExplicit)
                        end if
                       end do
                       end do
                       end do
                    else
                      stop 123
                    end if
                   else
                    ! No axis-symmetric versions yet
                    ! kkc 051115 there is now !
                      if( gridType.eq.rectangular )then
                        ! scale factors for 2nd and fourth order:
                          cr = 1.      ! [-1  1 ]/ 2
                          crr = 4.     ! [1 -2 1  ]/ 1
                        imLambda=0.
                        reLambda=0.
                        imLambdaExplicit=0.
                        reLambdaExplicit=0.
                        imLambdaImplicit=0.
                        reLambdaImplicit=0.
                        dtVarMin=1.e22  ! we need a REAL_MAX for fortran
                        dtVarMax=0.
c ...............................................
                         do i3=n3a,n3b
                         do i2=n2a,n2b
                         do i1=n1a,n1b
                          if( mask(i1,i2,i3).gt.0 )then
                        ! speed of sound squared:
                         cSq = gamma*Rg*u(i1,i2,i3,tc)
                            imPartImplicit= cr*( abs(uu(i1,i2,i3,uc))
     & /dx(0) + abs(uu(i1,i2,i3,vc))/dx(1) + abs(uu(i1,i2,i3,wc))/dx(
     & 2) )
                            imPartExplicit=imPartImplicit + sqrt( cSq*(
     &  1./dx(0)**2 +1./dx(1)**2 +1./dx(2)**2 ) )
                          if( linearizeImplicitMethod.eq.1 )then
                            ! ************** check this ***************
                            ! in the linearized method some of the pressure is done explicitly
                            cSq0 = gamma*abs((u(i1,i2,i3,pc)-pL(i1,i2,
     & i3))*(1./u(i1,i2,i3,rc)-1./rL(i1,i2,i3)))
                              imPartImplicit=imPartImplicit+ sqrt( 
     & cSq0*( 1./dx(0)**2 +1./dx(1)**2 +1./dx(2)**2 ) )
                          end if
                              rePart= crr*(mukt/u(i1,i2,i3,rc))*( 
     & 1./dx(0)**2 + 1./dx(1)**2 + 1./dx(2)**2 )
                              yy=yc(i2)
                              if( abs(yy).ge.dx(1) )then
                                imPartImplicit=imPartImplicit + cr*( 
     & 1./(yy*dx(1)) )  ! u.y/y
                                imPartExplicit=imPartExplicit + cr*( 
     & 1./(yy*dx(1)) )
                              else
                                rePart=rePart + crr*(mu/u(i1,i2,i3,rc))
     & *( 1./dx(1)**2 )   ! u.yy
                              end if
                          if( use2ndOrderAD.eq.1 )then
                                rePart=rePart + 12.*( ad21 + cd22*( 
     & abs(ux23r(i1,i2,i3,uc))+abs(ux23r(i1,i2,i3,vc))+abs(ux23r(i1,
     & i2,i3,wc))+abs(uy23r(i1,i2,i3,uc))+abs(uy23r(i1,i2,i3,vc))+abs(
     & uy23r(i1,i2,i3,wc))+abs(uz23r(i1,i2,i3,uc))+abs(uz23r(i1,i2,i3,
     & vc))+abs(uz23r(i1,i2,i3,wc))) )
                          end if
                          if(  use4thOrderAD.eq.1 )then
                              rePart=rePart + 48.*( ad21 + cd22*( abs(
     & ux23r(i1,i2,i3,uc))+abs(ux23r(i1,i2,i3,vc))+abs(ux23r(i1,i2,i3,
     & wc))+abs(uy23r(i1,i2,i3,uc))+abs(uy23r(i1,i2,i3,vc))+abs(uy23r(
     & i1,i2,i3,wc))+abs(uz23r(i1,i2,i3,uc))+abs(uz23r(i1,i2,i3,vc))+
     & abs(uz23r(i1,i2,i3,wc))) )
                          end if
                         reLambdaImplicit=max(reLambdaImplicit,rePart)
                         imLambdaImplicit=max(imLambdaImplicit,
     & imPartImplicit)
                         reLambdaExplicit=max(reLambdaExplicit,rePart)
                         imLambdaExplicit=max(imLambdaExplicit,
     & imPartExplicit)
                          end if
                         end do
                         end do
                         end do
                      else if(  gridType.eq.curvilinear )then
                        ! scale factors for 2nd and fourth order:
                          cr = 1.      ! [-1  1 ]/ 2
                          crr = 4.     ! [1 -2 1  ]/ 1
                        imLambda=0.
                        reLambda=0.
                        imLambdaExplicit=0.
                        reLambdaExplicit=0.
                        imLambdaImplicit=0.
                        reLambdaImplicit=0.
                        dtVarMin=1.e22  ! we need a REAL_MAX for fortran
                        dtVarMax=0.
c ...............................................
                         do i3=n3a,n3b
                         do i2=n2a,n2b
                         do i1=n1a,n1b
                          if( mask(i1,i2,i3).gt.0 )then
                        ! speed of sound squared:
                         cSq = gamma*Rg*u(i1,i2,i3,tc)
                            a1   = uu(i1,i2,i3,uc)*rx(i1,i2,i3)+uu(i1,
     & i2,i3,vc)*ry(i1,i2,i3)+uu(i1,i2,i3,wc)*rz(i1,i2,i3)
                            a2   = uu(i1,i2,i3,uc)*sx(i1,i2,i3)+uu(i1,
     & i2,i3,vc)*sy(i1,i2,i3)+uu(i1,i2,i3,wc)*sz(i1,i2,i3)
                            a3   = uu(i1,i2,i3,uc)*tx(i1,i2,i3)+uu(i1,
     & i2,i3,vc)*ty(i1,i2,i3)+uu(i1,i2,i3,wc)*tz(i1,i2,i3)
                            ! constant nu case
                              muktbr=mukt/u(i1,i2,i3,rc)
                              a1=a1 -muktbr*( rxx23(i1,i2,i3) + ryy23(
     & i1,i2,i3) + rzz23(i1,i2,i3))
                              a2=a2 -muktbr*( sxx23(i1,i2,i3) + syy23(
     & i1,i2,i3) + szz23(i1,i2,i3) )
                              a3=a3 -muktbr*( txx23(i1,i2,i3) + tyy23(
     & i1,i2,i3) + tzz23(i1,i2,i3) )
                              rePart = muktbr*( ( rx(i1,i2,i3)**2      
     &     + ry(i1,i2,i3)**2         + rz(i1,i2,i3)**2 )*(crr/(dr(0)*
     & dr(0))) +( sx(i1,i2,i3)**2          + sy(i1,i2,i3)**2         +
     &  sz(i1,i2,i3)**2 )*(crr/(dr(1)*dr(1)))  +( tx(i1,i2,i3)**2     
     &      + ty(i1,i2,i3)**2         + tz(i1,i2,i3)**2 )*(crr/(dr(2)*
     & dr(2)))  +abs( rx(i1,i2,i3)*sx(i1,i2,i3)+ry(i1,i2,i3)*sy(i1,i2,
     & i3)+rz(i1,i2,i3)*sz(i1,i2,i3) )*(cr/(dr(0)*dr(1))) +abs( rx(i1,
     & i2,i3)*tx(i1,i2,i3)+ry(i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(
     & i1,i2,i3) )*(cr/(dr(0)*dr(2))) +abs( sx(i1,i2,i3)*tx(i1,i2,i3)+
     & sy(i1,i2,i3)*ty(i1,i2,i3)+sz(i1,i2,i3)*tz(i1,i2,i3) )*(cr/(dr(
     & 1)*dr(2))) )
                            imPartImplicit = cr*( abs(a1)/dr(0)+abs(a2)
     & /dr(1)+abs(a3)/dr(2) )
                            imPartExplicit=sqrt( cSq*( rx(i1,i2,i3)**2 
     & *(1./(dr(0)**2))                        +sy(i1,i2,i3)**2 *(1./(
     & dr(1)**2))                        +tz(i1,i2,i3)**2 *(1./(dr(2)*
     & *2)) ) )
                          if( linearizeImplicitMethod.eq.1 )then
                            ! ************** check this ***************
                            ! in the linearized method some of the pressure is done explicitly
                            cSq0 = gamma*abs((u(i1,i2,i3,pc)-pL(i1,i2,
     & i3))*(1./u(i1,i2,i3,rc)-1./rL(i1,i2,i3)))
                              imPartImplicit=imPartImplicit+sqrt( cSq0*
     & ( rx(i1,i2,i3)**2 *(1./(dr(0)**2))                         +sy(
     & i1,i2,i3)**2 *(1./(dr(1)**2))                         +tz(i1,
     & i2,i3)**2 *(1./(dr(2)**2)) ) )
                            ! write(*,'(" asfdts: i1,i2=",2i3," cSq0,pL,rL,imPartI=",5f6.2)') i1,i2,cSq0,pL(i1,i2,i3),rL(i1,i2,i3),imPartImplicit
                          end if
                        ! end curvilinear:
                          if( use2ndOrderAD.eq.1 )then
                                rePart=rePart + 12.*( ad21 + cd22*( 
     & abs(ux23(i1,i2,i3,uc))+abs(ux23(i1,i2,i3,vc))+abs(ux23(i1,i2,
     & i3,wc))+abs(uy23(i1,i2,i3,uc))+abs(uy23(i1,i2,i3,vc))+abs(uy23(
     & i1,i2,i3,wc))+abs(uz23(i1,i2,i3,uc))+abs(uz23(i1,i2,i3,vc))+
     & abs(uz23(i1,i2,i3,wc))) )
                          end if
                          if(  use4thOrderAD.eq.1 )then
                              rePart=rePart + 48.*( ad21 + cd22*( abs(
     & ux23(i1,i2,i3,uc))+abs(ux23(i1,i2,i3,vc))+abs(ux23(i1,i2,i3,wc)
     & )+abs(uy23(i1,i2,i3,uc))+abs(uy23(i1,i2,i3,vc))+abs(uy23(i1,i2,
     & i3,wc))+abs(uz23(i1,i2,i3,uc))+abs(uz23(i1,i2,i3,vc))+abs(uz23(
     & i1,i2,i3,wc))) )
                          end if
                         reLambdaImplicit=max(reLambdaImplicit,rePart)
                         imLambdaImplicit=max(imLambdaImplicit,
     & imPartImplicit)
                         reLambdaExplicit=max(reLambdaExplicit,rePart)
                         imLambdaExplicit=max(imLambdaExplicit,
     & imPartExplicit)
                          end if
                         end do
                         end do
                         end do
                      else
                       stop 123
                      end if
                   end if
                else
                  stop 123
                end if
              else if( orderOfAccuracy.eq.4 )then
               ! no fourth order versions yet   
               ! getTimeSteppingEigenvaluesByDimension(ASF,GLOBAL,EXPLICIT,AD24,4)
                 stop 321
              else
                stop 123
              end if
           else
            !kkc 060228 activated this line, not sure what differs yet
c Don''t split by ADTYPE since this makes the file too long for no big benefit.
c if( use2ndOrderAD.eq.0 .and. use4thOrderAD.eq.0 )then 
c  getTimeSteppingEigenvaluesByOrder(ASF,GLOBAL,IMPLICIT,NONE)
c else if( use2ndOrderAD.eq.1 .and. use4thOrderAD.eq.0 )then 
c  getTimeSteppingEigenvaluesByOrder(ASF,GLOBAL,IMPLICIT,AD2)
c else if( use2ndOrderAD.eq.0 .and. use4thOrderAD.eq.1 )then 
c  getTimeSteppingEigenvaluesByOrder(ASF,GLOBAL,IMPLICIT,AD4)
c else if( use2ndOrderAD.eq.1 .and. use4thOrderAD.eq.1 )then 
c  getTimeSteppingEigenvaluesByOrder(ASF,GLOBAL,IMPLICIT,AD24)
c else
c   stop 123
c end if
              if( orderOfAccuracy.eq.2 )then
                if( nd.eq.2 )then
                   if( isAxisymmetric.eq.0 )then
                    if( gridType.eq.rectangular )then
                      ! scale factors for 2nd and fourth order:
                        cr = 1.      ! [-1  1 ]/ 2
                        crr = 4.     ! [1 -2 1  ]/ 1
                      imLambda=0.
                      reLambda=0.
                      imLambdaExplicit=0.
                      reLambdaExplicit=0.
                      imLambdaImplicit=0.
                      reLambdaImplicit=0.
                      dtVarMin=1.e22  ! we need a REAL_MAX for fortran
                      dtVarMax=0.
c ...............................................
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                      ! speed of sound squared:
                       cSq = gamma*Rg*u(i1,i2,i3,tc)
                          imPartImplicit= cr*( abs(uu(i1,i2,i3,uc))/dx(
     & 0)+abs(uu(i1,i2,i3,vc))/dx(1) )
                          imPartExplicit=imPartImplicit +sqrt( cSq*( 
     & 1./dx(0)**2+1./dx(1)**2 ) )
                        if( linearizeImplicitMethod.eq.1 )then
                          ! ************** check this ***************
                          ! in the linearized method some of the pressure is done explicitly
                          cSq0 = gamma*abs((u(i1,i2,i3,pc)-pL(i1,i2,i3)
     & )*(1./u(i1,i2,i3,rc)-1./rL(i1,i2,i3)))
                            imPartImplicit=imPartImplicit+sqrt( cSq0*( 
     & 1./dx(0)**2+1./dx(1)**2 ) )
                        end if
                          rePart=0.
                       reLambdaImplicit=max(reLambdaImplicit,rePart)
                       imLambdaImplicit=max(imLambdaImplicit,
     & imPartImplicit)
                       reLambdaExplicit=max(reLambdaExplicit,rePart)
                       imLambdaExplicit=max(imLambdaExplicit,
     & imPartExplicit)
                        end if
                       end do
                       end do
                       end do
                    else if(  gridType.eq.curvilinear )then
                      ! scale factors for 2nd and fourth order:
                        cr = 1.      ! [-1  1 ]/ 2
                        crr = 4.     ! [1 -2 1  ]/ 1
                      imLambda=0.
                      reLambda=0.
                      imLambdaExplicit=0.
                      reLambdaExplicit=0.
                      imLambdaImplicit=0.
                      reLambdaImplicit=0.
                      dtVarMin=1.e22  ! we need a REAL_MAX for fortran
                      dtVarMax=0.
c ...............................................
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                      ! speed of sound squared:
                       cSq = gamma*Rg*u(i1,i2,i3,tc)
                          a1   = uu(i1,i2,i3,uc)*rx(i1,i2,i3)+uu(i1,i2,
     & i3,vc)*ry(i1,i2,i3)
                          a2   = uu(i1,i2,i3,uc)*sx(i1,i2,i3)+uu(i1,i2,
     & i3,vc)*sy(i1,i2,i3)
                          rePart=0.
                          imPartImplicit=cr*(abs(a1)/dr(0)+abs(a2)/dr(
     & 1))
                          ! write(*,'(" asfdts: i1,i2=",2i3," cr,a1,a2,dr(0),dr(1),imPartI=",6f6.2)') i1,i2,cr,a1,a2,dr(0),dr(1),imPartImplicit
                          imPartExplicit=sqrt( cSq*( rx(i1,i2,i3)**2 *(
     & 1./(dr(0)**2))                        +sy(i1,i2,i3)**2 *(1./(
     & dr(1)**2)) ) )
                        if( linearizeImplicitMethod.eq.1 )then
                          ! ************** check this ***************
                          ! in the linearized method some of the pressure is done explicitly
                          cSq0 = gamma*abs((u(i1,i2,i3,pc)-pL(i1,i2,i3)
     & )*(1./u(i1,i2,i3,rc)-1./rL(i1,i2,i3)))
                            imPartImplicit=imPartImplicit+sqrt( cSq0*( 
     & rx(i1,i2,i3)**2 *(1./(dr(0)**2))                         +sy(
     & i1,i2,i3)**2 *(1./(dr(1)**2)) ) )
                          ! write(*,'(" asfdts: i1,i2=",2i3," cSq0,pL,rL,imPartI=",5f6.2)') i1,i2,cSq0,pL(i1,i2,i3),rL(i1,i2,i3),imPartImplicit
                        end if
                      ! end curvilinear:
                       reLambdaImplicit=max(reLambdaImplicit,rePart)
                       imLambdaImplicit=max(imLambdaImplicit,
     & imPartImplicit)
                       reLambdaExplicit=max(reLambdaExplicit,rePart)
                       imLambdaExplicit=max(imLambdaExplicit,
     & imPartExplicit)
                        end if
                       end do
                       end do
                       end do
                    else
                      stop 123
                    end if
                   else
                    ! No axis-symmetric versions yet
                    ! kkc 051115 there is now !
                      if( gridType.eq.rectangular )then
                        ! scale factors for 2nd and fourth order:
                          cr = 1.      ! [-1  1 ]/ 2
                          crr = 4.     ! [1 -2 1  ]/ 1
                        imLambda=0.
                        reLambda=0.
                        imLambdaExplicit=0.
                        reLambdaExplicit=0.
                        imLambdaImplicit=0.
                        reLambdaImplicit=0.
                        dtVarMin=1.e22  ! we need a REAL_MAX for fortran
                        dtVarMax=0.
c ...............................................
                         do i3=n3a,n3b
                         do i2=n2a,n2b
                         do i1=n1a,n1b
                          if( mask(i1,i2,i3).gt.0 )then
                        ! speed of sound squared:
                         cSq = gamma*Rg*u(i1,i2,i3,tc)
                            imPartImplicit= cr*( abs(uu(i1,i2,i3,uc))
     & /dx(0)+abs(uu(i1,i2,i3,vc))/dx(1) )
                            imPartExplicit=imPartImplicit +sqrt( cSq*( 
     & 1./dx(0)**2+1./dx(1)**2 ) )
                          if( linearizeImplicitMethod.eq.1 )then
                            ! ************** check this ***************
                            ! in the linearized method some of the pressure is done explicitly
                            cSq0 = gamma*abs((u(i1,i2,i3,pc)-pL(i1,i2,
     & i3))*(1./u(i1,i2,i3,rc)-1./rL(i1,i2,i3)))
                              imPartImplicit=imPartImplicit+sqrt( cSq0*
     & ( 1./dx(0)**2+1./dx(1)**2 ) )
                          end if
                            rePart=0.
                         reLambdaImplicit=max(reLambdaImplicit,rePart)
                         imLambdaImplicit=max(imLambdaImplicit,
     & imPartImplicit)
                         reLambdaExplicit=max(reLambdaExplicit,rePart)
                         imLambdaExplicit=max(imLambdaExplicit,
     & imPartExplicit)
                          end if
                         end do
                         end do
                         end do
                      else if(  gridType.eq.curvilinear )then
                        ! scale factors for 2nd and fourth order:
                          cr = 1.      ! [-1  1 ]/ 2
                          crr = 4.     ! [1 -2 1  ]/ 1
                        imLambda=0.
                        reLambda=0.
                        imLambdaExplicit=0.
                        reLambdaExplicit=0.
                        imLambdaImplicit=0.
                        reLambdaImplicit=0.
                        dtVarMin=1.e22  ! we need a REAL_MAX for fortran
                        dtVarMax=0.
c ...............................................
                         do i3=n3a,n3b
                         do i2=n2a,n2b
                         do i1=n1a,n1b
                          if( mask(i1,i2,i3).gt.0 )then
                        ! speed of sound squared:
                         cSq = gamma*Rg*u(i1,i2,i3,tc)
                            a1   = uu(i1,i2,i3,uc)*rx(i1,i2,i3)+uu(i1,
     & i2,i3,vc)*ry(i1,i2,i3)
                            a2   = uu(i1,i2,i3,uc)*sx(i1,i2,i3)+uu(i1,
     & i2,i3,vc)*sy(i1,i2,i3)
                            rePart=0.
                            imPartImplicit=cr*(abs(a1)/dr(0)+abs(a2)
     & /dr(1))
                            ! write(*,'(" asfdts: i1,i2=",2i3," cr,a1,a2,dr(0),dr(1),imPartI=",6f6.2)') i1,i2,cr,a1,a2,dr(0),dr(1),imPartImplicit
                            imPartExplicit=sqrt( cSq*( rx(i1,i2,i3)**2 
     & *(1./(dr(0)**2))                        +sy(i1,i2,i3)**2 *(1./(
     & dr(1)**2)) ) )
                          if( linearizeImplicitMethod.eq.1 )then
                            ! ************** check this ***************
                            ! in the linearized method some of the pressure is done explicitly
                            cSq0 = gamma*abs((u(i1,i2,i3,pc)-pL(i1,i2,
     & i3))*(1./u(i1,i2,i3,rc)-1./rL(i1,i2,i3)))
                              imPartImplicit=imPartImplicit+sqrt( cSq0*
     & ( rx(i1,i2,i3)**2 *(1./(dr(0)**2))                         +sy(
     & i1,i2,i3)**2 *(1./(dr(1)**2)) ) )
                            ! write(*,'(" asfdts: i1,i2=",2i3," cSq0,pL,rL,imPartI=",5f6.2)') i1,i2,cSq0,pL(i1,i2,i3),rL(i1,i2,i3),imPartImplicit
                          end if
                        ! end curvilinear:
                         reLambdaImplicit=max(reLambdaImplicit,rePart)
                         imLambdaImplicit=max(imLambdaImplicit,
     & imPartImplicit)
                         reLambdaExplicit=max(reLambdaExplicit,rePart)
                         imLambdaExplicit=max(imLambdaExplicit,
     & imPartExplicit)
                          end if
                         end do
                         end do
                         end do
                      else
                       stop 123
                      end if
                   end if
                else if( nd.eq.3 )then
                   if( isAxisymmetric.eq.0 )then
                    if( gridType.eq.rectangular )then
                      ! scale factors for 2nd and fourth order:
                        cr = 1.      ! [-1  1 ]/ 2
                        crr = 4.     ! [1 -2 1  ]/ 1
                      imLambda=0.
                      reLambda=0.
                      imLambdaExplicit=0.
                      reLambdaExplicit=0.
                      imLambdaImplicit=0.
                      reLambdaImplicit=0.
                      dtVarMin=1.e22  ! we need a REAL_MAX for fortran
                      dtVarMax=0.
c ...............................................
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                      ! speed of sound squared:
                       cSq = gamma*Rg*u(i1,i2,i3,tc)
                          imPartImplicit= cr*( abs(uu(i1,i2,i3,uc))/dx(
     & 0) + abs(uu(i1,i2,i3,vc))/dx(1) + abs(uu(i1,i2,i3,wc))/dx(2) )
                          imPartExplicit=imPartImplicit + sqrt( cSq*( 
     & 1./dx(0)**2 +1./dx(1)**2 +1./dx(2)**2 ) )
                        if( linearizeImplicitMethod.eq.1 )then
                          ! ************** check this ***************
                          ! in the linearized method some of the pressure is done explicitly
                          cSq0 = gamma*abs((u(i1,i2,i3,pc)-pL(i1,i2,i3)
     & )*(1./u(i1,i2,i3,rc)-1./rL(i1,i2,i3)))
                            imPartImplicit=imPartImplicit+ sqrt( cSq0*(
     &  1./dx(0)**2 +1./dx(1)**2 +1./dx(2)**2 ) )
                        end if
                          rePart=0.
                       reLambdaImplicit=max(reLambdaImplicit,rePart)
                       imLambdaImplicit=max(imLambdaImplicit,
     & imPartImplicit)
                       reLambdaExplicit=max(reLambdaExplicit,rePart)
                       imLambdaExplicit=max(imLambdaExplicit,
     & imPartExplicit)
                        end if
                       end do
                       end do
                       end do
                    else if(  gridType.eq.curvilinear )then
                      ! scale factors for 2nd and fourth order:
                        cr = 1.      ! [-1  1 ]/ 2
                        crr = 4.     ! [1 -2 1  ]/ 1
                      imLambda=0.
                      reLambda=0.
                      imLambdaExplicit=0.
                      reLambdaExplicit=0.
                      imLambdaImplicit=0.
                      reLambdaImplicit=0.
                      dtVarMin=1.e22  ! we need a REAL_MAX for fortran
                      dtVarMax=0.
c ...............................................
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                      ! speed of sound squared:
                       cSq = gamma*Rg*u(i1,i2,i3,tc)
                          a1   = uu(i1,i2,i3,uc)*rx(i1,i2,i3)+uu(i1,i2,
     & i3,vc)*ry(i1,i2,i3)+uu(i1,i2,i3,wc)*rz(i1,i2,i3)
                          a2   = uu(i1,i2,i3,uc)*sx(i1,i2,i3)+uu(i1,i2,
     & i3,vc)*sy(i1,i2,i3)+uu(i1,i2,i3,wc)*sz(i1,i2,i3)
                          a3   = uu(i1,i2,i3,uc)*tx(i1,i2,i3)+uu(i1,i2,
     & i3,vc)*ty(i1,i2,i3)+uu(i1,i2,i3,wc)*tz(i1,i2,i3)
                          rePart=0.
                          imPartImplicit = cr*( abs(a1)/dr(0)+abs(a2)
     & /dr(1)+abs(a3)/dr(2) )
                          imPartExplicit=sqrt( cSq*( rx(i1,i2,i3)**2 *(
     & 1./(dr(0)**2))                        +sy(i1,i2,i3)**2 *(1./(
     & dr(1)**2))                        +tz(i1,i2,i3)**2 *(1./(dr(2)*
     & *2)) ) )
                        if( linearizeImplicitMethod.eq.1 )then
                          ! ************** check this ***************
                          ! in the linearized method some of the pressure is done explicitly
                          cSq0 = gamma*abs((u(i1,i2,i3,pc)-pL(i1,i2,i3)
     & )*(1./u(i1,i2,i3,rc)-1./rL(i1,i2,i3)))
                            imPartImplicit=imPartImplicit+sqrt( cSq0*( 
     & rx(i1,i2,i3)**2 *(1./(dr(0)**2))                         +sy(
     & i1,i2,i3)**2 *(1./(dr(1)**2))                         +tz(i1,
     & i2,i3)**2 *(1./(dr(2)**2)) ) )
                          ! write(*,'(" asfdts: i1,i2=",2i3," cSq0,pL,rL,imPartI=",5f6.2)') i1,i2,cSq0,pL(i1,i2,i3),rL(i1,i2,i3),imPartImplicit
                        end if
                      ! end curvilinear:
                       reLambdaImplicit=max(reLambdaImplicit,rePart)
                       imLambdaImplicit=max(imLambdaImplicit,
     & imPartImplicit)
                       reLambdaExplicit=max(reLambdaExplicit,rePart)
                       imLambdaExplicit=max(imLambdaExplicit,
     & imPartExplicit)
                        end if
                       end do
                       end do
                       end do
                    else
                      stop 123
                    end if
                   else
                    ! No axis-symmetric versions yet
                    ! kkc 051115 there is now !
                      if( gridType.eq.rectangular )then
                        ! scale factors for 2nd and fourth order:
                          cr = 1.      ! [-1  1 ]/ 2
                          crr = 4.     ! [1 -2 1  ]/ 1
                        imLambda=0.
                        reLambda=0.
                        imLambdaExplicit=0.
                        reLambdaExplicit=0.
                        imLambdaImplicit=0.
                        reLambdaImplicit=0.
                        dtVarMin=1.e22  ! we need a REAL_MAX for fortran
                        dtVarMax=0.
c ...............................................
                         do i3=n3a,n3b
                         do i2=n2a,n2b
                         do i1=n1a,n1b
                          if( mask(i1,i2,i3).gt.0 )then
                        ! speed of sound squared:
                         cSq = gamma*Rg*u(i1,i2,i3,tc)
                            imPartImplicit= cr*( abs(uu(i1,i2,i3,uc))
     & /dx(0) + abs(uu(i1,i2,i3,vc))/dx(1) + abs(uu(i1,i2,i3,wc))/dx(
     & 2) )
                            imPartExplicit=imPartImplicit + sqrt( cSq*(
     &  1./dx(0)**2 +1./dx(1)**2 +1./dx(2)**2 ) )
                          if( linearizeImplicitMethod.eq.1 )then
                            ! ************** check this ***************
                            ! in the linearized method some of the pressure is done explicitly
                            cSq0 = gamma*abs((u(i1,i2,i3,pc)-pL(i1,i2,
     & i3))*(1./u(i1,i2,i3,rc)-1./rL(i1,i2,i3)))
                              imPartImplicit=imPartImplicit+ sqrt( 
     & cSq0*( 1./dx(0)**2 +1./dx(1)**2 +1./dx(2)**2 ) )
                          end if
                            rePart=0.
                         reLambdaImplicit=max(reLambdaImplicit,rePart)
                         imLambdaImplicit=max(imLambdaImplicit,
     & imPartImplicit)
                         reLambdaExplicit=max(reLambdaExplicit,rePart)
                         imLambdaExplicit=max(imLambdaExplicit,
     & imPartExplicit)
                          end if
                         end do
                         end do
                         end do
                      else if(  gridType.eq.curvilinear )then
                        ! scale factors for 2nd and fourth order:
                          cr = 1.      ! [-1  1 ]/ 2
                          crr = 4.     ! [1 -2 1  ]/ 1
                        imLambda=0.
                        reLambda=0.
                        imLambdaExplicit=0.
                        reLambdaExplicit=0.
                        imLambdaImplicit=0.
                        reLambdaImplicit=0.
                        dtVarMin=1.e22  ! we need a REAL_MAX for fortran
                        dtVarMax=0.
c ...............................................
                         do i3=n3a,n3b
                         do i2=n2a,n2b
                         do i1=n1a,n1b
                          if( mask(i1,i2,i3).gt.0 )then
                        ! speed of sound squared:
                         cSq = gamma*Rg*u(i1,i2,i3,tc)
                            a1   = uu(i1,i2,i3,uc)*rx(i1,i2,i3)+uu(i1,
     & i2,i3,vc)*ry(i1,i2,i3)+uu(i1,i2,i3,wc)*rz(i1,i2,i3)
                            a2   = uu(i1,i2,i3,uc)*sx(i1,i2,i3)+uu(i1,
     & i2,i3,vc)*sy(i1,i2,i3)+uu(i1,i2,i3,wc)*sz(i1,i2,i3)
                            a3   = uu(i1,i2,i3,uc)*tx(i1,i2,i3)+uu(i1,
     & i2,i3,vc)*ty(i1,i2,i3)+uu(i1,i2,i3,wc)*tz(i1,i2,i3)
                            rePart=0.
                            imPartImplicit = cr*( abs(a1)/dr(0)+abs(a2)
     & /dr(1)+abs(a3)/dr(2) )
                            imPartExplicit=sqrt( cSq*( rx(i1,i2,i3)**2 
     & *(1./(dr(0)**2))                        +sy(i1,i2,i3)**2 *(1./(
     & dr(1)**2))                        +tz(i1,i2,i3)**2 *(1./(dr(2)*
     & *2)) ) )
                          if( linearizeImplicitMethod.eq.1 )then
                            ! ************** check this ***************
                            ! in the linearized method some of the pressure is done explicitly
                            cSq0 = gamma*abs((u(i1,i2,i3,pc)-pL(i1,i2,
     & i3))*(1./u(i1,i2,i3,rc)-1./rL(i1,i2,i3)))
                              imPartImplicit=imPartImplicit+sqrt( cSq0*
     & ( rx(i1,i2,i3)**2 *(1./(dr(0)**2))                         +sy(
     & i1,i2,i3)**2 *(1./(dr(1)**2))                         +tz(i1,
     & i2,i3)**2 *(1./(dr(2)**2)) ) )
                            ! write(*,'(" asfdts: i1,i2=",2i3," cSq0,pL,rL,imPartI=",5f6.2)') i1,i2,cSq0,pL(i1,i2,i3),rL(i1,i2,i3),imPartImplicit
                          end if
                        ! end curvilinear:
                         reLambdaImplicit=max(reLambdaImplicit,rePart)
                         imLambdaImplicit=max(imLambdaImplicit,
     & imPartImplicit)
                         reLambdaExplicit=max(reLambdaExplicit,rePart)
                         imLambdaExplicit=max(imLambdaExplicit,
     & imPartExplicit)
                          end if
                         end do
                         end do
                         end do
                      else
                       stop 123
                      end if
                   end if
                else
                  stop 123
                end if
              else if( orderOfAccuracy.eq.4 )then
               ! no fourth order versions yet   
               ! getTimeSteppingEigenvaluesByDimension(ASF,GLOBAL,IMPLICIT,AD24,4)
                 stop 321
              else
                stop 123
              end if
          !  stop 634
           end if
         else
            if( gridIsImplicit.eq.0 )then
c Don''t split by ADTYPE since this makes the file too long for no big benefit.
c if( use2ndOrderAD.eq.0 .and. use4thOrderAD.eq.0 )then 
c  getTimeSteppingEigenvaluesByOrder(ASF,LOCAL,EXPLICIT,NONE)
c else if( use2ndOrderAD.eq.1 .and. use4thOrderAD.eq.0 )then 
c  getTimeSteppingEigenvaluesByOrder(ASF,LOCAL,EXPLICIT,AD2)
c else if( use2ndOrderAD.eq.0 .and. use4thOrderAD.eq.1 )then 
c  getTimeSteppingEigenvaluesByOrder(ASF,LOCAL,EXPLICIT,AD4)
c else if( use2ndOrderAD.eq.1 .and. use4thOrderAD.eq.1 )then 
c  getTimeSteppingEigenvaluesByOrder(ASF,LOCAL,EXPLICIT,AD24)
c else
c   stop 123
c end if
               if( orderOfAccuracy.eq.2 )then
                 if( nd.eq.2 )then
                    if( isAxisymmetric.eq.0 )then
                     if( gridType.eq.rectangular )then
                       ! scale factors for 2nd and fourth order:
                         cr = 1.      ! [-1  1 ]/ 2
                         crr = 4.     ! [1 -2 1  ]/ 1
                       imLambda=0.
                       reLambda=0.
                       imLambdaExplicit=0.
                       reLambdaExplicit=0.
                       imLambdaImplicit=0.
                       reLambdaImplicit=0.
                       dtVarMin=1.e22  ! we need a REAL_MAX for fortran
                       dtVarMax=0.
c ...............................................
                        do i3=n3a,n3b
                        do i2=n2a,n2b
                        do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
                       ! speed of sound squared:
                        cSq = gamma*Rg*u(i1,i2,i3,tc)
                           imPartImplicit= cr*( abs(uu(i1,i2,i3,uc))
     & /dx(0)+abs(uu(i1,i2,i3,vc))/dx(1) )
                           imPartExplicit=imPartImplicit +sqrt( cSq*( 
     & 1./dx(0)**2+1./dx(1)**2 ) )
                         if( linearizeImplicitMethod.eq.1 )then
                           ! ************** check this ***************
                           ! in the linearized method some of the pressure is done explicitly
                           cSq0 = gamma*abs((u(i1,i2,i3,pc)-pL(i1,i2,
     & i3))*(1./u(i1,i2,i3,rc)-1./rL(i1,i2,i3)))
                             imPartImplicit=imPartImplicit+sqrt( cSq0*(
     &  1./dx(0)**2+1./dx(1)**2 ) )
                         end if
                             rePart= crr*(mukt/u(i1,i2,i3,rc))*( 1./dx(
     & 0)**2 + 1./dx(1)**2 )
                         if( use2ndOrderAD.eq.1 )then
                               rePart=rePart + 8.*( ad21 + cd22*( abs(
     & ux22r(i1,i2,i3,uc))+abs(ux22r(i1,i2,i3,vc))+abs(uy22r(i1,i2,i3,
     & uc))+abs(uy22r(i1,i2,i3,vc)) ) )
                         end if
                         if(  use4thOrderAD.eq.1 )then
                             rePart=rePart + 32.*( ad41 + cd42*(abs(
     & ux22r(i1,i2,i3,uc))+abs(ux22r(i1,i2,i3,vc))+abs(uy22r(i1,i2,i3,
     & uc))+abs(uy22r(i1,i2,i3,vc))) )
                         end if
                         if( implicitMethod.eq.lineImplicit )then      
     &          ! line-implicit
                           dtVar(i1,i2,i3)=1./max( dtMaxInverse, sqrt( 
     & imPartImplicit**2 + rePart**2 ) )
                         else
                           dtVar(i1,i2,i3)=1./sqrt( imPartImplicit**2 +
     &  rePart**2 )      ! explicit local time stepping
                         end if
                         dtVarMin=min(dtVarMin,dtVar(i1,i2,i3))
                         dtVarMax=max(dtVarMax,dtVar(i1,i2,i3))
                        reLambdaImplicit=max(reLambdaImplicit,rePart)
                        imLambdaImplicit=max(imLambdaImplicit,
     & imPartImplicit)
                        reLambdaExplicit=max(reLambdaExplicit,rePart)
                        imLambdaExplicit=max(imLambdaExplicit,
     & imPartExplicit)
                         end if
                        end do
                        end do
                        end do
                     else if(  gridType.eq.curvilinear )then
                       ! scale factors for 2nd and fourth order:
                         cr = 1.      ! [-1  1 ]/ 2
                         crr = 4.     ! [1 -2 1  ]/ 1
                       imLambda=0.
                       reLambda=0.
                       imLambdaExplicit=0.
                       reLambdaExplicit=0.
                       imLambdaImplicit=0.
                       reLambdaImplicit=0.
                       dtVarMin=1.e22  ! we need a REAL_MAX for fortran
                       dtVarMax=0.
c ...............................................
                        do i3=n3a,n3b
                        do i2=n2a,n2b
                        do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
                       ! speed of sound squared:
                        cSq = gamma*Rg*u(i1,i2,i3,tc)
                           a1   = uu(i1,i2,i3,uc)*rx(i1,i2,i3)+uu(i1,
     & i2,i3,vc)*ry(i1,i2,i3)
                           a2   = uu(i1,i2,i3,uc)*sx(i1,i2,i3)+uu(i1,
     & i2,i3,vc)*sy(i1,i2,i3)
                           ! constant nu case
                             ! ************* check this ***********
                             a1=a1 -(mu/u(i1,i2,i3,rc))*( rxx22(i1,i2,
     & i3) + ryy22(i1,i2,i3) )
                             a2=a2 -(mu/u(i1,i2,i3,rc))*( sxx22(i1,i2,
     & i3) + syy22(i1,i2,i3) )
                             rePart = (mukt/u(i1,i2,i3,rc))*( ( rx(i1,
     & i2,i3)**2          + ry(i1,i2,i3)**2          )*(crr/(dr(0)*dr(
     & 0))) +abs( rx(i1,i2,i3)*sx(i1,i2,i3)+ry(i1,i2,i3)*sy(i1,i2,i3) 
     & )*( cr/(dr(0)*dr(1))) +( sx(i1,i2,i3)**2          + sy(i1,i2,
     & i3)**2          )*(crr/(dr(1)*dr(1))) )
                           imPartImplicit=cr*(abs(a1)/dr(0)+abs(a2)/dr(
     & 1))
                           ! write(*,'(" asfdts: i1,i2=",2i3," cr,a1,a2,dr(0),dr(1),imPartI=",6f6.2)') i1,i2,cr,a1,a2,dr(0),dr(1),imPartImplicit
                           imPartExplicit=sqrt( cSq*( rx(i1,i2,i3)**2 *
     & (1./(dr(0)**2))                        +sy(i1,i2,i3)**2 *(1./(
     & dr(1)**2)) ) )
                         if( linearizeImplicitMethod.eq.1 )then
                           ! ************** check this ***************
                           ! in the linearized method some of the pressure is done explicitly
                           cSq0 = gamma*abs((u(i1,i2,i3,pc)-pL(i1,i2,
     & i3))*(1./u(i1,i2,i3,rc)-1./rL(i1,i2,i3)))
                             imPartImplicit=imPartImplicit+sqrt( cSq0*(
     &  rx(i1,i2,i3)**2 *(1./(dr(0)**2))                         +sy(
     & i1,i2,i3)**2 *(1./(dr(1)**2)) ) )
                           ! write(*,'(" asfdts: i1,i2=",2i3," cSq0,pL,rL,imPartI=",5f6.2)') i1,i2,cSq0,pL(i1,i2,i3),rL(i1,i2,i3),imPartImplicit
                         end if
                       ! end curvilinear:
                         if( use2ndOrderAD.eq.1 )then
                               rePart=rePart + 8.*( ad21 + cd22*( abs(
     & ux22(i1,i2,i3,uc))+abs(ux22(i1,i2,i3,vc))+abs(uy22(i1,i2,i3,uc)
     & )+abs(uy22(i1,i2,i3,vc)) ) )
                         end if
                         if(  use4thOrderAD.eq.1 )then
                             rePart=rePart + 32.*( ad41 + cd42*(abs(
     & ux22(i1,i2,i3,uc))+abs(ux22(i1,i2,i3,vc))+abs(uy22(i1,i2,i3,uc)
     & )+abs(uy22(i1,i2,i3,vc))) )
                         end if
                         if( implicitMethod.eq.lineImplicit )then      
     &          ! line-implicit
                           dtVar(i1,i2,i3)=1./max( dtMaxInverse, sqrt( 
     & imPartImplicit**2 + rePart**2 ) )
                         else
                           dtVar(i1,i2,i3)=1./sqrt( imPartImplicit**2 +
     &  rePart**2 )      ! explicit local time stepping
                         end if
                         dtVarMin=min(dtVarMin,dtVar(i1,i2,i3))
                         dtVarMax=max(dtVarMax,dtVar(i1,i2,i3))
                        reLambdaImplicit=max(reLambdaImplicit,rePart)
                        imLambdaImplicit=max(imLambdaImplicit,
     & imPartImplicit)
                        reLambdaExplicit=max(reLambdaExplicit,rePart)
                        imLambdaExplicit=max(imLambdaExplicit,
     & imPartExplicit)
                         end if
                        end do
                        end do
                        end do
                     else
                       stop 123
                     end if
                    else
                     ! No axis-symmetric versions yet
                     ! kkc 051115 there is now !
                       if( gridType.eq.rectangular )then
                         ! scale factors for 2nd and fourth order:
                           cr = 1.      ! [-1  1 ]/ 2
                           crr = 4.     ! [1 -2 1  ]/ 1
                         imLambda=0.
                         reLambda=0.
                         imLambdaExplicit=0.
                         reLambdaExplicit=0.
                         imLambdaImplicit=0.
                         reLambdaImplicit=0.
                         dtVarMin=1.e22  ! we need a REAL_MAX for fortran
                         dtVarMax=0.
c ...............................................
                          do i3=n3a,n3b
                          do i2=n2a,n2b
                          do i1=n1a,n1b
                           if( mask(i1,i2,i3).gt.0 )then
                         ! speed of sound squared:
                          cSq = gamma*Rg*u(i1,i2,i3,tc)
                             imPartImplicit= cr*( abs(uu(i1,i2,i3,uc))
     & /dx(0)+abs(uu(i1,i2,i3,vc))/dx(1) )
                             imPartExplicit=imPartImplicit +sqrt( cSq*(
     &  1./dx(0)**2+1./dx(1)**2 ) )
                           if( linearizeImplicitMethod.eq.1 )then
                             ! ************** check this ***************
                             ! in the linearized method some of the pressure is done explicitly
                             cSq0 = gamma*abs((u(i1,i2,i3,pc)-pL(i1,i2,
     & i3))*(1./u(i1,i2,i3,rc)-1./rL(i1,i2,i3)))
                               imPartImplicit=imPartImplicit+sqrt( 
     & cSq0*( 1./dx(0)**2+1./dx(1)**2 ) )
                           end if
                               rePart= crr*(mukt/u(i1,i2,i3,rc))*( 
     & 1./dx(0)**2 + 1./dx(1)**2 )
                               yy=yc(i2)
                               if( abs(yy).ge.dx(1) )then
                                 imPartImplicit=imPartImplicit + cr*( 
     & 1./(yy*dx(1)) )  ! u.y/y
                                 imPartExplicit=imPartExplicit + cr*( 
     & 1./(yy*dx(1)) )
                               else
                                 rePart=rePart + crr*(mu/u(i1,i2,i3,rc)
     & )*( 1./dx(1)**2 )   ! u.yy
                               end if
                           if( use2ndOrderAD.eq.1 )then
                                 rePart=rePart + 8.*( ad21 + cd22*( 
     & abs(ux22r(i1,i2,i3,uc))+abs(ux22r(i1,i2,i3,vc))+abs(uy22r(i1,
     & i2,i3,uc))+abs(uy22r(i1,i2,i3,vc)) ) )
                           end if
                           if(  use4thOrderAD.eq.1 )then
                               rePart=rePart + 32.*( ad41 + cd42*(abs(
     & ux22r(i1,i2,i3,uc))+abs(ux22r(i1,i2,i3,vc))+abs(uy22r(i1,i2,i3,
     & uc))+abs(uy22r(i1,i2,i3,vc))) )
                           end if
                           if( implicitMethod.eq.lineImplicit )then    
     &            ! line-implicit
                             dtVar(i1,i2,i3)=1./max( dtMaxInverse, 
     & sqrt( imPartImplicit**2 + rePart**2 ) )
                           else
                             dtVar(i1,i2,i3)=1./sqrt( imPartImplicit**
     & 2 + rePart**2 )      ! explicit local time stepping
                           end if
                           dtVarMin=min(dtVarMin,dtVar(i1,i2,i3))
                           dtVarMax=max(dtVarMax,dtVar(i1,i2,i3))
                          reLambdaImplicit=max(reLambdaImplicit,rePart)
                          imLambdaImplicit=max(imLambdaImplicit,
     & imPartImplicit)
                          reLambdaExplicit=max(reLambdaExplicit,rePart)
                          imLambdaExplicit=max(imLambdaExplicit,
     & imPartExplicit)
                           end if
                          end do
                          end do
                          end do
                       else if(  gridType.eq.curvilinear )then
                         ! scale factors for 2nd and fourth order:
                           cr = 1.      ! [-1  1 ]/ 2
                           crr = 4.     ! [1 -2 1  ]/ 1
                         imLambda=0.
                         reLambda=0.
                         imLambdaExplicit=0.
                         reLambdaExplicit=0.
                         imLambdaImplicit=0.
                         reLambdaImplicit=0.
                         dtVarMin=1.e22  ! we need a REAL_MAX for fortran
                         dtVarMax=0.
c ...............................................
                          do i3=n3a,n3b
                          do i2=n2a,n2b
                          do i1=n1a,n1b
                           if( mask(i1,i2,i3).gt.0 )then
                         ! speed of sound squared:
                          cSq = gamma*Rg*u(i1,i2,i3,tc)
                             a1   = uu(i1,i2,i3,uc)*rx(i1,i2,i3)+uu(i1,
     & i2,i3,vc)*ry(i1,i2,i3)
                             a2   = uu(i1,i2,i3,uc)*sx(i1,i2,i3)+uu(i1,
     & i2,i3,vc)*sy(i1,i2,i3)
                             ! constant nu case
                               ! ************* check this ***********
                               a1=a1 -(mu/u(i1,i2,i3,rc))*( rxx22(i1,
     & i2,i3) + ryy22(i1,i2,i3) )
                               a2=a2 -(mu/u(i1,i2,i3,rc))*( sxx22(i1,
     & i2,i3) + syy22(i1,i2,i3) )
                               rePart = (mukt/u(i1,i2,i3,rc))*( ( rx(
     & i1,i2,i3)**2          + ry(i1,i2,i3)**2          )*(crr/(dr(0)*
     & dr(0))) +abs( rx(i1,i2,i3)*sx(i1,i2,i3)+ry(i1,i2,i3)*sy(i1,i2,
     & i3) )*( cr/(dr(0)*dr(1))) +( sx(i1,i2,i3)**2          + sy(i1,
     & i2,i3)**2          )*(crr/(dr(1)*dr(1))) )
                               yy=xy(i1,i2,i3,1)
                               if( abs(yy).gt.yEps )then
                                 ! u.y/y 
                                 a1 = a1 -(mu/u(i1,i2,i3,rc))*( ry(i1,
     & i2,i3)/yy )
                                 a2 = a2 -(mu/u(i1,i2,i3,rc))*( sy(i1,
     & i2,i3)/yy )
                               else
                                 ! u.yy 
                                 rePart= rePart+(mu/u(i1,i2,i3,rc))*(  
     & (                            ry(i1,i2,i3)**2          )*(crr/(
     & dr(0)*dr(0))) +abs(                           ry(i1,i2,i3)*sy(
     & i1,i2,i3) )*( cr/(dr(0)*dr(1))) +(                            
     & sy(i1,i2,i3)**2          )*(crr/(dr(1)*dr(1))) )
                               end if
                             imPartImplicit=cr*(abs(a1)/dr(0)+abs(a2)
     & /dr(1))
                             ! write(*,'(" asfdts: i1,i2=",2i3," cr,a1,a2,dr(0),dr(1),imPartI=",6f6.2)') i1,i2,cr,a1,a2,dr(0),dr(1),imPartImplicit
                             imPartExplicit=sqrt( cSq*( rx(i1,i2,i3)**
     & 2 *(1./(dr(0)**2))                        +sy(i1,i2,i3)**2 *(
     & 1./(dr(1)**2)) ) )
                           if( linearizeImplicitMethod.eq.1 )then
                             ! ************** check this ***************
                             ! in the linearized method some of the pressure is done explicitly
                             cSq0 = gamma*abs((u(i1,i2,i3,pc)-pL(i1,i2,
     & i3))*(1./u(i1,i2,i3,rc)-1./rL(i1,i2,i3)))
                               imPartImplicit=imPartImplicit+sqrt( 
     & cSq0*( rx(i1,i2,i3)**2 *(1./(dr(0)**2))                        
     &  +sy(i1,i2,i3)**2 *(1./(dr(1)**2)) ) )
                             ! write(*,'(" asfdts: i1,i2=",2i3," cSq0,pL,rL,imPartI=",5f6.2)') i1,i2,cSq0,pL(i1,i2,i3),rL(i1,i2,i3),imPartImplicit
                           end if
                         ! end curvilinear:
                           if( use2ndOrderAD.eq.1 )then
                                 rePart=rePart + 8.*( ad21 + cd22*( 
     & abs(ux22(i1,i2,i3,uc))+abs(ux22(i1,i2,i3,vc))+abs(uy22(i1,i2,
     & i3,uc))+abs(uy22(i1,i2,i3,vc)) ) )
                           end if
                           if(  use4thOrderAD.eq.1 )then
                               rePart=rePart + 32.*( ad41 + cd42*(abs(
     & ux22(i1,i2,i3,uc))+abs(ux22(i1,i2,i3,vc))+abs(uy22(i1,i2,i3,uc)
     & )+abs(uy22(i1,i2,i3,vc))) )
                           end if
                           if( implicitMethod.eq.lineImplicit )then    
     &            ! line-implicit
                             dtVar(i1,i2,i3)=1./max( dtMaxInverse, 
     & sqrt( imPartImplicit**2 + rePart**2 ) )
                           else
                             dtVar(i1,i2,i3)=1./sqrt( imPartImplicit**
     & 2 + rePart**2 )      ! explicit local time stepping
                           end if
                           dtVarMin=min(dtVarMin,dtVar(i1,i2,i3))
                           dtVarMax=max(dtVarMax,dtVar(i1,i2,i3))
                          reLambdaImplicit=max(reLambdaImplicit,rePart)
                          imLambdaImplicit=max(imLambdaImplicit,
     & imPartImplicit)
                          reLambdaExplicit=max(reLambdaExplicit,rePart)
                          imLambdaExplicit=max(imLambdaExplicit,
     & imPartExplicit)
                           end if
                          end do
                          end do
                          end do
                       else
                        stop 123
                       end if
                    end if
                 else if( nd.eq.3 )then
                    if( isAxisymmetric.eq.0 )then
                     if( gridType.eq.rectangular )then
                       ! scale factors for 2nd and fourth order:
                         cr = 1.      ! [-1  1 ]/ 2
                         crr = 4.     ! [1 -2 1  ]/ 1
                       imLambda=0.
                       reLambda=0.
                       imLambdaExplicit=0.
                       reLambdaExplicit=0.
                       imLambdaImplicit=0.
                       reLambdaImplicit=0.
                       dtVarMin=1.e22  ! we need a REAL_MAX for fortran
                       dtVarMax=0.
c ...............................................
                        do i3=n3a,n3b
                        do i2=n2a,n2b
                        do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
                       ! speed of sound squared:
                        cSq = gamma*Rg*u(i1,i2,i3,tc)
                           imPartImplicit= cr*( abs(uu(i1,i2,i3,uc))
     & /dx(0) + abs(uu(i1,i2,i3,vc))/dx(1) + abs(uu(i1,i2,i3,wc))/dx(
     & 2) )
                           imPartExplicit=imPartImplicit + sqrt( cSq*( 
     & 1./dx(0)**2 +1./dx(1)**2 +1./dx(2)**2 ) )
                         if( linearizeImplicitMethod.eq.1 )then
                           ! ************** check this ***************
                           ! in the linearized method some of the pressure is done explicitly
                           cSq0 = gamma*abs((u(i1,i2,i3,pc)-pL(i1,i2,
     & i3))*(1./u(i1,i2,i3,rc)-1./rL(i1,i2,i3)))
                             imPartImplicit=imPartImplicit+ sqrt( cSq0*
     & ( 1./dx(0)**2 +1./dx(1)**2 +1./dx(2)**2 ) )
                         end if
                             rePart= crr*(mukt/u(i1,i2,i3,rc))*( 1./dx(
     & 0)**2 + 1./dx(1)**2 + 1./dx(2)**2 )
                         if( use2ndOrderAD.eq.1 )then
                               rePart=rePart + 12.*( ad21 + cd22*( abs(
     & ux23r(i1,i2,i3,uc))+abs(ux23r(i1,i2,i3,vc))+abs(ux23r(i1,i2,i3,
     & wc))+abs(uy23r(i1,i2,i3,uc))+abs(uy23r(i1,i2,i3,vc))+abs(uy23r(
     & i1,i2,i3,wc))+abs(uz23r(i1,i2,i3,uc))+abs(uz23r(i1,i2,i3,vc))+
     & abs(uz23r(i1,i2,i3,wc))) )
                         end if
                         if(  use4thOrderAD.eq.1 )then
                             rePart=rePart + 48.*( ad21 + cd22*( abs(
     & ux23r(i1,i2,i3,uc))+abs(ux23r(i1,i2,i3,vc))+abs(ux23r(i1,i2,i3,
     & wc))+abs(uy23r(i1,i2,i3,uc))+abs(uy23r(i1,i2,i3,vc))+abs(uy23r(
     & i1,i2,i3,wc))+abs(uz23r(i1,i2,i3,uc))+abs(uz23r(i1,i2,i3,vc))+
     & abs(uz23r(i1,i2,i3,wc))) )
                         end if
                         if( implicitMethod.eq.lineImplicit )then      
     &          ! line-implicit
                           dtVar(i1,i2,i3)=1./max( dtMaxInverse, sqrt( 
     & imPartImplicit**2 + rePart**2 ) )
                         else
                           dtVar(i1,i2,i3)=1./sqrt( imPartImplicit**2 +
     &  rePart**2 )      ! explicit local time stepping
                         end if
                         dtVarMin=min(dtVarMin,dtVar(i1,i2,i3))
                         dtVarMax=max(dtVarMax,dtVar(i1,i2,i3))
                        reLambdaImplicit=max(reLambdaImplicit,rePart)
                        imLambdaImplicit=max(imLambdaImplicit,
     & imPartImplicit)
                        reLambdaExplicit=max(reLambdaExplicit,rePart)
                        imLambdaExplicit=max(imLambdaExplicit,
     & imPartExplicit)
                         end if
                        end do
                        end do
                        end do
                     else if(  gridType.eq.curvilinear )then
                       ! scale factors for 2nd and fourth order:
                         cr = 1.      ! [-1  1 ]/ 2
                         crr = 4.     ! [1 -2 1  ]/ 1
                       imLambda=0.
                       reLambda=0.
                       imLambdaExplicit=0.
                       reLambdaExplicit=0.
                       imLambdaImplicit=0.
                       reLambdaImplicit=0.
                       dtVarMin=1.e22  ! we need a REAL_MAX for fortran
                       dtVarMax=0.
c ...............................................
                        do i3=n3a,n3b
                        do i2=n2a,n2b
                        do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
                       ! speed of sound squared:
                        cSq = gamma*Rg*u(i1,i2,i3,tc)
                           a1   = uu(i1,i2,i3,uc)*rx(i1,i2,i3)+uu(i1,
     & i2,i3,vc)*ry(i1,i2,i3)+uu(i1,i2,i3,wc)*rz(i1,i2,i3)
                           a2   = uu(i1,i2,i3,uc)*sx(i1,i2,i3)+uu(i1,
     & i2,i3,vc)*sy(i1,i2,i3)+uu(i1,i2,i3,wc)*sz(i1,i2,i3)
                           a3   = uu(i1,i2,i3,uc)*tx(i1,i2,i3)+uu(i1,
     & i2,i3,vc)*ty(i1,i2,i3)+uu(i1,i2,i3,wc)*tz(i1,i2,i3)
                           ! constant nu case
                             muktbr=mukt/u(i1,i2,i3,rc)
                             a1=a1 -muktbr*( rxx23(i1,i2,i3) + ryy23(
     & i1,i2,i3) + rzz23(i1,i2,i3))
                             a2=a2 -muktbr*( sxx23(i1,i2,i3) + syy23(
     & i1,i2,i3) + szz23(i1,i2,i3) )
                             a3=a3 -muktbr*( txx23(i1,i2,i3) + tyy23(
     & i1,i2,i3) + tzz23(i1,i2,i3) )
                             rePart = muktbr*( ( rx(i1,i2,i3)**2       
     &    + ry(i1,i2,i3)**2         + rz(i1,i2,i3)**2 )*(crr/(dr(0)*
     & dr(0))) +( sx(i1,i2,i3)**2          + sy(i1,i2,i3)**2         +
     &  sz(i1,i2,i3)**2 )*(crr/(dr(1)*dr(1)))  +( tx(i1,i2,i3)**2     
     &      + ty(i1,i2,i3)**2         + tz(i1,i2,i3)**2 )*(crr/(dr(2)*
     & dr(2)))  +abs( rx(i1,i2,i3)*sx(i1,i2,i3)+ry(i1,i2,i3)*sy(i1,i2,
     & i3)+rz(i1,i2,i3)*sz(i1,i2,i3) )*(cr/(dr(0)*dr(1))) +abs( rx(i1,
     & i2,i3)*tx(i1,i2,i3)+ry(i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(
     & i1,i2,i3) )*(cr/(dr(0)*dr(2))) +abs( sx(i1,i2,i3)*tx(i1,i2,i3)+
     & sy(i1,i2,i3)*ty(i1,i2,i3)+sz(i1,i2,i3)*tz(i1,i2,i3) )*(cr/(dr(
     & 1)*dr(2))) )
                           imPartImplicit = cr*( abs(a1)/dr(0)+abs(a2)
     & /dr(1)+abs(a3)/dr(2) )
                           imPartExplicit=sqrt( cSq*( rx(i1,i2,i3)**2 *
     & (1./(dr(0)**2))                        +sy(i1,i2,i3)**2 *(1./(
     & dr(1)**2))                        +tz(i1,i2,i3)**2 *(1./(dr(2)*
     & *2)) ) )
                         if( linearizeImplicitMethod.eq.1 )then
                           ! ************** check this ***************
                           ! in the linearized method some of the pressure is done explicitly
                           cSq0 = gamma*abs((u(i1,i2,i3,pc)-pL(i1,i2,
     & i3))*(1./u(i1,i2,i3,rc)-1./rL(i1,i2,i3)))
                             imPartImplicit=imPartImplicit+sqrt( cSq0*(
     &  rx(i1,i2,i3)**2 *(1./(dr(0)**2))                         +sy(
     & i1,i2,i3)**2 *(1./(dr(1)**2))                         +tz(i1,
     & i2,i3)**2 *(1./(dr(2)**2)) ) )
                           ! write(*,'(" asfdts: i1,i2=",2i3," cSq0,pL,rL,imPartI=",5f6.2)') i1,i2,cSq0,pL(i1,i2,i3),rL(i1,i2,i3),imPartImplicit
                         end if
                       ! end curvilinear:
                         if( use2ndOrderAD.eq.1 )then
                               rePart=rePart + 12.*( ad21 + cd22*( abs(
     & ux23(i1,i2,i3,uc))+abs(ux23(i1,i2,i3,vc))+abs(ux23(i1,i2,i3,wc)
     & )+abs(uy23(i1,i2,i3,uc))+abs(uy23(i1,i2,i3,vc))+abs(uy23(i1,i2,
     & i3,wc))+abs(uz23(i1,i2,i3,uc))+abs(uz23(i1,i2,i3,vc))+abs(uz23(
     & i1,i2,i3,wc))) )
                         end if
                         if(  use4thOrderAD.eq.1 )then
                             rePart=rePart + 48.*( ad21 + cd22*( abs(
     & ux23(i1,i2,i3,uc))+abs(ux23(i1,i2,i3,vc))+abs(ux23(i1,i2,i3,wc)
     & )+abs(uy23(i1,i2,i3,uc))+abs(uy23(i1,i2,i3,vc))+abs(uy23(i1,i2,
     & i3,wc))+abs(uz23(i1,i2,i3,uc))+abs(uz23(i1,i2,i3,vc))+abs(uz23(
     & i1,i2,i3,wc))) )
                         end if
                         if( implicitMethod.eq.lineImplicit )then      
     &          ! line-implicit
                           dtVar(i1,i2,i3)=1./max( dtMaxInverse, sqrt( 
     & imPartImplicit**2 + rePart**2 ) )
                         else
                           dtVar(i1,i2,i3)=1./sqrt( imPartImplicit**2 +
     &  rePart**2 )      ! explicit local time stepping
                         end if
                         dtVarMin=min(dtVarMin,dtVar(i1,i2,i3))
                         dtVarMax=max(dtVarMax,dtVar(i1,i2,i3))
                        reLambdaImplicit=max(reLambdaImplicit,rePart)
                        imLambdaImplicit=max(imLambdaImplicit,
     & imPartImplicit)
                        reLambdaExplicit=max(reLambdaExplicit,rePart)
                        imLambdaExplicit=max(imLambdaExplicit,
     & imPartExplicit)
                         end if
                        end do
                        end do
                        end do
                     else
                       stop 123
                     end if
                    else
                     ! No axis-symmetric versions yet
                     ! kkc 051115 there is now !
                       if( gridType.eq.rectangular )then
                         ! scale factors for 2nd and fourth order:
                           cr = 1.      ! [-1  1 ]/ 2
                           crr = 4.     ! [1 -2 1  ]/ 1
                         imLambda=0.
                         reLambda=0.
                         imLambdaExplicit=0.
                         reLambdaExplicit=0.
                         imLambdaImplicit=0.
                         reLambdaImplicit=0.
                         dtVarMin=1.e22  ! we need a REAL_MAX for fortran
                         dtVarMax=0.
c ...............................................
                          do i3=n3a,n3b
                          do i2=n2a,n2b
                          do i1=n1a,n1b
                           if( mask(i1,i2,i3).gt.0 )then
                         ! speed of sound squared:
                          cSq = gamma*Rg*u(i1,i2,i3,tc)
                             imPartImplicit= cr*( abs(uu(i1,i2,i3,uc))
     & /dx(0) + abs(uu(i1,i2,i3,vc))/dx(1) + abs(uu(i1,i2,i3,wc))/dx(
     & 2) )
                             imPartExplicit=imPartImplicit + sqrt( cSq*
     & ( 1./dx(0)**2 +1./dx(1)**2 +1./dx(2)**2 ) )
                           if( linearizeImplicitMethod.eq.1 )then
                             ! ************** check this ***************
                             ! in the linearized method some of the pressure is done explicitly
                             cSq0 = gamma*abs((u(i1,i2,i3,pc)-pL(i1,i2,
     & i3))*(1./u(i1,i2,i3,rc)-1./rL(i1,i2,i3)))
                               imPartImplicit=imPartImplicit+ sqrt( 
     & cSq0*( 1./dx(0)**2 +1./dx(1)**2 +1./dx(2)**2 ) )
                           end if
                               rePart= crr*(mukt/u(i1,i2,i3,rc))*( 
     & 1./dx(0)**2 + 1./dx(1)**2 + 1./dx(2)**2 )
                               yy=yc(i2)
                               if( abs(yy).ge.dx(1) )then
                                 imPartImplicit=imPartImplicit + cr*( 
     & 1./(yy*dx(1)) )  ! u.y/y
                                 imPartExplicit=imPartExplicit + cr*( 
     & 1./(yy*dx(1)) )
                               else
                                 rePart=rePart + crr*(mu/u(i1,i2,i3,rc)
     & )*( 1./dx(1)**2 )   ! u.yy
                               end if
                           if( use2ndOrderAD.eq.1 )then
                                 rePart=rePart + 12.*( ad21 + cd22*( 
     & abs(ux23r(i1,i2,i3,uc))+abs(ux23r(i1,i2,i3,vc))+abs(ux23r(i1,
     & i2,i3,wc))+abs(uy23r(i1,i2,i3,uc))+abs(uy23r(i1,i2,i3,vc))+abs(
     & uy23r(i1,i2,i3,wc))+abs(uz23r(i1,i2,i3,uc))+abs(uz23r(i1,i2,i3,
     & vc))+abs(uz23r(i1,i2,i3,wc))) )
                           end if
                           if(  use4thOrderAD.eq.1 )then
                               rePart=rePart + 48.*( ad21 + cd22*( abs(
     & ux23r(i1,i2,i3,uc))+abs(ux23r(i1,i2,i3,vc))+abs(ux23r(i1,i2,i3,
     & wc))+abs(uy23r(i1,i2,i3,uc))+abs(uy23r(i1,i2,i3,vc))+abs(uy23r(
     & i1,i2,i3,wc))+abs(uz23r(i1,i2,i3,uc))+abs(uz23r(i1,i2,i3,vc))+
     & abs(uz23r(i1,i2,i3,wc))) )
                           end if
                           if( implicitMethod.eq.lineImplicit )then    
     &            ! line-implicit
                             dtVar(i1,i2,i3)=1./max( dtMaxInverse, 
     & sqrt( imPartImplicit**2 + rePart**2 ) )
                           else
                             dtVar(i1,i2,i3)=1./sqrt( imPartImplicit**
     & 2 + rePart**2 )      ! explicit local time stepping
                           end if
                           dtVarMin=min(dtVarMin,dtVar(i1,i2,i3))
                           dtVarMax=max(dtVarMax,dtVar(i1,i2,i3))
                          reLambdaImplicit=max(reLambdaImplicit,rePart)
                          imLambdaImplicit=max(imLambdaImplicit,
     & imPartImplicit)
                          reLambdaExplicit=max(reLambdaExplicit,rePart)
                          imLambdaExplicit=max(imLambdaExplicit,
     & imPartExplicit)
                           end if
                          end do
                          end do
                          end do
                       else if(  gridType.eq.curvilinear )then
                         ! scale factors for 2nd and fourth order:
                           cr = 1.      ! [-1  1 ]/ 2
                           crr = 4.     ! [1 -2 1  ]/ 1
                         imLambda=0.
                         reLambda=0.
                         imLambdaExplicit=0.
                         reLambdaExplicit=0.
                         imLambdaImplicit=0.
                         reLambdaImplicit=0.
                         dtVarMin=1.e22  ! we need a REAL_MAX for fortran
                         dtVarMax=0.
c ...............................................
                          do i3=n3a,n3b
                          do i2=n2a,n2b
                          do i1=n1a,n1b
                           if( mask(i1,i2,i3).gt.0 )then
                         ! speed of sound squared:
                          cSq = gamma*Rg*u(i1,i2,i3,tc)
                             a1   = uu(i1,i2,i3,uc)*rx(i1,i2,i3)+uu(i1,
     & i2,i3,vc)*ry(i1,i2,i3)+uu(i1,i2,i3,wc)*rz(i1,i2,i3)
                             a2   = uu(i1,i2,i3,uc)*sx(i1,i2,i3)+uu(i1,
     & i2,i3,vc)*sy(i1,i2,i3)+uu(i1,i2,i3,wc)*sz(i1,i2,i3)
                             a3   = uu(i1,i2,i3,uc)*tx(i1,i2,i3)+uu(i1,
     & i2,i3,vc)*ty(i1,i2,i3)+uu(i1,i2,i3,wc)*tz(i1,i2,i3)
                             ! constant nu case
                               muktbr=mukt/u(i1,i2,i3,rc)
                               a1=a1 -muktbr*( rxx23(i1,i2,i3) + ryy23(
     & i1,i2,i3) + rzz23(i1,i2,i3))
                               a2=a2 -muktbr*( sxx23(i1,i2,i3) + syy23(
     & i1,i2,i3) + szz23(i1,i2,i3) )
                               a3=a3 -muktbr*( txx23(i1,i2,i3) + tyy23(
     & i1,i2,i3) + tzz23(i1,i2,i3) )
                               rePart = muktbr*( ( rx(i1,i2,i3)**2     
     &      + ry(i1,i2,i3)**2         + rz(i1,i2,i3)**2 )*(crr/(dr(0)*
     & dr(0))) +( sx(i1,i2,i3)**2          + sy(i1,i2,i3)**2         +
     &  sz(i1,i2,i3)**2 )*(crr/(dr(1)*dr(1)))  +( tx(i1,i2,i3)**2     
     &      + ty(i1,i2,i3)**2         + tz(i1,i2,i3)**2 )*(crr/(dr(2)*
     & dr(2)))  +abs( rx(i1,i2,i3)*sx(i1,i2,i3)+ry(i1,i2,i3)*sy(i1,i2,
     & i3)+rz(i1,i2,i3)*sz(i1,i2,i3) )*(cr/(dr(0)*dr(1))) +abs( rx(i1,
     & i2,i3)*tx(i1,i2,i3)+ry(i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(
     & i1,i2,i3) )*(cr/(dr(0)*dr(2))) +abs( sx(i1,i2,i3)*tx(i1,i2,i3)+
     & sy(i1,i2,i3)*ty(i1,i2,i3)+sz(i1,i2,i3)*tz(i1,i2,i3) )*(cr/(dr(
     & 1)*dr(2))) )
                             imPartImplicit = cr*( abs(a1)/dr(0)+abs(
     & a2)/dr(1)+abs(a3)/dr(2) )
                             imPartExplicit=sqrt( cSq*( rx(i1,i2,i3)**
     & 2 *(1./(dr(0)**2))                        +sy(i1,i2,i3)**2 *(
     & 1./(dr(1)**2))                        +tz(i1,i2,i3)**2 *(1./(
     & dr(2)**2)) ) )
                           if( linearizeImplicitMethod.eq.1 )then
                             ! ************** check this ***************
                             ! in the linearized method some of the pressure is done explicitly
                             cSq0 = gamma*abs((u(i1,i2,i3,pc)-pL(i1,i2,
     & i3))*(1./u(i1,i2,i3,rc)-1./rL(i1,i2,i3)))
                               imPartImplicit=imPartImplicit+sqrt( 
     & cSq0*( rx(i1,i2,i3)**2 *(1./(dr(0)**2))                        
     &  +sy(i1,i2,i3)**2 *(1./(dr(1)**2))                         +tz(
     & i1,i2,i3)**2 *(1./(dr(2)**2)) ) )
                             ! write(*,'(" asfdts: i1,i2=",2i3," cSq0,pL,rL,imPartI=",5f6.2)') i1,i2,cSq0,pL(i1,i2,i3),rL(i1,i2,i3),imPartImplicit
                           end if
                         ! end curvilinear:
                           if( use2ndOrderAD.eq.1 )then
                                 rePart=rePart + 12.*( ad21 + cd22*( 
     & abs(ux23(i1,i2,i3,uc))+abs(ux23(i1,i2,i3,vc))+abs(ux23(i1,i2,
     & i3,wc))+abs(uy23(i1,i2,i3,uc))+abs(uy23(i1,i2,i3,vc))+abs(uy23(
     & i1,i2,i3,wc))+abs(uz23(i1,i2,i3,uc))+abs(uz23(i1,i2,i3,vc))+
     & abs(uz23(i1,i2,i3,wc))) )
                           end if
                           if(  use4thOrderAD.eq.1 )then
                               rePart=rePart + 48.*( ad21 + cd22*( abs(
     & ux23(i1,i2,i3,uc))+abs(ux23(i1,i2,i3,vc))+abs(ux23(i1,i2,i3,wc)
     & )+abs(uy23(i1,i2,i3,uc))+abs(uy23(i1,i2,i3,vc))+abs(uy23(i1,i2,
     & i3,wc))+abs(uz23(i1,i2,i3,uc))+abs(uz23(i1,i2,i3,vc))+abs(uz23(
     & i1,i2,i3,wc))) )
                           end if
                           if( implicitMethod.eq.lineImplicit )then    
     &            ! line-implicit
                             dtVar(i1,i2,i3)=1./max( dtMaxInverse, 
     & sqrt( imPartImplicit**2 + rePart**2 ) )
                           else
                             dtVar(i1,i2,i3)=1./sqrt( imPartImplicit**
     & 2 + rePart**2 )      ! explicit local time stepping
                           end if
                           dtVarMin=min(dtVarMin,dtVar(i1,i2,i3))
                           dtVarMax=max(dtVarMax,dtVar(i1,i2,i3))
                          reLambdaImplicit=max(reLambdaImplicit,rePart)
                          imLambdaImplicit=max(imLambdaImplicit,
     & imPartImplicit)
                          reLambdaExplicit=max(reLambdaExplicit,rePart)
                          imLambdaExplicit=max(imLambdaExplicit,
     & imPartExplicit)
                           end if
                          end do
                          end do
                          end do
                       else
                        stop 123
                       end if
                    end if
                 else
                   stop 123
                 end if
               else if( orderOfAccuracy.eq.4 )then
                ! no fourth order versions yet   
                ! getTimeSteppingEigenvaluesByDimension(ASF,LOCAL,EXPLICIT,AD24,4)
                  stop 321
               else
                 stop 123
               end if
            else
             !kkc 060228 activated this line, not sure what differs yet
c Don''t split by ADTYPE since this makes the file too long for no big benefit.
c if( use2ndOrderAD.eq.0 .and. use4thOrderAD.eq.0 )then 
c  getTimeSteppingEigenvaluesByOrder(ASF,LOCAL,IMPLICIT,NONE)
c else if( use2ndOrderAD.eq.1 .and. use4thOrderAD.eq.0 )then 
c  getTimeSteppingEigenvaluesByOrder(ASF,LOCAL,IMPLICIT,AD2)
c else if( use2ndOrderAD.eq.0 .and. use4thOrderAD.eq.1 )then 
c  getTimeSteppingEigenvaluesByOrder(ASF,LOCAL,IMPLICIT,AD4)
c else if( use2ndOrderAD.eq.1 .and. use4thOrderAD.eq.1 )then 
c  getTimeSteppingEigenvaluesByOrder(ASF,LOCAL,IMPLICIT,AD24)
c else
c   stop 123
c end if
               if( orderOfAccuracy.eq.2 )then
                 if( nd.eq.2 )then
                    if( isAxisymmetric.eq.0 )then
                     if( gridType.eq.rectangular )then
                       ! scale factors for 2nd and fourth order:
                         cr = 1.      ! [-1  1 ]/ 2
                         crr = 4.     ! [1 -2 1  ]/ 1
                       imLambda=0.
                       reLambda=0.
                       imLambdaExplicit=0.
                       reLambdaExplicit=0.
                       imLambdaImplicit=0.
                       reLambdaImplicit=0.
                       dtVarMin=1.e22  ! we need a REAL_MAX for fortran
                       dtVarMax=0.
c ...............................................
                        do i3=n3a,n3b
                        do i2=n2a,n2b
                        do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
                       ! speed of sound squared:
                        cSq = gamma*Rg*u(i1,i2,i3,tc)
                           imPartImplicit= cr*( abs(uu(i1,i2,i3,uc))
     & /dx(0)+abs(uu(i1,i2,i3,vc))/dx(1) )
                           imPartExplicit=imPartImplicit +sqrt( cSq*( 
     & 1./dx(0)**2+1./dx(1)**2 ) )
                         if( linearizeImplicitMethod.eq.1 )then
                           ! ************** check this ***************
                           ! in the linearized method some of the pressure is done explicitly
                           cSq0 = gamma*abs((u(i1,i2,i3,pc)-pL(i1,i2,
     & i3))*(1./u(i1,i2,i3,rc)-1./rL(i1,i2,i3)))
                             imPartImplicit=imPartImplicit+sqrt( cSq0*(
     &  1./dx(0)**2+1./dx(1)**2 ) )
                         end if
                           rePart=0.
                         if( implicitMethod.eq.lineImplicit )then      
     &          ! line-implicit
                           dtVar(i1,i2,i3)=1./max( dtMaxInverse, sqrt( 
     & imPartImplicit**2 + rePart**2 ) )
                         else
                           dtVar(i1,i2,i3)=1./sqrt( imPartImplicit**2 +
     &  rePart**2 )      ! explicit local time stepping
                         end if
                         dtVarMin=min(dtVarMin,dtVar(i1,i2,i3))
                         dtVarMax=max(dtVarMax,dtVar(i1,i2,i3))
                        reLambdaImplicit=max(reLambdaImplicit,rePart)
                        imLambdaImplicit=max(imLambdaImplicit,
     & imPartImplicit)
                        reLambdaExplicit=max(reLambdaExplicit,rePart)
                        imLambdaExplicit=max(imLambdaExplicit,
     & imPartExplicit)
                         end if
                        end do
                        end do
                        end do
                     else if(  gridType.eq.curvilinear )then
                       ! scale factors for 2nd and fourth order:
                         cr = 1.      ! [-1  1 ]/ 2
                         crr = 4.     ! [1 -2 1  ]/ 1
                       imLambda=0.
                       reLambda=0.
                       imLambdaExplicit=0.
                       reLambdaExplicit=0.
                       imLambdaImplicit=0.
                       reLambdaImplicit=0.
                       dtVarMin=1.e22  ! we need a REAL_MAX for fortran
                       dtVarMax=0.
c ...............................................
                        do i3=n3a,n3b
                        do i2=n2a,n2b
                        do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
                       ! speed of sound squared:
                        cSq = gamma*Rg*u(i1,i2,i3,tc)
                           a1   = uu(i1,i2,i3,uc)*rx(i1,i2,i3)+uu(i1,
     & i2,i3,vc)*ry(i1,i2,i3)
                           a2   = uu(i1,i2,i3,uc)*sx(i1,i2,i3)+uu(i1,
     & i2,i3,vc)*sy(i1,i2,i3)
                           rePart=0.
                           imPartImplicit=cr*(abs(a1)/dr(0)+abs(a2)/dr(
     & 1))
                           ! write(*,'(" asfdts: i1,i2=",2i3," cr,a1,a2,dr(0),dr(1),imPartI=",6f6.2)') i1,i2,cr,a1,a2,dr(0),dr(1),imPartImplicit
                           imPartExplicit=sqrt( cSq*( rx(i1,i2,i3)**2 *
     & (1./(dr(0)**2))                        +sy(i1,i2,i3)**2 *(1./(
     & dr(1)**2)) ) )
                         if( linearizeImplicitMethod.eq.1 )then
                           ! ************** check this ***************
                           ! in the linearized method some of the pressure is done explicitly
                           cSq0 = gamma*abs((u(i1,i2,i3,pc)-pL(i1,i2,
     & i3))*(1./u(i1,i2,i3,rc)-1./rL(i1,i2,i3)))
                             imPartImplicit=imPartImplicit+sqrt( cSq0*(
     &  rx(i1,i2,i3)**2 *(1./(dr(0)**2))                         +sy(
     & i1,i2,i3)**2 *(1./(dr(1)**2)) ) )
                           ! write(*,'(" asfdts: i1,i2=",2i3," cSq0,pL,rL,imPartI=",5f6.2)') i1,i2,cSq0,pL(i1,i2,i3),rL(i1,i2,i3),imPartImplicit
                         end if
                       ! end curvilinear:
                         if( implicitMethod.eq.lineImplicit )then      
     &          ! line-implicit
                           dtVar(i1,i2,i3)=1./max( dtMaxInverse, sqrt( 
     & imPartImplicit**2 + rePart**2 ) )
                         else
                           dtVar(i1,i2,i3)=1./sqrt( imPartImplicit**2 +
     &  rePart**2 )      ! explicit local time stepping
                         end if
                         dtVarMin=min(dtVarMin,dtVar(i1,i2,i3))
                         dtVarMax=max(dtVarMax,dtVar(i1,i2,i3))
                        reLambdaImplicit=max(reLambdaImplicit,rePart)
                        imLambdaImplicit=max(imLambdaImplicit,
     & imPartImplicit)
                        reLambdaExplicit=max(reLambdaExplicit,rePart)
                        imLambdaExplicit=max(imLambdaExplicit,
     & imPartExplicit)
                         end if
                        end do
                        end do
                        end do
                     else
                       stop 123
                     end if
                    else
                     ! No axis-symmetric versions yet
                     ! kkc 051115 there is now !
                       if( gridType.eq.rectangular )then
                         ! scale factors for 2nd and fourth order:
                           cr = 1.      ! [-1  1 ]/ 2
                           crr = 4.     ! [1 -2 1  ]/ 1
                         imLambda=0.
                         reLambda=0.
                         imLambdaExplicit=0.
                         reLambdaExplicit=0.
                         imLambdaImplicit=0.
                         reLambdaImplicit=0.
                         dtVarMin=1.e22  ! we need a REAL_MAX for fortran
                         dtVarMax=0.
c ...............................................
                          do i3=n3a,n3b
                          do i2=n2a,n2b
                          do i1=n1a,n1b
                           if( mask(i1,i2,i3).gt.0 )then
                         ! speed of sound squared:
                          cSq = gamma*Rg*u(i1,i2,i3,tc)
                             imPartImplicit= cr*( abs(uu(i1,i2,i3,uc))
     & /dx(0)+abs(uu(i1,i2,i3,vc))/dx(1) )
                             imPartExplicit=imPartImplicit +sqrt( cSq*(
     &  1./dx(0)**2+1./dx(1)**2 ) )
                           if( linearizeImplicitMethod.eq.1 )then
                             ! ************** check this ***************
                             ! in the linearized method some of the pressure is done explicitly
                             cSq0 = gamma*abs((u(i1,i2,i3,pc)-pL(i1,i2,
     & i3))*(1./u(i1,i2,i3,rc)-1./rL(i1,i2,i3)))
                               imPartImplicit=imPartImplicit+sqrt( 
     & cSq0*( 1./dx(0)**2+1./dx(1)**2 ) )
                           end if
                             rePart=0.
                           if( implicitMethod.eq.lineImplicit )then    
     &            ! line-implicit
                             dtVar(i1,i2,i3)=1./max( dtMaxInverse, 
     & sqrt( imPartImplicit**2 + rePart**2 ) )
                           else
                             dtVar(i1,i2,i3)=1./sqrt( imPartImplicit**
     & 2 + rePart**2 )      ! explicit local time stepping
                           end if
                           dtVarMin=min(dtVarMin,dtVar(i1,i2,i3))
                           dtVarMax=max(dtVarMax,dtVar(i1,i2,i3))
                          reLambdaImplicit=max(reLambdaImplicit,rePart)
                          imLambdaImplicit=max(imLambdaImplicit,
     & imPartImplicit)
                          reLambdaExplicit=max(reLambdaExplicit,rePart)
                          imLambdaExplicit=max(imLambdaExplicit,
     & imPartExplicit)
                           end if
                          end do
                          end do
                          end do
                       else if(  gridType.eq.curvilinear )then
                         ! scale factors for 2nd and fourth order:
                           cr = 1.      ! [-1  1 ]/ 2
                           crr = 4.     ! [1 -2 1  ]/ 1
                         imLambda=0.
                         reLambda=0.
                         imLambdaExplicit=0.
                         reLambdaExplicit=0.
                         imLambdaImplicit=0.
                         reLambdaImplicit=0.
                         dtVarMin=1.e22  ! we need a REAL_MAX for fortran
                         dtVarMax=0.
c ...............................................
                          do i3=n3a,n3b
                          do i2=n2a,n2b
                          do i1=n1a,n1b
                           if( mask(i1,i2,i3).gt.0 )then
                         ! speed of sound squared:
                          cSq = gamma*Rg*u(i1,i2,i3,tc)
                             a1   = uu(i1,i2,i3,uc)*rx(i1,i2,i3)+uu(i1,
     & i2,i3,vc)*ry(i1,i2,i3)
                             a2   = uu(i1,i2,i3,uc)*sx(i1,i2,i3)+uu(i1,
     & i2,i3,vc)*sy(i1,i2,i3)
                             rePart=0.
                             imPartImplicit=cr*(abs(a1)/dr(0)+abs(a2)
     & /dr(1))
                             ! write(*,'(" asfdts: i1,i2=",2i3," cr,a1,a2,dr(0),dr(1),imPartI=",6f6.2)') i1,i2,cr,a1,a2,dr(0),dr(1),imPartImplicit
                             imPartExplicit=sqrt( cSq*( rx(i1,i2,i3)**
     & 2 *(1./(dr(0)**2))                        +sy(i1,i2,i3)**2 *(
     & 1./(dr(1)**2)) ) )
                           if( linearizeImplicitMethod.eq.1 )then
                             ! ************** check this ***************
                             ! in the linearized method some of the pressure is done explicitly
                             cSq0 = gamma*abs((u(i1,i2,i3,pc)-pL(i1,i2,
     & i3))*(1./u(i1,i2,i3,rc)-1./rL(i1,i2,i3)))
                               imPartImplicit=imPartImplicit+sqrt( 
     & cSq0*( rx(i1,i2,i3)**2 *(1./(dr(0)**2))                        
     &  +sy(i1,i2,i3)**2 *(1./(dr(1)**2)) ) )
                             ! write(*,'(" asfdts: i1,i2=",2i3," cSq0,pL,rL,imPartI=",5f6.2)') i1,i2,cSq0,pL(i1,i2,i3),rL(i1,i2,i3),imPartImplicit
                           end if
                         ! end curvilinear:
                           if( implicitMethod.eq.lineImplicit )then    
     &            ! line-implicit
                             dtVar(i1,i2,i3)=1./max( dtMaxInverse, 
     & sqrt( imPartImplicit**2 + rePart**2 ) )
                           else
                             dtVar(i1,i2,i3)=1./sqrt( imPartImplicit**
     & 2 + rePart**2 )      ! explicit local time stepping
                           end if
                           dtVarMin=min(dtVarMin,dtVar(i1,i2,i3))
                           dtVarMax=max(dtVarMax,dtVar(i1,i2,i3))
                          reLambdaImplicit=max(reLambdaImplicit,rePart)
                          imLambdaImplicit=max(imLambdaImplicit,
     & imPartImplicit)
                          reLambdaExplicit=max(reLambdaExplicit,rePart)
                          imLambdaExplicit=max(imLambdaExplicit,
     & imPartExplicit)
                           end if
                          end do
                          end do
                          end do
                       else
                        stop 123
                       end if
                    end if
                 else if( nd.eq.3 )then
                    if( isAxisymmetric.eq.0 )then
                     if( gridType.eq.rectangular )then
                       ! scale factors for 2nd and fourth order:
                         cr = 1.      ! [-1  1 ]/ 2
                         crr = 4.     ! [1 -2 1  ]/ 1
                       imLambda=0.
                       reLambda=0.
                       imLambdaExplicit=0.
                       reLambdaExplicit=0.
                       imLambdaImplicit=0.
                       reLambdaImplicit=0.
                       dtVarMin=1.e22  ! we need a REAL_MAX for fortran
                       dtVarMax=0.
c ...............................................
                        do i3=n3a,n3b
                        do i2=n2a,n2b
                        do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
                       ! speed of sound squared:
                        cSq = gamma*Rg*u(i1,i2,i3,tc)
                           imPartImplicit= cr*( abs(uu(i1,i2,i3,uc))
     & /dx(0) + abs(uu(i1,i2,i3,vc))/dx(1) + abs(uu(i1,i2,i3,wc))/dx(
     & 2) )
                           imPartExplicit=imPartImplicit + sqrt( cSq*( 
     & 1./dx(0)**2 +1./dx(1)**2 +1./dx(2)**2 ) )
                         if( linearizeImplicitMethod.eq.1 )then
                           ! ************** check this ***************
                           ! in the linearized method some of the pressure is done explicitly
                           cSq0 = gamma*abs((u(i1,i2,i3,pc)-pL(i1,i2,
     & i3))*(1./u(i1,i2,i3,rc)-1./rL(i1,i2,i3)))
                             imPartImplicit=imPartImplicit+ sqrt( cSq0*
     & ( 1./dx(0)**2 +1./dx(1)**2 +1./dx(2)**2 ) )
                         end if
                           rePart=0.
                         if( implicitMethod.eq.lineImplicit )then      
     &          ! line-implicit
                           dtVar(i1,i2,i3)=1./max( dtMaxInverse, sqrt( 
     & imPartImplicit**2 + rePart**2 ) )
                         else
                           dtVar(i1,i2,i3)=1./sqrt( imPartImplicit**2 +
     &  rePart**2 )      ! explicit local time stepping
                         end if
                         dtVarMin=min(dtVarMin,dtVar(i1,i2,i3))
                         dtVarMax=max(dtVarMax,dtVar(i1,i2,i3))
                        reLambdaImplicit=max(reLambdaImplicit,rePart)
                        imLambdaImplicit=max(imLambdaImplicit,
     & imPartImplicit)
                        reLambdaExplicit=max(reLambdaExplicit,rePart)
                        imLambdaExplicit=max(imLambdaExplicit,
     & imPartExplicit)
                         end if
                        end do
                        end do
                        end do
                     else if(  gridType.eq.curvilinear )then
                       ! scale factors for 2nd and fourth order:
                         cr = 1.      ! [-1  1 ]/ 2
                         crr = 4.     ! [1 -2 1  ]/ 1
                       imLambda=0.
                       reLambda=0.
                       imLambdaExplicit=0.
                       reLambdaExplicit=0.
                       imLambdaImplicit=0.
                       reLambdaImplicit=0.
                       dtVarMin=1.e22  ! we need a REAL_MAX for fortran
                       dtVarMax=0.
c ...............................................
                        do i3=n3a,n3b
                        do i2=n2a,n2b
                        do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
                       ! speed of sound squared:
                        cSq = gamma*Rg*u(i1,i2,i3,tc)
                           a1   = uu(i1,i2,i3,uc)*rx(i1,i2,i3)+uu(i1,
     & i2,i3,vc)*ry(i1,i2,i3)+uu(i1,i2,i3,wc)*rz(i1,i2,i3)
                           a2   = uu(i1,i2,i3,uc)*sx(i1,i2,i3)+uu(i1,
     & i2,i3,vc)*sy(i1,i2,i3)+uu(i1,i2,i3,wc)*sz(i1,i2,i3)
                           a3   = uu(i1,i2,i3,uc)*tx(i1,i2,i3)+uu(i1,
     & i2,i3,vc)*ty(i1,i2,i3)+uu(i1,i2,i3,wc)*tz(i1,i2,i3)
                           rePart=0.
                           imPartImplicit = cr*( abs(a1)/dr(0)+abs(a2)
     & /dr(1)+abs(a3)/dr(2) )
                           imPartExplicit=sqrt( cSq*( rx(i1,i2,i3)**2 *
     & (1./(dr(0)**2))                        +sy(i1,i2,i3)**2 *(1./(
     & dr(1)**2))                        +tz(i1,i2,i3)**2 *(1./(dr(2)*
     & *2)) ) )
                         if( linearizeImplicitMethod.eq.1 )then
                           ! ************** check this ***************
                           ! in the linearized method some of the pressure is done explicitly
                           cSq0 = gamma*abs((u(i1,i2,i3,pc)-pL(i1,i2,
     & i3))*(1./u(i1,i2,i3,rc)-1./rL(i1,i2,i3)))
                             imPartImplicit=imPartImplicit+sqrt( cSq0*(
     &  rx(i1,i2,i3)**2 *(1./(dr(0)**2))                         +sy(
     & i1,i2,i3)**2 *(1./(dr(1)**2))                         +tz(i1,
     & i2,i3)**2 *(1./(dr(2)**2)) ) )
                           ! write(*,'(" asfdts: i1,i2=",2i3," cSq0,pL,rL,imPartI=",5f6.2)') i1,i2,cSq0,pL(i1,i2,i3),rL(i1,i2,i3),imPartImplicit
                         end if
                       ! end curvilinear:
                         if( implicitMethod.eq.lineImplicit )then      
     &          ! line-implicit
                           dtVar(i1,i2,i3)=1./max( dtMaxInverse, sqrt( 
     & imPartImplicit**2 + rePart**2 ) )
                         else
                           dtVar(i1,i2,i3)=1./sqrt( imPartImplicit**2 +
     &  rePart**2 )      ! explicit local time stepping
                         end if
                         dtVarMin=min(dtVarMin,dtVar(i1,i2,i3))
                         dtVarMax=max(dtVarMax,dtVar(i1,i2,i3))
                        reLambdaImplicit=max(reLambdaImplicit,rePart)
                        imLambdaImplicit=max(imLambdaImplicit,
     & imPartImplicit)
                        reLambdaExplicit=max(reLambdaExplicit,rePart)
                        imLambdaExplicit=max(imLambdaExplicit,
     & imPartExplicit)
                         end if
                        end do
                        end do
                        end do
                     else
                       stop 123
                     end if
                    else
                     ! No axis-symmetric versions yet
                     ! kkc 051115 there is now !
                       if( gridType.eq.rectangular )then
                         ! scale factors for 2nd and fourth order:
                           cr = 1.      ! [-1  1 ]/ 2
                           crr = 4.     ! [1 -2 1  ]/ 1
                         imLambda=0.
                         reLambda=0.
                         imLambdaExplicit=0.
                         reLambdaExplicit=0.
                         imLambdaImplicit=0.
                         reLambdaImplicit=0.
                         dtVarMin=1.e22  ! we need a REAL_MAX for fortran
                         dtVarMax=0.
c ...............................................
                          do i3=n3a,n3b
                          do i2=n2a,n2b
                          do i1=n1a,n1b
                           if( mask(i1,i2,i3).gt.0 )then
                         ! speed of sound squared:
                          cSq = gamma*Rg*u(i1,i2,i3,tc)
                             imPartImplicit= cr*( abs(uu(i1,i2,i3,uc))
     & /dx(0) + abs(uu(i1,i2,i3,vc))/dx(1) + abs(uu(i1,i2,i3,wc))/dx(
     & 2) )
                             imPartExplicit=imPartImplicit + sqrt( cSq*
     & ( 1./dx(0)**2 +1./dx(1)**2 +1./dx(2)**2 ) )
                           if( linearizeImplicitMethod.eq.1 )then
                             ! ************** check this ***************
                             ! in the linearized method some of the pressure is done explicitly
                             cSq0 = gamma*abs((u(i1,i2,i3,pc)-pL(i1,i2,
     & i3))*(1./u(i1,i2,i3,rc)-1./rL(i1,i2,i3)))
                               imPartImplicit=imPartImplicit+ sqrt( 
     & cSq0*( 1./dx(0)**2 +1./dx(1)**2 +1./dx(2)**2 ) )
                           end if
                             rePart=0.
                           if( implicitMethod.eq.lineImplicit )then    
     &            ! line-implicit
                             dtVar(i1,i2,i3)=1./max( dtMaxInverse, 
     & sqrt( imPartImplicit**2 + rePart**2 ) )
                           else
                             dtVar(i1,i2,i3)=1./sqrt( imPartImplicit**
     & 2 + rePart**2 )      ! explicit local time stepping
                           end if
                           dtVarMin=min(dtVarMin,dtVar(i1,i2,i3))
                           dtVarMax=max(dtVarMax,dtVar(i1,i2,i3))
                          reLambdaImplicit=max(reLambdaImplicit,rePart)
                          imLambdaImplicit=max(imLambdaImplicit,
     & imPartImplicit)
                          reLambdaExplicit=max(reLambdaExplicit,rePart)
                          imLambdaExplicit=max(imLambdaExplicit,
     & imPartExplicit)
                           end if
                          end do
                          end do
                          end do
                       else if(  gridType.eq.curvilinear )then
                         ! scale factors for 2nd and fourth order:
                           cr = 1.      ! [-1  1 ]/ 2
                           crr = 4.     ! [1 -2 1  ]/ 1
                         imLambda=0.
                         reLambda=0.
                         imLambdaExplicit=0.
                         reLambdaExplicit=0.
                         imLambdaImplicit=0.
                         reLambdaImplicit=0.
                         dtVarMin=1.e22  ! we need a REAL_MAX for fortran
                         dtVarMax=0.
c ...............................................
                          do i3=n3a,n3b
                          do i2=n2a,n2b
                          do i1=n1a,n1b
                           if( mask(i1,i2,i3).gt.0 )then
                         ! speed of sound squared:
                          cSq = gamma*Rg*u(i1,i2,i3,tc)
                             a1   = uu(i1,i2,i3,uc)*rx(i1,i2,i3)+uu(i1,
     & i2,i3,vc)*ry(i1,i2,i3)+uu(i1,i2,i3,wc)*rz(i1,i2,i3)
                             a2   = uu(i1,i2,i3,uc)*sx(i1,i2,i3)+uu(i1,
     & i2,i3,vc)*sy(i1,i2,i3)+uu(i1,i2,i3,wc)*sz(i1,i2,i3)
                             a3   = uu(i1,i2,i3,uc)*tx(i1,i2,i3)+uu(i1,
     & i2,i3,vc)*ty(i1,i2,i3)+uu(i1,i2,i3,wc)*tz(i1,i2,i3)
                             rePart=0.
                             imPartImplicit = cr*( abs(a1)/dr(0)+abs(
     & a2)/dr(1)+abs(a3)/dr(2) )
                             imPartExplicit=sqrt( cSq*( rx(i1,i2,i3)**
     & 2 *(1./(dr(0)**2))                        +sy(i1,i2,i3)**2 *(
     & 1./(dr(1)**2))                        +tz(i1,i2,i3)**2 *(1./(
     & dr(2)**2)) ) )
                           if( linearizeImplicitMethod.eq.1 )then
                             ! ************** check this ***************
                             ! in the linearized method some of the pressure is done explicitly
                             cSq0 = gamma*abs((u(i1,i2,i3,pc)-pL(i1,i2,
     & i3))*(1./u(i1,i2,i3,rc)-1./rL(i1,i2,i3)))
                               imPartImplicit=imPartImplicit+sqrt( 
     & cSq0*( rx(i1,i2,i3)**2 *(1./(dr(0)**2))                        
     &  +sy(i1,i2,i3)**2 *(1./(dr(1)**2))                         +tz(
     & i1,i2,i3)**2 *(1./(dr(2)**2)) ) )
                             ! write(*,'(" asfdts: i1,i2=",2i3," cSq0,pL,rL,imPartI=",5f6.2)') i1,i2,cSq0,pL(i1,i2,i3),rL(i1,i2,i3),imPartImplicit
                           end if
                         ! end curvilinear:
                           if( implicitMethod.eq.lineImplicit )then    
     &            ! line-implicit
                             dtVar(i1,i2,i3)=1./max( dtMaxInverse, 
     & sqrt( imPartImplicit**2 + rePart**2 ) )
                           else
                             dtVar(i1,i2,i3)=1./sqrt( imPartImplicit**
     & 2 + rePart**2 )      ! explicit local time stepping
                           end if
                           dtVarMin=min(dtVarMin,dtVar(i1,i2,i3))
                           dtVarMax=max(dtVarMax,dtVar(i1,i2,i3))
                          reLambdaImplicit=max(reLambdaImplicit,rePart)
                          imLambdaImplicit=max(imLambdaImplicit,
     & imPartImplicit)
                          reLambdaExplicit=max(reLambdaExplicit,rePart)
                          imLambdaExplicit=max(imLambdaExplicit,
     & imPartExplicit)
                           end if
                          end do
                          end do
                          end do
                       else
                        stop 123
                       end if
                    end if
                 else
                   stop 123
                 end if
               else if( orderOfAccuracy.eq.4 )then
                ! no fourth order versions yet   
                ! getTimeSteppingEigenvaluesByDimension(ASF,LOCAL,IMPLICIT,AD24,4)
                  stop 321
               else
                 stop 123
               end if
           !  stop 634
            end if
        !kkc  stop 916
         end if
        if( useLocalTimeStepping.eq.1 )then
          write(*,'(" asfdts: local dt, grid=",i3," dtVar (min,max)=(",
     & e10.2,",",e10.2,")")') grid,dtVarMin,dtVarMax
          ! '
        end if
        rpar(0)=reLambdaImplicit
        rpar(1)=imLambdaImplicit
        rpar(2)=reLambdaExplicit
        rpar(3)=imLambdaExplicit
        return
        end
