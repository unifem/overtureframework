! This file automatically generated from insLineSolveNew.bf with bpp.
      subroutine lineSolveResidualVP(nd,
     & n1a,n1b,n2a,n2b,n3a,n3b,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,
     & mask,rsxy,  u,gv,dt,f,dw, residual,
     & bc, ipar, rpar, ierr )
c======================================================================
c
c  *********** Compute the residual *****************
c
c nd : number of space dimensions
c
c u : input - current solution
c f : input rhs forcing
c
c dw: distance to wall for SA TM
c======================================================================
      implicit none
      integer nd, n1a,n1b,n2a,n2b,n3a,n3b,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b
      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real dw(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real residual(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
      real gv(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real dt(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real f(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer bc(0:1,0:*),ierr
      real dtScale,cfl
      integer ipar(0:*)
      real rpar(0:*)
c     ---- local variables -----
      integer m,n,c,kd,i1,i2,i3,orderOfAccuracy,gridIsMoving,
     & useWhereMask
      integer gridIsImplicit,implicitOption,implicitMethod,ibc,
     & isAxisymmetric,use2ndOrderAD,use4thOrderAD,
     & useSelfAdjointDiffusion,
     & orderOfExtrapolation,fourthOrder,dirp1,dirp2
      integer pc,uc,vc,wc,tc, vsc,fc,fcu,fcv,fcw,fcn,fct,grid,side,
     & gridType
      integer computeMatrix,computeRHS,computeMatrixBC,
     & computeTemperature
      integer twilightZoneFlow
      integer indexRange(0:1,0:2),gid(0:1,0:2),is1,is2,is3
      real nu,kThermal,thermalExpansivity,gravity(0:2)
      real dx(0:2),dx0,dy,dz,dxi,dyi,dzi,dri,dsi,dti
      real dxv2i(0:2),dx2i,dy2i,dz2i
      real dxvsqi(0:2),dxsqi,dysqi,dzsqi
      real drv2i(0:2),dr2i,ds2i,dt2i
      real drvsqi(0:2),drsqi,dssqi,dtsqi
      real dx12i,dy12i,dz12i,dxsq12i,dysq12i,dzsq12i,
     & dxy4i,dxz4i,dyz4i
      real ad21,ad22,ad41,ad42,cd22,cd42,adc,sn,adc2,adc4
      real ad21n,ad22n,ad41n,ad42n,cd22n,cd42n
      real dr(0:2)
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
     & cv1e3, cd0, cr0
      real dd,dndx(0:2)
      real chi,fnu1,fnu2,s,r,g,fw,dKappaSq,nBydSqLhs,nSqBydSq,nutb
      real nuTilde,nuT,nuTx(0:2),fv1,fv1x,fv1y,fv1z
      real nuTSA,chi3,nuTd
      real kbl,alpha,a0p,ccp,ckleb,cwk !baldwin-lomax constants
      integer itrip,jtrip,ktrip !baldwin-lomax trip location
      integer numberOfComponents,systemComponent
      integer nc
      integer rectangular,curvilinear
      parameter( rectangular=0, curvilinear=1 )
      integer pdeModel,standardModel,BoussinesqModel,viscoPlasticModel
      parameter( standardModel=0,BoussinesqModel=1,viscoPlasticModel=2 
     & )
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
      real u0,u0x,u0y,u0z
      real v0,v0x,v0y,v0z
      real w0,w0x,w0y,w0z
       real nu0ph,nu0mh,nu1ph,nu1mh,nu2ph,nu2mh
       real nuzzm,nuzmz,numzz,nuzzz,nupzz,nuzpz,nuzzp
       real ajzzm,ajzmz,ajmzz,ajzzz,ajpzz,ajzpz,ajzzp
       real a11ph,a11mh,a22ph,a22mh,a33ph,a33mh,a11mzz,a11zzz,a11pzz,
     & a22zmz,a22zzz,a22zpz,a33zzm,a33zzz,a33zzp,a12pzz,a12zzz,a12mzz,
     & a13pzz,a13zzz,a13mzz,a21zpz,a21zzz,a21zmz,a23zpz,a23zzz,a23zmz,
     & a31zzp,a31zzz,a31zzm,a32zzp,a32zzz,a32zzm
       real b11ph,b11mh,b22ph,b22mh,b33ph,b33mh,b11mzz,b11zzz,b11pzz,
     & b22zmz,b22zzz,b22zpz,b33zzm,b33zzz,b33zzp,b12pzz,b12zzz,b12mzz,
     & b13pzz,b13zzz,b13mzz,b21zpz,b21zzz,b21zmz,b23zpz,b23zzz,b23zmz,
     & b31zzp,b31zzz,b31zzm,b32zzp,b32zzz,b32zzm
       real c11ph,c11mh,c22ph,c22mh,c33ph,c33mh,c11mzz,c11zzz,c11pzz,
     & c22zmz,c22zzz,c22zpz,c33zzm,c33zzz,c33zzp,c12pzz,c12zzz,c12mzz,
     & c13pzz,c13zzz,c13mzz,c21zpz,c21zzz,c21zmz,c23zpz,c23zzz,c23zmz,
     & c31zzp,c31zzz,c31zzm,c32zzp,c32zzz,c32zzm
       real au11ph,au11mh,au22ph,au22mh,au33ph,au33mh,au11mzz,au11zzz,
     & au11pzz,au22zmz,au22zzz,au22zpz,au33zzm,au33zzz,au33zzp,
     & au12pzz,au12zzz,au12mzz,au13pzz,au13zzz,au13mzz,au21zpz,
     & au21zzz,au21zmz,au23zpz,au23zzz,au23zmz,au31zzp,au31zzz,
     & au31zzm,au32zzp,au32zzz,au32zzm
       real av11ph,av11mh,av22ph,av22mh,av33ph,av33mh,av11mzz,av11zzz,
     & av11pzz,av22zmz,av22zzz,av22zpz,av33zzm,av33zzz,av33zzp,
     & av12pzz,av12zzz,av12mzz,av13pzz,av13zzz,av13mzz,av21zpz,
     & av21zzz,av21zmz,av23zpz,av23zzz,av23zmz,av31zzp,av31zzz,
     & av31zzm,av32zzp,av32zzz,av32zzm
       real aw11ph,aw11mh,aw22ph,aw22mh,aw33ph,aw33mh,aw11mzz,aw11zzz,
     & aw11pzz,aw22zmz,aw22zzz,aw22zpz,aw33zzm,aw33zzz,aw33zzp,
     & aw12pzz,aw12zzz,aw12mzz,aw13pzz,aw13zzz,aw13mzz,aw21zpz,
     & aw21zzz,aw21zmz,aw23zpz,aw23zzz,aw23zmz,aw31zzp,aw31zzz,
     & aw31zzm,aw32zzp,aw32zzz,aw32zzm
       real bu11ph,bu11mh,bu22ph,bu22mh,bu33ph,bu33mh,bu11mzz,bu11zzz,
     & bu11pzz,bu22zmz,bu22zzz,bu22zpz,bu33zzm,bu33zzz,bu33zzp,
     & bu12pzz,bu12zzz,bu12mzz,bu13pzz,bu13zzz,bu13mzz,bu21zpz,
     & bu21zzz,bu21zmz,bu23zpz,bu23zzz,bu23zmz,bu31zzp,bu31zzz,
     & bu31zzm,bu32zzp,bu32zzz,bu32zzm
       real bv11ph,bv11mh,bv22ph,bv22mh,bv33ph,bv33mh,bv11mzz,bv11zzz,
     & bv11pzz,bv22zmz,bv22zzz,bv22zpz,bv33zzm,bv33zzz,bv33zzp,
     & bv12pzz,bv12zzz,bv12mzz,bv13pzz,bv13zzz,bv13mzz,bv21zpz,
     & bv21zzz,bv21zmz,bv23zpz,bv23zzz,bv23zmz,bv31zzp,bv31zzz,
     & bv31zzm,bv32zzp,bv32zzz,bv32zzm
       real bw11ph,bw11mh,bw22ph,bw22mh,bw33ph,bw33mh,bw11mzz,bw11zzz,
     & bw11pzz,bw22zmz,bw22zzz,bw22zpz,bw33zzm,bw33zzz,bw33zzp,
     & bw12pzz,bw12zzz,bw12mzz,bw13pzz,bw13zzz,bw13mzz,bw21zpz,
     & bw21zzz,bw21zmz,bw23zpz,bw23zzz,bw23zmz,bw31zzp,bw31zzz,
     & bw31zzm,bw32zzp,bw32zzz,bw32zzm
       real cu11ph,cu11mh,cu22ph,cu22mh,cu33ph,cu33mh,cu11mzz,cu11zzz,
     & cu11pzz,cu22zmz,cu22zzz,cu22zpz,cu33zzm,cu33zzz,cu33zzp,
     & cu12pzz,cu12zzz,cu12mzz,cu13pzz,cu13zzz,cu13mzz,cu21zpz,
     & cu21zzz,cu21zmz,cu23zpz,cu23zzz,cu23zmz,cu31zzp,cu31zzz,
     & cu31zzm,cu32zzp,cu32zzz,cu32zzm
       real cv11ph,cv11mh,cv22ph,cv22mh,cv33ph,cv33mh,cv11mzz,cv11zzz,
     & cv11pzz,cv22zmz,cv22zzz,cv22zpz,cv33zzm,cv33zzz,cv33zzp,
     & cv12pzz,cv12zzz,cv12mzz,cv13pzz,cv13zzz,cv13mzz,cv21zpz,
     & cv21zzz,cv21zmz,cv23zpz,cv23zzz,cv23zmz,cv31zzp,cv31zzz,
     & cv31zzm,cv32zzp,cv32zzz,cv32zzm
       real cw11ph,cw11mh,cw22ph,cw22mh,cw33ph,cw33mh,cw11mzz,cw11zzz,
     & cw11pzz,cw22zmz,cw22zzz,cw22zpz,cw33zzm,cw33zzz,cw33zzp,
     & cw12pzz,cw12zzz,cw12mzz,cw13pzz,cw13zzz,cw13mzz,cw21zpz,
     & cw21zzz,cw21zmz,cw23zpz,cw23zzz,cw23zmz,cw31zzp,cw31zzz,
     & cw31zzm,cw32zzp,cw32zzz,cw32zzm
c     ------------ start statement functions -------------------
      real rx,ry,rz,sx,sy,sz,tx,ty,tz
      real uu, ux2,uy2,uz2,uxx2,uyy2,uzz2,lap2d2,lap3d2
      real ux4,uy4,uz4,uxx4,lap2d4,lap3d4,uxy2,uxz2,uyz2,uxy4,uxz4,
     & uyz4,uyy4,uzz4
      real  ad2Coeff,ad2rCoeff,ad2,ad23Coeff,ad23rCoeff,ad23,ad4Coeff,
     & ad4rCoeff,ad4,ad43Coeff,ad43rCoeff,ad43
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
!      declareDifferenceOrder4(u,RX)
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
!      defineDifferenceOrder4Components1(u,RX)
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
!*       ux4(c)   = ux42r(i1,i2,i3,c)
!*       uy4(c)   = uy42r(i1,i2,i3,c)
!*       uz4(c)   = uz43r(i1,i2,i3,c)
!*       uxy4(c)  = uxy42r(i1,i2,i3,c)
!*       uxz4(c)  = uxz43r(i1,i2,i3,c) 
!*       uyz4(c)  = uyz43r(i1,i2,i3,c) 
!*       uxx4(c)  = uxx42r(i1,i2,i3,c) 
!*       uyy4(c)  = uyy42r(i1,i2,i3,c) 
!*       uzz4(c)  = uzz43r(i1,i2,i3,c) 
!*       lap2d4(c)= ulaplacian42r(i1,i2,i3,c)
!*       lap3d4(c)= ulaplacian43r(i1,i2,i3,c)
      ux2c(m) = ux22(i1,i2,i3,m)
      uy2c(m) = uy22(i1,i2,i3,m)
      ux3c(m) = ux23(i1,i2,i3,m)
      uy3c(m) = uy23(i1,i2,i3,m)
      uz3c(m) = uz23(i1,i2,i3,m)
      lap2d2c(m) = ulaplacian22(i1,i2,i3,m)
      lap3d2c(m) = ulaplacian23(i1,i2,i3,m)
c    --- 2nd order 2D artificial diffusion ---
      ad2Coeff()=(ad21 + cd22*
     &    ( abs(ux22(i1,i2,i3,uc))+abs(uy22(i1,i2,i3,uc))
     &     +abs(ux22(i1,i2,i3,vc))+abs(uy22(i1,i2,i3,vc)) ) )
      ad2rCoeff()=(ad21 + cd22*
     &    ( abs(ux22r(i1,i2,i3,uc))+abs(uy22r(i1,i2,i3,uc))
     &     +abs(ux22r(i1,i2,i3,vc))+abs(uy22r(i1,i2,i3,vc)) ) )
      ad2(adc,c)=adc*(u(i1+1,i2,i3,c)-4.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c)
     &               +u(i1,i2+1,i3,c)                 +u(i1,i2-1,i3,c))
c    --- 2nd order 3D artificial diffusion ---
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
      ad23(adc,c)=adc
     &    *(u(i1+1,i2,i3,c)-6.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c)
     &     +u(i1,i2+1,i3,c)                   +u(i1,i2-1,i3,c)
     &     +u(i1,i2,i3+1,c)                   +u(i1,i2,i3-1,c))
c     ---fourth-order artificial diffusion in 2D
      ad4Coeff()=(ad41 + cd42*
     &    ( abs(ux22(i1,i2,i3,uc))+abs(uy22(i1,i2,i3,uc))
     &     +abs(ux22(i1,i2,i3,vc))+abs(uy22(i1,i2,i3,vc)) ) )
      ad4rCoeff()=(ad41 + cd42*
     &    ( abs(ux22r(i1,i2,i3,uc))+abs(uy22r(i1,i2,i3,uc))
     &     +abs(ux22r(i1,i2,i3,vc))+abs(uy22r(i1,i2,i3,vc)) ) )
      ad4(adc,c)=adc
     &    *(   -u(i1+2,i2,i3,c)-u(i1-2,i2,i3,c)
     &         -u(i1,i2+2,i3,c)-u(i1,i2-2,i3,c)
     &     +4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)
     &         +u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c))
     &      -12.*u(i1,i2,i3,c) )
