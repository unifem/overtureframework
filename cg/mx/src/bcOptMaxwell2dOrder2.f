! This file automatically generated from bcOptMaxwell.bf with bpp.
        subroutine bcOptMaxwell2dOrder2( nd, nd1a,nd1b,nd2a,nd2b,nd3a,
     & nd3b,ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,gridIndexRange,
     & dimension,u,f,mask,rsxy, xy,bc, boundaryCondition, ipar, rpar, 
     & ierr )
       ! ===================================================================================
       !  Optimised Boundary conditions for Maxwell's Equations. '
       !
       !  gridType : 0=rectangular, 1=curvilinear
       !  useForcing : 1=use f for RHS to BC
       !  side,axis : 0:1 and 0:2
       ! ===================================================================================
        implicit none
        integer nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, ndf1a,ndf1b,ndf2a,
     & ndf2b,ndf3a,ndf3b,n1a,n1b,n2a,n2b,n3a,n3b, ndc, bc,ierr
        real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
        real f(ndf1a:ndf1b,ndf2a:ndf2b,ndf3a:ndf3b,0:*)
        integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
        real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
        real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
        integer gridIndexRange(0:1,0:2),dimension(0:1,0:2)
        integer ipar(0:*),boundaryCondition(0:1,0:2)
         real rpar(0:*),pwc(0:5)
       !     --- local variables ----
        integer md1a,md1b,md2a,md2b,md3a,md3b
        integer indexRange(0:1,0:2),isPeriodic(0:2) ! used in call to periodic update
        real ep ! holds the pointer to the TZ function
        real pu ! holds pointer to P++ array
        real dt,kx,ky,kz,eps,mu,c,cc,twoPi,slowStartInterval,ssf,ssft,
     & ssftt,ssfttt,ssftttt,tt
        integer is1,is2,is3,js1,js2,js3,ks1,ks2,ks3,orderOfAccuracy,
     & gridType,debug,grid,side,axis,useForcing,ex,ey,ez,hx,hy,hz,
     & useWhereMask,side1,side2,side3,m1,m2,m3,bc1,bc2, js1a,js2a,
     & js3a,ks1a,ks2a,ks3a,forcingOption,useChargeDensity,fieldOption
        real dr(0:2), dx(0:2), t, uv(0:5), uvm(0:5), uv0(0:5), uvp(0:5)
     & , uvm2(0:5), uvp2(0:5)
        real uvmm(0:2),uvzm(0:2),uvpm(0:2)
        real uvmz(0:2),uvzz(0:2),uvpz(0:2)
        real uvmp(0:2),uvzp(0:2),uvpp(0:2)
        integer i10,i20,i30
        real jac3di(-2:2,-2:2,-2:2)
        integer orderOfExtrapolation
        logical setCornersToExact
        logical extrapInterpGhost   ! extrapolate ghost points next to boundary interpolation points
        ! boundary conditions parameters
