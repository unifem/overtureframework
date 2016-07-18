! This file automatically generated from bcOptMaxwell4.bf with bpp.
        subroutine bcOptMaxwell3dOrder4( nd, nd1a,nd1b,nd2a,nd2b,nd3a,
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
        !  --- NOTE: extra determines "extra points" in the tangential directions  ----
        !  extra=-1 by default (if adjacent BC>0) no need to do corners -- these are already done
        !  extra=numberOfGhostPoints, if bc==0, (set in begin loop over sides)
        !  extra=0 if bc<0  (set in begin loop over sides)
        extra=-1
        numberOfGhostPoints=orderOfAccuracy/2
        if( gridType.eq.curvilinear )then
         ! the 4th-order 3d BCs require two steps -- the first step gives initial values at all ghost points
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
             write(*,'(" bcOpt: grid,side,axis=",3i3,", loop bounds: 
     & n1a,n1b,n2a,n2b,n3a,n3b=",6i3)') grid,side,axis,n1a,n1b,n2a,
     & n2b,n3a,n3b
           end if
           if( useForcing.eq.0 )then
              ! Since is1 is +1 or -1 we need to flip the sign of dr in the derivative approximations
              dra = dr(axis  )*(1-2*side)
              dsa = dr(axisp1)*(1-2*side)
              dta = dr(axisp2)*(1-2*side)
              drb = dr(axis  )
              dsb = dr(axisp1)
              dtb = dr(axisp2)
              ! ** Fourth-order for tau.Delta\uv=0, setting  ctlrr=ctlr=0 in the code will revert to 2nd-order
              ctlrr=1.
              ctlr=1.
              if( debug.gt.0 )then
                write(*,'(" **bcCurvilinear3dOrder4Step1: START: grid,
     & side,axis=",3i2," is1,is2,is3=",3i3," ks1,ks2,ks3=",3i3)')grid,
     & side,axis,is1,is2,is3,ks1,ks2,ks3
              end if
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
              if( mask(i1,i2,i3).gt.0 )then
               ! precompute the inverse of the jacobian, used in macros AmnD3J
               i10=i1  ! used by jac3di in macros
               i20=i2
               i30=i3
               do m3=-2,2
               do m2=-2,2
               do m1=-2,2
                jac3di(m1,m2,m3)=1./(rx(i1+m1,i2+m2,i3+m3)*(sy(i1+m1,
     & i2+m2,i3+m3)*tz(i1+m1,i2+m2,i3+m3)-sz(i1+m1,i2+m2,i3+m3)*ty(i1+
     & m1,i2+m2,i3+m3))+ry(i1+m1,i2+m2,i3+m3)*(sz(i1+m1,i2+m2,i3+m3)*
     & tx(i1+m1,i2+m2,i3+m3)-sx(i1+m1,i2+m2,i3+m3)*tz(i1+m1,i2+m2,i3+
     & m3))+rz(i1+m1,i2+m2,i3+m3)*(sx(i1+m1,i2+m2,i3+m3)*ty(i1+m1,i2+
     & m2,i3+m3)-sy(i1+m1,i2+m2,i3+m3)*tx(i1+m1,i2+m2,i3+m3)))
               end do
               end do
               end do
               a11 =(rsxy(i1,i2,i3,axis,0)*jac3di(i1-i10,i2-i20,i3-i30)
     & )
               a12 =(rsxy(i1,i2,i3,axis,1)*jac3di(i1-i10,i2-i20,i3-i30)
     & )
               a13 =(rsxy(i1,i2,i3,axis,2)*jac3di(i1-i10,i2-i20,i3-i30)
     & )
               a21 =(rsxy(i1,i2,i3,axisp1,0)*jac3di(i1-i10,i2-i20,i3-
     & i30))
               a22 =(rsxy(i1,i2,i3,axisp1,1)*jac3di(i1-i10,i2-i20,i3-
     & i30))
               a23 =(rsxy(i1,i2,i3,axisp1,2)*jac3di(i1-i10,i2-i20,i3-
     & i30))
               a31 =(rsxy(i1,i2,i3,axisp2,0)*jac3di(i1-i10,i2-i20,i3-
     & i30))
               a32 =(rsxy(i1,i2,i3,axisp2,1)*jac3di(i1-i10,i2-i20,i3-
     & i30))
               a33 =(rsxy(i1,i2,i3,axisp2,2)*jac3di(i1-i10,i2-i20,i3-
     & i30))
               a11m1 =(rsxy(i1-is1,i2-is2,i3-is3,axis,0)*jac3di(i1-is1-
     & i10,i2-is2-i20,i3-is3-i30))
               a12m1 =(rsxy(i1-is1,i2-is2,i3-is3,axis,1)*jac3di(i1-is1-
     & i10,i2-is2-i20,i3-is3-i30))
               a13m1 =(rsxy(i1-is1,i2-is2,i3-is3,axis,2)*jac3di(i1-is1-
     & i10,i2-is2-i20,i3-is3-i30))
               a11p1 =(rsxy(i1+is1,i2+is2,i3+is3,axis,0)*jac3di(i1+is1-
     & i10,i2+is2-i20,i3+is3-i30))
               a12p1 =(rsxy(i1+is1,i2+is2,i3+is3,axis,1)*jac3di(i1+is1-
     & i10,i2+is2-i20,i3+is3-i30))
               a13p1 =(rsxy(i1+is1,i2+is2,i3+is3,axis,2)*jac3di(i1+is1-
     & i10,i2+is2-i20,i3+is3-i30))
               a11m2 =(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,0)*jac3di(
     & i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))
               a12m2 =(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,1)*jac3di(
     & i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))
               a13m2 =(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,2)*jac3di(
     & i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))
               a11p2 =(rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axis,0)*jac3di(
     & i1+2*is1-i10,i2+2*is2-i20,i3+2*is3-i30))
               a12p2 =(rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axis,1)*jac3di(
     & i1+2*is1-i10,i2+2*is2-i20,i3+2*is3-i30))
               a13p2 =(rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axis,2)*jac3di(
     & i1+2*is1-i10,i2+2*is2-i20,i3+2*is3-i30))
               c11 = (rsxy(i1,i2,i3,axis,0)**2+rsxy(i1,i2,i3,axis,1)**
     & 2+rsxy(i1,i2,i3,axis,2)**2)
               c22 = (rsxy(i1,i2,i3,axisp1,0)**2+rsxy(i1,i2,i3,axisp1,
     & 1)**2+rsxy(i1,i2,i3,axisp1,2)**2)
               c33 = (rsxy(i1,i2,i3,axisp2,0)**2+rsxy(i1,i2,i3,axisp2,
     & 1)**2+rsxy(i1,i2,i3,axisp2,2)**2)
               c1 = (rsxyx43(i1,i2,i3,axis,0)+rsxyy43(i1,i2,i3,axis,1)+
     & rsxyz43(i1,i2,i3,axis,2))
               c2 = (rsxyx43(i1,i2,i3,axisp1,0)+rsxyy43(i1,i2,i3,
     & axisp1,1)+rsxyz43(i1,i2,i3,axisp1,2))
               c3 = (rsxyx43(i1,i2,i3,axisp2,0)+rsxyy43(i1,i2,i3,
     & axisp2,1)+rsxyz43(i1,i2,i3,axisp2,2))
               us=(8.*(u(i1+js1,i2+js2,i3+js3,ex)-u(i1-js1,i2-js2,i3-
     & js3,ex))-(u(i1+2*js1,i2+2*js2,i3+2*js3,ex)-u(i1-2*js1,i2-2*js2,
     & i3-2*js3,ex)))/(12.*dsa)
               uss=(-30.*u(i1,i2,i3,ex)+16.*(u(i1+js1,i2+js2,i3+js3,ex)
     & +u(i1-js1,i2-js2,i3-js3,ex))-(u(i1+2*js1,i2+2*js2,i3+2*js3,ex)+
     & u(i1-2*js1,i2-2*js2,i3-2*js3,ex)))/(12.*dsa**2)
               vs=(8.*(u(i1+js1,i2+js2,i3+js3,ey)-u(i1-js1,i2-js2,i3-
     & js3,ey))-(u(i1+2*js1,i2+2*js2,i3+2*js3,ey)-u(i1-2*js1,i2-2*js2,
     & i3-2*js3,ey)))/(12.*dsa)
               vss=(-30.*u(i1,i2,i3,ey)+16.*(u(i1+js1,i2+js2,i3+js3,ey)
     & +u(i1-js1,i2-js2,i3-js3,ey))-(u(i1+2*js1,i2+2*js2,i3+2*js3,ey)+
     & u(i1-2*js1,i2-2*js2,i3-2*js3,ey)))/(12.*dsa**2)
               ws=(8.*(u(i1+js1,i2+js2,i3+js3,ez)-u(i1-js1,i2-js2,i3-
     & js3,ez))-(u(i1+2*js1,i2+2*js2,i3+2*js3,ez)-u(i1-2*js1,i2-2*js2,
     & i3-2*js3,ez)))/(12.*dsa)
               wss=(-30.*u(i1,i2,i3,ez)+16.*(u(i1+js1,i2+js2,i3+js3,ez)
     & +u(i1-js1,i2-js2,i3-js3,ez))-(u(i1+2*js1,i2+2*js2,i3+2*js3,ez)+
     & u(i1-2*js1,i2-2*js2,i3-2*js3,ez)))/(12.*dsa**2)
               ut=(8.*(u(i1+ks1,i2+ks2,i3+ks3,ex)-u(i1-ks1,i2-ks2,i3-
     & ks3,ex))-(u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ex)-u(i1-2*ks1,i2-2*ks2,
     & i3-2*ks3,ex)))/(12.*dta)
               utt=(-30.*u(i1,i2,i3,ex)+16.*(u(i1+ks1,i2+ks2,i3+ks3,ex)
     & +u(i1-ks1,i2-ks2,i3-ks3,ex))-(u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ex)+
     & u(i1-2*ks1,i2-2*ks2,i3-2*ks3,ex)))/(12.*dta**2)
               vt=(8.*(u(i1+ks1,i2+ks2,i3+ks3,ey)-u(i1-ks1,i2-ks2,i3-
     & ks3,ey))-(u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ey)-u(i1-2*ks1,i2-2*ks2,
     & i3-2*ks3,ey)))/(12.*dta)
               vtt=(-30.*u(i1,i2,i3,ey)+16.*(u(i1+ks1,i2+ks2,i3+ks3,ey)
     & +u(i1-ks1,i2-ks2,i3-ks3,ey))-(u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ey)+
     & u(i1-2*ks1,i2-2*ks2,i3-2*ks3,ey)))/(12.*dta**2)
               wt=(8.*(u(i1+ks1,i2+ks2,i3+ks3,ez)-u(i1-ks1,i2-ks2,i3-
     & ks3,ez))-(u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ez)-u(i1-2*ks1,i2-2*ks2,
     & i3-2*ks3,ez)))/(12.*dta)
               wtt=(-30.*u(i1,i2,i3,ez)+16.*(u(i1+ks1,i2+ks2,i3+ks3,ez)
     & +u(i1-ks1,i2-ks2,i3-ks3,ez))-(u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ez)+
     & u(i1-2*ks1,i2-2*ks2,i3-2*ks3,ez)))/(12.*dta**2)
              tau11=rsxy(i1,i2,i3,axisp1,0)
              tau12=rsxy(i1,i2,i3,axisp1,1)
              tau13=rsxy(i1,i2,i3,axisp1,2)
              tau21=rsxy(i1,i2,i3,axisp2,0)
              tau22=rsxy(i1,i2,i3,axisp2,1)
              tau23=rsxy(i1,i2,i3,axisp2,2)
              uex=u(i1,i2,i3,ex)
              uey=u(i1,i2,i3,ey)
              uez=u(i1,i2,i3,ez)
             ! ************ Answer *******************
              Da1DotU=0.
              tau1DotUtt=0.
              tau2DotUtt=0.
              gIVf1=0.
              gIVf2=0.
              if( forcingOption.eq.planeWaveBoundaryForcing )then
                ! In the plane wave forcing case we subtract out a plane wave incident field
                ! This causes the BC to be 
                !           tau.u = - tau.uI
                !   and     tau.utt = -tau.uI.tt
                a21s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axisp1,0)*
     & jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))-(rsxy(i1-js1,i2-js2,
     & i3-js3,axisp1,0)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,0)*jac3di(i1+2*js1-i10,
     & i2+2*js2-i20,i3+2*js3-i30))-(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,
     & axisp1,0)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))/(
     & 12.*dsa)
                a22s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axisp1,1)*
     & jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))-(rsxy(i1-js1,i2-js2,
     & i3-js3,axisp1,1)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,1)*jac3di(i1+2*js1-i10,
     & i2+2*js2-i20,i3+2*js3-i30))-(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,
     & axisp1,1)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))/(
     & 12.*dsa)
                a23s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axisp1,2)*
     & jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))-(rsxy(i1-js1,i2-js2,
     & i3-js3,axisp1,2)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,2)*jac3di(i1+2*js1-i10,
     & i2+2*js2-i20,i3+2*js3-i30))-(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,
     & axisp1,2)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))/(
     & 12.*dsa)
                a31t = (8.*((rsxy(i1+ks1,i2+ks2,i3+ks3,axisp2,0)*
     & jac3di(i1+ks1-i10,i2+ks2-i20,i3+ks3-i30))-(rsxy(i1-ks1,i2-ks2,
     & i3-ks3,axisp2,0)*jac3di(i1-ks1-i10,i2-ks2-i20,i3-ks3-i30)))-((
     & rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axisp2,0)*jac3di(i1+2*ks1-i10,
     & i2+2*ks2-i20,i3+2*ks3-i30))-(rsxy(i1-2*ks1,i2-2*ks2,i3-2*ks3,
     & axisp2,0)*jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30))))/(
     & 12.*dta)
                a32t = (8.*((rsxy(i1+ks1,i2+ks2,i3+ks3,axisp2,1)*
     & jac3di(i1+ks1-i10,i2+ks2-i20,i3+ks3-i30))-(rsxy(i1-ks1,i2-ks2,
     & i3-ks3,axisp2,1)*jac3di(i1-ks1-i10,i2-ks2-i20,i3-ks3-i30)))-((
     & rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axisp2,1)*jac3di(i1+2*ks1-i10,
     & i2+2*ks2-i20,i3+2*ks3-i30))-(rsxy(i1-2*ks1,i2-2*ks2,i3-2*ks3,
     & axisp2,1)*jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30))))/(
     & 12.*dta)
                a33t = (8.*((rsxy(i1+ks1,i2+ks2,i3+ks3,axisp2,2)*
     & jac3di(i1+ks1-i10,i2+ks2-i20,i3+ks3-i30))-(rsxy(i1-ks1,i2-ks2,
     & i3-ks3,axisp2,2)*jac3di(i1-ks1-i10,i2-ks2-i20,i3-ks3-i30)))-((
     & rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axisp2,2)*jac3di(i1+2*ks1-i10,
     & i2+2*ks2-i20,i3+2*ks3-i30))-(rsxy(i1-2*ks1,i2-2*ks2,i3-2*ks3,
     & axisp2,2)*jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30))))/(
     & 12.*dta)
                ! *** set RHS for (a1.u).r =  - Ds( a2.uv ) -Dt( a3.uv )
                Da1DotU = -(  a21s*uex+a22s*uey+a23s*uez + a21*us+a22*
     & vs+a23*ws + a31t*uex+a32t*uey+a33t*uez + a31*ut+a32*vt+a33*wt )
                 x00=xy(i1,i2,i3,0)
                 y00=xy(i1,i2,i3,1)
                 z00=xy(i1,i2,i3,2)
                 if( fieldOption.eq.0 )then
                   udd=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x00)+ky*(
     & y00)+kz*(z00)-cc*(t)))*pwc(0))+2.*ssft*(-twoPi*cc)*cos(twoPi*(
     & kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0)+ssftt*sin(twoPi*(kx*
     & (x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))
                   vdd=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x00)+ky*(
     & y00)+kz*(z00)-cc*(t)))*pwc(1))+2.*ssft*(-twoPi*cc)*cos(twoPi*(
     & kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1)+ssftt*sin(twoPi*(kx*
     & (x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))
                   wdd=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x00)+ky*(
     & y00)+kz*(z00)-cc*(t)))*pwc(2))+2.*ssft*(-twoPi*cc)*cos(twoPi*(
     & kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2)+ssftt*sin(twoPi*(kx*
     & (x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))
                 else
                   ! get time derivative (sosup) 
                   udd=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x00)+ky*(
     & y00)+kz*(z00)-cc*(t)))*pwc(0))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))+3.*ssftt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0)
     & +ssfttt*sin(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))
                   vdd=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x00)+ky*(
     & y00)+kz*(z00)-cc*(t)))*pwc(1))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))+3.*ssftt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1)
     & +ssfttt*sin(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))
                   wdd=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x00)+ky*(
     & y00)+kz*(z00)-cc*(t)))*pwc(2))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))+3.*ssftt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2)
     & +ssfttt*sin(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))
                 end if
                tau1DotUtt = tau11*udd+tau12*vdd+tau13*wdd
                tau2DotUtt = tau21*udd+tau22*vdd+tau23*wdd
              end if
             ! Now assign E at the ghost points:


! ************ Results from bc43d.maple *******************


! ************ solution using extrapolation for a1.u *******************
      gIII1=-tau11*(c22*uss+c2*us+c33*utt+c3*ut)-tau12*(c22*vss+c2*vs+
     & c33*vtt+c3*vt)-tau13*(c22*wss+c2*ws+c33*wtt+c3*wt)

      gIII2=-tau21*(c22*uss+c2*us+c33*utt+c3*ut)-tau22*(c22*vss+c2*vs+
     & c33*vtt+c3*vt)-tau23*(c22*wss+c2*ws+c33*wtt+c3*wt)

      tau1U=tau11*u(i1,i2,i3,ex)+tau12*u(i1,i2,i3,ey)+tau13*u(i1,i2,i3,
     & ez)

      tau1Up1=tau11*u(i1+is1,i2+is2,i3+is3,ex)+tau12*u(i1+is1,i2+is2,
     & i3+is3,ey)+tau13*u(i1+is1,i2+is2,i3+is3,ez)

      tau1Up2=tau11*u(i1+2*is1,i2+2*is2,i3+2*is3,ex)+tau12*u(i1+2*is1,
     & i2+2*is2,i3+2*is3,ey)+tau13*u(i1+2*is1,i2+2*is2,i3+2*is3,ez)

      tau1Up3=tau11*u(i1+3*is1,i2+3*is2,i3+3*is3,ex)+tau12*u(i1+3*is1,
     & i2+3*is2,i3+3*is3,ey)+tau13*u(i1+3*is1,i2+3*is2,i3+3*is3,ez)

      tau2U=tau21*u(i1,i2,i3,ex)+tau22*u(i1,i2,i3,ey)+tau23*u(i1,i2,i3,
     & ez)

      tau2Up1=tau21*u(i1+is1,i2+is2,i3+is3,ex)+tau22*u(i1+is1,i2+is2,
     & i3+is3,ey)+tau23*u(i1+is1,i2+is2,i3+is3,ez)

      tau2Up2=tau21*u(i1+2*is1,i2+2*is2,i3+2*is3,ex)+tau22*u(i1+2*is1,
     & i2+2*is2,i3+2*is3,ey)+tau23*u(i1+2*is1,i2+2*is2,i3+2*is3,ez)

      tau2Up3=tau21*u(i1+3*is1,i2+3*is2,i3+3*is3,ex)+tau22*u(i1+3*is1,
     & i2+3*is2,i3+3*is3,ey)+tau23*u(i1+3*is1,i2+3*is2,i3+3*is3,ez)

! tau1.D+^p u = 0
      gIV1=-10*tau1U+10*tau1Up1-5*tau1Up2+tau1Up3 +gIVf1

! tau2.D+^p u = 0
      gIV2=-10*tau2U+10*tau2Up1-5*tau2Up2+tau2Up3 +gIVf2


! ttu11 = tau1.u(-1), ttu12 = tau1.u(-2)
      ttu11=-(-12*c11*tau1Up1+24*c11*tau1U+c11*ctlrr*tau1Up2-4*c11*
     & ctlrr*tau1Up1+6*c11*ctlrr*tau1U+c11*ctlrr*gIV1-6*c1*dra*
     & tau1Up1+c1*dra*ctlr*tau1Up2-2*c1*dra*ctlr*tau1Up1-c1*dra*ctlr*
     & gIV1+12*gIII1*dra**2+12*tau1DotUtt*dra**2)/(-12*c11+c11*ctlrr+
     & 6*c1*dra-3*c1*dra*ctlr)
      ttu12=-(-60*c11*tau1Up1+120*c11*tau1U+5*c11*ctlrr*tau1Up2-20*c11*
     & ctlrr*tau1Up1+30*c11*ctlrr*tau1U+4*c11*ctlrr*gIV1-30*c1*dra*
     & tau1Up1+5*c1*dra*ctlr*tau1Up2-10*c1*dra*ctlr*tau1Up1-2*c1*dra*
     & ctlr*gIV1+60*gIII1*dra**2+60*tau1DotUtt*dra**2+12*c11*gIV1-6*
     & gIV1*c1*dra)/(-12*c11+c11*ctlrr+6*c1*dra-3*c1*dra*ctlr)

! ttu21 = tau2.u(-1), ttu22 = tau2.u(-2)
      ttu21=-(-12*c11*tau2Up1+24*c11*tau2U+c11*ctlrr*tau2Up2-4*c11*
     & ctlrr*tau2Up1+6*c11*ctlrr*tau2U+c11*ctlrr*gIV2-6*c1*dra*
     & tau2Up1+c1*dra*ctlr*tau2Up2-2*c1*dra*ctlr*tau2Up1-c1*dra*ctlr*
     & gIV2+12*gIII2*dra**2+12*tau2DotUtt*dra**2)/(-12*c11+c11*ctlrr+
     & 6*c1*dra-3*c1*dra*ctlr)
      ttu22=-(-60*c11*tau2Up1+120*c11*tau2U+5*c11*ctlrr*tau2Up2-20*c11*
     & ctlrr*tau2Up1+30*c11*ctlrr*tau2U+4*c11*ctlrr*gIV2-30*c1*dra*
     & tau2Up1+5*c1*dra*ctlr*tau2Up2-10*c1*dra*ctlr*tau2Up1-2*c1*dra*
     & ctlr*gIV2+60*gIII2*dra**2+60*tau2DotUtt*dra**2+12*c11*gIV2-6*
     & gIV2*c1*dra)/(-12*c11+c11*ctlrr+6*c1*dra-3*c1*dra*ctlr)

      ! *********** set tangential components to be exact *****
      ! OGF3D(i1-is1,i2-is2,i3-is3,t, uvm(0),uvm(1),uvm(2))
      ! ttu11=tau11*uvm(0)+tau12*uvm(1)+tau13*uvm(2)
      ! ttu21=tau21*uvm(0)+tau22*uvm(1)+tau23*uvm(2)
      ! OGF3D(i1-2*is1,i2-2*is2,i3-2*is3,t, uvm(0),uvm(1),uvm(2))
      ! ttu12=tau11*uvm(0)+tau12*uvm(1)+tau13*uvm(2)
      ! ttu22=tau21*uvm(0)+tau22*uvm(1)+tau23*uvm(2)
      ! ******************************************************

      f1f  =a11*(15.*u(i1,i2,i3,ex)-20.*u(i1+is1,i2+is2,i3+is3,ex)+15.*
     & u(i1+2*is1,i2+2*is2,i3+2*is3,ex)-6.*u(i1+3*is1,i2+3*is2,i3+3*
     & is3,ex)+u(i1+4*is1,i2+4*is2,i3+4*is3,ex))+a12*(15.*u(i1,i2,i3,
     & ey)-20.*u(i1+is1,i2+is2,i3+is3,ey)+15.*u(i1+2*is1,i2+2*is2,i3+
     & 2*is3,ey)-6.*u(i1+3*is1,i2+3*is2,i3+3*is3,ey)+u(i1+4*is1,i2+4*
     & is2,i3+4*is3,ey))+a13*(15.*u(i1,i2,i3,ez)-20.*u(i1+is1,i2+is2,
     & i3+is3,ez)+15.*u(i1+2*is1,i2+2*is2,i3+2*is3,ez)-6.*u(i1+3*is1,
     & i2+3*is2,i3+3*is3,ez)+u(i1+4*is1,i2+4*is2,i3+4*is3,ez))

      f2um2=1/12.*a11m2
      f2um1=-2/3.*a11m1
      f2vm2=1/12.*a12m2
      f2vm1=-2/3.*a12m1
      f2wm2=1/12.*a13m2
      f2wm1=-2/3.*a13m1
      f2f  =2/3.*a11p1*u(i1+is1,i2+is2,i3+is3,ex)+2/3.*a12p1*u(i1+is1,
     & i2+is2,i3+is3,ey)+2/3.*a13p1*u(i1+is1,i2+is2,i3+is3,ez)-1/12.*
     & a11p2*u(i1+2*is1,i2+2*is2,i3+2*is3,ex)-1/12.*a12p2*u(i1+2*is1,
     & i2+2*is2,i3+2*is3,ey)-1/12.*a13p2*u(i1+2*is1,i2+2*is2,i3+2*is3,
     & ez)-Da1DotU*dra

      u(i1-is1,i2-is2,i3-is3,ez) = -1.*(tau21*f1f*f2um2*tau23*tau12**2-
     & 1.*tau12*tau22*tau13*tau21*f1f*f2um2+f1f*tau22*f2vm2*tau23*
     & tau11**2-1.*f1f*tau22**2*f2wm2*tau11**2+tau11*tau13*f1f*tau22**
     & 2*f2um2-1.*tau11*f1f*tau23*tau12*f2vm2*tau21-1.*tau11*tau13*
     & f1f*tau22*f2vm2*tau21-1.*tau11*f1f*tau22*f2um2*tau23*tau12+2.*
     & tau11*f1f*tau22*f2wm2*tau12*tau21+tau12*tau13*tau21**2*f1f*
     & f2vm2-1.*tau21**2*f1f*f2wm2*tau12**2+ttu12*f2vm2*tau21**2*a13*
     & tau12-1.*ttu12*f2vm2*tau21*tau23*a11*tau12-1.*ttu12*tau22*
     & f2vm2*tau21*a13*tau11+ttu12*tau22**2*f2um2*a13*tau11-1.*ttu12*
     & tau22*f2um2*a13*tau21*tau12-1.*ttu12*tau22*f2um2*tau23*a12*
     & tau11+f2um1*ttu21*tau21*tau12**2*a13+tau21**2*f2f*tau12**2*a13+
     & f2um2*a13*tau12**2*tau21*ttu22-1.*tau12*tau22*tau21*a13*ttu11*
     & f2um1-1.*tau21*f2f*tau23*tau12**2*a11-1.*tau12*tau21*a13*ttu21*
     & tau11*f2vm1-1.*tau12*tau21*tau23*a11*ttu11*f2vm1-1.*tau12*
     & tau22*ttu22*f2um2*a13*tau11-1.*tau12*tau13*f2um1*ttu21*tau21*
     & a12-1.*tau12*tau13*f2um2*a12*tau21*ttu22+tau12*tau21**2*a13*
     & ttu11*f2vm1+tau12*tau22*tau13*tau21*f2f*a11-1.*tau12*tau13*
     & tau21**2*f2f*a12+tau11*a13*ttu11*tau22**2*f2um1-1.*tau11*tau13*
     & tau22*f2vm2*ttu22*a11-1.*tau11*tau13*a11*tau22**2*f2f+tau11*
     & tau22*f2f*tau23*tau12*a11-1.*tau11*tau13*a11*ttu21*tau22*f2vm1-
     & 1.*tau11*tau21*a13*ttu11*tau22*f2vm1+tau11*tau21*tau13*a12*
     & tau22*f2f-2.*tau11*tau21*a13*tau12*tau22*f2f-1.*tau11*a13*
     & tau12*tau22*f2um1*ttu21-1.*tau11*a12*tau22*tau23*f2um1*ttu11+
     & tau11*tau21*a12*tau23*tau12*f2f+a13*tau22**2*tau11**2*f2f+
     & tau22*f2vm2*ttu22*a13*tau11**2-1.*a12*tau23*tau11**2*tau22*f2f-
     & 1.*tau11*tau21*ttu22*f2vm2*a13*tau12+a13*ttu21*tau11**2*tau22*
     & f2vm1+6.*ttu21*f2wm2*tau12**2*tau21*a11+tau22*tau11*f2wm2*
     & tau12*ttu22*a11+tau21*tau23*ttu11*f2vm1*tau11*a12+tau21*tau13*
     & a11*ttu11*tau22*f2vm1+tau22*f2vm2*tau23*tau11*ttu12*a11-1.*
     & tau21*tau12**2*f2wm2*ttu22*a11+tau21*a11*tau12*tau22*f2wm2*
     & ttu12-1.*ttu21*tau23*f2vm1*tau11**2*a12-6.*ttu21*f2vm2*tau23*
     & tau11**2*a12-6.*ttu21*f2um2*tau23*a11*tau12**2+ttu21*tau23*
     & f2vm1*tau11*tau12*a11-6.*ttu21*tau22*f2wm2*tau11*a11*tau12+6.*
     & ttu21*f2um2*tau23*a12*tau11*tau12+6.*ttu21*f2vm2*tau23*tau11*
     & a11*tau12-6.*tau21*ttu11*tau22*f2wm2*tau11*a12+6.*tau21*tau13*
     & ttu11*tau22*f2um2*a12+tau21*tau13*ttu21*f2vm1*tau11*a12+tau21*
     & a12*tau12*f2wm2*tau11*ttu22+tau22*tau13*f2um2*a12*tau11*ttu22+
     & 6.*tau22*tau13*f2um2*a11*ttu21*tau12+6.*tau22*tau13*f2vm2*
     & tau21*a11*ttu11-1.*tau22*f2wm2*tau11**2*a12*ttu22+tau22*tau13*
     & f2um1*ttu11*tau21*a12+tau22*tau13*f2um1*ttu21*tau12*a11-6.*
     & ttu21*tau12*f2wm2*tau11*a12*tau21-6.*ttu21*tau13*f2vm2*tau21*
     & a11*tau12+6.*tau22*f2um2*tau23*a11*ttu11*tau12-6.*tau22*f2wm2*
     & tau12*tau21*a11*ttu11-6.*tau22*f2vm2*tau23*tau11*a11*ttu11+
     & tau22*tau23*f2um1*ttu11*tau12*a11+tau22*tau21*f2wm2*ttu12*a12*
     & tau11+tau21*tau13*tau12*f2vm2*ttu22*a11-6.*tau21**2*tau13*
     & ttu11*f2vm2*a12+6.*tau21**2*ttu11*f2wm2*tau12*a12-1.*tau21**2*
     & tau13*ttu11*f2vm1*a12-1.*tau21**2*ttu12*f2wm2*tau12*a12+6.*
     & tau21*tau13*ttu21*f2vm2*a12*tau11-6.*ttu21*tau22*tau13*f2um2*
     & a12*tau11-1.*ttu21*tau23*f2um1*tau12**2*a11+ttu21*tau23*f2um1*
     & tau12*a12*tau11+6.*ttu21*tau22*f2wm2*tau11**2*a12-6.*tau22**2*
     & tau13*f2um2*a11*ttu11-1.*tau22**2*tau13*f2um1*ttu11*a11-1.*
     & tau22**2*f2wm2*ttu12*tau11*a11+6.*tau22**2*f2wm2*tau11*a11*
     & ttu11+tau21*tau23*ttu12*f2um2*a12*tau12+6.*tau21*tau23*ttu11*
     & f2vm2*a12*tau11-6.*tau21*tau23*ttu11*f2um2*a12*tau12)/(tau23*
     & tau12*f2vm1*tau21*a13*tau11+6.*tau23*tau12*f2wm2*tau11*a12*
     & tau21+tau23*tau12*f2wm1*tau11*tau21*a12-1.*f2wm1*tau21*tau12**
     & 2*tau23*a11+6.*tau23*tau12*f2vm2*tau21*a13*tau11-1.*tau23*
     & f2um1*tau12**2*tau21*a13+6.*tau13*f2vm2*tau21*tau23*a11*tau12-
     & 2.*tau13*tau23*f2vm1*tau11*tau21*a12-6.*f2wm2*tau12**2*tau21*
     & tau23*a11-6.*tau13*f2vm2*tau21**2*a13*tau12+tau13*f2vm1*tau21*
     & tau23*tau12*a11-1.*tau23**2*f2um1*tau12*a12*tau11+tau13*tau23*
     & f2um1*tau12*tau21*a12-12.*tau13*f2vm2*tau21*tau23*a12*tau11-6.*
     & tau13*f2wm2*tau12*tau21**2*a12-1.*tau22**2*tau13*f2um1*a13*
     & tau11+tau22*f2wm1*tau11*tau23*tau12*a11-6.*tau22*f2wm2*tau11**
     & 2*tau23*a12-6.*tau22*tau13**2*f2um2*a12*tau21-1.*tau22*tau13**
     & 2*f2um1*tau21*a12-6.*tau22*tau13**2*f2vm2*tau21*a11-1.*tau22*
     & tau13**2*f2vm1*tau21*a11-1.*tau22*f2wm1*tau11**2*a12*tau23-1.*
     & tau22*tau23*f2vm1*tau11**2*a13-12.*tau22*tau13*f2um2*tau23*a11*
     & tau12-1.*tau13*f2vm1*tau21**2*tau12*a13-2.*tau22*f2wm1*tau21*
     & tau12*a13*tau11+tau22*tau23*f2um1*tau12*a13*tau11-6.*f2um2*
     & tau23**2*a12*tau11*tau12-6.*f2um2*a13*tau21*tau12**2*tau23-6.*
     & f2vm2*tau23**2*tau11*a11*tau12+6.*tau13*f2um2*a12*tau21*tau23*
     & tau12+6.*tau22*tau13*f2um2*a13*tau21*tau12+6.*tau22*tau13*
     & f2wm2*tau12*tau21*a11+tau13**2*f2vm1*tau21**2*a12+tau22*tau13*
     & f2vm1*tau21*a13*tau11+tau22*tau13*f2wm1*tau11*tau21*a12+6.*
     & tau13**2*f2vm2*tau21**2*a12-6.*tau22**2*tau13*f2um2*a13*tau11+
     & tau22*tau13*f2wm1*tau21*tau12*a11+6.*tau22*tau13*f2vm2*tau21*
     & a13*tau11-2.*tau22*tau13*f2um1*a11*tau23*tau12+tau22*tau13*
     & f2um1*a12*tau11*tau23-1.*tau22**2*tau13*f2wm1*tau11*a11+tau22*
     & tau13*f2um1*tau21*tau12*a13-1.*tau23**2*f2vm1*tau11*tau12*a11+
     & 6.*tau22*f2wm2*tau11*tau23*a11*tau12-1.*tau13*f2wm1*tau21**2*
     & tau12*a12-6.*tau22*f2vm2*tau23*tau11**2*a13-6.*tau22**2*tau13*
     & f2wm2*tau11*a11+tau23**2*f2vm1*tau11**2*a12+6.*f2vm2*tau23**2*
     & tau11**2*a12+6.*f2um2*tau23**2*a11*tau12**2+6.*tau22*f2um2*a13*
     & tau11*tau23*tau12-12.*tau22*f2wm2*tau12*tau21*a13*tau11+6.*
     & tau22*tau13*f2wm2*tau11*a12*tau21+tau22*tau13*tau23*f2vm1*
     & tau11*a11+6.*tau22*tau13*f2vm2*tau23*tau11*a11+6.*tau22*tau13*
     & f2um2*tau23*a12*tau11+tau22**2*tau13**2*f2um1*a11+6.*f2wm2*
     & tau12**2*tau21**2*a13+tau23**2*f2um1*tau12**2*a11+f2wm1*tau21**
     & 2*tau12**2*a13+6.*tau22**2*f2wm2*tau11**2*a13+tau22**2*f2wm1*
     & tau11**2*a13+6.*tau22**2*tau13**2*f2um2*a11)

      u(i1-2*is1,i2-2*is2,i3-2*is3,ez) = (-1.*tau21**2*f2wm1*tau12**2*
     & f1f-1.*f2wm1*tau11**2*tau22**2*f1f+f2vm1*tau23*tau11**2*f1f*
     & tau22+tau11*tau13*f2um1*tau22**2*f1f-1.*tau11*tau21*f2vm1*f1f*
     & tau23*tau12-1.*tau11*tau21*tau13*f2vm1*f1f*tau22-1.*tau11*
     & f2um1*tau23*tau12*f1f*tau22+tau12*tau13*tau21**2*f2vm1*f1f-1.*
     & tau12*tau22*tau13*tau21*f2um1*f1f+tau21*f2um1*tau23*tau12**2*
     & f1f+2.*tau11*tau21*f2wm1*tau12*f1f*tau22-1.*ttu12*tau22*tau13*
     & f2um1*tau21*a12-6.*ttu12*tau22*tau13*f2vm2*tau21*a11+ttu12*
     & tau23*f2um1*tau12*tau21*a12-6.*ttu12*tau22*tau13*f2um2*a12*
     & tau21-1.*ttu12*tau22*tau13*f2vm1*tau21*a11-6.*ttu12*f2vm2*
     & tau21**2*a13*tau12+6.*ttu12*f2vm2*tau21*tau23*a11*tau12+6.*
     & ttu12*tau22*f2vm2*tau21*a13*tau11+ttu12*tau22*f2wm1*tau11*
     & tau21*a12+6.*ttu12*tau13*f2vm2*tau21**2*a12-6.*ttu12*tau22**2*
     & f2um2*a13*tau11+6.*ttu12*tau22*f2um2*a13*tau21*tau12+ttu12*
     & tau13*f2vm1*tau21**2*a12+ttu12*tau22*tau23*f2vm1*tau11*a11+
     & ttu12*tau22*f2wm1*tau21*tau12*a11+ttu12*tau22**2*tau13*f2um1*
     & a11-1.*ttu12*f2wm1*tau21**2*tau12*a12-1.*ttu12*tau22**2*f2wm1*
     & tau11*a11+6.*ttu12*tau22**2*tau13*f2um2*a11+6.*ttu12*tau22*
     & f2um2*tau23*a12*tau11-6.*f2um1*ttu21*tau21*tau12**2*a13-6.*
     & tau21**2*f2f*tau12**2*a13-6.*f2um2*a13*tau12**2*tau21*ttu22+6.*
     & tau12*tau22*tau21*a13*ttu11*f2um1+6.*tau21*f2f*tau23*tau12**2*
     & a11-1.*tau12*tau22*tau13*ttu22*f2um1*a11-6.*tau12*tau22*tau13*
     & ttu22*f2um2*a11+tau12*tau22*ttu22*f2wm1*tau11*a11-1.*tau12*
     & tau22*tau23*ttu12*f2um1*a11+6.*tau12*tau21*a13*ttu21*tau11*
     & f2vm1-1.*tau12*ttu22*tau23*f2vm1*tau11*a11+6.*tau12*tau21*
     & tau23*a11*ttu11*f2vm1-6.*tau12*tau22*tau23*ttu12*f2um2*a11+6.*
     & tau12*tau22*ttu22*f2um2*a13*tau11-6.*tau12*tau23*f2um1*ttu11*
     & tau21*a12+6.*tau12*tau13*f2um1*ttu21*tau21*a12-6.*tau12*f2wm1*
     & ttu21*tau11*tau21*a12+6.*tau12*tau13*f2um2*a12*tau21*ttu22-6.*
     & tau12*tau13*tau21*a11*ttu21*f2vm1+tau12*tau13*ttu22*f2vm1*
     & tau21*a11-6.*tau12*tau21**2*a13*ttu11*f2vm1+6.*tau12*tau21**2*
     & f2wm1*ttu11*a12-6.*tau12*tau22*tau13*tau21*f2f*a11-6.*tau12*
     & tau22*tau21*f2wm1*ttu11*a11+6.*tau12*tau13*tau21**2*f2f*a12-6.*
     & tau12*f2um2*a12*tau11*tau23*ttu22-6.*tau11*tau23*a11*ttu11*
     & tau22*f2vm1-1.*tau11*a12*ttu22*tau23*f2um1*tau12-6.*tau11*a13*
     & ttu11*tau22**2*f2um1+6.*tau11*tau13*tau22*f2vm2*ttu22*a11+6.*
     & tau11*tau13*a11*tau22**2*f2f-6.*tau11*tau22*f2f*tau23*tau12*
     & a11+6.*tau11*tau13*a11*ttu21*tau22*f2vm1+6.*tau11*tau21*a13*
     & ttu11*tau22*f2vm1-6.*tau11*tau21*tau13*a12*ttu22*f2vm2-6.*
     & tau11*tau21*tau13*a12*tau22*f2f+12.*tau11*tau21*a13*tau12*
     & tau22*f2f+tau11*tau21*a12*ttu22*f2wm1*tau12-1.*tau11*tau21*
     & tau13*a12*ttu22*f2vm1-6.*tau11*tau13*a12*tau22*f2um1*ttu21+6.*
     & tau11*a13*tau12*tau22*f2um1*ttu21+6.*tau11*a12*tau22*tau23*
     & f2um1*ttu11-6.*tau11*a11*ttu21*tau22*f2wm1*tau12+ttu22*f2um1*
     & a11*tau23*tau12**2-1.*ttu22*f2wm1*tau21*tau12**2*a11+6.*tau21*
     & a11*ttu21*f2wm1*tau12**2+6.*ttu22*f2um2*tau23*a11*tau12**2-6.*
     & tau11*tau21*a12*tau23*tau12*f2f-6.*a13*tau22**2*tau11**2*f2f+
     & a12*ttu22*tau23*f2vm1*tau11**2+6.*a12*tau22*f2wm1*ttu21*tau11**
     & 2-6.*tau22*f2vm2*ttu22*a13*tau11**2+6.*a12*ttu22*f2vm2*tau23*
     & tau11**2-1.*a12*ttu22*tau22*f2wm1*tau11**2+6.*a12*tau23*tau11**
     & 2*tau22*f2f+6.*tau11*a11*tau22**2*f2wm1*ttu11+tau11*tau13*a12*
     & ttu22*tau22*f2um1-6.*tau11*tau23*tau12*f2vm2*ttu22*a11-1.*
     & tau11*tau21*a12*tau23*ttu12*f2vm1-6.*tau11*tau21*a12*tau22*
     & f2wm1*ttu11+6.*tau11*tau21*ttu22*f2vm2*a13*tau12-6.*tau11*
     & tau21*a12*tau23*ttu12*f2vm2-6.*a13*ttu21*tau11**2*tau22*f2vm1)
     & /(tau23*tau12*f2vm1*tau21*a13*tau11+6.*tau23*tau12*f2wm2*tau11*
     & a12*tau21+tau23*tau12*f2wm1*tau11*tau21*a12-1.*f2wm1*tau21*
     & tau12**2*tau23*a11+6.*tau23*tau12*f2vm2*tau21*a13*tau11-1.*
     & tau23*f2um1*tau12**2*tau21*a13+6.*tau13*f2vm2*tau21*tau23*a11*
     & tau12-2.*tau13*tau23*f2vm1*tau11*tau21*a12-6.*f2wm2*tau12**2*
     & tau21*tau23*a11-6.*tau13*f2vm2*tau21**2*a13*tau12+tau13*f2vm1*
     & tau21*tau23*tau12*a11-1.*tau23**2*f2um1*tau12*a12*tau11+tau13*
     & tau23*f2um1*tau12*tau21*a12-12.*tau13*f2vm2*tau21*tau23*a12*
     & tau11-6.*tau13*f2wm2*tau12*tau21**2*a12-1.*tau22**2*tau13*
     & f2um1*a13*tau11+tau22*f2wm1*tau11*tau23*tau12*a11-6.*tau22*
     & f2wm2*tau11**2*tau23*a12-6.*tau22*tau13**2*f2um2*a12*tau21-1.*
     & tau22*tau13**2*f2um1*tau21*a12-6.*tau22*tau13**2*f2vm2*tau21*
     & a11-1.*tau22*tau13**2*f2vm1*tau21*a11-1.*tau22*f2wm1*tau11**2*
     & a12*tau23-1.*tau22*tau23*f2vm1*tau11**2*a13-12.*tau22*tau13*
     & f2um2*tau23*a11*tau12-1.*tau13*f2vm1*tau21**2*tau12*a13-2.*
     & tau22*f2wm1*tau21*tau12*a13*tau11+tau22*tau23*f2um1*tau12*a13*
     & tau11-6.*f2um2*tau23**2*a12*tau11*tau12-6.*f2um2*a13*tau21*
     & tau12**2*tau23-6.*f2vm2*tau23**2*tau11*a11*tau12+6.*tau13*
     & f2um2*a12*tau21*tau23*tau12+6.*tau22*tau13*f2um2*a13*tau21*
     & tau12+6.*tau22*tau13*f2wm2*tau12*tau21*a11+tau13**2*f2vm1*
     & tau21**2*a12+tau22*tau13*f2vm1*tau21*a13*tau11+tau22*tau13*
     & f2wm1*tau11*tau21*a12+6.*tau13**2*f2vm2*tau21**2*a12-6.*tau22**
     & 2*tau13*f2um2*a13*tau11+tau22*tau13*f2wm1*tau21*tau12*a11+6.*
     & tau22*tau13*f2vm2*tau21*a13*tau11-2.*tau22*tau13*f2um1*a11*
     & tau23*tau12+tau22*tau13*f2um1*a12*tau11*tau23-1.*tau22**2*
     & tau13*f2wm1*tau11*a11+tau22*tau13*f2um1*tau21*tau12*a13-1.*
     & tau23**2*f2vm1*tau11*tau12*a11+6.*tau22*f2wm2*tau11*tau23*a11*
     & tau12-1.*tau13*f2wm1*tau21**2*tau12*a12-6.*tau22*f2vm2*tau23*
     & tau11**2*a13-6.*tau22**2*tau13*f2wm2*tau11*a11+tau23**2*f2vm1*
     & tau11**2*a12+6.*f2vm2*tau23**2*tau11**2*a12+6.*f2um2*tau23**2*
     & a11*tau12**2+6.*tau22*f2um2*a13*tau11*tau23*tau12-12.*tau22*
     & f2wm2*tau12*tau21*a13*tau11+6.*tau22*tau13*f2wm2*tau11*a12*
     & tau21+tau22*tau13*tau23*f2vm1*tau11*a11+6.*tau22*tau13*f2vm2*
     & tau23*tau11*a11+6.*tau22*tau13*f2um2*tau23*a12*tau11+tau22**2*
     & tau13**2*f2um1*a11+6.*f2wm2*tau12**2*tau21**2*a13+tau23**2*
     & f2um1*tau12**2*a11+f2wm1*tau21**2*tau12**2*a13+6.*tau22**2*
     & f2wm2*tau11**2*a13+tau22**2*f2wm1*tau11**2*a13+6.*tau22**2*
     & tau13**2*f2um2*a11)

      u(i1-is1,i2-is2,i3-is3,ex) = -1.*(-1.*tau21*tau13*a12*tau22*
     & f2wm1*ttu11+tau21*tau13*a12*tau23*tau12*f2f+6.*tau13*tau22**2*
     & f2um2*a13*ttu11+2.*tau13*f1f*tau22*f2um2*tau23*tau12+tau13**2*
     & f1f*tau22*f2vm2*tau21-1.*tau13*tau23*tau12*f2vm2*ttu22*a11-1.*
     & f1f*tau23*tau12*f2wm2*tau11*tau22+f1f*tau23**2*tau12*f2vm2*
     & tau11-1.*f1f*tau23*tau12*tau13*f2vm2*tau21+f1f*tau23*tau12**2*
     & f2wm2*tau21-1.*tau13*f1f*tau22*f2wm2*tau12*tau21+tau13*f1f*
     & tau22**2*f2wm2*tau11-1.*f1f*tau23**2*tau12**2*f2um2-1.*tau13**
     & 2*f1f*tau22**2*f2um2-1.*tau13*f1f*tau22*f2vm2*tau23*tau11+
     & tau23**2*a11*tau12**2*f2f+tau13*a11*tau22**2*f2wm2*ttu12-1.*
     & a12*tau23**2*tau12*f2f*tau11-1.*a12*tau23*tau12*f2wm2*tau11*
     & ttu22-1.*a12*tau23*tau12*f2wm1*ttu21*tau11+tau23**2*a11*ttu11*
     & tau12*f2vm1+tau23**2*tau12*f2vm2*ttu12*a11+tau23*tau12**2*
     & f2wm2*ttu22*a11+a13*tau12*tau23*tau11*tau22*f2f+a11*ttu21*
     & tau23*tau12**2*f2wm1-1.*tau23*a11*tau12*tau22*f2wm1*ttu11-1.*
     & tau23*a11*tau12*tau22*f2wm2*ttu12-1.*tau13*tau22*f2vm2*ttu22*
     & a13*tau11+tau13*a12*tau23*tau11*tau22*f2f-1.*tau13*tau22*f2vm2*
     & tau23*ttu12*a11-1.*tau13*tau22*f2wm2*tau12*ttu22*a11-1.*tau13*
     & tau23*a11*ttu11*tau22*f2vm1-1.*tau13*a11*ttu21*tau23*tau12*
     & f2vm1-1.*tau13*a13*ttu21*tau11*tau22*f2vm1+tau13*a11*tau22**2*
     & f2wm1*ttu11-1.*tau13*a11*ttu21*tau22*f2wm1*tau12-1.*tau13*a13*
     & tau22**2*tau11*f2f+tau13**2*tau22*f2vm2*ttu22*a11+tau13**2*a11*
     & ttu21*tau22*f2vm1+tau13**2*a11*tau22**2*f2f-2.*tau13*tau22*f2f*
     & tau23*tau12*a11+tau22*f2wm1*ttu21*tau11*tau12*a13+tau23*ttu12*
     & tau22*f2um2*a13*tau12+6.*tau22*f2vm2*tau23*tau11*a13*ttu11-6.*
     & tau22*f2um2*a13*ttu11*tau23*tau12+6.*tau22*f2wm2*tau12*a13*
     & ttu21*tau11+tau23*tau11*a13*ttu11*tau22*f2vm1+6.*tau23*ttu11*
     & tau22*f2wm2*tau11*a12-6.*tau13*tau23*ttu11*tau22*f2um2*a12-6.*
     & tau13*ttu21*f2um2*a12*tau23*tau12+tau13*ttu21*tau23*f2vm1*
     & tau11*a12+6.*tau13*ttu21*f2vm2*tau23*a12*tau11-6.*tau13*ttu21*
     & tau22*f2wm2*tau11*a12+tau13*tau23*ttu12*tau22*f2um2*a12-6.*
     & tau13*ttu21*tau22*f2um2*a13*tau12+tau13*ttu22*tau22*f2um2*a13*
     & tau12+tau13*ttu22*f2um2*a12*tau23*tau12-1.*tau13**2*ttu22*
     & tau22*f2um2*a12+tau13*ttu22*tau22*f2wm2*tau11*a12+tau23*tau11*
     & ttu22*f2vm2*a13*tau12-6.*tau23*tau12*f2vm2*a13*ttu21*tau11+
     & tau23*tau11*a12*tau22*f2wm1*ttu11-6.*tau21*tau13**2*ttu21*
     & f2vm2*a12-1.*tau21*ttu21*f2wm1*tau12**2*a13-6.*tau21*ttu21*
     & f2wm2*tau12**2*a13-1.*tau21*tau13**2*ttu21*f2vm1*a12+tau21*
     & tau13*ttu21*f2vm1*tau12*a13+tau21*tau13*tau22*f2vm2*a13*ttu12+
     & 6.*tau21*tau13*tau23*ttu11*f2vm2*a12+6.*tau21*tau13*ttu21*
     & f2vm2*a13*tau12+6.*tau21*tau13*ttu21*f2wm2*tau12*a12-6.*tau21*
     & tau23*ttu11*f2wm2*tau12*a12+tau21*tau13*tau23*ttu11*f2vm1*a12-
     & 6.*tau21*tau13*tau22*f2vm2*a13*ttu11+6.*tau21*tau22*f2wm2*
     & tau12*a13*ttu11+tau21*tau22*f2wm1*ttu11*tau12*a13+tau21*tau23*
     & ttu12*f2wm2*tau12*a12+tau21*tau13*ttu21*f2wm1*tau12*a12-6.*
     & tau22**2*f2wm2*tau11*a13*ttu11-1.*tau21*tau13**2*a12*tau22*f2f-
     & 1.*tau21*tau13*a12*tau22*f2wm2*ttu12+tau21*tau13*a13*tau12*
     & tau22*f2f-1.*tau22**2*f2wm1*ttu11*a13*tau11-1.*tau23**2*ttu12*
     & f2um2*a12*tau12-1.*ttu22*f2um2*a13*tau12**2*tau23+6.*ttu21*
     & f2um2*a13*tau12**2*tau23-1.*tau21*tau23*tau12**2*f2f*a13-6.*
     & tau23**2*ttu11*f2vm2*a12*tau11+6.*tau23**2*ttu11*f2um2*a12*
     & tau12-1.*tau13*tau22**2*f2um2*a13*ttu12-1.*tau23**2*ttu11*
     & f2vm1*tau11*a12-1.*tau21*a13*ttu11*tau23*tau12*f2vm1-1.*tau21*
     & tau23*tau12*f2vm2*a13*ttu12+6.*tau13**2*ttu21*tau22*f2um2*a12)
     & /(tau23*tau12*f2vm1*tau21*a13*tau11+6.*tau23*tau12*f2wm2*tau11*
     & a12*tau21+tau23*tau12*f2wm1*tau11*tau21*a12-1.*f2wm1*tau21*
     & tau12**2*tau23*a11+6.*tau23*tau12*f2vm2*tau21*a13*tau11-1.*
     & tau23*f2um1*tau12**2*tau21*a13+6.*tau13*f2vm2*tau21*tau23*a11*
     & tau12-2.*tau13*tau23*f2vm1*tau11*tau21*a12-6.*f2wm2*tau12**2*
     & tau21*tau23*a11-6.*tau13*f2vm2*tau21**2*a13*tau12+tau13*f2vm1*
     & tau21*tau23*tau12*a11-1.*tau23**2*f2um1*tau12*a12*tau11+tau13*
     & tau23*f2um1*tau12*tau21*a12-12.*tau13*f2vm2*tau21*tau23*a12*
     & tau11-6.*tau13*f2wm2*tau12*tau21**2*a12-1.*tau22**2*tau13*
     & f2um1*a13*tau11+tau22*f2wm1*tau11*tau23*tau12*a11-6.*tau22*
     & f2wm2*tau11**2*tau23*a12-6.*tau22*tau13**2*f2um2*a12*tau21-1.*
     & tau22*tau13**2*f2um1*tau21*a12-6.*tau22*tau13**2*f2vm2*tau21*
     & a11-1.*tau22*tau13**2*f2vm1*tau21*a11-1.*tau22*f2wm1*tau11**2*
     & a12*tau23-1.*tau22*tau23*f2vm1*tau11**2*a13-12.*tau22*tau13*
     & f2um2*tau23*a11*tau12-1.*tau13*f2vm1*tau21**2*tau12*a13-2.*
     & tau22*f2wm1*tau21*tau12*a13*tau11+tau22*tau23*f2um1*tau12*a13*
     & tau11-6.*f2um2*tau23**2*a12*tau11*tau12-6.*f2um2*a13*tau21*
     & tau12**2*tau23-6.*f2vm2*tau23**2*tau11*a11*tau12+6.*tau13*
     & f2um2*a12*tau21*tau23*tau12+6.*tau22*tau13*f2um2*a13*tau21*
     & tau12+6.*tau22*tau13*f2wm2*tau12*tau21*a11+tau13**2*f2vm1*
     & tau21**2*a12+tau22*tau13*f2vm1*tau21*a13*tau11+tau22*tau13*
     & f2wm1*tau11*tau21*a12+6.*tau13**2*f2vm2*tau21**2*a12-6.*tau22**
     & 2*tau13*f2um2*a13*tau11+tau22*tau13*f2wm1*tau21*tau12*a11+6.*
     & tau22*tau13*f2vm2*tau21*a13*tau11-2.*tau22*tau13*f2um1*a11*
     & tau23*tau12+tau22*tau13*f2um1*a12*tau11*tau23-1.*tau22**2*
     & tau13*f2wm1*tau11*a11+tau22*tau13*f2um1*tau21*tau12*a13-1.*
     & tau23**2*f2vm1*tau11*tau12*a11+6.*tau22*f2wm2*tau11*tau23*a11*
     & tau12-1.*tau13*f2wm1*tau21**2*tau12*a12-6.*tau22*f2vm2*tau23*
     & tau11**2*a13-6.*tau22**2*tau13*f2wm2*tau11*a11+tau23**2*f2vm1*
     & tau11**2*a12+6.*f2vm2*tau23**2*tau11**2*a12+6.*f2um2*tau23**2*
     & a11*tau12**2+6.*tau22*f2um2*a13*tau11*tau23*tau12-12.*tau22*
     & f2wm2*tau12*tau21*a13*tau11+6.*tau22*tau13*f2wm2*tau11*a12*
     & tau21+tau22*tau13*tau23*f2vm1*tau11*a11+6.*tau22*tau13*f2vm2*
     & tau23*tau11*a11+6.*tau22*tau13*f2um2*tau23*a12*tau11+tau22**2*
     & tau13**2*f2um1*a11+6.*f2wm2*tau12**2*tau21**2*a13+tau23**2*
     & f2um1*tau12**2*a11+f2wm1*tau21**2*tau12**2*a13+6.*tau22**2*
     & f2wm2*tau11**2*a13+tau22**2*f2wm1*tau11**2*a13+6.*tau22**2*
     & tau13**2*f2um2*a11)

      u(i1-2*is1,i2-2*is2,i3-2*is3,ex) = (-1.*a12*tau23**2*ttu12*f2um1*
     & tau12-1.*tau21*a13*ttu12*tau22*f2wm1*tau12-6.*tau21*tau13*a12*
     & tau23*ttu12*f2vm2+6.*tau21*tau13*a12*tau22*f2wm1*ttu11-6.*
     & tau21*tau13*ttu22*f2vm2*a13*tau12-6.*tau21*tau13*a12*tau23*
     & tau12*f2f-6.*tau21*tau13*a12*ttu22*f2wm2*tau12-1.*tau21*tau13*
     & a12*tau23*ttu12*f2vm1-6.*tau21*a12*tau23*tau12*f2wm1*ttu11+
     & tau21*ttu22*f2wm1*tau12**2*a13-1.*tau13**2*a12*ttu22*tau22*
     & f2um1+6.*tau13*tau23*tau12*f2vm2*ttu22*a11+a13*ttu12*tau22**2*
     & f2wm1*tau11-6.*tau23**2*a11*tau12**2*f2f+2.*tau13*f2um1*tau23*
     & tau12*f1f*tau22-1.*tau13*f2vm1*tau23*tau11*f1f*tau22-6.*tau13*
     & a11*tau22**2*f2wm2*ttu12+6.*a12*tau23**2*tau12*f2f*tau11+6.*
     & a12*tau23*tau12*f2wm2*tau11*ttu22-1.*a12*tau23*ttu12*tau22*
     & f2wm1*tau11-6.*a12*tau23*tau11*tau22*f2wm2*ttu12+6.*a12*tau23*
     & tau12*f2wm1*ttu21*tau11+6.*a12*tau11*tau23**2*ttu12*f2vm2+6.*
     & a12*tau23**2*tau12*f2um1*ttu11+a12*tau11*tau23**2*ttu12*f2vm1-
     & 6.*tau23**2*a11*ttu11*tau12*f2vm1-1.*ttu22*tau23*f2um1*tau12**
     & 2*a13+ttu22*tau23*tau12*f2vm1*a13*tau11+f2vm1*tau23**2*tau11*
     & f1f*tau12-1.*f2um1*tau23**2*tau12**2*f1f-1.*tau13**2*f2um1*
     & tau22**2*f1f-6.*tau23**2*tau12*f2vm2*ttu12*a11+6.*tau23*tau12**
     & 2*f2um1*ttu21*a13-1.*f2wm1*tau12*f1f*tau22*tau23*tau11-6.*
     & tau23*tau12**2*f2wm2*ttu22*a11-6.*a13*tau12*tau23*tau11*tau22*
     & f2f-6.*a11*ttu21*tau23*tau12**2*f2wm1+a13*tau12*tau23*ttu12*
     & tau22*f2um1-6.*tau23*tau11*tau22*f2vm2*a13*ttu12-6.*a13*tau12*
     & tau22*tau23*f2um1*ttu11+6.*tau23*a11*tau12*tau22*f2wm1*ttu11+
     & 6.*tau23*a11*tau12*tau22*f2wm2*ttu12-1.*ttu22*tau22*f2wm1*
     & tau12*a13*tau11-6.*a13*tau12*tau22*f2wm2*tau11*ttu22-1.*tau23*
     & ttu12*tau22*f2vm1*a13*tau11+tau13*a12*tau23*ttu12*tau22*f2um1-
     & 6.*tau13*a12*tau22*tau23*f2um1*ttu11-6.*tau13*a12*tau23*tau12*
     & f2um1*ttu21-1.*tau13*a12*ttu22*tau23*f2vm1*tau11+tau13*a12*
     & ttu22*tau23*f2um1*tau12-6.*tau13*a12*tau22*f2wm1*ttu21*tau11+
     & 6.*tau13*tau22*f2vm2*ttu22*a13*tau11-6.*tau13*a12*ttu22*f2vm2*
     & tau23*tau11+tau13*a12*ttu22*tau22*f2wm1*tau11-6.*tau13*a12*
     & tau23*tau11*tau22*f2f+6.*tau13*tau22*f2vm2*tau23*ttu12*a11-6.*
     & tau13*a13*tau12*tau22*f2um1*ttu21+6.*tau13*tau22*f2wm2*tau12*
     & ttu22*a11+6.*tau13*tau23*a11*ttu11*tau22*f2vm1+tau13*ttu22*
     & tau22*f2um1*tau12*a13+6.*tau13*a11*ttu21*tau23*tau12*f2vm1+6.*
     & tau13*a13*ttu21*tau11*tau22*f2vm1-6.*tau13*a11*tau22**2*f2wm1*
     & ttu11+6.*tau13*a11*ttu21*tau22*f2wm1*tau12+6.*tau13*a13*tau22**
     & 2*tau11*f2f+6.*tau13*a13*ttu11*tau22**2*f2um1-1.*tau13*a13*
     & ttu12*tau22**2*f2um1-6.*a13*ttu21*tau11*tau23*tau12*f2vm1-6.*
     & tau13**2*tau22*f2vm2*ttu22*a11+6.*tau13**2*a12*tau22*f2um1*
     & ttu21-6.*tau13**2*a11*ttu21*tau22*f2vm1-6.*tau13**2*a11*tau22**
     & 2*f2f+tau13*f2wm1*tau11*tau22**2*f1f+tau21*f2wm1*tau12**2*f1f*
     & tau23+12.*tau13*tau22*f2f*tau23*tau12*a11+tau21*tau13**2*a12*
     & ttu22*f2vm1+6.*tau21*tau13**2*a12*tau22*f2f+tau21*a12*tau23*
     & ttu12*f2wm1*tau12-1.*tau21*tau13*a12*ttu22*f2wm1*tau12-1.*
     & tau21*tau13*ttu22*f2vm1*tau12*a13+tau21*tau13*a13*ttu12*tau22*
     & f2vm1-6.*tau21*tau13*a13*ttu11*tau22*f2vm1+6.*tau21*tau13*a12*
     & tau22*f2wm2*ttu12-6.*tau21*tau13*a13*tau12*tau22*f2f+6.*tau21*
     & tau13**2*a12*ttu22*f2vm2+6.*tau21*tau23*tau12**2*f2f*a13+6.*
     & tau21*ttu22*f2wm2*tau12**2*a13+6.*tau21*a13*ttu11*tau23*tau12*
     & f2vm1-6.*tau21*a13*tau12*tau22*f2wm2*ttu12+6.*tau21*tau23*
     & tau12*f2vm2*a13*ttu12-1.*tau21*tau13*f2wm1*tau12*f1f*tau22-1.*
     & tau21*tau13*f2vm1*f1f*tau23*tau12+tau21*tau13**2*f2vm1*f1f*
     & tau22+6.*a13*tau22**2*tau11*f2wm2*ttu12)/(tau23*tau12*f2vm1*
     & tau21*a13*tau11+6.*tau23*tau12*f2wm2*tau11*a12*tau21+tau23*
     & tau12*f2wm1*tau11*tau21*a12-1.*f2wm1*tau21*tau12**2*tau23*a11+
     & 6.*tau23*tau12*f2vm2*tau21*a13*tau11-1.*tau23*f2um1*tau12**2*
     & tau21*a13+6.*tau13*f2vm2*tau21*tau23*a11*tau12-2.*tau13*tau23*
     & f2vm1*tau11*tau21*a12-6.*f2wm2*tau12**2*tau21*tau23*a11-6.*
     & tau13*f2vm2*tau21**2*a13*tau12+tau13*f2vm1*tau21*tau23*tau12*
     & a11-1.*tau23**2*f2um1*tau12*a12*tau11+tau13*tau23*f2um1*tau12*
     & tau21*a12-12.*tau13*f2vm2*tau21*tau23*a12*tau11-6.*tau13*f2wm2*
     & tau12*tau21**2*a12-1.*tau22**2*tau13*f2um1*a13*tau11+tau22*
     & f2wm1*tau11*tau23*tau12*a11-6.*tau22*f2wm2*tau11**2*tau23*a12-
     & 6.*tau22*tau13**2*f2um2*a12*tau21-1.*tau22*tau13**2*f2um1*
     & tau21*a12-6.*tau22*tau13**2*f2vm2*tau21*a11-1.*tau22*tau13**2*
     & f2vm1*tau21*a11-1.*tau22*f2wm1*tau11**2*a12*tau23-1.*tau22*
     & tau23*f2vm1*tau11**2*a13-12.*tau22*tau13*f2um2*tau23*a11*tau12-
     & 1.*tau13*f2vm1*tau21**2*tau12*a13-2.*tau22*f2wm1*tau21*tau12*
     & a13*tau11+tau22*tau23*f2um1*tau12*a13*tau11-6.*f2um2*tau23**2*
     & a12*tau11*tau12-6.*f2um2*a13*tau21*tau12**2*tau23-6.*f2vm2*
     & tau23**2*tau11*a11*tau12+6.*tau13*f2um2*a12*tau21*tau23*tau12+
     & 6.*tau22*tau13*f2um2*a13*tau21*tau12+6.*tau22*tau13*f2wm2*
     & tau12*tau21*a11+tau13**2*f2vm1*tau21**2*a12+tau22*tau13*f2vm1*
     & tau21*a13*tau11+tau22*tau13*f2wm1*tau11*tau21*a12+6.*tau13**2*
     & f2vm2*tau21**2*a12-6.*tau22**2*tau13*f2um2*a13*tau11+tau22*
     & tau13*f2wm1*tau21*tau12*a11+6.*tau22*tau13*f2vm2*tau21*a13*
     & tau11-2.*tau22*tau13*f2um1*a11*tau23*tau12+tau22*tau13*f2um1*
     & a12*tau11*tau23-1.*tau22**2*tau13*f2wm1*tau11*a11+tau22*tau13*
     & f2um1*tau21*tau12*a13-1.*tau23**2*f2vm1*tau11*tau12*a11+6.*
     & tau22*f2wm2*tau11*tau23*a11*tau12-1.*tau13*f2wm1*tau21**2*
     & tau12*a12-6.*tau22*f2vm2*tau23*tau11**2*a13-6.*tau22**2*tau13*
     & f2wm2*tau11*a11+tau23**2*f2vm1*tau11**2*a12+6.*f2vm2*tau23**2*
     & tau11**2*a12+6.*f2um2*tau23**2*a11*tau12**2+6.*tau22*f2um2*a13*
     & tau11*tau23*tau12-12.*tau22*f2wm2*tau12*tau21*a13*tau11+6.*
     & tau22*tau13*f2wm2*tau11*a12*tau21+tau22*tau13*tau23*f2vm1*
     & tau11*a11+6.*tau22*tau13*f2vm2*tau23*tau11*a11+6.*tau22*tau13*
     & f2um2*tau23*a12*tau11+tau22**2*tau13**2*f2um1*a11+6.*f2wm2*
     & tau12**2*tau21**2*a13+tau23**2*f2um1*tau12**2*a11+f2wm1*tau21**
     & 2*tau12**2*a13+6.*tau22**2*f2wm2*tau11**2*a13+tau22**2*f2wm1*
     & tau11**2*a13+6.*tau22**2*tau13**2*f2um2*a11)

      u(i1-is1,i2-is2,i3-is3,ey) = (-1.*tau23**2*tau11*f1f*f2um2*tau12+
     & tau23*tau12*tau21*f1f*f2wm2*tau11+tau13**2*tau21**2*f1f*f2vm2+
     & tau23**2*tau11**2*f1f*f2vm2-1.*tau13*tau21**2*f1f*f2wm2*tau12+
     & tau13*tau21*f1f*f2um2*tau23*tau12-1.*tau22*tau23*tau11**2*f1f*
     & f2wm2-2.*tau13*tau21*f1f*f2vm2*tau23*tau11-1.*tau22*tau13**2*
     & tau21*f1f*f2um2+tau22*tau13*tau21*f1f*f2wm2*tau11-6.*tau13**2*
     & f2vm2*tau21*a11*ttu21-1.*tau13**2*f2um2*a12*tau21*ttu22+tau13*
     & f2wm2*tau11*a12*tau21*ttu22+tau13*f2um2*a13*tau12*tau21*ttu22-
     & 1.*tau13*tau21*f2f*tau23*tau12*a11+tau13*tau23*f2um1*ttu11*
     & tau21*a12-1.*tau13*f2vm2*tau21*ttu22*a13*tau11+tau13*f2um1*
     & ttu21*a12*tau11*tau23+tau13*f2um1*ttu21*tau21*tau12*a13-1.*
     & tau13*f2um1*ttu21*tau23*tau12*a11+tau13*tau21**2*f2f*tau12*a13-
     & 1.*tau13*tau21**2*f2wm2*ttu12*a12+6.*tau13*f2vm2*tau21*a13*
     & ttu21*tau11-1.*tau13*f2vm2*tau21*tau23*ttu12*a11-1.*tau13*
     & f2wm2*tau12*tau21*ttu22*a11-1.*tau13*f2vm2*tau23*tau11*ttu22*
     & a11+tau13*f2wm1*ttu21*tau11*tau21*a12+tau13*f2vm2*tau21**2*a13*
     & ttu12+tau22*tau13*tau23*tau11*f1f*f2um2-1.*tau23*tau12*tau21*
     & f2f*a13*tau11-6.*tau13*f2vm2*tau21**2*a13*ttu11+2.*tau13*tau21*
     & f2f*a12*tau11*tau23+6.*tau13*f2vm2*tau23*tau11*a11*ttu21+tau13*
     & f2um2*a12*tau21*tau23*ttu12+tau13*f2um2*a12*tau11*tau23*ttu22+
     & 6.*tau13*f2wm2*tau12*tau21*a11*ttu21-6.*tau13*f2um2*a11*ttu21*
     & tau23*tau12+6.*tau13*f2vm2*tau21*tau23*a11*ttu11-1.*tau13**2*
     & f2um1*ttu21*tau21*a12+tau13**2*f2vm2*tau21*ttu22*a11+6.*f2vm2*
     & tau23*tau11*a13*tau21*ttu11+tau23*tau11*f2wm1*tau12*ttu21*a11-
     & 1.*f2vm2*tau21*tau23*ttu12*a13*tau11-1.*f2wm2*tau11**2*a12*
     & tau23*ttu22+f2vm2*tau23*tau11**2*ttu22*a13+tau23**2*f2um1*
     & ttu11*tau12*a11-6.*f2wm2*tau12*tau21*a13*ttu21*tau11-1.*f2wm1*
     & ttu21*tau11*tau21*tau12*a13-1.*f2wm1*ttu21*tau11**2*a12*tau23-
     & 6.*f2vm2*tau23*tau11**2*a13*ttu21-1.*f2um2*tau23*a13*tau12*
     & tau11*ttu22-1.*tau21*f2wm1*ttu11*a11*tau23*tau12+f2vm2*tau23**
     & 2*tau11*ttu12*a11+tau21*f2wm2*ttu12*a12*tau11*tau23+tau23*
     & tau11*f2wm2*tau12*ttu22*a11+6.*f2um2*a13*ttu21*tau11*tau23*
     & tau12-1.*tau23*f2um1*ttu11*tau21*tau12*a13-1.*tau23**2*f2um1*
     & ttu11*a12*tau11+tau23**2*tau11*f2f*tau12*a11+tau21**2*f2wm1*
     & ttu11*tau12*a13+tau21*f2wm1*ttu11*a12*tau11*tau23-1.*tau23**2*
     & tau11**2*f2f*a12-1.*tau13**2*tau21**2*f2f*a12-1.*tau13*tau21**
     & 2*f2wm1*ttu11*a12+6.*f2um2*tau23**2*a11*ttu11*tau12+6.*f2wm2*
     & tau12*tau21**2*a13*ttu11-6.*f2wm2*tau12*tau21*tau23*a11*ttu11-
     & 1.*f2um2*a12*tau11*tau23**2*ttu12-6.*f2um2*a13*tau21*ttu11*
     & tau23*tau12-6.*f2vm2*tau23**2*tau11*a11*ttu11-1.*tau22*tau13*
     & tau21*f2f*a13*tau11-1.*tau22*tau13*tau23*f2um1*ttu11*a11+tau22*
     & tau13*tau21*f2wm2*ttu12*a11-6.*tau22*tau13*f2wm2*tau11*a11*
     & ttu21-1.*tau22*tau23*f2wm2*ttu12*tau11*a11-6.*tau22*tau13*
     & f2um2*a13*ttu21*tau11+6.*tau22*f2wm2*tau11**2*a13*ttu21+tau22*
     & f2wm1*ttu21*tau11**2*a13+tau22*tau23*tau11**2*f2f*a13+tau22*
     & tau13**2*f2um1*ttu21*a11+tau22*tau13**2*tau21*f2f*a11+6.*tau22*
     & tau13**2*f2um2*a11*ttu21+6.*tau22*f2wm2*tau11*tau23*a11*ttu11-
     & 1.*tau22*tau13*tau23*tau11*f2f*a11-1.*tau22*tau13*f2wm1*ttu21*
     & tau11*a11-1.*tau22*tau13*f2um1*ttu21*a13*tau11-1.*tau22*tau13*
     & f2um2*a13*ttu12*tau21+tau22*tau13*tau21*f2wm1*ttu11*a11-6.*
     & tau22*f2wm2*tau11*a13*tau21*ttu11+tau22*tau23*f2um1*ttu11*a13*
     & tau11+tau22*f2um2*tau23*a13*ttu12*tau11+6.*tau22*tau13*f2um2*
     & a13*tau21*ttu11-1.*tau22*tau21*f2wm1*ttu11*a13*tau11-6.*tau22*
     & tau13*f2um2*tau23*a11*ttu11)/(tau23*tau12*f2vm1*tau21*a13*
     & tau11+6.*tau23*tau12*f2wm2*tau11*a12*tau21+tau23*tau12*f2wm1*
     & tau11*tau21*a12-1.*f2wm1*tau21*tau12**2*tau23*a11+6.*tau23*
     & tau12*f2vm2*tau21*a13*tau11-1.*tau23*f2um1*tau12**2*tau21*a13+
     & 6.*tau13*f2vm2*tau21*tau23*a11*tau12-2.*tau13*tau23*f2vm1*
     & tau11*tau21*a12-6.*f2wm2*tau12**2*tau21*tau23*a11-6.*tau13*
     & f2vm2*tau21**2*a13*tau12+tau13*f2vm1*tau21*tau23*tau12*a11-1.*
     & tau23**2*f2um1*tau12*a12*tau11+tau13*tau23*f2um1*tau12*tau21*
     & a12-12.*tau13*f2vm2*tau21*tau23*a12*tau11-6.*tau13*f2wm2*tau12*
     & tau21**2*a12-1.*tau22**2*tau13*f2um1*a13*tau11+tau22*f2wm1*
     & tau11*tau23*tau12*a11-6.*tau22*f2wm2*tau11**2*tau23*a12-6.*
     & tau22*tau13**2*f2um2*a12*tau21-1.*tau22*tau13**2*f2um1*tau21*
     & a12-6.*tau22*tau13**2*f2vm2*tau21*a11-1.*tau22*tau13**2*f2vm1*
     & tau21*a11-1.*tau22*f2wm1*tau11**2*a12*tau23-1.*tau22*tau23*
     & f2vm1*tau11**2*a13-12.*tau22*tau13*f2um2*tau23*a11*tau12-1.*
     & tau13*f2vm1*tau21**2*tau12*a13-2.*tau22*f2wm1*tau21*tau12*a13*
     & tau11+tau22*tau23*f2um1*tau12*a13*tau11-6.*f2um2*tau23**2*a12*
     & tau11*tau12-6.*f2um2*a13*tau21*tau12**2*tau23-6.*f2vm2*tau23**
     & 2*tau11*a11*tau12+6.*tau13*f2um2*a12*tau21*tau23*tau12+6.*
     & tau22*tau13*f2um2*a13*tau21*tau12+6.*tau22*tau13*f2wm2*tau12*
     & tau21*a11+tau13**2*f2vm1*tau21**2*a12+tau22*tau13*f2vm1*tau21*
     & a13*tau11+tau22*tau13*f2wm1*tau11*tau21*a12+6.*tau13**2*f2vm2*
     & tau21**2*a12-6.*tau22**2*tau13*f2um2*a13*tau11+tau22*tau13*
     & f2wm1*tau21*tau12*a11+6.*tau22*tau13*f2vm2*tau21*a13*tau11-2.*
     & tau22*tau13*f2um1*a11*tau23*tau12+tau22*tau13*f2um1*a12*tau11*
     & tau23-1.*tau22**2*tau13*f2wm1*tau11*a11+tau22*tau13*f2um1*
     & tau21*tau12*a13-1.*tau23**2*f2vm1*tau11*tau12*a11+6.*tau22*
     & f2wm2*tau11*tau23*a11*tau12-1.*tau13*f2wm1*tau21**2*tau12*a12-
     & 6.*tau22*f2vm2*tau23*tau11**2*a13-6.*tau22**2*tau13*f2wm2*
     & tau11*a11+tau23**2*f2vm1*tau11**2*a12+6.*f2vm2*tau23**2*tau11**
     & 2*a12+6.*f2um2*tau23**2*a11*tau12**2+6.*tau22*f2um2*a13*tau11*
     & tau23*tau12-12.*tau22*f2wm2*tau12*tau21*a13*tau11+6.*tau22*
     & tau13*f2wm2*tau11*a12*tau21+tau22*tau13*tau23*f2vm1*tau11*a11+
     & 6.*tau22*tau13*f2vm2*tau23*tau11*a11+6.*tau22*tau13*f2um2*
     & tau23*a12*tau11+tau22**2*tau13**2*f2um1*a11+6.*f2wm2*tau12**2*
     & tau21**2*a13+tau23**2*f2um1*tau12**2*a11+f2wm1*tau21**2*tau12**
     & 2*a13+6.*tau22**2*f2wm2*tau11**2*a13+tau22**2*f2wm1*tau11**2*
     & a13+6.*tau22**2*tau13**2*f2um2*a11)

      u(i1-2*is1,i2-2*is2,i3-2*is3,ey) = -1.*(-1.*tau13*tau21**2*f2wm1*
     & tau12*f1f+tau13**2*tau21**2*f2vm1*f1f+tau23*tau12*tau21*f2wm1*
     & tau11*f1f-1.*tau23**2*tau11*f2um1*tau12*f1f-1.*tau22*tau23*
     & tau11**2*f2wm1*f1f-2.*tau13*tau21*f2vm1*tau23*tau11*f1f+tau13*
     & tau21*f2um1*tau23*tau12*f1f-1.*tau22*tau13**2*tau21*f2um1*f1f+
     & tau22*tau13*tau21*f2wm1*tau11*f1f+tau22*tau13*tau23*tau11*
     & f2um1*f1f+6.*tau22*tau13*tau21*a13*ttu11*f2um1+tau22*tau13*
     & tau23*ttu12*f2um1*a11+tau22*tau21*a13*ttu12*f2wm1*tau11+6.*
     & tau22*tau13*ttu22*f2wm2*tau11*a11+6.*tau22*tau23*tau11*a11*
     & f2wm1*ttu11-1.*tau22*tau13**2*ttu22*f2um1*a11-6.*tau22*tau13**
     & 2*ttu22*f2um2*a11-1.*tau22*ttu22*f2wm1*tau11**2*a13-6.*tau22*
     & ttu22*f2wm2*tau11**2*a13+6.*tau22*tau21*a13*tau11*f2wm2*ttu12-
     & 1.*tau22*tau23*ttu12*f2wm1*tau11*a11+tau22*tau13*ttu22*f2wm1*
     & tau11*a11+tau22*tau13*ttu22*f2um1*a13*tau11+6.*tau22*tau13*
     & tau23*ttu12*f2um2*a11+6.*tau22*tau13*ttu22*f2um2*a13*tau11-1.*
     & tau22*tau13*tau21*a13*ttu12*f2um1+tau13*tau21**2*a13*ttu12*
     & f2vm1-6.*tau13*tau21**2*a13*ttu11*f2vm1+6.*tau13*tau21*a13*
     & ttu21*tau11*f2vm1+6.*tau13*tau21*a11*ttu21*f2wm1*tau12-1.*
     & tau13*tau23*ttu12*f2vm1*tau21*a11-1.*tau13*ttu22*tau23*f2vm1*
     & tau11*a11+6.*tau13*ttu22*f2um2*tau23*a11*tau12+6.*tau13*tau21*
     & tau23*a11*ttu11*f2vm1-1.*tau13*ttu22*f2vm1*tau21*a13*tau11-1.*
     & tau13*ttu22*f2wm1*tau21*tau12*a11+tau13*ttu22*f2um1*a11*tau23*
     & tau12+6.*tau13*tau23*tau11*a11*ttu21*f2vm1-6.*tau13**2*tau21*
     & a11*ttu21*f2vm1+tau13**2*ttu22*f2vm1*tau21*a11-6.*tau21**2*a13*
     & tau12*f2wm2*ttu12+6.*tau23*tau11*a13*tau12*f2um1*ttu21-1.*
     & tau23*tau11*ttu22*f2um1*tau12*a13-6.*tau23*tau11**2*a13*ttu21*
     & f2vm1-6.*tau23**2*ttu12*f2um2*a11*tau12+tau21*ttu22*f2wm1*
     & tau12*a13*tau11+6.*tau21*a13*tau12*f2wm2*tau11*ttu22+ttu22*
     & tau23*f2vm1*tau11**2*a13-1.*tau23**2*ttu12*f2um1*a11*tau12-1.*
     & tau21**2*a13*ttu12*f2wm1*tau12+6.*tau21*tau23*a11*tau12*f2wm2*
     & ttu12+tau23*ttu12*f2wm1*tau21*tau12*a11+tau23**2*ttu12*f2vm1*
     & tau11*a11+6.*tau23*ttu12*f2um2*a13*tau21*tau12+6.*tau23*tau11*
     & tau21*a13*ttu11*f2vm1-1.*tau21*tau23*ttu12*f2vm1*a13*tau11-6.*
     & tau23**2*tau11*a11*ttu11*f2vm1+tau21*a13*tau12*tau23*ttu12*
     & f2um1+tau23**2*tau11**2*f2vm1*f1f+6.*tau13**2*f2um2*a12*tau21*
     & ttu22-6.*tau13*f2wm2*tau11*a12*tau21*ttu22-6.*tau13*f2um2*a13*
     & tau12*tau21*ttu22+6.*tau13*tau21*f2f*tau23*tau12*a11-6.*tau13*
     & tau23*f2um1*ttu11*tau21*a12-6.*tau13*f2um1*ttu21*a12*tau11*
     & tau23-6.*tau13*f2um1*ttu21*tau21*tau12*a13-6.*tau13*tau21**2*
     & f2f*tau12*a13+6.*tau13*tau21**2*f2wm2*ttu12*a12-6.*tau13*f2wm1*
     & ttu21*tau11*tau21*a12+6.*tau23*tau12*tau21*f2f*a13*tau11-12.*
     & tau13*tau21*f2f*a12*tau11*tau23-6.*tau13*f2um2*a12*tau21*tau23*
     & ttu12-6.*tau13*f2um2*a12*tau11*tau23*ttu22+6.*tau13**2*f2um1*
     & ttu21*tau21*a12-6.*tau23*tau11*f2wm1*tau12*ttu21*a11+6.*f2wm2*
     & tau11**2*a12*tau23*ttu22+6.*f2wm1*ttu21*tau11**2*a12*tau23-6.*
     & tau21*f2wm2*ttu12*a12*tau11*tau23-6.*tau23*tau11*f2wm2*tau12*
     & ttu22*a11+6.*tau23**2*f2um1*ttu11*a12*tau11-6.*tau23**2*tau11*
     & f2f*tau12*a11-6.*tau21*f2wm1*ttu11*a12*tau11*tau23+6.*tau23**2*
     & tau11**2*f2f*a12+6.*tau13**2*tau21**2*f2f*a12+6.*tau13*tau21**
     & 2*f2wm1*ttu11*a12+6.*f2um2*a12*tau11*tau23**2*ttu12+6.*tau22*
     & tau13*tau21*f2f*a13*tau11-6.*tau22*tau13*tau21*f2wm2*ttu12*a11-
     & 6.*tau22*tau23*tau11**2*f2f*a13-6.*tau22*tau13**2*tau21*f2f*
     & a11+6.*tau22*tau13*tau23*tau11*f2f*a11-6.*tau22*tau13*tau21*
     & f2wm1*ttu11*a11-6.*tau22*tau23*f2um1*ttu11*a13*tau11-6.*tau22*
     & f2um2*tau23*a13*ttu12*tau11)/(tau23*tau12*f2vm1*tau21*a13*
     & tau11+6.*tau23*tau12*f2wm2*tau11*a12*tau21+tau23*tau12*f2wm1*
     & tau11*tau21*a12-1.*f2wm1*tau21*tau12**2*tau23*a11+6.*tau23*
     & tau12*f2vm2*tau21*a13*tau11-1.*tau23*f2um1*tau12**2*tau21*a13+
     & 6.*tau13*f2vm2*tau21*tau23*a11*tau12-2.*tau13*tau23*f2vm1*
     & tau11*tau21*a12-6.*f2wm2*tau12**2*tau21*tau23*a11-6.*tau13*
     & f2vm2*tau21**2*a13*tau12+tau13*f2vm1*tau21*tau23*tau12*a11-1.*
     & tau23**2*f2um1*tau12*a12*tau11+tau13*tau23*f2um1*tau12*tau21*
     & a12-12.*tau13*f2vm2*tau21*tau23*a12*tau11-6.*tau13*f2wm2*tau12*
     & tau21**2*a12-1.*tau22**2*tau13*f2um1*a13*tau11+tau22*f2wm1*
     & tau11*tau23*tau12*a11-6.*tau22*f2wm2*tau11**2*tau23*a12-6.*
     & tau22*tau13**2*f2um2*a12*tau21-1.*tau22*tau13**2*f2um1*tau21*
     & a12-6.*tau22*tau13**2*f2vm2*tau21*a11-1.*tau22*tau13**2*f2vm1*
     & tau21*a11-1.*tau22*f2wm1*tau11**2*a12*tau23-1.*tau22*tau23*
     & f2vm1*tau11**2*a13-12.*tau22*tau13*f2um2*tau23*a11*tau12-1.*
     & tau13*f2vm1*tau21**2*tau12*a13-2.*tau22*f2wm1*tau21*tau12*a13*
     & tau11+tau22*tau23*f2um1*tau12*a13*tau11-6.*f2um2*tau23**2*a12*
     & tau11*tau12-6.*f2um2*a13*tau21*tau12**2*tau23-6.*f2vm2*tau23**
     & 2*tau11*a11*tau12+6.*tau13*f2um2*a12*tau21*tau23*tau12+6.*
     & tau22*tau13*f2um2*a13*tau21*tau12+6.*tau22*tau13*f2wm2*tau12*
     & tau21*a11+tau13**2*f2vm1*tau21**2*a12+tau22*tau13*f2vm1*tau21*
     & a13*tau11+tau22*tau13*f2wm1*tau11*tau21*a12+6.*tau13**2*f2vm2*
     & tau21**2*a12-6.*tau22**2*tau13*f2um2*a13*tau11+tau22*tau13*
     & f2wm1*tau21*tau12*a11+6.*tau22*tau13*f2vm2*tau21*a13*tau11-2.*
     & tau22*tau13*f2um1*a11*tau23*tau12+tau22*tau13*f2um1*a12*tau11*
     & tau23-1.*tau22**2*tau13*f2wm1*tau11*a11+tau22*tau13*f2um1*
     & tau21*tau12*a13-1.*tau23**2*f2vm1*tau11*tau12*a11+6.*tau22*
     & f2wm2*tau11*tau23*a11*tau12-1.*tau13*f2wm1*tau21**2*tau12*a12-
     & 6.*tau22*f2vm2*tau23*tau11**2*a13-6.*tau22**2*tau13*f2wm2*
     & tau11*a11+tau23**2*f2vm1*tau11**2*a12+6.*f2vm2*tau23**2*tau11**
     & 2*a12+6.*f2um2*tau23**2*a11*tau12**2+6.*tau22*f2um2*a13*tau11*
     & tau23*tau12-12.*tau22*f2wm2*tau12*tau21*a13*tau11+6.*tau22*
     & tau13*f2wm2*tau11*a12*tau21+tau22*tau13*tau23*f2vm1*tau11*a11+
     & 6.*tau22*tau13*f2vm2*tau23*tau11*a11+6.*tau22*tau13*f2um2*
     & tau23*a12*tau11+tau22**2*tau13**2*f2um1*a11+6.*f2wm2*tau12**2*
     & tau21**2*a13+tau23**2*f2um1*tau12**2*a11+f2wm1*tau21**2*tau12**
     & 2*a13+6.*tau22**2*f2wm2*tau11**2*a13+tau22**2*f2wm1*tau11**2*
     & a13+6.*tau22**2*tau13**2*f2um2*a11)


 ! *********** done *********************
             !  if( debug.gt.0 )then
             !
             !   write(*,'(" bc4:extrap: i1,i2,i3=",3i3," u(-1)=",3f8.2," u(-2)=",3f8.2)') i1,i2,i3,!          u(i1-is1,i2-is2,i3-is3,ex),u(i1-is1,i2-is2,i3-is3,ey),u(i1-is1,i2-is2,i3-is3,ez),!          u(i1-2*is1,i2-2*is2,i3-2*is3,ex),u(i1-2*is1,i2-2*is2,i3-2*is3,ey),u(i1-2*is1,i2-2*is2,i3-2*is3,ez)
             !  end if
               ! set to exact for testing
               ! OGF3D(i1-is1,i2-is2,i3-is3,t, uvm(0),uvm(1),uvm(2))
               ! OGF3D(i1-2*is1,i2-2*is2,i3-2*is3,t, uvm2(0),uvm2(1),uvm2(2))
               ! u(i1-is1,i2-is2,i3-is3,ex)=uvm(0)
               ! u(i1-is1,i2-is2,i3-is3,ey)=uvm(1)
               ! u(i1-is1,i2-is2,i3-is3,ez)=uvm(2)
               ! u(i1-2*is1,i2-2*is2,i3-2*is3,ex)=uvm2(0)
               ! u(i1-2*is1,i2-2*is2,i3-2*is3,ey)=uvm2(1)
               ! u(i1-2*is1,i2-2*is2,i3-2*is3,ez)=uvm2(2)
             else if( mask(i1,i2,i3).lt.0 )then
              ! ** NEW WAY **  *wdh
              ! extrapolate ghost points next to boundary interpolation points  *wdh* 2015/08/11
              if( .false. .and. t.le.dt )then
                write(*,'("--MX-- BC4 extrap ghost next to interp t,
     & dt=",2e12.3)') t,dt
              end if
               u(i1-is1,i2-is2,i3-is3,ex) = (5.*u(i1,i2,i3,ex)-10.*u(
     & i1+is1,i2+is2,i3+is3,ex)+10.*u(i1+2*is1,i2+2*is2,i3+2*is3,ex)-
     & 5.*u(i1+3*is1,i2+3*is2,i3+3*is3,ex)+u(i1+4*is1,i2+4*is2,i3+4*
     & is3,ex))
               u(i1-is1,i2-is2,i3-is3,ey) = (5.*u(i1,i2,i3,ey)-10.*u(
     & i1+is1,i2+is2,i3+is3,ey)+10.*u(i1+2*is1,i2+2*is2,i3+2*is3,ey)-
     & 5.*u(i1+3*is1,i2+3*is2,i3+3*is3,ey)+u(i1+4*is1,i2+4*is2,i3+4*
     & is3,ey))
               u(i1-is1,i2-is2,i3-is3,ez) = (5.*u(i1,i2,i3,ez)-10.*u(
     & i1+is1,i2+is2,i3+is3,ez)+10.*u(i1+2*is1,i2+2*is2,i3+2*is3,ez)-
     & 5.*u(i1+3*is1,i2+3*is2,i3+3*is3,ez)+u(i1+4*is1,i2+4*is2,i3+4*
     & is3,ez))
               u(i1-2*is1,i2-2*is2,i3-2*is3,ex) = (5.*u(i1-is1,i2-is2,
     & i3-is3,ex)-10.*u(i1-is1+is1,i2-is2+is2,i3-is3+is3,ex)+10.*u(i1-
     & is1+2*is1,i2-is2+2*is2,i3-is3+2*is3,ex)-5.*u(i1-is1+3*is1,i2-
     & is2+3*is2,i3-is3+3*is3,ex)+u(i1-is1+4*is1,i2-is2+4*is2,i3-is3+
     & 4*is3,ex))
               u(i1-2*is1,i2-2*is2,i3-2*is3,ey) = (5.*u(i1-is1,i2-is2,
     & i3-is3,ey)-10.*u(i1-is1+is1,i2-is2+is2,i3-is3+is3,ey)+10.*u(i1-
     & is1+2*is1,i2-is2+2*is2,i3-is3+2*is3,ey)-5.*u(i1-is1+3*is1,i2-
     & is2+3*is2,i3-is3+3*is3,ey)+u(i1-is1+4*is1,i2-is2+4*is2,i3-is3+
     & 4*is3,ey))
               u(i1-2*is1,i2-2*is2,i3-2*is3,ez) = (5.*u(i1-is1,i2-is2,
     & i3-is3,ez)-10.*u(i1-is1+is1,i2-is2+is2,i3-is3+is3,ez)+10.*u(i1-
     & is1+2*is1,i2-is2+2*is2,i3-is3+2*is3,ez)-5.*u(i1-is1+3*is1,i2-
     & is2+3*is2,i3-is3+3*is3,ez)+u(i1-is1+4*is1,i2-is2+4*is2,i3-is3+
     & 4*is3,ez))
             else if( .FALSE. .and. mask(i1,i2,i3).lt.0 )then
               ! **OLD WAY**
              ! QUESTION: August 8, 2015 -- is this accurate enough ??
              ! we need to assign ghost points that lie outside of interpolation points
              if( debug.gt.0 )then
                write(*,'(" **ghost-interp3d: grid,side,axis=",3i2,", 
     & i1,i2,i3=",3i4)') grid,side,axis,i1,i2,i3
              end if
              tau11=rsxy(i1,i2,i3,axisp1,0)
              tau12=rsxy(i1,i2,i3,axisp1,1)
              tau13=rsxy(i1,i2,i3,axisp1,2)
              tau21=rsxy(i1,i2,i3,axisp2,0)
              tau22=rsxy(i1,i2,i3,axisp2,1)
              tau23=rsxy(i1,i2,i3,axisp2,2)
              uex=u(i1,i2,i3,ex)
              uey=u(i1,i2,i3,ey)
              uez=u(i1,i2,i3,ez)
              a11 =(rsxy(i1,i2,i3,axis,0)/(rx(i1,i2,i3)*(sy(i1,i2,i3)*
     & tz(i1,i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(sz(i1,i2,
     & i3)*tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,i3)*(sx(
     & i1,i2,i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3))))
              a12 =(rsxy(i1,i2,i3,axis,1)/(rx(i1,i2,i3)*(sy(i1,i2,i3)*
     & tz(i1,i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(sz(i1,i2,
     & i3)*tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,i3)*(sx(
     & i1,i2,i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3))))
              a13 =(rsxy(i1,i2,i3,axis,2)/(rx(i1,i2,i3)*(sy(i1,i2,i3)*
     & tz(i1,i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(sz(i1,i2,
     & i3)*tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,i3)*(sx(
     & i1,i2,i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3))))
              a21 =(rsxy(i1,i2,i3,axisp1,0)/(rx(i1,i2,i3)*(sy(i1,i2,i3)
     & *tz(i1,i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(sz(i1,
     & i2,i3)*tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,i3)*(
     & sx(i1,i2,i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3))))
              a22 =(rsxy(i1,i2,i3,axisp1,1)/(rx(i1,i2,i3)*(sy(i1,i2,i3)
     & *tz(i1,i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(sz(i1,
     & i2,i3)*tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,i3)*(
     & sx(i1,i2,i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3))))
              a23 =(rsxy(i1,i2,i3,axisp1,2)/(rx(i1,i2,i3)*(sy(i1,i2,i3)
     & *tz(i1,i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(sz(i1,
     & i2,i3)*tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,i3)*(
     & sx(i1,i2,i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3))))
              a31 =(rsxy(i1,i2,i3,axisp2,0)/(rx(i1,i2,i3)*(sy(i1,i2,i3)
     & *tz(i1,i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(sz(i1,
     & i2,i3)*tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,i3)*(
     & sx(i1,i2,i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3))))
              a32 =(rsxy(i1,i2,i3,axisp2,1)/(rx(i1,i2,i3)*(sy(i1,i2,i3)
     & *tz(i1,i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(sz(i1,
     & i2,i3)*tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,i3)*(
     & sx(i1,i2,i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3))))
              a33 =(rsxy(i1,i2,i3,axisp2,2)/(rx(i1,i2,i3)*(sy(i1,i2,i3)
     & *tz(i1,i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(sz(i1,
     & i2,i3)*tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,i3)*(
     & sx(i1,i2,i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3))))
              a11m1 =(rsxy(i1-is1,i2-is2,i3-is3,axis,0)/(rx(i1-is1,i2-
     & is2,i3-is3)*(sy(i1-is1,i2-is2,i3-is3)*tz(i1-is1,i2-is2,i3-is3)-
     & sz(i1-is1,i2-is2,i3-is3)*ty(i1-is1,i2-is2,i3-is3))+ry(i1-is1,
     & i2-is2,i3-is3)*(sz(i1-is1,i2-is2,i3-is3)*tx(i1-is1,i2-is2,i3-
     & is3)-sx(i1-is1,i2-is2,i3-is3)*tz(i1-is1,i2-is2,i3-is3))+rz(i1-
     & is1,i2-is2,i3-is3)*(sx(i1-is1,i2-is2,i3-is3)*ty(i1-is1,i2-is2,
     & i3-is3)-sy(i1-is1,i2-is2,i3-is3)*tx(i1-is1,i2-is2,i3-is3))))
              a12m1 =(rsxy(i1-is1,i2-is2,i3-is3,axis,1)/(rx(i1-is1,i2-
     & is2,i3-is3)*(sy(i1-is1,i2-is2,i3-is3)*tz(i1-is1,i2-is2,i3-is3)-
     & sz(i1-is1,i2-is2,i3-is3)*ty(i1-is1,i2-is2,i3-is3))+ry(i1-is1,
     & i2-is2,i3-is3)*(sz(i1-is1,i2-is2,i3-is3)*tx(i1-is1,i2-is2,i3-
     & is3)-sx(i1-is1,i2-is2,i3-is3)*tz(i1-is1,i2-is2,i3-is3))+rz(i1-
     & is1,i2-is2,i3-is3)*(sx(i1-is1,i2-is2,i3-is3)*ty(i1-is1,i2-is2,
     & i3-is3)-sy(i1-is1,i2-is2,i3-is3)*tx(i1-is1,i2-is2,i3-is3))))
              a13m1 =(rsxy(i1-is1,i2-is2,i3-is3,axis,2)/(rx(i1-is1,i2-
     & is2,i3-is3)*(sy(i1-is1,i2-is2,i3-is3)*tz(i1-is1,i2-is2,i3-is3)-
     & sz(i1-is1,i2-is2,i3-is3)*ty(i1-is1,i2-is2,i3-is3))+ry(i1-is1,
     & i2-is2,i3-is3)*(sz(i1-is1,i2-is2,i3-is3)*tx(i1-is1,i2-is2,i3-
     & is3)-sx(i1-is1,i2-is2,i3-is3)*tz(i1-is1,i2-is2,i3-is3))+rz(i1-
     & is1,i2-is2,i3-is3)*(sx(i1-is1,i2-is2,i3-is3)*ty(i1-is1,i2-is2,
     & i3-is3)-sy(i1-is1,i2-is2,i3-is3)*tx(i1-is1,i2-is2,i3-is3))))
              a11p1 =(rsxy(i1+is1,i2+is2,i3+is3,axis,0)/(rx(i1+is1,i2+
     & is2,i3+is3)*(sy(i1+is1,i2+is2,i3+is3)*tz(i1+is1,i2+is2,i3+is3)-
     & sz(i1+is1,i2+is2,i3+is3)*ty(i1+is1,i2+is2,i3+is3))+ry(i1+is1,
     & i2+is2,i3+is3)*(sz(i1+is1,i2+is2,i3+is3)*tx(i1+is1,i2+is2,i3+
     & is3)-sx(i1+is1,i2+is2,i3+is3)*tz(i1+is1,i2+is2,i3+is3))+rz(i1+
     & is1,i2+is2,i3+is3)*(sx(i1+is1,i2+is2,i3+is3)*ty(i1+is1,i2+is2,
     & i3+is3)-sy(i1+is1,i2+is2,i3+is3)*tx(i1+is1,i2+is2,i3+is3))))
              a12p1 =(rsxy(i1+is1,i2+is2,i3+is3,axis,1)/(rx(i1+is1,i2+
     & is2,i3+is3)*(sy(i1+is1,i2+is2,i3+is3)*tz(i1+is1,i2+is2,i3+is3)-
     & sz(i1+is1,i2+is2,i3+is3)*ty(i1+is1,i2+is2,i3+is3))+ry(i1+is1,
     & i2+is2,i3+is3)*(sz(i1+is1,i2+is2,i3+is3)*tx(i1+is1,i2+is2,i3+
     & is3)-sx(i1+is1,i2+is2,i3+is3)*tz(i1+is1,i2+is2,i3+is3))+rz(i1+
     & is1,i2+is2,i3+is3)*(sx(i1+is1,i2+is2,i3+is3)*ty(i1+is1,i2+is2,
     & i3+is3)-sy(i1+is1,i2+is2,i3+is3)*tx(i1+is1,i2+is2,i3+is3))))
              a13p1 =(rsxy(i1+is1,i2+is2,i3+is3,axis,2)/(rx(i1+is1,i2+
     & is2,i3+is3)*(sy(i1+is1,i2+is2,i3+is3)*tz(i1+is1,i2+is2,i3+is3)-
     & sz(i1+is1,i2+is2,i3+is3)*ty(i1+is1,i2+is2,i3+is3))+ry(i1+is1,
     & i2+is2,i3+is3)*(sz(i1+is1,i2+is2,i3+is3)*tx(i1+is1,i2+is2,i3+
     & is3)-sx(i1+is1,i2+is2,i3+is3)*tz(i1+is1,i2+is2,i3+is3))+rz(i1+
     & is1,i2+is2,i3+is3)*(sx(i1+is1,i2+is2,i3+is3)*ty(i1+is1,i2+is2,
     & i3+is3)-sy(i1+is1,i2+is2,i3+is3)*tx(i1+is1,i2+is2,i3+is3))))
              a11m2 =(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,0)/(rx(i1-2*
     & is1,i2-2*is2,i3-2*is3)*(sy(i1-2*is1,i2-2*is2,i3-2*is3)*tz(i1-2*
     & is1,i2-2*is2,i3-2*is3)-sz(i1-2*is1,i2-2*is2,i3-2*is3)*ty(i1-2*
     & is1,i2-2*is2,i3-2*is3))+ry(i1-2*is1,i2-2*is2,i3-2*is3)*(sz(i1-
     & 2*is1,i2-2*is2,i3-2*is3)*tx(i1-2*is1,i2-2*is2,i3-2*is3)-sx(i1-
     & 2*is1,i2-2*is2,i3-2*is3)*tz(i1-2*is1,i2-2*is2,i3-2*is3))+rz(i1-
     & 2*is1,i2-2*is2,i3-2*is3)*(sx(i1-2*is1,i2-2*is2,i3-2*is3)*ty(i1-
     & 2*is1,i2-2*is2,i3-2*is3)-sy(i1-2*is1,i2-2*is2,i3-2*is3)*tx(i1-
     & 2*is1,i2-2*is2,i3-2*is3))))
              a12m2 =(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,1)/(rx(i1-2*
     & is1,i2-2*is2,i3-2*is3)*(sy(i1-2*is1,i2-2*is2,i3-2*is3)*tz(i1-2*
     & is1,i2-2*is2,i3-2*is3)-sz(i1-2*is1,i2-2*is2,i3-2*is3)*ty(i1-2*
     & is1,i2-2*is2,i3-2*is3))+ry(i1-2*is1,i2-2*is2,i3-2*is3)*(sz(i1-
     & 2*is1,i2-2*is2,i3-2*is3)*tx(i1-2*is1,i2-2*is2,i3-2*is3)-sx(i1-
     & 2*is1,i2-2*is2,i3-2*is3)*tz(i1-2*is1,i2-2*is2,i3-2*is3))+rz(i1-
     & 2*is1,i2-2*is2,i3-2*is3)*(sx(i1-2*is1,i2-2*is2,i3-2*is3)*ty(i1-
     & 2*is1,i2-2*is2,i3-2*is3)-sy(i1-2*is1,i2-2*is2,i3-2*is3)*tx(i1-
     & 2*is1,i2-2*is2,i3-2*is3))))
              a13m2 =(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,2)/(rx(i1-2*
     & is1,i2-2*is2,i3-2*is3)*(sy(i1-2*is1,i2-2*is2,i3-2*is3)*tz(i1-2*
     & is1,i2-2*is2,i3-2*is3)-sz(i1-2*is1,i2-2*is2,i3-2*is3)*ty(i1-2*
     & is1,i2-2*is2,i3-2*is3))+ry(i1-2*is1,i2-2*is2,i3-2*is3)*(sz(i1-
     & 2*is1,i2-2*is2,i3-2*is3)*tx(i1-2*is1,i2-2*is2,i3-2*is3)-sx(i1-
     & 2*is1,i2-2*is2,i3-2*is3)*tz(i1-2*is1,i2-2*is2,i3-2*is3))+rz(i1-
     & 2*is1,i2-2*is2,i3-2*is3)*(sx(i1-2*is1,i2-2*is2,i3-2*is3)*ty(i1-
     & 2*is1,i2-2*is2,i3-2*is3)-sy(i1-2*is1,i2-2*is2,i3-2*is3)*tx(i1-
     & 2*is1,i2-2*is2,i3-2*is3))))
              a11p2 =(rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axis,0)/(rx(i1+2*
     & is1,i2+2*is2,i3+2*is3)*(sy(i1+2*is1,i2+2*is2,i3+2*is3)*tz(i1+2*
     & is1,i2+2*is2,i3+2*is3)-sz(i1+2*is1,i2+2*is2,i3+2*is3)*ty(i1+2*
     & is1,i2+2*is2,i3+2*is3))+ry(i1+2*is1,i2+2*is2,i3+2*is3)*(sz(i1+
     & 2*is1,i2+2*is2,i3+2*is3)*tx(i1+2*is1,i2+2*is2,i3+2*is3)-sx(i1+
     & 2*is1,i2+2*is2,i3+2*is3)*tz(i1+2*is1,i2+2*is2,i3+2*is3))+rz(i1+
     & 2*is1,i2+2*is2,i3+2*is3)*(sx(i1+2*is1,i2+2*is2,i3+2*is3)*ty(i1+
     & 2*is1,i2+2*is2,i3+2*is3)-sy(i1+2*is1,i2+2*is2,i3+2*is3)*tx(i1+
     & 2*is1,i2+2*is2,i3+2*is3))))
              a12p2 =(rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axis,1)/(rx(i1+2*
     & is1,i2+2*is2,i3+2*is3)*(sy(i1+2*is1,i2+2*is2,i3+2*is3)*tz(i1+2*
     & is1,i2+2*is2,i3+2*is3)-sz(i1+2*is1,i2+2*is2,i3+2*is3)*ty(i1+2*
     & is1,i2+2*is2,i3+2*is3))+ry(i1+2*is1,i2+2*is2,i3+2*is3)*(sz(i1+
     & 2*is1,i2+2*is2,i3+2*is3)*tx(i1+2*is1,i2+2*is2,i3+2*is3)-sx(i1+
     & 2*is1,i2+2*is2,i3+2*is3)*tz(i1+2*is1,i2+2*is2,i3+2*is3))+rz(i1+
     & 2*is1,i2+2*is2,i3+2*is3)*(sx(i1+2*is1,i2+2*is2,i3+2*is3)*ty(i1+
     & 2*is1,i2+2*is2,i3+2*is3)-sy(i1+2*is1,i2+2*is2,i3+2*is3)*tx(i1+
     & 2*is1,i2+2*is2,i3+2*is3))))
              a13p2 =(rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axis,2)/(rx(i1+2*
     & is1,i2+2*is2,i3+2*is3)*(sy(i1+2*is1,i2+2*is2,i3+2*is3)*tz(i1+2*
     & is1,i2+2*is2,i3+2*is3)-sz(i1+2*is1,i2+2*is2,i3+2*is3)*ty(i1+2*
     & is1,i2+2*is2,i3+2*is3))+ry(i1+2*is1,i2+2*is2,i3+2*is3)*(sz(i1+
     & 2*is1,i2+2*is2,i3+2*is3)*tx(i1+2*is1,i2+2*is2,i3+2*is3)-sx(i1+
     & 2*is1,i2+2*is2,i3+2*is3)*tz(i1+2*is1,i2+2*is2,i3+2*is3))+rz(i1+
     & 2*is1,i2+2*is2,i3+2*is3)*(sx(i1+2*is1,i2+2*is2,i3+2*is3)*ty(i1+
     & 2*is1,i2+2*is2,i3+2*is3)-sy(i1+2*is1,i2+2*is2,i3+2*is3)*tx(i1+
     & 2*is1,i2+2*is2,i3+2*is3))))
              c11 = (rsxy(i1,i2,i3,axis,0)**2+rsxy(i1,i2,i3,axis,1)**2+
     & rsxy(i1,i2,i3,axis,2)**2)
              c22 = (rsxy(i1,i2,i3,axisp1,0)**2+rsxy(i1,i2,i3,axisp1,1)
     & **2+rsxy(i1,i2,i3,axisp1,2)**2)
              c33 = (rsxy(i1,i2,i3,axisp2,0)**2+rsxy(i1,i2,i3,axisp2,1)
     & **2+rsxy(i1,i2,i3,axisp2,2)**2)
              ! ***************************************************************************************
              ! Use one sided approximations as needed for expressions needing tangential derivatives
              ! ***************************************************************************************
              js1a=abs(js1)
              js2a=abs(js2)
              js3a=abs(js3)
              ks1a=abs(ks1)
              ks2a=abs(ks2)
              ks3a=abs(ks3)
              ! *** first do metric derivatives -- no need to worry about the mask value ****
              if( (i1-2*js1a).ge.md1a .and. (i1+2*js1a).le.md1b .and. (
     & i2-2*js2a).ge.md2a .and. (i2+2*js2a).le.md2b .and. (i3-2*js3a)
     & .ge.md3a .and. (i3+2*js3a).le.md3b .and. (i1-2*ks1a).ge.md1a 
     & .and. (i1+2*ks1a).le.md1b .and. (i2-2*ks2a).ge.md2a .and. (i2+
     & 2*ks2a).le.md2b .and. (i3-2*ks3a).ge.md3a .and. (i3+2*ks3a)
     & .le.md3b )then
                ! centered approximation is ok
                c1 = (rsxyx43(i1,i2,i3,axis,0)+rsxyy43(i1,i2,i3,axis,1)
     & +rsxyz43(i1,i2,i3,axis,2))
                c2 = (rsxyx43(i1,i2,i3,axisp1,0)+rsxyy43(i1,i2,i3,
     & axisp1,1)+rsxyz43(i1,i2,i3,axisp1,2))
                c3 = (rsxyx43(i1,i2,i3,axisp2,0)+rsxyy43(i1,i2,i3,
     & axisp2,1)+rsxyz43(i1,i2,i3,axisp2,2))
              else if( (i1-js1a).ge.md1a .and. (i1+js1a).le.md1b .and. 
     & (i2-js2a).ge.md2a .and. (i2+js2a).le.md2b .and. (i3-js3a)
     & .ge.md3a .and. (i3+js3a).le.md3b .and. (i1-ks1a).ge.md1a .and. 
     & (i1+ks1a).le.md1b .and. (i2-ks2a).ge.md2a .and. (i2+ks2a)
     & .le.md2b .and. (i3-ks3a).ge.md3a .and. (i3+ks3a).le.md3b )then
                ! use 2nd-order centered approximation
                c1 = (rsxyx23(i1,i2,i3,axis,0)+rsxyy23(i1,i2,i3,axis,1)
     & +rsxyz23(i1,i2,i3,axis,2))
                c2 = (rsxyx23(i1,i2,i3,axisp1,0)+rsxyy23(i1,i2,i3,
     & axisp1,1)+rsxyz23(i1,i2,i3,axisp1,2))
                c3 = (rsxyx23(i1,i2,i3,axisp2,0)+rsxyy23(i1,i2,i3,
     & axisp2,1)+rsxyz23(i1,i2,i3,axisp2,2))
              else if( (i1-3*js1a).ge.md1a .and. (i2-3*js2a).ge.md2a 
     & .and. (i3-3*js3a).ge.md3a )then
               ! one sided  2nd-order:
               c1 = 2.*(rsxyx23(i1-js1a,i2-js2a,i3-js3a,axis,0)+
     & rsxyy23(i1-js1a,i2-js2a,i3-js3a,axis,1)+rsxyz23(i1-js1a,i2-
     & js2a,i3-js3a,axis,2))-(rsxyx23(i1-2*js1a,i2-2*js2a,i3-2*js3a,
     & axis,0)+rsxyy23(i1-2*js1a,i2-2*js2a,i3-2*js3a,axis,1)+rsxyz23(
     & i1-2*js1a,i2-2*js2a,i3-2*js3a,axis,2))
               c2 = 2.*(rsxyx23(i1-js1a,i2-js2a,i3-js3a,axisp1,0)+
     & rsxyy23(i1-js1a,i2-js2a,i3-js3a,axisp1,1)+rsxyz23(i1-js1a,i2-
     & js2a,i3-js3a,axisp1,2))-(rsxyx23(i1-2*js1a,i2-2*js2a,i3-2*js3a,
     & axisp1,0)+rsxyy23(i1-2*js1a,i2-2*js2a,i3-2*js3a,axisp1,1)+
     & rsxyz23(i1-2*js1a,i2-2*js2a,i3-2*js3a,axisp1,2))
               c3 = 2.*(rsxyx23(i1-js1a,i2-js2a,i3-js3a,axisp2,0)+
     & rsxyy23(i1-js1a,i2-js2a,i3-js3a,axisp2,1)+rsxyz23(i1-js1a,i2-
     & js2a,i3-js3a,axisp2,2))-(rsxyx23(i1-2*js1a,i2-2*js2a,i3-2*js3a,
     & axisp2,0)+rsxyy23(i1-2*js1a,i2-2*js2a,i3-2*js3a,axisp2,1)+
     & rsxyz23(i1-2*js1a,i2-2*js2a,i3-2*js3a,axisp2,2))
              else if( (i1+3*js1a).le.md1b .and. (i2+3*js2a).le.md2b 
     & .and. (i3+3*js3a).le.md3b )then
               ! one sided  2nd-order:
               c1 = 2.*(rsxyx23(i1+js1a,i2+js2a,i3+js3a,axis,0)+
     & rsxyy23(i1+js1a,i2+js2a,i3+js3a,axis,1)+rsxyz23(i1+js1a,i2+
     & js2a,i3+js3a,axis,2))-(rsxyx23(i1+2*js1a,i2+2*js2a,i3+2*js3a,
     & axis,0)+rsxyy23(i1+2*js1a,i2+2*js2a,i3+2*js3a,axis,1)+rsxyz23(
     & i1+2*js1a,i2+2*js2a,i3+2*js3a,axis,2))
               c2 = 2.*(rsxyx23(i1+js1a,i2+js2a,i3+js3a,axisp1,0)+
     & rsxyy23(i1+js1a,i2+js2a,i3+js3a,axisp1,1)+rsxyz23(i1+js1a,i2+
     & js2a,i3+js3a,axisp1,2))-(rsxyx23(i1+2*js1a,i2+2*js2a,i3+2*js3a,
     & axisp1,0)+rsxyy23(i1+2*js1a,i2+2*js2a,i3+2*js3a,axisp1,1)+
     & rsxyz23(i1+2*js1a,i2+2*js2a,i3+2*js3a,axisp1,2))
               c3 = 2.*(rsxyx23(i1+js1a,i2+js2a,i3+js3a,axisp2,0)+
     & rsxyy23(i1+js1a,i2+js2a,i3+js3a,axisp2,1)+rsxyz23(i1+js1a,i2+
     & js2a,i3+js3a,axisp2,2))-(rsxyx23(i1+2*js1a,i2+2*js2a,i3+2*js3a,
     & axisp2,0)+rsxyy23(i1+2*js1a,i2+2*js2a,i3+2*js3a,axisp2,1)+
     & rsxyz23(i1+2*js1a,i2+2*js2a,i3+2*js3a,axisp2,2))
              else if( (i1-3*ks1a).ge.md1a .and. (i2-3*ks2a).ge.md2a 
     & .and. (i3-3*ks3a).ge.md3a )then
               ! one sided  2nd-order:  -- this case should not be needed?
               c1 = 2.*(rsxyx23(i1-ks1a,i2-ks2a,i3-ks3a,axis,0)+
     & rsxyy23(i1-ks1a,i2-ks2a,i3-ks3a,axis,1)+rsxyz23(i1-ks1a,i2-
     & ks2a,i3-ks3a,axis,2))-(rsxyx23(i1-2*ks1a,i2-2*ks2a,i3-2*ks3a,
     & axis,0)+rsxyy23(i1-2*ks1a,i2-2*ks2a,i3-2*ks3a,axis,1)+rsxyz23(
     & i1-2*ks1a,i2-2*ks2a,i3-2*ks3a,axis,2))
               c2 = 2.*(rsxyx23(i1-ks1a,i2-ks2a,i3-ks3a,axisp1,0)+
     & rsxyy23(i1-ks1a,i2-ks2a,i3-ks3a,axisp1,1)+rsxyz23(i1-ks1a,i2-
     & ks2a,i3-ks3a,axisp1,2))-(rsxyx23(i1-2*ks1a,i2-2*ks2a,i3-2*ks3a,
     & axisp1,0)+rsxyy23(i1-2*ks1a,i2-2*ks2a,i3-2*ks3a,axisp1,1)+
     & rsxyz23(i1-2*ks1a,i2-2*ks2a,i3-2*ks3a,axisp1,2))
               c3 = 2.*(rsxyx23(i1-ks1a,i2-ks2a,i3-ks3a,axisp2,0)+
     & rsxyy23(i1-ks1a,i2-ks2a,i3-ks3a,axisp2,1)+rsxyz23(i1-ks1a,i2-
     & ks2a,i3-ks3a,axisp2,2))-(rsxyx23(i1-2*ks1a,i2-2*ks2a,i3-2*ks3a,
     & axisp2,0)+rsxyy23(i1-2*ks1a,i2-2*ks2a,i3-2*ks3a,axisp2,1)+
     & rsxyz23(i1-2*ks1a,i2-2*ks2a,i3-2*ks3a,axisp2,2))
              else if( (i1+3*ks1a).le.md1b .and. (i2+3*ks2a).le.md2b 
     & .and. (i3+3*ks3a).le.md3b )then
               ! one sided  2nd-order: -- this case should not be needed?
               c1 = 2.*(rsxyx23(i1+ks1a,i2+ks2a,i3+ks3a,axis,0)+
     & rsxyy23(i1+ks1a,i2+ks2a,i3+ks3a,axis,1)+rsxyz23(i1+ks1a,i2+
     & ks2a,i3+ks3a,axis,2))-(rsxyx23(i1+2*ks1a,i2+2*ks2a,i3+2*ks3a,
     & axis,0)+rsxyy23(i1+2*ks1a,i2+2*ks2a,i3+2*ks3a,axis,1)+rsxyz23(
     & i1+2*ks1a,i2+2*ks2a,i3+2*ks3a,axis,2))
               c2 = 2.*(rsxyx23(i1+ks1a,i2+ks2a,i3+ks3a,axisp1,0)+
     & rsxyy23(i1+ks1a,i2+ks2a,i3+ks3a,axisp1,1)+rsxyz23(i1+ks1a,i2+
     & ks2a,i3+ks3a,axisp1,2))-(rsxyx23(i1+2*ks1a,i2+2*ks2a,i3+2*ks3a,
     & axisp1,0)+rsxyy23(i1+2*ks1a,i2+2*ks2a,i3+2*ks3a,axisp1,1)+
     & rsxyz23(i1+2*ks1a,i2+2*ks2a,i3+2*ks3a,axisp1,2))
               c3 = 2.*(rsxyx23(i1+ks1a,i2+ks2a,i3+ks3a,axisp2,0)+
     & rsxyy23(i1+ks1a,i2+ks2a,i3+ks3a,axisp2,1)+rsxyz23(i1+ks1a,i2+
     & ks2a,i3+ks3a,axisp2,2))-(rsxyx23(i1+2*ks1a,i2+2*ks2a,i3+2*ks3a,
     & axisp2,0)+rsxyy23(i1+2*ks1a,i2+2*ks2a,i3+2*ks3a,axisp2,1)+
     & rsxyz23(i1+2*ks1a,i2+2*ks2a,i3+2*ks3a,axisp2,2))
              else
               ! this case should not happen
               stop 40066
              end if
              ! ***** Now do "s"-derivatives *****
              ! warning -- the compiler could still try to evaluate the mask at an invalid point
              if( (i1-js1a).ge.md1a .and. (i2-js2a).ge.md2a .and. (i3-
     & js3a).ge.md3a .and. mask(i1-js1a,i2-js2a,i3-js3a).ne.0 .and. (
     & i1+js1a).le.md1b .and. (i2+js2a).le.md2b .and. (i3+js3a)
     & .le.md3b .and. mask(i1+js1a,i2+js2a,i3+js3a).ne.0 )then
                us=(u(i1+js1,i2+js2,i3+js3,ex)-u(i1-js1,i2-js2,i3-js3,
     & ex))/(2.*dsa)
                vs=(u(i1+js1,i2+js2,i3+js3,ey)-u(i1-js1,i2-js2,i3-js3,
     & ey))/(2.*dsa)
                ws=(u(i1+js1,i2+js2,i3+js3,ez)-u(i1-js1,i2-js2,i3-js3,
     & ez))/(2.*dsa)
                uss=(u(i1+js1,i2+js2,i3+js3,ex)-2.*u(i1,i2,i3,ex)+u(i1-
     & js1,i2-js2,i3-js3,ex))/(dsa**2)
                vss=(u(i1+js1,i2+js2,i3+js3,ey)-2.*u(i1,i2,i3,ey)+u(i1-
     & js1,i2-js2,i3-js3,ey))/(dsa**2)
                wss=(u(i1+js1,i2+js2,i3+js3,ez)-2.*u(i1,i2,i3,ez)+u(i1-
     & js1,i2-js2,i3-js3,ez))/(dsa**2)
               if( debug.gt.0 )then
                 call ogf3d(ep,xy(i1-js1a,i2-js2a,i3-js3a,0),xy(i1-
     & js1a,i2-js2a,i3-js3a,1),xy(i1-js1a,i2-js2a,i3-js3a,2),t,uvm(0),
     & uvm(1),uvm(2))
                 call ogf3d(ep,xy(i1+js1a,i2+js2a,i3+js3a,0),xy(i1+
     & js1a,i2+js2a,i3+js3a,1),xy(i1+js1a,i2+js2a,i3+js3a,2),t,uvp(0),
     & uvp(1),uvp(2))
                write(*,'(" **ghost-interp3d: use central-diff: us,
     & uss=",2f8.3," us2,usm,usp=",3f8.3)') us,uss,(uvp(0)-uvm(0))/(
     & 2.*dsb),(-(-3.*u(i1,i2,i3,ex)+4.*u(i1-js1a,i2-js2a,i3-js3a,ex)-
     & u(i1-2*js1a,i2-2*js2a,i3-2*js3a,ex))/(2.*dsb)),((-3.*u(i1,i2,
     & i3,ex)+4.*u(i1+js1a,i2+js2a,i3+js3a,ex)-u(i1+2*js1a,i2+2*js2a,
     & i3+2*js3a,ex))/(2.*dsb))
               end if
              else if( (i1-2*js1a).ge.md1a .and. (i2-2*js2a).ge.md2a 
     & .and. (i3-2*js3a).ge.md3a .and. mask(i1-js1a,i2-js2a,i3-js3a)
     & .ne.0 .and. mask(i1-2*js1a,i2-2*js2a,i3-js3a).ne.0 )then
               ! 2nd-order one-sided: ** note ** use ds not dsa
               us = (-(-3.*u(i1,i2,i3,ex)+4.*u(i1-js1a,i2-js2a,i3-js3a,
     & ex)-u(i1-2*js1a,i2-2*js2a,i3-2*js3a,ex))/(2.*dsb))
               vs = (-(-3.*u(i1,i2,i3,ey)+4.*u(i1-js1a,i2-js2a,i3-js3a,
     & ey)-u(i1-2*js1a,i2-2*js2a,i3-2*js3a,ey))/(2.*dsb))
               ws = (-(-3.*u(i1,i2,i3,ez)+4.*u(i1-js1a,i2-js2a,i3-js3a,
     & ez)-u(i1-2*js1a,i2-2*js2a,i3-2*js3a,ez))/(2.*dsb))
               uss = ((2.*u(i1,i2,i3,ex)-5.*u(i1-js1a,i2-js2a,i3-js3a,
     & ex)+4.*u(i1-2*js1a,i2-2*js2a,i3-2*js3a,ex)-u(i1-3*js1a,i2-3*
     & js2a,i3-3*js3a,ex))/(dsb**2))
               vss = ((2.*u(i1,i2,i3,ey)-5.*u(i1-js1a,i2-js2a,i3-js3a,
     & ey)+4.*u(i1-2*js1a,i2-2*js2a,i3-2*js3a,ey)-u(i1-3*js1a,i2-3*
     & js2a,i3-3*js3a,ey))/(dsb**2))
               wss = ((2.*u(i1,i2,i3,ez)-5.*u(i1-js1a,i2-js2a,i3-js3a,
     & ez)+4.*u(i1-2*js1a,i2-2*js2a,i3-2*js3a,ez)-u(i1-3*js1a,i2-3*
     & js2a,i3-3*js3a,ez))/(dsb**2))
               if( debug.gt.0 )then
                write(*,'(" **ghost-interp3d: use left-difference: us,
     & uss=",2e10.2," us1=",e10.2," js1,js2=",2i2)') us,uss,(u(i1,i2,
     & i3,ex)-u(i1-js1a,i2-js2a,i3-js3a,ex))/dsb,js1,js2
               end if
              else if( (i1+2*js1a).le.md1b .and. (i2+2*js2a).le.md2b 
     & .and.  (i3+2*js3a).le.md3b .and.  mask(i1+js1a,i2+js2a,i3+js3a)
     & .ne.0 .and. mask(i1+2*js1a,i2+2*js2a,i3+2*js3a).ne.0 )then
               ! 2nd-order one-sided:
               us = ((-3.*u(i1,i2,i3,ex)+4.*u(i1+js1a,i2+js2a,i3+js3a,
     & ex)-u(i1+2*js1a,i2+2*js2a,i3+2*js3a,ex))/(2.*dsb))
               vs = ((-3.*u(i1,i2,i3,ey)+4.*u(i1+js1a,i2+js2a,i3+js3a,
     & ey)-u(i1+2*js1a,i2+2*js2a,i3+2*js3a,ey))/(2.*dsb))
               ws = ((-3.*u(i1,i2,i3,ez)+4.*u(i1+js1a,i2+js2a,i3+js3a,
     & ez)-u(i1+2*js1a,i2+2*js2a,i3+2*js3a,ez))/(2.*dsb))
               uss = ((2.*u(i1,i2,i3,ex)-5.*u(i1+js1a,i2+js2a,i3+js3a,
     & ex)+4.*u(i1+2*js1a,i2+2*js2a,i3+2*js3a,ex)-u(i1+3*js1a,i2+3*
     & js2a,i3+3*js3a,ex))/(dsb**2))
               vss = ((2.*u(i1,i2,i3,ey)-5.*u(i1+js1a,i2+js2a,i3+js3a,
     & ey)+4.*u(i1+2*js1a,i2+2*js2a,i3+2*js3a,ey)-u(i1+3*js1a,i2+3*
     & js2a,i3+3*js3a,ey))/(dsb**2))
               wss = ((2.*u(i1,i2,i3,ez)-5.*u(i1+js1a,i2+js2a,i3+js3a,
     & ez)+4.*u(i1+2*js1a,i2+2*js2a,i3+2*js3a,ez)-u(i1+3*js1a,i2+3*
     & js2a,i3+3*js3a,ez))/(dsb**2))
               if( debug.gt.0 )then
                write(*,'(" **ghost-interp3d: use right-difference: us,
     & uss=",2e10.2," us1=",e10.2," js1,js2=",2i2)') us,uss,(u(i1+
     & js1a,i2+js2a,i3+js3a,ex)-u(i1,i2,i3,ex))/dsb,js1,js2
               end if
              else
                ! this case shouldn't matter
                us=0.
                vs=0.
                ws=0.
                uss=0.
                vss=0.
                wss=0.
              end if
              ! **** t - derivatives ****
              if( (i1-ks1a).ge.md1a .and. (i2-ks2a).ge.md2a .and. (i3-
     & ks3a).ge.md3a .and. mask(i1-ks1a,i2-ks2a,i3-ks3a).ne.0 .and. (
     & i1+ks1a).le.md1b .and. (i2+ks2a).le.md2b .and. (i3+ks3a)
     & .le.md3b .and. mask(i1+ks1a,i2+ks2a,i3+ks3a).ne.0 )then
                ut=(u(i1+ks1,i2+ks2,i3+ks3,ex)-u(i1-ks1,i2-ks2,i3-ks3,
     & ex))/(2.*dta)
                vt=(u(i1+ks1,i2+ks2,i3+ks3,ey)-u(i1-ks1,i2-ks2,i3-ks3,
     & ey))/(2.*dta)
                wt=(u(i1+ks1,i2+ks2,i3+ks3,ez)-u(i1-ks1,i2-ks2,i3-ks3,
     & ez))/(2.*dta)
                utt=(u(i1+ks1,i2+ks2,i3+ks3,ex)-2.*u(i1,i2,i3,ex)+u(i1-
     & ks1,i2-ks2,i3-ks3,ex))/(dta**2)
                vtt=(u(i1+ks1,i2+ks2,i3+ks3,ey)-2.*u(i1,i2,i3,ey)+u(i1-
     & ks1,i2-ks2,i3-ks3,ey))/(dta**2)
                wtt=(u(i1+ks1,i2+ks2,i3+ks3,ez)-2.*u(i1,i2,i3,ez)+u(i1-
     & ks1,i2-ks2,i3-ks3,ez))/(dta**2)
               if( debug.gt.0 )then
                 call ogf3d(ep,xy(i1-ks1a,i2-ks2a,i3-ks3a,0),xy(i1-
     & ks1a,i2-ks2a,i3-ks3a,1),xy(i1-ks1a,i2-ks2a,i3-ks3a,2),t,uvm(0),
     & uvm(1),uvm(2))
                 call ogf3d(ep,xy(i1+ks1a,i2+ks2a,i3+ks3a,0),xy(i1+
     & ks1a,i2+ks2a,i3+ks3a,1),xy(i1+ks1a,i2+ks2a,i3+ks3a,2),t,uvp(0),
     & uvp(1),uvp(2))
                write(*,'(" **ghost-interp3d: use central-diff: ut,
     & utt=",2f8.3," ut2=",f8.3)') ut,utt,(uvp(0)-uvm(0))/(2.*dtb)
               end if
              else if( (i1-2*ks1a).ge.md1a .and. (i2-2*ks2a).ge.md2a 
     & .and. (i3-2*ks3a).ge.md3a .and. mask(i1-ks1a,i2-ks2a,i3-ks3a)
     & .ne.0 .and. mask(i1-2*ks1a,i2-2*ks2a,i3-ks3a).ne.0 )then
               ! 2nd-order one-sided:
               ut = (-(-3.*u(i1,i2,i3,ex)+4.*u(i1-ks1a,i2-ks2a,i3-ks3a,
     & ex)-u(i1-2*ks1a,i2-2*ks2a,i3-2*ks3a,ex))/(2.*dtb))
               vt = (-(-3.*u(i1,i2,i3,ey)+4.*u(i1-ks1a,i2-ks2a,i3-ks3a,
     & ey)-u(i1-2*ks1a,i2-2*ks2a,i3-2*ks3a,ey))/(2.*dtb))
               wt = (-(-3.*u(i1,i2,i3,ez)+4.*u(i1-ks1a,i2-ks2a,i3-ks3a,
     & ez)-u(i1-2*ks1a,i2-2*ks2a,i3-2*ks3a,ez))/(2.*dtb))
               utt = ((2.*u(i1,i2,i3,ex)-5.*u(i1-ks1a,i2-ks2a,i3-ks3a,
     & ex)+4.*u(i1-2*ks1a,i2-2*ks2a,i3-2*ks3a,ex)-u(i1-3*ks1a,i2-3*
     & ks2a,i3-3*ks3a,ex))/(dtb**2))
               vtt = ((2.*u(i1,i2,i3,ey)-5.*u(i1-ks1a,i2-ks2a,i3-ks3a,
     & ey)+4.*u(i1-2*ks1a,i2-2*ks2a,i3-2*ks3a,ey)-u(i1-3*ks1a,i2-3*
     & ks2a,i3-3*ks3a,ey))/(dtb**2))
               wtt = ((2.*u(i1,i2,i3,ez)-5.*u(i1-ks1a,i2-ks2a,i3-ks3a,
     & ez)+4.*u(i1-2*ks1a,i2-2*ks2a,i3-2*ks3a,ez)-u(i1-3*ks1a,i2-3*
     & ks2a,i3-3*ks3a,ez))/(dtb**2))
               if( debug.gt.0 )then
                write(*,'(" **ghost-interp3d: use left-difference: ut,
     & utt=",2e10.2," ut1=",e10.2," kt1,kt2=",2i2)') ut,utt,(u(i1,i2,
     & i3,ex)-u(i1-ks1a,i2-ks2a,i3-ks3a,ex))/dtb,ks1,ks2
               end if
              else if( (i1+2*ks1a).le.md1b .and. (i2+2*ks2a).le.md2b 
     & .and.  (i3+2*ks3a).le.md3b .and.  mask(i1+ks1a,i2+ks2a,i3+ks3a)
     & .ne.0 .and. mask(i1+2*ks1a,i2+2*ks2a,i3+2*ks3a).ne.0 )then
               ! 2nd-order one-sided:
               ut = ((-3.*u(i1,i2,i3,ex)+4.*u(i1+ks1a,i2+ks2a,i3+ks3a,
     & ex)-u(i1+2*ks1a,i2+2*ks2a,i3+2*ks3a,ex))/(2.*dtb))
               vt = ((-3.*u(i1,i2,i3,ey)+4.*u(i1+ks1a,i2+ks2a,i3+ks3a,
     & ey)-u(i1+2*ks1a,i2+2*ks2a,i3+2*ks3a,ey))/(2.*dtb))
               wt = ((-3.*u(i1,i2,i3,ez)+4.*u(i1+ks1a,i2+ks2a,i3+ks3a,
     & ez)-u(i1+2*ks1a,i2+2*ks2a,i3+2*ks3a,ez))/(2.*dtb))
               utt = ((2.*u(i1,i2,i3,ex)-5.*u(i1+ks1a,i2+ks2a,i3+ks3a,
     & ex)+4.*u(i1+2*ks1a,i2+2*ks2a,i3+2*ks3a,ex)-u(i1+3*ks1a,i2+3*
     & ks2a,i3+3*ks3a,ex))/(dtb**2))
               vtt = ((2.*u(i1,i2,i3,ey)-5.*u(i1+ks1a,i2+ks2a,i3+ks3a,
     & ey)+4.*u(i1+2*ks1a,i2+2*ks2a,i3+2*ks3a,ey)-u(i1+3*ks1a,i2+3*
     & ks2a,i3+3*ks3a,ey))/(dtb**2))
               wtt = ((2.*u(i1,i2,i3,ez)-5.*u(i1+ks1a,i2+ks2a,i3+ks3a,
     & ez)+4.*u(i1+2*ks1a,i2+2*ks2a,i3+2*ks3a,ez)-u(i1+3*ks1a,i2+3*
     & ks2a,i3+3*ks3a,ez))/(dtb**2))
               if( debug.gt.0 )then
                 call ogf3d(ep,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,
     & i3,2),t,uv0(0),uv0(1),uv0(2))
                 call ogf3d(ep,xy(i1+ks1a,i2+ks2a,i3+ks3a,0),xy(i1+
     & ks1a,i2+ks2a,i3+ks3a,1),xy(i1+ks1a,i2+ks2a,i3+ks3a,2),t,uvp(0),
     & uvp(1),uvp(2))
                 call ogf3d(ep,xy(i1+2*ks1a,i2+2*ks2a,i3+2*ks3a,0),xy(
     & i1+2*ks1a,i2+2*ks2a,i3+2*ks3a,1),xy(i1+2*ks1a,i2+2*ks2a,i3+2*
     & ks3a,2),t,uvp2(0),uvp2(1),uvp2(2))
                write(*,'(" **ghost-interp3d: use right-diff: ut,utt=",
     & 2f8.3," ut1,ut2=",2f8.3," dta,dtb=",2f7.4)') ut,utt,(u(i1+ks1a,
     & i2+ks2a,i3+ks3a,ex)-u(i1,i2,i3,ex))/dtb,(4.*uvp(0)-3.*uv0(0)-
     & uvp2(0))/(2.*dtb),dta,dtb
               end if
              ! write(*,'(" **ghost-interp: use right-difference: ut,utt=",2e10.2)') ut,utt
              else
                ! this case shouldn't matter
                ut=0.
                vt=0.
                wt=0.
                utt=0.
                vtt=0.
                wtt=0.
              end if
              Da1DotU=0.
              tau1DotUtt=0.
              tau2DotUtt=0.
              gIVf1=0.
              gIVf2=0.
              ! Compute a21s, a31t, ... for RHS to div equation
              if( forcingOption.ne.0 )then
               if( (i1-js1a).ge.md1a .and. (i1+js1a).le.md1b .and. (i2-
     & js2a).ge.md2a .and. (i2+js2a).le.md2b .and. (i3-js3a).ge.md3a 
     & .and. (i3+js3a).le.md3b )then
                a21s = ((rsxy(i1+js1,i2+js2,i3+js3,axisp1,0)/(rx(i1+
     & js1,i2+js2,i3+js3)*(sy(i1+js1,i2+js2,i3+js3)*tz(i1+js1,i2+js2,
     & i3+js3)-sz(i1+js1,i2+js2,i3+js3)*ty(i1+js1,i2+js2,i3+js3))+ry(
     & i1+js1,i2+js2,i3+js3)*(sz(i1+js1,i2+js2,i3+js3)*tx(i1+js1,i2+
     & js2,i3+js3)-sx(i1+js1,i2+js2,i3+js3)*tz(i1+js1,i2+js2,i3+js3))+
     & rz(i1+js1,i2+js2,i3+js3)*(sx(i1+js1,i2+js2,i3+js3)*ty(i1+js1,
     & i2+js2,i3+js3)-sy(i1+js1,i2+js2,i3+js3)*tx(i1+js1,i2+js2,i3+
     & js3))))-(rsxy(i1-js1,i2-js2,i3-js3,axisp1,0)/(rx(i1-js1,i2-js2,
     & i3-js3)*(sy(i1-js1,i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3)-sz(
     & i1-js1,i2-js2,i3-js3)*ty(i1-js1,i2-js2,i3-js3))+ry(i1-js1,i2-
     & js2,i3-js3)*(sz(i1-js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3)-
     & sx(i1-js1,i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3))+rz(i1-js1,
     & i2-js2,i3-js3)*(sx(i1-js1,i2-js2,i3-js3)*ty(i1-js1,i2-js2,i3-
     & js3)-sy(i1-js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3)))))/(2.*
     & dsa)
                a22s = ((rsxy(i1+js1,i2+js2,i3+js3,axisp1,1)/(rx(i1+
     & js1,i2+js2,i3+js3)*(sy(i1+js1,i2+js2,i3+js3)*tz(i1+js1,i2+js2,
     & i3+js3)-sz(i1+js1,i2+js2,i3+js3)*ty(i1+js1,i2+js2,i3+js3))+ry(
     & i1+js1,i2+js2,i3+js3)*(sz(i1+js1,i2+js2,i3+js3)*tx(i1+js1,i2+
     & js2,i3+js3)-sx(i1+js1,i2+js2,i3+js3)*tz(i1+js1,i2+js2,i3+js3))+
     & rz(i1+js1,i2+js2,i3+js3)*(sx(i1+js1,i2+js2,i3+js3)*ty(i1+js1,
     & i2+js2,i3+js3)-sy(i1+js1,i2+js2,i3+js3)*tx(i1+js1,i2+js2,i3+
     & js3))))-(rsxy(i1-js1,i2-js2,i3-js3,axisp1,1)/(rx(i1-js1,i2-js2,
     & i3-js3)*(sy(i1-js1,i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3)-sz(
     & i1-js1,i2-js2,i3-js3)*ty(i1-js1,i2-js2,i3-js3))+ry(i1-js1,i2-
     & js2,i3-js3)*(sz(i1-js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3)-
     & sx(i1-js1,i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3))+rz(i1-js1,
     & i2-js2,i3-js3)*(sx(i1-js1,i2-js2,i3-js3)*ty(i1-js1,i2-js2,i3-
     & js3)-sy(i1-js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3)))))/(2.*
     & dsa)
                a23s = ((rsxy(i1+js1,i2+js2,i3+js3,axisp1,2)/(rx(i1+
     & js1,i2+js2,i3+js3)*(sy(i1+js1,i2+js2,i3+js3)*tz(i1+js1,i2+js2,
     & i3+js3)-sz(i1+js1,i2+js2,i3+js3)*ty(i1+js1,i2+js2,i3+js3))+ry(
     & i1+js1,i2+js2,i3+js3)*(sz(i1+js1,i2+js2,i3+js3)*tx(i1+js1,i2+
     & js2,i3+js3)-sx(i1+js1,i2+js2,i3+js3)*tz(i1+js1,i2+js2,i3+js3))+
     & rz(i1+js1,i2+js2,i3+js3)*(sx(i1+js1,i2+js2,i3+js3)*ty(i1+js1,
     & i2+js2,i3+js3)-sy(i1+js1,i2+js2,i3+js3)*tx(i1+js1,i2+js2,i3+
     & js3))))-(rsxy(i1-js1,i2-js2,i3-js3,axisp1,2)/(rx(i1-js1,i2-js2,
     & i3-js3)*(sy(i1-js1,i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3)-sz(
     & i1-js1,i2-js2,i3-js3)*ty(i1-js1,i2-js2,i3-js3))+ry(i1-js1,i2-
     & js2,i3-js3)*(sz(i1-js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3)-
     & sx(i1-js1,i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3))+rz(i1-js1,
     & i2-js2,i3-js3)*(sx(i1-js1,i2-js2,i3-js3)*ty(i1-js1,i2-js2,i3-
     & js3)-sy(i1-js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3)))))/(2.*
     & dsa)
               else if( (i1+js1).ge.md1a .and. (i1+js1).le.md1b .and. (
     & i2+js2).ge.md2a .and. (i2+js2).le.md2b .and. (i3+js3).ge.md3a 
     & .and. (i3+js3).le.md3b )then
                a21s = ((rsxy(i1+js1,i2+js2,i3+js3,axisp1,0)/(rx(i1+
     & js1,i2+js2,i3+js3)*(sy(i1+js1,i2+js2,i3+js3)*tz(i1+js1,i2+js2,
     & i3+js3)-sz(i1+js1,i2+js2,i3+js3)*ty(i1+js1,i2+js2,i3+js3))+ry(
     & i1+js1,i2+js2,i3+js3)*(sz(i1+js1,i2+js2,i3+js3)*tx(i1+js1,i2+
     & js2,i3+js3)-sx(i1+js1,i2+js2,i3+js3)*tz(i1+js1,i2+js2,i3+js3))+
     & rz(i1+js1,i2+js2,i3+js3)*(sx(i1+js1,i2+js2,i3+js3)*ty(i1+js1,
     & i2+js2,i3+js3)-sy(i1+js1,i2+js2,i3+js3)*tx(i1+js1,i2+js2,i3+
     & js3))))-(rsxy(i1,i2,i3,axisp1,0)/(rx(i1,i2,i3)*(sy(i1,i2,i3)*
     & tz(i1,i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(sz(i1,i2,
     & i3)*tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,i3)*(sx(
     & i1,i2,i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3)))))/(dsa)
                a22s = ((rsxy(i1+js1,i2+js2,i3+js3,axisp1,1)/(rx(i1+
     & js1,i2+js2,i3+js3)*(sy(i1+js1,i2+js2,i3+js3)*tz(i1+js1,i2+js2,
     & i3+js3)-sz(i1+js1,i2+js2,i3+js3)*ty(i1+js1,i2+js2,i3+js3))+ry(
     & i1+js1,i2+js2,i3+js3)*(sz(i1+js1,i2+js2,i3+js3)*tx(i1+js1,i2+
     & js2,i3+js3)-sx(i1+js1,i2+js2,i3+js3)*tz(i1+js1,i2+js2,i3+js3))+
     & rz(i1+js1,i2+js2,i3+js3)*(sx(i1+js1,i2+js2,i3+js3)*ty(i1+js1,
     & i2+js2,i3+js3)-sy(i1+js1,i2+js2,i3+js3)*tx(i1+js1,i2+js2,i3+
     & js3))))-(rsxy(i1,i2,i3,axisp1,1)/(rx(i1,i2,i3)*(sy(i1,i2,i3)*
     & tz(i1,i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(sz(i1,i2,
     & i3)*tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,i3)*(sx(
     & i1,i2,i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3)))))/(dsa)
                a23s = ((rsxy(i1+js1,i2+js2,i3+js3,axisp1,2)/(rx(i1+
     & js1,i2+js2,i3+js3)*(sy(i1+js1,i2+js2,i3+js3)*tz(i1+js1,i2+js2,
     & i3+js3)-sz(i1+js1,i2+js2,i3+js3)*ty(i1+js1,i2+js2,i3+js3))+ry(
     & i1+js1,i2+js2,i3+js3)*(sz(i1+js1,i2+js2,i3+js3)*tx(i1+js1,i2+
     & js2,i3+js3)-sx(i1+js1,i2+js2,i3+js3)*tz(i1+js1,i2+js2,i3+js3))+
     & rz(i1+js1,i2+js2,i3+js3)*(sx(i1+js1,i2+js2,i3+js3)*ty(i1+js1,
     & i2+js2,i3+js3)-sy(i1+js1,i2+js2,i3+js3)*tx(i1+js1,i2+js2,i3+
     & js3))))-(rsxy(i1,i2,i3,axisp1,2)/(rx(i1,i2,i3)*(sy(i1,i2,i3)*
     & tz(i1,i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(sz(i1,i2,
     & i3)*tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,i3)*(sx(
     & i1,i2,i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3)))))/(dsa)
               else if( (i1-js1).ge.md1a .and. (i1-js1).le.md1b .and. (
     & i2-js2).ge.md2a .and. (i2-js2).le.md2b .and. (i3-js3).ge.md3a 
     & .and. (i3-js3).le.md3b )then
                a21s = ((rsxy(i1,i2,i3,axisp1,0)/(rx(i1,i2,i3)*(sy(i1,
     & i2,i3)*tz(i1,i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(
     & sz(i1,i2,i3)*tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,
     & i3)*(sx(i1,i2,i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3))))-(
     & rsxy(i1-js1,i2-js2,i3-js3,axisp1,0)/(rx(i1-js1,i2-js2,i3-js3)*(
     & sy(i1-js1,i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3)-sz(i1-js1,i2-
     & js2,i3-js3)*ty(i1-js1,i2-js2,i3-js3))+ry(i1-js1,i2-js2,i3-js3)*
     & (sz(i1-js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3)-sx(i1-js1,
     & i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3))+rz(i1-js1,i2-js2,i3-
     & js3)*(sx(i1-js1,i2-js2,i3-js3)*ty(i1-js1,i2-js2,i3-js3)-sy(i1-
     & js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3)))))/(dsa)
                a22s = ((rsxy(i1,i2,i3,axisp1,1)/(rx(i1,i2,i3)*(sy(i1,
     & i2,i3)*tz(i1,i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(
     & sz(i1,i2,i3)*tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,
     & i3)*(sx(i1,i2,i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3))))-(
     & rsxy(i1-js1,i2-js2,i3-js3,axisp1,1)/(rx(i1-js1,i2-js2,i3-js3)*(
     & sy(i1-js1,i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3)-sz(i1-js1,i2-
     & js2,i3-js3)*ty(i1-js1,i2-js2,i3-js3))+ry(i1-js1,i2-js2,i3-js3)*
     & (sz(i1-js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3)-sx(i1-js1,
     & i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3))+rz(i1-js1,i2-js2,i3-
     & js3)*(sx(i1-js1,i2-js2,i3-js3)*ty(i1-js1,i2-js2,i3-js3)-sy(i1-
     & js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3)))))/(dsa)
                a23s = ((rsxy(i1,i2,i3,axisp1,2)/(rx(i1,i2,i3)*(sy(i1,
     & i2,i3)*tz(i1,i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(
     & sz(i1,i2,i3)*tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,
     & i3)*(sx(i1,i2,i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3))))-(
     & rsxy(i1-js1,i2-js2,i3-js3,axisp1,2)/(rx(i1-js1,i2-js2,i3-js3)*(
     & sy(i1-js1,i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3)-sz(i1-js1,i2-
     & js2,i3-js3)*ty(i1-js1,i2-js2,i3-js3))+ry(i1-js1,i2-js2,i3-js3)*
     & (sz(i1-js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3)-sx(i1-js1,
     & i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3))+rz(i1-js1,i2-js2,i3-
     & js3)*(sx(i1-js1,i2-js2,i3-js3)*ty(i1-js1,i2-js2,i3-js3)-sy(i1-
     & js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3)))))/(dsa)
               else
                 stop 82750
               end if
               if( (i1-ks1a).ge.md1a .and. (i1+ks1a).le.md1b .and. (i2-
     & ks2a).ge.md2a .and. (i2+ks2a).le.md2b .and. (i3-ks3a).ge.md3a 
     & .and. (i3+ks3a).le.md3b )then
                a31t = ((rsxy(i1+ks1,i2+ks2,i3+ks3,axisp2,0)/(rx(i1+
     & ks1,i2+ks2,i3+ks3)*(sy(i1+ks1,i2+ks2,i3+ks3)*tz(i1+ks1,i2+ks2,
     & i3+ks3)-sz(i1+ks1,i2+ks2,i3+ks3)*ty(i1+ks1,i2+ks2,i3+ks3))+ry(
     & i1+ks1,i2+ks2,i3+ks3)*(sz(i1+ks1,i2+ks2,i3+ks3)*tx(i1+ks1,i2+
     & ks2,i3+ks3)-sx(i1+ks1,i2+ks2,i3+ks3)*tz(i1+ks1,i2+ks2,i3+ks3))+
     & rz(i1+ks1,i2+ks2,i3+ks3)*(sx(i1+ks1,i2+ks2,i3+ks3)*ty(i1+ks1,
     & i2+ks2,i3+ks3)-sy(i1+ks1,i2+ks2,i3+ks3)*tx(i1+ks1,i2+ks2,i3+
     & ks3))))-(rsxy(i1-ks1,i2-ks2,i3-ks3,axisp2,0)/(rx(i1-ks1,i2-ks2,
     & i3-ks3)*(sy(i1-ks1,i2-ks2,i3-ks3)*tz(i1-ks1,i2-ks2,i3-ks3)-sz(
     & i1-ks1,i2-ks2,i3-ks3)*ty(i1-ks1,i2-ks2,i3-ks3))+ry(i1-ks1,i2-
     & ks2,i3-ks3)*(sz(i1-ks1,i2-ks2,i3-ks3)*tx(i1-ks1,i2-ks2,i3-ks3)-
     & sx(i1-ks1,i2-ks2,i3-ks3)*tz(i1-ks1,i2-ks2,i3-ks3))+rz(i1-ks1,
     & i2-ks2,i3-ks3)*(sx(i1-ks1,i2-ks2,i3-ks3)*ty(i1-ks1,i2-ks2,i3-
     & ks3)-sy(i1-ks1,i2-ks2,i3-ks3)*tx(i1-ks1,i2-ks2,i3-ks3)))))/(2.*
     & dta)
                a32t = ((rsxy(i1+ks1,i2+ks2,i3+ks3,axisp2,1)/(rx(i1+
     & ks1,i2+ks2,i3+ks3)*(sy(i1+ks1,i2+ks2,i3+ks3)*tz(i1+ks1,i2+ks2,
     & i3+ks3)-sz(i1+ks1,i2+ks2,i3+ks3)*ty(i1+ks1,i2+ks2,i3+ks3))+ry(
     & i1+ks1,i2+ks2,i3+ks3)*(sz(i1+ks1,i2+ks2,i3+ks3)*tx(i1+ks1,i2+
     & ks2,i3+ks3)-sx(i1+ks1,i2+ks2,i3+ks3)*tz(i1+ks1,i2+ks2,i3+ks3))+
     & rz(i1+ks1,i2+ks2,i3+ks3)*(sx(i1+ks1,i2+ks2,i3+ks3)*ty(i1+ks1,
     & i2+ks2,i3+ks3)-sy(i1+ks1,i2+ks2,i3+ks3)*tx(i1+ks1,i2+ks2,i3+
     & ks3))))-(rsxy(i1-ks1,i2-ks2,i3-ks3,axisp2,1)/(rx(i1-ks1,i2-ks2,
     & i3-ks3)*(sy(i1-ks1,i2-ks2,i3-ks3)*tz(i1-ks1,i2-ks2,i3-ks3)-sz(
     & i1-ks1,i2-ks2,i3-ks3)*ty(i1-ks1,i2-ks2,i3-ks3))+ry(i1-ks1,i2-
     & ks2,i3-ks3)*(sz(i1-ks1,i2-ks2,i3-ks3)*tx(i1-ks1,i2-ks2,i3-ks3)-
     & sx(i1-ks1,i2-ks2,i3-ks3)*tz(i1-ks1,i2-ks2,i3-ks3))+rz(i1-ks1,
     & i2-ks2,i3-ks3)*(sx(i1-ks1,i2-ks2,i3-ks3)*ty(i1-ks1,i2-ks2,i3-
     & ks3)-sy(i1-ks1,i2-ks2,i3-ks3)*tx(i1-ks1,i2-ks2,i3-ks3)))))/(2.*
     & dta)
                a33t = ((rsxy(i1+ks1,i2+ks2,i3+ks3,axisp2,2)/(rx(i1+
     & ks1,i2+ks2,i3+ks3)*(sy(i1+ks1,i2+ks2,i3+ks3)*tz(i1+ks1,i2+ks2,
     & i3+ks3)-sz(i1+ks1,i2+ks2,i3+ks3)*ty(i1+ks1,i2+ks2,i3+ks3))+ry(
     & i1+ks1,i2+ks2,i3+ks3)*(sz(i1+ks1,i2+ks2,i3+ks3)*tx(i1+ks1,i2+
     & ks2,i3+ks3)-sx(i1+ks1,i2+ks2,i3+ks3)*tz(i1+ks1,i2+ks2,i3+ks3))+
     & rz(i1+ks1,i2+ks2,i3+ks3)*(sx(i1+ks1,i2+ks2,i3+ks3)*ty(i1+ks1,
     & i2+ks2,i3+ks3)-sy(i1+ks1,i2+ks2,i3+ks3)*tx(i1+ks1,i2+ks2,i3+
     & ks3))))-(rsxy(i1-ks1,i2-ks2,i3-ks3,axisp2,2)/(rx(i1-ks1,i2-ks2,
     & i3-ks3)*(sy(i1-ks1,i2-ks2,i3-ks3)*tz(i1-ks1,i2-ks2,i3-ks3)-sz(
     & i1-ks1,i2-ks2,i3-ks3)*ty(i1-ks1,i2-ks2,i3-ks3))+ry(i1-ks1,i2-
     & ks2,i3-ks3)*(sz(i1-ks1,i2-ks2,i3-ks3)*tx(i1-ks1,i2-ks2,i3-ks3)-
     & sx(i1-ks1,i2-ks2,i3-ks3)*tz(i1-ks1,i2-ks2,i3-ks3))+rz(i1-ks1,
     & i2-ks2,i3-ks3)*(sx(i1-ks1,i2-ks2,i3-ks3)*ty(i1-ks1,i2-ks2,i3-
     & ks3)-sy(i1-ks1,i2-ks2,i3-ks3)*tx(i1-ks1,i2-ks2,i3-ks3)))))/(2.*
     & dta)
               else if( (i1+ks1).ge.md1a .and. (i1+ks1).le.md1b .and. (
     & i2+ks2).ge.md2a .and. (i2+ks2).le.md2b .and. (i3+ks3).ge.md3a 
     & .and. (i3+ks3).le.md3b )then
                a31t = ((rsxy(i1+ks1,i2+ks2,i3+ks3,axisp2,0)/(rx(i1+
     & ks1,i2+ks2,i3+ks3)*(sy(i1+ks1,i2+ks2,i3+ks3)*tz(i1+ks1,i2+ks2,
     & i3+ks3)-sz(i1+ks1,i2+ks2,i3+ks3)*ty(i1+ks1,i2+ks2,i3+ks3))+ry(
     & i1+ks1,i2+ks2,i3+ks3)*(sz(i1+ks1,i2+ks2,i3+ks3)*tx(i1+ks1,i2+
     & ks2,i3+ks3)-sx(i1+ks1,i2+ks2,i3+ks3)*tz(i1+ks1,i2+ks2,i3+ks3))+
     & rz(i1+ks1,i2+ks2,i3+ks3)*(sx(i1+ks1,i2+ks2,i3+ks3)*ty(i1+ks1,
     & i2+ks2,i3+ks3)-sy(i1+ks1,i2+ks2,i3+ks3)*tx(i1+ks1,i2+ks2,i3+
     & ks3))))-(rsxy(i1,i2,i3,axisp2,0)/(rx(i1,i2,i3)*(sy(i1,i2,i3)*
     & tz(i1,i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(sz(i1,i2,
     & i3)*tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,i3)*(sx(
     & i1,i2,i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3)))))/(dta)
                a32t = ((rsxy(i1+ks1,i2+ks2,i3+ks3,axisp2,1)/(rx(i1+
     & ks1,i2+ks2,i3+ks3)*(sy(i1+ks1,i2+ks2,i3+ks3)*tz(i1+ks1,i2+ks2,
     & i3+ks3)-sz(i1+ks1,i2+ks2,i3+ks3)*ty(i1+ks1,i2+ks2,i3+ks3))+ry(
     & i1+ks1,i2+ks2,i3+ks3)*(sz(i1+ks1,i2+ks2,i3+ks3)*tx(i1+ks1,i2+
     & ks2,i3+ks3)-sx(i1+ks1,i2+ks2,i3+ks3)*tz(i1+ks1,i2+ks2,i3+ks3))+
     & rz(i1+ks1,i2+ks2,i3+ks3)*(sx(i1+ks1,i2+ks2,i3+ks3)*ty(i1+ks1,
     & i2+ks2,i3+ks3)-sy(i1+ks1,i2+ks2,i3+ks3)*tx(i1+ks1,i2+ks2,i3+
     & ks3))))-(rsxy(i1,i2,i3,axisp2,1)/(rx(i1,i2,i3)*(sy(i1,i2,i3)*
     & tz(i1,i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(sz(i1,i2,
     & i3)*tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,i3)*(sx(
     & i1,i2,i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3)))))/(dta)
                a33t = ((rsxy(i1+ks1,i2+ks2,i3+ks3,axisp2,2)/(rx(i1+
     & ks1,i2+ks2,i3+ks3)*(sy(i1+ks1,i2+ks2,i3+ks3)*tz(i1+ks1,i2+ks2,
     & i3+ks3)-sz(i1+ks1,i2+ks2,i3+ks3)*ty(i1+ks1,i2+ks2,i3+ks3))+ry(
     & i1+ks1,i2+ks2,i3+ks3)*(sz(i1+ks1,i2+ks2,i3+ks3)*tx(i1+ks1,i2+
     & ks2,i3+ks3)-sx(i1+ks1,i2+ks2,i3+ks3)*tz(i1+ks1,i2+ks2,i3+ks3))+
     & rz(i1+ks1,i2+ks2,i3+ks3)*(sx(i1+ks1,i2+ks2,i3+ks3)*ty(i1+ks1,
     & i2+ks2,i3+ks3)-sy(i1+ks1,i2+ks2,i3+ks3)*tx(i1+ks1,i2+ks2,i3+
     & ks3))))-(rsxy(i1,i2,i3,axisp2,2)/(rx(i1,i2,i3)*(sy(i1,i2,i3)*
     & tz(i1,i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(sz(i1,i2,
     & i3)*tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,i3)*(sx(
     & i1,i2,i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3)))))/(dta)
               else if( (i1-ks1).ge.md1a .and. (i1-ks1).le.md1b .and. (
     & i2-ks2).ge.md2a .and. (i2-ks2).le.md2b .and. (i3-ks3).ge.md3a 
     & .and. (i3-ks3).le.md3b )then
                a31t = ((rsxy(i1,i2,i3,axisp2,0)/(rx(i1,i2,i3)*(sy(i1,
     & i2,i3)*tz(i1,i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(
     & sz(i1,i2,i3)*tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,
     & i3)*(sx(i1,i2,i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3))))-(
     & rsxy(i1-ks1,i2-ks2,i3-ks3,axisp2,0)/(rx(i1-ks1,i2-ks2,i3-ks3)*(
     & sy(i1-ks1,i2-ks2,i3-ks3)*tz(i1-ks1,i2-ks2,i3-ks3)-sz(i1-ks1,i2-
     & ks2,i3-ks3)*ty(i1-ks1,i2-ks2,i3-ks3))+ry(i1-ks1,i2-ks2,i3-ks3)*
     & (sz(i1-ks1,i2-ks2,i3-ks3)*tx(i1-ks1,i2-ks2,i3-ks3)-sx(i1-ks1,
     & i2-ks2,i3-ks3)*tz(i1-ks1,i2-ks2,i3-ks3))+rz(i1-ks1,i2-ks2,i3-
     & ks3)*(sx(i1-ks1,i2-ks2,i3-ks3)*ty(i1-ks1,i2-ks2,i3-ks3)-sy(i1-
     & ks1,i2-ks2,i3-ks3)*tx(i1-ks1,i2-ks2,i3-ks3)))))/(dta)
                a32t = ((rsxy(i1,i2,i3,axisp2,1)/(rx(i1,i2,i3)*(sy(i1,
     & i2,i3)*tz(i1,i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(
     & sz(i1,i2,i3)*tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,
     & i3)*(sx(i1,i2,i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3))))-(
     & rsxy(i1-ks1,i2-ks2,i3-ks3,axisp2,1)/(rx(i1-ks1,i2-ks2,i3-ks3)*(
     & sy(i1-ks1,i2-ks2,i3-ks3)*tz(i1-ks1,i2-ks2,i3-ks3)-sz(i1-ks1,i2-
     & ks2,i3-ks3)*ty(i1-ks1,i2-ks2,i3-ks3))+ry(i1-ks1,i2-ks2,i3-ks3)*
     & (sz(i1-ks1,i2-ks2,i3-ks3)*tx(i1-ks1,i2-ks2,i3-ks3)-sx(i1-ks1,
     & i2-ks2,i3-ks3)*tz(i1-ks1,i2-ks2,i3-ks3))+rz(i1-ks1,i2-ks2,i3-
     & ks3)*(sx(i1-ks1,i2-ks2,i3-ks3)*ty(i1-ks1,i2-ks2,i3-ks3)-sy(i1-
     & ks1,i2-ks2,i3-ks3)*tx(i1-ks1,i2-ks2,i3-ks3)))))/(dta)
                a33t = ((rsxy(i1,i2,i3,axisp2,2)/(rx(i1,i2,i3)*(sy(i1,
     & i2,i3)*tz(i1,i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(
     & sz(i1,i2,i3)*tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,
     & i3)*(sx(i1,i2,i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3))))-(
     & rsxy(i1-ks1,i2-ks2,i3-ks3,axisp2,2)/(rx(i1-ks1,i2-ks2,i3-ks3)*(
     & sy(i1-ks1,i2-ks2,i3-ks3)*tz(i1-ks1,i2-ks2,i3-ks3)-sz(i1-ks1,i2-
     & ks2,i3-ks3)*ty(i1-ks1,i2-ks2,i3-ks3))+ry(i1-ks1,i2-ks2,i3-ks3)*
     & (sz(i1-ks1,i2-ks2,i3-ks3)*tx(i1-ks1,i2-ks2,i3-ks3)-sx(i1-ks1,
     & i2-ks2,i3-ks3)*tz(i1-ks1,i2-ks2,i3-ks3))+rz(i1-ks1,i2-ks2,i3-
     & ks3)*(sx(i1-ks1,i2-ks2,i3-ks3)*ty(i1-ks1,i2-ks2,i3-ks3)-sy(i1-
     & ks1,i2-ks2,i3-ks3)*tx(i1-ks1,i2-ks2,i3-ks3)))))/(dta)
               else
                 stop 8250
               end if
              end if
              if( forcingOption.eq.planeWaveBoundaryForcing )then
                ! In the plane wave forcing case we subtract out a plane wave incident field
                !   --->    tau.utt = -tau.uI.tt
                ! *** set RHS for (a1.u).r =  - Ds( a2.uv ) -Dt( a3.uv )
                Da1DotU = -(  a21s*uex+a22s*uey+a23s*uez + a21*us+a22*
     & vs+a23*ws + a31t*uex+a32t*uey+a33t*uez + a31*ut+a32*vt+a33*wt )
                 x00=xy(i1,i2,i3,0)
                 y00=xy(i1,i2,i3,1)
                 z00=xy(i1,i2,i3,2)
                 if( fieldOption.eq.0 )then
                   udd=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x00)+ky*(
     & y00)+kz*(z00)-cc*(t)))*pwc(0))+2.*ssft*(-twoPi*cc)*cos(twoPi*(
     & kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0)+ssftt*sin(twoPi*(kx*
     & (x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))
                   vdd=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x00)+ky*(
     & y00)+kz*(z00)-cc*(t)))*pwc(1))+2.*ssft*(-twoPi*cc)*cos(twoPi*(
     & kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1)+ssftt*sin(twoPi*(kx*
     & (x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))
                   wdd=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x00)+ky*(
     & y00)+kz*(z00)-cc*(t)))*pwc(2))+2.*ssft*(-twoPi*cc)*cos(twoPi*(
     & kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2)+ssftt*sin(twoPi*(kx*
     & (x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))
                 else
                   ! get time derivative (sosup) 
                   udd=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x00)+ky*(
     & y00)+kz*(z00)-cc*(t)))*pwc(0))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))+3.*ssftt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0)
     & +ssfttt*sin(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))
                   vdd=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x00)+ky*(
     & y00)+kz*(z00)-cc*(t)))*pwc(1))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))+3.*ssftt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1)
     & +ssfttt*sin(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))
                   wdd=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x00)+ky*(
     & y00)+kz*(z00)-cc*(t)))*pwc(2))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))+3.*ssftt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2)
     & +ssfttt*sin(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))
                 end if
                tau1DotUtt = tau11*udd+tau12*vdd+tau13*wdd
                tau2DotUtt = tau21*udd+tau22*vdd+tau23*wdd
              end if
             ! Now assign E at the ghost points:


! ************ Results from bc43d.maple *******************


! ************ solution using extrapolation for a1.u *******************
      gIII1=-tau11*(c22*uss+c2*us+c33*utt+c3*ut)-tau12*(c22*vss+c2*vs+
     & c33*vtt+c3*vt)-tau13*(c22*wss+c2*ws+c33*wtt+c3*wt)

      gIII2=-tau21*(c22*uss+c2*us+c33*utt+c3*ut)-tau22*(c22*vss+c2*vs+
     & c33*vtt+c3*vt)-tau23*(c22*wss+c2*ws+c33*wtt+c3*wt)

      tau1U=tau11*u(i1,i2,i3,ex)+tau12*u(i1,i2,i3,ey)+tau13*u(i1,i2,i3,
     & ez)

      tau1Up1=tau11*u(i1+is1,i2+is2,i3+is3,ex)+tau12*u(i1+is1,i2+is2,
     & i3+is3,ey)+tau13*u(i1+is1,i2+is2,i3+is3,ez)

      tau1Up2=tau11*u(i1+2*is1,i2+2*is2,i3+2*is3,ex)+tau12*u(i1+2*is1,
     & i2+2*is2,i3+2*is3,ey)+tau13*u(i1+2*is1,i2+2*is2,i3+2*is3,ez)

      tau1Up3=tau11*u(i1+3*is1,i2+3*is2,i3+3*is3,ex)+tau12*u(i1+3*is1,
     & i2+3*is2,i3+3*is3,ey)+tau13*u(i1+3*is1,i2+3*is2,i3+3*is3,ez)

      tau2U=tau21*u(i1,i2,i3,ex)+tau22*u(i1,i2,i3,ey)+tau23*u(i1,i2,i3,
     & ez)

      tau2Up1=tau21*u(i1+is1,i2+is2,i3+is3,ex)+tau22*u(i1+is1,i2+is2,
     & i3+is3,ey)+tau23*u(i1+is1,i2+is2,i3+is3,ez)

      tau2Up2=tau21*u(i1+2*is1,i2+2*is2,i3+2*is3,ex)+tau22*u(i1+2*is1,
     & i2+2*is2,i3+2*is3,ey)+tau23*u(i1+2*is1,i2+2*is2,i3+2*is3,ez)

      tau2Up3=tau21*u(i1+3*is1,i2+3*is2,i3+3*is3,ex)+tau22*u(i1+3*is1,
     & i2+3*is2,i3+3*is3,ey)+tau23*u(i1+3*is1,i2+3*is2,i3+3*is3,ez)

! tau1.D+^p u = 0
      gIV1=-10*tau1U+10*tau1Up1-5*tau1Up2+tau1Up3 +gIVf1

! tau2.D+^p u = 0
      gIV2=-10*tau2U+10*tau2Up1-5*tau2Up2+tau2Up3 +gIVf2


! ttu11 = tau1.u(-1), ttu12 = tau1.u(-2)
      ttu11=-(-12*c11*tau1Up1+24*c11*tau1U+c11*ctlrr*tau1Up2-4*c11*
     & ctlrr*tau1Up1+6*c11*ctlrr*tau1U+c11*ctlrr*gIV1-6*c1*dra*
     & tau1Up1+c1*dra*ctlr*tau1Up2-2*c1*dra*ctlr*tau1Up1-c1*dra*ctlr*
     & gIV1+12*gIII1*dra**2+12*tau1DotUtt*dra**2)/(-12*c11+c11*ctlrr+
     & 6*c1*dra-3*c1*dra*ctlr)
      ttu12=-(-60*c11*tau1Up1+120*c11*tau1U+5*c11*ctlrr*tau1Up2-20*c11*
     & ctlrr*tau1Up1+30*c11*ctlrr*tau1U+4*c11*ctlrr*gIV1-30*c1*dra*
     & tau1Up1+5*c1*dra*ctlr*tau1Up2-10*c1*dra*ctlr*tau1Up1-2*c1*dra*
     & ctlr*gIV1+60*gIII1*dra**2+60*tau1DotUtt*dra**2+12*c11*gIV1-6*
     & gIV1*c1*dra)/(-12*c11+c11*ctlrr+6*c1*dra-3*c1*dra*ctlr)

! ttu21 = tau2.u(-1), ttu22 = tau2.u(-2)
      ttu21=-(-12*c11*tau2Up1+24*c11*tau2U+c11*ctlrr*tau2Up2-4*c11*
     & ctlrr*tau2Up1+6*c11*ctlrr*tau2U+c11*ctlrr*gIV2-6*c1*dra*
     & tau2Up1+c1*dra*ctlr*tau2Up2-2*c1*dra*ctlr*tau2Up1-c1*dra*ctlr*
     & gIV2+12*gIII2*dra**2+12*tau2DotUtt*dra**2)/(-12*c11+c11*ctlrr+
     & 6*c1*dra-3*c1*dra*ctlr)
      ttu22=-(-60*c11*tau2Up1+120*c11*tau2U+5*c11*ctlrr*tau2Up2-20*c11*
     & ctlrr*tau2Up1+30*c11*ctlrr*tau2U+4*c11*ctlrr*gIV2-30*c1*dra*
     & tau2Up1+5*c1*dra*ctlr*tau2Up2-10*c1*dra*ctlr*tau2Up1-2*c1*dra*
     & ctlr*gIV2+60*gIII2*dra**2+60*tau2DotUtt*dra**2+12*c11*gIV2-6*
     & gIV2*c1*dra)/(-12*c11+c11*ctlrr+6*c1*dra-3*c1*dra*ctlr)

      ! *********** set tangential components to be exact *****
      ! OGF3D(i1-is1,i2-is2,i3-is3,t, uvm(0),uvm(1),uvm(2))
      ! ttu11=tau11*uvm(0)+tau12*uvm(1)+tau13*uvm(2)
      ! ttu21=tau21*uvm(0)+tau22*uvm(1)+tau23*uvm(2)
      ! OGF3D(i1-2*is1,i2-2*is2,i3-2*is3,t, uvm(0),uvm(1),uvm(2))
      ! ttu12=tau11*uvm(0)+tau12*uvm(1)+tau13*uvm(2)
      ! ttu22=tau21*uvm(0)+tau22*uvm(1)+tau23*uvm(2)
      ! ******************************************************

      f1f  =a11*(15.*u(i1,i2,i3,ex)-20.*u(i1+is1,i2+is2,i3+is3,ex)+15.*
     & u(i1+2*is1,i2+2*is2,i3+2*is3,ex)-6.*u(i1+3*is1,i2+3*is2,i3+3*
     & is3,ex)+u(i1+4*is1,i2+4*is2,i3+4*is3,ex))+a12*(15.*u(i1,i2,i3,
     & ey)-20.*u(i1+is1,i2+is2,i3+is3,ey)+15.*u(i1+2*is1,i2+2*is2,i3+
     & 2*is3,ey)-6.*u(i1+3*is1,i2+3*is2,i3+3*is3,ey)+u(i1+4*is1,i2+4*
     & is2,i3+4*is3,ey))+a13*(15.*u(i1,i2,i3,ez)-20.*u(i1+is1,i2+is2,
     & i3+is3,ez)+15.*u(i1+2*is1,i2+2*is2,i3+2*is3,ez)-6.*u(i1+3*is1,
     & i2+3*is2,i3+3*is3,ez)+u(i1+4*is1,i2+4*is2,i3+4*is3,ez))

      f2um2=1/12.*a11m2
      f2um1=-2/3.*a11m1
      f2vm2=1/12.*a12m2
      f2vm1=-2/3.*a12m1
      f2wm2=1/12.*a13m2
      f2wm1=-2/3.*a13m1
      f2f  =2/3.*a11p1*u(i1+is1,i2+is2,i3+is3,ex)+2/3.*a12p1*u(i1+is1,
     & i2+is2,i3+is3,ey)+2/3.*a13p1*u(i1+is1,i2+is2,i3+is3,ez)-1/12.*
     & a11p2*u(i1+2*is1,i2+2*is2,i3+2*is3,ex)-1/12.*a12p2*u(i1+2*is1,
     & i2+2*is2,i3+2*is3,ey)-1/12.*a13p2*u(i1+2*is1,i2+2*is2,i3+2*is3,
     & ez)-Da1DotU*dra

      u(i1-is1,i2-is2,i3-is3,ez) = -1.*(tau21*f1f*f2um2*tau23*tau12**2-
     & 1.*tau12*tau22*tau13*tau21*f1f*f2um2+f1f*tau22*f2vm2*tau23*
     & tau11**2-1.*f1f*tau22**2*f2wm2*tau11**2+tau11*tau13*f1f*tau22**
     & 2*f2um2-1.*tau11*f1f*tau23*tau12*f2vm2*tau21-1.*tau11*tau13*
     & f1f*tau22*f2vm2*tau21-1.*tau11*f1f*tau22*f2um2*tau23*tau12+2.*
     & tau11*f1f*tau22*f2wm2*tau12*tau21+tau12*tau13*tau21**2*f1f*
     & f2vm2-1.*tau21**2*f1f*f2wm2*tau12**2+ttu12*f2vm2*tau21**2*a13*
     & tau12-1.*ttu12*f2vm2*tau21*tau23*a11*tau12-1.*ttu12*tau22*
     & f2vm2*tau21*a13*tau11+ttu12*tau22**2*f2um2*a13*tau11-1.*ttu12*
     & tau22*f2um2*a13*tau21*tau12-1.*ttu12*tau22*f2um2*tau23*a12*
     & tau11+f2um1*ttu21*tau21*tau12**2*a13+tau21**2*f2f*tau12**2*a13+
     & f2um2*a13*tau12**2*tau21*ttu22-1.*tau12*tau22*tau21*a13*ttu11*
     & f2um1-1.*tau21*f2f*tau23*tau12**2*a11-1.*tau12*tau21*a13*ttu21*
     & tau11*f2vm1-1.*tau12*tau21*tau23*a11*ttu11*f2vm1-1.*tau12*
     & tau22*ttu22*f2um2*a13*tau11-1.*tau12*tau13*f2um1*ttu21*tau21*
     & a12-1.*tau12*tau13*f2um2*a12*tau21*ttu22+tau12*tau21**2*a13*
     & ttu11*f2vm1+tau12*tau22*tau13*tau21*f2f*a11-1.*tau12*tau13*
     & tau21**2*f2f*a12+tau11*a13*ttu11*tau22**2*f2um1-1.*tau11*tau13*
     & tau22*f2vm2*ttu22*a11-1.*tau11*tau13*a11*tau22**2*f2f+tau11*
     & tau22*f2f*tau23*tau12*a11-1.*tau11*tau13*a11*ttu21*tau22*f2vm1-
     & 1.*tau11*tau21*a13*ttu11*tau22*f2vm1+tau11*tau21*tau13*a12*
     & tau22*f2f-2.*tau11*tau21*a13*tau12*tau22*f2f-1.*tau11*a13*
     & tau12*tau22*f2um1*ttu21-1.*tau11*a12*tau22*tau23*f2um1*ttu11+
     & tau11*tau21*a12*tau23*tau12*f2f+a13*tau22**2*tau11**2*f2f+
     & tau22*f2vm2*ttu22*a13*tau11**2-1.*a12*tau23*tau11**2*tau22*f2f-
     & 1.*tau11*tau21*ttu22*f2vm2*a13*tau12+a13*ttu21*tau11**2*tau22*
     & f2vm1+6.*ttu21*f2wm2*tau12**2*tau21*a11+tau22*tau11*f2wm2*
     & tau12*ttu22*a11+tau21*tau23*ttu11*f2vm1*tau11*a12+tau21*tau13*
     & a11*ttu11*tau22*f2vm1+tau22*f2vm2*tau23*tau11*ttu12*a11-1.*
     & tau21*tau12**2*f2wm2*ttu22*a11+tau21*a11*tau12*tau22*f2wm2*
     & ttu12-1.*ttu21*tau23*f2vm1*tau11**2*a12-6.*ttu21*f2vm2*tau23*
     & tau11**2*a12-6.*ttu21*f2um2*tau23*a11*tau12**2+ttu21*tau23*
     & f2vm1*tau11*tau12*a11-6.*ttu21*tau22*f2wm2*tau11*a11*tau12+6.*
     & ttu21*f2um2*tau23*a12*tau11*tau12+6.*ttu21*f2vm2*tau23*tau11*
     & a11*tau12-6.*tau21*ttu11*tau22*f2wm2*tau11*a12+6.*tau21*tau13*
     & ttu11*tau22*f2um2*a12+tau21*tau13*ttu21*f2vm1*tau11*a12+tau21*
     & a12*tau12*f2wm2*tau11*ttu22+tau22*tau13*f2um2*a12*tau11*ttu22+
     & 6.*tau22*tau13*f2um2*a11*ttu21*tau12+6.*tau22*tau13*f2vm2*
     & tau21*a11*ttu11-1.*tau22*f2wm2*tau11**2*a12*ttu22+tau22*tau13*
     & f2um1*ttu11*tau21*a12+tau22*tau13*f2um1*ttu21*tau12*a11-6.*
     & ttu21*tau12*f2wm2*tau11*a12*tau21-6.*ttu21*tau13*f2vm2*tau21*
     & a11*tau12+6.*tau22*f2um2*tau23*a11*ttu11*tau12-6.*tau22*f2wm2*
     & tau12*tau21*a11*ttu11-6.*tau22*f2vm2*tau23*tau11*a11*ttu11+
     & tau22*tau23*f2um1*ttu11*tau12*a11+tau22*tau21*f2wm2*ttu12*a12*
     & tau11+tau21*tau13*tau12*f2vm2*ttu22*a11-6.*tau21**2*tau13*
     & ttu11*f2vm2*a12+6.*tau21**2*ttu11*f2wm2*tau12*a12-1.*tau21**2*
     & tau13*ttu11*f2vm1*a12-1.*tau21**2*ttu12*f2wm2*tau12*a12+6.*
     & tau21*tau13*ttu21*f2vm2*a12*tau11-6.*ttu21*tau22*tau13*f2um2*
     & a12*tau11-1.*ttu21*tau23*f2um1*tau12**2*a11+ttu21*tau23*f2um1*
     & tau12*a12*tau11+6.*ttu21*tau22*f2wm2*tau11**2*a12-6.*tau22**2*
     & tau13*f2um2*a11*ttu11-1.*tau22**2*tau13*f2um1*ttu11*a11-1.*
     & tau22**2*f2wm2*ttu12*tau11*a11+6.*tau22**2*f2wm2*tau11*a11*
     & ttu11+tau21*tau23*ttu12*f2um2*a12*tau12+6.*tau21*tau23*ttu11*
     & f2vm2*a12*tau11-6.*tau21*tau23*ttu11*f2um2*a12*tau12)/(tau23*
     & tau12*f2vm1*tau21*a13*tau11+6.*tau23*tau12*f2wm2*tau11*a12*
     & tau21+tau23*tau12*f2wm1*tau11*tau21*a12-1.*f2wm1*tau21*tau12**
     & 2*tau23*a11+6.*tau23*tau12*f2vm2*tau21*a13*tau11-1.*tau23*
     & f2um1*tau12**2*tau21*a13+6.*tau13*f2vm2*tau21*tau23*a11*tau12-
     & 2.*tau13*tau23*f2vm1*tau11*tau21*a12-6.*f2wm2*tau12**2*tau21*
     & tau23*a11-6.*tau13*f2vm2*tau21**2*a13*tau12+tau13*f2vm1*tau21*
     & tau23*tau12*a11-1.*tau23**2*f2um1*tau12*a12*tau11+tau13*tau23*
     & f2um1*tau12*tau21*a12-12.*tau13*f2vm2*tau21*tau23*a12*tau11-6.*
     & tau13*f2wm2*tau12*tau21**2*a12-1.*tau22**2*tau13*f2um1*a13*
     & tau11+tau22*f2wm1*tau11*tau23*tau12*a11-6.*tau22*f2wm2*tau11**
     & 2*tau23*a12-6.*tau22*tau13**2*f2um2*a12*tau21-1.*tau22*tau13**
     & 2*f2um1*tau21*a12-6.*tau22*tau13**2*f2vm2*tau21*a11-1.*tau22*
     & tau13**2*f2vm1*tau21*a11-1.*tau22*f2wm1*tau11**2*a12*tau23-1.*
     & tau22*tau23*f2vm1*tau11**2*a13-12.*tau22*tau13*f2um2*tau23*a11*
     & tau12-1.*tau13*f2vm1*tau21**2*tau12*a13-2.*tau22*f2wm1*tau21*
     & tau12*a13*tau11+tau22*tau23*f2um1*tau12*a13*tau11-6.*f2um2*
     & tau23**2*a12*tau11*tau12-6.*f2um2*a13*tau21*tau12**2*tau23-6.*
     & f2vm2*tau23**2*tau11*a11*tau12+6.*tau13*f2um2*a12*tau21*tau23*
     & tau12+6.*tau22*tau13*f2um2*a13*tau21*tau12+6.*tau22*tau13*
     & f2wm2*tau12*tau21*a11+tau13**2*f2vm1*tau21**2*a12+tau22*tau13*
     & f2vm1*tau21*a13*tau11+tau22*tau13*f2wm1*tau11*tau21*a12+6.*
     & tau13**2*f2vm2*tau21**2*a12-6.*tau22**2*tau13*f2um2*a13*tau11+
     & tau22*tau13*f2wm1*tau21*tau12*a11+6.*tau22*tau13*f2vm2*tau21*
     & a13*tau11-2.*tau22*tau13*f2um1*a11*tau23*tau12+tau22*tau13*
     & f2um1*a12*tau11*tau23-1.*tau22**2*tau13*f2wm1*tau11*a11+tau22*
     & tau13*f2um1*tau21*tau12*a13-1.*tau23**2*f2vm1*tau11*tau12*a11+
     & 6.*tau22*f2wm2*tau11*tau23*a11*tau12-1.*tau13*f2wm1*tau21**2*
     & tau12*a12-6.*tau22*f2vm2*tau23*tau11**2*a13-6.*tau22**2*tau13*
     & f2wm2*tau11*a11+tau23**2*f2vm1*tau11**2*a12+6.*f2vm2*tau23**2*
     & tau11**2*a12+6.*f2um2*tau23**2*a11*tau12**2+6.*tau22*f2um2*a13*
     & tau11*tau23*tau12-12.*tau22*f2wm2*tau12*tau21*a13*tau11+6.*
     & tau22*tau13*f2wm2*tau11*a12*tau21+tau22*tau13*tau23*f2vm1*
     & tau11*a11+6.*tau22*tau13*f2vm2*tau23*tau11*a11+6.*tau22*tau13*
     & f2um2*tau23*a12*tau11+tau22**2*tau13**2*f2um1*a11+6.*f2wm2*
     & tau12**2*tau21**2*a13+tau23**2*f2um1*tau12**2*a11+f2wm1*tau21**
     & 2*tau12**2*a13+6.*tau22**2*f2wm2*tau11**2*a13+tau22**2*f2wm1*
     & tau11**2*a13+6.*tau22**2*tau13**2*f2um2*a11)

      u(i1-2*is1,i2-2*is2,i3-2*is3,ez) = (-1.*tau21**2*f2wm1*tau12**2*
     & f1f-1.*f2wm1*tau11**2*tau22**2*f1f+f2vm1*tau23*tau11**2*f1f*
     & tau22+tau11*tau13*f2um1*tau22**2*f1f-1.*tau11*tau21*f2vm1*f1f*
     & tau23*tau12-1.*tau11*tau21*tau13*f2vm1*f1f*tau22-1.*tau11*
     & f2um1*tau23*tau12*f1f*tau22+tau12*tau13*tau21**2*f2vm1*f1f-1.*
     & tau12*tau22*tau13*tau21*f2um1*f1f+tau21*f2um1*tau23*tau12**2*
     & f1f+2.*tau11*tau21*f2wm1*tau12*f1f*tau22-1.*ttu12*tau22*tau13*
     & f2um1*tau21*a12-6.*ttu12*tau22*tau13*f2vm2*tau21*a11+ttu12*
     & tau23*f2um1*tau12*tau21*a12-6.*ttu12*tau22*tau13*f2um2*a12*
     & tau21-1.*ttu12*tau22*tau13*f2vm1*tau21*a11-6.*ttu12*f2vm2*
     & tau21**2*a13*tau12+6.*ttu12*f2vm2*tau21*tau23*a11*tau12+6.*
     & ttu12*tau22*f2vm2*tau21*a13*tau11+ttu12*tau22*f2wm1*tau11*
     & tau21*a12+6.*ttu12*tau13*f2vm2*tau21**2*a12-6.*ttu12*tau22**2*
     & f2um2*a13*tau11+6.*ttu12*tau22*f2um2*a13*tau21*tau12+ttu12*
     & tau13*f2vm1*tau21**2*a12+ttu12*tau22*tau23*f2vm1*tau11*a11+
     & ttu12*tau22*f2wm1*tau21*tau12*a11+ttu12*tau22**2*tau13*f2um1*
     & a11-1.*ttu12*f2wm1*tau21**2*tau12*a12-1.*ttu12*tau22**2*f2wm1*
     & tau11*a11+6.*ttu12*tau22**2*tau13*f2um2*a11+6.*ttu12*tau22*
     & f2um2*tau23*a12*tau11-6.*f2um1*ttu21*tau21*tau12**2*a13-6.*
     & tau21**2*f2f*tau12**2*a13-6.*f2um2*a13*tau12**2*tau21*ttu22+6.*
     & tau12*tau22*tau21*a13*ttu11*f2um1+6.*tau21*f2f*tau23*tau12**2*
     & a11-1.*tau12*tau22*tau13*ttu22*f2um1*a11-6.*tau12*tau22*tau13*
     & ttu22*f2um2*a11+tau12*tau22*ttu22*f2wm1*tau11*a11-1.*tau12*
     & tau22*tau23*ttu12*f2um1*a11+6.*tau12*tau21*a13*ttu21*tau11*
     & f2vm1-1.*tau12*ttu22*tau23*f2vm1*tau11*a11+6.*tau12*tau21*
     & tau23*a11*ttu11*f2vm1-6.*tau12*tau22*tau23*ttu12*f2um2*a11+6.*
     & tau12*tau22*ttu22*f2um2*a13*tau11-6.*tau12*tau23*f2um1*ttu11*
     & tau21*a12+6.*tau12*tau13*f2um1*ttu21*tau21*a12-6.*tau12*f2wm1*
     & ttu21*tau11*tau21*a12+6.*tau12*tau13*f2um2*a12*tau21*ttu22-6.*
     & tau12*tau13*tau21*a11*ttu21*f2vm1+tau12*tau13*ttu22*f2vm1*
     & tau21*a11-6.*tau12*tau21**2*a13*ttu11*f2vm1+6.*tau12*tau21**2*
     & f2wm1*ttu11*a12-6.*tau12*tau22*tau13*tau21*f2f*a11-6.*tau12*
     & tau22*tau21*f2wm1*ttu11*a11+6.*tau12*tau13*tau21**2*f2f*a12-6.*
     & tau12*f2um2*a12*tau11*tau23*ttu22-6.*tau11*tau23*a11*ttu11*
     & tau22*f2vm1-1.*tau11*a12*ttu22*tau23*f2um1*tau12-6.*tau11*a13*
     & ttu11*tau22**2*f2um1+6.*tau11*tau13*tau22*f2vm2*ttu22*a11+6.*
     & tau11*tau13*a11*tau22**2*f2f-6.*tau11*tau22*f2f*tau23*tau12*
     & a11+6.*tau11*tau13*a11*ttu21*tau22*f2vm1+6.*tau11*tau21*a13*
     & ttu11*tau22*f2vm1-6.*tau11*tau21*tau13*a12*ttu22*f2vm2-6.*
     & tau11*tau21*tau13*a12*tau22*f2f+12.*tau11*tau21*a13*tau12*
     & tau22*f2f+tau11*tau21*a12*ttu22*f2wm1*tau12-1.*tau11*tau21*
     & tau13*a12*ttu22*f2vm1-6.*tau11*tau13*a12*tau22*f2um1*ttu21+6.*
     & tau11*a13*tau12*tau22*f2um1*ttu21+6.*tau11*a12*tau22*tau23*
     & f2um1*ttu11-6.*tau11*a11*ttu21*tau22*f2wm1*tau12+ttu22*f2um1*
     & a11*tau23*tau12**2-1.*ttu22*f2wm1*tau21*tau12**2*a11+6.*tau21*
     & a11*ttu21*f2wm1*tau12**2+6.*ttu22*f2um2*tau23*a11*tau12**2-6.*
     & tau11*tau21*a12*tau23*tau12*f2f-6.*a13*tau22**2*tau11**2*f2f+
     & a12*ttu22*tau23*f2vm1*tau11**2+6.*a12*tau22*f2wm1*ttu21*tau11**
     & 2-6.*tau22*f2vm2*ttu22*a13*tau11**2+6.*a12*ttu22*f2vm2*tau23*
     & tau11**2-1.*a12*ttu22*tau22*f2wm1*tau11**2+6.*a12*tau23*tau11**
     & 2*tau22*f2f+6.*tau11*a11*tau22**2*f2wm1*ttu11+tau11*tau13*a12*
     & ttu22*tau22*f2um1-6.*tau11*tau23*tau12*f2vm2*ttu22*a11-1.*
     & tau11*tau21*a12*tau23*ttu12*f2vm1-6.*tau11*tau21*a12*tau22*
     & f2wm1*ttu11+6.*tau11*tau21*ttu22*f2vm2*a13*tau12-6.*tau11*
     & tau21*a12*tau23*ttu12*f2vm2-6.*a13*ttu21*tau11**2*tau22*f2vm1)
     & /(tau23*tau12*f2vm1*tau21*a13*tau11+6.*tau23*tau12*f2wm2*tau11*
     & a12*tau21+tau23*tau12*f2wm1*tau11*tau21*a12-1.*f2wm1*tau21*
     & tau12**2*tau23*a11+6.*tau23*tau12*f2vm2*tau21*a13*tau11-1.*
     & tau23*f2um1*tau12**2*tau21*a13+6.*tau13*f2vm2*tau21*tau23*a11*
     & tau12-2.*tau13*tau23*f2vm1*tau11*tau21*a12-6.*f2wm2*tau12**2*
     & tau21*tau23*a11-6.*tau13*f2vm2*tau21**2*a13*tau12+tau13*f2vm1*
     & tau21*tau23*tau12*a11-1.*tau23**2*f2um1*tau12*a12*tau11+tau13*
     & tau23*f2um1*tau12*tau21*a12-12.*tau13*f2vm2*tau21*tau23*a12*
     & tau11-6.*tau13*f2wm2*tau12*tau21**2*a12-1.*tau22**2*tau13*
     & f2um1*a13*tau11+tau22*f2wm1*tau11*tau23*tau12*a11-6.*tau22*
     & f2wm2*tau11**2*tau23*a12-6.*tau22*tau13**2*f2um2*a12*tau21-1.*
     & tau22*tau13**2*f2um1*tau21*a12-6.*tau22*tau13**2*f2vm2*tau21*
     & a11-1.*tau22*tau13**2*f2vm1*tau21*a11-1.*tau22*f2wm1*tau11**2*
     & a12*tau23-1.*tau22*tau23*f2vm1*tau11**2*a13-12.*tau22*tau13*
     & f2um2*tau23*a11*tau12-1.*tau13*f2vm1*tau21**2*tau12*a13-2.*
     & tau22*f2wm1*tau21*tau12*a13*tau11+tau22*tau23*f2um1*tau12*a13*
     & tau11-6.*f2um2*tau23**2*a12*tau11*tau12-6.*f2um2*a13*tau21*
     & tau12**2*tau23-6.*f2vm2*tau23**2*tau11*a11*tau12+6.*tau13*
     & f2um2*a12*tau21*tau23*tau12+6.*tau22*tau13*f2um2*a13*tau21*
     & tau12+6.*tau22*tau13*f2wm2*tau12*tau21*a11+tau13**2*f2vm1*
     & tau21**2*a12+tau22*tau13*f2vm1*tau21*a13*tau11+tau22*tau13*
     & f2wm1*tau11*tau21*a12+6.*tau13**2*f2vm2*tau21**2*a12-6.*tau22**
     & 2*tau13*f2um2*a13*tau11+tau22*tau13*f2wm1*tau21*tau12*a11+6.*
     & tau22*tau13*f2vm2*tau21*a13*tau11-2.*tau22*tau13*f2um1*a11*
     & tau23*tau12+tau22*tau13*f2um1*a12*tau11*tau23-1.*tau22**2*
     & tau13*f2wm1*tau11*a11+tau22*tau13*f2um1*tau21*tau12*a13-1.*
     & tau23**2*f2vm1*tau11*tau12*a11+6.*tau22*f2wm2*tau11*tau23*a11*
     & tau12-1.*tau13*f2wm1*tau21**2*tau12*a12-6.*tau22*f2vm2*tau23*
     & tau11**2*a13-6.*tau22**2*tau13*f2wm2*tau11*a11+tau23**2*f2vm1*
     & tau11**2*a12+6.*f2vm2*tau23**2*tau11**2*a12+6.*f2um2*tau23**2*
     & a11*tau12**2+6.*tau22*f2um2*a13*tau11*tau23*tau12-12.*tau22*
     & f2wm2*tau12*tau21*a13*tau11+6.*tau22*tau13*f2wm2*tau11*a12*
     & tau21+tau22*tau13*tau23*f2vm1*tau11*a11+6.*tau22*tau13*f2vm2*
     & tau23*tau11*a11+6.*tau22*tau13*f2um2*tau23*a12*tau11+tau22**2*
     & tau13**2*f2um1*a11+6.*f2wm2*tau12**2*tau21**2*a13+tau23**2*
     & f2um1*tau12**2*a11+f2wm1*tau21**2*tau12**2*a13+6.*tau22**2*
     & f2wm2*tau11**2*a13+tau22**2*f2wm1*tau11**2*a13+6.*tau22**2*
     & tau13**2*f2um2*a11)

      u(i1-is1,i2-is2,i3-is3,ex) = -1.*(-1.*tau21*tau13*a12*tau22*
     & f2wm1*ttu11+tau21*tau13*a12*tau23*tau12*f2f+6.*tau13*tau22**2*
     & f2um2*a13*ttu11+2.*tau13*f1f*tau22*f2um2*tau23*tau12+tau13**2*
     & f1f*tau22*f2vm2*tau21-1.*tau13*tau23*tau12*f2vm2*ttu22*a11-1.*
     & f1f*tau23*tau12*f2wm2*tau11*tau22+f1f*tau23**2*tau12*f2vm2*
     & tau11-1.*f1f*tau23*tau12*tau13*f2vm2*tau21+f1f*tau23*tau12**2*
     & f2wm2*tau21-1.*tau13*f1f*tau22*f2wm2*tau12*tau21+tau13*f1f*
     & tau22**2*f2wm2*tau11-1.*f1f*tau23**2*tau12**2*f2um2-1.*tau13**
     & 2*f1f*tau22**2*f2um2-1.*tau13*f1f*tau22*f2vm2*tau23*tau11+
     & tau23**2*a11*tau12**2*f2f+tau13*a11*tau22**2*f2wm2*ttu12-1.*
     & a12*tau23**2*tau12*f2f*tau11-1.*a12*tau23*tau12*f2wm2*tau11*
     & ttu22-1.*a12*tau23*tau12*f2wm1*ttu21*tau11+tau23**2*a11*ttu11*
     & tau12*f2vm1+tau23**2*tau12*f2vm2*ttu12*a11+tau23*tau12**2*
     & f2wm2*ttu22*a11+a13*tau12*tau23*tau11*tau22*f2f+a11*ttu21*
     & tau23*tau12**2*f2wm1-1.*tau23*a11*tau12*tau22*f2wm1*ttu11-1.*
     & tau23*a11*tau12*tau22*f2wm2*ttu12-1.*tau13*tau22*f2vm2*ttu22*
     & a13*tau11+tau13*a12*tau23*tau11*tau22*f2f-1.*tau13*tau22*f2vm2*
     & tau23*ttu12*a11-1.*tau13*tau22*f2wm2*tau12*ttu22*a11-1.*tau13*
     & tau23*a11*ttu11*tau22*f2vm1-1.*tau13*a11*ttu21*tau23*tau12*
     & f2vm1-1.*tau13*a13*ttu21*tau11*tau22*f2vm1+tau13*a11*tau22**2*
     & f2wm1*ttu11-1.*tau13*a11*ttu21*tau22*f2wm1*tau12-1.*tau13*a13*
     & tau22**2*tau11*f2f+tau13**2*tau22*f2vm2*ttu22*a11+tau13**2*a11*
     & ttu21*tau22*f2vm1+tau13**2*a11*tau22**2*f2f-2.*tau13*tau22*f2f*
     & tau23*tau12*a11+tau22*f2wm1*ttu21*tau11*tau12*a13+tau23*ttu12*
     & tau22*f2um2*a13*tau12+6.*tau22*f2vm2*tau23*tau11*a13*ttu11-6.*
     & tau22*f2um2*a13*ttu11*tau23*tau12+6.*tau22*f2wm2*tau12*a13*
     & ttu21*tau11+tau23*tau11*a13*ttu11*tau22*f2vm1+6.*tau23*ttu11*
     & tau22*f2wm2*tau11*a12-6.*tau13*tau23*ttu11*tau22*f2um2*a12-6.*
     & tau13*ttu21*f2um2*a12*tau23*tau12+tau13*ttu21*tau23*f2vm1*
     & tau11*a12+6.*tau13*ttu21*f2vm2*tau23*a12*tau11-6.*tau13*ttu21*
     & tau22*f2wm2*tau11*a12+tau13*tau23*ttu12*tau22*f2um2*a12-6.*
     & tau13*ttu21*tau22*f2um2*a13*tau12+tau13*ttu22*tau22*f2um2*a13*
     & tau12+tau13*ttu22*f2um2*a12*tau23*tau12-1.*tau13**2*ttu22*
     & tau22*f2um2*a12+tau13*ttu22*tau22*f2wm2*tau11*a12+tau23*tau11*
     & ttu22*f2vm2*a13*tau12-6.*tau23*tau12*f2vm2*a13*ttu21*tau11+
     & tau23*tau11*a12*tau22*f2wm1*ttu11-6.*tau21*tau13**2*ttu21*
     & f2vm2*a12-1.*tau21*ttu21*f2wm1*tau12**2*a13-6.*tau21*ttu21*
     & f2wm2*tau12**2*a13-1.*tau21*tau13**2*ttu21*f2vm1*a12+tau21*
     & tau13*ttu21*f2vm1*tau12*a13+tau21*tau13*tau22*f2vm2*a13*ttu12+
     & 6.*tau21*tau13*tau23*ttu11*f2vm2*a12+6.*tau21*tau13*ttu21*
     & f2vm2*a13*tau12+6.*tau21*tau13*ttu21*f2wm2*tau12*a12-6.*tau21*
     & tau23*ttu11*f2wm2*tau12*a12+tau21*tau13*tau23*ttu11*f2vm1*a12-
     & 6.*tau21*tau13*tau22*f2vm2*a13*ttu11+6.*tau21*tau22*f2wm2*
     & tau12*a13*ttu11+tau21*tau22*f2wm1*ttu11*tau12*a13+tau21*tau23*
     & ttu12*f2wm2*tau12*a12+tau21*tau13*ttu21*f2wm1*tau12*a12-6.*
     & tau22**2*f2wm2*tau11*a13*ttu11-1.*tau21*tau13**2*a12*tau22*f2f-
     & 1.*tau21*tau13*a12*tau22*f2wm2*ttu12+tau21*tau13*a13*tau12*
     & tau22*f2f-1.*tau22**2*f2wm1*ttu11*a13*tau11-1.*tau23**2*ttu12*
     & f2um2*a12*tau12-1.*ttu22*f2um2*a13*tau12**2*tau23+6.*ttu21*
     & f2um2*a13*tau12**2*tau23-1.*tau21*tau23*tau12**2*f2f*a13-6.*
     & tau23**2*ttu11*f2vm2*a12*tau11+6.*tau23**2*ttu11*f2um2*a12*
     & tau12-1.*tau13*tau22**2*f2um2*a13*ttu12-1.*tau23**2*ttu11*
     & f2vm1*tau11*a12-1.*tau21*a13*ttu11*tau23*tau12*f2vm1-1.*tau21*
     & tau23*tau12*f2vm2*a13*ttu12+6.*tau13**2*ttu21*tau22*f2um2*a12)
     & /(tau23*tau12*f2vm1*tau21*a13*tau11+6.*tau23*tau12*f2wm2*tau11*
     & a12*tau21+tau23*tau12*f2wm1*tau11*tau21*a12-1.*f2wm1*tau21*
     & tau12**2*tau23*a11+6.*tau23*tau12*f2vm2*tau21*a13*tau11-1.*
     & tau23*f2um1*tau12**2*tau21*a13+6.*tau13*f2vm2*tau21*tau23*a11*
     & tau12-2.*tau13*tau23*f2vm1*tau11*tau21*a12-6.*f2wm2*tau12**2*
     & tau21*tau23*a11-6.*tau13*f2vm2*tau21**2*a13*tau12+tau13*f2vm1*
     & tau21*tau23*tau12*a11-1.*tau23**2*f2um1*tau12*a12*tau11+tau13*
     & tau23*f2um1*tau12*tau21*a12-12.*tau13*f2vm2*tau21*tau23*a12*
     & tau11-6.*tau13*f2wm2*tau12*tau21**2*a12-1.*tau22**2*tau13*
     & f2um1*a13*tau11+tau22*f2wm1*tau11*tau23*tau12*a11-6.*tau22*
     & f2wm2*tau11**2*tau23*a12-6.*tau22*tau13**2*f2um2*a12*tau21-1.*
     & tau22*tau13**2*f2um1*tau21*a12-6.*tau22*tau13**2*f2vm2*tau21*
     & a11-1.*tau22*tau13**2*f2vm1*tau21*a11-1.*tau22*f2wm1*tau11**2*
     & a12*tau23-1.*tau22*tau23*f2vm1*tau11**2*a13-12.*tau22*tau13*
     & f2um2*tau23*a11*tau12-1.*tau13*f2vm1*tau21**2*tau12*a13-2.*
     & tau22*f2wm1*tau21*tau12*a13*tau11+tau22*tau23*f2um1*tau12*a13*
     & tau11-6.*f2um2*tau23**2*a12*tau11*tau12-6.*f2um2*a13*tau21*
     & tau12**2*tau23-6.*f2vm2*tau23**2*tau11*a11*tau12+6.*tau13*
     & f2um2*a12*tau21*tau23*tau12+6.*tau22*tau13*f2um2*a13*tau21*
     & tau12+6.*tau22*tau13*f2wm2*tau12*tau21*a11+tau13**2*f2vm1*
     & tau21**2*a12+tau22*tau13*f2vm1*tau21*a13*tau11+tau22*tau13*
     & f2wm1*tau11*tau21*a12+6.*tau13**2*f2vm2*tau21**2*a12-6.*tau22**
     & 2*tau13*f2um2*a13*tau11+tau22*tau13*f2wm1*tau21*tau12*a11+6.*
     & tau22*tau13*f2vm2*tau21*a13*tau11-2.*tau22*tau13*f2um1*a11*
     & tau23*tau12+tau22*tau13*f2um1*a12*tau11*tau23-1.*tau22**2*
     & tau13*f2wm1*tau11*a11+tau22*tau13*f2um1*tau21*tau12*a13-1.*
     & tau23**2*f2vm1*tau11*tau12*a11+6.*tau22*f2wm2*tau11*tau23*a11*
     & tau12-1.*tau13*f2wm1*tau21**2*tau12*a12-6.*tau22*f2vm2*tau23*
     & tau11**2*a13-6.*tau22**2*tau13*f2wm2*tau11*a11+tau23**2*f2vm1*
     & tau11**2*a12+6.*f2vm2*tau23**2*tau11**2*a12+6.*f2um2*tau23**2*
     & a11*tau12**2+6.*tau22*f2um2*a13*tau11*tau23*tau12-12.*tau22*
     & f2wm2*tau12*tau21*a13*tau11+6.*tau22*tau13*f2wm2*tau11*a12*
     & tau21+tau22*tau13*tau23*f2vm1*tau11*a11+6.*tau22*tau13*f2vm2*
     & tau23*tau11*a11+6.*tau22*tau13*f2um2*tau23*a12*tau11+tau22**2*
     & tau13**2*f2um1*a11+6.*f2wm2*tau12**2*tau21**2*a13+tau23**2*
     & f2um1*tau12**2*a11+f2wm1*tau21**2*tau12**2*a13+6.*tau22**2*
     & f2wm2*tau11**2*a13+tau22**2*f2wm1*tau11**2*a13+6.*tau22**2*
     & tau13**2*f2um2*a11)

      u(i1-2*is1,i2-2*is2,i3-2*is3,ex) = (-1.*a12*tau23**2*ttu12*f2um1*
     & tau12-1.*tau21*a13*ttu12*tau22*f2wm1*tau12-6.*tau21*tau13*a12*
     & tau23*ttu12*f2vm2+6.*tau21*tau13*a12*tau22*f2wm1*ttu11-6.*
     & tau21*tau13*ttu22*f2vm2*a13*tau12-6.*tau21*tau13*a12*tau23*
     & tau12*f2f-6.*tau21*tau13*a12*ttu22*f2wm2*tau12-1.*tau21*tau13*
     & a12*tau23*ttu12*f2vm1-6.*tau21*a12*tau23*tau12*f2wm1*ttu11+
     & tau21*ttu22*f2wm1*tau12**2*a13-1.*tau13**2*a12*ttu22*tau22*
     & f2um1+6.*tau13*tau23*tau12*f2vm2*ttu22*a11+a13*ttu12*tau22**2*
     & f2wm1*tau11-6.*tau23**2*a11*tau12**2*f2f+2.*tau13*f2um1*tau23*
     & tau12*f1f*tau22-1.*tau13*f2vm1*tau23*tau11*f1f*tau22-6.*tau13*
     & a11*tau22**2*f2wm2*ttu12+6.*a12*tau23**2*tau12*f2f*tau11+6.*
     & a12*tau23*tau12*f2wm2*tau11*ttu22-1.*a12*tau23*ttu12*tau22*
     & f2wm1*tau11-6.*a12*tau23*tau11*tau22*f2wm2*ttu12+6.*a12*tau23*
     & tau12*f2wm1*ttu21*tau11+6.*a12*tau11*tau23**2*ttu12*f2vm2+6.*
     & a12*tau23**2*tau12*f2um1*ttu11+a12*tau11*tau23**2*ttu12*f2vm1-
     & 6.*tau23**2*a11*ttu11*tau12*f2vm1-1.*ttu22*tau23*f2um1*tau12**
     & 2*a13+ttu22*tau23*tau12*f2vm1*a13*tau11+f2vm1*tau23**2*tau11*
     & f1f*tau12-1.*f2um1*tau23**2*tau12**2*f1f-1.*tau13**2*f2um1*
     & tau22**2*f1f-6.*tau23**2*tau12*f2vm2*ttu12*a11+6.*tau23*tau12**
     & 2*f2um1*ttu21*a13-1.*f2wm1*tau12*f1f*tau22*tau23*tau11-6.*
     & tau23*tau12**2*f2wm2*ttu22*a11-6.*a13*tau12*tau23*tau11*tau22*
     & f2f-6.*a11*ttu21*tau23*tau12**2*f2wm1+a13*tau12*tau23*ttu12*
     & tau22*f2um1-6.*tau23*tau11*tau22*f2vm2*a13*ttu12-6.*a13*tau12*
     & tau22*tau23*f2um1*ttu11+6.*tau23*a11*tau12*tau22*f2wm1*ttu11+
     & 6.*tau23*a11*tau12*tau22*f2wm2*ttu12-1.*ttu22*tau22*f2wm1*
     & tau12*a13*tau11-6.*a13*tau12*tau22*f2wm2*tau11*ttu22-1.*tau23*
     & ttu12*tau22*f2vm1*a13*tau11+tau13*a12*tau23*ttu12*tau22*f2um1-
     & 6.*tau13*a12*tau22*tau23*f2um1*ttu11-6.*tau13*a12*tau23*tau12*
     & f2um1*ttu21-1.*tau13*a12*ttu22*tau23*f2vm1*tau11+tau13*a12*
     & ttu22*tau23*f2um1*tau12-6.*tau13*a12*tau22*f2wm1*ttu21*tau11+
     & 6.*tau13*tau22*f2vm2*ttu22*a13*tau11-6.*tau13*a12*ttu22*f2vm2*
     & tau23*tau11+tau13*a12*ttu22*tau22*f2wm1*tau11-6.*tau13*a12*
     & tau23*tau11*tau22*f2f+6.*tau13*tau22*f2vm2*tau23*ttu12*a11-6.*
     & tau13*a13*tau12*tau22*f2um1*ttu21+6.*tau13*tau22*f2wm2*tau12*
     & ttu22*a11+6.*tau13*tau23*a11*ttu11*tau22*f2vm1+tau13*ttu22*
     & tau22*f2um1*tau12*a13+6.*tau13*a11*ttu21*tau23*tau12*f2vm1+6.*
     & tau13*a13*ttu21*tau11*tau22*f2vm1-6.*tau13*a11*tau22**2*f2wm1*
     & ttu11+6.*tau13*a11*ttu21*tau22*f2wm1*tau12+6.*tau13*a13*tau22**
     & 2*tau11*f2f+6.*tau13*a13*ttu11*tau22**2*f2um1-1.*tau13*a13*
     & ttu12*tau22**2*f2um1-6.*a13*ttu21*tau11*tau23*tau12*f2vm1-6.*
     & tau13**2*tau22*f2vm2*ttu22*a11+6.*tau13**2*a12*tau22*f2um1*
     & ttu21-6.*tau13**2*a11*ttu21*tau22*f2vm1-6.*tau13**2*a11*tau22**
     & 2*f2f+tau13*f2wm1*tau11*tau22**2*f1f+tau21*f2wm1*tau12**2*f1f*
     & tau23+12.*tau13*tau22*f2f*tau23*tau12*a11+tau21*tau13**2*a12*
     & ttu22*f2vm1+6.*tau21*tau13**2*a12*tau22*f2f+tau21*a12*tau23*
     & ttu12*f2wm1*tau12-1.*tau21*tau13*a12*ttu22*f2wm1*tau12-1.*
     & tau21*tau13*ttu22*f2vm1*tau12*a13+tau21*tau13*a13*ttu12*tau22*
     & f2vm1-6.*tau21*tau13*a13*ttu11*tau22*f2vm1+6.*tau21*tau13*a12*
     & tau22*f2wm2*ttu12-6.*tau21*tau13*a13*tau12*tau22*f2f+6.*tau21*
     & tau13**2*a12*ttu22*f2vm2+6.*tau21*tau23*tau12**2*f2f*a13+6.*
     & tau21*ttu22*f2wm2*tau12**2*a13+6.*tau21*a13*ttu11*tau23*tau12*
     & f2vm1-6.*tau21*a13*tau12*tau22*f2wm2*ttu12+6.*tau21*tau23*
     & tau12*f2vm2*a13*ttu12-1.*tau21*tau13*f2wm1*tau12*f1f*tau22-1.*
     & tau21*tau13*f2vm1*f1f*tau23*tau12+tau21*tau13**2*f2vm1*f1f*
     & tau22+6.*a13*tau22**2*tau11*f2wm2*ttu12)/(tau23*tau12*f2vm1*
     & tau21*a13*tau11+6.*tau23*tau12*f2wm2*tau11*a12*tau21+tau23*
     & tau12*f2wm1*tau11*tau21*a12-1.*f2wm1*tau21*tau12**2*tau23*a11+
     & 6.*tau23*tau12*f2vm2*tau21*a13*tau11-1.*tau23*f2um1*tau12**2*
     & tau21*a13+6.*tau13*f2vm2*tau21*tau23*a11*tau12-2.*tau13*tau23*
     & f2vm1*tau11*tau21*a12-6.*f2wm2*tau12**2*tau21*tau23*a11-6.*
     & tau13*f2vm2*tau21**2*a13*tau12+tau13*f2vm1*tau21*tau23*tau12*
     & a11-1.*tau23**2*f2um1*tau12*a12*tau11+tau13*tau23*f2um1*tau12*
     & tau21*a12-12.*tau13*f2vm2*tau21*tau23*a12*tau11-6.*tau13*f2wm2*
     & tau12*tau21**2*a12-1.*tau22**2*tau13*f2um1*a13*tau11+tau22*
     & f2wm1*tau11*tau23*tau12*a11-6.*tau22*f2wm2*tau11**2*tau23*a12-
     & 6.*tau22*tau13**2*f2um2*a12*tau21-1.*tau22*tau13**2*f2um1*
     & tau21*a12-6.*tau22*tau13**2*f2vm2*tau21*a11-1.*tau22*tau13**2*
     & f2vm1*tau21*a11-1.*tau22*f2wm1*tau11**2*a12*tau23-1.*tau22*
     & tau23*f2vm1*tau11**2*a13-12.*tau22*tau13*f2um2*tau23*a11*tau12-
     & 1.*tau13*f2vm1*tau21**2*tau12*a13-2.*tau22*f2wm1*tau21*tau12*
     & a13*tau11+tau22*tau23*f2um1*tau12*a13*tau11-6.*f2um2*tau23**2*
     & a12*tau11*tau12-6.*f2um2*a13*tau21*tau12**2*tau23-6.*f2vm2*
     & tau23**2*tau11*a11*tau12+6.*tau13*f2um2*a12*tau21*tau23*tau12+
     & 6.*tau22*tau13*f2um2*a13*tau21*tau12+6.*tau22*tau13*f2wm2*
     & tau12*tau21*a11+tau13**2*f2vm1*tau21**2*a12+tau22*tau13*f2vm1*
     & tau21*a13*tau11+tau22*tau13*f2wm1*tau11*tau21*a12+6.*tau13**2*
     & f2vm2*tau21**2*a12-6.*tau22**2*tau13*f2um2*a13*tau11+tau22*
     & tau13*f2wm1*tau21*tau12*a11+6.*tau22*tau13*f2vm2*tau21*a13*
     & tau11-2.*tau22*tau13*f2um1*a11*tau23*tau12+tau22*tau13*f2um1*
     & a12*tau11*tau23-1.*tau22**2*tau13*f2wm1*tau11*a11+tau22*tau13*
     & f2um1*tau21*tau12*a13-1.*tau23**2*f2vm1*tau11*tau12*a11+6.*
     & tau22*f2wm2*tau11*tau23*a11*tau12-1.*tau13*f2wm1*tau21**2*
     & tau12*a12-6.*tau22*f2vm2*tau23*tau11**2*a13-6.*tau22**2*tau13*
     & f2wm2*tau11*a11+tau23**2*f2vm1*tau11**2*a12+6.*f2vm2*tau23**2*
     & tau11**2*a12+6.*f2um2*tau23**2*a11*tau12**2+6.*tau22*f2um2*a13*
     & tau11*tau23*tau12-12.*tau22*f2wm2*tau12*tau21*a13*tau11+6.*
     & tau22*tau13*f2wm2*tau11*a12*tau21+tau22*tau13*tau23*f2vm1*
     & tau11*a11+6.*tau22*tau13*f2vm2*tau23*tau11*a11+6.*tau22*tau13*
     & f2um2*tau23*a12*tau11+tau22**2*tau13**2*f2um1*a11+6.*f2wm2*
     & tau12**2*tau21**2*a13+tau23**2*f2um1*tau12**2*a11+f2wm1*tau21**
     & 2*tau12**2*a13+6.*tau22**2*f2wm2*tau11**2*a13+tau22**2*f2wm1*
     & tau11**2*a13+6.*tau22**2*tau13**2*f2um2*a11)

      u(i1-is1,i2-is2,i3-is3,ey) = (-1.*tau23**2*tau11*f1f*f2um2*tau12+
     & tau23*tau12*tau21*f1f*f2wm2*tau11+tau13**2*tau21**2*f1f*f2vm2+
     & tau23**2*tau11**2*f1f*f2vm2-1.*tau13*tau21**2*f1f*f2wm2*tau12+
     & tau13*tau21*f1f*f2um2*tau23*tau12-1.*tau22*tau23*tau11**2*f1f*
     & f2wm2-2.*tau13*tau21*f1f*f2vm2*tau23*tau11-1.*tau22*tau13**2*
     & tau21*f1f*f2um2+tau22*tau13*tau21*f1f*f2wm2*tau11-6.*tau13**2*
     & f2vm2*tau21*a11*ttu21-1.*tau13**2*f2um2*a12*tau21*ttu22+tau13*
     & f2wm2*tau11*a12*tau21*ttu22+tau13*f2um2*a13*tau12*tau21*ttu22-
     & 1.*tau13*tau21*f2f*tau23*tau12*a11+tau13*tau23*f2um1*ttu11*
     & tau21*a12-1.*tau13*f2vm2*tau21*ttu22*a13*tau11+tau13*f2um1*
     & ttu21*a12*tau11*tau23+tau13*f2um1*ttu21*tau21*tau12*a13-1.*
     & tau13*f2um1*ttu21*tau23*tau12*a11+tau13*tau21**2*f2f*tau12*a13-
     & 1.*tau13*tau21**2*f2wm2*ttu12*a12+6.*tau13*f2vm2*tau21*a13*
     & ttu21*tau11-1.*tau13*f2vm2*tau21*tau23*ttu12*a11-1.*tau13*
     & f2wm2*tau12*tau21*ttu22*a11-1.*tau13*f2vm2*tau23*tau11*ttu22*
     & a11+tau13*f2wm1*ttu21*tau11*tau21*a12+tau13*f2vm2*tau21**2*a13*
     & ttu12+tau22*tau13*tau23*tau11*f1f*f2um2-1.*tau23*tau12*tau21*
     & f2f*a13*tau11-6.*tau13*f2vm2*tau21**2*a13*ttu11+2.*tau13*tau21*
     & f2f*a12*tau11*tau23+6.*tau13*f2vm2*tau23*tau11*a11*ttu21+tau13*
     & f2um2*a12*tau21*tau23*ttu12+tau13*f2um2*a12*tau11*tau23*ttu22+
     & 6.*tau13*f2wm2*tau12*tau21*a11*ttu21-6.*tau13*f2um2*a11*ttu21*
     & tau23*tau12+6.*tau13*f2vm2*tau21*tau23*a11*ttu11-1.*tau13**2*
     & f2um1*ttu21*tau21*a12+tau13**2*f2vm2*tau21*ttu22*a11+6.*f2vm2*
     & tau23*tau11*a13*tau21*ttu11+tau23*tau11*f2wm1*tau12*ttu21*a11-
     & 1.*f2vm2*tau21*tau23*ttu12*a13*tau11-1.*f2wm2*tau11**2*a12*
     & tau23*ttu22+f2vm2*tau23*tau11**2*ttu22*a13+tau23**2*f2um1*
     & ttu11*tau12*a11-6.*f2wm2*tau12*tau21*a13*ttu21*tau11-1.*f2wm1*
     & ttu21*tau11*tau21*tau12*a13-1.*f2wm1*ttu21*tau11**2*a12*tau23-
     & 6.*f2vm2*tau23*tau11**2*a13*ttu21-1.*f2um2*tau23*a13*tau12*
     & tau11*ttu22-1.*tau21*f2wm1*ttu11*a11*tau23*tau12+f2vm2*tau23**
     & 2*tau11*ttu12*a11+tau21*f2wm2*ttu12*a12*tau11*tau23+tau23*
     & tau11*f2wm2*tau12*ttu22*a11+6.*f2um2*a13*ttu21*tau11*tau23*
     & tau12-1.*tau23*f2um1*ttu11*tau21*tau12*a13-1.*tau23**2*f2um1*
     & ttu11*a12*tau11+tau23**2*tau11*f2f*tau12*a11+tau21**2*f2wm1*
     & ttu11*tau12*a13+tau21*f2wm1*ttu11*a12*tau11*tau23-1.*tau23**2*
     & tau11**2*f2f*a12-1.*tau13**2*tau21**2*f2f*a12-1.*tau13*tau21**
     & 2*f2wm1*ttu11*a12+6.*f2um2*tau23**2*a11*ttu11*tau12+6.*f2wm2*
     & tau12*tau21**2*a13*ttu11-6.*f2wm2*tau12*tau21*tau23*a11*ttu11-
     & 1.*f2um2*a12*tau11*tau23**2*ttu12-6.*f2um2*a13*tau21*ttu11*
     & tau23*tau12-6.*f2vm2*tau23**2*tau11*a11*ttu11-1.*tau22*tau13*
     & tau21*f2f*a13*tau11-1.*tau22*tau13*tau23*f2um1*ttu11*a11+tau22*
     & tau13*tau21*f2wm2*ttu12*a11-6.*tau22*tau13*f2wm2*tau11*a11*
     & ttu21-1.*tau22*tau23*f2wm2*ttu12*tau11*a11-6.*tau22*tau13*
     & f2um2*a13*ttu21*tau11+6.*tau22*f2wm2*tau11**2*a13*ttu21+tau22*
     & f2wm1*ttu21*tau11**2*a13+tau22*tau23*tau11**2*f2f*a13+tau22*
     & tau13**2*f2um1*ttu21*a11+tau22*tau13**2*tau21*f2f*a11+6.*tau22*
     & tau13**2*f2um2*a11*ttu21+6.*tau22*f2wm2*tau11*tau23*a11*ttu11-
     & 1.*tau22*tau13*tau23*tau11*f2f*a11-1.*tau22*tau13*f2wm1*ttu21*
     & tau11*a11-1.*tau22*tau13*f2um1*ttu21*a13*tau11-1.*tau22*tau13*
     & f2um2*a13*ttu12*tau21+tau22*tau13*tau21*f2wm1*ttu11*a11-6.*
     & tau22*f2wm2*tau11*a13*tau21*ttu11+tau22*tau23*f2um1*ttu11*a13*
     & tau11+tau22*f2um2*tau23*a13*ttu12*tau11+6.*tau22*tau13*f2um2*
     & a13*tau21*ttu11-1.*tau22*tau21*f2wm1*ttu11*a13*tau11-6.*tau22*
     & tau13*f2um2*tau23*a11*ttu11)/(tau23*tau12*f2vm1*tau21*a13*
     & tau11+6.*tau23*tau12*f2wm2*tau11*a12*tau21+tau23*tau12*f2wm1*
     & tau11*tau21*a12-1.*f2wm1*tau21*tau12**2*tau23*a11+6.*tau23*
     & tau12*f2vm2*tau21*a13*tau11-1.*tau23*f2um1*tau12**2*tau21*a13+
     & 6.*tau13*f2vm2*tau21*tau23*a11*tau12-2.*tau13*tau23*f2vm1*
     & tau11*tau21*a12-6.*f2wm2*tau12**2*tau21*tau23*a11-6.*tau13*
     & f2vm2*tau21**2*a13*tau12+tau13*f2vm1*tau21*tau23*tau12*a11-1.*
     & tau23**2*f2um1*tau12*a12*tau11+tau13*tau23*f2um1*tau12*tau21*
     & a12-12.*tau13*f2vm2*tau21*tau23*a12*tau11-6.*tau13*f2wm2*tau12*
     & tau21**2*a12-1.*tau22**2*tau13*f2um1*a13*tau11+tau22*f2wm1*
     & tau11*tau23*tau12*a11-6.*tau22*f2wm2*tau11**2*tau23*a12-6.*
     & tau22*tau13**2*f2um2*a12*tau21-1.*tau22*tau13**2*f2um1*tau21*
     & a12-6.*tau22*tau13**2*f2vm2*tau21*a11-1.*tau22*tau13**2*f2vm1*
     & tau21*a11-1.*tau22*f2wm1*tau11**2*a12*tau23-1.*tau22*tau23*
     & f2vm1*tau11**2*a13-12.*tau22*tau13*f2um2*tau23*a11*tau12-1.*
     & tau13*f2vm1*tau21**2*tau12*a13-2.*tau22*f2wm1*tau21*tau12*a13*
     & tau11+tau22*tau23*f2um1*tau12*a13*tau11-6.*f2um2*tau23**2*a12*
     & tau11*tau12-6.*f2um2*a13*tau21*tau12**2*tau23-6.*f2vm2*tau23**
     & 2*tau11*a11*tau12+6.*tau13*f2um2*a12*tau21*tau23*tau12+6.*
     & tau22*tau13*f2um2*a13*tau21*tau12+6.*tau22*tau13*f2wm2*tau12*
     & tau21*a11+tau13**2*f2vm1*tau21**2*a12+tau22*tau13*f2vm1*tau21*
     & a13*tau11+tau22*tau13*f2wm1*tau11*tau21*a12+6.*tau13**2*f2vm2*
     & tau21**2*a12-6.*tau22**2*tau13*f2um2*a13*tau11+tau22*tau13*
     & f2wm1*tau21*tau12*a11+6.*tau22*tau13*f2vm2*tau21*a13*tau11-2.*
     & tau22*tau13*f2um1*a11*tau23*tau12+tau22*tau13*f2um1*a12*tau11*
     & tau23-1.*tau22**2*tau13*f2wm1*tau11*a11+tau22*tau13*f2um1*
     & tau21*tau12*a13-1.*tau23**2*f2vm1*tau11*tau12*a11+6.*tau22*
     & f2wm2*tau11*tau23*a11*tau12-1.*tau13*f2wm1*tau21**2*tau12*a12-
     & 6.*tau22*f2vm2*tau23*tau11**2*a13-6.*tau22**2*tau13*f2wm2*
     & tau11*a11+tau23**2*f2vm1*tau11**2*a12+6.*f2vm2*tau23**2*tau11**
     & 2*a12+6.*f2um2*tau23**2*a11*tau12**2+6.*tau22*f2um2*a13*tau11*
     & tau23*tau12-12.*tau22*f2wm2*tau12*tau21*a13*tau11+6.*tau22*
     & tau13*f2wm2*tau11*a12*tau21+tau22*tau13*tau23*f2vm1*tau11*a11+
     & 6.*tau22*tau13*f2vm2*tau23*tau11*a11+6.*tau22*tau13*f2um2*
     & tau23*a12*tau11+tau22**2*tau13**2*f2um1*a11+6.*f2wm2*tau12**2*
     & tau21**2*a13+tau23**2*f2um1*tau12**2*a11+f2wm1*tau21**2*tau12**
     & 2*a13+6.*tau22**2*f2wm2*tau11**2*a13+tau22**2*f2wm1*tau11**2*
     & a13+6.*tau22**2*tau13**2*f2um2*a11)

      u(i1-2*is1,i2-2*is2,i3-2*is3,ey) = -1.*(-1.*tau13*tau21**2*f2wm1*
     & tau12*f1f+tau13**2*tau21**2*f2vm1*f1f+tau23*tau12*tau21*f2wm1*
     & tau11*f1f-1.*tau23**2*tau11*f2um1*tau12*f1f-1.*tau22*tau23*
     & tau11**2*f2wm1*f1f-2.*tau13*tau21*f2vm1*tau23*tau11*f1f+tau13*
     & tau21*f2um1*tau23*tau12*f1f-1.*tau22*tau13**2*tau21*f2um1*f1f+
     & tau22*tau13*tau21*f2wm1*tau11*f1f+tau22*tau13*tau23*tau11*
     & f2um1*f1f+6.*tau22*tau13*tau21*a13*ttu11*f2um1+tau22*tau13*
     & tau23*ttu12*f2um1*a11+tau22*tau21*a13*ttu12*f2wm1*tau11+6.*
     & tau22*tau13*ttu22*f2wm2*tau11*a11+6.*tau22*tau23*tau11*a11*
     & f2wm1*ttu11-1.*tau22*tau13**2*ttu22*f2um1*a11-6.*tau22*tau13**
     & 2*ttu22*f2um2*a11-1.*tau22*ttu22*f2wm1*tau11**2*a13-6.*tau22*
     & ttu22*f2wm2*tau11**2*a13+6.*tau22*tau21*a13*tau11*f2wm2*ttu12-
     & 1.*tau22*tau23*ttu12*f2wm1*tau11*a11+tau22*tau13*ttu22*f2wm1*
     & tau11*a11+tau22*tau13*ttu22*f2um1*a13*tau11+6.*tau22*tau13*
     & tau23*ttu12*f2um2*a11+6.*tau22*tau13*ttu22*f2um2*a13*tau11-1.*
     & tau22*tau13*tau21*a13*ttu12*f2um1+tau13*tau21**2*a13*ttu12*
     & f2vm1-6.*tau13*tau21**2*a13*ttu11*f2vm1+6.*tau13*tau21*a13*
     & ttu21*tau11*f2vm1+6.*tau13*tau21*a11*ttu21*f2wm1*tau12-1.*
     & tau13*tau23*ttu12*f2vm1*tau21*a11-1.*tau13*ttu22*tau23*f2vm1*
     & tau11*a11+6.*tau13*ttu22*f2um2*tau23*a11*tau12+6.*tau13*tau21*
     & tau23*a11*ttu11*f2vm1-1.*tau13*ttu22*f2vm1*tau21*a13*tau11-1.*
     & tau13*ttu22*f2wm1*tau21*tau12*a11+tau13*ttu22*f2um1*a11*tau23*
     & tau12+6.*tau13*tau23*tau11*a11*ttu21*f2vm1-6.*tau13**2*tau21*
     & a11*ttu21*f2vm1+tau13**2*ttu22*f2vm1*tau21*a11-6.*tau21**2*a13*
     & tau12*f2wm2*ttu12+6.*tau23*tau11*a13*tau12*f2um1*ttu21-1.*
     & tau23*tau11*ttu22*f2um1*tau12*a13-6.*tau23*tau11**2*a13*ttu21*
     & f2vm1-6.*tau23**2*ttu12*f2um2*a11*tau12+tau21*ttu22*f2wm1*
     & tau12*a13*tau11+6.*tau21*a13*tau12*f2wm2*tau11*ttu22+ttu22*
     & tau23*f2vm1*tau11**2*a13-1.*tau23**2*ttu12*f2um1*a11*tau12-1.*
     & tau21**2*a13*ttu12*f2wm1*tau12+6.*tau21*tau23*a11*tau12*f2wm2*
     & ttu12+tau23*ttu12*f2wm1*tau21*tau12*a11+tau23**2*ttu12*f2vm1*
     & tau11*a11+6.*tau23*ttu12*f2um2*a13*tau21*tau12+6.*tau23*tau11*
     & tau21*a13*ttu11*f2vm1-1.*tau21*tau23*ttu12*f2vm1*a13*tau11-6.*
     & tau23**2*tau11*a11*ttu11*f2vm1+tau21*a13*tau12*tau23*ttu12*
     & f2um1+tau23**2*tau11**2*f2vm1*f1f+6.*tau13**2*f2um2*a12*tau21*
     & ttu22-6.*tau13*f2wm2*tau11*a12*tau21*ttu22-6.*tau13*f2um2*a13*
     & tau12*tau21*ttu22+6.*tau13*tau21*f2f*tau23*tau12*a11-6.*tau13*
     & tau23*f2um1*ttu11*tau21*a12-6.*tau13*f2um1*ttu21*a12*tau11*
     & tau23-6.*tau13*f2um1*ttu21*tau21*tau12*a13-6.*tau13*tau21**2*
     & f2f*tau12*a13+6.*tau13*tau21**2*f2wm2*ttu12*a12-6.*tau13*f2wm1*
     & ttu21*tau11*tau21*a12+6.*tau23*tau12*tau21*f2f*a13*tau11-12.*
     & tau13*tau21*f2f*a12*tau11*tau23-6.*tau13*f2um2*a12*tau21*tau23*
     & ttu12-6.*tau13*f2um2*a12*tau11*tau23*ttu22+6.*tau13**2*f2um1*
     & ttu21*tau21*a12-6.*tau23*tau11*f2wm1*tau12*ttu21*a11+6.*f2wm2*
     & tau11**2*a12*tau23*ttu22+6.*f2wm1*ttu21*tau11**2*a12*tau23-6.*
     & tau21*f2wm2*ttu12*a12*tau11*tau23-6.*tau23*tau11*f2wm2*tau12*
     & ttu22*a11+6.*tau23**2*f2um1*ttu11*a12*tau11-6.*tau23**2*tau11*
     & f2f*tau12*a11-6.*tau21*f2wm1*ttu11*a12*tau11*tau23+6.*tau23**2*
     & tau11**2*f2f*a12+6.*tau13**2*tau21**2*f2f*a12+6.*tau13*tau21**
     & 2*f2wm1*ttu11*a12+6.*f2um2*a12*tau11*tau23**2*ttu12+6.*tau22*
     & tau13*tau21*f2f*a13*tau11-6.*tau22*tau13*tau21*f2wm2*ttu12*a11-
     & 6.*tau22*tau23*tau11**2*f2f*a13-6.*tau22*tau13**2*tau21*f2f*
     & a11+6.*tau22*tau13*tau23*tau11*f2f*a11-6.*tau22*tau13*tau21*
     & f2wm1*ttu11*a11-6.*tau22*tau23*f2um1*ttu11*a13*tau11-6.*tau22*
     & f2um2*tau23*a13*ttu12*tau11)/(tau23*tau12*f2vm1*tau21*a13*
     & tau11+6.*tau23*tau12*f2wm2*tau11*a12*tau21+tau23*tau12*f2wm1*
     & tau11*tau21*a12-1.*f2wm1*tau21*tau12**2*tau23*a11+6.*tau23*
     & tau12*f2vm2*tau21*a13*tau11-1.*tau23*f2um1*tau12**2*tau21*a13+
     & 6.*tau13*f2vm2*tau21*tau23*a11*tau12-2.*tau13*tau23*f2vm1*
     & tau11*tau21*a12-6.*f2wm2*tau12**2*tau21*tau23*a11-6.*tau13*
     & f2vm2*tau21**2*a13*tau12+tau13*f2vm1*tau21*tau23*tau12*a11-1.*
     & tau23**2*f2um1*tau12*a12*tau11+tau13*tau23*f2um1*tau12*tau21*
     & a12-12.*tau13*f2vm2*tau21*tau23*a12*tau11-6.*tau13*f2wm2*tau12*
     & tau21**2*a12-1.*tau22**2*tau13*f2um1*a13*tau11+tau22*f2wm1*
     & tau11*tau23*tau12*a11-6.*tau22*f2wm2*tau11**2*tau23*a12-6.*
     & tau22*tau13**2*f2um2*a12*tau21-1.*tau22*tau13**2*f2um1*tau21*
     & a12-6.*tau22*tau13**2*f2vm2*tau21*a11-1.*tau22*tau13**2*f2vm1*
     & tau21*a11-1.*tau22*f2wm1*tau11**2*a12*tau23-1.*tau22*tau23*
     & f2vm1*tau11**2*a13-12.*tau22*tau13*f2um2*tau23*a11*tau12-1.*
     & tau13*f2vm1*tau21**2*tau12*a13-2.*tau22*f2wm1*tau21*tau12*a13*
     & tau11+tau22*tau23*f2um1*tau12*a13*tau11-6.*f2um2*tau23**2*a12*
     & tau11*tau12-6.*f2um2*a13*tau21*tau12**2*tau23-6.*f2vm2*tau23**
     & 2*tau11*a11*tau12+6.*tau13*f2um2*a12*tau21*tau23*tau12+6.*
     & tau22*tau13*f2um2*a13*tau21*tau12+6.*tau22*tau13*f2wm2*tau12*
     & tau21*a11+tau13**2*f2vm1*tau21**2*a12+tau22*tau13*f2vm1*tau21*
     & a13*tau11+tau22*tau13*f2wm1*tau11*tau21*a12+6.*tau13**2*f2vm2*
     & tau21**2*a12-6.*tau22**2*tau13*f2um2*a13*tau11+tau22*tau13*
     & f2wm1*tau21*tau12*a11+6.*tau22*tau13*f2vm2*tau21*a13*tau11-2.*
     & tau22*tau13*f2um1*a11*tau23*tau12+tau22*tau13*f2um1*a12*tau11*
     & tau23-1.*tau22**2*tau13*f2wm1*tau11*a11+tau22*tau13*f2um1*
     & tau21*tau12*a13-1.*tau23**2*f2vm1*tau11*tau12*a11+6.*tau22*
     & f2wm2*tau11*tau23*a11*tau12-1.*tau13*f2wm1*tau21**2*tau12*a12-
     & 6.*tau22*f2vm2*tau23*tau11**2*a13-6.*tau22**2*tau13*f2wm2*
     & tau11*a11+tau23**2*f2vm1*tau11**2*a12+6.*f2vm2*tau23**2*tau11**
     & 2*a12+6.*f2um2*tau23**2*a11*tau12**2+6.*tau22*f2um2*a13*tau11*
     & tau23*tau12-12.*tau22*f2wm2*tau12*tau21*a13*tau11+6.*tau22*
     & tau13*f2wm2*tau11*a12*tau21+tau22*tau13*tau23*f2vm1*tau11*a11+
     & 6.*tau22*tau13*f2vm2*tau23*tau11*a11+6.*tau22*tau13*f2um2*
     & tau23*a12*tau11+tau22**2*tau13**2*f2um1*a11+6.*f2wm2*tau12**2*
     & tau21**2*a13+tau23**2*f2um1*tau12**2*a11+f2wm1*tau21**2*tau12**
     & 2*a13+6.*tau22**2*f2wm2*tau11**2*a13+tau22**2*f2wm1*tau11**2*
     & a13+6.*tau22**2*tau13**2*f2um2*a11)


 ! *********** done *********************
               if( debug.gt.0 )then
                 call ogf3d(ep,xy(i1-is1,i2-is2,i3-is3,0),xy(i1-is1,i2-
     & is2,i3-is3,1),xy(i1-is1,i2-is2,i3-is3,2),t,uvm(0),uvm(1),uvm(2)
     & )
                 call ogf3d(ep,xy(i1-2*is1,i2-2*is2,i3-2*is3,0),xy(i1-
     & 2*is1,i2-2*is2,i3-2*is3,1),xy(i1-2*is1,i2-2*is2,i3-2*is3,2),t,
     & uvm2(0),uvm2(1),uvm2(2))
                write(*,'(" **ghost-interp3d: errors u(-1)=",3e10.2)') 
     & u(i1-is1,i2-is2,i3-is3,ex)-uvm(0),u(i1-is1,i2-is2,i3-is3,ey)-
     & uvm(1),u(i1-is1,i2-is2,i3-is3,ez)-uvm(2)
                write(*,'(" **ghost-interp3d: errors u(-2)=",3e10.2)') 
     & u(i1-2*is1,i2-2*is2,i3-2*is3,ex)-uvm2(0),u(i1-2*is1,i2-2*is2,
     & i3-2*is3,ey)-uvm2(1),u(i1-2*is1,i2-2*is2,i3-2*is3,ez)-uvm2(2)
               end if
               ! set to exact for testing
               ! OGF3D(i1-is1,i2-is2,i3-is3,t, uvm(0),uvm(1),uvm(2))
               ! OGF3D(i1-2*is1,i2-2*is2,i3-2*is3,t, uvm2(0),uvm2(1),uvm2(2))
               ! u(i1-is1,i2-is2,i3-is3,ex)=uvm(0)
               ! u(i1-is1,i2-is2,i3-is3,ey)=uvm(1)
               ! u(i1-is1,i2-is2,i3-is3,ez)=uvm(2)
               ! u(i1-2*is1,i2-2*is2,i3-2*is3,ex)=uvm2(0)
               ! u(i1-2*is1,i2-2*is2,i3-2*is3,ey)=uvm2(1)
               ! u(i1-2*is1,i2-2*is2,i3-2*is3,ez)=uvm2(2)
             ! &&&&&&&&&&&&&&&&&&&&&&&
             ! *   detnt=tau23*a11*tau12-tau23*a12*tau11-a13*tau21*tau12+tau21*tau13*a12+a13*tau22*tau11-tau22*tau13*a11
             ! *   do m=1,2
             ! *     m1=i1-m*is1
             ! *     m2=i2-m*is2
             ! *     m3=i3-m*is3
             ! *     ! use u.r=0 for now:
             ! *     !    tau.urr=0
             ! *     a1DotU= a11*u(i1+m*is1,i2+m*is2,i3+m*is3,ex)! *            +a12*u(i1+m*is1,i2+m*is2,i3+m*is3,ey)! *            +a13*u(i1+m*is1,i2+m*is2,i3+m*is3,ez)  
             ! *     tau1DotU=-( tau11*u(i1+m*is1,i2+m*is2,i3+m*is3,ex)! *                +tau12*u(i1+m*is1,i2+m*is2,i3+m*is3,ey)! *                +tau13*u(i1+m*is1,i2+m*is2,i3+m*is3,ez) )
             ! *     tau2DotU=-( tau21*u(i1+m*is1,i2+m*is2,i3+m*is3,ex)! *                +tau22*u(i1+m*is1,i2+m*is2,i3+m*is3,ey)! *                +tau23*u(i1+m*is1,i2+m*is2,i3+m*is3,ez) )
             ! *   
             ! *     u(m1,m2,m3,ex)=(tau23*a1DotU*tau12-a13*tau2DotU*tau12+a13*tau22*tau1DotU+tau2DotU*tau13*a12-tau22*tau13*a1DotU-tau23*a12*tau1DotU)/detnt
             ! *     u(m1,m2,m3,ey)=(-tau13*a11*tau2DotU+tau13*a1DotU*tau21+a11*tau23*tau1DotU+a13*tau11*tau2DotU-a1DotU*tau23*tau11-a13*tau1DotU*tau21)/detnt
             ! *     u(m1,m2,m3,ez)=(a11*tau2DotU*tau12-a11*tau22*tau1DotU-a12*tau11*tau2DotU+a12*tau1DotU*tau21-a1DotU*tau21*tau12+a1DotU*tau22*tau11)/detnt
             ! *   end do 
             end if
             end do
             end do
             end do
           else
              ! Since is1 is +1 or -1 we need to flip the sign of dr in the derivative approximations
              dra = dr(axis  )*(1-2*side)
              dsa = dr(axisp1)*(1-2*side)
              dta = dr(axisp2)*(1-2*side)
              drb = dr(axis  )
              dsb = dr(axisp1)
              dtb = dr(axisp2)
              ! ** Fourth-order for tau.Delta\uv=0, setting  ctlrr=ctlr=0 in the code will revert to 2nd-order
              ctlrr=1.
              ctlr=1.
              if( debug.gt.0 )then
                write(*,'(" **bcCurvilinear3dOrder4Step1: START: grid,
     & side,axis=",3i2," is1,is2,is3=",3i3," ks1,ks2,ks3=",3i3)')grid,
     & side,axis,is1,is2,is3,ks1,ks2,ks3
              end if
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
              if( mask(i1,i2,i3).gt.0 )then
               ! precompute the inverse of the jacobian, used in macros AmnD3J
               i10=i1  ! used by jac3di in macros
               i20=i2
               i30=i3
               do m3=-2,2
               do m2=-2,2
               do m1=-2,2
                jac3di(m1,m2,m3)=1./(rx(i1+m1,i2+m2,i3+m3)*(sy(i1+m1,
     & i2+m2,i3+m3)*tz(i1+m1,i2+m2,i3+m3)-sz(i1+m1,i2+m2,i3+m3)*ty(i1+
     & m1,i2+m2,i3+m3))+ry(i1+m1,i2+m2,i3+m3)*(sz(i1+m1,i2+m2,i3+m3)*
     & tx(i1+m1,i2+m2,i3+m3)-sx(i1+m1,i2+m2,i3+m3)*tz(i1+m1,i2+m2,i3+
     & m3))+rz(i1+m1,i2+m2,i3+m3)*(sx(i1+m1,i2+m2,i3+m3)*ty(i1+m1,i2+
     & m2,i3+m3)-sy(i1+m1,i2+m2,i3+m3)*tx(i1+m1,i2+m2,i3+m3)))
               end do
               end do
               end do
               a11 =(rsxy(i1,i2,i3,axis,0)*jac3di(i1-i10,i2-i20,i3-i30)
     & )
               a12 =(rsxy(i1,i2,i3,axis,1)*jac3di(i1-i10,i2-i20,i3-i30)
     & )
               a13 =(rsxy(i1,i2,i3,axis,2)*jac3di(i1-i10,i2-i20,i3-i30)
     & )
               a21 =(rsxy(i1,i2,i3,axisp1,0)*jac3di(i1-i10,i2-i20,i3-
     & i30))
               a22 =(rsxy(i1,i2,i3,axisp1,1)*jac3di(i1-i10,i2-i20,i3-
     & i30))
               a23 =(rsxy(i1,i2,i3,axisp1,2)*jac3di(i1-i10,i2-i20,i3-
     & i30))
               a31 =(rsxy(i1,i2,i3,axisp2,0)*jac3di(i1-i10,i2-i20,i3-
     & i30))
               a32 =(rsxy(i1,i2,i3,axisp2,1)*jac3di(i1-i10,i2-i20,i3-
     & i30))
               a33 =(rsxy(i1,i2,i3,axisp2,2)*jac3di(i1-i10,i2-i20,i3-
     & i30))
               a11m1 =(rsxy(i1-is1,i2-is2,i3-is3,axis,0)*jac3di(i1-is1-
     & i10,i2-is2-i20,i3-is3-i30))
               a12m1 =(rsxy(i1-is1,i2-is2,i3-is3,axis,1)*jac3di(i1-is1-
     & i10,i2-is2-i20,i3-is3-i30))
               a13m1 =(rsxy(i1-is1,i2-is2,i3-is3,axis,2)*jac3di(i1-is1-
     & i10,i2-is2-i20,i3-is3-i30))
               a11p1 =(rsxy(i1+is1,i2+is2,i3+is3,axis,0)*jac3di(i1+is1-
     & i10,i2+is2-i20,i3+is3-i30))
               a12p1 =(rsxy(i1+is1,i2+is2,i3+is3,axis,1)*jac3di(i1+is1-
     & i10,i2+is2-i20,i3+is3-i30))
               a13p1 =(rsxy(i1+is1,i2+is2,i3+is3,axis,2)*jac3di(i1+is1-
     & i10,i2+is2-i20,i3+is3-i30))
               a11m2 =(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,0)*jac3di(
     & i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))
               a12m2 =(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,1)*jac3di(
     & i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))
               a13m2 =(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,2)*jac3di(
     & i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))
               a11p2 =(rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axis,0)*jac3di(
     & i1+2*is1-i10,i2+2*is2-i20,i3+2*is3-i30))
               a12p2 =(rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axis,1)*jac3di(
     & i1+2*is1-i10,i2+2*is2-i20,i3+2*is3-i30))
               a13p2 =(rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axis,2)*jac3di(
     & i1+2*is1-i10,i2+2*is2-i20,i3+2*is3-i30))
               c11 = (rsxy(i1,i2,i3,axis,0)**2+rsxy(i1,i2,i3,axis,1)**
     & 2+rsxy(i1,i2,i3,axis,2)**2)
               c22 = (rsxy(i1,i2,i3,axisp1,0)**2+rsxy(i1,i2,i3,axisp1,
     & 1)**2+rsxy(i1,i2,i3,axisp1,2)**2)
               c33 = (rsxy(i1,i2,i3,axisp2,0)**2+rsxy(i1,i2,i3,axisp2,
     & 1)**2+rsxy(i1,i2,i3,axisp2,2)**2)
               c1 = (rsxyx43(i1,i2,i3,axis,0)+rsxyy43(i1,i2,i3,axis,1)+
     & rsxyz43(i1,i2,i3,axis,2))
               c2 = (rsxyx43(i1,i2,i3,axisp1,0)+rsxyy43(i1,i2,i3,
     & axisp1,1)+rsxyz43(i1,i2,i3,axisp1,2))
               c3 = (rsxyx43(i1,i2,i3,axisp2,0)+rsxyy43(i1,i2,i3,
     & axisp2,1)+rsxyz43(i1,i2,i3,axisp2,2))
               us=(8.*(u(i1+js1,i2+js2,i3+js3,ex)-u(i1-js1,i2-js2,i3-
     & js3,ex))-(u(i1+2*js1,i2+2*js2,i3+2*js3,ex)-u(i1-2*js1,i2-2*js2,
     & i3-2*js3,ex)))/(12.*dsa)
               uss=(-30.*u(i1,i2,i3,ex)+16.*(u(i1+js1,i2+js2,i3+js3,ex)
     & +u(i1-js1,i2-js2,i3-js3,ex))-(u(i1+2*js1,i2+2*js2,i3+2*js3,ex)+
     & u(i1-2*js1,i2-2*js2,i3-2*js3,ex)))/(12.*dsa**2)
               vs=(8.*(u(i1+js1,i2+js2,i3+js3,ey)-u(i1-js1,i2-js2,i3-
     & js3,ey))-(u(i1+2*js1,i2+2*js2,i3+2*js3,ey)-u(i1-2*js1,i2-2*js2,
     & i3-2*js3,ey)))/(12.*dsa)
               vss=(-30.*u(i1,i2,i3,ey)+16.*(u(i1+js1,i2+js2,i3+js3,ey)
     & +u(i1-js1,i2-js2,i3-js3,ey))-(u(i1+2*js1,i2+2*js2,i3+2*js3,ey)+
     & u(i1-2*js1,i2-2*js2,i3-2*js3,ey)))/(12.*dsa**2)
               ws=(8.*(u(i1+js1,i2+js2,i3+js3,ez)-u(i1-js1,i2-js2,i3-
     & js3,ez))-(u(i1+2*js1,i2+2*js2,i3+2*js3,ez)-u(i1-2*js1,i2-2*js2,
     & i3-2*js3,ez)))/(12.*dsa)
               wss=(-30.*u(i1,i2,i3,ez)+16.*(u(i1+js1,i2+js2,i3+js3,ez)
     & +u(i1-js1,i2-js2,i3-js3,ez))-(u(i1+2*js1,i2+2*js2,i3+2*js3,ez)+
     & u(i1-2*js1,i2-2*js2,i3-2*js3,ez)))/(12.*dsa**2)
               ut=(8.*(u(i1+ks1,i2+ks2,i3+ks3,ex)-u(i1-ks1,i2-ks2,i3-
     & ks3,ex))-(u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ex)-u(i1-2*ks1,i2-2*ks2,
     & i3-2*ks3,ex)))/(12.*dta)
               utt=(-30.*u(i1,i2,i3,ex)+16.*(u(i1+ks1,i2+ks2,i3+ks3,ex)
     & +u(i1-ks1,i2-ks2,i3-ks3,ex))-(u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ex)+
     & u(i1-2*ks1,i2-2*ks2,i3-2*ks3,ex)))/(12.*dta**2)
               vt=(8.*(u(i1+ks1,i2+ks2,i3+ks3,ey)-u(i1-ks1,i2-ks2,i3-
     & ks3,ey))-(u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ey)-u(i1-2*ks1,i2-2*ks2,
     & i3-2*ks3,ey)))/(12.*dta)
               vtt=(-30.*u(i1,i2,i3,ey)+16.*(u(i1+ks1,i2+ks2,i3+ks3,ey)
     & +u(i1-ks1,i2-ks2,i3-ks3,ey))-(u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ey)+
     & u(i1-2*ks1,i2-2*ks2,i3-2*ks3,ey)))/(12.*dta**2)
               wt=(8.*(u(i1+ks1,i2+ks2,i3+ks3,ez)-u(i1-ks1,i2-ks2,i3-
     & ks3,ez))-(u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ez)-u(i1-2*ks1,i2-2*ks2,
     & i3-2*ks3,ez)))/(12.*dta)
               wtt=(-30.*u(i1,i2,i3,ez)+16.*(u(i1+ks1,i2+ks2,i3+ks3,ez)
     & +u(i1-ks1,i2-ks2,i3-ks3,ez))-(u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ez)+
     & u(i1-2*ks1,i2-2*ks2,i3-2*ks3,ez)))/(12.*dta**2)
              tau11=rsxy(i1,i2,i3,axisp1,0)
              tau12=rsxy(i1,i2,i3,axisp1,1)
              tau13=rsxy(i1,i2,i3,axisp1,2)
              tau21=rsxy(i1,i2,i3,axisp2,0)
              tau22=rsxy(i1,i2,i3,axisp2,1)
              tau23=rsxy(i1,i2,i3,axisp2,2)
              uex=u(i1,i2,i3,ex)
              uey=u(i1,i2,i3,ey)
              uez=u(i1,i2,i3,ez)
             ! ************ Answer *******************
              Da1DotU=0.
              tau1DotUtt=0.
              tau2DotUtt=0.
              gIVf1=0.
              gIVf2=0.
              if( forcingOption.eq.planeWaveBoundaryForcing )then
                ! In the plane wave forcing case we subtract out a plane wave incident field
                ! This causes the BC to be 
                !           tau.u = - tau.uI
                !   and     tau.utt = -tau.uI.tt
                a21s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axisp1,0)*
     & jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))-(rsxy(i1-js1,i2-js2,
     & i3-js3,axisp1,0)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,0)*jac3di(i1+2*js1-i10,
     & i2+2*js2-i20,i3+2*js3-i30))-(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,
     & axisp1,0)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))/(
     & 12.*dsa)
                a22s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axisp1,1)*
     & jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))-(rsxy(i1-js1,i2-js2,
     & i3-js3,axisp1,1)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,1)*jac3di(i1+2*js1-i10,
     & i2+2*js2-i20,i3+2*js3-i30))-(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,
     & axisp1,1)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))/(
     & 12.*dsa)
                a23s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axisp1,2)*
     & jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))-(rsxy(i1-js1,i2-js2,
     & i3-js3,axisp1,2)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,2)*jac3di(i1+2*js1-i10,
     & i2+2*js2-i20,i3+2*js3-i30))-(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,
     & axisp1,2)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))/(
     & 12.*dsa)
                a31t = (8.*((rsxy(i1+ks1,i2+ks2,i3+ks3,axisp2,0)*
     & jac3di(i1+ks1-i10,i2+ks2-i20,i3+ks3-i30))-(rsxy(i1-ks1,i2-ks2,
     & i3-ks3,axisp2,0)*jac3di(i1-ks1-i10,i2-ks2-i20,i3-ks3-i30)))-((
     & rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axisp2,0)*jac3di(i1+2*ks1-i10,
     & i2+2*ks2-i20,i3+2*ks3-i30))-(rsxy(i1-2*ks1,i2-2*ks2,i3-2*ks3,
     & axisp2,0)*jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30))))/(
     & 12.*dta)
                a32t = (8.*((rsxy(i1+ks1,i2+ks2,i3+ks3,axisp2,1)*
     & jac3di(i1+ks1-i10,i2+ks2-i20,i3+ks3-i30))-(rsxy(i1-ks1,i2-ks2,
     & i3-ks3,axisp2,1)*jac3di(i1-ks1-i10,i2-ks2-i20,i3-ks3-i30)))-((
     & rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axisp2,1)*jac3di(i1+2*ks1-i10,
     & i2+2*ks2-i20,i3+2*ks3-i30))-(rsxy(i1-2*ks1,i2-2*ks2,i3-2*ks3,
     & axisp2,1)*jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30))))/(
     & 12.*dta)
                a33t = (8.*((rsxy(i1+ks1,i2+ks2,i3+ks3,axisp2,2)*
     & jac3di(i1+ks1-i10,i2+ks2-i20,i3+ks3-i30))-(rsxy(i1-ks1,i2-ks2,
     & i3-ks3,axisp2,2)*jac3di(i1-ks1-i10,i2-ks2-i20,i3-ks3-i30)))-((
     & rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axisp2,2)*jac3di(i1+2*ks1-i10,
     & i2+2*ks2-i20,i3+2*ks3-i30))-(rsxy(i1-2*ks1,i2-2*ks2,i3-2*ks3,
     & axisp2,2)*jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30))))/(
     & 12.*dta)
                ! *** set RHS for (a1.u).r =  - Ds( a2.uv ) -Dt( a3.uv )
                Da1DotU = -(  a21s*uex+a22s*uey+a23s*uez + a21*us+a22*
     & vs+a23*ws + a31t*uex+a32t*uey+a33t*uez + a31*ut+a32*vt+a33*wt )
                 x00=xy(i1,i2,i3,0)
                 y00=xy(i1,i2,i3,1)
                 z00=xy(i1,i2,i3,2)
                 if( fieldOption.eq.0 )then
                   udd=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x00)+ky*(
     & y00)+kz*(z00)-cc*(t)))*pwc(0))+2.*ssft*(-twoPi*cc)*cos(twoPi*(
     & kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0)+ssftt*sin(twoPi*(kx*
     & (x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))
                   vdd=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x00)+ky*(
     & y00)+kz*(z00)-cc*(t)))*pwc(1))+2.*ssft*(-twoPi*cc)*cos(twoPi*(
     & kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1)+ssftt*sin(twoPi*(kx*
     & (x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))
                   wdd=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x00)+ky*(
     & y00)+kz*(z00)-cc*(t)))*pwc(2))+2.*ssft*(-twoPi*cc)*cos(twoPi*(
     & kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2)+ssftt*sin(twoPi*(kx*
     & (x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))
                 else
                   ! get time derivative (sosup) 
                   udd=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x00)+ky*(
     & y00)+kz*(z00)-cc*(t)))*pwc(0))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))+3.*ssftt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0)
     & +ssfttt*sin(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))
                   vdd=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x00)+ky*(
     & y00)+kz*(z00)-cc*(t)))*pwc(1))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))+3.*ssftt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1)
     & +ssfttt*sin(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))
                   wdd=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x00)+ky*(
     & y00)+kz*(z00)-cc*(t)))*pwc(2))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))+3.*ssftt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2)
     & +ssfttt*sin(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))
                 end if
                tau1DotUtt = tau11*udd+tau12*vdd+tau13*wdd
                tau2DotUtt = tau21*udd+tau22*vdd+tau23*wdd
              end if
              if( useForcing.ne.0 )then
                ! For TZ: utt0 = utt - ett + Lap(e)
                 call ogDeriv3(ep, 0,2,0,0, xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),xy(i1,i2,i3,2),t, ex,uxx, ey,vxx, ez,wxx)
                 call ogDeriv3(ep, 0,0,2,0, xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),xy(i1,i2,i3,2),t, ex,uyy, ey,vyy, ez,wyy)
                 call ogDeriv3(ep, 0,0,0,2, xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),xy(i1,i2,i3,2),t, ex,uzz, ey,vzz, ez,wzz)
               utt00=uxx+uyy+uzz
               vtt00=vxx+vyy+vzz
               wtt00=wxx+wyy+wzz
               tau1DotUtt = tau11*utt00+tau12*vtt00+tau13*wtt00
               tau2DotUtt = tau21*utt00+tau22*vtt00+tau23*wtt00
               ! OGF3D(i1,i2,i3,t, uv0(0),uv0(1),uv0(2))
               ! OGF3D(i1-is1,i2-is2,i3-is3,t, uvm(0),uvm(1),uvm(2))
               ! OGF3D(i1+is1,i2+is2,i3+is3,t, uvp(0),uvp(1),uvp(2))
               ! OGF3D(i1-2*is1,i2-2*is2,i3-2*is3,t, uvm2(0),uvm2(1),uvm2(2))
               ! OGF3D(i1+2*is1,i2+2*is2,i3+2*is3,t, uvp2(0),uvp2(1),uvp2(2))
               ! Da1DotU = (a1.uv).r to 4th order
               ! Da1DotU = (8.*( (a11p1*uvp(0) +a12p1*uvp(1))  - (a11m1*uvm(0) +a12m1*uvm(1)) )!             - ( (a11p2*uvp2(0)+a12p2*uvp2(1)) - (a11m2*uvm2(0)+a12m2*uvm2(1)) ) )/(12.*dra)
              ! for now remove the error in the extrapolation ************
              ! gIVf1 = tau11*( uvm2(0)-4.*uvm(0)+6.*uv0(0)-4.*uvp(0)+uvp2(0)) +!         tau12*( uvm2(1)-4.*uvm(1)+6.*uv0(1)-4.*uvp(1)+uvp2(1)) +!         tau13*( uvm2(2)-4.*uvm(2)+6.*uv0(2)-4.*uvp(2)+uvp2(2))
              ! gIVf2 = tau21*( uvm2(0)-4.*uvm(0)+6.*uv0(0)-4.*uvp(0)+uvp2(0)) +!         tau22*( uvm2(1)-4.*uvm(1)+6.*uv0(1)-4.*uvp(1)+uvp2(1)) +!         tau23*( uvm2(2)-4.*uvm(2)+6.*uv0(2)-4.*uvp(2)+uvp2(2))
              ! gIVf1=0.  ! RHS for tau.D+^p(u)=0
              ! gIVf2=0.
               ! **** compute RHS for div(u) equation ****
               a21zp1= (rsxy(i1+js1,i2+js2,i3+js3,axisp1,0)*jac3di(i1+
     & js1-i10,i2+js2-i20,i3+js3-i30))
               a21zm1= (rsxy(i1-js1,i2-js2,i3-js3,axisp1,0)*jac3di(i1-
     & js1-i10,i2-js2-i20,i3-js3-i30))
               a21zp2= (rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,0)*
     & jac3di(i1+2*js1-i10,i2+2*js2-i20,i3+2*js3-i30))
               a21zm2= (rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axisp1,0)*
     & jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))
               a22zp1= (rsxy(i1+js1,i2+js2,i3+js3,axisp1,1)*jac3di(i1+
     & js1-i10,i2+js2-i20,i3+js3-i30))
               a22zm1= (rsxy(i1-js1,i2-js2,i3-js3,axisp1,1)*jac3di(i1-
     & js1-i10,i2-js2-i20,i3-js3-i30))
               a22zp2= (rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,1)*
     & jac3di(i1+2*js1-i10,i2+2*js2-i20,i3+2*js3-i30))
               a22zm2= (rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axisp1,1)*
     & jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))
               a23zp1= (rsxy(i1+js1,i2+js2,i3+js3,axisp1,2)*jac3di(i1+
     & js1-i10,i2+js2-i20,i3+js3-i30))
               a23zm1= (rsxy(i1-js1,i2-js2,i3-js3,axisp1,2)*jac3di(i1-
     & js1-i10,i2-js2-i20,i3-js3-i30))
               a23zp2= (rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,2)*
     & jac3di(i1+2*js1-i10,i2+2*js2-i20,i3+2*js3-i30))
               a23zm2= (rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axisp1,2)*
     & jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))
               a31zp1= (rsxy(i1+ks1,i2+ks2,i3+ks3,axisp2,0)*jac3di(i1+
     & ks1-i10,i2+ks2-i20,i3+ks3-i30))
               a31zm1= (rsxy(i1-ks1,i2-ks2,i3-ks3,axisp2,0)*jac3di(i1-
     & ks1-i10,i2-ks2-i20,i3-ks3-i30))
               a31zp2= (rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axisp2,0)*
     & jac3di(i1+2*ks1-i10,i2+2*ks2-i20,i3+2*ks3-i30))
               a31zm2= (rsxy(i1-2*ks1,i2-2*ks2,i3-2*ks3,axisp2,0)*
     & jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30))
               a32zp1= (rsxy(i1+ks1,i2+ks2,i3+ks3,axisp2,1)*jac3di(i1+
     & ks1-i10,i2+ks2-i20,i3+ks3-i30))
               a32zm1= (rsxy(i1-ks1,i2-ks2,i3-ks3,axisp2,1)*jac3di(i1-
     & ks1-i10,i2-ks2-i20,i3-ks3-i30))
               a32zp2= (rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axisp2,1)*
     & jac3di(i1+2*ks1-i10,i2+2*ks2-i20,i3+2*ks3-i30))
               a32zm2= (rsxy(i1-2*ks1,i2-2*ks2,i3-2*ks3,axisp2,1)*
     & jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30))
               a33zp1= (rsxy(i1+ks1,i2+ks2,i3+ks3,axisp2,2)*jac3di(i1+
     & ks1-i10,i2+ks2-i20,i3+ks3-i30))
               a33zm1= (rsxy(i1-ks1,i2-ks2,i3-ks3,axisp2,2)*jac3di(i1-
     & ks1-i10,i2-ks2-i20,i3-ks3-i30))
               a33zp2= (rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axisp2,2)*
     & jac3di(i1+2*ks1-i10,i2+2*ks2-i20,i3+2*ks3-i30))
               a33zm2= (rsxy(i1-2*ks1,i2-2*ks2,i3-2*ks3,axisp2,2)*
     & jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30))
               ! *** set to - Ds( a2.uv ) -Dt( a3.uv )
               Da1DotU = -(  ( 8.*(a21zp1*u(i1+  js1,i2+  js2,i3+  js3,
     & ex)-a21zm1*u(i1-  js1,i2-  js2,i3-  js3,ex)) -(a21zp2*u(i1+2*
     & js1,i2+2*js2,i3+2*js3,ex)-a21zm2*u(i1-2*js1,i2-2*js2,i3-2*js3,
     & ex)) )/(12.*dsa) +( 8.*(a22zp1*u(i1+  js1,i2+  js2,i3+  js3,ey)
     & -a22zm1*u(i1-  js1,i2-  js2,i3-  js3,ey)) -(a22zp2*u(i1+2*js1,
     & i2+2*js2,i3+2*js3,ey)-a22zm2*u(i1-2*js1,i2-2*js2,i3-2*js3,ey)) 
     & )/(12.*dsa) +( 8.*(a23zp1*u(i1+  js1,i2+  js2,i3+  js3,ez)-
     & a23zm1*u(i1-  js1,i2-  js2,i3-  js3,ez)) -(a23zp2*u(i1+2*js1,
     & i2+2*js2,i3+2*js3,ez)-a23zm2*u(i1-2*js1,i2-2*js2,i3-2*js3,ez)) 
     & )/(12.*dsa)  ) -(  ( 8.*(a31zp1*u(i1+  ks1,i2+  ks2,i3+  ks3,
     & ex)-a31zm1*u(i1-  ks1,i2-  ks2,i3-  ks3,ex)) -(a31zp2*u(i1+2*
     & ks1,i2+2*ks2,i3+2*ks3,ex)-a31zm2*u(i1-2*ks1,i2-2*ks2,i3-2*ks3,
     & ex)) )/(12.*dta) +( 8.*(a32zp1*u(i1+  ks1,i2+  ks2,i3+  ks3,ey)
     & -a32zm1*u(i1-  ks1,i2-  ks2,i3-  ks3,ey)) -(a32zp2*u(i1+2*ks1,
     & i2+2*ks2,i3+2*ks3,ey)-a32zm2*u(i1-2*ks1,i2-2*ks2,i3-2*ks3,ey)) 
     & )/(12.*dta) +( 8.*(a33zp1*u(i1+  ks1,i2+  ks2,i3+  ks3,ez)-
     & a33zm1*u(i1-  ks1,i2-  ks2,i3-  ks3,ez)) -(a33zp2*u(i1+2*ks1,
     & i2+2*ks2,i3+2*ks3,ez)-a33zm2*u(i1-2*ks1,i2-2*ks2,i3-2*ks3,ez)) 
     & )/(12.*dta)  )
              end if
             ! Now assign E at the ghost points:


! ************ Results from bc43d.maple *******************


! ************ solution using extrapolation for a1.u *******************
      gIII1=-tau11*(c22*uss+c2*us+c33*utt+c3*ut)-tau12*(c22*vss+c2*vs+
     & c33*vtt+c3*vt)-tau13*(c22*wss+c2*ws+c33*wtt+c3*wt)

      gIII2=-tau21*(c22*uss+c2*us+c33*utt+c3*ut)-tau22*(c22*vss+c2*vs+
     & c33*vtt+c3*vt)-tau23*(c22*wss+c2*ws+c33*wtt+c3*wt)

      tau1U=tau11*u(i1,i2,i3,ex)+tau12*u(i1,i2,i3,ey)+tau13*u(i1,i2,i3,
     & ez)

      tau1Up1=tau11*u(i1+is1,i2+is2,i3+is3,ex)+tau12*u(i1+is1,i2+is2,
     & i3+is3,ey)+tau13*u(i1+is1,i2+is2,i3+is3,ez)

      tau1Up2=tau11*u(i1+2*is1,i2+2*is2,i3+2*is3,ex)+tau12*u(i1+2*is1,
     & i2+2*is2,i3+2*is3,ey)+tau13*u(i1+2*is1,i2+2*is2,i3+2*is3,ez)

      tau1Up3=tau11*u(i1+3*is1,i2+3*is2,i3+3*is3,ex)+tau12*u(i1+3*is1,
     & i2+3*is2,i3+3*is3,ey)+tau13*u(i1+3*is1,i2+3*is2,i3+3*is3,ez)

      tau2U=tau21*u(i1,i2,i3,ex)+tau22*u(i1,i2,i3,ey)+tau23*u(i1,i2,i3,
     & ez)

      tau2Up1=tau21*u(i1+is1,i2+is2,i3+is3,ex)+tau22*u(i1+is1,i2+is2,
     & i3+is3,ey)+tau23*u(i1+is1,i2+is2,i3+is3,ez)

      tau2Up2=tau21*u(i1+2*is1,i2+2*is2,i3+2*is3,ex)+tau22*u(i1+2*is1,
     & i2+2*is2,i3+2*is3,ey)+tau23*u(i1+2*is1,i2+2*is2,i3+2*is3,ez)

      tau2Up3=tau21*u(i1+3*is1,i2+3*is2,i3+3*is3,ex)+tau22*u(i1+3*is1,
     & i2+3*is2,i3+3*is3,ey)+tau23*u(i1+3*is1,i2+3*is2,i3+3*is3,ez)

! tau1.D+^p u = 0
      gIV1=-10*tau1U+10*tau1Up1-5*tau1Up2+tau1Up3 +gIVf1

! tau2.D+^p u = 0
      gIV2=-10*tau2U+10*tau2Up1-5*tau2Up2+tau2Up3 +gIVf2


! ttu11 = tau1.u(-1), ttu12 = tau1.u(-2)
      ttu11=-(-12*c11*tau1Up1+24*c11*tau1U+c11*ctlrr*tau1Up2-4*c11*
     & ctlrr*tau1Up1+6*c11*ctlrr*tau1U+c11*ctlrr*gIV1-6*c1*dra*
     & tau1Up1+c1*dra*ctlr*tau1Up2-2*c1*dra*ctlr*tau1Up1-c1*dra*ctlr*
     & gIV1+12*gIII1*dra**2+12*tau1DotUtt*dra**2)/(-12*c11+c11*ctlrr+
     & 6*c1*dra-3*c1*dra*ctlr)
      ttu12=-(-60*c11*tau1Up1+120*c11*tau1U+5*c11*ctlrr*tau1Up2-20*c11*
     & ctlrr*tau1Up1+30*c11*ctlrr*tau1U+4*c11*ctlrr*gIV1-30*c1*dra*
     & tau1Up1+5*c1*dra*ctlr*tau1Up2-10*c1*dra*ctlr*tau1Up1-2*c1*dra*
     & ctlr*gIV1+60*gIII1*dra**2+60*tau1DotUtt*dra**2+12*c11*gIV1-6*
     & gIV1*c1*dra)/(-12*c11+c11*ctlrr+6*c1*dra-3*c1*dra*ctlr)

! ttu21 = tau2.u(-1), ttu22 = tau2.u(-2)
      ttu21=-(-12*c11*tau2Up1+24*c11*tau2U+c11*ctlrr*tau2Up2-4*c11*
     & ctlrr*tau2Up1+6*c11*ctlrr*tau2U+c11*ctlrr*gIV2-6*c1*dra*
     & tau2Up1+c1*dra*ctlr*tau2Up2-2*c1*dra*ctlr*tau2Up1-c1*dra*ctlr*
     & gIV2+12*gIII2*dra**2+12*tau2DotUtt*dra**2)/(-12*c11+c11*ctlrr+
     & 6*c1*dra-3*c1*dra*ctlr)
      ttu22=-(-60*c11*tau2Up1+120*c11*tau2U+5*c11*ctlrr*tau2Up2-20*c11*
     & ctlrr*tau2Up1+30*c11*ctlrr*tau2U+4*c11*ctlrr*gIV2-30*c1*dra*
     & tau2Up1+5*c1*dra*ctlr*tau2Up2-10*c1*dra*ctlr*tau2Up1-2*c1*dra*
     & ctlr*gIV2+60*gIII2*dra**2+60*tau2DotUtt*dra**2+12*c11*gIV2-6*
     & gIV2*c1*dra)/(-12*c11+c11*ctlrr+6*c1*dra-3*c1*dra*ctlr)

      ! *********** set tangential components to be exact *****
      ! OGF3D(i1-is1,i2-is2,i3-is3,t, uvm(0),uvm(1),uvm(2))
      ! ttu11=tau11*uvm(0)+tau12*uvm(1)+tau13*uvm(2)
      ! ttu21=tau21*uvm(0)+tau22*uvm(1)+tau23*uvm(2)
      ! OGF3D(i1-2*is1,i2-2*is2,i3-2*is3,t, uvm(0),uvm(1),uvm(2))
      ! ttu12=tau11*uvm(0)+tau12*uvm(1)+tau13*uvm(2)
      ! ttu22=tau21*uvm(0)+tau22*uvm(1)+tau23*uvm(2)
      ! ******************************************************

      f1f  =a11*(15.*u(i1,i2,i3,ex)-20.*u(i1+is1,i2+is2,i3+is3,ex)+15.*
     & u(i1+2*is1,i2+2*is2,i3+2*is3,ex)-6.*u(i1+3*is1,i2+3*is2,i3+3*
     & is3,ex)+u(i1+4*is1,i2+4*is2,i3+4*is3,ex))+a12*(15.*u(i1,i2,i3,
     & ey)-20.*u(i1+is1,i2+is2,i3+is3,ey)+15.*u(i1+2*is1,i2+2*is2,i3+
     & 2*is3,ey)-6.*u(i1+3*is1,i2+3*is2,i3+3*is3,ey)+u(i1+4*is1,i2+4*
     & is2,i3+4*is3,ey))+a13*(15.*u(i1,i2,i3,ez)-20.*u(i1+is1,i2+is2,
     & i3+is3,ez)+15.*u(i1+2*is1,i2+2*is2,i3+2*is3,ez)-6.*u(i1+3*is1,
     & i2+3*is2,i3+3*is3,ez)+u(i1+4*is1,i2+4*is2,i3+4*is3,ez))

      f2um2=1/12.*a11m2
      f2um1=-2/3.*a11m1
      f2vm2=1/12.*a12m2
      f2vm1=-2/3.*a12m1
      f2wm2=1/12.*a13m2
      f2wm1=-2/3.*a13m1
      f2f  =2/3.*a11p1*u(i1+is1,i2+is2,i3+is3,ex)+2/3.*a12p1*u(i1+is1,
     & i2+is2,i3+is3,ey)+2/3.*a13p1*u(i1+is1,i2+is2,i3+is3,ez)-1/12.*
     & a11p2*u(i1+2*is1,i2+2*is2,i3+2*is3,ex)-1/12.*a12p2*u(i1+2*is1,
     & i2+2*is2,i3+2*is3,ey)-1/12.*a13p2*u(i1+2*is1,i2+2*is2,i3+2*is3,
     & ez)-Da1DotU*dra

      u(i1-is1,i2-is2,i3-is3,ez) = -1.*(tau21*f1f*f2um2*tau23*tau12**2-
     & 1.*tau12*tau22*tau13*tau21*f1f*f2um2+f1f*tau22*f2vm2*tau23*
     & tau11**2-1.*f1f*tau22**2*f2wm2*tau11**2+tau11*tau13*f1f*tau22**
     & 2*f2um2-1.*tau11*f1f*tau23*tau12*f2vm2*tau21-1.*tau11*tau13*
     & f1f*tau22*f2vm2*tau21-1.*tau11*f1f*tau22*f2um2*tau23*tau12+2.*
     & tau11*f1f*tau22*f2wm2*tau12*tau21+tau12*tau13*tau21**2*f1f*
     & f2vm2-1.*tau21**2*f1f*f2wm2*tau12**2+ttu12*f2vm2*tau21**2*a13*
     & tau12-1.*ttu12*f2vm2*tau21*tau23*a11*tau12-1.*ttu12*tau22*
     & f2vm2*tau21*a13*tau11+ttu12*tau22**2*f2um2*a13*tau11-1.*ttu12*
     & tau22*f2um2*a13*tau21*tau12-1.*ttu12*tau22*f2um2*tau23*a12*
     & tau11+f2um1*ttu21*tau21*tau12**2*a13+tau21**2*f2f*tau12**2*a13+
     & f2um2*a13*tau12**2*tau21*ttu22-1.*tau12*tau22*tau21*a13*ttu11*
     & f2um1-1.*tau21*f2f*tau23*tau12**2*a11-1.*tau12*tau21*a13*ttu21*
     & tau11*f2vm1-1.*tau12*tau21*tau23*a11*ttu11*f2vm1-1.*tau12*
     & tau22*ttu22*f2um2*a13*tau11-1.*tau12*tau13*f2um1*ttu21*tau21*
     & a12-1.*tau12*tau13*f2um2*a12*tau21*ttu22+tau12*tau21**2*a13*
     & ttu11*f2vm1+tau12*tau22*tau13*tau21*f2f*a11-1.*tau12*tau13*
     & tau21**2*f2f*a12+tau11*a13*ttu11*tau22**2*f2um1-1.*tau11*tau13*
     & tau22*f2vm2*ttu22*a11-1.*tau11*tau13*a11*tau22**2*f2f+tau11*
     & tau22*f2f*tau23*tau12*a11-1.*tau11*tau13*a11*ttu21*tau22*f2vm1-
     & 1.*tau11*tau21*a13*ttu11*tau22*f2vm1+tau11*tau21*tau13*a12*
     & tau22*f2f-2.*tau11*tau21*a13*tau12*tau22*f2f-1.*tau11*a13*
     & tau12*tau22*f2um1*ttu21-1.*tau11*a12*tau22*tau23*f2um1*ttu11+
     & tau11*tau21*a12*tau23*tau12*f2f+a13*tau22**2*tau11**2*f2f+
     & tau22*f2vm2*ttu22*a13*tau11**2-1.*a12*tau23*tau11**2*tau22*f2f-
     & 1.*tau11*tau21*ttu22*f2vm2*a13*tau12+a13*ttu21*tau11**2*tau22*
     & f2vm1+6.*ttu21*f2wm2*tau12**2*tau21*a11+tau22*tau11*f2wm2*
     & tau12*ttu22*a11+tau21*tau23*ttu11*f2vm1*tau11*a12+tau21*tau13*
     & a11*ttu11*tau22*f2vm1+tau22*f2vm2*tau23*tau11*ttu12*a11-1.*
     & tau21*tau12**2*f2wm2*ttu22*a11+tau21*a11*tau12*tau22*f2wm2*
     & ttu12-1.*ttu21*tau23*f2vm1*tau11**2*a12-6.*ttu21*f2vm2*tau23*
     & tau11**2*a12-6.*ttu21*f2um2*tau23*a11*tau12**2+ttu21*tau23*
     & f2vm1*tau11*tau12*a11-6.*ttu21*tau22*f2wm2*tau11*a11*tau12+6.*
     & ttu21*f2um2*tau23*a12*tau11*tau12+6.*ttu21*f2vm2*tau23*tau11*
     & a11*tau12-6.*tau21*ttu11*tau22*f2wm2*tau11*a12+6.*tau21*tau13*
     & ttu11*tau22*f2um2*a12+tau21*tau13*ttu21*f2vm1*tau11*a12+tau21*
     & a12*tau12*f2wm2*tau11*ttu22+tau22*tau13*f2um2*a12*tau11*ttu22+
     & 6.*tau22*tau13*f2um2*a11*ttu21*tau12+6.*tau22*tau13*f2vm2*
     & tau21*a11*ttu11-1.*tau22*f2wm2*tau11**2*a12*ttu22+tau22*tau13*
     & f2um1*ttu11*tau21*a12+tau22*tau13*f2um1*ttu21*tau12*a11-6.*
     & ttu21*tau12*f2wm2*tau11*a12*tau21-6.*ttu21*tau13*f2vm2*tau21*
     & a11*tau12+6.*tau22*f2um2*tau23*a11*ttu11*tau12-6.*tau22*f2wm2*
     & tau12*tau21*a11*ttu11-6.*tau22*f2vm2*tau23*tau11*a11*ttu11+
     & tau22*tau23*f2um1*ttu11*tau12*a11+tau22*tau21*f2wm2*ttu12*a12*
     & tau11+tau21*tau13*tau12*f2vm2*ttu22*a11-6.*tau21**2*tau13*
     & ttu11*f2vm2*a12+6.*tau21**2*ttu11*f2wm2*tau12*a12-1.*tau21**2*
     & tau13*ttu11*f2vm1*a12-1.*tau21**2*ttu12*f2wm2*tau12*a12+6.*
     & tau21*tau13*ttu21*f2vm2*a12*tau11-6.*ttu21*tau22*tau13*f2um2*
     & a12*tau11-1.*ttu21*tau23*f2um1*tau12**2*a11+ttu21*tau23*f2um1*
     & tau12*a12*tau11+6.*ttu21*tau22*f2wm2*tau11**2*a12-6.*tau22**2*
     & tau13*f2um2*a11*ttu11-1.*tau22**2*tau13*f2um1*ttu11*a11-1.*
     & tau22**2*f2wm2*ttu12*tau11*a11+6.*tau22**2*f2wm2*tau11*a11*
     & ttu11+tau21*tau23*ttu12*f2um2*a12*tau12+6.*tau21*tau23*ttu11*
     & f2vm2*a12*tau11-6.*tau21*tau23*ttu11*f2um2*a12*tau12)/(tau23*
     & tau12*f2vm1*tau21*a13*tau11+6.*tau23*tau12*f2wm2*tau11*a12*
     & tau21+tau23*tau12*f2wm1*tau11*tau21*a12-1.*f2wm1*tau21*tau12**
     & 2*tau23*a11+6.*tau23*tau12*f2vm2*tau21*a13*tau11-1.*tau23*
     & f2um1*tau12**2*tau21*a13+6.*tau13*f2vm2*tau21*tau23*a11*tau12-
     & 2.*tau13*tau23*f2vm1*tau11*tau21*a12-6.*f2wm2*tau12**2*tau21*
     & tau23*a11-6.*tau13*f2vm2*tau21**2*a13*tau12+tau13*f2vm1*tau21*
     & tau23*tau12*a11-1.*tau23**2*f2um1*tau12*a12*tau11+tau13*tau23*
     & f2um1*tau12*tau21*a12-12.*tau13*f2vm2*tau21*tau23*a12*tau11-6.*
     & tau13*f2wm2*tau12*tau21**2*a12-1.*tau22**2*tau13*f2um1*a13*
     & tau11+tau22*f2wm1*tau11*tau23*tau12*a11-6.*tau22*f2wm2*tau11**
     & 2*tau23*a12-6.*tau22*tau13**2*f2um2*a12*tau21-1.*tau22*tau13**
     & 2*f2um1*tau21*a12-6.*tau22*tau13**2*f2vm2*tau21*a11-1.*tau22*
     & tau13**2*f2vm1*tau21*a11-1.*tau22*f2wm1*tau11**2*a12*tau23-1.*
     & tau22*tau23*f2vm1*tau11**2*a13-12.*tau22*tau13*f2um2*tau23*a11*
     & tau12-1.*tau13*f2vm1*tau21**2*tau12*a13-2.*tau22*f2wm1*tau21*
     & tau12*a13*tau11+tau22*tau23*f2um1*tau12*a13*tau11-6.*f2um2*
     & tau23**2*a12*tau11*tau12-6.*f2um2*a13*tau21*tau12**2*tau23-6.*
     & f2vm2*tau23**2*tau11*a11*tau12+6.*tau13*f2um2*a12*tau21*tau23*
     & tau12+6.*tau22*tau13*f2um2*a13*tau21*tau12+6.*tau22*tau13*
     & f2wm2*tau12*tau21*a11+tau13**2*f2vm1*tau21**2*a12+tau22*tau13*
     & f2vm1*tau21*a13*tau11+tau22*tau13*f2wm1*tau11*tau21*a12+6.*
     & tau13**2*f2vm2*tau21**2*a12-6.*tau22**2*tau13*f2um2*a13*tau11+
     & tau22*tau13*f2wm1*tau21*tau12*a11+6.*tau22*tau13*f2vm2*tau21*
     & a13*tau11-2.*tau22*tau13*f2um1*a11*tau23*tau12+tau22*tau13*
     & f2um1*a12*tau11*tau23-1.*tau22**2*tau13*f2wm1*tau11*a11+tau22*
     & tau13*f2um1*tau21*tau12*a13-1.*tau23**2*f2vm1*tau11*tau12*a11+
     & 6.*tau22*f2wm2*tau11*tau23*a11*tau12-1.*tau13*f2wm1*tau21**2*
     & tau12*a12-6.*tau22*f2vm2*tau23*tau11**2*a13-6.*tau22**2*tau13*
     & f2wm2*tau11*a11+tau23**2*f2vm1*tau11**2*a12+6.*f2vm2*tau23**2*
     & tau11**2*a12+6.*f2um2*tau23**2*a11*tau12**2+6.*tau22*f2um2*a13*
     & tau11*tau23*tau12-12.*tau22*f2wm2*tau12*tau21*a13*tau11+6.*
     & tau22*tau13*f2wm2*tau11*a12*tau21+tau22*tau13*tau23*f2vm1*
     & tau11*a11+6.*tau22*tau13*f2vm2*tau23*tau11*a11+6.*tau22*tau13*
     & f2um2*tau23*a12*tau11+tau22**2*tau13**2*f2um1*a11+6.*f2wm2*
     & tau12**2*tau21**2*a13+tau23**2*f2um1*tau12**2*a11+f2wm1*tau21**
     & 2*tau12**2*a13+6.*tau22**2*f2wm2*tau11**2*a13+tau22**2*f2wm1*
     & tau11**2*a13+6.*tau22**2*tau13**2*f2um2*a11)

      u(i1-2*is1,i2-2*is2,i3-2*is3,ez) = (-1.*tau21**2*f2wm1*tau12**2*
     & f1f-1.*f2wm1*tau11**2*tau22**2*f1f+f2vm1*tau23*tau11**2*f1f*
     & tau22+tau11*tau13*f2um1*tau22**2*f1f-1.*tau11*tau21*f2vm1*f1f*
     & tau23*tau12-1.*tau11*tau21*tau13*f2vm1*f1f*tau22-1.*tau11*
     & f2um1*tau23*tau12*f1f*tau22+tau12*tau13*tau21**2*f2vm1*f1f-1.*
     & tau12*tau22*tau13*tau21*f2um1*f1f+tau21*f2um1*tau23*tau12**2*
     & f1f+2.*tau11*tau21*f2wm1*tau12*f1f*tau22-1.*ttu12*tau22*tau13*
     & f2um1*tau21*a12-6.*ttu12*tau22*tau13*f2vm2*tau21*a11+ttu12*
     & tau23*f2um1*tau12*tau21*a12-6.*ttu12*tau22*tau13*f2um2*a12*
     & tau21-1.*ttu12*tau22*tau13*f2vm1*tau21*a11-6.*ttu12*f2vm2*
     & tau21**2*a13*tau12+6.*ttu12*f2vm2*tau21*tau23*a11*tau12+6.*
     & ttu12*tau22*f2vm2*tau21*a13*tau11+ttu12*tau22*f2wm1*tau11*
     & tau21*a12+6.*ttu12*tau13*f2vm2*tau21**2*a12-6.*ttu12*tau22**2*
     & f2um2*a13*tau11+6.*ttu12*tau22*f2um2*a13*tau21*tau12+ttu12*
     & tau13*f2vm1*tau21**2*a12+ttu12*tau22*tau23*f2vm1*tau11*a11+
     & ttu12*tau22*f2wm1*tau21*tau12*a11+ttu12*tau22**2*tau13*f2um1*
     & a11-1.*ttu12*f2wm1*tau21**2*tau12*a12-1.*ttu12*tau22**2*f2wm1*
     & tau11*a11+6.*ttu12*tau22**2*tau13*f2um2*a11+6.*ttu12*tau22*
     & f2um2*tau23*a12*tau11-6.*f2um1*ttu21*tau21*tau12**2*a13-6.*
     & tau21**2*f2f*tau12**2*a13-6.*f2um2*a13*tau12**2*tau21*ttu22+6.*
     & tau12*tau22*tau21*a13*ttu11*f2um1+6.*tau21*f2f*tau23*tau12**2*
     & a11-1.*tau12*tau22*tau13*ttu22*f2um1*a11-6.*tau12*tau22*tau13*
     & ttu22*f2um2*a11+tau12*tau22*ttu22*f2wm1*tau11*a11-1.*tau12*
     & tau22*tau23*ttu12*f2um1*a11+6.*tau12*tau21*a13*ttu21*tau11*
     & f2vm1-1.*tau12*ttu22*tau23*f2vm1*tau11*a11+6.*tau12*tau21*
     & tau23*a11*ttu11*f2vm1-6.*tau12*tau22*tau23*ttu12*f2um2*a11+6.*
     & tau12*tau22*ttu22*f2um2*a13*tau11-6.*tau12*tau23*f2um1*ttu11*
     & tau21*a12+6.*tau12*tau13*f2um1*ttu21*tau21*a12-6.*tau12*f2wm1*
     & ttu21*tau11*tau21*a12+6.*tau12*tau13*f2um2*a12*tau21*ttu22-6.*
     & tau12*tau13*tau21*a11*ttu21*f2vm1+tau12*tau13*ttu22*f2vm1*
     & tau21*a11-6.*tau12*tau21**2*a13*ttu11*f2vm1+6.*tau12*tau21**2*
     & f2wm1*ttu11*a12-6.*tau12*tau22*tau13*tau21*f2f*a11-6.*tau12*
     & tau22*tau21*f2wm1*ttu11*a11+6.*tau12*tau13*tau21**2*f2f*a12-6.*
     & tau12*f2um2*a12*tau11*tau23*ttu22-6.*tau11*tau23*a11*ttu11*
     & tau22*f2vm1-1.*tau11*a12*ttu22*tau23*f2um1*tau12-6.*tau11*a13*
     & ttu11*tau22**2*f2um1+6.*tau11*tau13*tau22*f2vm2*ttu22*a11+6.*
     & tau11*tau13*a11*tau22**2*f2f-6.*tau11*tau22*f2f*tau23*tau12*
     & a11+6.*tau11*tau13*a11*ttu21*tau22*f2vm1+6.*tau11*tau21*a13*
     & ttu11*tau22*f2vm1-6.*tau11*tau21*tau13*a12*ttu22*f2vm2-6.*
     & tau11*tau21*tau13*a12*tau22*f2f+12.*tau11*tau21*a13*tau12*
     & tau22*f2f+tau11*tau21*a12*ttu22*f2wm1*tau12-1.*tau11*tau21*
     & tau13*a12*ttu22*f2vm1-6.*tau11*tau13*a12*tau22*f2um1*ttu21+6.*
     & tau11*a13*tau12*tau22*f2um1*ttu21+6.*tau11*a12*tau22*tau23*
     & f2um1*ttu11-6.*tau11*a11*ttu21*tau22*f2wm1*tau12+ttu22*f2um1*
     & a11*tau23*tau12**2-1.*ttu22*f2wm1*tau21*tau12**2*a11+6.*tau21*
     & a11*ttu21*f2wm1*tau12**2+6.*ttu22*f2um2*tau23*a11*tau12**2-6.*
     & tau11*tau21*a12*tau23*tau12*f2f-6.*a13*tau22**2*tau11**2*f2f+
     & a12*ttu22*tau23*f2vm1*tau11**2+6.*a12*tau22*f2wm1*ttu21*tau11**
     & 2-6.*tau22*f2vm2*ttu22*a13*tau11**2+6.*a12*ttu22*f2vm2*tau23*
     & tau11**2-1.*a12*ttu22*tau22*f2wm1*tau11**2+6.*a12*tau23*tau11**
     & 2*tau22*f2f+6.*tau11*a11*tau22**2*f2wm1*ttu11+tau11*tau13*a12*
     & ttu22*tau22*f2um1-6.*tau11*tau23*tau12*f2vm2*ttu22*a11-1.*
     & tau11*tau21*a12*tau23*ttu12*f2vm1-6.*tau11*tau21*a12*tau22*
     & f2wm1*ttu11+6.*tau11*tau21*ttu22*f2vm2*a13*tau12-6.*tau11*
     & tau21*a12*tau23*ttu12*f2vm2-6.*a13*ttu21*tau11**2*tau22*f2vm1)
     & /(tau23*tau12*f2vm1*tau21*a13*tau11+6.*tau23*tau12*f2wm2*tau11*
     & a12*tau21+tau23*tau12*f2wm1*tau11*tau21*a12-1.*f2wm1*tau21*
     & tau12**2*tau23*a11+6.*tau23*tau12*f2vm2*tau21*a13*tau11-1.*
     & tau23*f2um1*tau12**2*tau21*a13+6.*tau13*f2vm2*tau21*tau23*a11*
     & tau12-2.*tau13*tau23*f2vm1*tau11*tau21*a12-6.*f2wm2*tau12**2*
     & tau21*tau23*a11-6.*tau13*f2vm2*tau21**2*a13*tau12+tau13*f2vm1*
     & tau21*tau23*tau12*a11-1.*tau23**2*f2um1*tau12*a12*tau11+tau13*
     & tau23*f2um1*tau12*tau21*a12-12.*tau13*f2vm2*tau21*tau23*a12*
     & tau11-6.*tau13*f2wm2*tau12*tau21**2*a12-1.*tau22**2*tau13*
     & f2um1*a13*tau11+tau22*f2wm1*tau11*tau23*tau12*a11-6.*tau22*
     & f2wm2*tau11**2*tau23*a12-6.*tau22*tau13**2*f2um2*a12*tau21-1.*
     & tau22*tau13**2*f2um1*tau21*a12-6.*tau22*tau13**2*f2vm2*tau21*
     & a11-1.*tau22*tau13**2*f2vm1*tau21*a11-1.*tau22*f2wm1*tau11**2*
     & a12*tau23-1.*tau22*tau23*f2vm1*tau11**2*a13-12.*tau22*tau13*
     & f2um2*tau23*a11*tau12-1.*tau13*f2vm1*tau21**2*tau12*a13-2.*
     & tau22*f2wm1*tau21*tau12*a13*tau11+tau22*tau23*f2um1*tau12*a13*
     & tau11-6.*f2um2*tau23**2*a12*tau11*tau12-6.*f2um2*a13*tau21*
     & tau12**2*tau23-6.*f2vm2*tau23**2*tau11*a11*tau12+6.*tau13*
     & f2um2*a12*tau21*tau23*tau12+6.*tau22*tau13*f2um2*a13*tau21*
     & tau12+6.*tau22*tau13*f2wm2*tau12*tau21*a11+tau13**2*f2vm1*
     & tau21**2*a12+tau22*tau13*f2vm1*tau21*a13*tau11+tau22*tau13*
     & f2wm1*tau11*tau21*a12+6.*tau13**2*f2vm2*tau21**2*a12-6.*tau22**
     & 2*tau13*f2um2*a13*tau11+tau22*tau13*f2wm1*tau21*tau12*a11+6.*
     & tau22*tau13*f2vm2*tau21*a13*tau11-2.*tau22*tau13*f2um1*a11*
     & tau23*tau12+tau22*tau13*f2um1*a12*tau11*tau23-1.*tau22**2*
     & tau13*f2wm1*tau11*a11+tau22*tau13*f2um1*tau21*tau12*a13-1.*
     & tau23**2*f2vm1*tau11*tau12*a11+6.*tau22*f2wm2*tau11*tau23*a11*
     & tau12-1.*tau13*f2wm1*tau21**2*tau12*a12-6.*tau22*f2vm2*tau23*
     & tau11**2*a13-6.*tau22**2*tau13*f2wm2*tau11*a11+tau23**2*f2vm1*
     & tau11**2*a12+6.*f2vm2*tau23**2*tau11**2*a12+6.*f2um2*tau23**2*
     & a11*tau12**2+6.*tau22*f2um2*a13*tau11*tau23*tau12-12.*tau22*
     & f2wm2*tau12*tau21*a13*tau11+6.*tau22*tau13*f2wm2*tau11*a12*
     & tau21+tau22*tau13*tau23*f2vm1*tau11*a11+6.*tau22*tau13*f2vm2*
     & tau23*tau11*a11+6.*tau22*tau13*f2um2*tau23*a12*tau11+tau22**2*
     & tau13**2*f2um1*a11+6.*f2wm2*tau12**2*tau21**2*a13+tau23**2*
     & f2um1*tau12**2*a11+f2wm1*tau21**2*tau12**2*a13+6.*tau22**2*
     & f2wm2*tau11**2*a13+tau22**2*f2wm1*tau11**2*a13+6.*tau22**2*
     & tau13**2*f2um2*a11)

      u(i1-is1,i2-is2,i3-is3,ex) = -1.*(-1.*tau21*tau13*a12*tau22*
     & f2wm1*ttu11+tau21*tau13*a12*tau23*tau12*f2f+6.*tau13*tau22**2*
     & f2um2*a13*ttu11+2.*tau13*f1f*tau22*f2um2*tau23*tau12+tau13**2*
     & f1f*tau22*f2vm2*tau21-1.*tau13*tau23*tau12*f2vm2*ttu22*a11-1.*
     & f1f*tau23*tau12*f2wm2*tau11*tau22+f1f*tau23**2*tau12*f2vm2*
     & tau11-1.*f1f*tau23*tau12*tau13*f2vm2*tau21+f1f*tau23*tau12**2*
     & f2wm2*tau21-1.*tau13*f1f*tau22*f2wm2*tau12*tau21+tau13*f1f*
     & tau22**2*f2wm2*tau11-1.*f1f*tau23**2*tau12**2*f2um2-1.*tau13**
     & 2*f1f*tau22**2*f2um2-1.*tau13*f1f*tau22*f2vm2*tau23*tau11+
     & tau23**2*a11*tau12**2*f2f+tau13*a11*tau22**2*f2wm2*ttu12-1.*
     & a12*tau23**2*tau12*f2f*tau11-1.*a12*tau23*tau12*f2wm2*tau11*
     & ttu22-1.*a12*tau23*tau12*f2wm1*ttu21*tau11+tau23**2*a11*ttu11*
     & tau12*f2vm1+tau23**2*tau12*f2vm2*ttu12*a11+tau23*tau12**2*
     & f2wm2*ttu22*a11+a13*tau12*tau23*tau11*tau22*f2f+a11*ttu21*
     & tau23*tau12**2*f2wm1-1.*tau23*a11*tau12*tau22*f2wm1*ttu11-1.*
     & tau23*a11*tau12*tau22*f2wm2*ttu12-1.*tau13*tau22*f2vm2*ttu22*
     & a13*tau11+tau13*a12*tau23*tau11*tau22*f2f-1.*tau13*tau22*f2vm2*
     & tau23*ttu12*a11-1.*tau13*tau22*f2wm2*tau12*ttu22*a11-1.*tau13*
     & tau23*a11*ttu11*tau22*f2vm1-1.*tau13*a11*ttu21*tau23*tau12*
     & f2vm1-1.*tau13*a13*ttu21*tau11*tau22*f2vm1+tau13*a11*tau22**2*
     & f2wm1*ttu11-1.*tau13*a11*ttu21*tau22*f2wm1*tau12-1.*tau13*a13*
     & tau22**2*tau11*f2f+tau13**2*tau22*f2vm2*ttu22*a11+tau13**2*a11*
     & ttu21*tau22*f2vm1+tau13**2*a11*tau22**2*f2f-2.*tau13*tau22*f2f*
     & tau23*tau12*a11+tau22*f2wm1*ttu21*tau11*tau12*a13+tau23*ttu12*
     & tau22*f2um2*a13*tau12+6.*tau22*f2vm2*tau23*tau11*a13*ttu11-6.*
     & tau22*f2um2*a13*ttu11*tau23*tau12+6.*tau22*f2wm2*tau12*a13*
     & ttu21*tau11+tau23*tau11*a13*ttu11*tau22*f2vm1+6.*tau23*ttu11*
     & tau22*f2wm2*tau11*a12-6.*tau13*tau23*ttu11*tau22*f2um2*a12-6.*
     & tau13*ttu21*f2um2*a12*tau23*tau12+tau13*ttu21*tau23*f2vm1*
     & tau11*a12+6.*tau13*ttu21*f2vm2*tau23*a12*tau11-6.*tau13*ttu21*
     & tau22*f2wm2*tau11*a12+tau13*tau23*ttu12*tau22*f2um2*a12-6.*
     & tau13*ttu21*tau22*f2um2*a13*tau12+tau13*ttu22*tau22*f2um2*a13*
     & tau12+tau13*ttu22*f2um2*a12*tau23*tau12-1.*tau13**2*ttu22*
     & tau22*f2um2*a12+tau13*ttu22*tau22*f2wm2*tau11*a12+tau23*tau11*
     & ttu22*f2vm2*a13*tau12-6.*tau23*tau12*f2vm2*a13*ttu21*tau11+
     & tau23*tau11*a12*tau22*f2wm1*ttu11-6.*tau21*tau13**2*ttu21*
     & f2vm2*a12-1.*tau21*ttu21*f2wm1*tau12**2*a13-6.*tau21*ttu21*
     & f2wm2*tau12**2*a13-1.*tau21*tau13**2*ttu21*f2vm1*a12+tau21*
     & tau13*ttu21*f2vm1*tau12*a13+tau21*tau13*tau22*f2vm2*a13*ttu12+
     & 6.*tau21*tau13*tau23*ttu11*f2vm2*a12+6.*tau21*tau13*ttu21*
     & f2vm2*a13*tau12+6.*tau21*tau13*ttu21*f2wm2*tau12*a12-6.*tau21*
     & tau23*ttu11*f2wm2*tau12*a12+tau21*tau13*tau23*ttu11*f2vm1*a12-
     & 6.*tau21*tau13*tau22*f2vm2*a13*ttu11+6.*tau21*tau22*f2wm2*
     & tau12*a13*ttu11+tau21*tau22*f2wm1*ttu11*tau12*a13+tau21*tau23*
     & ttu12*f2wm2*tau12*a12+tau21*tau13*ttu21*f2wm1*tau12*a12-6.*
     & tau22**2*f2wm2*tau11*a13*ttu11-1.*tau21*tau13**2*a12*tau22*f2f-
     & 1.*tau21*tau13*a12*tau22*f2wm2*ttu12+tau21*tau13*a13*tau12*
     & tau22*f2f-1.*tau22**2*f2wm1*ttu11*a13*tau11-1.*tau23**2*ttu12*
     & f2um2*a12*tau12-1.*ttu22*f2um2*a13*tau12**2*tau23+6.*ttu21*
     & f2um2*a13*tau12**2*tau23-1.*tau21*tau23*tau12**2*f2f*a13-6.*
     & tau23**2*ttu11*f2vm2*a12*tau11+6.*tau23**2*ttu11*f2um2*a12*
     & tau12-1.*tau13*tau22**2*f2um2*a13*ttu12-1.*tau23**2*ttu11*
     & f2vm1*tau11*a12-1.*tau21*a13*ttu11*tau23*tau12*f2vm1-1.*tau21*
     & tau23*tau12*f2vm2*a13*ttu12+6.*tau13**2*ttu21*tau22*f2um2*a12)
     & /(tau23*tau12*f2vm1*tau21*a13*tau11+6.*tau23*tau12*f2wm2*tau11*
     & a12*tau21+tau23*tau12*f2wm1*tau11*tau21*a12-1.*f2wm1*tau21*
     & tau12**2*tau23*a11+6.*tau23*tau12*f2vm2*tau21*a13*tau11-1.*
     & tau23*f2um1*tau12**2*tau21*a13+6.*tau13*f2vm2*tau21*tau23*a11*
     & tau12-2.*tau13*tau23*f2vm1*tau11*tau21*a12-6.*f2wm2*tau12**2*
     & tau21*tau23*a11-6.*tau13*f2vm2*tau21**2*a13*tau12+tau13*f2vm1*
     & tau21*tau23*tau12*a11-1.*tau23**2*f2um1*tau12*a12*tau11+tau13*
     & tau23*f2um1*tau12*tau21*a12-12.*tau13*f2vm2*tau21*tau23*a12*
     & tau11-6.*tau13*f2wm2*tau12*tau21**2*a12-1.*tau22**2*tau13*
     & f2um1*a13*tau11+tau22*f2wm1*tau11*tau23*tau12*a11-6.*tau22*
     & f2wm2*tau11**2*tau23*a12-6.*tau22*tau13**2*f2um2*a12*tau21-1.*
     & tau22*tau13**2*f2um1*tau21*a12-6.*tau22*tau13**2*f2vm2*tau21*
     & a11-1.*tau22*tau13**2*f2vm1*tau21*a11-1.*tau22*f2wm1*tau11**2*
     & a12*tau23-1.*tau22*tau23*f2vm1*tau11**2*a13-12.*tau22*tau13*
     & f2um2*tau23*a11*tau12-1.*tau13*f2vm1*tau21**2*tau12*a13-2.*
     & tau22*f2wm1*tau21*tau12*a13*tau11+tau22*tau23*f2um1*tau12*a13*
     & tau11-6.*f2um2*tau23**2*a12*tau11*tau12-6.*f2um2*a13*tau21*
     & tau12**2*tau23-6.*f2vm2*tau23**2*tau11*a11*tau12+6.*tau13*
     & f2um2*a12*tau21*tau23*tau12+6.*tau22*tau13*f2um2*a13*tau21*
     & tau12+6.*tau22*tau13*f2wm2*tau12*tau21*a11+tau13**2*f2vm1*
     & tau21**2*a12+tau22*tau13*f2vm1*tau21*a13*tau11+tau22*tau13*
     & f2wm1*tau11*tau21*a12+6.*tau13**2*f2vm2*tau21**2*a12-6.*tau22**
     & 2*tau13*f2um2*a13*tau11+tau22*tau13*f2wm1*tau21*tau12*a11+6.*
     & tau22*tau13*f2vm2*tau21*a13*tau11-2.*tau22*tau13*f2um1*a11*
     & tau23*tau12+tau22*tau13*f2um1*a12*tau11*tau23-1.*tau22**2*
     & tau13*f2wm1*tau11*a11+tau22*tau13*f2um1*tau21*tau12*a13-1.*
     & tau23**2*f2vm1*tau11*tau12*a11+6.*tau22*f2wm2*tau11*tau23*a11*
     & tau12-1.*tau13*f2wm1*tau21**2*tau12*a12-6.*tau22*f2vm2*tau23*
     & tau11**2*a13-6.*tau22**2*tau13*f2wm2*tau11*a11+tau23**2*f2vm1*
     & tau11**2*a12+6.*f2vm2*tau23**2*tau11**2*a12+6.*f2um2*tau23**2*
     & a11*tau12**2+6.*tau22*f2um2*a13*tau11*tau23*tau12-12.*tau22*
     & f2wm2*tau12*tau21*a13*tau11+6.*tau22*tau13*f2wm2*tau11*a12*
     & tau21+tau22*tau13*tau23*f2vm1*tau11*a11+6.*tau22*tau13*f2vm2*
     & tau23*tau11*a11+6.*tau22*tau13*f2um2*tau23*a12*tau11+tau22**2*
     & tau13**2*f2um1*a11+6.*f2wm2*tau12**2*tau21**2*a13+tau23**2*
     & f2um1*tau12**2*a11+f2wm1*tau21**2*tau12**2*a13+6.*tau22**2*
     & f2wm2*tau11**2*a13+tau22**2*f2wm1*tau11**2*a13+6.*tau22**2*
     & tau13**2*f2um2*a11)

      u(i1-2*is1,i2-2*is2,i3-2*is3,ex) = (-1.*a12*tau23**2*ttu12*f2um1*
     & tau12-1.*tau21*a13*ttu12*tau22*f2wm1*tau12-6.*tau21*tau13*a12*
     & tau23*ttu12*f2vm2+6.*tau21*tau13*a12*tau22*f2wm1*ttu11-6.*
     & tau21*tau13*ttu22*f2vm2*a13*tau12-6.*tau21*tau13*a12*tau23*
     & tau12*f2f-6.*tau21*tau13*a12*ttu22*f2wm2*tau12-1.*tau21*tau13*
     & a12*tau23*ttu12*f2vm1-6.*tau21*a12*tau23*tau12*f2wm1*ttu11+
     & tau21*ttu22*f2wm1*tau12**2*a13-1.*tau13**2*a12*ttu22*tau22*
     & f2um1+6.*tau13*tau23*tau12*f2vm2*ttu22*a11+a13*ttu12*tau22**2*
     & f2wm1*tau11-6.*tau23**2*a11*tau12**2*f2f+2.*tau13*f2um1*tau23*
     & tau12*f1f*tau22-1.*tau13*f2vm1*tau23*tau11*f1f*tau22-6.*tau13*
     & a11*tau22**2*f2wm2*ttu12+6.*a12*tau23**2*tau12*f2f*tau11+6.*
     & a12*tau23*tau12*f2wm2*tau11*ttu22-1.*a12*tau23*ttu12*tau22*
     & f2wm1*tau11-6.*a12*tau23*tau11*tau22*f2wm2*ttu12+6.*a12*tau23*
     & tau12*f2wm1*ttu21*tau11+6.*a12*tau11*tau23**2*ttu12*f2vm2+6.*
     & a12*tau23**2*tau12*f2um1*ttu11+a12*tau11*tau23**2*ttu12*f2vm1-
     & 6.*tau23**2*a11*ttu11*tau12*f2vm1-1.*ttu22*tau23*f2um1*tau12**
     & 2*a13+ttu22*tau23*tau12*f2vm1*a13*tau11+f2vm1*tau23**2*tau11*
     & f1f*tau12-1.*f2um1*tau23**2*tau12**2*f1f-1.*tau13**2*f2um1*
     & tau22**2*f1f-6.*tau23**2*tau12*f2vm2*ttu12*a11+6.*tau23*tau12**
     & 2*f2um1*ttu21*a13-1.*f2wm1*tau12*f1f*tau22*tau23*tau11-6.*
     & tau23*tau12**2*f2wm2*ttu22*a11-6.*a13*tau12*tau23*tau11*tau22*
     & f2f-6.*a11*ttu21*tau23*tau12**2*f2wm1+a13*tau12*tau23*ttu12*
     & tau22*f2um1-6.*tau23*tau11*tau22*f2vm2*a13*ttu12-6.*a13*tau12*
     & tau22*tau23*f2um1*ttu11+6.*tau23*a11*tau12*tau22*f2wm1*ttu11+
     & 6.*tau23*a11*tau12*tau22*f2wm2*ttu12-1.*ttu22*tau22*f2wm1*
     & tau12*a13*tau11-6.*a13*tau12*tau22*f2wm2*tau11*ttu22-1.*tau23*
     & ttu12*tau22*f2vm1*a13*tau11+tau13*a12*tau23*ttu12*tau22*f2um1-
     & 6.*tau13*a12*tau22*tau23*f2um1*ttu11-6.*tau13*a12*tau23*tau12*
     & f2um1*ttu21-1.*tau13*a12*ttu22*tau23*f2vm1*tau11+tau13*a12*
     & ttu22*tau23*f2um1*tau12-6.*tau13*a12*tau22*f2wm1*ttu21*tau11+
     & 6.*tau13*tau22*f2vm2*ttu22*a13*tau11-6.*tau13*a12*ttu22*f2vm2*
     & tau23*tau11+tau13*a12*ttu22*tau22*f2wm1*tau11-6.*tau13*a12*
     & tau23*tau11*tau22*f2f+6.*tau13*tau22*f2vm2*tau23*ttu12*a11-6.*
     & tau13*a13*tau12*tau22*f2um1*ttu21+6.*tau13*tau22*f2wm2*tau12*
     & ttu22*a11+6.*tau13*tau23*a11*ttu11*tau22*f2vm1+tau13*ttu22*
     & tau22*f2um1*tau12*a13+6.*tau13*a11*ttu21*tau23*tau12*f2vm1+6.*
     & tau13*a13*ttu21*tau11*tau22*f2vm1-6.*tau13*a11*tau22**2*f2wm1*
     & ttu11+6.*tau13*a11*ttu21*tau22*f2wm1*tau12+6.*tau13*a13*tau22**
     & 2*tau11*f2f+6.*tau13*a13*ttu11*tau22**2*f2um1-1.*tau13*a13*
     & ttu12*tau22**2*f2um1-6.*a13*ttu21*tau11*tau23*tau12*f2vm1-6.*
     & tau13**2*tau22*f2vm2*ttu22*a11+6.*tau13**2*a12*tau22*f2um1*
     & ttu21-6.*tau13**2*a11*ttu21*tau22*f2vm1-6.*tau13**2*a11*tau22**
     & 2*f2f+tau13*f2wm1*tau11*tau22**2*f1f+tau21*f2wm1*tau12**2*f1f*
     & tau23+12.*tau13*tau22*f2f*tau23*tau12*a11+tau21*tau13**2*a12*
     & ttu22*f2vm1+6.*tau21*tau13**2*a12*tau22*f2f+tau21*a12*tau23*
     & ttu12*f2wm1*tau12-1.*tau21*tau13*a12*ttu22*f2wm1*tau12-1.*
     & tau21*tau13*ttu22*f2vm1*tau12*a13+tau21*tau13*a13*ttu12*tau22*
     & f2vm1-6.*tau21*tau13*a13*ttu11*tau22*f2vm1+6.*tau21*tau13*a12*
     & tau22*f2wm2*ttu12-6.*tau21*tau13*a13*tau12*tau22*f2f+6.*tau21*
     & tau13**2*a12*ttu22*f2vm2+6.*tau21*tau23*tau12**2*f2f*a13+6.*
     & tau21*ttu22*f2wm2*tau12**2*a13+6.*tau21*a13*ttu11*tau23*tau12*
     & f2vm1-6.*tau21*a13*tau12*tau22*f2wm2*ttu12+6.*tau21*tau23*
     & tau12*f2vm2*a13*ttu12-1.*tau21*tau13*f2wm1*tau12*f1f*tau22-1.*
     & tau21*tau13*f2vm1*f1f*tau23*tau12+tau21*tau13**2*f2vm1*f1f*
     & tau22+6.*a13*tau22**2*tau11*f2wm2*ttu12)/(tau23*tau12*f2vm1*
     & tau21*a13*tau11+6.*tau23*tau12*f2wm2*tau11*a12*tau21+tau23*
     & tau12*f2wm1*tau11*tau21*a12-1.*f2wm1*tau21*tau12**2*tau23*a11+
     & 6.*tau23*tau12*f2vm2*tau21*a13*tau11-1.*tau23*f2um1*tau12**2*
     & tau21*a13+6.*tau13*f2vm2*tau21*tau23*a11*tau12-2.*tau13*tau23*
     & f2vm1*tau11*tau21*a12-6.*f2wm2*tau12**2*tau21*tau23*a11-6.*
     & tau13*f2vm2*tau21**2*a13*tau12+tau13*f2vm1*tau21*tau23*tau12*
     & a11-1.*tau23**2*f2um1*tau12*a12*tau11+tau13*tau23*f2um1*tau12*
     & tau21*a12-12.*tau13*f2vm2*tau21*tau23*a12*tau11-6.*tau13*f2wm2*
     & tau12*tau21**2*a12-1.*tau22**2*tau13*f2um1*a13*tau11+tau22*
     & f2wm1*tau11*tau23*tau12*a11-6.*tau22*f2wm2*tau11**2*tau23*a12-
     & 6.*tau22*tau13**2*f2um2*a12*tau21-1.*tau22*tau13**2*f2um1*
     & tau21*a12-6.*tau22*tau13**2*f2vm2*tau21*a11-1.*tau22*tau13**2*
     & f2vm1*tau21*a11-1.*tau22*f2wm1*tau11**2*a12*tau23-1.*tau22*
     & tau23*f2vm1*tau11**2*a13-12.*tau22*tau13*f2um2*tau23*a11*tau12-
     & 1.*tau13*f2vm1*tau21**2*tau12*a13-2.*tau22*f2wm1*tau21*tau12*
     & a13*tau11+tau22*tau23*f2um1*tau12*a13*tau11-6.*f2um2*tau23**2*
     & a12*tau11*tau12-6.*f2um2*a13*tau21*tau12**2*tau23-6.*f2vm2*
     & tau23**2*tau11*a11*tau12+6.*tau13*f2um2*a12*tau21*tau23*tau12+
     & 6.*tau22*tau13*f2um2*a13*tau21*tau12+6.*tau22*tau13*f2wm2*
     & tau12*tau21*a11+tau13**2*f2vm1*tau21**2*a12+tau22*tau13*f2vm1*
     & tau21*a13*tau11+tau22*tau13*f2wm1*tau11*tau21*a12+6.*tau13**2*
     & f2vm2*tau21**2*a12-6.*tau22**2*tau13*f2um2*a13*tau11+tau22*
     & tau13*f2wm1*tau21*tau12*a11+6.*tau22*tau13*f2vm2*tau21*a13*
     & tau11-2.*tau22*tau13*f2um1*a11*tau23*tau12+tau22*tau13*f2um1*
     & a12*tau11*tau23-1.*tau22**2*tau13*f2wm1*tau11*a11+tau22*tau13*
     & f2um1*tau21*tau12*a13-1.*tau23**2*f2vm1*tau11*tau12*a11+6.*
     & tau22*f2wm2*tau11*tau23*a11*tau12-1.*tau13*f2wm1*tau21**2*
     & tau12*a12-6.*tau22*f2vm2*tau23*tau11**2*a13-6.*tau22**2*tau13*
     & f2wm2*tau11*a11+tau23**2*f2vm1*tau11**2*a12+6.*f2vm2*tau23**2*
     & tau11**2*a12+6.*f2um2*tau23**2*a11*tau12**2+6.*tau22*f2um2*a13*
     & tau11*tau23*tau12-12.*tau22*f2wm2*tau12*tau21*a13*tau11+6.*
     & tau22*tau13*f2wm2*tau11*a12*tau21+tau22*tau13*tau23*f2vm1*
     & tau11*a11+6.*tau22*tau13*f2vm2*tau23*tau11*a11+6.*tau22*tau13*
     & f2um2*tau23*a12*tau11+tau22**2*tau13**2*f2um1*a11+6.*f2wm2*
     & tau12**2*tau21**2*a13+tau23**2*f2um1*tau12**2*a11+f2wm1*tau21**
     & 2*tau12**2*a13+6.*tau22**2*f2wm2*tau11**2*a13+tau22**2*f2wm1*
     & tau11**2*a13+6.*tau22**2*tau13**2*f2um2*a11)

      u(i1-is1,i2-is2,i3-is3,ey) = (-1.*tau23**2*tau11*f1f*f2um2*tau12+
     & tau23*tau12*tau21*f1f*f2wm2*tau11+tau13**2*tau21**2*f1f*f2vm2+
     & tau23**2*tau11**2*f1f*f2vm2-1.*tau13*tau21**2*f1f*f2wm2*tau12+
     & tau13*tau21*f1f*f2um2*tau23*tau12-1.*tau22*tau23*tau11**2*f1f*
     & f2wm2-2.*tau13*tau21*f1f*f2vm2*tau23*tau11-1.*tau22*tau13**2*
     & tau21*f1f*f2um2+tau22*tau13*tau21*f1f*f2wm2*tau11-6.*tau13**2*
     & f2vm2*tau21*a11*ttu21-1.*tau13**2*f2um2*a12*tau21*ttu22+tau13*
     & f2wm2*tau11*a12*tau21*ttu22+tau13*f2um2*a13*tau12*tau21*ttu22-
     & 1.*tau13*tau21*f2f*tau23*tau12*a11+tau13*tau23*f2um1*ttu11*
     & tau21*a12-1.*tau13*f2vm2*tau21*ttu22*a13*tau11+tau13*f2um1*
     & ttu21*a12*tau11*tau23+tau13*f2um1*ttu21*tau21*tau12*a13-1.*
     & tau13*f2um1*ttu21*tau23*tau12*a11+tau13*tau21**2*f2f*tau12*a13-
     & 1.*tau13*tau21**2*f2wm2*ttu12*a12+6.*tau13*f2vm2*tau21*a13*
     & ttu21*tau11-1.*tau13*f2vm2*tau21*tau23*ttu12*a11-1.*tau13*
     & f2wm2*tau12*tau21*ttu22*a11-1.*tau13*f2vm2*tau23*tau11*ttu22*
     & a11+tau13*f2wm1*ttu21*tau11*tau21*a12+tau13*f2vm2*tau21**2*a13*
     & ttu12+tau22*tau13*tau23*tau11*f1f*f2um2-1.*tau23*tau12*tau21*
     & f2f*a13*tau11-6.*tau13*f2vm2*tau21**2*a13*ttu11+2.*tau13*tau21*
     & f2f*a12*tau11*tau23+6.*tau13*f2vm2*tau23*tau11*a11*ttu21+tau13*
     & f2um2*a12*tau21*tau23*ttu12+tau13*f2um2*a12*tau11*tau23*ttu22+
     & 6.*tau13*f2wm2*tau12*tau21*a11*ttu21-6.*tau13*f2um2*a11*ttu21*
     & tau23*tau12+6.*tau13*f2vm2*tau21*tau23*a11*ttu11-1.*tau13**2*
     & f2um1*ttu21*tau21*a12+tau13**2*f2vm2*tau21*ttu22*a11+6.*f2vm2*
     & tau23*tau11*a13*tau21*ttu11+tau23*tau11*f2wm1*tau12*ttu21*a11-
     & 1.*f2vm2*tau21*tau23*ttu12*a13*tau11-1.*f2wm2*tau11**2*a12*
     & tau23*ttu22+f2vm2*tau23*tau11**2*ttu22*a13+tau23**2*f2um1*
     & ttu11*tau12*a11-6.*f2wm2*tau12*tau21*a13*ttu21*tau11-1.*f2wm1*
     & ttu21*tau11*tau21*tau12*a13-1.*f2wm1*ttu21*tau11**2*a12*tau23-
     & 6.*f2vm2*tau23*tau11**2*a13*ttu21-1.*f2um2*tau23*a13*tau12*
     & tau11*ttu22-1.*tau21*f2wm1*ttu11*a11*tau23*tau12+f2vm2*tau23**
     & 2*tau11*ttu12*a11+tau21*f2wm2*ttu12*a12*tau11*tau23+tau23*
     & tau11*f2wm2*tau12*ttu22*a11+6.*f2um2*a13*ttu21*tau11*tau23*
     & tau12-1.*tau23*f2um1*ttu11*tau21*tau12*a13-1.*tau23**2*f2um1*
     & ttu11*a12*tau11+tau23**2*tau11*f2f*tau12*a11+tau21**2*f2wm1*
     & ttu11*tau12*a13+tau21*f2wm1*ttu11*a12*tau11*tau23-1.*tau23**2*
     & tau11**2*f2f*a12-1.*tau13**2*tau21**2*f2f*a12-1.*tau13*tau21**
     & 2*f2wm1*ttu11*a12+6.*f2um2*tau23**2*a11*ttu11*tau12+6.*f2wm2*
     & tau12*tau21**2*a13*ttu11-6.*f2wm2*tau12*tau21*tau23*a11*ttu11-
     & 1.*f2um2*a12*tau11*tau23**2*ttu12-6.*f2um2*a13*tau21*ttu11*
     & tau23*tau12-6.*f2vm2*tau23**2*tau11*a11*ttu11-1.*tau22*tau13*
     & tau21*f2f*a13*tau11-1.*tau22*tau13*tau23*f2um1*ttu11*a11+tau22*
     & tau13*tau21*f2wm2*ttu12*a11-6.*tau22*tau13*f2wm2*tau11*a11*
     & ttu21-1.*tau22*tau23*f2wm2*ttu12*tau11*a11-6.*tau22*tau13*
     & f2um2*a13*ttu21*tau11+6.*tau22*f2wm2*tau11**2*a13*ttu21+tau22*
     & f2wm1*ttu21*tau11**2*a13+tau22*tau23*tau11**2*f2f*a13+tau22*
     & tau13**2*f2um1*ttu21*a11+tau22*tau13**2*tau21*f2f*a11+6.*tau22*
     & tau13**2*f2um2*a11*ttu21+6.*tau22*f2wm2*tau11*tau23*a11*ttu11-
     & 1.*tau22*tau13*tau23*tau11*f2f*a11-1.*tau22*tau13*f2wm1*ttu21*
     & tau11*a11-1.*tau22*tau13*f2um1*ttu21*a13*tau11-1.*tau22*tau13*
     & f2um2*a13*ttu12*tau21+tau22*tau13*tau21*f2wm1*ttu11*a11-6.*
     & tau22*f2wm2*tau11*a13*tau21*ttu11+tau22*tau23*f2um1*ttu11*a13*
     & tau11+tau22*f2um2*tau23*a13*ttu12*tau11+6.*tau22*tau13*f2um2*
     & a13*tau21*ttu11-1.*tau22*tau21*f2wm1*ttu11*a13*tau11-6.*tau22*
     & tau13*f2um2*tau23*a11*ttu11)/(tau23*tau12*f2vm1*tau21*a13*
     & tau11+6.*tau23*tau12*f2wm2*tau11*a12*tau21+tau23*tau12*f2wm1*
     & tau11*tau21*a12-1.*f2wm1*tau21*tau12**2*tau23*a11+6.*tau23*
     & tau12*f2vm2*tau21*a13*tau11-1.*tau23*f2um1*tau12**2*tau21*a13+
     & 6.*tau13*f2vm2*tau21*tau23*a11*tau12-2.*tau13*tau23*f2vm1*
     & tau11*tau21*a12-6.*f2wm2*tau12**2*tau21*tau23*a11-6.*tau13*
     & f2vm2*tau21**2*a13*tau12+tau13*f2vm1*tau21*tau23*tau12*a11-1.*
     & tau23**2*f2um1*tau12*a12*tau11+tau13*tau23*f2um1*tau12*tau21*
     & a12-12.*tau13*f2vm2*tau21*tau23*a12*tau11-6.*tau13*f2wm2*tau12*
     & tau21**2*a12-1.*tau22**2*tau13*f2um1*a13*tau11+tau22*f2wm1*
     & tau11*tau23*tau12*a11-6.*tau22*f2wm2*tau11**2*tau23*a12-6.*
     & tau22*tau13**2*f2um2*a12*tau21-1.*tau22*tau13**2*f2um1*tau21*
     & a12-6.*tau22*tau13**2*f2vm2*tau21*a11-1.*tau22*tau13**2*f2vm1*
     & tau21*a11-1.*tau22*f2wm1*tau11**2*a12*tau23-1.*tau22*tau23*
     & f2vm1*tau11**2*a13-12.*tau22*tau13*f2um2*tau23*a11*tau12-1.*
     & tau13*f2vm1*tau21**2*tau12*a13-2.*tau22*f2wm1*tau21*tau12*a13*
     & tau11+tau22*tau23*f2um1*tau12*a13*tau11-6.*f2um2*tau23**2*a12*
     & tau11*tau12-6.*f2um2*a13*tau21*tau12**2*tau23-6.*f2vm2*tau23**
     & 2*tau11*a11*tau12+6.*tau13*f2um2*a12*tau21*tau23*tau12+6.*
     & tau22*tau13*f2um2*a13*tau21*tau12+6.*tau22*tau13*f2wm2*tau12*
     & tau21*a11+tau13**2*f2vm1*tau21**2*a12+tau22*tau13*f2vm1*tau21*
     & a13*tau11+tau22*tau13*f2wm1*tau11*tau21*a12+6.*tau13**2*f2vm2*
     & tau21**2*a12-6.*tau22**2*tau13*f2um2*a13*tau11+tau22*tau13*
     & f2wm1*tau21*tau12*a11+6.*tau22*tau13*f2vm2*tau21*a13*tau11-2.*
     & tau22*tau13*f2um1*a11*tau23*tau12+tau22*tau13*f2um1*a12*tau11*
     & tau23-1.*tau22**2*tau13*f2wm1*tau11*a11+tau22*tau13*f2um1*
     & tau21*tau12*a13-1.*tau23**2*f2vm1*tau11*tau12*a11+6.*tau22*
     & f2wm2*tau11*tau23*a11*tau12-1.*tau13*f2wm1*tau21**2*tau12*a12-
     & 6.*tau22*f2vm2*tau23*tau11**2*a13-6.*tau22**2*tau13*f2wm2*
     & tau11*a11+tau23**2*f2vm1*tau11**2*a12+6.*f2vm2*tau23**2*tau11**
     & 2*a12+6.*f2um2*tau23**2*a11*tau12**2+6.*tau22*f2um2*a13*tau11*
     & tau23*tau12-12.*tau22*f2wm2*tau12*tau21*a13*tau11+6.*tau22*
     & tau13*f2wm2*tau11*a12*tau21+tau22*tau13*tau23*f2vm1*tau11*a11+
     & 6.*tau22*tau13*f2vm2*tau23*tau11*a11+6.*tau22*tau13*f2um2*
     & tau23*a12*tau11+tau22**2*tau13**2*f2um1*a11+6.*f2wm2*tau12**2*
     & tau21**2*a13+tau23**2*f2um1*tau12**2*a11+f2wm1*tau21**2*tau12**
     & 2*a13+6.*tau22**2*f2wm2*tau11**2*a13+tau22**2*f2wm1*tau11**2*
     & a13+6.*tau22**2*tau13**2*f2um2*a11)

      u(i1-2*is1,i2-2*is2,i3-2*is3,ey) = -1.*(-1.*tau13*tau21**2*f2wm1*
     & tau12*f1f+tau13**2*tau21**2*f2vm1*f1f+tau23*tau12*tau21*f2wm1*
     & tau11*f1f-1.*tau23**2*tau11*f2um1*tau12*f1f-1.*tau22*tau23*
     & tau11**2*f2wm1*f1f-2.*tau13*tau21*f2vm1*tau23*tau11*f1f+tau13*
     & tau21*f2um1*tau23*tau12*f1f-1.*tau22*tau13**2*tau21*f2um1*f1f+
     & tau22*tau13*tau21*f2wm1*tau11*f1f+tau22*tau13*tau23*tau11*
     & f2um1*f1f+6.*tau22*tau13*tau21*a13*ttu11*f2um1+tau22*tau13*
     & tau23*ttu12*f2um1*a11+tau22*tau21*a13*ttu12*f2wm1*tau11+6.*
     & tau22*tau13*ttu22*f2wm2*tau11*a11+6.*tau22*tau23*tau11*a11*
     & f2wm1*ttu11-1.*tau22*tau13**2*ttu22*f2um1*a11-6.*tau22*tau13**
     & 2*ttu22*f2um2*a11-1.*tau22*ttu22*f2wm1*tau11**2*a13-6.*tau22*
     & ttu22*f2wm2*tau11**2*a13+6.*tau22*tau21*a13*tau11*f2wm2*ttu12-
     & 1.*tau22*tau23*ttu12*f2wm1*tau11*a11+tau22*tau13*ttu22*f2wm1*
     & tau11*a11+tau22*tau13*ttu22*f2um1*a13*tau11+6.*tau22*tau13*
     & tau23*ttu12*f2um2*a11+6.*tau22*tau13*ttu22*f2um2*a13*tau11-1.*
     & tau22*tau13*tau21*a13*ttu12*f2um1+tau13*tau21**2*a13*ttu12*
     & f2vm1-6.*tau13*tau21**2*a13*ttu11*f2vm1+6.*tau13*tau21*a13*
     & ttu21*tau11*f2vm1+6.*tau13*tau21*a11*ttu21*f2wm1*tau12-1.*
     & tau13*tau23*ttu12*f2vm1*tau21*a11-1.*tau13*ttu22*tau23*f2vm1*
     & tau11*a11+6.*tau13*ttu22*f2um2*tau23*a11*tau12+6.*tau13*tau21*
     & tau23*a11*ttu11*f2vm1-1.*tau13*ttu22*f2vm1*tau21*a13*tau11-1.*
     & tau13*ttu22*f2wm1*tau21*tau12*a11+tau13*ttu22*f2um1*a11*tau23*
     & tau12+6.*tau13*tau23*tau11*a11*ttu21*f2vm1-6.*tau13**2*tau21*
     & a11*ttu21*f2vm1+tau13**2*ttu22*f2vm1*tau21*a11-6.*tau21**2*a13*
     & tau12*f2wm2*ttu12+6.*tau23*tau11*a13*tau12*f2um1*ttu21-1.*
     & tau23*tau11*ttu22*f2um1*tau12*a13-6.*tau23*tau11**2*a13*ttu21*
     & f2vm1-6.*tau23**2*ttu12*f2um2*a11*tau12+tau21*ttu22*f2wm1*
     & tau12*a13*tau11+6.*tau21*a13*tau12*f2wm2*tau11*ttu22+ttu22*
     & tau23*f2vm1*tau11**2*a13-1.*tau23**2*ttu12*f2um1*a11*tau12-1.*
     & tau21**2*a13*ttu12*f2wm1*tau12+6.*tau21*tau23*a11*tau12*f2wm2*
     & ttu12+tau23*ttu12*f2wm1*tau21*tau12*a11+tau23**2*ttu12*f2vm1*
     & tau11*a11+6.*tau23*ttu12*f2um2*a13*tau21*tau12+6.*tau23*tau11*
     & tau21*a13*ttu11*f2vm1-1.*tau21*tau23*ttu12*f2vm1*a13*tau11-6.*
     & tau23**2*tau11*a11*ttu11*f2vm1+tau21*a13*tau12*tau23*ttu12*
     & f2um1+tau23**2*tau11**2*f2vm1*f1f+6.*tau13**2*f2um2*a12*tau21*
     & ttu22-6.*tau13*f2wm2*tau11*a12*tau21*ttu22-6.*tau13*f2um2*a13*
     & tau12*tau21*ttu22+6.*tau13*tau21*f2f*tau23*tau12*a11-6.*tau13*
     & tau23*f2um1*ttu11*tau21*a12-6.*tau13*f2um1*ttu21*a12*tau11*
     & tau23-6.*tau13*f2um1*ttu21*tau21*tau12*a13-6.*tau13*tau21**2*
     & f2f*tau12*a13+6.*tau13*tau21**2*f2wm2*ttu12*a12-6.*tau13*f2wm1*
     & ttu21*tau11*tau21*a12+6.*tau23*tau12*tau21*f2f*a13*tau11-12.*
     & tau13*tau21*f2f*a12*tau11*tau23-6.*tau13*f2um2*a12*tau21*tau23*
     & ttu12-6.*tau13*f2um2*a12*tau11*tau23*ttu22+6.*tau13**2*f2um1*
     & ttu21*tau21*a12-6.*tau23*tau11*f2wm1*tau12*ttu21*a11+6.*f2wm2*
     & tau11**2*a12*tau23*ttu22+6.*f2wm1*ttu21*tau11**2*a12*tau23-6.*
     & tau21*f2wm2*ttu12*a12*tau11*tau23-6.*tau23*tau11*f2wm2*tau12*
     & ttu22*a11+6.*tau23**2*f2um1*ttu11*a12*tau11-6.*tau23**2*tau11*
     & f2f*tau12*a11-6.*tau21*f2wm1*ttu11*a12*tau11*tau23+6.*tau23**2*
     & tau11**2*f2f*a12+6.*tau13**2*tau21**2*f2f*a12+6.*tau13*tau21**
     & 2*f2wm1*ttu11*a12+6.*f2um2*a12*tau11*tau23**2*ttu12+6.*tau22*
     & tau13*tau21*f2f*a13*tau11-6.*tau22*tau13*tau21*f2wm2*ttu12*a11-
     & 6.*tau22*tau23*tau11**2*f2f*a13-6.*tau22*tau13**2*tau21*f2f*
     & a11+6.*tau22*tau13*tau23*tau11*f2f*a11-6.*tau22*tau13*tau21*
     & f2wm1*ttu11*a11-6.*tau22*tau23*f2um1*ttu11*a13*tau11-6.*tau22*
     & f2um2*tau23*a13*ttu12*tau11)/(tau23*tau12*f2vm1*tau21*a13*
     & tau11+6.*tau23*tau12*f2wm2*tau11*a12*tau21+tau23*tau12*f2wm1*
     & tau11*tau21*a12-1.*f2wm1*tau21*tau12**2*tau23*a11+6.*tau23*
     & tau12*f2vm2*tau21*a13*tau11-1.*tau23*f2um1*tau12**2*tau21*a13+
     & 6.*tau13*f2vm2*tau21*tau23*a11*tau12-2.*tau13*tau23*f2vm1*
     & tau11*tau21*a12-6.*f2wm2*tau12**2*tau21*tau23*a11-6.*tau13*
     & f2vm2*tau21**2*a13*tau12+tau13*f2vm1*tau21*tau23*tau12*a11-1.*
     & tau23**2*f2um1*tau12*a12*tau11+tau13*tau23*f2um1*tau12*tau21*
     & a12-12.*tau13*f2vm2*tau21*tau23*a12*tau11-6.*tau13*f2wm2*tau12*
     & tau21**2*a12-1.*tau22**2*tau13*f2um1*a13*tau11+tau22*f2wm1*
     & tau11*tau23*tau12*a11-6.*tau22*f2wm2*tau11**2*tau23*a12-6.*
     & tau22*tau13**2*f2um2*a12*tau21-1.*tau22*tau13**2*f2um1*tau21*
     & a12-6.*tau22*tau13**2*f2vm2*tau21*a11-1.*tau22*tau13**2*f2vm1*
     & tau21*a11-1.*tau22*f2wm1*tau11**2*a12*tau23-1.*tau22*tau23*
     & f2vm1*tau11**2*a13-12.*tau22*tau13*f2um2*tau23*a11*tau12-1.*
     & tau13*f2vm1*tau21**2*tau12*a13-2.*tau22*f2wm1*tau21*tau12*a13*
     & tau11+tau22*tau23*f2um1*tau12*a13*tau11-6.*f2um2*tau23**2*a12*
     & tau11*tau12-6.*f2um2*a13*tau21*tau12**2*tau23-6.*f2vm2*tau23**
     & 2*tau11*a11*tau12+6.*tau13*f2um2*a12*tau21*tau23*tau12+6.*
     & tau22*tau13*f2um2*a13*tau21*tau12+6.*tau22*tau13*f2wm2*tau12*
     & tau21*a11+tau13**2*f2vm1*tau21**2*a12+tau22*tau13*f2vm1*tau21*
     & a13*tau11+tau22*tau13*f2wm1*tau11*tau21*a12+6.*tau13**2*f2vm2*
     & tau21**2*a12-6.*tau22**2*tau13*f2um2*a13*tau11+tau22*tau13*
     & f2wm1*tau21*tau12*a11+6.*tau22*tau13*f2vm2*tau21*a13*tau11-2.*
     & tau22*tau13*f2um1*a11*tau23*tau12+tau22*tau13*f2um1*a12*tau11*
     & tau23-1.*tau22**2*tau13*f2wm1*tau11*a11+tau22*tau13*f2um1*
     & tau21*tau12*a13-1.*tau23**2*f2vm1*tau11*tau12*a11+6.*tau22*
     & f2wm2*tau11*tau23*a11*tau12-1.*tau13*f2wm1*tau21**2*tau12*a12-
     & 6.*tau22*f2vm2*tau23*tau11**2*a13-6.*tau22**2*tau13*f2wm2*
     & tau11*a11+tau23**2*f2vm1*tau11**2*a12+6.*f2vm2*tau23**2*tau11**
     & 2*a12+6.*f2um2*tau23**2*a11*tau12**2+6.*tau22*f2um2*a13*tau11*
     & tau23*tau12-12.*tau22*f2wm2*tau12*tau21*a13*tau11+6.*tau22*
     & tau13*f2wm2*tau11*a12*tau21+tau22*tau13*tau23*f2vm1*tau11*a11+
     & 6.*tau22*tau13*f2vm2*tau23*tau11*a11+6.*tau22*tau13*f2um2*
     & tau23*a12*tau11+tau22**2*tau13**2*f2um1*a11+6.*f2wm2*tau12**2*
     & tau21**2*a13+tau23**2*f2um1*tau12**2*a11+f2wm1*tau21**2*tau12**
     & 2*a13+6.*tau22**2*f2wm2*tau11**2*a13+tau22**2*f2wm1*tau11**2*
     & a13+6.*tau22**2*tau13**2*f2um2*a11)


 ! *********** done *********************
             !  if( debug.gt.0 )then
             !
             !   write(*,'(" bc4:extrap: i1,i2,i3=",3i3," u(-1)=",3f8.2," u(-2)=",3f8.2)') i1,i2,i3,!          u(i1-is1,i2-is2,i3-is3,ex),u(i1-is1,i2-is2,i3-is3,ey),u(i1-is1,i2-is2,i3-is3,ez),!          u(i1-2*is1,i2-2*is2,i3-2*is3,ex),u(i1-2*is1,i2-2*is2,i3-2*is3,ey),u(i1-2*is1,i2-2*is2,i3-2*is3,ez)
             !  end if
               ! set to exact for testing
               ! OGF3D(i1-is1,i2-is2,i3-is3,t, uvm(0),uvm(1),uvm(2))
               ! OGF3D(i1-2*is1,i2-2*is2,i3-2*is3,t, uvm2(0),uvm2(1),uvm2(2))
               ! u(i1-is1,i2-is2,i3-is3,ex)=uvm(0)
               ! u(i1-is1,i2-is2,i3-is3,ey)=uvm(1)
               ! u(i1-is1,i2-is2,i3-is3,ez)=uvm(2)
               ! u(i1-2*is1,i2-2*is2,i3-2*is3,ex)=uvm2(0)
               ! u(i1-2*is1,i2-2*is2,i3-2*is3,ey)=uvm2(1)
               ! u(i1-2*is1,i2-2*is2,i3-2*is3,ez)=uvm2(2)
             else if( mask(i1,i2,i3).lt.0 )then
              ! ** NEW WAY **  *wdh
              ! extrapolate ghost points next to boundary interpolation points  *wdh* 2015/08/11
              if( .false. .and. t.le.dt )then
                write(*,'("--MX-- BC4 extrap ghost next to interp t,
     & dt=",2e12.3)') t,dt
              end if
               u(i1-is1,i2-is2,i3-is3,ex) = (5.*u(i1,i2,i3,ex)-10.*u(
     & i1+is1,i2+is2,i3+is3,ex)+10.*u(i1+2*is1,i2+2*is2,i3+2*is3,ex)-
     & 5.*u(i1+3*is1,i2+3*is2,i3+3*is3,ex)+u(i1+4*is1,i2+4*is2,i3+4*
     & is3,ex))
               u(i1-is1,i2-is2,i3-is3,ey) = (5.*u(i1,i2,i3,ey)-10.*u(
     & i1+is1,i2+is2,i3+is3,ey)+10.*u(i1+2*is1,i2+2*is2,i3+2*is3,ey)-
     & 5.*u(i1+3*is1,i2+3*is2,i3+3*is3,ey)+u(i1+4*is1,i2+4*is2,i3+4*
     & is3,ey))
               u(i1-is1,i2-is2,i3-is3,ez) = (5.*u(i1,i2,i3,ez)-10.*u(
     & i1+is1,i2+is2,i3+is3,ez)+10.*u(i1+2*is1,i2+2*is2,i3+2*is3,ez)-
     & 5.*u(i1+3*is1,i2+3*is2,i3+3*is3,ez)+u(i1+4*is1,i2+4*is2,i3+4*
     & is3,ez))
               u(i1-2*is1,i2-2*is2,i3-2*is3,ex) = (5.*u(i1-is1,i2-is2,
     & i3-is3,ex)-10.*u(i1-is1+is1,i2-is2+is2,i3-is3+is3,ex)+10.*u(i1-
     & is1+2*is1,i2-is2+2*is2,i3-is3+2*is3,ex)-5.*u(i1-is1+3*is1,i2-
     & is2+3*is2,i3-is3+3*is3,ex)+u(i1-is1+4*is1,i2-is2+4*is2,i3-is3+
     & 4*is3,ex))
               u(i1-2*is1,i2-2*is2,i3-2*is3,ey) = (5.*u(i1-is1,i2-is2,
     & i3-is3,ey)-10.*u(i1-is1+is1,i2-is2+is2,i3-is3+is3,ey)+10.*u(i1-
     & is1+2*is1,i2-is2+2*is2,i3-is3+2*is3,ey)-5.*u(i1-is1+3*is1,i2-
     & is2+3*is2,i3-is3+3*is3,ey)+u(i1-is1+4*is1,i2-is2+4*is2,i3-is3+
     & 4*is3,ey))
               u(i1-2*is1,i2-2*is2,i3-2*is3,ez) = (5.*u(i1-is1,i2-is2,
     & i3-is3,ez)-10.*u(i1-is1+is1,i2-is2+is2,i3-is3+is3,ez)+10.*u(i1-
     & is1+2*is1,i2-is2+2*is2,i3-is3+2*is3,ez)-5.*u(i1-is1+3*is1,i2-
     & is2+3*is2,i3-is3+3*is3,ez)+u(i1-is1+4*is1,i2-is2+4*is2,i3-is3+
     & 4*is3,ez))
             else if( .FALSE. .and. mask(i1,i2,i3).lt.0 )then
               ! **OLD WAY**
              ! QUESTION: August 8, 2015 -- is this accurate enough ??
              ! we need to assign ghost points that lie outside of interpolation points
              if( debug.gt.0 )then
                write(*,'(" **ghost-interp3d: grid,side,axis=",3i2,", 
     & i1,i2,i3=",3i4)') grid,side,axis,i1,i2,i3
              end if
              tau11=rsxy(i1,i2,i3,axisp1,0)
              tau12=rsxy(i1,i2,i3,axisp1,1)
              tau13=rsxy(i1,i2,i3,axisp1,2)
              tau21=rsxy(i1,i2,i3,axisp2,0)
              tau22=rsxy(i1,i2,i3,axisp2,1)
              tau23=rsxy(i1,i2,i3,axisp2,2)
              uex=u(i1,i2,i3,ex)
              uey=u(i1,i2,i3,ey)
              uez=u(i1,i2,i3,ez)
              a11 =(rsxy(i1,i2,i3,axis,0)/(rx(i1,i2,i3)*(sy(i1,i2,i3)*
     & tz(i1,i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(sz(i1,i2,
     & i3)*tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,i3)*(sx(
     & i1,i2,i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3))))
              a12 =(rsxy(i1,i2,i3,axis,1)/(rx(i1,i2,i3)*(sy(i1,i2,i3)*
     & tz(i1,i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(sz(i1,i2,
     & i3)*tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,i3)*(sx(
     & i1,i2,i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3))))
              a13 =(rsxy(i1,i2,i3,axis,2)/(rx(i1,i2,i3)*(sy(i1,i2,i3)*
     & tz(i1,i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(sz(i1,i2,
     & i3)*tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,i3)*(sx(
     & i1,i2,i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3))))
              a21 =(rsxy(i1,i2,i3,axisp1,0)/(rx(i1,i2,i3)*(sy(i1,i2,i3)
     & *tz(i1,i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(sz(i1,
     & i2,i3)*tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,i3)*(
     & sx(i1,i2,i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3))))
              a22 =(rsxy(i1,i2,i3,axisp1,1)/(rx(i1,i2,i3)*(sy(i1,i2,i3)
     & *tz(i1,i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(sz(i1,
     & i2,i3)*tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,i3)*(
     & sx(i1,i2,i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3))))
              a23 =(rsxy(i1,i2,i3,axisp1,2)/(rx(i1,i2,i3)*(sy(i1,i2,i3)
     & *tz(i1,i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(sz(i1,
     & i2,i3)*tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,i3)*(
     & sx(i1,i2,i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3))))
              a31 =(rsxy(i1,i2,i3,axisp2,0)/(rx(i1,i2,i3)*(sy(i1,i2,i3)
     & *tz(i1,i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(sz(i1,
     & i2,i3)*tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,i3)*(
     & sx(i1,i2,i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3))))
              a32 =(rsxy(i1,i2,i3,axisp2,1)/(rx(i1,i2,i3)*(sy(i1,i2,i3)
     & *tz(i1,i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(sz(i1,
     & i2,i3)*tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,i3)*(
     & sx(i1,i2,i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3))))
              a33 =(rsxy(i1,i2,i3,axisp2,2)/(rx(i1,i2,i3)*(sy(i1,i2,i3)
     & *tz(i1,i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(sz(i1,
     & i2,i3)*tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,i3)*(
     & sx(i1,i2,i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3))))
              a11m1 =(rsxy(i1-is1,i2-is2,i3-is3,axis,0)/(rx(i1-is1,i2-
     & is2,i3-is3)*(sy(i1-is1,i2-is2,i3-is3)*tz(i1-is1,i2-is2,i3-is3)-
     & sz(i1-is1,i2-is2,i3-is3)*ty(i1-is1,i2-is2,i3-is3))+ry(i1-is1,
     & i2-is2,i3-is3)*(sz(i1-is1,i2-is2,i3-is3)*tx(i1-is1,i2-is2,i3-
     & is3)-sx(i1-is1,i2-is2,i3-is3)*tz(i1-is1,i2-is2,i3-is3))+rz(i1-
     & is1,i2-is2,i3-is3)*(sx(i1-is1,i2-is2,i3-is3)*ty(i1-is1,i2-is2,
     & i3-is3)-sy(i1-is1,i2-is2,i3-is3)*tx(i1-is1,i2-is2,i3-is3))))
              a12m1 =(rsxy(i1-is1,i2-is2,i3-is3,axis,1)/(rx(i1-is1,i2-
     & is2,i3-is3)*(sy(i1-is1,i2-is2,i3-is3)*tz(i1-is1,i2-is2,i3-is3)-
     & sz(i1-is1,i2-is2,i3-is3)*ty(i1-is1,i2-is2,i3-is3))+ry(i1-is1,
     & i2-is2,i3-is3)*(sz(i1-is1,i2-is2,i3-is3)*tx(i1-is1,i2-is2,i3-
     & is3)-sx(i1-is1,i2-is2,i3-is3)*tz(i1-is1,i2-is2,i3-is3))+rz(i1-
     & is1,i2-is2,i3-is3)*(sx(i1-is1,i2-is2,i3-is3)*ty(i1-is1,i2-is2,
     & i3-is3)-sy(i1-is1,i2-is2,i3-is3)*tx(i1-is1,i2-is2,i3-is3))))
              a13m1 =(rsxy(i1-is1,i2-is2,i3-is3,axis,2)/(rx(i1-is1,i2-
     & is2,i3-is3)*(sy(i1-is1,i2-is2,i3-is3)*tz(i1-is1,i2-is2,i3-is3)-
     & sz(i1-is1,i2-is2,i3-is3)*ty(i1-is1,i2-is2,i3-is3))+ry(i1-is1,
     & i2-is2,i3-is3)*(sz(i1-is1,i2-is2,i3-is3)*tx(i1-is1,i2-is2,i3-
     & is3)-sx(i1-is1,i2-is2,i3-is3)*tz(i1-is1,i2-is2,i3-is3))+rz(i1-
     & is1,i2-is2,i3-is3)*(sx(i1-is1,i2-is2,i3-is3)*ty(i1-is1,i2-is2,
     & i3-is3)-sy(i1-is1,i2-is2,i3-is3)*tx(i1-is1,i2-is2,i3-is3))))
              a11p1 =(rsxy(i1+is1,i2+is2,i3+is3,axis,0)/(rx(i1+is1,i2+
     & is2,i3+is3)*(sy(i1+is1,i2+is2,i3+is3)*tz(i1+is1,i2+is2,i3+is3)-
     & sz(i1+is1,i2+is2,i3+is3)*ty(i1+is1,i2+is2,i3+is3))+ry(i1+is1,
     & i2+is2,i3+is3)*(sz(i1+is1,i2+is2,i3+is3)*tx(i1+is1,i2+is2,i3+
     & is3)-sx(i1+is1,i2+is2,i3+is3)*tz(i1+is1,i2+is2,i3+is3))+rz(i1+
     & is1,i2+is2,i3+is3)*(sx(i1+is1,i2+is2,i3+is3)*ty(i1+is1,i2+is2,
     & i3+is3)-sy(i1+is1,i2+is2,i3+is3)*tx(i1+is1,i2+is2,i3+is3))))
              a12p1 =(rsxy(i1+is1,i2+is2,i3+is3,axis,1)/(rx(i1+is1,i2+
     & is2,i3+is3)*(sy(i1+is1,i2+is2,i3+is3)*tz(i1+is1,i2+is2,i3+is3)-
     & sz(i1+is1,i2+is2,i3+is3)*ty(i1+is1,i2+is2,i3+is3))+ry(i1+is1,
     & i2+is2,i3+is3)*(sz(i1+is1,i2+is2,i3+is3)*tx(i1+is1,i2+is2,i3+
     & is3)-sx(i1+is1,i2+is2,i3+is3)*tz(i1+is1,i2+is2,i3+is3))+rz(i1+
     & is1,i2+is2,i3+is3)*(sx(i1+is1,i2+is2,i3+is3)*ty(i1+is1,i2+is2,
     & i3+is3)-sy(i1+is1,i2+is2,i3+is3)*tx(i1+is1,i2+is2,i3+is3))))
              a13p1 =(rsxy(i1+is1,i2+is2,i3+is3,axis,2)/(rx(i1+is1,i2+
     & is2,i3+is3)*(sy(i1+is1,i2+is2,i3+is3)*tz(i1+is1,i2+is2,i3+is3)-
     & sz(i1+is1,i2+is2,i3+is3)*ty(i1+is1,i2+is2,i3+is3))+ry(i1+is1,
     & i2+is2,i3+is3)*(sz(i1+is1,i2+is2,i3+is3)*tx(i1+is1,i2+is2,i3+
     & is3)-sx(i1+is1,i2+is2,i3+is3)*tz(i1+is1,i2+is2,i3+is3))+rz(i1+
     & is1,i2+is2,i3+is3)*(sx(i1+is1,i2+is2,i3+is3)*ty(i1+is1,i2+is2,
     & i3+is3)-sy(i1+is1,i2+is2,i3+is3)*tx(i1+is1,i2+is2,i3+is3))))
              a11m2 =(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,0)/(rx(i1-2*
     & is1,i2-2*is2,i3-2*is3)*(sy(i1-2*is1,i2-2*is2,i3-2*is3)*tz(i1-2*
     & is1,i2-2*is2,i3-2*is3)-sz(i1-2*is1,i2-2*is2,i3-2*is3)*ty(i1-2*
     & is1,i2-2*is2,i3-2*is3))+ry(i1-2*is1,i2-2*is2,i3-2*is3)*(sz(i1-
     & 2*is1,i2-2*is2,i3-2*is3)*tx(i1-2*is1,i2-2*is2,i3-2*is3)-sx(i1-
     & 2*is1,i2-2*is2,i3-2*is3)*tz(i1-2*is1,i2-2*is2,i3-2*is3))+rz(i1-
     & 2*is1,i2-2*is2,i3-2*is3)*(sx(i1-2*is1,i2-2*is2,i3-2*is3)*ty(i1-
     & 2*is1,i2-2*is2,i3-2*is3)-sy(i1-2*is1,i2-2*is2,i3-2*is3)*tx(i1-
     & 2*is1,i2-2*is2,i3-2*is3))))
              a12m2 =(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,1)/(rx(i1-2*
     & is1,i2-2*is2,i3-2*is3)*(sy(i1-2*is1,i2-2*is2,i3-2*is3)*tz(i1-2*
     & is1,i2-2*is2,i3-2*is3)-sz(i1-2*is1,i2-2*is2,i3-2*is3)*ty(i1-2*
     & is1,i2-2*is2,i3-2*is3))+ry(i1-2*is1,i2-2*is2,i3-2*is3)*(sz(i1-
     & 2*is1,i2-2*is2,i3-2*is3)*tx(i1-2*is1,i2-2*is2,i3-2*is3)-sx(i1-
     & 2*is1,i2-2*is2,i3-2*is3)*tz(i1-2*is1,i2-2*is2,i3-2*is3))+rz(i1-
     & 2*is1,i2-2*is2,i3-2*is3)*(sx(i1-2*is1,i2-2*is2,i3-2*is3)*ty(i1-
     & 2*is1,i2-2*is2,i3-2*is3)-sy(i1-2*is1,i2-2*is2,i3-2*is3)*tx(i1-
     & 2*is1,i2-2*is2,i3-2*is3))))
              a13m2 =(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,2)/(rx(i1-2*
     & is1,i2-2*is2,i3-2*is3)*(sy(i1-2*is1,i2-2*is2,i3-2*is3)*tz(i1-2*
     & is1,i2-2*is2,i3-2*is3)-sz(i1-2*is1,i2-2*is2,i3-2*is3)*ty(i1-2*
     & is1,i2-2*is2,i3-2*is3))+ry(i1-2*is1,i2-2*is2,i3-2*is3)*(sz(i1-
     & 2*is1,i2-2*is2,i3-2*is3)*tx(i1-2*is1,i2-2*is2,i3-2*is3)-sx(i1-
     & 2*is1,i2-2*is2,i3-2*is3)*tz(i1-2*is1,i2-2*is2,i3-2*is3))+rz(i1-
     & 2*is1,i2-2*is2,i3-2*is3)*(sx(i1-2*is1,i2-2*is2,i3-2*is3)*ty(i1-
     & 2*is1,i2-2*is2,i3-2*is3)-sy(i1-2*is1,i2-2*is2,i3-2*is3)*tx(i1-
     & 2*is1,i2-2*is2,i3-2*is3))))
              a11p2 =(rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axis,0)/(rx(i1+2*
     & is1,i2+2*is2,i3+2*is3)*(sy(i1+2*is1,i2+2*is2,i3+2*is3)*tz(i1+2*
     & is1,i2+2*is2,i3+2*is3)-sz(i1+2*is1,i2+2*is2,i3+2*is3)*ty(i1+2*
     & is1,i2+2*is2,i3+2*is3))+ry(i1+2*is1,i2+2*is2,i3+2*is3)*(sz(i1+
     & 2*is1,i2+2*is2,i3+2*is3)*tx(i1+2*is1,i2+2*is2,i3+2*is3)-sx(i1+
     & 2*is1,i2+2*is2,i3+2*is3)*tz(i1+2*is1,i2+2*is2,i3+2*is3))+rz(i1+
     & 2*is1,i2+2*is2,i3+2*is3)*(sx(i1+2*is1,i2+2*is2,i3+2*is3)*ty(i1+
     & 2*is1,i2+2*is2,i3+2*is3)-sy(i1+2*is1,i2+2*is2,i3+2*is3)*tx(i1+
     & 2*is1,i2+2*is2,i3+2*is3))))
              a12p2 =(rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axis,1)/(rx(i1+2*
     & is1,i2+2*is2,i3+2*is3)*(sy(i1+2*is1,i2+2*is2,i3+2*is3)*tz(i1+2*
     & is1,i2+2*is2,i3+2*is3)-sz(i1+2*is1,i2+2*is2,i3+2*is3)*ty(i1+2*
     & is1,i2+2*is2,i3+2*is3))+ry(i1+2*is1,i2+2*is2,i3+2*is3)*(sz(i1+
     & 2*is1,i2+2*is2,i3+2*is3)*tx(i1+2*is1,i2+2*is2,i3+2*is3)-sx(i1+
     & 2*is1,i2+2*is2,i3+2*is3)*tz(i1+2*is1,i2+2*is2,i3+2*is3))+rz(i1+
     & 2*is1,i2+2*is2,i3+2*is3)*(sx(i1+2*is1,i2+2*is2,i3+2*is3)*ty(i1+
     & 2*is1,i2+2*is2,i3+2*is3)-sy(i1+2*is1,i2+2*is2,i3+2*is3)*tx(i1+
     & 2*is1,i2+2*is2,i3+2*is3))))
              a13p2 =(rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axis,2)/(rx(i1+2*
     & is1,i2+2*is2,i3+2*is3)*(sy(i1+2*is1,i2+2*is2,i3+2*is3)*tz(i1+2*
     & is1,i2+2*is2,i3+2*is3)-sz(i1+2*is1,i2+2*is2,i3+2*is3)*ty(i1+2*
     & is1,i2+2*is2,i3+2*is3))+ry(i1+2*is1,i2+2*is2,i3+2*is3)*(sz(i1+
     & 2*is1,i2+2*is2,i3+2*is3)*tx(i1+2*is1,i2+2*is2,i3+2*is3)-sx(i1+
     & 2*is1,i2+2*is2,i3+2*is3)*tz(i1+2*is1,i2+2*is2,i3+2*is3))+rz(i1+
     & 2*is1,i2+2*is2,i3+2*is3)*(sx(i1+2*is1,i2+2*is2,i3+2*is3)*ty(i1+
     & 2*is1,i2+2*is2,i3+2*is3)-sy(i1+2*is1,i2+2*is2,i3+2*is3)*tx(i1+
     & 2*is1,i2+2*is2,i3+2*is3))))
              c11 = (rsxy(i1,i2,i3,axis,0)**2+rsxy(i1,i2,i3,axis,1)**2+
     & rsxy(i1,i2,i3,axis,2)**2)
              c22 = (rsxy(i1,i2,i3,axisp1,0)**2+rsxy(i1,i2,i3,axisp1,1)
     & **2+rsxy(i1,i2,i3,axisp1,2)**2)
              c33 = (rsxy(i1,i2,i3,axisp2,0)**2+rsxy(i1,i2,i3,axisp2,1)
     & **2+rsxy(i1,i2,i3,axisp2,2)**2)
              ! ***************************************************************************************
              ! Use one sided approximations as needed for expressions needing tangential derivatives
              ! ***************************************************************************************
              js1a=abs(js1)
              js2a=abs(js2)
              js3a=abs(js3)
              ks1a=abs(ks1)
              ks2a=abs(ks2)
              ks3a=abs(ks3)
              ! *** first do metric derivatives -- no need to worry about the mask value ****
              if( (i1-2*js1a).ge.md1a .and. (i1+2*js1a).le.md1b .and. (
     & i2-2*js2a).ge.md2a .and. (i2+2*js2a).le.md2b .and. (i3-2*js3a)
     & .ge.md3a .and. (i3+2*js3a).le.md3b .and. (i1-2*ks1a).ge.md1a 
     & .and. (i1+2*ks1a).le.md1b .and. (i2-2*ks2a).ge.md2a .and. (i2+
     & 2*ks2a).le.md2b .and. (i3-2*ks3a).ge.md3a .and. (i3+2*ks3a)
     & .le.md3b )then
                ! centered approximation is ok
                c1 = (rsxyx43(i1,i2,i3,axis,0)+rsxyy43(i1,i2,i3,axis,1)
     & +rsxyz43(i1,i2,i3,axis,2))
                c2 = (rsxyx43(i1,i2,i3,axisp1,0)+rsxyy43(i1,i2,i3,
     & axisp1,1)+rsxyz43(i1,i2,i3,axisp1,2))
                c3 = (rsxyx43(i1,i2,i3,axisp2,0)+rsxyy43(i1,i2,i3,
     & axisp2,1)+rsxyz43(i1,i2,i3,axisp2,2))
              else if( (i1-js1a).ge.md1a .and. (i1+js1a).le.md1b .and. 
     & (i2-js2a).ge.md2a .and. (i2+js2a).le.md2b .and. (i3-js3a)
     & .ge.md3a .and. (i3+js3a).le.md3b .and. (i1-ks1a).ge.md1a .and. 
     & (i1+ks1a).le.md1b .and. (i2-ks2a).ge.md2a .and. (i2+ks2a)
     & .le.md2b .and. (i3-ks3a).ge.md3a .and. (i3+ks3a).le.md3b )then
                ! use 2nd-order centered approximation
                c1 = (rsxyx23(i1,i2,i3,axis,0)+rsxyy23(i1,i2,i3,axis,1)
     & +rsxyz23(i1,i2,i3,axis,2))
                c2 = (rsxyx23(i1,i2,i3,axisp1,0)+rsxyy23(i1,i2,i3,
     & axisp1,1)+rsxyz23(i1,i2,i3,axisp1,2))
                c3 = (rsxyx23(i1,i2,i3,axisp2,0)+rsxyy23(i1,i2,i3,
     & axisp2,1)+rsxyz23(i1,i2,i3,axisp2,2))
              else if( (i1-3*js1a).ge.md1a .and. (i2-3*js2a).ge.md2a 
     & .and. (i3-3*js3a).ge.md3a )then
               ! one sided  2nd-order:
               c1 = 2.*(rsxyx23(i1-js1a,i2-js2a,i3-js3a,axis,0)+
     & rsxyy23(i1-js1a,i2-js2a,i3-js3a,axis,1)+rsxyz23(i1-js1a,i2-
     & js2a,i3-js3a,axis,2))-(rsxyx23(i1-2*js1a,i2-2*js2a,i3-2*js3a,
     & axis,0)+rsxyy23(i1-2*js1a,i2-2*js2a,i3-2*js3a,axis,1)+rsxyz23(
     & i1-2*js1a,i2-2*js2a,i3-2*js3a,axis,2))
               c2 = 2.*(rsxyx23(i1-js1a,i2-js2a,i3-js3a,axisp1,0)+
     & rsxyy23(i1-js1a,i2-js2a,i3-js3a,axisp1,1)+rsxyz23(i1-js1a,i2-
     & js2a,i3-js3a,axisp1,2))-(rsxyx23(i1-2*js1a,i2-2*js2a,i3-2*js3a,
     & axisp1,0)+rsxyy23(i1-2*js1a,i2-2*js2a,i3-2*js3a,axisp1,1)+
     & rsxyz23(i1-2*js1a,i2-2*js2a,i3-2*js3a,axisp1,2))
               c3 = 2.*(rsxyx23(i1-js1a,i2-js2a,i3-js3a,axisp2,0)+
     & rsxyy23(i1-js1a,i2-js2a,i3-js3a,axisp2,1)+rsxyz23(i1-js1a,i2-
     & js2a,i3-js3a,axisp2,2))-(rsxyx23(i1-2*js1a,i2-2*js2a,i3-2*js3a,
     & axisp2,0)+rsxyy23(i1-2*js1a,i2-2*js2a,i3-2*js3a,axisp2,1)+
     & rsxyz23(i1-2*js1a,i2-2*js2a,i3-2*js3a,axisp2,2))
              else if( (i1+3*js1a).le.md1b .and. (i2+3*js2a).le.md2b 
     & .and. (i3+3*js3a).le.md3b )then
               ! one sided  2nd-order:
               c1 = 2.*(rsxyx23(i1+js1a,i2+js2a,i3+js3a,axis,0)+
     & rsxyy23(i1+js1a,i2+js2a,i3+js3a,axis,1)+rsxyz23(i1+js1a,i2+
     & js2a,i3+js3a,axis,2))-(rsxyx23(i1+2*js1a,i2+2*js2a,i3+2*js3a,
     & axis,0)+rsxyy23(i1+2*js1a,i2+2*js2a,i3+2*js3a,axis,1)+rsxyz23(
     & i1+2*js1a,i2+2*js2a,i3+2*js3a,axis,2))
               c2 = 2.*(rsxyx23(i1+js1a,i2+js2a,i3+js3a,axisp1,0)+
     & rsxyy23(i1+js1a,i2+js2a,i3+js3a,axisp1,1)+rsxyz23(i1+js1a,i2+
     & js2a,i3+js3a,axisp1,2))-(rsxyx23(i1+2*js1a,i2+2*js2a,i3+2*js3a,
     & axisp1,0)+rsxyy23(i1+2*js1a,i2+2*js2a,i3+2*js3a,axisp1,1)+
     & rsxyz23(i1+2*js1a,i2+2*js2a,i3+2*js3a,axisp1,2))
               c3 = 2.*(rsxyx23(i1+js1a,i2+js2a,i3+js3a,axisp2,0)+
     & rsxyy23(i1+js1a,i2+js2a,i3+js3a,axisp2,1)+rsxyz23(i1+js1a,i2+
     & js2a,i3+js3a,axisp2,2))-(rsxyx23(i1+2*js1a,i2+2*js2a,i3+2*js3a,
     & axisp2,0)+rsxyy23(i1+2*js1a,i2+2*js2a,i3+2*js3a,axisp2,1)+
     & rsxyz23(i1+2*js1a,i2+2*js2a,i3+2*js3a,axisp2,2))
              else if( (i1-3*ks1a).ge.md1a .and. (i2-3*ks2a).ge.md2a 
     & .and. (i3-3*ks3a).ge.md3a )then
               ! one sided  2nd-order:  -- this case should not be needed?
               c1 = 2.*(rsxyx23(i1-ks1a,i2-ks2a,i3-ks3a,axis,0)+
     & rsxyy23(i1-ks1a,i2-ks2a,i3-ks3a,axis,1)+rsxyz23(i1-ks1a,i2-
     & ks2a,i3-ks3a,axis,2))-(rsxyx23(i1-2*ks1a,i2-2*ks2a,i3-2*ks3a,
     & axis,0)+rsxyy23(i1-2*ks1a,i2-2*ks2a,i3-2*ks3a,axis,1)+rsxyz23(
     & i1-2*ks1a,i2-2*ks2a,i3-2*ks3a,axis,2))
               c2 = 2.*(rsxyx23(i1-ks1a,i2-ks2a,i3-ks3a,axisp1,0)+
     & rsxyy23(i1-ks1a,i2-ks2a,i3-ks3a,axisp1,1)+rsxyz23(i1-ks1a,i2-
     & ks2a,i3-ks3a,axisp1,2))-(rsxyx23(i1-2*ks1a,i2-2*ks2a,i3-2*ks3a,
     & axisp1,0)+rsxyy23(i1-2*ks1a,i2-2*ks2a,i3-2*ks3a,axisp1,1)+
     & rsxyz23(i1-2*ks1a,i2-2*ks2a,i3-2*ks3a,axisp1,2))
               c3 = 2.*(rsxyx23(i1-ks1a,i2-ks2a,i3-ks3a,axisp2,0)+
     & rsxyy23(i1-ks1a,i2-ks2a,i3-ks3a,axisp2,1)+rsxyz23(i1-ks1a,i2-
     & ks2a,i3-ks3a,axisp2,2))-(rsxyx23(i1-2*ks1a,i2-2*ks2a,i3-2*ks3a,
     & axisp2,0)+rsxyy23(i1-2*ks1a,i2-2*ks2a,i3-2*ks3a,axisp2,1)+
     & rsxyz23(i1-2*ks1a,i2-2*ks2a,i3-2*ks3a,axisp2,2))
              else if( (i1+3*ks1a).le.md1b .and. (i2+3*ks2a).le.md2b 
     & .and. (i3+3*ks3a).le.md3b )then
               ! one sided  2nd-order: -- this case should not be needed?
               c1 = 2.*(rsxyx23(i1+ks1a,i2+ks2a,i3+ks3a,axis,0)+
     & rsxyy23(i1+ks1a,i2+ks2a,i3+ks3a,axis,1)+rsxyz23(i1+ks1a,i2+
     & ks2a,i3+ks3a,axis,2))-(rsxyx23(i1+2*ks1a,i2+2*ks2a,i3+2*ks3a,
     & axis,0)+rsxyy23(i1+2*ks1a,i2+2*ks2a,i3+2*ks3a,axis,1)+rsxyz23(
     & i1+2*ks1a,i2+2*ks2a,i3+2*ks3a,axis,2))
               c2 = 2.*(rsxyx23(i1+ks1a,i2+ks2a,i3+ks3a,axisp1,0)+
     & rsxyy23(i1+ks1a,i2+ks2a,i3+ks3a,axisp1,1)+rsxyz23(i1+ks1a,i2+
     & ks2a,i3+ks3a,axisp1,2))-(rsxyx23(i1+2*ks1a,i2+2*ks2a,i3+2*ks3a,
     & axisp1,0)+rsxyy23(i1+2*ks1a,i2+2*ks2a,i3+2*ks3a,axisp1,1)+
     & rsxyz23(i1+2*ks1a,i2+2*ks2a,i3+2*ks3a,axisp1,2))
               c3 = 2.*(rsxyx23(i1+ks1a,i2+ks2a,i3+ks3a,axisp2,0)+
     & rsxyy23(i1+ks1a,i2+ks2a,i3+ks3a,axisp2,1)+rsxyz23(i1+ks1a,i2+
     & ks2a,i3+ks3a,axisp2,2))-(rsxyx23(i1+2*ks1a,i2+2*ks2a,i3+2*ks3a,
     & axisp2,0)+rsxyy23(i1+2*ks1a,i2+2*ks2a,i3+2*ks3a,axisp2,1)+
     & rsxyz23(i1+2*ks1a,i2+2*ks2a,i3+2*ks3a,axisp2,2))
              else
               ! this case should not happen
               stop 40066
              end if
              ! ***** Now do "s"-derivatives *****
              ! warning -- the compiler could still try to evaluate the mask at an invalid point
              if( (i1-js1a).ge.md1a .and. (i2-js2a).ge.md2a .and. (i3-
     & js3a).ge.md3a .and. mask(i1-js1a,i2-js2a,i3-js3a).ne.0 .and. (
     & i1+js1a).le.md1b .and. (i2+js2a).le.md2b .and. (i3+js3a)
     & .le.md3b .and. mask(i1+js1a,i2+js2a,i3+js3a).ne.0 )then
                us=(u(i1+js1,i2+js2,i3+js3,ex)-u(i1-js1,i2-js2,i3-js3,
     & ex))/(2.*dsa)
                vs=(u(i1+js1,i2+js2,i3+js3,ey)-u(i1-js1,i2-js2,i3-js3,
     & ey))/(2.*dsa)
                ws=(u(i1+js1,i2+js2,i3+js3,ez)-u(i1-js1,i2-js2,i3-js3,
     & ez))/(2.*dsa)
                uss=(u(i1+js1,i2+js2,i3+js3,ex)-2.*u(i1,i2,i3,ex)+u(i1-
     & js1,i2-js2,i3-js3,ex))/(dsa**2)
                vss=(u(i1+js1,i2+js2,i3+js3,ey)-2.*u(i1,i2,i3,ey)+u(i1-
     & js1,i2-js2,i3-js3,ey))/(dsa**2)
                wss=(u(i1+js1,i2+js2,i3+js3,ez)-2.*u(i1,i2,i3,ez)+u(i1-
     & js1,i2-js2,i3-js3,ez))/(dsa**2)
               if( debug.gt.0 )then
                 call ogf3d(ep,xy(i1-js1a,i2-js2a,i3-js3a,0),xy(i1-
     & js1a,i2-js2a,i3-js3a,1),xy(i1-js1a,i2-js2a,i3-js3a,2),t,uvm(0),
     & uvm(1),uvm(2))
                 call ogf3d(ep,xy(i1+js1a,i2+js2a,i3+js3a,0),xy(i1+
     & js1a,i2+js2a,i3+js3a,1),xy(i1+js1a,i2+js2a,i3+js3a,2),t,uvp(0),
     & uvp(1),uvp(2))
                write(*,'(" **ghost-interp3d: use central-diff: us,
     & uss=",2f8.3," us2,usm,usp=",3f8.3)') us,uss,(uvp(0)-uvm(0))/(
     & 2.*dsb),(-(-3.*u(i1,i2,i3,ex)+4.*u(i1-js1a,i2-js2a,i3-js3a,ex)-
     & u(i1-2*js1a,i2-2*js2a,i3-2*js3a,ex))/(2.*dsb)),((-3.*u(i1,i2,
     & i3,ex)+4.*u(i1+js1a,i2+js2a,i3+js3a,ex)-u(i1+2*js1a,i2+2*js2a,
     & i3+2*js3a,ex))/(2.*dsb))
               end if
              else if( (i1-2*js1a).ge.md1a .and. (i2-2*js2a).ge.md2a 
     & .and. (i3-2*js3a).ge.md3a .and. mask(i1-js1a,i2-js2a,i3-js3a)
     & .ne.0 .and. mask(i1-2*js1a,i2-2*js2a,i3-js3a).ne.0 )then
               ! 2nd-order one-sided: ** note ** use ds not dsa
               us = (-(-3.*u(i1,i2,i3,ex)+4.*u(i1-js1a,i2-js2a,i3-js3a,
     & ex)-u(i1-2*js1a,i2-2*js2a,i3-2*js3a,ex))/(2.*dsb))
               vs = (-(-3.*u(i1,i2,i3,ey)+4.*u(i1-js1a,i2-js2a,i3-js3a,
     & ey)-u(i1-2*js1a,i2-2*js2a,i3-2*js3a,ey))/(2.*dsb))
               ws = (-(-3.*u(i1,i2,i3,ez)+4.*u(i1-js1a,i2-js2a,i3-js3a,
     & ez)-u(i1-2*js1a,i2-2*js2a,i3-2*js3a,ez))/(2.*dsb))
               uss = ((2.*u(i1,i2,i3,ex)-5.*u(i1-js1a,i2-js2a,i3-js3a,
     & ex)+4.*u(i1-2*js1a,i2-2*js2a,i3-2*js3a,ex)-u(i1-3*js1a,i2-3*
     & js2a,i3-3*js3a,ex))/(dsb**2))
               vss = ((2.*u(i1,i2,i3,ey)-5.*u(i1-js1a,i2-js2a,i3-js3a,
     & ey)+4.*u(i1-2*js1a,i2-2*js2a,i3-2*js3a,ey)-u(i1-3*js1a,i2-3*
     & js2a,i3-3*js3a,ey))/(dsb**2))
               wss = ((2.*u(i1,i2,i3,ez)-5.*u(i1-js1a,i2-js2a,i3-js3a,
     & ez)+4.*u(i1-2*js1a,i2-2*js2a,i3-2*js3a,ez)-u(i1-3*js1a,i2-3*
     & js2a,i3-3*js3a,ez))/(dsb**2))
               if( debug.gt.0 )then
                write(*,'(" **ghost-interp3d: use left-difference: us,
     & uss=",2e10.2," us1=",e10.2," js1,js2=",2i2)') us,uss,(u(i1,i2,
     & i3,ex)-u(i1-js1a,i2-js2a,i3-js3a,ex))/dsb,js1,js2
               end if
              else if( (i1+2*js1a).le.md1b .and. (i2+2*js2a).le.md2b 
     & .and.  (i3+2*js3a).le.md3b .and.  mask(i1+js1a,i2+js2a,i3+js3a)
     & .ne.0 .and. mask(i1+2*js1a,i2+2*js2a,i3+2*js3a).ne.0 )then
               ! 2nd-order one-sided:
               us = ((-3.*u(i1,i2,i3,ex)+4.*u(i1+js1a,i2+js2a,i3+js3a,
     & ex)-u(i1+2*js1a,i2+2*js2a,i3+2*js3a,ex))/(2.*dsb))
               vs = ((-3.*u(i1,i2,i3,ey)+4.*u(i1+js1a,i2+js2a,i3+js3a,
     & ey)-u(i1+2*js1a,i2+2*js2a,i3+2*js3a,ey))/(2.*dsb))
               ws = ((-3.*u(i1,i2,i3,ez)+4.*u(i1+js1a,i2+js2a,i3+js3a,
     & ez)-u(i1+2*js1a,i2+2*js2a,i3+2*js3a,ez))/(2.*dsb))
               uss = ((2.*u(i1,i2,i3,ex)-5.*u(i1+js1a,i2+js2a,i3+js3a,
     & ex)+4.*u(i1+2*js1a,i2+2*js2a,i3+2*js3a,ex)-u(i1+3*js1a,i2+3*
     & js2a,i3+3*js3a,ex))/(dsb**2))
               vss = ((2.*u(i1,i2,i3,ey)-5.*u(i1+js1a,i2+js2a,i3+js3a,
     & ey)+4.*u(i1+2*js1a,i2+2*js2a,i3+2*js3a,ey)-u(i1+3*js1a,i2+3*
     & js2a,i3+3*js3a,ey))/(dsb**2))
               wss = ((2.*u(i1,i2,i3,ez)-5.*u(i1+js1a,i2+js2a,i3+js3a,
     & ez)+4.*u(i1+2*js1a,i2+2*js2a,i3+2*js3a,ez)-u(i1+3*js1a,i2+3*
     & js2a,i3+3*js3a,ez))/(dsb**2))
               if( debug.gt.0 )then
                write(*,'(" **ghost-interp3d: use right-difference: us,
     & uss=",2e10.2," us1=",e10.2," js1,js2=",2i2)') us,uss,(u(i1+
     & js1a,i2+js2a,i3+js3a,ex)-u(i1,i2,i3,ex))/dsb,js1,js2
               end if
              else
                ! this case shouldn't matter
                us=0.
                vs=0.
                ws=0.
                uss=0.
                vss=0.
                wss=0.
              end if
              ! **** t - derivatives ****
              if( (i1-ks1a).ge.md1a .and. (i2-ks2a).ge.md2a .and. (i3-
     & ks3a).ge.md3a .and. mask(i1-ks1a,i2-ks2a,i3-ks3a).ne.0 .and. (
     & i1+ks1a).le.md1b .and. (i2+ks2a).le.md2b .and. (i3+ks3a)
     & .le.md3b .and. mask(i1+ks1a,i2+ks2a,i3+ks3a).ne.0 )then
                ut=(u(i1+ks1,i2+ks2,i3+ks3,ex)-u(i1-ks1,i2-ks2,i3-ks3,
     & ex))/(2.*dta)
                vt=(u(i1+ks1,i2+ks2,i3+ks3,ey)-u(i1-ks1,i2-ks2,i3-ks3,
     & ey))/(2.*dta)
                wt=(u(i1+ks1,i2+ks2,i3+ks3,ez)-u(i1-ks1,i2-ks2,i3-ks3,
     & ez))/(2.*dta)
                utt=(u(i1+ks1,i2+ks2,i3+ks3,ex)-2.*u(i1,i2,i3,ex)+u(i1-
     & ks1,i2-ks2,i3-ks3,ex))/(dta**2)
                vtt=(u(i1+ks1,i2+ks2,i3+ks3,ey)-2.*u(i1,i2,i3,ey)+u(i1-
     & ks1,i2-ks2,i3-ks3,ey))/(dta**2)
                wtt=(u(i1+ks1,i2+ks2,i3+ks3,ez)-2.*u(i1,i2,i3,ez)+u(i1-
     & ks1,i2-ks2,i3-ks3,ez))/(dta**2)
               if( debug.gt.0 )then
                 call ogf3d(ep,xy(i1-ks1a,i2-ks2a,i3-ks3a,0),xy(i1-
     & ks1a,i2-ks2a,i3-ks3a,1),xy(i1-ks1a,i2-ks2a,i3-ks3a,2),t,uvm(0),
     & uvm(1),uvm(2))
                 call ogf3d(ep,xy(i1+ks1a,i2+ks2a,i3+ks3a,0),xy(i1+
     & ks1a,i2+ks2a,i3+ks3a,1),xy(i1+ks1a,i2+ks2a,i3+ks3a,2),t,uvp(0),
     & uvp(1),uvp(2))
                write(*,'(" **ghost-interp3d: use central-diff: ut,
     & utt=",2f8.3," ut2=",f8.3)') ut,utt,(uvp(0)-uvm(0))/(2.*dtb)
               end if
              else if( (i1-2*ks1a).ge.md1a .and. (i2-2*ks2a).ge.md2a 
     & .and. (i3-2*ks3a).ge.md3a .and. mask(i1-ks1a,i2-ks2a,i3-ks3a)
     & .ne.0 .and. mask(i1-2*ks1a,i2-2*ks2a,i3-ks3a).ne.0 )then
               ! 2nd-order one-sided:
               ut = (-(-3.*u(i1,i2,i3,ex)+4.*u(i1-ks1a,i2-ks2a,i3-ks3a,
     & ex)-u(i1-2*ks1a,i2-2*ks2a,i3-2*ks3a,ex))/(2.*dtb))
               vt = (-(-3.*u(i1,i2,i3,ey)+4.*u(i1-ks1a,i2-ks2a,i3-ks3a,
     & ey)-u(i1-2*ks1a,i2-2*ks2a,i3-2*ks3a,ey))/(2.*dtb))
               wt = (-(-3.*u(i1,i2,i3,ez)+4.*u(i1-ks1a,i2-ks2a,i3-ks3a,
     & ez)-u(i1-2*ks1a,i2-2*ks2a,i3-2*ks3a,ez))/(2.*dtb))
               utt = ((2.*u(i1,i2,i3,ex)-5.*u(i1-ks1a,i2-ks2a,i3-ks3a,
     & ex)+4.*u(i1-2*ks1a,i2-2*ks2a,i3-2*ks3a,ex)-u(i1-3*ks1a,i2-3*
     & ks2a,i3-3*ks3a,ex))/(dtb**2))
               vtt = ((2.*u(i1,i2,i3,ey)-5.*u(i1-ks1a,i2-ks2a,i3-ks3a,
     & ey)+4.*u(i1-2*ks1a,i2-2*ks2a,i3-2*ks3a,ey)-u(i1-3*ks1a,i2-3*
     & ks2a,i3-3*ks3a,ey))/(dtb**2))
               wtt = ((2.*u(i1,i2,i3,ez)-5.*u(i1-ks1a,i2-ks2a,i3-ks3a,
     & ez)+4.*u(i1-2*ks1a,i2-2*ks2a,i3-2*ks3a,ez)-u(i1-3*ks1a,i2-3*
     & ks2a,i3-3*ks3a,ez))/(dtb**2))
               if( debug.gt.0 )then
                write(*,'(" **ghost-interp3d: use left-difference: ut,
     & utt=",2e10.2," ut1=",e10.2," kt1,kt2=",2i2)') ut,utt,(u(i1,i2,
     & i3,ex)-u(i1-ks1a,i2-ks2a,i3-ks3a,ex))/dtb,ks1,ks2
               end if
              else if( (i1+2*ks1a).le.md1b .and. (i2+2*ks2a).le.md2b 
     & .and.  (i3+2*ks3a).le.md3b .and.  mask(i1+ks1a,i2+ks2a,i3+ks3a)
     & .ne.0 .and. mask(i1+2*ks1a,i2+2*ks2a,i3+2*ks3a).ne.0 )then
               ! 2nd-order one-sided:
               ut = ((-3.*u(i1,i2,i3,ex)+4.*u(i1+ks1a,i2+ks2a,i3+ks3a,
     & ex)-u(i1+2*ks1a,i2+2*ks2a,i3+2*ks3a,ex))/(2.*dtb))
               vt = ((-3.*u(i1,i2,i3,ey)+4.*u(i1+ks1a,i2+ks2a,i3+ks3a,
     & ey)-u(i1+2*ks1a,i2+2*ks2a,i3+2*ks3a,ey))/(2.*dtb))
               wt = ((-3.*u(i1,i2,i3,ez)+4.*u(i1+ks1a,i2+ks2a,i3+ks3a,
     & ez)-u(i1+2*ks1a,i2+2*ks2a,i3+2*ks3a,ez))/(2.*dtb))
               utt = ((2.*u(i1,i2,i3,ex)-5.*u(i1+ks1a,i2+ks2a,i3+ks3a,
     & ex)+4.*u(i1+2*ks1a,i2+2*ks2a,i3+2*ks3a,ex)-u(i1+3*ks1a,i2+3*
     & ks2a,i3+3*ks3a,ex))/(dtb**2))
               vtt = ((2.*u(i1,i2,i3,ey)-5.*u(i1+ks1a,i2+ks2a,i3+ks3a,
     & ey)+4.*u(i1+2*ks1a,i2+2*ks2a,i3+2*ks3a,ey)-u(i1+3*ks1a,i2+3*
     & ks2a,i3+3*ks3a,ey))/(dtb**2))
               wtt = ((2.*u(i1,i2,i3,ez)-5.*u(i1+ks1a,i2+ks2a,i3+ks3a,
     & ez)+4.*u(i1+2*ks1a,i2+2*ks2a,i3+2*ks3a,ez)-u(i1+3*ks1a,i2+3*
     & ks2a,i3+3*ks3a,ez))/(dtb**2))
               if( debug.gt.0 )then
                 call ogf3d(ep,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,
     & i3,2),t,uv0(0),uv0(1),uv0(2))
                 call ogf3d(ep,xy(i1+ks1a,i2+ks2a,i3+ks3a,0),xy(i1+
     & ks1a,i2+ks2a,i3+ks3a,1),xy(i1+ks1a,i2+ks2a,i3+ks3a,2),t,uvp(0),
     & uvp(1),uvp(2))
                 call ogf3d(ep,xy(i1+2*ks1a,i2+2*ks2a,i3+2*ks3a,0),xy(
     & i1+2*ks1a,i2+2*ks2a,i3+2*ks3a,1),xy(i1+2*ks1a,i2+2*ks2a,i3+2*
     & ks3a,2),t,uvp2(0),uvp2(1),uvp2(2))
                write(*,'(" **ghost-interp3d: use right-diff: ut,utt=",
     & 2f8.3," ut1,ut2=",2f8.3," dta,dtb=",2f7.4)') ut,utt,(u(i1+ks1a,
     & i2+ks2a,i3+ks3a,ex)-u(i1,i2,i3,ex))/dtb,(4.*uvp(0)-3.*uv0(0)-
     & uvp2(0))/(2.*dtb),dta,dtb
               end if
              ! write(*,'(" **ghost-interp: use right-difference: ut,utt=",2e10.2)') ut,utt
              else
                ! this case shouldn't matter
                ut=0.
                vt=0.
                wt=0.
                utt=0.
                vtt=0.
                wtt=0.
              end if
              Da1DotU=0.
              tau1DotUtt=0.
              tau2DotUtt=0.
              gIVf1=0.
              gIVf2=0.
              ! Compute a21s, a31t, ... for RHS to div equation
              if( forcingOption.ne.0 )then
               if( (i1-js1a).ge.md1a .and. (i1+js1a).le.md1b .and. (i2-
     & js2a).ge.md2a .and. (i2+js2a).le.md2b .and. (i3-js3a).ge.md3a 
     & .and. (i3+js3a).le.md3b )then
                a21s = ((rsxy(i1+js1,i2+js2,i3+js3,axisp1,0)/(rx(i1+
     & js1,i2+js2,i3+js3)*(sy(i1+js1,i2+js2,i3+js3)*tz(i1+js1,i2+js2,
     & i3+js3)-sz(i1+js1,i2+js2,i3+js3)*ty(i1+js1,i2+js2,i3+js3))+ry(
     & i1+js1,i2+js2,i3+js3)*(sz(i1+js1,i2+js2,i3+js3)*tx(i1+js1,i2+
     & js2,i3+js3)-sx(i1+js1,i2+js2,i3+js3)*tz(i1+js1,i2+js2,i3+js3))+
     & rz(i1+js1,i2+js2,i3+js3)*(sx(i1+js1,i2+js2,i3+js3)*ty(i1+js1,
     & i2+js2,i3+js3)-sy(i1+js1,i2+js2,i3+js3)*tx(i1+js1,i2+js2,i3+
     & js3))))-(rsxy(i1-js1,i2-js2,i3-js3,axisp1,0)/(rx(i1-js1,i2-js2,
     & i3-js3)*(sy(i1-js1,i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3)-sz(
     & i1-js1,i2-js2,i3-js3)*ty(i1-js1,i2-js2,i3-js3))+ry(i1-js1,i2-
     & js2,i3-js3)*(sz(i1-js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3)-
     & sx(i1-js1,i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3))+rz(i1-js1,
     & i2-js2,i3-js3)*(sx(i1-js1,i2-js2,i3-js3)*ty(i1-js1,i2-js2,i3-
     & js3)-sy(i1-js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3)))))/(2.*
     & dsa)
                a22s = ((rsxy(i1+js1,i2+js2,i3+js3,axisp1,1)/(rx(i1+
     & js1,i2+js2,i3+js3)*(sy(i1+js1,i2+js2,i3+js3)*tz(i1+js1,i2+js2,
     & i3+js3)-sz(i1+js1,i2+js2,i3+js3)*ty(i1+js1,i2+js2,i3+js3))+ry(
     & i1+js1,i2+js2,i3+js3)*(sz(i1+js1,i2+js2,i3+js3)*tx(i1+js1,i2+
     & js2,i3+js3)-sx(i1+js1,i2+js2,i3+js3)*tz(i1+js1,i2+js2,i3+js3))+
     & rz(i1+js1,i2+js2,i3+js3)*(sx(i1+js1,i2+js2,i3+js3)*ty(i1+js1,
     & i2+js2,i3+js3)-sy(i1+js1,i2+js2,i3+js3)*tx(i1+js1,i2+js2,i3+
     & js3))))-(rsxy(i1-js1,i2-js2,i3-js3,axisp1,1)/(rx(i1-js1,i2-js2,
     & i3-js3)*(sy(i1-js1,i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3)-sz(
     & i1-js1,i2-js2,i3-js3)*ty(i1-js1,i2-js2,i3-js3))+ry(i1-js1,i2-
     & js2,i3-js3)*(sz(i1-js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3)-
     & sx(i1-js1,i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3))+rz(i1-js1,
     & i2-js2,i3-js3)*(sx(i1-js1,i2-js2,i3-js3)*ty(i1-js1,i2-js2,i3-
     & js3)-sy(i1-js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3)))))/(2.*
     & dsa)
                a23s = ((rsxy(i1+js1,i2+js2,i3+js3,axisp1,2)/(rx(i1+
     & js1,i2+js2,i3+js3)*(sy(i1+js1,i2+js2,i3+js3)*tz(i1+js1,i2+js2,
     & i3+js3)-sz(i1+js1,i2+js2,i3+js3)*ty(i1+js1,i2+js2,i3+js3))+ry(
     & i1+js1,i2+js2,i3+js3)*(sz(i1+js1,i2+js2,i3+js3)*tx(i1+js1,i2+
     & js2,i3+js3)-sx(i1+js1,i2+js2,i3+js3)*tz(i1+js1,i2+js2,i3+js3))+
     & rz(i1+js1,i2+js2,i3+js3)*(sx(i1+js1,i2+js2,i3+js3)*ty(i1+js1,
     & i2+js2,i3+js3)-sy(i1+js1,i2+js2,i3+js3)*tx(i1+js1,i2+js2,i3+
     & js3))))-(rsxy(i1-js1,i2-js2,i3-js3,axisp1,2)/(rx(i1-js1,i2-js2,
     & i3-js3)*(sy(i1-js1,i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3)-sz(
     & i1-js1,i2-js2,i3-js3)*ty(i1-js1,i2-js2,i3-js3))+ry(i1-js1,i2-
     & js2,i3-js3)*(sz(i1-js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3)-
     & sx(i1-js1,i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3))+rz(i1-js1,
     & i2-js2,i3-js3)*(sx(i1-js1,i2-js2,i3-js3)*ty(i1-js1,i2-js2,i3-
     & js3)-sy(i1-js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3)))))/(2.*
     & dsa)
               else if( (i1+js1).ge.md1a .and. (i1+js1).le.md1b .and. (
     & i2+js2).ge.md2a .and. (i2+js2).le.md2b .and. (i3+js3).ge.md3a 
     & .and. (i3+js3).le.md3b )then
                a21s = ((rsxy(i1+js1,i2+js2,i3+js3,axisp1,0)/(rx(i1+
     & js1,i2+js2,i3+js3)*(sy(i1+js1,i2+js2,i3+js3)*tz(i1+js1,i2+js2,
     & i3+js3)-sz(i1+js1,i2+js2,i3+js3)*ty(i1+js1,i2+js2,i3+js3))+ry(
     & i1+js1,i2+js2,i3+js3)*(sz(i1+js1,i2+js2,i3+js3)*tx(i1+js1,i2+
     & js2,i3+js3)-sx(i1+js1,i2+js2,i3+js3)*tz(i1+js1,i2+js2,i3+js3))+
     & rz(i1+js1,i2+js2,i3+js3)*(sx(i1+js1,i2+js2,i3+js3)*ty(i1+js1,
     & i2+js2,i3+js3)-sy(i1+js1,i2+js2,i3+js3)*tx(i1+js1,i2+js2,i3+
     & js3))))-(rsxy(i1,i2,i3,axisp1,0)/(rx(i1,i2,i3)*(sy(i1,i2,i3)*
     & tz(i1,i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(sz(i1,i2,
     & i3)*tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,i3)*(sx(
     & i1,i2,i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3)))))/(dsa)
                a22s = ((rsxy(i1+js1,i2+js2,i3+js3,axisp1,1)/(rx(i1+
     & js1,i2+js2,i3+js3)*(sy(i1+js1,i2+js2,i3+js3)*tz(i1+js1,i2+js2,
     & i3+js3)-sz(i1+js1,i2+js2,i3+js3)*ty(i1+js1,i2+js2,i3+js3))+ry(
     & i1+js1,i2+js2,i3+js3)*(sz(i1+js1,i2+js2,i3+js3)*tx(i1+js1,i2+
     & js2,i3+js3)-sx(i1+js1,i2+js2,i3+js3)*tz(i1+js1,i2+js2,i3+js3))+
     & rz(i1+js1,i2+js2,i3+js3)*(sx(i1+js1,i2+js2,i3+js3)*ty(i1+js1,
     & i2+js2,i3+js3)-sy(i1+js1,i2+js2,i3+js3)*tx(i1+js1,i2+js2,i3+
     & js3))))-(rsxy(i1,i2,i3,axisp1,1)/(rx(i1,i2,i3)*(sy(i1,i2,i3)*
     & tz(i1,i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(sz(i1,i2,
     & i3)*tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,i3)*(sx(
     & i1,i2,i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3)))))/(dsa)
                a23s = ((rsxy(i1+js1,i2+js2,i3+js3,axisp1,2)/(rx(i1+
     & js1,i2+js2,i3+js3)*(sy(i1+js1,i2+js2,i3+js3)*tz(i1+js1,i2+js2,
     & i3+js3)-sz(i1+js1,i2+js2,i3+js3)*ty(i1+js1,i2+js2,i3+js3))+ry(
     & i1+js1,i2+js2,i3+js3)*(sz(i1+js1,i2+js2,i3+js3)*tx(i1+js1,i2+
     & js2,i3+js3)-sx(i1+js1,i2+js2,i3+js3)*tz(i1+js1,i2+js2,i3+js3))+
     & rz(i1+js1,i2+js2,i3+js3)*(sx(i1+js1,i2+js2,i3+js3)*ty(i1+js1,
     & i2+js2,i3+js3)-sy(i1+js1,i2+js2,i3+js3)*tx(i1+js1,i2+js2,i3+
     & js3))))-(rsxy(i1,i2,i3,axisp1,2)/(rx(i1,i2,i3)*(sy(i1,i2,i3)*
     & tz(i1,i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(sz(i1,i2,
     & i3)*tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,i3)*(sx(
     & i1,i2,i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3)))))/(dsa)
               else if( (i1-js1).ge.md1a .and. (i1-js1).le.md1b .and. (
     & i2-js2).ge.md2a .and. (i2-js2).le.md2b .and. (i3-js3).ge.md3a 
     & .and. (i3-js3).le.md3b )then
                a21s = ((rsxy(i1,i2,i3,axisp1,0)/(rx(i1,i2,i3)*(sy(i1,
     & i2,i3)*tz(i1,i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(
     & sz(i1,i2,i3)*tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,
     & i3)*(sx(i1,i2,i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3))))-(
     & rsxy(i1-js1,i2-js2,i3-js3,axisp1,0)/(rx(i1-js1,i2-js2,i3-js3)*(
     & sy(i1-js1,i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3)-sz(i1-js1,i2-
     & js2,i3-js3)*ty(i1-js1,i2-js2,i3-js3))+ry(i1-js1,i2-js2,i3-js3)*
     & (sz(i1-js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3)-sx(i1-js1,
     & i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3))+rz(i1-js1,i2-js2,i3-
     & js3)*(sx(i1-js1,i2-js2,i3-js3)*ty(i1-js1,i2-js2,i3-js3)-sy(i1-
     & js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3)))))/(dsa)
                a22s = ((rsxy(i1,i2,i3,axisp1,1)/(rx(i1,i2,i3)*(sy(i1,
     & i2,i3)*tz(i1,i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(
     & sz(i1,i2,i3)*tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,
     & i3)*(sx(i1,i2,i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3))))-(
     & rsxy(i1-js1,i2-js2,i3-js3,axisp1,1)/(rx(i1-js1,i2-js2,i3-js3)*(
     & sy(i1-js1,i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3)-sz(i1-js1,i2-
     & js2,i3-js3)*ty(i1-js1,i2-js2,i3-js3))+ry(i1-js1,i2-js2,i3-js3)*
     & (sz(i1-js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3)-sx(i1-js1,
     & i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3))+rz(i1-js1,i2-js2,i3-
     & js3)*(sx(i1-js1,i2-js2,i3-js3)*ty(i1-js1,i2-js2,i3-js3)-sy(i1-
     & js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3)))))/(dsa)
                a23s = ((rsxy(i1,i2,i3,axisp1,2)/(rx(i1,i2,i3)*(sy(i1,
     & i2,i3)*tz(i1,i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(
     & sz(i1,i2,i3)*tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,
     & i3)*(sx(i1,i2,i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3))))-(
     & rsxy(i1-js1,i2-js2,i3-js3,axisp1,2)/(rx(i1-js1,i2-js2,i3-js3)*(
     & sy(i1-js1,i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3)-sz(i1-js1,i2-
     & js2,i3-js3)*ty(i1-js1,i2-js2,i3-js3))+ry(i1-js1,i2-js2,i3-js3)*
     & (sz(i1-js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3)-sx(i1-js1,
     & i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3))+rz(i1-js1,i2-js2,i3-
     & js3)*(sx(i1-js1,i2-js2,i3-js3)*ty(i1-js1,i2-js2,i3-js3)-sy(i1-
     & js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3)))))/(dsa)
               else
                 stop 82750
               end if
               if( (i1-ks1a).ge.md1a .and. (i1+ks1a).le.md1b .and. (i2-
     & ks2a).ge.md2a .and. (i2+ks2a).le.md2b .and. (i3-ks3a).ge.md3a 
     & .and. (i3+ks3a).le.md3b )then
                a31t = ((rsxy(i1+ks1,i2+ks2,i3+ks3,axisp2,0)/(rx(i1+
     & ks1,i2+ks2,i3+ks3)*(sy(i1+ks1,i2+ks2,i3+ks3)*tz(i1+ks1,i2+ks2,
     & i3+ks3)-sz(i1+ks1,i2+ks2,i3+ks3)*ty(i1+ks1,i2+ks2,i3+ks3))+ry(
     & i1+ks1,i2+ks2,i3+ks3)*(sz(i1+ks1,i2+ks2,i3+ks3)*tx(i1+ks1,i2+
     & ks2,i3+ks3)-sx(i1+ks1,i2+ks2,i3+ks3)*tz(i1+ks1,i2+ks2,i3+ks3))+
     & rz(i1+ks1,i2+ks2,i3+ks3)*(sx(i1+ks1,i2+ks2,i3+ks3)*ty(i1+ks1,
     & i2+ks2,i3+ks3)-sy(i1+ks1,i2+ks2,i3+ks3)*tx(i1+ks1,i2+ks2,i3+
     & ks3))))-(rsxy(i1-ks1,i2-ks2,i3-ks3,axisp2,0)/(rx(i1-ks1,i2-ks2,
     & i3-ks3)*(sy(i1-ks1,i2-ks2,i3-ks3)*tz(i1-ks1,i2-ks2,i3-ks3)-sz(
     & i1-ks1,i2-ks2,i3-ks3)*ty(i1-ks1,i2-ks2,i3-ks3))+ry(i1-ks1,i2-
     & ks2,i3-ks3)*(sz(i1-ks1,i2-ks2,i3-ks3)*tx(i1-ks1,i2-ks2,i3-ks3)-
     & sx(i1-ks1,i2-ks2,i3-ks3)*tz(i1-ks1,i2-ks2,i3-ks3))+rz(i1-ks1,
     & i2-ks2,i3-ks3)*(sx(i1-ks1,i2-ks2,i3-ks3)*ty(i1-ks1,i2-ks2,i3-
     & ks3)-sy(i1-ks1,i2-ks2,i3-ks3)*tx(i1-ks1,i2-ks2,i3-ks3)))))/(2.*
     & dta)
                a32t = ((rsxy(i1+ks1,i2+ks2,i3+ks3,axisp2,1)/(rx(i1+
     & ks1,i2+ks2,i3+ks3)*(sy(i1+ks1,i2+ks2,i3+ks3)*tz(i1+ks1,i2+ks2,
     & i3+ks3)-sz(i1+ks1,i2+ks2,i3+ks3)*ty(i1+ks1,i2+ks2,i3+ks3))+ry(
     & i1+ks1,i2+ks2,i3+ks3)*(sz(i1+ks1,i2+ks2,i3+ks3)*tx(i1+ks1,i2+
     & ks2,i3+ks3)-sx(i1+ks1,i2+ks2,i3+ks3)*tz(i1+ks1,i2+ks2,i3+ks3))+
     & rz(i1+ks1,i2+ks2,i3+ks3)*(sx(i1+ks1,i2+ks2,i3+ks3)*ty(i1+ks1,
     & i2+ks2,i3+ks3)-sy(i1+ks1,i2+ks2,i3+ks3)*tx(i1+ks1,i2+ks2,i3+
     & ks3))))-(rsxy(i1-ks1,i2-ks2,i3-ks3,axisp2,1)/(rx(i1-ks1,i2-ks2,
     & i3-ks3)*(sy(i1-ks1,i2-ks2,i3-ks3)*tz(i1-ks1,i2-ks2,i3-ks3)-sz(
     & i1-ks1,i2-ks2,i3-ks3)*ty(i1-ks1,i2-ks2,i3-ks3))+ry(i1-ks1,i2-
     & ks2,i3-ks3)*(sz(i1-ks1,i2-ks2,i3-ks3)*tx(i1-ks1,i2-ks2,i3-ks3)-
     & sx(i1-ks1,i2-ks2,i3-ks3)*tz(i1-ks1,i2-ks2,i3-ks3))+rz(i1-ks1,
     & i2-ks2,i3-ks3)*(sx(i1-ks1,i2-ks2,i3-ks3)*ty(i1-ks1,i2-ks2,i3-
     & ks3)-sy(i1-ks1,i2-ks2,i3-ks3)*tx(i1-ks1,i2-ks2,i3-ks3)))))/(2.*
     & dta)
                a33t = ((rsxy(i1+ks1,i2+ks2,i3+ks3,axisp2,2)/(rx(i1+
     & ks1,i2+ks2,i3+ks3)*(sy(i1+ks1,i2+ks2,i3+ks3)*tz(i1+ks1,i2+ks2,
     & i3+ks3)-sz(i1+ks1,i2+ks2,i3+ks3)*ty(i1+ks1,i2+ks2,i3+ks3))+ry(
     & i1+ks1,i2+ks2,i3+ks3)*(sz(i1+ks1,i2+ks2,i3+ks3)*tx(i1+ks1,i2+
     & ks2,i3+ks3)-sx(i1+ks1,i2+ks2,i3+ks3)*tz(i1+ks1,i2+ks2,i3+ks3))+
     & rz(i1+ks1,i2+ks2,i3+ks3)*(sx(i1+ks1,i2+ks2,i3+ks3)*ty(i1+ks1,
     & i2+ks2,i3+ks3)-sy(i1+ks1,i2+ks2,i3+ks3)*tx(i1+ks1,i2+ks2,i3+
     & ks3))))-(rsxy(i1-ks1,i2-ks2,i3-ks3,axisp2,2)/(rx(i1-ks1,i2-ks2,
     & i3-ks3)*(sy(i1-ks1,i2-ks2,i3-ks3)*tz(i1-ks1,i2-ks2,i3-ks3)-sz(
     & i1-ks1,i2-ks2,i3-ks3)*ty(i1-ks1,i2-ks2,i3-ks3))+ry(i1-ks1,i2-
     & ks2,i3-ks3)*(sz(i1-ks1,i2-ks2,i3-ks3)*tx(i1-ks1,i2-ks2,i3-ks3)-
     & sx(i1-ks1,i2-ks2,i3-ks3)*tz(i1-ks1,i2-ks2,i3-ks3))+rz(i1-ks1,
     & i2-ks2,i3-ks3)*(sx(i1-ks1,i2-ks2,i3-ks3)*ty(i1-ks1,i2-ks2,i3-
     & ks3)-sy(i1-ks1,i2-ks2,i3-ks3)*tx(i1-ks1,i2-ks2,i3-ks3)))))/(2.*
     & dta)
               else if( (i1+ks1).ge.md1a .and. (i1+ks1).le.md1b .and. (
     & i2+ks2).ge.md2a .and. (i2+ks2).le.md2b .and. (i3+ks3).ge.md3a 
     & .and. (i3+ks3).le.md3b )then
                a31t = ((rsxy(i1+ks1,i2+ks2,i3+ks3,axisp2,0)/(rx(i1+
     & ks1,i2+ks2,i3+ks3)*(sy(i1+ks1,i2+ks2,i3+ks3)*tz(i1+ks1,i2+ks2,
     & i3+ks3)-sz(i1+ks1,i2+ks2,i3+ks3)*ty(i1+ks1,i2+ks2,i3+ks3))+ry(
     & i1+ks1,i2+ks2,i3+ks3)*(sz(i1+ks1,i2+ks2,i3+ks3)*tx(i1+ks1,i2+
     & ks2,i3+ks3)-sx(i1+ks1,i2+ks2,i3+ks3)*tz(i1+ks1,i2+ks2,i3+ks3))+
     & rz(i1+ks1,i2+ks2,i3+ks3)*(sx(i1+ks1,i2+ks2,i3+ks3)*ty(i1+ks1,
     & i2+ks2,i3+ks3)-sy(i1+ks1,i2+ks2,i3+ks3)*tx(i1+ks1,i2+ks2,i3+
     & ks3))))-(rsxy(i1,i2,i3,axisp2,0)/(rx(i1,i2,i3)*(sy(i1,i2,i3)*
     & tz(i1,i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(sz(i1,i2,
     & i3)*tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,i3)*(sx(
     & i1,i2,i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3)))))/(dta)
                a32t = ((rsxy(i1+ks1,i2+ks2,i3+ks3,axisp2,1)/(rx(i1+
     & ks1,i2+ks2,i3+ks3)*(sy(i1+ks1,i2+ks2,i3+ks3)*tz(i1+ks1,i2+ks2,
     & i3+ks3)-sz(i1+ks1,i2+ks2,i3+ks3)*ty(i1+ks1,i2+ks2,i3+ks3))+ry(
     & i1+ks1,i2+ks2,i3+ks3)*(sz(i1+ks1,i2+ks2,i3+ks3)*tx(i1+ks1,i2+
     & ks2,i3+ks3)-sx(i1+ks1,i2+ks2,i3+ks3)*tz(i1+ks1,i2+ks2,i3+ks3))+
     & rz(i1+ks1,i2+ks2,i3+ks3)*(sx(i1+ks1,i2+ks2,i3+ks3)*ty(i1+ks1,
     & i2+ks2,i3+ks3)-sy(i1+ks1,i2+ks2,i3+ks3)*tx(i1+ks1,i2+ks2,i3+
     & ks3))))-(rsxy(i1,i2,i3,axisp2,1)/(rx(i1,i2,i3)*(sy(i1,i2,i3)*
     & tz(i1,i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(sz(i1,i2,
     & i3)*tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,i3)*(sx(
     & i1,i2,i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3)))))/(dta)
                a33t = ((rsxy(i1+ks1,i2+ks2,i3+ks3,axisp2,2)/(rx(i1+
     & ks1,i2+ks2,i3+ks3)*(sy(i1+ks1,i2+ks2,i3+ks3)*tz(i1+ks1,i2+ks2,
     & i3+ks3)-sz(i1+ks1,i2+ks2,i3+ks3)*ty(i1+ks1,i2+ks2,i3+ks3))+ry(
     & i1+ks1,i2+ks2,i3+ks3)*(sz(i1+ks1,i2+ks2,i3+ks3)*tx(i1+ks1,i2+
     & ks2,i3+ks3)-sx(i1+ks1,i2+ks2,i3+ks3)*tz(i1+ks1,i2+ks2,i3+ks3))+
     & rz(i1+ks1,i2+ks2,i3+ks3)*(sx(i1+ks1,i2+ks2,i3+ks3)*ty(i1+ks1,
     & i2+ks2,i3+ks3)-sy(i1+ks1,i2+ks2,i3+ks3)*tx(i1+ks1,i2+ks2,i3+
     & ks3))))-(rsxy(i1,i2,i3,axisp2,2)/(rx(i1,i2,i3)*(sy(i1,i2,i3)*
     & tz(i1,i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(sz(i1,i2,
     & i3)*tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,i3)*(sx(
     & i1,i2,i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3)))))/(dta)
               else if( (i1-ks1).ge.md1a .and. (i1-ks1).le.md1b .and. (
     & i2-ks2).ge.md2a .and. (i2-ks2).le.md2b .and. (i3-ks3).ge.md3a 
     & .and. (i3-ks3).le.md3b )then
                a31t = ((rsxy(i1,i2,i3,axisp2,0)/(rx(i1,i2,i3)*(sy(i1,
     & i2,i3)*tz(i1,i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(
     & sz(i1,i2,i3)*tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,
     & i3)*(sx(i1,i2,i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3))))-(
     & rsxy(i1-ks1,i2-ks2,i3-ks3,axisp2,0)/(rx(i1-ks1,i2-ks2,i3-ks3)*(
     & sy(i1-ks1,i2-ks2,i3-ks3)*tz(i1-ks1,i2-ks2,i3-ks3)-sz(i1-ks1,i2-
     & ks2,i3-ks3)*ty(i1-ks1,i2-ks2,i3-ks3))+ry(i1-ks1,i2-ks2,i3-ks3)*
     & (sz(i1-ks1,i2-ks2,i3-ks3)*tx(i1-ks1,i2-ks2,i3-ks3)-sx(i1-ks1,
     & i2-ks2,i3-ks3)*tz(i1-ks1,i2-ks2,i3-ks3))+rz(i1-ks1,i2-ks2,i3-
     & ks3)*(sx(i1-ks1,i2-ks2,i3-ks3)*ty(i1-ks1,i2-ks2,i3-ks3)-sy(i1-
     & ks1,i2-ks2,i3-ks3)*tx(i1-ks1,i2-ks2,i3-ks3)))))/(dta)
                a32t = ((rsxy(i1,i2,i3,axisp2,1)/(rx(i1,i2,i3)*(sy(i1,
     & i2,i3)*tz(i1,i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(
     & sz(i1,i2,i3)*tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,
     & i3)*(sx(i1,i2,i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3))))-(
     & rsxy(i1-ks1,i2-ks2,i3-ks3,axisp2,1)/(rx(i1-ks1,i2-ks2,i3-ks3)*(
     & sy(i1-ks1,i2-ks2,i3-ks3)*tz(i1-ks1,i2-ks2,i3-ks3)-sz(i1-ks1,i2-
     & ks2,i3-ks3)*ty(i1-ks1,i2-ks2,i3-ks3))+ry(i1-ks1,i2-ks2,i3-ks3)*
     & (sz(i1-ks1,i2-ks2,i3-ks3)*tx(i1-ks1,i2-ks2,i3-ks3)-sx(i1-ks1,
     & i2-ks2,i3-ks3)*tz(i1-ks1,i2-ks2,i3-ks3))+rz(i1-ks1,i2-ks2,i3-
     & ks3)*(sx(i1-ks1,i2-ks2,i3-ks3)*ty(i1-ks1,i2-ks2,i3-ks3)-sy(i1-
     & ks1,i2-ks2,i3-ks3)*tx(i1-ks1,i2-ks2,i3-ks3)))))/(dta)
                a33t = ((rsxy(i1,i2,i3,axisp2,2)/(rx(i1,i2,i3)*(sy(i1,
     & i2,i3)*tz(i1,i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(
     & sz(i1,i2,i3)*tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,
     & i3)*(sx(i1,i2,i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3))))-(
     & rsxy(i1-ks1,i2-ks2,i3-ks3,axisp2,2)/(rx(i1-ks1,i2-ks2,i3-ks3)*(
     & sy(i1-ks1,i2-ks2,i3-ks3)*tz(i1-ks1,i2-ks2,i3-ks3)-sz(i1-ks1,i2-
     & ks2,i3-ks3)*ty(i1-ks1,i2-ks2,i3-ks3))+ry(i1-ks1,i2-ks2,i3-ks3)*
     & (sz(i1-ks1,i2-ks2,i3-ks3)*tx(i1-ks1,i2-ks2,i3-ks3)-sx(i1-ks1,
     & i2-ks2,i3-ks3)*tz(i1-ks1,i2-ks2,i3-ks3))+rz(i1-ks1,i2-ks2,i3-
     & ks3)*(sx(i1-ks1,i2-ks2,i3-ks3)*ty(i1-ks1,i2-ks2,i3-ks3)-sy(i1-
     & ks1,i2-ks2,i3-ks3)*tx(i1-ks1,i2-ks2,i3-ks3)))))/(dta)
               else
                 stop 8250
               end if
              end if
              if( forcingOption.eq.planeWaveBoundaryForcing )then
                ! In the plane wave forcing case we subtract out a plane wave incident field
                !   --->    tau.utt = -tau.uI.tt
                ! *** set RHS for (a1.u).r =  - Ds( a2.uv ) -Dt( a3.uv )
                Da1DotU = -(  a21s*uex+a22s*uey+a23s*uez + a21*us+a22*
     & vs+a23*ws + a31t*uex+a32t*uey+a33t*uez + a31*ut+a32*vt+a33*wt )
                 x00=xy(i1,i2,i3,0)
                 y00=xy(i1,i2,i3,1)
                 z00=xy(i1,i2,i3,2)
                 if( fieldOption.eq.0 )then
                   udd=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x00)+ky*(
     & y00)+kz*(z00)-cc*(t)))*pwc(0))+2.*ssft*(-twoPi*cc)*cos(twoPi*(
     & kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0)+ssftt*sin(twoPi*(kx*
     & (x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))
                   vdd=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x00)+ky*(
     & y00)+kz*(z00)-cc*(t)))*pwc(1))+2.*ssft*(-twoPi*cc)*cos(twoPi*(
     & kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1)+ssftt*sin(twoPi*(kx*
     & (x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))
                   wdd=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x00)+ky*(
     & y00)+kz*(z00)-cc*(t)))*pwc(2))+2.*ssft*(-twoPi*cc)*cos(twoPi*(
     & kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2)+ssftt*sin(twoPi*(kx*
     & (x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))
                 else
                   ! get time derivative (sosup) 
                   udd=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x00)+ky*(
     & y00)+kz*(z00)-cc*(t)))*pwc(0))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))+3.*ssftt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0)
     & +ssfttt*sin(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))
                   vdd=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x00)+ky*(
     & y00)+kz*(z00)-cc*(t)))*pwc(1))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))+3.*ssftt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1)
     & +ssfttt*sin(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))
                   wdd=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x00)+ky*(
     & y00)+kz*(z00)-cc*(t)))*pwc(2))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))+3.*ssftt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2)
     & +ssfttt*sin(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))
                 end if
                tau1DotUtt = tau11*udd+tau12*vdd+tau13*wdd
                tau2DotUtt = tau21*udd+tau22*vdd+tau23*wdd
              end if
              if( useForcing.ne.0 )then
                ! For TZ: utt0 = utt - ett + Lap(e)
                 call ogDeriv3(ep, 0,2,0,0, xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),xy(i1,i2,i3,2),t, ex,uxx, ey,vxx, ez,wxx)
                 call ogDeriv3(ep, 0,0,2,0, xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),xy(i1,i2,i3,2),t, ex,uyy, ey,vyy, ez,wyy)
                 call ogDeriv3(ep, 0,0,0,2, xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),xy(i1,i2,i3,2),t, ex,uzz, ey,vzz, ez,wzz)
               utt00=uxx+uyy+uzz
               vtt00=vxx+vyy+vzz
               wtt00=wxx+wyy+wzz
               tau1DotUtt = tau11*utt00+tau12*vtt00+tau13*wtt00
               tau2DotUtt = tau21*utt00+tau22*vtt00+tau23*wtt00
               ! OGF3D(i1,i2,i3,t, uv0(0),uv0(1),uv0(2))
               ! OGF3D(i1-is1,i2-is2,i3-is3,t, uvm(0),uvm(1),uvm(2))
               ! OGF3D(i1+is1,i2+is2,i3+is3,t, uvp(0),uvp(1),uvp(2))
               ! OGF3D(i1-2*is1,i2-2*is2,i3-2*is3,t, uvm2(0),uvm2(1),uvm2(2))
               ! OGF3D(i1+2*is1,i2+2*is2,i3+2*is3,t, uvp2(0),uvp2(1),uvp2(2))
               ! Da1DotU = (a1.uv).r to 4th order
               ! Da1DotU = (8.*( (a11p1*uvp(0) +a12p1*uvp(1))  - (a11m1*uvm(0) +a12m1*uvm(1)) )!             - ( (a11p2*uvp2(0)+a12p2*uvp2(1)) - (a11m2*uvm2(0)+a12m2*uvm2(1)) ) )/(12.*dra)
              ! for now remove the error in the extrapolation ************
              ! gIVf1 = tau11*( uvm2(0)-4.*uvm(0)+6.*uv0(0)-4.*uvp(0)+uvp2(0)) +!         tau12*( uvm2(1)-4.*uvm(1)+6.*uv0(1)-4.*uvp(1)+uvp2(1)) +!         tau13*( uvm2(2)-4.*uvm(2)+6.*uv0(2)-4.*uvp(2)+uvp2(2))
              ! gIVf2 = tau21*( uvm2(0)-4.*uvm(0)+6.*uv0(0)-4.*uvp(0)+uvp2(0)) +!         tau22*( uvm2(1)-4.*uvm(1)+6.*uv0(1)-4.*uvp(1)+uvp2(1)) +!         tau23*( uvm2(2)-4.*uvm(2)+6.*uv0(2)-4.*uvp(2)+uvp2(2))
              ! gIVf1=0.  ! RHS for tau.D+^p(u)=0
              ! gIVf2=0.
               ! **** compute RHS for div(u) equation ****
               ! *** set to - Ds( a2.uv ) -Dt( a3.uv )
               ! do this: Da1DotU = -( a2.us + a2s.u + a3.ut + a3t.u )
               Da1DotU = -( a21*us+a22*vs+a23*ws + a21s*uex + a22s*uey 
     & + a23s*uez +a31*ut+a32*vt+a33*wt + a31t*uex + a32t*uey + a33t*
     & uez )
             !  Da1DotU = -(  !       (a21zp1*u(i1+js1,i2+js2,i3+js3,ex)-a21zm1*u(i1-js1,i2-js2,i3-js3,ex))/(2.*dsa)!      +(a22zp1*u(i1+js1,i2+js2,i3+js3,ey)-a22zm1*u(i1-js1,i2-js2,i3-js3,ey))/(2.*dsa) !      +(a23zp1*u(i1+js1,i2+js2,i3+js3,ez)-a23zm1*u(i1-js1,i2-js2,i3-js3,ez))/(2.*dsa) ) !             -(  !       (a31zp1*u(i1+ks1,i2+ks2,i3+ks3,ex)-a31zm1*u(i1-ks1,i2-ks2,i3-ks3,ex))/(2.*dta) !      +(a32zp1*u(i1+ks1,i2+ks2,i3+ks3,ey)-a32zm1*u(i1-ks1,i2-ks2,i3-ks3,ey))/(2.*dta) !      +(a33zp1*u(i1+ks1,i2+ks2,i3+ks3,ez)-a33zm1*u(i1-ks1,i2-ks2,i3-ks3,ez))/(2.*dta) )
              end if
             ! Now assign E at the ghost points:


! ************ Results from bc43d.maple *******************


! ************ solution using extrapolation for a1.u *******************
      gIII1=-tau11*(c22*uss+c2*us+c33*utt+c3*ut)-tau12*(c22*vss+c2*vs+
     & c33*vtt+c3*vt)-tau13*(c22*wss+c2*ws+c33*wtt+c3*wt)

      gIII2=-tau21*(c22*uss+c2*us+c33*utt+c3*ut)-tau22*(c22*vss+c2*vs+
     & c33*vtt+c3*vt)-tau23*(c22*wss+c2*ws+c33*wtt+c3*wt)

      tau1U=tau11*u(i1,i2,i3,ex)+tau12*u(i1,i2,i3,ey)+tau13*u(i1,i2,i3,
     & ez)

      tau1Up1=tau11*u(i1+is1,i2+is2,i3+is3,ex)+tau12*u(i1+is1,i2+is2,
     & i3+is3,ey)+tau13*u(i1+is1,i2+is2,i3+is3,ez)

      tau1Up2=tau11*u(i1+2*is1,i2+2*is2,i3+2*is3,ex)+tau12*u(i1+2*is1,
     & i2+2*is2,i3+2*is3,ey)+tau13*u(i1+2*is1,i2+2*is2,i3+2*is3,ez)

      tau1Up3=tau11*u(i1+3*is1,i2+3*is2,i3+3*is3,ex)+tau12*u(i1+3*is1,
     & i2+3*is2,i3+3*is3,ey)+tau13*u(i1+3*is1,i2+3*is2,i3+3*is3,ez)

      tau2U=tau21*u(i1,i2,i3,ex)+tau22*u(i1,i2,i3,ey)+tau23*u(i1,i2,i3,
     & ez)

      tau2Up1=tau21*u(i1+is1,i2+is2,i3+is3,ex)+tau22*u(i1+is1,i2+is2,
     & i3+is3,ey)+tau23*u(i1+is1,i2+is2,i3+is3,ez)

      tau2Up2=tau21*u(i1+2*is1,i2+2*is2,i3+2*is3,ex)+tau22*u(i1+2*is1,
     & i2+2*is2,i3+2*is3,ey)+tau23*u(i1+2*is1,i2+2*is2,i3+2*is3,ez)

      tau2Up3=tau21*u(i1+3*is1,i2+3*is2,i3+3*is3,ex)+tau22*u(i1+3*is1,
     & i2+3*is2,i3+3*is3,ey)+tau23*u(i1+3*is1,i2+3*is2,i3+3*is3,ez)

! tau1.D+^p u = 0
      gIV1=-10*tau1U+10*tau1Up1-5*tau1Up2+tau1Up3 +gIVf1

! tau2.D+^p u = 0
      gIV2=-10*tau2U+10*tau2Up1-5*tau2Up2+tau2Up3 +gIVf2


! ttu11 = tau1.u(-1), ttu12 = tau1.u(-2)
      ttu11=-(-12*c11*tau1Up1+24*c11*tau1U+c11*ctlrr*tau1Up2-4*c11*
     & ctlrr*tau1Up1+6*c11*ctlrr*tau1U+c11*ctlrr*gIV1-6*c1*dra*
     & tau1Up1+c1*dra*ctlr*tau1Up2-2*c1*dra*ctlr*tau1Up1-c1*dra*ctlr*
     & gIV1+12*gIII1*dra**2+12*tau1DotUtt*dra**2)/(-12*c11+c11*ctlrr+
     & 6*c1*dra-3*c1*dra*ctlr)
      ttu12=-(-60*c11*tau1Up1+120*c11*tau1U+5*c11*ctlrr*tau1Up2-20*c11*
     & ctlrr*tau1Up1+30*c11*ctlrr*tau1U+4*c11*ctlrr*gIV1-30*c1*dra*
     & tau1Up1+5*c1*dra*ctlr*tau1Up2-10*c1*dra*ctlr*tau1Up1-2*c1*dra*
     & ctlr*gIV1+60*gIII1*dra**2+60*tau1DotUtt*dra**2+12*c11*gIV1-6*
     & gIV1*c1*dra)/(-12*c11+c11*ctlrr+6*c1*dra-3*c1*dra*ctlr)

! ttu21 = tau2.u(-1), ttu22 = tau2.u(-2)
      ttu21=-(-12*c11*tau2Up1+24*c11*tau2U+c11*ctlrr*tau2Up2-4*c11*
     & ctlrr*tau2Up1+6*c11*ctlrr*tau2U+c11*ctlrr*gIV2-6*c1*dra*
     & tau2Up1+c1*dra*ctlr*tau2Up2-2*c1*dra*ctlr*tau2Up1-c1*dra*ctlr*
     & gIV2+12*gIII2*dra**2+12*tau2DotUtt*dra**2)/(-12*c11+c11*ctlrr+
     & 6*c1*dra-3*c1*dra*ctlr)
      ttu22=-(-60*c11*tau2Up1+120*c11*tau2U+5*c11*ctlrr*tau2Up2-20*c11*
     & ctlrr*tau2Up1+30*c11*ctlrr*tau2U+4*c11*ctlrr*gIV2-30*c1*dra*
     & tau2Up1+5*c1*dra*ctlr*tau2Up2-10*c1*dra*ctlr*tau2Up1-2*c1*dra*
     & ctlr*gIV2+60*gIII2*dra**2+60*tau2DotUtt*dra**2+12*c11*gIV2-6*
     & gIV2*c1*dra)/(-12*c11+c11*ctlrr+6*c1*dra-3*c1*dra*ctlr)

      ! *********** set tangential components to be exact *****
      ! OGF3D(i1-is1,i2-is2,i3-is3,t, uvm(0),uvm(1),uvm(2))
      ! ttu11=tau11*uvm(0)+tau12*uvm(1)+tau13*uvm(2)
      ! ttu21=tau21*uvm(0)+tau22*uvm(1)+tau23*uvm(2)
      ! OGF3D(i1-2*is1,i2-2*is2,i3-2*is3,t, uvm(0),uvm(1),uvm(2))
      ! ttu12=tau11*uvm(0)+tau12*uvm(1)+tau13*uvm(2)
      ! ttu22=tau21*uvm(0)+tau22*uvm(1)+tau23*uvm(2)
      ! ******************************************************

      f1f  =a11*(15.*u(i1,i2,i3,ex)-20.*u(i1+is1,i2+is2,i3+is3,ex)+15.*
     & u(i1+2*is1,i2+2*is2,i3+2*is3,ex)-6.*u(i1+3*is1,i2+3*is2,i3+3*
     & is3,ex)+u(i1+4*is1,i2+4*is2,i3+4*is3,ex))+a12*(15.*u(i1,i2,i3,
     & ey)-20.*u(i1+is1,i2+is2,i3+is3,ey)+15.*u(i1+2*is1,i2+2*is2,i3+
     & 2*is3,ey)-6.*u(i1+3*is1,i2+3*is2,i3+3*is3,ey)+u(i1+4*is1,i2+4*
     & is2,i3+4*is3,ey))+a13*(15.*u(i1,i2,i3,ez)-20.*u(i1+is1,i2+is2,
     & i3+is3,ez)+15.*u(i1+2*is1,i2+2*is2,i3+2*is3,ez)-6.*u(i1+3*is1,
     & i2+3*is2,i3+3*is3,ez)+u(i1+4*is1,i2+4*is2,i3+4*is3,ez))

      f2um2=1/12.*a11m2
      f2um1=-2/3.*a11m1
      f2vm2=1/12.*a12m2
      f2vm1=-2/3.*a12m1
      f2wm2=1/12.*a13m2
      f2wm1=-2/3.*a13m1
      f2f  =2/3.*a11p1*u(i1+is1,i2+is2,i3+is3,ex)+2/3.*a12p1*u(i1+is1,
     & i2+is2,i3+is3,ey)+2/3.*a13p1*u(i1+is1,i2+is2,i3+is3,ez)-1/12.*
     & a11p2*u(i1+2*is1,i2+2*is2,i3+2*is3,ex)-1/12.*a12p2*u(i1+2*is1,
     & i2+2*is2,i3+2*is3,ey)-1/12.*a13p2*u(i1+2*is1,i2+2*is2,i3+2*is3,
     & ez)-Da1DotU*dra

      u(i1-is1,i2-is2,i3-is3,ez) = -1.*(tau21*f1f*f2um2*tau23*tau12**2-
     & 1.*tau12*tau22*tau13*tau21*f1f*f2um2+f1f*tau22*f2vm2*tau23*
     & tau11**2-1.*f1f*tau22**2*f2wm2*tau11**2+tau11*tau13*f1f*tau22**
     & 2*f2um2-1.*tau11*f1f*tau23*tau12*f2vm2*tau21-1.*tau11*tau13*
     & f1f*tau22*f2vm2*tau21-1.*tau11*f1f*tau22*f2um2*tau23*tau12+2.*
     & tau11*f1f*tau22*f2wm2*tau12*tau21+tau12*tau13*tau21**2*f1f*
     & f2vm2-1.*tau21**2*f1f*f2wm2*tau12**2+ttu12*f2vm2*tau21**2*a13*
     & tau12-1.*ttu12*f2vm2*tau21*tau23*a11*tau12-1.*ttu12*tau22*
     & f2vm2*tau21*a13*tau11+ttu12*tau22**2*f2um2*a13*tau11-1.*ttu12*
     & tau22*f2um2*a13*tau21*tau12-1.*ttu12*tau22*f2um2*tau23*a12*
     & tau11+f2um1*ttu21*tau21*tau12**2*a13+tau21**2*f2f*tau12**2*a13+
     & f2um2*a13*tau12**2*tau21*ttu22-1.*tau12*tau22*tau21*a13*ttu11*
     & f2um1-1.*tau21*f2f*tau23*tau12**2*a11-1.*tau12*tau21*a13*ttu21*
     & tau11*f2vm1-1.*tau12*tau21*tau23*a11*ttu11*f2vm1-1.*tau12*
     & tau22*ttu22*f2um2*a13*tau11-1.*tau12*tau13*f2um1*ttu21*tau21*
     & a12-1.*tau12*tau13*f2um2*a12*tau21*ttu22+tau12*tau21**2*a13*
     & ttu11*f2vm1+tau12*tau22*tau13*tau21*f2f*a11-1.*tau12*tau13*
     & tau21**2*f2f*a12+tau11*a13*ttu11*tau22**2*f2um1-1.*tau11*tau13*
     & tau22*f2vm2*ttu22*a11-1.*tau11*tau13*a11*tau22**2*f2f+tau11*
     & tau22*f2f*tau23*tau12*a11-1.*tau11*tau13*a11*ttu21*tau22*f2vm1-
     & 1.*tau11*tau21*a13*ttu11*tau22*f2vm1+tau11*tau21*tau13*a12*
     & tau22*f2f-2.*tau11*tau21*a13*tau12*tau22*f2f-1.*tau11*a13*
     & tau12*tau22*f2um1*ttu21-1.*tau11*a12*tau22*tau23*f2um1*ttu11+
     & tau11*tau21*a12*tau23*tau12*f2f+a13*tau22**2*tau11**2*f2f+
     & tau22*f2vm2*ttu22*a13*tau11**2-1.*a12*tau23*tau11**2*tau22*f2f-
     & 1.*tau11*tau21*ttu22*f2vm2*a13*tau12+a13*ttu21*tau11**2*tau22*
     & f2vm1+6.*ttu21*f2wm2*tau12**2*tau21*a11+tau22*tau11*f2wm2*
     & tau12*ttu22*a11+tau21*tau23*ttu11*f2vm1*tau11*a12+tau21*tau13*
     & a11*ttu11*tau22*f2vm1+tau22*f2vm2*tau23*tau11*ttu12*a11-1.*
     & tau21*tau12**2*f2wm2*ttu22*a11+tau21*a11*tau12*tau22*f2wm2*
     & ttu12-1.*ttu21*tau23*f2vm1*tau11**2*a12-6.*ttu21*f2vm2*tau23*
     & tau11**2*a12-6.*ttu21*f2um2*tau23*a11*tau12**2+ttu21*tau23*
     & f2vm1*tau11*tau12*a11-6.*ttu21*tau22*f2wm2*tau11*a11*tau12+6.*
     & ttu21*f2um2*tau23*a12*tau11*tau12+6.*ttu21*f2vm2*tau23*tau11*
     & a11*tau12-6.*tau21*ttu11*tau22*f2wm2*tau11*a12+6.*tau21*tau13*
     & ttu11*tau22*f2um2*a12+tau21*tau13*ttu21*f2vm1*tau11*a12+tau21*
     & a12*tau12*f2wm2*tau11*ttu22+tau22*tau13*f2um2*a12*tau11*ttu22+
     & 6.*tau22*tau13*f2um2*a11*ttu21*tau12+6.*tau22*tau13*f2vm2*
     & tau21*a11*ttu11-1.*tau22*f2wm2*tau11**2*a12*ttu22+tau22*tau13*
     & f2um1*ttu11*tau21*a12+tau22*tau13*f2um1*ttu21*tau12*a11-6.*
     & ttu21*tau12*f2wm2*tau11*a12*tau21-6.*ttu21*tau13*f2vm2*tau21*
     & a11*tau12+6.*tau22*f2um2*tau23*a11*ttu11*tau12-6.*tau22*f2wm2*
     & tau12*tau21*a11*ttu11-6.*tau22*f2vm2*tau23*tau11*a11*ttu11+
     & tau22*tau23*f2um1*ttu11*tau12*a11+tau22*tau21*f2wm2*ttu12*a12*
     & tau11+tau21*tau13*tau12*f2vm2*ttu22*a11-6.*tau21**2*tau13*
     & ttu11*f2vm2*a12+6.*tau21**2*ttu11*f2wm2*tau12*a12-1.*tau21**2*
     & tau13*ttu11*f2vm1*a12-1.*tau21**2*ttu12*f2wm2*tau12*a12+6.*
     & tau21*tau13*ttu21*f2vm2*a12*tau11-6.*ttu21*tau22*tau13*f2um2*
     & a12*tau11-1.*ttu21*tau23*f2um1*tau12**2*a11+ttu21*tau23*f2um1*
     & tau12*a12*tau11+6.*ttu21*tau22*f2wm2*tau11**2*a12-6.*tau22**2*
     & tau13*f2um2*a11*ttu11-1.*tau22**2*tau13*f2um1*ttu11*a11-1.*
     & tau22**2*f2wm2*ttu12*tau11*a11+6.*tau22**2*f2wm2*tau11*a11*
     & ttu11+tau21*tau23*ttu12*f2um2*a12*tau12+6.*tau21*tau23*ttu11*
     & f2vm2*a12*tau11-6.*tau21*tau23*ttu11*f2um2*a12*tau12)/(tau23*
     & tau12*f2vm1*tau21*a13*tau11+6.*tau23*tau12*f2wm2*tau11*a12*
     & tau21+tau23*tau12*f2wm1*tau11*tau21*a12-1.*f2wm1*tau21*tau12**
     & 2*tau23*a11+6.*tau23*tau12*f2vm2*tau21*a13*tau11-1.*tau23*
     & f2um1*tau12**2*tau21*a13+6.*tau13*f2vm2*tau21*tau23*a11*tau12-
     & 2.*tau13*tau23*f2vm1*tau11*tau21*a12-6.*f2wm2*tau12**2*tau21*
     & tau23*a11-6.*tau13*f2vm2*tau21**2*a13*tau12+tau13*f2vm1*tau21*
     & tau23*tau12*a11-1.*tau23**2*f2um1*tau12*a12*tau11+tau13*tau23*
     & f2um1*tau12*tau21*a12-12.*tau13*f2vm2*tau21*tau23*a12*tau11-6.*
     & tau13*f2wm2*tau12*tau21**2*a12-1.*tau22**2*tau13*f2um1*a13*
     & tau11+tau22*f2wm1*tau11*tau23*tau12*a11-6.*tau22*f2wm2*tau11**
     & 2*tau23*a12-6.*tau22*tau13**2*f2um2*a12*tau21-1.*tau22*tau13**
     & 2*f2um1*tau21*a12-6.*tau22*tau13**2*f2vm2*tau21*a11-1.*tau22*
     & tau13**2*f2vm1*tau21*a11-1.*tau22*f2wm1*tau11**2*a12*tau23-1.*
     & tau22*tau23*f2vm1*tau11**2*a13-12.*tau22*tau13*f2um2*tau23*a11*
     & tau12-1.*tau13*f2vm1*tau21**2*tau12*a13-2.*tau22*f2wm1*tau21*
     & tau12*a13*tau11+tau22*tau23*f2um1*tau12*a13*tau11-6.*f2um2*
     & tau23**2*a12*tau11*tau12-6.*f2um2*a13*tau21*tau12**2*tau23-6.*
     & f2vm2*tau23**2*tau11*a11*tau12+6.*tau13*f2um2*a12*tau21*tau23*
     & tau12+6.*tau22*tau13*f2um2*a13*tau21*tau12+6.*tau22*tau13*
     & f2wm2*tau12*tau21*a11+tau13**2*f2vm1*tau21**2*a12+tau22*tau13*
     & f2vm1*tau21*a13*tau11+tau22*tau13*f2wm1*tau11*tau21*a12+6.*
     & tau13**2*f2vm2*tau21**2*a12-6.*tau22**2*tau13*f2um2*a13*tau11+
     & tau22*tau13*f2wm1*tau21*tau12*a11+6.*tau22*tau13*f2vm2*tau21*
     & a13*tau11-2.*tau22*tau13*f2um1*a11*tau23*tau12+tau22*tau13*
     & f2um1*a12*tau11*tau23-1.*tau22**2*tau13*f2wm1*tau11*a11+tau22*
     & tau13*f2um1*tau21*tau12*a13-1.*tau23**2*f2vm1*tau11*tau12*a11+
     & 6.*tau22*f2wm2*tau11*tau23*a11*tau12-1.*tau13*f2wm1*tau21**2*
     & tau12*a12-6.*tau22*f2vm2*tau23*tau11**2*a13-6.*tau22**2*tau13*
     & f2wm2*tau11*a11+tau23**2*f2vm1*tau11**2*a12+6.*f2vm2*tau23**2*
     & tau11**2*a12+6.*f2um2*tau23**2*a11*tau12**2+6.*tau22*f2um2*a13*
     & tau11*tau23*tau12-12.*tau22*f2wm2*tau12*tau21*a13*tau11+6.*
     & tau22*tau13*f2wm2*tau11*a12*tau21+tau22*tau13*tau23*f2vm1*
     & tau11*a11+6.*tau22*tau13*f2vm2*tau23*tau11*a11+6.*tau22*tau13*
     & f2um2*tau23*a12*tau11+tau22**2*tau13**2*f2um1*a11+6.*f2wm2*
     & tau12**2*tau21**2*a13+tau23**2*f2um1*tau12**2*a11+f2wm1*tau21**
     & 2*tau12**2*a13+6.*tau22**2*f2wm2*tau11**2*a13+tau22**2*f2wm1*
     & tau11**2*a13+6.*tau22**2*tau13**2*f2um2*a11)

      u(i1-2*is1,i2-2*is2,i3-2*is3,ez) = (-1.*tau21**2*f2wm1*tau12**2*
     & f1f-1.*f2wm1*tau11**2*tau22**2*f1f+f2vm1*tau23*tau11**2*f1f*
     & tau22+tau11*tau13*f2um1*tau22**2*f1f-1.*tau11*tau21*f2vm1*f1f*
     & tau23*tau12-1.*tau11*tau21*tau13*f2vm1*f1f*tau22-1.*tau11*
     & f2um1*tau23*tau12*f1f*tau22+tau12*tau13*tau21**2*f2vm1*f1f-1.*
     & tau12*tau22*tau13*tau21*f2um1*f1f+tau21*f2um1*tau23*tau12**2*
     & f1f+2.*tau11*tau21*f2wm1*tau12*f1f*tau22-1.*ttu12*tau22*tau13*
     & f2um1*tau21*a12-6.*ttu12*tau22*tau13*f2vm2*tau21*a11+ttu12*
     & tau23*f2um1*tau12*tau21*a12-6.*ttu12*tau22*tau13*f2um2*a12*
     & tau21-1.*ttu12*tau22*tau13*f2vm1*tau21*a11-6.*ttu12*f2vm2*
     & tau21**2*a13*tau12+6.*ttu12*f2vm2*tau21*tau23*a11*tau12+6.*
     & ttu12*tau22*f2vm2*tau21*a13*tau11+ttu12*tau22*f2wm1*tau11*
     & tau21*a12+6.*ttu12*tau13*f2vm2*tau21**2*a12-6.*ttu12*tau22**2*
     & f2um2*a13*tau11+6.*ttu12*tau22*f2um2*a13*tau21*tau12+ttu12*
     & tau13*f2vm1*tau21**2*a12+ttu12*tau22*tau23*f2vm1*tau11*a11+
     & ttu12*tau22*f2wm1*tau21*tau12*a11+ttu12*tau22**2*tau13*f2um1*
     & a11-1.*ttu12*f2wm1*tau21**2*tau12*a12-1.*ttu12*tau22**2*f2wm1*
     & tau11*a11+6.*ttu12*tau22**2*tau13*f2um2*a11+6.*ttu12*tau22*
     & f2um2*tau23*a12*tau11-6.*f2um1*ttu21*tau21*tau12**2*a13-6.*
     & tau21**2*f2f*tau12**2*a13-6.*f2um2*a13*tau12**2*tau21*ttu22+6.*
     & tau12*tau22*tau21*a13*ttu11*f2um1+6.*tau21*f2f*tau23*tau12**2*
     & a11-1.*tau12*tau22*tau13*ttu22*f2um1*a11-6.*tau12*tau22*tau13*
     & ttu22*f2um2*a11+tau12*tau22*ttu22*f2wm1*tau11*a11-1.*tau12*
     & tau22*tau23*ttu12*f2um1*a11+6.*tau12*tau21*a13*ttu21*tau11*
     & f2vm1-1.*tau12*ttu22*tau23*f2vm1*tau11*a11+6.*tau12*tau21*
     & tau23*a11*ttu11*f2vm1-6.*tau12*tau22*tau23*ttu12*f2um2*a11+6.*
     & tau12*tau22*ttu22*f2um2*a13*tau11-6.*tau12*tau23*f2um1*ttu11*
     & tau21*a12+6.*tau12*tau13*f2um1*ttu21*tau21*a12-6.*tau12*f2wm1*
     & ttu21*tau11*tau21*a12+6.*tau12*tau13*f2um2*a12*tau21*ttu22-6.*
     & tau12*tau13*tau21*a11*ttu21*f2vm1+tau12*tau13*ttu22*f2vm1*
     & tau21*a11-6.*tau12*tau21**2*a13*ttu11*f2vm1+6.*tau12*tau21**2*
     & f2wm1*ttu11*a12-6.*tau12*tau22*tau13*tau21*f2f*a11-6.*tau12*
     & tau22*tau21*f2wm1*ttu11*a11+6.*tau12*tau13*tau21**2*f2f*a12-6.*
     & tau12*f2um2*a12*tau11*tau23*ttu22-6.*tau11*tau23*a11*ttu11*
     & tau22*f2vm1-1.*tau11*a12*ttu22*tau23*f2um1*tau12-6.*tau11*a13*
     & ttu11*tau22**2*f2um1+6.*tau11*tau13*tau22*f2vm2*ttu22*a11+6.*
     & tau11*tau13*a11*tau22**2*f2f-6.*tau11*tau22*f2f*tau23*tau12*
     & a11+6.*tau11*tau13*a11*ttu21*tau22*f2vm1+6.*tau11*tau21*a13*
     & ttu11*tau22*f2vm1-6.*tau11*tau21*tau13*a12*ttu22*f2vm2-6.*
     & tau11*tau21*tau13*a12*tau22*f2f+12.*tau11*tau21*a13*tau12*
     & tau22*f2f+tau11*tau21*a12*ttu22*f2wm1*tau12-1.*tau11*tau21*
     & tau13*a12*ttu22*f2vm1-6.*tau11*tau13*a12*tau22*f2um1*ttu21+6.*
     & tau11*a13*tau12*tau22*f2um1*ttu21+6.*tau11*a12*tau22*tau23*
     & f2um1*ttu11-6.*tau11*a11*ttu21*tau22*f2wm1*tau12+ttu22*f2um1*
     & a11*tau23*tau12**2-1.*ttu22*f2wm1*tau21*tau12**2*a11+6.*tau21*
     & a11*ttu21*f2wm1*tau12**2+6.*ttu22*f2um2*tau23*a11*tau12**2-6.*
     & tau11*tau21*a12*tau23*tau12*f2f-6.*a13*tau22**2*tau11**2*f2f+
     & a12*ttu22*tau23*f2vm1*tau11**2+6.*a12*tau22*f2wm1*ttu21*tau11**
     & 2-6.*tau22*f2vm2*ttu22*a13*tau11**2+6.*a12*ttu22*f2vm2*tau23*
     & tau11**2-1.*a12*ttu22*tau22*f2wm1*tau11**2+6.*a12*tau23*tau11**
     & 2*tau22*f2f+6.*tau11*a11*tau22**2*f2wm1*ttu11+tau11*tau13*a12*
     & ttu22*tau22*f2um1-6.*tau11*tau23*tau12*f2vm2*ttu22*a11-1.*
     & tau11*tau21*a12*tau23*ttu12*f2vm1-6.*tau11*tau21*a12*tau22*
     & f2wm1*ttu11+6.*tau11*tau21*ttu22*f2vm2*a13*tau12-6.*tau11*
     & tau21*a12*tau23*ttu12*f2vm2-6.*a13*ttu21*tau11**2*tau22*f2vm1)
     & /(tau23*tau12*f2vm1*tau21*a13*tau11+6.*tau23*tau12*f2wm2*tau11*
     & a12*tau21+tau23*tau12*f2wm1*tau11*tau21*a12-1.*f2wm1*tau21*
     & tau12**2*tau23*a11+6.*tau23*tau12*f2vm2*tau21*a13*tau11-1.*
     & tau23*f2um1*tau12**2*tau21*a13+6.*tau13*f2vm2*tau21*tau23*a11*
     & tau12-2.*tau13*tau23*f2vm1*tau11*tau21*a12-6.*f2wm2*tau12**2*
     & tau21*tau23*a11-6.*tau13*f2vm2*tau21**2*a13*tau12+tau13*f2vm1*
     & tau21*tau23*tau12*a11-1.*tau23**2*f2um1*tau12*a12*tau11+tau13*
     & tau23*f2um1*tau12*tau21*a12-12.*tau13*f2vm2*tau21*tau23*a12*
     & tau11-6.*tau13*f2wm2*tau12*tau21**2*a12-1.*tau22**2*tau13*
     & f2um1*a13*tau11+tau22*f2wm1*tau11*tau23*tau12*a11-6.*tau22*
     & f2wm2*tau11**2*tau23*a12-6.*tau22*tau13**2*f2um2*a12*tau21-1.*
     & tau22*tau13**2*f2um1*tau21*a12-6.*tau22*tau13**2*f2vm2*tau21*
     & a11-1.*tau22*tau13**2*f2vm1*tau21*a11-1.*tau22*f2wm1*tau11**2*
     & a12*tau23-1.*tau22*tau23*f2vm1*tau11**2*a13-12.*tau22*tau13*
     & f2um2*tau23*a11*tau12-1.*tau13*f2vm1*tau21**2*tau12*a13-2.*
     & tau22*f2wm1*tau21*tau12*a13*tau11+tau22*tau23*f2um1*tau12*a13*
     & tau11-6.*f2um2*tau23**2*a12*tau11*tau12-6.*f2um2*a13*tau21*
     & tau12**2*tau23-6.*f2vm2*tau23**2*tau11*a11*tau12+6.*tau13*
     & f2um2*a12*tau21*tau23*tau12+6.*tau22*tau13*f2um2*a13*tau21*
     & tau12+6.*tau22*tau13*f2wm2*tau12*tau21*a11+tau13**2*f2vm1*
     & tau21**2*a12+tau22*tau13*f2vm1*tau21*a13*tau11+tau22*tau13*
     & f2wm1*tau11*tau21*a12+6.*tau13**2*f2vm2*tau21**2*a12-6.*tau22**
     & 2*tau13*f2um2*a13*tau11+tau22*tau13*f2wm1*tau21*tau12*a11+6.*
     & tau22*tau13*f2vm2*tau21*a13*tau11-2.*tau22*tau13*f2um1*a11*
     & tau23*tau12+tau22*tau13*f2um1*a12*tau11*tau23-1.*tau22**2*
     & tau13*f2wm1*tau11*a11+tau22*tau13*f2um1*tau21*tau12*a13-1.*
     & tau23**2*f2vm1*tau11*tau12*a11+6.*tau22*f2wm2*tau11*tau23*a11*
     & tau12-1.*tau13*f2wm1*tau21**2*tau12*a12-6.*tau22*f2vm2*tau23*
     & tau11**2*a13-6.*tau22**2*tau13*f2wm2*tau11*a11+tau23**2*f2vm1*
     & tau11**2*a12+6.*f2vm2*tau23**2*tau11**2*a12+6.*f2um2*tau23**2*
     & a11*tau12**2+6.*tau22*f2um2*a13*tau11*tau23*tau12-12.*tau22*
     & f2wm2*tau12*tau21*a13*tau11+6.*tau22*tau13*f2wm2*tau11*a12*
     & tau21+tau22*tau13*tau23*f2vm1*tau11*a11+6.*tau22*tau13*f2vm2*
     & tau23*tau11*a11+6.*tau22*tau13*f2um2*tau23*a12*tau11+tau22**2*
     & tau13**2*f2um1*a11+6.*f2wm2*tau12**2*tau21**2*a13+tau23**2*
     & f2um1*tau12**2*a11+f2wm1*tau21**2*tau12**2*a13+6.*tau22**2*
     & f2wm2*tau11**2*a13+tau22**2*f2wm1*tau11**2*a13+6.*tau22**2*
     & tau13**2*f2um2*a11)

      u(i1-is1,i2-is2,i3-is3,ex) = -1.*(-1.*tau21*tau13*a12*tau22*
     & f2wm1*ttu11+tau21*tau13*a12*tau23*tau12*f2f+6.*tau13*tau22**2*
     & f2um2*a13*ttu11+2.*tau13*f1f*tau22*f2um2*tau23*tau12+tau13**2*
     & f1f*tau22*f2vm2*tau21-1.*tau13*tau23*tau12*f2vm2*ttu22*a11-1.*
     & f1f*tau23*tau12*f2wm2*tau11*tau22+f1f*tau23**2*tau12*f2vm2*
     & tau11-1.*f1f*tau23*tau12*tau13*f2vm2*tau21+f1f*tau23*tau12**2*
     & f2wm2*tau21-1.*tau13*f1f*tau22*f2wm2*tau12*tau21+tau13*f1f*
     & tau22**2*f2wm2*tau11-1.*f1f*tau23**2*tau12**2*f2um2-1.*tau13**
     & 2*f1f*tau22**2*f2um2-1.*tau13*f1f*tau22*f2vm2*tau23*tau11+
     & tau23**2*a11*tau12**2*f2f+tau13*a11*tau22**2*f2wm2*ttu12-1.*
     & a12*tau23**2*tau12*f2f*tau11-1.*a12*tau23*tau12*f2wm2*tau11*
     & ttu22-1.*a12*tau23*tau12*f2wm1*ttu21*tau11+tau23**2*a11*ttu11*
     & tau12*f2vm1+tau23**2*tau12*f2vm2*ttu12*a11+tau23*tau12**2*
     & f2wm2*ttu22*a11+a13*tau12*tau23*tau11*tau22*f2f+a11*ttu21*
     & tau23*tau12**2*f2wm1-1.*tau23*a11*tau12*tau22*f2wm1*ttu11-1.*
     & tau23*a11*tau12*tau22*f2wm2*ttu12-1.*tau13*tau22*f2vm2*ttu22*
     & a13*tau11+tau13*a12*tau23*tau11*tau22*f2f-1.*tau13*tau22*f2vm2*
     & tau23*ttu12*a11-1.*tau13*tau22*f2wm2*tau12*ttu22*a11-1.*tau13*
     & tau23*a11*ttu11*tau22*f2vm1-1.*tau13*a11*ttu21*tau23*tau12*
     & f2vm1-1.*tau13*a13*ttu21*tau11*tau22*f2vm1+tau13*a11*tau22**2*
     & f2wm1*ttu11-1.*tau13*a11*ttu21*tau22*f2wm1*tau12-1.*tau13*a13*
     & tau22**2*tau11*f2f+tau13**2*tau22*f2vm2*ttu22*a11+tau13**2*a11*
     & ttu21*tau22*f2vm1+tau13**2*a11*tau22**2*f2f-2.*tau13*tau22*f2f*
     & tau23*tau12*a11+tau22*f2wm1*ttu21*tau11*tau12*a13+tau23*ttu12*
     & tau22*f2um2*a13*tau12+6.*tau22*f2vm2*tau23*tau11*a13*ttu11-6.*
     & tau22*f2um2*a13*ttu11*tau23*tau12+6.*tau22*f2wm2*tau12*a13*
     & ttu21*tau11+tau23*tau11*a13*ttu11*tau22*f2vm1+6.*tau23*ttu11*
     & tau22*f2wm2*tau11*a12-6.*tau13*tau23*ttu11*tau22*f2um2*a12-6.*
     & tau13*ttu21*f2um2*a12*tau23*tau12+tau13*ttu21*tau23*f2vm1*
     & tau11*a12+6.*tau13*ttu21*f2vm2*tau23*a12*tau11-6.*tau13*ttu21*
     & tau22*f2wm2*tau11*a12+tau13*tau23*ttu12*tau22*f2um2*a12-6.*
     & tau13*ttu21*tau22*f2um2*a13*tau12+tau13*ttu22*tau22*f2um2*a13*
     & tau12+tau13*ttu22*f2um2*a12*tau23*tau12-1.*tau13**2*ttu22*
     & tau22*f2um2*a12+tau13*ttu22*tau22*f2wm2*tau11*a12+tau23*tau11*
     & ttu22*f2vm2*a13*tau12-6.*tau23*tau12*f2vm2*a13*ttu21*tau11+
     & tau23*tau11*a12*tau22*f2wm1*ttu11-6.*tau21*tau13**2*ttu21*
     & f2vm2*a12-1.*tau21*ttu21*f2wm1*tau12**2*a13-6.*tau21*ttu21*
     & f2wm2*tau12**2*a13-1.*tau21*tau13**2*ttu21*f2vm1*a12+tau21*
     & tau13*ttu21*f2vm1*tau12*a13+tau21*tau13*tau22*f2vm2*a13*ttu12+
     & 6.*tau21*tau13*tau23*ttu11*f2vm2*a12+6.*tau21*tau13*ttu21*
     & f2vm2*a13*tau12+6.*tau21*tau13*ttu21*f2wm2*tau12*a12-6.*tau21*
     & tau23*ttu11*f2wm2*tau12*a12+tau21*tau13*tau23*ttu11*f2vm1*a12-
     & 6.*tau21*tau13*tau22*f2vm2*a13*ttu11+6.*tau21*tau22*f2wm2*
     & tau12*a13*ttu11+tau21*tau22*f2wm1*ttu11*tau12*a13+tau21*tau23*
     & ttu12*f2wm2*tau12*a12+tau21*tau13*ttu21*f2wm1*tau12*a12-6.*
     & tau22**2*f2wm2*tau11*a13*ttu11-1.*tau21*tau13**2*a12*tau22*f2f-
     & 1.*tau21*tau13*a12*tau22*f2wm2*ttu12+tau21*tau13*a13*tau12*
     & tau22*f2f-1.*tau22**2*f2wm1*ttu11*a13*tau11-1.*tau23**2*ttu12*
     & f2um2*a12*tau12-1.*ttu22*f2um2*a13*tau12**2*tau23+6.*ttu21*
     & f2um2*a13*tau12**2*tau23-1.*tau21*tau23*tau12**2*f2f*a13-6.*
     & tau23**2*ttu11*f2vm2*a12*tau11+6.*tau23**2*ttu11*f2um2*a12*
     & tau12-1.*tau13*tau22**2*f2um2*a13*ttu12-1.*tau23**2*ttu11*
     & f2vm1*tau11*a12-1.*tau21*a13*ttu11*tau23*tau12*f2vm1-1.*tau21*
     & tau23*tau12*f2vm2*a13*ttu12+6.*tau13**2*ttu21*tau22*f2um2*a12)
     & /(tau23*tau12*f2vm1*tau21*a13*tau11+6.*tau23*tau12*f2wm2*tau11*
     & a12*tau21+tau23*tau12*f2wm1*tau11*tau21*a12-1.*f2wm1*tau21*
     & tau12**2*tau23*a11+6.*tau23*tau12*f2vm2*tau21*a13*tau11-1.*
     & tau23*f2um1*tau12**2*tau21*a13+6.*tau13*f2vm2*tau21*tau23*a11*
     & tau12-2.*tau13*tau23*f2vm1*tau11*tau21*a12-6.*f2wm2*tau12**2*
     & tau21*tau23*a11-6.*tau13*f2vm2*tau21**2*a13*tau12+tau13*f2vm1*
     & tau21*tau23*tau12*a11-1.*tau23**2*f2um1*tau12*a12*tau11+tau13*
     & tau23*f2um1*tau12*tau21*a12-12.*tau13*f2vm2*tau21*tau23*a12*
     & tau11-6.*tau13*f2wm2*tau12*tau21**2*a12-1.*tau22**2*tau13*
     & f2um1*a13*tau11+tau22*f2wm1*tau11*tau23*tau12*a11-6.*tau22*
     & f2wm2*tau11**2*tau23*a12-6.*tau22*tau13**2*f2um2*a12*tau21-1.*
     & tau22*tau13**2*f2um1*tau21*a12-6.*tau22*tau13**2*f2vm2*tau21*
     & a11-1.*tau22*tau13**2*f2vm1*tau21*a11-1.*tau22*f2wm1*tau11**2*
     & a12*tau23-1.*tau22*tau23*f2vm1*tau11**2*a13-12.*tau22*tau13*
     & f2um2*tau23*a11*tau12-1.*tau13*f2vm1*tau21**2*tau12*a13-2.*
     & tau22*f2wm1*tau21*tau12*a13*tau11+tau22*tau23*f2um1*tau12*a13*
     & tau11-6.*f2um2*tau23**2*a12*tau11*tau12-6.*f2um2*a13*tau21*
     & tau12**2*tau23-6.*f2vm2*tau23**2*tau11*a11*tau12+6.*tau13*
     & f2um2*a12*tau21*tau23*tau12+6.*tau22*tau13*f2um2*a13*tau21*
     & tau12+6.*tau22*tau13*f2wm2*tau12*tau21*a11+tau13**2*f2vm1*
     & tau21**2*a12+tau22*tau13*f2vm1*tau21*a13*tau11+tau22*tau13*
     & f2wm1*tau11*tau21*a12+6.*tau13**2*f2vm2*tau21**2*a12-6.*tau22**
     & 2*tau13*f2um2*a13*tau11+tau22*tau13*f2wm1*tau21*tau12*a11+6.*
     & tau22*tau13*f2vm2*tau21*a13*tau11-2.*tau22*tau13*f2um1*a11*
     & tau23*tau12+tau22*tau13*f2um1*a12*tau11*tau23-1.*tau22**2*
     & tau13*f2wm1*tau11*a11+tau22*tau13*f2um1*tau21*tau12*a13-1.*
     & tau23**2*f2vm1*tau11*tau12*a11+6.*tau22*f2wm2*tau11*tau23*a11*
     & tau12-1.*tau13*f2wm1*tau21**2*tau12*a12-6.*tau22*f2vm2*tau23*
     & tau11**2*a13-6.*tau22**2*tau13*f2wm2*tau11*a11+tau23**2*f2vm1*
     & tau11**2*a12+6.*f2vm2*tau23**2*tau11**2*a12+6.*f2um2*tau23**2*
     & a11*tau12**2+6.*tau22*f2um2*a13*tau11*tau23*tau12-12.*tau22*
     & f2wm2*tau12*tau21*a13*tau11+6.*tau22*tau13*f2wm2*tau11*a12*
     & tau21+tau22*tau13*tau23*f2vm1*tau11*a11+6.*tau22*tau13*f2vm2*
     & tau23*tau11*a11+6.*tau22*tau13*f2um2*tau23*a12*tau11+tau22**2*
     & tau13**2*f2um1*a11+6.*f2wm2*tau12**2*tau21**2*a13+tau23**2*
     & f2um1*tau12**2*a11+f2wm1*tau21**2*tau12**2*a13+6.*tau22**2*
     & f2wm2*tau11**2*a13+tau22**2*f2wm1*tau11**2*a13+6.*tau22**2*
     & tau13**2*f2um2*a11)

      u(i1-2*is1,i2-2*is2,i3-2*is3,ex) = (-1.*a12*tau23**2*ttu12*f2um1*
     & tau12-1.*tau21*a13*ttu12*tau22*f2wm1*tau12-6.*tau21*tau13*a12*
     & tau23*ttu12*f2vm2+6.*tau21*tau13*a12*tau22*f2wm1*ttu11-6.*
     & tau21*tau13*ttu22*f2vm2*a13*tau12-6.*tau21*tau13*a12*tau23*
     & tau12*f2f-6.*tau21*tau13*a12*ttu22*f2wm2*tau12-1.*tau21*tau13*
     & a12*tau23*ttu12*f2vm1-6.*tau21*a12*tau23*tau12*f2wm1*ttu11+
     & tau21*ttu22*f2wm1*tau12**2*a13-1.*tau13**2*a12*ttu22*tau22*
     & f2um1+6.*tau13*tau23*tau12*f2vm2*ttu22*a11+a13*ttu12*tau22**2*
     & f2wm1*tau11-6.*tau23**2*a11*tau12**2*f2f+2.*tau13*f2um1*tau23*
     & tau12*f1f*tau22-1.*tau13*f2vm1*tau23*tau11*f1f*tau22-6.*tau13*
     & a11*tau22**2*f2wm2*ttu12+6.*a12*tau23**2*tau12*f2f*tau11+6.*
     & a12*tau23*tau12*f2wm2*tau11*ttu22-1.*a12*tau23*ttu12*tau22*
     & f2wm1*tau11-6.*a12*tau23*tau11*tau22*f2wm2*ttu12+6.*a12*tau23*
     & tau12*f2wm1*ttu21*tau11+6.*a12*tau11*tau23**2*ttu12*f2vm2+6.*
     & a12*tau23**2*tau12*f2um1*ttu11+a12*tau11*tau23**2*ttu12*f2vm1-
     & 6.*tau23**2*a11*ttu11*tau12*f2vm1-1.*ttu22*tau23*f2um1*tau12**
     & 2*a13+ttu22*tau23*tau12*f2vm1*a13*tau11+f2vm1*tau23**2*tau11*
     & f1f*tau12-1.*f2um1*tau23**2*tau12**2*f1f-1.*tau13**2*f2um1*
     & tau22**2*f1f-6.*tau23**2*tau12*f2vm2*ttu12*a11+6.*tau23*tau12**
     & 2*f2um1*ttu21*a13-1.*f2wm1*tau12*f1f*tau22*tau23*tau11-6.*
     & tau23*tau12**2*f2wm2*ttu22*a11-6.*a13*tau12*tau23*tau11*tau22*
     & f2f-6.*a11*ttu21*tau23*tau12**2*f2wm1+a13*tau12*tau23*ttu12*
     & tau22*f2um1-6.*tau23*tau11*tau22*f2vm2*a13*ttu12-6.*a13*tau12*
     & tau22*tau23*f2um1*ttu11+6.*tau23*a11*tau12*tau22*f2wm1*ttu11+
     & 6.*tau23*a11*tau12*tau22*f2wm2*ttu12-1.*ttu22*tau22*f2wm1*
     & tau12*a13*tau11-6.*a13*tau12*tau22*f2wm2*tau11*ttu22-1.*tau23*
     & ttu12*tau22*f2vm1*a13*tau11+tau13*a12*tau23*ttu12*tau22*f2um1-
     & 6.*tau13*a12*tau22*tau23*f2um1*ttu11-6.*tau13*a12*tau23*tau12*
     & f2um1*ttu21-1.*tau13*a12*ttu22*tau23*f2vm1*tau11+tau13*a12*
     & ttu22*tau23*f2um1*tau12-6.*tau13*a12*tau22*f2wm1*ttu21*tau11+
     & 6.*tau13*tau22*f2vm2*ttu22*a13*tau11-6.*tau13*a12*ttu22*f2vm2*
     & tau23*tau11+tau13*a12*ttu22*tau22*f2wm1*tau11-6.*tau13*a12*
     & tau23*tau11*tau22*f2f+6.*tau13*tau22*f2vm2*tau23*ttu12*a11-6.*
     & tau13*a13*tau12*tau22*f2um1*ttu21+6.*tau13*tau22*f2wm2*tau12*
     & ttu22*a11+6.*tau13*tau23*a11*ttu11*tau22*f2vm1+tau13*ttu22*
     & tau22*f2um1*tau12*a13+6.*tau13*a11*ttu21*tau23*tau12*f2vm1+6.*
     & tau13*a13*ttu21*tau11*tau22*f2vm1-6.*tau13*a11*tau22**2*f2wm1*
     & ttu11+6.*tau13*a11*ttu21*tau22*f2wm1*tau12+6.*tau13*a13*tau22**
     & 2*tau11*f2f+6.*tau13*a13*ttu11*tau22**2*f2um1-1.*tau13*a13*
     & ttu12*tau22**2*f2um1-6.*a13*ttu21*tau11*tau23*tau12*f2vm1-6.*
     & tau13**2*tau22*f2vm2*ttu22*a11+6.*tau13**2*a12*tau22*f2um1*
     & ttu21-6.*tau13**2*a11*ttu21*tau22*f2vm1-6.*tau13**2*a11*tau22**
     & 2*f2f+tau13*f2wm1*tau11*tau22**2*f1f+tau21*f2wm1*tau12**2*f1f*
     & tau23+12.*tau13*tau22*f2f*tau23*tau12*a11+tau21*tau13**2*a12*
     & ttu22*f2vm1+6.*tau21*tau13**2*a12*tau22*f2f+tau21*a12*tau23*
     & ttu12*f2wm1*tau12-1.*tau21*tau13*a12*ttu22*f2wm1*tau12-1.*
     & tau21*tau13*ttu22*f2vm1*tau12*a13+tau21*tau13*a13*ttu12*tau22*
     & f2vm1-6.*tau21*tau13*a13*ttu11*tau22*f2vm1+6.*tau21*tau13*a12*
     & tau22*f2wm2*ttu12-6.*tau21*tau13*a13*tau12*tau22*f2f+6.*tau21*
     & tau13**2*a12*ttu22*f2vm2+6.*tau21*tau23*tau12**2*f2f*a13+6.*
     & tau21*ttu22*f2wm2*tau12**2*a13+6.*tau21*a13*ttu11*tau23*tau12*
     & f2vm1-6.*tau21*a13*tau12*tau22*f2wm2*ttu12+6.*tau21*tau23*
     & tau12*f2vm2*a13*ttu12-1.*tau21*tau13*f2wm1*tau12*f1f*tau22-1.*
     & tau21*tau13*f2vm1*f1f*tau23*tau12+tau21*tau13**2*f2vm1*f1f*
     & tau22+6.*a13*tau22**2*tau11*f2wm2*ttu12)/(tau23*tau12*f2vm1*
     & tau21*a13*tau11+6.*tau23*tau12*f2wm2*tau11*a12*tau21+tau23*
     & tau12*f2wm1*tau11*tau21*a12-1.*f2wm1*tau21*tau12**2*tau23*a11+
     & 6.*tau23*tau12*f2vm2*tau21*a13*tau11-1.*tau23*f2um1*tau12**2*
     & tau21*a13+6.*tau13*f2vm2*tau21*tau23*a11*tau12-2.*tau13*tau23*
     & f2vm1*tau11*tau21*a12-6.*f2wm2*tau12**2*tau21*tau23*a11-6.*
     & tau13*f2vm2*tau21**2*a13*tau12+tau13*f2vm1*tau21*tau23*tau12*
     & a11-1.*tau23**2*f2um1*tau12*a12*tau11+tau13*tau23*f2um1*tau12*
     & tau21*a12-12.*tau13*f2vm2*tau21*tau23*a12*tau11-6.*tau13*f2wm2*
     & tau12*tau21**2*a12-1.*tau22**2*tau13*f2um1*a13*tau11+tau22*
     & f2wm1*tau11*tau23*tau12*a11-6.*tau22*f2wm2*tau11**2*tau23*a12-
     & 6.*tau22*tau13**2*f2um2*a12*tau21-1.*tau22*tau13**2*f2um1*
     & tau21*a12-6.*tau22*tau13**2*f2vm2*tau21*a11-1.*tau22*tau13**2*
     & f2vm1*tau21*a11-1.*tau22*f2wm1*tau11**2*a12*tau23-1.*tau22*
     & tau23*f2vm1*tau11**2*a13-12.*tau22*tau13*f2um2*tau23*a11*tau12-
     & 1.*tau13*f2vm1*tau21**2*tau12*a13-2.*tau22*f2wm1*tau21*tau12*
     & a13*tau11+tau22*tau23*f2um1*tau12*a13*tau11-6.*f2um2*tau23**2*
     & a12*tau11*tau12-6.*f2um2*a13*tau21*tau12**2*tau23-6.*f2vm2*
     & tau23**2*tau11*a11*tau12+6.*tau13*f2um2*a12*tau21*tau23*tau12+
     & 6.*tau22*tau13*f2um2*a13*tau21*tau12+6.*tau22*tau13*f2wm2*
     & tau12*tau21*a11+tau13**2*f2vm1*tau21**2*a12+tau22*tau13*f2vm1*
     & tau21*a13*tau11+tau22*tau13*f2wm1*tau11*tau21*a12+6.*tau13**2*
     & f2vm2*tau21**2*a12-6.*tau22**2*tau13*f2um2*a13*tau11+tau22*
     & tau13*f2wm1*tau21*tau12*a11+6.*tau22*tau13*f2vm2*tau21*a13*
     & tau11-2.*tau22*tau13*f2um1*a11*tau23*tau12+tau22*tau13*f2um1*
     & a12*tau11*tau23-1.*tau22**2*tau13*f2wm1*tau11*a11+tau22*tau13*
     & f2um1*tau21*tau12*a13-1.*tau23**2*f2vm1*tau11*tau12*a11+6.*
     & tau22*f2wm2*tau11*tau23*a11*tau12-1.*tau13*f2wm1*tau21**2*
     & tau12*a12-6.*tau22*f2vm2*tau23*tau11**2*a13-6.*tau22**2*tau13*
     & f2wm2*tau11*a11+tau23**2*f2vm1*tau11**2*a12+6.*f2vm2*tau23**2*
     & tau11**2*a12+6.*f2um2*tau23**2*a11*tau12**2+6.*tau22*f2um2*a13*
     & tau11*tau23*tau12-12.*tau22*f2wm2*tau12*tau21*a13*tau11+6.*
     & tau22*tau13*f2wm2*tau11*a12*tau21+tau22*tau13*tau23*f2vm1*
     & tau11*a11+6.*tau22*tau13*f2vm2*tau23*tau11*a11+6.*tau22*tau13*
     & f2um2*tau23*a12*tau11+tau22**2*tau13**2*f2um1*a11+6.*f2wm2*
     & tau12**2*tau21**2*a13+tau23**2*f2um1*tau12**2*a11+f2wm1*tau21**
     & 2*tau12**2*a13+6.*tau22**2*f2wm2*tau11**2*a13+tau22**2*f2wm1*
     & tau11**2*a13+6.*tau22**2*tau13**2*f2um2*a11)

      u(i1-is1,i2-is2,i3-is3,ey) = (-1.*tau23**2*tau11*f1f*f2um2*tau12+
     & tau23*tau12*tau21*f1f*f2wm2*tau11+tau13**2*tau21**2*f1f*f2vm2+
     & tau23**2*tau11**2*f1f*f2vm2-1.*tau13*tau21**2*f1f*f2wm2*tau12+
     & tau13*tau21*f1f*f2um2*tau23*tau12-1.*tau22*tau23*tau11**2*f1f*
     & f2wm2-2.*tau13*tau21*f1f*f2vm2*tau23*tau11-1.*tau22*tau13**2*
     & tau21*f1f*f2um2+tau22*tau13*tau21*f1f*f2wm2*tau11-6.*tau13**2*
     & f2vm2*tau21*a11*ttu21-1.*tau13**2*f2um2*a12*tau21*ttu22+tau13*
     & f2wm2*tau11*a12*tau21*ttu22+tau13*f2um2*a13*tau12*tau21*ttu22-
     & 1.*tau13*tau21*f2f*tau23*tau12*a11+tau13*tau23*f2um1*ttu11*
     & tau21*a12-1.*tau13*f2vm2*tau21*ttu22*a13*tau11+tau13*f2um1*
     & ttu21*a12*tau11*tau23+tau13*f2um1*ttu21*tau21*tau12*a13-1.*
     & tau13*f2um1*ttu21*tau23*tau12*a11+tau13*tau21**2*f2f*tau12*a13-
     & 1.*tau13*tau21**2*f2wm2*ttu12*a12+6.*tau13*f2vm2*tau21*a13*
     & ttu21*tau11-1.*tau13*f2vm2*tau21*tau23*ttu12*a11-1.*tau13*
     & f2wm2*tau12*tau21*ttu22*a11-1.*tau13*f2vm2*tau23*tau11*ttu22*
     & a11+tau13*f2wm1*ttu21*tau11*tau21*a12+tau13*f2vm2*tau21**2*a13*
     & ttu12+tau22*tau13*tau23*tau11*f1f*f2um2-1.*tau23*tau12*tau21*
     & f2f*a13*tau11-6.*tau13*f2vm2*tau21**2*a13*ttu11+2.*tau13*tau21*
     & f2f*a12*tau11*tau23+6.*tau13*f2vm2*tau23*tau11*a11*ttu21+tau13*
     & f2um2*a12*tau21*tau23*ttu12+tau13*f2um2*a12*tau11*tau23*ttu22+
     & 6.*tau13*f2wm2*tau12*tau21*a11*ttu21-6.*tau13*f2um2*a11*ttu21*
     & tau23*tau12+6.*tau13*f2vm2*tau21*tau23*a11*ttu11-1.*tau13**2*
     & f2um1*ttu21*tau21*a12+tau13**2*f2vm2*tau21*ttu22*a11+6.*f2vm2*
     & tau23*tau11*a13*tau21*ttu11+tau23*tau11*f2wm1*tau12*ttu21*a11-
     & 1.*f2vm2*tau21*tau23*ttu12*a13*tau11-1.*f2wm2*tau11**2*a12*
     & tau23*ttu22+f2vm2*tau23*tau11**2*ttu22*a13+tau23**2*f2um1*
     & ttu11*tau12*a11-6.*f2wm2*tau12*tau21*a13*ttu21*tau11-1.*f2wm1*
     & ttu21*tau11*tau21*tau12*a13-1.*f2wm1*ttu21*tau11**2*a12*tau23-
     & 6.*f2vm2*tau23*tau11**2*a13*ttu21-1.*f2um2*tau23*a13*tau12*
     & tau11*ttu22-1.*tau21*f2wm1*ttu11*a11*tau23*tau12+f2vm2*tau23**
     & 2*tau11*ttu12*a11+tau21*f2wm2*ttu12*a12*tau11*tau23+tau23*
     & tau11*f2wm2*tau12*ttu22*a11+6.*f2um2*a13*ttu21*tau11*tau23*
     & tau12-1.*tau23*f2um1*ttu11*tau21*tau12*a13-1.*tau23**2*f2um1*
     & ttu11*a12*tau11+tau23**2*tau11*f2f*tau12*a11+tau21**2*f2wm1*
     & ttu11*tau12*a13+tau21*f2wm1*ttu11*a12*tau11*tau23-1.*tau23**2*
     & tau11**2*f2f*a12-1.*tau13**2*tau21**2*f2f*a12-1.*tau13*tau21**
     & 2*f2wm1*ttu11*a12+6.*f2um2*tau23**2*a11*ttu11*tau12+6.*f2wm2*
     & tau12*tau21**2*a13*ttu11-6.*f2wm2*tau12*tau21*tau23*a11*ttu11-
     & 1.*f2um2*a12*tau11*tau23**2*ttu12-6.*f2um2*a13*tau21*ttu11*
     & tau23*tau12-6.*f2vm2*tau23**2*tau11*a11*ttu11-1.*tau22*tau13*
     & tau21*f2f*a13*tau11-1.*tau22*tau13*tau23*f2um1*ttu11*a11+tau22*
     & tau13*tau21*f2wm2*ttu12*a11-6.*tau22*tau13*f2wm2*tau11*a11*
     & ttu21-1.*tau22*tau23*f2wm2*ttu12*tau11*a11-6.*tau22*tau13*
     & f2um2*a13*ttu21*tau11+6.*tau22*f2wm2*tau11**2*a13*ttu21+tau22*
     & f2wm1*ttu21*tau11**2*a13+tau22*tau23*tau11**2*f2f*a13+tau22*
     & tau13**2*f2um1*ttu21*a11+tau22*tau13**2*tau21*f2f*a11+6.*tau22*
     & tau13**2*f2um2*a11*ttu21+6.*tau22*f2wm2*tau11*tau23*a11*ttu11-
     & 1.*tau22*tau13*tau23*tau11*f2f*a11-1.*tau22*tau13*f2wm1*ttu21*
     & tau11*a11-1.*tau22*tau13*f2um1*ttu21*a13*tau11-1.*tau22*tau13*
     & f2um2*a13*ttu12*tau21+tau22*tau13*tau21*f2wm1*ttu11*a11-6.*
     & tau22*f2wm2*tau11*a13*tau21*ttu11+tau22*tau23*f2um1*ttu11*a13*
     & tau11+tau22*f2um2*tau23*a13*ttu12*tau11+6.*tau22*tau13*f2um2*
     & a13*tau21*ttu11-1.*tau22*tau21*f2wm1*ttu11*a13*tau11-6.*tau22*
     & tau13*f2um2*tau23*a11*ttu11)/(tau23*tau12*f2vm1*tau21*a13*
     & tau11+6.*tau23*tau12*f2wm2*tau11*a12*tau21+tau23*tau12*f2wm1*
     & tau11*tau21*a12-1.*f2wm1*tau21*tau12**2*tau23*a11+6.*tau23*
     & tau12*f2vm2*tau21*a13*tau11-1.*tau23*f2um1*tau12**2*tau21*a13+
     & 6.*tau13*f2vm2*tau21*tau23*a11*tau12-2.*tau13*tau23*f2vm1*
     & tau11*tau21*a12-6.*f2wm2*tau12**2*tau21*tau23*a11-6.*tau13*
     & f2vm2*tau21**2*a13*tau12+tau13*f2vm1*tau21*tau23*tau12*a11-1.*
     & tau23**2*f2um1*tau12*a12*tau11+tau13*tau23*f2um1*tau12*tau21*
     & a12-12.*tau13*f2vm2*tau21*tau23*a12*tau11-6.*tau13*f2wm2*tau12*
     & tau21**2*a12-1.*tau22**2*tau13*f2um1*a13*tau11+tau22*f2wm1*
     & tau11*tau23*tau12*a11-6.*tau22*f2wm2*tau11**2*tau23*a12-6.*
     & tau22*tau13**2*f2um2*a12*tau21-1.*tau22*tau13**2*f2um1*tau21*
     & a12-6.*tau22*tau13**2*f2vm2*tau21*a11-1.*tau22*tau13**2*f2vm1*
     & tau21*a11-1.*tau22*f2wm1*tau11**2*a12*tau23-1.*tau22*tau23*
     & f2vm1*tau11**2*a13-12.*tau22*tau13*f2um2*tau23*a11*tau12-1.*
     & tau13*f2vm1*tau21**2*tau12*a13-2.*tau22*f2wm1*tau21*tau12*a13*
     & tau11+tau22*tau23*f2um1*tau12*a13*tau11-6.*f2um2*tau23**2*a12*
     & tau11*tau12-6.*f2um2*a13*tau21*tau12**2*tau23-6.*f2vm2*tau23**
     & 2*tau11*a11*tau12+6.*tau13*f2um2*a12*tau21*tau23*tau12+6.*
     & tau22*tau13*f2um2*a13*tau21*tau12+6.*tau22*tau13*f2wm2*tau12*
     & tau21*a11+tau13**2*f2vm1*tau21**2*a12+tau22*tau13*f2vm1*tau21*
     & a13*tau11+tau22*tau13*f2wm1*tau11*tau21*a12+6.*tau13**2*f2vm2*
     & tau21**2*a12-6.*tau22**2*tau13*f2um2*a13*tau11+tau22*tau13*
     & f2wm1*tau21*tau12*a11+6.*tau22*tau13*f2vm2*tau21*a13*tau11-2.*
     & tau22*tau13*f2um1*a11*tau23*tau12+tau22*tau13*f2um1*a12*tau11*
     & tau23-1.*tau22**2*tau13*f2wm1*tau11*a11+tau22*tau13*f2um1*
     & tau21*tau12*a13-1.*tau23**2*f2vm1*tau11*tau12*a11+6.*tau22*
     & f2wm2*tau11*tau23*a11*tau12-1.*tau13*f2wm1*tau21**2*tau12*a12-
     & 6.*tau22*f2vm2*tau23*tau11**2*a13-6.*tau22**2*tau13*f2wm2*
     & tau11*a11+tau23**2*f2vm1*tau11**2*a12+6.*f2vm2*tau23**2*tau11**
     & 2*a12+6.*f2um2*tau23**2*a11*tau12**2+6.*tau22*f2um2*a13*tau11*
     & tau23*tau12-12.*tau22*f2wm2*tau12*tau21*a13*tau11+6.*tau22*
     & tau13*f2wm2*tau11*a12*tau21+tau22*tau13*tau23*f2vm1*tau11*a11+
     & 6.*tau22*tau13*f2vm2*tau23*tau11*a11+6.*tau22*tau13*f2um2*
     & tau23*a12*tau11+tau22**2*tau13**2*f2um1*a11+6.*f2wm2*tau12**2*
     & tau21**2*a13+tau23**2*f2um1*tau12**2*a11+f2wm1*tau21**2*tau12**
     & 2*a13+6.*tau22**2*f2wm2*tau11**2*a13+tau22**2*f2wm1*tau11**2*
     & a13+6.*tau22**2*tau13**2*f2um2*a11)

      u(i1-2*is1,i2-2*is2,i3-2*is3,ey) = -1.*(-1.*tau13*tau21**2*f2wm1*
     & tau12*f1f+tau13**2*tau21**2*f2vm1*f1f+tau23*tau12*tau21*f2wm1*
     & tau11*f1f-1.*tau23**2*tau11*f2um1*tau12*f1f-1.*tau22*tau23*
     & tau11**2*f2wm1*f1f-2.*tau13*tau21*f2vm1*tau23*tau11*f1f+tau13*
     & tau21*f2um1*tau23*tau12*f1f-1.*tau22*tau13**2*tau21*f2um1*f1f+
     & tau22*tau13*tau21*f2wm1*tau11*f1f+tau22*tau13*tau23*tau11*
     & f2um1*f1f+6.*tau22*tau13*tau21*a13*ttu11*f2um1+tau22*tau13*
     & tau23*ttu12*f2um1*a11+tau22*tau21*a13*ttu12*f2wm1*tau11+6.*
     & tau22*tau13*ttu22*f2wm2*tau11*a11+6.*tau22*tau23*tau11*a11*
     & f2wm1*ttu11-1.*tau22*tau13**2*ttu22*f2um1*a11-6.*tau22*tau13**
     & 2*ttu22*f2um2*a11-1.*tau22*ttu22*f2wm1*tau11**2*a13-6.*tau22*
     & ttu22*f2wm2*tau11**2*a13+6.*tau22*tau21*a13*tau11*f2wm2*ttu12-
     & 1.*tau22*tau23*ttu12*f2wm1*tau11*a11+tau22*tau13*ttu22*f2wm1*
     & tau11*a11+tau22*tau13*ttu22*f2um1*a13*tau11+6.*tau22*tau13*
     & tau23*ttu12*f2um2*a11+6.*tau22*tau13*ttu22*f2um2*a13*tau11-1.*
     & tau22*tau13*tau21*a13*ttu12*f2um1+tau13*tau21**2*a13*ttu12*
     & f2vm1-6.*tau13*tau21**2*a13*ttu11*f2vm1+6.*tau13*tau21*a13*
     & ttu21*tau11*f2vm1+6.*tau13*tau21*a11*ttu21*f2wm1*tau12-1.*
     & tau13*tau23*ttu12*f2vm1*tau21*a11-1.*tau13*ttu22*tau23*f2vm1*
     & tau11*a11+6.*tau13*ttu22*f2um2*tau23*a11*tau12+6.*tau13*tau21*
     & tau23*a11*ttu11*f2vm1-1.*tau13*ttu22*f2vm1*tau21*a13*tau11-1.*
     & tau13*ttu22*f2wm1*tau21*tau12*a11+tau13*ttu22*f2um1*a11*tau23*
     & tau12+6.*tau13*tau23*tau11*a11*ttu21*f2vm1-6.*tau13**2*tau21*
     & a11*ttu21*f2vm1+tau13**2*ttu22*f2vm1*tau21*a11-6.*tau21**2*a13*
     & tau12*f2wm2*ttu12+6.*tau23*tau11*a13*tau12*f2um1*ttu21-1.*
     & tau23*tau11*ttu22*f2um1*tau12*a13-6.*tau23*tau11**2*a13*ttu21*
     & f2vm1-6.*tau23**2*ttu12*f2um2*a11*tau12+tau21*ttu22*f2wm1*
     & tau12*a13*tau11+6.*tau21*a13*tau12*f2wm2*tau11*ttu22+ttu22*
     & tau23*f2vm1*tau11**2*a13-1.*tau23**2*ttu12*f2um1*a11*tau12-1.*
     & tau21**2*a13*ttu12*f2wm1*tau12+6.*tau21*tau23*a11*tau12*f2wm2*
     & ttu12+tau23*ttu12*f2wm1*tau21*tau12*a11+tau23**2*ttu12*f2vm1*
     & tau11*a11+6.*tau23*ttu12*f2um2*a13*tau21*tau12+6.*tau23*tau11*
     & tau21*a13*ttu11*f2vm1-1.*tau21*tau23*ttu12*f2vm1*a13*tau11-6.*
     & tau23**2*tau11*a11*ttu11*f2vm1+tau21*a13*tau12*tau23*ttu12*
     & f2um1+tau23**2*tau11**2*f2vm1*f1f+6.*tau13**2*f2um2*a12*tau21*
     & ttu22-6.*tau13*f2wm2*tau11*a12*tau21*ttu22-6.*tau13*f2um2*a13*
     & tau12*tau21*ttu22+6.*tau13*tau21*f2f*tau23*tau12*a11-6.*tau13*
     & tau23*f2um1*ttu11*tau21*a12-6.*tau13*f2um1*ttu21*a12*tau11*
     & tau23-6.*tau13*f2um1*ttu21*tau21*tau12*a13-6.*tau13*tau21**2*
     & f2f*tau12*a13+6.*tau13*tau21**2*f2wm2*ttu12*a12-6.*tau13*f2wm1*
     & ttu21*tau11*tau21*a12+6.*tau23*tau12*tau21*f2f*a13*tau11-12.*
     & tau13*tau21*f2f*a12*tau11*tau23-6.*tau13*f2um2*a12*tau21*tau23*
     & ttu12-6.*tau13*f2um2*a12*tau11*tau23*ttu22+6.*tau13**2*f2um1*
     & ttu21*tau21*a12-6.*tau23*tau11*f2wm1*tau12*ttu21*a11+6.*f2wm2*
     & tau11**2*a12*tau23*ttu22+6.*f2wm1*ttu21*tau11**2*a12*tau23-6.*
     & tau21*f2wm2*ttu12*a12*tau11*tau23-6.*tau23*tau11*f2wm2*tau12*
     & ttu22*a11+6.*tau23**2*f2um1*ttu11*a12*tau11-6.*tau23**2*tau11*
     & f2f*tau12*a11-6.*tau21*f2wm1*ttu11*a12*tau11*tau23+6.*tau23**2*
     & tau11**2*f2f*a12+6.*tau13**2*tau21**2*f2f*a12+6.*tau13*tau21**
     & 2*f2wm1*ttu11*a12+6.*f2um2*a12*tau11*tau23**2*ttu12+6.*tau22*
     & tau13*tau21*f2f*a13*tau11-6.*tau22*tau13*tau21*f2wm2*ttu12*a11-
     & 6.*tau22*tau23*tau11**2*f2f*a13-6.*tau22*tau13**2*tau21*f2f*
     & a11+6.*tau22*tau13*tau23*tau11*f2f*a11-6.*tau22*tau13*tau21*
     & f2wm1*ttu11*a11-6.*tau22*tau23*f2um1*ttu11*a13*tau11-6.*tau22*
     & f2um2*tau23*a13*ttu12*tau11)/(tau23*tau12*f2vm1*tau21*a13*
     & tau11+6.*tau23*tau12*f2wm2*tau11*a12*tau21+tau23*tau12*f2wm1*
     & tau11*tau21*a12-1.*f2wm1*tau21*tau12**2*tau23*a11+6.*tau23*
     & tau12*f2vm2*tau21*a13*tau11-1.*tau23*f2um1*tau12**2*tau21*a13+
     & 6.*tau13*f2vm2*tau21*tau23*a11*tau12-2.*tau13*tau23*f2vm1*
     & tau11*tau21*a12-6.*f2wm2*tau12**2*tau21*tau23*a11-6.*tau13*
     & f2vm2*tau21**2*a13*tau12+tau13*f2vm1*tau21*tau23*tau12*a11-1.*
     & tau23**2*f2um1*tau12*a12*tau11+tau13*tau23*f2um1*tau12*tau21*
     & a12-12.*tau13*f2vm2*tau21*tau23*a12*tau11-6.*tau13*f2wm2*tau12*
     & tau21**2*a12-1.*tau22**2*tau13*f2um1*a13*tau11+tau22*f2wm1*
     & tau11*tau23*tau12*a11-6.*tau22*f2wm2*tau11**2*tau23*a12-6.*
     & tau22*tau13**2*f2um2*a12*tau21-1.*tau22*tau13**2*f2um1*tau21*
     & a12-6.*tau22*tau13**2*f2vm2*tau21*a11-1.*tau22*tau13**2*f2vm1*
     & tau21*a11-1.*tau22*f2wm1*tau11**2*a12*tau23-1.*tau22*tau23*
     & f2vm1*tau11**2*a13-12.*tau22*tau13*f2um2*tau23*a11*tau12-1.*
     & tau13*f2vm1*tau21**2*tau12*a13-2.*tau22*f2wm1*tau21*tau12*a13*
     & tau11+tau22*tau23*f2um1*tau12*a13*tau11-6.*f2um2*tau23**2*a12*
     & tau11*tau12-6.*f2um2*a13*tau21*tau12**2*tau23-6.*f2vm2*tau23**
     & 2*tau11*a11*tau12+6.*tau13*f2um2*a12*tau21*tau23*tau12+6.*
     & tau22*tau13*f2um2*a13*tau21*tau12+6.*tau22*tau13*f2wm2*tau12*
     & tau21*a11+tau13**2*f2vm1*tau21**2*a12+tau22*tau13*f2vm1*tau21*
     & a13*tau11+tau22*tau13*f2wm1*tau11*tau21*a12+6.*tau13**2*f2vm2*
     & tau21**2*a12-6.*tau22**2*tau13*f2um2*a13*tau11+tau22*tau13*
     & f2wm1*tau21*tau12*a11+6.*tau22*tau13*f2vm2*tau21*a13*tau11-2.*
     & tau22*tau13*f2um1*a11*tau23*tau12+tau22*tau13*f2um1*a12*tau11*
     & tau23-1.*tau22**2*tau13*f2wm1*tau11*a11+tau22*tau13*f2um1*
     & tau21*tau12*a13-1.*tau23**2*f2vm1*tau11*tau12*a11+6.*tau22*
     & f2wm2*tau11*tau23*a11*tau12-1.*tau13*f2wm1*tau21**2*tau12*a12-
     & 6.*tau22*f2vm2*tau23*tau11**2*a13-6.*tau22**2*tau13*f2wm2*
     & tau11*a11+tau23**2*f2vm1*tau11**2*a12+6.*f2vm2*tau23**2*tau11**
     & 2*a12+6.*f2um2*tau23**2*a11*tau12**2+6.*tau22*f2um2*a13*tau11*
     & tau23*tau12-12.*tau22*f2wm2*tau12*tau21*a13*tau11+6.*tau22*
     & tau13*f2wm2*tau11*a12*tau21+tau22*tau13*tau23*f2vm1*tau11*a11+
     & 6.*tau22*tau13*f2vm2*tau23*tau11*a11+6.*tau22*tau13*f2um2*
     & tau23*a12*tau11+tau22**2*tau13**2*f2um1*a11+6.*f2wm2*tau12**2*
     & tau21**2*a13+tau23**2*f2um1*tau12**2*a11+f2wm1*tau21**2*tau12**
     & 2*a13+6.*tau22**2*f2wm2*tau11**2*a13+tau22**2*f2wm1*tau11**2*
     & a13+6.*tau22**2*tau13**2*f2um2*a11)


 ! *********** done *********************
               if( debug.gt.0 )then
                 call ogf3d(ep,xy(i1-is1,i2-is2,i3-is3,0),xy(i1-is1,i2-
     & is2,i3-is3,1),xy(i1-is1,i2-is2,i3-is3,2),t,uvm(0),uvm(1),uvm(2)
     & )
                 call ogf3d(ep,xy(i1-2*is1,i2-2*is2,i3-2*is3,0),xy(i1-
     & 2*is1,i2-2*is2,i3-2*is3,1),xy(i1-2*is1,i2-2*is2,i3-2*is3,2),t,
     & uvm2(0),uvm2(1),uvm2(2))
                write(*,'(" **ghost-interp3d: errors u(-1)=",3e10.2)') 
     & u(i1-is1,i2-is2,i3-is3,ex)-uvm(0),u(i1-is1,i2-is2,i3-is3,ey)-
     & uvm(1),u(i1-is1,i2-is2,i3-is3,ez)-uvm(2)
                write(*,'(" **ghost-interp3d: errors u(-2)=",3e10.2)') 
     & u(i1-2*is1,i2-2*is2,i3-2*is3,ex)-uvm2(0),u(i1-2*is1,i2-2*is2,
     & i3-2*is3,ey)-uvm2(1),u(i1-2*is1,i2-2*is2,i3-2*is3,ez)-uvm2(2)
               end if
               ! set to exact for testing
               ! OGF3D(i1-is1,i2-is2,i3-is3,t, uvm(0),uvm(1),uvm(2))
               ! OGF3D(i1-2*is1,i2-2*is2,i3-2*is3,t, uvm2(0),uvm2(1),uvm2(2))
               ! u(i1-is1,i2-is2,i3-is3,ex)=uvm(0)
               ! u(i1-is1,i2-is2,i3-is3,ey)=uvm(1)
               ! u(i1-is1,i2-is2,i3-is3,ez)=uvm(2)
               ! u(i1-2*is1,i2-2*is2,i3-2*is3,ex)=uvm2(0)
               ! u(i1-2*is1,i2-2*is2,i3-2*is3,ey)=uvm2(1)
               ! u(i1-2*is1,i2-2*is2,i3-2*is3,ez)=uvm2(2)
             ! &&&&&&&&&&&&&&&&&&&&&&&
             ! *   detnt=tau23*a11*tau12-tau23*a12*tau11-a13*tau21*tau12+tau21*tau13*a12+a13*tau22*tau11-tau22*tau13*a11
             ! *   do m=1,2
             ! *     m1=i1-m*is1
             ! *     m2=i2-m*is2
             ! *     m3=i3-m*is3
             ! *     ! use u.r=0 for now:
             ! *     !    tau.urr=0
             ! *     a1DotU= a11*u(i1+m*is1,i2+m*is2,i3+m*is3,ex)! *            +a12*u(i1+m*is1,i2+m*is2,i3+m*is3,ey)! *            +a13*u(i1+m*is1,i2+m*is2,i3+m*is3,ez)  
             ! *     tau1DotU=-( tau11*u(i1+m*is1,i2+m*is2,i3+m*is3,ex)! *                +tau12*u(i1+m*is1,i2+m*is2,i3+m*is3,ey)! *                +tau13*u(i1+m*is1,i2+m*is2,i3+m*is3,ez) )
             ! *     tau2DotU=-( tau21*u(i1+m*is1,i2+m*is2,i3+m*is3,ex)! *                +tau22*u(i1+m*is1,i2+m*is2,i3+m*is3,ey)! *                +tau23*u(i1+m*is1,i2+m*is2,i3+m*is3,ez) )
             ! *   
             ! *     u(m1,m2,m3,ex)=(tau23*a1DotU*tau12-a13*tau2DotU*tau12+a13*tau22*tau1DotU+tau2DotU*tau13*a12-tau22*tau13*a1DotU-tau23*a12*tau1DotU)/detnt
             ! *     u(m1,m2,m3,ey)=(-tau13*a11*tau2DotU+tau13*a1DotU*tau21+a11*tau23*tau1DotU+a13*tau11*tau2DotU-a1DotU*tau23*tau11-a13*tau1DotU*tau21)/detnt
             ! *     u(m1,m2,m3,ez)=(a11*tau2DotU*tau12-a11*tau22*tau1DotU-a12*tau11*tau2DotU+a12*tau1DotU*tau21-a1DotU*tau21*tau12+a1DotU*tau22*tau11)/detnt
             ! *   end do 
             end if
             end do
             end do
             end do
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
          ! ok if( .true. ) return ! **********************************************************
          ! In parallel we need to update ghost boundaries after stage 1
          ! **call updateGhostBoundaries(pu)
          call updateGhostAndPeriodic(pu)
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
                  u(i1-is1,i2-is2,i3-is3,et2)=2.*u(i1,i2,i3,et2)-u(i1+
     & is1,i2+is2,i3+is3,et2)
                if( useChargeDensity.eq.1 )then
                 ! div(eps*E) = rho , rho is saved in f(i1,i2,i3,0)
                 u(i1-is1,i2-is2,i3-is3,en1)=u(i1-is1,i2-is2,i3-is3,
     & en1) - 2.*dx(axis)*(1-2*side)*f(i1,i2,i3,0)/eps
                end if
                  u(i1-2*is1,i2-2*is2,i3-2*is3,en1)= u(i1+2*is1,i2+2*
     & is2,i3+2*is3,en1)
                  u(i1-2*is1,i2-2*is2,i3-2*is3,et1)=2.*u(i1,i2,i3,et1)-
     & u(i1+2*is1,i2+2*is2,i3+2*is3,et1)
                    u(i1-2*is1,i2-2*is2,i3-2*is3,et2)=2.*u(i1,i2,i3,
     & et2)-u(i1+2*is1,i2+2*is2,i3+2*is3,et2)
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
                  u(i1-is1,i2-is2,i3-is3,et2)=2.*u(i1,i2,i3,et2)-u(i1+
     & is1,i2+is2,i3+is3,et2)
                if( useChargeDensity.eq.1 )then
                 ! div(eps*E) = rho , rho is saved in f(i1,i2,i3,0)
                 u(i1-is1,i2-is2,i3-is3,en1)=u(i1-is1,i2-is2,i3-is3,
     & en1) - 2.*dx(axis)*(1-2*side)*f(i1,i2,i3,0)/eps
                end if
                      call ogf3dfo(ep,fieldOption,xy(i1-is1,i2-is2,i3-
     & is3,0),xy(i1-is1,i2-is2,i3-is3,1),xy(i1-is1,i2-is2,i3-is3,2),t,
     & uvm(ex),uvm(ey),uvm(ez))
                      call ogf3dfo(ep,fieldOption,xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),xy(i1,i2,i3,2),t,uv0(ex),uv0(ey),uv0(ez))
                      call ogf3dfo(ep,fieldOption,xy(i1+is1,i2+is2,i3+
     & is3,0),xy(i1+is1,i2+is2,i3+is3,1),xy(i1+is1,i2+is2,i3+is3,2),t,
     & uvp(ex),uvp(ey),uvp(ez))
                    u(i1-is1,i2-is2,i3-is3,en1)=u(i1-is1,i2-is2,i3-is3,
     & en1) + uvm(en1) - uvp(en1)
                    u(i1-is1,i2-is2,i3-is3,et1)=u(i1-is1,i2-is2,i3-is3,
     & et1) + uvm(et1) -2.*uv0(et1) + uvp(et1)
                    u(i1-is1,i2-is2,i3-is3,et2)=u(i1-is1,i2-is2,i3-is3,
     & et2) + uvm(et2) -2.*uv0(et2) + uvp(et2)
                  u(i1-2*is1,i2-2*is2,i3-2*is3,en1)= u(i1+2*is1,i2+2*
     & is2,i3+2*is3,en1)
                  u(i1-2*is1,i2-2*is2,i3-2*is3,et1)=2.*u(i1,i2,i3,et1)-
     & u(i1+2*is1,i2+2*is2,i3+2*is3,et1)
                    u(i1-2*is1,i2-2*is2,i3-2*is3,et2)=2.*u(i1,i2,i3,
     & et2)-u(i1+2*is1,i2+2*is2,i3+2*is3,et2)
                      call ogf3dfo(ep,fieldOption,xy(i1-2*is1,i2-2*is2,
     & i3-2*is3,0),xy(i1-2*is1,i2-2*is2,i3-2*is3,1),xy(i1-2*is1,i2-2*
     & is2,i3-2*is3,2),t,uvm(ex),uvm(ey),uvm(ez))
                      call ogf3dfo(ep,fieldOption,xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),xy(i1,i2,i3,2),t,uv0(ex),uv0(ey),uv0(ez))
                      call ogf3dfo(ep,fieldOption,xy(i1+2*is1,i2+2*is2,
     & i3+2*is3,0),xy(i1+2*is1,i2+2*is2,i3+2*is3,1),xy(i1+2*is1,i2+2*
     & is2,i3+2*is3,2),t,uvp(ex),uvp(ey),uvp(ez))
                    u(i1-2*is1,i2-2*is2,i3-2*is3,en1)=u(i1-2*is1,i2-2*
     & is2,i3-2*is3,en1) + uvm(en1) - uvp(en1)
                    u(i1-2*is1,i2-2*is2,i3-2*is3,et1)=u(i1-2*is1,i2-2*
     & is2,i3-2*is3,et1) + uvm(et1) -2.*uv0(et1) + uvp(et1)
                    u(i1-2*is1,i2-2*is2,i3-2*is3,et2)=u(i1-2*is1,i2-2*
     & is2,i3-2*is3,et2) + uvm(et2) -2.*uv0(et2) + uvp(et2)
                    ! if( debug.gt.1 )then
                    !  write(*,'(" bc4r: i=",3i4," err(-2)=",3e10.2)') i1,i2,i3,u(i1-2*is1,i2-2*is2,i3-2*is3,ex)-uvm(ex),!       u(i1-2*is1,i2-2*is2,i3-2*is3,ey)-uvm(ey), u(i1-2*is1,i2-2*is2,i3-2*is3,ez)-uvm(ez)
                    ! end if
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
                  ! Since is1 is +1 or -1 we need to flip the sign of dr in the derivative approximations
                  dra = dr(axis  )*(1-2*side)
                  dsa = dr(axisp1)*(1-2*side)
                  dta = dr(axisp2)*(1-2*side)
                  drb = dr(axis  )
                  dsb = dr(axisp1)
                  dtb = dr(axisp2)
                  ! ** Fourth-order for tau.Delta\uv=0, setting  ctlrr=ctlr=0 in the code will revert to 2nd-order
                  ctlrr=1.
                  ctlr=1.
                  if( debug.gt.0 )then
                    write(*,'(" **bcCurvilinear3dOrder4: START: grid,
     & side,axis=",3i2," is1,is2,is3=",3i3," ks1,ks2,ks3=",3i3)')grid,
     & side,axis,is1,is2,is3,ks1,ks2,ks3
                  end if
                 ! ******************************************
                 ! ************Correction loop***************
                 ! ******************************************
                 ! Given an initial answer at all points we now go back and resolve for the normal component
                 ! from   div(u)=0 and (a1.Delta u).r = 0 
                 ! We use the initial guess in order to compute the mixed derivatives urs, urss, urtt
                 if( .true. )then
                 ! ** Periodic update is now done in a previous step -- this doesn't work in parallel
                 ! first do a periodic update
                 ! if( .false. .and.(boundaryCondition(0,axisp1).lt.0 .or. boundaryCondition(0,axisp2).lt.0) )then
                 !   indexRange(0,0)=gridIndexRange(0,0)
                 !   indexRange(1,0)=gridIndexRange(1,0)
                 !   indexRange(0,1)=gridIndexRange(0,1)
                 !   indexRange(1,1)=gridIndexRange(1,1)
                 !   indexRange(0,2)=gridIndexRange(0,2)
                 !   indexRange(1,2)=gridIndexRange(1,2)
                 !
                 !   isPeriodic(0)=0
                 !   isPeriodic(1)=0
                 !   isPeriodic(2)=0
                 !   if( boundaryCondition(0,axisp1).lt.0 )then
                 !     indexRange(1,axisp1)=gridIndexRange(1,axisp1)-1
                 !     isPeriodic(axisp1)=1  
                 !   end if
                 !   if( boundaryCondition(0,axisp2).lt.0 )then
                 !     indexRange(1,axisp2)=gridIndexRange(1,axisp2)-1
                 !     isPeriodic(axisp2)=1  
                 !   end if
                 !
                 !  write(*,'(" *********** call periodic update grid,side,axis=",3i4)') grid,side,axis
                 !
                 !  call periodicUpdateMaxwell(nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,!     u,ex,ez, indexRange, gridIndexRange, dimension, isPeriodic )
                 ! end if
                  do i3=n3a,n3b
                  do i2=n2a,n2b
                  do i1=n1a,n1b
                  if( mask(i1,i2,i3).gt.0 )then
                   ! precompute the inverse of the jacobian, used in macros AmnD3J
                   i10=i1  ! used by jac3di in macros
                   i20=i2
                   i30=i3
                   do m3=-2,2
                   do m2=-2,2
                   do m1=-2,2
                    jac3di(m1,m2,m3)=1./(rx(i1+m1,i2+m2,i3+m3)*(sy(i1+
     & m1,i2+m2,i3+m3)*tz(i1+m1,i2+m2,i3+m3)-sz(i1+m1,i2+m2,i3+m3)*ty(
     & i1+m1,i2+m2,i3+m3))+ry(i1+m1,i2+m2,i3+m3)*(sz(i1+m1,i2+m2,i3+
     & m3)*tx(i1+m1,i2+m2,i3+m3)-sx(i1+m1,i2+m2,i3+m3)*tz(i1+m1,i2+m2,
     & i3+m3))+rz(i1+m1,i2+m2,i3+m3)*(sx(i1+m1,i2+m2,i3+m3)*ty(i1+m1,
     & i2+m2,i3+m3)-sy(i1+m1,i2+m2,i3+m3)*tx(i1+m1,i2+m2,i3+m3)))
                   end do
                   end do
                   end do
                   a11 =(rsxy(i1,i2,i3,axis,0)*jac3di(i1-i10,i2-i20,i3-
     & i30))
                   a12 =(rsxy(i1,i2,i3,axis,1)*jac3di(i1-i10,i2-i20,i3-
     & i30))
                   a13 =(rsxy(i1,i2,i3,axis,2)*jac3di(i1-i10,i2-i20,i3-
     & i30))
                   a21 =(rsxy(i1,i2,i3,axisp1,0)*jac3di(i1-i10,i2-i20,
     & i3-i30))
                   a22 =(rsxy(i1,i2,i3,axisp1,1)*jac3di(i1-i10,i2-i20,
     & i3-i30))
                   a23 =(rsxy(i1,i2,i3,axisp1,2)*jac3di(i1-i10,i2-i20,
     & i3-i30))
                   a31 =(rsxy(i1,i2,i3,axisp2,0)*jac3di(i1-i10,i2-i20,
     & i3-i30))
                   a32 =(rsxy(i1,i2,i3,axisp2,1)*jac3di(i1-i10,i2-i20,
     & i3-i30))
                   a33 =(rsxy(i1,i2,i3,axisp2,2)*jac3di(i1-i10,i2-i20,
     & i3-i30))
                   a11m1 =(rsxy(i1-is1,i2-is2,i3-is3,axis,0)*jac3di(i1-
     & is1-i10,i2-is2-i20,i3-is3-i30))
                   a12m1 =(rsxy(i1-is1,i2-is2,i3-is3,axis,1)*jac3di(i1-
     & is1-i10,i2-is2-i20,i3-is3-i30))
                   a13m1 =(rsxy(i1-is1,i2-is2,i3-is3,axis,2)*jac3di(i1-
     & is1-i10,i2-is2-i20,i3-is3-i30))
                   a11p1 =(rsxy(i1+is1,i2+is2,i3+is3,axis,0)*jac3di(i1+
     & is1-i10,i2+is2-i20,i3+is3-i30))
                   a12p1 =(rsxy(i1+is1,i2+is2,i3+is3,axis,1)*jac3di(i1+
     & is1-i10,i2+is2-i20,i3+is3-i30))
                   a13p1 =(rsxy(i1+is1,i2+is2,i3+is3,axis,2)*jac3di(i1+
     & is1-i10,i2+is2-i20,i3+is3-i30))
                   a11m2 =(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,0)*
     & jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))
                   a12m2 =(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,1)*
     & jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))
                   a13m2 =(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,2)*
     & jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))
                   a11p2 =(rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axis,0)*
     & jac3di(i1+2*is1-i10,i2+2*is2-i20,i3+2*is3-i30))
                   a12p2 =(rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axis,1)*
     & jac3di(i1+2*is1-i10,i2+2*is2-i20,i3+2*is3-i30))
                   a13p2 =(rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axis,2)*
     & jac3di(i1+2*is1-i10,i2+2*is2-i20,i3+2*is3-i30))
                   c11 = (rsxy(i1,i2,i3,axis,0)**2+rsxy(i1,i2,i3,axis,
     & 1)**2+rsxy(i1,i2,i3,axis,2)**2)
                   c22 = (rsxy(i1,i2,i3,axisp1,0)**2+rsxy(i1,i2,i3,
     & axisp1,1)**2+rsxy(i1,i2,i3,axisp1,2)**2)
                   c33 = (rsxy(i1,i2,i3,axisp2,0)**2+rsxy(i1,i2,i3,
     & axisp2,1)**2+rsxy(i1,i2,i3,axisp2,2)**2)
                   c1 = (rsxyx43(i1,i2,i3,axis,0)+rsxyy43(i1,i2,i3,
     & axis,1)+rsxyz43(i1,i2,i3,axis,2))
                   c2 = (rsxyx43(i1,i2,i3,axisp1,0)+rsxyy43(i1,i2,i3,
     & axisp1,1)+rsxyz43(i1,i2,i3,axisp1,2))
                   c3 = (rsxyx43(i1,i2,i3,axisp2,0)+rsxyy43(i1,i2,i3,
     & axisp2,1)+rsxyz43(i1,i2,i3,axisp2,2))
                   us=(8.*(u(i1+js1,i2+js2,i3+js3,ex)-u(i1-js1,i2-js2,
     & i3-js3,ex))-(u(i1+2*js1,i2+2*js2,i3+2*js3,ex)-u(i1-2*js1,i2-2*
     & js2,i3-2*js3,ex)))/(12.*dsa)
                   uss=(-30.*u(i1,i2,i3,ex)+16.*(u(i1+js1,i2+js2,i3+
     & js3,ex)+u(i1-js1,i2-js2,i3-js3,ex))-(u(i1+2*js1,i2+2*js2,i3+2*
     & js3,ex)+u(i1-2*js1,i2-2*js2,i3-2*js3,ex)))/(12.*dsa**2)
                   vs=(8.*(u(i1+js1,i2+js2,i3+js3,ey)-u(i1-js1,i2-js2,
     & i3-js3,ey))-(u(i1+2*js1,i2+2*js2,i3+2*js3,ey)-u(i1-2*js1,i2-2*
     & js2,i3-2*js3,ey)))/(12.*dsa)
                   vss=(-30.*u(i1,i2,i3,ey)+16.*(u(i1+js1,i2+js2,i3+
     & js3,ey)+u(i1-js1,i2-js2,i3-js3,ey))-(u(i1+2*js1,i2+2*js2,i3+2*
     & js3,ey)+u(i1-2*js1,i2-2*js2,i3-2*js3,ey)))/(12.*dsa**2)
                   ws=(8.*(u(i1+js1,i2+js2,i3+js3,ez)-u(i1-js1,i2-js2,
     & i3-js3,ez))-(u(i1+2*js1,i2+2*js2,i3+2*js3,ez)-u(i1-2*js1,i2-2*
     & js2,i3-2*js3,ez)))/(12.*dsa)
                   wss=(-30.*u(i1,i2,i3,ez)+16.*(u(i1+js1,i2+js2,i3+
     & js3,ez)+u(i1-js1,i2-js2,i3-js3,ez))-(u(i1+2*js1,i2+2*js2,i3+2*
     & js3,ez)+u(i1-2*js1,i2-2*js2,i3-2*js3,ez)))/(12.*dsa**2)
                   ut=(8.*(u(i1+ks1,i2+ks2,i3+ks3,ex)-u(i1-ks1,i2-ks2,
     & i3-ks3,ex))-(u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ex)-u(i1-2*ks1,i2-2*
     & ks2,i3-2*ks3,ex)))/(12.*dta)
                   utt=(-30.*u(i1,i2,i3,ex)+16.*(u(i1+ks1,i2+ks2,i3+
     & ks3,ex)+u(i1-ks1,i2-ks2,i3-ks3,ex))-(u(i1+2*ks1,i2+2*ks2,i3+2*
     & ks3,ex)+u(i1-2*ks1,i2-2*ks2,i3-2*ks3,ex)))/(12.*dta**2)
                   vt=(8.*(u(i1+ks1,i2+ks2,i3+ks3,ey)-u(i1-ks1,i2-ks2,
     & i3-ks3,ey))-(u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ey)-u(i1-2*ks1,i2-2*
     & ks2,i3-2*ks3,ey)))/(12.*dta)
                   vtt=(-30.*u(i1,i2,i3,ey)+16.*(u(i1+ks1,i2+ks2,i3+
     & ks3,ey)+u(i1-ks1,i2-ks2,i3-ks3,ey))-(u(i1+2*ks1,i2+2*ks2,i3+2*
     & ks3,ey)+u(i1-2*ks1,i2-2*ks2,i3-2*ks3,ey)))/(12.*dta**2)
                   wt=(8.*(u(i1+ks1,i2+ks2,i3+ks3,ez)-u(i1-ks1,i2-ks2,
     & i3-ks3,ez))-(u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ez)-u(i1-2*ks1,i2-2*
     & ks2,i3-2*ks3,ez)))/(12.*dta)
                   wtt=(-30.*u(i1,i2,i3,ez)+16.*(u(i1+ks1,i2+ks2,i3+
     & ks3,ez)+u(i1-ks1,i2-ks2,i3-ks3,ez))-(u(i1+2*ks1,i2+2*ks2,i3+2*
     & ks3,ez)+u(i1-2*ks1,i2-2*ks2,i3-2*ks3,ez)))/(12.*dta**2)
                  tau11=rsxy(i1,i2,i3,axisp1,0)
                  tau12=rsxy(i1,i2,i3,axisp1,1)
                  tau13=rsxy(i1,i2,i3,axisp1,2)
                  tau21=rsxy(i1,i2,i3,axisp2,0)
                  tau22=rsxy(i1,i2,i3,axisp2,1)
                  tau23=rsxy(i1,i2,i3,axisp2,2)
                  uex=u(i1,i2,i3,ex)
                  uey=u(i1,i2,i3,ey)
                  uez=u(i1,i2,i3,ez)
                  a11r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axis,0)*
     & jac3di(i1+is1-i10,i2+is2-i20,i3+is3-i30))-(rsxy(i1-is1,i2-is2,
     & i3-is3,axis,0)*jac3di(i1-is1-i10,i2-is2-i20,i3-is3-i30)))-((
     & rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axis,0)*jac3di(i1+2*is1-i10,i2+
     & 2*is2-i20,i3+2*is3-i30))-(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,
     & 0)*jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))))/(12.*dra)
                  a12r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axis,1)*
     & jac3di(i1+is1-i10,i2+is2-i20,i3+is3-i30))-(rsxy(i1-is1,i2-is2,
     & i3-is3,axis,1)*jac3di(i1-is1-i10,i2-is2-i20,i3-is3-i30)))-((
     & rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axis,1)*jac3di(i1+2*is1-i10,i2+
     & 2*is2-i20,i3+2*is3-i30))-(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,
     & 1)*jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))))/(12.*dra)
                  a13r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axis,2)*
     & jac3di(i1+is1-i10,i2+is2-i20,i3+is3-i30))-(rsxy(i1-is1,i2-is2,
     & i3-is3,axis,2)*jac3di(i1-is1-i10,i2-is2-i20,i3-is3-i30)))-((
     & rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axis,2)*jac3di(i1+2*is1-i10,i2+
     & 2*is2-i20,i3+2*is3-i30))-(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,
     & 2)*jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))))/(12.*dra)
                  if( axis.eq.0 )then
                    urs  =  urs4(i1,i2,i3,ex)
                    urt  =  urt4(i1,i2,i3,ex)
                    urss = urss4(i1,i2,i3,ex)
                    urtt = urtt4(i1,i2,i3,ex)
                    vrs  =  urs4(i1,i2,i3,ey)
                    vrt  =  urt4(i1,i2,i3,ey)
                    vrss = urss4(i1,i2,i3,ey)
                    vrtt = urtt4(i1,i2,i3,ey)
                    wrs  =  urs4(i1,i2,i3,ez)
                    wrt  =  urt4(i1,i2,i3,ez)
                    wrss = urss4(i1,i2,i3,ez)
                    wrtt = urtt4(i1,i2,i3,ez)
                    c11r = (2.*(rsxy(i1,i2,i3,axis,0)*rsxyr4(i1,i2,i3,
     & axis,0)+rsxy(i1,i2,i3,axis,1)*rsxyr4(i1,i2,i3,axis,1)+rsxy(i1,
     & i2,i3,axis,2)*rsxyr4(i1,i2,i3,axis,2)))
                    c22r = (2.*(rsxy(i1,i2,i3,axisp1,0)*rsxyr4(i1,i2,
     & i3,axisp1,0)+rsxy(i1,i2,i3,axisp1,1)*rsxyr4(i1,i2,i3,axisp1,1)+
     & rsxy(i1,i2,i3,axisp1,2)*rsxyr4(i1,i2,i3,axisp1,2)))
                    c33r = (2.*(rsxy(i1,i2,i3,axisp2,0)*rsxyr4(i1,i2,
     & i3,axisp2,0)+rsxy(i1,i2,i3,axisp2,1)*rsxyr4(i1,i2,i3,axisp2,1)+
     & rsxy(i1,i2,i3,axisp2,2)*rsxyr4(i1,i2,i3,axisp2,2)))
                    c1r = (rsxyxr43(i1,i2,i3,axis,0)+rsxyyr43(i1,i2,i3,
     & axis,1)+rsxyzr43(i1,i2,i3,axis,2))
                    c2r = (rsxyxr43(i1,i2,i3,axisp1,0)+rsxyyr43(i1,i2,
     & i3,axisp1,1)+rsxyzr43(i1,i2,i3,axisp1,2))
                    c3r = (rsxyxr43(i1,i2,i3,axisp2,0)+rsxyyr43(i1,i2,
     & i3,axisp2,1)+rsxyzr43(i1,i2,i3,axisp2,2))
                  else if( axis.eq.1 )then
                    urs  =  ust4(i1,i2,i3,ex)
                    urt  =  urs4(i1,i2,i3,ex)
                    urss = ustt4(i1,i2,i3,ex)
                    urtt = urrs4(i1,i2,i3,ex)
                    vrs  =  ust4(i1,i2,i3,ey)
                    vrt  =  urs4(i1,i2,i3,ey)
                    vrss = ustt4(i1,i2,i3,ey)
                    vrtt = urrs4(i1,i2,i3,ey)
                    wrs  =  ust4(i1,i2,i3,ez)
                    wrt  =  urs4(i1,i2,i3,ez)
                    wrss = ustt4(i1,i2,i3,ez)
                    wrtt = urrs4(i1,i2,i3,ez)
                    c11r = (2.*(rsxy(i1,i2,i3,axis,0)*rsxys4(i1,i2,i3,
     & axis,0)+rsxy(i1,i2,i3,axis,1)*rsxys4(i1,i2,i3,axis,1)+rsxy(i1,
     & i2,i3,axis,2)*rsxys4(i1,i2,i3,axis,2)))
                    c22r = (2.*(rsxy(i1,i2,i3,axisp1,0)*rsxys4(i1,i2,
     & i3,axisp1,0)+rsxy(i1,i2,i3,axisp1,1)*rsxys4(i1,i2,i3,axisp1,1)+
     & rsxy(i1,i2,i3,axisp1,2)*rsxys4(i1,i2,i3,axisp1,2)))
                    c33r = (2.*(rsxy(i1,i2,i3,axisp2,0)*rsxys4(i1,i2,
     & i3,axisp2,0)+rsxy(i1,i2,i3,axisp2,1)*rsxys4(i1,i2,i3,axisp2,1)+
     & rsxy(i1,i2,i3,axisp2,2)*rsxys4(i1,i2,i3,axisp2,2)))
                    c1r = (rsxyxs43(i1,i2,i3,axis,0)+rsxyys43(i1,i2,i3,
     & axis,1)+rsxyzs43(i1,i2,i3,axis,2))
                    c2r = (rsxyxs43(i1,i2,i3,axisp1,0)+rsxyys43(i1,i2,
     & i3,axisp1,1)+rsxyzs43(i1,i2,i3,axisp1,2))
                    c3r = (rsxyxs43(i1,i2,i3,axisp2,0)+rsxyys43(i1,i2,
     & i3,axisp2,1)+rsxyzs43(i1,i2,i3,axisp2,2))
                  else
                    urs  =  urt4(i1,i2,i3,ex)
                    urt  =  ust4(i1,i2,i3,ex)
                    urss = urrt4(i1,i2,i3,ex)
                    urtt = usst4(i1,i2,i3,ex)
                    vrs  =  urt4(i1,i2,i3,ey)
                    vrt  =  ust4(i1,i2,i3,ey)
                    vrss = urrt4(i1,i2,i3,ey)
                    vrtt = usst4(i1,i2,i3,ey)
                    wrs  =  urt4(i1,i2,i3,ez)
                    wrt  =  ust4(i1,i2,i3,ez)
                    wrss = urrt4(i1,i2,i3,ez)
                    wrtt = usst4(i1,i2,i3,ez)
                    c11r = (2.*(rsxy(i1,i2,i3,axis,0)*rsxyt4(i1,i2,i3,
     & axis,0)+rsxy(i1,i2,i3,axis,1)*rsxyt4(i1,i2,i3,axis,1)+rsxy(i1,
     & i2,i3,axis,2)*rsxyt4(i1,i2,i3,axis,2)))
                    c22r = (2.*(rsxy(i1,i2,i3,axisp1,0)*rsxyt4(i1,i2,
     & i3,axisp1,0)+rsxy(i1,i2,i3,axisp1,1)*rsxyt4(i1,i2,i3,axisp1,1)+
     & rsxy(i1,i2,i3,axisp1,2)*rsxyt4(i1,i2,i3,axisp1,2)))
                    c33r = (2.*(rsxy(i1,i2,i3,axisp2,0)*rsxyt4(i1,i2,
     & i3,axisp2,0)+rsxy(i1,i2,i3,axisp2,1)*rsxyt4(i1,i2,i3,axisp2,1)+
     & rsxy(i1,i2,i3,axisp2,2)*rsxyt4(i1,i2,i3,axisp2,2)))
                    c1r = (rsxyxt43(i1,i2,i3,axis,0)+rsxyyt43(i1,i2,i3,
     & axis,1)+rsxyzt43(i1,i2,i3,axis,2))
                    c2r = (rsxyxt43(i1,i2,i3,axisp1,0)+rsxyyt43(i1,i2,
     & i3,axisp1,1)+rsxyzt43(i1,i2,i3,axisp1,2))
                    c3r = (rsxyxt43(i1,i2,i3,axisp2,0)+rsxyyt43(i1,i2,
     & i3,axisp2,1)+rsxyzt43(i1,i2,i3,axisp2,2))
                  end if
                  Da1DotU=0.
                  ! bf = RHS to (a1.Delta u).r =0 
                  ! Here are the terms that remain after we eliminate the urrr, urr and ur terms
                  bf = a11*( c22*urss + c22r*uss + c2*urs + c2r*us + 
     & c33*urtt + c33r*utt + c3*urt + c3r*ut ) +a12*( c22*vrss + c22r*
     & vss + c2*vrs + c2r*vs + c33*vrtt + c33r*vtt + c3*vrt + c3r*vt )
     &  +a13*( c22*wrss + c22r*wss + c2*wrs + c2r*ws + c33*wrtt + 
     & c33r*wtt + c3*wrt + c3r*wt ) +a11r*( c22*uss + c2*us + c33*utt 
     & + c3*ut ) +a12r*( c22*vss + c2*vs + c33*vtt + c3*vt ) +a13r*( 
     & c22*wss + c2*ws + c33*wtt + c3*wt )
                  if( forcingOption.eq.planeWaveBoundaryForcing )then
                    ! In the plane wave forcing case we subtract out a plane wave incident field
                    a21s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axisp1,0)*
     & jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))-(rsxy(i1-js1,i2-js2,
     & i3-js3,axisp1,0)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,0)*jac3di(i1+2*js1-i10,
     & i2+2*js2-i20,i3+2*js3-i30))-(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,
     & axisp1,0)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))/(
     & 12.*dsa)
                    a22s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axisp1,1)*
     & jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))-(rsxy(i1-js1,i2-js2,
     & i3-js3,axisp1,1)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,1)*jac3di(i1+2*js1-i10,
     & i2+2*js2-i20,i3+2*js3-i30))-(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,
     & axisp1,1)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))/(
     & 12.*dsa)
                    a23s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axisp1,2)*
     & jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))-(rsxy(i1-js1,i2-js2,
     & i3-js3,axisp1,2)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,2)*jac3di(i1+2*js1-i10,
     & i2+2*js2-i20,i3+2*js3-i30))-(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,
     & axisp1,2)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))/(
     & 12.*dsa)
                    a31t = (8.*((rsxy(i1+ks1,i2+ks2,i3+ks3,axisp2,0)*
     & jac3di(i1+ks1-i10,i2+ks2-i20,i3+ks3-i30))-(rsxy(i1-ks1,i2-ks2,
     & i3-ks3,axisp2,0)*jac3di(i1-ks1-i10,i2-ks2-i20,i3-ks3-i30)))-((
     & rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axisp2,0)*jac3di(i1+2*ks1-i10,
     & i2+2*ks2-i20,i3+2*ks3-i30))-(rsxy(i1-2*ks1,i2-2*ks2,i3-2*ks3,
     & axisp2,0)*jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30))))/(
     & 12.*dta)
                    a32t = (8.*((rsxy(i1+ks1,i2+ks2,i3+ks3,axisp2,1)*
     & jac3di(i1+ks1-i10,i2+ks2-i20,i3+ks3-i30))-(rsxy(i1-ks1,i2-ks2,
     & i3-ks3,axisp2,1)*jac3di(i1-ks1-i10,i2-ks2-i20,i3-ks3-i30)))-((
     & rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axisp2,1)*jac3di(i1+2*ks1-i10,
     & i2+2*ks2-i20,i3+2*ks3-i30))-(rsxy(i1-2*ks1,i2-2*ks2,i3-2*ks3,
     & axisp2,1)*jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30))))/(
     & 12.*dta)
                    a33t = (8.*((rsxy(i1+ks1,i2+ks2,i3+ks3,axisp2,2)*
     & jac3di(i1+ks1-i10,i2+ks2-i20,i3+ks3-i30))-(rsxy(i1-ks1,i2-ks2,
     & i3-ks3,axisp2,2)*jac3di(i1-ks1-i10,i2-ks2-i20,i3-ks3-i30)))-((
     & rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axisp2,2)*jac3di(i1+2*ks1-i10,
     & i2+2*ks2-i20,i3+2*ks3-i30))-(rsxy(i1-2*ks1,i2-2*ks2,i3-2*ks3,
     & axisp2,2)*jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30))))/(
     & 12.*dta)
                    ! *** set RHS for (a1.u).r =  - Ds( a2.uv ) -Dt( a3.uv )
                    Da1DotU = -(  a21s*uex+a22s*uey+a23s*uez + a21*us+
     & a22*vs+a23*ws + a31t*uex+a32t*uey+a33t*uez + a31*ut+a32*vt+a33*
     & wt )
                    ! *** NOTE: "d" denotes the time derivative as in udd = two time derivatives of u
                    ! (a1.Delta u).r = - (a2.utt).s - (a3.utt).t
                    ! (a1.Delta u).r + bf = 0
                    ! bf = bf + (a2.utt).s + (a3.utt).t
                     x00=xy(i1,i2,i3,0)
                     y00=xy(i1,i2,i3,1)
                     z00=xy(i1,i2,i3,2)
                     if( fieldOption.eq.0 )then
                       udd=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x00)+
     & ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0)+ssftt*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))
                       vdd=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x00)+
     & ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1)+ssftt*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))
                       wdd=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x00)+
     & ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2)+ssftt*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))
                     else
                       ! get time derivative (sosup) 
                       udd=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x00)+ky*
     & (y00)+kz*(z00)-cc*(t)))*pwc(0))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))+3.*ssftt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0)
     & +ssfttt*sin(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))
                       vdd=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x00)+ky*
     & (y00)+kz*(z00)-cc*(t)))*pwc(1))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))+3.*ssftt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1)
     & +ssfttt*sin(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))
                       wdd=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x00)+ky*
     & (y00)+kz*(z00)-cc*(t)))*pwc(2))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))+3.*ssftt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2)
     & +ssfttt*sin(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))
                     end if
                     x00=xy(i1+js1,i2+js2,i3+js3,0)
                     y00=xy(i1+js1,i2+js2,i3+js3,1)
                     z00=xy(i1+js1,i2+js2,i3+js3,2)
                     if( fieldOption.eq.0 )then
                       uddp1=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x00)+
     & ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0)+ssftt*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))
                       vddp1=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x00)+
     & ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1)+ssftt*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))
                       wddp1=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x00)+
     & ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2)+ssftt*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))
                     else
                       ! get time derivative (sosup) 
                       uddp1=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x00)+
     & ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))+3.*ssftt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0)
     & +ssfttt*sin(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))
                       vddp1=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x00)+
     & ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))+3.*ssftt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1)
     & +ssfttt*sin(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))
                       wddp1=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x00)+
     & ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))+3.*ssftt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2)
     & +ssfttt*sin(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))
                     end if
                     x00=xy(i1-js1,i2-js2,i3-js3,0)
                     y00=xy(i1-js1,i2-js2,i3-js3,1)
                     z00=xy(i1-js1,i2-js2,i3-js3,2)
                     if( fieldOption.eq.0 )then
                       uddm1=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x00)+
     & ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0)+ssftt*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))
                       vddm1=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x00)+
     & ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1)+ssftt*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))
                       wddm1=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x00)+
     & ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2)+ssftt*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))
                     else
                       ! get time derivative (sosup) 
                       uddm1=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x00)+
     & ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))+3.*ssftt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0)
     & +ssfttt*sin(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))
                       vddm1=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x00)+
     & ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))+3.*ssftt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1)
     & +ssfttt*sin(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))
                       wddm1=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x00)+
     & ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))+3.*ssftt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2)
     & +ssfttt*sin(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))
                     end if
                    ! 2nd-order here should be good enough:
                    udds = (uddp1-uddm1)/(2.*dsa)
                    vdds = (vddp1-vddm1)/(2.*dsa)
                    wdds = (wddp1-wddm1)/(2.*dsa)
                     x00=xy(i1+ks1,i2+ks2,i3+ks3,0)
                     y00=xy(i1+ks1,i2+ks2,i3+ks3,1)
                     z00=xy(i1+ks1,i2+ks2,i3+ks3,2)
                     if( fieldOption.eq.0 )then
                       uddp1=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x00)+
     & ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0)+ssftt*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))
                       vddp1=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x00)+
     & ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1)+ssftt*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))
                       wddp1=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x00)+
     & ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2)+ssftt*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))
                     else
                       ! get time derivative (sosup) 
                       uddp1=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x00)+
     & ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))+3.*ssftt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0)
     & +ssfttt*sin(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))
                       vddp1=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x00)+
     & ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))+3.*ssftt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1)
     & +ssfttt*sin(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))
                       wddp1=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x00)+
     & ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))+3.*ssftt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2)
     & +ssfttt*sin(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))
                     end if
                     x00=xy(i1-ks1,i2-ks2,i3-ks3,0)
                     y00=xy(i1-ks1,i2-ks2,i3-ks3,1)
                     z00=xy(i1-ks1,i2-ks2,i3-ks3,2)
                     if( fieldOption.eq.0 )then
                       uddm1=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x00)+
     & ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0)+ssftt*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))
                       vddm1=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x00)+
     & ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1)+ssftt*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))
                       wddm1=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x00)+
     & ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2)+ssftt*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))
                     else
                       ! get time derivative (sosup) 
                       uddm1=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x00)+
     & ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))+3.*ssftt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0)
     & +ssfttt*sin(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))
                       vddm1=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x00)+
     & ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))+3.*ssftt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1)
     & +ssfttt*sin(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))
                       wddm1=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x00)+
     & ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))+3.*ssftt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2)
     & +ssfttt*sin(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))
                     end if
                    ! 2nd-order here should be good enough:
                    uddt = (uddp1-uddm1)/(2.*dta)
                    vddt = (vddp1-vddm1)/(2.*dta)
                    wddt = (wddp1-wddm1)/(2.*dta)
                    bf = bf + a21s*udd+a22s*vdd+a23s*wdd + a21*udds + 
     & a22*vdds+ a23*wdds + a31t*udd+a32t*vdd+a33t*wdd + a31*uddt + 
     & a32*vddt+ a33*wddt
                  end if
                 ! Now assign E at the ghost points:


! ************ Results from bc43d.maple *******************
      b3u=a11*c11
      b3v=a12*c11
      b3w=a13*c11
      b2u=a11*(c1+c11r)+a11r*c11
      b2v=a12*(c1+c11r)+a12r*c11
      b2w=a13*(c1+c11r)+a13r*c11
      b1u=a11*c1r+a11r*c1
      b1v=a12*c1r+a12r*c1
      b1w=a13*c1r+a13r*c1
      ttu11=tau11*u(i1-is1,i2-is2,i3-is3,ex)+tau12*u(i1-is1,i2-is2,i3-
     & is3,ey)+tau13*u(i1-is1,i2-is2,i3-is3,ez)
      ttu12=tau11*u(i1-2*is1,i2-2*is2,i3-2*is3,ex)+tau12*u(i1-2*is1,i2-
     & 2*is2,i3-2*is3,ey)+tau13*u(i1-2*is1,i2-2*is2,i3-2*is3,ez)
      ttu21=tau21*u(i1-is1,i2-is2,i3-is3,ex)+tau22*u(i1-is1,i2-is2,i3-
     & is3,ey)+tau23*u(i1-is1,i2-is2,i3-is3,ez)
      ttu22=tau21*u(i1-2*is1,i2-2*is2,i3-2*is3,ex)+tau22*u(i1-2*is1,i2-
     & 2*is2,i3-2*is3,ey)+tau23*u(i1-2*is1,i2-2*is2,i3-2*is3,ez)
      ! *********** set tangential components to be exact *****
      ! OGF3D(i1-is1,i2-is2,i3-is3,t, uvm(0),uvm(1),uvm(2))
      ! ttu11=tau11*uvm(0)+tau12*uvm(1)+tau13*uvm(2)
      ! ttu21=tau21*uvm(0)+tau22*uvm(1)+tau23*uvm(2)
      ! OGF3D(i1-2*is1,i2-2*is2,i3-2*is3,t, uvm(0),uvm(1),uvm(2))
      ! ttu12=tau11*uvm(0)+tau12*uvm(1)+tau13*uvm(2)
      ! ttu22=tau21*uvm(0)+tau22*uvm(1)+tau23*uvm(2)
      ! ******************************************************

      f1um2=-1/2.*b3u/dra**3-1/12.*b2u/dra**2+1/12.*b1u/dra
      f1um1=b3u/dra**3+4/3.*b2u/dra**2-2/3.*b1u/dra
      f1vm2=-1/2.*b3v/dra**3-1/12.*b2v/dra**2+1/12.*b1v/dra
      f1vm1=b3v/dra**3+4/3.*b2v/dra**2-2/3.*b1v/dra
      f1wm2=-1/2.*b3w/dra**3-1/12.*b2w/dra**2+1/12.*b1w/dra
      f1wm1=b3w/dra**3+4/3.*b2w/dra**2-2/3.*b1w/dra
      f1f  =-1/12.*(-6*b3u*u(i1+2*is1,i2+2*is2,i3+2*is3,ex)+12*b3u*u(
     & i1+is1,i2+is2,i3+is3,ex)-6*b3v*u(i1+2*is1,i2+2*is2,i3+2*is3,ey)
     & +12*b3v*u(i1+is1,i2+is2,i3+is3,ey)-6*b3w*u(i1+2*is1,i2+2*is2,
     & i3+2*is3,ez)+12*b3w*u(i1+is1,i2+is2,i3+is3,ez)-8*b1u*dra**2*u(
     & i1+is1,i2+is2,i3+is3,ex)-8*b1v*dra**2*u(i1+is1,i2+is2,i3+is3,
     & ey)-16*b2u*dra*u(i1+is1,i2+is2,i3+is3,ex)-12*bf*dra**3+b1w*dra*
     & *2*u(i1+2*is1,i2+2*is2,i3+2*is3,ez)-8*b1w*dra**2*u(i1+is1,i2+
     & is2,i3+is3,ez)+b1u*dra**2*u(i1+2*is1,i2+2*is2,i3+2*is3,ex)+b1v*
     & dra**2*u(i1+2*is1,i2+2*is2,i3+2*is3,ey)+b2u*dra*u(i1+2*is1,i2+
     & 2*is2,i3+2*is3,ex)+30*b2u*dra*u(i1,i2,i3,ex)+b2v*dra*u(i1+2*
     & is1,i2+2*is2,i3+2*is3,ey)+30*b2v*dra*u(i1,i2,i3,ey)-16*b2v*dra*
     & u(i1+is1,i2+is2,i3+is3,ey)+b2w*dra*u(i1+2*is1,i2+2*is2,i3+2*
     & is3,ez)+30*b2w*dra*u(i1,i2,i3,ez)-16*b2w*dra*u(i1+is1,i2+is2,
     & i3+is3,ez))/dra**3

      f2um2=1/12.*a11m2
      f2um1=-2/3.*a11m1
      f2vm2=1/12.*a12m2
      f2vm1=-2/3.*a12m1
      f2wm2=1/12.*a13m2
      f2wm1=-2/3.*a13m1
      f2f  =2/3.*a11p1*u(i1+is1,i2+is2,i3+is3,ex)+2/3.*a12p1*u(i1+is1,
     & i2+is2,i3+is3,ey)+2/3.*a13p1*u(i1+is1,i2+is2,i3+is3,ez)-1/12.*
     & a11p2*u(i1+2*is1,i2+2*is2,i3+2*is3,ex)-1/12.*a12p2*u(i1+2*is1,
     & i2+2*is2,i3+2*is3,ey)-1/12.*a13p2*u(i1+2*is1,i2+2*is2,i3+2*is3,
     & ez)-Da1DotU*dra

      u(i1-is1,i2-is2,i3-is3,ex) = (-2*tau13*f1f*tau22*f2um2*tau23*
     & tau12-tau13**2*f1f*tau22*f2vm2*tau21-tau13**2*f1um2*tau22**2*
     & f2f-f1um2*tau23**2*tau12**2*f2f+f1vm2*tau23**2*ttu12*f2um2*
     & tau12-f1vm2*tau23*ttu12*tau13*f2um2*tau22-f1vm2*tau23*ttu12*
     & f2wm2*tau12*tau21+f1wm1*tau12*ttu21*tau13*f2vm2*tau21-f1wm1*
     & tau12*ttu21*tau13*f2um2*tau22-f1wm1*tau12**2*ttu21*f2wm2*tau21+
     & f1wm2*tau12**2*ttu22*f2um2*tau23-f1wm2*tau12*ttu22*f2vm2*tau23*
     & tau11-f1wm2*tau12*ttu22*tau13*f2um2*tau22+f1wm1*tau12*ttu21*
     & f2wm2*tau11*tau22+f1wm1*tau12**2*ttu21*f2um2*tau23+tau13*ttu21*
     & f1vm1*f2wm2*tau12*tau21+f1vm1*tau23*ttu11*f2wm2*tau11*tau22+
     & f1vm1*tau23**2*ttu11*f2um2*tau12-f1vm1*tau23**2*ttu11*f2vm2*
     & tau11+f1vm1*tau23*ttu11*tau13*f2vm2*tau21-f1vm1*tau23*ttu11*
     & tau13*f2um2*tau22-f1vm1*tau23*ttu11*f2wm2*tau12*tau21-f1wm1*
     & ttu11*tau22**2*f2wm2*tau11-f1wm1*ttu11*tau22*f2um2*tau23*tau12+
     & f1wm1*ttu11*tau22*f2vm2*tau23*tau11-f1wm1*ttu11*tau22*tau13*
     & f2vm2*tau21+f1wm1*ttu11*tau22**2*tau13*f2um2+f1wm1*ttu11*tau22*
     & f2wm2*tau12*tau21+f1f*tau23*tau12*f2wm2*tau11*tau22-f1f*tau23**
     & 2*tau12*f2vm2*tau11+f1f*tau23*tau12*tau13*f2vm2*tau21-f1f*
     & tau23*tau12**2*f2wm2*tau21-f1wm2*ttu12*tau22*f2um2*tau23*tau12-
     & f1wm2*ttu12*tau22*tau13*f2vm2*tau21+f1wm2*ttu12*tau22**2*tau13*
     & f2um2+tau13**2*ttu21*f1vm1*f2um2*tau22-f1wm1*tau12*ttu21*f2vm2*
     & tau23*tau11-tau13*f1um2*tau22**2*f2wm1*ttu11+tau13*f1um2*tau22*
     & f2vm1*tau23*ttu11-tau13**2*f1um2*tau22*f2vm2*ttu22-tau13**2*
     & f1um2*tau22*ttu21*f2vm1+tau13*f1um2*tau22*f2vm2*tau23*ttu12+
     & tau13*f1um2*tau22*f2wm2*tau12*ttu22+2*tau13*f1um2*tau22*f2f*
     & tau23*tau12-tau13*f1um2*tau22**2*f2wm2*ttu12+tau13*f1um2*tau22*
     & f2wm1*tau12*ttu21+tau13*f1vm2*tau21*f2wm1*ttu11*tau22-tau13*
     & f1vm2*tau21*f2vm1*tau23*ttu11+tau13**2*f1vm2*tau21*f2f*tau22+
     & tau13**2*f1vm2*tau21*ttu21*f2vm1-tau13*f1vm2*tau21*f2f*tau23*
     & tau12+tau13*f1vm2*tau21*f2wm2*ttu12*tau22-tau13*f1vm2*tau21*
     & f2wm1*tau12*ttu21+tau13*f1f*tau22*f2wm2*tau12*tau21-tau13*
     & f1vm2*ttu22*f2wm2*tau11*tau22-tau13*f1vm2*ttu22*f2um2*tau23*
     & tau12-tau13*f1f*tau22**2*f2wm2*tau11+tau13**2*f1vm2*ttu22*
     & f2um2*tau22-tau13*ttu21*f1vm1*f2wm2*tau11*tau22-tau13*ttu21*
     & f1vm1*f2um2*tau23*tau12+tau13*ttu21*f1vm1*f2vm2*tau23*tau11-
     & tau13**2*ttu21*f1vm1*f2vm2*tau21+f1f*tau23**2*tau12**2*f2um2+
     & tau13**2*f1f*tau22**2*f2um2+tau13*f1f*tau22*f2vm2*tau23*tau11-
     & f1wm2*tau11*tau22*f2vm1*tau23*ttu11+f1wm2*tau11*tau22*tau13*
     & f2vm2*ttu22+f1wm2*tau11*tau22**2*tau13*f2f+f1wm2*tau11*tau22*
     & tau13*ttu21*f2vm1-f1wm2*tau11*tau22*f2f*tau23*tau12-f1wm2*
     & tau11*tau22*f2wm1*tau12*ttu21-f1wm2*tau12*tau21*f2wm1*ttu11*
     & tau22+f1wm2*tau12*tau21*f2vm1*tau23*ttu11-f1wm2*tau12*tau21*
     & tau13*f2f*tau22-f1wm2*tau12*tau21*tau13*ttu21*f2vm1+f1wm2*
     & tau12*tau21*f2vm2*tau23*ttu12+f1wm2*tau12**2*tau21*f2f*tau23+
     & f1wm2*tau12**2*tau21*f2wm1*ttu21-f1vm2*tau23*tau11*tau13*f2f*
     & tau22-f1vm2*tau23*tau11*tau13*ttu21*f2vm1+f1vm2*tau23*tau11*
     & f2wm2*tau12*ttu22+f1vm2*tau23**2*tau11*f2f*tau12+f1vm2*tau23*
     & tau11*f2wm1*tau12*ttu21+f1wm2*tau11*tau22**2*f2wm1*ttu11+f1um2*
     & tau23*tau12*f2wm1*ttu11*tau22-f1um2*tau23**2*tau12*f2vm1*ttu11+
     & f1um2*tau23*tau12*tau13*f2vm2*ttu22+f1um2*tau23*tau12*tau13*
     & ttu21*f2vm1-f1um2*tau23**2*tau12*f2vm2*ttu12-f1um2*tau23*tau12*
     & *2*f2wm2*ttu22+f1um2*tau23*tau12*f2wm2*ttu12*tau22-f1um2*tau23*
     & tau12**2*f2wm1*ttu21-f1vm2*tau23*tau11*f2wm1*ttu11*tau22+f1vm2*
     & tau23**2*tau11*f2vm1*ttu11)/(-f1um1*tau23**2*tau12**2*f2um2-
     & f1wm2*tau12**2*tau21*f2um1*tau23-f1vm2*tau23*tau11**2*f2wm1*
     & tau22+f1vm2*tau23*tau11*tau13*f2um1*tau22-f1wm2*tau11*tau22**2*
     & tau13*f2um1+f1vm2*tau23*tau11*f2wm1*tau12*tau21-f1vm2*tau23**2*
     & tau11*f2um1*tau12+f1um2*tau23*tau12*tau13*tau21*f2vm1+f1um2*
     & tau23*tau12*f2wm1*tau11*tau22-f1um2*tau23**2*tau12*f2vm1*tau11-
     & f1um2*tau23*tau12**2*f2wm1*tau21+f1um2*tau23**2*tau12**2*f2um1-
     & 2*f1wm2*tau11*tau22*f2wm1*tau12*tau21+f1wm2*tau11*tau22*f2um1*
     & tau23*tau12+f1wm2*tau12*tau21*tau13*f2um1*tau22-f1wm2*tau12*
     & tau21**2*tau13*f2vm1+f1wm2*tau12*tau21*f2vm1*tau23*tau11+f1wm2*
     & tau12**2*tau21**2*f2wm1+f1vm2*tau23**2*tau11**2*f2vm1+tau13*
     & f1um2*tau22*f2vm1*tau23*tau11+tau13*f1um2*tau22*f2wm1*tau12*
     & tau21-2*tau13*f1um2*tau22*f2um1*tau23*tau12-tau13**2*f1vm2*
     & tau21*f2um1*tau22+tau13**2*tau21*f1vm1*f2um2*tau22+tau13*tau21*
     & *2*f1vm1*f2wm2*tau12+f1wm2*tau11*tau22*tau13*tau21*f2vm1+f1wm2*
     & tau11**2*tau22**2*f2wm1-f1wm2*tau11**2*tau22*f2vm1*tau23+tau13*
     & f1vm2*tau21*f2wm1*tau11*tau22-2*tau13*f1vm2*tau21*f2vm1*tau23*
     & tau11-tau13*f1vm2*tau21**2*f2wm1*tau12+tau13*f1vm2*tau21*f2um1*
     & tau23*tau12+tau13*f1um1*tau22**2*f2wm2*tau11+2*tau13*f1um1*
     & tau22*f2um2*tau23*tau12-tau13*f1um1*tau22*f2vm2*tau23*tau11+
     & tau13**2*f1um1*tau22*f2vm2*tau21-tau13**2*f1um1*tau22**2*f2um2-
     & tau13*f1um1*tau22*f2wm2*tau12*tau21-tau13*tau21*f1vm1*f2wm2*
     & tau11*tau22-tau13*tau21*f1vm1*f2um2*tau23*tau12+2*tau13*tau21*
     & f1vm1*f2vm2*tau23*tau11-tau13**2*tau21**2*f1vm1*f2vm2-f1vm1*
     & tau23**2*tau11**2*f2vm2-f1vm1*tau23*tau11*tau13*f2um2*tau22-
     & f1vm1*tau23*tau11*f2wm2*tau12*tau21+tau13**2*f1um2*tau22**2*
     & f2um1+tau13**2*f1vm2*tau21**2*f2vm1+f1wm1*tau12*tau21**2*tau13*
     & f2vm2-f1wm1*tau12*tau21*tau13*f2um2*tau22+2*f1wm1*tau12*tau21*
     & f2wm2*tau11*tau22+f1wm1*tau12**2*tau21*f2um2*tau23-tau13**2*
     & f1um2*tau22*tau21*f2vm1-f1um1*tau23*tau12*f2wm2*tau11*tau22-
     & f1um1*tau23*tau12*tau13*f2vm2*tau21+f1um1*tau23*tau12**2*f2wm2*
     & tau21+f1wm1*tau11**2*tau22*f2vm2*tau23-f1wm1*tau11**2*tau22**2*
     & f2wm2+f1um1*tau23**2*tau12*f2vm2*tau11-f1wm1*tau11*tau22*f2um2*
     & tau23*tau12+f1vm1*tau23**2*tau11*f2um2*tau12-f1wm1*tau11*tau22*
     & tau13*f2vm2*tau21+f1wm1*tau11*tau22**2*tau13*f2um2-f1wm1*tau12*
     & tau21*f2vm2*tau23*tau11-f1wm1*tau12**2*tau21**2*f2wm2+f1vm1*
     & tau23*tau11**2*f2wm2*tau22-tau13*f1um2*tau22**2*f2wm1*tau11)

      u(i1-2*is1,i2-2*is2,i3-2*is3,ex) = (tau21*tau13*ttu22*f1vm1*
     & f2wm2*tau12+tau13*f2um1*tau22*f1wm1*tau12*ttu21+tau13*ttu22*
     & f1wm2*tau12*f2um1*tau22-tau13*f2vm1*tau23*ttu11*f1um1*tau22+
     & tau13*ttu22*f1vm2*f2um1*tau23*tau12+tau13*ttu22*f1vm2*f2wm1*
     & tau11*tau22-tau13*ttu22*f1vm2*f2vm1*tau23*tau11+2*tau13*f2um1*
     & tau23*tau12*f1f*tau22+tau13*f2f*tau22*f1vm1*tau23*tau11+tau13*
     & f2um1*tau22*f1vm1*tau23*ttu11-2*tau13*f2f*tau23*tau12*f1um1*
     & tau22-tau13*f2vm1*tau23*tau11*f1f*tau22+tau13*f2wm1*tau11*
     & tau22*ttu21*f1vm1+tau13*f2wm2*ttu12*tau22**2*f1um1-tau13*f2um1*
     & tau22**2*f1wm1*ttu11-tau13*ttu21*f2vm1*f1um1*tau23*tau12-tau13*
     & f2f*tau22**2*f1wm1*tau11-tau13*f2vm2*tau23*ttu12*f1um1*tau22+
     & tau13**2*ttu22*f1um1*tau22*f2vm2+tau13**2*ttu21*f2vm1*f1um1*
     & tau22+tau13**2*f2f*tau22**2*f1um1-tau13**2*ttu22*f1vm2*f2um1*
     & tau22-tau13**2*f2um1*tau22*ttu21*f1vm1-tau13*ttu22*f1um1*tau22*
     & f2wm2*tau12+tau23**2*ttu12*f1vm2*f2vm1*tau11-tau23**2*ttu12*
     & f1vm2*f2um1*tau12+f2wm1*tau12**2*ttu21*f1um1*tau23-f2wm2*tau12*
     & tau23*ttu12*f1um1*tau22+f2vm1*tau23*tau11*f1wm1*tau12*ttu21+
     & f2vm2*tau23*ttu12*f1wm1*tau11*tau22-f2vm2*tau23**2*ttu12*f1vm1*
     & tau11+f2vm2*tau23**2*ttu12*f1um1*tau12-f2vm1*tau23*tau11*f1wm2*
     & ttu12*tau22+f2vm1*tau23**2*tau11*f1f*tau12+f2um1*tau23*tau12*
     & f1wm1*ttu11*tau22-f2um1*tau23**2*tau12*f1vm1*ttu11-f2wm1*tau12*
     & f1wm2*tau11*tau22*ttu22-f2um1*tau23**2*tau12**2*f1f-tau13**2*
     & f2um1*tau22**2*f1f+f2vm1*tau23**2*ttu11*f1um1*tau12-f2um1*
     & tau23*tau12**2*f1wm1*ttu21-ttu22*f1wm2*tau12**2*f2um1*tau23+
     & f2um1*tau23*tau12*f1wm2*ttu12*tau22+ttu22*f1wm1*tau12*f2wm2*
     & tau11*tau22-tau23*ttu12*f1vm2*f2wm1*tau11*tau22+f2vm1*f1wm2*
     & tau12*ttu22*tau23*tau11-f2wm1*tau12*f1f*tau22*tau23*tau11+f2f*
     & tau22*f1wm1*tau12*tau23*tau11-f2wm1*ttu11*tau22*f1um1*tau23*
     & tau12-f2f*tau23**2*tau12*f1vm1*tau11+f2wm2*ttu12*tau22*f1vm1*
     & tau23*tau11-ttu22*f1vm1*tau23*tau11*f2wm2*tau12-f2wm1*tau12*
     & ttu21*f1vm1*tau23*tau11+f2wm1*tau11*tau22**2*f1wm2*ttu12+ttu22*
     & f1um1*tau23*tau12**2*f2wm2-f2wm2*ttu12*tau22**2*f1wm1*tau11+
     & tau13*f2vm2*ttu22*f1vm1*tau23*tau11-tau13*ttu21*f2vm1*f1wm1*
     & tau11*tau22+tau13*f2um1*tau23*tau12*ttu21*f1vm1-tau13*f2vm2*
     & ttu22*f1wm1*tau11*tau22-tau13*f2vm2*ttu22*f1um1*tau23*tau12+
     & tau13*tau23*ttu12*f1vm2*f2um1*tau22-tau13*f2wm1*tau12*ttu21*
     & f1um1*tau22+tau13*f2wm1*ttu11*tau22**2*f1um1+tau13*f2wm1*tau11*
     & tau22**2*f1f-tau13*f2um1*tau22**2*f1wm2*ttu12-tau21*ttu22*
     & f1wm1*tau12**2*f2wm2+tau21*ttu22*f1wm2*tau12**2*f2wm1+tau21*
     & f2wm1*tau12**2*f1f*tau23+f2f*tau23**2*tau12**2*f1um1+tau21*
     & tau13*f2vm1*f1wm2*ttu12*tau22+tau21*tau13*f2vm2*ttu22*f1wm1*
     & tau12-tau21*tau13*f2wm1*ttu11*tau22*f1vm1-tau21*tau13*f2wm1*
     & tau12*f1f*tau22+tau21*tau13*f2vm1*f1wm1*ttu11*tau22+tau21*
     & tau13*f2f*tau22*f1wm1*tau12+tau21*tau13*f2f*tau23*tau12*f1vm1-
     & tau21*tau13*ttu22*f1wm2*tau12*f2vm1-tau21*tau13*f2wm2*ttu12*
     & tau22*f1vm1-tau21*tau13*f2vm1*f1f*tau23*tau12-tau21*tau13*
     & ttu22*f1vm2*f2wm1*tau12-tau21*tau23*ttu12*f1wm1*tau12*f2vm2+
     & tau21*f2wm2*ttu12*tau22*f1wm1*tau12+tau21*tau23*ttu12*f1vm2*
     & f2wm1*tau12-tau21*f2wm1*tau12*f1wm2*ttu12*tau22+tau21*f2wm1*
     & tau12*f1vm1*tau23*ttu11+tau21*tau13*f2vm2*tau23*ttu12*f1vm1-
     & tau21*tau13**2*f2f*tau22*f1vm1+tau21*tau13**2*ttu22*f1vm2*
     & f2vm1+tau21*tau13**2*f2vm1*f1f*tau22-tau21*tau13**2*ttu22*
     & f1vm1*f2vm2-tau21*f2vm1*tau23*ttu11*f1wm1*tau12-tau21*tau13*
     & tau23*ttu12*f1vm2*f2vm1-tau21*f2f*tau23*tau12**2*f1wm1)/(-
     & f1um1*tau23**2*tau12**2*f2um2-f1wm2*tau12**2*tau21*f2um1*tau23-
     & f1vm2*tau23*tau11**2*f2wm1*tau22+f1vm2*tau23*tau11*tau13*f2um1*
     & tau22-f1wm2*tau11*tau22**2*tau13*f2um1+f1vm2*tau23*tau11*f2wm1*
     & tau12*tau21-f1vm2*tau23**2*tau11*f2um1*tau12+f1um2*tau23*tau12*
     & tau13*tau21*f2vm1+f1um2*tau23*tau12*f2wm1*tau11*tau22-f1um2*
     & tau23**2*tau12*f2vm1*tau11-f1um2*tau23*tau12**2*f2wm1*tau21+
     & f1um2*tau23**2*tau12**2*f2um1-2*f1wm2*tau11*tau22*f2wm1*tau12*
     & tau21+f1wm2*tau11*tau22*f2um1*tau23*tau12+f1wm2*tau12*tau21*
     & tau13*f2um1*tau22-f1wm2*tau12*tau21**2*tau13*f2vm1+f1wm2*tau12*
     & tau21*f2vm1*tau23*tau11+f1wm2*tau12**2*tau21**2*f2wm1+f1vm2*
     & tau23**2*tau11**2*f2vm1+tau13*f1um2*tau22*f2vm1*tau23*tau11+
     & tau13*f1um2*tau22*f2wm1*tau12*tau21-2*tau13*f1um2*tau22*f2um1*
     & tau23*tau12-tau13**2*f1vm2*tau21*f2um1*tau22+tau13**2*tau21*
     & f1vm1*f2um2*tau22+tau13*tau21**2*f1vm1*f2wm2*tau12+f1wm2*tau11*
     & tau22*tau13*tau21*f2vm1+f1wm2*tau11**2*tau22**2*f2wm1-f1wm2*
     & tau11**2*tau22*f2vm1*tau23+tau13*f1vm2*tau21*f2wm1*tau11*tau22-
     & 2*tau13*f1vm2*tau21*f2vm1*tau23*tau11-tau13*f1vm2*tau21**2*
     & f2wm1*tau12+tau13*f1vm2*tau21*f2um1*tau23*tau12+tau13*f1um1*
     & tau22**2*f2wm2*tau11+2*tau13*f1um1*tau22*f2um2*tau23*tau12-
     & tau13*f1um1*tau22*f2vm2*tau23*tau11+tau13**2*f1um1*tau22*f2vm2*
     & tau21-tau13**2*f1um1*tau22**2*f2um2-tau13*f1um1*tau22*f2wm2*
     & tau12*tau21-tau13*tau21*f1vm1*f2wm2*tau11*tau22-tau13*tau21*
     & f1vm1*f2um2*tau23*tau12+2*tau13*tau21*f1vm1*f2vm2*tau23*tau11-
     & tau13**2*tau21**2*f1vm1*f2vm2-f1vm1*tau23**2*tau11**2*f2vm2-
     & f1vm1*tau23*tau11*tau13*f2um2*tau22-f1vm1*tau23*tau11*f2wm2*
     & tau12*tau21+tau13**2*f1um2*tau22**2*f2um1+tau13**2*f1vm2*tau21*
     & *2*f2vm1+f1wm1*tau12*tau21**2*tau13*f2vm2-f1wm1*tau12*tau21*
     & tau13*f2um2*tau22+2*f1wm1*tau12*tau21*f2wm2*tau11*tau22+f1wm1*
     & tau12**2*tau21*f2um2*tau23-tau13**2*f1um2*tau22*tau21*f2vm1-
     & f1um1*tau23*tau12*f2wm2*tau11*tau22-f1um1*tau23*tau12*tau13*
     & f2vm2*tau21+f1um1*tau23*tau12**2*f2wm2*tau21+f1wm1*tau11**2*
     & tau22*f2vm2*tau23-f1wm1*tau11**2*tau22**2*f2wm2+f1um1*tau23**2*
     & tau12*f2vm2*tau11-f1wm1*tau11*tau22*f2um2*tau23*tau12+f1vm1*
     & tau23**2*tau11*f2um2*tau12-f1wm1*tau11*tau22*tau13*f2vm2*tau21+
     & f1wm1*tau11*tau22**2*tau13*f2um2-f1wm1*tau12*tau21*f2vm2*tau23*
     & tau11-f1wm1*tau12**2*tau21**2*f2wm2+f1vm1*tau23*tau11**2*f2wm2*
     & tau22-tau13*f1um2*tau22**2*f2wm1*tau11)

      u(i1-is1,i2-is2,i3-is3,ey) = (ttu21*f1wm1*tau11**2*f2vm2*tau23+
     & tau23**2*ttu11*f1um1*f2vm2*tau11-ttu21*f1vm2*tau23*tau11**2*
     & f2wm1-tau23**2*ttu11*f1um1*f2um2*tau12+tau23*tau11**2*f1wm2*
     & f2vm2*ttu22-tau23*ttu11*f1wm2*tau12*tau21*f2um1+tau21*f1vm2*
     & tau23*tau11*f2wm1*ttu11-tau23**2*ttu11*f1vm2*tau11*f2um1+tau23*
     & *2*tau11*f1um2*f2f*tau12-tau21**2*f1wm1*ttu11*f2wm2*tau12-
     & tau23*tau11**2*f1vm2*ttu22*f2wm2+tau21**2*f1wm2*tau12*f2wm1*
     & ttu11-tau23**2*tau11**2*f1vm2*f2f+tau23*ttu11*f1um1*f2wm2*
     & tau12*tau21+tau23**2*ttu11*f1um2*f2um1*tau12-tau23**2*tau11*
     & f1f*f2um2*tau12-tau23**2*tau11*f1vm2*ttu12*f2um2+tau21*f1wm1*
     & tau12*ttu21*f2wm2*tau11+tau23**2*tau11*f1um2*f2vm2*ttu12-tau23*
     & tau12*tau21*f1wm2*tau11*f2f+tau23*tau11*f1um2*f2wm2*tau12*
     & ttu22+tau13*tau21**2*f1wm2*tau12*f2f-tau13*tau21**2*f1vm2*
     & f2wm2*ttu12+tau23*tau11*f1um2*f2wm1*tau12*ttu21+tau23*tau11*
     & f1vm2*tau21*f2wm2*ttu12+tau13*tau21**2*f1wm2*ttu12*f2vm2+tau23*
     & tau12*tau21*f1f*f2wm2*tau11-tau13*tau21**2*f1vm2*f2wm1*ttu11-
     & tau23*tau11*f1wm1*tau12*ttu21*f2um2-tau23*tau11*f1wm2*tau12*
     & ttu22*f2um2-tau13**2*tau21*f1vm2*ttu22*f2um2-tau13**2*tau21**2*
     & f1vm2*f2f+tau13**2*tau21*f1um2*f2vm2*ttu22-tau23*tau11*f1wm2*
     & ttu12*f2vm2*tau21-tau21*f1wm2*tau11*f2wm1*tau12*ttu21+tau13**2*
     & tau21**2*f1f*f2vm2-tau21*f1um2*tau23*tau12*f2wm1*ttu11+tau13**
     & 2*ttu21*f1um1*f2vm2*tau21+tau21*f1wm1*ttu11*f2um2*tau23*tau12-
     & tau21*f1wm1*ttu11*f2vm2*tau23*tau11+tau23**2*tau11**2*f1f*
     & f2vm2-tau13**2*ttu21*f1vm2*tau21*f2um1-tau13*tau21**2*f1f*
     & f2wm2*tau12-tau13*ttu21*f1um2*f2um1*tau23*tau12+tau13*tau21**2*
     & f1wm1*ttu11*f2vm2-tau13*tau21*f1wm2*tau11*f2vm2*ttu22+tau13*
     & tau21*f1vm2*tau23*ttu12*f2um2+tau13*tau21*f1f*f2um2*tau23*
     & tau12-tau13*tau23*ttu11*f1um1*f2vm2*tau21-tau13*ttu21*f1um1*
     & f2vm2*tau23*tau11-tau13*ttu21*f1um1*f2wm2*tau12*tau21+tau13*
     & ttu21*f1um1*f2um2*tau23*tau12-tau13*tau21*f1um2*f2wm2*tau12*
     & ttu22+tau13*ttu21*f1vm2*tau23*tau11*f2um1+tau13*ttu21*f1vm2*
     & tau21*f2wm1*tau11+tau22*ttu21*f1wm2*tau11**2*f2wm1-tau13*tau21*
     & f1um2*f2vm2*tau23*ttu12+tau13*tau21*f1vm2*ttu22*f2wm2*tau11-
     & tau22*ttu21*f1wm1*tau11**2*f2wm2-tau13*tau21*f1um2*f2f*tau23*
     & tau12+tau13*tau23*tau11*f1vm2*ttu22*f2um2-tau22*tau23*tau11**2*
     & f1f*f2wm2+tau13*tau21*f1wm2*tau12*ttu22*f2um2-tau13*ttu21*
     & f1wm1*tau11*f2vm2*tau21+tau13*ttu21*f1wm2*tau12*tau21*f2um1+
     & tau13*tau23*ttu11*f1vm2*tau21*f2um1-tau13*tau23*tau11*f1um2*
     & f2vm2*ttu22-2*tau13*tau21*f1f*f2vm2*tau23*tau11+2*tau13*tau21*
     & f1vm2*tau23*tau11*f2f-tau22*tau13**2*tau21*f1f*f2um2+tau22*
     & tau13**2*ttu21*f1um2*f2um1+tau22*tau23*tau11**2*f1wm2*f2f-
     & tau22*tau13*ttu21*f1wm2*tau11*f2um1+tau22*tau13**2*tau21*f1um2*
     & f2f-tau22*tau13**2*ttu21*f1um1*f2um2-tau22*tau21*f1wm2*tau11*
     & f2wm1*ttu11-tau22*tau23*tau11*f1um2*f2wm2*ttu12+tau22*tau23*
     & ttu11*f1wm2*tau11*f2um1+tau22*tau21*f1wm1*ttu11*f2wm2*tau11-
     & tau22*tau13*ttu21*f1um2*f2wm1*tau11+tau22*tau13*tau21*f1um2*
     & f2wm2*ttu12-tau22*tau13*tau21*f1wm1*ttu11*f2um2+tau22*tau13*
     & tau21*f1f*f2wm2*tau11+tau22*tau13*tau21*f1um2*f2wm1*ttu11-
     & tau22*tau13*tau21*f1wm2*ttu12*f2um2-tau22*tau13*tau21*f1wm2*
     & tau11*f2f-tau22*tau13*tau23*ttu11*f1um2*f2um1+tau22*tau13*
     & ttu21*f1wm1*tau11*f2um2-tau22*tau23*ttu11*f1um1*f2wm2*tau11+
     & tau22*tau23*tau11*f1wm2*ttu12*f2um2+tau22*tau13*tau23*tau11*
     & f1f*f2um2-tau22*tau13*tau23*tau11*f1um2*f2f+tau22*tau13*tau23*
     & ttu11*f1um1*f2um2+tau22*tau13*ttu21*f1um1*f2wm2*tau11)/(-f1um1*
     & tau23**2*tau12**2*f2um2-f1wm2*tau12**2*tau21*f2um1*tau23-f1vm2*
     & tau23*tau11**2*f2wm1*tau22+f1vm2*tau23*tau11*tau13*f2um1*tau22-
     & f1wm2*tau11*tau22**2*tau13*f2um1+f1vm2*tau23*tau11*f2wm1*tau12*
     & tau21-f1vm2*tau23**2*tau11*f2um1*tau12+f1um2*tau23*tau12*tau13*
     & tau21*f2vm1+f1um2*tau23*tau12*f2wm1*tau11*tau22-f1um2*tau23**2*
     & tau12*f2vm1*tau11-f1um2*tau23*tau12**2*f2wm1*tau21+f1um2*tau23*
     & *2*tau12**2*f2um1-2*f1wm2*tau11*tau22*f2wm1*tau12*tau21+f1wm2*
     & tau11*tau22*f2um1*tau23*tau12+f1wm2*tau12*tau21*tau13*f2um1*
     & tau22-f1wm2*tau12*tau21**2*tau13*f2vm1+f1wm2*tau12*tau21*f2vm1*
     & tau23*tau11+f1wm2*tau12**2*tau21**2*f2wm1+f1vm2*tau23**2*tau11*
     & *2*f2vm1+tau13*f1um2*tau22*f2vm1*tau23*tau11+tau13*f1um2*tau22*
     & f2wm1*tau12*tau21-2*tau13*f1um2*tau22*f2um1*tau23*tau12-tau13**
     & 2*f1vm2*tau21*f2um1*tau22+tau13**2*tau21*f1vm1*f2um2*tau22+
     & tau13*tau21**2*f1vm1*f2wm2*tau12+f1wm2*tau11*tau22*tau13*tau21*
     & f2vm1+f1wm2*tau11**2*tau22**2*f2wm1-f1wm2*tau11**2*tau22*f2vm1*
     & tau23+tau13*f1vm2*tau21*f2wm1*tau11*tau22-2*tau13*f1vm2*tau21*
     & f2vm1*tau23*tau11-tau13*f1vm2*tau21**2*f2wm1*tau12+tau13*f1vm2*
     & tau21*f2um1*tau23*tau12+tau13*f1um1*tau22**2*f2wm2*tau11+2*
     & tau13*f1um1*tau22*f2um2*tau23*tau12-tau13*f1um1*tau22*f2vm2*
     & tau23*tau11+tau13**2*f1um1*tau22*f2vm2*tau21-tau13**2*f1um1*
     & tau22**2*f2um2-tau13*f1um1*tau22*f2wm2*tau12*tau21-tau13*tau21*
     & f1vm1*f2wm2*tau11*tau22-tau13*tau21*f1vm1*f2um2*tau23*tau12+2*
     & tau13*tau21*f1vm1*f2vm2*tau23*tau11-tau13**2*tau21**2*f1vm1*
     & f2vm2-f1vm1*tau23**2*tau11**2*f2vm2-f1vm1*tau23*tau11*tau13*
     & f2um2*tau22-f1vm1*tau23*tau11*f2wm2*tau12*tau21+tau13**2*f1um2*
     & tau22**2*f2um1+tau13**2*f1vm2*tau21**2*f2vm1+f1wm1*tau12*tau21*
     & *2*tau13*f2vm2-f1wm1*tau12*tau21*tau13*f2um2*tau22+2*f1wm1*
     & tau12*tau21*f2wm2*tau11*tau22+f1wm1*tau12**2*tau21*f2um2*tau23-
     & tau13**2*f1um2*tau22*tau21*f2vm1-f1um1*tau23*tau12*f2wm2*tau11*
     & tau22-f1um1*tau23*tau12*tau13*f2vm2*tau21+f1um1*tau23*tau12**2*
     & f2wm2*tau21+f1wm1*tau11**2*tau22*f2vm2*tau23-f1wm1*tau11**2*
     & tau22**2*f2wm2+f1um1*tau23**2*tau12*f2vm2*tau11-f1wm1*tau11*
     & tau22*f2um2*tau23*tau12+f1vm1*tau23**2*tau11*f2um2*tau12-f1wm1*
     & tau11*tau22*tau13*f2vm2*tau21+f1wm1*tau11*tau22**2*tau13*f2um2-
     & f1wm1*tau12*tau21*f2vm2*tau23*tau11-f1wm1*tau12**2*tau21**2*
     & f2wm2+f1vm1*tau23*tau11**2*f2wm2*tau22-tau13*f1um2*tau22**2*
     & f2wm1*tau11)

      u(i1-2*is1,i2-2*is2,i3-2*is3,ey) = -(tau23*ttu12*f1um2*f2wm1*
     & tau12*tau21+tau23**2*ttu12*f1um2*f2vm1*tau11+tau13*ttu22*f1vm1*
     & tau23*tau11*f2um2+tau13*tau21**2*f2vm1*f1wm2*ttu12-tau13*tau21*
     & *2*f2wm1*tau12*f1f+tau13*tau21**2*f2f*f1wm1*tau12+tau13*tau21**
     & 2*f2vm1*f1wm1*ttu11-tau13*tau21**2*f2wm2*ttu12*f1vm1+tau13**2*
     & tau21*ttu21*f2vm1*f1um1+tau13**2*ttu22*f1um2*tau21*f2vm1-tau13*
     & *2*ttu22*tau21*f1vm1*f2um2-tau13**2*tau21*f2um1*ttu21*f1vm1+
     & tau13*ttu22*tau21*f1vm1*f2wm2*tau11-tau13*ttu22*f1wm2*tau11*
     & tau21*f2vm1+tau13*ttu22*f1um2*f2um1*tau23*tau12+tau13*ttu22*
     & f1wm1*tau12*tau21*f2um2-tau13**2*tau21**2*f2f*f1vm1+tau13**2*
     & tau21**2*f2vm1*f1f-tau13*tau21**2*f2wm1*ttu11*f1vm1+tau13*
     & tau23*ttu12*tau21*f1vm1*f2um2+tau13*tau21*f2wm1*tau11*ttu21*
     & f1vm1-tau13*ttu22*f1um1*f2um2*tau23*tau12+ttu22*f1wm2*tau11**2*
     & f2vm1*tau23+tau23*tau12*tau21*f2wm1*tau11*f1f+tau21*f2wm1*
     & tau12*f1wm2*tau11*ttu22+tau21*f2um1*tau23*tau12*f1wm2*ttu12-
     & tau21*ttu22*f1wm1*tau12*f2wm2*tau11-tau21*f2wm2*tau12*tau23*
     & ttu12*f1um1-tau21*f2vm1*tau23*tau11*f1wm2*ttu12+tau23*tau11**2*
     & ttu21*f2vm1*f1wm1-tau23*tau12*tau21*f2f*f1wm1*tau11+tau21*
     & f2wm2*ttu12*f1vm1*tau23*tau11-tau23*tau11*f2um1*f1wm1*tau12*
     & ttu21+tau21**2*f2wm2*ttu12*f1wm1*tau12+tau23**2*tau11*f2vm1*
     & ttu11*f1um1-tau21**2*f2wm1*tau12*f1wm2*ttu12+tau23*tau11*tau21*
     & f2wm1*ttu11*f1vm1+tau23*tau11*f2wm1*tau12*ttu21*f1um1+tau23*
     & tau11*ttu22*f1um1*f2wm2*tau12-tau23**2*tau11*f2um1*tau12*f1f-
     & tau23**2*tau11*f2um1*f1vm1*ttu11+tau23**2*tau11*f2f*tau12*
     & f1um1-tau23*tau11*tau21*f2vm1*f1wm1*ttu11-ttu22*f1vm1*tau23*
     & tau11**2*f2wm2-tau23**2*ttu12*f1um2*f2um1*tau12-tau23*ttu12*
     & f1wm1*tau12*tau21*f2um2+tau23**2*ttu12*f1um1*f2um2*tau12-tau23*
     & *2*ttu12*f1vm1*tau11*f2um2-tau23*tau11*ttu22*f1wm2*tau12*f2um1-
     & tau23*tau11**2*f2wm1*ttu21*f1vm1+tau22*tau23*tau11**2*f2f*
     & f1wm1-tau22*tau23*tau11**2*f2wm1*f1f+tau22*ttu22*f1wm1*tau11**
     & 2*f2wm2+tau22*tau13*tau23*ttu12*f1um2*f2um1-tau22*tau13*tau23*
     & ttu12*f1um1*f2um2-tau22*tau13*ttu22*f1wm1*tau11*f2um2-tau13*
     & tau21*f2wm1*tau12*ttu21*f1um1-tau13*tau23*ttu12*f1um2*tau21*
     & f2vm1-tau13*tau21*f2f*tau23*tau12*f1um1-2*tau13*tau21*f2vm1*
     & tau23*tau11*f1f+tau13*tau21*f2um1*tau23*tau12*f1f+tau13*tau21*
     & f2um1*f1wm1*tau12*ttu21-tau13*tau21*ttu21*f2vm1*f1wm1*tau11-
     & tau13*tau23*tau11*ttu21*f2vm1*f1um1+tau13*tau23*tau11*f2um1*
     & ttu21*f1vm1+2*tau13*tau21*f2f*f1vm1*tau23*tau11+tau13*tau21*
     & f2um1*f1vm1*tau23*ttu11-tau13*tau21*f2vm1*tau23*ttu11*f1um1-
     & tau13*ttu22*f1um2*f2vm1*tau23*tau11-tau13*ttu22*f1um2*f2wm1*
     & tau12*tau21-tau22*tau13*tau21*f2f*f1wm1*tau11-tau22*tau13*
     & ttu22*f1um1*f2wm2*tau11+tau22*tau13**2*ttu22*f1um1*f2um2+tau22*
     & tau13**2*tau21*f2f*f1um1-tau22*tau13**2*ttu22*f1um2*f2um1-
     & tau22*tau13**2*tau21*f2um1*f1f+tau22*tau13*tau21*f2wm1*ttu11*
     & f1um1+tau22*tau13*ttu22*f1wm2*tau11*f2um1+tau22*tau13*tau21*
     & f2wm1*tau11*f1f+tau22*tau13*ttu22*f1um2*f2wm1*tau11-tau22*
     & tau13*tau21*f2um1*f1wm2*ttu12+tau22*tau13*tau21*f2wm2*ttu12*
     & f1um1-tau22*tau13*tau21*f2um1*f1wm1*ttu11-tau22*tau13*tau23*
     & tau11*f2f*f1um1-tau22*tau21*f2wm2*ttu12*f1wm1*tau11+tau22*
     & tau23*tau11*f2um1*f1wm1*ttu11-tau22*tau23*tau11*f2wm1*ttu11*
     & f1um1+tau22*tau21*f2wm1*tau11*f1wm2*ttu12+tau22*tau23*ttu12*
     & f1wm1*tau11*f2um2-tau22*ttu22*f1wm2*tau11**2*f2wm1+tau22*tau13*
     & tau23*tau11*f2um1*f1f-tau22*tau23*ttu12*f1um2*f2wm1*tau11+
     & tau23**2*tau11**2*f2vm1*f1f-tau23**2*tau11**2*f2f*f1vm1)/(-
     & f1um1*tau23**2*tau12**2*f2um2-f1wm2*tau12**2*tau21*f2um1*tau23-
     & f1vm2*tau23*tau11**2*f2wm1*tau22+f1vm2*tau23*tau11*tau13*f2um1*
     & tau22-f1wm2*tau11*tau22**2*tau13*f2um1+f1vm2*tau23*tau11*f2wm1*
     & tau12*tau21-f1vm2*tau23**2*tau11*f2um1*tau12+f1um2*tau23*tau12*
     & tau13*tau21*f2vm1+f1um2*tau23*tau12*f2wm1*tau11*tau22-f1um2*
     & tau23**2*tau12*f2vm1*tau11-f1um2*tau23*tau12**2*f2wm1*tau21+
     & f1um2*tau23**2*tau12**2*f2um1-2*f1wm2*tau11*tau22*f2wm1*tau12*
     & tau21+f1wm2*tau11*tau22*f2um1*tau23*tau12+f1wm2*tau12*tau21*
     & tau13*f2um1*tau22-f1wm2*tau12*tau21**2*tau13*f2vm1+f1wm2*tau12*
     & tau21*f2vm1*tau23*tau11+f1wm2*tau12**2*tau21**2*f2wm1+f1vm2*
     & tau23**2*tau11**2*f2vm1+tau13*f1um2*tau22*f2vm1*tau23*tau11+
     & tau13*f1um2*tau22*f2wm1*tau12*tau21-2*tau13*f1um2*tau22*f2um1*
     & tau23*tau12-tau13**2*f1vm2*tau21*f2um1*tau22+tau13**2*tau21*
     & f1vm1*f2um2*tau22+tau13*tau21**2*f1vm1*f2wm2*tau12+f1wm2*tau11*
     & tau22*tau13*tau21*f2vm1+f1wm2*tau11**2*tau22**2*f2wm1-f1wm2*
     & tau11**2*tau22*f2vm1*tau23+tau13*f1vm2*tau21*f2wm1*tau11*tau22-
     & 2*tau13*f1vm2*tau21*f2vm1*tau23*tau11-tau13*f1vm2*tau21**2*
     & f2wm1*tau12+tau13*f1vm2*tau21*f2um1*tau23*tau12+tau13*f1um1*
     & tau22**2*f2wm2*tau11+2*tau13*f1um1*tau22*f2um2*tau23*tau12-
     & tau13*f1um1*tau22*f2vm2*tau23*tau11+tau13**2*f1um1*tau22*f2vm2*
     & tau21-tau13**2*f1um1*tau22**2*f2um2-tau13*f1um1*tau22*f2wm2*
     & tau12*tau21-tau13*tau21*f1vm1*f2wm2*tau11*tau22-tau13*tau21*
     & f1vm1*f2um2*tau23*tau12+2*tau13*tau21*f1vm1*f2vm2*tau23*tau11-
     & tau13**2*tau21**2*f1vm1*f2vm2-f1vm1*tau23**2*tau11**2*f2vm2-
     & f1vm1*tau23*tau11*tau13*f2um2*tau22-f1vm1*tau23*tau11*f2wm2*
     & tau12*tau21+tau13**2*f1um2*tau22**2*f2um1+tau13**2*f1vm2*tau21*
     & *2*f2vm1+f1wm1*tau12*tau21**2*tau13*f2vm2-f1wm1*tau12*tau21*
     & tau13*f2um2*tau22+2*f1wm1*tau12*tau21*f2wm2*tau11*tau22+f1wm1*
     & tau12**2*tau21*f2um2*tau23-tau13**2*f1um2*tau22*tau21*f2vm1-
     & f1um1*tau23*tau12*f2wm2*tau11*tau22-f1um1*tau23*tau12*tau13*
     & f2vm2*tau21+f1um1*tau23*tau12**2*f2wm2*tau21+f1wm1*tau11**2*
     & tau22*f2vm2*tau23-f1wm1*tau11**2*tau22**2*f2wm2+f1um1*tau23**2*
     & tau12*f2vm2*tau11-f1wm1*tau11*tau22*f2um2*tau23*tau12+f1vm1*
     & tau23**2*tau11*f2um2*tau12-f1wm1*tau11*tau22*tau13*f2vm2*tau21+
     & f1wm1*tau11*tau22**2*tau13*f2um2-f1wm1*tau12*tau21*f2vm2*tau23*
     & tau11-f1wm1*tau12**2*tau21**2*f2wm2+f1vm1*tau23*tau11**2*f2wm2*
     & tau22-tau13*f1um2*tau22**2*f2wm1*tau11)

      u(i1-is1,i2-is2,i3-is3,ez) = (tau12*tau13*ttu21*f1vm2*tau21*
     & f2um1-tau21*f1f*f2um2*tau23*tau12**2+tau21*f1um2*f2f*tau23*
     & tau12**2+tau11*f1wm2*tau12*tau21*ttu21*f2vm1+tau11*ttu21*f1vm1*
     & f2um2*tau23*tau12+2*tau11*f1wm2*tau12*tau21*f2f*tau22-tau11*
     & tau13*f1vm2*tau21*ttu21*f2vm1-tau11*f1vm2*tau21*f2f*tau23*
     & tau12-ttu11*f1um1*tau22*f2vm2*tau23*tau11-ttu11*tau21*f1vm1*
     & f2wm2*tau11*tau22+ttu11*f1um1*tau22**2*f2wm2*tau11+ttu11*tau13*
     & f1um1*tau22*f2vm2*tau21-ttu11*tau13*f1vm2*tau21*f2um1*tau22+
     & ttu11*tau13*tau21*f1vm1*f2um2*tau22+ttu11*tau21**2*f1vm1*f2wm2*
     & tau12+ttu11*f1wm2*tau11*tau22*tau21*f2vm1+tau12*tau21*f1um2*
     & f2vm2*tau23*ttu12-tau12*tau21*f1vm2*ttu22*f2wm2*tau11+tau12*
     & ttu21*f1um1*f2vm2*tau23*tau11-tau12*ttu21*f1vm2*tau23*tau11*
     & f2um1-ttu11*tau13*f1um2*tau22*tau21*f2vm1+ttu11*tau13*f1vm2*
     & tau21**2*f2vm1-tau12*tau22*tau21*f1um2*f2wm2*ttu12-tau12*tau22*
     & tau13*tau21*f1um2*f2f+tau12*tau22*tau13*ttu21*f1um1*f2um2-
     & tau12*tau22*tau13*ttu21*f1um2*f2um1+tau12*tau22*ttu21*f1wm2*
     & tau11*f2um1+tau12*tau22*tau13*tau21*f1f*f2um2-tau11*tau13*
     & f1vm2*tau21*f2f*tau22+ttu11*f1um2*tau23*tau12*tau21*f2vm1+
     & tau12*tau22*tau23*ttu11*f1um1*f2um2-tau12*tau22*ttu21*f1um1*
     & f2wm2*tau11-tau12*tau22*tau23*ttu11*f1um2*f2um1-f1f*tau22*
     & f2vm2*tau23*tau11**2-f1wm2*tau11**2*tau22*f2vm2*ttu22-ttu11*
     & tau13*tau21**2*f1vm1*f2vm2+ttu11*tau13*f1um2*tau22**2*f2um1+
     & f1f*tau22**2*f2wm2*tau11**2+ttu21*f1vm1*f2wm2*tau11**2*tau22-
     & ttu11*tau21*f1vm1*f2um2*tau23*tau12+f1vm2*ttu22*f2wm2*tau11**2*
     & tau22-ttu21*f1vm1*f2vm2*tau23*tau11**2-ttu11*f1wm2*tau12*tau21*
     & *2*f2vm1+ttu11*f1wm2*tau12*tau21*f2um1*tau22+ttu11*f1vm2*tau23*
     & tau11*f2um1*tau22-ttu11*f1wm2*tau11*tau22**2*f2um1+tau11*tau13*
     & f1um2*tau22**2*f2f+tau11*f1vm2*tau23*ttu12*f2um2*tau22-tau11*
     & tau13*f1f*tau22**2*f2um2+tau11*f1vm1*tau23*ttu11*f2vm2*tau21+
     & tau11*f1wm2*tau12*ttu22*f2um2*tau22-tau11*ttu21*f1vm1*f2wm2*
     & tau12*tau21+tau11*tau13*f1um2*tau22*ttu21*f2vm1+tau11*f1um2*
     & tau22**2*f2wm2*ttu12+tau11*f1f*tau23*tau12*f2vm2*tau21+tau11*
     & tau13*f1f*tau22*f2vm2*tau21-tau11*f1wm2*ttu12*tau22**2*f2um2+
     & tau11*f1wm2*ttu12*tau22*f2vm2*tau21+tau11*f1f*tau22*f2um2*
     & tau23*tau12-f1wm2*tau11**2*tau22*ttu21*f2vm1-f1wm2*tau11**2*
     & tau22**2*f2f+f1vm2*tau23*tau11**2*f2f*tau22+f1vm2*tau23*tau11**
     & 2*ttu21*f2vm1-tau11*f1vm2*tau21*f2wm2*ttu12*tau22-2*tau11*f1f*
     & tau22*f2wm2*tau12*tau21-tau11*tau13*f1vm2*ttu22*f2um2*tau22-
     & tau11*f1vm2*tau21*f2vm1*tau23*ttu11-tau11*f1um2*tau22*f2vm2*
     & tau23*ttu12-tau11*f1um2*tau22*f2wm2*tau12*ttu22-tau11*f1um2*
     & tau22*f2f*tau23*tau12-tau11*tau13*ttu21*f1vm1*f2um2*tau22+
     & tau11*tau13*f1um2*tau22*f2vm2*ttu22-tau21**2*f1wm2*tau12**2*
     & f2f-tau11*f1um2*tau23*tau12*ttu21*f2vm1+tau11*tau13*ttu21*
     & f1vm1*f2vm2*tau21+ttu21*f1um1*f2wm2*tau12**2*tau21-ttu21*f1um1*
     & f2um2*tau23*tau12**2+tau21*f1um2*f2wm2*tau12**2*ttu22-tau12*
     & tau13*tau21**2*f1f*f2vm2-tau12*tau13*ttu21*f1um1*f2vm2*tau21+
     & tau12*tau13*tau21*f1vm2*ttu22*f2um2+tau12*tau13*tau21**2*f1vm2*
     & f2f-tau12*tau21**2*f1wm2*ttu12*f2vm2-tau12*tau13*tau21*f1um2*
     & f2vm2*ttu22+tau12*tau21*f1wm2*tau11*f2vm2*ttu22-tau12*tau21*
     & f1vm2*tau23*ttu12*f2um2+tau12*tau21**2*f1vm2*f2wm2*ttu12-tau21*
     & f1wm2*tau12**2*ttu22*f2um2-ttu21*f1wm2*tau12**2*tau21*f2um1+
     & ttu21*f1um2*f2um1*tau23*tau12**2+tau21**2*f1f*f2wm2*tau12**2+
     & tau12*tau22*tau21*f1wm2*ttu12*f2um2-ttu11*tau13*f1um1*tau22**2*
     & f2um2-ttu11*f1um1*tau22*f2wm2*tau12*tau21)/(-f1um1*tau23**2*
     & tau12**2*f2um2-f1wm2*tau12**2*tau21*f2um1*tau23-f1vm2*tau23*
     & tau11**2*f2wm1*tau22+f1vm2*tau23*tau11*tau13*f2um1*tau22-f1wm2*
     & tau11*tau22**2*tau13*f2um1+f1vm2*tau23*tau11*f2wm1*tau12*tau21-
     & f1vm2*tau23**2*tau11*f2um1*tau12+f1um2*tau23*tau12*tau13*tau21*
     & f2vm1+f1um2*tau23*tau12*f2wm1*tau11*tau22-f1um2*tau23**2*tau12*
     & f2vm1*tau11-f1um2*tau23*tau12**2*f2wm1*tau21+f1um2*tau23**2*
     & tau12**2*f2um1-2*f1wm2*tau11*tau22*f2wm1*tau12*tau21+f1wm2*
     & tau11*tau22*f2um1*tau23*tau12+f1wm2*tau12*tau21*tau13*f2um1*
     & tau22-f1wm2*tau12*tau21**2*tau13*f2vm1+f1wm2*tau12*tau21*f2vm1*
     & tau23*tau11+f1wm2*tau12**2*tau21**2*f2wm1+f1vm2*tau23**2*tau11*
     & *2*f2vm1+tau13*f1um2*tau22*f2vm1*tau23*tau11+tau13*f1um2*tau22*
     & f2wm1*tau12*tau21-2*tau13*f1um2*tau22*f2um1*tau23*tau12-tau13**
     & 2*f1vm2*tau21*f2um1*tau22+tau13**2*tau21*f1vm1*f2um2*tau22+
     & tau13*tau21**2*f1vm1*f2wm2*tau12+f1wm2*tau11*tau22*tau13*tau21*
     & f2vm1+f1wm2*tau11**2*tau22**2*f2wm1-f1wm2*tau11**2*tau22*f2vm1*
     & tau23+tau13*f1vm2*tau21*f2wm1*tau11*tau22-2*tau13*f1vm2*tau21*
     & f2vm1*tau23*tau11-tau13*f1vm2*tau21**2*f2wm1*tau12+tau13*f1vm2*
     & tau21*f2um1*tau23*tau12+tau13*f1um1*tau22**2*f2wm2*tau11+2*
     & tau13*f1um1*tau22*f2um2*tau23*tau12-tau13*f1um1*tau22*f2vm2*
     & tau23*tau11+tau13**2*f1um1*tau22*f2vm2*tau21-tau13**2*f1um1*
     & tau22**2*f2um2-tau13*f1um1*tau22*f2wm2*tau12*tau21-tau13*tau21*
     & f1vm1*f2wm2*tau11*tau22-tau13*tau21*f1vm1*f2um2*tau23*tau12+2*
     & tau13*tau21*f1vm1*f2vm2*tau23*tau11-tau13**2*tau21**2*f1vm1*
     & f2vm2-f1vm1*tau23**2*tau11**2*f2vm2-f1vm1*tau23*tau11*tau13*
     & f2um2*tau22-f1vm1*tau23*tau11*f2wm2*tau12*tau21+tau13**2*f1um2*
     & tau22**2*f2um1+tau13**2*f1vm2*tau21**2*f2vm1+f1wm1*tau12*tau21*
     & *2*tau13*f2vm2-f1wm1*tau12*tau21*tau13*f2um2*tau22+2*f1wm1*
     & tau12*tau21*f2wm2*tau11*tau22+f1wm1*tau12**2*tau21*f2um2*tau23-
     & tau13**2*f1um2*tau22*tau21*f2vm1-f1um1*tau23*tau12*f2wm2*tau11*
     & tau22-f1um1*tau23*tau12*tau13*f2vm2*tau21+f1um1*tau23*tau12**2*
     & f2wm2*tau21+f1wm1*tau11**2*tau22*f2vm2*tau23-f1wm1*tau11**2*
     & tau22**2*f2wm2+f1um1*tau23**2*tau12*f2vm2*tau11-f1wm1*tau11*
     & tau22*f2um2*tau23*tau12+f1vm1*tau23**2*tau11*f2um2*tau12-f1wm1*
     & tau11*tau22*tau13*f2vm2*tau21+f1wm1*tau11*tau22**2*tau13*f2um2-
     & f1wm1*tau12*tau21*f2vm2*tau23*tau11-f1wm1*tau12**2*tau21**2*
     & f2wm2+f1vm1*tau23*tau11**2*f2wm2*tau22-tau13*f1um2*tau22**2*
     & f2wm1*tau11)

      u(i1-2*is1,i2-2*is2,i3-2*is3,ez) = (tau12*tau22*tau23*ttu12*
     & f1um1*f2um2-tau12*tau22*ttu22*f1wm1*tau11*f2um2-tau12*tau21*
     & ttu21*f2vm1*f1wm1*tau11+tau12*tau21*f2um1*f1vm1*tau23*ttu11-
     & tau12*tau21*f2vm1*tau23*ttu11*f1um1-tau12*ttu22*f1um2*f2vm1*
     & tau23*tau11+tau12*tau22*tau13*ttu22*f1um1*f2um2+tau12*tau22*
     & tau13*tau21*f2f*f1um1-tau11*tau13*f2f*tau22**2*f1um1-tau12*
     & tau22*tau23*ttu12*f1um2*f2um1-tau21**2*f2wm1*tau12**2*f1f+
     & ttu12*tau13*f1um2*tau22**2*f2um1+ttu12*f1wm1*tau12*tau21**2*
     & f2vm2+ttu12*f1um2*tau22*f2vm1*tau23*tau11+ttu12*f1um2*tau22*
     & f2wm1*tau12*tau21-ttu12*tau13*f1vm2*tau21*f2um1*tau22-tau12*
     & tau22*tau21*f2um1*f1wm1*ttu11+tau11*tau21*tau13*f2f*tau22*
     & f1vm1+tau12*tau13*ttu22*f1um2*tau21*f2vm1-ttu12*f1wm1*tau11*
     & tau22*f2vm2*tau21+ttu12*f1wm1*tau11*tau22**2*f2um2-ttu12*f1um2*
     & tau22**2*f2wm1*tau11-ttu12*f1wm1*tau12*tau21*f2um2*tau22-ttu12*
     & tau13*f1um2*tau22*tau21*f2vm1-ttu12*f1um1*tau23*tau12*f2vm2*
     & tau21+ttu12*tau13*f1vm2*tau21**2*f2vm1+ttu12*tau13*tau21*f1vm1*
     & f2um2*tau22+ttu12*f1vm2*tau21*f2wm1*tau11*tau22-ttu12*f1vm2*
     & tau21**2*f2wm1*tau12+ttu12*f1vm2*tau21*f2um1*tau23*tau12+ttu12*
     & tau13*f1um1*tau22*f2vm2*tau21-ttu12*tau13*f1um1*tau22**2*f2um2-
     & ttu12*tau13*tau21**2*f1vm1*f2vm2-tau11*tau13*ttu21*f2vm1*f1um1*
     & tau22+tau11*tau13*ttu22*f1vm2*f2um1*tau22+tau11*tau13*f2um1*
     & tau22*ttu21*f1vm1-f2vm2*ttu22*f1vm1*tau23*tau11**2+ttu21*f2vm1*
     & f1wm1*tau11**2*tau22+f2vm2*ttu22*f1wm1*tau11**2*tau22-f2wm1*
     & tau11**2*tau22**2*f1f+f2vm1*tau23*tau11**2*f1f*tau22-f2wm1*
     & tau11**2*tau22*ttu21*f1vm1+f2f*tau22**2*f1wm1*tau11**2+ttu22*
     & f1vm2*f2vm1*tau23*tau11**2-ttu22*f1vm2*f2wm1*tau11**2*tau22-
     & tau11*tau21*f2vm1*f1wm1*ttu11*tau22+tau11*tau13*f2um1*tau22**2*
     & f1f+tau11*f2vm2*ttu22*f1um1*tau23*tau12-tau11*f2wm1*ttu11*
     & tau22**2*f1um1-2*tau11*tau21*f2f*tau22*f1wm1*tau12+tau11*tau21*
     & f2f*tau23*tau12*f1vm1-tau11*tau21*f2vm1*f1f*tau23*tau12+tau11*
     & tau21*ttu22*f1vm2*f2wm1*tau12+tau11*tau21*f2vm2*tau23*ttu12*
     & f1vm1-tau11*tau21*tau13*ttu22*f1vm2*f2vm1-tau11*tau21*tau13*
     & f2vm1*f1f*tau22+tau11*tau21*tau13*ttu22*f1vm1*f2vm2-tau11*
     & f2um1*tau22*f1wm1*tau12*ttu21+tau11*f2vm1*tau23*ttu11*f1um1*
     & tau22-tau11*ttu22*f1vm2*f2um1*tau23*tau12-tau11*f2um1*tau23*
     & tau12*f1f*tau22-tau11*f2um1*tau22*f1vm1*tau23*ttu11+tau11*f2f*
     & tau23*tau12*f1um1*tau22+tau11*f2um1*tau22**2*f1wm1*ttu11-tau11*
     & tau21*tau23*ttu12*f1vm2*f2vm1+tau21**2*f2f*f1wm1*tau12**2+
     & ttu22*f1um2*f2um1*tau23*tau12**2+tau12*ttu22*f1vm1*tau23*tau11*
     & f2um2+tau12*tau21**2*f2vm1*f1wm1*ttu11+tau12*tau13*tau21*ttu21*
     & f2vm1*f1um1-tau12*tau13*ttu22*tau21*f1vm1*f2um2-tau12*tau13*
     & tau21*f2um1*ttu21*f1vm1-tau12*tau13*tau21**2*f2f*f1vm1+tau12*
     & tau13*tau21**2*f2vm1*f1f-tau12*tau21**2*f2wm1*ttu11*f1vm1+
     & tau12*tau21*f2wm1*tau11*ttu21*f1vm1+tau11*f2wm1*tau12*ttu21*
     & f1um1*tau22-tau11*tau21*f2vm2*ttu22*f1wm1*tau12+tau11*tau21*
     & f2wm1*ttu11*tau22*f1vm1-tau12*tau22*tau13*ttu22*f1um2*f2um1-
     & tau12*tau22*tau13*tau21*f2um1*f1f+tau12*tau22*tau21*f2wm1*
     & ttu11*f1um1+tau12*tau22*ttu22*f1um2*f2wm1*tau11-tau21*f2wm1*
     & tau12**2*ttu21*f1um1-tau21*f2f*tau23*tau12**2*f1um1+tau21*
     & f2um1*tau23*tau12**2*f1f+tau21*f2um1*f1wm1*tau12**2*ttu21-
     & ttu22*f1um2*f2wm1*tau12**2*tau21+ttu22*f1wm1*tau12**2*tau21*
     & f2um2-ttu22*f1um1*f2um2*tau23*tau12**2+2*tau11*tau21*f2wm1*
     & tau12*f1f*tau22-tau11*tau13*ttu22*f1um1*tau22*f2vm2-f2f*tau22*
     & f1vm1*tau23*tau11**2-ttu12*f1vm1*tau23*tau11*f2um2*tau22)/(-
     & f1um1*tau23**2*tau12**2*f2um2-f1wm2*tau12**2*tau21*f2um1*tau23-
     & f1vm2*tau23*tau11**2*f2wm1*tau22+f1vm2*tau23*tau11*tau13*f2um1*
     & tau22-f1wm2*tau11*tau22**2*tau13*f2um1+f1vm2*tau23*tau11*f2wm1*
     & tau12*tau21-f1vm2*tau23**2*tau11*f2um1*tau12+f1um2*tau23*tau12*
     & tau13*tau21*f2vm1+f1um2*tau23*tau12*f2wm1*tau11*tau22-f1um2*
     & tau23**2*tau12*f2vm1*tau11-f1um2*tau23*tau12**2*f2wm1*tau21+
     & f1um2*tau23**2*tau12**2*f2um1-2*f1wm2*tau11*tau22*f2wm1*tau12*
     & tau21+f1wm2*tau11*tau22*f2um1*tau23*tau12+f1wm2*tau12*tau21*
     & tau13*f2um1*tau22-f1wm2*tau12*tau21**2*tau13*f2vm1+f1wm2*tau12*
     & tau21*f2vm1*tau23*tau11+f1wm2*tau12**2*tau21**2*f2wm1+f1vm2*
     & tau23**2*tau11**2*f2vm1+tau13*f1um2*tau22*f2vm1*tau23*tau11+
     & tau13*f1um2*tau22*f2wm1*tau12*tau21-2*tau13*f1um2*tau22*f2um1*
     & tau23*tau12-tau13**2*f1vm2*tau21*f2um1*tau22+tau13**2*tau21*
     & f1vm1*f2um2*tau22+tau13*tau21**2*f1vm1*f2wm2*tau12+f1wm2*tau11*
     & tau22*tau13*tau21*f2vm1+f1wm2*tau11**2*tau22**2*f2wm1-f1wm2*
     & tau11**2*tau22*f2vm1*tau23+tau13*f1vm2*tau21*f2wm1*tau11*tau22-
     & 2*tau13*f1vm2*tau21*f2vm1*tau23*tau11-tau13*f1vm2*tau21**2*
     & f2wm1*tau12+tau13*f1vm2*tau21*f2um1*tau23*tau12+tau13*f1um1*
     & tau22**2*f2wm2*tau11+2*tau13*f1um1*tau22*f2um2*tau23*tau12-
     & tau13*f1um1*tau22*f2vm2*tau23*tau11+tau13**2*f1um1*tau22*f2vm2*
     & tau21-tau13**2*f1um1*tau22**2*f2um2-tau13*f1um1*tau22*f2wm2*
     & tau12*tau21-tau13*tau21*f1vm1*f2wm2*tau11*tau22-tau13*tau21*
     & f1vm1*f2um2*tau23*tau12+2*tau13*tau21*f1vm1*f2vm2*tau23*tau11-
     & tau13**2*tau21**2*f1vm1*f2vm2-f1vm1*tau23**2*tau11**2*f2vm2-
     & f1vm1*tau23*tau11*tau13*f2um2*tau22-f1vm1*tau23*tau11*f2wm2*
     & tau12*tau21+tau13**2*f1um2*tau22**2*f2um1+tau13**2*f1vm2*tau21*
     & *2*f2vm1+f1wm1*tau12*tau21**2*tau13*f2vm2-f1wm1*tau12*tau21*
     & tau13*f2um2*tau22+2*f1wm1*tau12*tau21*f2wm2*tau11*tau22+f1wm1*
     & tau12**2*tau21*f2um2*tau23-tau13**2*f1um2*tau22*tau21*f2vm1-
     & f1um1*tau23*tau12*f2wm2*tau11*tau22-f1um1*tau23*tau12*tau13*
     & f2vm2*tau21+f1um1*tau23*tau12**2*f2wm2*tau21+f1wm1*tau11**2*
     & tau22*f2vm2*tau23-f1wm1*tau11**2*tau22**2*f2wm2+f1um1*tau23**2*
     & tau12*f2vm2*tau11-f1wm1*tau11*tau22*f2um2*tau23*tau12+f1vm1*
     & tau23**2*tau11*f2um2*tau12-f1wm1*tau11*tau22*tau13*f2vm2*tau21+
     & f1wm1*tau11*tau22**2*tau13*f2um2-f1wm1*tau12*tau21*f2vm2*tau23*
     & tau11-f1wm1*tau12**2*tau21**2*f2wm2+f1vm1*tau23*tau11**2*f2wm2*
     & tau22-tau13*f1um2*tau22**2*f2wm1*tau11)


 ! *********** done *********************
                 !  if( .true. .or. debug.gt.0 )then
                 !   write(*,'(" bc4:corr:   i1,i2,i3=",3i3," u(-1)=",3f8.2," u(-2)=",3f8.2)') i1,i2,i3,!          u(i1-is1,i2-is2,i3-is3,ex),u(i1-is1,i2-is2,i3-is3,ey),u(i1-is1,i2-is2,i3-is3,ez),!          u(i1-2*is1,i2-2*is2,i3-2*is3,ex),u(i1-2*is1,i2-2*is2,i3-2*is3,ey),u(i1-2*is1,i2-2*is2,i3-2*is3,ez)
                 !  end if
                   if( debug.gt.0 )then
                     call ogf3d(ep,xy(i1-is1,i2-is2,i3-is3,0),xy(i1-
     & is1,i2-is2,i3-is3,1),xy(i1-is1,i2-is2,i3-is3,2),t,uvm(0),uvm(1)
     & ,uvm(2))
                     call ogf3d(ep,xy(i1-2*is1,i2-2*is2,i3-2*is3,0),xy(
     & i1-2*is1,i2-2*is2,i3-2*is3,1),xy(i1-2*is1,i2-2*is2,i3-2*is3,2),
     & t,uvm2(0),uvm2(1),uvm2(2))
                    write(*,'(" **bc4:correction: i=",3i4," errors u(-
     & 1)=",3e10.2)') i1,i2,i3,u(i1-is1,i2-is2,i3-is3,ex)-uvm(0),u(i1-
     & is1,i2-is2,i3-is3,ey)-uvm(1),u(i1-is1,i2-is2,i3-is3,ez)-uvm(2)
                    write(*,'(" **bc4:correction: i=",3i4," errors u(-
     & 2)=",3e10.2)') i1,i2,i3,u(i1-2*is1,i2-2*is2,i3-2*is3,ex)-uvm2(
     & 0),u(i1-2*is1,i2-2*is2,i3-2*is3,ey)-uvm2(1),u(i1-2*is1,i2-2*
     & is2,i3-2*is3,ez)-uvm2(2)
                   end if
                   ! set to exact for testing
                   ! OGF3D(i1-is1,i2-is2,i3-is3,t, uvm(0),uvm(1),uvm(2))
                   ! OGF3D(i1-2*is1,i2-2*is2,i3-2*is3,t, uvm2(0),uvm2(1),uvm2(2))
                   ! u(i1-is1,i2-is2,i3-is3,ex)=uvm(0)
                   ! u(i1-is1,i2-is2,i3-is3,ey)=uvm(1)
                   ! u(i1-is1,i2-is2,i3-is3,ez)=uvm(2)
                   ! u(i1-2*is1,i2-2*is2,i3-2*is3,ex)=uvm2(0)
                   ! u(i1-2*is1,i2-2*is2,i3-2*is3,ey)=uvm2(1)
                   ! u(i1-2*is1,i2-2*is2,i3-2*is3,ez)=uvm2(2)
                 end if ! mask
                 end do
                 end do
                 end do
                 end if ! if true
                 if( debug.gt.0 )then
                 ! ============================DEBUG=======================================================
                 ! ============================END DEBUG=======================================================
                 end if
               !   stop 11122
               else
               ! This next instance does both ??
                  ! Since is1 is +1 or -1 we need to flip the sign of dr in the derivative approximations
                  dra = dr(axis  )*(1-2*side)
                  dsa = dr(axisp1)*(1-2*side)
                  dta = dr(axisp2)*(1-2*side)
                  drb = dr(axis  )
                  dsb = dr(axisp1)
                  dtb = dr(axisp2)
                  ! ** Fourth-order for tau.Delta\uv=0, setting  ctlrr=ctlr=0 in the code will revert to 2nd-order
                  ctlrr=1.
                  ctlr=1.
                  if( debug.gt.0 )then
                    write(*,'(" **bcCurvilinear3dOrder4: START: grid,
     & side,axis=",3i2," is1,is2,is3=",3i3," ks1,ks2,ks3=",3i3)')grid,
     & side,axis,is1,is2,is3,ks1,ks2,ks3
                  end if
                 ! ******************************************
                 ! ************Correction loop***************
                 ! ******************************************
                 ! Given an initial answer at all points we now go back and resolve for the normal component
                 ! from   div(u)=0 and (a1.Delta u).r = 0 
                 ! We use the initial guess in order to compute the mixed derivatives urs, urss, urtt
                 if( .true. )then
                 ! ** Periodic update is now done in a previous step -- this doesn't work in parallel
                 ! first do a periodic update
                 ! if( .false. .and.(boundaryCondition(0,axisp1).lt.0 .or. boundaryCondition(0,axisp2).lt.0) )then
                 !   indexRange(0,0)=gridIndexRange(0,0)
                 !   indexRange(1,0)=gridIndexRange(1,0)
                 !   indexRange(0,1)=gridIndexRange(0,1)
                 !   indexRange(1,1)=gridIndexRange(1,1)
                 !   indexRange(0,2)=gridIndexRange(0,2)
                 !   indexRange(1,2)=gridIndexRange(1,2)
                 !
                 !   isPeriodic(0)=0
                 !   isPeriodic(1)=0
                 !   isPeriodic(2)=0
                 !   if( boundaryCondition(0,axisp1).lt.0 )then
                 !     indexRange(1,axisp1)=gridIndexRange(1,axisp1)-1
                 !     isPeriodic(axisp1)=1  
                 !   end if
                 !   if( boundaryCondition(0,axisp2).lt.0 )then
                 !     indexRange(1,axisp2)=gridIndexRange(1,axisp2)-1
                 !     isPeriodic(axisp2)=1  
                 !   end if
                 !
                 !  write(*,'(" *********** call periodic update grid,side,axis=",3i4)') grid,side,axis
                 !
                 !  call periodicUpdateMaxwell(nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,!     u,ex,ez, indexRange, gridIndexRange, dimension, isPeriodic )
                 ! end if
                  do i3=n3a,n3b
                  do i2=n2a,n2b
                  do i1=n1a,n1b
                  if( mask(i1,i2,i3).gt.0 )then
                   ! precompute the inverse of the jacobian, used in macros AmnD3J
                   i10=i1  ! used by jac3di in macros
                   i20=i2
                   i30=i3
                   do m3=-2,2
                   do m2=-2,2
                   do m1=-2,2
                    jac3di(m1,m2,m3)=1./(rx(i1+m1,i2+m2,i3+m3)*(sy(i1+
     & m1,i2+m2,i3+m3)*tz(i1+m1,i2+m2,i3+m3)-sz(i1+m1,i2+m2,i3+m3)*ty(
     & i1+m1,i2+m2,i3+m3))+ry(i1+m1,i2+m2,i3+m3)*(sz(i1+m1,i2+m2,i3+
     & m3)*tx(i1+m1,i2+m2,i3+m3)-sx(i1+m1,i2+m2,i3+m3)*tz(i1+m1,i2+m2,
     & i3+m3))+rz(i1+m1,i2+m2,i3+m3)*(sx(i1+m1,i2+m2,i3+m3)*ty(i1+m1,
     & i2+m2,i3+m3)-sy(i1+m1,i2+m2,i3+m3)*tx(i1+m1,i2+m2,i3+m3)))
                   end do
                   end do
                   end do
                   a11 =(rsxy(i1,i2,i3,axis,0)*jac3di(i1-i10,i2-i20,i3-
     & i30))
                   a12 =(rsxy(i1,i2,i3,axis,1)*jac3di(i1-i10,i2-i20,i3-
     & i30))
                   a13 =(rsxy(i1,i2,i3,axis,2)*jac3di(i1-i10,i2-i20,i3-
     & i30))
                   a21 =(rsxy(i1,i2,i3,axisp1,0)*jac3di(i1-i10,i2-i20,
     & i3-i30))
                   a22 =(rsxy(i1,i2,i3,axisp1,1)*jac3di(i1-i10,i2-i20,
     & i3-i30))
                   a23 =(rsxy(i1,i2,i3,axisp1,2)*jac3di(i1-i10,i2-i20,
     & i3-i30))
                   a31 =(rsxy(i1,i2,i3,axisp2,0)*jac3di(i1-i10,i2-i20,
     & i3-i30))
                   a32 =(rsxy(i1,i2,i3,axisp2,1)*jac3di(i1-i10,i2-i20,
     & i3-i30))
                   a33 =(rsxy(i1,i2,i3,axisp2,2)*jac3di(i1-i10,i2-i20,
     & i3-i30))
                   a11m1 =(rsxy(i1-is1,i2-is2,i3-is3,axis,0)*jac3di(i1-
     & is1-i10,i2-is2-i20,i3-is3-i30))
                   a12m1 =(rsxy(i1-is1,i2-is2,i3-is3,axis,1)*jac3di(i1-
     & is1-i10,i2-is2-i20,i3-is3-i30))
                   a13m1 =(rsxy(i1-is1,i2-is2,i3-is3,axis,2)*jac3di(i1-
     & is1-i10,i2-is2-i20,i3-is3-i30))
                   a11p1 =(rsxy(i1+is1,i2+is2,i3+is3,axis,0)*jac3di(i1+
     & is1-i10,i2+is2-i20,i3+is3-i30))
                   a12p1 =(rsxy(i1+is1,i2+is2,i3+is3,axis,1)*jac3di(i1+
     & is1-i10,i2+is2-i20,i3+is3-i30))
                   a13p1 =(rsxy(i1+is1,i2+is2,i3+is3,axis,2)*jac3di(i1+
     & is1-i10,i2+is2-i20,i3+is3-i30))
                   a11m2 =(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,0)*
     & jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))
                   a12m2 =(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,1)*
     & jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))
                   a13m2 =(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,2)*
     & jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))
                   a11p2 =(rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axis,0)*
     & jac3di(i1+2*is1-i10,i2+2*is2-i20,i3+2*is3-i30))
                   a12p2 =(rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axis,1)*
     & jac3di(i1+2*is1-i10,i2+2*is2-i20,i3+2*is3-i30))
                   a13p2 =(rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axis,2)*
     & jac3di(i1+2*is1-i10,i2+2*is2-i20,i3+2*is3-i30))
                   c11 = (rsxy(i1,i2,i3,axis,0)**2+rsxy(i1,i2,i3,axis,
     & 1)**2+rsxy(i1,i2,i3,axis,2)**2)
                   c22 = (rsxy(i1,i2,i3,axisp1,0)**2+rsxy(i1,i2,i3,
     & axisp1,1)**2+rsxy(i1,i2,i3,axisp1,2)**2)
                   c33 = (rsxy(i1,i2,i3,axisp2,0)**2+rsxy(i1,i2,i3,
     & axisp2,1)**2+rsxy(i1,i2,i3,axisp2,2)**2)
                   c1 = (rsxyx43(i1,i2,i3,axis,0)+rsxyy43(i1,i2,i3,
     & axis,1)+rsxyz43(i1,i2,i3,axis,2))
                   c2 = (rsxyx43(i1,i2,i3,axisp1,0)+rsxyy43(i1,i2,i3,
     & axisp1,1)+rsxyz43(i1,i2,i3,axisp1,2))
                   c3 = (rsxyx43(i1,i2,i3,axisp2,0)+rsxyy43(i1,i2,i3,
     & axisp2,1)+rsxyz43(i1,i2,i3,axisp2,2))
                   us=(8.*(u(i1+js1,i2+js2,i3+js3,ex)-u(i1-js1,i2-js2,
     & i3-js3,ex))-(u(i1+2*js1,i2+2*js2,i3+2*js3,ex)-u(i1-2*js1,i2-2*
     & js2,i3-2*js3,ex)))/(12.*dsa)
                   uss=(-30.*u(i1,i2,i3,ex)+16.*(u(i1+js1,i2+js2,i3+
     & js3,ex)+u(i1-js1,i2-js2,i3-js3,ex))-(u(i1+2*js1,i2+2*js2,i3+2*
     & js3,ex)+u(i1-2*js1,i2-2*js2,i3-2*js3,ex)))/(12.*dsa**2)
                   vs=(8.*(u(i1+js1,i2+js2,i3+js3,ey)-u(i1-js1,i2-js2,
     & i3-js3,ey))-(u(i1+2*js1,i2+2*js2,i3+2*js3,ey)-u(i1-2*js1,i2-2*
     & js2,i3-2*js3,ey)))/(12.*dsa)
                   vss=(-30.*u(i1,i2,i3,ey)+16.*(u(i1+js1,i2+js2,i3+
     & js3,ey)+u(i1-js1,i2-js2,i3-js3,ey))-(u(i1+2*js1,i2+2*js2,i3+2*
     & js3,ey)+u(i1-2*js1,i2-2*js2,i3-2*js3,ey)))/(12.*dsa**2)
                   ws=(8.*(u(i1+js1,i2+js2,i3+js3,ez)-u(i1-js1,i2-js2,
     & i3-js3,ez))-(u(i1+2*js1,i2+2*js2,i3+2*js3,ez)-u(i1-2*js1,i2-2*
     & js2,i3-2*js3,ez)))/(12.*dsa)
                   wss=(-30.*u(i1,i2,i3,ez)+16.*(u(i1+js1,i2+js2,i3+
     & js3,ez)+u(i1-js1,i2-js2,i3-js3,ez))-(u(i1+2*js1,i2+2*js2,i3+2*
     & js3,ez)+u(i1-2*js1,i2-2*js2,i3-2*js3,ez)))/(12.*dsa**2)
                   ut=(8.*(u(i1+ks1,i2+ks2,i3+ks3,ex)-u(i1-ks1,i2-ks2,
     & i3-ks3,ex))-(u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ex)-u(i1-2*ks1,i2-2*
     & ks2,i3-2*ks3,ex)))/(12.*dta)
                   utt=(-30.*u(i1,i2,i3,ex)+16.*(u(i1+ks1,i2+ks2,i3+
     & ks3,ex)+u(i1-ks1,i2-ks2,i3-ks3,ex))-(u(i1+2*ks1,i2+2*ks2,i3+2*
     & ks3,ex)+u(i1-2*ks1,i2-2*ks2,i3-2*ks3,ex)))/(12.*dta**2)
                   vt=(8.*(u(i1+ks1,i2+ks2,i3+ks3,ey)-u(i1-ks1,i2-ks2,
     & i3-ks3,ey))-(u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ey)-u(i1-2*ks1,i2-2*
     & ks2,i3-2*ks3,ey)))/(12.*dta)
                   vtt=(-30.*u(i1,i2,i3,ey)+16.*(u(i1+ks1,i2+ks2,i3+
     & ks3,ey)+u(i1-ks1,i2-ks2,i3-ks3,ey))-(u(i1+2*ks1,i2+2*ks2,i3+2*
     & ks3,ey)+u(i1-2*ks1,i2-2*ks2,i3-2*ks3,ey)))/(12.*dta**2)
                   wt=(8.*(u(i1+ks1,i2+ks2,i3+ks3,ez)-u(i1-ks1,i2-ks2,
     & i3-ks3,ez))-(u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ez)-u(i1-2*ks1,i2-2*
     & ks2,i3-2*ks3,ez)))/(12.*dta)
                   wtt=(-30.*u(i1,i2,i3,ez)+16.*(u(i1+ks1,i2+ks2,i3+
     & ks3,ez)+u(i1-ks1,i2-ks2,i3-ks3,ez))-(u(i1+2*ks1,i2+2*ks2,i3+2*
     & ks3,ez)+u(i1-2*ks1,i2-2*ks2,i3-2*ks3,ez)))/(12.*dta**2)
                  tau11=rsxy(i1,i2,i3,axisp1,0)
                  tau12=rsxy(i1,i2,i3,axisp1,1)
                  tau13=rsxy(i1,i2,i3,axisp1,2)
                  tau21=rsxy(i1,i2,i3,axisp2,0)
                  tau22=rsxy(i1,i2,i3,axisp2,1)
                  tau23=rsxy(i1,i2,i3,axisp2,2)
                  uex=u(i1,i2,i3,ex)
                  uey=u(i1,i2,i3,ey)
                  uez=u(i1,i2,i3,ez)
                  a11r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axis,0)*
     & jac3di(i1+is1-i10,i2+is2-i20,i3+is3-i30))-(rsxy(i1-is1,i2-is2,
     & i3-is3,axis,0)*jac3di(i1-is1-i10,i2-is2-i20,i3-is3-i30)))-((
     & rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axis,0)*jac3di(i1+2*is1-i10,i2+
     & 2*is2-i20,i3+2*is3-i30))-(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,
     & 0)*jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))))/(12.*dra)
                  a12r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axis,1)*
     & jac3di(i1+is1-i10,i2+is2-i20,i3+is3-i30))-(rsxy(i1-is1,i2-is2,
     & i3-is3,axis,1)*jac3di(i1-is1-i10,i2-is2-i20,i3-is3-i30)))-((
     & rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axis,1)*jac3di(i1+2*is1-i10,i2+
     & 2*is2-i20,i3+2*is3-i30))-(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,
     & 1)*jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))))/(12.*dra)
                  a13r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axis,2)*
     & jac3di(i1+is1-i10,i2+is2-i20,i3+is3-i30))-(rsxy(i1-is1,i2-is2,
     & i3-is3,axis,2)*jac3di(i1-is1-i10,i2-is2-i20,i3-is3-i30)))-((
     & rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axis,2)*jac3di(i1+2*is1-i10,i2+
     & 2*is2-i20,i3+2*is3-i30))-(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,
     & 2)*jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))))/(12.*dra)
                  if( axis.eq.0 )then
                    urs  =  urs4(i1,i2,i3,ex)
                    urt  =  urt4(i1,i2,i3,ex)
                    urss = urss4(i1,i2,i3,ex)
                    urtt = urtt4(i1,i2,i3,ex)
                    vrs  =  urs4(i1,i2,i3,ey)
                    vrt  =  urt4(i1,i2,i3,ey)
                    vrss = urss4(i1,i2,i3,ey)
                    vrtt = urtt4(i1,i2,i3,ey)
                    wrs  =  urs4(i1,i2,i3,ez)
                    wrt  =  urt4(i1,i2,i3,ez)
                    wrss = urss4(i1,i2,i3,ez)
                    wrtt = urtt4(i1,i2,i3,ez)
                    c11r = (2.*(rsxy(i1,i2,i3,axis,0)*rsxyr4(i1,i2,i3,
     & axis,0)+rsxy(i1,i2,i3,axis,1)*rsxyr4(i1,i2,i3,axis,1)+rsxy(i1,
     & i2,i3,axis,2)*rsxyr4(i1,i2,i3,axis,2)))
                    c22r = (2.*(rsxy(i1,i2,i3,axisp1,0)*rsxyr4(i1,i2,
     & i3,axisp1,0)+rsxy(i1,i2,i3,axisp1,1)*rsxyr4(i1,i2,i3,axisp1,1)+
     & rsxy(i1,i2,i3,axisp1,2)*rsxyr4(i1,i2,i3,axisp1,2)))
                    c33r = (2.*(rsxy(i1,i2,i3,axisp2,0)*rsxyr4(i1,i2,
     & i3,axisp2,0)+rsxy(i1,i2,i3,axisp2,1)*rsxyr4(i1,i2,i3,axisp2,1)+
     & rsxy(i1,i2,i3,axisp2,2)*rsxyr4(i1,i2,i3,axisp2,2)))
                    c1r = (rsxyxr43(i1,i2,i3,axis,0)+rsxyyr43(i1,i2,i3,
     & axis,1)+rsxyzr43(i1,i2,i3,axis,2))
                    c2r = (rsxyxr43(i1,i2,i3,axisp1,0)+rsxyyr43(i1,i2,
     & i3,axisp1,1)+rsxyzr43(i1,i2,i3,axisp1,2))
                    c3r = (rsxyxr43(i1,i2,i3,axisp2,0)+rsxyyr43(i1,i2,
     & i3,axisp2,1)+rsxyzr43(i1,i2,i3,axisp2,2))
                  else if( axis.eq.1 )then
                    urs  =  ust4(i1,i2,i3,ex)
                    urt  =  urs4(i1,i2,i3,ex)
                    urss = ustt4(i1,i2,i3,ex)
                    urtt = urrs4(i1,i2,i3,ex)
                    vrs  =  ust4(i1,i2,i3,ey)
                    vrt  =  urs4(i1,i2,i3,ey)
                    vrss = ustt4(i1,i2,i3,ey)
                    vrtt = urrs4(i1,i2,i3,ey)
                    wrs  =  ust4(i1,i2,i3,ez)
                    wrt  =  urs4(i1,i2,i3,ez)
                    wrss = ustt4(i1,i2,i3,ez)
                    wrtt = urrs4(i1,i2,i3,ez)
                    c11r = (2.*(rsxy(i1,i2,i3,axis,0)*rsxys4(i1,i2,i3,
     & axis,0)+rsxy(i1,i2,i3,axis,1)*rsxys4(i1,i2,i3,axis,1)+rsxy(i1,
     & i2,i3,axis,2)*rsxys4(i1,i2,i3,axis,2)))
                    c22r = (2.*(rsxy(i1,i2,i3,axisp1,0)*rsxys4(i1,i2,
     & i3,axisp1,0)+rsxy(i1,i2,i3,axisp1,1)*rsxys4(i1,i2,i3,axisp1,1)+
     & rsxy(i1,i2,i3,axisp1,2)*rsxys4(i1,i2,i3,axisp1,2)))
                    c33r = (2.*(rsxy(i1,i2,i3,axisp2,0)*rsxys4(i1,i2,
     & i3,axisp2,0)+rsxy(i1,i2,i3,axisp2,1)*rsxys4(i1,i2,i3,axisp2,1)+
     & rsxy(i1,i2,i3,axisp2,2)*rsxys4(i1,i2,i3,axisp2,2)))
                    c1r = (rsxyxs43(i1,i2,i3,axis,0)+rsxyys43(i1,i2,i3,
     & axis,1)+rsxyzs43(i1,i2,i3,axis,2))
                    c2r = (rsxyxs43(i1,i2,i3,axisp1,0)+rsxyys43(i1,i2,
     & i3,axisp1,1)+rsxyzs43(i1,i2,i3,axisp1,2))
                    c3r = (rsxyxs43(i1,i2,i3,axisp2,0)+rsxyys43(i1,i2,
     & i3,axisp2,1)+rsxyzs43(i1,i2,i3,axisp2,2))
                  else
                    urs  =  urt4(i1,i2,i3,ex)
                    urt  =  ust4(i1,i2,i3,ex)
                    urss = urrt4(i1,i2,i3,ex)
                    urtt = usst4(i1,i2,i3,ex)
                    vrs  =  urt4(i1,i2,i3,ey)
                    vrt  =  ust4(i1,i2,i3,ey)
                    vrss = urrt4(i1,i2,i3,ey)
                    vrtt = usst4(i1,i2,i3,ey)
                    wrs  =  urt4(i1,i2,i3,ez)
                    wrt  =  ust4(i1,i2,i3,ez)
                    wrss = urrt4(i1,i2,i3,ez)
                    wrtt = usst4(i1,i2,i3,ez)
                    c11r = (2.*(rsxy(i1,i2,i3,axis,0)*rsxyt4(i1,i2,i3,
     & axis,0)+rsxy(i1,i2,i3,axis,1)*rsxyt4(i1,i2,i3,axis,1)+rsxy(i1,
     & i2,i3,axis,2)*rsxyt4(i1,i2,i3,axis,2)))
                    c22r = (2.*(rsxy(i1,i2,i3,axisp1,0)*rsxyt4(i1,i2,
     & i3,axisp1,0)+rsxy(i1,i2,i3,axisp1,1)*rsxyt4(i1,i2,i3,axisp1,1)+
     & rsxy(i1,i2,i3,axisp1,2)*rsxyt4(i1,i2,i3,axisp1,2)))
                    c33r = (2.*(rsxy(i1,i2,i3,axisp2,0)*rsxyt4(i1,i2,
     & i3,axisp2,0)+rsxy(i1,i2,i3,axisp2,1)*rsxyt4(i1,i2,i3,axisp2,1)+
     & rsxy(i1,i2,i3,axisp2,2)*rsxyt4(i1,i2,i3,axisp2,2)))
                    c1r = (rsxyxt43(i1,i2,i3,axis,0)+rsxyyt43(i1,i2,i3,
     & axis,1)+rsxyzt43(i1,i2,i3,axis,2))
                    c2r = (rsxyxt43(i1,i2,i3,axisp1,0)+rsxyyt43(i1,i2,
     & i3,axisp1,1)+rsxyzt43(i1,i2,i3,axisp1,2))
                    c3r = (rsxyxt43(i1,i2,i3,axisp2,0)+rsxyyt43(i1,i2,
     & i3,axisp2,1)+rsxyzt43(i1,i2,i3,axisp2,2))
                  end if
                  Da1DotU=0.
                  ! bf = RHS to (a1.Delta u).r =0 
                  ! Here are the terms that remain after we eliminate the urrr, urr and ur terms
                  bf = a11*( c22*urss + c22r*uss + c2*urs + c2r*us + 
     & c33*urtt + c33r*utt + c3*urt + c3r*ut ) +a12*( c22*vrss + c22r*
     & vss + c2*vrs + c2r*vs + c33*vrtt + c33r*vtt + c3*vrt + c3r*vt )
     &  +a13*( c22*wrss + c22r*wss + c2*wrs + c2r*ws + c33*wrtt + 
     & c33r*wtt + c3*wrt + c3r*wt ) +a11r*( c22*uss + c2*us + c33*utt 
     & + c3*ut ) +a12r*( c22*vss + c2*vs + c33*vtt + c3*vt ) +a13r*( 
     & c22*wss + c2*ws + c33*wtt + c3*wt )
                  if( forcingOption.eq.planeWaveBoundaryForcing )then
                    ! In the plane wave forcing case we subtract out a plane wave incident field
                    a21s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axisp1,0)*
     & jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))-(rsxy(i1-js1,i2-js2,
     & i3-js3,axisp1,0)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,0)*jac3di(i1+2*js1-i10,
     & i2+2*js2-i20,i3+2*js3-i30))-(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,
     & axisp1,0)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))/(
     & 12.*dsa)
                    a22s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axisp1,1)*
     & jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))-(rsxy(i1-js1,i2-js2,
     & i3-js3,axisp1,1)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,1)*jac3di(i1+2*js1-i10,
     & i2+2*js2-i20,i3+2*js3-i30))-(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,
     & axisp1,1)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))/(
     & 12.*dsa)
                    a23s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axisp1,2)*
     & jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))-(rsxy(i1-js1,i2-js2,
     & i3-js3,axisp1,2)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,2)*jac3di(i1+2*js1-i10,
     & i2+2*js2-i20,i3+2*js3-i30))-(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,
     & axisp1,2)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))/(
     & 12.*dsa)
                    a31t = (8.*((rsxy(i1+ks1,i2+ks2,i3+ks3,axisp2,0)*
     & jac3di(i1+ks1-i10,i2+ks2-i20,i3+ks3-i30))-(rsxy(i1-ks1,i2-ks2,
     & i3-ks3,axisp2,0)*jac3di(i1-ks1-i10,i2-ks2-i20,i3-ks3-i30)))-((
     & rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axisp2,0)*jac3di(i1+2*ks1-i10,
     & i2+2*ks2-i20,i3+2*ks3-i30))-(rsxy(i1-2*ks1,i2-2*ks2,i3-2*ks3,
     & axisp2,0)*jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30))))/(
     & 12.*dta)
                    a32t = (8.*((rsxy(i1+ks1,i2+ks2,i3+ks3,axisp2,1)*
     & jac3di(i1+ks1-i10,i2+ks2-i20,i3+ks3-i30))-(rsxy(i1-ks1,i2-ks2,
     & i3-ks3,axisp2,1)*jac3di(i1-ks1-i10,i2-ks2-i20,i3-ks3-i30)))-((
     & rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axisp2,1)*jac3di(i1+2*ks1-i10,
     & i2+2*ks2-i20,i3+2*ks3-i30))-(rsxy(i1-2*ks1,i2-2*ks2,i3-2*ks3,
     & axisp2,1)*jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30))))/(
     & 12.*dta)
                    a33t = (8.*((rsxy(i1+ks1,i2+ks2,i3+ks3,axisp2,2)*
     & jac3di(i1+ks1-i10,i2+ks2-i20,i3+ks3-i30))-(rsxy(i1-ks1,i2-ks2,
     & i3-ks3,axisp2,2)*jac3di(i1-ks1-i10,i2-ks2-i20,i3-ks3-i30)))-((
     & rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axisp2,2)*jac3di(i1+2*ks1-i10,
     & i2+2*ks2-i20,i3+2*ks3-i30))-(rsxy(i1-2*ks1,i2-2*ks2,i3-2*ks3,
     & axisp2,2)*jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30))))/(
     & 12.*dta)
                    ! *** set RHS for (a1.u).r =  - Ds( a2.uv ) -Dt( a3.uv )
                    Da1DotU = -(  a21s*uex+a22s*uey+a23s*uez + a21*us+
     & a22*vs+a23*ws + a31t*uex+a32t*uey+a33t*uez + a31*ut+a32*vt+a33*
     & wt )
                    ! *** NOTE: "d" denotes the time derivative as in udd = two time derivatives of u
                    ! (a1.Delta u).r = - (a2.utt).s - (a3.utt).t
                    ! (a1.Delta u).r + bf = 0
                    ! bf = bf + (a2.utt).s + (a3.utt).t
                     x00=xy(i1,i2,i3,0)
                     y00=xy(i1,i2,i3,1)
                     z00=xy(i1,i2,i3,2)
                     if( fieldOption.eq.0 )then
                       udd=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x00)+
     & ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0)+ssftt*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))
                       vdd=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x00)+
     & ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1)+ssftt*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))
                       wdd=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x00)+
     & ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2)+ssftt*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))
                     else
                       ! get time derivative (sosup) 
                       udd=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x00)+ky*
     & (y00)+kz*(z00)-cc*(t)))*pwc(0))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))+3.*ssftt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0)
     & +ssfttt*sin(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))
                       vdd=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x00)+ky*
     & (y00)+kz*(z00)-cc*(t)))*pwc(1))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))+3.*ssftt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1)
     & +ssfttt*sin(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))
                       wdd=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x00)+ky*
     & (y00)+kz*(z00)-cc*(t)))*pwc(2))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))+3.*ssftt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2)
     & +ssfttt*sin(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))
                     end if
                     x00=xy(i1+js1,i2+js2,i3+js3,0)
                     y00=xy(i1+js1,i2+js2,i3+js3,1)
                     z00=xy(i1+js1,i2+js2,i3+js3,2)
                     if( fieldOption.eq.0 )then
                       uddp1=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x00)+
     & ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0)+ssftt*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))
                       vddp1=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x00)+
     & ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1)+ssftt*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))
                       wddp1=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x00)+
     & ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2)+ssftt*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))
                     else
                       ! get time derivative (sosup) 
                       uddp1=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x00)+
     & ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))+3.*ssftt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0)
     & +ssfttt*sin(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))
                       vddp1=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x00)+
     & ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))+3.*ssftt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1)
     & +ssfttt*sin(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))
                       wddp1=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x00)+
     & ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))+3.*ssftt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2)
     & +ssfttt*sin(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))
                     end if
                     x00=xy(i1-js1,i2-js2,i3-js3,0)
                     y00=xy(i1-js1,i2-js2,i3-js3,1)
                     z00=xy(i1-js1,i2-js2,i3-js3,2)
                     if( fieldOption.eq.0 )then
                       uddm1=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x00)+
     & ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0)+ssftt*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))
                       vddm1=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x00)+
     & ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1)+ssftt*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))
                       wddm1=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x00)+
     & ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2)+ssftt*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))
                     else
                       ! get time derivative (sosup) 
                       uddm1=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x00)+
     & ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))+3.*ssftt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0)
     & +ssfttt*sin(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))
                       vddm1=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x00)+
     & ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))+3.*ssftt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1)
     & +ssfttt*sin(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))
                       wddm1=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x00)+
     & ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))+3.*ssftt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2)
     & +ssfttt*sin(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))
                     end if
                    ! 2nd-order here should be good enough:
                    udds = (uddp1-uddm1)/(2.*dsa)
                    vdds = (vddp1-vddm1)/(2.*dsa)
                    wdds = (wddp1-wddm1)/(2.*dsa)
                     x00=xy(i1+ks1,i2+ks2,i3+ks3,0)
                     y00=xy(i1+ks1,i2+ks2,i3+ks3,1)
                     z00=xy(i1+ks1,i2+ks2,i3+ks3,2)
                     if( fieldOption.eq.0 )then
                       uddp1=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x00)+
     & ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0)+ssftt*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))
                       vddp1=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x00)+
     & ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1)+ssftt*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))
                       wddp1=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x00)+
     & ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2)+ssftt*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))
                     else
                       ! get time derivative (sosup) 
                       uddp1=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x00)+
     & ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))+3.*ssftt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0)
     & +ssfttt*sin(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))
                       vddp1=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x00)+
     & ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))+3.*ssftt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1)
     & +ssfttt*sin(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))
                       wddp1=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x00)+
     & ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))+3.*ssftt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2)
     & +ssfttt*sin(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))
                     end if
                     x00=xy(i1-ks1,i2-ks2,i3-ks3,0)
                     y00=xy(i1-ks1,i2-ks2,i3-ks3,1)
                     z00=xy(i1-ks1,i2-ks2,i3-ks3,2)
                     if( fieldOption.eq.0 )then
                       uddm1=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x00)+
     & ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0)+ssftt*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))
                       vddm1=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x00)+
     & ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1)+ssftt*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))
                       wddm1=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x00)+
     & ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2)+ssftt*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))
                     else
                       ! get time derivative (sosup) 
                       uddm1=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x00)+
     & ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))+3.*ssftt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0)
     & +ssfttt*sin(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(0))
                       vddm1=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x00)+
     & ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))+3.*ssftt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1)
     & +ssfttt*sin(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(1))
                       wddm1=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x00)+
     & ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))+3.*ssftt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2)
     & +ssfttt*sin(twoPi*(kx*(x00)+ky*(y00)+kz*(z00)-cc*(t)))*pwc(2))
                     end if
                    ! 2nd-order here should be good enough:
                    uddt = (uddp1-uddm1)/(2.*dta)
                    vddt = (vddp1-vddm1)/(2.*dta)
                    wddt = (wddp1-wddm1)/(2.*dta)
                    bf = bf + a21s*udd+a22s*vdd+a23s*wdd + a21*udds + 
     & a22*vdds+ a23*wdds + a31t*udd+a32t*vdd+a33t*wdd + a31*uddt + 
     & a32*vddt+ a33*wddt
                  end if
                  if( useForcing.ne.0 )then
                   ! OGF3D(i1,i2,i3,t, uv0(0),uv0(1),uv0(2))
                   ! OGF3D(i1-is1,i2-is2,i3-is3,t, uvm(0),uvm(1),uvm(2))
                   ! OGF3D(i1+is1,i2+is2,i3+is3,t, uvp(0),uvp(1),uvp(2))
                   ! OGF3D(i1-2*is1,i2-2*is2,i3-2*is3,t, uvm2(0),uvm2(1),uvm2(2))
                   ! OGF3D(i1+2*is1,i2+2*is2,i3+2*is3,t, uvp2(0),uvp2(1),uvp2(2))
                     call ogDeriv3(ep, 0,2,0,0, xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),xy(i1,i2,i3,2),t, ex,uxx, ey,vxx, ez,wxx)
                     call ogDeriv3(ep, 0,0,2,0, xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),xy(i1,i2,i3,2),t, ex,uyy, ey,vyy, ez,wyy)
                     call ogDeriv3(ep, 0,0,0,2, xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),xy(i1,i2,i3,2),t, ex,uzz, ey,vzz, ez,wzz)
                     call ogDeriv3(ep, 0,2,0,0, xy(i1-is1,i2-is2,i3-
     & is3,0),xy(i1-is1,i2-is2,i3-is3,1),xy(i1-is1,i2-is2,i3-is3,2),t,
     &  ex,uxxm1, ey,vxxm1, ez,wxxm1)
                     call ogDeriv3(ep, 0,0,2,0, xy(i1-is1,i2-is2,i3-
     & is3,0),xy(i1-is1,i2-is2,i3-is3,1),xy(i1-is1,i2-is2,i3-is3,2),t,
     &  ex,uyym1, ey,vyym1, ez,wyym1)
                     call ogDeriv3(ep, 0,0,0,2, xy(i1-is1,i2-is2,i3-
     & is3,0),xy(i1-is1,i2-is2,i3-is3,1),xy(i1-is1,i2-is2,i3-is3,2),t,
     &  ex,uzzm1, ey,vzzm1, ez,wzzm1)
                     call ogDeriv3(ep, 0,2,0,0, xy(i1+is1,i2+is2,i3+
     & is3,0),xy(i1+is1,i2+is2,i3+is3,1),xy(i1+is1,i2+is2,i3+is3,2),t,
     &  ex,uxxp1, ey,vxxp1, ez,wxxp1)
                     call ogDeriv3(ep, 0,0,2,0, xy(i1+is1,i2+is2,i3+
     & is3,0),xy(i1+is1,i2+is2,i3+is3,1),xy(i1+is1,i2+is2,i3+is3,2),t,
     &  ex,uyyp1, ey,vyyp1, ez,wyyp1)
                     call ogDeriv3(ep, 0,0,0,2, xy(i1+is1,i2+is2,i3+
     & is3,0),xy(i1+is1,i2+is2,i3+is3,1),xy(i1+is1,i2+is2,i3+is3,2),t,
     &  ex,uzzp1, ey,vzzp1, ez,wzzp1)
                     call ogDeriv3(ep, 0,2,0,0, xy(i1-2*is1,i2-2*is2,
     & i3-2*is3,0),xy(i1-2*is1,i2-2*is2,i3-2*is3,1),xy(i1-2*is1,i2-2*
     & is2,i3-2*is3,2),t, ex,uxxm2, ey,vxxm2, ez,wxxm2)
                     call ogDeriv3(ep, 0,0,2,0, xy(i1-2*is1,i2-2*is2,
     & i3-2*is3,0),xy(i1-2*is1,i2-2*is2,i3-2*is3,1),xy(i1-2*is1,i2-2*
     & is2,i3-2*is3,2),t, ex,uyym2, ey,vyym2, ez,wyym2)
                     call ogDeriv3(ep, 0,0,0,2, xy(i1-2*is1,i2-2*is2,
     & i3-2*is3,0),xy(i1-2*is1,i2-2*is2,i3-2*is3,1),xy(i1-2*is1,i2-2*
     & is2,i3-2*is3,2),t, ex,uzzm2, ey,vzzm2, ez,wzzm2)
                     call ogDeriv3(ep, 0,2,0,0, xy(i1+2*is1,i2+2*is2,
     & i3+2*is3,0),xy(i1+2*is1,i2+2*is2,i3+2*is3,1),xy(i1+2*is1,i2+2*
     & is2,i3+2*is3,2),t, ex,uxxp2, ey,vxxp2, ez,wxxp2)
                     call ogDeriv3(ep, 0,0,2,0, xy(i1+2*is1,i2+2*is2,
     & i3+2*is3,0),xy(i1+2*is1,i2+2*is2,i3+2*is3,1),xy(i1+2*is1,i2+2*
     & is2,i3+2*is3,2),t, ex,uyyp2, ey,vyyp2, ez,wyyp2)
                     call ogDeriv3(ep, 0,0,0,2, xy(i1+2*is1,i2+2*is2,
     & i3+2*is3,0),xy(i1+2*is1,i2+2*is2,i3+2*is3,1),xy(i1+2*is1,i2+2*
     & is2,i3+2*is3,2),t, ex,uzzp2, ey,vzzp2, ez,wzzp2)
                   utt00=uxx+uyy+uzz
                   vtt00=vxx+vyy+vzz
                   wtt00=wxx+wyy+wzz
                   ! For TZ choose bf = bf - Dr( a1.Delta uvExact )
                   bf = bf - a11r*utt00 - a12r*vtt00 - a13r*wtt00 -a11*
     & ( 8.*((uxxp1+uyyp1+uzzp1)-(uxxm1+uyym1+uzzm1))-((uxxp2+uyyp2+
     & uzzp2)-(uxxm2+uyym2+uzzm2)) )/(12.*dra) -a12*( 8.*((vxxp1+
     & vyyp1+vzzp1)-(vxxm1+vyym1+vzzm1))-((vxxp2+vyyp2+vzzp2)-(vxxm2+
     & vyym2+vzzm2)) )/(12.*dra) -a13*( 8.*((wxxp1+wyyp1+wzzp1)-(
     & wxxm1+wyym1+wzzm1))-((wxxp2+wyyp2+wzzp2)-(wxxm2+wyym2+wzzm2)) )
     & /(12.*dra)
                   ! For testing we could set
                   !    bf = a1.( c11*urrr + c11r*urr + c1*urr + c1r*ur ) + a1r.( c11*urr + c1*ur )
                   ! Da1DotU = (a1.uv).r to 4th order
                   ! Da1DotU = (8.*( (a11p1*uvp(0) +a12p1*uvp(1))  - (a11m1*uvm(0) +a12m1*uvm(1)) )!             - ( (a11p2*uvp2(0)+a12p2*uvp2(1)) - (a11m2*uvm2(0)+a12m2*uvm2(1)) ) )/(12.*dra)
                   ! **** compute RHS for div(u) equation ****
                   a21zp1= (rsxy(i1+js1,i2+js2,i3+js3,axisp1,0)*jac3di(
     & i1+js1-i10,i2+js2-i20,i3+js3-i30))
                   a21zm1= (rsxy(i1-js1,i2-js2,i3-js3,axisp1,0)*jac3di(
     & i1-js1-i10,i2-js2-i20,i3-js3-i30))
                   a21zp2= (rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,0)*
     & jac3di(i1+2*js1-i10,i2+2*js2-i20,i3+2*js3-i30))
                   a21zm2= (rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axisp1,0)*
     & jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))
                   a22zp1= (rsxy(i1+js1,i2+js2,i3+js3,axisp1,1)*jac3di(
     & i1+js1-i10,i2+js2-i20,i3+js3-i30))
                   a22zm1= (rsxy(i1-js1,i2-js2,i3-js3,axisp1,1)*jac3di(
     & i1-js1-i10,i2-js2-i20,i3-js3-i30))
                   a22zp2= (rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,1)*
     & jac3di(i1+2*js1-i10,i2+2*js2-i20,i3+2*js3-i30))
                   a22zm2= (rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axisp1,1)*
     & jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))
                   a23zp1= (rsxy(i1+js1,i2+js2,i3+js3,axisp1,2)*jac3di(
     & i1+js1-i10,i2+js2-i20,i3+js3-i30))
                   a23zm1= (rsxy(i1-js1,i2-js2,i3-js3,axisp1,2)*jac3di(
     & i1-js1-i10,i2-js2-i20,i3-js3-i30))
                   a23zp2= (rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,2)*
     & jac3di(i1+2*js1-i10,i2+2*js2-i20,i3+2*js3-i30))
                   a23zm2= (rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axisp1,2)*
     & jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))
                   a31zp1= (rsxy(i1+ks1,i2+ks2,i3+ks3,axisp2,0)*jac3di(
     & i1+ks1-i10,i2+ks2-i20,i3+ks3-i30))
                   a31zm1= (rsxy(i1-ks1,i2-ks2,i3-ks3,axisp2,0)*jac3di(
     & i1-ks1-i10,i2-ks2-i20,i3-ks3-i30))
                   a31zp2= (rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axisp2,0)*
     & jac3di(i1+2*ks1-i10,i2+2*ks2-i20,i3+2*ks3-i30))
                   a31zm2= (rsxy(i1-2*ks1,i2-2*ks2,i3-2*ks3,axisp2,0)*
     & jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30))
                   a32zp1= (rsxy(i1+ks1,i2+ks2,i3+ks3,axisp2,1)*jac3di(
     & i1+ks1-i10,i2+ks2-i20,i3+ks3-i30))
                   a32zm1= (rsxy(i1-ks1,i2-ks2,i3-ks3,axisp2,1)*jac3di(
     & i1-ks1-i10,i2-ks2-i20,i3-ks3-i30))
                   a32zp2= (rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axisp2,1)*
     & jac3di(i1+2*ks1-i10,i2+2*ks2-i20,i3+2*ks3-i30))
                   a32zm2= (rsxy(i1-2*ks1,i2-2*ks2,i3-2*ks3,axisp2,1)*
     & jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30))
                   a33zp1= (rsxy(i1+ks1,i2+ks2,i3+ks3,axisp2,2)*jac3di(
     & i1+ks1-i10,i2+ks2-i20,i3+ks3-i30))
                   a33zm1= (rsxy(i1-ks1,i2-ks2,i3-ks3,axisp2,2)*jac3di(
     & i1-ks1-i10,i2-ks2-i20,i3-ks3-i30))
                   a33zp2= (rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axisp2,2)*
     & jac3di(i1+2*ks1-i10,i2+2*ks2-i20,i3+2*ks3-i30))
                   a33zm2= (rsxy(i1-2*ks1,i2-2*ks2,i3-2*ks3,axisp2,2)*
     & jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30))
                   ! *** set to - Ds( a2.uv ) -Dt( a3.uv )
                   Da1DotU = -(  ( 8.*(a21zp1*u(i1+  js1,i2+  js2,i3+  
     & js3,ex)-a21zm1*u(i1-  js1,i2-  js2,i3-  js3,ex)) -(a21zp2*u(i1+
     & 2*js1,i2+2*js2,i3+2*js3,ex)-a21zm2*u(i1-2*js1,i2-2*js2,i3-2*
     & js3,ex)) )/(12.*dsa) +( 8.*(a22zp1*u(i1+  js1,i2+  js2,i3+  
     & js3,ey)-a22zm1*u(i1-  js1,i2-  js2,i3-  js3,ey)) -(a22zp2*u(i1+
     & 2*js1,i2+2*js2,i3+2*js3,ey)-a22zm2*u(i1-2*js1,i2-2*js2,i3-2*
     & js3,ey)) )/(12.*dsa) +( 8.*(a23zp1*u(i1+  js1,i2+  js2,i3+  
     & js3,ez)-a23zm1*u(i1-  js1,i2-  js2,i3-  js3,ez)) -(a23zp2*u(i1+
     & 2*js1,i2+2*js2,i3+2*js3,ez)-a23zm2*u(i1-2*js1,i2-2*js2,i3-2*
     & js3,ez)) )/(12.*dsa)  ) -(  ( 8.*(a31zp1*u(i1+  ks1,i2+  ks2,
     & i3+  ks3,ex)-a31zm1*u(i1-  ks1,i2-  ks2,i3-  ks3,ex)) -(a31zp2*
     & u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ex)-a31zm2*u(i1-2*ks1,i2-2*ks2,i3-
     & 2*ks3,ex)) )/(12.*dta) +( 8.*(a32zp1*u(i1+  ks1,i2+  ks2,i3+  
     & ks3,ey)-a32zm1*u(i1-  ks1,i2-  ks2,i3-  ks3,ey)) -(a32zp2*u(i1+
     & 2*ks1,i2+2*ks2,i3+2*ks3,ey)-a32zm2*u(i1-2*ks1,i2-2*ks2,i3-2*
     & ks3,ey)) )/(12.*dta) +( 8.*(a33zp1*u(i1+  ks1,i2+  ks2,i3+  
     & ks3,ez)-a33zm1*u(i1-  ks1,i2-  ks2,i3-  ks3,ez)) -(a33zp2*u(i1+
     & 2*ks1,i2+2*ks2,i3+2*ks3,ez)-a33zm2*u(i1-2*ks1,i2-2*ks2,i3-2*
     & ks3,ez)) )/(12.*dta)  )
                  end if
                 ! Now assign E at the ghost points:


! ************ Results from bc43d.maple *******************
      b3u=a11*c11
      b3v=a12*c11
      b3w=a13*c11
      b2u=a11*(c1+c11r)+a11r*c11
      b2v=a12*(c1+c11r)+a12r*c11
      b2w=a13*(c1+c11r)+a13r*c11
      b1u=a11*c1r+a11r*c1
      b1v=a12*c1r+a12r*c1
      b1w=a13*c1r+a13r*c1
      ttu11=tau11*u(i1-is1,i2-is2,i3-is3,ex)+tau12*u(i1-is1,i2-is2,i3-
     & is3,ey)+tau13*u(i1-is1,i2-is2,i3-is3,ez)
      ttu12=tau11*u(i1-2*is1,i2-2*is2,i3-2*is3,ex)+tau12*u(i1-2*is1,i2-
     & 2*is2,i3-2*is3,ey)+tau13*u(i1-2*is1,i2-2*is2,i3-2*is3,ez)
      ttu21=tau21*u(i1-is1,i2-is2,i3-is3,ex)+tau22*u(i1-is1,i2-is2,i3-
     & is3,ey)+tau23*u(i1-is1,i2-is2,i3-is3,ez)
      ttu22=tau21*u(i1-2*is1,i2-2*is2,i3-2*is3,ex)+tau22*u(i1-2*is1,i2-
     & 2*is2,i3-2*is3,ey)+tau23*u(i1-2*is1,i2-2*is2,i3-2*is3,ez)
      ! *********** set tangential components to be exact *****
      ! OGF3D(i1-is1,i2-is2,i3-is3,t, uvm(0),uvm(1),uvm(2))
      ! ttu11=tau11*uvm(0)+tau12*uvm(1)+tau13*uvm(2)
      ! ttu21=tau21*uvm(0)+tau22*uvm(1)+tau23*uvm(2)
      ! OGF3D(i1-2*is1,i2-2*is2,i3-2*is3,t, uvm(0),uvm(1),uvm(2))
      ! ttu12=tau11*uvm(0)+tau12*uvm(1)+tau13*uvm(2)
      ! ttu22=tau21*uvm(0)+tau22*uvm(1)+tau23*uvm(2)
      ! ******************************************************

      f1um2=-1/2.*b3u/dra**3-1/12.*b2u/dra**2+1/12.*b1u/dra
      f1um1=b3u/dra**3+4/3.*b2u/dra**2-2/3.*b1u/dra
      f1vm2=-1/2.*b3v/dra**3-1/12.*b2v/dra**2+1/12.*b1v/dra
      f1vm1=b3v/dra**3+4/3.*b2v/dra**2-2/3.*b1v/dra
      f1wm2=-1/2.*b3w/dra**3-1/12.*b2w/dra**2+1/12.*b1w/dra
      f1wm1=b3w/dra**3+4/3.*b2w/dra**2-2/3.*b1w/dra
      f1f  =-1/12.*(-6*b3u*u(i1+2*is1,i2+2*is2,i3+2*is3,ex)+12*b3u*u(
     & i1+is1,i2+is2,i3+is3,ex)-6*b3v*u(i1+2*is1,i2+2*is2,i3+2*is3,ey)
     & +12*b3v*u(i1+is1,i2+is2,i3+is3,ey)-6*b3w*u(i1+2*is1,i2+2*is2,
     & i3+2*is3,ez)+12*b3w*u(i1+is1,i2+is2,i3+is3,ez)-8*b1u*dra**2*u(
     & i1+is1,i2+is2,i3+is3,ex)-8*b1v*dra**2*u(i1+is1,i2+is2,i3+is3,
     & ey)-16*b2u*dra*u(i1+is1,i2+is2,i3+is3,ex)-12*bf*dra**3+b1w*dra*
     & *2*u(i1+2*is1,i2+2*is2,i3+2*is3,ez)-8*b1w*dra**2*u(i1+is1,i2+
     & is2,i3+is3,ez)+b1u*dra**2*u(i1+2*is1,i2+2*is2,i3+2*is3,ex)+b1v*
     & dra**2*u(i1+2*is1,i2+2*is2,i3+2*is3,ey)+b2u*dra*u(i1+2*is1,i2+
     & 2*is2,i3+2*is3,ex)+30*b2u*dra*u(i1,i2,i3,ex)+b2v*dra*u(i1+2*
     & is1,i2+2*is2,i3+2*is3,ey)+30*b2v*dra*u(i1,i2,i3,ey)-16*b2v*dra*
     & u(i1+is1,i2+is2,i3+is3,ey)+b2w*dra*u(i1+2*is1,i2+2*is2,i3+2*
     & is3,ez)+30*b2w*dra*u(i1,i2,i3,ez)-16*b2w*dra*u(i1+is1,i2+is2,
     & i3+is3,ez))/dra**3

      f2um2=1/12.*a11m2
      f2um1=-2/3.*a11m1
      f2vm2=1/12.*a12m2
      f2vm1=-2/3.*a12m1
      f2wm2=1/12.*a13m2
      f2wm1=-2/3.*a13m1
      f2f  =2/3.*a11p1*u(i1+is1,i2+is2,i3+is3,ex)+2/3.*a12p1*u(i1+is1,
     & i2+is2,i3+is3,ey)+2/3.*a13p1*u(i1+is1,i2+is2,i3+is3,ez)-1/12.*
     & a11p2*u(i1+2*is1,i2+2*is2,i3+2*is3,ex)-1/12.*a12p2*u(i1+2*is1,
     & i2+2*is2,i3+2*is3,ey)-1/12.*a13p2*u(i1+2*is1,i2+2*is2,i3+2*is3,
     & ez)-Da1DotU*dra

      u(i1-is1,i2-is2,i3-is3,ex) = (-2*tau13*f1f*tau22*f2um2*tau23*
     & tau12-tau13**2*f1f*tau22*f2vm2*tau21-tau13**2*f1um2*tau22**2*
     & f2f-f1um2*tau23**2*tau12**2*f2f+f1vm2*tau23**2*ttu12*f2um2*
     & tau12-f1vm2*tau23*ttu12*tau13*f2um2*tau22-f1vm2*tau23*ttu12*
     & f2wm2*tau12*tau21+f1wm1*tau12*ttu21*tau13*f2vm2*tau21-f1wm1*
     & tau12*ttu21*tau13*f2um2*tau22-f1wm1*tau12**2*ttu21*f2wm2*tau21+
     & f1wm2*tau12**2*ttu22*f2um2*tau23-f1wm2*tau12*ttu22*f2vm2*tau23*
     & tau11-f1wm2*tau12*ttu22*tau13*f2um2*tau22+f1wm1*tau12*ttu21*
     & f2wm2*tau11*tau22+f1wm1*tau12**2*ttu21*f2um2*tau23+tau13*ttu21*
     & f1vm1*f2wm2*tau12*tau21+f1vm1*tau23*ttu11*f2wm2*tau11*tau22+
     & f1vm1*tau23**2*ttu11*f2um2*tau12-f1vm1*tau23**2*ttu11*f2vm2*
     & tau11+f1vm1*tau23*ttu11*tau13*f2vm2*tau21-f1vm1*tau23*ttu11*
     & tau13*f2um2*tau22-f1vm1*tau23*ttu11*f2wm2*tau12*tau21-f1wm1*
     & ttu11*tau22**2*f2wm2*tau11-f1wm1*ttu11*tau22*f2um2*tau23*tau12+
     & f1wm1*ttu11*tau22*f2vm2*tau23*tau11-f1wm1*ttu11*tau22*tau13*
     & f2vm2*tau21+f1wm1*ttu11*tau22**2*tau13*f2um2+f1wm1*ttu11*tau22*
     & f2wm2*tau12*tau21+f1f*tau23*tau12*f2wm2*tau11*tau22-f1f*tau23**
     & 2*tau12*f2vm2*tau11+f1f*tau23*tau12*tau13*f2vm2*tau21-f1f*
     & tau23*tau12**2*f2wm2*tau21-f1wm2*ttu12*tau22*f2um2*tau23*tau12-
     & f1wm2*ttu12*tau22*tau13*f2vm2*tau21+f1wm2*ttu12*tau22**2*tau13*
     & f2um2+tau13**2*ttu21*f1vm1*f2um2*tau22-f1wm1*tau12*ttu21*f2vm2*
     & tau23*tau11-tau13*f1um2*tau22**2*f2wm1*ttu11+tau13*f1um2*tau22*
     & f2vm1*tau23*ttu11-tau13**2*f1um2*tau22*f2vm2*ttu22-tau13**2*
     & f1um2*tau22*ttu21*f2vm1+tau13*f1um2*tau22*f2vm2*tau23*ttu12+
     & tau13*f1um2*tau22*f2wm2*tau12*ttu22+2*tau13*f1um2*tau22*f2f*
     & tau23*tau12-tau13*f1um2*tau22**2*f2wm2*ttu12+tau13*f1um2*tau22*
     & f2wm1*tau12*ttu21+tau13*f1vm2*tau21*f2wm1*ttu11*tau22-tau13*
     & f1vm2*tau21*f2vm1*tau23*ttu11+tau13**2*f1vm2*tau21*f2f*tau22+
     & tau13**2*f1vm2*tau21*ttu21*f2vm1-tau13*f1vm2*tau21*f2f*tau23*
     & tau12+tau13*f1vm2*tau21*f2wm2*ttu12*tau22-tau13*f1vm2*tau21*
     & f2wm1*tau12*ttu21+tau13*f1f*tau22*f2wm2*tau12*tau21-tau13*
     & f1vm2*ttu22*f2wm2*tau11*tau22-tau13*f1vm2*ttu22*f2um2*tau23*
     & tau12-tau13*f1f*tau22**2*f2wm2*tau11+tau13**2*f1vm2*ttu22*
     & f2um2*tau22-tau13*ttu21*f1vm1*f2wm2*tau11*tau22-tau13*ttu21*
     & f1vm1*f2um2*tau23*tau12+tau13*ttu21*f1vm1*f2vm2*tau23*tau11-
     & tau13**2*ttu21*f1vm1*f2vm2*tau21+f1f*tau23**2*tau12**2*f2um2+
     & tau13**2*f1f*tau22**2*f2um2+tau13*f1f*tau22*f2vm2*tau23*tau11-
     & f1wm2*tau11*tau22*f2vm1*tau23*ttu11+f1wm2*tau11*tau22*tau13*
     & f2vm2*ttu22+f1wm2*tau11*tau22**2*tau13*f2f+f1wm2*tau11*tau22*
     & tau13*ttu21*f2vm1-f1wm2*tau11*tau22*f2f*tau23*tau12-f1wm2*
     & tau11*tau22*f2wm1*tau12*ttu21-f1wm2*tau12*tau21*f2wm1*ttu11*
     & tau22+f1wm2*tau12*tau21*f2vm1*tau23*ttu11-f1wm2*tau12*tau21*
     & tau13*f2f*tau22-f1wm2*tau12*tau21*tau13*ttu21*f2vm1+f1wm2*
     & tau12*tau21*f2vm2*tau23*ttu12+f1wm2*tau12**2*tau21*f2f*tau23+
     & f1wm2*tau12**2*tau21*f2wm1*ttu21-f1vm2*tau23*tau11*tau13*f2f*
     & tau22-f1vm2*tau23*tau11*tau13*ttu21*f2vm1+f1vm2*tau23*tau11*
     & f2wm2*tau12*ttu22+f1vm2*tau23**2*tau11*f2f*tau12+f1vm2*tau23*
     & tau11*f2wm1*tau12*ttu21+f1wm2*tau11*tau22**2*f2wm1*ttu11+f1um2*
     & tau23*tau12*f2wm1*ttu11*tau22-f1um2*tau23**2*tau12*f2vm1*ttu11+
     & f1um2*tau23*tau12*tau13*f2vm2*ttu22+f1um2*tau23*tau12*tau13*
     & ttu21*f2vm1-f1um2*tau23**2*tau12*f2vm2*ttu12-f1um2*tau23*tau12*
     & *2*f2wm2*ttu22+f1um2*tau23*tau12*f2wm2*ttu12*tau22-f1um2*tau23*
     & tau12**2*f2wm1*ttu21-f1vm2*tau23*tau11*f2wm1*ttu11*tau22+f1vm2*
     & tau23**2*tau11*f2vm1*ttu11)/(-f1um1*tau23**2*tau12**2*f2um2-
     & f1wm2*tau12**2*tau21*f2um1*tau23-f1vm2*tau23*tau11**2*f2wm1*
     & tau22+f1vm2*tau23*tau11*tau13*f2um1*tau22-f1wm2*tau11*tau22**2*
     & tau13*f2um1+f1vm2*tau23*tau11*f2wm1*tau12*tau21-f1vm2*tau23**2*
     & tau11*f2um1*tau12+f1um2*tau23*tau12*tau13*tau21*f2vm1+f1um2*
     & tau23*tau12*f2wm1*tau11*tau22-f1um2*tau23**2*tau12*f2vm1*tau11-
     & f1um2*tau23*tau12**2*f2wm1*tau21+f1um2*tau23**2*tau12**2*f2um1-
     & 2*f1wm2*tau11*tau22*f2wm1*tau12*tau21+f1wm2*tau11*tau22*f2um1*
     & tau23*tau12+f1wm2*tau12*tau21*tau13*f2um1*tau22-f1wm2*tau12*
     & tau21**2*tau13*f2vm1+f1wm2*tau12*tau21*f2vm1*tau23*tau11+f1wm2*
     & tau12**2*tau21**2*f2wm1+f1vm2*tau23**2*tau11**2*f2vm1+tau13*
     & f1um2*tau22*f2vm1*tau23*tau11+tau13*f1um2*tau22*f2wm1*tau12*
     & tau21-2*tau13*f1um2*tau22*f2um1*tau23*tau12-tau13**2*f1vm2*
     & tau21*f2um1*tau22+tau13**2*tau21*f1vm1*f2um2*tau22+tau13*tau21*
     & *2*f1vm1*f2wm2*tau12+f1wm2*tau11*tau22*tau13*tau21*f2vm1+f1wm2*
     & tau11**2*tau22**2*f2wm1-f1wm2*tau11**2*tau22*f2vm1*tau23+tau13*
     & f1vm2*tau21*f2wm1*tau11*tau22-2*tau13*f1vm2*tau21*f2vm1*tau23*
     & tau11-tau13*f1vm2*tau21**2*f2wm1*tau12+tau13*f1vm2*tau21*f2um1*
     & tau23*tau12+tau13*f1um1*tau22**2*f2wm2*tau11+2*tau13*f1um1*
     & tau22*f2um2*tau23*tau12-tau13*f1um1*tau22*f2vm2*tau23*tau11+
     & tau13**2*f1um1*tau22*f2vm2*tau21-tau13**2*f1um1*tau22**2*f2um2-
     & tau13*f1um1*tau22*f2wm2*tau12*tau21-tau13*tau21*f1vm1*f2wm2*
     & tau11*tau22-tau13*tau21*f1vm1*f2um2*tau23*tau12+2*tau13*tau21*
     & f1vm1*f2vm2*tau23*tau11-tau13**2*tau21**2*f1vm1*f2vm2-f1vm1*
     & tau23**2*tau11**2*f2vm2-f1vm1*tau23*tau11*tau13*f2um2*tau22-
     & f1vm1*tau23*tau11*f2wm2*tau12*tau21+tau13**2*f1um2*tau22**2*
     & f2um1+tau13**2*f1vm2*tau21**2*f2vm1+f1wm1*tau12*tau21**2*tau13*
     & f2vm2-f1wm1*tau12*tau21*tau13*f2um2*tau22+2*f1wm1*tau12*tau21*
     & f2wm2*tau11*tau22+f1wm1*tau12**2*tau21*f2um2*tau23-tau13**2*
     & f1um2*tau22*tau21*f2vm1-f1um1*tau23*tau12*f2wm2*tau11*tau22-
     & f1um1*tau23*tau12*tau13*f2vm2*tau21+f1um1*tau23*tau12**2*f2wm2*
     & tau21+f1wm1*tau11**2*tau22*f2vm2*tau23-f1wm1*tau11**2*tau22**2*
     & f2wm2+f1um1*tau23**2*tau12*f2vm2*tau11-f1wm1*tau11*tau22*f2um2*
     & tau23*tau12+f1vm1*tau23**2*tau11*f2um2*tau12-f1wm1*tau11*tau22*
     & tau13*f2vm2*tau21+f1wm1*tau11*tau22**2*tau13*f2um2-f1wm1*tau12*
     & tau21*f2vm2*tau23*tau11-f1wm1*tau12**2*tau21**2*f2wm2+f1vm1*
     & tau23*tau11**2*f2wm2*tau22-tau13*f1um2*tau22**2*f2wm1*tau11)

      u(i1-2*is1,i2-2*is2,i3-2*is3,ex) = (tau21*tau13*ttu22*f1vm1*
     & f2wm2*tau12+tau13*f2um1*tau22*f1wm1*tau12*ttu21+tau13*ttu22*
     & f1wm2*tau12*f2um1*tau22-tau13*f2vm1*tau23*ttu11*f1um1*tau22+
     & tau13*ttu22*f1vm2*f2um1*tau23*tau12+tau13*ttu22*f1vm2*f2wm1*
     & tau11*tau22-tau13*ttu22*f1vm2*f2vm1*tau23*tau11+2*tau13*f2um1*
     & tau23*tau12*f1f*tau22+tau13*f2f*tau22*f1vm1*tau23*tau11+tau13*
     & f2um1*tau22*f1vm1*tau23*ttu11-2*tau13*f2f*tau23*tau12*f1um1*
     & tau22-tau13*f2vm1*tau23*tau11*f1f*tau22+tau13*f2wm1*tau11*
     & tau22*ttu21*f1vm1+tau13*f2wm2*ttu12*tau22**2*f1um1-tau13*f2um1*
     & tau22**2*f1wm1*ttu11-tau13*ttu21*f2vm1*f1um1*tau23*tau12-tau13*
     & f2f*tau22**2*f1wm1*tau11-tau13*f2vm2*tau23*ttu12*f1um1*tau22+
     & tau13**2*ttu22*f1um1*tau22*f2vm2+tau13**2*ttu21*f2vm1*f1um1*
     & tau22+tau13**2*f2f*tau22**2*f1um1-tau13**2*ttu22*f1vm2*f2um1*
     & tau22-tau13**2*f2um1*tau22*ttu21*f1vm1-tau13*ttu22*f1um1*tau22*
     & f2wm2*tau12+tau23**2*ttu12*f1vm2*f2vm1*tau11-tau23**2*ttu12*
     & f1vm2*f2um1*tau12+f2wm1*tau12**2*ttu21*f1um1*tau23-f2wm2*tau12*
     & tau23*ttu12*f1um1*tau22+f2vm1*tau23*tau11*f1wm1*tau12*ttu21+
     & f2vm2*tau23*ttu12*f1wm1*tau11*tau22-f2vm2*tau23**2*ttu12*f1vm1*
     & tau11+f2vm2*tau23**2*ttu12*f1um1*tau12-f2vm1*tau23*tau11*f1wm2*
     & ttu12*tau22+f2vm1*tau23**2*tau11*f1f*tau12+f2um1*tau23*tau12*
     & f1wm1*ttu11*tau22-f2um1*tau23**2*tau12*f1vm1*ttu11-f2wm1*tau12*
     & f1wm2*tau11*tau22*ttu22-f2um1*tau23**2*tau12**2*f1f-tau13**2*
     & f2um1*tau22**2*f1f+f2vm1*tau23**2*ttu11*f1um1*tau12-f2um1*
     & tau23*tau12**2*f1wm1*ttu21-ttu22*f1wm2*tau12**2*f2um1*tau23+
     & f2um1*tau23*tau12*f1wm2*ttu12*tau22+ttu22*f1wm1*tau12*f2wm2*
     & tau11*tau22-tau23*ttu12*f1vm2*f2wm1*tau11*tau22+f2vm1*f1wm2*
     & tau12*ttu22*tau23*tau11-f2wm1*tau12*f1f*tau22*tau23*tau11+f2f*
     & tau22*f1wm1*tau12*tau23*tau11-f2wm1*ttu11*tau22*f1um1*tau23*
     & tau12-f2f*tau23**2*tau12*f1vm1*tau11+f2wm2*ttu12*tau22*f1vm1*
     & tau23*tau11-ttu22*f1vm1*tau23*tau11*f2wm2*tau12-f2wm1*tau12*
     & ttu21*f1vm1*tau23*tau11+f2wm1*tau11*tau22**2*f1wm2*ttu12+ttu22*
     & f1um1*tau23*tau12**2*f2wm2-f2wm2*ttu12*tau22**2*f1wm1*tau11+
     & tau13*f2vm2*ttu22*f1vm1*tau23*tau11-tau13*ttu21*f2vm1*f1wm1*
     & tau11*tau22+tau13*f2um1*tau23*tau12*ttu21*f1vm1-tau13*f2vm2*
     & ttu22*f1wm1*tau11*tau22-tau13*f2vm2*ttu22*f1um1*tau23*tau12+
     & tau13*tau23*ttu12*f1vm2*f2um1*tau22-tau13*f2wm1*tau12*ttu21*
     & f1um1*tau22+tau13*f2wm1*ttu11*tau22**2*f1um1+tau13*f2wm1*tau11*
     & tau22**2*f1f-tau13*f2um1*tau22**2*f1wm2*ttu12-tau21*ttu22*
     & f1wm1*tau12**2*f2wm2+tau21*ttu22*f1wm2*tau12**2*f2wm1+tau21*
     & f2wm1*tau12**2*f1f*tau23+f2f*tau23**2*tau12**2*f1um1+tau21*
     & tau13*f2vm1*f1wm2*ttu12*tau22+tau21*tau13*f2vm2*ttu22*f1wm1*
     & tau12-tau21*tau13*f2wm1*ttu11*tau22*f1vm1-tau21*tau13*f2wm1*
     & tau12*f1f*tau22+tau21*tau13*f2vm1*f1wm1*ttu11*tau22+tau21*
     & tau13*f2f*tau22*f1wm1*tau12+tau21*tau13*f2f*tau23*tau12*f1vm1-
     & tau21*tau13*ttu22*f1wm2*tau12*f2vm1-tau21*tau13*f2wm2*ttu12*
     & tau22*f1vm1-tau21*tau13*f2vm1*f1f*tau23*tau12-tau21*tau13*
     & ttu22*f1vm2*f2wm1*tau12-tau21*tau23*ttu12*f1wm1*tau12*f2vm2+
     & tau21*f2wm2*ttu12*tau22*f1wm1*tau12+tau21*tau23*ttu12*f1vm2*
     & f2wm1*tau12-tau21*f2wm1*tau12*f1wm2*ttu12*tau22+tau21*f2wm1*
     & tau12*f1vm1*tau23*ttu11+tau21*tau13*f2vm2*tau23*ttu12*f1vm1-
     & tau21*tau13**2*f2f*tau22*f1vm1+tau21*tau13**2*ttu22*f1vm2*
     & f2vm1+tau21*tau13**2*f2vm1*f1f*tau22-tau21*tau13**2*ttu22*
     & f1vm1*f2vm2-tau21*f2vm1*tau23*ttu11*f1wm1*tau12-tau21*tau13*
     & tau23*ttu12*f1vm2*f2vm1-tau21*f2f*tau23*tau12**2*f1wm1)/(-
     & f1um1*tau23**2*tau12**2*f2um2-f1wm2*tau12**2*tau21*f2um1*tau23-
     & f1vm2*tau23*tau11**2*f2wm1*tau22+f1vm2*tau23*tau11*tau13*f2um1*
     & tau22-f1wm2*tau11*tau22**2*tau13*f2um1+f1vm2*tau23*tau11*f2wm1*
     & tau12*tau21-f1vm2*tau23**2*tau11*f2um1*tau12+f1um2*tau23*tau12*
     & tau13*tau21*f2vm1+f1um2*tau23*tau12*f2wm1*tau11*tau22-f1um2*
     & tau23**2*tau12*f2vm1*tau11-f1um2*tau23*tau12**2*f2wm1*tau21+
     & f1um2*tau23**2*tau12**2*f2um1-2*f1wm2*tau11*tau22*f2wm1*tau12*
     & tau21+f1wm2*tau11*tau22*f2um1*tau23*tau12+f1wm2*tau12*tau21*
     & tau13*f2um1*tau22-f1wm2*tau12*tau21**2*tau13*f2vm1+f1wm2*tau12*
     & tau21*f2vm1*tau23*tau11+f1wm2*tau12**2*tau21**2*f2wm1+f1vm2*
     & tau23**2*tau11**2*f2vm1+tau13*f1um2*tau22*f2vm1*tau23*tau11+
     & tau13*f1um2*tau22*f2wm1*tau12*tau21-2*tau13*f1um2*tau22*f2um1*
     & tau23*tau12-tau13**2*f1vm2*tau21*f2um1*tau22+tau13**2*tau21*
     & f1vm1*f2um2*tau22+tau13*tau21**2*f1vm1*f2wm2*tau12+f1wm2*tau11*
     & tau22*tau13*tau21*f2vm1+f1wm2*tau11**2*tau22**2*f2wm1-f1wm2*
     & tau11**2*tau22*f2vm1*tau23+tau13*f1vm2*tau21*f2wm1*tau11*tau22-
     & 2*tau13*f1vm2*tau21*f2vm1*tau23*tau11-tau13*f1vm2*tau21**2*
     & f2wm1*tau12+tau13*f1vm2*tau21*f2um1*tau23*tau12+tau13*f1um1*
     & tau22**2*f2wm2*tau11+2*tau13*f1um1*tau22*f2um2*tau23*tau12-
     & tau13*f1um1*tau22*f2vm2*tau23*tau11+tau13**2*f1um1*tau22*f2vm2*
     & tau21-tau13**2*f1um1*tau22**2*f2um2-tau13*f1um1*tau22*f2wm2*
     & tau12*tau21-tau13*tau21*f1vm1*f2wm2*tau11*tau22-tau13*tau21*
     & f1vm1*f2um2*tau23*tau12+2*tau13*tau21*f1vm1*f2vm2*tau23*tau11-
     & tau13**2*tau21**2*f1vm1*f2vm2-f1vm1*tau23**2*tau11**2*f2vm2-
     & f1vm1*tau23*tau11*tau13*f2um2*tau22-f1vm1*tau23*tau11*f2wm2*
     & tau12*tau21+tau13**2*f1um2*tau22**2*f2um1+tau13**2*f1vm2*tau21*
     & *2*f2vm1+f1wm1*tau12*tau21**2*tau13*f2vm2-f1wm1*tau12*tau21*
     & tau13*f2um2*tau22+2*f1wm1*tau12*tau21*f2wm2*tau11*tau22+f1wm1*
     & tau12**2*tau21*f2um2*tau23-tau13**2*f1um2*tau22*tau21*f2vm1-
     & f1um1*tau23*tau12*f2wm2*tau11*tau22-f1um1*tau23*tau12*tau13*
     & f2vm2*tau21+f1um1*tau23*tau12**2*f2wm2*tau21+f1wm1*tau11**2*
     & tau22*f2vm2*tau23-f1wm1*tau11**2*tau22**2*f2wm2+f1um1*tau23**2*
     & tau12*f2vm2*tau11-f1wm1*tau11*tau22*f2um2*tau23*tau12+f1vm1*
     & tau23**2*tau11*f2um2*tau12-f1wm1*tau11*tau22*tau13*f2vm2*tau21+
     & f1wm1*tau11*tau22**2*tau13*f2um2-f1wm1*tau12*tau21*f2vm2*tau23*
     & tau11-f1wm1*tau12**2*tau21**2*f2wm2+f1vm1*tau23*tau11**2*f2wm2*
     & tau22-tau13*f1um2*tau22**2*f2wm1*tau11)

      u(i1-is1,i2-is2,i3-is3,ey) = (ttu21*f1wm1*tau11**2*f2vm2*tau23+
     & tau23**2*ttu11*f1um1*f2vm2*tau11-ttu21*f1vm2*tau23*tau11**2*
     & f2wm1-tau23**2*ttu11*f1um1*f2um2*tau12+tau23*tau11**2*f1wm2*
     & f2vm2*ttu22-tau23*ttu11*f1wm2*tau12*tau21*f2um1+tau21*f1vm2*
     & tau23*tau11*f2wm1*ttu11-tau23**2*ttu11*f1vm2*tau11*f2um1+tau23*
     & *2*tau11*f1um2*f2f*tau12-tau21**2*f1wm1*ttu11*f2wm2*tau12-
     & tau23*tau11**2*f1vm2*ttu22*f2wm2+tau21**2*f1wm2*tau12*f2wm1*
     & ttu11-tau23**2*tau11**2*f1vm2*f2f+tau23*ttu11*f1um1*f2wm2*
     & tau12*tau21+tau23**2*ttu11*f1um2*f2um1*tau12-tau23**2*tau11*
     & f1f*f2um2*tau12-tau23**2*tau11*f1vm2*ttu12*f2um2+tau21*f1wm1*
     & tau12*ttu21*f2wm2*tau11+tau23**2*tau11*f1um2*f2vm2*ttu12-tau23*
     & tau12*tau21*f1wm2*tau11*f2f+tau23*tau11*f1um2*f2wm2*tau12*
     & ttu22+tau13*tau21**2*f1wm2*tau12*f2f-tau13*tau21**2*f1vm2*
     & f2wm2*ttu12+tau23*tau11*f1um2*f2wm1*tau12*ttu21+tau23*tau11*
     & f1vm2*tau21*f2wm2*ttu12+tau13*tau21**2*f1wm2*ttu12*f2vm2+tau23*
     & tau12*tau21*f1f*f2wm2*tau11-tau13*tau21**2*f1vm2*f2wm1*ttu11-
     & tau23*tau11*f1wm1*tau12*ttu21*f2um2-tau23*tau11*f1wm2*tau12*
     & ttu22*f2um2-tau13**2*tau21*f1vm2*ttu22*f2um2-tau13**2*tau21**2*
     & f1vm2*f2f+tau13**2*tau21*f1um2*f2vm2*ttu22-tau23*tau11*f1wm2*
     & ttu12*f2vm2*tau21-tau21*f1wm2*tau11*f2wm1*tau12*ttu21+tau13**2*
     & tau21**2*f1f*f2vm2-tau21*f1um2*tau23*tau12*f2wm1*ttu11+tau13**
     & 2*ttu21*f1um1*f2vm2*tau21+tau21*f1wm1*ttu11*f2um2*tau23*tau12-
     & tau21*f1wm1*ttu11*f2vm2*tau23*tau11+tau23**2*tau11**2*f1f*
     & f2vm2-tau13**2*ttu21*f1vm2*tau21*f2um1-tau13*tau21**2*f1f*
     & f2wm2*tau12-tau13*ttu21*f1um2*f2um1*tau23*tau12+tau13*tau21**2*
     & f1wm1*ttu11*f2vm2-tau13*tau21*f1wm2*tau11*f2vm2*ttu22+tau13*
     & tau21*f1vm2*tau23*ttu12*f2um2+tau13*tau21*f1f*f2um2*tau23*
     & tau12-tau13*tau23*ttu11*f1um1*f2vm2*tau21-tau13*ttu21*f1um1*
     & f2vm2*tau23*tau11-tau13*ttu21*f1um1*f2wm2*tau12*tau21+tau13*
     & ttu21*f1um1*f2um2*tau23*tau12-tau13*tau21*f1um2*f2wm2*tau12*
     & ttu22+tau13*ttu21*f1vm2*tau23*tau11*f2um1+tau13*ttu21*f1vm2*
     & tau21*f2wm1*tau11+tau22*ttu21*f1wm2*tau11**2*f2wm1-tau13*tau21*
     & f1um2*f2vm2*tau23*ttu12+tau13*tau21*f1vm2*ttu22*f2wm2*tau11-
     & tau22*ttu21*f1wm1*tau11**2*f2wm2-tau13*tau21*f1um2*f2f*tau23*
     & tau12+tau13*tau23*tau11*f1vm2*ttu22*f2um2-tau22*tau23*tau11**2*
     & f1f*f2wm2+tau13*tau21*f1wm2*tau12*ttu22*f2um2-tau13*ttu21*
     & f1wm1*tau11*f2vm2*tau21+tau13*ttu21*f1wm2*tau12*tau21*f2um1+
     & tau13*tau23*ttu11*f1vm2*tau21*f2um1-tau13*tau23*tau11*f1um2*
     & f2vm2*ttu22-2*tau13*tau21*f1f*f2vm2*tau23*tau11+2*tau13*tau21*
     & f1vm2*tau23*tau11*f2f-tau22*tau13**2*tau21*f1f*f2um2+tau22*
     & tau13**2*ttu21*f1um2*f2um1+tau22*tau23*tau11**2*f1wm2*f2f-
     & tau22*tau13*ttu21*f1wm2*tau11*f2um1+tau22*tau13**2*tau21*f1um2*
     & f2f-tau22*tau13**2*ttu21*f1um1*f2um2-tau22*tau21*f1wm2*tau11*
     & f2wm1*ttu11-tau22*tau23*tau11*f1um2*f2wm2*ttu12+tau22*tau23*
     & ttu11*f1wm2*tau11*f2um1+tau22*tau21*f1wm1*ttu11*f2wm2*tau11-
     & tau22*tau13*ttu21*f1um2*f2wm1*tau11+tau22*tau13*tau21*f1um2*
     & f2wm2*ttu12-tau22*tau13*tau21*f1wm1*ttu11*f2um2+tau22*tau13*
     & tau21*f1f*f2wm2*tau11+tau22*tau13*tau21*f1um2*f2wm1*ttu11-
     & tau22*tau13*tau21*f1wm2*ttu12*f2um2-tau22*tau13*tau21*f1wm2*
     & tau11*f2f-tau22*tau13*tau23*ttu11*f1um2*f2um1+tau22*tau13*
     & ttu21*f1wm1*tau11*f2um2-tau22*tau23*ttu11*f1um1*f2wm2*tau11+
     & tau22*tau23*tau11*f1wm2*ttu12*f2um2+tau22*tau13*tau23*tau11*
     & f1f*f2um2-tau22*tau13*tau23*tau11*f1um2*f2f+tau22*tau13*tau23*
     & ttu11*f1um1*f2um2+tau22*tau13*ttu21*f1um1*f2wm2*tau11)/(-f1um1*
     & tau23**2*tau12**2*f2um2-f1wm2*tau12**2*tau21*f2um1*tau23-f1vm2*
     & tau23*tau11**2*f2wm1*tau22+f1vm2*tau23*tau11*tau13*f2um1*tau22-
     & f1wm2*tau11*tau22**2*tau13*f2um1+f1vm2*tau23*tau11*f2wm1*tau12*
     & tau21-f1vm2*tau23**2*tau11*f2um1*tau12+f1um2*tau23*tau12*tau13*
     & tau21*f2vm1+f1um2*tau23*tau12*f2wm1*tau11*tau22-f1um2*tau23**2*
     & tau12*f2vm1*tau11-f1um2*tau23*tau12**2*f2wm1*tau21+f1um2*tau23*
     & *2*tau12**2*f2um1-2*f1wm2*tau11*tau22*f2wm1*tau12*tau21+f1wm2*
     & tau11*tau22*f2um1*tau23*tau12+f1wm2*tau12*tau21*tau13*f2um1*
     & tau22-f1wm2*tau12*tau21**2*tau13*f2vm1+f1wm2*tau12*tau21*f2vm1*
     & tau23*tau11+f1wm2*tau12**2*tau21**2*f2wm1+f1vm2*tau23**2*tau11*
     & *2*f2vm1+tau13*f1um2*tau22*f2vm1*tau23*tau11+tau13*f1um2*tau22*
     & f2wm1*tau12*tau21-2*tau13*f1um2*tau22*f2um1*tau23*tau12-tau13**
     & 2*f1vm2*tau21*f2um1*tau22+tau13**2*tau21*f1vm1*f2um2*tau22+
     & tau13*tau21**2*f1vm1*f2wm2*tau12+f1wm2*tau11*tau22*tau13*tau21*
     & f2vm1+f1wm2*tau11**2*tau22**2*f2wm1-f1wm2*tau11**2*tau22*f2vm1*
     & tau23+tau13*f1vm2*tau21*f2wm1*tau11*tau22-2*tau13*f1vm2*tau21*
     & f2vm1*tau23*tau11-tau13*f1vm2*tau21**2*f2wm1*tau12+tau13*f1vm2*
     & tau21*f2um1*tau23*tau12+tau13*f1um1*tau22**2*f2wm2*tau11+2*
     & tau13*f1um1*tau22*f2um2*tau23*tau12-tau13*f1um1*tau22*f2vm2*
     & tau23*tau11+tau13**2*f1um1*tau22*f2vm2*tau21-tau13**2*f1um1*
     & tau22**2*f2um2-tau13*f1um1*tau22*f2wm2*tau12*tau21-tau13*tau21*
     & f1vm1*f2wm2*tau11*tau22-tau13*tau21*f1vm1*f2um2*tau23*tau12+2*
     & tau13*tau21*f1vm1*f2vm2*tau23*tau11-tau13**2*tau21**2*f1vm1*
     & f2vm2-f1vm1*tau23**2*tau11**2*f2vm2-f1vm1*tau23*tau11*tau13*
     & f2um2*tau22-f1vm1*tau23*tau11*f2wm2*tau12*tau21+tau13**2*f1um2*
     & tau22**2*f2um1+tau13**2*f1vm2*tau21**2*f2vm1+f1wm1*tau12*tau21*
     & *2*tau13*f2vm2-f1wm1*tau12*tau21*tau13*f2um2*tau22+2*f1wm1*
     & tau12*tau21*f2wm2*tau11*tau22+f1wm1*tau12**2*tau21*f2um2*tau23-
     & tau13**2*f1um2*tau22*tau21*f2vm1-f1um1*tau23*tau12*f2wm2*tau11*
     & tau22-f1um1*tau23*tau12*tau13*f2vm2*tau21+f1um1*tau23*tau12**2*
     & f2wm2*tau21+f1wm1*tau11**2*tau22*f2vm2*tau23-f1wm1*tau11**2*
     & tau22**2*f2wm2+f1um1*tau23**2*tau12*f2vm2*tau11-f1wm1*tau11*
     & tau22*f2um2*tau23*tau12+f1vm1*tau23**2*tau11*f2um2*tau12-f1wm1*
     & tau11*tau22*tau13*f2vm2*tau21+f1wm1*tau11*tau22**2*tau13*f2um2-
     & f1wm1*tau12*tau21*f2vm2*tau23*tau11-f1wm1*tau12**2*tau21**2*
     & f2wm2+f1vm1*tau23*tau11**2*f2wm2*tau22-tau13*f1um2*tau22**2*
     & f2wm1*tau11)

      u(i1-2*is1,i2-2*is2,i3-2*is3,ey) = -(tau23*ttu12*f1um2*f2wm1*
     & tau12*tau21+tau23**2*ttu12*f1um2*f2vm1*tau11+tau13*ttu22*f1vm1*
     & tau23*tau11*f2um2+tau13*tau21**2*f2vm1*f1wm2*ttu12-tau13*tau21*
     & *2*f2wm1*tau12*f1f+tau13*tau21**2*f2f*f1wm1*tau12+tau13*tau21**
     & 2*f2vm1*f1wm1*ttu11-tau13*tau21**2*f2wm2*ttu12*f1vm1+tau13**2*
     & tau21*ttu21*f2vm1*f1um1+tau13**2*ttu22*f1um2*tau21*f2vm1-tau13*
     & *2*ttu22*tau21*f1vm1*f2um2-tau13**2*tau21*f2um1*ttu21*f1vm1+
     & tau13*ttu22*tau21*f1vm1*f2wm2*tau11-tau13*ttu22*f1wm2*tau11*
     & tau21*f2vm1+tau13*ttu22*f1um2*f2um1*tau23*tau12+tau13*ttu22*
     & f1wm1*tau12*tau21*f2um2-tau13**2*tau21**2*f2f*f1vm1+tau13**2*
     & tau21**2*f2vm1*f1f-tau13*tau21**2*f2wm1*ttu11*f1vm1+tau13*
     & tau23*ttu12*tau21*f1vm1*f2um2+tau13*tau21*f2wm1*tau11*ttu21*
     & f1vm1-tau13*ttu22*f1um1*f2um2*tau23*tau12+ttu22*f1wm2*tau11**2*
     & f2vm1*tau23+tau23*tau12*tau21*f2wm1*tau11*f1f+tau21*f2wm1*
     & tau12*f1wm2*tau11*ttu22+tau21*f2um1*tau23*tau12*f1wm2*ttu12-
     & tau21*ttu22*f1wm1*tau12*f2wm2*tau11-tau21*f2wm2*tau12*tau23*
     & ttu12*f1um1-tau21*f2vm1*tau23*tau11*f1wm2*ttu12+tau23*tau11**2*
     & ttu21*f2vm1*f1wm1-tau23*tau12*tau21*f2f*f1wm1*tau11+tau21*
     & f2wm2*ttu12*f1vm1*tau23*tau11-tau23*tau11*f2um1*f1wm1*tau12*
     & ttu21+tau21**2*f2wm2*ttu12*f1wm1*tau12+tau23**2*tau11*f2vm1*
     & ttu11*f1um1-tau21**2*f2wm1*tau12*f1wm2*ttu12+tau23*tau11*tau21*
     & f2wm1*ttu11*f1vm1+tau23*tau11*f2wm1*tau12*ttu21*f1um1+tau23*
     & tau11*ttu22*f1um1*f2wm2*tau12-tau23**2*tau11*f2um1*tau12*f1f-
     & tau23**2*tau11*f2um1*f1vm1*ttu11+tau23**2*tau11*f2f*tau12*
     & f1um1-tau23*tau11*tau21*f2vm1*f1wm1*ttu11-ttu22*f1vm1*tau23*
     & tau11**2*f2wm2-tau23**2*ttu12*f1um2*f2um1*tau12-tau23*ttu12*
     & f1wm1*tau12*tau21*f2um2+tau23**2*ttu12*f1um1*f2um2*tau12-tau23*
     & *2*ttu12*f1vm1*tau11*f2um2-tau23*tau11*ttu22*f1wm2*tau12*f2um1-
     & tau23*tau11**2*f2wm1*ttu21*f1vm1+tau22*tau23*tau11**2*f2f*
     & f1wm1-tau22*tau23*tau11**2*f2wm1*f1f+tau22*ttu22*f1wm1*tau11**
     & 2*f2wm2+tau22*tau13*tau23*ttu12*f1um2*f2um1-tau22*tau13*tau23*
     & ttu12*f1um1*f2um2-tau22*tau13*ttu22*f1wm1*tau11*f2um2-tau13*
     & tau21*f2wm1*tau12*ttu21*f1um1-tau13*tau23*ttu12*f1um2*tau21*
     & f2vm1-tau13*tau21*f2f*tau23*tau12*f1um1-2*tau13*tau21*f2vm1*
     & tau23*tau11*f1f+tau13*tau21*f2um1*tau23*tau12*f1f+tau13*tau21*
     & f2um1*f1wm1*tau12*ttu21-tau13*tau21*ttu21*f2vm1*f1wm1*tau11-
     & tau13*tau23*tau11*ttu21*f2vm1*f1um1+tau13*tau23*tau11*f2um1*
     & ttu21*f1vm1+2*tau13*tau21*f2f*f1vm1*tau23*tau11+tau13*tau21*
     & f2um1*f1vm1*tau23*ttu11-tau13*tau21*f2vm1*tau23*ttu11*f1um1-
     & tau13*ttu22*f1um2*f2vm1*tau23*tau11-tau13*ttu22*f1um2*f2wm1*
     & tau12*tau21-tau22*tau13*tau21*f2f*f1wm1*tau11-tau22*tau13*
     & ttu22*f1um1*f2wm2*tau11+tau22*tau13**2*ttu22*f1um1*f2um2+tau22*
     & tau13**2*tau21*f2f*f1um1-tau22*tau13**2*ttu22*f1um2*f2um1-
     & tau22*tau13**2*tau21*f2um1*f1f+tau22*tau13*tau21*f2wm1*ttu11*
     & f1um1+tau22*tau13*ttu22*f1wm2*tau11*f2um1+tau22*tau13*tau21*
     & f2wm1*tau11*f1f+tau22*tau13*ttu22*f1um2*f2wm1*tau11-tau22*
     & tau13*tau21*f2um1*f1wm2*ttu12+tau22*tau13*tau21*f2wm2*ttu12*
     & f1um1-tau22*tau13*tau21*f2um1*f1wm1*ttu11-tau22*tau13*tau23*
     & tau11*f2f*f1um1-tau22*tau21*f2wm2*ttu12*f1wm1*tau11+tau22*
     & tau23*tau11*f2um1*f1wm1*ttu11-tau22*tau23*tau11*f2wm1*ttu11*
     & f1um1+tau22*tau21*f2wm1*tau11*f1wm2*ttu12+tau22*tau23*ttu12*
     & f1wm1*tau11*f2um2-tau22*ttu22*f1wm2*tau11**2*f2wm1+tau22*tau13*
     & tau23*tau11*f2um1*f1f-tau22*tau23*ttu12*f1um2*f2wm1*tau11+
     & tau23**2*tau11**2*f2vm1*f1f-tau23**2*tau11**2*f2f*f1vm1)/(-
     & f1um1*tau23**2*tau12**2*f2um2-f1wm2*tau12**2*tau21*f2um1*tau23-
     & f1vm2*tau23*tau11**2*f2wm1*tau22+f1vm2*tau23*tau11*tau13*f2um1*
     & tau22-f1wm2*tau11*tau22**2*tau13*f2um1+f1vm2*tau23*tau11*f2wm1*
     & tau12*tau21-f1vm2*tau23**2*tau11*f2um1*tau12+f1um2*tau23*tau12*
     & tau13*tau21*f2vm1+f1um2*tau23*tau12*f2wm1*tau11*tau22-f1um2*
     & tau23**2*tau12*f2vm1*tau11-f1um2*tau23*tau12**2*f2wm1*tau21+
     & f1um2*tau23**2*tau12**2*f2um1-2*f1wm2*tau11*tau22*f2wm1*tau12*
     & tau21+f1wm2*tau11*tau22*f2um1*tau23*tau12+f1wm2*tau12*tau21*
     & tau13*f2um1*tau22-f1wm2*tau12*tau21**2*tau13*f2vm1+f1wm2*tau12*
     & tau21*f2vm1*tau23*tau11+f1wm2*tau12**2*tau21**2*f2wm1+f1vm2*
     & tau23**2*tau11**2*f2vm1+tau13*f1um2*tau22*f2vm1*tau23*tau11+
     & tau13*f1um2*tau22*f2wm1*tau12*tau21-2*tau13*f1um2*tau22*f2um1*
     & tau23*tau12-tau13**2*f1vm2*tau21*f2um1*tau22+tau13**2*tau21*
     & f1vm1*f2um2*tau22+tau13*tau21**2*f1vm1*f2wm2*tau12+f1wm2*tau11*
     & tau22*tau13*tau21*f2vm1+f1wm2*tau11**2*tau22**2*f2wm1-f1wm2*
     & tau11**2*tau22*f2vm1*tau23+tau13*f1vm2*tau21*f2wm1*tau11*tau22-
     & 2*tau13*f1vm2*tau21*f2vm1*tau23*tau11-tau13*f1vm2*tau21**2*
     & f2wm1*tau12+tau13*f1vm2*tau21*f2um1*tau23*tau12+tau13*f1um1*
     & tau22**2*f2wm2*tau11+2*tau13*f1um1*tau22*f2um2*tau23*tau12-
     & tau13*f1um1*tau22*f2vm2*tau23*tau11+tau13**2*f1um1*tau22*f2vm2*
     & tau21-tau13**2*f1um1*tau22**2*f2um2-tau13*f1um1*tau22*f2wm2*
     & tau12*tau21-tau13*tau21*f1vm1*f2wm2*tau11*tau22-tau13*tau21*
     & f1vm1*f2um2*tau23*tau12+2*tau13*tau21*f1vm1*f2vm2*tau23*tau11-
     & tau13**2*tau21**2*f1vm1*f2vm2-f1vm1*tau23**2*tau11**2*f2vm2-
     & f1vm1*tau23*tau11*tau13*f2um2*tau22-f1vm1*tau23*tau11*f2wm2*
     & tau12*tau21+tau13**2*f1um2*tau22**2*f2um1+tau13**2*f1vm2*tau21*
     & *2*f2vm1+f1wm1*tau12*tau21**2*tau13*f2vm2-f1wm1*tau12*tau21*
     & tau13*f2um2*tau22+2*f1wm1*tau12*tau21*f2wm2*tau11*tau22+f1wm1*
     & tau12**2*tau21*f2um2*tau23-tau13**2*f1um2*tau22*tau21*f2vm1-
     & f1um1*tau23*tau12*f2wm2*tau11*tau22-f1um1*tau23*tau12*tau13*
     & f2vm2*tau21+f1um1*tau23*tau12**2*f2wm2*tau21+f1wm1*tau11**2*
     & tau22*f2vm2*tau23-f1wm1*tau11**2*tau22**2*f2wm2+f1um1*tau23**2*
     & tau12*f2vm2*tau11-f1wm1*tau11*tau22*f2um2*tau23*tau12+f1vm1*
     & tau23**2*tau11*f2um2*tau12-f1wm1*tau11*tau22*tau13*f2vm2*tau21+
     & f1wm1*tau11*tau22**2*tau13*f2um2-f1wm1*tau12*tau21*f2vm2*tau23*
     & tau11-f1wm1*tau12**2*tau21**2*f2wm2+f1vm1*tau23*tau11**2*f2wm2*
     & tau22-tau13*f1um2*tau22**2*f2wm1*tau11)

      u(i1-is1,i2-is2,i3-is3,ez) = (tau12*tau13*ttu21*f1vm2*tau21*
     & f2um1-tau21*f1f*f2um2*tau23*tau12**2+tau21*f1um2*f2f*tau23*
     & tau12**2+tau11*f1wm2*tau12*tau21*ttu21*f2vm1+tau11*ttu21*f1vm1*
     & f2um2*tau23*tau12+2*tau11*f1wm2*tau12*tau21*f2f*tau22-tau11*
     & tau13*f1vm2*tau21*ttu21*f2vm1-tau11*f1vm2*tau21*f2f*tau23*
     & tau12-ttu11*f1um1*tau22*f2vm2*tau23*tau11-ttu11*tau21*f1vm1*
     & f2wm2*tau11*tau22+ttu11*f1um1*tau22**2*f2wm2*tau11+ttu11*tau13*
     & f1um1*tau22*f2vm2*tau21-ttu11*tau13*f1vm2*tau21*f2um1*tau22+
     & ttu11*tau13*tau21*f1vm1*f2um2*tau22+ttu11*tau21**2*f1vm1*f2wm2*
     & tau12+ttu11*f1wm2*tau11*tau22*tau21*f2vm1+tau12*tau21*f1um2*
     & f2vm2*tau23*ttu12-tau12*tau21*f1vm2*ttu22*f2wm2*tau11+tau12*
     & ttu21*f1um1*f2vm2*tau23*tau11-tau12*ttu21*f1vm2*tau23*tau11*
     & f2um1-ttu11*tau13*f1um2*tau22*tau21*f2vm1+ttu11*tau13*f1vm2*
     & tau21**2*f2vm1-tau12*tau22*tau21*f1um2*f2wm2*ttu12-tau12*tau22*
     & tau13*tau21*f1um2*f2f+tau12*tau22*tau13*ttu21*f1um1*f2um2-
     & tau12*tau22*tau13*ttu21*f1um2*f2um1+tau12*tau22*ttu21*f1wm2*
     & tau11*f2um1+tau12*tau22*tau13*tau21*f1f*f2um2-tau11*tau13*
     & f1vm2*tau21*f2f*tau22+ttu11*f1um2*tau23*tau12*tau21*f2vm1+
     & tau12*tau22*tau23*ttu11*f1um1*f2um2-tau12*tau22*ttu21*f1um1*
     & f2wm2*tau11-tau12*tau22*tau23*ttu11*f1um2*f2um1-f1f*tau22*
     & f2vm2*tau23*tau11**2-f1wm2*tau11**2*tau22*f2vm2*ttu22-ttu11*
     & tau13*tau21**2*f1vm1*f2vm2+ttu11*tau13*f1um2*tau22**2*f2um1+
     & f1f*tau22**2*f2wm2*tau11**2+ttu21*f1vm1*f2wm2*tau11**2*tau22-
     & ttu11*tau21*f1vm1*f2um2*tau23*tau12+f1vm2*ttu22*f2wm2*tau11**2*
     & tau22-ttu21*f1vm1*f2vm2*tau23*tau11**2-ttu11*f1wm2*tau12*tau21*
     & *2*f2vm1+ttu11*f1wm2*tau12*tau21*f2um1*tau22+ttu11*f1vm2*tau23*
     & tau11*f2um1*tau22-ttu11*f1wm2*tau11*tau22**2*f2um1+tau11*tau13*
     & f1um2*tau22**2*f2f+tau11*f1vm2*tau23*ttu12*f2um2*tau22-tau11*
     & tau13*f1f*tau22**2*f2um2+tau11*f1vm1*tau23*ttu11*f2vm2*tau21+
     & tau11*f1wm2*tau12*ttu22*f2um2*tau22-tau11*ttu21*f1vm1*f2wm2*
     & tau12*tau21+tau11*tau13*f1um2*tau22*ttu21*f2vm1+tau11*f1um2*
     & tau22**2*f2wm2*ttu12+tau11*f1f*tau23*tau12*f2vm2*tau21+tau11*
     & tau13*f1f*tau22*f2vm2*tau21-tau11*f1wm2*ttu12*tau22**2*f2um2+
     & tau11*f1wm2*ttu12*tau22*f2vm2*tau21+tau11*f1f*tau22*f2um2*
     & tau23*tau12-f1wm2*tau11**2*tau22*ttu21*f2vm1-f1wm2*tau11**2*
     & tau22**2*f2f+f1vm2*tau23*tau11**2*f2f*tau22+f1vm2*tau23*tau11**
     & 2*ttu21*f2vm1-tau11*f1vm2*tau21*f2wm2*ttu12*tau22-2*tau11*f1f*
     & tau22*f2wm2*tau12*tau21-tau11*tau13*f1vm2*ttu22*f2um2*tau22-
     & tau11*f1vm2*tau21*f2vm1*tau23*ttu11-tau11*f1um2*tau22*f2vm2*
     & tau23*ttu12-tau11*f1um2*tau22*f2wm2*tau12*ttu22-tau11*f1um2*
     & tau22*f2f*tau23*tau12-tau11*tau13*ttu21*f1vm1*f2um2*tau22+
     & tau11*tau13*f1um2*tau22*f2vm2*ttu22-tau21**2*f1wm2*tau12**2*
     & f2f-tau11*f1um2*tau23*tau12*ttu21*f2vm1+tau11*tau13*ttu21*
     & f1vm1*f2vm2*tau21+ttu21*f1um1*f2wm2*tau12**2*tau21-ttu21*f1um1*
     & f2um2*tau23*tau12**2+tau21*f1um2*f2wm2*tau12**2*ttu22-tau12*
     & tau13*tau21**2*f1f*f2vm2-tau12*tau13*ttu21*f1um1*f2vm2*tau21+
     & tau12*tau13*tau21*f1vm2*ttu22*f2um2+tau12*tau13*tau21**2*f1vm2*
     & f2f-tau12*tau21**2*f1wm2*ttu12*f2vm2-tau12*tau13*tau21*f1um2*
     & f2vm2*ttu22+tau12*tau21*f1wm2*tau11*f2vm2*ttu22-tau12*tau21*
     & f1vm2*tau23*ttu12*f2um2+tau12*tau21**2*f1vm2*f2wm2*ttu12-tau21*
     & f1wm2*tau12**2*ttu22*f2um2-ttu21*f1wm2*tau12**2*tau21*f2um1+
     & ttu21*f1um2*f2um1*tau23*tau12**2+tau21**2*f1f*f2wm2*tau12**2+
     & tau12*tau22*tau21*f1wm2*ttu12*f2um2-ttu11*tau13*f1um1*tau22**2*
     & f2um2-ttu11*f1um1*tau22*f2wm2*tau12*tau21)/(-f1um1*tau23**2*
     & tau12**2*f2um2-f1wm2*tau12**2*tau21*f2um1*tau23-f1vm2*tau23*
     & tau11**2*f2wm1*tau22+f1vm2*tau23*tau11*tau13*f2um1*tau22-f1wm2*
     & tau11*tau22**2*tau13*f2um1+f1vm2*tau23*tau11*f2wm1*tau12*tau21-
     & f1vm2*tau23**2*tau11*f2um1*tau12+f1um2*tau23*tau12*tau13*tau21*
     & f2vm1+f1um2*tau23*tau12*f2wm1*tau11*tau22-f1um2*tau23**2*tau12*
     & f2vm1*tau11-f1um2*tau23*tau12**2*f2wm1*tau21+f1um2*tau23**2*
     & tau12**2*f2um1-2*f1wm2*tau11*tau22*f2wm1*tau12*tau21+f1wm2*
     & tau11*tau22*f2um1*tau23*tau12+f1wm2*tau12*tau21*tau13*f2um1*
     & tau22-f1wm2*tau12*tau21**2*tau13*f2vm1+f1wm2*tau12*tau21*f2vm1*
     & tau23*tau11+f1wm2*tau12**2*tau21**2*f2wm1+f1vm2*tau23**2*tau11*
     & *2*f2vm1+tau13*f1um2*tau22*f2vm1*tau23*tau11+tau13*f1um2*tau22*
     & f2wm1*tau12*tau21-2*tau13*f1um2*tau22*f2um1*tau23*tau12-tau13**
     & 2*f1vm2*tau21*f2um1*tau22+tau13**2*tau21*f1vm1*f2um2*tau22+
     & tau13*tau21**2*f1vm1*f2wm2*tau12+f1wm2*tau11*tau22*tau13*tau21*
     & f2vm1+f1wm2*tau11**2*tau22**2*f2wm1-f1wm2*tau11**2*tau22*f2vm1*
     & tau23+tau13*f1vm2*tau21*f2wm1*tau11*tau22-2*tau13*f1vm2*tau21*
     & f2vm1*tau23*tau11-tau13*f1vm2*tau21**2*f2wm1*tau12+tau13*f1vm2*
     & tau21*f2um1*tau23*tau12+tau13*f1um1*tau22**2*f2wm2*tau11+2*
     & tau13*f1um1*tau22*f2um2*tau23*tau12-tau13*f1um1*tau22*f2vm2*
     & tau23*tau11+tau13**2*f1um1*tau22*f2vm2*tau21-tau13**2*f1um1*
     & tau22**2*f2um2-tau13*f1um1*tau22*f2wm2*tau12*tau21-tau13*tau21*
     & f1vm1*f2wm2*tau11*tau22-tau13*tau21*f1vm1*f2um2*tau23*tau12+2*
     & tau13*tau21*f1vm1*f2vm2*tau23*tau11-tau13**2*tau21**2*f1vm1*
     & f2vm2-f1vm1*tau23**2*tau11**2*f2vm2-f1vm1*tau23*tau11*tau13*
     & f2um2*tau22-f1vm1*tau23*tau11*f2wm2*tau12*tau21+tau13**2*f1um2*
     & tau22**2*f2um1+tau13**2*f1vm2*tau21**2*f2vm1+f1wm1*tau12*tau21*
     & *2*tau13*f2vm2-f1wm1*tau12*tau21*tau13*f2um2*tau22+2*f1wm1*
     & tau12*tau21*f2wm2*tau11*tau22+f1wm1*tau12**2*tau21*f2um2*tau23-
     & tau13**2*f1um2*tau22*tau21*f2vm1-f1um1*tau23*tau12*f2wm2*tau11*
     & tau22-f1um1*tau23*tau12*tau13*f2vm2*tau21+f1um1*tau23*tau12**2*
     & f2wm2*tau21+f1wm1*tau11**2*tau22*f2vm2*tau23-f1wm1*tau11**2*
     & tau22**2*f2wm2+f1um1*tau23**2*tau12*f2vm2*tau11-f1wm1*tau11*
     & tau22*f2um2*tau23*tau12+f1vm1*tau23**2*tau11*f2um2*tau12-f1wm1*
     & tau11*tau22*tau13*f2vm2*tau21+f1wm1*tau11*tau22**2*tau13*f2um2-
     & f1wm1*tau12*tau21*f2vm2*tau23*tau11-f1wm1*tau12**2*tau21**2*
     & f2wm2+f1vm1*tau23*tau11**2*f2wm2*tau22-tau13*f1um2*tau22**2*
     & f2wm1*tau11)

      u(i1-2*is1,i2-2*is2,i3-2*is3,ez) = (tau12*tau22*tau23*ttu12*
     & f1um1*f2um2-tau12*tau22*ttu22*f1wm1*tau11*f2um2-tau12*tau21*
     & ttu21*f2vm1*f1wm1*tau11+tau12*tau21*f2um1*f1vm1*tau23*ttu11-
     & tau12*tau21*f2vm1*tau23*ttu11*f1um1-tau12*ttu22*f1um2*f2vm1*
     & tau23*tau11+tau12*tau22*tau13*ttu22*f1um1*f2um2+tau12*tau22*
     & tau13*tau21*f2f*f1um1-tau11*tau13*f2f*tau22**2*f1um1-tau12*
     & tau22*tau23*ttu12*f1um2*f2um1-tau21**2*f2wm1*tau12**2*f1f+
     & ttu12*tau13*f1um2*tau22**2*f2um1+ttu12*f1wm1*tau12*tau21**2*
     & f2vm2+ttu12*f1um2*tau22*f2vm1*tau23*tau11+ttu12*f1um2*tau22*
     & f2wm1*tau12*tau21-ttu12*tau13*f1vm2*tau21*f2um1*tau22-tau12*
     & tau22*tau21*f2um1*f1wm1*ttu11+tau11*tau21*tau13*f2f*tau22*
     & f1vm1+tau12*tau13*ttu22*f1um2*tau21*f2vm1-ttu12*f1wm1*tau11*
     & tau22*f2vm2*tau21+ttu12*f1wm1*tau11*tau22**2*f2um2-ttu12*f1um2*
     & tau22**2*f2wm1*tau11-ttu12*f1wm1*tau12*tau21*f2um2*tau22-ttu12*
     & tau13*f1um2*tau22*tau21*f2vm1-ttu12*f1um1*tau23*tau12*f2vm2*
     & tau21+ttu12*tau13*f1vm2*tau21**2*f2vm1+ttu12*tau13*tau21*f1vm1*
     & f2um2*tau22+ttu12*f1vm2*tau21*f2wm1*tau11*tau22-ttu12*f1vm2*
     & tau21**2*f2wm1*tau12+ttu12*f1vm2*tau21*f2um1*tau23*tau12+ttu12*
     & tau13*f1um1*tau22*f2vm2*tau21-ttu12*tau13*f1um1*tau22**2*f2um2-
     & ttu12*tau13*tau21**2*f1vm1*f2vm2-tau11*tau13*ttu21*f2vm1*f1um1*
     & tau22+tau11*tau13*ttu22*f1vm2*f2um1*tau22+tau11*tau13*f2um1*
     & tau22*ttu21*f1vm1-f2vm2*ttu22*f1vm1*tau23*tau11**2+ttu21*f2vm1*
     & f1wm1*tau11**2*tau22+f2vm2*ttu22*f1wm1*tau11**2*tau22-f2wm1*
     & tau11**2*tau22**2*f1f+f2vm1*tau23*tau11**2*f1f*tau22-f2wm1*
     & tau11**2*tau22*ttu21*f1vm1+f2f*tau22**2*f1wm1*tau11**2+ttu22*
     & f1vm2*f2vm1*tau23*tau11**2-ttu22*f1vm2*f2wm1*tau11**2*tau22-
     & tau11*tau21*f2vm1*f1wm1*ttu11*tau22+tau11*tau13*f2um1*tau22**2*
     & f1f+tau11*f2vm2*ttu22*f1um1*tau23*tau12-tau11*f2wm1*ttu11*
     & tau22**2*f1um1-2*tau11*tau21*f2f*tau22*f1wm1*tau12+tau11*tau21*
     & f2f*tau23*tau12*f1vm1-tau11*tau21*f2vm1*f1f*tau23*tau12+tau11*
     & tau21*ttu22*f1vm2*f2wm1*tau12+tau11*tau21*f2vm2*tau23*ttu12*
     & f1vm1-tau11*tau21*tau13*ttu22*f1vm2*f2vm1-tau11*tau21*tau13*
     & f2vm1*f1f*tau22+tau11*tau21*tau13*ttu22*f1vm1*f2vm2-tau11*
     & f2um1*tau22*f1wm1*tau12*ttu21+tau11*f2vm1*tau23*ttu11*f1um1*
     & tau22-tau11*ttu22*f1vm2*f2um1*tau23*tau12-tau11*f2um1*tau23*
     & tau12*f1f*tau22-tau11*f2um1*tau22*f1vm1*tau23*ttu11+tau11*f2f*
     & tau23*tau12*f1um1*tau22+tau11*f2um1*tau22**2*f1wm1*ttu11-tau11*
     & tau21*tau23*ttu12*f1vm2*f2vm1+tau21**2*f2f*f1wm1*tau12**2+
     & ttu22*f1um2*f2um1*tau23*tau12**2+tau12*ttu22*f1vm1*tau23*tau11*
     & f2um2+tau12*tau21**2*f2vm1*f1wm1*ttu11+tau12*tau13*tau21*ttu21*
     & f2vm1*f1um1-tau12*tau13*ttu22*tau21*f1vm1*f2um2-tau12*tau13*
     & tau21*f2um1*ttu21*f1vm1-tau12*tau13*tau21**2*f2f*f1vm1+tau12*
     & tau13*tau21**2*f2vm1*f1f-tau12*tau21**2*f2wm1*ttu11*f1vm1+
     & tau12*tau21*f2wm1*tau11*ttu21*f1vm1+tau11*f2wm1*tau12*ttu21*
     & f1um1*tau22-tau11*tau21*f2vm2*ttu22*f1wm1*tau12+tau11*tau21*
     & f2wm1*ttu11*tau22*f1vm1-tau12*tau22*tau13*ttu22*f1um2*f2um1-
     & tau12*tau22*tau13*tau21*f2um1*f1f+tau12*tau22*tau21*f2wm1*
     & ttu11*f1um1+tau12*tau22*ttu22*f1um2*f2wm1*tau11-tau21*f2wm1*
     & tau12**2*ttu21*f1um1-tau21*f2f*tau23*tau12**2*f1um1+tau21*
     & f2um1*tau23*tau12**2*f1f+tau21*f2um1*f1wm1*tau12**2*ttu21-
     & ttu22*f1um2*f2wm1*tau12**2*tau21+ttu22*f1wm1*tau12**2*tau21*
     & f2um2-ttu22*f1um1*f2um2*tau23*tau12**2+2*tau11*tau21*f2wm1*
     & tau12*f1f*tau22-tau11*tau13*ttu22*f1um1*tau22*f2vm2-f2f*tau22*
     & f1vm1*tau23*tau11**2-ttu12*f1vm1*tau23*tau11*f2um2*tau22)/(-
     & f1um1*tau23**2*tau12**2*f2um2-f1wm2*tau12**2*tau21*f2um1*tau23-
     & f1vm2*tau23*tau11**2*f2wm1*tau22+f1vm2*tau23*tau11*tau13*f2um1*
     & tau22-f1wm2*tau11*tau22**2*tau13*f2um1+f1vm2*tau23*tau11*f2wm1*
     & tau12*tau21-f1vm2*tau23**2*tau11*f2um1*tau12+f1um2*tau23*tau12*
     & tau13*tau21*f2vm1+f1um2*tau23*tau12*f2wm1*tau11*tau22-f1um2*
     & tau23**2*tau12*f2vm1*tau11-f1um2*tau23*tau12**2*f2wm1*tau21+
     & f1um2*tau23**2*tau12**2*f2um1-2*f1wm2*tau11*tau22*f2wm1*tau12*
     & tau21+f1wm2*tau11*tau22*f2um1*tau23*tau12+f1wm2*tau12*tau21*
     & tau13*f2um1*tau22-f1wm2*tau12*tau21**2*tau13*f2vm1+f1wm2*tau12*
     & tau21*f2vm1*tau23*tau11+f1wm2*tau12**2*tau21**2*f2wm1+f1vm2*
     & tau23**2*tau11**2*f2vm1+tau13*f1um2*tau22*f2vm1*tau23*tau11+
     & tau13*f1um2*tau22*f2wm1*tau12*tau21-2*tau13*f1um2*tau22*f2um1*
     & tau23*tau12-tau13**2*f1vm2*tau21*f2um1*tau22+tau13**2*tau21*
     & f1vm1*f2um2*tau22+tau13*tau21**2*f1vm1*f2wm2*tau12+f1wm2*tau11*
     & tau22*tau13*tau21*f2vm1+f1wm2*tau11**2*tau22**2*f2wm1-f1wm2*
     & tau11**2*tau22*f2vm1*tau23+tau13*f1vm2*tau21*f2wm1*tau11*tau22-
     & 2*tau13*f1vm2*tau21*f2vm1*tau23*tau11-tau13*f1vm2*tau21**2*
     & f2wm1*tau12+tau13*f1vm2*tau21*f2um1*tau23*tau12+tau13*f1um1*
     & tau22**2*f2wm2*tau11+2*tau13*f1um1*tau22*f2um2*tau23*tau12-
     & tau13*f1um1*tau22*f2vm2*tau23*tau11+tau13**2*f1um1*tau22*f2vm2*
     & tau21-tau13**2*f1um1*tau22**2*f2um2-tau13*f1um1*tau22*f2wm2*
     & tau12*tau21-tau13*tau21*f1vm1*f2wm2*tau11*tau22-tau13*tau21*
     & f1vm1*f2um2*tau23*tau12+2*tau13*tau21*f1vm1*f2vm2*tau23*tau11-
     & tau13**2*tau21**2*f1vm1*f2vm2-f1vm1*tau23**2*tau11**2*f2vm2-
     & f1vm1*tau23*tau11*tau13*f2um2*tau22-f1vm1*tau23*tau11*f2wm2*
     & tau12*tau21+tau13**2*f1um2*tau22**2*f2um1+tau13**2*f1vm2*tau21*
     & *2*f2vm1+f1wm1*tau12*tau21**2*tau13*f2vm2-f1wm1*tau12*tau21*
     & tau13*f2um2*tau22+2*f1wm1*tau12*tau21*f2wm2*tau11*tau22+f1wm1*
     & tau12**2*tau21*f2um2*tau23-tau13**2*f1um2*tau22*tau21*f2vm1-
     & f1um1*tau23*tau12*f2wm2*tau11*tau22-f1um1*tau23*tau12*tau13*
     & f2vm2*tau21+f1um1*tau23*tau12**2*f2wm2*tau21+f1wm1*tau11**2*
     & tau22*f2vm2*tau23-f1wm1*tau11**2*tau22**2*f2wm2+f1um1*tau23**2*
     & tau12*f2vm2*tau11-f1wm1*tau11*tau22*f2um2*tau23*tau12+f1vm1*
     & tau23**2*tau11*f2um2*tau12-f1wm1*tau11*tau22*tau13*f2vm2*tau21+
     & f1wm1*tau11*tau22**2*tau13*f2um2-f1wm1*tau12*tau21*f2vm2*tau23*
     & tau11-f1wm1*tau12**2*tau21**2*f2wm2+f1vm1*tau23*tau11**2*f2wm2*
     & tau22-tau13*f1um2*tau22**2*f2wm1*tau11)


 ! *********** done *********************
                 !  if( .true. .or. debug.gt.0 )then
                 !   write(*,'(" bc4:corr:   i1,i2,i3=",3i3," u(-1)=",3f8.2," u(-2)=",3f8.2)') i1,i2,i3,!          u(i1-is1,i2-is2,i3-is3,ex),u(i1-is1,i2-is2,i3-is3,ey),u(i1-is1,i2-is2,i3-is3,ez),!          u(i1-2*is1,i2-2*is2,i3-2*is3,ex),u(i1-2*is1,i2-2*is2,i3-2*is3,ey),u(i1-2*is1,i2-2*is2,i3-2*is3,ez)
                 !  end if
                   if( debug.gt.0 )then
                     call ogf3d(ep,xy(i1-is1,i2-is2,i3-is3,0),xy(i1-
     & is1,i2-is2,i3-is3,1),xy(i1-is1,i2-is2,i3-is3,2),t,uvm(0),uvm(1)
     & ,uvm(2))
                     call ogf3d(ep,xy(i1-2*is1,i2-2*is2,i3-2*is3,0),xy(
     & i1-2*is1,i2-2*is2,i3-2*is3,1),xy(i1-2*is1,i2-2*is2,i3-2*is3,2),
     & t,uvm2(0),uvm2(1),uvm2(2))
                    write(*,'(" **bc4:correction: i=",3i4," errors u(-
     & 1)=",3e10.2)') i1,i2,i3,u(i1-is1,i2-is2,i3-is3,ex)-uvm(0),u(i1-
     & is1,i2-is2,i3-is3,ey)-uvm(1),u(i1-is1,i2-is2,i3-is3,ez)-uvm(2)
                    write(*,'(" **bc4:correction: i=",3i4," errors u(-
     & 2)=",3e10.2)') i1,i2,i3,u(i1-2*is1,i2-2*is2,i3-2*is3,ex)-uvm2(
     & 0),u(i1-2*is1,i2-2*is2,i3-2*is3,ey)-uvm2(1),u(i1-2*is1,i2-2*
     & is2,i3-2*is3,ez)-uvm2(2)
                   end if
                   ! set to exact for testing
                   ! OGF3D(i1-is1,i2-is2,i3-is3,t, uvm(0),uvm(1),uvm(2))
                   ! OGF3D(i1-2*is1,i2-2*is2,i3-2*is3,t, uvm2(0),uvm2(1),uvm2(2))
                   ! u(i1-is1,i2-is2,i3-is3,ex)=uvm(0)
                   ! u(i1-is1,i2-is2,i3-is3,ey)=uvm(1)
                   ! u(i1-is1,i2-is2,i3-is3,ez)=uvm(2)
                   ! u(i1-2*is1,i2-2*is2,i3-2*is3,ex)=uvm2(0)
                   ! u(i1-2*is1,i2-2*is2,i3-2*is3,ey)=uvm2(1)
                   ! u(i1-2*is1,i2-2*is2,i3-2*is3,ez)=uvm2(2)
                 end if ! mask
                 end do
                 end do
                 end do
                 end if ! if true
                 if( debug.gt.0 )then
                 ! ============================DEBUG=======================================================
                 if( useForcing.ne.0 )then
                 ! **** check that we satisfy all the equations ****
                 maxDivc=0.
                 maxTauDotLapu=0.
                 maxExtrap=0.
                 maxDr3aDotU=0.
                 write(*,'(" ***bc4:START grid=",i4,", side,axis=",2i3,
     & " **** ")') grid,side,axis
                 do i3=n3a,n3b
                 do i2=n2a,n2b
                 do i1=n1a,n1b
                 if( mask(i1,i2,i3).gt.0 )then
                   ! precompute the inverse of the jacobian, used in macros AmnD3J
                   i10=i1  ! used by jac3di in macros
                   i20=i2
                   i30=i3
                   do m3=-2,2
                   do m2=-2,2
                   do m1=-2,2
                    jac3di(m1,m2,m3)=1./(rx(i1+m1,i2+m2,i3+m3)*(sy(i1+
     & m1,i2+m2,i3+m3)*tz(i1+m1,i2+m2,i3+m3)-sz(i1+m1,i2+m2,i3+m3)*ty(
     & i1+m1,i2+m2,i3+m3))+ry(i1+m1,i2+m2,i3+m3)*(sz(i1+m1,i2+m2,i3+
     & m3)*tx(i1+m1,i2+m2,i3+m3)-sx(i1+m1,i2+m2,i3+m3)*tz(i1+m1,i2+m2,
     & i3+m3))+rz(i1+m1,i2+m2,i3+m3)*(sx(i1+m1,i2+m2,i3+m3)*ty(i1+m1,
     & i2+m2,i3+m3)-sy(i1+m1,i2+m2,i3+m3)*tx(i1+m1,i2+m2,i3+m3)))
                   end do
                   end do
                   end do
                   a11 =(rsxy(i1,i2,i3,axis,0)*jac3di(i1-i10,i2-i20,i3-
     & i30))
                   a12 =(rsxy(i1,i2,i3,axis,1)*jac3di(i1-i10,i2-i20,i3-
     & i30))
                   a13 =(rsxy(i1,i2,i3,axis,2)*jac3di(i1-i10,i2-i20,i3-
     & i30))
                   a21 =(rsxy(i1,i2,i3,axisp1,0)*jac3di(i1-i10,i2-i20,
     & i3-i30))
                   a22 =(rsxy(i1,i2,i3,axisp1,1)*jac3di(i1-i10,i2-i20,
     & i3-i30))
                   a23 =(rsxy(i1,i2,i3,axisp1,2)*jac3di(i1-i10,i2-i20,
     & i3-i30))
                   a31 =(rsxy(i1,i2,i3,axisp2,0)*jac3di(i1-i10,i2-i20,
     & i3-i30))
                   a32 =(rsxy(i1,i2,i3,axisp2,1)*jac3di(i1-i10,i2-i20,
     & i3-i30))
                   a33 =(rsxy(i1,i2,i3,axisp2,2)*jac3di(i1-i10,i2-i20,
     & i3-i30))
                   a11m1 =(rsxy(i1-is1,i2-is2,i3-is3,axis,0)*jac3di(i1-
     & is1-i10,i2-is2-i20,i3-is3-i30))
                   a12m1 =(rsxy(i1-is1,i2-is2,i3-is3,axis,1)*jac3di(i1-
     & is1-i10,i2-is2-i20,i3-is3-i30))
                   a13m1 =(rsxy(i1-is1,i2-is2,i3-is3,axis,2)*jac3di(i1-
     & is1-i10,i2-is2-i20,i3-is3-i30))
                   a11p1 =(rsxy(i1+is1,i2+is2,i3+is3,axis,0)*jac3di(i1+
     & is1-i10,i2+is2-i20,i3+is3-i30))
                   a12p1 =(rsxy(i1+is1,i2+is2,i3+is3,axis,1)*jac3di(i1+
     & is1-i10,i2+is2-i20,i3+is3-i30))
                   a13p1 =(rsxy(i1+is1,i2+is2,i3+is3,axis,2)*jac3di(i1+
     & is1-i10,i2+is2-i20,i3+is3-i30))
                   a11m2 =(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,0)*
     & jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))
                   a12m2 =(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,1)*
     & jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))
                   a13m2 =(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,2)*
     & jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))
                   a11p2 =(rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axis,0)*
     & jac3di(i1+2*is1-i10,i2+2*is2-i20,i3+2*is3-i30))
                   a12p2 =(rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axis,1)*
     & jac3di(i1+2*is1-i10,i2+2*is2-i20,i3+2*is3-i30))
                   a13p2 =(rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axis,2)*
     & jac3di(i1+2*is1-i10,i2+2*is2-i20,i3+2*is3-i30))
                   a11r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axis,0)*
     & jac3di(i1+is1-i10,i2+is2-i20,i3+is3-i30))-(rsxy(i1-is1,i2-is2,
     & i3-is3,axis,0)*jac3di(i1-is1-i10,i2-is2-i20,i3-is3-i30)))-((
     & rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axis,0)*jac3di(i1+2*is1-i10,i2+
     & 2*is2-i20,i3+2*is3-i30))-(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,
     & 0)*jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))))/(12.*dra)
                   a12r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axis,1)*
     & jac3di(i1+is1-i10,i2+is2-i20,i3+is3-i30))-(rsxy(i1-is1,i2-is2,
     & i3-is3,axis,1)*jac3di(i1-is1-i10,i2-is2-i20,i3-is3-i30)))-((
     & rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axis,1)*jac3di(i1+2*is1-i10,i2+
     & 2*is2-i20,i3+2*is3-i30))-(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,
     & 1)*jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))))/(12.*dra)
                   a13r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axis,2)*
     & jac3di(i1+is1-i10,i2+is2-i20,i3+is3-i30))-(rsxy(i1-is1,i2-is2,
     & i3-is3,axis,2)*jac3di(i1-is1-i10,i2-is2-i20,i3-is3-i30)))-((
     & rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axis,2)*jac3di(i1+2*is1-i10,i2+
     & 2*is2-i20,i3+2*is3-i30))-(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,
     & 2)*jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))))/(12.*dra)
                   a21r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axisp1,0)*
     & jac3di(i1+is1-i10,i2+is2-i20,i3+is3-i30))-(rsxy(i1-is1,i2-is2,
     & i3-is3,axisp1,0)*jac3di(i1-is1-i10,i2-is2-i20,i3-is3-i30)))-((
     & rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp1,0)*jac3di(i1+2*is1-i10,
     & i2+2*is2-i20,i3+2*is3-i30))-(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,
     & axisp1,0)*jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))))/(
     & 12.*dra)
                   a22r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axisp1,1)*
     & jac3di(i1+is1-i10,i2+is2-i20,i3+is3-i30))-(rsxy(i1-is1,i2-is2,
     & i3-is3,axisp1,1)*jac3di(i1-is1-i10,i2-is2-i20,i3-is3-i30)))-((
     & rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp1,1)*jac3di(i1+2*is1-i10,
     & i2+2*is2-i20,i3+2*is3-i30))-(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,
     & axisp1,1)*jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))))/(
     & 12.*dra)
                   a23r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axisp1,2)*
     & jac3di(i1+is1-i10,i2+is2-i20,i3+is3-i30))-(rsxy(i1-is1,i2-is2,
     & i3-is3,axisp1,2)*jac3di(i1-is1-i10,i2-is2-i20,i3-is3-i30)))-((
     & rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp1,2)*jac3di(i1+2*is1-i10,
     & i2+2*is2-i20,i3+2*is3-i30))-(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,
     & axisp1,2)*jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))))/(
     & 12.*dra)
                   a31r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axisp2,0)*
     & jac3di(i1+is1-i10,i2+is2-i20,i3+is3-i30))-(rsxy(i1-is1,i2-is2,
     & i3-is3,axisp2,0)*jac3di(i1-is1-i10,i2-is2-i20,i3-is3-i30)))-((
     & rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp2,0)*jac3di(i1+2*is1-i10,
     & i2+2*is2-i20,i3+2*is3-i30))-(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,
     & axisp2,0)*jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))))/(
     & 12.*dra)
                   a32r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axisp2,1)*
     & jac3di(i1+is1-i10,i2+is2-i20,i3+is3-i30))-(rsxy(i1-is1,i2-is2,
     & i3-is3,axisp2,1)*jac3di(i1-is1-i10,i2-is2-i20,i3-is3-i30)))-((
     & rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp2,1)*jac3di(i1+2*is1-i10,
     & i2+2*is2-i20,i3+2*is3-i30))-(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,
     & axisp2,1)*jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))))/(
     & 12.*dra)
                   a33r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axisp2,2)*
     & jac3di(i1+is1-i10,i2+is2-i20,i3+is3-i30))-(rsxy(i1-is1,i2-is2,
     & i3-is3,axisp2,2)*jac3di(i1-is1-i10,i2-is2-i20,i3-is3-i30)))-((
     & rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp2,2)*jac3di(i1+2*is1-i10,
     & i2+2*is2-i20,i3+2*is3-i30))-(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,
     & axisp2,2)*jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))))/(
     & 12.*dra)
                   a11rr = ((-30.*(rsxy(i1,i2,i3,axis,0)*jac3di(i1-i10,
     & i2-i20,i3-i30))+16.*((rsxy(i1+is1,i2+is2,i3+is3,axis,0)*jac3di(
     & i1+is1-i10,i2+is2-i20,i3+is3-i30))+(rsxy(i1-is1,i2-is2,i3-is3,
     & axis,0)*jac3di(i1-is1-i10,i2-is2-i20,i3-is3-i30)))-((rsxy(i1+2*
     & is1,i2+2*is2,i3+2*is3,axis,0)*jac3di(i1+2*is1-i10,i2+2*is2-i20,
     & i3+2*is3-i30))+(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,0)*jac3di(
     & i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))))/(12.*dra**2))
                   a12rr = ((-30.*(rsxy(i1,i2,i3,axis,1)*jac3di(i1-i10,
     & i2-i20,i3-i30))+16.*((rsxy(i1+is1,i2+is2,i3+is3,axis,1)*jac3di(
     & i1+is1-i10,i2+is2-i20,i3+is3-i30))+(rsxy(i1-is1,i2-is2,i3-is3,
     & axis,1)*jac3di(i1-is1-i10,i2-is2-i20,i3-is3-i30)))-((rsxy(i1+2*
     & is1,i2+2*is2,i3+2*is3,axis,1)*jac3di(i1+2*is1-i10,i2+2*is2-i20,
     & i3+2*is3-i30))+(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,1)*jac3di(
     & i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))))/(12.*dra**2))
                   a13rr = ((-30.*(rsxy(i1,i2,i3,axis,2)*jac3di(i1-i10,
     & i2-i20,i3-i30))+16.*((rsxy(i1+is1,i2+is2,i3+is3,axis,2)*jac3di(
     & i1+is1-i10,i2+is2-i20,i3+is3-i30))+(rsxy(i1-is1,i2-is2,i3-is3,
     & axis,2)*jac3di(i1-is1-i10,i2-is2-i20,i3-is3-i30)))-((rsxy(i1+2*
     & is1,i2+2*is2,i3+2*is3,axis,2)*jac3di(i1+2*is1-i10,i2+2*is2-i20,
     & i3+2*is3-i30))+(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,2)*jac3di(
     & i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))))/(12.*dra**2))
                   a21rr = ((-30.*(rsxy(i1,i2,i3,axisp1,0)*jac3di(i1-
     & i10,i2-i20,i3-i30))+16.*((rsxy(i1+is1,i2+is2,i3+is3,axisp1,0)*
     & jac3di(i1+is1-i10,i2+is2-i20,i3+is3-i30))+(rsxy(i1-is1,i2-is2,
     & i3-is3,axisp1,0)*jac3di(i1-is1-i10,i2-is2-i20,i3-is3-i30)))-((
     & rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp1,0)*jac3di(i1+2*is1-i10,
     & i2+2*is2-i20,i3+2*is3-i30))+(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,
     & axisp1,0)*jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))))/(
     & 12.*dra**2))
                   a22rr = ((-30.*(rsxy(i1,i2,i3,axisp1,1)*jac3di(i1-
     & i10,i2-i20,i3-i30))+16.*((rsxy(i1+is1,i2+is2,i3+is3,axisp1,1)*
     & jac3di(i1+is1-i10,i2+is2-i20,i3+is3-i30))+(rsxy(i1-is1,i2-is2,
     & i3-is3,axisp1,1)*jac3di(i1-is1-i10,i2-is2-i20,i3-is3-i30)))-((
     & rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp1,1)*jac3di(i1+2*is1-i10,
     & i2+2*is2-i20,i3+2*is3-i30))+(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,
     & axisp1,1)*jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))))/(
     & 12.*dra**2))
                   a23rr = ((-30.*(rsxy(i1,i2,i3,axisp1,2)*jac3di(i1-
     & i10,i2-i20,i3-i30))+16.*((rsxy(i1+is1,i2+is2,i3+is3,axisp1,2)*
     & jac3di(i1+is1-i10,i2+is2-i20,i3+is3-i30))+(rsxy(i1-is1,i2-is2,
     & i3-is3,axisp1,2)*jac3di(i1-is1-i10,i2-is2-i20,i3-is3-i30)))-((
     & rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp1,2)*jac3di(i1+2*is1-i10,
     & i2+2*is2-i20,i3+2*is3-i30))+(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,
     & axisp1,2)*jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))))/(
     & 12.*dra**2))
                   a31rr = ((-30.*(rsxy(i1,i2,i3,axisp2,0)*jac3di(i1-
     & i10,i2-i20,i3-i30))+16.*((rsxy(i1+is1,i2+is2,i3+is3,axisp2,0)*
     & jac3di(i1+is1-i10,i2+is2-i20,i3+is3-i30))+(rsxy(i1-is1,i2-is2,
     & i3-is3,axisp2,0)*jac3di(i1-is1-i10,i2-is2-i20,i3-is3-i30)))-((
     & rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp2,0)*jac3di(i1+2*is1-i10,
     & i2+2*is2-i20,i3+2*is3-i30))+(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,
     & axisp2,0)*jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))))/(
     & 12.*dra**2))
                   a32rr = ((-30.*(rsxy(i1,i2,i3,axisp2,1)*jac3di(i1-
     & i10,i2-i20,i3-i30))+16.*((rsxy(i1+is1,i2+is2,i3+is3,axisp2,1)*
     & jac3di(i1+is1-i10,i2+is2-i20,i3+is3-i30))+(rsxy(i1-is1,i2-is2,
     & i3-is3,axisp2,1)*jac3di(i1-is1-i10,i2-is2-i20,i3-is3-i30)))-((
     & rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp2,1)*jac3di(i1+2*is1-i10,
     & i2+2*is2-i20,i3+2*is3-i30))+(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,
     & axisp2,1)*jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))))/(
     & 12.*dra**2))
                   a33rr = ((-30.*(rsxy(i1,i2,i3,axisp2,2)*jac3di(i1-
     & i10,i2-i20,i3-i30))+16.*((rsxy(i1+is1,i2+is2,i3+is3,axisp2,2)*
     & jac3di(i1+is1-i10,i2+is2-i20,i3+is3-i30))+(rsxy(i1-is1,i2-is2,
     & i3-is3,axisp2,2)*jac3di(i1-is1-i10,i2-is2-i20,i3-is3-i30)))-((
     & rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp2,2)*jac3di(i1+2*is1-i10,
     & i2+2*is2-i20,i3+2*is3-i30))+(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,
     & axisp2,2)*jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))))/(
     & 12.*dra**2))
                   a11s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axis,0)*
     & jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))-(rsxy(i1-js1,i2-js2,
     & i3-js3,axis,0)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axis,0)*jac3di(i1+2*js1-i10,i2+
     & 2*js2-i20,i3+2*js3-i30))-(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axis,
     & 0)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))/(12.*dsa)
                   a12s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axis,1)*
     & jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))-(rsxy(i1-js1,i2-js2,
     & i3-js3,axis,1)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axis,1)*jac3di(i1+2*js1-i10,i2+
     & 2*js2-i20,i3+2*js3-i30))-(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axis,
     & 1)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))/(12.*dsa)
                   a13s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axis,2)*
     & jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))-(rsxy(i1-js1,i2-js2,
     & i3-js3,axis,2)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axis,2)*jac3di(i1+2*js1-i10,i2+
     & 2*js2-i20,i3+2*js3-i30))-(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axis,
     & 2)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))/(12.*dsa)
                   a21s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axisp1,0)*
     & jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))-(rsxy(i1-js1,i2-js2,
     & i3-js3,axisp1,0)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,0)*jac3di(i1+2*js1-i10,
     & i2+2*js2-i20,i3+2*js3-i30))-(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,
     & axisp1,0)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))/(
     & 12.*dsa)
                   a22s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axisp1,1)*
     & jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))-(rsxy(i1-js1,i2-js2,
     & i3-js3,axisp1,1)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,1)*jac3di(i1+2*js1-i10,
     & i2+2*js2-i20,i3+2*js3-i30))-(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,
     & axisp1,1)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))/(
     & 12.*dsa)
                   a23s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axisp1,2)*
     & jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))-(rsxy(i1-js1,i2-js2,
     & i3-js3,axisp1,2)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,2)*jac3di(i1+2*js1-i10,
     & i2+2*js2-i20,i3+2*js3-i30))-(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,
     & axisp1,2)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))/(
     & 12.*dsa)
                   a31s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axisp2,0)*
     & jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))-(rsxy(i1-js1,i2-js2,
     & i3-js3,axisp2,0)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp2,0)*jac3di(i1+2*js1-i10,
     & i2+2*js2-i20,i3+2*js3-i30))-(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,
     & axisp2,0)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))/(
     & 12.*dsa)
                   a32s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axisp2,1)*
     & jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))-(rsxy(i1-js1,i2-js2,
     & i3-js3,axisp2,1)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp2,1)*jac3di(i1+2*js1-i10,
     & i2+2*js2-i20,i3+2*js3-i30))-(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,
     & axisp2,1)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))/(
     & 12.*dsa)
                   a33s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axisp2,2)*
     & jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))-(rsxy(i1-js1,i2-js2,
     & i3-js3,axisp2,2)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp2,2)*jac3di(i1+2*js1-i10,
     & i2+2*js2-i20,i3+2*js3-i30))-(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,
     & axisp2,2)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))/(
     & 12.*dsa)
                   a11ss = ((-30.*(rsxy(i1,i2,i3,axis,0)*jac3di(i1-i10,
     & i2-i20,i3-i30))+16.*((rsxy(i1+js1,i2+js2,i3+js3,axis,0)*jac3di(
     & i1+js1-i10,i2+js2-i20,i3+js3-i30))+(rsxy(i1-js1,i2-js2,i3-js3,
     & axis,0)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))-((rsxy(i1+2*
     & js1,i2+2*js2,i3+2*js3,axis,0)*jac3di(i1+2*js1-i10,i2+2*js2-i20,
     & i3+2*js3-i30))+(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axis,0)*jac3di(
     & i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))/(12.*dsa**2))
                   a12ss = ((-30.*(rsxy(i1,i2,i3,axis,1)*jac3di(i1-i10,
     & i2-i20,i3-i30))+16.*((rsxy(i1+js1,i2+js2,i3+js3,axis,1)*jac3di(
     & i1+js1-i10,i2+js2-i20,i3+js3-i30))+(rsxy(i1-js1,i2-js2,i3-js3,
     & axis,1)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))-((rsxy(i1+2*
     & js1,i2+2*js2,i3+2*js3,axis,1)*jac3di(i1+2*js1-i10,i2+2*js2-i20,
     & i3+2*js3-i30))+(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axis,1)*jac3di(
     & i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))/(12.*dsa**2))
                   a13ss = ((-30.*(rsxy(i1,i2,i3,axis,2)*jac3di(i1-i10,
     & i2-i20,i3-i30))+16.*((rsxy(i1+js1,i2+js2,i3+js3,axis,2)*jac3di(
     & i1+js1-i10,i2+js2-i20,i3+js3-i30))+(rsxy(i1-js1,i2-js2,i3-js3,
     & axis,2)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))-((rsxy(i1+2*
     & js1,i2+2*js2,i3+2*js3,axis,2)*jac3di(i1+2*js1-i10,i2+2*js2-i20,
     & i3+2*js3-i30))+(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axis,2)*jac3di(
     & i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))/(12.*dsa**2))
                   a21ss = ((-30.*(rsxy(i1,i2,i3,axisp1,0)*jac3di(i1-
     & i10,i2-i20,i3-i30))+16.*((rsxy(i1+js1,i2+js2,i3+js3,axisp1,0)*
     & jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))+(rsxy(i1-js1,i2-js2,
     & i3-js3,axisp1,0)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,0)*jac3di(i1+2*js1-i10,
     & i2+2*js2-i20,i3+2*js3-i30))+(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,
     & axisp1,0)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))/(
     & 12.*dsa**2))
                   a22ss = ((-30.*(rsxy(i1,i2,i3,axisp1,1)*jac3di(i1-
     & i10,i2-i20,i3-i30))+16.*((rsxy(i1+js1,i2+js2,i3+js3,axisp1,1)*
     & jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))+(rsxy(i1-js1,i2-js2,
     & i3-js3,axisp1,1)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,1)*jac3di(i1+2*js1-i10,
     & i2+2*js2-i20,i3+2*js3-i30))+(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,
     & axisp1,1)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))/(
     & 12.*dsa**2))
                   a23ss = ((-30.*(rsxy(i1,i2,i3,axisp1,2)*jac3di(i1-
     & i10,i2-i20,i3-i30))+16.*((rsxy(i1+js1,i2+js2,i3+js3,axisp1,2)*
     & jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))+(rsxy(i1-js1,i2-js2,
     & i3-js3,axisp1,2)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,2)*jac3di(i1+2*js1-i10,
     & i2+2*js2-i20,i3+2*js3-i30))+(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,
     & axisp1,2)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))/(
     & 12.*dsa**2))
                   a31ss = ((-30.*(rsxy(i1,i2,i3,axisp2,0)*jac3di(i1-
     & i10,i2-i20,i3-i30))+16.*((rsxy(i1+js1,i2+js2,i3+js3,axisp2,0)*
     & jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))+(rsxy(i1-js1,i2-js2,
     & i3-js3,axisp2,0)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp2,0)*jac3di(i1+2*js1-i10,
     & i2+2*js2-i20,i3+2*js3-i30))+(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,
     & axisp2,0)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))/(
     & 12.*dsa**2))
                   a32ss = ((-30.*(rsxy(i1,i2,i3,axisp2,1)*jac3di(i1-
     & i10,i2-i20,i3-i30))+16.*((rsxy(i1+js1,i2+js2,i3+js3,axisp2,1)*
     & jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))+(rsxy(i1-js1,i2-js2,
     & i3-js3,axisp2,1)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp2,1)*jac3di(i1+2*js1-i10,
     & i2+2*js2-i20,i3+2*js3-i30))+(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,
     & axisp2,1)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))/(
     & 12.*dsa**2))
                   a33ss = ((-30.*(rsxy(i1,i2,i3,axisp2,2)*jac3di(i1-
     & i10,i2-i20,i3-i30))+16.*((rsxy(i1+js1,i2+js2,i3+js3,axisp2,2)*
     & jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))+(rsxy(i1-js1,i2-js2,
     & i3-js3,axisp2,2)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp2,2)*jac3di(i1+2*js1-i10,
     & i2+2*js2-i20,i3+2*js3-i30))+(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,
     & axisp2,2)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))/(
     & 12.*dsa**2))
                   a11sss = ((rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axis,0)*
     & jac3di(i1+2*js1-i10,i2+2*js2-i20,i3+2*js3-i30))-2.*(rsxy(i1+
     & js1,i2+js2,i3+js3,axis,0)*jac3di(i1+js1-i10,i2+js2-i20,i3+js3-
     & i30))+2.*(rsxy(i1-js1,i2-js2,i3-js3,axis,0)*jac3di(i1-js1-i10,
     & i2-js2-i20,i3-js3-i30))-(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axis,
     & 0)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30)))/(2.*dsa**3)
                   a12sss = ((rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axis,1)*
     & jac3di(i1+2*js1-i10,i2+2*js2-i20,i3+2*js3-i30))-2.*(rsxy(i1+
     & js1,i2+js2,i3+js3,axis,1)*jac3di(i1+js1-i10,i2+js2-i20,i3+js3-
     & i30))+2.*(rsxy(i1-js1,i2-js2,i3-js3,axis,1)*jac3di(i1-js1-i10,
     & i2-js2-i20,i3-js3-i30))-(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axis,
     & 1)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30)))/(2.*dsa**3)
                   a13sss = ((rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axis,2)*
     & jac3di(i1+2*js1-i10,i2+2*js2-i20,i3+2*js3-i30))-2.*(rsxy(i1+
     & js1,i2+js2,i3+js3,axis,2)*jac3di(i1+js1-i10,i2+js2-i20,i3+js3-
     & i30))+2.*(rsxy(i1-js1,i2-js2,i3-js3,axis,2)*jac3di(i1-js1-i10,
     & i2-js2-i20,i3-js3-i30))-(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axis,
     & 2)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30)))/(2.*dsa**3)
                   a21sss = ((rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,0)
     & *jac3di(i1+2*js1-i10,i2+2*js2-i20,i3+2*js3-i30))-2.*(rsxy(i1+
     & js1,i2+js2,i3+js3,axisp1,0)*jac3di(i1+js1-i10,i2+js2-i20,i3+
     & js3-i30))+2.*(rsxy(i1-js1,i2-js2,i3-js3,axisp1,0)*jac3di(i1-
     & js1-i10,i2-js2-i20,i3-js3-i30))-(rsxy(i1-2*js1,i2-2*js2,i3-2*
     & js3,axisp1,0)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30)))
     & /(2.*dsa**3)
                   a22sss = ((rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,1)
     & *jac3di(i1+2*js1-i10,i2+2*js2-i20,i3+2*js3-i30))-2.*(rsxy(i1+
     & js1,i2+js2,i3+js3,axisp1,1)*jac3di(i1+js1-i10,i2+js2-i20,i3+
     & js3-i30))+2.*(rsxy(i1-js1,i2-js2,i3-js3,axisp1,1)*jac3di(i1-
     & js1-i10,i2-js2-i20,i3-js3-i30))-(rsxy(i1-2*js1,i2-2*js2,i3-2*
     & js3,axisp1,1)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30)))
     & /(2.*dsa**3)
                   a23sss = ((rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,2)
     & *jac3di(i1+2*js1-i10,i2+2*js2-i20,i3+2*js3-i30))-2.*(rsxy(i1+
     & js1,i2+js2,i3+js3,axisp1,2)*jac3di(i1+js1-i10,i2+js2-i20,i3+
     & js3-i30))+2.*(rsxy(i1-js1,i2-js2,i3-js3,axisp1,2)*jac3di(i1-
     & js1-i10,i2-js2-i20,i3-js3-i30))-(rsxy(i1-2*js1,i2-2*js2,i3-2*
     & js3,axisp1,2)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30)))
     & /(2.*dsa**3)
                   a31sss = ((rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp2,0)
     & *jac3di(i1+2*js1-i10,i2+2*js2-i20,i3+2*js3-i30))-2.*(rsxy(i1+
     & js1,i2+js2,i3+js3,axisp2,0)*jac3di(i1+js1-i10,i2+js2-i20,i3+
     & js3-i30))+2.*(rsxy(i1-js1,i2-js2,i3-js3,axisp2,0)*jac3di(i1-
     & js1-i10,i2-js2-i20,i3-js3-i30))-(rsxy(i1-2*js1,i2-2*js2,i3-2*
     & js3,axisp2,0)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30)))
     & /(2.*dsa**3)
                   a32sss = ((rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp2,1)
     & *jac3di(i1+2*js1-i10,i2+2*js2-i20,i3+2*js3-i30))-2.*(rsxy(i1+
     & js1,i2+js2,i3+js3,axisp2,1)*jac3di(i1+js1-i10,i2+js2-i20,i3+
     & js3-i30))+2.*(rsxy(i1-js1,i2-js2,i3-js3,axisp2,1)*jac3di(i1-
     & js1-i10,i2-js2-i20,i3-js3-i30))-(rsxy(i1-2*js1,i2-2*js2,i3-2*
     & js3,axisp2,1)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30)))
     & /(2.*dsa**3)
                   a33sss = ((rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp2,2)
     & *jac3di(i1+2*js1-i10,i2+2*js2-i20,i3+2*js3-i30))-2.*(rsxy(i1+
     & js1,i2+js2,i3+js3,axisp2,2)*jac3di(i1+js1-i10,i2+js2-i20,i3+
     & js3-i30))+2.*(rsxy(i1-js1,i2-js2,i3-js3,axisp2,2)*jac3di(i1-
     & js1-i10,i2-js2-i20,i3-js3-i30))-(rsxy(i1-2*js1,i2-2*js2,i3-2*
     & js3,axisp2,2)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30)))
     & /(2.*dsa**3)
                   a11t = (8.*((rsxy(i1+ks1,i2+ks2,i3+ks3,axis,0)*
     & jac3di(i1+ks1-i10,i2+ks2-i20,i3+ks3-i30))-(rsxy(i1-ks1,i2-ks2,
     & i3-ks3,axis,0)*jac3di(i1-ks1-i10,i2-ks2-i20,i3-ks3-i30)))-((
     & rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axis,0)*jac3di(i1+2*ks1-i10,i2+
     & 2*ks2-i20,i3+2*ks3-i30))-(rsxy(i1-2*ks1,i2-2*ks2,i3-2*ks3,axis,
     & 0)*jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30))))/(12.*dta)
                   a12t = (8.*((rsxy(i1+ks1,i2+ks2,i3+ks3,axis,1)*
     & jac3di(i1+ks1-i10,i2+ks2-i20,i3+ks3-i30))-(rsxy(i1-ks1,i2-ks2,
     & i3-ks3,axis,1)*jac3di(i1-ks1-i10,i2-ks2-i20,i3-ks3-i30)))-((
     & rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axis,1)*jac3di(i1+2*ks1-i10,i2+
     & 2*ks2-i20,i3+2*ks3-i30))-(rsxy(i1-2*ks1,i2-2*ks2,i3-2*ks3,axis,
     & 1)*jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30))))/(12.*dta)
                   a13t = (8.*((rsxy(i1+ks1,i2+ks2,i3+ks3,axis,2)*
     & jac3di(i1+ks1-i10,i2+ks2-i20,i3+ks3-i30))-(rsxy(i1-ks1,i2-ks2,
     & i3-ks3,axis,2)*jac3di(i1-ks1-i10,i2-ks2-i20,i3-ks3-i30)))-((
     & rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axis,2)*jac3di(i1+2*ks1-i10,i2+
     & 2*ks2-i20,i3+2*ks3-i30))-(rsxy(i1-2*ks1,i2-2*ks2,i3-2*ks3,axis,
     & 2)*jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30))))/(12.*dta)
                   a21t = (8.*((rsxy(i1+ks1,i2+ks2,i3+ks3,axisp1,0)*
     & jac3di(i1+ks1-i10,i2+ks2-i20,i3+ks3-i30))-(rsxy(i1-ks1,i2-ks2,
     & i3-ks3,axisp1,0)*jac3di(i1-ks1-i10,i2-ks2-i20,i3-ks3-i30)))-((
     & rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axisp1,0)*jac3di(i1+2*ks1-i10,
     & i2+2*ks2-i20,i3+2*ks3-i30))-(rsxy(i1-2*ks1,i2-2*ks2,i3-2*ks3,
     & axisp1,0)*jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30))))/(
     & 12.*dta)
                   a22t = (8.*((rsxy(i1+ks1,i2+ks2,i3+ks3,axisp1,1)*
     & jac3di(i1+ks1-i10,i2+ks2-i20,i3+ks3-i30))-(rsxy(i1-ks1,i2-ks2,
     & i3-ks3,axisp1,1)*jac3di(i1-ks1-i10,i2-ks2-i20,i3-ks3-i30)))-((
     & rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axisp1,1)*jac3di(i1+2*ks1-i10,
     & i2+2*ks2-i20,i3+2*ks3-i30))-(rsxy(i1-2*ks1,i2-2*ks2,i3-2*ks3,
     & axisp1,1)*jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30))))/(
     & 12.*dta)
                   a23t = (8.*((rsxy(i1+ks1,i2+ks2,i3+ks3,axisp1,2)*
     & jac3di(i1+ks1-i10,i2+ks2-i20,i3+ks3-i30))-(rsxy(i1-ks1,i2-ks2,
     & i3-ks3,axisp1,2)*jac3di(i1-ks1-i10,i2-ks2-i20,i3-ks3-i30)))-((
     & rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axisp1,2)*jac3di(i1+2*ks1-i10,
     & i2+2*ks2-i20,i3+2*ks3-i30))-(rsxy(i1-2*ks1,i2-2*ks2,i3-2*ks3,
     & axisp1,2)*jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30))))/(
     & 12.*dta)
                   a31t = (8.*((rsxy(i1+ks1,i2+ks2,i3+ks3,axisp2,0)*
     & jac3di(i1+ks1-i10,i2+ks2-i20,i3+ks3-i30))-(rsxy(i1-ks1,i2-ks2,
     & i3-ks3,axisp2,0)*jac3di(i1-ks1-i10,i2-ks2-i20,i3-ks3-i30)))-((
     & rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axisp2,0)*jac3di(i1+2*ks1-i10,
     & i2+2*ks2-i20,i3+2*ks3-i30))-(rsxy(i1-2*ks1,i2-2*ks2,i3-2*ks3,
     & axisp2,0)*jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30))))/(
     & 12.*dta)
                   a32t = (8.*((rsxy(i1+ks1,i2+ks2,i3+ks3,axisp2,1)*
     & jac3di(i1+ks1-i10,i2+ks2-i20,i3+ks3-i30))-(rsxy(i1-ks1,i2-ks2,
     & i3-ks3,axisp2,1)*jac3di(i1-ks1-i10,i2-ks2-i20,i3-ks3-i30)))-((
     & rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axisp2,1)*jac3di(i1+2*ks1-i10,
     & i2+2*ks2-i20,i3+2*ks3-i30))-(rsxy(i1-2*ks1,i2-2*ks2,i3-2*ks3,
     & axisp2,1)*jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30))))/(
     & 12.*dta)
                   a33t = (8.*((rsxy(i1+ks1,i2+ks2,i3+ks3,axisp2,2)*
     & jac3di(i1+ks1-i10,i2+ks2-i20,i3+ks3-i30))-(rsxy(i1-ks1,i2-ks2,
     & i3-ks3,axisp2,2)*jac3di(i1-ks1-i10,i2-ks2-i20,i3-ks3-i30)))-((
     & rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axisp2,2)*jac3di(i1+2*ks1-i10,
     & i2+2*ks2-i20,i3+2*ks3-i30))-(rsxy(i1-2*ks1,i2-2*ks2,i3-2*ks3,
     & axisp2,2)*jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30))))/(
     & 12.*dta)
                   a11tt = ((-30.*(rsxy(i1,i2,i3,axis,0)*jac3di(i1-i10,
     & i2-i20,i3-i30))+16.*((rsxy(i1+ks1,i2+ks2,i3+ks3,axis,0)*jac3di(
     & i1+ks1-i10,i2+ks2-i20,i3+ks3-i30))+(rsxy(i1-ks1,i2-ks2,i3-ks3,
     & axis,0)*jac3di(i1-ks1-i10,i2-ks2-i20,i3-ks3-i30)))-((rsxy(i1+2*
     & ks1,i2+2*ks2,i3+2*ks3,axis,0)*jac3di(i1+2*ks1-i10,i2+2*ks2-i20,
     & i3+2*ks3-i30))+(rsxy(i1-2*ks1,i2-2*ks2,i3-2*ks3,axis,0)*jac3di(
     & i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30))))/(12.*dta**2))
                   a12tt = ((-30.*(rsxy(i1,i2,i3,axis,1)*jac3di(i1-i10,
     & i2-i20,i3-i30))+16.*((rsxy(i1+ks1,i2+ks2,i3+ks3,axis,1)*jac3di(
     & i1+ks1-i10,i2+ks2-i20,i3+ks3-i30))+(rsxy(i1-ks1,i2-ks2,i3-ks3,
     & axis,1)*jac3di(i1-ks1-i10,i2-ks2-i20,i3-ks3-i30)))-((rsxy(i1+2*
     & ks1,i2+2*ks2,i3+2*ks3,axis,1)*jac3di(i1+2*ks1-i10,i2+2*ks2-i20,
     & i3+2*ks3-i30))+(rsxy(i1-2*ks1,i2-2*ks2,i3-2*ks3,axis,1)*jac3di(
     & i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30))))/(12.*dta**2))
                   a13tt = ((-30.*(rsxy(i1,i2,i3,axis,2)*jac3di(i1-i10,
     & i2-i20,i3-i30))+16.*((rsxy(i1+ks1,i2+ks2,i3+ks3,axis,2)*jac3di(
     & i1+ks1-i10,i2+ks2-i20,i3+ks3-i30))+(rsxy(i1-ks1,i2-ks2,i3-ks3,
     & axis,2)*jac3di(i1-ks1-i10,i2-ks2-i20,i3-ks3-i30)))-((rsxy(i1+2*
     & ks1,i2+2*ks2,i3+2*ks3,axis,2)*jac3di(i1+2*ks1-i10,i2+2*ks2-i20,
     & i3+2*ks3-i30))+(rsxy(i1-2*ks1,i2-2*ks2,i3-2*ks3,axis,2)*jac3di(
     & i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30))))/(12.*dta**2))
                   a21tt = ((-30.*(rsxy(i1,i2,i3,axisp1,0)*jac3di(i1-
     & i10,i2-i20,i3-i30))+16.*((rsxy(i1+ks1,i2+ks2,i3+ks3,axisp1,0)*
     & jac3di(i1+ks1-i10,i2+ks2-i20,i3+ks3-i30))+(rsxy(i1-ks1,i2-ks2,
     & i3-ks3,axisp1,0)*jac3di(i1-ks1-i10,i2-ks2-i20,i3-ks3-i30)))-((
     & rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axisp1,0)*jac3di(i1+2*ks1-i10,
     & i2+2*ks2-i20,i3+2*ks3-i30))+(rsxy(i1-2*ks1,i2-2*ks2,i3-2*ks3,
     & axisp1,0)*jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30))))/(
     & 12.*dta**2))
                   a22tt = ((-30.*(rsxy(i1,i2,i3,axisp1,1)*jac3di(i1-
     & i10,i2-i20,i3-i30))+16.*((rsxy(i1+ks1,i2+ks2,i3+ks3,axisp1,1)*
     & jac3di(i1+ks1-i10,i2+ks2-i20,i3+ks3-i30))+(rsxy(i1-ks1,i2-ks2,
     & i3-ks3,axisp1,1)*jac3di(i1-ks1-i10,i2-ks2-i20,i3-ks3-i30)))-((
     & rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axisp1,1)*jac3di(i1+2*ks1-i10,
     & i2+2*ks2-i20,i3+2*ks3-i30))+(rsxy(i1-2*ks1,i2-2*ks2,i3-2*ks3,
     & axisp1,1)*jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30))))/(
     & 12.*dta**2))
                   a23tt = ((-30.*(rsxy(i1,i2,i3,axisp1,2)*jac3di(i1-
     & i10,i2-i20,i3-i30))+16.*((rsxy(i1+ks1,i2+ks2,i3+ks3,axisp1,2)*
     & jac3di(i1+ks1-i10,i2+ks2-i20,i3+ks3-i30))+(rsxy(i1-ks1,i2-ks2,
     & i3-ks3,axisp1,2)*jac3di(i1-ks1-i10,i2-ks2-i20,i3-ks3-i30)))-((
     & rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axisp1,2)*jac3di(i1+2*ks1-i10,
     & i2+2*ks2-i20,i3+2*ks3-i30))+(rsxy(i1-2*ks1,i2-2*ks2,i3-2*ks3,
     & axisp1,2)*jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30))))/(
     & 12.*dta**2))
                   a31tt = ((-30.*(rsxy(i1,i2,i3,axisp2,0)*jac3di(i1-
     & i10,i2-i20,i3-i30))+16.*((rsxy(i1+ks1,i2+ks2,i3+ks3,axisp2,0)*
     & jac3di(i1+ks1-i10,i2+ks2-i20,i3+ks3-i30))+(rsxy(i1-ks1,i2-ks2,
     & i3-ks3,axisp2,0)*jac3di(i1-ks1-i10,i2-ks2-i20,i3-ks3-i30)))-((
     & rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axisp2,0)*jac3di(i1+2*ks1-i10,
     & i2+2*ks2-i20,i3+2*ks3-i30))+(rsxy(i1-2*ks1,i2-2*ks2,i3-2*ks3,
     & axisp2,0)*jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30))))/(
     & 12.*dta**2))
                   a32tt = ((-30.*(rsxy(i1,i2,i3,axisp2,1)*jac3di(i1-
     & i10,i2-i20,i3-i30))+16.*((rsxy(i1+ks1,i2+ks2,i3+ks3,axisp2,1)*
     & jac3di(i1+ks1-i10,i2+ks2-i20,i3+ks3-i30))+(rsxy(i1-ks1,i2-ks2,
     & i3-ks3,axisp2,1)*jac3di(i1-ks1-i10,i2-ks2-i20,i3-ks3-i30)))-((
     & rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axisp2,1)*jac3di(i1+2*ks1-i10,
     & i2+2*ks2-i20,i3+2*ks3-i30))+(rsxy(i1-2*ks1,i2-2*ks2,i3-2*ks3,
     & axisp2,1)*jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30))))/(
     & 12.*dta**2))
                   a33tt = ((-30.*(rsxy(i1,i2,i3,axisp2,2)*jac3di(i1-
     & i10,i2-i20,i3-i30))+16.*((rsxy(i1+ks1,i2+ks2,i3+ks3,axisp2,2)*
     & jac3di(i1+ks1-i10,i2+ks2-i20,i3+ks3-i30))+(rsxy(i1-ks1,i2-ks2,
     & i3-ks3,axisp2,2)*jac3di(i1-ks1-i10,i2-ks2-i20,i3-ks3-i30)))-((
     & rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axisp2,2)*jac3di(i1+2*ks1-i10,
     & i2+2*ks2-i20,i3+2*ks3-i30))+(rsxy(i1-2*ks1,i2-2*ks2,i3-2*ks3,
     & axisp2,2)*jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30))))/(
     & 12.*dta**2))
                   a11ttt = ((rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axis,0)*
     & jac3di(i1+2*ks1-i10,i2+2*ks2-i20,i3+2*ks3-i30))-2.*(rsxy(i1+
     & ks1,i2+ks2,i3+ks3,axis,0)*jac3di(i1+ks1-i10,i2+ks2-i20,i3+ks3-
     & i30))+2.*(rsxy(i1-ks1,i2-ks2,i3-ks3,axis,0)*jac3di(i1-ks1-i10,
     & i2-ks2-i20,i3-ks3-i30))-(rsxy(i1-2*ks1,i2-2*ks2,i3-2*ks3,axis,
     & 0)*jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30)))/(2.*dta**3)
                   a12ttt = ((rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axis,1)*
     & jac3di(i1+2*ks1-i10,i2+2*ks2-i20,i3+2*ks3-i30))-2.*(rsxy(i1+
     & ks1,i2+ks2,i3+ks3,axis,1)*jac3di(i1+ks1-i10,i2+ks2-i20,i3+ks3-
     & i30))+2.*(rsxy(i1-ks1,i2-ks2,i3-ks3,axis,1)*jac3di(i1-ks1-i10,
     & i2-ks2-i20,i3-ks3-i30))-(rsxy(i1-2*ks1,i2-2*ks2,i3-2*ks3,axis,
     & 1)*jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30)))/(2.*dta**3)
                   a13ttt = ((rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axis,2)*
     & jac3di(i1+2*ks1-i10,i2+2*ks2-i20,i3+2*ks3-i30))-2.*(rsxy(i1+
     & ks1,i2+ks2,i3+ks3,axis,2)*jac3di(i1+ks1-i10,i2+ks2-i20,i3+ks3-
     & i30))+2.*(rsxy(i1-ks1,i2-ks2,i3-ks3,axis,2)*jac3di(i1-ks1-i10,
     & i2-ks2-i20,i3-ks3-i30))-(rsxy(i1-2*ks1,i2-2*ks2,i3-2*ks3,axis,
     & 2)*jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30)))/(2.*dta**3)
                   a21ttt = ((rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axisp1,0)
     & *jac3di(i1+2*ks1-i10,i2+2*ks2-i20,i3+2*ks3-i30))-2.*(rsxy(i1+
     & ks1,i2+ks2,i3+ks3,axisp1,0)*jac3di(i1+ks1-i10,i2+ks2-i20,i3+
     & ks3-i30))+2.*(rsxy(i1-ks1,i2-ks2,i3-ks3,axisp1,0)*jac3di(i1-
     & ks1-i10,i2-ks2-i20,i3-ks3-i30))-(rsxy(i1-2*ks1,i2-2*ks2,i3-2*
     & ks3,axisp1,0)*jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30)))
     & /(2.*dta**3)
                   a22ttt = ((rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axisp1,1)
     & *jac3di(i1+2*ks1-i10,i2+2*ks2-i20,i3+2*ks3-i30))-2.*(rsxy(i1+
     & ks1,i2+ks2,i3+ks3,axisp1,1)*jac3di(i1+ks1-i10,i2+ks2-i20,i3+
     & ks3-i30))+2.*(rsxy(i1-ks1,i2-ks2,i3-ks3,axisp1,1)*jac3di(i1-
     & ks1-i10,i2-ks2-i20,i3-ks3-i30))-(rsxy(i1-2*ks1,i2-2*ks2,i3-2*
     & ks3,axisp1,1)*jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30)))
     & /(2.*dta**3)
                   a23ttt = ((rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axisp1,2)
     & *jac3di(i1+2*ks1-i10,i2+2*ks2-i20,i3+2*ks3-i30))-2.*(rsxy(i1+
     & ks1,i2+ks2,i3+ks3,axisp1,2)*jac3di(i1+ks1-i10,i2+ks2-i20,i3+
     & ks3-i30))+2.*(rsxy(i1-ks1,i2-ks2,i3-ks3,axisp1,2)*jac3di(i1-
     & ks1-i10,i2-ks2-i20,i3-ks3-i30))-(rsxy(i1-2*ks1,i2-2*ks2,i3-2*
     & ks3,axisp1,2)*jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30)))
     & /(2.*dta**3)
                   a31ttt = ((rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axisp2,0)
     & *jac3di(i1+2*ks1-i10,i2+2*ks2-i20,i3+2*ks3-i30))-2.*(rsxy(i1+
     & ks1,i2+ks2,i3+ks3,axisp2,0)*jac3di(i1+ks1-i10,i2+ks2-i20,i3+
     & ks3-i30))+2.*(rsxy(i1-ks1,i2-ks2,i3-ks3,axisp2,0)*jac3di(i1-
     & ks1-i10,i2-ks2-i20,i3-ks3-i30))-(rsxy(i1-2*ks1,i2-2*ks2,i3-2*
     & ks3,axisp2,0)*jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30)))
     & /(2.*dta**3)
                   a32ttt = ((rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axisp2,1)
     & *jac3di(i1+2*ks1-i10,i2+2*ks2-i20,i3+2*ks3-i30))-2.*(rsxy(i1+
     & ks1,i2+ks2,i3+ks3,axisp2,1)*jac3di(i1+ks1-i10,i2+ks2-i20,i3+
     & ks3-i30))+2.*(rsxy(i1-ks1,i2-ks2,i3-ks3,axisp2,1)*jac3di(i1-
     & ks1-i10,i2-ks2-i20,i3-ks3-i30))-(rsxy(i1-2*ks1,i2-2*ks2,i3-2*
     & ks3,axisp2,1)*jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30)))
     & /(2.*dta**3)
                   a33ttt = ((rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axisp2,2)
     & *jac3di(i1+2*ks1-i10,i2+2*ks2-i20,i3+2*ks3-i30))-2.*(rsxy(i1+
     & ks1,i2+ks2,i3+ks3,axisp2,2)*jac3di(i1+ks1-i10,i2+ks2-i20,i3+
     & ks3-i30))+2.*(rsxy(i1-ks1,i2-ks2,i3-ks3,axisp2,2)*jac3di(i1-
     & ks1-i10,i2-ks2-i20,i3-ks3-i30))-(rsxy(i1-2*ks1,i2-2*ks2,i3-2*
     & ks3,axisp2,2)*jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30)))
     & /(2.*dta**3)
                   c11 = (rsxy(i1,i2,i3,axis,0)**2+rsxy(i1,i2,i3,axis,
     & 1)**2+rsxy(i1,i2,i3,axis,2)**2)
                   c22 = (rsxy(i1,i2,i3,axisp1,0)**2+rsxy(i1,i2,i3,
     & axisp1,1)**2+rsxy(i1,i2,i3,axisp1,2)**2)
                   c33 = (rsxy(i1,i2,i3,axisp2,0)**2+rsxy(i1,i2,i3,
     & axisp2,1)**2+rsxy(i1,i2,i3,axisp2,2)**2)
                   c1 = (rsxyx43(i1,i2,i3,axis,0)+rsxyy43(i1,i2,i3,
     & axis,1)+rsxyz43(i1,i2,i3,axis,2))
                   c2 = (rsxyx43(i1,i2,i3,axisp1,0)+rsxyy43(i1,i2,i3,
     & axisp1,1)+rsxyz43(i1,i2,i3,axisp1,2))
                   c3 = (rsxyx43(i1,i2,i3,axisp2,0)+rsxyy43(i1,i2,i3,
     & axisp2,1)+rsxyz43(i1,i2,i3,axisp2,2))
                   c11r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axis,0)**2+
     & rsxy(i1+is1,i2+is2,i3+is3,axis,1)**2+rsxy(i1+is1,i2+is2,i3+is3,
     & axis,2)**2)-(rsxy(i1-is1,i2-is2,i3-is3,axis,0)**2+rsxy(i1-is1,
     & i2-is2,i3-is3,axis,1)**2+rsxy(i1-is1,i2-is2,i3-is3,axis,2)**2))
     &    -((rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axis,0)**2+rsxy(i1+2*is1,
     & i2+2*is2,i3+2*is3,axis,1)**2+rsxy(i1+2*is1,i2+2*is2,i3+2*is3,
     & axis,2)**2)-(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,0)**2+rsxy(
     & i1-2*is1,i2-2*is2,i3-2*is3,axis,1)**2+rsxy(i1-2*is1,i2-2*is2,
     & i3-2*is3,axis,2)**2))   )/(12.*dra)
                   c22r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axisp1,0)**2+
     & rsxy(i1+is1,i2+is2,i3+is3,axisp1,1)**2+rsxy(i1+is1,i2+is2,i3+
     & is3,axisp1,2)**2)-(rsxy(i1-is1,i2-is2,i3-is3,axisp1,0)**2+rsxy(
     & i1-is1,i2-is2,i3-is3,axisp1,1)**2+rsxy(i1-is1,i2-is2,i3-is3,
     & axisp1,2)**2))   -((rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp1,0)**
     & 2+rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp1,1)**2+rsxy(i1+2*is1,
     & i2+2*is2,i3+2*is3,axisp1,2)**2)-(rsxy(i1-2*is1,i2-2*is2,i3-2*
     & is3,axisp1,0)**2+rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axisp1,1)**2+
     & rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axisp1,2)**2))   )/(12.*dra)
                   c33r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axisp2,0)**2+
     & rsxy(i1+is1,i2+is2,i3+is3,axisp2,1)**2+rsxy(i1+is1,i2+is2,i3+
     & is3,axisp2,2)**2)-(rsxy(i1-is1,i2-is2,i3-is3,axisp2,0)**2+rsxy(
     & i1-is1,i2-is2,i3-is3,axisp2,1)**2+rsxy(i1-is1,i2-is2,i3-is3,
     & axisp2,2)**2))   -((rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp2,0)**
     & 2+rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp2,1)**2+rsxy(i1+2*is1,
     & i2+2*is2,i3+2*is3,axisp2,2)**2)-(rsxy(i1-2*is1,i2-2*is2,i3-2*
     & is3,axisp2,0)**2+rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axisp2,1)**2+
     & rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axisp2,2)**2))   )/(12.*dra)
                   if( axis.eq.0 )then
                     c1r = (rsxyxr43(i1,i2,i3,axis,0)+rsxyyr43(i1,i2,
     & i3,axis,1)+rsxyzr43(i1,i2,i3,axis,2))
                     c2r = (rsxyxr43(i1,i2,i3,axisp1,0)+rsxyyr43(i1,i2,
     & i3,axisp1,1)+rsxyzr43(i1,i2,i3,axisp1,2))
                     c3r = (rsxyxr43(i1,i2,i3,axisp2,0)+rsxyyr43(i1,i2,
     & i3,axisp2,1)+rsxyzr43(i1,i2,i3,axisp2,2))
                   else if( axis.eq.1 )then
                     c1r = (rsxyxs43(i1,i2,i3,axis,0)+rsxyys43(i1,i2,
     & i3,axis,1)+rsxyzs43(i1,i2,i3,axis,2))
                     c2r = (rsxyxs43(i1,i2,i3,axisp1,0)+rsxyys43(i1,i2,
     & i3,axisp1,1)+rsxyzs43(i1,i2,i3,axisp1,2))
                     c3r = (rsxyxs43(i1,i2,i3,axisp2,0)+rsxyys43(i1,i2,
     & i3,axisp2,1)+rsxyzs43(i1,i2,i3,axisp2,2))
                   else
                     c1r = (rsxyxt43(i1,i2,i3,axis,0)+rsxyyt43(i1,i2,
     & i3,axis,1)+rsxyzt43(i1,i2,i3,axis,2))
                     c2r = (rsxyxt43(i1,i2,i3,axisp1,0)+rsxyyt43(i1,i2,
     & i3,axisp1,1)+rsxyzt43(i1,i2,i3,axisp1,2))
                     c3r = (rsxyxt43(i1,i2,i3,axisp2,0)+rsxyyt43(i1,i2,
     & i3,axisp2,1)+rsxyzt43(i1,i2,i3,axisp2,2))
                   end if
                   us=(8.*(u(i1+js1,i2+js2,i3+js3,ex)-u(i1-js1,i2-js2,
     & i3-js3,ex))-(u(i1+2*js1,i2+2*js2,i3+2*js3,ex)-u(i1-2*js1,i2-2*
     & js2,i3-2*js3,ex)))/(12.*dsa)
                   uss=(-30.*u(i1,i2,i3,ex)+16.*(u(i1+js1,i2+js2,i3+
     & js3,ex)+u(i1-js1,i2-js2,i3-js3,ex))-(u(i1+2*js1,i2+2*js2,i3+2*
     & js3,ex)+u(i1-2*js1,i2-2*js2,i3-2*js3,ex)))/(12.*dsa**2)
                   usss=(u(i1+2*js1,i2+2*js2,i3+2*js3,ex)-2.*u(i1+js1,
     & i2+js2,i3+js3,ex)+2.*u(i1-js1,i2-js2,i3-js3,ex)-u(i1-2*js1,i2-
     & 2*js2,i3-2*js3,ex))/(2.*dsa**3)
                   vs=(8.*(u(i1+js1,i2+js2,i3+js3,ey)-u(i1-js1,i2-js2,
     & i3-js3,ey))-(u(i1+2*js1,i2+2*js2,i3+2*js3,ey)-u(i1-2*js1,i2-2*
     & js2,i3-2*js3,ey)))/(12.*dsa)
                   vss=(-30.*u(i1,i2,i3,ey)+16.*(u(i1+js1,i2+js2,i3+
     & js3,ey)+u(i1-js1,i2-js2,i3-js3,ey))-(u(i1+2*js1,i2+2*js2,i3+2*
     & js3,ey)+u(i1-2*js1,i2-2*js2,i3-2*js3,ey)))/(12.*dsa**2)
                   vsss=(u(i1+2*js1,i2+2*js2,i3+2*js3,ey)-2.*u(i1+js1,
     & i2+js2,i3+js3,ey)+2.*u(i1-js1,i2-js2,i3-js3,ey)-u(i1-2*js1,i2-
     & 2*js2,i3-2*js3,ey))/(2.*dsa**3)
                   ws=(8.*(u(i1+js1,i2+js2,i3+js3,ez)-u(i1-js1,i2-js2,
     & i3-js3,ez))-(u(i1+2*js1,i2+2*js2,i3+2*js3,ez)-u(i1-2*js1,i2-2*
     & js2,i3-2*js3,ez)))/(12.*dsa)
                   wss=(-30.*u(i1,i2,i3,ez)+16.*(u(i1+js1,i2+js2,i3+
     & js3,ez)+u(i1-js1,i2-js2,i3-js3,ez))-(u(i1+2*js1,i2+2*js2,i3+2*
     & js3,ez)+u(i1-2*js1,i2-2*js2,i3-2*js3,ez)))/(12.*dsa**2)
                   wsss=(u(i1+2*js1,i2+2*js2,i3+2*js3,ez)-2.*u(i1+js1,
     & i2+js2,i3+js3,ez)+2.*u(i1-js1,i2-js2,i3-js3,ez)-u(i1-2*js1,i2-
     & 2*js2,i3-2*js3,ez))/(2.*dsa**3)
                   ut=(8.*(u(i1+ks1,i2+ks2,i3+ks3,ex)-u(i1-ks1,i2-ks2,
     & i3-ks3,ex))-(u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ex)-u(i1-2*ks1,i2-2*
     & ks2,i3-2*ks3,ex)))/(12.*dta)
                   utt=(-30.*u(i1,i2,i3,ex)+16.*(u(i1+ks1,i2+ks2,i3+
     & ks3,ex)+u(i1-ks1,i2-ks2,i3-ks3,ex))-(u(i1+2*ks1,i2+2*ks2,i3+2*
     & ks3,ex)+u(i1-2*ks1,i2-2*ks2,i3-2*ks3,ex)))/(12.*dta**2)
                   uttt=(u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ex)-2.*u(i1+ks1,
     & i2+ks2,i3+ks3,ex)+2.*u(i1-ks1,i2-ks2,i3-ks3,ex)-u(i1-2*ks1,i2-
     & 2*ks2,i3-2*ks3,ex))/(2.*dta**3)
                   vt=(8.*(u(i1+ks1,i2+ks2,i3+ks3,ey)-u(i1-ks1,i2-ks2,
     & i3-ks3,ey))-(u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ey)-u(i1-2*ks1,i2-2*
     & ks2,i3-2*ks3,ey)))/(12.*dta)
                   vtt=(-30.*u(i1,i2,i3,ey)+16.*(u(i1+ks1,i2+ks2,i3+
     & ks3,ey)+u(i1-ks1,i2-ks2,i3-ks3,ey))-(u(i1+2*ks1,i2+2*ks2,i3+2*
     & ks3,ey)+u(i1-2*ks1,i2-2*ks2,i3-2*ks3,ey)))/(12.*dta**2)
                   vttt=(u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ey)-2.*u(i1+ks1,
     & i2+ks2,i3+ks3,ey)+2.*u(i1-ks1,i2-ks2,i3-ks3,ey)-u(i1-2*ks1,i2-
     & 2*ks2,i3-2*ks3,ey))/(2.*dta**3)
                   wt=(8.*(u(i1+ks1,i2+ks2,i3+ks3,ez)-u(i1-ks1,i2-ks2,
     & i3-ks3,ez))-(u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ez)-u(i1-2*ks1,i2-2*
     & ks2,i3-2*ks3,ez)))/(12.*dta)
                   wtt=(-30.*u(i1,i2,i3,ez)+16.*(u(i1+ks1,i2+ks2,i3+
     & ks3,ez)+u(i1-ks1,i2-ks2,i3-ks3,ez))-(u(i1+2*ks1,i2+2*ks2,i3+2*
     & ks3,ez)+u(i1-2*ks1,i2-2*ks2,i3-2*ks3,ez)))/(12.*dta**2)
                   wttt=(u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ez)-2.*u(i1+ks1,
     & i2+ks2,i3+ks3,ez)+2.*u(i1-ks1,i2-ks2,i3-ks3,ez)-u(i1-2*ks1,i2-
     & 2*ks2,i3-2*ks3,ez))/(2.*dta**3)
                  if( axis.eq.0 )then
                     a11rs = (8.*((8.*((rsxy(i1+1,i2+1,i3,axis,0)*
     & jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axis,0)*
     & jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,axis,0)*
     & jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,axis,0)*
     & jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+
     & 1,i2-1,i3,axis,0)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-(rsxy(i1-1,
     & i2-1,i3,axis,0)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))-((rsxy(i1+2,
     & i2-1,i3,axis,0)*jac3di(i1+2-i10,i2-1-i20,i3-i30))-(rsxy(i1-2,
     & i2-1,i3,axis,0)*jac3di(i1-2-i10,i2-1-i20,i3-i30))))/(12.*dr(0))
     & )-((8.*((rsxy(i1+1,i2+2,i3,axis,0)*jac3di(i1+1-i10,i2+2-i20,i3-
     & i30))-(rsxy(i1-1,i2+2,i3,axis,0)*jac3di(i1-1-i10,i2+2-i20,i3-
     & i30)))-((rsxy(i1+2,i2+2,i3,axis,0)*jac3di(i1+2-i10,i2+2-i20,i3-
     & i30))-(rsxy(i1-2,i2+2,i3,axis,0)*jac3di(i1-2-i10,i2+2-i20,i3-
     & i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,i3,axis,0)*jac3di(i1+
     & 1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,i3,axis,0)*jac3di(i1-1-
     & i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-2,i3,axis,0)*jac3di(i1+2-
     & i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-2,i3,axis,0)*jac3di(i1-2-
     & i10,i2-2-i20,i3-i30))))/(12.*dr(0))))/(12.*dr(1))
                     a12rs = (8.*((8.*((rsxy(i1+1,i2+1,i3,axis,1)*
     & jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axis,1)*
     & jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,axis,1)*
     & jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,axis,1)*
     & jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+
     & 1,i2-1,i3,axis,1)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-(rsxy(i1-1,
     & i2-1,i3,axis,1)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))-((rsxy(i1+2,
     & i2-1,i3,axis,1)*jac3di(i1+2-i10,i2-1-i20,i3-i30))-(rsxy(i1-2,
     & i2-1,i3,axis,1)*jac3di(i1-2-i10,i2-1-i20,i3-i30))))/(12.*dr(0))
     & )-((8.*((rsxy(i1+1,i2+2,i3,axis,1)*jac3di(i1+1-i10,i2+2-i20,i3-
     & i30))-(rsxy(i1-1,i2+2,i3,axis,1)*jac3di(i1-1-i10,i2+2-i20,i3-
     & i30)))-((rsxy(i1+2,i2+2,i3,axis,1)*jac3di(i1+2-i10,i2+2-i20,i3-
     & i30))-(rsxy(i1-2,i2+2,i3,axis,1)*jac3di(i1-2-i10,i2+2-i20,i3-
     & i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,i3,axis,1)*jac3di(i1+
     & 1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,i3,axis,1)*jac3di(i1-1-
     & i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-2,i3,axis,1)*jac3di(i1+2-
     & i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-2,i3,axis,1)*jac3di(i1-2-
     & i10,i2-2-i20,i3-i30))))/(12.*dr(0))))/(12.*dr(1))
                     a13rs = (8.*((8.*((rsxy(i1+1,i2+1,i3,axis,2)*
     & jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axis,2)*
     & jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,axis,2)*
     & jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,axis,2)*
     & jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+
     & 1,i2-1,i3,axis,2)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-(rsxy(i1-1,
     & i2-1,i3,axis,2)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))-((rsxy(i1+2,
     & i2-1,i3,axis,2)*jac3di(i1+2-i10,i2-1-i20,i3-i30))-(rsxy(i1-2,
     & i2-1,i3,axis,2)*jac3di(i1-2-i10,i2-1-i20,i3-i30))))/(12.*dr(0))
     & )-((8.*((rsxy(i1+1,i2+2,i3,axis,2)*jac3di(i1+1-i10,i2+2-i20,i3-
     & i30))-(rsxy(i1-1,i2+2,i3,axis,2)*jac3di(i1-1-i10,i2+2-i20,i3-
     & i30)))-((rsxy(i1+2,i2+2,i3,axis,2)*jac3di(i1+2-i10,i2+2-i20,i3-
     & i30))-(rsxy(i1-2,i2+2,i3,axis,2)*jac3di(i1-2-i10,i2+2-i20,i3-
     & i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,i3,axis,2)*jac3di(i1+
     & 1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,i3,axis,2)*jac3di(i1-1-
     & i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-2,i3,axis,2)*jac3di(i1+2-
     & i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-2,i3,axis,2)*jac3di(i1-2-
     & i10,i2-2-i20,i3-i30))))/(12.*dr(0))))/(12.*dr(1))
                     a21rs = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp1,0)*
     & jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axisp1,0)*
     & jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,axisp1,
     & 0)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,axisp1,
     & 0)*jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(
     & i1+1,i2-1,i3,axisp1,0)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-(rsxy(
     & i1-1,i2-1,i3,axisp1,0)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))-((
     & rsxy(i1+2,i2-1,i3,axisp1,0)*jac3di(i1+2-i10,i2-1-i20,i3-i30))-(
     & rsxy(i1-2,i2-1,i3,axisp1,0)*jac3di(i1-2-i10,i2-1-i20,i3-i30))))
     & /(12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp1,0)*jac3di(i1+1-
     & i10,i2+2-i20,i3-i30))-(rsxy(i1-1,i2+2,i3,axisp1,0)*jac3di(i1-1-
     & i10,i2+2-i20,i3-i30)))-((rsxy(i1+2,i2+2,i3,axisp1,0)*jac3di(i1+
     & 2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,i2+2,i3,axisp1,0)*jac3di(i1-
     & 2-i10,i2+2-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,i3,
     & axisp1,0)*jac3di(i1+1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,i3,
     & axisp1,0)*jac3di(i1-1-i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-2,
     & i3,axisp1,0)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-2,
     & i3,axisp1,0)*jac3di(i1-2-i10,i2-2-i20,i3-i30))))/(12.*dr(0))))
     & /(12.*dr(1))
                     a22rs = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp1,1)*
     & jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axisp1,1)*
     & jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,axisp1,
     & 1)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,axisp1,
     & 1)*jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(
     & i1+1,i2-1,i3,axisp1,1)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-(rsxy(
     & i1-1,i2-1,i3,axisp1,1)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))-((
     & rsxy(i1+2,i2-1,i3,axisp1,1)*jac3di(i1+2-i10,i2-1-i20,i3-i30))-(
     & rsxy(i1-2,i2-1,i3,axisp1,1)*jac3di(i1-2-i10,i2-1-i20,i3-i30))))
     & /(12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp1,1)*jac3di(i1+1-
     & i10,i2+2-i20,i3-i30))-(rsxy(i1-1,i2+2,i3,axisp1,1)*jac3di(i1-1-
     & i10,i2+2-i20,i3-i30)))-((rsxy(i1+2,i2+2,i3,axisp1,1)*jac3di(i1+
     & 2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,i2+2,i3,axisp1,1)*jac3di(i1-
     & 2-i10,i2+2-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,i3,
     & axisp1,1)*jac3di(i1+1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,i3,
     & axisp1,1)*jac3di(i1-1-i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-2,
     & i3,axisp1,1)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-2,
     & i3,axisp1,1)*jac3di(i1-2-i10,i2-2-i20,i3-i30))))/(12.*dr(0))))
     & /(12.*dr(1))
                     a23rs = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp1,2)*
     & jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axisp1,2)*
     & jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,axisp1,
     & 2)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,axisp1,
     & 2)*jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(
     & i1+1,i2-1,i3,axisp1,2)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-(rsxy(
     & i1-1,i2-1,i3,axisp1,2)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))-((
     & rsxy(i1+2,i2-1,i3,axisp1,2)*jac3di(i1+2-i10,i2-1-i20,i3-i30))-(
     & rsxy(i1-2,i2-1,i3,axisp1,2)*jac3di(i1-2-i10,i2-1-i20,i3-i30))))
     & /(12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp1,2)*jac3di(i1+1-
     & i10,i2+2-i20,i3-i30))-(rsxy(i1-1,i2+2,i3,axisp1,2)*jac3di(i1-1-
     & i10,i2+2-i20,i3-i30)))-((rsxy(i1+2,i2+2,i3,axisp1,2)*jac3di(i1+
     & 2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,i2+2,i3,axisp1,2)*jac3di(i1-
     & 2-i10,i2+2-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,i3,
     & axisp1,2)*jac3di(i1+1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,i3,
     & axisp1,2)*jac3di(i1-1-i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-2,
     & i3,axisp1,2)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-2,
     & i3,axisp1,2)*jac3di(i1-2-i10,i2-2-i20,i3-i30))))/(12.*dr(0))))
     & /(12.*dr(1))
                     a31rs = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp2,0)*
     & jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axisp2,0)*
     & jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,axisp2,
     & 0)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,axisp2,
     & 0)*jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(
     & i1+1,i2-1,i3,axisp2,0)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-(rsxy(
     & i1-1,i2-1,i3,axisp2,0)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))-((
     & rsxy(i1+2,i2-1,i3,axisp2,0)*jac3di(i1+2-i10,i2-1-i20,i3-i30))-(
     & rsxy(i1-2,i2-1,i3,axisp2,0)*jac3di(i1-2-i10,i2-1-i20,i3-i30))))
     & /(12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp2,0)*jac3di(i1+1-
     & i10,i2+2-i20,i3-i30))-(rsxy(i1-1,i2+2,i3,axisp2,0)*jac3di(i1-1-
     & i10,i2+2-i20,i3-i30)))-((rsxy(i1+2,i2+2,i3,axisp2,0)*jac3di(i1+
     & 2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,i2+2,i3,axisp2,0)*jac3di(i1-
     & 2-i10,i2+2-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,i3,
     & axisp2,0)*jac3di(i1+1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,i3,
     & axisp2,0)*jac3di(i1-1-i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-2,
     & i3,axisp2,0)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-2,
     & i3,axisp2,0)*jac3di(i1-2-i10,i2-2-i20,i3-i30))))/(12.*dr(0))))
     & /(12.*dr(1))
                     a32rs = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp2,1)*
     & jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axisp2,1)*
     & jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,axisp2,
     & 1)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,axisp2,
     & 1)*jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(
     & i1+1,i2-1,i3,axisp2,1)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-(rsxy(
     & i1-1,i2-1,i3,axisp2,1)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))-((
     & rsxy(i1+2,i2-1,i3,axisp2,1)*jac3di(i1+2-i10,i2-1-i20,i3-i30))-(
     & rsxy(i1-2,i2-1,i3,axisp2,1)*jac3di(i1-2-i10,i2-1-i20,i3-i30))))
     & /(12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp2,1)*jac3di(i1+1-
     & i10,i2+2-i20,i3-i30))-(rsxy(i1-1,i2+2,i3,axisp2,1)*jac3di(i1-1-
     & i10,i2+2-i20,i3-i30)))-((rsxy(i1+2,i2+2,i3,axisp2,1)*jac3di(i1+
     & 2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,i2+2,i3,axisp2,1)*jac3di(i1-
     & 2-i10,i2+2-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,i3,
     & axisp2,1)*jac3di(i1+1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,i3,
     & axisp2,1)*jac3di(i1-1-i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-2,
     & i3,axisp2,1)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-2,
     & i3,axisp2,1)*jac3di(i1-2-i10,i2-2-i20,i3-i30))))/(12.*dr(0))))
     & /(12.*dr(1))
                     a33rs = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp2,2)*
     & jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axisp2,2)*
     & jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,axisp2,
     & 2)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,axisp2,
     & 2)*jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(
     & i1+1,i2-1,i3,axisp2,2)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-(rsxy(
     & i1-1,i2-1,i3,axisp2,2)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))-((
     & rsxy(i1+2,i2-1,i3,axisp2,2)*jac3di(i1+2-i10,i2-1-i20,i3-i30))-(
     & rsxy(i1-2,i2-1,i3,axisp2,2)*jac3di(i1-2-i10,i2-1-i20,i3-i30))))
     & /(12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp2,2)*jac3di(i1+1-
     & i10,i2+2-i20,i3-i30))-(rsxy(i1-1,i2+2,i3,axisp2,2)*jac3di(i1-1-
     & i10,i2+2-i20,i3-i30)))-((rsxy(i1+2,i2+2,i3,axisp2,2)*jac3di(i1+
     & 2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,i2+2,i3,axisp2,2)*jac3di(i1-
     & 2-i10,i2+2-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,i3,
     & axisp2,2)*jac3di(i1+1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,i3,
     & axisp2,2)*jac3di(i1-1-i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-2,
     & i3,axisp2,2)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-2,
     & i3,axisp2,2)*jac3di(i1-2-i10,i2-2-i20,i3-i30))))/(12.*dr(0))))
     & /(12.*dr(1))
                     a11rs = (8.*((8.*((rsxy(i1+1,i2+1,i3,axis,0)*
     & jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axis,0)*
     & jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,axis,0)*
     & jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,axis,0)*
     & jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+
     & 1,i2-1,i3,axis,0)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-(rsxy(i1-1,
     & i2-1,i3,axis,0)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))-((rsxy(i1+2,
     & i2-1,i3,axis,0)*jac3di(i1+2-i10,i2-1-i20,i3-i30))-(rsxy(i1-2,
     & i2-1,i3,axis,0)*jac3di(i1-2-i10,i2-1-i20,i3-i30))))/(12.*dr(0))
     & )-((8.*((rsxy(i1+1,i2+2,i3,axis,0)*jac3di(i1+1-i10,i2+2-i20,i3-
     & i30))-(rsxy(i1-1,i2+2,i3,axis,0)*jac3di(i1-1-i10,i2+2-i20,i3-
     & i30)))-((rsxy(i1+2,i2+2,i3,axis,0)*jac3di(i1+2-i10,i2+2-i20,i3-
     & i30))-(rsxy(i1-2,i2+2,i3,axis,0)*jac3di(i1-2-i10,i2+2-i20,i3-
     & i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,i3,axis,0)*jac3di(i1+
     & 1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,i3,axis,0)*jac3di(i1-1-
     & i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-2,i3,axis,0)*jac3di(i1+2-
     & i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-2,i3,axis,0)*jac3di(i1-2-
     & i10,i2-2-i20,i3-i30))))/(12.*dr(0))))/(12.*dr(1))
                     a12rs = (8.*((8.*((rsxy(i1+1,i2+1,i3,axis,1)*
     & jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axis,1)*
     & jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,axis,1)*
     & jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,axis,1)*
     & jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+
     & 1,i2-1,i3,axis,1)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-(rsxy(i1-1,
     & i2-1,i3,axis,1)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))-((rsxy(i1+2,
     & i2-1,i3,axis,1)*jac3di(i1+2-i10,i2-1-i20,i3-i30))-(rsxy(i1-2,
     & i2-1,i3,axis,1)*jac3di(i1-2-i10,i2-1-i20,i3-i30))))/(12.*dr(0))
     & )-((8.*((rsxy(i1+1,i2+2,i3,axis,1)*jac3di(i1+1-i10,i2+2-i20,i3-
     & i30))-(rsxy(i1-1,i2+2,i3,axis,1)*jac3di(i1-1-i10,i2+2-i20,i3-
     & i30)))-((rsxy(i1+2,i2+2,i3,axis,1)*jac3di(i1+2-i10,i2+2-i20,i3-
     & i30))-(rsxy(i1-2,i2+2,i3,axis,1)*jac3di(i1-2-i10,i2+2-i20,i3-
     & i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,i3,axis,1)*jac3di(i1+
     & 1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,i3,axis,1)*jac3di(i1-1-
     & i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-2,i3,axis,1)*jac3di(i1+2-
     & i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-2,i3,axis,1)*jac3di(i1-2-
     & i10,i2-2-i20,i3-i30))))/(12.*dr(0))))/(12.*dr(1))
                     a13rs = (8.*((8.*((rsxy(i1+1,i2+1,i3,axis,2)*
     & jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axis,2)*
     & jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,axis,2)*
     & jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,axis,2)*
     & jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+
     & 1,i2-1,i3,axis,2)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-(rsxy(i1-1,
     & i2-1,i3,axis,2)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))-((rsxy(i1+2,
     & i2-1,i3,axis,2)*jac3di(i1+2-i10,i2-1-i20,i3-i30))-(rsxy(i1-2,
     & i2-1,i3,axis,2)*jac3di(i1-2-i10,i2-1-i20,i3-i30))))/(12.*dr(0))
     & )-((8.*((rsxy(i1+1,i2+2,i3,axis,2)*jac3di(i1+1-i10,i2+2-i20,i3-
     & i30))-(rsxy(i1-1,i2+2,i3,axis,2)*jac3di(i1-1-i10,i2+2-i20,i3-
     & i30)))-((rsxy(i1+2,i2+2,i3,axis,2)*jac3di(i1+2-i10,i2+2-i20,i3-
     & i30))-(rsxy(i1-2,i2+2,i3,axis,2)*jac3di(i1-2-i10,i2+2-i20,i3-
     & i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,i3,axis,2)*jac3di(i1+
     & 1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,i3,axis,2)*jac3di(i1-1-
     & i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-2,i3,axis,2)*jac3di(i1+2-
     & i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-2,i3,axis,2)*jac3di(i1-2-
     & i10,i2-2-i20,i3-i30))))/(12.*dr(0))))/(12.*dr(1))
                     a21rt = (8.*((8.*((rsxy(i1+1,i2,i3+1,axisp1,0)*
     & jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axisp1,0)*
     & jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,axisp1,
     & 0)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,axisp1,
     & 0)*jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*((rsxy(
     & i1+1,i2,i3-1,axisp1,0)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-(rsxy(
     & i1-1,i2,i3-1,axisp1,0)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))-((
     & rsxy(i1+2,i2,i3-1,axisp1,0)*jac3di(i1+2-i10,i2-i20,i3-1-i30))-(
     & rsxy(i1-2,i2,i3-1,axisp1,0)*jac3di(i1-2-i10,i2-i20,i3-1-i30))))
     & /(12.*dr(0)))-((8.*((rsxy(i1+1,i2,i3+2,axisp1,0)*jac3di(i1+1-
     & i10,i2-i20,i3+2-i30))-(rsxy(i1-1,i2,i3+2,axisp1,0)*jac3di(i1-1-
     & i10,i2-i20,i3+2-i30)))-((rsxy(i1+2,i2,i3+2,axisp1,0)*jac3di(i1+
     & 2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,i2,i3+2,axisp1,0)*jac3di(i1-
     & 2-i10,i2-i20,i3+2-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-2,
     & axisp1,0)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-2,
     & axisp1,0)*jac3di(i1-1-i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,i3-
     & 2,axisp1,0)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,i3-
     & 2,axisp1,0)*jac3di(i1-2-i10,i2-i20,i3-2-i30))))/(12.*dr(0))))/(
     & 12.*dr(2))
                     a22rt = (8.*((8.*((rsxy(i1+1,i2,i3+1,axisp1,1)*
     & jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axisp1,1)*
     & jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,axisp1,
     & 1)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,axisp1,
     & 1)*jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*((rsxy(
     & i1+1,i2,i3-1,axisp1,1)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-(rsxy(
     & i1-1,i2,i3-1,axisp1,1)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))-((
     & rsxy(i1+2,i2,i3-1,axisp1,1)*jac3di(i1+2-i10,i2-i20,i3-1-i30))-(
     & rsxy(i1-2,i2,i3-1,axisp1,1)*jac3di(i1-2-i10,i2-i20,i3-1-i30))))
     & /(12.*dr(0)))-((8.*((rsxy(i1+1,i2,i3+2,axisp1,1)*jac3di(i1+1-
     & i10,i2-i20,i3+2-i30))-(rsxy(i1-1,i2,i3+2,axisp1,1)*jac3di(i1-1-
     & i10,i2-i20,i3+2-i30)))-((rsxy(i1+2,i2,i3+2,axisp1,1)*jac3di(i1+
     & 2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,i2,i3+2,axisp1,1)*jac3di(i1-
     & 2-i10,i2-i20,i3+2-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-2,
     & axisp1,1)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-2,
     & axisp1,1)*jac3di(i1-1-i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,i3-
     & 2,axisp1,1)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,i3-
     & 2,axisp1,1)*jac3di(i1-2-i10,i2-i20,i3-2-i30))))/(12.*dr(0))))/(
     & 12.*dr(2))
                     a23rt = (8.*((8.*((rsxy(i1+1,i2,i3+1,axisp1,2)*
     & jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axisp1,2)*
     & jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,axisp1,
     & 2)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,axisp1,
     & 2)*jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*((rsxy(
     & i1+1,i2,i3-1,axisp1,2)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-(rsxy(
     & i1-1,i2,i3-1,axisp1,2)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))-((
     & rsxy(i1+2,i2,i3-1,axisp1,2)*jac3di(i1+2-i10,i2-i20,i3-1-i30))-(
     & rsxy(i1-2,i2,i3-1,axisp1,2)*jac3di(i1-2-i10,i2-i20,i3-1-i30))))
     & /(12.*dr(0)))-((8.*((rsxy(i1+1,i2,i3+2,axisp1,2)*jac3di(i1+1-
     & i10,i2-i20,i3+2-i30))-(rsxy(i1-1,i2,i3+2,axisp1,2)*jac3di(i1-1-
     & i10,i2-i20,i3+2-i30)))-((rsxy(i1+2,i2,i3+2,axisp1,2)*jac3di(i1+
     & 2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,i2,i3+2,axisp1,2)*jac3di(i1-
     & 2-i10,i2-i20,i3+2-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-2,
     & axisp1,2)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-2,
     & axisp1,2)*jac3di(i1-1-i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,i3-
     & 2,axisp1,2)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,i3-
     & 2,axisp1,2)*jac3di(i1-2-i10,i2-i20,i3-2-i30))))/(12.*dr(0))))/(
     & 12.*dr(2))
                     a31rt = (8.*((8.*((rsxy(i1+1,i2,i3+1,axisp2,0)*
     & jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axisp2,0)*
     & jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,axisp2,
     & 0)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,axisp2,
     & 0)*jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*((rsxy(
     & i1+1,i2,i3-1,axisp2,0)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-(rsxy(
     & i1-1,i2,i3-1,axisp2,0)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))-((
     & rsxy(i1+2,i2,i3-1,axisp2,0)*jac3di(i1+2-i10,i2-i20,i3-1-i30))-(
     & rsxy(i1-2,i2,i3-1,axisp2,0)*jac3di(i1-2-i10,i2-i20,i3-1-i30))))
     & /(12.*dr(0)))-((8.*((rsxy(i1+1,i2,i3+2,axisp2,0)*jac3di(i1+1-
     & i10,i2-i20,i3+2-i30))-(rsxy(i1-1,i2,i3+2,axisp2,0)*jac3di(i1-1-
     & i10,i2-i20,i3+2-i30)))-((rsxy(i1+2,i2,i3+2,axisp2,0)*jac3di(i1+
     & 2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,i2,i3+2,axisp2,0)*jac3di(i1-
     & 2-i10,i2-i20,i3+2-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-2,
     & axisp2,0)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-2,
     & axisp2,0)*jac3di(i1-1-i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,i3-
     & 2,axisp2,0)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,i3-
     & 2,axisp2,0)*jac3di(i1-2-i10,i2-i20,i3-2-i30))))/(12.*dr(0))))/(
     & 12.*dr(2))
                     a32rt = (8.*((8.*((rsxy(i1+1,i2,i3+1,axisp2,1)*
     & jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axisp2,1)*
     & jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,axisp2,
     & 1)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,axisp2,
     & 1)*jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*((rsxy(
     & i1+1,i2,i3-1,axisp2,1)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-(rsxy(
     & i1-1,i2,i3-1,axisp2,1)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))-((
     & rsxy(i1+2,i2,i3-1,axisp2,1)*jac3di(i1+2-i10,i2-i20,i3-1-i30))-(
     & rsxy(i1-2,i2,i3-1,axisp2,1)*jac3di(i1-2-i10,i2-i20,i3-1-i30))))
     & /(12.*dr(0)))-((8.*((rsxy(i1+1,i2,i3+2,axisp2,1)*jac3di(i1+1-
     & i10,i2-i20,i3+2-i30))-(rsxy(i1-1,i2,i3+2,axisp2,1)*jac3di(i1-1-
     & i10,i2-i20,i3+2-i30)))-((rsxy(i1+2,i2,i3+2,axisp2,1)*jac3di(i1+
     & 2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,i2,i3+2,axisp2,1)*jac3di(i1-
     & 2-i10,i2-i20,i3+2-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-2,
     & axisp2,1)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-2,
     & axisp2,1)*jac3di(i1-1-i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,i3-
     & 2,axisp2,1)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,i3-
     & 2,axisp2,1)*jac3di(i1-2-i10,i2-i20,i3-2-i30))))/(12.*dr(0))))/(
     & 12.*dr(2))
                     a33rt = (8.*((8.*((rsxy(i1+1,i2,i3+1,axisp2,2)*
     & jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axisp2,2)*
     & jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,axisp2,
     & 2)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,axisp2,
     & 2)*jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*((rsxy(
     & i1+1,i2,i3-1,axisp2,2)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-(rsxy(
     & i1-1,i2,i3-1,axisp2,2)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))-((
     & rsxy(i1+2,i2,i3-1,axisp2,2)*jac3di(i1+2-i10,i2-i20,i3-1-i30))-(
     & rsxy(i1-2,i2,i3-1,axisp2,2)*jac3di(i1-2-i10,i2-i20,i3-1-i30))))
     & /(12.*dr(0)))-((8.*((rsxy(i1+1,i2,i3+2,axisp2,2)*jac3di(i1+1-
     & i10,i2-i20,i3+2-i30))-(rsxy(i1-1,i2,i3+2,axisp2,2)*jac3di(i1-1-
     & i10,i2-i20,i3+2-i30)))-((rsxy(i1+2,i2,i3+2,axisp2,2)*jac3di(i1+
     & 2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,i2,i3+2,axisp2,2)*jac3di(i1-
     & 2-i10,i2-i20,i3+2-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-2,
     & axisp2,2)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-2,
     & axisp2,2)*jac3di(i1-1-i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,i3-
     & 2,axisp2,2)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,i3-
     & 2,axisp2,2)*jac3di(i1-2-i10,i2-i20,i3-2-i30))))/(12.*dr(0))))/(
     & 12.*dr(2))
                     a11st = (8.*((8.*((rsxy(i1,i2+1,i3+1,axis,0)*
     & jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axis,0)*
     & jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,axis,0)*
     & jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,axis,0)*
     & jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*((rsxy(i1,
     & i2+1,i3-1,axis,0)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-(rsxy(i1,
     & i2-1,i3-1,axis,0)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))-((rsxy(i1,
     & i2+2,i3-1,axis,0)*jac3di(i1-i10,i2+2-i20,i3-1-i30))-(rsxy(i1,
     & i2-2,i3-1,axis,0)*jac3di(i1-i10,i2-2-i20,i3-1-i30))))/(12.*dr(
     & 1)))-((8.*((rsxy(i1,i2+1,i3+2,axis,0)*jac3di(i1-i10,i2+1-i20,
     & i3+2-i30))-(rsxy(i1,i2-1,i3+2,axis,0)*jac3di(i1-i10,i2-1-i20,
     & i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axis,0)*jac3di(i1-i10,i2+2-i20,
     & i3+2-i30))-(rsxy(i1,i2-2,i3+2,axis,0)*jac3di(i1-i10,i2-2-i20,
     & i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-2,axis,0)*
     & jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-2,axis,0)*
     & jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,i3-2,axis,0)*
     & jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,i3-2,axis,0)*
     & jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1))))/(12.*dr(2))
                     a12st = (8.*((8.*((rsxy(i1,i2+1,i3+1,axis,1)*
     & jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axis,1)*
     & jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,axis,1)*
     & jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,axis,1)*
     & jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*((rsxy(i1,
     & i2+1,i3-1,axis,1)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-(rsxy(i1,
     & i2-1,i3-1,axis,1)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))-((rsxy(i1,
     & i2+2,i3-1,axis,1)*jac3di(i1-i10,i2+2-i20,i3-1-i30))-(rsxy(i1,
     & i2-2,i3-1,axis,1)*jac3di(i1-i10,i2-2-i20,i3-1-i30))))/(12.*dr(
     & 1)))-((8.*((rsxy(i1,i2+1,i3+2,axis,1)*jac3di(i1-i10,i2+1-i20,
     & i3+2-i30))-(rsxy(i1,i2-1,i3+2,axis,1)*jac3di(i1-i10,i2-1-i20,
     & i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axis,1)*jac3di(i1-i10,i2+2-i20,
     & i3+2-i30))-(rsxy(i1,i2-2,i3+2,axis,1)*jac3di(i1-i10,i2-2-i20,
     & i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-2,axis,1)*
     & jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-2,axis,1)*
     & jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,i3-2,axis,1)*
     & jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,i3-2,axis,1)*
     & jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1))))/(12.*dr(2))
                     a13st = (8.*((8.*((rsxy(i1,i2+1,i3+1,axis,2)*
     & jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axis,2)*
     & jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,axis,2)*
     & jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,axis,2)*
     & jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*((rsxy(i1,
     & i2+1,i3-1,axis,2)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-(rsxy(i1,
     & i2-1,i3-1,axis,2)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))-((rsxy(i1,
     & i2+2,i3-1,axis,2)*jac3di(i1-i10,i2+2-i20,i3-1-i30))-(rsxy(i1,
     & i2-2,i3-1,axis,2)*jac3di(i1-i10,i2-2-i20,i3-1-i30))))/(12.*dr(
     & 1)))-((8.*((rsxy(i1,i2+1,i3+2,axis,2)*jac3di(i1-i10,i2+1-i20,
     & i3+2-i30))-(rsxy(i1,i2-1,i3+2,axis,2)*jac3di(i1-i10,i2-1-i20,
     & i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axis,2)*jac3di(i1-i10,i2+2-i20,
     & i3+2-i30))-(rsxy(i1,i2-2,i3+2,axis,2)*jac3di(i1-i10,i2-2-i20,
     & i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-2,axis,2)*
     & jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-2,axis,2)*
     & jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,i3-2,axis,2)*
     & jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,i3-2,axis,2)*
     & jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1))))/(12.*dr(2))
                     a21st = (8.*((8.*((rsxy(i1,i2+1,i3+1,axisp1,0)*
     & jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axisp1,0)*
     & jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,axisp1,
     & 0)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,axisp1,
     & 0)*jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*((rsxy(
     & i1,i2+1,i3-1,axisp1,0)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-(rsxy(
     & i1,i2-1,i3-1,axisp1,0)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))-((
     & rsxy(i1,i2+2,i3-1,axisp1,0)*jac3di(i1-i10,i2+2-i20,i3-1-i30))-(
     & rsxy(i1,i2-2,i3-1,axisp1,0)*jac3di(i1-i10,i2-2-i20,i3-1-i30))))
     & /(12.*dr(1)))-((8.*((rsxy(i1,i2+1,i3+2,axisp1,0)*jac3di(i1-i10,
     & i2+1-i20,i3+2-i30))-(rsxy(i1,i2-1,i3+2,axisp1,0)*jac3di(i1-i10,
     & i2-1-i20,i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axisp1,0)*jac3di(i1-
     & i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp1,0)*jac3di(i1-
     & i10,i2-2-i20,i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-2,
     & axisp1,0)*jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-2,
     & axisp1,0)*jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,i3-
     & 2,axisp1,0)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,i3-
     & 2,axisp1,0)*jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1))))/(
     & 12.*dr(2))
                     a22st = (8.*((8.*((rsxy(i1,i2+1,i3+1,axisp1,1)*
     & jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axisp1,1)*
     & jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,axisp1,
     & 1)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,axisp1,
     & 1)*jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*((rsxy(
     & i1,i2+1,i3-1,axisp1,1)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-(rsxy(
     & i1,i2-1,i3-1,axisp1,1)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))-((
     & rsxy(i1,i2+2,i3-1,axisp1,1)*jac3di(i1-i10,i2+2-i20,i3-1-i30))-(
     & rsxy(i1,i2-2,i3-1,axisp1,1)*jac3di(i1-i10,i2-2-i20,i3-1-i30))))
     & /(12.*dr(1)))-((8.*((rsxy(i1,i2+1,i3+2,axisp1,1)*jac3di(i1-i10,
     & i2+1-i20,i3+2-i30))-(rsxy(i1,i2-1,i3+2,axisp1,1)*jac3di(i1-i10,
     & i2-1-i20,i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axisp1,1)*jac3di(i1-
     & i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp1,1)*jac3di(i1-
     & i10,i2-2-i20,i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-2,
     & axisp1,1)*jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-2,
     & axisp1,1)*jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,i3-
     & 2,axisp1,1)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,i3-
     & 2,axisp1,1)*jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1))))/(
     & 12.*dr(2))
                     a23st = (8.*((8.*((rsxy(i1,i2+1,i3+1,axisp1,2)*
     & jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axisp1,2)*
     & jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,axisp1,
     & 2)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,axisp1,
     & 2)*jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*((rsxy(
     & i1,i2+1,i3-1,axisp1,2)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-(rsxy(
     & i1,i2-1,i3-1,axisp1,2)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))-((
     & rsxy(i1,i2+2,i3-1,axisp1,2)*jac3di(i1-i10,i2+2-i20,i3-1-i30))-(
     & rsxy(i1,i2-2,i3-1,axisp1,2)*jac3di(i1-i10,i2-2-i20,i3-1-i30))))
     & /(12.*dr(1)))-((8.*((rsxy(i1,i2+1,i3+2,axisp1,2)*jac3di(i1-i10,
     & i2+1-i20,i3+2-i30))-(rsxy(i1,i2-1,i3+2,axisp1,2)*jac3di(i1-i10,
     & i2-1-i20,i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axisp1,2)*jac3di(i1-
     & i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp1,2)*jac3di(i1-
     & i10,i2-2-i20,i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-2,
     & axisp1,2)*jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-2,
     & axisp1,2)*jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,i3-
     & 2,axisp1,2)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,i3-
     & 2,axisp1,2)*jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1))))/(
     & 12.*dr(2))
                     a31st = (8.*((8.*((rsxy(i1,i2+1,i3+1,axisp2,0)*
     & jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axisp2,0)*
     & jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,axisp2,
     & 0)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,axisp2,
     & 0)*jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*((rsxy(
     & i1,i2+1,i3-1,axisp2,0)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-(rsxy(
     & i1,i2-1,i3-1,axisp2,0)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))-((
     & rsxy(i1,i2+2,i3-1,axisp2,0)*jac3di(i1-i10,i2+2-i20,i3-1-i30))-(
     & rsxy(i1,i2-2,i3-1,axisp2,0)*jac3di(i1-i10,i2-2-i20,i3-1-i30))))
     & /(12.*dr(1)))-((8.*((rsxy(i1,i2+1,i3+2,axisp2,0)*jac3di(i1-i10,
     & i2+1-i20,i3+2-i30))-(rsxy(i1,i2-1,i3+2,axisp2,0)*jac3di(i1-i10,
     & i2-1-i20,i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axisp2,0)*jac3di(i1-
     & i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp2,0)*jac3di(i1-
     & i10,i2-2-i20,i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-2,
     & axisp2,0)*jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-2,
     & axisp2,0)*jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,i3-
     & 2,axisp2,0)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,i3-
     & 2,axisp2,0)*jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1))))/(
     & 12.*dr(2))
                     a32st = (8.*((8.*((rsxy(i1,i2+1,i3+1,axisp2,1)*
     & jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axisp2,1)*
     & jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,axisp2,
     & 1)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,axisp2,
     & 1)*jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*((rsxy(
     & i1,i2+1,i3-1,axisp2,1)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-(rsxy(
     & i1,i2-1,i3-1,axisp2,1)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))-((
     & rsxy(i1,i2+2,i3-1,axisp2,1)*jac3di(i1-i10,i2+2-i20,i3-1-i30))-(
     & rsxy(i1,i2-2,i3-1,axisp2,1)*jac3di(i1-i10,i2-2-i20,i3-1-i30))))
     & /(12.*dr(1)))-((8.*((rsxy(i1,i2+1,i3+2,axisp2,1)*jac3di(i1-i10,
     & i2+1-i20,i3+2-i30))-(rsxy(i1,i2-1,i3+2,axisp2,1)*jac3di(i1-i10,
     & i2-1-i20,i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axisp2,1)*jac3di(i1-
     & i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp2,1)*jac3di(i1-
     & i10,i2-2-i20,i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-2,
     & axisp2,1)*jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-2,
     & axisp2,1)*jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,i3-
     & 2,axisp2,1)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,i3-
     & 2,axisp2,1)*jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1))))/(
     & 12.*dr(2))
                     a33st = (8.*((8.*((rsxy(i1,i2+1,i3+1,axisp2,2)*
     & jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axisp2,2)*
     & jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,axisp2,
     & 2)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,axisp2,
     & 2)*jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*((rsxy(
     & i1,i2+1,i3-1,axisp2,2)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-(rsxy(
     & i1,i2-1,i3-1,axisp2,2)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))-((
     & rsxy(i1,i2+2,i3-1,axisp2,2)*jac3di(i1-i10,i2+2-i20,i3-1-i30))-(
     & rsxy(i1,i2-2,i3-1,axisp2,2)*jac3di(i1-i10,i2-2-i20,i3-1-i30))))
     & /(12.*dr(1)))-((8.*((rsxy(i1,i2+1,i3+2,axisp2,2)*jac3di(i1-i10,
     & i2+1-i20,i3+2-i30))-(rsxy(i1,i2-1,i3+2,axisp2,2)*jac3di(i1-i10,
     & i2-1-i20,i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axisp2,2)*jac3di(i1-
     & i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp2,2)*jac3di(i1-
     & i10,i2-2-i20,i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-2,
     & axisp2,2)*jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-2,
     & axisp2,2)*jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,i3-
     & 2,axisp2,2)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,i3-
     & 2,axisp2,2)*jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1))))/(
     & 12.*dr(2))
                     a11rss = (128*(rsxy(i1+1,i2+1,i3,axis,0)*jac3di(
     & i1+1-i10,i2+1-i20,i3-i30))-128*(rsxy(i1-1,i2+1,i3,axis,0)*
     & jac3di(i1-1-i10,i2+1-i20,i3-i30))-16*(rsxy(i1+2,i2+1,i3,axis,0)
     & *jac3di(i1+2-i10,i2+1-i20,i3-i30))+16*(rsxy(i1-2,i2+1,i3,axis,
     & 0)*jac3di(i1-2-i10,i2+1-i20,i3-i30))+128*(rsxy(i1+1,i2-1,i3,
     & axis,0)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-128*(rsxy(i1-1,i2-1,
     & i3,axis,0)*jac3di(i1-1-i10,i2-1-i20,i3-i30))-16*(rsxy(i1+2,i2-
     & 1,i3,axis,0)*jac3di(i1+2-i10,i2-1-i20,i3-i30))+16*(rsxy(i1-2,
     & i2-1,i3,axis,0)*jac3di(i1-2-i10,i2-1-i20,i3-i30))+30*(rsxy(i1+
     & 2,i2,i3,axis,0)*jac3di(i1+2-i10,i2-i20,i3-i30))+8*(rsxy(i1-1,
     & i2+2,i3,axis,0)*jac3di(i1-1-i10,i2+2-i20,i3-i30))+(rsxy(i1+2,
     & i2+2,i3,axis,0)*jac3di(i1+2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,
     & i2+2,i3,axis,0)*jac3di(i1-2-i10,i2+2-i20,i3-i30))-8*(rsxy(i1+1,
     & i2-2,i3,axis,0)*jac3di(i1+1-i10,i2-2-i20,i3-i30))+8*(rsxy(i1-1,
     & i2-2,i3,axis,0)*jac3di(i1-1-i10,i2-2-i20,i3-i30))+(rsxy(i1+2,
     & i2-2,i3,axis,0)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,
     & i2-2,i3,axis,0)*jac3di(i1-2-i10,i2-2-i20,i3-i30))-8*(rsxy(i1+1,
     & i2+2,i3,axis,0)*jac3di(i1+1-i10,i2+2-i20,i3-i30))-30*(rsxy(i1-
     & 2,i2,i3,axis,0)*jac3di(i1-2-i10,i2-i20,i3-i30))-240*(rsxy(i1+1,
     & i2,i3,axis,0)*jac3di(i1+1-i10,i2-i20,i3-i30))+240*(rsxy(i1-1,
     & i2,i3,axis,0)*jac3di(i1-1-i10,i2-i20,i3-i30)))/(144.*dr(1)**2*
     & dr(0))
                     a12rss = (128*(rsxy(i1+1,i2+1,i3,axis,1)*jac3di(
     & i1+1-i10,i2+1-i20,i3-i30))-128*(rsxy(i1-1,i2+1,i3,axis,1)*
     & jac3di(i1-1-i10,i2+1-i20,i3-i30))-16*(rsxy(i1+2,i2+1,i3,axis,1)
     & *jac3di(i1+2-i10,i2+1-i20,i3-i30))+16*(rsxy(i1-2,i2+1,i3,axis,
     & 1)*jac3di(i1-2-i10,i2+1-i20,i3-i30))+128*(rsxy(i1+1,i2-1,i3,
     & axis,1)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-128*(rsxy(i1-1,i2-1,
     & i3,axis,1)*jac3di(i1-1-i10,i2-1-i20,i3-i30))-16*(rsxy(i1+2,i2-
     & 1,i3,axis,1)*jac3di(i1+2-i10,i2-1-i20,i3-i30))+16*(rsxy(i1-2,
     & i2-1,i3,axis,1)*jac3di(i1-2-i10,i2-1-i20,i3-i30))+30*(rsxy(i1+
     & 2,i2,i3,axis,1)*jac3di(i1+2-i10,i2-i20,i3-i30))+8*(rsxy(i1-1,
     & i2+2,i3,axis,1)*jac3di(i1-1-i10,i2+2-i20,i3-i30))+(rsxy(i1+2,
     & i2+2,i3,axis,1)*jac3di(i1+2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,
     & i2+2,i3,axis,1)*jac3di(i1-2-i10,i2+2-i20,i3-i30))-8*(rsxy(i1+1,
     & i2-2,i3,axis,1)*jac3di(i1+1-i10,i2-2-i20,i3-i30))+8*(rsxy(i1-1,
     & i2-2,i3,axis,1)*jac3di(i1-1-i10,i2-2-i20,i3-i30))+(rsxy(i1+2,
     & i2-2,i3,axis,1)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,
     & i2-2,i3,axis,1)*jac3di(i1-2-i10,i2-2-i20,i3-i30))-8*(rsxy(i1+1,
     & i2+2,i3,axis,1)*jac3di(i1+1-i10,i2+2-i20,i3-i30))-30*(rsxy(i1-
     & 2,i2,i3,axis,1)*jac3di(i1-2-i10,i2-i20,i3-i30))-240*(rsxy(i1+1,
     & i2,i3,axis,1)*jac3di(i1+1-i10,i2-i20,i3-i30))+240*(rsxy(i1-1,
     & i2,i3,axis,1)*jac3di(i1-1-i10,i2-i20,i3-i30)))/(144.*dr(1)**2*
     & dr(0))
                     a13rss = (128*(rsxy(i1+1,i2+1,i3,axis,2)*jac3di(
     & i1+1-i10,i2+1-i20,i3-i30))-128*(rsxy(i1-1,i2+1,i3,axis,2)*
     & jac3di(i1-1-i10,i2+1-i20,i3-i30))-16*(rsxy(i1+2,i2+1,i3,axis,2)
     & *jac3di(i1+2-i10,i2+1-i20,i3-i30))+16*(rsxy(i1-2,i2+1,i3,axis,
     & 2)*jac3di(i1-2-i10,i2+1-i20,i3-i30))+128*(rsxy(i1+1,i2-1,i3,
     & axis,2)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-128*(rsxy(i1-1,i2-1,
     & i3,axis,2)*jac3di(i1-1-i10,i2-1-i20,i3-i30))-16*(rsxy(i1+2,i2-
     & 1,i3,axis,2)*jac3di(i1+2-i10,i2-1-i20,i3-i30))+16*(rsxy(i1-2,
     & i2-1,i3,axis,2)*jac3di(i1-2-i10,i2-1-i20,i3-i30))+30*(rsxy(i1+
     & 2,i2,i3,axis,2)*jac3di(i1+2-i10,i2-i20,i3-i30))+8*(rsxy(i1-1,
     & i2+2,i3,axis,2)*jac3di(i1-1-i10,i2+2-i20,i3-i30))+(rsxy(i1+2,
     & i2+2,i3,axis,2)*jac3di(i1+2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,
     & i2+2,i3,axis,2)*jac3di(i1-2-i10,i2+2-i20,i3-i30))-8*(rsxy(i1+1,
     & i2-2,i3,axis,2)*jac3di(i1+1-i10,i2-2-i20,i3-i30))+8*(rsxy(i1-1,
     & i2-2,i3,axis,2)*jac3di(i1-1-i10,i2-2-i20,i3-i30))+(rsxy(i1+2,
     & i2-2,i3,axis,2)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,
     & i2-2,i3,axis,2)*jac3di(i1-2-i10,i2-2-i20,i3-i30))-8*(rsxy(i1+1,
     & i2+2,i3,axis,2)*jac3di(i1+1-i10,i2+2-i20,i3-i30))-30*(rsxy(i1-
     & 2,i2,i3,axis,2)*jac3di(i1-2-i10,i2-i20,i3-i30))-240*(rsxy(i1+1,
     & i2,i3,axis,2)*jac3di(i1+1-i10,i2-i20,i3-i30))+240*(rsxy(i1-1,
     & i2,i3,axis,2)*jac3di(i1-1-i10,i2-i20,i3-i30)))/(144.*dr(1)**2*
     & dr(0))
                     a21rss = (128*(rsxy(i1+1,i2+1,i3,axisp1,0)*jac3di(
     & i1+1-i10,i2+1-i20,i3-i30))-128*(rsxy(i1-1,i2+1,i3,axisp1,0)*
     & jac3di(i1-1-i10,i2+1-i20,i3-i30))-16*(rsxy(i1+2,i2+1,i3,axisp1,
     & 0)*jac3di(i1+2-i10,i2+1-i20,i3-i30))+16*(rsxy(i1-2,i2+1,i3,
     & axisp1,0)*jac3di(i1-2-i10,i2+1-i20,i3-i30))+128*(rsxy(i1+1,i2-
     & 1,i3,axisp1,0)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-128*(rsxy(i1-
     & 1,i2-1,i3,axisp1,0)*jac3di(i1-1-i10,i2-1-i20,i3-i30))-16*(rsxy(
     & i1+2,i2-1,i3,axisp1,0)*jac3di(i1+2-i10,i2-1-i20,i3-i30))+16*(
     & rsxy(i1-2,i2-1,i3,axisp1,0)*jac3di(i1-2-i10,i2-1-i20,i3-i30))+
     & 30*(rsxy(i1+2,i2,i3,axisp1,0)*jac3di(i1+2-i10,i2-i20,i3-i30))+
     & 8*(rsxy(i1-1,i2+2,i3,axisp1,0)*jac3di(i1-1-i10,i2+2-i20,i3-i30)
     & )+(rsxy(i1+2,i2+2,i3,axisp1,0)*jac3di(i1+2-i10,i2+2-i20,i3-i30)
     & )-(rsxy(i1-2,i2+2,i3,axisp1,0)*jac3di(i1-2-i10,i2+2-i20,i3-i30)
     & )-8*(rsxy(i1+1,i2-2,i3,axisp1,0)*jac3di(i1+1-i10,i2-2-i20,i3-
     & i30))+8*(rsxy(i1-1,i2-2,i3,axisp1,0)*jac3di(i1-1-i10,i2-2-i20,
     & i3-i30))+(rsxy(i1+2,i2-2,i3,axisp1,0)*jac3di(i1+2-i10,i2-2-i20,
     & i3-i30))-(rsxy(i1-2,i2-2,i3,axisp1,0)*jac3di(i1-2-i10,i2-2-i20,
     & i3-i30))-8*(rsxy(i1+1,i2+2,i3,axisp1,0)*jac3di(i1+1-i10,i2+2-
     & i20,i3-i30))-30*(rsxy(i1-2,i2,i3,axisp1,0)*jac3di(i1-2-i10,i2-
     & i20,i3-i30))-240*(rsxy(i1+1,i2,i3,axisp1,0)*jac3di(i1+1-i10,i2-
     & i20,i3-i30))+240*(rsxy(i1-1,i2,i3,axisp1,0)*jac3di(i1-1-i10,i2-
     & i20,i3-i30)))/(144.*dr(1)**2*dr(0))
                     a22rss = (128*(rsxy(i1+1,i2+1,i3,axisp1,1)*jac3di(
     & i1+1-i10,i2+1-i20,i3-i30))-128*(rsxy(i1-1,i2+1,i3,axisp1,1)*
     & jac3di(i1-1-i10,i2+1-i20,i3-i30))-16*(rsxy(i1+2,i2+1,i3,axisp1,
     & 1)*jac3di(i1+2-i10,i2+1-i20,i3-i30))+16*(rsxy(i1-2,i2+1,i3,
     & axisp1,1)*jac3di(i1-2-i10,i2+1-i20,i3-i30))+128*(rsxy(i1+1,i2-
     & 1,i3,axisp1,1)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-128*(rsxy(i1-
     & 1,i2-1,i3,axisp1,1)*jac3di(i1-1-i10,i2-1-i20,i3-i30))-16*(rsxy(
     & i1+2,i2-1,i3,axisp1,1)*jac3di(i1+2-i10,i2-1-i20,i3-i30))+16*(
     & rsxy(i1-2,i2-1,i3,axisp1,1)*jac3di(i1-2-i10,i2-1-i20,i3-i30))+
     & 30*(rsxy(i1+2,i2,i3,axisp1,1)*jac3di(i1+2-i10,i2-i20,i3-i30))+
     & 8*(rsxy(i1-1,i2+2,i3,axisp1,1)*jac3di(i1-1-i10,i2+2-i20,i3-i30)
     & )+(rsxy(i1+2,i2+2,i3,axisp1,1)*jac3di(i1+2-i10,i2+2-i20,i3-i30)
     & )-(rsxy(i1-2,i2+2,i3,axisp1,1)*jac3di(i1-2-i10,i2+2-i20,i3-i30)
     & )-8*(rsxy(i1+1,i2-2,i3,axisp1,1)*jac3di(i1+1-i10,i2-2-i20,i3-
     & i30))+8*(rsxy(i1-1,i2-2,i3,axisp1,1)*jac3di(i1-1-i10,i2-2-i20,
     & i3-i30))+(rsxy(i1+2,i2-2,i3,axisp1,1)*jac3di(i1+2-i10,i2-2-i20,
     & i3-i30))-(rsxy(i1-2,i2-2,i3,axisp1,1)*jac3di(i1-2-i10,i2-2-i20,
     & i3-i30))-8*(rsxy(i1+1,i2+2,i3,axisp1,1)*jac3di(i1+1-i10,i2+2-
     & i20,i3-i30))-30*(rsxy(i1-2,i2,i3,axisp1,1)*jac3di(i1-2-i10,i2-
     & i20,i3-i30))-240*(rsxy(i1+1,i2,i3,axisp1,1)*jac3di(i1+1-i10,i2-
     & i20,i3-i30))+240*(rsxy(i1-1,i2,i3,axisp1,1)*jac3di(i1-1-i10,i2-
     & i20,i3-i30)))/(144.*dr(1)**2*dr(0))
                     a23rss = (128*(rsxy(i1+1,i2+1,i3,axisp1,2)*jac3di(
     & i1+1-i10,i2+1-i20,i3-i30))-128*(rsxy(i1-1,i2+1,i3,axisp1,2)*
     & jac3di(i1-1-i10,i2+1-i20,i3-i30))-16*(rsxy(i1+2,i2+1,i3,axisp1,
     & 2)*jac3di(i1+2-i10,i2+1-i20,i3-i30))+16*(rsxy(i1-2,i2+1,i3,
     & axisp1,2)*jac3di(i1-2-i10,i2+1-i20,i3-i30))+128*(rsxy(i1+1,i2-
     & 1,i3,axisp1,2)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-128*(rsxy(i1-
     & 1,i2-1,i3,axisp1,2)*jac3di(i1-1-i10,i2-1-i20,i3-i30))-16*(rsxy(
     & i1+2,i2-1,i3,axisp1,2)*jac3di(i1+2-i10,i2-1-i20,i3-i30))+16*(
     & rsxy(i1-2,i2-1,i3,axisp1,2)*jac3di(i1-2-i10,i2-1-i20,i3-i30))+
     & 30*(rsxy(i1+2,i2,i3,axisp1,2)*jac3di(i1+2-i10,i2-i20,i3-i30))+
     & 8*(rsxy(i1-1,i2+2,i3,axisp1,2)*jac3di(i1-1-i10,i2+2-i20,i3-i30)
     & )+(rsxy(i1+2,i2+2,i3,axisp1,2)*jac3di(i1+2-i10,i2+2-i20,i3-i30)
     & )-(rsxy(i1-2,i2+2,i3,axisp1,2)*jac3di(i1-2-i10,i2+2-i20,i3-i30)
     & )-8*(rsxy(i1+1,i2-2,i3,axisp1,2)*jac3di(i1+1-i10,i2-2-i20,i3-
     & i30))+8*(rsxy(i1-1,i2-2,i3,axisp1,2)*jac3di(i1-1-i10,i2-2-i20,
     & i3-i30))+(rsxy(i1+2,i2-2,i3,axisp1,2)*jac3di(i1+2-i10,i2-2-i20,
     & i3-i30))-(rsxy(i1-2,i2-2,i3,axisp1,2)*jac3di(i1-2-i10,i2-2-i20,
     & i3-i30))-8*(rsxy(i1+1,i2+2,i3,axisp1,2)*jac3di(i1+1-i10,i2+2-
     & i20,i3-i30))-30*(rsxy(i1-2,i2,i3,axisp1,2)*jac3di(i1-2-i10,i2-
     & i20,i3-i30))-240*(rsxy(i1+1,i2,i3,axisp1,2)*jac3di(i1+1-i10,i2-
     & i20,i3-i30))+240*(rsxy(i1-1,i2,i3,axisp1,2)*jac3di(i1-1-i10,i2-
     & i20,i3-i30)))/(144.*dr(1)**2*dr(0))
                     a31rss = (128*(rsxy(i1+1,i2+1,i3,axisp2,0)*jac3di(
     & i1+1-i10,i2+1-i20,i3-i30))-128*(rsxy(i1-1,i2+1,i3,axisp2,0)*
     & jac3di(i1-1-i10,i2+1-i20,i3-i30))-16*(rsxy(i1+2,i2+1,i3,axisp2,
     & 0)*jac3di(i1+2-i10,i2+1-i20,i3-i30))+16*(rsxy(i1-2,i2+1,i3,
     & axisp2,0)*jac3di(i1-2-i10,i2+1-i20,i3-i30))+128*(rsxy(i1+1,i2-
     & 1,i3,axisp2,0)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-128*(rsxy(i1-
     & 1,i2-1,i3,axisp2,0)*jac3di(i1-1-i10,i2-1-i20,i3-i30))-16*(rsxy(
     & i1+2,i2-1,i3,axisp2,0)*jac3di(i1+2-i10,i2-1-i20,i3-i30))+16*(
     & rsxy(i1-2,i2-1,i3,axisp2,0)*jac3di(i1-2-i10,i2-1-i20,i3-i30))+
     & 30*(rsxy(i1+2,i2,i3,axisp2,0)*jac3di(i1+2-i10,i2-i20,i3-i30))+
     & 8*(rsxy(i1-1,i2+2,i3,axisp2,0)*jac3di(i1-1-i10,i2+2-i20,i3-i30)
     & )+(rsxy(i1+2,i2+2,i3,axisp2,0)*jac3di(i1+2-i10,i2+2-i20,i3-i30)
     & )-(rsxy(i1-2,i2+2,i3,axisp2,0)*jac3di(i1-2-i10,i2+2-i20,i3-i30)
     & )-8*(rsxy(i1+1,i2-2,i3,axisp2,0)*jac3di(i1+1-i10,i2-2-i20,i3-
     & i30))+8*(rsxy(i1-1,i2-2,i3,axisp2,0)*jac3di(i1-1-i10,i2-2-i20,
     & i3-i30))+(rsxy(i1+2,i2-2,i3,axisp2,0)*jac3di(i1+2-i10,i2-2-i20,
     & i3-i30))-(rsxy(i1-2,i2-2,i3,axisp2,0)*jac3di(i1-2-i10,i2-2-i20,
     & i3-i30))-8*(rsxy(i1+1,i2+2,i3,axisp2,0)*jac3di(i1+1-i10,i2+2-
     & i20,i3-i30))-30*(rsxy(i1-2,i2,i3,axisp2,0)*jac3di(i1-2-i10,i2-
     & i20,i3-i30))-240*(rsxy(i1+1,i2,i3,axisp2,0)*jac3di(i1+1-i10,i2-
     & i20,i3-i30))+240*(rsxy(i1-1,i2,i3,axisp2,0)*jac3di(i1-1-i10,i2-
     & i20,i3-i30)))/(144.*dr(1)**2*dr(0))
                     a32rss = (128*(rsxy(i1+1,i2+1,i3,axisp2,1)*jac3di(
     & i1+1-i10,i2+1-i20,i3-i30))-128*(rsxy(i1-1,i2+1,i3,axisp2,1)*
     & jac3di(i1-1-i10,i2+1-i20,i3-i30))-16*(rsxy(i1+2,i2+1,i3,axisp2,
     & 1)*jac3di(i1+2-i10,i2+1-i20,i3-i30))+16*(rsxy(i1-2,i2+1,i3,
     & axisp2,1)*jac3di(i1-2-i10,i2+1-i20,i3-i30))+128*(rsxy(i1+1,i2-
     & 1,i3,axisp2,1)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-128*(rsxy(i1-
     & 1,i2-1,i3,axisp2,1)*jac3di(i1-1-i10,i2-1-i20,i3-i30))-16*(rsxy(
     & i1+2,i2-1,i3,axisp2,1)*jac3di(i1+2-i10,i2-1-i20,i3-i30))+16*(
     & rsxy(i1-2,i2-1,i3,axisp2,1)*jac3di(i1-2-i10,i2-1-i20,i3-i30))+
     & 30*(rsxy(i1+2,i2,i3,axisp2,1)*jac3di(i1+2-i10,i2-i20,i3-i30))+
     & 8*(rsxy(i1-1,i2+2,i3,axisp2,1)*jac3di(i1-1-i10,i2+2-i20,i3-i30)
     & )+(rsxy(i1+2,i2+2,i3,axisp2,1)*jac3di(i1+2-i10,i2+2-i20,i3-i30)
     & )-(rsxy(i1-2,i2+2,i3,axisp2,1)*jac3di(i1-2-i10,i2+2-i20,i3-i30)
     & )-8*(rsxy(i1+1,i2-2,i3,axisp2,1)*jac3di(i1+1-i10,i2-2-i20,i3-
     & i30))+8*(rsxy(i1-1,i2-2,i3,axisp2,1)*jac3di(i1-1-i10,i2-2-i20,
     & i3-i30))+(rsxy(i1+2,i2-2,i3,axisp2,1)*jac3di(i1+2-i10,i2-2-i20,
     & i3-i30))-(rsxy(i1-2,i2-2,i3,axisp2,1)*jac3di(i1-2-i10,i2-2-i20,
     & i3-i30))-8*(rsxy(i1+1,i2+2,i3,axisp2,1)*jac3di(i1+1-i10,i2+2-
     & i20,i3-i30))-30*(rsxy(i1-2,i2,i3,axisp2,1)*jac3di(i1-2-i10,i2-
     & i20,i3-i30))-240*(rsxy(i1+1,i2,i3,axisp2,1)*jac3di(i1+1-i10,i2-
     & i20,i3-i30))+240*(rsxy(i1-1,i2,i3,axisp2,1)*jac3di(i1-1-i10,i2-
     & i20,i3-i30)))/(144.*dr(1)**2*dr(0))
                     a33rss = (128*(rsxy(i1+1,i2+1,i3,axisp2,2)*jac3di(
     & i1+1-i10,i2+1-i20,i3-i30))-128*(rsxy(i1-1,i2+1,i3,axisp2,2)*
     & jac3di(i1-1-i10,i2+1-i20,i3-i30))-16*(rsxy(i1+2,i2+1,i3,axisp2,
     & 2)*jac3di(i1+2-i10,i2+1-i20,i3-i30))+16*(rsxy(i1-2,i2+1,i3,
     & axisp2,2)*jac3di(i1-2-i10,i2+1-i20,i3-i30))+128*(rsxy(i1+1,i2-
     & 1,i3,axisp2,2)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-128*(rsxy(i1-
     & 1,i2-1,i3,axisp2,2)*jac3di(i1-1-i10,i2-1-i20,i3-i30))-16*(rsxy(
     & i1+2,i2-1,i3,axisp2,2)*jac3di(i1+2-i10,i2-1-i20,i3-i30))+16*(
     & rsxy(i1-2,i2-1,i3,axisp2,2)*jac3di(i1-2-i10,i2-1-i20,i3-i30))+
     & 30*(rsxy(i1+2,i2,i3,axisp2,2)*jac3di(i1+2-i10,i2-i20,i3-i30))+
     & 8*(rsxy(i1-1,i2+2,i3,axisp2,2)*jac3di(i1-1-i10,i2+2-i20,i3-i30)
     & )+(rsxy(i1+2,i2+2,i3,axisp2,2)*jac3di(i1+2-i10,i2+2-i20,i3-i30)
     & )-(rsxy(i1-2,i2+2,i3,axisp2,2)*jac3di(i1-2-i10,i2+2-i20,i3-i30)
     & )-8*(rsxy(i1+1,i2-2,i3,axisp2,2)*jac3di(i1+1-i10,i2-2-i20,i3-
     & i30))+8*(rsxy(i1-1,i2-2,i3,axisp2,2)*jac3di(i1-1-i10,i2-2-i20,
     & i3-i30))+(rsxy(i1+2,i2-2,i3,axisp2,2)*jac3di(i1+2-i10,i2-2-i20,
     & i3-i30))-(rsxy(i1-2,i2-2,i3,axisp2,2)*jac3di(i1-2-i10,i2-2-i20,
     & i3-i30))-8*(rsxy(i1+1,i2+2,i3,axisp2,2)*jac3di(i1+1-i10,i2+2-
     & i20,i3-i30))-30*(rsxy(i1-2,i2,i3,axisp2,2)*jac3di(i1-2-i10,i2-
     & i20,i3-i30))-240*(rsxy(i1+1,i2,i3,axisp2,2)*jac3di(i1+1-i10,i2-
     & i20,i3-i30))+240*(rsxy(i1-1,i2,i3,axisp2,2)*jac3di(i1-1-i10,i2-
     & i20,i3-i30)))/(144.*dr(1)**2*dr(0))
                     a11rtt = (128*(rsxy(i1+1,i2,i3+1,axis,0)*jac3di(
     & i1+1-i10,i2-i20,i3+1-i30))-16*(rsxy(i1+2,i2,i3-1,axis,0)*
     & jac3di(i1+2-i10,i2-i20,i3-1-i30))+16*(rsxy(i1-2,i2,i3-1,axis,0)
     & *jac3di(i1-2-i10,i2-i20,i3-1-i30))-128*(rsxy(i1-1,i2,i3-1,axis,
     & 0)*jac3di(i1-1-i10,i2-i20,i3-1-i30))-16*(rsxy(i1+2,i2,i3+1,
     & axis,0)*jac3di(i1+2-i10,i2-i20,i3+1-i30))+16*(rsxy(i1-2,i2,i3+
     & 1,axis,0)*jac3di(i1-2-i10,i2-i20,i3+1-i30))+128*(rsxy(i1+1,i2,
     & i3-1,axis,0)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-128*(rsxy(i1-1,
     & i2,i3+1,axis,0)*jac3di(i1-1-i10,i2-i20,i3+1-i30))-8*(rsxy(i1+1,
     & i2,i3+2,axis,0)*jac3di(i1+1-i10,i2-i20,i3+2-i30))+8*(rsxy(i1-1,
     & i2,i3+2,axis,0)*jac3di(i1-1-i10,i2-i20,i3+2-i30))+(rsxy(i1+2,
     & i2,i3+2,axis,0)*jac3di(i1+2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,
     & i2,i3+2,axis,0)*jac3di(i1-2-i10,i2-i20,i3+2-i30))-8*(rsxy(i1+1,
     & i2,i3-2,axis,0)*jac3di(i1+1-i10,i2-i20,i3-2-i30))+8*(rsxy(i1-1,
     & i2,i3-2,axis,0)*jac3di(i1-1-i10,i2-i20,i3-2-i30))+(rsxy(i1+2,
     & i2,i3-2,axis,0)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,
     & i2,i3-2,axis,0)*jac3di(i1-2-i10,i2-i20,i3-2-i30))+30*(rsxy(i1+
     & 2,i2,i3,axis,0)*jac3di(i1+2-i10,i2-i20,i3-i30))-30*(rsxy(i1-2,
     & i2,i3,axis,0)*jac3di(i1-2-i10,i2-i20,i3-i30))-240*(rsxy(i1+1,
     & i2,i3,axis,0)*jac3di(i1+1-i10,i2-i20,i3-i30))+240*(rsxy(i1-1,
     & i2,i3,axis,0)*jac3di(i1-1-i10,i2-i20,i3-i30)))/(144.*dr(2)**2*
     & dr(0))
                     a12rtt = (128*(rsxy(i1+1,i2,i3+1,axis,1)*jac3di(
     & i1+1-i10,i2-i20,i3+1-i30))-16*(rsxy(i1+2,i2,i3-1,axis,1)*
     & jac3di(i1+2-i10,i2-i20,i3-1-i30))+16*(rsxy(i1-2,i2,i3-1,axis,1)
     & *jac3di(i1-2-i10,i2-i20,i3-1-i30))-128*(rsxy(i1-1,i2,i3-1,axis,
     & 1)*jac3di(i1-1-i10,i2-i20,i3-1-i30))-16*(rsxy(i1+2,i2,i3+1,
     & axis,1)*jac3di(i1+2-i10,i2-i20,i3+1-i30))+16*(rsxy(i1-2,i2,i3+
     & 1,axis,1)*jac3di(i1-2-i10,i2-i20,i3+1-i30))+128*(rsxy(i1+1,i2,
     & i3-1,axis,1)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-128*(rsxy(i1-1,
     & i2,i3+1,axis,1)*jac3di(i1-1-i10,i2-i20,i3+1-i30))-8*(rsxy(i1+1,
     & i2,i3+2,axis,1)*jac3di(i1+1-i10,i2-i20,i3+2-i30))+8*(rsxy(i1-1,
     & i2,i3+2,axis,1)*jac3di(i1-1-i10,i2-i20,i3+2-i30))+(rsxy(i1+2,
     & i2,i3+2,axis,1)*jac3di(i1+2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,
     & i2,i3+2,axis,1)*jac3di(i1-2-i10,i2-i20,i3+2-i30))-8*(rsxy(i1+1,
     & i2,i3-2,axis,1)*jac3di(i1+1-i10,i2-i20,i3-2-i30))+8*(rsxy(i1-1,
     & i2,i3-2,axis,1)*jac3di(i1-1-i10,i2-i20,i3-2-i30))+(rsxy(i1+2,
     & i2,i3-2,axis,1)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,
     & i2,i3-2,axis,1)*jac3di(i1-2-i10,i2-i20,i3-2-i30))+30*(rsxy(i1+
     & 2,i2,i3,axis,1)*jac3di(i1+2-i10,i2-i20,i3-i30))-30*(rsxy(i1-2,
     & i2,i3,axis,1)*jac3di(i1-2-i10,i2-i20,i3-i30))-240*(rsxy(i1+1,
     & i2,i3,axis,1)*jac3di(i1+1-i10,i2-i20,i3-i30))+240*(rsxy(i1-1,
     & i2,i3,axis,1)*jac3di(i1-1-i10,i2-i20,i3-i30)))/(144.*dr(2)**2*
     & dr(0))
                     a13rtt = (128*(rsxy(i1+1,i2,i3+1,axis,2)*jac3di(
     & i1+1-i10,i2-i20,i3+1-i30))-16*(rsxy(i1+2,i2,i3-1,axis,2)*
     & jac3di(i1+2-i10,i2-i20,i3-1-i30))+16*(rsxy(i1-2,i2,i3-1,axis,2)
     & *jac3di(i1-2-i10,i2-i20,i3-1-i30))-128*(rsxy(i1-1,i2,i3-1,axis,
     & 2)*jac3di(i1-1-i10,i2-i20,i3-1-i30))-16*(rsxy(i1+2,i2,i3+1,
     & axis,2)*jac3di(i1+2-i10,i2-i20,i3+1-i30))+16*(rsxy(i1-2,i2,i3+
     & 1,axis,2)*jac3di(i1-2-i10,i2-i20,i3+1-i30))+128*(rsxy(i1+1,i2,
     & i3-1,axis,2)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-128*(rsxy(i1-1,
     & i2,i3+1,axis,2)*jac3di(i1-1-i10,i2-i20,i3+1-i30))-8*(rsxy(i1+1,
     & i2,i3+2,axis,2)*jac3di(i1+1-i10,i2-i20,i3+2-i30))+8*(rsxy(i1-1,
     & i2,i3+2,axis,2)*jac3di(i1-1-i10,i2-i20,i3+2-i30))+(rsxy(i1+2,
     & i2,i3+2,axis,2)*jac3di(i1+2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,
     & i2,i3+2,axis,2)*jac3di(i1-2-i10,i2-i20,i3+2-i30))-8*(rsxy(i1+1,
     & i2,i3-2,axis,2)*jac3di(i1+1-i10,i2-i20,i3-2-i30))+8*(rsxy(i1-1,
     & i2,i3-2,axis,2)*jac3di(i1-1-i10,i2-i20,i3-2-i30))+(rsxy(i1+2,
     & i2,i3-2,axis,2)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,
     & i2,i3-2,axis,2)*jac3di(i1-2-i10,i2-i20,i3-2-i30))+30*(rsxy(i1+
     & 2,i2,i3,axis,2)*jac3di(i1+2-i10,i2-i20,i3-i30))-30*(rsxy(i1-2,
     & i2,i3,axis,2)*jac3di(i1-2-i10,i2-i20,i3-i30))-240*(rsxy(i1+1,
     & i2,i3,axis,2)*jac3di(i1+1-i10,i2-i20,i3-i30))+240*(rsxy(i1-1,
     & i2,i3,axis,2)*jac3di(i1-1-i10,i2-i20,i3-i30)))/(144.*dr(2)**2*
     & dr(0))
                     a21rtt = (128*(rsxy(i1+1,i2,i3+1,axisp1,0)*jac3di(
     & i1+1-i10,i2-i20,i3+1-i30))-16*(rsxy(i1+2,i2,i3-1,axisp1,0)*
     & jac3di(i1+2-i10,i2-i20,i3-1-i30))+16*(rsxy(i1-2,i2,i3-1,axisp1,
     & 0)*jac3di(i1-2-i10,i2-i20,i3-1-i30))-128*(rsxy(i1-1,i2,i3-1,
     & axisp1,0)*jac3di(i1-1-i10,i2-i20,i3-1-i30))-16*(rsxy(i1+2,i2,
     & i3+1,axisp1,0)*jac3di(i1+2-i10,i2-i20,i3+1-i30))+16*(rsxy(i1-2,
     & i2,i3+1,axisp1,0)*jac3di(i1-2-i10,i2-i20,i3+1-i30))+128*(rsxy(
     & i1+1,i2,i3-1,axisp1,0)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-128*(
     & rsxy(i1-1,i2,i3+1,axisp1,0)*jac3di(i1-1-i10,i2-i20,i3+1-i30))-
     & 8*(rsxy(i1+1,i2,i3+2,axisp1,0)*jac3di(i1+1-i10,i2-i20,i3+2-i30)
     & )+8*(rsxy(i1-1,i2,i3+2,axisp1,0)*jac3di(i1-1-i10,i2-i20,i3+2-
     & i30))+(rsxy(i1+2,i2,i3+2,axisp1,0)*jac3di(i1+2-i10,i2-i20,i3+2-
     & i30))-(rsxy(i1-2,i2,i3+2,axisp1,0)*jac3di(i1-2-i10,i2-i20,i3+2-
     & i30))-8*(rsxy(i1+1,i2,i3-2,axisp1,0)*jac3di(i1+1-i10,i2-i20,i3-
     & 2-i30))+8*(rsxy(i1-1,i2,i3-2,axisp1,0)*jac3di(i1-1-i10,i2-i20,
     & i3-2-i30))+(rsxy(i1+2,i2,i3-2,axisp1,0)*jac3di(i1+2-i10,i2-i20,
     & i3-2-i30))-(rsxy(i1-2,i2,i3-2,axisp1,0)*jac3di(i1-2-i10,i2-i20,
     & i3-2-i30))+30*(rsxy(i1+2,i2,i3,axisp1,0)*jac3di(i1+2-i10,i2-
     & i20,i3-i30))-30*(rsxy(i1-2,i2,i3,axisp1,0)*jac3di(i1-2-i10,i2-
     & i20,i3-i30))-240*(rsxy(i1+1,i2,i3,axisp1,0)*jac3di(i1+1-i10,i2-
     & i20,i3-i30))+240*(rsxy(i1-1,i2,i3,axisp1,0)*jac3di(i1-1-i10,i2-
     & i20,i3-i30)))/(144.*dr(2)**2*dr(0))
                     a22rtt = (128*(rsxy(i1+1,i2,i3+1,axisp1,1)*jac3di(
     & i1+1-i10,i2-i20,i3+1-i30))-16*(rsxy(i1+2,i2,i3-1,axisp1,1)*
     & jac3di(i1+2-i10,i2-i20,i3-1-i30))+16*(rsxy(i1-2,i2,i3-1,axisp1,
     & 1)*jac3di(i1-2-i10,i2-i20,i3-1-i30))-128*(rsxy(i1-1,i2,i3-1,
     & axisp1,1)*jac3di(i1-1-i10,i2-i20,i3-1-i30))-16*(rsxy(i1+2,i2,
     & i3+1,axisp1,1)*jac3di(i1+2-i10,i2-i20,i3+1-i30))+16*(rsxy(i1-2,
     & i2,i3+1,axisp1,1)*jac3di(i1-2-i10,i2-i20,i3+1-i30))+128*(rsxy(
     & i1+1,i2,i3-1,axisp1,1)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-128*(
     & rsxy(i1-1,i2,i3+1,axisp1,1)*jac3di(i1-1-i10,i2-i20,i3+1-i30))-
     & 8*(rsxy(i1+1,i2,i3+2,axisp1,1)*jac3di(i1+1-i10,i2-i20,i3+2-i30)
     & )+8*(rsxy(i1-1,i2,i3+2,axisp1,1)*jac3di(i1-1-i10,i2-i20,i3+2-
     & i30))+(rsxy(i1+2,i2,i3+2,axisp1,1)*jac3di(i1+2-i10,i2-i20,i3+2-
     & i30))-(rsxy(i1-2,i2,i3+2,axisp1,1)*jac3di(i1-2-i10,i2-i20,i3+2-
     & i30))-8*(rsxy(i1+1,i2,i3-2,axisp1,1)*jac3di(i1+1-i10,i2-i20,i3-
     & 2-i30))+8*(rsxy(i1-1,i2,i3-2,axisp1,1)*jac3di(i1-1-i10,i2-i20,
     & i3-2-i30))+(rsxy(i1+2,i2,i3-2,axisp1,1)*jac3di(i1+2-i10,i2-i20,
     & i3-2-i30))-(rsxy(i1-2,i2,i3-2,axisp1,1)*jac3di(i1-2-i10,i2-i20,
     & i3-2-i30))+30*(rsxy(i1+2,i2,i3,axisp1,1)*jac3di(i1+2-i10,i2-
     & i20,i3-i30))-30*(rsxy(i1-2,i2,i3,axisp1,1)*jac3di(i1-2-i10,i2-
     & i20,i3-i30))-240*(rsxy(i1+1,i2,i3,axisp1,1)*jac3di(i1+1-i10,i2-
     & i20,i3-i30))+240*(rsxy(i1-1,i2,i3,axisp1,1)*jac3di(i1-1-i10,i2-
     & i20,i3-i30)))/(144.*dr(2)**2*dr(0))
                     a23rtt = (128*(rsxy(i1+1,i2,i3+1,axisp1,2)*jac3di(
     & i1+1-i10,i2-i20,i3+1-i30))-16*(rsxy(i1+2,i2,i3-1,axisp1,2)*
     & jac3di(i1+2-i10,i2-i20,i3-1-i30))+16*(rsxy(i1-2,i2,i3-1,axisp1,
     & 2)*jac3di(i1-2-i10,i2-i20,i3-1-i30))-128*(rsxy(i1-1,i2,i3-1,
     & axisp1,2)*jac3di(i1-1-i10,i2-i20,i3-1-i30))-16*(rsxy(i1+2,i2,
     & i3+1,axisp1,2)*jac3di(i1+2-i10,i2-i20,i3+1-i30))+16*(rsxy(i1-2,
     & i2,i3+1,axisp1,2)*jac3di(i1-2-i10,i2-i20,i3+1-i30))+128*(rsxy(
     & i1+1,i2,i3-1,axisp1,2)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-128*(
     & rsxy(i1-1,i2,i3+1,axisp1,2)*jac3di(i1-1-i10,i2-i20,i3+1-i30))-
     & 8*(rsxy(i1+1,i2,i3+2,axisp1,2)*jac3di(i1+1-i10,i2-i20,i3+2-i30)
     & )+8*(rsxy(i1-1,i2,i3+2,axisp1,2)*jac3di(i1-1-i10,i2-i20,i3+2-
     & i30))+(rsxy(i1+2,i2,i3+2,axisp1,2)*jac3di(i1+2-i10,i2-i20,i3+2-
     & i30))-(rsxy(i1-2,i2,i3+2,axisp1,2)*jac3di(i1-2-i10,i2-i20,i3+2-
     & i30))-8*(rsxy(i1+1,i2,i3-2,axisp1,2)*jac3di(i1+1-i10,i2-i20,i3-
     & 2-i30))+8*(rsxy(i1-1,i2,i3-2,axisp1,2)*jac3di(i1-1-i10,i2-i20,
     & i3-2-i30))+(rsxy(i1+2,i2,i3-2,axisp1,2)*jac3di(i1+2-i10,i2-i20,
     & i3-2-i30))-(rsxy(i1-2,i2,i3-2,axisp1,2)*jac3di(i1-2-i10,i2-i20,
     & i3-2-i30))+30*(rsxy(i1+2,i2,i3,axisp1,2)*jac3di(i1+2-i10,i2-
     & i20,i3-i30))-30*(rsxy(i1-2,i2,i3,axisp1,2)*jac3di(i1-2-i10,i2-
     & i20,i3-i30))-240*(rsxy(i1+1,i2,i3,axisp1,2)*jac3di(i1+1-i10,i2-
     & i20,i3-i30))+240*(rsxy(i1-1,i2,i3,axisp1,2)*jac3di(i1-1-i10,i2-
     & i20,i3-i30)))/(144.*dr(2)**2*dr(0))
                     a31rtt = (128*(rsxy(i1+1,i2,i3+1,axisp2,0)*jac3di(
     & i1+1-i10,i2-i20,i3+1-i30))-16*(rsxy(i1+2,i2,i3-1,axisp2,0)*
     & jac3di(i1+2-i10,i2-i20,i3-1-i30))+16*(rsxy(i1-2,i2,i3-1,axisp2,
     & 0)*jac3di(i1-2-i10,i2-i20,i3-1-i30))-128*(rsxy(i1-1,i2,i3-1,
     & axisp2,0)*jac3di(i1-1-i10,i2-i20,i3-1-i30))-16*(rsxy(i1+2,i2,
     & i3+1,axisp2,0)*jac3di(i1+2-i10,i2-i20,i3+1-i30))+16*(rsxy(i1-2,
     & i2,i3+1,axisp2,0)*jac3di(i1-2-i10,i2-i20,i3+1-i30))+128*(rsxy(
     & i1+1,i2,i3-1,axisp2,0)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-128*(
     & rsxy(i1-1,i2,i3+1,axisp2,0)*jac3di(i1-1-i10,i2-i20,i3+1-i30))-
     & 8*(rsxy(i1+1,i2,i3+2,axisp2,0)*jac3di(i1+1-i10,i2-i20,i3+2-i30)
     & )+8*(rsxy(i1-1,i2,i3+2,axisp2,0)*jac3di(i1-1-i10,i2-i20,i3+2-
     & i30))+(rsxy(i1+2,i2,i3+2,axisp2,0)*jac3di(i1+2-i10,i2-i20,i3+2-
     & i30))-(rsxy(i1-2,i2,i3+2,axisp2,0)*jac3di(i1-2-i10,i2-i20,i3+2-
     & i30))-8*(rsxy(i1+1,i2,i3-2,axisp2,0)*jac3di(i1+1-i10,i2-i20,i3-
     & 2-i30))+8*(rsxy(i1-1,i2,i3-2,axisp2,0)*jac3di(i1-1-i10,i2-i20,
     & i3-2-i30))+(rsxy(i1+2,i2,i3-2,axisp2,0)*jac3di(i1+2-i10,i2-i20,
     & i3-2-i30))-(rsxy(i1-2,i2,i3-2,axisp2,0)*jac3di(i1-2-i10,i2-i20,
     & i3-2-i30))+30*(rsxy(i1+2,i2,i3,axisp2,0)*jac3di(i1+2-i10,i2-
     & i20,i3-i30))-30*(rsxy(i1-2,i2,i3,axisp2,0)*jac3di(i1-2-i10,i2-
     & i20,i3-i30))-240*(rsxy(i1+1,i2,i3,axisp2,0)*jac3di(i1+1-i10,i2-
     & i20,i3-i30))+240*(rsxy(i1-1,i2,i3,axisp2,0)*jac3di(i1-1-i10,i2-
     & i20,i3-i30)))/(144.*dr(2)**2*dr(0))
                     a32rtt = (128*(rsxy(i1+1,i2,i3+1,axisp2,1)*jac3di(
     & i1+1-i10,i2-i20,i3+1-i30))-16*(rsxy(i1+2,i2,i3-1,axisp2,1)*
     & jac3di(i1+2-i10,i2-i20,i3-1-i30))+16*(rsxy(i1-2,i2,i3-1,axisp2,
     & 1)*jac3di(i1-2-i10,i2-i20,i3-1-i30))-128*(rsxy(i1-1,i2,i3-1,
     & axisp2,1)*jac3di(i1-1-i10,i2-i20,i3-1-i30))-16*(rsxy(i1+2,i2,
     & i3+1,axisp2,1)*jac3di(i1+2-i10,i2-i20,i3+1-i30))+16*(rsxy(i1-2,
     & i2,i3+1,axisp2,1)*jac3di(i1-2-i10,i2-i20,i3+1-i30))+128*(rsxy(
     & i1+1,i2,i3-1,axisp2,1)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-128*(
     & rsxy(i1-1,i2,i3+1,axisp2,1)*jac3di(i1-1-i10,i2-i20,i3+1-i30))-
     & 8*(rsxy(i1+1,i2,i3+2,axisp2,1)*jac3di(i1+1-i10,i2-i20,i3+2-i30)
     & )+8*(rsxy(i1-1,i2,i3+2,axisp2,1)*jac3di(i1-1-i10,i2-i20,i3+2-
     & i30))+(rsxy(i1+2,i2,i3+2,axisp2,1)*jac3di(i1+2-i10,i2-i20,i3+2-
     & i30))-(rsxy(i1-2,i2,i3+2,axisp2,1)*jac3di(i1-2-i10,i2-i20,i3+2-
     & i30))-8*(rsxy(i1+1,i2,i3-2,axisp2,1)*jac3di(i1+1-i10,i2-i20,i3-
     & 2-i30))+8*(rsxy(i1-1,i2,i3-2,axisp2,1)*jac3di(i1-1-i10,i2-i20,
     & i3-2-i30))+(rsxy(i1+2,i2,i3-2,axisp2,1)*jac3di(i1+2-i10,i2-i20,
     & i3-2-i30))-(rsxy(i1-2,i2,i3-2,axisp2,1)*jac3di(i1-2-i10,i2-i20,
     & i3-2-i30))+30*(rsxy(i1+2,i2,i3,axisp2,1)*jac3di(i1+2-i10,i2-
     & i20,i3-i30))-30*(rsxy(i1-2,i2,i3,axisp2,1)*jac3di(i1-2-i10,i2-
     & i20,i3-i30))-240*(rsxy(i1+1,i2,i3,axisp2,1)*jac3di(i1+1-i10,i2-
     & i20,i3-i30))+240*(rsxy(i1-1,i2,i3,axisp2,1)*jac3di(i1-1-i10,i2-
     & i20,i3-i30)))/(144.*dr(2)**2*dr(0))
                     a33rtt = (128*(rsxy(i1+1,i2,i3+1,axisp2,2)*jac3di(
     & i1+1-i10,i2-i20,i3+1-i30))-16*(rsxy(i1+2,i2,i3-1,axisp2,2)*
     & jac3di(i1+2-i10,i2-i20,i3-1-i30))+16*(rsxy(i1-2,i2,i3-1,axisp2,
     & 2)*jac3di(i1-2-i10,i2-i20,i3-1-i30))-128*(rsxy(i1-1,i2,i3-1,
     & axisp2,2)*jac3di(i1-1-i10,i2-i20,i3-1-i30))-16*(rsxy(i1+2,i2,
     & i3+1,axisp2,2)*jac3di(i1+2-i10,i2-i20,i3+1-i30))+16*(rsxy(i1-2,
     & i2,i3+1,axisp2,2)*jac3di(i1-2-i10,i2-i20,i3+1-i30))+128*(rsxy(
     & i1+1,i2,i3-1,axisp2,2)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-128*(
     & rsxy(i1-1,i2,i3+1,axisp2,2)*jac3di(i1-1-i10,i2-i20,i3+1-i30))-
     & 8*(rsxy(i1+1,i2,i3+2,axisp2,2)*jac3di(i1+1-i10,i2-i20,i3+2-i30)
     & )+8*(rsxy(i1-1,i2,i3+2,axisp2,2)*jac3di(i1-1-i10,i2-i20,i3+2-
     & i30))+(rsxy(i1+2,i2,i3+2,axisp2,2)*jac3di(i1+2-i10,i2-i20,i3+2-
     & i30))-(rsxy(i1-2,i2,i3+2,axisp2,2)*jac3di(i1-2-i10,i2-i20,i3+2-
     & i30))-8*(rsxy(i1+1,i2,i3-2,axisp2,2)*jac3di(i1+1-i10,i2-i20,i3-
     & 2-i30))+8*(rsxy(i1-1,i2,i3-2,axisp2,2)*jac3di(i1-1-i10,i2-i20,
     & i3-2-i30))+(rsxy(i1+2,i2,i3-2,axisp2,2)*jac3di(i1+2-i10,i2-i20,
     & i3-2-i30))-(rsxy(i1-2,i2,i3-2,axisp2,2)*jac3di(i1-2-i10,i2-i20,
     & i3-2-i30))+30*(rsxy(i1+2,i2,i3,axisp2,2)*jac3di(i1+2-i10,i2-
     & i20,i3-i30))-30*(rsxy(i1-2,i2,i3,axisp2,2)*jac3di(i1-2-i10,i2-
     & i20,i3-i30))-240*(rsxy(i1+1,i2,i3,axisp2,2)*jac3di(i1+1-i10,i2-
     & i20,i3-i30))+240*(rsxy(i1-1,i2,i3,axisp2,2)*jac3di(i1-1-i10,i2-
     & i20,i3-i30)))/(144.*dr(2)**2*dr(0))
                     a11stt = (-240*(rsxy(i1,i2+1,i3,axis,0)*jac3di(i1-
     & i10,i2+1-i20,i3-i30))+240*(rsxy(i1,i2-1,i3,axis,0)*jac3di(i1-
     & i10,i2-1-i20,i3-i30))-8*(rsxy(i1,i2+1,i3+2,axis,0)*jac3di(i1-
     & i10,i2+1-i20,i3+2-i30))+8*(rsxy(i1,i2-1,i3+2,axis,0)*jac3di(i1-
     & i10,i2-1-i20,i3+2-i30))+(rsxy(i1,i2+2,i3+2,axis,0)*jac3di(i1-
     & i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axis,0)*jac3di(i1-
     & i10,i2-2-i20,i3+2-i30))-8*(rsxy(i1,i2+1,i3-2,axis,0)*jac3di(i1-
     & i10,i2+1-i20,i3-2-i30))+8*(rsxy(i1,i2-1,i3-2,axis,0)*jac3di(i1-
     & i10,i2-1-i20,i3-2-i30))+(rsxy(i1,i2+2,i3-2,axis,0)*jac3di(i1-
     & i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,i3-2,axis,0)*jac3di(i1-
     & i10,i2-2-i20,i3-2-i30))+30*(rsxy(i1,i2+2,i3,axis,0)*jac3di(i1-
     & i10,i2+2-i20,i3-i30))-30*(rsxy(i1,i2-2,i3,axis,0)*jac3di(i1-
     & i10,i2-2-i20,i3-i30))+128*(rsxy(i1,i2+1,i3+1,axis,0)*jac3di(i1-
     & i10,i2+1-i20,i3+1-i30))-128*(rsxy(i1,i2-1,i3+1,axis,0)*jac3di(
     & i1-i10,i2-1-i20,i3+1-i30))-16*(rsxy(i1,i2+2,i3+1,axis,0)*
     & jac3di(i1-i10,i2+2-i20,i3+1-i30))+16*(rsxy(i1,i2-2,i3+1,axis,0)
     & *jac3di(i1-i10,i2-2-i20,i3+1-i30))+128*(rsxy(i1,i2+1,i3-1,axis,
     & 0)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-128*(rsxy(i1,i2-1,i3-1,
     & axis,0)*jac3di(i1-i10,i2-1-i20,i3-1-i30))-16*(rsxy(i1,i2+2,i3-
     & 1,axis,0)*jac3di(i1-i10,i2+2-i20,i3-1-i30))+16*(rsxy(i1,i2-2,
     & i3-1,axis,0)*jac3di(i1-i10,i2-2-i20,i3-1-i30)))/(144.*dr(2)**2*
     & dr(1))
                     a12stt = (-240*(rsxy(i1,i2+1,i3,axis,1)*jac3di(i1-
     & i10,i2+1-i20,i3-i30))+240*(rsxy(i1,i2-1,i3,axis,1)*jac3di(i1-
     & i10,i2-1-i20,i3-i30))-8*(rsxy(i1,i2+1,i3+2,axis,1)*jac3di(i1-
     & i10,i2+1-i20,i3+2-i30))+8*(rsxy(i1,i2-1,i3+2,axis,1)*jac3di(i1-
     & i10,i2-1-i20,i3+2-i30))+(rsxy(i1,i2+2,i3+2,axis,1)*jac3di(i1-
     & i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axis,1)*jac3di(i1-
     & i10,i2-2-i20,i3+2-i30))-8*(rsxy(i1,i2+1,i3-2,axis,1)*jac3di(i1-
     & i10,i2+1-i20,i3-2-i30))+8*(rsxy(i1,i2-1,i3-2,axis,1)*jac3di(i1-
     & i10,i2-1-i20,i3-2-i30))+(rsxy(i1,i2+2,i3-2,axis,1)*jac3di(i1-
     & i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,i3-2,axis,1)*jac3di(i1-
     & i10,i2-2-i20,i3-2-i30))+30*(rsxy(i1,i2+2,i3,axis,1)*jac3di(i1-
     & i10,i2+2-i20,i3-i30))-30*(rsxy(i1,i2-2,i3,axis,1)*jac3di(i1-
     & i10,i2-2-i20,i3-i30))+128*(rsxy(i1,i2+1,i3+1,axis,1)*jac3di(i1-
     & i10,i2+1-i20,i3+1-i30))-128*(rsxy(i1,i2-1,i3+1,axis,1)*jac3di(
     & i1-i10,i2-1-i20,i3+1-i30))-16*(rsxy(i1,i2+2,i3+1,axis,1)*
     & jac3di(i1-i10,i2+2-i20,i3+1-i30))+16*(rsxy(i1,i2-2,i3+1,axis,1)
     & *jac3di(i1-i10,i2-2-i20,i3+1-i30))+128*(rsxy(i1,i2+1,i3-1,axis,
     & 1)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-128*(rsxy(i1,i2-1,i3-1,
     & axis,1)*jac3di(i1-i10,i2-1-i20,i3-1-i30))-16*(rsxy(i1,i2+2,i3-
     & 1,axis,1)*jac3di(i1-i10,i2+2-i20,i3-1-i30))+16*(rsxy(i1,i2-2,
     & i3-1,axis,1)*jac3di(i1-i10,i2-2-i20,i3-1-i30)))/(144.*dr(2)**2*
     & dr(1))
                     a13stt = (-240*(rsxy(i1,i2+1,i3,axis,2)*jac3di(i1-
     & i10,i2+1-i20,i3-i30))+240*(rsxy(i1,i2-1,i3,axis,2)*jac3di(i1-
     & i10,i2-1-i20,i3-i30))-8*(rsxy(i1,i2+1,i3+2,axis,2)*jac3di(i1-
     & i10,i2+1-i20,i3+2-i30))+8*(rsxy(i1,i2-1,i3+2,axis,2)*jac3di(i1-
     & i10,i2-1-i20,i3+2-i30))+(rsxy(i1,i2+2,i3+2,axis,2)*jac3di(i1-
     & i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axis,2)*jac3di(i1-
     & i10,i2-2-i20,i3+2-i30))-8*(rsxy(i1,i2+1,i3-2,axis,2)*jac3di(i1-
     & i10,i2+1-i20,i3-2-i30))+8*(rsxy(i1,i2-1,i3-2,axis,2)*jac3di(i1-
     & i10,i2-1-i20,i3-2-i30))+(rsxy(i1,i2+2,i3-2,axis,2)*jac3di(i1-
     & i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,i3-2,axis,2)*jac3di(i1-
     & i10,i2-2-i20,i3-2-i30))+30*(rsxy(i1,i2+2,i3,axis,2)*jac3di(i1-
     & i10,i2+2-i20,i3-i30))-30*(rsxy(i1,i2-2,i3,axis,2)*jac3di(i1-
     & i10,i2-2-i20,i3-i30))+128*(rsxy(i1,i2+1,i3+1,axis,2)*jac3di(i1-
     & i10,i2+1-i20,i3+1-i30))-128*(rsxy(i1,i2-1,i3+1,axis,2)*jac3di(
     & i1-i10,i2-1-i20,i3+1-i30))-16*(rsxy(i1,i2+2,i3+1,axis,2)*
     & jac3di(i1-i10,i2+2-i20,i3+1-i30))+16*(rsxy(i1,i2-2,i3+1,axis,2)
     & *jac3di(i1-i10,i2-2-i20,i3+1-i30))+128*(rsxy(i1,i2+1,i3-1,axis,
     & 2)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-128*(rsxy(i1,i2-1,i3-1,
     & axis,2)*jac3di(i1-i10,i2-1-i20,i3-1-i30))-16*(rsxy(i1,i2+2,i3-
     & 1,axis,2)*jac3di(i1-i10,i2+2-i20,i3-1-i30))+16*(rsxy(i1,i2-2,
     & i3-1,axis,2)*jac3di(i1-i10,i2-2-i20,i3-1-i30)))/(144.*dr(2)**2*
     & dr(1))
                     a21stt = (-240*(rsxy(i1,i2+1,i3,axisp1,0)*jac3di(
     & i1-i10,i2+1-i20,i3-i30))+240*(rsxy(i1,i2-1,i3,axisp1,0)*jac3di(
     & i1-i10,i2-1-i20,i3-i30))-8*(rsxy(i1,i2+1,i3+2,axisp1,0)*jac3di(
     & i1-i10,i2+1-i20,i3+2-i30))+8*(rsxy(i1,i2-1,i3+2,axisp1,0)*
     & jac3di(i1-i10,i2-1-i20,i3+2-i30))+(rsxy(i1,i2+2,i3+2,axisp1,0)*
     & jac3di(i1-i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp1,0)*
     & jac3di(i1-i10,i2-2-i20,i3+2-i30))-8*(rsxy(i1,i2+1,i3-2,axisp1,
     & 0)*jac3di(i1-i10,i2+1-i20,i3-2-i30))+8*(rsxy(i1,i2-1,i3-2,
     & axisp1,0)*jac3di(i1-i10,i2-1-i20,i3-2-i30))+(rsxy(i1,i2+2,i3-2,
     & axisp1,0)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,i3-2,
     & axisp1,0)*jac3di(i1-i10,i2-2-i20,i3-2-i30))+30*(rsxy(i1,i2+2,
     & i3,axisp1,0)*jac3di(i1-i10,i2+2-i20,i3-i30))-30*(rsxy(i1,i2-2,
     & i3,axisp1,0)*jac3di(i1-i10,i2-2-i20,i3-i30))+128*(rsxy(i1,i2+1,
     & i3+1,axisp1,0)*jac3di(i1-i10,i2+1-i20,i3+1-i30))-128*(rsxy(i1,
     & i2-1,i3+1,axisp1,0)*jac3di(i1-i10,i2-1-i20,i3+1-i30))-16*(rsxy(
     & i1,i2+2,i3+1,axisp1,0)*jac3di(i1-i10,i2+2-i20,i3+1-i30))+16*(
     & rsxy(i1,i2-2,i3+1,axisp1,0)*jac3di(i1-i10,i2-2-i20,i3+1-i30))+
     & 128*(rsxy(i1,i2+1,i3-1,axisp1,0)*jac3di(i1-i10,i2+1-i20,i3-1-
     & i30))-128*(rsxy(i1,i2-1,i3-1,axisp1,0)*jac3di(i1-i10,i2-1-i20,
     & i3-1-i30))-16*(rsxy(i1,i2+2,i3-1,axisp1,0)*jac3di(i1-i10,i2+2-
     & i20,i3-1-i30))+16*(rsxy(i1,i2-2,i3-1,axisp1,0)*jac3di(i1-i10,
     & i2-2-i20,i3-1-i30)))/(144.*dr(2)**2*dr(1))
                     a22stt = (-240*(rsxy(i1,i2+1,i3,axisp1,1)*jac3di(
     & i1-i10,i2+1-i20,i3-i30))+240*(rsxy(i1,i2-1,i3,axisp1,1)*jac3di(
     & i1-i10,i2-1-i20,i3-i30))-8*(rsxy(i1,i2+1,i3+2,axisp1,1)*jac3di(
     & i1-i10,i2+1-i20,i3+2-i30))+8*(rsxy(i1,i2-1,i3+2,axisp1,1)*
     & jac3di(i1-i10,i2-1-i20,i3+2-i30))+(rsxy(i1,i2+2,i3+2,axisp1,1)*
     & jac3di(i1-i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp1,1)*
     & jac3di(i1-i10,i2-2-i20,i3+2-i30))-8*(rsxy(i1,i2+1,i3-2,axisp1,
     & 1)*jac3di(i1-i10,i2+1-i20,i3-2-i30))+8*(rsxy(i1,i2-1,i3-2,
     & axisp1,1)*jac3di(i1-i10,i2-1-i20,i3-2-i30))+(rsxy(i1,i2+2,i3-2,
     & axisp1,1)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,i3-2,
     & axisp1,1)*jac3di(i1-i10,i2-2-i20,i3-2-i30))+30*(rsxy(i1,i2+2,
     & i3,axisp1,1)*jac3di(i1-i10,i2+2-i20,i3-i30))-30*(rsxy(i1,i2-2,
     & i3,axisp1,1)*jac3di(i1-i10,i2-2-i20,i3-i30))+128*(rsxy(i1,i2+1,
     & i3+1,axisp1,1)*jac3di(i1-i10,i2+1-i20,i3+1-i30))-128*(rsxy(i1,
     & i2-1,i3+1,axisp1,1)*jac3di(i1-i10,i2-1-i20,i3+1-i30))-16*(rsxy(
     & i1,i2+2,i3+1,axisp1,1)*jac3di(i1-i10,i2+2-i20,i3+1-i30))+16*(
     & rsxy(i1,i2-2,i3+1,axisp1,1)*jac3di(i1-i10,i2-2-i20,i3+1-i30))+
     & 128*(rsxy(i1,i2+1,i3-1,axisp1,1)*jac3di(i1-i10,i2+1-i20,i3-1-
     & i30))-128*(rsxy(i1,i2-1,i3-1,axisp1,1)*jac3di(i1-i10,i2-1-i20,
     & i3-1-i30))-16*(rsxy(i1,i2+2,i3-1,axisp1,1)*jac3di(i1-i10,i2+2-
     & i20,i3-1-i30))+16*(rsxy(i1,i2-2,i3-1,axisp1,1)*jac3di(i1-i10,
     & i2-2-i20,i3-1-i30)))/(144.*dr(2)**2*dr(1))
                     a23stt = (-240*(rsxy(i1,i2+1,i3,axisp1,2)*jac3di(
     & i1-i10,i2+1-i20,i3-i30))+240*(rsxy(i1,i2-1,i3,axisp1,2)*jac3di(
     & i1-i10,i2-1-i20,i3-i30))-8*(rsxy(i1,i2+1,i3+2,axisp1,2)*jac3di(
     & i1-i10,i2+1-i20,i3+2-i30))+8*(rsxy(i1,i2-1,i3+2,axisp1,2)*
     & jac3di(i1-i10,i2-1-i20,i3+2-i30))+(rsxy(i1,i2+2,i3+2,axisp1,2)*
     & jac3di(i1-i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp1,2)*
     & jac3di(i1-i10,i2-2-i20,i3+2-i30))-8*(rsxy(i1,i2+1,i3-2,axisp1,
     & 2)*jac3di(i1-i10,i2+1-i20,i3-2-i30))+8*(rsxy(i1,i2-1,i3-2,
     & axisp1,2)*jac3di(i1-i10,i2-1-i20,i3-2-i30))+(rsxy(i1,i2+2,i3-2,
     & axisp1,2)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,i3-2,
     & axisp1,2)*jac3di(i1-i10,i2-2-i20,i3-2-i30))+30*(rsxy(i1,i2+2,
     & i3,axisp1,2)*jac3di(i1-i10,i2+2-i20,i3-i30))-30*(rsxy(i1,i2-2,
     & i3,axisp1,2)*jac3di(i1-i10,i2-2-i20,i3-i30))+128*(rsxy(i1,i2+1,
     & i3+1,axisp1,2)*jac3di(i1-i10,i2+1-i20,i3+1-i30))-128*(rsxy(i1,
     & i2-1,i3+1,axisp1,2)*jac3di(i1-i10,i2-1-i20,i3+1-i30))-16*(rsxy(
     & i1,i2+2,i3+1,axisp1,2)*jac3di(i1-i10,i2+2-i20,i3+1-i30))+16*(
     & rsxy(i1,i2-2,i3+1,axisp1,2)*jac3di(i1-i10,i2-2-i20,i3+1-i30))+
     & 128*(rsxy(i1,i2+1,i3-1,axisp1,2)*jac3di(i1-i10,i2+1-i20,i3-1-
     & i30))-128*(rsxy(i1,i2-1,i3-1,axisp1,2)*jac3di(i1-i10,i2-1-i20,
     & i3-1-i30))-16*(rsxy(i1,i2+2,i3-1,axisp1,2)*jac3di(i1-i10,i2+2-
     & i20,i3-1-i30))+16*(rsxy(i1,i2-2,i3-1,axisp1,2)*jac3di(i1-i10,
     & i2-2-i20,i3-1-i30)))/(144.*dr(2)**2*dr(1))
                     a31stt = (-240*(rsxy(i1,i2+1,i3,axisp2,0)*jac3di(
     & i1-i10,i2+1-i20,i3-i30))+240*(rsxy(i1,i2-1,i3,axisp2,0)*jac3di(
     & i1-i10,i2-1-i20,i3-i30))-8*(rsxy(i1,i2+1,i3+2,axisp2,0)*jac3di(
     & i1-i10,i2+1-i20,i3+2-i30))+8*(rsxy(i1,i2-1,i3+2,axisp2,0)*
     & jac3di(i1-i10,i2-1-i20,i3+2-i30))+(rsxy(i1,i2+2,i3+2,axisp2,0)*
     & jac3di(i1-i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp2,0)*
     & jac3di(i1-i10,i2-2-i20,i3+2-i30))-8*(rsxy(i1,i2+1,i3-2,axisp2,
     & 0)*jac3di(i1-i10,i2+1-i20,i3-2-i30))+8*(rsxy(i1,i2-1,i3-2,
     & axisp2,0)*jac3di(i1-i10,i2-1-i20,i3-2-i30))+(rsxy(i1,i2+2,i3-2,
     & axisp2,0)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,i3-2,
     & axisp2,0)*jac3di(i1-i10,i2-2-i20,i3-2-i30))+30*(rsxy(i1,i2+2,
     & i3,axisp2,0)*jac3di(i1-i10,i2+2-i20,i3-i30))-30*(rsxy(i1,i2-2,
     & i3,axisp2,0)*jac3di(i1-i10,i2-2-i20,i3-i30))+128*(rsxy(i1,i2+1,
     & i3+1,axisp2,0)*jac3di(i1-i10,i2+1-i20,i3+1-i30))-128*(rsxy(i1,
     & i2-1,i3+1,axisp2,0)*jac3di(i1-i10,i2-1-i20,i3+1-i30))-16*(rsxy(
     & i1,i2+2,i3+1,axisp2,0)*jac3di(i1-i10,i2+2-i20,i3+1-i30))+16*(
     & rsxy(i1,i2-2,i3+1,axisp2,0)*jac3di(i1-i10,i2-2-i20,i3+1-i30))+
     & 128*(rsxy(i1,i2+1,i3-1,axisp2,0)*jac3di(i1-i10,i2+1-i20,i3-1-
     & i30))-128*(rsxy(i1,i2-1,i3-1,axisp2,0)*jac3di(i1-i10,i2-1-i20,
     & i3-1-i30))-16*(rsxy(i1,i2+2,i3-1,axisp2,0)*jac3di(i1-i10,i2+2-
     & i20,i3-1-i30))+16*(rsxy(i1,i2-2,i3-1,axisp2,0)*jac3di(i1-i10,
     & i2-2-i20,i3-1-i30)))/(144.*dr(2)**2*dr(1))
                     a32stt = (-240*(rsxy(i1,i2+1,i3,axisp2,1)*jac3di(
     & i1-i10,i2+1-i20,i3-i30))+240*(rsxy(i1,i2-1,i3,axisp2,1)*jac3di(
     & i1-i10,i2-1-i20,i3-i30))-8*(rsxy(i1,i2+1,i3+2,axisp2,1)*jac3di(
     & i1-i10,i2+1-i20,i3+2-i30))+8*(rsxy(i1,i2-1,i3+2,axisp2,1)*
     & jac3di(i1-i10,i2-1-i20,i3+2-i30))+(rsxy(i1,i2+2,i3+2,axisp2,1)*
     & jac3di(i1-i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp2,1)*
     & jac3di(i1-i10,i2-2-i20,i3+2-i30))-8*(rsxy(i1,i2+1,i3-2,axisp2,
     & 1)*jac3di(i1-i10,i2+1-i20,i3-2-i30))+8*(rsxy(i1,i2-1,i3-2,
     & axisp2,1)*jac3di(i1-i10,i2-1-i20,i3-2-i30))+(rsxy(i1,i2+2,i3-2,
     & axisp2,1)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,i3-2,
     & axisp2,1)*jac3di(i1-i10,i2-2-i20,i3-2-i30))+30*(rsxy(i1,i2+2,
     & i3,axisp2,1)*jac3di(i1-i10,i2+2-i20,i3-i30))-30*(rsxy(i1,i2-2,
     & i3,axisp2,1)*jac3di(i1-i10,i2-2-i20,i3-i30))+128*(rsxy(i1,i2+1,
     & i3+1,axisp2,1)*jac3di(i1-i10,i2+1-i20,i3+1-i30))-128*(rsxy(i1,
     & i2-1,i3+1,axisp2,1)*jac3di(i1-i10,i2-1-i20,i3+1-i30))-16*(rsxy(
     & i1,i2+2,i3+1,axisp2,1)*jac3di(i1-i10,i2+2-i20,i3+1-i30))+16*(
     & rsxy(i1,i2-2,i3+1,axisp2,1)*jac3di(i1-i10,i2-2-i20,i3+1-i30))+
     & 128*(rsxy(i1,i2+1,i3-1,axisp2,1)*jac3di(i1-i10,i2+1-i20,i3-1-
     & i30))-128*(rsxy(i1,i2-1,i3-1,axisp2,1)*jac3di(i1-i10,i2-1-i20,
     & i3-1-i30))-16*(rsxy(i1,i2+2,i3-1,axisp2,1)*jac3di(i1-i10,i2+2-
     & i20,i3-1-i30))+16*(rsxy(i1,i2-2,i3-1,axisp2,1)*jac3di(i1-i10,
     & i2-2-i20,i3-1-i30)))/(144.*dr(2)**2*dr(1))
                     a33stt = (-240*(rsxy(i1,i2+1,i3,axisp2,2)*jac3di(
     & i1-i10,i2+1-i20,i3-i30))+240*(rsxy(i1,i2-1,i3,axisp2,2)*jac3di(
     & i1-i10,i2-1-i20,i3-i30))-8*(rsxy(i1,i2+1,i3+2,axisp2,2)*jac3di(
     & i1-i10,i2+1-i20,i3+2-i30))+8*(rsxy(i1,i2-1,i3+2,axisp2,2)*
     & jac3di(i1-i10,i2-1-i20,i3+2-i30))+(rsxy(i1,i2+2,i3+2,axisp2,2)*
     & jac3di(i1-i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp2,2)*
     & jac3di(i1-i10,i2-2-i20,i3+2-i30))-8*(rsxy(i1,i2+1,i3-2,axisp2,
     & 2)*jac3di(i1-i10,i2+1-i20,i3-2-i30))+8*(rsxy(i1,i2-1,i3-2,
     & axisp2,2)*jac3di(i1-i10,i2-1-i20,i3-2-i30))+(rsxy(i1,i2+2,i3-2,
     & axisp2,2)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,i3-2,
     & axisp2,2)*jac3di(i1-i10,i2-2-i20,i3-2-i30))+30*(rsxy(i1,i2+2,
     & i3,axisp2,2)*jac3di(i1-i10,i2+2-i20,i3-i30))-30*(rsxy(i1,i2-2,
     & i3,axisp2,2)*jac3di(i1-i10,i2-2-i20,i3-i30))+128*(rsxy(i1,i2+1,
     & i3+1,axisp2,2)*jac3di(i1-i10,i2+1-i20,i3+1-i30))-128*(rsxy(i1,
     & i2-1,i3+1,axisp2,2)*jac3di(i1-i10,i2-1-i20,i3+1-i30))-16*(rsxy(
     & i1,i2+2,i3+1,axisp2,2)*jac3di(i1-i10,i2+2-i20,i3+1-i30))+16*(
     & rsxy(i1,i2-2,i3+1,axisp2,2)*jac3di(i1-i10,i2-2-i20,i3+1-i30))+
     & 128*(rsxy(i1,i2+1,i3-1,axisp2,2)*jac3di(i1-i10,i2+1-i20,i3-1-
     & i30))-128*(rsxy(i1,i2-1,i3-1,axisp2,2)*jac3di(i1-i10,i2-1-i20,
     & i3-1-i30))-16*(rsxy(i1,i2+2,i3-1,axisp2,2)*jac3di(i1-i10,i2+2-
     & i20,i3-1-i30))+16*(rsxy(i1,i2-2,i3-1,axisp2,2)*jac3di(i1-i10,
     & i2-2-i20,i3-1-i30)))/(144.*dr(2)**2*dr(1))
                     a11sst = (240*(rsxy(i1,i2,i3-1,axis,0)*jac3di(i1-
     & i10,i2-i20,i3-1-i30))-240*(rsxy(i1,i2,i3+1,axis,0)*jac3di(i1-
     & i10,i2-i20,i3+1-i30))-16*(rsxy(i1,i2+1,i3+2,axis,0)*jac3di(i1-
     & i10,i2+1-i20,i3+2-i30))-16*(rsxy(i1,i2-1,i3+2,axis,0)*jac3di(
     & i1-i10,i2-1-i20,i3+2-i30))+(rsxy(i1,i2+2,i3+2,axis,0)*jac3di(
     & i1-i10,i2+2-i20,i3+2-i30))+(rsxy(i1,i2-2,i3+2,axis,0)*jac3di(
     & i1-i10,i2-2-i20,i3+2-i30))+16*(rsxy(i1,i2+1,i3-2,axis,0)*
     & jac3di(i1-i10,i2+1-i20,i3-2-i30))+16*(rsxy(i1,i2-1,i3-2,axis,0)
     & *jac3di(i1-i10,i2-1-i20,i3-2-i30))-(rsxy(i1,i2+2,i3-2,axis,0)*
     & jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,i3-2,axis,0)*
     & jac3di(i1-i10,i2-2-i20,i3-2-i30))+30*(rsxy(i1,i2,i3+2,axis,0)*
     & jac3di(i1-i10,i2-i20,i3+2-i30))-30*(rsxy(i1,i2,i3-2,axis,0)*
     & jac3di(i1-i10,i2-i20,i3-2-i30))+128*(rsxy(i1,i2+1,i3+1,axis,0)*
     & jac3di(i1-i10,i2+1-i20,i3+1-i30))+128*(rsxy(i1,i2-1,i3+1,axis,
     & 0)*jac3di(i1-i10,i2-1-i20,i3+1-i30))-8*(rsxy(i1,i2+2,i3+1,axis,
     & 0)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-8*(rsxy(i1,i2-2,i3+1,axis,
     & 0)*jac3di(i1-i10,i2-2-i20,i3+1-i30))-128*(rsxy(i1,i2+1,i3-1,
     & axis,0)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-128*(rsxy(i1,i2-1,i3-
     & 1,axis,0)*jac3di(i1-i10,i2-1-i20,i3-1-i30))+8*(rsxy(i1,i2+2,i3-
     & 1,axis,0)*jac3di(i1-i10,i2+2-i20,i3-1-i30))+8*(rsxy(i1,i2-2,i3-
     & 1,axis,0)*jac3di(i1-i10,i2-2-i20,i3-1-i30)))/(144.*dr(1)**2*dr(
     & 2))
                     a12sst = (240*(rsxy(i1,i2,i3-1,axis,1)*jac3di(i1-
     & i10,i2-i20,i3-1-i30))-240*(rsxy(i1,i2,i3+1,axis,1)*jac3di(i1-
     & i10,i2-i20,i3+1-i30))-16*(rsxy(i1,i2+1,i3+2,axis,1)*jac3di(i1-
     & i10,i2+1-i20,i3+2-i30))-16*(rsxy(i1,i2-1,i3+2,axis,1)*jac3di(
     & i1-i10,i2-1-i20,i3+2-i30))+(rsxy(i1,i2+2,i3+2,axis,1)*jac3di(
     & i1-i10,i2+2-i20,i3+2-i30))+(rsxy(i1,i2-2,i3+2,axis,1)*jac3di(
     & i1-i10,i2-2-i20,i3+2-i30))+16*(rsxy(i1,i2+1,i3-2,axis,1)*
     & jac3di(i1-i10,i2+1-i20,i3-2-i30))+16*(rsxy(i1,i2-1,i3-2,axis,1)
     & *jac3di(i1-i10,i2-1-i20,i3-2-i30))-(rsxy(i1,i2+2,i3-2,axis,1)*
     & jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,i3-2,axis,1)*
     & jac3di(i1-i10,i2-2-i20,i3-2-i30))+30*(rsxy(i1,i2,i3+2,axis,1)*
     & jac3di(i1-i10,i2-i20,i3+2-i30))-30*(rsxy(i1,i2,i3-2,axis,1)*
     & jac3di(i1-i10,i2-i20,i3-2-i30))+128*(rsxy(i1,i2+1,i3+1,axis,1)*
     & jac3di(i1-i10,i2+1-i20,i3+1-i30))+128*(rsxy(i1,i2-1,i3+1,axis,
     & 1)*jac3di(i1-i10,i2-1-i20,i3+1-i30))-8*(rsxy(i1,i2+2,i3+1,axis,
     & 1)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-8*(rsxy(i1,i2-2,i3+1,axis,
     & 1)*jac3di(i1-i10,i2-2-i20,i3+1-i30))-128*(rsxy(i1,i2+1,i3-1,
     & axis,1)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-128*(rsxy(i1,i2-1,i3-
     & 1,axis,1)*jac3di(i1-i10,i2-1-i20,i3-1-i30))+8*(rsxy(i1,i2+2,i3-
     & 1,axis,1)*jac3di(i1-i10,i2+2-i20,i3-1-i30))+8*(rsxy(i1,i2-2,i3-
     & 1,axis,1)*jac3di(i1-i10,i2-2-i20,i3-1-i30)))/(144.*dr(1)**2*dr(
     & 2))
                     a13sst = (240*(rsxy(i1,i2,i3-1,axis,2)*jac3di(i1-
     & i10,i2-i20,i3-1-i30))-240*(rsxy(i1,i2,i3+1,axis,2)*jac3di(i1-
     & i10,i2-i20,i3+1-i30))-16*(rsxy(i1,i2+1,i3+2,axis,2)*jac3di(i1-
     & i10,i2+1-i20,i3+2-i30))-16*(rsxy(i1,i2-1,i3+2,axis,2)*jac3di(
     & i1-i10,i2-1-i20,i3+2-i30))+(rsxy(i1,i2+2,i3+2,axis,2)*jac3di(
     & i1-i10,i2+2-i20,i3+2-i30))+(rsxy(i1,i2-2,i3+2,axis,2)*jac3di(
     & i1-i10,i2-2-i20,i3+2-i30))+16*(rsxy(i1,i2+1,i3-2,axis,2)*
     & jac3di(i1-i10,i2+1-i20,i3-2-i30))+16*(rsxy(i1,i2-1,i3-2,axis,2)
     & *jac3di(i1-i10,i2-1-i20,i3-2-i30))-(rsxy(i1,i2+2,i3-2,axis,2)*
     & jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,i3-2,axis,2)*
     & jac3di(i1-i10,i2-2-i20,i3-2-i30))+30*(rsxy(i1,i2,i3+2,axis,2)*
     & jac3di(i1-i10,i2-i20,i3+2-i30))-30*(rsxy(i1,i2,i3-2,axis,2)*
     & jac3di(i1-i10,i2-i20,i3-2-i30))+128*(rsxy(i1,i2+1,i3+1,axis,2)*
     & jac3di(i1-i10,i2+1-i20,i3+1-i30))+128*(rsxy(i1,i2-1,i3+1,axis,
     & 2)*jac3di(i1-i10,i2-1-i20,i3+1-i30))-8*(rsxy(i1,i2+2,i3+1,axis,
     & 2)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-8*(rsxy(i1,i2-2,i3+1,axis,
     & 2)*jac3di(i1-i10,i2-2-i20,i3+1-i30))-128*(rsxy(i1,i2+1,i3-1,
     & axis,2)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-128*(rsxy(i1,i2-1,i3-
     & 1,axis,2)*jac3di(i1-i10,i2-1-i20,i3-1-i30))+8*(rsxy(i1,i2+2,i3-
     & 1,axis,2)*jac3di(i1-i10,i2+2-i20,i3-1-i30))+8*(rsxy(i1,i2-2,i3-
     & 1,axis,2)*jac3di(i1-i10,i2-2-i20,i3-1-i30)))/(144.*dr(1)**2*dr(
     & 2))
                     a21sst = (240*(rsxy(i1,i2,i3-1,axisp1,0)*jac3di(
     & i1-i10,i2-i20,i3-1-i30))-240*(rsxy(i1,i2,i3+1,axisp1,0)*jac3di(
     & i1-i10,i2-i20,i3+1-i30))-16*(rsxy(i1,i2+1,i3+2,axisp1,0)*
     & jac3di(i1-i10,i2+1-i20,i3+2-i30))-16*(rsxy(i1,i2-1,i3+2,axisp1,
     & 0)*jac3di(i1-i10,i2-1-i20,i3+2-i30))+(rsxy(i1,i2+2,i3+2,axisp1,
     & 0)*jac3di(i1-i10,i2+2-i20,i3+2-i30))+(rsxy(i1,i2-2,i3+2,axisp1,
     & 0)*jac3di(i1-i10,i2-2-i20,i3+2-i30))+16*(rsxy(i1,i2+1,i3-2,
     & axisp1,0)*jac3di(i1-i10,i2+1-i20,i3-2-i30))+16*(rsxy(i1,i2-1,
     & i3-2,axisp1,0)*jac3di(i1-i10,i2-1-i20,i3-2-i30))-(rsxy(i1,i2+2,
     & i3-2,axisp1,0)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,
     & i3-2,axisp1,0)*jac3di(i1-i10,i2-2-i20,i3-2-i30))+30*(rsxy(i1,
     & i2,i3+2,axisp1,0)*jac3di(i1-i10,i2-i20,i3+2-i30))-30*(rsxy(i1,
     & i2,i3-2,axisp1,0)*jac3di(i1-i10,i2-i20,i3-2-i30))+128*(rsxy(i1,
     & i2+1,i3+1,axisp1,0)*jac3di(i1-i10,i2+1-i20,i3+1-i30))+128*(
     & rsxy(i1,i2-1,i3+1,axisp1,0)*jac3di(i1-i10,i2-1-i20,i3+1-i30))-
     & 8*(rsxy(i1,i2+2,i3+1,axisp1,0)*jac3di(i1-i10,i2+2-i20,i3+1-i30)
     & )-8*(rsxy(i1,i2-2,i3+1,axisp1,0)*jac3di(i1-i10,i2-2-i20,i3+1-
     & i30))-128*(rsxy(i1,i2+1,i3-1,axisp1,0)*jac3di(i1-i10,i2+1-i20,
     & i3-1-i30))-128*(rsxy(i1,i2-1,i3-1,axisp1,0)*jac3di(i1-i10,i2-1-
     & i20,i3-1-i30))+8*(rsxy(i1,i2+2,i3-1,axisp1,0)*jac3di(i1-i10,i2+
     & 2-i20,i3-1-i30))+8*(rsxy(i1,i2-2,i3-1,axisp1,0)*jac3di(i1-i10,
     & i2-2-i20,i3-1-i30)))/(144.*dr(1)**2*dr(2))
                     a22sst = (240*(rsxy(i1,i2,i3-1,axisp1,1)*jac3di(
     & i1-i10,i2-i20,i3-1-i30))-240*(rsxy(i1,i2,i3+1,axisp1,1)*jac3di(
     & i1-i10,i2-i20,i3+1-i30))-16*(rsxy(i1,i2+1,i3+2,axisp1,1)*
     & jac3di(i1-i10,i2+1-i20,i3+2-i30))-16*(rsxy(i1,i2-1,i3+2,axisp1,
     & 1)*jac3di(i1-i10,i2-1-i20,i3+2-i30))+(rsxy(i1,i2+2,i3+2,axisp1,
     & 1)*jac3di(i1-i10,i2+2-i20,i3+2-i30))+(rsxy(i1,i2-2,i3+2,axisp1,
     & 1)*jac3di(i1-i10,i2-2-i20,i3+2-i30))+16*(rsxy(i1,i2+1,i3-2,
     & axisp1,1)*jac3di(i1-i10,i2+1-i20,i3-2-i30))+16*(rsxy(i1,i2-1,
     & i3-2,axisp1,1)*jac3di(i1-i10,i2-1-i20,i3-2-i30))-(rsxy(i1,i2+2,
     & i3-2,axisp1,1)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,
     & i3-2,axisp1,1)*jac3di(i1-i10,i2-2-i20,i3-2-i30))+30*(rsxy(i1,
     & i2,i3+2,axisp1,1)*jac3di(i1-i10,i2-i20,i3+2-i30))-30*(rsxy(i1,
     & i2,i3-2,axisp1,1)*jac3di(i1-i10,i2-i20,i3-2-i30))+128*(rsxy(i1,
     & i2+1,i3+1,axisp1,1)*jac3di(i1-i10,i2+1-i20,i3+1-i30))+128*(
     & rsxy(i1,i2-1,i3+1,axisp1,1)*jac3di(i1-i10,i2-1-i20,i3+1-i30))-
     & 8*(rsxy(i1,i2+2,i3+1,axisp1,1)*jac3di(i1-i10,i2+2-i20,i3+1-i30)
     & )-8*(rsxy(i1,i2-2,i3+1,axisp1,1)*jac3di(i1-i10,i2-2-i20,i3+1-
     & i30))-128*(rsxy(i1,i2+1,i3-1,axisp1,1)*jac3di(i1-i10,i2+1-i20,
     & i3-1-i30))-128*(rsxy(i1,i2-1,i3-1,axisp1,1)*jac3di(i1-i10,i2-1-
     & i20,i3-1-i30))+8*(rsxy(i1,i2+2,i3-1,axisp1,1)*jac3di(i1-i10,i2+
     & 2-i20,i3-1-i30))+8*(rsxy(i1,i2-2,i3-1,axisp1,1)*jac3di(i1-i10,
     & i2-2-i20,i3-1-i30)))/(144.*dr(1)**2*dr(2))
                     a23sst = (240*(rsxy(i1,i2,i3-1,axisp1,2)*jac3di(
     & i1-i10,i2-i20,i3-1-i30))-240*(rsxy(i1,i2,i3+1,axisp1,2)*jac3di(
     & i1-i10,i2-i20,i3+1-i30))-16*(rsxy(i1,i2+1,i3+2,axisp1,2)*
     & jac3di(i1-i10,i2+1-i20,i3+2-i30))-16*(rsxy(i1,i2-1,i3+2,axisp1,
     & 2)*jac3di(i1-i10,i2-1-i20,i3+2-i30))+(rsxy(i1,i2+2,i3+2,axisp1,
     & 2)*jac3di(i1-i10,i2+2-i20,i3+2-i30))+(rsxy(i1,i2-2,i3+2,axisp1,
     & 2)*jac3di(i1-i10,i2-2-i20,i3+2-i30))+16*(rsxy(i1,i2+1,i3-2,
     & axisp1,2)*jac3di(i1-i10,i2+1-i20,i3-2-i30))+16*(rsxy(i1,i2-1,
     & i3-2,axisp1,2)*jac3di(i1-i10,i2-1-i20,i3-2-i30))-(rsxy(i1,i2+2,
     & i3-2,axisp1,2)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,
     & i3-2,axisp1,2)*jac3di(i1-i10,i2-2-i20,i3-2-i30))+30*(rsxy(i1,
     & i2,i3+2,axisp1,2)*jac3di(i1-i10,i2-i20,i3+2-i30))-30*(rsxy(i1,
     & i2,i3-2,axisp1,2)*jac3di(i1-i10,i2-i20,i3-2-i30))+128*(rsxy(i1,
     & i2+1,i3+1,axisp1,2)*jac3di(i1-i10,i2+1-i20,i3+1-i30))+128*(
     & rsxy(i1,i2-1,i3+1,axisp1,2)*jac3di(i1-i10,i2-1-i20,i3+1-i30))-
     & 8*(rsxy(i1,i2+2,i3+1,axisp1,2)*jac3di(i1-i10,i2+2-i20,i3+1-i30)
     & )-8*(rsxy(i1,i2-2,i3+1,axisp1,2)*jac3di(i1-i10,i2-2-i20,i3+1-
     & i30))-128*(rsxy(i1,i2+1,i3-1,axisp1,2)*jac3di(i1-i10,i2+1-i20,
     & i3-1-i30))-128*(rsxy(i1,i2-1,i3-1,axisp1,2)*jac3di(i1-i10,i2-1-
     & i20,i3-1-i30))+8*(rsxy(i1,i2+2,i3-1,axisp1,2)*jac3di(i1-i10,i2+
     & 2-i20,i3-1-i30))+8*(rsxy(i1,i2-2,i3-1,axisp1,2)*jac3di(i1-i10,
     & i2-2-i20,i3-1-i30)))/(144.*dr(1)**2*dr(2))
                     a31sst = (240*(rsxy(i1,i2,i3-1,axisp2,0)*jac3di(
     & i1-i10,i2-i20,i3-1-i30))-240*(rsxy(i1,i2,i3+1,axisp2,0)*jac3di(
     & i1-i10,i2-i20,i3+1-i30))-16*(rsxy(i1,i2+1,i3+2,axisp2,0)*
     & jac3di(i1-i10,i2+1-i20,i3+2-i30))-16*(rsxy(i1,i2-1,i3+2,axisp2,
     & 0)*jac3di(i1-i10,i2-1-i20,i3+2-i30))+(rsxy(i1,i2+2,i3+2,axisp2,
     & 0)*jac3di(i1-i10,i2+2-i20,i3+2-i30))+(rsxy(i1,i2-2,i3+2,axisp2,
     & 0)*jac3di(i1-i10,i2-2-i20,i3+2-i30))+16*(rsxy(i1,i2+1,i3-2,
     & axisp2,0)*jac3di(i1-i10,i2+1-i20,i3-2-i30))+16*(rsxy(i1,i2-1,
     & i3-2,axisp2,0)*jac3di(i1-i10,i2-1-i20,i3-2-i30))-(rsxy(i1,i2+2,
     & i3-2,axisp2,0)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,
     & i3-2,axisp2,0)*jac3di(i1-i10,i2-2-i20,i3-2-i30))+30*(rsxy(i1,
     & i2,i3+2,axisp2,0)*jac3di(i1-i10,i2-i20,i3+2-i30))-30*(rsxy(i1,
     & i2,i3-2,axisp2,0)*jac3di(i1-i10,i2-i20,i3-2-i30))+128*(rsxy(i1,
     & i2+1,i3+1,axisp2,0)*jac3di(i1-i10,i2+1-i20,i3+1-i30))+128*(
     & rsxy(i1,i2-1,i3+1,axisp2,0)*jac3di(i1-i10,i2-1-i20,i3+1-i30))-
     & 8*(rsxy(i1,i2+2,i3+1,axisp2,0)*jac3di(i1-i10,i2+2-i20,i3+1-i30)
     & )-8*(rsxy(i1,i2-2,i3+1,axisp2,0)*jac3di(i1-i10,i2-2-i20,i3+1-
     & i30))-128*(rsxy(i1,i2+1,i3-1,axisp2,0)*jac3di(i1-i10,i2+1-i20,
     & i3-1-i30))-128*(rsxy(i1,i2-1,i3-1,axisp2,0)*jac3di(i1-i10,i2-1-
     & i20,i3-1-i30))+8*(rsxy(i1,i2+2,i3-1,axisp2,0)*jac3di(i1-i10,i2+
     & 2-i20,i3-1-i30))+8*(rsxy(i1,i2-2,i3-1,axisp2,0)*jac3di(i1-i10,
     & i2-2-i20,i3-1-i30)))/(144.*dr(1)**2*dr(2))
                     a32sst = (240*(rsxy(i1,i2,i3-1,axisp2,1)*jac3di(
     & i1-i10,i2-i20,i3-1-i30))-240*(rsxy(i1,i2,i3+1,axisp2,1)*jac3di(
     & i1-i10,i2-i20,i3+1-i30))-16*(rsxy(i1,i2+1,i3+2,axisp2,1)*
     & jac3di(i1-i10,i2+1-i20,i3+2-i30))-16*(rsxy(i1,i2-1,i3+2,axisp2,
     & 1)*jac3di(i1-i10,i2-1-i20,i3+2-i30))+(rsxy(i1,i2+2,i3+2,axisp2,
     & 1)*jac3di(i1-i10,i2+2-i20,i3+2-i30))+(rsxy(i1,i2-2,i3+2,axisp2,
     & 1)*jac3di(i1-i10,i2-2-i20,i3+2-i30))+16*(rsxy(i1,i2+1,i3-2,
     & axisp2,1)*jac3di(i1-i10,i2+1-i20,i3-2-i30))+16*(rsxy(i1,i2-1,
     & i3-2,axisp2,1)*jac3di(i1-i10,i2-1-i20,i3-2-i30))-(rsxy(i1,i2+2,
     & i3-2,axisp2,1)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,
     & i3-2,axisp2,1)*jac3di(i1-i10,i2-2-i20,i3-2-i30))+30*(rsxy(i1,
     & i2,i3+2,axisp2,1)*jac3di(i1-i10,i2-i20,i3+2-i30))-30*(rsxy(i1,
     & i2,i3-2,axisp2,1)*jac3di(i1-i10,i2-i20,i3-2-i30))+128*(rsxy(i1,
     & i2+1,i3+1,axisp2,1)*jac3di(i1-i10,i2+1-i20,i3+1-i30))+128*(
     & rsxy(i1,i2-1,i3+1,axisp2,1)*jac3di(i1-i10,i2-1-i20,i3+1-i30))-
     & 8*(rsxy(i1,i2+2,i3+1,axisp2,1)*jac3di(i1-i10,i2+2-i20,i3+1-i30)
     & )-8*(rsxy(i1,i2-2,i3+1,axisp2,1)*jac3di(i1-i10,i2-2-i20,i3+1-
     & i30))-128*(rsxy(i1,i2+1,i3-1,axisp2,1)*jac3di(i1-i10,i2+1-i20,
     & i3-1-i30))-128*(rsxy(i1,i2-1,i3-1,axisp2,1)*jac3di(i1-i10,i2-1-
     & i20,i3-1-i30))+8*(rsxy(i1,i2+2,i3-1,axisp2,1)*jac3di(i1-i10,i2+
     & 2-i20,i3-1-i30))+8*(rsxy(i1,i2-2,i3-1,axisp2,1)*jac3di(i1-i10,
     & i2-2-i20,i3-1-i30)))/(144.*dr(1)**2*dr(2))
                     a33sst = (240*(rsxy(i1,i2,i3-1,axisp2,2)*jac3di(
     & i1-i10,i2-i20,i3-1-i30))-240*(rsxy(i1,i2,i3+1,axisp2,2)*jac3di(
     & i1-i10,i2-i20,i3+1-i30))-16*(rsxy(i1,i2+1,i3+2,axisp2,2)*
     & jac3di(i1-i10,i2+1-i20,i3+2-i30))-16*(rsxy(i1,i2-1,i3+2,axisp2,
     & 2)*jac3di(i1-i10,i2-1-i20,i3+2-i30))+(rsxy(i1,i2+2,i3+2,axisp2,
     & 2)*jac3di(i1-i10,i2+2-i20,i3+2-i30))+(rsxy(i1,i2-2,i3+2,axisp2,
     & 2)*jac3di(i1-i10,i2-2-i20,i3+2-i30))+16*(rsxy(i1,i2+1,i3-2,
     & axisp2,2)*jac3di(i1-i10,i2+1-i20,i3-2-i30))+16*(rsxy(i1,i2-1,
     & i3-2,axisp2,2)*jac3di(i1-i10,i2-1-i20,i3-2-i30))-(rsxy(i1,i2+2,
     & i3-2,axisp2,2)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,
     & i3-2,axisp2,2)*jac3di(i1-i10,i2-2-i20,i3-2-i30))+30*(rsxy(i1,
     & i2,i3+2,axisp2,2)*jac3di(i1-i10,i2-i20,i3+2-i30))-30*(rsxy(i1,
     & i2,i3-2,axisp2,2)*jac3di(i1-i10,i2-i20,i3-2-i30))+128*(rsxy(i1,
     & i2+1,i3+1,axisp2,2)*jac3di(i1-i10,i2+1-i20,i3+1-i30))+128*(
     & rsxy(i1,i2-1,i3+1,axisp2,2)*jac3di(i1-i10,i2-1-i20,i3+1-i30))-
     & 8*(rsxy(i1,i2+2,i3+1,axisp2,2)*jac3di(i1-i10,i2+2-i20,i3+1-i30)
     & )-8*(rsxy(i1,i2-2,i3+1,axisp2,2)*jac3di(i1-i10,i2-2-i20,i3+1-
     & i30))-128*(rsxy(i1,i2+1,i3-1,axisp2,2)*jac3di(i1-i10,i2+1-i20,
     & i3-1-i30))-128*(rsxy(i1,i2-1,i3-1,axisp2,2)*jac3di(i1-i10,i2-1-
     & i20,i3-1-i30))+8*(rsxy(i1,i2+2,i3-1,axisp2,2)*jac3di(i1-i10,i2+
     & 2-i20,i3-1-i30))+8*(rsxy(i1,i2-2,i3-1,axisp2,2)*jac3di(i1-i10,
     & i2-2-i20,i3-1-i30)))/(144.*dr(1)**2*dr(2))
                     ust=ust4(i1,i2,i3,ex)
                     usst=usst4(i1,i2,i3,ex)
                     ustt=ustt4(i1,i2,i3,ex)
                     vst  =ust4(i1,i2,i3,ey)
                     vsst=usst4(i1,i2,i3,ey)
                     vstt=ustt4(i1,i2,i3,ey)
                     wst  =ust4(i1,i2,i3,ez)
                     wsst=usst4(i1,i2,i3,ez)
                     wstt=ustt4(i1,i2,i3,ez)
                  else if( axis.eq.1 )then
                     a11rs = (8.*((8.*((rsxy(i1,i2+1,i3+1,axis,0)*
     & jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axis,0)*
     & jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,axis,0)*
     & jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,axis,0)*
     & jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*((rsxy(i1,
     & i2+1,i3-1,axis,0)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-(rsxy(i1,
     & i2-1,i3-1,axis,0)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))-((rsxy(i1,
     & i2+2,i3-1,axis,0)*jac3di(i1-i10,i2+2-i20,i3-1-i30))-(rsxy(i1,
     & i2-2,i3-1,axis,0)*jac3di(i1-i10,i2-2-i20,i3-1-i30))))/(12.*dr(
     & 1)))-((8.*((rsxy(i1,i2+1,i3+2,axis,0)*jac3di(i1-i10,i2+1-i20,
     & i3+2-i30))-(rsxy(i1,i2-1,i3+2,axis,0)*jac3di(i1-i10,i2-1-i20,
     & i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axis,0)*jac3di(i1-i10,i2+2-i20,
     & i3+2-i30))-(rsxy(i1,i2-2,i3+2,axis,0)*jac3di(i1-i10,i2-2-i20,
     & i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-2,axis,0)*
     & jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-2,axis,0)*
     & jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,i3-2,axis,0)*
     & jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,i3-2,axis,0)*
     & jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1))))/(12.*dr(2))
                     a12rs = (8.*((8.*((rsxy(i1,i2+1,i3+1,axis,1)*
     & jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axis,1)*
     & jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,axis,1)*
     & jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,axis,1)*
     & jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*((rsxy(i1,
     & i2+1,i3-1,axis,1)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-(rsxy(i1,
     & i2-1,i3-1,axis,1)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))-((rsxy(i1,
     & i2+2,i3-1,axis,1)*jac3di(i1-i10,i2+2-i20,i3-1-i30))-(rsxy(i1,
     & i2-2,i3-1,axis,1)*jac3di(i1-i10,i2-2-i20,i3-1-i30))))/(12.*dr(
     & 1)))-((8.*((rsxy(i1,i2+1,i3+2,axis,1)*jac3di(i1-i10,i2+1-i20,
     & i3+2-i30))-(rsxy(i1,i2-1,i3+2,axis,1)*jac3di(i1-i10,i2-1-i20,
     & i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axis,1)*jac3di(i1-i10,i2+2-i20,
     & i3+2-i30))-(rsxy(i1,i2-2,i3+2,axis,1)*jac3di(i1-i10,i2-2-i20,
     & i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-2,axis,1)*
     & jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-2,axis,1)*
     & jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,i3-2,axis,1)*
     & jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,i3-2,axis,1)*
     & jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1))))/(12.*dr(2))
                     a13rs = (8.*((8.*((rsxy(i1,i2+1,i3+1,axis,2)*
     & jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axis,2)*
     & jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,axis,2)*
     & jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,axis,2)*
     & jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*((rsxy(i1,
     & i2+1,i3-1,axis,2)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-(rsxy(i1,
     & i2-1,i3-1,axis,2)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))-((rsxy(i1,
     & i2+2,i3-1,axis,2)*jac3di(i1-i10,i2+2-i20,i3-1-i30))-(rsxy(i1,
     & i2-2,i3-1,axis,2)*jac3di(i1-i10,i2-2-i20,i3-1-i30))))/(12.*dr(
     & 1)))-((8.*((rsxy(i1,i2+1,i3+2,axis,2)*jac3di(i1-i10,i2+1-i20,
     & i3+2-i30))-(rsxy(i1,i2-1,i3+2,axis,2)*jac3di(i1-i10,i2-1-i20,
     & i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axis,2)*jac3di(i1-i10,i2+2-i20,
     & i3+2-i30))-(rsxy(i1,i2-2,i3+2,axis,2)*jac3di(i1-i10,i2-2-i20,
     & i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-2,axis,2)*
     & jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-2,axis,2)*
     & jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,i3-2,axis,2)*
     & jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,i3-2,axis,2)*
     & jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1))))/(12.*dr(2))
                     a21rs = (8.*((8.*((rsxy(i1,i2+1,i3+1,axisp1,0)*
     & jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axisp1,0)*
     & jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,axisp1,
     & 0)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,axisp1,
     & 0)*jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*((rsxy(
     & i1,i2+1,i3-1,axisp1,0)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-(rsxy(
     & i1,i2-1,i3-1,axisp1,0)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))-((
     & rsxy(i1,i2+2,i3-1,axisp1,0)*jac3di(i1-i10,i2+2-i20,i3-1-i30))-(
     & rsxy(i1,i2-2,i3-1,axisp1,0)*jac3di(i1-i10,i2-2-i20,i3-1-i30))))
     & /(12.*dr(1)))-((8.*((rsxy(i1,i2+1,i3+2,axisp1,0)*jac3di(i1-i10,
     & i2+1-i20,i3+2-i30))-(rsxy(i1,i2-1,i3+2,axisp1,0)*jac3di(i1-i10,
     & i2-1-i20,i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axisp1,0)*jac3di(i1-
     & i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp1,0)*jac3di(i1-
     & i10,i2-2-i20,i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-2,
     & axisp1,0)*jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-2,
     & axisp1,0)*jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,i3-
     & 2,axisp1,0)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,i3-
     & 2,axisp1,0)*jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1))))/(
     & 12.*dr(2))
                     a22rs = (8.*((8.*((rsxy(i1,i2+1,i3+1,axisp1,1)*
     & jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axisp1,1)*
     & jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,axisp1,
     & 1)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,axisp1,
     & 1)*jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*((rsxy(
     & i1,i2+1,i3-1,axisp1,1)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-(rsxy(
     & i1,i2-1,i3-1,axisp1,1)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))-((
     & rsxy(i1,i2+2,i3-1,axisp1,1)*jac3di(i1-i10,i2+2-i20,i3-1-i30))-(
     & rsxy(i1,i2-2,i3-1,axisp1,1)*jac3di(i1-i10,i2-2-i20,i3-1-i30))))
     & /(12.*dr(1)))-((8.*((rsxy(i1,i2+1,i3+2,axisp1,1)*jac3di(i1-i10,
     & i2+1-i20,i3+2-i30))-(rsxy(i1,i2-1,i3+2,axisp1,1)*jac3di(i1-i10,
     & i2-1-i20,i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axisp1,1)*jac3di(i1-
     & i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp1,1)*jac3di(i1-
     & i10,i2-2-i20,i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-2,
     & axisp1,1)*jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-2,
     & axisp1,1)*jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,i3-
     & 2,axisp1,1)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,i3-
     & 2,axisp1,1)*jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1))))/(
     & 12.*dr(2))
                     a23rs = (8.*((8.*((rsxy(i1,i2+1,i3+1,axisp1,2)*
     & jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axisp1,2)*
     & jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,axisp1,
     & 2)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,axisp1,
     & 2)*jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*((rsxy(
     & i1,i2+1,i3-1,axisp1,2)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-(rsxy(
     & i1,i2-1,i3-1,axisp1,2)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))-((
     & rsxy(i1,i2+2,i3-1,axisp1,2)*jac3di(i1-i10,i2+2-i20,i3-1-i30))-(
     & rsxy(i1,i2-2,i3-1,axisp1,2)*jac3di(i1-i10,i2-2-i20,i3-1-i30))))
     & /(12.*dr(1)))-((8.*((rsxy(i1,i2+1,i3+2,axisp1,2)*jac3di(i1-i10,
     & i2+1-i20,i3+2-i30))-(rsxy(i1,i2-1,i3+2,axisp1,2)*jac3di(i1-i10,
     & i2-1-i20,i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axisp1,2)*jac3di(i1-
     & i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp1,2)*jac3di(i1-
     & i10,i2-2-i20,i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-2,
     & axisp1,2)*jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-2,
     & axisp1,2)*jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,i3-
     & 2,axisp1,2)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,i3-
     & 2,axisp1,2)*jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1))))/(
     & 12.*dr(2))
                     a31rs = (8.*((8.*((rsxy(i1,i2+1,i3+1,axisp2,0)*
     & jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axisp2,0)*
     & jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,axisp2,
     & 0)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,axisp2,
     & 0)*jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*((rsxy(
     & i1,i2+1,i3-1,axisp2,0)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-(rsxy(
     & i1,i2-1,i3-1,axisp2,0)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))-((
     & rsxy(i1,i2+2,i3-1,axisp2,0)*jac3di(i1-i10,i2+2-i20,i3-1-i30))-(
     & rsxy(i1,i2-2,i3-1,axisp2,0)*jac3di(i1-i10,i2-2-i20,i3-1-i30))))
     & /(12.*dr(1)))-((8.*((rsxy(i1,i2+1,i3+2,axisp2,0)*jac3di(i1-i10,
     & i2+1-i20,i3+2-i30))-(rsxy(i1,i2-1,i3+2,axisp2,0)*jac3di(i1-i10,
     & i2-1-i20,i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axisp2,0)*jac3di(i1-
     & i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp2,0)*jac3di(i1-
     & i10,i2-2-i20,i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-2,
     & axisp2,0)*jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-2,
     & axisp2,0)*jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,i3-
     & 2,axisp2,0)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,i3-
     & 2,axisp2,0)*jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1))))/(
     & 12.*dr(2))
                     a32rs = (8.*((8.*((rsxy(i1,i2+1,i3+1,axisp2,1)*
     & jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axisp2,1)*
     & jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,axisp2,
     & 1)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,axisp2,
     & 1)*jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*((rsxy(
     & i1,i2+1,i3-1,axisp2,1)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-(rsxy(
     & i1,i2-1,i3-1,axisp2,1)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))-((
     & rsxy(i1,i2+2,i3-1,axisp2,1)*jac3di(i1-i10,i2+2-i20,i3-1-i30))-(
     & rsxy(i1,i2-2,i3-1,axisp2,1)*jac3di(i1-i10,i2-2-i20,i3-1-i30))))
     & /(12.*dr(1)))-((8.*((rsxy(i1,i2+1,i3+2,axisp2,1)*jac3di(i1-i10,
     & i2+1-i20,i3+2-i30))-(rsxy(i1,i2-1,i3+2,axisp2,1)*jac3di(i1-i10,
     & i2-1-i20,i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axisp2,1)*jac3di(i1-
     & i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp2,1)*jac3di(i1-
     & i10,i2-2-i20,i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-2,
     & axisp2,1)*jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-2,
     & axisp2,1)*jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,i3-
     & 2,axisp2,1)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,i3-
     & 2,axisp2,1)*jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1))))/(
     & 12.*dr(2))
                     a33rs = (8.*((8.*((rsxy(i1,i2+1,i3+1,axisp2,2)*
     & jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axisp2,2)*
     & jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,axisp2,
     & 2)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,axisp2,
     & 2)*jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*((rsxy(
     & i1,i2+1,i3-1,axisp2,2)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-(rsxy(
     & i1,i2-1,i3-1,axisp2,2)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))-((
     & rsxy(i1,i2+2,i3-1,axisp2,2)*jac3di(i1-i10,i2+2-i20,i3-1-i30))-(
     & rsxy(i1,i2-2,i3-1,axisp2,2)*jac3di(i1-i10,i2-2-i20,i3-1-i30))))
     & /(12.*dr(1)))-((8.*((rsxy(i1,i2+1,i3+2,axisp2,2)*jac3di(i1-i10,
     & i2+1-i20,i3+2-i30))-(rsxy(i1,i2-1,i3+2,axisp2,2)*jac3di(i1-i10,
     & i2-1-i20,i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axisp2,2)*jac3di(i1-
     & i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp2,2)*jac3di(i1-
     & i10,i2-2-i20,i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-2,
     & axisp2,2)*jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-2,
     & axisp2,2)*jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,i3-
     & 2,axisp2,2)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,i3-
     & 2,axisp2,2)*jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1))))/(
     & 12.*dr(2))
                     a11rs = (8.*((8.*((rsxy(i1,i2+1,i3+1,axis,0)*
     & jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axis,0)*
     & jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,axis,0)*
     & jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,axis,0)*
     & jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*((rsxy(i1,
     & i2+1,i3-1,axis,0)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-(rsxy(i1,
     & i2-1,i3-1,axis,0)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))-((rsxy(i1,
     & i2+2,i3-1,axis,0)*jac3di(i1-i10,i2+2-i20,i3-1-i30))-(rsxy(i1,
     & i2-2,i3-1,axis,0)*jac3di(i1-i10,i2-2-i20,i3-1-i30))))/(12.*dr(
     & 1)))-((8.*((rsxy(i1,i2+1,i3+2,axis,0)*jac3di(i1-i10,i2+1-i20,
     & i3+2-i30))-(rsxy(i1,i2-1,i3+2,axis,0)*jac3di(i1-i10,i2-1-i20,
     & i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axis,0)*jac3di(i1-i10,i2+2-i20,
     & i3+2-i30))-(rsxy(i1,i2-2,i3+2,axis,0)*jac3di(i1-i10,i2-2-i20,
     & i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-2,axis,0)*
     & jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-2,axis,0)*
     & jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,i3-2,axis,0)*
     & jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,i3-2,axis,0)*
     & jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1))))/(12.*dr(2))
                     a12rs = (8.*((8.*((rsxy(i1,i2+1,i3+1,axis,1)*
     & jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axis,1)*
     & jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,axis,1)*
     & jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,axis,1)*
     & jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*((rsxy(i1,
     & i2+1,i3-1,axis,1)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-(rsxy(i1,
     & i2-1,i3-1,axis,1)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))-((rsxy(i1,
     & i2+2,i3-1,axis,1)*jac3di(i1-i10,i2+2-i20,i3-1-i30))-(rsxy(i1,
     & i2-2,i3-1,axis,1)*jac3di(i1-i10,i2-2-i20,i3-1-i30))))/(12.*dr(
     & 1)))-((8.*((rsxy(i1,i2+1,i3+2,axis,1)*jac3di(i1-i10,i2+1-i20,
     & i3+2-i30))-(rsxy(i1,i2-1,i3+2,axis,1)*jac3di(i1-i10,i2-1-i20,
     & i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axis,1)*jac3di(i1-i10,i2+2-i20,
     & i3+2-i30))-(rsxy(i1,i2-2,i3+2,axis,1)*jac3di(i1-i10,i2-2-i20,
     & i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-2,axis,1)*
     & jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-2,axis,1)*
     & jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,i3-2,axis,1)*
     & jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,i3-2,axis,1)*
     & jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1))))/(12.*dr(2))
                     a13rs = (8.*((8.*((rsxy(i1,i2+1,i3+1,axis,2)*
     & jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axis,2)*
     & jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,axis,2)*
     & jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,axis,2)*
     & jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*((rsxy(i1,
     & i2+1,i3-1,axis,2)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-(rsxy(i1,
     & i2-1,i3-1,axis,2)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))-((rsxy(i1,
     & i2+2,i3-1,axis,2)*jac3di(i1-i10,i2+2-i20,i3-1-i30))-(rsxy(i1,
     & i2-2,i3-1,axis,2)*jac3di(i1-i10,i2-2-i20,i3-1-i30))))/(12.*dr(
     & 1)))-((8.*((rsxy(i1,i2+1,i3+2,axis,2)*jac3di(i1-i10,i2+1-i20,
     & i3+2-i30))-(rsxy(i1,i2-1,i3+2,axis,2)*jac3di(i1-i10,i2-1-i20,
     & i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axis,2)*jac3di(i1-i10,i2+2-i20,
     & i3+2-i30))-(rsxy(i1,i2-2,i3+2,axis,2)*jac3di(i1-i10,i2-2-i20,
     & i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-2,axis,2)*
     & jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-2,axis,2)*
     & jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,i3-2,axis,2)*
     & jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,i3-2,axis,2)*
     & jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1))))/(12.*dr(2))
                     a21rt = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp1,0)*
     & jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axisp1,0)*
     & jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,axisp1,
     & 0)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,axisp1,
     & 0)*jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(
     & i1+1,i2-1,i3,axisp1,0)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-(rsxy(
     & i1-1,i2-1,i3,axisp1,0)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))-((
     & rsxy(i1+2,i2-1,i3,axisp1,0)*jac3di(i1+2-i10,i2-1-i20,i3-i30))-(
     & rsxy(i1-2,i2-1,i3,axisp1,0)*jac3di(i1-2-i10,i2-1-i20,i3-i30))))
     & /(12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp1,0)*jac3di(i1+1-
     & i10,i2+2-i20,i3-i30))-(rsxy(i1-1,i2+2,i3,axisp1,0)*jac3di(i1-1-
     & i10,i2+2-i20,i3-i30)))-((rsxy(i1+2,i2+2,i3,axisp1,0)*jac3di(i1+
     & 2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,i2+2,i3,axisp1,0)*jac3di(i1-
     & 2-i10,i2+2-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,i3,
     & axisp1,0)*jac3di(i1+1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,i3,
     & axisp1,0)*jac3di(i1-1-i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-2,
     & i3,axisp1,0)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-2,
     & i3,axisp1,0)*jac3di(i1-2-i10,i2-2-i20,i3-i30))))/(12.*dr(0))))
     & /(12.*dr(1))
                     a22rt = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp1,1)*
     & jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axisp1,1)*
     & jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,axisp1,
     & 1)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,axisp1,
     & 1)*jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(
     & i1+1,i2-1,i3,axisp1,1)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-(rsxy(
     & i1-1,i2-1,i3,axisp1,1)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))-((
     & rsxy(i1+2,i2-1,i3,axisp1,1)*jac3di(i1+2-i10,i2-1-i20,i3-i30))-(
     & rsxy(i1-2,i2-1,i3,axisp1,1)*jac3di(i1-2-i10,i2-1-i20,i3-i30))))
     & /(12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp1,1)*jac3di(i1+1-
     & i10,i2+2-i20,i3-i30))-(rsxy(i1-1,i2+2,i3,axisp1,1)*jac3di(i1-1-
     & i10,i2+2-i20,i3-i30)))-((rsxy(i1+2,i2+2,i3,axisp1,1)*jac3di(i1+
     & 2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,i2+2,i3,axisp1,1)*jac3di(i1-
     & 2-i10,i2+2-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,i3,
     & axisp1,1)*jac3di(i1+1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,i3,
     & axisp1,1)*jac3di(i1-1-i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-2,
     & i3,axisp1,1)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-2,
     & i3,axisp1,1)*jac3di(i1-2-i10,i2-2-i20,i3-i30))))/(12.*dr(0))))
     & /(12.*dr(1))
                     a23rt = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp1,2)*
     & jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axisp1,2)*
     & jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,axisp1,
     & 2)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,axisp1,
     & 2)*jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(
     & i1+1,i2-1,i3,axisp1,2)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-(rsxy(
     & i1-1,i2-1,i3,axisp1,2)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))-((
     & rsxy(i1+2,i2-1,i3,axisp1,2)*jac3di(i1+2-i10,i2-1-i20,i3-i30))-(
     & rsxy(i1-2,i2-1,i3,axisp1,2)*jac3di(i1-2-i10,i2-1-i20,i3-i30))))
     & /(12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp1,2)*jac3di(i1+1-
     & i10,i2+2-i20,i3-i30))-(rsxy(i1-1,i2+2,i3,axisp1,2)*jac3di(i1-1-
     & i10,i2+2-i20,i3-i30)))-((rsxy(i1+2,i2+2,i3,axisp1,2)*jac3di(i1+
     & 2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,i2+2,i3,axisp1,2)*jac3di(i1-
     & 2-i10,i2+2-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,i3,
     & axisp1,2)*jac3di(i1+1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,i3,
     & axisp1,2)*jac3di(i1-1-i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-2,
     & i3,axisp1,2)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-2,
     & i3,axisp1,2)*jac3di(i1-2-i10,i2-2-i20,i3-i30))))/(12.*dr(0))))
     & /(12.*dr(1))
                     a31rt = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp2,0)*
     & jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axisp2,0)*
     & jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,axisp2,
     & 0)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,axisp2,
     & 0)*jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(
     & i1+1,i2-1,i3,axisp2,0)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-(rsxy(
     & i1-1,i2-1,i3,axisp2,0)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))-((
     & rsxy(i1+2,i2-1,i3,axisp2,0)*jac3di(i1+2-i10,i2-1-i20,i3-i30))-(
     & rsxy(i1-2,i2-1,i3,axisp2,0)*jac3di(i1-2-i10,i2-1-i20,i3-i30))))
     & /(12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp2,0)*jac3di(i1+1-
     & i10,i2+2-i20,i3-i30))-(rsxy(i1-1,i2+2,i3,axisp2,0)*jac3di(i1-1-
     & i10,i2+2-i20,i3-i30)))-((rsxy(i1+2,i2+2,i3,axisp2,0)*jac3di(i1+
     & 2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,i2+2,i3,axisp2,0)*jac3di(i1-
     & 2-i10,i2+2-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,i3,
     & axisp2,0)*jac3di(i1+1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,i3,
     & axisp2,0)*jac3di(i1-1-i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-2,
     & i3,axisp2,0)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-2,
     & i3,axisp2,0)*jac3di(i1-2-i10,i2-2-i20,i3-i30))))/(12.*dr(0))))
     & /(12.*dr(1))
                     a32rt = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp2,1)*
     & jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axisp2,1)*
     & jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,axisp2,
     & 1)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,axisp2,
     & 1)*jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(
     & i1+1,i2-1,i3,axisp2,1)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-(rsxy(
     & i1-1,i2-1,i3,axisp2,1)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))-((
     & rsxy(i1+2,i2-1,i3,axisp2,1)*jac3di(i1+2-i10,i2-1-i20,i3-i30))-(
     & rsxy(i1-2,i2-1,i3,axisp2,1)*jac3di(i1-2-i10,i2-1-i20,i3-i30))))
     & /(12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp2,1)*jac3di(i1+1-
     & i10,i2+2-i20,i3-i30))-(rsxy(i1-1,i2+2,i3,axisp2,1)*jac3di(i1-1-
     & i10,i2+2-i20,i3-i30)))-((rsxy(i1+2,i2+2,i3,axisp2,1)*jac3di(i1+
     & 2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,i2+2,i3,axisp2,1)*jac3di(i1-
     & 2-i10,i2+2-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,i3,
     & axisp2,1)*jac3di(i1+1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,i3,
     & axisp2,1)*jac3di(i1-1-i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-2,
     & i3,axisp2,1)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-2,
     & i3,axisp2,1)*jac3di(i1-2-i10,i2-2-i20,i3-i30))))/(12.*dr(0))))
     & /(12.*dr(1))
                     a33rt = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp2,2)*
     & jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axisp2,2)*
     & jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,axisp2,
     & 2)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,axisp2,
     & 2)*jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(
     & i1+1,i2-1,i3,axisp2,2)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-(rsxy(
     & i1-1,i2-1,i3,axisp2,2)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))-((
     & rsxy(i1+2,i2-1,i3,axisp2,2)*jac3di(i1+2-i10,i2-1-i20,i3-i30))-(
     & rsxy(i1-2,i2-1,i3,axisp2,2)*jac3di(i1-2-i10,i2-1-i20,i3-i30))))
     & /(12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp2,2)*jac3di(i1+1-
     & i10,i2+2-i20,i3-i30))-(rsxy(i1-1,i2+2,i3,axisp2,2)*jac3di(i1-1-
     & i10,i2+2-i20,i3-i30)))-((rsxy(i1+2,i2+2,i3,axisp2,2)*jac3di(i1+
     & 2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,i2+2,i3,axisp2,2)*jac3di(i1-
     & 2-i10,i2+2-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,i3,
     & axisp2,2)*jac3di(i1+1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,i3,
     & axisp2,2)*jac3di(i1-1-i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-2,
     & i3,axisp2,2)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-2,
     & i3,axisp2,2)*jac3di(i1-2-i10,i2-2-i20,i3-i30))))/(12.*dr(0))))
     & /(12.*dr(1))
                     a11st = (8.*((8.*((rsxy(i1+1,i2,i3+1,axis,0)*
     & jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axis,0)*
     & jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,axis,0)*
     & jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,axis,0)*
     & jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*((rsxy(i1+
     & 1,i2,i3-1,axis,0)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-(rsxy(i1-1,
     & i2,i3-1,axis,0)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))-((rsxy(i1+2,
     & i2,i3-1,axis,0)*jac3di(i1+2-i10,i2-i20,i3-1-i30))-(rsxy(i1-2,
     & i2,i3-1,axis,0)*jac3di(i1-2-i10,i2-i20,i3-1-i30))))/(12.*dr(0))
     & )-((8.*((rsxy(i1+1,i2,i3+2,axis,0)*jac3di(i1+1-i10,i2-i20,i3+2-
     & i30))-(rsxy(i1-1,i2,i3+2,axis,0)*jac3di(i1-1-i10,i2-i20,i3+2-
     & i30)))-((rsxy(i1+2,i2,i3+2,axis,0)*jac3di(i1+2-i10,i2-i20,i3+2-
     & i30))-(rsxy(i1-2,i2,i3+2,axis,0)*jac3di(i1-2-i10,i2-i20,i3+2-
     & i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-2,axis,0)*jac3di(i1+
     & 1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-2,axis,0)*jac3di(i1-1-
     & i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,i3-2,axis,0)*jac3di(i1+2-
     & i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,i3-2,axis,0)*jac3di(i1-2-
     & i10,i2-i20,i3-2-i30))))/(12.*dr(0))))/(12.*dr(2))
                     a12st = (8.*((8.*((rsxy(i1+1,i2,i3+1,axis,1)*
     & jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axis,1)*
     & jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,axis,1)*
     & jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,axis,1)*
     & jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*((rsxy(i1+
     & 1,i2,i3-1,axis,1)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-(rsxy(i1-1,
     & i2,i3-1,axis,1)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))-((rsxy(i1+2,
     & i2,i3-1,axis,1)*jac3di(i1+2-i10,i2-i20,i3-1-i30))-(rsxy(i1-2,
     & i2,i3-1,axis,1)*jac3di(i1-2-i10,i2-i20,i3-1-i30))))/(12.*dr(0))
     & )-((8.*((rsxy(i1+1,i2,i3+2,axis,1)*jac3di(i1+1-i10,i2-i20,i3+2-
     & i30))-(rsxy(i1-1,i2,i3+2,axis,1)*jac3di(i1-1-i10,i2-i20,i3+2-
     & i30)))-((rsxy(i1+2,i2,i3+2,axis,1)*jac3di(i1+2-i10,i2-i20,i3+2-
     & i30))-(rsxy(i1-2,i2,i3+2,axis,1)*jac3di(i1-2-i10,i2-i20,i3+2-
     & i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-2,axis,1)*jac3di(i1+
     & 1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-2,axis,1)*jac3di(i1-1-
     & i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,i3-2,axis,1)*jac3di(i1+2-
     & i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,i3-2,axis,1)*jac3di(i1-2-
     & i10,i2-i20,i3-2-i30))))/(12.*dr(0))))/(12.*dr(2))
                     a13st = (8.*((8.*((rsxy(i1+1,i2,i3+1,axis,2)*
     & jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axis,2)*
     & jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,axis,2)*
     & jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,axis,2)*
     & jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*((rsxy(i1+
     & 1,i2,i3-1,axis,2)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-(rsxy(i1-1,
     & i2,i3-1,axis,2)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))-((rsxy(i1+2,
     & i2,i3-1,axis,2)*jac3di(i1+2-i10,i2-i20,i3-1-i30))-(rsxy(i1-2,
     & i2,i3-1,axis,2)*jac3di(i1-2-i10,i2-i20,i3-1-i30))))/(12.*dr(0))
     & )-((8.*((rsxy(i1+1,i2,i3+2,axis,2)*jac3di(i1+1-i10,i2-i20,i3+2-
     & i30))-(rsxy(i1-1,i2,i3+2,axis,2)*jac3di(i1-1-i10,i2-i20,i3+2-
     & i30)))-((rsxy(i1+2,i2,i3+2,axis,2)*jac3di(i1+2-i10,i2-i20,i3+2-
     & i30))-(rsxy(i1-2,i2,i3+2,axis,2)*jac3di(i1-2-i10,i2-i20,i3+2-
     & i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-2,axis,2)*jac3di(i1+
     & 1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-2,axis,2)*jac3di(i1-1-
     & i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,i3-2,axis,2)*jac3di(i1+2-
     & i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,i3-2,axis,2)*jac3di(i1-2-
     & i10,i2-i20,i3-2-i30))))/(12.*dr(0))))/(12.*dr(2))
                     a21st = (8.*((8.*((rsxy(i1+1,i2,i3+1,axisp1,0)*
     & jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axisp1,0)*
     & jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,axisp1,
     & 0)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,axisp1,
     & 0)*jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*((rsxy(
     & i1+1,i2,i3-1,axisp1,0)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-(rsxy(
     & i1-1,i2,i3-1,axisp1,0)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))-((
     & rsxy(i1+2,i2,i3-1,axisp1,0)*jac3di(i1+2-i10,i2-i20,i3-1-i30))-(
     & rsxy(i1-2,i2,i3-1,axisp1,0)*jac3di(i1-2-i10,i2-i20,i3-1-i30))))
     & /(12.*dr(0)))-((8.*((rsxy(i1+1,i2,i3+2,axisp1,0)*jac3di(i1+1-
     & i10,i2-i20,i3+2-i30))-(rsxy(i1-1,i2,i3+2,axisp1,0)*jac3di(i1-1-
     & i10,i2-i20,i3+2-i30)))-((rsxy(i1+2,i2,i3+2,axisp1,0)*jac3di(i1+
     & 2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,i2,i3+2,axisp1,0)*jac3di(i1-
     & 2-i10,i2-i20,i3+2-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-2,
     & axisp1,0)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-2,
     & axisp1,0)*jac3di(i1-1-i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,i3-
     & 2,axisp1,0)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,i3-
     & 2,axisp1,0)*jac3di(i1-2-i10,i2-i20,i3-2-i30))))/(12.*dr(0))))/(
     & 12.*dr(2))
                     a22st = (8.*((8.*((rsxy(i1+1,i2,i3+1,axisp1,1)*
     & jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axisp1,1)*
     & jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,axisp1,
     & 1)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,axisp1,
     & 1)*jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*((rsxy(
     & i1+1,i2,i3-1,axisp1,1)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-(rsxy(
     & i1-1,i2,i3-1,axisp1,1)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))-((
     & rsxy(i1+2,i2,i3-1,axisp1,1)*jac3di(i1+2-i10,i2-i20,i3-1-i30))-(
     & rsxy(i1-2,i2,i3-1,axisp1,1)*jac3di(i1-2-i10,i2-i20,i3-1-i30))))
     & /(12.*dr(0)))-((8.*((rsxy(i1+1,i2,i3+2,axisp1,1)*jac3di(i1+1-
     & i10,i2-i20,i3+2-i30))-(rsxy(i1-1,i2,i3+2,axisp1,1)*jac3di(i1-1-
     & i10,i2-i20,i3+2-i30)))-((rsxy(i1+2,i2,i3+2,axisp1,1)*jac3di(i1+
     & 2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,i2,i3+2,axisp1,1)*jac3di(i1-
     & 2-i10,i2-i20,i3+2-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-2,
     & axisp1,1)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-2,
     & axisp1,1)*jac3di(i1-1-i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,i3-
     & 2,axisp1,1)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,i3-
     & 2,axisp1,1)*jac3di(i1-2-i10,i2-i20,i3-2-i30))))/(12.*dr(0))))/(
     & 12.*dr(2))
                     a23st = (8.*((8.*((rsxy(i1+1,i2,i3+1,axisp1,2)*
     & jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axisp1,2)*
     & jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,axisp1,
     & 2)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,axisp1,
     & 2)*jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*((rsxy(
     & i1+1,i2,i3-1,axisp1,2)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-(rsxy(
     & i1-1,i2,i3-1,axisp1,2)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))-((
     & rsxy(i1+2,i2,i3-1,axisp1,2)*jac3di(i1+2-i10,i2-i20,i3-1-i30))-(
     & rsxy(i1-2,i2,i3-1,axisp1,2)*jac3di(i1-2-i10,i2-i20,i3-1-i30))))
     & /(12.*dr(0)))-((8.*((rsxy(i1+1,i2,i3+2,axisp1,2)*jac3di(i1+1-
     & i10,i2-i20,i3+2-i30))-(rsxy(i1-1,i2,i3+2,axisp1,2)*jac3di(i1-1-
     & i10,i2-i20,i3+2-i30)))-((rsxy(i1+2,i2,i3+2,axisp1,2)*jac3di(i1+
     & 2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,i2,i3+2,axisp1,2)*jac3di(i1-
     & 2-i10,i2-i20,i3+2-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-2,
     & axisp1,2)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-2,
     & axisp1,2)*jac3di(i1-1-i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,i3-
     & 2,axisp1,2)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,i3-
     & 2,axisp1,2)*jac3di(i1-2-i10,i2-i20,i3-2-i30))))/(12.*dr(0))))/(
     & 12.*dr(2))
                     a31st = (8.*((8.*((rsxy(i1+1,i2,i3+1,axisp2,0)*
     & jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axisp2,0)*
     & jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,axisp2,
     & 0)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,axisp2,
     & 0)*jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*((rsxy(
     & i1+1,i2,i3-1,axisp2,0)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-(rsxy(
     & i1-1,i2,i3-1,axisp2,0)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))-((
     & rsxy(i1+2,i2,i3-1,axisp2,0)*jac3di(i1+2-i10,i2-i20,i3-1-i30))-(
     & rsxy(i1-2,i2,i3-1,axisp2,0)*jac3di(i1-2-i10,i2-i20,i3-1-i30))))
     & /(12.*dr(0)))-((8.*((rsxy(i1+1,i2,i3+2,axisp2,0)*jac3di(i1+1-
     & i10,i2-i20,i3+2-i30))-(rsxy(i1-1,i2,i3+2,axisp2,0)*jac3di(i1-1-
     & i10,i2-i20,i3+2-i30)))-((rsxy(i1+2,i2,i3+2,axisp2,0)*jac3di(i1+
     & 2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,i2,i3+2,axisp2,0)*jac3di(i1-
     & 2-i10,i2-i20,i3+2-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-2,
     & axisp2,0)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-2,
     & axisp2,0)*jac3di(i1-1-i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,i3-
     & 2,axisp2,0)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,i3-
     & 2,axisp2,0)*jac3di(i1-2-i10,i2-i20,i3-2-i30))))/(12.*dr(0))))/(
     & 12.*dr(2))
                     a32st = (8.*((8.*((rsxy(i1+1,i2,i3+1,axisp2,1)*
     & jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axisp2,1)*
     & jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,axisp2,
     & 1)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,axisp2,
     & 1)*jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*((rsxy(
     & i1+1,i2,i3-1,axisp2,1)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-(rsxy(
     & i1-1,i2,i3-1,axisp2,1)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))-((
     & rsxy(i1+2,i2,i3-1,axisp2,1)*jac3di(i1+2-i10,i2-i20,i3-1-i30))-(
     & rsxy(i1-2,i2,i3-1,axisp2,1)*jac3di(i1-2-i10,i2-i20,i3-1-i30))))
     & /(12.*dr(0)))-((8.*((rsxy(i1+1,i2,i3+2,axisp2,1)*jac3di(i1+1-
     & i10,i2-i20,i3+2-i30))-(rsxy(i1-1,i2,i3+2,axisp2,1)*jac3di(i1-1-
     & i10,i2-i20,i3+2-i30)))-((rsxy(i1+2,i2,i3+2,axisp2,1)*jac3di(i1+
     & 2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,i2,i3+2,axisp2,1)*jac3di(i1-
     & 2-i10,i2-i20,i3+2-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-2,
     & axisp2,1)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-2,
     & axisp2,1)*jac3di(i1-1-i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,i3-
     & 2,axisp2,1)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,i3-
     & 2,axisp2,1)*jac3di(i1-2-i10,i2-i20,i3-2-i30))))/(12.*dr(0))))/(
     & 12.*dr(2))
                     a33st = (8.*((8.*((rsxy(i1+1,i2,i3+1,axisp2,2)*
     & jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axisp2,2)*
     & jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,axisp2,
     & 2)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,axisp2,
     & 2)*jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*((rsxy(
     & i1+1,i2,i3-1,axisp2,2)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-(rsxy(
     & i1-1,i2,i3-1,axisp2,2)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))-((
     & rsxy(i1+2,i2,i3-1,axisp2,2)*jac3di(i1+2-i10,i2-i20,i3-1-i30))-(
     & rsxy(i1-2,i2,i3-1,axisp2,2)*jac3di(i1-2-i10,i2-i20,i3-1-i30))))
     & /(12.*dr(0)))-((8.*((rsxy(i1+1,i2,i3+2,axisp2,2)*jac3di(i1+1-
     & i10,i2-i20,i3+2-i30))-(rsxy(i1-1,i2,i3+2,axisp2,2)*jac3di(i1-1-
     & i10,i2-i20,i3+2-i30)))-((rsxy(i1+2,i2,i3+2,axisp2,2)*jac3di(i1+
     & 2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,i2,i3+2,axisp2,2)*jac3di(i1-
     & 2-i10,i2-i20,i3+2-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-2,
     & axisp2,2)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-2,
     & axisp2,2)*jac3di(i1-1-i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,i3-
     & 2,axisp2,2)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,i3-
     & 2,axisp2,2)*jac3di(i1-2-i10,i2-i20,i3-2-i30))))/(12.*dr(0))))/(
     & 12.*dr(2))
                     a11rss = (-240*(rsxy(i1,i2+1,i3,axis,0)*jac3di(i1-
     & i10,i2+1-i20,i3-i30))+240*(rsxy(i1,i2-1,i3,axis,0)*jac3di(i1-
     & i10,i2-1-i20,i3-i30))-8*(rsxy(i1,i2+1,i3+2,axis,0)*jac3di(i1-
     & i10,i2+1-i20,i3+2-i30))+8*(rsxy(i1,i2-1,i3+2,axis,0)*jac3di(i1-
     & i10,i2-1-i20,i3+2-i30))+(rsxy(i1,i2+2,i3+2,axis,0)*jac3di(i1-
     & i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axis,0)*jac3di(i1-
     & i10,i2-2-i20,i3+2-i30))-8*(rsxy(i1,i2+1,i3-2,axis,0)*jac3di(i1-
     & i10,i2+1-i20,i3-2-i30))+8*(rsxy(i1,i2-1,i3-2,axis,0)*jac3di(i1-
     & i10,i2-1-i20,i3-2-i30))+(rsxy(i1,i2+2,i3-2,axis,0)*jac3di(i1-
     & i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,i3-2,axis,0)*jac3di(i1-
     & i10,i2-2-i20,i3-2-i30))+30*(rsxy(i1,i2+2,i3,axis,0)*jac3di(i1-
     & i10,i2+2-i20,i3-i30))-30*(rsxy(i1,i2-2,i3,axis,0)*jac3di(i1-
     & i10,i2-2-i20,i3-i30))+128*(rsxy(i1,i2+1,i3+1,axis,0)*jac3di(i1-
     & i10,i2+1-i20,i3+1-i30))-128*(rsxy(i1,i2-1,i3+1,axis,0)*jac3di(
     & i1-i10,i2-1-i20,i3+1-i30))-16*(rsxy(i1,i2+2,i3+1,axis,0)*
     & jac3di(i1-i10,i2+2-i20,i3+1-i30))+16*(rsxy(i1,i2-2,i3+1,axis,0)
     & *jac3di(i1-i10,i2-2-i20,i3+1-i30))+128*(rsxy(i1,i2+1,i3-1,axis,
     & 0)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-128*(rsxy(i1,i2-1,i3-1,
     & axis,0)*jac3di(i1-i10,i2-1-i20,i3-1-i30))-16*(rsxy(i1,i2+2,i3-
     & 1,axis,0)*jac3di(i1-i10,i2+2-i20,i3-1-i30))+16*(rsxy(i1,i2-2,
     & i3-1,axis,0)*jac3di(i1-i10,i2-2-i20,i3-1-i30)))/(144.*dr(2)**2*
     & dr(1))
                     a12rss = (-240*(rsxy(i1,i2+1,i3,axis,1)*jac3di(i1-
     & i10,i2+1-i20,i3-i30))+240*(rsxy(i1,i2-1,i3,axis,1)*jac3di(i1-
     & i10,i2-1-i20,i3-i30))-8*(rsxy(i1,i2+1,i3+2,axis,1)*jac3di(i1-
     & i10,i2+1-i20,i3+2-i30))+8*(rsxy(i1,i2-1,i3+2,axis,1)*jac3di(i1-
     & i10,i2-1-i20,i3+2-i30))+(rsxy(i1,i2+2,i3+2,axis,1)*jac3di(i1-
     & i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axis,1)*jac3di(i1-
     & i10,i2-2-i20,i3+2-i30))-8*(rsxy(i1,i2+1,i3-2,axis,1)*jac3di(i1-
     & i10,i2+1-i20,i3-2-i30))+8*(rsxy(i1,i2-1,i3-2,axis,1)*jac3di(i1-
     & i10,i2-1-i20,i3-2-i30))+(rsxy(i1,i2+2,i3-2,axis,1)*jac3di(i1-
     & i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,i3-2,axis,1)*jac3di(i1-
     & i10,i2-2-i20,i3-2-i30))+30*(rsxy(i1,i2+2,i3,axis,1)*jac3di(i1-
     & i10,i2+2-i20,i3-i30))-30*(rsxy(i1,i2-2,i3,axis,1)*jac3di(i1-
     & i10,i2-2-i20,i3-i30))+128*(rsxy(i1,i2+1,i3+1,axis,1)*jac3di(i1-
     & i10,i2+1-i20,i3+1-i30))-128*(rsxy(i1,i2-1,i3+1,axis,1)*jac3di(
     & i1-i10,i2-1-i20,i3+1-i30))-16*(rsxy(i1,i2+2,i3+1,axis,1)*
     & jac3di(i1-i10,i2+2-i20,i3+1-i30))+16*(rsxy(i1,i2-2,i3+1,axis,1)
     & *jac3di(i1-i10,i2-2-i20,i3+1-i30))+128*(rsxy(i1,i2+1,i3-1,axis,
     & 1)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-128*(rsxy(i1,i2-1,i3-1,
     & axis,1)*jac3di(i1-i10,i2-1-i20,i3-1-i30))-16*(rsxy(i1,i2+2,i3-
     & 1,axis,1)*jac3di(i1-i10,i2+2-i20,i3-1-i30))+16*(rsxy(i1,i2-2,
     & i3-1,axis,1)*jac3di(i1-i10,i2-2-i20,i3-1-i30)))/(144.*dr(2)**2*
     & dr(1))
                     a13rss = (-240*(rsxy(i1,i2+1,i3,axis,2)*jac3di(i1-
     & i10,i2+1-i20,i3-i30))+240*(rsxy(i1,i2-1,i3,axis,2)*jac3di(i1-
     & i10,i2-1-i20,i3-i30))-8*(rsxy(i1,i2+1,i3+2,axis,2)*jac3di(i1-
     & i10,i2+1-i20,i3+2-i30))+8*(rsxy(i1,i2-1,i3+2,axis,2)*jac3di(i1-
     & i10,i2-1-i20,i3+2-i30))+(rsxy(i1,i2+2,i3+2,axis,2)*jac3di(i1-
     & i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axis,2)*jac3di(i1-
     & i10,i2-2-i20,i3+2-i30))-8*(rsxy(i1,i2+1,i3-2,axis,2)*jac3di(i1-
     & i10,i2+1-i20,i3-2-i30))+8*(rsxy(i1,i2-1,i3-2,axis,2)*jac3di(i1-
     & i10,i2-1-i20,i3-2-i30))+(rsxy(i1,i2+2,i3-2,axis,2)*jac3di(i1-
     & i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,i3-2,axis,2)*jac3di(i1-
     & i10,i2-2-i20,i3-2-i30))+30*(rsxy(i1,i2+2,i3,axis,2)*jac3di(i1-
     & i10,i2+2-i20,i3-i30))-30*(rsxy(i1,i2-2,i3,axis,2)*jac3di(i1-
     & i10,i2-2-i20,i3-i30))+128*(rsxy(i1,i2+1,i3+1,axis,2)*jac3di(i1-
     & i10,i2+1-i20,i3+1-i30))-128*(rsxy(i1,i2-1,i3+1,axis,2)*jac3di(
     & i1-i10,i2-1-i20,i3+1-i30))-16*(rsxy(i1,i2+2,i3+1,axis,2)*
     & jac3di(i1-i10,i2+2-i20,i3+1-i30))+16*(rsxy(i1,i2-2,i3+1,axis,2)
     & *jac3di(i1-i10,i2-2-i20,i3+1-i30))+128*(rsxy(i1,i2+1,i3-1,axis,
     & 2)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-128*(rsxy(i1,i2-1,i3-1,
     & axis,2)*jac3di(i1-i10,i2-1-i20,i3-1-i30))-16*(rsxy(i1,i2+2,i3-
     & 1,axis,2)*jac3di(i1-i10,i2+2-i20,i3-1-i30))+16*(rsxy(i1,i2-2,
     & i3-1,axis,2)*jac3di(i1-i10,i2-2-i20,i3-1-i30)))/(144.*dr(2)**2*
     & dr(1))
                     a21rss = (-240*(rsxy(i1,i2+1,i3,axisp1,0)*jac3di(
     & i1-i10,i2+1-i20,i3-i30))+240*(rsxy(i1,i2-1,i3,axisp1,0)*jac3di(
     & i1-i10,i2-1-i20,i3-i30))-8*(rsxy(i1,i2+1,i3+2,axisp1,0)*jac3di(
     & i1-i10,i2+1-i20,i3+2-i30))+8*(rsxy(i1,i2-1,i3+2,axisp1,0)*
     & jac3di(i1-i10,i2-1-i20,i3+2-i30))+(rsxy(i1,i2+2,i3+2,axisp1,0)*
     & jac3di(i1-i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp1,0)*
     & jac3di(i1-i10,i2-2-i20,i3+2-i30))-8*(rsxy(i1,i2+1,i3-2,axisp1,
     & 0)*jac3di(i1-i10,i2+1-i20,i3-2-i30))+8*(rsxy(i1,i2-1,i3-2,
     & axisp1,0)*jac3di(i1-i10,i2-1-i20,i3-2-i30))+(rsxy(i1,i2+2,i3-2,
     & axisp1,0)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,i3-2,
     & axisp1,0)*jac3di(i1-i10,i2-2-i20,i3-2-i30))+30*(rsxy(i1,i2+2,
     & i3,axisp1,0)*jac3di(i1-i10,i2+2-i20,i3-i30))-30*(rsxy(i1,i2-2,
     & i3,axisp1,0)*jac3di(i1-i10,i2-2-i20,i3-i30))+128*(rsxy(i1,i2+1,
     & i3+1,axisp1,0)*jac3di(i1-i10,i2+1-i20,i3+1-i30))-128*(rsxy(i1,
     & i2-1,i3+1,axisp1,0)*jac3di(i1-i10,i2-1-i20,i3+1-i30))-16*(rsxy(
     & i1,i2+2,i3+1,axisp1,0)*jac3di(i1-i10,i2+2-i20,i3+1-i30))+16*(
     & rsxy(i1,i2-2,i3+1,axisp1,0)*jac3di(i1-i10,i2-2-i20,i3+1-i30))+
     & 128*(rsxy(i1,i2+1,i3-1,axisp1,0)*jac3di(i1-i10,i2+1-i20,i3-1-
     & i30))-128*(rsxy(i1,i2-1,i3-1,axisp1,0)*jac3di(i1-i10,i2-1-i20,
     & i3-1-i30))-16*(rsxy(i1,i2+2,i3-1,axisp1,0)*jac3di(i1-i10,i2+2-
     & i20,i3-1-i30))+16*(rsxy(i1,i2-2,i3-1,axisp1,0)*jac3di(i1-i10,
     & i2-2-i20,i3-1-i30)))/(144.*dr(2)**2*dr(1))
                     a22rss = (-240*(rsxy(i1,i2+1,i3,axisp1,1)*jac3di(
     & i1-i10,i2+1-i20,i3-i30))+240*(rsxy(i1,i2-1,i3,axisp1,1)*jac3di(
     & i1-i10,i2-1-i20,i3-i30))-8*(rsxy(i1,i2+1,i3+2,axisp1,1)*jac3di(
     & i1-i10,i2+1-i20,i3+2-i30))+8*(rsxy(i1,i2-1,i3+2,axisp1,1)*
     & jac3di(i1-i10,i2-1-i20,i3+2-i30))+(rsxy(i1,i2+2,i3+2,axisp1,1)*
     & jac3di(i1-i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp1,1)*
     & jac3di(i1-i10,i2-2-i20,i3+2-i30))-8*(rsxy(i1,i2+1,i3-2,axisp1,
     & 1)*jac3di(i1-i10,i2+1-i20,i3-2-i30))+8*(rsxy(i1,i2-1,i3-2,
     & axisp1,1)*jac3di(i1-i10,i2-1-i20,i3-2-i30))+(rsxy(i1,i2+2,i3-2,
     & axisp1,1)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,i3-2,
     & axisp1,1)*jac3di(i1-i10,i2-2-i20,i3-2-i30))+30*(rsxy(i1,i2+2,
     & i3,axisp1,1)*jac3di(i1-i10,i2+2-i20,i3-i30))-30*(rsxy(i1,i2-2,
     & i3,axisp1,1)*jac3di(i1-i10,i2-2-i20,i3-i30))+128*(rsxy(i1,i2+1,
     & i3+1,axisp1,1)*jac3di(i1-i10,i2+1-i20,i3+1-i30))-128*(rsxy(i1,
     & i2-1,i3+1,axisp1,1)*jac3di(i1-i10,i2-1-i20,i3+1-i30))-16*(rsxy(
     & i1,i2+2,i3+1,axisp1,1)*jac3di(i1-i10,i2+2-i20,i3+1-i30))+16*(
     & rsxy(i1,i2-2,i3+1,axisp1,1)*jac3di(i1-i10,i2-2-i20,i3+1-i30))+
     & 128*(rsxy(i1,i2+1,i3-1,axisp1,1)*jac3di(i1-i10,i2+1-i20,i3-1-
     & i30))-128*(rsxy(i1,i2-1,i3-1,axisp1,1)*jac3di(i1-i10,i2-1-i20,
     & i3-1-i30))-16*(rsxy(i1,i2+2,i3-1,axisp1,1)*jac3di(i1-i10,i2+2-
     & i20,i3-1-i30))+16*(rsxy(i1,i2-2,i3-1,axisp1,1)*jac3di(i1-i10,
     & i2-2-i20,i3-1-i30)))/(144.*dr(2)**2*dr(1))
                     a23rss = (-240*(rsxy(i1,i2+1,i3,axisp1,2)*jac3di(
     & i1-i10,i2+1-i20,i3-i30))+240*(rsxy(i1,i2-1,i3,axisp1,2)*jac3di(
     & i1-i10,i2-1-i20,i3-i30))-8*(rsxy(i1,i2+1,i3+2,axisp1,2)*jac3di(
     & i1-i10,i2+1-i20,i3+2-i30))+8*(rsxy(i1,i2-1,i3+2,axisp1,2)*
     & jac3di(i1-i10,i2-1-i20,i3+2-i30))+(rsxy(i1,i2+2,i3+2,axisp1,2)*
     & jac3di(i1-i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp1,2)*
     & jac3di(i1-i10,i2-2-i20,i3+2-i30))-8*(rsxy(i1,i2+1,i3-2,axisp1,
     & 2)*jac3di(i1-i10,i2+1-i20,i3-2-i30))+8*(rsxy(i1,i2-1,i3-2,
     & axisp1,2)*jac3di(i1-i10,i2-1-i20,i3-2-i30))+(rsxy(i1,i2+2,i3-2,
     & axisp1,2)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,i3-2,
     & axisp1,2)*jac3di(i1-i10,i2-2-i20,i3-2-i30))+30*(rsxy(i1,i2+2,
     & i3,axisp1,2)*jac3di(i1-i10,i2+2-i20,i3-i30))-30*(rsxy(i1,i2-2,
     & i3,axisp1,2)*jac3di(i1-i10,i2-2-i20,i3-i30))+128*(rsxy(i1,i2+1,
     & i3+1,axisp1,2)*jac3di(i1-i10,i2+1-i20,i3+1-i30))-128*(rsxy(i1,
     & i2-1,i3+1,axisp1,2)*jac3di(i1-i10,i2-1-i20,i3+1-i30))-16*(rsxy(
     & i1,i2+2,i3+1,axisp1,2)*jac3di(i1-i10,i2+2-i20,i3+1-i30))+16*(
     & rsxy(i1,i2-2,i3+1,axisp1,2)*jac3di(i1-i10,i2-2-i20,i3+1-i30))+
     & 128*(rsxy(i1,i2+1,i3-1,axisp1,2)*jac3di(i1-i10,i2+1-i20,i3-1-
     & i30))-128*(rsxy(i1,i2-1,i3-1,axisp1,2)*jac3di(i1-i10,i2-1-i20,
     & i3-1-i30))-16*(rsxy(i1,i2+2,i3-1,axisp1,2)*jac3di(i1-i10,i2+2-
     & i20,i3-1-i30))+16*(rsxy(i1,i2-2,i3-1,axisp1,2)*jac3di(i1-i10,
     & i2-2-i20,i3-1-i30)))/(144.*dr(2)**2*dr(1))
                     a31rss = (-240*(rsxy(i1,i2+1,i3,axisp2,0)*jac3di(
     & i1-i10,i2+1-i20,i3-i30))+240*(rsxy(i1,i2-1,i3,axisp2,0)*jac3di(
     & i1-i10,i2-1-i20,i3-i30))-8*(rsxy(i1,i2+1,i3+2,axisp2,0)*jac3di(
     & i1-i10,i2+1-i20,i3+2-i30))+8*(rsxy(i1,i2-1,i3+2,axisp2,0)*
     & jac3di(i1-i10,i2-1-i20,i3+2-i30))+(rsxy(i1,i2+2,i3+2,axisp2,0)*
     & jac3di(i1-i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp2,0)*
     & jac3di(i1-i10,i2-2-i20,i3+2-i30))-8*(rsxy(i1,i2+1,i3-2,axisp2,
     & 0)*jac3di(i1-i10,i2+1-i20,i3-2-i30))+8*(rsxy(i1,i2-1,i3-2,
     & axisp2,0)*jac3di(i1-i10,i2-1-i20,i3-2-i30))+(rsxy(i1,i2+2,i3-2,
     & axisp2,0)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,i3-2,
     & axisp2,0)*jac3di(i1-i10,i2-2-i20,i3-2-i30))+30*(rsxy(i1,i2+2,
     & i3,axisp2,0)*jac3di(i1-i10,i2+2-i20,i3-i30))-30*(rsxy(i1,i2-2,
     & i3,axisp2,0)*jac3di(i1-i10,i2-2-i20,i3-i30))+128*(rsxy(i1,i2+1,
     & i3+1,axisp2,0)*jac3di(i1-i10,i2+1-i20,i3+1-i30))-128*(rsxy(i1,
     & i2-1,i3+1,axisp2,0)*jac3di(i1-i10,i2-1-i20,i3+1-i30))-16*(rsxy(
     & i1,i2+2,i3+1,axisp2,0)*jac3di(i1-i10,i2+2-i20,i3+1-i30))+16*(
     & rsxy(i1,i2-2,i3+1,axisp2,0)*jac3di(i1-i10,i2-2-i20,i3+1-i30))+
     & 128*(rsxy(i1,i2+1,i3-1,axisp2,0)*jac3di(i1-i10,i2+1-i20,i3-1-
     & i30))-128*(rsxy(i1,i2-1,i3-1,axisp2,0)*jac3di(i1-i10,i2-1-i20,
     & i3-1-i30))-16*(rsxy(i1,i2+2,i3-1,axisp2,0)*jac3di(i1-i10,i2+2-
     & i20,i3-1-i30))+16*(rsxy(i1,i2-2,i3-1,axisp2,0)*jac3di(i1-i10,
     & i2-2-i20,i3-1-i30)))/(144.*dr(2)**2*dr(1))
                     a32rss = (-240*(rsxy(i1,i2+1,i3,axisp2,1)*jac3di(
     & i1-i10,i2+1-i20,i3-i30))+240*(rsxy(i1,i2-1,i3,axisp2,1)*jac3di(
     & i1-i10,i2-1-i20,i3-i30))-8*(rsxy(i1,i2+1,i3+2,axisp2,1)*jac3di(
     & i1-i10,i2+1-i20,i3+2-i30))+8*(rsxy(i1,i2-1,i3+2,axisp2,1)*
     & jac3di(i1-i10,i2-1-i20,i3+2-i30))+(rsxy(i1,i2+2,i3+2,axisp2,1)*
     & jac3di(i1-i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp2,1)*
     & jac3di(i1-i10,i2-2-i20,i3+2-i30))-8*(rsxy(i1,i2+1,i3-2,axisp2,
     & 1)*jac3di(i1-i10,i2+1-i20,i3-2-i30))+8*(rsxy(i1,i2-1,i3-2,
     & axisp2,1)*jac3di(i1-i10,i2-1-i20,i3-2-i30))+(rsxy(i1,i2+2,i3-2,
     & axisp2,1)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,i3-2,
     & axisp2,1)*jac3di(i1-i10,i2-2-i20,i3-2-i30))+30*(rsxy(i1,i2+2,
     & i3,axisp2,1)*jac3di(i1-i10,i2+2-i20,i3-i30))-30*(rsxy(i1,i2-2,
     & i3,axisp2,1)*jac3di(i1-i10,i2-2-i20,i3-i30))+128*(rsxy(i1,i2+1,
     & i3+1,axisp2,1)*jac3di(i1-i10,i2+1-i20,i3+1-i30))-128*(rsxy(i1,
     & i2-1,i3+1,axisp2,1)*jac3di(i1-i10,i2-1-i20,i3+1-i30))-16*(rsxy(
     & i1,i2+2,i3+1,axisp2,1)*jac3di(i1-i10,i2+2-i20,i3+1-i30))+16*(
     & rsxy(i1,i2-2,i3+1,axisp2,1)*jac3di(i1-i10,i2-2-i20,i3+1-i30))+
     & 128*(rsxy(i1,i2+1,i3-1,axisp2,1)*jac3di(i1-i10,i2+1-i20,i3-1-
     & i30))-128*(rsxy(i1,i2-1,i3-1,axisp2,1)*jac3di(i1-i10,i2-1-i20,
     & i3-1-i30))-16*(rsxy(i1,i2+2,i3-1,axisp2,1)*jac3di(i1-i10,i2+2-
     & i20,i3-1-i30))+16*(rsxy(i1,i2-2,i3-1,axisp2,1)*jac3di(i1-i10,
     & i2-2-i20,i3-1-i30)))/(144.*dr(2)**2*dr(1))
                     a33rss = (-240*(rsxy(i1,i2+1,i3,axisp2,2)*jac3di(
     & i1-i10,i2+1-i20,i3-i30))+240*(rsxy(i1,i2-1,i3,axisp2,2)*jac3di(
     & i1-i10,i2-1-i20,i3-i30))-8*(rsxy(i1,i2+1,i3+2,axisp2,2)*jac3di(
     & i1-i10,i2+1-i20,i3+2-i30))+8*(rsxy(i1,i2-1,i3+2,axisp2,2)*
     & jac3di(i1-i10,i2-1-i20,i3+2-i30))+(rsxy(i1,i2+2,i3+2,axisp2,2)*
     & jac3di(i1-i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp2,2)*
     & jac3di(i1-i10,i2-2-i20,i3+2-i30))-8*(rsxy(i1,i2+1,i3-2,axisp2,
     & 2)*jac3di(i1-i10,i2+1-i20,i3-2-i30))+8*(rsxy(i1,i2-1,i3-2,
     & axisp2,2)*jac3di(i1-i10,i2-1-i20,i3-2-i30))+(rsxy(i1,i2+2,i3-2,
     & axisp2,2)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,i3-2,
     & axisp2,2)*jac3di(i1-i10,i2-2-i20,i3-2-i30))+30*(rsxy(i1,i2+2,
     & i3,axisp2,2)*jac3di(i1-i10,i2+2-i20,i3-i30))-30*(rsxy(i1,i2-2,
     & i3,axisp2,2)*jac3di(i1-i10,i2-2-i20,i3-i30))+128*(rsxy(i1,i2+1,
     & i3+1,axisp2,2)*jac3di(i1-i10,i2+1-i20,i3+1-i30))-128*(rsxy(i1,
     & i2-1,i3+1,axisp2,2)*jac3di(i1-i10,i2-1-i20,i3+1-i30))-16*(rsxy(
     & i1,i2+2,i3+1,axisp2,2)*jac3di(i1-i10,i2+2-i20,i3+1-i30))+16*(
     & rsxy(i1,i2-2,i3+1,axisp2,2)*jac3di(i1-i10,i2-2-i20,i3+1-i30))+
     & 128*(rsxy(i1,i2+1,i3-1,axisp2,2)*jac3di(i1-i10,i2+1-i20,i3-1-
     & i30))-128*(rsxy(i1,i2-1,i3-1,axisp2,2)*jac3di(i1-i10,i2-1-i20,
     & i3-1-i30))-16*(rsxy(i1,i2+2,i3-1,axisp2,2)*jac3di(i1-i10,i2+2-
     & i20,i3-1-i30))+16*(rsxy(i1,i2-2,i3-1,axisp2,2)*jac3di(i1-i10,
     & i2-2-i20,i3-1-i30)))/(144.*dr(2)**2*dr(1))
                     a11rtt = (128*(rsxy(i1+1,i2+1,i3,axis,0)*jac3di(
     & i1+1-i10,i2+1-i20,i3-i30))-240*(rsxy(i1,i2+1,i3,axis,0)*jac3di(
     & i1-i10,i2+1-i20,i3-i30))+128*(rsxy(i1-1,i2+1,i3,axis,0)*jac3di(
     & i1-1-i10,i2+1-i20,i3-i30))-8*(rsxy(i1+2,i2+1,i3,axis,0)*jac3di(
     & i1+2-i10,i2+1-i20,i3-i30))-8*(rsxy(i1-2,i2+1,i3,axis,0)*jac3di(
     & i1-2-i10,i2+1-i20,i3-i30))-128*(rsxy(i1+1,i2-1,i3,axis,0)*
     & jac3di(i1+1-i10,i2-1-i20,i3-i30))+240*(rsxy(i1,i2-1,i3,axis,0)*
     & jac3di(i1-i10,i2-1-i20,i3-i30))-128*(rsxy(i1-1,i2-1,i3,axis,0)*
     & jac3di(i1-1-i10,i2-1-i20,i3-i30))+8*(rsxy(i1+2,i2-1,i3,axis,0)*
     & jac3di(i1+2-i10,i2-1-i20,i3-i30))+8*(rsxy(i1-2,i2-1,i3,axis,0)*
     & jac3di(i1-2-i10,i2-1-i20,i3-i30))+30*(rsxy(i1,i2+2,i3,axis,0)*
     & jac3di(i1-i10,i2+2-i20,i3-i30))-16*(rsxy(i1-1,i2+2,i3,axis,0)*
     & jac3di(i1-1-i10,i2+2-i20,i3-i30))+(rsxy(i1+2,i2+2,i3,axis,0)*
     & jac3di(i1+2-i10,i2+2-i20,i3-i30))+(rsxy(i1-2,i2+2,i3,axis,0)*
     & jac3di(i1-2-i10,i2+2-i20,i3-i30))+16*(rsxy(i1+1,i2-2,i3,axis,0)
     & *jac3di(i1+1-i10,i2-2-i20,i3-i30))-30*(rsxy(i1,i2-2,i3,axis,0)*
     & jac3di(i1-i10,i2-2-i20,i3-i30))+16*(rsxy(i1-1,i2-2,i3,axis,0)*
     & jac3di(i1-1-i10,i2-2-i20,i3-i30))-(rsxy(i1+2,i2-2,i3,axis,0)*
     & jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-2,i3,axis,0)*
     & jac3di(i1-2-i10,i2-2-i20,i3-i30))-16*(rsxy(i1+1,i2+2,i3,axis,0)
     & *jac3di(i1+1-i10,i2+2-i20,i3-i30)))/(144.*dr(0)**2*dr(1))
                     a12rtt = (128*(rsxy(i1+1,i2+1,i3,axis,1)*jac3di(
     & i1+1-i10,i2+1-i20,i3-i30))-240*(rsxy(i1,i2+1,i3,axis,1)*jac3di(
     & i1-i10,i2+1-i20,i3-i30))+128*(rsxy(i1-1,i2+1,i3,axis,1)*jac3di(
     & i1-1-i10,i2+1-i20,i3-i30))-8*(rsxy(i1+2,i2+1,i3,axis,1)*jac3di(
     & i1+2-i10,i2+1-i20,i3-i30))-8*(rsxy(i1-2,i2+1,i3,axis,1)*jac3di(
     & i1-2-i10,i2+1-i20,i3-i30))-128*(rsxy(i1+1,i2-1,i3,axis,1)*
     & jac3di(i1+1-i10,i2-1-i20,i3-i30))+240*(rsxy(i1,i2-1,i3,axis,1)*
     & jac3di(i1-i10,i2-1-i20,i3-i30))-128*(rsxy(i1-1,i2-1,i3,axis,1)*
     & jac3di(i1-1-i10,i2-1-i20,i3-i30))+8*(rsxy(i1+2,i2-1,i3,axis,1)*
     & jac3di(i1+2-i10,i2-1-i20,i3-i30))+8*(rsxy(i1-2,i2-1,i3,axis,1)*
     & jac3di(i1-2-i10,i2-1-i20,i3-i30))+30*(rsxy(i1,i2+2,i3,axis,1)*
     & jac3di(i1-i10,i2+2-i20,i3-i30))-16*(rsxy(i1-1,i2+2,i3,axis,1)*
     & jac3di(i1-1-i10,i2+2-i20,i3-i30))+(rsxy(i1+2,i2+2,i3,axis,1)*
     & jac3di(i1+2-i10,i2+2-i20,i3-i30))+(rsxy(i1-2,i2+2,i3,axis,1)*
     & jac3di(i1-2-i10,i2+2-i20,i3-i30))+16*(rsxy(i1+1,i2-2,i3,axis,1)
     & *jac3di(i1+1-i10,i2-2-i20,i3-i30))-30*(rsxy(i1,i2-2,i3,axis,1)*
     & jac3di(i1-i10,i2-2-i20,i3-i30))+16*(rsxy(i1-1,i2-2,i3,axis,1)*
     & jac3di(i1-1-i10,i2-2-i20,i3-i30))-(rsxy(i1+2,i2-2,i3,axis,1)*
     & jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-2,i3,axis,1)*
     & jac3di(i1-2-i10,i2-2-i20,i3-i30))-16*(rsxy(i1+1,i2+2,i3,axis,1)
     & *jac3di(i1+1-i10,i2+2-i20,i3-i30)))/(144.*dr(0)**2*dr(1))
                     a13rtt = (128*(rsxy(i1+1,i2+1,i3,axis,2)*jac3di(
     & i1+1-i10,i2+1-i20,i3-i30))-240*(rsxy(i1,i2+1,i3,axis,2)*jac3di(
     & i1-i10,i2+1-i20,i3-i30))+128*(rsxy(i1-1,i2+1,i3,axis,2)*jac3di(
     & i1-1-i10,i2+1-i20,i3-i30))-8*(rsxy(i1+2,i2+1,i3,axis,2)*jac3di(
     & i1+2-i10,i2+1-i20,i3-i30))-8*(rsxy(i1-2,i2+1,i3,axis,2)*jac3di(
     & i1-2-i10,i2+1-i20,i3-i30))-128*(rsxy(i1+1,i2-1,i3,axis,2)*
     & jac3di(i1+1-i10,i2-1-i20,i3-i30))+240*(rsxy(i1,i2-1,i3,axis,2)*
     & jac3di(i1-i10,i2-1-i20,i3-i30))-128*(rsxy(i1-1,i2-1,i3,axis,2)*
     & jac3di(i1-1-i10,i2-1-i20,i3-i30))+8*(rsxy(i1+2,i2-1,i3,axis,2)*
     & jac3di(i1+2-i10,i2-1-i20,i3-i30))+8*(rsxy(i1-2,i2-1,i3,axis,2)*
     & jac3di(i1-2-i10,i2-1-i20,i3-i30))+30*(rsxy(i1,i2+2,i3,axis,2)*
     & jac3di(i1-i10,i2+2-i20,i3-i30))-16*(rsxy(i1-1,i2+2,i3,axis,2)*
     & jac3di(i1-1-i10,i2+2-i20,i3-i30))+(rsxy(i1+2,i2+2,i3,axis,2)*
     & jac3di(i1+2-i10,i2+2-i20,i3-i30))+(rsxy(i1-2,i2+2,i3,axis,2)*
     & jac3di(i1-2-i10,i2+2-i20,i3-i30))+16*(rsxy(i1+1,i2-2,i3,axis,2)
     & *jac3di(i1+1-i10,i2-2-i20,i3-i30))-30*(rsxy(i1,i2-2,i3,axis,2)*
     & jac3di(i1-i10,i2-2-i20,i3-i30))+16*(rsxy(i1-1,i2-2,i3,axis,2)*
     & jac3di(i1-1-i10,i2-2-i20,i3-i30))-(rsxy(i1+2,i2-2,i3,axis,2)*
     & jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-2,i3,axis,2)*
     & jac3di(i1-2-i10,i2-2-i20,i3-i30))-16*(rsxy(i1+1,i2+2,i3,axis,2)
     & *jac3di(i1+1-i10,i2+2-i20,i3-i30)))/(144.*dr(0)**2*dr(1))
                     a21rtt = (128*(rsxy(i1+1,i2+1,i3,axisp1,0)*jac3di(
     & i1+1-i10,i2+1-i20,i3-i30))-240*(rsxy(i1,i2+1,i3,axisp1,0)*
     & jac3di(i1-i10,i2+1-i20,i3-i30))+128*(rsxy(i1-1,i2+1,i3,axisp1,
     & 0)*jac3di(i1-1-i10,i2+1-i20,i3-i30))-8*(rsxy(i1+2,i2+1,i3,
     & axisp1,0)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-8*(rsxy(i1-2,i2+1,
     & i3,axisp1,0)*jac3di(i1-2-i10,i2+1-i20,i3-i30))-128*(rsxy(i1+1,
     & i2-1,i3,axisp1,0)*jac3di(i1+1-i10,i2-1-i20,i3-i30))+240*(rsxy(
     & i1,i2-1,i3,axisp1,0)*jac3di(i1-i10,i2-1-i20,i3-i30))-128*(rsxy(
     & i1-1,i2-1,i3,axisp1,0)*jac3di(i1-1-i10,i2-1-i20,i3-i30))+8*(
     & rsxy(i1+2,i2-1,i3,axisp1,0)*jac3di(i1+2-i10,i2-1-i20,i3-i30))+
     & 8*(rsxy(i1-2,i2-1,i3,axisp1,0)*jac3di(i1-2-i10,i2-1-i20,i3-i30)
     & )+30*(rsxy(i1,i2+2,i3,axisp1,0)*jac3di(i1-i10,i2+2-i20,i3-i30))
     & -16*(rsxy(i1-1,i2+2,i3,axisp1,0)*jac3di(i1-1-i10,i2+2-i20,i3-
     & i30))+(rsxy(i1+2,i2+2,i3,axisp1,0)*jac3di(i1+2-i10,i2+2-i20,i3-
     & i30))+(rsxy(i1-2,i2+2,i3,axisp1,0)*jac3di(i1-2-i10,i2+2-i20,i3-
     & i30))+16*(rsxy(i1+1,i2-2,i3,axisp1,0)*jac3di(i1+1-i10,i2-2-i20,
     & i3-i30))-30*(rsxy(i1,i2-2,i3,axisp1,0)*jac3di(i1-i10,i2-2-i20,
     & i3-i30))+16*(rsxy(i1-1,i2-2,i3,axisp1,0)*jac3di(i1-1-i10,i2-2-
     & i20,i3-i30))-(rsxy(i1+2,i2-2,i3,axisp1,0)*jac3di(i1+2-i10,i2-2-
     & i20,i3-i30))-(rsxy(i1-2,i2-2,i3,axisp1,0)*jac3di(i1-2-i10,i2-2-
     & i20,i3-i30))-16*(rsxy(i1+1,i2+2,i3,axisp1,0)*jac3di(i1+1-i10,
     & i2+2-i20,i3-i30)))/(144.*dr(0)**2*dr(1))
                     a22rtt = (128*(rsxy(i1+1,i2+1,i3,axisp1,1)*jac3di(
     & i1+1-i10,i2+1-i20,i3-i30))-240*(rsxy(i1,i2+1,i3,axisp1,1)*
     & jac3di(i1-i10,i2+1-i20,i3-i30))+128*(rsxy(i1-1,i2+1,i3,axisp1,
     & 1)*jac3di(i1-1-i10,i2+1-i20,i3-i30))-8*(rsxy(i1+2,i2+1,i3,
     & axisp1,1)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-8*(rsxy(i1-2,i2+1,
     & i3,axisp1,1)*jac3di(i1-2-i10,i2+1-i20,i3-i30))-128*(rsxy(i1+1,
     & i2-1,i3,axisp1,1)*jac3di(i1+1-i10,i2-1-i20,i3-i30))+240*(rsxy(
     & i1,i2-1,i3,axisp1,1)*jac3di(i1-i10,i2-1-i20,i3-i30))-128*(rsxy(
     & i1-1,i2-1,i3,axisp1,1)*jac3di(i1-1-i10,i2-1-i20,i3-i30))+8*(
     & rsxy(i1+2,i2-1,i3,axisp1,1)*jac3di(i1+2-i10,i2-1-i20,i3-i30))+
     & 8*(rsxy(i1-2,i2-1,i3,axisp1,1)*jac3di(i1-2-i10,i2-1-i20,i3-i30)
     & )+30*(rsxy(i1,i2+2,i3,axisp1,1)*jac3di(i1-i10,i2+2-i20,i3-i30))
     & -16*(rsxy(i1-1,i2+2,i3,axisp1,1)*jac3di(i1-1-i10,i2+2-i20,i3-
     & i30))+(rsxy(i1+2,i2+2,i3,axisp1,1)*jac3di(i1+2-i10,i2+2-i20,i3-
     & i30))+(rsxy(i1-2,i2+2,i3,axisp1,1)*jac3di(i1-2-i10,i2+2-i20,i3-
     & i30))+16*(rsxy(i1+1,i2-2,i3,axisp1,1)*jac3di(i1+1-i10,i2-2-i20,
     & i3-i30))-30*(rsxy(i1,i2-2,i3,axisp1,1)*jac3di(i1-i10,i2-2-i20,
     & i3-i30))+16*(rsxy(i1-1,i2-2,i3,axisp1,1)*jac3di(i1-1-i10,i2-2-
     & i20,i3-i30))-(rsxy(i1+2,i2-2,i3,axisp1,1)*jac3di(i1+2-i10,i2-2-
     & i20,i3-i30))-(rsxy(i1-2,i2-2,i3,axisp1,1)*jac3di(i1-2-i10,i2-2-
     & i20,i3-i30))-16*(rsxy(i1+1,i2+2,i3,axisp1,1)*jac3di(i1+1-i10,
     & i2+2-i20,i3-i30)))/(144.*dr(0)**2*dr(1))
                     a23rtt = (128*(rsxy(i1+1,i2+1,i3,axisp1,2)*jac3di(
     & i1+1-i10,i2+1-i20,i3-i30))-240*(rsxy(i1,i2+1,i3,axisp1,2)*
     & jac3di(i1-i10,i2+1-i20,i3-i30))+128*(rsxy(i1-1,i2+1,i3,axisp1,
     & 2)*jac3di(i1-1-i10,i2+1-i20,i3-i30))-8*(rsxy(i1+2,i2+1,i3,
     & axisp1,2)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-8*(rsxy(i1-2,i2+1,
     & i3,axisp1,2)*jac3di(i1-2-i10,i2+1-i20,i3-i30))-128*(rsxy(i1+1,
     & i2-1,i3,axisp1,2)*jac3di(i1+1-i10,i2-1-i20,i3-i30))+240*(rsxy(
     & i1,i2-1,i3,axisp1,2)*jac3di(i1-i10,i2-1-i20,i3-i30))-128*(rsxy(
     & i1-1,i2-1,i3,axisp1,2)*jac3di(i1-1-i10,i2-1-i20,i3-i30))+8*(
     & rsxy(i1+2,i2-1,i3,axisp1,2)*jac3di(i1+2-i10,i2-1-i20,i3-i30))+
     & 8*(rsxy(i1-2,i2-1,i3,axisp1,2)*jac3di(i1-2-i10,i2-1-i20,i3-i30)
     & )+30*(rsxy(i1,i2+2,i3,axisp1,2)*jac3di(i1-i10,i2+2-i20,i3-i30))
     & -16*(rsxy(i1-1,i2+2,i3,axisp1,2)*jac3di(i1-1-i10,i2+2-i20,i3-
     & i30))+(rsxy(i1+2,i2+2,i3,axisp1,2)*jac3di(i1+2-i10,i2+2-i20,i3-
     & i30))+(rsxy(i1-2,i2+2,i3,axisp1,2)*jac3di(i1-2-i10,i2+2-i20,i3-
     & i30))+16*(rsxy(i1+1,i2-2,i3,axisp1,2)*jac3di(i1+1-i10,i2-2-i20,
     & i3-i30))-30*(rsxy(i1,i2-2,i3,axisp1,2)*jac3di(i1-i10,i2-2-i20,
     & i3-i30))+16*(rsxy(i1-1,i2-2,i3,axisp1,2)*jac3di(i1-1-i10,i2-2-
     & i20,i3-i30))-(rsxy(i1+2,i2-2,i3,axisp1,2)*jac3di(i1+2-i10,i2-2-
     & i20,i3-i30))-(rsxy(i1-2,i2-2,i3,axisp1,2)*jac3di(i1-2-i10,i2-2-
     & i20,i3-i30))-16*(rsxy(i1+1,i2+2,i3,axisp1,2)*jac3di(i1+1-i10,
     & i2+2-i20,i3-i30)))/(144.*dr(0)**2*dr(1))
                     a31rtt = (128*(rsxy(i1+1,i2+1,i3,axisp2,0)*jac3di(
     & i1+1-i10,i2+1-i20,i3-i30))-240*(rsxy(i1,i2+1,i3,axisp2,0)*
     & jac3di(i1-i10,i2+1-i20,i3-i30))+128*(rsxy(i1-1,i2+1,i3,axisp2,
     & 0)*jac3di(i1-1-i10,i2+1-i20,i3-i30))-8*(rsxy(i1+2,i2+1,i3,
     & axisp2,0)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-8*(rsxy(i1-2,i2+1,
     & i3,axisp2,0)*jac3di(i1-2-i10,i2+1-i20,i3-i30))-128*(rsxy(i1+1,
     & i2-1,i3,axisp2,0)*jac3di(i1+1-i10,i2-1-i20,i3-i30))+240*(rsxy(
     & i1,i2-1,i3,axisp2,0)*jac3di(i1-i10,i2-1-i20,i3-i30))-128*(rsxy(
     & i1-1,i2-1,i3,axisp2,0)*jac3di(i1-1-i10,i2-1-i20,i3-i30))+8*(
     & rsxy(i1+2,i2-1,i3,axisp2,0)*jac3di(i1+2-i10,i2-1-i20,i3-i30))+
     & 8*(rsxy(i1-2,i2-1,i3,axisp2,0)*jac3di(i1-2-i10,i2-1-i20,i3-i30)
     & )+30*(rsxy(i1,i2+2,i3,axisp2,0)*jac3di(i1-i10,i2+2-i20,i3-i30))
     & -16*(rsxy(i1-1,i2+2,i3,axisp2,0)*jac3di(i1-1-i10,i2+2-i20,i3-
     & i30))+(rsxy(i1+2,i2+2,i3,axisp2,0)*jac3di(i1+2-i10,i2+2-i20,i3-
     & i30))+(rsxy(i1-2,i2+2,i3,axisp2,0)*jac3di(i1-2-i10,i2+2-i20,i3-
     & i30))+16*(rsxy(i1+1,i2-2,i3,axisp2,0)*jac3di(i1+1-i10,i2-2-i20,
     & i3-i30))-30*(rsxy(i1,i2-2,i3,axisp2,0)*jac3di(i1-i10,i2-2-i20,
     & i3-i30))+16*(rsxy(i1-1,i2-2,i3,axisp2,0)*jac3di(i1-1-i10,i2-2-
     & i20,i3-i30))-(rsxy(i1+2,i2-2,i3,axisp2,0)*jac3di(i1+2-i10,i2-2-
     & i20,i3-i30))-(rsxy(i1-2,i2-2,i3,axisp2,0)*jac3di(i1-2-i10,i2-2-
     & i20,i3-i30))-16*(rsxy(i1+1,i2+2,i3,axisp2,0)*jac3di(i1+1-i10,
     & i2+2-i20,i3-i30)))/(144.*dr(0)**2*dr(1))
                     a32rtt = (128*(rsxy(i1+1,i2+1,i3,axisp2,1)*jac3di(
     & i1+1-i10,i2+1-i20,i3-i30))-240*(rsxy(i1,i2+1,i3,axisp2,1)*
     & jac3di(i1-i10,i2+1-i20,i3-i30))+128*(rsxy(i1-1,i2+1,i3,axisp2,
     & 1)*jac3di(i1-1-i10,i2+1-i20,i3-i30))-8*(rsxy(i1+2,i2+1,i3,
     & axisp2,1)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-8*(rsxy(i1-2,i2+1,
     & i3,axisp2,1)*jac3di(i1-2-i10,i2+1-i20,i3-i30))-128*(rsxy(i1+1,
     & i2-1,i3,axisp2,1)*jac3di(i1+1-i10,i2-1-i20,i3-i30))+240*(rsxy(
     & i1,i2-1,i3,axisp2,1)*jac3di(i1-i10,i2-1-i20,i3-i30))-128*(rsxy(
     & i1-1,i2-1,i3,axisp2,1)*jac3di(i1-1-i10,i2-1-i20,i3-i30))+8*(
     & rsxy(i1+2,i2-1,i3,axisp2,1)*jac3di(i1+2-i10,i2-1-i20,i3-i30))+
     & 8*(rsxy(i1-2,i2-1,i3,axisp2,1)*jac3di(i1-2-i10,i2-1-i20,i3-i30)
     & )+30*(rsxy(i1,i2+2,i3,axisp2,1)*jac3di(i1-i10,i2+2-i20,i3-i30))
     & -16*(rsxy(i1-1,i2+2,i3,axisp2,1)*jac3di(i1-1-i10,i2+2-i20,i3-
     & i30))+(rsxy(i1+2,i2+2,i3,axisp2,1)*jac3di(i1+2-i10,i2+2-i20,i3-
     & i30))+(rsxy(i1-2,i2+2,i3,axisp2,1)*jac3di(i1-2-i10,i2+2-i20,i3-
     & i30))+16*(rsxy(i1+1,i2-2,i3,axisp2,1)*jac3di(i1+1-i10,i2-2-i20,
     & i3-i30))-30*(rsxy(i1,i2-2,i3,axisp2,1)*jac3di(i1-i10,i2-2-i20,
     & i3-i30))+16*(rsxy(i1-1,i2-2,i3,axisp2,1)*jac3di(i1-1-i10,i2-2-
     & i20,i3-i30))-(rsxy(i1+2,i2-2,i3,axisp2,1)*jac3di(i1+2-i10,i2-2-
     & i20,i3-i30))-(rsxy(i1-2,i2-2,i3,axisp2,1)*jac3di(i1-2-i10,i2-2-
     & i20,i3-i30))-16*(rsxy(i1+1,i2+2,i3,axisp2,1)*jac3di(i1+1-i10,
     & i2+2-i20,i3-i30)))/(144.*dr(0)**2*dr(1))
                     a33rtt = (128*(rsxy(i1+1,i2+1,i3,axisp2,2)*jac3di(
     & i1+1-i10,i2+1-i20,i3-i30))-240*(rsxy(i1,i2+1,i3,axisp2,2)*
     & jac3di(i1-i10,i2+1-i20,i3-i30))+128*(rsxy(i1-1,i2+1,i3,axisp2,
     & 2)*jac3di(i1-1-i10,i2+1-i20,i3-i30))-8*(rsxy(i1+2,i2+1,i3,
     & axisp2,2)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-8*(rsxy(i1-2,i2+1,
     & i3,axisp2,2)*jac3di(i1-2-i10,i2+1-i20,i3-i30))-128*(rsxy(i1+1,
     & i2-1,i3,axisp2,2)*jac3di(i1+1-i10,i2-1-i20,i3-i30))+240*(rsxy(
     & i1,i2-1,i3,axisp2,2)*jac3di(i1-i10,i2-1-i20,i3-i30))-128*(rsxy(
     & i1-1,i2-1,i3,axisp2,2)*jac3di(i1-1-i10,i2-1-i20,i3-i30))+8*(
     & rsxy(i1+2,i2-1,i3,axisp2,2)*jac3di(i1+2-i10,i2-1-i20,i3-i30))+
     & 8*(rsxy(i1-2,i2-1,i3,axisp2,2)*jac3di(i1-2-i10,i2-1-i20,i3-i30)
     & )+30*(rsxy(i1,i2+2,i3,axisp2,2)*jac3di(i1-i10,i2+2-i20,i3-i30))
     & -16*(rsxy(i1-1,i2+2,i3,axisp2,2)*jac3di(i1-1-i10,i2+2-i20,i3-
     & i30))+(rsxy(i1+2,i2+2,i3,axisp2,2)*jac3di(i1+2-i10,i2+2-i20,i3-
     & i30))+(rsxy(i1-2,i2+2,i3,axisp2,2)*jac3di(i1-2-i10,i2+2-i20,i3-
     & i30))+16*(rsxy(i1+1,i2-2,i3,axisp2,2)*jac3di(i1+1-i10,i2-2-i20,
     & i3-i30))-30*(rsxy(i1,i2-2,i3,axisp2,2)*jac3di(i1-i10,i2-2-i20,
     & i3-i30))+16*(rsxy(i1-1,i2-2,i3,axisp2,2)*jac3di(i1-1-i10,i2-2-
     & i20,i3-i30))-(rsxy(i1+2,i2-2,i3,axisp2,2)*jac3di(i1+2-i10,i2-2-
     & i20,i3-i30))-(rsxy(i1-2,i2-2,i3,axisp2,2)*jac3di(i1-2-i10,i2-2-
     & i20,i3-i30))-16*(rsxy(i1+1,i2+2,i3,axisp2,2)*jac3di(i1+1-i10,
     & i2+2-i20,i3-i30)))/(144.*dr(0)**2*dr(1))
                     a11stt = (128*(rsxy(i1+1,i2,i3+1,axis,0)*jac3di(
     & i1+1-i10,i2-i20,i3+1-i30))+240*(rsxy(i1,i2,i3-1,axis,0)*jac3di(
     & i1-i10,i2-i20,i3-1-i30))+8*(rsxy(i1+2,i2,i3-1,axis,0)*jac3di(
     & i1+2-i10,i2-i20,i3-1-i30))+8*(rsxy(i1-2,i2,i3-1,axis,0)*jac3di(
     & i1-2-i10,i2-i20,i3-1-i30))-128*(rsxy(i1-1,i2,i3-1,axis,0)*
     & jac3di(i1-1-i10,i2-i20,i3-1-i30))-240*(rsxy(i1,i2,i3+1,axis,0)*
     & jac3di(i1-i10,i2-i20,i3+1-i30))-8*(rsxy(i1+2,i2,i3+1,axis,0)*
     & jac3di(i1+2-i10,i2-i20,i3+1-i30))-8*(rsxy(i1-2,i2,i3+1,axis,0)*
     & jac3di(i1-2-i10,i2-i20,i3+1-i30))-128*(rsxy(i1+1,i2,i3-1,axis,
     & 0)*jac3di(i1+1-i10,i2-i20,i3-1-i30))+128*(rsxy(i1-1,i2,i3+1,
     & axis,0)*jac3di(i1-1-i10,i2-i20,i3+1-i30))-16*(rsxy(i1+1,i2,i3+
     & 2,axis,0)*jac3di(i1+1-i10,i2-i20,i3+2-i30))+30*(rsxy(i1,i2,i3+
     & 2,axis,0)*jac3di(i1-i10,i2-i20,i3+2-i30))-16*(rsxy(i1-1,i2,i3+
     & 2,axis,0)*jac3di(i1-1-i10,i2-i20,i3+2-i30))+(rsxy(i1+2,i2,i3+2,
     & axis,0)*jac3di(i1+2-i10,i2-i20,i3+2-i30))+(rsxy(i1-2,i2,i3+2,
     & axis,0)*jac3di(i1-2-i10,i2-i20,i3+2-i30))+16*(rsxy(i1+1,i2,i3-
     & 2,axis,0)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-30*(rsxy(i1,i2,i3-
     & 2,axis,0)*jac3di(i1-i10,i2-i20,i3-2-i30))+16*(rsxy(i1-1,i2,i3-
     & 2,axis,0)*jac3di(i1-1-i10,i2-i20,i3-2-i30))-(rsxy(i1+2,i2,i3-2,
     & axis,0)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,i3-2,
     & axis,0)*jac3di(i1-2-i10,i2-i20,i3-2-i30)))/(144.*dr(0)**2*dr(2)
     & )
                     a12stt = (128*(rsxy(i1+1,i2,i3+1,axis,1)*jac3di(
     & i1+1-i10,i2-i20,i3+1-i30))+240*(rsxy(i1,i2,i3-1,axis,1)*jac3di(
     & i1-i10,i2-i20,i3-1-i30))+8*(rsxy(i1+2,i2,i3-1,axis,1)*jac3di(
     & i1+2-i10,i2-i20,i3-1-i30))+8*(rsxy(i1-2,i2,i3-1,axis,1)*jac3di(
     & i1-2-i10,i2-i20,i3-1-i30))-128*(rsxy(i1-1,i2,i3-1,axis,1)*
     & jac3di(i1-1-i10,i2-i20,i3-1-i30))-240*(rsxy(i1,i2,i3+1,axis,1)*
     & jac3di(i1-i10,i2-i20,i3+1-i30))-8*(rsxy(i1+2,i2,i3+1,axis,1)*
     & jac3di(i1+2-i10,i2-i20,i3+1-i30))-8*(rsxy(i1-2,i2,i3+1,axis,1)*
     & jac3di(i1-2-i10,i2-i20,i3+1-i30))-128*(rsxy(i1+1,i2,i3-1,axis,
     & 1)*jac3di(i1+1-i10,i2-i20,i3-1-i30))+128*(rsxy(i1-1,i2,i3+1,
     & axis,1)*jac3di(i1-1-i10,i2-i20,i3+1-i30))-16*(rsxy(i1+1,i2,i3+
     & 2,axis,1)*jac3di(i1+1-i10,i2-i20,i3+2-i30))+30*(rsxy(i1,i2,i3+
     & 2,axis,1)*jac3di(i1-i10,i2-i20,i3+2-i30))-16*(rsxy(i1-1,i2,i3+
     & 2,axis,1)*jac3di(i1-1-i10,i2-i20,i3+2-i30))+(rsxy(i1+2,i2,i3+2,
     & axis,1)*jac3di(i1+2-i10,i2-i20,i3+2-i30))+(rsxy(i1-2,i2,i3+2,
     & axis,1)*jac3di(i1-2-i10,i2-i20,i3+2-i30))+16*(rsxy(i1+1,i2,i3-
     & 2,axis,1)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-30*(rsxy(i1,i2,i3-
     & 2,axis,1)*jac3di(i1-i10,i2-i20,i3-2-i30))+16*(rsxy(i1-1,i2,i3-
     & 2,axis,1)*jac3di(i1-1-i10,i2-i20,i3-2-i30))-(rsxy(i1+2,i2,i3-2,
     & axis,1)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,i3-2,
     & axis,1)*jac3di(i1-2-i10,i2-i20,i3-2-i30)))/(144.*dr(0)**2*dr(2)
     & )
                     a13stt = (128*(rsxy(i1+1,i2,i3+1,axis,2)*jac3di(
     & i1+1-i10,i2-i20,i3+1-i30))+240*(rsxy(i1,i2,i3-1,axis,2)*jac3di(
     & i1-i10,i2-i20,i3-1-i30))+8*(rsxy(i1+2,i2,i3-1,axis,2)*jac3di(
     & i1+2-i10,i2-i20,i3-1-i30))+8*(rsxy(i1-2,i2,i3-1,axis,2)*jac3di(
     & i1-2-i10,i2-i20,i3-1-i30))-128*(rsxy(i1-1,i2,i3-1,axis,2)*
     & jac3di(i1-1-i10,i2-i20,i3-1-i30))-240*(rsxy(i1,i2,i3+1,axis,2)*
     & jac3di(i1-i10,i2-i20,i3+1-i30))-8*(rsxy(i1+2,i2,i3+1,axis,2)*
     & jac3di(i1+2-i10,i2-i20,i3+1-i30))-8*(rsxy(i1-2,i2,i3+1,axis,2)*
     & jac3di(i1-2-i10,i2-i20,i3+1-i30))-128*(rsxy(i1+1,i2,i3-1,axis,
     & 2)*jac3di(i1+1-i10,i2-i20,i3-1-i30))+128*(rsxy(i1-1,i2,i3+1,
     & axis,2)*jac3di(i1-1-i10,i2-i20,i3+1-i30))-16*(rsxy(i1+1,i2,i3+
     & 2,axis,2)*jac3di(i1+1-i10,i2-i20,i3+2-i30))+30*(rsxy(i1,i2,i3+
     & 2,axis,2)*jac3di(i1-i10,i2-i20,i3+2-i30))-16*(rsxy(i1-1,i2,i3+
     & 2,axis,2)*jac3di(i1-1-i10,i2-i20,i3+2-i30))+(rsxy(i1+2,i2,i3+2,
     & axis,2)*jac3di(i1+2-i10,i2-i20,i3+2-i30))+(rsxy(i1-2,i2,i3+2,
     & axis,2)*jac3di(i1-2-i10,i2-i20,i3+2-i30))+16*(rsxy(i1+1,i2,i3-
     & 2,axis,2)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-30*(rsxy(i1,i2,i3-
     & 2,axis,2)*jac3di(i1-i10,i2-i20,i3-2-i30))+16*(rsxy(i1-1,i2,i3-
     & 2,axis,2)*jac3di(i1-1-i10,i2-i20,i3-2-i30))-(rsxy(i1+2,i2,i3-2,
     & axis,2)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,i3-2,
     & axis,2)*jac3di(i1-2-i10,i2-i20,i3-2-i30)))/(144.*dr(0)**2*dr(2)
     & )
                     a21stt = (128*(rsxy(i1+1,i2,i3+1,axisp1,0)*jac3di(
     & i1+1-i10,i2-i20,i3+1-i30))+240*(rsxy(i1,i2,i3-1,axisp1,0)*
     & jac3di(i1-i10,i2-i20,i3-1-i30))+8*(rsxy(i1+2,i2,i3-1,axisp1,0)*
     & jac3di(i1+2-i10,i2-i20,i3-1-i30))+8*(rsxy(i1-2,i2,i3-1,axisp1,
     & 0)*jac3di(i1-2-i10,i2-i20,i3-1-i30))-128*(rsxy(i1-1,i2,i3-1,
     & axisp1,0)*jac3di(i1-1-i10,i2-i20,i3-1-i30))-240*(rsxy(i1,i2,i3+
     & 1,axisp1,0)*jac3di(i1-i10,i2-i20,i3+1-i30))-8*(rsxy(i1+2,i2,i3+
     & 1,axisp1,0)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-8*(rsxy(i1-2,i2,
     & i3+1,axisp1,0)*jac3di(i1-2-i10,i2-i20,i3+1-i30))-128*(rsxy(i1+
     & 1,i2,i3-1,axisp1,0)*jac3di(i1+1-i10,i2-i20,i3-1-i30))+128*(
     & rsxy(i1-1,i2,i3+1,axisp1,0)*jac3di(i1-1-i10,i2-i20,i3+1-i30))-
     & 16*(rsxy(i1+1,i2,i3+2,axisp1,0)*jac3di(i1+1-i10,i2-i20,i3+2-
     & i30))+30*(rsxy(i1,i2,i3+2,axisp1,0)*jac3di(i1-i10,i2-i20,i3+2-
     & i30))-16*(rsxy(i1-1,i2,i3+2,axisp1,0)*jac3di(i1-1-i10,i2-i20,
     & i3+2-i30))+(rsxy(i1+2,i2,i3+2,axisp1,0)*jac3di(i1+2-i10,i2-i20,
     & i3+2-i30))+(rsxy(i1-2,i2,i3+2,axisp1,0)*jac3di(i1-2-i10,i2-i20,
     & i3+2-i30))+16*(rsxy(i1+1,i2,i3-2,axisp1,0)*jac3di(i1+1-i10,i2-
     & i20,i3-2-i30))-30*(rsxy(i1,i2,i3-2,axisp1,0)*jac3di(i1-i10,i2-
     & i20,i3-2-i30))+16*(rsxy(i1-1,i2,i3-2,axisp1,0)*jac3di(i1-1-i10,
     & i2-i20,i3-2-i30))-(rsxy(i1+2,i2,i3-2,axisp1,0)*jac3di(i1+2-i10,
     & i2-i20,i3-2-i30))-(rsxy(i1-2,i2,i3-2,axisp1,0)*jac3di(i1-2-i10,
     & i2-i20,i3-2-i30)))/(144.*dr(0)**2*dr(2))
                     a22stt = (128*(rsxy(i1+1,i2,i3+1,axisp1,1)*jac3di(
     & i1+1-i10,i2-i20,i3+1-i30))+240*(rsxy(i1,i2,i3-1,axisp1,1)*
     & jac3di(i1-i10,i2-i20,i3-1-i30))+8*(rsxy(i1+2,i2,i3-1,axisp1,1)*
     & jac3di(i1+2-i10,i2-i20,i3-1-i30))+8*(rsxy(i1-2,i2,i3-1,axisp1,
     & 1)*jac3di(i1-2-i10,i2-i20,i3-1-i30))-128*(rsxy(i1-1,i2,i3-1,
     & axisp1,1)*jac3di(i1-1-i10,i2-i20,i3-1-i30))-240*(rsxy(i1,i2,i3+
     & 1,axisp1,1)*jac3di(i1-i10,i2-i20,i3+1-i30))-8*(rsxy(i1+2,i2,i3+
     & 1,axisp1,1)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-8*(rsxy(i1-2,i2,
     & i3+1,axisp1,1)*jac3di(i1-2-i10,i2-i20,i3+1-i30))-128*(rsxy(i1+
     & 1,i2,i3-1,axisp1,1)*jac3di(i1+1-i10,i2-i20,i3-1-i30))+128*(
     & rsxy(i1-1,i2,i3+1,axisp1,1)*jac3di(i1-1-i10,i2-i20,i3+1-i30))-
     & 16*(rsxy(i1+1,i2,i3+2,axisp1,1)*jac3di(i1+1-i10,i2-i20,i3+2-
     & i30))+30*(rsxy(i1,i2,i3+2,axisp1,1)*jac3di(i1-i10,i2-i20,i3+2-
     & i30))-16*(rsxy(i1-1,i2,i3+2,axisp1,1)*jac3di(i1-1-i10,i2-i20,
     & i3+2-i30))+(rsxy(i1+2,i2,i3+2,axisp1,1)*jac3di(i1+2-i10,i2-i20,
     & i3+2-i30))+(rsxy(i1-2,i2,i3+2,axisp1,1)*jac3di(i1-2-i10,i2-i20,
     & i3+2-i30))+16*(rsxy(i1+1,i2,i3-2,axisp1,1)*jac3di(i1+1-i10,i2-
     & i20,i3-2-i30))-30*(rsxy(i1,i2,i3-2,axisp1,1)*jac3di(i1-i10,i2-
     & i20,i3-2-i30))+16*(rsxy(i1-1,i2,i3-2,axisp1,1)*jac3di(i1-1-i10,
     & i2-i20,i3-2-i30))-(rsxy(i1+2,i2,i3-2,axisp1,1)*jac3di(i1+2-i10,
     & i2-i20,i3-2-i30))-(rsxy(i1-2,i2,i3-2,axisp1,1)*jac3di(i1-2-i10,
     & i2-i20,i3-2-i30)))/(144.*dr(0)**2*dr(2))
                     a23stt = (128*(rsxy(i1+1,i2,i3+1,axisp1,2)*jac3di(
     & i1+1-i10,i2-i20,i3+1-i30))+240*(rsxy(i1,i2,i3-1,axisp1,2)*
     & jac3di(i1-i10,i2-i20,i3-1-i30))+8*(rsxy(i1+2,i2,i3-1,axisp1,2)*
     & jac3di(i1+2-i10,i2-i20,i3-1-i30))+8*(rsxy(i1-2,i2,i3-1,axisp1,
     & 2)*jac3di(i1-2-i10,i2-i20,i3-1-i30))-128*(rsxy(i1-1,i2,i3-1,
     & axisp1,2)*jac3di(i1-1-i10,i2-i20,i3-1-i30))-240*(rsxy(i1,i2,i3+
     & 1,axisp1,2)*jac3di(i1-i10,i2-i20,i3+1-i30))-8*(rsxy(i1+2,i2,i3+
     & 1,axisp1,2)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-8*(rsxy(i1-2,i2,
     & i3+1,axisp1,2)*jac3di(i1-2-i10,i2-i20,i3+1-i30))-128*(rsxy(i1+
     & 1,i2,i3-1,axisp1,2)*jac3di(i1+1-i10,i2-i20,i3-1-i30))+128*(
     & rsxy(i1-1,i2,i3+1,axisp1,2)*jac3di(i1-1-i10,i2-i20,i3+1-i30))-
     & 16*(rsxy(i1+1,i2,i3+2,axisp1,2)*jac3di(i1+1-i10,i2-i20,i3+2-
     & i30))+30*(rsxy(i1,i2,i3+2,axisp1,2)*jac3di(i1-i10,i2-i20,i3+2-
     & i30))-16*(rsxy(i1-1,i2,i3+2,axisp1,2)*jac3di(i1-1-i10,i2-i20,
     & i3+2-i30))+(rsxy(i1+2,i2,i3+2,axisp1,2)*jac3di(i1+2-i10,i2-i20,
     & i3+2-i30))+(rsxy(i1-2,i2,i3+2,axisp1,2)*jac3di(i1-2-i10,i2-i20,
     & i3+2-i30))+16*(rsxy(i1+1,i2,i3-2,axisp1,2)*jac3di(i1+1-i10,i2-
     & i20,i3-2-i30))-30*(rsxy(i1,i2,i3-2,axisp1,2)*jac3di(i1-i10,i2-
     & i20,i3-2-i30))+16*(rsxy(i1-1,i2,i3-2,axisp1,2)*jac3di(i1-1-i10,
     & i2-i20,i3-2-i30))-(rsxy(i1+2,i2,i3-2,axisp1,2)*jac3di(i1+2-i10,
     & i2-i20,i3-2-i30))-(rsxy(i1-2,i2,i3-2,axisp1,2)*jac3di(i1-2-i10,
     & i2-i20,i3-2-i30)))/(144.*dr(0)**2*dr(2))
                     a31stt = (128*(rsxy(i1+1,i2,i3+1,axisp2,0)*jac3di(
     & i1+1-i10,i2-i20,i3+1-i30))+240*(rsxy(i1,i2,i3-1,axisp2,0)*
     & jac3di(i1-i10,i2-i20,i3-1-i30))+8*(rsxy(i1+2,i2,i3-1,axisp2,0)*
     & jac3di(i1+2-i10,i2-i20,i3-1-i30))+8*(rsxy(i1-2,i2,i3-1,axisp2,
     & 0)*jac3di(i1-2-i10,i2-i20,i3-1-i30))-128*(rsxy(i1-1,i2,i3-1,
     & axisp2,0)*jac3di(i1-1-i10,i2-i20,i3-1-i30))-240*(rsxy(i1,i2,i3+
     & 1,axisp2,0)*jac3di(i1-i10,i2-i20,i3+1-i30))-8*(rsxy(i1+2,i2,i3+
     & 1,axisp2,0)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-8*(rsxy(i1-2,i2,
     & i3+1,axisp2,0)*jac3di(i1-2-i10,i2-i20,i3+1-i30))-128*(rsxy(i1+
     & 1,i2,i3-1,axisp2,0)*jac3di(i1+1-i10,i2-i20,i3-1-i30))+128*(
     & rsxy(i1-1,i2,i3+1,axisp2,0)*jac3di(i1-1-i10,i2-i20,i3+1-i30))-
     & 16*(rsxy(i1+1,i2,i3+2,axisp2,0)*jac3di(i1+1-i10,i2-i20,i3+2-
     & i30))+30*(rsxy(i1,i2,i3+2,axisp2,0)*jac3di(i1-i10,i2-i20,i3+2-
     & i30))-16*(rsxy(i1-1,i2,i3+2,axisp2,0)*jac3di(i1-1-i10,i2-i20,
     & i3+2-i30))+(rsxy(i1+2,i2,i3+2,axisp2,0)*jac3di(i1+2-i10,i2-i20,
     & i3+2-i30))+(rsxy(i1-2,i2,i3+2,axisp2,0)*jac3di(i1-2-i10,i2-i20,
     & i3+2-i30))+16*(rsxy(i1+1,i2,i3-2,axisp2,0)*jac3di(i1+1-i10,i2-
     & i20,i3-2-i30))-30*(rsxy(i1,i2,i3-2,axisp2,0)*jac3di(i1-i10,i2-
     & i20,i3-2-i30))+16*(rsxy(i1-1,i2,i3-2,axisp2,0)*jac3di(i1-1-i10,
     & i2-i20,i3-2-i30))-(rsxy(i1+2,i2,i3-2,axisp2,0)*jac3di(i1+2-i10,
     & i2-i20,i3-2-i30))-(rsxy(i1-2,i2,i3-2,axisp2,0)*jac3di(i1-2-i10,
     & i2-i20,i3-2-i30)))/(144.*dr(0)**2*dr(2))
                     a32stt = (128*(rsxy(i1+1,i2,i3+1,axisp2,1)*jac3di(
     & i1+1-i10,i2-i20,i3+1-i30))+240*(rsxy(i1,i2,i3-1,axisp2,1)*
     & jac3di(i1-i10,i2-i20,i3-1-i30))+8*(rsxy(i1+2,i2,i3-1,axisp2,1)*
     & jac3di(i1+2-i10,i2-i20,i3-1-i30))+8*(rsxy(i1-2,i2,i3-1,axisp2,
     & 1)*jac3di(i1-2-i10,i2-i20,i3-1-i30))-128*(rsxy(i1-1,i2,i3-1,
     & axisp2,1)*jac3di(i1-1-i10,i2-i20,i3-1-i30))-240*(rsxy(i1,i2,i3+
     & 1,axisp2,1)*jac3di(i1-i10,i2-i20,i3+1-i30))-8*(rsxy(i1+2,i2,i3+
     & 1,axisp2,1)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-8*(rsxy(i1-2,i2,
     & i3+1,axisp2,1)*jac3di(i1-2-i10,i2-i20,i3+1-i30))-128*(rsxy(i1+
     & 1,i2,i3-1,axisp2,1)*jac3di(i1+1-i10,i2-i20,i3-1-i30))+128*(
     & rsxy(i1-1,i2,i3+1,axisp2,1)*jac3di(i1-1-i10,i2-i20,i3+1-i30))-
     & 16*(rsxy(i1+1,i2,i3+2,axisp2,1)*jac3di(i1+1-i10,i2-i20,i3+2-
     & i30))+30*(rsxy(i1,i2,i3+2,axisp2,1)*jac3di(i1-i10,i2-i20,i3+2-
     & i30))-16*(rsxy(i1-1,i2,i3+2,axisp2,1)*jac3di(i1-1-i10,i2-i20,
     & i3+2-i30))+(rsxy(i1+2,i2,i3+2,axisp2,1)*jac3di(i1+2-i10,i2-i20,
     & i3+2-i30))+(rsxy(i1-2,i2,i3+2,axisp2,1)*jac3di(i1-2-i10,i2-i20,
     & i3+2-i30))+16*(rsxy(i1+1,i2,i3-2,axisp2,1)*jac3di(i1+1-i10,i2-
     & i20,i3-2-i30))-30*(rsxy(i1,i2,i3-2,axisp2,1)*jac3di(i1-i10,i2-
     & i20,i3-2-i30))+16*(rsxy(i1-1,i2,i3-2,axisp2,1)*jac3di(i1-1-i10,
     & i2-i20,i3-2-i30))-(rsxy(i1+2,i2,i3-2,axisp2,1)*jac3di(i1+2-i10,
     & i2-i20,i3-2-i30))-(rsxy(i1-2,i2,i3-2,axisp2,1)*jac3di(i1-2-i10,
     & i2-i20,i3-2-i30)))/(144.*dr(0)**2*dr(2))
                     a33stt = (128*(rsxy(i1+1,i2,i3+1,axisp2,2)*jac3di(
     & i1+1-i10,i2-i20,i3+1-i30))+240*(rsxy(i1,i2,i3-1,axisp2,2)*
     & jac3di(i1-i10,i2-i20,i3-1-i30))+8*(rsxy(i1+2,i2,i3-1,axisp2,2)*
     & jac3di(i1+2-i10,i2-i20,i3-1-i30))+8*(rsxy(i1-2,i2,i3-1,axisp2,
     & 2)*jac3di(i1-2-i10,i2-i20,i3-1-i30))-128*(rsxy(i1-1,i2,i3-1,
     & axisp2,2)*jac3di(i1-1-i10,i2-i20,i3-1-i30))-240*(rsxy(i1,i2,i3+
     & 1,axisp2,2)*jac3di(i1-i10,i2-i20,i3+1-i30))-8*(rsxy(i1+2,i2,i3+
     & 1,axisp2,2)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-8*(rsxy(i1-2,i2,
     & i3+1,axisp2,2)*jac3di(i1-2-i10,i2-i20,i3+1-i30))-128*(rsxy(i1+
     & 1,i2,i3-1,axisp2,2)*jac3di(i1+1-i10,i2-i20,i3-1-i30))+128*(
     & rsxy(i1-1,i2,i3+1,axisp2,2)*jac3di(i1-1-i10,i2-i20,i3+1-i30))-
     & 16*(rsxy(i1+1,i2,i3+2,axisp2,2)*jac3di(i1+1-i10,i2-i20,i3+2-
     & i30))+30*(rsxy(i1,i2,i3+2,axisp2,2)*jac3di(i1-i10,i2-i20,i3+2-
     & i30))-16*(rsxy(i1-1,i2,i3+2,axisp2,2)*jac3di(i1-1-i10,i2-i20,
     & i3+2-i30))+(rsxy(i1+2,i2,i3+2,axisp2,2)*jac3di(i1+2-i10,i2-i20,
     & i3+2-i30))+(rsxy(i1-2,i2,i3+2,axisp2,2)*jac3di(i1-2-i10,i2-i20,
     & i3+2-i30))+16*(rsxy(i1+1,i2,i3-2,axisp2,2)*jac3di(i1+1-i10,i2-
     & i20,i3-2-i30))-30*(rsxy(i1,i2,i3-2,axisp2,2)*jac3di(i1-i10,i2-
     & i20,i3-2-i30))+16*(rsxy(i1-1,i2,i3-2,axisp2,2)*jac3di(i1-1-i10,
     & i2-i20,i3-2-i30))-(rsxy(i1+2,i2,i3-2,axisp2,2)*jac3di(i1+2-i10,
     & i2-i20,i3-2-i30))-(rsxy(i1-2,i2,i3-2,axisp2,2)*jac3di(i1-2-i10,
     & i2-i20,i3-2-i30)))/(144.*dr(0)**2*dr(2))
                     a11sst = (128*(rsxy(i1+1,i2,i3+1,axis,0)*jac3di(
     & i1+1-i10,i2-i20,i3+1-i30))-16*(rsxy(i1+2,i2,i3-1,axis,0)*
     & jac3di(i1+2-i10,i2-i20,i3-1-i30))+16*(rsxy(i1-2,i2,i3-1,axis,0)
     & *jac3di(i1-2-i10,i2-i20,i3-1-i30))-128*(rsxy(i1-1,i2,i3-1,axis,
     & 0)*jac3di(i1-1-i10,i2-i20,i3-1-i30))-16*(rsxy(i1+2,i2,i3+1,
     & axis,0)*jac3di(i1+2-i10,i2-i20,i3+1-i30))+16*(rsxy(i1-2,i2,i3+
     & 1,axis,0)*jac3di(i1-2-i10,i2-i20,i3+1-i30))+128*(rsxy(i1+1,i2,
     & i3-1,axis,0)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-128*(rsxy(i1-1,
     & i2,i3+1,axis,0)*jac3di(i1-1-i10,i2-i20,i3+1-i30))-8*(rsxy(i1+1,
     & i2,i3+2,axis,0)*jac3di(i1+1-i10,i2-i20,i3+2-i30))+8*(rsxy(i1-1,
     & i2,i3+2,axis,0)*jac3di(i1-1-i10,i2-i20,i3+2-i30))+(rsxy(i1+2,
     & i2,i3+2,axis,0)*jac3di(i1+2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,
     & i2,i3+2,axis,0)*jac3di(i1-2-i10,i2-i20,i3+2-i30))-8*(rsxy(i1+1,
     & i2,i3-2,axis,0)*jac3di(i1+1-i10,i2-i20,i3-2-i30))+8*(rsxy(i1-1,
     & i2,i3-2,axis,0)*jac3di(i1-1-i10,i2-i20,i3-2-i30))+(rsxy(i1+2,
     & i2,i3-2,axis,0)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,
     & i2,i3-2,axis,0)*jac3di(i1-2-i10,i2-i20,i3-2-i30))+30*(rsxy(i1+
     & 2,i2,i3,axis,0)*jac3di(i1+2-i10,i2-i20,i3-i30))-30*(rsxy(i1-2,
     & i2,i3,axis,0)*jac3di(i1-2-i10,i2-i20,i3-i30))-240*(rsxy(i1+1,
     & i2,i3,axis,0)*jac3di(i1+1-i10,i2-i20,i3-i30))+240*(rsxy(i1-1,
     & i2,i3,axis,0)*jac3di(i1-1-i10,i2-i20,i3-i30)))/(144.*dr(2)**2*
     & dr(0))
                     a12sst = (128*(rsxy(i1+1,i2,i3+1,axis,1)*jac3di(
     & i1+1-i10,i2-i20,i3+1-i30))-16*(rsxy(i1+2,i2,i3-1,axis,1)*
     & jac3di(i1+2-i10,i2-i20,i3-1-i30))+16*(rsxy(i1-2,i2,i3-1,axis,1)
     & *jac3di(i1-2-i10,i2-i20,i3-1-i30))-128*(rsxy(i1-1,i2,i3-1,axis,
     & 1)*jac3di(i1-1-i10,i2-i20,i3-1-i30))-16*(rsxy(i1+2,i2,i3+1,
     & axis,1)*jac3di(i1+2-i10,i2-i20,i3+1-i30))+16*(rsxy(i1-2,i2,i3+
     & 1,axis,1)*jac3di(i1-2-i10,i2-i20,i3+1-i30))+128*(rsxy(i1+1,i2,
     & i3-1,axis,1)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-128*(rsxy(i1-1,
     & i2,i3+1,axis,1)*jac3di(i1-1-i10,i2-i20,i3+1-i30))-8*(rsxy(i1+1,
     & i2,i3+2,axis,1)*jac3di(i1+1-i10,i2-i20,i3+2-i30))+8*(rsxy(i1-1,
     & i2,i3+2,axis,1)*jac3di(i1-1-i10,i2-i20,i3+2-i30))+(rsxy(i1+2,
     & i2,i3+2,axis,1)*jac3di(i1+2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,
     & i2,i3+2,axis,1)*jac3di(i1-2-i10,i2-i20,i3+2-i30))-8*(rsxy(i1+1,
     & i2,i3-2,axis,1)*jac3di(i1+1-i10,i2-i20,i3-2-i30))+8*(rsxy(i1-1,
     & i2,i3-2,axis,1)*jac3di(i1-1-i10,i2-i20,i3-2-i30))+(rsxy(i1+2,
     & i2,i3-2,axis,1)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,
     & i2,i3-2,axis,1)*jac3di(i1-2-i10,i2-i20,i3-2-i30))+30*(rsxy(i1+
     & 2,i2,i3,axis,1)*jac3di(i1+2-i10,i2-i20,i3-i30))-30*(rsxy(i1-2,
     & i2,i3,axis,1)*jac3di(i1-2-i10,i2-i20,i3-i30))-240*(rsxy(i1+1,
     & i2,i3,axis,1)*jac3di(i1+1-i10,i2-i20,i3-i30))+240*(rsxy(i1-1,
     & i2,i3,axis,1)*jac3di(i1-1-i10,i2-i20,i3-i30)))/(144.*dr(2)**2*
     & dr(0))
                     a13sst = (128*(rsxy(i1+1,i2,i3+1,axis,2)*jac3di(
     & i1+1-i10,i2-i20,i3+1-i30))-16*(rsxy(i1+2,i2,i3-1,axis,2)*
     & jac3di(i1+2-i10,i2-i20,i3-1-i30))+16*(rsxy(i1-2,i2,i3-1,axis,2)
     & *jac3di(i1-2-i10,i2-i20,i3-1-i30))-128*(rsxy(i1-1,i2,i3-1,axis,
     & 2)*jac3di(i1-1-i10,i2-i20,i3-1-i30))-16*(rsxy(i1+2,i2,i3+1,
     & axis,2)*jac3di(i1+2-i10,i2-i20,i3+1-i30))+16*(rsxy(i1-2,i2,i3+
     & 1,axis,2)*jac3di(i1-2-i10,i2-i20,i3+1-i30))+128*(rsxy(i1+1,i2,
     & i3-1,axis,2)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-128*(rsxy(i1-1,
     & i2,i3+1,axis,2)*jac3di(i1-1-i10,i2-i20,i3+1-i30))-8*(rsxy(i1+1,
     & i2,i3+2,axis,2)*jac3di(i1+1-i10,i2-i20,i3+2-i30))+8*(rsxy(i1-1,
     & i2,i3+2,axis,2)*jac3di(i1-1-i10,i2-i20,i3+2-i30))+(rsxy(i1+2,
     & i2,i3+2,axis,2)*jac3di(i1+2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,
     & i2,i3+2,axis,2)*jac3di(i1-2-i10,i2-i20,i3+2-i30))-8*(rsxy(i1+1,
     & i2,i3-2,axis,2)*jac3di(i1+1-i10,i2-i20,i3-2-i30))+8*(rsxy(i1-1,
     & i2,i3-2,axis,2)*jac3di(i1-1-i10,i2-i20,i3-2-i30))+(rsxy(i1+2,
     & i2,i3-2,axis,2)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,
     & i2,i3-2,axis,2)*jac3di(i1-2-i10,i2-i20,i3-2-i30))+30*(rsxy(i1+
     & 2,i2,i3,axis,2)*jac3di(i1+2-i10,i2-i20,i3-i30))-30*(rsxy(i1-2,
     & i2,i3,axis,2)*jac3di(i1-2-i10,i2-i20,i3-i30))-240*(rsxy(i1+1,
     & i2,i3,axis,2)*jac3di(i1+1-i10,i2-i20,i3-i30))+240*(rsxy(i1-1,
     & i2,i3,axis,2)*jac3di(i1-1-i10,i2-i20,i3-i30)))/(144.*dr(2)**2*
     & dr(0))
                     a21sst = (128*(rsxy(i1+1,i2,i3+1,axisp1,0)*jac3di(
     & i1+1-i10,i2-i20,i3+1-i30))-16*(rsxy(i1+2,i2,i3-1,axisp1,0)*
     & jac3di(i1+2-i10,i2-i20,i3-1-i30))+16*(rsxy(i1-2,i2,i3-1,axisp1,
     & 0)*jac3di(i1-2-i10,i2-i20,i3-1-i30))-128*(rsxy(i1-1,i2,i3-1,
     & axisp1,0)*jac3di(i1-1-i10,i2-i20,i3-1-i30))-16*(rsxy(i1+2,i2,
     & i3+1,axisp1,0)*jac3di(i1+2-i10,i2-i20,i3+1-i30))+16*(rsxy(i1-2,
     & i2,i3+1,axisp1,0)*jac3di(i1-2-i10,i2-i20,i3+1-i30))+128*(rsxy(
     & i1+1,i2,i3-1,axisp1,0)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-128*(
     & rsxy(i1-1,i2,i3+1,axisp1,0)*jac3di(i1-1-i10,i2-i20,i3+1-i30))-
     & 8*(rsxy(i1+1,i2,i3+2,axisp1,0)*jac3di(i1+1-i10,i2-i20,i3+2-i30)
     & )+8*(rsxy(i1-1,i2,i3+2,axisp1,0)*jac3di(i1-1-i10,i2-i20,i3+2-
     & i30))+(rsxy(i1+2,i2,i3+2,axisp1,0)*jac3di(i1+2-i10,i2-i20,i3+2-
     & i30))-(rsxy(i1-2,i2,i3+2,axisp1,0)*jac3di(i1-2-i10,i2-i20,i3+2-
     & i30))-8*(rsxy(i1+1,i2,i3-2,axisp1,0)*jac3di(i1+1-i10,i2-i20,i3-
     & 2-i30))+8*(rsxy(i1-1,i2,i3-2,axisp1,0)*jac3di(i1-1-i10,i2-i20,
     & i3-2-i30))+(rsxy(i1+2,i2,i3-2,axisp1,0)*jac3di(i1+2-i10,i2-i20,
     & i3-2-i30))-(rsxy(i1-2,i2,i3-2,axisp1,0)*jac3di(i1-2-i10,i2-i20,
     & i3-2-i30))+30*(rsxy(i1+2,i2,i3,axisp1,0)*jac3di(i1+2-i10,i2-
     & i20,i3-i30))-30*(rsxy(i1-2,i2,i3,axisp1,0)*jac3di(i1-2-i10,i2-
     & i20,i3-i30))-240*(rsxy(i1+1,i2,i3,axisp1,0)*jac3di(i1+1-i10,i2-
     & i20,i3-i30))+240*(rsxy(i1-1,i2,i3,axisp1,0)*jac3di(i1-1-i10,i2-
     & i20,i3-i30)))/(144.*dr(2)**2*dr(0))
                     a22sst = (128*(rsxy(i1+1,i2,i3+1,axisp1,1)*jac3di(
     & i1+1-i10,i2-i20,i3+1-i30))-16*(rsxy(i1+2,i2,i3-1,axisp1,1)*
     & jac3di(i1+2-i10,i2-i20,i3-1-i30))+16*(rsxy(i1-2,i2,i3-1,axisp1,
     & 1)*jac3di(i1-2-i10,i2-i20,i3-1-i30))-128*(rsxy(i1-1,i2,i3-1,
     & axisp1,1)*jac3di(i1-1-i10,i2-i20,i3-1-i30))-16*(rsxy(i1+2,i2,
     & i3+1,axisp1,1)*jac3di(i1+2-i10,i2-i20,i3+1-i30))+16*(rsxy(i1-2,
     & i2,i3+1,axisp1,1)*jac3di(i1-2-i10,i2-i20,i3+1-i30))+128*(rsxy(
     & i1+1,i2,i3-1,axisp1,1)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-128*(
     & rsxy(i1-1,i2,i3+1,axisp1,1)*jac3di(i1-1-i10,i2-i20,i3+1-i30))-
     & 8*(rsxy(i1+1,i2,i3+2,axisp1,1)*jac3di(i1+1-i10,i2-i20,i3+2-i30)
     & )+8*(rsxy(i1-1,i2,i3+2,axisp1,1)*jac3di(i1-1-i10,i2-i20,i3+2-
     & i30))+(rsxy(i1+2,i2,i3+2,axisp1,1)*jac3di(i1+2-i10,i2-i20,i3+2-
     & i30))-(rsxy(i1-2,i2,i3+2,axisp1,1)*jac3di(i1-2-i10,i2-i20,i3+2-
     & i30))-8*(rsxy(i1+1,i2,i3-2,axisp1,1)*jac3di(i1+1-i10,i2-i20,i3-
     & 2-i30))+8*(rsxy(i1-1,i2,i3-2,axisp1,1)*jac3di(i1-1-i10,i2-i20,
     & i3-2-i30))+(rsxy(i1+2,i2,i3-2,axisp1,1)*jac3di(i1+2-i10,i2-i20,
     & i3-2-i30))-(rsxy(i1-2,i2,i3-2,axisp1,1)*jac3di(i1-2-i10,i2-i20,
     & i3-2-i30))+30*(rsxy(i1+2,i2,i3,axisp1,1)*jac3di(i1+2-i10,i2-
     & i20,i3-i30))-30*(rsxy(i1-2,i2,i3,axisp1,1)*jac3di(i1-2-i10,i2-
     & i20,i3-i30))-240*(rsxy(i1+1,i2,i3,axisp1,1)*jac3di(i1+1-i10,i2-
     & i20,i3-i30))+240*(rsxy(i1-1,i2,i3,axisp1,1)*jac3di(i1-1-i10,i2-
     & i20,i3-i30)))/(144.*dr(2)**2*dr(0))
                     a23sst = (128*(rsxy(i1+1,i2,i3+1,axisp1,2)*jac3di(
     & i1+1-i10,i2-i20,i3+1-i30))-16*(rsxy(i1+2,i2,i3-1,axisp1,2)*
     & jac3di(i1+2-i10,i2-i20,i3-1-i30))+16*(rsxy(i1-2,i2,i3-1,axisp1,
     & 2)*jac3di(i1-2-i10,i2-i20,i3-1-i30))-128*(rsxy(i1-1,i2,i3-1,
     & axisp1,2)*jac3di(i1-1-i10,i2-i20,i3-1-i30))-16*(rsxy(i1+2,i2,
     & i3+1,axisp1,2)*jac3di(i1+2-i10,i2-i20,i3+1-i30))+16*(rsxy(i1-2,
     & i2,i3+1,axisp1,2)*jac3di(i1-2-i10,i2-i20,i3+1-i30))+128*(rsxy(
     & i1+1,i2,i3-1,axisp1,2)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-128*(
     & rsxy(i1-1,i2,i3+1,axisp1,2)*jac3di(i1-1-i10,i2-i20,i3+1-i30))-
     & 8*(rsxy(i1+1,i2,i3+2,axisp1,2)*jac3di(i1+1-i10,i2-i20,i3+2-i30)
     & )+8*(rsxy(i1-1,i2,i3+2,axisp1,2)*jac3di(i1-1-i10,i2-i20,i3+2-
     & i30))+(rsxy(i1+2,i2,i3+2,axisp1,2)*jac3di(i1+2-i10,i2-i20,i3+2-
     & i30))-(rsxy(i1-2,i2,i3+2,axisp1,2)*jac3di(i1-2-i10,i2-i20,i3+2-
     & i30))-8*(rsxy(i1+1,i2,i3-2,axisp1,2)*jac3di(i1+1-i10,i2-i20,i3-
     & 2-i30))+8*(rsxy(i1-1,i2,i3-2,axisp1,2)*jac3di(i1-1-i10,i2-i20,
     & i3-2-i30))+(rsxy(i1+2,i2,i3-2,axisp1,2)*jac3di(i1+2-i10,i2-i20,
     & i3-2-i30))-(rsxy(i1-2,i2,i3-2,axisp1,2)*jac3di(i1-2-i10,i2-i20,
     & i3-2-i30))+30*(rsxy(i1+2,i2,i3,axisp1,2)*jac3di(i1+2-i10,i2-
     & i20,i3-i30))-30*(rsxy(i1-2,i2,i3,axisp1,2)*jac3di(i1-2-i10,i2-
     & i20,i3-i30))-240*(rsxy(i1+1,i2,i3,axisp1,2)*jac3di(i1+1-i10,i2-
     & i20,i3-i30))+240*(rsxy(i1-1,i2,i3,axisp1,2)*jac3di(i1-1-i10,i2-
     & i20,i3-i30)))/(144.*dr(2)**2*dr(0))
                     a31sst = (128*(rsxy(i1+1,i2,i3+1,axisp2,0)*jac3di(
     & i1+1-i10,i2-i20,i3+1-i30))-16*(rsxy(i1+2,i2,i3-1,axisp2,0)*
     & jac3di(i1+2-i10,i2-i20,i3-1-i30))+16*(rsxy(i1-2,i2,i3-1,axisp2,
     & 0)*jac3di(i1-2-i10,i2-i20,i3-1-i30))-128*(rsxy(i1-1,i2,i3-1,
     & axisp2,0)*jac3di(i1-1-i10,i2-i20,i3-1-i30))-16*(rsxy(i1+2,i2,
     & i3+1,axisp2,0)*jac3di(i1+2-i10,i2-i20,i3+1-i30))+16*(rsxy(i1-2,
     & i2,i3+1,axisp2,0)*jac3di(i1-2-i10,i2-i20,i3+1-i30))+128*(rsxy(
     & i1+1,i2,i3-1,axisp2,0)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-128*(
     & rsxy(i1-1,i2,i3+1,axisp2,0)*jac3di(i1-1-i10,i2-i20,i3+1-i30))-
     & 8*(rsxy(i1+1,i2,i3+2,axisp2,0)*jac3di(i1+1-i10,i2-i20,i3+2-i30)
     & )+8*(rsxy(i1-1,i2,i3+2,axisp2,0)*jac3di(i1-1-i10,i2-i20,i3+2-
     & i30))+(rsxy(i1+2,i2,i3+2,axisp2,0)*jac3di(i1+2-i10,i2-i20,i3+2-
     & i30))-(rsxy(i1-2,i2,i3+2,axisp2,0)*jac3di(i1-2-i10,i2-i20,i3+2-
     & i30))-8*(rsxy(i1+1,i2,i3-2,axisp2,0)*jac3di(i1+1-i10,i2-i20,i3-
     & 2-i30))+8*(rsxy(i1-1,i2,i3-2,axisp2,0)*jac3di(i1-1-i10,i2-i20,
     & i3-2-i30))+(rsxy(i1+2,i2,i3-2,axisp2,0)*jac3di(i1+2-i10,i2-i20,
     & i3-2-i30))-(rsxy(i1-2,i2,i3-2,axisp2,0)*jac3di(i1-2-i10,i2-i20,
     & i3-2-i30))+30*(rsxy(i1+2,i2,i3,axisp2,0)*jac3di(i1+2-i10,i2-
     & i20,i3-i30))-30*(rsxy(i1-2,i2,i3,axisp2,0)*jac3di(i1-2-i10,i2-
     & i20,i3-i30))-240*(rsxy(i1+1,i2,i3,axisp2,0)*jac3di(i1+1-i10,i2-
     & i20,i3-i30))+240*(rsxy(i1-1,i2,i3,axisp2,0)*jac3di(i1-1-i10,i2-
     & i20,i3-i30)))/(144.*dr(2)**2*dr(0))
                     a32sst = (128*(rsxy(i1+1,i2,i3+1,axisp2,1)*jac3di(
     & i1+1-i10,i2-i20,i3+1-i30))-16*(rsxy(i1+2,i2,i3-1,axisp2,1)*
     & jac3di(i1+2-i10,i2-i20,i3-1-i30))+16*(rsxy(i1-2,i2,i3-1,axisp2,
     & 1)*jac3di(i1-2-i10,i2-i20,i3-1-i30))-128*(rsxy(i1-1,i2,i3-1,
     & axisp2,1)*jac3di(i1-1-i10,i2-i20,i3-1-i30))-16*(rsxy(i1+2,i2,
     & i3+1,axisp2,1)*jac3di(i1+2-i10,i2-i20,i3+1-i30))+16*(rsxy(i1-2,
     & i2,i3+1,axisp2,1)*jac3di(i1-2-i10,i2-i20,i3+1-i30))+128*(rsxy(
     & i1+1,i2,i3-1,axisp2,1)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-128*(
     & rsxy(i1-1,i2,i3+1,axisp2,1)*jac3di(i1-1-i10,i2-i20,i3+1-i30))-
     & 8*(rsxy(i1+1,i2,i3+2,axisp2,1)*jac3di(i1+1-i10,i2-i20,i3+2-i30)
     & )+8*(rsxy(i1-1,i2,i3+2,axisp2,1)*jac3di(i1-1-i10,i2-i20,i3+2-
     & i30))+(rsxy(i1+2,i2,i3+2,axisp2,1)*jac3di(i1+2-i10,i2-i20,i3+2-
     & i30))-(rsxy(i1-2,i2,i3+2,axisp2,1)*jac3di(i1-2-i10,i2-i20,i3+2-
     & i30))-8*(rsxy(i1+1,i2,i3-2,axisp2,1)*jac3di(i1+1-i10,i2-i20,i3-
     & 2-i30))+8*(rsxy(i1-1,i2,i3-2,axisp2,1)*jac3di(i1-1-i10,i2-i20,
     & i3-2-i30))+(rsxy(i1+2,i2,i3-2,axisp2,1)*jac3di(i1+2-i10,i2-i20,
     & i3-2-i30))-(rsxy(i1-2,i2,i3-2,axisp2,1)*jac3di(i1-2-i10,i2-i20,
     & i3-2-i30))+30*(rsxy(i1+2,i2,i3,axisp2,1)*jac3di(i1+2-i10,i2-
     & i20,i3-i30))-30*(rsxy(i1-2,i2,i3,axisp2,1)*jac3di(i1-2-i10,i2-
     & i20,i3-i30))-240*(rsxy(i1+1,i2,i3,axisp2,1)*jac3di(i1+1-i10,i2-
     & i20,i3-i30))+240*(rsxy(i1-1,i2,i3,axisp2,1)*jac3di(i1-1-i10,i2-
     & i20,i3-i30)))/(144.*dr(2)**2*dr(0))
                     a33sst = (128*(rsxy(i1+1,i2,i3+1,axisp2,2)*jac3di(
     & i1+1-i10,i2-i20,i3+1-i30))-16*(rsxy(i1+2,i2,i3-1,axisp2,2)*
     & jac3di(i1+2-i10,i2-i20,i3-1-i30))+16*(rsxy(i1-2,i2,i3-1,axisp2,
     & 2)*jac3di(i1-2-i10,i2-i20,i3-1-i30))-128*(rsxy(i1-1,i2,i3-1,
     & axisp2,2)*jac3di(i1-1-i10,i2-i20,i3-1-i30))-16*(rsxy(i1+2,i2,
     & i3+1,axisp2,2)*jac3di(i1+2-i10,i2-i20,i3+1-i30))+16*(rsxy(i1-2,
     & i2,i3+1,axisp2,2)*jac3di(i1-2-i10,i2-i20,i3+1-i30))+128*(rsxy(
     & i1+1,i2,i3-1,axisp2,2)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-128*(
     & rsxy(i1-1,i2,i3+1,axisp2,2)*jac3di(i1-1-i10,i2-i20,i3+1-i30))-
     & 8*(rsxy(i1+1,i2,i3+2,axisp2,2)*jac3di(i1+1-i10,i2-i20,i3+2-i30)
     & )+8*(rsxy(i1-1,i2,i3+2,axisp2,2)*jac3di(i1-1-i10,i2-i20,i3+2-
     & i30))+(rsxy(i1+2,i2,i3+2,axisp2,2)*jac3di(i1+2-i10,i2-i20,i3+2-
     & i30))-(rsxy(i1-2,i2,i3+2,axisp2,2)*jac3di(i1-2-i10,i2-i20,i3+2-
     & i30))-8*(rsxy(i1+1,i2,i3-2,axisp2,2)*jac3di(i1+1-i10,i2-i20,i3-
     & 2-i30))+8*(rsxy(i1-1,i2,i3-2,axisp2,2)*jac3di(i1-1-i10,i2-i20,
     & i3-2-i30))+(rsxy(i1+2,i2,i3-2,axisp2,2)*jac3di(i1+2-i10,i2-i20,
     & i3-2-i30))-(rsxy(i1-2,i2,i3-2,axisp2,2)*jac3di(i1-2-i10,i2-i20,
     & i3-2-i30))+30*(rsxy(i1+2,i2,i3,axisp2,2)*jac3di(i1+2-i10,i2-
     & i20,i3-i30))-30*(rsxy(i1-2,i2,i3,axisp2,2)*jac3di(i1-2-i10,i2-
     & i20,i3-i30))-240*(rsxy(i1+1,i2,i3,axisp2,2)*jac3di(i1+1-i10,i2-
     & i20,i3-i30))+240*(rsxy(i1-1,i2,i3,axisp2,2)*jac3di(i1-1-i10,i2-
     & i20,i3-i30)))/(144.*dr(2)**2*dr(0))
                     ust=urt4(i1,i2,i3,ex)
                     usst=urtt4(i1,i2,i3,ex)
                     ustt=urrt4(i1,i2,i3,ex)
                     vst  =urt4(i1,i2,i3,ey)
                     vsst=urtt4(i1,i2,i3,ey)
                     vstt=urrt4(i1,i2,i3,ey)
                     wst  =urt4(i1,i2,i3,ez)
                     wsst=urtt4(i1,i2,i3,ez)
                     wstt=urrt4(i1,i2,i3,ez)
                  else ! axis.eq.2
                     a11rs = (8.*((8.*((rsxy(i1+1,i2,i3+1,axis,0)*
     & jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axis,0)*
     & jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,axis,0)*
     & jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,axis,0)*
     & jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*((rsxy(i1+
     & 1,i2,i3-1,axis,0)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-(rsxy(i1-1,
     & i2,i3-1,axis,0)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))-((rsxy(i1+2,
     & i2,i3-1,axis,0)*jac3di(i1+2-i10,i2-i20,i3-1-i30))-(rsxy(i1-2,
     & i2,i3-1,axis,0)*jac3di(i1-2-i10,i2-i20,i3-1-i30))))/(12.*dr(0))
     & )-((8.*((rsxy(i1+1,i2,i3+2,axis,0)*jac3di(i1+1-i10,i2-i20,i3+2-
     & i30))-(rsxy(i1-1,i2,i3+2,axis,0)*jac3di(i1-1-i10,i2-i20,i3+2-
     & i30)))-((rsxy(i1+2,i2,i3+2,axis,0)*jac3di(i1+2-i10,i2-i20,i3+2-
     & i30))-(rsxy(i1-2,i2,i3+2,axis,0)*jac3di(i1-2-i10,i2-i20,i3+2-
     & i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-2,axis,0)*jac3di(i1+
     & 1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-2,axis,0)*jac3di(i1-1-
     & i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,i3-2,axis,0)*jac3di(i1+2-
     & i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,i3-2,axis,0)*jac3di(i1-2-
     & i10,i2-i20,i3-2-i30))))/(12.*dr(0))))/(12.*dr(2))
                     a12rs = (8.*((8.*((rsxy(i1+1,i2,i3+1,axis,1)*
     & jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axis,1)*
     & jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,axis,1)*
     & jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,axis,1)*
     & jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*((rsxy(i1+
     & 1,i2,i3-1,axis,1)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-(rsxy(i1-1,
     & i2,i3-1,axis,1)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))-((rsxy(i1+2,
     & i2,i3-1,axis,1)*jac3di(i1+2-i10,i2-i20,i3-1-i30))-(rsxy(i1-2,
     & i2,i3-1,axis,1)*jac3di(i1-2-i10,i2-i20,i3-1-i30))))/(12.*dr(0))
     & )-((8.*((rsxy(i1+1,i2,i3+2,axis,1)*jac3di(i1+1-i10,i2-i20,i3+2-
     & i30))-(rsxy(i1-1,i2,i3+2,axis,1)*jac3di(i1-1-i10,i2-i20,i3+2-
     & i30)))-((rsxy(i1+2,i2,i3+2,axis,1)*jac3di(i1+2-i10,i2-i20,i3+2-
     & i30))-(rsxy(i1-2,i2,i3+2,axis,1)*jac3di(i1-2-i10,i2-i20,i3+2-
     & i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-2,axis,1)*jac3di(i1+
     & 1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-2,axis,1)*jac3di(i1-1-
     & i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,i3-2,axis,1)*jac3di(i1+2-
     & i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,i3-2,axis,1)*jac3di(i1-2-
     & i10,i2-i20,i3-2-i30))))/(12.*dr(0))))/(12.*dr(2))
                     a13rs = (8.*((8.*((rsxy(i1+1,i2,i3+1,axis,2)*
     & jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axis,2)*
     & jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,axis,2)*
     & jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,axis,2)*
     & jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*((rsxy(i1+
     & 1,i2,i3-1,axis,2)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-(rsxy(i1-1,
     & i2,i3-1,axis,2)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))-((rsxy(i1+2,
     & i2,i3-1,axis,2)*jac3di(i1+2-i10,i2-i20,i3-1-i30))-(rsxy(i1-2,
     & i2,i3-1,axis,2)*jac3di(i1-2-i10,i2-i20,i3-1-i30))))/(12.*dr(0))
     & )-((8.*((rsxy(i1+1,i2,i3+2,axis,2)*jac3di(i1+1-i10,i2-i20,i3+2-
     & i30))-(rsxy(i1-1,i2,i3+2,axis,2)*jac3di(i1-1-i10,i2-i20,i3+2-
     & i30)))-((rsxy(i1+2,i2,i3+2,axis,2)*jac3di(i1+2-i10,i2-i20,i3+2-
     & i30))-(rsxy(i1-2,i2,i3+2,axis,2)*jac3di(i1-2-i10,i2-i20,i3+2-
     & i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-2,axis,2)*jac3di(i1+
     & 1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-2,axis,2)*jac3di(i1-1-
     & i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,i3-2,axis,2)*jac3di(i1+2-
     & i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,i3-2,axis,2)*jac3di(i1-2-
     & i10,i2-i20,i3-2-i30))))/(12.*dr(0))))/(12.*dr(2))
                     a21rs = (8.*((8.*((rsxy(i1+1,i2,i3+1,axisp1,0)*
     & jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axisp1,0)*
     & jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,axisp1,
     & 0)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,axisp1,
     & 0)*jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*((rsxy(
     & i1+1,i2,i3-1,axisp1,0)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-(rsxy(
     & i1-1,i2,i3-1,axisp1,0)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))-((
     & rsxy(i1+2,i2,i3-1,axisp1,0)*jac3di(i1+2-i10,i2-i20,i3-1-i30))-(
     & rsxy(i1-2,i2,i3-1,axisp1,0)*jac3di(i1-2-i10,i2-i20,i3-1-i30))))
     & /(12.*dr(0)))-((8.*((rsxy(i1+1,i2,i3+2,axisp1,0)*jac3di(i1+1-
     & i10,i2-i20,i3+2-i30))-(rsxy(i1-1,i2,i3+2,axisp1,0)*jac3di(i1-1-
     & i10,i2-i20,i3+2-i30)))-((rsxy(i1+2,i2,i3+2,axisp1,0)*jac3di(i1+
     & 2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,i2,i3+2,axisp1,0)*jac3di(i1-
     & 2-i10,i2-i20,i3+2-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-2,
     & axisp1,0)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-2,
     & axisp1,0)*jac3di(i1-1-i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,i3-
     & 2,axisp1,0)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,i3-
     & 2,axisp1,0)*jac3di(i1-2-i10,i2-i20,i3-2-i30))))/(12.*dr(0))))/(
     & 12.*dr(2))
                     a22rs = (8.*((8.*((rsxy(i1+1,i2,i3+1,axisp1,1)*
     & jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axisp1,1)*
     & jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,axisp1,
     & 1)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,axisp1,
     & 1)*jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*((rsxy(
     & i1+1,i2,i3-1,axisp1,1)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-(rsxy(
     & i1-1,i2,i3-1,axisp1,1)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))-((
     & rsxy(i1+2,i2,i3-1,axisp1,1)*jac3di(i1+2-i10,i2-i20,i3-1-i30))-(
     & rsxy(i1-2,i2,i3-1,axisp1,1)*jac3di(i1-2-i10,i2-i20,i3-1-i30))))
     & /(12.*dr(0)))-((8.*((rsxy(i1+1,i2,i3+2,axisp1,1)*jac3di(i1+1-
     & i10,i2-i20,i3+2-i30))-(rsxy(i1-1,i2,i3+2,axisp1,1)*jac3di(i1-1-
     & i10,i2-i20,i3+2-i30)))-((rsxy(i1+2,i2,i3+2,axisp1,1)*jac3di(i1+
     & 2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,i2,i3+2,axisp1,1)*jac3di(i1-
     & 2-i10,i2-i20,i3+2-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-2,
     & axisp1,1)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-2,
     & axisp1,1)*jac3di(i1-1-i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,i3-
     & 2,axisp1,1)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,i3-
     & 2,axisp1,1)*jac3di(i1-2-i10,i2-i20,i3-2-i30))))/(12.*dr(0))))/(
     & 12.*dr(2))
                     a23rs = (8.*((8.*((rsxy(i1+1,i2,i3+1,axisp1,2)*
     & jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axisp1,2)*
     & jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,axisp1,
     & 2)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,axisp1,
     & 2)*jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*((rsxy(
     & i1+1,i2,i3-1,axisp1,2)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-(rsxy(
     & i1-1,i2,i3-1,axisp1,2)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))-((
     & rsxy(i1+2,i2,i3-1,axisp1,2)*jac3di(i1+2-i10,i2-i20,i3-1-i30))-(
     & rsxy(i1-2,i2,i3-1,axisp1,2)*jac3di(i1-2-i10,i2-i20,i3-1-i30))))
     & /(12.*dr(0)))-((8.*((rsxy(i1+1,i2,i3+2,axisp1,2)*jac3di(i1+1-
     & i10,i2-i20,i3+2-i30))-(rsxy(i1-1,i2,i3+2,axisp1,2)*jac3di(i1-1-
     & i10,i2-i20,i3+2-i30)))-((rsxy(i1+2,i2,i3+2,axisp1,2)*jac3di(i1+
     & 2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,i2,i3+2,axisp1,2)*jac3di(i1-
     & 2-i10,i2-i20,i3+2-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-2,
     & axisp1,2)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-2,
     & axisp1,2)*jac3di(i1-1-i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,i3-
     & 2,axisp1,2)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,i3-
     & 2,axisp1,2)*jac3di(i1-2-i10,i2-i20,i3-2-i30))))/(12.*dr(0))))/(
     & 12.*dr(2))
                     a31rs = (8.*((8.*((rsxy(i1+1,i2,i3+1,axisp2,0)*
     & jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axisp2,0)*
     & jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,axisp2,
     & 0)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,axisp2,
     & 0)*jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*((rsxy(
     & i1+1,i2,i3-1,axisp2,0)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-(rsxy(
     & i1-1,i2,i3-1,axisp2,0)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))-((
     & rsxy(i1+2,i2,i3-1,axisp2,0)*jac3di(i1+2-i10,i2-i20,i3-1-i30))-(
     & rsxy(i1-2,i2,i3-1,axisp2,0)*jac3di(i1-2-i10,i2-i20,i3-1-i30))))
     & /(12.*dr(0)))-((8.*((rsxy(i1+1,i2,i3+2,axisp2,0)*jac3di(i1+1-
     & i10,i2-i20,i3+2-i30))-(rsxy(i1-1,i2,i3+2,axisp2,0)*jac3di(i1-1-
     & i10,i2-i20,i3+2-i30)))-((rsxy(i1+2,i2,i3+2,axisp2,0)*jac3di(i1+
     & 2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,i2,i3+2,axisp2,0)*jac3di(i1-
     & 2-i10,i2-i20,i3+2-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-2,
     & axisp2,0)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-2,
     & axisp2,0)*jac3di(i1-1-i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,i3-
     & 2,axisp2,0)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,i3-
     & 2,axisp2,0)*jac3di(i1-2-i10,i2-i20,i3-2-i30))))/(12.*dr(0))))/(
     & 12.*dr(2))
                     a32rs = (8.*((8.*((rsxy(i1+1,i2,i3+1,axisp2,1)*
     & jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axisp2,1)*
     & jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,axisp2,
     & 1)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,axisp2,
     & 1)*jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*((rsxy(
     & i1+1,i2,i3-1,axisp2,1)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-(rsxy(
     & i1-1,i2,i3-1,axisp2,1)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))-((
     & rsxy(i1+2,i2,i3-1,axisp2,1)*jac3di(i1+2-i10,i2-i20,i3-1-i30))-(
     & rsxy(i1-2,i2,i3-1,axisp2,1)*jac3di(i1-2-i10,i2-i20,i3-1-i30))))
     & /(12.*dr(0)))-((8.*((rsxy(i1+1,i2,i3+2,axisp2,1)*jac3di(i1+1-
     & i10,i2-i20,i3+2-i30))-(rsxy(i1-1,i2,i3+2,axisp2,1)*jac3di(i1-1-
     & i10,i2-i20,i3+2-i30)))-((rsxy(i1+2,i2,i3+2,axisp2,1)*jac3di(i1+
     & 2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,i2,i3+2,axisp2,1)*jac3di(i1-
     & 2-i10,i2-i20,i3+2-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-2,
     & axisp2,1)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-2,
     & axisp2,1)*jac3di(i1-1-i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,i3-
     & 2,axisp2,1)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,i3-
     & 2,axisp2,1)*jac3di(i1-2-i10,i2-i20,i3-2-i30))))/(12.*dr(0))))/(
     & 12.*dr(2))
                     a33rs = (8.*((8.*((rsxy(i1+1,i2,i3+1,axisp2,2)*
     & jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axisp2,2)*
     & jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,axisp2,
     & 2)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,axisp2,
     & 2)*jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*((rsxy(
     & i1+1,i2,i3-1,axisp2,2)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-(rsxy(
     & i1-1,i2,i3-1,axisp2,2)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))-((
     & rsxy(i1+2,i2,i3-1,axisp2,2)*jac3di(i1+2-i10,i2-i20,i3-1-i30))-(
     & rsxy(i1-2,i2,i3-1,axisp2,2)*jac3di(i1-2-i10,i2-i20,i3-1-i30))))
     & /(12.*dr(0)))-((8.*((rsxy(i1+1,i2,i3+2,axisp2,2)*jac3di(i1+1-
     & i10,i2-i20,i3+2-i30))-(rsxy(i1-1,i2,i3+2,axisp2,2)*jac3di(i1-1-
     & i10,i2-i20,i3+2-i30)))-((rsxy(i1+2,i2,i3+2,axisp2,2)*jac3di(i1+
     & 2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,i2,i3+2,axisp2,2)*jac3di(i1-
     & 2-i10,i2-i20,i3+2-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-2,
     & axisp2,2)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-2,
     & axisp2,2)*jac3di(i1-1-i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,i3-
     & 2,axisp2,2)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,i3-
     & 2,axisp2,2)*jac3di(i1-2-i10,i2-i20,i3-2-i30))))/(12.*dr(0))))/(
     & 12.*dr(2))
                     a11rs = (8.*((8.*((rsxy(i1+1,i2,i3+1,axis,0)*
     & jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axis,0)*
     & jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,axis,0)*
     & jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,axis,0)*
     & jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*((rsxy(i1+
     & 1,i2,i3-1,axis,0)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-(rsxy(i1-1,
     & i2,i3-1,axis,0)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))-((rsxy(i1+2,
     & i2,i3-1,axis,0)*jac3di(i1+2-i10,i2-i20,i3-1-i30))-(rsxy(i1-2,
     & i2,i3-1,axis,0)*jac3di(i1-2-i10,i2-i20,i3-1-i30))))/(12.*dr(0))
     & )-((8.*((rsxy(i1+1,i2,i3+2,axis,0)*jac3di(i1+1-i10,i2-i20,i3+2-
     & i30))-(rsxy(i1-1,i2,i3+2,axis,0)*jac3di(i1-1-i10,i2-i20,i3+2-
     & i30)))-((rsxy(i1+2,i2,i3+2,axis,0)*jac3di(i1+2-i10,i2-i20,i3+2-
     & i30))-(rsxy(i1-2,i2,i3+2,axis,0)*jac3di(i1-2-i10,i2-i20,i3+2-
     & i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-2,axis,0)*jac3di(i1+
     & 1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-2,axis,0)*jac3di(i1-1-
     & i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,i3-2,axis,0)*jac3di(i1+2-
     & i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,i3-2,axis,0)*jac3di(i1-2-
     & i10,i2-i20,i3-2-i30))))/(12.*dr(0))))/(12.*dr(2))
                     a12rs = (8.*((8.*((rsxy(i1+1,i2,i3+1,axis,1)*
     & jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axis,1)*
     & jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,axis,1)*
     & jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,axis,1)*
     & jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*((rsxy(i1+
     & 1,i2,i3-1,axis,1)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-(rsxy(i1-1,
     & i2,i3-1,axis,1)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))-((rsxy(i1+2,
     & i2,i3-1,axis,1)*jac3di(i1+2-i10,i2-i20,i3-1-i30))-(rsxy(i1-2,
     & i2,i3-1,axis,1)*jac3di(i1-2-i10,i2-i20,i3-1-i30))))/(12.*dr(0))
     & )-((8.*((rsxy(i1+1,i2,i3+2,axis,1)*jac3di(i1+1-i10,i2-i20,i3+2-
     & i30))-(rsxy(i1-1,i2,i3+2,axis,1)*jac3di(i1-1-i10,i2-i20,i3+2-
     & i30)))-((rsxy(i1+2,i2,i3+2,axis,1)*jac3di(i1+2-i10,i2-i20,i3+2-
     & i30))-(rsxy(i1-2,i2,i3+2,axis,1)*jac3di(i1-2-i10,i2-i20,i3+2-
     & i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-2,axis,1)*jac3di(i1+
     & 1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-2,axis,1)*jac3di(i1-1-
     & i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,i3-2,axis,1)*jac3di(i1+2-
     & i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,i3-2,axis,1)*jac3di(i1-2-
     & i10,i2-i20,i3-2-i30))))/(12.*dr(0))))/(12.*dr(2))
                     a13rs = (8.*((8.*((rsxy(i1+1,i2,i3+1,axis,2)*
     & jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axis,2)*
     & jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,axis,2)*
     & jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,axis,2)*
     & jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*((rsxy(i1+
     & 1,i2,i3-1,axis,2)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-(rsxy(i1-1,
     & i2,i3-1,axis,2)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))-((rsxy(i1+2,
     & i2,i3-1,axis,2)*jac3di(i1+2-i10,i2-i20,i3-1-i30))-(rsxy(i1-2,
     & i2,i3-1,axis,2)*jac3di(i1-2-i10,i2-i20,i3-1-i30))))/(12.*dr(0))
     & )-((8.*((rsxy(i1+1,i2,i3+2,axis,2)*jac3di(i1+1-i10,i2-i20,i3+2-
     & i30))-(rsxy(i1-1,i2,i3+2,axis,2)*jac3di(i1-1-i10,i2-i20,i3+2-
     & i30)))-((rsxy(i1+2,i2,i3+2,axis,2)*jac3di(i1+2-i10,i2-i20,i3+2-
     & i30))-(rsxy(i1-2,i2,i3+2,axis,2)*jac3di(i1-2-i10,i2-i20,i3+2-
     & i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-2,axis,2)*jac3di(i1+
     & 1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-2,axis,2)*jac3di(i1-1-
     & i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,i3-2,axis,2)*jac3di(i1+2-
     & i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,i3-2,axis,2)*jac3di(i1-2-
     & i10,i2-i20,i3-2-i30))))/(12.*dr(0))))/(12.*dr(2))
                     a21rt = (8.*((8.*((rsxy(i1,i2+1,i3+1,axisp1,0)*
     & jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axisp1,0)*
     & jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,axisp1,
     & 0)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,axisp1,
     & 0)*jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*((rsxy(
     & i1,i2+1,i3-1,axisp1,0)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-(rsxy(
     & i1,i2-1,i3-1,axisp1,0)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))-((
     & rsxy(i1,i2+2,i3-1,axisp1,0)*jac3di(i1-i10,i2+2-i20,i3-1-i30))-(
     & rsxy(i1,i2-2,i3-1,axisp1,0)*jac3di(i1-i10,i2-2-i20,i3-1-i30))))
     & /(12.*dr(1)))-((8.*((rsxy(i1,i2+1,i3+2,axisp1,0)*jac3di(i1-i10,
     & i2+1-i20,i3+2-i30))-(rsxy(i1,i2-1,i3+2,axisp1,0)*jac3di(i1-i10,
     & i2-1-i20,i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axisp1,0)*jac3di(i1-
     & i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp1,0)*jac3di(i1-
     & i10,i2-2-i20,i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-2,
     & axisp1,0)*jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-2,
     & axisp1,0)*jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,i3-
     & 2,axisp1,0)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,i3-
     & 2,axisp1,0)*jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1))))/(
     & 12.*dr(2))
                     a22rt = (8.*((8.*((rsxy(i1,i2+1,i3+1,axisp1,1)*
     & jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axisp1,1)*
     & jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,axisp1,
     & 1)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,axisp1,
     & 1)*jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*((rsxy(
     & i1,i2+1,i3-1,axisp1,1)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-(rsxy(
     & i1,i2-1,i3-1,axisp1,1)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))-((
     & rsxy(i1,i2+2,i3-1,axisp1,1)*jac3di(i1-i10,i2+2-i20,i3-1-i30))-(
     & rsxy(i1,i2-2,i3-1,axisp1,1)*jac3di(i1-i10,i2-2-i20,i3-1-i30))))
     & /(12.*dr(1)))-((8.*((rsxy(i1,i2+1,i3+2,axisp1,1)*jac3di(i1-i10,
     & i2+1-i20,i3+2-i30))-(rsxy(i1,i2-1,i3+2,axisp1,1)*jac3di(i1-i10,
     & i2-1-i20,i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axisp1,1)*jac3di(i1-
     & i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp1,1)*jac3di(i1-
     & i10,i2-2-i20,i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-2,
     & axisp1,1)*jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-2,
     & axisp1,1)*jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,i3-
     & 2,axisp1,1)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,i3-
     & 2,axisp1,1)*jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1))))/(
     & 12.*dr(2))
                     a23rt = (8.*((8.*((rsxy(i1,i2+1,i3+1,axisp1,2)*
     & jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axisp1,2)*
     & jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,axisp1,
     & 2)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,axisp1,
     & 2)*jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*((rsxy(
     & i1,i2+1,i3-1,axisp1,2)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-(rsxy(
     & i1,i2-1,i3-1,axisp1,2)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))-((
     & rsxy(i1,i2+2,i3-1,axisp1,2)*jac3di(i1-i10,i2+2-i20,i3-1-i30))-(
     & rsxy(i1,i2-2,i3-1,axisp1,2)*jac3di(i1-i10,i2-2-i20,i3-1-i30))))
     & /(12.*dr(1)))-((8.*((rsxy(i1,i2+1,i3+2,axisp1,2)*jac3di(i1-i10,
     & i2+1-i20,i3+2-i30))-(rsxy(i1,i2-1,i3+2,axisp1,2)*jac3di(i1-i10,
     & i2-1-i20,i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axisp1,2)*jac3di(i1-
     & i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp1,2)*jac3di(i1-
     & i10,i2-2-i20,i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-2,
     & axisp1,2)*jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-2,
     & axisp1,2)*jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,i3-
     & 2,axisp1,2)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,i3-
     & 2,axisp1,2)*jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1))))/(
     & 12.*dr(2))
                     a31rt = (8.*((8.*((rsxy(i1,i2+1,i3+1,axisp2,0)*
     & jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axisp2,0)*
     & jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,axisp2,
     & 0)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,axisp2,
     & 0)*jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*((rsxy(
     & i1,i2+1,i3-1,axisp2,0)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-(rsxy(
     & i1,i2-1,i3-1,axisp2,0)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))-((
     & rsxy(i1,i2+2,i3-1,axisp2,0)*jac3di(i1-i10,i2+2-i20,i3-1-i30))-(
     & rsxy(i1,i2-2,i3-1,axisp2,0)*jac3di(i1-i10,i2-2-i20,i3-1-i30))))
     & /(12.*dr(1)))-((8.*((rsxy(i1,i2+1,i3+2,axisp2,0)*jac3di(i1-i10,
     & i2+1-i20,i3+2-i30))-(rsxy(i1,i2-1,i3+2,axisp2,0)*jac3di(i1-i10,
     & i2-1-i20,i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axisp2,0)*jac3di(i1-
     & i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp2,0)*jac3di(i1-
     & i10,i2-2-i20,i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-2,
     & axisp2,0)*jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-2,
     & axisp2,0)*jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,i3-
     & 2,axisp2,0)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,i3-
     & 2,axisp2,0)*jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1))))/(
     & 12.*dr(2))
                     a32rt = (8.*((8.*((rsxy(i1,i2+1,i3+1,axisp2,1)*
     & jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axisp2,1)*
     & jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,axisp2,
     & 1)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,axisp2,
     & 1)*jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*((rsxy(
     & i1,i2+1,i3-1,axisp2,1)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-(rsxy(
     & i1,i2-1,i3-1,axisp2,1)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))-((
     & rsxy(i1,i2+2,i3-1,axisp2,1)*jac3di(i1-i10,i2+2-i20,i3-1-i30))-(
     & rsxy(i1,i2-2,i3-1,axisp2,1)*jac3di(i1-i10,i2-2-i20,i3-1-i30))))
     & /(12.*dr(1)))-((8.*((rsxy(i1,i2+1,i3+2,axisp2,1)*jac3di(i1-i10,
     & i2+1-i20,i3+2-i30))-(rsxy(i1,i2-1,i3+2,axisp2,1)*jac3di(i1-i10,
     & i2-1-i20,i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axisp2,1)*jac3di(i1-
     & i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp2,1)*jac3di(i1-
     & i10,i2-2-i20,i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-2,
     & axisp2,1)*jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-2,
     & axisp2,1)*jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,i3-
     & 2,axisp2,1)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,i3-
     & 2,axisp2,1)*jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1))))/(
     & 12.*dr(2))
                     a33rt = (8.*((8.*((rsxy(i1,i2+1,i3+1,axisp2,2)*
     & jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axisp2,2)*
     & jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,axisp2,
     & 2)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,axisp2,
     & 2)*jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*((rsxy(
     & i1,i2+1,i3-1,axisp2,2)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-(rsxy(
     & i1,i2-1,i3-1,axisp2,2)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))-((
     & rsxy(i1,i2+2,i3-1,axisp2,2)*jac3di(i1-i10,i2+2-i20,i3-1-i30))-(
     & rsxy(i1,i2-2,i3-1,axisp2,2)*jac3di(i1-i10,i2-2-i20,i3-1-i30))))
     & /(12.*dr(1)))-((8.*((rsxy(i1,i2+1,i3+2,axisp2,2)*jac3di(i1-i10,
     & i2+1-i20,i3+2-i30))-(rsxy(i1,i2-1,i3+2,axisp2,2)*jac3di(i1-i10,
     & i2-1-i20,i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axisp2,2)*jac3di(i1-
     & i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp2,2)*jac3di(i1-
     & i10,i2-2-i20,i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-2,
     & axisp2,2)*jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-2,
     & axisp2,2)*jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,i3-
     & 2,axisp2,2)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,i3-
     & 2,axisp2,2)*jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1))))/(
     & 12.*dr(2))
                     a11st = (8.*((8.*((rsxy(i1+1,i2+1,i3,axis,0)*
     & jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axis,0)*
     & jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,axis,0)*
     & jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,axis,0)*
     & jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+
     & 1,i2-1,i3,axis,0)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-(rsxy(i1-1,
     & i2-1,i3,axis,0)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))-((rsxy(i1+2,
     & i2-1,i3,axis,0)*jac3di(i1+2-i10,i2-1-i20,i3-i30))-(rsxy(i1-2,
     & i2-1,i3,axis,0)*jac3di(i1-2-i10,i2-1-i20,i3-i30))))/(12.*dr(0))
     & )-((8.*((rsxy(i1+1,i2+2,i3,axis,0)*jac3di(i1+1-i10,i2+2-i20,i3-
     & i30))-(rsxy(i1-1,i2+2,i3,axis,0)*jac3di(i1-1-i10,i2+2-i20,i3-
     & i30)))-((rsxy(i1+2,i2+2,i3,axis,0)*jac3di(i1+2-i10,i2+2-i20,i3-
     & i30))-(rsxy(i1-2,i2+2,i3,axis,0)*jac3di(i1-2-i10,i2+2-i20,i3-
     & i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,i3,axis,0)*jac3di(i1+
     & 1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,i3,axis,0)*jac3di(i1-1-
     & i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-2,i3,axis,0)*jac3di(i1+2-
     & i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-2,i3,axis,0)*jac3di(i1-2-
     & i10,i2-2-i20,i3-i30))))/(12.*dr(0))))/(12.*dr(1))
                     a12st = (8.*((8.*((rsxy(i1+1,i2+1,i3,axis,1)*
     & jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axis,1)*
     & jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,axis,1)*
     & jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,axis,1)*
     & jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+
     & 1,i2-1,i3,axis,1)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-(rsxy(i1-1,
     & i2-1,i3,axis,1)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))-((rsxy(i1+2,
     & i2-1,i3,axis,1)*jac3di(i1+2-i10,i2-1-i20,i3-i30))-(rsxy(i1-2,
     & i2-1,i3,axis,1)*jac3di(i1-2-i10,i2-1-i20,i3-i30))))/(12.*dr(0))
     & )-((8.*((rsxy(i1+1,i2+2,i3,axis,1)*jac3di(i1+1-i10,i2+2-i20,i3-
     & i30))-(rsxy(i1-1,i2+2,i3,axis,1)*jac3di(i1-1-i10,i2+2-i20,i3-
     & i30)))-((rsxy(i1+2,i2+2,i3,axis,1)*jac3di(i1+2-i10,i2+2-i20,i3-
     & i30))-(rsxy(i1-2,i2+2,i3,axis,1)*jac3di(i1-2-i10,i2+2-i20,i3-
     & i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,i3,axis,1)*jac3di(i1+
     & 1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,i3,axis,1)*jac3di(i1-1-
     & i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-2,i3,axis,1)*jac3di(i1+2-
     & i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-2,i3,axis,1)*jac3di(i1-2-
     & i10,i2-2-i20,i3-i30))))/(12.*dr(0))))/(12.*dr(1))
                     a13st = (8.*((8.*((rsxy(i1+1,i2+1,i3,axis,2)*
     & jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axis,2)*
     & jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,axis,2)*
     & jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,axis,2)*
     & jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+
     & 1,i2-1,i3,axis,2)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-(rsxy(i1-1,
     & i2-1,i3,axis,2)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))-((rsxy(i1+2,
     & i2-1,i3,axis,2)*jac3di(i1+2-i10,i2-1-i20,i3-i30))-(rsxy(i1-2,
     & i2-1,i3,axis,2)*jac3di(i1-2-i10,i2-1-i20,i3-i30))))/(12.*dr(0))
     & )-((8.*((rsxy(i1+1,i2+2,i3,axis,2)*jac3di(i1+1-i10,i2+2-i20,i3-
     & i30))-(rsxy(i1-1,i2+2,i3,axis,2)*jac3di(i1-1-i10,i2+2-i20,i3-
     & i30)))-((rsxy(i1+2,i2+2,i3,axis,2)*jac3di(i1+2-i10,i2+2-i20,i3-
     & i30))-(rsxy(i1-2,i2+2,i3,axis,2)*jac3di(i1-2-i10,i2+2-i20,i3-
     & i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,i3,axis,2)*jac3di(i1+
     & 1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,i3,axis,2)*jac3di(i1-1-
     & i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-2,i3,axis,2)*jac3di(i1+2-
     & i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-2,i3,axis,2)*jac3di(i1-2-
     & i10,i2-2-i20,i3-i30))))/(12.*dr(0))))/(12.*dr(1))
                     a21st = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp1,0)*
     & jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axisp1,0)*
     & jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,axisp1,
     & 0)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,axisp1,
     & 0)*jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(
     & i1+1,i2-1,i3,axisp1,0)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-(rsxy(
     & i1-1,i2-1,i3,axisp1,0)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))-((
     & rsxy(i1+2,i2-1,i3,axisp1,0)*jac3di(i1+2-i10,i2-1-i20,i3-i30))-(
     & rsxy(i1-2,i2-1,i3,axisp1,0)*jac3di(i1-2-i10,i2-1-i20,i3-i30))))
     & /(12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp1,0)*jac3di(i1+1-
     & i10,i2+2-i20,i3-i30))-(rsxy(i1-1,i2+2,i3,axisp1,0)*jac3di(i1-1-
     & i10,i2+2-i20,i3-i30)))-((rsxy(i1+2,i2+2,i3,axisp1,0)*jac3di(i1+
     & 2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,i2+2,i3,axisp1,0)*jac3di(i1-
     & 2-i10,i2+2-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,i3,
     & axisp1,0)*jac3di(i1+1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,i3,
     & axisp1,0)*jac3di(i1-1-i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-2,
     & i3,axisp1,0)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-2,
     & i3,axisp1,0)*jac3di(i1-2-i10,i2-2-i20,i3-i30))))/(12.*dr(0))))
     & /(12.*dr(1))
                     a22st = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp1,1)*
     & jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axisp1,1)*
     & jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,axisp1,
     & 1)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,axisp1,
     & 1)*jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(
     & i1+1,i2-1,i3,axisp1,1)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-(rsxy(
     & i1-1,i2-1,i3,axisp1,1)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))-((
     & rsxy(i1+2,i2-1,i3,axisp1,1)*jac3di(i1+2-i10,i2-1-i20,i3-i30))-(
     & rsxy(i1-2,i2-1,i3,axisp1,1)*jac3di(i1-2-i10,i2-1-i20,i3-i30))))
     & /(12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp1,1)*jac3di(i1+1-
     & i10,i2+2-i20,i3-i30))-(rsxy(i1-1,i2+2,i3,axisp1,1)*jac3di(i1-1-
     & i10,i2+2-i20,i3-i30)))-((rsxy(i1+2,i2+2,i3,axisp1,1)*jac3di(i1+
     & 2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,i2+2,i3,axisp1,1)*jac3di(i1-
     & 2-i10,i2+2-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,i3,
     & axisp1,1)*jac3di(i1+1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,i3,
     & axisp1,1)*jac3di(i1-1-i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-2,
     & i3,axisp1,1)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-2,
     & i3,axisp1,1)*jac3di(i1-2-i10,i2-2-i20,i3-i30))))/(12.*dr(0))))
     & /(12.*dr(1))
                     a23st = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp1,2)*
     & jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axisp1,2)*
     & jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,axisp1,
     & 2)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,axisp1,
     & 2)*jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(
     & i1+1,i2-1,i3,axisp1,2)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-(rsxy(
     & i1-1,i2-1,i3,axisp1,2)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))-((
     & rsxy(i1+2,i2-1,i3,axisp1,2)*jac3di(i1+2-i10,i2-1-i20,i3-i30))-(
     & rsxy(i1-2,i2-1,i3,axisp1,2)*jac3di(i1-2-i10,i2-1-i20,i3-i30))))
     & /(12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp1,2)*jac3di(i1+1-
     & i10,i2+2-i20,i3-i30))-(rsxy(i1-1,i2+2,i3,axisp1,2)*jac3di(i1-1-
     & i10,i2+2-i20,i3-i30)))-((rsxy(i1+2,i2+2,i3,axisp1,2)*jac3di(i1+
     & 2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,i2+2,i3,axisp1,2)*jac3di(i1-
     & 2-i10,i2+2-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,i3,
     & axisp1,2)*jac3di(i1+1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,i3,
     & axisp1,2)*jac3di(i1-1-i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-2,
     & i3,axisp1,2)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-2,
     & i3,axisp1,2)*jac3di(i1-2-i10,i2-2-i20,i3-i30))))/(12.*dr(0))))
     & /(12.*dr(1))
                     a31st = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp2,0)*
     & jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axisp2,0)*
     & jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,axisp2,
     & 0)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,axisp2,
     & 0)*jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(
     & i1+1,i2-1,i3,axisp2,0)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-(rsxy(
     & i1-1,i2-1,i3,axisp2,0)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))-((
     & rsxy(i1+2,i2-1,i3,axisp2,0)*jac3di(i1+2-i10,i2-1-i20,i3-i30))-(
     & rsxy(i1-2,i2-1,i3,axisp2,0)*jac3di(i1-2-i10,i2-1-i20,i3-i30))))
     & /(12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp2,0)*jac3di(i1+1-
     & i10,i2+2-i20,i3-i30))-(rsxy(i1-1,i2+2,i3,axisp2,0)*jac3di(i1-1-
     & i10,i2+2-i20,i3-i30)))-((rsxy(i1+2,i2+2,i3,axisp2,0)*jac3di(i1+
     & 2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,i2+2,i3,axisp2,0)*jac3di(i1-
     & 2-i10,i2+2-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,i3,
     & axisp2,0)*jac3di(i1+1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,i3,
     & axisp2,0)*jac3di(i1-1-i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-2,
     & i3,axisp2,0)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-2,
     & i3,axisp2,0)*jac3di(i1-2-i10,i2-2-i20,i3-i30))))/(12.*dr(0))))
     & /(12.*dr(1))
                     a32st = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp2,1)*
     & jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axisp2,1)*
     & jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,axisp2,
     & 1)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,axisp2,
     & 1)*jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(
     & i1+1,i2-1,i3,axisp2,1)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-(rsxy(
     & i1-1,i2-1,i3,axisp2,1)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))-((
     & rsxy(i1+2,i2-1,i3,axisp2,1)*jac3di(i1+2-i10,i2-1-i20,i3-i30))-(
     & rsxy(i1-2,i2-1,i3,axisp2,1)*jac3di(i1-2-i10,i2-1-i20,i3-i30))))
     & /(12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp2,1)*jac3di(i1+1-
     & i10,i2+2-i20,i3-i30))-(rsxy(i1-1,i2+2,i3,axisp2,1)*jac3di(i1-1-
     & i10,i2+2-i20,i3-i30)))-((rsxy(i1+2,i2+2,i3,axisp2,1)*jac3di(i1+
     & 2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,i2+2,i3,axisp2,1)*jac3di(i1-
     & 2-i10,i2+2-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,i3,
     & axisp2,1)*jac3di(i1+1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,i3,
     & axisp2,1)*jac3di(i1-1-i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-2,
     & i3,axisp2,1)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-2,
     & i3,axisp2,1)*jac3di(i1-2-i10,i2-2-i20,i3-i30))))/(12.*dr(0))))
     & /(12.*dr(1))
                     a33st = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp2,2)*
     & jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axisp2,2)*
     & jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,axisp2,
     & 2)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,axisp2,
     & 2)*jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(
     & i1+1,i2-1,i3,axisp2,2)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-(rsxy(
     & i1-1,i2-1,i3,axisp2,2)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))-((
     & rsxy(i1+2,i2-1,i3,axisp2,2)*jac3di(i1+2-i10,i2-1-i20,i3-i30))-(
     & rsxy(i1-2,i2-1,i3,axisp2,2)*jac3di(i1-2-i10,i2-1-i20,i3-i30))))
     & /(12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp2,2)*jac3di(i1+1-
     & i10,i2+2-i20,i3-i30))-(rsxy(i1-1,i2+2,i3,axisp2,2)*jac3di(i1-1-
     & i10,i2+2-i20,i3-i30)))-((rsxy(i1+2,i2+2,i3,axisp2,2)*jac3di(i1+
     & 2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,i2+2,i3,axisp2,2)*jac3di(i1-
     & 2-i10,i2+2-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,i3,
     & axisp2,2)*jac3di(i1+1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,i3,
     & axisp2,2)*jac3di(i1-1-i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-2,
     & i3,axisp2,2)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-2,
     & i3,axisp2,2)*jac3di(i1-2-i10,i2-2-i20,i3-i30))))/(12.*dr(0))))
     & /(12.*dr(1))
                     a11rss = (128*(rsxy(i1+1,i2,i3+1,axis,0)*jac3di(
     & i1+1-i10,i2-i20,i3+1-i30))+240*(rsxy(i1,i2,i3-1,axis,0)*jac3di(
     & i1-i10,i2-i20,i3-1-i30))+8*(rsxy(i1+2,i2,i3-1,axis,0)*jac3di(
     & i1+2-i10,i2-i20,i3-1-i30))+8*(rsxy(i1-2,i2,i3-1,axis,0)*jac3di(
     & i1-2-i10,i2-i20,i3-1-i30))-128*(rsxy(i1-1,i2,i3-1,axis,0)*
     & jac3di(i1-1-i10,i2-i20,i3-1-i30))-240*(rsxy(i1,i2,i3+1,axis,0)*
     & jac3di(i1-i10,i2-i20,i3+1-i30))-8*(rsxy(i1+2,i2,i3+1,axis,0)*
     & jac3di(i1+2-i10,i2-i20,i3+1-i30))-8*(rsxy(i1-2,i2,i3+1,axis,0)*
     & jac3di(i1-2-i10,i2-i20,i3+1-i30))-128*(rsxy(i1+1,i2,i3-1,axis,
     & 0)*jac3di(i1+1-i10,i2-i20,i3-1-i30))+128*(rsxy(i1-1,i2,i3+1,
     & axis,0)*jac3di(i1-1-i10,i2-i20,i3+1-i30))-16*(rsxy(i1+1,i2,i3+
     & 2,axis,0)*jac3di(i1+1-i10,i2-i20,i3+2-i30))+30*(rsxy(i1,i2,i3+
     & 2,axis,0)*jac3di(i1-i10,i2-i20,i3+2-i30))-16*(rsxy(i1-1,i2,i3+
     & 2,axis,0)*jac3di(i1-1-i10,i2-i20,i3+2-i30))+(rsxy(i1+2,i2,i3+2,
     & axis,0)*jac3di(i1+2-i10,i2-i20,i3+2-i30))+(rsxy(i1-2,i2,i3+2,
     & axis,0)*jac3di(i1-2-i10,i2-i20,i3+2-i30))+16*(rsxy(i1+1,i2,i3-
     & 2,axis,0)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-30*(rsxy(i1,i2,i3-
     & 2,axis,0)*jac3di(i1-i10,i2-i20,i3-2-i30))+16*(rsxy(i1-1,i2,i3-
     & 2,axis,0)*jac3di(i1-1-i10,i2-i20,i3-2-i30))-(rsxy(i1+2,i2,i3-2,
     & axis,0)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,i3-2,
     & axis,0)*jac3di(i1-2-i10,i2-i20,i3-2-i30)))/(144.*dr(0)**2*dr(2)
     & )
                     a12rss = (128*(rsxy(i1+1,i2,i3+1,axis,1)*jac3di(
     & i1+1-i10,i2-i20,i3+1-i30))+240*(rsxy(i1,i2,i3-1,axis,1)*jac3di(
     & i1-i10,i2-i20,i3-1-i30))+8*(rsxy(i1+2,i2,i3-1,axis,1)*jac3di(
     & i1+2-i10,i2-i20,i3-1-i30))+8*(rsxy(i1-2,i2,i3-1,axis,1)*jac3di(
     & i1-2-i10,i2-i20,i3-1-i30))-128*(rsxy(i1-1,i2,i3-1,axis,1)*
     & jac3di(i1-1-i10,i2-i20,i3-1-i30))-240*(rsxy(i1,i2,i3+1,axis,1)*
     & jac3di(i1-i10,i2-i20,i3+1-i30))-8*(rsxy(i1+2,i2,i3+1,axis,1)*
     & jac3di(i1+2-i10,i2-i20,i3+1-i30))-8*(rsxy(i1-2,i2,i3+1,axis,1)*
     & jac3di(i1-2-i10,i2-i20,i3+1-i30))-128*(rsxy(i1+1,i2,i3-1,axis,
     & 1)*jac3di(i1+1-i10,i2-i20,i3-1-i30))+128*(rsxy(i1-1,i2,i3+1,
     & axis,1)*jac3di(i1-1-i10,i2-i20,i3+1-i30))-16*(rsxy(i1+1,i2,i3+
     & 2,axis,1)*jac3di(i1+1-i10,i2-i20,i3+2-i30))+30*(rsxy(i1,i2,i3+
     & 2,axis,1)*jac3di(i1-i10,i2-i20,i3+2-i30))-16*(rsxy(i1-1,i2,i3+
     & 2,axis,1)*jac3di(i1-1-i10,i2-i20,i3+2-i30))+(rsxy(i1+2,i2,i3+2,
     & axis,1)*jac3di(i1+2-i10,i2-i20,i3+2-i30))+(rsxy(i1-2,i2,i3+2,
     & axis,1)*jac3di(i1-2-i10,i2-i20,i3+2-i30))+16*(rsxy(i1+1,i2,i3-
     & 2,axis,1)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-30*(rsxy(i1,i2,i3-
     & 2,axis,1)*jac3di(i1-i10,i2-i20,i3-2-i30))+16*(rsxy(i1-1,i2,i3-
     & 2,axis,1)*jac3di(i1-1-i10,i2-i20,i3-2-i30))-(rsxy(i1+2,i2,i3-2,
     & axis,1)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,i3-2,
     & axis,1)*jac3di(i1-2-i10,i2-i20,i3-2-i30)))/(144.*dr(0)**2*dr(2)
     & )
                     a13rss = (128*(rsxy(i1+1,i2,i3+1,axis,2)*jac3di(
     & i1+1-i10,i2-i20,i3+1-i30))+240*(rsxy(i1,i2,i3-1,axis,2)*jac3di(
     & i1-i10,i2-i20,i3-1-i30))+8*(rsxy(i1+2,i2,i3-1,axis,2)*jac3di(
     & i1+2-i10,i2-i20,i3-1-i30))+8*(rsxy(i1-2,i2,i3-1,axis,2)*jac3di(
     & i1-2-i10,i2-i20,i3-1-i30))-128*(rsxy(i1-1,i2,i3-1,axis,2)*
     & jac3di(i1-1-i10,i2-i20,i3-1-i30))-240*(rsxy(i1,i2,i3+1,axis,2)*
     & jac3di(i1-i10,i2-i20,i3+1-i30))-8*(rsxy(i1+2,i2,i3+1,axis,2)*
     & jac3di(i1+2-i10,i2-i20,i3+1-i30))-8*(rsxy(i1-2,i2,i3+1,axis,2)*
     & jac3di(i1-2-i10,i2-i20,i3+1-i30))-128*(rsxy(i1+1,i2,i3-1,axis,
     & 2)*jac3di(i1+1-i10,i2-i20,i3-1-i30))+128*(rsxy(i1-1,i2,i3+1,
     & axis,2)*jac3di(i1-1-i10,i2-i20,i3+1-i30))-16*(rsxy(i1+1,i2,i3+
     & 2,axis,2)*jac3di(i1+1-i10,i2-i20,i3+2-i30))+30*(rsxy(i1,i2,i3+
     & 2,axis,2)*jac3di(i1-i10,i2-i20,i3+2-i30))-16*(rsxy(i1-1,i2,i3+
     & 2,axis,2)*jac3di(i1-1-i10,i2-i20,i3+2-i30))+(rsxy(i1+2,i2,i3+2,
     & axis,2)*jac3di(i1+2-i10,i2-i20,i3+2-i30))+(rsxy(i1-2,i2,i3+2,
     & axis,2)*jac3di(i1-2-i10,i2-i20,i3+2-i30))+16*(rsxy(i1+1,i2,i3-
     & 2,axis,2)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-30*(rsxy(i1,i2,i3-
     & 2,axis,2)*jac3di(i1-i10,i2-i20,i3-2-i30))+16*(rsxy(i1-1,i2,i3-
     & 2,axis,2)*jac3di(i1-1-i10,i2-i20,i3-2-i30))-(rsxy(i1+2,i2,i3-2,
     & axis,2)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,i3-2,
     & axis,2)*jac3di(i1-2-i10,i2-i20,i3-2-i30)))/(144.*dr(0)**2*dr(2)
     & )
                     a21rss = (128*(rsxy(i1+1,i2,i3+1,axisp1,0)*jac3di(
     & i1+1-i10,i2-i20,i3+1-i30))+240*(rsxy(i1,i2,i3-1,axisp1,0)*
     & jac3di(i1-i10,i2-i20,i3-1-i30))+8*(rsxy(i1+2,i2,i3-1,axisp1,0)*
     & jac3di(i1+2-i10,i2-i20,i3-1-i30))+8*(rsxy(i1-2,i2,i3-1,axisp1,
     & 0)*jac3di(i1-2-i10,i2-i20,i3-1-i30))-128*(rsxy(i1-1,i2,i3-1,
     & axisp1,0)*jac3di(i1-1-i10,i2-i20,i3-1-i30))-240*(rsxy(i1,i2,i3+
     & 1,axisp1,0)*jac3di(i1-i10,i2-i20,i3+1-i30))-8*(rsxy(i1+2,i2,i3+
     & 1,axisp1,0)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-8*(rsxy(i1-2,i2,
     & i3+1,axisp1,0)*jac3di(i1-2-i10,i2-i20,i3+1-i30))-128*(rsxy(i1+
     & 1,i2,i3-1,axisp1,0)*jac3di(i1+1-i10,i2-i20,i3-1-i30))+128*(
     & rsxy(i1-1,i2,i3+1,axisp1,0)*jac3di(i1-1-i10,i2-i20,i3+1-i30))-
     & 16*(rsxy(i1+1,i2,i3+2,axisp1,0)*jac3di(i1+1-i10,i2-i20,i3+2-
     & i30))+30*(rsxy(i1,i2,i3+2,axisp1,0)*jac3di(i1-i10,i2-i20,i3+2-
     & i30))-16*(rsxy(i1-1,i2,i3+2,axisp1,0)*jac3di(i1-1-i10,i2-i20,
     & i3+2-i30))+(rsxy(i1+2,i2,i3+2,axisp1,0)*jac3di(i1+2-i10,i2-i20,
     & i3+2-i30))+(rsxy(i1-2,i2,i3+2,axisp1,0)*jac3di(i1-2-i10,i2-i20,
     & i3+2-i30))+16*(rsxy(i1+1,i2,i3-2,axisp1,0)*jac3di(i1+1-i10,i2-
     & i20,i3-2-i30))-30*(rsxy(i1,i2,i3-2,axisp1,0)*jac3di(i1-i10,i2-
     & i20,i3-2-i30))+16*(rsxy(i1-1,i2,i3-2,axisp1,0)*jac3di(i1-1-i10,
     & i2-i20,i3-2-i30))-(rsxy(i1+2,i2,i3-2,axisp1,0)*jac3di(i1+2-i10,
     & i2-i20,i3-2-i30))-(rsxy(i1-2,i2,i3-2,axisp1,0)*jac3di(i1-2-i10,
     & i2-i20,i3-2-i30)))/(144.*dr(0)**2*dr(2))
                     a22rss = (128*(rsxy(i1+1,i2,i3+1,axisp1,1)*jac3di(
     & i1+1-i10,i2-i20,i3+1-i30))+240*(rsxy(i1,i2,i3-1,axisp1,1)*
     & jac3di(i1-i10,i2-i20,i3-1-i30))+8*(rsxy(i1+2,i2,i3-1,axisp1,1)*
     & jac3di(i1+2-i10,i2-i20,i3-1-i30))+8*(rsxy(i1-2,i2,i3-1,axisp1,
     & 1)*jac3di(i1-2-i10,i2-i20,i3-1-i30))-128*(rsxy(i1-1,i2,i3-1,
     & axisp1,1)*jac3di(i1-1-i10,i2-i20,i3-1-i30))-240*(rsxy(i1,i2,i3+
     & 1,axisp1,1)*jac3di(i1-i10,i2-i20,i3+1-i30))-8*(rsxy(i1+2,i2,i3+
     & 1,axisp1,1)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-8*(rsxy(i1-2,i2,
     & i3+1,axisp1,1)*jac3di(i1-2-i10,i2-i20,i3+1-i30))-128*(rsxy(i1+
     & 1,i2,i3-1,axisp1,1)*jac3di(i1+1-i10,i2-i20,i3-1-i30))+128*(
     & rsxy(i1-1,i2,i3+1,axisp1,1)*jac3di(i1-1-i10,i2-i20,i3+1-i30))-
     & 16*(rsxy(i1+1,i2,i3+2,axisp1,1)*jac3di(i1+1-i10,i2-i20,i3+2-
     & i30))+30*(rsxy(i1,i2,i3+2,axisp1,1)*jac3di(i1-i10,i2-i20,i3+2-
     & i30))-16*(rsxy(i1-1,i2,i3+2,axisp1,1)*jac3di(i1-1-i10,i2-i20,
     & i3+2-i30))+(rsxy(i1+2,i2,i3+2,axisp1,1)*jac3di(i1+2-i10,i2-i20,
     & i3+2-i30))+(rsxy(i1-2,i2,i3+2,axisp1,1)*jac3di(i1-2-i10,i2-i20,
     & i3+2-i30))+16*(rsxy(i1+1,i2,i3-2,axisp1,1)*jac3di(i1+1-i10,i2-
     & i20,i3-2-i30))-30*(rsxy(i1,i2,i3-2,axisp1,1)*jac3di(i1-i10,i2-
     & i20,i3-2-i30))+16*(rsxy(i1-1,i2,i3-2,axisp1,1)*jac3di(i1-1-i10,
     & i2-i20,i3-2-i30))-(rsxy(i1+2,i2,i3-2,axisp1,1)*jac3di(i1+2-i10,
     & i2-i20,i3-2-i30))-(rsxy(i1-2,i2,i3-2,axisp1,1)*jac3di(i1-2-i10,
     & i2-i20,i3-2-i30)))/(144.*dr(0)**2*dr(2))
                     a23rss = (128*(rsxy(i1+1,i2,i3+1,axisp1,2)*jac3di(
     & i1+1-i10,i2-i20,i3+1-i30))+240*(rsxy(i1,i2,i3-1,axisp1,2)*
     & jac3di(i1-i10,i2-i20,i3-1-i30))+8*(rsxy(i1+2,i2,i3-1,axisp1,2)*
     & jac3di(i1+2-i10,i2-i20,i3-1-i30))+8*(rsxy(i1-2,i2,i3-1,axisp1,
     & 2)*jac3di(i1-2-i10,i2-i20,i3-1-i30))-128*(rsxy(i1-1,i2,i3-1,
     & axisp1,2)*jac3di(i1-1-i10,i2-i20,i3-1-i30))-240*(rsxy(i1,i2,i3+
     & 1,axisp1,2)*jac3di(i1-i10,i2-i20,i3+1-i30))-8*(rsxy(i1+2,i2,i3+
     & 1,axisp1,2)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-8*(rsxy(i1-2,i2,
     & i3+1,axisp1,2)*jac3di(i1-2-i10,i2-i20,i3+1-i30))-128*(rsxy(i1+
     & 1,i2,i3-1,axisp1,2)*jac3di(i1+1-i10,i2-i20,i3-1-i30))+128*(
     & rsxy(i1-1,i2,i3+1,axisp1,2)*jac3di(i1-1-i10,i2-i20,i3+1-i30))-
     & 16*(rsxy(i1+1,i2,i3+2,axisp1,2)*jac3di(i1+1-i10,i2-i20,i3+2-
     & i30))+30*(rsxy(i1,i2,i3+2,axisp1,2)*jac3di(i1-i10,i2-i20,i3+2-
     & i30))-16*(rsxy(i1-1,i2,i3+2,axisp1,2)*jac3di(i1-1-i10,i2-i20,
     & i3+2-i30))+(rsxy(i1+2,i2,i3+2,axisp1,2)*jac3di(i1+2-i10,i2-i20,
     & i3+2-i30))+(rsxy(i1-2,i2,i3+2,axisp1,2)*jac3di(i1-2-i10,i2-i20,
     & i3+2-i30))+16*(rsxy(i1+1,i2,i3-2,axisp1,2)*jac3di(i1+1-i10,i2-
     & i20,i3-2-i30))-30*(rsxy(i1,i2,i3-2,axisp1,2)*jac3di(i1-i10,i2-
     & i20,i3-2-i30))+16*(rsxy(i1-1,i2,i3-2,axisp1,2)*jac3di(i1-1-i10,
     & i2-i20,i3-2-i30))-(rsxy(i1+2,i2,i3-2,axisp1,2)*jac3di(i1+2-i10,
     & i2-i20,i3-2-i30))-(rsxy(i1-2,i2,i3-2,axisp1,2)*jac3di(i1-2-i10,
     & i2-i20,i3-2-i30)))/(144.*dr(0)**2*dr(2))
                     a31rss = (128*(rsxy(i1+1,i2,i3+1,axisp2,0)*jac3di(
     & i1+1-i10,i2-i20,i3+1-i30))+240*(rsxy(i1,i2,i3-1,axisp2,0)*
     & jac3di(i1-i10,i2-i20,i3-1-i30))+8*(rsxy(i1+2,i2,i3-1,axisp2,0)*
     & jac3di(i1+2-i10,i2-i20,i3-1-i30))+8*(rsxy(i1-2,i2,i3-1,axisp2,
     & 0)*jac3di(i1-2-i10,i2-i20,i3-1-i30))-128*(rsxy(i1-1,i2,i3-1,
     & axisp2,0)*jac3di(i1-1-i10,i2-i20,i3-1-i30))-240*(rsxy(i1,i2,i3+
     & 1,axisp2,0)*jac3di(i1-i10,i2-i20,i3+1-i30))-8*(rsxy(i1+2,i2,i3+
     & 1,axisp2,0)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-8*(rsxy(i1-2,i2,
     & i3+1,axisp2,0)*jac3di(i1-2-i10,i2-i20,i3+1-i30))-128*(rsxy(i1+
     & 1,i2,i3-1,axisp2,0)*jac3di(i1+1-i10,i2-i20,i3-1-i30))+128*(
     & rsxy(i1-1,i2,i3+1,axisp2,0)*jac3di(i1-1-i10,i2-i20,i3+1-i30))-
     & 16*(rsxy(i1+1,i2,i3+2,axisp2,0)*jac3di(i1+1-i10,i2-i20,i3+2-
     & i30))+30*(rsxy(i1,i2,i3+2,axisp2,0)*jac3di(i1-i10,i2-i20,i3+2-
     & i30))-16*(rsxy(i1-1,i2,i3+2,axisp2,0)*jac3di(i1-1-i10,i2-i20,
     & i3+2-i30))+(rsxy(i1+2,i2,i3+2,axisp2,0)*jac3di(i1+2-i10,i2-i20,
     & i3+2-i30))+(rsxy(i1-2,i2,i3+2,axisp2,0)*jac3di(i1-2-i10,i2-i20,
     & i3+2-i30))+16*(rsxy(i1+1,i2,i3-2,axisp2,0)*jac3di(i1+1-i10,i2-
     & i20,i3-2-i30))-30*(rsxy(i1,i2,i3-2,axisp2,0)*jac3di(i1-i10,i2-
     & i20,i3-2-i30))+16*(rsxy(i1-1,i2,i3-2,axisp2,0)*jac3di(i1-1-i10,
     & i2-i20,i3-2-i30))-(rsxy(i1+2,i2,i3-2,axisp2,0)*jac3di(i1+2-i10,
     & i2-i20,i3-2-i30))-(rsxy(i1-2,i2,i3-2,axisp2,0)*jac3di(i1-2-i10,
     & i2-i20,i3-2-i30)))/(144.*dr(0)**2*dr(2))
                     a32rss = (128*(rsxy(i1+1,i2,i3+1,axisp2,1)*jac3di(
     & i1+1-i10,i2-i20,i3+1-i30))+240*(rsxy(i1,i2,i3-1,axisp2,1)*
     & jac3di(i1-i10,i2-i20,i3-1-i30))+8*(rsxy(i1+2,i2,i3-1,axisp2,1)*
     & jac3di(i1+2-i10,i2-i20,i3-1-i30))+8*(rsxy(i1-2,i2,i3-1,axisp2,
     & 1)*jac3di(i1-2-i10,i2-i20,i3-1-i30))-128*(rsxy(i1-1,i2,i3-1,
     & axisp2,1)*jac3di(i1-1-i10,i2-i20,i3-1-i30))-240*(rsxy(i1,i2,i3+
     & 1,axisp2,1)*jac3di(i1-i10,i2-i20,i3+1-i30))-8*(rsxy(i1+2,i2,i3+
     & 1,axisp2,1)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-8*(rsxy(i1-2,i2,
     & i3+1,axisp2,1)*jac3di(i1-2-i10,i2-i20,i3+1-i30))-128*(rsxy(i1+
     & 1,i2,i3-1,axisp2,1)*jac3di(i1+1-i10,i2-i20,i3-1-i30))+128*(
     & rsxy(i1-1,i2,i3+1,axisp2,1)*jac3di(i1-1-i10,i2-i20,i3+1-i30))-
     & 16*(rsxy(i1+1,i2,i3+2,axisp2,1)*jac3di(i1+1-i10,i2-i20,i3+2-
     & i30))+30*(rsxy(i1,i2,i3+2,axisp2,1)*jac3di(i1-i10,i2-i20,i3+2-
     & i30))-16*(rsxy(i1-1,i2,i3+2,axisp2,1)*jac3di(i1-1-i10,i2-i20,
     & i3+2-i30))+(rsxy(i1+2,i2,i3+2,axisp2,1)*jac3di(i1+2-i10,i2-i20,
     & i3+2-i30))+(rsxy(i1-2,i2,i3+2,axisp2,1)*jac3di(i1-2-i10,i2-i20,
     & i3+2-i30))+16*(rsxy(i1+1,i2,i3-2,axisp2,1)*jac3di(i1+1-i10,i2-
     & i20,i3-2-i30))-30*(rsxy(i1,i2,i3-2,axisp2,1)*jac3di(i1-i10,i2-
     & i20,i3-2-i30))+16*(rsxy(i1-1,i2,i3-2,axisp2,1)*jac3di(i1-1-i10,
     & i2-i20,i3-2-i30))-(rsxy(i1+2,i2,i3-2,axisp2,1)*jac3di(i1+2-i10,
     & i2-i20,i3-2-i30))-(rsxy(i1-2,i2,i3-2,axisp2,1)*jac3di(i1-2-i10,
     & i2-i20,i3-2-i30)))/(144.*dr(0)**2*dr(2))
                     a33rss = (128*(rsxy(i1+1,i2,i3+1,axisp2,2)*jac3di(
     & i1+1-i10,i2-i20,i3+1-i30))+240*(rsxy(i1,i2,i3-1,axisp2,2)*
     & jac3di(i1-i10,i2-i20,i3-1-i30))+8*(rsxy(i1+2,i2,i3-1,axisp2,2)*
     & jac3di(i1+2-i10,i2-i20,i3-1-i30))+8*(rsxy(i1-2,i2,i3-1,axisp2,
     & 2)*jac3di(i1-2-i10,i2-i20,i3-1-i30))-128*(rsxy(i1-1,i2,i3-1,
     & axisp2,2)*jac3di(i1-1-i10,i2-i20,i3-1-i30))-240*(rsxy(i1,i2,i3+
     & 1,axisp2,2)*jac3di(i1-i10,i2-i20,i3+1-i30))-8*(rsxy(i1+2,i2,i3+
     & 1,axisp2,2)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-8*(rsxy(i1-2,i2,
     & i3+1,axisp2,2)*jac3di(i1-2-i10,i2-i20,i3+1-i30))-128*(rsxy(i1+
     & 1,i2,i3-1,axisp2,2)*jac3di(i1+1-i10,i2-i20,i3-1-i30))+128*(
     & rsxy(i1-1,i2,i3+1,axisp2,2)*jac3di(i1-1-i10,i2-i20,i3+1-i30))-
     & 16*(rsxy(i1+1,i2,i3+2,axisp2,2)*jac3di(i1+1-i10,i2-i20,i3+2-
     & i30))+30*(rsxy(i1,i2,i3+2,axisp2,2)*jac3di(i1-i10,i2-i20,i3+2-
     & i30))-16*(rsxy(i1-1,i2,i3+2,axisp2,2)*jac3di(i1-1-i10,i2-i20,
     & i3+2-i30))+(rsxy(i1+2,i2,i3+2,axisp2,2)*jac3di(i1+2-i10,i2-i20,
     & i3+2-i30))+(rsxy(i1-2,i2,i3+2,axisp2,2)*jac3di(i1-2-i10,i2-i20,
     & i3+2-i30))+16*(rsxy(i1+1,i2,i3-2,axisp2,2)*jac3di(i1+1-i10,i2-
     & i20,i3-2-i30))-30*(rsxy(i1,i2,i3-2,axisp2,2)*jac3di(i1-i10,i2-
     & i20,i3-2-i30))+16*(rsxy(i1-1,i2,i3-2,axisp2,2)*jac3di(i1-1-i10,
     & i2-i20,i3-2-i30))-(rsxy(i1+2,i2,i3-2,axisp2,2)*jac3di(i1+2-i10,
     & i2-i20,i3-2-i30))-(rsxy(i1-2,i2,i3-2,axisp2,2)*jac3di(i1-2-i10,
     & i2-i20,i3-2-i30)))/(144.*dr(0)**2*dr(2))
                     a11rtt = (240*(rsxy(i1,i2,i3-1,axis,0)*jac3di(i1-
     & i10,i2-i20,i3-1-i30))-240*(rsxy(i1,i2,i3+1,axis,0)*jac3di(i1-
     & i10,i2-i20,i3+1-i30))-16*(rsxy(i1,i2+1,i3+2,axis,0)*jac3di(i1-
     & i10,i2+1-i20,i3+2-i30))-16*(rsxy(i1,i2-1,i3+2,axis,0)*jac3di(
     & i1-i10,i2-1-i20,i3+2-i30))+(rsxy(i1,i2+2,i3+2,axis,0)*jac3di(
     & i1-i10,i2+2-i20,i3+2-i30))+(rsxy(i1,i2-2,i3+2,axis,0)*jac3di(
     & i1-i10,i2-2-i20,i3+2-i30))+16*(rsxy(i1,i2+1,i3-2,axis,0)*
     & jac3di(i1-i10,i2+1-i20,i3-2-i30))+16*(rsxy(i1,i2-1,i3-2,axis,0)
     & *jac3di(i1-i10,i2-1-i20,i3-2-i30))-(rsxy(i1,i2+2,i3-2,axis,0)*
     & jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,i3-2,axis,0)*
     & jac3di(i1-i10,i2-2-i20,i3-2-i30))+30*(rsxy(i1,i2,i3+2,axis,0)*
     & jac3di(i1-i10,i2-i20,i3+2-i30))-30*(rsxy(i1,i2,i3-2,axis,0)*
     & jac3di(i1-i10,i2-i20,i3-2-i30))+128*(rsxy(i1,i2+1,i3+1,axis,0)*
     & jac3di(i1-i10,i2+1-i20,i3+1-i30))+128*(rsxy(i1,i2-1,i3+1,axis,
     & 0)*jac3di(i1-i10,i2-1-i20,i3+1-i30))-8*(rsxy(i1,i2+2,i3+1,axis,
     & 0)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-8*(rsxy(i1,i2-2,i3+1,axis,
     & 0)*jac3di(i1-i10,i2-2-i20,i3+1-i30))-128*(rsxy(i1,i2+1,i3-1,
     & axis,0)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-128*(rsxy(i1,i2-1,i3-
     & 1,axis,0)*jac3di(i1-i10,i2-1-i20,i3-1-i30))+8*(rsxy(i1,i2+2,i3-
     & 1,axis,0)*jac3di(i1-i10,i2+2-i20,i3-1-i30))+8*(rsxy(i1,i2-2,i3-
     & 1,axis,0)*jac3di(i1-i10,i2-2-i20,i3-1-i30)))/(144.*dr(1)**2*dr(
     & 2))
                     a12rtt = (240*(rsxy(i1,i2,i3-1,axis,1)*jac3di(i1-
     & i10,i2-i20,i3-1-i30))-240*(rsxy(i1,i2,i3+1,axis,1)*jac3di(i1-
     & i10,i2-i20,i3+1-i30))-16*(rsxy(i1,i2+1,i3+2,axis,1)*jac3di(i1-
     & i10,i2+1-i20,i3+2-i30))-16*(rsxy(i1,i2-1,i3+2,axis,1)*jac3di(
     & i1-i10,i2-1-i20,i3+2-i30))+(rsxy(i1,i2+2,i3+2,axis,1)*jac3di(
     & i1-i10,i2+2-i20,i3+2-i30))+(rsxy(i1,i2-2,i3+2,axis,1)*jac3di(
     & i1-i10,i2-2-i20,i3+2-i30))+16*(rsxy(i1,i2+1,i3-2,axis,1)*
     & jac3di(i1-i10,i2+1-i20,i3-2-i30))+16*(rsxy(i1,i2-1,i3-2,axis,1)
     & *jac3di(i1-i10,i2-1-i20,i3-2-i30))-(rsxy(i1,i2+2,i3-2,axis,1)*
     & jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,i3-2,axis,1)*
     & jac3di(i1-i10,i2-2-i20,i3-2-i30))+30*(rsxy(i1,i2,i3+2,axis,1)*
     & jac3di(i1-i10,i2-i20,i3+2-i30))-30*(rsxy(i1,i2,i3-2,axis,1)*
     & jac3di(i1-i10,i2-i20,i3-2-i30))+128*(rsxy(i1,i2+1,i3+1,axis,1)*
     & jac3di(i1-i10,i2+1-i20,i3+1-i30))+128*(rsxy(i1,i2-1,i3+1,axis,
     & 1)*jac3di(i1-i10,i2-1-i20,i3+1-i30))-8*(rsxy(i1,i2+2,i3+1,axis,
     & 1)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-8*(rsxy(i1,i2-2,i3+1,axis,
     & 1)*jac3di(i1-i10,i2-2-i20,i3+1-i30))-128*(rsxy(i1,i2+1,i3-1,
     & axis,1)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-128*(rsxy(i1,i2-1,i3-
     & 1,axis,1)*jac3di(i1-i10,i2-1-i20,i3-1-i30))+8*(rsxy(i1,i2+2,i3-
     & 1,axis,1)*jac3di(i1-i10,i2+2-i20,i3-1-i30))+8*(rsxy(i1,i2-2,i3-
     & 1,axis,1)*jac3di(i1-i10,i2-2-i20,i3-1-i30)))/(144.*dr(1)**2*dr(
     & 2))
                     a13rtt = (240*(rsxy(i1,i2,i3-1,axis,2)*jac3di(i1-
     & i10,i2-i20,i3-1-i30))-240*(rsxy(i1,i2,i3+1,axis,2)*jac3di(i1-
     & i10,i2-i20,i3+1-i30))-16*(rsxy(i1,i2+1,i3+2,axis,2)*jac3di(i1-
     & i10,i2+1-i20,i3+2-i30))-16*(rsxy(i1,i2-1,i3+2,axis,2)*jac3di(
     & i1-i10,i2-1-i20,i3+2-i30))+(rsxy(i1,i2+2,i3+2,axis,2)*jac3di(
     & i1-i10,i2+2-i20,i3+2-i30))+(rsxy(i1,i2-2,i3+2,axis,2)*jac3di(
     & i1-i10,i2-2-i20,i3+2-i30))+16*(rsxy(i1,i2+1,i3-2,axis,2)*
     & jac3di(i1-i10,i2+1-i20,i3-2-i30))+16*(rsxy(i1,i2-1,i3-2,axis,2)
     & *jac3di(i1-i10,i2-1-i20,i3-2-i30))-(rsxy(i1,i2+2,i3-2,axis,2)*
     & jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,i3-2,axis,2)*
     & jac3di(i1-i10,i2-2-i20,i3-2-i30))+30*(rsxy(i1,i2,i3+2,axis,2)*
     & jac3di(i1-i10,i2-i20,i3+2-i30))-30*(rsxy(i1,i2,i3-2,axis,2)*
     & jac3di(i1-i10,i2-i20,i3-2-i30))+128*(rsxy(i1,i2+1,i3+1,axis,2)*
     & jac3di(i1-i10,i2+1-i20,i3+1-i30))+128*(rsxy(i1,i2-1,i3+1,axis,
     & 2)*jac3di(i1-i10,i2-1-i20,i3+1-i30))-8*(rsxy(i1,i2+2,i3+1,axis,
     & 2)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-8*(rsxy(i1,i2-2,i3+1,axis,
     & 2)*jac3di(i1-i10,i2-2-i20,i3+1-i30))-128*(rsxy(i1,i2+1,i3-1,
     & axis,2)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-128*(rsxy(i1,i2-1,i3-
     & 1,axis,2)*jac3di(i1-i10,i2-1-i20,i3-1-i30))+8*(rsxy(i1,i2+2,i3-
     & 1,axis,2)*jac3di(i1-i10,i2+2-i20,i3-1-i30))+8*(rsxy(i1,i2-2,i3-
     & 1,axis,2)*jac3di(i1-i10,i2-2-i20,i3-1-i30)))/(144.*dr(1)**2*dr(
     & 2))
                     a21rtt = (240*(rsxy(i1,i2,i3-1,axisp1,0)*jac3di(
     & i1-i10,i2-i20,i3-1-i30))-240*(rsxy(i1,i2,i3+1,axisp1,0)*jac3di(
     & i1-i10,i2-i20,i3+1-i30))-16*(rsxy(i1,i2+1,i3+2,axisp1,0)*
     & jac3di(i1-i10,i2+1-i20,i3+2-i30))-16*(rsxy(i1,i2-1,i3+2,axisp1,
     & 0)*jac3di(i1-i10,i2-1-i20,i3+2-i30))+(rsxy(i1,i2+2,i3+2,axisp1,
     & 0)*jac3di(i1-i10,i2+2-i20,i3+2-i30))+(rsxy(i1,i2-2,i3+2,axisp1,
     & 0)*jac3di(i1-i10,i2-2-i20,i3+2-i30))+16*(rsxy(i1,i2+1,i3-2,
     & axisp1,0)*jac3di(i1-i10,i2+1-i20,i3-2-i30))+16*(rsxy(i1,i2-1,
     & i3-2,axisp1,0)*jac3di(i1-i10,i2-1-i20,i3-2-i30))-(rsxy(i1,i2+2,
     & i3-2,axisp1,0)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,
     & i3-2,axisp1,0)*jac3di(i1-i10,i2-2-i20,i3-2-i30))+30*(rsxy(i1,
     & i2,i3+2,axisp1,0)*jac3di(i1-i10,i2-i20,i3+2-i30))-30*(rsxy(i1,
     & i2,i3-2,axisp1,0)*jac3di(i1-i10,i2-i20,i3-2-i30))+128*(rsxy(i1,
     & i2+1,i3+1,axisp1,0)*jac3di(i1-i10,i2+1-i20,i3+1-i30))+128*(
     & rsxy(i1,i2-1,i3+1,axisp1,0)*jac3di(i1-i10,i2-1-i20,i3+1-i30))-
     & 8*(rsxy(i1,i2+2,i3+1,axisp1,0)*jac3di(i1-i10,i2+2-i20,i3+1-i30)
     & )-8*(rsxy(i1,i2-2,i3+1,axisp1,0)*jac3di(i1-i10,i2-2-i20,i3+1-
     & i30))-128*(rsxy(i1,i2+1,i3-1,axisp1,0)*jac3di(i1-i10,i2+1-i20,
     & i3-1-i30))-128*(rsxy(i1,i2-1,i3-1,axisp1,0)*jac3di(i1-i10,i2-1-
     & i20,i3-1-i30))+8*(rsxy(i1,i2+2,i3-1,axisp1,0)*jac3di(i1-i10,i2+
     & 2-i20,i3-1-i30))+8*(rsxy(i1,i2-2,i3-1,axisp1,0)*jac3di(i1-i10,
     & i2-2-i20,i3-1-i30)))/(144.*dr(1)**2*dr(2))
                     a22rtt = (240*(rsxy(i1,i2,i3-1,axisp1,1)*jac3di(
     & i1-i10,i2-i20,i3-1-i30))-240*(rsxy(i1,i2,i3+1,axisp1,1)*jac3di(
     & i1-i10,i2-i20,i3+1-i30))-16*(rsxy(i1,i2+1,i3+2,axisp1,1)*
     & jac3di(i1-i10,i2+1-i20,i3+2-i30))-16*(rsxy(i1,i2-1,i3+2,axisp1,
     & 1)*jac3di(i1-i10,i2-1-i20,i3+2-i30))+(rsxy(i1,i2+2,i3+2,axisp1,
     & 1)*jac3di(i1-i10,i2+2-i20,i3+2-i30))+(rsxy(i1,i2-2,i3+2,axisp1,
     & 1)*jac3di(i1-i10,i2-2-i20,i3+2-i30))+16*(rsxy(i1,i2+1,i3-2,
     & axisp1,1)*jac3di(i1-i10,i2+1-i20,i3-2-i30))+16*(rsxy(i1,i2-1,
     & i3-2,axisp1,1)*jac3di(i1-i10,i2-1-i20,i3-2-i30))-(rsxy(i1,i2+2,
     & i3-2,axisp1,1)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,
     & i3-2,axisp1,1)*jac3di(i1-i10,i2-2-i20,i3-2-i30))+30*(rsxy(i1,
     & i2,i3+2,axisp1,1)*jac3di(i1-i10,i2-i20,i3+2-i30))-30*(rsxy(i1,
     & i2,i3-2,axisp1,1)*jac3di(i1-i10,i2-i20,i3-2-i30))+128*(rsxy(i1,
     & i2+1,i3+1,axisp1,1)*jac3di(i1-i10,i2+1-i20,i3+1-i30))+128*(
     & rsxy(i1,i2-1,i3+1,axisp1,1)*jac3di(i1-i10,i2-1-i20,i3+1-i30))-
     & 8*(rsxy(i1,i2+2,i3+1,axisp1,1)*jac3di(i1-i10,i2+2-i20,i3+1-i30)
     & )-8*(rsxy(i1,i2-2,i3+1,axisp1,1)*jac3di(i1-i10,i2-2-i20,i3+1-
     & i30))-128*(rsxy(i1,i2+1,i3-1,axisp1,1)*jac3di(i1-i10,i2+1-i20,
     & i3-1-i30))-128*(rsxy(i1,i2-1,i3-1,axisp1,1)*jac3di(i1-i10,i2-1-
     & i20,i3-1-i30))+8*(rsxy(i1,i2+2,i3-1,axisp1,1)*jac3di(i1-i10,i2+
     & 2-i20,i3-1-i30))+8*(rsxy(i1,i2-2,i3-1,axisp1,1)*jac3di(i1-i10,
     & i2-2-i20,i3-1-i30)))/(144.*dr(1)**2*dr(2))
                     a23rtt = (240*(rsxy(i1,i2,i3-1,axisp1,2)*jac3di(
     & i1-i10,i2-i20,i3-1-i30))-240*(rsxy(i1,i2,i3+1,axisp1,2)*jac3di(
     & i1-i10,i2-i20,i3+1-i30))-16*(rsxy(i1,i2+1,i3+2,axisp1,2)*
     & jac3di(i1-i10,i2+1-i20,i3+2-i30))-16*(rsxy(i1,i2-1,i3+2,axisp1,
     & 2)*jac3di(i1-i10,i2-1-i20,i3+2-i30))+(rsxy(i1,i2+2,i3+2,axisp1,
     & 2)*jac3di(i1-i10,i2+2-i20,i3+2-i30))+(rsxy(i1,i2-2,i3+2,axisp1,
     & 2)*jac3di(i1-i10,i2-2-i20,i3+2-i30))+16*(rsxy(i1,i2+1,i3-2,
     & axisp1,2)*jac3di(i1-i10,i2+1-i20,i3-2-i30))+16*(rsxy(i1,i2-1,
     & i3-2,axisp1,2)*jac3di(i1-i10,i2-1-i20,i3-2-i30))-(rsxy(i1,i2+2,
     & i3-2,axisp1,2)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,
     & i3-2,axisp1,2)*jac3di(i1-i10,i2-2-i20,i3-2-i30))+30*(rsxy(i1,
     & i2,i3+2,axisp1,2)*jac3di(i1-i10,i2-i20,i3+2-i30))-30*(rsxy(i1,
     & i2,i3-2,axisp1,2)*jac3di(i1-i10,i2-i20,i3-2-i30))+128*(rsxy(i1,
     & i2+1,i3+1,axisp1,2)*jac3di(i1-i10,i2+1-i20,i3+1-i30))+128*(
     & rsxy(i1,i2-1,i3+1,axisp1,2)*jac3di(i1-i10,i2-1-i20,i3+1-i30))-
     & 8*(rsxy(i1,i2+2,i3+1,axisp1,2)*jac3di(i1-i10,i2+2-i20,i3+1-i30)
     & )-8*(rsxy(i1,i2-2,i3+1,axisp1,2)*jac3di(i1-i10,i2-2-i20,i3+1-
     & i30))-128*(rsxy(i1,i2+1,i3-1,axisp1,2)*jac3di(i1-i10,i2+1-i20,
     & i3-1-i30))-128*(rsxy(i1,i2-1,i3-1,axisp1,2)*jac3di(i1-i10,i2-1-
     & i20,i3-1-i30))+8*(rsxy(i1,i2+2,i3-1,axisp1,2)*jac3di(i1-i10,i2+
     & 2-i20,i3-1-i30))+8*(rsxy(i1,i2-2,i3-1,axisp1,2)*jac3di(i1-i10,
     & i2-2-i20,i3-1-i30)))/(144.*dr(1)**2*dr(2))
                     a31rtt = (240*(rsxy(i1,i2,i3-1,axisp2,0)*jac3di(
     & i1-i10,i2-i20,i3-1-i30))-240*(rsxy(i1,i2,i3+1,axisp2,0)*jac3di(
     & i1-i10,i2-i20,i3+1-i30))-16*(rsxy(i1,i2+1,i3+2,axisp2,0)*
     & jac3di(i1-i10,i2+1-i20,i3+2-i30))-16*(rsxy(i1,i2-1,i3+2,axisp2,
     & 0)*jac3di(i1-i10,i2-1-i20,i3+2-i30))+(rsxy(i1,i2+2,i3+2,axisp2,
     & 0)*jac3di(i1-i10,i2+2-i20,i3+2-i30))+(rsxy(i1,i2-2,i3+2,axisp2,
     & 0)*jac3di(i1-i10,i2-2-i20,i3+2-i30))+16*(rsxy(i1,i2+1,i3-2,
     & axisp2,0)*jac3di(i1-i10,i2+1-i20,i3-2-i30))+16*(rsxy(i1,i2-1,
     & i3-2,axisp2,0)*jac3di(i1-i10,i2-1-i20,i3-2-i30))-(rsxy(i1,i2+2,
     & i3-2,axisp2,0)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,
     & i3-2,axisp2,0)*jac3di(i1-i10,i2-2-i20,i3-2-i30))+30*(rsxy(i1,
     & i2,i3+2,axisp2,0)*jac3di(i1-i10,i2-i20,i3+2-i30))-30*(rsxy(i1,
     & i2,i3-2,axisp2,0)*jac3di(i1-i10,i2-i20,i3-2-i30))+128*(rsxy(i1,
     & i2+1,i3+1,axisp2,0)*jac3di(i1-i10,i2+1-i20,i3+1-i30))+128*(
     & rsxy(i1,i2-1,i3+1,axisp2,0)*jac3di(i1-i10,i2-1-i20,i3+1-i30))-
     & 8*(rsxy(i1,i2+2,i3+1,axisp2,0)*jac3di(i1-i10,i2+2-i20,i3+1-i30)
     & )-8*(rsxy(i1,i2-2,i3+1,axisp2,0)*jac3di(i1-i10,i2-2-i20,i3+1-
     & i30))-128*(rsxy(i1,i2+1,i3-1,axisp2,0)*jac3di(i1-i10,i2+1-i20,
     & i3-1-i30))-128*(rsxy(i1,i2-1,i3-1,axisp2,0)*jac3di(i1-i10,i2-1-
     & i20,i3-1-i30))+8*(rsxy(i1,i2+2,i3-1,axisp2,0)*jac3di(i1-i10,i2+
     & 2-i20,i3-1-i30))+8*(rsxy(i1,i2-2,i3-1,axisp2,0)*jac3di(i1-i10,
     & i2-2-i20,i3-1-i30)))/(144.*dr(1)**2*dr(2))
                     a32rtt = (240*(rsxy(i1,i2,i3-1,axisp2,1)*jac3di(
     & i1-i10,i2-i20,i3-1-i30))-240*(rsxy(i1,i2,i3+1,axisp2,1)*jac3di(
     & i1-i10,i2-i20,i3+1-i30))-16*(rsxy(i1,i2+1,i3+2,axisp2,1)*
     & jac3di(i1-i10,i2+1-i20,i3+2-i30))-16*(rsxy(i1,i2-1,i3+2,axisp2,
     & 1)*jac3di(i1-i10,i2-1-i20,i3+2-i30))+(rsxy(i1,i2+2,i3+2,axisp2,
     & 1)*jac3di(i1-i10,i2+2-i20,i3+2-i30))+(rsxy(i1,i2-2,i3+2,axisp2,
     & 1)*jac3di(i1-i10,i2-2-i20,i3+2-i30))+16*(rsxy(i1,i2+1,i3-2,
     & axisp2,1)*jac3di(i1-i10,i2+1-i20,i3-2-i30))+16*(rsxy(i1,i2-1,
     & i3-2,axisp2,1)*jac3di(i1-i10,i2-1-i20,i3-2-i30))-(rsxy(i1,i2+2,
     & i3-2,axisp2,1)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,
     & i3-2,axisp2,1)*jac3di(i1-i10,i2-2-i20,i3-2-i30))+30*(rsxy(i1,
     & i2,i3+2,axisp2,1)*jac3di(i1-i10,i2-i20,i3+2-i30))-30*(rsxy(i1,
     & i2,i3-2,axisp2,1)*jac3di(i1-i10,i2-i20,i3-2-i30))+128*(rsxy(i1,
     & i2+1,i3+1,axisp2,1)*jac3di(i1-i10,i2+1-i20,i3+1-i30))+128*(
     & rsxy(i1,i2-1,i3+1,axisp2,1)*jac3di(i1-i10,i2-1-i20,i3+1-i30))-
     & 8*(rsxy(i1,i2+2,i3+1,axisp2,1)*jac3di(i1-i10,i2+2-i20,i3+1-i30)
     & )-8*(rsxy(i1,i2-2,i3+1,axisp2,1)*jac3di(i1-i10,i2-2-i20,i3+1-
     & i30))-128*(rsxy(i1,i2+1,i3-1,axisp2,1)*jac3di(i1-i10,i2+1-i20,
     & i3-1-i30))-128*(rsxy(i1,i2-1,i3-1,axisp2,1)*jac3di(i1-i10,i2-1-
     & i20,i3-1-i30))+8*(rsxy(i1,i2+2,i3-1,axisp2,1)*jac3di(i1-i10,i2+
     & 2-i20,i3-1-i30))+8*(rsxy(i1,i2-2,i3-1,axisp2,1)*jac3di(i1-i10,
     & i2-2-i20,i3-1-i30)))/(144.*dr(1)**2*dr(2))
                     a33rtt = (240*(rsxy(i1,i2,i3-1,axisp2,2)*jac3di(
     & i1-i10,i2-i20,i3-1-i30))-240*(rsxy(i1,i2,i3+1,axisp2,2)*jac3di(
     & i1-i10,i2-i20,i3+1-i30))-16*(rsxy(i1,i2+1,i3+2,axisp2,2)*
     & jac3di(i1-i10,i2+1-i20,i3+2-i30))-16*(rsxy(i1,i2-1,i3+2,axisp2,
     & 2)*jac3di(i1-i10,i2-1-i20,i3+2-i30))+(rsxy(i1,i2+2,i3+2,axisp2,
     & 2)*jac3di(i1-i10,i2+2-i20,i3+2-i30))+(rsxy(i1,i2-2,i3+2,axisp2,
     & 2)*jac3di(i1-i10,i2-2-i20,i3+2-i30))+16*(rsxy(i1,i2+1,i3-2,
     & axisp2,2)*jac3di(i1-i10,i2+1-i20,i3-2-i30))+16*(rsxy(i1,i2-1,
     & i3-2,axisp2,2)*jac3di(i1-i10,i2-1-i20,i3-2-i30))-(rsxy(i1,i2+2,
     & i3-2,axisp2,2)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,
     & i3-2,axisp2,2)*jac3di(i1-i10,i2-2-i20,i3-2-i30))+30*(rsxy(i1,
     & i2,i3+2,axisp2,2)*jac3di(i1-i10,i2-i20,i3+2-i30))-30*(rsxy(i1,
     & i2,i3-2,axisp2,2)*jac3di(i1-i10,i2-i20,i3-2-i30))+128*(rsxy(i1,
     & i2+1,i3+1,axisp2,2)*jac3di(i1-i10,i2+1-i20,i3+1-i30))+128*(
     & rsxy(i1,i2-1,i3+1,axisp2,2)*jac3di(i1-i10,i2-1-i20,i3+1-i30))-
     & 8*(rsxy(i1,i2+2,i3+1,axisp2,2)*jac3di(i1-i10,i2+2-i20,i3+1-i30)
     & )-8*(rsxy(i1,i2-2,i3+1,axisp2,2)*jac3di(i1-i10,i2-2-i20,i3+1-
     & i30))-128*(rsxy(i1,i2+1,i3-1,axisp2,2)*jac3di(i1-i10,i2+1-i20,
     & i3-1-i30))-128*(rsxy(i1,i2-1,i3-1,axisp2,2)*jac3di(i1-i10,i2-1-
     & i20,i3-1-i30))+8*(rsxy(i1,i2+2,i3-1,axisp2,2)*jac3di(i1-i10,i2+
     & 2-i20,i3-1-i30))+8*(rsxy(i1,i2-2,i3-1,axisp2,2)*jac3di(i1-i10,
     & i2-2-i20,i3-1-i30)))/(144.*dr(1)**2*dr(2))
                     a11stt = (128*(rsxy(i1+1,i2+1,i3,axis,0)*jac3di(
     & i1+1-i10,i2+1-i20,i3-i30))-128*(rsxy(i1-1,i2+1,i3,axis,0)*
     & jac3di(i1-1-i10,i2+1-i20,i3-i30))-16*(rsxy(i1+2,i2+1,i3,axis,0)
     & *jac3di(i1+2-i10,i2+1-i20,i3-i30))+16*(rsxy(i1-2,i2+1,i3,axis,
     & 0)*jac3di(i1-2-i10,i2+1-i20,i3-i30))+128*(rsxy(i1+1,i2-1,i3,
     & axis,0)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-128*(rsxy(i1-1,i2-1,
     & i3,axis,0)*jac3di(i1-1-i10,i2-1-i20,i3-i30))-16*(rsxy(i1+2,i2-
     & 1,i3,axis,0)*jac3di(i1+2-i10,i2-1-i20,i3-i30))+16*(rsxy(i1-2,
     & i2-1,i3,axis,0)*jac3di(i1-2-i10,i2-1-i20,i3-i30))+30*(rsxy(i1+
     & 2,i2,i3,axis,0)*jac3di(i1+2-i10,i2-i20,i3-i30))+8*(rsxy(i1-1,
     & i2+2,i3,axis,0)*jac3di(i1-1-i10,i2+2-i20,i3-i30))+(rsxy(i1+2,
     & i2+2,i3,axis,0)*jac3di(i1+2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,
     & i2+2,i3,axis,0)*jac3di(i1-2-i10,i2+2-i20,i3-i30))-8*(rsxy(i1+1,
     & i2-2,i3,axis,0)*jac3di(i1+1-i10,i2-2-i20,i3-i30))+8*(rsxy(i1-1,
     & i2-2,i3,axis,0)*jac3di(i1-1-i10,i2-2-i20,i3-i30))+(rsxy(i1+2,
     & i2-2,i3,axis,0)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,
     & i2-2,i3,axis,0)*jac3di(i1-2-i10,i2-2-i20,i3-i30))-8*(rsxy(i1+1,
     & i2+2,i3,axis,0)*jac3di(i1+1-i10,i2+2-i20,i3-i30))-30*(rsxy(i1-
     & 2,i2,i3,axis,0)*jac3di(i1-2-i10,i2-i20,i3-i30))-240*(rsxy(i1+1,
     & i2,i3,axis,0)*jac3di(i1+1-i10,i2-i20,i3-i30))+240*(rsxy(i1-1,
     & i2,i3,axis,0)*jac3di(i1-1-i10,i2-i20,i3-i30)))/(144.*dr(1)**2*
     & dr(0))
                     a12stt = (128*(rsxy(i1+1,i2+1,i3,axis,1)*jac3di(
     & i1+1-i10,i2+1-i20,i3-i30))-128*(rsxy(i1-1,i2+1,i3,axis,1)*
     & jac3di(i1-1-i10,i2+1-i20,i3-i30))-16*(rsxy(i1+2,i2+1,i3,axis,1)
     & *jac3di(i1+2-i10,i2+1-i20,i3-i30))+16*(rsxy(i1-2,i2+1,i3,axis,
     & 1)*jac3di(i1-2-i10,i2+1-i20,i3-i30))+128*(rsxy(i1+1,i2-1,i3,
     & axis,1)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-128*(rsxy(i1-1,i2-1,
     & i3,axis,1)*jac3di(i1-1-i10,i2-1-i20,i3-i30))-16*(rsxy(i1+2,i2-
     & 1,i3,axis,1)*jac3di(i1+2-i10,i2-1-i20,i3-i30))+16*(rsxy(i1-2,
     & i2-1,i3,axis,1)*jac3di(i1-2-i10,i2-1-i20,i3-i30))+30*(rsxy(i1+
     & 2,i2,i3,axis,1)*jac3di(i1+2-i10,i2-i20,i3-i30))+8*(rsxy(i1-1,
     & i2+2,i3,axis,1)*jac3di(i1-1-i10,i2+2-i20,i3-i30))+(rsxy(i1+2,
     & i2+2,i3,axis,1)*jac3di(i1+2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,
     & i2+2,i3,axis,1)*jac3di(i1-2-i10,i2+2-i20,i3-i30))-8*(rsxy(i1+1,
     & i2-2,i3,axis,1)*jac3di(i1+1-i10,i2-2-i20,i3-i30))+8*(rsxy(i1-1,
     & i2-2,i3,axis,1)*jac3di(i1-1-i10,i2-2-i20,i3-i30))+(rsxy(i1+2,
     & i2-2,i3,axis,1)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,
     & i2-2,i3,axis,1)*jac3di(i1-2-i10,i2-2-i20,i3-i30))-8*(rsxy(i1+1,
     & i2+2,i3,axis,1)*jac3di(i1+1-i10,i2+2-i20,i3-i30))-30*(rsxy(i1-
     & 2,i2,i3,axis,1)*jac3di(i1-2-i10,i2-i20,i3-i30))-240*(rsxy(i1+1,
     & i2,i3,axis,1)*jac3di(i1+1-i10,i2-i20,i3-i30))+240*(rsxy(i1-1,
     & i2,i3,axis,1)*jac3di(i1-1-i10,i2-i20,i3-i30)))/(144.*dr(1)**2*
     & dr(0))
                     a13stt = (128*(rsxy(i1+1,i2+1,i3,axis,2)*jac3di(
     & i1+1-i10,i2+1-i20,i3-i30))-128*(rsxy(i1-1,i2+1,i3,axis,2)*
     & jac3di(i1-1-i10,i2+1-i20,i3-i30))-16*(rsxy(i1+2,i2+1,i3,axis,2)
     & *jac3di(i1+2-i10,i2+1-i20,i3-i30))+16*(rsxy(i1-2,i2+1,i3,axis,
     & 2)*jac3di(i1-2-i10,i2+1-i20,i3-i30))+128*(rsxy(i1+1,i2-1,i3,
     & axis,2)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-128*(rsxy(i1-1,i2-1,
     & i3,axis,2)*jac3di(i1-1-i10,i2-1-i20,i3-i30))-16*(rsxy(i1+2,i2-
     & 1,i3,axis,2)*jac3di(i1+2-i10,i2-1-i20,i3-i30))+16*(rsxy(i1-2,
     & i2-1,i3,axis,2)*jac3di(i1-2-i10,i2-1-i20,i3-i30))+30*(rsxy(i1+
     & 2,i2,i3,axis,2)*jac3di(i1+2-i10,i2-i20,i3-i30))+8*(rsxy(i1-1,
     & i2+2,i3,axis,2)*jac3di(i1-1-i10,i2+2-i20,i3-i30))+(rsxy(i1+2,
     & i2+2,i3,axis,2)*jac3di(i1+2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,
     & i2+2,i3,axis,2)*jac3di(i1-2-i10,i2+2-i20,i3-i30))-8*(rsxy(i1+1,
     & i2-2,i3,axis,2)*jac3di(i1+1-i10,i2-2-i20,i3-i30))+8*(rsxy(i1-1,
     & i2-2,i3,axis,2)*jac3di(i1-1-i10,i2-2-i20,i3-i30))+(rsxy(i1+2,
     & i2-2,i3,axis,2)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,
     & i2-2,i3,axis,2)*jac3di(i1-2-i10,i2-2-i20,i3-i30))-8*(rsxy(i1+1,
     & i2+2,i3,axis,2)*jac3di(i1+1-i10,i2+2-i20,i3-i30))-30*(rsxy(i1-
     & 2,i2,i3,axis,2)*jac3di(i1-2-i10,i2-i20,i3-i30))-240*(rsxy(i1+1,
     & i2,i3,axis,2)*jac3di(i1+1-i10,i2-i20,i3-i30))+240*(rsxy(i1-1,
     & i2,i3,axis,2)*jac3di(i1-1-i10,i2-i20,i3-i30)))/(144.*dr(1)**2*
     & dr(0))
                     a21stt = (128*(rsxy(i1+1,i2+1,i3,axisp1,0)*jac3di(
     & i1+1-i10,i2+1-i20,i3-i30))-128*(rsxy(i1-1,i2+1,i3,axisp1,0)*
     & jac3di(i1-1-i10,i2+1-i20,i3-i30))-16*(rsxy(i1+2,i2+1,i3,axisp1,
     & 0)*jac3di(i1+2-i10,i2+1-i20,i3-i30))+16*(rsxy(i1-2,i2+1,i3,
     & axisp1,0)*jac3di(i1-2-i10,i2+1-i20,i3-i30))+128*(rsxy(i1+1,i2-
     & 1,i3,axisp1,0)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-128*(rsxy(i1-
     & 1,i2-1,i3,axisp1,0)*jac3di(i1-1-i10,i2-1-i20,i3-i30))-16*(rsxy(
     & i1+2,i2-1,i3,axisp1,0)*jac3di(i1+2-i10,i2-1-i20,i3-i30))+16*(
     & rsxy(i1-2,i2-1,i3,axisp1,0)*jac3di(i1-2-i10,i2-1-i20,i3-i30))+
     & 30*(rsxy(i1+2,i2,i3,axisp1,0)*jac3di(i1+2-i10,i2-i20,i3-i30))+
     & 8*(rsxy(i1-1,i2+2,i3,axisp1,0)*jac3di(i1-1-i10,i2+2-i20,i3-i30)
     & )+(rsxy(i1+2,i2+2,i3,axisp1,0)*jac3di(i1+2-i10,i2+2-i20,i3-i30)
     & )-(rsxy(i1-2,i2+2,i3,axisp1,0)*jac3di(i1-2-i10,i2+2-i20,i3-i30)
     & )-8*(rsxy(i1+1,i2-2,i3,axisp1,0)*jac3di(i1+1-i10,i2-2-i20,i3-
     & i30))+8*(rsxy(i1-1,i2-2,i3,axisp1,0)*jac3di(i1-1-i10,i2-2-i20,
     & i3-i30))+(rsxy(i1+2,i2-2,i3,axisp1,0)*jac3di(i1+2-i10,i2-2-i20,
     & i3-i30))-(rsxy(i1-2,i2-2,i3,axisp1,0)*jac3di(i1-2-i10,i2-2-i20,
     & i3-i30))-8*(rsxy(i1+1,i2+2,i3,axisp1,0)*jac3di(i1+1-i10,i2+2-
     & i20,i3-i30))-30*(rsxy(i1-2,i2,i3,axisp1,0)*jac3di(i1-2-i10,i2-
     & i20,i3-i30))-240*(rsxy(i1+1,i2,i3,axisp1,0)*jac3di(i1+1-i10,i2-
     & i20,i3-i30))+240*(rsxy(i1-1,i2,i3,axisp1,0)*jac3di(i1-1-i10,i2-
     & i20,i3-i30)))/(144.*dr(1)**2*dr(0))
                     a22stt = (128*(rsxy(i1+1,i2+1,i3,axisp1,1)*jac3di(
     & i1+1-i10,i2+1-i20,i3-i30))-128*(rsxy(i1-1,i2+1,i3,axisp1,1)*
     & jac3di(i1-1-i10,i2+1-i20,i3-i30))-16*(rsxy(i1+2,i2+1,i3,axisp1,
     & 1)*jac3di(i1+2-i10,i2+1-i20,i3-i30))+16*(rsxy(i1-2,i2+1,i3,
     & axisp1,1)*jac3di(i1-2-i10,i2+1-i20,i3-i30))+128*(rsxy(i1+1,i2-
     & 1,i3,axisp1,1)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-128*(rsxy(i1-
     & 1,i2-1,i3,axisp1,1)*jac3di(i1-1-i10,i2-1-i20,i3-i30))-16*(rsxy(
     & i1+2,i2-1,i3,axisp1,1)*jac3di(i1+2-i10,i2-1-i20,i3-i30))+16*(
     & rsxy(i1-2,i2-1,i3,axisp1,1)*jac3di(i1-2-i10,i2-1-i20,i3-i30))+
     & 30*(rsxy(i1+2,i2,i3,axisp1,1)*jac3di(i1+2-i10,i2-i20,i3-i30))+
     & 8*(rsxy(i1-1,i2+2,i3,axisp1,1)*jac3di(i1-1-i10,i2+2-i20,i3-i30)
     & )+(rsxy(i1+2,i2+2,i3,axisp1,1)*jac3di(i1+2-i10,i2+2-i20,i3-i30)
     & )-(rsxy(i1-2,i2+2,i3,axisp1,1)*jac3di(i1-2-i10,i2+2-i20,i3-i30)
     & )-8*(rsxy(i1+1,i2-2,i3,axisp1,1)*jac3di(i1+1-i10,i2-2-i20,i3-
     & i30))+8*(rsxy(i1-1,i2-2,i3,axisp1,1)*jac3di(i1-1-i10,i2-2-i20,
     & i3-i30))+(rsxy(i1+2,i2-2,i3,axisp1,1)*jac3di(i1+2-i10,i2-2-i20,
     & i3-i30))-(rsxy(i1-2,i2-2,i3,axisp1,1)*jac3di(i1-2-i10,i2-2-i20,
     & i3-i30))-8*(rsxy(i1+1,i2+2,i3,axisp1,1)*jac3di(i1+1-i10,i2+2-
     & i20,i3-i30))-30*(rsxy(i1-2,i2,i3,axisp1,1)*jac3di(i1-2-i10,i2-
     & i20,i3-i30))-240*(rsxy(i1+1,i2,i3,axisp1,1)*jac3di(i1+1-i10,i2-
     & i20,i3-i30))+240*(rsxy(i1-1,i2,i3,axisp1,1)*jac3di(i1-1-i10,i2-
     & i20,i3-i30)))/(144.*dr(1)**2*dr(0))
                     a23stt = (128*(rsxy(i1+1,i2+1,i3,axisp1,2)*jac3di(
     & i1+1-i10,i2+1-i20,i3-i30))-128*(rsxy(i1-1,i2+1,i3,axisp1,2)*
     & jac3di(i1-1-i10,i2+1-i20,i3-i30))-16*(rsxy(i1+2,i2+1,i3,axisp1,
     & 2)*jac3di(i1+2-i10,i2+1-i20,i3-i30))+16*(rsxy(i1-2,i2+1,i3,
     & axisp1,2)*jac3di(i1-2-i10,i2+1-i20,i3-i30))+128*(rsxy(i1+1,i2-
     & 1,i3,axisp1,2)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-128*(rsxy(i1-
     & 1,i2-1,i3,axisp1,2)*jac3di(i1-1-i10,i2-1-i20,i3-i30))-16*(rsxy(
     & i1+2,i2-1,i3,axisp1,2)*jac3di(i1+2-i10,i2-1-i20,i3-i30))+16*(
     & rsxy(i1-2,i2-1,i3,axisp1,2)*jac3di(i1-2-i10,i2-1-i20,i3-i30))+
     & 30*(rsxy(i1+2,i2,i3,axisp1,2)*jac3di(i1+2-i10,i2-i20,i3-i30))+
     & 8*(rsxy(i1-1,i2+2,i3,axisp1,2)*jac3di(i1-1-i10,i2+2-i20,i3-i30)
     & )+(rsxy(i1+2,i2+2,i3,axisp1,2)*jac3di(i1+2-i10,i2+2-i20,i3-i30)
     & )-(rsxy(i1-2,i2+2,i3,axisp1,2)*jac3di(i1-2-i10,i2+2-i20,i3-i30)
     & )-8*(rsxy(i1+1,i2-2,i3,axisp1,2)*jac3di(i1+1-i10,i2-2-i20,i3-
     & i30))+8*(rsxy(i1-1,i2-2,i3,axisp1,2)*jac3di(i1-1-i10,i2-2-i20,
     & i3-i30))+(rsxy(i1+2,i2-2,i3,axisp1,2)*jac3di(i1+2-i10,i2-2-i20,
     & i3-i30))-(rsxy(i1-2,i2-2,i3,axisp1,2)*jac3di(i1-2-i10,i2-2-i20,
     & i3-i30))-8*(rsxy(i1+1,i2+2,i3,axisp1,2)*jac3di(i1+1-i10,i2+2-
     & i20,i3-i30))-30*(rsxy(i1-2,i2,i3,axisp1,2)*jac3di(i1-2-i10,i2-
     & i20,i3-i30))-240*(rsxy(i1+1,i2,i3,axisp1,2)*jac3di(i1+1-i10,i2-
     & i20,i3-i30))+240*(rsxy(i1-1,i2,i3,axisp1,2)*jac3di(i1-1-i10,i2-
     & i20,i3-i30)))/(144.*dr(1)**2*dr(0))
                     a31stt = (128*(rsxy(i1+1,i2+1,i3,axisp2,0)*jac3di(
     & i1+1-i10,i2+1-i20,i3-i30))-128*(rsxy(i1-1,i2+1,i3,axisp2,0)*
     & jac3di(i1-1-i10,i2+1-i20,i3-i30))-16*(rsxy(i1+2,i2+1,i3,axisp2,
     & 0)*jac3di(i1+2-i10,i2+1-i20,i3-i30))+16*(rsxy(i1-2,i2+1,i3,
     & axisp2,0)*jac3di(i1-2-i10,i2+1-i20,i3-i30))+128*(rsxy(i1+1,i2-
     & 1,i3,axisp2,0)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-128*(rsxy(i1-
     & 1,i2-1,i3,axisp2,0)*jac3di(i1-1-i10,i2-1-i20,i3-i30))-16*(rsxy(
     & i1+2,i2-1,i3,axisp2,0)*jac3di(i1+2-i10,i2-1-i20,i3-i30))+16*(
     & rsxy(i1-2,i2-1,i3,axisp2,0)*jac3di(i1-2-i10,i2-1-i20,i3-i30))+
     & 30*(rsxy(i1+2,i2,i3,axisp2,0)*jac3di(i1+2-i10,i2-i20,i3-i30))+
     & 8*(rsxy(i1-1,i2+2,i3,axisp2,0)*jac3di(i1-1-i10,i2+2-i20,i3-i30)
     & )+(rsxy(i1+2,i2+2,i3,axisp2,0)*jac3di(i1+2-i10,i2+2-i20,i3-i30)
     & )-(rsxy(i1-2,i2+2,i3,axisp2,0)*jac3di(i1-2-i10,i2+2-i20,i3-i30)
     & )-8*(rsxy(i1+1,i2-2,i3,axisp2,0)*jac3di(i1+1-i10,i2-2-i20,i3-
     & i30))+8*(rsxy(i1-1,i2-2,i3,axisp2,0)*jac3di(i1-1-i10,i2-2-i20,
     & i3-i30))+(rsxy(i1+2,i2-2,i3,axisp2,0)*jac3di(i1+2-i10,i2-2-i20,
     & i3-i30))-(rsxy(i1-2,i2-2,i3,axisp2,0)*jac3di(i1-2-i10,i2-2-i20,
     & i3-i30))-8*(rsxy(i1+1,i2+2,i3,axisp2,0)*jac3di(i1+1-i10,i2+2-
     & i20,i3-i30))-30*(rsxy(i1-2,i2,i3,axisp2,0)*jac3di(i1-2-i10,i2-
     & i20,i3-i30))-240*(rsxy(i1+1,i2,i3,axisp2,0)*jac3di(i1+1-i10,i2-
     & i20,i3-i30))+240*(rsxy(i1-1,i2,i3,axisp2,0)*jac3di(i1-1-i10,i2-
     & i20,i3-i30)))/(144.*dr(1)**2*dr(0))
                     a32stt = (128*(rsxy(i1+1,i2+1,i3,axisp2,1)*jac3di(
     & i1+1-i10,i2+1-i20,i3-i30))-128*(rsxy(i1-1,i2+1,i3,axisp2,1)*
     & jac3di(i1-1-i10,i2+1-i20,i3-i30))-16*(rsxy(i1+2,i2+1,i3,axisp2,
     & 1)*jac3di(i1+2-i10,i2+1-i20,i3-i30))+16*(rsxy(i1-2,i2+1,i3,
     & axisp2,1)*jac3di(i1-2-i10,i2+1-i20,i3-i30))+128*(rsxy(i1+1,i2-
     & 1,i3,axisp2,1)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-128*(rsxy(i1-
     & 1,i2-1,i3,axisp2,1)*jac3di(i1-1-i10,i2-1-i20,i3-i30))-16*(rsxy(
     & i1+2,i2-1,i3,axisp2,1)*jac3di(i1+2-i10,i2-1-i20,i3-i30))+16*(
     & rsxy(i1-2,i2-1,i3,axisp2,1)*jac3di(i1-2-i10,i2-1-i20,i3-i30))+
     & 30*(rsxy(i1+2,i2,i3,axisp2,1)*jac3di(i1+2-i10,i2-i20,i3-i30))+
     & 8*(rsxy(i1-1,i2+2,i3,axisp2,1)*jac3di(i1-1-i10,i2+2-i20,i3-i30)
     & )+(rsxy(i1+2,i2+2,i3,axisp2,1)*jac3di(i1+2-i10,i2+2-i20,i3-i30)
     & )-(rsxy(i1-2,i2+2,i3,axisp2,1)*jac3di(i1-2-i10,i2+2-i20,i3-i30)
     & )-8*(rsxy(i1+1,i2-2,i3,axisp2,1)*jac3di(i1+1-i10,i2-2-i20,i3-
     & i30))+8*(rsxy(i1-1,i2-2,i3,axisp2,1)*jac3di(i1-1-i10,i2-2-i20,
     & i3-i30))+(rsxy(i1+2,i2-2,i3,axisp2,1)*jac3di(i1+2-i10,i2-2-i20,
     & i3-i30))-(rsxy(i1-2,i2-2,i3,axisp2,1)*jac3di(i1-2-i10,i2-2-i20,
     & i3-i30))-8*(rsxy(i1+1,i2+2,i3,axisp2,1)*jac3di(i1+1-i10,i2+2-
     & i20,i3-i30))-30*(rsxy(i1-2,i2,i3,axisp2,1)*jac3di(i1-2-i10,i2-
     & i20,i3-i30))-240*(rsxy(i1+1,i2,i3,axisp2,1)*jac3di(i1+1-i10,i2-
     & i20,i3-i30))+240*(rsxy(i1-1,i2,i3,axisp2,1)*jac3di(i1-1-i10,i2-
     & i20,i3-i30)))/(144.*dr(1)**2*dr(0))
                     a33stt = (128*(rsxy(i1+1,i2+1,i3,axisp2,2)*jac3di(
     & i1+1-i10,i2+1-i20,i3-i30))-128*(rsxy(i1-1,i2+1,i3,axisp2,2)*
     & jac3di(i1-1-i10,i2+1-i20,i3-i30))-16*(rsxy(i1+2,i2+1,i3,axisp2,
     & 2)*jac3di(i1+2-i10,i2+1-i20,i3-i30))+16*(rsxy(i1-2,i2+1,i3,
     & axisp2,2)*jac3di(i1-2-i10,i2+1-i20,i3-i30))+128*(rsxy(i1+1,i2-
     & 1,i3,axisp2,2)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-128*(rsxy(i1-
     & 1,i2-1,i3,axisp2,2)*jac3di(i1-1-i10,i2-1-i20,i3-i30))-16*(rsxy(
     & i1+2,i2-1,i3,axisp2,2)*jac3di(i1+2-i10,i2-1-i20,i3-i30))+16*(
     & rsxy(i1-2,i2-1,i3,axisp2,2)*jac3di(i1-2-i10,i2-1-i20,i3-i30))+
     & 30*(rsxy(i1+2,i2,i3,axisp2,2)*jac3di(i1+2-i10,i2-i20,i3-i30))+
     & 8*(rsxy(i1-1,i2+2,i3,axisp2,2)*jac3di(i1-1-i10,i2+2-i20,i3-i30)
     & )+(rsxy(i1+2,i2+2,i3,axisp2,2)*jac3di(i1+2-i10,i2+2-i20,i3-i30)
     & )-(rsxy(i1-2,i2+2,i3,axisp2,2)*jac3di(i1-2-i10,i2+2-i20,i3-i30)
     & )-8*(rsxy(i1+1,i2-2,i3,axisp2,2)*jac3di(i1+1-i10,i2-2-i20,i3-
     & i30))+8*(rsxy(i1-1,i2-2,i3,axisp2,2)*jac3di(i1-1-i10,i2-2-i20,
     & i3-i30))+(rsxy(i1+2,i2-2,i3,axisp2,2)*jac3di(i1+2-i10,i2-2-i20,
     & i3-i30))-(rsxy(i1-2,i2-2,i3,axisp2,2)*jac3di(i1-2-i10,i2-2-i20,
     & i3-i30))-8*(rsxy(i1+1,i2+2,i3,axisp2,2)*jac3di(i1+1-i10,i2+2-
     & i20,i3-i30))-30*(rsxy(i1-2,i2,i3,axisp2,2)*jac3di(i1-2-i10,i2-
     & i20,i3-i30))-240*(rsxy(i1+1,i2,i3,axisp2,2)*jac3di(i1+1-i10,i2-
     & i20,i3-i30))+240*(rsxy(i1-1,i2,i3,axisp2,2)*jac3di(i1-1-i10,i2-
     & i20,i3-i30)))/(144.*dr(1)**2*dr(0))
                     a11sst = (128*(rsxy(i1+1,i2+1,i3,axis,0)*jac3di(
     & i1+1-i10,i2+1-i20,i3-i30))-240*(rsxy(i1,i2+1,i3,axis,0)*jac3di(
     & i1-i10,i2+1-i20,i3-i30))+128*(rsxy(i1-1,i2+1,i3,axis,0)*jac3di(
     & i1-1-i10,i2+1-i20,i3-i30))-8*(rsxy(i1+2,i2+1,i3,axis,0)*jac3di(
     & i1+2-i10,i2+1-i20,i3-i30))-8*(rsxy(i1-2,i2+1,i3,axis,0)*jac3di(
     & i1-2-i10,i2+1-i20,i3-i30))-128*(rsxy(i1+1,i2-1,i3,axis,0)*
     & jac3di(i1+1-i10,i2-1-i20,i3-i30))+240*(rsxy(i1,i2-1,i3,axis,0)*
     & jac3di(i1-i10,i2-1-i20,i3-i30))-128*(rsxy(i1-1,i2-1,i3,axis,0)*
     & jac3di(i1-1-i10,i2-1-i20,i3-i30))+8*(rsxy(i1+2,i2-1,i3,axis,0)*
     & jac3di(i1+2-i10,i2-1-i20,i3-i30))+8*(rsxy(i1-2,i2-1,i3,axis,0)*
     & jac3di(i1-2-i10,i2-1-i20,i3-i30))+30*(rsxy(i1,i2+2,i3,axis,0)*
     & jac3di(i1-i10,i2+2-i20,i3-i30))-16*(rsxy(i1-1,i2+2,i3,axis,0)*
     & jac3di(i1-1-i10,i2+2-i20,i3-i30))+(rsxy(i1+2,i2+2,i3,axis,0)*
     & jac3di(i1+2-i10,i2+2-i20,i3-i30))+(rsxy(i1-2,i2+2,i3,axis,0)*
     & jac3di(i1-2-i10,i2+2-i20,i3-i30))+16*(rsxy(i1+1,i2-2,i3,axis,0)
     & *jac3di(i1+1-i10,i2-2-i20,i3-i30))-30*(rsxy(i1,i2-2,i3,axis,0)*
     & jac3di(i1-i10,i2-2-i20,i3-i30))+16*(rsxy(i1-1,i2-2,i3,axis,0)*
     & jac3di(i1-1-i10,i2-2-i20,i3-i30))-(rsxy(i1+2,i2-2,i3,axis,0)*
     & jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-2,i3,axis,0)*
     & jac3di(i1-2-i10,i2-2-i20,i3-i30))-16*(rsxy(i1+1,i2+2,i3,axis,0)
     & *jac3di(i1+1-i10,i2+2-i20,i3-i30)))/(144.*dr(0)**2*dr(1))
                     a12sst = (128*(rsxy(i1+1,i2+1,i3,axis,1)*jac3di(
     & i1+1-i10,i2+1-i20,i3-i30))-240*(rsxy(i1,i2+1,i3,axis,1)*jac3di(
     & i1-i10,i2+1-i20,i3-i30))+128*(rsxy(i1-1,i2+1,i3,axis,1)*jac3di(
     & i1-1-i10,i2+1-i20,i3-i30))-8*(rsxy(i1+2,i2+1,i3,axis,1)*jac3di(
     & i1+2-i10,i2+1-i20,i3-i30))-8*(rsxy(i1-2,i2+1,i3,axis,1)*jac3di(
     & i1-2-i10,i2+1-i20,i3-i30))-128*(rsxy(i1+1,i2-1,i3,axis,1)*
     & jac3di(i1+1-i10,i2-1-i20,i3-i30))+240*(rsxy(i1,i2-1,i3,axis,1)*
     & jac3di(i1-i10,i2-1-i20,i3-i30))-128*(rsxy(i1-1,i2-1,i3,axis,1)*
     & jac3di(i1-1-i10,i2-1-i20,i3-i30))+8*(rsxy(i1+2,i2-1,i3,axis,1)*
     & jac3di(i1+2-i10,i2-1-i20,i3-i30))+8*(rsxy(i1-2,i2-1,i3,axis,1)*
     & jac3di(i1-2-i10,i2-1-i20,i3-i30))+30*(rsxy(i1,i2+2,i3,axis,1)*
     & jac3di(i1-i10,i2+2-i20,i3-i30))-16*(rsxy(i1-1,i2+2,i3,axis,1)*
     & jac3di(i1-1-i10,i2+2-i20,i3-i30))+(rsxy(i1+2,i2+2,i3,axis,1)*
     & jac3di(i1+2-i10,i2+2-i20,i3-i30))+(rsxy(i1-2,i2+2,i3,axis,1)*
     & jac3di(i1-2-i10,i2+2-i20,i3-i30))+16*(rsxy(i1+1,i2-2,i3,axis,1)
     & *jac3di(i1+1-i10,i2-2-i20,i3-i30))-30*(rsxy(i1,i2-2,i3,axis,1)*
     & jac3di(i1-i10,i2-2-i20,i3-i30))+16*(rsxy(i1-1,i2-2,i3,axis,1)*
     & jac3di(i1-1-i10,i2-2-i20,i3-i30))-(rsxy(i1+2,i2-2,i3,axis,1)*
     & jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-2,i3,axis,1)*
     & jac3di(i1-2-i10,i2-2-i20,i3-i30))-16*(rsxy(i1+1,i2+2,i3,axis,1)
     & *jac3di(i1+1-i10,i2+2-i20,i3-i30)))/(144.*dr(0)**2*dr(1))
                     a13sst = (128*(rsxy(i1+1,i2+1,i3,axis,2)*jac3di(
     & i1+1-i10,i2+1-i20,i3-i30))-240*(rsxy(i1,i2+1,i3,axis,2)*jac3di(
     & i1-i10,i2+1-i20,i3-i30))+128*(rsxy(i1-1,i2+1,i3,axis,2)*jac3di(
     & i1-1-i10,i2+1-i20,i3-i30))-8*(rsxy(i1+2,i2+1,i3,axis,2)*jac3di(
     & i1+2-i10,i2+1-i20,i3-i30))-8*(rsxy(i1-2,i2+1,i3,axis,2)*jac3di(
     & i1-2-i10,i2+1-i20,i3-i30))-128*(rsxy(i1+1,i2-1,i3,axis,2)*
     & jac3di(i1+1-i10,i2-1-i20,i3-i30))+240*(rsxy(i1,i2-1,i3,axis,2)*
     & jac3di(i1-i10,i2-1-i20,i3-i30))-128*(rsxy(i1-1,i2-1,i3,axis,2)*
     & jac3di(i1-1-i10,i2-1-i20,i3-i30))+8*(rsxy(i1+2,i2-1,i3,axis,2)*
     & jac3di(i1+2-i10,i2-1-i20,i3-i30))+8*(rsxy(i1-2,i2-1,i3,axis,2)*
     & jac3di(i1-2-i10,i2-1-i20,i3-i30))+30*(rsxy(i1,i2+2,i3,axis,2)*
     & jac3di(i1-i10,i2+2-i20,i3-i30))-16*(rsxy(i1-1,i2+2,i3,axis,2)*
     & jac3di(i1-1-i10,i2+2-i20,i3-i30))+(rsxy(i1+2,i2+2,i3,axis,2)*
     & jac3di(i1+2-i10,i2+2-i20,i3-i30))+(rsxy(i1-2,i2+2,i3,axis,2)*
     & jac3di(i1-2-i10,i2+2-i20,i3-i30))+16*(rsxy(i1+1,i2-2,i3,axis,2)
     & *jac3di(i1+1-i10,i2-2-i20,i3-i30))-30*(rsxy(i1,i2-2,i3,axis,2)*
     & jac3di(i1-i10,i2-2-i20,i3-i30))+16*(rsxy(i1-1,i2-2,i3,axis,2)*
     & jac3di(i1-1-i10,i2-2-i20,i3-i30))-(rsxy(i1+2,i2-2,i3,axis,2)*
     & jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-2,i3,axis,2)*
     & jac3di(i1-2-i10,i2-2-i20,i3-i30))-16*(rsxy(i1+1,i2+2,i3,axis,2)
     & *jac3di(i1+1-i10,i2+2-i20,i3-i30)))/(144.*dr(0)**2*dr(1))
                     a21sst = (128*(rsxy(i1+1,i2+1,i3,axisp1,0)*jac3di(
     & i1+1-i10,i2+1-i20,i3-i30))-240*(rsxy(i1,i2+1,i3,axisp1,0)*
     & jac3di(i1-i10,i2+1-i20,i3-i30))+128*(rsxy(i1-1,i2+1,i3,axisp1,
     & 0)*jac3di(i1-1-i10,i2+1-i20,i3-i30))-8*(rsxy(i1+2,i2+1,i3,
     & axisp1,0)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-8*(rsxy(i1-2,i2+1,
     & i3,axisp1,0)*jac3di(i1-2-i10,i2+1-i20,i3-i30))-128*(rsxy(i1+1,
     & i2-1,i3,axisp1,0)*jac3di(i1+1-i10,i2-1-i20,i3-i30))+240*(rsxy(
     & i1,i2-1,i3,axisp1,0)*jac3di(i1-i10,i2-1-i20,i3-i30))-128*(rsxy(
     & i1-1,i2-1,i3,axisp1,0)*jac3di(i1-1-i10,i2-1-i20,i3-i30))+8*(
     & rsxy(i1+2,i2-1,i3,axisp1,0)*jac3di(i1+2-i10,i2-1-i20,i3-i30))+
     & 8*(rsxy(i1-2,i2-1,i3,axisp1,0)*jac3di(i1-2-i10,i2-1-i20,i3-i30)
     & )+30*(rsxy(i1,i2+2,i3,axisp1,0)*jac3di(i1-i10,i2+2-i20,i3-i30))
     & -16*(rsxy(i1-1,i2+2,i3,axisp1,0)*jac3di(i1-1-i10,i2+2-i20,i3-
     & i30))+(rsxy(i1+2,i2+2,i3,axisp1,0)*jac3di(i1+2-i10,i2+2-i20,i3-
     & i30))+(rsxy(i1-2,i2+2,i3,axisp1,0)*jac3di(i1-2-i10,i2+2-i20,i3-
     & i30))+16*(rsxy(i1+1,i2-2,i3,axisp1,0)*jac3di(i1+1-i10,i2-2-i20,
     & i3-i30))-30*(rsxy(i1,i2-2,i3,axisp1,0)*jac3di(i1-i10,i2-2-i20,
     & i3-i30))+16*(rsxy(i1-1,i2-2,i3,axisp1,0)*jac3di(i1-1-i10,i2-2-
     & i20,i3-i30))-(rsxy(i1+2,i2-2,i3,axisp1,0)*jac3di(i1+2-i10,i2-2-
     & i20,i3-i30))-(rsxy(i1-2,i2-2,i3,axisp1,0)*jac3di(i1-2-i10,i2-2-
     & i20,i3-i30))-16*(rsxy(i1+1,i2+2,i3,axisp1,0)*jac3di(i1+1-i10,
     & i2+2-i20,i3-i30)))/(144.*dr(0)**2*dr(1))
                     a22sst = (128*(rsxy(i1+1,i2+1,i3,axisp1,1)*jac3di(
     & i1+1-i10,i2+1-i20,i3-i30))-240*(rsxy(i1,i2+1,i3,axisp1,1)*
     & jac3di(i1-i10,i2+1-i20,i3-i30))+128*(rsxy(i1-1,i2+1,i3,axisp1,
     & 1)*jac3di(i1-1-i10,i2+1-i20,i3-i30))-8*(rsxy(i1+2,i2+1,i3,
     & axisp1,1)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-8*(rsxy(i1-2,i2+1,
     & i3,axisp1,1)*jac3di(i1-2-i10,i2+1-i20,i3-i30))-128*(rsxy(i1+1,
     & i2-1,i3,axisp1,1)*jac3di(i1+1-i10,i2-1-i20,i3-i30))+240*(rsxy(
     & i1,i2-1,i3,axisp1,1)*jac3di(i1-i10,i2-1-i20,i3-i30))-128*(rsxy(
     & i1-1,i2-1,i3,axisp1,1)*jac3di(i1-1-i10,i2-1-i20,i3-i30))+8*(
     & rsxy(i1+2,i2-1,i3,axisp1,1)*jac3di(i1+2-i10,i2-1-i20,i3-i30))+
     & 8*(rsxy(i1-2,i2-1,i3,axisp1,1)*jac3di(i1-2-i10,i2-1-i20,i3-i30)
     & )+30*(rsxy(i1,i2+2,i3,axisp1,1)*jac3di(i1-i10,i2+2-i20,i3-i30))
     & -16*(rsxy(i1-1,i2+2,i3,axisp1,1)*jac3di(i1-1-i10,i2+2-i20,i3-
     & i30))+(rsxy(i1+2,i2+2,i3,axisp1,1)*jac3di(i1+2-i10,i2+2-i20,i3-
     & i30))+(rsxy(i1-2,i2+2,i3,axisp1,1)*jac3di(i1-2-i10,i2+2-i20,i3-
     & i30))+16*(rsxy(i1+1,i2-2,i3,axisp1,1)*jac3di(i1+1-i10,i2-2-i20,
     & i3-i30))-30*(rsxy(i1,i2-2,i3,axisp1,1)*jac3di(i1-i10,i2-2-i20,
     & i3-i30))+16*(rsxy(i1-1,i2-2,i3,axisp1,1)*jac3di(i1-1-i10,i2-2-
     & i20,i3-i30))-(rsxy(i1+2,i2-2,i3,axisp1,1)*jac3di(i1+2-i10,i2-2-
     & i20,i3-i30))-(rsxy(i1-2,i2-2,i3,axisp1,1)*jac3di(i1-2-i10,i2-2-
     & i20,i3-i30))-16*(rsxy(i1+1,i2+2,i3,axisp1,1)*jac3di(i1+1-i10,
     & i2+2-i20,i3-i30)))/(144.*dr(0)**2*dr(1))
                     a23sst = (128*(rsxy(i1+1,i2+1,i3,axisp1,2)*jac3di(
     & i1+1-i10,i2+1-i20,i3-i30))-240*(rsxy(i1,i2+1,i3,axisp1,2)*
     & jac3di(i1-i10,i2+1-i20,i3-i30))+128*(rsxy(i1-1,i2+1,i3,axisp1,
     & 2)*jac3di(i1-1-i10,i2+1-i20,i3-i30))-8*(rsxy(i1+2,i2+1,i3,
     & axisp1,2)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-8*(rsxy(i1-2,i2+1,
     & i3,axisp1,2)*jac3di(i1-2-i10,i2+1-i20,i3-i30))-128*(rsxy(i1+1,
     & i2-1,i3,axisp1,2)*jac3di(i1+1-i10,i2-1-i20,i3-i30))+240*(rsxy(
     & i1,i2-1,i3,axisp1,2)*jac3di(i1-i10,i2-1-i20,i3-i30))-128*(rsxy(
     & i1-1,i2-1,i3,axisp1,2)*jac3di(i1-1-i10,i2-1-i20,i3-i30))+8*(
     & rsxy(i1+2,i2-1,i3,axisp1,2)*jac3di(i1+2-i10,i2-1-i20,i3-i30))+
     & 8*(rsxy(i1-2,i2-1,i3,axisp1,2)*jac3di(i1-2-i10,i2-1-i20,i3-i30)
     & )+30*(rsxy(i1,i2+2,i3,axisp1,2)*jac3di(i1-i10,i2+2-i20,i3-i30))
     & -16*(rsxy(i1-1,i2+2,i3,axisp1,2)*jac3di(i1-1-i10,i2+2-i20,i3-
     & i30))+(rsxy(i1+2,i2+2,i3,axisp1,2)*jac3di(i1+2-i10,i2+2-i20,i3-
     & i30))+(rsxy(i1-2,i2+2,i3,axisp1,2)*jac3di(i1-2-i10,i2+2-i20,i3-
     & i30))+16*(rsxy(i1+1,i2-2,i3,axisp1,2)*jac3di(i1+1-i10,i2-2-i20,
     & i3-i30))-30*(rsxy(i1,i2-2,i3,axisp1,2)*jac3di(i1-i10,i2-2-i20,
     & i3-i30))+16*(rsxy(i1-1,i2-2,i3,axisp1,2)*jac3di(i1-1-i10,i2-2-
     & i20,i3-i30))-(rsxy(i1+2,i2-2,i3,axisp1,2)*jac3di(i1+2-i10,i2-2-
     & i20,i3-i30))-(rsxy(i1-2,i2-2,i3,axisp1,2)*jac3di(i1-2-i10,i2-2-
     & i20,i3-i30))-16*(rsxy(i1+1,i2+2,i3,axisp1,2)*jac3di(i1+1-i10,
     & i2+2-i20,i3-i30)))/(144.*dr(0)**2*dr(1))
                     a31sst = (128*(rsxy(i1+1,i2+1,i3,axisp2,0)*jac3di(
     & i1+1-i10,i2+1-i20,i3-i30))-240*(rsxy(i1,i2+1,i3,axisp2,0)*
     & jac3di(i1-i10,i2+1-i20,i3-i30))+128*(rsxy(i1-1,i2+1,i3,axisp2,
     & 0)*jac3di(i1-1-i10,i2+1-i20,i3-i30))-8*(rsxy(i1+2,i2+1,i3,
     & axisp2,0)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-8*(rsxy(i1-2,i2+1,
     & i3,axisp2,0)*jac3di(i1-2-i10,i2+1-i20,i3-i30))-128*(rsxy(i1+1,
     & i2-1,i3,axisp2,0)*jac3di(i1+1-i10,i2-1-i20,i3-i30))+240*(rsxy(
     & i1,i2-1,i3,axisp2,0)*jac3di(i1-i10,i2-1-i20,i3-i30))-128*(rsxy(
     & i1-1,i2-1,i3,axisp2,0)*jac3di(i1-1-i10,i2-1-i20,i3-i30))+8*(
     & rsxy(i1+2,i2-1,i3,axisp2,0)*jac3di(i1+2-i10,i2-1-i20,i3-i30))+
     & 8*(rsxy(i1-2,i2-1,i3,axisp2,0)*jac3di(i1-2-i10,i2-1-i20,i3-i30)
     & )+30*(rsxy(i1,i2+2,i3,axisp2,0)*jac3di(i1-i10,i2+2-i20,i3-i30))
     & -16*(rsxy(i1-1,i2+2,i3,axisp2,0)*jac3di(i1-1-i10,i2+2-i20,i3-
     & i30))+(rsxy(i1+2,i2+2,i3,axisp2,0)*jac3di(i1+2-i10,i2+2-i20,i3-
     & i30))+(rsxy(i1-2,i2+2,i3,axisp2,0)*jac3di(i1-2-i10,i2+2-i20,i3-
     & i30))+16*(rsxy(i1+1,i2-2,i3,axisp2,0)*jac3di(i1+1-i10,i2-2-i20,
     & i3-i30))-30*(rsxy(i1,i2-2,i3,axisp2,0)*jac3di(i1-i10,i2-2-i20,
     & i3-i30))+16*(rsxy(i1-1,i2-2,i3,axisp2,0)*jac3di(i1-1-i10,i2-2-
     & i20,i3-i30))-(rsxy(i1+2,i2-2,i3,axisp2,0)*jac3di(i1+2-i10,i2-2-
     & i20,i3-i30))-(rsxy(i1-2,i2-2,i3,axisp2,0)*jac3di(i1-2-i10,i2-2-
     & i20,i3-i30))-16*(rsxy(i1+1,i2+2,i3,axisp2,0)*jac3di(i1+1-i10,
     & i2+2-i20,i3-i30)))/(144.*dr(0)**2*dr(1))
                     a32sst = (128*(rsxy(i1+1,i2+1,i3,axisp2,1)*jac3di(
     & i1+1-i10,i2+1-i20,i3-i30))-240*(rsxy(i1,i2+1,i3,axisp2,1)*
     & jac3di(i1-i10,i2+1-i20,i3-i30))+128*(rsxy(i1-1,i2+1,i3,axisp2,
     & 1)*jac3di(i1-1-i10,i2+1-i20,i3-i30))-8*(rsxy(i1+2,i2+1,i3,
     & axisp2,1)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-8*(rsxy(i1-2,i2+1,
     & i3,axisp2,1)*jac3di(i1-2-i10,i2+1-i20,i3-i30))-128*(rsxy(i1+1,
     & i2-1,i3,axisp2,1)*jac3di(i1+1-i10,i2-1-i20,i3-i30))+240*(rsxy(
     & i1,i2-1,i3,axisp2,1)*jac3di(i1-i10,i2-1-i20,i3-i30))-128*(rsxy(
     & i1-1,i2-1,i3,axisp2,1)*jac3di(i1-1-i10,i2-1-i20,i3-i30))+8*(
     & rsxy(i1+2,i2-1,i3,axisp2,1)*jac3di(i1+2-i10,i2-1-i20,i3-i30))+
     & 8*(rsxy(i1-2,i2-1,i3,axisp2,1)*jac3di(i1-2-i10,i2-1-i20,i3-i30)
     & )+30*(rsxy(i1,i2+2,i3,axisp2,1)*jac3di(i1-i10,i2+2-i20,i3-i30))
     & -16*(rsxy(i1-1,i2+2,i3,axisp2,1)*jac3di(i1-1-i10,i2+2-i20,i3-
     & i30))+(rsxy(i1+2,i2+2,i3,axisp2,1)*jac3di(i1+2-i10,i2+2-i20,i3-
     & i30))+(rsxy(i1-2,i2+2,i3,axisp2,1)*jac3di(i1-2-i10,i2+2-i20,i3-
     & i30))+16*(rsxy(i1+1,i2-2,i3,axisp2,1)*jac3di(i1+1-i10,i2-2-i20,
     & i3-i30))-30*(rsxy(i1,i2-2,i3,axisp2,1)*jac3di(i1-i10,i2-2-i20,
     & i3-i30))+16*(rsxy(i1-1,i2-2,i3,axisp2,1)*jac3di(i1-1-i10,i2-2-
     & i20,i3-i30))-(rsxy(i1+2,i2-2,i3,axisp2,1)*jac3di(i1+2-i10,i2-2-
     & i20,i3-i30))-(rsxy(i1-2,i2-2,i3,axisp2,1)*jac3di(i1-2-i10,i2-2-
     & i20,i3-i30))-16*(rsxy(i1+1,i2+2,i3,axisp2,1)*jac3di(i1+1-i10,
     & i2+2-i20,i3-i30)))/(144.*dr(0)**2*dr(1))
                     a33sst = (128*(rsxy(i1+1,i2+1,i3,axisp2,2)*jac3di(
     & i1+1-i10,i2+1-i20,i3-i30))-240*(rsxy(i1,i2+1,i3,axisp2,2)*
     & jac3di(i1-i10,i2+1-i20,i3-i30))+128*(rsxy(i1-1,i2+1,i3,axisp2,
     & 2)*jac3di(i1-1-i10,i2+1-i20,i3-i30))-8*(rsxy(i1+2,i2+1,i3,
     & axisp2,2)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-8*(rsxy(i1-2,i2+1,
     & i3,axisp2,2)*jac3di(i1-2-i10,i2+1-i20,i3-i30))-128*(rsxy(i1+1,
     & i2-1,i3,axisp2,2)*jac3di(i1+1-i10,i2-1-i20,i3-i30))+240*(rsxy(
     & i1,i2-1,i3,axisp2,2)*jac3di(i1-i10,i2-1-i20,i3-i30))-128*(rsxy(
     & i1-1,i2-1,i3,axisp2,2)*jac3di(i1-1-i10,i2-1-i20,i3-i30))+8*(
     & rsxy(i1+2,i2-1,i3,axisp2,2)*jac3di(i1+2-i10,i2-1-i20,i3-i30))+
     & 8*(rsxy(i1-2,i2-1,i3,axisp2,2)*jac3di(i1-2-i10,i2-1-i20,i3-i30)
     & )+30*(rsxy(i1,i2+2,i3,axisp2,2)*jac3di(i1-i10,i2+2-i20,i3-i30))
     & -16*(rsxy(i1-1,i2+2,i3,axisp2,2)*jac3di(i1-1-i10,i2+2-i20,i3-
     & i30))+(rsxy(i1+2,i2+2,i3,axisp2,2)*jac3di(i1+2-i10,i2+2-i20,i3-
     & i30))+(rsxy(i1-2,i2+2,i3,axisp2,2)*jac3di(i1-2-i10,i2+2-i20,i3-
     & i30))+16*(rsxy(i1+1,i2-2,i3,axisp2,2)*jac3di(i1+1-i10,i2-2-i20,
     & i3-i30))-30*(rsxy(i1,i2-2,i3,axisp2,2)*jac3di(i1-i10,i2-2-i20,
     & i3-i30))+16*(rsxy(i1-1,i2-2,i3,axisp2,2)*jac3di(i1-1-i10,i2-2-
     & i20,i3-i30))-(rsxy(i1+2,i2-2,i3,axisp2,2)*jac3di(i1+2-i10,i2-2-
     & i20,i3-i30))-(rsxy(i1-2,i2-2,i3,axisp2,2)*jac3di(i1-2-i10,i2-2-
     & i20,i3-i30))-16*(rsxy(i1+1,i2+2,i3,axisp2,2)*jac3di(i1+1-i10,
     & i2+2-i20,i3-i30)))/(144.*dr(0)**2*dr(1))
                     ust=urs4(i1,i2,i3,ex)
                     usst=urrs4(i1,i2,i3,ex)
                     ustt=urss4(i1,i2,i3,ex)
                     vst  =urs4(i1,i2,i3,ey)
                     vsst=urrs4(i1,i2,i3,ey)
                     vstt=urss4(i1,i2,i3,ey)
                     wst  =urs4(i1,i2,i3,ez)
                     wsst=urrs4(i1,i2,i3,ez)
                     wstt=urss4(i1,i2,i3,ez)
                  end if
                  tau11=rsxy(i1,i2,i3,axisp1,0)
                  tau12=rsxy(i1,i2,i3,axisp1,1)
                  tau13=rsxy(i1,i2,i3,axisp1,2)
                  tau21=rsxy(i1,i2,i3,axisp2,0)
                  tau22=rsxy(i1,i2,i3,axisp2,1)
                  tau23=rsxy(i1,i2,i3,axisp2,2)
                  uex=u(i1,i2,i3,ex)
                  uey=u(i1,i2,i3,ey)
                  uez=u(i1,i2,i3,ez)
                  ur=(8.*(u(i1+is1,i2+is2,i3+is3,ex)-u(i1-is1,i2-is2,
     & i3-is3,ex))-(u(i1+2*is1,i2+2*is2,i3+2*is3,ex)-u(i1-2*is1,i2-2*
     & is2,i3-2*is3,ex)))/(12.*dra)
                  vr=(8.*(u(i1+is1,i2+is2,i3+is3,ey)-u(i1-is1,i2-is2,
     & i3-is3,ey))-(u(i1+2*is1,i2+2*is2,i3+2*is3,ey)-u(i1-2*is1,i2-2*
     & is2,i3-2*is3,ey)))/(12.*dra)
                  urr=(-30.*u(i1,i2,i3,ex)+16.*(u(i1+is1,i2+is2,i3+is3,
     & ex)+u(i1-is1,i2-is2,i3-is3,ex))-(u(i1+2*is1,i2+2*is2,i3+2*is3,
     & ex)+u(i1-2*is1,i2-2*is2,i3-2*is3,ex)))/(12.*dra**2)
                  vrr=(-30.*u(i1,i2,i3,ey)+16.*(u(i1+is1,i2+is2,i3+is3,
     & ey)+u(i1-is1,i2-is2,i3-is3,ey))-(u(i1+2*is1,i2+2*is2,i3+2*is3,
     & ey)+u(i1-2*is1,i2-2*is2,i3-2*is3,ey)))/(12.*dra**2)
                  urs=(8.*((8.*(u(i1+is1,i2+js2+is2,i3,ex)-u(i1-is1,i2+
     & js2-is2,i3,ex))-(u(i1+2*is1,i2+js2+2*is2,i3,ex)-u(i1-2*is1,i2+
     & js2-2*is2,i3,ex)))/(12.*dra)-(8.*(u(i1+is1,i2-js2+is2,i3,ex)-u(
     & i1-is1,i2-js2-is2,i3,ex))-(u(i1+2*is1,i2-js2+2*is2,i3,ex)-u(i1-
     & 2*is1,i2-js2-2*is2,i3,ex)))/(12.*dra))-((8.*(u(i1+is1,i2+2*js2+
     & is2,i3,ex)-u(i1-is1,i2+2*js2-is2,i3,ex))-(u(i1+2*is1,i2+2*js2+
     & 2*is2,i3,ex)-u(i1-2*is1,i2+2*js2-2*is2,i3,ex)))/(12.*dra)-(8.*(
     & u(i1+is1,i2-2*js2+is2,i3,ex)-u(i1-is1,i2-2*js2-is2,i3,ex))-(u(
     & i1+2*is1,i2-2*js2+2*is2,i3,ex)-u(i1-2*is1,i2-2*js2-2*is2,i3,ex)
     & ))/(12.*dra)))/(12.*dsa)
                  vrs=(8.*((8.*(u(i1+is1,i2+js2+is2,i3,ey)-u(i1-is1,i2+
     & js2-is2,i3,ey))-(u(i1+2*is1,i2+js2+2*is2,i3,ey)-u(i1-2*is1,i2+
     & js2-2*is2,i3,ey)))/(12.*dra)-(8.*(u(i1+is1,i2-js2+is2,i3,ey)-u(
     & i1-is1,i2-js2-is2,i3,ey))-(u(i1+2*is1,i2-js2+2*is2,i3,ey)-u(i1-
     & 2*is1,i2-js2-2*is2,i3,ey)))/(12.*dra))-((8.*(u(i1+is1,i2+2*js2+
     & is2,i3,ey)-u(i1-is1,i2+2*js2-is2,i3,ey))-(u(i1+2*is1,i2+2*js2+
     & 2*is2,i3,ey)-u(i1-2*is1,i2+2*js2-2*is2,i3,ey)))/(12.*dra)-(8.*(
     & u(i1+is1,i2-2*js2+is2,i3,ey)-u(i1-is1,i2-2*js2-is2,i3,ey))-(u(
     & i1+2*is1,i2-2*js2+2*is2,i3,ey)-u(i1-2*is1,i2-2*js2-2*is2,i3,ey)
     & ))/(12.*dra)))/(12.*dsa)
                  urrs=(8.*((-30.*u(i1,i2+js2,i3,ex)+16.*(u(i1+is1,i2+
     & js2+is2,i3,ex)+u(i1-is1,i2+js2-is2,i3,ex))-(u(i1+2*is1,i2+js2+
     & 2*is2,i3,ex)+u(i1-2*is1,i2+js2-2*is2,i3,ex)))/(12.*dra**2)-(-
     & 30.*u(i1,i2-js2,i3,ex)+16.*(u(i1+is1,i2-js2+is2,i3,ex)+u(i1-
     & is1,i2-js2-is2,i3,ex))-(u(i1+2*is1,i2-js2+2*is2,i3,ex)+u(i1-2*
     & is1,i2-js2-2*is2,i3,ex)))/(12.*dra**2))-((-30.*u(i1,i2+2*js2,
     & i3,ex)+16.*(u(i1+is1,i2+2*js2+is2,i3,ex)+u(i1-is1,i2+2*js2-is2,
     & i3,ex))-(u(i1+2*is1,i2+2*js2+2*is2,i3,ex)+u(i1-2*is1,i2+2*js2-
     & 2*is2,i3,ex)))/(12.*dra**2)-(-30.*u(i1,i2-2*js2,i3,ex)+16.*(u(
     & i1+is1,i2-2*js2+is2,i3,ex)+u(i1-is1,i2-2*js2-is2,i3,ex))-(u(i1+
     & 2*is1,i2-2*js2+2*is2,i3,ex)+u(i1-2*is1,i2-2*js2-2*is2,i3,ex)))
     & /(12.*dra**2)))/(12.*dsa)
                  vrrs=(8.*((-30.*u(i1,i2+js2,i3,ey)+16.*(u(i1+is1,i2+
     & js2+is2,i3,ey)+u(i1-is1,i2+js2-is2,i3,ey))-(u(i1+2*is1,i2+js2+
     & 2*is2,i3,ey)+u(i1-2*is1,i2+js2-2*is2,i3,ey)))/(12.*dra**2)-(-
     & 30.*u(i1,i2-js2,i3,ey)+16.*(u(i1+is1,i2-js2+is2,i3,ey)+u(i1-
     & is1,i2-js2-is2,i3,ey))-(u(i1+2*is1,i2-js2+2*is2,i3,ey)+u(i1-2*
     & is1,i2-js2-2*is2,i3,ey)))/(12.*dra**2))-((-30.*u(i1,i2+2*js2,
     & i3,ey)+16.*(u(i1+is1,i2+2*js2+is2,i3,ey)+u(i1-is1,i2+2*js2-is2,
     & i3,ey))-(u(i1+2*is1,i2+2*js2+2*is2,i3,ey)+u(i1-2*is1,i2+2*js2-
     & 2*is2,i3,ey)))/(12.*dra**2)-(-30.*u(i1,i2-2*js2,i3,ey)+16.*(u(
     & i1+is1,i2-2*js2+is2,i3,ey)+u(i1-is1,i2-2*js2-is2,i3,ey))-(u(i1+
     & 2*is1,i2-2*js2+2*is2,i3,ey)+u(i1-2*is1,i2-2*js2-2*is2,i3,ey)))
     & /(12.*dra**2)))/(12.*dsa)
                  urrr=(u(i1+2*is1,i2+2*is2,i3+2*is3,ex)-2.*u(i1+is1,
     & i2+is2,i3+is3,ex)+2.*u(i1-is1,i2-is2,i3-is3,ex)-u(i1-2*is1,i2-
     & 2*is2,i3-2*is3,ex))/(2.*dra**3)
                  vrrr=(u(i1+2*is1,i2+2*is2,i3+2*is3,ey)-2.*u(i1+is1,
     & i2+is2,i3+is3,ey)+2.*u(i1-is1,i2-is2,i3-is3,ey)-u(i1-2*is1,i2-
     & 2*is2,i3-2*is3,ey))/(2.*dra**3)
                  urss=(8.*(((-30.*u(i1+is1,i2,i3,ex)+16.*(u(i1+is1+
     & js1,i2+js2,i3,ex)+u(i1+is1-js1,i2-js2,i3,ex))-(u(i1+is1+2*js1,
     & i2+2*js2,i3,ex)+u(i1+is1-2*js1,i2-2*js2,i3,ex)))/(12.*dsa**2))-
     & ((-30.*u(i1-is1,i2,i3,ex)+16.*(u(i1-is1+js1,i2+js2,i3,ex)+u(i1-
     & is1-js1,i2-js2,i3,ex))-(u(i1-is1+2*js1,i2+2*js2,i3,ex)+u(i1-
     & is1-2*js1,i2-2*js2,i3,ex)))/(12.*dsa**2)))-(((-30.*u(i1+2*is1,
     & i2,i3,ex)+16.*(u(i1+2*is1+js1,i2+js2,i3,ex)+u(i1+2*is1-js1,i2-
     & js2,i3,ex))-(u(i1+2*is1+2*js1,i2+2*js2,i3,ex)+u(i1+2*is1-2*js1,
     & i2-2*js2,i3,ex)))/(12.*dsa**2))-((-30.*u(i1-2*is1,i2,i3,ex)+
     & 16.*(u(i1-2*is1+js1,i2+js2,i3,ex)+u(i1-2*is1-js1,i2-js2,i3,ex))
     & -(u(i1-2*is1+2*js1,i2+2*js2,i3,ex)+u(i1-2*is1-2*js1,i2-2*js2,
     & i3,ex)))/(12.*dsa**2))))/(12.*dra)
                  vrss=(8.*(((-30.*u(i1+is1,i2,i3,ey)+16.*(u(i1+is1+
     & js1,i2+js2,i3,ey)+u(i1+is1-js1,i2-js2,i3,ey))-(u(i1+is1+2*js1,
     & i2+2*js2,i3,ey)+u(i1+is1-2*js1,i2-2*js2,i3,ey)))/(12.*dsa**2))-
     & ((-30.*u(i1-is1,i2,i3,ey)+16.*(u(i1-is1+js1,i2+js2,i3,ey)+u(i1-
     & is1-js1,i2-js2,i3,ey))-(u(i1-is1+2*js1,i2+2*js2,i3,ey)+u(i1-
     & is1-2*js1,i2-2*js2,i3,ey)))/(12.*dsa**2)))-(((-30.*u(i1+2*is1,
     & i2,i3,ey)+16.*(u(i1+2*is1+js1,i2+js2,i3,ey)+u(i1+2*is1-js1,i2-
     & js2,i3,ey))-(u(i1+2*is1+2*js1,i2+2*js2,i3,ey)+u(i1+2*is1-2*js1,
     & i2-2*js2,i3,ey)))/(12.*dsa**2))-((-30.*u(i1-2*is1,i2,i3,ey)+
     & 16.*(u(i1-2*is1+js1,i2+js2,i3,ey)+u(i1-2*is1-js1,i2-js2,i3,ey))
     & -(u(i1-2*is1+2*js1,i2+2*js2,i3,ey)+u(i1-2*is1-2*js1,i2-2*js2,
     & i3,ey)))/(12.*dsa**2))))/(12.*dra)
                  div = ux43(i1,i2,i3,ex)+uy43(i1,i2,i3,ey)+uz43(i1,i2,
     & i3,ez)
                  a11zp1= (rsxy(i1+is1,i2+is2,i3+is3,axis,0)*jac3di(i1+
     & is1-i10,i2+is2-i20,i3+is3-i30))
                  a11zm1= (rsxy(i1-is1,i2-is2,i3-is3,axis,0)*jac3di(i1-
     & is1-i10,i2-is2-i20,i3-is3-i30))
                  a11zp2= (rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axis,0)*
     & jac3di(i1+2*is1-i10,i2+2*is2-i20,i3+2*is3-i30))
                  a11zm2= (rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,0)*
     & jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))
                  a12zp1= (rsxy(i1+is1,i2+is2,i3+is3,axis,1)*jac3di(i1+
     & is1-i10,i2+is2-i20,i3+is3-i30))
                  a12zm1= (rsxy(i1-is1,i2-is2,i3-is3,axis,1)*jac3di(i1-
     & is1-i10,i2-is2-i20,i3-is3-i30))
                  a12zp2= (rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axis,1)*
     & jac3di(i1+2*is1-i10,i2+2*is2-i20,i3+2*is3-i30))
                  a12zm2= (rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,1)*
     & jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))
                  a13zp1= (rsxy(i1+is1,i2+is2,i3+is3,axis,2)*jac3di(i1+
     & is1-i10,i2+is2-i20,i3+is3-i30))
                  a13zm1= (rsxy(i1-is1,i2-is2,i3-is3,axis,2)*jac3di(i1-
     & is1-i10,i2-is2-i20,i3-is3-i30))
                  a13zp2= (rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axis,2)*
     & jac3di(i1+2*is1-i10,i2+2*is2-i20,i3+2*is3-i30))
                  a13zm2= (rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,2)*
     & jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))
                  a21zp1= (rsxy(i1+js1,i2+js2,i3+js3,axisp1,0)*jac3di(
     & i1+js1-i10,i2+js2-i20,i3+js3-i30))
                  a21zm1= (rsxy(i1-js1,i2-js2,i3-js3,axisp1,0)*jac3di(
     & i1-js1-i10,i2-js2-i20,i3-js3-i30))
                  a21zp2= (rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,0)*
     & jac3di(i1+2*js1-i10,i2+2*js2-i20,i3+2*js3-i30))
                  a21zm2= (rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axisp1,0)*
     & jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))
                  a22zp1= (rsxy(i1+js1,i2+js2,i3+js3,axisp1,1)*jac3di(
     & i1+js1-i10,i2+js2-i20,i3+js3-i30))
                  a22zm1= (rsxy(i1-js1,i2-js2,i3-js3,axisp1,1)*jac3di(
     & i1-js1-i10,i2-js2-i20,i3-js3-i30))
                  a22zp2= (rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,1)*
     & jac3di(i1+2*js1-i10,i2+2*js2-i20,i3+2*js3-i30))
                  a22zm2= (rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axisp1,1)*
     & jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))
                  a23zp1= (rsxy(i1+js1,i2+js2,i3+js3,axisp1,2)*jac3di(
     & i1+js1-i10,i2+js2-i20,i3+js3-i30))
                  a23zm1= (rsxy(i1-js1,i2-js2,i3-js3,axisp1,2)*jac3di(
     & i1-js1-i10,i2-js2-i20,i3-js3-i30))
                  a23zp2= (rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,2)*
     & jac3di(i1+2*js1-i10,i2+2*js2-i20,i3+2*js3-i30))
                  a23zm2= (rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axisp1,2)*
     & jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))
                  a31zp1= (rsxy(i1+ks1,i2+ks2,i3+ks3,axisp2,0)*jac3di(
     & i1+ks1-i10,i2+ks2-i20,i3+ks3-i30))
                  a31zm1= (rsxy(i1-ks1,i2-ks2,i3-ks3,axisp2,0)*jac3di(
     & i1-ks1-i10,i2-ks2-i20,i3-ks3-i30))
                  a31zp2= (rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axisp2,0)*
     & jac3di(i1+2*ks1-i10,i2+2*ks2-i20,i3+2*ks3-i30))
                  a31zm2= (rsxy(i1-2*ks1,i2-2*ks2,i3-2*ks3,axisp2,0)*
     & jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30))
                  a32zp1= (rsxy(i1+ks1,i2+ks2,i3+ks3,axisp2,1)*jac3di(
     & i1+ks1-i10,i2+ks2-i20,i3+ks3-i30))
                  a32zm1= (rsxy(i1-ks1,i2-ks2,i3-ks3,axisp2,1)*jac3di(
     & i1-ks1-i10,i2-ks2-i20,i3-ks3-i30))
                  a32zp2= (rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axisp2,1)*
     & jac3di(i1+2*ks1-i10,i2+2*ks2-i20,i3+2*ks3-i30))
                  a32zm2= (rsxy(i1-2*ks1,i2-2*ks2,i3-2*ks3,axisp2,1)*
     & jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30))
                  a33zp1= (rsxy(i1+ks1,i2+ks2,i3+ks3,axisp2,2)*jac3di(
     & i1+ks1-i10,i2+ks2-i20,i3+ks3-i30))
                  a33zm1= (rsxy(i1-ks1,i2-ks2,i3-ks3,axisp2,2)*jac3di(
     & i1-ks1-i10,i2-ks2-i20,i3-ks3-i30))
                  a33zp2= (rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axisp2,2)*
     & jac3di(i1+2*ks1-i10,i2+2*ks2-i20,i3+2*ks3-i30))
                  a33zm2= (rsxy(i1-2*ks1,i2-2*ks2,i3-2*ks3,axisp2,2)*
     & jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30))
                  ! conservative form of the divergence
                  divc=( 8.*(a11zp1*u(i1+  is1,i2+  is2,i3+  is3,ex)-
     & a11zm1*u(i1-  is1,i2-  is2,i3-  is3,ex)) -(a11zp2*u(i1+2*is1,
     & i2+2*is2,i3+2*is3,ex)-a11zm2*u(i1-2*is1,i2-2*is2,i3-2*is3,ex)) 
     & )/(12.*dra) +( 8.*(a12zp1*u(i1+  is1,i2+  is2,i3+  is3,ey)-
     & a12zm1*u(i1-  is1,i2-  is2,i3-  is3,ey)) -(a12zp2*u(i1+2*is1,
     & i2+2*is2,i3+2*is3,ey)-a12zm2*u(i1-2*is1,i2-2*is2,i3-2*is3,ey)) 
     & )/(12.*dra) +( 8.*(a13zp1*u(i1+  is1,i2+  is2,i3+  is3,ez)-
     & a13zm1*u(i1-  is1,i2-  is2,i3-  is3,ez)) -(a13zp2*u(i1+2*is1,
     & i2+2*is2,i3+2*is3,ez)-a13zm2*u(i1-2*is1,i2-2*is2,i3-2*is3,ez)) 
     & )/(12.*dra)  +( 8.*(a21zp1*u(i1+  js1,i2+  js2,i3+  js3,ex)-
     & a21zm1*u(i1-  js1,i2-  js2,i3-  js3,ex)) -(a21zp2*u(i1+2*js1,
     & i2+2*js2,i3+2*js3,ex)-a21zm2*u(i1-2*js1,i2-2*js2,i3-2*js3,ex)) 
     & )/(12.*dsa) +( 8.*(a22zp1*u(i1+  js1,i2+  js2,i3+  js3,ey)-
     & a22zm1*u(i1-  js1,i2-  js2,i3-  js3,ey)) -(a22zp2*u(i1+2*js1,
     & i2+2*js2,i3+2*js3,ey)-a22zm2*u(i1-2*js1,i2-2*js2,i3-2*js3,ey)) 
     & )/(12.*dsa) +( 8.*(a23zp1*u(i1+  js1,i2+  js2,i3+  js3,ez)-
     & a23zm1*u(i1-  js1,i2-  js2,i3-  js3,ez)) -(a23zp2*u(i1+2*js1,
     & i2+2*js2,i3+2*js3,ez)-a23zm2*u(i1-2*js1,i2-2*js2,i3-2*js3,ez)) 
     & )/(12.*dsa)  +( 8.*(a31zp1*u(i1+  ks1,i2+  ks2,i3+  ks3,ex)-
     & a31zm1*u(i1-  ks1,i2-  ks2,i3-  ks3,ex)) -(a31zp2*u(i1+2*ks1,
     & i2+2*ks2,i3+2*ks3,ex)-a31zm2*u(i1-2*ks1,i2-2*ks2,i3-2*ks3,ex)) 
     & )/(12.*dta) +( 8.*(a32zp1*u(i1+  ks1,i2+  ks2,i3+  ks3,ey)-
     & a32zm1*u(i1-  ks1,i2-  ks2,i3-  ks3,ey)) -(a32zp2*u(i1+2*ks1,
     & i2+2*ks2,i3+2*ks3,ey)-a32zm2*u(i1-2*ks1,i2-2*ks2,i3-2*ks3,ey)) 
     & )/(12.*dta) +( 8.*(a33zp1*u(i1+  ks1,i2+  ks2,i3+  ks3,ez)-
     & a33zm1*u(i1-  ks1,i2-  ks2,i3-  ks3,ez)) -(a33zp2*u(i1+2*ks1,
     & i2+2*ks2,i3+2*ks3,ez)-a33zm2*u(i1-2*ks1,i2-2*ks2,i3-2*ks3,ez)) 
     & )/(12.*dta)
                  divc=divc*(rx(i1,i2,i3)*(sy(i1,i2,i3)*tz(i1,i2,i3)-
     & sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(sz(i1,i2,i3)*tx(i1,i2,
     & i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,i3)*(sx(i1,i2,i3)*ty(
     & i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3)))
                  tau1Up1=tau11*(u(i1-2*is1,i2-2*is2,i3-2*is3,ex)-4.*u(
     & i1-is1,i2-is2,i3-is3,ex)+6.*u(i1,i2,i3,ex)-4.*u(i1+is1,i2+is2,
     & i3+is3,ex)+u(i1+2*is1,i2+2*is2,i3+2*is3,ex))+tau12*(u(i1-2*is1,
     & i2-2*is2,i3-2*is3,ey)-4.*u(i1-is1,i2-is2,i3-is3,ey)+6.*u(i1,i2,
     & i3,ey)-4.*u(i1+is1,i2+is2,i3+is3,ey)+u(i1+2*is1,i2+2*is2,i3+2*
     & is3,ey))+tau13*(u(i1-2*is1,i2-2*is2,i3-2*is3,ez)-4.*u(i1-is1,
     & i2-is2,i3-is3,ez)+6.*u(i1,i2,i3,ez)-4.*u(i1+is1,i2+is2,i3+is3,
     & ez)+u(i1+2*is1,i2+2*is2,i3+2*is3,ez))
                  tau2Up1=tau21*(u(i1-2*is1,i2-2*is2,i3-2*is3,ex)-4.*u(
     & i1-is1,i2-is2,i3-is3,ex)+6.*u(i1,i2,i3,ex)-4.*u(i1+is1,i2+is2,
     & i3+is3,ex)+u(i1+2*is1,i2+2*is2,i3+2*is3,ex))+tau22*(u(i1-2*is1,
     & i2-2*is2,i3-2*is3,ey)-4.*u(i1-is1,i2-is2,i3-is3,ey)+6.*u(i1,i2,
     & i3,ey)-4.*u(i1+is1,i2+is2,i3+is3,ey)+u(i1+2*is1,i2+2*is2,i3+2*
     & is3,ey))+tau23*(u(i1-2*is1,i2-2*is2,i3-2*is3,ez)-4.*u(i1-is1,
     & i2-is2,i3-is3,ez)+6.*u(i1,i2,i3,ez)-4.*u(i1+is1,i2+is2,i3+is3,
     & ez)+u(i1+2*is1,i2+2*is2,i3+2*is3,ez))
                  uLap=ulaplacian43(i1,i2,i3,ex)
                  vLap=ulaplacian43(i1,i2,i3,ey)
                  wLap=ulaplacian43(i1,i2,i3,ez)
                  tau1DotLap= tau11*uLap+tau12*vLap+tau13*wLap
                  tau2DotLap= tau21*uLap+tau22*vLap+tau23*wLap
                  errLapex=(c11*(-30.*u(i1,i2,i3,ex)+16.*(u(i1+is1,i2+
     & is2,i3+is3,ex)+u(i1-is1,i2-is2,i3-is3,ex))-(u(i1+2*is1,i2+2*
     & is2,i3+2*is3,ex)+u(i1-2*is1,i2-2*is2,i3-2*is3,ex)))/(12.*dra**
     & 2)+c22*(-30.*u(i1,i2,i3,ex)+16.*(u(i1+js1,i2+js2,i3+js3,ex)+u(
     & i1-js1,i2-js2,i3-js3,ex))-(u(i1+2*js1,i2+2*js2,i3+2*js3,ex)+u(
     & i1-2*js1,i2-2*js2,i3-2*js3,ex)))/(12.*dsa**2)+c33*(-30.*u(i1,
     & i2,i3,ex)+16.*(u(i1+ks1,i2+ks2,i3+ks3,ex)+u(i1-ks1,i2-ks2,i3-
     & ks3,ex))-(u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ex)+u(i1-2*ks1,i2-2*ks2,
     & i3-2*ks3,ex)))/(12.*dta**2)+c1*(8.*(u(i1+is1,i2+is2,i3+is3,ex)-
     & u(i1-is1,i2-is2,i3-is3,ex))-(u(i1+2*is1,i2+2*is2,i3+2*is3,ex)-
     & u(i1-2*is1,i2-2*is2,i3-2*is3,ex)))/(12.*dra)+c2*(8.*(u(i1+js1,
     & i2+js2,i3+js3,ex)-u(i1-js1,i2-js2,i3-js3,ex))-(u(i1+2*js1,i2+2*
     & js2,i3+2*js3,ex)-u(i1-2*js1,i2-2*js2,i3-2*js3,ex)))/(12.*dsa)+
     & c3*(8.*(u(i1+ks1,i2+ks2,i3+ks3,ex)-u(i1-ks1,i2-ks2,i3-ks3,ex))-
     & (u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ex)-u(i1-2*ks1,i2-2*ks2,i3-2*ks3,
     & ex)))/(12.*dta))-uLap
                  errLapey=(c11*(-30.*u(i1,i2,i3,ey)+16.*(u(i1+is1,i2+
     & is2,i3+is3,ey)+u(i1-is1,i2-is2,i3-is3,ey))-(u(i1+2*is1,i2+2*
     & is2,i3+2*is3,ey)+u(i1-2*is1,i2-2*is2,i3-2*is3,ey)))/(12.*dra**
     & 2)+c22*(-30.*u(i1,i2,i3,ey)+16.*(u(i1+js1,i2+js2,i3+js3,ey)+u(
     & i1-js1,i2-js2,i3-js3,ey))-(u(i1+2*js1,i2+2*js2,i3+2*js3,ey)+u(
     & i1-2*js1,i2-2*js2,i3-2*js3,ey)))/(12.*dsa**2)+c33*(-30.*u(i1,
     & i2,i3,ey)+16.*(u(i1+ks1,i2+ks2,i3+ks3,ey)+u(i1-ks1,i2-ks2,i3-
     & ks3,ey))-(u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ey)+u(i1-2*ks1,i2-2*ks2,
     & i3-2*ks3,ey)))/(12.*dta**2)+c1*(8.*(u(i1+is1,i2+is2,i3+is3,ey)-
     & u(i1-is1,i2-is2,i3-is3,ey))-(u(i1+2*is1,i2+2*is2,i3+2*is3,ey)-
     & u(i1-2*is1,i2-2*is2,i3-2*is3,ey)))/(12.*dra)+c2*(8.*(u(i1+js1,
     & i2+js2,i3+js3,ey)-u(i1-js1,i2-js2,i3-js3,ey))-(u(i1+2*js1,i2+2*
     & js2,i3+2*js3,ey)-u(i1-2*js1,i2-2*js2,i3-2*js3,ey)))/(12.*dsa)+
     & c3*(8.*(u(i1+ks1,i2+ks2,i3+ks3,ey)-u(i1-ks1,i2-ks2,i3-ks3,ey))-
     & (u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ey)-u(i1-2*ks1,i2-2*ks2,i3-2*ks3,
     & ey)))/(12.*dta))-vLap
                  errLapez=(c11*(-30.*u(i1,i2,i3,ez)+16.*(u(i1+is1,i2+
     & is2,i3+is3,ez)+u(i1-is1,i2-is2,i3-is3,ez))-(u(i1+2*is1,i2+2*
     & is2,i3+2*is3,ez)+u(i1-2*is1,i2-2*is2,i3-2*is3,ez)))/(12.*dra**
     & 2)+c22*(-30.*u(i1,i2,i3,ez)+16.*(u(i1+js1,i2+js2,i3+js3,ez)+u(
     & i1-js1,i2-js2,i3-js3,ez))-(u(i1+2*js1,i2+2*js2,i3+2*js3,ez)+u(
     & i1-2*js1,i2-2*js2,i3-2*js3,ez)))/(12.*dsa**2)+c33*(-30.*u(i1,
     & i2,i3,ez)+16.*(u(i1+ks1,i2+ks2,i3+ks3,ez)+u(i1-ks1,i2-ks2,i3-
     & ks3,ez))-(u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ez)+u(i1-2*ks1,i2-2*ks2,
     & i3-2*ks3,ez)))/(12.*dta**2)+c1*(8.*(u(i1+is1,i2+is2,i3+is3,ez)-
     & u(i1-is1,i2-is2,i3-is3,ez))-(u(i1+2*is1,i2+2*is2,i3+2*is3,ez)-
     & u(i1-2*is1,i2-2*is2,i3-2*is3,ez)))/(12.*dra)+c2*(8.*(u(i1+js1,
     & i2+js2,i3+js3,ez)-u(i1-js1,i2-js2,i3-js3,ez))-(u(i1+2*js1,i2+2*
     & js2,i3+2*js3,ez)-u(i1-2*js1,i2-2*js2,i3-2*js3,ez)))/(12.*dsa)+
     & c3*(8.*(u(i1+ks1,i2+ks2,i3+ks3,ez)-u(i1-ks1,i2-ks2,i3-ks3,ez))-
     & (u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ez)-u(i1-2*ks1,i2-2*ks2,i3-2*ks3,
     & ez)))/(12.*dta))-wLap
                  ! f1 := Dzr(Dpr(Dmr( a11*u + a12*v )))(i1,i2,i3)/dra^3 - cur*Dzr(u)(i1,i2,i3)/dra - cvr*Dzr(v)(i1,i2,i3)/dra - gI:
                    call ogf3d(ep,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,
     & i2,i3,2),t,uv0(0),uv0(1),uv0(2))
                    call ogf3d(ep,xy(i1-is1,i2-is2,i3-is3,0),xy(i1-is1,
     & i2-is2,i3-is3,1),xy(i1-is1,i2-is2,i3-is3,2),t,uvm(0),uvm(1),
     & uvm(2))
                    call ogf3d(ep,xy(i1+is1,i2+is2,i3+is3,0),xy(i1+is1,
     & i2+is2,i3+is3,1),xy(i1+is1,i2+is2,i3+is3,2),t,uvp(0),uvp(1),
     & uvp(2))
                    call ogf3d(ep,xy(i1-2*is1,i2-2*is2,i3-2*is3,0),xy(
     & i1-2*is1,i2-2*is2,i3-2*is3,1),xy(i1-2*is1,i2-2*is2,i3-2*is3,2),
     & t,uvm2(0),uvm2(1),uvm2(2))
                    call ogf3d(ep,xy(i1+2*is1,i2+2*is2,i3+2*is3,0),xy(
     & i1+2*is1,i2+2*is2,i3+2*is3,1),xy(i1+2*is1,i2+2*is2,i3+2*is3,2),
     & t,uvp2(0),uvp2(1),uvp2(2))
                  tau1DotU = tau11*(uex-uv0(0))+tau12*(uey-uv0(1))+
     & tau13*(uez-uv0(2))
                  tau2DotU = tau21*(uex-uv0(0))+tau22*(uey-uv0(1))+
     & tau23*(uez-uv0(2))
                  write(*,'("  bc4: (i1,i2,i3)=(",i6,",",i6,",",i6,") (
     & side,axis)=(",i2,",",i2,")")') i1,i2,i3,side,axis
                 !  write(*,'("  bc4: a1=(",3e10.2,"), tau1=(",3e10.2,"), tau2=(",3e10.2,")")') a11,a12,a13,tau11,tau12,tau13,tau21,tau22,tau23
                 !  write(*,'("  bc4: a11r,a12r,a13r=",3e10.2)') a11r,a12r,a13r
                 !  write(*,'("  bc4: a11s,a12s,a13s=",3e10.2)') a11s,a12s,a13s
                 !  write(*,'("  bc4: a11t,a12t,a13t=",3e10.2)') a11t,a12t,a13t
                 !  write(*,'("  bc4: a11ss,a12ss,a13ss=",3e10.2)') a11ss,a12ss,a13ss
                 ! 
                 !  write(*,'("  bc4: a21r,a22r,a23r=",3e10.2)') a21r,a22r,a23r
                 !  write(*,'("  bc4: a21s,a22s,a23s=",3e10.2)') a21s,a22s,a23s
                 !  write(*,'("  bc4: a21t,a22t,a23t=",3e10.2)') a21t,a22t,a23t
                 !  write(*,'("  bc4: a21ss,a22ss,a23ss=",3e10.2)') a21ss,a22ss,a23ss
                 ! 
                 !  write(*,'("  bc4: a31r,a32r,a33r=",3e10.2)') a31r,a32r,a33r
                 !  write(*,'("  bc4: a31s,a32s,a33s=",3e10.2)') a31s,a32s,a33s
                 !  write(*,'("  bc4: a31t,a32t,a33t=",3e10.2)') a31t,a32t,a33t
                 !  write(*,'("  bc4: a31tt,a32tt,a33tt=",3e10.2)') a31tt,a32tt,a33tt
                 ! 
                 !  write(*,'("  bc4: c11,c22,c33,c1,c2,c3=",6e10.2)') c11,c22,c33,c1,c2,c3
                 !  write(*,'("  bc4: c11r,c22r,c33r,c1r,c2r,c3r=",6e10.2)') c11r,c22r,c33r,c1r,c2r,c3r
                 !  write(*,'("  bc4: c11s,c22s,c33s,c1s,c2s,c3s=",6e10.2)') c11s,c22s,c33s,c1s,c2s,c3s
                 !  write(*,'("  bc4: c11t,c22t,c33t,c1t,c2t,c3t=",6e10.2)') c11t,c22t,c33t,c1t,c2t,c3t
                 ! print neighbours
                 !  do m3=-2,2
                 !  do m2=-2,2
                 !  do m1=-2,2
                 !    OGF3D(i1+m1,i2+m2,i3+m3,t,uvm(0),uvm(1),uvm(2)
                 !    write(*,'("  err(E(",i2,",",i2,",",i2,") =",3e9.1)')!       i1+m1,i2+m2,i3+m3,!       u(i1+m1,i2+m2,i3+m3,ex)-uvm(0),!       u(i1+m1,i2+m2,i3+m3,ey)-uvm(1),!       u(i1+m1,i2+m2,i3+m3,ez)-uvm(2)
                 !  end do
                 !  end do
                 !  end do
                  write(*,'("  bc4: E(-1)=",3e11.3,", E(-2)=",3e11.3)')
     &  u(i1-is1,i2-is2,i3-is3,ex),u(i1-is1,i2-is2,i3-is3,ey),u(i1-
     & is1,i2-is2,i3-is3,ez),u(i1-2*is1,i2-2*is2,i3-2*is3,ex),u(i1-2*
     & is1,i2-2*is2,i3-2*is3,ey),u(i1-2*is1,i2-2*is2,i3-2*is3,ez)
                  write(*,'("  bc4: err(E)(-1) =",3e11.3," err(E)(-2)
     & =",3e11.3)') u(i1-is1,i2-is2,i3-is3,ex)-uvm(0),u(i1-is1,i2-is2,
     & i3-is3,ey)-uvm(1),u(i1-is1,i2-is2,i3-is3,ez)-uvm(2),u(i1-2*is1,
     & i2-2*is2,i3-2*is3,ex)-uvm2(0),u(i1-2*is1,i2-2*is2,i3-2*is3,ey)-
     & uvm2(1),u(i1-2*is1,i2-2*is2,i3-2*is3,ez)-uvm2(2)
                  write(*,'("  bc4: err(E)(0) =",3e11.3)') u(i1,i2,i3,
     & ex)-uv0(0),u(i1,i2,i3,ey)-uv0(1),u(i1,i2,i3,ez)-uv0(2)
                  write(*,'("  bc4: err(tau1.u)=",e9.1,", err(tau2.u)
     & =",e9.1," div4(u)=",e9.1," divc(u)=",e9.1,", divc2=",e9.1)') 
     & tau1DotU,tau2DotU,div,divc,divc2
                 ! ttu11 = tau1.u(-1), ttu12 = tau1.u(-2)
                 ! ttu21 = tau2.u(-1), ttu22 = tau2.u(-2)
                  write(*,'("  bc4: err(tau1.u(-1,-2))=",2e9.1," err(
     & tau2.u(-1,-2))=",2e9.1)')tau11*(u(i1-is1,i2-is2,i3-is3,ex)-uvm(
     & 0))+tau12*(u(i1-is1,i2-is2,i3-is3,ey)-uvm(1))+tau13*(u(i1-is1,
     & i2-is2,i3-is3,ez)-uvm(2)),tau11*(u(i1-2*is1,i2-2*is2,i3-2*is3,
     & ex)-uvm2(0))+tau12*(u(i1-2*is1,i2-2*is2,i3-2*is3,ey)-uvm2(1))+
     & tau13*(u(i1-2*is1,i2-2*is2,i3-2*is3,ez)-uvm2(2)),tau21*(u(i1-
     & is1,i2-is2,i3-is3,ex)-uvm(0))+tau22*(u(i1-is1,i2-is2,i3-is3,ey)
     & -uvm(1))+tau23*(u(i1-is1,i2-is2,i3-is3,ez)-uvm(2)),tau21*(u(i1-
     & 2*is1,i2-2*is2,i3-2*is3,ex)-uvm2(0))+tau22*(u(i1-2*is1,i2-2*
     & is2,i3-2*is3,ey)-uvm2(1))+tau23*(u(i1-2*is1,i2-2*is2,i3-2*is3,
     & ez)-uvm2(2))
                  write(*,'("  bc4: a1.extrap(u(-2))=",e10.2)') a11*(  
     & u(i1-2*is1,i2-2*is2,i3-2*is3,ex)-6.*u(i1-is1,i2-is2,i3-is3,ex)+
     & 15.*u(i1,i2,i3,ex)-20.*u(i1+is1,i2+is2,i3+is3,ex)+15.*u(i1+2*
     & is1,i2+2*is2,i3+2*is3,ex)-6.*u(i1+3*is1,i2+3*is2,i3+3*is3,ex)+
     & u(i1+4*is1,i2+4*is2,i3+4*is3,ex) )+a12*(  u(i1-2*is1,i2-2*is2,
     & i3-2*is3,ey)-6.*u(i1-is1,i2-is2,i3-is3,ey)+15.*u(i1,i2,i3,ey)-
     & 20.*u(i1+is1,i2+is2,i3+is3,ey)+15.*u(i1+2*is1,i2+2*is2,i3+2*
     & is3,ey)-6.*u(i1+3*is1,i2+3*is2,i3+3*is3,ey)+u(i1+4*is1,i2+4*
     & is2,i3+4*is3,ey) )+a13*(  u(i1-2*is1,i2-2*is2,i3-2*is3,ez)-6.*
     & u(i1-is1,i2-is2,i3-is3,ez)+15.*u(i1,i2,i3,ez)-20.*u(i1+is1,i2+
     & is2,i3+is3,ez)+15.*u(i1+2*is1,i2+2*is2,i3+2*is3,ez)-6.*u(i1+3*
     & is1,i2+3*is2,i3+3*is3,ez)+u(i1+4*is1,i2+4*is2,i3+4*is3,ez) )
                 ! These need use to recompute ttu11,...
                 ! write(*,'("  bc4: tau1.u(-1)-ttu11=",e9.1,", tau1.u(2)-ttu12=",e9.1)')!   tau11*u(i1-is1,i2-is2,i3-is3,ex)+tau12*u(i1-is1,i2-is2,i3-is3,ey)+tau13*u(i1-is1,i2-is2,i3-is3,ez) -ttu11,!   tau11*u(i1-2*is1,i2-2*is2,i3-2*is3,ex)+tau12*u(i1-2*is1,i2-2*is2,i3-2*is3,ey)!  +tau13*u(i1-2*is1,i2-2*is2,i3-2*is3,ez) -ttu12
                 ! write(*,'("  bc4: tau2.u(-1)-ttu21=",e9.1,", tau2.u(2)-ttu22=",e9.1)')!   tau21*u(i1-is1,i2-is2,i3-is3,ex)+tau22*u(i1-is1,i2-is2,i3-is3,ey)+tau23*u(i1-is1,i2-is2,i3-is3,ez) -ttu21,!   tau21*u(i1-2*is1,i2-2*is2,i3-2*is3,ex)+tau22*u(i1-2*is1,i2-2*is2,i3-2*is3,ey)!  +tau23*u(i1-2*is1,i2-2*is2,i3-2*is3,ez) -ttu22
                  ! for now remove the error in the extrapolation ************
                  gIVf1 = tau11*( uvm2(0)-4.*uvm(0)+6.*uv0(0)-4.*uvp(0)
     & +uvp2(0)) +tau12*( uvm2(1)-4.*uvm(1)+6.*uv0(1)-4.*uvp(1)+uvp2(
     & 1)) +tau13*( uvm2(2)-4.*uvm(2)+6.*uv0(2)-4.*uvp(2)+uvp2(2))
                  gIVf2 = tau21*( uvm2(0)-4.*uvm(0)+6.*uv0(0)-4.*uvp(0)
     & +uvp2(0)) +tau22*( uvm2(1)-4.*uvm(1)+6.*uv0(1)-4.*uvp(1)+uvp2(
     & 1)) +tau23*( uvm2(2)-4.*uvm(2)+6.*uv0(2)-4.*uvp(2)+uvp2(2))
                  write(*,'("  bc4: tau1.D+4u-gIV1=",e9.1,", tau2.D+4u-
     & gIV2=",e9.1)') tau1Up1-gIVf1,tau2Up1-gIVf2
                   ! For TZ: utt0 = utt - ett + Lap(e)
                    call ogDeriv3(ep, 0,2,0,0, xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t, ex,uxx, ey,vxx, ez,wxx)
                    call ogDeriv3(ep, 0,0,2,0, xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t, ex,uyy, ey,vyy, ez,wyy)
                    call ogDeriv3(ep, 0,0,0,2, xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t, ex,uzz, ey,vzz, ez,wzz)
                  utt00=uxx+uyy+uzz
                  vtt00=vxx+vyy+vzz
                  wtt00=wxx+wyy+wzz
                  write(*,'("  bc4: Lu-utt=",e10.2," Lv-vtt=",e10.2," 
     & Lw-wtt=",e10.2)') uLap-utt00,vLap-vtt00,wLap-wtt00
                  write(*,'("  bc4: tau1.(L\uv-\uvtt)=",e10.2," tau2.(
     & L\uv-\uvtt)=",e10.2)') tau11*(uLap-utt00)+tau12*(vLap-vtt00)+
     & tau13*(wLap-wtt00), tau21*(uLap-utt00)+tau22*(vLap-vtt00)+
     & tau23*(wLap-wtt00)
                  ! '
                  ! write(*,'("  bc4: tau1.Lap=",e9.1,", tau2.Lap=",e9.1)')tau1DotLap,tau2DotLap
                  write(*,'("  bc4: err(lap43-(c11*urr...))=",3e9.1)') 
     & errLapex,errLapey,errLapez
                  write(*,'("  bc4: err(Delta u)=",3e9.1)') uLap-utt00,
     & vLap-vtt00,wLap-wtt00
                 !  write(*,'(" error in a1r.Delta u =",e11.3)') a11r*uLap+a12r*vLap+ a13r*wLap-a11r*utt00-a12r*vtt00-a13r*wtt00 
                 !  write(*,'(" error in (Delta u).r=",e11.3," computed,true=",2e11.3)') !     ( c11*urrr+ c22*urss + c33*urtt + c1*urr + c2*urs + c3*urt !       +c11r*urr+c22r*uss+c33r*utt+c1r*ur+c2r*us+c3r*ut)-!       ( 8.*((uxxp1+uyyp1+uzzp1)-(uxxm1+uyym1+uzzm1))-((uxxp2+uyyp2+uzzp2)-(uxxm2+uyym2+uzzm2)) )/(12.*dra),!    ( c11*urrr+ c22*urss + c33*urtt + c1*urr + c2*urs + c3*urt !       +c11r*urr+c22r*uss+c33r*utt+c1r*ur+c2r*us+c3r*ut), !       ( 8.*((uxxp1+uyyp1+uzzp1)-(uxxm1+uyym1+uzzm1))-((uxxp2+uyyp2+uzzp2)-(uxxm2+uyym2+uzzm2)) )/(12.*dra)
                  write(*,'(" ")')  ! done this (i1,i2,i3)
                  maxDivc=max(maxDivc,divc)
                  maxTauDotLapu=max(maxTauDotLapu,tau1DotLap)
                  maxTauDotLapu=max(maxTauDotLapu,tau2DotLap)
                  maxExtrap=max(maxExtrap,tau1Up1)
                  maxExtrap=max(maxExtrap,tau2Up1)
                  ! maxDr3aDotU=max(maxDr3aDotU,g2a)
                 end if
                 end do
                 end do
                 end do
                  write(*,'(" ***bc4: grid=",i4,", side,axis=",2i3," 
     & maxDivc=",e8.1,", maxTauDotLapu=",e8.1,", maxExtrap=",e8.1,", 
     & maxDr3aDotU=",e8.1," ***** ",/)') grid,side,axis,maxDivc,
     & maxTauDotLapu,maxExtrap,maxDr3aDotU
                 end if ! end if forcing
                 ! ============================END DEBUG=======================================================
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
