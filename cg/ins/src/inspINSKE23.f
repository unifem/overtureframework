! This file automatically generated from inspf.bf with bpp.
        subroutine assignPressureRhsINSKE23(nd,  n1a,n1b,n2a,n2b,n3a,
     & n3b,  nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,  mask,rsxy,  u,
     & uu,f,gv,divDamping,  bc, indexRange, ndb, bcData,   nr1a,nr1b,
     & nr2a,nr2b,nr3a,nr3b,  normal00,normal10,normal01,normal11,
     & normal02,normal12,  ipar, rpar, ierr )
         implicit none
         integer nd, n1a,n1b,n2a,n2b,n3a,n3b, nd1a,nd1b,nd2a,nd2b,nd3a,
     & nd3b,nd4a,nd4b,ndb
         integer nr1a,nr1b,nr2a,nr2b,nr3a,nr3b
         real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
         real uu(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
         real f(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
         real gv(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
         real divDamping(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
         real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
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
     & interfaceBoundaryCondition
         parameter( noSlipWall=1,inflowWithVelocityGiven=2, outflow=5,
     & convectiveOutflow=14,tractionFree=15, inflowWithPandTV=3, 
     & dirichletBoundaryCondition=12, symmetry=11,axisymmetric=13,
     & interfaceBoundaryCondition=17 )
         integer pdeModel,standardModel,BoussinesqModel,
     & viscoPlasticModel
         parameter( standardModel=0,BoussinesqModel=1,
     & viscoPlasticModel=2 )
c  enum BoundaryCondition
c  {
c0    interpolation=0,
c1    noSlipWall,
c2    inflowWithVelocityGiven,
c3    inflowWithPressureAndTangentialVelocityGiven,
c4    slipWall,
c5    outflow,
c6    superSonicInflow,
c7    superSonicOutflow,
c8    subSonicInflow,
c9    subSonicInflow2,
c0    subSonicOutflow,
c1    symmetry,
c2    dirichletBoundaryCondition,
c3    axisymmetric,
c4    convectiveOutflow,  
c5    tractionFree,
c6    numberOfBCNames     // counts number of entries
c  };
c     ---- local variables -----
         integer c,i1,i2,i3,orderOfAccuracy,gridIsMoving,useWhereMask
         integer isAxisymmetric,is1,is2,is3,pressureBC,gridType
         integer pc,uc,vc,wc,grid,side,axis,bc0,numberOfComponents,
     & axisp1,axisp2,sidep1,sidep2,axisp
         integer nc,tc,vsc
         real nu,dt,advectionCoefficient,inflowPressure,a1
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
         integer rectangular,curvilinear
         parameter( rectangular=0, curvilinear=1 )
         integer turbulenceModel,noTurbulenceModel
         integer baldwinLomax,spalartAllmaras,kEpsilon,kOmega
         parameter (noTurbulenceModel=0,baldwinLomax=1,kEpsilon=2,
     & kOmega=3,spalartAllmaras=4 )
         integer m,n,kd,kdd,kd3,ndc,dir
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
c     .....begin statement functions
         real rx,ry,rz,sx,sy,sz,tx,ty,tz
         real dr(0:2), dx(0:2)
         ! include 'declareDiffOrder2f.h'
         ! include 'declareDiffOrder4f.h'
        ! declareDifferenceOrder2(u,RX)
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
         real ad2,ad23,ad4,ad43
c     --- begin statement functions
c    --- 2nd order 2D artificial diffusion ---
         ad2(c)=    (u(i1+1,i2,i3,c)-4.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c)  
     & +u(i1,i2+1,i3,c)                 +u(i1,i2-1,i3,c))
c    --- 2nd order 3D artificial diffusion ---
         ad23(c)= (u(i1+1,i2,i3,c)-6.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c)   +
     & u(i1,i2+1,i3,c)                   +u(i1,i2-1,i3,c)  +u(i1,i2,
     & i3+1,c)                   +u(i1,i2,i3-1,c))
c     ---fourth-order artificial diffusion in 2D
         ad4(c)= (   -u(i1+2,i2,i3,c)-u(i1-2,i2,i3,c)   -u(i1,i2+2,i3,
     & c)-u(i1,i2-2,i3,c)   +4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)   +u(
     & i1,i2+1,i3,c)+u(i1,i2-1,i3,c))  -12.*u(i1,i2,i3,c) )