! define BC parameters for fortran routines
! boundary conditions
      integer dirichlet,perfectElectricalConductor,
     & perfectMagneticConductor,planeWaveBoundaryCondition,
     & interfaceBC,symmetryBoundaryCondition,abcEM2,abcPML,abc3,abc4,
     & abc5,rbcNonLocal,rbcLocal,lastBC
      parameter( dirichlet=1,perfectElectricalConductor=2,
     & perfectMagneticConductor=3,planeWaveBoundaryCondition=4,
     & symmetryBoundaryCondition=5,interfaceBC=6,abcEM2=7,abcPML=8,
     & abc3=9,abc4=10,abc5=11,rbcNonLocal=12,rbcLocal=13,lastBC=13 )
        integer rectangular,curvilinear
        parameter(rectangular=0,curvilinear=1)
        ! forcing options
      ! forcingOptions -- these should match ForcingEnum in Maxwell.h 
      integer noForcing,magneticSinusoidalPointSource,gaussianSource,
     & twilightZoneForcing,planeWaveBoundaryForcing, 
     & gaussianChargeSource, userDefinedForcingOption
      parameter(noForcing                =0,
     & magneticSinusoidalPointSource =1,gaussianSource                
     & =2,twilightZoneForcing           =3,planeWaveBoundaryForcing   
     &    =4,    gaussianChargeSource          =5,
     & userDefinedForcingOption      =6 )
        integer i1,i2,i3,j1,j2,j3,axisp1,axisp2,en1,et1,et2,hn1,ht1,
     & ht2,numberOfGhostPoints
        integer extra,extra1a,extra1b,extra2a,extra2b,extra3a,extra3b
        real det,dra,dsa,dta,dxa,dya,dza,drb,dsb,dtb
        real uttp1,uttp2,uttm1,uttm2, vttp1,vttp2,vttm1,vttm2, utts, 
     & vtts
        real tau1,tau2,tau11,tau12,tau13, tau21,tau22,tau23
        real tau11s,tau12s,tau13s, tau21s,tau22s,tau23s
        real tau11t,tau12t,tau13t, tau21t,tau22t,tau23t
        real tau1u,tau2u,tau1Up1,tau1Up2,tau1Up3,tau2Up1,tau2Up2,
     & tau2Up3
        real tau1Dotu,tau2Dotu,tauU,tauUp1,tauUp2,tauUp3,ttu1,ttu2
        real ttu11,ttu12,ttu13, ttu21,ttu22,ttu23
        real DtTau1DotUvr,DtTau2DotUvr,DsTau1DotUvr,DsTau2DotUvr,
     & tau1DotUtt,tau2DotUtt,Da1DotU,a1DotU,a1Dotur
        real drA1DotDeltaU
       ! real tau1DotUvrs, tau2DotUvrs, tau1DotUvrt, tau2DotUvrt
        real gx1,gx2,g1a,g2a
        real g1,g2,g3
        real tauDotExtrap
        real u0t,v0t,w0t
        real jac,jacm1,jacp1,jacp2,jacm2,jac0,detnt
        real a11,a12,a13,a21,a22,a23,a31,a32,a33
        real a11r,a12r,a13r,a21r,a22r,a23r,a31r,a32r,a33r
        real a11s,a12s,a13s,a21s,a22s,a23s,a31s,a32s,a33s
        real a11t,a12t,a13t,a21t,a22t,a23t,a31t,a32t,a33t
        real a11rr,a12rr,a13rr,a21rr,a22rr,a23rr,a31rr,a32rr,a33rr
        real a11ss,a12ss,a13ss,a21ss,a22ss,a23ss,a31ss,a32ss,a33ss
        real a11tt,a12tt,a13tt,a21tt,a22tt,a23tt,a31tt,a32tt,a33tt
        real a11rs,a12rs,a13rs,a21rs,a22rs,a23rs,a31rs,a32rs,a33rs
        real a11rt,a12rt,a13rt,a21rt,a22rt,a23rt,a31rt,a32rt,a33rt
        real a11st,a12st,a13st,a21st,a22st,a23st,a31st,a32st,a33st
        real a11rrs,a12rrs,a13rrs,a21rrs,a22rrs,a23rrs,a31rrs,a32rrs,
     & a33rrs
        real a11sss,a12sss,a13sss,a21sss,a22sss,a23sss,a31sss,a32sss,
     & a33sss
        real a11rss,a12rss,a13rss,a21rss,a22rss,a23rss,a31rss,a32rss,
     & a33rss
        real a11ttt,a12ttt,a13ttt,a21ttt,a22ttt,a23ttt,a31ttt,a32ttt,
     & a33ttt
        real a11rtt,a12rtt,a13rtt,a21rtt,a22rtt,a23rtt,a31rtt,a32rtt,
     & a33rtt
        real a11sst,a12sst,a13sst,a21sst,a22sst,a23sst,a31sst,a32sst,
     & a33sst
        real a11stt,a12stt,a13stt,a21stt,a22stt,a23stt,a31stt,a32stt,
     & a33stt
        real a11zm1,a12zm1,a13zm1,a21zm1,a22zm1,a23zm1,a31zm1,a32zm1,
     & a33zm1
        real a11zp1,a12zp1,a13zp1,a21zp1,a22zp1,a23zp1,a31zp1,a32zp1,
     & a33zp1
        real a11zm2,a12zm2,a13zm2,a21zm2,a22zm2,a23zm2,a31zm2,a32zm2,
     & a33zm2
        real a11zp2,a12zp2,a13zp2,a21zp2,a22zp2,a23zp2,a31zp2,a32zp2,
     & a33zp2
        real a11m,a12m,a13m,a21m,a22m,a23m,a31m,a32m,a33m
        real a11p,a12p,a13p,a21p,a22p,a23p,a31p,a32p,a33p
        real a11m1,a12m1,a13m1,a21m1,a22m1,a23m1,a31m1,a32m1,a33m1
        real a11p1,a12p1,a13p1,a21p1,a22p1,a23p1,a31p1,a32p1,a33p1
        real a11m2,a12m2,a13m2,a21m2,a22m2,a23m2,a31m2,a32m2,a33m2
        real a11p2,a12p2,a13p2,a21p2,a22p2,a23p2,a31p2,a32p2,a33p2
        real c11,c22,c33,c1,c2,c3
        real c11r,c22r,c33r,c1r,c2r,c3r
        real c11s,c22s,c33s,c1s,c2s,c3s
        real c11t,c22t,c33t,c1t,c2t,c3t
        real uex,uey,uez
        real ur,us,ut,urr, uss,utt,urs,urt,ust, urrr,usss,uttt,urrs,
     & urss,urtt,usst,ustt, urrrr,ussss,urrss,urrrs,ursss
        real vr,vs,vt,vrr, vss,vtt,vrs,vrt,vst, vrrr,vsss,vttt,vrrs,
     & vrss,vrtt,vsst,vstt, vrrrr,vssss,vrrss,vrrrs,vrsss
        real wr,ws,wt,wrr, wss,wtt,wrs,wrt,wst, wrrr,wsss,wttt,wrrs,
     & wrss,wrtt,wsst,wstt, wrrrr,wssss,wrrss,wrrrs,wrsss
        real ursm,urrsm,vrsm,vrrsm, urrm,vrrm
        real uxx,uyy,uzz, vxx,vyy,vzz, wxx,wyy,wzz
        real uxxm2,uyym2,uzzm2, vxxm2,vyym2,vzzm2, wxxm2,wyym2,wzzm2
        real uxxm1,uyym1,uzzm1, vxxm1,vyym1,vzzm1, wxxm1,wyym1,wzzm1
        real uxxp1,uyyp1,uzzp1, vxxp1,vyyp1,vzzp1, wxxp1,wyyp1,wzzp1
        real uxxp2,uyyp2,uzzp2, vxxp2,vyyp2,vzzp2, wxxp2,wyyp2,wzzp2
        real cur,cvr,gI,gIa,gIII,gIV,gIVf
        real uTmTm,vTmTm,wTmTm
        real uTmTmr,vTmTmr,wTmTmr
        real ut0,vt0,utp1,vtp1,utm1,vtm1,uttt0,vttt0
        real uzm,uzp,vzm,vzp,wzm,wzp,wx,wy
        real b3u,b3v,b3w, b2u,b2v,b2w, b1u,b1v,b1w, bf,divtt
        real cw1,cw2,bfw2,fw1,fw2,fw3,fw4
        real fw1m1,fw1p1,wsm1,wsp1
        real f1um1,f1um2,f1vm1,f1vm2,f1wm1,f1wm2,f1f
        real f2um1,f2um2,f2vm1,f2vm2,f2wm1,f2wm2,f2f
        real cursu,cursv,cursw, cvrsu,cvrsv,cvrsw,  cwrsu,cwrsv,cwrsw
        real curtu,curtv,curtw, cvrtu,cvrtv,cvrtw,  cwrtu,cwrtv,cwrtw
        real furs,fvrs,fwrs, furt,fvrt,fwrt
        real a1DotUvrsRHS,a1DotUvrtRHS, a1DotUvrssRHS,a1DotUvrttRHS
        real gIII1,gIII2,gIVf1,gIVf2,gIV1,gIV2
        real uLap,vLap,wLap,tau1DotLap,tau2DotLap
        real cgI,gIf
        real aNorm,aDotUp,aDotUm,ctlrr,ctlr,div,divc,divc2,tauDotLap,
     & errLapex,errLapey,errLapez
        real aDot1,aDot2,aDotUm2,aDotUm1,aDotU,aDotUp1,aDotUp2,aDotUp3
        real xm,ym,x0,y0,z0,xp,yp,um,vm,wm,u0,v0,w0,up,vp,wp,x00,y00,
     & z00
        real tdu10,tdu01,tdu20,tdu02,gLu,gLv,utt00,vtt00,wtt00
        real cu10,cu01,cu20,cu02,cv10,cv01,cv20,cv02
        ! Here are time derivatives which are denoted using "d"
        real udd,vdd,wdd,uddp1,vddp1,wddp1,uddm1,vddm1,wddm1,uddp2,
     & vddp2,wddp2,uddm2,vddm2,wddm2
        real udds,vdds,wdds,uddt,vddt,wddt
        real maxDivc,maxTauDotLapu,maxExtrap,maxDr3aDotU,dr3aDotU,
     & a1Doturss
       ! real uxxx22r,uyyy22r,uxxx42r,uyyy42r,uxxxx22r,uyyyy22r, urrrr2,ussss2
        real urrrr2,ussss2
        real urrs4,urrt4,usst4,urss4,ustt4,urtt4
        real urrs2,urrt2,usst2,urss2,ustt2,urtt2
      real rsxyr2,rsxys2,rsxyt2,rsxyx22,rsxyy22,rsxyr4,rsxys4,rsxyx42,
     & rsxyy42
      real rsxyxs42, rsxyys42, rsxyxr42, rsxyyr42
      real rsxyrr2,rsxyss2,rsxyrs2, rsxyrr4,rsxyss4,rsxyrs4

      real rsxyx43,rsxyy43,rsxyz43,rsxyt4,rsxytt4,rsxyrt4,rsxyst4
      real rsxyxr43,rsxyxs43,rsxyxt43
      real rsxyyr43,rsxyys43,rsxyyt43
      real rsxyzr43,rsxyzs43,rsxyzt43

      real rsxyxr22,rsxyxs22,rsxyyr22,rsxyys22
      real rsxyx23,rsxyy23,rsxyz23,rsxytt2,rsxyrt2,rsxyst2
      real rsxyxr23,rsxyxs23,rsxyxt23
      real rsxyyr23,rsxyys23,rsxyyt23
      real rsxyzr23,rsxyzs23,rsxyzt23
       !     --- start statement function ----
        integer kd,m,n
        real rx,ry,rz,sx,sy,sz,tx,ty,tz
       ! old: include 'declareDiffOrder2f.h'
       ! old: include 'declareDiffOrder4f.h'
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
       ! define derivatives of rsxy
      rsxyr2(i1,i2,i3,m,n)=(rsxy(i1+1,i2,i3,m,n)-rsxy(i1-1,i2,i3,m,n))*
     & d12(0)
      rsxys2(i1,i2,i3,m,n)=(rsxy(i1,i2+1,i3,m,n)-rsxy(i1,i2-1,i3,m,n))*
     & d12(1)
      rsxyt2(i1,i2,i3,m,n)=(rsxy(i1,i2,i3+1,m,n)-rsxy(i1,i2,i3-1,m,n))*
     & d12(2)

      rsxyrr2(i1,i2,i3,m,n)=(rsxy(i1+1,i2,i3,m,n)-2.*rsxy(i1,i2,i3,m,n)
     & +rsxy(i1-1,i2,i3,m,n))*d22(0)
      rsxyss2(i1,i2,i3,m,n)=(rsxy(i1,i2+1,i3,m,n)-2.*rsxy(i1,i2,i3,m,n)
     & +rsxy(i1,i2-1,i3,m,n))*d22(1)
      rsxytt2(i1,i2,i3,m,n)=(rsxy(i1,i2,i3+1,m,n)-2.*rsxy(i1,i2,i3,m,n)
     & +rsxy(i1,i2,i3-1,m,n))*d22(2)

      rsxyx22(i1,i2,i3,m,n)= rx(i1,i2,i3)*rsxyr2(i1,i2,i3,m,n)+sx(i1,
     & i2,i3)*rsxys2(i1,i2,i3,m,n)
      rsxyy22(i1,i2,i3,m,n)= ry(i1,i2,i3)*rsxyr2(i1,i2,i3,m,n)+sy(i1,
     & i2,i3)*rsxys2(i1,i2,i3,m,n)


      ! check these again:
      !  -- 2nd -order ---

      rsxyrs2(i1,i2,i3,m,n)=(rsxyr2(i1,i2+1,i3,m,n)-rsxyr2(i1,i2-1,i3,
     & m,n))*d12(1)
      rsxyrt2(i1,i2,i3,m,n)=(rsxyr2(i1,i2,i3+1,m,n)-rsxyr2(i1,i2,i3-1,
     & m,n))*d12(2)
      rsxyst2(i1,i2,i3,m,n)=(rsxys2(i1,i2,i3+1,m,n)-rsxys2(i1,i2,i3-1,
     & m,n))*d12(2)

      rsxyxr22(i1,i2,i3,m,n)= rsxyr2(i1,i2,i3,0,0)*rsxyr2(i1,i2,i3,m,n)
     &  + rx(i1,i2,i3)*rsxyrr2(i1,i2,i3,m,n)+rsxyr2(i1,i2,i3,1,0)*
     & rsxys2(i1,i2,i3,m,n) + sx(i1,i2,i3)*rsxyrs2(i1,i2,i3,m,n)
      rsxyxs22(i1,i2,i3,m,n)= rsxys2(i1,i2,i3,0,0)*rsxyr2(i1,i2,i3,m,n)
     &  + rx(i1,i2,i3)*rsxyrs2(i1,i2,i3,m,n)+rsxys2(i1,i2,i3,1,0)*
     & rsxys2(i1,i2,i3,m,n) + sx(i1,i2,i3)*rsxyss2(i1,i2,i3,m,n)

      rsxyyr22(i1,i2,i3,m,n)= rsxyr2(i1,i2,i3,0,1)*rsxyr2(i1,i2,i3,m,n)
     &  + ry(i1,i2,i3)*rsxyrr2(i1,i2,i3,m,n)+rsxyr2(i1,i2,i3,1,1)*
     & rsxys2(i1,i2,i3,m,n) + sy(i1,i2,i3)*rsxyrs2(i1,i2,i3,m,n)
      rsxyys22(i1,i2,i3,m,n)= rsxys2(i1,i2,i3,0,1)*rsxyr2(i1,i2,i3,m,n)
     &  + ry(i1,i2,i3)*rsxyrs2(i1,i2,i3,m,n)+rsxys2(i1,i2,i3,1,1)*
     & rsxys2(i1,i2,i3,m,n) + sy(i1,i2,i3)*rsxyss2(i1,i2,i3,m,n)

      ! 3d versions -- check these again
      rsxyx23(i1,i2,i3,m,n)= rx(i1,i2,i3)*rsxyr2(i1,i2,i3,m,n)+sx(i1,
     & i2,i3)*rsxys2(i1,i2,i3,m,n)+tx(i1,i2,i3)*rsxyt2(i1,i2,i3,m,n)
      rsxyy23(i1,i2,i3,m,n)= ry(i1,i2,i3)*rsxyr2(i1,i2,i3,m,n)+sy(i1,
     & i2,i3)*rsxys2(i1,i2,i3,m,n)+ty(i1,i2,i3)*rsxyt2(i1,i2,i3,m,n)
      rsxyz23(i1,i2,i3,m,n)= rz(i1,i2,i3)*rsxyr2(i1,i2,i3,m,n)+sz(i1,
     & i2,i3)*rsxys2(i1,i2,i3,m,n)+tz(i1,i2,i3)*rsxyt2(i1,i2,i3,m,n)

      rsxyxr23(i1,i2,i3,m,n)= rsxyr2(i1,i2,i3,0,0)*rsxyr2(i1,i2,i3,m,n)
     &  + rx(i1,i2,i3)*rsxyrr2(i1,i2,i3,m,n)+rsxyr2(i1,i2,i3,1,0)*
     & rsxys2(i1,i2,i3,m,n) + sx(i1,i2,i3)*rsxyrs2(i1,i2,i3,m,n)+
     & rsxyr2(i1,i2,i3,2,0)*rsxyt2(i1,i2,i3,m,n) + tx(i1,i2,i3)*
     & rsxyrt2(i1,i2,i3,m,n)

      rsxyxs23(i1,i2,i3,m,n)= rsxys2(i1,i2,i3,0,0)*rsxyr2(i1,i2,i3,m,n)
     &  + rx(i1,i2,i3)*rsxyrs2(i1,i2,i3,m,n)+rsxys2(i1,i2,i3,1,0)*
     & rsxys2(i1,i2,i3,m,n) + sx(i1,i2,i3)*rsxyss2(i1,i2,i3,m,n)+
     & rsxys2(i1,i2,i3,2,0)*rsxyt2(i1,i2,i3,m,n) + tx(i1,i2,i3)*
     & rsxyst2(i1,i2,i3,m,n)

      rsxyxt23(i1,i2,i3,m,n)= rsxyt2(i1,i2,i3,0,0)*rsxyr2(i1,i2,i3,m,n)
     &  + rx(i1,i2,i3)*rsxyrt2(i1,i2,i3,m,n)+rsxyt2(i1,i2,i3,1,0)*
     & rsxys2(i1,i2,i3,m,n) + sx(i1,i2,i3)*rsxyst2(i1,i2,i3,m,n)+
     & rsxyt2(i1,i2,i3,2,0)*rsxyt2(i1,i2,i3,m,n) + tx(i1,i2,i3)*
     & rsxytt2(i1,i2,i3,m,n)

      rsxyyr23(i1,i2,i3,m,n)= rsxyr2(i1,i2,i3,0,1)*rsxyr2(i1,i2,i3,m,n)
     &  + ry(i1,i2,i3)*rsxyrr2(i1,i2,i3,m,n)+rsxyr2(i1,i2,i3,1,1)*
     & rsxys2(i1,i2,i3,m,n) + sy(i1,i2,i3)*rsxyrs2(i1,i2,i3,m,n)+
     & rsxyr2(i1,i2,i3,2,1)*rsxyt2(i1,i2,i3,m,n) + ty(i1,i2,i3)*
     & rsxyrt2(i1,i2,i3,m,n)

      rsxyys23(i1,i2,i3,m,n)= rsxys2(i1,i2,i3,0,1)*rsxyr2(i1,i2,i3,m,n)
     &  + ry(i1,i2,i3)*rsxyrs2(i1,i2,i3,m,n)+rsxys2(i1,i2,i3,1,1)*
     & rsxys2(i1,i2,i3,m,n) + sy(i1,i2,i3)*rsxyss2(i1,i2,i3,m,n)+
     & rsxys2(i1,i2,i3,2,1)*rsxyt2(i1,i2,i3,m,n) + ty(i1,i2,i3)*
     & rsxyst2(i1,i2,i3,m,n)

      rsxyyt23(i1,i2,i3,m,n)= rsxyt2(i1,i2,i3,0,1)*rsxyr2(i1,i2,i3,m,n)
     &  + ry(i1,i2,i3)*rsxyrt2(i1,i2,i3,m,n)+rsxyt2(i1,i2,i3,1,1)*
     & rsxys2(i1,i2,i3,m,n) + sy(i1,i2,i3)*rsxyst2(i1,i2,i3,m,n)+
     & rsxyt2(i1,i2,i3,2,1)*rsxyt2(i1,i2,i3,m,n) + ty(i1,i2,i3)*
     & rsxytt2(i1,i2,i3,m,n)

      rsxyzr23(i1,i2,i3,m,n)= rsxyr2(i1,i2,i3,0,2)*rsxyr2(i1,i2,i3,m,n)
     &  + rz(i1,i2,i3)*rsxyrr2(i1,i2,i3,m,n)+rsxyr2(i1,i2,i3,1,2)*
     & rsxys2(i1,i2,i3,m,n) + sz(i1,i2,i3)*rsxyrs2(i1,i2,i3,m,n)+
     & rsxyr2(i1,i2,i3,2,2)*rsxyt2(i1,i2,i3,m,n) + tz(i1,i2,i3)*
     & rsxyrt2(i1,i2,i3,m,n)

      rsxyzs23(i1,i2,i3,m,n)= rsxys2(i1,i2,i3,0,2)*rsxyr2(i1,i2,i3,m,n)
     &  + rz(i1,i2,i3)*rsxyrs2(i1,i2,i3,m,n)+rsxys2(i1,i2,i3,1,2)*
     & rsxys2(i1,i2,i3,m,n) + sz(i1,i2,i3)*rsxyss2(i1,i2,i3,m,n)+
     & rsxys2(i1,i2,i3,2,2)*rsxyt2(i1,i2,i3,m,n) + tz(i1,i2,i3)*
     & rsxyst2(i1,i2,i3,m,n)

      rsxyzt23(i1,i2,i3,m,n)= rsxyt2(i1,i2,i3,0,2)*rsxyr2(i1,i2,i3,m,n)
     &  + rz(i1,i2,i3)*rsxyrt2(i1,i2,i3,m,n)+rsxyt2(i1,i2,i3,1,2)*
     & rsxys2(i1,i2,i3,m,n) + sz(i1,i2,i3)*rsxyst2(i1,i2,i3,m,n)+
     & rsxyt2(i1,i2,i3,2,2)*rsxyt2(i1,i2,i3,m,n) + tz(i1,i2,i3)*
     & rsxytt2(i1,i2,i3,m,n)

      ! ---- 4th order ---

      rsxyr4(i1,i2,i3,m,n)=(8.*(rsxy(i1+1,i2,i3,m,n)-rsxy(i1-1,i2,i3,m,
     & n))-(rsxy(i1+2,i2,i3,m,n)-rsxy(i1-2,i2,i3,m,n)))*d14(0)
      rsxys4(i1,i2,i3,m,n)=(8.*(rsxy(i1,i2+1,i3,m,n)-rsxy(i1,i2-1,i3,m,
     & n))-(rsxy(i1,i2+2,i3,m,n)-rsxy(i1,i2-2,i3,m,n)))*d14(1)
      rsxyt4(i1,i2,i3,m,n)=(8.*(rsxy(i1,i2,i3+1,m,n)-rsxy(i1,i2,i3-1,m,
     & n))-(rsxy(i1,i2,i3+2,m,n)-rsxy(i1,i2,i3-2,m,n)))*d14(2)

      rsxyrr4(i1,i2,i3,m,n)=(-30.*rsxy(i1,i2,i3,m,n)+16.*(rsxy(i1+1,i2,
     & i3,m,n)+rsxy(i1-1,i2,i3,m,n))-(rsxy(i1+2,i2,i3,m,n)+rsxy(i1-2,
     & i2,i3,m,n)) )*d24(0)

      rsxyss4(i1,i2,i3,m,n)=(-30.*rsxy(i1,i2,i3,m,n)+16.*(rsxy(i1,i2+1,
     & i3,m,n)+rsxy(i1,i2-1,i3,m,n))-(rsxy(i1,i2+2,i3,m,n)+rsxy(i1,i2-
     & 2,i3,m,n)) )*d24(1)

      rsxytt4(i1,i2,i3,m,n)=(-30.*rsxy(i1,i2,i3,m,n)+16.*(rsxy(i1,i2,
     & i3+1,m,n)+rsxy(i1,i2,i3-1,m,n))-(rsxy(i1,i2,i3+2,m,n)+rsxy(i1,
     & i2,i3-2,m,n)) )*d24(2)

      rsxyrs4(i1,i2,i3,m,n)=(8.*(rsxyr4(i1,i2+1,i3,m,n)-rsxyr4(i1,i2-1,
     & i3,m,n))-(rsxyr4(i1,i2+2,i3,m,n)-rsxyr4(i1,i2-2,i3,m,n)))*d14(
     & 1)

      rsxyrt4(i1,i2,i3,m,n)=(8.*(rsxyr4(i1,i2,i3+1,m,n)-rsxyr4(i1,i2,
     & i3-1,m,n))-(rsxyr4(i1,i2,i3+2,m,n)-rsxyr4(i1,i2,i3-2,m,n)))*
     & d14(2)

      rsxyst4(i1,i2,i3,m,n)=(8.*(rsxys4(i1,i2,i3+1,m,n)-rsxys4(i1,i2,
     & i3-1,m,n))-(rsxys4(i1,i2,i3+2,m,n)-rsxys4(i1,i2,i3-2,m,n)))*
     & d14(2)

      rsxyx42(i1,i2,i3,m,n)= rx(i1,i2,i3)*rsxyr4(i1,i2,i3,m,n)+sx(i1,
     & i2,i3)*rsxys4(i1,i2,i3,m,n)
      rsxyy42(i1,i2,i3,m,n)= ry(i1,i2,i3)*rsxyr4(i1,i2,i3,m,n)+sy(i1,
     & i2,i3)*rsxys4(i1,i2,i3,m,n)

      rsxyxr42(i1,i2,i3,m,n)= rsxyr4(i1,i2,i3,0,0)*rsxyr4(i1,i2,i3,m,n)
     &  + rx(i1,i2,i3)*rsxyrr4(i1,i2,i3,m,n)+rsxyr4(i1,i2,i3,1,0)*
     & rsxys4(i1,i2,i3,m,n) + sx(i1,i2,i3)*rsxyrs4(i1,i2,i3,m,n)
      rsxyxs42(i1,i2,i3,m,n)= rsxys4(i1,i2,i3,0,0)*rsxyr4(i1,i2,i3,m,n)
     &  + rx(i1,i2,i3)*rsxyrs4(i1,i2,i3,m,n)+rsxys4(i1,i2,i3,1,0)*
     & rsxys4(i1,i2,i3,m,n) + sx(i1,i2,i3)*rsxyss4(i1,i2,i3,m,n)

      rsxyyr42(i1,i2,i3,m,n)= rsxyr4(i1,i2,i3,0,1)*rsxyr4(i1,i2,i3,m,n)
     &  + ry(i1,i2,i3)*rsxyrr4(i1,i2,i3,m,n)+rsxyr4(i1,i2,i3,1,1)*
     & rsxys4(i1,i2,i3,m,n) + sy(i1,i2,i3)*rsxyrs4(i1,i2,i3,m,n)
      rsxyys42(i1,i2,i3,m,n)= rsxys4(i1,i2,i3,0,1)*rsxyr4(i1,i2,i3,m,n)
     &  + ry(i1,i2,i3)*rsxyrs4(i1,i2,i3,m,n)+rsxys4(i1,i2,i3,1,1)*
     & rsxys4(i1,i2,i3,m,n) + sy(i1,i2,i3)*rsxyss4(i1,i2,i3,m,n)

      rsxyx43(i1,i2,i3,m,n)= rx(i1,i2,i3)*rsxyr4(i1,i2,i3,m,n)+sx(i1,
     & i2,i3)*rsxys4(i1,i2,i3,m,n)+tx(i1,i2,i3)*rsxyt4(i1,i2,i3,m,n)
      rsxyy43(i1,i2,i3,m,n)= ry(i1,i2,i3)*rsxyr4(i1,i2,i3,m,n)+sy(i1,
     & i2,i3)*rsxys4(i1,i2,i3,m,n)+ty(i1,i2,i3)*rsxyt4(i1,i2,i3,m,n)
      rsxyz43(i1,i2,i3,m,n)= rz(i1,i2,i3)*rsxyr4(i1,i2,i3,m,n)+sz(i1,
     & i2,i3)*rsxys4(i1,i2,i3,m,n)+tz(i1,i2,i3)*rsxyt4(i1,i2,i3,m,n)

      rsxyxr43(i1,i2,i3,m,n)= rsxyr4(i1,i2,i3,0,0)*rsxyr4(i1,i2,i3,m,n)
     &  + rx(i1,i2,i3)*rsxyrr4(i1,i2,i3,m,n)+rsxyr4(i1,i2,i3,1,0)*
     & rsxys4(i1,i2,i3,m,n) + sx(i1,i2,i3)*rsxyrs4(i1,i2,i3,m,n)+
     & rsxyr4(i1,i2,i3,2,0)*rsxyt4(i1,i2,i3,m,n) + tx(i1,i2,i3)*
     & rsxyrt4(i1,i2,i3,m,n)

      rsxyxs43(i1,i2,i3,m,n)= rsxys4(i1,i2,i3,0,0)*rsxyr4(i1,i2,i3,m,n)
     &  + rx(i1,i2,i3)*rsxyrs4(i1,i2,i3,m,n)+rsxys4(i1,i2,i3,1,0)*
     & rsxys4(i1,i2,i3,m,n) + sx(i1,i2,i3)*rsxyss4(i1,i2,i3,m,n)+
     & rsxys4(i1,i2,i3,2,0)*rsxyt4(i1,i2,i3,m,n) + tx(i1,i2,i3)*
     & rsxyst4(i1,i2,i3,m,n)

      rsxyxt43(i1,i2,i3,m,n)= rsxyt4(i1,i2,i3,0,0)*rsxyr4(i1,i2,i3,m,n)
     &  + rx(i1,i2,i3)*rsxyrt4(i1,i2,i3,m,n)+rsxyt4(i1,i2,i3,1,0)*
     & rsxys4(i1,i2,i3,m,n) + sx(i1,i2,i3)*rsxyst4(i1,i2,i3,m,n)+
     & rsxyt4(i1,i2,i3,2,0)*rsxyt4(i1,i2,i3,m,n) + tx(i1,i2,i3)*
     & rsxytt4(i1,i2,i3,m,n)

      rsxyyr43(i1,i2,i3,m,n)= rsxyr4(i1,i2,i3,0,1)*rsxyr4(i1,i2,i3,m,n)
     &  + ry(i1,i2,i3)*rsxyrr4(i1,i2,i3,m,n)+rsxyr4(i1,i2,i3,1,1)*
     & rsxys4(i1,i2,i3,m,n) + sy(i1,i2,i3)*rsxyrs4(i1,i2,i3,m,n)+
     & rsxyr4(i1,i2,i3,2,1)*rsxyt4(i1,i2,i3,m,n) + ty(i1,i2,i3)*
     & rsxyrt4(i1,i2,i3,m,n)

      rsxyys43(i1,i2,i3,m,n)= rsxys4(i1,i2,i3,0,1)*rsxyr4(i1,i2,i3,m,n)
     &  + ry(i1,i2,i3)*rsxyrs4(i1,i2,i3,m,n)+rsxys4(i1,i2,i3,1,1)*
     & rsxys4(i1,i2,i3,m,n) + sy(i1,i2,i3)*rsxyss4(i1,i2,i3,m,n)+
     & rsxys4(i1,i2,i3,2,1)*rsxyt4(i1,i2,i3,m,n) + ty(i1,i2,i3)*
     & rsxyst4(i1,i2,i3,m,n)

      rsxyyt43(i1,i2,i3,m,n)= rsxyt4(i1,i2,i3,0,1)*rsxyr4(i1,i2,i3,m,n)
     &  + ry(i1,i2,i3)*rsxyrt4(i1,i2,i3,m,n)+rsxyt4(i1,i2,i3,1,1)*
     & rsxys4(i1,i2,i3,m,n) + sy(i1,i2,i3)*rsxyst4(i1,i2,i3,m,n)+
     & rsxyt4(i1,i2,i3,2,1)*rsxyt4(i1,i2,i3,m,n) + ty(i1,i2,i3)*
     & rsxytt4(i1,i2,i3,m,n)

      rsxyzr43(i1,i2,i3,m,n)= rsxyr4(i1,i2,i3,0,2)*rsxyr4(i1,i2,i3,m,n)
     &  + rz(i1,i2,i3)*rsxyrr4(i1,i2,i3,m,n)+rsxyr4(i1,i2,i3,1,2)*
     & rsxys4(i1,i2,i3,m,n) + sz(i1,i2,i3)*rsxyrs4(i1,i2,i3,m,n)+
     & rsxyr4(i1,i2,i3,2,2)*rsxyt4(i1,i2,i3,m,n) + tz(i1,i2,i3)*
     & rsxyrt4(i1,i2,i3,m,n)

      rsxyzs43(i1,i2,i3,m,n)= rsxys4(i1,i2,i3,0,2)*rsxyr4(i1,i2,i3,m,n)
     &  + rz(i1,i2,i3)*rsxyrs4(i1,i2,i3,m,n)+rsxys4(i1,i2,i3,1,2)*
     & rsxys4(i1,i2,i3,m,n) + sz(i1,i2,i3)*rsxyss4(i1,i2,i3,m,n)+
     & rsxys4(i1,i2,i3,2,2)*rsxyt4(i1,i2,i3,m,n) + tz(i1,i2,i3)*
     & rsxyst4(i1,i2,i3,m,n)

      rsxyzt43(i1,i2,i3,m,n)= rsxyt4(i1,i2,i3,0,2)*rsxyr4(i1,i2,i3,m,n)
     &  + rz(i1,i2,i3)*rsxyrt4(i1,i2,i3,m,n)+rsxyt4(i1,i2,i3,1,2)*
     & rsxys4(i1,i2,i3,m,n) + sz(i1,i2,i3)*rsxyst4(i1,i2,i3,m,n)+
     & rsxyt4(i1,i2,i3,2,2)*rsxyt4(i1,i2,i3,m,n) + tz(i1,i2,i3)*
     & rsxytt4(i1,i2,i3,m,n)

       ! uxxx22r(i1,i2,i3,kd)=(-2.*(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))+(u(i1+2,i2,i3,kd)-u(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
       ! uyyy22r(i1,i2,i3,kd)=(-2.*(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))+(u(i1,i2+2,i3,kd)-u(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
       ! uxxxx22r(i1,i2,i3,kd)=(6.*u(i1,i2,i3,kd)-4.*(u(i1+1,i2,i3,kd)+u(i1-1,i2,i3,kd))!                         +(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd)) )/(dx(0)**4)
       ! uyyyy22r(i1,i2,i3,kd)=(6.*u(i1,i2,i3,kd)-4.*(u(i1,i2+1,i3,kd)+u(i1,i2-1,i3,kd))!                         +(u(i1,i2+2,i3,kd)+u(i1,i2-2,i3,kd)) )/(dx(1)**4)
        urrrr2(i1,i2,i3,kd)=(6.*u(i1,i2,i3,kd)-4.*(u(i1+1,i2,i3,kd)+u(
     & i1-1,i2,i3,kd))+(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd)) )/(dr(0)**
     & 4)
        ussss2(i1,i2,i3,kd)=(6.*u(i1,i2,i3,kd)-4.*(u(i1,i2+1,i3,kd)+u(
     & i1,i2-1,i3,kd))+(u(i1,i2+2,i3,kd)+u(i1,i2-2,i3,kd)) )/(dr(1)**
     & 4)
       ! add these to the derivatives include file
        urrs2(i1,i2,i3,kd)=(urr2(i1,i2+1,i3,kd)-urr2(i1,i2-1,i3,kd))/(
     & 2.*dr(1))
        urrt2(i1,i2,i3,kd)=(urr2(i1,i2,i3+1,kd)-urr2(i1,i2,i3-1,kd))/(
     & 2.*dr(2))
        urss2(i1,i2,i3,kd)=(uss2(i1+1,i2,i3,kd)-uss2(i1-1,i2,i3,kd))/(
     & 2.*dr(0))
        usst2(i1,i2,i3,kd)=(uss2(i1,i2,i3+1,kd)-uss2(i1,i2,i3-1,kd))/(
     & 2.*dr(2))
        urtt2(i1,i2,i3,kd)=(utt2(i1+1,i2,i3,kd)-utt2(i1-1,i2,i3,kd))/(
     & 2.*dr(0))
        ustt2(i1,i2,i3,kd)=(utt2(i1,i2+1,i3,kd)-utt2(i1,i2-1,i3,kd))/(
     & 2.*dr(1))
       ! these are from diff.maple
        urrs4(i1,i2,i3,kd) = (u(i1-2,i2+2,i3,kd)+16*u(i1+1,i2-2,i3,kd)-
     & 30*u(i1,i2-2,i3,kd)+16*u(i1-1,i2-2,i3,kd)-u(i1+2,i2-2,i3,kd)-u(
     & i1-2,i2-2,i3,kd)-16*u(i1+1,i2+2,i3,kd)+30*u(i1,i2+2,i3,kd)-16*
     & u(i1-1,i2+2,i3,kd)+u(i1+2,i2+2,i3,kd)-240*u(i1,i2+1,i3,kd)-8*u(
     & i1+2,i2+1,i3,kd)-8*u(i1-2,i2+1,i3,kd)-128*u(i1+1,i2-1,i3,kd)+
     & 240*u(i1,i2-1,i3,kd)-128*u(i1-1,i2-1,i3,kd)+8*u(i1+2,i2-1,i3,
     & kd)+8*u(i1-2,i2-1,i3,kd)+128*u(i1-1,i2+1,i3,kd)+128*u(i1+1,i2+
     & 1,i3,kd))/(144.*dr(0)**2*dr(1))
        urrt4(i1,i2,i3,kd) = (30*u(i1,i2,i3+2,kd)-16*u(i1-1,i2,i3+2,kd)
     & +u(i1+2,i2,i3+2,kd)-16*u(i1+1,i2,i3+2,kd)-30*u(i1,i2,i3-2,kd)+
     & 16*u(i1+1,i2,i3-2,kd)+u(i1-2,i2,i3+2,kd)-u(i1+2,i2,i3-2,kd)-u(
     & i1-2,i2,i3-2,kd)+16*u(i1-1,i2,i3-2,kd)+128*u(i1+1,i2,i3+1,kd)-
     & 240*u(i1,i2,i3+1,kd)+128*u(i1-1,i2,i3+1,kd)-8*u(i1+2,i2,i3+1,
     & kd)-8*u(i1-2,i2,i3+1,kd)-128*u(i1+1,i2,i3-1,kd)+240*u(i1,i2,i3-
     & 1,kd)-128*u(i1-1,i2,i3-1,kd)+8*u(i1+2,i2,i3-1,kd)+8*u(i1-2,i2,
     & i3-1,kd))/(144.*dr(0)**2*dr(2))
        usst4(i1,i2,i3,kd) = (30*u(i1,i2,i3+2,kd)-30*u(i1,i2,i3-2,kd)+
     & 128*u(i1,i2+1,i3+1,kd)+128*u(i1,i2-1,i3+1,kd)-8*u(i1,i2+2,i3+1,
     & kd)-8*u(i1,i2-2,i3+1,kd)-128*u(i1,i2+1,i3-1,kd)-128*u(i1,i2-1,
     & i3-1,kd)+8*u(i1,i2+2,i3-1,kd)+8*u(i1,i2-2,i3-1,kd)-240*u(i1,i2,
     & i3+1,kd)+240*u(i1,i2,i3-1,kd)+16*u(i1,i2+1,i3-2,kd)-16*u(i1,i2+
     & 1,i3+2,kd)-16*u(i1,i2-1,i3+2,kd)+u(i1,i2+2,i3+2,kd)+u(i1,i2-2,
     & i3+2,kd)+16*u(i1,i2-1,i3-2,kd)-u(i1,i2+2,i3-2,kd)-u(i1,i2-2,i3-
     & 2,kd))/(144.*dr(1)**2*dr(2))
        urss4(i1,i2,i3,kd) = (-240*u(i1+1,i2,i3,kd)+240*u(i1-1,i2,i3,
     & kd)-u(i1-2,i2+2,i3,kd)-8*u(i1+1,i2-2,i3,kd)+8*u(i1-1,i2-2,i3,
     & kd)+u(i1+2,i2-2,i3,kd)-u(i1-2,i2-2,i3,kd)-8*u(i1+1,i2+2,i3,kd)+
     & 8*u(i1-1,i2+2,i3,kd)+u(i1+2,i2+2,i3,kd)-16*u(i1+2,i2+1,i3,kd)+
     & 16*u(i1-2,i2+1,i3,kd)+128*u(i1+1,i2-1,i3,kd)-128*u(i1-1,i2-1,
     & i3,kd)-16*u(i1+2,i2-1,i3,kd)+16*u(i1-2,i2-1,i3,kd)-128*u(i1-1,
     & i2+1,i3,kd)+128*u(i1+1,i2+1,i3,kd)-30*u(i1-2,i2,i3,kd)+30*u(i1+
     & 2,i2,i3,kd))/(144.*dr(1)**2*dr(0))
        ustt4(i1,i2,i3,kd) = (-30*u(i1,i2-2,i3,kd)+30*u(i1,i2+2,i3,kd)-
     & 240*u(i1,i2+1,i3,kd)+240*u(i1,i2-1,i3,kd)+128*u(i1,i2+1,i3+1,
     & kd)-128*u(i1,i2-1,i3+1,kd)-16*u(i1,i2+2,i3+1,kd)+16*u(i1,i2-2,
     & i3+1,kd)+128*u(i1,i2+1,i3-1,kd)-128*u(i1,i2-1,i3-1,kd)-16*u(i1,
     & i2+2,i3-1,kd)+16*u(i1,i2-2,i3-1,kd)-8*u(i1,i2+1,i3-2,kd)-8*u(
     & i1,i2+1,i3+2,kd)+8*u(i1,i2-1,i3+2,kd)+u(i1,i2+2,i3+2,kd)-u(i1,
     & i2-2,i3+2,kd)+8*u(i1,i2-1,i3-2,kd)+u(i1,i2+2,i3-2,kd)-u(i1,i2-
     & 2,i3-2,kd))/(144.*dr(2)**2*dr(1))
        urtt4(i1,i2,i3,kd) = (-240*u(i1+1,i2,i3,kd)+240*u(i1-1,i2,i3,
     & kd)+8*u(i1-1,i2,i3+2,kd)+u(i1+2,i2,i3+2,kd)-8*u(i1+1,i2,i3+2,
     & kd)-8*u(i1+1,i2,i3-2,kd)-u(i1-2,i2,i3+2,kd)+u(i1+2,i2,i3-2,kd)-
     & u(i1-2,i2,i3-2,kd)+8*u(i1-1,i2,i3-2,kd)+128*u(i1+1,i2,i3+1,kd)-
     & 128*u(i1-1,i2,i3+1,kd)-16*u(i1+2,i2,i3+1,kd)+16*u(i1-2,i2,i3+1,
     & kd)+128*u(i1+1,i2,i3-1,kd)-128*u(i1-1,i2,i3-1,kd)-16*u(i1+2,i2,
     & i3-1,kd)+16*u(i1-2,i2,i3-1,kd)-30*u(i1-2,i2,i3,kd)+30*u(i1+2,
     & i2,i3,kd))/(144.*dr(2)**2*dr(0))
       !     --- end statement functions ----
        ierr=0
        side                 =ipar(0)
        axis                 =ipar(1)
        n1a                  =ipar(2)
        n1b                  =ipar(3)
        n2a                  =ipar(4)
        n2b                  =ipar(5)
        n3a                  =ipar(6)
        n3b                  =ipar(7)
        gridType             =ipar(8)
        orderOfAccuracy      =ipar(9)
        orderOfExtrapolation =ipar(10)
        useForcing           =ipar(11)
        ex                   =ipar(12)
        ey                   =ipar(13)
        ez                   =ipar(14)
        hx                   =ipar(15)
        hy                   =ipar(16)
        hz                   =ipar(17)
        useWhereMask         =ipar(18)
        grid                 =ipar(19)
        debug                =ipar(20)
        forcingOption        =ipar(21)
        useChargeDensity     =ipar(24)
        fieldOption          =ipar(29)  ! 0=assign field, 1=assign time derivatives
        dx(0)                =rpar(0)
        dx(1)                =rpar(1)
        dx(2)                =rpar(2)
        dr(0)                =rpar(3)
        dr(1)                =rpar(4)
        dr(2)                =rpar(5)
        t                    =rpar(6)
        ep                   =rpar(7)
        dt                   =rpar(8)
        c                    =rpar(9)
        eps                  =rpar(10)
        mu                   =rpar(11)
        kx                   =rpar(12)  ! for plane wave forcing
        ky                   =rpar(13)
        kz                   =rpar(14)
        slowStartInterval    =rpar(15)
        ! pmlLayerStrength   =rpar(16)
        pu                   =rpar(17)   ! for to P++ array
        pwc(0)               =rpar(20) ! coeffs. for plane wave
        pwc(1)               =rpar(21)
        pwc(2)               =rpar(22)
        pwc(3)               =rpar(23)
        pwc(4)               =rpar(24)
        pwc(5)               =rpar(25)
        if( abs(pwc(0))+abs(pwc(1))+abs(pwc(2)) .eq. 0. )then
          ! sanity check
          stop 12345
        end if
        dxa=dx(0)
        dya=dx(1)
        dza=dx(2)
          ! In parallel the dimension may not be the same as the bounds nd1a,nd1b,...
        md1a=dimension(0,0)
        md1b=dimension(1,0)
        md2a=dimension(0,1)
        md2b=dimension(1,1)
        md3a=dimension(0,2)
        md3b=dimension(1,2)
        twoPi=8.*atan2(1.,1.)
        cc= c*sqrt( kx*kx+ky*ky+kz*kz )
c write(*,'("initializeBoundaryForcing slowStartInterval=",e10.2)') slowStartInterval
        if( t.le.0 .and. slowStartInterval.gt.0. )then
          ssf = 0.
          ssft = 0.
          ssftt = 0.
          ssfttt = 0.
          ssftttt = 0.
        else if( t.lt.slowStartInterval )then
          tt=t/slowStartInterval
          ssf = (126*(tt)**5-315*(tt)**8+70*(tt)**9-420*(tt)**6+540*(
     & tt)**7)
          ssft = (630*(tt)**4-2520*(tt)**7+630*(tt)**8-2520*(tt)**5+
     & 3780*(tt)**6)
          ssftt = (2520*(tt)**3-17640*(tt)**6+5040*(tt)**7-12600*(tt)**
     & 4+22680*(tt)**5)
          ssfttt = (7560*(tt)**2-105840*(tt)**5+35280*(tt)**6-50400*(
     & tt)**3+113400*(tt)**4)
          ssftttt = (15120*(tt)-529200*(tt)**4+211680*(tt)**5-151200*(
     & tt)**2+453600*(tt)**3)
        ! Here we turn off the plane wave after some time:
        ! else if( t.gt.1.0 )then
        !  ssf = 0.
        !  ssft = 0. 
        !  ssftt = 0. 
        !  ssfttt = 0. 
        !  ssftttt = 0. 
         else
          ssf = 1.
          ssft = 0.
          ssftt = 0.
          ssfttt = 0.
          ssftttt = 0.
        end if
        ! ****
        ! write(*,'(" bcOpt: t=",e10.2," fieldOption=",i2," ex,ey,hz=",3i3)') t,fieldOption,ex,ey,hz
        !  write(*,'(" ***bcOpt: slowStartInterval,t=",2f10.4," ssf,ssft,ssftt,sfttt=",4f9.4)') slowStartInterval,t,ssf,ssft,ssftt,ssfttt
        !  --- NOT: extra determines "extra points" in the tangential directions  ----
        !  extra=-1 by default (if adjacent BC>0) no need to do corners -- these are already done
        !  extra=numberOfGhostPoints, if bc==0, (set in begin loop over sides)
        !  extra=0 if bc<0  (set in begin loop over sides)
        extra=-1
        numberOfGhostPoints=orderOfAccuracy/2
        if( gridType.eq.curvilinear )then
         ! the 4th-order 3d BCs require two steps -- the first step gives initial values at all ghost points
        end if
        ! ok if( .true. ) return ! **********************************************************
        ! ==================================================================================
         extra1a=extra
         extra1b=extra
         extra2a=extra
         extra2b=extra
         if( nd.eq.3 )then
           extra3a=extra
           extra3b=extra
         else
           extra3a=0
           extra3b=0
         end if
         if( boundaryCondition(0,0).lt.0 )then
           extra1a=max(0,extra1a) ! over-ride extra=-1 : assign ends in periodic directions (or internal parallel boundaries)
         else if( boundaryCondition(0,0).eq.0 )then
           extra1a=numberOfGhostPoints  ! include interpolation points since we assign ghost points outside these
         end if
         ! **NOTE** the bc on the right may be negative even it is not on the left (for parallel)
         if( boundaryCondition(1,0).lt.0 )then
           extra1b=max(0,extra1b) ! over-ride extra=-1 : assign ends in periodic directions
         else if( boundaryCondition(1,0).eq.0 )then
           extra1b=numberOfGhostPoints
         end if
         if( boundaryCondition(0,1).lt.0 )then
           extra2a=max(0,extra2a) ! over-ride extra=-1 : assign ends in periodic directions (or internal parallel boundaries)
         else if( boundaryCondition(0,1).eq.0 )then
           extra2a=numberOfGhostPoints  ! include interpolation points since we assign ghost points outside these
         end if
         ! **NOTE** the bc on the right may be negative even it is not on the left (for parallel)
         if( boundaryCondition(1,1).lt.0 )then
           extra2b=max(0,extra2b) ! over-ride extra=-1 : assign ends in periodic directions
         else if( boundaryCondition(1,1).eq.0 )then
           extra2b=numberOfGhostPoints
         end if
         if(  nd.eq.3 )then
          if( boundaryCondition(0,2).lt.0 )then
            extra3a=max(0,extra3a) ! over-ride extra=-1 : assign ends in periodic directions (or internal parallel boundaries)
          else if( boundaryCondition(0,2).eq.0 )then
            extra3a=numberOfGhostPoints  ! include interpolation points since we assign ghost points outside these
          end if
          ! **NOTE** the bc on the right may be negative even it is not on the left (for parallel)
          if( boundaryCondition(1,2).lt.0 )then
            extra3b=max(0,extra3b) ! over-ride extra=-1 : assign ends in periodic directions
          else if( boundaryCondition(1,2).eq.0 )then
            extra3b=numberOfGhostPoints
          end if
         end if
         do axis=0,nd-1
         do side=0,1
           if( boundaryCondition(side,axis)
     & .eq.perfectElectricalConductor )then
             ! write(*,'(" bcOpt: side,axis,bc=",3i2)') side,axis,boundaryCondition(side,axis)
             n1a=gridIndexRange(0,0)-extra1a
             n1b=gridIndexRange(1,0)+extra1b
             n2a=gridIndexRange(0,1)-extra2a
             n2b=gridIndexRange(1,1)+extra2b
             n3a=gridIndexRange(0,2)-extra3a
             n3b=gridIndexRange(1,2)+extra3b
             if( axis.eq.0 )then
               n1a=gridIndexRange(side,axis)
               n1b=gridIndexRange(side,axis)
             else if( axis.eq.1 )then
               n2a=gridIndexRange(side,axis)
               n2b=gridIndexRange(side,axis)
             else
               n3a=gridIndexRange(side,axis)
               n3b=gridIndexRange(side,axis)
             end if
             is1=0
             is2=0
             is3=0
             if( axis.eq.0 )then
               is1=1-2*side
             else if( axis.eq.1 )then
               is2=1-2*side
             else if( axis.eq.2 )then
               is3=1-2*side
             else
               stop 5
             end if
             axisp1=mod(axis+1,nd)
             axisp2=mod(axis+2,nd)
             ! (js1,js2,js3) used to compute tangential derivatives
             js1=0
             js2=0
             js3=0
             if( axisp1.eq.0 )then
               js1=1-2*side
             else if( axisp1.eq.1 )then
               js2=1-2*side
             else if( axisp1.eq.2 )then
               js3=1-2*side
             else
               stop 5
             end if
             ! (ks1,ks2,ks3) used to compute second tangential derivative
             ks1=0
             ks2=0
             ks3=0
             if( axisp2.eq.0 )then
               ks1=1-2*side
             else if( axisp2.eq.1 )then
               ks2=1-2*side
             else if( axisp2.eq.2 )then
               ks3=1-2*side
             else
               stop 5
             end if
         if( debug.gt.7 )then
           write(*,'(" bcOpt: grid,side,axis=",3i3,", loop bounds: n1a,
     & n1b,n2a,n2b,n3a,n3b=",6i3)') grid,side,axis,n1a,n1b,n2a,n2b,
     & n3a,n3b
         end if
         if( gridType.eq.rectangular )then
           ! ***********************************************
           ! ************rectangular grid*******************
           ! ***********************************************
           ! odd symmetry for the normal components
           ! even symmetry for tangential components
           ! en1=normal component of E
           ! et1=tangential component 1 of E
           ! et2=tangential component 2 of E
           ! hn1=normal component of H
           ! ht1=tangential component 1 of H
           ! ht2=tangential component 2 of H
           ! write(*,'(" bcOpt: called for rectangular side,axis=",2i2)') side,axis
           if( axis.eq.0 )then
             en1=ex
             et1=ey
             et2=ez
             hn1=hx
             ht1=hy
             ht2=hz
           else if( axis.eq.1 )then
             et1=ex
             en1=ey
             et2=ez
             ht1=hx
             hn1=hy
             ht2=hz
           else
             et1=ex
             et2=ey
             en1=ez
             ht1=hx
             ht2=hy
             hn1=hz
           end if
           if( useForcing.eq.0 )then
              if( debug.gt.1 )then
                write(*,'(" bc4r: **START** grid=",i4," side,axis=",
     & 2i2)') grid,side,axis
              end if
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
               if( mask(i1,i2,i3).ne.0 )then
                ! ** u(i1,i2,i3,et1)=0.
                u(i1-is1,i2-is2,i3-is3,en1)= u(i1+is1,i2+is2,i3+is3,
     & en1)
                u(i1-is1,i2-is2,i3-is3,et1)=2.*u(i1,i2,i3,et1)-u(i1+
     & is1,i2+is2,i3+is3,et1)
                  u(i1-is1,i2-is2,i3-is3,hz)=u(i1+is1,i2+is2,i3+is3,hz)
                if( useChargeDensity.eq.1 )then
                 ! div(eps*E) = rho , rho is saved in f(i1,i2,i3,0)
                 u(i1-is1,i2-is2,i3-is3,en1)=u(i1-is1,i2-is2,i3-is3,
     & en1) - 2.*dx(axis)*(1-2*side)*f(i1,i2,i3,0)/eps
                end if
               end if ! mask
              end do
              end do
              end do
           else
              if( debug.gt.1 )then
                write(*,'(" bc4r: **START** grid=",i4," side,axis=",
     & 2i2)') grid,side,axis
              end if
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
               if( mask(i1,i2,i3).ne.0 )then
                ! ** u(i1,i2,i3,et1)=0.
                u(i1-is1,i2-is2,i3-is3,en1)= u(i1+is1,i2+is2,i3+is3,
     & en1)
                u(i1-is1,i2-is2,i3-is3,et1)=2.*u(i1,i2,i3,et1)-u(i1+
     & is1,i2+is2,i3+is3,et1)
                  u(i1-is1,i2-is2,i3-is3,hz)=u(i1+is1,i2+is2,i3+is3,hz)
                if( useChargeDensity.eq.1 )then
                 ! div(eps*E) = rho , rho is saved in f(i1,i2,i3,0)
                 u(i1-is1,i2-is2,i3-is3,en1)=u(i1-is1,i2-is2,i3-is3,
     & en1) - 2.*dx(axis)*(1-2*side)*f(i1,i2,i3,0)/eps
                end if
                     call ogf2dfo(ep,fieldOption,xy(i1-is1,i2-is2,i3,0)
     & ,xy(i1-is1,i2-is2,i3,1),t,uvm(ex),uvm(ey),uvm(hz))
                     call ogf2dfo(ep,fieldOption,xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),t,uv0(ex),uv0(ey),uv0(hz))
                     call ogf2dfo(ep,fieldOption,xy(i1+is1,i2+is2,i3,0)
     & ,xy(i1+is1,i2+is2,i3,1),t,uvp(ex),uvp(ey),uvp(hz))
             ! write(*,'("..bcRectangular: side,axis=",2i3," i1,i2,i3=",3i3," en1,uvm(en1),uvp(en1)=",3e12.4)')!            side,axis,i1,i2,i3,u(i1-is1,i2-is2,i3,en1),uvm(en1),uvp(en1)
                    u(i1-is1,i2-is2,i3,en1)=u(i1-is1,i2-is2,i3,en1) + 
     & uvm(en1) - uvp(en1)
                    u(i1-is1,i2-is2,i3,et1)=u(i1-is1,i2-is2,i3,et1) + 
     & uvm(et1) -2.*uv0(et1) + uvp(et1)
                    u(i1-is1,i2-is2,i3,hz )=u(i1-is1,i2-is2,i3,hz ) + 
     & uvm(hz)-uvp(hz)
               end if ! mask
              end do
              end do
              end do
           end if
         else
           ! ***********************************************
           ! ************curvilinear grid*******************
           ! ***********************************************
           ! write(*,'(" bcOpt: called for curvilinear, order=",i2," side,axis=",2i2)') orderOfAccuracy,side,axis
               if( useForcing.eq.0 )then
                  dra = dr(axis)*(1-2*side)
                  dsa = dr(axisp1)*(1-2*side)
                  do i3=n3a,n3b
                  do i2=n2a,n2b
                  do i1=n1a,n1b
                   if( mask(i1,i2,i3).gt.0 )then ! check for mask added 2015/06/01 *wdh*
                    ! ---- Boundary point is a physical point ---
                    jac=1./(rx(i1-is1,i2-is2,i3-is3)*sy(i1-is1,i2-is2,
     & i3-is3)-ry(i1-is1,i2-is2,i3-is3)*sx(i1-is1,i2-is2,i3-is3))
                    a11 =rsxy(i1-is1,i2-is2,i3-is3,axis  ,0)*jac
                    a12 =rsxy(i1-is1,i2-is2,i3-is3,axis  ,1)*jac
                    tau1=rsxy(i1-is1,i2-is2,i3-is3,axisp1,0)
                    tau2=rsxy(i1-is1,i2-is2,i3-is3,axisp1,1)
                    det = -tau2*a11+a12*tau1
                    jacp1=1./(rx(i1+is1,i2+is2,i3+is3)*sy(i1+is1,i2+
     & is2,i3+is3)-ry(i1+is1,i2+is2,i3+is3)*sx(i1+is1,i2+is2,i3+is3))
                    a11p1=jacp1*rsxy(i1+is1,i2+is2,i3+is3,axis,0)
                    a12p1=jacp1*rsxy(i1+is1,i2+is2,i3+is3,axis,1)
                    ! ** tau1DotU=tau1*u(i1,i2,i3,ex)+tau2*u(i1,i2,i3,ey)
                    ! ** u(i1,i2,i3,ex)=u(i1,i2,i3,ex)-tau1DotU*tau1
                    ! ** u(i1,i2,i3,ey)=u(i1,i2,i3,ey)-tau1DotU*tau2
                    ! gx2=tau1*(3.*u(i1,i2,i3,ex)-3.*u(i1+is1,i2+is2,i3+is3,ex)+u(i1+2*is1,i2+2*is2,i3,ex))!    +tau2*(3.*u(i1,i2,i3,ey)-3.*u(i1+is1,i2+is2,i3+is3,ey)+u(i1+2*is1,i2+2*is2,i3,ey))
                    ! use even derivatives of the tangential component:
                    ! gx2=tau1*(2.*u(i1,i2,i3,ex)-u(i1+is1,i2+is2,i3+is3,ex))!    +tau2*(2.*u(i1,i2,i3,ey)-u(i1+is1,i2+is2,i3+is3,ey))
                    gx2=tau1*(4.*u(i1,i2,i3,ex)-6.*u(i1+is1,i2+is2,i3,
     & ex)+4.*u(i1+2*is1,i2+2*is2,i3,ex)-u(i1+3*is1,i2+3*is2,i3,ex))+
     & tau2*(4.*u(i1,i2,i3,ey)-6.*u(i1+is1,i2+is2,i3,ey)+4.*u(i1+2*
     & is1,i2+2*is2,i3,ey)-u(i1+3*is1,i2+3*is2,i3,ey))
                    g2a=0.
                    ! include a2 terms in case tangential components are non-zero:
                    a21zp1=(rsxy(i1+js1,i2+js2,i3,axisp1,0)/(rx(i1+js1,
     & i2+js2,i3)*sy(i1+js1,i2+js2,i3)-ry(i1+js1,i2+js2,i3)*sx(i1+js1,
     & i2+js2,i3)))
                    a22zp1=(rsxy(i1+js1,i2+js2,i3,axisp1,1)/(rx(i1+js1,
     & i2+js2,i3)*sy(i1+js1,i2+js2,i3)-ry(i1+js1,i2+js2,i3)*sx(i1+js1,
     & i2+js2,i3)))
                    a21zm1=(rsxy(i1-js1,i2-js2,i3,axisp1,0)/(rx(i1-js1,
     & i2-js2,i3)*sy(i1-js1,i2-js2,i3)-ry(i1-js1,i2-js2,i3)*sx(i1-js1,
     & i2-js2,i3)))
                    a22zm1=(rsxy(i1-js1,i2-js2,i3,axisp1,1)/(rx(i1-js1,
     & i2-js2,i3)*sy(i1-js1,i2-js2,i3)-ry(i1-js1,i2-js2,i3)*sx(i1-js1,
     & i2-js2,i3)))
                    g1a=    a11p1*u(i1+is1,i2+is2,i3,ex)+ a12p1*u(i1+
     & is1,i2+is2,i3,ey)+ ( (a21zp1*u(i1+js1,i2+js2,i3,ex)+a22zp1*u(
     & i1+js1,i2+js2,i3,ey))-(a21zm1*u(i1-js1,i2-js2,i3,ex)+a22zm1*u(
     & i1-js1,i2-js2,i3,ey)) )*dra/dsa
                    if( forcingOption.eq.planeWaveBoundaryForcing )then
                      ! *** for planeWaveBoundaryForcing we need to use: u.t=w.y and v.t=-w.x =>
                      ! *****  (n1,n2).(w.x,w.y) = -n1*v.t + n2*u.t
                      !  OR    (rx,ry).(w.x,w.y) = -rx*v.t + ry*u.t
                      !   (rx**2+ry**2) w.r + (rx*sx+ry*sy)*ws = -rx*vt + ry*ut 
                      x0=xy(i1,i2,i3,0)
                      y0=xy(i1,i2,i3,1)
                      ! Note minus sign since we are subtracting out the incident field
                      if( fieldOption.eq.0 )then
                        u0t=-(ssf*(-twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(
     & y0)-cc*(t)))*pwc(0)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*
     & pwc(0))
                        v0t=-(ssf*(-twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(
     & y0)-cc*(t)))*pwc(1)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*
     & pwc(1))
                      else
                        ! we are assigning time derivatives (sosup)
                        u0t=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x0)+
     & ky*(y0)-cc*(t)))*pwc(0))+2.*ssft*(-twoPi*cc)*cos(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(0)+ssftt*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(
     & t)))*pwc(0))
                        v0t=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x0)+
     & ky*(y0)-cc*(t)))*pwc(1))+2.*ssft*(-twoPi*cc)*cos(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(1)+ssftt*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(
     & t)))*pwc(1))
                      endif
                      g2a=(2.*dra)*( (rsxy(i1,i2,i3,axis,0)*rsxy(i1,i2,
     & i3,axisp1,0)+rsxy(i1,i2,i3,axis,1)*rsxy(i1,i2,i3,axisp1,1))* (
     & u(i1+js1,i2+js2,i3,hz)-u(i1-js1,i2-js2,i3,hz))/(2.*dsa) + rsxy(
     & i1,i2,i3,axis,0)*v0t - rsxy(i1,i2,i3,axis,1)*u0t )/(rsxy(i1,i2,
     & i3,axis,0)**2+rsxy(i1,i2,i3,axis,1)**2)
                   ! write(*,'(" bc2: i=",3i3," side,axis,is1,is2,js1,js2=",6i3," dra,dsa=",2e10.2)') i1,i2,i3,side,axis,is1,is2,js1,js2,dra,dsa
                    else
                      ! this is new: include RHS 040116  -- note: this will be zero if the grid is orthogonal
                      g2a=(2.*dra)*( (rsxy(i1,i2,i3,axis,0)*rsxy(i1,i2,
     & i3,axisp1,0)+rsxy(i1,i2,i3,axis,1)*rsxy(i1,i2,i3,axisp1,1))* (
     & u(i1+js1,i2+js2,i3,hz)-u(i1-js1,i2-js2,i3,hz))/(2.*dsa) )/(
     & rsxy(i1,i2,i3,axis,0)**2+rsxy(i1,i2,i3,axis,1)**2)
                    end if
                    if( useChargeDensity.eq.1 )then
                     ! div(eps*E) = rho , rho is saved in f(i1,i2,i3,0)
                     jac0=1./(rx(i1,i2,i3)*sy(i1,i2,i3)-ry(i1,i2,i3)*
     & sx(i1,i2,i3))
                     g1a = g1a - 2.*dra*f(i1,i2,i3,0)*jac0/eps
                    end if
                    u(i1-is1,i2-is2,i3-is3,ex) = (-tau2*g1a +a12*gx2)
     & /det
                    u(i1-is1,i2-is2,i3-is3,ey) = ( tau1*g1a -a11*gx2)
     & /det
                    u(i1-is1,i2-is2,i3-is3,hz)=u(i1+is1,i2+is2,i3+is3,
     & hz) + g2a
                   else if( mask(i1,i2,i3).lt.0 )then
                    ! ---- Boundary point is an interpolation point ---
                    ! extrapolate ghost points:
                     u(i1-is1,i2-is2,i3-is3,ex)=(3.*u(i1,i2,i3,ex)-3.*
     & u(i1+is1,i2+is2,i3+is3,ex)+u(i1+2*is1,i2+2*is2,i3+2*is3,ex))
                     u(i1-is1,i2-is2,i3-is3,ey)=(3.*u(i1,i2,i3,ey)-3.*
     & u(i1+is1,i2+is2,i3+is3,ey)+u(i1+2*is1,i2+2*is2,i3+2*is3,ey))
                     u(i1-is1,i2-is2,i3-is3,hz)=(3.*u(i1,i2,i3,hz)-3.*
     & u(i1+is1,i2+is2,i3+is3,hz)+u(i1+2*is1,i2+2*is2,i3+2*is3,hz))
                   else
                     ! boundary point is unused -- extrap for now ??
                     u(i1-is1,i2-is2,i3-is3,ex)=(3.*u(i1,i2,i3,ex)-3.*
     & u(i1+is1,i2+is2,i3+is3,ex)+u(i1+2*is1,i2+2*is2,i3+2*is3,ex))
                     u(i1-is1,i2-is2,i3-is3,ey)=(3.*u(i1,i2,i3,ey)-3.*
     & u(i1+is1,i2+is2,i3+is3,ey)+u(i1+2*is1,i2+2*is2,i3+2*is3,ey))
                     u(i1-is1,i2-is2,i3-is3,hz)=(3.*u(i1,i2,i3,hz)-3.*
     & u(i1+is1,i2+is2,i3+is3,hz)+u(i1+2*is1,i2+2*is2,i3+2*is3,hz))
                   end if
                  end do
                  end do
                  end do
                  if( .false. )then
                    do i3=n3a,n3b
                    do i2=n2a,n2b
                    do i1=n1a,n1b
                     a11p1= (rsxy(i1+is1,i2+is2,i3,axis,0)/(rx(i1+is1,
     & i2+is2,i3)*sy(i1+is1,i2+is2,i3)-ry(i1+is1,i2+is2,i3)*sx(i1+is1,
     & i2+is2,i3)))
                     a11m1= (rsxy(i1-is1,i2-is2,i3,axis,0)/(rx(i1-is1,
     & i2-is2,i3)*sy(i1-is1,i2-is2,i3)-ry(i1-is1,i2-is2,i3)*sx(i1-is1,
     & i2-is2,i3)))
                     a12p1= (rsxy(i1+is1,i2+is2,i3,axis,1)/(rx(i1+is1,
     & i2+is2,i3)*sy(i1+is1,i2+is2,i3)-ry(i1+is1,i2+is2,i3)*sx(i1+is1,
     & i2+is2,i3)))
                     a12m1= (rsxy(i1-is1,i2-is2,i3,axis,1)/(rx(i1-is1,
     & i2-is2,i3)*sy(i1-is1,i2-is2,i3)-ry(i1-is1,i2-is2,i3)*sx(i1-is1,
     & i2-is2,i3)))
                      a21zp1=(rsxy(i1+js1,i2+js2,i3,axisp1,0)/(rx(i1+
     & js1,i2+js2,i3)*sy(i1+js1,i2+js2,i3)-ry(i1+js1,i2+js2,i3)*sx(i1+
     & js1,i2+js2,i3)))
                      a22zp1=(rsxy(i1+js1,i2+js2,i3,axisp1,1)/(rx(i1+
     & js1,i2+js2,i3)*sy(i1+js1,i2+js2,i3)-ry(i1+js1,i2+js2,i3)*sx(i1+
     & js1,i2+js2,i3)))
                      a21zm1=(rsxy(i1-js1,i2-js2,i3,axisp1,0)/(rx(i1-
     & js1,i2-js2,i3)*sy(i1-js1,i2-js2,i3)-ry(i1-js1,i2-js2,i3)*sx(i1-
     & js1,i2-js2,i3)))
                      a22zm1=(rsxy(i1-js1,i2-js2,i3,axisp1,1)/(rx(i1-
     & js1,i2-js2,i3)*sy(i1-js1,i2-js2,i3)-ry(i1-js1,i2-js2,i3)*sx(i1-
     & js1,i2-js2,i3)))
                     divc2= (a11p1*u(i1+  is1,i2+  is2,i3,ex)-a11m1*u(
     & i1-  is1,i2-  is2,i3,ex))/(2.*dra) +(a12p1*u(i1+  is1,i2+  is2,
     & i3,ey)-a12m1*u(i1-  is1,i2-  is2,i3,ey))/(2.*dra) +(a21zp1*u(
     & i1+  js1,i2+  js2,i3,ex)-a21zm1*u(i1-  js1,i2-  js2,i3,ex))/(
     & 2.*dsa) +(a22zp1*u(i1+  js1,i2+  js2,i3,ey)-a22zm1*u(i1-  js1,
     & i2-  js2,i3,ey))/(2.*dsa)
                      write(*,'(" bc2: i=",3i3," u=",2f12.9," divc2=",
     & e10.2," div2=",e10.2)') i1,i2,i3,u(i1,i2,i3,ex),u(i1,i2,i3,ey),
     & divc2,ux22(i1,i2,i3,ex)+uy22(i1,i2,i3,ey)
                          ! '
                    end do
                    end do
                    end do
                  end if
               else
                 ! write(*,'(" bcOpt: called for curvilinear twilightZone, order=",i2," side,axis=",2i2)') orderOfAccuracy,side,axis
                  dra = dr(axis)*(1-2*side)
                  dsa = dr(axisp1)*(1-2*side)
                  do i3=n3a,n3b
                  do i2=n2a,n2b
                  do i1=n1a,n1b
                   if( mask(i1,i2,i3).gt.0 )then ! check for mask added 2015/06/01 *wdh*
                    ! ---- Boundary point is a physical point ---
                    jac=1./(rx(i1-is1,i2-is2,i3-is3)*sy(i1-is1,i2-is2,
     & i3-is3)-ry(i1-is1,i2-is2,i3-is3)*sx(i1-is1,i2-is2,i3-is3))
                    a11 =rsxy(i1-is1,i2-is2,i3-is3,axis  ,0)*jac
                    a12 =rsxy(i1-is1,i2-is2,i3-is3,axis  ,1)*jac
                    tau1=rsxy(i1-is1,i2-is2,i3-is3,axisp1,0)
                    tau2=rsxy(i1-is1,i2-is2,i3-is3,axisp1,1)
                    det = -tau2*a11+a12*tau1
                    jacp1=1./(rx(i1+is1,i2+is2,i3+is3)*sy(i1+is1,i2+
     & is2,i3+is3)-ry(i1+is1,i2+is2,i3+is3)*sx(i1+is1,i2+is2,i3+is3))
                    a11p1=jacp1*rsxy(i1+is1,i2+is2,i3+is3,axis,0)
                    a12p1=jacp1*rsxy(i1+is1,i2+is2,i3+is3,axis,1)
                    ! ** tau1DotU=tau1*u(i1,i2,i3,ex)+tau2*u(i1,i2,i3,ey)
                    ! ** u(i1,i2,i3,ex)=u(i1,i2,i3,ex)-tau1DotU*tau1
                    ! ** u(i1,i2,i3,ey)=u(i1,i2,i3,ey)-tau1DotU*tau2
                    ! gx2=tau1*(3.*u(i1,i2,i3,ex)-3.*u(i1+is1,i2+is2,i3+is3,ex)+u(i1+2*is1,i2+2*is2,i3,ex))!    +tau2*(3.*u(i1,i2,i3,ey)-3.*u(i1+is1,i2+is2,i3+is3,ey)+u(i1+2*is1,i2+2*is2,i3,ey))
                    ! use even derivatives of the tangential component:
                    ! gx2=tau1*(2.*u(i1,i2,i3,ex)-u(i1+is1,i2+is2,i3+is3,ex))!    +tau2*(2.*u(i1,i2,i3,ey)-u(i1+is1,i2+is2,i3+is3,ey))
                    gx2=tau1*(4.*u(i1,i2,i3,ex)-6.*u(i1+is1,i2+is2,i3,
     & ex)+4.*u(i1+2*is1,i2+2*is2,i3,ex)-u(i1+3*is1,i2+3*is2,i3,ex))+
     & tau2*(4.*u(i1,i2,i3,ey)-6.*u(i1+is1,i2+is2,i3,ey)+4.*u(i1+2*
     & is1,i2+2*is2,i3,ey)-u(i1+3*is1,i2+3*is2,i3,ey))
                    g2a=0.
                    ! include a2 terms in case tangential components are non-zero:
                    a21zp1=(rsxy(i1+js1,i2+js2,i3,axisp1,0)/(rx(i1+js1,
     & i2+js2,i3)*sy(i1+js1,i2+js2,i3)-ry(i1+js1,i2+js2,i3)*sx(i1+js1,
     & i2+js2,i3)))
                    a22zp1=(rsxy(i1+js1,i2+js2,i3,axisp1,1)/(rx(i1+js1,
     & i2+js2,i3)*sy(i1+js1,i2+js2,i3)-ry(i1+js1,i2+js2,i3)*sx(i1+js1,
     & i2+js2,i3)))
                    a21zm1=(rsxy(i1-js1,i2-js2,i3,axisp1,0)/(rx(i1-js1,
     & i2-js2,i3)*sy(i1-js1,i2-js2,i3)-ry(i1-js1,i2-js2,i3)*sx(i1-js1,
     & i2-js2,i3)))
                    a22zm1=(rsxy(i1-js1,i2-js2,i3,axisp1,1)/(rx(i1-js1,
     & i2-js2,i3)*sy(i1-js1,i2-js2,i3)-ry(i1-js1,i2-js2,i3)*sx(i1-js1,
     & i2-js2,i3)))
                    g1a=    a11p1*u(i1+is1,i2+is2,i3,ex)+ a12p1*u(i1+
     & is1,i2+is2,i3,ey)+ ( (a21zp1*u(i1+js1,i2+js2,i3,ex)+a22zp1*u(
     & i1+js1,i2+js2,i3,ey))-(a21zm1*u(i1-js1,i2-js2,i3,ex)+a22zm1*u(
     & i1-js1,i2-js2,i3,ey)) )*dra/dsa
                    if( forcingOption.eq.planeWaveBoundaryForcing )then
                      ! *** for planeWaveBoundaryForcing we need to use: u.t=w.y and v.t=-w.x =>
                      ! *****  (n1,n2).(w.x,w.y) = -n1*v.t + n2*u.t
                      !  OR    (rx,ry).(w.x,w.y) = -rx*v.t + ry*u.t
                      !   (rx**2+ry**2) w.r + (rx*sx+ry*sy)*ws = -rx*vt + ry*ut 
                      x0=xy(i1,i2,i3,0)
                      y0=xy(i1,i2,i3,1)
                      ! Note minus sign since we are subtracting out the incident field
                      if( fieldOption.eq.0 )then
                        u0t=-(ssf*(-twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(
     & y0)-cc*(t)))*pwc(0)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*
     & pwc(0))
                        v0t=-(ssf*(-twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(
     & y0)-cc*(t)))*pwc(1)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*
     & pwc(1))
                      else
                        ! we are assigning time derivatives (sosup)
                        u0t=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x0)+
     & ky*(y0)-cc*(t)))*pwc(0))+2.*ssft*(-twoPi*cc)*cos(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(0)+ssftt*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(
     & t)))*pwc(0))
                        v0t=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x0)+
     & ky*(y0)-cc*(t)))*pwc(1))+2.*ssft*(-twoPi*cc)*cos(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(1)+ssftt*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(
     & t)))*pwc(1))
                      endif
                      g2a=(2.*dra)*( (rsxy(i1,i2,i3,axis,0)*rsxy(i1,i2,
     & i3,axisp1,0)+rsxy(i1,i2,i3,axis,1)*rsxy(i1,i2,i3,axisp1,1))* (
     & u(i1+js1,i2+js2,i3,hz)-u(i1-js1,i2-js2,i3,hz))/(2.*dsa) + rsxy(
     & i1,i2,i3,axis,0)*v0t - rsxy(i1,i2,i3,axis,1)*u0t )/(rsxy(i1,i2,
     & i3,axis,0)**2+rsxy(i1,i2,i3,axis,1)**2)
                   ! write(*,'(" bc2: i=",3i3," side,axis,is1,is2,js1,js2=",6i3," dra,dsa=",2e10.2)') i1,i2,i3,side,axis,is1,is2,js1,js2,dra,dsa
                    else
                      ! this is new: include RHS 040116  -- note: this will be zero if the grid is orthogonal
                      g2a=(2.*dra)*( (rsxy(i1,i2,i3,axis,0)*rsxy(i1,i2,
     & i3,axisp1,0)+rsxy(i1,i2,i3,axis,1)*rsxy(i1,i2,i3,axisp1,1))* (
     & u(i1+js1,i2+js2,i3,hz)-u(i1-js1,i2-js2,i3,hz))/(2.*dsa) )/(
     & rsxy(i1,i2,i3,axis,0)**2+rsxy(i1,i2,i3,axis,1)**2)
                    end if
                      ! Since div(E)=0 for the TZ solutions, we do not need to adjust the BC's
                      ! *wdh* 2015/05/31 
                      !* g1a= a11p1*u(i1+is1,i2+is2,i3,ex)+ a12p1*u(i1+is1,i2+is2,i3,ey)
                      ! Evaluate true solution (fieldOption==0) or its time derivative (fieldOption==1)
                      !* call ogf2dfo(ep,fieldOption,xy(i1-is1,i2-is2,i3,0),xy(i1-is1,i2-is2,i3,1),t, um,vm,wm)
                      !* call ogf2dfo(ep,fieldOption,xy(i1    ,i2    ,i3,0),xy(i1    ,i2    ,i3,1),t, u0,v0,w0)
                      !* call ogf2dfo(ep,fieldOption,xy(i1+is1,i2+is2,i3,0),xy(i1+is1,i2+is2,i3,1),t, up,vp,wp)
                      !* g1a=g1a+ (  a11*um+  a12*vm ) - (a11p1*up+a12p1*vp)
                      !* gx2=gx2- (tau1*(2.*u0-up -um) !*          +tau2*(2.*v0-vp -vm) )
                      ! old: g2a=g2a+ wm-wp
                      call ogf2dfo(ep,fieldOption,xy(i1-is1,i2-is2,i3,
     & 0),xy(i1-is1,i2-is2,i3,1),t, um,vm,wm)
                      call ogf2dfo(ep,fieldOption,xy(i1+is1,i2+is2,i3,
     & 0),xy(i1+is1,i2+is2,i3,1),t, up,vp,wp)
                      call ogf2dfo(ep,fieldOption,xy(i1-js1,i2-js2,i3,
     & 0),xy(i1-js1,i2-js2,i3,1),t, uzm,vzm,wzm)
                      call ogf2dfo(ep,fieldOption,xy(i1+js1,i2+js2,i3,
     & 0),xy(i1+js1,i2+js2,i3,1),t, uzp,vzp,wzp)
                      ws = (wzp-wzm)/(2.*dsa)
                      wr = (wp - wm)/(2.*dra)
                      wx = rsxy(i1,i2,i3,axis,0)*wr + rsxy(i1,i2,i3,
     & axisp1,0)*ws
                      wy = rsxy(i1,i2,i3,axis,1)*wr + rsxy(i1,i2,i3,
     & axisp1,1)*ws
                      g2a=g2a -  (2.*dra)*( rsxy(i1,i2,i3,axis,0)*wx + 
     & rsxy(i1,i2,i3,axis,1)*wy )/(rsxy(i1,i2,i3,axis,0)**2+rsxy(i1,
     & i2,i3,axis,1)**2)
                      ! write(*,'("bcOpt: side,axis=",2i2," i1,i2=",2i4," x,y=",2f6.3," ex,ey,hz=",3f6.2)') !      side,axis,i1,i2,xy(i1    ,i2    ,i3,0),xy(i1    ,i2    ,i3,1),u0,v0,w0
                    if( useChargeDensity.eq.1 )then
                     ! div(eps*E) = rho , rho is saved in f(i1,i2,i3,0)
                     jac0=1./(rx(i1,i2,i3)*sy(i1,i2,i3)-ry(i1,i2,i3)*
     & sx(i1,i2,i3))
                     g1a = g1a - 2.*dra*f(i1,i2,i3,0)*jac0/eps
                    end if
                    u(i1-is1,i2-is2,i3-is3,ex) = (-tau2*g1a +a12*gx2)
     & /det
                    u(i1-is1,i2-is2,i3-is3,ey) = ( tau1*g1a -a11*gx2)
     & /det
                    u(i1-is1,i2-is2,i3-is3,hz)=u(i1+is1,i2+is2,i3+is3,
     & hz) + g2a
                   else if( mask(i1,i2,i3).lt.0 )then
                    ! ---- Boundary point is an interpolation point ---
                    ! extrapolate ghost points:
                     u(i1-is1,i2-is2,i3-is3,ex)=(3.*u(i1,i2,i3,ex)-3.*
     & u(i1+is1,i2+is2,i3+is3,ex)+u(i1+2*is1,i2+2*is2,i3+2*is3,ex))
                     u(i1-is1,i2-is2,i3-is3,ey)=(3.*u(i1,i2,i3,ey)-3.*
     & u(i1+is1,i2+is2,i3+is3,ey)+u(i1+2*is1,i2+2*is2,i3+2*is3,ey))
                     u(i1-is1,i2-is2,i3-is3,hz)=(3.*u(i1,i2,i3,hz)-3.*
     & u(i1+is1,i2+is2,i3+is3,hz)+u(i1+2*is1,i2+2*is2,i3+2*is3,hz))
                   else
                     ! boundary point is unused -- extrap for now ??
                     u(i1-is1,i2-is2,i3-is3,ex)=(3.*u(i1,i2,i3,ex)-3.*
     & u(i1+is1,i2+is2,i3+is3,ex)+u(i1+2*is1,i2+2*is2,i3+2*is3,ex))
                     u(i1-is1,i2-is2,i3-is3,ey)=(3.*u(i1,i2,i3,ey)-3.*
     & u(i1+is1,i2+is2,i3+is3,ey)+u(i1+2*is1,i2+2*is2,i3+2*is3,ey))
                     u(i1-is1,i2-is2,i3-is3,hz)=(3.*u(i1,i2,i3,hz)-3.*
     & u(i1+is1,i2+is2,i3+is3,hz)+u(i1+2*is1,i2+2*is2,i3+2*is3,hz))
                   end if
                  end do
                  end do
                  end do
                  if( .false. )then
                    do i3=n3a,n3b
                    do i2=n2a,n2b
                    do i1=n1a,n1b
                     a11p1= (rsxy(i1+is1,i2+is2,i3,axis,0)/(rx(i1+is1,
     & i2+is2,i3)*sy(i1+is1,i2+is2,i3)-ry(i1+is1,i2+is2,i3)*sx(i1+is1,
     & i2+is2,i3)))
                     a11m1= (rsxy(i1-is1,i2-is2,i3,axis,0)/(rx(i1-is1,
     & i2-is2,i3)*sy(i1-is1,i2-is2,i3)-ry(i1-is1,i2-is2,i3)*sx(i1-is1,
     & i2-is2,i3)))
                     a12p1= (rsxy(i1+is1,i2+is2,i3,axis,1)/(rx(i1+is1,
     & i2+is2,i3)*sy(i1+is1,i2+is2,i3)-ry(i1+is1,i2+is2,i3)*sx(i1+is1,
     & i2+is2,i3)))
                     a12m1= (rsxy(i1-is1,i2-is2,i3,axis,1)/(rx(i1-is1,
     & i2-is2,i3)*sy(i1-is1,i2-is2,i3)-ry(i1-is1,i2-is2,i3)*sx(i1-is1,
     & i2-is2,i3)))
                      a21zp1=(rsxy(i1+js1,i2+js2,i3,axisp1,0)/(rx(i1+
     & js1,i2+js2,i3)*sy(i1+js1,i2+js2,i3)-ry(i1+js1,i2+js2,i3)*sx(i1+
     & js1,i2+js2,i3)))
                      a22zp1=(rsxy(i1+js1,i2+js2,i3,axisp1,1)/(rx(i1+
     & js1,i2+js2,i3)*sy(i1+js1,i2+js2,i3)-ry(i1+js1,i2+js2,i3)*sx(i1+
     & js1,i2+js2,i3)))
                      a21zm1=(rsxy(i1-js1,i2-js2,i3,axisp1,0)/(rx(i1-
     & js1,i2-js2,i3)*sy(i1-js1,i2-js2,i3)-ry(i1-js1,i2-js2,i3)*sx(i1-
     & js1,i2-js2,i3)))
                      a22zm1=(rsxy(i1-js1,i2-js2,i3,axisp1,1)/(rx(i1-
     & js1,i2-js2,i3)*sy(i1-js1,i2-js2,i3)-ry(i1-js1,i2-js2,i3)*sx(i1-
     & js1,i2-js2,i3)))
                     divc2= (a11p1*u(i1+  is1,i2+  is2,i3,ex)-a11m1*u(
     & i1-  is1,i2-  is2,i3,ex))/(2.*dra) +(a12p1*u(i1+  is1,i2+  is2,
     & i3,ey)-a12m1*u(i1-  is1,i2-  is2,i3,ey))/(2.*dra) +(a21zp1*u(
     & i1+  js1,i2+  js2,i3,ex)-a21zm1*u(i1-  js1,i2-  js2,i3,ex))/(
     & 2.*dsa) +(a22zp1*u(i1+  js1,i2+  js2,i3,ey)-a22zm1*u(i1-  js1,
     & i2-  js2,i3,ey))/(2.*dsa)
                      write(*,'(" bc2: i=",3i3," u=",2f12.9," divc2=",
     & e10.2," div2=",e10.2)') i1,i2,i3,u(i1,i2,i3,ex),u(i1,i2,i3,ey),
     & divc2,ux22(i1,i2,i3,ex)+uy22(i1,i2,i3,ey)
                          ! '
                    end do
                    end do
                    end do
                  end if
               end if
         end if
           else if( boundaryCondition(side,axis).gt.0 .and. 
     & boundaryCondition(side,axis).ne.dirichlet .and. 
     & boundaryCondition(side,axis).ne.planeWaveBoundaryCondition 
     & .and. boundaryCondition(side,axis)
     & .ne.symmetryBoundaryCondition .and. boundaryCondition(side,
     & axis).gt.lastBC )then
           ! Note: some BC's such as dirichlet are done in assignBoundaryConditions.C
             write(*,'(" endLoopOverSides:ERROR: unknown 
     & boundaryCondition=",i6)') boundaryCondition(side,axis)
           ! '
             stop 7733
           end if
         end do
         end do
       !     **************************************************************************
        return
        end
