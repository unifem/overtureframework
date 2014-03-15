! This file automatically generated from insLineSolve.bf with bpp.
        subroutine lineSolveINS(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,md1a,md1b,md2a,md2b,md3a,md3b, 
     & mask,rsxy,  u,gv,dt,f,dw,dir,am,bm,cm,dm,em,  bc, 
     & boundaryCondition, ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,
     & ndbcd3b,ndbcd4a,ndbcd4b,bcData, ipar, rpar, ierr )
c======================================================================
c
c nd : number of space dimensions
c
c n1a,n1b,n2a,n2b,n3a,n3b : INTERIOR points (does not include boundary points along axis=dir)
c
c dir : 0,1,2 - direction of line 
c a,b,c : output: tridiagonal matrix
c a,b,c,d,e  : output: penta-diagonal matrix (for fourth-order)
c
c ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,ndbcd3b,ndbcd4a,ndbcd4b : dimensions for the bcData array
c bcData : holds coefficients for BC's
c 
c dw: distance to wall for SA TM
c======================================================================
        implicit none
        integer nd, n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,
     & nd3b,nd4a,nd4b,md1a,md1b,md2a,md2b,md3a,md3b,dir
        real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
        real dw(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
        real am(md1a:md1b,md2a:md2b,md3a:md3b)
        real bm(md1a:md1b,md2a:md2b,md3a:md3b)
        real cm(md1a:md1b,md2a:md2b,md3a:md3b)
        real dm(md1a:md1b,md2a:md2b,md3a:md3b)
        real em(md1a:md1b,md2a:md2b,md3a:md3b)
        real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
        real gv(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
        real dt(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
        real f(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
        integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
        integer bc(0:1,0:*),boundaryCondition(0:1,0:*), ierr
        real dtScale,cfl
        ! bcData(component+numberOfComponents*(0),side,axis,grid)
        integer numberOfComponents
        integer ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,ndbcd3b,
     & ndbcd4a,ndbcd4b
        real bcData(ndbcd1a:ndbcd1b,ndbcd2a:ndbcd2b,ndbcd3a:ndbcd3b,
     & ndbcd4a:ndbcd4b)
        integer ipar(0:*)
        real rpar(0:*)
        !     ---- local variables -----
        integer m,n,c,i1,i2,i3,orderOfAccuracy,gridIsMoving,
     & useWhereMask
        integer gridIsImplicit,implicitOption,implicitMethod,ibc,
     & isAxisymmetric,use2ndOrderAD,use4thOrderAD,
     & useSelfAdjointDiffusion,orderOfExtrapolation,fourthOrder,dirp1,
     & dirp2
        integer pc,uc,vc,wc,tc,fc,fcu,fcv,fcw,fcn,fct,grid,side,
     & gridType
        integer computeMatrix,computeRHS,computeMatrixBC
        integer twilightZoneFlow,computeTemperature
        integer indexRange(0:1,0:2),is1,is2,is3
        real nu,kThermal,thermalExpansivity,gravity(0:2)
        real dx(0:2),dx0,dy,dz,dxi,dyi,dzi,dri,dsi,dti
        real dxv2i(0:2),dx2i,dy2i,dz2i
        real dxvsqi(0:2),dxsqi,dysqi,dzsqi
        real drv2i(0:2),dr2i,ds2i,dt2i
        real drvsqi(0:2),drsqi,dssqi,dtsqi
        real dx12i,dy12i,dz12i,dxsq12i,dysq12i,dzsq12i,dxy4i,dxz4i,
     & dyz4i
        real ad21,ad22,ad41,ad42,cd22,cd42,adc,sn
        real ad21n,ad22n,ad41n,ad42n,cd22n,cd42n
        real dr(0:2)
        real adCoeff2,adCoeff4
        real cexa,cexb,cexc,cexd,cexe
        real c4exa,c4exb,c4exc,c4exd,c4exe
        integer option
        integer assignINS,assignSpalartAllmaras,setupSweep,
     & assignTemperature
        parameter( assignINS=0, assignSpalartAllmaras=1, setupSweep=2, 
     & assignTemperature=3 )
        integer turbulenceModel,noTurbulenceModel
        integer baldwinLomax,spalartAllmaras,kEpsilon,kOmega
        parameter (noTurbulenceModel=0,baldwinLomax=1,kEpsilon=2,
     & kOmega=3,spalartAllmaras=4 )
        real cb1, cb2, cv1, sigma, sigmai, kappa, cw1, cw2, cw3, cw3e6,
     &  cv1e3, cd0, cr0
        real dd,dndx(0:2)
        integer axis,kd
        real kbl,alpha,a0p,ccp,ckleb,cwk !baldwin-lomax constants
        real magu,magumax,ymax,ulmax,lmixw,lmixmax,lmix2max,vto,vort,
     & fdotn,tawu ! baldwin-lomax tmp variables
        real yscale,yplus,nmag,ftan(3),norm(3),tauw,maxumag,maxvt,
     & ctrans,ditrip ! more baldwin-lomax tmp variables
        integer iswitch, ibb, ibe, i, ii1,ii2,ii3,io(3) ! baldwin-lomax loop variables
        integer itrip,jtrip,ktrip !baldwin-lomax trip location
        real chi,fnu1,fnu2,s,r,g,fw,dKappaSq,nBydSqLhs,nSqBydSq,nutb
        real nuTilde,nuT,nuTx(0:2),fv1,fv1x,fv1y,fv1z
        real nuTSA,chi3,nuTd
        real urr0,uss0,utt0
        integer nc
        real fsa2d0,fsa2d1,fsa2d2
        real fsa3d0,fsa3d1,fsa3d2
        real fsac2d0,fsac2d1,fsac2d2
        real fsac3d0,fsac3d1,fsac3d2
        real fusar2d0,fusar2d1,fusar2d2
        real fusar3d0,fusar3d1,fusar3d2
        real fusac2d0,fusac2d1,fusac2d2
        real fusac3d0,fusac3d1,fusac3d2
        integer noSlipWall,outflow,convectiveOutflow,tractionFree,
     & inflowWithPandTV,dirichletBoundaryCondition,symmetry,
     & axisymmetric
        parameter( noSlipWall=1,outflow=5,convectiveOutflow=14,
     & tractionFree=15,inflowWithPandTV=3,
     & dirichletBoundaryCondition=12,symmetry=11,axisymmetric=13 )
        integer rectangular,curvilinear
        parameter( rectangular=0, curvilinear=1 )
        integer interpolate,dirichlet,neumann,extrapolate
        parameter( interpolate=0, dirichlet=1, neumann=2, 
     & extrapolate=3 )
        integer pdeModel,standardModel,BoussinesqModel,
     & viscoPlasticModel
        parameter( standardModel=0,BoussinesqModel=1,
     & viscoPlasticModel=2 )
        !     --- begin statement functions
        real t1,t2
        real fr2d0,fr2d1,fr2d2,fc2d0,fc2d1,fc2d2
        ! real resr2d0,resr2d1
        real fr3d0,fr3d1,fr3d2,fc3d0,fc3d1,fc3d2
        real ftr2d0,ftr2d1,ftr2d2,ftc2d0,ftc2d1,ftc2d2
        real ftr3d0,ftr3d1,ftr3d2,ftc3d0,ftc3d1,ftc3d2
        real uAve0,uAve1,uAve2,uAve3d0,uAve3d1,uAve3d2
        real ad2Coeff,ad2,ad23Coeff,ad23,ad4Coeff,ad4,ad43Coeff,ad43
        real ad2cCoeff,ad23cCoeff
        real ad2nCoeff,ad23nCoeff,ad2cnCoeff,ad23cnCoeff
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
        real adSelfAdjoint2dR,adSelfAdjoint3dR,adSelfAdjoint2dC,
     & adSelfAdjoint3dC
        real adSelfAdjoint2dRSA,adSelfAdjoint3dRSA,adSelfAdjoint2dCSA,
     & adSelfAdjoint3dCSA
        real rxi,rxr,rxs,rxt,rxx,rxy,ryy,rxx3,rxy3,rxz3
        real ur,us,ut,urs,urt,ust,urr,uss,utt
        real uxx0,uyy0,uzz0,ux2c,uy2c,ux3c,uy3c,uz3c
        real lap2d2c,lap3d2c
        real uu, ux2,uy2,uz2,uxx2,uyy2,uzz2,lap2d2,lap3d2
        real ux4,uy4,uz4,uxx4,lap2d4,lap3d4,uxy2,uxz2,uyz2,uxy4,uxz4,
     & uyz4,uyy4,uzz4
        real mixedRHS,mixedCoeff,mixedNormalCoeff,a0,a1
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
        !*      include 'insDeriv.h'
        !*      include 'insDerivc.h'
        uu(c)    = u(i1,i2,i3,c)
        ux2(c)   = ux22r(i1,i2,i3,c)
        uy2(c)   = uy22r(i1,i2,i3,c)
        uz2(c)   = uz23r(i1,i2,i3,c)
        uxy2(c)  = uxy22r(i1,i2,i3,c)
        uxz2(c)  = uxz23r(i1,i2,i3,c)
        uyz2(c)  = uyz23r(i1,i2,i3,c)
        uxx2(c)  = uxx22r(i1,i2,i3,c)
        uyy2(c)  = uyy22r(i1,i2,i3,c)
        uzz2(c)  = uzz23r(i1,i2,i3,c)
        lap2d2(c)= ulaplacian22r(i1,i2,i3,c)
        lap3d2(c)= ulaplacian23r(i1,i2,i3,c)
        ux4(c)   = ux42r(i1,i2,i3,c)
        uy4(c)   = uy42r(i1,i2,i3,c)
        uz4(c)   = uz43r(i1,i2,i3,c)
        uxy4(c)  = uxy42r(i1,i2,i3,c)
        uxz4(c)  = uxz43r(i1,i2,i3,c)
        uyz4(c)  = uyz43r(i1,i2,i3,c)
        uxx4(c)  = uxx42r(i1,i2,i3,c)
        uyy4(c)  = uyy42r(i1,i2,i3,c)
        uzz4(c)  = uzz43r(i1,i2,i3,c)
        lap2d4(c)= ulaplacian42r(i1,i2,i3,c)
        lap3d4(c)= ulaplacian43r(i1,i2,i3,c)
        rxi(m,n) = rsxy(i1,i2,i3,m,n)
        rxr(m,n) = (rsxy(i1+1,i2,i3,m,n)-rsxy(i1-1,i2,i3,m,n))*dr2i
        rxs(m,n) = (rsxy(i1,i2+1,i3,m,n)-rsxy(i1,i2-1,i3,m,n))*ds2i
        rxt(m,n) = (rsxy(i1,i2,i3+1,m,n)-rsxy(i1,i2,i3-1,m,n))*dt2i
        rxx(m,n) = rxi(0,0)*rxr(m,n)+rxi(1,0)*rxs(m,n)
        rxy(m,n) = rxi(0,1)*rxr(m,n)+rxi(1,1)*rxs(m,n)
        ryy(m,n) = rxy(m,n)
        rxx3(m,n)= rxi(0,0)*rxr(m,n)+rxi(1,0)*rxs(m,n)+rxi(2,0)*rxt(m,
     & n)
        rxy3(m,n)= rxi(0,1)*rxr(m,n)+rxi(1,1)*rxs(m,n)+rxi(2,1)*rxt(m,
     & n)
        rxz3(m,n)= rxi(0,2)*rxr(m,n)+rxi(1,2)*rxs(m,n)+rxi(2,2)*rxt(m,
     & n)
        ur(m) = ur2(i1,i2,i3,m)
        us(m) = us2(i1,i2,i3,m)
        ut(m) = ut2(i1,i2,i3,m)
        urs(m)= urs2(i1,i2,i3,m)
        urt(m)= urt2(i1,i2,i3,m)
        ust(m)= ust2(i1,i2,i3,m)
        urr(m)= urr2(i1,i2,i3,m)
        uss(m)= uss2(i1,i2,i3,m)
        utt(m)= utt2(i1,i2,i3,m)
        ux2c(m) = ux22(i1,i2,i3,m)
        uy2c(m) = uy22(i1,i2,i3,m)
        ux3c(m) = ux23(i1,i2,i3,m)
        uy3c(m) = uy23(i1,i2,i3,m)
        uz3c(m) = uz23(i1,i2,i3,m)
        lap2d2c(m) = ulaplacian22(i1,i2,i3,m)
        lap3d2c(m) = ulaplacian23(i1,i2,i3,m)
        !      ux(c) = (u(i1+1,i2,i3,c)-u(i1-1,i2,i3,c))*dx2i
        !      uy(c) = (u(i1,i2+1,i3,c)-u(i1,i2-1,i3,c))*dy2i
        !      uxx(c) = (u(i1+1,i2,i3,c)-2.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c))*dxsqi
        !      uyy(c) = (u(i1,i2+1,i3,c)-2.*u(i1,i2,i3,c)+u(i1,i2-1,i3,c))*dysqi
        uxx0(c) = (u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c))*dxsqi  ! without diagonal term
        uyy0(c) = (u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c))*dysqi  ! without diagonal term
        uzz0(c) = (u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c))*dzsqi  ! without diagonal term
        urr0(m)  = (u(i1+1,i2,i3,m)+u(i1-1,i2,i3,m))*drsqi  ! without diagonal term
        uss0(m)  = (u(i1,i2+1,i3,m)+u(i1,i2-1,i3,m))*dssqi  ! without diagonal term
        utt0(m)  = (u(i1,i2,i3+1,m)+u(i1,i2,i3-1,m))*dtsqi  ! without diagonal term
        uAve0(c) = (u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c))
        uAve1(c) = (u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c))
        uAve2(c) = 0.
        uAve3d0(c) = (u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c)+u(i1,i2,i3+1,c)+
     & u(i1,i2,i3-1,c))
        uAve3d1(c) = (u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)+u(i1,i2,i3+1,c)+
     & u(i1,i2,i3-1,c))
        uAve3d2(c) = (u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)+u(i1,i2+1,i3,c)+
     & u(i1,i2-1,i3,c))
       ! resr2d0(m) = f(i1,i2,i3,0) -uu(uc)*ux2(m)-uu(vc)*uy2(m)+nu*lap2d2(m)
       ! resr2d1(m) = f(i1,i2,i3,1) -uu(uc)*ux2(m)-uu(vc)*uy2(m)+nu*lap2d2(m)
        ! INS - RHS forcing for rectangular grids, directions=0,1,2 (do NOT include grad(p) terms, since then macro is valid for m=uc,vc,wc)
        fr2d0(m) = uu(m)*dtScale/dt(i1,i2,i3) -uu(vc)*uy2(m)+nu*uyy0(m)
     &  -thermalExpansivity*gravity(m-uc)*u(i1,i2,i3,tc)
        fr2d1(m) = uu(m)*dtScale/dt(i1,i2,i3) -uu(uc)*ux2(m)+nu*uxx0(m)
     &  -thermalExpansivity*gravity(m-uc)*u(i1,i2,i3,tc)
        fr2d2(m) = 0.
        fr3d0(m) = uu(m)*dtScale/dt(i1,i2,i3) -uu(vc)*uy2(m)-uu(wc)*
     & uz2(m)+nu*(uyy0(m)+uzz0(m)) -thermalExpansivity*gravity(m-uc)*
     & u(i1,i2,i3,tc)
        fr3d1(m) = uu(m)*dtScale/dt(i1,i2,i3) -uu(uc)*ux2(m)-uu(wc)*
     & uz2(m)+nu*(uxx0(m)+uzz0(m)) -thermalExpansivity*gravity(m-uc)*
     & u(i1,i2,i3,tc)
        fr3d2(m) = uu(m)*dtScale/dt(i1,i2,i3) -uu(uc)*ux2(m)-uu(vc)*
     & uy2(m)+nu*(uxx0(m)+uyy0(m)) -thermalExpansivity*gravity(m-uc)*
     & u(i1,i2,i3,tc)
        ! Temperature - RHS, forcing for rectangular grids, directions=0,1,2
        ftr2d0(m) = uu(m)*dtScale/dt(i1,i2,i3) -uu(vc)*uy2(m)+kThermal*
     & uyy0(m)
        ftr2d1(m) = uu(m)*dtScale/dt(i1,i2,i3) -uu(uc)*ux2(m)+kThermal*
     & uxx0(m)
        ftr2d2(m) = 0.
        ftr3d0(m) = uu(m)*dtScale/dt(i1,i2,i3) -uu(vc)*uy2(m)-uu(wc)*
     & uz2(m)+kThermal*(uyy0(m)+uzz0(m))
        ftr3d1(m) = uu(m)*dtScale/dt(i1,i2,i3) -uu(uc)*ux2(m)-uu(wc)*
     & uz2(m)+kThermal*(uxx0(m)+uzz0(m))
        ftr3d2(m) = uu(m)*dtScale/dt(i1,i2,i3) -uu(uc)*ux2(m)-uu(vc)*
     & uy2(m)+kThermal*(uxx0(m)+uyy0(m))
        ! INS - RHS forcing for curvilinear grids, directions=0,1,2  (do NOT include grad(p) terms)
        fc2d0(m)=uu(m)*dtScale/dt(i1,i2,i3)  +(-uu(uc)*rxi(1,0)-uu(vc)*
     & rxi(1,1)+nu*(rxx(1,0)+ryy(1,1)))*us(m)+nu*((rxi(1,0)**2+rxi(1,
     & 1)**2)*uss0(m)+2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1))*urs(m))
     &  -thermalExpansivity*gravity(m-uc)*u(i1,i2,i3,tc)
        fc2d1(m)=uu(m)*dtScale/dt(i1,i2,i3) +(-uu(uc)*rxi(0,0)-uu(vc)*
     & rxi(0,1)+nu*(rxx(0,0)+ryy(0,1)))*ur(m)+nu*((rxi(0,0)**2+rxi(0,
     & 1)**2)*urr0(m)+2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1))*urs(m))
     &  -thermalExpansivity*gravity(m-uc)*u(i1,i2,i3,tc)
        fc2d2(m) = 0.
        fc3d0(m)=uu(m)*dtScale/dt(i1,i2,i3)  +(-uu(uc)*rxi(1,0)-uu(vc)*
     & rxi(1,1)-uu(wc)*rxi(1,2)+nu*(rxx3(1,0)+rxy3(1,1)+rxz3(1,2)))*
     & us(m)+(-uu(uc)*rxi(2,0)-uu(vc)*rxi(2,1)-uu(wc)*rxi(2,2)+nu*(
     & rxx3(2,0)+rxy3(2,1)+rxz3(2,2)))*ut(m)+nu*( (rxi(1,0)**2+rxi(1,
     & 1)**2+rxi(1,2)**2)*uss0(m)+(rxi(2,0)**2+rxi(2,1)**2+rxi(2,2)**
     & 2)*utt0(m)+2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1)+rxi(0,2)*
     & rxi(1,2))*urs(m)+2.*(rxi(0,0)*rxi(2,0)+rxi(0,1)*rxi(2,1)+rxi(0,
     & 2)*rxi(2,2))*urt(m)+2.*(rxi(1,0)*rxi(2,0)+rxi(1,1)*rxi(2,1)+
     & rxi(1,2)*rxi(2,2))*ust(m)) -thermalExpansivity*gravity(m-uc)*u(
     & i1,i2,i3,tc)
        fc3d1(m)=uu(m)*dtScale/dt(i1,i2,i3)  +(-uu(uc)*rxi(0,0)-uu(vc)*
     & rxi(0,1)-uu(wc)*rxi(0,2)+nu*(rxx3(0,0)+rxy3(0,1)+rxz3(0,2)))*
     & ur(m)+(-uu(uc)*rxi(2,0)-uu(vc)*rxi(2,1)-uu(wc)*rxi(2,2)+nu*(
     & rxx3(2,0)+rxy3(2,1)+rxz3(2,2)))*ut(m)+nu*( (rxi(0,0)**2+rxi(0,
     & 1)**2+rxi(0,2)**2)*urr0(m)+(rxi(2,0)**2+rxi(2,1)**2+rxi(2,2)**
     & 2)*utt0(m)+2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1)+rxi(0,2)*
     & rxi(1,2))*urs(m)+2.*(rxi(0,0)*rxi(2,0)+rxi(0,1)*rxi(2,1)+rxi(0,
     & 2)*rxi(2,2))*urt(m)+2.*(rxi(1,0)*rxi(2,0)+rxi(1,1)*rxi(2,1)+
     & rxi(1,2)*rxi(2,2))*ust(m)) -thermalExpansivity*gravity(m-uc)*u(
     & i1,i2,i3,tc)
        fc3d2(m)=uu(m)*dtScale/dt(i1,i2,i3)  +(-uu(uc)*rxi(0,0)-uu(vc)*
     & rxi(0,1)-uu(wc)*rxi(0,2)+nu*(rxx3(0,0)+rxy3(0,1)+rxz3(0,2)))*
     & ur(m)+(-uu(uc)*rxi(1,0)-uu(vc)*rxi(1,1)-uu(wc)*rxi(1,2)+nu*(
     & rxx3(1,0)+rxy3(1,1)+rxz3(1,2)))*us(m)+nu*( (rxi(0,0)**2+rxi(0,
     & 1)**2+rxi(0,2)**2)*urr0(m)+(rxi(1,0)**2+rxi(1,1)**2+rxi(1,2)**
     & 2)*uss0(m)+2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1)+rxi(0,2)*
     & rxi(1,2))*urs(m)+2.*(rxi(0,0)*rxi(2,0)+rxi(0,1)*rxi(2,1)+rxi(0,
     & 2)*rxi(2,2))*urt(m)+2.*(rxi(1,0)*rxi(2,0)+rxi(1,1)*rxi(2,1)+
     & rxi(1,2)*rxi(2,2))*ust(m)) -thermalExpansivity*gravity(m-uc)*u(
     & i1,i2,i3,tc)
        ! Temperature - RHS forcing for curvilinear grids, directions=0,1,2
        ftc2d0(m)=uu(m)*dtScale/dt(i1,i2,i3)  +(-uu(uc)*rxi(1,0)-uu(vc)
     & *rxi(1,1)+kThermal*(rxx(1,0)+ryy(1,1)))*us(m)+kThermal*((rxi(1,
     & 0)**2+rxi(1,1)**2)*uss0(m)+2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(
     & 1,1))*urs(m))
        ftc2d1(m)=uu(m)*dtScale/dt(i1,i2,i3) +(-uu(uc)*rxi(0,0)-uu(vc)*
     & rxi(0,1)+kThermal*(rxx(0,0)+ryy(0,1)))*ur(m)+kThermal*((rxi(0,
     & 0)**2+rxi(0,1)**2)*urr0(m)+2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(
     & 1,1))*urs(m))
        ftc2d2(m) = 0.
        ftc3d0(m)=uu(m)*dtScale/dt(i1,i2,i3)  +(-uu(uc)*rxi(1,0)-uu(vc)
     & *rxi(1,1)-uu(wc)*rxi(1,2)+kThermal*(rxx3(1,0)+rxy3(1,1)+rxz3(1,
     & 2)))*us(m)+(-uu(uc)*rxi(2,0)-uu(vc)*rxi(2,1)-uu(wc)*rxi(2,2)+
     & kThermal*(rxx3(2,0)+rxy3(2,1)+rxz3(2,2)))*ut(m)+kThermal*( (
     & rxi(1,0)**2+rxi(1,1)**2+rxi(1,2)**2)*uss0(m)+(rxi(2,0)**2+rxi(
     & 2,1)**2+rxi(2,2)**2)*utt0(m)+2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*
     & rxi(1,1)+rxi(0,2)*rxi(1,2))*urs(m)+2.*(rxi(0,0)*rxi(2,0)+rxi(0,
     & 1)*rxi(2,1)+rxi(0,2)*rxi(2,2))*urt(m)+2.*(rxi(1,0)*rxi(2,0)+
     & rxi(1,1)*rxi(2,1)+rxi(1,2)*rxi(2,2))*ust(m))
        ftc3d1(m)=uu(m)*dtScale/dt(i1,i2,i3)  +(-uu(uc)*rxi(0,0)-uu(vc)
     & *rxi(0,1)-uu(wc)*rxi(0,2)+kThermal*(rxx3(0,0)+rxy3(0,1)+rxz3(0,
     & 2)))*ur(m)+(-uu(uc)*rxi(2,0)-uu(vc)*rxi(2,1)-uu(wc)*rxi(2,2)+
     & kThermal*(rxx3(2,0)+rxy3(2,1)+rxz3(2,2)))*ut(m)+kThermal*( (
     & rxi(0,0)**2+rxi(0,1)**2+rxi(0,2)**2)*urr0(m)+(rxi(2,0)**2+rxi(
     & 2,1)**2+rxi(2,2)**2)*utt0(m)+2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*
     & rxi(1,1)+rxi(0,2)*rxi(1,2))*urs(m)+2.*(rxi(0,0)*rxi(2,0)+rxi(0,
     & 1)*rxi(2,1)+rxi(0,2)*rxi(2,2))*urt(m)+2.*(rxi(1,0)*rxi(2,0)+
     & rxi(1,1)*rxi(2,1)+rxi(1,2)*rxi(2,2))*ust(m))
        ftc3d2(m)=uu(m)*dtScale/dt(i1,i2,i3)  +(-uu(uc)*rxi(0,0)-uu(vc)
     & *rxi(0,1)-uu(wc)*rxi(0,2)+kThermal*(rxx3(0,0)+rxy3(0,1)+rxz3(0,
     & 2)))*ur(m)+(-uu(uc)*rxi(1,0)-uu(vc)*rxi(1,1)-uu(wc)*rxi(1,2)+
     & kThermal*(rxx3(1,0)+rxy3(1,1)+rxz3(1,2)))*us(m)+kThermal*( (
     & rxi(0,0)**2+rxi(0,1)**2+rxi(0,2)**2)*urr0(m)+(rxi(1,0)**2+rxi(
     & 1,1)**2+rxi(1,2)**2)*uss0(m)+2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*
     & rxi(1,1)+rxi(0,2)*rxi(1,2))*urs(m)+2.*(rxi(0,0)*rxi(2,0)+rxi(0,
     & 1)*rxi(2,1)+rxi(0,2)*rxi(2,2))*urt(m)+2.*(rxi(1,0)*rxi(2,0)+
     & rxi(1,1)*rxi(2,1)+rxi(1,2)*rxi(2,2))*ust(m))
        !     --- for SA TM ---
        nuTSA(i1,i2,i3)=u(i1,i2,i3,nc)*(u(i1,i2,i3,nc)/nu)**3/((u(i1,
     & i2,i3,nc)/nu)**3+cv1e3)
        fusar2d0(m) = uu(m)*dtScale/dt(i1,i2,i3) -uu(vc)*uy2(m)+nuT*
     & uyy0(m)
        fusar2d1(m) = uu(m)*dtScale/dt(i1,i2,i3) -uu(uc)*ux2(m)+nuT*
     & uxx0(m)
        fusar2d2(m) = 0.
        fusar3d0(m) = uu(m)*dtScale/dt(i1,i2,i3) -uu(vc)*uy2(m)-uu(wc)*
     & uz2(m)+nuT*(uyy0(m)+uzz0(m))
        fusar3d1(m) = uu(m)*dtScale/dt(i1,i2,i3) -uu(uc)*ux2(m)-uu(wc)*
     & uz2(m)+nuT*(uxx0(m)+uzz0(m))
        fusar3d2(m) = uu(m)*dtScale/dt(i1,i2,i3) -uu(uc)*ux2(m)-uu(vc)*
     & uy2(m)+nuT*(uxx0(m)+uyy0(m))
        fsa2d0(m)=uu(m)*dtScale/dt(i1,i2,i3) -uu(vc)*dndx(1)+nutb*uyy0(
     & nc)+(1.+cb2)*sigmai*dndx(1)**2
        fsa2d1(m)=uu(m)*dtScale/dt(i1,i2,i3) -uu(uc)*dndx(0)+nutb*uxx0(
     & nc)+(1.+cb2)*sigmai*dndx(0)**2
        fsa2d2(m)=0.
        fsa3d0(m)=uu(m)*dtScale/dt(i1,i2,i3) -uu(vc)*dndx(1)-uu(wc)*
     & dndx(2)+nutb*(uyy0(nc)+uzz0(nc))+(1.+cb2)*sigmai*(dndx(1)**2+
     & dndx(2)**2)
        fsa3d1(m)=uu(m)*dtScale/dt(i1,i2,i3) -uu(uc)*dndx(0)-uu(wc)*
     & dndx(2)+nutb*(uxx0(nc)+uzz0(nc))+(1.+cb2)*sigmai*(dndx(0)**2+
     & dndx(2)**2)
        fsa3d2(m)=uu(m)*dtScale/dt(i1,i2,i3) -uu(uc)*dndx(0)-uu(vc)*
     & dndx(1)+nutb*(uxx0(nc)+uyy0(nc))+(1.+cb2)*sigmai*(dndx(0)**2+
     & dndx(1)**2)
        !     --- SA TM curvilinear grid case ----    
        ! The momentum equations are the same as above but with nu replaced by nuT
        fusac2d0(m)=uu(m)*dtScale/dt(i1,i2,i3)  +(-uu(uc)*rxi(1,0)-uu(
     & vc)*rxi(1,1)+nuT*(rxx(1,0)+ryy(1,1)))*us(m)+nuT*((rxi(1,0)**2+
     & rxi(1,1)**2)*uss0(m)+2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1))*
     & urs(m))
        fusac2d1(m)=uu(m)*dtScale/dt(i1,i2,i3) +(-uu(uc)*rxi(0,0)-uu(
     & vc)*rxi(0,1)+nuT*(rxx(0,0)+ryy(0,1)))*ur(m)+nuT*((rxi(0,0)**2+
     & rxi(0,1)**2)*urr0(m)+2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1))*
     & urs(m))
        fusac2d2(m) = 0.
        fusac3d0(m)=uu(m)*dtScale/dt(i1,i2,i3)  +(-uu(uc)*rxi(1,0)-uu(
     & vc)*rxi(1,1)-uu(wc)*rxi(1,2)+nuT*(rxx3(1,0)+rxy3(1,1)+rxz3(1,2)
     & ))*us(m)+(-uu(uc)*rxi(2,0)-uu(vc)*rxi(2,1)-uu(wc)*rxi(2,2)+nuT*
     & (rxx3(2,0)+rxy3(2,1)+rxz3(2,2)))*ut(m)+nuT*( (rxi(1,0)**2+rxi(
     & 1,1)**2+rxi(1,2)**2)*uss0(m)+(rxi(2,0)**2+rxi(2,1)**2+rxi(2,2)*
     & *2)*utt0(m)+2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1)+rxi(0,2)*
     & rxi(1,2))*urs(m)+2.*(rxi(0,0)*rxi(2,0)+rxi(0,1)*rxi(2,1)+rxi(0,
     & 2)*rxi(2,2))*urt(m)+2.*(rxi(1,0)*rxi(2,0)+rxi(1,1)*rxi(2,1)+
     & rxi(1,2)*rxi(2,2))*ust(m))
        fusac3d1(m)=uu(m)*dtScale/dt(i1,i2,i3)  +(-uu(uc)*rxi(0,0)-uu(
     & vc)*rxi(0,1)-uu(wc)*rxi(0,2)+nuT*(rxx3(0,0)+rxy3(0,1)+rxz3(0,2)
     & ))*ur(m)+(-uu(uc)*rxi(2,0)-uu(vc)*rxi(2,1)-uu(wc)*rxi(2,2)+nuT*
     & (rxx3(2,0)+rxy3(2,1)+rxz3(2,2)))*ut(m)+nuT*( (rxi(0,0)**2+rxi(
     & 0,1)**2+rxi(0,2)**2)*urr0(m)+(rxi(2,0)**2+rxi(2,1)**2+rxi(2,2)*
     & *2)*utt0(m)+2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1)+rxi(0,2)*
     & rxi(1,2))*urs(m)+2.*(rxi(0,0)*rxi(2,0)+rxi(0,1)*rxi(2,1)+rxi(0,
     & 2)*rxi(2,2))*urt(m)+2.*(rxi(1,0)*rxi(2,0)+rxi(1,1)*rxi(2,1)+
     & rxi(1,2)*rxi(2,2))*ust(m))
        fusac3d2(m)=uu(m)*dtScale/dt(i1,i2,i3)  +(-uu(uc)*rxi(0,0)-uu(
     & vc)*rxi(0,1)-uu(wc)*rxi(0,2)+nuT*(rxx3(0,0)+rxy3(0,1)+rxz3(0,2)
     & ))*ur(m)+(-uu(uc)*rxi(1,0)-uu(vc)*rxi(1,1)-uu(wc)*rxi(1,2)+nuT*
     & (rxx3(1,0)+rxy3(1,1)+rxz3(1,2)))*us(m)+nuT*( (rxi(0,0)**2+rxi(
     & 0,1)**2+rxi(0,2)**2)*urr0(m)+(rxi(1,0)**2+rxi(1,1)**2+rxi(1,2)*
     & *2)*uss0(m)+2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1)+rxi(0,2)*
     & rxi(1,2))*urs(m)+2.*(rxi(0,0)*rxi(2,0)+rxi(0,1)*rxi(2,1)+rxi(0,
     & 2)*rxi(2,2))*urt(m)+2.*(rxi(1,0)*rxi(2,0)+rxi(1,1)*rxi(2,1)+
     & rxi(1,2)*rxi(2,2))*ust(m))
        fsac2d0(m)=uu(m)*dtScale/dt(i1,i2,i3)+ ( (-uu(uc)+(1.+cb2)*
     & sigmai*dndx(0))*rxi(1,0)+   (-uu(vc)+(1.+cb2)*sigmai*dndx(1))*
     & rxi(1,1)+   nutb*(rxx(1,0)+ryy(1,1)) )*us(m)+  nutb*( (rxi(1,0)
     & **2+rxi(1,1)**2)*uss0(m)+         2.*(rxi(0,0)*rxi(1,0)+rxi(0,
     & 1)*rxi(1,1))*urs(m) )
        fsac2d1(m)=uu(m)*dtScale/dt(i1,i2,i3)+ ( (-uu(uc)+(1.+cb2)*
     & sigmai*dndx(0))*rxi(0,0)+   (-uu(vc)+(1.+cb2)*sigmai*dndx(1))*
     & rxi(0,1)+   nutb*(rxx(0,0)+ryy(0,1)) )*ur(m)+  nutb*( (rxi(0,0)
     & **2+rxi(0,1)**2)*urr0(m)+         2.*(rxi(0,0)*rxi(1,0)+rxi(0,
     & 1)*rxi(1,1))*urs(m) )
        fsac2d2(m)=0.
        fsac3d0(m)=uu(m)*dtScale/dt(i1,i2,i3)+ ( (-uu(uc)+(1.+cb2)*
     & sigmai*dndx(0))*rxi(1,0)+   (-uu(vc)+(1.+cb2)*sigmai*dndx(1))*
     & rxi(1,1)+   (-uu(wc)+(1.+cb2)*sigmai*dndx(2))*rxi(1,2)+     
     & nutb*(rxx3(1,0)+rxy3(1,1)+rxz3(1,2)) )*us(m)+ ( (-uu(uc)+(1.+
     & cb2)*sigmai*dndx(0))*rxi(2,0)+   (-uu(vc)+(1.+cb2)*sigmai*dndx(
     & 1))*rxi(2,1)+   (-uu(wc)+(1.+cb2)*sigmai*dndx(2))*rxi(2,2)+    
     &  nutb*(rxx3(2,0)+rxy3(2,1)+rxz3(2,2)) )*ut(m) +  nutb*( (rxi(1,
     & 0)**2+rxi(1,1)**2+rxi(1,2)**2)*uss0(m)+         (rxi(2,0)**2+
     & rxi(2,1)**2+rxi(2,2)**2)*utt0(m)+         2.*(rxi(0,0)*rxi(1,0)
     & +rxi(0,1)*rxi(1,1)+rxi(0,2)*rxi(1,2))*urs(m)+         2.*(rxi(
     & 0,0)*rxi(2,0)+rxi(0,1)*rxi(2,1)+rxi(0,2)*rxi(2,2))*urt(m)+     
     &     2.*(rxi(1,0)*rxi(2,0)+rxi(1,1)*rxi(2,1)+rxi(1,2)*rxi(2,2))*
     & ust(m) )
        fsac3d1(m)=uu(m)*dtScale/dt(i1,i2,i3)+ ( (-uu(uc)+(1.+cb2)*
     & sigmai*dndx(0))*rxi(0,0)+   (-uu(vc)+(1.+cb2)*sigmai*dndx(1))*
     & rxi(0,1)+   (-uu(wc)+(1.+cb2)*sigmai*dndx(2))*rxi(0,2)+     
     & nutb*(rxx3(0,0)+rxy3(0,1)+rxz3(0,2)) )*ur(m)+ ( (-uu(uc)+(1.+
     & cb2)*sigmai*dndx(0))*rxi(2,0)+   (-uu(vc)+(1.+cb2)*sigmai*dndx(
     & 1))*rxi(2,1)+   (-uu(wc)+(1.+cb2)*sigmai*dndx(2))*rxi(2,2)+    
     &  nutb*(rxx3(2,0)+rxy3(2,1)+rxz3(2,2)) )*ut(m) +  nutb*( (rxi(0,
     & 0)**2+rxi(0,1)**2+rxi(0,2)**2)*urr0(m)+         (rxi(2,0)**2+
     & rxi(2,1)**2+rxi(2,2)**2)*utt0(m)+         2.*(rxi(0,0)*rxi(1,0)
     & +rxi(0,1)*rxi(1,1)+rxi(0,2)*rxi(1,2))*urs(m)+         2.*(rxi(
     & 0,0)*rxi(2,0)+rxi(0,1)*rxi(2,1)+rxi(0,2)*rxi(2,2))*urt(m)+     
     &     2.*(rxi(1,0)*rxi(2,0)+rxi(1,1)*rxi(2,1)+rxi(1,2)*rxi(2,2))*
     & ust(m) )
        fsac3d2(m)=uu(m)*dtScale/dt(i1,i2,i3)+ ( (-uu(uc)+(1.+cb2)*
     & sigmai*dndx(0))*rxi(0,0)+   (-uu(vc)+(1.+cb2)*sigmai*dndx(1))*
     & rxi(0,1)+   (-uu(wc)+(1.+cb2)*sigmai*dndx(2))*rxi(0,2)+     
     & nutb*(rxx3(0,0)+rxy3(0,1)+rxz3(0,2)) )*ur(m)+ ( (-uu(uc)+(1.+
     & cb2)*sigmai*dndx(0))*rxi(1,0)+   (-uu(vc)+(1.+cb2)*sigmai*dndx(
     & 1))*rxi(1,1)+   (-uu(wc)+(1.+cb2)*sigmai*dndx(2))*rxi(1,2)+    
     &  nutb*(rxx3(1,0)+rxy3(1,1)+rxz3(1,2)) )*us(m) +  nutb*( (rxi(0,
     & 0)**2+rxi(0,1)**2+rxi(0,2)**2)*urr0(m)+         (rxi(1,0)**2+
     & rxi(1,1)**2+rxi(1,2)**2)*uss0(m)+         2.*(rxi(0,0)*rxi(1,0)
     & +rxi(0,1)*rxi(1,1)+rxi(0,2)*rxi(1,2))*urs(m)+         2.*(rxi(
     & 0,0)*rxi(2,0)+rxi(0,1)*rxi(2,1)+rxi(0,2)*rxi(2,2))*urt(m)+     
     &     2.*(rxi(1,0)*rxi(2,0)+rxi(1,1)*rxi(2,1)+rxi(1,2)*rxi(2,2))*
     & ust(m) )
        !    --- 2nd order 2D artificial diffusion ---
        ad2Coeff()=(ad21 + cd22* ( abs(ux2(uc))+abs(uy2(uc))  +abs(ux2(
     & vc))+abs(uy2(vc)) ) )
        ad2cCoeff()=(ad21 + cd22* ( abs(ux2c(uc))+abs(uy2c(uc))  +abs(
     & ux2c(vc))+abs(uy2c(vc)) ) )
        ad2nCoeff() =(ad21 + cd22*( abs(ux2(nc)) +abs(uy2(nc)) ) ) ! for eddy viscosity
        ad2cnCoeff()=(ad21 + cd22*( abs(ux2c(nc))+abs(uy2c(nc)) ) )
        ad2(c)=adc*(u(i1+1,i2,i3,c)-4.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c)  +
     & u(i1,i2+1,i3,c)                 +u(i1,i2-1,i3,c))
        !    --- 2nd order 3D artificial diffusion ---
        ad23Coeff()=(ad21 + cd22*   ( abs(ux2(uc))+abs(uy2(uc))+abs(
     & uz2(uc)) +abs(ux2(vc))+abs(uy2(vc))+abs(uz2(vc))  +abs(ux2(wc))
     & +abs(uy2(wc))+abs(uz2(wc)) ) )
        ad23cCoeff()=(ad21 + cd22*   ( abs(ux3c(uc))+abs(uy3c(uc))+abs(
     & uz3c(uc)) +abs(ux3c(vc))+abs(uy3c(vc))+abs(uz3c(vc))  +abs(
     & ux3c(wc))+abs(uy3c(wc))+abs(uz3c(wc)) ) )
        ad23nCoeff() =(ad21 + cd22*( abs(ux2(nc)) +abs(uy2(nc)) +abs(
     & uz2(nc)) ) ) ! for eddy viscosity
        ad23cnCoeff()=(ad21 + cd22*( abs(ux3c(nc))+abs(uy3c(nc))+abs(
     & uz3c(nc)) ) )
        ad23(c)=adc*(u(i1+1,i2,i3,c)-6.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c)  
     & +u(i1,i2+1,i3,c)                   +u(i1,i2-1,i3,c) +u(i1,i2,
     & i3+1,c)                   +u(i1,i2,i3-1,c))
        !     ---fourth-order artficial diffusion in 2D
        ad4Coeff()=(ad41 + cd42*    ( abs(ux2(uc))+abs(uy2(uc))    +
     & abs(ux2(vc))+abs(uy2(vc)) ) )
        ad4(c)=adc*(   -u(i1+2,i2,i3,c)-u(i1-2,i2,i3,c)    -u(i1,i2+2,
     & i3,c)-u(i1,i2-2,i3,c)    +4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)  
     &   +u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c))   -12.*u(i1,i2,i3,c) )
        !     ---fourth-order artficial diffusion in 3D
        ad43Coeff()=(ad41 + cd42*    ( abs(ux2(uc))+abs(uy2(uc))+abs(
     & uz2(uc))    +abs(ux2(vc))+abs(uy2(vc))+abs(uz2(vc))    +abs(
     & ux2(wc))+abs(uy2(wc))+abs(uz2(wc)) ) )
        ad43(c)=adc*(   -u(i1+2,i2,i3,c)-u(i1-2,i2,i3,c)   -u(i1,i2+2,
     & i3,c)-u(i1,i2-2,i3,c)   -u(i1,i2,i3+2,c)-u(i1,i2,i3-2,c)   +4.*
     & (u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)   +u(i1,i2+1,i3,c)+u(i1,i2-1,
     & i3,c)   +u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c))  -18.*u(i1,i2,i3,c) )
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


       ! statement functions to access coefficients of mixed-boundary conditions
        mixedRHS(c,side,axis,grid)         =bcData(c+
     & numberOfComponents*(0),side,axis,grid)
        mixedCoeff(c,side,axis,grid)       =bcData(c+
     & numberOfComponents*(1),side,axis,grid)
        mixedNormalCoeff(c,side,axis,grid) =bcData(c+
     & numberOfComponents*(2),side,axis,grid)
        !     --- end statement functions
        ierr=0
        ! write(*,*) 'Inside insLineSolve'
              pc                =ipar(0)
              uc                =ipar(1)
              vc                =ipar(2)
              wc                =ipar(3)
              grid              =ipar(4)
              orderOfAccuracy   =ipar(5)
              gridIsMoving      =ipar(6)
              useWhereMask      =ipar(7)
              gridIsImplicit    =ipar(8)
              implicitMethod    =ipar(9)
              implicitOption    =ipar(10)
              isAxisymmetric    =ipar(11)
              use2ndOrderAD     =ipar(12)
              use4thOrderAD     =ipar(13)
              gridType          =ipar(14)
              computeMatrix     =ipar(15)
              computeRHS        =ipar(16)
              computeMatrixBC   =ipar(17)
              fc                =ipar(18)
              fcu=fc
              fcv=fc+1
              fcw=fc+2
              fcn=fc+nd
              fct=fc+nd
              orderOfExtrapolation=ipar(19)
              ibc               = ipar(20)
              option            = ipar(21)
              nc                = ipar(22)
              turbulenceModel   = ipar(23)
              twilightZoneFlow  = ipar(24)
              useSelfAdjointDiffusion=ipar(25)
              fourthOrder       = ipar(26)
              pdeModel          = ipar(27)
              tc                = ipar(28)
              numberOfComponents= ipar(29)
              dx(0)            =rpar(0)
              dx(1)            =rpar(1)
              dx(2)            =rpar(2)
              nu                =rpar(3)
              ad21              =rpar(4)
              ad22              =rpar(5)
              ad41              =rpar(6)
              ad42              =rpar(7)
              dr(0)             =rpar(8)
              dr(1)             =rpar(9)
              dr(2)             =rpar(10)
              cfl               =rpar(11)
              ad21n             =rpar(12)
              ad22n             =rpar(13)
              ad41n             =rpar(14)
              ad42n             =rpar(15)
              kThermal          =rpar(16)
              thermalExpansivity=rpar(17)
              gravity(0)        =rpar(18)
              gravity(1)        =rpar(19)
              gravity(2)        =rpar(20)
              computeTemperature = 0
              if( pdeModel.eq.BoussinesqModel .or. 
     & pdeModel.eq.viscoPlasticModel )then
                computeTemperature=1
              else
                tc=uc ! give this default value to tc so we can always add a gravity term, even if there is no T equation
                thermalExpansivity=0.   ! set to zero to turn off the gravity term
              end if
              do m=0,2
               dxv2i(m)=1./(2.*dx(m))
               dxvsqi(m)=1./(dx(m)**2)
               drv2i(m)=1./(2.*dr(m))
               drvsqi(m)=1./(dr(m)**2)
              end do
              dx0=dx(0)
              dy=dx(1)
              dz=dx(2)
              dx2i=1./(2.*dx0)
              dy2i=1./(2.*dy)
              dz2i=1./(2.*dz)
              dxsqi=1./(dx0*dx0)
              dysqi=1./(dy*dy)
              dzsqi=1./(dz*dz)
              dr2i=1./(2.*dr(0))
              ds2i=1./(2.*dr(1))
              dt2i=1./(2.*dr(2))
              drsqi=1./(dr(0)**2)
              dssqi=1./(dr(1)**2)
              dtsqi=1./(dr(2)**2)
              dxi=1./dx0
              dyi=1./dy
              dzi=1./dz
              dri=1./dr(0)
              dsi=1./dr(1)
              dti=1./dr(2)
              if( orderOfAccuracy.eq.4 )then
                dx12i=1./(12.*dx0)
                dy12i=1./(12.*dy)
                dz12i=1./(12.*dz)
                dxsq12i=1./(12.*dx0**2)
                dysq12i=1./(12.*dy**2)
                dzsq12i=1./(12.*dz**2)
              end if
              cd22=ad22/(nd**2)
              cd42=ad42/(nd**2)
              cd22n=ad22n/nd     ! for the SA TM model
              cd42n=ad42n/nd
