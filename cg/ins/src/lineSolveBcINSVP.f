! This file automatically generated from insLineSolveBC.bf with bpp.
        subroutine lineSolveBcINSVP(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,
     & nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,md1a,md1b,md2a,md2b,md3a,
     & md3b, mask,rsxy,  u,gv,dt,f,dw,dir,am,bm,cm,dm,em,  bc, 
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
c bc(0:1,0:nd-1) : line solver BC's 
c boundaryCondition(0:1,0:nd-1) : MappedGrid boundary conditions
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
        integer numberOfComponents,systemComponent
        integer ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,ndbcd3b,
     & ndbcd4a,ndbcd4b
        real bcData(ndbcd1a:ndbcd1b,ndbcd2a:ndbcd2b,ndbcd3a:ndbcd3b,
     & ndbcd4a:ndbcd4b)
        integer ipar(0:*)
        real rpar(0:*)
        !     ---- local variables -----
        integer m,n,c,i1,i2,i3,j1,j2,j3,orderOfAccuracy,gridIsMoving,
     & useWhereMask
        integer gridIsImplicit,implicitOption,implicitMethod,ibc,
     & isAxisymmetric,use2ndOrderAD,use4thOrderAD,
     & useSelfAdjointDiffusion,orderOfExtrapolation,fourthOrder,dirp1,
     & dirp2
        integer pc,uc,vc,wc,tc,vsc,fc,fcu,fcv,fcw,fcn,fct,grid,side,
     & gridType
        integer computeMatrix,computeRHS,computeMatrixBC
        integer twilightZoneFlow,computeTemperature
        integer indexRange(0:1,0:2),gid(0:1,0:2),is1,is2,is3
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
        real cna,cnb,cnc
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
       ! #If "INSVP" == "INSVP"
        ! --- visco plastic variables ---
        ! declareViscoPlasticVariables()
       ! #End
        double precision pdb
        character *50 name
        integer ok,getInt,getReal
        integer nc
        integer noSlipWall,inflowWithVelocityGiven,slipWall,outflow,
     & convectiveOutflow,tractionFree,inflowWithPandTV,
     & dirichletBoundaryCondition,symmetry,axisymmetric
        parameter( noSlipWall=1,inflowWithVelocityGiven=2,slipWall=4,
     & outflow=5,convectiveOutflow=14,tractionFree=15,
     & inflowWithPandTV=3,dirichletBoundaryCondition=12,symmetry=11,
     & axisymmetric=13 )
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
        real uAve0,uAve1,uAve2,uAve3d0,uAve3d1,uAve3d2
        real rxi,rxr,rxs,rxt,rxx,rxy,ryy,rxx3,rxy3,rxz3
        real ur,us,ut,urs,urt,ust,urr,uss,utt
        real uxx0,uyy0,uzz0,ux2c,uy2c,ux3c,uy3c,uz3c
        real lap2d2c,lap3d2c
        real uu, ux2,uy2,uz2,uxx2,uyy2,uzz2,lap2d2,lap3d2
        real ux4,uy4,uz4,uxx4,lap2d4,lap3d4,uxy2,uxz2,uyz2,uxy4,uxz4,
     & uyz4,uyy4,uzz4
        real mixedRHS,mixedCoeff,mixedNormalCoeff,a0,a1
        real an1,an2,an3,aNormi,cnm,cnz,cnp,epsx
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
       ! declareDifferenceOrder4(u,RX)
        ! This include file (created above) declares variables needed by the getDuDx() macros. (
       !** include 'insLSdeclareTemporaryVariablesOrder2.h'
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
        ! defineDifferenceOrder4Components1(u,RX)
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
       !* ux4(c)   = ux42r(i1,i2,i3,c)
       !* uy4(c)   = uy42r(i1,i2,i3,c)
       !* uz4(c)   = uz43r(i1,i2,i3,c)
       !* uxy4(c)  = uxy42r(i1,i2,i3,c)
       !* uxz4(c)  = uxz43r(i1,i2,i3,c) 
       !* uyz4(c)  = uyz43r(i1,i2,i3,c) 
       !* uxx4(c)  = uxx42r(i1,i2,i3,c) 
       !* uyy4(c)  = uyy42r(i1,i2,i3,c) 
       !* uzz4(c)  = uzz43r(i1,i2,i3,c) 
       !* lap2d4(c)= ulaplacian42r(i1,i2,i3,c)
       !* lap3d4(c)= ulaplacian43r(i1,i2,i3,c)
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
        ! This next macro is defined in initLineSolveParameters.h 
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
              systemComponent   = ipar(30) ! form the tridiagonal system for this component
              gid(0,0)          = ipar(31)
              gid(1,0)          = ipar(32)
              gid(0,1)          = ipar(33)
              gid(1,1)          = ipar(34)
              gid(0,2)          = ipar(35)
              gid(1,2)          = ipar(36)
              vsc               = ipar(37)
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
        !*       nuViscoPlastic         =rpar(21)
        !*       etaViscoPlastic        =rpar(22)
        !*       yieldStressViscoPlastic=rpar(23)
        !*       exponentViscoPlastic   =rpar(24)
        !*       epsViscoPlastic        =rpar(25)   ! small parameter used to offset the effective strain rate 
              ! here are the names used by the getViscoPlasticViscosity macro -- what should we do about this ? 
        !*       etaVP=etaViscoPlastic
        !*       yieldStressVP=yieldStressViscoPlastic
        !*       exponentVP=exponentViscoPlastic
        !*       epsVP=epsViscoPlastic
        !*  write(*,'("lineSolveNewINSVP: nuViscoPlastic=",e10.3)') nuViscoPlastic
        !*  write(*,'("lineSolveNewINSVP: etaViscoPlastic=",e10.3)') etaViscoPlastic
        !*  write(*,'("lineSolveNewINSVP: yieldStressViscoPlastic=",e10.3)') yieldStressViscoPlastic
        !*  write(*,'("lineSolveNewINSVP: exponentViscoPlastic=",e10.3)') exponentViscoPlastic
        !*  write(*,'("lineSolveNewINSVP: epsViscoPlastic=",e10.3)') epsViscoPlastic
        !* -- new way:
        !*  double precision pdb
        !*  character *50 name
        !*  integer ok,getInt,getReal
        !*  ! get visco-plastic parameters
        !*  nuViscoPlastic=1.   ! default value
        !*  etaViscoPlastic=1.
        !*  yieldStressViscoPlastic=10.
        !*  exponentViscoPlastic=10.
        !*  epsViscoPlastic=1.e-10   ! small parameter used to offset the effective strain rate 
        !* 
        !*  name ='nuViscoPlastic'
        !*  ok = getReal(pdb,name,nuViscoPlastic)
        !* 
        !*  if( ok.eq.1 )then
        !*    write(*,'("*** ut: name=",a10,", num=",e9.3)') name,nuViscoPlastic
        !*  else
        !*    write(*,'("*** ut: name=",a10,", NOT FOUND")') name
        !*  end if
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
        ! write(*,'(" entering lineSolveBcINSVP ")') 
        ! ****** Boundary Conditions ******
        indexRange(0,0)=n1a
        indexRange(1,0)=n1b
        indexRange(0,1)=n2a
        indexRange(1,1)=n2b
        indexRange(0,2)=n3a
        indexRange(1,2)=n3b
        ! Assign BC's on tangential directions -- 
        do axis=0,nd-1
         if( axis.ne.dir )then
         do side=0,1
          if( axis.eq.0 )then
            is1=1-2*side
            n1a=indexRange(side,axis)
            n1b=n1a
          else if( axis.eq.1 )then
            is2=1-2*side
            n2a=indexRange(side,axis)
            n2b=n2a
          else
            is3=1-2*side
            n3a=indexRange(side,axis)
            n3b=n3a
          end if
          if( boundaryCondition(side,axis)
     & .eq.dirichletBoundaryCondition .or.boundaryCondition(side,axis)
     & .eq.noSlipWall.or.boundaryCondition(side,axis)
     & .eq.inflowWithVelocityGiven )then
           if( systemComponent.eq.uc .or. systemComponent.eq.vc .or. 
     & systemComponent.eq.wc )then
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
             ! fill in the identity matrix on the boundary (dicihlet BC)
             am(i1,i2,i3)=0.
             bm(i1,i2,i3)=1.
             cm(i1,i2,i3)=0.
             end do
             end do
             end do
           else if( computeTemperature.ne.0 .and. 
     & systemComponent.eq.tc )then
            a0 = mixedCoeff(tc,side,axis,grid)
            a1 = mixedNormalCoeff(tc,side,axis,grid)
            if( a1.eq.0. )then
             ! Dirichlet BC for T
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
              ! fill in the identity matrix on the boundary (dicihlet BC)
              am(i1,i2,i3)=0.
              bm(i1,i2,i3)=1.
              cm(i1,i2,i3)=0.
              end do
              end do
              end do
            else
             if( boundaryCondition(side,axis)
     & .eq.inflowWithVelocityGiven )then
               write(*,'(" insLineBC: mixed BC at inflow!")')
             end if
            end if
           end if
          else if( boundaryCondition(side,axis).eq.outflow )then
           ! leave as is
          else if( boundaryCondition(side,axis).eq.slipWall )then
           if( systemComponent.eq.(uc+axis) )then
             ! normal component is dirichlet (leave other components as the eqn)
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
             ! fill in the identity matrix on the boundary 
             am(i1,i2,i3)=0.
             bm(i1,i2,i3)=1.
             cm(i1,i2,i3)=0.
             end do
             end do
             end do
           end if
          else if( boundaryCondition(side,axis).gt.0 )then
            write(*,'("insLineBC: ERROR unknown bc=",i4)') 
     & boundaryCondition(side,axis)
          end if
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
         end do ! side
         end if
        end do ! axis
       ! assign loop variables to correspond to the boundary
        epsx = 1.e-20   ! for normal, fix me, use REAL_MIN*100 ??
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
         if( bc(side,ibc).eq.dirichlet .and. boundaryCondition(side,
     & dir).gt.0 )then
           do i3=n3a,n3b
           do i2=n2a,n2b
           do i1=n1a,n1b
           j1=i1+is1
           j2=i2+is2
           j3=i3+is3
           if( mask(i1,i2,i3).gt.0 )then
            ! fill in the identity matrix on the boundary and ghost line 
            am(i1,i2,i3)=0.
            bm(i1,i2,i3)=1.
            cm(i1,i2,i3)=0.
            am(j1,j2,j3)=0.
            bm(j1,j2,j3)=1.
            cm(j1,j2,j3)=0.
           else
            ! for interpolation points or unused:
            am(i1,i2,i3)=0.
            bm(i1,i2,i3)=1.
            cm(i1,i2,i3)=0.
            am(j1,j2,j3)=0.
            bm(j1,j2,j3)=1.
            cm(j1,j2,j3)=0.
           end if
           end do
           end do
           end do
         else if( bc(side,ibc).eq.dirichlet .and. boundaryCondition(
     & side,dir).le.0 )then
          ! this must be an internal parallel boundary
           do i3=n3a,n3b
           do i2=n2a,n2b
           do i1=n1a,n1b
           if( mask(i1,i2,i3).gt.0 )then
            ! fill in the identity matrix on the boundary 
            am(i1,i2,i3)=0.
            bm(i1,i2,i3)=1.
            cm(i1,i2,i3)=0.
           else
            ! for interpolation points or unused:
            am(i1,i2,i3)=0.
            bm(i1,i2,i3)=1.
            cm(i1,i2,i3)=0.
           end if
           end do
           end do
           end do
         else if( bc(side,ibc).eq.neumann )then
           ! apply a neumann BC on this side.
           !             | b[0] c[0] a[0]                |
           !             | a[1] b[1] c[1]                |
           !         A = |      a[2] b[2] c[2]           |
           !             |            .    .    .        |
           !             |                a[.] b[.] c[.] |
           !             |                c[n] a[n] b[n] |
          ! write(*,'(">>>insLineSolveBC: neumann BC for side,dir,grid=",3i3, " ibc,computeT,systemComponent,nc=",4i3)') side,dir,grid,ibc,computeTemperature,systemComponent,numberOfComponents
          ! ' 
          if( computeTemperature.ne.0 .and. systemComponent.eq.tc 
     & .and. boundaryCondition(side,dir).ne.slipWall)then
           ! mixed boundary condition on T 
           a0 = mixedCoeff(tc,side,dir,grid)
           a1 = mixedNormalCoeff(tc,side,dir,grid)
           ! write(*,'(" insLineSolveBC: T BC: (a0,a1)=(",f3.1,",",f3.1,") for side,dir,grid=",3i3)') a0,a1,side,dir,grid
           ! '
           ! write(*,*) 'bcData=',bcData
           ! a0*u + a1*u.n = 
           ! a0*u + a1*( n1*( rx*ur + sx*us ) + n2*( ry*ur + sy*us ) )
           !  n1 = rsxy(dir,0), n2=rsxy(dir,1)
           if( gridType.eq.rectangular )then
            ! mixed-BC : rectangular
            if( side.eq.0 )then
             ! left side with outward normal : [1 0 -1] = [b c a]
             cnb= a1/(2.*dx(dir))
             cnc= a0
             cna=-cnb
            else
             ! right side with outward normal : [-1 0 1] = [c a b]
             cnc=-a1/(2.*dx(dir))
             cna= a0
             cnb=-cnc
            end if
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
             if( mask(i1,i2,i3).gt.0 )then
              am(i1,i2,i3)=cna
              bm(i1,i2,i3)=cnb
              cm(i1,i2,i3)=cnc
             else
              ! for interpolation points or unused:
              am(i1,i2,i3)=0.
              bm(i1,i2,i3)=1.
              cm(i1,i2,i3)=0.
             end if
             end do
             end do
             end do
           else if( gridType.eq.curvilinear )then
            if( nd.eq.2 )then
             ! mixed-BC : 2D curvilinear  
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
             if( mask(i1,i2,i3).gt.0 )then
              an1 = rsxy(i1,i2,i3,dir,0)
              an2 = rsxy(i1,i2,i3,dir,1)
              aNormi = sn/max( epsx,sqrt(an1**2 + an2**2) )  ! note: multiply by the sign of the normal
              an1=an1*aNormi
              an2=an2*aNormi
              ! cnm : coeff of ghost point
              ! cnp : coeff of first point in 
              cnm = -a1*( an1*rsxy(i1,i2,i3,dir,0) + an2*rsxy(i1,i2,i3,
     & dir,1) )/(2.*dr(dir))
              cnz = a0
              cnp = -cnm
              if( side.eq.0 )then
                bm(i1,i2,i3)=cnm
                cm(i1,i2,i3)=cnz
                am(i1,i2,i3)=cnp
              else
                cm(i1,i2,i3)=cnm
                am(i1,i2,i3)=cnz
                bm(i1,i2,i3)=cnp
              end if
             else
              ! for interpolation points or unused:
              am(i1,i2,i3)=0.
              bm(i1,i2,i3)=1.
              cm(i1,i2,i3)=0.
             end if
              end do
              end do
              end do
            else if( nd.eq.3 )then
             ! mixed-BC : 3D curvilinear  
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
             if( mask(i1,i2,i3).gt.0 )then
              an1 = rsxy(i1,i2,i3,dir,0)
              an2 = rsxy(i1,i2,i3,dir,1)
              an3 = rsxy(i1,i2,i3,dir,2)
              aNormi = sn/max( epsx,sqrt(an1**2 + an2**2 + an3**2) )  ! note: multiply by the sign of the normal
              an1=an1*aNormi
              an2=an2*aNormi
              an3=an3*aNormi
              cnm = -a1*( an1*rsxy(i1,i2,i3,dir,0) + an2*rsxy(i1,i2,i3,
     & dir,1) + an3*rsxy(i1,i2,i3,dir,2))/(2.*dr(dir))
              cnz = a0
              cnp = -cnm
              if( side.eq.0 )then
                bm(i1,i2,i3)=cnm
                cm(i1,i2,i3)=cnz
                am(i1,i2,i3)=cnp
              else
                cm(i1,i2,i3)=cnm
                am(i1,i2,i3)=cnz
                bm(i1,i2,i3)=cnp
              end if
             else
              ! for interpolation points or unused:
              am(i1,i2,i3)=0.
              bm(i1,i2,i3)=1.
              cm(i1,i2,i3)=0.
             end if
              end do
              end do
              end do
            end if
           else
             write(*,'(" lineSolveBC: unknown gridType=",i6)') gridType
             stop 10013
           end if
          else ! not Temperature
           if( side.eq.0 )then
            ! left side with outward normal : [1 0 -1] = [b c a]
            cnb= 1.
            cnc= 0.
            cna=-1.
            !      loopsMatrixBC(INSVP,!                    bm(i1,i2,i3)= 1.,!                    cm(i1,i2,i3)=0.,!                    am(i1,i2,i3)=-1.,,,)
           else
            ! right side with outward normal : [-1 0 1] = [c a b]
            cnc=-1.
            cna= 0.
            cnb= 1.
            !     loopsMatrixBC(INSVP,!                    cm(i1,i2,i3)=-1.,!                    am(i1,i2,i3)=0.,!                    bm(i1,i2,i3)= 1.,,,)
           end if
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
            if( mask(i1,i2,i3).gt.0 )then
             am(i1,i2,i3)=cna
             bm(i1,i2,i3)=cnb
             cm(i1,i2,i3)=cnc
            else
             ! for interpolation points or unused:
             am(i1,i2,i3)=0.
             bm(i1,i2,i3)=1.
             cm(i1,i2,i3)=0.
            end if
            end do
            end do
            end do
          end if
         else if( bc(side,ibc).eq.extrapolate )then
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
            write(*,*) 'ERROR: not implemeted: orderOfExtrapolation=',
     & orderOfExtrapolation
            stop 1111
          end if
           do i3=n3a,n3b
           do i2=n2a,n2b
           do i1=n1a,n1b
           if( mask(i1,i2,i3).gt.0 )then
            am(i1,i2,i3)=cexa
            bm(i1,i2,i3)=cexb
            cm(i1,i2,i3)=cexc
           else
            ! for interpolation points or unused:
            am(i1,i2,i3)=0.
            bm(i1,i2,i3)=1.
            cm(i1,i2,i3)=0.
           end if
           end do
           end do
           end do
          ! loopsMatrixBC( INSVP,!                am(i1,i2,i3)=cexa,!                bm(i1,i2,i3)=cexb,!                cm(i1,i2,i3)=cexc,,,)
         else if( bc(side,ibc).gt.0 )then
          write(*,'(" lineSolve:BC: unknown bc=",i2)') bc(side,ibc)
          stop 7102
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
        ! Now fix up the "ends" of some boundary conditions
c$$$ do side=0,1
c$$$  is1=0
c$$$  is2=0
c$$$  is3=0
c$$$  if( dir.eq.0 )then
c$$$    is1=1-2*side
c$$$    n1a=indexRange(side,dir)-is1    ! boundary is 1 pt outside
c$$$    n1b=n1a
c$$$  else if( dir.eq.1 )then
c$$$    is2=1-2*side
c$$$    n2a=indexRange(side,dir)-is2
c$$$    n2b=n2a
c$$$  else
c$$$    is3=1-2*side
c$$$    n3a=indexRange(side,dir)-is3
c$$$    n3b=n3a
c$$$  end if
c$$$    
c$$$  sn=2*side-1 ! sign for normal
c$$$
c$$$  if( boundaryCondition(side,axis).eq.slipWall )then
c$$$
c$$$    ! The last point on slip walls: extrapolate the tang. velocity rather than using 
c$$$    if( systemComponent.eq.(uc+axis) )then
c$$$      ! normal component is dirichlet (leave other components as the eqn)
c$$$   
c$$$     beginLoops()
c$$$      ! fill in the identity matrix on the boundary 
c$$$      am(i1,i2,i3)=0.
c$$$      bm(i1,i2,i3)=1.
c$$$      cm(i1,i2,i3)=0.
c$$$     endLoops()
c$$$    
c$$$    end if
c$$$
c$$$  end if
c$$$
c$$$
c$$$  ! reset values
c$$$  if( dir.eq.0 )then
c$$$    n1a=indexRange(0,dir)
c$$$    n1b=indexRange(1,dir)
c$$$  else if( dir.eq.1 )then
c$$$    n2a=indexRange(0,dir)
c$$$    n2b=indexRange(1,dir)
c$$$  else
c$$$    n3a=indexRange(0,dir)
c$$$    n3b=indexRange(1,dir)
c$$$  end if
c$$$ end do ! do side
        return
        end
