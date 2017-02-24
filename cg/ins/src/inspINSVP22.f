! This file automatically generated from inspf.bf with bpp.
        subroutine assignPressureRhsINSVP22(nd,  n1a,n1b,n2a,n2b,n3a,
     & n3b,  nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,  mask,xy,rsxy,
     & radiusInverse,  u,uu,f,gv,divDamping,udf,  bc, indexRange, ndb,
     &  bcData,   nr1a,nr1b,nr2a,nr2b,nr3a,nr3b,  normal00,normal10,
     & normal01,normal11,normal02,normal12,  dim, bcf0,bcOffset,
     & addBoundaryForcing, ipar, rpar, ierr )
         implicit none
         integer nd, n1a,n1b,n2a,n2b,n3a,n3b, nd1a,nd1b,nd2a,nd2b,nd3a,
     & nd3b,nd4a,nd4b,ndb
         integer nr1a,nr1b,nr2a,nr2b,nr3a,nr3b
         real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
         real uu(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
         real f(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
         real gv(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
         real divDamping(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
         real udf(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:*)        ! user defined force
         real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
         real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
         real radiusInverse(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
         real bcData(0:ndb-1,0:1,0:2)
         real normal00(nr1a:nr1a,nd2a:nd2b,nd3a:nd3b,0:*)
         real normal10(nr1b:nr1b,nd2a:nd2b,nd3a:nd3b,0:*)
         real normal01(nd1a:nd1b,nr2a:nr2a,nd3a:nd3b,0:*)
         real normal11(nd1a:nd1b,nr2b:nr2b,nd3a:nd3b,0:*)
         real normal02(nd1a:nd1b,nd2a:nd2b,nr3a:nr3a,0:*)
         real normal12(nd1a:nd1b,nd2a:nd2b,nr3b:nr3b,0:*)
         integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
         integer bc(0:1,0:2),indexRange(0:1,0:2), ierr
         integer ipar(0:*)
         real rpar(0:*)
         integer noSlipWall, inflowWithVelocityGiven, outflow, 
     & convectiveOutflow, tractionFree, inflowWithPandTV, 
     & dirichletBoundaryCondition, symmetry, axisymmetric, 
     & interfaceBoundaryCondition,freeSurfaceBoundaryCondition
         parameter( noSlipWall=1,inflowWithVelocityGiven=2, outflow=5,
     & convectiveOutflow=14,tractionFree=15, inflowWithPandTV=3, 
     & dirichletBoundaryCondition=12, symmetry=11,axisymmetric=13,
     & interfaceBoundaryCondition=17, freeSurfaceBoundaryCondition=31 
     & )
         integer pdeModel,standardModel,BoussinesqModel,
     & viscoPlasticModel,twoPhaseFlowModel
         parameter( standardModel=0,BoussinesqModel=1,
     & viscoPlasticModel=2,twoPhaseFlowModel=3 )
        !  enum BoundaryCondition
        !  {
        !0    interpolation=0,
        !1    noSlipWall,
        !2    inflowWithVelocityGiven,
        !3    inflowWithPressureAndTangentialVelocityGiven,
        !4    slipWall,
        !5    outflow,
        !6    superSonicInflow,
        !7    superSonicOutflow,
        !8    subSonicInflow,
        !9    subSonicInflow2,
        !0    subSonicOutflow,
        !1    symmetry,
        !2    dirichletBoundaryCondition,
        !3    axisymmetric,
        !4    convectiveOutflow,  
        !5    tractionFree,
        !6    numberOfBCNames     // counts number of entries
        !  };
        !     ---- local variables -----
         integer c,i1,i2,i3,orderOfAccuracy,gridIsMoving,useWhereMask,
     & twilightZoneFlow,turnOnBodyForcing,debug
         integer isAxisymmetric,is1,is2,is3,pressureBC,gridType,
     & initialConditionsAreBeingProjected
         integer rc,pc,uc,vc,wc,grid,side,axis,bc0,numberOfComponents,
     & axisp1,axisp2,sidep1,sidep2,axisp
         integer nc,tc,vsc
         real nu,dt,advectionCoefficient,advectCoeff,inflowPressure,a1,
     & an(0:2)
         real gravity(0:2),thermalExpansivity,adcBoussinesq
         real adSelfAdjoint2dR,adSelfAdjoint3dR,adSelfAdjoint2dC,
     & adSelfAdjoint3dC
         real adSelfAdjoint2dRSA,adSelfAdjoint3dRSA,adSelfAdjoint2dCSA,
     & adSelfAdjoint3dCSA
         real u0x,u0y,u0z,v0x,v0y,v0z,w0x,w0y,w0z,u0Lap,v0Lap,w0Lap
         real u0xx,u0xy,u0xz,u0yy,u0yz,u0zz
         real v0xx,v0xy,v0xz,v0yy,v0yz,v0zz
         real w0xx,w0xy,w0xz,w0yy,w0yz,w0zz
         real delta2u,delta2v,delta2w,delta4u,delta4v,delta4w
         real n0x,n0y,n0z,n0xx,n0xy,n0xz,n0yy,n0yz,n0zz
         real chi,chi3,nuT,nuTd,nuTdd
         real nuTx,nuTy,nuTz,nuTxx,nuTxy,nuTxz,nuTyy,nuTyz,nuTzz
         real rsxy1, dr1,dx1
         integer rectangular,curvilinear
         parameter( rectangular=0, curvilinear=1 )
         integer turbulenceModel,noTurbulenceModel
         integer baldwinLomax,spalartAllmaras,kEpsilon,kOmega
         parameter (noTurbulenceModel=0,baldwinLomax=1,kEpsilon=2,
     & kOmega=3,spalartAllmaras=4 )
         integer m,n,kd,kdd,kd3,ndc,dir,ks
         ! for SPAL TM
         real cb1, cb2, cv1, sigma, sigmai, kappa, cw1, cw2, cw3, 
     & cw3e6, cv1e3, cd0, cr0
         ! for KE turbulence model
         real cMu,cEps1,cEps2,sigmaEpsI,sigmaKI
         integer kc,ec
         real k0,k0x,k0y,k0z,k0xx,k0xy,k0xz,k0yy,k0yz,k0zz
         real e0,e0x,e0y,e0z,e0xx,e0xy,e0xz,e0yy,e0yz,e0zz
         ! for visco-plastic model
         ! real nuVP,etaVP,yieldStressVP,exponentVP,epsVP
         ! real eDotNorm,exp0,eDotNormSqx,eDotNormSqy,eDotNormSqxx,eDotNormSqxy,eDotNormSqyy
         ! real u0xxx,u0xxy,u0xyy,u0yyy,v0xxx,v0xxy,v0xyy,v0yyy
         integer use2ndOrderAD,use4thOrderAD,useImplicit4thOrderAD,
     & includeADinPressure
         real ad21,ad22,ad41,ad42,cd22,cd42
         real adCoeffu,adCoeffv,adCoeffw,adCoeff2,adCoeff4
         ! -- variables for the free surface: 
         real det,deti,detMin,detr,dets
         real xr,yr,zr,xs,ys,zs, xt,yt,zt
         real xrr,yrr,xss,yss
         real rxi,ryi,rzi, sxi,syi,szi, txi,tyi,tzi
         real rxr,ryr,rzr, sxr,syr,szr, txr,tyr,tzr
         real rxs,rys,rzs, sxs,sys,szs, txs,tys,tzs
         real rxt,ryt,rzt, sxt,syt,szt, txt,tyt,tzt
         real pAtmosphere,surfaceTension,meanCurvature
         ! -- variables for boundary forcing (bcData)
         integer dim(0:1,0:2,0:1,0:2), addBoundaryForcing(0:1,0:2)
         real bcf0(0:*)
         integer*8 bcOffset(0:1,0:2)
         real bcf
        !     .....begin statement functions
         real rx,ry,rz,sx,sy,sz,tx,ty,tz
         real dr(0:2), dx(0:2)
         real dr12
         real dr22
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
         real urrs2
         real urss2
         real urrt2
         real usst2
         real urtt2
         real ustt2
         real urrrr2
         real ussss2
         real utttt2
         real urrss2
         real urrtt2
         real usstt2
         real urrrs2
         real ursss2
         real urrrt2
         real ussst2
         real urttt2
         real usttt2
         real rsxyr2
         real rsxys2
         real rsxyt2
         real rsxyrr2
         real rsxyss2
         real rsxyrs2
         real rsxytt2
         real rsxyrt2
         real rsxyst2
         real rsxyrrr2
         real rsxysss2
         real rsxyttt2
         real rsxyrrs2
         real rsxyrss2
         real rsxyrrt2
         real rsxysst2
         real rsxyrtt2
         real rsxystt2
         real rsxyrrrr2
         real rsxyssss2
         real rsxytttt2
         real rsxyrrss2
         real rsxyrrtt2
         real rsxysstt2
         real ux21
         real uy21
         real uz21
         real ux22
         real uy22
         real uz22
         real ux23
         real uy23
         real uz23
         real rsxyx21
         real rsxyx22
         real rsxyy22
         real rsxyx23
         real rsxyy23
         real rsxyz23
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
         real rsxyxx22
         real rsxyyy22
         real rsxyxy22
         real rsxyxxx22
         real rsxyxxy22
         real rsxyxyy22
         real rsxyyyy22
         real uxxx22
         real uxxy22
         real uxyy22
         real uyyy22
         real uxxxx22
         real uxxxy22
         real uxxyy22
         real uxyyy22
         real uyyyy22
         real uLapSq22
         real uxx23
         real uyy23
         real uzz23
         real uxy23
         real uxz23
         real uyz23
         real ulaplacian23
         real dx12
         real dx22
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
         real uLapSq22r
         real uxxx23r
         real uyyy23r
         real uzzz23r
         real uxxy23r
         real uxyy23r
         real uxxz23r
         real uyyz23r
         real uxzz23r
         real uyzz23r
         real uxxxx23r
         real uyyyy23r
         real uzzzz23r
         real uxxyy23r
         real uxxzz23r
         real uyyzz23r
         real dr14
         real dr24
         real ur4
         real us4
         real ut4
         real urr4
         real uss4
         real utt4
         real urs4
         real urt4
         real ust4
         real rsxyr4
         real rsxys4
         real rsxyt4
         real ux41
         real uy41
         real uz41
         real ux42
         real uy42
         real uz42
         real ux43
         real uy43
         real uz43
         real rsxyx41
         real rsxyx42
         real rsxyy42
         real rsxyx43
         real rsxyy43
         real rsxyz43
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
         real dx41
         real dx42
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
         real dr112
         real dr122
         real udfr2
         real udfs2
         real udft2
         real udfrr2
         real udfss2
         real udfrs2
         real udftt2
         real udfrt2
         real udfst2
         real udfrrr2
         real udfsss2
         real udfttt2
         real udfrrs2
         real udfrss2
         real udfrrt2
         real udfsst2
         real udfrtt2
         real udfstt2
         real udfrrrr2
         real udfssss2
         real udftttt2
         real udfrrss2
         real udfrrtt2
         real udfsstt2
         real udfrrrs2
         real udfrsss2
         real udfrrrt2
         real udfssst2
         real udfrttt2
         real udfsttt2
         real rsxy1r2
         real rsxy1s2
         real rsxy1t2
         real rsxy1rr2
         real rsxy1ss2
         real rsxy1rs2
         real rsxy1tt2
         real rsxy1rt2
         real rsxy1st2
         real rsxy1rrr2
         real rsxy1sss2
         real rsxy1ttt2
         real rsxy1rrs2
         real rsxy1rss2
         real rsxy1rrt2
         real rsxy1sst2
         real rsxy1rtt2
         real rsxy1stt2
         real rsxy1rrrr2
         real rsxy1ssss2
         real rsxy1tttt2
         real rsxy1rrss2
         real rsxy1rrtt2
         real rsxy1sstt2
         real udfx21
         real udfy21
         real udfz21
         real udfx22
         real udfy22
         real udfz22
         real udfx23
         real udfy23
         real udfz23
         real rsxy1x21
         real rsxy1x22
         real rsxy1y22
         real rsxy1x23
         real rsxy1y23
         real rsxy1z23
         real udfxx21
         real udfyy21
         real udfxy21
         real udfxz21
         real udfyz21
         real udfzz21
         real udflaplacian21
         real udfxx22
         real udfyy22
         real udfxy22
         real udfxz22
         real udfyz22
         real udfzz22
         real udflaplacian22
         real rsxy1xx22
         real rsxy1yy22
         real rsxy1xy22
         real rsxy1xxx22
         real rsxy1xxy22
         real rsxy1xyy22
         real rsxy1yyy22
         real udfxxx22
         real udfxxy22
         real udfxyy22
         real udfyyy22
         real udfxxxx22
         real udfxxxy22
         real udfxxyy22
         real udfxyyy22
         real udfyyyy22
         real udfLapSq22
         real udfxx23
         real udfyy23
         real udfzz23
         real udfxy23
         real udfxz23
         real udfyz23
         real udflaplacian23
         real dx112
         real dx122
         real udfx23r
         real udfy23r
         real udfz23r
         real udfxx23r
         real udfyy23r
         real udfxy23r
         real udfzz23r
         real udfxz23r
         real udfyz23r
         real udfx21r
         real udfy21r
         real udfz21r
         real udfxx21r
         real udfyy21r
         real udfzz21r
         real udfxy21r
         real udfxz21r
         real udfyz21r
         real udflaplacian21r
         real udfx22r
         real udfy22r
         real udfz22r
         real udfxx22r
         real udfyy22r
         real udfzz22r
         real udfxy22r
         real udfxz22r
         real udfyz22r
         real udflaplacian22r
         real udflaplacian23r
         real udfxxx22r
         real udfyyy22r
         real udfxxy22r
         real udfxyy22r
         real udfxxxx22r
         real udfyyyy22r
         real udfxxyy22r
         real udfLapSq22r
         real udfxxx23r
         real udfyyy23r
         real udfzzz23r
         real udfxxy23r
         real udfxyy23r
         real udfxxz23r
         real udfyyz23r
         real udfxzz23r
         real udfyzz23r
         real udfxxxx23r
         real udfyyyy23r
         real udfzzzz23r
         real udfxxyy23r
         real udfxxzz23r
         real udfyyzz23r
         real dr114
         real dr124
         real udfr4
         real udfs4
         real udft4
         real udfrr4
         real udfss4
         real udftt4
         real udfrs4
         real udfrt4
         real udfst4
         real rsxy1r4
         real rsxy1s4
         real rsxy1t4
         real udfx41
         real udfy41
         real udfz41
         real udfx42
         real udfy42
         real udfz42
         real udfx43
         real udfy43
         real udfz43
         real rsxy1x41
         real rsxy1x42
         real rsxy1y42
         real rsxy1x43
         real rsxy1y43
         real rsxy1z43
         real udfxx41
         real udfyy41
         real udfxy41
         real udfxz41
         real udfyz41
         real udfzz41
         real udflaplacian41
         real udfxx42
         real udfyy42
         real udfxy42
         real udfxz42
         real udfyz42
         real udfzz42
         real udflaplacian42
         real udfxx43
         real udfyy43
         real udfzz43
         real udfxy43
         real udfxz43
         real udfyz43
         real udflaplacian43
         real dx141
         real dx142
         real udfx43r
         real udfy43r
         real udfz43r
         real udfxx43r
         real udfyy43r
         real udfzz43r
         real udfxy43r
         real udfxz43r
         real udfyz43r
         real udfx41r
         real udfy41r
         real udfz41r
         real udfxx41r
         real udfyy41r
         real udfzz41r
         real udfxy41r
         real udfxz41r
         real udfyz41r
         real udflaplacian41r
         real udfx42r
         real udfy42r
         real udfz42r
         real udfxx42r
         real udfyy42r
         real udfzz42r
         real udfxy42r
         real udfxz42r
         real udfyz42r
         real udflaplacian42r
         real udflaplacian43r
         real ad2,ad23,ad4,ad43
        !     --- begin statement functions
        ! Here is the the generic boundary condition forcing array. It uses the bcOffset(side,axis) values as an
        ! an offset from the bcf0 array to access the bcf10, bcf01, bcf11, ... arrays
         bcf(side,axis,i1,i2,i3,m) = bcf0(bcOffset(side,axis) + (i1-
     & dim(0,0,side,axis)+(dim(1,0,side,axis)-dim(0,0,side,axis)+1)* (
     & i2-dim(0,1,side,axis)+(dim(1,1,side,axis)-dim(0,1,side,axis)+1)
     & * (i3-dim(0,2,side,axis)+(dim(1,2,side,axis)-dim(0,2,side,axis)
     & +1)*(m)))))
        !    --- 2nd order 2D artificial diffusion ---
         ad2(c)=    (u(i1+1,i2,i3,c)-4.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c)  
     & +u(i1,i2+1,i3,c)                 +u(i1,i2-1,i3,c))
        !    --- 2nd order 3D artificial diffusion ---
         ad23(c)= (u(i1+1,i2,i3,c)-6.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c)   +
     & u(i1,i2+1,i3,c)                   +u(i1,i2-1,i3,c)  +u(i1,i2,
     & i3+1,c)                   +u(i1,i2,i3-1,c))
        !     ---fourth-order artificial diffusion in 2D
         ad4(c)= (   -u(i1+2,i2,i3,c)-u(i1-2,i2,i3,c)   -u(i1,i2+2,i3,
     & c)-u(i1,i2-2,i3,c)   +4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)   +u(
     & i1,i2+1,i3,c)+u(i1,i2-1,i3,c))  -12.*u(i1,i2,i3,c) )
        !     ---fourth-order artificial diffusion in 3D
         ad43(c)= (   -u(i1+2,i2,i3,c)-u(i1-2,i2,i3,c)  -u(i1,i2+2,i3,
     & c)-u(i1,i2-2,i3,c)  -u(i1,i2,i3+2,c)-u(i1,i2,i3-2,c)  +4.*(u(
     & i1+1,i2,i3,c)+u(i1-1,i2,i3,c)  +u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c)
     &   +u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c)) -18.*u(i1,i2,i3,c) )
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
         ! define these for the derivatives of udf:
         rsxy1(i1,i2,i3,kd,ks) = rsxy(i1,i2,i3,kd,ks)
         dr1(kd) = dr(kd)
         dx1(kd) = dx(kd)
        !     The next macro call will define the difference approximation statement functions
         dr12(kd) = 1./(2.*dr(kd))
         dr22(kd) = 1./(dr(kd)**2)
         ur2(i1,i2,i3,kd)=(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))*dr12(0)
         us2(i1,i2,i3,kd)=(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))*dr12(1)
         ut2(i1,i2,i3,kd)=(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))*dr12(2)
         urr2(i1,i2,i3,kd)=(-2.*u(i1,i2,i3,kd)+(u(i1+1,i2,i3,kd)+u(i1-
     & 1,i2,i3,kd)) )*dr22(0)
         uss2(i1,i2,i3,kd)=(-2.*u(i1,i2,i3,kd)+(u(i1,i2+1,i3,kd)+u(i1,
     & i2-1,i3,kd)) )*dr22(1)
         urs2(i1,i2,i3,kd)=(ur2(i1,i2+1,i3,kd)-ur2(i1,i2-1,i3,kd))*
     & dr12(1)
         utt2(i1,i2,i3,kd)=(-2.*u(i1,i2,i3,kd)+(u(i1,i2,i3+1,kd)+u(i1,
     & i2,i3-1,kd)) )*dr22(2)
         urt2(i1,i2,i3,kd)=(ur2(i1,i2,i3+1,kd)-ur2(i1,i2,i3-1,kd))*
     & dr12(2)
         ust2(i1,i2,i3,kd)=(us2(i1,i2,i3+1,kd)-us2(i1,i2,i3-1,kd))*
     & dr12(2)
         urrr2(i1,i2,i3,kd)=(-2.*(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))+(
     & u(i1+2,i2,i3,kd)-u(i1-2,i2,i3,kd)) )*dr22(0)*dr12(0)
         usss2(i1,i2,i3,kd)=(-2.*(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))+(
     & u(i1,i2+2,i3,kd)-u(i1,i2-2,i3,kd)) )*dr22(1)*dr12(1)
         uttt2(i1,i2,i3,kd)=(-2.*(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))+(
     & u(i1,i2,i3+2,kd)-u(i1,i2,i3-2,kd)) )*dr22(1)*dr12(2)
         urrs2(i1,i2,i3,kd)=( urr2(i1,i2+1,i3,kd)-urr2(i1,i2-1,i3,kd))
     & /(2.*dr(1))
         urss2(i1,i2,i3,kd)=( uss2(i1+1,i2,i3,kd)-uss2(i1-1,i2,i3,kd))
     & /(2.*dr(0))
         urrt2(i1,i2,i3,kd)=( urr2(i1,i2,i3+1,kd)-urr2(i1,i2,i3-1,kd))
     & /(2.*dr(2))
         usst2(i1,i2,i3,kd)=( uss2(i1,i2,i3+1,kd)-uss2(i1,i2,i3-1,kd))
     & /(2.*dr(2))
         urtt2(i1,i2,i3,kd)=( utt2(i1+1,i2,i3,kd)-utt2(i1-1,i2,i3,kd))
     & /(2.*dr(0))
         ustt2(i1,i2,i3,kd)=( utt2(i1,i2+1,i3,kd)-utt2(i1,i2-1,i3,kd))
     & /(2.*dr(1))
         urrrr2(i1,i2,i3,kd)=(6.*u(i1,i2,i3,kd)-4.*(u(i1+1,i2,i3,kd)+u(
     & i1-1,i2,i3,kd))+(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd)) )/(dr(0)**
     & 4)
         ussss2(i1,i2,i3,kd)=(6.*u(i1,i2,i3,kd)-4.*(u(i1,i2+1,i3,kd)+u(
     & i1,i2-1,i3,kd))+(u(i1,i2+2,i3,kd)+u(i1,i2-2,i3,kd)) )/(dr(1)**
     & 4)
         utttt2(i1,i2,i3,kd)=(6.*u(i1,i2,i3,kd)-4.*(u(i1,i2,i3+1,kd)+u(
     & i1,i2,i3-1,kd))+(u(i1,i2,i3+2,kd)+u(i1,i2,i3-2,kd)) )/(dr(2)**
     & 4)
         urrss2(i1,i2,i3,kd)=( 4.*u(i1,i2,i3,kd)-2.*(u(i1+1,i2,i3,kd)+
     & u(i1-1,i2,i3,kd)+u(i1,i2+1,i3,kd)+u(i1,i2-1,i3,kd))+   (u(i1+1,
     & i2+1,i3,kd)+u(i1-1,i2+1,i3,kd)+u(i1+1,i2-1,i3,kd)+u(i1-1,i2-1,
     & i3,kd)) )/(dr(0)**2*dr(1)**2)
         urrtt2(i1,i2,i3,kd)=( 4.*u(i1,i2,i3,kd)-2.*(u(i1+1,i2,i3,kd)+
     & u(i1-1,i2,i3,kd)+u(i1,i2,i3+1,kd)+u(i1,i2,i3-1,kd))+   (u(i1+1,
     & i2,i3+1,kd)+u(i1-1,i2,i3+1,kd)+u(i1+1,i2,i3-1,kd)+u(i1-1,i2,i3-
     & 1,kd)) )/(dr(0)**2*dr(2)**2)
         usstt2(i1,i2,i3,kd)=( 4.*u(i1,i2,i3,kd)-2.*(u(i1,i2+1,i3,kd)  
     & +u(i1,i2-1,i3,kd)+  u(i1,i2  ,i3+1,kd)+u(i1,i2  ,i3-1,kd))+   (
     & u(i1,i2+1,i3+1,kd)+u(i1,i2-1,i3+1,kd)+u(i1,i2+1,i3-1,kd)+u(i1,
     & i2-1,i3-1,kd)) )/(dr(1)**2*dr(2)**2)
         urrrs2(i1,i2,i3,kd)=( urrr2(i1,i2+1,i3,kd)-urrr2(i1,i2-1,i3,
     & kd))/(2.*dr(1))
         ursss2(i1,i2,i3,kd)=( usss2(i1+1,i2,i3,kd)-usss2(i1-1,i2,i3,
     & kd))/(2.*dr(0))
         urrrt2(i1,i2,i3,kd)=( urrr2(i1,i2,i3+1,kd)-urrr2(i1,i2,i3-1,
     & kd))/(2.*dr(2))
         ussst2(i1,i2,i3,kd)=( usss2(i1,i2,i3+1,kd)-usss2(i1,i2,i3-1,
     & kd))/(2.*dr(2))
         urttt2(i1,i2,i3,kd)=( uttt2(i1+1,i2,i3,kd)-uttt2(i1-1,i2,i3,
     & kd))/(2.*dr(0))
         usttt2(i1,i2,i3,kd)=( uttt2(i1,i2+1,i3,kd)-uttt2(i1,i2-1,i3,
     & kd))/(2.*dr(1))
         rsxyr2(i1,i2,i3,m,n)=(rsxy(i1+1,i2,i3,m,n)-rsxy(i1-1,i2,i3,m,
     & n))*dr12(0)
         rsxys2(i1,i2,i3,m,n)=(rsxy(i1,i2+1,i3,m,n)-rsxy(i1,i2-1,i3,m,
     & n))*dr12(1)
         rsxyt2(i1,i2,i3,m,n)=(rsxy(i1,i2,i3+1,m,n)-rsxy(i1,i2,i3-1,m,
     & n))*dr12(2)
         rsxyrr2(i1,i2,i3,m,n)=(-2.*rsxy(i1,i2,i3,m,n)+(rsxy(i1+1,i2,
     & i3,m,n)+rsxy(i1-1,i2,i3,m,n)) )*dr22(0)
         rsxyss2(i1,i2,i3,m,n)=(-2.*rsxy(i1,i2,i3,m,n)+(rsxy(i1,i2+1,
     & i3,m,n)+rsxy(i1,i2-1,i3,m,n)) )*dr22(1)
         rsxyrs2(i1,i2,i3,m,n)=(rsxyr2(i1,i2+1,i3,m,n)-rsxyr2(i1,i2-1,
     & i3,m,n))*dr12(1)
         rsxytt2(i1,i2,i3,m,n)=(-2.*rsxy(i1,i2,i3,m,n)+(rsxy(i1,i2,i3+
     & 1,m,n)+rsxy(i1,i2,i3-1,m,n)) )*dr22(2)
         rsxyrt2(i1,i2,i3,m,n)=(rsxyr2(i1,i2,i3+1,m,n)-rsxyr2(i1,i2,i3-
     & 1,m,n))*dr12(2)
         rsxyst2(i1,i2,i3,m,n)=(rsxys2(i1,i2,i3+1,m,n)-rsxys2(i1,i2,i3-
     & 1,m,n))*dr12(2)
         rsxyrrr2(i1,i2,i3,m,n)=(-2.*(rsxy(i1+1,i2,i3,m,n)-rsxy(i1-1,
     & i2,i3,m,n))+(rsxy(i1+2,i2,i3,m,n)-rsxy(i1-2,i2,i3,m,n)) )*dr22(
     & 0)*dr12(0)
         rsxysss2(i1,i2,i3,m,n)=(-2.*(rsxy(i1,i2+1,i3,m,n)-rsxy(i1,i2-
     & 1,i3,m,n))+(rsxy(i1,i2+2,i3,m,n)-rsxy(i1,i2-2,i3,m,n)) )*dr22(
     & 1)*dr12(1)
         rsxyttt2(i1,i2,i3,m,n)=(-2.*(rsxy(i1,i2,i3+1,m,n)-rsxy(i1,i2,
     & i3-1,m,n))+(rsxy(i1,i2,i3+2,m,n)-rsxy(i1,i2,i3-2,m,n)) )*dr22(
     & 1)*dr12(2)
         rsxyrrs2(i1,i2,i3,m,n)=( rsxyrr2(i1,i2+1,i3,m,n)-rsxyrr2(i1,
     & i2-1,i3,m,n))/(2.*dr(1))
         rsxyrss2(i1,i2,i3,m,n)=( rsxyss2(i1+1,i2,i3,m,n)-rsxyss2(i1-1,
     & i2,i3,m,n))/(2.*dr(0))
         rsxyrrt2(i1,i2,i3,m,n)=( rsxyrr2(i1,i2,i3+1,m,n)-rsxyrr2(i1,
     & i2,i3-1,m,n))/(2.*dr(2))
         rsxysst2(i1,i2,i3,m,n)=( rsxyss2(i1,i2,i3+1,m,n)-rsxyss2(i1,
     & i2,i3-1,m,n))/(2.*dr(2))
         rsxyrtt2(i1,i2,i3,m,n)=( rsxytt2(i1+1,i2,i3,m,n)-rsxytt2(i1-1,
     & i2,i3,m,n))/(2.*dr(0))
         rsxystt2(i1,i2,i3,m,n)=( rsxytt2(i1,i2+1,i3,m,n)-rsxytt2(i1,
     & i2-1,i3,m,n))/(2.*dr(1))
         rsxyrrrr2(i1,i2,i3,m,n)=(6.*rsxy(i1,i2,i3,m,n)-4.*(rsxy(i1+1,
     & i2,i3,m,n)+rsxy(i1-1,i2,i3,m,n))+(rsxy(i1+2,i2,i3,m,n)+rsxy(i1-
     & 2,i2,i3,m,n)) )/(dr(0)**4)
         rsxyssss2(i1,i2,i3,m,n)=(6.*rsxy(i1,i2,i3,m,n)-4.*(rsxy(i1,i2+
     & 1,i3,m,n)+rsxy(i1,i2-1,i3,m,n))+(rsxy(i1,i2+2,i3,m,n)+rsxy(i1,
     & i2-2,i3,m,n)) )/(dr(1)**4)
         rsxytttt2(i1,i2,i3,m,n)=(6.*rsxy(i1,i2,i3,m,n)-4.*(rsxy(i1,i2,
     & i3+1,m,n)+rsxy(i1,i2,i3-1,m,n))+(rsxy(i1,i2,i3+2,m,n)+rsxy(i1,
     & i2,i3-2,m,n)) )/(dr(2)**4)
         rsxyrrss2(i1,i2,i3,m,n)=( 4.*rsxy(i1,i2,i3,m,n)-2.*(rsxy(i1+1,
     & i2,i3,m,n)+rsxy(i1-1,i2,i3,m,n)+rsxy(i1,i2+1,i3,m,n)+rsxy(i1,
     & i2-1,i3,m,n))+   (rsxy(i1+1,i2+1,i3,m,n)+rsxy(i1-1,i2+1,i3,m,n)
     & +rsxy(i1+1,i2-1,i3,m,n)+rsxy(i1-1,i2-1,i3,m,n)) )/(dr(0)**2*dr(
     & 1)**2)
         rsxyrrtt2(i1,i2,i3,m,n)=( 4.*rsxy(i1,i2,i3,m,n)-2.*(rsxy(i1+1,
     & i2,i3,m,n)+rsxy(i1-1,i2,i3,m,n)+rsxy(i1,i2,i3+1,m,n)+rsxy(i1,
     & i2,i3-1,m,n))+   (rsxy(i1+1,i2,i3+1,m,n)+rsxy(i1-1,i2,i3+1,m,n)
     & +rsxy(i1+1,i2,i3-1,m,n)+rsxy(i1-1,i2,i3-1,m,n)) )/(dr(0)**2*dr(
     & 2)**2)
         rsxysstt2(i1,i2,i3,m,n)=( 4.*rsxy(i1,i2,i3,m,n)-2.*(rsxy(i1,
     & i2+1,i3,m,n)  +rsxy(i1,i2-1,i3,m,n)+  rsxy(i1,i2  ,i3+1,m,n)+
     & rsxy(i1,i2  ,i3-1,m,n))+   (rsxy(i1,i2+1,i3+1,m,n)+rsxy(i1,i2-
     & 1,i3+1,m,n)+rsxy(i1,i2+1,i3-1,m,n)+rsxy(i1,i2-1,i3-1,m,n)) )/(
     & dr(1)**2*dr(2)**2)
         ux21(i1,i2,i3,kd)= rsxy(i1,i2,i3,0,0)*ur2(i1,i2,i3,kd)
         uy21(i1,i2,i3,kd)=0
         uz21(i1,i2,i3,kd)=0
         ux22(i1,i2,i3,kd)= rsxy(i1,i2,i3,0,0)*ur2(i1,i2,i3,kd)+rsxy(
     & i1,i2,i3,1,0)*us2(i1,i2,i3,kd)
         uy22(i1,i2,i3,kd)= rsxy(i1,i2,i3,0,1)*ur2(i1,i2,i3,kd)+rsxy(
     & i1,i2,i3,1,1)*us2(i1,i2,i3,kd)
         uz22(i1,i2,i3,kd)=0
         ux23(i1,i2,i3,kd)=rsxy(i1,i2,i3,0,0)*ur2(i1,i2,i3,kd)+rsxy(i1,
     & i2,i3,1,0)*us2(i1,i2,i3,kd)+rsxy(i1,i2,i3,2,0)*ut2(i1,i2,i3,kd)
         uy23(i1,i2,i3,kd)=rsxy(i1,i2,i3,0,1)*ur2(i1,i2,i3,kd)+rsxy(i1,
     & i2,i3,1,1)*us2(i1,i2,i3,kd)+rsxy(i1,i2,i3,2,1)*ut2(i1,i2,i3,kd)
         uz23(i1,i2,i3,kd)=rsxy(i1,i2,i3,0,2)*ur2(i1,i2,i3,kd)+rsxy(i1,
     & i2,i3,1,2)*us2(i1,i2,i3,kd)+rsxy(i1,i2,i3,2,2)*ut2(i1,i2,i3,kd)
         rsxyx21(i1,i2,i3,m,n)= rsxy(i1,i2,i3,0,0)*rsxyr2(i1,i2,i3,m,n)
         rsxyx22(i1,i2,i3,m,n)= rsxy(i1,i2,i3,0,0)*rsxyr2(i1,i2,i3,m,n)
     & +rsxy(i1,i2,i3,1,0)*rsxys2(i1,i2,i3,m,n)
         rsxyy22(i1,i2,i3,m,n)= rsxy(i1,i2,i3,0,1)*rsxyr2(i1,i2,i3,m,n)
     & +rsxy(i1,i2,i3,1,1)*rsxys2(i1,i2,i3,m,n)
         rsxyx23(i1,i2,i3,m,n)=rsxy(i1,i2,i3,0,0)*rsxyr2(i1,i2,i3,m,n)+
     & rsxy(i1,i2,i3,1,0)*rsxys2(i1,i2,i3,m,n)+rsxy(i1,i2,i3,2,0)*
     & rsxyt2(i1,i2,i3,m,n)
         rsxyy23(i1,i2,i3,m,n)=rsxy(i1,i2,i3,0,1)*rsxyr2(i1,i2,i3,m,n)+
     & rsxy(i1,i2,i3,1,1)*rsxys2(i1,i2,i3,m,n)+rsxy(i1,i2,i3,2,1)*
     & rsxyt2(i1,i2,i3,m,n)
         rsxyz23(i1,i2,i3,m,n)=rsxy(i1,i2,i3,0,2)*rsxyr2(i1,i2,i3,m,n)+
     & rsxy(i1,i2,i3,1,2)*rsxys2(i1,i2,i3,m,n)+rsxy(i1,i2,i3,2,2)*
     & rsxyt2(i1,i2,i3,m,n)
         uxx21(i1,i2,i3,kd)=(rsxy(i1,i2,i3,0,0)**2)*urr2(i1,i2,i3,kd)+(
     & rsxyx22(i1,i2,i3,0,0))*ur2(i1,i2,i3,kd)
         uyy21(i1,i2,i3,kd)=0
         uxy21(i1,i2,i3,kd)=0
         uxz21(i1,i2,i3,kd)=0
         uyz21(i1,i2,i3,kd)=0
         uzz21(i1,i2,i3,kd)=0
         ulaplacian21(i1,i2,i3,kd)=uxx21(i1,i2,i3,kd)
         uxx22(i1,i2,i3,kd)=(rsxy(i1,i2,i3,0,0)**2)*urr2(i1,i2,i3,kd)+
     & 2.*(rsxy(i1,i2,i3,0,0)*rsxy(i1,i2,i3,1,0))*urs2(i1,i2,i3,kd)+(
     & rsxy(i1,i2,i3,1,0)**2)*uss2(i1,i2,i3,kd)+(rsxyx22(i1,i2,i3,0,0)
     & )*ur2(i1,i2,i3,kd)+(rsxyx22(i1,i2,i3,1,0))*us2(i1,i2,i3,kd)
         uyy22(i1,i2,i3,kd)=(rsxy(i1,i2,i3,0,1)**2)*urr2(i1,i2,i3,kd)+
     & 2.*(rsxy(i1,i2,i3,0,1)*rsxy(i1,i2,i3,1,1))*urs2(i1,i2,i3,kd)+(
     & rsxy(i1,i2,i3,1,1)**2)*uss2(i1,i2,i3,kd)+(rsxyy22(i1,i2,i3,0,1)
     & )*ur2(i1,i2,i3,kd)+(rsxyy22(i1,i2,i3,1,1))*us2(i1,i2,i3,kd)
         uxy22(i1,i2,i3,kd)=rsxy(i1,i2,i3,0,0)*rsxy(i1,i2,i3,0,1)*urr2(
     & i1,i2,i3,kd)+(rsxy(i1,i2,i3,0,0)*rsxy(i1,i2,i3,1,1)+rsxy(i1,i2,
     & i3,0,1)*rsxy(i1,i2,i3,1,0))*urs2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,
     & 0)*rsxy(i1,i2,i3,1,1)*uss2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,0,1)*
     & ur2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,1,1)*us2(i1,i2,i3,kd)
         uxz22(i1,i2,i3,kd)=0
         uyz22(i1,i2,i3,kd)=0
         uzz22(i1,i2,i3,kd)=0
         ulaplacian22(i1,i2,i3,kd)=(rsxy(i1,i2,i3,0,0)**2+rsxy(i1,i2,
     & i3,0,1)**2)*urr2(i1,i2,i3,kd)+2.*(rsxy(i1,i2,i3,0,0)*rsxy(i1,
     & i2,i3,1,0)+ rsxy(i1,i2,i3,0,1)*rsxy(i1,i2,i3,1,1))*urs2(i1,i2,
     & i3,kd)+(rsxy(i1,i2,i3,1,0)**2+rsxy(i1,i2,i3,1,1)**2)*uss2(i1,
     & i2,i3,kd)+(rsxyx22(i1,i2,i3,0,0)+rsxyy22(i1,i2,i3,0,1))*ur2(i1,
     & i2,i3,kd)+(rsxyx22(i1,i2,i3,1,0)+rsxyy22(i1,i2,i3,1,1))*us2(i1,
     & i2,i3,kd)
c ..... start: 3rd and 4th derivatives, 2D ....
         rsxyxx22(i1,i2,i3,m,n)=(rsxy(i1,i2,i3,0,0)**2)*rsxyrr2(i1,i2,
     & i3,m,n)+2.*(rsxy(i1,i2,i3,0,0)*rsxy(i1,i2,i3,1,0))*rsxyrs2(i1,
     & i2,i3,m,n)+(rsxy(i1,i2,i3,1,0)**2)*rsxyss2(i1,i2,i3,m,n)+(
     & rsxyx22(i1,i2,i3,0,0))*rsxyr2(i1,i2,i3,m,n)+(rsxyx22(i1,i2,i3,
     & 1,0))*rsxys2(i1,i2,i3,m,n)
         rsxyyy22(i1,i2,i3,m,n)=(rsxy(i1,i2,i3,0,1)**2)*rsxyrr2(i1,i2,
     & i3,m,n)+2.*(rsxy(i1,i2,i3,0,1)*rsxy(i1,i2,i3,1,1))*rsxyrs2(i1,
     & i2,i3,m,n)+(rsxy(i1,i2,i3,1,1)**2)*rsxyss2(i1,i2,i3,m,n)+(
     & rsxyy22(i1,i2,i3,0,1))*rsxyr2(i1,i2,i3,m,n)+(rsxyy22(i1,i2,i3,
     & 1,1))*rsxys2(i1,i2,i3,m,n)
         rsxyxy22(i1,i2,i3,m,n)=rsxy(i1,i2,i3,0,0)*rsxy(i1,i2,i3,0,1)*
     & rsxyrr2(i1,i2,i3,m,n)+(rsxy(i1,i2,i3,0,0)*rsxy(i1,i2,i3,1,1)+
     & rsxy(i1,i2,i3,0,1)*rsxy(i1,i2,i3,1,0))*rsxyrs2(i1,i2,i3,m,n)+
     & rsxy(i1,i2,i3,1,0)*rsxy(i1,i2,i3,1,1)*rsxyss2(i1,i2,i3,m,n)+
     & rsxyx22(i1,i2,i3,0,1)*rsxyr2(i1,i2,i3,m,n)+rsxyx22(i1,i2,i3,1,
     & 1)*rsxys2(i1,i2,i3,m,n)
         rsxyxxx22(i1,i2,i3,m,n)=rsxyxx22(i1,i2,i3,0,0)*rsxyr2(i1,i2,
     & i3,m,n)+rsxyxx22(i1,i2,i3,1,0)*rsxys2(i1,i2,i3,m,n)+rsxyx22(i1,
     & i2,i3,0,0)*(rsxy(i1,i2,i3,0,0)*rsxyrr2(i1,i2,i3,m,n)+rsxy(i1,
     & i2,i3,1,0)*rsxyrs2(i1,i2,i3,m,n))+rsxy(i1,i2,i3,0,0)*(rsxyx22(
     & i1,i2,i3,0,0)*rsxyrr2(i1,i2,i3,m,n)+rsxyx22(i1,i2,i3,1,0)*
     & rsxyrs2(i1,i2,i3,m,n))+rsxyx22(i1,i2,i3,1,0)*(rsxy(i1,i2,i3,0,
     & 0)*rsxyrs2(i1,i2,i3,m,n)+rsxy(i1,i2,i3,1,0)*rsxyss2(i1,i2,i3,m,
     & n))+rsxy(i1,i2,i3,1,0)*(rsxyx22(i1,i2,i3,0,0)*rsxyrs2(i1,i2,i3,
     & m,n)+rsxyx22(i1,i2,i3,1,0)*rsxyss2(i1,i2,i3,m,n))+rsxy(i1,i2,
     & i3,0,0)*(rsxyx22(i1,i2,i3,0,0)*rsxyrr2(i1,i2,i3,m,n)+rsxyx22(
     & i1,i2,i3,1,0)*rsxyrs2(i1,i2,i3,m,n)+rsxy(i1,i2,i3,0,0)*(rsxy(
     & i1,i2,i3,0,0)*rsxyrrr2(i1,i2,i3,m,n)+rsxy(i1,i2,i3,1,0)*
     & rsxyrrs2(i1,i2,i3,m,n))+rsxy(i1,i2,i3,1,0)*(rsxy(i1,i2,i3,0,0)*
     & rsxyrrs2(i1,i2,i3,m,n)+rsxy(i1,i2,i3,1,0)*rsxyrss2(i1,i2,i3,m,
     & n)))+rsxy(i1,i2,i3,1,0)*(rsxyx22(i1,i2,i3,0,0)*rsxyrs2(i1,i2,
     & i3,m,n)+rsxyx22(i1,i2,i3,1,0)*rsxyss2(i1,i2,i3,m,n)+rsxy(i1,i2,
     & i3,0,0)*(rsxy(i1,i2,i3,0,0)*rsxyrrs2(i1,i2,i3,m,n)+rsxy(i1,i2,
     & i3,1,0)*rsxyrss2(i1,i2,i3,m,n))+rsxy(i1,i2,i3,1,0)*(rsxy(i1,i2,
     & i3,0,0)*rsxyrss2(i1,i2,i3,m,n)+rsxy(i1,i2,i3,1,0)*rsxysss2(i1,
     & i2,i3,m,n)))
         rsxyxxy22(i1,i2,i3,m,n)=rsxyxy22(i1,i2,i3,0,0)*rsxyr2(i1,i2,
     & i3,m,n)+rsxyxy22(i1,i2,i3,1,0)*rsxys2(i1,i2,i3,m,n)+rsxyx22(i1,
     & i2,i3,0,1)*(rsxy(i1,i2,i3,0,0)*rsxyrr2(i1,i2,i3,m,n)+rsxy(i1,
     & i2,i3,1,0)*rsxyrs2(i1,i2,i3,m,n))+rsxy(i1,i2,i3,0,0)*(rsxyx22(
     & i1,i2,i3,0,1)*rsxyrr2(i1,i2,i3,m,n)+rsxyx22(i1,i2,i3,1,1)*
     & rsxyrs2(i1,i2,i3,m,n))+rsxyx22(i1,i2,i3,1,1)*(rsxy(i1,i2,i3,0,
     & 0)*rsxyrs2(i1,i2,i3,m,n)+rsxy(i1,i2,i3,1,0)*rsxyss2(i1,i2,i3,m,
     & n))+rsxy(i1,i2,i3,1,0)*(rsxyx22(i1,i2,i3,0,1)*rsxyrs2(i1,i2,i3,
     & m,n)+rsxyx22(i1,i2,i3,1,1)*rsxyss2(i1,i2,i3,m,n))+rsxy(i1,i2,
     & i3,0,1)*(rsxyx22(i1,i2,i3,0,0)*rsxyrr2(i1,i2,i3,m,n)+rsxyx22(
     & i1,i2,i3,1,0)*rsxyrs2(i1,i2,i3,m,n)+rsxy(i1,i2,i3,0,0)*(rsxy(
     & i1,i2,i3,0,0)*rsxyrrr2(i1,i2,i3,m,n)+rsxy(i1,i2,i3,1,0)*
     & rsxyrrs2(i1,i2,i3,m,n))+rsxy(i1,i2,i3,1,0)*(rsxy(i1,i2,i3,0,0)*
     & rsxyrrs2(i1,i2,i3,m,n)+rsxy(i1,i2,i3,1,0)*rsxyrss2(i1,i2,i3,m,
     & n)))+rsxy(i1,i2,i3,1,1)*(rsxyx22(i1,i2,i3,0,0)*rsxyrs2(i1,i2,
     & i3,m,n)+rsxyx22(i1,i2,i3,1,0)*rsxyss2(i1,i2,i3,m,n)+rsxy(i1,i2,
     & i3,0,0)*(rsxy(i1,i2,i3,0,0)*rsxyrrs2(i1,i2,i3,m,n)+rsxy(i1,i2,
     & i3,1,0)*rsxyrss2(i1,i2,i3,m,n))+rsxy(i1,i2,i3,1,0)*(rsxy(i1,i2,
     & i3,0,0)*rsxyrss2(i1,i2,i3,m,n)+rsxy(i1,i2,i3,1,0)*rsxysss2(i1,
     & i2,i3,m,n)))
         rsxyxyy22(i1,i2,i3,m,n)=rsxyyy22(i1,i2,i3,0,0)*rsxyr2(i1,i2,
     & i3,m,n)+rsxyyy22(i1,i2,i3,1,0)*rsxys2(i1,i2,i3,m,n)+rsxyy22(i1,
     & i2,i3,0,1)*(rsxy(i1,i2,i3,0,0)*rsxyrr2(i1,i2,i3,m,n)+rsxy(i1,
     & i2,i3,1,0)*rsxyrs2(i1,i2,i3,m,n))+rsxy(i1,i2,i3,0,1)*(rsxyx22(
     & i1,i2,i3,0,1)*rsxyrr2(i1,i2,i3,m,n)+rsxyx22(i1,i2,i3,1,1)*
     & rsxyrs2(i1,i2,i3,m,n))+rsxyy22(i1,i2,i3,1,1)*(rsxy(i1,i2,i3,0,
     & 0)*rsxyrs2(i1,i2,i3,m,n)+rsxy(i1,i2,i3,1,0)*rsxyss2(i1,i2,i3,m,
     & n))+rsxy(i1,i2,i3,1,1)*(rsxyx22(i1,i2,i3,0,1)*rsxyrs2(i1,i2,i3,
     & m,n)+rsxyx22(i1,i2,i3,1,1)*rsxyss2(i1,i2,i3,m,n))+rsxy(i1,i2,
     & i3,0,1)*(rsxyx22(i1,i2,i3,0,1)*rsxyrr2(i1,i2,i3,m,n)+rsxyx22(
     & i1,i2,i3,1,1)*rsxyrs2(i1,i2,i3,m,n)+rsxy(i1,i2,i3,0,1)*(rsxy(
     & i1,i2,i3,0,0)*rsxyrrr2(i1,i2,i3,m,n)+rsxy(i1,i2,i3,1,0)*
     & rsxyrrs2(i1,i2,i3,m,n))+rsxy(i1,i2,i3,1,1)*(rsxy(i1,i2,i3,0,0)*
     & rsxyrrs2(i1,i2,i3,m,n)+rsxy(i1,i2,i3,1,0)*rsxyrss2(i1,i2,i3,m,
     & n)))+rsxy(i1,i2,i3,1,1)*(rsxyx22(i1,i2,i3,0,1)*rsxyrs2(i1,i2,
     & i3,m,n)+rsxyx22(i1,i2,i3,1,1)*rsxyss2(i1,i2,i3,m,n)+rsxy(i1,i2,
     & i3,0,1)*(rsxy(i1,i2,i3,0,0)*rsxyrrs2(i1,i2,i3,m,n)+rsxy(i1,i2,
     & i3,1,0)*rsxyrss2(i1,i2,i3,m,n))+rsxy(i1,i2,i3,1,1)*(rsxy(i1,i2,
     & i3,0,0)*rsxyrss2(i1,i2,i3,m,n)+rsxy(i1,i2,i3,1,0)*rsxysss2(i1,
     & i2,i3,m,n)))
         rsxyyyy22(i1,i2,i3,m,n)=rsxyyy22(i1,i2,i3,0,1)*rsxyr2(i1,i2,
     & i3,m,n)+rsxyyy22(i1,i2,i3,1,1)*rsxys2(i1,i2,i3,m,n)+rsxyy22(i1,
     & i2,i3,0,1)*(rsxy(i1,i2,i3,0,1)*rsxyrr2(i1,i2,i3,m,n)+rsxy(i1,
     & i2,i3,1,1)*rsxyrs2(i1,i2,i3,m,n))+rsxy(i1,i2,i3,0,1)*(rsxyy22(
     & i1,i2,i3,0,1)*rsxyrr2(i1,i2,i3,m,n)+rsxyy22(i1,i2,i3,1,1)*
     & rsxyrs2(i1,i2,i3,m,n))+rsxyy22(i1,i2,i3,1,1)*(rsxy(i1,i2,i3,0,
     & 1)*rsxyrs2(i1,i2,i3,m,n)+rsxy(i1,i2,i3,1,1)*rsxyss2(i1,i2,i3,m,
     & n))+rsxy(i1,i2,i3,1,1)*(rsxyy22(i1,i2,i3,0,1)*rsxyrs2(i1,i2,i3,
     & m,n)+rsxyy22(i1,i2,i3,1,1)*rsxyss2(i1,i2,i3,m,n))+rsxy(i1,i2,
     & i3,0,1)*(rsxyy22(i1,i2,i3,0,1)*rsxyrr2(i1,i2,i3,m,n)+rsxyy22(
     & i1,i2,i3,1,1)*rsxyrs2(i1,i2,i3,m,n)+rsxy(i1,i2,i3,0,1)*(rsxy(
     & i1,i2,i3,0,1)*rsxyrrr2(i1,i2,i3,m,n)+rsxy(i1,i2,i3,1,1)*
     & rsxyrrs2(i1,i2,i3,m,n))+rsxy(i1,i2,i3,1,1)*(rsxy(i1,i2,i3,0,1)*
     & rsxyrrs2(i1,i2,i3,m,n)+rsxy(i1,i2,i3,1,1)*rsxyrss2(i1,i2,i3,m,
     & n)))+rsxy(i1,i2,i3,1,1)*(rsxyy22(i1,i2,i3,0,1)*rsxyrs2(i1,i2,
     & i3,m,n)+rsxyy22(i1,i2,i3,1,1)*rsxyss2(i1,i2,i3,m,n)+rsxy(i1,i2,
     & i3,0,1)*(rsxy(i1,i2,i3,0,1)*rsxyrrs2(i1,i2,i3,m,n)+rsxy(i1,i2,
     & i3,1,1)*rsxyrss2(i1,i2,i3,m,n))+rsxy(i1,i2,i3,1,1)*(rsxy(i1,i2,
     & i3,0,1)*rsxyrss2(i1,i2,i3,m,n)+rsxy(i1,i2,i3,1,1)*rsxysss2(i1,
     & i2,i3,m,n)))
         uxxx22(i1,i2,i3,kd)=rsxyxx22(i1,i2,i3,0,0)*ur2(i1,i2,i3,kd)+
     & rsxyxx22(i1,i2,i3,1,0)*us2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,0,0)*(
     & rsxy(i1,i2,i3,0,0)*urr2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*urs2(
     & i1,i2,i3,kd))+rsxy(i1,i2,i3,0,0)*(rsxyx22(i1,i2,i3,0,0)*urr2(
     & i1,i2,i3,kd)+rsxyx22(i1,i2,i3,1,0)*urs2(i1,i2,i3,kd))+rsxyx22(
     & i1,i2,i3,1,0)*(rsxy(i1,i2,i3,0,0)*urs2(i1,i2,i3,kd)+rsxy(i1,i2,
     & i3,1,0)*uss2(i1,i2,i3,kd))+rsxy(i1,i2,i3,1,0)*(rsxyx22(i1,i2,
     & i3,0,0)*urs2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,1,0)*uss2(i1,i2,i3,
     & kd))+rsxy(i1,i2,i3,0,0)*(rsxyx22(i1,i2,i3,0,0)*urr2(i1,i2,i3,
     & kd)+rsxyx22(i1,i2,i3,1,0)*urs2(i1,i2,i3,kd)+rsxy(i1,i2,i3,0,0)*
     & (rsxy(i1,i2,i3,0,0)*urrr2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*
     & urrs2(i1,i2,i3,kd))+rsxy(i1,i2,i3,1,0)*(rsxy(i1,i2,i3,0,0)*
     & urrs2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*urss2(i1,i2,i3,kd)))+
     & rsxy(i1,i2,i3,1,0)*(rsxyx22(i1,i2,i3,0,0)*urs2(i1,i2,i3,kd)+
     & rsxyx22(i1,i2,i3,1,0)*uss2(i1,i2,i3,kd)+rsxy(i1,i2,i3,0,0)*(
     & rsxy(i1,i2,i3,0,0)*urrs2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*urss2(
     & i1,i2,i3,kd))+rsxy(i1,i2,i3,1,0)*(rsxy(i1,i2,i3,0,0)*urss2(i1,
     & i2,i3,kd)+rsxy(i1,i2,i3,1,0)*usss2(i1,i2,i3,kd)))
         uxxy22(i1,i2,i3,kd)=rsxyxy22(i1,i2,i3,0,0)*ur2(i1,i2,i3,kd)+
     & rsxyxy22(i1,i2,i3,1,0)*us2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,0,1)*(
     & rsxy(i1,i2,i3,0,0)*urr2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*urs2(
     & i1,i2,i3,kd))+rsxy(i1,i2,i3,0,0)*(rsxyx22(i1,i2,i3,0,1)*urr2(
     & i1,i2,i3,kd)+rsxyx22(i1,i2,i3,1,1)*urs2(i1,i2,i3,kd))+rsxyx22(
     & i1,i2,i3,1,1)*(rsxy(i1,i2,i3,0,0)*urs2(i1,i2,i3,kd)+rsxy(i1,i2,
     & i3,1,0)*uss2(i1,i2,i3,kd))+rsxy(i1,i2,i3,1,0)*(rsxyx22(i1,i2,
     & i3,0,1)*urs2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,1,1)*uss2(i1,i2,i3,
     & kd))+rsxy(i1,i2,i3,0,1)*(rsxyx22(i1,i2,i3,0,0)*urr2(i1,i2,i3,
     & kd)+rsxyx22(i1,i2,i3,1,0)*urs2(i1,i2,i3,kd)+rsxy(i1,i2,i3,0,0)*
     & (rsxy(i1,i2,i3,0,0)*urrr2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*
     & urrs2(i1,i2,i3,kd))+rsxy(i1,i2,i3,1,0)*(rsxy(i1,i2,i3,0,0)*
     & urrs2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*urss2(i1,i2,i3,kd)))+
     & rsxy(i1,i2,i3,1,1)*(rsxyx22(i1,i2,i3,0,0)*urs2(i1,i2,i3,kd)+
     & rsxyx22(i1,i2,i3,1,0)*uss2(i1,i2,i3,kd)+rsxy(i1,i2,i3,0,0)*(
     & rsxy(i1,i2,i3,0,0)*urrs2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*urss2(
     & i1,i2,i3,kd))+rsxy(i1,i2,i3,1,0)*(rsxy(i1,i2,i3,0,0)*urss2(i1,
     & i2,i3,kd)+rsxy(i1,i2,i3,1,0)*usss2(i1,i2,i3,kd)))
         uxyy22(i1,i2,i3,kd)=rsxyyy22(i1,i2,i3,0,0)*ur2(i1,i2,i3,kd)+
     & rsxyyy22(i1,i2,i3,1,0)*us2(i1,i2,i3,kd)+rsxyy22(i1,i2,i3,0,1)*(
     & rsxy(i1,i2,i3,0,0)*urr2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*urs2(
     & i1,i2,i3,kd))+rsxy(i1,i2,i3,0,1)*(rsxyx22(i1,i2,i3,0,1)*urr2(
     & i1,i2,i3,kd)+rsxyx22(i1,i2,i3,1,1)*urs2(i1,i2,i3,kd))+rsxyy22(
     & i1,i2,i3,1,1)*(rsxy(i1,i2,i3,0,0)*urs2(i1,i2,i3,kd)+rsxy(i1,i2,
     & i3,1,0)*uss2(i1,i2,i3,kd))+rsxy(i1,i2,i3,1,1)*(rsxyx22(i1,i2,
     & i3,0,1)*urs2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,1,1)*uss2(i1,i2,i3,
     & kd))+rsxy(i1,i2,i3,0,1)*(rsxyx22(i1,i2,i3,0,1)*urr2(i1,i2,i3,
     & kd)+rsxyx22(i1,i2,i3,1,1)*urs2(i1,i2,i3,kd)+rsxy(i1,i2,i3,0,1)*
     & (rsxy(i1,i2,i3,0,0)*urrr2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*
     & urrs2(i1,i2,i3,kd))+rsxy(i1,i2,i3,1,1)*(rsxy(i1,i2,i3,0,0)*
     & urrs2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*urss2(i1,i2,i3,kd)))+
     & rsxy(i1,i2,i3,1,1)*(rsxyx22(i1,i2,i3,0,1)*urs2(i1,i2,i3,kd)+
     & rsxyx22(i1,i2,i3,1,1)*uss2(i1,i2,i3,kd)+rsxy(i1,i2,i3,0,1)*(
     & rsxy(i1,i2,i3,0,0)*urrs2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*urss2(
     & i1,i2,i3,kd))+rsxy(i1,i2,i3,1,1)*(rsxy(i1,i2,i3,0,0)*urss2(i1,
     & i2,i3,kd)+rsxy(i1,i2,i3,1,0)*usss2(i1,i2,i3,kd)))
         uyyy22(i1,i2,i3,kd)=rsxyyy22(i1,i2,i3,0,1)*ur2(i1,i2,i3,kd)+
     & rsxyyy22(i1,i2,i3,1,1)*us2(i1,i2,i3,kd)+rsxyy22(i1,i2,i3,0,1)*(
     & rsxy(i1,i2,i3,0,1)*urr2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,1)*urs2(
     & i1,i2,i3,kd))+rsxy(i1,i2,i3,0,1)*(rsxyy22(i1,i2,i3,0,1)*urr2(
     & i1,i2,i3,kd)+rsxyy22(i1,i2,i3,1,1)*urs2(i1,i2,i3,kd))+rsxyy22(
     & i1,i2,i3,1,1)*(rsxy(i1,i2,i3,0,1)*urs2(i1,i2,i3,kd)+rsxy(i1,i2,
     & i3,1,1)*uss2(i1,i2,i3,kd))+rsxy(i1,i2,i3,1,1)*(rsxyy22(i1,i2,
     & i3,0,1)*urs2(i1,i2,i3,kd)+rsxyy22(i1,i2,i3,1,1)*uss2(i1,i2,i3,
     & kd))+rsxy(i1,i2,i3,0,1)*(rsxyy22(i1,i2,i3,0,1)*urr2(i1,i2,i3,
     & kd)+rsxyy22(i1,i2,i3,1,1)*urs2(i1,i2,i3,kd)+rsxy(i1,i2,i3,0,1)*
     & (rsxy(i1,i2,i3,0,1)*urrr2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,1)*
     & urrs2(i1,i2,i3,kd))+rsxy(i1,i2,i3,1,1)*(rsxy(i1,i2,i3,0,1)*
     & urrs2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,1)*urss2(i1,i2,i3,kd)))+
     & rsxy(i1,i2,i3,1,1)*(rsxyy22(i1,i2,i3,0,1)*urs2(i1,i2,i3,kd)+
     & rsxyy22(i1,i2,i3,1,1)*uss2(i1,i2,i3,kd)+rsxy(i1,i2,i3,0,1)*(
     & rsxy(i1,i2,i3,0,1)*urrs2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,1)*urss2(
     & i1,i2,i3,kd))+rsxy(i1,i2,i3,1,1)*(rsxy(i1,i2,i3,0,1)*urss2(i1,
     & i2,i3,kd)+rsxy(i1,i2,i3,1,1)*usss2(i1,i2,i3,kd)))
         uxxxx22(i1,i2,i3,kd)=rsxyxxx22(i1,i2,i3,0,0)*ur2(i1,i2,i3,kd)+
     & rsxyxxx22(i1,i2,i3,1,0)*us2(i1,i2,i3,kd)+rsxyxx22(i1,i2,i3,0,0)
     & *(rsxy(i1,i2,i3,0,0)*urr2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*urs2(
     & i1,i2,i3,kd))+2*rsxyx22(i1,i2,i3,0,0)*(rsxyx22(i1,i2,i3,0,0)*
     & urr2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,1,0)*urs2(i1,i2,i3,kd))+
     & rsxy(i1,i2,i3,0,0)*(rsxyxx22(i1,i2,i3,0,0)*urr2(i1,i2,i3,kd)+
     & rsxyxx22(i1,i2,i3,1,0)*urs2(i1,i2,i3,kd))+rsxyxx22(i1,i2,i3,1,
     & 0)*(rsxy(i1,i2,i3,0,0)*urs2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*
     & uss2(i1,i2,i3,kd))+2*rsxyx22(i1,i2,i3,1,0)*(rsxyx22(i1,i2,i3,0,
     & 0)*urs2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,1,0)*uss2(i1,i2,i3,kd))+
     & rsxy(i1,i2,i3,1,0)*(rsxyxx22(i1,i2,i3,0,0)*urs2(i1,i2,i3,kd)+
     & rsxyxx22(i1,i2,i3,1,0)*uss2(i1,i2,i3,kd))+rsxyx22(i1,i2,i3,0,0)
     & *(rsxyx22(i1,i2,i3,0,0)*urr2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,1,0)
     & *urs2(i1,i2,i3,kd)+rsxy(i1,i2,i3,0,0)*(rsxy(i1,i2,i3,0,0)*
     & urrr2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*urrs2(i1,i2,i3,kd))+rsxy(
     & i1,i2,i3,1,0)*(rsxy(i1,i2,i3,0,0)*urrs2(i1,i2,i3,kd)+rsxy(i1,
     & i2,i3,1,0)*urss2(i1,i2,i3,kd)))+rsxy(i1,i2,i3,0,0)*(rsxyxx22(
     & i1,i2,i3,0,0)*urr2(i1,i2,i3,kd)+rsxyxx22(i1,i2,i3,1,0)*urs2(i1,
     & i2,i3,kd)+rsxyx22(i1,i2,i3,0,0)*(rsxy(i1,i2,i3,0,0)*urrr2(i1,
     & i2,i3,kd)+rsxy(i1,i2,i3,1,0)*urrs2(i1,i2,i3,kd))+rsxy(i1,i2,i3,
     & 0,0)*(rsxyx22(i1,i2,i3,0,0)*urrr2(i1,i2,i3,kd)+rsxyx22(i1,i2,
     & i3,1,0)*urrs2(i1,i2,i3,kd))+rsxyx22(i1,i2,i3,1,0)*(rsxy(i1,i2,
     & i3,0,0)*urrs2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*urss2(i1,i2,i3,
     & kd))+rsxy(i1,i2,i3,1,0)*(rsxyx22(i1,i2,i3,0,0)*urrs2(i1,i2,i3,
     & kd)+rsxyx22(i1,i2,i3,1,0)*urss2(i1,i2,i3,kd)))+rsxyx22(i1,i2,
     & i3,1,0)*(rsxyx22(i1,i2,i3,0,0)*urs2(i1,i2,i3,kd)+rsxyx22(i1,i2,
     & i3,1,0)*uss2(i1,i2,i3,kd)+rsxy(i1,i2,i3,0,0)*(rsxy(i1,i2,i3,0,
     & 0)*urrs2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*urss2(i1,i2,i3,kd))+
     & rsxy(i1,i2,i3,1,0)*(rsxy(i1,i2,i3,0,0)*urss2(i1,i2,i3,kd)+rsxy(
     & i1,i2,i3,1,0)*usss2(i1,i2,i3,kd)))+rsxy(i1,i2,i3,1,0)*(
     & rsxyxx22(i1,i2,i3,0,0)*urs2(i1,i2,i3,kd)+rsxyxx22(i1,i2,i3,1,0)
     & *uss2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,0,0)*(rsxy(i1,i2,i3,0,0)*
     & urrs2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*urss2(i1,i2,i3,kd))+rsxy(
     & i1,i2,i3,0,0)*(rsxyx22(i1,i2,i3,0,0)*urrs2(i1,i2,i3,kd)+
     & rsxyx22(i1,i2,i3,1,0)*urss2(i1,i2,i3,kd))+rsxyx22(i1,i2,i3,1,0)
     & *(rsxy(i1,i2,i3,0,0)*urss2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*
     & usss2(i1,i2,i3,kd))+rsxy(i1,i2,i3,1,0)*(rsxyx22(i1,i2,i3,0,0)*
     & urss2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,1,0)*usss2(i1,i2,i3,kd)))+
     & rsxy(i1,i2,i3,0,0)*(rsxyxx22(i1,i2,i3,0,0)*urr2(i1,i2,i3,kd)+
     & rsxyxx22(i1,i2,i3,1,0)*urs2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,0,0)*
     & (rsxy(i1,i2,i3,0,0)*urrr2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*
     & urrs2(i1,i2,i3,kd))+rsxy(i1,i2,i3,0,0)*(rsxyx22(i1,i2,i3,0,0)*
     & urrr2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,1,0)*urrs2(i1,i2,i3,kd))+
     & rsxyx22(i1,i2,i3,1,0)*(rsxy(i1,i2,i3,0,0)*urrs2(i1,i2,i3,kd)+
     & rsxy(i1,i2,i3,1,0)*urss2(i1,i2,i3,kd))+rsxy(i1,i2,i3,1,0)*(
     & rsxyx22(i1,i2,i3,0,0)*urrs2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,1,0)*
     & urss2(i1,i2,i3,kd))+rsxy(i1,i2,i3,0,0)*(rsxyx22(i1,i2,i3,0,0)*
     & urrr2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,1,0)*urrs2(i1,i2,i3,kd)+
     & rsxy(i1,i2,i3,0,0)*(rsxy(i1,i2,i3,0,0)*urrrr2(i1,i2,i3,kd)+
     & rsxy(i1,i2,i3,1,0)*urrrs2(i1,i2,i3,kd))+rsxy(i1,i2,i3,1,0)*(
     & rsxy(i1,i2,i3,0,0)*urrrs2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*
     & urrss2(i1,i2,i3,kd)))+rsxy(i1,i2,i3,1,0)*(rsxyx22(i1,i2,i3,0,0)
     & *urrs2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,1,0)*urss2(i1,i2,i3,kd)+
     & rsxy(i1,i2,i3,0,0)*(rsxy(i1,i2,i3,0,0)*urrrs2(i1,i2,i3,kd)+
     & rsxy(i1,i2,i3,1,0)*urrss2(i1,i2,i3,kd))+rsxy(i1,i2,i3,1,0)*(
     & rsxy(i1,i2,i3,0,0)*urrss2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*
     & ursss2(i1,i2,i3,kd))))+rsxy(i1,i2,i3,1,0)*(rsxyxx22(i1,i2,i3,0,
     & 0)*urs2(i1,i2,i3,kd)+rsxyxx22(i1,i2,i3,1,0)*uss2(i1,i2,i3,kd)+
     & rsxyx22(i1,i2,i3,0,0)*(rsxy(i1,i2,i3,0,0)*urrs2(i1,i2,i3,kd)+
     & rsxy(i1,i2,i3,1,0)*urss2(i1,i2,i3,kd))+rsxy(i1,i2,i3,0,0)*(
     & rsxyx22(i1,i2,i3,0,0)*urrs2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,1,0)*
     & urss2(i1,i2,i3,kd))+rsxyx22(i1,i2,i3,1,0)*(rsxy(i1,i2,i3,0,0)*
     & urss2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*usss2(i1,i2,i3,kd))+rsxy(
     & i1,i2,i3,1,0)*(rsxyx22(i1,i2,i3,0,0)*urss2(i1,i2,i3,kd)+
     & rsxyx22(i1,i2,i3,1,0)*usss2(i1,i2,i3,kd))+rsxy(i1,i2,i3,0,0)*(
     & rsxyx22(i1,i2,i3,0,0)*urrs2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,1,0)*
     & urss2(i1,i2,i3,kd)+rsxy(i1,i2,i3,0,0)*(rsxy(i1,i2,i3,0,0)*
     & urrrs2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*urrss2(i1,i2,i3,kd))+
     & rsxy(i1,i2,i3,1,0)*(rsxy(i1,i2,i3,0,0)*urrss2(i1,i2,i3,kd)+
     & rsxy(i1,i2,i3,1,0)*ursss2(i1,i2,i3,kd)))+rsxy(i1,i2,i3,1,0)*(
     & rsxyx22(i1,i2,i3,0,0)*urss2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,1,0)*
     & usss2(i1,i2,i3,kd)+rsxy(i1,i2,i3,0,0)*(rsxy(i1,i2,i3,0,0)*
     & urrss2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*ursss2(i1,i2,i3,kd))+
     & rsxy(i1,i2,i3,1,0)*(rsxy(i1,i2,i3,0,0)*ursss2(i1,i2,i3,kd)+
     & rsxy(i1,i2,i3,1,0)*ussss2(i1,i2,i3,kd))))
         uxxxy22(i1,i2,i3,kd)=rsxyxxy22(i1,i2,i3,0,0)*ur2(i1,i2,i3,kd)+
     & rsxyxxy22(i1,i2,i3,1,0)*us2(i1,i2,i3,kd)+rsxyxy22(i1,i2,i3,0,0)
     & *(rsxy(i1,i2,i3,0,0)*urr2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*urs2(
     & i1,i2,i3,kd))+rsxyx22(i1,i2,i3,0,0)*(rsxyx22(i1,i2,i3,0,1)*
     & urr2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,1,1)*urs2(i1,i2,i3,kd))+
     & rsxyx22(i1,i2,i3,0,1)*(rsxyx22(i1,i2,i3,0,0)*urr2(i1,i2,i3,kd)+
     & rsxyx22(i1,i2,i3,1,0)*urs2(i1,i2,i3,kd))+rsxy(i1,i2,i3,0,0)*(
     & rsxyxy22(i1,i2,i3,0,0)*urr2(i1,i2,i3,kd)+rsxyxy22(i1,i2,i3,1,0)
     & *urs2(i1,i2,i3,kd))+rsxyxy22(i1,i2,i3,1,0)*(rsxy(i1,i2,i3,0,0)*
     & urs2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*uss2(i1,i2,i3,kd))+
     & rsxyx22(i1,i2,i3,1,0)*(rsxyx22(i1,i2,i3,0,1)*urs2(i1,i2,i3,kd)+
     & rsxyx22(i1,i2,i3,1,1)*uss2(i1,i2,i3,kd))+rsxyx22(i1,i2,i3,1,1)*
     & (rsxyx22(i1,i2,i3,0,0)*urs2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,1,0)*
     & uss2(i1,i2,i3,kd))+rsxy(i1,i2,i3,1,0)*(rsxyxy22(i1,i2,i3,0,0)*
     & urs2(i1,i2,i3,kd)+rsxyxy22(i1,i2,i3,1,0)*uss2(i1,i2,i3,kd))+
     & rsxyx22(i1,i2,i3,0,1)*(rsxyx22(i1,i2,i3,0,0)*urr2(i1,i2,i3,kd)+
     & rsxyx22(i1,i2,i3,1,0)*urs2(i1,i2,i3,kd)+rsxy(i1,i2,i3,0,0)*(
     & rsxy(i1,i2,i3,0,0)*urrr2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*urrs2(
     & i1,i2,i3,kd))+rsxy(i1,i2,i3,1,0)*(rsxy(i1,i2,i3,0,0)*urrs2(i1,
     & i2,i3,kd)+rsxy(i1,i2,i3,1,0)*urss2(i1,i2,i3,kd)))+rsxy(i1,i2,
     & i3,0,0)*(rsxyxy22(i1,i2,i3,0,0)*urr2(i1,i2,i3,kd)+rsxyxy22(i1,
     & i2,i3,1,0)*urs2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,0,1)*(rsxy(i1,i2,
     & i3,0,0)*urrr2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*urrs2(i1,i2,i3,
     & kd))+rsxy(i1,i2,i3,0,0)*(rsxyx22(i1,i2,i3,0,1)*urrr2(i1,i2,i3,
     & kd)+rsxyx22(i1,i2,i3,1,1)*urrs2(i1,i2,i3,kd))+rsxyx22(i1,i2,i3,
     & 1,1)*(rsxy(i1,i2,i3,0,0)*urrs2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*
     & urss2(i1,i2,i3,kd))+rsxy(i1,i2,i3,1,0)*(rsxyx22(i1,i2,i3,0,1)*
     & urrs2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,1,1)*urss2(i1,i2,i3,kd)))+
     & rsxyx22(i1,i2,i3,1,1)*(rsxyx22(i1,i2,i3,0,0)*urs2(i1,i2,i3,kd)+
     & rsxyx22(i1,i2,i3,1,0)*uss2(i1,i2,i3,kd)+rsxy(i1,i2,i3,0,0)*(
     & rsxy(i1,i2,i3,0,0)*urrs2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*urss2(
     & i1,i2,i3,kd))+rsxy(i1,i2,i3,1,0)*(rsxy(i1,i2,i3,0,0)*urss2(i1,
     & i2,i3,kd)+rsxy(i1,i2,i3,1,0)*usss2(i1,i2,i3,kd)))+rsxy(i1,i2,
     & i3,1,0)*(rsxyxy22(i1,i2,i3,0,0)*urs2(i1,i2,i3,kd)+rsxyxy22(i1,
     & i2,i3,1,0)*uss2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,0,1)*(rsxy(i1,i2,
     & i3,0,0)*urrs2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*urss2(i1,i2,i3,
     & kd))+rsxy(i1,i2,i3,0,0)*(rsxyx22(i1,i2,i3,0,1)*urrs2(i1,i2,i3,
     & kd)+rsxyx22(i1,i2,i3,1,1)*urss2(i1,i2,i3,kd))+rsxyx22(i1,i2,i3,
     & 1,1)*(rsxy(i1,i2,i3,0,0)*urss2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*
     & usss2(i1,i2,i3,kd))+rsxy(i1,i2,i3,1,0)*(rsxyx22(i1,i2,i3,0,1)*
     & urss2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,1,1)*usss2(i1,i2,i3,kd)))+
     & rsxy(i1,i2,i3,0,1)*(rsxyxx22(i1,i2,i3,0,0)*urr2(i1,i2,i3,kd)+
     & rsxyxx22(i1,i2,i3,1,0)*urs2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,0,0)*
     & (rsxy(i1,i2,i3,0,0)*urrr2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*
     & urrs2(i1,i2,i3,kd))+rsxy(i1,i2,i3,0,0)*(rsxyx22(i1,i2,i3,0,0)*
     & urrr2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,1,0)*urrs2(i1,i2,i3,kd))+
     & rsxyx22(i1,i2,i3,1,0)*(rsxy(i1,i2,i3,0,0)*urrs2(i1,i2,i3,kd)+
     & rsxy(i1,i2,i3,1,0)*urss2(i1,i2,i3,kd))+rsxy(i1,i2,i3,1,0)*(
     & rsxyx22(i1,i2,i3,0,0)*urrs2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,1,0)*
     & urss2(i1,i2,i3,kd))+rsxy(i1,i2,i3,0,0)*(rsxyx22(i1,i2,i3,0,0)*
     & urrr2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,1,0)*urrs2(i1,i2,i3,kd)+
     & rsxy(i1,i2,i3,0,0)*(rsxy(i1,i2,i3,0,0)*urrrr2(i1,i2,i3,kd)+
     & rsxy(i1,i2,i3,1,0)*urrrs2(i1,i2,i3,kd))+rsxy(i1,i2,i3,1,0)*(
     & rsxy(i1,i2,i3,0,0)*urrrs2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*
     & urrss2(i1,i2,i3,kd)))+rsxy(i1,i2,i3,1,0)*(rsxyx22(i1,i2,i3,0,0)
     & *urrs2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,1,0)*urss2(i1,i2,i3,kd)+
     & rsxy(i1,i2,i3,0,0)*(rsxy(i1,i2,i3,0,0)*urrrs2(i1,i2,i3,kd)+
     & rsxy(i1,i2,i3,1,0)*urrss2(i1,i2,i3,kd))+rsxy(i1,i2,i3,1,0)*(
     & rsxy(i1,i2,i3,0,0)*urrss2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*
     & ursss2(i1,i2,i3,kd))))+rsxy(i1,i2,i3,1,1)*(rsxyxx22(i1,i2,i3,0,
     & 0)*urs2(i1,i2,i3,kd)+rsxyxx22(i1,i2,i3,1,0)*uss2(i1,i2,i3,kd)+
     & rsxyx22(i1,i2,i3,0,0)*(rsxy(i1,i2,i3,0,0)*urrs2(i1,i2,i3,kd)+
     & rsxy(i1,i2,i3,1,0)*urss2(i1,i2,i3,kd))+rsxy(i1,i2,i3,0,0)*(
     & rsxyx22(i1,i2,i3,0,0)*urrs2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,1,0)*
     & urss2(i1,i2,i3,kd))+rsxyx22(i1,i2,i3,1,0)*(rsxy(i1,i2,i3,0,0)*
     & urss2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*usss2(i1,i2,i3,kd))+rsxy(
     & i1,i2,i3,1,0)*(rsxyx22(i1,i2,i3,0,0)*urss2(i1,i2,i3,kd)+
     & rsxyx22(i1,i2,i3,1,0)*usss2(i1,i2,i3,kd))+rsxy(i1,i2,i3,0,0)*(
     & rsxyx22(i1,i2,i3,0,0)*urrs2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,1,0)*
     & urss2(i1,i2,i3,kd)+rsxy(i1,i2,i3,0,0)*(rsxy(i1,i2,i3,0,0)*
     & urrrs2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*urrss2(i1,i2,i3,kd))+
     & rsxy(i1,i2,i3,1,0)*(rsxy(i1,i2,i3,0,0)*urrss2(i1,i2,i3,kd)+
     & rsxy(i1,i2,i3,1,0)*ursss2(i1,i2,i3,kd)))+rsxy(i1,i2,i3,1,0)*(
     & rsxyx22(i1,i2,i3,0,0)*urss2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,1,0)*
     & usss2(i1,i2,i3,kd)+rsxy(i1,i2,i3,0,0)*(rsxy(i1,i2,i3,0,0)*
     & urrss2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*ursss2(i1,i2,i3,kd))+
     & rsxy(i1,i2,i3,1,0)*(rsxy(i1,i2,i3,0,0)*ursss2(i1,i2,i3,kd)+
     & rsxy(i1,i2,i3,1,0)*ussss2(i1,i2,i3,kd))))
         uxxyy22(i1,i2,i3,kd)=rsxyxyy22(i1,i2,i3,0,0)*ur2(i1,i2,i3,kd)+
     & rsxyxyy22(i1,i2,i3,1,0)*us2(i1,i2,i3,kd)+rsxyyy22(i1,i2,i3,0,0)
     & *(rsxy(i1,i2,i3,0,0)*urr2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*urs2(
     & i1,i2,i3,kd))+2*rsxyx22(i1,i2,i3,0,1)*(rsxyx22(i1,i2,i3,0,1)*
     & urr2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,1,1)*urs2(i1,i2,i3,kd))+
     & rsxy(i1,i2,i3,0,0)*(rsxyyy22(i1,i2,i3,0,0)*urr2(i1,i2,i3,kd)+
     & rsxyyy22(i1,i2,i3,1,0)*urs2(i1,i2,i3,kd))+rsxyyy22(i1,i2,i3,1,
     & 0)*(rsxy(i1,i2,i3,0,0)*urs2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*
     & uss2(i1,i2,i3,kd))+2*rsxyx22(i1,i2,i3,1,1)*(rsxyx22(i1,i2,i3,0,
     & 1)*urs2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,1,1)*uss2(i1,i2,i3,kd))+
     & rsxy(i1,i2,i3,1,0)*(rsxyyy22(i1,i2,i3,0,0)*urs2(i1,i2,i3,kd)+
     & rsxyyy22(i1,i2,i3,1,0)*uss2(i1,i2,i3,kd))+rsxyy22(i1,i2,i3,0,1)
     & *(rsxyx22(i1,i2,i3,0,0)*urr2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,1,0)
     & *urs2(i1,i2,i3,kd)+rsxy(i1,i2,i3,0,0)*(rsxy(i1,i2,i3,0,0)*
     & urrr2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*urrs2(i1,i2,i3,kd))+rsxy(
     & i1,i2,i3,1,0)*(rsxy(i1,i2,i3,0,0)*urrs2(i1,i2,i3,kd)+rsxy(i1,
     & i2,i3,1,0)*urss2(i1,i2,i3,kd)))+rsxy(i1,i2,i3,0,1)*(rsxyxy22(
     & i1,i2,i3,0,0)*urr2(i1,i2,i3,kd)+rsxyxy22(i1,i2,i3,1,0)*urs2(i1,
     & i2,i3,kd)+rsxyx22(i1,i2,i3,0,1)*(rsxy(i1,i2,i3,0,0)*urrr2(i1,
     & i2,i3,kd)+rsxy(i1,i2,i3,1,0)*urrs2(i1,i2,i3,kd))+rsxy(i1,i2,i3,
     & 0,0)*(rsxyx22(i1,i2,i3,0,1)*urrr2(i1,i2,i3,kd)+rsxyx22(i1,i2,
     & i3,1,1)*urrs2(i1,i2,i3,kd))+rsxyx22(i1,i2,i3,1,1)*(rsxy(i1,i2,
     & i3,0,0)*urrs2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*urss2(i1,i2,i3,
     & kd))+rsxy(i1,i2,i3,1,0)*(rsxyx22(i1,i2,i3,0,1)*urrs2(i1,i2,i3,
     & kd)+rsxyx22(i1,i2,i3,1,1)*urss2(i1,i2,i3,kd)))+rsxyy22(i1,i2,
     & i3,1,1)*(rsxyx22(i1,i2,i3,0,0)*urs2(i1,i2,i3,kd)+rsxyx22(i1,i2,
     & i3,1,0)*uss2(i1,i2,i3,kd)+rsxy(i1,i2,i3,0,0)*(rsxy(i1,i2,i3,0,
     & 0)*urrs2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*urss2(i1,i2,i3,kd))+
     & rsxy(i1,i2,i3,1,0)*(rsxy(i1,i2,i3,0,0)*urss2(i1,i2,i3,kd)+rsxy(
     & i1,i2,i3,1,0)*usss2(i1,i2,i3,kd)))+rsxy(i1,i2,i3,1,1)*(
     & rsxyxy22(i1,i2,i3,0,0)*urs2(i1,i2,i3,kd)+rsxyxy22(i1,i2,i3,1,0)
     & *uss2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,0,1)*(rsxy(i1,i2,i3,0,0)*
     & urrs2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*urss2(i1,i2,i3,kd))+rsxy(
     & i1,i2,i3,0,0)*(rsxyx22(i1,i2,i3,0,1)*urrs2(i1,i2,i3,kd)+
     & rsxyx22(i1,i2,i3,1,1)*urss2(i1,i2,i3,kd))+rsxyx22(i1,i2,i3,1,1)
     & *(rsxy(i1,i2,i3,0,0)*urss2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*
     & usss2(i1,i2,i3,kd))+rsxy(i1,i2,i3,1,0)*(rsxyx22(i1,i2,i3,0,1)*
     & urss2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,1,1)*usss2(i1,i2,i3,kd)))+
     & rsxy(i1,i2,i3,0,1)*(rsxyxy22(i1,i2,i3,0,0)*urr2(i1,i2,i3,kd)+
     & rsxyxy22(i1,i2,i3,1,0)*urs2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,0,1)*
     & (rsxy(i1,i2,i3,0,0)*urrr2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*
     & urrs2(i1,i2,i3,kd))+rsxy(i1,i2,i3,0,0)*(rsxyx22(i1,i2,i3,0,1)*
     & urrr2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,1,1)*urrs2(i1,i2,i3,kd))+
     & rsxyx22(i1,i2,i3,1,1)*(rsxy(i1,i2,i3,0,0)*urrs2(i1,i2,i3,kd)+
     & rsxy(i1,i2,i3,1,0)*urss2(i1,i2,i3,kd))+rsxy(i1,i2,i3,1,0)*(
     & rsxyx22(i1,i2,i3,0,1)*urrs2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,1,1)*
     & urss2(i1,i2,i3,kd))+rsxy(i1,i2,i3,0,1)*(rsxyx22(i1,i2,i3,0,0)*
     & urrr2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,1,0)*urrs2(i1,i2,i3,kd)+
     & rsxy(i1,i2,i3,0,0)*(rsxy(i1,i2,i3,0,0)*urrrr2(i1,i2,i3,kd)+
     & rsxy(i1,i2,i3,1,0)*urrrs2(i1,i2,i3,kd))+rsxy(i1,i2,i3,1,0)*(
     & rsxy(i1,i2,i3,0,0)*urrrs2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*
     & urrss2(i1,i2,i3,kd)))+rsxy(i1,i2,i3,1,1)*(rsxyx22(i1,i2,i3,0,0)
     & *urrs2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,1,0)*urss2(i1,i2,i3,kd)+
     & rsxy(i1,i2,i3,0,0)*(rsxy(i1,i2,i3,0,0)*urrrs2(i1,i2,i3,kd)+
     & rsxy(i1,i2,i3,1,0)*urrss2(i1,i2,i3,kd))+rsxy(i1,i2,i3,1,0)*(
     & rsxy(i1,i2,i3,0,0)*urrss2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*
     & ursss2(i1,i2,i3,kd))))+rsxy(i1,i2,i3,1,1)*(rsxyxy22(i1,i2,i3,0,
     & 0)*urs2(i1,i2,i3,kd)+rsxyxy22(i1,i2,i3,1,0)*uss2(i1,i2,i3,kd)+
     & rsxyx22(i1,i2,i3,0,1)*(rsxy(i1,i2,i3,0,0)*urrs2(i1,i2,i3,kd)+
     & rsxy(i1,i2,i3,1,0)*urss2(i1,i2,i3,kd))+rsxy(i1,i2,i3,0,0)*(
     & rsxyx22(i1,i2,i3,0,1)*urrs2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,1,1)*
     & urss2(i1,i2,i3,kd))+rsxyx22(i1,i2,i3,1,1)*(rsxy(i1,i2,i3,0,0)*
     & urss2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*usss2(i1,i2,i3,kd))+rsxy(
     & i1,i2,i3,1,0)*(rsxyx22(i1,i2,i3,0,1)*urss2(i1,i2,i3,kd)+
     & rsxyx22(i1,i2,i3,1,1)*usss2(i1,i2,i3,kd))+rsxy(i1,i2,i3,0,1)*(
     & rsxyx22(i1,i2,i3,0,0)*urrs2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,1,0)*
     & urss2(i1,i2,i3,kd)+rsxy(i1,i2,i3,0,0)*(rsxy(i1,i2,i3,0,0)*
     & urrrs2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*urrss2(i1,i2,i3,kd))+
     & rsxy(i1,i2,i3,1,0)*(rsxy(i1,i2,i3,0,0)*urrss2(i1,i2,i3,kd)+
     & rsxy(i1,i2,i3,1,0)*ursss2(i1,i2,i3,kd)))+rsxy(i1,i2,i3,1,1)*(
     & rsxyx22(i1,i2,i3,0,0)*urss2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,1,0)*
     & usss2(i1,i2,i3,kd)+rsxy(i1,i2,i3,0,0)*(rsxy(i1,i2,i3,0,0)*
     & urrss2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*ursss2(i1,i2,i3,kd))+
     & rsxy(i1,i2,i3,1,0)*(rsxy(i1,i2,i3,0,0)*ursss2(i1,i2,i3,kd)+
     & rsxy(i1,i2,i3,1,0)*ussss2(i1,i2,i3,kd))))
         uxyyy22(i1,i2,i3,kd)=rsxyyyy22(i1,i2,i3,0,0)*ur2(i1,i2,i3,kd)+
     & rsxyyyy22(i1,i2,i3,1,0)*us2(i1,i2,i3,kd)+rsxyyy22(i1,i2,i3,0,1)
     & *(rsxy(i1,i2,i3,0,0)*urr2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*urs2(
     & i1,i2,i3,kd))+2*rsxyy22(i1,i2,i3,0,1)*(rsxyx22(i1,i2,i3,0,1)*
     & urr2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,1,1)*urs2(i1,i2,i3,kd))+
     & rsxy(i1,i2,i3,0,1)*(rsxyyy22(i1,i2,i3,0,0)*urr2(i1,i2,i3,kd)+
     & rsxyyy22(i1,i2,i3,1,0)*urs2(i1,i2,i3,kd))+rsxyyy22(i1,i2,i3,1,
     & 1)*(rsxy(i1,i2,i3,0,0)*urs2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*
     & uss2(i1,i2,i3,kd))+2*rsxyy22(i1,i2,i3,1,1)*(rsxyx22(i1,i2,i3,0,
     & 1)*urs2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,1,1)*uss2(i1,i2,i3,kd))+
     & rsxy(i1,i2,i3,1,1)*(rsxyyy22(i1,i2,i3,0,0)*urs2(i1,i2,i3,kd)+
     & rsxyyy22(i1,i2,i3,1,0)*uss2(i1,i2,i3,kd))+rsxyy22(i1,i2,i3,0,1)
     & *(rsxyx22(i1,i2,i3,0,1)*urr2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,1,1)
     & *urs2(i1,i2,i3,kd)+rsxy(i1,i2,i3,0,1)*(rsxy(i1,i2,i3,0,0)*
     & urrr2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*urrs2(i1,i2,i3,kd))+rsxy(
     & i1,i2,i3,1,1)*(rsxy(i1,i2,i3,0,0)*urrs2(i1,i2,i3,kd)+rsxy(i1,
     & i2,i3,1,0)*urss2(i1,i2,i3,kd)))+rsxy(i1,i2,i3,0,1)*(rsxyyy22(
     & i1,i2,i3,0,0)*urr2(i1,i2,i3,kd)+rsxyyy22(i1,i2,i3,1,0)*urs2(i1,
     & i2,i3,kd)+rsxyy22(i1,i2,i3,0,1)*(rsxy(i1,i2,i3,0,0)*urrr2(i1,
     & i2,i3,kd)+rsxy(i1,i2,i3,1,0)*urrs2(i1,i2,i3,kd))+rsxy(i1,i2,i3,
     & 0,1)*(rsxyx22(i1,i2,i3,0,1)*urrr2(i1,i2,i3,kd)+rsxyx22(i1,i2,
     & i3,1,1)*urrs2(i1,i2,i3,kd))+rsxyy22(i1,i2,i3,1,1)*(rsxy(i1,i2,
     & i3,0,0)*urrs2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*urss2(i1,i2,i3,
     & kd))+rsxy(i1,i2,i3,1,1)*(rsxyx22(i1,i2,i3,0,1)*urrs2(i1,i2,i3,
     & kd)+rsxyx22(i1,i2,i3,1,1)*urss2(i1,i2,i3,kd)))+rsxyy22(i1,i2,
     & i3,1,1)*(rsxyx22(i1,i2,i3,0,1)*urs2(i1,i2,i3,kd)+rsxyx22(i1,i2,
     & i3,1,1)*uss2(i1,i2,i3,kd)+rsxy(i1,i2,i3,0,1)*(rsxy(i1,i2,i3,0,
     & 0)*urrs2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*urss2(i1,i2,i3,kd))+
     & rsxy(i1,i2,i3,1,1)*(rsxy(i1,i2,i3,0,0)*urss2(i1,i2,i3,kd)+rsxy(
     & i1,i2,i3,1,0)*usss2(i1,i2,i3,kd)))+rsxy(i1,i2,i3,1,1)*(
     & rsxyyy22(i1,i2,i3,0,0)*urs2(i1,i2,i3,kd)+rsxyyy22(i1,i2,i3,1,0)
     & *uss2(i1,i2,i3,kd)+rsxyy22(i1,i2,i3,0,1)*(rsxy(i1,i2,i3,0,0)*
     & urrs2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*urss2(i1,i2,i3,kd))+rsxy(
     & i1,i2,i3,0,1)*(rsxyx22(i1,i2,i3,0,1)*urrs2(i1,i2,i3,kd)+
     & rsxyx22(i1,i2,i3,1,1)*urss2(i1,i2,i3,kd))+rsxyy22(i1,i2,i3,1,1)
     & *(rsxy(i1,i2,i3,0,0)*urss2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*
     & usss2(i1,i2,i3,kd))+rsxy(i1,i2,i3,1,1)*(rsxyx22(i1,i2,i3,0,1)*
     & urss2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,1,1)*usss2(i1,i2,i3,kd)))+
     & rsxy(i1,i2,i3,0,1)*(rsxyyy22(i1,i2,i3,0,0)*urr2(i1,i2,i3,kd)+
     & rsxyyy22(i1,i2,i3,1,0)*urs2(i1,i2,i3,kd)+rsxyy22(i1,i2,i3,0,1)*
     & (rsxy(i1,i2,i3,0,0)*urrr2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*
     & urrs2(i1,i2,i3,kd))+rsxy(i1,i2,i3,0,1)*(rsxyx22(i1,i2,i3,0,1)*
     & urrr2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,1,1)*urrs2(i1,i2,i3,kd))+
     & rsxyy22(i1,i2,i3,1,1)*(rsxy(i1,i2,i3,0,0)*urrs2(i1,i2,i3,kd)+
     & rsxy(i1,i2,i3,1,0)*urss2(i1,i2,i3,kd))+rsxy(i1,i2,i3,1,1)*(
     & rsxyx22(i1,i2,i3,0,1)*urrs2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,1,1)*
     & urss2(i1,i2,i3,kd))+rsxy(i1,i2,i3,0,1)*(rsxyx22(i1,i2,i3,0,1)*
     & urrr2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,1,1)*urrs2(i1,i2,i3,kd)+
     & rsxy(i1,i2,i3,0,1)*(rsxy(i1,i2,i3,0,0)*urrrr2(i1,i2,i3,kd)+
     & rsxy(i1,i2,i3,1,0)*urrrs2(i1,i2,i3,kd))+rsxy(i1,i2,i3,1,1)*(
     & rsxy(i1,i2,i3,0,0)*urrrs2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*
     & urrss2(i1,i2,i3,kd)))+rsxy(i1,i2,i3,1,1)*(rsxyx22(i1,i2,i3,0,1)
     & *urrs2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,1,1)*urss2(i1,i2,i3,kd)+
     & rsxy(i1,i2,i3,0,1)*(rsxy(i1,i2,i3,0,0)*urrrs2(i1,i2,i3,kd)+
     & rsxy(i1,i2,i3,1,0)*urrss2(i1,i2,i3,kd))+rsxy(i1,i2,i3,1,1)*(
     & rsxy(i1,i2,i3,0,0)*urrss2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*
     & ursss2(i1,i2,i3,kd))))+rsxy(i1,i2,i3,1,1)*(rsxyyy22(i1,i2,i3,0,
     & 0)*urs2(i1,i2,i3,kd)+rsxyyy22(i1,i2,i3,1,0)*uss2(i1,i2,i3,kd)+
     & rsxyy22(i1,i2,i3,0,1)*(rsxy(i1,i2,i3,0,0)*urrs2(i1,i2,i3,kd)+
     & rsxy(i1,i2,i3,1,0)*urss2(i1,i2,i3,kd))+rsxy(i1,i2,i3,0,1)*(
     & rsxyx22(i1,i2,i3,0,1)*urrs2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,1,1)*
     & urss2(i1,i2,i3,kd))+rsxyy22(i1,i2,i3,1,1)*(rsxy(i1,i2,i3,0,0)*
     & urss2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*usss2(i1,i2,i3,kd))+rsxy(
     & i1,i2,i3,1,1)*(rsxyx22(i1,i2,i3,0,1)*urss2(i1,i2,i3,kd)+
     & rsxyx22(i1,i2,i3,1,1)*usss2(i1,i2,i3,kd))+rsxy(i1,i2,i3,0,1)*(
     & rsxyx22(i1,i2,i3,0,1)*urrs2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,1,1)*
     & urss2(i1,i2,i3,kd)+rsxy(i1,i2,i3,0,1)*(rsxy(i1,i2,i3,0,0)*
     & urrrs2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*urrss2(i1,i2,i3,kd))+
     & rsxy(i1,i2,i3,1,1)*(rsxy(i1,i2,i3,0,0)*urrss2(i1,i2,i3,kd)+
     & rsxy(i1,i2,i3,1,0)*ursss2(i1,i2,i3,kd)))+rsxy(i1,i2,i3,1,1)*(
     & rsxyx22(i1,i2,i3,0,1)*urss2(i1,i2,i3,kd)+rsxyx22(i1,i2,i3,1,1)*
     & usss2(i1,i2,i3,kd)+rsxy(i1,i2,i3,0,1)*(rsxy(i1,i2,i3,0,0)*
     & urrss2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*ursss2(i1,i2,i3,kd))+
     & rsxy(i1,i2,i3,1,1)*(rsxy(i1,i2,i3,0,0)*ursss2(i1,i2,i3,kd)+
     & rsxy(i1,i2,i3,1,0)*ussss2(i1,i2,i3,kd))))
         uyyyy22(i1,i2,i3,kd)=rsxyyyy22(i1,i2,i3,0,1)*ur2(i1,i2,i3,kd)+
     & rsxyyyy22(i1,i2,i3,1,1)*us2(i1,i2,i3,kd)+rsxyyy22(i1,i2,i3,0,1)
     & *(rsxy(i1,i2,i3,0,1)*urr2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,1)*urs2(
     & i1,i2,i3,kd))+2*rsxyy22(i1,i2,i3,0,1)*(rsxyy22(i1,i2,i3,0,1)*
     & urr2(i1,i2,i3,kd)+rsxyy22(i1,i2,i3,1,1)*urs2(i1,i2,i3,kd))+
     & rsxy(i1,i2,i3,0,1)*(rsxyyy22(i1,i2,i3,0,1)*urr2(i1,i2,i3,kd)+
     & rsxyyy22(i1,i2,i3,1,1)*urs2(i1,i2,i3,kd))+rsxyyy22(i1,i2,i3,1,
     & 1)*(rsxy(i1,i2,i3,0,1)*urs2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,1)*
     & uss2(i1,i2,i3,kd))+2*rsxyy22(i1,i2,i3,1,1)*(rsxyy22(i1,i2,i3,0,
     & 1)*urs2(i1,i2,i3,kd)+rsxyy22(i1,i2,i3,1,1)*uss2(i1,i2,i3,kd))+
     & rsxy(i1,i2,i3,1,1)*(rsxyyy22(i1,i2,i3,0,1)*urs2(i1,i2,i3,kd)+
     & rsxyyy22(i1,i2,i3,1,1)*uss2(i1,i2,i3,kd))+rsxyy22(i1,i2,i3,0,1)
     & *(rsxyy22(i1,i2,i3,0,1)*urr2(i1,i2,i3,kd)+rsxyy22(i1,i2,i3,1,1)
     & *urs2(i1,i2,i3,kd)+rsxy(i1,i2,i3,0,1)*(rsxy(i1,i2,i3,0,1)*
     & urrr2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,1)*urrs2(i1,i2,i3,kd))+rsxy(
     & i1,i2,i3,1,1)*(rsxy(i1,i2,i3,0,1)*urrs2(i1,i2,i3,kd)+rsxy(i1,
     & i2,i3,1,1)*urss2(i1,i2,i3,kd)))+rsxy(i1,i2,i3,0,1)*(rsxyyy22(
     & i1,i2,i3,0,1)*urr2(i1,i2,i3,kd)+rsxyyy22(i1,i2,i3,1,1)*urs2(i1,
     & i2,i3,kd)+rsxyy22(i1,i2,i3,0,1)*(rsxy(i1,i2,i3,0,1)*urrr2(i1,
     & i2,i3,kd)+rsxy(i1,i2,i3,1,1)*urrs2(i1,i2,i3,kd))+rsxy(i1,i2,i3,
     & 0,1)*(rsxyy22(i1,i2,i3,0,1)*urrr2(i1,i2,i3,kd)+rsxyy22(i1,i2,
     & i3,1,1)*urrs2(i1,i2,i3,kd))+rsxyy22(i1,i2,i3,1,1)*(rsxy(i1,i2,
     & i3,0,1)*urrs2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,1)*urss2(i1,i2,i3,
     & kd))+rsxy(i1,i2,i3,1,1)*(rsxyy22(i1,i2,i3,0,1)*urrs2(i1,i2,i3,
     & kd)+rsxyy22(i1,i2,i3,1,1)*urss2(i1,i2,i3,kd)))+rsxyy22(i1,i2,
     & i3,1,1)*(rsxyy22(i1,i2,i3,0,1)*urs2(i1,i2,i3,kd)+rsxyy22(i1,i2,
     & i3,1,1)*uss2(i1,i2,i3,kd)+rsxy(i1,i2,i3,0,1)*(rsxy(i1,i2,i3,0,
     & 1)*urrs2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,1)*urss2(i1,i2,i3,kd))+
     & rsxy(i1,i2,i3,1,1)*(rsxy(i1,i2,i3,0,1)*urss2(i1,i2,i3,kd)+rsxy(
     & i1,i2,i3,1,1)*usss2(i1,i2,i3,kd)))+rsxy(i1,i2,i3,1,1)*(
     & rsxyyy22(i1,i2,i3,0,1)*urs2(i1,i2,i3,kd)+rsxyyy22(i1,i2,i3,1,1)
     & *uss2(i1,i2,i3,kd)+rsxyy22(i1,i2,i3,0,1)*(rsxy(i1,i2,i3,0,1)*
     & urrs2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,1)*urss2(i1,i2,i3,kd))+rsxy(
     & i1,i2,i3,0,1)*(rsxyy22(i1,i2,i3,0,1)*urrs2(i1,i2,i3,kd)+
     & rsxyy22(i1,i2,i3,1,1)*urss2(i1,i2,i3,kd))+rsxyy22(i1,i2,i3,1,1)
     & *(rsxy(i1,i2,i3,0,1)*urss2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,1)*
     & usss2(i1,i2,i3,kd))+rsxy(i1,i2,i3,1,1)*(rsxyy22(i1,i2,i3,0,1)*
     & urss2(i1,i2,i3,kd)+rsxyy22(i1,i2,i3,1,1)*usss2(i1,i2,i3,kd)))+
     & rsxy(i1,i2,i3,0,1)*(rsxyyy22(i1,i2,i3,0,1)*urr2(i1,i2,i3,kd)+
     & rsxyyy22(i1,i2,i3,1,1)*urs2(i1,i2,i3,kd)+rsxyy22(i1,i2,i3,0,1)*
     & (rsxy(i1,i2,i3,0,1)*urrr2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,1)*
     & urrs2(i1,i2,i3,kd))+rsxy(i1,i2,i3,0,1)*(rsxyy22(i1,i2,i3,0,1)*
     & urrr2(i1,i2,i3,kd)+rsxyy22(i1,i2,i3,1,1)*urrs2(i1,i2,i3,kd))+
     & rsxyy22(i1,i2,i3,1,1)*(rsxy(i1,i2,i3,0,1)*urrs2(i1,i2,i3,kd)+
     & rsxy(i1,i2,i3,1,1)*urss2(i1,i2,i3,kd))+rsxy(i1,i2,i3,1,1)*(
     & rsxyy22(i1,i2,i3,0,1)*urrs2(i1,i2,i3,kd)+rsxyy22(i1,i2,i3,1,1)*
     & urss2(i1,i2,i3,kd))+rsxy(i1,i2,i3,0,1)*(rsxyy22(i1,i2,i3,0,1)*
     & urrr2(i1,i2,i3,kd)+rsxyy22(i1,i2,i3,1,1)*urrs2(i1,i2,i3,kd)+
     & rsxy(i1,i2,i3,0,1)*(rsxy(i1,i2,i3,0,1)*urrrr2(i1,i2,i3,kd)+
     & rsxy(i1,i2,i3,1,1)*urrrs2(i1,i2,i3,kd))+rsxy(i1,i2,i3,1,1)*(
     & rsxy(i1,i2,i3,0,1)*urrrs2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,1)*
     & urrss2(i1,i2,i3,kd)))+rsxy(i1,i2,i3,1,1)*(rsxyy22(i1,i2,i3,0,1)
     & *urrs2(i1,i2,i3,kd)+rsxyy22(i1,i2,i3,1,1)*urss2(i1,i2,i3,kd)+
     & rsxy(i1,i2,i3,0,1)*(rsxy(i1,i2,i3,0,1)*urrrs2(i1,i2,i3,kd)+
     & rsxy(i1,i2,i3,1,1)*urrss2(i1,i2,i3,kd))+rsxy(i1,i2,i3,1,1)*(
     & rsxy(i1,i2,i3,0,1)*urrss2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,1)*
     & ursss2(i1,i2,i3,kd))))+rsxy(i1,i2,i3,1,1)*(rsxyyy22(i1,i2,i3,0,
     & 1)*urs2(i1,i2,i3,kd)+rsxyyy22(i1,i2,i3,1,1)*uss2(i1,i2,i3,kd)+
     & rsxyy22(i1,i2,i3,0,1)*(rsxy(i1,i2,i3,0,1)*urrs2(i1,i2,i3,kd)+
     & rsxy(i1,i2,i3,1,1)*urss2(i1,i2,i3,kd))+rsxy(i1,i2,i3,0,1)*(
     & rsxyy22(i1,i2,i3,0,1)*urrs2(i1,i2,i3,kd)+rsxyy22(i1,i2,i3,1,1)*
     & urss2(i1,i2,i3,kd))+rsxyy22(i1,i2,i3,1,1)*(rsxy(i1,i2,i3,0,1)*
     & urss2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,1)*usss2(i1,i2,i3,kd))+rsxy(
     & i1,i2,i3,1,1)*(rsxyy22(i1,i2,i3,0,1)*urss2(i1,i2,i3,kd)+
     & rsxyy22(i1,i2,i3,1,1)*usss2(i1,i2,i3,kd))+rsxy(i1,i2,i3,0,1)*(
     & rsxyy22(i1,i2,i3,0,1)*urrs2(i1,i2,i3,kd)+rsxyy22(i1,i2,i3,1,1)*
     & urss2(i1,i2,i3,kd)+rsxy(i1,i2,i3,0,1)*(rsxy(i1,i2,i3,0,1)*
     & urrrs2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,1)*urrss2(i1,i2,i3,kd))+
     & rsxy(i1,i2,i3,1,1)*(rsxy(i1,i2,i3,0,1)*urrss2(i1,i2,i3,kd)+
     & rsxy(i1,i2,i3,1,1)*ursss2(i1,i2,i3,kd)))+rsxy(i1,i2,i3,1,1)*(
     & rsxyy22(i1,i2,i3,0,1)*urss2(i1,i2,i3,kd)+rsxyy22(i1,i2,i3,1,1)*
     & usss2(i1,i2,i3,kd)+rsxy(i1,i2,i3,0,1)*(rsxy(i1,i2,i3,0,1)*
     & urrss2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,1)*ursss2(i1,i2,i3,kd))+
     & rsxy(i1,i2,i3,1,1)*(rsxy(i1,i2,i3,0,1)*ursss2(i1,i2,i3,kd)+
     & rsxy(i1,i2,i3,1,1)*ussss2(i1,i2,i3,kd))))
         uLapSq22(i1,i2,i3,kd)=uxxxx22(i1,i2,i3,kd)+uyyyy22(i1,i2,i3,
     & kd)+2.*uxxyy22(i1,i2,i3,kd)
c ..... end: 3rd and 4th derivatives, 2D ....
         uxx23(i1,i2,i3,kd)=rsxy(i1,i2,i3,0,0)**2*urr2(i1,i2,i3,kd)+
     & rsxy(i1,i2,i3,1,0)**2*uss2(i1,i2,i3,kd)+rsxy(i1,i2,i3,2,0)**2*
     & utt2(i1,i2,i3,kd)+2.*rsxy(i1,i2,i3,0,0)*rsxy(i1,i2,i3,1,0)*
     & urs2(i1,i2,i3,kd)+2.*rsxy(i1,i2,i3,0,0)*rsxy(i1,i2,i3,2,0)*
     & urt2(i1,i2,i3,kd)+2.*rsxy(i1,i2,i3,1,0)*rsxy(i1,i2,i3,2,0)*
     & ust2(i1,i2,i3,kd)+rsxyx23(i1,i2,i3,0,0)*ur2(i1,i2,i3,kd)+
     & rsxyx23(i1,i2,i3,1,0)*us2(i1,i2,i3,kd)+rsxyx23(i1,i2,i3,2,0)*
     & ut2(i1,i2,i3,kd)
         uyy23(i1,i2,i3,kd)=rsxy(i1,i2,i3,0,1)**2*urr2(i1,i2,i3,kd)+
     & rsxy(i1,i2,i3,1,1)**2*uss2(i1,i2,i3,kd)+rsxy(i1,i2,i3,2,1)**2*
     & utt2(i1,i2,i3,kd)+2.*rsxy(i1,i2,i3,0,1)*rsxy(i1,i2,i3,1,1)*
     & urs2(i1,i2,i3,kd)+2.*rsxy(i1,i2,i3,0,1)*rsxy(i1,i2,i3,2,1)*
     & urt2(i1,i2,i3,kd)+2.*rsxy(i1,i2,i3,1,1)*rsxy(i1,i2,i3,2,1)*
     & ust2(i1,i2,i3,kd)+rsxyy23(i1,i2,i3,0,1)*ur2(i1,i2,i3,kd)+
     & rsxyy23(i1,i2,i3,1,1)*us2(i1,i2,i3,kd)+rsxyy23(i1,i2,i3,2,1)*
     & ut2(i1,i2,i3,kd)
         uzz23(i1,i2,i3,kd)=rsxy(i1,i2,i3,0,2)**2*urr2(i1,i2,i3,kd)+
     & rsxy(i1,i2,i3,1,2)**2*uss2(i1,i2,i3,kd)+rsxy(i1,i2,i3,2,2)**2*
     & utt2(i1,i2,i3,kd)+2.*rsxy(i1,i2,i3,0,2)*rsxy(i1,i2,i3,1,2)*
     & urs2(i1,i2,i3,kd)+2.*rsxy(i1,i2,i3,0,2)*rsxy(i1,i2,i3,2,2)*
     & urt2(i1,i2,i3,kd)+2.*rsxy(i1,i2,i3,1,2)*rsxy(i1,i2,i3,2,2)*
     & ust2(i1,i2,i3,kd)+rsxyz23(i1,i2,i3,0,2)*ur2(i1,i2,i3,kd)+
     & rsxyz23(i1,i2,i3,1,2)*us2(i1,i2,i3,kd)+rsxyz23(i1,i2,i3,2,2)*
     & ut2(i1,i2,i3,kd)
         uxy23(i1,i2,i3,kd)=rsxy(i1,i2,i3,0,0)*rsxy(i1,i2,i3,0,1)*urr2(
     & i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*rsxy(i1,i2,i3,1,1)*uss2(i1,i2,
     & i3,kd)+rsxy(i1,i2,i3,2,0)*rsxy(i1,i2,i3,2,1)*utt2(i1,i2,i3,kd)+
     & (rsxy(i1,i2,i3,0,0)*rsxy(i1,i2,i3,1,1)+rsxy(i1,i2,i3,0,1)*rsxy(
     & i1,i2,i3,1,0))*urs2(i1,i2,i3,kd)+(rsxy(i1,i2,i3,0,0)*rsxy(i1,
     & i2,i3,2,1)+rsxy(i1,i2,i3,0,1)*rsxy(i1,i2,i3,2,0))*urt2(i1,i2,
     & i3,kd)+(rsxy(i1,i2,i3,1,0)*rsxy(i1,i2,i3,2,1)+rsxy(i1,i2,i3,1,
     & 1)*rsxy(i1,i2,i3,2,0))*ust2(i1,i2,i3,kd)+rsxyx23(i1,i2,i3,0,1)*
     & ur2(i1,i2,i3,kd)+rsxyx23(i1,i2,i3,1,1)*us2(i1,i2,i3,kd)+
     & rsxyx23(i1,i2,i3,2,1)*ut2(i1,i2,i3,kd)
         uxz23(i1,i2,i3,kd)=rsxy(i1,i2,i3,0,0)*rsxy(i1,i2,i3,0,2)*urr2(
     & i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*rsxy(i1,i2,i3,1,2)*uss2(i1,i2,
     & i3,kd)+rsxy(i1,i2,i3,2,0)*rsxy(i1,i2,i3,2,2)*utt2(i1,i2,i3,kd)+
     & (rsxy(i1,i2,i3,0,0)*rsxy(i1,i2,i3,1,2)+rsxy(i1,i2,i3,0,2)*rsxy(
     & i1,i2,i3,1,0))*urs2(i1,i2,i3,kd)+(rsxy(i1,i2,i3,0,0)*rsxy(i1,
     & i2,i3,2,2)+rsxy(i1,i2,i3,0,2)*rsxy(i1,i2,i3,2,0))*urt2(i1,i2,
     & i3,kd)+(rsxy(i1,i2,i3,1,0)*rsxy(i1,i2,i3,2,2)+rsxy(i1,i2,i3,1,
     & 2)*rsxy(i1,i2,i3,2,0))*ust2(i1,i2,i3,kd)+rsxyx23(i1,i2,i3,0,2)*
     & ur2(i1,i2,i3,kd)+rsxyx23(i1,i2,i3,1,2)*us2(i1,i2,i3,kd)+
     & rsxyx23(i1,i2,i3,2,2)*ut2(i1,i2,i3,kd)
         uyz23(i1,i2,i3,kd)=rsxy(i1,i2,i3,0,1)*rsxy(i1,i2,i3,0,2)*urr2(
     & i1,i2,i3,kd)+rsxy(i1,i2,i3,1,1)*rsxy(i1,i2,i3,1,2)*uss2(i1,i2,
     & i3,kd)+rsxy(i1,i2,i3,2,1)*rsxy(i1,i2,i3,2,2)*utt2(i1,i2,i3,kd)+
     & (rsxy(i1,i2,i3,0,1)*rsxy(i1,i2,i3,1,2)+rsxy(i1,i2,i3,0,2)*rsxy(
     & i1,i2,i3,1,1))*urs2(i1,i2,i3,kd)+(rsxy(i1,i2,i3,0,1)*rsxy(i1,
     & i2,i3,2,2)+rsxy(i1,i2,i3,0,2)*rsxy(i1,i2,i3,2,1))*urt2(i1,i2,
     & i3,kd)+(rsxy(i1,i2,i3,1,1)*rsxy(i1,i2,i3,2,2)+rsxy(i1,i2,i3,1,
     & 2)*rsxy(i1,i2,i3,2,1))*ust2(i1,i2,i3,kd)+rsxyy23(i1,i2,i3,0,2)*
     & ur2(i1,i2,i3,kd)+rsxyy23(i1,i2,i3,1,2)*us2(i1,i2,i3,kd)+
     & rsxyy23(i1,i2,i3,2,2)*ut2(i1,i2,i3,kd)
         ulaplacian23(i1,i2,i3,kd)=(rsxy(i1,i2,i3,0,0)**2+rsxy(i1,i2,
     & i3,0,1)**2+rsxy(i1,i2,i3,0,2)**2)*urr2(i1,i2,i3,kd)+(rsxy(i1,
     & i2,i3,1,0)**2+rsxy(i1,i2,i3,1,1)**2+rsxy(i1,i2,i3,1,2)**2)*
     & uss2(i1,i2,i3,kd)+(rsxy(i1,i2,i3,2,0)**2+rsxy(i1,i2,i3,2,1)**2+
     & rsxy(i1,i2,i3,2,2)**2)*utt2(i1,i2,i3,kd)+2.*(rsxy(i1,i2,i3,0,0)
     & *rsxy(i1,i2,i3,1,0)+ rsxy(i1,i2,i3,0,1)*rsxy(i1,i2,i3,1,1)+
     & rsxy(i1,i2,i3,0,2)*rsxy(i1,i2,i3,1,2))*urs2(i1,i2,i3,kd)+2.*(
     & rsxy(i1,i2,i3,0,0)*rsxy(i1,i2,i3,2,0)+ rsxy(i1,i2,i3,0,1)*rsxy(
     & i1,i2,i3,2,1)+rsxy(i1,i2,i3,0,2)*rsxy(i1,i2,i3,2,2))*urt2(i1,
     & i2,i3,kd)+2.*(rsxy(i1,i2,i3,1,0)*rsxy(i1,i2,i3,2,0)+ rsxy(i1,
     & i2,i3,1,1)*rsxy(i1,i2,i3,2,1)+rsxy(i1,i2,i3,1,2)*rsxy(i1,i2,i3,
     & 2,2))*ust2(i1,i2,i3,kd)+(rsxyx23(i1,i2,i3,0,0)+rsxyy23(i1,i2,
     & i3,0,1)+rsxyz23(i1,i2,i3,0,2))*ur2(i1,i2,i3,kd)+(rsxyx23(i1,i2,
     & i3,1,0)+rsxyy23(i1,i2,i3,1,1)+rsxyz23(i1,i2,i3,1,2))*us2(i1,i2,
     & i3,kd)+(rsxyx23(i1,i2,i3,2,0)+rsxyy23(i1,i2,i3,2,1)+rsxyz23(i1,
     & i2,i3,2,2))*ut2(i1,i2,i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
         dx12(kd) = 1./(2.*dx(kd))
         dx22(kd) = 1./(dx(kd)**2)
         ux23r(i1,i2,i3,kd)=(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))*dx12(0)
         uy23r(i1,i2,i3,kd)=(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))*dx12(1)
         uz23r(i1,i2,i3,kd)=(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))*dx12(2)
         uxx23r(i1,i2,i3,kd)=(-2.*u(i1,i2,i3,kd)+(u(i1+1,i2,i3,kd)+u(
     & i1-1,i2,i3,kd)) )*dx22(0)
         uyy23r(i1,i2,i3,kd)=(-2.*u(i1,i2,i3,kd)+(u(i1,i2+1,i3,kd)+u(
     & i1,i2-1,i3,kd)) )*dx22(1)
         uxy23r(i1,i2,i3,kd)=(ux23r(i1,i2+1,i3,kd)-ux23r(i1,i2-1,i3,kd)
     & )*dx12(1)
         uzz23r(i1,i2,i3,kd)=(-2.*u(i1,i2,i3,kd)+(u(i1,i2,i3+1,kd)+u(
     & i1,i2,i3-1,kd)) )*dx22(2)
         uxz23r(i1,i2,i3,kd)=(ux23r(i1,i2,i3+1,kd)-ux23r(i1,i2,i3-1,kd)
     & )*dx12(2)
         uyz23r(i1,i2,i3,kd)=(uy23r(i1,i2,i3+1,kd)-uy23r(i1,i2,i3-1,kd)
     & )*dx12(2)
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
     & (u(i1+2,i2,i3,kd)-u(i1-2,i2,i3,kd)) )*dx22(0)*dx12(0)
         uyyy22r(i1,i2,i3,kd)=(-2.*(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))+
     & (u(i1,i2+2,i3,kd)-u(i1,i2-2,i3,kd)) )*dx22(1)*dx12(1)
         uxxy22r(i1,i2,i3,kd)=( uxx22r(i1,i2+1,i3,kd)-uxx22r(i1,i2-1,
     & i3,kd))/(2.*dx(1))
         uxyy22r(i1,i2,i3,kd)=( uyy22r(i1+1,i2,i3,kd)-uyy22r(i1-1,i2,
     & i3,kd))/(2.*dx(0))
         uxxxx22r(i1,i2,i3,kd)=(6.*u(i1,i2,i3,kd)-4.*(u(i1+1,i2,i3,kd)+
     & u(i1-1,i2,i3,kd)) +(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd)) )/(dx(0)
     & **4)
         uyyyy22r(i1,i2,i3,kd)=(6.*u(i1,i2,i3,kd)-4.*(u(i1,i2+1,i3,kd)+
     & u(i1,i2-1,i3,kd)) +(u(i1,i2+2,i3,kd)+u(i1,i2-2,i3,kd)) )/(dx(1)
     & **4)
         uxxyy22r(i1,i2,i3,kd)=( 4.*u(i1,i2,i3,kd)-2.*(u(i1+1,i2,i3,kd)
     & +u(i1-1,i2,i3,kd)+u(i1,i2+1,i3,kd)+u(i1,i2-1,i3,kd))+   (u(i1+
     & 1,i2+1,i3,kd)+u(i1-1,i2+1,i3,kd)+u(i1+1,i2-1,i3,kd)+u(i1-1,i2-
     & 1,i3,kd)) )/(dx(0)**2*dx(1)**2)
         uLapSq22r(i1,i2,i3,kd)= ( 6.*u(i1,i2,i3,kd)- 4.*(u(i1+1,i2,i3,
     & kd)+u(i1-1,i2,i3,kd))+(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd)) )/(
     & dx(0)**4)+( 6.*u(i1,i2,i3,kd)-4.*(u(i1,i2+1,i3,kd)+u(i1,i2-1,
     & i3,kd)) +(u(i1,i2+2,i3,kd)+u(i1,i2-2,i3,kd)) )/(dx(1)**4)+( 8.*
     & u(i1,i2,i3,kd)-4.*(u(i1+1,i2,i3,kd)+u(i1-1,i2,i3,kd)+u(i1,i2+1,
     & i3,kd)+u(i1,i2-1,i3,kd))+2.*(u(i1+1,i2+1,i3,kd)+u(i1-1,i2+1,i3,
     & kd)+u(i1+1,i2-1,i3,kd)+u(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**
     & 2)
         uxxx23r(i1,i2,i3,kd)=(-2.*(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))+
     & (u(i1+2,i2,i3,kd)-u(i1-2,i2,i3,kd)) )*dx22(0)*dx12(0)
         uyyy23r(i1,i2,i3,kd)=(-2.*(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))+
     & (u(i1,i2+2,i3,kd)-u(i1,i2-2,i3,kd)) )*dx22(1)*dx12(1)
         uzzz23r(i1,i2,i3,kd)=(-2.*(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))+
     & (u(i1,i2,i3+2,kd)-u(i1,i2,i3-2,kd)) )*dx22(1)*dx12(2)
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
         uxxyy23r(i1,i2,i3,kd)=( 4.*u(i1,i2,i3,kd)-2.*(u(i1+1,i2,i3,kd)
     & +u(i1-1,i2,i3,kd)+u(i1,i2+1,i3,kd)+u(i1,i2-1,i3,kd))+   (u(i1+
     & 1,i2+1,i3,kd)+u(i1-1,i2+1,i3,kd)+u(i1+1,i2-1,i3,kd)+u(i1-1,i2-
     & 1,i3,kd)) )/(dx(0)**2*dx(1)**2)
         uxxzz23r(i1,i2,i3,kd)=( 4.*u(i1,i2,i3,kd)-2.*(u(i1+1,i2,i3,kd)
     & +u(i1-1,i2,i3,kd)+u(i1,i2,i3+1,kd)+u(i1,i2,i3-1,kd))+   (u(i1+
     & 1,i2,i3+1,kd)+u(i1-1,i2,i3+1,kd)+u(i1+1,i2,i3-1,kd)+u(i1-1,i2,
     & i3-1,kd)) )/(dx(0)**2*dx(2)**2)
         uyyzz23r(i1,i2,i3,kd)=( 4.*u(i1,i2,i3,kd)-2.*(u(i1,i2+1,i3,kd)
     &   +u(i1,i2-1,i3,kd)+  u(i1,i2  ,i3+1,kd)+u(i1,i2  ,i3-1,kd))+  
     &  (u(i1,i2+1,i3+1,kd)+u(i1,i2-1,i3+1,kd)+u(i1,i2+1,i3-1,kd)+u(
     & i1,i2-1,i3-1,kd)) )/(dx(1)**2*dx(2)**2)
         dr14(kd) = 1./(12.*dr(kd))
         dr24(kd) = 1./(12.*dr(kd)**2)
         ur4(i1,i2,i3,kd)=(8.*(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))-(u(
     & i1+2,i2,i3,kd)-u(i1-2,i2,i3,kd)))*dr14(0)
         us4(i1,i2,i3,kd)=(8.*(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))-(u(
     & i1,i2+2,i3,kd)-u(i1,i2-2,i3,kd)))*dr14(1)
         ut4(i1,i2,i3,kd)=(8.*(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))-(u(
     & i1,i2,i3+2,kd)-u(i1,i2,i3-2,kd)))*dr14(2)
         urr4(i1,i2,i3,kd)=(-30.*u(i1,i2,i3,kd)+16.*(u(i1+1,i2,i3,kd)+
     & u(i1-1,i2,i3,kd))-(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd)) )*dr24(0)
         uss4(i1,i2,i3,kd)=(-30.*u(i1,i2,i3,kd)+16.*(u(i1,i2+1,i3,kd)+
     & u(i1,i2-1,i3,kd))-(u(i1,i2+2,i3,kd)+u(i1,i2-2,i3,kd)) )*dr24(1)
         utt4(i1,i2,i3,kd)=(-30.*u(i1,i2,i3,kd)+16.*(u(i1,i2,i3+1,kd)+
     & u(i1,i2,i3-1,kd))-(u(i1,i2,i3+2,kd)+u(i1,i2,i3-2,kd)) )*dr24(2)
         urs4(i1,i2,i3,kd)=(8.*(ur4(i1,i2+1,i3,kd)-ur4(i1,i2-1,i3,kd))-
     & (ur4(i1,i2+2,i3,kd)-ur4(i1,i2-2,i3,kd)))*dr14(1)
         urt4(i1,i2,i3,kd)=(8.*(ur4(i1,i2,i3+1,kd)-ur4(i1,i2,i3-1,kd))-
     & (ur4(i1,i2,i3+2,kd)-ur4(i1,i2,i3-2,kd)))*dr14(2)
         ust4(i1,i2,i3,kd)=(8.*(us4(i1,i2,i3+1,kd)-us4(i1,i2,i3-1,kd))-
     & (us4(i1,i2,i3+2,kd)-us4(i1,i2,i3-2,kd)))*dr14(2)
         rsxyr4(i1,i2,i3,m,n)=(8.*(rsxy(i1+1,i2,i3,m,n)-rsxy(i1-1,i2,
     & i3,m,n))-(rsxy(i1+2,i2,i3,m,n)-rsxy(i1-2,i2,i3,m,n)))*dr14(0)
         rsxys4(i1,i2,i3,m,n)=(8.*(rsxy(i1,i2+1,i3,m,n)-rsxy(i1,i2-1,
     & i3,m,n))-(rsxy(i1,i2+2,i3,m,n)-rsxy(i1,i2-2,i3,m,n)))*dr14(1)
         rsxyt4(i1,i2,i3,m,n)=(8.*(rsxy(i1,i2,i3+1,m,n)-rsxy(i1,i2,i3-
     & 1,m,n))-(rsxy(i1,i2,i3+2,m,n)-rsxy(i1,i2,i3-2,m,n)))*dr14(2)
         ux41(i1,i2,i3,kd)= rsxy(i1,i2,i3,0,0)*ur4(i1,i2,i3,kd)
         uy41(i1,i2,i3,kd)=0
         uz41(i1,i2,i3,kd)=0
         ux42(i1,i2,i3,kd)= rsxy(i1,i2,i3,0,0)*ur4(i1,i2,i3,kd)+rsxy(
     & i1,i2,i3,1,0)*us4(i1,i2,i3,kd)
         uy42(i1,i2,i3,kd)= rsxy(i1,i2,i3,0,1)*ur4(i1,i2,i3,kd)+rsxy(
     & i1,i2,i3,1,1)*us4(i1,i2,i3,kd)
         uz42(i1,i2,i3,kd)=0
         ux43(i1,i2,i3,kd)=rsxy(i1,i2,i3,0,0)*ur4(i1,i2,i3,kd)+rsxy(i1,
     & i2,i3,1,0)*us4(i1,i2,i3,kd)+rsxy(i1,i2,i3,2,0)*ut4(i1,i2,i3,kd)
         uy43(i1,i2,i3,kd)=rsxy(i1,i2,i3,0,1)*ur4(i1,i2,i3,kd)+rsxy(i1,
     & i2,i3,1,1)*us4(i1,i2,i3,kd)+rsxy(i1,i2,i3,2,1)*ut4(i1,i2,i3,kd)
         uz43(i1,i2,i3,kd)=rsxy(i1,i2,i3,0,2)*ur4(i1,i2,i3,kd)+rsxy(i1,
     & i2,i3,1,2)*us4(i1,i2,i3,kd)+rsxy(i1,i2,i3,2,2)*ut4(i1,i2,i3,kd)
         rsxyx41(i1,i2,i3,m,n)= rsxy(i1,i2,i3,0,0)*rsxyr4(i1,i2,i3,m,n)
         rsxyx42(i1,i2,i3,m,n)= rsxy(i1,i2,i3,0,0)*rsxyr4(i1,i2,i3,m,n)
     & +rsxy(i1,i2,i3,1,0)*rsxys4(i1,i2,i3,m,n)
         rsxyy42(i1,i2,i3,m,n)= rsxy(i1,i2,i3,0,1)*rsxyr4(i1,i2,i3,m,n)
     & +rsxy(i1,i2,i3,1,1)*rsxys4(i1,i2,i3,m,n)
         rsxyx43(i1,i2,i3,m,n)=rsxy(i1,i2,i3,0,0)*rsxyr4(i1,i2,i3,m,n)+
     & rsxy(i1,i2,i3,1,0)*rsxys4(i1,i2,i3,m,n)+rsxy(i1,i2,i3,2,0)*
     & rsxyt4(i1,i2,i3,m,n)
         rsxyy43(i1,i2,i3,m,n)=rsxy(i1,i2,i3,0,1)*rsxyr4(i1,i2,i3,m,n)+
     & rsxy(i1,i2,i3,1,1)*rsxys4(i1,i2,i3,m,n)+rsxy(i1,i2,i3,2,1)*
     & rsxyt4(i1,i2,i3,m,n)
         rsxyz43(i1,i2,i3,m,n)=rsxy(i1,i2,i3,0,2)*rsxyr4(i1,i2,i3,m,n)+
     & rsxy(i1,i2,i3,1,2)*rsxys4(i1,i2,i3,m,n)+rsxy(i1,i2,i3,2,2)*
     & rsxyt4(i1,i2,i3,m,n)
         uxx41(i1,i2,i3,kd)=(rsxy(i1,i2,i3,0,0)**2)*urr4(i1,i2,i3,kd)+(
     & rsxyx42(i1,i2,i3,0,0))*ur4(i1,i2,i3,kd)
         uyy41(i1,i2,i3,kd)=0
         uxy41(i1,i2,i3,kd)=0
         uxz41(i1,i2,i3,kd)=0
         uyz41(i1,i2,i3,kd)=0
         uzz41(i1,i2,i3,kd)=0
         ulaplacian41(i1,i2,i3,kd)=uxx41(i1,i2,i3,kd)
         uxx42(i1,i2,i3,kd)=(rsxy(i1,i2,i3,0,0)**2)*urr4(i1,i2,i3,kd)+
     & 2.*(rsxy(i1,i2,i3,0,0)*rsxy(i1,i2,i3,1,0))*urs4(i1,i2,i3,kd)+(
     & rsxy(i1,i2,i3,1,0)**2)*uss4(i1,i2,i3,kd)+(rsxyx42(i1,i2,i3,0,0)
     & )*ur4(i1,i2,i3,kd)+(rsxyx42(i1,i2,i3,1,0))*us4(i1,i2,i3,kd)
         uyy42(i1,i2,i3,kd)=(rsxy(i1,i2,i3,0,1)**2)*urr4(i1,i2,i3,kd)+
     & 2.*(rsxy(i1,i2,i3,0,1)*rsxy(i1,i2,i3,1,1))*urs4(i1,i2,i3,kd)+(
     & rsxy(i1,i2,i3,1,1)**2)*uss4(i1,i2,i3,kd)+(rsxyy42(i1,i2,i3,0,1)
     & )*ur4(i1,i2,i3,kd)+(rsxyy42(i1,i2,i3,1,1))*us4(i1,i2,i3,kd)
         uxy42(i1,i2,i3,kd)=rsxy(i1,i2,i3,0,0)*rsxy(i1,i2,i3,0,1)*urr4(
     & i1,i2,i3,kd)+(rsxy(i1,i2,i3,0,0)*rsxy(i1,i2,i3,1,1)+rsxy(i1,i2,
     & i3,0,1)*rsxy(i1,i2,i3,1,0))*urs4(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,
     & 0)*rsxy(i1,i2,i3,1,1)*uss4(i1,i2,i3,kd)+rsxyx42(i1,i2,i3,0,1)*
     & ur4(i1,i2,i3,kd)+rsxyx42(i1,i2,i3,1,1)*us4(i1,i2,i3,kd)
         uxz42(i1,i2,i3,kd)=0
         uyz42(i1,i2,i3,kd)=0
         uzz42(i1,i2,i3,kd)=0
         ulaplacian42(i1,i2,i3,kd)=(rsxy(i1,i2,i3,0,0)**2+rsxy(i1,i2,
     & i3,0,1)**2)*urr4(i1,i2,i3,kd)+2.*(rsxy(i1,i2,i3,0,0)*rsxy(i1,
     & i2,i3,1,0)+ rsxy(i1,i2,i3,0,1)*rsxy(i1,i2,i3,1,1))*urs4(i1,i2,
     & i3,kd)+(rsxy(i1,i2,i3,1,0)**2+rsxy(i1,i2,i3,1,1)**2)*uss4(i1,
     & i2,i3,kd)+(rsxyx42(i1,i2,i3,0,0)+rsxyy42(i1,i2,i3,0,1))*ur4(i1,
     & i2,i3,kd)+(rsxyx42(i1,i2,i3,1,0)+rsxyy42(i1,i2,i3,1,1))*us4(i1,
     & i2,i3,kd)
         uxx43(i1,i2,i3,kd)=rsxy(i1,i2,i3,0,0)**2*urr4(i1,i2,i3,kd)+
     & rsxy(i1,i2,i3,1,0)**2*uss4(i1,i2,i3,kd)+rsxy(i1,i2,i3,2,0)**2*
     & utt4(i1,i2,i3,kd)+2.*rsxy(i1,i2,i3,0,0)*rsxy(i1,i2,i3,1,0)*
     & urs4(i1,i2,i3,kd)+2.*rsxy(i1,i2,i3,0,0)*rsxy(i1,i2,i3,2,0)*
     & urt4(i1,i2,i3,kd)+2.*rsxy(i1,i2,i3,1,0)*rsxy(i1,i2,i3,2,0)*
     & ust4(i1,i2,i3,kd)+rsxyx43(i1,i2,i3,0,0)*ur4(i1,i2,i3,kd)+
     & rsxyx43(i1,i2,i3,1,0)*us4(i1,i2,i3,kd)+rsxyx43(i1,i2,i3,2,0)*
     & ut4(i1,i2,i3,kd)
         uyy43(i1,i2,i3,kd)=rsxy(i1,i2,i3,0,1)**2*urr4(i1,i2,i3,kd)+
     & rsxy(i1,i2,i3,1,1)**2*uss4(i1,i2,i3,kd)+rsxy(i1,i2,i3,2,1)**2*
     & utt4(i1,i2,i3,kd)+2.*rsxy(i1,i2,i3,0,1)*rsxy(i1,i2,i3,1,1)*
     & urs4(i1,i2,i3,kd)+2.*rsxy(i1,i2,i3,0,1)*rsxy(i1,i2,i3,2,1)*
     & urt4(i1,i2,i3,kd)+2.*rsxy(i1,i2,i3,1,1)*rsxy(i1,i2,i3,2,1)*
     & ust4(i1,i2,i3,kd)+rsxyy43(i1,i2,i3,0,1)*ur4(i1,i2,i3,kd)+
     & rsxyy43(i1,i2,i3,1,1)*us4(i1,i2,i3,kd)+rsxyy43(i1,i2,i3,2,1)*
     & ut4(i1,i2,i3,kd)
         uzz43(i1,i2,i3,kd)=rsxy(i1,i2,i3,0,2)**2*urr4(i1,i2,i3,kd)+
     & rsxy(i1,i2,i3,1,2)**2*uss4(i1,i2,i3,kd)+rsxy(i1,i2,i3,2,2)**2*
     & utt4(i1,i2,i3,kd)+2.*rsxy(i1,i2,i3,0,2)*rsxy(i1,i2,i3,1,2)*
     & urs4(i1,i2,i3,kd)+2.*rsxy(i1,i2,i3,0,2)*rsxy(i1,i2,i3,2,2)*
     & urt4(i1,i2,i3,kd)+2.*rsxy(i1,i2,i3,1,2)*rsxy(i1,i2,i3,2,2)*
     & ust4(i1,i2,i3,kd)+rsxyz43(i1,i2,i3,0,2)*ur4(i1,i2,i3,kd)+
     & rsxyz43(i1,i2,i3,1,2)*us4(i1,i2,i3,kd)+rsxyz43(i1,i2,i3,2,2)*
     & ut4(i1,i2,i3,kd)
         uxy43(i1,i2,i3,kd)=rsxy(i1,i2,i3,0,0)*rsxy(i1,i2,i3,0,1)*urr4(
     & i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*rsxy(i1,i2,i3,1,1)*uss4(i1,i2,
     & i3,kd)+rsxy(i1,i2,i3,2,0)*rsxy(i1,i2,i3,2,1)*utt4(i1,i2,i3,kd)+
     & (rsxy(i1,i2,i3,0,0)*rsxy(i1,i2,i3,1,1)+rsxy(i1,i2,i3,0,1)*rsxy(
     & i1,i2,i3,1,0))*urs4(i1,i2,i3,kd)+(rsxy(i1,i2,i3,0,0)*rsxy(i1,
     & i2,i3,2,1)+rsxy(i1,i2,i3,0,1)*rsxy(i1,i2,i3,2,0))*urt4(i1,i2,
     & i3,kd)+(rsxy(i1,i2,i3,1,0)*rsxy(i1,i2,i3,2,1)+rsxy(i1,i2,i3,1,
     & 1)*rsxy(i1,i2,i3,2,0))*ust4(i1,i2,i3,kd)+rsxyx43(i1,i2,i3,0,1)*
     & ur4(i1,i2,i3,kd)+rsxyx43(i1,i2,i3,1,1)*us4(i1,i2,i3,kd)+
     & rsxyx43(i1,i2,i3,2,1)*ut4(i1,i2,i3,kd)
         uxz43(i1,i2,i3,kd)=rsxy(i1,i2,i3,0,0)*rsxy(i1,i2,i3,0,2)*urr4(
     & i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*rsxy(i1,i2,i3,1,2)*uss4(i1,i2,
     & i3,kd)+rsxy(i1,i2,i3,2,0)*rsxy(i1,i2,i3,2,2)*utt4(i1,i2,i3,kd)+
     & (rsxy(i1,i2,i3,0,0)*rsxy(i1,i2,i3,1,2)+rsxy(i1,i2,i3,0,2)*rsxy(
     & i1,i2,i3,1,0))*urs4(i1,i2,i3,kd)+(rsxy(i1,i2,i3,0,0)*rsxy(i1,
     & i2,i3,2,2)+rsxy(i1,i2,i3,0,2)*rsxy(i1,i2,i3,2,0))*urt4(i1,i2,
     & i3,kd)+(rsxy(i1,i2,i3,1,0)*rsxy(i1,i2,i3,2,2)+rsxy(i1,i2,i3,1,
     & 2)*rsxy(i1,i2,i3,2,0))*ust4(i1,i2,i3,kd)+rsxyx43(i1,i2,i3,0,2)*
     & ur4(i1,i2,i3,kd)+rsxyx43(i1,i2,i3,1,2)*us4(i1,i2,i3,kd)+
     & rsxyx43(i1,i2,i3,2,2)*ut4(i1,i2,i3,kd)
         uyz43(i1,i2,i3,kd)=rsxy(i1,i2,i3,0,1)*rsxy(i1,i2,i3,0,2)*urr4(
     & i1,i2,i3,kd)+rsxy(i1,i2,i3,1,1)*rsxy(i1,i2,i3,1,2)*uss4(i1,i2,
     & i3,kd)+rsxy(i1,i2,i3,2,1)*rsxy(i1,i2,i3,2,2)*utt4(i1,i2,i3,kd)+
     & (rsxy(i1,i2,i3,0,1)*rsxy(i1,i2,i3,1,2)+rsxy(i1,i2,i3,0,2)*rsxy(
     & i1,i2,i3,1,1))*urs4(i1,i2,i3,kd)+(rsxy(i1,i2,i3,0,1)*rsxy(i1,
     & i2,i3,2,2)+rsxy(i1,i2,i3,0,2)*rsxy(i1,i2,i3,2,1))*urt4(i1,i2,
     & i3,kd)+(rsxy(i1,i2,i3,1,1)*rsxy(i1,i2,i3,2,2)+rsxy(i1,i2,i3,1,
     & 2)*rsxy(i1,i2,i3,2,1))*ust4(i1,i2,i3,kd)+rsxyy43(i1,i2,i3,0,2)*
     & ur4(i1,i2,i3,kd)+rsxyy43(i1,i2,i3,1,2)*us4(i1,i2,i3,kd)+
     & rsxyy43(i1,i2,i3,2,2)*ut4(i1,i2,i3,kd)
         ulaplacian43(i1,i2,i3,kd)=(rsxy(i1,i2,i3,0,0)**2+rsxy(i1,i2,
     & i3,0,1)**2+rsxy(i1,i2,i3,0,2)**2)*urr4(i1,i2,i3,kd)+(rsxy(i1,
     & i2,i3,1,0)**2+rsxy(i1,i2,i3,1,1)**2+rsxy(i1,i2,i3,1,2)**2)*
     & uss4(i1,i2,i3,kd)+(rsxy(i1,i2,i3,2,0)**2+rsxy(i1,i2,i3,2,1)**2+
     & rsxy(i1,i2,i3,2,2)**2)*utt4(i1,i2,i3,kd)+2.*(rsxy(i1,i2,i3,0,0)
     & *rsxy(i1,i2,i3,1,0)+ rsxy(i1,i2,i3,0,1)*rsxy(i1,i2,i3,1,1)+
     & rsxy(i1,i2,i3,0,2)*rsxy(i1,i2,i3,1,2))*urs4(i1,i2,i3,kd)+2.*(
     & rsxy(i1,i2,i3,0,0)*rsxy(i1,i2,i3,2,0)+ rsxy(i1,i2,i3,0,1)*rsxy(
     & i1,i2,i3,2,1)+rsxy(i1,i2,i3,0,2)*rsxy(i1,i2,i3,2,2))*urt4(i1,
     & i2,i3,kd)+2.*(rsxy(i1,i2,i3,1,0)*rsxy(i1,i2,i3,2,0)+ rsxy(i1,
     & i2,i3,1,1)*rsxy(i1,i2,i3,2,1)+rsxy(i1,i2,i3,1,2)*rsxy(i1,i2,i3,
     & 2,2))*ust4(i1,i2,i3,kd)+(rsxyx43(i1,i2,i3,0,0)+rsxyy43(i1,i2,
     & i3,0,1)+rsxyz43(i1,i2,i3,0,2))*ur4(i1,i2,i3,kd)+(rsxyx43(i1,i2,
     & i3,1,0)+rsxyy43(i1,i2,i3,1,1)+rsxyz43(i1,i2,i3,1,2))*us4(i1,i2,
     & i3,kd)+(rsxyx43(i1,i2,i3,2,0)+rsxyy43(i1,i2,i3,2,1)+rsxyz43(i1,
     & i2,i3,2,2))*ut4(i1,i2,i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
         dx41(kd) = 1./(12.*dx(kd))
         dx42(kd) = 1./(12.*dx(kd)**2)
         ux43r(i1,i2,i3,kd)=(8.*(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))-(u(
     & i1+2,i2,i3,kd)-u(i1-2,i2,i3,kd)))*dx41(0)
         uy43r(i1,i2,i3,kd)=(8.*(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))-(u(
     & i1,i2+2,i3,kd)-u(i1,i2-2,i3,kd)))*dx41(1)
         uz43r(i1,i2,i3,kd)=(8.*(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))-(u(
     & i1,i2,i3+2,kd)-u(i1,i2,i3-2,kd)))*dx41(2)
         uxx43r(i1,i2,i3,kd)=( -30.*u(i1,i2,i3,kd)+16.*(u(i1+1,i2,i3,
     & kd)+u(i1-1,i2,i3,kd))-(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd)) )*
     & dx42(0)
         uyy43r(i1,i2,i3,kd)=( -30.*u(i1,i2,i3,kd)+16.*(u(i1,i2+1,i3,
     & kd)+u(i1,i2-1,i3,kd))-(u(i1,i2+2,i3,kd)+u(i1,i2-2,i3,kd)) )*
     & dx42(1)
         uzz43r(i1,i2,i3,kd)=( -30.*u(i1,i2,i3,kd)+16.*(u(i1,i2,i3+1,
     & kd)+u(i1,i2,i3-1,kd))-(u(i1,i2,i3+2,kd)+u(i1,i2,i3-2,kd)) )*
     & dx42(2)
         uxy43r(i1,i2,i3,kd)=( (u(i1+2,i2+2,i3,kd)-u(i1-2,i2+2,i3,kd)- 
     & u(i1+2,i2-2,i3,kd)+u(i1-2,i2-2,i3,kd)) +8.*(u(i1-1,i2+2,i3,kd)-
     & u(i1-1,i2-2,i3,kd)-u(i1+1,i2+2,i3,kd)+u(i1+1,i2-2,i3,kd) +u(i1+
     & 2,i2-1,i3,kd)-u(i1-2,i2-1,i3,kd)-u(i1+2,i2+1,i3,kd)+u(i1-2,i2+
     & 1,i3,kd))+64.*(u(i1+1,i2+1,i3,kd)-u(i1-1,i2+1,i3,kd)- u(i1+1,
     & i2-1,i3,kd)+u(i1-1,i2-1,i3,kd)))*(dx41(0)*dx41(1))
         uxz43r(i1,i2,i3,kd)=( (u(i1+2,i2,i3+2,kd)-u(i1-2,i2,i3+2,kd)-
     & u(i1+2,i2,i3-2,kd)+u(i1-2,i2,i3-2,kd)) +8.*(u(i1-1,i2,i3+2,kd)-
     & u(i1-1,i2,i3-2,kd)-u(i1+1,i2,i3+2,kd)+u(i1+1,i2,i3-2,kd) +u(i1+
     & 2,i2,i3-1,kd)-u(i1-2,i2,i3-1,kd)- u(i1+2,i2,i3+1,kd)+u(i1-2,i2,
     & i3+1,kd)) +64.*(u(i1+1,i2,i3+1,kd)-u(i1-1,i2,i3+1,kd)-u(i1+1,
     & i2,i3-1,kd)+u(i1-1,i2,i3-1,kd)) )*(dx41(0)*dx41(2))
         uyz43r(i1,i2,i3,kd)=( (u(i1,i2+2,i3+2,kd)-u(i1,i2-2,i3+2,kd)-
     & u(i1,i2+2,i3-2,kd)+u(i1,i2-2,i3-2,kd)) +8.*(u(i1,i2-1,i3+2,kd)-
     & u(i1,i2-1,i3-2,kd)-u(i1,i2+1,i3+2,kd)+u(i1,i2+1,i3-2,kd) +u(i1,
     & i2+2,i3-1,kd)-u(i1,i2-2,i3-1,kd)-u(i1,i2+2,i3+1,kd)+u(i1,i2-2,
     & i3+1,kd)) +64.*(u(i1,i2+1,i3+1,kd)-u(i1,i2-1,i3+1,kd)-u(i1,i2+
     & 1,i3-1,kd)+u(i1,i2-1,i3-1,kd)) )*(dx41(1)*dx41(2))
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
         ulaplacian42r(i1,i2,i3,kd)=uxx43r(i1,i2,i3,kd)+uyy43r(i1,i2,
     & i3,kd)
         ulaplacian43r(i1,i2,i3,kd)=uxx43r(i1,i2,i3,kd)+uyy43r(i1,i2,
     & i3,kd)+uzz43r(i1,i2,i3,kd)
         dr112(kd) = 1./(2.*dr1(kd))
         dr122(kd) = 1./(dr1(kd)**2)
         udfr2(i1,i2,i3,kd)=(udf(i1+1,i2,i3,kd)-udf(i1-1,i2,i3,kd))*
     & dr112(0)
         udfs2(i1,i2,i3,kd)=(udf(i1,i2+1,i3,kd)-udf(i1,i2-1,i3,kd))*
     & dr112(1)
         udft2(i1,i2,i3,kd)=(udf(i1,i2,i3+1,kd)-udf(i1,i2,i3-1,kd))*
     & dr112(2)
         udfrr2(i1,i2,i3,kd)=(-2.*udf(i1,i2,i3,kd)+(udf(i1+1,i2,i3,kd)+
     & udf(i1-1,i2,i3,kd)) )*dr122(0)
         udfss2(i1,i2,i3,kd)=(-2.*udf(i1,i2,i3,kd)+(udf(i1,i2+1,i3,kd)+
     & udf(i1,i2-1,i3,kd)) )*dr122(1)
         udfrs2(i1,i2,i3,kd)=(udfr2(i1,i2+1,i3,kd)-udfr2(i1,i2-1,i3,kd)
     & )*dr112(1)
         udftt2(i1,i2,i3,kd)=(-2.*udf(i1,i2,i3,kd)+(udf(i1,i2,i3+1,kd)+
     & udf(i1,i2,i3-1,kd)) )*dr122(2)
         udfrt2(i1,i2,i3,kd)=(udfr2(i1,i2,i3+1,kd)-udfr2(i1,i2,i3-1,kd)
     & )*dr112(2)
         udfst2(i1,i2,i3,kd)=(udfs2(i1,i2,i3+1,kd)-udfs2(i1,i2,i3-1,kd)
     & )*dr112(2)
         udfrrr2(i1,i2,i3,kd)=(-2.*(udf(i1+1,i2,i3,kd)-udf(i1-1,i2,i3,
     & kd))+(udf(i1+2,i2,i3,kd)-udf(i1-2,i2,i3,kd)) )*dr122(0)*dr112(
     & 0)
         udfsss2(i1,i2,i3,kd)=(-2.*(udf(i1,i2+1,i3,kd)-udf(i1,i2-1,i3,
     & kd))+(udf(i1,i2+2,i3,kd)-udf(i1,i2-2,i3,kd)) )*dr122(1)*dr112(
     & 1)
         udfttt2(i1,i2,i3,kd)=(-2.*(udf(i1,i2,i3+1,kd)-udf(i1,i2,i3-1,
     & kd))+(udf(i1,i2,i3+2,kd)-udf(i1,i2,i3-2,kd)) )*dr122(1)*dr112(
     & 2)
         udfrrs2(i1,i2,i3,kd)=( udfrr2(i1,i2+1,i3,kd)-udfrr2(i1,i2-1,
     & i3,kd))/(2.*dr1(1))
         udfrss2(i1,i2,i3,kd)=( udfss2(i1+1,i2,i3,kd)-udfss2(i1-1,i2,
     & i3,kd))/(2.*dr1(0))
         udfrrt2(i1,i2,i3,kd)=( udfrr2(i1,i2,i3+1,kd)-udfrr2(i1,i2,i3-
     & 1,kd))/(2.*dr1(2))
         udfsst2(i1,i2,i3,kd)=( udfss2(i1,i2,i3+1,kd)-udfss2(i1,i2,i3-
     & 1,kd))/(2.*dr1(2))
         udfrtt2(i1,i2,i3,kd)=( udftt2(i1+1,i2,i3,kd)-udftt2(i1-1,i2,
     & i3,kd))/(2.*dr1(0))
         udfstt2(i1,i2,i3,kd)=( udftt2(i1,i2+1,i3,kd)-udftt2(i1,i2-1,
     & i3,kd))/(2.*dr1(1))
         udfrrrr2(i1,i2,i3,kd)=(6.*udf(i1,i2,i3,kd)-4.*(udf(i1+1,i2,i3,
     & kd)+udf(i1-1,i2,i3,kd))+(udf(i1+2,i2,i3,kd)+udf(i1-2,i2,i3,kd))
     &  )/(dr1(0)**4)
         udfssss2(i1,i2,i3,kd)=(6.*udf(i1,i2,i3,kd)-4.*(udf(i1,i2+1,i3,
     & kd)+udf(i1,i2-1,i3,kd))+(udf(i1,i2+2,i3,kd)+udf(i1,i2-2,i3,kd))
     &  )/(dr1(1)**4)
         udftttt2(i1,i2,i3,kd)=(6.*udf(i1,i2,i3,kd)-4.*(udf(i1,i2,i3+1,
     & kd)+udf(i1,i2,i3-1,kd))+(udf(i1,i2,i3+2,kd)+udf(i1,i2,i3-2,kd))
     &  )/(dr1(2)**4)
         udfrrss2(i1,i2,i3,kd)=( 4.*udf(i1,i2,i3,kd)-2.*(udf(i1+1,i2,
     & i3,kd)+udf(i1-1,i2,i3,kd)+udf(i1,i2+1,i3,kd)+udf(i1,i2-1,i3,kd)
     & )+   (udf(i1+1,i2+1,i3,kd)+udf(i1-1,i2+1,i3,kd)+udf(i1+1,i2-1,
     & i3,kd)+udf(i1-1,i2-1,i3,kd)) )/(dr1(0)**2*dr1(1)**2)
         udfrrtt2(i1,i2,i3,kd)=( 4.*udf(i1,i2,i3,kd)-2.*(udf(i1+1,i2,
     & i3,kd)+udf(i1-1,i2,i3,kd)+udf(i1,i2,i3+1,kd)+udf(i1,i2,i3-1,kd)
     & )+   (udf(i1+1,i2,i3+1,kd)+udf(i1-1,i2,i3+1,kd)+udf(i1+1,i2,i3-
     & 1,kd)+udf(i1-1,i2,i3-1,kd)) )/(dr1(0)**2*dr1(2)**2)
         udfsstt2(i1,i2,i3,kd)=( 4.*udf(i1,i2,i3,kd)-2.*(udf(i1,i2+1,
     & i3,kd)  +udf(i1,i2-1,i3,kd)+  udf(i1,i2  ,i3+1,kd)+udf(i1,i2  ,
     & i3-1,kd))+   (udf(i1,i2+1,i3+1,kd)+udf(i1,i2-1,i3+1,kd)+udf(i1,
     & i2+1,i3-1,kd)+udf(i1,i2-1,i3-1,kd)) )/(dr1(1)**2*dr1(2)**2)
         udfrrrs2(i1,i2,i3,kd)=( udfrrr2(i1,i2+1,i3,kd)-udfrrr2(i1,i2-
     & 1,i3,kd))/(2.*dr1(1))
         udfrsss2(i1,i2,i3,kd)=( udfsss2(i1+1,i2,i3,kd)-udfsss2(i1-1,
     & i2,i3,kd))/(2.*dr1(0))
         udfrrrt2(i1,i2,i3,kd)=( udfrrr2(i1,i2,i3+1,kd)-udfrrr2(i1,i2,
     & i3-1,kd))/(2.*dr1(2))
         udfssst2(i1,i2,i3,kd)=( udfsss2(i1,i2,i3+1,kd)-udfsss2(i1,i2,
     & i3-1,kd))/(2.*dr1(2))
         udfrttt2(i1,i2,i3,kd)=( udfttt2(i1+1,i2,i3,kd)-udfttt2(i1-1,
     & i2,i3,kd))/(2.*dr1(0))
         udfsttt2(i1,i2,i3,kd)=( udfttt2(i1,i2+1,i3,kd)-udfttt2(i1,i2-
     & 1,i3,kd))/(2.*dr1(1))
         rsxy1r2(i1,i2,i3,m,n)=(rsxy1(i1+1,i2,i3,m,n)-rsxy1(i1-1,i2,i3,
     & m,n))*dr112(0)
         rsxy1s2(i1,i2,i3,m,n)=(rsxy1(i1,i2+1,i3,m,n)-rsxy1(i1,i2-1,i3,
     & m,n))*dr112(1)
         rsxy1t2(i1,i2,i3,m,n)=(rsxy1(i1,i2,i3+1,m,n)-rsxy1(i1,i2,i3-1,
     & m,n))*dr112(2)
         rsxy1rr2(i1,i2,i3,m,n)=(-2.*rsxy1(i1,i2,i3,m,n)+(rsxy1(i1+1,
     & i2,i3,m,n)+rsxy1(i1-1,i2,i3,m,n)) )*dr122(0)
         rsxy1ss2(i1,i2,i3,m,n)=(-2.*rsxy1(i1,i2,i3,m,n)+(rsxy1(i1,i2+
     & 1,i3,m,n)+rsxy1(i1,i2-1,i3,m,n)) )*dr122(1)
         rsxy1rs2(i1,i2,i3,m,n)=(rsxy1r2(i1,i2+1,i3,m,n)-rsxy1r2(i1,i2-
     & 1,i3,m,n))*dr112(1)
         rsxy1tt2(i1,i2,i3,m,n)=(-2.*rsxy1(i1,i2,i3,m,n)+(rsxy1(i1,i2,
     & i3+1,m,n)+rsxy1(i1,i2,i3-1,m,n)) )*dr122(2)
         rsxy1rt2(i1,i2,i3,m,n)=(rsxy1r2(i1,i2,i3+1,m,n)-rsxy1r2(i1,i2,
     & i3-1,m,n))*dr112(2)
         rsxy1st2(i1,i2,i3,m,n)=(rsxy1s2(i1,i2,i3+1,m,n)-rsxy1s2(i1,i2,
     & i3-1,m,n))*dr112(2)
         rsxy1rrr2(i1,i2,i3,m,n)=(-2.*(rsxy1(i1+1,i2,i3,m,n)-rsxy1(i1-
     & 1,i2,i3,m,n))+(rsxy1(i1+2,i2,i3,m,n)-rsxy1(i1-2,i2,i3,m,n)) )*
     & dr122(0)*dr112(0)
         rsxy1sss2(i1,i2,i3,m,n)=(-2.*(rsxy1(i1,i2+1,i3,m,n)-rsxy1(i1,
     & i2-1,i3,m,n))+(rsxy1(i1,i2+2,i3,m,n)-rsxy1(i1,i2-2,i3,m,n)) )*
     & dr122(1)*dr112(1)
         rsxy1ttt2(i1,i2,i3,m,n)=(-2.*(rsxy1(i1,i2,i3+1,m,n)-rsxy1(i1,
     & i2,i3-1,m,n))+(rsxy1(i1,i2,i3+2,m,n)-rsxy1(i1,i2,i3-2,m,n)) )*
     & dr122(1)*dr112(2)
         rsxy1rrs2(i1,i2,i3,m,n)=( rsxy1rr2(i1,i2+1,i3,m,n)-rsxy1rr2(
     & i1,i2-1,i3,m,n))/(2.*dr1(1))
         rsxy1rss2(i1,i2,i3,m,n)=( rsxy1ss2(i1+1,i2,i3,m,n)-rsxy1ss2(
     & i1-1,i2,i3,m,n))/(2.*dr1(0))
         rsxy1rrt2(i1,i2,i3,m,n)=( rsxy1rr2(i1,i2,i3+1,m,n)-rsxy1rr2(
     & i1,i2,i3-1,m,n))/(2.*dr1(2))
         rsxy1sst2(i1,i2,i3,m,n)=( rsxy1ss2(i1,i2,i3+1,m,n)-rsxy1ss2(
     & i1,i2,i3-1,m,n))/(2.*dr1(2))
         rsxy1rtt2(i1,i2,i3,m,n)=( rsxy1tt2(i1+1,i2,i3,m,n)-rsxy1tt2(
     & i1-1,i2,i3,m,n))/(2.*dr1(0))
         rsxy1stt2(i1,i2,i3,m,n)=( rsxy1tt2(i1,i2+1,i3,m,n)-rsxy1tt2(
     & i1,i2-1,i3,m,n))/(2.*dr1(1))
         rsxy1rrrr2(i1,i2,i3,m,n)=(6.*rsxy1(i1,i2,i3,m,n)-4.*(rsxy1(i1+
     & 1,i2,i3,m,n)+rsxy1(i1-1,i2,i3,m,n))+(rsxy1(i1+2,i2,i3,m,n)+
     & rsxy1(i1-2,i2,i3,m,n)) )/(dr1(0)**4)
         rsxy1ssss2(i1,i2,i3,m,n)=(6.*rsxy1(i1,i2,i3,m,n)-4.*(rsxy1(i1,
     & i2+1,i3,m,n)+rsxy1(i1,i2-1,i3,m,n))+(rsxy1(i1,i2+2,i3,m,n)+
     & rsxy1(i1,i2-2,i3,m,n)) )/(dr1(1)**4)
         rsxy1tttt2(i1,i2,i3,m,n)=(6.*rsxy1(i1,i2,i3,m,n)-4.*(rsxy1(i1,
     & i2,i3+1,m,n)+rsxy1(i1,i2,i3-1,m,n))+(rsxy1(i1,i2,i3+2,m,n)+
     & rsxy1(i1,i2,i3-2,m,n)) )/(dr1(2)**4)
         rsxy1rrss2(i1,i2,i3,m,n)=( 4.*rsxy1(i1,i2,i3,m,n)-2.*(rsxy1(
     & i1+1,i2,i3,m,n)+rsxy1(i1-1,i2,i3,m,n)+rsxy1(i1,i2+1,i3,m,n)+
     & rsxy1(i1,i2-1,i3,m,n))+   (rsxy1(i1+1,i2+1,i3,m,n)+rsxy1(i1-1,
     & i2+1,i3,m,n)+rsxy1(i1+1,i2-1,i3,m,n)+rsxy1(i1-1,i2-1,i3,m,n)) )
     & /(dr1(0)**2*dr1(1)**2)
         rsxy1rrtt2(i1,i2,i3,m,n)=( 4.*rsxy1(i1,i2,i3,m,n)-2.*(rsxy1(
     & i1+1,i2,i3,m,n)+rsxy1(i1-1,i2,i3,m,n)+rsxy1(i1,i2,i3+1,m,n)+
     & rsxy1(i1,i2,i3-1,m,n))+   (rsxy1(i1+1,i2,i3+1,m,n)+rsxy1(i1-1,
     & i2,i3+1,m,n)+rsxy1(i1+1,i2,i3-1,m,n)+rsxy1(i1-1,i2,i3-1,m,n)) )
     & /(dr1(0)**2*dr1(2)**2)
         rsxy1sstt2(i1,i2,i3,m,n)=( 4.*rsxy1(i1,i2,i3,m,n)-2.*(rsxy1(
     & i1,i2+1,i3,m,n)  +rsxy1(i1,i2-1,i3,m,n)+  rsxy1(i1,i2  ,i3+1,m,
     & n)+rsxy1(i1,i2  ,i3-1,m,n))+   (rsxy1(i1,i2+1,i3+1,m,n)+rsxy1(
     & i1,i2-1,i3+1,m,n)+rsxy1(i1,i2+1,i3-1,m,n)+rsxy1(i1,i2-1,i3-1,m,
     & n)) )/(dr1(1)**2*dr1(2)**2)
         udfx21(i1,i2,i3,kd)= rsxy1(i1,i2,i3,0,0)*udfr2(i1,i2,i3,kd)
         udfy21(i1,i2,i3,kd)=0
         udfz21(i1,i2,i3,kd)=0
         udfx22(i1,i2,i3,kd)= rsxy1(i1,i2,i3,0,0)*udfr2(i1,i2,i3,kd)+
     & rsxy1(i1,i2,i3,1,0)*udfs2(i1,i2,i3,kd)
         udfy22(i1,i2,i3,kd)= rsxy1(i1,i2,i3,0,1)*udfr2(i1,i2,i3,kd)+
     & rsxy1(i1,i2,i3,1,1)*udfs2(i1,i2,i3,kd)
         udfz22(i1,i2,i3,kd)=0
         udfx23(i1,i2,i3,kd)=rsxy1(i1,i2,i3,0,0)*udfr2(i1,i2,i3,kd)+
     & rsxy1(i1,i2,i3,1,0)*udfs2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,2,0)*
     & udft2(i1,i2,i3,kd)
         udfy23(i1,i2,i3,kd)=rsxy1(i1,i2,i3,0,1)*udfr2(i1,i2,i3,kd)+
     & rsxy1(i1,i2,i3,1,1)*udfs2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,2,1)*
     & udft2(i1,i2,i3,kd)
         udfz23(i1,i2,i3,kd)=rsxy1(i1,i2,i3,0,2)*udfr2(i1,i2,i3,kd)+
     & rsxy1(i1,i2,i3,1,2)*udfs2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,2,2)*
     & udft2(i1,i2,i3,kd)
         rsxy1x21(i1,i2,i3,m,n)= rsxy1(i1,i2,i3,0,0)*rsxy1r2(i1,i2,i3,
     & m,n)
         rsxy1x22(i1,i2,i3,m,n)= rsxy1(i1,i2,i3,0,0)*rsxy1r2(i1,i2,i3,
     & m,n)+rsxy1(i1,i2,i3,1,0)*rsxy1s2(i1,i2,i3,m,n)
         rsxy1y22(i1,i2,i3,m,n)= rsxy1(i1,i2,i3,0,1)*rsxy1r2(i1,i2,i3,
     & m,n)+rsxy1(i1,i2,i3,1,1)*rsxy1s2(i1,i2,i3,m,n)
         rsxy1x23(i1,i2,i3,m,n)=rsxy1(i1,i2,i3,0,0)*rsxy1r2(i1,i2,i3,m,
     & n)+rsxy1(i1,i2,i3,1,0)*rsxy1s2(i1,i2,i3,m,n)+rsxy1(i1,i2,i3,2,
     & 0)*rsxy1t2(i1,i2,i3,m,n)
         rsxy1y23(i1,i2,i3,m,n)=rsxy1(i1,i2,i3,0,1)*rsxy1r2(i1,i2,i3,m,
     & n)+rsxy1(i1,i2,i3,1,1)*rsxy1s2(i1,i2,i3,m,n)+rsxy1(i1,i2,i3,2,
     & 1)*rsxy1t2(i1,i2,i3,m,n)
         rsxy1z23(i1,i2,i3,m,n)=rsxy1(i1,i2,i3,0,2)*rsxy1r2(i1,i2,i3,m,
     & n)+rsxy1(i1,i2,i3,1,2)*rsxy1s2(i1,i2,i3,m,n)+rsxy1(i1,i2,i3,2,
     & 2)*rsxy1t2(i1,i2,i3,m,n)
         udfxx21(i1,i2,i3,kd)=(rsxy1(i1,i2,i3,0,0)**2)*udfrr2(i1,i2,i3,
     & kd)+(rsxy1x22(i1,i2,i3,0,0))*udfr2(i1,i2,i3,kd)
         udfyy21(i1,i2,i3,kd)=0
         udfxy21(i1,i2,i3,kd)=0
         udfxz21(i1,i2,i3,kd)=0
         udfyz21(i1,i2,i3,kd)=0
         udfzz21(i1,i2,i3,kd)=0
         udflaplacian21(i1,i2,i3,kd)=udfxx21(i1,i2,i3,kd)
         udfxx22(i1,i2,i3,kd)=(rsxy1(i1,i2,i3,0,0)**2)*udfrr2(i1,i2,i3,
     & kd)+2.*(rsxy1(i1,i2,i3,0,0)*rsxy1(i1,i2,i3,1,0))*udfrs2(i1,i2,
     & i3,kd)+(rsxy1(i1,i2,i3,1,0)**2)*udfss2(i1,i2,i3,kd)+(rsxy1x22(
     & i1,i2,i3,0,0))*udfr2(i1,i2,i3,kd)+(rsxy1x22(i1,i2,i3,1,0))*
     & udfs2(i1,i2,i3,kd)
         udfyy22(i1,i2,i3,kd)=(rsxy1(i1,i2,i3,0,1)**2)*udfrr2(i1,i2,i3,
     & kd)+2.*(rsxy1(i1,i2,i3,0,1)*rsxy1(i1,i2,i3,1,1))*udfrs2(i1,i2,
     & i3,kd)+(rsxy1(i1,i2,i3,1,1)**2)*udfss2(i1,i2,i3,kd)+(rsxy1y22(
     & i1,i2,i3,0,1))*udfr2(i1,i2,i3,kd)+(rsxy1y22(i1,i2,i3,1,1))*
     & udfs2(i1,i2,i3,kd)
         udfxy22(i1,i2,i3,kd)=rsxy1(i1,i2,i3,0,0)*rsxy1(i1,i2,i3,0,1)*
     & udfrr2(i1,i2,i3,kd)+(rsxy1(i1,i2,i3,0,0)*rsxy1(i1,i2,i3,1,1)+
     & rsxy1(i1,i2,i3,0,1)*rsxy1(i1,i2,i3,1,0))*udfrs2(i1,i2,i3,kd)+
     & rsxy1(i1,i2,i3,1,0)*rsxy1(i1,i2,i3,1,1)*udfss2(i1,i2,i3,kd)+
     & rsxy1x22(i1,i2,i3,0,1)*udfr2(i1,i2,i3,kd)+rsxy1x22(i1,i2,i3,1,
     & 1)*udfs2(i1,i2,i3,kd)
         udfxz22(i1,i2,i3,kd)=0
         udfyz22(i1,i2,i3,kd)=0
         udfzz22(i1,i2,i3,kd)=0
         udflaplacian22(i1,i2,i3,kd)=(rsxy1(i1,i2,i3,0,0)**2+rsxy1(i1,
     & i2,i3,0,1)**2)*udfrr2(i1,i2,i3,kd)+2.*(rsxy1(i1,i2,i3,0,0)*
     & rsxy1(i1,i2,i3,1,0)+ rsxy1(i1,i2,i3,0,1)*rsxy1(i1,i2,i3,1,1))*
     & udfrs2(i1,i2,i3,kd)+(rsxy1(i1,i2,i3,1,0)**2+rsxy1(i1,i2,i3,1,1)
     & **2)*udfss2(i1,i2,i3,kd)+(rsxy1x22(i1,i2,i3,0,0)+rsxy1y22(i1,
     & i2,i3,0,1))*udfr2(i1,i2,i3,kd)+(rsxy1x22(i1,i2,i3,1,0)+
     & rsxy1y22(i1,i2,i3,1,1))*udfs2(i1,i2,i3,kd)
c ..... start: 3rd and 4th derivatives, 2D ....
         rsxy1xx22(i1,i2,i3,m,n)=(rsxy1(i1,i2,i3,0,0)**2)*rsxy1rr2(i1,
     & i2,i3,m,n)+2.*(rsxy1(i1,i2,i3,0,0)*rsxy1(i1,i2,i3,1,0))*
     & rsxy1rs2(i1,i2,i3,m,n)+(rsxy1(i1,i2,i3,1,0)**2)*rsxy1ss2(i1,i2,
     & i3,m,n)+(rsxy1x22(i1,i2,i3,0,0))*rsxy1r2(i1,i2,i3,m,n)+(
     & rsxy1x22(i1,i2,i3,1,0))*rsxy1s2(i1,i2,i3,m,n)
         rsxy1yy22(i1,i2,i3,m,n)=(rsxy1(i1,i2,i3,0,1)**2)*rsxy1rr2(i1,
     & i2,i3,m,n)+2.*(rsxy1(i1,i2,i3,0,1)*rsxy1(i1,i2,i3,1,1))*
     & rsxy1rs2(i1,i2,i3,m,n)+(rsxy1(i1,i2,i3,1,1)**2)*rsxy1ss2(i1,i2,
     & i3,m,n)+(rsxy1y22(i1,i2,i3,0,1))*rsxy1r2(i1,i2,i3,m,n)+(
     & rsxy1y22(i1,i2,i3,1,1))*rsxy1s2(i1,i2,i3,m,n)
         rsxy1xy22(i1,i2,i3,m,n)=rsxy1(i1,i2,i3,0,0)*rsxy1(i1,i2,i3,0,
     & 1)*rsxy1rr2(i1,i2,i3,m,n)+(rsxy1(i1,i2,i3,0,0)*rsxy1(i1,i2,i3,
     & 1,1)+rsxy1(i1,i2,i3,0,1)*rsxy1(i1,i2,i3,1,0))*rsxy1rs2(i1,i2,
     & i3,m,n)+rsxy1(i1,i2,i3,1,0)*rsxy1(i1,i2,i3,1,1)*rsxy1ss2(i1,i2,
     & i3,m,n)+rsxy1x22(i1,i2,i3,0,1)*rsxy1r2(i1,i2,i3,m,n)+rsxy1x22(
     & i1,i2,i3,1,1)*rsxy1s2(i1,i2,i3,m,n)
         rsxy1xxx22(i1,i2,i3,m,n)=rsxy1xx22(i1,i2,i3,0,0)*rsxy1r2(i1,
     & i2,i3,m,n)+rsxy1xx22(i1,i2,i3,1,0)*rsxy1s2(i1,i2,i3,m,n)+
     & rsxy1x22(i1,i2,i3,0,0)*(rsxy1(i1,i2,i3,0,0)*rsxy1rr2(i1,i2,i3,
     & m,n)+rsxy1(i1,i2,i3,1,0)*rsxy1rs2(i1,i2,i3,m,n))+rsxy1(i1,i2,
     & i3,0,0)*(rsxy1x22(i1,i2,i3,0,0)*rsxy1rr2(i1,i2,i3,m,n)+
     & rsxy1x22(i1,i2,i3,1,0)*rsxy1rs2(i1,i2,i3,m,n))+rsxy1x22(i1,i2,
     & i3,1,0)*(rsxy1(i1,i2,i3,0,0)*rsxy1rs2(i1,i2,i3,m,n)+rsxy1(i1,
     & i2,i3,1,0)*rsxy1ss2(i1,i2,i3,m,n))+rsxy1(i1,i2,i3,1,0)*(
     & rsxy1x22(i1,i2,i3,0,0)*rsxy1rs2(i1,i2,i3,m,n)+rsxy1x22(i1,i2,
     & i3,1,0)*rsxy1ss2(i1,i2,i3,m,n))+rsxy1(i1,i2,i3,0,0)*(rsxy1x22(
     & i1,i2,i3,0,0)*rsxy1rr2(i1,i2,i3,m,n)+rsxy1x22(i1,i2,i3,1,0)*
     & rsxy1rs2(i1,i2,i3,m,n)+rsxy1(i1,i2,i3,0,0)*(rsxy1(i1,i2,i3,0,0)
     & *rsxy1rrr2(i1,i2,i3,m,n)+rsxy1(i1,i2,i3,1,0)*rsxy1rrs2(i1,i2,
     & i3,m,n))+rsxy1(i1,i2,i3,1,0)*(rsxy1(i1,i2,i3,0,0)*rsxy1rrs2(i1,
     & i2,i3,m,n)+rsxy1(i1,i2,i3,1,0)*rsxy1rss2(i1,i2,i3,m,n)))+rsxy1(
     & i1,i2,i3,1,0)*(rsxy1x22(i1,i2,i3,0,0)*rsxy1rs2(i1,i2,i3,m,n)+
     & rsxy1x22(i1,i2,i3,1,0)*rsxy1ss2(i1,i2,i3,m,n)+rsxy1(i1,i2,i3,0,
     & 0)*(rsxy1(i1,i2,i3,0,0)*rsxy1rrs2(i1,i2,i3,m,n)+rsxy1(i1,i2,i3,
     & 1,0)*rsxy1rss2(i1,i2,i3,m,n))+rsxy1(i1,i2,i3,1,0)*(rsxy1(i1,i2,
     & i3,0,0)*rsxy1rss2(i1,i2,i3,m,n)+rsxy1(i1,i2,i3,1,0)*rsxy1sss2(
     & i1,i2,i3,m,n)))
         rsxy1xxy22(i1,i2,i3,m,n)=rsxy1xy22(i1,i2,i3,0,0)*rsxy1r2(i1,
     & i2,i3,m,n)+rsxy1xy22(i1,i2,i3,1,0)*rsxy1s2(i1,i2,i3,m,n)+
     & rsxy1x22(i1,i2,i3,0,1)*(rsxy1(i1,i2,i3,0,0)*rsxy1rr2(i1,i2,i3,
     & m,n)+rsxy1(i1,i2,i3,1,0)*rsxy1rs2(i1,i2,i3,m,n))+rsxy1(i1,i2,
     & i3,0,0)*(rsxy1x22(i1,i2,i3,0,1)*rsxy1rr2(i1,i2,i3,m,n)+
     & rsxy1x22(i1,i2,i3,1,1)*rsxy1rs2(i1,i2,i3,m,n))+rsxy1x22(i1,i2,
     & i3,1,1)*(rsxy1(i1,i2,i3,0,0)*rsxy1rs2(i1,i2,i3,m,n)+rsxy1(i1,
     & i2,i3,1,0)*rsxy1ss2(i1,i2,i3,m,n))+rsxy1(i1,i2,i3,1,0)*(
     & rsxy1x22(i1,i2,i3,0,1)*rsxy1rs2(i1,i2,i3,m,n)+rsxy1x22(i1,i2,
     & i3,1,1)*rsxy1ss2(i1,i2,i3,m,n))+rsxy1(i1,i2,i3,0,1)*(rsxy1x22(
     & i1,i2,i3,0,0)*rsxy1rr2(i1,i2,i3,m,n)+rsxy1x22(i1,i2,i3,1,0)*
     & rsxy1rs2(i1,i2,i3,m,n)+rsxy1(i1,i2,i3,0,0)*(rsxy1(i1,i2,i3,0,0)
     & *rsxy1rrr2(i1,i2,i3,m,n)+rsxy1(i1,i2,i3,1,0)*rsxy1rrs2(i1,i2,
     & i3,m,n))+rsxy1(i1,i2,i3,1,0)*(rsxy1(i1,i2,i3,0,0)*rsxy1rrs2(i1,
     & i2,i3,m,n)+rsxy1(i1,i2,i3,1,0)*rsxy1rss2(i1,i2,i3,m,n)))+rsxy1(
     & i1,i2,i3,1,1)*(rsxy1x22(i1,i2,i3,0,0)*rsxy1rs2(i1,i2,i3,m,n)+
     & rsxy1x22(i1,i2,i3,1,0)*rsxy1ss2(i1,i2,i3,m,n)+rsxy1(i1,i2,i3,0,
     & 0)*(rsxy1(i1,i2,i3,0,0)*rsxy1rrs2(i1,i2,i3,m,n)+rsxy1(i1,i2,i3,
     & 1,0)*rsxy1rss2(i1,i2,i3,m,n))+rsxy1(i1,i2,i3,1,0)*(rsxy1(i1,i2,
     & i3,0,0)*rsxy1rss2(i1,i2,i3,m,n)+rsxy1(i1,i2,i3,1,0)*rsxy1sss2(
     & i1,i2,i3,m,n)))
         rsxy1xyy22(i1,i2,i3,m,n)=rsxy1yy22(i1,i2,i3,0,0)*rsxy1r2(i1,
     & i2,i3,m,n)+rsxy1yy22(i1,i2,i3,1,0)*rsxy1s2(i1,i2,i3,m,n)+
     & rsxy1y22(i1,i2,i3,0,1)*(rsxy1(i1,i2,i3,0,0)*rsxy1rr2(i1,i2,i3,
     & m,n)+rsxy1(i1,i2,i3,1,0)*rsxy1rs2(i1,i2,i3,m,n))+rsxy1(i1,i2,
     & i3,0,1)*(rsxy1x22(i1,i2,i3,0,1)*rsxy1rr2(i1,i2,i3,m,n)+
     & rsxy1x22(i1,i2,i3,1,1)*rsxy1rs2(i1,i2,i3,m,n))+rsxy1y22(i1,i2,
     & i3,1,1)*(rsxy1(i1,i2,i3,0,0)*rsxy1rs2(i1,i2,i3,m,n)+rsxy1(i1,
     & i2,i3,1,0)*rsxy1ss2(i1,i2,i3,m,n))+rsxy1(i1,i2,i3,1,1)*(
     & rsxy1x22(i1,i2,i3,0,1)*rsxy1rs2(i1,i2,i3,m,n)+rsxy1x22(i1,i2,
     & i3,1,1)*rsxy1ss2(i1,i2,i3,m,n))+rsxy1(i1,i2,i3,0,1)*(rsxy1x22(
     & i1,i2,i3,0,1)*rsxy1rr2(i1,i2,i3,m,n)+rsxy1x22(i1,i2,i3,1,1)*
     & rsxy1rs2(i1,i2,i3,m,n)+rsxy1(i1,i2,i3,0,1)*(rsxy1(i1,i2,i3,0,0)
     & *rsxy1rrr2(i1,i2,i3,m,n)+rsxy1(i1,i2,i3,1,0)*rsxy1rrs2(i1,i2,
     & i3,m,n))+rsxy1(i1,i2,i3,1,1)*(rsxy1(i1,i2,i3,0,0)*rsxy1rrs2(i1,
     & i2,i3,m,n)+rsxy1(i1,i2,i3,1,0)*rsxy1rss2(i1,i2,i3,m,n)))+rsxy1(
     & i1,i2,i3,1,1)*(rsxy1x22(i1,i2,i3,0,1)*rsxy1rs2(i1,i2,i3,m,n)+
     & rsxy1x22(i1,i2,i3,1,1)*rsxy1ss2(i1,i2,i3,m,n)+rsxy1(i1,i2,i3,0,
     & 1)*(rsxy1(i1,i2,i3,0,0)*rsxy1rrs2(i1,i2,i3,m,n)+rsxy1(i1,i2,i3,
     & 1,0)*rsxy1rss2(i1,i2,i3,m,n))+rsxy1(i1,i2,i3,1,1)*(rsxy1(i1,i2,
     & i3,0,0)*rsxy1rss2(i1,i2,i3,m,n)+rsxy1(i1,i2,i3,1,0)*rsxy1sss2(
     & i1,i2,i3,m,n)))
         rsxy1yyy22(i1,i2,i3,m,n)=rsxy1yy22(i1,i2,i3,0,1)*rsxy1r2(i1,
     & i2,i3,m,n)+rsxy1yy22(i1,i2,i3,1,1)*rsxy1s2(i1,i2,i3,m,n)+
     & rsxy1y22(i1,i2,i3,0,1)*(rsxy1(i1,i2,i3,0,1)*rsxy1rr2(i1,i2,i3,
     & m,n)+rsxy1(i1,i2,i3,1,1)*rsxy1rs2(i1,i2,i3,m,n))+rsxy1(i1,i2,
     & i3,0,1)*(rsxy1y22(i1,i2,i3,0,1)*rsxy1rr2(i1,i2,i3,m,n)+
     & rsxy1y22(i1,i2,i3,1,1)*rsxy1rs2(i1,i2,i3,m,n))+rsxy1y22(i1,i2,
     & i3,1,1)*(rsxy1(i1,i2,i3,0,1)*rsxy1rs2(i1,i2,i3,m,n)+rsxy1(i1,
     & i2,i3,1,1)*rsxy1ss2(i1,i2,i3,m,n))+rsxy1(i1,i2,i3,1,1)*(
     & rsxy1y22(i1,i2,i3,0,1)*rsxy1rs2(i1,i2,i3,m,n)+rsxy1y22(i1,i2,
     & i3,1,1)*rsxy1ss2(i1,i2,i3,m,n))+rsxy1(i1,i2,i3,0,1)*(rsxy1y22(
     & i1,i2,i3,0,1)*rsxy1rr2(i1,i2,i3,m,n)+rsxy1y22(i1,i2,i3,1,1)*
     & rsxy1rs2(i1,i2,i3,m,n)+rsxy1(i1,i2,i3,0,1)*(rsxy1(i1,i2,i3,0,1)
     & *rsxy1rrr2(i1,i2,i3,m,n)+rsxy1(i1,i2,i3,1,1)*rsxy1rrs2(i1,i2,
     & i3,m,n))+rsxy1(i1,i2,i3,1,1)*(rsxy1(i1,i2,i3,0,1)*rsxy1rrs2(i1,
     & i2,i3,m,n)+rsxy1(i1,i2,i3,1,1)*rsxy1rss2(i1,i2,i3,m,n)))+rsxy1(
     & i1,i2,i3,1,1)*(rsxy1y22(i1,i2,i3,0,1)*rsxy1rs2(i1,i2,i3,m,n)+
     & rsxy1y22(i1,i2,i3,1,1)*rsxy1ss2(i1,i2,i3,m,n)+rsxy1(i1,i2,i3,0,
     & 1)*(rsxy1(i1,i2,i3,0,1)*rsxy1rrs2(i1,i2,i3,m,n)+rsxy1(i1,i2,i3,
     & 1,1)*rsxy1rss2(i1,i2,i3,m,n))+rsxy1(i1,i2,i3,1,1)*(rsxy1(i1,i2,
     & i3,0,1)*rsxy1rss2(i1,i2,i3,m,n)+rsxy1(i1,i2,i3,1,1)*rsxy1sss2(
     & i1,i2,i3,m,n)))
         udfxxx22(i1,i2,i3,kd)=rsxy1xx22(i1,i2,i3,0,0)*udfr2(i1,i2,i3,
     & kd)+rsxy1xx22(i1,i2,i3,1,0)*udfs2(i1,i2,i3,kd)+rsxy1x22(i1,i2,
     & i3,0,0)*(rsxy1(i1,i2,i3,0,0)*udfrr2(i1,i2,i3,kd)+rsxy1(i1,i2,
     & i3,1,0)*udfrs2(i1,i2,i3,kd))+rsxy1(i1,i2,i3,0,0)*(rsxy1x22(i1,
     & i2,i3,0,0)*udfrr2(i1,i2,i3,kd)+rsxy1x22(i1,i2,i3,1,0)*udfrs2(
     & i1,i2,i3,kd))+rsxy1x22(i1,i2,i3,1,0)*(rsxy1(i1,i2,i3,0,0)*
     & udfrs2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*udfss2(i1,i2,i3,kd))+
     & rsxy1(i1,i2,i3,1,0)*(rsxy1x22(i1,i2,i3,0,0)*udfrs2(i1,i2,i3,kd)
     & +rsxy1x22(i1,i2,i3,1,0)*udfss2(i1,i2,i3,kd))+rsxy1(i1,i2,i3,0,
     & 0)*(rsxy1x22(i1,i2,i3,0,0)*udfrr2(i1,i2,i3,kd)+rsxy1x22(i1,i2,
     & i3,1,0)*udfrs2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,0,0)*(rsxy1(i1,i2,
     & i3,0,0)*udfrrr2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*udfrrs2(i1,i2,
     & i3,kd))+rsxy1(i1,i2,i3,1,0)*(rsxy1(i1,i2,i3,0,0)*udfrrs2(i1,i2,
     & i3,kd)+rsxy1(i1,i2,i3,1,0)*udfrss2(i1,i2,i3,kd)))+rsxy1(i1,i2,
     & i3,1,0)*(rsxy1x22(i1,i2,i3,0,0)*udfrs2(i1,i2,i3,kd)+rsxy1x22(
     & i1,i2,i3,1,0)*udfss2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,0,0)*(rsxy1(
     & i1,i2,i3,0,0)*udfrrs2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*udfrss2(
     & i1,i2,i3,kd))+rsxy1(i1,i2,i3,1,0)*(rsxy1(i1,i2,i3,0,0)*udfrss2(
     & i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*udfsss2(i1,i2,i3,kd)))
         udfxxy22(i1,i2,i3,kd)=rsxy1xy22(i1,i2,i3,0,0)*udfr2(i1,i2,i3,
     & kd)+rsxy1xy22(i1,i2,i3,1,0)*udfs2(i1,i2,i3,kd)+rsxy1x22(i1,i2,
     & i3,0,1)*(rsxy1(i1,i2,i3,0,0)*udfrr2(i1,i2,i3,kd)+rsxy1(i1,i2,
     & i3,1,0)*udfrs2(i1,i2,i3,kd))+rsxy1(i1,i2,i3,0,0)*(rsxy1x22(i1,
     & i2,i3,0,1)*udfrr2(i1,i2,i3,kd)+rsxy1x22(i1,i2,i3,1,1)*udfrs2(
     & i1,i2,i3,kd))+rsxy1x22(i1,i2,i3,1,1)*(rsxy1(i1,i2,i3,0,0)*
     & udfrs2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*udfss2(i1,i2,i3,kd))+
     & rsxy1(i1,i2,i3,1,0)*(rsxy1x22(i1,i2,i3,0,1)*udfrs2(i1,i2,i3,kd)
     & +rsxy1x22(i1,i2,i3,1,1)*udfss2(i1,i2,i3,kd))+rsxy1(i1,i2,i3,0,
     & 1)*(rsxy1x22(i1,i2,i3,0,0)*udfrr2(i1,i2,i3,kd)+rsxy1x22(i1,i2,
     & i3,1,0)*udfrs2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,0,0)*(rsxy1(i1,i2,
     & i3,0,0)*udfrrr2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*udfrrs2(i1,i2,
     & i3,kd))+rsxy1(i1,i2,i3,1,0)*(rsxy1(i1,i2,i3,0,0)*udfrrs2(i1,i2,
     & i3,kd)+rsxy1(i1,i2,i3,1,0)*udfrss2(i1,i2,i3,kd)))+rsxy1(i1,i2,
     & i3,1,1)*(rsxy1x22(i1,i2,i3,0,0)*udfrs2(i1,i2,i3,kd)+rsxy1x22(
     & i1,i2,i3,1,0)*udfss2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,0,0)*(rsxy1(
     & i1,i2,i3,0,0)*udfrrs2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*udfrss2(
     & i1,i2,i3,kd))+rsxy1(i1,i2,i3,1,0)*(rsxy1(i1,i2,i3,0,0)*udfrss2(
     & i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*udfsss2(i1,i2,i3,kd)))
         udfxyy22(i1,i2,i3,kd)=rsxy1yy22(i1,i2,i3,0,0)*udfr2(i1,i2,i3,
     & kd)+rsxy1yy22(i1,i2,i3,1,0)*udfs2(i1,i2,i3,kd)+rsxy1y22(i1,i2,
     & i3,0,1)*(rsxy1(i1,i2,i3,0,0)*udfrr2(i1,i2,i3,kd)+rsxy1(i1,i2,
     & i3,1,0)*udfrs2(i1,i2,i3,kd))+rsxy1(i1,i2,i3,0,1)*(rsxy1x22(i1,
     & i2,i3,0,1)*udfrr2(i1,i2,i3,kd)+rsxy1x22(i1,i2,i3,1,1)*udfrs2(
     & i1,i2,i3,kd))+rsxy1y22(i1,i2,i3,1,1)*(rsxy1(i1,i2,i3,0,0)*
     & udfrs2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*udfss2(i1,i2,i3,kd))+
     & rsxy1(i1,i2,i3,1,1)*(rsxy1x22(i1,i2,i3,0,1)*udfrs2(i1,i2,i3,kd)
     & +rsxy1x22(i1,i2,i3,1,1)*udfss2(i1,i2,i3,kd))+rsxy1(i1,i2,i3,0,
     & 1)*(rsxy1x22(i1,i2,i3,0,1)*udfrr2(i1,i2,i3,kd)+rsxy1x22(i1,i2,
     & i3,1,1)*udfrs2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,0,1)*(rsxy1(i1,i2,
     & i3,0,0)*udfrrr2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*udfrrs2(i1,i2,
     & i3,kd))+rsxy1(i1,i2,i3,1,1)*(rsxy1(i1,i2,i3,0,0)*udfrrs2(i1,i2,
     & i3,kd)+rsxy1(i1,i2,i3,1,0)*udfrss2(i1,i2,i3,kd)))+rsxy1(i1,i2,
     & i3,1,1)*(rsxy1x22(i1,i2,i3,0,1)*udfrs2(i1,i2,i3,kd)+rsxy1x22(
     & i1,i2,i3,1,1)*udfss2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,0,1)*(rsxy1(
     & i1,i2,i3,0,0)*udfrrs2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*udfrss2(
     & i1,i2,i3,kd))+rsxy1(i1,i2,i3,1,1)*(rsxy1(i1,i2,i3,0,0)*udfrss2(
     & i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*udfsss2(i1,i2,i3,kd)))
         udfyyy22(i1,i2,i3,kd)=rsxy1yy22(i1,i2,i3,0,1)*udfr2(i1,i2,i3,
     & kd)+rsxy1yy22(i1,i2,i3,1,1)*udfs2(i1,i2,i3,kd)+rsxy1y22(i1,i2,
     & i3,0,1)*(rsxy1(i1,i2,i3,0,1)*udfrr2(i1,i2,i3,kd)+rsxy1(i1,i2,
     & i3,1,1)*udfrs2(i1,i2,i3,kd))+rsxy1(i1,i2,i3,0,1)*(rsxy1y22(i1,
     & i2,i3,0,1)*udfrr2(i1,i2,i3,kd)+rsxy1y22(i1,i2,i3,1,1)*udfrs2(
     & i1,i2,i3,kd))+rsxy1y22(i1,i2,i3,1,1)*(rsxy1(i1,i2,i3,0,1)*
     & udfrs2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,1)*udfss2(i1,i2,i3,kd))+
     & rsxy1(i1,i2,i3,1,1)*(rsxy1y22(i1,i2,i3,0,1)*udfrs2(i1,i2,i3,kd)
     & +rsxy1y22(i1,i2,i3,1,1)*udfss2(i1,i2,i3,kd))+rsxy1(i1,i2,i3,0,
     & 1)*(rsxy1y22(i1,i2,i3,0,1)*udfrr2(i1,i2,i3,kd)+rsxy1y22(i1,i2,
     & i3,1,1)*udfrs2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,0,1)*(rsxy1(i1,i2,
     & i3,0,1)*udfrrr2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,1)*udfrrs2(i1,i2,
     & i3,kd))+rsxy1(i1,i2,i3,1,1)*(rsxy1(i1,i2,i3,0,1)*udfrrs2(i1,i2,
     & i3,kd)+rsxy1(i1,i2,i3,1,1)*udfrss2(i1,i2,i3,kd)))+rsxy1(i1,i2,
     & i3,1,1)*(rsxy1y22(i1,i2,i3,0,1)*udfrs2(i1,i2,i3,kd)+rsxy1y22(
     & i1,i2,i3,1,1)*udfss2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,0,1)*(rsxy1(
     & i1,i2,i3,0,1)*udfrrs2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,1)*udfrss2(
     & i1,i2,i3,kd))+rsxy1(i1,i2,i3,1,1)*(rsxy1(i1,i2,i3,0,1)*udfrss2(
     & i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,1)*udfsss2(i1,i2,i3,kd)))
         udfxxxx22(i1,i2,i3,kd)=rsxy1xxx22(i1,i2,i3,0,0)*udfr2(i1,i2,
     & i3,kd)+rsxy1xxx22(i1,i2,i3,1,0)*udfs2(i1,i2,i3,kd)+rsxy1xx22(
     & i1,i2,i3,0,0)*(rsxy1(i1,i2,i3,0,0)*udfrr2(i1,i2,i3,kd)+rsxy1(
     & i1,i2,i3,1,0)*udfrs2(i1,i2,i3,kd))+2*rsxy1x22(i1,i2,i3,0,0)*(
     & rsxy1x22(i1,i2,i3,0,0)*udfrr2(i1,i2,i3,kd)+rsxy1x22(i1,i2,i3,1,
     & 0)*udfrs2(i1,i2,i3,kd))+rsxy1(i1,i2,i3,0,0)*(rsxy1xx22(i1,i2,
     & i3,0,0)*udfrr2(i1,i2,i3,kd)+rsxy1xx22(i1,i2,i3,1,0)*udfrs2(i1,
     & i2,i3,kd))+rsxy1xx22(i1,i2,i3,1,0)*(rsxy1(i1,i2,i3,0,0)*udfrs2(
     & i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*udfss2(i1,i2,i3,kd))+2*
     & rsxy1x22(i1,i2,i3,1,0)*(rsxy1x22(i1,i2,i3,0,0)*udfrs2(i1,i2,i3,
     & kd)+rsxy1x22(i1,i2,i3,1,0)*udfss2(i1,i2,i3,kd))+rsxy1(i1,i2,i3,
     & 1,0)*(rsxy1xx22(i1,i2,i3,0,0)*udfrs2(i1,i2,i3,kd)+rsxy1xx22(i1,
     & i2,i3,1,0)*udfss2(i1,i2,i3,kd))+rsxy1x22(i1,i2,i3,0,0)*(
     & rsxy1x22(i1,i2,i3,0,0)*udfrr2(i1,i2,i3,kd)+rsxy1x22(i1,i2,i3,1,
     & 0)*udfrs2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,0,0)*(rsxy1(i1,i2,i3,0,0)
     & *udfrrr2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*udfrrs2(i1,i2,i3,kd))
     & +rsxy1(i1,i2,i3,1,0)*(rsxy1(i1,i2,i3,0,0)*udfrrs2(i1,i2,i3,kd)+
     & rsxy1(i1,i2,i3,1,0)*udfrss2(i1,i2,i3,kd)))+rsxy1(i1,i2,i3,0,0)*
     & (rsxy1xx22(i1,i2,i3,0,0)*udfrr2(i1,i2,i3,kd)+rsxy1xx22(i1,i2,
     & i3,1,0)*udfrs2(i1,i2,i3,kd)+rsxy1x22(i1,i2,i3,0,0)*(rsxy1(i1,
     & i2,i3,0,0)*udfrrr2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*udfrrs2(i1,
     & i2,i3,kd))+rsxy1(i1,i2,i3,0,0)*(rsxy1x22(i1,i2,i3,0,0)*udfrrr2(
     & i1,i2,i3,kd)+rsxy1x22(i1,i2,i3,1,0)*udfrrs2(i1,i2,i3,kd))+
     & rsxy1x22(i1,i2,i3,1,0)*(rsxy1(i1,i2,i3,0,0)*udfrrs2(i1,i2,i3,
     & kd)+rsxy1(i1,i2,i3,1,0)*udfrss2(i1,i2,i3,kd))+rsxy1(i1,i2,i3,1,
     & 0)*(rsxy1x22(i1,i2,i3,0,0)*udfrrs2(i1,i2,i3,kd)+rsxy1x22(i1,i2,
     & i3,1,0)*udfrss2(i1,i2,i3,kd)))+rsxy1x22(i1,i2,i3,1,0)*(
     & rsxy1x22(i1,i2,i3,0,0)*udfrs2(i1,i2,i3,kd)+rsxy1x22(i1,i2,i3,1,
     & 0)*udfss2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,0,0)*(rsxy1(i1,i2,i3,0,0)
     & *udfrrs2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*udfrss2(i1,i2,i3,kd))
     & +rsxy1(i1,i2,i3,1,0)*(rsxy1(i1,i2,i3,0,0)*udfrss2(i1,i2,i3,kd)+
     & rsxy1(i1,i2,i3,1,0)*udfsss2(i1,i2,i3,kd)))+rsxy1(i1,i2,i3,1,0)*
     & (rsxy1xx22(i1,i2,i3,0,0)*udfrs2(i1,i2,i3,kd)+rsxy1xx22(i1,i2,
     & i3,1,0)*udfss2(i1,i2,i3,kd)+rsxy1x22(i1,i2,i3,0,0)*(rsxy1(i1,
     & i2,i3,0,0)*udfrrs2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*udfrss2(i1,
     & i2,i3,kd))+rsxy1(i1,i2,i3,0,0)*(rsxy1x22(i1,i2,i3,0,0)*udfrrs2(
     & i1,i2,i3,kd)+rsxy1x22(i1,i2,i3,1,0)*udfrss2(i1,i2,i3,kd))+
     & rsxy1x22(i1,i2,i3,1,0)*(rsxy1(i1,i2,i3,0,0)*udfrss2(i1,i2,i3,
     & kd)+rsxy1(i1,i2,i3,1,0)*udfsss2(i1,i2,i3,kd))+rsxy1(i1,i2,i3,1,
     & 0)*(rsxy1x22(i1,i2,i3,0,0)*udfrss2(i1,i2,i3,kd)+rsxy1x22(i1,i2,
     & i3,1,0)*udfsss2(i1,i2,i3,kd)))+rsxy1(i1,i2,i3,0,0)*(rsxy1xx22(
     & i1,i2,i3,0,0)*udfrr2(i1,i2,i3,kd)+rsxy1xx22(i1,i2,i3,1,0)*
     & udfrs2(i1,i2,i3,kd)+rsxy1x22(i1,i2,i3,0,0)*(rsxy1(i1,i2,i3,0,0)
     & *udfrrr2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*udfrrs2(i1,i2,i3,kd))
     & +rsxy1(i1,i2,i3,0,0)*(rsxy1x22(i1,i2,i3,0,0)*udfrrr2(i1,i2,i3,
     & kd)+rsxy1x22(i1,i2,i3,1,0)*udfrrs2(i1,i2,i3,kd))+rsxy1x22(i1,
     & i2,i3,1,0)*(rsxy1(i1,i2,i3,0,0)*udfrrs2(i1,i2,i3,kd)+rsxy1(i1,
     & i2,i3,1,0)*udfrss2(i1,i2,i3,kd))+rsxy1(i1,i2,i3,1,0)*(rsxy1x22(
     & i1,i2,i3,0,0)*udfrrs2(i1,i2,i3,kd)+rsxy1x22(i1,i2,i3,1,0)*
     & udfrss2(i1,i2,i3,kd))+rsxy1(i1,i2,i3,0,0)*(rsxy1x22(i1,i2,i3,0,
     & 0)*udfrrr2(i1,i2,i3,kd)+rsxy1x22(i1,i2,i3,1,0)*udfrrs2(i1,i2,
     & i3,kd)+rsxy1(i1,i2,i3,0,0)*(rsxy1(i1,i2,i3,0,0)*udfrrrr2(i1,i2,
     & i3,kd)+rsxy1(i1,i2,i3,1,0)*udfrrrs2(i1,i2,i3,kd))+rsxy1(i1,i2,
     & i3,1,0)*(rsxy1(i1,i2,i3,0,0)*udfrrrs2(i1,i2,i3,kd)+rsxy1(i1,i2,
     & i3,1,0)*udfrrss2(i1,i2,i3,kd)))+rsxy1(i1,i2,i3,1,0)*(rsxy1x22(
     & i1,i2,i3,0,0)*udfrrs2(i1,i2,i3,kd)+rsxy1x22(i1,i2,i3,1,0)*
     & udfrss2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,0,0)*(rsxy1(i1,i2,i3,0,0)*
     & udfrrrs2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*udfrrss2(i1,i2,i3,kd)
     & )+rsxy1(i1,i2,i3,1,0)*(rsxy1(i1,i2,i3,0,0)*udfrrss2(i1,i2,i3,
     & kd)+rsxy1(i1,i2,i3,1,0)*udfrsss2(i1,i2,i3,kd))))+rsxy1(i1,i2,
     & i3,1,0)*(rsxy1xx22(i1,i2,i3,0,0)*udfrs2(i1,i2,i3,kd)+rsxy1xx22(
     & i1,i2,i3,1,0)*udfss2(i1,i2,i3,kd)+rsxy1x22(i1,i2,i3,0,0)*(
     & rsxy1(i1,i2,i3,0,0)*udfrrs2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*
     & udfrss2(i1,i2,i3,kd))+rsxy1(i1,i2,i3,0,0)*(rsxy1x22(i1,i2,i3,0,
     & 0)*udfrrs2(i1,i2,i3,kd)+rsxy1x22(i1,i2,i3,1,0)*udfrss2(i1,i2,
     & i3,kd))+rsxy1x22(i1,i2,i3,1,0)*(rsxy1(i1,i2,i3,0,0)*udfrss2(i1,
     & i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*udfsss2(i1,i2,i3,kd))+rsxy1(i1,
     & i2,i3,1,0)*(rsxy1x22(i1,i2,i3,0,0)*udfrss2(i1,i2,i3,kd)+
     & rsxy1x22(i1,i2,i3,1,0)*udfsss2(i1,i2,i3,kd))+rsxy1(i1,i2,i3,0,
     & 0)*(rsxy1x22(i1,i2,i3,0,0)*udfrrs2(i1,i2,i3,kd)+rsxy1x22(i1,i2,
     & i3,1,0)*udfrss2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,0,0)*(rsxy1(i1,i2,
     & i3,0,0)*udfrrrs2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*udfrrss2(i1,
     & i2,i3,kd))+rsxy1(i1,i2,i3,1,0)*(rsxy1(i1,i2,i3,0,0)*udfrrss2(
     & i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*udfrsss2(i1,i2,i3,kd)))+rsxy1(
     & i1,i2,i3,1,0)*(rsxy1x22(i1,i2,i3,0,0)*udfrss2(i1,i2,i3,kd)+
     & rsxy1x22(i1,i2,i3,1,0)*udfsss2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,0,0)
     & *(rsxy1(i1,i2,i3,0,0)*udfrrss2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)
     & *udfrsss2(i1,i2,i3,kd))+rsxy1(i1,i2,i3,1,0)*(rsxy1(i1,i2,i3,0,
     & 0)*udfrsss2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*udfssss2(i1,i2,i3,
     & kd))))
         udfxxxy22(i1,i2,i3,kd)=rsxy1xxy22(i1,i2,i3,0,0)*udfr2(i1,i2,
     & i3,kd)+rsxy1xxy22(i1,i2,i3,1,0)*udfs2(i1,i2,i3,kd)+rsxy1xy22(
     & i1,i2,i3,0,0)*(rsxy1(i1,i2,i3,0,0)*udfrr2(i1,i2,i3,kd)+rsxy1(
     & i1,i2,i3,1,0)*udfrs2(i1,i2,i3,kd))+rsxy1x22(i1,i2,i3,0,0)*(
     & rsxy1x22(i1,i2,i3,0,1)*udfrr2(i1,i2,i3,kd)+rsxy1x22(i1,i2,i3,1,
     & 1)*udfrs2(i1,i2,i3,kd))+rsxy1x22(i1,i2,i3,0,1)*(rsxy1x22(i1,i2,
     & i3,0,0)*udfrr2(i1,i2,i3,kd)+rsxy1x22(i1,i2,i3,1,0)*udfrs2(i1,
     & i2,i3,kd))+rsxy1(i1,i2,i3,0,0)*(rsxy1xy22(i1,i2,i3,0,0)*udfrr2(
     & i1,i2,i3,kd)+rsxy1xy22(i1,i2,i3,1,0)*udfrs2(i1,i2,i3,kd))+
     & rsxy1xy22(i1,i2,i3,1,0)*(rsxy1(i1,i2,i3,0,0)*udfrs2(i1,i2,i3,
     & kd)+rsxy1(i1,i2,i3,1,0)*udfss2(i1,i2,i3,kd))+rsxy1x22(i1,i2,i3,
     & 1,0)*(rsxy1x22(i1,i2,i3,0,1)*udfrs2(i1,i2,i3,kd)+rsxy1x22(i1,
     & i2,i3,1,1)*udfss2(i1,i2,i3,kd))+rsxy1x22(i1,i2,i3,1,1)*(
     & rsxy1x22(i1,i2,i3,0,0)*udfrs2(i1,i2,i3,kd)+rsxy1x22(i1,i2,i3,1,
     & 0)*udfss2(i1,i2,i3,kd))+rsxy1(i1,i2,i3,1,0)*(rsxy1xy22(i1,i2,
     & i3,0,0)*udfrs2(i1,i2,i3,kd)+rsxy1xy22(i1,i2,i3,1,0)*udfss2(i1,
     & i2,i3,kd))+rsxy1x22(i1,i2,i3,0,1)*(rsxy1x22(i1,i2,i3,0,0)*
     & udfrr2(i1,i2,i3,kd)+rsxy1x22(i1,i2,i3,1,0)*udfrs2(i1,i2,i3,kd)+
     & rsxy1(i1,i2,i3,0,0)*(rsxy1(i1,i2,i3,0,0)*udfrrr2(i1,i2,i3,kd)+
     & rsxy1(i1,i2,i3,1,0)*udfrrs2(i1,i2,i3,kd))+rsxy1(i1,i2,i3,1,0)*(
     & rsxy1(i1,i2,i3,0,0)*udfrrs2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*
     & udfrss2(i1,i2,i3,kd)))+rsxy1(i1,i2,i3,0,0)*(rsxy1xy22(i1,i2,i3,
     & 0,0)*udfrr2(i1,i2,i3,kd)+rsxy1xy22(i1,i2,i3,1,0)*udfrs2(i1,i2,
     & i3,kd)+rsxy1x22(i1,i2,i3,0,1)*(rsxy1(i1,i2,i3,0,0)*udfrrr2(i1,
     & i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*udfrrs2(i1,i2,i3,kd))+rsxy1(i1,
     & i2,i3,0,0)*(rsxy1x22(i1,i2,i3,0,1)*udfrrr2(i1,i2,i3,kd)+
     & rsxy1x22(i1,i2,i3,1,1)*udfrrs2(i1,i2,i3,kd))+rsxy1x22(i1,i2,i3,
     & 1,1)*(rsxy1(i1,i2,i3,0,0)*udfrrs2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,
     & 1,0)*udfrss2(i1,i2,i3,kd))+rsxy1(i1,i2,i3,1,0)*(rsxy1x22(i1,i2,
     & i3,0,1)*udfrrs2(i1,i2,i3,kd)+rsxy1x22(i1,i2,i3,1,1)*udfrss2(i1,
     & i2,i3,kd)))+rsxy1x22(i1,i2,i3,1,1)*(rsxy1x22(i1,i2,i3,0,0)*
     & udfrs2(i1,i2,i3,kd)+rsxy1x22(i1,i2,i3,1,0)*udfss2(i1,i2,i3,kd)+
     & rsxy1(i1,i2,i3,0,0)*(rsxy1(i1,i2,i3,0,0)*udfrrs2(i1,i2,i3,kd)+
     & rsxy1(i1,i2,i3,1,0)*udfrss2(i1,i2,i3,kd))+rsxy1(i1,i2,i3,1,0)*(
     & rsxy1(i1,i2,i3,0,0)*udfrss2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*
     & udfsss2(i1,i2,i3,kd)))+rsxy1(i1,i2,i3,1,0)*(rsxy1xy22(i1,i2,i3,
     & 0,0)*udfrs2(i1,i2,i3,kd)+rsxy1xy22(i1,i2,i3,1,0)*udfss2(i1,i2,
     & i3,kd)+rsxy1x22(i1,i2,i3,0,1)*(rsxy1(i1,i2,i3,0,0)*udfrrs2(i1,
     & i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*udfrss2(i1,i2,i3,kd))+rsxy1(i1,
     & i2,i3,0,0)*(rsxy1x22(i1,i2,i3,0,1)*udfrrs2(i1,i2,i3,kd)+
     & rsxy1x22(i1,i2,i3,1,1)*udfrss2(i1,i2,i3,kd))+rsxy1x22(i1,i2,i3,
     & 1,1)*(rsxy1(i1,i2,i3,0,0)*udfrss2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,
     & 1,0)*udfsss2(i1,i2,i3,kd))+rsxy1(i1,i2,i3,1,0)*(rsxy1x22(i1,i2,
     & i3,0,1)*udfrss2(i1,i2,i3,kd)+rsxy1x22(i1,i2,i3,1,1)*udfsss2(i1,
     & i2,i3,kd)))+rsxy1(i1,i2,i3,0,1)*(rsxy1xx22(i1,i2,i3,0,0)*
     & udfrr2(i1,i2,i3,kd)+rsxy1xx22(i1,i2,i3,1,0)*udfrs2(i1,i2,i3,kd)
     & +rsxy1x22(i1,i2,i3,0,0)*(rsxy1(i1,i2,i3,0,0)*udfrrr2(i1,i2,i3,
     & kd)+rsxy1(i1,i2,i3,1,0)*udfrrs2(i1,i2,i3,kd))+rsxy1(i1,i2,i3,0,
     & 0)*(rsxy1x22(i1,i2,i3,0,0)*udfrrr2(i1,i2,i3,kd)+rsxy1x22(i1,i2,
     & i3,1,0)*udfrrs2(i1,i2,i3,kd))+rsxy1x22(i1,i2,i3,1,0)*(rsxy1(i1,
     & i2,i3,0,0)*udfrrs2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*udfrss2(i1,
     & i2,i3,kd))+rsxy1(i1,i2,i3,1,0)*(rsxy1x22(i1,i2,i3,0,0)*udfrrs2(
     & i1,i2,i3,kd)+rsxy1x22(i1,i2,i3,1,0)*udfrss2(i1,i2,i3,kd))+
     & rsxy1(i1,i2,i3,0,0)*(rsxy1x22(i1,i2,i3,0,0)*udfrrr2(i1,i2,i3,
     & kd)+rsxy1x22(i1,i2,i3,1,0)*udfrrs2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,
     & 0,0)*(rsxy1(i1,i2,i3,0,0)*udfrrrr2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,
     & 1,0)*udfrrrs2(i1,i2,i3,kd))+rsxy1(i1,i2,i3,1,0)*(rsxy1(i1,i2,
     & i3,0,0)*udfrrrs2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*udfrrss2(i1,
     & i2,i3,kd)))+rsxy1(i1,i2,i3,1,0)*(rsxy1x22(i1,i2,i3,0,0)*
     & udfrrs2(i1,i2,i3,kd)+rsxy1x22(i1,i2,i3,1,0)*udfrss2(i1,i2,i3,
     & kd)+rsxy1(i1,i2,i3,0,0)*(rsxy1(i1,i2,i3,0,0)*udfrrrs2(i1,i2,i3,
     & kd)+rsxy1(i1,i2,i3,1,0)*udfrrss2(i1,i2,i3,kd))+rsxy1(i1,i2,i3,
     & 1,0)*(rsxy1(i1,i2,i3,0,0)*udfrrss2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,
     & 1,0)*udfrsss2(i1,i2,i3,kd))))+rsxy1(i1,i2,i3,1,1)*(rsxy1xx22(
     & i1,i2,i3,0,0)*udfrs2(i1,i2,i3,kd)+rsxy1xx22(i1,i2,i3,1,0)*
     & udfss2(i1,i2,i3,kd)+rsxy1x22(i1,i2,i3,0,0)*(rsxy1(i1,i2,i3,0,0)
     & *udfrrs2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*udfrss2(i1,i2,i3,kd))
     & +rsxy1(i1,i2,i3,0,0)*(rsxy1x22(i1,i2,i3,0,0)*udfrrs2(i1,i2,i3,
     & kd)+rsxy1x22(i1,i2,i3,1,0)*udfrss2(i1,i2,i3,kd))+rsxy1x22(i1,
     & i2,i3,1,0)*(rsxy1(i1,i2,i3,0,0)*udfrss2(i1,i2,i3,kd)+rsxy1(i1,
     & i2,i3,1,0)*udfsss2(i1,i2,i3,kd))+rsxy1(i1,i2,i3,1,0)*(rsxy1x22(
     & i1,i2,i3,0,0)*udfrss2(i1,i2,i3,kd)+rsxy1x22(i1,i2,i3,1,0)*
     & udfsss2(i1,i2,i3,kd))+rsxy1(i1,i2,i3,0,0)*(rsxy1x22(i1,i2,i3,0,
     & 0)*udfrrs2(i1,i2,i3,kd)+rsxy1x22(i1,i2,i3,1,0)*udfrss2(i1,i2,
     & i3,kd)+rsxy1(i1,i2,i3,0,0)*(rsxy1(i1,i2,i3,0,0)*udfrrrs2(i1,i2,
     & i3,kd)+rsxy1(i1,i2,i3,1,0)*udfrrss2(i1,i2,i3,kd))+rsxy1(i1,i2,
     & i3,1,0)*(rsxy1(i1,i2,i3,0,0)*udfrrss2(i1,i2,i3,kd)+rsxy1(i1,i2,
     & i3,1,0)*udfrsss2(i1,i2,i3,kd)))+rsxy1(i1,i2,i3,1,0)*(rsxy1x22(
     & i1,i2,i3,0,0)*udfrss2(i1,i2,i3,kd)+rsxy1x22(i1,i2,i3,1,0)*
     & udfsss2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,0,0)*(rsxy1(i1,i2,i3,0,0)*
     & udfrrss2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*udfrsss2(i1,i2,i3,kd)
     & )+rsxy1(i1,i2,i3,1,0)*(rsxy1(i1,i2,i3,0,0)*udfrsss2(i1,i2,i3,
     & kd)+rsxy1(i1,i2,i3,1,0)*udfssss2(i1,i2,i3,kd))))
         udfxxyy22(i1,i2,i3,kd)=rsxy1xyy22(i1,i2,i3,0,0)*udfr2(i1,i2,
     & i3,kd)+rsxy1xyy22(i1,i2,i3,1,0)*udfs2(i1,i2,i3,kd)+rsxy1yy22(
     & i1,i2,i3,0,0)*(rsxy1(i1,i2,i3,0,0)*udfrr2(i1,i2,i3,kd)+rsxy1(
     & i1,i2,i3,1,0)*udfrs2(i1,i2,i3,kd))+2*rsxy1x22(i1,i2,i3,0,1)*(
     & rsxy1x22(i1,i2,i3,0,1)*udfrr2(i1,i2,i3,kd)+rsxy1x22(i1,i2,i3,1,
     & 1)*udfrs2(i1,i2,i3,kd))+rsxy1(i1,i2,i3,0,0)*(rsxy1yy22(i1,i2,
     & i3,0,0)*udfrr2(i1,i2,i3,kd)+rsxy1yy22(i1,i2,i3,1,0)*udfrs2(i1,
     & i2,i3,kd))+rsxy1yy22(i1,i2,i3,1,0)*(rsxy1(i1,i2,i3,0,0)*udfrs2(
     & i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*udfss2(i1,i2,i3,kd))+2*
     & rsxy1x22(i1,i2,i3,1,1)*(rsxy1x22(i1,i2,i3,0,1)*udfrs2(i1,i2,i3,
     & kd)+rsxy1x22(i1,i2,i3,1,1)*udfss2(i1,i2,i3,kd))+rsxy1(i1,i2,i3,
     & 1,0)*(rsxy1yy22(i1,i2,i3,0,0)*udfrs2(i1,i2,i3,kd)+rsxy1yy22(i1,
     & i2,i3,1,0)*udfss2(i1,i2,i3,kd))+rsxy1y22(i1,i2,i3,0,1)*(
     & rsxy1x22(i1,i2,i3,0,0)*udfrr2(i1,i2,i3,kd)+rsxy1x22(i1,i2,i3,1,
     & 0)*udfrs2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,0,0)*(rsxy1(i1,i2,i3,0,0)
     & *udfrrr2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*udfrrs2(i1,i2,i3,kd))
     & +rsxy1(i1,i2,i3,1,0)*(rsxy1(i1,i2,i3,0,0)*udfrrs2(i1,i2,i3,kd)+
     & rsxy1(i1,i2,i3,1,0)*udfrss2(i1,i2,i3,kd)))+rsxy1(i1,i2,i3,0,1)*
     & (rsxy1xy22(i1,i2,i3,0,0)*udfrr2(i1,i2,i3,kd)+rsxy1xy22(i1,i2,
     & i3,1,0)*udfrs2(i1,i2,i3,kd)+rsxy1x22(i1,i2,i3,0,1)*(rsxy1(i1,
     & i2,i3,0,0)*udfrrr2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*udfrrs2(i1,
     & i2,i3,kd))+rsxy1(i1,i2,i3,0,0)*(rsxy1x22(i1,i2,i3,0,1)*udfrrr2(
     & i1,i2,i3,kd)+rsxy1x22(i1,i2,i3,1,1)*udfrrs2(i1,i2,i3,kd))+
     & rsxy1x22(i1,i2,i3,1,1)*(rsxy1(i1,i2,i3,0,0)*udfrrs2(i1,i2,i3,
     & kd)+rsxy1(i1,i2,i3,1,0)*udfrss2(i1,i2,i3,kd))+rsxy1(i1,i2,i3,1,
     & 0)*(rsxy1x22(i1,i2,i3,0,1)*udfrrs2(i1,i2,i3,kd)+rsxy1x22(i1,i2,
     & i3,1,1)*udfrss2(i1,i2,i3,kd)))+rsxy1y22(i1,i2,i3,1,1)*(
     & rsxy1x22(i1,i2,i3,0,0)*udfrs2(i1,i2,i3,kd)+rsxy1x22(i1,i2,i3,1,
     & 0)*udfss2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,0,0)*(rsxy1(i1,i2,i3,0,0)
     & *udfrrs2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*udfrss2(i1,i2,i3,kd))
     & +rsxy1(i1,i2,i3,1,0)*(rsxy1(i1,i2,i3,0,0)*udfrss2(i1,i2,i3,kd)+
     & rsxy1(i1,i2,i3,1,0)*udfsss2(i1,i2,i3,kd)))+rsxy1(i1,i2,i3,1,1)*
     & (rsxy1xy22(i1,i2,i3,0,0)*udfrs2(i1,i2,i3,kd)+rsxy1xy22(i1,i2,
     & i3,1,0)*udfss2(i1,i2,i3,kd)+rsxy1x22(i1,i2,i3,0,1)*(rsxy1(i1,
     & i2,i3,0,0)*udfrrs2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*udfrss2(i1,
     & i2,i3,kd))+rsxy1(i1,i2,i3,0,0)*(rsxy1x22(i1,i2,i3,0,1)*udfrrs2(
     & i1,i2,i3,kd)+rsxy1x22(i1,i2,i3,1,1)*udfrss2(i1,i2,i3,kd))+
     & rsxy1x22(i1,i2,i3,1,1)*(rsxy1(i1,i2,i3,0,0)*udfrss2(i1,i2,i3,
     & kd)+rsxy1(i1,i2,i3,1,0)*udfsss2(i1,i2,i3,kd))+rsxy1(i1,i2,i3,1,
     & 0)*(rsxy1x22(i1,i2,i3,0,1)*udfrss2(i1,i2,i3,kd)+rsxy1x22(i1,i2,
     & i3,1,1)*udfsss2(i1,i2,i3,kd)))+rsxy1(i1,i2,i3,0,1)*(rsxy1xy22(
     & i1,i2,i3,0,0)*udfrr2(i1,i2,i3,kd)+rsxy1xy22(i1,i2,i3,1,0)*
     & udfrs2(i1,i2,i3,kd)+rsxy1x22(i1,i2,i3,0,1)*(rsxy1(i1,i2,i3,0,0)
     & *udfrrr2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*udfrrs2(i1,i2,i3,kd))
     & +rsxy1(i1,i2,i3,0,0)*(rsxy1x22(i1,i2,i3,0,1)*udfrrr2(i1,i2,i3,
     & kd)+rsxy1x22(i1,i2,i3,1,1)*udfrrs2(i1,i2,i3,kd))+rsxy1x22(i1,
     & i2,i3,1,1)*(rsxy1(i1,i2,i3,0,0)*udfrrs2(i1,i2,i3,kd)+rsxy1(i1,
     & i2,i3,1,0)*udfrss2(i1,i2,i3,kd))+rsxy1(i1,i2,i3,1,0)*(rsxy1x22(
     & i1,i2,i3,0,1)*udfrrs2(i1,i2,i3,kd)+rsxy1x22(i1,i2,i3,1,1)*
     & udfrss2(i1,i2,i3,kd))+rsxy1(i1,i2,i3,0,1)*(rsxy1x22(i1,i2,i3,0,
     & 0)*udfrrr2(i1,i2,i3,kd)+rsxy1x22(i1,i2,i3,1,0)*udfrrs2(i1,i2,
     & i3,kd)+rsxy1(i1,i2,i3,0,0)*(rsxy1(i1,i2,i3,0,0)*udfrrrr2(i1,i2,
     & i3,kd)+rsxy1(i1,i2,i3,1,0)*udfrrrs2(i1,i2,i3,kd))+rsxy1(i1,i2,
     & i3,1,0)*(rsxy1(i1,i2,i3,0,0)*udfrrrs2(i1,i2,i3,kd)+rsxy1(i1,i2,
     & i3,1,0)*udfrrss2(i1,i2,i3,kd)))+rsxy1(i1,i2,i3,1,1)*(rsxy1x22(
     & i1,i2,i3,0,0)*udfrrs2(i1,i2,i3,kd)+rsxy1x22(i1,i2,i3,1,0)*
     & udfrss2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,0,0)*(rsxy1(i1,i2,i3,0,0)*
     & udfrrrs2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*udfrrss2(i1,i2,i3,kd)
     & )+rsxy1(i1,i2,i3,1,0)*(rsxy1(i1,i2,i3,0,0)*udfrrss2(i1,i2,i3,
     & kd)+rsxy1(i1,i2,i3,1,0)*udfrsss2(i1,i2,i3,kd))))+rsxy1(i1,i2,
     & i3,1,1)*(rsxy1xy22(i1,i2,i3,0,0)*udfrs2(i1,i2,i3,kd)+rsxy1xy22(
     & i1,i2,i3,1,0)*udfss2(i1,i2,i3,kd)+rsxy1x22(i1,i2,i3,0,1)*(
     & rsxy1(i1,i2,i3,0,0)*udfrrs2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*
     & udfrss2(i1,i2,i3,kd))+rsxy1(i1,i2,i3,0,0)*(rsxy1x22(i1,i2,i3,0,
     & 1)*udfrrs2(i1,i2,i3,kd)+rsxy1x22(i1,i2,i3,1,1)*udfrss2(i1,i2,
     & i3,kd))+rsxy1x22(i1,i2,i3,1,1)*(rsxy1(i1,i2,i3,0,0)*udfrss2(i1,
     & i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*udfsss2(i1,i2,i3,kd))+rsxy1(i1,
     & i2,i3,1,0)*(rsxy1x22(i1,i2,i3,0,1)*udfrss2(i1,i2,i3,kd)+
     & rsxy1x22(i1,i2,i3,1,1)*udfsss2(i1,i2,i3,kd))+rsxy1(i1,i2,i3,0,
     & 1)*(rsxy1x22(i1,i2,i3,0,0)*udfrrs2(i1,i2,i3,kd)+rsxy1x22(i1,i2,
     & i3,1,0)*udfrss2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,0,0)*(rsxy1(i1,i2,
     & i3,0,0)*udfrrrs2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*udfrrss2(i1,
     & i2,i3,kd))+rsxy1(i1,i2,i3,1,0)*(rsxy1(i1,i2,i3,0,0)*udfrrss2(
     & i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*udfrsss2(i1,i2,i3,kd)))+rsxy1(
     & i1,i2,i3,1,1)*(rsxy1x22(i1,i2,i3,0,0)*udfrss2(i1,i2,i3,kd)+
     & rsxy1x22(i1,i2,i3,1,0)*udfsss2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,0,0)
     & *(rsxy1(i1,i2,i3,0,0)*udfrrss2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)
     & *udfrsss2(i1,i2,i3,kd))+rsxy1(i1,i2,i3,1,0)*(rsxy1(i1,i2,i3,0,
     & 0)*udfrsss2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*udfssss2(i1,i2,i3,
     & kd))))
         udfxyyy22(i1,i2,i3,kd)=rsxy1yyy22(i1,i2,i3,0,0)*udfr2(i1,i2,
     & i3,kd)+rsxy1yyy22(i1,i2,i3,1,0)*udfs2(i1,i2,i3,kd)+rsxy1yy22(
     & i1,i2,i3,0,1)*(rsxy1(i1,i2,i3,0,0)*udfrr2(i1,i2,i3,kd)+rsxy1(
     & i1,i2,i3,1,0)*udfrs2(i1,i2,i3,kd))+2*rsxy1y22(i1,i2,i3,0,1)*(
     & rsxy1x22(i1,i2,i3,0,1)*udfrr2(i1,i2,i3,kd)+rsxy1x22(i1,i2,i3,1,
     & 1)*udfrs2(i1,i2,i3,kd))+rsxy1(i1,i2,i3,0,1)*(rsxy1yy22(i1,i2,
     & i3,0,0)*udfrr2(i1,i2,i3,kd)+rsxy1yy22(i1,i2,i3,1,0)*udfrs2(i1,
     & i2,i3,kd))+rsxy1yy22(i1,i2,i3,1,1)*(rsxy1(i1,i2,i3,0,0)*udfrs2(
     & i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*udfss2(i1,i2,i3,kd))+2*
     & rsxy1y22(i1,i2,i3,1,1)*(rsxy1x22(i1,i2,i3,0,1)*udfrs2(i1,i2,i3,
     & kd)+rsxy1x22(i1,i2,i3,1,1)*udfss2(i1,i2,i3,kd))+rsxy1(i1,i2,i3,
     & 1,1)*(rsxy1yy22(i1,i2,i3,0,0)*udfrs2(i1,i2,i3,kd)+rsxy1yy22(i1,
     & i2,i3,1,0)*udfss2(i1,i2,i3,kd))+rsxy1y22(i1,i2,i3,0,1)*(
     & rsxy1x22(i1,i2,i3,0,1)*udfrr2(i1,i2,i3,kd)+rsxy1x22(i1,i2,i3,1,
     & 1)*udfrs2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,0,1)*(rsxy1(i1,i2,i3,0,0)
     & *udfrrr2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*udfrrs2(i1,i2,i3,kd))
     & +rsxy1(i1,i2,i3,1,1)*(rsxy1(i1,i2,i3,0,0)*udfrrs2(i1,i2,i3,kd)+
     & rsxy1(i1,i2,i3,1,0)*udfrss2(i1,i2,i3,kd)))+rsxy1(i1,i2,i3,0,1)*
     & (rsxy1yy22(i1,i2,i3,0,0)*udfrr2(i1,i2,i3,kd)+rsxy1yy22(i1,i2,
     & i3,1,0)*udfrs2(i1,i2,i3,kd)+rsxy1y22(i1,i2,i3,0,1)*(rsxy1(i1,
     & i2,i3,0,0)*udfrrr2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*udfrrs2(i1,
     & i2,i3,kd))+rsxy1(i1,i2,i3,0,1)*(rsxy1x22(i1,i2,i3,0,1)*udfrrr2(
     & i1,i2,i3,kd)+rsxy1x22(i1,i2,i3,1,1)*udfrrs2(i1,i2,i3,kd))+
     & rsxy1y22(i1,i2,i3,1,1)*(rsxy1(i1,i2,i3,0,0)*udfrrs2(i1,i2,i3,
     & kd)+rsxy1(i1,i2,i3,1,0)*udfrss2(i1,i2,i3,kd))+rsxy1(i1,i2,i3,1,
     & 1)*(rsxy1x22(i1,i2,i3,0,1)*udfrrs2(i1,i2,i3,kd)+rsxy1x22(i1,i2,
     & i3,1,1)*udfrss2(i1,i2,i3,kd)))+rsxy1y22(i1,i2,i3,1,1)*(
     & rsxy1x22(i1,i2,i3,0,1)*udfrs2(i1,i2,i3,kd)+rsxy1x22(i1,i2,i3,1,
     & 1)*udfss2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,0,1)*(rsxy1(i1,i2,i3,0,0)
     & *udfrrs2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*udfrss2(i1,i2,i3,kd))
     & +rsxy1(i1,i2,i3,1,1)*(rsxy1(i1,i2,i3,0,0)*udfrss2(i1,i2,i3,kd)+
     & rsxy1(i1,i2,i3,1,0)*udfsss2(i1,i2,i3,kd)))+rsxy1(i1,i2,i3,1,1)*
     & (rsxy1yy22(i1,i2,i3,0,0)*udfrs2(i1,i2,i3,kd)+rsxy1yy22(i1,i2,
     & i3,1,0)*udfss2(i1,i2,i3,kd)+rsxy1y22(i1,i2,i3,0,1)*(rsxy1(i1,
     & i2,i3,0,0)*udfrrs2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*udfrss2(i1,
     & i2,i3,kd))+rsxy1(i1,i2,i3,0,1)*(rsxy1x22(i1,i2,i3,0,1)*udfrrs2(
     & i1,i2,i3,kd)+rsxy1x22(i1,i2,i3,1,1)*udfrss2(i1,i2,i3,kd))+
     & rsxy1y22(i1,i2,i3,1,1)*(rsxy1(i1,i2,i3,0,0)*udfrss2(i1,i2,i3,
     & kd)+rsxy1(i1,i2,i3,1,0)*udfsss2(i1,i2,i3,kd))+rsxy1(i1,i2,i3,1,
     & 1)*(rsxy1x22(i1,i2,i3,0,1)*udfrss2(i1,i2,i3,kd)+rsxy1x22(i1,i2,
     & i3,1,1)*udfsss2(i1,i2,i3,kd)))+rsxy1(i1,i2,i3,0,1)*(rsxy1yy22(
     & i1,i2,i3,0,0)*udfrr2(i1,i2,i3,kd)+rsxy1yy22(i1,i2,i3,1,0)*
     & udfrs2(i1,i2,i3,kd)+rsxy1y22(i1,i2,i3,0,1)*(rsxy1(i1,i2,i3,0,0)
     & *udfrrr2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*udfrrs2(i1,i2,i3,kd))
     & +rsxy1(i1,i2,i3,0,1)*(rsxy1x22(i1,i2,i3,0,1)*udfrrr2(i1,i2,i3,
     & kd)+rsxy1x22(i1,i2,i3,1,1)*udfrrs2(i1,i2,i3,kd))+rsxy1y22(i1,
     & i2,i3,1,1)*(rsxy1(i1,i2,i3,0,0)*udfrrs2(i1,i2,i3,kd)+rsxy1(i1,
     & i2,i3,1,0)*udfrss2(i1,i2,i3,kd))+rsxy1(i1,i2,i3,1,1)*(rsxy1x22(
     & i1,i2,i3,0,1)*udfrrs2(i1,i2,i3,kd)+rsxy1x22(i1,i2,i3,1,1)*
     & udfrss2(i1,i2,i3,kd))+rsxy1(i1,i2,i3,0,1)*(rsxy1x22(i1,i2,i3,0,
     & 1)*udfrrr2(i1,i2,i3,kd)+rsxy1x22(i1,i2,i3,1,1)*udfrrs2(i1,i2,
     & i3,kd)+rsxy1(i1,i2,i3,0,1)*(rsxy1(i1,i2,i3,0,0)*udfrrrr2(i1,i2,
     & i3,kd)+rsxy1(i1,i2,i3,1,0)*udfrrrs2(i1,i2,i3,kd))+rsxy1(i1,i2,
     & i3,1,1)*(rsxy1(i1,i2,i3,0,0)*udfrrrs2(i1,i2,i3,kd)+rsxy1(i1,i2,
     & i3,1,0)*udfrrss2(i1,i2,i3,kd)))+rsxy1(i1,i2,i3,1,1)*(rsxy1x22(
     & i1,i2,i3,0,1)*udfrrs2(i1,i2,i3,kd)+rsxy1x22(i1,i2,i3,1,1)*
     & udfrss2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,0,1)*(rsxy1(i1,i2,i3,0,0)*
     & udfrrrs2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*udfrrss2(i1,i2,i3,kd)
     & )+rsxy1(i1,i2,i3,1,1)*(rsxy1(i1,i2,i3,0,0)*udfrrss2(i1,i2,i3,
     & kd)+rsxy1(i1,i2,i3,1,0)*udfrsss2(i1,i2,i3,kd))))+rsxy1(i1,i2,
     & i3,1,1)*(rsxy1yy22(i1,i2,i3,0,0)*udfrs2(i1,i2,i3,kd)+rsxy1yy22(
     & i1,i2,i3,1,0)*udfss2(i1,i2,i3,kd)+rsxy1y22(i1,i2,i3,0,1)*(
     & rsxy1(i1,i2,i3,0,0)*udfrrs2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*
     & udfrss2(i1,i2,i3,kd))+rsxy1(i1,i2,i3,0,1)*(rsxy1x22(i1,i2,i3,0,
     & 1)*udfrrs2(i1,i2,i3,kd)+rsxy1x22(i1,i2,i3,1,1)*udfrss2(i1,i2,
     & i3,kd))+rsxy1y22(i1,i2,i3,1,1)*(rsxy1(i1,i2,i3,0,0)*udfrss2(i1,
     & i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*udfsss2(i1,i2,i3,kd))+rsxy1(i1,
     & i2,i3,1,1)*(rsxy1x22(i1,i2,i3,0,1)*udfrss2(i1,i2,i3,kd)+
     & rsxy1x22(i1,i2,i3,1,1)*udfsss2(i1,i2,i3,kd))+rsxy1(i1,i2,i3,0,
     & 1)*(rsxy1x22(i1,i2,i3,0,1)*udfrrs2(i1,i2,i3,kd)+rsxy1x22(i1,i2,
     & i3,1,1)*udfrss2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,0,1)*(rsxy1(i1,i2,
     & i3,0,0)*udfrrrs2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*udfrrss2(i1,
     & i2,i3,kd))+rsxy1(i1,i2,i3,1,1)*(rsxy1(i1,i2,i3,0,0)*udfrrss2(
     & i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*udfrsss2(i1,i2,i3,kd)))+rsxy1(
     & i1,i2,i3,1,1)*(rsxy1x22(i1,i2,i3,0,1)*udfrss2(i1,i2,i3,kd)+
     & rsxy1x22(i1,i2,i3,1,1)*udfsss2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,0,1)
     & *(rsxy1(i1,i2,i3,0,0)*udfrrss2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)
     & *udfrsss2(i1,i2,i3,kd))+rsxy1(i1,i2,i3,1,1)*(rsxy1(i1,i2,i3,0,
     & 0)*udfrsss2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*udfssss2(i1,i2,i3,
     & kd))))
         udfyyyy22(i1,i2,i3,kd)=rsxy1yyy22(i1,i2,i3,0,1)*udfr2(i1,i2,
     & i3,kd)+rsxy1yyy22(i1,i2,i3,1,1)*udfs2(i1,i2,i3,kd)+rsxy1yy22(
     & i1,i2,i3,0,1)*(rsxy1(i1,i2,i3,0,1)*udfrr2(i1,i2,i3,kd)+rsxy1(
     & i1,i2,i3,1,1)*udfrs2(i1,i2,i3,kd))+2*rsxy1y22(i1,i2,i3,0,1)*(
     & rsxy1y22(i1,i2,i3,0,1)*udfrr2(i1,i2,i3,kd)+rsxy1y22(i1,i2,i3,1,
     & 1)*udfrs2(i1,i2,i3,kd))+rsxy1(i1,i2,i3,0,1)*(rsxy1yy22(i1,i2,
     & i3,0,1)*udfrr2(i1,i2,i3,kd)+rsxy1yy22(i1,i2,i3,1,1)*udfrs2(i1,
     & i2,i3,kd))+rsxy1yy22(i1,i2,i3,1,1)*(rsxy1(i1,i2,i3,0,1)*udfrs2(
     & i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,1)*udfss2(i1,i2,i3,kd))+2*
     & rsxy1y22(i1,i2,i3,1,1)*(rsxy1y22(i1,i2,i3,0,1)*udfrs2(i1,i2,i3,
     & kd)+rsxy1y22(i1,i2,i3,1,1)*udfss2(i1,i2,i3,kd))+rsxy1(i1,i2,i3,
     & 1,1)*(rsxy1yy22(i1,i2,i3,0,1)*udfrs2(i1,i2,i3,kd)+rsxy1yy22(i1,
     & i2,i3,1,1)*udfss2(i1,i2,i3,kd))+rsxy1y22(i1,i2,i3,0,1)*(
     & rsxy1y22(i1,i2,i3,0,1)*udfrr2(i1,i2,i3,kd)+rsxy1y22(i1,i2,i3,1,
     & 1)*udfrs2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,0,1)*(rsxy1(i1,i2,i3,0,1)
     & *udfrrr2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,1)*udfrrs2(i1,i2,i3,kd))
     & +rsxy1(i1,i2,i3,1,1)*(rsxy1(i1,i2,i3,0,1)*udfrrs2(i1,i2,i3,kd)+
     & rsxy1(i1,i2,i3,1,1)*udfrss2(i1,i2,i3,kd)))+rsxy1(i1,i2,i3,0,1)*
     & (rsxy1yy22(i1,i2,i3,0,1)*udfrr2(i1,i2,i3,kd)+rsxy1yy22(i1,i2,
     & i3,1,1)*udfrs2(i1,i2,i3,kd)+rsxy1y22(i1,i2,i3,0,1)*(rsxy1(i1,
     & i2,i3,0,1)*udfrrr2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,1)*udfrrs2(i1,
     & i2,i3,kd))+rsxy1(i1,i2,i3,0,1)*(rsxy1y22(i1,i2,i3,0,1)*udfrrr2(
     & i1,i2,i3,kd)+rsxy1y22(i1,i2,i3,1,1)*udfrrs2(i1,i2,i3,kd))+
     & rsxy1y22(i1,i2,i3,1,1)*(rsxy1(i1,i2,i3,0,1)*udfrrs2(i1,i2,i3,
     & kd)+rsxy1(i1,i2,i3,1,1)*udfrss2(i1,i2,i3,kd))+rsxy1(i1,i2,i3,1,
     & 1)*(rsxy1y22(i1,i2,i3,0,1)*udfrrs2(i1,i2,i3,kd)+rsxy1y22(i1,i2,
     & i3,1,1)*udfrss2(i1,i2,i3,kd)))+rsxy1y22(i1,i2,i3,1,1)*(
     & rsxy1y22(i1,i2,i3,0,1)*udfrs2(i1,i2,i3,kd)+rsxy1y22(i1,i2,i3,1,
     & 1)*udfss2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,0,1)*(rsxy1(i1,i2,i3,0,1)
     & *udfrrs2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,1)*udfrss2(i1,i2,i3,kd))
     & +rsxy1(i1,i2,i3,1,1)*(rsxy1(i1,i2,i3,0,1)*udfrss2(i1,i2,i3,kd)+
     & rsxy1(i1,i2,i3,1,1)*udfsss2(i1,i2,i3,kd)))+rsxy1(i1,i2,i3,1,1)*
     & (rsxy1yy22(i1,i2,i3,0,1)*udfrs2(i1,i2,i3,kd)+rsxy1yy22(i1,i2,
     & i3,1,1)*udfss2(i1,i2,i3,kd)+rsxy1y22(i1,i2,i3,0,1)*(rsxy1(i1,
     & i2,i3,0,1)*udfrrs2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,1)*udfrss2(i1,
     & i2,i3,kd))+rsxy1(i1,i2,i3,0,1)*(rsxy1y22(i1,i2,i3,0,1)*udfrrs2(
     & i1,i2,i3,kd)+rsxy1y22(i1,i2,i3,1,1)*udfrss2(i1,i2,i3,kd))+
     & rsxy1y22(i1,i2,i3,1,1)*(rsxy1(i1,i2,i3,0,1)*udfrss2(i1,i2,i3,
     & kd)+rsxy1(i1,i2,i3,1,1)*udfsss2(i1,i2,i3,kd))+rsxy1(i1,i2,i3,1,
     & 1)*(rsxy1y22(i1,i2,i3,0,1)*udfrss2(i1,i2,i3,kd)+rsxy1y22(i1,i2,
     & i3,1,1)*udfsss2(i1,i2,i3,kd)))+rsxy1(i1,i2,i3,0,1)*(rsxy1yy22(
     & i1,i2,i3,0,1)*udfrr2(i1,i2,i3,kd)+rsxy1yy22(i1,i2,i3,1,1)*
     & udfrs2(i1,i2,i3,kd)+rsxy1y22(i1,i2,i3,0,1)*(rsxy1(i1,i2,i3,0,1)
     & *udfrrr2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,1)*udfrrs2(i1,i2,i3,kd))
     & +rsxy1(i1,i2,i3,0,1)*(rsxy1y22(i1,i2,i3,0,1)*udfrrr2(i1,i2,i3,
     & kd)+rsxy1y22(i1,i2,i3,1,1)*udfrrs2(i1,i2,i3,kd))+rsxy1y22(i1,
     & i2,i3,1,1)*(rsxy1(i1,i2,i3,0,1)*udfrrs2(i1,i2,i3,kd)+rsxy1(i1,
     & i2,i3,1,1)*udfrss2(i1,i2,i3,kd))+rsxy1(i1,i2,i3,1,1)*(rsxy1y22(
     & i1,i2,i3,0,1)*udfrrs2(i1,i2,i3,kd)+rsxy1y22(i1,i2,i3,1,1)*
     & udfrss2(i1,i2,i3,kd))+rsxy1(i1,i2,i3,0,1)*(rsxy1y22(i1,i2,i3,0,
     & 1)*udfrrr2(i1,i2,i3,kd)+rsxy1y22(i1,i2,i3,1,1)*udfrrs2(i1,i2,
     & i3,kd)+rsxy1(i1,i2,i3,0,1)*(rsxy1(i1,i2,i3,0,1)*udfrrrr2(i1,i2,
     & i3,kd)+rsxy1(i1,i2,i3,1,1)*udfrrrs2(i1,i2,i3,kd))+rsxy1(i1,i2,
     & i3,1,1)*(rsxy1(i1,i2,i3,0,1)*udfrrrs2(i1,i2,i3,kd)+rsxy1(i1,i2,
     & i3,1,1)*udfrrss2(i1,i2,i3,kd)))+rsxy1(i1,i2,i3,1,1)*(rsxy1y22(
     & i1,i2,i3,0,1)*udfrrs2(i1,i2,i3,kd)+rsxy1y22(i1,i2,i3,1,1)*
     & udfrss2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,0,1)*(rsxy1(i1,i2,i3,0,1)*
     & udfrrrs2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,1)*udfrrss2(i1,i2,i3,kd)
     & )+rsxy1(i1,i2,i3,1,1)*(rsxy1(i1,i2,i3,0,1)*udfrrss2(i1,i2,i3,
     & kd)+rsxy1(i1,i2,i3,1,1)*udfrsss2(i1,i2,i3,kd))))+rsxy1(i1,i2,
     & i3,1,1)*(rsxy1yy22(i1,i2,i3,0,1)*udfrs2(i1,i2,i3,kd)+rsxy1yy22(
     & i1,i2,i3,1,1)*udfss2(i1,i2,i3,kd)+rsxy1y22(i1,i2,i3,0,1)*(
     & rsxy1(i1,i2,i3,0,1)*udfrrs2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,1)*
     & udfrss2(i1,i2,i3,kd))+rsxy1(i1,i2,i3,0,1)*(rsxy1y22(i1,i2,i3,0,
     & 1)*udfrrs2(i1,i2,i3,kd)+rsxy1y22(i1,i2,i3,1,1)*udfrss2(i1,i2,
     & i3,kd))+rsxy1y22(i1,i2,i3,1,1)*(rsxy1(i1,i2,i3,0,1)*udfrss2(i1,
     & i2,i3,kd)+rsxy1(i1,i2,i3,1,1)*udfsss2(i1,i2,i3,kd))+rsxy1(i1,
     & i2,i3,1,1)*(rsxy1y22(i1,i2,i3,0,1)*udfrss2(i1,i2,i3,kd)+
     & rsxy1y22(i1,i2,i3,1,1)*udfsss2(i1,i2,i3,kd))+rsxy1(i1,i2,i3,0,
     & 1)*(rsxy1y22(i1,i2,i3,0,1)*udfrrs2(i1,i2,i3,kd)+rsxy1y22(i1,i2,
     & i3,1,1)*udfrss2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,0,1)*(rsxy1(i1,i2,
     & i3,0,1)*udfrrrs2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,1)*udfrrss2(i1,
     & i2,i3,kd))+rsxy1(i1,i2,i3,1,1)*(rsxy1(i1,i2,i3,0,1)*udfrrss2(
     & i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,1)*udfrsss2(i1,i2,i3,kd)))+rsxy1(
     & i1,i2,i3,1,1)*(rsxy1y22(i1,i2,i3,0,1)*udfrss2(i1,i2,i3,kd)+
     & rsxy1y22(i1,i2,i3,1,1)*udfsss2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,0,1)
     & *(rsxy1(i1,i2,i3,0,1)*udfrrss2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,1)
     & *udfrsss2(i1,i2,i3,kd))+rsxy1(i1,i2,i3,1,1)*(rsxy1(i1,i2,i3,0,
     & 1)*udfrsss2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,1)*udfssss2(i1,i2,i3,
     & kd))))
         udfLapSq22(i1,i2,i3,kd)=udfxxxx22(i1,i2,i3,kd)+udfyyyy22(i1,
     & i2,i3,kd)+2.*udfxxyy22(i1,i2,i3,kd)
c ..... end: 3rd and 4th derivatives, 2D ....
         udfxx23(i1,i2,i3,kd)=rsxy1(i1,i2,i3,0,0)**2*udfrr2(i1,i2,i3,
     & kd)+rsxy1(i1,i2,i3,1,0)**2*udfss2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,
     & 2,0)**2*udftt2(i1,i2,i3,kd)+2.*rsxy1(i1,i2,i3,0,0)*rsxy1(i1,i2,
     & i3,1,0)*udfrs2(i1,i2,i3,kd)+2.*rsxy1(i1,i2,i3,0,0)*rsxy1(i1,i2,
     & i3,2,0)*udfrt2(i1,i2,i3,kd)+2.*rsxy1(i1,i2,i3,1,0)*rsxy1(i1,i2,
     & i3,2,0)*udfst2(i1,i2,i3,kd)+rsxy1x23(i1,i2,i3,0,0)*udfr2(i1,i2,
     & i3,kd)+rsxy1x23(i1,i2,i3,1,0)*udfs2(i1,i2,i3,kd)+rsxy1x23(i1,
     & i2,i3,2,0)*udft2(i1,i2,i3,kd)
         udfyy23(i1,i2,i3,kd)=rsxy1(i1,i2,i3,0,1)**2*udfrr2(i1,i2,i3,
     & kd)+rsxy1(i1,i2,i3,1,1)**2*udfss2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,
     & 2,1)**2*udftt2(i1,i2,i3,kd)+2.*rsxy1(i1,i2,i3,0,1)*rsxy1(i1,i2,
     & i3,1,1)*udfrs2(i1,i2,i3,kd)+2.*rsxy1(i1,i2,i3,0,1)*rsxy1(i1,i2,
     & i3,2,1)*udfrt2(i1,i2,i3,kd)+2.*rsxy1(i1,i2,i3,1,1)*rsxy1(i1,i2,
     & i3,2,1)*udfst2(i1,i2,i3,kd)+rsxy1y23(i1,i2,i3,0,1)*udfr2(i1,i2,
     & i3,kd)+rsxy1y23(i1,i2,i3,1,1)*udfs2(i1,i2,i3,kd)+rsxy1y23(i1,
     & i2,i3,2,1)*udft2(i1,i2,i3,kd)
         udfzz23(i1,i2,i3,kd)=rsxy1(i1,i2,i3,0,2)**2*udfrr2(i1,i2,i3,
     & kd)+rsxy1(i1,i2,i3,1,2)**2*udfss2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,
     & 2,2)**2*udftt2(i1,i2,i3,kd)+2.*rsxy1(i1,i2,i3,0,2)*rsxy1(i1,i2,
     & i3,1,2)*udfrs2(i1,i2,i3,kd)+2.*rsxy1(i1,i2,i3,0,2)*rsxy1(i1,i2,
     & i3,2,2)*udfrt2(i1,i2,i3,kd)+2.*rsxy1(i1,i2,i3,1,2)*rsxy1(i1,i2,
     & i3,2,2)*udfst2(i1,i2,i3,kd)+rsxy1z23(i1,i2,i3,0,2)*udfr2(i1,i2,
     & i3,kd)+rsxy1z23(i1,i2,i3,1,2)*udfs2(i1,i2,i3,kd)+rsxy1z23(i1,
     & i2,i3,2,2)*udft2(i1,i2,i3,kd)
         udfxy23(i1,i2,i3,kd)=rsxy1(i1,i2,i3,0,0)*rsxy1(i1,i2,i3,0,1)*
     & udfrr2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*rsxy1(i1,i2,i3,1,1)*
     & udfss2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,2,0)*rsxy1(i1,i2,i3,2,1)*
     & udftt2(i1,i2,i3,kd)+(rsxy1(i1,i2,i3,0,0)*rsxy1(i1,i2,i3,1,1)+
     & rsxy1(i1,i2,i3,0,1)*rsxy1(i1,i2,i3,1,0))*udfrs2(i1,i2,i3,kd)+(
     & rsxy1(i1,i2,i3,0,0)*rsxy1(i1,i2,i3,2,1)+rsxy1(i1,i2,i3,0,1)*
     & rsxy1(i1,i2,i3,2,0))*udfrt2(i1,i2,i3,kd)+(rsxy1(i1,i2,i3,1,0)*
     & rsxy1(i1,i2,i3,2,1)+rsxy1(i1,i2,i3,1,1)*rsxy1(i1,i2,i3,2,0))*
     & udfst2(i1,i2,i3,kd)+rsxy1x23(i1,i2,i3,0,1)*udfr2(i1,i2,i3,kd)+
     & rsxy1x23(i1,i2,i3,1,1)*udfs2(i1,i2,i3,kd)+rsxy1x23(i1,i2,i3,2,
     & 1)*udft2(i1,i2,i3,kd)
         udfxz23(i1,i2,i3,kd)=rsxy1(i1,i2,i3,0,0)*rsxy1(i1,i2,i3,0,2)*
     & udfrr2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*rsxy1(i1,i2,i3,1,2)*
     & udfss2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,2,0)*rsxy1(i1,i2,i3,2,2)*
     & udftt2(i1,i2,i3,kd)+(rsxy1(i1,i2,i3,0,0)*rsxy1(i1,i2,i3,1,2)+
     & rsxy1(i1,i2,i3,0,2)*rsxy1(i1,i2,i3,1,0))*udfrs2(i1,i2,i3,kd)+(
     & rsxy1(i1,i2,i3,0,0)*rsxy1(i1,i2,i3,2,2)+rsxy1(i1,i2,i3,0,2)*
     & rsxy1(i1,i2,i3,2,0))*udfrt2(i1,i2,i3,kd)+(rsxy1(i1,i2,i3,1,0)*
     & rsxy1(i1,i2,i3,2,2)+rsxy1(i1,i2,i3,1,2)*rsxy1(i1,i2,i3,2,0))*
     & udfst2(i1,i2,i3,kd)+rsxy1x23(i1,i2,i3,0,2)*udfr2(i1,i2,i3,kd)+
     & rsxy1x23(i1,i2,i3,1,2)*udfs2(i1,i2,i3,kd)+rsxy1x23(i1,i2,i3,2,
     & 2)*udft2(i1,i2,i3,kd)
         udfyz23(i1,i2,i3,kd)=rsxy1(i1,i2,i3,0,1)*rsxy1(i1,i2,i3,0,2)*
     & udfrr2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,1)*rsxy1(i1,i2,i3,1,2)*
     & udfss2(i1,i2,i3,kd)+rsxy1(i1,i2,i3,2,1)*rsxy1(i1,i2,i3,2,2)*
     & udftt2(i1,i2,i3,kd)+(rsxy1(i1,i2,i3,0,1)*rsxy1(i1,i2,i3,1,2)+
     & rsxy1(i1,i2,i3,0,2)*rsxy1(i1,i2,i3,1,1))*udfrs2(i1,i2,i3,kd)+(
     & rsxy1(i1,i2,i3,0,1)*rsxy1(i1,i2,i3,2,2)+rsxy1(i1,i2,i3,0,2)*
     & rsxy1(i1,i2,i3,2,1))*udfrt2(i1,i2,i3,kd)+(rsxy1(i1,i2,i3,1,1)*
     & rsxy1(i1,i2,i3,2,2)+rsxy1(i1,i2,i3,1,2)*rsxy1(i1,i2,i3,2,1))*
     & udfst2(i1,i2,i3,kd)+rsxy1y23(i1,i2,i3,0,2)*udfr2(i1,i2,i3,kd)+
     & rsxy1y23(i1,i2,i3,1,2)*udfs2(i1,i2,i3,kd)+rsxy1y23(i1,i2,i3,2,
     & 2)*udft2(i1,i2,i3,kd)
         udflaplacian23(i1,i2,i3,kd)=(rsxy1(i1,i2,i3,0,0)**2+rsxy1(i1,
     & i2,i3,0,1)**2+rsxy1(i1,i2,i3,0,2)**2)*udfrr2(i1,i2,i3,kd)+(
     & rsxy1(i1,i2,i3,1,0)**2+rsxy1(i1,i2,i3,1,1)**2+rsxy1(i1,i2,i3,1,
     & 2)**2)*udfss2(i1,i2,i3,kd)+(rsxy1(i1,i2,i3,2,0)**2+rsxy1(i1,i2,
     & i3,2,1)**2+rsxy1(i1,i2,i3,2,2)**2)*udftt2(i1,i2,i3,kd)+2.*(
     & rsxy1(i1,i2,i3,0,0)*rsxy1(i1,i2,i3,1,0)+ rsxy1(i1,i2,i3,0,1)*
     & rsxy1(i1,i2,i3,1,1)+rsxy1(i1,i2,i3,0,2)*rsxy1(i1,i2,i3,1,2))*
     & udfrs2(i1,i2,i3,kd)+2.*(rsxy1(i1,i2,i3,0,0)*rsxy1(i1,i2,i3,2,0)
     & + rsxy1(i1,i2,i3,0,1)*rsxy1(i1,i2,i3,2,1)+rsxy1(i1,i2,i3,0,2)*
     & rsxy1(i1,i2,i3,2,2))*udfrt2(i1,i2,i3,kd)+2.*(rsxy1(i1,i2,i3,1,
     & 0)*rsxy1(i1,i2,i3,2,0)+ rsxy1(i1,i2,i3,1,1)*rsxy1(i1,i2,i3,2,1)
     & +rsxy1(i1,i2,i3,1,2)*rsxy1(i1,i2,i3,2,2))*udfst2(i1,i2,i3,kd)+(
     & rsxy1x23(i1,i2,i3,0,0)+rsxy1y23(i1,i2,i3,0,1)+rsxy1z23(i1,i2,
     & i3,0,2))*udfr2(i1,i2,i3,kd)+(rsxy1x23(i1,i2,i3,1,0)+rsxy1y23(
     & i1,i2,i3,1,1)+rsxy1z23(i1,i2,i3,1,2))*udfs2(i1,i2,i3,kd)+(
     & rsxy1x23(i1,i2,i3,2,0)+rsxy1y23(i1,i2,i3,2,1)+rsxy1z23(i1,i2,
     & i3,2,2))*udft2(i1,i2,i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
         dx112(kd) = 1./(2.*dx1(kd))
         dx122(kd) = 1./(dx1(kd)**2)
         udfx23r(i1,i2,i3,kd)=(udf(i1+1,i2,i3,kd)-udf(i1-1,i2,i3,kd))*
     & dx112(0)
         udfy23r(i1,i2,i3,kd)=(udf(i1,i2+1,i3,kd)-udf(i1,i2-1,i3,kd))*
     & dx112(1)
         udfz23r(i1,i2,i3,kd)=(udf(i1,i2,i3+1,kd)-udf(i1,i2,i3-1,kd))*
     & dx112(2)
         udfxx23r(i1,i2,i3,kd)=(-2.*udf(i1,i2,i3,kd)+(udf(i1+1,i2,i3,
     & kd)+udf(i1-1,i2,i3,kd)) )*dx122(0)
         udfyy23r(i1,i2,i3,kd)=(-2.*udf(i1,i2,i3,kd)+(udf(i1,i2+1,i3,
     & kd)+udf(i1,i2-1,i3,kd)) )*dx122(1)
         udfxy23r(i1,i2,i3,kd)=(udfx23r(i1,i2+1,i3,kd)-udfx23r(i1,i2-1,
     & i3,kd))*dx112(1)
         udfzz23r(i1,i2,i3,kd)=(-2.*udf(i1,i2,i3,kd)+(udf(i1,i2,i3+1,
     & kd)+udf(i1,i2,i3-1,kd)) )*dx122(2)
         udfxz23r(i1,i2,i3,kd)=(udfx23r(i1,i2,i3+1,kd)-udfx23r(i1,i2,
     & i3-1,kd))*dx112(2)
         udfyz23r(i1,i2,i3,kd)=(udfy23r(i1,i2,i3+1,kd)-udfy23r(i1,i2,
     & i3-1,kd))*dx112(2)
         udfx21r(i1,i2,i3,kd)= udfx23r(i1,i2,i3,kd)
         udfy21r(i1,i2,i3,kd)= udfy23r(i1,i2,i3,kd)
         udfz21r(i1,i2,i3,kd)= udfz23r(i1,i2,i3,kd)
         udfxx21r(i1,i2,i3,kd)= udfxx23r(i1,i2,i3,kd)
         udfyy21r(i1,i2,i3,kd)= udfyy23r(i1,i2,i3,kd)
         udfzz21r(i1,i2,i3,kd)= udfzz23r(i1,i2,i3,kd)
         udfxy21r(i1,i2,i3,kd)= udfxy23r(i1,i2,i3,kd)
         udfxz21r(i1,i2,i3,kd)= udfxz23r(i1,i2,i3,kd)
         udfyz21r(i1,i2,i3,kd)= udfyz23r(i1,i2,i3,kd)
         udflaplacian21r(i1,i2,i3,kd)=udfxx23r(i1,i2,i3,kd)
         udfx22r(i1,i2,i3,kd)= udfx23r(i1,i2,i3,kd)
         udfy22r(i1,i2,i3,kd)= udfy23r(i1,i2,i3,kd)
         udfz22r(i1,i2,i3,kd)= udfz23r(i1,i2,i3,kd)
         udfxx22r(i1,i2,i3,kd)= udfxx23r(i1,i2,i3,kd)
         udfyy22r(i1,i2,i3,kd)= udfyy23r(i1,i2,i3,kd)
         udfzz22r(i1,i2,i3,kd)= udfzz23r(i1,i2,i3,kd)
         udfxy22r(i1,i2,i3,kd)= udfxy23r(i1,i2,i3,kd)
         udfxz22r(i1,i2,i3,kd)= udfxz23r(i1,i2,i3,kd)
         udfyz22r(i1,i2,i3,kd)= udfyz23r(i1,i2,i3,kd)
         udflaplacian22r(i1,i2,i3,kd)=udfxx23r(i1,i2,i3,kd)+udfyy23r(
     & i1,i2,i3,kd)
         udflaplacian23r(i1,i2,i3,kd)=udfxx23r(i1,i2,i3,kd)+udfyy23r(
     & i1,i2,i3,kd)+udfzz23r(i1,i2,i3,kd)
         udfxxx22r(i1,i2,i3,kd)=(-2.*(udf(i1+1,i2,i3,kd)-udf(i1-1,i2,
     & i3,kd))+(udf(i1+2,i2,i3,kd)-udf(i1-2,i2,i3,kd)) )*dx122(0)*
     & dx112(0)
         udfyyy22r(i1,i2,i3,kd)=(-2.*(udf(i1,i2+1,i3,kd)-udf(i1,i2-1,
     & i3,kd))+(udf(i1,i2+2,i3,kd)-udf(i1,i2-2,i3,kd)) )*dx122(1)*
     & dx112(1)
         udfxxy22r(i1,i2,i3,kd)=( udfxx22r(i1,i2+1,i3,kd)-udfxx22r(i1,
     & i2-1,i3,kd))/(2.*dx1(1))
         udfxyy22r(i1,i2,i3,kd)=( udfyy22r(i1+1,i2,i3,kd)-udfyy22r(i1-
     & 1,i2,i3,kd))/(2.*dx1(0))
         udfxxxx22r(i1,i2,i3,kd)=(6.*udf(i1,i2,i3,kd)-4.*(udf(i1+1,i2,
     & i3,kd)+udf(i1-1,i2,i3,kd)) +(udf(i1+2,i2,i3,kd)+udf(i1-2,i2,i3,
     & kd)) )/(dx1(0)**4)
         udfyyyy22r(i1,i2,i3,kd)=(6.*udf(i1,i2,i3,kd)-4.*(udf(i1,i2+1,
     & i3,kd)+udf(i1,i2-1,i3,kd)) +(udf(i1,i2+2,i3,kd)+udf(i1,i2-2,i3,
     & kd)) )/(dx1(1)**4)
         udfxxyy22r(i1,i2,i3,kd)=( 4.*udf(i1,i2,i3,kd)-2.*(udf(i1+1,i2,
     & i3,kd)+udf(i1-1,i2,i3,kd)+udf(i1,i2+1,i3,kd)+udf(i1,i2-1,i3,kd)
     & )+   (udf(i1+1,i2+1,i3,kd)+udf(i1-1,i2+1,i3,kd)+udf(i1+1,i2-1,
     & i3,kd)+udf(i1-1,i2-1,i3,kd)) )/(dx1(0)**2*dx1(1)**2)
         udfLapSq22r(i1,i2,i3,kd)= ( 6.*udf(i1,i2,i3,kd)- 4.*(udf(i1+1,
     & i2,i3,kd)+udf(i1-1,i2,i3,kd))+(udf(i1+2,i2,i3,kd)+udf(i1-2,i2,
     & i3,kd)) )/(dx1(0)**4)+( 6.*udf(i1,i2,i3,kd)-4.*(udf(i1,i2+1,i3,
     & kd)+udf(i1,i2-1,i3,kd)) +(udf(i1,i2+2,i3,kd)+udf(i1,i2-2,i3,kd)
     & ) )/(dx1(1)**4)+( 8.*udf(i1,i2,i3,kd)-4.*(udf(i1+1,i2,i3,kd)+
     & udf(i1-1,i2,i3,kd)+udf(i1,i2+1,i3,kd)+udf(i1,i2-1,i3,kd))+2.*(
     & udf(i1+1,i2+1,i3,kd)+udf(i1-1,i2+1,i3,kd)+udf(i1+1,i2-1,i3,kd)+
     & udf(i1-1,i2-1,i3,kd)) )/(dx1(0)**2*dx1(1)**2)
         udfxxx23r(i1,i2,i3,kd)=(-2.*(udf(i1+1,i2,i3,kd)-udf(i1-1,i2,
     & i3,kd))+(udf(i1+2,i2,i3,kd)-udf(i1-2,i2,i3,kd)) )*dx122(0)*
     & dx112(0)
         udfyyy23r(i1,i2,i3,kd)=(-2.*(udf(i1,i2+1,i3,kd)-udf(i1,i2-1,
     & i3,kd))+(udf(i1,i2+2,i3,kd)-udf(i1,i2-2,i3,kd)) )*dx122(1)*
     & dx112(1)
         udfzzz23r(i1,i2,i3,kd)=(-2.*(udf(i1,i2,i3+1,kd)-udf(i1,i2,i3-
     & 1,kd))+(udf(i1,i2,i3+2,kd)-udf(i1,i2,i3-2,kd)) )*dx122(1)*
     & dx112(2)
         udfxxy23r(i1,i2,i3,kd)=( udfxx22r(i1,i2+1,i3,kd)-udfxx22r(i1,
     & i2-1,i3,kd))/(2.*dx1(1))
         udfxyy23r(i1,i2,i3,kd)=( udfyy22r(i1+1,i2,i3,kd)-udfyy22r(i1-
     & 1,i2,i3,kd))/(2.*dx1(0))
         udfxxz23r(i1,i2,i3,kd)=( udfxx22r(i1,i2,i3+1,kd)-udfxx22r(i1,
     & i2,i3-1,kd))/(2.*dx1(2))
         udfyyz23r(i1,i2,i3,kd)=( udfyy22r(i1,i2,i3+1,kd)-udfyy22r(i1,
     & i2,i3-1,kd))/(2.*dx1(2))
         udfxzz23r(i1,i2,i3,kd)=( udfzz22r(i1+1,i2,i3,kd)-udfzz22r(i1-
     & 1,i2,i3,kd))/(2.*dx1(0))
         udfyzz23r(i1,i2,i3,kd)=( udfzz22r(i1,i2+1,i3,kd)-udfzz22r(i1,
     & i2-1,i3,kd))/(2.*dx1(1))
         udfxxxx23r(i1,i2,i3,kd)=(6.*udf(i1,i2,i3,kd)-4.*(udf(i1+1,i2,
     & i3,kd)+udf(i1-1,i2,i3,kd))+(udf(i1+2,i2,i3,kd)+udf(i1-2,i2,i3,
     & kd)) )/(dx1(0)**4)
         udfyyyy23r(i1,i2,i3,kd)=(6.*udf(i1,i2,i3,kd)-4.*(udf(i1,i2+1,
     & i3,kd)+udf(i1,i2-1,i3,kd))+(udf(i1,i2+2,i3,kd)+udf(i1,i2-2,i3,
     & kd)) )/(dx1(1)**4)
         udfzzzz23r(i1,i2,i3,kd)=(6.*udf(i1,i2,i3,kd)-4.*(udf(i1,i2,i3+
     & 1,kd)+udf(i1,i2,i3-1,kd))+(udf(i1,i2,i3+2,kd)+udf(i1,i2,i3-2,
     & kd)) )/(dx1(2)**4)
         udfxxyy23r(i1,i2,i3,kd)=( 4.*udf(i1,i2,i3,kd)-2.*(udf(i1+1,i2,
     & i3,kd)+udf(i1-1,i2,i3,kd)+udf(i1,i2+1,i3,kd)+udf(i1,i2-1,i3,kd)
     & )+   (udf(i1+1,i2+1,i3,kd)+udf(i1-1,i2+1,i3,kd)+udf(i1+1,i2-1,
     & i3,kd)+udf(i1-1,i2-1,i3,kd)) )/(dx1(0)**2*dx1(1)**2)
         udfxxzz23r(i1,i2,i3,kd)=( 4.*udf(i1,i2,i3,kd)-2.*(udf(i1+1,i2,
     & i3,kd)+udf(i1-1,i2,i3,kd)+udf(i1,i2,i3+1,kd)+udf(i1,i2,i3-1,kd)
     & )+   (udf(i1+1,i2,i3+1,kd)+udf(i1-1,i2,i3+1,kd)+udf(i1+1,i2,i3-
     & 1,kd)+udf(i1-1,i2,i3-1,kd)) )/(dx1(0)**2*dx1(2)**2)
         udfyyzz23r(i1,i2,i3,kd)=( 4.*udf(i1,i2,i3,kd)-2.*(udf(i1,i2+1,
     & i3,kd)  +udf(i1,i2-1,i3,kd)+  udf(i1,i2  ,i3+1,kd)+udf(i1,i2  ,
     & i3-1,kd))+   (udf(i1,i2+1,i3+1,kd)+udf(i1,i2-1,i3+1,kd)+udf(i1,
     & i2+1,i3-1,kd)+udf(i1,i2-1,i3-1,kd)) )/(dx1(1)**2*dx1(2)**2)
         dr114(kd) = 1./(12.*dr1(kd))
         dr124(kd) = 1./(12.*dr1(kd)**2)
         udfr4(i1,i2,i3,kd)=(8.*(udf(i1+1,i2,i3,kd)-udf(i1-1,i2,i3,kd))
     & -(udf(i1+2,i2,i3,kd)-udf(i1-2,i2,i3,kd)))*dr114(0)
         udfs4(i1,i2,i3,kd)=(8.*(udf(i1,i2+1,i3,kd)-udf(i1,i2-1,i3,kd))
     & -(udf(i1,i2+2,i3,kd)-udf(i1,i2-2,i3,kd)))*dr114(1)
         udft4(i1,i2,i3,kd)=(8.*(udf(i1,i2,i3+1,kd)-udf(i1,i2,i3-1,kd))
     & -(udf(i1,i2,i3+2,kd)-udf(i1,i2,i3-2,kd)))*dr114(2)
         udfrr4(i1,i2,i3,kd)=(-30.*udf(i1,i2,i3,kd)+16.*(udf(i1+1,i2,
     & i3,kd)+udf(i1-1,i2,i3,kd))-(udf(i1+2,i2,i3,kd)+udf(i1-2,i2,i3,
     & kd)) )*dr124(0)
         udfss4(i1,i2,i3,kd)=(-30.*udf(i1,i2,i3,kd)+16.*(udf(i1,i2+1,
     & i3,kd)+udf(i1,i2-1,i3,kd))-(udf(i1,i2+2,i3,kd)+udf(i1,i2-2,i3,
     & kd)) )*dr124(1)
         udftt4(i1,i2,i3,kd)=(-30.*udf(i1,i2,i3,kd)+16.*(udf(i1,i2,i3+
     & 1,kd)+udf(i1,i2,i3-1,kd))-(udf(i1,i2,i3+2,kd)+udf(i1,i2,i3-2,
     & kd)) )*dr124(2)
         udfrs4(i1,i2,i3,kd)=(8.*(udfr4(i1,i2+1,i3,kd)-udfr4(i1,i2-1,
     & i3,kd))-(udfr4(i1,i2+2,i3,kd)-udfr4(i1,i2-2,i3,kd)))*dr114(1)
         udfrt4(i1,i2,i3,kd)=(8.*(udfr4(i1,i2,i3+1,kd)-udfr4(i1,i2,i3-
     & 1,kd))-(udfr4(i1,i2,i3+2,kd)-udfr4(i1,i2,i3-2,kd)))*dr114(2)
         udfst4(i1,i2,i3,kd)=(8.*(udfs4(i1,i2,i3+1,kd)-udfs4(i1,i2,i3-
     & 1,kd))-(udfs4(i1,i2,i3+2,kd)-udfs4(i1,i2,i3-2,kd)))*dr114(2)
         rsxy1r4(i1,i2,i3,m,n)=(8.*(rsxy1(i1+1,i2,i3,m,n)-rsxy1(i1-1,
     & i2,i3,m,n))-(rsxy1(i1+2,i2,i3,m,n)-rsxy1(i1-2,i2,i3,m,n)))*
     & dr114(0)
         rsxy1s4(i1,i2,i3,m,n)=(8.*(rsxy1(i1,i2+1,i3,m,n)-rsxy1(i1,i2-
     & 1,i3,m,n))-(rsxy1(i1,i2+2,i3,m,n)-rsxy1(i1,i2-2,i3,m,n)))*
     & dr114(1)
         rsxy1t4(i1,i2,i3,m,n)=(8.*(rsxy1(i1,i2,i3+1,m,n)-rsxy1(i1,i2,
     & i3-1,m,n))-(rsxy1(i1,i2,i3+2,m,n)-rsxy1(i1,i2,i3-2,m,n)))*
     & dr114(2)
         udfx41(i1,i2,i3,kd)= rsxy1(i1,i2,i3,0,0)*udfr4(i1,i2,i3,kd)
         udfy41(i1,i2,i3,kd)=0
         udfz41(i1,i2,i3,kd)=0
         udfx42(i1,i2,i3,kd)= rsxy1(i1,i2,i3,0,0)*udfr4(i1,i2,i3,kd)+
     & rsxy1(i1,i2,i3,1,0)*udfs4(i1,i2,i3,kd)
         udfy42(i1,i2,i3,kd)= rsxy1(i1,i2,i3,0,1)*udfr4(i1,i2,i3,kd)+
     & rsxy1(i1,i2,i3,1,1)*udfs4(i1,i2,i3,kd)
         udfz42(i1,i2,i3,kd)=0
         udfx43(i1,i2,i3,kd)=rsxy1(i1,i2,i3,0,0)*udfr4(i1,i2,i3,kd)+
     & rsxy1(i1,i2,i3,1,0)*udfs4(i1,i2,i3,kd)+rsxy1(i1,i2,i3,2,0)*
     & udft4(i1,i2,i3,kd)
         udfy43(i1,i2,i3,kd)=rsxy1(i1,i2,i3,0,1)*udfr4(i1,i2,i3,kd)+
     & rsxy1(i1,i2,i3,1,1)*udfs4(i1,i2,i3,kd)+rsxy1(i1,i2,i3,2,1)*
     & udft4(i1,i2,i3,kd)
         udfz43(i1,i2,i3,kd)=rsxy1(i1,i2,i3,0,2)*udfr4(i1,i2,i3,kd)+
     & rsxy1(i1,i2,i3,1,2)*udfs4(i1,i2,i3,kd)+rsxy1(i1,i2,i3,2,2)*
     & udft4(i1,i2,i3,kd)
         rsxy1x41(i1,i2,i3,m,n)= rsxy1(i1,i2,i3,0,0)*rsxy1r4(i1,i2,i3,
     & m,n)
         rsxy1x42(i1,i2,i3,m,n)= rsxy1(i1,i2,i3,0,0)*rsxy1r4(i1,i2,i3,
     & m,n)+rsxy1(i1,i2,i3,1,0)*rsxy1s4(i1,i2,i3,m,n)
         rsxy1y42(i1,i2,i3,m,n)= rsxy1(i1,i2,i3,0,1)*rsxy1r4(i1,i2,i3,
     & m,n)+rsxy1(i1,i2,i3,1,1)*rsxy1s4(i1,i2,i3,m,n)
         rsxy1x43(i1,i2,i3,m,n)=rsxy1(i1,i2,i3,0,0)*rsxy1r4(i1,i2,i3,m,
     & n)+rsxy1(i1,i2,i3,1,0)*rsxy1s4(i1,i2,i3,m,n)+rsxy1(i1,i2,i3,2,
     & 0)*rsxy1t4(i1,i2,i3,m,n)
         rsxy1y43(i1,i2,i3,m,n)=rsxy1(i1,i2,i3,0,1)*rsxy1r4(i1,i2,i3,m,
     & n)+rsxy1(i1,i2,i3,1,1)*rsxy1s4(i1,i2,i3,m,n)+rsxy1(i1,i2,i3,2,
     & 1)*rsxy1t4(i1,i2,i3,m,n)
         rsxy1z43(i1,i2,i3,m,n)=rsxy1(i1,i2,i3,0,2)*rsxy1r4(i1,i2,i3,m,
     & n)+rsxy1(i1,i2,i3,1,2)*rsxy1s4(i1,i2,i3,m,n)+rsxy1(i1,i2,i3,2,
     & 2)*rsxy1t4(i1,i2,i3,m,n)
         udfxx41(i1,i2,i3,kd)=(rsxy1(i1,i2,i3,0,0)**2)*udfrr4(i1,i2,i3,
     & kd)+(rsxy1x42(i1,i2,i3,0,0))*udfr4(i1,i2,i3,kd)
         udfyy41(i1,i2,i3,kd)=0
         udfxy41(i1,i2,i3,kd)=0
         udfxz41(i1,i2,i3,kd)=0
         udfyz41(i1,i2,i3,kd)=0
         udfzz41(i1,i2,i3,kd)=0
         udflaplacian41(i1,i2,i3,kd)=udfxx41(i1,i2,i3,kd)
         udfxx42(i1,i2,i3,kd)=(rsxy1(i1,i2,i3,0,0)**2)*udfrr4(i1,i2,i3,
     & kd)+2.*(rsxy1(i1,i2,i3,0,0)*rsxy1(i1,i2,i3,1,0))*udfrs4(i1,i2,
     & i3,kd)+(rsxy1(i1,i2,i3,1,0)**2)*udfss4(i1,i2,i3,kd)+(rsxy1x42(
     & i1,i2,i3,0,0))*udfr4(i1,i2,i3,kd)+(rsxy1x42(i1,i2,i3,1,0))*
     & udfs4(i1,i2,i3,kd)
         udfyy42(i1,i2,i3,kd)=(rsxy1(i1,i2,i3,0,1)**2)*udfrr4(i1,i2,i3,
     & kd)+2.*(rsxy1(i1,i2,i3,0,1)*rsxy1(i1,i2,i3,1,1))*udfrs4(i1,i2,
     & i3,kd)+(rsxy1(i1,i2,i3,1,1)**2)*udfss4(i1,i2,i3,kd)+(rsxy1y42(
     & i1,i2,i3,0,1))*udfr4(i1,i2,i3,kd)+(rsxy1y42(i1,i2,i3,1,1))*
     & udfs4(i1,i2,i3,kd)
         udfxy42(i1,i2,i3,kd)=rsxy1(i1,i2,i3,0,0)*rsxy1(i1,i2,i3,0,1)*
     & udfrr4(i1,i2,i3,kd)+(rsxy1(i1,i2,i3,0,0)*rsxy1(i1,i2,i3,1,1)+
     & rsxy1(i1,i2,i3,0,1)*rsxy1(i1,i2,i3,1,0))*udfrs4(i1,i2,i3,kd)+
     & rsxy1(i1,i2,i3,1,0)*rsxy1(i1,i2,i3,1,1)*udfss4(i1,i2,i3,kd)+
     & rsxy1x42(i1,i2,i3,0,1)*udfr4(i1,i2,i3,kd)+rsxy1x42(i1,i2,i3,1,
     & 1)*udfs4(i1,i2,i3,kd)
         udfxz42(i1,i2,i3,kd)=0
         udfyz42(i1,i2,i3,kd)=0
         udfzz42(i1,i2,i3,kd)=0
         udflaplacian42(i1,i2,i3,kd)=(rsxy1(i1,i2,i3,0,0)**2+rsxy1(i1,
     & i2,i3,0,1)**2)*udfrr4(i1,i2,i3,kd)+2.*(rsxy1(i1,i2,i3,0,0)*
     & rsxy1(i1,i2,i3,1,0)+ rsxy1(i1,i2,i3,0,1)*rsxy1(i1,i2,i3,1,1))*
     & udfrs4(i1,i2,i3,kd)+(rsxy1(i1,i2,i3,1,0)**2+rsxy1(i1,i2,i3,1,1)
     & **2)*udfss4(i1,i2,i3,kd)+(rsxy1x42(i1,i2,i3,0,0)+rsxy1y42(i1,
     & i2,i3,0,1))*udfr4(i1,i2,i3,kd)+(rsxy1x42(i1,i2,i3,1,0)+
     & rsxy1y42(i1,i2,i3,1,1))*udfs4(i1,i2,i3,kd)
         udfxx43(i1,i2,i3,kd)=rsxy1(i1,i2,i3,0,0)**2*udfrr4(i1,i2,i3,
     & kd)+rsxy1(i1,i2,i3,1,0)**2*udfss4(i1,i2,i3,kd)+rsxy1(i1,i2,i3,
     & 2,0)**2*udftt4(i1,i2,i3,kd)+2.*rsxy1(i1,i2,i3,0,0)*rsxy1(i1,i2,
     & i3,1,0)*udfrs4(i1,i2,i3,kd)+2.*rsxy1(i1,i2,i3,0,0)*rsxy1(i1,i2,
     & i3,2,0)*udfrt4(i1,i2,i3,kd)+2.*rsxy1(i1,i2,i3,1,0)*rsxy1(i1,i2,
     & i3,2,0)*udfst4(i1,i2,i3,kd)+rsxy1x43(i1,i2,i3,0,0)*udfr4(i1,i2,
     & i3,kd)+rsxy1x43(i1,i2,i3,1,0)*udfs4(i1,i2,i3,kd)+rsxy1x43(i1,
     & i2,i3,2,0)*udft4(i1,i2,i3,kd)
         udfyy43(i1,i2,i3,kd)=rsxy1(i1,i2,i3,0,1)**2*udfrr4(i1,i2,i3,
     & kd)+rsxy1(i1,i2,i3,1,1)**2*udfss4(i1,i2,i3,kd)+rsxy1(i1,i2,i3,
     & 2,1)**2*udftt4(i1,i2,i3,kd)+2.*rsxy1(i1,i2,i3,0,1)*rsxy1(i1,i2,
     & i3,1,1)*udfrs4(i1,i2,i3,kd)+2.*rsxy1(i1,i2,i3,0,1)*rsxy1(i1,i2,
     & i3,2,1)*udfrt4(i1,i2,i3,kd)+2.*rsxy1(i1,i2,i3,1,1)*rsxy1(i1,i2,
     & i3,2,1)*udfst4(i1,i2,i3,kd)+rsxy1y43(i1,i2,i3,0,1)*udfr4(i1,i2,
     & i3,kd)+rsxy1y43(i1,i2,i3,1,1)*udfs4(i1,i2,i3,kd)+rsxy1y43(i1,
     & i2,i3,2,1)*udft4(i1,i2,i3,kd)
         udfzz43(i1,i2,i3,kd)=rsxy1(i1,i2,i3,0,2)**2*udfrr4(i1,i2,i3,
     & kd)+rsxy1(i1,i2,i3,1,2)**2*udfss4(i1,i2,i3,kd)+rsxy1(i1,i2,i3,
     & 2,2)**2*udftt4(i1,i2,i3,kd)+2.*rsxy1(i1,i2,i3,0,2)*rsxy1(i1,i2,
     & i3,1,2)*udfrs4(i1,i2,i3,kd)+2.*rsxy1(i1,i2,i3,0,2)*rsxy1(i1,i2,
     & i3,2,2)*udfrt4(i1,i2,i3,kd)+2.*rsxy1(i1,i2,i3,1,2)*rsxy1(i1,i2,
     & i3,2,2)*udfst4(i1,i2,i3,kd)+rsxy1z43(i1,i2,i3,0,2)*udfr4(i1,i2,
     & i3,kd)+rsxy1z43(i1,i2,i3,1,2)*udfs4(i1,i2,i3,kd)+rsxy1z43(i1,
     & i2,i3,2,2)*udft4(i1,i2,i3,kd)
         udfxy43(i1,i2,i3,kd)=rsxy1(i1,i2,i3,0,0)*rsxy1(i1,i2,i3,0,1)*
     & udfrr4(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*rsxy1(i1,i2,i3,1,1)*
     & udfss4(i1,i2,i3,kd)+rsxy1(i1,i2,i3,2,0)*rsxy1(i1,i2,i3,2,1)*
     & udftt4(i1,i2,i3,kd)+(rsxy1(i1,i2,i3,0,0)*rsxy1(i1,i2,i3,1,1)+
     & rsxy1(i1,i2,i3,0,1)*rsxy1(i1,i2,i3,1,0))*udfrs4(i1,i2,i3,kd)+(
     & rsxy1(i1,i2,i3,0,0)*rsxy1(i1,i2,i3,2,1)+rsxy1(i1,i2,i3,0,1)*
     & rsxy1(i1,i2,i3,2,0))*udfrt4(i1,i2,i3,kd)+(rsxy1(i1,i2,i3,1,0)*
     & rsxy1(i1,i2,i3,2,1)+rsxy1(i1,i2,i3,1,1)*rsxy1(i1,i2,i3,2,0))*
     & udfst4(i1,i2,i3,kd)+rsxy1x43(i1,i2,i3,0,1)*udfr4(i1,i2,i3,kd)+
     & rsxy1x43(i1,i2,i3,1,1)*udfs4(i1,i2,i3,kd)+rsxy1x43(i1,i2,i3,2,
     & 1)*udft4(i1,i2,i3,kd)
         udfxz43(i1,i2,i3,kd)=rsxy1(i1,i2,i3,0,0)*rsxy1(i1,i2,i3,0,2)*
     & udfrr4(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,0)*rsxy1(i1,i2,i3,1,2)*
     & udfss4(i1,i2,i3,kd)+rsxy1(i1,i2,i3,2,0)*rsxy1(i1,i2,i3,2,2)*
     & udftt4(i1,i2,i3,kd)+(rsxy1(i1,i2,i3,0,0)*rsxy1(i1,i2,i3,1,2)+
     & rsxy1(i1,i2,i3,0,2)*rsxy1(i1,i2,i3,1,0))*udfrs4(i1,i2,i3,kd)+(
     & rsxy1(i1,i2,i3,0,0)*rsxy1(i1,i2,i3,2,2)+rsxy1(i1,i2,i3,0,2)*
     & rsxy1(i1,i2,i3,2,0))*udfrt4(i1,i2,i3,kd)+(rsxy1(i1,i2,i3,1,0)*
     & rsxy1(i1,i2,i3,2,2)+rsxy1(i1,i2,i3,1,2)*rsxy1(i1,i2,i3,2,0))*
     & udfst4(i1,i2,i3,kd)+rsxy1x43(i1,i2,i3,0,2)*udfr4(i1,i2,i3,kd)+
     & rsxy1x43(i1,i2,i3,1,2)*udfs4(i1,i2,i3,kd)+rsxy1x43(i1,i2,i3,2,
     & 2)*udft4(i1,i2,i3,kd)
         udfyz43(i1,i2,i3,kd)=rsxy1(i1,i2,i3,0,1)*rsxy1(i1,i2,i3,0,2)*
     & udfrr4(i1,i2,i3,kd)+rsxy1(i1,i2,i3,1,1)*rsxy1(i1,i2,i3,1,2)*
     & udfss4(i1,i2,i3,kd)+rsxy1(i1,i2,i3,2,1)*rsxy1(i1,i2,i3,2,2)*
     & udftt4(i1,i2,i3,kd)+(rsxy1(i1,i2,i3,0,1)*rsxy1(i1,i2,i3,1,2)+
     & rsxy1(i1,i2,i3,0,2)*rsxy1(i1,i2,i3,1,1))*udfrs4(i1,i2,i3,kd)+(
     & rsxy1(i1,i2,i3,0,1)*rsxy1(i1,i2,i3,2,2)+rsxy1(i1,i2,i3,0,2)*
     & rsxy1(i1,i2,i3,2,1))*udfrt4(i1,i2,i3,kd)+(rsxy1(i1,i2,i3,1,1)*
     & rsxy1(i1,i2,i3,2,2)+rsxy1(i1,i2,i3,1,2)*rsxy1(i1,i2,i3,2,1))*
     & udfst4(i1,i2,i3,kd)+rsxy1y43(i1,i2,i3,0,2)*udfr4(i1,i2,i3,kd)+
     & rsxy1y43(i1,i2,i3,1,2)*udfs4(i1,i2,i3,kd)+rsxy1y43(i1,i2,i3,2,
     & 2)*udft4(i1,i2,i3,kd)
         udflaplacian43(i1,i2,i3,kd)=(rsxy1(i1,i2,i3,0,0)**2+rsxy1(i1,
     & i2,i3,0,1)**2+rsxy1(i1,i2,i3,0,2)**2)*udfrr4(i1,i2,i3,kd)+(
     & rsxy1(i1,i2,i3,1,0)**2+rsxy1(i1,i2,i3,1,1)**2+rsxy1(i1,i2,i3,1,
     & 2)**2)*udfss4(i1,i2,i3,kd)+(rsxy1(i1,i2,i3,2,0)**2+rsxy1(i1,i2,
     & i3,2,1)**2+rsxy1(i1,i2,i3,2,2)**2)*udftt4(i1,i2,i3,kd)+2.*(
     & rsxy1(i1,i2,i3,0,0)*rsxy1(i1,i2,i3,1,0)+ rsxy1(i1,i2,i3,0,1)*
     & rsxy1(i1,i2,i3,1,1)+rsxy1(i1,i2,i3,0,2)*rsxy1(i1,i2,i3,1,2))*
     & udfrs4(i1,i2,i3,kd)+2.*(rsxy1(i1,i2,i3,0,0)*rsxy1(i1,i2,i3,2,0)
     & + rsxy1(i1,i2,i3,0,1)*rsxy1(i1,i2,i3,2,1)+rsxy1(i1,i2,i3,0,2)*
     & rsxy1(i1,i2,i3,2,2))*udfrt4(i1,i2,i3,kd)+2.*(rsxy1(i1,i2,i3,1,
     & 0)*rsxy1(i1,i2,i3,2,0)+ rsxy1(i1,i2,i3,1,1)*rsxy1(i1,i2,i3,2,1)
     & +rsxy1(i1,i2,i3,1,2)*rsxy1(i1,i2,i3,2,2))*udfst4(i1,i2,i3,kd)+(
     & rsxy1x43(i1,i2,i3,0,0)+rsxy1y43(i1,i2,i3,0,1)+rsxy1z43(i1,i2,
     & i3,0,2))*udfr4(i1,i2,i3,kd)+(rsxy1x43(i1,i2,i3,1,0)+rsxy1y43(
     & i1,i2,i3,1,1)+rsxy1z43(i1,i2,i3,1,2))*udfs4(i1,i2,i3,kd)+(
     & rsxy1x43(i1,i2,i3,2,0)+rsxy1y43(i1,i2,i3,2,1)+rsxy1z43(i1,i2,
     & i3,2,2))*udft4(i1,i2,i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
         dx141(kd) = 1./(12.*dx1(kd))
         dx142(kd) = 1./(12.*dx1(kd)**2)
         udfx43r(i1,i2,i3,kd)=(8.*(udf(i1+1,i2,i3,kd)-udf(i1-1,i2,i3,
     & kd))-(udf(i1+2,i2,i3,kd)-udf(i1-2,i2,i3,kd)))*dx141(0)
         udfy43r(i1,i2,i3,kd)=(8.*(udf(i1,i2+1,i3,kd)-udf(i1,i2-1,i3,
     & kd))-(udf(i1,i2+2,i3,kd)-udf(i1,i2-2,i3,kd)))*dx141(1)
         udfz43r(i1,i2,i3,kd)=(8.*(udf(i1,i2,i3+1,kd)-udf(i1,i2,i3-1,
     & kd))-(udf(i1,i2,i3+2,kd)-udf(i1,i2,i3-2,kd)))*dx141(2)
         udfxx43r(i1,i2,i3,kd)=( -30.*udf(i1,i2,i3,kd)+16.*(udf(i1+1,
     & i2,i3,kd)+udf(i1-1,i2,i3,kd))-(udf(i1+2,i2,i3,kd)+udf(i1-2,i2,
     & i3,kd)) )*dx142(0)
         udfyy43r(i1,i2,i3,kd)=( -30.*udf(i1,i2,i3,kd)+16.*(udf(i1,i2+
     & 1,i3,kd)+udf(i1,i2-1,i3,kd))-(udf(i1,i2+2,i3,kd)+udf(i1,i2-2,
     & i3,kd)) )*dx142(1)
         udfzz43r(i1,i2,i3,kd)=( -30.*udf(i1,i2,i3,kd)+16.*(udf(i1,i2,
     & i3+1,kd)+udf(i1,i2,i3-1,kd))-(udf(i1,i2,i3+2,kd)+udf(i1,i2,i3-
     & 2,kd)) )*dx142(2)
         udfxy43r(i1,i2,i3,kd)=( (udf(i1+2,i2+2,i3,kd)-udf(i1-2,i2+2,
     & i3,kd)- udf(i1+2,i2-2,i3,kd)+udf(i1-2,i2-2,i3,kd)) +8.*(udf(i1-
     & 1,i2+2,i3,kd)-udf(i1-1,i2-2,i3,kd)-udf(i1+1,i2+2,i3,kd)+udf(i1+
     & 1,i2-2,i3,kd) +udf(i1+2,i2-1,i3,kd)-udf(i1-2,i2-1,i3,kd)-udf(
     & i1+2,i2+1,i3,kd)+udf(i1-2,i2+1,i3,kd))+64.*(udf(i1+1,i2+1,i3,
     & kd)-udf(i1-1,i2+1,i3,kd)- udf(i1+1,i2-1,i3,kd)+udf(i1-1,i2-1,
     & i3,kd)))*(dx141(0)*dx141(1))
         udfxz43r(i1,i2,i3,kd)=( (udf(i1+2,i2,i3+2,kd)-udf(i1-2,i2,i3+
     & 2,kd)-udf(i1+2,i2,i3-2,kd)+udf(i1-2,i2,i3-2,kd)) +8.*(udf(i1-1,
     & i2,i3+2,kd)-udf(i1-1,i2,i3-2,kd)-udf(i1+1,i2,i3+2,kd)+udf(i1+1,
     & i2,i3-2,kd) +udf(i1+2,i2,i3-1,kd)-udf(i1-2,i2,i3-1,kd)- udf(i1+
     & 2,i2,i3+1,kd)+udf(i1-2,i2,i3+1,kd)) +64.*(udf(i1+1,i2,i3+1,kd)-
     & udf(i1-1,i2,i3+1,kd)-udf(i1+1,i2,i3-1,kd)+udf(i1-1,i2,i3-1,kd))
     &  )*(dx141(0)*dx141(2))
         udfyz43r(i1,i2,i3,kd)=( (udf(i1,i2+2,i3+2,kd)-udf(i1,i2-2,i3+
     & 2,kd)-udf(i1,i2+2,i3-2,kd)+udf(i1,i2-2,i3-2,kd)) +8.*(udf(i1,
     & i2-1,i3+2,kd)-udf(i1,i2-1,i3-2,kd)-udf(i1,i2+1,i3+2,kd)+udf(i1,
     & i2+1,i3-2,kd) +udf(i1,i2+2,i3-1,kd)-udf(i1,i2-2,i3-1,kd)-udf(
     & i1,i2+2,i3+1,kd)+udf(i1,i2-2,i3+1,kd)) +64.*(udf(i1,i2+1,i3+1,
     & kd)-udf(i1,i2-1,i3+1,kd)-udf(i1,i2+1,i3-1,kd)+udf(i1,i2-1,i3-1,
     & kd)) )*(dx141(1)*dx141(2))
         udfx41r(i1,i2,i3,kd)= udfx43r(i1,i2,i3,kd)
         udfy41r(i1,i2,i3,kd)= udfy43r(i1,i2,i3,kd)
         udfz41r(i1,i2,i3,kd)= udfz43r(i1,i2,i3,kd)
         udfxx41r(i1,i2,i3,kd)= udfxx43r(i1,i2,i3,kd)
         udfyy41r(i1,i2,i3,kd)= udfyy43r(i1,i2,i3,kd)
         udfzz41r(i1,i2,i3,kd)= udfzz43r(i1,i2,i3,kd)
         udfxy41r(i1,i2,i3,kd)= udfxy43r(i1,i2,i3,kd)
         udfxz41r(i1,i2,i3,kd)= udfxz43r(i1,i2,i3,kd)
         udfyz41r(i1,i2,i3,kd)= udfyz43r(i1,i2,i3,kd)
         udflaplacian41r(i1,i2,i3,kd)=udfxx43r(i1,i2,i3,kd)
         udfx42r(i1,i2,i3,kd)= udfx43r(i1,i2,i3,kd)
         udfy42r(i1,i2,i3,kd)= udfy43r(i1,i2,i3,kd)
         udfz42r(i1,i2,i3,kd)= udfz43r(i1,i2,i3,kd)
         udfxx42r(i1,i2,i3,kd)= udfxx43r(i1,i2,i3,kd)
         udfyy42r(i1,i2,i3,kd)= udfyy43r(i1,i2,i3,kd)
         udfzz42r(i1,i2,i3,kd)= udfzz43r(i1,i2,i3,kd)
         udfxy42r(i1,i2,i3,kd)= udfxy43r(i1,i2,i3,kd)
         udfxz42r(i1,i2,i3,kd)= udfxz43r(i1,i2,i3,kd)
         udfyz42r(i1,i2,i3,kd)= udfyz43r(i1,i2,i3,kd)
         udflaplacian42r(i1,i2,i3,kd)=udfxx43r(i1,i2,i3,kd)+udfyy43r(
     & i1,i2,i3,kd)
         udflaplacian43r(i1,i2,i3,kd)=udfxx43r(i1,i2,i3,kd)+udfyy43r(
     & i1,i2,i3,kd)+udfzz43r(i1,i2,i3,kd)
        !     --- end statement functions
         ierr=0
         ! write(*,*) 'Inside assignPressureRHSOpt'
         pc                   =ipar(0)
         uc                   =ipar(1)
         vc                   =ipar(2)
         wc                   =ipar(3)
         tc                   =ipar(4) ! **new**
         nc                   =ipar(5)
         grid                 =ipar(6)
         orderOfAccuracy      =ipar(7)
         gridIsMoving         =ipar(8)
         useWhereMask         =ipar(9)
         isAxisymmetric       =ipar(10)
         pressureBC           =ipar(11)
         numberOfComponents   =ipar(12)
         gridType             =ipar(13)
         turbulenceModel      =ipar(14)
         use2ndOrderAD        =ipar(15)
         use4thOrderAD        =ipar(16)
         useImplicit4thOrderAD=ipar(17)
         includeADinPressure  =ipar(18)
         pdeModel             =ipar(19) ! **new**
         vsc                  =ipar(20)
         twilightZoneFlow     =ipar(21)
         rc                   =ipar(22)
         initialConditionsAreBeingProjected=ipar(23)
         turnOnBodyForcing    =ipar(24)
         debug                =ipar(25)
         dr(0)               =rpar(0)
         dr(1)               =rpar(1)
         dr(2)               =rpar(2)
         dx(0)               =rpar(3)
         dx(1)               =rpar(4)
         dx(2)               =rpar(5)
         nu                  =rpar(6)
         advectionCoefficient=rpar(7)
         inflowPressure      =rpar(8)
         ad21                =rpar(9)
         ad22                =rpar(10)
         ad41                =rpar(11)
         ad42                =rpar(12)
         gravity(0)          =rpar(13) ! **new**
         gravity(1)          =rpar(14)
         gravity(2)          =rpar(15)
         thermalExpansivity  =rpar(16)
         adcBoussinesq       =rpar(17)
         surfaceTension      =rpar(18)
         pAtmosphere         =rpar(19)
        !  nuVP                =rpar(18) ! visco-plastic parameters
        !  etaVP               =rpar(19)
        !  yieldStressVP       =rpar(20)
        !  exponentVP          =rpar(21)
        !  epsVP               =rpar(22)
         detMin=1.e-30     ! **FIX ME**
         if( debug.gt.3 .and. surfaceTension.ne.0. )then
           write(*,'("inspf: surfaceTension=",e10.2," pAtmosphere=",
     & e10.2)') surfaceTension,pAtmosphere
         end if
         cd22=ad22/(nd**2)
         cd42=ad42/(nd**2)
         kc=nc
         ec=kc+1
         ! for non-moving grids, u=uu, and we need to multiply uu by advectionCoefficient in the pressure BC
         advectCoeff=advectionCoefficient
         if( gridIsMoving.ne.0 .and. 
     & initialConditionsAreBeingProjected.eq.0 )then
           ! For moving grids we need to multiply only u by advectionCoefficient, and mutiply by advectCoeff=1 in the pressure BC
           advectCoeff=1.
         end if
         ! for visco-plastic
         if( orderOfAccuracy.ne.2 .and. orderOfAccuracy.ne.4 )then
           write(*,'("assignPressureRHSOpt:ERROR orderOfAccuracy=",i6)
     & ') orderOfAccuracy
           stop 1
         end if
         if( gridType.ne.rectangular .and. gridType.ne.curvilinear )
     & then
           write(*,'("assignPressureRHSOpt:ERROR gridType=",i6)') 
     & gridType
           stop 2
         end if
         if( numberOfComponents.le.nd )then
           write(*,'("assignPressureRHSOpt:ERROR nd,
     & numberOfComponents=",2i6)') nd,numberOfComponents
           stop 3
         end if
         if( uc.lt.0 .or. vc.lt.0 .or. (nd.eq.3 .and. wc.lt.0) )then
           write(*,'("assignPressureRHSOpt:ERROR uc,vc,ws=",2i6)') uc,
     & vc,wc
           stop 4
         end if
        if( gridType.eq.rectangular )then
              ! user defined force:
           if( useWhereMask.ne.0 )then
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
               if( mask(i1,i2,i3).ne.0 )then
                    u0x=ux22r(i1,i2,i3,uc)
                    u0y=uy22r(i1,i2,i3,uc)
                    v0x=ux22r(i1,i2,i3,vc)
                    v0y=uy22r(i1,i2,i3,vc)
                    u0Lap=ulaplacian22r(i1,i2,i3,uc)
                    v0Lap=ulaplacian22r(i1,i2,i3,vc)
                    nuT = u(i1,i2,i3,vsc)
                    nuTx=ux22r(i1,i2,i3,vsc)
                    nuTy=uy22r(i1,i2,i3,vsc)
                    nuTxx=uxx22r(i1,i2,i3,vsc)
                    nuTxy=uxy22r(i1,i2,i3,vsc)
                    nuTyy=uyy22r(i1,i2,i3,vsc)
                 f(i1,i2,i3)=(-advectionCoefficient)*(u0x**2+2.*u0y*
     & v0x+v0y**2)+divDamping(i1,i2,i3)*(u0x+v0y)+2.*(nuTx*u0Lap+
     & nuTxx*u0x+nuTxy*u0y+nuTy*v0Lap+nuTxy*v0x+nuTyy*v0y)


               end if
             end do
             end do
             end do
           else
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
                    u0x=ux22r(i1,i2,i3,uc)
                    u0y=uy22r(i1,i2,i3,uc)
                    v0x=ux22r(i1,i2,i3,vc)
                    v0y=uy22r(i1,i2,i3,vc)
                    u0Lap=ulaplacian22r(i1,i2,i3,uc)
                    v0Lap=ulaplacian22r(i1,i2,i3,vc)
                    nuT = u(i1,i2,i3,vsc)
                    nuTx=ux22r(i1,i2,i3,vsc)
                    nuTy=uy22r(i1,i2,i3,vsc)
                    nuTxx=uxx22r(i1,i2,i3,vsc)
                    nuTxy=uxy22r(i1,i2,i3,vsc)
                    nuTyy=uyy22r(i1,i2,i3,vsc)
                 f(i1,i2,i3)=(-advectionCoefficient)*(u0x**2+2.*u0y*
     & v0x+v0y**2)+divDamping(i1,i2,i3)*(u0x+v0y)+2.*(nuTx*u0Lap+
     & nuTxx*u0x+nuTxy*u0y+nuTy*v0Lap+nuTxy*v0x+nuTyy*v0y)


             end do
             end do
             end do
           end if
          ! For the Boussinesq approximation: -alpha*( (gravity.grad) T )
          if( pdeModel.eq.BoussinesqModel .or. 
     & pdeModel.eq.viscoPlasticModel )then
            if( useWhereMask.ne.0 )then
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
                if( mask(i1,i2,i3).ne.0 )then
                  f(i1,i2,i3)=f(i1,i2,i3)-thermalExpansivity*(gravity(
     & 0)*ux22r(i1,i2,i3,tc)+gravity(1)*uy22r(i1,i2,i3,tc))
                end if
              end do
              end do
              end do
            else
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
                  f(i1,i2,i3)=f(i1,i2,i3)-thermalExpansivity*(gravity(
     & 0)*ux22r(i1,i2,i3,tc)+gravity(1)*uy22r(i1,i2,i3,tc))
              end do
              end do
              end do
            end if
          end if
          ! -- Add on the divergence of the user defined force ---
          if( turnOnBodyForcing.eq.1 )then
         !!120224 kkc gets annoying after awhile  write(*,'(" *** inspf: add divergence of the body force to the pressure RHS ***")') 
            if( useWhereMask.ne.0 )then
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
                if( mask(i1,i2,i3).ne.0 )then
                  f(i1,i2,i3)=f(i1,i2,i3)+udfx22r(i1,i2,i3,uc)+udfy22r(
     & i1,i2,i3,vc)
                end if
              end do
              end do
              end do
            else
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
                  f(i1,i2,i3)=f(i1,i2,i3)+udfx22r(i1,i2,i3,uc)+udfy22r(
     & i1,i2,i3,vc)
              end do
              end do
              end do
            end if
          end if
        else ! curvilinear
              ! user defined force:
           if( useWhereMask.ne.0 )then
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
               if( mask(i1,i2,i3).ne.0 )then
                    u0x=ux22(i1,i2,i3,uc)
                    u0y=uy22(i1,i2,i3,uc)
                    v0x=ux22(i1,i2,i3,vc)
                    v0y=uy22(i1,i2,i3,vc)
                    u0Lap=ulaplacian22(i1,i2,i3,uc)
                    v0Lap=ulaplacian22(i1,i2,i3,vc)
                    nuT = u(i1,i2,i3,vsc)
                    nuTx=ux22(i1,i2,i3,vsc)
                    nuTy=uy22(i1,i2,i3,vsc)
                    nuTxx=uxx22(i1,i2,i3,vsc)
                    nuTxy=uxy22(i1,i2,i3,vsc)
                    nuTyy=uyy22(i1,i2,i3,vsc)
                 f(i1,i2,i3)=(-advectionCoefficient)*(u0x**2+2.*u0y*
     & v0x+v0y**2)+divDamping(i1,i2,i3)*(u0x+v0y)+2.*(nuTx*u0Lap+
     & nuTxx*u0x+nuTxy*u0y+nuTy*v0Lap+nuTxy*v0x+nuTyy*v0y)


               end if
             end do
             end do
             end do
           else
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
                    u0x=ux22(i1,i2,i3,uc)
                    u0y=uy22(i1,i2,i3,uc)
                    v0x=ux22(i1,i2,i3,vc)
                    v0y=uy22(i1,i2,i3,vc)
                    u0Lap=ulaplacian22(i1,i2,i3,uc)
                    v0Lap=ulaplacian22(i1,i2,i3,vc)
                    nuT = u(i1,i2,i3,vsc)
                    nuTx=ux22(i1,i2,i3,vsc)
                    nuTy=uy22(i1,i2,i3,vsc)
                    nuTxx=uxx22(i1,i2,i3,vsc)
                    nuTxy=uxy22(i1,i2,i3,vsc)
                    nuTyy=uyy22(i1,i2,i3,vsc)
                 f(i1,i2,i3)=(-advectionCoefficient)*(u0x**2+2.*u0y*
     & v0x+v0y**2)+divDamping(i1,i2,i3)*(u0x+v0y)+2.*(nuTx*u0Lap+
     & nuTxx*u0x+nuTxy*u0y+nuTy*v0Lap+nuTxy*v0x+nuTyy*v0y)


             end do
             end do
             end do
           end if
          ! For the Boussinesq approximation: -alpha*( (gravity.grad) T )
          if( pdeModel.eq.BoussinesqModel .or. 
     & pdeModel.eq.viscoPlasticModel )then
            if( useWhereMask.ne.0 )then
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
                if( mask(i1,i2,i3).ne.0 )then
                  f(i1,i2,i3)=f(i1,i2,i3)-thermalExpansivity*(gravity(
     & 0)*ux22(i1,i2,i3,tc)+gravity(1)*uy22(i1,i2,i3,tc))
                end if
              end do
              end do
              end do
            else
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
                  f(i1,i2,i3)=f(i1,i2,i3)-thermalExpansivity*(gravity(
     & 0)*ux22(i1,i2,i3,tc)+gravity(1)*uy22(i1,i2,i3,tc))
              end do
              end do
              end do
            end if
          end if
          ! -- Add on the divergence of the user defined force ---
          if( turnOnBodyForcing.eq.1 )then
         !!120224 kkc gets annoying after awhile  write(*,'(" *** inspf: add divergence of the body force to the pressure RHS ***")') 
            if( useWhereMask.ne.0 )then
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
                if( mask(i1,i2,i3).ne.0 )then
                  f(i1,i2,i3)=f(i1,i2,i3)+udfx22(i1,i2,i3,uc)+udfy22(
     & i1,i2,i3,vc)
                end if
              end do
              end do
              end do
            else
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
                  f(i1,i2,i3)=f(i1,i2,i3)+udfx22(i1,i2,i3,uc)+udfy22(
     & i1,i2,i3,vc)
              end do
              end do
              end do
            end if
          end if
        end if
       !     ***************** assign RHS for BC ********************      
       !**      if( gridType.ne.rectangular )then
       !**         write(*,*) 'ERROR:assignPressureRHSOpt gridType.ne.rectangular'
       !**         stop 1
       !**        return
       !**      end if
        do axis=0,nd-1
         axisp1=mod(axis+1,nd)
         axisp2=mod(axis+2,nd)
         do side=0,1
          n1a=indexRange(0,0)
          n1b=indexRange(1,0)
          n2a=indexRange(0,1)
          n2b=indexRange(1,1)
          n3a=indexRange(0,2)
          n3b=indexRange(1,2)
          is1=0
          is2=0
          is3=0
          if( axis.eq.0 )then
            n1a=indexRange(side,axis)
            n1b=indexRange(side,axis)
            is1=2*side-1
          else if( axis.eq.1 )then
            n2a=indexRange(side,axis)
            n2b=indexRange(side,axis)
            is2=2*side-1
          else
            n3a=indexRange(side,axis)
            n3b=indexRange(side,axis)
            is3=2*side-1
          end if
          ! an : outward normal on a Cartesian grid
          an(0)=0.
          an(1)=0.
          an(2)=0.
          an(axis)=2*side-1
          bc0=bc(side,axis)
          if( bc0.le.0 )then
            ! do nothing
          else if( bc0.eq.outflow .or. bc0.eq.convectiveOutflow ) then
            a1=bcData(pc+numberOfComponents*2,side,axis) ! coeff of p.n
            ! write(*,*) 'pressureBC opt: pc,nc,side,axis,a1=',pc,numberOfComponents,side,axis,a1
            if( a1.ne.0. ) then
              ! printf("**apply mixed BC on pressure rhs...\n");
              ! if( addBoundaryForcing(side,axis).ne.0 .and. initialConditionsAreBeingProjected.eq.0 )then ! *wdh* 2013/12/01
              ! *wdh* 2014/11/21 - turn off RHS when projecting initial conditions:
              ! *wdh* 2016/11/25 -- make sure to use zero RHS when projecting initial conditions:
              ! if( addBoundaryForcing(side,axis).ne.0 .and. initialConditionsAreBeingProjected.eq.0 )then 
              if( addBoundaryForcing(side,axis).ne.0 )then
                if( initialConditionsAreBeingProjected.eq.0 )then
                 ! write(*,'("inspf:INFO: set pressure profile at outflow")')
                 if( useWhereMask.ne.0 )then
                   do i3=n3a,n3b
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                     if( mask(i1,i2,i3).ne.0 )then
                       f(i1+is1,i2+is2,i3+is3)=bcf(side,axis,i1,i2,i3,
     & pc)



                     end if
                   end do
                   end do
                   end do
                 else
                   do i3=n3a,n3b
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                       f(i1+is1,i2+is2,i3+is3)=bcf(side,axis,i1,i2,i3,
     & pc)



                   end do
                   end do
                   end do
                 end if
                else
                 if( useWhereMask.ne.0 )then
                   do i3=n3a,n3b
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                     if( mask(i1,i2,i3).ne.0 )then
                       f(i1+is1,i2+is2,i3+is3)=0.



                     end if
                   end do
                   end do
                   end do
                 else
                   do i3=n3a,n3b
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                       f(i1+is1,i2+is2,i3+is3)=0.



                   end do
                   end do
                   end do
                 end if
                end if
              else
               if( useWhereMask.ne.0 )then
                 do i3=n3a,n3b
                 do i2=n2a,n2b
                 do i1=n1a,n1b
                   if( mask(i1,i2,i3).ne.0 )then
                     f(i1+is1,i2+is2,i3+is3)=bcData(pc,side,axis)



                   end if
                 end do
                 end do
                 end do
               else
                 do i3=n3a,n3b
                 do i2=n2a,n2b
                 do i1=n1a,n1b
                     f(i1+is1,i2+is2,i3+is3)=bcData(pc,side,axis)



                 end do
                 end do
                 end do
               end if
              end if
            else
              ! dirichlet :
              ! if( addBoundaryForcing(side,axis).ne.0 )then ! *wdh* 2013/12/01
              ! *wdh* 2014/11/21 - turn off RHS when projecting initial conditions:
              ! *wdh* 2016/11/25 -- make sure to use zero RHS when projecting initial conditions:
              ! if( addBoundaryForcing(side,axis).ne.0 .and. initialConditionsAreBeingProjected.eq.0 )then 
              if( addBoundaryForcing(side,axis).ne.0 )then
                if( initialConditionsAreBeingProjected.eq.0 )then
                 ! write(*,'("inspf:INFO: set pressure profile at outflow")')
                 if( useWhereMask.ne.0 )then
                   do i3=n3a,n3b
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                     if( mask(i1,i2,i3).ne.0 )then
                       f(i1,i2,i3)=bcf(side,axis,i1,i2,i3,pc)



                     end if
                   end do
                   end do
                   end do
                 else
                   do i3=n3a,n3b
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                       f(i1,i2,i3)=bcf(side,axis,i1,i2,i3,pc)



                   end do
                   end do
                   end do
                 end if
                else
                 if( useWhereMask.ne.0 )then
                   do i3=n3a,n3b
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                     if( mask(i1,i2,i3).ne.0 )then
                       f(i1,i2,i3)=0.



                     end if
                   end do
                   end do
                   end do
                 else
                   do i3=n3a,n3b
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                       f(i1,i2,i3)=0.



                   end do
                   end do
                   end do
                 end if
                end if
              else
               if( useWhereMask.ne.0 )then
                 do i3=n3a,n3b
                 do i2=n2a,n2b
                 do i1=n1a,n1b
                   if( mask(i1,i2,i3).ne.0 )then
                     f(i1,i2,i3)=bcData(pc,side,axis)



                   end if
                 end do
                 end do
                 end do
               else
                 do i3=n3a,n3b
                 do i2=n2a,n2b
                 do i1=n1a,n1b
                     f(i1,i2,i3)=bcData(pc,side,axis)



                 end do
                 end do
                 end do
               end if
              end if
              ! for extrapolation :
              if( useWhereMask.ne.0 )then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                  if( mask(i1,i2,i3).ne.0 )then
                    f(i1+is1,i2+is2,i3+is3)=0.



                  end if
                end do
                end do
                end do
              else
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                    f(i1+is1,i2+is2,i3+is3)=0.



                end do
                end do
                end do
              end if
            end if
          else if( bc0.eq.inflowWithPandTV .or.     
     & bc0.eq.dirichletBoundaryCondition )then
            if( addBoundaryForcing(side,axis).ne.0 )then ! *wdh* 2013/12/01
             if( useWhereMask.ne.0 )then
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                 if( mask(i1,i2,i3).ne.0 )then
                   f(i1,i2,i3)=bcf(side,axis,i1,i2,i3,pc)



                 end if
               end do
               end do
               end do
             else
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                   f(i1,i2,i3)=bcf(side,axis,i1,i2,i3,pc)



               end do
               end do
               end do
             end if
            else
             inflowPressure=bcData(pc,side,axis) ! *wdh* 100809
             if( useWhereMask.ne.0 )then
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                 if( mask(i1,i2,i3).ne.0 )then
                   f(i1,i2,i3)=inflowPressure



                 end if
               end do
               end do
               end do
             else
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                   f(i1,i2,i3)=inflowPressure



               end do
               end do
               end do
             end if
            end if
          else if( bc0.eq.freeSurfaceBoundaryCondition .or. 
     & bc0.eq.tractionFree )then
            ! Free surface and tractionFree are really the same thing *wdh* 2014/12/17
              ! The free surface BC for pressure is
              !   p = p_a - n.sigma.n - surfaceTension * 2 *H 
              !   H = mean-curvature = .5( 1/R_1 + 1/R_2)
              !       2 H = - div( normal )
              !
              if( addBoundaryForcing(side,axis).ne.0 .and. 
     & initialConditionsAreBeingProjected.eq.0 )then
                write(*,'(" --inspf-- add RHS to traction (or free 
     & surface) BC")')
              end if
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
               if( mask(i1,i2,i3).ne.0 )then
                 ! FINISH ME -- ADD viscous stress contribution:
                 ! -- Compute the mean curvature --
                 !   Note: curvature is zero on a rectangular grid, so skip this part:
                 if( surfaceTension.ne.0. .and. 
     & gridType.ne.rectangular )then
                  if( nd.eq.2 )then
                    rxi= rsxy(i1,i2,i3,0,0)
                    ryi= rsxy(i1,i2,i3,0,1)
                    sxi= rsxy(i1,i2,i3,1,0)
                    syi= rsxy(i1,i2,i3,1,1)
                    det = rxi*syi-sxi*ryi
                    deti=1./max( detMin, det )
                    xr = syi * deti
                    yr =-sxi * deti
                    xs =-ryi * deti
                    ys = rxi * deti
                    if( axis.eq.0 )then
                      ! left or right side: tangential direction is "s"
                      rxs = rsxys2(i1,i2,i3,0,0)
                      rys = rsxys2(i1,i2,i3,0,1)
                      sxs = rsxys2(i1,i2,i3,1,0)
                      sys = rsxys2(i1,i2,i3,1,1)
                      dets = rxs*syi + rxi*sys - sxs*ryi - sxi*rys
                      xss = (-rys*det + ryi*dets )*( deti**2 )
                      yss = ( rxs*det - rxi*dets )*( deti**2 )
                      meanCurvature = -.5*( xs*yss - ys*xss )/( (xs**2 
     & + ys**2)**(1.5) )
                  write(*,'(" i1,i2=",2i3," meanCurvature=",f6.2)') i1,
     & i2,meanCurvature
                    else if( axis.eq.1 )then
                      ! top or bottom side : tangential direction is "r"
                      rxr = rsxyr2(i1,i2,i3,0,0)
                      ryr = rsxyr2(i1,i2,i3,0,1)
                      sxr = rsxyr2(i1,i2,i3,1,0)
                      syr = rsxyr2(i1,i2,i3,1,1)
                      detr = rxr*syi + rxi*syr - sxr*ryi - sxi*ryr
                      xrr = ( syr*det - syi*detr )*( deti**2 )
                      yrr = (-sxr*det + sxi*detr )*( deti**2 )
                      meanCurvature = -.5*( xr*yrr - yr*xrr )/( (xr**2 
     & + yr**2)**(1.5) )
                  ! write(*,'(" i1,i2=",2i3," meanCurvature=",f6.2,)') i1,i2,meanCurvature
                    else
                      stop 1009
                    end if
                  else if( nd.eq.3 )then
                    ! finish me 
                    stop 8256
                  else
                    stop 8257
                  end if
                  f(i1,i2,i3)= pAtmosphere - 2.*surfaceTension*
     & meanCurvature
                else
                  ! surfaceTension==0 : 
                  f(i1,i2,i3)= pAtmosphere
                end if
                 ! 2014/12/17 -- add forcing to traction BC
                 if( addBoundaryForcing(side,axis).ne.0 .and. 
     & initialConditionsAreBeingProjected.eq.0 )then
                    f(i1,i2,i3)= f(i1,i2,i3) + bcf(side,axis,i1,i2,i3,
     & pc)
                 end if
               end if ! end if mask
              end do
              end do
              end do
          else if( bc0.eq.symmetry .or. bc0.eq.axisymmetric .or. 
     & pressureBC.eq.2 ) then
             !  p.n=0
            if( useWhereMask.ne.0 )then
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
                if( mask(i1,i2,i3).ne.0 )then
                  f(i1+is1,i2+is2,i3+is3)=0.



                end if
              end do
              end do
              end do
            else
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
                  f(i1+is1,i2+is2,i3+is3)=0.



              end do
              end do
              end do
            end if
          else
       !         ********* wall condition *********
            ! if( (includeADinPressure.eq.1) )then
            !   if( use2ndOrderAD.eq.1 .and. use4thOrderAD.eq.0 ) then
            !     applyBcByGridType(INSVP,AD2,2,2)
            !   else if( use2ndOrderAD.eq.0 .and. use4thOrderAD.eq.1 ) then
            !     applyBcByGridType(INSVP,AD4,2,2)
            !   else if( use2ndOrderAD.eq.1 .and. use4thOrderAD.eq.1 ) then
            !     applyBcByGridType(INSVP,AD24,2,2)
            !   end if
            ! end if
            ! if( (includeADinPressure.eq.0) .or. (use2ndOrderAD.eq.0 .and. use4thOrderAD.eq.0 ) )then
                if( gridType.eq.rectangular )then
                         ! user defined force:
                   if( side.eq.0. .and. axis.eq.0 )then
                       ! Use the curl-curl form of the equations
                         do i3=n3a,n3b
                         do i2=n2a,n2b
                         do i1=n1a,n1b
                           if( mask(i1,i2,i3).ne.0 )then
                          ! Define derivative macros before calling this macro
                          ! By default there is no AD:
                            ! get nuT,nuTx,nuTy,nuTz
                                 nuT = u(i1,i2,i3,vsc)
                                 nuTx=ux22r(i1,i2,i3,vsc)
                                 nuTy=uy22r(i1,i2,i3,vsc)
                                ! curl-curl form of the diffusion operator in 2D
                          ! Artificial Dissipation  **todo** implicit-line, self-adjoint versions
                          ! Here now is the statement where the ghost line value in the RHS is assigned.
                              f(i1+is1,i2,i3)=(2*side-1)*( (nuT*(-
     & uxy22r(i1,i2,i3,vc)+uyy22r(i1,i2,i3,uc))-2.*nuTx*(uy22r(i1,i2,
     & i3,vc))+nuTy*(uy22r(i1,i2,i3,uc)+ux22r(i1,i2,i3,vc)))-(
     & advectCoeff*(uu(i1,i2,i3,uc)*ux22r(i1,i2,i3,uc)+uu(i1,i2,i3,vc)
     & *uy22r(i1,i2,i3,uc)))  )
                           end if
                         end do
                         end do
                         end do
                       ! For the Boussinesq approximation:  -alpha*T* normal00.gravity
                       if( pdeModel.eq.BoussinesqModel .or. 
     & pdeModel.eq.viscoPlasticModel )then
                           do i3=n3a,n3b
                           do i2=n2a,n2b
                           do i1=n1a,n1b
                             if( mask(i1,i2,i3).ne.0 )then
                            f(i1+is1,i2+is2,i3+is3)=f(i1+is1,i2+is2,i3+
     & is3)-(2*side-1)*thermalExpansivity*u(i1,i2,i3,tc)*gravity(axis)
                             end if
                           end do
                           end do
                           end do
                       else if( pdeModel.eq.twoPhaseFlowModel )then
                           do i3=n3a,n3b
                           do i2=n2a,n2b
                           do i1=n1a,n1b
                             if( mask(i1,i2,i3).ne.0 )then
                            f(i1+is1,i2+is2,i3+is3)=f(i1+is1,i2+is2,i3+
     & is3)+(2*side-1)*u(i1,i2,i3,rc)*gravity(axis)
                             end if
                           end do
                           end do
                           end do
                       end if
                       ! -- include contribution to BC from the body force --- *wdh* 2012/07/06
                       if( turnOnBodyForcing.eq.1 )then
                         ! write(*,'(" *** inspf: add body force to pressure BC ***")') 
                           do i3=n3a,n3b
                           do i2=n2a,n2b
                           do i1=n1a,n1b
                             if( mask(i1,i2,i3).ne.0 )then
                             f(i1+is1,i2+is2,i3+is3)=f(i1+is1,i2+is2,
     & i3+is3)+(2*side-1)*udf(i1,i2,i3,uc+axis)
                             end if
                           end do
                           end do
                           end do
                       end if
                       ! end points that meet a wall are treated differently to make sure p.x=0 where it should be
                       ! --> do not use the curl-curl boundary condition
                       do kd=1,nd-1 ! loop over the two-tangential directions
                        axisp=mod(axis+kd,nd)
                        do sidep1=0,1
                         ! For now: do not apply the corner correction for TZ flow (we need to fix TZ forcing for this case)
                         if( bc(sidep1,axisp)
     & .eq.inflowWithVelocityGiven .or. bc(sidep1,axisp)
     & .eq.noSlipWall .and. twilightZoneFlow.eq.0 )then
                           if( axisp.eq.0 )then
                             n1a=indexRange(sidep1,0)
                             n1b=indexRange(sidep1,0)
                           else if( axisp.eq.1 )then
                             n2a=indexRange(sidep1,1)
                             n2b=indexRange(sidep1,1)
                           else
                             n3a=indexRange(sidep1,2)
                             n3b=indexRange(sidep1,2)
                           end if
                             do i3=n3a,n3b
                             do i2=n2a,n2b
                             do i1=n1a,n1b
                               if( mask(i1,i2,i3).ne.0 )then
                              ! Define derivative macros before calling this macro
                              ! By default there is no AD:
                                ! get nuT,nuTx,nuTy,nuTz
                                     nuT = u(i1,i2,i3,vsc)
                                     nuTx=ux22r(i1,i2,i3,vsc)
                                     nuTy=uy22r(i1,i2,i3,vsc)
                                    ! normal00-form of the diffusion operator in 2D
                              ! Artificial Dissipation  **todo** implicit-line, self-adjoint versions
                              ! Here now is the statement where the ghost line value in the RHS is assigned.
                                  f(i1+is1,i2,i3)=(2*side-1)*( (nuT*(
     & ulaplacian22r(i1,i2,i3,uc))+2.*nuTx*(ux22r(i1,i2,i3,uc))+nuTy*(
     & uy22r(i1,i2,i3,uc)+ux22r(i1,i2,i3,vc)))-(advectCoeff*(uu(i1,i2,
     & i3,uc)*ux22r(i1,i2,i3,uc)+uu(i1,i2,i3,vc)*uy22r(i1,i2,i3,uc))) 
     &  )
                               end if
                             end do
                             end do
                             end do
                           ! For the Boussinesq approximation:  -alpha*T* normal00.gravity
                           if( pdeModel.eq.BoussinesqModel .or. 
     & pdeModel.eq.viscoPlasticModel )then
                               do i3=n3a,n3b
                               do i2=n2a,n2b
                               do i1=n1a,n1b
                                 if( mask(i1,i2,i3).ne.0 )then
                                f(i1+is1,i2+is2,i3+is3)=f(i1+is1,i2+
     & is2,i3+is3)-(2*side-1)*thermalExpansivity*u(i1,i2,i3,tc)*
     & gravity(axis)
                                 end if
                               end do
                               end do
                               end do
                           else if( pdeModel.eq.twoPhaseFlowModel )then
                               do i3=n3a,n3b
                               do i2=n2a,n2b
                               do i1=n1a,n1b
                                 if( mask(i1,i2,i3).ne.0 )then
                                f(i1+is1,i2+is2,i3+is3)=f(i1+is1,i2+
     & is2,i3+is3)+(2*side-1)*u(i1,i2,i3,rc)*gravity(axis)
                                 end if
                               end do
                               end do
                               end do
                           end if
                           ! -- include contribution to BC from the body force ---
                           if( turnOnBodyForcing.eq.1 )then
                               do i3=n3a,n3b
                               do i2=n2a,n2b
                               do i1=n1a,n1b
                                 if( mask(i1,i2,i3).ne.0 )then
                                 f(i1+is1,i2+is2,i3+is3)=f(i1+is1,i2+
     & is2,i3+is3)+(2*side-1)*udf(i1,i2,i3,uc+axis)
                                 end if
                               end do
                               end do
                               end do
                           end if
                           ! reset loop bounds
                           if( axisp.eq.0 )then
                             n1a=indexRange(0,0)
                             n1b=indexRange(1,0)
                           else if( axisp.eq.1 )then
                             n2a=indexRange(0,1)
                             n2b=indexRange(1,1)
                           else
                             n3a=indexRange(0,2)
                             n3b=indexRange(1,2)
                           end if
                          end if
                        end do
                       end do
                   else if( side.eq.1 .and. axis.eq.0 )then
                       ! Use the curl-curl form of the equations
                         do i3=n3a,n3b
                         do i2=n2a,n2b
                         do i1=n1a,n1b
                           if( mask(i1,i2,i3).ne.0 )then
                          ! Define derivative macros before calling this macro
                          ! By default there is no AD:
                            ! get nuT,nuTx,nuTy,nuTz
                                 nuT = u(i1,i2,i3,vsc)
                                 nuTx=ux22r(i1,i2,i3,vsc)
                                 nuTy=uy22r(i1,i2,i3,vsc)
                                ! curl-curl form of the diffusion operator in 2D
                          ! Artificial Dissipation  **todo** implicit-line, self-adjoint versions
                          ! Here now is the statement where the ghost line value in the RHS is assigned.
                              f(i1+is1,i2,i3)=(2*side-1)*( (nuT*(-
     & uxy22r(i1,i2,i3,vc)+uyy22r(i1,i2,i3,uc))-2.*nuTx*(uy22r(i1,i2,
     & i3,vc))+nuTy*(uy22r(i1,i2,i3,uc)+ux22r(i1,i2,i3,vc)))-(
     & advectCoeff*(uu(i1,i2,i3,uc)*ux22r(i1,i2,i3,uc)+uu(i1,i2,i3,vc)
     & *uy22r(i1,i2,i3,uc)))  )
                           end if
                         end do
                         end do
                         end do
                       ! For the Boussinesq approximation:  -alpha*T* normal10.gravity
                       if( pdeModel.eq.BoussinesqModel .or. 
     & pdeModel.eq.viscoPlasticModel )then
                           do i3=n3a,n3b
                           do i2=n2a,n2b
                           do i1=n1a,n1b
                             if( mask(i1,i2,i3).ne.0 )then
                            f(i1+is1,i2+is2,i3+is3)=f(i1+is1,i2+is2,i3+
     & is3)-(2*side-1)*thermalExpansivity*u(i1,i2,i3,tc)*gravity(axis)
                             end if
                           end do
                           end do
                           end do
                       else if( pdeModel.eq.twoPhaseFlowModel )then
                           do i3=n3a,n3b
                           do i2=n2a,n2b
                           do i1=n1a,n1b
                             if( mask(i1,i2,i3).ne.0 )then
                            f(i1+is1,i2+is2,i3+is3)=f(i1+is1,i2+is2,i3+
     & is3)+(2*side-1)*u(i1,i2,i3,rc)*gravity(axis)
                             end if
                           end do
                           end do
                           end do
                       end if
                       ! -- include contribution to BC from the body force --- *wdh* 2012/07/06
                       if( turnOnBodyForcing.eq.1 )then
                         ! write(*,'(" *** inspf: add body force to pressure BC ***")') 
                           do i3=n3a,n3b
                           do i2=n2a,n2b
                           do i1=n1a,n1b
                             if( mask(i1,i2,i3).ne.0 )then
                             f(i1+is1,i2+is2,i3+is3)=f(i1+is1,i2+is2,
     & i3+is3)+(2*side-1)*udf(i1,i2,i3,uc+axis)
                             end if
                           end do
                           end do
                           end do
                       end if
                       ! end points that meet a wall are treated differently to make sure p.x=0 where it should be
                       ! --> do not use the curl-curl boundary condition
                       do kd=1,nd-1 ! loop over the two-tangential directions
                        axisp=mod(axis+kd,nd)
                        do sidep1=0,1
                         ! For now: do not apply the corner correction for TZ flow (we need to fix TZ forcing for this case)
                         if( bc(sidep1,axisp)
     & .eq.inflowWithVelocityGiven .or. bc(sidep1,axisp)
     & .eq.noSlipWall .and. twilightZoneFlow.eq.0 )then
                           if( axisp.eq.0 )then
                             n1a=indexRange(sidep1,0)
                             n1b=indexRange(sidep1,0)
                           else if( axisp.eq.1 )then
                             n2a=indexRange(sidep1,1)
                             n2b=indexRange(sidep1,1)
                           else
                             n3a=indexRange(sidep1,2)
                             n3b=indexRange(sidep1,2)
                           end if
                             do i3=n3a,n3b
                             do i2=n2a,n2b
                             do i1=n1a,n1b
                               if( mask(i1,i2,i3).ne.0 )then
                              ! Define derivative macros before calling this macro
                              ! By default there is no AD:
                                ! get nuT,nuTx,nuTy,nuTz
                                     nuT = u(i1,i2,i3,vsc)
                                     nuTx=ux22r(i1,i2,i3,vsc)
                                     nuTy=uy22r(i1,i2,i3,vsc)
                                    ! normal10-form of the diffusion operator in 2D
                              ! Artificial Dissipation  **todo** implicit-line, self-adjoint versions
                              ! Here now is the statement where the ghost line value in the RHS is assigned.
                                  f(i1+is1,i2,i3)=(2*side-1)*( (nuT*(
     & ulaplacian22r(i1,i2,i3,uc))+2.*nuTx*(ux22r(i1,i2,i3,uc))+nuTy*(
     & uy22r(i1,i2,i3,uc)+ux22r(i1,i2,i3,vc)))-(advectCoeff*(uu(i1,i2,
     & i3,uc)*ux22r(i1,i2,i3,uc)+uu(i1,i2,i3,vc)*uy22r(i1,i2,i3,uc))) 
     &  )
                               end if
                             end do
                             end do
                             end do
                           ! For the Boussinesq approximation:  -alpha*T* normal10.gravity
                           if( pdeModel.eq.BoussinesqModel .or. 
     & pdeModel.eq.viscoPlasticModel )then
                               do i3=n3a,n3b
                               do i2=n2a,n2b
                               do i1=n1a,n1b
                                 if( mask(i1,i2,i3).ne.0 )then
                                f(i1+is1,i2+is2,i3+is3)=f(i1+is1,i2+
     & is2,i3+is3)-(2*side-1)*thermalExpansivity*u(i1,i2,i3,tc)*
     & gravity(axis)
                                 end if
                               end do
                               end do
                               end do
                           else if( pdeModel.eq.twoPhaseFlowModel )then
                               do i3=n3a,n3b
                               do i2=n2a,n2b
                               do i1=n1a,n1b
                                 if( mask(i1,i2,i3).ne.0 )then
                                f(i1+is1,i2+is2,i3+is3)=f(i1+is1,i2+
     & is2,i3+is3)+(2*side-1)*u(i1,i2,i3,rc)*gravity(axis)
                                 end if
                               end do
                               end do
                               end do
                           end if
                           ! -- include contribution to BC from the body force ---
                           if( turnOnBodyForcing.eq.1 )then
                               do i3=n3a,n3b
                               do i2=n2a,n2b
                               do i1=n1a,n1b
                                 if( mask(i1,i2,i3).ne.0 )then
                                 f(i1+is1,i2+is2,i3+is3)=f(i1+is1,i2+
     & is2,i3+is3)+(2*side-1)*udf(i1,i2,i3,uc+axis)
                                 end if
                               end do
                               end do
                               end do
                           end if
                           ! reset loop bounds
                           if( axisp.eq.0 )then
                             n1a=indexRange(0,0)
                             n1b=indexRange(1,0)
                           else if( axisp.eq.1 )then
                             n2a=indexRange(0,1)
                             n2b=indexRange(1,1)
                           else
                             n3a=indexRange(0,2)
                             n3b=indexRange(1,2)
                           end if
                          end if
                        end do
                       end do
                   else if( side.eq.0 .and. axis.eq.1 )then
                       ! Use the curl-curl form of the equations
                         do i3=n3a,n3b
                         do i2=n2a,n2b
                         do i1=n1a,n1b
                           if( mask(i1,i2,i3).ne.0 )then
                          ! Define derivative macros before calling this macro
                          ! By default there is no AD:
                            ! get nuT,nuTx,nuTy,nuTz
                                 nuT = u(i1,i2,i3,vsc)
                                 nuTx=ux22r(i1,i2,i3,vsc)
                                 nuTy=uy22r(i1,i2,i3,vsc)
                                ! curl-curl form of the diffusion operator in 2D
                          ! Artificial Dissipation  **todo** implicit-line, self-adjoint versions
                          ! Here now is the statement where the ghost line value in the RHS is assigned.
                              f(i1,i2+is2,i3)=(2*side-1)*( (nuT*(
     & uxx22r(i1,i2,i3,vc)-uxy22r(i1,i2,i3,uc))-2.*nuTy*(ux22r(i1,i2,
     & i3,uc))+nuTx*(ux22r(i1,i2,i3,vc)+uy22r(i1,i2,i3,uc)))-(
     & advectCoeff*(uu(i1,i2,i3,uc)*ux22r(i1,i2,i3,vc)+uu(i1,i2,i3,vc)
     & *uy22r(i1,i2,i3,vc)))  )
                           end if
                         end do
                         end do
                         end do
                       ! For the Boussinesq approximation:  -alpha*T* normal01.gravity
                       if( pdeModel.eq.BoussinesqModel .or. 
     & pdeModel.eq.viscoPlasticModel )then
                           do i3=n3a,n3b
                           do i2=n2a,n2b
                           do i1=n1a,n1b
                             if( mask(i1,i2,i3).ne.0 )then
                            f(i1+is1,i2+is2,i3+is3)=f(i1+is1,i2+is2,i3+
     & is3)-(2*side-1)*thermalExpansivity*u(i1,i2,i3,tc)*gravity(axis)
                             end if
                           end do
                           end do
                           end do
                       else if( pdeModel.eq.twoPhaseFlowModel )then
                           do i3=n3a,n3b
                           do i2=n2a,n2b
                           do i1=n1a,n1b
                             if( mask(i1,i2,i3).ne.0 )then
                            f(i1+is1,i2+is2,i3+is3)=f(i1+is1,i2+is2,i3+
     & is3)+(2*side-1)*u(i1,i2,i3,rc)*gravity(axis)
                             end if
                           end do
                           end do
                           end do
                       end if
                       ! -- include contribution to BC from the body force --- *wdh* 2012/07/06
                       if( turnOnBodyForcing.eq.1 )then
                         ! write(*,'(" *** inspf: add body force to pressure BC ***")') 
                           do i3=n3a,n3b
                           do i2=n2a,n2b
                           do i1=n1a,n1b
                             if( mask(i1,i2,i3).ne.0 )then
                             f(i1+is1,i2+is2,i3+is3)=f(i1+is1,i2+is2,
     & i3+is3)+(2*side-1)*udf(i1,i2,i3,uc+axis)
                             end if
                           end do
                           end do
                           end do
                       end if
                       ! end points that meet a wall are treated differently to make sure p.x=0 where it should be
                       ! --> do not use the curl-curl boundary condition
                       do kd=1,nd-1 ! loop over the two-tangential directions
                        axisp=mod(axis+kd,nd)
                        do sidep1=0,1
                         ! For now: do not apply the corner correction for TZ flow (we need to fix TZ forcing for this case)
                         if( bc(sidep1,axisp)
     & .eq.inflowWithVelocityGiven .or. bc(sidep1,axisp)
     & .eq.noSlipWall .and. twilightZoneFlow.eq.0 )then
                           if( axisp.eq.0 )then
                             n1a=indexRange(sidep1,0)
                             n1b=indexRange(sidep1,0)
                           else if( axisp.eq.1 )then
                             n2a=indexRange(sidep1,1)
                             n2b=indexRange(sidep1,1)
                           else
                             n3a=indexRange(sidep1,2)
                             n3b=indexRange(sidep1,2)
                           end if
                             do i3=n3a,n3b
                             do i2=n2a,n2b
                             do i1=n1a,n1b
                               if( mask(i1,i2,i3).ne.0 )then
                              ! Define derivative macros before calling this macro
                              ! By default there is no AD:
                                ! get nuT,nuTx,nuTy,nuTz
                                     nuT = u(i1,i2,i3,vsc)
                                     nuTx=ux22r(i1,i2,i3,vsc)
                                     nuTy=uy22r(i1,i2,i3,vsc)
                                    ! normal01-form of the diffusion operator in 2D
                              ! Artificial Dissipation  **todo** implicit-line, self-adjoint versions
                              ! Here now is the statement where the ghost line value in the RHS is assigned.
                                  f(i1,i2+is2,i3)=(2*side-1)*( (nuT*(
     & ulaplacian22r(i1,i2,i3,vc))+2.*nuTy*(uy22r(i1,i2,i3,vc))+nuTx*(
     & ux22r(i1,i2,i3,vc)+uy22r(i1,i2,i3,uc)))-(advectCoeff*(uu(i1,i2,
     & i3,uc)*ux22r(i1,i2,i3,vc)+uu(i1,i2,i3,vc)*uy22r(i1,i2,i3,vc))) 
     &  )
                               end if
                             end do
                             end do
                             end do
                           ! For the Boussinesq approximation:  -alpha*T* normal01.gravity
                           if( pdeModel.eq.BoussinesqModel .or. 
     & pdeModel.eq.viscoPlasticModel )then
                               do i3=n3a,n3b
                               do i2=n2a,n2b
                               do i1=n1a,n1b
                                 if( mask(i1,i2,i3).ne.0 )then
                                f(i1+is1,i2+is2,i3+is3)=f(i1+is1,i2+
     & is2,i3+is3)-(2*side-1)*thermalExpansivity*u(i1,i2,i3,tc)*
     & gravity(axis)
                                 end if
                               end do
                               end do
                               end do
                           else if( pdeModel.eq.twoPhaseFlowModel )then
                               do i3=n3a,n3b
                               do i2=n2a,n2b
                               do i1=n1a,n1b
                                 if( mask(i1,i2,i3).ne.0 )then
                                f(i1+is1,i2+is2,i3+is3)=f(i1+is1,i2+
     & is2,i3+is3)+(2*side-1)*u(i1,i2,i3,rc)*gravity(axis)
                                 end if
                               end do
                               end do
                               end do
                           end if
                           ! -- include contribution to BC from the body force ---
                           if( turnOnBodyForcing.eq.1 )then
                               do i3=n3a,n3b
                               do i2=n2a,n2b
                               do i1=n1a,n1b
                                 if( mask(i1,i2,i3).ne.0 )then
                                 f(i1+is1,i2+is2,i3+is3)=f(i1+is1,i2+
     & is2,i3+is3)+(2*side-1)*udf(i1,i2,i3,uc+axis)
                                 end if
                               end do
                               end do
                               end do
                           end if
                           ! reset loop bounds
                           if( axisp.eq.0 )then
                             n1a=indexRange(0,0)
                             n1b=indexRange(1,0)
                           else if( axisp.eq.1 )then
                             n2a=indexRange(0,1)
                             n2b=indexRange(1,1)
                           else
                             n3a=indexRange(0,2)
                             n3b=indexRange(1,2)
                           end if
                          end if
                        end do
                       end do
                   else if( side.eq.1 .and. axis.eq.1 )then
                       ! Use the curl-curl form of the equations
                         do i3=n3a,n3b
                         do i2=n2a,n2b
                         do i1=n1a,n1b
                           if( mask(i1,i2,i3).ne.0 )then
                          ! Define derivative macros before calling this macro
                          ! By default there is no AD:
                            ! get nuT,nuTx,nuTy,nuTz
                                 nuT = u(i1,i2,i3,vsc)
                                 nuTx=ux22r(i1,i2,i3,vsc)
                                 nuTy=uy22r(i1,i2,i3,vsc)
                                ! curl-curl form of the diffusion operator in 2D
                          ! Artificial Dissipation  **todo** implicit-line, self-adjoint versions
                          ! Here now is the statement where the ghost line value in the RHS is assigned.
                              f(i1,i2+is2,i3)=(2*side-1)*( (nuT*(
     & uxx22r(i1,i2,i3,vc)-uxy22r(i1,i2,i3,uc))-2.*nuTy*(ux22r(i1,i2,
     & i3,uc))+nuTx*(ux22r(i1,i2,i3,vc)+uy22r(i1,i2,i3,uc)))-(
     & advectCoeff*(uu(i1,i2,i3,uc)*ux22r(i1,i2,i3,vc)+uu(i1,i2,i3,vc)
     & *uy22r(i1,i2,i3,vc)))  )
                           end if
                         end do
                         end do
                         end do
                       ! For the Boussinesq approximation:  -alpha*T* normal11.gravity
                       if( pdeModel.eq.BoussinesqModel .or. 
     & pdeModel.eq.viscoPlasticModel )then
                           do i3=n3a,n3b
                           do i2=n2a,n2b
                           do i1=n1a,n1b
                             if( mask(i1,i2,i3).ne.0 )then
                            f(i1+is1,i2+is2,i3+is3)=f(i1+is1,i2+is2,i3+
     & is3)-(2*side-1)*thermalExpansivity*u(i1,i2,i3,tc)*gravity(axis)
                             end if
                           end do
                           end do
                           end do
                       else if( pdeModel.eq.twoPhaseFlowModel )then
                           do i3=n3a,n3b
                           do i2=n2a,n2b
                           do i1=n1a,n1b
                             if( mask(i1,i2,i3).ne.0 )then
                            f(i1+is1,i2+is2,i3+is3)=f(i1+is1,i2+is2,i3+
     & is3)+(2*side-1)*u(i1,i2,i3,rc)*gravity(axis)
                             end if
                           end do
                           end do
                           end do
                       end if
                       ! -- include contribution to BC from the body force --- *wdh* 2012/07/06
                       if( turnOnBodyForcing.eq.1 )then
                         ! write(*,'(" *** inspf: add body force to pressure BC ***")') 
                           do i3=n3a,n3b
                           do i2=n2a,n2b
                           do i1=n1a,n1b
                             if( mask(i1,i2,i3).ne.0 )then
                             f(i1+is1,i2+is2,i3+is3)=f(i1+is1,i2+is2,
     & i3+is3)+(2*side-1)*udf(i1,i2,i3,uc+axis)
                             end if
                           end do
                           end do
                           end do
                       end if
                       ! end points that meet a wall are treated differently to make sure p.x=0 where it should be
                       ! --> do not use the curl-curl boundary condition
                       do kd=1,nd-1 ! loop over the two-tangential directions
                        axisp=mod(axis+kd,nd)
                        do sidep1=0,1
                         ! For now: do not apply the corner correction for TZ flow (we need to fix TZ forcing for this case)
                         if( bc(sidep1,axisp)
     & .eq.inflowWithVelocityGiven .or. bc(sidep1,axisp)
     & .eq.noSlipWall .and. twilightZoneFlow.eq.0 )then
                           if( axisp.eq.0 )then
                             n1a=indexRange(sidep1,0)
                             n1b=indexRange(sidep1,0)
                           else if( axisp.eq.1 )then
                             n2a=indexRange(sidep1,1)
                             n2b=indexRange(sidep1,1)
                           else
                             n3a=indexRange(sidep1,2)
                             n3b=indexRange(sidep1,2)
                           end if
                             do i3=n3a,n3b
                             do i2=n2a,n2b
                             do i1=n1a,n1b
                               if( mask(i1,i2,i3).ne.0 )then
                              ! Define derivative macros before calling this macro
                              ! By default there is no AD:
                                ! get nuT,nuTx,nuTy,nuTz
                                     nuT = u(i1,i2,i3,vsc)
                                     nuTx=ux22r(i1,i2,i3,vsc)
                                     nuTy=uy22r(i1,i2,i3,vsc)
                                    ! normal11-form of the diffusion operator in 2D
                              ! Artificial Dissipation  **todo** implicit-line, self-adjoint versions
                              ! Here now is the statement where the ghost line value in the RHS is assigned.
                                  f(i1,i2+is2,i3)=(2*side-1)*( (nuT*(
     & ulaplacian22r(i1,i2,i3,vc))+2.*nuTy*(uy22r(i1,i2,i3,vc))+nuTx*(
     & ux22r(i1,i2,i3,vc)+uy22r(i1,i2,i3,uc)))-(advectCoeff*(uu(i1,i2,
     & i3,uc)*ux22r(i1,i2,i3,vc)+uu(i1,i2,i3,vc)*uy22r(i1,i2,i3,vc))) 
     &  )
                               end if
                             end do
                             end do
                             end do
                           ! For the Boussinesq approximation:  -alpha*T* normal11.gravity
                           if( pdeModel.eq.BoussinesqModel .or. 
     & pdeModel.eq.viscoPlasticModel )then
                               do i3=n3a,n3b
                               do i2=n2a,n2b
                               do i1=n1a,n1b
                                 if( mask(i1,i2,i3).ne.0 )then
                                f(i1+is1,i2+is2,i3+is3)=f(i1+is1,i2+
     & is2,i3+is3)-(2*side-1)*thermalExpansivity*u(i1,i2,i3,tc)*
     & gravity(axis)
                                 end if
                               end do
                               end do
                               end do
                           else if( pdeModel.eq.twoPhaseFlowModel )then
                               do i3=n3a,n3b
                               do i2=n2a,n2b
                               do i1=n1a,n1b
                                 if( mask(i1,i2,i3).ne.0 )then
                                f(i1+is1,i2+is2,i3+is3)=f(i1+is1,i2+
     & is2,i3+is3)+(2*side-1)*u(i1,i2,i3,rc)*gravity(axis)
                                 end if
                               end do
                               end do
                               end do
                           end if
                           ! -- include contribution to BC from the body force ---
                           if( turnOnBodyForcing.eq.1 )then
                               do i3=n3a,n3b
                               do i2=n2a,n2b
                               do i1=n1a,n1b
                                 if( mask(i1,i2,i3).ne.0 )then
                                 f(i1+is1,i2+is2,i3+is3)=f(i1+is1,i2+
     & is2,i3+is3)+(2*side-1)*udf(i1,i2,i3,uc+axis)
                                 end if
                               end do
                               end do
                               end do
                           end if
                           ! reset loop bounds
                           if( axisp.eq.0 )then
                             n1a=indexRange(0,0)
                             n1b=indexRange(1,0)
                           else if( axisp.eq.1 )then
                             n2a=indexRange(0,1)
                             n2b=indexRange(1,1)
                           else
                             n3a=indexRange(0,2)
                             n3b=indexRange(1,2)
                           end if
                          end if
                        end do
                       end do
                   else
                     stop 33
                   end if
                else if( gridType.eq.curvilinear )then
                         ! user defined force:
                   if( side.eq.0. .and. axis.eq.0 )then
                       ! Use the curl-curl form of the equations
                         do i3=n3a,n3b
                         do i2=n2a,n2b
                         do i1=n1a,n1b
                           if( mask(i1,i2,i3).ne.0 )then
                          ! Define derivative macros before calling this macro
                          ! By default there is no AD:
                            ! get nuT,nuTx,nuTy,nuTz
                                 nuT = u(i1,i2,i3,vsc)
                                 nuTx=ux22(i1,i2,i3,vsc)
                                 nuTy=uy22(i1,i2,i3,vsc)
                                ! curl-curl form of the diffusion operator in 2D
                          ! Artificial Dissipation  **todo** implicit-line, self-adjoint versions
                          ! Here now is the statement where the ghost line value in the RHS is assigned.
                              f(i1+is1,i2+is2,i3)=normal00(i1,i2,i3,0)*
     & ( (nuT*(-uxy22(i1,i2,i3,vc)+uyy22(i1,i2,i3,uc))-2.*nuTx*(uy22(
     & i1,i2,i3,vc))+nuTy*(uy22(i1,i2,i3,uc)+ux22(i1,i2,i3,vc)))-(
     & advectCoeff*(uu(i1,i2,i3,uc)*ux22(i1,i2,i3,uc)+uu(i1,i2,i3,vc)*
     & uy22(i1,i2,i3,uc)))  )+normal00(i1,i2,i3,1)*( (nuT*(uxx22(i1,
     & i2,i3,vc)-uxy22(i1,i2,i3,uc))-2.*nuTy*(ux22(i1,i2,i3,uc))+nuTx*
     & (ux22(i1,i2,i3,vc)+uy22(i1,i2,i3,uc)))-(advectCoeff*(uu(i1,i2,
     & i3,uc)*ux22(i1,i2,i3,vc)+uu(i1,i2,i3,vc)*uy22(i1,i2,i3,vc)))  )
                           end if
                         end do
                         end do
                         end do
                       ! For the Boussinesq approximation:  -alpha*T* normal00.gravity
                       if( pdeModel.eq.BoussinesqModel .or. 
     & pdeModel.eq.viscoPlasticModel )then
                           do i3=n3a,n3b
                           do i2=n2a,n2b
                           do i1=n1a,n1b
                             if( mask(i1,i2,i3).ne.0 )then
                             f(i1+is1,i2+is2,i3)=f(i1+is1,i2+is2,i3)-
     & thermalExpansivity*u(i1,i2,i3,tc)*(gravity(0)*normal00(i1,i2,
     & i3,0)+gravity(1)*normal00(i1,i2,i3,1))
                             end if
                           end do
                           end do
                           end do
                       else if( pdeModel.eq.twoPhaseFlowModel )then
                           do i3=n3a,n3b
                           do i2=n2a,n2b
                           do i1=n1a,n1b
                             if( mask(i1,i2,i3).ne.0 )then
                             f(i1+is1,i2+is2,i3)=f(i1+is1,i2+is2,i3)+u(
     & i1,i2,i3,rc)*(gravity(0)*normal00(i1,i2,i3,0)+gravity(1)*
     & normal00(i1,i2,i3,1))
                             end if
                           end do
                           end do
                           end do
                       end if
                       ! -- include contribution to BC from the body force --- *wdh* 2012/07/06
                       if( turnOnBodyForcing.eq.1 )then
                         ! write(*,'(" *** inspf: add body force to pressure BC ***")') 
                           do i3=n3a,n3b
                           do i2=n2a,n2b
                           do i1=n1a,n1b
                             if( mask(i1,i2,i3).ne.0 )then
                             f(i1+is1,i2+is2,i3)=f(i1+is1,i2+is2,i3)+
     & normal00(i1,i2,i3,0)*udf(i1,i2,i3,uc)+normal00(i1,i2,i3,1)*udf(
     & i1,i2,i3,vc)
                             end if
                           end do
                           end do
                           end do
                       end if
                       ! end points that meet a wall are treated differently to make sure p.x=0 where it should be
                       ! --> do not use the curl-curl boundary condition
                       do kd=1,nd-1 ! loop over the two-tangential directions
                        axisp=mod(axis+kd,nd)
                        do sidep1=0,1
                         ! For now: do not apply the corner correction for TZ flow (we need to fix TZ forcing for this case)
                         if( bc(sidep1,axisp)
     & .eq.inflowWithVelocityGiven .or. bc(sidep1,axisp)
     & .eq.noSlipWall .and. twilightZoneFlow.eq.0 )then
                           if( axisp.eq.0 )then
                             n1a=indexRange(sidep1,0)
                             n1b=indexRange(sidep1,0)
                           else if( axisp.eq.1 )then
                             n2a=indexRange(sidep1,1)
                             n2b=indexRange(sidep1,1)
                           else
                             n3a=indexRange(sidep1,2)
                             n3b=indexRange(sidep1,2)
                           end if
                             do i3=n3a,n3b
                             do i2=n2a,n2b
                             do i1=n1a,n1b
                               if( mask(i1,i2,i3).ne.0 )then
                              ! Define derivative macros before calling this macro
                              ! By default there is no AD:
                                ! get nuT,nuTx,nuTy,nuTz
                                     nuT = u(i1,i2,i3,vsc)
                                     nuTx=ux22(i1,i2,i3,vsc)
                                     nuTy=uy22(i1,i2,i3,vsc)
                                    ! normal00-form of the diffusion operator in 2D
                              ! Artificial Dissipation  **todo** implicit-line, self-adjoint versions
                              ! Here now is the statement where the ghost line value in the RHS is assigned.
                                  f(i1+is1,i2+is2,i3)=normal00(i1,i2,
     & i3,0)*( (nuT*(ulaplacian22(i1,i2,i3,uc))+2.*nuTx*(ux22(i1,i2,
     & i3,uc))+nuTy*(uy22(i1,i2,i3,uc)+ux22(i1,i2,i3,vc)))-(
     & advectCoeff*(uu(i1,i2,i3,uc)*ux22(i1,i2,i3,uc)+uu(i1,i2,i3,vc)*
     & uy22(i1,i2,i3,uc)))  )+normal00(i1,i2,i3,1)*( (nuT*(
     & ulaplacian22(i1,i2,i3,vc))+2.*nuTy*(uy22(i1,i2,i3,vc))+nuTx*(
     & ux22(i1,i2,i3,vc)+uy22(i1,i2,i3,uc)))-(advectCoeff*(uu(i1,i2,
     & i3,uc)*ux22(i1,i2,i3,vc)+uu(i1,i2,i3,vc)*uy22(i1,i2,i3,vc)))  )
                               end if
                             end do
                             end do
                             end do
                           ! For the Boussinesq approximation:  -alpha*T* normal00.gravity
                           if( pdeModel.eq.BoussinesqModel .or. 
     & pdeModel.eq.viscoPlasticModel )then
                               do i3=n3a,n3b
                               do i2=n2a,n2b
                               do i1=n1a,n1b
                                 if( mask(i1,i2,i3).ne.0 )then
                                 f(i1+is1,i2+is2,i3)=f(i1+is1,i2+is2,
     & i3)-thermalExpansivity*u(i1,i2,i3,tc)*(gravity(0)*normal00(i1,
     & i2,i3,0)+gravity(1)*normal00(i1,i2,i3,1))
                                 end if
                               end do
                               end do
                               end do
                           else if( pdeModel.eq.twoPhaseFlowModel )then
                               do i3=n3a,n3b
                               do i2=n2a,n2b
                               do i1=n1a,n1b
                                 if( mask(i1,i2,i3).ne.0 )then
                                 f(i1+is1,i2+is2,i3)=f(i1+is1,i2+is2,
     & i3)+u(i1,i2,i3,rc)*(gravity(0)*normal00(i1,i2,i3,0)+gravity(1)*
     & normal00(i1,i2,i3,1))
                                 end if
                               end do
                               end do
                               end do
                           end if
                           ! -- include contribution to BC from the body force ---
                           if( turnOnBodyForcing.eq.1 )then
                               do i3=n3a,n3b
                               do i2=n2a,n2b
                               do i1=n1a,n1b
                                 if( mask(i1,i2,i3).ne.0 )then
                                 f(i1+is1,i2+is2,i3)=f(i1+is1,i2+is2,
     & i3)+normal00(i1,i2,i3,0)*udf(i1,i2,i3,uc)+normal00(i1,i2,i3,1)*
     & udf(i1,i2,i3,vc)
                                 end if
                               end do
                               end do
                               end do
                           end if
                           ! reset loop bounds
                           if( axisp.eq.0 )then
                             n1a=indexRange(0,0)
                             n1b=indexRange(1,0)
                           else if( axisp.eq.1 )then
                             n2a=indexRange(0,1)
                             n2b=indexRange(1,1)
                           else
                             n3a=indexRange(0,2)
                             n3b=indexRange(1,2)
                           end if
                          end if
                        end do
                       end do
                   else if( side.eq.1 .and. axis.eq.0 )then
                       ! Use the curl-curl form of the equations
                         do i3=n3a,n3b
                         do i2=n2a,n2b
                         do i1=n1a,n1b
                           if( mask(i1,i2,i3).ne.0 )then
                          ! Define derivative macros before calling this macro
                          ! By default there is no AD:
                            ! get nuT,nuTx,nuTy,nuTz
                                 nuT = u(i1,i2,i3,vsc)
                                 nuTx=ux22(i1,i2,i3,vsc)
                                 nuTy=uy22(i1,i2,i3,vsc)
                                ! curl-curl form of the diffusion operator in 2D
                          ! Artificial Dissipation  **todo** implicit-line, self-adjoint versions
                          ! Here now is the statement where the ghost line value in the RHS is assigned.
                              f(i1+is1,i2+is2,i3)=normal10(i1,i2,i3,0)*
     & ( (nuT*(-uxy22(i1,i2,i3,vc)+uyy22(i1,i2,i3,uc))-2.*nuTx*(uy22(
     & i1,i2,i3,vc))+nuTy*(uy22(i1,i2,i3,uc)+ux22(i1,i2,i3,vc)))-(
     & advectCoeff*(uu(i1,i2,i3,uc)*ux22(i1,i2,i3,uc)+uu(i1,i2,i3,vc)*
     & uy22(i1,i2,i3,uc)))  )+normal10(i1,i2,i3,1)*( (nuT*(uxx22(i1,
     & i2,i3,vc)-uxy22(i1,i2,i3,uc))-2.*nuTy*(ux22(i1,i2,i3,uc))+nuTx*
     & (ux22(i1,i2,i3,vc)+uy22(i1,i2,i3,uc)))-(advectCoeff*(uu(i1,i2,
     & i3,uc)*ux22(i1,i2,i3,vc)+uu(i1,i2,i3,vc)*uy22(i1,i2,i3,vc)))  )
                           end if
                         end do
                         end do
                         end do
                       ! For the Boussinesq approximation:  -alpha*T* normal10.gravity
                       if( pdeModel.eq.BoussinesqModel .or. 
     & pdeModel.eq.viscoPlasticModel )then
                           do i3=n3a,n3b
                           do i2=n2a,n2b
                           do i1=n1a,n1b
                             if( mask(i1,i2,i3).ne.0 )then
                             f(i1+is1,i2+is2,i3)=f(i1+is1,i2+is2,i3)-
     & thermalExpansivity*u(i1,i2,i3,tc)*(gravity(0)*normal10(i1,i2,
     & i3,0)+gravity(1)*normal10(i1,i2,i3,1))
                             end if
                           end do
                           end do
                           end do
                       else if( pdeModel.eq.twoPhaseFlowModel )then
                           do i3=n3a,n3b
                           do i2=n2a,n2b
                           do i1=n1a,n1b
                             if( mask(i1,i2,i3).ne.0 )then
                             f(i1+is1,i2+is2,i3)=f(i1+is1,i2+is2,i3)+u(
     & i1,i2,i3,rc)*(gravity(0)*normal10(i1,i2,i3,0)+gravity(1)*
     & normal10(i1,i2,i3,1))
                             end if
                           end do
                           end do
                           end do
                       end if
                       ! -- include contribution to BC from the body force --- *wdh* 2012/07/06
                       if( turnOnBodyForcing.eq.1 )then
                         ! write(*,'(" *** inspf: add body force to pressure BC ***")') 
                           do i3=n3a,n3b
                           do i2=n2a,n2b
                           do i1=n1a,n1b
                             if( mask(i1,i2,i3).ne.0 )then
                             f(i1+is1,i2+is2,i3)=f(i1+is1,i2+is2,i3)+
     & normal10(i1,i2,i3,0)*udf(i1,i2,i3,uc)+normal10(i1,i2,i3,1)*udf(
     & i1,i2,i3,vc)
                             end if
                           end do
                           end do
                           end do
                       end if
                       ! end points that meet a wall are treated differently to make sure p.x=0 where it should be
                       ! --> do not use the curl-curl boundary condition
                       do kd=1,nd-1 ! loop over the two-tangential directions
                        axisp=mod(axis+kd,nd)
                        do sidep1=0,1
                         ! For now: do not apply the corner correction for TZ flow (we need to fix TZ forcing for this case)
                         if( bc(sidep1,axisp)
     & .eq.inflowWithVelocityGiven .or. bc(sidep1,axisp)
     & .eq.noSlipWall .and. twilightZoneFlow.eq.0 )then
                           if( axisp.eq.0 )then
                             n1a=indexRange(sidep1,0)
                             n1b=indexRange(sidep1,0)
                           else if( axisp.eq.1 )then
                             n2a=indexRange(sidep1,1)
                             n2b=indexRange(sidep1,1)
                           else
                             n3a=indexRange(sidep1,2)
                             n3b=indexRange(sidep1,2)
                           end if
                             do i3=n3a,n3b
                             do i2=n2a,n2b
                             do i1=n1a,n1b
                               if( mask(i1,i2,i3).ne.0 )then
                              ! Define derivative macros before calling this macro
                              ! By default there is no AD:
                                ! get nuT,nuTx,nuTy,nuTz
                                     nuT = u(i1,i2,i3,vsc)
                                     nuTx=ux22(i1,i2,i3,vsc)
                                     nuTy=uy22(i1,i2,i3,vsc)
                                    ! normal10-form of the diffusion operator in 2D
                              ! Artificial Dissipation  **todo** implicit-line, self-adjoint versions
                              ! Here now is the statement where the ghost line value in the RHS is assigned.
                                  f(i1+is1,i2+is2,i3)=normal10(i1,i2,
     & i3,0)*( (nuT*(ulaplacian22(i1,i2,i3,uc))+2.*nuTx*(ux22(i1,i2,
     & i3,uc))+nuTy*(uy22(i1,i2,i3,uc)+ux22(i1,i2,i3,vc)))-(
     & advectCoeff*(uu(i1,i2,i3,uc)*ux22(i1,i2,i3,uc)+uu(i1,i2,i3,vc)*
     & uy22(i1,i2,i3,uc)))  )+normal10(i1,i2,i3,1)*( (nuT*(
     & ulaplacian22(i1,i2,i3,vc))+2.*nuTy*(uy22(i1,i2,i3,vc))+nuTx*(
     & ux22(i1,i2,i3,vc)+uy22(i1,i2,i3,uc)))-(advectCoeff*(uu(i1,i2,
     & i3,uc)*ux22(i1,i2,i3,vc)+uu(i1,i2,i3,vc)*uy22(i1,i2,i3,vc)))  )
                               end if
                             end do
                             end do
                             end do
                           ! For the Boussinesq approximation:  -alpha*T* normal10.gravity
                           if( pdeModel.eq.BoussinesqModel .or. 
     & pdeModel.eq.viscoPlasticModel )then
                               do i3=n3a,n3b
                               do i2=n2a,n2b
                               do i1=n1a,n1b
                                 if( mask(i1,i2,i3).ne.0 )then
                                 f(i1+is1,i2+is2,i3)=f(i1+is1,i2+is2,
     & i3)-thermalExpansivity*u(i1,i2,i3,tc)*(gravity(0)*normal10(i1,
     & i2,i3,0)+gravity(1)*normal10(i1,i2,i3,1))
                                 end if
                               end do
                               end do
                               end do
                           else if( pdeModel.eq.twoPhaseFlowModel )then
                               do i3=n3a,n3b
                               do i2=n2a,n2b
                               do i1=n1a,n1b
                                 if( mask(i1,i2,i3).ne.0 )then
                                 f(i1+is1,i2+is2,i3)=f(i1+is1,i2+is2,
     & i3)+u(i1,i2,i3,rc)*(gravity(0)*normal10(i1,i2,i3,0)+gravity(1)*
     & normal10(i1,i2,i3,1))
                                 end if
                               end do
                               end do
                               end do
                           end if
                           ! -- include contribution to BC from the body force ---
                           if( turnOnBodyForcing.eq.1 )then
                               do i3=n3a,n3b
                               do i2=n2a,n2b
                               do i1=n1a,n1b
                                 if( mask(i1,i2,i3).ne.0 )then
                                 f(i1+is1,i2+is2,i3)=f(i1+is1,i2+is2,
     & i3)+normal10(i1,i2,i3,0)*udf(i1,i2,i3,uc)+normal10(i1,i2,i3,1)*
     & udf(i1,i2,i3,vc)
                                 end if
                               end do
                               end do
                               end do
                           end if
                           ! reset loop bounds
                           if( axisp.eq.0 )then
                             n1a=indexRange(0,0)
                             n1b=indexRange(1,0)
                           else if( axisp.eq.1 )then
                             n2a=indexRange(0,1)
                             n2b=indexRange(1,1)
                           else
                             n3a=indexRange(0,2)
                             n3b=indexRange(1,2)
                           end if
                          end if
                        end do
                       end do
                   else if( side.eq.0 .and. axis.eq.1 )then
                       ! Use the curl-curl form of the equations
                         do i3=n3a,n3b
                         do i2=n2a,n2b
                         do i1=n1a,n1b
                           if( mask(i1,i2,i3).ne.0 )then
                          ! Define derivative macros before calling this macro
                          ! By default there is no AD:
                            ! get nuT,nuTx,nuTy,nuTz
                                 nuT = u(i1,i2,i3,vsc)
                                 nuTx=ux22(i1,i2,i3,vsc)
                                 nuTy=uy22(i1,i2,i3,vsc)
                                ! curl-curl form of the diffusion operator in 2D
                          ! Artificial Dissipation  **todo** implicit-line, self-adjoint versions
                          ! Here now is the statement where the ghost line value in the RHS is assigned.
                              f(i1+is1,i2+is2,i3)=normal01(i1,i2,i3,0)*
     & ( (nuT*(-uxy22(i1,i2,i3,vc)+uyy22(i1,i2,i3,uc))-2.*nuTx*(uy22(
     & i1,i2,i3,vc))+nuTy*(uy22(i1,i2,i3,uc)+ux22(i1,i2,i3,vc)))-(
     & advectCoeff*(uu(i1,i2,i3,uc)*ux22(i1,i2,i3,uc)+uu(i1,i2,i3,vc)*
     & uy22(i1,i2,i3,uc)))  )+normal01(i1,i2,i3,1)*( (nuT*(uxx22(i1,
     & i2,i3,vc)-uxy22(i1,i2,i3,uc))-2.*nuTy*(ux22(i1,i2,i3,uc))+nuTx*
     & (ux22(i1,i2,i3,vc)+uy22(i1,i2,i3,uc)))-(advectCoeff*(uu(i1,i2,
     & i3,uc)*ux22(i1,i2,i3,vc)+uu(i1,i2,i3,vc)*uy22(i1,i2,i3,vc)))  )
                           end if
                         end do
                         end do
                         end do
                       ! For the Boussinesq approximation:  -alpha*T* normal01.gravity
                       if( pdeModel.eq.BoussinesqModel .or. 
     & pdeModel.eq.viscoPlasticModel )then
                           do i3=n3a,n3b
                           do i2=n2a,n2b
                           do i1=n1a,n1b
                             if( mask(i1,i2,i3).ne.0 )then
                             f(i1+is1,i2+is2,i3)=f(i1+is1,i2+is2,i3)-
     & thermalExpansivity*u(i1,i2,i3,tc)*(gravity(0)*normal01(i1,i2,
     & i3,0)+gravity(1)*normal01(i1,i2,i3,1))
                             end if
                           end do
                           end do
                           end do
                       else if( pdeModel.eq.twoPhaseFlowModel )then
                           do i3=n3a,n3b
                           do i2=n2a,n2b
                           do i1=n1a,n1b
                             if( mask(i1,i2,i3).ne.0 )then
                             f(i1+is1,i2+is2,i3)=f(i1+is1,i2+is2,i3)+u(
     & i1,i2,i3,rc)*(gravity(0)*normal01(i1,i2,i3,0)+gravity(1)*
     & normal01(i1,i2,i3,1))
                             end if
                           end do
                           end do
                           end do
                       end if
                       ! -- include contribution to BC from the body force --- *wdh* 2012/07/06
                       if( turnOnBodyForcing.eq.1 )then
                         ! write(*,'(" *** inspf: add body force to pressure BC ***")') 
                           do i3=n3a,n3b
                           do i2=n2a,n2b
                           do i1=n1a,n1b
                             if( mask(i1,i2,i3).ne.0 )then
                             f(i1+is1,i2+is2,i3)=f(i1+is1,i2+is2,i3)+
     & normal01(i1,i2,i3,0)*udf(i1,i2,i3,uc)+normal01(i1,i2,i3,1)*udf(
     & i1,i2,i3,vc)
                             end if
                           end do
                           end do
                           end do
                       end if
                       ! end points that meet a wall are treated differently to make sure p.x=0 where it should be
                       ! --> do not use the curl-curl boundary condition
                       do kd=1,nd-1 ! loop over the two-tangential directions
                        axisp=mod(axis+kd,nd)
                        do sidep1=0,1
                         ! For now: do not apply the corner correction for TZ flow (we need to fix TZ forcing for this case)
                         if( bc(sidep1,axisp)
     & .eq.inflowWithVelocityGiven .or. bc(sidep1,axisp)
     & .eq.noSlipWall .and. twilightZoneFlow.eq.0 )then
                           if( axisp.eq.0 )then
                             n1a=indexRange(sidep1,0)
                             n1b=indexRange(sidep1,0)
                           else if( axisp.eq.1 )then
                             n2a=indexRange(sidep1,1)
                             n2b=indexRange(sidep1,1)
                           else
                             n3a=indexRange(sidep1,2)
                             n3b=indexRange(sidep1,2)
                           end if
                             do i3=n3a,n3b
                             do i2=n2a,n2b
                             do i1=n1a,n1b
                               if( mask(i1,i2,i3).ne.0 )then
                              ! Define derivative macros before calling this macro
                              ! By default there is no AD:
                                ! get nuT,nuTx,nuTy,nuTz
                                     nuT = u(i1,i2,i3,vsc)
                                     nuTx=ux22(i1,i2,i3,vsc)
                                     nuTy=uy22(i1,i2,i3,vsc)
                                    ! normal01-form of the diffusion operator in 2D
                              ! Artificial Dissipation  **todo** implicit-line, self-adjoint versions
                              ! Here now is the statement where the ghost line value in the RHS is assigned.
                                  f(i1+is1,i2+is2,i3)=normal01(i1,i2,
     & i3,0)*( (nuT*(ulaplacian22(i1,i2,i3,uc))+2.*nuTx*(ux22(i1,i2,
     & i3,uc))+nuTy*(uy22(i1,i2,i3,uc)+ux22(i1,i2,i3,vc)))-(
     & advectCoeff*(uu(i1,i2,i3,uc)*ux22(i1,i2,i3,uc)+uu(i1,i2,i3,vc)*
     & uy22(i1,i2,i3,uc)))  )+normal01(i1,i2,i3,1)*( (nuT*(
     & ulaplacian22(i1,i2,i3,vc))+2.*nuTy*(uy22(i1,i2,i3,vc))+nuTx*(
     & ux22(i1,i2,i3,vc)+uy22(i1,i2,i3,uc)))-(advectCoeff*(uu(i1,i2,
     & i3,uc)*ux22(i1,i2,i3,vc)+uu(i1,i2,i3,vc)*uy22(i1,i2,i3,vc)))  )
                               end if
                             end do
                             end do
                             end do
                           ! For the Boussinesq approximation:  -alpha*T* normal01.gravity
                           if( pdeModel.eq.BoussinesqModel .or. 
     & pdeModel.eq.viscoPlasticModel )then
                               do i3=n3a,n3b
                               do i2=n2a,n2b
                               do i1=n1a,n1b
                                 if( mask(i1,i2,i3).ne.0 )then
                                 f(i1+is1,i2+is2,i3)=f(i1+is1,i2+is2,
     & i3)-thermalExpansivity*u(i1,i2,i3,tc)*(gravity(0)*normal01(i1,
     & i2,i3,0)+gravity(1)*normal01(i1,i2,i3,1))
                                 end if
                               end do
                               end do
                               end do
                           else if( pdeModel.eq.twoPhaseFlowModel )then
                               do i3=n3a,n3b
                               do i2=n2a,n2b
                               do i1=n1a,n1b
                                 if( mask(i1,i2,i3).ne.0 )then
                                 f(i1+is1,i2+is2,i3)=f(i1+is1,i2+is2,
     & i3)+u(i1,i2,i3,rc)*(gravity(0)*normal01(i1,i2,i3,0)+gravity(1)*
     & normal01(i1,i2,i3,1))
                                 end if
                               end do
                               end do
                               end do
                           end if
                           ! -- include contribution to BC from the body force ---
                           if( turnOnBodyForcing.eq.1 )then
                               do i3=n3a,n3b
                               do i2=n2a,n2b
                               do i1=n1a,n1b
                                 if( mask(i1,i2,i3).ne.0 )then
                                 f(i1+is1,i2+is2,i3)=f(i1+is1,i2+is2,
     & i3)+normal01(i1,i2,i3,0)*udf(i1,i2,i3,uc)+normal01(i1,i2,i3,1)*
     & udf(i1,i2,i3,vc)
                                 end if
                               end do
                               end do
                               end do
                           end if
                           ! reset loop bounds
                           if( axisp.eq.0 )then
                             n1a=indexRange(0,0)
                             n1b=indexRange(1,0)
                           else if( axisp.eq.1 )then
                             n2a=indexRange(0,1)
                             n2b=indexRange(1,1)
                           else
                             n3a=indexRange(0,2)
                             n3b=indexRange(1,2)
                           end if
                          end if
                        end do
                       end do
                   else if( side.eq.1 .and. axis.eq.1 )then
                       ! Use the curl-curl form of the equations
                         do i3=n3a,n3b
                         do i2=n2a,n2b
                         do i1=n1a,n1b
                           if( mask(i1,i2,i3).ne.0 )then
                          ! Define derivative macros before calling this macro
                          ! By default there is no AD:
                            ! get nuT,nuTx,nuTy,nuTz
                                 nuT = u(i1,i2,i3,vsc)
                                 nuTx=ux22(i1,i2,i3,vsc)
                                 nuTy=uy22(i1,i2,i3,vsc)
                                ! curl-curl form of the diffusion operator in 2D
                          ! Artificial Dissipation  **todo** implicit-line, self-adjoint versions
                          ! Here now is the statement where the ghost line value in the RHS is assigned.
                              f(i1+is1,i2+is2,i3)=normal11(i1,i2,i3,0)*
     & ( (nuT*(-uxy22(i1,i2,i3,vc)+uyy22(i1,i2,i3,uc))-2.*nuTx*(uy22(
     & i1,i2,i3,vc))+nuTy*(uy22(i1,i2,i3,uc)+ux22(i1,i2,i3,vc)))-(
     & advectCoeff*(uu(i1,i2,i3,uc)*ux22(i1,i2,i3,uc)+uu(i1,i2,i3,vc)*
     & uy22(i1,i2,i3,uc)))  )+normal11(i1,i2,i3,1)*( (nuT*(uxx22(i1,
     & i2,i3,vc)-uxy22(i1,i2,i3,uc))-2.*nuTy*(ux22(i1,i2,i3,uc))+nuTx*
     & (ux22(i1,i2,i3,vc)+uy22(i1,i2,i3,uc)))-(advectCoeff*(uu(i1,i2,
     & i3,uc)*ux22(i1,i2,i3,vc)+uu(i1,i2,i3,vc)*uy22(i1,i2,i3,vc)))  )
                           end if
                         end do
                         end do
                         end do
                       ! For the Boussinesq approximation:  -alpha*T* normal11.gravity
                       if( pdeModel.eq.BoussinesqModel .or. 
     & pdeModel.eq.viscoPlasticModel )then
                           do i3=n3a,n3b
                           do i2=n2a,n2b
                           do i1=n1a,n1b
                             if( mask(i1,i2,i3).ne.0 )then
                             f(i1+is1,i2+is2,i3)=f(i1+is1,i2+is2,i3)-
     & thermalExpansivity*u(i1,i2,i3,tc)*(gravity(0)*normal11(i1,i2,
     & i3,0)+gravity(1)*normal11(i1,i2,i3,1))
                             end if
                           end do
                           end do
                           end do
                       else if( pdeModel.eq.twoPhaseFlowModel )then
                           do i3=n3a,n3b
                           do i2=n2a,n2b
                           do i1=n1a,n1b
                             if( mask(i1,i2,i3).ne.0 )then
                             f(i1+is1,i2+is2,i3)=f(i1+is1,i2+is2,i3)+u(
     & i1,i2,i3,rc)*(gravity(0)*normal11(i1,i2,i3,0)+gravity(1)*
     & normal11(i1,i2,i3,1))
                             end if
                           end do
                           end do
                           end do
                       end if
                       ! -- include contribution to BC from the body force --- *wdh* 2012/07/06
                       if( turnOnBodyForcing.eq.1 )then
                         ! write(*,'(" *** inspf: add body force to pressure BC ***")') 
                           do i3=n3a,n3b
                           do i2=n2a,n2b
                           do i1=n1a,n1b
                             if( mask(i1,i2,i3).ne.0 )then
                             f(i1+is1,i2+is2,i3)=f(i1+is1,i2+is2,i3)+
     & normal11(i1,i2,i3,0)*udf(i1,i2,i3,uc)+normal11(i1,i2,i3,1)*udf(
     & i1,i2,i3,vc)
                             end if
                           end do
                           end do
                           end do
                       end if
                       ! end points that meet a wall are treated differently to make sure p.x=0 where it should be
                       ! --> do not use the curl-curl boundary condition
                       do kd=1,nd-1 ! loop over the two-tangential directions
                        axisp=mod(axis+kd,nd)
                        do sidep1=0,1
                         ! For now: do not apply the corner correction for TZ flow (we need to fix TZ forcing for this case)
                         if( bc(sidep1,axisp)
     & .eq.inflowWithVelocityGiven .or. bc(sidep1,axisp)
     & .eq.noSlipWall .and. twilightZoneFlow.eq.0 )then
                           if( axisp.eq.0 )then
                             n1a=indexRange(sidep1,0)
                             n1b=indexRange(sidep1,0)
                           else if( axisp.eq.1 )then
                             n2a=indexRange(sidep1,1)
                             n2b=indexRange(sidep1,1)
                           else
                             n3a=indexRange(sidep1,2)
                             n3b=indexRange(sidep1,2)
                           end if
                             do i3=n3a,n3b
                             do i2=n2a,n2b
                             do i1=n1a,n1b
                               if( mask(i1,i2,i3).ne.0 )then
                              ! Define derivative macros before calling this macro
                              ! By default there is no AD:
                                ! get nuT,nuTx,nuTy,nuTz
                                     nuT = u(i1,i2,i3,vsc)
                                     nuTx=ux22(i1,i2,i3,vsc)
                                     nuTy=uy22(i1,i2,i3,vsc)
                                    ! normal11-form of the diffusion operator in 2D
                              ! Artificial Dissipation  **todo** implicit-line, self-adjoint versions
                              ! Here now is the statement where the ghost line value in the RHS is assigned.
                                  f(i1+is1,i2+is2,i3)=normal11(i1,i2,
     & i3,0)*( (nuT*(ulaplacian22(i1,i2,i3,uc))+2.*nuTx*(ux22(i1,i2,
     & i3,uc))+nuTy*(uy22(i1,i2,i3,uc)+ux22(i1,i2,i3,vc)))-(
     & advectCoeff*(uu(i1,i2,i3,uc)*ux22(i1,i2,i3,uc)+uu(i1,i2,i3,vc)*
     & uy22(i1,i2,i3,uc)))  )+normal11(i1,i2,i3,1)*( (nuT*(
     & ulaplacian22(i1,i2,i3,vc))+2.*nuTy*(uy22(i1,i2,i3,vc))+nuTx*(
     & ux22(i1,i2,i3,vc)+uy22(i1,i2,i3,uc)))-(advectCoeff*(uu(i1,i2,
     & i3,uc)*ux22(i1,i2,i3,vc)+uu(i1,i2,i3,vc)*uy22(i1,i2,i3,vc)))  )
                               end if
                             end do
                             end do
                             end do
                           ! For the Boussinesq approximation:  -alpha*T* normal11.gravity
                           if( pdeModel.eq.BoussinesqModel .or. 
     & pdeModel.eq.viscoPlasticModel )then
                               do i3=n3a,n3b
                               do i2=n2a,n2b
                               do i1=n1a,n1b
                                 if( mask(i1,i2,i3).ne.0 )then
                                 f(i1+is1,i2+is2,i3)=f(i1+is1,i2+is2,
     & i3)-thermalExpansivity*u(i1,i2,i3,tc)*(gravity(0)*normal11(i1,
     & i2,i3,0)+gravity(1)*normal11(i1,i2,i3,1))
                                 end if
                               end do
                               end do
                               end do
                           else if( pdeModel.eq.twoPhaseFlowModel )then
                               do i3=n3a,n3b
                               do i2=n2a,n2b
                               do i1=n1a,n1b
                                 if( mask(i1,i2,i3).ne.0 )then
                                 f(i1+is1,i2+is2,i3)=f(i1+is1,i2+is2,
     & i3)+u(i1,i2,i3,rc)*(gravity(0)*normal11(i1,i2,i3,0)+gravity(1)*
     & normal11(i1,i2,i3,1))
                                 end if
                               end do
                               end do
                               end do
                           end if
                           ! -- include contribution to BC from the body force ---
                           if( turnOnBodyForcing.eq.1 )then
                               do i3=n3a,n3b
                               do i2=n2a,n2b
                               do i1=n1a,n1b
                                 if( mask(i1,i2,i3).ne.0 )then
                                 f(i1+is1,i2+is2,i3)=f(i1+is1,i2+is2,
     & i3)+normal11(i1,i2,i3,0)*udf(i1,i2,i3,uc)+normal11(i1,i2,i3,1)*
     & udf(i1,i2,i3,vc)
                                 end if
                               end do
                               end do
                               end do
                           end if
                           ! reset loop bounds
                           if( axisp.eq.0 )then
                             n1a=indexRange(0,0)
                             n1b=indexRange(1,0)
                           else if( axisp.eq.1 )then
                             n2a=indexRange(0,1)
                             n2b=indexRange(1,1)
                           else
                             n3a=indexRange(0,2)
                             n3b=indexRange(1,2)
                           end if
                          end if
                        end do
                       end do
                   else
                     stop 33
                   end if
                else
                  stop 35
                end if
            ! end if
          end if ! end bc
         end do ! side
        end do ! axis
        return
        end