c      write(*,*) 'insLineSolve: use2ndOrderAD,ad21,cd22=',
c     & use2ndOrderAD,ad21,cd22
              dtScale=1./cfl
              if( fourthOrder.eq.1 .and. 
     & turbulenceModel.ne.noTurbulenceModel )then
                write(*,'("insLineSolve: ERROR: fourth-order only 
     & available for INS")')
                ! " '
                stop 6543
              end if
              if( turbulenceModel.eq.spalartAllmaras )then
                call getSpalartAllmarasParameters(cb1, cb2, cv1, sigma,
     &  sigmai, kappa, cw1, cw2, cw3, cw3e6, cv1e3, cd0, cr0)
              else if( turbulenceModel.eq.kEpsilon )then
c**        call getKEpsilonParameters( cMu,cEps1,cEps2,sigmaEpsI,sigmaKI )
              else if( turbulenceModel.ne.noTurbulenceModel )then
                stop 88
              end if
              if( turbulenceModel.eq.baldwinLomax )then
                 ! assign constants for baldwin-lomax
                 kbl=.4
                 alpha=.0168
                 a0p=26.
c         ccp=1.6
                 ccp=2.6619
                 ckleb=0.3
                 cwk=.25
c         cwk=1
              end if
              itrip = ipar(50)
              jtrip = ipar(51)
              ktrip = ipar(52)
        if ( option.eq.setupSweep ) then
           stop 825
        else if( option.eq.assignINS )then
         ! **************************************************************************
         ! Fill in the tridiagonal matrix for the momentum equations for the INS plus
         ! artificial dissipation and/or turbulence model
         ! ***************************************************************************
            if( gridType.eq.rectangular )then
              ! *******************************************
              ! ************** rectangular  ***************
              ! *******************************************
              if( orderOfAccuracy.eq.2 )then
                if( dir.eq.0 )then
c  ****** Incompressible NS, No turbulence model *****
                 if( nd.eq.2 )then
                   ! defineDerivativeMacros(DIM,ORDER,GRIDTYPE)
                   if( use2ndOrderAD.eq.0 .and. use4thOrderAD.eq.0 )
     & then
                     if( computeMatrix.eq.1 .and. computeRHS.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                        am(i1,i2,i3)=-uu(uc+0)*dxv2i(0)-nu*dxvsqi(0)
                        bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*nu*(
     & dxvsqi(0)+dxvsqi(1))
                        cm(i1,i2,i3)=uu(uc+0)*dxv2i(0)-nu*dxvsqi(0)







c rhs equations:
                        f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fr2d0(uc)-ux2(
     & pc)
                        f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fr2d0(vc)-uy2(
     & pc)

                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                       end if
                      end do
                      end do
                      end do
                     else if( computeMatrix.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                        am(i1,i2,i3)=-uu(uc+0)*dxv2i(0)-nu*dxvsqi(0)
                        bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*nu*(
     & dxvsqi(0)+dxvsqi(1))
                        cm(i1,i2,i3)=uu(uc+0)*dxv2i(0)-nu*dxvsqi(0)







                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                       end if
                      end do
                      end do
                      end do
                     else if( computeRHS.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c rhs equations:
                        f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fr2d0(uc)-ux2(
     & pc)
                        f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fr2d0(vc)-uy2(
     & pc)

                       else
c for interpolation points or unused:
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                       end if
                      end do
                      end do
                      end do
                     end if
                   else if( use2ndOrderAD.eq.1 .and. 
     & use4thOrderAD.eq.0 )then
                     if( computeMatrix.eq.1 .and. computeRHS.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                            cdmz=admzR(i1  ,i2  ,i3)
                            cdpz=admzR(i1+1,i2  ,i3)
                            cdzm=adzmR(i1  ,i2  ,i3)
                            cdzp=adzmR(i1  ,i2+1,i3)
                            ! write(*,'(1x,''insLS:i1,i2,cdmz,cdzm='',2i3,2f9.3)') i1,i2,cdmz,cdzm
                            ! cdmz=0.
                            ! cdpz=0.
                            ! cdzm=0.
                            ! cdzp=0.
                            cdDiag=cdmz+cdpz+cdzm+cdzp
                              cdm=cdmz
                              cdp=cdpz
                        am(i1,i2,i3)=-uu(uc+0)*dxv2i(0)-nu*dxvsqi(0)-
     & cdm
                        bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*nu*(
     & dxvsqi(0)+dxvsqi(1))+cdDiag
                        cm(i1,i2,i3)=uu(uc+0)*dxv2i(0)-nu*dxvsqi(0)-cdp






c rhs equations:
                        f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fr2d0(uc)-ux2(
     & pc)+adE0(i1,i2,i3,uc)
                        f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fr2d0(vc)-uy2(
     & pc)+adE0(i1,i2,i3,vc)

                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                       end if
                      end do
                      end do
                      end do
                     else if( computeMatrix.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                            cdmz=admzR(i1  ,i2  ,i3)
                            cdpz=admzR(i1+1,i2  ,i3)
                            cdzm=adzmR(i1  ,i2  ,i3)
                            cdzp=adzmR(i1  ,i2+1,i3)
                            ! write(*,'(1x,''insLS:i1,i2,cdmz,cdzm='',2i3,2f9.3)') i1,i2,cdmz,cdzm
                            ! cdmz=0.
                            ! cdpz=0.
                            ! cdzm=0.
                            ! cdzp=0.
                            cdDiag=cdmz+cdpz+cdzm+cdzp
                              cdm=cdmz
                              cdp=cdpz
                        am(i1,i2,i3)=-uu(uc+0)*dxv2i(0)-nu*dxvsqi(0)-
     & cdm
                        bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*nu*(
     & dxvsqi(0)+dxvsqi(1))+cdDiag
                        cm(i1,i2,i3)=uu(uc+0)*dxv2i(0)-nu*dxvsqi(0)-cdp






                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                       end if
                      end do
                      end do
                      end do
                     else if( computeRHS.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c rhs equations:
                        f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fr2d0(uc)-ux2(
     & pc)+adE0(i1,i2,i3,uc)
                        f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fr2d0(vc)-uy2(
     & pc)+adE0(i1,i2,i3,vc)

                       else
c for interpolation points or unused:
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                       end if
                      end do
                      end do
                      end do
                     end if
                   else if( use4thOrderAD.eq.1 )then
                      ! By default there is no AD:
                      ! 2nd-order artificial dissipation
                      ! 4th-order artficial dissipation  **todo** implicit-line, self-adjoint versions
                      ! Both 2nd and 4th order dissipation
                     if( computeMatrix.eq.1 .and. computeRHS.eq.1 )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                                adCoeff2 = ad21+cd22*( abs(ux22r(i1,i2,
     & i3,uc))+abs(uy22r(i1,i2,i3,uc))+abs(ux22r(i1,i2,i3,vc))+abs(
     & uy22r(i1,i2,i3,vc)) )
                            ! Artficial Dissipation  **todo** implicit-line, self-adjoint versions
                                adCoeff4 = ad41+cd42*( abs(ux22r(i1,i2,
     & i3,uc))+abs(uy22r(i1,i2,i3,uc))+abs(ux22r(i1,i2,i3,vc))+abs(
     & uy22r(i1,i2,i3,vc)) )
                           am(i1,i2,i3)=adCoeff4
                           bm(i1,i2,i3)=-uu(uc+0)*dxv2i(0)-nu*dxvsqi(0)
     & -adCoeff2-4.*adCoeff4
                           cm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*nu*(
     & dxvsqi(0)+dxvsqi(1))+4.*adCoeff2+12.*adCoeff4
                           dm(i1,i2,i3)=uu(uc+0)*dxv2i(0)-nu*dxvsqi(0)-
     & adCoeff2-4.*adCoeff4
                           em(i1,i2,i3)=adCoeff4




c rhs equations:
                           f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fr2d0(uc)-
     & ux2(pc)+adCoeff2*(u(i1,i2-1,i3,uc)+u(i1,i2+1,i3,uc))+adCoeff4*(
     & -u(i1,i2-2,i3,uc)-u(i1,i2+2,i3,uc)+4.*(u(i1,i2-1,i3,uc)+u(i1,
     & i2+1,i3,uc)))
                           f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fr2d0(vc)-
     & uy2(pc)+adCoeff2*(u(i1,i2-1,i3,vc)+u(i1,i2+1,i3,vc))+adCoeff4*(
     & -u(i1,i2-2,i3,vc)-u(i1,i2+2,i3,vc)+4.*(u(i1,i2-1,i3,vc)+u(i1,
     & i2+1,i3,vc)))

                         else
c for interpolation points or unused:
                           am(i1,i2,i3)=0.
                           bm(i1,i2,i3)=0.
                           cm(i1,i2,i3)=1.
                           dm(i1,i2,i3)=0.
                           em(i1,i2,i3)=0.
                           f(i1,i2,i3,fcu)=uu(uc)
                           f(i1,i2,i3,fcv)=uu(vc)
                         end if
                       end do
                       end do
                       end do
                     else if( computeMatrix.eq.1 )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                                adCoeff2 = ad21+cd22*( abs(ux22r(i1,i2,
     & i3,uc))+abs(uy22r(i1,i2,i3,uc))+abs(ux22r(i1,i2,i3,vc))+abs(
     & uy22r(i1,i2,i3,vc)) )
                            ! Artficial Dissipation  **todo** implicit-line, self-adjoint versions
                                adCoeff4 = ad41+cd42*( abs(ux22r(i1,i2,
     & i3,uc))+abs(uy22r(i1,i2,i3,uc))+abs(ux22r(i1,i2,i3,vc))+abs(
     & uy22r(i1,i2,i3,vc)) )
                           am(i1,i2,i3)=adCoeff4
                           bm(i1,i2,i3)=-uu(uc+0)*dxv2i(0)-nu*dxvsqi(0)
     & -adCoeff2-4.*adCoeff4
                           cm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*nu*(
     & dxvsqi(0)+dxvsqi(1))+4.*adCoeff2+12.*adCoeff4
                           dm(i1,i2,i3)=uu(uc+0)*dxv2i(0)-nu*dxvsqi(0)-
     & adCoeff2-4.*adCoeff4
                           em(i1,i2,i3)=adCoeff4




                         else
c for interpolation points or unused:
                           am(i1,i2,i3)=0.
                           bm(i1,i2,i3)=0.
                           cm(i1,i2,i3)=1.
                           dm(i1,i2,i3)=0.
                           em(i1,i2,i3)=0.
                         end if
                       end do
                       end do
                       end do
                     else if( computeRHS.eq.1 )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
c rhs equations:
                           f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fr2d0(uc)-
     & ux2(pc)+adCoeff2*(u(i1,i2-1,i3,uc)+u(i1,i2+1,i3,uc))+adCoeff4*(
     & -u(i1,i2-2,i3,uc)-u(i1,i2+2,i3,uc)+4.*(u(i1,i2-1,i3,uc)+u(i1,
     & i2+1,i3,uc)))
                           f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fr2d0(vc)-
     & uy2(pc)+adCoeff2*(u(i1,i2-1,i3,vc)+u(i1,i2+1,i3,vc))+adCoeff4*(
     & -u(i1,i2-2,i3,vc)-u(i1,i2+2,i3,vc)+4.*(u(i1,i2-1,i3,vc)+u(i1,
     & i2+1,i3,vc)))

                         else
c for interpolation points or unused:
                           f(i1,i2,i3,fcu)=uu(uc)
                           f(i1,i2,i3,fcv)=uu(vc)
                         end if
                       end do
                       end do
                       end do
                     end if
                   else
                     stop 111
                   end if
                 else if( nd.eq.3 )then
                   ! defineDerivativeMacros(DIM,ORDER,GRIDTYPE)
                   if( use2ndOrderAD.eq.0 .and. use4thOrderAD.eq.0 )
     & then
                     if( computeMatrix.eq.1 .and. computeRHS.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                        am(i1,i2,i3)=-uu(uc+0)*dxv2i(0)-nu*dxvsqi(0)
                        bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*nu*(
     & dxvsqi(0)+dxvsqi(1)+dxvsqi(2))
                        cm(i1,i2,i3)=uu(uc+0)*dxv2i(0)-nu*dxvsqi(0)







c rhs equations:
                        f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fr3d0(uc)-ux2(
     & pc)
                        f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fr3d0(vc)-uy2(
     & pc)
                        f(i1,i2,i3,fcw)=f(i1,i2,i3,fcw)+fr3d0(wc)-uz2(
     & pc)
                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                        f(i1,i2,i3,fcw)=uu(wc)
                       end if
                      end do
                      end do
                      end do
                     else if( computeMatrix.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                        am(i1,i2,i3)=-uu(uc+0)*dxv2i(0)-nu*dxvsqi(0)
                        bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*nu*(
     & dxvsqi(0)+dxvsqi(1)+dxvsqi(2))
                        cm(i1,i2,i3)=uu(uc+0)*dxv2i(0)-nu*dxvsqi(0)







                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                       end if
                      end do
                      end do
                      end do
                     else if( computeRHS.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c rhs equations:
                        f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fr3d0(uc)-ux2(
     & pc)
                        f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fr3d0(vc)-uy2(
     & pc)
                        f(i1,i2,i3,fcw)=f(i1,i2,i3,fcw)+fr3d0(wc)-uz2(
     & pc)
                       else
c for interpolation points or unused:
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                        f(i1,i2,i3,fcw)=uu(wc)
                       end if
                      end do
                      end do
                      end do
                     end if
                   else if( use2ndOrderAD.eq.1 .and. 
     & use4thOrderAD.eq.0 )then
                     if( computeMatrix.eq.1 .and. computeRHS.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                            cdmzz=admzzR(i1  ,i2  ,i3  )
                            cdpzz=admzzR(i1+1,i2  ,i3  )
                            cdzmz=adzmzR(i1  ,i2  ,i3  )
                            cdzpz=adzmzR(i1  ,i2+1,i3  )
                            cdzzm=adzzmR(i1  ,i2  ,i3  )
                            cdzzp=adzzmR(i1  ,i2  ,i3+1)
                            cdDiag=cdmzz+cdpzz+cdzmz+cdzpz+cdzzm+cdzzp
                              cdm=cdmzz
                              cdp=cdpzz
                        am(i1,i2,i3)=-uu(uc+0)*dxv2i(0)-nu*dxvsqi(0)-
     & cdm
                        bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*nu*(
     & dxvsqi(0)+dxvsqi(1)+dxvsqi(2))+cdDiag
                        cm(i1,i2,i3)=uu(uc+0)*dxv2i(0)-nu*dxvsqi(0)-cdp






c rhs equations:
                        f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fr3d0(uc)-ux2(
     & pc)+adE3d0(i1,i2,i3,uc)
                        f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fr3d0(vc)-uy2(
     & pc)+adE3d0(i1,i2,i3,vc)
                        f(i1,i2,i3,fcw)=f(i1,i2,i3,fcw)+fr3d0(wc)-uz2(
     & pc)+adE3d0(i1,i2,i3,wc)
                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                        f(i1,i2,i3,fcw)=uu(wc)
                       end if
                      end do
                      end do
                      end do
                     else if( computeMatrix.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                            cdmzz=admzzR(i1  ,i2  ,i3  )
                            cdpzz=admzzR(i1+1,i2  ,i3  )
                            cdzmz=adzmzR(i1  ,i2  ,i3  )
                            cdzpz=adzmzR(i1  ,i2+1,i3  )
                            cdzzm=adzzmR(i1  ,i2  ,i3  )
                            cdzzp=adzzmR(i1  ,i2  ,i3+1)
                            cdDiag=cdmzz+cdpzz+cdzmz+cdzpz+cdzzm+cdzzp
                              cdm=cdmzz
                              cdp=cdpzz
                        am(i1,i2,i3)=-uu(uc+0)*dxv2i(0)-nu*dxvsqi(0)-
     & cdm
                        bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*nu*(
     & dxvsqi(0)+dxvsqi(1)+dxvsqi(2))+cdDiag
                        cm(i1,i2,i3)=uu(uc+0)*dxv2i(0)-nu*dxvsqi(0)-cdp






                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                       end if
                      end do
                      end do
                      end do
                     else if( computeRHS.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c rhs equations:
                        f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fr3d0(uc)-ux2(
     & pc)+adE3d0(i1,i2,i3,uc)
                        f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fr3d0(vc)-uy2(
     & pc)+adE3d0(i1,i2,i3,vc)
                        f(i1,i2,i3,fcw)=f(i1,i2,i3,fcw)+fr3d0(wc)-uz2(
     & pc)+adE3d0(i1,i2,i3,wc)
                       else
c for interpolation points or unused:
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                        f(i1,i2,i3,fcw)=uu(wc)
                       end if
                      end do
                      end do
                      end do
                     end if
                   else if( use4thOrderAD.eq.1 )then
                      ! By default there is no AD:
                      ! 2nd-order artificial dissipation
                      ! 4th-order artficial dissipation  **todo** implicit-line, self-adjoint versions
                      ! Both 2nd and 4th order dissipation
                     if( computeMatrix.eq.1 .and. computeRHS.eq.1 )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                                adCoeff2 = ad21+cd22*( abs(ux23r(i1,i2,
     & i3,uc))+abs(uy23r(i1,i2,i3,uc))+abs(uz23r(i1,i2,i3,uc))+abs(
     & ux23r(i1,i2,i3,vc))+abs(uy23r(i1,i2,i3,vc))+abs(uz23r(i1,i2,i3,
     & vc))+abs(ux23r(i1,i2,i3,wc))+abs(uy23r(i1,i2,i3,wc))+abs(uz23r(
     & i1,i2,i3,wc)))
                            ! Artficial Dissipation  **todo** implicit-line, self-adjoint versions
                                adCoeff4 = ad41+cd42*( abs(ux23r(i1,i2,
     & i3,uc))+abs(uy23r(i1,i2,i3,uc))+abs(uz23r(i1,i2,i3,uc))+abs(
     & ux23r(i1,i2,i3,vc))+abs(uy23r(i1,i2,i3,vc))+abs(uz23r(i1,i2,i3,
     & vc))+abs(ux23r(i1,i2,i3,wc))+abs(uy23r(i1,i2,i3,wc))+abs(uz23r(
     & i1,i2,i3,wc)))
                           am(i1,i2,i3)=adCoeff4
                           bm(i1,i2,i3)=-uu(uc+0)*dxv2i(0)-nu*dxvsqi(0)
     & -adCoeff2-4.*adCoeff4
                           cm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*nu*(
     & dxvsqi(0)+dxvsqi(1)+dxvsqi(2))+6.*adCoeff2+18.*adCoeff4
                           dm(i1,i2,i3)=uu(uc+0)*dxv2i(0)-nu*dxvsqi(0)-
     & adCoeff2-4.*adCoeff4
                           em(i1,i2,i3)=adCoeff4




c rhs equations:
                           f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fr3d0(uc)-
     & ux2(pc)+adCoeff2*(u(i1,i2-1,i3,uc)+u(i1,i2+1,i3,uc)+u(i1,i2,i3-
     & 1,uc)+u(i1,i2,i3+1,uc))+adCoeff4*(-u(i1,i2-2,i3,uc)-u(i1,i2+2,
     & i3,uc)-u(i1,i2,i3-2,uc)-u(i1,i2,i3+2,uc)+4.*(u(i1,i2-1,i3,uc)+
     & u(i1,i2+1,i3,uc)+u(i1,i2,i3-1,uc)+u(i1,i2,i3+1,uc)))
                           f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fr3d0(vc)-
     & uy2(pc)+adCoeff2*(u(i1,i2-1,i3,vc)+u(i1,i2+1,i3,vc)+u(i1,i2,i3-
     & 1,vc)+u(i1,i2,i3+1,vc))+adCoeff4*(-u(i1,i2-2,i3,vc)-u(i1,i2+2,
     & i3,vc)-u(i1,i2,i3-2,vc)-u(i1,i2,i3+2,vc)+4.*(u(i1,i2-1,i3,vc)+
     & u(i1,i2+1,i3,vc)+u(i1,i2,i3-1,vc)+u(i1,i2,i3+1,vc)))
                           f(i1,i2,i3,fcw)=f(i1,i2,i3,fcw)+fr3d0(wc)-
     & uz2(pc)+adCoeff2*(u(i1,i2-1,i3,wc)+u(i1,i2+1,i3,wc)+u(i1,i2,i3-
     & 1,wc)+u(i1,i2,i3+1,wc))+adCoeff4*(-u(i1,i2-2,i3,wc)-u(i1,i2+2,
     & i3,wc)-u(i1,i2,i3-2,wc)-u(i1,i2,i3+2,wc)+4.*(u(i1,i2-1,i3,wc)+
     & u(i1,i2+1,i3,wc)+u(i1,i2,i3-1,wc)+u(i1,i2,i3+1,wc)))
                         else
c for interpolation points or unused:
                           am(i1,i2,i3)=0.
                           bm(i1,i2,i3)=0.
                           cm(i1,i2,i3)=1.
                           dm(i1,i2,i3)=0.
                           em(i1,i2,i3)=0.
                           f(i1,i2,i3,fcu)=uu(uc)
                           f(i1,i2,i3,fcv)=uu(vc)
                           f(i1,i2,i3,fcw)=uu(wc)
                         end if
                       end do
                       end do
                       end do
                     else if( computeMatrix.eq.1 )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                                adCoeff2 = ad21+cd22*( abs(ux23r(i1,i2,
     & i3,uc))+abs(uy23r(i1,i2,i3,uc))+abs(uz23r(i1,i2,i3,uc))+abs(
     & ux23r(i1,i2,i3,vc))+abs(uy23r(i1,i2,i3,vc))+abs(uz23r(i1,i2,i3,
     & vc))+abs(ux23r(i1,i2,i3,wc))+abs(uy23r(i1,i2,i3,wc))+abs(uz23r(
     & i1,i2,i3,wc)))
                            ! Artficial Dissipation  **todo** implicit-line, self-adjoint versions
                                adCoeff4 = ad41+cd42*( abs(ux23r(i1,i2,
     & i3,uc))+abs(uy23r(i1,i2,i3,uc))+abs(uz23r(i1,i2,i3,uc))+abs(
     & ux23r(i1,i2,i3,vc))+abs(uy23r(i1,i2,i3,vc))+abs(uz23r(i1,i2,i3,
     & vc))+abs(ux23r(i1,i2,i3,wc))+abs(uy23r(i1,i2,i3,wc))+abs(uz23r(
     & i1,i2,i3,wc)))
                           am(i1,i2,i3)=adCoeff4
                           bm(i1,i2,i3)=-uu(uc+0)*dxv2i(0)-nu*dxvsqi(0)
     & -adCoeff2-4.*adCoeff4
                           cm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*nu*(
     & dxvsqi(0)+dxvsqi(1)+dxvsqi(2))+6.*adCoeff2+18.*adCoeff4
                           dm(i1,i2,i3)=uu(uc+0)*dxv2i(0)-nu*dxvsqi(0)-
     & adCoeff2-4.*adCoeff4
                           em(i1,i2,i3)=adCoeff4




                         else
c for interpolation points or unused:
                           am(i1,i2,i3)=0.
                           bm(i1,i2,i3)=0.
                           cm(i1,i2,i3)=1.
                           dm(i1,i2,i3)=0.
                           em(i1,i2,i3)=0.
                         end if
                       end do
                       end do
                       end do
                     else if( computeRHS.eq.1 )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
c rhs equations:
                           f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fr3d0(uc)-
     & ux2(pc)+adCoeff2*(u(i1,i2-1,i3,uc)+u(i1,i2+1,i3,uc)+u(i1,i2,i3-
     & 1,uc)+u(i1,i2,i3+1,uc))+adCoeff4*(-u(i1,i2-2,i3,uc)-u(i1,i2+2,
     & i3,uc)-u(i1,i2,i3-2,uc)-u(i1,i2,i3+2,uc)+4.*(u(i1,i2-1,i3,uc)+
     & u(i1,i2+1,i3,uc)+u(i1,i2,i3-1,uc)+u(i1,i2,i3+1,uc)))
                           f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fr3d0(vc)-
     & uy2(pc)+adCoeff2*(u(i1,i2-1,i3,vc)+u(i1,i2+1,i3,vc)+u(i1,i2,i3-
     & 1,vc)+u(i1,i2,i3+1,vc))+adCoeff4*(-u(i1,i2-2,i3,vc)-u(i1,i2+2,
     & i3,vc)-u(i1,i2,i3-2,vc)-u(i1,i2,i3+2,vc)+4.*(u(i1,i2-1,i3,vc)+
     & u(i1,i2+1,i3,vc)+u(i1,i2,i3-1,vc)+u(i1,i2,i3+1,vc)))
                           f(i1,i2,i3,fcw)=f(i1,i2,i3,fcw)+fr3d0(wc)-
     & uz2(pc)+adCoeff2*(u(i1,i2-1,i3,wc)+u(i1,i2+1,i3,wc)+u(i1,i2,i3-
     & 1,wc)+u(i1,i2,i3+1,wc))+adCoeff4*(-u(i1,i2-2,i3,wc)-u(i1,i2+2,
     & i3,wc)-u(i1,i2,i3-2,wc)-u(i1,i2,i3+2,wc)+4.*(u(i1,i2-1,i3,wc)+
     & u(i1,i2+1,i3,wc)+u(i1,i2,i3-1,wc)+u(i1,i2,i3+1,wc)))
                         else
c for interpolation points or unused:
                           f(i1,i2,i3,fcu)=uu(uc)
                           f(i1,i2,i3,fcv)=uu(vc)
                           f(i1,i2,i3,fcw)=uu(wc)
                         end if
                       end do
                       end do
                       end do
                     end if
                   else
                     stop 111
                   end if
                 else
                   stop 888
                 end if
                else if( dir.eq.1 )then
c  ****** Incompressible NS, No turbulence model *****
                 if( nd.eq.2 )then
                   ! defineDerivativeMacros(DIM,ORDER,GRIDTYPE)
                   if( use2ndOrderAD.eq.0 .and. use4thOrderAD.eq.0 )
     & then
                     if( computeMatrix.eq.1 .and. computeRHS.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                        am(i1,i2,i3)=-uu(uc+1)*dxv2i(1)-nu*dxvsqi(1)
                        bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*nu*(
     & dxvsqi(0)+dxvsqi(1))
                        cm(i1,i2,i3)=uu(uc+1)*dxv2i(1)-nu*dxvsqi(1)







c rhs equations:
                        f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fr2d1(uc)-ux2(
     & pc)
                        f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fr2d1(vc)-uy2(
     & pc)

                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                       end if
                      end do
                      end do
                      end do
                     else if( computeMatrix.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                        am(i1,i2,i3)=-uu(uc+1)*dxv2i(1)-nu*dxvsqi(1)
                        bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*nu*(
     & dxvsqi(0)+dxvsqi(1))
                        cm(i1,i2,i3)=uu(uc+1)*dxv2i(1)-nu*dxvsqi(1)







                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                       end if
                      end do
                      end do
                      end do
                     else if( computeRHS.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c rhs equations:
                        f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fr2d1(uc)-ux2(
     & pc)
                        f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fr2d1(vc)-uy2(
     & pc)

                       else
c for interpolation points or unused:
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                       end if
                      end do
                      end do
                      end do
                     end if
                   else if( use2ndOrderAD.eq.1 .and. 
     & use4thOrderAD.eq.0 )then
                     if( computeMatrix.eq.1 .and. computeRHS.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                            cdmz=admzR(i1  ,i2  ,i3)
                            cdpz=admzR(i1+1,i2  ,i3)
                            cdzm=adzmR(i1  ,i2  ,i3)
                            cdzp=adzmR(i1  ,i2+1,i3)
                            ! write(*,'(1x,''insLS:i1,i2,cdmz,cdzm='',2i3,2f9.3)') i1,i2,cdmz,cdzm
                            ! cdmz=0.
                            ! cdpz=0.
                            ! cdzm=0.
                            ! cdzp=0.
                            cdDiag=cdmz+cdpz+cdzm+cdzp
                              cdm=cdzm
                              cdp=cdzp
                        am(i1,i2,i3)=-uu(uc+1)*dxv2i(1)-nu*dxvsqi(1)-
     & cdm
                        bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*nu*(
     & dxvsqi(0)+dxvsqi(1))+cdDiag
                        cm(i1,i2,i3)=uu(uc+1)*dxv2i(1)-nu*dxvsqi(1)-cdp






c rhs equations:
                        f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fr2d1(uc)-ux2(
     & pc)+adE1(i1,i2,i3,uc)
                        f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fr2d1(vc)-uy2(
     & pc)+adE1(i1,i2,i3,vc)

                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                       end if
                      end do
                      end do
                      end do
                     else if( computeMatrix.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                            cdmz=admzR(i1  ,i2  ,i3)
                            cdpz=admzR(i1+1,i2  ,i3)
                            cdzm=adzmR(i1  ,i2  ,i3)
                            cdzp=adzmR(i1  ,i2+1,i3)
                            ! write(*,'(1x,''insLS:i1,i2,cdmz,cdzm='',2i3,2f9.3)') i1,i2,cdmz,cdzm
                            ! cdmz=0.
                            ! cdpz=0.
                            ! cdzm=0.
                            ! cdzp=0.
                            cdDiag=cdmz+cdpz+cdzm+cdzp
                              cdm=cdzm
                              cdp=cdzp
                        am(i1,i2,i3)=-uu(uc+1)*dxv2i(1)-nu*dxvsqi(1)-
     & cdm
                        bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*nu*(
     & dxvsqi(0)+dxvsqi(1))+cdDiag
                        cm(i1,i2,i3)=uu(uc+1)*dxv2i(1)-nu*dxvsqi(1)-cdp






                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                       end if
                      end do
                      end do
                      end do
                     else if( computeRHS.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c rhs equations:
                        f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fr2d1(uc)-ux2(
     & pc)+adE1(i1,i2,i3,uc)
                        f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fr2d1(vc)-uy2(
     & pc)+adE1(i1,i2,i3,vc)

                       else
c for interpolation points or unused:
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                       end if
                      end do
                      end do
                      end do
                     end if
                   else if( use4thOrderAD.eq.1 )then
                      ! By default there is no AD:
                      ! 2nd-order artificial dissipation
                      ! 4th-order artficial dissipation  **todo** implicit-line, self-adjoint versions
                      ! Both 2nd and 4th order dissipation
                     if( computeMatrix.eq.1 .and. computeRHS.eq.1 )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                                adCoeff2 = ad21+cd22*( abs(ux22r(i1,i2,
     & i3,uc))+abs(uy22r(i1,i2,i3,uc))+abs(ux22r(i1,i2,i3,vc))+abs(
     & uy22r(i1,i2,i3,vc)) )
                            ! Artficial Dissipation  **todo** implicit-line, self-adjoint versions
                                adCoeff4 = ad41+cd42*( abs(ux22r(i1,i2,
     & i3,uc))+abs(uy22r(i1,i2,i3,uc))+abs(ux22r(i1,i2,i3,vc))+abs(
     & uy22r(i1,i2,i3,vc)) )
                           am(i1,i2,i3)=adCoeff4
                           bm(i1,i2,i3)=-uu(uc+1)*dxv2i(1)-nu*dxvsqi(1)
     & -adCoeff2-4.*adCoeff4
                           cm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*nu*(
     & dxvsqi(0)+dxvsqi(1))+4.*adCoeff2+12.*adCoeff4
                           dm(i1,i2,i3)=uu(uc+1)*dxv2i(1)-nu*dxvsqi(1)-
     & adCoeff2-4.*adCoeff4
                           em(i1,i2,i3)=adCoeff4




c rhs equations:
                           f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fr2d1(uc)-
     & ux2(pc)+adCoeff2*(u(i1-1,i2,i3,uc)+u(i1+1,i2,i3,uc))+adCoeff4*(
     & -u(i1-2,i2,i3,uc)-u(i1+2,i2,i3,uc)+4.*(u(i1-1,i2,i3,uc)+u(i1+1,
     & i2,i3,uc)))
                           f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fr2d1(vc)-
     & uy2(pc)+adCoeff2*(u(i1-1,i2,i3,vc)+u(i1+1,i2,i3,vc))+adCoeff4*(
     & -u(i1-2,i2,i3,vc)-u(i1+2,i2,i3,vc)+4.*(u(i1-1,i2,i3,vc)+u(i1+1,
     & i2,i3,vc)))

                         else
c for interpolation points or unused:
                           am(i1,i2,i3)=0.
                           bm(i1,i2,i3)=0.
                           cm(i1,i2,i3)=1.
                           dm(i1,i2,i3)=0.
                           em(i1,i2,i3)=0.
                           f(i1,i2,i3,fcu)=uu(uc)
                           f(i1,i2,i3,fcv)=uu(vc)
                         end if
                       end do
                       end do
                       end do
                     else if( computeMatrix.eq.1 )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                                adCoeff2 = ad21+cd22*( abs(ux22r(i1,i2,
     & i3,uc))+abs(uy22r(i1,i2,i3,uc))+abs(ux22r(i1,i2,i3,vc))+abs(
     & uy22r(i1,i2,i3,vc)) )
                            ! Artficial Dissipation  **todo** implicit-line, self-adjoint versions
                                adCoeff4 = ad41+cd42*( abs(ux22r(i1,i2,
     & i3,uc))+abs(uy22r(i1,i2,i3,uc))+abs(ux22r(i1,i2,i3,vc))+abs(
     & uy22r(i1,i2,i3,vc)) )
                           am(i1,i2,i3)=adCoeff4
                           bm(i1,i2,i3)=-uu(uc+1)*dxv2i(1)-nu*dxvsqi(1)
     & -adCoeff2-4.*adCoeff4
                           cm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*nu*(
     & dxvsqi(0)+dxvsqi(1))+4.*adCoeff2+12.*adCoeff4
                           dm(i1,i2,i3)=uu(uc+1)*dxv2i(1)-nu*dxvsqi(1)-
     & adCoeff2-4.*adCoeff4
                           em(i1,i2,i3)=adCoeff4




                         else
c for interpolation points or unused:
                           am(i1,i2,i3)=0.
                           bm(i1,i2,i3)=0.
                           cm(i1,i2,i3)=1.
                           dm(i1,i2,i3)=0.
                           em(i1,i2,i3)=0.
                         end if
                       end do
                       end do
                       end do
                     else if( computeRHS.eq.1 )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
c rhs equations:
                           f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fr2d1(uc)-
     & ux2(pc)+adCoeff2*(u(i1-1,i2,i3,uc)+u(i1+1,i2,i3,uc))+adCoeff4*(
     & -u(i1-2,i2,i3,uc)-u(i1+2,i2,i3,uc)+4.*(u(i1-1,i2,i3,uc)+u(i1+1,
     & i2,i3,uc)))
                           f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fr2d1(vc)-
     & uy2(pc)+adCoeff2*(u(i1-1,i2,i3,vc)+u(i1+1,i2,i3,vc))+adCoeff4*(
     & -u(i1-2,i2,i3,vc)-u(i1+2,i2,i3,vc)+4.*(u(i1-1,i2,i3,vc)+u(i1+1,
     & i2,i3,vc)))

                         else
c for interpolation points or unused:
                           f(i1,i2,i3,fcu)=uu(uc)
                           f(i1,i2,i3,fcv)=uu(vc)
                         end if
                       end do
                       end do
                       end do
                     end if
                   else
                     stop 111
                   end if
                 else if( nd.eq.3 )then
                   ! defineDerivativeMacros(DIM,ORDER,GRIDTYPE)
                   if( use2ndOrderAD.eq.0 .and. use4thOrderAD.eq.0 )
     & then
                     if( computeMatrix.eq.1 .and. computeRHS.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                        am(i1,i2,i3)=-uu(uc+1)*dxv2i(1)-nu*dxvsqi(1)
                        bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*nu*(
     & dxvsqi(0)+dxvsqi(1)+dxvsqi(2))
                        cm(i1,i2,i3)=uu(uc+1)*dxv2i(1)-nu*dxvsqi(1)







c rhs equations:
                        f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fr3d1(uc)-ux2(
     & pc)
                        f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fr3d1(vc)-uy2(
     & pc)
                        f(i1,i2,i3,fcw)=f(i1,i2,i3,fcw)+fr3d1(wc)-uz2(
     & pc)
                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                        f(i1,i2,i3,fcw)=uu(wc)
                       end if
                      end do
                      end do
                      end do
                     else if( computeMatrix.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                        am(i1,i2,i3)=-uu(uc+1)*dxv2i(1)-nu*dxvsqi(1)
                        bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*nu*(
     & dxvsqi(0)+dxvsqi(1)+dxvsqi(2))
                        cm(i1,i2,i3)=uu(uc+1)*dxv2i(1)-nu*dxvsqi(1)







                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                       end if
                      end do
                      end do
                      end do
                     else if( computeRHS.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c rhs equations:
                        f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fr3d1(uc)-ux2(
     & pc)
                        f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fr3d1(vc)-uy2(
     & pc)
                        f(i1,i2,i3,fcw)=f(i1,i2,i3,fcw)+fr3d1(wc)-uz2(
     & pc)
                       else
c for interpolation points or unused:
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                        f(i1,i2,i3,fcw)=uu(wc)
                       end if
                      end do
                      end do
                      end do
                     end if
                   else if( use2ndOrderAD.eq.1 .and. 
     & use4thOrderAD.eq.0 )then
                     if( computeMatrix.eq.1 .and. computeRHS.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                            cdmzz=admzzR(i1  ,i2  ,i3  )
                            cdpzz=admzzR(i1+1,i2  ,i3  )
                            cdzmz=adzmzR(i1  ,i2  ,i3  )
                            cdzpz=adzmzR(i1  ,i2+1,i3  )
                            cdzzm=adzzmR(i1  ,i2  ,i3  )
                            cdzzp=adzzmR(i1  ,i2  ,i3+1)
                            cdDiag=cdmzz+cdpzz+cdzmz+cdzpz+cdzzm+cdzzp
                              cdm=cdzmz
                              cdp=cdzpz
                        am(i1,i2,i3)=-uu(uc+1)*dxv2i(1)-nu*dxvsqi(1)-
     & cdm
                        bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*nu*(
     & dxvsqi(0)+dxvsqi(1)+dxvsqi(2))+cdDiag
                        cm(i1,i2,i3)=uu(uc+1)*dxv2i(1)-nu*dxvsqi(1)-cdp






c rhs equations:
                        f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fr3d1(uc)-ux2(
     & pc)+adE3d1(i1,i2,i3,uc)
                        f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fr3d1(vc)-uy2(
     & pc)+adE3d1(i1,i2,i3,vc)
                        f(i1,i2,i3,fcw)=f(i1,i2,i3,fcw)+fr3d1(wc)-uz2(
     & pc)+adE3d1(i1,i2,i3,wc)
                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                        f(i1,i2,i3,fcw)=uu(wc)
                       end if
                      end do
                      end do
                      end do
                     else if( computeMatrix.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                            cdmzz=admzzR(i1  ,i2  ,i3  )
                            cdpzz=admzzR(i1+1,i2  ,i3  )
                            cdzmz=adzmzR(i1  ,i2  ,i3  )
                            cdzpz=adzmzR(i1  ,i2+1,i3  )
                            cdzzm=adzzmR(i1  ,i2  ,i3  )
                            cdzzp=adzzmR(i1  ,i2  ,i3+1)
                            cdDiag=cdmzz+cdpzz+cdzmz+cdzpz+cdzzm+cdzzp
                              cdm=cdzmz
                              cdp=cdzpz
                        am(i1,i2,i3)=-uu(uc+1)*dxv2i(1)-nu*dxvsqi(1)-
     & cdm
                        bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*nu*(
     & dxvsqi(0)+dxvsqi(1)+dxvsqi(2))+cdDiag
                        cm(i1,i2,i3)=uu(uc+1)*dxv2i(1)-nu*dxvsqi(1)-cdp






                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                       end if
                      end do
                      end do
                      end do
                     else if( computeRHS.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c rhs equations:
                        f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fr3d1(uc)-ux2(
     & pc)+adE3d1(i1,i2,i3,uc)
                        f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fr3d1(vc)-uy2(
     & pc)+adE3d1(i1,i2,i3,vc)
                        f(i1,i2,i3,fcw)=f(i1,i2,i3,fcw)+fr3d1(wc)-uz2(
     & pc)+adE3d1(i1,i2,i3,wc)
                       else
c for interpolation points or unused:
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                        f(i1,i2,i3,fcw)=uu(wc)
                       end if
                      end do
                      end do
                      end do
                     end if
                   else if( use4thOrderAD.eq.1 )then
                      ! By default there is no AD:
                      ! 2nd-order artificial dissipation
                      ! 4th-order artficial dissipation  **todo** implicit-line, self-adjoint versions
                      ! Both 2nd and 4th order dissipation
                     if( computeMatrix.eq.1 .and. computeRHS.eq.1 )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                                adCoeff2 = ad21+cd22*( abs(ux23r(i1,i2,
     & i3,uc))+abs(uy23r(i1,i2,i3,uc))+abs(uz23r(i1,i2,i3,uc))+abs(
     & ux23r(i1,i2,i3,vc))+abs(uy23r(i1,i2,i3,vc))+abs(uz23r(i1,i2,i3,
     & vc))+abs(ux23r(i1,i2,i3,wc))+abs(uy23r(i1,i2,i3,wc))+abs(uz23r(
     & i1,i2,i3,wc)))
                            ! Artficial Dissipation  **todo** implicit-line, self-adjoint versions
                                adCoeff4 = ad41+cd42*( abs(ux23r(i1,i2,
     & i3,uc))+abs(uy23r(i1,i2,i3,uc))+abs(uz23r(i1,i2,i3,uc))+abs(
     & ux23r(i1,i2,i3,vc))+abs(uy23r(i1,i2,i3,vc))+abs(uz23r(i1,i2,i3,
     & vc))+abs(ux23r(i1,i2,i3,wc))+abs(uy23r(i1,i2,i3,wc))+abs(uz23r(
     & i1,i2,i3,wc)))
                           am(i1,i2,i3)=adCoeff4
                           bm(i1,i2,i3)=-uu(uc+1)*dxv2i(1)-nu*dxvsqi(1)
     & -adCoeff2-4.*adCoeff4
                           cm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*nu*(
     & dxvsqi(0)+dxvsqi(1)+dxvsqi(2))+6.*adCoeff2+18.*adCoeff4
                           dm(i1,i2,i3)=uu(uc+1)*dxv2i(1)-nu*dxvsqi(1)-
     & adCoeff2-4.*adCoeff4
                           em(i1,i2,i3)=adCoeff4




c rhs equations:
                           f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fr3d1(uc)-
     & ux2(pc)+adCoeff2*(u(i1-1,i2,i3,uc)+u(i1+1,i2,i3,uc)+u(i1,i2,i3-
     & 1,uc)+u(i1,i2,i3+1,uc))+adCoeff4*(-u(i1-2,i2,i3,uc)-u(i1+2,i2,
     & i3,uc)-u(i1,i2,i3-2,uc)-u(i1,i2,i3+2,uc)+4.*(u(i1-1,i2,i3,uc)+
     & u(i1+1,i2,i3,uc)+u(i1,i2,i3-1,uc)+u(i1,i2,i3+1,uc)))
                           f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fr3d1(vc)-
     & uy2(pc)+adCoeff2*(u(i1-1,i2,i3,vc)+u(i1+1,i2,i3,vc)+u(i1,i2,i3-
     & 1,vc)+u(i1,i2,i3+1,vc))+adCoeff4*(-u(i1-2,i2,i3,vc)-u(i1+2,i2,
     & i3,vc)-u(i1,i2,i3-2,vc)-u(i1,i2,i3+2,vc)+4.*(u(i1-1,i2,i3,vc)+
     & u(i1+1,i2,i3,vc)+u(i1,i2,i3-1,vc)+u(i1,i2,i3+1,vc)))
                           f(i1,i2,i3,fcw)=f(i1,i2,i3,fcw)+fr3d1(wc)-
     & uz2(pc)+adCoeff2*(u(i1-1,i2,i3,wc)+u(i1+1,i2,i3,wc)+u(i1,i2,i3-
     & 1,wc)+u(i1,i2,i3+1,wc))+adCoeff4*(-u(i1-2,i2,i3,wc)-u(i1+2,i2,
     & i3,wc)-u(i1,i2,i3-2,wc)-u(i1,i2,i3+2,wc)+4.*(u(i1-1,i2,i3,wc)+
     & u(i1+1,i2,i3,wc)+u(i1,i2,i3-1,wc)+u(i1,i2,i3+1,wc)))
                         else
c for interpolation points or unused:
                           am(i1,i2,i3)=0.
                           bm(i1,i2,i3)=0.
                           cm(i1,i2,i3)=1.
                           dm(i1,i2,i3)=0.
                           em(i1,i2,i3)=0.
                           f(i1,i2,i3,fcu)=uu(uc)
                           f(i1,i2,i3,fcv)=uu(vc)
                           f(i1,i2,i3,fcw)=uu(wc)
                         end if
                       end do
                       end do
                       end do
                     else if( computeMatrix.eq.1 )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                                adCoeff2 = ad21+cd22*( abs(ux23r(i1,i2,
     & i3,uc))+abs(uy23r(i1,i2,i3,uc))+abs(uz23r(i1,i2,i3,uc))+abs(
     & ux23r(i1,i2,i3,vc))+abs(uy23r(i1,i2,i3,vc))+abs(uz23r(i1,i2,i3,
     & vc))+abs(ux23r(i1,i2,i3,wc))+abs(uy23r(i1,i2,i3,wc))+abs(uz23r(
     & i1,i2,i3,wc)))
                            ! Artficial Dissipation  **todo** implicit-line, self-adjoint versions
                                adCoeff4 = ad41+cd42*( abs(ux23r(i1,i2,
     & i3,uc))+abs(uy23r(i1,i2,i3,uc))+abs(uz23r(i1,i2,i3,uc))+abs(
     & ux23r(i1,i2,i3,vc))+abs(uy23r(i1,i2,i3,vc))+abs(uz23r(i1,i2,i3,
     & vc))+abs(ux23r(i1,i2,i3,wc))+abs(uy23r(i1,i2,i3,wc))+abs(uz23r(
     & i1,i2,i3,wc)))
                           am(i1,i2,i3)=adCoeff4
                           bm(i1,i2,i3)=-uu(uc+1)*dxv2i(1)-nu*dxvsqi(1)
     & -adCoeff2-4.*adCoeff4
                           cm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*nu*(
     & dxvsqi(0)+dxvsqi(1)+dxvsqi(2))+6.*adCoeff2+18.*adCoeff4
                           dm(i1,i2,i3)=uu(uc+1)*dxv2i(1)-nu*dxvsqi(1)-
     & adCoeff2-4.*adCoeff4
                           em(i1,i2,i3)=adCoeff4




                         else
c for interpolation points or unused:
                           am(i1,i2,i3)=0.
                           bm(i1,i2,i3)=0.
                           cm(i1,i2,i3)=1.
                           dm(i1,i2,i3)=0.
                           em(i1,i2,i3)=0.
                         end if
                       end do
                       end do
                       end do
                     else if( computeRHS.eq.1 )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
c rhs equations:
                           f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fr3d1(uc)-
     & ux2(pc)+adCoeff2*(u(i1-1,i2,i3,uc)+u(i1+1,i2,i3,uc)+u(i1,i2,i3-
     & 1,uc)+u(i1,i2,i3+1,uc))+adCoeff4*(-u(i1-2,i2,i3,uc)-u(i1+2,i2,
     & i3,uc)-u(i1,i2,i3-2,uc)-u(i1,i2,i3+2,uc)+4.*(u(i1-1,i2,i3,uc)+
     & u(i1+1,i2,i3,uc)+u(i1,i2,i3-1,uc)+u(i1,i2,i3+1,uc)))
                           f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fr3d1(vc)-
     & uy2(pc)+adCoeff2*(u(i1-1,i2,i3,vc)+u(i1+1,i2,i3,vc)+u(i1,i2,i3-
     & 1,vc)+u(i1,i2,i3+1,vc))+adCoeff4*(-u(i1-2,i2,i3,vc)-u(i1+2,i2,
     & i3,vc)-u(i1,i2,i3-2,vc)-u(i1,i2,i3+2,vc)+4.*(u(i1-1,i2,i3,vc)+
     & u(i1+1,i2,i3,vc)+u(i1,i2,i3-1,vc)+u(i1,i2,i3+1,vc)))
                           f(i1,i2,i3,fcw)=f(i1,i2,i3,fcw)+fr3d1(wc)-
     & uz2(pc)+adCoeff2*(u(i1-1,i2,i3,wc)+u(i1+1,i2,i3,wc)+u(i1,i2,i3-
     & 1,wc)+u(i1,i2,i3+1,wc))+adCoeff4*(-u(i1-2,i2,i3,wc)-u(i1+2,i2,
     & i3,wc)-u(i1,i2,i3-2,wc)-u(i1,i2,i3+2,wc)+4.*(u(i1-1,i2,i3,wc)+
     & u(i1+1,i2,i3,wc)+u(i1,i2,i3-1,wc)+u(i1,i2,i3+1,wc)))
                         else
c for interpolation points or unused:
                           f(i1,i2,i3,fcu)=uu(uc)
                           f(i1,i2,i3,fcv)=uu(vc)
                           f(i1,i2,i3,fcw)=uu(wc)
                         end if
                       end do
                       end do
                       end do
                     end if
                   else
                     stop 111
                   end if
                 else
                   stop 888
                 end if
               else ! dir.eq.2
c  ****** Incompressible NS, No turbulence model *****
                 if( nd.eq.2 )then
                   ! defineDerivativeMacros(DIM,ORDER,GRIDTYPE)
                   if( use2ndOrderAD.eq.0 .and. use4thOrderAD.eq.0 )
     & then
                     if( computeMatrix.eq.1 .and. computeRHS.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                        am(i1,i2,i3)=-uu(uc+2)*dxv2i(2)-nu*dxvsqi(2)
                        bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*nu*(
     & dxvsqi(0)+dxvsqi(1))
                        cm(i1,i2,i3)=uu(uc+2)*dxv2i(2)-nu*dxvsqi(2)







c rhs equations:
                        f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fr2d2(uc)-ux2(
     & pc)
                        f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fr2d2(vc)-uy2(
     & pc)

                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                       end if
                      end do
                      end do
                      end do
                     else if( computeMatrix.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                        am(i1,i2,i3)=-uu(uc+2)*dxv2i(2)-nu*dxvsqi(2)
                        bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*nu*(
     & dxvsqi(0)+dxvsqi(1))
                        cm(i1,i2,i3)=uu(uc+2)*dxv2i(2)-nu*dxvsqi(2)







                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                       end if
                      end do
                      end do
                      end do
                     else if( computeRHS.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c rhs equations:
                        f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fr2d2(uc)-ux2(
     & pc)
                        f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fr2d2(vc)-uy2(
     & pc)

                       else
c for interpolation points or unused:
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                       end if
                      end do
                      end do
                      end do
                     end if
                   else if( use2ndOrderAD.eq.1 .and. 
     & use4thOrderAD.eq.0 )then
                     if( computeMatrix.eq.1 .and. computeRHS.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                            cdmz=admzR(i1  ,i2  ,i3)
                            cdpz=admzR(i1+1,i2  ,i3)
                            cdzm=adzmR(i1  ,i2  ,i3)
                            cdzp=adzmR(i1  ,i2+1,i3)
                            ! write(*,'(1x,''insLS:i1,i2,cdmz,cdzm='',2i3,2f9.3)') i1,i2,cdmz,cdzm
                            ! cdmz=0.
                            ! cdpz=0.
                            ! cdzm=0.
                            ! cdzp=0.
                            cdDiag=cdmz+cdpz+cdzm+cdzp
                              stop 1234
                        am(i1,i2,i3)=-uu(uc+2)*dxv2i(2)-nu*dxvsqi(2)-
     & cdm
                        bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*nu*(
     & dxvsqi(0)+dxvsqi(1))+cdDiag
                        cm(i1,i2,i3)=uu(uc+2)*dxv2i(2)-nu*dxvsqi(2)-cdp






c rhs equations:
                        f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fr2d2(uc)-ux2(
     & pc)+adE2(i1,i2,i3,uc)
                        f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fr2d2(vc)-uy2(
     & pc)+adE2(i1,i2,i3,vc)

                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                       end if
                      end do
                      end do
                      end do
                     else if( computeMatrix.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                            cdmz=admzR(i1  ,i2  ,i3)
                            cdpz=admzR(i1+1,i2  ,i3)
                            cdzm=adzmR(i1  ,i2  ,i3)
                            cdzp=adzmR(i1  ,i2+1,i3)
                            ! write(*,'(1x,''insLS:i1,i2,cdmz,cdzm='',2i3,2f9.3)') i1,i2,cdmz,cdzm
                            ! cdmz=0.
                            ! cdpz=0.
                            ! cdzm=0.
                            ! cdzp=0.
                            cdDiag=cdmz+cdpz+cdzm+cdzp
                              stop 1234
                        am(i1,i2,i3)=-uu(uc+2)*dxv2i(2)-nu*dxvsqi(2)-
     & cdm
                        bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*nu*(
     & dxvsqi(0)+dxvsqi(1))+cdDiag
                        cm(i1,i2,i3)=uu(uc+2)*dxv2i(2)-nu*dxvsqi(2)-cdp






                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                       end if
                      end do
                      end do
                      end do
                     else if( computeRHS.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c rhs equations:
                        f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fr2d2(uc)-ux2(
     & pc)+adE2(i1,i2,i3,uc)
                        f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fr2d2(vc)-uy2(
     & pc)+adE2(i1,i2,i3,vc)

                       else
c for interpolation points or unused:
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                       end if
                      end do
                      end do
                      end do
                     end if
                   else if( use4thOrderAD.eq.1 )then
                         stop 676
                      ! By default there is no AD:
                      ! 2nd-order artificial dissipation
                      ! 4th-order artficial dissipation  **todo** implicit-line, self-adjoint versions
                      ! Both 2nd and 4th order dissipation
                     if( computeMatrix.eq.1 .and. computeRHS.eq.1 )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                                adCoeff2 = ad21+cd22*( abs(ux22r(i1,i2,
     & i3,uc))+abs(uy22r(i1,i2,i3,uc))+abs(ux22r(i1,i2,i3,vc))+abs(
     & uy22r(i1,i2,i3,vc)) )
                            ! Artficial Dissipation  **todo** implicit-line, self-adjoint versions
                                adCoeff4 = ad41+cd42*( abs(ux22r(i1,i2,
     & i3,uc))+abs(uy22r(i1,i2,i3,uc))+abs(ux22r(i1,i2,i3,vc))+abs(
     & uy22r(i1,i2,i3,vc)) )
                           am(i1,i2,i3)=adCoeff4
                           bm(i1,i2,i3)=-uu(uc+2)*dxv2i(2)-nu*dxvsqi(2)
     & -adCoeff2-4.*adCoeff4
                           cm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*nu*(
     & dxvsqi(0)+dxvsqi(1))+4.*adCoeff2+12.*adCoeff4
                           dm(i1,i2,i3)=uu(uc+2)*dxv2i(2)-nu*dxvsqi(2)-
     & adCoeff2-4.*adCoeff4
                           em(i1,i2,i3)=adCoeff4




c rhs equations:
                           f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fr2d2(uc)-
     & ux2(pc)+adCoeff2*(u(i1-1,i2,i3,uc)+u(i1+1,i2,i3,uc)+u(i1,i2,i3-
     & 1,uc)+u(i1,i2,i3+1,uc))+adCoeff4*(-u(i1-2,i2,i3,uc)-u(i1+2,i2,
     & i3,uc)-u(i1,i2,i3-2,uc)-u(i1,i2,i3+2,uc)+4.*(u(i1-1,i2,i3,uc)+
     & u(i1+1,i2,i3,uc)+u(i1,i2,i3-1,uc)+u(i1,i2,i3+1,uc)))
                           f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fr2d2(vc)-
     & uy2(pc)+adCoeff2*(u(i1-1,i2,i3,vc)+u(i1+1,i2,i3,vc)+u(i1,i2,i3-
     & 1,vc)+u(i1,i2,i3+1,vc))+adCoeff4*(-u(i1-2,i2,i3,vc)-u(i1+2,i2,
     & i3,vc)-u(i1,i2,i3-2,vc)-u(i1,i2,i3+2,vc)+4.*(u(i1-1,i2,i3,vc)+
     & u(i1+1,i2,i3,vc)+u(i1,i2,i3-1,vc)+u(i1,i2,i3+1,vc)))

                         else
c for interpolation points or unused:
                           am(i1,i2,i3)=0.
                           bm(i1,i2,i3)=0.
                           cm(i1,i2,i3)=1.
                           dm(i1,i2,i3)=0.
                           em(i1,i2,i3)=0.
                           f(i1,i2,i3,fcu)=uu(uc)
                           f(i1,i2,i3,fcv)=uu(vc)
                         end if
                       end do
                       end do
                       end do
                     else if( computeMatrix.eq.1 )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                                adCoeff2 = ad21+cd22*( abs(ux22r(i1,i2,
     & i3,uc))+abs(uy22r(i1,i2,i3,uc))+abs(ux22r(i1,i2,i3,vc))+abs(
     & uy22r(i1,i2,i3,vc)) )
                            ! Artficial Dissipation  **todo** implicit-line, self-adjoint versions
                                adCoeff4 = ad41+cd42*( abs(ux22r(i1,i2,
     & i3,uc))+abs(uy22r(i1,i2,i3,uc))+abs(ux22r(i1,i2,i3,vc))+abs(
     & uy22r(i1,i2,i3,vc)) )
                           am(i1,i2,i3)=adCoeff4
                           bm(i1,i2,i3)=-uu(uc+2)*dxv2i(2)-nu*dxvsqi(2)
     & -adCoeff2-4.*adCoeff4
                           cm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*nu*(
     & dxvsqi(0)+dxvsqi(1))+4.*adCoeff2+12.*adCoeff4
                           dm(i1,i2,i3)=uu(uc+2)*dxv2i(2)-nu*dxvsqi(2)-
     & adCoeff2-4.*adCoeff4
                           em(i1,i2,i3)=adCoeff4




                         else
c for interpolation points or unused:
                           am(i1,i2,i3)=0.
                           bm(i1,i2,i3)=0.
                           cm(i1,i2,i3)=1.
                           dm(i1,i2,i3)=0.
                           em(i1,i2,i3)=0.
                         end if
                       end do
                       end do
                       end do
                     else if( computeRHS.eq.1 )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
c rhs equations:
                           f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fr2d2(uc)-
     & ux2(pc)+adCoeff2*(u(i1-1,i2,i3,uc)+u(i1+1,i2,i3,uc)+u(i1,i2,i3-
     & 1,uc)+u(i1,i2,i3+1,uc))+adCoeff4*(-u(i1-2,i2,i3,uc)-u(i1+2,i2,
     & i3,uc)-u(i1,i2,i3-2,uc)-u(i1,i2,i3+2,uc)+4.*(u(i1-1,i2,i3,uc)+
     & u(i1+1,i2,i3,uc)+u(i1,i2,i3-1,uc)+u(i1,i2,i3+1,uc)))
                           f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fr2d2(vc)-
     & uy2(pc)+adCoeff2*(u(i1-1,i2,i3,vc)+u(i1+1,i2,i3,vc)+u(i1,i2,i3-
     & 1,vc)+u(i1,i2,i3+1,vc))+adCoeff4*(-u(i1-2,i2,i3,vc)-u(i1+2,i2,
     & i3,vc)-u(i1,i2,i3-2,vc)-u(i1,i2,i3+2,vc)+4.*(u(i1-1,i2,i3,vc)+
     & u(i1+1,i2,i3,vc)+u(i1,i2,i3-1,vc)+u(i1,i2,i3+1,vc)))

                         else
c for interpolation points or unused:
                           f(i1,i2,i3,fcu)=uu(uc)
                           f(i1,i2,i3,fcv)=uu(vc)
                         end if
                       end do
                       end do
                       end do
                     end if
                   else
                     stop 111
                   end if
                 else if( nd.eq.3 )then
                   ! defineDerivativeMacros(DIM,ORDER,GRIDTYPE)
                   if( use2ndOrderAD.eq.0 .and. use4thOrderAD.eq.0 )
     & then
                     if( computeMatrix.eq.1 .and. computeRHS.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                        am(i1,i2,i3)=-uu(uc+2)*dxv2i(2)-nu*dxvsqi(2)
                        bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*nu*(
     & dxvsqi(0)+dxvsqi(1)+dxvsqi(2))
                        cm(i1,i2,i3)=uu(uc+2)*dxv2i(2)-nu*dxvsqi(2)







c rhs equations:
                        f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fr3d2(uc)-ux2(
     & pc)
                        f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fr3d2(vc)-uy2(
     & pc)
                        f(i1,i2,i3,fcw)=f(i1,i2,i3,fcw)+fr3d2(wc)-uz2(
     & pc)
                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                        f(i1,i2,i3,fcw)=uu(wc)
                       end if
                      end do
                      end do
                      end do
                     else if( computeMatrix.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                        am(i1,i2,i3)=-uu(uc+2)*dxv2i(2)-nu*dxvsqi(2)
                        bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*nu*(
     & dxvsqi(0)+dxvsqi(1)+dxvsqi(2))
                        cm(i1,i2,i3)=uu(uc+2)*dxv2i(2)-nu*dxvsqi(2)







                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                       end if
                      end do
                      end do
                      end do
                     else if( computeRHS.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c rhs equations:
                        f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fr3d2(uc)-ux2(
     & pc)
                        f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fr3d2(vc)-uy2(
     & pc)
                        f(i1,i2,i3,fcw)=f(i1,i2,i3,fcw)+fr3d2(wc)-uz2(
     & pc)
                       else
c for interpolation points or unused:
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                        f(i1,i2,i3,fcw)=uu(wc)
                       end if
                      end do
                      end do
                      end do
                     end if
                   else if( use2ndOrderAD.eq.1 .and. 
     & use4thOrderAD.eq.0 )then
                     if( computeMatrix.eq.1 .and. computeRHS.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                            cdmzz=admzzR(i1  ,i2  ,i3  )
                            cdpzz=admzzR(i1+1,i2  ,i3  )
                            cdzmz=adzmzR(i1  ,i2  ,i3  )
                            cdzpz=adzmzR(i1  ,i2+1,i3  )
                            cdzzm=adzzmR(i1  ,i2  ,i3  )
                            cdzzp=adzzmR(i1  ,i2  ,i3+1)
                            cdDiag=cdmzz+cdpzz+cdzmz+cdzpz+cdzzm+cdzzp
                              cdm=cdzzm
                              cdp=cdzzp
                        am(i1,i2,i3)=-uu(uc+2)*dxv2i(2)-nu*dxvsqi(2)-
     & cdm
                        bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*nu*(
     & dxvsqi(0)+dxvsqi(1)+dxvsqi(2))+cdDiag
                        cm(i1,i2,i3)=uu(uc+2)*dxv2i(2)-nu*dxvsqi(2)-cdp






c rhs equations:
                        f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fr3d2(uc)-ux2(
     & pc)+adE3d2(i1,i2,i3,uc)
                        f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fr3d2(vc)-uy2(
     & pc)+adE3d2(i1,i2,i3,vc)
                        f(i1,i2,i3,fcw)=f(i1,i2,i3,fcw)+fr3d2(wc)-uz2(
     & pc)+adE3d2(i1,i2,i3,wc)
                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                        f(i1,i2,i3,fcw)=uu(wc)
                       end if
                      end do
                      end do
                      end do
                     else if( computeMatrix.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                            cdmzz=admzzR(i1  ,i2  ,i3  )
                            cdpzz=admzzR(i1+1,i2  ,i3  )
                            cdzmz=adzmzR(i1  ,i2  ,i3  )
                            cdzpz=adzmzR(i1  ,i2+1,i3  )
                            cdzzm=adzzmR(i1  ,i2  ,i3  )
                            cdzzp=adzzmR(i1  ,i2  ,i3+1)
                            cdDiag=cdmzz+cdpzz+cdzmz+cdzpz+cdzzm+cdzzp
                              cdm=cdzzm
                              cdp=cdzzp
                        am(i1,i2,i3)=-uu(uc+2)*dxv2i(2)-nu*dxvsqi(2)-
     & cdm
                        bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*nu*(
     & dxvsqi(0)+dxvsqi(1)+dxvsqi(2))+cdDiag
                        cm(i1,i2,i3)=uu(uc+2)*dxv2i(2)-nu*dxvsqi(2)-cdp






                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                       end if
                      end do
                      end do
                      end do
                     else if( computeRHS.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c rhs equations:
                        f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fr3d2(uc)-ux2(
     & pc)+adE3d2(i1,i2,i3,uc)
                        f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fr3d2(vc)-uy2(
     & pc)+adE3d2(i1,i2,i3,vc)
                        f(i1,i2,i3,fcw)=f(i1,i2,i3,fcw)+fr3d2(wc)-uz2(
     & pc)+adE3d2(i1,i2,i3,wc)
                       else
c for interpolation points or unused:
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                        f(i1,i2,i3,fcw)=uu(wc)
                       end if
                      end do
                      end do
                      end do
                     end if
                   else if( use4thOrderAD.eq.1 )then
                      ! By default there is no AD:
                      ! 2nd-order artificial dissipation
                      ! 4th-order artficial dissipation  **todo** implicit-line, self-adjoint versions
                      ! Both 2nd and 4th order dissipation
                     if( computeMatrix.eq.1 .and. computeRHS.eq.1 )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                                adCoeff2 = ad21+cd22*( abs(ux23r(i1,i2,
     & i3,uc))+abs(uy23r(i1,i2,i3,uc))+abs(uz23r(i1,i2,i3,uc))+abs(
     & ux23r(i1,i2,i3,vc))+abs(uy23r(i1,i2,i3,vc))+abs(uz23r(i1,i2,i3,
     & vc))+abs(ux23r(i1,i2,i3,wc))+abs(uy23r(i1,i2,i3,wc))+abs(uz23r(
     & i1,i2,i3,wc)))
                            ! Artficial Dissipation  **todo** implicit-line, self-adjoint versions
                                adCoeff4 = ad41+cd42*( abs(ux23r(i1,i2,
     & i3,uc))+abs(uy23r(i1,i2,i3,uc))+abs(uz23r(i1,i2,i3,uc))+abs(
     & ux23r(i1,i2,i3,vc))+abs(uy23r(i1,i2,i3,vc))+abs(uz23r(i1,i2,i3,
     & vc))+abs(ux23r(i1,i2,i3,wc))+abs(uy23r(i1,i2,i3,wc))+abs(uz23r(
     & i1,i2,i3,wc)))
                           am(i1,i2,i3)=adCoeff4
                           bm(i1,i2,i3)=-uu(uc+2)*dxv2i(2)-nu*dxvsqi(2)
     & -adCoeff2-4.*adCoeff4
                           cm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*nu*(
     & dxvsqi(0)+dxvsqi(1)+dxvsqi(2))+6.*adCoeff2+18.*adCoeff4
                           dm(i1,i2,i3)=uu(uc+2)*dxv2i(2)-nu*dxvsqi(2)-
     & adCoeff2-4.*adCoeff4
                           em(i1,i2,i3)=adCoeff4




c rhs equations:
                           f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fr3d2(uc)-
     & ux2(pc)+adCoeff2*(u(i1-1,i2,i3,uc)+u(i1+1,i2,i3,uc)+u(i1,i2-1,
     & i3,uc)+u(i1,i2+1,i3,uc))+adCoeff4*(-u(i1-2,i2,i3,uc)-u(i1+2,i2,
     & i3,uc)-u(i1,i2-2,i3,uc)-u(i1,i2+2,i3,uc)+4.*(u(i1-1,i2,i3,uc)+
     & u(i1+1,i2,i3,uc)+u(i1,i2-1,i3,uc)+u(i1,i2+1,i3,uc)))
                           f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fr3d2(vc)-
     & uy2(pc)+adCoeff2*(u(i1-1,i2,i3,vc)+u(i1+1,i2,i3,vc)+u(i1,i2-1,
     & i3,vc)+u(i1,i2+1,i3,vc))+adCoeff4*(-u(i1-2,i2,i3,vc)-u(i1+2,i2,
     & i3,vc)-u(i1,i2-2,i3,vc)-u(i1,i2+2,i3,vc)+4.*(u(i1-1,i2,i3,vc)+
     & u(i1+1,i2,i3,vc)+u(i1,i2-1,i3,vc)+u(i1,i2+1,i3,vc)))
                           f(i1,i2,i3,fcw)=f(i1,i2,i3,fcw)+fr3d2(wc)-
     & uz2(pc)+adCoeff2*(u(i1-1,i2,i3,wc)+u(i1+1,i2,i3,wc)+u(i1,i2-1,
     & i3,wc)+u(i1,i2+1,i3,wc))+adCoeff4*(-u(i1-2,i2,i3,wc)-u(i1+2,i2,
     & i3,wc)-u(i1,i2-2,i3,wc)-u(i1,i2+2,i3,wc)+4.*(u(i1-1,i2,i3,wc)+
     & u(i1+1,i2,i3,wc)+u(i1,i2-1,i3,wc)+u(i1,i2+1,i3,wc)))
                         else
c for interpolation points or unused:
                           am(i1,i2,i3)=0.
                           bm(i1,i2,i3)=0.
                           cm(i1,i2,i3)=1.
                           dm(i1,i2,i3)=0.
                           em(i1,i2,i3)=0.
                           f(i1,i2,i3,fcu)=uu(uc)
                           f(i1,i2,i3,fcv)=uu(vc)
                           f(i1,i2,i3,fcw)=uu(wc)
                         end if
                       end do
                       end do
                       end do
                     else if( computeMatrix.eq.1 )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                                adCoeff2 = ad21+cd22*( abs(ux23r(i1,i2,
     & i3,uc))+abs(uy23r(i1,i2,i3,uc))+abs(uz23r(i1,i2,i3,uc))+abs(
     & ux23r(i1,i2,i3,vc))+abs(uy23r(i1,i2,i3,vc))+abs(uz23r(i1,i2,i3,
     & vc))+abs(ux23r(i1,i2,i3,wc))+abs(uy23r(i1,i2,i3,wc))+abs(uz23r(
     & i1,i2,i3,wc)))
                            ! Artficial Dissipation  **todo** implicit-line, self-adjoint versions
                                adCoeff4 = ad41+cd42*( abs(ux23r(i1,i2,
     & i3,uc))+abs(uy23r(i1,i2,i3,uc))+abs(uz23r(i1,i2,i3,uc))+abs(
     & ux23r(i1,i2,i3,vc))+abs(uy23r(i1,i2,i3,vc))+abs(uz23r(i1,i2,i3,
     & vc))+abs(ux23r(i1,i2,i3,wc))+abs(uy23r(i1,i2,i3,wc))+abs(uz23r(
     & i1,i2,i3,wc)))
                           am(i1,i2,i3)=adCoeff4
                           bm(i1,i2,i3)=-uu(uc+2)*dxv2i(2)-nu*dxvsqi(2)
     & -adCoeff2-4.*adCoeff4
                           cm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*nu*(
     & dxvsqi(0)+dxvsqi(1)+dxvsqi(2))+6.*adCoeff2+18.*adCoeff4
                           dm(i1,i2,i3)=uu(uc+2)*dxv2i(2)-nu*dxvsqi(2)-
     & adCoeff2-4.*adCoeff4
                           em(i1,i2,i3)=adCoeff4




                         else
c for interpolation points or unused:
                           am(i1,i2,i3)=0.
                           bm(i1,i2,i3)=0.
                           cm(i1,i2,i3)=1.
                           dm(i1,i2,i3)=0.
                           em(i1,i2,i3)=0.
                         end if
                       end do
                       end do
                       end do
                     else if( computeRHS.eq.1 )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
c rhs equations:
                           f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fr3d2(uc)-
     & ux2(pc)+adCoeff2*(u(i1-1,i2,i3,uc)+u(i1+1,i2,i3,uc)+u(i1,i2-1,
     & i3,uc)+u(i1,i2+1,i3,uc))+adCoeff4*(-u(i1-2,i2,i3,uc)-u(i1+2,i2,
     & i3,uc)-u(i1,i2-2,i3,uc)-u(i1,i2+2,i3,uc)+4.*(u(i1-1,i2,i3,uc)+
     & u(i1+1,i2,i3,uc)+u(i1,i2-1,i3,uc)+u(i1,i2+1,i3,uc)))
                           f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fr3d2(vc)-
     & uy2(pc)+adCoeff2*(u(i1-1,i2,i3,vc)+u(i1+1,i2,i3,vc)+u(i1,i2-1,
     & i3,vc)+u(i1,i2+1,i3,vc))+adCoeff4*(-u(i1-2,i2,i3,vc)-u(i1+2,i2,
     & i3,vc)-u(i1,i2-2,i3,vc)-u(i1,i2+2,i3,vc)+4.*(u(i1-1,i2,i3,vc)+
     & u(i1+1,i2,i3,vc)+u(i1,i2-1,i3,vc)+u(i1,i2+1,i3,vc)))
                           f(i1,i2,i3,fcw)=f(i1,i2,i3,fcw)+fr3d2(wc)-
     & uz2(pc)+adCoeff2*(u(i1-1,i2,i3,wc)+u(i1+1,i2,i3,wc)+u(i1,i2-1,
     & i3,wc)+u(i1,i2+1,i3,wc))+adCoeff4*(-u(i1-2,i2,i3,wc)-u(i1+2,i2,
     & i3,wc)-u(i1,i2-2,i3,wc)-u(i1,i2+2,i3,wc)+4.*(u(i1-1,i2,i3,wc)+
     & u(i1+1,i2,i3,wc)+u(i1,i2-1,i3,wc)+u(i1,i2+1,i3,wc)))
                         else
c for interpolation points or unused:
                           f(i1,i2,i3,fcu)=uu(uc)
                           f(i1,i2,i3,fcv)=uu(vc)
                           f(i1,i2,i3,fcw)=uu(wc)
                         end if
                       end do
                       end do
                       end do
                     end if
                   else
                     stop 111
                   end if
                 else
                   stop 888
                 end if
               end if ! end dir
              else ! order==4
              end if
            else if( gridType.eq.curvilinear )then
              ! *******************************************
              ! ************** curvilinear  ***************
              ! *******************************************
              if( orderOfAccuracy.eq.2 )then
                if( dir.eq.0 )then
                 dirp1=mod(0+1,nd)
                 dirp2=mod(0+2,nd)
c  ****** Incompressible NS, No turbulence model *****
                 if( nd.eq.2 )then
                   if( use2ndOrderAD.eq.0 .and. use4thOrderAD.eq.0 )
     & then
                     if( computeMatrix.eq.1 .and. computeRHS.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                        t1=(uu(uc)*rxi(0,0)+uu(vc)*rxi(0,1)-nu*(rxx(0,
     & 0)+rxy(0,1)))*drv2i(0)
                        t2=nu*(rxi(0,0)**2+rxi(0,1)**2)*drvsqi(0)
                        am(i1,i2,i3)=-t1-t2
                        bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*(t2+nu*(
     & rxi(dirp1,0)**2+rxi(dirp1,1)**2)*drvsqi(dirp1))
                        cm(i1,i2,i3)=t1-t2





c rhs equations:
                        f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fc2d0(uc)-ux2c(
     & pc)
                        f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fc2d0(vc)-uy2c(
     & pc)

                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                       end if
                      end do
                      end do
                      end do
                     else if( computeMatrix.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                        t1=(uu(uc)*rxi(0,0)+uu(vc)*rxi(0,1)-nu*(rxx(0,
     & 0)+rxy(0,1)))*drv2i(0)
                        t2=nu*(rxi(0,0)**2+rxi(0,1)**2)*drvsqi(0)
                        am(i1,i2,i3)=-t1-t2
                        bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*(t2+nu*(
     & rxi(dirp1,0)**2+rxi(dirp1,1)**2)*drvsqi(dirp1))
                        cm(i1,i2,i3)=t1-t2





                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                       end if
                      end do
                      end do
                      end do
                     else if( computeRHS.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c rhs equations:
                        f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fc2d0(uc)-ux2c(
     & pc)
                        f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fc2d0(vc)-uy2c(
     & pc)

                       else
c for interpolation points or unused:
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                       end if
                      end do
                      end do
                      end do
                     end if
                   else if( use2ndOrderAD.eq.1 .and. 
     & use4thOrderAD.eq.0 )then
                     if( computeMatrix.eq.1 .and. computeRHS.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                            cdmz=admzC(i1  ,i2  ,i3)
                            cdpz=admzC(i1+1,i2  ,i3)
                            cdzm=adzmC(i1  ,i2  ,i3)
                            cdzp=adzmC(i1  ,i2+1,i3)
                            ! write(*,'(1x,''insLS:i1,i2,cdmz,cdzm='',2i3,2f9.3)') i1,i2,cdmz,cdzm
                            ! cdmz=0.
                            ! cdpz=0.
                            ! cdzm=0.
                            ! cdzp=0.
                            cdDiag=cdmz+cdpz+cdzm+cdzp
                              cdm=cdmz
                              cdp=cdpz
                        t1=(uu(uc)*rxi(0,0)+uu(vc)*rxi(0,1)-nu*(rxx(0,
     & 0)+rxy(0,1)))*drv2i(0)
                        t2=nu*(rxi(0,0)**2+rxi(0,1)**2)*drvsqi(0)
                        am(i1,i2,i3)=-t1-t2-cdm
                        bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*(t2+nu*(
     & rxi(dirp1,0)**2+rxi(dirp1,1)**2)*drvsqi(dirp1))+cdDiag
                        cm(i1,i2,i3)=t1-t2-cdp




c rhs equations:
                        f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fc2d0(uc)-ux2c(
     & pc)+adE0(i1,i2,i3,uc)
                        f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fc2d0(vc)-uy2c(
     & pc)+adE0(i1,i2,i3,vc)

                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                       end if
                      end do
                      end do
                      end do
                     else if( computeMatrix.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                            cdmz=admzC(i1  ,i2  ,i3)
                            cdpz=admzC(i1+1,i2  ,i3)
                            cdzm=adzmC(i1  ,i2  ,i3)
                            cdzp=adzmC(i1  ,i2+1,i3)
                            ! write(*,'(1x,''insLS:i1,i2,cdmz,cdzm='',2i3,2f9.3)') i1,i2,cdmz,cdzm
                            ! cdmz=0.
                            ! cdpz=0.
                            ! cdzm=0.
                            ! cdzp=0.
                            cdDiag=cdmz+cdpz+cdzm+cdzp
                              cdm=cdmz
                              cdp=cdpz
                        t1=(uu(uc)*rxi(0,0)+uu(vc)*rxi(0,1)-nu*(rxx(0,
     & 0)+rxy(0,1)))*drv2i(0)
                        t2=nu*(rxi(0,0)**2+rxi(0,1)**2)*drvsqi(0)
                        am(i1,i2,i3)=-t1-t2-cdm
                        bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*(t2+nu*(
     & rxi(dirp1,0)**2+rxi(dirp1,1)**2)*drvsqi(dirp1))+cdDiag
                        cm(i1,i2,i3)=t1-t2-cdp




                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                       end if
                      end do
                      end do
                      end do
                     else if( computeRHS.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c rhs equations:
                        f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fc2d0(uc)-ux2c(
     & pc)+adE0(i1,i2,i3,uc)
                        f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fc2d0(vc)-uy2c(
     & pc)+adE0(i1,i2,i3,vc)

                       else
c for interpolation points or unused:
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                       end if
                      end do
                      end do
                      end do
                     end if
                   else if( use4thOrderAD.eq.1 )then
                      ! By default there is no AD:
                      ! 2nd-order artificial dissipation
                      ! 4th-order artficial dissipation  **todo** implicit-line, self-adjoint versions
                      ! Both 2nd and 4th order dissipation
                     if( computeMatrix.eq.1 .and. computeRHS.eq.1 )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                                adCoeff2 = ad21+cd22*( abs(ux22(i1,i2,
     & i3,uc))+abs(uy22(i1,i2,i3,uc))+abs(ux22(i1,i2,i3,vc))+abs(uy22(
     & i1,i2,i3,vc)) )
                            ! Artficial Dissipation  **todo** implicit-line, self-adjoint versions
                                adCoeff4 = ad41+cd42*( abs(ux22(i1,i2,
     & i3,uc))+abs(uy22(i1,i2,i3,uc))+abs(ux22(i1,i2,i3,vc))+abs(uy22(
     & i1,i2,i3,vc)) )
                           t1=(uu(uc)*rxi(0,0)+uu(vc)*rxi(0,1)-nu*(rxx(
     & 0,0)+rxy(0,1)))*drv2i(0)
                           t2=nu*(rxi(0,0)**2+rxi(0,1)**2)*drvsqi(0)
                           am(i1,i2,i3)=adCoeff4
                           bm(i1,i2,i3)=-t1-t2-adCoeff2-4.*adCoeff4
                           cm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*(t2+nu*
     & (rxi(dirp1,0)**2+rxi(dirp1,1)**2)*drvsqi(dirp1))+4.*adCoeff2+
     & 12.*adCoeff4
                           dm(i1,i2,i3)=t1-t2-adCoeff2-4.*adCoeff4
                           em(i1,i2,i3)=adCoeff4


c rhs equations:
                           f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fc2d0(uc)-
     & ux2c(pc)+adCoeff2*(u(i1,i2-1,i3,uc)+u(i1,i2+1,i3,uc))+adCoeff4*
     & (-u(i1,i2-2,i3,uc)-u(i1,i2+2,i3,uc)+4.*(u(i1,i2-1,i3,uc)+u(i1,
     & i2+1,i3,uc)))
                           f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fc2d0(vc)-
     & uy2c(pc)+adCoeff2*(u(i1,i2-1,i3,vc)+u(i1,i2+1,i3,vc))+adCoeff4*
     & (-u(i1,i2-2,i3,vc)-u(i1,i2+2,i3,vc)+4.*(u(i1,i2-1,i3,vc)+u(i1,
     & i2+1,i3,vc)))

                         else
c for interpolation points or unused:
                           am(i1,i2,i3)=0.
                           bm(i1,i2,i3)=0.
                           cm(i1,i2,i3)=1.
                           dm(i1,i2,i3)=0.
                           em(i1,i2,i3)=0.
                           f(i1,i2,i3,fcu)=uu(uc)
                           f(i1,i2,i3,fcv)=uu(vc)
                         end if
                       end do
                       end do
                       end do
                     else if( computeMatrix.eq.1 )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                                adCoeff2 = ad21+cd22*( abs(ux22(i1,i2,
     & i3,uc))+abs(uy22(i1,i2,i3,uc))+abs(ux22(i1,i2,i3,vc))+abs(uy22(
     & i1,i2,i3,vc)) )
                            ! Artficial Dissipation  **todo** implicit-line, self-adjoint versions
                                adCoeff4 = ad41+cd42*( abs(ux22(i1,i2,
     & i3,uc))+abs(uy22(i1,i2,i3,uc))+abs(ux22(i1,i2,i3,vc))+abs(uy22(
     & i1,i2,i3,vc)) )
                           t1=(uu(uc)*rxi(0,0)+uu(vc)*rxi(0,1)-nu*(rxx(
     & 0,0)+rxy(0,1)))*drv2i(0)
                           t2=nu*(rxi(0,0)**2+rxi(0,1)**2)*drvsqi(0)
                           am(i1,i2,i3)=adCoeff4
                           bm(i1,i2,i3)=-t1-t2-adCoeff2-4.*adCoeff4
                           cm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*(t2+nu*
     & (rxi(dirp1,0)**2+rxi(dirp1,1)**2)*drvsqi(dirp1))+4.*adCoeff2+
     & 12.*adCoeff4
                           dm(i1,i2,i3)=t1-t2-adCoeff2-4.*adCoeff4
                           em(i1,i2,i3)=adCoeff4


                         else
c for interpolation points or unused:
                           am(i1,i2,i3)=0.
                           bm(i1,i2,i3)=0.
                           cm(i1,i2,i3)=1.
                           dm(i1,i2,i3)=0.
                           em(i1,i2,i3)=0.
                         end if
                       end do
                       end do
                       end do
                     else if( computeRHS.eq.1 )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
c rhs equations:
                           f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fc2d0(uc)-
     & ux2c(pc)+adCoeff2*(u(i1,i2-1,i3,uc)+u(i1,i2+1,i3,uc))+adCoeff4*
     & (-u(i1,i2-2,i3,uc)-u(i1,i2+2,i3,uc)+4.*(u(i1,i2-1,i3,uc)+u(i1,
     & i2+1,i3,uc)))
                           f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fc2d0(vc)-
     & uy2c(pc)+adCoeff2*(u(i1,i2-1,i3,vc)+u(i1,i2+1,i3,vc))+adCoeff4*
     & (-u(i1,i2-2,i3,vc)-u(i1,i2+2,i3,vc)+4.*(u(i1,i2-1,i3,vc)+u(i1,
     & i2+1,i3,vc)))

                         else
c for interpolation points or unused:
                           f(i1,i2,i3,fcu)=uu(uc)
                           f(i1,i2,i3,fcv)=uu(vc)
                         end if
                       end do
                       end do
                       end do
                     end if
                   else
                     stop 2222
                   end if
                 else if( nd.eq.3 )then
                   if( use2ndOrderAD.eq.0 .and. use4thOrderAD.eq.0 )
     & then
                     if( computeMatrix.eq.1 .and. computeRHS.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                        t1=(uu(uc)*rxi(0,0)+uu(vc)*rxi(0,1)+uu(wc)*rxi(
     & 0,2)-nu*(rxx3(0,0)+rxy3(0,1)+rxz3(0,2)))*drv2i(0)
                        t2=nu*(rxi(0,0)**2+rxi(0,1)**2+rxi(0,2)**2)*
     & drvsqi(0)
                        am(i1,i2,i3)=-t1-t2
                        bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*(t2+nu*((
     & rxi(dirp1,0)**2+rxi(dirp1,1)**2+rxi(dirp1,2)**2)*drvsqi(dirp1)+
     & (rxi(dirp2,0)**2+rxi(dirp2,1)**2+rxi(dirp2,2)**2)*drvsqi(dirp2)
     & ))
                        cm(i1,i2,i3)=t1-t2





c rhs equations:
                        f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fc3d0(uc)-ux3c(
     & pc)
                        f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fc3d0(vc)-uy3c(
     & pc)
                        f(i1,i2,i3,fcw)=f(i1,i2,i3,fcw)+fc3d0(wc)-uz3c(
     & pc)
                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                        f(i1,i2,i3,fcw)=uu(wc)
                       end if
                      end do
                      end do
                      end do
                     else if( computeMatrix.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                        t1=(uu(uc)*rxi(0,0)+uu(vc)*rxi(0,1)+uu(wc)*rxi(
     & 0,2)-nu*(rxx3(0,0)+rxy3(0,1)+rxz3(0,2)))*drv2i(0)
                        t2=nu*(rxi(0,0)**2+rxi(0,1)**2+rxi(0,2)**2)*
     & drvsqi(0)
                        am(i1,i2,i3)=-t1-t2
                        bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*(t2+nu*((
     & rxi(dirp1,0)**2+rxi(dirp1,1)**2+rxi(dirp1,2)**2)*drvsqi(dirp1)+
     & (rxi(dirp2,0)**2+rxi(dirp2,1)**2+rxi(dirp2,2)**2)*drvsqi(dirp2)
     & ))
                        cm(i1,i2,i3)=t1-t2





                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                       end if
                      end do
                      end do
                      end do
                     else if( computeRHS.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c rhs equations:
                        f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fc3d0(uc)-ux3c(
     & pc)
                        f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fc3d0(vc)-uy3c(
     & pc)
                        f(i1,i2,i3,fcw)=f(i1,i2,i3,fcw)+fc3d0(wc)-uz3c(
     & pc)
                       else
c for interpolation points or unused:
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                        f(i1,i2,i3,fcw)=uu(wc)
                       end if
                      end do
                      end do
                      end do
                     end if
                   else if( use2ndOrderAD.eq.1 .and. 
     & use4thOrderAD.eq.0 )then
                     if( computeMatrix.eq.1 .and. computeRHS.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                            cdmzz=admzzC(i1  ,i2  ,i3  )
                            cdpzz=admzzC(i1+1,i2  ,i3  )
                            cdzmz=adzmzC(i1  ,i2  ,i3  )
                            cdzpz=adzmzC(i1  ,i2+1,i3  )
                            cdzzm=adzzmC(i1  ,i2  ,i3  )
                            cdzzp=adzzmC(i1  ,i2  ,i3+1)
                            cdDiag=cdmzz+cdpzz+cdzmz+cdzpz+cdzzm+cdzzp
                              cdm=cdmzz
                              cdp=cdpzz
                        t1=(uu(uc)*rxi(0,0)+uu(vc)*rxi(0,1)+uu(wc)*rxi(
     & 0,2)-nu*(rxx3(0,0)+rxy3(0,1)+rxz3(0,2)))*drv2i(0)
                        t2=nu*(rxi(0,0)**2+rxi(0,1)**2+rxi(0,2)**2)*
     & drvsqi(0)
                        am(i1,i2,i3)=-t1-t2-cdm
                        bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*(t2+nu*((
     & rxi(dirp1,0)**2+rxi(dirp1,1)**2+rxi(dirp1,2)**2)*drvsqi(dirp1)+
     & (rxi(dirp2,0)**2+rxi(dirp2,1)**2+rxi(dirp2,2)**2)*drvsqi(dirp2)
     & ))+cdDiag
                        cm(i1,i2,i3)=t1-t2-cdp




c rhs equations:
                        f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fc3d0(uc)-ux3c(
     & pc)+adE3d0(i1,i2,i3,uc)
                        f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fc3d0(vc)-uy3c(
     & pc)+adE3d0(i1,i2,i3,vc)
                        f(i1,i2,i3,fcw)=f(i1,i2,i3,fcw)+fc3d0(wc)-uz3c(
     & pc)+adE3d0(i1,i2,i3,wc)
                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                        f(i1,i2,i3,fcw)=uu(wc)
                       end if
                      end do
                      end do
                      end do
                     else if( computeMatrix.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                            cdmzz=admzzC(i1  ,i2  ,i3  )
                            cdpzz=admzzC(i1+1,i2  ,i3  )
                            cdzmz=adzmzC(i1  ,i2  ,i3  )
                            cdzpz=adzmzC(i1  ,i2+1,i3  )
                            cdzzm=adzzmC(i1  ,i2  ,i3  )
                            cdzzp=adzzmC(i1  ,i2  ,i3+1)
                            cdDiag=cdmzz+cdpzz+cdzmz+cdzpz+cdzzm+cdzzp
                              cdm=cdmzz
                              cdp=cdpzz
                        t1=(uu(uc)*rxi(0,0)+uu(vc)*rxi(0,1)+uu(wc)*rxi(
     & 0,2)-nu*(rxx3(0,0)+rxy3(0,1)+rxz3(0,2)))*drv2i(0)
                        t2=nu*(rxi(0,0)**2+rxi(0,1)**2+rxi(0,2)**2)*
     & drvsqi(0)
                        am(i1,i2,i3)=-t1-t2-cdm
                        bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*(t2+nu*((
     & rxi(dirp1,0)**2+rxi(dirp1,1)**2+rxi(dirp1,2)**2)*drvsqi(dirp1)+
     & (rxi(dirp2,0)**2+rxi(dirp2,1)**2+rxi(dirp2,2)**2)*drvsqi(dirp2)
     & ))+cdDiag
                        cm(i1,i2,i3)=t1-t2-cdp




                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                       end if
                      end do
                      end do
                      end do
                     else if( computeRHS.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c rhs equations:
                        f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fc3d0(uc)-ux3c(
     & pc)+adE3d0(i1,i2,i3,uc)
                        f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fc3d0(vc)-uy3c(
     & pc)+adE3d0(i1,i2,i3,vc)
                        f(i1,i2,i3,fcw)=f(i1,i2,i3,fcw)+fc3d0(wc)-uz3c(
     & pc)+adE3d0(i1,i2,i3,wc)
                       else
c for interpolation points or unused:
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                        f(i1,i2,i3,fcw)=uu(wc)
                       end if
                      end do
                      end do
                      end do
                     end if
                   else if( use4thOrderAD.eq.1 )then
                      ! By default there is no AD:
                      ! 2nd-order artificial dissipation
                      ! 4th-order artficial dissipation  **todo** implicit-line, self-adjoint versions
                      ! Both 2nd and 4th order dissipation
                     if( computeMatrix.eq.1 .and. computeRHS.eq.1 )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                                adCoeff2 = ad21+cd22*( abs(ux23(i1,i2,
     & i3,uc))+abs(uy23(i1,i2,i3,uc))+abs(uz23(i1,i2,i3,uc))+abs(ux23(
     & i1,i2,i3,vc))+abs(uy23(i1,i2,i3,vc))+abs(uz23(i1,i2,i3,vc))+
     & abs(ux23(i1,i2,i3,wc))+abs(uy23(i1,i2,i3,wc))+abs(uz23(i1,i2,
     & i3,wc)))
                            ! Artficial Dissipation  **todo** implicit-line, self-adjoint versions
                                adCoeff4 = ad41+cd42*( abs(ux23(i1,i2,
     & i3,uc))+abs(uy23(i1,i2,i3,uc))+abs(uz23(i1,i2,i3,uc))+abs(ux23(
     & i1,i2,i3,vc))+abs(uy23(i1,i2,i3,vc))+abs(uz23(i1,i2,i3,vc))+
     & abs(ux23(i1,i2,i3,wc))+abs(uy23(i1,i2,i3,wc))+abs(uz23(i1,i2,
     & i3,wc)))
                           t1=(uu(uc)*rxi(0,0)+uu(vc)*rxi(0,1)+uu(wc)*
     & rxi(0,2)-nu*(rxx3(0,0)+rxy3(0,1)+rxz3(0,2)))*drv2i(0)
                           t2=nu*(rxi(0,0)**2+rxi(0,1)**2+rxi(0,2)**2)*
     & drvsqi(0)
                           am(i1,i2,i3)=adCoeff4
                           bm(i1,i2,i3)=-t1-t2-adCoeff2-4.*adCoeff4
                           cm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*(t2+nu*
     & ((rxi(dirp1,0)**2+rxi(dirp1,1)**2+rxi(dirp1,2)**2)*drvsqi(
     & dirp1)+(rxi(dirp2,0)**2+rxi(dirp2,1)**2+rxi(dirp2,2)**2)*
     & drvsqi(dirp2)))+6.*adCoeff2+18.*adCoeff4
                           dm(i1,i2,i3)=t1-t2-adCoeff2-4.*adCoeff4
                           em(i1,i2,i3)=adCoeff4


c rhs equations:
                           f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fc3d0(uc)-
     & ux3c(pc)+adCoeff2*(u(i1,i2-1,i3,uc)+u(i1,i2+1,i3,uc)+u(i1,i2,
     & i3-1,uc)+u(i1,i2,i3+1,uc))+adCoeff4*(-u(i1,i2-2,i3,uc)-u(i1,i2+
     & 2,i3,uc)-u(i1,i2,i3-2,uc)-u(i1,i2,i3+2,uc)+4.*(u(i1,i2-1,i3,uc)
     & +u(i1,i2+1,i3,uc)+u(i1,i2,i3-1,uc)+u(i1,i2,i3+1,uc)))
                           f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fc3d0(vc)-
     & uy3c(pc)+adCoeff2*(u(i1,i2-1,i3,vc)+u(i1,i2+1,i3,vc)+u(i1,i2,
     & i3-1,vc)+u(i1,i2,i3+1,vc))+adCoeff4*(-u(i1,i2-2,i3,vc)-u(i1,i2+
     & 2,i3,vc)-u(i1,i2,i3-2,vc)-u(i1,i2,i3+2,vc)+4.*(u(i1,i2-1,i3,vc)
     & +u(i1,i2+1,i3,vc)+u(i1,i2,i3-1,vc)+u(i1,i2,i3+1,vc)))
                           f(i1,i2,i3,fcw)=f(i1,i2,i3,fcw)+fc3d0(wc)-
     & uz3c(pc)+adCoeff2*(u(i1,i2-1,i3,wc)+u(i1,i2+1,i3,wc)+u(i1,i2,
     & i3-1,wc)+u(i1,i2,i3+1,wc))+adCoeff4*(-u(i1,i2-2,i3,wc)-u(i1,i2+
     & 2,i3,wc)-u(i1,i2,i3-2,wc)-u(i1,i2,i3+2,wc)+4.*(u(i1,i2-1,i3,wc)
     & +u(i1,i2+1,i3,wc)+u(i1,i2,i3-1,wc)+u(i1,i2,i3+1,wc)))
                         else
c for interpolation points or unused:
                           am(i1,i2,i3)=0.
                           bm(i1,i2,i3)=0.
                           cm(i1,i2,i3)=1.
                           dm(i1,i2,i3)=0.
                           em(i1,i2,i3)=0.
                           f(i1,i2,i3,fcu)=uu(uc)
                           f(i1,i2,i3,fcv)=uu(vc)
                           f(i1,i2,i3,fcw)=uu(wc)
                         end if
                       end do
                       end do
                       end do
                     else if( computeMatrix.eq.1 )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                                adCoeff2 = ad21+cd22*( abs(ux23(i1,i2,
     & i3,uc))+abs(uy23(i1,i2,i3,uc))+abs(uz23(i1,i2,i3,uc))+abs(ux23(
     & i1,i2,i3,vc))+abs(uy23(i1,i2,i3,vc))+abs(uz23(i1,i2,i3,vc))+
     & abs(ux23(i1,i2,i3,wc))+abs(uy23(i1,i2,i3,wc))+abs(uz23(i1,i2,
     & i3,wc)))
                            ! Artficial Dissipation  **todo** implicit-line, self-adjoint versions
                                adCoeff4 = ad41+cd42*( abs(ux23(i1,i2,
     & i3,uc))+abs(uy23(i1,i2,i3,uc))+abs(uz23(i1,i2,i3,uc))+abs(ux23(
     & i1,i2,i3,vc))+abs(uy23(i1,i2,i3,vc))+abs(uz23(i1,i2,i3,vc))+
     & abs(ux23(i1,i2,i3,wc))+abs(uy23(i1,i2,i3,wc))+abs(uz23(i1,i2,
     & i3,wc)))
                           t1=(uu(uc)*rxi(0,0)+uu(vc)*rxi(0,1)+uu(wc)*
     & rxi(0,2)-nu*(rxx3(0,0)+rxy3(0,1)+rxz3(0,2)))*drv2i(0)
                           t2=nu*(rxi(0,0)**2+rxi(0,1)**2+rxi(0,2)**2)*
     & drvsqi(0)
                           am(i1,i2,i3)=adCoeff4
                           bm(i1,i2,i3)=-t1-t2-adCoeff2-4.*adCoeff4
                           cm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*(t2+nu*
     & ((rxi(dirp1,0)**2+rxi(dirp1,1)**2+rxi(dirp1,2)**2)*drvsqi(
     & dirp1)+(rxi(dirp2,0)**2+rxi(dirp2,1)**2+rxi(dirp2,2)**2)*
     & drvsqi(dirp2)))+6.*adCoeff2+18.*adCoeff4
                           dm(i1,i2,i3)=t1-t2-adCoeff2-4.*adCoeff4
                           em(i1,i2,i3)=adCoeff4


                         else
c for interpolation points or unused:
                           am(i1,i2,i3)=0.
                           bm(i1,i2,i3)=0.
                           cm(i1,i2,i3)=1.
                           dm(i1,i2,i3)=0.
                           em(i1,i2,i3)=0.
                         end if
                       end do
                       end do
                       end do
                     else if( computeRHS.eq.1 )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
c rhs equations:
                           f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fc3d0(uc)-
     & ux3c(pc)+adCoeff2*(u(i1,i2-1,i3,uc)+u(i1,i2+1,i3,uc)+u(i1,i2,
     & i3-1,uc)+u(i1,i2,i3+1,uc))+adCoeff4*(-u(i1,i2-2,i3,uc)-u(i1,i2+
     & 2,i3,uc)-u(i1,i2,i3-2,uc)-u(i1,i2,i3+2,uc)+4.*(u(i1,i2-1,i3,uc)
     & +u(i1,i2+1,i3,uc)+u(i1,i2,i3-1,uc)+u(i1,i2,i3+1,uc)))
                           f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fc3d0(vc)-
     & uy3c(pc)+adCoeff2*(u(i1,i2-1,i3,vc)+u(i1,i2+1,i3,vc)+u(i1,i2,
     & i3-1,vc)+u(i1,i2,i3+1,vc))+adCoeff4*(-u(i1,i2-2,i3,vc)-u(i1,i2+
     & 2,i3,vc)-u(i1,i2,i3-2,vc)-u(i1,i2,i3+2,vc)+4.*(u(i1,i2-1,i3,vc)
     & +u(i1,i2+1,i3,vc)+u(i1,i2,i3-1,vc)+u(i1,i2,i3+1,vc)))
                           f(i1,i2,i3,fcw)=f(i1,i2,i3,fcw)+fc3d0(wc)-
     & uz3c(pc)+adCoeff2*(u(i1,i2-1,i3,wc)+u(i1,i2+1,i3,wc)+u(i1,i2,
     & i3-1,wc)+u(i1,i2,i3+1,wc))+adCoeff4*(-u(i1,i2-2,i3,wc)-u(i1,i2+
     & 2,i3,wc)-u(i1,i2,i3-2,wc)-u(i1,i2,i3+2,wc)+4.*(u(i1,i2-1,i3,wc)
     & +u(i1,i2+1,i3,wc)+u(i1,i2,i3-1,wc)+u(i1,i2,i3+1,wc)))
                         else
c for interpolation points or unused:
                           f(i1,i2,i3,fcu)=uu(uc)
                           f(i1,i2,i3,fcv)=uu(vc)
                           f(i1,i2,i3,fcw)=uu(wc)
                         end if
                       end do
                       end do
                       end do
                     end if
                   else
                     stop 33333
                   end if
                 else
                   stop 222
                 end if
                else if( dir.eq.1 )then
                 dirp1=mod(1+1,nd)
                 dirp2=mod(1+2,nd)
c  ****** Incompressible NS, No turbulence model *****
                 if( nd.eq.2 )then
                   if( use2ndOrderAD.eq.0 .and. use4thOrderAD.eq.0 )
     & then
                     if( computeMatrix.eq.1 .and. computeRHS.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                        t1=(uu(uc)*rxi(1,0)+uu(vc)*rxi(1,1)-nu*(rxx(1,
     & 0)+rxy(1,1)))*drv2i(1)
                        t2=nu*(rxi(1,0)**2+rxi(1,1)**2)*drvsqi(1)
                        am(i1,i2,i3)=-t1-t2
                        bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*(t2+nu*(
     & rxi(dirp1,0)**2+rxi(dirp1,1)**2)*drvsqi(dirp1))
                        cm(i1,i2,i3)=t1-t2





c rhs equations:
                        f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fc2d1(uc)-ux2c(
     & pc)
                        f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fc2d1(vc)-uy2c(
     & pc)

                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                       end if
                      end do
                      end do
                      end do
                     else if( computeMatrix.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                        t1=(uu(uc)*rxi(1,0)+uu(vc)*rxi(1,1)-nu*(rxx(1,
     & 0)+rxy(1,1)))*drv2i(1)
                        t2=nu*(rxi(1,0)**2+rxi(1,1)**2)*drvsqi(1)
                        am(i1,i2,i3)=-t1-t2
                        bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*(t2+nu*(
     & rxi(dirp1,0)**2+rxi(dirp1,1)**2)*drvsqi(dirp1))
                        cm(i1,i2,i3)=t1-t2





                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                       end if
                      end do
                      end do
                      end do
                     else if( computeRHS.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c rhs equations:
                        f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fc2d1(uc)-ux2c(
     & pc)
                        f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fc2d1(vc)-uy2c(
     & pc)

                       else
c for interpolation points or unused:
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                       end if
                      end do
                      end do
                      end do
                     end if
                   else if( use2ndOrderAD.eq.1 .and. 
     & use4thOrderAD.eq.0 )then
                     if( computeMatrix.eq.1 .and. computeRHS.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                            cdmz=admzC(i1  ,i2  ,i3)
                            cdpz=admzC(i1+1,i2  ,i3)
                            cdzm=adzmC(i1  ,i2  ,i3)
                            cdzp=adzmC(i1  ,i2+1,i3)
                            ! write(*,'(1x,''insLS:i1,i2,cdmz,cdzm='',2i3,2f9.3)') i1,i2,cdmz,cdzm
                            ! cdmz=0.
                            ! cdpz=0.
                            ! cdzm=0.
                            ! cdzp=0.
                            cdDiag=cdmz+cdpz+cdzm+cdzp
                              cdm=cdzm
                              cdp=cdzp
                        t1=(uu(uc)*rxi(1,0)+uu(vc)*rxi(1,1)-nu*(rxx(1,
     & 0)+rxy(1,1)))*drv2i(1)
                        t2=nu*(rxi(1,0)**2+rxi(1,1)**2)*drvsqi(1)
                        am(i1,i2,i3)=-t1-t2-cdm
                        bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*(t2+nu*(
     & rxi(dirp1,0)**2+rxi(dirp1,1)**2)*drvsqi(dirp1))+cdDiag
                        cm(i1,i2,i3)=t1-t2-cdp




c rhs equations:
                        f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fc2d1(uc)-ux2c(
     & pc)+adE1(i1,i2,i3,uc)
                        f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fc2d1(vc)-uy2c(
     & pc)+adE1(i1,i2,i3,vc)

                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                       end if
                      end do
                      end do
                      end do
                     else if( computeMatrix.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                            cdmz=admzC(i1  ,i2  ,i3)
                            cdpz=admzC(i1+1,i2  ,i3)
                            cdzm=adzmC(i1  ,i2  ,i3)
                            cdzp=adzmC(i1  ,i2+1,i3)
                            ! write(*,'(1x,''insLS:i1,i2,cdmz,cdzm='',2i3,2f9.3)') i1,i2,cdmz,cdzm
                            ! cdmz=0.
                            ! cdpz=0.
                            ! cdzm=0.
                            ! cdzp=0.
                            cdDiag=cdmz+cdpz+cdzm+cdzp
                              cdm=cdzm
                              cdp=cdzp
                        t1=(uu(uc)*rxi(1,0)+uu(vc)*rxi(1,1)-nu*(rxx(1,
     & 0)+rxy(1,1)))*drv2i(1)
                        t2=nu*(rxi(1,0)**2+rxi(1,1)**2)*drvsqi(1)
                        am(i1,i2,i3)=-t1-t2-cdm
                        bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*(t2+nu*(
     & rxi(dirp1,0)**2+rxi(dirp1,1)**2)*drvsqi(dirp1))+cdDiag
                        cm(i1,i2,i3)=t1-t2-cdp




                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                       end if
                      end do
                      end do
                      end do
                     else if( computeRHS.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c rhs equations:
                        f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fc2d1(uc)-ux2c(
     & pc)+adE1(i1,i2,i3,uc)
                        f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fc2d1(vc)-uy2c(
     & pc)+adE1(i1,i2,i3,vc)

                       else
c for interpolation points or unused:
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                       end if
                      end do
                      end do
                      end do
                     end if
                   else if( use4thOrderAD.eq.1 )then
                      ! By default there is no AD:
                      ! 2nd-order artificial dissipation
                      ! 4th-order artficial dissipation  **todo** implicit-line, self-adjoint versions
                      ! Both 2nd and 4th order dissipation
                     if( computeMatrix.eq.1 .and. computeRHS.eq.1 )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                                adCoeff2 = ad21+cd22*( abs(ux22(i1,i2,
     & i3,uc))+abs(uy22(i1,i2,i3,uc))+abs(ux22(i1,i2,i3,vc))+abs(uy22(
     & i1,i2,i3,vc)) )
                            ! Artficial Dissipation  **todo** implicit-line, self-adjoint versions
                                adCoeff4 = ad41+cd42*( abs(ux22(i1,i2,
     & i3,uc))+abs(uy22(i1,i2,i3,uc))+abs(ux22(i1,i2,i3,vc))+abs(uy22(
     & i1,i2,i3,vc)) )
                           t1=(uu(uc)*rxi(1,0)+uu(vc)*rxi(1,1)-nu*(rxx(
     & 1,0)+rxy(1,1)))*drv2i(1)
                           t2=nu*(rxi(1,0)**2+rxi(1,1)**2)*drvsqi(1)
                           am(i1,i2,i3)=adCoeff4
                           bm(i1,i2,i3)=-t1-t2-adCoeff2-4.*adCoeff4
                           cm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*(t2+nu*
     & (rxi(dirp1,0)**2+rxi(dirp1,1)**2)*drvsqi(dirp1))+4.*adCoeff2+
     & 12.*adCoeff4
                           dm(i1,i2,i3)=t1-t2-adCoeff2-4.*adCoeff4
                           em(i1,i2,i3)=adCoeff4


c rhs equations:
                           f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fc2d1(uc)-
     & ux2c(pc)+adCoeff2*(u(i1-1,i2,i3,uc)+u(i1+1,i2,i3,uc))+adCoeff4*
     & (-u(i1-2,i2,i3,uc)-u(i1+2,i2,i3,uc)+4.*(u(i1-1,i2,i3,uc)+u(i1+
     & 1,i2,i3,uc)))
                           f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fc2d1(vc)-
     & uy2c(pc)+adCoeff2*(u(i1-1,i2,i3,vc)+u(i1+1,i2,i3,vc))+adCoeff4*
     & (-u(i1-2,i2,i3,vc)-u(i1+2,i2,i3,vc)+4.*(u(i1-1,i2,i3,vc)+u(i1+
     & 1,i2,i3,vc)))

                         else
c for interpolation points or unused:
                           am(i1,i2,i3)=0.
                           bm(i1,i2,i3)=0.
                           cm(i1,i2,i3)=1.
                           dm(i1,i2,i3)=0.
                           em(i1,i2,i3)=0.
                           f(i1,i2,i3,fcu)=uu(uc)
                           f(i1,i2,i3,fcv)=uu(vc)
                         end if
                       end do
                       end do
                       end do
                     else if( computeMatrix.eq.1 )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                                adCoeff2 = ad21+cd22*( abs(ux22(i1,i2,
     & i3,uc))+abs(uy22(i1,i2,i3,uc))+abs(ux22(i1,i2,i3,vc))+abs(uy22(
     & i1,i2,i3,vc)) )
                            ! Artficial Dissipation  **todo** implicit-line, self-adjoint versions
                                adCoeff4 = ad41+cd42*( abs(ux22(i1,i2,
     & i3,uc))+abs(uy22(i1,i2,i3,uc))+abs(ux22(i1,i2,i3,vc))+abs(uy22(
     & i1,i2,i3,vc)) )
                           t1=(uu(uc)*rxi(1,0)+uu(vc)*rxi(1,1)-nu*(rxx(
     & 1,0)+rxy(1,1)))*drv2i(1)
                           t2=nu*(rxi(1,0)**2+rxi(1,1)**2)*drvsqi(1)
                           am(i1,i2,i3)=adCoeff4
                           bm(i1,i2,i3)=-t1-t2-adCoeff2-4.*adCoeff4
                           cm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*(t2+nu*
     & (rxi(dirp1,0)**2+rxi(dirp1,1)**2)*drvsqi(dirp1))+4.*adCoeff2+
     & 12.*adCoeff4
                           dm(i1,i2,i3)=t1-t2-adCoeff2-4.*adCoeff4
                           em(i1,i2,i3)=adCoeff4


                         else
c for interpolation points or unused:
                           am(i1,i2,i3)=0.
                           bm(i1,i2,i3)=0.
                           cm(i1,i2,i3)=1.
                           dm(i1,i2,i3)=0.
                           em(i1,i2,i3)=0.
                         end if
                       end do
                       end do
                       end do
                     else if( computeRHS.eq.1 )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
c rhs equations:
                           f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fc2d1(uc)-
     & ux2c(pc)+adCoeff2*(u(i1-1,i2,i3,uc)+u(i1+1,i2,i3,uc))+adCoeff4*
     & (-u(i1-2,i2,i3,uc)-u(i1+2,i2,i3,uc)+4.*(u(i1-1,i2,i3,uc)+u(i1+
     & 1,i2,i3,uc)))
                           f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fc2d1(vc)-
     & uy2c(pc)+adCoeff2*(u(i1-1,i2,i3,vc)+u(i1+1,i2,i3,vc))+adCoeff4*
     & (-u(i1-2,i2,i3,vc)-u(i1+2,i2,i3,vc)+4.*(u(i1-1,i2,i3,vc)+u(i1+
     & 1,i2,i3,vc)))

                         else
c for interpolation points or unused:
                           f(i1,i2,i3,fcu)=uu(uc)
                           f(i1,i2,i3,fcv)=uu(vc)
                         end if
                       end do
                       end do
                       end do
                     end if
                   else
                     stop 2222
                   end if
                 else if( nd.eq.3 )then
                   if( use2ndOrderAD.eq.0 .and. use4thOrderAD.eq.0 )
     & then
                     if( computeMatrix.eq.1 .and. computeRHS.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                        t1=(uu(uc)*rxi(1,0)+uu(vc)*rxi(1,1)+uu(wc)*rxi(
     & 1,2)-nu*(rxx3(1,0)+rxy3(1,1)+rxz3(1,2)))*drv2i(1)
                        t2=nu*(rxi(1,0)**2+rxi(1,1)**2+rxi(1,2)**2)*
     & drvsqi(1)
                        am(i1,i2,i3)=-t1-t2
                        bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*(t2+nu*((
     & rxi(dirp1,0)**2+rxi(dirp1,1)**2+rxi(dirp1,2)**2)*drvsqi(dirp1)+
     & (rxi(dirp2,0)**2+rxi(dirp2,1)**2+rxi(dirp2,2)**2)*drvsqi(dirp2)
     & ))
                        cm(i1,i2,i3)=t1-t2





c rhs equations:
                        f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fc3d1(uc)-ux3c(
     & pc)
                        f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fc3d1(vc)-uy3c(
     & pc)
                        f(i1,i2,i3,fcw)=f(i1,i2,i3,fcw)+fc3d1(wc)-uz3c(
     & pc)
                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                        f(i1,i2,i3,fcw)=uu(wc)
                       end if
                      end do
                      end do
                      end do
                     else if( computeMatrix.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                        t1=(uu(uc)*rxi(1,0)+uu(vc)*rxi(1,1)+uu(wc)*rxi(
     & 1,2)-nu*(rxx3(1,0)+rxy3(1,1)+rxz3(1,2)))*drv2i(1)
                        t2=nu*(rxi(1,0)**2+rxi(1,1)**2+rxi(1,2)**2)*
     & drvsqi(1)
                        am(i1,i2,i3)=-t1-t2
                        bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*(t2+nu*((
     & rxi(dirp1,0)**2+rxi(dirp1,1)**2+rxi(dirp1,2)**2)*drvsqi(dirp1)+
     & (rxi(dirp2,0)**2+rxi(dirp2,1)**2+rxi(dirp2,2)**2)*drvsqi(dirp2)
     & ))
                        cm(i1,i2,i3)=t1-t2





                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                       end if
                      end do
                      end do
                      end do
                     else if( computeRHS.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c rhs equations:
                        f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fc3d1(uc)-ux3c(
     & pc)
                        f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fc3d1(vc)-uy3c(
     & pc)
                        f(i1,i2,i3,fcw)=f(i1,i2,i3,fcw)+fc3d1(wc)-uz3c(
     & pc)
                       else
c for interpolation points or unused:
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                        f(i1,i2,i3,fcw)=uu(wc)
                       end if
                      end do
                      end do
                      end do
                     end if
                   else if( use2ndOrderAD.eq.1 .and. 
     & use4thOrderAD.eq.0 )then
                     if( computeMatrix.eq.1 .and. computeRHS.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                            cdmzz=admzzC(i1  ,i2  ,i3  )
                            cdpzz=admzzC(i1+1,i2  ,i3  )
                            cdzmz=adzmzC(i1  ,i2  ,i3  )
                            cdzpz=adzmzC(i1  ,i2+1,i3  )
                            cdzzm=adzzmC(i1  ,i2  ,i3  )
                            cdzzp=adzzmC(i1  ,i2  ,i3+1)
                            cdDiag=cdmzz+cdpzz+cdzmz+cdzpz+cdzzm+cdzzp
                              cdm=cdzmz
                              cdp=cdzpz
                        t1=(uu(uc)*rxi(1,0)+uu(vc)*rxi(1,1)+uu(wc)*rxi(
     & 1,2)-nu*(rxx3(1,0)+rxy3(1,1)+rxz3(1,2)))*drv2i(1)
                        t2=nu*(rxi(1,0)**2+rxi(1,1)**2+rxi(1,2)**2)*
     & drvsqi(1)
                        am(i1,i2,i3)=-t1-t2-cdm
                        bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*(t2+nu*((
     & rxi(dirp1,0)**2+rxi(dirp1,1)**2+rxi(dirp1,2)**2)*drvsqi(dirp1)+
     & (rxi(dirp2,0)**2+rxi(dirp2,1)**2+rxi(dirp2,2)**2)*drvsqi(dirp2)
     & ))+cdDiag
                        cm(i1,i2,i3)=t1-t2-cdp




c rhs equations:
                        f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fc3d1(uc)-ux3c(
     & pc)+adE3d1(i1,i2,i3,uc)
                        f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fc3d1(vc)-uy3c(
     & pc)+adE3d1(i1,i2,i3,vc)
                        f(i1,i2,i3,fcw)=f(i1,i2,i3,fcw)+fc3d1(wc)-uz3c(
     & pc)+adE3d1(i1,i2,i3,wc)
                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                        f(i1,i2,i3,fcw)=uu(wc)
                       end if
                      end do
                      end do
                      end do
                     else if( computeMatrix.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                            cdmzz=admzzC(i1  ,i2  ,i3  )
                            cdpzz=admzzC(i1+1,i2  ,i3  )
                            cdzmz=adzmzC(i1  ,i2  ,i3  )
                            cdzpz=adzmzC(i1  ,i2+1,i3  )
                            cdzzm=adzzmC(i1  ,i2  ,i3  )
                            cdzzp=adzzmC(i1  ,i2  ,i3+1)
                            cdDiag=cdmzz+cdpzz+cdzmz+cdzpz+cdzzm+cdzzp
                              cdm=cdzmz
                              cdp=cdzpz
                        t1=(uu(uc)*rxi(1,0)+uu(vc)*rxi(1,1)+uu(wc)*rxi(
     & 1,2)-nu*(rxx3(1,0)+rxy3(1,1)+rxz3(1,2)))*drv2i(1)
                        t2=nu*(rxi(1,0)**2+rxi(1,1)**2+rxi(1,2)**2)*
     & drvsqi(1)
                        am(i1,i2,i3)=-t1-t2-cdm
                        bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*(t2+nu*((
     & rxi(dirp1,0)**2+rxi(dirp1,1)**2+rxi(dirp1,2)**2)*drvsqi(dirp1)+
     & (rxi(dirp2,0)**2+rxi(dirp2,1)**2+rxi(dirp2,2)**2)*drvsqi(dirp2)
     & ))+cdDiag
                        cm(i1,i2,i3)=t1-t2-cdp




                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                       end if
                      end do
                      end do
                      end do
                     else if( computeRHS.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c rhs equations:
                        f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fc3d1(uc)-ux3c(
     & pc)+adE3d1(i1,i2,i3,uc)
                        f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fc3d1(vc)-uy3c(
     & pc)+adE3d1(i1,i2,i3,vc)
                        f(i1,i2,i3,fcw)=f(i1,i2,i3,fcw)+fc3d1(wc)-uz3c(
     & pc)+adE3d1(i1,i2,i3,wc)
                       else
c for interpolation points or unused:
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                        f(i1,i2,i3,fcw)=uu(wc)
                       end if
                      end do
                      end do
                      end do
                     end if
                   else if( use4thOrderAD.eq.1 )then
                      ! By default there is no AD:
                      ! 2nd-order artificial dissipation
                      ! 4th-order artficial dissipation  **todo** implicit-line, self-adjoint versions
                      ! Both 2nd and 4th order dissipation
                     if( computeMatrix.eq.1 .and. computeRHS.eq.1 )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                                adCoeff2 = ad21+cd22*( abs(ux23(i1,i2,
     & i3,uc))+abs(uy23(i1,i2,i3,uc))+abs(uz23(i1,i2,i3,uc))+abs(ux23(
     & i1,i2,i3,vc))+abs(uy23(i1,i2,i3,vc))+abs(uz23(i1,i2,i3,vc))+
     & abs(ux23(i1,i2,i3,wc))+abs(uy23(i1,i2,i3,wc))+abs(uz23(i1,i2,
     & i3,wc)))
                            ! Artficial Dissipation  **todo** implicit-line, self-adjoint versions
                                adCoeff4 = ad41+cd42*( abs(ux23(i1,i2,
     & i3,uc))+abs(uy23(i1,i2,i3,uc))+abs(uz23(i1,i2,i3,uc))+abs(ux23(
     & i1,i2,i3,vc))+abs(uy23(i1,i2,i3,vc))+abs(uz23(i1,i2,i3,vc))+
     & abs(ux23(i1,i2,i3,wc))+abs(uy23(i1,i2,i3,wc))+abs(uz23(i1,i2,
     & i3,wc)))
                           t1=(uu(uc)*rxi(1,0)+uu(vc)*rxi(1,1)+uu(wc)*
     & rxi(1,2)-nu*(rxx3(1,0)+rxy3(1,1)+rxz3(1,2)))*drv2i(1)
                           t2=nu*(rxi(1,0)**2+rxi(1,1)**2+rxi(1,2)**2)*
     & drvsqi(1)
                           am(i1,i2,i3)=adCoeff4
                           bm(i1,i2,i3)=-t1-t2-adCoeff2-4.*adCoeff4
                           cm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*(t2+nu*
     & ((rxi(dirp1,0)**2+rxi(dirp1,1)**2+rxi(dirp1,2)**2)*drvsqi(
     & dirp1)+(rxi(dirp2,0)**2+rxi(dirp2,1)**2+rxi(dirp2,2)**2)*
     & drvsqi(dirp2)))+6.*adCoeff2+18.*adCoeff4
                           dm(i1,i2,i3)=t1-t2-adCoeff2-4.*adCoeff4
                           em(i1,i2,i3)=adCoeff4


c rhs equations:
                           f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fc3d1(uc)-
     & ux3c(pc)+adCoeff2*(u(i1-1,i2,i3,uc)+u(i1+1,i2,i3,uc)+u(i1,i2,
     & i3-1,uc)+u(i1,i2,i3+1,uc))+adCoeff4*(-u(i1-2,i2,i3,uc)-u(i1+2,
     & i2,i3,uc)-u(i1,i2,i3-2,uc)-u(i1,i2,i3+2,uc)+4.*(u(i1-1,i2,i3,
     & uc)+u(i1+1,i2,i3,uc)+u(i1,i2,i3-1,uc)+u(i1,i2,i3+1,uc)))
                           f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fc3d1(vc)-
     & uy3c(pc)+adCoeff2*(u(i1-1,i2,i3,vc)+u(i1+1,i2,i3,vc)+u(i1,i2,
     & i3-1,vc)+u(i1,i2,i3+1,vc))+adCoeff4*(-u(i1-2,i2,i3,vc)-u(i1+2,
     & i2,i3,vc)-u(i1,i2,i3-2,vc)-u(i1,i2,i3+2,vc)+4.*(u(i1-1,i2,i3,
     & vc)+u(i1+1,i2,i3,vc)+u(i1,i2,i3-1,vc)+u(i1,i2,i3+1,vc)))
                           f(i1,i2,i3,fcw)=f(i1,i2,i3,fcw)+fc3d1(wc)-
     & uz3c(pc)+adCoeff2*(u(i1-1,i2,i3,wc)+u(i1+1,i2,i3,wc)+u(i1,i2,
     & i3-1,wc)+u(i1,i2,i3+1,wc))+adCoeff4*(-u(i1-2,i2,i3,wc)-u(i1+2,
     & i2,i3,wc)-u(i1,i2,i3-2,wc)-u(i1,i2,i3+2,wc)+4.*(u(i1-1,i2,i3,
     & wc)+u(i1+1,i2,i3,wc)+u(i1,i2,i3-1,wc)+u(i1,i2,i3+1,wc)))
                         else
c for interpolation points or unused:
                           am(i1,i2,i3)=0.
                           bm(i1,i2,i3)=0.
                           cm(i1,i2,i3)=1.
                           dm(i1,i2,i3)=0.
                           em(i1,i2,i3)=0.
                           f(i1,i2,i3,fcu)=uu(uc)
                           f(i1,i2,i3,fcv)=uu(vc)
                           f(i1,i2,i3,fcw)=uu(wc)
                         end if
                       end do
                       end do
                       end do
                     else if( computeMatrix.eq.1 )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                                adCoeff2 = ad21+cd22*( abs(ux23(i1,i2,
     & i3,uc))+abs(uy23(i1,i2,i3,uc))+abs(uz23(i1,i2,i3,uc))+abs(ux23(
     & i1,i2,i3,vc))+abs(uy23(i1,i2,i3,vc))+abs(uz23(i1,i2,i3,vc))+
     & abs(ux23(i1,i2,i3,wc))+abs(uy23(i1,i2,i3,wc))+abs(uz23(i1,i2,
     & i3,wc)))
                            ! Artficial Dissipation  **todo** implicit-line, self-adjoint versions
                                adCoeff4 = ad41+cd42*( abs(ux23(i1,i2,
     & i3,uc))+abs(uy23(i1,i2,i3,uc))+abs(uz23(i1,i2,i3,uc))+abs(ux23(
     & i1,i2,i3,vc))+abs(uy23(i1,i2,i3,vc))+abs(uz23(i1,i2,i3,vc))+
     & abs(ux23(i1,i2,i3,wc))+abs(uy23(i1,i2,i3,wc))+abs(uz23(i1,i2,
     & i3,wc)))
                           t1=(uu(uc)*rxi(1,0)+uu(vc)*rxi(1,1)+uu(wc)*
     & rxi(1,2)-nu*(rxx3(1,0)+rxy3(1,1)+rxz3(1,2)))*drv2i(1)
                           t2=nu*(rxi(1,0)**2+rxi(1,1)**2+rxi(1,2)**2)*
     & drvsqi(1)
                           am(i1,i2,i3)=adCoeff4
                           bm(i1,i2,i3)=-t1-t2-adCoeff2-4.*adCoeff4
                           cm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*(t2+nu*
     & ((rxi(dirp1,0)**2+rxi(dirp1,1)**2+rxi(dirp1,2)**2)*drvsqi(
     & dirp1)+(rxi(dirp2,0)**2+rxi(dirp2,1)**2+rxi(dirp2,2)**2)*
     & drvsqi(dirp2)))+6.*adCoeff2+18.*adCoeff4
                           dm(i1,i2,i3)=t1-t2-adCoeff2-4.*adCoeff4
                           em(i1,i2,i3)=adCoeff4


                         else
c for interpolation points or unused:
                           am(i1,i2,i3)=0.
                           bm(i1,i2,i3)=0.
                           cm(i1,i2,i3)=1.
                           dm(i1,i2,i3)=0.
                           em(i1,i2,i3)=0.
                         end if
                       end do
                       end do
                       end do
                     else if( computeRHS.eq.1 )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
c rhs equations:
                           f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fc3d1(uc)-
     & ux3c(pc)+adCoeff2*(u(i1-1,i2,i3,uc)+u(i1+1,i2,i3,uc)+u(i1,i2,
     & i3-1,uc)+u(i1,i2,i3+1,uc))+adCoeff4*(-u(i1-2,i2,i3,uc)-u(i1+2,
     & i2,i3,uc)-u(i1,i2,i3-2,uc)-u(i1,i2,i3+2,uc)+4.*(u(i1-1,i2,i3,
     & uc)+u(i1+1,i2,i3,uc)+u(i1,i2,i3-1,uc)+u(i1,i2,i3+1,uc)))
                           f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fc3d1(vc)-
     & uy3c(pc)+adCoeff2*(u(i1-1,i2,i3,vc)+u(i1+1,i2,i3,vc)+u(i1,i2,
     & i3-1,vc)+u(i1,i2,i3+1,vc))+adCoeff4*(-u(i1-2,i2,i3,vc)-u(i1+2,
     & i2,i3,vc)-u(i1,i2,i3-2,vc)-u(i1,i2,i3+2,vc)+4.*(u(i1-1,i2,i3,
     & vc)+u(i1+1,i2,i3,vc)+u(i1,i2,i3-1,vc)+u(i1,i2,i3+1,vc)))
                           f(i1,i2,i3,fcw)=f(i1,i2,i3,fcw)+fc3d1(wc)-
     & uz3c(pc)+adCoeff2*(u(i1-1,i2,i3,wc)+u(i1+1,i2,i3,wc)+u(i1,i2,
     & i3-1,wc)+u(i1,i2,i3+1,wc))+adCoeff4*(-u(i1-2,i2,i3,wc)-u(i1+2,
     & i2,i3,wc)-u(i1,i2,i3-2,wc)-u(i1,i2,i3+2,wc)+4.*(u(i1-1,i2,i3,
     & wc)+u(i1+1,i2,i3,wc)+u(i1,i2,i3-1,wc)+u(i1,i2,i3+1,wc)))
                         else
c for interpolation points or unused:
                           f(i1,i2,i3,fcu)=uu(uc)
                           f(i1,i2,i3,fcv)=uu(vc)
                           f(i1,i2,i3,fcw)=uu(wc)
                         end if
                       end do
                       end do
                       end do
                     end if
                   else
                     stop 33333
                   end if
                 else
                   stop 222
                 end if
               else ! dir.eq.2
                 dirp1=mod(2+1,nd)
                 dirp2=mod(2+2,nd)
c  ****** Incompressible NS, No turbulence model *****
                 if( nd.eq.2 )then
                   if( use2ndOrderAD.eq.0 .and. use4thOrderAD.eq.0 )
     & then
                     if( computeMatrix.eq.1 .and. computeRHS.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                        t1=(uu(uc)*rxi(2,0)+uu(vc)*rxi(2,1)-nu*(rxx(2,
     & 0)+rxy(2,1)))*drv2i(2)
                        t2=nu*(rxi(2,0)**2+rxi(2,1)**2)*drvsqi(2)
                        am(i1,i2,i3)=-t1-t2
                        bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*(t2+nu*(
     & rxi(dirp1,0)**2+rxi(dirp1,1)**2)*drvsqi(dirp1))
                        cm(i1,i2,i3)=t1-t2





c rhs equations:
                        f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fc2d2(uc)-ux2c(
     & pc)
                        f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fc2d2(vc)-uy2c(
     & pc)

                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                       end if
                      end do
                      end do
                      end do
                     else if( computeMatrix.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                        t1=(uu(uc)*rxi(2,0)+uu(vc)*rxi(2,1)-nu*(rxx(2,
     & 0)+rxy(2,1)))*drv2i(2)
                        t2=nu*(rxi(2,0)**2+rxi(2,1)**2)*drvsqi(2)
                        am(i1,i2,i3)=-t1-t2
                        bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*(t2+nu*(
     & rxi(dirp1,0)**2+rxi(dirp1,1)**2)*drvsqi(dirp1))
                        cm(i1,i2,i3)=t1-t2





                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                       end if
                      end do
                      end do
                      end do
                     else if( computeRHS.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c rhs equations:
                        f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fc2d2(uc)-ux2c(
     & pc)
                        f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fc2d2(vc)-uy2c(
     & pc)

                       else
c for interpolation points or unused:
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                       end if
                      end do
                      end do
                      end do
                     end if
                   else if( use2ndOrderAD.eq.1 .and. 
     & use4thOrderAD.eq.0 )then
                     if( computeMatrix.eq.1 .and. computeRHS.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                            cdmz=admzC(i1  ,i2  ,i3)
                            cdpz=admzC(i1+1,i2  ,i3)
                            cdzm=adzmC(i1  ,i2  ,i3)
                            cdzp=adzmC(i1  ,i2+1,i3)
                            ! write(*,'(1x,''insLS:i1,i2,cdmz,cdzm='',2i3,2f9.3)') i1,i2,cdmz,cdzm
                            ! cdmz=0.
                            ! cdpz=0.
                            ! cdzm=0.
                            ! cdzp=0.
                            cdDiag=cdmz+cdpz+cdzm+cdzp
                              stop 1234
                        t1=(uu(uc)*rxi(2,0)+uu(vc)*rxi(2,1)-nu*(rxx(2,
     & 0)+rxy(2,1)))*drv2i(2)
                        t2=nu*(rxi(2,0)**2+rxi(2,1)**2)*drvsqi(2)
                        am(i1,i2,i3)=-t1-t2-cdm
                        bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*(t2+nu*(
     & rxi(dirp1,0)**2+rxi(dirp1,1)**2)*drvsqi(dirp1))+cdDiag
                        cm(i1,i2,i3)=t1-t2-cdp




c rhs equations:
                        f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fc2d2(uc)-ux2c(
     & pc)+adE2(i1,i2,i3,uc)
                        f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fc2d2(vc)-uy2c(
     & pc)+adE2(i1,i2,i3,vc)

                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                       end if
                      end do
                      end do
                      end do
                     else if( computeMatrix.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                            cdmz=admzC(i1  ,i2  ,i3)
                            cdpz=admzC(i1+1,i2  ,i3)
                            cdzm=adzmC(i1  ,i2  ,i3)
                            cdzp=adzmC(i1  ,i2+1,i3)
                            ! write(*,'(1x,''insLS:i1,i2,cdmz,cdzm='',2i3,2f9.3)') i1,i2,cdmz,cdzm
                            ! cdmz=0.
                            ! cdpz=0.
                            ! cdzm=0.
                            ! cdzp=0.
                            cdDiag=cdmz+cdpz+cdzm+cdzp
                              stop 1234
                        t1=(uu(uc)*rxi(2,0)+uu(vc)*rxi(2,1)-nu*(rxx(2,
     & 0)+rxy(2,1)))*drv2i(2)
                        t2=nu*(rxi(2,0)**2+rxi(2,1)**2)*drvsqi(2)
                        am(i1,i2,i3)=-t1-t2-cdm
                        bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*(t2+nu*(
     & rxi(dirp1,0)**2+rxi(dirp1,1)**2)*drvsqi(dirp1))+cdDiag
                        cm(i1,i2,i3)=t1-t2-cdp




                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                       end if
                      end do
                      end do
                      end do
                     else if( computeRHS.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c rhs equations:
                        f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fc2d2(uc)-ux2c(
     & pc)+adE2(i1,i2,i3,uc)
                        f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fc2d2(vc)-uy2c(
     & pc)+adE2(i1,i2,i3,vc)

                       else
c for interpolation points or unused:
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                       end if
                      end do
                      end do
                      end do
                     end if
                   else if( use4thOrderAD.eq.1 )then
                         stop 676
                      ! By default there is no AD:
                      ! 2nd-order artificial dissipation
                      ! 4th-order artficial dissipation  **todo** implicit-line, self-adjoint versions
                      ! Both 2nd and 4th order dissipation
                     if( computeMatrix.eq.1 .and. computeRHS.eq.1 )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                                adCoeff2 = ad21+cd22*( abs(ux22(i1,i2,
     & i3,uc))+abs(uy22(i1,i2,i3,uc))+abs(ux22(i1,i2,i3,vc))+abs(uy22(
     & i1,i2,i3,vc)) )
                            ! Artficial Dissipation  **todo** implicit-line, self-adjoint versions
                                adCoeff4 = ad41+cd42*( abs(ux22(i1,i2,
     & i3,uc))+abs(uy22(i1,i2,i3,uc))+abs(ux22(i1,i2,i3,vc))+abs(uy22(
     & i1,i2,i3,vc)) )
                           t1=(uu(uc)*rxi(2,0)+uu(vc)*rxi(2,1)-nu*(rxx(
     & 2,0)+rxy(2,1)))*drv2i(2)
                           t2=nu*(rxi(2,0)**2+rxi(2,1)**2)*drvsqi(2)
                           am(i1,i2,i3)=adCoeff4
                           bm(i1,i2,i3)=-t1-t2-adCoeff2-4.*adCoeff4
                           cm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*(t2+nu*
     & (rxi(dirp1,0)**2+rxi(dirp1,1)**2)*drvsqi(dirp1))+4.*adCoeff2+
     & 12.*adCoeff4
                           dm(i1,i2,i3)=t1-t2-adCoeff2-4.*adCoeff4
                           em(i1,i2,i3)=adCoeff4


c rhs equations:
                           f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fc2d2(uc)-
     & ux2c(pc)+adCoeff2*(u(i1-1,i2,i3,uc)+u(i1+1,i2,i3,uc)+u(i1,i2,
     & i3-1,uc)+u(i1,i2,i3+1,uc))+adCoeff4*(-u(i1-2,i2,i3,uc)-u(i1+2,
     & i2,i3,uc)-u(i1,i2,i3-2,uc)-u(i1,i2,i3+2,uc)+4.*(u(i1-1,i2,i3,
     & uc)+u(i1+1,i2,i3,uc)+u(i1,i2,i3-1,uc)+u(i1,i2,i3+1,uc)))
                           f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fc2d2(vc)-
     & uy2c(pc)+adCoeff2*(u(i1-1,i2,i3,vc)+u(i1+1,i2,i3,vc)+u(i1,i2,
     & i3-1,vc)+u(i1,i2,i3+1,vc))+adCoeff4*(-u(i1-2,i2,i3,vc)-u(i1+2,
     & i2,i3,vc)-u(i1,i2,i3-2,vc)-u(i1,i2,i3+2,vc)+4.*(u(i1-1,i2,i3,
     & vc)+u(i1+1,i2,i3,vc)+u(i1,i2,i3-1,vc)+u(i1,i2,i3+1,vc)))

                         else
c for interpolation points or unused:
                           am(i1,i2,i3)=0.
                           bm(i1,i2,i3)=0.
                           cm(i1,i2,i3)=1.
                           dm(i1,i2,i3)=0.
                           em(i1,i2,i3)=0.
                           f(i1,i2,i3,fcu)=uu(uc)
                           f(i1,i2,i3,fcv)=uu(vc)
                         end if
                       end do
                       end do
                       end do
                     else if( computeMatrix.eq.1 )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                                adCoeff2 = ad21+cd22*( abs(ux22(i1,i2,
     & i3,uc))+abs(uy22(i1,i2,i3,uc))+abs(ux22(i1,i2,i3,vc))+abs(uy22(
     & i1,i2,i3,vc)) )
                            ! Artficial Dissipation  **todo** implicit-line, self-adjoint versions
                                adCoeff4 = ad41+cd42*( abs(ux22(i1,i2,
     & i3,uc))+abs(uy22(i1,i2,i3,uc))+abs(ux22(i1,i2,i3,vc))+abs(uy22(
     & i1,i2,i3,vc)) )
                           t1=(uu(uc)*rxi(2,0)+uu(vc)*rxi(2,1)-nu*(rxx(
     & 2,0)+rxy(2,1)))*drv2i(2)
                           t2=nu*(rxi(2,0)**2+rxi(2,1)**2)*drvsqi(2)
                           am(i1,i2,i3)=adCoeff4
                           bm(i1,i2,i3)=-t1-t2-adCoeff2-4.*adCoeff4
                           cm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*(t2+nu*
     & (rxi(dirp1,0)**2+rxi(dirp1,1)**2)*drvsqi(dirp1))+4.*adCoeff2+
     & 12.*adCoeff4
                           dm(i1,i2,i3)=t1-t2-adCoeff2-4.*adCoeff4
                           em(i1,i2,i3)=adCoeff4


                         else
c for interpolation points or unused:
                           am(i1,i2,i3)=0.
                           bm(i1,i2,i3)=0.
                           cm(i1,i2,i3)=1.
                           dm(i1,i2,i3)=0.
                           em(i1,i2,i3)=0.
                         end if
                       end do
                       end do
                       end do
                     else if( computeRHS.eq.1 )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
c rhs equations:
                           f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fc2d2(uc)-
     & ux2c(pc)+adCoeff2*(u(i1-1,i2,i3,uc)+u(i1+1,i2,i3,uc)+u(i1,i2,
     & i3-1,uc)+u(i1,i2,i3+1,uc))+adCoeff4*(-u(i1-2,i2,i3,uc)-u(i1+2,
     & i2,i3,uc)-u(i1,i2,i3-2,uc)-u(i1,i2,i3+2,uc)+4.*(u(i1-1,i2,i3,
     & uc)+u(i1+1,i2,i3,uc)+u(i1,i2,i3-1,uc)+u(i1,i2,i3+1,uc)))
                           f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fc2d2(vc)-
     & uy2c(pc)+adCoeff2*(u(i1-1,i2,i3,vc)+u(i1+1,i2,i3,vc)+u(i1,i2,
     & i3-1,vc)+u(i1,i2,i3+1,vc))+adCoeff4*(-u(i1-2,i2,i3,vc)-u(i1+2,
     & i2,i3,vc)-u(i1,i2,i3-2,vc)-u(i1,i2,i3+2,vc)+4.*(u(i1-1,i2,i3,
     & vc)+u(i1+1,i2,i3,vc)+u(i1,i2,i3-1,vc)+u(i1,i2,i3+1,vc)))

                         else
c for interpolation points or unused:
                           f(i1,i2,i3,fcu)=uu(uc)
                           f(i1,i2,i3,fcv)=uu(vc)
                         end if
                       end do
                       end do
                       end do
                     end if
                   else
                     stop 2222
                   end if
                 else if( nd.eq.3 )then
                   if( use2ndOrderAD.eq.0 .and. use4thOrderAD.eq.0 )
     & then
                     if( computeMatrix.eq.1 .and. computeRHS.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                        t1=(uu(uc)*rxi(2,0)+uu(vc)*rxi(2,1)+uu(wc)*rxi(
     & 2,2)-nu*(rxx3(2,0)+rxy3(2,1)+rxz3(2,2)))*drv2i(2)
                        t2=nu*(rxi(2,0)**2+rxi(2,1)**2+rxi(2,2)**2)*
     & drvsqi(2)
                        am(i1,i2,i3)=-t1-t2
                        bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*(t2+nu*((
     & rxi(dirp1,0)**2+rxi(dirp1,1)**2+rxi(dirp1,2)**2)*drvsqi(dirp1)+
     & (rxi(dirp2,0)**2+rxi(dirp2,1)**2+rxi(dirp2,2)**2)*drvsqi(dirp2)
     & ))
                        cm(i1,i2,i3)=t1-t2





c rhs equations:
                        f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fc3d2(uc)-ux3c(
     & pc)
                        f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fc3d2(vc)-uy3c(
     & pc)
                        f(i1,i2,i3,fcw)=f(i1,i2,i3,fcw)+fc3d2(wc)-uz3c(
     & pc)
                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                        f(i1,i2,i3,fcw)=uu(wc)
                       end if
                      end do
                      end do
                      end do
                     else if( computeMatrix.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                        t1=(uu(uc)*rxi(2,0)+uu(vc)*rxi(2,1)+uu(wc)*rxi(
     & 2,2)-nu*(rxx3(2,0)+rxy3(2,1)+rxz3(2,2)))*drv2i(2)
                        t2=nu*(rxi(2,0)**2+rxi(2,1)**2+rxi(2,2)**2)*
     & drvsqi(2)
                        am(i1,i2,i3)=-t1-t2
                        bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*(t2+nu*((
     & rxi(dirp1,0)**2+rxi(dirp1,1)**2+rxi(dirp1,2)**2)*drvsqi(dirp1)+
     & (rxi(dirp2,0)**2+rxi(dirp2,1)**2+rxi(dirp2,2)**2)*drvsqi(dirp2)
     & ))
                        cm(i1,i2,i3)=t1-t2





                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                       end if
                      end do
                      end do
                      end do
                     else if( computeRHS.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c rhs equations:
                        f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fc3d2(uc)-ux3c(
     & pc)
                        f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fc3d2(vc)-uy3c(
     & pc)
                        f(i1,i2,i3,fcw)=f(i1,i2,i3,fcw)+fc3d2(wc)-uz3c(
     & pc)
                       else
c for interpolation points or unused:
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                        f(i1,i2,i3,fcw)=uu(wc)
                       end if
                      end do
                      end do
                      end do
                     end if
                   else if( use2ndOrderAD.eq.1 .and. 
     & use4thOrderAD.eq.0 )then
                     if( computeMatrix.eq.1 .and. computeRHS.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                            cdmzz=admzzC(i1  ,i2  ,i3  )
                            cdpzz=admzzC(i1+1,i2  ,i3  )
                            cdzmz=adzmzC(i1  ,i2  ,i3  )
                            cdzpz=adzmzC(i1  ,i2+1,i3  )
                            cdzzm=adzzmC(i1  ,i2  ,i3  )
                            cdzzp=adzzmC(i1  ,i2  ,i3+1)
                            cdDiag=cdmzz+cdpzz+cdzmz+cdzpz+cdzzm+cdzzp
                              cdm=cdzzm
                              cdp=cdzzp
                        t1=(uu(uc)*rxi(2,0)+uu(vc)*rxi(2,1)+uu(wc)*rxi(
     & 2,2)-nu*(rxx3(2,0)+rxy3(2,1)+rxz3(2,2)))*drv2i(2)
                        t2=nu*(rxi(2,0)**2+rxi(2,1)**2+rxi(2,2)**2)*
     & drvsqi(2)
                        am(i1,i2,i3)=-t1-t2-cdm
                        bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*(t2+nu*((
     & rxi(dirp1,0)**2+rxi(dirp1,1)**2+rxi(dirp1,2)**2)*drvsqi(dirp1)+
     & (rxi(dirp2,0)**2+rxi(dirp2,1)**2+rxi(dirp2,2)**2)*drvsqi(dirp2)
     & ))+cdDiag
                        cm(i1,i2,i3)=t1-t2-cdp




c rhs equations:
                        f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fc3d2(uc)-ux3c(
     & pc)+adE3d2(i1,i2,i3,uc)
                        f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fc3d2(vc)-uy3c(
     & pc)+adE3d2(i1,i2,i3,vc)
                        f(i1,i2,i3,fcw)=f(i1,i2,i3,fcw)+fc3d2(wc)-uz3c(
     & pc)+adE3d2(i1,i2,i3,wc)
                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                        f(i1,i2,i3,fcw)=uu(wc)
                       end if
                      end do
                      end do
                      end do
                     else if( computeMatrix.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                            cdmzz=admzzC(i1  ,i2  ,i3  )
                            cdpzz=admzzC(i1+1,i2  ,i3  )
                            cdzmz=adzmzC(i1  ,i2  ,i3  )
                            cdzpz=adzmzC(i1  ,i2+1,i3  )
                            cdzzm=adzzmC(i1  ,i2  ,i3  )
                            cdzzp=adzzmC(i1  ,i2  ,i3+1)
                            cdDiag=cdmzz+cdpzz+cdzmz+cdzpz+cdzzm+cdzzp
                              cdm=cdzzm
                              cdp=cdzzp
                        t1=(uu(uc)*rxi(2,0)+uu(vc)*rxi(2,1)+uu(wc)*rxi(
     & 2,2)-nu*(rxx3(2,0)+rxy3(2,1)+rxz3(2,2)))*drv2i(2)
                        t2=nu*(rxi(2,0)**2+rxi(2,1)**2+rxi(2,2)**2)*
     & drvsqi(2)
                        am(i1,i2,i3)=-t1-t2-cdm
                        bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*(t2+nu*((
     & rxi(dirp1,0)**2+rxi(dirp1,1)**2+rxi(dirp1,2)**2)*drvsqi(dirp1)+
     & (rxi(dirp2,0)**2+rxi(dirp2,1)**2+rxi(dirp2,2)**2)*drvsqi(dirp2)
     & ))+cdDiag
                        cm(i1,i2,i3)=t1-t2-cdp




                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                       end if
                      end do
                      end do
                      end do
                     else if( computeRHS.eq.1 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
c rhs equations:
                        f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fc3d2(uc)-ux3c(
     & pc)+adE3d2(i1,i2,i3,uc)
                        f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fc3d2(vc)-uy3c(
     & pc)+adE3d2(i1,i2,i3,vc)
                        f(i1,i2,i3,fcw)=f(i1,i2,i3,fcw)+fc3d2(wc)-uz3c(
     & pc)+adE3d2(i1,i2,i3,wc)
                       else
c for interpolation points or unused:
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                        f(i1,i2,i3,fcw)=uu(wc)
                       end if
                      end do
                      end do
                      end do
                     end if
                   else if( use4thOrderAD.eq.1 )then
                      ! By default there is no AD:
                      ! 2nd-order artificial dissipation
                      ! 4th-order artficial dissipation  **todo** implicit-line, self-adjoint versions
                      ! Both 2nd and 4th order dissipation
                     if( computeMatrix.eq.1 .and. computeRHS.eq.1 )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                                adCoeff2 = ad21+cd22*( abs(ux23(i1,i2,
     & i3,uc))+abs(uy23(i1,i2,i3,uc))+abs(uz23(i1,i2,i3,uc))+abs(ux23(
     & i1,i2,i3,vc))+abs(uy23(i1,i2,i3,vc))+abs(uz23(i1,i2,i3,vc))+
     & abs(ux23(i1,i2,i3,wc))+abs(uy23(i1,i2,i3,wc))+abs(uz23(i1,i2,
     & i3,wc)))
                            ! Artficial Dissipation  **todo** implicit-line, self-adjoint versions
                                adCoeff4 = ad41+cd42*( abs(ux23(i1,i2,
     & i3,uc))+abs(uy23(i1,i2,i3,uc))+abs(uz23(i1,i2,i3,uc))+abs(ux23(
     & i1,i2,i3,vc))+abs(uy23(i1,i2,i3,vc))+abs(uz23(i1,i2,i3,vc))+
     & abs(ux23(i1,i2,i3,wc))+abs(uy23(i1,i2,i3,wc))+abs(uz23(i1,i2,
     & i3,wc)))
                           t1=(uu(uc)*rxi(2,0)+uu(vc)*rxi(2,1)+uu(wc)*
     & rxi(2,2)-nu*(rxx3(2,0)+rxy3(2,1)+rxz3(2,2)))*drv2i(2)
                           t2=nu*(rxi(2,0)**2+rxi(2,1)**2+rxi(2,2)**2)*
     & drvsqi(2)
                           am(i1,i2,i3)=adCoeff4
                           bm(i1,i2,i3)=-t1-t2-adCoeff2-4.*adCoeff4
                           cm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*(t2+nu*
     & ((rxi(dirp1,0)**2+rxi(dirp1,1)**2+rxi(dirp1,2)**2)*drvsqi(
     & dirp1)+(rxi(dirp2,0)**2+rxi(dirp2,1)**2+rxi(dirp2,2)**2)*
     & drvsqi(dirp2)))+6.*adCoeff2+18.*adCoeff4
                           dm(i1,i2,i3)=t1-t2-adCoeff2-4.*adCoeff4
                           em(i1,i2,i3)=adCoeff4


c rhs equations:
                           f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fc3d2(uc)-
     & ux3c(pc)+adCoeff2*(u(i1-1,i2,i3,uc)+u(i1+1,i2,i3,uc)+u(i1,i2-1,
     & i3,uc)+u(i1,i2+1,i3,uc))+adCoeff4*(-u(i1-2,i2,i3,uc)-u(i1+2,i2,
     & i3,uc)-u(i1,i2-2,i3,uc)-u(i1,i2+2,i3,uc)+4.*(u(i1-1,i2,i3,uc)+
     & u(i1+1,i2,i3,uc)+u(i1,i2-1,i3,uc)+u(i1,i2+1,i3,uc)))
                           f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fc3d2(vc)-
     & uy3c(pc)+adCoeff2*(u(i1-1,i2,i3,vc)+u(i1+1,i2,i3,vc)+u(i1,i2-1,
     & i3,vc)+u(i1,i2+1,i3,vc))+adCoeff4*(-u(i1-2,i2,i3,vc)-u(i1+2,i2,
     & i3,vc)-u(i1,i2-2,i3,vc)-u(i1,i2+2,i3,vc)+4.*(u(i1-1,i2,i3,vc)+
     & u(i1+1,i2,i3,vc)+u(i1,i2-1,i3,vc)+u(i1,i2+1,i3,vc)))
                           f(i1,i2,i3,fcw)=f(i1,i2,i3,fcw)+fc3d2(wc)-
     & uz3c(pc)+adCoeff2*(u(i1-1,i2,i3,wc)+u(i1+1,i2,i3,wc)+u(i1,i2-1,
     & i3,wc)+u(i1,i2+1,i3,wc))+adCoeff4*(-u(i1-2,i2,i3,wc)-u(i1+2,i2,
     & i3,wc)-u(i1,i2-2,i3,wc)-u(i1,i2+2,i3,wc)+4.*(u(i1-1,i2,i3,wc)+
     & u(i1+1,i2,i3,wc)+u(i1,i2-1,i3,wc)+u(i1,i2+1,i3,wc)))
                         else
c for interpolation points or unused:
                           am(i1,i2,i3)=0.
                           bm(i1,i2,i3)=0.
                           cm(i1,i2,i3)=1.
                           dm(i1,i2,i3)=0.
                           em(i1,i2,i3)=0.
                           f(i1,i2,i3,fcu)=uu(uc)
                           f(i1,i2,i3,fcv)=uu(vc)
                           f(i1,i2,i3,fcw)=uu(wc)
                         end if
                       end do
                       end do
                       end do
                     else if( computeMatrix.eq.1 )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                                adCoeff2 = ad21+cd22*( abs(ux23(i1,i2,
     & i3,uc))+abs(uy23(i1,i2,i3,uc))+abs(uz23(i1,i2,i3,uc))+abs(ux23(
     & i1,i2,i3,vc))+abs(uy23(i1,i2,i3,vc))+abs(uz23(i1,i2,i3,vc))+
     & abs(ux23(i1,i2,i3,wc))+abs(uy23(i1,i2,i3,wc))+abs(uz23(i1,i2,
     & i3,wc)))
                            ! Artficial Dissipation  **todo** implicit-line, self-adjoint versions
                                adCoeff4 = ad41+cd42*( abs(ux23(i1,i2,
     & i3,uc))+abs(uy23(i1,i2,i3,uc))+abs(uz23(i1,i2,i3,uc))+abs(ux23(
     & i1,i2,i3,vc))+abs(uy23(i1,i2,i3,vc))+abs(uz23(i1,i2,i3,vc))+
     & abs(ux23(i1,i2,i3,wc))+abs(uy23(i1,i2,i3,wc))+abs(uz23(i1,i2,
     & i3,wc)))
                           t1=(uu(uc)*rxi(2,0)+uu(vc)*rxi(2,1)+uu(wc)*
     & rxi(2,2)-nu*(rxx3(2,0)+rxy3(2,1)+rxz3(2,2)))*drv2i(2)
                           t2=nu*(rxi(2,0)**2+rxi(2,1)**2+rxi(2,2)**2)*
     & drvsqi(2)
                           am(i1,i2,i3)=adCoeff4
                           bm(i1,i2,i3)=-t1-t2-adCoeff2-4.*adCoeff4
                           cm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*(t2+nu*
     & ((rxi(dirp1,0)**2+rxi(dirp1,1)**2+rxi(dirp1,2)**2)*drvsqi(
     & dirp1)+(rxi(dirp2,0)**2+rxi(dirp2,1)**2+rxi(dirp2,2)**2)*
     & drvsqi(dirp2)))+6.*adCoeff2+18.*adCoeff4
                           dm(i1,i2,i3)=t1-t2-adCoeff2-4.*adCoeff4
                           em(i1,i2,i3)=adCoeff4


                         else
c for interpolation points or unused:
                           am(i1,i2,i3)=0.
                           bm(i1,i2,i3)=0.
                           cm(i1,i2,i3)=1.
                           dm(i1,i2,i3)=0.
                           em(i1,i2,i3)=0.
                         end if
                       end do
                       end do
                       end do
                     else if( computeRHS.eq.1 )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
c rhs equations:
                           f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fc3d2(uc)-
     & ux3c(pc)+adCoeff2*(u(i1-1,i2,i3,uc)+u(i1+1,i2,i3,uc)+u(i1,i2-1,
     & i3,uc)+u(i1,i2+1,i3,uc))+adCoeff4*(-u(i1-2,i2,i3,uc)-u(i1+2,i2,
     & i3,uc)-u(i1,i2-2,i3,uc)-u(i1,i2+2,i3,uc)+4.*(u(i1-1,i2,i3,uc)+
     & u(i1+1,i2,i3,uc)+u(i1,i2-1,i3,uc)+u(i1,i2+1,i3,uc)))
                           f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fc3d2(vc)-
     & uy3c(pc)+adCoeff2*(u(i1-1,i2,i3,vc)+u(i1+1,i2,i3,vc)+u(i1,i2-1,
     & i3,vc)+u(i1,i2+1,i3,vc))+adCoeff4*(-u(i1-2,i2,i3,vc)-u(i1+2,i2,
     & i3,vc)-u(i1,i2-2,i3,vc)-u(i1,i2+2,i3,vc)+4.*(u(i1-1,i2,i3,vc)+
     & u(i1+1,i2,i3,vc)+u(i1,i2-1,i3,vc)+u(i1,i2+1,i3,vc)))
                           f(i1,i2,i3,fcw)=f(i1,i2,i3,fcw)+fc3d2(wc)-
     & uz3c(pc)+adCoeff2*(u(i1-1,i2,i3,wc)+u(i1+1,i2,i3,wc)+u(i1,i2-1,
     & i3,wc)+u(i1,i2+1,i3,wc))+adCoeff4*(-u(i1-2,i2,i3,wc)-u(i1+2,i2,
     & i3,wc)-u(i1,i2-2,i3,wc)-u(i1,i2+2,i3,wc)+4.*(u(i1-1,i2,i3,wc)+
     & u(i1+1,i2,i3,wc)+u(i1,i2-1,i3,wc)+u(i1,i2+1,i3,wc)))
                         else
c for interpolation points or unused:
                           f(i1,i2,i3,fcu)=uu(uc)
                           f(i1,i2,i3,fcv)=uu(vc)
                           f(i1,i2,i3,fcw)=uu(wc)
                         end if
                       end do
                       end do
                       end do
                     end if
                   else
                     stop 33333
                   end if
                 else
                   stop 222
                 end if
               end if ! end dir
              else ! order==4
              end if
            else
              stop 111
            end if
        else if( option.eq.assignTemperature )then
         ! **************************************************************************
         ! Fill in the tridiagonal matrix for the INS Temperature equation 
         ! ***************************************************************************
           if( gridType.eq.rectangular )then
             ! *******************************************
             ! ************** rectangular  ***************
             ! *******************************************
             if( orderOfAccuracy.eq.2 )then
               if( dir.eq.0 )then
c  ****** INS Temperature equation **********
                if( nd.eq.2 )then
                  ! defineDerivativeMacros(DIM,ORDER,GRIDTYPE)
                  if( computeMatrix.eq.1 .and. computeRHS.eq.1 )then
                   do i3=n3a,n3b
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                    if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                     am(i1,i2,i3)=-uu(uc+0)*dxv2i(0)-kThermal*dxvsqi(0)
                     bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*kThermal*(
     & dxvsqi(0)+dxvsqi(1))
                     cm(i1,i2,i3)=uu(uc+0)*dxv2i(0)-kThermal*dxvsqi(0)







c rhs equations:
                     f(i1,i2,i3,fct)=f(i1,i2,i3,fct)+ftr2d0(tc)


                    else
c for interpolation points or unused:
                     am(i1,i2,i3)=0.
                     bm(i1,i2,i3)=1.
                     cm(i1,i2,i3)=0.
                     f(i1,i2,i3,fct)=uu(tc)
                    end if
                   end do
                   end do
                   end do
                  else if( computeMatrix.eq.1 )then
                   do i3=n3a,n3b
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                    if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                     am(i1,i2,i3)=-uu(uc+0)*dxv2i(0)-kThermal*dxvsqi(0)
                     bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*kThermal*(
     & dxvsqi(0)+dxvsqi(1))
                     cm(i1,i2,i3)=uu(uc+0)*dxv2i(0)-kThermal*dxvsqi(0)







                    else
c for interpolation points or unused:
                     am(i1,i2,i3)=0.
                     bm(i1,i2,i3)=1.
                     cm(i1,i2,i3)=0.
                    end if
                   end do
                   end do
                   end do
                  else if( computeRHS.eq.1 )then
                   do i3=n3a,n3b
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                    if( mask(i1,i2,i3).gt.0 )then
c rhs equations:
                     f(i1,i2,i3,fct)=f(i1,i2,i3,fct)+ftr2d0(tc)


                    else
c for interpolation points or unused:
                     f(i1,i2,i3,fct)=uu(tc)
                    end if
                   end do
                   end do
                   end do
                  end if
                else if( nd.eq.3 )then
                  ! defineDerivativeMacros(DIM,ORDER,GRIDTYPE)
                  if( computeMatrix.eq.1 .and. computeRHS.eq.1 )then
                   do i3=n3a,n3b
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                    if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                     am(i1,i2,i3)=-uu(uc+0)*dxv2i(0)-kThermal*dxvsqi(0)
                     bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*kThermal*(
     & dxvsqi(0)+dxvsqi(1)+dxvsqi(2))
                     cm(i1,i2,i3)=uu(uc+0)*dxv2i(0)-kThermal*dxvsqi(0)







c rhs equations:
                     f(i1,i2,i3,fct)=f(i1,i2,i3,fct)+ftr3d0(tc)


                    else
c for interpolation points or unused:
                     am(i1,i2,i3)=0.
                     bm(i1,i2,i3)=1.
                     cm(i1,i2,i3)=0.
                     f(i1,i2,i3,fct)=uu(tc)
                    end if
                   end do
                   end do
                   end do
                  else if( computeMatrix.eq.1 )then
                   do i3=n3a,n3b
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                    if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                     am(i1,i2,i3)=-uu(uc+0)*dxv2i(0)-kThermal*dxvsqi(0)
                     bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*kThermal*(
     & dxvsqi(0)+dxvsqi(1)+dxvsqi(2))
                     cm(i1,i2,i3)=uu(uc+0)*dxv2i(0)-kThermal*dxvsqi(0)







                    else
c for interpolation points or unused:
                     am(i1,i2,i3)=0.
                     bm(i1,i2,i3)=1.
                     cm(i1,i2,i3)=0.
                    end if
                   end do
                   end do
                   end do
                  else if( computeRHS.eq.1 )then
                   do i3=n3a,n3b
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                    if( mask(i1,i2,i3).gt.0 )then
c rhs equations:
                     f(i1,i2,i3,fct)=f(i1,i2,i3,fct)+ftr3d0(tc)


                    else
c for interpolation points or unused:
                     f(i1,i2,i3,fct)=uu(tc)
                    end if
                   end do
                   end do
                   end do
                  end if
                else
                  stop 889
                end if
               else if( dir.eq.1 )then
c  ****** INS Temperature equation **********
                if( nd.eq.2 )then
                  ! defineDerivativeMacros(DIM,ORDER,GRIDTYPE)
                  if( computeMatrix.eq.1 .and. computeRHS.eq.1 )then
                   do i3=n3a,n3b
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                    if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                     am(i1,i2,i3)=-uu(uc+1)*dxv2i(1)-kThermal*dxvsqi(1)
                     bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*kThermal*(
     & dxvsqi(0)+dxvsqi(1))
                     cm(i1,i2,i3)=uu(uc+1)*dxv2i(1)-kThermal*dxvsqi(1)







c rhs equations:
                     f(i1,i2,i3,fct)=f(i1,i2,i3,fct)+ftr2d1(tc)


                    else
c for interpolation points or unused:
                     am(i1,i2,i3)=0.
                     bm(i1,i2,i3)=1.
                     cm(i1,i2,i3)=0.
                     f(i1,i2,i3,fct)=uu(tc)
                    end if
                   end do
                   end do
                   end do
                  else if( computeMatrix.eq.1 )then
                   do i3=n3a,n3b
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                    if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                     am(i1,i2,i3)=-uu(uc+1)*dxv2i(1)-kThermal*dxvsqi(1)
                     bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*kThermal*(
     & dxvsqi(0)+dxvsqi(1))
                     cm(i1,i2,i3)=uu(uc+1)*dxv2i(1)-kThermal*dxvsqi(1)







                    else
c for interpolation points or unused:
                     am(i1,i2,i3)=0.
                     bm(i1,i2,i3)=1.
                     cm(i1,i2,i3)=0.
                    end if
                   end do
                   end do
                   end do
                  else if( computeRHS.eq.1 )then
                   do i3=n3a,n3b
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                    if( mask(i1,i2,i3).gt.0 )then
c rhs equations:
                     f(i1,i2,i3,fct)=f(i1,i2,i3,fct)+ftr2d1(tc)


                    else
c for interpolation points or unused:
                     f(i1,i2,i3,fct)=uu(tc)
                    end if
                   end do
                   end do
                   end do
                  end if
                else if( nd.eq.3 )then
                  ! defineDerivativeMacros(DIM,ORDER,GRIDTYPE)
                  if( computeMatrix.eq.1 .and. computeRHS.eq.1 )then
                   do i3=n3a,n3b
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                    if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                     am(i1,i2,i3)=-uu(uc+1)*dxv2i(1)-kThermal*dxvsqi(1)
                     bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*kThermal*(
     & dxvsqi(0)+dxvsqi(1)+dxvsqi(2))
                     cm(i1,i2,i3)=uu(uc+1)*dxv2i(1)-kThermal*dxvsqi(1)







c rhs equations:
                     f(i1,i2,i3,fct)=f(i1,i2,i3,fct)+ftr3d1(tc)


                    else
c for interpolation points or unused:
                     am(i1,i2,i3)=0.
                     bm(i1,i2,i3)=1.
                     cm(i1,i2,i3)=0.
                     f(i1,i2,i3,fct)=uu(tc)
                    end if
                   end do
                   end do
                   end do
                  else if( computeMatrix.eq.1 )then
                   do i3=n3a,n3b
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                    if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                     am(i1,i2,i3)=-uu(uc+1)*dxv2i(1)-kThermal*dxvsqi(1)
                     bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*kThermal*(
     & dxvsqi(0)+dxvsqi(1)+dxvsqi(2))
                     cm(i1,i2,i3)=uu(uc+1)*dxv2i(1)-kThermal*dxvsqi(1)







                    else
c for interpolation points or unused:
                     am(i1,i2,i3)=0.
                     bm(i1,i2,i3)=1.
                     cm(i1,i2,i3)=0.
                    end if
                   end do
                   end do
                   end do
                  else if( computeRHS.eq.1 )then
                   do i3=n3a,n3b
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                    if( mask(i1,i2,i3).gt.0 )then
c rhs equations:
                     f(i1,i2,i3,fct)=f(i1,i2,i3,fct)+ftr3d1(tc)


                    else
c for interpolation points or unused:
                     f(i1,i2,i3,fct)=uu(tc)
                    end if
                   end do
                   end do
                   end do
                  end if
                else
                  stop 889
                end if
              else ! dir.eq.2
c  ****** INS Temperature equation **********
                if( nd.eq.2 )then
                  ! defineDerivativeMacros(DIM,ORDER,GRIDTYPE)
                  if( computeMatrix.eq.1 .and. computeRHS.eq.1 )then
                   do i3=n3a,n3b
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                    if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                     am(i1,i2,i3)=-uu(uc+2)*dxv2i(2)-kThermal*dxvsqi(2)
                     bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*kThermal*(
     & dxvsqi(0)+dxvsqi(1))
                     cm(i1,i2,i3)=uu(uc+2)*dxv2i(2)-kThermal*dxvsqi(2)







c rhs equations:
                     f(i1,i2,i3,fct)=f(i1,i2,i3,fct)+ftr2d2(tc)


                    else
c for interpolation points or unused:
                     am(i1,i2,i3)=0.
                     bm(i1,i2,i3)=1.
                     cm(i1,i2,i3)=0.
                     f(i1,i2,i3,fct)=uu(tc)
                    end if
                   end do
                   end do
                   end do
                  else if( computeMatrix.eq.1 )then
                   do i3=n3a,n3b
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                    if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                     am(i1,i2,i3)=-uu(uc+2)*dxv2i(2)-kThermal*dxvsqi(2)
                     bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*kThermal*(
     & dxvsqi(0)+dxvsqi(1))
                     cm(i1,i2,i3)=uu(uc+2)*dxv2i(2)-kThermal*dxvsqi(2)







                    else
c for interpolation points or unused:
                     am(i1,i2,i3)=0.
                     bm(i1,i2,i3)=1.
                     cm(i1,i2,i3)=0.
                    end if
                   end do
                   end do
                   end do
                  else if( computeRHS.eq.1 )then
                   do i3=n3a,n3b
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                    if( mask(i1,i2,i3).gt.0 )then
c rhs equations:
                     f(i1,i2,i3,fct)=f(i1,i2,i3,fct)+ftr2d2(tc)


                    else
c for interpolation points or unused:
                     f(i1,i2,i3,fct)=uu(tc)
                    end if
                   end do
                   end do
                   end do
                  end if
                else if( nd.eq.3 )then
                  ! defineDerivativeMacros(DIM,ORDER,GRIDTYPE)
                  if( computeMatrix.eq.1 .and. computeRHS.eq.1 )then
                   do i3=n3a,n3b
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                    if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                     am(i1,i2,i3)=-uu(uc+2)*dxv2i(2)-kThermal*dxvsqi(2)
                     bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*kThermal*(
     & dxvsqi(0)+dxvsqi(1)+dxvsqi(2))
                     cm(i1,i2,i3)=uu(uc+2)*dxv2i(2)-kThermal*dxvsqi(2)







c rhs equations:
                     f(i1,i2,i3,fct)=f(i1,i2,i3,fct)+ftr3d2(tc)


                    else
c for interpolation points or unused:
                     am(i1,i2,i3)=0.
                     bm(i1,i2,i3)=1.
                     cm(i1,i2,i3)=0.
                     f(i1,i2,i3,fct)=uu(tc)
                    end if
                   end do
                   end do
                   end do
                  else if( computeMatrix.eq.1 )then
                   do i3=n3a,n3b
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                    if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                     am(i1,i2,i3)=-uu(uc+2)*dxv2i(2)-kThermal*dxvsqi(2)
                     bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*kThermal*(
     & dxvsqi(0)+dxvsqi(1)+dxvsqi(2))
                     cm(i1,i2,i3)=uu(uc+2)*dxv2i(2)-kThermal*dxvsqi(2)







                    else
c for interpolation points or unused:
                     am(i1,i2,i3)=0.
                     bm(i1,i2,i3)=1.
                     cm(i1,i2,i3)=0.
                    end if
                   end do
                   end do
                   end do
                  else if( computeRHS.eq.1 )then
                   do i3=n3a,n3b
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                    if( mask(i1,i2,i3).gt.0 )then
c rhs equations:
                     f(i1,i2,i3,fct)=f(i1,i2,i3,fct)+ftr3d2(tc)


                    else
c for interpolation points or unused:
                     f(i1,i2,i3,fct)=uu(tc)
                    end if
                   end do
                   end do
                   end do
                  end if
                else
                  stop 889
                end if
              end if ! end dir
             else ! order==4
             end if
           else if( gridType.eq.curvilinear )then
             ! *******************************************
             ! ************** curvilinear  ***************
             ! *******************************************
             if( orderOfAccuracy.eq.2 )then
               if( dir.eq.0 )then
                dirp1=mod(0+1,nd)
                dirp2=mod(0+2,nd)
c  ****** Temperature Equation *****
                if( nd.eq.2 )then
                  if( computeMatrix.eq.1 .and. computeRHS.eq.1 )then
                   do i3=n3a,n3b
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                    if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                     t1=(uu(uc)*rxi(0,0)+uu(vc)*rxi(0,1)-kThermal*(rxx(
     & 0,0)+rxy(0,1)))*drv2i(0)
                     t2=kThermal*(rxi(0,0)**2+rxi(0,1)**2)*drvsqi(0)
                     am(i1,i2,i3)=-t1-t2
                     bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*(t2+kThermal*
     & (rxi(dirp1,0)**2+rxi(dirp1,1)**2)*drvsqi(dirp1))
                     cm(i1,i2,i3)=t1-t2





c rhs equations:
                     f(i1,i2,i3,fct)=f(i1,i2,i3,fct)+ftc2d0(tc)


                    else
c for interpolation points or unused:
                     am(i1,i2,i3)=0.
                     bm(i1,i2,i3)=1.
                     cm(i1,i2,i3)=0.
                     f(i1,i2,i3,fct)=uu(tc)
                    end if
                   end do
                   end do
                   end do
                  else if( computeMatrix.eq.1 )then
                   do i3=n3a,n3b
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                    if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                     t1=(uu(uc)*rxi(0,0)+uu(vc)*rxi(0,1)-kThermal*(rxx(
     & 0,0)+rxy(0,1)))*drv2i(0)
                     t2=kThermal*(rxi(0,0)**2+rxi(0,1)**2)*drvsqi(0)
                     am(i1,i2,i3)=-t1-t2
                     bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*(t2+kThermal*
     & (rxi(dirp1,0)**2+rxi(dirp1,1)**2)*drvsqi(dirp1))
                     cm(i1,i2,i3)=t1-t2





                    else
c for interpolation points or unused:
                     am(i1,i2,i3)=0.
                     bm(i1,i2,i3)=1.
                     cm(i1,i2,i3)=0.
                    end if
                   end do
                   end do
                   end do
                  else if( computeRHS.eq.1 )then
                   do i3=n3a,n3b
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                    if( mask(i1,i2,i3).gt.0 )then
c rhs equations:
                     f(i1,i2,i3,fct)=f(i1,i2,i3,fct)+ftc2d0(tc)


                    else
c for interpolation points or unused:
                     f(i1,i2,i3,fct)=uu(tc)
                    end if
                   end do
                   end do
                   end do
                  end if
                else if( nd.eq.3 )then
                  if( computeMatrix.eq.1 .and. computeRHS.eq.1 )then
                   do i3=n3a,n3b
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                    if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                     t1=(uu(uc)*rxi(0,0)+uu(vc)*rxi(0,1)+uu(wc)*rxi(0,
     & 2)-kThermal*(rxx3(0,0)+rxy3(0,1)+rxz3(0,2)))*drv2i(0)
                     t2=kThermal*(rxi(0,0)**2+rxi(0,1)**2+rxi(0,2)**2)*
     & drvsqi(0)
                     am(i1,i2,i3)=-t1-t2
                     bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*(t2+kThermal*
     & ((rxi(dirp1,0)**2+rxi(dirp1,1)**2+rxi(dirp1,2)**2)*drvsqi(
     & dirp1)+(rxi(dirp2,0)**2+rxi(dirp2,1)**2+rxi(dirp2,2)**2)*
     & drvsqi(dirp2)))
                     cm(i1,i2,i3)=t1-t2





c rhs equations:
                     f(i1,i2,i3,fct)=f(i1,i2,i3,fct)+ftc3d0(tc)


                    else
c for interpolation points or unused:
                     am(i1,i2,i3)=0.
                     bm(i1,i2,i3)=1.
                     cm(i1,i2,i3)=0.
                     f(i1,i2,i3,fct)=uu(tc)
                    end if
                   end do
                   end do
                   end do
                  else if( computeMatrix.eq.1 )then
                   do i3=n3a,n3b
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                    if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                     t1=(uu(uc)*rxi(0,0)+uu(vc)*rxi(0,1)+uu(wc)*rxi(0,
     & 2)-kThermal*(rxx3(0,0)+rxy3(0,1)+rxz3(0,2)))*drv2i(0)
                     t2=kThermal*(rxi(0,0)**2+rxi(0,1)**2+rxi(0,2)**2)*
     & drvsqi(0)
                     am(i1,i2,i3)=-t1-t2
                     bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*(t2+kThermal*
     & ((rxi(dirp1,0)**2+rxi(dirp1,1)**2+rxi(dirp1,2)**2)*drvsqi(
     & dirp1)+(rxi(dirp2,0)**2+rxi(dirp2,1)**2+rxi(dirp2,2)**2)*
     & drvsqi(dirp2)))
                     cm(i1,i2,i3)=t1-t2





                    else
c for interpolation points or unused:
                     am(i1,i2,i3)=0.
                     bm(i1,i2,i3)=1.
                     cm(i1,i2,i3)=0.
                    end if
                   end do
                   end do
                   end do
                  else if( computeRHS.eq.1 )then
                   do i3=n3a,n3b
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                    if( mask(i1,i2,i3).gt.0 )then
c rhs equations:
                     f(i1,i2,i3,fct)=f(i1,i2,i3,fct)+ftc3d0(tc)


                    else
c for interpolation points or unused:
                     f(i1,i2,i3,fct)=uu(tc)
                    end if
                   end do
                   end do
                   end do
                  end if
                else
                  stop 223
                end if
               else if( dir.eq.1 )then
                dirp1=mod(1+1,nd)
                dirp2=mod(1+2,nd)
c  ****** Temperature Equation *****
                if( nd.eq.2 )then
                  if( computeMatrix.eq.1 .and. computeRHS.eq.1 )then
                   do i3=n3a,n3b
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                    if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                     t1=(uu(uc)*rxi(1,0)+uu(vc)*rxi(1,1)-kThermal*(rxx(
     & 1,0)+rxy(1,1)))*drv2i(1)
                     t2=kThermal*(rxi(1,0)**2+rxi(1,1)**2)*drvsqi(1)
                     am(i1,i2,i3)=-t1-t2
                     bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*(t2+kThermal*
     & (rxi(dirp1,0)**2+rxi(dirp1,1)**2)*drvsqi(dirp1))
                     cm(i1,i2,i3)=t1-t2





c rhs equations:
                     f(i1,i2,i3,fct)=f(i1,i2,i3,fct)+ftc2d1(tc)


                    else
c for interpolation points or unused:
                     am(i1,i2,i3)=0.
                     bm(i1,i2,i3)=1.
                     cm(i1,i2,i3)=0.
                     f(i1,i2,i3,fct)=uu(tc)
                    end if
                   end do
                   end do
                   end do
                  else if( computeMatrix.eq.1 )then
                   do i3=n3a,n3b
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                    if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                     t1=(uu(uc)*rxi(1,0)+uu(vc)*rxi(1,1)-kThermal*(rxx(
     & 1,0)+rxy(1,1)))*drv2i(1)
                     t2=kThermal*(rxi(1,0)**2+rxi(1,1)**2)*drvsqi(1)
                     am(i1,i2,i3)=-t1-t2
                     bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*(t2+kThermal*
     & (rxi(dirp1,0)**2+rxi(dirp1,1)**2)*drvsqi(dirp1))
                     cm(i1,i2,i3)=t1-t2





                    else
c for interpolation points or unused:
                     am(i1,i2,i3)=0.
                     bm(i1,i2,i3)=1.
                     cm(i1,i2,i3)=0.
                    end if
                   end do
                   end do
                   end do
                  else if( computeRHS.eq.1 )then
                   do i3=n3a,n3b
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                    if( mask(i1,i2,i3).gt.0 )then
c rhs equations:
                     f(i1,i2,i3,fct)=f(i1,i2,i3,fct)+ftc2d1(tc)


                    else
c for interpolation points or unused:
                     f(i1,i2,i3,fct)=uu(tc)
                    end if
                   end do
                   end do
                   end do
                  end if
                else if( nd.eq.3 )then
                  if( computeMatrix.eq.1 .and. computeRHS.eq.1 )then
                   do i3=n3a,n3b
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                    if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                     t1=(uu(uc)*rxi(1,0)+uu(vc)*rxi(1,1)+uu(wc)*rxi(1,
     & 2)-kThermal*(rxx3(1,0)+rxy3(1,1)+rxz3(1,2)))*drv2i(1)
                     t2=kThermal*(rxi(1,0)**2+rxi(1,1)**2+rxi(1,2)**2)*
     & drvsqi(1)
                     am(i1,i2,i3)=-t1-t2
                     bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*(t2+kThermal*
     & ((rxi(dirp1,0)**2+rxi(dirp1,1)**2+rxi(dirp1,2)**2)*drvsqi(
     & dirp1)+(rxi(dirp2,0)**2+rxi(dirp2,1)**2+rxi(dirp2,2)**2)*
     & drvsqi(dirp2)))
                     cm(i1,i2,i3)=t1-t2





c rhs equations:
                     f(i1,i2,i3,fct)=f(i1,i2,i3,fct)+ftc3d1(tc)


                    else
c for interpolation points or unused:
                     am(i1,i2,i3)=0.
                     bm(i1,i2,i3)=1.
                     cm(i1,i2,i3)=0.
                     f(i1,i2,i3,fct)=uu(tc)
                    end if
                   end do
                   end do
                   end do
                  else if( computeMatrix.eq.1 )then
                   do i3=n3a,n3b
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                    if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                     t1=(uu(uc)*rxi(1,0)+uu(vc)*rxi(1,1)+uu(wc)*rxi(1,
     & 2)-kThermal*(rxx3(1,0)+rxy3(1,1)+rxz3(1,2)))*drv2i(1)
                     t2=kThermal*(rxi(1,0)**2+rxi(1,1)**2+rxi(1,2)**2)*
     & drvsqi(1)
                     am(i1,i2,i3)=-t1-t2
                     bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*(t2+kThermal*
     & ((rxi(dirp1,0)**2+rxi(dirp1,1)**2+rxi(dirp1,2)**2)*drvsqi(
     & dirp1)+(rxi(dirp2,0)**2+rxi(dirp2,1)**2+rxi(dirp2,2)**2)*
     & drvsqi(dirp2)))
                     cm(i1,i2,i3)=t1-t2





                    else
c for interpolation points or unused:
                     am(i1,i2,i3)=0.
                     bm(i1,i2,i3)=1.
                     cm(i1,i2,i3)=0.
                    end if
                   end do
                   end do
                   end do
                  else if( computeRHS.eq.1 )then
                   do i3=n3a,n3b
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                    if( mask(i1,i2,i3).gt.0 )then
c rhs equations:
                     f(i1,i2,i3,fct)=f(i1,i2,i3,fct)+ftc3d1(tc)


                    else
c for interpolation points or unused:
                     f(i1,i2,i3,fct)=uu(tc)
                    end if
                   end do
                   end do
                   end do
                  end if
                else
                  stop 223
                end if
              else ! dir.eq.2
                dirp1=mod(2+1,nd)
                dirp2=mod(2+2,nd)
c  ****** Temperature Equation *****
                if( nd.eq.2 )then
                  if( computeMatrix.eq.1 .and. computeRHS.eq.1 )then
                   do i3=n3a,n3b
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                    if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                     t1=(uu(uc)*rxi(2,0)+uu(vc)*rxi(2,1)-kThermal*(rxx(
     & 2,0)+rxy(2,1)))*drv2i(2)
                     t2=kThermal*(rxi(2,0)**2+rxi(2,1)**2)*drvsqi(2)
                     am(i1,i2,i3)=-t1-t2
                     bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*(t2+kThermal*
     & (rxi(dirp1,0)**2+rxi(dirp1,1)**2)*drvsqi(dirp1))
                     cm(i1,i2,i3)=t1-t2





c rhs equations:
                     f(i1,i2,i3,fct)=f(i1,i2,i3,fct)+ftc2d2(tc)


                    else
c for interpolation points or unused:
                     am(i1,i2,i3)=0.
                     bm(i1,i2,i3)=1.
                     cm(i1,i2,i3)=0.
                     f(i1,i2,i3,fct)=uu(tc)
                    end if
                   end do
                   end do
                   end do
                  else if( computeMatrix.eq.1 )then
                   do i3=n3a,n3b
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                    if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                     t1=(uu(uc)*rxi(2,0)+uu(vc)*rxi(2,1)-kThermal*(rxx(
     & 2,0)+rxy(2,1)))*drv2i(2)
                     t2=kThermal*(rxi(2,0)**2+rxi(2,1)**2)*drvsqi(2)
                     am(i1,i2,i3)=-t1-t2
                     bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*(t2+kThermal*
     & (rxi(dirp1,0)**2+rxi(dirp1,1)**2)*drvsqi(dirp1))
                     cm(i1,i2,i3)=t1-t2





                    else
c for interpolation points or unused:
                     am(i1,i2,i3)=0.
                     bm(i1,i2,i3)=1.
                     cm(i1,i2,i3)=0.
                    end if
                   end do
                   end do
                   end do
                  else if( computeRHS.eq.1 )then
                   do i3=n3a,n3b
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                    if( mask(i1,i2,i3).gt.0 )then
c rhs equations:
                     f(i1,i2,i3,fct)=f(i1,i2,i3,fct)+ftc2d2(tc)


                    else
c for interpolation points or unused:
                     f(i1,i2,i3,fct)=uu(tc)
                    end if
                   end do
                   end do
                   end do
                  end if
                else if( nd.eq.3 )then
                  if( computeMatrix.eq.1 .and. computeRHS.eq.1 )then
                   do i3=n3a,n3b
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                    if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                     t1=(uu(uc)*rxi(2,0)+uu(vc)*rxi(2,1)+uu(wc)*rxi(2,
     & 2)-kThermal*(rxx3(2,0)+rxy3(2,1)+rxz3(2,2)))*drv2i(2)
                     t2=kThermal*(rxi(2,0)**2+rxi(2,1)**2+rxi(2,2)**2)*
     & drvsqi(2)
                     am(i1,i2,i3)=-t1-t2
                     bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*(t2+kThermal*
     & ((rxi(dirp1,0)**2+rxi(dirp1,1)**2+rxi(dirp1,2)**2)*drvsqi(
     & dirp1)+(rxi(dirp2,0)**2+rxi(dirp2,1)**2+rxi(dirp2,2)**2)*
     & drvsqi(dirp2)))
                     cm(i1,i2,i3)=t1-t2





c rhs equations:
                     f(i1,i2,i3,fct)=f(i1,i2,i3,fct)+ftc3d2(tc)


                    else
c for interpolation points or unused:
                     am(i1,i2,i3)=0.
                     bm(i1,i2,i3)=1.
                     cm(i1,i2,i3)=0.
                     f(i1,i2,i3,fct)=uu(tc)
                    end if
                   end do
                   end do
                   end do
                  else if( computeMatrix.eq.1 )then
                   do i3=n3a,n3b
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                    if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
                     t1=(uu(uc)*rxi(2,0)+uu(vc)*rxi(2,1)+uu(wc)*rxi(2,
     & 2)-kThermal*(rxx3(2,0)+rxy3(2,1)+rxz3(2,2)))*drv2i(2)
                     t2=kThermal*(rxi(2,0)**2+rxi(2,1)**2+rxi(2,2)**2)*
     & drvsqi(2)
                     am(i1,i2,i3)=-t1-t2
                     bm(i1,i2,i3)=dtScale/dt(i1,i2,i3)+2.*(t2+kThermal*
     & ((rxi(dirp1,0)**2+rxi(dirp1,1)**2+rxi(dirp1,2)**2)*drvsqi(
     & dirp1)+(rxi(dirp2,0)**2+rxi(dirp2,1)**2+rxi(dirp2,2)**2)*
     & drvsqi(dirp2)))
                     cm(i1,i2,i3)=t1-t2





                    else
c for interpolation points or unused:
                     am(i1,i2,i3)=0.
                     bm(i1,i2,i3)=1.
                     cm(i1,i2,i3)=0.
                    end if
                   end do
                   end do
                   end do
                  else if( computeRHS.eq.1 )then
                   do i3=n3a,n3b
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                    if( mask(i1,i2,i3).gt.0 )then
c rhs equations:
                     f(i1,i2,i3,fct)=f(i1,i2,i3,fct)+ftc3d2(tc)


                    else
c for interpolation points or unused:
                     f(i1,i2,i3,fct)=uu(tc)
                    end if
                   end do
                   end do
                   end do
                  end if
                else
                  stop 223
                end if
              end if ! end dir
             else ! order==4
             end if
           else
             stop 111
           end if
        else if( option.eq.assignSpalartAllmaras )then
         ! **************************************************************************
         ! Fill in the tridiagonal matrix for the turbulent eddy viscosity eqution for 
         ! the Spalart Almaras TM
         ! ***************************************************************************
          stop 777
        else
          write(*,*) 'Unknown option=',option
          stop 8
        end if ! option
        ! ****** Boundary Conditions ******
        indexRange(0,0)=n1a
        indexRange(1,0)=n1b
        indexRange(0,1)=n2a
        indexRange(1,1)=n2b
        indexRange(0,2)=n3a
        indexRange(1,2)=n3b
        ! assign loop variables to correspond to the boundary
        do side=0,1
          is1=0
          is2=0
          is3=0
          if( dir.eq.0 )then
            is1=1-2*side
            n1a=indexRange(side,dir)-is1    ! boundary is 1 pt outside
            n1b=n1a
          else if( dir.eq.1 )then
            is2=1-2*side
            n2a=indexRange(side,dir)-is2
            n2b=n2a
          else
            is3=1-2*side
            n3a=indexRange(side,dir)-is3
            n3b=n3a
          end if
         sn=2*side-1 ! sign for normal
         ! write(*,*) '$$$$$ side,bc = ',side,bc(side,ibc)
         if( bc(side,ibc).eq.dirichlet )then
           if( computeMatrixBC.eq.1 )then
             if( fourthOrder.eq.0 )then
                 if( nd.eq.2 )then
                   if( option.eq.assignINS )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.



                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                       end if
                      end do
                      end do
                      end do
                   else if( option.eq.assignTemperature )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.



                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                        f(i1,i2,i3,fct)=uu(tc)
                       end if
                      end do
                      end do
                      end do
                   else if( option.eq.assignSpalartAllmaras )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.



                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                        f(i1,i2,i3,nc)=uu(nc)
                       end if
                      end do
                      end do
                      end do
                   end if
                 else
                   if( option.eq.assignINS )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.



                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                        f(i1,i2,i3,fcw)=uu(wc)
                       end if
                      end do
                      end do
                      end do
                   else if( option.eq.assignTemperature )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.



                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                        f(i1,i2,i3,fct)=uu(tc)
                       end if
                      end do
                      end do
                      end do
                   else if( option.eq.assignSpalartAllmaras )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.



                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                        f(i1,i2,i3,fcw)=uu(wc)
                        f(i1,i2,i3,nc)=uu(nc)
                       end if
                      end do
                      end do
                      end do
                   end if
                 end if
             else
                 if( nd.eq.2 )then
                   if( option.eq.assignINS )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
c write(*,'(" fill am,bm,...,em: i1,i2,i3=",3i3)') i1,i2,i3
                            am(i1,i2,i3)=0.
                            bm(i1,i2,i3)=0.
                            cm(i1,i2,i3)=1.
                            dm(i1,i2,i3)=0.
                            em(i1,i2,i3)=0.
                            am(i1-is1,i2-is2,i3-is3)=0.
                            bm(i1-is1,i2-is2,i3-is3)=0.
                            cm(i1-is1,i2-is2,i3-is3)=1.
                            dm(i1-is1,i2-is2,i3-is3)=0.
                            em(i1-is1,i2-is2,i3-is3)=0.





                         else
c for interpolation points or unused:
                           am(i1,i2,i3)=0.
                           bm(i1,i2,i3)=0.
                           cm(i1,i2,i3)=1.
                           dm(i1,i2,i3)=0.
                           em(i1,i2,i3)=0.
                           am(i1-is1,i2-is2,i3-is3)=0.
                           bm(i1-is1,i2-is2,i3-is3)=0.
                           cm(i1-is1,i2-is2,i3-is3)=1.
                           dm(i1-is1,i2-is2,i3-is3)=0.
                           em(i1-is1,i2-is2,i3-is3)=0.
                           f(i1,i2,i3,fcu)=uu(uc)
                           f(i1,i2,i3,fcv)=uu(vc)
                           f(i1-is1,i2-is2,i3-is3,fcu)=u(i1-is1,i2-is2,
     & i3-is3,uc)
                           f(i1-is1,i2-is2,i3-is3,fcv)=u(i1-is1,i2-is2,
     & i3-is3,vc)
                         end if
                       end do
                       end do
                       end do
                   else if( option.eq.assignTemperature )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
c write(*,'(" fill am,bm,...,em: i1,i2,i3=",3i3)') i1,i2,i3
                            am(i1,i2,i3)=0.
                            bm(i1,i2,i3)=0.
                            cm(i1,i2,i3)=1.
                            dm(i1,i2,i3)=0.
                            em(i1,i2,i3)=0.
                            am(i1-is1,i2-is2,i3-is3)=0.
                            bm(i1-is1,i2-is2,i3-is3)=0.
                            cm(i1-is1,i2-is2,i3-is3)=1.
                            dm(i1-is1,i2-is2,i3-is3)=0.
                            em(i1-is1,i2-is2,i3-is3)=0.





                         else
c for interpolation points or unused:
                           am(i1,i2,i3)=0.
                           bm(i1,i2,i3)=0.
                           cm(i1,i2,i3)=1.
                           dm(i1,i2,i3)=0.
                           em(i1,i2,i3)=0.
                           am(i1-is1,i2-is2,i3-is3)=0.
                           bm(i1-is1,i2-is2,i3-is3)=0.
                           cm(i1-is1,i2-is2,i3-is3)=1.
                           dm(i1-is1,i2-is2,i3-is3)=0.
                           em(i1-is1,i2-is2,i3-is3)=0.
                           f(i1,i2,i3,fct)=uu(tc)
                           f(i1-is1,i2-is2,i3-is3,fct)=u(i1-is1,i2-is2,
     & i3-is3,tc)
                         end if
                       end do
                       end do
                       end do
                   else if( option.eq.assignSpalartAllmaras )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
c write(*,'(" fill am,bm,...,em: i1,i2,i3=",3i3)') i1,i2,i3
                            am(i1,i2,i3)=0.
                            bm(i1,i2,i3)=0.
                            cm(i1,i2,i3)=1.
                            dm(i1,i2,i3)=0.
                            em(i1,i2,i3)=0.
                            am(i1-is1,i2-is2,i3-is3)=0.
                            bm(i1-is1,i2-is2,i3-is3)=0.
                            cm(i1-is1,i2-is2,i3-is3)=1.
                            dm(i1-is1,i2-is2,i3-is3)=0.
                            em(i1-is1,i2-is2,i3-is3)=0.





                         else
c for interpolation points or unused:
                           am(i1,i2,i3)=0.
                           bm(i1,i2,i3)=0.
                           cm(i1,i2,i3)=1.
                           dm(i1,i2,i3)=0.
                           em(i1,i2,i3)=0.
                           am(i1-is1,i2-is2,i3-is3)=0.
                           bm(i1-is1,i2-is2,i3-is3)=0.
                           cm(i1-is1,i2-is2,i3-is3)=1.
                           dm(i1-is1,i2-is2,i3-is3)=0.
                           em(i1-is1,i2-is2,i3-is3)=0.
                           f(i1,i2,i3,fcu)=uu(uc)
                           f(i1,i2,i3,fcv)=uu(vc)
                           f(i1-is1,i2-is2,i3-is3,fcu)=u(i1-is1,i2-is2,
     & i3-is3,uc)
                           f(i1-is1,i2-is2,i3-is3,fcv)=u(i1-is1,i2-is2,
     & i3-is3,vc)
                           f(i1,i2,i3,nc)=uu(nc)
                           f(i1-is1,i2-is2,i3-is3,nc)=u(i1-is1,i2-is2,
     & i3-is3,nc)
                         end if
                       end do
                       end do
                       end do
                   end if
                 else
                   if( option.eq.assignINS )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
c write(*,'(" fill am,bm,...,em: i1,i2,i3=",3i3)') i1,i2,i3
                            am(i1,i2,i3)=0.
                            bm(i1,i2,i3)=0.
                            cm(i1,i2,i3)=1.
                            dm(i1,i2,i3)=0.
                            em(i1,i2,i3)=0.
                            am(i1-is1,i2-is2,i3-is3)=0.
                            bm(i1-is1,i2-is2,i3-is3)=0.
                            cm(i1-is1,i2-is2,i3-is3)=1.
                            dm(i1-is1,i2-is2,i3-is3)=0.
                            em(i1-is1,i2-is2,i3-is3)=0.





                         else
c for interpolation points or unused:
                           am(i1,i2,i3)=0.
                           bm(i1,i2,i3)=0.
                           cm(i1,i2,i3)=1.
                           dm(i1,i2,i3)=0.
                           em(i1,i2,i3)=0.
                           am(i1-is1,i2-is2,i3-is3)=0.
                           bm(i1-is1,i2-is2,i3-is3)=0.
                           cm(i1-is1,i2-is2,i3-is3)=1.
                           dm(i1-is1,i2-is2,i3-is3)=0.
                           em(i1-is1,i2-is2,i3-is3)=0.
                           f(i1,i2,i3,fcu)=uu(uc)
                           f(i1,i2,i3,fcv)=uu(vc)
                           f(i1-is1,i2-is2,i3-is3,fcu)=u(i1-is1,i2-is2,
     & i3-is3,uc)
                           f(i1-is1,i2-is2,i3-is3,fcv)=u(i1-is1,i2-is2,
     & i3-is3,vc)
                           f(i1,i2,i3,fcw)=uu(wc)
                           f(i1-is1,i2-is2,i3-is3,fcw)=u(i1-is1,i2-is2,
     & i3-is3,wc)
                         end if
                       end do
                       end do
                       end do
                   else if( option.eq.assignTemperature )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
c write(*,'(" fill am,bm,...,em: i1,i2,i3=",3i3)') i1,i2,i3
                            am(i1,i2,i3)=0.
                            bm(i1,i2,i3)=0.
                            cm(i1,i2,i3)=1.
                            dm(i1,i2,i3)=0.
                            em(i1,i2,i3)=0.
                            am(i1-is1,i2-is2,i3-is3)=0.
                            bm(i1-is1,i2-is2,i3-is3)=0.
                            cm(i1-is1,i2-is2,i3-is3)=1.
                            dm(i1-is1,i2-is2,i3-is3)=0.
                            em(i1-is1,i2-is2,i3-is3)=0.





                         else
c for interpolation points or unused:
                           am(i1,i2,i3)=0.
                           bm(i1,i2,i3)=0.
                           cm(i1,i2,i3)=1.
                           dm(i1,i2,i3)=0.
                           em(i1,i2,i3)=0.
                           am(i1-is1,i2-is2,i3-is3)=0.
                           bm(i1-is1,i2-is2,i3-is3)=0.
                           cm(i1-is1,i2-is2,i3-is3)=1.
                           dm(i1-is1,i2-is2,i3-is3)=0.
                           em(i1-is1,i2-is2,i3-is3)=0.
                           f(i1,i2,i3,fct)=uu(tc)
                           f(i1-is1,i2-is2,i3-is3,fct)=u(i1-is1,i2-is2,
     & i3-is3,tc)
                         end if
                       end do
                       end do
                       end do
                   else if( option.eq.assignSpalartAllmaras )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
c write(*,'(" fill am,bm,...,em: i1,i2,i3=",3i3)') i1,i2,i3
                            am(i1,i2,i3)=0.
                            bm(i1,i2,i3)=0.
                            cm(i1,i2,i3)=1.
                            dm(i1,i2,i3)=0.
                            em(i1,i2,i3)=0.
                            am(i1-is1,i2-is2,i3-is3)=0.
                            bm(i1-is1,i2-is2,i3-is3)=0.
                            cm(i1-is1,i2-is2,i3-is3)=1.
                            dm(i1-is1,i2-is2,i3-is3)=0.
                            em(i1-is1,i2-is2,i3-is3)=0.





                         else
c for interpolation points or unused:
                           am(i1,i2,i3)=0.
                           bm(i1,i2,i3)=0.
                           cm(i1,i2,i3)=1.
                           dm(i1,i2,i3)=0.
                           em(i1,i2,i3)=0.
                           am(i1-is1,i2-is2,i3-is3)=0.
                           bm(i1-is1,i2-is2,i3-is3)=0.
                           cm(i1-is1,i2-is2,i3-is3)=1.
                           dm(i1-is1,i2-is2,i3-is3)=0.
                           em(i1-is1,i2-is2,i3-is3)=0.
                           f(i1,i2,i3,fcu)=uu(uc)
                           f(i1,i2,i3,fcv)=uu(vc)
                           f(i1-is1,i2-is2,i3-is3,fcu)=u(i1-is1,i2-is2,
     & i3-is3,uc)
                           f(i1-is1,i2-is2,i3-is3,fcv)=u(i1-is1,i2-is2,
     & i3-is3,vc)
                           f(i1,i2,i3,fcw)=uu(wc)
                           f(i1-is1,i2-is2,i3-is3,fcw)=u(i1-is1,i2-is2,
     & i3-is3,wc)
                           f(i1,i2,i3,nc)=uu(nc)
                           f(i1-is1,i2-is2,i3-is3,nc)=u(i1-is1,i2-is2,
     & i3-is3,nc)
                         end if
                       end do
                       end do
                       end do
                   end if
                 end if
             end if
           end if
         else if( bc(side,ibc).eq.neumann )then
           ! apply a neumann BC on this side.
           !             | b[0] c[0] a[0]                |
           !             | a[1] b[1] c[1]                |
           !         A = |      a[2] b[2] c[2]           |
           !             |            .    .    .        |
           !             |                a[.] b[.] c[.] |
           !             |                c[n] a[n] b[n] |
           if( computeMatrixBC.eq.1 )then
             if( computeTemperature.ne.0 )then
               a0 = mixedCoeff(tc,side,dir,grid)
               a1 = mixedNormalCoeff(tc,side,dir,grid)
               write(*,'(" insLineSolve: T BC: (a0,a1)=(",f3.1,",",
     & f3.1,") for side,dir,grid=",3i3)') a0,a1,side,dir,grid
               ! '
             end if
             if( fourthOrder.eq.0 )then
               if( side.eq.0 )then
                   if( nd.eq.2 )then
                     if( option.eq.assignINS )then
                        do i3=n3a,n3b
                        do i2=n2a,n2b
                        do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
                          bm(i1,i2,i3)=1.
                          cm(i1,i2,i3)=0.
                          am(i1,i2,i3)=-1.



                         else
c for interpolation points or unused:
                          am(i1,i2,i3)=0.
                          bm(i1,i2,i3)=1.
                          cm(i1,i2,i3)=0.
                          f(i1,i2,i3,fcu)=uu(uc)
                          f(i1,i2,i3,fcv)=uu(vc)
                         end if
                        end do
                        end do
                        end do
                     else if( option.eq.assignTemperature )then
                        do i3=n3a,n3b
                        do i2=n2a,n2b
                        do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
                          bm(i1,i2,i3)=1.
                          cm(i1,i2,i3)=0.
                          am(i1,i2,i3)=-1.



                         else
c for interpolation points or unused:
                          am(i1,i2,i3)=0.
                          bm(i1,i2,i3)=1.
                          cm(i1,i2,i3)=0.
                          f(i1,i2,i3,fct)=uu(tc)
                         end if
                        end do
                        end do
                        end do
                     else if( option.eq.assignSpalartAllmaras )then
                        do i3=n3a,n3b
                        do i2=n2a,n2b
                        do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
                          bm(i1,i2,i3)=1.
                          cm(i1,i2,i3)=0.
                          am(i1,i2,i3)=-1.



                         else
c for interpolation points or unused:
                          am(i1,i2,i3)=0.
                          bm(i1,i2,i3)=1.
                          cm(i1,i2,i3)=0.
                          f(i1,i2,i3,fcu)=uu(uc)
                          f(i1,i2,i3,fcv)=uu(vc)
                          f(i1,i2,i3,nc)=uu(nc)
                         end if
                        end do
                        end do
                        end do
                     end if
                   else
                     if( option.eq.assignINS )then
                        do i3=n3a,n3b
                        do i2=n2a,n2b
                        do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
                          bm(i1,i2,i3)=1.
                          cm(i1,i2,i3)=0.
                          am(i1,i2,i3)=-1.



                         else
c for interpolation points or unused:
                          am(i1,i2,i3)=0.
                          bm(i1,i2,i3)=1.
                          cm(i1,i2,i3)=0.
                          f(i1,i2,i3,fcu)=uu(uc)
                          f(i1,i2,i3,fcv)=uu(vc)
                          f(i1,i2,i3,fcw)=uu(wc)
                         end if
                        end do
                        end do
                        end do
                     else if( option.eq.assignTemperature )then
                        do i3=n3a,n3b
                        do i2=n2a,n2b
                        do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
                          bm(i1,i2,i3)=1.
                          cm(i1,i2,i3)=0.
                          am(i1,i2,i3)=-1.



                         else
c for interpolation points or unused:
                          am(i1,i2,i3)=0.
                          bm(i1,i2,i3)=1.
                          cm(i1,i2,i3)=0.
                          f(i1,i2,i3,fct)=uu(tc)
                         end if
                        end do
                        end do
                        end do
                     else if( option.eq.assignSpalartAllmaras )then
                        do i3=n3a,n3b
                        do i2=n2a,n2b
                        do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
                          bm(i1,i2,i3)=1.
                          cm(i1,i2,i3)=0.
                          am(i1,i2,i3)=-1.



                         else
c for interpolation points or unused:
                          am(i1,i2,i3)=0.
                          bm(i1,i2,i3)=1.
                          cm(i1,i2,i3)=0.
                          f(i1,i2,i3,fcu)=uu(uc)
                          f(i1,i2,i3,fcv)=uu(vc)
                          f(i1,i2,i3,fcw)=uu(wc)
                          f(i1,i2,i3,nc)=uu(nc)
                         end if
                        end do
                        end do
                        end do
                     end if
                   end if
               else
                   if( nd.eq.2 )then
                     if( option.eq.assignINS )then
                        do i3=n3a,n3b
                        do i2=n2a,n2b
                        do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
                          cm(i1,i2,i3)=-1.
                          am(i1,i2,i3)=0.
                          bm(i1,i2,i3)=1.



                         else
c for interpolation points or unused:
                          am(i1,i2,i3)=0.
                          bm(i1,i2,i3)=1.
                          cm(i1,i2,i3)=0.
                          f(i1,i2,i3,fcu)=uu(uc)
                          f(i1,i2,i3,fcv)=uu(vc)
                         end if
                        end do
                        end do
                        end do
                     else if( option.eq.assignTemperature )then
                        do i3=n3a,n3b
                        do i2=n2a,n2b
                        do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
                          cm(i1,i2,i3)=-1.
                          am(i1,i2,i3)=0.
                          bm(i1,i2,i3)=1.



                         else
c for interpolation points or unused:
                          am(i1,i2,i3)=0.
                          bm(i1,i2,i3)=1.
                          cm(i1,i2,i3)=0.
                          f(i1,i2,i3,fct)=uu(tc)
                         end if
                        end do
                        end do
                        end do
                     else if( option.eq.assignSpalartAllmaras )then
                        do i3=n3a,n3b
                        do i2=n2a,n2b
                        do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
                          cm(i1,i2,i3)=-1.
                          am(i1,i2,i3)=0.
                          bm(i1,i2,i3)=1.



                         else
c for interpolation points or unused:
                          am(i1,i2,i3)=0.
                          bm(i1,i2,i3)=1.
                          cm(i1,i2,i3)=0.
                          f(i1,i2,i3,fcu)=uu(uc)
                          f(i1,i2,i3,fcv)=uu(vc)
                          f(i1,i2,i3,nc)=uu(nc)
                         end if
                        end do
                        end do
                        end do
                     end if
                   else
                     if( option.eq.assignINS )then
                        do i3=n3a,n3b
                        do i2=n2a,n2b
                        do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
                          cm(i1,i2,i3)=-1.
                          am(i1,i2,i3)=0.
                          bm(i1,i2,i3)=1.



                         else
c for interpolation points or unused:
                          am(i1,i2,i3)=0.
                          bm(i1,i2,i3)=1.
                          cm(i1,i2,i3)=0.
                          f(i1,i2,i3,fcu)=uu(uc)
                          f(i1,i2,i3,fcv)=uu(vc)
                          f(i1,i2,i3,fcw)=uu(wc)
                         end if
                        end do
                        end do
                        end do
                     else if( option.eq.assignTemperature )then
                        do i3=n3a,n3b
                        do i2=n2a,n2b
                        do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
                          cm(i1,i2,i3)=-1.
                          am(i1,i2,i3)=0.
                          bm(i1,i2,i3)=1.



                         else
c for interpolation points or unused:
                          am(i1,i2,i3)=0.
                          bm(i1,i2,i3)=1.
                          cm(i1,i2,i3)=0.
                          f(i1,i2,i3,fct)=uu(tc)
                         end if
                        end do
                        end do
                        end do
                     else if( option.eq.assignSpalartAllmaras )then
                        do i3=n3a,n3b
                        do i2=n2a,n2b
                        do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
                          cm(i1,i2,i3)=-1.
                          am(i1,i2,i3)=0.
                          bm(i1,i2,i3)=1.



                         else
c for interpolation points or unused:
                          am(i1,i2,i3)=0.
                          bm(i1,i2,i3)=1.
                          cm(i1,i2,i3)=0.
                          f(i1,i2,i3,fcu)=uu(uc)
                          f(i1,i2,i3,fcv)=uu(vc)
                          f(i1,i2,i3,fcw)=uu(wc)
                          f(i1,i2,i3,nc)=uu(nc)
                         end if
                        end do
                        end do
                        end do
                     end if
                   end if
               end if
             else
               ! use +-D0 and D+D-D0
               if( side.eq.0 )then
                 cexa= 0.
                 cexb= 0.
                 cexc= 1.
                 cexd= 0.
                 cexe=-1.
                 c4exa= 2.
                 c4exb=-1.
                 c4exc= 1.
                 c4exd=-2.
                 c4exe= 0.
               else
                 cexa=-1.
                 cexb= 0.
                 cexc= 1.
                 cexd= 0.
                 cexe= 0.
                 c4exa= 0.
                 c4exb=-2.
                 c4exc= 1.
                 c4exd=-1.
                 c4exe= 2.
               end if
                 if( nd.eq.2 )then
                   if( option.eq.assignINS )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
                            am(i1,i2,i3)=cexa
                            bm(i1,i2,i3)=cexb
                            cm(i1,i2,i3)=cexc
                            dm(i1,i2,i3)=cexd
                            em(i1,i2,i3)=cexe
                            am(i1-is1,i2-is2,i3-is3)=c4exa
                            bm(i1-is1,i2-is2,i3-is3)=c4exb
                            cm(i1-is1,i2-is2,i3-is3)=c4exc
                            dm(i1-is1,i2-is2,i3-is3)=c4exd
                            em(i1-is1,i2-is2,i3-is3)=c4exe





                         else
c for interpolation points or unused:
                           am(i1,i2,i3)=0.
                           bm(i1,i2,i3)=0.
                           cm(i1,i2,i3)=1.
                           dm(i1,i2,i3)=0.
                           em(i1,i2,i3)=0.
                           am(i1-is1,i2-is2,i3-is3)=0.
                           bm(i1-is1,i2-is2,i3-is3)=0.
                           cm(i1-is1,i2-is2,i3-is3)=1.
                           dm(i1-is1,i2-is2,i3-is3)=0.
                           em(i1-is1,i2-is2,i3-is3)=0.
                           f(i1,i2,i3,fcu)=uu(uc)
                           f(i1,i2,i3,fcv)=uu(vc)
                           f(i1-is1,i2-is2,i3-is3,fcu)=u(i1-is1,i2-is2,
     & i3-is3,uc)
                           f(i1-is1,i2-is2,i3-is3,fcv)=u(i1-is1,i2-is2,
     & i3-is3,vc)
                         end if
                       end do
                       end do
                       end do
                   else if( option.eq.assignTemperature )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
                            am(i1,i2,i3)=cexa
                            bm(i1,i2,i3)=cexb
                            cm(i1,i2,i3)=cexc
                            dm(i1,i2,i3)=cexd
                            em(i1,i2,i3)=cexe
                            am(i1-is1,i2-is2,i3-is3)=c4exa
                            bm(i1-is1,i2-is2,i3-is3)=c4exb
                            cm(i1-is1,i2-is2,i3-is3)=c4exc
                            dm(i1-is1,i2-is2,i3-is3)=c4exd
                            em(i1-is1,i2-is2,i3-is3)=c4exe





                         else
c for interpolation points or unused:
                           am(i1,i2,i3)=0.
                           bm(i1,i2,i3)=0.
                           cm(i1,i2,i3)=1.
                           dm(i1,i2,i3)=0.
                           em(i1,i2,i3)=0.
                           am(i1-is1,i2-is2,i3-is3)=0.
                           bm(i1-is1,i2-is2,i3-is3)=0.
                           cm(i1-is1,i2-is2,i3-is3)=1.
                           dm(i1-is1,i2-is2,i3-is3)=0.
                           em(i1-is1,i2-is2,i3-is3)=0.
                           f(i1,i2,i3,fct)=uu(tc)
                           f(i1-is1,i2-is2,i3-is3,fct)=u(i1-is1,i2-is2,
     & i3-is3,tc)
                         end if
                       end do
                       end do
                       end do
                   else if( option.eq.assignSpalartAllmaras )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
                            am(i1,i2,i3)=cexa
                            bm(i1,i2,i3)=cexb
                            cm(i1,i2,i3)=cexc
                            dm(i1,i2,i3)=cexd
                            em(i1,i2,i3)=cexe
                            am(i1-is1,i2-is2,i3-is3)=c4exa
                            bm(i1-is1,i2-is2,i3-is3)=c4exb
                            cm(i1-is1,i2-is2,i3-is3)=c4exc
                            dm(i1-is1,i2-is2,i3-is3)=c4exd
                            em(i1-is1,i2-is2,i3-is3)=c4exe





                         else
c for interpolation points or unused:
                           am(i1,i2,i3)=0.
                           bm(i1,i2,i3)=0.
                           cm(i1,i2,i3)=1.
                           dm(i1,i2,i3)=0.
                           em(i1,i2,i3)=0.
                           am(i1-is1,i2-is2,i3-is3)=0.
                           bm(i1-is1,i2-is2,i3-is3)=0.
                           cm(i1-is1,i2-is2,i3-is3)=1.
                           dm(i1-is1,i2-is2,i3-is3)=0.
                           em(i1-is1,i2-is2,i3-is3)=0.
                           f(i1,i2,i3,fcu)=uu(uc)
                           f(i1,i2,i3,fcv)=uu(vc)
                           f(i1-is1,i2-is2,i3-is3,fcu)=u(i1-is1,i2-is2,
     & i3-is3,uc)
                           f(i1-is1,i2-is2,i3-is3,fcv)=u(i1-is1,i2-is2,
     & i3-is3,vc)
                           f(i1,i2,i3,nc)=uu(nc)
                           f(i1-is1,i2-is2,i3-is3,nc)=u(i1-is1,i2-is2,
     & i3-is3,nc)
                         end if
                       end do
                       end do
                       end do
                   end if
                 else
                   if( option.eq.assignINS )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
                            am(i1,i2,i3)=cexa
                            bm(i1,i2,i3)=cexb
                            cm(i1,i2,i3)=cexc
                            dm(i1,i2,i3)=cexd
                            em(i1,i2,i3)=cexe
                            am(i1-is1,i2-is2,i3-is3)=c4exa
                            bm(i1-is1,i2-is2,i3-is3)=c4exb
                            cm(i1-is1,i2-is2,i3-is3)=c4exc
                            dm(i1-is1,i2-is2,i3-is3)=c4exd
                            em(i1-is1,i2-is2,i3-is3)=c4exe





                         else
c for interpolation points or unused:
                           am(i1,i2,i3)=0.
                           bm(i1,i2,i3)=0.
                           cm(i1,i2,i3)=1.
                           dm(i1,i2,i3)=0.
                           em(i1,i2,i3)=0.
                           am(i1-is1,i2-is2,i3-is3)=0.
                           bm(i1-is1,i2-is2,i3-is3)=0.
                           cm(i1-is1,i2-is2,i3-is3)=1.
                           dm(i1-is1,i2-is2,i3-is3)=0.
                           em(i1-is1,i2-is2,i3-is3)=0.
                           f(i1,i2,i3,fcu)=uu(uc)
                           f(i1,i2,i3,fcv)=uu(vc)
                           f(i1-is1,i2-is2,i3-is3,fcu)=u(i1-is1,i2-is2,
     & i3-is3,uc)
                           f(i1-is1,i2-is2,i3-is3,fcv)=u(i1-is1,i2-is2,
     & i3-is3,vc)
                           f(i1,i2,i3,fcw)=uu(wc)
                           f(i1-is1,i2-is2,i3-is3,fcw)=u(i1-is1,i2-is2,
     & i3-is3,wc)
                         end if
                       end do
                       end do
                       end do
                   else if( option.eq.assignTemperature )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
                            am(i1,i2,i3)=cexa
                            bm(i1,i2,i3)=cexb
                            cm(i1,i2,i3)=cexc
                            dm(i1,i2,i3)=cexd
                            em(i1,i2,i3)=cexe
                            am(i1-is1,i2-is2,i3-is3)=c4exa
                            bm(i1-is1,i2-is2,i3-is3)=c4exb
                            cm(i1-is1,i2-is2,i3-is3)=c4exc
                            dm(i1-is1,i2-is2,i3-is3)=c4exd
                            em(i1-is1,i2-is2,i3-is3)=c4exe





                         else
c for interpolation points or unused:
                           am(i1,i2,i3)=0.
                           bm(i1,i2,i3)=0.
                           cm(i1,i2,i3)=1.
                           dm(i1,i2,i3)=0.
                           em(i1,i2,i3)=0.
                           am(i1-is1,i2-is2,i3-is3)=0.
                           bm(i1-is1,i2-is2,i3-is3)=0.
                           cm(i1-is1,i2-is2,i3-is3)=1.
                           dm(i1-is1,i2-is2,i3-is3)=0.
                           em(i1-is1,i2-is2,i3-is3)=0.
                           f(i1,i2,i3,fct)=uu(tc)
                           f(i1-is1,i2-is2,i3-is3,fct)=u(i1-is1,i2-is2,
     & i3-is3,tc)
                         end if
                       end do
                       end do
                       end do
                   else if( option.eq.assignSpalartAllmaras )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
                            am(i1,i2,i3)=cexa
                            bm(i1,i2,i3)=cexb
                            cm(i1,i2,i3)=cexc
                            dm(i1,i2,i3)=cexd
                            em(i1,i2,i3)=cexe
                            am(i1-is1,i2-is2,i3-is3)=c4exa
                            bm(i1-is1,i2-is2,i3-is3)=c4exb
                            cm(i1-is1,i2-is2,i3-is3)=c4exc
                            dm(i1-is1,i2-is2,i3-is3)=c4exd
                            em(i1-is1,i2-is2,i3-is3)=c4exe





                         else
c for interpolation points or unused:
                           am(i1,i2,i3)=0.
                           bm(i1,i2,i3)=0.
                           cm(i1,i2,i3)=1.
                           dm(i1,i2,i3)=0.
                           em(i1,i2,i3)=0.
                           am(i1-is1,i2-is2,i3-is3)=0.
                           bm(i1-is1,i2-is2,i3-is3)=0.
                           cm(i1-is1,i2-is2,i3-is3)=1.
                           dm(i1-is1,i2-is2,i3-is3)=0.
                           em(i1-is1,i2-is2,i3-is3)=0.
                           f(i1,i2,i3,fcu)=uu(uc)
                           f(i1,i2,i3,fcv)=uu(vc)
                           f(i1-is1,i2-is2,i3-is3,fcu)=u(i1-is1,i2-is2,
     & i3-is3,uc)
                           f(i1-is1,i2-is2,i3-is3,fcv)=u(i1-is1,i2-is2,
     & i3-is3,vc)
                           f(i1,i2,i3,fcw)=uu(wc)
                           f(i1-is1,i2-is2,i3-is3,fcw)=u(i1-is1,i2-is2,
     & i3-is3,wc)
                           f(i1,i2,i3,nc)=uu(nc)
                           f(i1-is1,i2-is2,i3-is3,nc)=u(i1-is1,i2-is2,
     & i3-is3,nc)
                         end if
                       end do
                       end do
                       end do
                   end if
                 end if
             end if
           end if
         else if( bc(side,ibc).eq.extrapolate )then
           if( computeMatrixBC.eq.1 )then
             if( fourthOrder.eq.0 )then
               ! **** second order ****
               if( orderOfExtrapolation.eq.2 )then
                 if( side.eq.0 )then
                   cexa= 1.
                   cexb= 1.
                   cexc=-2.
                 else
                   cexa=-2.
                   cexb= 1.
                   cexc= 1.
                 end if
               else if( orderOfExtrapolation.eq.3 )then
                 if( side.eq.0 )then
                   cexa= 3.
                   cexb= 1.
                   cexc=-3.
                 else
                   cexa=-3.
                   cexb= 1.
                   cexc= 3.
                 end if
               else
                 write(*,*) 'ERROR: not implemeted: 
     & orderOfExtrapolation=',orderOfExtrapolation
                 stop 1111
               end if
                 if( nd.eq.2 )then
                   if( option.eq.assignINS )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
                        am(i1,i2,i3)=cexa
                        bm(i1,i2,i3)=cexb
                        cm(i1,i2,i3)=cexc



                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                       end if
                      end do
                      end do
                      end do
                   else if( option.eq.assignTemperature )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
                        am(i1,i2,i3)=cexa
                        bm(i1,i2,i3)=cexb
                        cm(i1,i2,i3)=cexc



                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                        f(i1,i2,i3,fct)=uu(tc)
                       end if
                      end do
                      end do
                      end do
                   else if( option.eq.assignSpalartAllmaras )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
                        am(i1,i2,i3)=cexa
                        bm(i1,i2,i3)=cexb
                        cm(i1,i2,i3)=cexc



                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                        f(i1,i2,i3,nc)=uu(nc)
                       end if
                      end do
                      end do
                      end do
                   end if
                 else
                   if( option.eq.assignINS )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
                        am(i1,i2,i3)=cexa
                        bm(i1,i2,i3)=cexb
                        cm(i1,i2,i3)=cexc



                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                        f(i1,i2,i3,fcw)=uu(wc)
                       end if
                      end do
                      end do
                      end do
                   else if( option.eq.assignTemperature )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
                        am(i1,i2,i3)=cexa
                        bm(i1,i2,i3)=cexb
                        cm(i1,i2,i3)=cexc



                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                        f(i1,i2,i3,fct)=uu(tc)
                       end if
                      end do
                      end do
                      end do
                   else if( option.eq.assignSpalartAllmaras )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
                        am(i1,i2,i3)=cexa
                        bm(i1,i2,i3)=cexb
                        cm(i1,i2,i3)=cexc



                       else
c for interpolation points or unused:
                        am(i1,i2,i3)=0.
                        bm(i1,i2,i3)=1.
                        cm(i1,i2,i3)=0.
                        f(i1,i2,i3,fcu)=uu(uc)
                        f(i1,i2,i3,fcv)=uu(vc)
                        f(i1,i2,i3,fcw)=uu(wc)
                        f(i1,i2,i3,nc)=uu(nc)
                       end if
                      end do
                      end do
                      end do
                   end if
                 end if
             else
               ! **** fourth order ****
               if( orderOfExtrapolation.eq.2 )then
                 if( side.eq.0 )then
                   cexa= 0.
                   cexb= 0.
                   cexc= 1.
                   cexd=-2.
                   cexe= 1.
                 else
                   cexa= 1.
                   cexb=-2.
                   cexc= 1.
                   cexd= 0.
                   cexe= 0.
                 end if
               else if( orderOfExtrapolation.eq.3 )then
                 if( side.eq.0 )then
                   cexa=-1.
                   cexb= 0.
                   cexc= 1.
                   cexd=-3.
                   cexe= 3.
                 else
                   cexa= 3.
                   cexb=-3.
                   cexc= 1.
                   cexd= 0.
                   cexe=-1.
                 end if
               else
                 write(*,*) 'ERROR: not implemeted: 
     & orderOfExtrapolation=',orderOfExtrapolation
                 stop 1111
               end if
               if( side.eq.0 )then
                 c4exa=-4.
                 c4exb= 1.
                 c4exc= 1.
                 c4exd=-4.
                 c4exe= 6.
               else
                 c4exa=+6.
                 c4exb=-4.
                 c4exc= 1.
                 c4exd= 1.
                 c4exe=-4.
               end if
                 if( nd.eq.2 )then
                   if( option.eq.assignINS )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
                            am(i1,i2,i3)=cexa
                            bm(i1,i2,i3)=cexb
                            cm(i1,i2,i3)=cexc
                            dm(i1,i2,i3)=cexd
                            em(i1,i2,i3)=cexe
                            am(i1-is1,i2-is2,i3-is3)=c4exa
                            bm(i1-is1,i2-is2,i3-is3)=c4exb
                            cm(i1-is1,i2-is2,i3-is3)=c4exc
                            dm(i1-is1,i2-is2,i3-is3)=c4exd
                            em(i1-is1,i2-is2,i3-is3)=c4exe





                         else
c for interpolation points or unused:
                           am(i1,i2,i3)=0.
                           bm(i1,i2,i3)=0.
                           cm(i1,i2,i3)=1.
                           dm(i1,i2,i3)=0.
                           em(i1,i2,i3)=0.
                           am(i1-is1,i2-is2,i3-is3)=0.
                           bm(i1-is1,i2-is2,i3-is3)=0.
                           cm(i1-is1,i2-is2,i3-is3)=1.
                           dm(i1-is1,i2-is2,i3-is3)=0.
                           em(i1-is1,i2-is2,i3-is3)=0.
                           f(i1,i2,i3,fcu)=uu(uc)
                           f(i1,i2,i3,fcv)=uu(vc)
                           f(i1-is1,i2-is2,i3-is3,fcu)=u(i1-is1,i2-is2,
     & i3-is3,uc)
                           f(i1-is1,i2-is2,i3-is3,fcv)=u(i1-is1,i2-is2,
     & i3-is3,vc)
                         end if
                       end do
                       end do
                       end do
                   else if( option.eq.assignTemperature )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
                            am(i1,i2,i3)=cexa
                            bm(i1,i2,i3)=cexb
                            cm(i1,i2,i3)=cexc
                            dm(i1,i2,i3)=cexd
                            em(i1,i2,i3)=cexe
                            am(i1-is1,i2-is2,i3-is3)=c4exa
                            bm(i1-is1,i2-is2,i3-is3)=c4exb
                            cm(i1-is1,i2-is2,i3-is3)=c4exc
                            dm(i1-is1,i2-is2,i3-is3)=c4exd
                            em(i1-is1,i2-is2,i3-is3)=c4exe





                         else
c for interpolation points or unused:
                           am(i1,i2,i3)=0.
                           bm(i1,i2,i3)=0.
                           cm(i1,i2,i3)=1.
                           dm(i1,i2,i3)=0.
                           em(i1,i2,i3)=0.
                           am(i1-is1,i2-is2,i3-is3)=0.
                           bm(i1-is1,i2-is2,i3-is3)=0.
                           cm(i1-is1,i2-is2,i3-is3)=1.
                           dm(i1-is1,i2-is2,i3-is3)=0.
                           em(i1-is1,i2-is2,i3-is3)=0.
                           f(i1,i2,i3,fct)=uu(tc)
                           f(i1-is1,i2-is2,i3-is3,fct)=u(i1-is1,i2-is2,
     & i3-is3,tc)
                         end if
                       end do
                       end do
                       end do
                   else if( option.eq.assignSpalartAllmaras )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
                            am(i1,i2,i3)=cexa
                            bm(i1,i2,i3)=cexb
                            cm(i1,i2,i3)=cexc
                            dm(i1,i2,i3)=cexd
                            em(i1,i2,i3)=cexe
                            am(i1-is1,i2-is2,i3-is3)=c4exa
                            bm(i1-is1,i2-is2,i3-is3)=c4exb
                            cm(i1-is1,i2-is2,i3-is3)=c4exc
                            dm(i1-is1,i2-is2,i3-is3)=c4exd
                            em(i1-is1,i2-is2,i3-is3)=c4exe





                         else
c for interpolation points or unused:
                           am(i1,i2,i3)=0.
                           bm(i1,i2,i3)=0.
                           cm(i1,i2,i3)=1.
                           dm(i1,i2,i3)=0.
                           em(i1,i2,i3)=0.
                           am(i1-is1,i2-is2,i3-is3)=0.
                           bm(i1-is1,i2-is2,i3-is3)=0.
                           cm(i1-is1,i2-is2,i3-is3)=1.
                           dm(i1-is1,i2-is2,i3-is3)=0.
                           em(i1-is1,i2-is2,i3-is3)=0.
                           f(i1,i2,i3,fcu)=uu(uc)
                           f(i1,i2,i3,fcv)=uu(vc)
                           f(i1-is1,i2-is2,i3-is3,fcu)=u(i1-is1,i2-is2,
     & i3-is3,uc)
                           f(i1-is1,i2-is2,i3-is3,fcv)=u(i1-is1,i2-is2,
     & i3-is3,vc)
                           f(i1,i2,i3,nc)=uu(nc)
                           f(i1-is1,i2-is2,i3-is3,nc)=u(i1-is1,i2-is2,
     & i3-is3,nc)
                         end if
                       end do
                       end do
                       end do
                   end if
                 else
                   if( option.eq.assignINS )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
                            am(i1,i2,i3)=cexa
                            bm(i1,i2,i3)=cexb
                            cm(i1,i2,i3)=cexc
                            dm(i1,i2,i3)=cexd
                            em(i1,i2,i3)=cexe
                            am(i1-is1,i2-is2,i3-is3)=c4exa
                            bm(i1-is1,i2-is2,i3-is3)=c4exb
                            cm(i1-is1,i2-is2,i3-is3)=c4exc
                            dm(i1-is1,i2-is2,i3-is3)=c4exd
                            em(i1-is1,i2-is2,i3-is3)=c4exe





                         else
c for interpolation points or unused:
                           am(i1,i2,i3)=0.
                           bm(i1,i2,i3)=0.
                           cm(i1,i2,i3)=1.
                           dm(i1,i2,i3)=0.
                           em(i1,i2,i3)=0.
                           am(i1-is1,i2-is2,i3-is3)=0.
                           bm(i1-is1,i2-is2,i3-is3)=0.
                           cm(i1-is1,i2-is2,i3-is3)=1.
                           dm(i1-is1,i2-is2,i3-is3)=0.
                           em(i1-is1,i2-is2,i3-is3)=0.
                           f(i1,i2,i3,fcu)=uu(uc)
                           f(i1,i2,i3,fcv)=uu(vc)
                           f(i1-is1,i2-is2,i3-is3,fcu)=u(i1-is1,i2-is2,
     & i3-is3,uc)
                           f(i1-is1,i2-is2,i3-is3,fcv)=u(i1-is1,i2-is2,
     & i3-is3,vc)
                           f(i1,i2,i3,fcw)=uu(wc)
                           f(i1-is1,i2-is2,i3-is3,fcw)=u(i1-is1,i2-is2,
     & i3-is3,wc)
                         end if
                       end do
                       end do
                       end do
                   else if( option.eq.assignTemperature )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
                            am(i1,i2,i3)=cexa
                            bm(i1,i2,i3)=cexb
                            cm(i1,i2,i3)=cexc
                            dm(i1,i2,i3)=cexd
                            em(i1,i2,i3)=cexe
                            am(i1-is1,i2-is2,i3-is3)=c4exa
                            bm(i1-is1,i2-is2,i3-is3)=c4exb
                            cm(i1-is1,i2-is2,i3-is3)=c4exc
                            dm(i1-is1,i2-is2,i3-is3)=c4exd
                            em(i1-is1,i2-is2,i3-is3)=c4exe





                         else
c for interpolation points or unused:
                           am(i1,i2,i3)=0.
                           bm(i1,i2,i3)=0.
                           cm(i1,i2,i3)=1.
                           dm(i1,i2,i3)=0.
                           em(i1,i2,i3)=0.
                           am(i1-is1,i2-is2,i3-is3)=0.
                           bm(i1-is1,i2-is2,i3-is3)=0.
                           cm(i1-is1,i2-is2,i3-is3)=1.
                           dm(i1-is1,i2-is2,i3-is3)=0.
                           em(i1-is1,i2-is2,i3-is3)=0.
                           f(i1,i2,i3,fct)=uu(tc)
                           f(i1-is1,i2-is2,i3-is3,fct)=u(i1-is1,i2-is2,
     & i3-is3,tc)
                         end if
                       end do
                       end do
                       end do
                   else if( option.eq.assignSpalartAllmaras )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
                            am(i1,i2,i3)=cexa
                            bm(i1,i2,i3)=cexb
                            cm(i1,i2,i3)=cexc
                            dm(i1,i2,i3)=cexd
                            em(i1,i2,i3)=cexe
                            am(i1-is1,i2-is2,i3-is3)=c4exa
                            bm(i1-is1,i2-is2,i3-is3)=c4exb
                            cm(i1-is1,i2-is2,i3-is3)=c4exc
                            dm(i1-is1,i2-is2,i3-is3)=c4exd
                            em(i1-is1,i2-is2,i3-is3)=c4exe





                         else
c for interpolation points or unused:
                           am(i1,i2,i3)=0.
                           bm(i1,i2,i3)=0.
                           cm(i1,i2,i3)=1.
                           dm(i1,i2,i3)=0.
                           em(i1,i2,i3)=0.
                           am(i1-is1,i2-is2,i3-is3)=0.
                           bm(i1-is1,i2-is2,i3-is3)=0.
                           cm(i1-is1,i2-is2,i3-is3)=1.
                           dm(i1-is1,i2-is2,i3-is3)=0.
                           em(i1-is1,i2-is2,i3-is3)=0.
                           f(i1,i2,i3,fcu)=uu(uc)
                           f(i1,i2,i3,fcv)=uu(vc)
                           f(i1-is1,i2-is2,i3-is3,fcu)=u(i1-is1,i2-is2,
     & i3-is3,uc)
                           f(i1-is1,i2-is2,i3-is3,fcv)=u(i1-is1,i2-is2,
     & i3-is3,vc)
                           f(i1,i2,i3,fcw)=uu(wc)
                           f(i1-is1,i2-is2,i3-is3,fcw)=u(i1-is1,i2-is2,
     & i3-is3,wc)
                           f(i1,i2,i3,nc)=uu(nc)
                           f(i1-is1,i2-is2,i3-is3,nc)=u(i1-is1,i2-is2,
     & i3-is3,nc)
                         end if
                       end do
                       end do
                       end do
                   end if
                 end if
             end if
           end if
         end if
         ! reset values
         if( dir.eq.0 )then
           n1a=indexRange(0,dir)
           n1b=indexRange(1,dir)
         else if( dir.eq.1 )then
           n2a=indexRange(0,dir)
           n2b=indexRange(1,dir)
         else
           n3a=indexRange(0,dir)
           n3b=indexRange(1,dir)
         end if
        end do ! do side
        return
        end