c     ---fourth-order artificial diffusion in 3D
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
      ad43(adc,c)=adc
     &    *(   -u(i1+2,i2,i3,c)-u(i1-2,i2,i3,c)
     &         -u(i1,i2+2,i3,c)-u(i1,i2-2,i3,c)
     &         -u(i1,i2,i3+2,c)-u(i1,i2,i3-2,c)
     &     +4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)
     &         +u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c)
     &         +u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c))
     &      -18.*u(i1,i2,i3,c) )
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


c ------------ end statement functions -------------------
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
      if( fourthOrder.eq.1 .and. turbulenceModel.ne.noTurbulenceModel )
     & then
        write(*,'("insLineSolve: ERROR: fourth-order only available 
     & for INS")')
        ! " '
        stop 6543
      end if
      if( turbulenceModel.eq.spalartAllmaras )then
        call getSpalartAllmarasParameters(cb1, cb2, cv1, sigma, sigmai,
     &  kappa, cw1, cw2, cw3, cw3e6, cv1e3, cd0, cr0)
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
      if( orderOfAccuracy.eq.2 )then
        if( gridType.eq.rectangular )then
          ! write(*,'("new: computeResidualVP, use2ndOrderAD=",i2)') use2ndOrderAD
          if( use4thOrderAD.eq.1 )then
           write(*,*) 'insLineSolve: computeResidualVP: 4th order diss 
     & not finished'
           stop 7654
          end if
          if( nd.eq.2 )then
           ! defineDerivativeMacros(DIM,ORDER,rectangular) : defineMacro UX(cc) ux22r(i1,i2,i3,cc) etc. 
           ! defineDerivativeMacros(2,2,rectangular)
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
           if( mask(i1,i2,i3).gt.0 )then
            ! Get the nonlinear viscosity at nearby points: 
            nuzmz=u(i1  ,i2-1,i3,vsc)
            numzz=u(i1-1,i2  ,i3,vsc)
            nuzzz=u(i1  ,i2  ,i3,vsc)
            nupzz=u(i1+1,i2  ,i3,vsc)
            nuzpz=u(i1  ,i2+1,i3,vsc)
            ! Evaluate the nonlinear viscosity "nu"
            ! getViscoPlasticViscosityCoefficient(nuzmz,i1  ,i2-1,i3,2,rectangular)
            ! getViscoPlasticViscosityCoefficient(numzz,i1-1,i2  ,i3,2,rectangular)
            ! getViscoPlasticViscosityCoefficient(nuzzz,i1  ,i2  ,i3,2,rectangular)
            ! getViscoPlasticViscosityCoefficient(nupzz,i1+1,i2  ,i3,2,rectangular)
            ! getViscoPlasticViscosityCoefficient(nuzpz,i1  ,i2+1,i3,2,rectangular)
             nu0ph = .5*( nupzz+nuzzz )  ! nu(i1+1/2,i2,i3)
             nu0mh = .5*( nuzzz+numzz )  ! nu(i1-1/2,i2,i3)
             nu1ph = .5*( nuzpz+nuzzz )  ! nu(i1,i2+1/2,i3)
             nu1mh = .5*( nuzzz+nuzmz )  ! nu(i1,i2-1/2,i3)
             ! u.t + u.grad(u) + p.x = Dx( 2*nu*u.x ) + Dy(   nu*u.y ) + Dy( nu*v.x )
             ! v.t + u.grad(v) + p.y = Dx(   nu*v.x ) + Dy( 2*nu*v.y ) + Dx( nu*u.y )
             residual(i1,i2,i3,uc)=f(i1,i2,i3,uc)-u(i1,i2,i3,uc)*ux2(
     & uc)-u(i1,i2,i3,vc)*uy2(uc)-ux2(pc)-thermalExpansivity*gravity(
     & 0)*u(i1,i2,i3,tc) +2.*(nu0ph*u(i1+1,i2,i3,uc) -(nu0ph+nu0mh)*u(
     & i1,i2,i3,uc) + nu0mh*u(i1-1,i2,i3,uc))*dxvsqi(0)+   (nu1ph*u(
     & i1,i2+1,i3,uc) -(nu1ph+nu1mh)*u(i1,i2,i3,uc) + nu1mh*u(i1,i2-1,
     & i3,uc))*dxvsqi(1)  +   (nuzpz*(u(i1+1,i2+1,i3,vc)-u(i1-1,i2+1,
     & i3,vc))-nuzmz*(u(i1+1,i2-1,i3,vc)-u(i1-1,i2-1,i3,vc)))/(4.*dx(
     & 0)*dx(1))
             residual(i1,i2,i3,vc)=f(i1,i2,i3,vc)-u(i1,i2,i3,uc)*ux2(
     & vc)-u(i1,i2,i3,vc)*uy2(vc)-uy2(pc)-thermalExpansivity*gravity(
     & 1)*u(i1,i2,i3,tc) +   (nu0ph*u(i1+1,i2,i3,vc) -(nu0ph+nu0mh)*u(
     & i1,i2,i3,vc) + nu0mh*u(i1-1,i2,i3,vc))*dxvsqi(0)+2.*(nu1ph*u(
     & i1,i2+1,i3,vc) -(nu1ph+nu1mh)*u(i1,i2,i3,vc) + nu1mh*u(i1,i2-1,
     & i3,vc))*dxvsqi(1)  +   (nupzz*(u(i1+1,i2+1,i3,uc)-u(i1+1,i2-1,
     & i3,uc))-numzz*(u(i1-1,i2+1,i3,uc)-u(i1-1,i2-1,i3,uc)))/(4.*dx(
     & 0)*dx(1))
             if( use2ndOrderAD.eq.1 )then
              residual(i1,i2,i3,uc)=residual(i1,i2,i3,uc)+
     & adSelfAdjoint2dR(i1,i2,i3,uc)
              residual(i1,i2,i3,vc)=residual(i1,i2,i3,vc)+
     & adSelfAdjoint2dR(i1,i2,i3,vc)
             end if
             if( computeTemperature.ne.0 )then
               residual(i1,i2,i3,tc)=f(i1,i2,i3,tc)-u(i1,i2,i3,uc)*ux2(
     & tc)-u(i1,i2,i3,vc)*uy2(tc)+kThermal*lap2d2(tc)
               ! --- artificial dissipation for T: do this for now: 
               if( use2ndOrderAD.eq.1 .and. use4thOrderAD.eq.0 )then
                 residual(i1,i2,i3,tc)=residual(i1,i2,i3,tc)+
     & adSelfAdjoint2dR(i1,i2,i3,tc)
               else if( use4thOrderAD.eq.1 )then
                ! compute adc2, adc4: 
                 u0x=ux22r(i1,i2,i3,uc)
                 u0y=uy22r(i1,i2,i3,uc)
                 v0x=ux22r(i1,i2,i3,vc)
                 v0y=uy22r(i1,i2,i3,vc)
                 adc = abs(u0x)+abs(u0y)+abs(v0x)+abs(v0y)
                 adc2= ad21 + cd22*adc
                 adc4= ad41 + cd42*adc
                residual(i1,i2,i3,tc)=residual(i1,i2,i3,tc)+ad2(adc2,
     & tc)+ad4(adc4,tc)
               end if
             end if
           end if
            end do
            end do
            end do
          else if( nd.eq.3 )then
           stop 2945
          else
           stop 888 ! unexpected value for nd
          end if
        else
          ! write(*,'("new: computeResidualVP, use2ndOrderAD=",i2)') use2ndOrderAD
          if( use4thOrderAD.eq.1 )then
           write(*,*) 'insLineSolve: computeResidualVP: 4th order diss 
     & not finished'
           stop 7654
          end if
          if( nd.eq.2 )then
           ! defineDerivativeMacros(DIM,ORDER,curvilinear) : defineMacro UX(cc) ux22r(i1,i2,i3,cc) etc. 
           ! defineDerivativeMacros(2,2,curvilinear)
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
           if( mask(i1,i2,i3).gt.0 )then
            ! Get the nonlinear viscosity at nearby points: 
            nuzmz=u(i1  ,i2-1,i3,vsc)
            numzz=u(i1-1,i2  ,i3,vsc)
            nuzzz=u(i1  ,i2  ,i3,vsc)
            nupzz=u(i1+1,i2  ,i3,vsc)
            nuzpz=u(i1  ,i2+1,i3,vsc)
            ! Evaluate the nonlinear viscosity "nu"
            ! getViscoPlasticViscosityCoefficient(nuzmz,i1  ,i2-1,i3,2,curvilinear)
            ! getViscoPlasticViscosityCoefficient(numzz,i1-1,i2  ,i3,2,curvilinear)
            ! getViscoPlasticViscosityCoefficient(nuzzz,i1  ,i2  ,i3,2,curvilinear)
            ! getViscoPlasticViscosityCoefficient(nupzz,i1+1,i2  ,i3,2,curvilinear)
            ! getViscoPlasticViscosityCoefficient(nuzpz,i1  ,i2+1,i3,2,curvilinear)
              ! ************ VP curvilinear case  ********************
             ! evaluate the jacobian at nearby points:
             ajzmz = (1./(rx(i1,i2-1,i3)*sy(i1,i2-1,i3)-ry(i1,i2-1,i3)*
     & sx(i1,i2-1,i3)))
             ajmzz = (1./(rx(i1-1,i2,i3)*sy(i1-1,i2,i3)-ry(i1-1,i2,i3)*
     & sx(i1-1,i2,i3)))
             ajzzz = (1./(rx(i1,i2,i3)*sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,
     & i2,i3)))
             ajpzz = (1./(rx(i1+1,i2,i3)*sy(i1+1,i2,i3)-ry(i1+1,i2,i3)*
     & sx(i1+1,i2,i3)))
             ajzpz = (1./(rx(i1,i2+1,i3)*sy(i1,i2+1,i3)-ry(i1,i2+1,i3)*
     & sx(i1,i2+1,i3)))
             ! 1. Get coefficients a11ph, a11mh, a22ph, etc. for 
             !          Dx( 2*nu*u.x ) + Dy(   nu*u.y ) 
              a11mzz = ajmzz*( (2.*numzz)*rx(i1-1,i2,i3)*rx(i1-1,i2,i3)
     &  + (numzz)*ry(i1-1,i2,i3)*ry(i1-1,i2,i3) )
              a11zzz = ajzzz*( (2.*nuzzz)*rx(i1  ,i2,i3)*rx(i1  ,i2,i3)
     &  + (nuzzz)*ry(i1  ,i2,i3)*ry(i1  ,i2,i3) )
              a11pzz = ajpzz*( (2.*nupzz)*rx(i1+1,i2,i3)*rx(i1+1,i2,i3)
     &  + (nupzz)*ry(i1+1,i2,i3)*ry(i1+1,i2,i3) )
              a11ph = .5*( a11zzz+a11pzz )
              a11mh = .5*( a11zzz+a11mzz )
              a22zmz = ajzmz*( (2.*nuzmz)*sx(i1,i2-1,i3)*sx(i1,i2-1,i3)
     &  + (nuzmz)*sy(i1,i2-1,i3)*sy(i1,i2-1,i3) )
              a22zzz = ajzzz*( (2.*nuzzz)*sx(i1,i2  ,i3)*sx(i1,i2  ,i3)
     &  + (nuzzz)*sy(i1,i2  ,i3)*sy(i1,i2  ,i3) )
              a22zpz = ajzpz*( (2.*nuzpz)*sx(i1,i2+1,i3)*sx(i1,i2+1,i3)
     &  + (nuzpz)*sy(i1,i2+1,i3)*sy(i1,i2+1,i3) )
              a22ph = .5*( a22zzz+a22zpz )
              a22mh = .5*( a22zzz+a22zmz )
              a12mzz = ajmzz*( (2.*numzz)*rx(i1-1,i2,i3)*sx(i1-1,i2,i3)
     &  + (numzz)*ry(i1-1,i2,i3)*sy(i1-1,i2,i3) )
              a12zzz = ajzzz*( (2.*nuzzz)*rx(i1  ,i2,i3)*sx(i1  ,i2,i3)
     &  + (nuzzz)*ry(i1  ,i2,i3)*sy(i1  ,i2,i3) )
              a12pzz = ajpzz*( (2.*nupzz)*rx(i1+1,i2,i3)*sx(i1+1,i2,i3)
     &  + (nupzz)*ry(i1+1,i2,i3)*sy(i1+1,i2,i3) )
              a21zmz = ajzmz*( (2.*nuzmz)*sx(i1,i2-1,i3)*rx(i1,i2-1,i3)
     &  + (nuzmz)*sy(i1,i2-1,i3)*ry(i1,i2-1,i3) )
              a21zzz = ajzzz*( (2.*nuzzz)*sx(i1,i2  ,i3)*rx(i1,i2  ,i3)
     &  + (nuzzz)*sy(i1,i2  ,i3)*ry(i1,i2  ,i3) )
              a21zpz = ajzpz*( (2.*nuzpz)*sx(i1,i2+1,i3)*rx(i1,i2+1,i3)
     &  + (nuzpz)*sy(i1,i2+1,i3)*ry(i1,i2+1,i3) )
             ! 1b. Get coefficients b11ph,b11mh, etc. for 
             !            Dy( nu*v.x )
              b11mzz = ajmzz*( (numzz)*ry(i1-1,i2,i3)*rx(i1-1,i2,i3) )
              b11zzz = ajzzz*( (nuzzz)*ry(i1  ,i2,i3)*rx(i1  ,i2,i3) )
              b11pzz = ajpzz*( (nupzz)*ry(i1+1,i2,i3)*rx(i1+1,i2,i3) )
              b11ph = .5*( b11zzz+b11pzz )
              b11mh = .5*( b11zzz+b11mzz )
              b22zmz = ajzmz*( (nuzmz)*sy(i1,i2-1,i3)*sx(i1,i2-1,i3) )
              b22zzz = ajzzz*( (nuzzz)*sy(i1,i2  ,i3)*sx(i1,i2  ,i3) )
              b22zpz = ajzpz*( (nuzpz)*sy(i1,i2+1,i3)*sx(i1,i2+1,i3) )
              b22ph = .5*( b22zzz+b22zpz )
              b22mh = .5*( b22zzz+b22zmz )
              b12mzz = ajmzz*( (numzz)*ry(i1-1,i2,i3)*sx(i1-1,i2,i3) )
              b12zzz = ajzzz*( (nuzzz)*ry(i1  ,i2,i3)*sx(i1  ,i2,i3) )
              b12pzz = ajpzz*( (nupzz)*ry(i1+1,i2,i3)*sx(i1+1,i2,i3) )
              b21zmz = ajzmz*( (nuzmz)*sy(i1,i2-1,i3)*rx(i1,i2-1,i3) )
              b21zzz = ajzzz*( (nuzzz)*sy(i1,i2  ,i3)*rx(i1,i2  ,i3) )
              b21zpz = ajzpz*( (nuzpz)*sy(i1,i2+1,i3)*rx(i1,i2+1,i3) )
             residual(i1,i2,i3,uc)=f(i1,i2,i3,uc)-u(i1,i2,i3,uc)*ux2c(
     & uc)-u(i1,i2,i3,vc)*uy2c(uc)-ux2c(pc)-thermalExpansivity*
     & gravity(0)*u(i1,i2,i3,tc) + ( + ( a11ph*(u(i1+1,i2,i3,uc)-u(i1,
     & i2,i3,uc)) - a11mh*(u(i1,i2,i3,uc)-u(i1-1,i2,i3,uc)) )/dr(0)**
     & 2+ ( a22ph*(u(i1,i2+1,i3,uc)-u(i1,i2,i3,uc)) - a22mh*(u(i1,i2,
     & i3,uc)-u(i1,i2-1,i3,uc)) )/dr(1)**2+ (a12pzz*(u(i1+1,i2+1,i3,
     & uc)-u(i1+1,i2-1,i3,uc))-a12mzz*(u(i1-1,i2+1,i3,uc)-u(i1-1,i2-1,
     & i3,uc)))/(4.*dr(0)*dr(1))+ (a21zpz*(u(i1+1,i2+1,i3,uc)-u(i1-1,
     & i2+1,i3,uc))-a21zmz*(u(i1+1,i2-1,i3,uc)-u(i1-1,i2-1,i3,uc)))/(
     & 4.*dr(0)*dr(1))+ ( b11ph*(u(i1+1,i2,i3,vc)-u(i1,i2,i3,vc)) - 
     & b11mh*(u(i1,i2,i3,vc)-u(i1-1,i2,i3,vc)) )/dr(0)**2+ ( b22ph*(u(
     & i1,i2+1,i3,vc)-u(i1,i2,i3,vc)) - b22mh*(u(i1,i2,i3,vc)-u(i1,i2-
     & 1,i3,vc)) )/dr(1)**2+ (b12pzz*(u(i1+1,i2+1,i3,vc)-u(i1+1,i2-1,
     & i3,vc))-b12mzz*(u(i1-1,i2+1,i3,vc)-u(i1-1,i2-1,i3,vc)))/(4.*dr(
     & 0)*dr(1))+ (b21zpz*(u(i1+1,i2+1,i3,vc)-u(i1-1,i2+1,i3,vc))-
     & b21zmz*(u(i1+1,i2-1,i3,vc)-u(i1-1,i2-1,i3,vc)))/(4.*dr(0)*dr(1)
     & ) )/ajzzz
             ! 2. Dx( nu*v.x ) + Dy( 2*nu*v.y ) 
              a11mzz = ajmzz*( (numzz)*rx(i1-1,i2,i3)*rx(i1-1,i2,i3) + 
     & (2.*numzz)*ry(i1-1,i2,i3)*ry(i1-1,i2,i3) )
              a11zzz = ajzzz*( (nuzzz)*rx(i1  ,i2,i3)*rx(i1  ,i2,i3) + 
     & (2.*nuzzz)*ry(i1  ,i2,i3)*ry(i1  ,i2,i3) )
              a11pzz = ajpzz*( (nupzz)*rx(i1+1,i2,i3)*rx(i1+1,i2,i3) + 
     & (2.*nupzz)*ry(i1+1,i2,i3)*ry(i1+1,i2,i3) )
              a11ph = .5*( a11zzz+a11pzz )
              a11mh = .5*( a11zzz+a11mzz )
              a22zmz = ajzmz*( (nuzmz)*sx(i1,i2-1,i3)*sx(i1,i2-1,i3) + 
     & (2.*nuzmz)*sy(i1,i2-1,i3)*sy(i1,i2-1,i3) )
              a22zzz = ajzzz*( (nuzzz)*sx(i1,i2  ,i3)*sx(i1,i2  ,i3) + 
     & (2.*nuzzz)*sy(i1,i2  ,i3)*sy(i1,i2  ,i3) )
              a22zpz = ajzpz*( (nuzpz)*sx(i1,i2+1,i3)*sx(i1,i2+1,i3) + 
     & (2.*nuzpz)*sy(i1,i2+1,i3)*sy(i1,i2+1,i3) )
              a22ph = .5*( a22zzz+a22zpz )
              a22mh = .5*( a22zzz+a22zmz )
              a12mzz = ajmzz*( (numzz)*rx(i1-1,i2,i3)*sx(i1-1,i2,i3) + 
     & (2.*numzz)*ry(i1-1,i2,i3)*sy(i1-1,i2,i3) )
              a12zzz = ajzzz*( (nuzzz)*rx(i1  ,i2,i3)*sx(i1  ,i2,i3) + 
     & (2.*nuzzz)*ry(i1  ,i2,i3)*sy(i1  ,i2,i3) )
              a12pzz = ajpzz*( (nupzz)*rx(i1+1,i2,i3)*sx(i1+1,i2,i3) + 
     & (2.*nupzz)*ry(i1+1,i2,i3)*sy(i1+1,i2,i3) )
              a21zmz = ajzmz*( (nuzmz)*sx(i1,i2-1,i3)*rx(i1,i2-1,i3) + 
     & (2.*nuzmz)*sy(i1,i2-1,i3)*ry(i1,i2-1,i3) )
              a21zzz = ajzzz*( (nuzzz)*sx(i1,i2  ,i3)*rx(i1,i2  ,i3) + 
     & (2.*nuzzz)*sy(i1,i2  ,i3)*ry(i1,i2  ,i3) )
              a21zpz = ajzpz*( (nuzpz)*sx(i1,i2+1,i3)*rx(i1,i2+1,i3) + 
     & (2.*nuzpz)*sy(i1,i2+1,i3)*ry(i1,i2+1,i3) )
             ! 2b.  Dx( nu*u.y )
              b11mzz = ajmzz*( (numzz)*rx(i1-1,i2,i3)*ry(i1-1,i2,i3) )
              b11zzz = ajzzz*( (nuzzz)*rx(i1  ,i2,i3)*ry(i1  ,i2,i3) )
              b11pzz = ajpzz*( (nupzz)*rx(i1+1,i2,i3)*ry(i1+1,i2,i3) )
              b11ph = .5*( b11zzz+b11pzz )
              b11mh = .5*( b11zzz+b11mzz )
              b22zmz = ajzmz*( (nuzmz)*sx(i1,i2-1,i3)*sy(i1,i2-1,i3) )
              b22zzz = ajzzz*( (nuzzz)*sx(i1,i2  ,i3)*sy(i1,i2  ,i3) )
              b22zpz = ajzpz*( (nuzpz)*sx(i1,i2+1,i3)*sy(i1,i2+1,i3) )
              b22ph = .5*( b22zzz+b22zpz )
              b22mh = .5*( b22zzz+b22zmz )
              b12mzz = ajmzz*( (numzz)*rx(i1-1,i2,i3)*sy(i1-1,i2,i3) )
              b12zzz = ajzzz*( (nuzzz)*rx(i1  ,i2,i3)*sy(i1  ,i2,i3) )
              b12pzz = ajpzz*( (nupzz)*rx(i1+1,i2,i3)*sy(i1+1,i2,i3) )
              b21zmz = ajzmz*( (nuzmz)*sx(i1,i2-1,i3)*ry(i1,i2-1,i3) )
              b21zzz = ajzzz*( (nuzzz)*sx(i1,i2  ,i3)*ry(i1,i2  ,i3) )
              b21zpz = ajzpz*( (nuzpz)*sx(i1,i2+1,i3)*ry(i1,i2+1,i3) )
             residual(i1,i2,i3,vc)=f(i1,i2,i3,vc)-u(i1,i2,i3,uc)*ux2c(
     & vc)-u(i1,i2,i3,vc)*uy2c(vc)-uy2c(pc)-thermalExpansivity*
     & gravity(1)*u(i1,i2,i3,tc) + ( + ( a11ph*(u(i1+1,i2,i3,vc)-u(i1,
     & i2,i3,vc)) - a11mh*(u(i1,i2,i3,vc)-u(i1-1,i2,i3,vc)) )/dr(0)**
     & 2+ ( a22ph*(u(i1,i2+1,i3,vc)-u(i1,i2,i3,vc)) - a22mh*(u(i1,i2,
     & i3,vc)-u(i1,i2-1,i3,vc)) )/dr(1)**2+ (a12pzz*(u(i1+1,i2+1,i3,
     & vc)-u(i1+1,i2-1,i3,vc))-a12mzz*(u(i1-1,i2+1,i3,vc)-u(i1-1,i2-1,
     & i3,vc)))/(4.*dr(0)*dr(1))+ (a21zpz*(u(i1+1,i2+1,i3,vc)-u(i1-1,
     & i2+1,i3,vc))-a21zmz*(u(i1+1,i2-1,i3,vc)-u(i1-1,i2-1,i3,vc)))/(
     & 4.*dr(0)*dr(1))+ ( b11ph*(u(i1+1,i2,i3,uc)-u(i1,i2,i3,uc)) - 
     & b11mh*(u(i1,i2,i3,uc)-u(i1-1,i2,i3,uc)) )/dr(0)**2+ ( b22ph*(u(
     & i1,i2+1,i3,uc)-u(i1,i2,i3,uc)) - b22mh*(u(i1,i2,i3,uc)-u(i1,i2-
     & 1,i3,uc)) )/dr(1)**2+ (b12pzz*(u(i1+1,i2+1,i3,uc)-u(i1+1,i2-1,
     & i3,uc))-b12mzz*(u(i1-1,i2+1,i3,uc)-u(i1-1,i2-1,i3,uc)))/(4.*dr(
     & 0)*dr(1))+ (b21zpz*(u(i1+1,i2+1,i3,uc)-u(i1-1,i2+1,i3,uc))-
     & b21zmz*(u(i1+1,i2-1,i3,uc)-u(i1-1,i2-1,i3,uc)))/(4.*dr(0)*dr(1)
     & ) )/ajzzz
             if( use2ndOrderAD.eq.1 )then
              residual(i1,i2,i3,uc)=residual(i1,i2,i3,uc)+
     & adSelfAdjoint2dC(i1,i2,i3,uc)
              residual(i1,i2,i3,vc)=residual(i1,i2,i3,vc)+
     & adSelfAdjoint2dC(i1,i2,i3,vc)
             end if
             if( computeTemperature.ne.0 )then
               residual(i1,i2,i3,tc)=f(i1,i2,i3,tc)-u(i1,i2,i3,uc)*
     & ux2c(tc)-u(i1,i2,i3,vc)*uy2c(tc)+kThermal*lap2d2c(tc)
               ! --- artificial dissipation for T: do this for now: 
               if( use2ndOrderAD.eq.1 .and. use4thOrderAD.eq.0 )then
                 residual(i1,i2,i3,tc)=residual(i1,i2,i3,tc)+
     & adSelfAdjoint2dC(i1,i2,i3,tc)
               else if( use4thOrderAD.eq.1 )then
               ! compute adc2, adc4: 
                 u0x=ux22(i1,i2,i3,uc)
                 u0y=uy22(i1,i2,i3,uc)
                 v0x=ux22(i1,i2,i3,vc)
                 v0y=uy22(i1,i2,i3,vc)
                 adc = abs(u0x)+abs(u0y)+abs(v0x)+abs(v0y)
                 adc2= ad21 + cd22*adc
                 adc4= ad41 + cd42*adc
                residual(i1,i2,i3,tc)=residual(i1,i2,i3,tc)+ad2(adc2,
     & tc)+ad4(adc4,tc)
               end if
             end if
           end if
            end do
            end do
            end do
          else if( nd.eq.3 )then
           stop 2945
          else
           stop 888 ! unexpected value for nd
          end if
        end if
      else ! order==4
      end if ! end order
      return
      end