c     ---fourth-order artificial diffusion in 3D
         ad43(c)= (   -u(i1+2,i2,i3,c)-u(i1-2,i2,i3,c)  -u(i1,i2+2,i3,
     & c)-u(i1,i2-2,i3,c)  -u(i1,i2,i3+2,c)-u(i1,i2,i3-2,c)  +4.*(u(
     & i1+1,i2,i3,c)+u(i1-1,i2,i3,c)  +u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c)
     &   +u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c)) -18.*u(i1,i2,i3,c) )
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
c  defineDifferenceOrder2Components1(u,RX)
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
         d14(kd) = 1./(12.*dr(kd))
         d24(kd) = 1./(12.*dr(kd)**2)
         ur4(i1,i2,i3,kd)=(8.*(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))-(u(
     & i1+2,i2,i3,kd)-u(i1-2,i2,i3,kd)))*d14(0)
         us4(i1,i2,i3,kd)=(8.*(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))-(u(
     & i1,i2+2,i3,kd)-u(i1,i2-2,i3,kd)))*d14(1)
         ut4(i1,i2,i3,kd)=(8.*(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))-(u(
     & i1,i2,i3+2,kd)-u(i1,i2,i3-2,kd)))*d14(2)
         urr4(i1,i2,i3,kd)=(-30.*u(i1,i2,i3,kd)+16.*(u(i1+1,i2,i3,kd)+
     & u(i1-1,i2,i3,kd))-(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd)) )*d24(0)
         uss4(i1,i2,i3,kd)=(-30.*u(i1,i2,i3,kd)+16.*(u(i1,i2+1,i3,kd)+
     & u(i1,i2-1,i3,kd))-(u(i1,i2+2,i3,kd)+u(i1,i2-2,i3,kd)) )*d24(1)
         utt4(i1,i2,i3,kd)=(-30.*u(i1,i2,i3,kd)+16.*(u(i1,i2,i3+1,kd)+
     & u(i1,i2,i3-1,kd))-(u(i1,i2,i3+2,kd)+u(i1,i2,i3-2,kd)) )*d24(2)
         urs4(i1,i2,i3,kd)=(8.*(ur4(i1,i2+1,i3,kd)-ur4(i1,i2-1,i3,kd))-
     & (ur4(i1,i2+2,i3,kd)-ur4(i1,i2-2,i3,kd)))*d14(1)
         urt4(i1,i2,i3,kd)=(8.*(ur4(i1,i2,i3+1,kd)-ur4(i1,i2,i3-1,kd))-
     & (ur4(i1,i2,i3+2,kd)-ur4(i1,i2,i3-2,kd)))*d14(2)
         ust4(i1,i2,i3,kd)=(8.*(us4(i1,i2,i3+1,kd)-us4(i1,i2,i3-1,kd))-
     & (us4(i1,i2,i3+2,kd)-us4(i1,i2,i3-2,kd)))*d14(2)
         rxr4(i1,i2,i3)=(8.*(rx(i1+1,i2,i3)-rx(i1-1,i2,i3))-(rx(i1+2,
     & i2,i3)-rx(i1-2,i2,i3)))*d14(0)
         rxs4(i1,i2,i3)=(8.*(rx(i1,i2+1,i3)-rx(i1,i2-1,i3))-(rx(i1,i2+
     & 2,i3)-rx(i1,i2-2,i3)))*d14(1)
         rxt4(i1,i2,i3)=(8.*(rx(i1,i2,i3+1)-rx(i1,i2,i3-1))-(rx(i1,i2,
     & i3+2)-rx(i1,i2,i3-2)))*d14(2)
         ryr4(i1,i2,i3)=(8.*(ry(i1+1,i2,i3)-ry(i1-1,i2,i3))-(ry(i1+2,
     & i2,i3)-ry(i1-2,i2,i3)))*d14(0)
         rys4(i1,i2,i3)=(8.*(ry(i1,i2+1,i3)-ry(i1,i2-1,i3))-(ry(i1,i2+
     & 2,i3)-ry(i1,i2-2,i3)))*d14(1)
         ryt4(i1,i2,i3)=(8.*(ry(i1,i2,i3+1)-ry(i1,i2,i3-1))-(ry(i1,i2,
     & i3+2)-ry(i1,i2,i3-2)))*d14(2)
         rzr4(i1,i2,i3)=(8.*(rz(i1+1,i2,i3)-rz(i1-1,i2,i3))-(rz(i1+2,
     & i2,i3)-rz(i1-2,i2,i3)))*d14(0)
         rzs4(i1,i2,i3)=(8.*(rz(i1,i2+1,i3)-rz(i1,i2-1,i3))-(rz(i1,i2+
     & 2,i3)-rz(i1,i2-2,i3)))*d14(1)
         rzt4(i1,i2,i3)=(8.*(rz(i1,i2,i3+1)-rz(i1,i2,i3-1))-(rz(i1,i2,
     & i3+2)-rz(i1,i2,i3-2)))*d14(2)
         sxr4(i1,i2,i3)=(8.*(sx(i1+1,i2,i3)-sx(i1-1,i2,i3))-(sx(i1+2,
     & i2,i3)-sx(i1-2,i2,i3)))*d14(0)
         sxs4(i1,i2,i3)=(8.*(sx(i1,i2+1,i3)-sx(i1,i2-1,i3))-(sx(i1,i2+
     & 2,i3)-sx(i1,i2-2,i3)))*d14(1)
         sxt4(i1,i2,i3)=(8.*(sx(i1,i2,i3+1)-sx(i1,i2,i3-1))-(sx(i1,i2,
     & i3+2)-sx(i1,i2,i3-2)))*d14(2)
         syr4(i1,i2,i3)=(8.*(sy(i1+1,i2,i3)-sy(i1-1,i2,i3))-(sy(i1+2,
     & i2,i3)-sy(i1-2,i2,i3)))*d14(0)
         sys4(i1,i2,i3)=(8.*(sy(i1,i2+1,i3)-sy(i1,i2-1,i3))-(sy(i1,i2+
     & 2,i3)-sy(i1,i2-2,i3)))*d14(1)
         syt4(i1,i2,i3)=(8.*(sy(i1,i2,i3+1)-sy(i1,i2,i3-1))-(sy(i1,i2,
     & i3+2)-sy(i1,i2,i3-2)))*d14(2)
         szr4(i1,i2,i3)=(8.*(sz(i1+1,i2,i3)-sz(i1-1,i2,i3))-(sz(i1+2,
     & i2,i3)-sz(i1-2,i2,i3)))*d14(0)
         szs4(i1,i2,i3)=(8.*(sz(i1,i2+1,i3)-sz(i1,i2-1,i3))-(sz(i1,i2+
     & 2,i3)-sz(i1,i2-2,i3)))*d14(1)
         szt4(i1,i2,i3)=(8.*(sz(i1,i2,i3+1)-sz(i1,i2,i3-1))-(sz(i1,i2,
     & i3+2)-sz(i1,i2,i3-2)))*d14(2)
         txr4(i1,i2,i3)=(8.*(tx(i1+1,i2,i3)-tx(i1-1,i2,i3))-(tx(i1+2,
     & i2,i3)-tx(i1-2,i2,i3)))*d14(0)
         txs4(i1,i2,i3)=(8.*(tx(i1,i2+1,i3)-tx(i1,i2-1,i3))-(tx(i1,i2+
     & 2,i3)-tx(i1,i2-2,i3)))*d14(1)
         txt4(i1,i2,i3)=(8.*(tx(i1,i2,i3+1)-tx(i1,i2,i3-1))-(tx(i1,i2,
     & i3+2)-tx(i1,i2,i3-2)))*d14(2)
         tyr4(i1,i2,i3)=(8.*(ty(i1+1,i2,i3)-ty(i1-1,i2,i3))-(ty(i1+2,
     & i2,i3)-ty(i1-2,i2,i3)))*d14(0)
         tys4(i1,i2,i3)=(8.*(ty(i1,i2+1,i3)-ty(i1,i2-1,i3))-(ty(i1,i2+
     & 2,i3)-ty(i1,i2-2,i3)))*d14(1)
         tyt4(i1,i2,i3)=(8.*(ty(i1,i2,i3+1)-ty(i1,i2,i3-1))-(ty(i1,i2,
     & i3+2)-ty(i1,i2,i3-2)))*d14(2)
         tzr4(i1,i2,i3)=(8.*(tz(i1+1,i2,i3)-tz(i1-1,i2,i3))-(tz(i1+2,
     & i2,i3)-tz(i1-2,i2,i3)))*d14(0)
         tzs4(i1,i2,i3)=(8.*(tz(i1,i2+1,i3)-tz(i1,i2-1,i3))-(tz(i1,i2+
     & 2,i3)-tz(i1,i2-2,i3)))*d14(1)
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
         rxx42(i1,i2,i3)= rx(i1,i2,i3)*rxr4(i1,i2,i3)+sx(i1,i2,i3)*
     & rxs4(i1,i2,i3)
         rxy42(i1,i2,i3)= ry(i1,i2,i3)*rxr4(i1,i2,i3)+sy(i1,i2,i3)*
     & rxs4(i1,i2,i3)
         rxx43(i1,i2,i3)=rx(i1,i2,i3)*rxr4(i1,i2,i3)+sx(i1,i2,i3)*rxs4(
     & i1,i2,i3)+tx(i1,i2,i3)*rxt4(i1,i2,i3)
         rxy43(i1,i2,i3)=ry(i1,i2,i3)*rxr4(i1,i2,i3)+sy(i1,i2,i3)*rxs4(
     & i1,i2,i3)+ty(i1,i2,i3)*rxt4(i1,i2,i3)
         rxz43(i1,i2,i3)=rz(i1,i2,i3)*rxr4(i1,i2,i3)+sz(i1,i2,i3)*rxs4(
     & i1,i2,i3)+tz(i1,i2,i3)*rxt4(i1,i2,i3)
         ryx42(i1,i2,i3)= rx(i1,i2,i3)*ryr4(i1,i2,i3)+sx(i1,i2,i3)*
     & rys4(i1,i2,i3)
         ryy42(i1,i2,i3)= ry(i1,i2,i3)*ryr4(i1,i2,i3)+sy(i1,i2,i3)*
     & rys4(i1,i2,i3)
         ryx43(i1,i2,i3)=rx(i1,i2,i3)*ryr4(i1,i2,i3)+sx(i1,i2,i3)*rys4(
     & i1,i2,i3)+tx(i1,i2,i3)*ryt4(i1,i2,i3)
         ryy43(i1,i2,i3)=ry(i1,i2,i3)*ryr4(i1,i2,i3)+sy(i1,i2,i3)*rys4(
     & i1,i2,i3)+ty(i1,i2,i3)*ryt4(i1,i2,i3)
         ryz43(i1,i2,i3)=rz(i1,i2,i3)*ryr4(i1,i2,i3)+sz(i1,i2,i3)*rys4(
     & i1,i2,i3)+tz(i1,i2,i3)*ryt4(i1,i2,i3)
         rzx42(i1,i2,i3)= rx(i1,i2,i3)*rzr4(i1,i2,i3)+sx(i1,i2,i3)*
     & rzs4(i1,i2,i3)
         rzy42(i1,i2,i3)= ry(i1,i2,i3)*rzr4(i1,i2,i3)+sy(i1,i2,i3)*
     & rzs4(i1,i2,i3)
         rzx43(i1,i2,i3)=rx(i1,i2,i3)*rzr4(i1,i2,i3)+sx(i1,i2,i3)*rzs4(
     & i1,i2,i3)+tx(i1,i2,i3)*rzt4(i1,i2,i3)
         rzy43(i1,i2,i3)=ry(i1,i2,i3)*rzr4(i1,i2,i3)+sy(i1,i2,i3)*rzs4(
     & i1,i2,i3)+ty(i1,i2,i3)*rzt4(i1,i2,i3)
         rzz43(i1,i2,i3)=rz(i1,i2,i3)*rzr4(i1,i2,i3)+sz(i1,i2,i3)*rzs4(
     & i1,i2,i3)+tz(i1,i2,i3)*rzt4(i1,i2,i3)
         sxx42(i1,i2,i3)= rx(i1,i2,i3)*sxr4(i1,i2,i3)+sx(i1,i2,i3)*
     & sxs4(i1,i2,i3)
         sxy42(i1,i2,i3)= ry(i1,i2,i3)*sxr4(i1,i2,i3)+sy(i1,i2,i3)*
     & sxs4(i1,i2,i3)
         sxx43(i1,i2,i3)=rx(i1,i2,i3)*sxr4(i1,i2,i3)+sx(i1,i2,i3)*sxs4(
     & i1,i2,i3)+tx(i1,i2,i3)*sxt4(i1,i2,i3)
         sxy43(i1,i2,i3)=ry(i1,i2,i3)*sxr4(i1,i2,i3)+sy(i1,i2,i3)*sxs4(
     & i1,i2,i3)+ty(i1,i2,i3)*sxt4(i1,i2,i3)
         sxz43(i1,i2,i3)=rz(i1,i2,i3)*sxr4(i1,i2,i3)+sz(i1,i2,i3)*sxs4(
     & i1,i2,i3)+tz(i1,i2,i3)*sxt4(i1,i2,i3)
         syx42(i1,i2,i3)= rx(i1,i2,i3)*syr4(i1,i2,i3)+sx(i1,i2,i3)*
     & sys4(i1,i2,i3)
         syy42(i1,i2,i3)= ry(i1,i2,i3)*syr4(i1,i2,i3)+sy(i1,i2,i3)*
     & sys4(i1,i2,i3)
         syx43(i1,i2,i3)=rx(i1,i2,i3)*syr4(i1,i2,i3)+sx(i1,i2,i3)*sys4(
     & i1,i2,i3)+tx(i1,i2,i3)*syt4(i1,i2,i3)
         syy43(i1,i2,i3)=ry(i1,i2,i3)*syr4(i1,i2,i3)+sy(i1,i2,i3)*sys4(
     & i1,i2,i3)+ty(i1,i2,i3)*syt4(i1,i2,i3)
         syz43(i1,i2,i3)=rz(i1,i2,i3)*syr4(i1,i2,i3)+sz(i1,i2,i3)*sys4(
     & i1,i2,i3)+tz(i1,i2,i3)*syt4(i1,i2,i3)
         szx42(i1,i2,i3)= rx(i1,i2,i3)*szr4(i1,i2,i3)+sx(i1,i2,i3)*
     & szs4(i1,i2,i3)
         szy42(i1,i2,i3)= ry(i1,i2,i3)*szr4(i1,i2,i3)+sy(i1,i2,i3)*
     & szs4(i1,i2,i3)
         szx43(i1,i2,i3)=rx(i1,i2,i3)*szr4(i1,i2,i3)+sx(i1,i2,i3)*szs4(
     & i1,i2,i3)+tx(i1,i2,i3)*szt4(i1,i2,i3)
         szy43(i1,i2,i3)=ry(i1,i2,i3)*szr4(i1,i2,i3)+sy(i1,i2,i3)*szs4(
     & i1,i2,i3)+ty(i1,i2,i3)*szt4(i1,i2,i3)
         szz43(i1,i2,i3)=rz(i1,i2,i3)*szr4(i1,i2,i3)+sz(i1,i2,i3)*szs4(
     & i1,i2,i3)+tz(i1,i2,i3)*szt4(i1,i2,i3)
         txx42(i1,i2,i3)= rx(i1,i2,i3)*txr4(i1,i2,i3)+sx(i1,i2,i3)*
     & txs4(i1,i2,i3)
         txy42(i1,i2,i3)= ry(i1,i2,i3)*txr4(i1,i2,i3)+sy(i1,i2,i3)*
     & txs4(i1,i2,i3)
         txx43(i1,i2,i3)=rx(i1,i2,i3)*txr4(i1,i2,i3)+sx(i1,i2,i3)*txs4(
     & i1,i2,i3)+tx(i1,i2,i3)*txt4(i1,i2,i3)
         txy43(i1,i2,i3)=ry(i1,i2,i3)*txr4(i1,i2,i3)+sy(i1,i2,i3)*txs4(
     & i1,i2,i3)+ty(i1,i2,i3)*txt4(i1,i2,i3)
         txz43(i1,i2,i3)=rz(i1,i2,i3)*txr4(i1,i2,i3)+sz(i1,i2,i3)*txs4(
     & i1,i2,i3)+tz(i1,i2,i3)*txt4(i1,i2,i3)
         tyx42(i1,i2,i3)= rx(i1,i2,i3)*tyr4(i1,i2,i3)+sx(i1,i2,i3)*
     & tys4(i1,i2,i3)
         tyy42(i1,i2,i3)= ry(i1,i2,i3)*tyr4(i1,i2,i3)+sy(i1,i2,i3)*
     & tys4(i1,i2,i3)
         tyx43(i1,i2,i3)=rx(i1,i2,i3)*tyr4(i1,i2,i3)+sx(i1,i2,i3)*tys4(
     & i1,i2,i3)+tx(i1,i2,i3)*tyt4(i1,i2,i3)
         tyy43(i1,i2,i3)=ry(i1,i2,i3)*tyr4(i1,i2,i3)+sy(i1,i2,i3)*tys4(
     & i1,i2,i3)+ty(i1,i2,i3)*tyt4(i1,i2,i3)
         tyz43(i1,i2,i3)=rz(i1,i2,i3)*tyr4(i1,i2,i3)+sz(i1,i2,i3)*tys4(
     & i1,i2,i3)+tz(i1,i2,i3)*tyt4(i1,i2,i3)
         tzx42(i1,i2,i3)= rx(i1,i2,i3)*tzr4(i1,i2,i3)+sx(i1,i2,i3)*
     & tzs4(i1,i2,i3)
         tzy42(i1,i2,i3)= ry(i1,i2,i3)*tzr4(i1,i2,i3)+sy(i1,i2,i3)*
     & tzs4(i1,i2,i3)
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
         uxy42(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*urr4(i1,i2,i3,kd)
     & +(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*urs4(i1,
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
         uxy43(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*urr4(i1,i2,i3,kd)
     & +sx(i1,i2,i3)*sy(i1,i2,i3)*uss4(i1,i2,i3,kd)+tx(i1,i2,i3)*ty(
     & i1,i2,i3)*utt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,
     & i2,i3)*sx(i1,i2,i3))*urs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(i1,i2,
     & i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*urt4(i1,i2,i3,kd)+(sx(i1,i2,i3)*
     & ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*ust4(i1,i2,i3,kd)+
     & rxy43(i1,i2,i3)*ur4(i1,i2,i3,kd)+sxy43(i1,i2,i3)*us4(i1,i2,i3,
     & kd)+txy43(i1,i2,i3)*ut4(i1,i2,i3,kd)
         uxz43(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*urr4(i1,i2,i3,kd)
     & +sx(i1,i2,i3)*sz(i1,i2,i3)*uss4(i1,i2,i3,kd)+tx(i1,i2,i3)*tz(
     & i1,i2,i3)*utt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,
     & i2,i3)*sx(i1,i2,i3))*urs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(i1,i2,
     & i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*urt4(i1,i2,i3,kd)+(sx(i1,i2,i3)*
     & tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*ust4(i1,i2,i3,kd)+
     & rxz43(i1,i2,i3)*ur4(i1,i2,i3,kd)+sxz43(i1,i2,i3)*us4(i1,i2,i3,
     & kd)+txz43(i1,i2,i3)*ut4(i1,i2,i3,kd)
         uyz43(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*urr4(i1,i2,i3,kd)
     & +sy(i1,i2,i3)*sz(i1,i2,i3)*uss4(i1,i2,i3,kd)+ty(i1,i2,i3)*tz(
     & i1,i2,i3)*utt4(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,
     & i2,i3)*sy(i1,i2,i3))*urs4(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(i1,i2,
     & i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*urt4(i1,i2,i3,kd)+(sy(i1,i2,i3)*
     & tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*ust4(i1,i2,i3,kd)+
     & ryz43(i1,i2,i3)*ur4(i1,i2,i3,kd)+syz43(i1,i2,i3)*us4(i1,i2,i3,
     & kd)+tyz43(i1,i2,i3)*ut4(i1,i2,i3,kd)
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
         uxx43r(i1,i2,i3,kd)=( -30.*u(i1,i2,i3,kd)+16.*(u(i1+1,i2,i3,
     & kd)+u(i1-1,i2,i3,kd))-(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd)) )*
     & h42(0)
         uyy43r(i1,i2,i3,kd)=( -30.*u(i1,i2,i3,kd)+16.*(u(i1,i2+1,i3,
     & kd)+u(i1,i2-1,i3,kd))-(u(i1,i2+2,i3,kd)+u(i1,i2-2,i3,kd)) )*
     & h42(1)
         uzz43r(i1,i2,i3,kd)=( -30.*u(i1,i2,i3,kd)+16.*(u(i1,i2,i3+1,
     & kd)+u(i1,i2,i3-1,kd))-(u(i1,i2,i3+2,kd)+u(i1,i2,i3-2,kd)) )*
     & h42(2)
         uxy43r(i1,i2,i3,kd)=( (u(i1+2,i2+2,i3,kd)-u(i1-2,i2+2,i3,kd)- 
     & u(i1+2,i2-2,i3,kd)+u(i1-2,i2-2,i3,kd)) +8.*(u(i1-1,i2+2,i3,kd)-
     & u(i1-1,i2-2,i3,kd)-u(i1+1,i2+2,i3,kd)+u(i1+1,i2-2,i3,kd) +u(i1+
     & 2,i2-1,i3,kd)-u(i1-2,i2-1,i3,kd)-u(i1+2,i2+1,i3,kd)+u(i1-2,i2+
     & 1,i3,kd))+64.*(u(i1+1,i2+1,i3,kd)-u(i1-1,i2+1,i3,kd)- u(i1+1,
     & i2-1,i3,kd)+u(i1-1,i2-1,i3,kd)))*(h41(0)*h41(1))
         uxz43r(i1,i2,i3,kd)=( (u(i1+2,i2,i3+2,kd)-u(i1-2,i2,i3+2,kd)-
     & u(i1+2,i2,i3-2,kd)+u(i1-2,i2,i3-2,kd)) +8.*(u(i1-1,i2,i3+2,kd)-
     & u(i1-1,i2,i3-2,kd)-u(i1+1,i2,i3+2,kd)+u(i1+1,i2,i3-2,kd) +u(i1+
     & 2,i2,i3-1,kd)-u(i1-2,i2,i3-1,kd)- u(i1+2,i2,i3+1,kd)+u(i1-2,i2,
     & i3+1,kd)) +64.*(u(i1+1,i2,i3+1,kd)-u(i1-1,i2,i3+1,kd)-u(i1+1,
     & i2,i3-1,kd)+u(i1-1,i2,i3-1,kd)) )*(h41(0)*h41(2))
         uyz43r(i1,i2,i3,kd)=( (u(i1,i2+2,i3+2,kd)-u(i1,i2-2,i3+2,kd)-
     & u(i1,i2+2,i3-2,kd)+u(i1,i2-2,i3-2,kd)) +8.*(u(i1,i2-1,i3+2,kd)-
     & u(i1,i2-1,i3-2,kd)-u(i1,i2+1,i3+2,kd)+u(i1,i2+1,i3-2,kd) +u(i1,
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
         ulaplacian42r(i1,i2,i3,kd)=uxx43r(i1,i2,i3,kd)+uyy43r(i1,i2,
     & i3,kd)
         ulaplacian43r(i1,i2,i3,kd)=uxx43r(i1,i2,i3,kd)+uyy43r(i1,i2,
     & i3,kd)+uzz43r(i1,i2,i3,kd)
c     --- end statement functions
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
        !  nuVP                =rpar(18) ! visco-plastic parameters
        !  etaVP               =rpar(19)
        !  yieldStressVP       =rpar(20)
        !  exponentVP          =rpar(21)
        !  epsVP               =rpar(22)
         cd22=ad22/(nd**2)
         cd42=ad42/(nd**2)
         kc=nc
         ec=kc+1
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
         call getKEpsilonParameters( cMu,cEps1,cEps2,sigmaEpsI,sigmaKI 
     & )
        if( gridType.eq.rectangular )then
           if( useWhereMask.ne.0 )then
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
               if( mask(i1,i2,i3).ne.0 )then
                   u0x=ux23r(i1,i2,i3,uc)
                   u0y=uy23r(i1,i2,i3,uc)
                   v0x=ux23r(i1,i2,i3,vc)
                   v0y=uy23r(i1,i2,i3,vc)
                   u0Lap=ulaplacian23r(i1,i2,i3,uc)
                   v0Lap=ulaplacian23r(i1,i2,i3,vc)
                    u0z=uz23r(i1,i2,i3,uc)
                    v0z=uz23r(i1,i2,i3,vc)
                    w0x=ux23r(i1,i2,i3,wc)
                    w0y=uy23r(i1,i2,i3,wc)
                    w0z=uz23r(i1,i2,i3,wc)
                    w0Lap=ulaplacian23r(i1,i2,i3,wc)
                   k0=u(i1,i2,i3,kc)
                   e0=u(i1,i2,i3,ec)
                   nuT = nu + cMu*k0**2/e0
                   k0x=ux23r(i1,i2,i3,kc)
                   e0x=ux23r(i1,i2,i3,ec)
                   k0y=uy23r(i1,i2,i3,kc)
                   e0y=uy23r(i1,i2,i3,ec)
                   nuTx=cMu*k0*( 2.*k0x*e0 - k0*e0x )/e0**2
                   nuTy=cMu*k0*( 2.*k0y*e0 - k0*e0y )/e0**2
                   k0xx=uxx23r(i1,i2,i3,kc)
                   k0xy=uxy23r(i1,i2,i3,kc)
                   k0yy=uyy23r(i1,i2,i3,kc)
                   e0xx=uxx23r(i1,i2,i3,ec)
                   e0xy=uxy23r(i1,i2,i3,ec)
                   e0yy=uyy23r(i1,i2,i3,ec)
                   nuTxx=cMu*(2.*k0xx*e0**2-4.*k0*k0x*e0x*e0+2*k0*k0xx*
     & e0**2+2*k0**2.*e0x**2-k0**2.*e0xx*e0)/e0**3
                   nuTxy=cMu*(2*k0y*k0x*e0**2-2*k0*k0x*e0y*e0+2*k0*
     & k0xy*e0**2-2*k0*e0x*k0y*e0+2*k0**2*e0x*e0y-k0**2*e0xy*e0)/e0**3
                   nuTyy=cMu*(2.*k0yy*e0**2-4.*k0*k0y*e0y*e0+2*k0*k0yy*
     & e0**2+2*k0**2.*e0y**2-k0**2.*e0yy*e0)/e0**3
                    k0z=uz23r(i1,i2,i3,kc)
                    e0z=uz23r(i1,i2,i3,ec)
                    nuTz=cMu*k0*( 2.*k0z*e0 - k0*e0z )/e0**2
                    k0xy=uxy23r(i1,i2,i3,kc)
                    k0yz=uyz23r(i1,i2,i3,kc)
                    k0zz=uzz23r(i1,i2,i3,kc)
                    e0xy=uxy23r(i1,i2,i3,ec)
                    e0xz=uxz23r(i1,i2,i3,ec)
                    e0yz=uyz23r(i1,i2,i3,ec)
                    e0zz=uzz23r(i1,i2,i3,ec)
                    nuTxz=cMu*(2*k0z*k0x*e0**2-2*k0*k0x*e0z*e0+2*k0*
     & k0xz*e0**2-2*k0*e0x*k0z*e0+2*k0**2*e0x*e0z-k0**2*e0xz*e0)/e0**3
                    nuTyz=cMu*(2*k0z*k0y*e0**2-2*k0*k0y*e0z*e0+2*k0*
     & k0yz*e0**2-2*k0*e0y*k0z*e0+2*k0**2*e0y*e0z-k0**2*e0yz*e0)/e0**3
                    nuTzz=cMu*(2.*k0zz*e0**2-4.*k0*k0z*e0z*e0+2*k0*
     & k0zz*e0**2+2*k0**2.*e0z**2-k0**2.*e0zz*e0)/e0**3
                 f(i1,i2,i3)=(-advectionCoefficient)*(u0x**2+v0y**2+
     & w0z**2+2.*(u0y*v0x+u0z*w0x+v0z*w0y))+divDamping(i1,i2,i3)*(u0x+
     & v0y+w0z)+2.*(nuTx*u0Lap+nuTxx*u0x+nuTxy*u0y+nuTxz*u0z+nuTy*
     & v0Lap+nuTxy*v0x+nuTyy*v0y+nuTyz*v0z+nuTz*w0Lap+nuTxz*w0x+nuTyz*
     & w0y+nuTzz*w0z)


               end if
             end do
             end do
             end do
           else
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
                   u0x=ux23r(i1,i2,i3,uc)
                   u0y=uy23r(i1,i2,i3,uc)
                   v0x=ux23r(i1,i2,i3,vc)
                   v0y=uy23r(i1,i2,i3,vc)
                   u0Lap=ulaplacian23r(i1,i2,i3,uc)
                   v0Lap=ulaplacian23r(i1,i2,i3,vc)
                    u0z=uz23r(i1,i2,i3,uc)
                    v0z=uz23r(i1,i2,i3,vc)
                    w0x=ux23r(i1,i2,i3,wc)
                    w0y=uy23r(i1,i2,i3,wc)
                    w0z=uz23r(i1,i2,i3,wc)
                    w0Lap=ulaplacian23r(i1,i2,i3,wc)
                   k0=u(i1,i2,i3,kc)
                   e0=u(i1,i2,i3,ec)
                   nuT = nu + cMu*k0**2/e0
                   k0x=ux23r(i1,i2,i3,kc)
                   e0x=ux23r(i1,i2,i3,ec)
                   k0y=uy23r(i1,i2,i3,kc)
                   e0y=uy23r(i1,i2,i3,ec)
                   nuTx=cMu*k0*( 2.*k0x*e0 - k0*e0x )/e0**2
                   nuTy=cMu*k0*( 2.*k0y*e0 - k0*e0y )/e0**2
                   k0xx=uxx23r(i1,i2,i3,kc)
                   k0xy=uxy23r(i1,i2,i3,kc)
                   k0yy=uyy23r(i1,i2,i3,kc)
                   e0xx=uxx23r(i1,i2,i3,ec)
                   e0xy=uxy23r(i1,i2,i3,ec)
                   e0yy=uyy23r(i1,i2,i3,ec)
                   nuTxx=cMu*(2.*k0xx*e0**2-4.*k0*k0x*e0x*e0+2*k0*k0xx*
     & e0**2+2*k0**2.*e0x**2-k0**2.*e0xx*e0)/e0**3
                   nuTxy=cMu*(2*k0y*k0x*e0**2-2*k0*k0x*e0y*e0+2*k0*
     & k0xy*e0**2-2*k0*e0x*k0y*e0+2*k0**2*e0x*e0y-k0**2*e0xy*e0)/e0**3
                   nuTyy=cMu*(2.*k0yy*e0**2-4.*k0*k0y*e0y*e0+2*k0*k0yy*
     & e0**2+2*k0**2.*e0y**2-k0**2.*e0yy*e0)/e0**3
                    k0z=uz23r(i1,i2,i3,kc)
                    e0z=uz23r(i1,i2,i3,ec)
                    nuTz=cMu*k0*( 2.*k0z*e0 - k0*e0z )/e0**2
                    k0xy=uxy23r(i1,i2,i3,kc)
                    k0yz=uyz23r(i1,i2,i3,kc)
                    k0zz=uzz23r(i1,i2,i3,kc)
                    e0xy=uxy23r(i1,i2,i3,ec)
                    e0xz=uxz23r(i1,i2,i3,ec)
                    e0yz=uyz23r(i1,i2,i3,ec)
                    e0zz=uzz23r(i1,i2,i3,ec)
                    nuTxz=cMu*(2*k0z*k0x*e0**2-2*k0*k0x*e0z*e0+2*k0*
     & k0xz*e0**2-2*k0*e0x*k0z*e0+2*k0**2*e0x*e0z-k0**2*e0xz*e0)/e0**3
                    nuTyz=cMu*(2*k0z*k0y*e0**2-2*k0*k0y*e0z*e0+2*k0*
     & k0yz*e0**2-2*k0*e0y*k0z*e0+2*k0**2*e0y*e0z-k0**2*e0yz*e0)/e0**3
                    nuTzz=cMu*(2.*k0zz*e0**2-4.*k0*k0z*e0z*e0+2*k0*
     & k0zz*e0**2+2*k0**2.*e0z**2-k0**2.*e0zz*e0)/e0**3
                 f(i1,i2,i3)=(-advectionCoefficient)*(u0x**2+v0y**2+
     & w0z**2+2.*(u0y*v0x+u0z*w0x+v0z*w0y))+divDamping(i1,i2,i3)*(u0x+
     & v0y+w0z)+2.*(nuTx*u0Lap+nuTxx*u0x+nuTxy*u0y+nuTxz*u0z+nuTy*
     & v0Lap+nuTxy*v0x+nuTyy*v0y+nuTyz*v0z+nuTz*w0Lap+nuTxz*w0x+nuTyz*
     & w0y+nuTzz*w0z)


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
     & 0)*ux23r(i1,i2,i3,tc)+gravity(1)*uy23r(i1,i2,i3,tc)+gravity(2)*
     & uz23r(i1,i2,i3,tc))
                end if
              end do
              end do
              end do
            else
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
                  f(i1,i2,i3)=f(i1,i2,i3)-thermalExpansivity*(gravity(
     & 0)*ux23r(i1,i2,i3,tc)+gravity(1)*uy23r(i1,i2,i3,tc)+gravity(2)*
     & uz23r(i1,i2,i3,tc))
              end do
              end do
              end do
            end if
          end if
        else ! curvilinear
           if( useWhereMask.ne.0 )then
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
               if( mask(i1,i2,i3).ne.0 )then
                   u0x=ux23(i1,i2,i3,uc)
                   u0y=uy23(i1,i2,i3,uc)
                   v0x=ux23(i1,i2,i3,vc)
                   v0y=uy23(i1,i2,i3,vc)
                   u0Lap=ulaplacian23(i1,i2,i3,uc)
                   v0Lap=ulaplacian23(i1,i2,i3,vc)
                    u0z=uz23(i1,i2,i3,uc)
                    v0z=uz23(i1,i2,i3,vc)
                    w0x=ux23(i1,i2,i3,wc)
                    w0y=uy23(i1,i2,i3,wc)
                    w0z=uz23(i1,i2,i3,wc)
                    w0Lap=ulaplacian23(i1,i2,i3,wc)
                   k0=u(i1,i2,i3,kc)
                   e0=u(i1,i2,i3,ec)
                   nuT = nu + cMu*k0**2/e0
                   k0x=ux23(i1,i2,i3,kc)
                   e0x=ux23(i1,i2,i3,ec)
                   k0y=uy23(i1,i2,i3,kc)
                   e0y=uy23(i1,i2,i3,ec)
                   nuTx=cMu*k0*( 2.*k0x*e0 - k0*e0x )/e0**2
                   nuTy=cMu*k0*( 2.*k0y*e0 - k0*e0y )/e0**2
                   k0xx=uxx23(i1,i2,i3,kc)
                   k0xy=uxy23(i1,i2,i3,kc)
                   k0yy=uyy23(i1,i2,i3,kc)
                   e0xx=uxx23(i1,i2,i3,ec)
                   e0xy=uxy23(i1,i2,i3,ec)
                   e0yy=uyy23(i1,i2,i3,ec)
                   nuTxx=cMu*(2.*k0xx*e0**2-4.*k0*k0x*e0x*e0+2*k0*k0xx*
     & e0**2+2*k0**2.*e0x**2-k0**2.*e0xx*e0)/e0**3
                   nuTxy=cMu*(2*k0y*k0x*e0**2-2*k0*k0x*e0y*e0+2*k0*
     & k0xy*e0**2-2*k0*e0x*k0y*e0+2*k0**2*e0x*e0y-k0**2*e0xy*e0)/e0**3
                   nuTyy=cMu*(2.*k0yy*e0**2-4.*k0*k0y*e0y*e0+2*k0*k0yy*
     & e0**2+2*k0**2.*e0y**2-k0**2.*e0yy*e0)/e0**3
                    k0z=uz23(i1,i2,i3,kc)
                    e0z=uz23(i1,i2,i3,ec)
                    nuTz=cMu*k0*( 2.*k0z*e0 - k0*e0z )/e0**2
                    k0xy=uxy23(i1,i2,i3,kc)
                    k0yz=uyz23(i1,i2,i3,kc)
                    k0zz=uzz23(i1,i2,i3,kc)
                    e0xy=uxy23(i1,i2,i3,ec)
                    e0xz=uxz23(i1,i2,i3,ec)
                    e0yz=uyz23(i1,i2,i3,ec)
                    e0zz=uzz23(i1,i2,i3,ec)
                    nuTxz=cMu*(2*k0z*k0x*e0**2-2*k0*k0x*e0z*e0+2*k0*
     & k0xz*e0**2-2*k0*e0x*k0z*e0+2*k0**2*e0x*e0z-k0**2*e0xz*e0)/e0**3
                    nuTyz=cMu*(2*k0z*k0y*e0**2-2*k0*k0y*e0z*e0+2*k0*
     & k0yz*e0**2-2*k0*e0y*k0z*e0+2*k0**2*e0y*e0z-k0**2*e0yz*e0)/e0**3
                    nuTzz=cMu*(2.*k0zz*e0**2-4.*k0*k0z*e0z*e0+2*k0*
     & k0zz*e0**2+2*k0**2.*e0z**2-k0**2.*e0zz*e0)/e0**3
                 f(i1,i2,i3)=(-advectionCoefficient)*(u0x**2+v0y**2+
     & w0z**2+2.*(u0y*v0x+u0z*w0x+v0z*w0y))+divDamping(i1,i2,i3)*(u0x+
     & v0y+w0z)+2.*(nuTx*u0Lap+nuTxx*u0x+nuTxy*u0y+nuTxz*u0z+nuTy*
     & v0Lap+nuTxy*v0x+nuTyy*v0y+nuTyz*v0z+nuTz*w0Lap+nuTxz*w0x+nuTyz*
     & w0y+nuTzz*w0z)


               end if
             end do
             end do
             end do
           else
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
                   u0x=ux23(i1,i2,i3,uc)
                   u0y=uy23(i1,i2,i3,uc)
                   v0x=ux23(i1,i2,i3,vc)
                   v0y=uy23(i1,i2,i3,vc)
                   u0Lap=ulaplacian23(i1,i2,i3,uc)
                   v0Lap=ulaplacian23(i1,i2,i3,vc)
                    u0z=uz23(i1,i2,i3,uc)
                    v0z=uz23(i1,i2,i3,vc)
                    w0x=ux23(i1,i2,i3,wc)
                    w0y=uy23(i1,i2,i3,wc)
                    w0z=uz23(i1,i2,i3,wc)
                    w0Lap=ulaplacian23(i1,i2,i3,wc)
                   k0=u(i1,i2,i3,kc)
                   e0=u(i1,i2,i3,ec)
                   nuT = nu + cMu*k0**2/e0
                   k0x=ux23(i1,i2,i3,kc)
                   e0x=ux23(i1,i2,i3,ec)
                   k0y=uy23(i1,i2,i3,kc)
                   e0y=uy23(i1,i2,i3,ec)
                   nuTx=cMu*k0*( 2.*k0x*e0 - k0*e0x )/e0**2
                   nuTy=cMu*k0*( 2.*k0y*e0 - k0*e0y )/e0**2
                   k0xx=uxx23(i1,i2,i3,kc)
                   k0xy=uxy23(i1,i2,i3,kc)
                   k0yy=uyy23(i1,i2,i3,kc)
                   e0xx=uxx23(i1,i2,i3,ec)
                   e0xy=uxy23(i1,i2,i3,ec)
                   e0yy=uyy23(i1,i2,i3,ec)
                   nuTxx=cMu*(2.*k0xx*e0**2-4.*k0*k0x*e0x*e0+2*k0*k0xx*
     & e0**2+2*k0**2.*e0x**2-k0**2.*e0xx*e0)/e0**3
                   nuTxy=cMu*(2*k0y*k0x*e0**2-2*k0*k0x*e0y*e0+2*k0*
     & k0xy*e0**2-2*k0*e0x*k0y*e0+2*k0**2*e0x*e0y-k0**2*e0xy*e0)/e0**3
                   nuTyy=cMu*(2.*k0yy*e0**2-4.*k0*k0y*e0y*e0+2*k0*k0yy*
     & e0**2+2*k0**2.*e0y**2-k0**2.*e0yy*e0)/e0**3
                    k0z=uz23(i1,i2,i3,kc)
                    e0z=uz23(i1,i2,i3,ec)
                    nuTz=cMu*k0*( 2.*k0z*e0 - k0*e0z )/e0**2
                    k0xy=uxy23(i1,i2,i3,kc)
                    k0yz=uyz23(i1,i2,i3,kc)
                    k0zz=uzz23(i1,i2,i3,kc)
                    e0xy=uxy23(i1,i2,i3,ec)
                    e0xz=uxz23(i1,i2,i3,ec)
                    e0yz=uyz23(i1,i2,i3,ec)
                    e0zz=uzz23(i1,i2,i3,ec)
                    nuTxz=cMu*(2*k0z*k0x*e0**2-2*k0*k0x*e0z*e0+2*k0*
     & k0xz*e0**2-2*k0*e0x*k0z*e0+2*k0**2*e0x*e0z-k0**2*e0xz*e0)/e0**3
                    nuTyz=cMu*(2*k0z*k0y*e0**2-2*k0*k0y*e0z*e0+2*k0*
     & k0yz*e0**2-2*k0*e0y*k0z*e0+2*k0**2*e0y*e0z-k0**2*e0yz*e0)/e0**3
                    nuTzz=cMu*(2.*k0zz*e0**2-4.*k0*k0z*e0z*e0+2*k0*
     & k0zz*e0**2+2*k0**2.*e0z**2-k0**2.*e0zz*e0)/e0**3
                 f(i1,i2,i3)=(-advectionCoefficient)*(u0x**2+v0y**2+
     & w0z**2+2.*(u0y*v0x+u0z*w0x+v0z*w0y))+divDamping(i1,i2,i3)*(u0x+
     & v0y+w0z)+2.*(nuTx*u0Lap+nuTxx*u0x+nuTxy*u0y+nuTxz*u0z+nuTy*
     & v0Lap+nuTxy*v0x+nuTyy*v0y+nuTyz*v0z+nuTz*w0Lap+nuTxz*w0x+nuTyz*
     & w0y+nuTzz*w0z)


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
     & 0)*ux23(i1,i2,i3,tc)+gravity(1)*uy23(i1,i2,i3,tc)+gravity(2)*
     & uz23(i1,i2,i3,tc))
                end if
              end do
              end do
              end do
            else
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
                  f(i1,i2,i3)=f(i1,i2,i3)-thermalExpansivity*(gravity(
     & 0)*ux23(i1,i2,i3,tc)+gravity(1)*uy23(i1,i2,i3,tc)+gravity(2)*
     & uz23(i1,i2,i3,tc))
              end do
              end do
              end do
            end if
          end if
        end if
c     ***************** assign RHS for BC ********************      
c**      if( gridType.ne.rectangular )then
c**         write(*,*) 'ERROR:assignPressureRHSOpt gridType.ne.rectangular'
c**         stop 1
c**        return
c**      end if
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
          bc0=bc(side,axis)
          if( bc0.le.0 )then
            ! do nothing
          else if( bc0.eq.outflow .or. bc0.eq.convectiveOutflow .or. 
     & bc0.eq.tractionFree ) then
            a1=bcData(pc+numberOfComponents*2,side,axis) ! coeff of p.n
            ! write(*,*) 'pressureBC opt: pc,nc,side,axis,a1=',pc,numberOfComponents,side,axis,a1
            if( a1.ne.0. ) then
              ! printf("**apply mixed BC on pressure rhs...\n");
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
            else
              ! dirichlet :
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
c         ********* wall condition *********
c if( (includeADinPressure.eq.1) )then
c   if( use2ndOrderAD.eq.1 .and. use4thOrderAD.eq.0 ) then
c     applyBcByGridType(INSKE,AD2,2,3)
c   else if( use2ndOrderAD.eq.0 .and. use4thOrderAD.eq.1 ) then
c     applyBcByGridType(INSKE,AD4,2,3)
c   else if( use2ndOrderAD.eq.1 .and. use4thOrderAD.eq.1 ) then
c     applyBcByGridType(INSKE,AD24,2,3)
c   end if
c end if
c if( (includeADinPressure.eq.0) .or. (use2ndOrderAD.eq.0 .and. use4thOrderAD.eq.0 ) )then
                if( gridType.eq.rectangular )then
                   if( side.eq.0. .and. axis.eq.0 )then
                       ! Use the curl-curl form of the equations
                         do i3=n3a,n3b
                         do i2=n2a,n2b
                         do i1=n1a,n1b
                           if( mask(i1,i2,i3).ne.0 )then
                          ! Define derivative macros before calling this macro
                          ! By default there is no AD:
                            ! get nuT,nuTx,nuTy,nuTz
                                k0=u(i1,i2,i3,kc)
                                e0=u(i1,i2,i3,ec)
                                nuT = nu + cMu*k0**2/e0
                                k0x=ux23r(i1,i2,i3,kc)
                                e0x=ux23r(i1,i2,i3,ec)
                                nuTx=cMu*k0*( 2.*k0x*e0 - k0*e0x )/e0**
     & 2
                                k0y=uy23r(i1,i2,i3,kc)
                                e0y=uy23r(i1,i2,i3,ec)
                                nuTy=cMu*k0*( 2.*k0y*e0 - k0*e0y )/e0**
     & 2
                                 k0z=uz23r(i1,i2,i3,kc)
                                 e0z=uz23r(i1,i2,i3,ec)
                                 nuTz=cMu*k0*( 2.*k0z*e0 - k0*e0z )/e0*
     & *2
                                ! curl-curl form of the diffusion operator in 3D
                          ! Artificial Dissipation  **todo** implicit-line, self-adjoint versions
                          ! Here now is the statement where the ghost line value in the RHS is assigned.
                              f(i1+is1,i2,i3)=(2*side-1)*( (nuT*(-
     & uxy23r(i1,i2,i3,vc)-uxz23r(i1,i2,i3,wc)+uyy23r(i1,i2,i3,uc)+
     & uzz23r(i1,i2,i3,uc))-2.*nuTx*(uy23r(i1,i2,i3,vc)+uz23r(i1,i2,
     & i3,wc))+nuTy*(uy23r(i1,i2,i3,uc)+ux23r(i1,i2,i3,vc))+nuTz*(
     & uz23r(i1,i2,i3,uc)+ux23r(i1,i2,i3,wc)))-advectionCoefficient*(
     & uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,uc)+uu(i1,i2,i3,vc)*uy23r(i1,i2,
     & i3,uc)+uu(i1,i2,i3,wc)*uz23r(i1,i2,i3,uc))  )
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
                            f(i1+is1,i2+is2,i3)=f(i1+is1,i2+is2,i3)-(2*
     & side-1)*thermalExpansivity*u(i1,i2,i3,tc)*gravity(axis)
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
                         if( bc(sidep1,axisp)
     & .eq.inflowWithVelocityGiven .or. bc(sidep1,axisp)
     & .eq.noSlipWall )then
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
                                    k0=u(i1,i2,i3,kc)
                                    e0=u(i1,i2,i3,ec)
                                    nuT = nu + cMu*k0**2/e0
                                    k0x=ux23r(i1,i2,i3,kc)
                                    e0x=ux23r(i1,i2,i3,ec)
                                    nuTx=cMu*k0*( 2.*k0x*e0 - k0*e0x )
     & /e0**2
                                    k0y=uy23r(i1,i2,i3,kc)
                                    e0y=uy23r(i1,i2,i3,ec)
                                    nuTy=cMu*k0*( 2.*k0y*e0 - k0*e0y )
     & /e0**2
                                     k0z=uz23r(i1,i2,i3,kc)
                                     e0z=uz23r(i1,i2,i3,ec)
                                     nuTz=cMu*k0*( 2.*k0z*e0 - k0*e0z )
     & /e0**2
                                    ! normal00-form of the diffusion operator in 3D
                              ! Artificial Dissipation  **todo** implicit-line, self-adjoint versions
                              ! Here now is the statement where the ghost line value in the RHS is assigned.
                                  f(i1+is1,i2,i3)=(2*side-1)*( (nuT*(
     & ulaplacian23r(i1,i2,i3,uc))+2.*nuTx*(ux23r(i1,i2,i3,uc))+nuTy*(
     & uy23r(i1,i2,i3,uc)+ux23r(i1,i2,i3,vc))+nuTz*(uz23r(i1,i2,i3,uc)
     & +ux23r(i1,i2,i3,wc)))-advectionCoefficient*(uu(i1,i2,i3,uc)*
     & ux23r(i1,i2,i3,uc)+uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,uc)+uu(i1,i2,
     & i3,wc)*uz23r(i1,i2,i3,uc))  )
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
                                f(i1+is1,i2+is2,i3)=f(i1+is1,i2+is2,i3)
     & -(2*side-1)*thermalExpansivity*u(i1,i2,i3,tc)*gravity(axis)
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
                                k0=u(i1,i2,i3,kc)
                                e0=u(i1,i2,i3,ec)
                                nuT = nu + cMu*k0**2/e0
                                k0x=ux23r(i1,i2,i3,kc)
                                e0x=ux23r(i1,i2,i3,ec)
                                nuTx=cMu*k0*( 2.*k0x*e0 - k0*e0x )/e0**
     & 2
                                k0y=uy23r(i1,i2,i3,kc)
                                e0y=uy23r(i1,i2,i3,ec)
                                nuTy=cMu*k0*( 2.*k0y*e0 - k0*e0y )/e0**
     & 2
                                 k0z=uz23r(i1,i2,i3,kc)
                                 e0z=uz23r(i1,i2,i3,ec)
                                 nuTz=cMu*k0*( 2.*k0z*e0 - k0*e0z )/e0*
     & *2
                                ! curl-curl form of the diffusion operator in 3D
                          ! Artificial Dissipation  **todo** implicit-line, self-adjoint versions
                          ! Here now is the statement where the ghost line value in the RHS is assigned.
                              f(i1+is1,i2,i3)=(2*side-1)*( (nuT*(-
     & uxy23r(i1,i2,i3,vc)-uxz23r(i1,i2,i3,wc)+uyy23r(i1,i2,i3,uc)+
     & uzz23r(i1,i2,i3,uc))-2.*nuTx*(uy23r(i1,i2,i3,vc)+uz23r(i1,i2,
     & i3,wc))+nuTy*(uy23r(i1,i2,i3,uc)+ux23r(i1,i2,i3,vc))+nuTz*(
     & uz23r(i1,i2,i3,uc)+ux23r(i1,i2,i3,wc)))-advectionCoefficient*(
     & uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,uc)+uu(i1,i2,i3,vc)*uy23r(i1,i2,
     & i3,uc)+uu(i1,i2,i3,wc)*uz23r(i1,i2,i3,uc))  )
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
                            f(i1+is1,i2+is2,i3)=f(i1+is1,i2+is2,i3)-(2*
     & side-1)*thermalExpansivity*u(i1,i2,i3,tc)*gravity(axis)
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
                         if( bc(sidep1,axisp)
     & .eq.inflowWithVelocityGiven .or. bc(sidep1,axisp)
     & .eq.noSlipWall )then
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
                                    k0=u(i1,i2,i3,kc)
                                    e0=u(i1,i2,i3,ec)
                                    nuT = nu + cMu*k0**2/e0
                                    k0x=ux23r(i1,i2,i3,kc)
                                    e0x=ux23r(i1,i2,i3,ec)
                                    nuTx=cMu*k0*( 2.*k0x*e0 - k0*e0x )
     & /e0**2
                                    k0y=uy23r(i1,i2,i3,kc)
                                    e0y=uy23r(i1,i2,i3,ec)
                                    nuTy=cMu*k0*( 2.*k0y*e0 - k0*e0y )
     & /e0**2
                                     k0z=uz23r(i1,i2,i3,kc)
                                     e0z=uz23r(i1,i2,i3,ec)
                                     nuTz=cMu*k0*( 2.*k0z*e0 - k0*e0z )
     & /e0**2
                                    ! normal10-form of the diffusion operator in 3D
                              ! Artificial Dissipation  **todo** implicit-line, self-adjoint versions
                              ! Here now is the statement where the ghost line value in the RHS is assigned.
                                  f(i1+is1,i2,i3)=(2*side-1)*( (nuT*(
     & ulaplacian23r(i1,i2,i3,uc))+2.*nuTx*(ux23r(i1,i2,i3,uc))+nuTy*(
     & uy23r(i1,i2,i3,uc)+ux23r(i1,i2,i3,vc))+nuTz*(uz23r(i1,i2,i3,uc)
     & +ux23r(i1,i2,i3,wc)))-advectionCoefficient*(uu(i1,i2,i3,uc)*
     & ux23r(i1,i2,i3,uc)+uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,uc)+uu(i1,i2,
     & i3,wc)*uz23r(i1,i2,i3,uc))  )
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
                                f(i1+is1,i2+is2,i3)=f(i1+is1,i2+is2,i3)
     & -(2*side-1)*thermalExpansivity*u(i1,i2,i3,tc)*gravity(axis)
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
                                k0=u(i1,i2,i3,kc)
                                e0=u(i1,i2,i3,ec)
                                nuT = nu + cMu*k0**2/e0
                                k0x=ux23r(i1,i2,i3,kc)
                                e0x=ux23r(i1,i2,i3,ec)
                                nuTx=cMu*k0*( 2.*k0x*e0 - k0*e0x )/e0**
     & 2
                                k0y=uy23r(i1,i2,i3,kc)
                                e0y=uy23r(i1,i2,i3,ec)
                                nuTy=cMu*k0*( 2.*k0y*e0 - k0*e0y )/e0**
     & 2
                                 k0z=uz23r(i1,i2,i3,kc)
                                 e0z=uz23r(i1,i2,i3,ec)
                                 nuTz=cMu*k0*( 2.*k0z*e0 - k0*e0z )/e0*
     & *2
                                ! curl-curl form of the diffusion operator in 3D
                          ! Artificial Dissipation  **todo** implicit-line, self-adjoint versions
                          ! Here now is the statement where the ghost line value in the RHS is assigned.
                              f(i1,i2+is2,i3)=(2*side-1)*( (nuT*(
     & uxx23r(i1,i2,i3,vc)-uxy23r(i1,i2,i3,uc)-uyz23r(i1,i2,i3,wc)+
     & uzz23r(i1,i2,i3,vc))-2.*nuTy*(uz23r(i1,i2,i3,wc)+ux23r(i1,i2,
     & i3,uc))+nuTz*(uz23r(i1,i2,i3,vc)+uy23r(i1,i2,i3,wc))+nuTx*(
     & ux23r(i1,i2,i3,vc)+uy23r(i1,i2,i3,uc)))-advectionCoefficient*(
     & uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,vc)+uu(i1,i2,i3,vc)*uy23r(i1,i2,
     & i3,vc)+uu(i1,i2,i3,wc)*uz23r(i1,i2,i3,vc))  )
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
                            f(i1+is1,i2+is2,i3)=f(i1+is1,i2+is2,i3)-(2*
     & side-1)*thermalExpansivity*u(i1,i2,i3,tc)*gravity(axis)
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
                         if( bc(sidep1,axisp)
     & .eq.inflowWithVelocityGiven .or. bc(sidep1,axisp)
     & .eq.noSlipWall )then
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
                                    k0=u(i1,i2,i3,kc)
                                    e0=u(i1,i2,i3,ec)
                                    nuT = nu + cMu*k0**2/e0
                                    k0x=ux23r(i1,i2,i3,kc)
                                    e0x=ux23r(i1,i2,i3,ec)
                                    nuTx=cMu*k0*( 2.*k0x*e0 - k0*e0x )
     & /e0**2
                                    k0y=uy23r(i1,i2,i3,kc)
                                    e0y=uy23r(i1,i2,i3,ec)
                                    nuTy=cMu*k0*( 2.*k0y*e0 - k0*e0y )
     & /e0**2
                                     k0z=uz23r(i1,i2,i3,kc)
                                     e0z=uz23r(i1,i2,i3,ec)
                                     nuTz=cMu*k0*( 2.*k0z*e0 - k0*e0z )
     & /e0**2
                                    ! normal01-form of the diffusion operator in 3D
                              ! Artificial Dissipation  **todo** implicit-line, self-adjoint versions
                              ! Here now is the statement where the ghost line value in the RHS is assigned.
                                  f(i1,i2+is2,i3)=(2*side-1)*( (nuT*(
     & ulaplacian23r(i1,i2,i3,vc))+2.*nuTy*(uy23r(i1,i2,i3,vc))+nuTz*(
     & uz23r(i1,i2,i3,vc)+uy23r(i1,i2,i3,wc))+nuTx*(ux23r(i1,i2,i3,vc)
     & +uy23r(i1,i2,i3,uc)))-advectionCoefficient*(uu(i1,i2,i3,uc)*
     & ux23r(i1,i2,i3,vc)+uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,vc)+uu(i1,i2,
     & i3,wc)*uz23r(i1,i2,i3,vc))  )
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
                                f(i1+is1,i2+is2,i3)=f(i1+is1,i2+is2,i3)
     & -(2*side-1)*thermalExpansivity*u(i1,i2,i3,tc)*gravity(axis)
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
                                k0=u(i1,i2,i3,kc)
                                e0=u(i1,i2,i3,ec)
                                nuT = nu + cMu*k0**2/e0
                                k0x=ux23r(i1,i2,i3,kc)
                                e0x=ux23r(i1,i2,i3,ec)
                                nuTx=cMu*k0*( 2.*k0x*e0 - k0*e0x )/e0**
     & 2
                                k0y=uy23r(i1,i2,i3,kc)
                                e0y=uy23r(i1,i2,i3,ec)
                                nuTy=cMu*k0*( 2.*k0y*e0 - k0*e0y )/e0**
     & 2
                                 k0z=uz23r(i1,i2,i3,kc)
                                 e0z=uz23r(i1,i2,i3,ec)
                                 nuTz=cMu*k0*( 2.*k0z*e0 - k0*e0z )/e0*
     & *2
                                ! curl-curl form of the diffusion operator in 3D
                          ! Artificial Dissipation  **todo** implicit-line, self-adjoint versions
                          ! Here now is the statement where the ghost line value in the RHS is assigned.
                              f(i1,i2+is2,i3)=(2*side-1)*( (nuT*(
     & uxx23r(i1,i2,i3,vc)-uxy23r(i1,i2,i3,uc)-uyz23r(i1,i2,i3,wc)+
     & uzz23r(i1,i2,i3,vc))-2.*nuTy*(uz23r(i1,i2,i3,wc)+ux23r(i1,i2,
     & i3,uc))+nuTz*(uz23r(i1,i2,i3,vc)+uy23r(i1,i2,i3,wc))+nuTx*(
     & ux23r(i1,i2,i3,vc)+uy23r(i1,i2,i3,uc)))-advectionCoefficient*(
     & uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,vc)+uu(i1,i2,i3,vc)*uy23r(i1,i2,
     & i3,vc)+uu(i1,i2,i3,wc)*uz23r(i1,i2,i3,vc))  )
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
                            f(i1+is1,i2+is2,i3)=f(i1+is1,i2+is2,i3)-(2*
     & side-1)*thermalExpansivity*u(i1,i2,i3,tc)*gravity(axis)
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
                         if( bc(sidep1,axisp)
     & .eq.inflowWithVelocityGiven .or. bc(sidep1,axisp)
     & .eq.noSlipWall )then
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
                                    k0=u(i1,i2,i3,kc)
                                    e0=u(i1,i2,i3,ec)
                                    nuT = nu + cMu*k0**2/e0
                                    k0x=ux23r(i1,i2,i3,kc)
                                    e0x=ux23r(i1,i2,i3,ec)
                                    nuTx=cMu*k0*( 2.*k0x*e0 - k0*e0x )
     & /e0**2
                                    k0y=uy23r(i1,i2,i3,kc)
                                    e0y=uy23r(i1,i2,i3,ec)
                                    nuTy=cMu*k0*( 2.*k0y*e0 - k0*e0y )
     & /e0**2
                                     k0z=uz23r(i1,i2,i3,kc)
                                     e0z=uz23r(i1,i2,i3,ec)
                                     nuTz=cMu*k0*( 2.*k0z*e0 - k0*e0z )
     & /e0**2
                                    ! normal11-form of the diffusion operator in 3D
                              ! Artificial Dissipation  **todo** implicit-line, self-adjoint versions
                              ! Here now is the statement where the ghost line value in the RHS is assigned.
                                  f(i1,i2+is2,i3)=(2*side-1)*( (nuT*(
     & ulaplacian23r(i1,i2,i3,vc))+2.*nuTy*(uy23r(i1,i2,i3,vc))+nuTz*(
     & uz23r(i1,i2,i3,vc)+uy23r(i1,i2,i3,wc))+nuTx*(ux23r(i1,i2,i3,vc)
     & +uy23r(i1,i2,i3,uc)))-advectionCoefficient*(uu(i1,i2,i3,uc)*
     & ux23r(i1,i2,i3,vc)+uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,vc)+uu(i1,i2,
     & i3,wc)*uz23r(i1,i2,i3,vc))  )
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
                                f(i1+is1,i2+is2,i3)=f(i1+is1,i2+is2,i3)
     & -(2*side-1)*thermalExpansivity*u(i1,i2,i3,tc)*gravity(axis)
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
                   else if( side.eq.0 .and. axis.eq.2 )then
                       ! Use the curl-curl form of the equations
                         do i3=n3a,n3b
                         do i2=n2a,n2b
                         do i1=n1a,n1b
                           if( mask(i1,i2,i3).ne.0 )then
                          ! Define derivative macros before calling this macro
                          ! By default there is no AD:
                            ! get nuT,nuTx,nuTy,nuTz
                                k0=u(i1,i2,i3,kc)
                                e0=u(i1,i2,i3,ec)
                                nuT = nu + cMu*k0**2/e0
                                k0x=ux23r(i1,i2,i3,kc)
                                e0x=ux23r(i1,i2,i3,ec)
                                nuTx=cMu*k0*( 2.*k0x*e0 - k0*e0x )/e0**
     & 2
                                k0y=uy23r(i1,i2,i3,kc)
                                e0y=uy23r(i1,i2,i3,ec)
                                nuTy=cMu*k0*( 2.*k0y*e0 - k0*e0y )/e0**
     & 2
                                 k0z=uz23r(i1,i2,i3,kc)
                                 e0z=uz23r(i1,i2,i3,ec)
                                 nuTz=cMu*k0*( 2.*k0z*e0 - k0*e0z )/e0*
     & *2
                                ! curl-curl form of the diffusion operator in 3D
                          ! Artificial Dissipation  **todo** implicit-line, self-adjoint versions
                          ! Here now is the statement where the ghost line value in the RHS is assigned.
                              f(i1,i2,i3+is3)=(2*side-1)*( (nuT*(
     & uxx23r(i1,i2,i3,wc)+uyy23r(i1,i2,i3,wc)-uxz23r(i1,i2,i3,uc)-
     & uyz23r(i1,i2,i3,vc))-2.*nuTz*(ux23r(i1,i2,i3,uc)+uy23r(i1,i2,
     & i3,vc))+nuTx*(ux23r(i1,i2,i3,wc)+uz23r(i1,i2,i3,uc))+nuTy*(
     & uy23r(i1,i2,i3,wc)+uz23r(i1,i2,i3,vc)))-advectionCoefficient*(
     & uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,wc)+uu(i1,i2,i3,vc)*uy23r(i1,i2,
     & i3,wc)+uu(i1,i2,i3,wc)*uz23r(i1,i2,i3,wc))  )
                           end if
                         end do
                         end do
                         end do
                       ! For the Boussinesq approximation:  -alpha*T* normal02.gravity
                       if( pdeModel.eq.BoussinesqModel .or. 
     & pdeModel.eq.viscoPlasticModel )then
                           do i3=n3a,n3b
                           do i2=n2a,n2b
                           do i1=n1a,n1b
                             if( mask(i1,i2,i3).ne.0 )then
                            f(i1+is1,i2+is2,i3)=f(i1+is1,i2+is2,i3)-(2*
     & side-1)*thermalExpansivity*u(i1,i2,i3,tc)*gravity(axis)
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
                         if( bc(sidep1,axisp)
     & .eq.inflowWithVelocityGiven .or. bc(sidep1,axisp)
     & .eq.noSlipWall )then
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
                                    k0=u(i1,i2,i3,kc)
                                    e0=u(i1,i2,i3,ec)
                                    nuT = nu + cMu*k0**2/e0
                                    k0x=ux23r(i1,i2,i3,kc)
                                    e0x=ux23r(i1,i2,i3,ec)
                                    nuTx=cMu*k0*( 2.*k0x*e0 - k0*e0x )
     & /e0**2
                                    k0y=uy23r(i1,i2,i3,kc)
                                    e0y=uy23r(i1,i2,i3,ec)
                                    nuTy=cMu*k0*( 2.*k0y*e0 - k0*e0y )
     & /e0**2
                                     k0z=uz23r(i1,i2,i3,kc)
                                     e0z=uz23r(i1,i2,i3,ec)
                                     nuTz=cMu*k0*( 2.*k0z*e0 - k0*e0z )
     & /e0**2
                                    ! normal02-form of the diffusion operator in 3D
                              ! Artificial Dissipation  **todo** implicit-line, self-adjoint versions
                              ! Here now is the statement where the ghost line value in the RHS is assigned.
                                  f(i1,i2,i3+is3)=(2*side-1)*( (nuT*(
     & ulaplacian23r(i1,i2,i3,wc))+2.*nuTz*(uz23r(i1,i2,i3,wc))+nuTx*(
     & ux23r(i1,i2,i3,wc)+uz23r(i1,i2,i3,uc))+nuTy*(uy23r(i1,i2,i3,wc)
     & +uz23r(i1,i2,i3,vc)))-advectionCoefficient*(uu(i1,i2,i3,uc)*
     & ux23r(i1,i2,i3,wc)+uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,wc)+uu(i1,i2,
     & i3,wc)*uz23r(i1,i2,i3,wc))  )
                               end if
                             end do
                             end do
                             end do
                           ! For the Boussinesq approximation:  -alpha*T* normal02.gravity
                           if( pdeModel.eq.BoussinesqModel .or. 
     & pdeModel.eq.viscoPlasticModel )then
                               do i3=n3a,n3b
                               do i2=n2a,n2b
                               do i1=n1a,n1b
                                 if( mask(i1,i2,i3).ne.0 )then
                                f(i1+is1,i2+is2,i3)=f(i1+is1,i2+is2,i3)
     & -(2*side-1)*thermalExpansivity*u(i1,i2,i3,tc)*gravity(axis)
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
                   else if( side.eq.1 .and. axis.eq.2 )then
                       ! Use the curl-curl form of the equations
                         do i3=n3a,n3b
                         do i2=n2a,n2b
                         do i1=n1a,n1b
                           if( mask(i1,i2,i3).ne.0 )then
                          ! Define derivative macros before calling this macro
                          ! By default there is no AD:
                            ! get nuT,nuTx,nuTy,nuTz
                                k0=u(i1,i2,i3,kc)
                                e0=u(i1,i2,i3,ec)
                                nuT = nu + cMu*k0**2/e0
                                k0x=ux23r(i1,i2,i3,kc)
                                e0x=ux23r(i1,i2,i3,ec)
                                nuTx=cMu*k0*( 2.*k0x*e0 - k0*e0x )/e0**
     & 2
                                k0y=uy23r(i1,i2,i3,kc)
                                e0y=uy23r(i1,i2,i3,ec)
                                nuTy=cMu*k0*( 2.*k0y*e0 - k0*e0y )/e0**
     & 2
                                 k0z=uz23r(i1,i2,i3,kc)
                                 e0z=uz23r(i1,i2,i3,ec)
                                 nuTz=cMu*k0*( 2.*k0z*e0 - k0*e0z )/e0*
     & *2
                                ! curl-curl form of the diffusion operator in 3D
                          ! Artificial Dissipation  **todo** implicit-line, self-adjoint versions
                          ! Here now is the statement where the ghost line value in the RHS is assigned.
                              f(i1,i2,i3+is3)=(2*side-1)*( (nuT*(
     & uxx23r(i1,i2,i3,wc)+uyy23r(i1,i2,i3,wc)-uxz23r(i1,i2,i3,uc)-
     & uyz23r(i1,i2,i3,vc))-2.*nuTz*(ux23r(i1,i2,i3,uc)+uy23r(i1,i2,
     & i3,vc))+nuTx*(ux23r(i1,i2,i3,wc)+uz23r(i1,i2,i3,uc))+nuTy*(
     & uy23r(i1,i2,i3,wc)+uz23r(i1,i2,i3,vc)))-advectionCoefficient*(
     & uu(i1,i2,i3,uc)*ux23r(i1,i2,i3,wc)+uu(i1,i2,i3,vc)*uy23r(i1,i2,
     & i3,wc)+uu(i1,i2,i3,wc)*uz23r(i1,i2,i3,wc))  )
                           end if
                         end do
                         end do
                         end do
                       ! For the Boussinesq approximation:  -alpha*T* normal12.gravity
                       if( pdeModel.eq.BoussinesqModel .or. 
     & pdeModel.eq.viscoPlasticModel )then
                           do i3=n3a,n3b
                           do i2=n2a,n2b
                           do i1=n1a,n1b
                             if( mask(i1,i2,i3).ne.0 )then
                            f(i1+is1,i2+is2,i3)=f(i1+is1,i2+is2,i3)-(2*
     & side-1)*thermalExpansivity*u(i1,i2,i3,tc)*gravity(axis)
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
                         if( bc(sidep1,axisp)
     & .eq.inflowWithVelocityGiven .or. bc(sidep1,axisp)
     & .eq.noSlipWall )then
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
                                    k0=u(i1,i2,i3,kc)
                                    e0=u(i1,i2,i3,ec)
                                    nuT = nu + cMu*k0**2/e0
                                    k0x=ux23r(i1,i2,i3,kc)
                                    e0x=ux23r(i1,i2,i3,ec)
                                    nuTx=cMu*k0*( 2.*k0x*e0 - k0*e0x )
     & /e0**2
                                    k0y=uy23r(i1,i2,i3,kc)
                                    e0y=uy23r(i1,i2,i3,ec)
                                    nuTy=cMu*k0*( 2.*k0y*e0 - k0*e0y )
     & /e0**2
                                     k0z=uz23r(i1,i2,i3,kc)
                                     e0z=uz23r(i1,i2,i3,ec)
                                     nuTz=cMu*k0*( 2.*k0z*e0 - k0*e0z )
     & /e0**2
                                    ! normal12-form of the diffusion operator in 3D
                              ! Artificial Dissipation  **todo** implicit-line, self-adjoint versions
                              ! Here now is the statement where the ghost line value in the RHS is assigned.
                                  f(i1,i2,i3+is3)=(2*side-1)*( (nuT*(
     & ulaplacian23r(i1,i2,i3,wc))+2.*nuTz*(uz23r(i1,i2,i3,wc))+nuTx*(
     & ux23r(i1,i2,i3,wc)+uz23r(i1,i2,i3,uc))+nuTy*(uy23r(i1,i2,i3,wc)
     & +uz23r(i1,i2,i3,vc)))-advectionCoefficient*(uu(i1,i2,i3,uc)*
     & ux23r(i1,i2,i3,wc)+uu(i1,i2,i3,vc)*uy23r(i1,i2,i3,wc)+uu(i1,i2,
     & i3,wc)*uz23r(i1,i2,i3,wc))  )
                               end if
                             end do
                             end do
                             end do
                           ! For the Boussinesq approximation:  -alpha*T* normal12.gravity
                           if( pdeModel.eq.BoussinesqModel .or. 
     & pdeModel.eq.viscoPlasticModel )then
                               do i3=n3a,n3b
                               do i2=n2a,n2b
                               do i1=n1a,n1b
                                 if( mask(i1,i2,i3).ne.0 )then
                                f(i1+is1,i2+is2,i3)=f(i1+is1,i2+is2,i3)
     & -(2*side-1)*thermalExpansivity*u(i1,i2,i3,tc)*gravity(axis)
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
                   if( side.eq.0. .and. axis.eq.0 )then
                       ! Use the curl-curl form of the equations
                         do i3=n3a,n3b
                         do i2=n2a,n2b
                         do i1=n1a,n1b
                           if( mask(i1,i2,i3).ne.0 )then
                          ! Define derivative macros before calling this macro
                          ! By default there is no AD:
                            ! get nuT,nuTx,nuTy,nuTz
                                k0=u(i1,i2,i3,kc)
                                e0=u(i1,i2,i3,ec)
                                nuT = nu + cMu*k0**2/e0
                                k0x=ux23(i1,i2,i3,kc)
                                e0x=ux23(i1,i2,i3,ec)
                                nuTx=cMu*k0*( 2.*k0x*e0 - k0*e0x )/e0**
     & 2
                                k0y=uy23(i1,i2,i3,kc)
                                e0y=uy23(i1,i2,i3,ec)
                                nuTy=cMu*k0*( 2.*k0y*e0 - k0*e0y )/e0**
     & 2
                                 k0z=uz23(i1,i2,i3,kc)
                                 e0z=uz23(i1,i2,i3,ec)
                                 nuTz=cMu*k0*( 2.*k0z*e0 - k0*e0z )/e0*
     & *2
                                ! curl-curl form of the diffusion operator in 3D
                          ! Artificial Dissipation  **todo** implicit-line, self-adjoint versions
                          ! Here now is the statement where the ghost line value in the RHS is assigned.
                              f(i1+is1,i2+is2,i3+is3)=normal00(i1,i2,
     & i3,0)*( (nuT*(-uxy23(i1,i2,i3,vc)-uxz23(i1,i2,i3,wc)+uyy23(i1,
     & i2,i3,uc)+uzz23(i1,i2,i3,uc))-2.*nuTx*(uy23(i1,i2,i3,vc)+uz23(
     & i1,i2,i3,wc))+nuTy*(uy23(i1,i2,i3,uc)+ux23(i1,i2,i3,vc))+nuTz*(
     & uz23(i1,i2,i3,uc)+ux23(i1,i2,i3,wc)))-advectionCoefficient*(uu(
     & i1,i2,i3,uc)*ux23(i1,i2,i3,uc)+uu(i1,i2,i3,vc)*uy23(i1,i2,i3,
     & uc)+uu(i1,i2,i3,wc)*uz23(i1,i2,i3,uc))  )+normal00(i1,i2,i3,1)*
     & ( (nuT*(uxx23(i1,i2,i3,vc)-uxy23(i1,i2,i3,uc)-uyz23(i1,i2,i3,
     & wc)+uzz23(i1,i2,i3,vc))-2.*nuTy*(uz23(i1,i2,i3,wc)+ux23(i1,i2,
     & i3,uc))+nuTz*(uz23(i1,i2,i3,vc)+uy23(i1,i2,i3,wc))+nuTx*(ux23(
     & i1,i2,i3,vc)+uy23(i1,i2,i3,uc)))-advectionCoefficient*(uu(i1,
     & i2,i3,uc)*ux23(i1,i2,i3,vc)+uu(i1,i2,i3,vc)*uy23(i1,i2,i3,vc)+
     & uu(i1,i2,i3,wc)*uz23(i1,i2,i3,vc))  )+normal00(i1,i2,i3,2)*( (
     & nuT*(uxx23(i1,i2,i3,wc)+uyy23(i1,i2,i3,wc)-uxz23(i1,i2,i3,uc)-
     & uyz23(i1,i2,i3,vc))-2.*nuTz*(ux23(i1,i2,i3,uc)+uy23(i1,i2,i3,
     & vc))+nuTx*(ux23(i1,i2,i3,wc)+uz23(i1,i2,i3,uc))+nuTy*(uy23(i1,
     & i2,i3,wc)+uz23(i1,i2,i3,vc)))-advectionCoefficient*(uu(i1,i2,
     & i3,uc)*ux23(i1,i2,i3,wc)+uu(i1,i2,i3,vc)*uy23(i1,i2,i3,wc)+uu(
     & i1,i2,i3,wc)*uz23(i1,i2,i3,wc))  )
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
                             f(i1+is1,i2+is2,i3+is3)=f(i1+is1,i2+is2,
     & i3+is3)-thermalExpansivity*u(i1,i2,i3,tc)*(gravity(0)*normal00(
     & i1,i2,i3,0)+gravity(1)*normal00(i1,i2,i3,1)+gravity(2)*
     & normal00(i1,i2,i3,2))
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
                         if( bc(sidep1,axisp)
     & .eq.inflowWithVelocityGiven .or. bc(sidep1,axisp)
     & .eq.noSlipWall )then
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
                                    k0=u(i1,i2,i3,kc)
                                    e0=u(i1,i2,i3,ec)
                                    nuT = nu + cMu*k0**2/e0
                                    k0x=ux23(i1,i2,i3,kc)
                                    e0x=ux23(i1,i2,i3,ec)
                                    nuTx=cMu*k0*( 2.*k0x*e0 - k0*e0x )
     & /e0**2
                                    k0y=uy23(i1,i2,i3,kc)
                                    e0y=uy23(i1,i2,i3,ec)
                                    nuTy=cMu*k0*( 2.*k0y*e0 - k0*e0y )
     & /e0**2
                                     k0z=uz23(i1,i2,i3,kc)
                                     e0z=uz23(i1,i2,i3,ec)
                                     nuTz=cMu*k0*( 2.*k0z*e0 - k0*e0z )
     & /e0**2
                                    ! normal00-form of the diffusion operator in 3D
                              ! Artificial Dissipation  **todo** implicit-line, self-adjoint versions
                              ! Here now is the statement where the ghost line value in the RHS is assigned.
                                  f(i1+is1,i2+is2,i3+is3)=normal00(i1,
     & i2,i3,0)*( (nuT*(ulaplacian23(i1,i2,i3,uc))+2.*nuTx*(ux23(i1,
     & i2,i3,uc))+nuTy*(uy23(i1,i2,i3,uc)+ux23(i1,i2,i3,vc))+nuTz*(
     & uz23(i1,i2,i3,uc)+ux23(i1,i2,i3,wc)))-advectionCoefficient*(uu(
     & i1,i2,i3,uc)*ux23(i1,i2,i3,uc)+uu(i1,i2,i3,vc)*uy23(i1,i2,i3,
     & uc)+uu(i1,i2,i3,wc)*uz23(i1,i2,i3,uc))  )+normal00(i1,i2,i3,1)*
     & ( (nuT*(ulaplacian23(i1,i2,i3,vc))+2.*nuTy*(uy23(i1,i2,i3,vc))+
     & nuTz*(uz23(i1,i2,i3,vc)+uy23(i1,i2,i3,wc))+nuTx*(ux23(i1,i2,i3,
     & vc)+uy23(i1,i2,i3,uc)))-advectionCoefficient*(uu(i1,i2,i3,uc)*
     & ux23(i1,i2,i3,vc)+uu(i1,i2,i3,vc)*uy23(i1,i2,i3,vc)+uu(i1,i2,
     & i3,wc)*uz23(i1,i2,i3,vc))  )+normal00(i1,i2,i3,2)*( (nuT*(
     & ulaplacian23(i1,i2,i3,wc))+2.*nuTz*(uz23(i1,i2,i3,wc))+nuTx*(
     & ux23(i1,i2,i3,wc)+uz23(i1,i2,i3,uc))+nuTy*(uy23(i1,i2,i3,wc)+
     & uz23(i1,i2,i3,vc)))-advectionCoefficient*(uu(i1,i2,i3,uc)*ux23(
     & i1,i2,i3,wc)+uu(i1,i2,i3,vc)*uy23(i1,i2,i3,wc)+uu(i1,i2,i3,wc)*
     & uz23(i1,i2,i3,wc))  )
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
     & is2,i3+is3)-thermalExpansivity*u(i1,i2,i3,tc)*(gravity(0)*
     & normal00(i1,i2,i3,0)+gravity(1)*normal00(i1,i2,i3,1)+gravity(2)
     & *normal00(i1,i2,i3,2))
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
                                k0=u(i1,i2,i3,kc)
                                e0=u(i1,i2,i3,ec)
                                nuT = nu + cMu*k0**2/e0
                                k0x=ux23(i1,i2,i3,kc)
                                e0x=ux23(i1,i2,i3,ec)
                                nuTx=cMu*k0*( 2.*k0x*e0 - k0*e0x )/e0**
     & 2
                                k0y=uy23(i1,i2,i3,kc)
                                e0y=uy23(i1,i2,i3,ec)
                                nuTy=cMu*k0*( 2.*k0y*e0 - k0*e0y )/e0**
     & 2
                                 k0z=uz23(i1,i2,i3,kc)
                                 e0z=uz23(i1,i2,i3,ec)
                                 nuTz=cMu*k0*( 2.*k0z*e0 - k0*e0z )/e0*
     & *2
                                ! curl-curl form of the diffusion operator in 3D
                          ! Artificial Dissipation  **todo** implicit-line, self-adjoint versions
                          ! Here now is the statement where the ghost line value in the RHS is assigned.
                              f(i1+is1,i2+is2,i3+is3)=normal10(i1,i2,
     & i3,0)*( (nuT*(-uxy23(i1,i2,i3,vc)-uxz23(i1,i2,i3,wc)+uyy23(i1,
     & i2,i3,uc)+uzz23(i1,i2,i3,uc))-2.*nuTx*(uy23(i1,i2,i3,vc)+uz23(
     & i1,i2,i3,wc))+nuTy*(uy23(i1,i2,i3,uc)+ux23(i1,i2,i3,vc))+nuTz*(
     & uz23(i1,i2,i3,uc)+ux23(i1,i2,i3,wc)))-advectionCoefficient*(uu(
     & i1,i2,i3,uc)*ux23(i1,i2,i3,uc)+uu(i1,i2,i3,vc)*uy23(i1,i2,i3,
     & uc)+uu(i1,i2,i3,wc)*uz23(i1,i2,i3,uc))  )+normal10(i1,i2,i3,1)*
     & ( (nuT*(uxx23(i1,i2,i3,vc)-uxy23(i1,i2,i3,uc)-uyz23(i1,i2,i3,
     & wc)+uzz23(i1,i2,i3,vc))-2.*nuTy*(uz23(i1,i2,i3,wc)+ux23(i1,i2,
     & i3,uc))+nuTz*(uz23(i1,i2,i3,vc)+uy23(i1,i2,i3,wc))+nuTx*(ux23(
     & i1,i2,i3,vc)+uy23(i1,i2,i3,uc)))-advectionCoefficient*(uu(i1,
     & i2,i3,uc)*ux23(i1,i2,i3,vc)+uu(i1,i2,i3,vc)*uy23(i1,i2,i3,vc)+
     & uu(i1,i2,i3,wc)*uz23(i1,i2,i3,vc))  )+normal10(i1,i2,i3,2)*( (
     & nuT*(uxx23(i1,i2,i3,wc)+uyy23(i1,i2,i3,wc)-uxz23(i1,i2,i3,uc)-
     & uyz23(i1,i2,i3,vc))-2.*nuTz*(ux23(i1,i2,i3,uc)+uy23(i1,i2,i3,
     & vc))+nuTx*(ux23(i1,i2,i3,wc)+uz23(i1,i2,i3,uc))+nuTy*(uy23(i1,
     & i2,i3,wc)+uz23(i1,i2,i3,vc)))-advectionCoefficient*(uu(i1,i2,
     & i3,uc)*ux23(i1,i2,i3,wc)+uu(i1,i2,i3,vc)*uy23(i1,i2,i3,wc)+uu(
     & i1,i2,i3,wc)*uz23(i1,i2,i3,wc))  )
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
                             f(i1+is1,i2+is2,i3+is3)=f(i1+is1,i2+is2,
     & i3+is3)-thermalExpansivity*u(i1,i2,i3,tc)*(gravity(0)*normal10(
     & i1,i2,i3,0)+gravity(1)*normal10(i1,i2,i3,1)+gravity(2)*
     & normal10(i1,i2,i3,2))
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
                         if( bc(sidep1,axisp)
     & .eq.inflowWithVelocityGiven .or. bc(sidep1,axisp)
     & .eq.noSlipWall )then
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
                                    k0=u(i1,i2,i3,kc)
                                    e0=u(i1,i2,i3,ec)
                                    nuT = nu + cMu*k0**2/e0
                                    k0x=ux23(i1,i2,i3,kc)
                                    e0x=ux23(i1,i2,i3,ec)
                                    nuTx=cMu*k0*( 2.*k0x*e0 - k0*e0x )
     & /e0**2
                                    k0y=uy23(i1,i2,i3,kc)
                                    e0y=uy23(i1,i2,i3,ec)
                                    nuTy=cMu*k0*( 2.*k0y*e0 - k0*e0y )
     & /e0**2
                                     k0z=uz23(i1,i2,i3,kc)
                                     e0z=uz23(i1,i2,i3,ec)
                                     nuTz=cMu*k0*( 2.*k0z*e0 - k0*e0z )
     & /e0**2
                                    ! normal10-form of the diffusion operator in 3D
                              ! Artificial Dissipation  **todo** implicit-line, self-adjoint versions
                              ! Here now is the statement where the ghost line value in the RHS is assigned.
                                  f(i1+is1,i2+is2,i3+is3)=normal10(i1,
     & i2,i3,0)*( (nuT*(ulaplacian23(i1,i2,i3,uc))+2.*nuTx*(ux23(i1,
     & i2,i3,uc))+nuTy*(uy23(i1,i2,i3,uc)+ux23(i1,i2,i3,vc))+nuTz*(
     & uz23(i1,i2,i3,uc)+ux23(i1,i2,i3,wc)))-advectionCoefficient*(uu(
     & i1,i2,i3,uc)*ux23(i1,i2,i3,uc)+uu(i1,i2,i3,vc)*uy23(i1,i2,i3,
     & uc)+uu(i1,i2,i3,wc)*uz23(i1,i2,i3,uc))  )+normal10(i1,i2,i3,1)*
     & ( (nuT*(ulaplacian23(i1,i2,i3,vc))+2.*nuTy*(uy23(i1,i2,i3,vc))+
     & nuTz*(uz23(i1,i2,i3,vc)+uy23(i1,i2,i3,wc))+nuTx*(ux23(i1,i2,i3,
     & vc)+uy23(i1,i2,i3,uc)))-advectionCoefficient*(uu(i1,i2,i3,uc)*
     & ux23(i1,i2,i3,vc)+uu(i1,i2,i3,vc)*uy23(i1,i2,i3,vc)+uu(i1,i2,
     & i3,wc)*uz23(i1,i2,i3,vc))  )+normal10(i1,i2,i3,2)*( (nuT*(
     & ulaplacian23(i1,i2,i3,wc))+2.*nuTz*(uz23(i1,i2,i3,wc))+nuTx*(
     & ux23(i1,i2,i3,wc)+uz23(i1,i2,i3,uc))+nuTy*(uy23(i1,i2,i3,wc)+
     & uz23(i1,i2,i3,vc)))-advectionCoefficient*(uu(i1,i2,i3,uc)*ux23(
     & i1,i2,i3,wc)+uu(i1,i2,i3,vc)*uy23(i1,i2,i3,wc)+uu(i1,i2,i3,wc)*
     & uz23(i1,i2,i3,wc))  )
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
     & is2,i3+is3)-thermalExpansivity*u(i1,i2,i3,tc)*(gravity(0)*
     & normal10(i1,i2,i3,0)+gravity(1)*normal10(i1,i2,i3,1)+gravity(2)
     & *normal10(i1,i2,i3,2))
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
                                k0=u(i1,i2,i3,kc)
                                e0=u(i1,i2,i3,ec)
                                nuT = nu + cMu*k0**2/e0
                                k0x=ux23(i1,i2,i3,kc)
                                e0x=ux23(i1,i2,i3,ec)
                                nuTx=cMu*k0*( 2.*k0x*e0 - k0*e0x )/e0**
     & 2
                                k0y=uy23(i1,i2,i3,kc)
                                e0y=uy23(i1,i2,i3,ec)
                                nuTy=cMu*k0*( 2.*k0y*e0 - k0*e0y )/e0**
     & 2
                                 k0z=uz23(i1,i2,i3,kc)
                                 e0z=uz23(i1,i2,i3,ec)
                                 nuTz=cMu*k0*( 2.*k0z*e0 - k0*e0z )/e0*
     & *2
                                ! curl-curl form of the diffusion operator in 3D
                          ! Artificial Dissipation  **todo** implicit-line, self-adjoint versions
                          ! Here now is the statement where the ghost line value in the RHS is assigned.
                              f(i1+is1,i2+is2,i3+is3)=normal01(i1,i2,
     & i3,0)*( (nuT*(-uxy23(i1,i2,i3,vc)-uxz23(i1,i2,i3,wc)+uyy23(i1,
     & i2,i3,uc)+uzz23(i1,i2,i3,uc))-2.*nuTx*(uy23(i1,i2,i3,vc)+uz23(
     & i1,i2,i3,wc))+nuTy*(uy23(i1,i2,i3,uc)+ux23(i1,i2,i3,vc))+nuTz*(
     & uz23(i1,i2,i3,uc)+ux23(i1,i2,i3,wc)))-advectionCoefficient*(uu(
     & i1,i2,i3,uc)*ux23(i1,i2,i3,uc)+uu(i1,i2,i3,vc)*uy23(i1,i2,i3,
     & uc)+uu(i1,i2,i3,wc)*uz23(i1,i2,i3,uc))  )+normal01(i1,i2,i3,1)*
     & ( (nuT*(uxx23(i1,i2,i3,vc)-uxy23(i1,i2,i3,uc)-uyz23(i1,i2,i3,
     & wc)+uzz23(i1,i2,i3,vc))-2.*nuTy*(uz23(i1,i2,i3,wc)+ux23(i1,i2,
     & i3,uc))+nuTz*(uz23(i1,i2,i3,vc)+uy23(i1,i2,i3,wc))+nuTx*(ux23(
     & i1,i2,i3,vc)+uy23(i1,i2,i3,uc)))-advectionCoefficient*(uu(i1,
     & i2,i3,uc)*ux23(i1,i2,i3,vc)+uu(i1,i2,i3,vc)*uy23(i1,i2,i3,vc)+
     & uu(i1,i2,i3,wc)*uz23(i1,i2,i3,vc))  )+normal01(i1,i2,i3,2)*( (
     & nuT*(uxx23(i1,i2,i3,wc)+uyy23(i1,i2,i3,wc)-uxz23(i1,i2,i3,uc)-
     & uyz23(i1,i2,i3,vc))-2.*nuTz*(ux23(i1,i2,i3,uc)+uy23(i1,i2,i3,
     & vc))+nuTx*(ux23(i1,i2,i3,wc)+uz23(i1,i2,i3,uc))+nuTy*(uy23(i1,
     & i2,i3,wc)+uz23(i1,i2,i3,vc)))-advectionCoefficient*(uu(i1,i2,
     & i3,uc)*ux23(i1,i2,i3,wc)+uu(i1,i2,i3,vc)*uy23(i1,i2,i3,wc)+uu(
     & i1,i2,i3,wc)*uz23(i1,i2,i3,wc))  )
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
                             f(i1+is1,i2+is2,i3+is3)=f(i1+is1,i2+is2,
     & i3+is3)-thermalExpansivity*u(i1,i2,i3,tc)*(gravity(0)*normal01(
     & i1,i2,i3,0)+gravity(1)*normal01(i1,i2,i3,1)+gravity(2)*
     & normal01(i1,i2,i3,2))
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
                         if( bc(sidep1,axisp)
     & .eq.inflowWithVelocityGiven .or. bc(sidep1,axisp)
     & .eq.noSlipWall )then
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
                                    k0=u(i1,i2,i3,kc)
                                    e0=u(i1,i2,i3,ec)
                                    nuT = nu + cMu*k0**2/e0
                                    k0x=ux23(i1,i2,i3,kc)
                                    e0x=ux23(i1,i2,i3,ec)
                                    nuTx=cMu*k0*( 2.*k0x*e0 - k0*e0x )
     & /e0**2
                                    k0y=uy23(i1,i2,i3,kc)
                                    e0y=uy23(i1,i2,i3,ec)
                                    nuTy=cMu*k0*( 2.*k0y*e0 - k0*e0y )
     & /e0**2
                                     k0z=uz23(i1,i2,i3,kc)
                                     e0z=uz23(i1,i2,i3,ec)
                                     nuTz=cMu*k0*( 2.*k0z*e0 - k0*e0z )
     & /e0**2
                                    ! normal01-form of the diffusion operator in 3D
                              ! Artificial Dissipation  **todo** implicit-line, self-adjoint versions
                              ! Here now is the statement where the ghost line value in the RHS is assigned.
                                  f(i1+is1,i2+is2,i3+is3)=normal01(i1,
     & i2,i3,0)*( (nuT*(ulaplacian23(i1,i2,i3,uc))+2.*nuTx*(ux23(i1,
     & i2,i3,uc))+nuTy*(uy23(i1,i2,i3,uc)+ux23(i1,i2,i3,vc))+nuTz*(
     & uz23(i1,i2,i3,uc)+ux23(i1,i2,i3,wc)))-advectionCoefficient*(uu(
     & i1,i2,i3,uc)*ux23(i1,i2,i3,uc)+uu(i1,i2,i3,vc)*uy23(i1,i2,i3,
     & uc)+uu(i1,i2,i3,wc)*uz23(i1,i2,i3,uc))  )+normal01(i1,i2,i3,1)*
     & ( (nuT*(ulaplacian23(i1,i2,i3,vc))+2.*nuTy*(uy23(i1,i2,i3,vc))+
     & nuTz*(uz23(i1,i2,i3,vc)+uy23(i1,i2,i3,wc))+nuTx*(ux23(i1,i2,i3,
     & vc)+uy23(i1,i2,i3,uc)))-advectionCoefficient*(uu(i1,i2,i3,uc)*
     & ux23(i1,i2,i3,vc)+uu(i1,i2,i3,vc)*uy23(i1,i2,i3,vc)+uu(i1,i2,
     & i3,wc)*uz23(i1,i2,i3,vc))  )+normal01(i1,i2,i3,2)*( (nuT*(
     & ulaplacian23(i1,i2,i3,wc))+2.*nuTz*(uz23(i1,i2,i3,wc))+nuTx*(
     & ux23(i1,i2,i3,wc)+uz23(i1,i2,i3,uc))+nuTy*(uy23(i1,i2,i3,wc)+
     & uz23(i1,i2,i3,vc)))-advectionCoefficient*(uu(i1,i2,i3,uc)*ux23(
     & i1,i2,i3,wc)+uu(i1,i2,i3,vc)*uy23(i1,i2,i3,wc)+uu(i1,i2,i3,wc)*
     & uz23(i1,i2,i3,wc))  )
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
     & is2,i3+is3)-thermalExpansivity*u(i1,i2,i3,tc)*(gravity(0)*
     & normal01(i1,i2,i3,0)+gravity(1)*normal01(i1,i2,i3,1)+gravity(2)
     & *normal01(i1,i2,i3,2))
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
                                k0=u(i1,i2,i3,kc)
                                e0=u(i1,i2,i3,ec)
                                nuT = nu + cMu*k0**2/e0
                                k0x=ux23(i1,i2,i3,kc)
                                e0x=ux23(i1,i2,i3,ec)
                                nuTx=cMu*k0*( 2.*k0x*e0 - k0*e0x )/e0**
     & 2
                                k0y=uy23(i1,i2,i3,kc)
                                e0y=uy23(i1,i2,i3,ec)
                                nuTy=cMu*k0*( 2.*k0y*e0 - k0*e0y )/e0**
     & 2
                                 k0z=uz23(i1,i2,i3,kc)
                                 e0z=uz23(i1,i2,i3,ec)
                                 nuTz=cMu*k0*( 2.*k0z*e0 - k0*e0z )/e0*
     & *2
                                ! curl-curl form of the diffusion operator in 3D
                          ! Artificial Dissipation  **todo** implicit-line, self-adjoint versions
                          ! Here now is the statement where the ghost line value in the RHS is assigned.
                              f(i1+is1,i2+is2,i3+is3)=normal11(i1,i2,
     & i3,0)*( (nuT*(-uxy23(i1,i2,i3,vc)-uxz23(i1,i2,i3,wc)+uyy23(i1,
     & i2,i3,uc)+uzz23(i1,i2,i3,uc))-2.*nuTx*(uy23(i1,i2,i3,vc)+uz23(
     & i1,i2,i3,wc))+nuTy*(uy23(i1,i2,i3,uc)+ux23(i1,i2,i3,vc))+nuTz*(
     & uz23(i1,i2,i3,uc)+ux23(i1,i2,i3,wc)))-advectionCoefficient*(uu(
     & i1,i2,i3,uc)*ux23(i1,i2,i3,uc)+uu(i1,i2,i3,vc)*uy23(i1,i2,i3,
     & uc)+uu(i1,i2,i3,wc)*uz23(i1,i2,i3,uc))  )+normal11(i1,i2,i3,1)*
     & ( (nuT*(uxx23(i1,i2,i3,vc)-uxy23(i1,i2,i3,uc)-uyz23(i1,i2,i3,
     & wc)+uzz23(i1,i2,i3,vc))-2.*nuTy*(uz23(i1,i2,i3,wc)+ux23(i1,i2,
     & i3,uc))+nuTz*(uz23(i1,i2,i3,vc)+uy23(i1,i2,i3,wc))+nuTx*(ux23(
     & i1,i2,i3,vc)+uy23(i1,i2,i3,uc)))-advectionCoefficient*(uu(i1,
     & i2,i3,uc)*ux23(i1,i2,i3,vc)+uu(i1,i2,i3,vc)*uy23(i1,i2,i3,vc)+
     & uu(i1,i2,i3,wc)*uz23(i1,i2,i3,vc))  )+normal11(i1,i2,i3,2)*( (
     & nuT*(uxx23(i1,i2,i3,wc)+uyy23(i1,i2,i3,wc)-uxz23(i1,i2,i3,uc)-
     & uyz23(i1,i2,i3,vc))-2.*nuTz*(ux23(i1,i2,i3,uc)+uy23(i1,i2,i3,
     & vc))+nuTx*(ux23(i1,i2,i3,wc)+uz23(i1,i2,i3,uc))+nuTy*(uy23(i1,
     & i2,i3,wc)+uz23(i1,i2,i3,vc)))-advectionCoefficient*(uu(i1,i2,
     & i3,uc)*ux23(i1,i2,i3,wc)+uu(i1,i2,i3,vc)*uy23(i1,i2,i3,wc)+uu(
     & i1,i2,i3,wc)*uz23(i1,i2,i3,wc))  )
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
                             f(i1+is1,i2+is2,i3+is3)=f(i1+is1,i2+is2,
     & i3+is3)-thermalExpansivity*u(i1,i2,i3,tc)*(gravity(0)*normal11(
     & i1,i2,i3,0)+gravity(1)*normal11(i1,i2,i3,1)+gravity(2)*
     & normal11(i1,i2,i3,2))
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
                         if( bc(sidep1,axisp)
     & .eq.inflowWithVelocityGiven .or. bc(sidep1,axisp)
     & .eq.noSlipWall )then
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
                                    k0=u(i1,i2,i3,kc)
                                    e0=u(i1,i2,i3,ec)
                                    nuT = nu + cMu*k0**2/e0
                                    k0x=ux23(i1,i2,i3,kc)
                                    e0x=ux23(i1,i2,i3,ec)
                                    nuTx=cMu*k0*( 2.*k0x*e0 - k0*e0x )
     & /e0**2
                                    k0y=uy23(i1,i2,i3,kc)
                                    e0y=uy23(i1,i2,i3,ec)
                                    nuTy=cMu*k0*( 2.*k0y*e0 - k0*e0y )
     & /e0**2
                                     k0z=uz23(i1,i2,i3,kc)
                                     e0z=uz23(i1,i2,i3,ec)
                                     nuTz=cMu*k0*( 2.*k0z*e0 - k0*e0z )
     & /e0**2
                                    ! normal11-form of the diffusion operator in 3D
                              ! Artificial Dissipation  **todo** implicit-line, self-adjoint versions
                              ! Here now is the statement where the ghost line value in the RHS is assigned.
                                  f(i1+is1,i2+is2,i3+is3)=normal11(i1,
     & i2,i3,0)*( (nuT*(ulaplacian23(i1,i2,i3,uc))+2.*nuTx*(ux23(i1,
     & i2,i3,uc))+nuTy*(uy23(i1,i2,i3,uc)+ux23(i1,i2,i3,vc))+nuTz*(
     & uz23(i1,i2,i3,uc)+ux23(i1,i2,i3,wc)))-advectionCoefficient*(uu(
     & i1,i2,i3,uc)*ux23(i1,i2,i3,uc)+uu(i1,i2,i3,vc)*uy23(i1,i2,i3,
     & uc)+uu(i1,i2,i3,wc)*uz23(i1,i2,i3,uc))  )+normal11(i1,i2,i3,1)*
     & ( (nuT*(ulaplacian23(i1,i2,i3,vc))+2.*nuTy*(uy23(i1,i2,i3,vc))+
     & nuTz*(uz23(i1,i2,i3,vc)+uy23(i1,i2,i3,wc))+nuTx*(ux23(i1,i2,i3,
     & vc)+uy23(i1,i2,i3,uc)))-advectionCoefficient*(uu(i1,i2,i3,uc)*
     & ux23(i1,i2,i3,vc)+uu(i1,i2,i3,vc)*uy23(i1,i2,i3,vc)+uu(i1,i2,
     & i3,wc)*uz23(i1,i2,i3,vc))  )+normal11(i1,i2,i3,2)*( (nuT*(
     & ulaplacian23(i1,i2,i3,wc))+2.*nuTz*(uz23(i1,i2,i3,wc))+nuTx*(
     & ux23(i1,i2,i3,wc)+uz23(i1,i2,i3,uc))+nuTy*(uy23(i1,i2,i3,wc)+
     & uz23(i1,i2,i3,vc)))-advectionCoefficient*(uu(i1,i2,i3,uc)*ux23(
     & i1,i2,i3,wc)+uu(i1,i2,i3,vc)*uy23(i1,i2,i3,wc)+uu(i1,i2,i3,wc)*
     & uz23(i1,i2,i3,wc))  )
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
     & is2,i3+is3)-thermalExpansivity*u(i1,i2,i3,tc)*(gravity(0)*
     & normal11(i1,i2,i3,0)+gravity(1)*normal11(i1,i2,i3,1)+gravity(2)
     & *normal11(i1,i2,i3,2))
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
                   else if( side.eq.0 .and. axis.eq.2 )then
                       ! Use the curl-curl form of the equations
                         do i3=n3a,n3b
                         do i2=n2a,n2b
                         do i1=n1a,n1b
                           if( mask(i1,i2,i3).ne.0 )then
                          ! Define derivative macros before calling this macro
                          ! By default there is no AD:
                            ! get nuT,nuTx,nuTy,nuTz
                                k0=u(i1,i2,i3,kc)
                                e0=u(i1,i2,i3,ec)
                                nuT = nu + cMu*k0**2/e0
                                k0x=ux23(i1,i2,i3,kc)
                                e0x=ux23(i1,i2,i3,ec)
                                nuTx=cMu*k0*( 2.*k0x*e0 - k0*e0x )/e0**
     & 2
                                k0y=uy23(i1,i2,i3,kc)
                                e0y=uy23(i1,i2,i3,ec)
                                nuTy=cMu*k0*( 2.*k0y*e0 - k0*e0y )/e0**
     & 2
                                 k0z=uz23(i1,i2,i3,kc)
                                 e0z=uz23(i1,i2,i3,ec)
                                 nuTz=cMu*k0*( 2.*k0z*e0 - k0*e0z )/e0*
     & *2
                                ! curl-curl form of the diffusion operator in 3D
                          ! Artificial Dissipation  **todo** implicit-line, self-adjoint versions
                          ! Here now is the statement where the ghost line value in the RHS is assigned.
                              f(i1+is1,i2+is2,i3+is3)=normal02(i1,i2,
     & i3,0)*( (nuT*(-uxy23(i1,i2,i3,vc)-uxz23(i1,i2,i3,wc)+uyy23(i1,
     & i2,i3,uc)+uzz23(i1,i2,i3,uc))-2.*nuTx*(uy23(i1,i2,i3,vc)+uz23(
     & i1,i2,i3,wc))+nuTy*(uy23(i1,i2,i3,uc)+ux23(i1,i2,i3,vc))+nuTz*(
     & uz23(i1,i2,i3,uc)+ux23(i1,i2,i3,wc)))-advectionCoefficient*(uu(
     & i1,i2,i3,uc)*ux23(i1,i2,i3,uc)+uu(i1,i2,i3,vc)*uy23(i1,i2,i3,
     & uc)+uu(i1,i2,i3,wc)*uz23(i1,i2,i3,uc))  )+normal02(i1,i2,i3,1)*
     & ( (nuT*(uxx23(i1,i2,i3,vc)-uxy23(i1,i2,i3,uc)-uyz23(i1,i2,i3,
     & wc)+uzz23(i1,i2,i3,vc))-2.*nuTy*(uz23(i1,i2,i3,wc)+ux23(i1,i2,
     & i3,uc))+nuTz*(uz23(i1,i2,i3,vc)+uy23(i1,i2,i3,wc))+nuTx*(ux23(
     & i1,i2,i3,vc)+uy23(i1,i2,i3,uc)))-advectionCoefficient*(uu(i1,
     & i2,i3,uc)*ux23(i1,i2,i3,vc)+uu(i1,i2,i3,vc)*uy23(i1,i2,i3,vc)+
     & uu(i1,i2,i3,wc)*uz23(i1,i2,i3,vc))  )+normal02(i1,i2,i3,2)*( (
     & nuT*(uxx23(i1,i2,i3,wc)+uyy23(i1,i2,i3,wc)-uxz23(i1,i2,i3,uc)-
     & uyz23(i1,i2,i3,vc))-2.*nuTz*(ux23(i1,i2,i3,uc)+uy23(i1,i2,i3,
     & vc))+nuTx*(ux23(i1,i2,i3,wc)+uz23(i1,i2,i3,uc))+nuTy*(uy23(i1,
     & i2,i3,wc)+uz23(i1,i2,i3,vc)))-advectionCoefficient*(uu(i1,i2,
     & i3,uc)*ux23(i1,i2,i3,wc)+uu(i1,i2,i3,vc)*uy23(i1,i2,i3,wc)+uu(
     & i1,i2,i3,wc)*uz23(i1,i2,i3,wc))  )
                           end if
                         end do
                         end do
                         end do
                       ! For the Boussinesq approximation:  -alpha*T* normal02.gravity
                       if( pdeModel.eq.BoussinesqModel .or. 
     & pdeModel.eq.viscoPlasticModel )then
                           do i3=n3a,n3b
                           do i2=n2a,n2b
                           do i1=n1a,n1b
                             if( mask(i1,i2,i3).ne.0 )then
                             f(i1+is1,i2+is2,i3+is3)=f(i1+is1,i2+is2,
     & i3+is3)-thermalExpansivity*u(i1,i2,i3,tc)*(gravity(0)*normal02(
     & i1,i2,i3,0)+gravity(1)*normal02(i1,i2,i3,1)+gravity(2)*
     & normal02(i1,i2,i3,2))
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
                         if( bc(sidep1,axisp)
     & .eq.inflowWithVelocityGiven .or. bc(sidep1,axisp)
     & .eq.noSlipWall )then
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
                                    k0=u(i1,i2,i3,kc)
                                    e0=u(i1,i2,i3,ec)
                                    nuT = nu + cMu*k0**2/e0
                                    k0x=ux23(i1,i2,i3,kc)
                                    e0x=ux23(i1,i2,i3,ec)
                                    nuTx=cMu*k0*( 2.*k0x*e0 - k0*e0x )
     & /e0**2
                                    k0y=uy23(i1,i2,i3,kc)
                                    e0y=uy23(i1,i2,i3,ec)
                                    nuTy=cMu*k0*( 2.*k0y*e0 - k0*e0y )
     & /e0**2
                                     k0z=uz23(i1,i2,i3,kc)
                                     e0z=uz23(i1,i2,i3,ec)
                                     nuTz=cMu*k0*( 2.*k0z*e0 - k0*e0z )
     & /e0**2
                                    ! normal02-form of the diffusion operator in 3D
                              ! Artificial Dissipation  **todo** implicit-line, self-adjoint versions
                              ! Here now is the statement where the ghost line value in the RHS is assigned.
                                  f(i1+is1,i2+is2,i3+is3)=normal02(i1,
     & i2,i3,0)*( (nuT*(ulaplacian23(i1,i2,i3,uc))+2.*nuTx*(ux23(i1,
     & i2,i3,uc))+nuTy*(uy23(i1,i2,i3,uc)+ux23(i1,i2,i3,vc))+nuTz*(
     & uz23(i1,i2,i3,uc)+ux23(i1,i2,i3,wc)))-advectionCoefficient*(uu(
     & i1,i2,i3,uc)*ux23(i1,i2,i3,uc)+uu(i1,i2,i3,vc)*uy23(i1,i2,i3,
     & uc)+uu(i1,i2,i3,wc)*uz23(i1,i2,i3,uc))  )+normal02(i1,i2,i3,1)*
     & ( (nuT*(ulaplacian23(i1,i2,i3,vc))+2.*nuTy*(uy23(i1,i2,i3,vc))+
     & nuTz*(uz23(i1,i2,i3,vc)+uy23(i1,i2,i3,wc))+nuTx*(ux23(i1,i2,i3,
     & vc)+uy23(i1,i2,i3,uc)))-advectionCoefficient*(uu(i1,i2,i3,uc)*
     & ux23(i1,i2,i3,vc)+uu(i1,i2,i3,vc)*uy23(i1,i2,i3,vc)+uu(i1,i2,
     & i3,wc)*uz23(i1,i2,i3,vc))  )+normal02(i1,i2,i3,2)*( (nuT*(
     & ulaplacian23(i1,i2,i3,wc))+2.*nuTz*(uz23(i1,i2,i3,wc))+nuTx*(
     & ux23(i1,i2,i3,wc)+uz23(i1,i2,i3,uc))+nuTy*(uy23(i1,i2,i3,wc)+
     & uz23(i1,i2,i3,vc)))-advectionCoefficient*(uu(i1,i2,i3,uc)*ux23(
     & i1,i2,i3,wc)+uu(i1,i2,i3,vc)*uy23(i1,i2,i3,wc)+uu(i1,i2,i3,wc)*
     & uz23(i1,i2,i3,wc))  )
                               end if
                             end do
                             end do
                             end do
                           ! For the Boussinesq approximation:  -alpha*T* normal02.gravity
                           if( pdeModel.eq.BoussinesqModel .or. 
     & pdeModel.eq.viscoPlasticModel )then
                               do i3=n3a,n3b
                               do i2=n2a,n2b
                               do i1=n1a,n1b
                                 if( mask(i1,i2,i3).ne.0 )then
                                 f(i1+is1,i2+is2,i3+is3)=f(i1+is1,i2+
     & is2,i3+is3)-thermalExpansivity*u(i1,i2,i3,tc)*(gravity(0)*
     & normal02(i1,i2,i3,0)+gravity(1)*normal02(i1,i2,i3,1)+gravity(2)
     & *normal02(i1,i2,i3,2))
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
                   else if( side.eq.1 .and. axis.eq.2 )then
                       ! Use the curl-curl form of the equations
                         do i3=n3a,n3b
                         do i2=n2a,n2b
                         do i1=n1a,n1b
                           if( mask(i1,i2,i3).ne.0 )then
                          ! Define derivative macros before calling this macro
                          ! By default there is no AD:
                            ! get nuT,nuTx,nuTy,nuTz
                                k0=u(i1,i2,i3,kc)
                                e0=u(i1,i2,i3,ec)
                                nuT = nu + cMu*k0**2/e0
                                k0x=ux23(i1,i2,i3,kc)
                                e0x=ux23(i1,i2,i3,ec)
                                nuTx=cMu*k0*( 2.*k0x*e0 - k0*e0x )/e0**
     & 2
                                k0y=uy23(i1,i2,i3,kc)
                                e0y=uy23(i1,i2,i3,ec)
                                nuTy=cMu*k0*( 2.*k0y*e0 - k0*e0y )/e0**
     & 2
                                 k0z=uz23(i1,i2,i3,kc)
                                 e0z=uz23(i1,i2,i3,ec)
                                 nuTz=cMu*k0*( 2.*k0z*e0 - k0*e0z )/e0*
     & *2
                                ! curl-curl form of the diffusion operator in 3D
                          ! Artificial Dissipation  **todo** implicit-line, self-adjoint versions
                          ! Here now is the statement where the ghost line value in the RHS is assigned.
                              f(i1+is1,i2+is2,i3+is3)=normal12(i1,i2,
     & i3,0)*( (nuT*(-uxy23(i1,i2,i3,vc)-uxz23(i1,i2,i3,wc)+uyy23(i1,
     & i2,i3,uc)+uzz23(i1,i2,i3,uc))-2.*nuTx*(uy23(i1,i2,i3,vc)+uz23(
     & i1,i2,i3,wc))+nuTy*(uy23(i1,i2,i3,uc)+ux23(i1,i2,i3,vc))+nuTz*(
     & uz23(i1,i2,i3,uc)+ux23(i1,i2,i3,wc)))-advectionCoefficient*(uu(
     & i1,i2,i3,uc)*ux23(i1,i2,i3,uc)+uu(i1,i2,i3,vc)*uy23(i1,i2,i3,
     & uc)+uu(i1,i2,i3,wc)*uz23(i1,i2,i3,uc))  )+normal12(i1,i2,i3,1)*
     & ( (nuT*(uxx23(i1,i2,i3,vc)-uxy23(i1,i2,i3,uc)-uyz23(i1,i2,i3,
     & wc)+uzz23(i1,i2,i3,vc))-2.*nuTy*(uz23(i1,i2,i3,wc)+ux23(i1,i2,
     & i3,uc))+nuTz*(uz23(i1,i2,i3,vc)+uy23(i1,i2,i3,wc))+nuTx*(ux23(
     & i1,i2,i3,vc)+uy23(i1,i2,i3,uc)))-advectionCoefficient*(uu(i1,
     & i2,i3,uc)*ux23(i1,i2,i3,vc)+uu(i1,i2,i3,vc)*uy23(i1,i2,i3,vc)+
     & uu(i1,i2,i3,wc)*uz23(i1,i2,i3,vc))  )+normal12(i1,i2,i3,2)*( (
     & nuT*(uxx23(i1,i2,i3,wc)+uyy23(i1,i2,i3,wc)-uxz23(i1,i2,i3,uc)-
     & uyz23(i1,i2,i3,vc))-2.*nuTz*(ux23(i1,i2,i3,uc)+uy23(i1,i2,i3,
     & vc))+nuTx*(ux23(i1,i2,i3,wc)+uz23(i1,i2,i3,uc))+nuTy*(uy23(i1,
     & i2,i3,wc)+uz23(i1,i2,i3,vc)))-advectionCoefficient*(uu(i1,i2,
     & i3,uc)*ux23(i1,i2,i3,wc)+uu(i1,i2,i3,vc)*uy23(i1,i2,i3,wc)+uu(
     & i1,i2,i3,wc)*uz23(i1,i2,i3,wc))  )
                           end if
                         end do
                         end do
                         end do
                       ! For the Boussinesq approximation:  -alpha*T* normal12.gravity
                       if( pdeModel.eq.BoussinesqModel .or. 
     & pdeModel.eq.viscoPlasticModel )then
                           do i3=n3a,n3b
                           do i2=n2a,n2b
                           do i1=n1a,n1b
                             if( mask(i1,i2,i3).ne.0 )then
                             f(i1+is1,i2+is2,i3+is3)=f(i1+is1,i2+is2,
     & i3+is3)-thermalExpansivity*u(i1,i2,i3,tc)*(gravity(0)*normal12(
     & i1,i2,i3,0)+gravity(1)*normal12(i1,i2,i3,1)+gravity(2)*
     & normal12(i1,i2,i3,2))
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
                         if( bc(sidep1,axisp)
     & .eq.inflowWithVelocityGiven .or. bc(sidep1,axisp)
     & .eq.noSlipWall )then
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
                                    k0=u(i1,i2,i3,kc)
                                    e0=u(i1,i2,i3,ec)
                                    nuT = nu + cMu*k0**2/e0
                                    k0x=ux23(i1,i2,i3,kc)
                                    e0x=ux23(i1,i2,i3,ec)
                                    nuTx=cMu*k0*( 2.*k0x*e0 - k0*e0x )
     & /e0**2
                                    k0y=uy23(i1,i2,i3,kc)
                                    e0y=uy23(i1,i2,i3,ec)
                                    nuTy=cMu*k0*( 2.*k0y*e0 - k0*e0y )
     & /e0**2
                                     k0z=uz23(i1,i2,i3,kc)
                                     e0z=uz23(i1,i2,i3,ec)
                                     nuTz=cMu*k0*( 2.*k0z*e0 - k0*e0z )
     & /e0**2
                                    ! normal12-form of the diffusion operator in 3D
                              ! Artificial Dissipation  **todo** implicit-line, self-adjoint versions
                              ! Here now is the statement where the ghost line value in the RHS is assigned.
                                  f(i1+is1,i2+is2,i3+is3)=normal12(i1,
     & i2,i3,0)*( (nuT*(ulaplacian23(i1,i2,i3,uc))+2.*nuTx*(ux23(i1,
     & i2,i3,uc))+nuTy*(uy23(i1,i2,i3,uc)+ux23(i1,i2,i3,vc))+nuTz*(
     & uz23(i1,i2,i3,uc)+ux23(i1,i2,i3,wc)))-advectionCoefficient*(uu(
     & i1,i2,i3,uc)*ux23(i1,i2,i3,uc)+uu(i1,i2,i3,vc)*uy23(i1,i2,i3,
     & uc)+uu(i1,i2,i3,wc)*uz23(i1,i2,i3,uc))  )+normal12(i1,i2,i3,1)*
     & ( (nuT*(ulaplacian23(i1,i2,i3,vc))+2.*nuTy*(uy23(i1,i2,i3,vc))+
     & nuTz*(uz23(i1,i2,i3,vc)+uy23(i1,i2,i3,wc))+nuTx*(ux23(i1,i2,i3,
     & vc)+uy23(i1,i2,i3,uc)))-advectionCoefficient*(uu(i1,i2,i3,uc)*
     & ux23(i1,i2,i3,vc)+uu(i1,i2,i3,vc)*uy23(i1,i2,i3,vc)+uu(i1,i2,
     & i3,wc)*uz23(i1,i2,i3,vc))  )+normal12(i1,i2,i3,2)*( (nuT*(
     & ulaplacian23(i1,i2,i3,wc))+2.*nuTz*(uz23(i1,i2,i3,wc))+nuTx*(
     & ux23(i1,i2,i3,wc)+uz23(i1,i2,i3,uc))+nuTy*(uy23(i1,i2,i3,wc)+
     & uz23(i1,i2,i3,vc)))-advectionCoefficient*(uu(i1,i2,i3,uc)*ux23(
     & i1,i2,i3,wc)+uu(i1,i2,i3,vc)*uy23(i1,i2,i3,wc)+uu(i1,i2,i3,wc)*
     & uz23(i1,i2,i3,wc))  )
                               end if
                             end do
                             end do
                             end do
                           ! For the Boussinesq approximation:  -alpha*T* normal12.gravity
                           if( pdeModel.eq.BoussinesqModel .or. 
     & pdeModel.eq.viscoPlasticModel )then
                               do i3=n3a,n3b
                               do i2=n2a,n2b
                               do i1=n1a,n1b
                                 if( mask(i1,i2,i3).ne.0 )then
                                 f(i1+is1,i2+is2,i3+is3)=f(i1+is1,i2+
     & is2,i3+is3)-thermalExpansivity*u(i1,i2,i3,tc)*(gravity(0)*
     & normal12(i1,i2,i3,0)+gravity(1)*normal12(i1,i2,i3,1)+gravity(2)
     & *normal12(i1,i2,i3,2))
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
c end if
          end if ! end bc
         end do ! side
        end do ! axis
        return
        end
